#include "StartupHelper.h"

#include "PlatformUtils.h"

#include <QtConcurrentRun>

void StartupHelper::runInitFunc()
{
	m_initFunc();
	emit startupComplete();
}

void StartupHelper::doWait()
{
	while (QCoreApplication::startingUp())
	{
		PlatformUtils::msleep(100);
	}
	QMetaObject::invokeMethod(this, "runInitFunc", Qt::QueuedConnection);
}

void StartupHelper::watchForStartup()
{
	QtConcurrent::run(this, &StartupHelper::doWait);
}

