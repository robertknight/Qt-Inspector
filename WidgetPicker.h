#pragma once

#include <QtCore/QObject>

class ObjectProxy;

class WidgetPicker : public QObject
{
	Q_OBJECT

	public:
		WidgetPicker(QObject* parent);

	public Q_SLOTS:
		virtual void start() = 0;
		virtual void cancel() = 0;

	Q_SIGNALS:
		void widgetPicked(ObjectProxy* widget);
};

