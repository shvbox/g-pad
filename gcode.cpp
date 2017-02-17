#include "gcode.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <algorithm>

GCode::GCode(QObject *parent) 
    : QObject(parent),
      mSpeedUnis(mmPerS)
{
//    resetBounds();
}

GCode::~GCode()
{
    clearData();
}

void GCode::clearData()
{
    qDeleteAll(mLines);
    mLines.clear();
    qDeleteAll(mMoves);
    mMoves.clear();
}

void GCode::buildMapping()
{
    mLMMap.resize(mLines.size());
    mLMMap.fill(-1);
    
    for (int i = 0; i < mMLMap.size(); ++i) {
        mLMMap[mMLMap.at(i)] = i;
    }    
}

void GCode::clearMapping()
{
    mLMMap.clear();
    mMLMap.clear();
}

//void GCode::updateBounds(double X, double Y)
//{
//    if (X > mWidth) mWidth = X;
//    if (Y > mHeight) mHeight = Y;
//}

bool GCode::read(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    emit beginReset();
    clearMapping();
    clearData();
//    resetBounds();
    
    mSelection.clear();
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLines.append(new GLine(line));
    }
    
    int size = mLines.size();
    mSelection.resize(size);
    mSelection.fill(false);
    mMLMap.reserve(size);
    
    GMove *mpp = new GMove();
    GMove *mp = mpp;
    for (int i = 0; i < size; ++i) {
        GLine *l = mLines.at(i);
        if (GMove::testCode(l->code())) {
            mMLMap.append(i);
            GMove *m = new GMove(l->fields(), *mp);
            mMoves.append(m);
//            updateBounds(m->X(), m->Y());
            mp = m;
        }
    }
    delete mpp;
    
    buildMapping();
    
    file.close();
    
    emit endReset();
    return true;
}

int GCode::lineToMove(int line, bool search)
{
    if (line < 0 || line >= mLMMap.size()) {
        return -1;
    }
    int m = mLMMap.at(line);
    if (m >= 0 || !search) {
        return m;
    }
    
    while (m < 0 && line < mLMMap.size()) {
        m = mLMMap.at(line++);
    }
    return m;
}

int GCode::moveToLine(int moveRow)
{
    if (moveRow < 0 || moveRow >= mMLMap.size()) {
        return -1;
    }
    return mMLMap.at(moveRow);
}

double GCode::X(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    return mMoves.at(moveRow)->X();
}

double GCode::Y(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    return mMoves.at(moveRow)->Y();
}

double GCode::Z(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    return mMoves.at(moveRow)->Z();
}

double GCode::E(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    return mMoves.at(moveRow)->E();
}

double GCode::F(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    float f = mMoves.at(moveRow)->F();
    return mSpeedUnis == mmPerMin ? f : f / 60;
}

double GCode::distance(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }

//    float dX = mMoves[moveRow]->X();
//    float dY = mMoves[moveRow]->Y();
//    float dZ = mMoves[moveRow]->Z();
//    if (moveRow > 0) {
//        int prevRow = moveRow - 1;
//        dX -= mMoves[prevRow]->X();
//        dY -= mMoves[prevRow]->Y();
//        dZ -= mMoves[prevRow]->Z();
//    }
//    return qSqrt(dX * dX + dY * dY + dZ * dZ);
    return mMoves.at(moveRow)->distance();
}

double GCode::deltaE(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    
//    float dE = mMoves[moveRow]->E();
//    if (moveRow > 0) {
//        dE -= mMoves[moveRow - 1]->E();
//    }
//    return dE;
    return mMoves.at(moveRow)->deltaE();
}

double GCode::flow(int moveRow) const
{
    if (moveRow < 0 || moveRow >= mMoves.size()) {
        return 0;
    }
    
//    float d = distance(moveRow);
//    if (d == 0) {
//        return 0;
//    }
    
//    return deltaE(moveRow) / d;
    return mMoves.at(moveRow)->flow();
}

//QRectF GCode::bounds() const
//{
//    return QRectF(0.0f, 0.0f, mWidth, mHeight);
//}

void GCode::selectAll()
{
    mSelection.fill(true);
    emit changed(0, mLines.size() - 1);
}

void GCode::select(int row)
{
    if (row < 0 || row >= mLines.size()) {
        return;
    }
    mSelection[row] = true;
    emit changed(row, row);
}

void GCode::select(int first, int last)
{
    int min = first; 
    int max = last; 
    if (last < first) {
        min = last; 
        max = first; 
    }
    min = min < 0 ? 0 : min;
    max = max >= mLines.size() ? mLines.size() - 1 : max;
    
    for (int i = min; i <= max; i++) {
        mSelection[i] = true;
    }
    
    emit changed(first, last);
}

void GCode::deselectAll()
{
    mSelection.fill(false);
    emit changed(0, mLines.size() - 1);
}

void GCode::deselect(int row)
{
    if (row < 0 || row >= mLines.size()) {
        return;
    }
    mSelection[row] = false;
    emit changed(row, row);
}

void GCode::deselect(int first, int last)
{
    int min = first; 
    int max = last; 
    if (last < first) {
        min = last; 
        max = first; 
    }
    min = min < 0 ? 0 : min;
    max = max >= mLines.size() ? mLines.size() - 1 : max;

    for (int i = min; i <= max; i++) {
        mSelection[i] = false;
    }
    
    emit changed(first, last);
}

bool GCode::toggleSelection(int row) 
{
    if (row < 0 || row >= mLines.size()) {
        return false;
    }
    
    mSelection[row] = !mSelection[row];
    emit changed(row, row);
    return mSelection[row]; 
}

void GCode::toggleSelection(int first, int last)
{
    int min = first; 
    int max = last; 
    if (last < first) {
        min = last; 
        max = first; 
    }
    min = min < 0 ? 0 : min;
    max = max >= mLines.size() ? mLines.size() - 1 : max;

    for (int i = min; i <= max; i++) {
        mSelection[i] = !mSelection[i];
    }
    
    emit changed(first, last);
}

