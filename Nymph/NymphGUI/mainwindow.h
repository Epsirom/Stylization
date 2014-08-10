#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nympheditor.h"
#include "resultwindow.h"
#include "nymphlua.h"
#include "nymphgui_global.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void executeCMD(const QString &cmd);

    void showResultWindow(int id);
    void appendInfo(const QString &log);
    void appendWarning(const QString &log);
    void appendError(const QString &log);

    static MainWindow* instance();

public slots:
    void updateResultWindow(int id);
    void appendLog(const QString &log, int type = NYMPH_LOG_INFO);

signals:
    void appendLogSignal(const QString &log, int type);

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

    void on_action_cmd_trigger_triggered();

private:
    Ui::MainWindow *ui;

    NymphEditor * activeMdiChild();
    NymphEditor * getMdiChild(int id);
    void iniConnect();

};

#endif // MAINWINDOW_H
