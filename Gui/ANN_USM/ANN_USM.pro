#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T21:04:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ANN_USM
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    neatStructure.cpp \
    neatSpecification.cpp \
    neatNode.cpp \
    neatConnection.cpp \
    neatLayer.cpp \
    geneticEncoding.cpp

HEADERS  += mainWindow.h \
    neatStructure.h \
    neatSpecification.h \
    neatNode.h \
    neatConnection.h \
    neatLayer.h \
    geneticEncoding.h

FORMS    += mainWindow.ui \
    neatStructure.ui \
    neatSpecification.ui \
    neatNode.ui \
    neatConnection.ui \
    neatLayer.ui
