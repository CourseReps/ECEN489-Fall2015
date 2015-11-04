#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:18:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = pumpflowrate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp


HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
