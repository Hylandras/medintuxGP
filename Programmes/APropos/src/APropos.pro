# -------------------------------------------------
# Project created by QtCreator 2010-06-03T10:21:40
# -------------------------------------------------
QT += core \
      gui \
      network \
      sql \
      webkit


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets
message(Qt version: $$[QT_VERSION])
#........... make the binary name and version number available in c++ code using macro definition ................
TARGET       = APropos
NUM_VERS     = 2.16.001

DEFINES	    += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES	    += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""

ROOT_PROJECT = $$PWD/../
SRC_DIR      = $${ROOT_PROJECT}/src/
DESTDIR      = $${ROOT_PROJECT}/bin
TEMPLATE     = app

SOURCES += main.cpp \
    ../../MedinTuxTools-QT4/C_DlgApropos/C_DlgApropos.cpp \
    CApp.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp
HEADERS += ../../MedinTuxTools-QT4/C_DlgApropos/C_DlgApropos.h \
    CApp.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h \
    ../../MedinTuxTools-QT4/CGestIni.h
FORMS       += ../../MedinTuxTools-QT4/C_DlgApropos/C_DlgApropos.ui
OTHER_FILES += ../bin/Ressources/Changements.html

# ......... define building path ..................
unix:OBJECTS_DIR  = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR = $${SRC_DIR}/obj/win32
mac:OBJECTS_DIR   = $${SRC_DIR}/obj/mac

UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc
TRANSLATIONS = Manager_fr.ts

greaterThan(QT_MAJOR_VERSION, 4): unix: LIBS += -L$$PWD/../../../../../../../../usr/lib/ -lsqlite


INCLUDEPATH += $$PWD/../../../../../../../../usr/include
DEPENDPATH  += $$PWD/../../../../../../../../usr/include
