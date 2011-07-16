#include "TargetApplicationProxy.h"

#include "ExternalObjectProxy.h"
#include "GdbLibraryInjector.h"
#include "InspectorServer.h"
#include "MessageWriter.h"
#include "ObjectProxy.h"

#include "inspector.pb.h"

TargetApplicationProxy::TargetApplicationProxy()
: m_socket(new QLocalSocket(this))
{
	connect(m_socket,SIGNAL(error(QLocalSocket::LocalSocketError)),
	        this,SLOT(socketError(QLocalSocket::LocalSocketError)));
	connect(m_socket,SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),
	        this,SLOT(socketStateChanged(QLocalSocket::LocalSocketState)));
}

bool TargetApplicationProxy::connectToTarget(int pid)
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

	return true;
}

void TargetApplicationProxy::disconnect()
{
	m_socket->disconnectFromServer();
}

QList<ObjectProxy*> TargetApplicationProxy::fetchTopLevelWidgets()
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	QList<ObjectProxy*> objects;

	request.set_type(service::InspectorRequest::FetchTopLevelWidgetsRequest);
	if (!sendRequest(request,&response))
	{
		qWarning() << "Failed to send top-level widget request fetch to child process";
		return objects;
	}

	for (int i=0; i < response.object_size(); i++)
	{
		int id = response.object(i).id();
		ExternalObjectProxy* proxy = dynamic_cast<ExternalObjectProxy*>(fetchProxy(id));
		updateProxy(response.object(i),proxy);
		objects << proxy;
	}

	return objects;
}

ObjectProxy* TargetApplicationProxy::fetchProxy(int objectId)
{
	ExternalObjectProxy* proxy = m_objectProxies.value(objectId);
	if (proxy)
	{
		return proxy;
	}
	proxy = new ExternalObjectProxy(this,objectId);
	m_objectProxies.insert(objectId,proxy);
	return proxy;
}

bool TargetApplicationProxy::fetchObject(ExternalObjectProxy* proxy)
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	request.set_type(service::InspectorRequest::FetchObjectRequest);
	request.set_objectid(proxy->objectId());

	if (!sendRequest(request,&response))
	{
		qWarning() << "Failed to send object fetch request to child process";
		return false;
	}

	if (response.object_size() == 1)
	{
		service::QtObject object = response.object(0);
		updateProxy(object,proxy);
		return true;
	}
	else
	{
		qWarning() << "Expected to receive one object but received" << response.object_size();
		return false;
	}
}

void TargetApplicationProxy::updateProxy(const service::QtObject& object, ExternalObjectProxy* proxy)
{
	proxy->setLoaded(true);
	proxy->setClassName(QString::fromStdString(object.classname()));
	proxy->setObjectName(QString::fromStdString(object.objectname()));
	proxy->setAddress(object.address());
	
	for (int i=0; i < object.property_size(); i++)
	{
		ObjectProxy::Property property;
		property.name = QString::fromStdString(object.property(i).name());
		property.value = QVariant(QString::fromStdString(object.property(i).value()));
		property.isWritable = object.property(i).iswritable();
		proxy->addProperty(property);
	}

	QList<int> childIds;
	for (int i=0; i < object.childid_size(); i++)
	{
		childIds << object.childid(i);
	}
	proxy->setChildIds(childIds);
}

void TargetApplicationProxy::updateProperty(int objectId, const ObjectProxy::Property& property)
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	request.set_type(service::InspectorRequest::WritePropertyRequest);
	request.set_objectid(objectId);
	request.mutable_propertyupdate()->set_name(property.name.toStdString());
	request.mutable_propertyupdate()->set_value(property.value.toString().toStdString());
	request.mutable_propertyupdate()->set_iswritable(true);

	sendRequest(request,&response);
}

ObjectProxy* TargetApplicationProxy::pickWidget()
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	request.set_type(service::InspectorRequest::PickWidgetRequest);
	sendRequest(request,&response);

	if (response.object_size() == 1)
	{
		service::QtObject object = response.object(0);
		ExternalObjectProxy* proxy = dynamic_cast<ExternalObjectProxy*>(fetchProxy(object.id()));
		updateProxy(object,proxy);
		return proxy;
	}
	else
	{
		return 0;
	}
}

bool TargetApplicationProxy::sendRequest(const service::InspectorRequest& request,
                                         service::InspectorResponse* response)
{
	QByteArray requestData(request.ByteSize(),0);
	request.SerializeToArray(requestData.data(),requestData.count());
	QByteArray messageData = MessageWriter::toMessage(requestData);
	int bytesWritten = m_socket->write(messageData);
	if (bytesWritten != messageData.count())
	{
		qWarning() << "Tried to write" << messageData.count() << "bytes to helper but only sent" << bytesWritten;
	}
	m_socket->waitForBytesWritten();

	while (m_messageReader.messageCount() < 1)
	{
		m_socket->waitForReadyRead();
		QByteArray data = m_socket->readAll();
		m_messageReader.parse(data.constData(),data.count());
	}

	QByteArray responseData = m_messageReader.nextMessage();
	response->ParseFromArray(responseData.constData(),responseData.size());
	return true;
}

void TargetApplicationProxy::socketError(QLocalSocket::LocalSocketError error)
{
	qWarning() << "Local socket reported error" << error;
}

void TargetApplicationProxy::socketStateChanged(QLocalSocket::LocalSocketState state)
{
	qWarning() << "Local socket state changed" << state;
}

