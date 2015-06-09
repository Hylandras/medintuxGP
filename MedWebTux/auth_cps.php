<?php
session_start();
//connexion au serveur mysql et a la base de donnee en question
include_once("connexion_serveur.php");

$db = mysql_connect($host, $loginbase, $pwd);
$base = "DrTuxTest";

if (!mysql_select_db($base,$db)) {
  print "Erreur ".mysql_error($db)."<br />";
  mysql_close($db);
  exit;
}

//génération de nombres aleatoires
function uuid() 
{
  return sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
	  mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ),
	  mt_rand( 0, 0x0fff ) | 0x4000,
	  mt_rand( 0, 0x3fff ) | 0x8000,
	  mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ) );
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr-fr" lang="fr-fr">
<head>
 <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1" />
  <title>MedWebTux - Connexion CPS </title>
  <link rel="stylesheet" type="text/css" href="css/style.css" />
</head>
<body>
<?php
//emetteur du certificat
//$emetteur = "{$_SERVER['SSL_CLIENT_I_DN']}";
//$emetteur = explode("=", $emetteur);

//client du certificat le PS
$client = "{$_SERVER['SSL_CLIENT_S_DN']}";
$client = explode("=", $client);

//numero de serie de la CPS
//$serie = "{$_SERVER['SSL_CLIENT_M_SERIAL']}";

//expiration en nombre de jour
//$expiration = "{$_SERVER['SSL_CLIENT_V_REMAIN']}";

//variable a enregistrer
//$emetteur = $emetteur[4];
$rpps = substr($client[4], 0, 12);
$nom  = substr($client[5], 0, -3);
$prenom  = $client[6];
//$connexion = date("d-m-Y H:i:s", time());

if (!$nom && !$rpps) {
echo "<div class=\"groupe\">
	<h1>MedWebTux - Connexion CPS</h1>";
	echo "<div class=\"erreur\">";
	echo "Vous ne pouvez pas acc&eacute;der &agrave; cette page, merci de consulter l'administrateur de l'application pour vous aider.";
	echo "<br /><br />Merci de patienter retour dans quelques secondes.";
	echo "</div></div>";

	echo "<meta http-equiv=\"refresh\" content=\"5;url=index.php\" />";
	exit();
}
else {
//------------------ connexion db requete sql sur table auth_cps --------------------------
	     $query = "select count(*) from Personnes where NumRPPS = '$rpps'"; 
	     $result = mysql_query($query,$db);
	     $count = @mysql_result($result, 0, 0);
//-----------------------------------------------------------------------------------------

//------------ Si l'identifiant et l'emetteur sont correctes record session ---------------
     	if ($count > 0) {
		$_SESSION['login'] = $nom; 
		$_SESSION['NumRPPS'] = $rpps;

		//selection du GUID et Signataire en fonction du rpps connecte
		$sql_guid = "select * from Personnes, user_perms where Personnes.GUID = user_perms.FriendUserGUID and NumRPPS = '$rpps'";
		$resultat = mysql_query($sql_guid,$db);
		$query = @mysql_fetch_array($resultat);

		//rechercher le Login dans la table user_perms en fonction du friend
		$sql_friend = "select * from user_perms where FriendUserGUID = '$query[FriendUserGUID]'";
		$result = mysql_query($sql_friend,$db);

		// si l'identificant n'a pas de delegue il est delegue de lui meme
		if (!$query[FriendUserGUID]) {
			echo "<meta http-equiv=\"refresh\" content=\"0;url=index.php?utilisateur_autorisant=$nom\" />";
		}
		// sinon l'identifiant choisi un delegue dans le menu deroulant 
		else {
			echo "<div class=\"groupe\">
			<h1>
			  MedWebTux - Connexion CPS
			</h1>";
			echo "<div class=\"login\">    
	    		<fieldset class=\"fieldset_login\">
	      		<legend>Choisir un d&eacute;l&eacute;gu&eacute; de la liste &nbsp;&nbsp; ";

			echo "<form action=\"index.php\" method=\"get\">
	  		<select name=\"utilisateur_autorisant\" id=\"utilisateur_autorisant\" onchange=\"form.submit()\">
	    		<option value=\"\">- Choisir dans la liste -</option>";
			echo "<option value=\"$nom\">".$nom."</option>";

			while ($query = @mysql_fetch_array($result)) {
				//rechercher le Login dans la table Personnes en fonction du SignataireGUID
				$sql_delegue = "select * from Personnes where GUID = '$query[SignataireGUID]'";
				$result_delegue = mysql_query($sql_delegue,$db);
				$delegue = @mysql_fetch_array($result_delegue);

				echo "<option value=\"$delegue[Login]\">"; 
				echo $delegue["Login"];
	      			echo "</option>";

			}
			echo "</select>
				</form></legend>";
			echo "La connexion CPS vous demande de choisir un d&eacute;l&eacute;gu&eacute; pour continuer. Vous pouvez &ecirc;tre d&eacute;l&eacute;gu&eacute; de vous m&ecirc;me ou de quelqu'un d'autre.";
			echo "</fieldset>
				</div></div>";
		}
		exit();
     	}
//------------ Si l'identifiant et l'emetteur sont correctes record session ---------------

//--------------------- Sinon enregistrement si auth_cps est active -----------------------
	else {
		// si auth_cps_non_auto active ne pas faire d'enregistrement automatique
		if (file_exists ('sources/auth_cps_non_auto.txt')) {
			echo "<div class=\"groupe\">
			<h1>MedWebTux - Connexion CPS</h1>";
			echo "<div class=\"erreur\">";
			echo "Vous ne pouvez pas acc&eacute;der &agrave; cette page, merci de consulter l'administrateur de l'application pour vous aider.";
			echo "<br /><br />Merci de patienter retour dans quelques secondes.";
			echo "</div></div>";

			echo "<meta http-equiv=\"refresh\" content=\"5;url=index.php\" />";
	       		exit();
		}
		else {
			$sql_nom = "select * from Personnes where Nom = '$nom' and Prenom = '$prenom'";
			$result_nom = mysql_query($sql_nom,$db);
			$rech_nom = @mysql_fetch_array($result_nom);

			//rechercher le Login dans la table Personnes en fonction du nom et du prenom et faire maj du NumRPPS
			if ((strtoupper($rech_nom['Nom']) == $nom) && (strtoupper($rech_nom['Prenom']) == $prenom)) {
				$GUID = $rech_nom['GUID'];
				$sql = "update Personnes set NumRPPS = '$rpps' where GUID = '$GUID'";
			 	$result = mysql_query($sql,$db);
				echo "<meta http-equiv=\"refresh\" content=\"0;url=auth_cps.php\" />";
		       		exit();
			}
			// sinon faire l'enregistrement automatique
			else {
				$GUID = uuid();
			  	$sql_cps = "insert into Personnes values('$GUID','','$nom','','','-ata-obc-idc-agc-atv-obv-sgn-','','','','$nom','$prenom','','','','','','','','','','','','','','','','','','','','','','','$rpps','')";
			  	$resultat_cps = mysql_query($sql_cps,$db);
				echo "<meta http-equiv=\"refresh\" content=\"0;url=auth_cps.php\" />";
		       		exit();
			}
		}
     	}
//--------------------- Sinon enregistrement si auth_cps est active -----------------------
}
?>
</body>
</html>
