#include "nymphsingleviewer.h"
#include "nymphgui_global.h"
#include <QScrollBar>
#include <QTransform>

NymphSingleViewer::NymphSingleViewer(QWidget *parent) :
    QGraphicsView(parent),
    sibling(0),
    offset_x(0),
    offset_y(0)
{
    QGraphicsScene * scn = new QGraphicsScene(this);
    this->setScene( scn );
    QPixmap pix( ":/default_img" );
    setImage(pix);

    this->grabGesture(Qt::PinchGesture);
}

void NymphSingleViewer::setImage(QPixmap &img)
{
    scene()->clear();
    scene()->addPixmap(img);
}

void NymphSingleViewer::scrollContentsBy(int dx, int dy)
{
    //nymphLog(qPrintable(QString("Scroll to (%1,%2)").arg(dx).arg(dy)));
    QGraphicsView::scrollContentsBy(dx, dy);
    if (sibling)
    {
        sibling->syncScroll(horizontalScrollBar()->value(), verticalScrollBar()->value());
    }
}

bool NymphSingleViewer::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
        QGestureEvent * gevent = static_cast<QGestureEvent*>(event);
        if (QGesture *pinch = gevent->gesture(Qt::PinchGesture))
        {
            gevent->accept();
            pinchTriggered(static_cast<QPinchGesture *>(pinch));
        }
    }
    return QGraphicsView::event(event);
}

bool NymphSingleViewer::pinchTriggered(QPinchGesture *g)
{
    if (g->scaleFactor() == 1)
    {
        return false;
    }
    emit pinchZoom(g->scaleFactor());
    g->setScaleFactor(1.);
    return true;
}

void NymphSingleViewer::syncScroll(int offx, int offy)
{
    horizontalScrollBar()->setValue(offx);
    verticalScrollBar()->setValue(offy);
    //nymphLog(qPrintable(QString("Scroll to (%1,%2)").arg(offx).arg(offy)));
}

void NymphSingleViewer::setScale(qreal sx, qreal sy)
{
    QTransform t = transform();
    setTransform(QTransform(sx, t.m12(), t.m13(), t.m21(), sy, t.m23(), t.m31(), t.m32(), t.m33()));
}

qreal NymphSingleViewer::getFitScale()
{
    QRect myRect = rect();
    QGraphicsScene* myScene = scene();
    qreal wscale = qreal(myRect.width()) / myScene->width();
    qreal hscale = qreal(myRect.height()) / myScene->height();
    return wscale < hscale ? wscale : hscale;
}
