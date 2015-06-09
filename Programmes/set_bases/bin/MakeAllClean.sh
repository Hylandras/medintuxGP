#!/bin/bash

File_Err=./Err.Mt
Projet=Medintux

Os=`uname`

#-------------------------------------------- error-------------------------------------------------
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

#-------------------------------------------- main-------------------------------------------------
#............. "1  er arguement est celui de la liste ds modules a compiler .................
#               si il commence par TARGET=  alors le nom du module doit etre extrait du chemin
#               devant suivre la mention TARGET=

if [ `echo  $1 | cut -c1-7` == 'TARGET=' ]; then
      CIBLE=$(echo $1 | grep -o "Programmes/.*"|cut -d/ -f2)
else
      CIBLE=$1
fi

# Debut
# clear
 echo -e "------------------------------------------------"
 echo -e "Debut commande nettoyage pour projet $Projet"
 echo -e "------------------------------------------------"

 if [ $CIBLE ==  ];then
        LISTE="drtux gest_user personnes QLightPad QLaboFTP mise_a_jour set_bases comptabilite check_dus compta-plugins qmedicabase med_stat calendrier MrPropre"
 else
        LISTE=$CIBLE
 fi
 cd ../..
 for module in $LISTE
 do
         # echo -en "      - Patch executable Mac lib QT pour $module"
         echo -e " "
         echo -e " ========================== Nettoyage en cours de  $module  ======================="
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
                     src_rep=./comptabilite
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
         echo -e $src_rep
         make clean
         #................. se mettre dans le rep Debug et nettoyer .........................
         toclean_rep="Debug"
         cd "${toclean_rep}"
         if [ $? == 0 ]; then
             echo "---- nettoyage du Repertoire "${toclean_rep}"----"
             ls
             rm -Rf *
             cd ..
         else
             echo "!!!!! Repertoire "${toclean_rep}" non trouve : $?"
         fi
         #................. se mettre dans le rep Release et nettoyer .........................
         toclean_rep="Release"
         cd "${toclean_rep}"
         if [ $? == 0 ]; then
             echo "---- nettoyage du Repertoire "${toclean_rep}"---- "
             ls
             rm -Rf *
             cd ..
         else
             echo "!!!!! Repertoire "${toclean_rep}" non trouve : $?"
         fi
         #................. se mettre dans le rep moc et nettoyer .........................
         toclean_rep="moc"
         cd "${toclean_rep}"
         if [ $? == 0 ]; then
             echo "---- nettoyage du Repertoire "${toclean_rep}"---- "
             ls
             rm -Rf *
             cd ..
         else
             echo "!!!!! Repertoire "${toclean_rep}" non trouve : $?"
         fi
         #................. se mettre dans le rep obj et nettoyer .........................
         toclean_rep="obj"
         cd "${toclean_rep}"
         if [ $? == 0 ]; then
             echo "---- nettoyage du Repertoire "${toclean_rep}"---- "
             ls
             rm -Rf *
             cd ..
         else
             echo "!!!!! Repertoire "${toclean_rep}" non trouve : $?"
         fi

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
 echo -e "------------------------------------------------"
 echo -e "Fin commande nettoyage pour projet $Projet"
 echo -e "------------------------------------------------"


