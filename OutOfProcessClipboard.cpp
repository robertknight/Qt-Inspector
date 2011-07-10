#include "OutOfProcessClipboard.h"

#include <QtCore/QFile>

#include <QtDebug>

OutOfProcessClipboard::OutOfProcessClipboard(QObject* parent)
: QObject(parent)
{
}

void OutOfProcessClipboard::setText(const QString& text)
{
	QFile tempFile("/tmp/qt-widget-inspector-clipboard");
	tempFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	tempFile.write(text.toUtf8());
	tempFile.close();

	// start the xclip utility detached so that it is not an inferior
	// of the current process and hence will not be interrupted by gdb
	// if the user interrupts the current process
	if (!QProcess::startDetached("xclip",QStringList() << tempFile.fileName() << "-i" << "-selection" << "clipboard"))
	{
		emit error("There was a problem copying text with 'xclip', please check that it is installed.");
	}
}

