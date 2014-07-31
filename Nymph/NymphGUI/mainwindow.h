#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nympheditor.h"
#include "resultwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void appendLog(const QString &log);
    void executeCMD(const QString &cmd);

    static MainWindow* instance();

protected:
     void changeEvent(QEvent *e);
     void closeEvent(QCloseEvent *);
     void dragEnterEvent(QDragEnterEvent *e);
     void dropEvent(QDropEvent *event);
//     void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_action_new_file_triggered();

    void on_action_save_file_triggered();

    void on_action_about_qt_triggered();

    void on_action_save_file_as_triggered();

    void on_action_open_file_triggered();

    void on_action_close_file_triggered();

    void on_action_run_triggered();

    void on_action_show_result_triggered();

    void on_action_about_me_triggered();

    void updateMenus();
    void highLightCurrentLine();
    void documentWasModified();

    void on_cmdEdit_activated(const QString &arg1);

    void on_cmdEdit_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    ResultWindow resultWindow;
    NymphEditor * activeMdiChild();
    void iniConnect();
};

#endif // MAINWINDOW_H
