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
