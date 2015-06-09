<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">

<head>
  <meta name="description" content="Interface compatible avec MedinTux, logiciel de consultation médicale" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta http-equiv="Content-Language" content="fr" />
  <link rel="stylesheet" type="text/css" href="css/style.css" />
  <link rel="stylesheet" href="css/print.css" type="text/css" media="print" />
  <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />

<!-- rajout css et scripts pour nouveau calendrier intégré par Yanick MORZA  -->
  <link rel='stylesheet' href='css/ui.core.css' type='text/css' />
  <link rel='stylesheet' href='css/ui.theme.css' type='text/css' />
  <link rel="stylesheet" href="css/ui.datepicker.css" type="text/css" media="screen" />
  <link rel="stylesheet" href="css/ui.datepickeur.css" type="text/css" media="screen" />

  <script type="text/javascript" src="scripts/jquery/jquery-1.3.2.js"></script>
  <script type="text/javascript" src="scripts/jquery/ui.core.js"></script>
  <script type="text/javascript" src="scripts/jquery/ui.datepicker.js"></script>
  <script type="text/javascript" src="scripts/jquery/ui.datepickeur.js"></script>
  <script type="text/javascript">
	$(function() {
		$("#datepicker").datepicker();
	});
	$(function() {
		$("#datepickeur").datepickeur();
	});
  </script>
<!-- rajout css et scripts pour nouveau calendrier intégré par Yanick MORZA  -->

	<!-- NB : "MedWebTux" est la plate-forme web de la suite de gestion médicale "MedInTux", un programme conçu par Data Medical Design (http://medintux.org/medintux/) et distribué sous licence GNU GPL.
	L'architecture graphique que voici est une réalisation contributive de TOSSAVI Kodjo Patrice (http://www.tossavi.net)
	29 Septembre 2008, modifiée en 2012-2013 par Yanick Morza
	-->

<?php

//------------------------------ supprimer un dossier et son contenu ------------------------
function clearDir($dossier) {
	$ouverture=@opendir($dossier);
	if (!$ouverture) return;
	while($fichier=readdir($ouverture)) {
		if ($fichier == '.' || $fichier == '..') continue;
			if (is_dir($dossier."/".$fichier)) {
				$r=clearDir($dossier."/".$fichier);
				if (!$r) return false;
			}
			else {
				$r=@unlink($dossier."/".$fichier);
				if (!$r) return false;
			}
	}
closedir($ouverture);
$r=@rmdir($dossier);
if (!$r) return false;
	return true;
}
//-------------------------------------------------------------------------------------------

//-------------------------------- suppression du dossier set_up ----------------------------
if (isset($_GET["del_setup"])) {
	$del_setup = $_GET["del_setup"];
	clearDir($del_setup);
	echo "<meta http-equiv=\"refresh\" content=\"0;url=index.php\">";
}
//-------------------------------- suppression du dossier set_up ----------------------------

?>
