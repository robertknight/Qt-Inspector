#include "TargetApplicationProxy.h"

#include "GdbLibraryInjector.h"
#include "InspectorServer.h"
#include "MessageWriter.h"
#include "ObjectProxy.h"

#include <QtNetwork/QLocalSocket>

TargetApplicationProxy::TargetApplicationProxy()
: m_socket(new QLocalSocket(this))
{
}

bool TargetApplicationProxy::connect(int pid)
{
	// inject the helper library
	GdbLibraryInjector injector;
	if (!injector.inject(pid,"libQtInspector.so","qtInspectorInit"))
	{
		return false;
	}

	// connect to the socket set up by the
	// helper library
	m_socket->connectToServer(InspectorServer::socketName(pid));
	if (!m_socket->waitForConnected(1000))
	{
		qWarning() << "Failed to connect to local socket server in target process" << pid;
		return false;
	}

	m_socket->write(MessageWriter::toMessage("Hello from the connecting app"));

	return true;
}

void TargetApplicationProxy::disconnect()
{
	m_socket->disconnectFromServer();
}

QList<ObjectProxy*> TargetApplicationProxy::fetchTopLevelWidgets()
{
	QList<ObjectProxy*> objects;
	return objects;
}

