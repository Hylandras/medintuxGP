# Fichier g���par le module QMake de KDevelop.
# --------------------------------------------------
# Sous dossier relatif au dossier principal du projet: ./src
# Cible: une biblioth??que??:  ../bin/qfselib

TARGET = ../../qfselib/bin/qfselib
CONFIG += release \
          warn_on \
          qt \
          thread \
          exceptions \
          dll
TEMPLATE = lib
HEADERS += ../../SesamVitale-QT3/CSesam_API.h \
           ../../MedinTuxTools/CGestIni.h \
           ../../SesamVitale-QT3/C_Cps.h \
           ../../SesamVitale-QT3/C_Vitale.h \
           ../../SesamVitale-QT3/CSesam_API_InfoGroupe.h \
           sgd.h \
           ssv.h \
           srt.h \
           unixdef.h
SOURCES += ../../SesamVitale-QT3/CSesam_API.cpp \
           ../../MedinTuxTools/CGestIni.cpp \
           ../../SesamVitale-QT3/C_Cps.cpp \
           ../../SesamVitale-QT3/C_Vitale.cpp \
           ../../SesamVitale-QT3/CSesam_API_InfoGroupe.cpp
win32 {
    LIBS += ssvw32.lib \
  sgdw32.lib \
  srtw32.lib
}
unix {
    LIBS += libssvlux.so \
  libsgdlux.so \
  libsrtlux.so
}
INSTALLS += target

target.path = ../../Sesam-API-Local/ApiLinux/LibMedinTux

