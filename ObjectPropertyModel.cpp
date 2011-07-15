#include "ObjectPropertyModel.h"

#include <QtCore/QMetaProperty>
#include <QtDebug>

ObjectPropertyModel::ObjectPropertyModel(QObject* parent)
: QStandardItemModel(parent)
{
	setColumnCount(2);
	setObject(0);

	connect(this,SIGNAL(itemChanged(QStandardItem*)),
	        this,SLOT(updatePropertyValue(QStandardItem*)));
}

QVariant ObjectPropertyModel::toEditValue(const QVariant& value)
{
	// TODO - Find existing code which can handle display and editing
	// of common value types.
	switch (value.userType())
	{
		case QVariant::Size:
		case QVariant::SizeF:
			{
				QSize size = value.toSize();
				return QString("%1 x %2").arg(size.width()).arg(size.height());
			}
		case QVariant::Rect:
		case QVariant::RectF:
			{
				QRect rect = value.toRect();
				return QString("(%1,%2) (%3,%4)").arg(rect.left()).arg(rect.top()).arg(rect.width()).arg(rect.height());
			}
	}
	return value;
}

void ObjectPropertyModel::updatePropertyValue(QStandardItem* item)
{
	QStandardItem* propertyItem = this->item(item->row(),0);
	QString propertyName = propertyItem->text();

	if (m_object)
	{
		QVariant newValue = item->data(Qt::EditRole);
		m_object.data()->setProperty(propertyName.toLocal8Bit(),newValue);
	}
}

void ObjectPropertyModel::setObject(QObject* object)
{
	clear();

	setHorizontalHeaderLabels(
		QStringList() << tr("Property") << tr("Value")
	);

	m_object = object;

	if (object)
	{
		const QMetaObject* metaObject = object->metaObject();
		for (int i=0; i < metaObject->propertyCount(); i++)
		{
			QMetaProperty property = metaObject->property(i);

			QVariant value = property.read(object);
			QVariant editValue = toEditValue(value);

			QStandardItem* nameItem = new QStandardItem(property.name());
			nameItem->setEditable(false);

			QStandardItem* valueItem = new QStandardItem;
			valueItem->setData(editValue,Qt::EditRole);
			valueItem->setEditable(property.isWritable());

			insertRow(i,QList<QStandardItem*>() << nameItem << valueItem);
		}
	}
}

