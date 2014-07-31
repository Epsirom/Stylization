#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = 0);
    ~ResultWindow();

private:
    Ui::ResultWindow *ui;

signals:
    void windowActiveChanged(bool);
};

#endif // RESULTWINDOW_H
