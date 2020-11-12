#include "find.h"
#include "widget.h"
#include <QDebug>

Find::Find(QWidget *parent, Widget* widget, QPlainTextEdit *textedit) : QWidget(parent), m_widget(widget), m_textEdit(textedit)
{
    uiFindWidget.setupUi(this);
    connect(uiFindWidget.toolClose, SIGNAL(clicked()), this, SLOT(hide()) );
    connect(uiFindWidget.editFind, SIGNAL(textChanged(QString)), this, SLOT(findWidget_textChanged(QString)) );
    connect(uiFindWidget.editFind, SIGNAL(returnPressed()), this, SLOT(findNext()) );
    connect(uiFindWidget.toolPrevious, SIGNAL(clicked()), this, SLOT(findPrevious()) );
    connect(uiFindWidget.toolNext, SIGNAL(clicked()), this, SLOT(findNext()) );
    autoHideFindWidgetTimer = new QTimer(this);
    autoHideFindWidgetTimer->setInterval(10000);
    autoHideFindWidgetTimer->setSingleShot(true);
    connect(autoHideFindWidgetTimer, SIGNAL(timeout()), this, SLOT(hide()));
    this->hide();

}

void Find::find()
{
    autoHideFindWidgetTimer->stop();
    this->show();
    uiFindWidget.editFind->setFocus(Qt::ShortcutFocusReason);
    //if ( m_textEdit->textCursor().selectedText().length() )
    uiFindWidget.editFind->setText( m_textEdit->textCursor().selectedText() );
    //else
    //    uiFindWidget.editFind->setText( m_textEdit->wordUnderCursor() );
    uiFindWidget.editFind->selectAll();
    //if (m_hideFindReplace > 0)
    //{
    autoHideFindWidgetTimer->setInterval(10000);
    autoHideFindWidgetTimer->start();
    //}

}

void Find::findPrevious()
{
    findBackward = true;
    if ( m_textEdit->textCursor().selectedText().length() )
    {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, cursor.selectedText().length());
        cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
        m_textEdit->setTextCursor(cursor);
    }
    findWidget_textChanged(uiFindWidget.editFind->text(), true);
}

void Find::findNext()
{
    findBackward = false;
    findWidget_textChanged(uiFindWidget.editFind->text(), true);
}

void Find::findWidget_textChanged(QString text, bool fromButton)
{
    int options = 0;
    autoHideFindWidgetTimer->stop();
    if ( findBackward )
        options |= QTextDocument::FindBackward;
    if ( uiFindWidget.checkWholeWords->isChecked() )
        options |= QTextDocument::FindWholeWords;
    if ( uiFindWidget.checkCase->isChecked() ) {
        options |= QTextDocument::FindCaseSensitively;
    }
    m_widget->slotFind(uiFindWidget, text, (QTextDocument::FindFlags) options, fromButton);
}

