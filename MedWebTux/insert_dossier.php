<?php
session_start() ;
//page non affichable. Cree un nouveau dossier et renvoie sur le dossier cree
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=nouveau_dossier' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$utilisateur_autorisant=$tab_login[1];

include("config.php");
//variables a initialiser pour l'include de templates - ne serviront pas
$naissance='';
$adresse='';
$secu='';
$profession='';
$sexe='';
$genre[]='';
$nom='';
$prenom='';
$date='';
$titre='';
$sexe='M';
$genre['M']='';

include("templates.php"); //pour le terrain

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br>";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}
function uuid() //génération de nombres aleatoires
{
  return sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
    mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ),
    mt_rand( 0, 0x0fff ) | 0x4000,
    mt_rand( 0, 0x3fff ) | 0x8000,
    mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ) );
}
function fullUpper($string){
  return strtr(strtoupper($string), array(
      "à" => "À",
      "è" => "È",
      "ì" => "Ì",
      "ò" => "Ò",
      "ù" => "Ù",
          "á" => "Á",
      "é" => "É",
      "í" => "Í",
      "ó" => "Ó",
      "ú" => "Ú",
          "â" => "Â",
      "ê" => "Ê",
      "î" => "Î",
      "ô" => "Ô",
      "û" => "Û",
          "ç" => "Ç",
    ));
}

function fixBirthDate($birthdate) //corriger certains formats de date de naissance
{
$find=array('/',' ');//remplacer les mauvais séparateurs
$replace=array('-','-');
$birthdate=str_replace ($find,$replace,$birthdate);
return $birthdate;
}

if (isset($_REQUEST['ID_PrimKey']))
  $ID_PrimKey=$_REQUEST['ID_PrimKey'];
if (isset($_REQUEST['Nom']))
  $Nom=fullUpper($_REQUEST['Nom']);
if (isset($_REQUEST['Prenom']))
  $Prenom=fullUpper($_REQUEST['Prenom']);
if (isset($_REQUEST['NomJF']))
  $NomJF=fullUpper($_REQUEST['NomJF']);
if (isset($_REQUEST['naissance']))
  $Date=fixBirthDate($_REQUEST['naissance']);
$list_date=explode ("-",$Date);
if ($date_format=='fr')
{
//on repasse en iso les dates qui arrivent en francais
  $Date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
}
elseif ($date_format=='en')
  $Date=$list_date[2].'-'.$list_date[0].'-'.$list_date[1];

  $DN_longue=str_replace(" ","",$Date)." 00:00:00";

if (isset($_REQUEST['sexe']))
  $sexe=$_REQUEST['sexe'];
if (isset($_REQUEST['gem']))
  $gem=$_REQUEST['gem'];
if (isset($_REQUEST['Tel1']))
  $Tel1=$_REQUEST['Tel1'];
$Tel1=str_replace(" ","",$Tel1);
if (isset($_REQUEST['Tel2']))
  $Tel2=$_REQUEST['Tel2'];
$Tel2=str_replace(" ","",$Tel2);
if (isset($_REQUEST['Tel3']))
  $Tel3=$_REQUEST['Tel3'];
$Tel3=str_replace(" ","",$Tel3);
if (isset($_REQUEST['Profession']))
  $Profession=$_REQUEST['Profession'];
if (isset($_REQUEST['Adresse']))
  $Adresse=$_REQUEST['Adresse'];
if (isset($_REQUEST['CP']))
  $CP=$_REQUEST['CP'];
if (isset($_REQUEST['Ville']))
  $Ville=$_REQUEST['Ville'];
if (isset($_REQUEST['Secu']))
{
  $Secu=$_REQUEST['Secu'];
$Secu=str_replace(" ","",$Secu);
}
if (isset($_REQUEST['cle_secu']))
{
  $cle_secu=$_REQUEST['cle_secu'];
  $Secu=$Secu.$cle_secu;
}
if (isset($_REQUEST['pas_patient']))
{
  $pas_patient="0"; //case cochee
}
else
  $pas_patient="1";
if (isset($_REQUEST['email']))
  $email=$_REQUEST['email'];
if (isset($_REQUEST['nom_assure']))
  $nom_assure=$_REQUEST['nom_assure'];
if (isset($_REQUEST['prenom_assure']))
  $prenom_assure=$_REQUEST['prenom_assure'];
if (isset($_REQUEST['titre']))
  $titre=$_REQUEST['titre'];
if (isset($_REQUEST['Note']))
  $Note=$_REQUEST['Note'];
if (isset($_REQUEST['Envoyer']))
  $Envoyer=$_REQUEST['Envoyer'];

