#pragma once

#include <QtGlobal>

#ifdef LIB_MENDELEY_BUILD
#define LIB_MENDELEY_EXPORT Q_DECL_EXPORT
#else
#define LIB_MENDELEY_EXPORT Q_DECL_IMPORT
#endif
