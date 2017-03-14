#include "gmoveline.h"

#include <math.h>
#include <QPainter>

#include "g.h"
#include "gmovenode.h"
#include "gmove.h"
#include "gmovesmodel.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static const double Pi3 = Pi / 3;
static const double Pi23 = Pi - Pi3;
static const double TwoPi = 2.0 * Pi;

static const qreal A = 0.1;
static const qreal HA = A / 2;
static const qreal W = 0;
static const qreal HW = W / 2;
static const qreal ADJ = HA + HW;


GMoveLine::GMoveLine(GMoveNode *src, GMoveNode *dst, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mSrc(src),
      mDst(dst)
{
//    setCacheMode(ItemCoordinateCache);
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
    
    return QRectF(mLine.p1(), mLine.p2()).normalized().adjusted(-ADJ, -ADJ, ADJ, ADJ);
}

void GMoveLine::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    qreal length = mLine.length();
    
    if (qFuzzyCompare(length + 1, qreal(1.0))) {
        return;
    }
    
//    if (!(mSrc->isVisible() || mDst->isVisible())) {
    if (!mDst->isVisible()) {
        return;
    }
    
    bool selected = (mSrc ? mSrc->isSelected() : false) && (mDst ? mDst->isSelected() : false);
    QBrush b(Qt::SolidPattern);
    if (selected) {
        b.setColor(Qt::darkRed);
        
    } else {
        if (mDst) {
            switch (mDst->index().data(G::MoveTypeRole).toInt()) {
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
    
    // Draw the arrow
    if (length > 3 * A) {
        double angle = acos(mLine.dx() / length);
        if (mLine.dy() >= 0) {
            angle = TwoPi - angle;
        }
    
        qreal pos = 1 - 2 * A / length;
        QPointF ap0 = mLine.pointAt(pos < 0.6 ? 0.6 : pos);
        QPointF ap1 = ap0 + QPointF(sin(angle - Pi3) * A, cos(angle - Pi3) * A);
        QPointF ap2 = ap0 + QPointF(sin(angle - Pi23) * A, cos(angle - Pi23) * A);
    
        painter->drawPolyline(QPolygonF() << ap1 <<  ap0 << ap2);
    }
}
