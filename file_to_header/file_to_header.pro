#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T10:21:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = file_to_header
TEMPLATE = app

LIBS += \
       -lboost_system\

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.hpp

FORMS    += widget.ui
