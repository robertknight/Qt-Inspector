#pragma once

#include "ObjectProxy.h"

#include <QtCore/QString>

class QWidget;

class ObjectFilter
{
	public:

		virtual ~ObjectFilter();
		virtual bool accepts(ObjectProxy::Pointer object) const = 0;
		QString name() const;


	protected:

		ObjectFilter(const QString& name);

		QString m_name;
};

class QWidgetFilter : public ObjectFilter
{
	public:
		QWidgetFilter();
		bool accepts(ObjectProxy::Pointer object) const;
};
