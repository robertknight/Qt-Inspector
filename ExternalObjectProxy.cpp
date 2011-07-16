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

void ExternalObjectProxy::setProperty(const QString& name, const QVariant& value)
{
	m_properties.insert(name,value);
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

QVariant ExternalObjectProxy::readProperty(const QString& name) const
{
	doLoad();
	return m_properties.value(name);
}

void ExternalObjectProxy::writeProperty(const QString& name, const QVariant& value)
{
	doLoad();
	m_properties.insert(name,value);
}

QList<ObjectProxy*> ExternalObjectProxy::children()
{
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

QHash<QString,QVariant> ExternalObjectProxy::properties() const
{
	doLoad();
	return m_properties;
}

