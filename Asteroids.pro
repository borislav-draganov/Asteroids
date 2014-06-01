#-------------------------------------------------
#
# Project created by QtCreator 2014-04-01T16:44:33
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Asteroids
TEMPLATE = app


SOURCES += main.cpp \
    asteroid.cpp \
    ship.cpp \
    saucer.cpp \
    missile.cpp \
    mainwindow.cpp \
    topscorewin.cpp

HEADERS  += \
    asteroid.h \
    ship.h \
    saucer.h \
    missile.h \
    mainwindow.h \
    topscorewin.h

OTHER_FILES +=

RESOURCES += \
    resources.qrc
