#pragma once

#include <QtCore/QString>

/** Injector which uses GDB to attach to an external process,
  * load a library it and then invoke a specified
  * entry point in the library.
  */
class GdbLibraryInjector
{
	public:
		bool inject(int pid, const QString& libraryPath, const QString& entryPoint);
};


