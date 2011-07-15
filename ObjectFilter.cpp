#include "ObjectFilter.h"

#include <QtGui/QWidget>

ObjectFilter::ObjectFilter(const QString& name)
: m_name(name)
{
}


ObjectFilter::~ObjectFilter()
{
}

QString ObjectFilter::name() const
{
	return m_name;
}

QWidgetFilter::QWidgetFilter()
	: ObjectFilter("Widgets only")
{
}

bool QWidgetFilter::accepts(QObject *object) const
{
	QWidget* widget = qobject_cast<QWidget*>(object);
	return (widget != 0);
}
