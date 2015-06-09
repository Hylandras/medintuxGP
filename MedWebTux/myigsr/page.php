<?php
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
#	des operations suivantes, de 2009 à 2013 par Yanick Morza.							#
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
	@set_time_limit(0);
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
	//recuperation des variables de connexion par la methode GET
	$host = $_GET['host'];
	$loginbase = $_GET['loginbase'];
	$pwd = $_GET['pwd'];
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
if (!file_exists("connexion_serveur.php")) {
?>
<fieldset class="fieldset_page">
<legend>Formulaire de connexion au serveur mysql <?php echo $_SERVER["HTTP_HOST"] ?></legend>

<div class="admin_liste">
<form action="" method="get">
<table>
<tr>
  <td>Serveur Mysql</td>
  <td><input type="text" size=30 name="host" value="<?php echo $host; ?>"></td>
</tr>
<tr>
  <td>Login</td>
  <td><input type="text" size=30 name="loginbase" value="<?php echo $loginbase; ?>"></td>
</tr>
<tr>
  <td>Password</td>
  <td><input type = "password" size=30 name="pwd" value="<?php echo $pwd; ?>"></td>
</tr>
<tr>
  <th colspan="2"><input type = "submit" value="Connexion"></th>
</tr>
</table>
</form>
</div>
</fieldset>
<?php

//-----------------------------------------------------------------------------------------------

//-------------------------------- test de connexion au serveur ---------------------------------
   if (!$host || !$loginbase || !$pwd) {
   
   	$resultat = @mysql_connect("$host", "$loginbase", "$pwd");
   	echo "<br />La tentative de connexion au serveur MySQL de ".$_SERVER["HTTP_HOST"]." a &eacute;chou&eacute;.";
	echo "<br />Seul l'administrateur système de vos bases de données doit avoir l'accès.";	
   }
   else {
//------------------------------ debut creation du fichier connexion ----------------------------
$f = 'connexion_serveur.php';
$text = "<?php
//connexion au serveur mysql 
  \$host = '$host';
  \$loginbase = '$loginbase';
  \$pwd = '$pwd';
?>
";

	$handle = @fopen($f,"w");
	// regarde si le fichier est accessible en ecriture
	if (is_writable($f)) {
	// Ecriture
	echo "<meta http-equiv=\"refresh\" content=\"0;url=page.php\" />";
	echo("<script>parent.menu.location='menu.php';</script>");  
	 
	    if (fwrite($handle, $text) === FALSE) {
	      echo 'Impossible d\'&eacute;crire dans le fichier '.$f.' Faire un chmod 644 sur ce fichier.<br>';
	      exit();
	    }
	    fclose($handle);                
	}
	else {
	      echo "<font color='red'>Impossible d'&eacute;crire dans le fichier <b>".$f."</b> Faire un chmod 644 sur ce fichier.&nbsp;&nbsp;<img src='images/attention.gif'><br></font>";
	      exit;
	}
   }
//--------------------------- fin creation du fichier connexion ---------------------------------
}
else {
//--------------------------- recuperation de la variable database ------------------------------
 if (isset($_GET['database'])) {
  	$database = $_GET['database'];

	if (!is_dir($database)) {
		@mkdir("bases_donnees/".$database);
		@copy("lib/index.html", "bases_donnees/".$database."/index.html");
 	}
 }
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
  require_once ("lib/pclzip/pclzip.lib.php");
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
if (isset($_GET['backup_base'])) {
  	$backup_base = $_GET['backup_base'];
  	
	if (!file_exists("bases_donnees/".$backup_base."/backup_tables_".$backup_base."_db.zip")) {
	  	$archive = new PclZip("bases_donnees/".$backup_base."/backup_tables_".$backup_base."_db.zip");
	  	
		if ($archive->create("bases_donnees/".$backup_base."/") == 0) {
	    	die("Error : ".$archive->errorInfo(true));
		}
		else {
			echo "<meta http-equiv=\"refresh\" content=\"0;url=page.php?database=$backup_base#form2\" />";
		}
	}
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
	else {
	  	$archive = new PclZip("bases_donnees/".$backup_base."/backup_tables_".$backup_base."_db.zip");
		if ($archive->extract() == 0) {
	    	die("Error : ".$archive->errorInfo(true));
		}	
		else {
			@unlink("bases_donnees/".$backup_base."/backup_tables_".$backup_base."_db.zip");
			echo "<meta http-equiv=\"refresh\" content=\"0;url=page.php?database=$backup_base#form2\" />";
		}
	}
}
//-----------------------------------------------------------------------------------------------

//----------------------------------- Connexion au serveur Mysql --------------------------------
	@include_once("connexion_serveur.php");
	//echo $host."&nbsp;".$loginbase."&nbsp;".$pwd;
//-----------------------------------------------------------------------------------------------

//------------------ recuperation de la variable document par la methode GET --------------------
   	$document = $_GET['document'];

	if (isset($_GET[document])) {
		//supprimer le ou les fichiers de backup dans le repertoire database
	    	@unlink("bases_donnees/".$database."/".$document);
		echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form2\" />";
	}
//-----------------------------------------------------------------------------------------------

//------------------ recuperation de la variable supprimer par la methode GET -------------------
   $supprimer = $_GET['supprimer'];

	if (isset($_GET['supprimer'])) {
		//supprimer tous les fichiers .sql dans le repertoire database selectionne	
		$repertoire = opendir("bases_donnees/".$database); 
 
		while (false !== ($fichier = @readdir($repertoire))) {
			$chemin = "bases_donnees/".$database."/".$fichier; 
 
			//si le fichier n'est pas un repertoire
			if ($fichier != ".." && $fichier != "." && $fichier != "index.html" && $fichier != "backup_tables_".$database."_db.zip" && !is_dir($fichier)) {
			       @unlink($chemin); 
			}
		}
		closedir($repertoire);
		echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form2\" />";
	}
//-----------------------------------------------------------------------------------------------

//----------------------- recuperation de la variable backcocher par GET ------------------------
	if (isset($_GET['backcocher'])) {
		$backcocher = $_GET['backcocher'];
	}
//-----------------------------------------------------------------------------------------------

//---------------------- recuperation de la variable restococher par GET ------------------------
	if (isset($_GET['restococher'])) {
		$restococher = $_GET['restococher'];
	}
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
	// Connecter au serveur et choisir sa base de donnees
	$conn = @mysql_connect($host, $loginbase, $pwd);
	@mysql_select_db ($database);
//-----------------------------------------------------------------------------------------------

//-------------------- renommer toutes les tables avec nouveau prefixe --------------------------
if (isset($_POST["new_prefixe"]) && isset($_POST["prefixe"])) {

	$new_prefixe = $_POST["new_prefixe"];

	//calcul le nombre de caractere du prefixe 
	$prefixe = $_POST["prefixe"];
	$long_pref = strlen($prefixe);

	$req_table = "SHOW TABLES like '$prefixe%'";
	$result_table = mysql_query($req_table) or die ("Impossible d'executer la requete concernant la recherche des tables - ".mysql_error());
		
	while ($donnees_table = mysql_fetch_array($result_table)) {		
		$new_table = substr($donnees_table[0], $long_pref);						
		$rename_sql = "RENAME TABLE ".$donnees_table[0]." TO ".$new_prefixe.$new_table."";
		$result_sql = mysql_query($rename_sql) or die ("Impossible d'executer la requete concernant la renommage des tables - ".mysql_error());
	}
	echo "<meta http-equiv=\"refresh\" content=\"0;url=page.php?database=$database\" />";
}
//-------------------- renommer toutes les tables avec nouveau prefixe --------------------------

//------------------------- formulaire modifier le prefixe des tables ---------------------------
if (isset($_GET["renommer_tables"]) && isset($_GET["database"])) {

	$req_table = "SHOW TABLES from $database";
	$result_table = mysql_query($req_table) or die ("Impossible d'executer la requete concernant la recherche des tables - ".mysql_error());	
	$donnees_table = mysql_fetch_array($result_table);
	
	if (stristr($donnees_table[0], '_') === FALSE) {
    		echo "<div class=\"admin_liste\">";
    		echo "Impossible de renommer ces tables car elles ne poss&egrave;dent pas de pr&eacute;fixe !";
    		echo "</div>";
  	}
	else {
		$prefixe = explode("_", $donnees_table[0]);
		$prefixe = $prefixe[0];
	
		echo "<div class=\"admin_liste\">";
		echo "<form action=\"\" method=\"post\">";
		echo "<b>Ajouter</b> ou <b>modifier</b> le pr&eacute;fixe des tables de la Base de donn&eacute;es<b> $database</b>&nbsp;&nbsp;";
		echo "<input type=\"hidden\" name=\"prefixe\" value=\"$prefixe\" />&nbsp;";
		echo "<input type=\"text\" name=\"new_prefixe\" size=\"8\" onfocus=\"this.value='';\" value=\"$prefixe\" />&nbsp;";
		echo "<input type=\"submit\" name=\"renommer\" value=\"Enregistrer\" />";
		echo "</form>";
		echo "</div>";
	}

}
//------------------------- formulaire modifier le prefixe des tables ---------------------------

//----------------------- recuperation de la variable vidertable par GET ------------------------
	if (isset($_GET['vidertable'])) {
	
		$vidertable = $_GET['vidertable'];
		
		$query = "TRUNCATE TABLE ".$vidertable; 
        	mysql_query($query, $conn);
        	echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form1\" />";
	}
//-----------------------------------------------------------------------------------------------

//----------------------- recuperation de la variable deltable par GET --------------------------
	if (isset($_GET['deltable'])) {
	
		$deltable = $_GET['deltable'];
		
		$query = "DROP TABLE ".$deltable; 
        	mysql_query($query, $conn);
        	echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form1\" />";
        	echo("<script>parent.menu.location='menu.php';</script>");
	}
//-----------------------------------------------------------------------------------------------

//---------------------------------- Vider toutes les tables ------------------------------------
	if (isset($_GET['vider_tables'])) {
	
		$vider_tables = $_GET['vider_tables'];
		$database = $_GET['database'];
		
		$req_vider_tables = "SHOW TABLES FROM $database";
		$result_vider_tables = @mysql_query($req_vider_tables, $conn) or die ("<i>Impossible d'acc&eacute;der aux tables de votre base de donn&eacute;es.</i>");
		while ($show_vider_tables = @mysql_fetch_array($result_vider_tables)) {
			$vider_nom_table = $show_vider_tables[0];
			
			$query = "TRUNCATE TABLE ".$vider_nom_table; 
			mysql_query($query, $conn);	
		}
        	echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form1\" />";
	}
//-----------------------------------------------------------------------------------------------

//-------------------------------- Supprimer toutes les tables ----------------------------------
	if (isset($_GET['supprimer_tables'])) {
	
		$supprimer_tables = $_GET['supprimer_tables'];
		$database = $_GET['database'];
		
		$req_supprimer_tables = "SHOW TABLES FROM $database";
		$result_supprimer_tables = @mysql_query($req_supprimer_tables, $conn) or die ("<i>Impossible d'acc&eacute;der aux tables de votre base de donn&eacute;es.</i>");
		while ($show_supprimer_tables = @mysql_fetch_array($result_supprimer_tables)) {
			$del_nom_table = $show_supprimer_tables[0];
			
			$query = "DROP TABLE ".$del_nom_table; 
			mysql_query($query, $conn);	
		}
        	echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form1\" />";
        	echo("<script>parent.menu.location='menu.php';</script>"); 
	}
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
if (isset($_POST["modifie_document"]) && isset($_POST["nom_fichier"])) {

		$modifie_document = stripslashes(html_entity_decode($_POST["modifie_document"]));
		$nom_fichier = htmlentities($_POST["nom_fichier"]);
		
		$f = "$nom_fichier";
		$text = "$modifie_document";
		
		$handle = @fopen("$f","w");
		  // regarde si le fichier est accessible en écriture
		  if (is_writable($f)) {
		    // Ecriture
		    if (@fwrite($handle, $text) === FALSE) {
		      echo 'Impossible d\'&eacute;crire dans le fichier '.$f.' Faire un chmod 777 sur ce fichier.<br>';
		      exit();
		    }
		      @fclose($handle);                
		  }
	echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]\" />";
} 
//-----------------------------------------------------------------------------------------------

