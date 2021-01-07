#include "cherchefichier.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>


Cherchefichier::Cherchefichier(QString _nomFichier, QRegularExpression _regularExpression)
    : nomFichier(_nomFichier), regularExpression(_regularExpression)
{
    //connect(this, &Cherchefichier::finished, this, &QObject::deleteLater);
}


void Cherchefichier::chercher() {
    QFile file(nomFichier);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream s1(&file);
    bool found = regularExpression.globalMatch(s1.readAll()).hasNext();
    file.close();
    if(found) {
        emit fichierTrouve(nomFichier);
    }
    emit finished();
}
