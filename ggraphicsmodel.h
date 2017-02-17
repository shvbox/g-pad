#ifndef GGRAPHICSMODEL_H
#define GGRAPHICSMODEL_H

#include <QGraphicsScene>

class GCode;
class GMoveNode;

class GGraphicsModel : public QGraphicsScene
{
    Q_OBJECT
public:
    GGraphicsModel(GCode *data, QObject *parent = 0);
    ~GGraphicsModel();
    
//    // QAbstractItemModel interface
//public:
//    QModelIndex index(int row, int column, const QModelIndex &parent) const;
//    QModelIndex parent(const QModelIndex &child) const;
//    int rowCount(const QModelIndex &parent) const;
//    int columnCount(const QModelIndex &parent) const;
//    QVariant data(const QModelIndex &index, int role) const;
    
public slots:
    void update(int top, int bottom);

protected slots:
    void resetData();
    
private:
//    void clearItems();
    
    GCode *mGCode;
    qreal mMargin;
    
//    QList<GMoveNode*> mNodes;
    
    // QGraphicsScene interface
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // GGRAPHICSMODEL_H
