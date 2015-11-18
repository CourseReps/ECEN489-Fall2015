#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T00:29:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WaterFlowModel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modelthread.cpp

HEADERS  += mainwindow.h \
    modelthread.h

FORMS    += mainwindow.ui
