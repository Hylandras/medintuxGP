<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=correspondant' );
  exit;
}

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");


if(!$db)
{
  print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
$ID_corresp="";
$envoyer="";
$soumettre="";
$soumettre_patient="";
$exact="";

if (isset($_GET['ID_corresp']))
  $ID_corresp=$_GET['ID_corresp'];
if (isset($_GET['envoyer']))
  $envoyer=$_GET['envoyer'];
if (isset($_GET['soumettre']))
  $soumettre=$_GET['soumettre'];
if (isset($_GET['cle']))
  $cle=$_GET['cle'];
if (isset($_GET['patient_ID']))
  $patient_ID=$_GET['patient_ID'];
if (isset($_GET['patient_GUID']))
  $patient_GUID=$_GET['patient_GUID'];
if (isset($_GET['ID_fiche_liaison']))
  $ID_fiche_liaison=$_GET['ID_fiche_liaison'];
if (isset($_GET['from']))
  $from=$_GET['from'];
if (isset($_GET['soumettre_patient']))
  $soumettre_patient=$_GET['soumettre_patient'];
if (isset($_GET['ID_patient']))
  $ID_patient=$_GET['ID_patient'];
if (isset($_GET['exact']))
  $exact=$_GET['exact'];
if (isset($_GET['liste_spe']))
  $liste_spe=$_GET['liste_spe'];

if ($soumettre=="Lier")
{
//On fait la liaison et on revient a la fiche patient
//on cherche si la liaison n'existe pas deja pour eviter les doublons
  $sql_chercher_doublons="SELECT * FROM fchpat_Intervenants WHERE fchpat_Intervenants_IntervPK='$ID_corresp' AND fchpat_Intervenants_PatGUID='$patient_GUID'";
  $resultat_chercher_doublons=mysqli_query($db,$sql_chercher_doublons);
  $count_chercher_doublons=mysqli_num_rows($resultat_chercher_doublons);
  if ($count_chercher_doublons==0)
  {
    $sql_liaison="INSERT INTO fchpat_Intervenants (fchpat_Intervenants_PatGUID,fchpat_Intervenants_PatPK,fchpat_Intervenants_IntervPK) VALUES ('$patient_GUID','$patient_ID','$ID_corresp')";
    $resultat_liaison=mysqli_query($db,$sql_liaison);
  }
  echo("<script language=\"javascript\">");
  echo("top.location.href = \"frame_patient.php?GUID=".$patient_GUID."\"");
  echo("</script>");  
  exit;
}

$sql="SELECT * FROM Personnes WHERE ID_PrimKey='$ID_corresp'";	
$resultat=mysqli_query($db,$sql);
$liste=mysqli_fetch_array($resultat);

if ($envoyer=="Oui") //On confirme la suppression de la fiche
{
  $sql_supprimer_corresp="DELETE FROM fchpat_Intervenants WHERE fchpat_Intervenants_PK='$ID_fiche_liaison'";
  $resultat_supprimer_corresp=mysqli_query($db,$sql_supprimer_corresp);
  if ($from=="intervenant")
    header('location: fiche_intervenant.php?intervenant='.$ID_corresp );
  elseif ($from=="patient")
  {
    echo("<script language=\"javascript\">");
    echo("top.location.href = \"frame_patient.php?GUID=".$patient_ID."\"");
    echo("</script>");  
    exit;
  }
}
elseif ($envoyer=="Non") //on refuse la suppression de la fiche
{
  if ($from=="intervenant")
    header('location: fiche_intervenant.php?intervenant='.$ID_corresp.'&montrer_patients=Montrer');
  elseif ($from=="patient")
{
    echo("<script language=\"javascript\">");
    echo("top.location.href = \"frame_patient.php?GUID=".$patient_ID."\"");
    echo("</script>");  
}

  exit;
}
include("inc/header.php");
?>
    <title>
      Liaison à <?php echo $liste['Nom'].' '.$liste['Prenom'] ?> - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
<?php
if ($envoyer!="Annuler le lien") //pas besoin de javascript sur la page d'annulation
{
?>
<script type="text/javascript">
<!--
function donner_focus(chp)
{
document.getElementById(chp).focus();
}
-->
</script>

<script type="text/javascript">
<!--
function verif_champ(champ)
{
  if (champ == "")
  {
    alert("Le champ n'est pas rempli\nMettez le signe % si vous voulez vraiment afficher tout le fichier");
    return false;
  }
  return true;
}
-->
</script>
<?php
}
?>
  </head>
    <body style="font-size:<?php echo $fontsize; ?>pt" onload="donner_focus('cle')">
      <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Liaison_à_un_intervenant';
if ($envoyer!='patient')
  include("inc/menu-horiz.php");
?>
<?php

if ($envoyer=="Annuler le lien")
{
  $sql_ID="SELECT FchGnrl_NomDos,FchGnrl_Prenom FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient_ID'";	
  $resultat_ID=mysqli_query($db,$sql_ID);
  $ligne_ID=mysqli_fetch_array($resultat_ID);

  $sql_corresp="SELECT Nom FROM Personnes WHERE ID_PrimKey='$ID_corresp'";	
  $resultat_corresp=mysqli_query($db,$sql_corresp);
  $ligne_corresp=mysqli_fetch_array($resultat_corresp);
?>
  <div class="groupe">
	<h1>
	  MedWebTux - Liaison &agrave; l'intervenant
	</h1>

	Confirmez-vous la suppression de la liaison entre le patient <b><?php echo $ligne_ID['FchGnrl_NomDos']." ".$ligne_ID['FchGnrl_Prenom'] ?></b> et l'intervenant <b><?php echo  $ligne_corresp['Nom'] ?></b>&nbsp;?
	<br />
	<form action="liaison_correspondant.php" method="get" target="_top">
	  <div style="text-align:center;">
	    <input name="from" type="hidden" value="<?php echo $from ?>" />
	    <input name="ID_corresp" type="hidden" value="<?php echo $ID_corresp ?>" />
	    <input name="patient_ID" type="hidden" value="<?php echo $patient_ID ?>" />
	    <input name="ID_fiche_liaison" type="hidden" value="<?php echo $ID_fiche_liaison ?>" />
	    <input name="envoyer" type="submit" value="Oui" />
	    <input name="envoyer" type="submit" value="Non" />
	  </div>
	</form>
  </div>
<?php
}

if ($envoyer=="Lier un nouveau patient" or $soumettre=="Chercher")
{
?>
  <div class="groupe">
	<h1>
	  Liaison &agrave; l'intervenant <a href="fiche_intervenant.php?intervenant=<?php echo $ID_corresp ?>" target="_top"><?php echo $liste['Nom']?></a>
	</h1>
	<table>
	  <tr>
	    <td>
	      <form action="liaison_correspondant.php" method="get" onsubmit="return verif_champ(this.cle.value);" >
		<fieldset>
		  <legend>
		    Nom du patient : 
		  </legend>
		  <input name="ID_corresp" type="hidden" value="<?php echo $ID_corresp ?>" />
		  <input name="cle" id="cle" type="text" value="" size="40" title="Donnez ici une partie du nom du patient &agrave; lier ou le nom exact si vous cochez la case en dessous" />
		  <br />
		  <input name="exact" id="exact" type="checkbox" value="1" />
		  <label for="exact">
		    Correspondance exacte
		  </label>
		</fieldset>
		<p>			
		  <input name="soumettre" type="submit" value="Chercher" />
		</p>
	      </form>
	    </td>
	  </tr>
	</table>
    </div>
<?php
}
elseif ($envoyer=="patient" or $soumettre_patient=="Chercher")
{
  $GUID=$_GET['GUID'];
  $sql_nom="SELECT FchGnrl_NomDos,ID_PrimKey FROM IndexNomPrenom WHERE FchGnrl_IDDos='$GUID'";	
  $resultat_nom=mysqli_query($db,$sql_nom);
  $ligne_nom=mysqli_fetch_array($resultat_nom);

?>
  <div class="groupe">
      <h1>
	Liaison au patient <a href="frame_patient.php?GUID=<?php echo $GUID ?>" target="_top"><?php echo $ligne_nom['FchGnrl_NomDos'] ?></a>
      </h1>
      <table>
	<tr>
	  <td>
	    <form action="liaison_correspondant.php" method="get"  onsubmit="return verif_champ(this.cle.value);">
	      <p>
		Sp&eacute;cialit&eacute; : 
		<select name="liste_spe" >
		  <option value="%">
		    Toutes
		  </option>
<?php
  $sql_specialite="SELECT Qualite FROM Personnes GROUP BY Qualite ORDER BY Qualite";
  $resultat_specialite=mysqli_query($db,$sql_specialite);
	
  while ($ligne_specialite=mysqli_fetch_array($resultat_specialite))//recherche des specialites pour en faire un deroulant
  { 
    if ($ligne_specialite['Qualite'])//suppression des lignes vides
    {
?>
		  <option value="<?php echo $ligne_specialite['Qualite'];?>" >
		    <?php echo $ligne_specialite['Qualite'] ?>
		  </option>
<?php
    }
  }
?>
		</select>
	      </p>
	      <p>
		Nom de l'intervenant : 
		<input name="cle" type="text" value="" size="40" />
		<input name="GUID" type="hidden" value="<?php echo $GUID ?>" />
	      </p>
	      <p>
		<input name="exact" type="checkbox" value="1" />
		Correspondance exacte
	      </p>
	      <p>
		<input name="soumettre_patient" type="submit" value="Chercher"  />
	      </p>
	    </form>
	  </td>
	</tr>
      </table>
  </div>
<?php
}
//mode de liaison pa partir du dossier patient
if ($soumettre=="Chercher") //recherche de patient selon la cle fournie pour remplir la liste et les valeurs cachees des boutons Lier
{
  if ($exact!=1)
    $cle="%".$cle."%";
  $sql_patient="SELECT * FROM IndexNomPrenom INNER JOIN fchpat ON IndexNomPrenom.FchGnrl_IDDos=fchpat.FchPat_GUID_Doss WHERE IndexNomPrenom.FchGnrl_NomDos LIKE '$cle' ORDER BY FchGnrl_NomDos,FchGnrl_Prenom";
  $resultat_patient=mysqli_query($db,$sql_patient);
?>
    <div class="groupe">
      <table>
	<tr>
	  <th class="fond_th">
	    Nom
	  </th>
	  <th class="fond_th">
	    Pr&eacute;nom
	  </th>
	  <th class="fond_th">
	    Date de naissance
	  </th>
	  <th class="fond_th">
	    Intervenant
	  </th>
	</tr>
<?php
  while ($liste_patient=mysqli_fetch_array($resultat_patient))
  {
    $annee = substr($liste_patient["FchPat_Nee"], 0, 4);
    $mois = substr($liste_patient["FchPat_Nee"], 5, 2);
    $jour = substr($liste_patient["FchPat_Nee"], 8, 2);
?>
	<tr>
	  <td class="fond_td">
	    <?php echo $liste_patient['FchGnrl_NomDos'] ?>
	  </td>
	  <td class="fond_td">
	    <?php echo $liste_patient['FchGnrl_Prenom'] ?>
	  </td>
	  <td class="fond_td">
	    <?php echo  $jour."-". $mois."-". $annee ?>
	  </td>
	  <td class="fond_td">
<?php
//on cherche si la liaison n'existe pas deja pour eviter les doublons
    $GUID=$liste_patient['FchGnrl_IDDos'];
    $sql_chercher_doublons="SELECT * FROM fchpat_Intervenants WHERE fchpat_Intervenants_IntervPK='$ID_corresp' AND fchpat_Intervenants_PatGUID='$GUID'";
    $resultat_chercher_doublons=mysqli_query($db,$sql_chercher_doublons);
    $count_chercher_doublons=mysqli_num_rows($resultat_chercher_doublons);
    if ($count_chercher_doublons==0)
    {
?>

	    <form action="liaison_correspondant.php"><!--Bouton Lier -->
	      <div>
		<input name="ID_corresp" type="hidden" value="<?php echo $ID_corresp ?>" />
		<input name="patient_ID" type="hidden" value="<?php echo $liste_patient['ID_PrimKey'] ?>" />
		<input name="patient_GUID" type="hidden" value="<?php echo $liste_patient['FchGnrl_IDDos'] ?>" />
		<input name="soumettre" type="submit" value="Lier" /> 
	      </div>
	    </form>
<?php
    }
    else
      echo "
	    Intervenant déjà lié";
?>
	  </td>
	</tr>
<?php
  }
?>
      </table>
  </div>
<?php
}
//Mode de liaison a partir du dossier intervenant
if ($soumettre_patient=="Chercher")
{
  if ($exact==1)
    $sql_intervenant="SELECT * FROM Personnes WHERE Nom= '$cle' AND Qualite LIKE '$liste_spe' ORDER BY Nom";
  else
  {
    $cle="%".$cle."%";
    $sql_intervenant="SELECT * FROM Personnes WHERE Nom LIKE '$cle' AND Qualite LIKE '$liste_spe' ORDER BY Nom";
  }
  $resultat_intervenant=mysqli_query($db,$sql_intervenant);
?>
   <div class="groupe">
      <table>
	<tr>
	  <th class="fond_th">
	    Nom
	  </th>
	  <th class="fond_th">
	    Sp&eacute;cialit&eacute;
	  </th>
	  <th class="fond_th">
	    Patient
	  </th>
	</tr>
<?php
  while ($liste_intervenant=mysqli_fetch_array($resultat_intervenant))
  {
?>
	<tr>
	  <td class="fond_td">
	    <a href="fiche_intervenant.php?intervenant=<?php echo $liste_intervenant['ID_PrimKey']?>" ><?php echo $liste_intervenant['Nom'] ?></a>
	  </td>
	  <td class="fond_td">
	    <?php echo $liste_intervenant['Qualite'] ?>
	  </td>
	  <td class="fond_td">
<?php
//on cherche si la liaison n'existe pas deja pour eviter les doublons
    $ID_corresp=$liste_intervenant['ID_PrimKey'];
    $sql_chercher_doublons="SELECT * FROM fchpat_Intervenants WHERE fchpat_Intervenants_IntervPK='$ID_corresp' AND fchpat_Intervenants_PatGUID='$GUID'";
    $resultat_chercher_doublons=mysqli_query($db,$sql_chercher_doublons);
    $count_chercher_doublons=mysqli_num_rows($resultat_chercher_doublons);
    if ($count_chercher_doublons==0)
    {
?>
	    <form action="liaison_correspondant.php"><!--Bouton Lier -->
	      <div>
		<input name="ID_corresp" type="hidden" value="<?php echo $liste_intervenant['ID_PrimKey'] ?>" />
		<input name="patient_ID" type="hidden" value="<?php echo $ligne_nom['ID_PrimKey'] ?>" />
		<input name="patient_GUID" type="hidden" value="<?php echo $GUID ?>" />
		<input name="soumettre" type="submit" value="Lier" /> 
	      </div>
	    </form>
<?php
    }
    else
      echo "
	    Intervenant déjà lié";
?>

	  </td>

	</tr>
<?php
  }
?>
      </table>
  </div>
<?php
}
?>
<?php
include("inc/footer.php");
?>