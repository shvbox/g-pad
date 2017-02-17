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
    
    // QObject interface
public:
    virtual bool eventFilter(QObject *obj, QEvent *event);

public slots:
    virtual void update(int top, int bottom);
    virtual void clicked(const QModelIndex &index);
    virtual void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    
protected slots:
    void beginResetData();
    void endResetData();
    
protected:
    virtual int rowToLine(int row) { return row; }
    virtual int lineToRow(int line) { return line; }
    
    GCode *mGCode;

    Qt::KeyboardModifiers mKeyModifiers;
    int mPrevRow;
    int mLastEvent;
};

#endif // GABSTRACTTABLEMODEL_H
