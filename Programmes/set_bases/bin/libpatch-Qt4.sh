#!/bin/bash

Projet=Medintux

# Debut  
# clear
 echo -e "--------------------------------------"
 echo -e "Debut commande Patch lib Qt4 pour projet $Projet"
 echo -e "--------------------------------------"
 
cd ../../Qt4-Mac         # se mettre dans le bon endroit car on part de Qt/set_bases/bin/
MYSQL_LIB=libmysqlclient.18.dylib
export MYSQL_LIB

echo -e "................ phonon ................"
otool -L phonon                       # /Module/bin/Module.app/Contents/MacOS  ==>  /../../../../../
install_name_tool -id @executable_path/../../../../../Qt4-Mac/phonon phonon
#                         old value                                                          new value                                     target
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui phonon
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore phonon
otool -L phonon

echo -e "................ Qt3Support ................"
otool -L Qt3Support
install_name_tool -id @executable_path/../../../../../Qt4-Mac/Qt3Support Qt3Support
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtSql.framework/Versions/4/QtSql @executable_path/../../../../../Qt4-Mac/QtSql Qt3Support
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtXml.framework/Versions/4/QtXml @executable_path/../../../../../Qt4-Mac/QtXml Qt3Support
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui Qt3Support
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../Qt4-Mac/QtNetwork Qt3Support
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore Qt3Support
otool -L Qt3Support

echo -e "................ QtCore ................"
otool -L QtCore
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtCore QtCore
otool -L QtCore

echo -e "................ QtDeclarative ................"
otool -L QtDeclarative
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtDeclarative QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtScript.framework/Versions/4/QtScript @executable_path/../../../../../Qt4-Mac/QtScript QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtSvg.framework/Versions/4/QtSvg @executable_path/../../../../../Qt4-Mac/QtSvg QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtSql.framework/Versions/4/QtSql @executable_path/../../../../../Qt4-Mac/QtSql QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtXmlPatterns.framework/Versions/4/QtXmlPatterns @executable_path/../../../../../Qt4-Mac/QtXmlPatterns QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtOpenGL.framework/Versions/4/QtOpenGL @executable_path/../../../../../Qt4-Mac/QtOpenGL QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../Qt4-Mac/QtNetwork QtDeclarative
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtDeclarative
otool -L QtDeclarative

echo -e "................ QtGui ................"
otool -L QtGui
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtGui QtGui
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtGui
otool -L QtGui

echo -e "................ QtHelp ................"
otool -L QtHelp
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtHelp QtHelp
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtSql.framework/Versions/4/QtSql @executable_path/../../../../../Qt4-Mac/QtSql QtHelp
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtXml.framework/Versions/4/QtXml @executable_path/../../../../../Qt4-Mac/QtXml QtHelp
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtHelp
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../Qt4-Mac/QtNetwork QtHelp
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtHelp
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/libQtCLucene.4.dylib @executable_path/../../../../../Qt4-Mac/libQtCLucene.4.dylib QtHelp
otool -L QtHelp

echo -e "................ QtMultimedia ................"
otool -L QtMultimedia
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtMultimedia QtMultimedia
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtMultimedia
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtMultimedia
otool -L QtMultimedia

echo -e "................ QtNetwork ................"
otool -L QtNetwork
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtNetwork QtNetwork
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtNetwork
otool -L QtNetwork

echo -e "................ QtOpenGL ................"
otool -L QtOpenGL
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtOpenGL QtOpenGL
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtOpenGL
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtOpenGL
otool -L QtOpenGL

echo -e "................ QtScript ................"
otool -L QtScript
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtScript QtScript
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtScript
otool -L QtScript

echo -e "................ QtScriptTools ................"
otool -L QtScriptTools
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtScriptTools QtScriptTools
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtScript.framework/Versions/4/QtScript @executable_path/../../../../../Qt4-Mac/QtScript QtScriptTools
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtScriptTools
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtScriptTools
otool -L QtScriptTools

echo -e "................ QtSql ................"
otool -L QtSql
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtSql QtSql
install_name_tool -change $MYSQL_LIB @executable_path/../../../../../Qt/lib/$MYSQL_LIB QtSql
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtSql
otool -L QtSql

echo -e "................ QtSvg ................"
otool -L QtSvg
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtSvg QtSvg
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtSvg
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtSvg
otool -L QtSvg

echo -e "................ QtTest ................"
otool -L QtTest
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtTest QtTest
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtTest
otool -L QtTest

echo -e "................ QtWebKit ................"
otool -L QtWebKit
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtWebKit QtWebKit
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/phonon.framework/Versions/4/phonon @executable_path/../../../../../Qt4-Mac/phonon QtWebKit
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../Qt4-Mac/QtGui QtWebKit
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../Qt4-Mac/QtNetwork QtWebKit
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtWebKit
otool -L QtWebKit

echo -e "................ QtXml ................"
otool -L QtXml
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtXml QtXml
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtXml
otool -L QtXml

echo -e "................ QtXmlPatterns ................"
otool -L QtXmlPatterns
install_name_tool -id @executable_path/../../../../../Qt4-Mac/QtXmlPatterns QtXmlPatterns
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../Qt4-Mac/QtNetwork QtXmlPatterns
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore QtXmlPatterns
otool -L QtXmlPatterns

echo -e "................ libQtCLucene.4.7.0.dylib ................"
otool -L libQtCLucene.4.7.0.dylib
install_name_tool -id @executable_path/../../../../../Qt4-Mac/libQtCLucene.4.7.0.dylib libQtCLucene.4.7.0.dylib
install_name_tool -change /usr/local/Trolltech/Qt-4.7.0/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../Qt4-Mac/QtCore libQtCLucene.4.7.0.dylib
otool -L libQtCLucene.4.7.0.dylib


 echo -e "---------------------------------"
 echo -e "Fin commande Pach lib Qt pour projet $Projet"
 echo -e "---------------------------------"


