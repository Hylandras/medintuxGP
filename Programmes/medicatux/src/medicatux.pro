#-------------------------------------------------
#
# Project created by QtCreator 2010-12-07T15:00:33
#
#-------------------------------------------------
QT += core gui \
    network \
    svg \
    webkit \
    sql \
    xml

message(Qt version: $$[QT_VERSION])
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets webkitwidgets printsupport
greaterThan(QT_MAJOR_VERSION, 4):DEFINES += QT_5

ROOT_PROJECT        = $$PWD/../
SRC_DIR             = $${ROOT_PROJECT}/src/
DESTDIR             = $${ROOT_PROJECT}/bin/

#........... make the binary name and version number available in c++ code using macro definition ................
TARGET       = medicatux
NUM_VERS     = 2.16.000
NAME_DOM     = MedinTux
NAME_ORG     = Data Medical Design

#-------- sous ubuntu 64 ne pas definir MODE_GETGUID  ----------
exists( ../../../qt4_l64/installQt4.sh ){
    message( "================== On ubuntu 64 ==============" )
}
else{
DEFINES	    += "MODE_GETGUID=\"\\\"$${IS_GETGUID}\\\"\""
}

DEFINES     += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES     += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""
DEFINES     += "NAME_ORGANISATION=\"\\\"$${NAME_ORG}\\\"\""
DEFINES     += "NAME_DOMAINE=\"\\\"$${NAME_DOM}\\\"\""
DEFINES     += "SQLSYNTAX_MYSQL"
TEMPLATE     = app

include(../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.pri)

unix:!macx: LIBS += -lX11

SOURCES += main.cpp \
    C_MW_Prescription.cpp \
    CApp.cpp \
    ../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.cpp \
    ../../MedinTuxTools-QT4/C_QToolBox/C_QToolBox.cpp \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.cpp \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/C_BaseCommon.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Log.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Html.cpp \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.cpp \
    ../../MedinTuxTools-QT4/C_AppCore.cpp \
    ../../MedinTuxTools-QT4/C_Macro_Core.cpp \
    C_Macro.cpp \
    ../../MedinTuxTools-QT4/C_DragQTreeWidget/C_DragQTreeWidget.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_DBVar.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_Api.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_Theriaque_DBVar.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_TheriaquePlugin.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_DatasempPlugin.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_GenericPlugin.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_ApiVar.cpp \
    ../../MedinTuxTools-QT4/univers/C_Universal_Ident.cpp \
    ../../MedinTuxTools-QT4/univers/C_Human_Properties.cpp \
    ../../MedinTuxTools-QT4/univers/C_DateTools.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_PatientCtx.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_UserCtx.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.cpp \
    ../../MedinTuxTools-QT4/C_QLineEdit/C_QLineEdit.cpp \
    ../../MedinTuxTools-QT4/C_ComBus.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_PosologieGrammar.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDialCorpoFact.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_IndicationChoice.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_LevelItemDelegate.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_Dlg_NotesInfos.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_Control.cpp \
    ../../MedinTuxTools-QT4/C_QTabWidget/C_QTabWidget.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_TerrainChoice.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_GouvPlugin.cpp \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_Info.cpp
HEADERS += C_MW_Prescription.h \
    CApp.h \
    ../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h \
    ../../MedinTuxTools-QT4/C_QToolBox/C_QToolBox.h \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.h \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h \
    ../../MedinTuxTools-QT4/CGestIni.h \
    ../../MedinTuxTools-QT4/C_BaseCommon.h \
    ../../MedinTuxTools-QT4/C_Utils_Log.h \
    ../../MedinTuxTools-QT4/C_Utils_Html.h \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.h \
    ../../MedinTuxTools-QT4/C_AppCore.h \
    ../../MedinTuxTools-QT4/C_Macro_Core.h \
    C_Macro.h \
    ../../MedinTuxTools-QT4/C_DragQTreeWidget/C_DragQTreeWidget.h \
    ../../MedinTuxTools-QT4/medicabase/C_DBVar.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h \
    ../../MedinTuxTools-QT4/medicabase/C_Theriaque_DBVar.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_TheriaquePlugin.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_DatasempPlugin.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_GenericPlugin.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_ApiVar.h \
    ../../MedinTuxTools-QT4/univers/C_Universal_Ident.h \
    ../../MedinTuxTools-QT4/univers/C_Human_Properties.h \
    ../../MedinTuxTools-QT4/univers/C_DateTools.h \
    ../../MedinTuxTools-QT4/medicabase/C_PatientCtx.h \
    ../../MedinTuxTools-QT4/medicabase/C_UserCtx.h \
    ../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.h \
    ../../MedinTuxTools-QT4/C_QLineEdit/C_QLineEdit.h \
    ../../MedinTuxTools-QT4/C_ComBus.h \
    ../../MedinTuxTools-QT4/medicabase/C_PosologieGrammar.h \
    ../../MedinTuxTools-QT4/medicabase/C_InterResultPair.h \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDialCorpoFact.h \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_IndicationChoice.h \
    ../../MedinTuxTools-QT4/medicabase/C_LevelItemDelegate.h \
    ../../MedinTuxTools-QT4/medicabase/C_Dlg_NotesInfos.h \
    ../../MedinTuxTools-QT4/medicabase/C_WordGenrePair.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_Control.h \
    ../../MedinTuxTools-QT4/C_QTabWidget/C_QTabWidget.h \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_TerrainChoice.h \
    ../../MedinTuxTools-QT4/medicabase/C_BDM_GouvPlugin.h \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_Info.h
FORMS       += C_MW_Prescription.ui \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.ui \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDialCorpoFact.ui \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_IndicationChoice.ui \
    ../../MedinTuxTools-QT4/medicabase/C_Dlg_NotesInfos.ui \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_TerrainChoice.ui \
    ../../MedinTuxTools-QT4/medicabase/C_PopupDial_Info.ui
OTHER_FILES += ../bin/Ressources/Changements.html \
    ../bin/prescriptionBases.cfg \
    ../bin/C_BDM_Api.cfg \
    ../bin/medicatuxBases.cfg

# ......... define building path ..................
unix:OBJECTS_DIR  = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR = $${SRC_DIR}/obj/win32
mac:OBJECTS_DIR   = $${SRC_DIR}/obj/mac

UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc
