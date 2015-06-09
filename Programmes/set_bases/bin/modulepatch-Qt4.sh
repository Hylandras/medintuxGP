#!/bin/bash

Projet=Medintux

QT_VERS=4.7.0
QT_SRC=Qt-4.7.0
QT_DST=Qt4-Mac

#........... param 1 nom du MODULE a traiter .................
MODULE=$1
export MODULE
#........... param 2 chemin du repertoire des librairies .................
if [ $2 == ];then
   TO_LIBREP=@executable_path/../../../../../$QT_DST
else
   TO_LIBREP=$2
fi
export TO_LIBREP

#........... param 3 nom de la librairies MySQL.................
if [ $3 == ];then
   MYSQL_LIB_NAME=libmysqlclient.18.dylib
else
   MYSQL_LIB_NAME=$3
fi
export MYSQL_LIB_NAME

#........... param 4 chemin du repertoire de la librairies MySQL.................
if [ $4 == ];then
   MYSQL_LIB_PATH=$TO_LIBREP
else
   MYSQL_LIB_PATH=$4
fi
export MYSQL_LIB_PATH

display_help()
{   echo -e "-------------------------------- modulepatch-Qt4 -----------------------------"
    echo -e "Debut commande modulepatch-Qt4 du module : $MODULE pour projet : $Projet"
    echo -e "Ce module doit s'executer a partir du repertoire binaire de set_bases."
    echo -e "Le mode administrateur est requis"
    echo -e "1) Le premier parametre est le nom du module QT4"
    echo -e "2) Le deuxieme parametre (facultatif) est le chemin relatif a l'executable des librairies QT4"
    echo -e "   par defaut : @executable_path/../../../../../Qt4-Mac"
    echo -e "3) Le troisieme parametre (facultatif) est le nom de la lib MySQL"
    echo -e "   par defaut : libmysqlclient.18.dylib"
    echo -e "4) Le quatrieme parametre (facultatif) est le chemin relatif a l'executable de la lib MySQL"
    echo -e "   par defaut : celui du deuxieme parametre"
    echo -e "-> exemple de commande : ./modulepatch-Qt4 Manager"
    echo -e "-> exemple de commande : ./modulepatch-Qt4 Manager @executable_path/../../../../../Qt-MacLib"
    echo -e "-> exemple de commande : ./modulepatch-Qt4 Manager @executable_path/../../../../../Qt-MacLib libmysqlclient.18.dylib @executable_path/../../../../../Qt-MacLib"
    echo -e "------------------------------------------------------------------------------"
}

if [ $MODULE == '--help' ];then
 echo $MODULE
 display_help
 exit
fi



# Debut  
# clear
 display_help

 if [ $MODULE == 'guinch' ]; then
     cd ../../Manager/bin/$MODULE.app/Contents/MacOS  # se mettre dans le bon endroit car on part de set_bases/bin/    
 else
     cd ../../$MODULE/bin/$MODULE.app/Contents/MacOS  # se mettre dans le bon endroit car on part de set_bases/bin/
 fi


echo -e "................ $MODULE ................"
otool -L $MODULE                       # /Module/bin/Module.app/Contents/MacOS  ==>  /../../../../../
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/libQtCLucene.$QT_VERS.dylib $TO_LIBREP/libQtCLucene.4.7.0.dylib $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/phonon.framework/Versions/4/phonon $TO_LIBREP/phonon $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/Qt3Support.framework/Versions/4/Qt3Support $TO_LIBREP/Qt3Support $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtDesigner.framework/Versions/4/QtDesigner $TO_LIBREP/QtDesigner $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtDesignerComponents.framework/Versions/4/QtDesignerComponents $TO_LIBREP/QtDesignerComponents $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtCore.framework/Versions/4/QtCore $TO_LIBREP/QtCore $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtDeclarative.framework/Versions/4/QtDeclarative $TO_LIBREP/QtDeclarative $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtGui.framework/Versions/4/QtGui $TO_LIBREP/QtGui $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtHelp.framework/Versions/4/QtHelp $TO_LIBREP/QtHelp $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtMultimedia.framework/Versions/4/QtMultimedia $TO_LIBREP/QtMultimedia $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtNetwork.framework/Versions/4/QtNetwork $TO_LIBREP/QtNetwork $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtOpenGL.framework/Versions/4/QtOpenGL $TO_LIBREP/QtOpenGL $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtScript.framework/Versions/4/QtScript $TO_LIBREP/QtScript $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtScriptTools.framework/Versions/4/QtScriptTools $TO_LIBREP/QtScriptTools $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtSql.framework/Versions/4/QtSql $TO_LIBREP/QtSql $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtSvg.framework/Versions/4/QtSvg $TO_LIBREP/QtSvg $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtTest.framework/Versions/4/QtTest $TO_LIBREP/QtTest $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtWebKit.framework/Versions/4/QtWebKit $TO_LIBREP/QtWebKit $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtXml.framework/Versions/4/QtXml $TO_LIBREP/QtXml $MODULE
install_name_tool -change /usr/local/Trolltech/Qt-$QT_VERS/lib/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $TO_LIBREP/QtXmlPatterns $MODULE
install_name_tool -change $MYSQL_LIB_NAME $MYSQL_LIB_PATH/$MYSQL_LIB_NAME $MODULE
otool -L $MODULE

cd ../../../../../set_bases/bin/set_bases.app/Contents/MacOS  # se mettre dans le bon endroit pour revenir dans set_bases/bin/
echo -e "---------------------------------"
echo -e "Fin commande modulepatch-Qt4 du module : $MODULE pour projet : $Projet"
echo -e "---------------------------------"


