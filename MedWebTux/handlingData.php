<?php
//page utilsée par AJAX pour verifier si un nouveau login ne doublonne pas avec un existant
header("Content-Type: text/plain");

$idUser = (isset($_GET["User"])) ? $_GET["User"] : NULL;

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "<center><b>Erreur de connexion &agrave; la base
de donn&eacute;es sur ".$host."<br /><br /></b></center> Veuillez vous
assurer que la base de donn&eacute;es est active sur le serveur, que
vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est
pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de
passe d'administration de la base de donn&eacute;es sont bien
renseign&eacute;s dans config.php.<br />";
exit;
}

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}

if ($idUser) 
{
  $sql="SELECT * FROM Personnes WHERE Login='". mysqli_real_escape_string($idUser)."'";
  $resultat=mysqli_query($db,$sql);
  if ($num_rows=mysqli_num_rows($resultat))
  {
    echo "L'utilisateur ".$idUser." existe déjà. Choisissez un autre identifiant"; 
  }
  else
    echo "OK"; 
}
?>

