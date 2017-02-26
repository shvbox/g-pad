#ifndef GCODE_H
#define GCODE_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QBitArray>
#include <QPointF>

#include "g.h"
#include "gline.h"
#include "gmove.h"

class GCode : public QObject
{
    Q_OBJECT
    friend class GNavigator;

public:
    explicit GCode(QObject *parent = 0);
    ~GCode();
    
    bool read(const QString &fileName);

    int linesCount() const { return mLines.size(); }
    int movesCount() const { return mMoves.size(); }
//    int zCount() const { return mZs.size(); }
    
    // G-Code Lines
    QString line(int line) const { return mLines.at(line)->line(); }
    QString command(int line) const { return mLines.at(line)->command(); }
    QString comment(int line) const { return mLines.at(line)->comment(); }
    GLine::LineType lineType(int line) const { return mLines.at(line)->type(); }
    QString code(int line) const { return mLines.at(line)->code(); }

    // Moves
    int lineToMove(int line, bool search = false);
    int moveToLine(int move);
    double X(int move) const;
    double Y(int move) const;
    double Z(int move) const;
    double E(int move) const;
    double F(int move) const;
    double distance(int move) const;
    double deltaE(int move) const;
    double flow(int move) const;
    GMove::MoveType moveType(int move) const;
    
    QPointF XY(int move) const;

    // Information
    double zLayer(int layer) const;
    
    // Selection
    bool selected(int line) const { return mSelected.at(line); }
    void selectAll();
    void select(int line);
    void select(int firstLine, int lastLine);
    void deselectAll();
    void deselect(int line);
    void deselect(int firstLine, int lastLine);
    bool toggleSelection(int line);
    void toggleSelection(int firstLine, int lastLine);
    
    // Appearance
    bool visible(int line) const { return mVisible.testBit(line); }
    void showAll();
    void show(int line);
    void show(int firstLine, int lastLine);
    void hideAll();
    void hide(int line);
    void hide(int firstLine, int lastLine);
    bool toggleVisible(int line);
    void toggleVisible(int firstLine, int lastLine);
    
signals:
    void beginReset();
    void endReset();
    void dataChanged(int top, int bottom);
    void selectionChanged(int top, int bottom);
    void visibilityChanged(int top, int bottom);
    
public slots:
    
private:
    void clearData();
    void buildMapping();
    void clearMapping();
    
    Units::SpeedUnits mSpeedUnis;
    
    QList<GLine*> mLines;
    QList<GMove*> mMoves;
//    QVector<double> mZs;
    
    QBitArray mSelected;
    QBitArray mVisible;
    
    QVector<int> mMLMap;
    QVector<int> mLMMap;
};

#endif // GCODE_H
