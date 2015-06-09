<?php
session_start() ;
//page invisible d'insertion d'image envoyee en tant que fichier dans le document en reference.
//renvoie a la page consultation.php
include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$signuser=str_replace(" ","",$tab_login[1]); //pour chrome

if(!$db)
{
  print "Erreur de connexion &agrave; $host<br>";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}
if (isset($_FILES['selection'])) 
{
  if ($selection = $_FILES['selection']['tmp_name'])
    echo "";
  else
    echo "echec de FILES<br />"; 
  if ($fichier=fopen($selection,"r")) //ouverture du fichier renvoye par l'interface
    echo ""; 
  else
    echo "echec fichier<br />";

  $guid=$_REQUEST['guid'];
  $filename_components=explode('.',$_FILES['selection']['name']);
  $contents=fread( $fichier,filesize($selection));
  $img=base64_encode($contents);
  $size=getimagesize($selection);
 
  $obs='<?xml version="1.0" encoding="ISO-8859-1" standalone="yes" ?>
<MedinTux_Multimedia_data>
<HTML_Data>
<html><head><meta name="qrichtext" content="1" /></head><body style="font-size:10pt;font-family:Arial">
<p style="text-align:center"><span style="font-family:Sans">
<img height="'.$size[1].'" src="image.'.$filename_components[1].'" width="'.$size[0].'" /></span>
</body></html>
</HTML_Data>
<ImageListe>
<name>
image.'.$filename_components[1].' 
</name>
<fileName>
</fileName>
<ext_Type>
'.$filename_components[1].'
</ext_Type>
<base64>
'.$img.'
</base64>
</ImageListe>
</MedinTux_Multimedia_data>
';
  $sql_insert_obs=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) values ('%s','$guid')",mysqli_real_escape_string($db,$obs));
  $resultat_insert_obs=mysqli_query($db,$sql_insert_obs);

  $heure=date('H:i:s', date('U')); // la date du jour
//pour convertir les formats de dates
function local_to_iso($date,$date_format)
{
  $list_date=explode ("-",$date);
  if ($date_format=='fr')
  {
    $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
  }
  elseif ($date_format=='en')
    $date=$list_date[2].'-'.$list_date[0].'-'.$list_date[1];
  return $date;
}

$date=local_to_iso($_REQUEST['date'],$date_format);
$date=$date.' '.$heure;
$title=$_REQUEST['title'];
  $id_blob=mysqli_insert_id($db);
//on insere l'en-tete
  $sql_insert_header="INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$guid','20080000','$title','$date','$user','$signuser','$id_blob')";
  $resultat_insert_header=mysqli_query($db,$sql_insert_header);
}

header ('location: consultation.php?numeroID='.$guid.'&date='.$date );
?>