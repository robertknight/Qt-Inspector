#pragma once

#include "libMendeleyExport.h"

#include <QtGui/QWidget>

class ObjectInspector;
class ObjectTreeModel;
class ObjectPropertyModel;
class ObjectProxy;
class OutOfProcessClipboard;
class WidgetPicker;

class QModelIndex;
class QTreeView;

class LIB_MENDELEY_EXPORT WidgetInspector : public QWidget
{
	Q_OBJECT

	public:
		WidgetInspector(QWidget* parent = 0);

		void setRootObjects(const QList<ObjectProxy*>& roots);

		static void registerGlobalShortcut(const QKeySequence& key, QWidget* parentWidget);

	private Q_SLOTS:
		void resetModel();
		void search(const QString& query);
		void pickerFinished(QWidget* widget);
		void selectionChanged(const QModelIndex& current, const QModelIndex& previous);
		void copyDebuggerReference();

	private:
		void select(ObjectProxy* object);

		ObjectTreeModel* m_objectModel;
		QTreeView* m_objectTree;
		WidgetPicker* m_picker;
		ObjectInspector* m_objectInspector;
		OutOfProcessClipboard* m_externalClipboard;
};

