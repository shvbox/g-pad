#include "gmovenode.h"

#include <QPainter>
#include <QPointF>
#include <QDebug>

#include "g.h"
#include "gmove.h"
#include "gmovesmodel.h"

static const qreal D = 0.06;
static const qreal R = D / 2;
static const qreal P = D / 3;
static const qreal DP = D + 2 * P;
static const qreal RP = R + P;

GMoveNode::GMoveNode(const QPersistentModelIndex &index, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mIndex(index)
{
    setFlag(ItemIsSelectable);// | QGraphicsItem::ItemIsMovable);
    setPos(mIndex.data(G::MoveXYRole).toPointF());
    setZValue(mIndex.data(G::MoveZRole).toDouble());
}

QRectF GMoveNode::boundingRect() const
{
    return QRectF(-RP, -RP, DP, DP);
}

void GMoveNode::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    QColor c(mIndex.data(G::SelectionRole).toBool() ? Qt::red : Qt::blue);
    
    int type = mIndex.data(G::MoveTypeRole).toInt();
    
    if (type == GMove::RetractPrime) {
        painter->setPen(QPen(Qt::darkGreen, P));
        
    } else if (type == GMove::RetractSuck) {
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
