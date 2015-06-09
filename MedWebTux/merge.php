<?php
session_start() ;
/*
Page de resolution des doublons
*/
include("config.php");
if ( !isset( $_SESSION['login'] ) )
{
	header('location: index.php?page=liste' );
	exit;
}
//$user=$_SESSION['login'];
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$sign=$tab_login[1];

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
//Recherche si l'utilisateur a des droits admin
$sql_droit_admin="SELECT Droits,GUID FROM Personnes WHERE Login='$user'";
$resultat_droit_admin=mysqli_query($db,$sql_droit_admin);
$ligne_droit_admin=mysqli_fetch_array($resultat_droit_admin);
if (!preg_match("`adr`",$ligne_droit_admin["Droits"])) //si l'utilisateur n'a pas les droits directs de resolution de doublons, on verifie les droits delegues
{
  $user_GUID=$ligne_droit_admin['GUID'];
  $sql_sign="SELECT Droits,GUID FROM Personnes WHERE Login='$sign'";
  $resultat_sign=mysqli_query($db,$sql_sign);
  $ligne_sign=mysqli_fetch_array($resultat_sign);
  $sign_GUID=$ligne_sign['GUID'];
  $sign_droits=$ligne_sign['Droits'];
  //On cherche s'il a des droits delegues par n'importe quel utilisateur signataire
  if (preg_match("`adr`",$sign_droits)) //on verifie d'abord que le signataire a les droits
  {
  //On cherche si le delegue a les droits
    $sql_droits_delegues="SELECT FriendUserDroits FROM user_perms WHERE SignataireGUID='$sign_GUID' AND FriendUserGUID='$user_GUID'";
    $resultat_droits_delegues=mysqli_query($db,$sql_droits_delegues);
    $ligne_droits_delegues=mysqli_fetch_array($resultat_droits_delegues);
    if (!preg_match("`adr`",$ligne_droits_delegues['Droits'])) //Si le delegue n'a pas le droit adm, on sort
    {
      header ('location: liste.php' );
      exit;

    }
  }
  else //si le signataire n'a pas les droits, on sort
  {
    header ('location: liste.php' );
    exit;
  }
}
$selection_patient=$_REQUEST['selection_patient'];
/* Les différents champs à fusionner
FchGnrl_NomDos
FchGnrl_Prenom
FchPat_NomFille
FchPat_Adresse
FchPat_Ville
FchPat_CP
FchPat_NumSS
FchPat_Nee
FchPat_Profession
FchPat_Tel1
*/
if (isset($_REQUEST['button_valid_merge'])) //si on recoit le bouton de fusion
{
//on recupere les champs a fusionner et on en deduit les valeurs a mettre a jour
//seul le nom est un GUID, les autres sont les chaines a inserer
  $GUID=$_REQUEST['Nom'];
//on cherche l'id_primkey pour metter a jour dans les intervenants et les notes
  $sql_chercher_ID="SELECT ID_PrimKey FROM IndexNomPrenom WHERE FchGnrl_IDDos='$GUID'";
  $resultat_chercher_ID=mysqli_query($db,$sql_chercher_ID);
  $ligne_chercher_ID=mysqli_fetch_array($resultat_chercher_ID);
  $ID_PrimKey=$ligne_chercher_ID['ID_PrimKey'];

  $Prenom=$_REQUEST['Prenom'];
  $jeunefille=$_REQUEST['jeunefille'];
  $nee=$_REQUEST['nee'];
  $sexe=$_REQUEST['sexe'];
  $adresse=$_REQUEST['adresse'];
  $CP=$_REQUEST['CP'];
  $ville=$_REQUEST['ville'];
  $tel1=$_REQUEST['tel1'];
  $tel2=$_REQUEST['tel2'];
  $tel3=$_REQUEST['tel3'];
  $email=$_REQUEST['email'];
  $NumSS=$_REQUEST['NumSS'];
  $PatientAss=$_REQUEST['PatientAss'];
  $PrenomAss=$_REQUEST['PrenomAss'];
  $NomAss=$_REQUEST['NomAss'];
  $Profession=$_REQUEST['Profession'];
  $Titre=$_REQUEST['Titre'];
  $Notes=$_REQUEST['Notes'];
//les requetes
//on met a jour la fiche d'index
  $sql_merge_index_nom_prenom=sprintf("UPDATE IndexNomPrenom SET FchGnrl_Prenom='%s' WHERE FchGnrl_IDDos='$GUID'",mysqli_real_escape_string($db,$Prenom));
  $resultat_index_nom_prenom=mysqli_query($db,$sql_merge_index_nom_prenom);
//on met a jour fchpat
  $sql_merge_fchpat=sprintf("UPDATE fchpat SET FchPat_NomFille='%s',FchPat_Nee='%s',FchPat_Sexe='%s',FchPat_Adresse='%s',FchPat_CP='%s',FchPat_Ville='%s',FchPat_Tel1='%s',FchPat_Tel2='%s',FchPat_Tel3='%s',FchPat_Email='%s',FchPat_NumSS='%s',FchPat_PatientAss='%s',FchPat_NomAss='%s',FchPat_PrenomAss='%s',FchPat_Profession='%s',FchPat_Titre='%s' WHERE FchPat_GUID_Doss='$GUID'",mysqli_real_escape_string($db,$jeunefille),mysqli_real_escape_string($db,$nee),mysqli_real_escape_string($db,$sexe),mysqli_real_escape_string($db,$adresse),mysqli_real_escape_string($db,$CP),mysqli_real_escape_string($db,$ville),mysqli_real_escape_string($db,$tel1),mysqli_real_escape_string($db,$tel2),mysqli_real_escape_string($db,$tel3),mysqli_real_escape_string($db,$email),mysqli_real_escape_string($db,$NumSS),mysqli_real_escape_string($db,$PatientAss),mysqli_real_escape_string($db,$NomAss),mysqli_real_escape_string($db,$PrenomAss),mysqli_real_escape_string($db,$Profession),mysqli_real_escape_string($db,$Titre));
  $resultat_merge_fchpat=mysqli_query($db,$sql_merge_fchpat);

//on remplace tous les GUID des patients a supprimer dans les intervenants par celui a garder
  foreach ($selection_patient as $ce_patient) //on examine tous les GUID transmis par les hidden
  {
    if ($ce_patient != $GUID) //on supprime les fiches qui ne sont pas celle de GUID et on met a jour intervenants et notes
    {
//on supprime les liaisons aux intervenants si provoquent des doublons
      $sql_chercher_intervenants_de_ce_patient="SELECT * FROM fchpat_Intervenants WHERE fchpat_Intervenants_PatGUID='$ce_patient' ";
      $resultat_chercher_intervenants_de_ce_patient=mysqli_query($db,$sql_chercher_intervenants_de_ce_patient);
      while ($ligne_chercher_intervenants_de_ce_patient=mysqli_fetch_array($resultat_chercher_intervenants_de_ce_patient))
      {
	$doublon_a_chercher=$ligne_chercher_intervenants_de_ce_patient['fchpat_Intervenants_IntervPK'];
	$sql_supprimer_doublon_intervenant="DELETE FROM fchpat_Intervenants WHERE fchpat_Intervenants_IntervPK='$doublon_a_chercher' AND fchpat_Intervenants_PatGUID='$GUID'";
	$resultat_supprimer_doublon_intervenant=mysqli_query($db,$sql_supprimer_doublon_intervenant);
      }
//on fusionne les intervenants
    $sql_merge_intervenant="UPDATE fchpat_Intervenants SET fchpat_Intervenants_PatGUID='$GUID',fchpat_Intervenants_PatPK='$ID_PrimKey' WHERE  fchpat_Intervenants_PatGUID='$ce_patient'";
    $resultat_merge_intervenant=mysqli_query($db,$sql_merge_intervenant);
    
    $sql_rename_document="UPDATE RubriquesBlobs SET RbDate_IDDos='$GUID' WHERE RbDate_IDDos='$ce_patient'";
    $resultat_rename_document=mysqli_query($db,$sql_rename_document);
    
    $sql_rename_title="UPDATE RubriquesHead SET RbDate_IDDos='$GUID' WHERE  RbDate_IDDos='$ce_patient'";
    $resultat_rename_title=mysqli_query($db,$sql_rename_title);
    
    $sql_merge_notes="UPDATE fchpat_Note SET fchpat_Note_PatGUID='$GUID', fchpat_Note_PatPK='$ID_PrimKey' WHERE fchpat_Note_PatGUID='$ce_patient'";
    $resultat_merge_notes=mysqli_query($db,$sql_merge_notes);
    
    $sql_merge_agenda="UPDATE agenda SET GUID='$GUID' WHERE fchpat_Note_PatGUID='$ce_patient'";
    $resultat_merge_agenda=mysqli_query($db,$sql_merge_agenda);
    
    $sql_delete_fchpat="DELETE FROM fchpat WHERE FchPat_GUID_Doss='$ce_patient'";
    $resultat_delete_fchpat=mysqli_query($db,$sql_delete_fchpat);
    
    $sql_delete_patient="DELETE FROM IndexNomPrenom WHERE FchGnrl_IDDos='$ce_patient'";
    $resultat_delete_patient=mysqli_query($db,$sql_delete_patient);
    }
  }
//on arrive a la page du patient de destination apres fusion
header ('location: patient.php?GUID='.$GUID.'&affichage=listes');
}
	include("inc/header.php");
