#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

class OutOfProcessClipboard : public QObject
{
	Q_OBJECT

	public:
		OutOfProcessClipboard(QObject* parent);

		void setText(const QString& text);

	Q_SIGNALS:
		void error(const QString&);
};

