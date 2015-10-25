#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T10:24:32
#
#-------------------------------------------------

QT       += core gui sql

LIBS += -L/usr/include/boost -lboost_system

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets bluetooth core printsupport

TARGET = ard_bluetooth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
    json/jsoncpp.cpp \
    tcpsocket.cpp

HEADERS  += mainwindow.h \
    json/json-forwards.h \
    json/json.h \
    tcpsocket.h

FORMS    += mainwindow.ui
