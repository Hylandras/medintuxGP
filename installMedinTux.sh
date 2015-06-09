#!/bin/bash

# à lancer en utilisateur normal ./installMedinTux.sh

workFolder=$(readlink -f $(dirname $0))
clear
echo "---------- MedinTux installation from : $workFolder -------------"
user_log1=$LOGNAME
echo "user 1 : (installMedinTux.sh) "$user_log1 
#-------------------- activer les icones dans les menus ----------------------------------
gconftool-2 --type boolean --set /desktop/gnome/interface/buttons_have_icons true
gconftool-2 --type boolean --set /desktop/gnome/interface/menus_have_icons true 

#--------------------  rendre set_bases executable----------------------------------------
chmod ugo+x $workFolder/Programmes/set_bases/bin/set_bases
#--------------------  rendre le script de lancement dialogue lancement-------------------
chmod ugo+x $workFolder/Programmes/Doc/Start_MedinTux/start_medintux.sh

#-------------------- creer raccourcis ---------------------------------------------------
chmod ugo+x $workFolder/Icones/ubuntu_raccourcis/installShortCut.sh
$workFolder/Icones/ubuntu_raccourcis/./installShortCut.sh $workFolder
#-------------------- installer les paquets indispensables MedinTux-----------------------
echo "---------- MedinTux installation de QT3 et Qt4 en sudo -------------"
chmod ugo+x $workFolder/qt3_l64/installQt3.sh
chmod ugo+x $workFolder/qt4_l64/installQt4.sh

echo "---------- MedinTux MySQL installation -----------------------------"
isMySQL=no
if [ -f "/usr/bin/mysql" ]; then
   echo "mysql seems to be present"
   isMySQL=yes;
   #sudo mysql -uroot -e "CREATE USER 'root'@'localhost' IDENTIFIED BY 'admin'"
   #"c:\wamp\bin\mysql\mysql5.5.16\bin\mysql" -u root -e "GRANT ALL PRIVILEGES ON *.* TO ''@'' IDENTIFIED BY ''"
   #"c:\wamp\bin\mysql\mysql5.5.16\bin\mysql" -u root -e "FLUSH PRIVILEGES";
else
   sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password password admin'
   sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password admin'
   sudo apt-get install -y mysql-server
fi
echo "---------- MedinTux installation des paquets ubuntu en sudo -------------"
sudo apt-get install -y libx11-dev libxext-dev libmysqlclient-dev libssl-dev
sudo apt-get install -y libqt4-sql-mysql libqt4-qt3support g++ gcc-opt synaptic

sudo $workFolder/qt3_l64/./installQt3.sh   $workFolder/qt3_l64
sudo $workFolder/qt4_l64/./installQt4.sh   $workFolder/qt4_l64
#-------------------- installer les paquets indispensables MedWebTux----------------------
sudo apt-get install -y apache2 php5-mysql php5-intl php5 php-http-request2 phpqrcode
sudo cp -r $workFolder/MedWebTux /var/www/html
sudo chown -R www-data /var/www/html/MedWebTux
#-------------------- lancer set_bases ---------------------------------------------------
echo "---------- MedinTux installation des bases de donnees -------------"
cd $workFolder/Programmes/set_bases/bin
chmod ugo+x libmac.sh
chmod ugo+x libpatch-Qt3.sh
chmod ugo+x libpatch-Qt4.sh
chmod ugo+x libpatch-Qt4_8_5.sh
chmod ugo+x libpatch-Qt_4_7_0.sh
chmod ugo+x libpatch-Qt_4_8_5.sh
chmod ugo+x MakeAllClean.sh
chmod ugo+x MakeAllMac.sh
chmod ugo+x makeModule.sh
chmod ugo+x modulepatch-Qt4.sh
chmod ugo+x modulepatch-Qt_4_7_0.sh
chmod ugo+x modulepatch-Qt_4_8_5.sh
chmod ugo+x moduleQtpatch-Qt4.sh
chmod ugo+x PostInstall-SesamAPI.sh
if [ "$isMySQL" = yes ]; then
    ./set_bases
else
   ./set_bases BASE_INSTALL
fi

#-------------------- lancer fenetre de demarrage ----------------------------------------
if [ -f "$workFolder/Programmes/APropos/bin/APropos" ]; then
   echo "MedinTux binaries seems presents"
else
   echo "MedinTux binaries semms absents"
   $workFolder/Programmes/set_bases/bin/./set_bases 'COMPILATION(param_view|qfselib|makedistrib)'
fi
$workFolder/Programmes/APropos/bin/./APropos BrowserOnly $workFolder/Programmes/Doc/Start_MedinTux/start_medintux.html



