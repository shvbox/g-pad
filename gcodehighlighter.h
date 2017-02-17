#ifndef GCODEHIGHLIGHTER_H
#define GCODEHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

class GCodeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    GCodeHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };
    
    QVector<HighlightingRule> mHighlightingRules;

    QTextCharFormat mGCodeMovesFormat;
    QTextCharFormat mGCodeGeneralFormat;
    QTextCharFormat mRepRapFormat;
    QTextCharFormat mToolSelectFormat;
    QTextCharFormat mCoordinateFormat;
    QTextCharFormat mParameterFormat;
    QTextCharFormat mCommentFormat;
};
#endif // GCODEHIGHLIGHTER_H
