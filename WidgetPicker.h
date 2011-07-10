#pragma once

#include <QtCore/QObject>

class WidgetPicker : public QObject
{
	Q_OBJECT

	public:
		WidgetPicker(QObject* parent);

		virtual bool eventFilter(QObject* watched, QEvent* event);

	public Q_SLOTS:
		void start();
		void cancel();

	Q_SIGNALS:
		void widgetPicked(QWidget* widget);
};

