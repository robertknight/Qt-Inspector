#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QList>

class MessageReader
{
	public:
		MessageReader();

		void parse(const char* data, int length);
		void reset();

		int messageCount();
		QByteArray nextMessage();

	private:
		enum State
		{
			ParseHeader,
			ParseBody
		};
		QList<QByteArray> m_messages;

		State m_state;
		QByteArray m_header;
		QByteArray m_body;
		int m_bodyLength;
};


