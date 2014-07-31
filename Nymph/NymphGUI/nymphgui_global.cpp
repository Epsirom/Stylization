#include "nymphgui_global.h"
#include "mainwindow.h"

void nymphLog(const char *log)
{
    MainWindow::instance()->appendLog(QString(log));
}

void nymphLog(const QString &log)
{
    MainWindow::instance()->appendLog(log);
}
