#ifndef GMOVEARC_H
#define GMOVEARC_H

#include <QGraphicsItem>

class GMoveNode;
class GMoveArcNode;

class GMoveArc : public QGraphicsItem
{
public:
    GMoveArc(GMoveNode* src, GMoveNode* dst, GMoveArcNode* center, QGraphicsItem * parent = 0);
    
    void adjust();
    
private:
    GMoveNode* mSrc;
    GMoveNode* mDst;
    GMoveArcNode* mCenter;
    
    int mDir; // Direction: positive CCW, negative CW
    qreal mRadius;
    qreal mLength;
    QRectF mBounds;
    
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GMOVEARC_H
