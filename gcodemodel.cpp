#include "gcodemodel.h"

#include <QFont>
#include <QBrush>
#include <QDebug>
#include "gcode.h"

GCodeModel::GCodeModel(GCode *data, QObject *parent)
    : GAbstractTableModel(data, parent)
{
}

int GCodeModel::rowCount(const QModelIndex &) const
{
    return mGCode->linesCount();
}

int GCodeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant GCodeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch(role){
    case Qt::DisplayRole:
        if (col == GCodeLineColumn) {
            return mGCode->line(row);
        }
        break;
    }

    return GAbstractTableModel::data(index, role);
}

Qt::ItemFlags GCodeModel::flags(const QModelIndex &index) const
{
    return GAbstractTableModel::flags(index);
//    return (index.column() == LineNumberColumn ? Qt::ItemIsUserCheckable : Qt::NoItemFlags) | QAbstractTableModel::flags(index);
    //    return QAbstractTableModel::flags(index) | (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

QVariant GCodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == GCodeLineColumn) {
                return mLongestLine;
            }
        }
    }
    
    return GAbstractTableModel::headerData(section, orientation, role);
}


void GCodeModel::endResetData()
{
    mLongestLine = QString();
    int maxLen = mLongestLine.length();
    for (int i = 0; i < mGCode->linesCount(); ++i) {
        QString line = mGCode->line(i);
        if (line.length() > maxLen) {
            mLongestLine = line;
            maxLen = mLongestLine.length();
        }
    }
    
    mLongestLine += "1234567890";
    
    GAbstractTableModel::endResetData();
}
