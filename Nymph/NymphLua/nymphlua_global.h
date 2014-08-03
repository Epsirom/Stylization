#ifndef NYMPHLUA_GLOBAL_H
#define NYMPHLUA_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(NYMPHLUA_LIBRARY)
#  define NYMPHLUASHARED_EXPORT Q_DECL_EXPORT
#else
#  define NYMPHLUASHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // NYMPHLUA_GLOBAL_H
