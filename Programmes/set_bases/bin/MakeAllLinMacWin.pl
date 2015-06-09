#!/usr/bin/perl -w

$env = $^O;
if($env eq "mMSwin32"){
$cheminmedintux = "F:/MedinTux-V2.13/Programmes/";
$cheminqmakeqt3 = "F:/Qt/3.3.4/bin/qmake";
$cheminqmakeqt4 = "F:/Qt/4.3.4/bin/qmake";
$cheminmake = "F:/CodeBlocks/MinGW/bin/mingw32-make";
}
#@qmakeqt3project = ("$cheminqmakeqt3","-project");
#@qmakeqt4project = ("$cheminqmakeqt4","-project");
if($env eq "linux"){
$cheminmedintux = "/home/ro/MedinTux-V2.13-Mac-Intel-105-src/Programmes/";
$cheminqmakeqt3 = "/usr/bin/qmake";
$cheminqmakeqt4 = "/usr/bin/qmake-qt4";
$cheminmake = "/usr/bin/make";
}
if($env eq "darwin"){
$cheminmedintux = "/home/pmddeb/Desktop/MedinTux-V2.13/Programmes/";
$cheminqmakeqt3 = "/usr/bin/qmake";
$cheminqmakeqt4 = "/usr/bin/qmake-qt4";
$cheminmake = "/usr/bin/make";
}


@qmakecompta = ($cheminqmakeqt3, $cheminmedintux."comptabilite/compta/compta.pro");
@qmakecheck_dus = ($cheminqmakeqt3,$cheminmedintux."comptabilite/check_dus/check_dus.pro");
@qmakecomptaplug = ($cheminqmakeqt3,$cheminmedintux."comptabilite/compta-plugins/compta-plugins.pro");
@qmakechoix = ($cheminqmakeqt4 ,$cheminmedintux."comptabilite/comptabilite_interfaces_src/choixdialog/choixdialog.pro");
@qmakerecette = ($cheminqmakeqt4 ,$cheminmedintux."comptabilite/comptabilite_interfaces_src/recettedialog/recettedialog.pro");
@qmakedepense = ($cheminqmakeqt4 ,$cheminmedintux."comptabilite/comptabilite_interfaces_src/depensedialog/depensedialog.pro");
@qmakealarmes = ($cheminqmakeqt4 ,$cheminmedintux."pensebetux/Alarmes/src/src.pro");
@qmakelecturealarmes = ($cheminqmakeqt4 ,$cheminmedintux."pensebetux/lecture_table_alarmes/src/src.pro");
@makeclean = ($cheminmake,"clean");
@make = ($cheminmake);
#construction de la liste des make 

@listemake = ("ccamview", "drtux", "gest_user", "personnes", "QLightPad", "QLaboFTP", "Manager", "mise_a_jour", "set_bases", "qgetdatasemp", "qmedicabase", "manu", "med_stat", "cal_obst", "calendrier", "param_view", "comptabilite","pensebetux","sauvegarde","crypt");

for($i=0;$i<@listemake;$i++){

if($listemake[$i] eq "comptabilite"){
#-----------------------------------------------------------------------------------------------

#system(@qmakeqt3project) == 0
#	or die "system @qmakeqt3project erreur:$?";
chdir($cheminmedintux."comptabilite/compta/");
system(@qmakecompta) == 0
	or die "system @qmakecompta erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation compta OK ======================\n";
#------------------------------------------------------------------------------------------------

chdir($cheminmedintux."comptabilite/check_dus/");
system(@qmakecheck_dus) == 0
	or die "system @qmakecheck_dus erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation check_dus OK ======================\n";
#------------------------------------------------------------------------------------------------

chdir($cheminmedintux."comptabilite/compta-plugins/");
system(@qmakecomptaplug) == 0
	or die "system @qmakecomptaplug erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation compta-plugins OK ======================\n";

#compilation en QT4 -----------------------------------------------------------------------------
#choixdialog --------------------------------------------------------------------------------------
chdir($cheminmedintux."comptabilite/comptabilite_interfaces_src/choixdialog/");

system(@qmakechoix) == 0
	or die "system @qmakechoix erreur:$?";

system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation choixdialog OK ======================\n";
#recettedialog -----------------------------------------------------------------------------------------------------
chdir($cheminmedintux."comptabilite/comptabilite_interfaces_src/recettedialog/");
system(@qmakerecette) == 0
	or die "system @qmakerecette erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "compilation recettedialog\n";

#depensedialog------------------------------------------------------------------------------------------------------
chdir($cheminmedintux."comptabilite/comptabilite_interfaces_src/depensedialog/");
system(@qmakedepense) == 0
	or die "system @qmakedepense erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation comptabilite_interfaces_src OK ======================";
}

#compilation sous QT4 ----------------------------------------------------------------------------------------------
#compilation pensebetux ---------------------------------------------------------------------------------------------
elsif ($listemake[$i] eq "pensebetux"){
#compilation Alarmes ---------------------------------------------------------------------------------------------------------
chdir($cheminmedintux."pensebetux/Alarmes/src/");
system(@qmakealarmes) == 0
	or die "system @qmakealarmes erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "compilation Alarmes OK\n";
#compilation lecture_table_alarmes -----------------------------------------------------------------------------------
chdir($cheminmedintux."pensebetux/lecture_table_alarmes/src/");
system(@qmakelecturealarmes) == 0
	or die "system @qmakelecturealarmes erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation lecture_table_alarmes OK ======================\n";
}
#compilation sauvegarde et crypt ------------------------------------------------------------------------------------
elsif( $listemake[$i] eq "sauvegarde" || $listemake[$i] eq "crypt" ){
chdir($cheminmedintux.$listemake[$i]."/src/");
@qmakeqt4 = ($cheminqmakeqt4,$cheminmedintux.$listemake[$i]."/src/src.pro");
system(@qmakeqt4) == 0
	or die "system @qmakeqt4 erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation ".$listemake[$i]." OK ======================\n";
}
#compilation de tous les modules qt3 organisés en module/bin et module/src
else{
chdir($cheminmedintux.$listemake[$i]."/src/");
@qmake = ($cheminqmakeqt3,$cheminmedintux.$listemake[$i]."/src/src.pro");
system(@qmake) == 0
	or die "system @qmake erreur:$?";
system(@makeclean) == 0
	or die "system @make erreur:$?";
system(@make) == 0
	or die "system @make erreur:$?";
print "====================== compilation ".$listemake[$i]." OK ======================\n";
}
}

print "\n---------compilation réussie----------------\n";





