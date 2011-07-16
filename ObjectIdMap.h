#pragma once

#include <QtCore/QHash>
#include <QtCore/QWeakPointer>

class ObjectIdMap
{
	public:
		ObjectIdMap();

		int addObject(QObject* object);
		QObject* getObject(int id);

	private:
		QHash<int,QWeakPointer<QObject> > m_objects;
		int m_maxId;
};

