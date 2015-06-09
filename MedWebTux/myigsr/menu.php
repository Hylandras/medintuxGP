<?php
@session_start();

if ( !isset( $_SESSION['login'] ) ) {
  exit;
}
/*
#########################################################################################################################
#	Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les termes 				#
#	de la Licence Publique Generale GNU telle que publiee par la Free Software Foundation; 				#
#	soit la version 2 de cette licence, soit (à vous de voir...) toute autre version ulterieure.			#
#															#
#	Ce programme est distribue dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE, 				#
#	ni explicite ni implicite, y compris les garanties de commercialisation ou d'adaptation 			#
#	dans un but specifique. Prenez connaissance de la Licence Publique Generale GNU pour plus de details.		#
#															#
#	Vous devez avoir reçu une copie de la Licence Publique Generale GNU avec ce programme; 				#
#	si ce n'est pas le cas, ecrivez à la Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.	#
#	Interface de Gestion des Sauvegardes et des Restaurations pour MySQL decoule d Oeuvres 				#
#	protegees sous licence GNU General Public License. 								#
#															#
#	Il inclut ou est derive de travaux sous licence GNU General Public License ou autres libres ou open source 	#
#	licences de logiciels, y compris Oeuvres sous copyright par une partie ou la totalite 				#
#	des operations suivantes, de 2009 à 2012 par Yanick Morza.							#
#															#
#	email : yanick.morza@free.fr	web : http://www.igswsoft.com 							#
#########################################################################################################################	
*/
/*
	Interface de Gestion de Sauvegarde et de Restauration developpe et maintenu par Yanick Morza 
	C'est un produit derive d'un module venant du CMS IGSW. 
*/
//-----------------------------------------------------------------------------------------------
	error_reporting(E_ALL ^ E_NOTICE);
//-----------------------------------------------------------------------------------------------
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr-fr" lang="fr-fr">
<head>
 <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1" />
 <link rel="shortcut icon" type="image/x-icon" href="images/araignee.ico" />
  <title>Interface de Gestion de Sauvegarde et de Restauration</title>
 <link rel="stylesheet" href="css/style_myigsr.css" type="text/css" />
</head>
<body>
<?php
//-----------------------------------------------------------------------------------------------
 if (!file_exists("connexion_serveur.php")) {
	echo "<img src=\"images/info.png\" />";
	echo "Merci de compl&eacute;ter le formulaire ci contre.";
 }
 else {

//----------------------------------- Connexion au serveur Mysql --------------------------------
	@include_once("connexion_serveur.php");
//-----------------------------------------------------------------------------------------------

//-------------------------------- Drop database selectionner -----------------------------------
	if (isset($_GET['database'])) {
		$database = $_GET[database];

		$conn = @mysql_pconnect($host, $loginbase, $pwd);
		$sql = "DROP DATABASE ".$database;
		@mysql_query($sql, $conn) or die ("Impossible de supprimer la base de donn&eacute;es ". $database .mysql_error());
		
		if (is_dir("bases_donnees/".$database)) {
		
			//supprimer tous les fichiers dans le repertoire database selectionne	
			$repertoire = @opendir("bases_donnees/".$database); 
	 
			while (false !== ($fichier = @readdir($repertoire))) {
				$chemin = "bases_donnees/".$database."/".$fichier; 
				@unlink($chemin); 
			}
			@closedir($repertoire);
	
			//supprimer le repertoire database
			@rmdir("bases_donnees/".$database);
		}		
		echo "<meta http-equiv=\"refresh\" content=\"0;url=menu.php\" />";
		echo("<script>parent.page.location='page.php';</script>"); 
	}
//-----------------------------------------------------------------------------------------------

//---------------------------------- Les parametres a exclure -----------------------------------
	if (isset($_GET['parametre'])) {
		$parametre = htmlentities($_GET[parametre]);

		$f = 'exclure.txt';
		$text = " $parametre ;";

			$handle = @fopen($f,"a+");
			// regarde si le fichier est accessible en ecriture
			if (is_writable($f)) {
			// Ecriture
				if (fwrite($handle, $text) === FALSE) {
				  echo 'Impossible d\'&eacute;crire dans le fichier '.$f.' Faire un chmod 777 sur ce fichier.<br>';
				  exit();
				}
				fclose($handle);                
			}
		echo "<meta http-equiv=\"refresh\" content=\"0;url=menu.php\" />";
	}
//-----------------------------------------------------------------------------------------------

//----------------------------------- Quitter l'application -------------------------------------
	if (isset($_GET['quitter'])) {
		$quitter = $_GET[quitter];
		@unlink("connexion_serveur.php"); 
		echo "<meta http-equiv=\"refresh\" content=\"0;url=menu.php\" />";
		echo("<script>parent.page.location='page.php';</script>");
	}
//-----------------------------------------------------------------------------------------------	 

//----------------------------------- Formulaire choix de la base -------------------------------
?>
<!-- <img src="images/MyIGSR.png" alt="MyIGSR" title="MyIGSR" width="240px" />  -->
<fieldset class="fieldset_menu">
<legend> Bases de donn&eacute;es trouv&eacute;es</legend>

<div class="admin_liste">

<a href="page.php" target="page"><img src="images/home.png" border="0" title="Accueil" alt="Accueil" /></a>
<a href="menu.php?quitter=quitter" onclick="parent.page.location='page.php'"><img src="images/quitter.png" border="0" title="Quitter" alt="Quitter" /></a>
<br /><br />
<?php
	
	$conn = @mysql_pconnect($host, $loginbase, $pwd);
   	$resultat = @mysql_query('SHOW DATABASES', $conn);

	while ($show = @mysql_fetch_array($resultat)) {
		if ($show[0] == "basegettest" || $show[0] == "BaseGetTest" || $show[0] == "ccamtest" || $show[0] == "CCAMTest" || $show[0] == "cim10test" || $show[0] == "CIM10Test" || $show[0] == "datasemptest" || $show[0] == "DatasempTest" || $show[0] == "DrTuxTest" || $show[0] == "drtuxtest" || $show[0] == "medicatuxtest" || $show[0] == "MedicaTuxTest" || $show[0] == "qlaboftp" || $show[0] == "QLaboFTP" || $show[0] == "comptabilite") {
		
			$base = $show[0];
			$req_table = "SHOW TABLES FROM $base";
			$result_table = @mysql_query($req_table, $conn) or die ("<i>Impossible d'acc&eacute;der aux tables de votre base de donn&eacute;es.</i>");
			$nbr_table = @mysql_num_rows($result_table);
			
			echo "<a href=\"menu.php?database=$show[0]\" onclick=\"return confirm('Attention vous allez supprimer la base de donnees $show[0], voulez-vous continuer ?')\"><img height=\"16\" src=\"images/drop.gif\" border=\"0\" alt=\"\" title=\"Supprimer la base de donn&eacute;es $show[0] \"/></a>&nbsp;&nbsp;&nbsp;<a href=\"page.php?database=$show[0]\" target=\"page\" title=\"Afficher les tables de la base de donn&eacute;es $show[0] \">".$show[0]."</a> (".$nbr_table.")<br />";
		}
	}
?>

</div>

</fieldset>

<p><a class="a_bouton" href="https://www.dropbox.com/s/e5y7hmwl5bo6zh8/backup_tables_CCAMTest_db.zip" target="_blank">Derni&egrave;re version CCAM</a></p>

<?php
 }
?>
</body>
</html>
