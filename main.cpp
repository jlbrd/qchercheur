#include <singleapplication.h>
#include "mainwindow.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    // Allow secondary instances
    SingleApplication app( argc, argv, true/*, SingleApplication::SecondaryNotification*/ );

    MainWindow mainwindow;
    mainwindow.show();
    // If this is a secondary instance
    if( app.isSecondary() ) {
        //AllowSetForegroundWindow( DWORD( app.primaryPid() ) );
        QString chemin;
        if(app.arguments().size() > 2 && app.arguments().at(1) == "--working-dir") {
            chemin = app.arguments().at(2);
        }
        app.sendMessage( chemin.toUtf8() );
        qDebug() << "App already running.";
        qDebug() << "Primary instance PID: " << app.primaryPid();
        qDebug() << "Primary instance user: " << app.primaryUser();
        return 0;
    } else {
        QObject::connect(
                    &app,
                    &SingleApplication::receivedMessage,
                    &mainwindow,
                    &MainWindow::receivedMessage
                    );
        /*
        QObject::connect(
                    &app,
                    &SingleApplication::instanceStarted,
                    &mainwindow,
                    &MainWindow::instanceStarted
                    );
        */

    }

    return app.exec();
}
