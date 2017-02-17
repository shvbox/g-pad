#include "gabstracttablemodel.h"

#include <QKeyEvent>
#include <QDebug>
#include "gcode.h"

GAbstractTableModel::GAbstractTableModel(GCode *data, QObject *parent)
    : QAbstractTableModel(parent),
      mGCode(data),
      mKeyModifiers(0),
      mPrevRow(-1)
{
    connect(mGCode, SIGNAL(changed(int, int)), this, SLOT(update(int, int)));
    connect(mGCode, SIGNAL(beginReset()), this, SLOT(beginResetData()));
    connect(mGCode, SIGNAL(endReset()), this, SLOT(endResetData()));
}

bool GAbstractTableModel::eventFilter(QObject */*obj*/, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) {
        mLastEvent = event->type();
//        qDebug() << obj;
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        mKeyModifiers = keyEvent->modifiers();
        
        QKeySequence seq(keyEvent->key() + mKeyModifiers);
                
        if (seq == QKeySequence(QKeySequence::SelectAll)) {
            mGCode->selectAll();
            
        } else if (seq == QKeySequence(QKeySequence::Deselect)) {
            mGCode->deselectAll();
        }
    }
    
    return false;
}

void GAbstractTableModel::clicked(const QModelIndex &index)
{
    int row = rowToLine(index.row());

    if (row == mPrevRow && mKeyModifiers != 0) {
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

void GAbstractTableModel::selectionChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    int row = rowToLine(current.row());

    if (mPrevRow >= 0) {
        int col = current.column();
        
        if (col != LineNumberColumn) {
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

void GAbstractTableModel::update(int top, int bottom)
{
    emit dataChanged(index(lineToRow(top), LineNumberColumn), index(lineToRow(bottom), columnCount() - 1));
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
