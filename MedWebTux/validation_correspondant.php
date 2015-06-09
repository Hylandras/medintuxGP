<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=agenda' );
  exit;
}
$user=$_SESSION['login'];
include("config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$base))
{
echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />";
echo "<div class=\"erreur\">
<b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
<a href=\"setup/index.php\">Renseigner les donn&eacute;es de connexion au serveur Mysql</a>
</div>";
	@rename("set_up", "setup");
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
//le mode vcard
if (isset($_FILES['selection'])) 
{
  if ($selection = $_FILES['selection']['tmp_name'])
    echo "";
  else
    echo "echec de FILES<br>"; //pas bon :-(
  if ($fichier=fopen($selection,"r")) //ouverture du fichier renvoye par l'interface
    echo ""; 
  else
    echo "echec fichier <br />";
  $filename_components=explode('.',$_FILES['selection']['name']);
  $contents=fread( $fichier,filesize($selection));
  $line=explode("\n",$contents);
  foreach ($line AS $this_line)
  {
    if ($this_line!='')
    {
    //correction pour apicrypt : on remplace le deux points par un point virgule sur la ligne nom
      $this_line=preg_replace('`^N:`','N;',$this_line);
      $encodage="";
      if (preg_match ('`BEGIN:`',$this_line))
      {
	$new_file=1;
      }
      elseif (preg_match ("`END:`",$this_line))
      {
	$new_file=0;
      }
      if ($new_file)
      {
	$elements=array_map('trim',explode(";",$this_line));
	$zero=$elements[0]; //la cle de debut de ligne
	if ($zero=="N")
	{
	  $titre='';
	  if (isset($elements[4]))
	    $titre=$elements[4];
	  $nom_long=$elements[1]; //champ qui suit la cle de debut de ligne
	  if (preg_match('`:`',$nom_long))
	  {
	    $nom_tableau=explode(":",$nom_long); // separe l'encodage
	    $encodage=strtoupper($nom_tableau[0]);
	    if ($encodage=='UTF-8')
	      $nom=$nom_tableau[1];
	    else
	      $nom=utf8_encode($nom_tableau[1]);
	  }
	  else //encodage absent
	    $nom=$nom_long;
	  if ($encodage=='UTF-8')
	    $prenom=$elements[2];
	  else
	    $prenom=utf8_encode($elements[2]);
	}
	elseif ($zero=="TITLE")
	{
	  $specialite_long=$elements[1];
	  $specialite_tableau=explode(":",$specialite_long);
	  $specialite=$specialite_tableau[1];
	}
	elseif ($zero=="ADR")
	{
	  $address=$elements[3];
	  $town=$elements[4]; //region = champ 5
	  $zipcode=$elements[6];
	}
	elseif ($zero=="TEL")
	{
	  $tel=explode(":",$elements[1]);
	}
	elseif ($zero=="EMAIL")
	{
	  $email=explode(":",$elements[2]);
	}
	elseif ($zero=="NOTE")
	{
	  $note=explode(":",$elements[1]);
	}
      }
      if ($new_file==0 AND $nom) //ne pas inserer fiche si nom vide
      {
	$GUID=uuid();
	$sql_insert_file=sprintf("INSERT INTO Personnes (GUID,Nom,Prenom,Adresse,CodePostal,Ville,Tel_1,EMail,Note,Qualite,Titre) VALUES ('$GUID','%s','%s','%s','$zipcode','%s','$tel[1]','$email[1]','%s','%s','%s')",mysqli_real_escape_string($db,$nom),mysqli_real_escape_string($db,$prenom),mysqli_real_escape_string($db,$address),mysqli_real_escape_string($db,$town),mysqli_real_escape_string($db,$note[1]),mysqli_real_escape_string($db,$specialite),mysqli_real_escape_string($db,$titre));
	$resultat_insert_file=mysqli_query($db,$sql_insert_file);
      }
    }
  }
//  header ('location:correspondant.php');
//  exit;
}

//mode suppression
$confirmer=$_GET['confirmer'];
$Nom=$_GET['Nom'];
if ($confirmer=="Conserver")
{
  header('location: correspondant.php?&critere_recherche=Nom&cle='.$Nom.'&envoyer=Chercher&intervenant_user[]=users&intervenant_user[]=no_users');
  exit;
}
elseif ($confirmer=="Supprimer") //suppression de fiche apres confirmation
{
  $ID_PrimKey=$_GET['ID_PrimKey'];
  $sql_supprimer_liaisons="DELETE FROM fchpat_Intervenants WHERE fchpat_Intervenants_PK='$ID_PrimKey'";
  $resultat_supprimer_liaisons=mysqli_query($db,$sql_supprimer_liaisons);
  $sql_supprimer_fiche_corresp="DELETE FROM Personnes WHERE ID_PrimKey='$ID_PrimKey'";
  $resultat_supprimer_fiche_corresp=mysqli_query($db,$sql_supprimer_fiche_corresp);
  header('location: correspondant.php?&critere_recherche=Nom&cle='.$Nom.'&envoyer=Chercher&intervenant_user[]=users&intervenant_user[]=no_users');
  exit;
}
$envoyer=$_GET['envoyer'];
$nom_corresp=$_GET['nom_corresp'];
$prenom_corresp=$_GET['prenom_corresp'];
$titre_corresp=$_GET['titre_corresp'];
$ordre_corresp=$_GET['ordre_corresp'];
$rpps_corresp=$_GET['rpps_corresp'];
$Adresse_corresp=$_GET['Adresse_corresp'];
$CP=$_GET['CP'];
$Ville=$_GET['Ville'];
$mail_corresp=$_GET['mail_corresp'];
$tel1_corresp=str_replace(" ",'',$_GET['tel1_corresp']);
$tel2_corresp=str_replace(" ",'',$_GET['tel2_corresp']);
$tel3_corresp=str_replace(" ",'',$_GET['tel3_corresp']);	
$tel1_abr_corresp=$_GET['tel1_abr_corresp'];
$tel2_abr_corresp=$_GET['tel2_abr_corresp'];
$tel3_abr_corresp=$_GET['tel3_abr_corresp'];

$liste_type_tel1=$_GET['liste_type_tel1'];

if ($liste_type_tel1=="Autre")
  $tel1_type_corresp=$_GET['tel1_type_corresp'];
else
  $tel1_type_corresp=$liste_type_tel1;

$liste_type_tel2=$_GET['liste_type_tel2'];

if ($liste_type_tel2=="Autre")
  $tel2_type_corresp=$_GET['tel2_type_corresp'];
else
  $tel2_type_corresp=$liste_type_tel2;

$liste_type_tel3=$_GET['liste_type_tel3'];

if ($liste_type_tel3=="Autre")
  $tel3_type_corresp=$_GET['tel3_type_corresp'];
else
  $tel3_type_corresp=$liste_type_tel3;

$sexe_intervenant=$_GET['sexe_intervenant'];
$politesse=$_GET['politesse'];
$Notes_corresp=$_GET['Notes_corresp'];

$liste_spe=$_GET['liste_spe']; //On recupere la specialite ou Autre

if ($liste_spe=="Autre")
  $specialite=$_GET['specialite']; //On recupere la nouvelle specialite si Autre
else
  $specialite=$liste_spe;

if ($envoyer=="Modifier")
{
  $ID_PrimKey=$_GET['ID_PrimKey'];

  $sql_modifier=sprintf("UPDATE Personnes SET Nom='%s',Prenom='%s',Adresse='%s',CodePostal='$CP',Ville='%s',NumOrdre='$ordre_corresp',NumRPPS='$rpps_corresp',Tel_1='$tel1_corresp',Tel_2='$tel2_corresp',Tel_3='$tel3_corresp',Tel_Type1='$tel1_type_corresp',Tel_Type2='$tel2_type_corresp',Tel_Type3='$tel3_type_corresp',Tel_Abr_1='$tel1_abr_corresp',Tel_Abr_2='$tel2_abr_corresp',Tel_Abr_3='$tel3_abr_corresp',Email='$mail_corresp',Qualite='%s',Titre='$titre_corresp',Note='%s',Sexe='$sexe_intervenant',Cher='%s' WHERE ID_PrimKey='$ID_PrimKey' ",mysqli_real_escape_string($db,$nom_corresp),mysqli_real_escape_string($db,$prenom_corresp),mysqli_real_escape_string($db,$Adresse_corresp),mysqli_real_escape_string($db,$Ville),mysqli_real_escape_string($db,$specialite),mysqli_real_escape_string($db,$Notes_corresp),mysqli_real_escape_string($db,$politesse));
  $resultat_modifier=mysqli_query($db,$sql_modifier);

  header('location:fiche_intervenant.php?intervenant='.$ID_PrimKey );
  exit;
}
elseif ($envoyer=="Ajouter")
{
  $GUID=uuid();

  $sql_ajouter=sprintf("INSERT INTO Personnes (GUID,Sexe,NumOrdre,NumRPPS,Nom,Prenom,Adresse,CodePostal,Ville,Cher,Tel_1,Tel_2,Tel_3,EMail,Note,Qualite,Titre,Tel_Type1,Tel_Type2,Tel_Type3,Tel_Abr_1,Tel_Abr_2,Tel_Abr_3) VALUES ('$GUID','$sexe_intervenant','$ordre_corresp','$rpps_corresp','%s','%s','%s','$CP','%s','%s','$tel1_corresp','$tel2_corresp','$tel3_corresp','$mail_corresp','%s','%s','$titre_corresp','$tel1_type_corresp','$tel2_type_corresp','$tel3_type_corresp','$tel1_abr_corresp','$tel2_abr_corresp','$tel3_abr_corresp')",mysqli_real_escape_string($db,$nom_corresp),mysqli_real_escape_string($db,$prenom_corresp),mysqli_real_escape_string($db,$Adresse_corresp),mysqli_real_escape_string($db,$Ville),mysqli_real_escape_string($db,$politesse),mysqli_real_escape_string($db,$Notes_corresp),mysqli_real_escape_string($db,$specialite));

  $resultat_ajouter=mysqli_query($db,$sql_ajouter);

  $id=mysqli_insert_id($db); //recuperation de l'ID de la fiche qui vient d'etre inseree
  
  header('location:fiche_intervenant.php?intervenant='.$id );
  exit;
}
?>
