#-------------------------------------------------
#
# Project created by QtCreator 2015-10-17T14:28:34
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robot_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialcommhandler.cpp

HEADERS  += \
    mainwindow.h \
    serialcommhandler.h

FORMS    += mainwindow.ui
