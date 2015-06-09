<?php
//page non affichable
//creation de vcard a partir des donnees fournies
session_start() ;	
include("config.php");
if ( isset( $_SESSION['login'] ) )
{
  $db=mysqli_connect($host, $loginbase, $pwd);
  $codage=mysqli_query($db,"SET NAMES 'UTF8'");
  if(!$db)
  {
    print "Erreur de connexion &agrave; $db<br>";
    exit;
  }
  // on choisit la bonne base
  if (!mysqli_select_db($db,$base))
  {
    print "Erreur ".mysqli_error($db)."<br>";
    mysqli_close($db);
    exit;
  }
}
else //si pas logue
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=correspondant' );
  exit;
}
//formater une vcard
function vcard($Nom,$Prenom,$Titre,$Qualite,$Adresse,$Ville,$CodePostal,$Tel_1,$EMail,$Note)
{
  $card='
  BEGIN:VCARD
  VERSION:2.1
  N;CHARSET=UTF-8:'.$Nom.';'.$Prenom.';;'.$Titre.'
  FN;CHARSET=UTF-8:'.$Prenom.' '.$Nom.'
  TITLE;CHARSET=UTF-8:'.$Qualite.'
  ADR;WORK;CHARSET=UTF-8:;'.$Adresse.';'.$Ville.';;'.$CodePostal.';
  TEL;WORK:'.$Tel_1.'
  EMAIL;PREF;INTERNET:'.$EMail.'
  NOTE;CHARSET=UTF-8:'.$Note.'
  END:VCARD';
  return $card;
}

if (isset($_REQUEST['vcard']))
{
  if (isset($_GET['exact']))
    $exact=$_GET['exact'];
  else
    $exact=0;
$intervenants_ou_non='';
//&intervenant_user[]=users&intervenant_user[]=no_users
  if (isset($_REQUEST['intervenant_user']))
  {
    $intervenant_user=$_REQUEST['intervenant_user'];
//print_r($intervenant_user);
    $type=" AND (";
    foreach ($intervenant_user AS $cet_intervenant_user)
    {
      if ($type==" AND (") //premier tour de la boucle
      {
	if ($cet_intervenant_user=="users")
	  $type=$type." Login!=''";
	if ($cet_intervenant_user=="no_users")
	  $type=$type." Login =''";
      }
      else
      {
	if ($cet_intervenant_user=="users")
	  $type=$type." OR Login !=''";
	if ($cet_intervenant_user=="no_users")
	  $type=$type." OR Login =''";
      }
      if ($cet_intervenant_user=='users')
      {
	$intervenants_ou_non.='&intervenant_user[]=users';
      }
      if ($cet_intervenant_user=='no_users')
      {
	$intervenants_ou_non.='&intervenant_user[]=no_users';
      }
    }
    $type=$type.")";
  }
  else //deux decoches (aberrant)
  {
    $type=" AND 0";
  }
//Qualite=&critere_recherche=&cle=&exact=&intervenant_user[]=&vcard=Exporter+en+vcard
  $Qualite="";
  $critere_recherche="";
  extract($_POST);

  $critere['Nom']="Nom";
  $critere['Prenom']="Pr&eacute;nom";
  $critere['Adresse']="Adresse";
  $critere['Ville']="Ville";
  $critere['CodePostal']="Code Postal";

  if (isset($cle))
  {
    if ($exact==1)//Recherche par cle exacte des intervenants
      $sql_chercher_personnes=sprintf("SELECT * FROM Personnes WHERE $critere_recherche = '%s' AND Qualite LIKE '$Qualite' $type ORDER BY Qualite,Nom",mysqli_real_escape_string($db,$cle));	
    else // si pas exact
    {
      $cle_courte=$cle;
      $cle="%".$cle."%";//On ajoute les Jokers
      $sql_chercher_personnes=sprintf("SELECT * FROM Personnes WHERE $critere_recherche LIKE '%s' AND Qualite LIKE '$Qualite' $type ORDER BY Qualite,Nom",mysqli_real_escape_string($db,$cle));	
    }

  }
  else
    $cle='';

  $card='';
//chercher les intervenants
  $resultat_chercher_personnes=mysqli_query($db,$sql_chercher_personnes);
  while ($ligne_chercher_personnes=mysqli_fetch_array($resultat_chercher_personnes))
  {
    $card=$card.vcard($ligne_chercher_personnes['Nom'],$ligne_chercher_personnes['Prenom'],$ligne_chercher_personnes['Titre'],$ligne_chercher_personnes['Qualite'],$ligne_chercher_personnes['Adresse'],$ligne_chercher_personnes['Ville'],$ligne_chercher_personnes['CodePostal'],$ligne_chercher_personnes['Tel_1'],$ligne_chercher_personnes['EMail'],$ligne_chercher_personnes['Note']);
  }
}
//on ecrit la vcard finale dans un fichier
if ($fichier=fopen("log/vcard_intervenants.vcf",'w')) //on cree ou on ouvre un fichier de toutes les vcard
{
  fwrite($fichier,$card);
  fclose($fichier);
}
//retourner automatiquement a la page appelante
//Qualite=ALCOOLOGUE&critere_recherche=Nom&cle=&intervenant_user[]=users&intervenant_user[]=no_users&envoyer=Chercher
header ('location:correspondant.php?vcard=yes&envoyer=chercher&Qualite='.$Qualite.'&critere_recherche='.$critere_recherche.'&cle='.$cle.$intervenants_ou_non);
?>