if ($Envoyer=="Ajouter")//Execution de la creation de fiche
{
  $sql_chercher_doublon=sprintf("SELECT * FROM IndexNomPrenom INNER JOIN fchpat ON IndexNomPrenom.FchGnrl_IDDos=fchpat.FchPat_GUID_Doss WHERE FchGnrl_Prenom='%s' AND FchGnrl_NomDos='%s' AND FchPat_Nee='$DN_longue'",mysqli_real_escape_string($db,$Prenom),mysqli_real_escape_string($db,$Nom));
  $resultat_chercher_doublon=mysqli_query($db,$sql_chercher_doublon);
  $count_chercher_doublon=mysqli_num_rows($resultat_chercher_doublon);
  if ($count_chercher_doublon) //s'il existe un doublon, on ne cree pas la fiche et on va directement dessus
  {
    $ligne_chercher_doublon=mysqli_fetch_array($resultat_chercher_doublon);
    header ("location:frame_patient.php?GUID=".$ligne_chercher_doublon['FchGnrl_IDDos']);
    exit;
  }
  $GUID=uuid(); //On fabrique un nouveau GUID
  $sql1=sprintf("INSERT INTO IndexNomPrenom (FchGnrl_IDDos,FchGnrl_NomDos,FchGnrl_Prenom) VALUES ('$GUID','%s','%s')",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom)); //insertion de nom-prenom
  $resultat1=mysqli_query($db,$sql1);
  $id=mysqli_insert_id($db); //recuperation de l'ID de la fiche qui vient d'etre inseree
  
  $sql2=sprintf("INSERT INTO fchpat (FchPat_GUID_Doss,FchPat_NomFille,FchPat_Nee,FchPat_Sexe,FchPat_NbEnfant,FchPat_Adresse,FchPat_CP,FchPat_Ville,FchPat_Cdx,FchPat_Pays,FchPat_Tel1,FchPat_Tel1_Typ,FchPat_Tel2,  	FchPat_Tel2_Typ,FchPat_Tel3,FchPat_Tel3_Typ,FchPat_Email,FchPat_NumSS,FchPat_PatientAss,FchPat_NomAss,FchPat_PrenomAss,FchPat_Profession,FchPat_Titre,FchPat_RefPk,FchPat_Geme) VALUES ('$GUID','%s','$DN_longue','$sexe',NULL,'%s','$CP','%s',NULL,NULL,'$Tel1',NULL,'$Tel2',NULL,'$Tel3',NULL,'$email','$Secu','$pas_patient','%s','%s','%s','%s','$id','$gem')",mysqli_real_escape_string($db,$NomJF),mysqli_real_escape_string($db,$Adresse),mysqli_real_escape_string($db,$Ville),mysqli_real_escape_string($db,$nom_assure),mysqli_real_escape_string($db,$prenom_assure),mysqli_real_escape_string($db,$Profession),mysqli_real_escape_string($db,$titre)); //insertion de l'adresse, etc
  $resultat2=mysqli_query($db,$sql2);

//on insere un terrain vierge
//d'abord le contenu
  $sql_inserer_blob_terrain=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s', '$GUID')",mysqli_real_escape_string($db,$terrain_modele));
  $resultat_inserer_blob_terrain=mysqli_query($db,$sql_inserer_blob_terrain);
  $id_blob_terrain=mysqli_insert_id($db); //on recupere l'ID pour la refiler au Head

  $date=date('Y-m-d H:i:s');
  //puis le titre
  $sql_inserer_titre_terrain="INSERT INTO RubriquesHead ( RbDate_IDDos, RbDate_TypeRub, RbDate_NomDate, RbDate_Date, RbDate_CreateUser,RbDate_CreateSignUser, RbDate_Ref_NumDoss, RbDate_RefBlobs_PrimKey) VALUES ('$GUID' ,20060000, 'Terrain','$date','$user','$utilisateur_autorisant','$id','$id_blob_terrain' )";
  $resultat_inserer_titre_terrain=mysqli_query($db,$sql_inserer_titre_terrain);

  if ($Note) //Pas besoin de creer un enregistrement de notes s'il n'y en a pas 
  {
	  $sql3=sprintf("INSERT INTO fchpat_Note VALUES (NULL,'$GUID','$id','%s')",mysqli_real_escape_string($db,$Note)); //insertion des notes
	  $resultat3=mysqli_query($db,$sql3);
  }

 header('location:frame_patient.php?GUID='.$GUID); //aller directement sur la page du patient
	exit;
}
elseif ($Envoyer=="Modifier")//Execution de la modification de dossier
{
//On met a jour indexnomprenom
  $sql_index=sprintf("UPDATE IndexNomPrenom SET FchGnrl_NomDos='%s',FchGnrl_Prenom='%s' WHERE ID_PrimKey='$ID_PrimKey'",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom));
  $resultat_index=mysqli_query($db,$sql_index);
