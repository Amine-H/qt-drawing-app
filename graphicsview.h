#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView();
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:
    void mouseReleased(QPoint);
    void mouseMoved(QPoint);
    void mouseButton(Qt::MouseButton);
};

#endif // GRAPHICSVIEW_H
