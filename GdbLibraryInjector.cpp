#include "GdbLibraryInjector.h"

#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>

#include <QtDebug>

#include <dlfcn.h>

void GdbLibraryInjector::logToFile(QProcess* gdbProcess, QTextStream* stream)
{
	QFile gdbLogFile("gdb.log");
	gdbLogFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream logStream(&gdbLogFile);
	logStream << QString("== gdb output for process %1 ==\n").arg(gdbProcess->pid());
	logStream << stream->readAll();
}

bool GdbLibraryInjector::readGdbResponse(QTextStream* gdbStream, const QString& command, QStringList* lines)
{
	QString commandStart = QString("&\"%1").arg(command);

	bool readingCommand = false;
	while (true)
	{
		if (gdbStream->atEnd())
		{
			if (!gdbStream->device()->waitForReadyRead(5000))
			{
				qWarning() << "Failed to read stream";
				return false;
			}
		}
		QString line = gdbStream->readLine();
		if (line.startsWith(commandStart))
		{
			readingCommand = true;
		}
		else if (line.startsWith('^') && readingCommand)
		{
			return true;
		}

		if (readingCommand)
		{
			*lines << line;
		}
	}
}

bool GdbLibraryInjector::startAndInject(const QString& program, const QStringList& args,
		                                const QString& libraryPath, const QString& entryPoint,
                                        int* outPid)
{
	TargetInfo target;
	target.program = program;
	target.args = args;
	return inject(target,libraryPath,entryPoint,outPid);
}

bool GdbLibraryInjector::inject(int pid, const QString& libraryPath, const QString& entryPoint)
{
	TargetInfo target;
	target.pid = pid;
	return inject(target,libraryPath,entryPoint,0);
}

bool GdbLibraryInjector::inject(const TargetInfo& target, const QString& libraryPath, const QString& entryPoint,
                                int* outPid)
{
	int pid = target.pid;
	QString fullLibraryPath;

	QFileInfo libraryInfo(libraryPath);
	if (libraryInfo.exists()) {
		fullLibraryPath = libraryInfo.absoluteFilePath();
	} else {
		fullLibraryPath = libraryPath;
	}

	QLibrary library(fullLibraryPath);
	if (!library.load())
	{
		qWarning() << "Failed to load" << fullLibraryPath << ":" << library.errorString();
		return false;
	}

	QStringList programArgs;
	if (target.pid == 0)
	{
		programArgs << "--args";
		programArgs << target.program;
		programArgs += target.args;
	}

	// start gdb in MI mode with .gdbinit commands ignored so
	// that we have a clean state to attach to the process, inject the helper
	// library and invoke the entry point
	QScopedPointer<QProcess> gdbProcess(new QProcess);

	QStringList gdbArgs;
	gdbArgs << "--interpreter=mi";
	gdbArgs << "-nx";
	gdbArgs += programArgs;

	gdbProcess->start("gdb",gdbArgs);
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

	if (target.pid != 0)
	{
		gdbStream << "attach " << QString::number(pid) << '\n';
	}
	else
	{
		gdbStream << "br _start\n";
		gdbStream << "run\n";
		gdbStream << "info proc\n";
		gdbStream.flush();

		QStringList procInfo;
		if (!readGdbResponse(&gdbStream,"info proc",&procInfo))
		{
			qWarning() << "Unable to get information from newly minted process";
			return false;
		}

		QRegExp processLine("process ([0-9]+)");
		Q_FOREACH(const QString& line, procInfo)
		{
			if (processLine.indexIn(line) != -1)
			{
				*outPid = processLine.capturedTexts()[1].toInt();
				pid = *outPid;
			}
		}

		// wait for the application to reach the Qt event
		// loop before we try to inject the helper library
		gdbStream << "sharedlibrary QtCore\n";
		gdbStream << "br QCoreApplication::exec\n";
		gdbStream << "continue\n";
	}

	gdbStream << "sharedlibrary libc\n";
	gdbStream << "sharedlibrary libdl\n"; // load libdl library for dlopen()

	// load the library into the process and invoke the entry point
	QString flag_RTLD_NOW = QString::number(RTLD_NOW);
	gdbStream << "call dlopen(\"" << fullLibraryPath << "\"," << flag_RTLD_NOW << ")\n";

	// call dlerror() so that any problems loading the injected library
	// are logged
	gdbStream << "print ((char*)(dlerror()))\n";

	// load symbols for the helper library so that the entry point can
	// be invoked
	gdbStream << "sharedlibrary " << fullLibraryPath << '\n';
	gdbStream << "call " << entryPoint << "()\n";
	gdbStream << "detach\n";
	gdbStream << "quit\n";
	gdbStream.flush();

	if (!gdbProcess->waitForFinished())
	{
		qWarning() << "Failed to kill gdb helper attached to" << pid;
		logToFile(gdbProcess.data(),&gdbStream);
		return false;
	}
	if (gdbProcess->exitStatus() != QProcess::NormalExit)
	{
		qWarning() << "gdb helper process attached to" << pid << "crashed";
		logToFile(gdbProcess.data(),&gdbStream);
		return false;
	}
	logToFile(gdbProcess.data(),&gdbStream);

	return true;
}

