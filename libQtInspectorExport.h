#pragma once

#include <QtGlobal>

#ifdef LIB_QTINSPECTOR_BUILD
#define LIB_QTI_EXPORT Q_DECL_EXPORT
#else
#define LIB_QTI_EXPORT Q_DECL_IMPORT
#endif
