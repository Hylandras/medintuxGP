# Fichier g���par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??:  ../bin/calendrier

HEADERS += CApp.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
           ../../MedinTuxTools/Calendar/MyDateTable.h \
           ../../MedinTuxTools/Calendar/C_Horloge.h \
          ../../MedinTuxTools/Theme.h 
 
SOURCES += main.cpp \
           CApp.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
           ../../MedinTuxTools/Calendar/MyDateTable.cpp \
           ../../MedinTuxTools/Calendar/C_Horloge.cpp \
          ../../MedinTuxTools/Theme.cpp
 
FORMS +=../../MedinTuxTools/Calendar/Dlg_Calendar.ui

TEMPLATE = app
CONFIG += debug \
          warn_on \
          thread \
          qt
TARGET = ../bin/calendrier
UI_DIR = ui

OTHER_FILES += \
    ../../set_bases/bin/MakeAllMac.sh \
    ../../set_bases/bin/MakeAllClean.sh
