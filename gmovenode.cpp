#include "gmovenode.h"

#include <QPainter>
#include <QPointF>
#include <QDebug>

#include "gpad.h"
#include "gmove.h"
#include "gmovesmodel.h"

static const qreal D = 0.06;        // Diameter
static const qreal R = D / 2;       // Radius
static const qreal P = D / 3;       // Perimeter width
static const qreal DP = D + 2 * P;  // Outer size
static const qreal RP = R + P;      // Radius + Perimeter

GMoveNode::GMoveNode(const QPersistentModelIndex &index, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mIndex(index)
{
    setFlag(ItemIsSelectable);// | QGraphicsItem::ItemIsMovable);
    setPos(mIndex.data(GPad::MoveXYRole).toPointF());
    setZValue(mIndex.data(GPad::MoveZRole).toDouble());
}

QRectF GMoveNode::boundingRect() const
{
    return QRectF(-RP, -RP, DP, DP);
}

void GMoveNode::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    bool selected = mIndex.data(GPad::SelectionRole).toBool();
    QColor c(selected ? Qt::red : Qt::blue);
    c.setAlpha(0xaa);
    
    int type = mIndex.data(GPad::MoveTypeRole).toInt();
    
    if (type == GMove::DestringPrime) {
        painter->setPen(QPen(Qt::darkGreen, P));
        
    } else if (type == GMove::DestringSuck) {
        painter->setPen(QPen(Qt::gray, P));
            
    } else {
        painter->setPen(QPen(Qt::NoPen));
    }
    painter->setBrush(QBrush(c));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(boundingRect());
}


QVariant GMoveNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//    qDebug() << __PRETTY_FUNCTION__ << change;
    return QGraphicsItem::itemChange(change, value);
}
