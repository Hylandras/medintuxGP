CONFIG      += designer plugin
 TARGET      = $$qtLibraryTarget($$TARGET)
 TEMPLATE    = lib
 QTDIR_build:DESTDIR     = $$QT_BUILD_TREE/plugins/designer

 HEADERS     = C_QLineEdit.h \
               C_QLineEditPlugin.h
 SOURCES     = C_QLineEdit.cpp \
               C_QLineEditPlugin.cpp

 # install
 target.path = $$[QT_INSTALL_PLUGINS]/designer
 sources.files = $$SOURCES $$HEADERS *.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/designer/customwidgetplugin
 INSTALLS += target sources
