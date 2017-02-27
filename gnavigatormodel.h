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
    enum { LineNumberColumn = 0 };
//    enum { GCodeLineColumn = LineNumberColumn + 1 };
    
signals:
    
public slots:
    void clicked(const QModelIndex &index);
    void currentChanged(const QModelIndex &current);
    
protected slots:
    void dataUpdated(int top, int bottom);
    void selectionUpdated(int top, int bottom);
    void visibilityUpdated(int top, int bottom);
    void beginResetData();
    void endResetData();
    
private:
    GNavigator *mNavigator;
    Qt::KeyboardModifiers mKeyModifiers;
    QModelIndex mCurrentIndex;
    
    // QObject interface
public:
    bool eventFilter(QObject *obj, QEvent *event);
    
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
