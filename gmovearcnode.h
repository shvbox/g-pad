#ifndef GMOVEARCNODE_H
#define GMOVEARCNODE_H

#include <QGraphicsItem>
#include <QPersistentModelIndex>

class GMoveArcNode : public QGraphicsItem
{
public:
    GMoveArcNode(const QPersistentModelIndex &index, QGraphicsItem *parent = 0);
    
//    void setIn(QGraphicsItem* in);
//    void setOut(QGraphicsItem* out);
    
    QPersistentModelIndex index() const { return mIndex; }
//    void setIndex(int index) { mIndex = index; }
    
signals:
    
public slots:
    
private:
    QPersistentModelIndex mIndex;
    
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    
    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GMOVEARCNODE_H
