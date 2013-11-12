#pragma once

#include <QtCore/QHash>
#include <QtCore/QPointer>

class ObjectIdMap
{
	public:
		ObjectIdMap();

		int addObject(QObject* object);
		QObject* getObject(int id);

	private:
		QHash<int,QPointer<QObject> > m_objects;
		int m_maxId;
};

