#ifndef GGRAPHICSVIEW_H
#define GGRAPHICSVIEW_H

#include <QGraphicsView>

class GGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GGraphicsView(QWidget *parent = 0);
    
public slots:
    void zoomIn();
    void zoomOut();
    
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    void scaleView(qreal scaleFactor);
    
    // QGraphicsView interface
protected:
    
//    // QAbstractItemView interface
//public:
//    QRect visualRect(const QModelIndex &index) const;
//    void scrollTo(const QModelIndex &index, ScrollHint hint);
//    QModelIndex indexAt(const QPoint &point) const;
    
//protected:
//    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
//    int horizontalOffset() const;
//    int verticalOffset() const;
//    bool isIndexHidden(const QModelIndex &index) const;
//    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
//    QRegion visualRegionForSelection(const QItemSelection &selection) const;
};

#endif // GGRAPHICSVIEW_H