//----------------------------- Formulaire modification fichier sql -----------------------------
if (isset($_GET["edit_document"])) {
	$edit_document = $_GET["edit_document"];
	$source = @file_get_contents("$edit_document");
?>

<div class="admin_liste">
<form method="post" action="">
<table>
 <tr>
 <tr>
  <th><h3>Modifier le fichier : <?php echo $edit_document; ?></h3></th>
 </tr>
  <th>
  <input type="hidden" name="nom_fichier" value="<?php echo $edit_document; ?>">
  <textarea cols="100" rows="20" name="modifie_document"><?php echo $source; ?></textarea>
  </th>
 </tr>
 <tr>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <th><input type="submit" value="Enregistrer"></th>
 </tr>
</table>
</form>
</div>

<?php
// ----------- ON RECUPERE LE CONTENU DU FICHIER SQL ET ON L'INTEGRE DANS LA BDD --------------
if (isset($_POST["execute_sql"])) {

	$execute_sql = $_POST["execute_sql"];
	$execute_sql = explode("/", $execute_sql);

	$file_sql = $execute_sql[0]."/".$execute_sql[1]."/".$execute_sql[2];
	$baseselect = $execute_sql[1];	

	//connexion a la base de donnee
	$conn = mysql_connect($host, $loginbase, $pwd);
	mysql_select_db ($baseselect);

	$handle = fopen($file_sql,"r");
	$contenu = fread($handle, filesize($file_sql));
	$contenu = str_replace(");",")@@",$contenu);
	$contenu = explode("@@",$contenu);
	fclose ($handle);
	foreach($contenu as $ligne)		
	{ 
		mysql_query($ligne); 
	}
	echo("<script>parent.menu.location='menu.php';</script>"); 
}
// ----------- ON RECUPERE LE CONTENU DU FICHIER SQL ET ON L'INTEGRE DANS LA BDD --------------
?>
<div class="admin_liste">
<form method="post" action="">
<input type="hidden" name="execute_sql" value="<?php echo $edit_document; ?>">
<input type="submit" value="Ex&eacute;cuter ce fichier SQL">
</form>
</div>

<?php
	exit();
}
//----------------------------- Formulaire modification fichier sql -----------------------------

