QT += core gui \
    network \
    webkit \
    sql

message(Qt version: $$[QT_VERSION])
ROOT_PROJECT = $$PWD/../
SRC_DIR      = $${ROOT_PROJECT}/src/
DESTDIR      = $${ROOT_PROJECT}/bin

#........... make the binary name and version number available in c++ code using macro definition ................
TARGET       = synoptux
NUM_VERS     = 2.16.000
NAME_DOM     = MedinTux
NAME_ORG     = Data Medical Design
DEFINES     += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES     += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""
DEFINES     += "NAME_ORGANISATION=\"\\\"$${NAME_ORG}\\\"\""
DEFINES     += "NAME_DOMAINE=\"\\\"$${NAME_DOM}\\\"\""
TEMPLATE     = app

include(../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.pri)

SOURCES = main.cpp \
    mainwindow.cpp \
    C_Wdg_Box.cpp \
    dlgbox.cpp \
    dlgetat.cpp \
    dlgresp.cpp \
    dlgtache.cpp \
    dlgparam.cpp \
    dlgdesti.cpp \
    CApp.cpp \
    C_BaseSynoptux.cpp \
    C_DBVar.cpp \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.cpp \
    ../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.cpp \
    ../../MedinTuxTools-QT4/qtsingleapplication/src/qtlocalpeer.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/C_AppCore.cpp \
    ../../MedinTuxTools-QT4/C_BaseCommon.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Log.cpp \
    C_Dlg_Changements.cpp \
    C_ClickableLed.cpp

HEADERS = mainwindow.h \
    C_Wdg_Box.h \
    dlgbox.h \
    dlgetat.h \
    dlgresp.h \
    dlgtache.h \
    dlgparam.h \
    dlgdesti.h \
    CApp.h \
    C_BaseSynoptux.h \
    C_DBVar.h \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.h \
    ../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.h \
    ../../MedinTuxTools-QT4/qtsingleapplication/src/qtlocalpeer.h \
    ../../MedinTuxTools-QT4/CGestIni.h \
    ../../MedinTuxTools-QT4/C_AppCore.h \
    ../../MedinTuxTools-QT4/C_BaseCommon.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h \
    ../../MedinTuxTools-QT4/C_Utils_Log.h \
    C_Dlg_Changements.h \
    C_ClickableLed.h

mac:        LIBS += -L/usr/X11/lib
unix:!macx: LIBS += -lX11

RESOURCES = synoptux.qrc

FORMS += box.ui \
    dlgbox.ui \
    dlgetat.ui \
    dlgresp.ui \
    dlgtache.ui \
    dlgparam.ui \
    dlgdesti.ui \
    C_Dlg_Changements.ui

OTHER_FILES += Analyse.txt \
    ../bin/synoptuxBases.cfg \
    ../bin/synoptux.ini

TRANSLATIONS = synoptux_fr.ts \
               synoptux_it.ts

# ......... define building path ..................
unix:OBJECTS_DIR  = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR = $${SRC_DIR}/obj/win32
mac:OBJECTS_DIR   = $${SRC_DIR}/obj/mac

UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc


