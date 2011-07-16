#pragma once

#include "ObjectProxy.h"

class TargetApplicationProxy;

class ExternalObjectProxy : public ObjectProxy
{
	public:
		ExternalObjectProxy(TargetApplicationProxy* appProxy, int objectId);

		int objectId() const;
		void setClassName(const QString& className);
		void setObjectName(const QString& objectName);
		void setChildIds(const QList<int>& children);
		void setLoaded(bool loaded);
		void addProperty(const Property& property);
		void removeProperty(const QString& name);

		virtual QString className() const;
		virtual QString objectName() const;
		virtual QList<ObjectProxy*> children();
		virtual QList<Property> properties() const;
		virtual void writeProperty(const QString& name, const QVariant& value);

	private:
		bool doLoad() const;

		TargetApplicationProxy* m_appProxy;
		int m_objectId;
		bool m_isLoaded;

		QString m_className;
		QString m_objectName;
		QList<Property> m_properties;
		QList<int> m_childIds;

};

