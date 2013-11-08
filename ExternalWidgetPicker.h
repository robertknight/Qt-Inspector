#pragma once

#include "lib/WidgetPicker.h"
#include "TargetApplicationProxy.h"

class ExternalWidgetPicker : public WidgetPicker
{
	Q_OBJECT

	public:
		ExternalWidgetPicker(TargetApplicationProxy* appProxy, QObject* parent);

	public Q_SLOTS:
		virtual void start();
		virtual void cancel();

	private:
		TargetApplicationProxy* m_appProxy;
};