//----------------------------------- Debut formulaire sauvegarde -------------------------------
// Rechercher toutes les tables de la bdd concernee
$req_table = "SHOW TABLES";
$result_table = @mysql_query($req_table) or die (); 
//-----------------------------------------------------------------------------------------------

//-------------- recuperation des variables newtable oldtable database par POST -----------------
	if (isset($_POST['newtable']) && isset($_POST['oldtable']) && isset($_POST['database'])) {
		$newtable = $_POST['newtable'];
		$oldtable = $_POST['oldtable'];
		$database = $_POST['database'];
		
		$query = "RENAME TABLE `".$database."`.`".$oldtable."` TO `".$database."`.`".$newtable."` ";  
        mysql_query($query, $conn);
        echo "<meta http-equiv=\"refresh\" content=\"0;url=page.php?database=$database\" />";
	}
//-----------------------------------------------------------------------------------------------

//----------------- recuperation des variables edittable database par GET -----------------------
	if (isset($_GET['edittable']) && isset($_GET['database'])) {
		$edittable = $_GET['edittable'];
		$database = $_GET['database'];
		
		echo "<div class=\"admin_liste\">";
		echo "<form action=\"\" method=\"post\">";
		echo "<input type=\"hidden\" name=\"oldtable\" value=\"".$edittable."\" />";
		echo "<input type=\"hidden\" name=\"database\" value=\"".$database."\" />";
		echo "<b>Modifier le nom de la table</b> <input type=\"text\" name=\"newtable\" value=\"".$edittable."\" size=\"30\" />";
		echo "&nbsp;<input type=\"submit\" value=\"modifier\" />";
		echo "</form>";
		echo "</div>";
	}
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
echo "<fieldset class=\"fieldset_page\">";
echo "<legend>Cr&eacute;er une nouvelle sauvegarde pour la base de donn&eacute;es <u>".$database."</u></legend>";

