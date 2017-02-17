#include "gmovenode.h"

#include <QPainter>
#include <QPointF>
#include "gmove.h"

GMoveNode::GMoveNode(int index, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mIndex(-1),
      mSelected(false)
{
    
}



QRectF GMoveNode::boundingRect() const
{
    return QRectF( -1, -1, 2, 2);
}

void GMoveNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(Qt::NoPen);
//    painter->setBrush(Qt::darkGray);
//    painter->drawEllipse(-7, -7, 20, 20);

//    QRadialGradient gradient(-3, -3, 10);
//    if (option->state & QStyle::State_Sunken) {
//        gradient.setCenter(3, 3);
//        gradient.setFocalPoint(3, 3);
//        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
//        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
//    } else {
//        gradient.setColorAt(0, Qt::yellow);
//        gradient.setColorAt(1, Qt::darkYellow);
//    }
//    painter->setBrush(gradient);
    QColor c(mSelected ? Qt::red : Qt::blue);
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(QBrush(c));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(QPointF(), 0.1, 0.1);
//    painter->drawPoint(QPointF());
}
