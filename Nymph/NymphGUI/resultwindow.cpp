#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "nymphgui_global.h"
#include "ui_nymphviewer.h"

ResultWindow::ResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    ui->styleViewer->ui->action_viewer_name->setText(tr("Style"));
    ui->resultViewer->ui->action_viewer_name->setText(tr("Nymph"));
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::updateImages(NymphImagePack *pack)
{
    ui->styleViewer->ui->sourceView->setImage(pack->style_in);
    ui->styleViewer->ui->resultView->setImage(pack->style_out);
    ui->resultViewer->ui->sourceView->setImage(pack->nymph_in);
    ui->resultViewer->ui->resultView->setImage(pack->nymph_out);
}
