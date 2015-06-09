#-------------------------------------------------
# Project created by QtCreator 2010-12-07T15:00:33
#-------------------------------------------------
QT += core gui \
    network \
    webkit \
    sql \
    xml

ROOT_PROJECT        = $$PWD/../
SRC_DIR             = $${ROOT_PROJECT}/src/
DESTDIR             = $${ROOT_PROJECT}/bin
message(Qt version: $$[QT_VERSION])
#........... make the binary name and version number available in c++ code using macro definition ................
TARGET       = SigEntrees
NUM_VERS     = 2.15.000
NAME_DOM     = MedinTux
NAME_ORG     = Data Medical Design
DEFINES     += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES     += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""
DEFINES     += "NAME_ORGANISATION=\"\\\"$${NAME_ORG}\\\"\""
DEFINES     += "NAME_DOMAINE=\"\\\"$${NAME_DOM}\\\"\""
TEMPLATE     = app

mac { LIBS +=  -L/usr/X11/lib
    }
unix:!macx: LIBS += -lX11

# include( $${SRC_DIR}/config.pri)
# include( $${SOURCES_LIBS_PATH}/aggregation.pri)
# include( $${SOURCES_LIBS_PATH}/extensionsystem.pri )
include(../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.pri)
SOURCES += main.cpp \
    C_Dlg_SigEntrees.cpp \
    C_DSigemsVar.cpp \
    C_BaseSigEntrees.cpp \
    CApp.cpp \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/C_BaseCommon.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Log.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Html.cpp \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.cpp \
    ../../MedinTuxTools-QT4/C_AppCore.cpp \
    C_ScruteFTP.cpp \
    C_ScruteDB.cpp \
    ../../MedinTuxTools-QT4/CCoolPopup.cpp \
    C_ClickableLed.cpp \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.cpp

HEADERS += C_Dlg_SigEntrees.h \
    C_DSigemsVar.h \
    C_BaseSigEntrees.h \
    CApp.h \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h \
    ../../MedinTuxTools-QT4/CGestIni.h \
    ../../MedinTuxTools-QT4/C_BaseCommon.h \
    ../../MedinTuxTools-QT4/C_Utils_Log.h \
    ../../MedinTuxTools-QT4/C_Utils_Html.h \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.h \
    ../../MedinTuxTools-QT4/C_AppCore.h \
    C_ScruteFTP.h \
    C_ScruteDB.h \
    ../../MedinTuxTools-QT4/CCoolPopup.h \
    C_ClickableLed.h \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.h

FORMS       += C_Dlg_SigEntrees.ui \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.ui

OTHER_FILES += ../bin/Ressources/Changements.html \
    ../bin/sigentrees.cfg

# ......... define building path ..................
unix:OBJECTS_DIR  = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR = $${SRC_DIR}/obj/win32
mac:OBJECTS_DIR   = $${SRC_DIR}/obj/mac

UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc
