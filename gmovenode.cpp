#include "gmovenode.h"

#include <QPainter>
#include <QPointF>
#include <QDebug>

#include "g.h"
#include "gmove.h"
#include "gmovesmodel.h"

static const qreal D = 0.2;
static const qreal R = D / 2;

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
    return QRectF( -R, -R, D, D);
}

void GMoveNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c(mIndex.data(G::SelectionRole).toBool() ? Qt::red : Qt::blue);
    painter->setBrush(QBrush(c));
    painter->setPen(QPen(Qt::NoPen));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(boundingRect());
}


QVariant GMoveNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//    qDebug() << __PRETTY_FUNCTION__ << change;
    return QGraphicsItem::itemChange(change, value);
}