echo " <class id='form1'>";
echo "<div class=\"admin_liste\">";
echo "<img border=\"0\" height=\"48\" title=\"Cr&eacute;er une nouvelle sauvegarde\" src=\"images/backup.png\" alt=\"\" />";
echo "<form action=\"\" method=\"post\">";
echo "<table>
<tr>";
//initialisation des compteurs
	$compteur = 0;
	$i = 0;
// Afficher sous forme de lien
	while ($donnees_table = @mysql_fetch_array($result_table)) {

	// compter le nombre de ligne dans chaque table
		$sql_nbr_ligne = "select * from ".$donnees_table[0]."";
  		$result_nbr_ligne = @mysql_query($sql_nbr_ligne, $conn);
  		$nbr_ligne = @mysql_num_rows($result_nbr_ligne);

		if ($nbr_ligne <= "1") {
			$ligne = "ligne";
		}
		else {
			$ligne = "lignes";
		}
	// compter le nombre de ligne dans chaque table

	// Compter la taille de chaque table 
		$sql = "SHOW TABLE STATUS LIKE '$donnees_table[0]'";
		$result = mysql_query($sql);
		$tab = mysql_fetch_array($result);
		$taille = ($tab[Data_length] + $tab[Index_length]);

		if ($taille > 1024 * 1024) {
            		$taille = (($taille * 100 / (1024 * 1024)) / 100);
			$taille = round($taille , 2)."Mo";
		}
          	else {
            		$taille = (($taille * 100 / 1024) / 100);
			$taille = round($taille , 2)."Ko";
		}

	// Compter la taille de chaque table

		if ($compteur % 3 != 0) {
				echo "<td>
				
	<a href=\"page.php?vidertable=$donnees_table[0]&amp;database=$database\" onclick=\"return confirm('Attention vous allez vider la table $donnees_table[0], voulez vous continuer ?')\">
	<img height=\"16\" src=\"images/truncate.png\" border=\"0\" alt=\"\" title=\"Vider la table $donnees_table[0]\" /></a>
	
	<a href=\"page.php?deltable=$donnees_table[0]&amp;database=$database\" onclick=\"return confirm('Attention vous allez supprimer la table $donnees_table[0], voulez vous continuer ?')\">
	<img height=\"16\" src=\"images/drop.gif\" border=\"0\" alt=\"\" title=\"Supprimer la table $donnees_table[0]\" /></a>
	
	<a href=\"page.php?edittable=$donnees_table[0]&amp;database=$database\">
	<img height=\"16\" src=\"images/edit.png\" border=\"0\" alt=\"\" title=\"Modifier la table $donnees_table[0]\" /></a>
	
	<input type=\"checkbox\" name=\"options[]\" value=\"$donnees_table[0]\" $backcocher title=\"Sauvegarder la table $donnees_table[0] (".$nbr_ligne." $ligne)\" /> ".$donnees_table[0]."&nbsp;<i>(".$taille.")</i></td>";
		}
		else {
			echo "</tr><tr><td>
			
	<a href=\"page.php?vidertable=$donnees_table[0]&amp;database=$database\" onclick=\"return confirm('Attention vous allez vider la table $donnees_table[0], voulez vous continuer ?')\">
	<img height=\"16\" src=\"images/truncate.png\" border=\"0\" alt=\"\" title=\"Vider la table $donnees_table[0]\" /></a>
	
	<a href=\"page.php?deltable=$donnees_table[0]&amp;database=$database\" onclick=\"return confirm('Attention vous allez supprimer la table $donnees_table[0], voulez vous continuer ?')\">
	<img height=\"16\" src=\"images/drop.gif\" border=\"0\" alt=\"\" title=\"Supprimer la table $donnees_table[0]\"  /></a>
	
	<a href=\"page.php?edittable=$donnees_table[0]&amp;database=$database\">
	<img height=\"16\" src=\"images/edit.png\" border=\"0\" alt=\"\" title=\"Modifier la table $donnees_table[0]\" /></a>
	
	<input type=\"checkbox\" name=\"options[]\" value=\"$donnees_table[0]\" $backcocher title=\"Sauvegarder la table $donnees_table[0] (".$nbr_ligne." $ligne)\" /> ".$donnees_table[0]."&nbsp;<i>(".$taille.")</i></td>";
		}
		$compteur = $compteur + 1;
		$i = $i + 1;
	}
