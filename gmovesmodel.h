#ifndef GMOVESMODEL_H
#define GMOVESMODEL_H

#include "gabstracttablemodel.h"

class GMovesModel : public GAbstractTableModel
{
    Q_OBJECT
public:
    GMovesModel(GCode *data, QObject *parent = 0);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    Qt::ItemFlags flags(const QModelIndex & index) const;
    
    // GAbstractTableModel interface
protected:
    int rowToLine(int row);
    int lineToRow(int line);

public:
    enum { CodeColumn = LineNumberColumn + 1,
           XColumn,
           YColumn,
           ZColumn, 
           EColumn, 
           FColumn,
           distColumn,
           dEColumn,
           flowColumn
         };
    
signals:
    
public slots:
    
private:
    
};

#endif // GMOVESMODEL_H
