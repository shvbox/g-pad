#include "gmoveline.h"

#include <QPainter>

#include "gmovenode.h"
#include "gmove.h"
#include "gmovesmodel.h"


static const qreal W = 0.2;
static const qreal HW = W / 2;

GMoveLine::GMoveLine(GMoveNode *src, GMoveNode *dst, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mSrc(src),
      mDst(dst)
{
    adjust();
}

void GMoveLine::adjust()
{
    QPointF srcPoint = mSrc ? mapFromItem(mSrc, 0, 0) : QPointF();
    QPointF dstPoint = mDst ? mapFromItem(mDst, 0, 0) : srcPoint;
    
    prepareGeometryChange();
    
    setZValue(qMin(mSrc ? mSrc->zValue() : 0.0, mDst ? mDst->zValue() : 0.0) - 0.0000001);
    mLine = QLineF(srcPoint, dstPoint);
}

QRectF GMoveLine::boundingRect() const
{
    return QRectF(mLine.p1(), mLine.p2()).normalized().adjusted(-HW, -HW, HW, HW);
}

void GMoveLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (qFuzzyCompare(mLine.length() + 1, qreal(1.0))) {
        return;
    }
    
    if (!mSrc->isVisible() || !mDst->isVisible()) {
        return;
    }
    
    bool selected = (mSrc ? mSrc->isSelected() : false) && (mDst ? mDst->isSelected() : false);
    QBrush b(Qt::SolidPattern);
    if (selected) {
        b.setColor(Qt::darkRed);
        
    } else {
        if (mDst) {
            switch (mDst->index().data(GraphicsRole::TypeRole).toInt()) {
//            case GMove::Extrusion:
//                b.setColor(Qt::black);
//                break;
                
            case GMove::Traverse:
                b.setColor(Qt::darkGreen);
                break;
                
            default:
                b.setColor(Qt::black);
                break;
            }
            
        } else {
            b.setColor(Qt::black);
        }
    }
    QPen p(b, W, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(p);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawLine(mLine);
}
