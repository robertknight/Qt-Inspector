#pragma once

#include "lib/ObjectProxy.h"

#include <QtGui/QStandardItemModel>
#include <QtCore/QWeakPointer>

class ObjectPropertyModel : public QStandardItemModel
{
	Q_OBJECT

	public:
		ObjectPropertyModel(QObject* parent);

		void setObject(ObjectProxy::Pointer object);
		ObjectProxy::Pointer object() const;

		static QVariant toEditValue(const QVariant& value);

	private Q_SLOTS:
		void updatePropertyValue(QStandardItem* item);

	private:
		ObjectProxy::Pointer m_object;
};


