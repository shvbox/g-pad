#ifndef GNAVIGATORVIEW_H
#define GNAVIGATORVIEW_H

#include <QTreeView>

class GNavigatorModel;

class GNavigatorView : public QTreeView
{
    Q_OBJECT
public:
    explicit GNavigatorView(QWidget *parent = 0);
    
    // QAbstractItemView interface
public:
    void setModel(QAbstractItemModel *model);
    
public slots:
    
    
private:
    GNavigatorModel *mNavModel;
    
    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // GNAVIGATORVIEW_H
