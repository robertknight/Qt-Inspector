#include "WindowsHookInjector.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

#include <Windows.h>

bool WindowsHookInjector::startAndInject(const QString& program, const QStringList& args, const QString& libraryPath, const QString& entryPoint, int* pid)
{
    *pid = 0;

    QProcess* process = new QProcess;
    QObject::connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), process, SLOT(deleteLater()));
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(program, args);
    if (!process->waitForStarted())
    {
        return false;
    }

    *pid = process->pid()->dwProcessId;

    return inject(process->pid()->dwThreadId, libraryPath, entryPoint);
}

bool WindowsHookInjector::inject(int pid, const QString& libraryPath, const QString& entryPoint)
{
    DWORD threadId = static_cast<DWORD>(pid);

    Q_UNUSED(entryPoint);

    QString libPath = QFileInfo(libraryPath).absoluteFilePath();
    if (!QFile::exists(libPath))
    {
        qWarning() << "Failed to find injector library" << libraryPath;
        return false;
    }

    HMODULE injectorLib = LoadLibrary(libraryPath.utf16());
    if (injectorLib == 0)
    {
        qWarning() << "loading injector lib failed" << GetLastError();
        return false;
    }
    HOOKPROC hookProc = reinterpret_cast<HOOKPROC>(GetProcAddress(injectorLib, QFile::encodeName(entryPoint).constData()));
    if (hookProc == 0)
    {
        qWarning() << "resolving injector entry point failed" << GetLastError();
        return false;
    }

    qDebug() << "hook proc" << hookProc << "injector lib" << injectorLib << "thread ID" << threadId;

    if (threadId == 0)
    {
        qWarning() << "failed to determine thread ID";
        return false;
    }

    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD, hookProc, injectorLib, 0);
    if (hook != 0)
    {
        waitForServerReady(pid);
        UnhookWindowsHookEx(hook);

        return true;
    }
    else
    {
        qWarning() << "hook setup failed" << GetLastError();
        return false;
    }
}
