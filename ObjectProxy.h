#pragma once

#include <QtCore/QHash>
#include <QtCore/QVariant>

/** Base class representing an object.  Depending on the sub-class,
  * the object may live in the same process or may
  * live in an external process.
  */
class ObjectProxy
{
	public:
		virtual ~ObjectProxy() {}

		virtual QString className() const = 0;
		virtual QString objectName() const = 0;
		virtual QVariant readProperty(const QString& name) const = 0;
		virtual void writeProperty(const QString& name, const QVariant& value) = 0;
		virtual QHash<QString,QVariant> properties() const = 0;
		virtual QList<ObjectProxy*> children() = 0;
};

