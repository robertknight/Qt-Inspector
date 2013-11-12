#include "DirectWidgetPicker.h"

#include <QApplication>
#include <QWidget>

DirectWidgetPicker::DirectWidgetPicker(QObject* parent)
: WidgetPicker(parent)
, m_lastPicked(0)
{
}

QWidget* DirectWidgetPicker::lastPicked() const
{
	return m_lastPicked;
}

void DirectWidgetPicker::start()
{
	QCoreApplication::instance()->installEventFilter(this);
	QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void DirectWidgetPicker::cancel()
{
	QApplication::restoreOverrideCursor();
	QCoreApplication::instance()->removeEventFilter(this);
}

bool DirectWidgetPicker::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		// TODO - Map to object proxy
		QWidget* widget = static_cast<QWidget*>(watched);
		m_lastPicked = widget;
		//emit widgetPicked(widget);
		emit widgetPicked(ObjectProxy::Pointer());

		cancel();
		return true;
	}
	return false;
}


