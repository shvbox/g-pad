#include "ggraphicsproxy.h"

#include <QPainter>
#include <QDebug>

#include "gpad.h"
#include "gmovesmodel.h"
#include "gmovenode.h"
#include "gmovearcnode.h"
#include "gmoveline.h"
#include "gmovearc.h"

GGraphicsProxy::GGraphicsProxy(GMovesModel *model, QObject *parent)
    : QGraphicsScene(parent),
      mModel(model),
      mMargin(10.0)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-mMargin, -mMargin, 205.0 + mMargin * 2, 205.0 + mMargin * 2);
    
//    connect(mModel, SIGNAL(changed(int, int)), this, SLOT(update(int, int)));
    connect(mModel, SIGNAL(modelReset()), this, SLOT(resetData()));
    
    connect(mModel, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));
    connect(mModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
             this, SLOT(rowsInserted(const QModelIndex &, int, int)));
    connect(mModel, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)),
             this, SLOT(rowsAboutToBeRemoved(const QModelIndex &, int, int)));
    connect(mModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
             this, SLOT(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
}

GGraphicsProxy::~GGraphicsProxy()
{
}

//void GGraphicsProxy::updateData(int top, int bottom)
//{
    
//}

void GGraphicsProxy::resetData()
{
    clear();
    mIndexToNodes.clear();
    GMoveNode* prevNode = NULL;
    for (int i = 0; i < mModel->rowCount(); ++i) {
        QPersistentModelIndex index = mModel->index(i, 0);
        bool visible = index.data(GPad::VisibilityRole).toBool();
        GMoveNode* node = new GMoveNode(index);
        node->setVisible(visible);

        addItem(node);
        if (index.data(GPad::MoveArcDirectionRole).toInt() == 0) {
            addItem(new GMoveLine(prevNode, node));
            
        } else {
            GMoveArcNode* arcCenter = new GMoveArcNode(index);
            arcCenter->setVisible(visible);
            addItem(arcCenter);
            
            mIndexToArcNodes.insert(index, arcCenter);
            addItem(new GMoveArc(prevNode, node, arcCenter));
        }
        
        mIndexToNodes.insert(index, node);
        
        prevNode = node;
    }
}

void GGraphicsProxy::layoutChanged()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void GGraphicsProxy::rowsInserted(const QModelIndex &parent, int start, int end)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void GGraphicsProxy::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void GGraphicsProxy::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!(roles.empty() || mIndexToNodes.isEmpty())) {
        int min = topLeft.row() == -1 ? 0 : qMin(topLeft.row(), bottomRight.row());
        int max = bottomRight.row() == -1 ? mModel->rowCount() - 1 : qMax(topLeft.row(), bottomRight.row());
        
        if (roles.contains(GPad::VisibilityRole)) {
            //qDebug() << __PRETTY_FUNCTION__ << "VisibilityRole" << min << max;
            for (int i = min; i <= max; ++i) {
                QPersistentModelIndex index = mModel->index(i, 0);
                mIndexToNodes.value(index)->setVisible(index.data(GPad::VisibilityRole).toBool());
                if (mIndexToArcNodes.contains(index)) {
                    mIndexToArcNodes.value(index)->setVisible(index.data(GPad::VisibilityRole).toBool());
                }
            }
        }
        
        if (roles.contains(GPad::SelectionRole)) {
//            qDebug() << __PRETTY_FUNCTION__ << "SelectionRole";
            for (int i = min; i <= max; ++i) {
                QPersistentModelIndex index = mModel->index(i, 0);
                bool v = mModel->data(index, GPad::SelectionRole).toBool();
                
                mIndexToNodes.value(index)->setSelected(v);
                if (mIndexToArcNodes.contains(index)) {
                    mIndexToArcNodes.value(index)->setSelected(v);
                }
            }
        }
    }
    
    update(sceneRect());
}

void GGraphicsProxy::drawBackground(QPainter *painter, const QRectF &rect)
{
    //TODO: Move to view
    QRectF border = sceneRect().adjusted(mMargin, mMargin, -mMargin, -mMargin);
    qreal step = 10.0;
    qreal x = border.left() + step;
    qreal y = border.top() + step;
    
    painter->setPen(QPen(Qt::lightGray, 0));
    
    
    while (x < border.right()) {
        painter->drawLine(x, 0, x, border.bottom());
        x += step;
    }
    
    while (y < border.bottom()) {
        painter->drawLine(0, y, border.right(), y);
        y += step;
    }
    
    painter->setPen(QPen(Qt::lightGray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawRect(border);
}
