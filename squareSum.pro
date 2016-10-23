#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T14:47:00
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = squareSum
TEMPLATE = app


SOURCES += main.cpp\
        squareSumUI.cpp \
    findSquareSumWorker.cpp

HEADERS  += squareSumUI.h \
    findSquareSumWorker.h

FORMS    += squareSumUI.ui
