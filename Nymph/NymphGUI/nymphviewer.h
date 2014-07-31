#ifndef NYMPHVIEWER_H
#define NYMPHVIEWER_H

#include <QWidget>

namespace Ui {
class NymphViewer;
}

class NymphViewer : public QWidget
{
    Q_OBJECT

public:
    explicit NymphViewer(QWidget *parent = 0);
    ~NymphViewer();

private slots:
    void on_action_zoom_in_triggered();

    void on_action_zoom_out_triggered();

    void on_action_zoom_origin_triggered();

    void on_action_zoom_fit_triggered();

    void on_action_scale_triggered();

private:
    Ui::NymphViewer *ui;

    void syncScale();

    int scale;

    friend class ResultWindow;
};

#endif // NYMPHVIEWER_H
