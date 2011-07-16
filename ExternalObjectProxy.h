#pragma once

#include "ObjectProxy.h"

class TargetApplicationProxy;

class ExternalObjectProxy : public ObjectProxy
{
	public:
		ExternalObjectProxy(TargetApplicationProxy* appProxy, int objectId);

		virtual QString className() const;
		virtual QString objectName() const;
		virtual QVariant readProperty(const QString& name) const;
		virtual void writeProperty(const QString& name, const QVariant& value);
		virtual QList<ObjectProxy*> children();
};

