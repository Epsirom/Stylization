#include <QApplication>
#include <QPainter>
#include <QTextBlock>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QScrollBar>
#include "nymphgui_global.h"
#include "nympheditor.h"

NymphEditor::NymphEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    linesExecuted = 0;
    setAcceptDrops(false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWordWrapMode(QTextOption::NoWrap);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int NymphEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void NymphEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void NymphEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void NymphEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void NymphEditor::highlightCurrentLine()
{
    if (!isReadOnly()) {
        QList<QTextEdit::ExtraSelection> extraSelections;
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::cyan).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
        setExtraSelections(extraSelections);
    }
}

void NymphEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void NymphEditor::zoomIn(int step)
{
    QFont f = font();
    f.setPointSize(f.pointSize() + step);
    setFont(f);
}

void NymphEditor::zoomOut(int step)
{
    QFont f = font();
    f.setPointSize(f.pointSize() - step);
    setFont(f);
}

void NymphEditor::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("untitled %1").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
}

bool NymphEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->setPlainText(in.readAll());
   //  this->setHtml(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    return true;
}

bool NymphEditor::closeDocument()
{
    if (this->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Warning"),
                     tr("Close '%1' without saving?")
                     .arg(curFile),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool NymphEditor::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool NymphEditor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,tr("Save as"),curFile,tr(AVAILABLE_FILE_TYPES));

    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool NymphEditor::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

void NymphEditor::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(QFileInfo(fileName).fileName() + "[*]");
}

void NymphEditor::documentWasModified()
{
    setWindowModified(document()->isModified());
}

void NymphEditor::wheelEvent(QWheelEvent *e)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier)//是否按下Ctrl键
    {
        if(e->delta() > 0 )//上滚
                this->zoomIn(1);//放大
            else
                this->zoomOut(1);
    }
    else//实现文本的上下滚动
    {
         if(e->delta() > 0 )//上滚
             this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-3);
         else
             this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()+3);

    }
}

void NymphEditor::closeEvent(QCloseEvent *event)
{
    if (closeDocument()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void NymphEditor::updateLineColor(int colorID)
{
    if (isReadOnly()) {
        QList<QTextEdit::ExtraSelection> extraS = extraSelections();
        QTextEdit::ExtraSelection selection;

        QColor lineColor;
        switch (colorID)
        {
        case -1:
            lineColor = QColor(Qt::yellow).lighter(160);
            break;
        case 0:
            lineColor = QColor(Qt::green).lighter(160);
            break;
        default:
            // Error
            lineColor = QColor(Qt::red).lighter(160);
            break;
        }

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = QTextCursor(document()->findBlockByLineNumber(linesExecuted));
        selection.cursor.clearSelection();
        extraS.append(selection);
        setExtraSelections(extraS);
    }
}
