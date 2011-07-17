#include "MessageWriter.h"

#include <QtCore/QString>

MessageWriter::MessageWriter(QByteArray* buffer)
: m_buffer(buffer)
{
}

void MessageWriter::writeMessage(const char* data, int length)
{
	QByteArray lengthString = QString::number(length).toAscii();
	m_buffer->append(lengthString);
	m_buffer->append(':');
	m_buffer->append(data,length);
	m_buffer->append(',');
}

QByteArray MessageWriter::toMessage(const QByteArray& data)
{
	QByteArray message;
	MessageWriter writer(&message);
	writer.writeMessage(data.constData(),data.count());
	return message;
}
