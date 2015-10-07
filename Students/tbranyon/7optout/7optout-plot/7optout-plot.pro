#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T11:33:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = 7optout-plot
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    sqlite3.c

HEADERS  += mainwindow.h \
    qcustomplot.h \
    sqlite3.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -lsqlite3
QMAKE_CFLAGS += -lsqlite3 -ldl -lpthread

LIBS += -lpthread -ldl -lsqlite3
