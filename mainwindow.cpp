#include "mainwindow.h"
#include "widget.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint
            | Qt::WindowSystemMenuHint
            | Qt::WindowMaximizeButtonHint
            | Qt::WindowMinimizeButtonHint
            | Qt::WindowCloseButtonHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1024, 700);

    ui->tabWidget->removeTab(0);
    slotAddTab();

    ui->tabWidget->addTab(new QLabel("Add tabs by pressing \"+\""), QString());
    ui->tabWidget->setTabEnabled(1, false);

    QToolButton* tb = new QToolButton(this);
    tb->setText("+");
    ui->tabWidget->tabBar()->setTabButton(1, QTabBar::RightSide, tb);

    connect(tb, SIGNAL(clicked()), this, SLOT(slotAddTab()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotCloseTab(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAddTab()
{
    ui->tabWidget->insertTab(ui->tabWidget->count()-1, new Widget(this), tr("Recherche %1").arg(QString::number(ui->tabWidget->count())));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count());
}

void MainWindow::slotCloseTab(int index)
{
    delete ui->tabWidget->widget(index);
}

void MainWindow::on_actionFind_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        Widget *widget = (Widget *)ui->tabWidget->currentWidget();
        widget->find();
    }
}

void MainWindow::on_actionNext_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        Widget *widget = (Widget *)ui->tabWidget->currentWidget();
        widget->findNext();
    }
}

void MainWindow::on_actionPrevious_triggered()
{
    if(ui->tabWidget->currentWidget() != 0) {
        Widget *widget = (Widget *)ui->tabWidget->currentWidget();
        widget->findPrevious();
    }
}

void MainWindow::nouvelleRecherche(QString chemin, QString texte) {
    Widget* widget = new Widget(this, chemin, texte);
    ui->tabWidget->insertTab(ui->tabWidget->count()-1, widget, tr("Recherche %1").arg(QString::number(ui->tabWidget->count())));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count());
}
