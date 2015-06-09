<?php
//page affichable de saisie des honoraires
//appelee depuis la fiche patient, la page d'activite ou la page consultation
// On demarre la session
session_start();
$loginOK = false;  // cf Astuce
if ( !isset( $_SESSION['login'] ) )
{
  header('location: ../index.php?page=liste' );
  exit;
}
//$user=$_SESSION['login'];
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];

include("../config.php");
//echo "host".$host. $loginbase. $pwd;

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

//on recupere le GUID du patient s'il existe
if (isset($_REQUEST['guid_patient']))
{
  $guid_patient=$_REQUEST['guid_patient'];
  if ($guid_patient=="")
    $patient=$_REQUEST['patient'];
}
elseif (isset($_REQUEST['patient'])) //si patient inconnu, on recupere au moins le nom
{
  $guid_patient="";
  $patient=$_REQUEST['patient'];
}
else // si rien connu, on initialise tout a vide
{
  $guid_patient="";
  $patient="";
}
// on recupere la date
if (isset($_REQUEST['date']))
{
    $date=$_REQUEST['date'];
    if (!$date)
  {
  //les dates de recherche selon la langue
    $date=iso_to_local(date('Y-m-d', date('U')),$date_format);
  }
}
else //pas de date dans l'url
{
  $date=iso_to_local(date('Y-m-d', date('U')),$date_format);

}
 
if (isset($_REQUEST['part_perso']))
  $part_perso=$_REQUEST['part_perso'];
else
  $part_perso="";

