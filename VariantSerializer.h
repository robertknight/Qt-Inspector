#pragma once

#include <QtCore/QVariant>

class VariantSerializer
{
	public:
		static QByteArray encode(const QVariant& value);
		static QVariant decode(const QByteArray& value);
};

