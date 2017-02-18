#include "gmovenode.h"

#include <QPainter>
#include <QPointF>

#include "gmove.h"
#include "gmovesmodel.h"

static const qreal D = 0.2;
static const qreal R = D / 2;

GMoveNode::GMoveNode(const QPersistentModelIndex &index, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mIndex(index)
{
    setPos(mIndex.data(GraphicsRole::XYRole).toPointF());
    setZValue(mIndex.data(GraphicsRole::ZRole).toDouble());
    setFlags(QGraphicsItem::ItemIsSelectable);// | QGraphicsItem::ItemIsMovable);
}

QRectF GMoveNode::boundingRect() const
{
    return QRectF( -R, -R, D, D);
}

void GMoveNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    setSelected(mIndex.data(GraphicsRole::SelectionRole).toBool());
    QColor c(mIndex.data(GraphicsRole::SelectionRole).toBool() ? Qt::red : Qt::blue);
    painter->setBrush(QBrush(c));
    painter->setPen(QPen(Qt::NoPen));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(boundingRect());
}
