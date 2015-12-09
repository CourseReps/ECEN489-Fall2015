#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T22:19:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = simulator1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    pid_controller.cpp \
    watersystem.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    pid_controller.h \
    watersystem.h \
    macro_value.h

FORMS    += mainwindow.ui
