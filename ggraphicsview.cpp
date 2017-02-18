#include <QtWidgets>
#include <cmath>

#include "ggraphicsview.h"

GGraphicsView::GGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setBackgroundBrush(Qt::NoBrush);
//    QGraphicsScene *scene = new QGraphicsScene(this);
//    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
//    scene->setSceneRect(-200, -200, 400, 400);
//    setScene(scene);
//    setCacheMode(CacheBackground);
//    setViewportUpdateMode(BoundingRectViewportUpdate);
//    setRenderHint(QPainter::Antialiasing);
//    setTransformationAnchor(AnchorUnderMouse);
    
    qreal s = 96.0 / 24.5;
    scale(s, s);
    //    setMinimumSize(400, 400);

    // Get the current transform
    QTransform transform(this->transform());
    
    qreal m11 = transform.m11();    // Horizontal scaling
    qreal m12 = transform.m12();    // Vertical shearing
    qreal m13 = transform.m13();    // Horizontal Projection
    qreal m21 = transform.m21();    // Horizontal shearing
    qreal m22 = transform.m22();    // vertical scaling
    qreal m23 = transform.m23();    // Vertical Projection
    qreal m31 = transform.m31();    // Horizontal Position (DX)
    qreal m32 = transform.m32();    // Vertical Position (DY)
    qreal m33 = transform.m33();    // Addtional Projection Factor
    
    // Vertical flip
    m22 = -m22;
    
    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
    
    // Set the items transformation
    setTransform(transform);
}

void GGraphicsView::zoomIn()
{
    scaleView(qreal(1.1));
}

void GGraphicsView::zoomOut()
{
    scaleView(1 / qreal(1.1));
}

void GGraphicsView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
//    case Qt::Key_Up:
//        centerNode->moveBy(0, -20);
//        break;
//    case Qt::Key_Down:
//        centerNode->moveBy(0, 20);
//        break;
//    case Qt::Key_Left:
//        centerNode->moveBy(-20, 0);
//        break;
//    case Qt::Key_Right:
//        centerNode->moveBy(20, 0);
//        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
//    case Qt::Key_Space:
//    case Qt::Key_Enter:
//        shuffle();
//        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

#ifndef QT_NO_WHEELEVENT
void GGraphicsView::wheelEvent(QWheelEvent *event)
{
//    scaleView(pow(2.0, event->delta() / 240.0));
    scaleView(pow(1.2, event->delta() / 240.0));
}
#endif

void GGraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 1 || factor > 500)
        return;

    scale(scaleFactor, scaleFactor);
}


//QRect GGraphicsView::visualRect(const QModelIndex &index) const
//{
//}

//void GGraphicsView::scrollTo(const QModelIndex &index, ScrollHint hint)
//{
//}

//QModelIndex GGraphicsView::indexAt(const QPoint &point) const
//{
//}

//QModelIndex GGraphicsView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
//{
//}

//int GGraphicsView::horizontalOffset() const
//{
//}

//int GGraphicsView::verticalOffset() const
//{
//}

//bool GGraphicsView::isIndexHidden(const QModelIndex &index) const
//{
//}

//void GGraphicsView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
//{
//}

//QRegion GGraphicsView::visualRegionForSelection(const QItemSelection &selection) const
//{
//}
