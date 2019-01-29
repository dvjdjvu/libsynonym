#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T16:45:22
#
#-------------------------------------------------

QT       += core gui sql network
CONFIG   += qt stl debug_and_release build_all

DESTDIR = lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = synonym
#TEMPLATE = app
TEMPLATE = lib

# Версия библиотеки.
VERSION = 1.0.0

#DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += src/synonym.cpp \
            src/config.cpp
            #src/main.cpp

HEADERS  += src/synonym.h \
             src/config.h

lib.path = /usr/local/lib
lib.files = lib/*.so*

headers.path = /usr/local/include/synonym
headers.files += src/synonym.h \
                  src/config.h

INSTALLS += lib headers



