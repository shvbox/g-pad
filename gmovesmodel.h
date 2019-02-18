#ifndef GMOVESMODEL_H
#define GMOVESMODEL_H

#include "gabstracttablemodel.h"

class GMovesModel : public GAbstractTableModel
{
    Q_OBJECT
public:
    GMovesModel(GCode *data, QObject *parent = 0);

    enum { CodeColumn = LineNumberColumn + 1,
           XColumn,
           YColumn,
           ZColumn, 
           RColumn, 
           EColumn, 
           EEffectiveColumn, 
           ETotalColumn, 
           dEColumn,
           DistanceColumn,
           FlowColumn,
           FColumn,
           FEffectiveColumn,
           BedColumn,
           ExtruderColumn,
           FanColumn
         };
    
signals:
    
public slots:
    
private:
    
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
//    Qt::ItemFlags flags(const QModelIndex & index) const;
//    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    // GAbstractTableModel interface
    
protected:
    int targetToSource(int targetRow) const;
    int sourceToTarget(int sourceRow, bool forward = true) const;
};

#endif // GMOVESMODEL_H
