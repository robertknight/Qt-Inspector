#include "WidgetPicker.h"

#include <QtGui/QApplication>
#include <QtGui/QWidget>

WidgetPicker::WidgetPicker(QObject* parent)
: QObject(parent)
{
}

void WidgetPicker::start()
{
	QCoreApplication::instance()->installEventFilter(this);
	QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void WidgetPicker::cancel()
{
	QApplication::restoreOverrideCursor();
	QCoreApplication::instance()->removeEventFilter(this);
}

bool WidgetPicker::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		QWidget* widget = static_cast<QWidget*>(watched);
		emit widgetPicked(widget);
		cancel();
		return true;
	}
	return false;
}


