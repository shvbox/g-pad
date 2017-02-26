#include "gnavigator.h"

#include <QDebug>
#include "gnavigatoritem.h"

GNavigator::GNavigator(GCode *data, QObject *parent) 
    : QObject(parent),
      mGCode(data),
      mRootItem(NULL)
{
    connect(mGCode, SIGNAL(dataChanged(int, int)), this, SIGNAL(dataChanged(int,int)));
    connect(mGCode, SIGNAL(selectionChanged(int,int)), this, SIGNAL(selectionChanged(int,int)));
    connect(mGCode, SIGNAL(visibilityChanged(int,int)), this, SIGNAL(visibilityChanged(int,int)));
    connect(mGCode, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mGCode, SIGNAL(endReset()), this, SLOT(endResetData()));
    
    setupModelData();
}

GNavigator::~GNavigator()
{
    delete mRootItem;
}

int GNavigator::testVisibility(GNavigatorItem *item)
{
    QBitArray ba(mGCode->mVisible);
    ba.fill(true, item->firstLine(), item->lastLine() + 1);
    ba ^= mGCode->mVisible;
    
    int cnt = ba.count(true);
    if (cnt == 0) {
        return Visible;
    }
    
    int len = item->lastLine() - item->firstLine() + 1;
    if (cnt == len) {
        return Invisible;
    }
    
    return PartiallyVisible;
}

//GNavigatorItem *GNavigator::parent(GNavigatorItem *child) const
//{
    
//}

//GNavigatorItem *GNavigator::child(GNavigatorItem *parent) const
//{
    
//}

void GNavigator::beginResetData()
{
    emit beginReset();
}

void GNavigator::endResetData()
{
    delete mRootItem;
    setupModelData();
//    qDebug() << __PRETTY_FUNCTION__;
    emit endReset();
}

void GNavigator::setupModelData()
{
    QList<QVariant> dumb;
    dumb << QString("root");
    mRootItem = new GNavigatorItem(0, mGCode->linesCount() - 1, dumb);
    
    double zp = 0.0;
//    GNavigatorItem *zChild = NULL;
    int firstLine = 0;

    for (int line = 0; line < mGCode->linesCount(); ++line) {
        int move = mGCode->lineToMove(line);
        if (move > 0) {
            double z = mGCode->Z(move);
            if (z != zp) {
                QList<QVariant> data1;
                data1 << (QString("Z = %1").arg(zp)) << mGCode->line(firstLine);
                GNavigatorItem *zChild = new GNavigatorItem(firstLine, line - 1, data1, mRootItem);
                mRootItem->appendChild(zChild);
                zp = z;
                firstLine = line;
                
                for (int j = 0; j < 2; ++j) {
                    QList<QVariant> data2;
                    data2 << (j + 1) << ("test" + j);
                    
                    GNavigatorItem *child2 = new GNavigatorItem(j, j, data2, zChild);
                    zChild->appendChild(child2);
                }
            }
        }
    }
}