echo "<tr><td colspan=\"2\">&nbsp;</td></tr>";
echo "<tr><td colspan=\"2\">&nbsp;La Base de donn&eacute;e<b> $dbase </b>contient "; 
 if ($i <= "1") {
	echo "<b> $i </b>table.</td></tr>";
 }
 else {
 	echo "<b> $i </b>tables.</td></tr>";
 }
echo "<tr><td colspan=\"3\">&nbsp;</td></tr></table>";
echo "<a href=\"page.php?backcocher=checked&amp;database=$database&amp;#form1\">Tout cocher</a>   |
	<a href=\"page.php?backcocher=false&amp;database=$database&amp;#form1\">Tout d&eacute;cocher</a>   |";
echo " <input type=\"submit\" name=\"sauvegarde\" value=\"Sauvegarder\" />";
echo "   |   <a href=\"page.php?vider_tables=vider_tables&amp;database=$database\" onclick=\"return confirm('Attention vous allez vider toutes les tables de $database, voulez-vous continuer ?')\">Vider les tables</a>   |
	<a href=\"page.php?supprimer_tables=supprimer_tables&amp;database=$database\" onclick=\"return confirm('Attention vous allez supprimer toutes les tables de $database, voulez-vous continuer ?')\">Supprimer les tables</a>   |
	<a href=\"page.php?renommer_tables=renommer_tables&amp;database=$database\" >Renommer les tables</a>";
echo "</form>";
echo "</div>";
echo "</fieldset>";
//----------------------------------- Fin formulaire sauvegarde ---------------------------------

//--------------------------- recuperation de la variable option --------------------------------
 $options = $_POST['options'];
 if (isset($_POST['options'])) {
  		echo "<div class=\"admin_liste\">
		<div align=\"center\"><i>Sauvegarde termin&eacute;e</i><br /></div>
		</div>";
 }
