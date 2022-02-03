#pragma once

#include "lib/ObjectProxy.h"

#include <QWidget>

class ObjectInspector;
class ObjectTreeModel;
class ObjectPropertyModel;
class OutOfProcessClipboard;
class RootObjectList;
class WidgetPicker;

class QPushButton;
class QModelIndex;
class QTreeView;

class WidgetInspector : public QWidget
{
	Q_OBJECT

	public:
		WidgetInspector(RootObjectList* rootList, QWidget* parent = 0);

		/** Sets the picker used by the 'Pick Widget' button
		  * to select a widget in the application by clicking on it.
		  *
		  * The WidgetInspector takes ownership of the picker.
		  */
		void setWidgetPicker(WidgetPicker* picker);

		static void registerGlobalShortcut(const QKeySequence& key, QWidget* parentWidget);

	private Q_SLOTS:
		void refreshTree();
		void search(const QString& query);
		void pickerFinished(ObjectProxy::Pointer widget);
		void selectionChanged(const QModelIndex& current, const QModelIndex& previous);
		void copyDebuggerReference();
		void flashWidget();

	private:
		void select(ObjectProxy::Pointer object);

		RootObjectList* m_rootList;
		ObjectTreeModel* m_objectModel;
		QTreeView* m_objectTree;
		WidgetPicker* m_picker;
		ObjectInspector* m_objectInspector;
		OutOfProcessClipboard* m_externalClipboard;
		QPushButton* m_pickButton;
};

