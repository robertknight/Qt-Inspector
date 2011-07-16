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

		struct Property
		{
			Property()
			: isWritable(false)
			{
			}

			QString name;
			QVariant value;
			bool isWritable;
		};

		virtual QString className() const = 0;
		virtual QString objectName() const = 0;
		virtual QList<ObjectProxy*> children() = 0;
		virtual QList<Property> properties() const = 0;
		virtual void writeProperty(const QString& name, const QVariant& value) = 0;
};

