#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextBlock>
#include <QRegularExpression>
#include "ui_findwidget.h"
#include "textedit.h"

class QTableWidgetItem;
class QSyntaxHighlighter;
class Find;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0, QString chemin = "", QString texte = "");
    ~Widget();
    void find();
    void findPrevious();
    void findNext();
    QString repertoire();
private slots:

    void on_changerRepertoire_clicked();

    void on_tableResultats_itemClicked(QTableWidgetItem *item);

    void on_premier_clicked();

    void on_precedent_clicked();

    void on_suivant_clicked();

    void on_dernier_clicked();

    void on_tableResultats_customContextMenuRequested(const QPoint &pos);

    void copierTexte();

    void on_notepadd_clicked();
    void apercuTexte(QString filename);
    void apercuImage(QString filename);

    void on_comboContenant_editTextChanged(const QString &arg1);

    void on_comboRepertoires_editTextChanged(const QString &arg1);

public slots:
    void on_boutonDepart_clicked();
    void on_boutonArret_clicked();
    void slotFind(Ui::FindWidget uiFind, QString ttf=QString(), QTextDocument::FindFlags options=0, bool fromButton=false);
    void nouvelleRecherche(QString);
    void threadFinished();
private:
    Ui::Widget *ui;
    bool repertoireExclu(QString nomFichier);
    bool stopper;
    QSyntaxHighlighter *highlighter = 0;
    Find* finder;
    TextEdit* textApercu;
    QRegularExpression regularExpression();
    void fichierTrouve(const QString &nomFichier);
    int nbThread;
};

#endif // WIDGET_H
