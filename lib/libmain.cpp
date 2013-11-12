
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtNetwork/QLocalServer>

#include "../libQtInspectorExport.h"
#include "InspectorServer.h"
#include "StartupHelper.h"

#include <iostream>

#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

extern void qtInspectorInit();

class LogFile
{
	public:
		LogFile(const QString& fileName)
		{
			m_file.setFileName(fileName);
			m_file.open(QIODevice::WriteOnly);
			m_stream.setDevice(&m_file);
		}

		QTextStream& stream()
		{
			return m_stream;
		}

	private:
		QFile m_file;
		QTextStream m_stream;
};

#ifdef Q_OS_UNIX
 __attribute__((constructor)) void qtInspectorLibInit()
{
	StartupHelper* initHelper = new StartupHelper(qtInspectorInit);
	QObject::connect(initHelper, SIGNAL(startupComplete()), initHelper, SLOT(deleteLater()));
	initHelper->watchForStartup();
}
#endif

#ifdef Q_OS_WIN32
INT APIENTRY DllMain(HMODULE,DWORD,LPVOID)
{
    std::cout << "DllMain invoked in " << GetCurrentProcessId() << std::endl;
    return TRUE;
}

 extern "C" __declspec(dllexport) LRESULT WINAPI qtInspectorLibInit(int nCode, WPARAM wParam, LPARAM lParam)
{
	static bool used = false;
	if (!used)
	{
		std::cout << "invoking libinit func" << std::endl;
		used = true;
	}
	return CallNextHookEx(0 /* HHOOK - unused */, nCode, wParam, lParam);
}
#endif

static QScopedPointer<LogFile> logFile;

// entry point for the inspector helper library
void qtInspectorInit()
{
	QString appName = QFileInfo(QCoreApplication::applicationFilePath()).baseName();
	if (appName == "qtinspector")
	{
		return;
	}

	if (logFile)
	{
		logFile->stream() << "Hmm, we've already attached to this app\n";
		logFile->stream().flush();
		return;
	}
	
	QString logName = QDir::temp().filePath(QString("/tmp/inspector-log-%1").arg(QCoreApplication::applicationPid()));
	LogFile* log = new LogFile(logName);
	logFile.reset(log);
	logFile->stream() << "Hello from the injected app\n";

	new InspectorServer(&logFile->stream(),QCoreApplication::instance());

	logFile->stream().flush();
}

