#-------------------------------------------------
#
# Project created by QtCreator 2018-04-11T18:21:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = render
TEMPLATE = app

CONFIG += c++14

LIBS += -L/usr/lib/x86_64-linux-gnu/ \
  -lboost_system

SOURCES += main.cpp \
    main_window.cpp \
    backend.cpp \
    controller.cpp \
    vizualizer.cpp

HEADERS  += \
    ui_main.h \
    main_window.h \
    backend.h \
    controller.h \
    vizualizer.h

FORMS    += main.ui
