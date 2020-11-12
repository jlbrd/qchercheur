#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

class TextEdit;

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(TextEdit *parent);
    QSize sizeHint() ;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
     TextEdit* textEdit;

};

#endif // LINENUMBERAREA_H
