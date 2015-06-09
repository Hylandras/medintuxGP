# Fichier g���par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??:  ../bin/gest_user
DEFINES = _GEST_USER
exists( ../../SesamVitale-QT3/qtsingleapplication.pri ){
    DEFINES += SESAMVITALE_VERSION
    message( "================== Sesam-Vitale version ===============" )
    include(../../SesamVitale-QT3/qtsingleapplication.pri)
    HEADERS += ../../SesamVitale-QT3/C_Cps.h \
                   ../../SesamVitale-QT3/CSesam_API_InfoGroupe.h \
                   ../../SesamVitale-QT3/C_Vitale.h
    SOURCES += ../../SesamVitale-QT3/C_Cps.cpp \
                   ../../SesamVitale-QT3/CSesam_API_InfoGroupe.cpp \
                   ../../SesamVitale-QT3/C_Vitale.cpp
}
else {    message(   "================== CeCILL Licence version ===================" )
}
message(         "|   gest_user is a MedinTux software by Data Medical Design   |" )
message(         "=============================================================" )
FORMS += DockFormUserList.ui \
         DockFormUserListDoc.ui \
         RubIdentite.ui \
         FormRubOrdo.ui \
         FormRubCertificat.ui \
         FormRubObservation.ui \
         FormRubParam.ui \
         DlgPassChange.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \
         ../../MedinTuxTools/DLG_InsertList.ui \
         Dlg_PermsUserChoice.ui \
         ../../MedinTuxTools/Dlg_PasswordGet.ui \
         Dlg_PermsDefine.ui \
         Dlg_Appartenance.ui \
         Dlg_IntervList.ui
TRANSLATIONS += gest_user_fr.ts
HEADERS += gest_user.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
           FormOngletRubrique.h \
           MyEditText.h \
           Rubriques.h \
           ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/CPrtQListViewItem.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           ../../drtux/src/CMoteur_Base.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/CGenTools.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CSetupBase.h \
           ../../MedinTuxTools/CMedinTuxApp.h \
           ../../MedinTuxTools/CPreview.h \
           CApp.h \
           DlgPassChangeDefine.h \
           ../../MedinTuxTools/cdlg_passwordget.h \
           CDlg_IntervList.h
SOURCES += gest_user.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
           main.cpp \
           FormOngletRubrique.cpp \
           MyEditText.cpp \
           ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/CPrtQListViewItem.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           ../../drtux/src/CMoteur_Base.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/CGenTools.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CSetupBase.cpp \
           ../../MedinTuxTools/CMedinTuxApp.cpp \
           CApp.cpp \
           ../../MedinTuxTools/cdlg_passwordget.cpp \
           CDlg_IntervList.cpp
LANGUAGE = C++
INCLUDEPATH += ../src
MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
TARGET = ../bin/gest_user
CONFIG += qt \
thread \
 debug
TEMPLATE = app
CONFIG -= release

