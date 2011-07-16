#include "InspectorServer.h"

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QtCore/QCoreApplication>

InspectorServer::InspectorServer(QTextStream* log, QObject* parent)
: QObject(parent)
, m_log(log)
{
	m_server = new QLocalServer(this);
	connect(m_server,SIGNAL(newConnection()),
	        this,SLOT(handleConnection()));

	QString name = socketName(QCoreApplication::applicationPid());
	if (!m_server->listen(name))
	{
		*m_log << "Failed to listen on socket '" << name << "'"
		       << " error: " << m_server->errorString() << '\n';
	}
}

QString InspectorServer::socketName(int pid)
{
	return QString("qtInspector-%1").arg(pid);
}

void InspectorServer::handleConnection()
{
	*m_log << "received new connection\n";
	QLocalSocket* socket = m_server->nextPendingConnection();
	connect(socket,SIGNAL(readyRead()),
	        this,SLOT(readyRead()));
}

void InspectorServer::readyRead()
{
	QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
	QByteArray data = socket->readAll();

	*m_log << "received " << data.count() << " bytes\n";
	*m_log << "received data '" << data << "'\n";
	m_messageReader.parse(data.constData(),data.count());

	while (m_messageReader.messageCount() > 0)
	{
		QByteArray message = m_messageReader.nextMessage();
		processMessage(message);
	}
}

void InspectorServer::processMessage(const QByteArray& message)
{
	*m_log << "received message: " << QString(message);
	m_log->flush();
}

