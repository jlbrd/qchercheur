#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include "highlighter.h"
#include <QSyntaxHighlighter>
#include <QTextEdit>

class XmlHighlighter : public Highlighter
{
    Q_OBJECT
public:
    XmlHighlighter(QObject * parent);
    XmlHighlighter(QTextDocument * parent, QRegularExpression _regularExpression);
    XmlHighlighter(QTextEdit * parent);

protected:
    virtual void highlightBlock(const QString & text);

private:
    void highlightByRegex(const QTextCharFormat & format,
                          const QRegExp & regex, const QString & text);

    void setRegexes();
    void setFormats();

private:
    QTextCharFormat     m_xmlKeywordFormat;
    QTextCharFormat     m_xmlElementFormat;
    QTextCharFormat     m_xmlAttributeFormat;
    QTextCharFormat     m_xmlValueFormat;
    QTextCharFormat     m_xmlCommentFormat;

    QList<QRegExp>      m_xmlKeywordRegexes;
    QRegExp             m_xmlElementRegex;
    QRegExp             m_xmlAttributeRegex;
    QRegExp             m_xmlValueRegex;
    QRegExp             m_xmlCommentRegex;
};

#endif // XMLHIGHLIGHTER_H
