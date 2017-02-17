#include "gmovesmodel.h"

#include <QFont>
#include <QBrush>
#include <QDebug>
#include "gcode.h"

GMovesModel::GMovesModel(GCode *data, QObject *parent)
    : GAbstractTableModel(data, parent)
{
}

int GMovesModel::rowCount(const QModelIndex &) const
{
    return mGCode->movesCount();
}

int GMovesModel::columnCount(const QModelIndex &) const
{
    return 10;
}

QVariant GMovesModel::data(const QModelIndex &index, int role) const
{
    int move = index.row();
    int line = mGCode->moveToLine(move);
    int col = index.column();
    
    switch(role){
    case Qt::DisplayRole: {
        switch (col) {
        case LineNumberColumn:
            return line + 1;
        case CodeColumn:
            return mGCode->code(line);
        case XColumn:
            return mGCode->X(move);
        case YColumn:
            return mGCode->Y(move);
        case ZColumn:
            return mGCode->Z(move);
        case EColumn:
            return mGCode->E(move);
        case FColumn:
            return mGCode->F(move);
        case distColumn:
            return mGCode->distance(move);
        case dEColumn:
            return mGCode->deltaE(move);
        case flowColumn:
            return mGCode->flow(move);
        default:
            break;
        }
    }
//        qDebug() << (gCode->line(row));
//        return mGCode->code(row);
        
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
            
        } else if (mGCode->selected(line)) {
            QBrush selectColor(QColor("#d0d0d0"));
            return selectColor;
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
//        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
//        {
//            return Qt::Checked;
//        }
        break;
    }

    return QVariant();
}

QVariant GMovesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case LineNumberColumn:
                return QString();
            case CodeColumn:
                return QString(tr("Code"));
            case XColumn:
                return QString("X");
            case YColumn:
                return QString("Y");
            case ZColumn:
                return QString("Z");
            case EColumn:
                return QString("E");
            case FColumn:
                return QString("F");
            case distColumn:
                return QString("l");
            case dEColumn:
                return QString("dE");
            case flowColumn:
                return QString("f");
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags GMovesModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
    //    return QAbstractTableModel::flags(index) | (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

int GMovesModel::rowToLine(int row)
{
    return mGCode->moveToLine(row);
}

int GMovesModel::lineToRow(int line)
{
    return mGCode->lineToMove(line);
}

