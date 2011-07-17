#include "ExternalWidgetPicker.h"

ExternalWidgetPicker::ExternalWidgetPicker(TargetApplicationProxy* appProxy, QObject* parent)
: WidgetPicker(parent)
, m_appProxy(appProxy)
{
}

void ExternalWidgetPicker::start()
{
	ObjectProxy::Pointer proxy = m_appProxy->pickWidget();
	emit widgetPicked(proxy);
}

void ExternalWidgetPicker::cancel()
{
	// TODO
}

