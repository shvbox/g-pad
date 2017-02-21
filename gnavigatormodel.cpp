#include "gnavigatormodel.h"

#include <QFont>
#include <QBrush>
#include <QDebug>

#include "gnavigator.h"
#include "gnavigatoritem.h"

GNavigatorModel::GNavigatorModel(GNavigator *data, QObject *parent)
    : QAbstractItemModel(parent),
      mNavigator(data)
{
    connect(mNavigator, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mNavigator, SIGNAL(endReset()), this, SLOT(endResetData()));
    
//    setupModelData();
}

GNavigatorModel::~GNavigatorModel()
{
//    delete mRootItem;
}

void GNavigatorModel::beginResetData()
{
    beginResetModel();
}

//void GNavigatorModel::setupModelData()
//{
//    QList<QVariant> dumb;
//    dumb << QString("root");
//    mRootItem = new GNavigatorItem(dumb);
    
//    for (int i = 0; i < mNavigator->zCount(); ++i) {
//        QList<QVariant> data1;
//        data1 << (i + 1) << mNavigator->zLayer(i);
        
//        GNavigatorItem *child1 = new GNavigatorItem(data1, mRootItem);
//        mRootItem->appendChild(child1);
        
//        for (int j = 0; j < 2; ++j) {
//            QList<QVariant> data2;
//            data2 << (j + 1) << "test";
            
//            GNavigatorItem *child2 = new GNavigatorItem(data2, child1);
//            child1->appendChild(child2);
//        }
//    }
//}

int GNavigatorModel::rowCount(const QModelIndex &parent) const
{
    GNavigatorItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = mNavigator->root();
    } else {
        parentItem = static_cast<GNavigatorItem*>(parent.internalPointer());
    }

//    qDebug() << __PRETTY_FUNCTION__ << parentItem->childCount() << parentItem->data(0);
    return parentItem->childCount();
}

int GNavigatorModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant GNavigatorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    GNavigatorItem *item = static_cast<GNavigatorItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags GNavigatorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
//    return (index.column() == LineNumberColumn ? Qt::ItemIsUserCheckable : Qt::NoItemFlags) | QAbstractTableModel::flags(index);
    //    return QAbstractTableModel::flags(index) | (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}


QModelIndex GNavigatorModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    GNavigatorItem *parentItem;

    if (!parent.isValid()) {
        parentItem = mNavigator->root();
        
    } else {
        parentItem = static_cast<GNavigatorItem*>(parent.internalPointer());
    }

    GNavigatorItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    
    return QModelIndex();
}

QModelIndex GNavigatorModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    GNavigatorItem *childItem = static_cast<GNavigatorItem*>(child.internalPointer());
    GNavigatorItem *parentItem = childItem->parentItem();

    if (parentItem == mNavigator->root()) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}


void GNavigatorModel::endResetData()
{
//    delete mRootItem;
//    setupModelData();
//    qDebug() << __PRETTY_FUNCTION__;
    endResetModel();
}
