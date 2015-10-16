#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T17:01:37
#
#-------------------------------------------------

QT       += core gui sql bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = 1projectQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    btthread.cpp \
    bluetooth_connect.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    btthread.h \
    bluetooth_connect.h

FORMS    += mainwindow.ui

else:unix: LIBS += -lbluetooth

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
