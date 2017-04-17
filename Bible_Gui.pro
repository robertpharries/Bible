#-------------------------------------------------
#
# Project created by QtCreator 2016-02-07T14:27:25
#
#-------------------------------------------------

QT       += core gui
QT += webkitwidgets

#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bible_Gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bible.cpp \
    searchdiag.cpp \
    resultsdiag.cpp \
    gotodiag.cpp \
    fontdialog.cpp \
    searchtemplnamediag.cpp \
    managetempldiag.cpp \
    control.cpp

HEADERS  += mainwindow.h \
    bible.h \
    searchdiag.h \
    resultsdiag.h \
    gotodiag.h \
    fontdialog.h \
    searchtemplnamediag.h \
    managetempldiag.h \
    control.h

FORMS    += mainwindow.ui \
    searchdiag.ui \
    resultsdiag.ui \
    gotodiag.ui \
    fontdialog.ui \
    searchtemplnamediag.ui \
    managetempldiag.ui

DISTFILES +=
