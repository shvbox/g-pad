#include "gnavigatormodel.h"

#include <QKeyEvent>
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
//    qDebug() << __PRETTY_FUNCTION__ << index.isValid() << (index.parent() == QModelIndex());
    if (!(index.isValid() && index == mCurrentIndex && index.parent() == QModelIndex())) return;
    
    GNavigatorItem *item = static_cast<GNavigatorItem*>(index.internalPointer());
    int col = index.column();
    
    if (col == LineNumberColumn) {
        switch (mKeyModifiers) {
        case Qt::SHIFT: {
            mNavigator->show(item);
        }
            break;
        case Qt::CTRL: {
            mNavigator->hide(item);
        }
            break;
        case Qt::ALT: {
//                mNavigator->toggleVisible(line);
        }
            break;
        default:
            break;
        }
        
    } else {
        switch (mKeyModifiers) {
        case Qt::SHIFT: {
//                mGCode->select(line);
        }
            break;
        case Qt::CTRL: {
//                mGCode->deselect(line);
        }
            break;
        case Qt::ALT: {
//                mGCode->toggleSelection(line);
        }
            break;
        default:
            break;
        }
    }
    mKeyModifiers = Qt::NoModifier;
}

void GNavigatorModel::currentChanged(const QModelIndex &current)
{
    if (!current.isValid()) return;
    
    if (!mCurrentIndex.isValid()) {
        mCurrentIndex = current;
        mKeyModifiers = Qt::NoModifier;
        
    } else {
//        // Prevent recursion
//        int line = targetToSource(current.row());
//        int prevLine = targetToSource(mCurrentIndex.row());
//        Qt::KeyboardModifiers keyModifiers = mKeyModifiers;
                
        mCurrentIndex = current;
        mKeyModifiers = Qt::NoModifier;
        
//        int col = current.column();
        
//        if (col == LineNumberColumn) {
//            switch (keyModifiers) {
//            case Qt::SHIFT: {
//                mGCode->show(prevLine, line);
//            }
//                break;
                
//            case Qt::CTRL: {
////                qDebug() << __PRETTY_FUNCTION__ << prevLine << line;
//                mGCode->hide(prevLine, line);
//            }
//                break;
                
//            case Qt::ALT: {
//                mGCode->toggleVisible(prevLine, line);
//            }
//                break;
                
//            default:
//                break;
//            }
            
//        } else {
//            switch (keyModifiers) {
//            case Qt::SHIFT: {
//                mGCode->select(prevLine, line);
//            }
//                break;
                
//            case Qt::CTRL: {
////                qDebug() << __PRETTY_FUNCTION__ << prevLine << line;
//                mGCode->deselect(prevLine, line);
//            }
//                break;
                
//            case Qt::ALT: {
//                mGCode->toggleSelection(prevLine, line);
//            }
//                break;
                
//            default:
//                break;
//            }
//        }
    }
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
        
    case Qt::FontRole:
        if (col == LineNumberColumn) {
            QFont font;
            switch (mNavigator->visible(item)) {
            case Qt::Checked:
                font.setBold(true);
                break;
            case Qt::PartiallyChecked:
                font.setWeight(QFont::DemiBold);
                break;
            default:
                break;
            }
            return font;
        }
        break;
        
    case Qt::BackgroundRole:
        if (col == LineNumberColumn) {
            switch (mNavigator->visible(item)) {
            case Qt::Checked:
                return QBrush("#d0d0d0");
                break;
            case Qt::PartiallyChecked:
                return QBrush("#e0e0e0");
                break;
            case Qt::Unchecked:
                return QBrush("#f0f0f0");
                break;
            default:
                break;
            }
            
        } else {
            switch (mNavigator->selected(item)) {
            case Qt::Checked:
                return QBrush("#d0d0d0");
                break;
            case Qt::PartiallyChecked:
                return QBrush("#e0e0e0");
                break;
            default:
                break;
            }
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

bool GNavigatorModel::eventFilter(QObject */*obj*/, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        mKeyModifiers = keyEvent->modifiers();
        
        QKeySequence seq(keyEvent->key() + mKeyModifiers);
                
        if (seq == QKeySequence(QKeySequence::SelectAll)) {
            if (mCurrentIndex.column() == LineNumberColumn) {
                mNavigator->showAll();
                
            } else {
                mNavigator->selectAll();
            }
            
        } else if (seq == QKeySequence(QKeySequence::Deselect)) {
            if (mCurrentIndex.column() == LineNumberColumn) {
                mNavigator->hideAll();
                
            } else {
                mNavigator->deselectAll();
            }
        }
    }
    
    return false;
}
