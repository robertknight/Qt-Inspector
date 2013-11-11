#pragma once

#include "../libQtInspectorExport.h"

#include <QtCore/QByteArray>
#include <QtCore/QList>

/** NetstringReader parses messages in netstrings format
  * from a byte stream.
  */
class LIB_QTI_EXPORT NetstringReader
{
	public:
		NetstringReader();

		void parse(const char* data, int length);
		void reset();

		int messageCount();
		QByteArray nextMessage();

	private:
		enum State
		{
			ParseHeader,
			ParseBody,
			ParseTrailer
		};
		QList<QByteArray> m_messages;

		State m_state;
		QByteArray m_header;
		QByteArray m_body;
		int m_bodyLength;
};


