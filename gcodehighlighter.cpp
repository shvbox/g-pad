#include "gcodehighlighter.h"

GCodeHighlighter::GCodeHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    mGCodeMovesFormat.setForeground(Qt::darkRed);
//    gWorkMovesFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bG0\\b" << "\\bG1\\b" << "\\bG2\\b" << "\\bG3\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = mGCodeMovesFormat;
        mHighlightingRules.append(rule);
    }
    
    mGCodeGeneralFormat.setForeground(Qt::darkMagenta);
    keywordPatterns.clear();
    keywordPatterns << "G4" << "G6" << "G[\\d]{2}";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = mGCodeGeneralFormat;
        mHighlightingRules.append(rule);
    }
    
    mRepRapFormat.setForeground(Qt::darkYellow);//QColor("#808000"));
    rule.pattern = QRegExp("\\bM[\\d]{1,3}\\b");
    rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
    rule.format = mRepRapFormat;
    mHighlightingRules.append(rule);
    
    mToolSelectFormat.setForeground(Qt::red);//QColor("#808000"));
    rule.pattern = QRegExp("\\bT[\\d]{1,3}\\b");
    rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
    rule.format = mToolSelectFormat;
    mHighlightingRules.append(rule);
    
    mCoordinateFormat.setForeground(Qt::darkCyan);
    keywordPatterns.clear();
    keywordPatterns << "X" << "Y" << "Z" << "E" 
                    << "U" << "V" << "W" 
                    << "I" << "J";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "(?=\\d)");
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = mCoordinateFormat;
        mHighlightingRules.append(rule);
    }
    
    mParameterFormat.setForeground(Qt::blue);
    keywordPatterns.clear();
    keywordPatterns << "F" << "S" << "P" << "D" 
                    << "H" << "R" << "N";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "(?=\\d)");
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = mParameterFormat;
        mHighlightingRules.append(rule);
    }
    
    mCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp(";[^\n]*");
    rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
    rule.format = mCommentFormat;
    mHighlightingRules.append(rule);
}

void GCodeHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, mHighlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
