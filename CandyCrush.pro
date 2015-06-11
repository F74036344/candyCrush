#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T16:04:29
#
#-------------------------------------------------

QT       += core gui\
            multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CandyCrush
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sound.cpp \
    data.cpp \
    star.cpp \
    result.cpp \
    gamewindow.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    sound.h \
    data.h \
    star.h \
    result.h \
    gamewindow.h \
    settings.h

FORMS    += mainwindow.ui \
    result.ui \
    gamewindow.ui \
    settings.ui

RESOURCES += \
    resource.qrc
