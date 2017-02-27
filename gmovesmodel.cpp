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
    int line = targetToSource(move);
    int col = index.column();
    
    switch(role){
    case G::MoveXYRole: 
        return mGCode->XY(move);
        break;
        
    case G::MoveZRole: 
        return mGCode->Z(move);
        break;
        
    case G::MoveTypeRole: 
        return mGCode->moveType(move);
        break;
        
    case Qt::DisplayRole: {
        switch (col) {
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
        break;
    }
    
    return GAbstractTableModel::data(index, role);
}

QVariant GMovesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
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
            default:
                break;
            }
        }
    }

    return GAbstractTableModel::headerData(section, orientation, role);
}

//Qt::ItemFlags GMovesModel::flags(const QModelIndex &index) const
//{
//    return QAbstractTableModel::flags(index);
//    //    return QAbstractTableModel::flags(index) | (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
//}

int GMovesModel::targetToSource(int targetRow) const
{
    return mGCode->moveToLine(targetRow);
}

int GMovesModel::sourceToTarget(int sourceRow, bool forward) const
{
    return forward ? mGCode->lineToMoveForward(sourceRow) : mGCode->lineToMoveBackward(sourceRow);
}
