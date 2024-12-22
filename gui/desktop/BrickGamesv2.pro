QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    brickgame.cc \
    main.cc \
    snakeqt.cc \
    ../../brick_game/tetris/tetris_backend.c \
    ../../brick_game/snake/controller/controller.cc \
    ../../brick_game/snake/model/snake.cc \
    tetrisqt.cc


HEADERS += \
    brickgame.h \
    snakeqt.h \
    tetrisqt.h \
    ../../brick_game/tetris/tetris_backend.h \
    ../../brick_game/snake/controller/controller.h \
    ../../brick_game/snake/model/snake.h

FORMS += \
    brickgame.ui

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

