<?php
include("../config.php");
//moulinette destinee a transferer les  donnees afssaps CIS (specialites) vers MedinTux
$db=mysql_connect($host, $loginbase, $pwd); //connexion au serveur
$codage=mysql_query("SET NAMES 'UTF8'");
if(!$db)
{
	print "Erreur de connexion &agrave; $db<br>";
	exit;
}

// on choisit la bonne base
//base medintux
$base_GET="BaseGetTest";
if (!mysql_select_db($base_GET,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}
//base afssaps
$base_afssaps="afssaps";
if (!mysql_select_db($base_afssaps,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<HTML>
<HEAD>
	<META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=UTF-8">
  <META NAME="GENERATOR" CONTENT="PHP">
  <TITLE>MedWebTux - Importation des données AFSSAPS dans la base GET</TITLE>
</head>
<body>
<h1>Importation des fichiers de l'AFSSAPS</h1>
Passer le fichier original en utf-8. Exemple : recode ISO-8859-1..UTF-8 fichier CIS.txt

<p>Beaucoup de réglages de PHP (dans php.ini) peuvent empêcher le chargement du fichier. Citons : 
<ul>
<li>post_max_size
<li>file_uploads = On
<li>upload_max_filesize 
<li>memory_limit
<li>max_execution_time
<li>max_input_time
</ul>
<h2>Importation du fichier CIS (spécialités)</h2>
	  <form align="center" action="afssaps.php" method="POST" ENCTYPE="multipart/form-data">
	    <input type="hidden" name="MAX_FILE_SIZE" value="300000000" />
	    <INPUT type="file" name="selection" />
	    <INPUT NAME="envoyer_CIS" TYPE="submit" VALUE="Importer" title="Ajouter "/>
	  </form>
<?php

if (isset($_FILES['selection']) AND isset ($_REQUEST['envoyer_CIS'])) 
{
  echo "Selection bien reçue<br>"; //OK
  if ($selection = $_FILES['selection']['tmp_name'])
    echo "FILES OK";
  else
    echo "echec de FILES"; //pas bon :-(
  if ($fichier=fopen($selection,"r")) //ouverture du fichier renvoye par l'interface
    echo "fichier OK"; 
  else
    echo "echec fichier";
  $compteur=0;
/*CIP 
VO 
PRINCI 
NOM_COMMER 
DOS 
PRE 
SI 
PRIXLIST 
LISTCONV 
ECARTPRIX 
ECT 
NOUNI 
PRIXUNI 
COD3 
CODE_AT2 
REPERT 
NUSOUSGROU 
NUORDRE 
FLECHE 
PRIX 
NOTE_CIP 
VERSION 
LABORATOIR 
SMRINS 
TFR */
  while (!feof($fichier)) 
  {
    $compteur++;
    echo $compteur;
    $line = stream_get_line($fichier, 65536,"\n"); //analyse des lignes une par une du fichier source
    $split=explode("\t",$line); //separation de la ligne selon les tabulations
    $CIS_cis=$split[0];
    $CIS_nom_comm=$split[1];
    $CIS_forme=$split[2];
    $CIS_voie=$split[3];
    $CIS_amm=$split[4];
    $CIS_procedure=$split[5];
    $CIS_commercialise=$split[6];
    $CIS_cip7=$split[7];

    //	insertion DE LA FICHE
      $sql_inserer_cis=sprintf("INSERT INTO CIS (CIS_cis,CIS_nom_comm,CIS_forme,CIS_voie,CIS_amm,CIS_procedure,CIS_commercialise,CIS_cip7) VALUES ('$CIS_cis','%s','%s','%s','%s','%s','%s','$CIS_cip7')",mysql_real_escape_string($CIS_nom_comm),mysql_real_escape_string($CIS_forme),mysql_real_escape_string($CIS_voie),mysql_real_escape_string($CIS_amm),mysql_real_escape_string($CIS_procedure),mysql_real_escape_string($CIS_commercialise));
      $resultat_inserer_cis=mysql_query($sql_inserer_cis,$db);
      echo $sql_inserer_cis."<br>";
  } //fin while (recuperation des lignes une par une)
fclose($fichier);
} //fin files
else
  echo "pas de fichier fourni";
// base GET
// 	CIP 	VO 	PRINCI 	NOM_COMMER 	DOS 	PRE 	SI 	PRIXLIST 	LISTCONV 	ECARTPRIX 	ECT 	NOUNI 	PRIXUNI 	COD3 	CODE_AT2 	REPERT 	NUSOUSGROU 	NUORDRE 	FLECHE 	PRIX 	NOTE_CIP 	VERSION 	LABORATOIR 	SMRINS 	TFR
?>
<h2>Importation du fichier COMPO (compositions)</h2>
	  <form align="center" action="afssaps.php" method="POST" ENCTYPE="multipart/form-data">
	    <input type="hidden" name="MAX_FILE_SIZE" value="300000000" />
	    <INPUT type="file" name="selection" />
	    <INPUT NAME="envoyer_COMPO" TYPE="submit" VALUE="Importer" title="Ajouter "/>
	  </form>
<?php

if (isset($_FILES['selection']) AND isset ($_REQUEST['envoyer_COMPO'])) 
{
  echo "Selection bien reçue<br>"; //OK
  if ($selection = $_FILES['selection']['tmp_name'])
    echo "FILES OK";
  else
    echo "echec de FILES"; //pas bon :-(
  if ($fichier=fopen($selection,"r")) //ouverture du fichier renvoye par l'interface
    echo "fichier OK"; 
  else
    echo "echec fichier";
  $compteur=0;
  while (!feof($fichier)) 
  {
    $compteur++;
    echo $compteur;
    $line = stream_get_line($fichier, 65536,"\n"); //analyse des lignes une par une du fichier source
    $split=explode("\t",$line); //separation de la ligne selon les tabulations
    $compo_cis=$split[0];
    $compo_forme=$split[1];
    $compo_num=$split[2];
    $compo_compo=$split[3];
    $compo_dose=$split[4];
    $compo_prescription=$split[5];
    $compo_sa=$split[6];
    $compo_cle=$split[7];

    //	insertion DE LA FICHE
      $sql_inserer_compo=sprintf("INSERT INTO compo (compo_cis,compo_forme,compo_num,compo_compo,compo_dose,compo_prescription,compo_sa,compo_cle) VALUES ('$compo_cis','%s','$compo_num','%s','%s','%s','$compo_sa','$compo_cle')",mysql_real_escape_string($compo_forme),mysql_real_escape_string($compo_compo),mysql_real_escape_string($compo_dose),mysql_real_escape_string($compo_prescription));
      $resultat_inserer_compo=mysql_query($sql_inserer_compo,$db);
      echo $sql_inserer_compo."<br>";
  } //fin while (recuperation des lignes une par une)
fclose($fichier);
} //fin files
else
  echo "pas de fichier fourni";
?>
<h2>Importation du fichier CIS_CIP des conditionnements</h2>
	  <form align="center" action="afssaps.php" method="POST" ENCTYPE="multipart/form-data">
	    <input type="hidden" name="MAX_FILE_SIZE" value="300000000" />
	    <INPUT type="file" name="selection" />
	    <INPUT NAME="envoyer_CIS_CIP" TYPE="submit" VALUE="Importer" title="Ajouter "/>
	  </form>
<?php

if (isset($_FILES['selection']) AND isset ($_REQUEST['envoyer_CIS_CIP'])) 
//if (isset($_FILES['selection'])) 
{
  echo "Selection bien reçue<br>"; //OK
  if ($selection = $_FILES['selection']['tmp_name'])
    echo "FILES OK";
  else
    echo "echec de FILES"; //pas bon :-(
  if ($fichier=fopen($selection,"r")) //ouverture du fichier renvoye par l'interface
    echo "fichier OK"; 
  else
    echo "echec fichier";
  $compteur=0;
/*CIS_CIP_cis
CIS_CIP_cip7
CIS_CIP_boite
CIS_CIP_actif
CIS_CIP_declaration
CIS_CIP_cip13
*/
  while (!feof($fichier)) 
  {
    $compteur++;
    echo $compteur;
    $line = stream_get_line($fichier, 65536,"\n"); //analyse des lignes une par une du fichier source
    $split=explode("\t",$line); //separation de la ligne selon les tabulations
print_r($split);
    $CIS_CIP_cis=$split[0];
    $CIS_CIP_cip7=$split[1];
    $CIS_CIP_boite=$split[2];
    $CIS_CIP_actif=$split[3];
    $CIS_CIP_declaration=$split[4];
    $date=$split[5];
    $date_composants=explode("/",$date);
    $CIS_CIP_date=$date_composants[2]."-".$date_composants[1]."-".$date_composants[0]; //date format mysql
    $CIS_CIP_cip13=$split[6];
    //	insertion DE LA FICHE
    $sql_inserer_cis_cip=sprintf("INSERT INTO CIS_CIP (CIS_CIP_cis,CIS_CIP_cip7,CIS_CIP_boite,CIS_CIP_actif,CIS_CIP_declaration,CIS_CIP_date,CIS_CIP_cip13) VALUES ('$CIS_CIP_cis','$CIS_CIP_cip7','%s','%s','%s','$CIS_CIP_date','$CIS_CIP_cip13')",mysql_real_escape_string($CIS_CIP_boite),mysql_real_escape_string($CIS_CIP_actif),mysql_real_escape_string($CIS_CIP_declaration));
    $resultat_inserer_cis_cip=mysql_query($sql_inserer_cis_cip,$db);
    echo $sql_inserer_cis_cip."<br>";
  }
}
?>

<h2>Injection de la base CIS_CIP dans GET</h2>
Pernser à retirer les "Déclaration d'arrêt de commercialisation" avant d'injecter
<form action="afssaps.php" method="GET">
<input name="bouton_ajouter_cis_cip_dans_get" value="Envoyer CIS_CIP" type="submit" />
</form>
<?php
if (isset ($_REQUEST['bouton_ajouter_cis_cip_dans_get']))
{
/*echo "bouton OK";
  if ($connect=mysql_select_db($base_afssaps))
echo "connect OK";
else
echo "connect pas OK";
*/
  $sql_chercher_cis_cip="SELECT * FROM CIS_CIP";
  $resultat_chercher_cis_cip=mysql_query($sql_chercher_cis_cip);
$count_chercher_cis_cip =mysql_num_rows($resultat_chercher_cis_cip);
echo "nombre=".$count_chercher_cis_cip."<br>";
  $connect=mysql_select_db($base_GET);
  while ($ligne_chercher_cis_cip=mysql_fetch_array($resultat_chercher_cis_cip))
  {
    $cip7=$ligne_chercher_cis_cip['CIS_CIP_cip7'];
    $boite=$ligne_chercher_cis_cip['CIS_CIP_boite'];
    $sql_inserer_cis_cip="INSERT INTO get (CIP,PRE) VALUES ('$cip7','$boite')";
echo $sql_inserer_cis_cip."<br>";
    $resultat_inserer_cis_cip=mysql_query($sql_inserer_cis_cip);
  }
}

?>
<h2>Injection de la base CIS dans GET</h2>
Penser à retirer les Non commercialisés avant d'injecter
<form action="afssaps.php" method="GET">
<input name="bouton_ajouter_cis_dans_get" value="Envoyer CIS" type="submit" />
</form>
<?php
if (isset ($_REQUEST['bouton_ajouter_cis_dans_get']))
{
/*echo "bouton OK";
  if ($connect=mysql_select_db($base_afssaps))
echo "connect OK";
else
echo "connect pas OK";
*/
//  $sql_chercher_cis="SELECT * FROM CIS INNER JOIN CIS_CIP ON CIS.CIS_cis=CIS_CIP.CIS_CIP_cis";
//  $sql_chercher_cis="SELECT * FROM CIS_CIP INNER JOIN CIS ON CIS_CIP.CIS_CIP_cis=CIS.CIS_cis"; //marche pas
  $sql_chercher_cis="SELECT * FROM  CIS "; //marche
  $resultat_chercher_cis=mysql_query($sql_chercher_cis);
  $count_chercher_cis =mysql_num_rows($resultat_chercher_cis);
  echo "nombre=".$count_chercher_cis."<br>";

  while ($ligne_chercher_cis=mysql_fetch_array($resultat_chercher_cis))
  {
    $connect=mysql_select_db($base_afssaps);
    $cis=$ligne_chercher_cis['CIS_cis'];
    $sql_chercher_cip="SELECT CIS_CIP_cip7 FROM CIS_CIP WHERE CIS_CIP_cis='$cis'";
    $resultat_chercher_cip=mysql_query($sql_chercher_cip);
    $ligne_chercher_cip=mysql_fetch_array($resultat_chercher_cip);
    $cip7=$ligne_chercher_cip['CIS_CIP_cip7'];//CIP

    $CIS_nom_comm=$ligne_chercher_cis['CIS_nom_comm']; //NOM_COMMER
    $CIS_voie=$ligne_chercher_cis['CIS_voie'];// ->VO
    
    $connect=mysql_select_db($base_GET);
    $sql_inserer_cis="UPDATE get SET NOM_COMMER='$CIS_nom_comm', VO='$CIS_voie' WHERE CIP='$cip7'";
echo $sql_inserer_cis."<br>";
    $resultat_inserer_cis=mysql_query($sql_inserer_cis);
  }
}

?><h2>Injection de la base Compo dans GET</h2>
<form action="afssaps.php" method="GET">
<input name="bouton_ajouter_compo_dans_get" value="Envoyer compo" type="submit" />
</form>
<?php
if (isset ($_REQUEST['bouton_ajouter_compo_dans_get']))
{
/*echo "bouton OK";
  if ($connect=mysql_select_db($base_afssaps))
echo "connect OK";
else
echo "connect pas OK";
*/
//  $sql_chercher_cis="SELECT * FROM CIS INNER JOIN CIS_CIP ON CIS.CIS_cis=CIS_CIP.CIS_CIP_cis";
//  $sql_chercher_cis="SELECT * FROM CIS_CIP INNER JOIN CIS ON CIS_CIP.CIS_CIP_cis=CIS.CIS_cis"; //marche pas
  $sql_chercher_compo="SELECT * FROM  compo "; //marche
  $resultat_chercher_compo=mysql_query($sql_chercher_compo);
  $count_chercher_compo =mysql_num_rows($resultat_chercher_compo);
  echo "nombre=".$count_chercher_compo."<br>";

  while ($ligne_chercher_compo=mysql_fetch_array($resultat_chercher_compo))
  {
    $connect=mysql_select_db($base_afssaps);
    $cis=$ligne_chercher_compo['compo_cis'];
    $sql_chercher_cip="SELECT CIS_CIP_cip7 FROM CIS_CIP WHERE CIS_CIP_cis='$cis'";
    $resultat_chercher_cip=mysql_query($sql_chercher_cip);
    $ligne_chercher_cip=mysql_fetch_array($resultat_chercher_cip);
    $cip7=$ligne_chercher_cip['CIS_CIP_cip7'];//CIP

    $compo_compo=$ligne_chercher_compo['compo_compo']; //NOM_COMMER
    $compo_dose=$ligne_chercher_compo['compo_dose'];// ->VO
    
    $connect=mysql_select_db($base_GET);
    $sql_inserer_compo=sprintf("UPDATE get SET PRINCI=CONCAT_WS('+',PRINCI,'%s'), DOS=CONCAT_WS('+',DOS,'%s') WHERE CIP='$cip7'",mysql_real_escape_string($compo_compo),mysql_real_escape_string($compo_dose));
echo $sql_inserer_compo."<br>";
    $resultat_inserer_compo=mysql_query($sql_inserer_compo);
  }
}

?>
</body>
