#pragma once

#include <QtGui/QWidget>
#include <QtCore/QWeakPointer>

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

		void setObject(QObject* object);
		QObject* object() const;

		static QString formatAddress(void* ptr);

	private Q_SLOTS:
		void changeFilter(const QString& text);

	private:
		QLabel* m_nameLabel;
		QTableView* m_propertyView;
		QWeakPointer<QObject> m_currentObject;
		ObjectPropertyModel* m_model;
		QSortFilterProxyModel* m_propertySortModel;
		QLineEdit* m_propertyFilterEdit;
};

