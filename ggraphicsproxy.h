#ifndef GGRAPHICSPROXY_H
#define GGRAPHICSPROXY_H

#include <QGraphicsScene>
#include <QHash>

class GMovesModel;
class QModelIndex;
class GMoveNode;
class GMoveArcNode;

class GGraphicsProxy : public QGraphicsScene
{
    Q_OBJECT
public:
    GGraphicsProxy(GMovesModel *model, QObject *parent = 0);
    ~GGraphicsProxy();
    
public slots:
//    void updateData(int top, int bottom);

protected slots:
    void resetData();
    void layoutChanged();
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    
private:
    GMovesModel *mModel;
    qreal mMargin;
    
    QHash<QPersistentModelIndex, GMoveNode*> mIndexToNodes;
    QHash<QPersistentModelIndex, GMoveArcNode*> mIndexToArcNodes;
    
    // QGraphicsScene interface
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // GGRAPHICSPROXY_H
