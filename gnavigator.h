#ifndef GNAVIGATOR_H
#define GNAVIGATOR_H

#include <QObject>
#include <QHash>

#include "gcode.h"

//class GCode;
class GNavigatorItem;

class GNavigator : public QObject
{
    Q_OBJECT
public:
    GNavigator(GCode *data, QObject *parent = 0);
    virtual ~GNavigator();
 
    enum { 
        Visible = 0,
        PartiallyVisible,
        Invisible
    };
    
    int testVisibility(GNavigatorItem* item);
    
    GNavigatorItem* root() const { return mRootItem; }
//    GNavigatorItem* parent(GNavigatorItem* child) const;
//    GNavigatorItem* child(GNavigatorItem* parent) const;
    
signals:
    void dataChanged(int top, int bottom);
    void selectionChanged(int top, int bottom);
    void visibilityChanged(int top, int bottom);
    void beginReset();
    void endReset();
    
public slots:
    
protected slots:
    void beginResetData();
    void endResetData();
    
private:
    void setupModelData();
    
    
    GCode *mGCode;
    GNavigatorItem *mRootItem;
};

#endif // GNAVIGATOR_H
