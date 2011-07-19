#include "TargetApplicationProxy.h"

#include "ExternalObjectProxy.h"
#include "GdbLibraryInjector.h"
#include "InspectorServer.h"
#include "NetstringWriter.h"
#include "ObjectProxy.h"
#include "VariantSerializer.h"

#include "inspector.pb.h"

#include <QtCore/QTime>

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
	// connect to the socket set up by the
	// helper library
	m_socket->connectToServer(InspectorServer::socketName(pid));
	if (!m_socket->waitForConnected(3000))
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

QList<ObjectProxy::Pointer> TargetApplicationProxy::fetchTopLevelWidgets()
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	QList<ObjectProxy::Pointer> objects;

	request.set_type(service::InspectorRequest::FetchObjectTree);
	if (!sendRequest(request,&response))
	{
		qWarning() << "Failed to send top-level widget request fetch to child process";
		return objects;
	}

	for (int i=0; i < response.object_size(); i++)
	{
		int id = response.object(i).id();
		ObjectProxy::Pointer proxy = fetchProxy(id);
		updateProxy(response.object(i),proxy.dynamicCast<ExternalObjectProxy>().data());
	}

	for (int i=0; i < response.rootobjectid_size(); i++)
	{
		int id = response.rootobjectid(i);
		objects << fetchProxy(id);
	}

	return objects;
}

ObjectProxy::Pointer TargetApplicationProxy::fetchProxy(int objectId)
{
	ExternalObjectProxy::Pointer proxy = m_objectProxies.value(objectId);
	if (proxy)
	{
		return proxy;
	}
	proxy = ObjectProxy::Pointer(new ExternalObjectProxy(this,objectId));
	m_objectProxies.insert(objectId,proxy);
	return proxy;
}

bool TargetApplicationProxy::fetchObject(ExternalObjectProxy* proxy, bool fetchProperties)
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	request.set_type(service::InspectorRequest::FetchObjectRequest);
	request.set_objectid(proxy->objectId());
	request.set_fetchproperties(fetchProperties);

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
		proxy->setPropertiesLoaded(true);

		ObjectProxy::Property property;
		property.name = QString::fromStdString(object.property(i).name());

		QByteArray propertyBytes(object.property(i).value().c_str(),object.property(i).value().size());
		property.value = VariantSerializer::decode(propertyBytes);
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

	QByteArray valueBuffer = VariantSerializer::encode(property.value);
	request.mutable_propertyupdate()->set_value(std::string(valueBuffer.constData(),valueBuffer.count()));
	request.mutable_propertyupdate()->set_iswritable(true);

	sendRequest(request,&response);
}

ObjectProxy::Pointer TargetApplicationProxy::pickWidget()
{
	service::InspectorRequest request;
	service::InspectorResponse response;

	request.set_type(service::InspectorRequest::PickWidgetRequest);
	sendRequest(request,&response);

	if (response.object_size() == 1)
	{
		service::QtObject object = response.object(0);
		ObjectProxy::Pointer proxy = fetchProxy(object.id());
		updateProxy(object,proxy.dynamicCast<ExternalObjectProxy>().data());
		return proxy;
	}
	else
	{
		return ObjectProxy::Pointer();
	}
}

bool TargetApplicationProxy::sendRequest(const service::InspectorRequest& request,
                                         service::InspectorResponse* response)
{
	if (m_socket->state() != QLocalSocket::ConnectedState)
	{
		return false;
	}

	QByteArray requestData(request.ByteSize(),0);
	request.SerializeToArray(requestData.data(),requestData.count());
	QByteArray messageData = NetstringWriter::toMessage(requestData);
	int bytesWritten = m_socket->write(messageData);
	if (bytesWritten != messageData.count())
	{
		qWarning() << "Tried to write" << messageData.count() << "bytes to helper but only sent" << bytesWritten;
	}
	m_socket->waitForBytesWritten();

	int maxDelay = 10000;
	while (m_messageReader.messageCount() < 1 &&
	       m_socket->state() == QLocalSocket::ConnectedState)
	{
		if (!m_socket->waitForReadyRead(maxDelay))
		{
			qWarning() << "Failed to read response from process";
			return false;
		}
		QByteArray data = m_socket->readAll();
		m_messageReader.parse(data.constData(),data.count());
	}
	
	if (m_messageReader.messageCount() < 1)
	{
		return false;
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

QList<ObjectProxy::Pointer> TargetApplicationProxy::rootObjects()
{
	return fetchTopLevelWidgets();
}