//-----------------------------------------------------------------------------------------------

//-------------------------- recuperation de la variable optionsql ------------------------------
 $optionsql = $_POST['optionsql'];
 if (isset($_POST['optionsql'])) {
  		echo "<div class=\"admin_liste\">
		<div align=\"center\"><i>Restauration termin&eacute;e</i<br /></div>
		</div>";
 }
//-----------------------------------------------------------------------------------------------

//----------------------------------- Debut formulaire restauration -----------------------------
echo "<fieldset class=\"fieldset_page\">";
echo "<legend>Restaurer une sauvegarde pour la base de donn&eacute;es <u>".$database."</u></legend>";

echo "<class id='form2'>";
echo "<div class=\"admin_liste\">";
echo "<img border=\"0\" height=\"48\" title=\"Restaurer une sauvegarde\" src=\"".images."/restore.png\" alt=\"\" />";
echo "<form action=\"\" method=\"post\">";
echo "<table>";
echo "<tr>";

//initialisation des compteurs
$cpt = 0;
$j = 0;

//ouvrir le repertoire database
$rep = opendir("bases_donnees/".$database);
	   	
// lecture des fichiers dans le repertoire database
	while ($document = @readdir($rep)) {	
		if ($document != '..' && $document !='.' && $document !='' && $document !='index.html') { 
						$filesql = filesize("bases_donnees/".$database."/".$document);
						$filesql = $filesql/1024;
						$size = ceil($filesql);
						
			if ($cpt % 3 != 0) {
				if ($document == "backup_tables_".$database."_db.zip") {
					echo "<td>&nbsp;&nbsp;&nbsp;&nbsp;<u><a href=\"bases_donnees/".$database."/".$document."\"> ".$document." <b>".$size."Ko</b></u></a></td>";
				}
				else {
					echo "<td><a href=\"page.php?document=$document&amp;database=$database\" onclick=\"return confirm('Attention vous allez supprimer la sauvegarde $document, voulez-vous continuer ?')\">
		<img height=\"16\" src=\"images/drop.gif\" border=\"0\" alt=\"\" title=\"Supprimer le fichier $document\" /></a>
		<input type=\"checkbox\" name=\"optionsql[]\" value=\"$document\" $restococher title=\"Restaurer le fichier $document\" />
		<a href=\"page.php?edit_document=bases_donnees/".$database."/".$document."\"> ".$document." </a><i>(".$size."Ko)</i></td>";
				}
			}
			else {
				if ($document == "backup_tables_".$database."_db.zip") {
					echo "</tr><tr><td>&nbsp;&nbsp;&nbsp;&nbsp;<u><a href=\"bases_donnees/".$database."/".$document."\"> ".$document." <b>".$size."Ko</b></u></a></td>";
				}
				else {
					echo "</tr><tr><td><a href=\"page.php?document=$document&amp;database=$database\" onclick=\"return confirm('Attention vous allez supprimer la sauvegarde $document, voulez-vous continuer ?')\">
		<img height=\"16\" src=\"images/drop.gif\" border=\"0\" alt=\"\" title=\"Supprimer le fichier $document\" /></a>
		<input type=\"checkbox\" name=\"optionsql[]\" value=\"$document\" $restococher title=\"Restaurer le fichier $document\" />
		<a href=\"page.php?edit_document=bases_donnees/".$database."/".$document."\"> ".$document." </a><i>(".$size."Ko)</i></td>";
				}
			}
			$cpt = $cpt + 1;
			$j = $j + 1;
		}		 
	}
echo "<tr><td colspan=\"2\">&nbsp;</td></tr>";
echo "<tr><td colspan=\"2\">&nbsp;Le r&eacute;pertoire<b> $database </b>contient "; 
 if ($j <= "1") {
	echo "<b> $j </b>fichier.</td></tr>";
 }
 else {
 	echo "<b> $j </b>fichiers.</td></tr>";
 }

echo "<tr><td colspan=\"2\">&nbsp;</td></tr></table>";
echo "<a href=\"page.php?restococher=checked&amp;database=$database&amp;#form2\">Tout cocher</a>   |
	<a href=\"page.php?restococher=false&amp;database=$database&amp;#form2\">Tout d&eacute;cocher</a>   |";
echo " <input type=\"submit\" name=\"restaurer\" value=\"Restaurer\" />   |";
echo " <a href=\"page.php?supprimer=supprimer&amp;database=$database\" onclick=\"return confirm('Attention vous allez supprimer toutes vos sauvegardes, voulez-vous continuer ?')\">Tout supprimer</a>   |";

