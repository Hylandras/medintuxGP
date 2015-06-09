# Fichier g���par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??:  ../bin/med_stat

LANGUAGE = C++
MOC_DIR = moc
OBJECTS_DIR = obj
TARGET = ../bin/med_stat
CONFIG += qt \
thread \
 warn_off \
 debug
TEMPLATE = app
UI_DIR = ui

exists( ../../SesamVitale-QT3/qtsingleapplication.pri ){
    DEFINES += SESAMVITALE_VERSION
    message( "================== SesamVitale version ===============" )
    include(../../SesamVitale-QT3/qtsingleapplication.pri)
    HEADERS += ../../SesamVitale-QT3/C_Cps.h \
                   ../../SesamVitale-QT3/C_Vitale.h \
                   ../../SesamVitale-QT3/CSesam_API_InfoGroupe.h \
                   ../../SesamVitale-QT3/C_MUI_App.h
    SOURCES += ../../SesamVitale-QT3/C_Cps.cpp \
                   ../../SesamVitale-QT3/C_Vitale.cpp \
                   ../../SesamVitale-QT3/CSesam_API_InfoGroupe.cpp \
                   ../../SesamVitale-QT3/C_MUI_App.cpp
}
else {    message(   "================== CeCILL Licence version ===================" )
    HEADERS += ../../MedinTuxTools/C_MUI_App.h
    SOURCES += ../../MedinTuxTools/C_MUI_App.cpp
}
message(         "|  med_stat is a MedinTux software by Data Medical Design   |" )
message(         "=============================================================" )

FORMS += Wdg_User.ui \
         Wdg_Central.ui \
         DlgPassChange.ui \
         ../../MedinTuxTools/Calendar/Dlg_Calendar.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \
         Dlg_GetCodePorteur.ui \
         Dlg_NewUser.ui \
         ../../MedinTuxTools/Dlg_PasswordGet.ui \
         Dlg_NewClassDenombre.ui \
         Dlg_MetaFieldDefine.ui \
         Dlg_ExportConfig.ui
TRANSLATIONS += med_stat_fr.ts
HEADERS += med_stat.h \
           ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/CPrtQListViewItem.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CSetupBase.h \
           ../../MedinTuxTools/CGenTools.h \
           ../../MedinTuxTools/CMedicaBase.h \
           ../../drtux/src/CMoteur_Base.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
           ../../MedinTuxTools/Calendar/MyDateTable.h \
           ../../MedinTuxTools/Calendar/C_Horloge.h \
           CBigShrinker.h \
           CApp.h \
           ../../MedinTuxTools/cdlg_passwordget.h \
           CMetaField.h \
           CDenombre.h \
           CMetaFiltre.h \
           CDlg_newclassdenombre.h \
           CDlg_MetaFieldDefine.h \
           CDlg_ExportConfig.h

SOURCES += med_stat.cpp \
           ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/CPrtQListViewItem.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CSetupBase.cpp \
           ../../MedinTuxTools/CGenTools.cpp \
           ../../MedinTuxTools/CMedicaBase.cpp \
           ../../drtux/src/CMoteur_Base.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
           ../../MedinTuxTools/Calendar/MyDateTable.cpp \
           ../../MedinTuxTools/Calendar/C_Horloge.cpp \
           main.cpp \
           CApp.cpp \
           ../../MedinTuxTools/cdlg_passwordget.cpp \
           CMetaField.cpp \
           CDenombre.cpp \
           CMetaFiltre.cpp \
           CDlg_newclassdenombre.cpp \
           CDlg_MetaFieldDefine.cpp \
           CDlg_ExportConfig.cpp
CONFIG -= warn_on \
 release

