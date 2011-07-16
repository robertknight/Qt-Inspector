#pragma once

#include "MessageReader.h"

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QTextStream>

class QLocalServer;

class InspectorServer : public QObject
{
	Q_OBJECT

	public:
		InspectorServer(QTextStream* log, QObject* parent);

		static QString socketName(int pid);

	private Q_SLOTS:
		void handleConnection();
		void readyRead();

	private:
		void processMessage(const QByteArray& message);

		QTextStream* m_log;
		QLocalServer* m_server;
		MessageReader m_messageReader;
};

