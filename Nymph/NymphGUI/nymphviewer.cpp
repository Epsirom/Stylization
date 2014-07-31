#include "nymphviewer.h"
#include "ui_nymphviewer.h"
#include <QToolBar>
#include "nymphgui_global.h"
#include <QInputDialog>

NymphViewer::NymphViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NymphViewer),
    scale(100)
{
    ui->setupUi(this);
    ui->sourceView->sibling = ui->resultView;
    ui->resultView->sibling = ui->sourceView;

    syncScale();
}

NymphViewer::~NymphViewer()
{
    delete ui;
}

void NymphViewer::syncScale()
{
    if (scale >= 500) {
        scale = 500;
        ui->action_zoom_in->setEnabled(false);
    } else {
        ui->action_zoom_in->setEnabled(true);
    }
    if (scale <= 10) {
        scale = 10;
        ui->action_zoom_out->setEnabled(false);
    } else {
        ui->action_zoom_out->setEnabled(true);
    }
    ui->action_zoom_origin->setEnabled(scale != 100);
    ui->action_zoom_fit->setEnabled(scale != ui->sourceView->getFitScale() * 100 - 1);

    qreal rscale = qreal(scale) / 100;
    ui->sourceView->setScale(rscale, rscale);
    ui->resultView->setScale(rscale, rscale);
    ui->action_scale->setText(QString("Current scale: %1%").arg(scale));

    //nymphLog(QString("Scale to %1").arg(rscale));
}

void NymphViewer::on_action_zoom_in_triggered()
{
    //nymphLog(QString("Zoom in from %1").arg(scale));
    scale += 10;
    syncScale();
}

void NymphViewer::on_action_zoom_out_triggered()
{
    //nymphLog(QString("Zoom out from %1").arg(scale));
    scale -= 10;
    syncScale();
}

void NymphViewer::on_action_zoom_origin_triggered()
{
    scale = 100;
    syncScale();
}

void NymphViewer::on_action_zoom_fit_triggered()
{
    scale = ui->sourceView->getFitScale() * 100 - 1;
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
