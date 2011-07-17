#pragma once

#include "NetstringReader.h"
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
		QList<ObjectProxy::Pointer> fetchTopLevelWidgets();
		ObjectProxy::Pointer fetchProxy(int objectId);
		
		// helpers to issue requests to the target process
		bool fetchObject(ExternalObjectProxy* proxy);
		void updateProperty(int objectId, const ObjectProxy::Property& property);
		ObjectProxy::Pointer pickWidget();

		// implements RootObjectList

		/** Same as fetchTopLevelWidgets() */
		virtual QList<ObjectProxy::Pointer> rootObjects();

	private Q_SLOTS:
		void socketError(QLocalSocket::LocalSocketError error);
		void socketStateChanged(QLocalSocket::LocalSocketState state);

	private:
		void updateProxy(const service::QtObject& object, ExternalObjectProxy* proxy);
		bool sendRequest(const service::InspectorRequest& request,
                         service::InspectorResponse* response);

		QLocalSocket* m_socket;
		QHash<int,ObjectProxy::Pointer> m_objectProxies;
		NetstringReader m_messageReader;
};

