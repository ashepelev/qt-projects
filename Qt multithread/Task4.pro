#-------------------------------------------------
#
# Project created by QtCreator 2014-12-14T10:19:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Task4
TEMPLATE = app

INCLUDEPATH += "E:\\opencv\\build\\include" \
                   "E:\\opencv\\build\\include\\opencv" \
                   "E:\\opencv\\build\\include\\opencv2"

LIBS += -L"E:\\opencv\\build\\x86\\mingw\\bin" \
        -lopencv_core2410 \
        -lopencv_highgui2410 \
        -lopencv_imgproc2410

SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    worker.h

FORMS    += mainwindow.ui
