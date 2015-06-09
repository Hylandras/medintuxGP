<?php
session_start() ;
/*if ( !isset( $_SESSION['login'] ) ) 
{
	header('location: index.php?page=drc' )
	exit;
}
*/
include("config.php");
$base="DRC_2005_11";
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion au serveur de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br />";
	exit;
}
if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}
if (isset($_REQUEST['classe_rc']))
  $classe_rc=$_REQUEST['classe_rc'];
else
  $classe_rc="";
//echo "critere=".$critere_principal;
$sql_REF_CLASSRC="SELECT * FROM REF_CLASSRC";
$resultat_REF_CLASSRC=mysql_query($sql_REF_CLASSRC);
	include("inc/header.php"); //contient les en-tetes
?>
<title>MedinPHP - DRC</title>
</head>
<body>
<h1>DRC</h1>
<form action="drc.php" method="GET"> 
Classes de RC
<select name="classe_rc">
<?php
while ($ligne_REF_CLASSRC=mysql_fetch_array($resultat_REF_CLASSRC))
{
?>
<option <?php if ($classe_rc==$ligne_REF_CLASSRC['REF_CLASSRC_SEQ']) echo "selected" ?> value="<?php echo $ligne_REF_CLASSRC['REF_CLASSRC_SEQ'] ?>"><?php echo $ligne_REF_CLASSRC['REF_CLASSRC_LIB'] ?></option>

<?php
}
?>
</select>
<input name="bouton_ref_classe" type="submit" value="Envoyer" />
</form>
</body>
</html>