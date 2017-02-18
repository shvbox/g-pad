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
    return QAbstractTableModel::flags(index);
//    return (index.column() == LineNumberColumn ? Qt::ItemIsUserCheckable : Qt::NoItemFlags) | QAbstractTableModel::flags(index);
    //    return QAbstractTableModel::flags(index) | (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}


