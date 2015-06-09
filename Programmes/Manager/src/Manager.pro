# -------------------------------------------------
# Project created by QtCreator 2009-05-17T08:20:13
# -------------------------------------------------
QT += network \
    sql \
    webkit \
    xml \
    qt3support

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets printsupport
message( " ")
message(Qt version: $$[QT_VERSION])

TARGET = Manager
NUM_VERS            = 2.16.014
NAME_DOM            = MedinTux
NAME_ORG            = Data Medical Design
IS_MY_PIXMAP_CACHE  = 1

DEFINES     += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES     += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""
DEFINES     += "NAME_ORGANISATION=\"\\\"$${NAME_ORG}\\\"\""
DEFINES     += "NAME_DOMAINE=\"\\\"$${NAME_DOM}\\\"\""
DEFINES     += "SQLSYNTAX_MYSQL"
DEFINES	    += "USE_MY_PIXMAP_CACHE=\"\\\"$${IS_MY_PIXMAP_CACHE}\\\"\""

TEMPLATE     = app

OBJECTS_DIR = ../src/obj
MOC_DIR = ../src/moc
UI_DIR = ../src/ui
DESTDIR = ../bin
DEFINES += QT3_SUPPORT
exists( ../../SigEntrees/bin/sigentrees.ini ) {
    message( "================== Support des Entrees SIGEMS ==============" )
    DEFINES += ENTREES_SIGEMS
    HEADERS += ../../SigEntrees/src/C_DSigemsVar.h
    SOURCES += ../../SigEntrees/src/C_DSigemsVar.cpp
    }
exists( ../../SesamVitale-QT4/CSesam_API.h ) {
    DEFINES += SESAM_VERSION
    DEFINES += AGENDA_IN_GUI
    message( "================== Support de Sesam Vitale ==============" )
    HEADERS += ../../SesamVitale-QT4/C_Cps.h \
        ../../SesamVitale-QT4/C_Vitale.h \
        ../../SesamVitale-QT4/CSesam_API_InfoGroupe.h
    SOURCES += ../../SesamVitale-QT4/C_Cps.cpp \
        ../../SesamVitale-QT4/C_Vitale.cpp \
        ../../SesamVitale-QT4/CSesam_API_InfoGroupe.cpp
}
else:message( "============= pas de Support de Sesam Vitale ============" )
message( "| Manager is a MedinTux software by Data Medical Design |" )
message( "=============================================================" )
include(../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.pri)

include(../../MedinTuxTools-QT4/C_GoogleOAuth2/src/C_GoogleOAuth2.pri)

mac:LIBS +=  -L/usr/X11/lib
unix:LIBS += -lX11

SOURCES += main.cpp \
    CApp.cpp \
    C_Manager.cpp \
    C_Dlg_Assure.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Log.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Html.cpp \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.cpp \
    ../../MedinTuxTools-QT4/C_AppCore.cpp \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.cpp \
    ../../MedinTuxTools-QT4/C_QLineEdit/C_QLineEdit.cpp \
    ../../MedinTuxTools-QT4/CHtmlTools.cpp \
    ../../MedinTuxTools-QT4/Agenda/CMoteurAgenda.cpp \
    ../../MedinTuxTools-QT4/CMoteur_Base.cpp \
    ../../MedinTuxTools-QT4/C_DBVarDrTux.cpp \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_Agenda.cpp \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_ImprimerRDV.cpp \
#    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp \
    ../../MedinTuxTools-QT4/CCoolPopup.cpp \
    ../../MedinTuxTools-QT4/UserList/C_Frm_UserList.cpp \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_RdvTypeConfig.cpp \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_ChercheRDV.cpp \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_KeyPressEater.cpp \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_Dlg_DoList.cpp \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.cpp \
    ../../MedinTuxTools-QT4/Agenda/C_RendezVous.cpp \
    ../../MedinTuxTools-QT4/C_DragQTreeWidget/C_DragQTreeWidget.cpp \
    ../../MedinTuxTools-QT4/C_GoogleAPI/C_GoogleAPI.cpp \
    ../../MedinTuxTools-QT4/C_GoogleAPI/C_GoogleEvent.cpp \
    ../../MedinTuxTools-QT4/C_GoogleAPI/C_CalendarManager.cpp \
    ../../MedinTuxTools-QT4/Agenda/c_dlg_plagesdispo.cpp
HEADERS += CApp.h \
    C_Manager.h \
    C_Dlg_Assure.h \
    ../../MedinTuxTools-QT4/C_Utils_Log.h \
    ../../MedinTuxTools-QT4/C_Utils_Html.h \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.h \
    ../../MedinTuxTools-QT4/C_AppCore.h \
    ../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h \
    ../../MedinTuxTools-QT4/C_QLineEdit/C_QLineEdit.h \
    ../../MedinTuxTools-QT4/CHtmlTools.h \
    ../../MedinTuxTools-QT4/Agenda/CMoteurAgenda.h \
    ../../MedinTuxTools-QT4/CMoteur_Base.h \
    ../../MedinTuxTools-QT4/C_DBVarDrTux.h \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_Agenda.h \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_ImprimerRDV.h \
    ../../MedinTuxTools-QT4/Agenda/C_RendezVous.h \
#    ../../MedinTuxTools-QT4/CGestIni.h \
    ../../MedinTuxTools-QT4/Theme/ThemePopup.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h \
    ../../MedinTuxTools-QT4/CCoolPopup.h \
    ../../MedinTuxTools-QT4/UserList/C_Frm_UserList.h \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_RdvTypeConfig.h \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_ChercheRDV.h \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_KeyPressEater.h \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_Dlg_DoList.h \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.h \
    ../../MedinTuxTools-QT4/C_DragQTreeWidget/C_DragQTreeWidget.h \
    ../../MedinTuxTools-QT4/C_GoogleAPI/C_GoogleAPI.h \
    ../../MedinTuxTools-QT4/C_GoogleAPI/C_GoogleEvent.h \
    ../../MedinTuxTools-QT4/C_GoogleAPI/C_CalendarManager.h \
    ../../MedinTuxTools-QT4/Agenda/c_dlg_plagesdispo.h
FORMS += C_Manager.ui \
         C_Dlg_Assure.ui \
    ../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.ui \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_RdvTypeConfig.ui \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_ChercheRDV.ui \
    ../../MedinTuxTools-QT4/Agenda/C_Dlg_ImprimerRDV.ui \
    ../../MedinTuxTools-QT4/UserList/C_Frm_UserList.ui \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_Dlg_DoList.ui \
    ../../MedinTuxTools-QT4/Agenda/c_dlg_plagesdispo.ui
TRANSLATIONS = Manager_fr.ts \
Manager_es.ts
CODECFORTR = UTF8
OTHER_FILES += ../bin/Ressources/Changements.html \
    Manager_fr.ts \
    ../bin/lang/Manager_fr.qm \
    ../bin/Manager.ini \
    ../bin/DataBase.cfg \
    ../bin/DataBaseAgenda.cfg


