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
    serialcommhandler.cpp \
    serialthread.cpp \
    SerialGui.cpp \
    RobotCmdGui.cpp \
    SerialBoostHandler.cpp

HEADERS  += \
    serialcommhandler.h \
    serialthread.h \
    SerialGui.h \
    RobotCmdGui.h \
    SerialBoostHandler.h


INCLUDEPATH += J:\Boost\boost_1_59_0
LIBPATH     += J:\Boost\boost_1_59_0\stage\lib

FORMS    += \
    SerialGui.ui \
    RobotCmdGui.ui

LIBS += -LJ:/Boost\boost_1_59_0/stage/lib
LIBS += -lboost_system-mgw49-mt-d-1_59 \
        -lboost_thread-mgw49-mt-d-1_59 \
        -lboost_serialization-mgw49-mt-d-1_59 \
        -lboost_regex-mgw49-mt-d-1_59 \
        -lboost_date_time-mgw49-mt-d-1_59\
        -lwsock32 \
        -lws2_32

