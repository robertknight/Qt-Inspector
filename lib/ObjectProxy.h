#pragma once

#include <QtCore/QHash>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

/** Base class representing an object.  Depending on the sub-class,
  * the object may live in the same process or may
  * live in an external process.
  */
class ObjectProxy
{
	public:
		typedef QSharedPointer<ObjectProxy> Pointer;

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

		/** Returns the address of the QObject which this proxy represents,
		  * in the process where the QObject lives.
		  */
		virtual quintptr address() const = 0;

		/** Returns the class name (as returned by QMetaObject::className()) of the object. */
		virtual QString className() const = 0;
		virtual QString objectName() const = 0;

		/** Returns a list of proxies representing the child objects of this QObject. */
		virtual QList<ObjectProxy::Pointer> children() = 0;

		/** Returns a list of properties and their current values in the object. */
		virtual QList<Property> properties() const = 0;

		/** Change the value of a property to @p value. */
		virtual void writeProperty(const QString& name, const QVariant& value) = 0;

		/** Clears any cached property values in this proxy. */
		virtual void refresh() = 0;
};

