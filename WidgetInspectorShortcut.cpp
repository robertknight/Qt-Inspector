#include "WidgetInspectorShortcut.h"

#include "WidgetInspector.h"

WidgetInspectorShortcut::WidgetInspectorShortcut(QWidget* parent)
: QShortcut(parent)
, m_inspector(0)
{
	connect(this,SIGNAL(activated()),this,SLOT(showInspector()));
}

void WidgetInspectorShortcut::showInspector()
{
	if (!m_inspector)
	{
		// TODO - Re-implement me for in-process usage
		//m_inspector = new WidgetInspector;
		m_inspector = 0;
	}
	m_inspector->show();
}

