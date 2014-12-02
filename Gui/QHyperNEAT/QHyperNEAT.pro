#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T17:14:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QHyperNEAT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GeneticEncoding.cpp \
    UserSpecifications.cpp \
    HyperNEAT.cpp

HEADERS  += mainwindow.h \
    GeneticEncoding.h \
    UserSpecifications.h \
    HyperNEAT.h

FORMS    += mainwindow.ui
