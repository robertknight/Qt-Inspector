#pragma once

#include "Injector.h"

#include <QtCore/QStringList>

class QProcess;
class QTextStream;

/** Injector which uses GDB to attach to an external process,
  * load a library it and then invoke a specified
  * entry point in the library.
  */
class GdbLibraryInjector : public Injector
{
	public:
		// implements Injector
		bool startAndInject(const QString& program, const QStringList& args,
		                    const QString& libraryPath, const QString& entryPoint,
		                    int* pid);
		bool inject(int pid, const QString& libraryPath, const QString& entryPoint);

	private:
		struct TargetInfo
		{
			TargetInfo()
			: pid(0)
			{}

			int pid;
			QString program;
			QStringList args;
		};

		bool inject(const TargetInfo& target, const QString& libraryPath, const QString& entryPoint,
		            int* outPid);

		void logToFile(QProcess* gdbProcess, QTextStream* stream);
		bool readGdbResponse(QTextStream* gdbStream, const QString& command, QStringList* lines);
};


