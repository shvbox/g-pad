#include "ggraphicsmodel.h"

#include <QPainter>
#include <QDebug>
#include "gmovenode.h"
#include "gcode.h"

GGraphicsModel::GGraphicsModel(GCode *data, QObject *parent)
    : QGraphicsScene(parent),
      mGCode(data),
      mMargin(10.0)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(0.0, 0.0, 205.0, 205.0);
//    addItem(new GGridItem());
    
    connect(mGCode, SIGNAL(changed(int, int)), this, SLOT(update(int, int)));
    connect(mGCode, SIGNAL(endReset()), this, SLOT(resetData()));
}

GGraphicsModel::~GGraphicsModel()
{
//    clearItems();
}

void GGraphicsModel::update(int top, int bottom)
{
    
}

void GGraphicsModel::resetData()
{
//    clearItems();
    clear();
    for (int i = 0; i < mGCode->movesCount(); ++i) {
        GMoveNode* node = new GMoveNode(i);
        addItem(node);
        node->setPos(mGCode->X(i) + mMargin, mGCode->Y(i) + mMargin);
    }
}

//void GGraphicsModel::clearItems()
//{
//    qDeleteAll(mNodes);
//    mNodes.clear();
//}



//QModelIndex GGraphicsModel::index(int row, int column, const QModelIndex &parent) const
//{
//}

//QModelIndex GGraphicsModel::parent(const QModelIndex &child) const
//{
//}

//int GGraphicsModel::rowCount(const QModelIndex &parent) const
//{
//    return mGCode->movesCount();
//}

//int GGraphicsModel::columnCount(const QModelIndex &parent) const
//{
//    return 2;
//}

//QVariant GGraphicsModel::data(const QModelIndex &index, int role) const
//{
//    int move = index.row();
//    int line = mGCode->moveToLine(move);
//    int col = index.column();
    
////    switch(role){
////    case Qt::DisplayRole: {
////        switch (col) {
////        case LineNumberColumn:
////            return line + 1;
////        case CodeColumn:
////            return mGCode->code(line);
////        case XColumn:
////            return mGCode->X(move);
////        case YColumn:
////            return mGCode->Y(move);
////        case ZColumn:
////            return mGCode->Z(move);
////        case EColumn:
////            return mGCode->E(move);
////        case FColumn:
////            return mGCode->F(move);
////        case distColumn:
////            return mGCode->distance(move);
////        case dEColumn:
////            return mGCode->deltaE(move);
////        case flowColumn:
////            return mGCode->flow(move);
////        default:
////            break;
////        }
//}


void GGraphicsModel::drawBackground(QPainter *painter, const QRectF &rect)
{
    QRectF bounds = sceneRect();
//    qreal h = bounds.height() - 2 * mMargin;
//    qreal w = bounds.width() - 2 * mMargin;
    qreal x = bounds.left() + mMargin;
    qreal xR = bounds.width() - mMargin;
    qreal y = bounds.top() + mMargin;
    qreal yB = bounds.height() - mMargin;
    
    QRectF border(mMargin, mMargin, xR - mMargin, yB - mMargin);
    
//    QPen pen(Qt::lightGray);
//    pen.setCosmetic(true);
    
    painter->setPen(QPen(Qt::lightGray, 0));
//    painter->setBrush(Qt::lightGray);
    
    
    while (x <= xR) {
        painter->drawLine(x, mMargin, x, yB);
        x += 10.0;
    }
    
    while (y <= yB) {
        painter->drawLine(mMargin, y, xR, y);
        y += 10.0;
    }
    
    painter->setPen(QPen(Qt::lightGray, 1));
    painter->drawRect(border);
}
