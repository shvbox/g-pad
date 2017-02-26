#ifndef GCODEHIGHLIGHTERDELEGATE_H
#define GCODEHIGHLIGHTERDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class GCodeHighlighterDelegate : public QStyledItemDelegate
{
public:
    explicit GCodeHighlighterDelegate(QObject * parent = 0);
    
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // GCODEHIGHLIGHTERDELEGATE_H
