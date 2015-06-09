# Fichier généré par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet : ./src
# Cible : une application??:  ../bin/QLightPad

FORMS += DLG_InsertLink.ui \
         DLG_InsertAncre.ui \
         DLG_InsertEmail.ui \
         DLG_Browser.ui \
         DLG_BodyAttributs.ui \
         DLG_SaveRequest.ui \
         ../../MedinTuxTools/DLG_InsertList.ui \
         ../../MedinTuxTools/DlgListOff_Listes.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \
         ../../drtux/src/DlgListFieldMngr.ui \
         DLG_A_propos.ui 
TRANSLATIONS += QLightPad_fr.ts 
HEADERS += MyEditText.h \
           QLightPad.h \
           ../../drtux/src/C_DlgListFieldMngr.h \           
           ../../MedinTuxTools/CGenTools.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/CCoolPopup.h \
           ../../MedinTuxTools/CW_EditText.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CPreview.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           ../../drtux/src/C_KeyPressEater.h \
           CMyTabWidget.h \
           SaveRequest.h 
SOURCES += QLightPad.cpp \
           ../../drtux/src/C_DlgListFieldMngr.cpp \           
           main.cpp \
           MyEditText.cpp \
           ../../MedinTuxTools/CGenTools.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/CCoolPopup.cpp \
           ../../MedinTuxTools/CW_EditText.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           ../../drtux/src/C_KeyPressEater.cpp \
           CMyTabWidget.cpp 
DEFINES += FOR_MEDINTUX
DEFINES += QLIGHTPAD
TEMPLATE = app
LANGUAGE = C++
CONFIG += debug warn_on qt thread
INCLUDEPATH += ../src
TARGET = ../bin/QLightPad
OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
