#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T19:47:37
#
#-------------------------------------------------

QT       += core gui
QT +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT+=printsupport
TARGET = final
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    dbmanager.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    dbmanager.h

FORMS    += mainwindow.ui
