# Fichier g�n�r� par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet�: ./src
# Cible�: une application??:  ../bin/qgetdatasemp

MOC_DIR = moc 
OBJECTS_DIR = obj 
TARGET = ../bin/qgetdatasemp 
CONFIG += qt \
          debug
TEMPLATE = app
UI_DIR = ui
FORMS += Dlg_QGetDatasemp.ui 
HEADERS += CApp.h \
           ../../MedinTuxTools/C_Zlib/C_Zlib.h \
           ../../MedinTuxTools/C_Zlib/deflate.h \
           ../../MedinTuxTools/C_Zlib/inftrees.h \
           ../../MedinTuxTools/C_Zlib/zconf.h \
           ../../MedinTuxTools/C_Zlib/zconf.in.h \
           ../../MedinTuxTools/C_Zlib/zlib.h \
           ../../MedinTuxTools/C_Zlib/zutil.h \
           ../../MedinTuxTools/CPrtQListBoxItem.h \
           ../../MedinTuxTools/CHtmlTools.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../MedinTuxTools/CSetupBase.h \
           ../../MedinTuxTools/CW_EditText.h \
           ../../MedinTuxTools/CMedicaBase.h \
           CFtpTools.h \
           CFtp.h \
           CGetDatasempAppCtx.h \
           CFtpInfo.h  \
 C_Dlg_QGetDatasemp.h
SOURCES += main.cpp \
           CApp.cpp \
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
           ../../MedinTuxTools/CPrtQListBoxItem.cpp \
           ../../MedinTuxTools/CHtmlTools.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../MedinTuxTools/CSetupBase.cpp \
           ../../MedinTuxTools/CW_EditText.cpp \
           ../../MedinTuxTools/CMedicaBase.cpp \
           CFtpTools.cpp \
           CFtp.cpp \
           CGetDatasempAppCtx.cpp \
           CFtpInfo.cpp  \
 C_Dlg_QGetDatasemp.cpp


CONFIG -= release

