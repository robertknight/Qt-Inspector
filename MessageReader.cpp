#include "MessageReader.h"

MessageReader::MessageReader()
{
	reset();
}

void MessageReader::reset()
{
	m_state = ParseHeader;
	m_header.clear();
	m_bodyLength = -1;
	m_body.clear();
}

void MessageReader::parse(const char* data, int length)
{
	int bytesRead = 0;
	while (bytesRead < length)
	{
		switch (m_state)
		{
			case ParseHeader:
				{
					for (;bytesRead < length; bytesRead++)
					{
						if (data[bytesRead] == '\n')
						{
							bool ok = false;
							m_bodyLength = m_header.toInt(&ok);
							m_header.clear();
							m_state = ParseBody;
							++bytesRead;
							break;
						}
						else
						{
							m_header.append(data[bytesRead]);
						}
					}
				}
				break;
			case ParseBody:
				{
					for (;bytesRead < length; bytesRead++)
					{
						m_body.append(data[bytesRead]);
						if (m_body.length() == m_bodyLength)
						{
							m_messages << m_body;
							++bytesRead;
							reset();
							break;
						}
					}
				}
				break;
		}
	}
}

int MessageReader::messageCount()
{
	return m_messages.count();
}

QByteArray MessageReader::nextMessage()
{
	return m_messages.takeFirst();
}

