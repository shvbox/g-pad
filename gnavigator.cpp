#include "gnavigator.h"

#include "gcode.h"
#include "gnavigatoritem.h"

GNavigator::GNavigator(GCode *data, QObject *parent) 
    : QObject(parent),
      mGCode(data),
      mRootItem(NULL)
{
    connect(mGCode, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mGCode, SIGNAL(endReset()), this, SLOT(endResetData()));
    
    setupModelData();
}

GNavigator::~GNavigator()
{
    delete mRootItem;
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
    mRootItem = new GNavigatorItem(dumb);
    
    double zp = 0.0;

    for (int line = 0; line < mGCode->linesCount(); ++line) {
        QList<QVariant> data1;
        int move = mGCode->lineToMove(line);
        if (move > 0) {
            double z = mGCode->Z(move);
            if (z != zp) {
                data1 << (line + 1) << (QString("Z = %1").arg(z));
                
                GNavigatorItem *child1 = new GNavigatorItem(data1, mRootItem);
                mRootItem->appendChild(child1);
                zp = z;
                
                for (int j = 0; j < 2; ++j) {
                    QList<QVariant> data2;
                    data2 << (j + 1) << ("test" + j);
                    
                    GNavigatorItem *child2 = new GNavigatorItem(data2, child1);
                    child1->appendChild(child2);
                }
            }
        }
    }
}

