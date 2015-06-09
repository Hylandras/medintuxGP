#!/bin/bash

Projet=Medintux
MODULE=$1
export MODULE

if [ $2 == ];then
   FROM=/usr/local/Trolltech/Qt-4.7.0/lib
else
   FROM=$2
fi
export FROM

if [ $3 == ];then
   TO=@executable_path/../../../../lib
else
   TO=$2
fi
export TO

# Debut  
# clear
 echo -e "--------------------------------------"
 echo -e "Debut commande modulepatch-Qt4 du module : $MODULE pour projet : $Projet"
 echo -e "--------------------------------------"

 cd $MODULE.app/Contents/MacOS  # se mettre dans le bon endroit 

MYSQL_LIB=libmysqlclient.18.dylib
export MYSQL_LIB


echo -e "................ $MODULE ................"
otool -L $MODULE                       # /Module/bin/Module.app/Contents/MacOS  ==>  /../../../../../
install_name_tool -change $FROM/libQtCLucene.4.dylib @executable_path/../../../../lib/libQtCLucene.4.dylib $MODULE
install_name_tool -change $FROM/QtDesignerComponents.framework/Versions/4/QtDesignerComponents $TO/QtDesignerComponents.framework/Versions/4/QtDesignerComponents $MODULE
install_name_tool -change $FROM/phonon.framework/Versions/4/phonon $TO/phonon.framework/Versions/4/phonon $MODULE
install_name_tool -change $FROM/Qt3Support.framework/Versions/4/Qt3Support $TO/Qt3Support.framework/Versions/4/Qt3Support $MODULE
install_name_tool -change $FROM/QtCore.framework/Versions/4/QtCore $TO/QtCore.framework/Versions/4/QtCore $MODULE
install_name_tool -change $FROM/QtDeclarative.framework/Versions/4/QtDeclarative $TO/QtDeclarative.framework/Versions/4/QtDeclarative $MODULE
install_name_tool -change $FROM/QtGui.framework/Versions/4/QtGui $TO/QtGui.framework/Versions/4/QtGui $MODULE
install_name_tool -change $FROM/QtHelp.framework/Versions/4/QtHelp $TO/QtHelp.framework/Versions/4/QtHelp $MODULE
install_name_tool -change $FROM/QtMultimedia.framework/Versions/4/QtMultimedia $TO/QtMultimedia.framework/Versions/4/QtMultimedia $MODULE
install_name_tool -change $FROM/QtNetwork.framework/Versions/4/QtNetwork $TO/QtNetwork.framework/Versions/4/QtNetwork $MODULE
install_name_tool -change $FROM/QtOpenGL.framework/Versions/4/QtOpenGL $TO/QtOpenGL.framework/Versions/4/QtOpenGL $MODULE
install_name_tool -change $FROM/QtScript.framework/Versions/4/QtScript $TO/QtScript.framework/Versions/4/QtScript $MODULE
install_name_tool -change $FROM/QtScriptTools.framework/Versions/4/QtScriptTools $TO/QtScriptTools.framework/Versions/4/QtScriptTools $MODULE
install_name_tool -change $FROM/QtSql.framework/Versions/4/QtSql $TO/QtSql.framework/Versions/4/QtSql $MODULE
install_name_tool -change $FROM/QtSvg.framework/Versions/4/QtSvg $TO/QtSvg.framework/Versions/4/QtSvg $MODULE
install_name_tool -change $FROM/QtTest.framework/Versions/4/QtTest $TO/QtTest.framework/Versions/4/QtTest $MODULE
install_name_tool -change $FROM/QtWebKit.framework/Versions/4/ $TO/QtWebKit.framework/Versions/4/ $MODULE
install_name_tool -change $FROM/QtXml.framework/Versions/4/QtXml $TO/QtXml.framework/Versions/4/QtXml $MODULE
install_name_tool -change $FROM/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $TO/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $MODULE
install_name_tool -change $MYSQL_LIB @executable_path/../../../../lib/$MYSQL_LIB $MODULE
otool -L $MODULE

cd ../../..
echo -e "---------------------------------"
echo -e "Fin commande modulepatch-Qt4 du module : $MODULE pour projet : $Projet"
echo -e "---------------------------------"


