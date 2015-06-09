#!/bin/bash

File_Err=./Err.Mt
Projet=Medintux
MYSQL_LIB=libmysqlclient.18.dylib
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
 echo -e "-----------------------------------------------"
 echo -e "Debut commande stripping pour projet $Projet"
 echo -e "-----------------------------------------------"
 if [ $2 ==  ];then
 	LISTE="ccamview drtux gest_user personnes guinch QLightPad QLaboFTP Manager mise_a_jour set_bases comptabilite check_dus compta-plugins qgetdatasemp qmedicabase manu med_stat cal_obst calendrier param_view MrPropre"
 else
 	LISTE=$2
 fi
 cd ../..
 for module in $LISTE
 do
         echo -en "      - Strip Executable Module $module"

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


         #......................... repertoires sources pour compilation ...................
         src_rep=./src
         if [ $module == 'compta-plugins' ]; then
             src_rep=./compta-plugins
         else
             if [ $module == 'check_dus' ]; then
                 src_rep=./check_dus
             else
                 if [ $module == 'comptabilite' ]; then
                     src_rep=./compta
                 else
                     if [ $module == 'guinch' ]; then
                           src_rep=./guinch
                     else
                           src_rep=./src
                     fi
                 fi
             fi
         fi
         #................. se mettre dans le rep src et nettoyer .........................
         cd $src_rep
         cd ..
         #................. se mettre dans le rep src et nettoyer .........................
         if [ $Os == 'Darwin' ]; then
	     filestrip=./bin/$module.app/Contents/MacOS/$module
             strip -u -r $filestrip
             if [ -f $filestrip.strip ]; then
                  rm $filestrip
	          mv $filestrip.strip $filestrip
             fi
         else
	         filestrip=./bin/$module
	         strip -s $filestrip
	     fi

         if [ $? == 0 ];then
                 echo -e " OK"
         else
                 error "Erreur : $? Patch sur $filestrip"
         fi
	cd ..
 done
 echo -e "-----------------------------------------------"
 echo -e "Fin commande stripping pour projet $Projet"
 echo -e "-----------------------------------------------"


