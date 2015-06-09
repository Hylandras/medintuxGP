#!/bin/bash

#----------------------------- display_help ------------------------------------------------
display_help()
{   echo -e "-------------------------------- set_basesMac.sh -----------------------------"
    echo -e "Debut commande set_basesMac pour projet : $Projet"
    echo -e "Ce module doit s'executer a partir du repertoire ou se trouve set_base.app"
    echo -e "Le mode administrateur est requis"
    echo -e "cela permet de lancer directement set_bases"
    echo -e "pour que le mode compilation soit operationnel"
    echo -e "------------------------------------------------------------------------------"
}

#----------------------------- check_root_AIA ------------------------------------------------
check_root_AIA()
{
 if [ `id -u` -ne 0 ]; then
  echo "------------------------------------------------------------------------------"
  echo "******************** Error: need to run as root! Aborting... *****************"
  display_help
  exit 1
 fi
}

#----------------------------- main  ------------------------------------------------
Projet=Medintux
check_root_AIA
#........... param 1 nom du MODULE a traiter .................
MODULE=$1
export MODULE

if [ $MODULE == '--help' ];then
 display_help
 exit
fi
cd set_bases.app/Contents/MacOS    
exec ./set_bases
echo -e "---------------------------------"
echo -e "Fin commande set_basesMac pour projet : $Projet"
echo -e "---------------------------------"


