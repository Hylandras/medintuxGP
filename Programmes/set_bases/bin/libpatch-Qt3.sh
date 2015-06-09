#!/bin/bash

Projet=Medintux

# Debut  
# clear
 echo -e "--------------------------------------"
 echo -e "Debut commande Pach lib Qt pour projet $Projet"
 echo -e "--------------------------------------"
 
cd ../../Qt/lib         #se mettre dans Qt/lib  car on part de Qt/set_bases/bin/
MYSQL_LIB=libmysqlclient.18.dylib
export MYSQL_LIB
echo -e "................ libqt-mt.3.3.8.dylib ................"
otool -L libqt-mt.3.3.8.dylib
install_name_tool -id @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib libqt-mt.3.3.8.dylib
# install_name_tool -change /usr/local/mysql/lib/mysql/$MYSQL_LIB @executable_path/../../../../../Qt/lib/$MYSQL_LIB libqt-mt.3.3.8.dylib
install_name_tool -change $MYSQL_LIB @executable_path/../../../../../Qt/lib/$MYSQL_LIB libqt-mt.3.3.8.dylib
otool -L libqt-mt.3.3.8.dylib

echo -e "................ libqui.1.0.0.dylib ................"
otool -L libqui.1.0.0.dylib
install_name_tool -id @executable_path/../../../../../Qt/lib/libqui.1.0.0.dylib libqui.1.0.0.dylib
install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib libqui.1.0.0.dylib
otool -L libqui.1.0.0.dylib

echo -e "................ $MYSQL_LIB ................"
otool -L $MYSQL_LIB
install_name_tool -id @executable_path/../../../../../Qt/lib/$MYSQL_LIB $MYSQL_LIB
otool -L $MYSQL_LIB

echo -e "................ Patch designer ................"
cd ../bin/designer.app/Contents/MacOS
otool -L designer
install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib designer
install_name_tool -change libqui.1.dylib @executable_path/../../../../../Qt/lib/libqui.1.0.0.dylib designer

install_name_tool -change @executable_path/../../../../Qt/lib/libqt-mt.3.3.8.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib designer
install_name_tool -change @executable_path/../../../../Qt/lib/libqui.1.0.0.dylib @executable_path/../../../../../Qt/lib/libqui.1.0.0.dylib designer
otool -L designer
echo -e "................ Patch assistant ................"
cd ../../../assistant.app/Contents/MacOS
otool -L assistant
install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib assistant
install_name_tool -change @executable_path/../../../../Qt/lib/libqt-mt.3.3.8.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib assistant
otool -L assistant
echo -e "................ Patch linguist ................"
cd ../../../linguist.app/Contents/MacOS
otool -L linguist
install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib linguist
install_name_tool -change @executable_path/../../../../Qt/lib/libqt-mt.3.3.8.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib linguist
otool -L linguist
echo -e "................ Patch qtconfig ................"
cd ../../../qtconfig.app/Contents/MacOS
otool -L qtconfig
install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib qtconfig
install_name_tool -change @executable_path/../../../../Qt/lib/libqt-mt.3.3.8.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib qtconfig
otool -L qtconfig
echo -e "................ Patch uic ................"
cd ../../../../bin
otool -L uic
install_name_tool -change libqt-mt.3.dylib @executable_path/../../Qt/lib/libqt-mt.3.3.8.dylib uic
otool -L uic
 echo -e "---------------------------------"
 echo -e "Fin commande Pach lib Qt pour projet $Projet"
 echo -e "---------------------------------"


