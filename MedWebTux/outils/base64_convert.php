<?php
session_start() ;
//page non affichable convertissant TOUS les documents de MedinTux encodes en base 64 (par exemple après un export par phpMyAdmin) en fichiers lisibles.
//Cette page ne doit être utilisee qu'exceptionnellement et a bon escient, car elle rendrait illisible tous les documents auparavant lisibles sans decodage
include("../config.php");
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion &agrave; $host<br>";
	exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
}
$user=$_SESSION['login'];

if (isset( $_REQUEST['checkbox_valider']) AND isset( $_REQUEST['bouton_valider']))
{
echo "OK"; //on va executer le travail
//les differents blobs disponibles
//DataDocuments dans UserDoc
//FriendUserBlobs dans user_perms
//fchpat_Note_Html dans fchpat_Note
//RbDate_DataRub dans RubriquesBlobs
//les documents
  $sql_chercher_documents="SELECT RbDate_DataRub FROM RubriquesBlobs";
  $resultat_chercher_documents=mysql_query($sql_chercher_documents,$db);
  while ($ligne_chercher_documents=mysql_fetch_array($resultat_chercher_documents))
  {
    $ce_document_base64=$ligne_chercher_documents['RbDate_DataRub'];
    $ce_document_decode=base64_decode($ce_document_base64);
    $sql_corriger_document="UPDATE RubriquesBlobs  RbDate_DataRub=$ce_document WHERE RbDate_DataRub='$ce_document_base64'";
    echo $sql_corriger_document.'<br>';
  }

  //les documents utilisateurs
  $sql_chercher_data_documents="SELECT DataDocuments FROM UserDoc";
  $resultat_chercher_data_documents=mysql_query($sql_chercher_data_documents,$db);
  while ($ligne_chercher_data_documents=mysql_fetch_array($resultat_chercher_data_documents))
  {
    $ce_document_base64=$ligne_chercher_data_documents['DataDocuments'];
    $ce_document_decode=base64_decode($ce_document_base64);
    $sql_corriger_document="UPDATE UserDoc  DataDocuments=$ce_document WHERE DataDocuments='$ce_document_base64'";
    echo $sql_corriger_document.'<br>';
  }
  //les utilisateurs delegues
  $sql_chercher_FriendUserBlobs="SELECT DataDocuments FROM user_perms";
  $resultat_chercher_FriendUserBlobs=mysql_query($sql_chercher_FriendUserBlobs,$db);
  while ($ligne_chercher_FriendUserBlobs=mysql_fetch_array($resultat_chercher_FriendUserBlobs))
  {
    $ce_document_base64=$ligne_chercher_FriendUserBlobs['DataDocuments'];
    $ce_document_decode=base64_decode($ce_document_base64);
    $sql_corriger_document="UPDATE user_perms FriendUserBlobs=$ce_document WHERE FriendUserBlobs='$ce_document_base64'";
    echo $sql_corriger_document.'<br>';
  }
  //les notes
  $sql_chercher_fchpat_Note_Html="SELECT fchpat_Note_Html FROM fchpat_Note";
  $resultat_chercher_fchpat_Note_Html=mysql_query($sql_chercher_fchpat_Note_Html,$db);
  while ($ligne_chercher_fchpat_Note_Htmls=mysql_fetch_array($resultat_chercher_fchpat_Note_Html))
  {
    $ce_document_base64=$ligne_chercher_fchpat_Note_Html['DataDocuments'];
    $ce_document_decode=base64_decode($ce_document_base64);
    $sql_corriger_document="UPDATE fchpat_Notev fchpat_Note_Html=$ce_document WHERE fchpat_Note_Html='$ce_document_base64'";
    echo $sql_corriger_document.'<br>';
  }
}
else
  echo "pas OK";
?>
<html>
<head>
	<META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=UTF-8">
	<META NAME="GENERATOR" CONTENT="PHP">
<title>MedWebTux - Conversion de base64</title>
</head>
<body>
<b>Attention !</b>
Cette page permet de convertir TOUT le fichier DrTuxTest préalablement encodé en base64 (par exemple après un export par phpMyAdmin) en fichier lisible.<br>
Rappel : le base64 est un encodage permettant de transformer en caractères ordinaires choisis parmi 64 caractères affichables différents n'importe quelle donnée binaire.<br>
Si votre fichier est déjà lisible, vous ne devez SURTOUT PAS cliquer sur le bouton.<br>
Si vous êtes certain que TOUTES vos fiches sont encodées en base64, cochez la croix et cliquez ensuite sur le bouton.<br>
Cela corrigera 
<ul>
<li>DataDocuments dans UserDoc
<li>FriendUserBlobs dans user_perms
<li>fchpat_Note_Html dans fchpat_Note
<li>RbDate_DataRub dans RubriquesBlobs
</ul>
<form action="base64_convert.php" method="POST">
Cochez puis validez
<input name="checkbox_valider" type="checkbox" />
<input name="bouton_valider" type=submit value="Valider" />
</form>
</body>
</html>