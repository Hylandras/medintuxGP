<?php
session_start() ;
//page non affichable fabriquant un fichier d'export xml en vue du transfert de dossier dant un autre fichier, avec conservation du GUID.
include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; $host<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];

//Recherche si l'utilisateur a des droits admin
$sql_droit_admin="SELECT Droits FROM Personnes WHERE Login='$user'";
$resultat_droit_admin=mysqli_query($db,$sql_droit_admin);
$ligne_droit_admin=mysqli_fetch_array($resultat_droit_admin);

if (isset($_GET['numeroID']))
{
  $patient=$_GET['numeroID'];
  if (!fopen ('log/test','w')) //on fait un test d'ecriture. Si marche pas, on revoie un message d'erreur
  {
    header('location: patient.php?GUID='.$patient.'&message=message_pas_dir_pour_export' );
    exit;
  }
  if (!stripos($ligne_droit_admin["Droits"],"adm")) //si non admin, on revient sur la fiche patient avec un message d'erreur
  {
    header('location: patient.php?GUID='.$patient.'&message=message_pas_admin_pour_export' );
    exit;
  }
//On cherche dans la table IndexNomPrenom la fiche dont l'ID a ete demande
  $sql_chercher_nom="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
  $resultat_chercher_nom=mysqli_query($db,$sql_chercher_nom);
  $ligne_chercher_nom=mysqli_fetch_array($resultat_chercher_nom);
//ON CHERCHE LES DONNES COMPLEMENTAIRES d'identite
  $sql_chercher_fchpat="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$patient'";
  $resultat_chercher_fchpat=mysqli_query($db,$sql_chercher_fchpat);
  $ligne_chercher_fchpat=mysqli_fetch_array($resultat_chercher_fchpat);

  //On cherche les notes
  $sql_notes="SELECT * FROM fchpat_Note WHERE fchpat_Note_PatGUID='$patient'";
  $resultat_notes=mysqli_query($db,$sql_notes);
  $ligne_notes=mysqli_fetch_array($resultat_notes);

  //on cherche les observations, etc.
  $sql_documents="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesBlobs.RbDate_IDDos='$patient'";
  $resultat_documents=mysqli_query($db,$sql_documents);
}
else //S'il manque le patient, on est redirige automatiquement sur la page de recherche des patients
{
  header('location: liste.php' );
  exit;
}
$pattern[0]='/&lt;/';
$pattern[1]='/&/';
$pattern[2]='/</';
$pattern[3]='/\\0/';
$replacement[0]='&amp;lt;';
$replacement[1]='&amp;';
$replacement[2]='&lt;';
$replacement[3]='\\\\0';
//On trouve toutes les donnees uniques
//on remplace les sigles interdits par le xml dans les notes
$dossier="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n
<fiche>\n
<identite>\n
<FchGnrl_IDDos>".$patient."</FchGnrl_IDDos>\n
<FchGnrl_NomDos>".preg_replace($pattern,$replacement,$ligne_chercher_nom["FchGnrl_NomDos"])."</FchGnrl_NomDos>\n
<FchGnrl_Prenom>".preg_replace($pattern,$replacement,$ligne_chercher_nom["FchGnrl_Prenom"])."</FchGnrl_Prenom>\n
<FchPat_NomFille>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_NomFille'])."</FchPat_NomFille>\n
<FchPat_Nee>".$ligne_chercher_fchpat['FchPat_Nee']."</FchPat_Nee>\n
<FchPat_Sexe>".$ligne_chercher_fchpat['FchPat_Sexe']."</FchPat_Sexe>\n
<FchPat_Adresse>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Adresse'])."</FchPat_Adresse>\n
<FchPat_CP>".$ligne_chercher_fchpat['FchPat_CP']."</FchPat_CP>\n
<FchPat_Ville>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Ville'])."</FchPat_Ville>\n
<FchPat_Tel1>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Tel1'])."</FchPat_Tel1>\n
<FchPat_Tel2>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Tel2'])."</FchPat_Tel2>\n
<FchPat_Tel3>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Tel3'])."</FchPat_Tel3>\n
<FchPat_Email>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Email'])."</FchPat_Email>\n
<FchPat_NumSS>".$ligne_chercher_fchpat['FchPat_NumSS']."</FchPat_NumSS>\n
<FchPat_PatientAss>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_PatientAss'])."</FchPat_PatientAss>\n
<FchPat_NomAss>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_NomAss'])."</FchPat_NomAss>\n
<FchPat_PrenomAss>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_PrenomAss'])."</FchPat_PrenomAss>\n
<FchPat_Profession>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Profession'])."</FchPat_Profession>\n
<FchPat_Titre>".preg_replace($pattern,$replacement,$ligne_chercher_fchpat['FchPat_Titre'])."</FchPat_Titre>\n
<FchPat_Geme>".$ligne_chercher_fchpat['FchPat_Geme']."</FchPat_Geme>\n
<fchpat_Note_Html>".preg_replace($pattern,$replacement,$ligne_notes['fchpat_Note_Html'])."</fchpat_Note_Html>\n
</identite>\n
<documents>\n";
//On trouve tous les documents
while ($ligne_documents=mysqli_fetch_array($resultat_documents))
{
//on transforme les $lt; en &amp;lt;, les < en &lt; et les & en &amp; dans les blobs;
//On fait la manoeuvre inverse a l'importation
  $dossier=$dossier."<document>\n
  <RbDate_DataRub>".preg_replace($pattern,$replacement,$ligne_documents['RbDate_DataRub'])."</RbDate_DataRub>\n
  <RbDate_TypeRub>".$ligne_documents['RbDate_TypeRub']."</RbDate_TypeRub>\n
  <RbDate_NomDate>".$ligne_documents['RbDate_NomDate']."</RbDate_NomDate>\n
  <RbDate_SubTypeRub>".$ligne_documents['RbDate_SubTypeRub']."</RbDate_SubTypeRub>\n
  <RbDate_Date>".$ligne_documents['RbDate_Date']."</RbDate_Date>\n
  <RbDate_CreateUser>".$ligne_documents['RbDate_CreateUser']."</RbDate_CreateUser>\n
  <RbDate_CreateSignUser>".$ligne_documents['RbDate_CreateSignUser']."</RbDate_CreateSignUser>\n
  <RbDate_Fin>".$ligne_documents['RbDate_Fin']."</RbDate_Fin>\n
  <RbDate_DureeMod>".$ligne_documents['RbDate_DureeMod']."</RbDate_DureeMod>\n
  </document>\n";
}
$dossier=$dossier."</documents></fiche>";

if ($fiche=fopen("log/patient".$patient.".xml",'w')) //on cree ou on ouvre un fichier au numero du patient
{
  fwrite($fiche,$dossier);
  fclose($fiche);
}
//On revient a la fiche avec un message apres insertion
header('location: patient.php?GUID='.$patient.'&export=ok');
exit;
?>
