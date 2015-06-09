#!/bin/bash

File_Err=./Err.Mt
Projet=Medintux
MYSQL_LIB=libmysqlclient.18.dylib
Os=`uname`

#-------------------------------------------- error-------------------------------------------------
# Routine d'affichage Erreur
# Sign : String --> nil
#        Erreur --> nil
error()
{
        echo "		$1 "
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
#............. "2  eme arguement est celui indiquant le repertoire Qt .................
 if [[ -z "$2" ]];then
   export QTDIR
   QMAKE=$QTDIR/bin/qmake
 else
   QTDIR=$2
   export QTDIR
   QMAKE=$QTDIR/bin/qmake
 fi
#............. "3  eme arguement est celui indiquant si il faut nettoyer avant .................
#                  si egal a MAKE_ONLY alors pas de nettoyage avant compilation
if [[ -z "$3" ]];then
  COMPIL_MODE=MAKE_CLEAN_AND_ALL
else
  COMPIL_MODE=$3
fi

#............. "4  eme arguement est celui indiquant si mode DEBUG .................

 if [[ -z "$4" ]];then
  DEBUG_MODE=RELEASE
 else
  DEBUG_MODE=$4
 fi

 if [ $Os == 'Darwin' ]; then
       export QMAKESPEC=$QTDIR/mkspecs/macx-g++
       PATH=$PATH:$QTDIR/bin
       export PATH
       DYLD_LIBRARY_PATH=$QTDIR/lib:$DYLD_LIBRARY_PATH
       DYLD_LIBRARY_PATH=$QTDIR/plugins/sqldrivers:$DYLD_LIBRARY_PATH
       DYLD_LIBRARY_PATH=/usr/local/mysql/lib:$DYLD_LIBRARY_PATH
       INCLUDE=/usr/local/mysql/include:$INCLUDE
       QMAKESPEC=$QTDIR/mkspecs/macx-g++
       export QMAKESPEC
       export DYLD_LIBRARY_PATH
       export INCLUDE
 else
    if [ $Os == 'Linux' ]; then
       export QMAKESPEC=$QTDIR/mkspecs/linux-g++
    fi
 fi

# Debut
# clear
#  echo -e "----------- Compilation en cours sur : $Os -----------------"
#  echo -e "Arg 1 (module name)      : $CIBLE"
#  echo -e "Arg 2 (Qt directory)     : $2"
#  echo -e "Arg 3 (compilation mode) : $3"
#  echo -e "Arg 4 (debug mode)       : $4"
#  echo -e "QTDIR                    : $QTDIR"
#  echo -e "QMAKESPEC                : $QMAKESPEC"
 echo -e "-------------------------------------------------"
 echo -e "Debut commande MakeAllMac pour projet $Projet"
 echo -e "-------------------------------------------------"

 if [[ -z "$CIBLE" ]];then
        LISTE="ccamview drtux gest_user personnes QLightPad Manager guinch mise_a_jour set_bases comptabilite check_dus compta-plugins qgetdatasemp qmedicabase med_stat calendrier param_view MrPropre qgetdatasemp"
 else
        LISTE=$CIBLE
 fi
 cd ../..
 for module in $LISTE
 do
         echo -e "========================== compilation en cours de :  $module  sur : $Os ======================="
#         echo -e "----------- Compilation en cours sur : $Os -----------------"
         echo -e "Arg 1 (module name)      : $module"
         echo -e "Arg 2 (Qt directory)     : $2"
         echo -e "Arg 3 (compilation mode) : $3"
         echo -e "Arg 4 (debug mode)       : $4"
         echo -e "QTDIR                    : $QTDIR"
         echo -e "QMAKESPEC                : $QMAKESPEC"
         echo -e "-------------------------------------------------"

         # echo -en "      - Patch executable Mac lib QT pour $module"
         # echo -e " "
         rep_Module=$module
         if [ $module == 'compta-plugins' ]; then
             rep_Module=comptabilite
         else
              if [ $module == 'check_dus' ]; then
                  rep_Module=comptabilite
              else
                  if [ $module == 'guinch' ]; then
                       rep_Module=Manager
                  fi
              fi
         fi
         cd "$rep_Module"
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

         cd "$src_rep"
         # ......................... on fait le qmake (soit sur src.pro si existe soit sur  $module.pro  si existe) ...................
         if [ $COMPIL_MODE != 'MAKE_ONLY' ];then
            echo -e "..... on efface les makefiles ......."
            rm 'Makefile'
            rm 'makefile'
         fi
         if test -f src.pro; then
             $QMAKE src.pro
             echo -e "..... on fait $QMAKE src.pro ......."
         else 
             $QMAKE $module.pro 
             echo -e "..... on fait $QMAKE $module.pro ......."
         fi
         
         if [ $COMPIL_MODE != 'MAKE_ONLY' ];then
            echo -e "..... on fait le menage (make clean) des .O et autres fichiers intermediaires avant le make ......."
            make clean
         fi
         make
         # ......................... si troisieme argument (DEBUG) absent on stripe ...................
         if [ $DEBUG_MODE != 'DEBUG' ];then
            if [ $Os == 'Darwin' ]; then
                echo -e "..... on stripe un peu : ../bin/$module.app/Contents/MacOS/$module..... "
                strip ../bin/$module.app/Contents/MacOS/$module    # pas d'option -s sous mac
            else
                echo -e "..... on stripe un peu : ../bin/$module..... "
                strip -s ../bin/$module
                make install
            fi
         else
            echo -e "..... on ne stripe pas car DEBUG mode ....."
         fi

         cd ..
         # ......................... QUE POUR MAC  patcher pour les infos sur les libs ...................
         if [ $Os == 'Darwin' ]; then
             filestrip=./bin/$module.app/Contents/MacOS/$module
             # QTlib
             # install_name_tool -change libqt-mt.3.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.dylib $filestrip
             install_name_tool -change @executable_path/../../../../Qt/lib/libqt-mt.3.3.8.dylib @executable_path/../../../../../Qt/lib/libqt-mt.3.3.8.dylib $filestrip
             # Mysqllib
             # install_name_tool -change /usr/local/mysql/lib/$MYSQL_LIB @executable_path/../../../../../Qt/lib/$MYSQL_LIB $filestrip
         fi
         if [ $? == 0 ];then
                 echo -e " OK fin"
         else
                 error "Erreur : $? Patch sur $filestrip"
         fi
         cd ..
 done
 echo -e "--------------------------------------------------"
 echo -e "Fin commande MakeAllMac pour $Projet du module : ../bin/$module "
 echo -e "--------------------------------------------------"


