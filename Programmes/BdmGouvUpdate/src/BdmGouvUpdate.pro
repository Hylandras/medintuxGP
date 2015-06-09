
#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T15:31:22
#
#-------------------------------------------------
QT += core gui \
    network \
    svg \
    webkit \
    sql \
    xml \
    multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

message(Qt version: $$[QT_VERSION])

ROOT_PROJECT        = $$PWD/../
SRC_DIR             = $${ROOT_PROJECT}/src/
DESTDIR             = $${ROOT_PROJECT}/bin/

#........... make the binary name and version number available in c++ code using macro definition ................
TARGET       = BdmGouvUpdate
NUM_VERS     = 2.16.000
NAME_DOM     = MedinTux
NAME_ORG     = Data Medical Design
DEFINES     += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES     += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""
DEFINES     += "NAME_ORGANISATION=\"\\\"$${NAME_ORG}\\\"\""
DEFINES     += "NAME_DOMAINE=\"\\\"$${NAME_DOM}\\\"\""
DEFINES     += "SQLSYNTAX_MYSQL"
TEMPLATE     = app

include(../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.pri)
unix:!macx: LIBS += -lX11
SOURCES += main.cpp \
           C_MainWindow.cpp \
           CApp.cpp \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.cpp \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/C_BaseCommon.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Log.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Html.cpp \
    ../../MedinTuxTools-QT4/C_AppCore.cpp \
    C_BDM_Manager.cpp

HEADERS += C_MainWindow.h \
           CApp.h \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.h \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h \
    ../../MedinTuxTools-QT4/CGestIni.h \
    ../../MedinTuxTools-QT4/C_BaseCommon.h \
    ../../MedinTuxTools-QT4/C_Utils_Log.h \
    ../../MedinTuxTools-QT4/C_Utils_Html.h \
    ../../MedinTuxTools-QT4/C_AppCore.h \
    C_BDM_Manager.h

FORMS       += C_MW_MainWindow.ui
OTHER_FILES += ../bin/Ressources/Changements.html

# ......... define building path ..................
unix:OBJECTS_DIR  = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR = $${SRC_DIR}/obj/win32
mac:OBJECTS_DIR   = $${SRC_DIR}/obj/mac

UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc
