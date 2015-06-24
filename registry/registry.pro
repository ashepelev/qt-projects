#-------------------------------------------------
#
# Project created by QtCreator 2014-11-17T19:13:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = registry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    registrymodel.cpp \
    tablemodel.cpp \
    dialog.cpp \
    about.cpp \
    spinboxdelegate.cpp

HEADERS  += mainwindow.h \
    registrymodel.h \
    tablemodel.h \
    dialog.h \
    about.h \
    spinboxdelegate.h

FORMS    += mainwindow.ui \
    dialog.ui \
    about.ui

TRANSLATIONS += registry_ru.ts

OTHER_FILES += \
    last_opened.txt
