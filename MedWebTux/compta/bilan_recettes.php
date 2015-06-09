<?php
//page affichable de bilan des recettes.
//Permet de creer des remises de cheques ou d'afficher toutes les recettes selon differents criteres
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
//$user=$tab_login[0];
$sign=$tab_login[1];

include("../config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; la base<br />";
  exit;
}

// on se connecte a drtuxtest
if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
$especes=0;
$cheques=0;
$virement=0;
$daf=0;
$cb=0;
$autre=0;
//On recupere les ID du medecin
$sql_chercher_user="SELECT * FROM Personnes WHERE Login='$sign'";
//echo $sql_chercher_user;
$resultat_chercher_user=mysqli_query($db,$sql_chercher_user);
$ligne_chercher_user=mysqli_fetch_array($resultat_chercher_user);
$id_drtux_user=$ligne_chercher_user['ID_PrimKey'];

//on se connecte à la compta
if (!mysqli_select_db($db,$base_compta))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
$sql_chercher_utilisateur="SELECT * FROM utilisateurs WHERE id_drtux_usr='$id_drtux_user'";
$resultat_chercher_utilisateur=mysqli_query($db,$sql_chercher_utilisateur);
$count_chercher_utilisateur=mysqli_num_rows($resultat_chercher_utilisateur);
//echo "count".$count_chercher_utilisateur;
$ligne_chercher_utilisateur=mysqli_fetch_array($resultat_chercher_utilisateur);
$id_user=$ligne_chercher_utilisateur['id_usr'];
$praticien=$ligne_chercher_utilisateur['nom_usr'];

$sql_trouver_actes="SELECT * FROM actes_disponibles";
$resultat_trouver_actes=mysqli_query($db,$sql_trouver_actes);

// on recupere les dates
if (isset($_REQUEST['date_debut']))
  $date_debut=$_REQUEST['date_debut'];
else
{
  if ($date_format=='fr')
    $date_debut=date('d-m-Y', date('U'));
  elseif ($date_format=='en')
    $date_debut=date('m-d-Y', date('U'));
  else 
    $date_debut=date('Y-m-d', date('U'));
}

if (isset($_REQUEST['date_fin']))
  $date_fin=$_REQUEST['date_fin'];
else
{
  if ($date_format=='fr')
    $date_fin=date('d-m-Y', date('U'));
  elseif ($date_format=='en')
    $date_fin=date('m-d-Y', date('U'));
  else 
    $date_fin=date('Y-m-d', date('U'));
}

function choix_date($date_debut,$date_fin)
{
?>
      <form action="bilan_recettes.php" method="get" id="form_saisie_filtre">
	<fieldset>
	  <legend >
	    Dates...
	  </legend >

	  <label for="date_debut">
	    <b>Début : </b>
	  </label>
	  <input name="date_debut" id="datepicker" type="text" value="<?php echo $date_debut ?>" />
<!--
<a href="#" onclick="return getCalendar(document.forms['form_saisie_filtre'].date_debut);"><img src="../pics/calendar.png" style="border:0" alt="calendar picture" /></a>
-->
	  <label for="date_fin">
	    <b>Fin : </b>
	  </label>
	  <input name="date_fin" id="datepickeur" type="text" value="<?php echo $date_fin ?>" />
<!--
<a href="#" onclick="return getCalendar(document.forms['form_saisie_filtre'].date_fin);"><img src="../pics/calendar.png" style="border:0" alt="calendar picture" /></a>
-->
	</fieldset>
	<fieldset>
	  <legend >
	    Types...
	  </legend >
	  <label for="type_paiement_tous">
	    Tous
	  </label>
	  <input name="type_paiement[]" id="type_paiement_tous" value="tous" type="checkbox" />
	  <label for="type_paiement_esp">
	    Espèces
	  </label>
	  <input name="type_paiement[]" id="type_paiement_esp" value="esp" type="checkbox" />
	  <label for="type_paiement_chq">
	    Chèques
	  </label>
	  <input name="type_paiement[]" id="type_paiement_chq" value="chq" type="checkbox" />
	  <label for="type_paiement_cb">
	    CB
	  </label>
	  <input name="type_paiement[]" id="type_paiement_cb" value="cb" type="checkbox" />
	  <label for="type_paiement_daf">
	    DAF
	  </label>
	  <input name="type_paiement[]" id="type_paiement_daf" value="daf" type="checkbox" />
	  <label for="type_paiement_autre">
	    Autres
	  </label>
	  <input name="type_paiement[]" id="type_paiement_autre" value="autre" type="checkbox" />
	  <label for="type_paiement_du">
	    Dus
	  </label>
	  <input name="type_paiement[]" id="type_paiement_du" value="du" type="checkbox" />
	</fieldset>
	<fieldset>
	  <legend >
	    N'afficher que...
	  </legend >
	  <input name="coche_non_valide" id="coche_non_valide" value="oui" type="checkbox" />
	  <label for="coche_non_valide">
	  Les non validés
	  </label>
	  <input name="bouton_envoyer_dates" type="submit" value="Envoyer" />
	</fieldset>
      </form>

<?php
}

