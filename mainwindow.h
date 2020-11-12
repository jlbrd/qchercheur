#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void nouvelleRecherche(QString chemin, QString texte);
private slots:
    void slotAddTab();
    void slotCloseTab(int);
    void on_actionFind_triggered();
    void on_actionNext_triggered();
    void on_actionPrevious_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
