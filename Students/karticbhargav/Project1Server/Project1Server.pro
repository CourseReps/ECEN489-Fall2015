#-------------------------------------------------
#
# Project created by QtCreator 2015-10-06T00:23:48
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Project1Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    btthread.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    btthread.h

FORMS    += mainwindow.ui
