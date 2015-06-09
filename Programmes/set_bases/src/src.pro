# Fichier g�n�r� par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet�: ./src
# Cible�: une application??:  ../bin/set_bases

LANGUAGE = C++
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = ui
TARGET = ../bin/set_bases
CONFIG += qt \
          thread \
          x11 \
 debug \
 exceptions
TEMPLATE = app
FORMS += Dlg_MainDialog.ui \
         ../../MedinTuxTools/DlgGetString.ui \
         ../../MedinTuxTools/DlgMyMessageBox.ui \

TRANSLATIONS += set_bases_fr.ts
HEADERS += ../../MedinTuxTools/C_Zlib/C_Zlib.h \
           ../../MedinTuxTools/C_Zlib/deflate.h \
           ../../MedinTuxTools/C_Zlib/inftrees.h \
           ../../MedinTuxTools/C_Zlib/zconf.h \
           ../../MedinTuxTools/C_Zlib/zconf.in.h \
           ../../MedinTuxTools/C_Zlib/zlib.h \
           ../../MedinTuxTools/C_Zlib/zutil.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CMedicaBase.h \
           ../../MedinTuxTools/CHtmlTools.h \
          ../../MedinTuxTools/CGenTools.h \
           ../../drtux/src/CMoteur_Base.h \
           ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/ThemePopup.h \
           ../../MedinTuxTools/Theme.h \
           ../../MedinTuxTools/CPrtQListViewItem.h \
           CSetupBase.h \
           CApp.h \
           ../../MedinTuxTools/CW_EditText.h \
           CUpdateThread.h \
           CIndexMakeThread.h \
           C_DlgMainDialog.h
SOURCES += main.cpp \
           ../../MedinTuxTools/C_Zlib/C_Zlib.cpp \
           ../../MedinTuxTools/C_Zlib/adler32.c \
           ../../MedinTuxTools/C_Zlib/compress.c \
           ../../MedinTuxTools/C_Zlib/crc32.c \
           ../../MedinTuxTools/C_Zlib/deflate.c \
           ../../MedinTuxTools/C_Zlib/gzio.c \
           ../../MedinTuxTools/C_Zlib/infback.c \
           ../../MedinTuxTools/C_Zlib/inffast.c \
           ../../MedinTuxTools/C_Zlib/inflate.c \
           ../../MedinTuxTools/C_Zlib/inftrees.c \
           ../../MedinTuxTools/C_Zlib/trees.c \
           ../../MedinTuxTools/C_Zlib/uncompr.c \
           ../../MedinTuxTools/C_Zlib/zutil.c \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CMedicaBase.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
          ../../MedinTuxTools/CGenTools.cpp \
           ../../drtux/src/CMoteur_Base.cpp \
           ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/CPrtQListViewItem.cpp \
           ../../MedinTuxTools/ThemePopup.cpp \
           ../../MedinTuxTools/Theme.cpp \
           CSetupBase.cpp \
           CApp.cpp \
           ../../MedinTuxTools/CW_EditText.cpp \
           CUpdateThread.cpp \
           CIndexMakeThread.cpp \
           C_DlgMainDialog.cpp

CONFIG -= release

