#ifndef NYMPHEDITOR_H
#define NYMPHEDITOR_H

#include <QPlainTextEdit>
#include "nymphrunner.h"
#include "resultwindow.h"

class NymphEditor : public QPlainTextEdit
{
    friend class MainWindow;

    Q_OBJECT
public:
    static NymphEditor* instance(QWidget *parent = 0);
    explicit NymphEditor(int id, QWidget *parent = 0);

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

    void startRunner();
    void executeCMD(const QString& cmd);

    bool isRunning() {return _isRunning;}

    void showResultWindow();

    void updateImages();

private:
    NymphRunner runner;
    ResultWindow resultWindow;

    int _id;

    bool isUntitled;
    bool _isRunning;
    QString curFile;
    int linesExecuted;

    QWidget *lineNumberArea;

    void setCurrentFile(const QString &fileName);
    void finishRunner();

protected:
    void wheelEvent(QWheelEvent *e);
    void scrollContentsBy(int dx, int dy);
    void closeEvent(QCloseEvent *event);
    //void setWindowTitle(const QString & newTitle);
    void resizeEvent(QResizeEvent *e);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateLineColor(int colorID);

    void scriptFinished();

signals:
    void statusChanged();

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
