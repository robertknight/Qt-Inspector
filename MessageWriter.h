#pragma once

#include <QtCore/QByteArray>

class MessageWriter
{
	public:
		MessageWriter(QByteArray* buffer);

		void writeMessage(const char* data, int length);

		static QByteArray toMessage(const QByteArray& data);

	private:
		QByteArray* m_buffer;
};

