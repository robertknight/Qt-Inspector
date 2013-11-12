#include "NetstringWriter.h"

#include <QtCore/QString>

NetstringWriter::NetstringWriter(QByteArray* buffer)
: m_buffer(buffer)
{
}

void NetstringWriter::writeMessage(const char* data, int length)
{
	QByteArray lengthString = QString::number(length).toUtf8();
	m_buffer->append(lengthString);
	m_buffer->append(':');
	m_buffer->append(data,length);
	m_buffer->append(',');
}

QByteArray NetstringWriter::toMessage(const QByteArray& data)
{
	QByteArray message;
	NetstringWriter writer(&message);
	writer.writeMessage(data.constData(),data.count());
	return message;
}
