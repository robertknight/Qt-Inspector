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
		void setProperty(const QString& name, const QVariant& value);
		void setChildIds(const QList<int>& children);
		void setLoaded(bool loaded);

		virtual QString className() const;
		virtual QString objectName() const;
		virtual QVariant readProperty(const QString& name) const;
		virtual void writeProperty(const QString& name, const QVariant& value);
		virtual QList<ObjectProxy*> children();

	private:
		bool doLoad() const;

		TargetApplicationProxy* m_appProxy;
		int m_objectId;
		bool m_isLoaded;

		QString m_className;
		QString m_objectName;
		QHash<QString,QVariant> m_properties;
		QList<int> m_childIds;

};

