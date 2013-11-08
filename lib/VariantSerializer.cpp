#include "VariantSerializer.h"

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QMutex>

class DataStreamBuffer
{
	public:
		DataStreamBuffer()
		: stream(&buffer)
		{
		}

		QBuffer buffer;
		QDataStream stream;
};

// constructing/destroying a new QBuffer and QDataStream on each call to encode()
// and decode() is expensive but profiling shows that most of the cost of encode()
// and decode() when processing QVariants for a typical widget tree
// is down to the QDataStream operators for complex data types
// (eg. QIcon, QPalette, QTransform etc.)
QByteArray VariantSerializer::encode(const QVariant& value)
{
	DataStreamBuffer dataStream;
	QByteArray result;
	dataStream.buffer.setBuffer(&result);
	dataStream.buffer.open(QIODevice::WriteOnly);
	dataStream.stream << value;
	dataStream.buffer.close();
	return result;
}

QVariant VariantSerializer::decode(const QByteArray& value)
{
	DataStreamBuffer dataStream;
	QVariant result;
	dataStream.buffer.setData(value);
	dataStream.buffer.open(QIODevice::ReadOnly);
	dataStream.stream >> result;
	dataStream.buffer.close();
	return result;
}

