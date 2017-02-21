#ifndef GNAVIGATORMODEL_H
#define GNAVIGATORMODEL_H

#include <QAbstractItemModel>

class GNavigator;
class GNavigatorItem;

class GNavigatorModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    GNavigatorModel(GNavigator *data, QObject *parent = 0);
    ~GNavigatorModel();
    
public:
//    enum { GCodeLineColumn = LineNumberColumn + 1 };
    
signals:
    
public slots:
    
protected slots:
    void beginResetData();
    void endResetData();
    
private:
//    void setupModelData();
    
    GNavigator *mNavigator;
//    GNavigatorItem *mRootItem;
    
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    
    Qt::ItemFlags flags(const QModelIndex & index) const;
    
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
};

#endif // GNAVIGATORMODEL_H
