# -------------------------------------------------
# Project created by QtCreator 2009-11-24T01:27:23
# -------------------------------------------------
QT += sql \
    webkit
TARGET = ../bin/personnes
TEMPLATE = app
MOC_DIR = ../src/moc
UI_DIR = ../src/ui
DESTDIR = ../bin
OBJECTS_DIR = ../src/obj
SOURCES += main.cpp \
    C_Dlg_personnes.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp \
    ../../MedinTuxTools-QT4/C_QLineEdit/C_QLineEdit.cpp \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_KeyPressEater.cpp \
    C_App.cpp \
    ../../MedinTuxTools-QT4/Theme/Theme.cpp
HEADERS += C_Dlg_personnes.h \
    ../../MedinTuxTools-QT4/CGestIni.h \
    ../../MedinTuxTools-QT4/C_QLineEdit/C_QLineEdit.h \
    ../../MedinTuxTools-QT4/C_Dlg_DoList/C_KeyPressEater.h \
    C_App.h \
    ../../MedinTuxTools-QT4/Theme/Theme.h
FORMS += Dlg_personnes.ui
OTHER_FILES += ../bin/Ressources/Changements.html \
    ../bin/DataBase.cfg
TRANSLATIONS = personnes_fr.ts\
personnes_es.ts
