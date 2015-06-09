<?php
//page utilisee par AJAX pour chercher les intolerances medicamenteuses a ajouter au terrain
header("Content-Type: text/plain");

$substance = (isset($_GET["substance"])) ? $_GET["substance"] : NULL;
$num = (isset($_GET["num"])) ? $_GET["num"] : NULL; //Le numero du terrain a modifier
$basemed="DatasempTest";

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$basemed))
{
echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />";
echo "<div class=\"erreur\">
<b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
</div>";
	exit;
}

if (strlen($substance) > 2) 
{
  $sql="SELECT f0,f2 FROM t27 WHERE CAST(f2 AS CHAR) LIKE '".mysqli_real_escape_string($db,$substance)."%'"; //f2 = nom, f0 = numero substance
  $resultat=mysqli_query($db,$sql);
  $substances='';
  if ($num_rows=mysqli_num_rows($resultat))
  {
    while ($ligne=mysqli_fetch_array($resultat))
    {
        $substances=$substances.'|'.$ligne['f2'].' code substance '.$ligne['f0'];
    }
    echo $substances."_".$num;
  }
  else
    echo "NOP_".$num; 
}
else
  echo "NOP_".$num; 
?>

