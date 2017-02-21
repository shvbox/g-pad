#ifndef GNAVIGATOR_H
#define GNAVIGATOR_H

#include <QObject>
#include <QHash>

class GCode;
class GNavigatorItem;

class GNavigator : public QObject
{
    Q_OBJECT
public:
    GNavigator(GCode *data, QObject *parent = 0);
    virtual ~GNavigator();
    
    GNavigatorItem* root() const { return mRootItem; }
//    GNavigatorItem* parent(GNavigatorItem* child) const;
//    GNavigatorItem* child(GNavigatorItem* parent) const;
    
signals:
    void beginReset();
    void endReset();
    
public slots:
    
private slots:
    void beginResetData();
    void endResetData();
    
private:
    void setupModelData();
    
    GCode *mGCode;
    GNavigatorItem *mRootItem;
};

#endif // GNAVIGATOR_H
