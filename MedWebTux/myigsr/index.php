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
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD html 4.0 Frameset//EN"
   "http://www.w3.org/TR/REC-html40/frameset.dtd">
<html>
  <head>
    	<link rel="shortcut icon" type="image/x-icon" href="images/araignee.ico" />
  	<title>Interface de Gestion de Sauvegarde et de Restauration</title>
 	<link rel="stylesheet" href="css/style_myigsr.css" type="text/css" />
  </head>
  <frameset cols="280,*">
  <noframe>
   <frame src="menu.php" name="menu" noresize frameborder="0">
   <frame src="page.php" name="page" noresize frameborder="0">
  </noframe>
  </frameset>
</html>
