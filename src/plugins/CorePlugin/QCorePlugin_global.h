#ifndef QCOREPLUGIN_GLOBAL_H
#define QCOREPLUGIN_GLOBAL_H

#include <qglobal.h>

#if defined(QCORE_LIBRARY)
#  define CORE_EXPORT Q_DECL_EXPORT
#elif defined(CORE_STATIC_LIBRARY) // Abuse single files for manual tests
#  define CORE_EXPORT
#else
#  define CORE_EXPORT Q_DECL_IMPORT
#endif

#endif // QCOREPLUGIN_GLOBAL_H
