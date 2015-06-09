<?php
/*
Cette page invisible sert juste à modifier le serveur de destination de MedWebTux.
Le plus souvent, la bonne valeur est localhost ou 127.0.0.1
Si votre serveur mysql est sur une autre machine que le serveur apache, vous devez appeler cette page de la manière suivante :
select.php?adr=votre_adresse_du_serveur_mysql&loginbase=login_de_mysql&pwd=motde passe de mysql&login=login de medintux&pass=pass crypte de medintux&ID=GUID patient
Elle va réécrire le fichier server.php, qui ne contient que l'adresse du serveur, le login mysql et le mot de passe de mysql.
ensuite, elle appelle la page verifLogin.php avec les options de login automatique et l'ID du patient.
La page du patient sera alors automatiquement ouverte.
En l'absence d'ID du patient, s'ouvrira la recherche de patient
*/
//login=&pass=&ID
if (isset($_REQUEST['login']))
  $login=$_REQUEST['login'];
else
  $login='';
if (isset($_REQUEST['pass']))
  $pass=$_REQUEST['pass'];
else
  $pass='';
if (isset($_REQUEST['']))
  $ID=$_REQUEST['ID'];
else
  $ID='';
if (isset($_REQUEST['loginbase']))
  $loginbase=$_REQUEST['loginbase'];
else
  $loginbase='';
if (isset($_REQUEST['pwd']))
  $pwd=$_REQUEST['pwd'];
else
  $pwd='';

//on recupere l'adresse en parametre
/*if (is_writable('server.php') or die ('Impossible d\'&eacute;crire dans server.php')) 
{
  if (isset($_REQUEST['adr']))
  {
    $adr=$_REQUEST['adr'];
    $chaine='<?php $host=\''.$adr.'\'; $loginbase=\''.$loginbase.'\'; $pwd=\''.$pwd.'\'; ?>';
  //on ouvre le server.php en lecture ecriture
    if ($fichier=fopen('server.php','w+'))
    {
      fwrite ($fichier,$chaine);
      fclose ($fichier);
    }
*/
  //une fois l'adresse modifiee dans le fichier de server.php, on renvoie sur la page patient
    header ('location:verifLogin.php?login='.$login.'&pass='.$pass.'&ID='.$ID);
    exit;
/*
  }
  else
    echo "Une adresse doit &ecirc;tre donn&eacute;e en argument";
}
*/
?>
