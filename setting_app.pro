#-------------------------------------------------
#
# Project created by QtCreator 2017-01-09T15:18:05
#
#-------------------------------------------------

QT       += core gui network printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = setting_app
TEMPLATE = app


SOURCES +=  main.cpp\
            settings.cpp \
            licensewizard.cpp \
            license.cpp

HEADERS +=  settings.h \
            licensewizard.h \
            license.h

RESOURCES += \
    app_icons.qrc

RC_FILE = app_icon.qrc

include(QtService/qtservice.pri)
