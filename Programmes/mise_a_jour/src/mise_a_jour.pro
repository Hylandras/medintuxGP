# Fichier g�n�r� par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet�: ./src
# Cible�: une application??:  ../bin/mise_a_jour

FORMS += Dlg_Patch.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \

HEADERS += ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/CPrtQListViewItem.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
          ../../MedinTuxTools/CGenTools.h \
           ../../MedinTuxTools/CMedicaBase.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../drtux/src/CMoteur_Base.h \
           CTheraBase.h 
SOURCES += ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/CPrtQListViewItem.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
          ../../MedinTuxTools/CGenTools.cpp \
           ../../MedinTuxTools/CMedicaBase.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../drtux/src/CMoteur_Base.cpp \
           CTheraBase.cpp \
           main.cpp
MOC_DIR = moc
OBJECTS_DIR = obj
TARGET = ../bin/mise_a_jour
CONFIG += qt \
thread \
 exceptions \
 warn_off \
 debug
TEMPLATE = app
UI_DIR = ui


CONFIG -= warn_on \
 release

