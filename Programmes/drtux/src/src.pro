# Fichier g���par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??:  ../bin/drtux

LANGUAGE = C++
INCLUDEPATH += ../src
MOC_DIR = moc
UI_DIR = ui
TRANSLATIONS += drtux_fr.ts \
    drtux_fi.ts \
    drtux_us.ts \
    drtux_no.ts \
    drtux_se.ts
CODECFORTR = UTF8
OBJECTS_DIR = obj
DEFINES = _DR_TUX
CONFIG += x11 \
 exceptions \
 thread \
 debug
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
message(             "|      DrTux is a MedinTux software by Data Medical Design    |" )
message(             "=============================================================" )
TARGET = ../bin/drtux
TEMPLATE = app
FORMS += RubTerrain.ui \
         RubIdentite.ui \
         RubChoixPatient.ui \
         RubPrescription.ui \
         Dock_TabRubrique.ui \
         FormGlossaire.ui \
         DlgListFieldMngr.ui \
         ../../gest_user/src/Dlg_PermsUserChoice.ui \
         ../../MedinTuxTools/DLG_InsertList.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/Calendar/Dlg_Calendar.ui \
         ../../MedinTuxTools/DlgListOff_Listes.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \
         ../../MedinTuxTools/Form_Organiseur.ui \
         Dlg_GestionATCD.ui \
         DlgPrint.ui \
         DlgGetText.ui \
         DlgChoixUser.ui \
         DlgDateNom.ui \
         Dlg_PosoManager.ui \
         Dlg_MedicaTux.ui \
         Dlg_MedicaAdjustPoso.ui \
         ../../MedinTuxTools/Dlg_PasswordGet.ui \
         Dlg_HtmlView.ui \
         Dlg_AdvancedTerrain.ui \
         FormConfig.ui \
         Dlg_ImageProperty.ui \
         DlgAtcd_txt.ui \
         Dock_Menu.ui \
         Dlg_VigieFiltreConf.ui \
         Dlg_Pendule.ui \
         Dlg_FamilleGenre.ui

HEADERS += drtux.h \
           CApp.h \
           ../../drtux/src/C_DlgListFieldMngr.h \
           ../../MedinTuxTools/C_ImageList.h \
           C_FormGlossaire.h \
           C_TokenInterpret.h \
           CDevilCrucible.h \
           MyEditText.h \
           CMoteur_Base.h \
           FormOngletRubrique.h \
           Global.h \
           CPrinter.h \
           Rubriques.h \
           structureparser.h \
           MyButton.h \
           CBigShrinker.h \
           CMyQtable.h \
           CRubList.h \
           CPreview.h \
           C_KeyPressEater.h \
           ../../MedinTuxTools/C_Organiseur.h \
           ../../MedinTuxTools/Codecs/qmdcodec.h \
           ../../MedinTuxTools/CW_EditText.h \
           ../../MedinTuxTools/Calendar/MyDateTable.h \
           ../../MedinTuxTools/Calendar/C_Horloge.h \
           ../../MedinTuxTools/CGenTools.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
           ../../MedinTuxTools/CCoolPopup.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CMedicaBase.h \
           ../../MedinTuxTools/cdlg_passwordget.h \
           ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           CMDI_Terrain.h \
           CMDI_Generic.h \
           CMDI_Ident.h \
           CMDI_ChoixPatient.h \
           CMDI_Observation.h \
           CMDI_Prescription.h \
           CDevilCalc.h \
           CDevilGraph.h \
           FormConfig.ui.h \
           C_RubIdentite.h \
           C_Dlg_ImageProperty.h \
           Dock_Menu.ui.h \
           C_Atcd.h \
           ../../MedinTuxTools/CPrtQListViewItem.h \
           C_Var.h \
           RubTerrain.ui.h \
           RubPrescription.ui.h \
           RubChoixPatient.ui.h \
           DlgPrint.ui.h \
           RubIdentite.ui.h \
           DlgListFieldMngr.ui.h \
           C_Dlg_GestionATCD.h \
           C_Dlg_VigieFiltreConf.h \
           C_Dlg_Pendule.h \
           C_Dlg_ChoixUser.h \
           C_DrTuxDb_CnxManager.h \
           C_ListViewATCDManager.h \
           C_Dlg_FamilleGenre.h \
           C_RubObservation.h

SOURCES += drtux.cpp \
           CApp.cpp \
           ../../drtux/src/C_DlgListFieldMngr.cpp \
           ../../MedinTuxTools/C_ImageList.cpp \
           C_FormGlossaire.cpp \
           C_TokenInterpret.cpp \
           CDevilCrucible.cpp \
           main.cpp \
           MyEditText.cpp \
           CMoteur_Base.cpp \
           FormOngletRubrique.cpp \
           CPrinter.cpp \
           structureparser.cpp \
           MyButton.cpp \
           CMyQtable.cpp \
           C_KeyPressEater.cpp \
           ../../MedinTuxTools/C_Organiseur.cpp \
           ../../MedinTuxTools/Codecs/qmdcodec.cpp \
           ../../MedinTuxTools/CW_EditText.cpp \
           ../../MedinTuxTools/Calendar/MyDateTable.cpp \
           ../../MedinTuxTools/Calendar/C_Horloge.cpp \
           ../../MedinTuxTools/CGenTools.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
           ../../MedinTuxTools/CCoolPopup.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CMedicaBase.cpp \
           ../../MedinTuxTools/cdlg_passwordget.cpp \
           ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           ../../MedinTuxTools/CPrtQListViewItem.cpp \
           CMDI_Ident.cpp \
           CMDI_ChoixPatient.cpp \
           CMDI_Observation.cpp \
           CMDI_Prescription.cpp \
           CMDI_Terrain.cpp \
           CMDI_Generic.cpp \
           CDevilCalc.cpp \
           CDevilGraph.cpp \
           C_RubIdentite.cpp \
           C_Dlg_ImageProperty.cpp \
           C_Atcd.cpp \
           C_Var.cpp \
           C_Dlg_GestionATCD.cpp \
           C_Dlg_VigieFiltreConf.cpp \
           C_Dlg_Pendule.cpp \
           C_Dlg_ChoixUser.cpp \
           C_DrTuxDb_CnxManager.cpp \
           C_ListViewATCDManager.cpp \
           C_Dlg_FamilleGenre.cpp \
           C_RubObservation.cpp






CONFIG -= release

