#!/bin/bash

Projet=Medintux

# Debut  
# clear
echo -e "---------------- Lancement de set_bases en mode console ----------------------"
echo -e "                 pour pouvoir utiliser la  compilation"
echo -e "                 penser a se mettre sous un compte root"
if [ "$UID" -ne "0" ]
then
   echo "                 ATTENTION ce compte n'est pas root    "
   #exit 1
fi

exec set_bases.app/Contents/MacOS/set_bases
 


