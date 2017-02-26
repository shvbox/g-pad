#ifndef GTABLEVIEW_H
#define GTABLEVIEW_H

#include <QTableView>

class GAbstractTableModel;
class GFilterProxy;

class GTableView : public QTableView
{
    Q_OBJECT
public:
    explicit GTableView(QWidget *parent = 0);
    
    // QAbstractItemView interface
public:
    void setModel(QAbstractItemModel *model);
    
public slots:
    
private:
    GAbstractTableModel *mGModel;
    GFilterProxy *mGProxy;
    
    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void scrollToCurrent();
};

#endif // GTABLEVIEW_H
