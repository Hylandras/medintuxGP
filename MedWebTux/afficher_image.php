<?php
/*
Fichier qui est appelé par les modules qui ont beson de décoder des images en base64.
On recoit le numero du document a convertir et le numero de l'image dans le document, on convertit l'image base 64 et on la renvoie en tant qu'image
*/
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=nouveau_dossier' );
  exit;
}
include("config.php");
error_reporting(-1);

$db=mysqli_connect($host, $loginbase, $pwd);

if(!$db)
{
  print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br>";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}

if (isset($_REQUEST['RbDate_PrimKey']))
  $RbDate_PrimKey =$_REQUEST['RbDate_PrimKey'];
if (isset($_REQUEST['compteur_image']))
  $compteur_image =$_REQUEST['compteur_image'];
  
header('Content-type: image/png');

$sql_recherche_doc="SELECT RbDate_DataRub FROM RubriquesBlobs WHERE RbDate_PrimKey  = '$RbDate_PrimKey'";
$resultat_recherche_doc=mysqli_query($db,$sql_recherche_doc);
$ligne_recherche_doc=mysqli_fetch_array($resultat_recherche_doc); //un seul document pour cet id
$ce_doc=$ligne_recherche_doc["RbDate_DataRub"] ; 
$tableau_lignes_texte=explode("\n",$ce_doc);//on explose ce document aux retours chariot
$compteur_base64=0;
$afficher_ligne=0;

foreach ($tableau_lignes_texte AS $cette_ligne)
{
  if ($afficher_ligne==1)
  {
    echo base64_decode($cette_ligne); //l'image brut, renvoyee a la page appelante
  }
  if (preg_match('`<base64>`',$cette_ligne) AND $compteur_image==$compteur_base64++) // on recherche debut balise image dans le document
  {
    $afficher_ligne=1;
    $compteur_base64++;
  }
  if (preg_match('`</base64>`',$cette_ligne)) // on recherche fin balise image dans le document
  {
    $afficher_ligne=0;
  }
}	
?>
