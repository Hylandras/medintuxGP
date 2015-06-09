<?php
include("../config.php");
//fichier experimental pour inserer une image BLOB dans MedinTux
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");
if(!$db)
{
	print "Erreur de connexion &agrave; $db<br />";
	exit;
}
// on choisit la bonne base
if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br />";
	mysql_close($db);
	exit;
}
$GUID='72b472e3-72b2-49ce-9346-73b865ee9f99';
$date=date('Y-m-d', date('U'));
$date_complete=$date.' 00:00:00';
$user='delafond';
$begin="<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>
<MedinTux_Multimedia_data>
    <HTML_Data>
<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body>
<p align=\"center\"><img src=\"\">
</body></html>
    </HTML_Data>
    <ImageListe>
      <base64>";
$end="</base64>
    </ImageListe>
</MedinTux_Multimedia_data>";
$type="20080000";
$titre_observation="Image";

function inserer_document($db,$GUID,$date_complete,$user,$begin,$end,$type,$titre_observation)
{
  $file=$_FILES['nom_du_fichier']['name'];
//  $description=$_REQUEST['description'];
  if ($_FILES['nom_du_fichier']['error']) //traitement des erreurs
  {
    switch ($_FILES['nom_du_fichier']['error'])
    {
      case 1: // UPLOAD_ERR_INI_SIZE
      echo "Le fichier dépasse la limite autorisée par le serveur (fichier php.ini) !";
      break;
      case 2: // UPLOAD_ERR_FORM_SIZE
      echo "Le fichier dépasse la limite autorisée dans le formulaire HTML !";
      break;
      case 3: // UPLOAD_ERR_PARTIAL
      echo "L'envoi du fichier a été interrompu pendant le transfert !";
      break;
      case 4: // UPLOAD_ERR_NO_FILE
      echo "Le fichier que vous avez envoyé a une taille nulle !";
      break;
    }
  }
  else //on ne fait les operations que si pas d'erreur
  {
echo "fichier ".$file." bien reçu !";
echo $_FILES['nom_du_fichier']['tmp_name'];
$contents = file_get_contents($_FILES['nom_du_fichier']['tmp_name']);
//$handle=fopen($file,'r');
//$contents = fread($handle, filesize($file));
echo $contents;
$base64=base64_encode ($contents);
echo $base64;
$page=$begin.$base64.$end;
$sql_inserer_blob="INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) values ('$page','$GUID')";
$resultat_inserer_blob=mysql_query($sql_inserer_blob);
  $id_blob=mysql_insert_id();
//on insere l'en-tete
  $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$GUID','$type','%s','$date_complete','$user','$user','$id_blob')",mysql_real_escape_string($titre_observation));
  $resultat_insert_header=mysql_query($sql_insert_header,$db);
  }
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<HTML>
<HEAD>
	<META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=UTF-8">
  <META NAME="GENERATOR" CONTENT="PHP">
  <TITLE>MedWebTux - Insertion d'image</TITLE>

<?php
function choisir_document()
{
?>
<form action="insertion_image.php" method="POST" ENCTYPE="multipart/form-data">
<fieldset>
<legend >Ajouter un document</legend>
  <INPUT type=hidden name=MAX_FILE_SIZE  VALUE=2048000 />
  <INPUT type=file name="nom_du_fichier" />
  <INPUT name="bouton_choisir_document" type=submit value="Envoyer" />
</fieldset>
</form>
<?php
}
choisir_document();
inserer_document($db,$GUID,$date_complete,$user,$begin,$end,$type,$titre_observation);
?>
</body>
</html>