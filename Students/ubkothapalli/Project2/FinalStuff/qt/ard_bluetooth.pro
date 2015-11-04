#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T10:24:32
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets bluetooth core printsupport

TARGET = ard_bluetooth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bluetoothcom.cpp \
    todatabase.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    bluetoothcom.h \
    todatabase.h \
    qcustomplot.h

FORMS    += mainwindow.ui
