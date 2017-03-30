#-------------------------------------------------
#
# Project created by QtCreator 2017-03-23T00:23:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LevelEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        editorwindow.cpp \
    ../map.cpp \
    ../global.cpp \
    ../move_stack.cpp \
    ../tile.cpp

HEADERS  += editorwindow.h \
    ../map.h \
    ../global.h \
    ../move_stack.h \
    ../tile.h

FORMS    += editorwindow.ui

RESOURCES += \
    ../resources.qrc

DISTFILES += \
    ../SokoBan.pro.user.b0363ed
