#include "gcode.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <algorithm>

GCode::GCode(QObject *parent) 
    : QObject(parent),
      mSpeedUnis(Units::mmPerS)
{
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
    
    mSelected.clear();
    mVisible.clear();
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLines.append(new GLine(line));
    }
    
    int size = mLines.size();
    mSelected.resize(size);
    mSelected.fill(false);
    mVisible.resize(size);
    mVisible.fill(true);
    mMLMap.reserve(size);
    
    GMove *mpp = new GMove();
    GMove *mp = mpp;
    for (int i = 0; i < size; ++i) {
        GLine *l = mLines.at(i);
        if (GMove::testCode(l->code())) {
            mMLMap.append(i);
            GMove *m = new GMove(l->fields(), *mp);
            mMoves.append(m);
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

int GCode::moveToLine(int move)
{
    if (move < 0 || move >= mMLMap.size()) {
        return -1;
    }
    return mMLMap.at(move);
}

double GCode::X(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->X();
}

double GCode::Y(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->Y();
}

double GCode::Z(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->Z();
}

double GCode::E(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->E();
}

double GCode::F(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    float f = mMoves.at(move)->F();
    return mSpeedUnis == Units::mmPerMin ? f : f / 60;
}

double GCode::distance(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->distance();
}

double GCode::deltaE(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->deltaE();
}

double GCode::flow(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->flow();
}

GMove::MoveType GCode::moveType(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return mMoves.at(move)->type();
}

QPointF GCode::XY(int move) const
{
    Q_ASSERT(move >= 0 && move < mMoves.size());
    return QPointF(mMoves.at(move)->X(), mMoves.at(move)->Y());
}

//QRectF GCode::bounds() const
//{
//    return QRectF(0.0f, 0.0f, mWidth, mHeight);
//}

void GCode::selectAll()
{
    select(0, mLines.size() - 1);
//    mSelected.fill(true);
//    emit selectionChanged(0, mLines.size() - 1);
}

void GCode::select(int line)
{
    Q_ASSERT(line >= 0 && line < mLines.size());
    if (mSelected[line]) return;

    mSelected[line] = mVisible[line];
    emit selectionChanged(line, line);
}

void GCode::select(int firstLine, int lastLine)
{
    Q_ASSERT(firstLine >= 0 && firstLine < mLines.size());
    Q_ASSERT(lastLine >= 0 && lastLine < mLines.size());
    int min = qMin(firstLine, lastLine);
    int max = qMax(firstLine, lastLine);

    bool changed = false;
    for (int i = min; i <= max; i++) {
        changed = changed || (!mSelected[i] && mVisible[i]);
        mSelected[i] = mVisible[i];
    }
    
    if (changed) {
        emit selectionChanged(firstLine, lastLine);
    }
}

void GCode::deselectAll()
{
    deselect(0, mLines.size() - 1);
//    mSelected.fill(false);
//    emit selectionChanged(0, mLines.size() - 1);
}

void GCode::deselect(int line)
{
    Q_ASSERT(line >= 0 && line < mLines.size());
    if (!mSelected[line]) return;

    mSelected[line] = false;
    emit selectionChanged(line, line);
}

void GCode::deselect(int firstLine, int lastLine)
{
    Q_ASSERT(firstLine >= 0 && firstLine < mLines.size());
    Q_ASSERT(lastLine >= 0 && lastLine < mLines.size());
    int min = qMin(firstLine, lastLine);
    int max = qMax(firstLine, lastLine);

    bool changed = false;
    for (int i = min; i <= max; i++) {
        changed = changed || mSelected[i];
        mSelected[i] = false;
    }
    
    if (changed) {
        emit selectionChanged(firstLine, lastLine);
    }
}

bool GCode::toggleSelection(int line) 
{
    Q_ASSERT(line >= 0 && line < mLines.size());
    mSelected[line] = !mSelected[line];
    emit selectionChanged(line, line);
    return mSelected[line]; 
}

void GCode::toggleSelection(int firstLine, int lastLine)
{
    Q_ASSERT(firstLine >= 0 && firstLine < mLines.size());
    Q_ASSERT(lastLine >= 0 && lastLine < mLines.size());
    int min = qMin(firstLine, lastLine);
    int max = qMax(firstLine, lastLine);

    for (int i = min; i <= max; i++) {
        mSelected[i] = !mSelected[i];
    }
    
    emit selectionChanged(firstLine, lastLine);
}

void GCode::showAll()
{
    show(0, mLines.size() - 1);
//    mVisible.fill(true);
//    emit visibilityChanged(0, mLines.size() - 1);
}

void GCode::show(int line)
{
    Q_ASSERT(line >= 0 && line < mLines.size());
    if (mVisible[line]) return;

    mVisible[line] = true;
    emit visibilityChanged(line, line);
}

void GCode::show(int firstLine, int lastLine)
{
    Q_ASSERT(firstLine >= 0 && firstLine < mLines.size());
    Q_ASSERT(lastLine >= 0 && lastLine < mLines.size());
    int min = qMin(firstLine, lastLine);
    int max = qMax(firstLine, lastLine);

    bool changed = false;
    for (int i = min; i <= max; i++) {
        changed = changed || !mVisible[i];
        mVisible[i] = true;
    }
    
    if (changed) {
        emit visibilityChanged(firstLine, lastLine);
    }
}

void GCode::hideAll()
{
    hide(0, mLines.size() - 1);
}

void GCode::hide(int line)
{
    Q_ASSERT(line >= 0 && line < mLines.size());
    if (!mVisible[line]) return;

    mVisible[line] = false;
    deselect(line);
    emit visibilityChanged(line, line);
}

void GCode::hide(int firstLine, int lastLine)
{
    Q_ASSERT(firstLine >= 0 && firstLine < mLines.size());
    Q_ASSERT(lastLine >= 0 && lastLine < mLines.size());
    int min = qMin(firstLine, lastLine);
    int max = qMax(firstLine, lastLine);

    bool changed = false;
    for (int i = min; i <= max; i++) {
        changed = changed || mVisible[i];
        mVisible[i] = false;
    }
    
    if (changed) {
        deselect(firstLine, lastLine);
        emit visibilityChanged(firstLine, lastLine);
    }
}

bool GCode::toggleVisible(int line)
{
    Q_ASSERT(line >= 0 && line < mLines.size());
    
    mVisible[line] = !mVisible[line];
    if (!mVisible[line]) {
        deselect(line);
    }
    emit visibilityChanged(line, line);
    return mVisible[line]; 
}

void GCode::toggleVisible(int firstLine, int lastLine)
{
    Q_ASSERT(firstLine >= 0 && firstLine < mLines.size());
    Q_ASSERT(lastLine >= 0 && lastLine < mLines.size());
    int min = qMin(firstLine, lastLine);
    int max = qMax(firstLine, lastLine);

    bool hided = false;
    for (int i = min; i <= max; i++) {
        mVisible[i] = !mVisible[i];
        hided = hided || !mVisible[i];
    }
    
    if (hided) {
        deselect(firstLine, lastLine);
    }
    
    emit visibilityChanged(firstLine, lastLine);
}

