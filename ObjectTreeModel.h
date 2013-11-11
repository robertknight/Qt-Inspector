#pragma once

#include "lib/ObjectProxy.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QWeakPointer>

class ObjectFilter;

class ObjectTreeModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		ObjectTreeModel(QObject* parent);

		void setFilter(ObjectFilter *filter);
		void setRootObjects(const QList<ObjectProxy::Pointer>& roots);
		QList<ObjectProxy::Pointer> rootObjects() const;

		QList<ObjectProxy::Pointer> search(const QString& query) const;

		QModelIndex index(ObjectProxy::Pointer object) const;

		virtual int columnCount(const QModelIndex& parent) const;
		virtual int rowCount(const QModelIndex& parent) const;
		virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
		virtual QModelIndex parent(const QModelIndex& index) const;
		virtual QVariant data(const QModelIndex& index, int role) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

		static ObjectProxy::Pointer objectFromIndex(const QModelIndex& index);

	private:
		class ObjectItem
		{
			public:
				~ObjectItem()
				{
					qDeleteAll(children);
				}

				ObjectItem* parent;
				QList<ObjectItem*> children;
				ObjectProxy::Pointer object;
		};

		int indexInParent(ObjectItem* item) const;
		ObjectItem* index(ObjectProxy::Pointer object, const QList<ObjectItem*>& items) const;

		static ObjectItem* createItem(ObjectProxy::Pointer object, ObjectItem* parent, ObjectFilter *filter);
		static ObjectItem* itemFromIndex(const QModelIndex& index);

		bool matches(ObjectProxy::Pointer object, const QString& query) const;
		void search(QList<ObjectProxy::Pointer>* matches, ObjectItem* item, const QString& query) const;
		QString displayText(ObjectProxy::Pointer object) const;

		QList<ObjectItem*> m_roots;
		ObjectFilter* m_filter;
};

