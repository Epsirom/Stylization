#ifndef NYMPHGUI_GLOBAL_H
#define NYMPHGUI_GLOBAL_H

#include <QString>

#define AVAILABLE_FILE_TYPES "Nymph Lua Script Files(*.nl)"";;Nymph Lua Module Files(*.lua)"";;All Files(*.*)"

typedef enum nymph_log_type {
    NYMPH_LOG_INFO = 0,
    NYMPH_LOG_WARNING = 1,
    NYMPH_LOG_ERROR = 2
} NymphLogType;

void nymphLog(const QString &log);
void nymphWarning(const QString &log);
void nymphError(const QString &log);

#define NYMPH_DEBUG

#ifdef NYMPH_DEBUG
#define nymphDebug(log) nymphLog(log)
#else
#define nymphDebug(log) ;
#endif
#endif // NYMPHGUI_GLOBAL_H
