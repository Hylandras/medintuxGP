#!/bin/bash

# à lancer en superutilisateur sudo ./installShortCut.sh

 if [[ -z "$1" ]];then
   workFolder=$(readlink -f $(dirname $0))
 else
   workFolder=$1
 fi

echo "---------- installShortCut from : $workFolder ----------------------------"
user_log2=$LOGNAME
echo "user 2 : (installShortCut.sh) "$user_log2
 
# cp $workFolder/Icones/ubuntu_raccourcis/template.txt ~/.local/share/applications/personnes.desktop

# ---- template.txt ----------------------
# Name=::Title
# Icon=::Icone
# Path=::Path
# Exec=::Exec
# StartupNotify=false
# StartupWMClass=::ClassName
# OnlyShowIn=Unity;
# X-UnityGenerated=true
# Name[fr_FR]=::NameFr

#-------------------------------- medintux -----------------------------------
rm -f $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt
rm -f $workFolder/Icones/ubuntu_raccourcis/Demarrer_medintux.txt
rm -f ~/Bureau/Demarrer_medintux.desktop
rm -f ~/.local/share/applications/Demarrer_medintux.desktop

cp $workFolder/Icones/ubuntu_raccourcis/template.txt $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt	

# awk [-F] [-v var=valeur] 'programme' fichier
# gsub(/a/,"ai",oi") Remplace la chaine oi par ai
titre_1="Demarrer_medintux"
icone_1="$workFolder/Icones/MedinTux.png"
path_1="$workFolder/Programmes/Doc/Start_MedinTux"
exec_1="$workFolder/Programmes/Doc/Start_MedinTux/start_medintux.sh"
classname_1=Start_MedinTux
namefr_1=Demarrer_medintux
awk -v titre=$titre_1 -v icone=$icone_1 -v path=$path_1 -v exec=$exec_1 -v classname=$classname_1 -v namefr=$namefr_1 '
{
  gsub( "#Title", titre)
  gsub( "#Icone", icone)
  gsub( "#Path", path)
  gsub( "#Exec", exec)
  gsub( "#ClassName", classname)
  gsub( "#NameFr", namefr)
  print
}' $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt > $workFolder/Icones/ubuntu_raccourcis/Demarrer_medintux.txt
cp $workFolder/Icones/ubuntu_raccourcis/Demarrer_medintux.txt ~/.local/share/applications/Demarrer_medintux.desktop
chmod +x ~/.local/share/applications/Demarrer_medintux.desktop
chown $LOGNAME ~/.local/share/applications/Demarrer_medintux.desktop
cp -a ~/.local/share/applications/Demarrer_medintux.desktop ~/Bureau
sudo chown $LOGNAME ~/Bureau/Demarrer_medintux.desktop

#-------------------------------- personnes -----------------------------------
rm -f $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt
rm -f $workFolder/Icones/ubuntu_raccourcis/personnes.txt
rm -f ~/Bureau/personnes.desktop
rm -f ~/.local/share/applications/personnes.desktop

cp $workFolder/Icones/ubuntu_raccourcis/template.txt $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt	

# awk [-F] [-v var=valeur] 'programme' fichier
# gsub(/a/,"ai",oi") Remplace la chaine oi par ai
titre_1="Annuaire"
icone_1="$workFolder/Icones/PersonnesTel.png"
path_1="$workFolder/Programmes/personnes/bin"
exec_1="$workFolder/Programmes/personnes/bin/personnes"
classname_1=ClassAnnuaire
namefr_1=Annuaire
awk -v titre=$titre_1 -v icone=$icone_1 -v path=$path_1 -v exec=$exec_1 -v classname=$classname_1 -v namefr=$namefr_1 '
{
  gsub( "#Title", titre)
  gsub( "#Icone", icone)
  gsub( "#Path", path)
  gsub( "#Exec", exec)
  gsub( "#ClassName", classname)
  gsub( "#NameFr", namefr)
  print
}' $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt > $workFolder/Icones/ubuntu_raccourcis/personnes.txt
cp $workFolder/Icones/ubuntu_raccourcis/personnes.txt ~/.local/share/applications/personnes.desktop
chmod +x ~/.local/share/applications/personnes.desktop
chown $LOGNAME ~/.local/share/applications/personnes.desktop
cp -a ~/.local/share/applications/personnes.desktop ~/Bureau
sudo chown $LOGNAME ~/Bureau/personnes.desktop

#-------------------------------- Manager -----------------------------------
rm -f $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt
rm -f $workFolder/Icones/ubuntu_raccourcis/Manager.txt
rm -f ~/Bureau/Manager.desktop
rm -f ~/.local/share/applications/Manager.desktop

cp $workFolder/Icones/ubuntu_raccourcis/template.txt $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt	

