#include "gmovearc.h"

#include <math.h>
#include <QPainter>
#include <QDebug>

#include "gpad.h"
#include "gmovenode.h"
#include "gmovearcnode.h"
#include "gmove.h"
#include "gmovesmodel.h"

static const double PI = 3.14159265358979323846264338327950288419717;
static const double PI_D3 = PI / 3;
static const double PI_2D3 = PI - PI_D3;
static const double PI_2 = 2.0 * PI;

// Arrow sizes
static const qreal A = 0.1;
static const qreal HA = A / 2;
static const qreal W = 0;
static const qreal HW = W / 2;
static const qreal ADJ = HA + HW;


GMoveArc::GMoveArc(GMoveNode *src, GMoveNode *dst, GMoveArcNode *center, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      mSrc(src),
      mDst(dst),
      mCenter(center)
{
//    setCacheMode(ItemCoordinateCache);
    mDir = mCenter->index().data(GPad::MoveArcDirectionRole).toInt();
    mRadius = mCenter->index().data(GPad::MoveArcRadiusRole).toDouble();
    mLength = mCenter->index().data(GPad::MoveDistanceRole).toDouble();
    
    adjust();
}

void GMoveArc::adjust()
{
    QPointF topLeft = mapFromItem(mCenter, QPointF(-mRadius, mRadius));
    QPointF bottomRight = mapFromItem(mCenter, QPointF(mRadius, -mRadius));
//    qDebug() << mapFromItem(mSrc, 0, 0) << mapFromItem(mDst, 0, 0) << mDst->index().data(GPad::MoveXYRole).toPointF();
    
    mBounds = QRectF(topLeft, bottomRight).normalized();
    
    prepareGeometryChange();
    
    setZValue(qMin(mSrc ? mSrc->zValue() : 0.0, mDst ? mDst->zValue() : 0.0) - 0.0000001);
}

QRectF GMoveArc::boundingRect() const
{
    return mBounds.adjusted(-ADJ, -ADJ, ADJ, ADJ);
}

void GMoveArc::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    if (!mDst->isVisible()) {
        return;
    }
    
    if (qFuzzyCompare(mBounds.width() + 1, qreal(1.0))) {
        return;
    }
    
    bool selected = (mSrc ? mSrc->isSelected() : false) && (mDst ? mDst->isSelected() : false);
    QBrush b(Qt::SolidPattern);
    if (selected) {
        b.setColor(Qt::darkRed);
        
    } else {
        if (mDst) {
            switch (mDst->index().data(GPad::MoveTypeRole).toInt()) {
            case GMove::Travel:
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
    
    QPointF center = mCenter->pos();
    QPointF src = mSrc->pos();
    QPointF dst = mDst->pos();
    
    int srcAngle = atan2(src.y() - center.y(), src.x() - center.x()) * 2880.0 / PI;
    int dstAngle = atan2(dst.y() - center.y(), dst.x() - center.x()) * 2880.0 / PI;

    
    if (srcAngle < 0) srcAngle = 5760 + srcAngle;
    if (dstAngle < 0) dstAngle = 5760 + dstAngle;
    
    int spanAngle;
    if (mDir < 0) {
        if (dstAngle > srcAngle) srcAngle += 5760;
        spanAngle = srcAngle - dstAngle;
        
    } else {
        if (dstAngle < srcAngle) dstAngle += 5760;
        spanAngle = dstAngle - srcAngle;
    }
    
    // Flip angles around X axis
    int startAngle = 5760 - srcAngle;
    
    QPen p(b, W, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(p);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawArc(mBounds, startAngle, -mDir * spanAngle);
    
    // Draw the arrow
    if (mLength > 4 * A) {
        double cosPhi = 0.5 * A / mRadius; // Phi is angle between radius and segment with length A
        double sinPhi = sin(acos(cosPhi));

        QLineF segment = QLineF(dst, QLineF(dst, center).pointAt(A / mRadius));
        // Rotate segment to angle Fi
        segment.setP2(QPointF(segment.dx() * cosPhi - mDir * segment.dy() * sinPhi, 
                              mDir * segment.dx() * sinPhi + segment.dy() * cosPhi) + dst);
//        painter->drawLine(segment);
        
        qreal length = segment.length();
        double angle = acos(-segment.dx() / length);
        if (segment.dy() < 0) {
            angle = PI_2 - angle;
        }

        QPointF ap0 = segment.p2();
        QPointF ap1 = ap0 + QPointF(sin(angle - PI_D3) * A, cos(angle - PI_D3) * A);
        QPointF ap2 = ap0 + QPointF(sin(angle - PI_2D3) * A, cos(angle - PI_2D3) * A);
    
        painter->drawPolyline(QPolygonF() << ap1 <<  ap0 << ap2);
    }
}

