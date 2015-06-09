#!/bin/bash

# à lancer en superutilisateur sudo ./installQt3.sh

 if [[ -z "$1" ]];then
   workFolder=$(readlink -f $(dirname $0))
 else
   workFolder=$1
 fi

echo "---------- installQt3 from : $workFolder ----------------------------"
user_log3=$LOGNAME
echo "user 3 : (installQt3.sh) "$user_log3

#--------------------- installer Qt3 et Qt4 -------------------------------------------

sudo mkdir /usr/lib/qt3
sudo mkdir /usr/lib/qt3/plugins
cd $workFolder
sudo cp -a toPutInUsrLib/lib* /usr/lib
sudo cp -a toPutInUsrLib/plugins /usr/lib/qt3

#------------- refaire les liens dans le repertoire des lib de depart de Qt3 ----------
echo "===> re make link in  '$workFolder/toPutInUsrLib' "
cd toPutInUsrLib
rm libqt-mt.so.3.3
rm libqt-mt.so.3
rm libqt-mt.so

ln -s libqt-mt.so.3.3.8 libqt-mt.so.3.3
ln -s libqt-mt.so.3.3.8 libqt-mt.so.3
ln -s libqt-mt.so.3.3.8 libqt-mt.so

rm libqui.so.1.0
rm libqui.so.1
rm libqui.so

ln -s libqui.so.1.0.0 libqui.so.1.0
ln -s libqui.so.1.0.0 libqui.so.1
ln -s libqui.so.1.0.0 libqui.so
cd ..

#-------------------- refaire les liens dans Qt3 lui meme -----------------------------
echo "===> re make link in  '$workFolder/lib' "
cd lib

rm libqt-mt.so.3.3
rm libqt-mt.so.3
rm libqt-mt.so

ln -s libqt-mt.so.3.3.8 libqt-mt.so.3.3
ln -s libqt-mt.so.3.3.8 libqt-mt.so.3
ln -s libqt-mt.so.3.3.8 libqt-mt.so

rm libqui.so.1.0
rm libqui.so.1
rm libqui.so

ln -s libqui.so.1.0.0 libqui.so.1.0
ln -s libqui.so.1.0.0 libqui.so.1
ln -s libqui.so.1.0.0 libqui.so

#-------------------- refaire ces liens en dernier ------------------------------------
echo "===> re make link in  '/usr/lib' "
cd /usr/lib
ln -s libqt-mt.so.3.3.8 libqt.so.3

