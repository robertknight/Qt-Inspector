#pragma once

#include <QtCore/QStringList>

/** Interface for starting a target app and injecting
 * the inspector's helper library into it.
 */
class Injector
{
	public:
		virtual ~Injector() {}

		/** Start a process and inject a library into it.  The library will
		  * be injected when the application reaches the Qt event loop.
		  */
		virtual bool startAndInject(const QString& program, const QStringList& args,
		                    const QString& libraryPath, const QString& entryPoint,
		                    int* pid) = 0;
		virtual bool inject(int pid, const QString& libraryPath, const QString& entryPoint) = 0;

		static void waitForServerReady(int pid);
};

