#pragma once

#include "lib/ObjectProxy.h"

#include <QtGui/QWidget>

class QLabel;
class QLineEdit;
class QTableView;
class QSortFilterProxyModel;

class ObjectPropertyModel;

class ObjectInspector : public QWidget
{
	Q_OBJECT

	public:
		ObjectInspector(QWidget* parent);

		void setObject(ObjectProxy::Pointer object);
		ObjectProxy::Pointer object() const;

		static QString formatAddress(void* ptr);

	public Q_SLOTS:
		void refresh();

	private Q_SLOTS:
		void changeFilter(const QString& pattern);

	private:
		QLabel* m_nameLabel;
		QTableView* m_propertyView;
		ObjectProxy::Pointer m_currentObject;
		ObjectPropertyModel* m_model;
		QSortFilterProxyModel* m_propertySortModel;
		QLineEdit* m_propertyFilterEdit;
};

