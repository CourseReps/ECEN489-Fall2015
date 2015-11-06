#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T10:22:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets bluetooth core printsupport

TARGET = project2_controller
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    btcommunication.cpp \
    writetodatabase.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    btcommunication.h \
    writetodatabase.h \
    qcustomplot.h

FORMS    += mainwindow.ui
