#include "ggriditem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <QDebug>

GGridItem::GGridItem(QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    
}

QRectF GGridItem::boundingRect() const
{
    QGraphicsScene* s = scene();
    
    return s == NULL ? QRectF() : s->sceneRect();
}

void GGridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QRectF bounds = boundingRect();
////    qreal h = bounds.height();
////    qreal w = bounds.width();
//    qreal x = bounds.left();
//    qreal y = bounds.top();
    
//    QPen pen(Qt::SolidLine);
//    pen.setCosmetic(true);
    
//    painter->setPen(pen);
//    while (x <= bounds.right()) {
//        painter->drawLine(x, bounds.bottom(), x, bounds.top());
//        x += 10;
//    }
    
//    while (y <= bounds.bottom()) {
//        painter->drawLine(bounds.left(), y, bounds.right(), y);
//        y += 10;
//    }
}
