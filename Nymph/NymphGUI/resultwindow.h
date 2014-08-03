#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>

class QPixmap;

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWindow(int id, QWidget *parent = 0);
    ~ResultWindow();

    void updateResultWindow(QPixmap style_in, QPixmap style_out, QPixmap nymph_in, QPixmap nymph_out);

private:
    Ui::ResultWindow *ui;

signals:
    void windowActiveChanged(bool);
};

#endif // RESULTWINDOW_H
