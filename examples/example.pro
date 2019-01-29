#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T16:45:22
#
#-------------------------------------------------

QT       += core gui sql network
CONFIG   += qt stl debug_and_release build_all

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app

SOURCES += main.cpp

LIBS += -lsynonym
