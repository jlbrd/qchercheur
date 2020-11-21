#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QObject>

class QTextDocument;


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent, QRegularExpression _regularExpression);
protected:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QRegularExpression regularExpression;
    QVector<HighlightingRule> highlightingRules;
    void highlightBlock(const QString &text) override;
    bool highlightBlockRecherche(const QString &text);
private:
    QTextCharFormat rechercheFormat;
};

#endif // HIGHLIGHTER_H
