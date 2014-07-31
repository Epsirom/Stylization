#ifndef NYMPHGUI_GLOBAL_H
#define NYMPHGUI_GLOBAL_H

#include <QString>

#define AVAILABLE_FILE_TYPES "Nymph Lua Script Files(*.nl)"";;All Files(*.*)"

void nymphLog(const char*);
void nymphLog(const QString &log);

#define NYMPH_DEBUG

#ifdef NYMPH_DEBUG
#define nymphDebug(log) nymphLog(log)
#else
#define nymphDebug(log) ;
#endif
#endif // NYMPHGUI_GLOBAL_H
