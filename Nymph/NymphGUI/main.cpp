#include "mainwindow.h"
#include <QApplication>

#include "nymphgui_global.h"
#include "nymphlua_ext.h"

int nymph_lua_print(int id, const char* name, const char* str)
{
    if (QString(str).startsWith("[warning]"))
    {
        nymphWarning(QString("[%1_%2] %3").arg(id).arg(name).arg(str + 9));
    }
    else
    {
        nymphLog(QString("[%1_%2] %3").arg(id).arg(name).arg(str));
    }
    return 0;
}

void nymph_lua_syncview(int id)
{
    MainWindow::instance()->updateResultWindow(id);
}

void nymph_lua_showresult(int id)
{
    MainWindow::instance()->showResultWindow(id);
}

int main(int argc, char *argv[])
{
    install_new_print(nymph_lua_print);
    install_new_syncview(nymph_lua_syncview);
    install_new_showresult(nymph_lua_showresult);

    QApplication a(argc, argv);
    MainWindow* w = MainWindow::instance();
    w->show();

    return a.exec();
}
