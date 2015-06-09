<?php

header("Content-Type: text/xml");
echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
echo "<list>";

$idEditor = (isset($_POST["IdEditor"])) ? htmlentities($_POST["IdEditor"]) : NULL;
//$idEditor ="6BE6AE4A-577E-3E4C-8C18-EEB2D55D6CDE";
include("config.php");
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

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

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

if ($idEditor) {
	
$query = mysql_query("SELECT * FROM Personnes WHERE GUID='" . mysql_real_escape_string($idEditor) ."'");
	while ($back = mysql_fetch_assoc($query)) {
		echo "<item id=\"" . $back["GUID"] . "\" name=\"" . $back["GUID"] . "\" />";
	}
}
echo "</list>";

?>

