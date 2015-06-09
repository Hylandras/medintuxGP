#-------------------------------------------------
#
# Project created by QtCreator 2013-11-05T10:01:20
#
#-------------------------------------------------

QT       += core

ROOT_PROJECT        = $$PWD/../
SRC_DIR             = $${ROOT_PROJECT}/src/
DESTDIR             = $${ROOT_PROJECT}/bin

# ......... define building path ..................
unix:OBJECTS_DIR   = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR  = $${SRC_DIR}/obj/win32
unix:OBJECTS_DIR   = $${SRC_DIR}/obj/mac
UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc

win32 {
    CONFIG   += console
}
# CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp \


