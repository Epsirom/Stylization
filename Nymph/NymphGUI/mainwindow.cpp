#include "nymphgui_global.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSplitter>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMimeData>
#include <QTime>
#include <QLineEdit>
#include <QStringList>
#include <QDebug>


MainWindow* MainWindow::instance()
{
    static MainWindow * _instance = 0;
    if (_instance == 0)
    {
        _instance = new MainWindow();
    }
    return _instance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, this);
    splitterMain->addWidget(ui->mdiWidget);
    splitterMain->addWidget(ui->logWidget);
    this->setCentralWidget(splitterMain);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateMenus);
    //connect(&resultWindow, &ResultWindow::windowActiveChanged, this, &MainWindow::updateMenus);
    connect(qApp, &QApplication::focusChanged, this, &MainWindow::updateMenus);

    //connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateResultWindow);

    updateMenus();

    appendLog(tr("Nymph initialized."));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendLog(const QString &log, NymphLogType type)
{
    static const QString colors[] = {"black", "orange", "red"};
    QString color = colors[int(type)];

    ui->logEdit->appendHtml(QString("<span style='color:%1'>[%2] %3</span>")
                            .arg(color).arg(QTime::currentTime().toString(QString("hh:mm:ss.zzz"))).arg(log));

    ui->logEdit->moveCursor(QTextCursor::End);
}

void MainWindow::executeCMD(const QString &cmd)
{
    QStringList cmd_parts = cmd.split(" ");
    if (cmd_parts.length() > 0)
    {
        QString ins = cmd_parts[0];
        if (ins == "cls" || ins == "clc") {
            ui->logEdit->clear();
        } else if (ins == "run") {
            if (activeMdiChild() != 0)
                ui->action_run->trigger();
            else
                nymphWarning("No file opened! Run failed.");
        } else if (ins == "edit") {
            if (activeMdiChild() != 0)
                activeMdiChild()->setFocus();
            else
                nymphWarning("No file opened! Edit failed.");
        }
    }
}

void MainWindow::showResultWindow(int id)
{
    NymphEditor* editor = getMdiChild(id);
    if (editor) {
        editor->showResultWindow();
    } else {
        qWarning() << "No matched editor:" << id;
    }
}

void MainWindow::updateResultWindow(int id)
{
    NymphEditor* editor = getMdiChild(id);
    if (editor) {
        editor->updateImages();
    } else {
        qWarning() << "No matched editor:" << id;
    }
}

void MainWindow::documentWasModified()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
    {
        qobject_cast<NymphEditor *>(activeSubWindow->widget())->setWindowModified(true);
    }
}

void MainWindow::iniConnect()
{
    connect(activeMdiChild()->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);
    connect(activeMdiChild()->document(), &QTextDocument::contentsChanged, this, &MainWindow::updateMenus);
    connect(activeMdiChild(), &NymphEditor::statusChanged, this, &MainWindow::updateMenus);
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    bool isRunning = hasMdiChild && (activeMdiChild()->isRunning());
    bool isModified = hasMdiChild && (activeMdiChild()->document()->isModified());
    bool hasResultWindow = false;
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        NymphEditor *mdiChild = qobject_cast<NymphEditor *>(window->widget());
        if (mdiChild->resultWindow.isActiveWindow())
        {
            hasResultWindow = true;
            break;
        }
    }

    ui->action_close_file->setEnabled(hasMdiChild || hasResultWindow);
    ui->action_run->setEnabled(hasMdiChild && !isRunning);
    ui->action_save_file->setEnabled(hasMdiChild && isModified);
    ui->action_save_file_as->setEnabled(hasMdiChild);
    ui->action_show_result->setEnabled(hasMdiChild);
}

void MainWindow::highLightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!activeMdiChild()->isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = activeMdiChild()->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    activeMdiChild()->setExtraSelections(extraSelections);
}

NymphEditor* MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<NymphEditor *>(activeSubWindow->widget());
    return NULL;
}

NymphEditor* MainWindow::getMdiChild(int id)
{
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        NymphEditor *mdiChild = qobject_cast<NymphEditor *>(window->widget());
        if (mdiChild->_id == id)
        {
            return mdiChild;
        }
    }
    return 0;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasFormat("text/uri-list"))
        e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
    {
        return;
    }
    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
    {
        return;
    }
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
    bool hasOpened = false;
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        NymphEditor *mdiChild = qobject_cast<NymphEditor *>(window->widget());
        if( mdiChild->currentFile() == canonicalFilePath)
        {
            hasOpened = true;
            ui->mdiArea->setActiveSubWindow(window);
            break;
        }
    }
    if(!hasOpened)
    {
        NymphEditor *child = NymphEditor::instance();
        ui->mdiArea->addSubWindow(child);
        if(child->loadFile(fileName))
        {
            child->setVisible(true);
            iniConnect();
        }
        else
            child->close();
    }
}

void MainWindow::on_action_new_file_triggered()
{
    NymphEditor *child = NymphEditor::instance();
    ui->mdiArea->addSubWindow(child);
    child->newFile();
    child->setVisible(true);
    iniConnect();
}

void MainWindow::on_action_save_file_triggered()
{
    if (activeMdiChild() != NULL)
    {
        activeMdiChild()->save();
    }
}

void MainWindow::on_action_save_file_as_triggered()
{
    if (activeMdiChild() != NULL)
    {
        activeMdiChild()->saveAs();
    }
}

void MainWindow::on_action_open_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName
        (this,"open file",NULL,tr(AVAILABLE_FILE_TYPES));
    if(!fileName.isEmpty())
    {
        QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
        bool hasOpened = false;
        foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
        {
            NymphEditor *mdiChild = qobject_cast<NymphEditor *>(window->widget());
            if( mdiChild->currentFile() == canonicalFilePath)
            {
                hasOpened = true;
                ui->mdiArea->setActiveSubWindow(window);
                break;
            }
        }
        if(!hasOpened)
        {
            NymphEditor *child = NymphEditor::instance();
            ui->mdiArea->addSubWindow(child);
            if(child->loadFile(fileName))
            {
                child->setVisible(true);
                iniConnect();
            }
            else
            {
                child->close();
            }
        }
    }
}

void MainWindow::on_action_close_file_triggered()
{
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        NymphEditor *mdiChild = qobject_cast<NymphEditor *>(window->widget());
        if (mdiChild->resultWindow.isActiveWindow())
        {
            mdiChild->resultWindow.close();
            return;
        }
    }
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_action_run_triggered()
{
    activeMdiChild()->startRunner();
}

void MainWindow::on_action_show_result_triggered()
{
    activeMdiChild()->showResultWindow();
}

void MainWindow::on_action_about_me_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Nymph v0.0.1 alpha\nBy Huarong Chen http://chr.asia\nCopy rights reserved.\n2014.08"));
}

void MainWindow::on_action_about_qt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void MainWindow::on_cmdEdit_activated(const QString &)
{
    //ui->cmdEdit->clearEditText();
    ui->cmdEdit->setCurrentIndex(-1);
}

void MainWindow::on_cmdEdit_currentIndexChanged(const QString &arg1)
{
    if (ui->cmdEdit->currentIndex() >= 0)
    {
        executeCMD(arg1.simplified());
    }
}

void MainWindow::on_action_cmd_trigger_triggered()
{
    ui->cmdEdit->setFocus();
}
