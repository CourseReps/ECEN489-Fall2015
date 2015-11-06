#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T18:15:39
#
#-------------------------------------------------

QT       += core gui sql bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = 2projectQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bluetooth_connect.cpp \
    btthread.cpp \
    client_connect.cpp \
    clientthread.cpp

HEADERS  += mainwindow.h \
    bluetooth_connect.h \
    bluetooththread.h \
    btthread.h \
    client_connect.h \
    clientthread.h

FORMS    += mainwindow.ui

unix: LIBS += -lbluetooth

#INCLUDEPATH += $$PWD/
#DEPENDPATH += $$PWD/
