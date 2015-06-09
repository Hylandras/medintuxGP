<?php
session_start() ;
//page non affichable pour effectuer les effacements de rendez-vous
if ( !isset( $_SESSION['login'] ) ) 
{
	header('location: index.php?page=agenda' );
	exit;
}
//$user=$_SESSION['login'];
include("config.php");
//if (isset )
//	$date=date('Y-m-d', date('U'));

$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");


if(!$db)
{
	print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br>";
	exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

//récupération des valeurs de l'URL
/*
if (isset($_GET['utilisateur_autorisant']) )
{
	$user=$_GET['utilisateur_autorisant'];
}
*/
if (isset($_GET['confirmer']))
	$confirmer=$_GET['confirmer'];
else
	$confirmer="";

//url pour coches suppression en serie
// coche[]=6583&coche[]=6584&confirmer=Supprimer&button_supprimer=Supprimer
if (isset ($_REQUEST['debut'])) //MODE SUPPRESSION MUTIPLE
{
  $debut=$_REQUEST['debut'];
  $fin=$_REQUEST['fin'];
}
//MODE SUPPRESSION MUTIPLE
if ($confirmer=="Supprimer") //coche de confirmation
{
  if (isset ($_REQUEST['coche'])) //coches
  {
    $coche=$_REQUEST['coche'];
    foreach ($coche AS $id_rdv )
    {
      $sqlsuppr="delete from agenda where PrimKey='$id_rdv'";
      $resultatsuppr=mysql_query($sqlsuppr,$db);
    //echo $sqlsuppr;
    }
  }
  header('location: agenda.php?debut='.$debut.'&fin='.$fin.'&envoyer=Chercher&nom=%');
  exit;
}
else //mode coches sans confirmation
{
	header('location: agenda.php?debut='.$debut.'&fin='.$fin.'&envoyer=Chercher&nom=%');
	exit;
}
?>