function affichage_journee($date_debut, $date_fin,$db,$base_compta,$date_format,$especes,$cheques,$cb,$daf,$autre)
{
  $types="";
  if (isset($_REQUEST['type_paiement']))
  {
    $type_paiement=$_REQUEST['type_paiement'];
    foreach ($type_paiement AS $ce_type_paiement)
    {
      $types=$types." AND ".$ce_type_paiement;
      if ($ce_type_paiement=="tous")
      {
	$types="";
	break;
      }
    }
  }

  if (isset($_REQUEST['coche_non_valide']))
    $afficher_seulement=" AND valide=0";
  else
    $afficher_seulement="";
  echo "
      <fieldset>
	<legend>
	  Recette
	</legend>
	<b>Nombre de recettes durant la période du ".$date_debut." au ".$date_fin." : ";
//reformer les dates locales en iso
  $date_tableau=explode ("-",$date_debut);
  if ($date_format=='fr')
    $date_debut=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
      $date_debut=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
  $date_tableau=explode ("-",$date_fin);
  if ($date_format=='fr')
    $date_fin=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
      $date_fin=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
  $connect=mysqli_select_db($db,$base_compta);
  $sql_chercher_honoraires_journee="SELECT * FROM honoraires WHERE date>='$date_debut' AND date <='$date_fin' $types $afficher_seulement ORDER BY date";
  $resultat_chercher_honoraires_journee=mysqli_query($db,$sql_chercher_honoraires_journee);
  $count_chercher_honoraires_journee=mysqli_num_rows($resultat_chercher_honoraires_journee);
  echo $count_chercher_honoraires_journee."</b>";

?>      
<div>
      <form action="saisie_hono.php">
	<div>
	  <input type="submit" value="Nouvelle saisie"/>
	</div>
      </form>
</div>
      <form action="validation_hono.php" method="get" id="form1"> 
	<table class="tableau">
	  <tr>
	    <th class="fond_th">
	      Patient
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
	      Date
	    </th>
	    <th class="fond_th">
	      Acte
	    </th>
	    <th class="fond_th">
	      Émetteur
	    </th>
	    <th class="fond_th">
	      Validé
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
  $ces_actes=explode("|",$ligne_chercher_honoraires_journee['acte']);
//print_r ($ces_actes);
  $les_actes="";
  $acte_court="";
  foreach ($ces_actes AS $cet_acte)
  {
    $acte_court=explode("//",$cet_acte);
//A revoir. Ne marche pas
//echo $cet_acte."<br />";
//print_r ($acte_court);
    $les_actes=$les_actes." ".$acte_court[0];
  }
//$emetteur_long="";
//$emetteur_long=explode("\|",$ligne_chercher_honoraires_journee['remarque']);
//$emetteur_court=explode("=",$emetteur_long[2]);
  $emetteur_court=explode("=",$ligne_chercher_honoraires_journee['remarque']);
  //patient 	GUID 	praticien 	date 	acte 	remarque 	esp 	chq 	cb 	daf
  $especes=$especes+$ligne_chercher_honoraires_journee['esp'];
  $cheques=$cheques+$ligne_chercher_honoraires_journee['chq'];
  $cb=$cb+$ligne_chercher_honoraires_journee['cb'];
  $daf=$daf+$ligne_chercher_honoraires_journee['daf'];
  $autre=$autre+$ligne_chercher_honoraires_journee['autre'];
  //$virement=$virement+$ligne_chercher_honoraires_journee[''];

  //reformer les dates locales en locales
  $date=$ligne_chercher_honoraires_journee['date'];
  $date_tableau=explode ("-",$date);
  if ($date_format=='fr')
    $date=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
    $date=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];

  echo "
	  <tr>
	    <td class=\"fond_td\">
	      <a href=\"../frame_patient.php?GUID=".$ligne_chercher_honoraires_journee['GUID']."\">".$ligne_chercher_honoraires_journee['patient']."</a>
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['esp']."
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['chq']."
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['cb']."
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['daf']."
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['autre']."
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['du']."
	    </td>
	    <td class=\"fond_td\">
	      ".$date."
	    </td>
	    <td class=\"fond_td\">
	      ".$les_actes."
	    </td>
	    <!--<td>".$ligne_chercher_honoraires_journee['remarque']."</td>-->
	    <td class=\"fond_td\">
	      ".$emetteur_court[1]."
	    </td>
	    <td class=\"fond_td\">
	      ".$ligne_chercher_honoraires_journee['valide']."
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
	    <th>
	      Total
	    </th>
	    <td>
	      <?php echo $especes ?>
	    </td>
	    <td>
	      <?php echo $cheques?>
	    </td>
	    <td>
	      <?php echo $cb?>
	    </td>
	    <td>
	      <?php echo $daf?>
	    </td>
	    <td>
	      <?php echo $autre ?>
	    </td>
	    <td>
	    </td>
	    <th title="hors DAF et dus">
	      Total payé
	    </th>
	    <td>
	      <?php echo $especes+$cheques+$cb+$autre?>
	    </td>
	    <td>
	    </td>
	    <td>
	    </td>
	    <td colspan="2" >
	      <input name="button_invert_selection" onclick="Invers()" type="button" value=" Inverser la sélection " />
	    </td>
	  </tr>
	  <tr>
	    <td colspan="13">
	      Pour la sélection : 
	      <label for="coche_confirmation_suppression">
		Suppression
	      </label>
	      <input name="coche_confirmation_suppression" id="coche_confirmation_suppression" value="oui" type="checkbox" />
	      <input name="bouton_supprimer_hono" value="Supprimer" type="submit" />
	      <input name="bouton_valider_hono" value="Valider" type="submit"/>
	      <input name="bouton_remise_cheque" value="Remise de chèques" type="submit"/>
	      Compte : 
	      <select name="liste_comptes" id="liste_comptes">
