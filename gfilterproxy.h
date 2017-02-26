#ifndef GFILTERPROXY_H
#define GFILTERPROXY_H

#include <QSortFilterProxyModel>

class GAbstractTableModel;

class GFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit GFilterProxy(QObject *parent = 0);
    
signals:
    void mouseClicked(const QModelIndex &index);
    
public slots:
    void toggleFilter(bool state);
    void clicked(const QModelIndex &index);
    void sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
    
private:
    GAbstractTableModel* mGModel;
    bool mDoFilter;
    
    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent = QModelIndex()) const;
    
    // QAbstractProxyModel interface
public:
    void setSourceModel(QAbstractItemModel *sourceModel);
};

#endif // GFILTERPROXY_H
