#ifndef CHERCHEFICHIER_H
#define CHERCHEFICHIER_H

#include <QRegularExpression>


class Cherchefichier : public QObject
{
    Q_OBJECT
public:
    Cherchefichier(QString _nomFichier, QRegularExpression _regularExpression);
private:
    QString nomFichier;
    QRegularExpression regularExpression;
signals:
    void fichierTrouve(const QString &fichier);
    void finished();
public slots:
    void chercher();
};

#endif // CHERCHEFICHIER_H
