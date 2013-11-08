#pragma once

#include "WidgetPicker.h"

class DirectWidgetPicker : public WidgetPicker
{
	Q_OBJECT

	public:
		DirectWidgetPicker(QObject* parent);

		QWidget* lastPicked() const;

		virtual bool eventFilter(QObject* watched, QEvent* event);

	public Q_SLOTS:
		virtual void start();
		virtual void cancel();

	private:
		QWidget* m_lastPicked;
};
