#!/bin/bash

File_Err=./Err.Mt
Projet=Medintux

Os=`uname`

# Routine d'affichage Erreur
# Sign : String --> nil
#        Erreur --> nil
error()
{
	
	echo "		$1"
	if [ -f $File_Err ];then
		cat $File_Err
	fi
	rm $File_Err
	exit 1
}
#

# Debut  
# clear
 echo -e "--------------------------------------"
 echo -e "Debut commande stripping pour projet $Projet"
 echo -e "--------------------------------------"
 if [ $1 ==  ];then
 	LISTE="ccamview drtux gest_user personnes guinch QLightPad QLaboFTP Manager mise_a_jour set_bases comptabilite check_dus compta-plugins qgetdatasemp qmedicabase manu med_stat cal_obst calendrier param_view"
 else
 	LISTE=$1
 fi
 cd ../..
 for module in $LISTE
 do
         echo -en "      - Patch executable Mac lib QT pour $module"

         if [ $module == 'compta-plugins' ]; then
             cd comptabilite
         else
              if [ $module == 'check_dus' ]; then
                  cd comptabilite
              else
                  if [ $module == 'guinch' ]; then
                     cd Manager
                  else
                     cd $module
                  fi
              fi
         fi
	 if [ $Os == 'Darwin' ]; then
	         filestrip=./bin/$module.app/Contents/MacOS/$module
		 # QTlib
                 install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../QT/lib/libqt-mt.3.dylib $filestrip 
                 #  install_name_tool -change @executable_path/../../../../QT/lib/libqt-mt.3.3.8.dylib @executable_path/../../../../../QT/lib/libqt-mt.3.3.8.dylib $filestrip
		 # Mysqllib
                 # install_name_tool -change /usr/local/mysql/lib/libmysqlclient.15.dylib @executable_path/../../../../../QT/lib/libmysqlclient.15.dylib $filestrip 
	 fi
         if [ $? == 0 ];then
                 echo -e " OK"
         else
                 error "Erreur : $? Patch sur $filestrip"
         fi
	 cd ..
 done
 echo -e "---------------------------------"
 echo -e "Fin commande stripping pour projet $Projet"
 echo -e "---------------------------------"


