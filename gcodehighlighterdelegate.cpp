#include "gcodehighlighterdelegate.h"

#include "gcodehighlighter.h"

#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDebug>

void GCodeHighlighterDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyle *style = optionV4.widget ? optionV4.widget->style() : QApplication::style();

    QTextDocument doc;
    doc.setDefaultTextOption(QTextOption(Qt::AlignVCenter));
    doc.setDefaultFont(optionV4.font);
    new GCodeHighlighter(&doc);
    doc.setPlainText(optionV4.text);
//    qDebug() << optionV4.font;

    // Painting item without text
    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    
    QAbstractTextDocumentLayout::PaintContext ctx;

    // Highlighting text if item is selected
    if (optionV4.state & QStyle::State_Selected) {
        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));
    }

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
    
    painter->save();
    painter->translate(textRect.topLeft().x(), textRect.topLeft().y() + (textRect.height() - doc.size().height()) / 2);
//    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize GCodeHighlighterDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setDefaultFont(optionV4.font);
    doc.setPlainText(optionV4.text);

    return QSize(doc.idealWidth() + 10, doc.size().height());
}
