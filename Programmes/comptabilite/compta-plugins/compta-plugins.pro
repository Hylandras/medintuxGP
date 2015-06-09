TEMPLATE = app

SOURCES += main.cpp \
../../MedinTuxTools/CGestIni.cpp \
../comptabilite/Listes.cpp \
../comptabilite/Tools.cpp \
 CApp_Plug.cpp \
 ComptaMoteurBase.cpp
HEADERS += ../../MedinTuxTools/CGestIni.h \
../comptabilite/Listes.h \
../comptabilite/Tools.h \
../comptabilite/g_ui/acte_selection.ui.h \
../comptabilite/Defines.h \
 CApp_Plug.h \
 ../comptabilite/g_ui/FormPlugins.ui.h \
 ComptaMoteurBase.h
FORMS += ../comptabilite/g_ui/acte_selection.ui \
 ../comptabilite/g_ui/FormPlugins.ui
LANGAGE = C++
CONFIG += release \
qt \
thread \
 warn_off
TARGET = ../bin/compta-plugins

OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
CONFIG -= warn_on

