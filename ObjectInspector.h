#pragma once

#include <QtGui/QWidget>
#include <QtCore/QWeakPointer>

class QLabel;
class QTableView;

class ObjectPropertyModel;

class ObjectInspector : public QWidget
{
	Q_OBJECT

	public:
		ObjectInspector(QWidget* parent);

		void setObject(QObject* object);
		QObject* object() const;

		static QString formatAddress(void* ptr);

	private:
		QLabel* m_nameLabel;
		QTableView* m_propertyView;
		QWeakPointer<QObject> m_currentObject;
		ObjectPropertyModel* m_model;
};