//on met a jour fchpat
  $sql_fchpat=sprintf("UPDATE fchpat SET FchPat_NomFille='%s',FchPat_Nee='$DN_longue',FchPat_Sexe='$sexe',FchPat_Adresse='%s',FchPat_CP='$CP',FchPat_Ville='%s',FchPat_Tel1='$Tel1',FchPat_Tel2='$Tel2',FchPat_Tel3='$Tel3',FchPat_Email='$email',FchPat_NumSS='$Secu',FchPat_PatientAss='$pas_patient',FchPat_NomAss='%s',FchPat_PrenomAss='%s',FchPat_Profession='%s',FchPat_Titre='%s',FchPat_Geme='$gem' WHERE FchPat_RefPk='$ID_PrimKey'",mysqli_real_escape_string($db,$NomJF),mysqli_real_escape_string($db,$Adresse),mysqli_real_escape_string($db,$Ville),mysqli_real_escape_string($db,$nom_assure),mysqli_real_escape_string($db,$prenom_assure),mysqli_real_escape_string($db,$Profession),mysqli_real_escape_string($db,$titre));
  $resultat_fchpat=mysqli_query($db,$sql_fchpat);

//on verifie que des notes existent pour pouvoir les mettre a jour
  $sql_verif="SELECT * FROM fchpat_Note WHERE fchpat_Note_PatPK='$ID_PrimKey'";
  $resultat_verif=mysqli_query($db,$sql_verif);
  $count_verif=mysqli_num_rows($resultat_verif);
  if ($count_verif)
  {
    $sql_note=sprintf("UPDATE fchpat_Note SET fchpat_Note_Html='%s' WHERE fchpat_Note_PatPK='$ID_PrimKey'",mysqli_real_escape_string($db,$Note));
    $resultat_note=mysqli_query($db,$sql_note);
  }
  else //sinon, on cree l'enregistrement
  {
    if ($Note) //seulement si une note a ete envoyee par l'URL
    {
      $sql_guid="SELECT FchGnrl_IDDos FROM IndexNomPrenom WHERE ID_PrimKey='$ID_PrimKey'";
      $resultat_guid=mysqli_query($db,$sql_guid);
      $liste_guid=mysqli_fetch_array($resultat_guid);
      $GUID=$liste_guid['FchGnrl_IDDos'];
      $sql_note=sprintf("INSERT INTO fchpat_Note values (NULL,'$GUID','$ID_PrimKey','%s')",mysqli_real_escape_string($db,$Note));
      $resultat_note=mysqli_query($db,$sql_note);
    }
  }
  
//enlever_verrou apres modification fiche
  $sql_chercher_GUID="SELECT FchGnrl_IDDos FROM IndexNomPrenom WHERE ID_PrimKey='$ID_PrimKey'";
  $resultat_chercher_GUID=mysqli_query($db,$sql_chercher_GUID);
  $ligne_chercher_GUID=mysqli_fetch_array($resultat_chercher_GUID);

  $GUID_a_enlever=$ligne_chercher_GUID['FchGnrl_IDDos'];

  $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$GUID_a_enlever'";
  $resultat_enlever_verrou=mysqli_query($db,$sql_enlever_verrou);
  header('location:frame_patient.php?GUID='.$GUID_a_enlever);
  exit;
}
if (isset($_REQUEST['confirmer'])) //confirmation d'effacement
	$confirmer=$_REQUEST['confirmer'];
if ($confirmer=="Supprimer")
{
  $UUID=$_REQUEST['UUID'];
    $sql_delete_fchpat="DELETE FROM fchpat WHERE FchPat_RefPk='$ID_PrimKey'"; //Suppression du complement d'identite
  $resultat_delete_fchpat=mysqli_query($db,$sql_delete_fchpat);
    $sql_delete_note="DELETE FROM fchpat_Note WHERE fchpat_Note_PatGUID='$UUID'"; //Suppression des notes
  $resultat_delete_note=mysqli_query($db,$sql_delete_note);
    $sql_delete_head="DELETE FROM RubriquesHead WHERE RbDate_IDDos='$UUID'"; //Suppression des en-tetes de consult
  $resultat_delete_head=mysqli_query($db,$sql_delete_head);
    $sql_delete_blob="DELETE FROM RubriquesBlobs WHERE RbDate_IDDos='$UUID'"; //Suppression des consluts
  $resultat_delete_blob=mysqli_query($db,$sql_delete_blob);
    $sql_delete_corresp="DELETE FROM fchpat_Intervenants WHERE fchpat_Intervenants_PatPK='$ID_PrimKey'"; //Suppression des liens avec les correspondants
  $resultat_delete_corresp=mysqli_query($db,$sql_delete_corresp);
    $sql_delete_index="DELETE FROM IndexNomPrenom WHERE ID_PrimKey='$ID_PrimKey'"; //Suppression de la fiche de base
  $resultat_delete_index=mysqli_query($db,$sql_delete_index);
  header('location: liste.php' );
  exit;
}
elseif ($confirmer=="Conserver")
{
  header('location: liste.php?critere_recherche=FchGnrl_NomDos&cle='.$Nom.'&envoyer=Chercher' ); //renvoie sur la liste avec le nom du patient
  exit;
}
?>