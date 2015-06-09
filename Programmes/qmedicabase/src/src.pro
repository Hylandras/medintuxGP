# Fichier g���par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??:  ../bin/qmedicabase

LANGUAGE = C++
MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj
TARGET = ../bin/qmedicabase
CONFIG += warn_on \
qt \
thread \
exceptions
TEMPLATE = app

DEFINES += SIMPLE_VERSION

exists( ../../SesamVitale-QT3/qtsingleapplication.pri ){
    DEFINES += SESAMVITALE_VERSION
    message(         "================== SesamVitale version ======================" )
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
message(         "| medicabase is a MedinTux software by Data Medical Design  |" )
message(         "=============================================================" )

FORMS += ../../MedinTuxTools/Dlg_PasswordGet.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \
         ../../MedinTuxTools/medicabase/Dlg_PosoManager.ui \
         ../../MedinTuxTools/medicabase/Dlg_MedicaTux.ui \
         ../../MedinTuxTools/medicabase/Dlg_HtmlView.ui \
         ../../MedinTuxTools/medicabase/Dlg_AdvancedTerrain.ui \
         ../../MedinTuxTools/medicabase/Dlg_MedicaAdjustPoso.ui

HEADERS +=  CApp.h \
           ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/CPrtQListViewItem.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CGenTools.h \
           ../../MedinTuxTools/CMedicaBase.h \
           ../../drtux/src/CMoteur_Base.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/cdlg_passwordget.h

SOURCES += main.cpp \
           CApp.cpp \
           ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/CPrtQListViewItem.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CGenTools.cpp \
           ../../MedinTuxTools/CMedicaBase.cpp \
           ../../drtux/src/CMoteur_Base.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/cdlg_passwordget.cpp