if (!file_exists("bases_donnees/".$database."/backup_tables_".$database."_db.zip")) {
	echo "   <a href=\"page.php?backup_base=".$database."\" onclick=\"return alert('Un fichier zip appelle backup_tables_".$database."_db.zip a ete cree. Il est disponible dans le repertoire $database')\">Compresser</a>";
}
else {
	echo "   <a href=\"page.php?backup_base=".$database."\">D&eacute;compresser</a>";
}
echo "</form>";
echo "</div>";
echo "</fieldset>";
//----------------------------------- Fin formulaire restauration -------------------------------

//----------------------------------- Traitement de la sauvegarde -------------------------------

	if ($options) {

		foreach ($options as $table) {                                       
			// Demander la "creation" de la table
			$req_structure = "SHOW CREATE TABLE $table";
			$result_structure = @mysql_query($req_structure) or die ("Impossible de trouver la structure de ". $table .mysql_error());
			$donnee_structure = @mysql_fetch_array($result_structure);
			$structure  = "";
			$structure .= $donnee_structure[1] ;
			$structure .= "; <br /><br />" ;

			//supprimer les chiffres auto_ecrement qui reste dans les tables
			for ($i=1; $i<100000; $i++) {
				$structure = str_replace(" ENGINE=MyISAM AUTO_INCREMENT=$i DEFAULT CHARSET=latin1", "", $structure);
				$structure = str_replace(" ENGINE=InnoDB AUTO_INCREMENT=$i DEFAULT CHARSET=latin1", "", $structure);
				$structure = str_replace(" ENGINE=MyISAM DEFAULT CHARSET=latin1", "", $structure);
				$structure = str_replace(" ENGINE=InnoDB DEFAULT CHARSET=latin1", "", $structure);
				$structure = str_replace(" COLLATE=latin1_general_ci", "", $structure);
			} 

			// Recuperer le nombre de champs presents dans la table   
			$req_champ = "SHOW COLUMNS FROM $table";
			$result_champ = mysql_query ($req_champ) or die ("Impossible de trouver les champs de ". $table .mysql_error());
			$nbre_champ = mysql_num_rows($result_champ);
		 
			// Rechercher tous les enregistrements de la table concernee 
			$req_tout = "SELECT * FROM $table"; 
			$result_tout = mysql_query($req_tout) or die ("Impossible de trouver les enregistrements de ". $table .mysql_error()); 
			$contenu = "";
			$tout_contenu = "";
	 
					// Boucle pour sortir toutes les donnees 
					while ($donnees_tout = mysql_fetch_array($result_tout)) { 
							$contenu = "INSERT INTO " . $table . " VALUES ("; 
			 
							$i = 0; 
							// Boucle sur tous les champs 
							while ( $i < $nbre_champ ) { 
									// Remplacer les apostrophes du contenu par 2 apostrophes
									$donnees_tout[$i] = str_replace("'","''",$donnees_tout[$i]);
											
									// Affiche les resultats en fonction des champs et dans l'ordre des champs 
									$contenu .= "'" . $donnees_tout[$i] . "',"; 
									$i++; 
							} 
							// Enlever la dernière virgule
							$contenu = substr($contenu,0,-1);
							$contenu .= ");\n"; 
							//echo "<br />"; 
							$tout_contenu .= $contenu;  
					}
			
					if (isset($_POST['sauvegarde'])) {
					  // Creer un nom pour le fichier .sql correspondant a la table
					  $date      = date("d-m",time());
					  $nom_table = $table."_".$date;
					  $nom_table .= ".sql";
									
							// Teste si le fichier .sql existe deja
							if (file_exists("bases_donnees/".$database."/".$nom_table)) {
							   // si oui, on le supprime 
							   @unlink("bases_donnees/".$database."/".$nom_table);
							}
					}
					
					   // Ouvrir le fichier .sql en ecriture et on le cree en meme temps s'il est inexistant
					   $sauve = fopen("bases_donnees/".$database."/".$nom_table , "a+");                        
															
					   $structure = str_replace("<br />" , "\n", $structure);
					   fputs($sauve, $structure);
					   fputs($sauve, $tout_contenu);
					
					   // Fermer le fichier .sql     
					   fclose($sauve);
		}
			echo "<meta http-equiv=\"refresh\" content=\"1;url=page.php?database=$database\" />";
	}
//----------------------------------- Traitement de la sauvegarde -------------------------------

