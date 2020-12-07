#ifndef SETTINGS_H
#define SETTINGS_H


class QSettings;
class QStringList;
class QString;

class Settings
{
public:
    static QSettings* settings;
    Settings();
    static QSettings* appSettings();
    static QStringList repertoires();
    static void ajouterRepertoires(QString rep);
};

#endif // SETTINGS_H
