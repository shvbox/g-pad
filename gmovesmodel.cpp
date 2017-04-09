#include "gmovesmodel.h"

#include <QFont>
#include <QBrush>
#include <QDebug>
#include "gpad.h"
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
    return 16;
}

QVariant GMovesModel::data(const QModelIndex &index, int role) const
{
    int move = index.row();
    int line = targetToSource(move);
    int col = index.column();
    
    switch(role){
    case GPad::MoveXYRole: 
        return mGCode->XY(move);
        break;
        
    case GPad::MoveZRole: 
        return mGCode->Z(move);
        break;
        
    case GPad::MoveTypeRole: 
        return mGCode->moveType(move);
        break;
        
    case Qt::DisplayRole: {
        switch (col) {
        case CodeColumn:
            return mGCode->code(line);
        case XColumn:
            return QString::number(mGCode->X(move));
        case YColumn:
            return QString::number(mGCode->Y(move));
        case ZColumn:
            return QString::number(mGCode->Z(move));
        case EColumn:
            return QString::number(mGCode->E(move));
        case EEffectiveColumn:
            return QString::number(mGCode->Ee(move));
        case ETotalColumn:
            return QString::number(mGCode->ET(move),'f', 4);
        case FColumn:
            return QString::number(mGCode->F(move));
        case FEffectiveColumn:
            return QString::number(mGCode->Fe(move));
        case DistanceColumn:
            return QString::number(mGCode->distance(move));
        case dEColumn:
            return QString::number(mGCode->dEe(move));
        case FlowColumn:
            return QString::number(mGCode->flow(move));
        case BedColumn:
            return QString::number(mGCode->bedT(move));
        case ExtruderColumn:
            return QString::number(mGCode->extT(move));
        case FanColumn:
            return QString::number(mGCode->fanSpeed(move),'f', 1);
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
            case EEffectiveColumn:
                return QString(tr("E\'"));
            case ETotalColumn:
                return QString(tr("ΣE\'"));
            case FColumn:
                return QString("F");
            case FEffectiveColumn:
                return QString(tr("F\'"));
            case DistanceColumn:
                return QString(tr("dist."));
            case dEColumn:
                return QString(tr("ΔE\'"));
            case FlowColumn:
                return QString(tr("flow\'"));
            case BedColumn:
                return QString(tr("bed t°"));
            case ExtruderColumn:
                return QString(tr("ext. t°"));
            case FanColumn:
                return QString(tr("fan %"));
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
