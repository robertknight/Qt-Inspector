#pragma once

#include "libMendeleyExport.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QWeakPointer>

class ObjectFilter;

class LIB_MENDELEY_EXPORT ObjectTreeModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		ObjectTreeModel(QObject* parent);

		void setFilter(ObjectFilter *filter);
		void setRootObjects(const QList<QObject*>& roots);
		QList<QObject*> rootObjects() const;

		QList<QObject*> search(const QString& query) const;

		QModelIndex index(QObject* object) const;

		virtual int columnCount(const QModelIndex& parent) const;
		virtual int rowCount(const QModelIndex& parent) const;
		virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
		virtual QModelIndex parent(const QModelIndex& index) const;
		virtual QVariant data(const QModelIndex& index, int role) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

		static QObject* objectFromIndex(const QModelIndex& index);

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
				QWeakPointer<QObject> object;
		};

		int indexInParent(ObjectItem* item) const;
		ObjectItem* index(QObject* object, const QList<ObjectItem*>& items) const;

		static ObjectItem* createItem(QObject* object, ObjectItem* parent, ObjectFilter *filter);
		static ObjectItem* itemFromIndex(const QModelIndex& index);

		bool matches(QObject* object, const QString& query) const;
		void search(QList<QObject*>* matches, ObjectItem* item, const QString& query) const;
		QString displayText(QObject* object) const;

		QList<ObjectItem*> m_roots;
		ObjectFilter* m_filter;
};

