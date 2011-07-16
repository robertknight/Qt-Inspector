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

bool QWidgetFilter::accepts(ObjectProxy* object) const
{
	// to-do - re-implement me with support for inheritance tests
	return false;
}
