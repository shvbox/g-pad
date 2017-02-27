#ifndef GABSTRACTTABLEMODEL_H
#define GABSTRACTTABLEMODEL_H

#include <QAbstractTableModel>

class GCode;

class GAbstractTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    GAbstractTableModel(GCode *data, QObject *parent = 0);
    
    enum { LineNumberColumn = 0 };

public slots:
    virtual void clicked(const QModelIndex &index);
    virtual void currentChanged(const QModelIndex &current);
    
protected slots:
    virtual void dataUpdated(int top, int bottom);
    virtual void selectionUpdated(int top, int bottom);
    virtual void visibilityUpdated(int top, int bottom);
    virtual void beginResetData();
    virtual void endResetData();
    
protected:
    virtual int targetToSource(int targetRow) const { return targetRow; }
    virtual int sourceToTarget(int sourceRow, bool forward = true) const { Q_UNUSED(forward); return sourceRow; }
    
    GCode *mGCode;

    Qt::KeyboardModifiers mKeyModifiers;
    QModelIndex mCurrentIndex;
    
    // QObject interface
public:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    
    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // GABSTRACTTABLEMODEL_H
