#pragma once

#include <QtCore/QString>

class ObjectProxy;
class QWidget;

class ObjectFilter
{
	public:

		virtual ~ObjectFilter();
		virtual bool accepts(ObjectProxy *object) const = 0;
		QString name() const;


	protected:

		ObjectFilter(const QString& name);

		QString m_name;
};

class QWidgetFilter : public ObjectFilter
{
	public:

		QWidgetFilter();
		bool accepts(ObjectProxy *object) const;
};
