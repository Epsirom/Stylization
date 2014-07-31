#ifndef NYMPHEDITOR_H
#define NYMPHEDITOR_H

#include <QPlainTextEdit>

class NymphEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit NymphEditor(QWidget *parent = 0);

    void newFile();
    bool loadFile(const QString &fileName);
    bool closeDocument();
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString currentFile() { return curFile; }

    void execute();

    void stop();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void zoomIn(int step);
    void zoomOut(int step);

private:
    bool isUntitled;
    QString curFile;
    int linesExecuted;

    QWidget *lineNumberArea;

    void setCurrentFile(const QString &fileName);

protected:
    void wheelEvent(QWheelEvent *e);
    void closeEvent(QCloseEvent *event);
    //void setWindowTitle(const QString & newTitle);
    void resizeEvent(QResizeEvent *e);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateLineColor(int colorID);

signals:

public slots:
    void documentWasModified();

};


class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(NymphEditor *editor = 0)
        : QWidget(editor)
    {
        nymphEditor = editor;
    }
    QSize sizeHint() const
    {
        return QSize(nymphEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        nymphEditor->lineNumberAreaPaintEvent(event);
    }

private:
    NymphEditor *nymphEditor;
};


#endif // NYMPHEDITOR_H
