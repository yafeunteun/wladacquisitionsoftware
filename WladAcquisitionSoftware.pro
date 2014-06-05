#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T10:54:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WladAcquisitionSoftware
TEMPLATE = app

# Enable C++ 2011 features
QMAKE_CXXFLAGS += -std=c++11

# QExtSerialPort Library
CONFIG += extserialport
include(3rdparty/qextserialport/src/qextserialport.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    acquisitionsettings.cpp \
    acquisitionsettingsproxy.cpp \
    data.cpp \
    serialport.cpp

HEADERS  += mainwindow.h \
    acquisitionsettings.h \
    acquisitionsettingsproxy.h \
    data.h \
    serialport.h \
    debug.h
