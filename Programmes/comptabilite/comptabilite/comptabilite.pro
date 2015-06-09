# Fichier g���par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet: ./comptabilite
# Cible: une application??:  ../bin/comptabilite

LANGAGE = C++
MOC_DIR = moc
UI_DIR = ui
OBJECTS_DIR = obj 
TARGET = ../bin/comptabilite 
CONFIG += qt \
          thread \
 warn_off
TEMPLATE = app 
FORMS += g_ui/FormListMenu.ui \
         g_ui/filtre_hono.ui \
         g_ui/banque_view.ui \
         g_ui/FormSelectionUser.ui \
         g_ui/FormFirstUse.ui \
         g_ui/acte_modif.ui \
         g_ui/FormPlugins.ui \
         g_ui/acte_selection.ui \
         g_ui/mvts_add.ui \
         g_ui/cpts_add.ui \
         g_ui/immobilisations_add.ui \
         g_ui/livres.ui \
         g_ui/mvts_ui.ui \
         g_ui/param_view.ui \
         g_ui/a_propos.ui \
         g_ui/daf_view.ui \
         g_ui/cmu_view.ui \
         g_ui/hono_view.ui \
         g_ui/hono_visualisation.ui \
         g_ui/mvts_visualisation.ui \
         g_ui/depots_validation.ui \
         g_ui/Dlg_LoginPass_Medintux.ui \
         g_ui/FormLinkTux.ui 
TRANSLATIONS += fr.ts 
DISTFILES += ../bin/Changelog.txt \
             Version.ini \
             ../bin/todos.txt \
             ../bin/Fonctionnalites.html 
HEADERS += CApp.h \
           compta.h \
           ComptaMoteurBase.h \
           Listes.h \
           CMDI_Generic.h \
           g_ui/FormListMenu.ui.h \
           CMDI_Honoraires.h \
           CMDI_Actes.h \
           g_ui/filtre_hono.ui.h \
           CMDI_Param.h \
           Defines.h \
           EspecesCalculator.h \
           Depots.h \
           g_ui/FormSelectionUser.ui.h \
           g_ui/FormFirstUse.ui.h \
           FirstUse.h \
           CMDI_Depots.h \
           Actes_ui.h \
           CMDI_Daf.h \
           CMDI_Cmu.h \
           g_ui/FormPlugins.ui.h \
           g_ui/acte_selection.ui.h \
           g_ui/FormLinkTux.ui.h \
           CMDI_Mouvements.h \
           g_ui/mvts_add.ui.h \
           g_ui/cpts_add.ui.h \
           CMDI_Releve.h \
           g_ui/immobilisations_add.ui.h \
           Tools.h \
           CMDI_Livres.h \
           g_ui/livres.ui.h \
           icones.h \
           g_ui/mvts_ui.ui.h \
           g_ui/daf_view.ui.h \
           g_ui/cmu_view.ui.h \
           g_ui/hono_view.ui.h \
           g_ui/hono_visualisation.ui.h \
           g_ui/mvts_visualisation.ui.h \
           g_ui/depots_validation.ui.h \
           ../../MedinTuxTools/CGestIni.h 
SOURCES += CApp.cpp \
           compta.cpp \
           ComptaMoteurBase.cpp \
           main.cpp \
           CMDI_Honoraires.cpp \
           CMDI_Actes.cpp \
           CMDI_Param.cpp \
           Listes.cpp \
           EspecesCalculator.cpp \
           Depots.cpp \
           FirstUse.cpp \
           CMDI_Depots.cpp \
           Actes_ui.cpp \
           CMDI_Daf.cpp \
           CMDI_Cmu.cpp \
           CMDI_Mouvements.cpp \
           CMDI_Releve.cpp \
           Tools.cpp \
           CMDI_Livres.cpp \
           ../../MedinTuxTools/CGestIni.cpp 
CONFIG -= warn_on

