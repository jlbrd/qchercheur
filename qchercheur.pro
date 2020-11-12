#-------------------------------------------------
#
# Project created by QtCreator 2020-10-22T12:17:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qchercheur
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE += qchercheur.rc

SOURCES += \
    linenumberarea.cpp \
        main.cpp \
        mainwindow.cpp \
    textedit.cpp \
    widget.cpp \
    javahighlighter.cpp \
	find.cpp \
    xmlhighlighter.cpp

HEADERS += \
    linenumberarea.h \
        mainwindow.h \
    textedit.h \
    widget.h \
    javahighlighter.h \
	find.h \
    xmlhighlighter.h

FORMS += \
    mainwindow.ui \
    widget.ui \
	findwidget.ui

RESOURCES += \
    images.qrc
