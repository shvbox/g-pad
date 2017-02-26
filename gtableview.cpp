#include "gtableview.h"

#include "gabstracttablemodel.h"
#include "gfilterproxy.h"

#include <QScrollBar>
#include <QHeaderView>
#include <QApplication>
#include <QStyledItemDelegate>
#include <QDebug>

class _LineNumberDelegate : public QStyledItemDelegate
{
public:
    explicit _LineNumberDelegate(QObject * parent = 0);
    
    // QAbstractItemDelegate interface
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

_LineNumberDelegate::_LineNumberDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void _LineNumberDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);
    
    if (!(optionV4.state & QStyle::State_HasFocus)) {
        optionV4.state &= ~(QStyle::State_Active | QStyle::State_Selected);
    }

    QStyle *style = optionV4.widget ? optionV4.widget->style() : QApplication::style();

    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);
}

GTableView::GTableView(QWidget *parent)
    : QTableView(parent),
      mGModel(NULL),
      mGProxy(NULL)
{
    setItemDelegateForColumn(0, new _LineNumberDelegate(this));
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void GTableView::setModel(QAbstractItemModel *model)
{
    Q_ASSERT(model->inherits("GAbstractTableModel") || model->inherits("GFilterProxy"));
    QTableView::setModel(model);

    if (model->inherits("GAbstractTableModel")) {
        mGModel = static_cast<GAbstractTableModel*>(model);
        connect(this, SIGNAL(clicked(QModelIndex)), mGModel, SLOT(clicked(QModelIndex)));
        
    } else {
        mGProxy= static_cast<GFilterProxy*>(model);
        connect(mGProxy, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(scrollToCurrent()));
        connect(mGProxy, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(scrollToCurrent()));
        
        mGModel = static_cast<GAbstractTableModel*>(mGProxy->sourceModel());
        connect(this, SIGNAL(clicked(QModelIndex)), mGProxy, SLOT(clicked(QModelIndex)));
    }
    
    viewport()->installEventFilter(mGModel);
    installEventFilter(mGModel);
}

void GTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
//    qDebug() << __PRETTY_FUNCTION__ << current.row();
    QTableView::currentChanged(current, previous);
    mGModel->currentChanged(mGProxy ? mGProxy->mapToSource(current) : current);
}

void GTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableView::selectionChanged(selected, deselected);
    if (mGProxy) {
        scrollTo(mGProxy->index(currentIndex().row(), 0), QAbstractItemView::EnsureVisible);
        scrollTo(mGProxy->index(currentIndex().row(), 0), QAbstractItemView::EnsureVisible); // Do not delete
        
    } else {
        scrollTo(mGModel->index(currentIndex().row(), 0), QAbstractItemView::EnsureVisible);
    }
}

void GTableView::scrollToCurrent()
{
    if (mGProxy) {
        scrollTo(mGProxy->index(currentIndex().row(), 0), QAbstractItemView::EnsureVisible);
        scrollTo(mGProxy->index(currentIndex().row(), 0), QAbstractItemView::EnsureVisible); // Do not delete
    }
}
