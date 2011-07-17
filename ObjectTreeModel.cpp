#include "ObjectTreeModel.h"

#include "ObjectFilter.h"

#include <QtDebug>

ObjectTreeModel::ObjectTreeModel(QObject* parent)
: QAbstractItemModel(parent)
, m_filter(0)
{
}

void ObjectTreeModel::setFilter(ObjectFilter *filter)
{
	m_filter = filter;
}

int ObjectTreeModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return 1;
}

int ObjectTreeModel::rowCount(const QModelIndex& parent) const
{
	ObjectItem* parentItem = itemFromIndex(parent);
	if (!parentItem)
	{
		return m_roots.count();
	}
	return parentItem->children.count();
}

void ObjectTreeModel::setRootObjects(const QList<ObjectProxy::Pointer>& roots)
{
	qDeleteAll(m_roots);
	m_roots.clear();

	Q_FOREACH(ObjectProxy::Pointer root, roots)
	{
		if (! m_filter || m_filter->accepts(root))
		{
			ObjectItem* rootItem = createItem(root,0,m_filter);
			m_roots << rootItem;
		}
	}

	reset();
}

QList<ObjectProxy::Pointer> ObjectTreeModel::rootObjects() const
{
	QList<ObjectProxy::Pointer> objects;
	Q_FOREACH(ObjectItem* item, m_roots)
	{
		objects << item->object;
	}
	return objects;
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	ObjectItem* parentItem = itemFromIndex(parent);
	if (row < 0 || column != 0)
	{
		return QModelIndex();
	}

	QList<ObjectItem*> children;
	if (parentItem)
	{
		children = parentItem->children;
	}
	else
	{
		children = m_roots;
	}

	if (row >= children.count())
	{
		return QModelIndex();
	}
	else
	{
		return createIndex(row,column,children.at(row));
	}
}

int ObjectTreeModel::indexInParent(ObjectItem* item) const
{
	ObjectItem* parent = item->parent;
	int row;
	if (parent)
	{
		row = parent->children.indexOf(item);
	}
	else
	{
		row = m_roots.indexOf(item);
	}
	return row;
}

QModelIndex ObjectTreeModel::parent(const QModelIndex& index) const
{
	ObjectItem* item = itemFromIndex(index);
	if (!item)
	{
		return QModelIndex();
	}

	ObjectItem* parent = item->parent;
	if (!parent)
	{
		return QModelIndex();
	}

	return createIndex(indexInParent(parent),index.column(),parent);
}

QVariant ObjectTreeModel::data(const QModelIndex& index, int role) const
{
	ObjectItem* item = itemFromIndex(index);
	if (!item)
	{
		return QVariant();
	}
	
	ObjectProxy::Pointer object = item->object;

	switch (role)
	{
		case Qt::DisplayRole:
			if (!object)
			{
				return "[deleted]";
			}
			else
			{
				return displayText(object);
			}
		default:
			return QVariant();
	}
}

QString ObjectTreeModel::displayText(ObjectProxy::Pointer object) const
{
	QString text;
	text += object->className();

	if (!object->objectName().isEmpty())
	{
		text += QString("[%1]").arg(object->objectName());
	}

	return text;
}

ObjectProxy::Pointer ObjectTreeModel::objectFromIndex(const QModelIndex& index)
{
	return itemFromIndex(index)->object;
}

ObjectTreeModel::ObjectItem* ObjectTreeModel::itemFromIndex(const QModelIndex& index)
{
	return static_cast<ObjectItem*>(index.internalPointer());
}

QVariant ObjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return tr("Object");
	}
	else
	{
		return QAbstractItemModel::headerData(section,orientation,role);
	}
}

ObjectTreeModel::ObjectItem* ObjectTreeModel::createItem(ObjectProxy::Pointer object, ObjectItem* parent, ObjectFilter *filter)
{
	ObjectItem* item = new ObjectItem;
	item->parent = parent;
	item->object = object;

	Q_FOREACH(ObjectProxy::Pointer child, object->children())
	{
		if (! filter || filter->accepts(child))
		{ 
			item->children << createItem(child,item,filter);
		}
	}

	return item;
}

ObjectTreeModel::ObjectItem* ObjectTreeModel::index(ObjectProxy::Pointer object, const QList<ObjectItem*>& items) const
{
	Q_FOREACH(ObjectItem* item, items)
	{
		if (item->object == object)
		{
			return item;
		}

		ObjectItem* match = index(object,item->children);
		if (match)
		{
			return match;
		}
	}
	return 0;
}

QModelIndex ObjectTreeModel::index(ObjectProxy::Pointer object) const
{
	ObjectItem* item = index(object,m_roots);
	if (item)
	{
		return createIndex(indexInParent(item),0,item);
	}
	else
	{
		return QModelIndex();
	}
}

bool ObjectTreeModel::matches(ObjectProxy::Pointer object, const QString& query) const
{
	return displayText(object).contains(query,Qt::CaseInsensitive);
}

void ObjectTreeModel::search(QList<ObjectProxy::Pointer>* hits, ObjectItem* item, const QString& query) const
{
	ObjectProxy::Pointer object = item->object;
	if (!object)
	{
		return;
	}
	if (matches(object,query))
	{
		*hits << object;
	}
	Q_FOREACH(ObjectItem* child, item->children)
	{
		search(hits,child,query);
	}
}

QList<ObjectProxy::Pointer> ObjectTreeModel::search(const QString& query) const
{
	if (query.isEmpty())
	{
		return QList<ObjectProxy::Pointer>();
	}

	QList<ObjectProxy::Pointer> matches;
	Q_FOREACH(ObjectItem* item, m_roots)
	{
		search(&matches,item,query);
	}
	return matches;
}

