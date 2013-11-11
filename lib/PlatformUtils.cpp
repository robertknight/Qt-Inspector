#include "PlatformUtils.h"

#include <QtCore/QThread>

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
