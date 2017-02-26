#include "gnavigatoritem.h"

#include <QStringList>
#include <QDebug>

GNavigatorItem::GNavigatorItem(int firstLine, int lastLine, const QList<QVariant> &data, GNavigatorItem *parent)
    : mParentItem(parent),
      mItemData(data),
      mFirstLine(firstLine),
      mLastLine(lastLine)
{
}

GNavigatorItem::~GNavigatorItem()
{
//    qDebug() << __PRETTY_FUNCTION__;
    qDeleteAll(mChildItems);
}

void GNavigatorItem::appendChild(GNavigatorItem *item)
{
    mChildItems.append(item);
}

GNavigatorItem *GNavigatorItem::child(int row)
{
    return mChildItems.value(row);
}

int GNavigatorItem::childCount() const
{
    return mChildItems.count();
}

int GNavigatorItem::columnCount() const
{
    return mItemData.count();
}

QVariant GNavigatorItem::data(int column) const
{
    return mItemData.value(column);
}

GNavigatorItem *GNavigatorItem::parentItem()
{
    return mParentItem;
}

int GNavigatorItem::row() const
{
    if (mParentItem) {
        return mParentItem->mChildItems.indexOf(const_cast<GNavigatorItem*>(this));
    }

    return 0;
}
