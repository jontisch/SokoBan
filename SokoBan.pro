#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T16:57:17
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SokoBan
TEMPLATE = app
CONFIG += C++11
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += global.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    menuitem.cpp \
    menuactiondelegate.cpp \
    map/map.cpp \
    map/move_stack.cpp \
    map/tile.cpp \
    map/entities/entity.cpp \
    map/entities/button.cpp \
    map/entities/door.cpp \
    levelgrid.cpp \
    audiolibrary.cpp \
    highscorelist.cpp

HEADERS  += game.h \
    mainwindow.h \
    global.h \
    menuitem.h \
    collection.h \
    menuactiondelegate.h \
    map/map.h \
    map/move_stack.h \
    map/tile.h \
    map/entities/entity.h \
    map/entities/button.h \
    map/entities/door.h \
    levelgrid.h \
    audiolibrary.h \
    highscorelist.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
