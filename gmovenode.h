#ifndef GMOVENODE_H
#define GMOVENODE_H

#include <QGraphicsItem>
#include <QPersistentModelIndex>

class GMoveNode : public QGraphicsItem
{
public:
    GMoveNode(const QPersistentModelIndex &index, QGraphicsItem *parent = 0);
    
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

};

#endif // GMOVENODE_H
