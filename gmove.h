#ifndef GMOVE_H
#define GMOVE_H

#include <QString>
#include <QStringList>

class GMove
{
    friend class GCode;
    
private:
    GMove();
    GMove(const QStringList &fields, const GMove &previous = GMove());
    
    static bool testCode(const QString &code);
    
    bool isValid() const { return mValid; }
    double X() const { return mX; }
    double Y() const { return mY; }
    double Z() const { return mZ; }
    double E() const { return mE; }
    double F() const { return mF; }
    int S() const { return mS; }
    double distance() const { return mLen; }
    double deltaE() const { return mDE; }
    double flow() const { return mFlow; }
    
private:
    bool mValid;
    double mX;
    double mY;
    double mZ;
    double mE;
    double mF;
    int mS;
    double mLen;
    double mDE;
    double mFlow;
};

#endif // GMOVE_H
