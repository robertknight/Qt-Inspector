#include "PlatformUtils.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QThread>

#ifdef Q_OS_UNIX
#include <dlfcn.h>
#endif

class PlatformUtilsThread : public QThread
{
    public:
        // expose protected QThread::msleep() method
        static void msleep(int ms)
        {
            QThread::msleep(ms);
        }
};

void PlatformUtils::msleep(int ms)
{
    PlatformUtilsThread::msleep(ms);
}
		
QString PlatformUtils::binaryPath(void* symbol)
{
#ifdef Q_OS_UNIX
	Dl_info symInfo;
	if (dladdr(symbol, &symInfo) != 0)
	{
		return QFile::decodeName(symInfo.dli_fname);
	}
#endif
	return QString();
}