?>
    <title>
      Fusion de dossiers
    </title>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
  </head>
	
  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Fusion_de_fiches';
include("inc/menu-horiz.php");		
?>
   <div class="groupe">
      <h1>
	MedWebTux - Fusion de dossiers
      </h1>
	<p class="information">
	  Cette page vous permet de fusionner plusieurs dossiers.<br />
	  En effet, il peut arriver qu'un même patient possède plusieurs dossiers, souvent à la suite d'une erreur de saisie.<br />
	  Grâce à cette interface, vous pouvez choisir quels éléments vous gardez de chaque dossier (un dossier peut être exact en ce qui concerne, le nom, tandis qu'un autre peut avoir l'adresse exacte).<br />
	  Choisissez au moyen des boutons radio les éléments que vous voulez conserver, puis validez.<br />
	  Les éléments qui n'entrent pas en conflit (observations, ordonnances, rendez-vous, etc.) s'ajouteront automatiquement dans l'unique dossier.
	</p><br />

	<div class="tableau">
	<form action="merge.php" method="post">
	  <table>
	    <tr>
	      <th class="fond_th">
		Nom
	      </th>
	      <th class="fond_th">
		Prénom
	      </th>
	      <th class="fond_th">
		Jeune fille
	      </th>
	      <th class="fond_th">
		Naissance
	      </th>
	      <th class="fond_th">
		Sexe
	      </th>
	      <th class="fond_th">
		Adresse
	      </th>
	      <th class="fond_th">
		Code postal
	      </th>
	      <th class="fond_th">
		Ville
	      </th>
	      <th class="fond_th">
		Tel1
	      </th>
	      <th class="fond_th">
		Tel2
	      </th>
	      <th class="fond_th">
		Tel3
	      </th>
	      <th class="fond_th">
		E-Mail
	      </th>
	      <th class="fond_th">
		Num sécu
	      </th>
	      <th class="fond_th">
		Patient ass
	      </th>
	      <th class="fond_th">
		Nom ass
	      </th>
	      <th class="fond_th">
		Prénom ass
	      </th>
	      <th class="fond_th">
		Profession
	      </th>
	      <th class="fond_th">
		Titre
	      </th>
	      <th class="fond_th">
		Notes
	      </th>
	    </tr>
