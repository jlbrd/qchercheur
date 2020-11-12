#ifndef FIND_H
#define FIND_H

#include <QWidget>
#include "ui_findwidget.h"
#include <QTimer>

class Widget;
class QPlainTextEdit;

class Find : public QWidget
{
    Q_OBJECT
public:
    explicit Find(QWidget *, Widget *, QPlainTextEdit *);
    void find();
private:
    Ui::FindWidget uiFindWidget;
    QTimer* autoHideFindWidgetTimer;
    bool findBackward;
    Widget *m_widget;
    QPlainTextEdit *m_textEdit;
signals:

private slots:
    void findWidget_textChanged(QString text="", bool fromButton=false);
public slots:
    void findPrevious();
    void findNext();
};

#endif // FIND_H
