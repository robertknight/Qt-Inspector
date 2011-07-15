#pragma once

#include <QtGui/QShortcut>

class WidgetInspector;

class WidgetInspectorShortcut : public QShortcut
{
	Q_OBJECT

	public:
		WidgetInspectorShortcut(QWidget* parent);

	private Q_SLOTS:
		void showInspector();

	private:
		WidgetInspector* m_inspector;	
};

