#pragma once

#include <QtCore/QString>

class PlatformUtils
{
    public:
        static void msleep(int ms);

		/** Returns the path to the library which provides the symbol
		 * at the given address.
		 */
		static QString binaryPath(void* symbol);
};
