#include "graphicsview.h"
GraphicsView::GraphicsView()
{
    setMouseTracking(true);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint p(event->globalPos());
    emit mouseReleased(p);
    emit mouseButton(event->button());
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p(event->pos());
    emit mouseMoved(p);
}

