# Fichier gnr par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une application??: ../bin/ccamview
QT += network \
    sql \
    webkit \
    qt3support
LANGUAGE = C++
Dlg_MyAlertBox.ui.target = Dlg_MyAlertBox.ui
Dlg_MyAlertBox.ui.commands = $$IDL_COMPILER
Dlg_ChargeMAJCCAM.ui.target = Dlg_ChargeMAJCCAM.ui
Dlg_ChargeMAJCCAM.ui.commands = $$IDL_COMPILER
DlgPrint.ui.target = DlgPrint.ui
TARGET = ../bin/ccamview
TEMPLATE = app
OBJECTS_DIR = ../src/obj
MOC_DIR = ../src/moc
UI_DIR = ../src/ui
DESTDIR = ../bin
CONFIG += release \
    warn_off \
    qt \
    thread \
    exceptions
TEMPLATE = app

# The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3 += FeuillePrincipale.ui \
    Dlg_SigemsLogin.ui \
    Dlg_login.ui \
    Dlg_Praticiens.ui \
    Dlg_PatientActeProperty.ui \
    Dlg_Categories.ui \
    DlgPrint.ui \
    Dlg_MyAlertBox.ui \
    Dlg_ChargeMAJCCAM.ui
TRANSLATIONS += ccamview_fr.ts
HEADERS += CMoteurCCAM_Base.h \
    CSoapInterface.h \
    CPrinter.h \
    ../../MedinTuxTools-QT4/CMaskedLineEdit.h \
    ../../MedinTuxTools-QT4/CGestIni.h \
    Global.h \
    C_Dlg_Categories.h \
    C_Dlg_ChangeMAJCCAM.h \
    C_Dlg_login.h \
    C_Dlg_MyAlertBox.h \
    C_Dlg_PatientActeProperty.h \
    C_Dlg_Praticiens.h \
    C_Dlg_Print.h \
    C_Dlg_SigemsLogin.h \
    C_Dlg_FeuillePrincipale.h
SOURCES += main.cpp \
    CMoteurCCAM_Base.cpp \
    CSaopInterface.cpp \
    CPrinter.cpp \
    C_Dlg_Categories.cpp \
    C_Dlg_ChangeMAJCCAM.cpp \
    C_Dlg_login.cpp \
    C_Dlg_MyAlertBox.cpp \
    C_Dlg_PatientActeProperty.cpp \
    C_Dlg_Praticiens.cpp \
    C_Dlg_Print.cpp \
    C_Dlg_SigemsLogin.cpp \
    C_Dlg_FeuillePrincipale.cpp \
    ../../MedinTuxTools-QT4/CMaskedLineEdit.cpp \
    ../../MedinTuxTools-QT4/CGestIni.cpp
FORMS += 

# The following line was inserted by qt3to4
QT += xml \
    sql \
    qt3support

# The following line was inserted by qt3to4
CONFIG += uic3
