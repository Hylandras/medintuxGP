# Fichier g�n�r� par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet�: ./src
# Cible�: une application??:  ../bin/QFseVitale

MOC_DIR = moc 
OBJECTS_DIR = obj
UI_DIR = ui
TARGET = ../bin/QFseVitale 
CONFIG += warn_on \
          qt \
          thread
TEMPLATE = app 
FORMS += Dlg_FseDefine.ui \
         Dlg_GetCodePorteur.ui \
         ../../MedinTuxTools/Calendar/Dlg_Calendar.ui 
HEADERS += ../../SesamVitale-QT3/CSesam_API.h \
           ../../SesamVitale-QT3/C_Vitale.h \
           ../../SesamVitale-QT3/CSesam_API_InfoGroupe.h \
           ../../SesamVitale-QT3/C_Cps.h \
           ../../SesamVitale-QT3/C_CpsExt.h \
           ../../SesamVitale-QT3/C_VitaleExt.h \
           ../../MedinTuxTools/CCoolPopup.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CMaskedLineEdit.h \
           ../../MedinTuxTools/Calendar/MyDateTable.h \
           ../../MedinTuxTools/Calendar/C_Horloge.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           CFseVitaleAppCtx.h \
           CFseVitaleApp.h \
           CVitaleReadThread.h \
           C_Dlg_FseDefine.h \
           C_VitaleListBoxItem.h \
           C_Fse.h  \
 C_Dlg_GetCodePorteur.h
SOURCES += ../../SesamVitale-QT3/C_Cps.cpp \
           ../../SesamVitale-QT3/C_Vitale.cpp \
           ../../SesamVitale-QT3/CSesam_API_InfoGroupe.cpp \
           ../../SesamVitale-QT3/C_CpsExt.cpp \
           ../../SesamVitale-QT3/C_VitaleExt.cpp \
           ../../MedinTuxTools/CCoolPopup.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CMaskedLineEdit.cpp \
           ../../MedinTuxTools/Calendar/MyDateTable.cpp \
           ../../MedinTuxTools/Calendar/C_Horloge.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           main.cpp \
           CFseVitaleAppCtx.cpp \
           CFseVitaleApp.cpp \
           CVitaleReadThread.cpp \
           C_Dlg_FseDefine.cpp \
           C_VitaleListBoxItem.cpp \
           C_Fse.cpp  \
 C_Dlg_GetCodePorteur.cpp
win32 {
    LIBS += ../../qfselib/bin/qfselib 
}
unix {
    LIBS += ../../qfselib/bin/libqfselib.so.1 
}
