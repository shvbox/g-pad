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
        if (col == LineNumberColumn) {
            return (QString("%1").arg(row + 1));
        }

//        qDebug() << (gCode->line(row));
        return mGCode->line(row);
        break;
        
    case Qt::FontRole:
//        if (col == 0) {
//            QFont font;
//            font.setBold(true);
            
//            return font;
//        }
        break;
        
    case Qt::BackgroundRole:
        if (col == LineNumberColumn) {
            QBrush gray(QColor("#f0f0f0"));
            return gray;
            
        } else if (col == GCodeLineColumn) {
            if (mGCode->selected(row)) {
                QBrush selectColor(QColor("#d0d0d0"));
                return selectColor;
            }
        }
        break;
        
    case Qt::ForegroundRole:
        if (col == LineNumberColumn) {
            QBrush gray(Qt::gray);
            return gray;
        }
        break;
        
    case Qt::TextAlignmentRole:
        if (col == LineNumberColumn) {
            return Qt::AlignRight + Qt::AlignVCenter;
        }
        break;
        
    case Qt::CheckStateRole:
//        if (col == 0) {
//            return this->buddy(QModelIndex());
//        }
        break;
    }

    return QVariant();
}

Qt::ItemFlags GCodeModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
//    return (index.column() == LineNumberColumn ? Qt::ItemIsUserCheckable : Qt::NoItemFlags) | QAbstractTableModel::flags(index);
    //    return QAbstractTableModel::flags(index) | (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}


