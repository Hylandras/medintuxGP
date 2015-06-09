<?php
include("../config.php");
include ("rtfclass.php"); //convertisseur rtf->html
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
	header('location: index.php?page=agenda' );
	exit;
}

//test pour integrer un fichier rtf dans un document medintux
$db=mysql_connect($host, $loginbase, $pwd);
if(!$db)
{
	print "Erreur de connexion &agrave; $db<br>";
	exit;
}

// on choisit la bonne base
$date_complete=date('Y-m-d', date('U')).' 00:00:00';
$type='20020300';//prescription
$type='20080000';//document
$titre_observation='Document rtf';
if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}
	$user=$_SESSION['login'];

if (isset($_FILES['selection'])) 
{
//  echo "Selection bien reçue<br>"; //OK
  if ($selection = $_FILES['selection']['tmp_name'])
    echo "";
  else
    echo "echec de FILES<br>"; //pas bon :-(
  if ($fichier=fopen($selection,"r")) //ouverture du fichier renvoye par l'interface
    echo ""; 
  else
    echo "echec fichier<br>";
//  $compteur=0;
//  while (!feof($fichier)) 
//{
$contents=fread( $fichier,filesize($selection));
//echo $contents.'<br><br>'; //le rtf recu
		$r = new rtf( $contents );
		$r->output( "html");
		@$r->parse();
		if( count( $r->err) == 0) // no errors detected
//			echo $r->out;
			$contents= $r->out;
			$contents="<html><head><meta name=\"qrichtext\" content=\"charset=utf-8\" /></head><body text=\"#000000\"  style=\"font-size:10pt;font-family:MS Shell Dlg\"><p>".$contents."</p>\n</body></html>\n\0\0";
}
//On recupere les variables dans l'URL
if (isset($_REQUEST['GUID']))
{
	$GUID=$_REQUEST['GUID'];
//on insere le texte
  $sql_insert_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$GUID')",mysql_real_escape_string($contents));
//  echo $sql_insert_blob;
//  $resultat_insert_blob=mysql_query($sql_insert_blob,$db);
	  
//on recupere l'ID du blob
  $id_blob=mysql_insert_id();
//on insere l'en-tete
  $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$GUID','$type','%s','$date_complete','$user','$user','$id_blob')",mysql_real_escape_string($titre_observation));
//  echo $sql_insert_header;
//  $resultat_insert_header=mysql_query($sql_insert_header,$db);
  //on affiche le resultat
 $sql_chercher_doc="SELECT * FROM rubriquesblobs WHERE RbDate_PrimKey='$id_blob'";
$resultat_chercher_doc=mysql_query($sql_chercher_doc);
$ligne_chercher_doc=mysql_fetch_array($resultat_chercher_doc);
?>
Ligne insérée : 
<?php
echo $ligne_chercher_doc['RbDate_DataRub']; //on affiche le resultat de la MAJ
 
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<HTML>
<HEAD>
	<META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=UTF-8">
  <META NAME="GENERATOR" CONTENT="PHP">
  <TITLE>MedWebTux - Intégration d'un document rtf</TITLE>
  <script language="JavaScript">
<!--
function donner_focus(chp)
{
document.getElementById(chp).focus();
}
-->
</script>
<script language="javascript">
function chercher_patient()
// on ouvre dans une fenêtre le fichier passé en paramètre.
{ window.open('../recherche_patient_agenda.php','Choisir','width=700,height=450,top=50,left=50,toolbar=yes, scrollbars=yes, location=no'); }
</script>

</head>
<body>
<h1>Lecture d'un fichier rtf</h1>
Cette page permet d'intégrer un fichier rtf comme document de MedinTux après conversion de format en html.
<br><a href="#" onclick="chercher_patient()">Choisir un patient</a>
  <form ACTION="test_file.php" METHOD="POST" name="form_jour" ENCTYPE="multipart/form-data">
    <input name="GUID" id="GUID" type="hidden" value="" />
    <input name="RDV_PrisAvec" type="hidden" value="<?php echo $user ?>" />
	<table><tr>
		<th><label for="Nom">Nom : </label></th>
		<td><INPUT NAME="Nom" id="Nom" TYPE="text" value="" SIZE=20 maxlength=40 /></td>
	</tr>
	<tr>
		<th><label for="Prenom">Pr&eacute;nom : </label></th>
		<td><INPUT NAME="Prenom" id="Prenom" TYPE="text" value="" SIZE=20 maxlength=40 /></td>
	</tr>
	<tr>
		<td><b><label for="Tel">T&eacute;l&eacute;phone&nbsp;: </label></b></td>
		<td><INPUT NAME="Tel" id="Tel" TYPE="text" value="" SIZE=10 maxlength=10 /></td>
	</tr>
</table>
Document rtf à intégrer : 
	    <input type="hidden" name="MAX_FILE_SIZE" value="300000000" />
	    <INPUT type="file" name="selection">
	    <INPUT NAME="envoyer" TYPE="submit" VALUE="Importer" title="Ouvrir un fichier rtf"/>
	  </form>
<?php
/*$sql_update_obs_194=sprintf("UPDATE rubriquesblobs SET RbDate_DataRub='%s' WHERE RbDate_PrimKey='194'",mysql_real_escape_string($contents));
$resultat=mysql_query($sql_update_obs_194);
$sql_chercher_doc_194="SELECT * FROM rubriquesblobs WHERE RbDate_PrimKey='194'";
$resultat_chercher_doc_194=mysql_query($sql_chercher_doc_194);
$ligne_chercher_doc_194=mysql_fetch_array($resultat_chercher_doc_194);
echo $ligne_chercher_doc_194['RbDate_DataRub']; //on affiche le resultat de la MAJ
} 
*/
?>
</body>
</html>