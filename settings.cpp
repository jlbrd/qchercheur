#include "settings.h"
#include <QSettings>
#include <QDir>
#include <QList>
#include <QDebug>
#define QD qDebug() << __FILE__ << __LINE__ << ":"

Settings::Settings()
{

}

QSettings* Settings::appSettings()
{
    return Settings::settings;
}

QSettings* Settings::settings = new QSettings(QDir::home().absolutePath()+"/qchercheur.ini", QSettings::IniFormat);

QStringList Settings::repertoires() {
    return settings->value("repertoires", QStringList()).toStringList();
}

void Settings::ajouterRepertoires(QString rep) {
    if(rep == "" || !QDir(rep).exists()) {
        return;
    }
    QStringList reps = repertoires();
    if(reps.contains(rep)) {
        reps.removeOne(rep);
    }
    reps.prepend(rep);
    reps.removeDuplicates();
    while(reps.size() > 25) {
        reps.removeAt(reps.size()-1);
    }
    settings->setValue("repertoires", reps);
}
