#include "linenumberarea.h"
#include "textedit.h"

LineNumberArea::LineNumberArea(TextEdit *parent)  : QWidget(parent), textEdit(parent)
{

}

QSize LineNumberArea::sizeHint()
{
    return QSize(textEdit->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    textEdit->lineNumberAreaPaintEvent(event);
}
