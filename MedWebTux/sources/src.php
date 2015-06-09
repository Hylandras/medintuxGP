<?php
@session_start();

if ( !isset( $_SESSION['login'] ) ) {
  exit;
}

//---------------------------- controle du fichier maj ------------------------------------
	$maj_serveur = "".URL3."/".REPERTOIRES."/MAJ?dl=1&token_hash=AAE-3VI7LuxBERIm2A8ttftNx9pyhbMc88bVoKmL40v67g";
  	$maj_serveur = @file_get_contents($maj_serveur);
  	
  	$maj_locale = "".DOSSIER_SOURCES."MAJ";
  	$maj_locale = @file_get_contents($maj_locale);
//-----------------------------------------------------------------------------------------

//-------------------- generer le fichier dc_constante si il n'existe pas -----------------
if (!file_exists("".DOSSIER_SOURCES."/dc_constante.php") || file_exists("".DOSSIER_SOURCES."/dc_constante.php")) {
   
	$file = @fopen("".DOSSIER_SOURCES."/dc_constante.php", "w");	
	$text = "<?php
define('DC_LOADER_SERVICE','".URL2."/netupdate/');
define('DC_LOADER_ARCHIVE','".URL1."/".REPERTOIRES."/MEDWEBTUX.zip?dl=1&token_hash=AAE-3VI7LuxBERIm2A8ttftNx9pyhbMc88bVoKmL40v67g');
?>"; 
	fwrite($file, $text);
	@fclose($file);
}
//-----------------------------------------------------------------------------------------
	
//------------------------ fichiers se trouvant sur le serveur ----------------------------
	 if ($maj_serveur == $maj_locale) {
		$maj_locale = explode("\t", $maj_locale);
	 	echo "<fieldset class=\"fieldset\">";
		echo "<legend class=\"legend\">Netupdate</legend>";
	 	echo "<p>Dernière mise à jour du logiciel : <b> " .substr($maj_locale[0],"-10"). "  à  ".$maj_locale[1]."  -  ".$maj_locale[2]."</b></p>";
	 	echo "</fieldset>";
	 }
	 else {
		echo "<fieldset class=\"fieldset\">";
		echo "<legend class=\"legend\">Netupdate</legend>";
	 	echo "<iframe src=\"netupdate.php\" width=\"100%\" height=\"180px\" scrolling=\"no\" frameborder=\"0\"></iframe>";
	 	echo "</fieldset>";
	 }
?>

