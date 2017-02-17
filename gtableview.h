#ifndef GTABLEVIEW_H
#define GTABLEVIEW_H

#include <QTableView>

#include "gabstracttablemodel.h"

class GTableView : public QTableView
{
    Q_OBJECT
public:
    explicit GTableView(QWidget *parent = 0);
    
    // QAbstractItemView interface
public:
    void setModel(QAbstractItemModel *model);
    
public slots:
    
    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    
private:
    GAbstractTableModel *mGModel;
};

#endif // GTABLEVIEW_H
