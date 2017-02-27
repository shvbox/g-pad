#include "gnavigatorview.h"

#include "gnavigatormodel.h"

#include <QDebug>

GNavigatorView::GNavigatorView(QWidget *parent)
    : QTreeView(parent),
      mNavModel(NULL)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void GNavigatorView::setModel(QAbstractItemModel *model)
{
    Q_ASSERT(model->inherits("GNavigatorModel"));
    QTreeView::setModel(model);

    mNavModel = static_cast<GNavigatorModel*>(model);
    connect(this, SIGNAL(clicked(QModelIndex)), mNavModel, SLOT(clicked(QModelIndex)));
    
    viewport()->installEventFilter(mNavModel);
    installEventFilter(mNavModel);
}

void GNavigatorView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
//    qDebug() << __PRETTY_FUNCTION__ << current.row();
    QTreeView::currentChanged(current, previous);
    mNavModel->currentChanged(current);
}

