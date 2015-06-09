<?php
/*
Utilitaire pour ajouter les balises MedinTux dans des blobs qui n'en seraient pas pourvus ou avec de mauvaises.
Valable lors d'une importation d'une autre programme
*/
session_start() ;
?>
	<title>MedWebTux - Correction des observations</title>
</head>
<body style="font-size:10pt" >
<div class="conteneur">
<?php	
// insertion du menu d'en-tete	
	include("../inc/menu-horiz.php");
?>


</tr>
</table>
<?php
include("../inc/footer.php");
?>
</div>
<?php
if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
	header('location: index.php?page=liste' );
	exit;
}
$user=$_SESSION['login'];
include("../config.php");
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion &agrave; $host<br>";
	exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

//on recupere tous les blobs
$sql_recup_obs="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey =RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RbDate_TypeRub!='20080000'";
//on exclut les documents, qui ont un autre en-tete
$resultat_recup_obs=mysql_query($sql_recup_obs,$db);
$compteur=0;
$expr="(<body[^>]*>)(.*)(<\/body>)";
while ($ligne_recup_obs=mysql_fetch_array($resultat_recup_obs))
{
  $compteur++;
  echo $compteur."<br>";
//  if ($compteur==10)
//	  break;
  $obs=$ligne_recup_obs['RbDate_DataRub'];
  $fiche_mieux=(eregi($expr,$obs,$tab)); // a passer en preg_replace pour les nouvelles versions de php
  $obs_modifiee=eregi_replace("<body.*>", " ",$tab[2]);
  $obs_modifiee="<html><head><meta name=\"qrichtext\" content=\"charset=utf-8\" /></head><body text=\"#000000\"  style=\"font-size:10pt;font-family:MS Shell Dlg\"><p>".$obs_modifiee."</p>\n</body></html>\n\0\0";

  echo $ligne_recup_obs['RbDate_IDDos'].$obs_modifiee."<br>" ;
//on met à jour la fiche en sql
  $sql_blobs=sprintf("UPDATE RubriquesBlobs SET RbDate_DataRub='%s' WHERE RbDate_PrimKey='$observation_ID'",mysql_real_escape_string($obs_modifiee));
  $resultat_blobs=mysql_query($sql_blobs,$db);
}
echo "fin".$compteur;
		
include("../inc/header.php");
?>
</body>

</html>
