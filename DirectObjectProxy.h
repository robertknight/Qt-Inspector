#pragma once

#include "ObjectProxy.h"

class DirectObjectProxy : public ObjectProxy
{
	public:
		DirectObjectProxy(QObject* object);

		virtual QString className() const = 0;
		virtual QString objectName() const = 0;
		virtual QVariant readProperty(const QString& name) const = 0;
		virtual void writeProperty(const QString& name, const QVariant& value) = 0;

	private:
		QWeakPointer<QObject> m_object;
};


