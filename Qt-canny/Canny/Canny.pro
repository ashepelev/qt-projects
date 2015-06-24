#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T00:15:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Canny
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp \
    about.cpp

INCLUDEPATH += "C:\\opencv\\build\\include"

LIBS += -L"C:\\opencv\\build\\x86\\mingw\\bin" \
        -lopencv_core2410 \
        -lopencv_highgui2410 \
        -lopencv_imgproc2410

HEADERS  += mainwindow.h \
    worker.h \
    about.h

FORMS    += mainwindow.ui \
    about.ui
