#pragma once

#include "../libQtInspectorExport.h"
#include "ObjectProxy.h"

#include <QtCore/QObject>

class LIB_QTI_EXPORT WidgetPicker : public QObject
{
	Q_OBJECT

	public:
		WidgetPicker(QObject* parent);

	public Q_SLOTS:
		virtual void start() = 0;
		virtual void cancel() = 0;

	Q_SIGNALS:
		void widgetPicked(ObjectProxy::Pointer widget);
};

