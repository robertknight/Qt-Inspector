#pragma once

#include "../libQtInspectorExport.h"
#include "NetstringReader.h"
#include "ObjectIdMap.h"

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QTextStream>

class QLocalServer;
class QLocalSocket;

namespace service
{
	class InspectorRequest;
	class InspectorResponse;
	class QtObject;
	class QtObject_Property;
};

/** Entry point for the QtInspector library */
void LIB_QTI_EXPORT qtInspectorInit();

class LIB_QTI_EXPORT InspectorServer : public QObject
{
	Q_OBJECT

	public:
		InspectorServer(QTextStream* log, QObject* parent);

		static QString socketName(int pid);

	private Q_SLOTS:
		void handleConnection();
		void readyRead();

	private:
		void readFromSocket(QLocalSocket* socket);
		void handleRequest(const service::InspectorRequest& request,
                           service::InspectorResponse* response);
		void updateObjectMessage(QObject* object, service::QtObject* message,
		                         bool fetchProperties);
		void updateObjectProperty(QObject* object, const service::QtObject_Property& property);
		QObject* pickWidget();
		void fetchObjectTree(QList<int>* ids, QObject* object);

		QTextStream* m_log;
		QLocalServer* m_server;
		NetstringReader m_messageReader;
		ObjectIdMap m_objectMap;
};

