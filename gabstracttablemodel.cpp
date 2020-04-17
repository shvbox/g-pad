#include "gabstracttablemodel.h"

#include <QKeyEvent>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QTableView>
#include <QDebug>

#include "gpad.h"
#include "gcode.h"

GAbstractTableModel::GAbstractTableModel(GCode *data, QObject *parent)
    : QAbstractTableModel(parent),
      mGCode(data),
      mKeyModifiers(Qt::NoModifier)
{
    connect(mGCode, SIGNAL(dataChanged(int, int)), this, SLOT(dataUpdated(int, int)));
    connect(mGCode, SIGNAL(selectionChanged(int,int)), this, SLOT(selectionUpdated(int, int)));
    connect(mGCode, SIGNAL(visibilityChanged(int,int)), this, SLOT(visibilityUpdated(int, int)));
    connect(mGCode, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mGCode, SIGNAL(endReset()), this, SLOT(endResetData()));
}

void GAbstractTableModel::clicked(const QModelIndex &index)
{
    if (!(index.isValid() && index == mCurrentIndex)) return;
    
    // Prevent recursion
    int line = targetToSource(index.row());
    int col = index.column();
    Qt::KeyboardModifiers keyModifiers = mKeyModifiers;
    mKeyModifiers = Qt::NoModifier;

    if (col == LineNumberColumn) {
        switch (keyModifiers) {
        case Qt::SHIFT: {
            mGCode->show(line);
        }
            break;
        case Qt::CTRL: {
            mGCode->hide(line);
        }
            break;
        case Qt::ALT: {
            mGCode->toggleVisible(line);
        }
            break;
        default:
            break;
        }
        
    } else {
        switch (keyModifiers) {
        case Qt::SHIFT: {
            mGCode->select(line);
        }
            break;
        case Qt::CTRL: {
            mGCode->deselect(line);
        }
            break;
        case Qt::ALT: {
            mGCode->toggleSelection(line);
        }
            break;
        default:
            break;
        }
    }
}

void GAbstractTableModel::currentChanged(const QModelIndex &current)
{
//    qDebug() << __PRETTY_FUNCTION__ << current.isValid();
    if (!current.isValid()) return;
    
    if (!mCurrentIndex.isValid()) {
        mCurrentIndex = current;
        mKeyModifiers = Qt::NoModifier;
        
    } else {
        // Prevent recursion
        int line = targetToSource(current.row());
        int prevLine = targetToSource(mCurrentIndex.row());
        Qt::KeyboardModifiers keyModifiers = mKeyModifiers;
                
        mCurrentIndex = current;
        mKeyModifiers = Qt::NoModifier;
        
        int col = current.column();
        
        if (col == LineNumberColumn) {
            switch (keyModifiers) {
            case Qt::SHIFT: {
                mGCode->show(prevLine, line);
            }
                break;
                
            case Qt::CTRL: {
//                qDebug() << __PRETTY_FUNCTION__ << prevLine << line;
                mGCode->hide(prevLine, line);
            }
                break;
                
            case Qt::ALT: {
                mGCode->toggleVisible(prevLine, line);
            }
                break;
                
            default:
                break;
            }
            
        } else {
            switch (keyModifiers) {
            case Qt::SHIFT: {
                mGCode->select(prevLine, line);
            }
                break;
                
            case Qt::CTRL: {
//                qDebug() << __PRETTY_FUNCTION__ << prevLine << line;
                mGCode->deselect(prevLine, line);
            }
                break;
                
            case Qt::ALT: {
                mGCode->toggleSelection(prevLine, line);
            }
                break;
                
            default:
                break;
            }
        }
    }
}

void GAbstractTableModel::dataUpdated(int top, int bottom)
{
    emit dataChanged(index(sourceToTarget(top), 0), index(sourceToTarget(bottom, false), columnCount() - 1));
}

void GAbstractTableModel::selectionUpdated(int top, int bottom)
{
    QVector<int> roles;
    roles << GPad::SelectionRole
          << Qt::FontRole
          << Qt::ForegroundRole
          << Qt::BackgroundRole
          << Qt::TextAlignmentRole;
    
    emit dataChanged(index(sourceToTarget(top), LineNumberColumn + 1), index(sourceToTarget(bottom, false), columnCount() - 1), roles);
}

void GAbstractTableModel::visibilityUpdated(int top, int bottom)
{
//    qDebug() << __PRETTY_FUNCTION__ << top << bottom;
    QVector<int> roles;
    roles << GPad::VisibilityRole
          << Qt::FontRole
          << Qt::ForegroundRole
          << Qt::BackgroundRole
          << Qt::TextAlignmentRole;
    
    emit dataChanged(index(sourceToTarget(top), LineNumberColumn), index(sourceToTarget(bottom, false), LineNumberColumn), roles);
}

void GAbstractTableModel::beginResetData()
{
    beginResetModel();
}

void GAbstractTableModel::endResetData()
{
    mKeyModifiers = Qt::NoModifier;
    mCurrentIndex = QModelIndex();
    endResetModel();
}

bool GAbstractTableModel::eventFilter(QObject */*obj*/, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        mKeyModifiers = keyEvent->modifiers();
        
        QKeySequence seq(keyEvent->key() + mKeyModifiers);
//        qDebug() << obj << seq << mKeyModifiers;
                
        if (seq == QKeySequence(QKeySequence::SelectAll)) {
            if (mCurrentIndex.column() == LineNumberColumn) {
                mGCode->showAll();
                
            } else {
                mGCode->selectAll();
            }
            
        } else if (seq == QKeySequence(QKeySequence::Deselect)) {
            if (mCurrentIndex.column() == LineNumberColumn) {
                mGCode->hideAll();
                
            } else {
                mGCode->deselectAll();
            }
        }
    }
    
    return false;
}

QVariant GAbstractTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    
    int row = targetToSource(index.row());
    int col = index.column();
    
    switch(role){
    case GPad::SelectionRole: 
        return mGCode->selected(row);
        break;
        
    case GPad::VisibilityRole: 
//        qDebug() << __PRETTY_FUNCTION__ << "G::VisibilityRole" << row;
        return mGCode->visible(row);
        break;
        
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
