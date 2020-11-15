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
    void changeTitre(QWidget *, QString);
private slots:
    void slotAddTab();
    void slotCloseTab(int);
    void on_actionFind_triggered();
    void on_actionNext_triggered();
    void on_actionPrevious_triggered();
    void on_actionQuit_triggered();
public slots:
    void receivedMessage(int instanceId, QByteArray message );
    void instanceStarted();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
