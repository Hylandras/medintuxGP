#-------------------------------------------------
#
# Project created by QtCreator 2012-01-16T17:27:13
#
#-------------------------------------------------


QT          += core gui webkit network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

#.............. include qjson parser ...........................
INCLUDEPATH += $$PWD

include(../../qjson/src/qjson.pri)

linux* {
    #LIBS += ../../MedinTuxTools-QT4/qjson/bin/libqjson.so
    INCLUDEPATH += ../../MedinTuxTools-QT4/qjson/include
}

win32-g++ {
    #LIBS += ../../MedinTuxTools-QT4/qjson/bin/qjson0.dll
    INCLUDEPATH += ../../MedinTuxTools-QT4/qjson/include
}

win32-msvc* {
    #LIBS += ../../MedinTuxTools-QT4/qjson/bin/qjson0.lib
    INCLUDEPATH += ../../MedinTuxTools-QT4/qjson/include
}

macx* {
    INCLUDEPATH += ../../MedinTuxTools-QT4/qjson/include
    #LIBS += ../../MedinTuxTools-QT4/qjson/bin/libqjson.dylib
}


HEADERS += $$PWD/C_OAuth2.h \
           $$PWD/C_Dlg_GetSecrets.h \
           $$PWD/C_Dlg_logindialog.h \
           $$PWD/../../CGestIni.h

SOURCES += $$PWD/C_OAuth2.cpp \
           $$PWD/C_Dlg_GetSecrets.cpp \
           $$PWD/C_Dlg_logindialog.cpp \
           $$PWD/../../CGestIni.cpp

FORMS    +=  $$PWD/C_Dlg_logindialog.ui \
             $$PWD/C_Dlg_GetSecrets.ui
message( "---------------- C_GoogleOAuth2.pri-------------------------------" )
message( " Google oAuth 2 authentification implementation for MedinTux" )
message( " WARNING : C_GoogleOAuth2.pri brings CGestIni don't  include it after" )
message( " in your project if you see this message." )
message( "------------------------------------------------------------------" )









