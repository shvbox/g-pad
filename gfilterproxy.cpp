#include "gfilterproxy.h"

#include "g.h"
#include "gabstracttablemodel.h"

#include <QDebug>

GFilterProxy::GFilterProxy(QObject *parent)
    : QSortFilterProxyModel(parent),
      mDoFilter(false)
{
}

void GFilterProxy::toggleFilter(bool state)
{
    mDoFilter = state;
    invalidateFilter();
}

void GFilterProxy::clicked(const QModelIndex &index)
{
    emit mouseClicked(mapToSource(index));
}

void GFilterProxy::sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
//    qDebug() << __PRETTY_FUNCTION__ << mapFromSource(topLeft).row() << mapFromSource(bottomRight).row();
    emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight), roles);
    if (mDoFilter && roles.contains(G::VisibilityRole)) {
        invalidateFilter();
    }
}

bool GFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &/*source_parent*/) const
{
//    qDebug() << __PRETTY_FUNCTION__ << source_row;
    return !mDoFilter || sourceModel()->index(source_row, 0).data(G::VisibilityRole).toBool();
}


void GFilterProxy::setSourceModel(QAbstractItemModel *sourceModel)
{
    Q_ASSERT(sourceModel->inherits("GAbstractTableModel"));
    QSortFilterProxyModel::setSourceModel(sourceModel);
    
    mGModel = static_cast<GAbstractTableModel*>(sourceModel);
    connect(this, SIGNAL(mouseClicked(QModelIndex)), mGModel, SLOT(clicked(QModelIndex)));
    connect(mGModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), 
            this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
}
