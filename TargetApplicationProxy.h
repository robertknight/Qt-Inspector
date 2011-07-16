#pragma once

#include "MessageReader.h"

#include <QtCore/QHash>
#include <QtCore/QObject>

#include <QtNetwork/QLocalSocket>

class ExternalObjectProxy;
class ObjectProxy;

class QLocalSocket;

namespace service
{
	class InspectorRequest;
	class InspectorResponse;
	class QtObject;
}

/** Represents an external Qt process. */
class TargetApplicationProxy : public QObject
{
	Q_OBJECT

	public:
		TargetApplicationProxy();

		bool connectToTarget(int pid);
		void disconnect();

		/** Return a list of top-level widgets in the application. */
		QList<ObjectProxy*> fetchTopLevelWidgets();
		ObjectProxy* fetchProxy(int objectId);
		
		bool fetchObject(ExternalObjectProxy* proxy);

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