<?php
foreach ($selection_patient as $ce_patient)
{
?>
	    <tr>
	      <td class="fond_td">
		<input name="selection_patient[]" type="hidden" value="<?php echo $ce_patient ?>" />
<?php
  $resultat="";
  $sql="SELECT * FROM IndexNomPrenom INNER JOIN fchpat ON IndexNomPrenom.FchGnrl_IDDos=fchpat.FchPat_GUID_Doss WHERE FchGnrl_IDDos = '$ce_patient'";
  $resultat=mysqli_query($db,$sql);
  while ($ligne=mysqli_fetch_array($resultat))
  {
?>
		<input type="radio" value="<?php echo $ce_patient ?>" name="Nom" checked="checked" />
<?php

    echo "
		<a href=\"patient.php?GUID=".$ce_patient."\">".$ligne['FchGnrl_NomDos']."</a>";
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchGnrl_Prenom'] ?>" name="Prenom" checked="checked"  />
<?php
    echo '
		'.$ligne['FchGnrl_Prenom'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_NomFille'] ?>" name="jeunefille" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_NomFille'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Nee'] ?>" name="nee" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Nee'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Sexe'] ?>" name="sexe" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Sexe'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Adresse'] ?>" name="adresse" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Adresse'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_CP'] ?>" name="CP" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_CP'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Ville'] ?>" name="ville" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Ville'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Tel1'] ?>" name="tel1" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Tel1'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Tel2'] ?>" name="tel2" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Tel2'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Tel3'] ?>" name="tel3" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Tel3'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_EMail'] ?>" name="email" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Email'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_NumSS'] ?>" name="NumSS" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_NumSS'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_PatientAs'] ?>" name="PatientAss" checked="checked"  />
<?php
    echo '
	      '.$ligne['FchPat_PatientAss'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_PrenomAss'] ?>" name="PrenomAss" checked="checked"  />
<?php
    echo '
	      '.$ligne['FchPat_PrenomAss'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_NomAss'] ?>" name="NomAss" checked="checked"  />
<?php
    echo '
	      '.$ligne['FchPat_NomAss']
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Profession'] ?>" name="Profession" checked="checked"  />
<?php
    echo '
	      '.$ligne['FchPat_Profession'];
?>
	      </td>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne['FchPat_Titre'] ?>" name="Titre" checked="checked"  />
<?php
    echo '
		'.$ligne['FchPat_Titre']."
	      </td>";
  }
$sql_notes=sprintf("SELECT * FROM fchpat_Note WHERE fchpat_Note_PatGUID='%s'",mysqli_real_escape_string($db,$ce_patient));
$resultat_notes=mysqli_query($db,$sql_notes);
$ligne_notes=mysqli_fetch_array($resultat_notes);

?>
	      <td class="fond_td">
		<input type="radio" value="<?php echo $ligne_notes['fchpat_Note_Html'] ?>" name="Notes" checked="checked"  />
<?php
echo '
		'.$ligne_notes['fchpat_Note_Html']."
	      </td>
	    </tr>";
}
?>
	  </table>
	  <p>
	    <input name="button_valid_merge" type="submit" value="Fusionner" />
	  </p>
	</form>
	</div>
   </div>
<?php
include("inc/footer.php");
?>

