#pragma once

#include "Injector.h"

/** An injector which uses the library-preload mechanism on Mac/Linux
 * to inject the helper lib into the process.
 *
 * This injector does not support attaching to existing processes.
 */
class PreloadInjector : public Injector
{
	public:
		// implements Injector
		virtual bool startAndInject(const QString& program, const QStringList& args,
		                    const QString& libraryPath, const QString& entryPoint, int* pid); 
		virtual bool inject(int pid, const QString& libraryPath, const QString& entryPoint);
};
