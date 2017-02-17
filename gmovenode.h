#ifndef GMOVENODE_H
#define GMOVENODE_H

#include <QGraphicsItem>

class GMove;

class GMoveNode : public QGraphicsItem
{
public:
    GMoveNode(int index, QGraphicsItem * parent = 0);
    
    int index() const { return mIndex; }
    void setIndex(int index) { mIndex = index; }
    bool selected() const { return mSelected; }
    void setSelection(bool state) { mSelected = state; }
    
signals:
    
public slots:
    
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
    int mIndex;
    bool mSelected;
};

#endif // GMOVENODE_H
