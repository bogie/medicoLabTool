#-------------------------------------------------
#
# Project created by QtCreator 2020-04-13T17:47:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = COVAS-Labtool3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        followupview.cpp \
        labprofiledialog.cpp \
        labvalue.cpp \
        main.cpp \
        mainwindow.cpp \
        paramdialog.cpp \
        profiletreewidget.cpp \
        profileview.cpp \
        registerview.cpp \
        tabledialog.cpp \
        urinview.cpp \
        viewselectordialog.cpp

HEADERS += \
        followupview.h \
        labprofiledialog.h \
        labvalue.h \
        mainwindow.h \
        paramdialog.h \
        profiletreewidget.h \
        profileview.h \
        registerview.h \
        tabledialog.h \
        urinview.h \
        viewselectordialog.h

FORMS += \
        followupview.ui \
        labprofiledialog.ui \
        mainwindow.ui \
        paramdialog.ui \
        profileview.ui \
        registerview.ui \
        tabledialog.ui \
        urinview.ui \
        viewselectordialog.ui

TRANSLATIONS = labtool_de.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
