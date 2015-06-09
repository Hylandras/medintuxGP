#!/bin/bash

# à lancer en superutilisateur sudo ./installQt4.sh

 if [[ -z "$1" ]];then
   workFolder=$(readlink -f $(dirname $0))
 else
   workFolder=$1
 fi

echo "---------- installQt4 from : $workFolder ----------------------------"
user_log4=$LOGNAME
echo "user 4 : (installQt4.sh) "$user_log4

#--------------------- installer Qt3 et Qt4 -------------------------------------------
echo "===> copy 'Qt-4.8.5' in '/usr/local/Trolltech'"
cd $workFolder
mkdir /usr/local/Trolltech
sudo cp -a 'Qt-4.8.5' /usr/local/Trolltech 