# awk [-F] [-v var=valeur] 'programme' fichier
# gsub(/a/,"ai",oi") Remplace la chaine oi par ai
titre_1="Manager"
icone_1="$workFolder/Icones/Manager.png"
path_1="$workFolder/Programmes/Manager/bin"
exec_1="$workFolder/Programmes/Manager/bin/Manager"
classname_1=ClassManager
namefr_1=Manager
awk -v titre=$titre_1 -v icone=$icone_1 -v path=$path_1 -v exec=$exec_1 -v classname=$classname_1 -v namefr=$namefr_1 '
{
  gsub( "#Title", titre)
  gsub( "#Icone", icone)
  gsub( "#Path", path)
  gsub( "#Exec", exec)
  gsub( "#ClassName", classname)
  gsub( "#NameFr", namefr)
  print
}' $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt > $workFolder/Icones/ubuntu_raccourcis/Manager.txt
cp $workFolder/Icones/ubuntu_raccourcis/Manager.txt ~/.local/share/applications/Manager.desktop
chmod +x ~/.local/share/applications/Manager.desktop
sudo chown $LOGNAME ~/.local/share/applications/Manager.desktop
cp -a ~/.local/share/applications/Manager.desktop ~/Bureau
sudo chown $LOGNAME ~/Bureau/Manager.desktop

#-------------------------------- set_bases -----------------------------------
rm -f $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt
rm -f $workFolder/Icones/ubuntu_raccourcis/set_bases.txt
rm -f ~/Bureau/set_bases.desktop
rm -f ~/.local/share/applications/set_bases.desktop

cp $workFolder/Icones/ubuntu_raccourcis/template.txt $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt	

# awk [-F] [-v var=valeur] 'programme' fichier
# gsub(/a/,"ai",oi") Remplace la chaine oi par ai
titre_1="Set_bases"
icone_1="$workFolder/Icones/set_bases.png"
path_1="$workFolder/Programmes/set_bases/bin"
exec_1="$workFolder/Programmes/set_bases/bin/set_bases"
classname_1=ClassSet_bases
namefr_1=Configuration_de_MedinTux
awk -v titre=$titre_1 -v icone=$icone_1 -v path=$path_1 -v exec=$exec_1 -v classname=$classname_1 -v namefr=$namefr_1 '
{
  gsub( "#Title", titre)
  gsub( "#Icone", icone)
  gsub( "#Path", path)
  gsub( "#Exec", exec)
  gsub( "#ClassName", classname)
  gsub( "#NameFr", namefr)
  print
}' $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt > $workFolder/Icones/ubuntu_raccourcis/set_bases.txt
cp $workFolder/Icones/ubuntu_raccourcis/set_bases.txt ~/.local/share/applications/set_bases.desktop
chmod +x ~/.local/share/applications/set_bases.desktop
sudo chown $LOGNAME ~/.local/share/applications/set_bases.desktop
cp -a ~/.local/share/applications/set_bases.desktop ~/Bureau
sudo chown $LOGNAME ~/Bureau/set_bases.desktop

#-------------------------------- APropos -----------------------------------
rm -f $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt
rm -f $workFolder/Icones/ubuntu_raccourcis/APropos.txt
rm -f ~/Bureau/APropos.desktop
rm -f ~/.local/share/applications/APropos.desktop

cp $workFolder/Icones/ubuntu_raccourcis/template.txt $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt	

# awk [-F] [-v var=valeur] 'programme' fichier
# gsub(/a/,"ai",oi") Remplace la chaine oi par ai
titre_1="APropos"
icone_1="$workFolder/Icones/APropos.png"
path_1="$workFolder/Programmes/APropos/bin"
exec_1="$workFolder/Programmes/APropos/bin/APropos"
classname_1=ClassAPropos
namefr_1=Aide_de_MedinTux
awk -v titre=$titre_1 -v icone=$icone_1 -v path=$path_1 -v exec=$exec_1 -v classname=$classname_1 -v namefr=$namefr_1 '
{
  gsub( "#Title", titre)
  gsub( "#Icone", icone)
  gsub( "#Path", path)
  gsub( "#Exec", exec)
  gsub( "#ClassName", classname)
  gsub( "#NameFr", namefr)
  print
}' $workFolder/Icones/ubuntu_raccourcis/templateTmp.txt > $workFolder/Icones/ubuntu_raccourcis/APropos.txt
cp $workFolder/Icones/ubuntu_raccourcis/APropos.txt ~/.local/share/applications/APropos.desktop
chmod +x ~/.local/share/applications/APropos.desktop
sudo chown $LOGNAME ~/.local/share/applications/APropos.desktop
cp -a ~/.local/share/applications/APropos.desktop ~/Bureau
sudo chown $LOGNAME ~/Bureau/APropos.desktop


