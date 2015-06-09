# Fichier g���par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??:  ../bin/MrPropre

FORMS += Dlg_MainDialog.ui 
HEADERS += C_Dlg_MainDialog.h \            
           ../../MedinTuxTools/CGestIni.h 

SOURCES += main.cpp \
           C_Dlg_MainDialog.cpp \            
           ../../MedinTuxTools/CGestIni.cpp 
TEMPLATE = app
UI_DIR = ui
CONFIG += warn_on \
thread \
qt \
 debug
TARGET = ../bin/MrPropre
CONFIG -= release

