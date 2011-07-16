#include "ExternalObjectProxy.h"

#include "TargetApplicationProxy.h"

ExternalObjectProxy::ExternalObjectProxy(TargetApplicationProxy* appProxy, int objectId)
: m_appProxy(appProxy)
, m_objectId(objectId)
, m_isLoaded(false)
{
}

void ExternalObjectProxy::setLoaded(bool loaded)
{
	m_isLoaded = loaded;
}

int ExternalObjectProxy::objectId() const
{
	return m_objectId;
}

void ExternalObjectProxy::setClassName(const QString& className)
{
	m_className = className;
}

void ExternalObjectProxy::setObjectName(const QString& objectName)
{
	m_objectName = objectName;
}

void ExternalObjectProxy::addProperty(const Property& property)
{
	bool found = false;
	QMutableListIterator<Property> iter(m_properties);
	while (iter.hasNext())
	{
		if (iter.next().name == property.name)
		{
			iter.remove();
		}
	}

	m_properties << property;
}

void ExternalObjectProxy::setChildIds(const QList<int>& childIds)
{
	m_childIds = childIds;
}

QString ExternalObjectProxy::className() const
{
	doLoad();
	return m_className;
}

QString ExternalObjectProxy::objectName() const
{
	doLoad();
	return m_objectName;
}

void ExternalObjectProxy::writeProperty(const QString& name, const QVariant& value)
{
	doLoad();

	Property updatedProperty;

	QMutableListIterator<Property> iter(m_properties);
	while (iter.hasNext())
	{
		Property& property = iter.next();
		if (property.name == name)
		{
			property.value = value;
			updatedProperty = property;
		}
	}

	m_appProxy->updateProperty(m_objectId,updatedProperty);
}

QList<ObjectProxy*> ExternalObjectProxy::children()
{
	doLoad();
	QList<ObjectProxy*> result;
	Q_FOREACH(int childId, m_childIds)
	{
		result << m_appProxy->fetchProxy(childId);
	}
	return result;
}

bool ExternalObjectProxy::doLoad() const
{
	if (m_isLoaded)
	{
		return true;
	}
	return m_appProxy->fetchObject(const_cast<ExternalObjectProxy*>(this));
}

QList<ObjectProxy::Property> ExternalObjectProxy::properties() const
{
	doLoad();
	return m_properties;
}

void ExternalObjectProxy::setAddress(quintptr address)
{
	m_address = address;
}

quintptr ExternalObjectProxy::address() const
{
	doLoad();
	return m_address;
}

