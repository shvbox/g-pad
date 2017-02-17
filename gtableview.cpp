#include "gtableview.h"

#include <QDebug>

GTableView::GTableView(QWidget *parent)
    : QTableView(parent)
{
}

void GTableView::setModel(QAbstractItemModel *model)
{
    Q_ASSERT(model->inherits("GAbstractTableModel"));
    
    QTableView::setModel(model);
    mGModel = static_cast<GAbstractTableModel*>(model);
    
    setSelectionMode(QAbstractItemView::SingleSelection);
    
    viewport()->installEventFilter(model);
    installEventFilter(model);
    
    connect(this, SIGNAL(clicked(QModelIndex)), mGModel, SLOT(clicked(QModelIndex)));
}

void GTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    mGModel->selectionChanged(current, previous);
    scrollTo(mGModel->index(current.row(), 0), QAbstractItemView::EnsureVisible);
}

