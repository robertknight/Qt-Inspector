#pragma once

#include "MessageReader.h"
#include "ObjectProxy.h"
#include "RootObjectList.h"

#include <QtCore/QHash>
#include <QtCore/QObject>

#include <QtNetwork/QLocalSocket>

class ExternalObjectProxy;

class QLocalSocket;

namespace service
{
	class InspectorRequest;
	class InspectorResponse;
	class QtObject;
}

/** Represents an external Qt process. */
class TargetApplicationProxy : public QObject , public RootObjectList
{
	Q_OBJECT

	public:
		TargetApplicationProxy();

		bool connectToTarget(int pid);
		void disconnect();

		/** Return a list of top-level widgets in the application. */
		QList<ObjectProxy*> fetchTopLevelWidgets();
		ObjectProxy* fetchProxy(int objectId);
		
		// helpers to issue requests to the target process
		bool fetchObject(ExternalObjectProxy* proxy);
		void updateProperty(int objectId, const ObjectProxy::Property& property);
		ObjectProxy* pickWidget();

		// implements RootObjectList

		/** Same as fetchTopLevelWidgets() */
		QList<ObjectProxy*> rootObjects();

	private Q_SLOTS:
		void socketError(QLocalSocket::LocalSocketError error);
		void socketStateChanged(QLocalSocket::LocalSocketState state);

	private:
		void updateProxy(const service::QtObject& object, ExternalObjectProxy* proxy);
		bool sendRequest(const service::InspectorRequest& request,
                         service::InspectorResponse* response);

		QLocalSocket* m_socket;
		QHash<int,ExternalObjectProxy*> m_objectProxies;
		MessageReader m_messageReader;
};

