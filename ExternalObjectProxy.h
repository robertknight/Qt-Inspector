#pragma once

#include "ObjectProxy.h"

class TargetApplicationProxy;

/** Proxy representing a QObject in an external process. */
class ExternalObjectProxy : public ObjectProxy
{
	public:
		/** Construct a new proxy representing a QObject in the external process
		  * connected to @p appProxy.  @p objectId is the internal ID assigned
		  * by the external process to this object.
		  */
		ExternalObjectProxy(TargetApplicationProxy* appProxy, int objectId);

		// accessors used by TargetApplicationProxy to update the proxy
		// using data received from the target process
		int objectId() const;
		void setClassName(const QString& className);
		void setObjectName(const QString& objectName);
		void setChildIds(const QList<int>& children);
		void setAddress(quintptr address);
		void setLoaded(bool loaded);
		void addProperty(const Property& property);
		void removeProperty(const QString& name);

		// implements ObjectProxy
		virtual quintptr address() const;
		virtual QString className() const;
		virtual QString objectName() const;
		virtual QList<ObjectProxy::Pointer> children();
		virtual QList<Property> properties() const;
		virtual void writeProperty(const QString& name, const QVariant& value);

	private:
		// the proxy starts out as a flyweight.  This is a blocking call
		// to update the proxy's properties from the target process.
		bool doLoad() const;

		TargetApplicationProxy* m_appProxy;
		int m_objectId;
		bool m_isLoaded;

		quintptr m_address;
		QString m_className;
		QString m_objectName;
		QList<Property> m_properties;
		QList<int> m_childIds;

};

