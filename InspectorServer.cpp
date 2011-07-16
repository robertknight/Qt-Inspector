#include "InspectorServer.h"

#include "MessageWriter.h"
#include "inspector.pb.h"

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QtCore/QCoreApplication>

InspectorServer::InspectorServer(QTextStream* log, QObject* parent)
: QObject(parent)
, m_log(log)
{
	m_server = new QLocalServer(this);
	connect(m_server,SIGNAL(newConnection()),
	        this,SLOT(handleConnection()));

	QString name = socketName(QCoreApplication::applicationPid());
	if (!m_server->listen(name))
	{
		*m_log << "Failed to listen on socket '" << name << "'"
		       << " error: " << m_server->errorString() << '\n';
	}
	else
	{
		*m_log << "Started listening on socket " << name << '\n';
	}
}

QString InspectorServer::socketName(int pid)
{
	return QString("qtInspector-%1").arg(pid);
}

void InspectorServer::handleConnection()
{
	*m_log << "received new connection\n";
	QLocalSocket* socket = m_server->nextPendingConnection();
	connect(socket,SIGNAL(readyRead()),
	        this,SLOT(readyRead()));
	if (socket->bytesAvailable() > 0)
	{
		readFromSocket(socket);
	}
}

void InspectorServer::readyRead()
{
	QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
	readFromSocket(socket);
}

void InspectorServer::readFromSocket(QLocalSocket* socket)
{
	QByteArray data = socket->readAll();

	*m_log << "received " << data.count() << " bytes\n";
	m_messageReader.parse(data.constData(),data.count());

	*m_log << "have " << m_messageReader.messageCount() << " requests pending\n";

	while (m_messageReader.messageCount() > 0)
	{
		QByteArray message = m_messageReader.nextMessage();

		service::InspectorRequest request;
		service::InspectorResponse response;

		request.ParseFromArray(message.constData(),message.count());
		handleRequest(request,&response);

		QByteArray responseData(response.ByteSize(),0);
		response.SerializeToArray(responseData.data(),responseData.count());

		socket->write(MessageWriter::toMessage(responseData));

		*m_log << "wrote " << responseData.count() << " bytes in response";
	}
}

void InspectorServer::updateObjectMessage(QObject* object, service::QtObject* message)
{
	int id = m_objectMap.addObject(object);
	message->set_id(id);
	message->set_classname(object->metaObject()->className());
	message->set_objectname(object->objectName().toStdString());

	Q_FOREACH(QObject* child, object->children())
	{
		int childId = m_objectMap.addObject(child);
		message->add_childid(childId);
	}
}

void InspectorServer::handleRequest(const service::InspectorRequest& request,
                                    service::InspectorResponse* response)
{
	response->set_request(request.type());
	switch (request.type())
	{
		case service::InspectorRequest::FetchTopLevelWidgetsRequest:
			{
				QList<QWidget*> widgets = QApplication::topLevelWidgets();
				Q_FOREACH(QObject* object, widgets)
				{
					service::QtObject* objectMessage = response->add_object();
					updateObjectMessage(object,objectMessage);
				}
			}
			break;
		case service::InspectorRequest::FetchObjectRequest:
			{
				QObject* object = m_objectMap.getObject(request.objectid());
				service::QtObject* objectMessage = response->add_object();
				updateObjectMessage(object,objectMessage);
			}
			break;
	};
}
