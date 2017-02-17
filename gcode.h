#ifndef GCODE_H
#define GCODE_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QMap>
#include <QRectF>

#include "gline.h"
#include "gmove.h"

class GCode : public QObject
{
    Q_OBJECT
public:
    explicit GCode(QObject *parent = 0);
    ~GCode();
    
    bool read(const QString &fileName);

    int linesCount() const { return mLines.size(); }
    int movesCount() const { return mMoves.size(); }
    
    enum SpeedUnits {
        mmPerMin,
        mmPerS
    };
    
    // G-Code Lines
    QString line(int line) const { return mLines.at(line)->line(); }
    QString command(int line) const { return mLines.at(line)->command(); }
    QString comment(int line) const { return mLines.at(line)->comment(); }
    GLine::LineType type(int line) const { return mLines.at(line)->type(); }
    QString code(int line) const { return mLines.at(line)->code(); }

    // Moves
    int lineToMove(int line, bool search = false);
    int moveToLine(int moveRow);
    double X(int moveRow) const;
    double Y(int moveRow) const;
    double Z(int moveRow) const;
    double E(int moveRow) const;
    double F(int moveRow) const;
    double distance(int moveRow) const;
    double deltaE(int moveRow) const;
    double flow(int moveRow) const;

    // Information
//    QRectF bounds() const;
    
    // Selection
    bool selected(int row) const { return mSelection.at(row); }
    void selectAll();
    void select(int row);
    void select(int first, int last);
    void deselectAll();
    void deselect(int row);
    void deselect(int first, int last);
    bool toggleSelection(int row);
    void toggleSelection(int first, int last);
    
signals:
    void beginReset();
    void endReset();
    void changed(int top, int bottom);
    
public slots:
    
private:
    void clearData();
    void buildMapping();
    void clearMapping();
    
//    void updateBounds(double X, double Y);
//    void resetBounds() { mWidth = 100; mHeight = 100; }
    
    SpeedUnits mSpeedUnis;
    
    QList<GLine*> mLines;
    QList<GMove*> mMoves;
    QMap<int, GMove*> mMovesMap;
    
    QVector<bool> mSelection;
    QVector<int> mMLMap;
    QVector<int> mLMMap;
    
//    double mWidth;
//    double mHeight;
};

#endif // GCODE_H
