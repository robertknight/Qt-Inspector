#include "DirectObjectProxy.h"

#include <QtCore/QMetaProperty>

DirectObjectProxy::DirectObjectProxy(QObject* object)
: m_object(object)
{
}

quintptr DirectObjectProxy::address() const
{
	return reinterpret_cast<quintptr>(m_object.data());
}

QString DirectObjectProxy::className() const
{
	if (m_object)
	{
		return m_object.data()->metaObject()->className();
	}
	else
	{
		return QString("[deleted]");
	}
}

QString DirectObjectProxy::objectName() const
{
	if (m_object)
	{
		return m_object.data()->objectName();
	}
	else
	{
		return QString();
	}
}

QList<ObjectProxy::Property> DirectObjectProxy::properties() const
{
	QList<Property> properties;
	if (!m_object)
	{
		return properties;
	}

	const QMetaObject* metaObject = m_object.data()->metaObject();
	for (int i=0; i < metaObject->propertyCount(); i++)
	{
		QMetaProperty metaProperty = metaObject->property(i);

		Property property;
		property.name = metaProperty.name();
		property.value = metaProperty.read(m_object.data());
		property.isWritable = metaProperty.isWritable();
		properties << property;
	}

	return properties;
}

void DirectObjectProxy::writeProperty(const QString& name, const QVariant& value)
{
	if (m_object)
	{
		m_object.data()->setProperty(name.toUtf8().constData(),value);
	}
}
		
QList<ObjectProxy::Pointer> DirectObjectProxy::children()
{
	// TODO
	return QList<ObjectProxy::Pointer>();
}

void DirectObjectProxy::refresh()
{
}

