#include "mainwindow.h"
#include <QApplication>

#include "nymphgui_global.h"
#include "nymphlua_ext.h"

int nymph_lua_print(const char* name, const char* str)
{
    if (QString(str).startsWith("[warning]"))
    {
        nymphWarning(QString("[%1] %2").arg(name).arg(str + 9));
    }
    else
    {
        nymphLog(QString("[%1] %2").arg(name).arg(str));
    }
    return 0;
}

void nymph_lua_syncview()
{
    MainWindow::instance()->updateResultWindow();
}

int main(int argc, char *argv[])
{
    install_new_print(nymph_lua_print);
    install_new_syncview(nymph_lua_syncview);

    QApplication a(argc, argv);
    MainWindow* w = MainWindow::instance();
    w->show();

    return a.exec();
}
