#pragma once

#include <QtGui/QStandardItemModel>
#include <QtCore/QWeakPointer>

class ObjectProxy;

class ObjectPropertyModel : public QStandardItemModel
{
	Q_OBJECT

	public:
		ObjectPropertyModel(QObject* parent);

		void setObject(ObjectProxy* object);

		static QVariant toEditValue(const QVariant& value);

	private Q_SLOTS:
		void updatePropertyValue(QStandardItem* item);

	private:
		ObjectProxy* m_object;
};


