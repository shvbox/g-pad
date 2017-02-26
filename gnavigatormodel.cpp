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
    connect(mNavigator, SIGNAL(dataChanged(int, int)), this, SLOT(dataUpdated(int, int)));
    connect(mNavigator, SIGNAL(selectionChanged(int,int)), this, SLOT(selectionUpdated(int, int)));
    connect(mNavigator, SIGNAL(visibilityChanged(int,int)), this, SLOT(visibilityUpdated(int, int)));
    connect(mNavigator, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mNavigator, SIGNAL(endReset()), this, SLOT(endResetData()));
}

GNavigatorModel::~GNavigatorModel()
{
}

void GNavigatorModel::clicked(const QModelIndex &index)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void GNavigatorModel::currentChanged(const QModelIndex &current)
{
    
}

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
    return 3;
}

QVariant GNavigatorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    GNavigatorItem *item = static_cast<GNavigatorItem*>(index.internalPointer());
    int line = item->firstLine();
    int col = index.column();
    
    switch(role){
    case Qt::DisplayRole:
        if (col == LineNumberColumn) {
            return (QString("%1").arg(line + 1));
        } else {
            return item->data(index.column() - 1);
        }
        break;
        
//    case Qt::FontRole:
//        if (col == LineNumberColumn) {
//            if (mGCode->visible(row)) {
//                QFont font;
//                font.setBold(true);
//                return font;
//            }
//        }
//        break;
        
    case Qt::BackgroundRole:
        if (col == LineNumberColumn) {
            switch (mNavigator->testVisibility(item)) {
            case GNavigator::Visible:
                return QBrush("#d0d0d0");
                break;
            case GNavigator::PartiallyVisible:
                return QBrush("#e0e0e0");
                break;
            case GNavigator::Invisible:
                return QBrush("#f0f0f0");
                break;
            default:
                break;
            }
//            return QBrush(mGCode->visible(line) ? QColor("#d0d0d0") : QColor("#f0f0f0"));
            
//        } else if (mGCode->selected(line)) {
//            return QBrush(QColor("#d0d0d0"));
        }
        break;
        
    case Qt::ForegroundRole:
        if (col == LineNumberColumn) {
            return QBrush(Qt::gray);
        }
        break;
        
//    case Qt::TextAlignmentRole:
//        if (col == LineNumberColumn) {
//            return Qt::AlignRight + Qt::AlignVCenter;
//        }
//        break;
    }

    return QVariant();//item->data(index.column());
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

void GNavigatorModel::dataUpdated(int top, int bottom)
{
    emit dataChanged(QModelIndex(), QModelIndex());
}

void GNavigatorModel::selectionUpdated(int top, int bottom)
{
    QVector<int> roles;
    roles << G::SelectionRole
          << Qt::FontRole
          << Qt::ForegroundRole
          << Qt::BackgroundRole
          << Qt::TextAlignmentRole;
    
    emit dataChanged(QModelIndex(), QModelIndex(), roles);
}

void GNavigatorModel::visibilityUpdated(int top, int bottom)
{
    QVector<int> roles;
    roles << G::VisibilityRole
          << Qt::FontRole
          << Qt::ForegroundRole
          << Qt::BackgroundRole
          << Qt::TextAlignmentRole;
    
    emit dataChanged(QModelIndex(), QModelIndex(), roles);
}

void GNavigatorModel::beginResetData()
{
    beginResetModel();
}

void GNavigatorModel::endResetData()
{
//    qDebug() << __PRETTY_FUNCTION__;
    endResetModel();
}
