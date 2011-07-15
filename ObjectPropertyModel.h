#pragma once

#include <QtGui/QStandardItemModel>
#include <QtCore/QWeakPointer>

class ObjectPropertyModel : public QStandardItemModel
{
	Q_OBJECT

	public:
		ObjectPropertyModel(QObject* parent);

		void setObject(QObject* object);

		static QVariant toEditValue(const QVariant& value);

	private Q_SLOTS:
		void updatePropertyValue(QStandardItem* item);

	private:
		QWeakPointer<QObject> m_object;
};


