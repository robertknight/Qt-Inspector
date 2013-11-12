#include "StartupHelper.h"

#include <QtConcurrentRun>

#include <unistd.h>

void StartupHelper::runInitFunc()
{
	m_initFunc();
	emit startupComplete();
}

void StartupHelper::doWait()
{
	while (QCoreApplication::startingUp())
	{
		usleep(100 * 1000);
	}
	QMetaObject::invokeMethod(this, "runInitFunc", Qt::QueuedConnection);
}

void StartupHelper::watchForStartup()
{
	QtConcurrent::run(this, &StartupHelper::doWait);
}

