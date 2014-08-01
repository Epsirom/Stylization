#include "nymphviewer.h"
#include "ui_nymphviewer.h"
#include <QToolBar>
#include "nymphgui_global.h"
#include <QInputDialog>

NymphViewer::NymphViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NymphViewer),
    scale(1.0)
{
    ui->setupUi(this);
    ui->sourceView->sibling = ui->resultView;
    ui->resultView->sibling = ui->sourceView;

    connect(ui->sourceView, &NymphSingleViewer::pinchZoom, this, &NymphViewer::pinch_zoom);
    connect(ui->resultView, &NymphSingleViewer::pinchZoom, this, &NymphViewer::pinch_zoom);

    syncScale();
}

NymphViewer::~NymphViewer()
{
    delete ui;
}

void NymphViewer::syncScale()
{
    if (scale >= 5) {
        scale = 5;
        ui->action_zoom_in->setEnabled(false);
    } else {
        ui->action_zoom_in->setEnabled(true);
    }
    if (scale <= 0.1) {
        scale = 0.1;
        ui->action_zoom_out->setEnabled(false);
    } else {
        ui->action_zoom_out->setEnabled(true);
    }
    ui->action_zoom_origin->setEnabled(scale != 1.0);
    //ui->action_zoom_fit->setEnabled(scale != ui->sourceView->getFitScale() - 0.01);

    ui->sourceView->setScale(scale, scale);
    ui->resultView->setScale(scale, scale);
    ui->action_scale->setText(QString("Current scale: %1%").arg(scale * 100));

    //nymphLog(QString("Scale to %1").arg(rscale));
}

void NymphViewer::on_action_zoom_in_triggered()
{
    //nymphLog(QString("Zoom in from %1").arg(scale));
    scale += 0.1;
    syncScale();
}

void NymphViewer::on_action_zoom_out_triggered()
{
    //nymphLog(QString("Zoom out from %1").arg(scale));
    scale -= 0.1;
    syncScale();
}

void NymphViewer::on_action_zoom_origin_triggered()
{
    scale = 1.0;
    syncScale();
}

void NymphViewer::on_action_zoom_fit_triggered()
{
    scale = ui->sourceView->getFitScale() - 0.01;
    syncScale();
}

void NymphViewer::on_action_scale_triggered()
{
    bool ok;
    int user_input = QInputDialog::getInt(this, tr("Set scale"),
                                 tr("Input a number between 10 and 500:"), scale, 10, 500, 1, &ok);
    if (ok)
    {
        scale = user_input;
        syncScale();
    }
}

void NymphViewer::pinch_zoom(qreal factor)
{
    scale *= factor;
    syncScale();
}
