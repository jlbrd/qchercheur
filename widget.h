#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextBlock>
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
private slots:
    void on_boutonDepart_clicked();

    void on_boutonArret_clicked();

    void on_changerRepertoire_clicked();

    void on_tableResultats_itemClicked(QTableWidgetItem *item);

    void on_premier_clicked();

    void on_precedent_clicked();

    void on_suivant_clicked();

    void on_dernier_clicked();

    void on_tableResultats_customContextMenuRequested(const QPoint &pos);

    void copierTexte();
    void nouvelleRecherche();

    void on_notepadd_clicked();
    void on_apercu_customContextMenuRequested(const QPoint &pos);
    void apercuTexte(QString filename);
    void apercuImage(QString filename);

public slots:
    void slotFind(Ui::FindWidget uiFind, QString ttf=QString(), QTextDocument::FindFlags options=0, bool fromButton=false);

private:
    Ui::Widget *ui;
    bool fichierContient(QString nomFichier);
    bool repertoireExclu(QString nomFichier);
    bool stopper;
    void surligne();
    QSyntaxHighlighter *highlighter = 0;
    Find* finder;
    TextEdit* textApercu;
};

#endif // WIDGET_H
