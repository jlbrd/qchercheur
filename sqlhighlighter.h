#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include "highlighter.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QObject>

class SqlHighlighter : public Highlighter
{
    Q_OBJECT
public:
    SqlHighlighter(QTextDocument *parent, QRegularExpression _regularExpression);
protected:
    void highlightBlock(const QString &text) override;

private:
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // SQLHIGHLIGHTER_H
