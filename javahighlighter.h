#ifndef JAVAHIGHLIGHTER_H
#define JAVAHIGHLIGHTER_H

#include "highlighter.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QObject>

class QTextDocument;

class JavaHighlighter : public Highlighter
{
    Q_OBJECT
public:
    JavaHighlighter(QTextDocument *parent, QRegularExpression _regularExpression);
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

#endif // JAVAHIGHLIGHTER_H
