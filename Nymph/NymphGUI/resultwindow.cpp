#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "nymphgui_global.h"
#include "ui_nymphviewer.h"
#include "mainwindow.h"

ResultWindow::ResultWindow(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    ui->styleViewer->ui->action_viewer_name->setText(tr("Style"));
    ui->resultViewer->ui->action_viewer_name->setText(tr("Nymph"));
    setWindowTitle(QString("Nymph Result - %1").arg(id));
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::updateResultWindow(QPixmap style_in, QPixmap style_out, QPixmap nymph_in, QPixmap nymph_out)
{
    ui->styleViewer->ui->sourceView->setImage(style_in);
    ui->styleViewer->ui->resultView->setImage(style_out);
    ui->resultViewer->ui->sourceView->setImage(nymph_in);
    ui->resultViewer->ui->resultView->setImage(nymph_out);
}
