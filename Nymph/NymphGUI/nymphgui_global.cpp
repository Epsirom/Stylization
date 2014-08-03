#include "nymphgui_global.h"
#include "mainwindow.h"

#define _NYMPH_LOG_TEMPLATE(type) MainWindow::instance()->appendLog(log, type)

void nymphLog(const QString &log)
{
    _NYMPH_LOG_TEMPLATE(NYMPH_LOG_INFO);
}

void nymphWarning(const QString &log)
{
    _NYMPH_LOG_TEMPLATE(NYMPH_LOG_WARNING);
}

void nymphError(const QString &log)
{
    _NYMPH_LOG_TEMPLATE(NYMPH_LOG_ERROR);
}
