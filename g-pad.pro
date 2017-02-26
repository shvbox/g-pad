#-------------------------------------------------
#
# Project created by QtCreator 2017-01-03T17:39:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = g-pad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gcode.cpp \
    gcodehighlighter.cpp \
    gline.cpp \
    gmovesmodel.cpp \
    gmove.cpp \
    gabstracttablemodel.cpp \
    gtableview.cpp \
    gcodemodel.cpp \
    gcodehighlighterdelegate.cpp \
    ggraphicsview.cpp \
    gmovenode.cpp \
    ggraphicsproxy.cpp \
    gmoveline.cpp \
    gnavigatormodel.cpp \
    gnavigatoritem.cpp \
    gnavigator.cpp \
    gfilterproxy.cpp

HEADERS  += mainwindow.h \
    gcode.h \
    gcodehighlighter.h \
    gline.h \
    gmovesmodel.h \
    gmove.h \
    gabstracttablemodel.h \
    gtableview.h \
    gcodemodel.h \
    gcodehighlighterdelegate.h \
    ggraphicsview.h \
    gmovenode.h \
    ggraphicsproxy.h \
    gmoveline.h \
    gnavigatormodel.h \
    gnavigatoritem.h \
    gnavigator.h \
    gfilterproxy.h \
    g.h

FORMS    += mainwindow.ui

RESOURCES     = g-pad.qrc
