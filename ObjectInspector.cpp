#include "ObjectInspector.h"

#include <QtGui/QLabel>
#include <QtGui/QStandardItemModel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTableView>
#include <QtCore/QMetaProperty>

class ObjectPropertyModel : public QStandardItemModel
{
	public:
		ObjectPropertyModel(QObject* parent);

		void setObject(QObject* object);
};

ObjectPropertyModel::ObjectPropertyModel(QObject* parent)
: QStandardItemModel(parent)
{
	setColumnCount(2);
	setObject(0);
}

void ObjectPropertyModel::setObject(QObject* object)
{
	clear();

	setHorizontalHeaderLabels(
		QStringList() << "Property" << "Value"
	);

	if (object)
	{
		const QMetaObject* metaObject = object->metaObject();
		for (int i=0; i < metaObject->propertyCount(); i++)
		{
			QMetaProperty property = metaObject->property(i);

			QStandardItem* nameItem = new QStandardItem(property.name());
			QStandardItem* valueItem = new QStandardItem(property.read(object).toString());
			insertRow(i,QList<QStandardItem*>() << nameItem << valueItem);
		}
	}
}

ObjectInspector::ObjectInspector(QWidget* parent)
: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);

	m_nameLabel = new QLabel(this);
	layout->addWidget(m_nameLabel);

	m_propertyView = new QTableView(this);
	m_model = new ObjectPropertyModel(this);
	m_propertyView->setModel(m_model);

	layout->addWidget(m_propertyView);

	setObject(0);
}

QString ObjectInspector::formatAddress(void* ptr)
{
	quintptr intPtr = reinterpret_cast<quintptr>(ptr);
	return QString("0x%1").arg(QString(QByteArray::number(intPtr,16)));
}

void ObjectInspector::setObject(QObject* object)
{
	QString labelText;
	
	if (object)
	{
		labelText = QString("%1 [%2] (%3)")
		  .arg(object->metaObject()->className())
		  .arg(object->objectName())
		  .arg(formatAddress(object));
	}
	else
	{
		labelText = tr("[No Object Selected]");
	}

	m_nameLabel->setText(labelText);
	m_currentObject = object;
	m_model->setObject(object);
}

QObject* ObjectInspector::object() const
{
	return m_currentObject.data();
}

