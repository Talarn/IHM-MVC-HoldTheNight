#-------------------------------------------------
#
# Project created by QtCreator 2018-03-21T09:21:47
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HoldTheNight
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


SOURCES += \
     src\main.cpp \
     src\display.cpp \
     src\map.cpp \
     src\game.cpp \
     src\Element\element.cpp \
     src\Element\Resource\resource.cpp \
     src\Element\Building\building.cpp \
     src\Element\Unit\abstractunit.cpp \
     src\Element\Unit\AllyUnit\allyunit.cpp \
     src\Element\Unit\AllyUnit\worker.cpp \
     src\Element\Unit\AllyUnit\warrior.cpp \
     src\Element\Unit\AllyUnit\bomber.cpp \
     src\Element\Unit\AllyUnit\antiair.cpp \
     src\Element\Unit\EnemyUnit\enemyunit.cpp \
    src/Element/Unit/EnemyUnit/boss.cpp \
    src/Element/Unit/EnemyUnit/basic.cpp \
    src/Element/Unit/EnemyUnit/assassin.cpp \
    src/Element/Unit/EnemyUnit/kamikaze.cpp \
    src/settings.cpp

HEADERS += \
     include\display.h \
     include\map.h \
     include\game.h \
     include\Element\element.h \
     include\Element\Resource\resource.h \
     include\Element\Building\building.h \
     include\Element\Unit\abstractunit.h \
     include\Element\Unit\AllyUnit\allyunit.h \
     include\Element\Unit\AllyUnit\worker.h \
     include\Element\Unit\AllyUnit\warrior.h \
     include\Element\Unit\AllyUnit\bomber.h \
     include\Element\Unit\AllyUnit\antiair.h \
     include\Element\Unit\EnemyUnit\enemyunit.h \
    include/Element/Unit/EnemyUnit/boss.h \
    include/Element/Unit/EnemyUnit/basic.h \
    include/Element/Unit/EnemyUnit/assassin.h \
    include/Element/Unit/EnemyUnit/kamikaze.h \
    include/settings.h

FORMS += \
        display.ui

RESOURCES += \
    assets.qrc

RC_ICONS = icon.ico
