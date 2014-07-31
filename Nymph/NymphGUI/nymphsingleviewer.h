#ifndef NYMPHSINGLEVIEWER_H
#define NYMPHSINGLEVIEWER_H

#include <QGraphicsView>
#include "nymphviewer.h"

class NymphSingleViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit NymphSingleViewer(QWidget *parent = 0);

    void syncScroll(int offset_x, int offset_y);

    void setScale(qreal sx, qreal sy);

    qreal getFitScale();

protected:
    void scrollContentsBy(int dx, int dy);

private:
    NymphSingleViewer* sibling;
    int offset_x, offset_y;
    friend class NymphViewer;

signals:

public slots:

};

#endif // NYMPHSINGLEVIEWER_H
