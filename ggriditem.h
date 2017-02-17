#ifndef GGRIDITEM_H
#define GGRIDITEM_H

#include <QGraphicsItem>

class GGridItem : public QGraphicsItem
{
public:
    GGridItem(QGraphicsItem * parent = 0);
    
    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
private:
//    QVector<QLineF> mPrimaryLines;
};

#endif // GGRIDITEM_H
