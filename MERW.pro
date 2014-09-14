#-------------------------------------------------
#
# Project created by QtCreator 2014-08-22T13:43:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MERW
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

# INCLUDEPATH

SOURCES += main.cpp\
        mainwindow.cpp\
        MERW.cpp

HEADERS  += mainwindow.h\
         MERW.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

