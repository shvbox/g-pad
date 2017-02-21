#include "gabstracttablemodel.h"

#include <QKeyEvent>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QDebug>

#include "gcode.h"

GAbstractTableModel::GAbstractTableModel(GCode *data, QObject *parent)
    : QAbstractTableModel(parent),
      mGCode(data),
      mKeyModifiers(0),
      mPrevRow(-1)
{
    connect(mGCode, SIGNAL(dataChanged(int, int)), this, SLOT(dataUpdated(int, int)));
    connect(mGCode, SIGNAL(selectionChanged(int,int)), this, SLOT(selectionUpdated(int, int)));
    connect(mGCode, SIGNAL(visibilityChanged(int,int)), this, SLOT(visibilityUpdated(int, int)));
    connect(mGCode, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mGCode, SIGNAL(endReset()), this, SLOT(endResetData()));
}

void GAbstractTableModel::clicked(const QModelIndex &index)
{
    int row = targetToSource(index.row());
    int col = index.column();

    if (row == mPrevRow && mKeyModifiers != 0) {
        if (col == LineNumberColumn) {
            switch (mKeyModifiers) {
            case Qt::SHIFT: {
                mGCode->show(row);
            }
                break;
            case Qt::CTRL: {
                mGCode->hide(row);
            }
                break;
            case Qt::ALT: {
                mGCode->toggleVisible(row);
            }
                break;
            default:
                break;
            }
            
        } else {
            switch (mKeyModifiers) {
            case Qt::SHIFT: {
                mGCode->select(row);
            }
                break;
            case Qt::CTRL: {
                mGCode->deselect(row);
            }
                break;
            case Qt::ALT: {
                mGCode->toggleSelection(row);
            }
                break;
            default:
                break;
            }
        }
    }
}

void GAbstractTableModel::selectionChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    int row = targetToSource(current.row());

    if (mPrevRow >= 0) {
        int col = current.column();
        
        if (col == LineNumberColumn) {
            switch (mKeyModifiers) {
            case Qt::SHIFT: {
                mGCode->show(mPrevRow, row);
            }
                break;
                
            case Qt::CTRL: {
                mGCode->hide(mPrevRow, row);
            }
                break;
                
            case Qt::ALT: {
                mGCode->toggleVisible(mPrevRow, row);
            }
                break;
                
            default:
                break;
            }
            
        } else {
            switch (mKeyModifiers) {
            case Qt::SHIFT: {
                mGCode->select(mPrevRow, row);
            }
                break;
                
            case Qt::CTRL: {
                mGCode->deselect(mPrevRow, row);
            }
                break;
                
            case Qt::ALT: {
                mGCode->toggleSelection(mPrevRow, row);
            }
                break;
                
            default:
                break;
            }
        }
    }
    
    mPrevRow = row;
}

void GAbstractTableModel::dataUpdated(int top, int bottom)
{
    emit dataChanged(index(sourceToTarget(top), 0), index(sourceToTarget(bottom), columnCount() - 1));
}

void GAbstractTableModel::selectionUpdated(int top, int bottom)
{
    emit dataChanged(index(sourceToTarget(top), LineNumberColumn + 1), index(sourceToTarget(bottom), columnCount() - 1));
}

void GAbstractTableModel::visibilityUpdated(int top, int bottom)
{
    emit dataChanged(index(sourceToTarget(top), LineNumberColumn), index(sourceToTarget(bottom), LineNumberColumn));
}

void GAbstractTableModel::beginResetData()
{
    beginResetModel();
}

void GAbstractTableModel::endResetData()
{
    mKeyModifiers = 0;
    mPrevRow = -1;
    endResetModel();
}

bool GAbstractTableModel::eventFilter(QObject */*obj*/, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) {
        mLastEvent = event->type();
//        qDebug() << obj;
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        mKeyModifiers = keyEvent->modifiers();
        
        QKeySequence seq(keyEvent->key() + mKeyModifiers);
//        qDebug() << seq;
                
        if (seq == QKeySequence(QKeySequence::SelectAll)) {
            mGCode->selectAll();
            
        } else if (seq == QKeySequence(QKeySequence::Deselect)) {
            mGCode->deselectAll();
            
        } else if (seq == QKeySequence(Qt::ALT + Qt::Key_A)) {
            mGCode->showAll();
            
        } else if (seq == QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_A)) {
            mGCode->hideAll();
        }
    }
    
    return false;
}

QVariant GAbstractTableModel::data(const QModelIndex &index, int role) const
{
    int row = targetToSource(index.row());
    int col = index.column();
    
    switch(role){
    case Qt::DisplayRole:
        if (col == LineNumberColumn) {
            return (QString("%1").arg(row + 1));
        }
        break;
        
    case Qt::FontRole:
        if (col == LineNumberColumn) {
            if (mGCode->visible(row)) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }
        break;
        
    case Qt::BackgroundRole:
        if (col == LineNumberColumn) {
            return QBrush(mGCode->visible(row) ? QColor("#d0d0d0") : QColor("#f0f0f0"));
            
        } else if (mGCode->selected(row)) {
            return QBrush(QColor("#d0d0d0"));
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

    return QVariant();
}

QVariant GAbstractTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == LineNumberColumn) {
                return mGCode->linesCount();
            }
        }
        
    } else if (role == Qt::ForegroundRole) {
        if (section == LineNumberColumn) {
            return QBrush(Qt::transparent);
        }
        
    } else if (role == Qt::FontRole) {
        if (section == LineNumberColumn) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    
    return QAbstractItemModel::headerData(section, orientation, role);
}
