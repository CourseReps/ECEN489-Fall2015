#
#  QCustomPlot Plot Examples
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = 7optout
TEMPLATE = app

INCLUDEPATH += /usr/local/include/.

SOURCES += main.cpp\
           mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui

CONFIG += c++11

LIBS += "-L/usr/local/Cellar/boost/1.58.0/lib/" -lboost_system

LIBS += -lsqlite3 \