//---------------------------------- Traitement de la restauration ------------------------------
//recuperation de la variable optionsql
 $optionsql = $_POST['optionsql'];
	if ($optionsql) {
 
		foreach ($optionsql as $document) { 
		
				//fichier a envoyer dans la base de donnees
				$fichier_sql = "bases_donnees/".$database."/".$document;

				//connexion a la base de donnee
				$conn = mysql_connect($host, $loginbase, $pwd);
				mysql_select_db ($dbase);

				//supprimer la table si elle existe
				$table_sql = substr($document, 0, -10);
				$sql = "DROP TABLE IF EXISTS $table_sql";
				$result = mysql_query($sql) or die ("Impossible de supprimer la table ". $table_sql .mysql_error());
				  
				//recuperer le contenu du fichier sql pour injecter dans la BDD
				$handle  = fopen($fichier_sql,"r");
				$contenu = fread($handle, filesize($fichier_sql));
				$contenu = str_replace(");",")@@",$contenu);
				$contenu = explode("@@",$contenu);
				fclose ($handle);
			
				//pour chaque ligne du fichier sql insertion du contenu
				foreach($contenu as $ligne) { 
					mysql_query($ligne); 
				}
		}
		echo "<meta http-equiv=\"refresh\" content=\"1;url=page.php?database=$database\" />";
		echo("<script>parent.menu.location='menu.php';</script>");  
	}
//---------------------------------- Traitement de la restauration ------------------------------

//------------------- affichage du formulaire d'importation du fichier zip ----------------------
if (isset($_POST["choix"])) {
		$choix = $_POST["choix"];
		
		if ($choix == "oui.txt") {
			$handle = @fopen("lib/".$choix,"w");
			@fclose($handle);
			@unlink("lib/non.txt");
		}
		elseif ($choix == "non.txt") {
			$handle = @fopen("lib/".$choix,"w");
			@fclose($handle);
			@unlink("lib/oui.txt");
		}
	echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form3\" />";
}
?>

<div class="admin_liste">
<form action="" method="post">
<?php
if (file_exists("lib/oui.txt")) {
?>
<input type="radio" name="choix" value="oui.txt" onclick="javascript:submit()" checked="true">&nbsp;<b>Afficher le formulaire d'import</b>
<input type="radio" name="choix" value="non.txt" onclick="javascript:submit()">&nbsp;<b>Ne pas afficher le formulaire d'import</b>
<?php
}
else {
?>
<input type="radio" name="choix" value="oui.txt" onclick="javascript:submit()" >&nbsp;<b>Afficher le formulaire d'import</b>
<input type="radio" name="choix" value="non.txt" onclick="javascript:submit()" checked="true">&nbsp;<b>Ne pas afficher le formulaire d'import</b>
<?php
}
?>
</form>
</div>

<?php
//----------------------------------- upload du fichier backup -----------------------------------
  if (file_exists("lib/oui.txt")) {

	    if (isset($_FILES['backup']) && $_POST['database']) {
	 
		$tmp_file = $_FILES['backup']['tmp_name'];

		if (($tmp_file) && ($tmp_file != "none")) {
			$database = $_POST["database"];
		
		   	$filename = "bases_donnees/".$database."/".$_FILES['backup']['name']."";
			move_uploaded_file ($tmp_file, $filename);
			echo "<meta http-equiv=\"refresh\" content=\"0;url=$_SERVER[HTTP_REFERER]&amp;#form3\" />";	
	 	}
	    }	
	    
    	echo "<class id=\"form3\">";
	echo "<div class=\"admin_liste\">";
	echo "<img src=\"images/add_backup.png\" border=\"0\" height=\"48\" title=\"Importer une sauvegarde\" alt=\"\" /><b>Importer une sauvegarde</b>";
	echo "<form action=\"\" method=\"post\" enctype=\"multipart/form-data\">";
	echo "<table>";
	echo "<tr>
  			<td>";
	echo "		
			<table>
			<tr>
  			<td colspan=\"3\">
			<b>Importer un fichier sql ou le fichier backup_tables_".$database."_db.zip &nbsp;
			<input type=\"file\" name=\"backup\" size=\"20\" />&nbsp;
			<input type=\"hidden\" name=\"database\" value=\"$database\">
			<input type=\"submit\" value=\"Charger\" /></b>	
			</td>
			</tr>
			</table>
			</td>
 	       </tr>
	</table>
	</form>
</div>";
  }
//----------------------------------- upload du fichier backup -----------------------------------
}
?>
 </body>
</html>
