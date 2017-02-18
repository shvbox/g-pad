#ifndef GMOVELINE_H
#define GMOVELINE_H

#include <QGraphicsItem>

class GMoveNode;

class GMoveLine : public QGraphicsItem
{
public:
    GMoveLine(GMoveNode* src, GMoveNode* dst, QGraphicsItem * parent = 0);
    
    void adjust();
    
private:
    GMoveNode* mSrc;
    GMoveNode* mDst;
    
    QLineF mLine;
    
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GMOVELINE_H
