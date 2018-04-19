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

SOURCES += main.cpp \
    main_window.cpp \
    client_sock.cpp \
    backend.cpp

HEADERS  += \
    ui_main.h \
    client_sock.h \
    main_window.h \
    backend.h

FORMS    += main.ui
