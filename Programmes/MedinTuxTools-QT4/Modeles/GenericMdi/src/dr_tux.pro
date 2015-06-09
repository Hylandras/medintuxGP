#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T11:33:06
#
#-------------------------------------------------

QT           += core gui \
                network \
                widgets \
                sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets
message(Qt version: $$[QT_VERSION])

TARGET       = dr_tux
NUM_VERS     = 2.16.000
NAME_DOM     = MedinTux
NAME_ORG     = Data Medical Design
IS_GETGUID   = QT5

DEFINES	    += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES	    += "NAME_APPLI=\"\\\"$${TARGET}\\\"\""
DEFINES     += "NAME_ORGANISATION=\"\\\"$${NAME_ORG}\\\"\""
DEFINES     += "NAME_DOMAINE=\"\\\"$${NAME_DOM}\\\"\""
DEFINES	    += "NUM_VERSION=\"\\\"$${NUM_VERS}\\\"\""
DEFINES	    += "MODE_GETGUID=\"\\\"$${IS_GETGUID}\\\"\""

ROOT_PROJECT = $$PWD/../
SRC_DIR      = $${ROOT_PROJECT}/src/
DESTDIR      = $${ROOT_PROJECT}/bin
TEMPLATE     = app


include(../../MedinTuxTools-QT4/qtsingleapplication/src/qtsingleapplication.pri)
unix:!macx: LIBS += -lX11

#........... make the binary name and version number available in c++ code using macro definition ................
SOURCES  += C_MainWindow.cpp \
            CApp.cpp \
            C_QMdiArea.cpp \
            C_QMdiSubWindow.cpp \
            ../../MedinTuxTools-QT4/C_AppCore.cpp \
            main.cpp \
            ../../MedinTuxTools-QT4/CGestIni.cpp \
            ../../MedinTuxTools-QT4/Theme.cpp \
    C_RubriquesTabBar.cpp \
    C_RubriquesManager.cpp \
    C_Mdi.cpp \
    ../../MedinTuxTools-QT4/C_BaseCommon.cpp \
    ../../MedinTuxTools-QT4/C_Utils_Log.cpp \
    C_SqlBase.cpp \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.cpp


HEADERS += C_MainWindow.h \
           CApp.h \
           C_QMdiArea.h \
           C_QMdiSubWindow.h \
           ../../MedinTuxTools-QT4/C_AppCore.h \
           ../../MedinTuxTools-QT4/Theme.h \
           ../../MedinTuxTools-QT4/CGestIni.h \
    C_RubriquesTabBar.h \
    C_RubriquesManager.h \
    C_Mdi.h \
    ../../MedinTuxTools-QT4/C_BaseCommon.h \
    ../../MedinTuxTools-QT4/C_Utils_Log.h \
    C_SqlBase.h \
    DataBase.h \
    ../../MedinTuxTools-QT4/uuid/C_MyUuid.h

# FORMS     += C_MainWindow.ui

OTHER_FILES += ../bin/$${TARGET}.ini

# ......... define building path ..................
unix:OBJECTS_DIR  = $${SRC_DIR}/obj/unix
win32:OBJECTS_DIR = $${SRC_DIR}/obj/win32
mac:OBJECTS_DIR   = $${SRC_DIR}/obj/mac

UI_DIR   = $${SRC_DIR}/ui
MOC_DIR  = $${SRC_DIR}/moc
RCC_DIR  = $${SRC_DIR}/rcc
TRANSLATIONS = $${TARGET}_fr.ts

# greaterThan(QT_MAJOR_VERSION, 4): unix: LIBS += -L$$PWD/../../../../../../../../usr/lib/ -lsqlite


#INCLUDEPATH += $$PWD/../../../../../../../../usr/include
#DEPENDPATH  += $$PWD/../../../../../../../../usr/include
