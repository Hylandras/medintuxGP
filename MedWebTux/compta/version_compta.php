<?php
// On demarre la session
session_start();
$loginOK = false;  // cf Astuce
if ( !isset( $_SESSION['login'] ) )
{
	header('location: ../index.php?page=liste' );
	exit;
}
$user=$_SESSION['login'];
include("../config.php");

$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion &agrave; la base<br>";
	exit;
}

//on se connecte a la compta
if (!mysql_select_db($base_compta,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

if (isset($_REQUEST['bouton_envoyer_modif']))
{
  $bouton_envoyer_modif="on";
  $champ_affichage_version=$_REQUEST['champ_affichage_version'];
}
else 
  $bouton_envoyer_modif="";

if ($bouton_envoyer_modif)
{
$sql_modifier_en_tete="UPDATE z_version SET infos_version='$champ_affichage_version' ";
$resultat_modifier_en_tete=mysql_query($sql_modifier_en_tete,$db);
}
echo $champ_affichage_version;
include("inc/header.php");
?>
		<title>
			MedWebTux - Numéro de version de comptabilité pour MedinTux
		</title>
	<link rel="stylesheet" href="../css/style.css" type="text/css" media="screen" />
	</head>
	<body style="font-size:<?php echo $fontsize; ?>pt" >
<div class="conteneur">
<?php	
// insertion du menu d'en-tete	
	include("inc/menu-horiz-compta.php");
//pour tests
$sql_chercher_utilisateur="SELECT * FROM utilisateurs";
$resultat_chercher_utilisateur=mysql_query($sql_chercher_utilisateur,$db);
$count_chercher_utilisateur=mysql_num_rows($resultat_chercher_utilisateur);
echo "count".$count_chercher_utilisateur."<br>";


$sql_chercher_version="SELECT * FROM z_version";
$resultat_chercher_version=mysql_query($sql_chercher_version,$db);
echo "resultat=".$resultat_sql_chercher_version."<br>";
$ligne_chercher_version=mysql_fetch_array($resultat_chercher_version);
echo "nombre=".mysql_num_rows($resultat_sql_chercher_version)."<br>";
?>
	<div class="groupe">
		<h1>MedWebTux - Version de la compta</h1>
<form action="version_compta.php" method="GET">
<textarea name="champ_affichage_version" rows="25" cols=60  >
<?php echo $ligne_chercher_version['infos_version'] ?>
</textarea>

<!-- <input name="champ_affichage_version" type="textedit" value="<?php echo $ligne_chercher_version['infos_version'] ?>"> -->
<input name="bouton_envoyer_modif" type="submit" value="Modifier">
</form>

<?php
// on ferme la base
mysql_close($db);
echo "</div>";

include("inc/footer.php");
?>
</div>
</body>
</html>
