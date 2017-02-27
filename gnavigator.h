#ifndef GNAVIGATOR_H
#define GNAVIGATOR_H

#include <QObject>
#include <QHash>

#include "gcode.h"
#include "gnavigatoritem.h"

class GNavigator : public QObject
{
    Q_OBJECT
public:
    GNavigator(GCode *data, QObject *parent = 0);
    virtual ~GNavigator();
 
    
    GNavigatorItem* root() const { return mRootItem; }
//    GNavigatorItem* parent(GNavigatorItem* child) const;
//    GNavigatorItem* child(GNavigatorItem* parent) const;
    
    Qt::CheckState selected(GNavigatorItem* item) const { return testState(item, mGCode->selection()); }
    void selectAll() { mGCode->selectAll(); }
    void deselectAll() { mGCode->deselectAll(); }
    
    Qt::CheckState visible(GNavigatorItem* item) const { return testState(item, mGCode->visibility()); }
    void showAll() { mGCode->showAll(); }
    void show(GNavigatorItem* item) { mGCode->show(item->firstLine(), item->lastLine()); }
    void hideAll() { mGCode->hideAll(); }
    void hide(GNavigatorItem* item) { mGCode->hide(item->firstLine(), item->lastLine()); }
    
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
    Qt::CheckState testState(GNavigatorItem* item, const QBitArray &state) const;
    
    GCode *mGCode;
    GNavigatorItem *mRootItem;
};

#endif // GNAVIGATOR_H
