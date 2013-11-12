#pragma once

#include "../libQtInspectorExport.h"

#include <QtCore/QVariant>

class LIB_QTI_EXPORT VariantSerializer
{
	public:
		static QByteArray encode(const QVariant& value);
		static QVariant decode(const QByteArray& value);
};

