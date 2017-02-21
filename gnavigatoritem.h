#ifndef GNAVIGATORITEM_H
#define GNAVIGATORITEM_H

#include <QList>
#include <QVariant>

class GNavigatorItem
{
public:
    explicit GNavigatorItem(const QList<QVariant> &data, GNavigatorItem *parentItem = 0);
    ~GNavigatorItem();

    void appendChild(GNavigatorItem *child);

    GNavigatorItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    GNavigatorItem *parentItem();

private:
    QList<GNavigatorItem*> mChildItems;
    QList<QVariant> mItemData;
    GNavigatorItem *mParentItem;
};

#endif // GNAVIGATORITEM_H
