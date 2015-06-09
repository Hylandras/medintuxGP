#!/bin/bash

Projet=Medintux

# Debut  
# clear
 echo -e "--------------------------------------"
 echo -e "Debut commande Patch lib Qt4 pour projet $Projet"
 echo -e "--------------------------------------"
 
MYSQL_LIB=libmysqlclient.18.dylib

QT_VERS=4.7.0
QT_SRC=Qt-4.7.0
QT_DST=Qt4-Mac
export MYSQL_LIB

cd ../../$QT_DST         # se mettre dans le bon endroit car on part de Qt/set_bases/bin/

echo -e "................ phonon ................"
otool -L phonon                       # /Module/bin/Module.app/Contents/MacOS  ==>  /../../../../../
install_name_tool -id @executable_path/../../../../../Qt4-Mac/phonon phonon
#                         old value                                                          new value                                     target
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui phonon
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore phonon
otool -L phonon

echo -e "................ Qt3Support ................"
otool -L Qt3Support
install_name_tool -id @executable_path/../../../../../$QT_DST/Qt3Support Qt3Support
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtSql.framework/Versions/4/QtSql @executable_path/../../../../../$QT_DST/QtSql Qt3Support
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtXml.framework/Versions/4/QtXml @executable_path/../../../../../$QT_DST/QtXml Qt3Support
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui Qt3Support
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../$QT_DST/QtNetwork Qt3Support
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore Qt3Support
install_name_tool -change $MYSQL_LIB @executable_path/../../../../../$QT_DST/$MYSQL_LIB QtSql
otool -L Qt3Support

echo -e "................ QtCore ................"
otool -L QtCore
install_name_tool -id @executable_path/../../../../../$QT_DST/QtCore QtCore
otool -L QtCore

echo -e "................ QtDeclarative ................"
otool -L QtDeclarative
install_name_tool -id @executable_path/../../../../../$QT_DST/QtDeclarative QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtScript.framework/Versions/4/QtScript @executable_path/../../../../../$QT_DST/QtScript QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtSvg.framework/Versions/4/QtSvg @executable_path/../../../../../$QT_DST/QtSvg QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtSql.framework/Versions/4/QtSql @executable_path/../../../../../$QT_DST/QtSql QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtXmlPatterns.framework/Versions/4/QtXmlPatterns @executable_path/../../../../../$QT_DST/QtXmlPatterns QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtOpenGL.framework/Versions/4/QtOpenGL @executable_path/../../../../../$QT_DST/QtOpenGL QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../$QT_DST/QtNetwork QtDeclarative
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtDeclarative
install_name_tool -change $MYSQL_LIB @executable_path/../../../../../$QT_DST/$MYSQL_LIB QtSql
otool -L QtDeclarative

echo -e "................ QtGui ................"
otool -L QtGui
install_name_tool -id @executable_path/../../../../../$QT_DST/QtGui QtGui
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtGui
otool -L QtGui

echo -e "................ QtHelp ................"
otool -L QtHelp
install_name_tool -id @executable_path/../../../../../$QT_DST/QtHelp QtHelp
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtSql.framework/Versions/4/QtSql @executable_path/../../../../../$QT_DST/QtSql QtHelp
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtXml.framework/Versions/4/QtXml @executable_path/../../../../../$QT_DST/QtXml QtHelp
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtHelp
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../$QT_DST/QtNetwork QtHelp
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtHelp
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/libQtCLucene.4.dylib @executable_path/../../../../../$QT_DST/libQtCLucene.4.dylib QtHelp
install_name_tool -change $MYSQL_LIB @executable_path/../../../../../$QT_DST/$MYSQL_LIB QtSql
otool -L QtHelp

echo -e "................ QtMultimedia ................"
otool -L QtMultimedia
install_name_tool -id @executable_path/../../../../../$QT_DST/QtMultimedia QtMultimedia
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtMultimedia
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtMultimedia
otool -L QtMultimedia

echo -e "................ QtNetwork ................"
otool -L QtNetwork
install_name_tool -id @executable_path/../../../../../$QT_DST/QtNetwork QtNetwork
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtNetwork
otool -L QtNetwork

echo -e "................ QtOpenGL ................"
otool -L QtOpenGL
install_name_tool -id @executable_path/../../../../../$QT_DST/QtOpenGL QtOpenGL
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtOpenGL
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtOpenGL
otool -L QtOpenGL

echo -e "................ QtScript ................"
otool -L QtScript
install_name_tool -id @executable_path/../../../../../$QT_DST/QtScript QtScript
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtScript
otool -L QtScript

echo -e "................ QtScriptTools ................"
otool -L QtScriptTools
install_name_tool -id @executable_path/../../../../../$QT_DST/QtScriptTools QtScriptTools
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtScript.framework/Versions/4/QtScript @executable_path/../../../../../$QT_DST/QtScript QtScriptTools
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtScriptTools
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtScriptTools
otool -L QtScriptTools

echo -e "................ QtSql ................"
otool -L QtSql
install_name_tool -id @executable_path/../../../../../$QT_DST/QtSql QtSql
install_name_tool -change $MYSQL_LIB @executable_path/../../../../../$QT_DST/$MYSQL_LIB QtSql
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtSql
otool -L QtSql

echo -e "................ QtSvg ................"
otool -L QtSvg
install_name_tool -id @executable_path/../../../../../$QT_DST/QtSvg QtSvg
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtSvg
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtSvg
otool -L QtSvg

echo -e "................ QtTest ................"
otool -L QtTest
install_name_tool -id @executable_path/../../../../../$QT_DST/QtTest QtTest
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtTest
otool -L QtTest

echo -e "................ QtWebKit ................"
otool -L QtWebKit
install_name_tool -id @executable_path/../../../../../$QT_DST/QtWebKit QtWebKit
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/phonon.framework/Versions/4/phonon @executable_path/../../../../../$QT_DST/phonon QtWebKit
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtGui.framework/Versions/4/QtGui @executable_path/../../../../../$QT_DST/QtGui QtWebKit
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../$QT_DST/QtNetwork QtWebKit
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtWebKit
otool -L QtWebKit

echo -e "................ QtXml ................"
otool -L QtXml
install_name_tool -id @executable_path/../../../../../$QT_DST/QtXml QtXml
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtXml
otool -L QtXml

echo -e "................ QtXmlPatterns ................"
otool -L QtXmlPatterns
install_name_tool -id @executable_path/../../../../../$QT_DST/QtXmlPatterns QtXmlPatterns
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtNetwork.framework/Versions/4/QtNetwork @executable_path/../../../../../$QT_DST/QtNetwork QtXmlPatterns
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore QtXmlPatterns
otool -L QtXmlPatterns

echo -e "................ libQtCLucene.4.7.0.dylib ................"
otool -L libQtCLucene.$QT_VERS.dylib
install_name_tool -id @executable_path/../../../../../$QT_DST/libQtCLucene.$QT_VERS.dylib libQtCLucene.$QT_VERS.dylib
install_name_tool -change /usr/local/Trolltech/$QT_SRC/lib/QtCore.framework/Versions/4/QtCore @executable_path/../../../../../$QT_DST/QtCore libQtCLucene.$QT_VERS.dylib
otool -L libQtCLucene.$QT_VERS.dylib


echo -e "---------------------------------"
echo -e "Fin commande Pach lib Qt pour projet $Projet"
echo -e "---------------------------------"


