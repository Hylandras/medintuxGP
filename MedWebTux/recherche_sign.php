<?php
//page utilsée par AJAX pour chercher les utilisateurs signataires
header("Content-Type: text/plain");

$idUser = (isset($_GET["User"])) ? $_GET["User"] : NULL;

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$base))
{
echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />";
echo "<div class=\"erreur\">
<b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
<a href=\"setup/index.php\">Renseigner les donn&eacute;es de connexion au serveur Mysql</a>
</div>";
	@rename("set_up", "setup");
	exit;
}

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}
//rechercher ici si l'utilisateur est signataire pour ne pas le mettre si non signataire =sgn
if ($idUser) 
{
  $sql="SELECT * FROM Personnes WHERE Login='". mysqli_real_escape_string($db,$idUser)."'";
  $resultat=mysqli_query($db,$sql);
  $ligne=mysqli_fetch_array($resultat);
  if ($num_rows=mysqli_num_rows($resultat))
  {
    if (preg_match('/sgn/',$ligne['Droits']))
    {
      $users=$idUser;
    }
    else
      $users='';
    $sql_utilisateur_delegue="SELECT * FROM Personnes INNER JOIN user_perms ON Personnes.GUID=user_perms.FriendUserGUID WHERE Login='". mysqli_real_escape_string($db,$idUser)."'";
    $resultat_utilisateur_delegue=mysqli_query($db,$sql_utilisateur_delegue);
    while ($ligne_utilisateur_delegue=mysqli_fetch_array($resultat_utilisateur_delegue))
    {
      $signataire=$ligne_utilisateur_delegue['SignataireGUID'];
      $sql_utilisateur_signataire="SELECT * FROM Personnes WHERE GUID='$signataire'";
      $resultat_utilisateur_signataire=mysqli_query($db,$sql_utilisateur_signataire);
      $ligne_utilisateur_signataire=mysqli_fetch_array($resultat_utilisateur_signataire);
      if ($users)
	$users=$users.'|'.$ligne_utilisateur_signataire['Login'];
      else
	$users=$ligne_utilisateur_signataire['Login']; //pas de premier tour si utilisateur non signataire
    }
    echo $users;
  }
  else
    echo "NOP"; 
}
?>

