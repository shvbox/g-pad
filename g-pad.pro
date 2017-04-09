QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = g-pad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gcodehighlighter.cpp \
    gmovesmodel.cpp \
    gabstracttablemodel.cpp \
    gtableview.cpp \
    gcodemodel.cpp \
    gcodehighlighterdelegate.cpp \
    ggraphicsview.cpp \
    gmovenode.cpp \
    ggraphicsproxy.cpp \
    gmoveline.cpp \
    gnavigatormodel.cpp \
    gfilterproxy.cpp \
    gnavigatorview.cpp

HEADERS  += mainwindow.h \
    gcodehighlighter.h \
    gmovesmodel.h \
    gabstracttablemodel.h \
    gtableview.h \
    gcodemodel.h \
    gcodehighlighterdelegate.h \
    ggraphicsview.h \
    gmovenode.h \
    ggraphicsproxy.h \
    gmoveline.h \
    gnavigatormodel.h \
    gfilterproxy.h \
    gnavigatorview.h \
    gpad.h

FORMS    += mainwindow.ui

RESOURCES     = g-pad.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Release/ -lgcodelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug -lgcodelib
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Release/ -lgcodelib
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug/ -lgcodelib

INCLUDEPATH += $$PWD/../../gcodelib/gcodelib
DEPENDPATH += $$PWD/../../gcodelib/gcodelib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug/release/libgcodelib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug/debug/libgcodelib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug/release/gcodelib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug/debug/gcodelib.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Release/libgcodelib.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../gcodelib/build-gcodelib-Desktop_Qt_5_5_1_GCC_64bit-Debug/libgcodelib.a
