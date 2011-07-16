#include "ObjectIdMap.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>

ObjectIdMap::ObjectIdMap()
: m_maxId(1)
{
}

int ObjectIdMap::addObject(QObject* object)
{
	if (!object)
	{
		return 0;
	}

	int id = object->property("INSPECTOR_ID").toInt();
	if (id == 0)
	{
		id = ++m_maxId;
		object->setProperty("INSPECTOR_ID",id);
		m_objects.insert(id,object);
	}

	return id;
}

QObject* ObjectIdMap::getObject(int id)
{
	return m_objects.value(id).data();
}

