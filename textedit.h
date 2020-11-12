#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class QPlainTextEdit;

class TextEdit : public QPlainTextEdit
{
public:
    TextEdit(QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    //void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
private:
    QWidget *lineNumberArea;
};

#endif // TEXTEDIT_H
