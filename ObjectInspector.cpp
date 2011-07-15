#include "ObjectInspector.h"

#include "ObjectPropertyModel.h"

#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

ObjectInspector::ObjectInspector(QWidget* parent)
: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);

	m_nameLabel = new QLabel(this);
	layout->addWidget(m_nameLabel);

	m_propertyView = new QTableView(this);
	m_propertyView->verticalHeader()->setVisible(false);
	m_propertyView->horizontalHeader()->setStretchLastSection(true);

	QHBoxLayout* filterLayout = new QHBoxLayout;
	QLabel* filterLabel = new QLabel(tr("Filter:"),this);
	m_propertyFilterEdit = new QLineEdit(this);
	connect(m_propertyFilterEdit,SIGNAL(textChanged(QString)),
	        this,SLOT(changeFilter(QString)));
	filterLayout->addWidget(filterLabel);
	filterLayout->addWidget(m_propertyFilterEdit);

	m_model = new ObjectPropertyModel(this);
	m_propertySortModel = new QSortFilterProxyModel(this);
	m_propertySortModel->setSourceModel(m_model);
	m_propertySortModel->sort(0,Qt::AscendingOrder);

	m_propertyView->setModel(m_propertySortModel);

	layout->addWidget(m_propertyView);
	layout->addLayout(filterLayout);

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

void ObjectInspector::changeFilter(const QString& text)
{
	m_propertySortModel->setFilterFixedString(text);
}

