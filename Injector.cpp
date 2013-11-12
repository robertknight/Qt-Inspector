#include "Injector.h"

#include "lib/InspectorServer.h"
#include "lib/PlatformUtils.h"

void Injector::waitForServerReady(int pid)
{
	QString socketPath = InspectorServer::socketName(pid);
	while (!QFile::exists(socketPath))
	{
#ifdef Q_OS_UNIX
		usleep(100 * 1000);
#endif
	}
}
