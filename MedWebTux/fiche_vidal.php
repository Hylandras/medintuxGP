<?php
session_start() ;
include("config.php");
$basemed="DatasempTest";

if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=medocs' );
  exit;
}


if (!$db=mysqli_connect($host, $loginbase, $pwd))
{
  print "Erreur de connexion &agrave; ".$host."<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$basemed))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}

/*
t5C
  f0 identifiant ->
    t48.fO -> t48.f1 = code image ->t46.f0 t46.f1=nom image
      t5D.f1 ->f0
	t00.f1
    t5D.f1 
    t5E.f1
  f1 titre
  f3 monographe Vidal
t5D : relations documents-Unités de vente
t5D.f0=t00.f1
t5E Relation documents-produits
*/
/*
function detectUTF8($string) //pour les fiches encodees en utf-8 avec mauvais en-tete - version 198A
{
  return preg_match('%(?:
  [\\xC2-\\xDF][\\x80-\\xBF] # non-overlong 2-byte
  |\\xE0[\\xA0-\\xBF][\\x80-\\xBF] # excluding overlongs
  |[\\xE1-\\xEC\\xEE\\xEF][\\x80-\\xBF]{2} # straight 3-byte
  |\\xED[\\x80-\\x9F][\\x80-\\xBF] # excluding surrogates
  |\\xF0[\\x90-\\xBF][\\x80-\\xBF]{2} # planes 1-3
  |[\\xF1-\\xF3][\\x80-\\xBF]{3} # planes 4-15
  |\\xF4[\\x80-\\x8F][\\x80-\\xBF]{2} # plane 16
  )+%xs', $string);
}
*/

$cuv_medoc=$_GET['cuv_medoc'];

$sql_fiche_vidal="SELECT * FROM t5C INNER JOIN t5D ON t5C.f0=t5D.f1 WHERE t5D.f0='$cuv_medoc'";
$resultat_fiche_vidal=mysqli_query($db,$sql_fiche_vidal);
$ligne_fiche_vidal=mysqli_fetch_array($resultat_fiche_vidal);

/*if (detectUTF8($ligne_fiche_vidal['f3']))
  $rcp= str_replace ('charset=windows-1252','charset=utf-8',$ligne_fiche_vidal['f3']);
else */
  $rcp=$ligne_fiche_vidal['f3'];

echo str_replace ('<?xml version="1.0" encoding="windows-1252"?>','',$rcp);
?>

