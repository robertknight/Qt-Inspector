#include "GdbLibraryInjector.h"

#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>

#include <QtDebug>

#include <dlfcn.h>

bool GdbLibraryInjector::inject(int pid, const QString& libraryPath, const QString& entryPoint)
{
	QFileInfo libraryInfo(libraryPath);
	if (!libraryInfo.exists())
	{
		qWarning() << "Library file" << libraryPath << "not found";
		return false;
	}

	// start gdb in MI mode with .gdbinit commands ignored so
	// that we have a clean state to attach to the process, inject the helper
	// library and invoke the entry point
	QScopedPointer<QProcess> gdbProcess(new QProcess);
	gdbProcess->start("gdb",QStringList() << "--interpreter=mi" << "-nx");
	if (!gdbProcess->waitForStarted())
	{
		qWarning() << "Failed to start gdb helper to attach to" << pid;
		return false;
	}

	QTextStream gdbStream(gdbProcess.data());

	gdbStream << "set auto-solib-add off\n"; // disable automatic shared library loading for
	                                       // faster attaching

	gdbStream << "set unwindonsignal on\n"; // in the event of the helper causing a segfault
	                                        // or uncaught exception,
	                                        // roll back to the previous state
	gdbStream << "set unwind-on-terminating-exception on\n";

	gdbStream << "attach " << QString::number(pid) << '\n';
	gdbStream << "sharedlibrary libc\n";
	gdbStream << "sharedlibrary libdl\n"; // load libdl library for dlopen()

	// load the library into the process and invoke the entry point
	QString flag_RTLD_NOW = QString::number(RTLD_NOW);
	gdbStream << "call dlopen(\"" << libraryInfo.absoluteFilePath() << "\"," << flag_RTLD_NOW << ")\n";

	// call dlerror() so that any problems loading the injected library
	// are logged
	gdbStream << "print ((char*)(dlerror()))\n";

	// load symbols for the helper library so that the entry point can
	// be invoked
	gdbStream << "sharedlibrary " << libraryInfo.absoluteFilePath() << '\n';
	gdbStream << "call " << entryPoint << "()\n";
	gdbStream << "detach\n";
	gdbStream << "quit\n";
	gdbStream.flush();

	if (!gdbProcess->waitForFinished())
	{
		qWarning() << "Failed to kill gdb helper attached to" << pid;
		return false;
	}
	if (gdbProcess->exitStatus() != QProcess::NormalExit)
	{
		qWarning() << "gdb helper process attached to" << pid << "crashed";
		return false;
	}

	QFile gdbLogFile("gdb.log");
	gdbLogFile.open(QIODevice::WriteOnly | QIODevice::Append);
	gdbLogFile.write(QString("== gdb output for process %1 ==\n").arg(pid).toUtf8());
	gdbLogFile.write(gdbProcess->readAll());

	return true;
}

