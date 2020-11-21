#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent, QRegularExpression _regularExpression)
    : QSyntaxHighlighter(parent), regularExpression(_regularExpression)
{
}

void Highlighter::highlightBlock(const QString &text)
{
    highlightBlockRecherche(text);
}

bool Highlighter::highlightBlockRecherche(const QString &text)
{
    bool trouve = false;

    HighlightingRule rule;
    rechercheFormat.setBackground(Qt::yellow);
    rule.pattern = regularExpression;
    rule.format = rechercheFormat;
    QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        trouve = true;
    }
    return trouve;
}
