#pragma once

#include <QtGui/QWidget>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;

class ObjectPropertyModel;
class ObjectProxy;

class ObjectInspector : public QWidget
{
	Q_OBJECT

	public:
		ObjectInspector(QWidget* parent);

		void setObject(ObjectProxy* object);
		ObjectProxy* object() const;

		static QString formatAddress(void* ptr);

	private Q_SLOTS:
		void changeFilter(const QString& text);

	private:
		QLabel* m_nameLabel;
		QTableView* m_propertyView;
		ObjectProxy* m_currentObject;
		ObjectPropertyModel* m_model;
		QSortFilterProxyModel* m_propertySortModel;
		QLineEdit* m_propertyFilterEdit;
};

