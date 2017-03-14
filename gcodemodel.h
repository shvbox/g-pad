#ifndef GCODEMODEL_H
#define GCODEMODEL_H

#include "gabstracttablemodel.h"

class GCodeModel : public GAbstractTableModel
{
    Q_OBJECT
public:
    GCodeModel(GCode *data, QObject *parent = 0);
    
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    
    Qt::ItemFlags flags(const QModelIndex & index) const;
    
public:
    enum { GCodeLineColumn = LineNumberColumn + 1 };
    
signals:
    
public slots:
    
private:
    QString mLongestLine;
    
    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    // GAbstractTableModel interface
protected slots:
    void endResetData();
};

#endif // GCODEMODEL_H
