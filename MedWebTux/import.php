<?php
session_start() ;
//page non affichable fabriquant une fiche à partir d'un fichier xml précédemment exporté, avec conservation du GUID
include("config.php");
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

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];

//$user=$_SESSION['login'];

if (isset($_FILES['selection'])) 
{
  $selection = $_FILES['selection']['tmp_name'];
}
else
{
  header('location: index.php?page=liste' );
  exit;
}
/*
if ($fiche_xml=fopen($selection,r))
*/

if ($contents=simplexml_load_file($selection))
{//var_dump($contents);
  //print_r($contents);
  //$simpleXml = new SimpleXMLElement($contents);
  //simplexml_load_string pour les documents

  //On cherche le GUID

  $FchGnrl_IDDos=$contents->identite->FchGnrl_IDDos;
  //echo $FchGnrl_IDDos;
  //on verifie que cette fiche n'existe pas deja avant de l'inserer
  $sql_verif_doublon="SELECT FchGnrl_IDDos FROM IndexNomPrenom WHERE FchGnrl_IDDos='$FchGnrl_IDDos'";
  $resultat_verif_doublon=mysql_query($sql_verif_doublon,$db);
  if (mysql_num_rows($resultat_verif_doublon))
  {
    header('location: liste.php?message=message_doublon' );
    exit;
  }

  /*Pour info, Les remplacements faits a l'export
  $pattern[0]='/&lt;/';
  $pattern[1]='/&/';
  $pattern[2]='/</';
  $pattern[3]='^@';
  $replacement[0]='&amp;lt;';
  $replacement[1]='&amp;';
  $replacement[2]='&lt;';
  $replacement[2]='';
  */
  //Remplacements pour restaurer la fiche comme elle etait avant les transformations de l'export pour eviter les caracteres interdits < et &
  $pattern[0]='`/&lt;/`';
  $pattern[1]='`/&amp;/`';
  $pattern[2]='`/&amp;lt;/`';
  $pattern[3]='`/\\0/`';
  $replacement[0]='`<`';
  $replacement[1]='`&`';
  $replacement[2]='`&lt;`';
  $replacement[3]='`\0`';

  $FchGnrl_NomDos= preg_replace($pattern,$replacement,$contents->identite->FchGnrl_NomDos);
  $FchGnrl_Prenom= preg_replace($pattern,$replacement,$contents->identite->FchGnrl_Prenom);

  $sql_index=sprintf("INSERT INTO IndexNomPrenom (FchGnrl_IDDos ,	FchGnrl_NomDos ,FchGnrl_Prenom) VALUES ('$FchGnrl_IDDos','%s','%s')",mysql_real_escape_string($FchGnrl_NomDos),mysql_real_escape_string($FchGnrl_Prenom));
  //test
  //echo $sql_index."<br>";
  $resultat_index=mysql_query($sql_index,$db);
  $FchPat_RefPk=mysql_insert_id();

  $FchPat_NomFille= preg_replace($pattern,$replacement,$contents->identite->FchPat_NomFille);
  $FchPat_Nee= preg_replace($pattern,$replacement,$contents->identite->FchPat_Nee);
  $FchPat_Sexe= preg_replace($pattern,$replacement,$contents->identite->FchPat_Sexe);
  $FchPat_Adresse= preg_replace($pattern,$replacement,$contents->identite->FchPat_Adresse);
  $FchPat_CP= preg_replace($pattern,$replacement,$contents->identite->FchPat_CP);
  $FchPat_Ville= preg_replace($pattern,$replacement,$contents->identite->FchPat_Ville);
  $FchPat_Tel1= preg_replace($pattern,$replacement,$contents->identite->FchPat_Tel1);
  $FchPat_Tel2= preg_replace($pattern,$replacement,$contents->identite->FchPat_Tel2);
  $FchPat_Tel3= preg_replace($pattern,$replacement,$contents->identite->FchPat_Tel3);
  $FchPat_Email= preg_replace($pattern,$replacement,$contents->identite->FchPat_Email);
  $FchPat_NumSS= preg_replace($pattern,$replacement,$contents->identite->FchPat_NumSS);
  $FchPat_PatientAss= preg_replace($pattern,$replacement,$contents->identite->FchPat_PatientAss);
  $FchPat_NomAss= preg_replace($pattern,$replacement,$contents->identite->FchPat_NomAss);
  $FchPat_PrenomAss= preg_replace($pattern,$replacement,$contents->identite->FchPat_PrenomAss);
  $FchPat_Profession= preg_replace($pattern,$replacement,$contents->identite->FchPat_Profession);
  $FchPat_Titre= preg_replace($pattern,$replacement,$contents->identite->FchPat_Titre);
  $FchPat_Geme= preg_replace($pattern,$replacement,$contents->identite->FchPat_Geme);
  $fchpat_Note_Html= preg_replace($pattern,$replacement,$contents->identite->fchpat_Note_Html);

  $sql_fchpat=sprintf("INSERT INTO fchpat (FchPat_GUID_Doss, FchPat_NomFille, FchPat_Nee, FchPat_Sexe, FchPat_Adresse, FchPat_CP, FchPat_Ville, FchPat_Tel1 ,FchPat_Tel2, 	FchPat_Tel3, FchPat_Email, FchPat_NumSS, FchPat_PatientAss, FchPat_NomAss, FchPat_PrenomAss ,FchPat_Profession, FchPat_Titre, FchPat_RefPk, FchPat_Geme) VALUES ('$FchGnrl_IDDos', '%s', '$FchPat_Nee', '$FchPat_Sexe', '%s', '$FchPat_CP', '%s', '$FchPat_Tel1' ,'$FchPat_Tel2', '$FchPat_Tel3', '$FchPat_Email', '$FchPat_NumSS','$FchPat_PatientAss', '%s', '%s' ,'%s', '%s', '$FchPat_RefPk','$FchPat_Geme')",mysql_real_escape_string($FchPat_NomFille),mysql_real_escape_string($FchPat_Adresse),mysql_real_escape_string($FchPat_Ville),mysql_real_escape_string($FchPat_NomAss),mysql_real_escape_string($FchPat_PrenomAss),mysql_real_escape_string($FchPat_Profession),mysql_real_escape_string($FchPat_Titre));
  //echo $sql_fchpat;
  $resultat_fchpat=mysql_query($sql_fchpat,$db);

  $sql_notes=sprintf("INSERT INTO fchpat_Note (fchpat_Note_PatGUID,fchpat_Note_PatPK,fchpat_Note_Html) VALUES ('$FchGnrl_IDDos[0]','$FchPat_RefPk','%s')",mysql_real_escape_string($fchpat_Note_Html));
  //echo $sql_notes;
  $resultat_notes=mysql_query($sql_notes,$db);

  //boucle pour recuperer les documents
  $doc_cnt = count($contents->documents->document); 
  //echo "count_doc =".$doc_cnt; //marche
  for($i = 0; $i < $doc_cnt; $i++) 
  {
    $RbDate_DataRub=preg_replace($pattern,$replacement,$contents->documents->document[$i]->RbDate_DataRub);
    $RbDate_TypeRub=$contents->documents->document[$i]->RbDate_TypeRub;
    $RbDate_NomDate=preg_replace($pattern,$replacement,$contents->documents->document[$i]->RbDate_NomDate);
    $RbDate_SubTypeRub=preg_replace($pattern,$replacement,$contents->documents->document[$i]->RbDate_SubTypeRub);
    $RbDate_Date=$contents->documents->document[$i]->RbDate_Date;
    $RbDate_CreateUser=preg_replace($pattern,$replacement,$contents->documents->document[$i]->RbDate_CreateUser);
    //$RbDate_CreateSignUser=element_set('RbDate_CreateSignUser',$ce_document);
    $RbDate_Fin=preg_replace($pattern,$replacement,$contents->documents->document[$i]->RbDate_Fin);
    $RbDate_DureeMod=preg_replace($pattern,$replacement,$contents->documents->document[$i]->RbDate_DureeMod);

    $sql_blobs=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$FchGnrl_IDDos')",mysql_real_escape_string($RbDate_DataRub));
    $resultat_blobs=mysql_query($sql_blobs,$db);
    //test
    //echo $sql_blobs."<br>";
    $Ref_blob=mysql_insert_id();
    //On utilise l'utilisateur principal exportateur comme utilisateur delegue de l'import et l'utilisateur qui importe comme utilisateur principal
    $sql_head=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_SubTypeRub,RbDate_Date,	RbDate_CreateUser,RbDate_CreateSignUser,RbDate_Fin,RbDate_DureeMod, RbDate_Ref_NumDoss,	RbDate_RefBlobs_PrimKey) VALUES ('$FchGnrl_IDDos','$RbDate_TypeRub','%s','$RbDate_SubTypeRub','$RbDate_Date','$RbDate_CreateUser','$user','$RbDate_Fin','$RbDate_DureeMod','$FchPat_RefPk','$Ref_blob')",mysql_real_escape_string($RbDate_NomDate));
    $resultat_head=mysql_query($sql_head,$db);
  } //fin de la boucle des documents
}//fin de la tentative d'ouverture du fichier xml
else
  echo "Fichier introuvable";

//on retourne sur la fiche qu'on vient de créer
header ('location:patient.php?GUID='.$FchGnrl_IDDos[0]);
exit;
?>