<?php
$sql_chercher_comptes="SELECT * FROM comptes_bancaires";
$resultat_chercher_comptes=mysqli_query($db,$sql_chercher_comptes);
while ($ligne_chercher_comptes=mysqli_fetch_array($resultat_chercher_comptes))
{
  echo "
		    <option value=\"".$ligne_chercher_comptes['id_compte']."\">
		      ".$ligne_chercher_comptes['libelle']."
		    </option>";
}
?>
		  </select>
		</td>
	      </tr>
	    </table>
	  </form>
	</fieldset>

<?php
} //fin function affichage recettes

include("inc/header.php");
//include("calendar.php"); //on inclut le calendrier javascript function getCalendar(in_dateField) function killCalendar() 

?>
    <title>
      MedWebTux - Comptabilité pour MedinTux
    </title>
  <link rel="stylesheet" href="../css/style.css" type="text/css" media="screen" />

<!-- rajout css et scripts pour nouveau calendrier intégré par Yanick MORZA  -->
  <link rel='stylesheet' href='../css/ui.core.css' type='text/css' />
  <link rel='stylesheet' href='../css/ui.theme.css' type='text/css' />
  <link rel="stylesheet" href="../css/ui.datepicker.css" type="text/css" media="screen" />
  <link rel="stylesheet" href="../css/ui.datepickeur.css" type="text/css" media="screen" />

  <script type="text/javascript" src="../scripts/jquery/jquery-1.3.2.js"></script>
  <script type="text/javascript" src="../scripts/jquery/ui.core.js"></script>
  <script type="text/javascript" src="../scripts/jquery/ui.datepicker.js"></script>
  <script type="text/javascript" src="../scripts/jquery/ui.datepickeur.js"></script>
  <script type="text/javascript">
	$(function() {
		$("#datepicker").datepicker();
	});
	$(function() {
		$("#datepickeur").datepickeur();
	});
  </script>
<!-- rajout css et scripts pour nouveau calendrier intégré par Yanick MORZA  -->

  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" >
  <script type="text/javascript">
<!--
//pour inverser les coches des paiements - pratique pour les remises de cheques
function Invers()
{
  temp = document.forms['form1'].elements.length-5 ;
  for (i=0; i < temp; i++)
  {
    if(document.forms['form1'].elements[i].checked == 1)
    {
      document.forms['form1'].elements[i].checked = 0;
    }
    else 
    {
      document.forms['form1'].elements[i].checked = 1
    }
  }
}
//-->
</script>

<div class="conteneur">
<?php	
// insertion du menu d'en-tete	
	include("inc/menu-horiz-compta.php");

?>
	<div class="groupe">
		<h1>MedWebTux - Bilan des honoraires</h1>
<?php
choix_date($date_debut,$date_fin);
affichage_journee($date_debut, $date_fin,$db,$base_compta,$date_format,$especes,$cheques,$cb,$daf,$autre);

// on ferme la base
mysqli_close($db);
echo "</div>";

include("inc/footer.php");
?>
    </div>
  </body>
</html>
