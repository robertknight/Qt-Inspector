#pragma once

#include "ObjectProxy.h"

#include <QtCore/QPointer>

class DirectObjectProxy : public ObjectProxy
{
	public:
		DirectObjectProxy(QObject* object);

		virtual quintptr address() const;
		virtual QString className() const;
		virtual QString objectName() const;
		virtual QList<ObjectProxy::Pointer> children();

		virtual QList<Property> properties() const;
		virtual void writeProperty(const QString& name, const QVariant& value);
		virtual void refresh();

	private:
		QPointer<QObject> m_object;
};


