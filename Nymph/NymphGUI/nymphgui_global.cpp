#include "nymphgui_global.h"
#include "mainwindow.h"

#define _NYMPH_MAIN MainWindow::instance()
#define _NYMPH_LOG_TEMPLATE(type) MainWindow::instance()->append##type(log)


void nymphLog(const QString &log)
{
    _NYMPH_LOG_TEMPLATE(Info);
}

void nymphWarning(const QString &log)
{
    _NYMPH_LOG_TEMPLATE(Warning);
}

void nymphError(const QString &log)
{
    _NYMPH_LOG_TEMPLATE(Error);
}