if(!$db)
{
  print "Erreur de connexion &agrave; la base<br :>";
  exit;
}
//echo $base.$db;
// on se connecte a drtuxtest
if (!mysqli_select_db($db,$base))
{
  print "Erreur drtuxtest ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
//On recupere les ID du medecin
$sql_chercher_user="SELECT * FROM Personnes WHERE Login='$user'";
$resultat_chercher_user=mysqli_query($db,$sql_chercher_user);
$ligne_chercher_user=mysqli_fetch_array($resultat_chercher_user);
$id_drtux_user=$ligne_chercher_user['ID_PrimKey'];

//On cherche l'activite du jour
$sql_chercher_activite="SELECT RbDate_Date FROM RubriquesHead WHERE RbDate_Date LIKE '".local_to_iso($date,$date_format)."%' AND RbDate_TypeRub LIKE '20030000'";
$resultat_chercher_activite=mysqli_query($db,$sql_chercher_activite);
$count_chercher_activite=mysqli_num_rows($resultat_chercher_activite);

//on se connecte à la compta
if (!mysqli_select_db($db,$base_compta))
{
  print "Erreur compta".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
$sql_chercher_utilisateur="SELECT * FROM utilisateurs WHERE id_drtux_usr='$id_drtux_user'";
$resultat_chercher_utilisateur=mysqli_query($db,$sql_chercher_utilisateur);
$count_chercher_utilisateur=mysqli_num_rows($resultat_chercher_utilisateur);

$ligne_chercher_utilisateur=mysqli_fetch_array($resultat_chercher_utilisateur);
$id_user=$ligne_chercher_utilisateur['id_usr'];
$praticien=$ligne_chercher_utilisateur['nom_usr'];

$sql_trouver_actes="SELECT * FROM actes_disponibles";
$resultat_trouver_actes=mysqli_query($db,$sql_trouver_actes);

$resultat_trouver_actes_2=mysqli_query($db,$sql_trouver_actes);
if (isset($_REQUEST['total']))
  $total=$_REQUEST['total'];
else
  $total="";
$payeur='';
//recuperation d'une saisie d'honoraires
if (isset($_REQUEST['Bouton_calculer']))
{
  $payeur=$_REQUEST['payeur'];
  $type_paiement=$_REQUEST['type_paiement'];
  $nom_acte=$_REQUEST['nom_acte'];
}
else
{
  $nom_acte[]="";
$type_paiement="";
}

//pour convertir les formats de dates
function local_to_iso($date,$date_format)
{
  $list_date=explode ("-",$date);
  if ($date_format=='fr')
  {
    $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
  }
  elseif ($date_format=='en')
    $date=$list_date[2].'-'.$list_date[0].'-'.$list_date[1];
  return $date;
}

function iso_to_local($date,$date_format)
{
  $list_date=explode ("-",$date);
  if ($date_format=='fr')
  {
  //on repasse en iso les dates qui arrivent en francais
    $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
  }
  elseif ($date_format=='en')
    $date=$list_date[1].'-'.$list_date[2].'-'.$list_date[0];
  return $date;
}

//recuperation des donnees si modifier recette
if (isset($_REQUEST['bouton_modifier_hono']))
{
  $id_hono=$_REQUEST['bouton_modifier_hono'];
  $connect=mysqli_select_db($db,$base_compta);
  $sql_chercher_hono="SELECT * FROM honoraires WHERE id_hono='$id_hono'";
  $resultat_chercher_hono=mysqli_query($db,$sql_chercher_hono);
  $ligne_chercher_hono=mysqli_fetch_array($resultat_chercher_hono);
  $date=$ligne_chercher_hono['date']; //iso
//repasser en format local
  $date=iso_to_local($date,$date_format);

  $guid_patient=$ligne_chercher_hono['GUID'];
  $emetteur_long="";
  $emetteur_long=explode("=",$ligne_chercher_hono['remarque']);
  //valeur 0=dep|em
  //valeur 1= payeur
  $payeur=$emetteur_long[1];
  if ($ligne_chercher_hono['esp'])
    $type_paiement='Espèces';
  elseif ($ligne_chercher_hono['chq'])
    $type_paiement='Chèque';
  elseif ($ligne_chercher_hono['cb'])
    $type_paiement='CB';
  //elseif ($ligne_chercher_hono['daf'])
  //  $type_paiement='DAF';
  elseif ($ligne_chercher_hono['autre'])
    $type_paiement='Autre';
  elseif ($ligne_chercher_hono['du'])
    $type_paiement='Dû';

//recuperation des actes
  $ces_actes=explode("\|",$ligne_chercher_hono['acte']);
  foreach ($ces_actes AS $cet_acte)
  {
    $acte_court=explode("//",$cet_acte);
    $nom_acte[]=$acte_court[0];
  }
//esp 	chq 	cb 	daf 	autre 	du
$total=$ligne_chercher_hono['esp']+$ligne_chercher_hono['chq']+$ligne_chercher_hono['cb']+$ligne_chercher_hono['daf']+$ligne_chercher_hono['autre']+$ligne_chercher_hono['du'];
}
$connect=mysqli_select_db($db,$base);
$sql_patient="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$guid_patient'";
$resultat_patient=mysqli_query($db,$sql_patient);
$count_patient=mysqli_num_rows($resultat_patient);

function affichage_journee($date,$db,$base_compta,$part_perso,$date_format,$count_chercher_activite,$db)
{ 
//formatage de la date
  $date_tableau=explode ("-",$date);
  if ($date_format=='fr')
    $date_iso=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
    $date_iso=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
  else
    $date_iso=$date;

  $connect=mysqli_select_db($db,$base_compta);
  $sql_chercher_honoraires_journee="SELECT * FROM honoraires WHERE date='$date_iso'";
  $resultat_chercher_honoraires_journee=mysqli_query($db,$sql_chercher_honoraires_journee);
  $count_chercher_honoraires_journee=mysqli_num_rows($resultat_chercher_honoraires_journee);
  
  echo "
      <div class=\"information\">
	<div style=\"text-align:center\">
	  <b>Nombre de recettes ce jour : </b>".$date." = <b>$count_chercher_honoraires_journee</b> pour $count_chercher_activite actes
	</div>
      </div>";
  ?>

  <form action="validation_hono.php" method="get" id="form_tableau_synthese"> 
    <div>
      <input name="date" type="hidden" value="<?php echo $date?>"/>
    </div>
    <div class="groupe">
  <div class="tableau">
      <table>
	<tr>
	  <th class="fond_th">
	    Patient
	  </th>
	  <th class="fond_th">
	    Payeur
	  </th>
	  <th class="fond_th">
	    Espèces
	  </th>
	  <th class="fond_th">
	    Chèques
	  </th>
	  <th class="fond_th">
	    Carte
	  </th>
	  <th class="fond_th">
	    DAF
	  </th>
	  <th class="fond_th">
	    Autre
	  </th>
	  <th class="fond_th">
	    Dû
	  </th>
	  <th class="fond_th">
	    Modifier
	  </th>
	  <th class="fond_th">
	    Sélectionner
	  </th>
	</tr>
  <?php
  while ($ligne_chercher_honoraires_journee=mysqli_fetch_array($resultat_chercher_honoraires_journee))
  {
    $emetteur_long=explode("=",$ligne_chercher_honoraires_journee['remarque']);
//valeur 0=dep|em
//valeur 1= payeur
    $payeur=$emetteur_long[1];

  //patient 	GUID 	praticien 	date 	acte 	remarque 	esp 	chq 	cb 	daf
    echo "
	<tr>
	  <td class=\"fond_td\">
	    <a href=\"../frame_patient.php?GUID=".$ligne_chercher_honoraires_journee['GUID']."\">".stripslashes($ligne_chercher_honoraires_journee['patient'])."</a>
	  </td>
	  <td class=\"fond_td\">
	    ".stripslashes($payeur)."
	  </td>
	  <td class=\"fond_td\" style=\"text-align:right\">
	    ".$ligne_chercher_honoraires_journee['esp']."
	  </td>
	  <td class=\"fond_td\" style=\"text-align:right\">
	    ".$ligne_chercher_honoraires_journee['chq']."
	  </td>
	  <td class=\"fond_td\" style=\"text-align:right\">
	    ".$ligne_chercher_honoraires_journee['cb']."
	  </td>
	  <td class=\"fond_td\" style=\"text-align:right\">
	    ".$ligne_chercher_honoraires_journee['daf']."
	  </td>
	  <td class=\"fond_td\" style=\"text-align:right\">
	    ".$ligne_chercher_honoraires_journee['autre']."
	  </td>
	  <td class=\"fond_td\" style=\"text-align:right\">
	    ".$ligne_chercher_honoraires_journee['du']."
	  </td>
	  <td class=\"fond_td\">
	    <input name=\"bouton_modifier_hono[".$ligne_chercher_honoraires_journee['id_hono']."]\" type=\"submit\" value=\"Modifier\" />
	  </td>
	  <td class=\"fond_td\">
	    <input name=\"coche_selectionner_hono[]\" type=\"checkbox\" value=\"".$ligne_chercher_honoraires_journee['id_hono']."\" />
	  </td>
	</tr>";
  }
  ?>
	<tr>
	<td colspan="9">
	  <label for="coche_confirmation_suppression">
	    Suppression
	  </label>
	  <input name="coche_confirmation_suppression" id="coche_confirmation_suppression" value="oui" type="checkbox" />
	  <input name="bouton_supprimer_hono" value="Supprimer" type="submit" />
	</td>
      </tr>
    </table>
    </div>
    </div>
  </form>
</div>
<?php
} //fin fonction affichage journee

include("../inc/header.php");
?>
		<title>
		  MedWebTux - Comptabilité pour MedinTux
		</title>
<?php 
include("calendar.php"); //on inclut le calendrier javascript function getCalendar(in_dateField) function killCalendar() 
?>
<script type="text/javascript">
<!-- 
function mettre_valeur()
{
  var acte=new Array;
<?php

//la recuperation de la valeur des actes en fonction de leur position dans la liste et creation des variables en javascript
  $i=0;
  while ($ligne_trouver_actes=mysqli_fetch_array($resultat_trouver_actes)) //liste des actes connus de la base
  {
    echo "acte[$i]=".$ligne_trouver_actes['montant_total'].";\n";
    $i++;
  }
?>
// acte[0]=22; //exemple de resultat de la boucle php
  document.forms['form_saisie_hono'].total.value =''; //vider la zone total avant nouvelle valeur
  var valeur = document.forms['form_saisie_hono'].nom_acte.options;
  var tmp=0;
  for (var i=0;i < valeur.length; i++)
  {    
    if ( valeur[i].selected )
      {    
      tmp+=acte[i];
    }
  }
  document.forms['form_saisie_hono'].total.value += tmp; // on ecrit le resultat dans total
}
-->
</script>

<script type="text/javascript">
<!-- 
function verif_vide(montant)
{
  if (montant == "")
  { 
  alert("Le champ n'est pas rempli\nMettez un montant");
  return false;
  }
  return true;
}
-->
</script>

    <link rel="stylesheet" href="../css/style.css" type="text/css" media="screen" />
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='saisie_hono';
include("inc/menu-horiz-compta.php"); //class=nav

?>
      <div class="groupe">
	<h1>MedWebTux - Saisie d'honoraires</h1>

	<p>
	  &nbsp;&nbsp;&nbsp;&nbsp;<a href="bilan_recettes.php">Aller au bilan des recettes</a>
	</p>

<?php
if ($count_patient)
{
  $ligne_patient=mysqli_fetch_array($resultat_patient);
  $nom_patient=$ligne_patient['FchGnrl_NomDos'];
  $prenom_patient=$ligne_patient['FchGnrl_Prenom'];
}
else //aucun patient ne correspond au GUID donne
{
  $nom_patient=$patient;
  $prenom_patient="";
}

//mode deroulant actes
$connect=mysqli_select_db($db,$base_compta);
$resultat_trouver_actes=mysqli_query($db,$sql_trouver_actes);
?>
	<form action="saisie_hono.php">
	  <div>
	    <input  type="submit" value="Nettoyer le formulaire"/>
	  </div>
	</form>
	<form action="validation_hono.php" method="get" id="form_saisie_hono"> 
	  <div>
	    <input name="guid_patient" type="hidden" value="<?php echo $guid_patient ?>" />
	  </div>
	  <table>
	    <tr>
	      <th>
		<label for="patient">
		  Patient 
		</label>
	      </th>
	      <td>
		<input name="patient" id="patient" type="text" value="<?php echo stripslashes($nom_patient)." ".stripslashes($prenom_patient); ?>" />
	      </td>
	    </tr>
	    <tr>
	      <th>
		<label for="payeur">
		  Payeur 
		</label>
	      </th>
	      <td>
		<input name="payeur" id="payeur" type="text" value="<?php if ($payeur) echo stripslashes($payeur); else echo stripslashes($nom_patient)." ".stripslashes($prenom_patient) ?>" />
	      </td>
	    </tr>
	    <tr>
	      <th>
		<label for="date">
		  Date 
		</label>
	      </th>
	      <td>
		<input name="date" id="date" type="text" value="<?php echo $date ?>"/>
		<a href="#" onclick="return getCalendar(document.forms['form_saisie_hono'].date);"><img src="../pics/calendar.png" style="border:0" alt="calendar picture" /></a>
	      </td>
	    </tr>
	    <tr>
	      <th>
		Actes
	      </th>
	      <td>
		<select name="nom_acte[]" id="nom_acte" title="options multiples possibles (touche Ctrl ou Maj)" multiple="multiple" size="7"  onclick="mettre_valeur()"><!-- On appelle la fonction javascript qui lira toutes les selections et ecrira dans la zone total -->
<?php
  $i=0;
  while ($ligne_trouver_actes=mysqli_fetch_array($resultat_trouver_actes)) //liste des actes connus de la base
  {
  ?>
		  <option value="<?php echo $ligne_trouver_actes['nom_acte'] ?>"
<?php 
    foreach ($nom_acte as $cet_acte)//La liste des actes fournis par l'URL
    {
      if ($cet_acte==$ligne_trouver_actes['nom_acte'])
      {
	echo "selected='selected'" ;
      }
    }
?>
>
<?php
//le contenu de option =nom des actes en clair avec la valeur
      echo $ligne_trouver_actes['nom_acte']." ".$ligne_trouver_actes['type']." ".$ligne_trouver_actes['montant_total'];
      $i++;
?>
		  </option>
<?php
  }
?>
		</select>
	      </td>
	    </tr>
	    <tr>
	      <th>
		Autre : 
	      </th>
	      <td>
		<input name="nom_acte[]" type="text" />
	      </td>
	    </tr>
	    <tr>
	      <th>
		Montant : 
	      </th>
	      <td>
		<input name="total" id="total" type="text" value="<?php echo $total ?>" />
		<select name="type_paiement">
		  <option <?php if ($type_paiement=="Espèces") echo "selected='selected'" ?> >Espèces</option>
		  <option <?php if ($type_paiement=="Chèque") echo "selected='selected'" ?>>Chèque</option>
		  <option <?php if ($type_paiement=="CB") echo "selected='selected'" ?>>CB</option>
		<!-- 	<option <?php //if ($type_paiement=="DAF") echo "selected" ?>>DAF</option> -->
		  <option <?php if ($type_paiement=="Dû") echo "selected='selected'" ?>>Dû</option>
		  <option <?php if ($type_paiement=="Autre") echo "selected='selected'" ?>>Autre</option>
		</select>
	      </td>
	    </tr>
	    <tr>
	      <th>
		Part personnelle
	      </th>
	      <td>
		<select name="part_perso">
		  <option <?php if ($part_perso=="100") echo "selected='selected'" ?> >100</option>
		  <option <?php if ($part_perso=="50") echo "selected='selected'" ?> >50</option>
		  <option <?php if ($part_perso=="30") echo "selected='selected'" ?> >30</option>
		  <option <?php if ($part_perso=="0") echo "selected='selected'" ?> >0</option>
		</select>
		%
	      </td>
	    </tr>
	    <tr>
	      <td>
		<input name="Bouton_calculer" type="submit" value="Calculer" />
	      </td>
	      <td>
<?php
if (isset($_REQUEST['bouton_modifier_hono']))
{
?>
		<input name="id_hono" value="<?php echo $id_hono ?>" type="hidden" />
		<input name="Bouton_enregistrer_modification" type="submit" value="Modifier" />
<?php
}
else
{
?>
		<input name="Bouton_enregistrer" type="submit" value="Enregistrer" onclick="return verif_vide(document.forms['form_saisie_hono'].total.value);"/>
<?php
}
?>
	    </td>
	  </tr>
      </table>
    </form>
<?php //bouton pour retourner a l'activite
list($annee,$mois,$jour,)=explode ("-",$date,3);
//utilisateur=delafond%3A%3Adelafond&jour_debut=2013&mois_debut=04&annee_debut=04&jour_fin=2013&mois_fin=04&annee_fin=04&critere_recherche=aucun&cle=&envoyer_date=Chercher&bouton_afficher_tout=Montrer+les+consultations&bouton_activite=Activité //marche pas

//utilisateur=delafond&debut=04-04-2013&fin=04-04-2013&critere_recherche=aucun&cle=&envoyer_date=Chercher //marche
?>

      <form action="../activite.php" method="get">
	<div style="text-align:center">
	  <input name="utilisateur" type="hidden" value="<?php echo $user ?>"/>
	  <input name="debut" type="hidden" value="<?php echo $date?>"/>
	  <input name="fin" type="hidden" value="<?php echo $date ?>"/>
	  <input name="critere_recherche" type="hidden" value="aucun"/>
	  <input name="cle" type="hidden" value=""/>
	  <input name="envoyer_date" type="hidden" value="Chercher"/>
	  <input name="bouton_afficher_tout" type="hidden" value="Montrer les consultations"/>
	  <input type="submit" value="Activité" name="bouton_activite" />
	</div>
      </form>
<?php
affichage_journee($date,$db,$base_compta,$part_perso,$date_format,$count_chercher_activite,$db);
// on ferme la base
mysqli_close($db);
include("inc/footer.php");
?>
    </div>
  </body>
</html>
