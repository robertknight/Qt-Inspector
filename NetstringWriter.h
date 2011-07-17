#pragma once

#include <QtCore/QByteArray>

/** NetstringWriter converts byte arrays into netstrings-format messages.
  * The original byte arrays can be unpacked from netstrings messages using
  * NetstringReader.
  */
class NetstringWriter
{
	public:
		NetstringWriter(QByteArray* buffer);

		void writeMessage(const char* data, int length);

		static QByteArray toMessage(const QByteArray& data);

	private:
		QByteArray* m_buffer;
};

