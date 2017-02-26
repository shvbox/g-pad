#ifndef GNAVIGATORITEM_H
#define GNAVIGATORITEM_H

#include <QList>
#include <QVariant>

class GNavigatorItem
{
//    friend class 
public:
    explicit GNavigatorItem(int firstLine, int lastLine, const QList<QVariant> &data, GNavigatorItem *parentItem = 0);
    ~GNavigatorItem();

    void appendChild(GNavigatorItem *child);

    GNavigatorItem *parentItem();
    GNavigatorItem *child(int row);
    
    int firstLine() const { return mFirstLine; }
    int lastLine() const { return mLastLine; }

    int row() const;
    int childCount() const;
    
    int columnCount() const;
    QVariant data(int column) const;

private:
    GNavigatorItem *mParentItem;
    QList<GNavigatorItem*> mChildItems;
    
    QList<QVariant> mItemData;
    
    int mFirstLine;
    int mLastLine;
};

#endif // GNAVIGATORITEM_H
