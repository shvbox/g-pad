#include "gmovearcnode.h"

#include <QPainter>
#include <QPointF>
#include <QDebug>

#include "gpad.h"
#include "gmove.h"
#include "gmovesmodel.h"

static const qreal D = 0.2;//0.06;
static const qreal R = D / 2;
static const qreal P = D / 3;
static const qreal DP = D + 2 * P;
static const qreal RP = R + P;

GMoveArcNode::GMoveArcNode(const QPersistentModelIndex &index, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mIndex(index)
{
    setFlag(ItemIsSelectable);// | QGraphicsItem::ItemIsMovable);
    setPos(mIndex.data(GPad::MoveArcCenterRole).toPointF());
    setZValue(mIndex.data(GPad::MoveZRole).toDouble());
}

QRectF GMoveArcNode::boundingRect() const
{
    return QRectF(-RP, -RP, DP, DP);
}

void GMoveArcNode::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    bool selected = mIndex.data(GPad::SelectionRole).toBool();
    QColor c(selected ? Qt::darkRed : Qt::darkBlue);
    c.setAlpha(0xaa);
    
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(QBrush(c));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawEllipse(boundingRect());
}


QVariant GMoveArcNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
//    qDebug() << __PRETTY_FUNCTION__ << change;
    return QGraphicsItem::itemChange(change, value);
}

