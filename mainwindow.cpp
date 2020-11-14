#include "mainwindow.h"
#include "widget.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QTabBar>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1024, 700);

    ui->tabWidget->removeTab(0);
    if(qApp->arguments().size() > 2 && qApp->arguments().at(1) == "--working-dir") {
        QString chemin = qApp->arguments().at(2);
        nouvelleRecherche(chemin, "");
        //QMessageBox msgBox;
        //msgBox.setText(qApp->arguments().at(1) + " " + );
        //msgBox.exec();
    }
    else {
        slotAddTab();
    }

    ui->tabWidget->addTab(new QLabel("Add tabs by pressing \"+\""), QString());
    ui->tabWidget->setTabEnabled(1, false);

    QToolButton* tb = new QToolButton(this);
    tb->setText("+");
    ui->tabWidget->tabBar()->setTabButton(1, QTabBar::RightSide, tb);

    connect(tb, SIGNAL(clicked()), this, SLOT(slotAddTab()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotCloseTab(int)));
    installEventFilter(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        Widget *widget = qobject_cast<Widget*>(ui->tabWidget->currentWidget());
        if(!widget) {
            return false;
        }
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ((key->key() == Qt::Key_Enter) || (key->key()==Qt::Key_Return) ) {
            widget->on_boutonDepart_clicked();
        } if ((key->key() == Qt::Key_Escape)) {
            widget->on_boutonArret_clicked();
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}
void MainWindow::slotAddTab()
{
    ui->tabWidget->insertTab(ui->tabWidget->count()-1, new Widget(this), tr("Recherche %1").arg(QString::number(ui->tabWidget->count())));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
}

void MainWindow::slotCloseTab(int index)
{
    delete ui->tabWidget->widget(index);
    ui->tabWidget->setCurrentIndex(qMax(index-1, 0));
}

void MainWindow::on_actionFind_triggered()
{
    Widget *widget = qobject_cast<Widget*>(ui->tabWidget->currentWidget());
    if(!widget) {
        return;
    }
    widget->find();
}

void MainWindow::on_actionNext_triggered()
{
    Widget *widget = qobject_cast<Widget*>(ui->tabWidget->currentWidget());
    if(!widget) {
        return;
    }
    widget->findNext();
}

void MainWindow::on_actionPrevious_triggered()
{
    Widget *widget = qobject_cast<Widget*>(ui->tabWidget->currentWidget());
    if(!widget) {
        return;
    }
    widget->findPrevious();
}

void MainWindow::nouvelleRecherche(QString chemin, QString texte) {
    Widget* widget = new Widget(this, chemin, texte);
    QString titre;
    if(titre == "") {
        titre = tr("Recherche %1").arg(QString::number(ui->tabWidget->count()));
    }
    else {
        titre = texte.left(15) + (texte.length() > 14 ? "..." : "");
    }
    ui->tabWidget->insertTab(ui->tabWidget->count()-1, widget, titre);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
}

void MainWindow::changeTitre(QWidget* widget, QString texte) {
    int index = ui->tabWidget->indexOf(widget);
    QString titre;
    if(texte.isEmpty()) {
        titre = tr("Recherche %1").arg(QString::number(index));
    } else {
        titre = texte.left(15) + (texte.length() > 14 ? "..." : "");
    }
    ui->tabWidget->setTabText(index, titre);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
