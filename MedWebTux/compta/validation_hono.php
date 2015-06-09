<?php
// On demarre la session
session_start();
if (isset($_REQUEST['bouton_modifier_hono'])) //on renvoie sur la page de saisie en mode modification si on recoit un bouton modifier hono
{
//on recupere l'id hono entre crochets
$bouton_modifier_hono=$_REQUEST['bouton_modifier_hono'];
foreach ($bouton_modifier_hono as $les_id_hono => $cet_hono)
//echo $les_id_hono;
header ('location:saisie_hono.php?bouton_modifier_hono='.$les_id_hono);
exit;
}

$loginOK = false;  // cf Astuce
if ( !isset( $_SESSION['login'] ) )
{
	header('location: ../index.php?page=liste' );
	exit;
}
$user=$_SESSION['login'];
include("../config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion &agrave; la base<br>";
	exit;
}

// on se connecte a drtuxtest
if (!mysqli_select_db($db,$base))
{
	print "Erreur ".mysqli_error($db)."<br>";
	mysqli_close($db);
	exit;
}
//On recupere les ID du medecin
$sql_chercher_user="SELECT * FROM Personnes WHERE Login='$user'";
$resultat_chercher_user=mysqli_query($db,$sql_chercher_user);
$ligne_chercher_user=mysqli_fetch_array($resultat_chercher_user);
$id_drtux_user=$ligne_chercher_user['ID_PrimKey'];

if (isset($_REQUEST['guid_patient']))
{
	$guid_patient=$_REQUEST['guid_patient'];
}
else
	$guid_patient="";

if (isset($_REQUEST['date']))
{
  $date=$_REQUEST['date']; //format local. Il faut repasser en iso pour enregistrer
  $date_tableau=explode ("-",$date);
  if ($date_format=='fr')
    $date=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
    $date=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
}
else
  $date=date('Y-m-d', date('U')); // la date du jour en iso


if (isset($_REQUEST['part_perso']))
  $part_perso=$_REQUEST['part_perso'];
else
  $part_perso="";
//on se connecte à la compta
if (!mysqli_select_db($db,$base_compta))
{
  print "Erreur ".mysqli_error($db)."<br>";
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

$total="";


//recuperation d'une saisie d'honoraires
if (isset($_REQUEST['Bouton_calculer']))
{
  $patient=$_REQUEST['patient'];
  $payeur=$_REQUEST['payeur'];
  $type_paiement=$_REQUEST['type_paiement'];
  $nom_acte=$_REQUEST['nom_acte'];
//calcul de la valeur totale des actes a partir de la valeur de chaque acte
  foreach ($nom_acte as $cet_acte)
  {
    $sql_trouver_actes_demande="SELECT * FROM actes_disponibles WHERE nom_acte='$cet_acte'";
    $resultat_trouver_actes_demande=mysqli_query($db,$sql_trouver_actes_demande);
    $ligne_trouver_actes_demande=mysqli_fetch_array($resultat_trouver_actes_demande);
    $total=$total+$ligne_trouver_actes_demande['montant_total'];
  }
  $actes="";
  $nom_acte=$_REQUEST['nom_acte'];
  foreach ($nom_acte AS $cet_acte)
  {
    $actes=$actes."&nom_acte[]=".$cet_acte;
  }
//On renvoie le resultat a la page d'affichage
//on repasse en format de date local
  $date_tableau=explode ("-",$date);
  if ($date_format=='fr')
    $date=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
      $date=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
  header('location: saisie_hono.php?total='.$total.'&patient='.$patient.'&payeur='.$payeur.'&type_paiement='.$type_paiement.$actes."&Bouton_calculer=Calculer&guid_patient=".$guid_patient.'&date='.$date.'&part_perso='.$part_perso );
  exit;
}

$connect=mysqli_select_db($db,$base);

if (isset($_REQUEST['guid_patient']))
{
  if ($guid_patient)
  {
    $sql_patient="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$guid_patient'";
    $resultat_patient=mysqli_query($db,$sql_patient);
    $ligne_patient=mysqli_fetch_array($resultat_patient);
    $nom_patient=$ligne_patient['FchGnrl_NomDos'];
    $prenom_patient=$ligne_patient['FchGnrl_Prenom'];
    $nom_complet=$nom_patient." ".$prenom_patient;
  }
  else ///on travaille sans GUID
  {
    if (isset($_REQUEST['patient']))//si un nom a ete donne
      $nom_patient=$_REQUEST['patient'];
    else
      $nom_patient="";
    $prenom_patient="";
    $nom_complet=$nom_patient; //sera introduit dans la requete
  }
}

if (isset ($_REQUEST['bouton_valider_hono']) OR isset ($_REQUEST['bouton_remise_cheque']))
{
//on recupere les lignes d'honoraire a valider
  $connect=mysqli_select_db($db,$base_compta);
  $id_honos=$_REQUEST['coche_selectionner_hono'];
  foreach ($id_honos AS $cet_hono)
  {
    $sql_valider_hono="UPDATE honoraires SET valide=1 WHERE id_hono='$cet_hono'";
    $resultat_valider_hono=mysqli_query($db,$sql_valider_hono);
  }
  if (isset ($_REQUEST['bouton_remise_cheque'])) //on envoie sur la page remise de cheque apres validation
  {
    $id_honos=$_REQUEST['coche_selectionner_hono'];
    $liste_comptes=$_REQUEST['liste_comptes'];
    $les_honos="";
    foreach ($id_honos AS $cet_hono)
    {
      $les_honos=$les_honos."id_hono[]=".$cet_hono."&";
    }
    header('location: bordereau_bancaire.php?'.$les_honos."liste_comptes=".$liste_comptes) ;
    exit;
  }
  else //seulement si valider recettes
  {
//on repasse en format de date local
  $date_tableau=explode ("-",$date); //iso
  if ($date_format=='fr')
    $date=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
      $date=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
    header('location: bilan_recettes.php?date='.$date); //local
    exit;
  }
}

if (isset ($_REQUEST['Bouton_enregistrer']) OR isset($_REQUEST['Bouton_enregistrer_modification']))
{
//	$guid_patient=$_REQUEST['guid_patient'];
	$payeur=$_REQUEST['payeur'];
	$nom_acte=$_REQUEST['nom_acte'];
	$type_paiement=$_REQUEST['type_paiement'];
	$total=$_REQUEST['total'];
	$esp="0";
	$chq="0";
	$cb="0";
	$du="0";
	$autre="0";
	$remarque="|dep|em=".$payeur;
	if ($part_perso=="0")
	{
	  $daf=$total;
	  $total=0;
	}
	else
	{
	  $total=$total*$part_perso/100;
	  $daf=$_REQUEST['total']-$total;
	}

	if ($type_paiement=='Espèces')
	{
	  $esp=$total;
	  $esp_unitaire="";
	}
	elseif ($type_paiement=='Chèque')
	{
	  $chq=$total;
	}
	elseif ($type_paiement=='CB')
        {
	  $cb= $total;
        }
//	elseif ($type_paiement=='DAF')
//        {
//	  $daf= $total; 
//        }
	elseif ($type_paiement=='Dû')
        {
	  $du = $total;
        }
	elseif ($type_paiement=='Autre')
        {
	  $autre = $total;
       }
	$acte_complet[]="";
	$actes="";
	$compteur=0;
        $connect=mysqli_select_db($db,$base_compta);

	foreach ($nom_acte as $cet_acte)
	{
		$sql_chercher_nom_acte="SELECT * FROM  actes_disponibles WHERE nom_acte='$cet_acte'";
		$resultat_chercher_acte=mysqli_query($db,$sql_chercher_nom_acte);
		$ligne_chercher_acte=mysqli_fetch_array($resultat_chercher_acte);
		$esp_unitaire=0;
		$chq_unitaire=0;
		$cb_unitaire=0;
		$daf_unitaire=0;
		$du_unitaire=0;
		$autre_unitaire=0;
		if ($type_paiement=='Espèces')
			$esp_unitaire=$ligne_chercher_acte['montant_total'];
		elseif ($type_paiement=='Chèque')
			$chq_unitaire=$ligne_chercher_acte['montant_total'];
		elseif ($type_paiement=='CB')
			$cb_unitaire=$ligne_chercher_acte['montant_total'];
		elseif ($type_paiement=='DAF')
			$daf_unitaire=$ligne_chercher_acte['montant_total']; 
		elseif ($type_paiement=='Dû')
			$du_unitaire =$ligne_chercher_acte['montant_total'];
		elseif ($type_paiement=='Autre')
			$autre_unitaire=$ligne_chercher_acte['montant_total'];
		
		$acte_complet[$compteur]=$cet_acte."//".$ligne_chercher_acte['type']."//N//".$esp_unitaire."//".$chq_unitaire."//".$cb_unitaire."//".$daf_unitaire."//".$du_unitaire."//".$autre_unitaire;
		$compteur++;
	}
	for ($i=0;$i<$compteur;$i++)
	{
		$actes=$acte_complet[$i]."|".$actes;
	}
}//fin bouton enregistrer ou modifier
if (isset ($_REQUEST['Bouton_enregistrer']))
{
	$sql_inserer_recette=sprintf("INSERT INTO honoraires (id_usr,id_drtux_usr,patient,GUID,praticien,date,acte,remarque,esp,chq,cb,daf,autre,du,valide,tracabilite) VALUES ('$id_user','$id_drtux_user','%s','$guid_patient','%s','$date','$actes','%s','$esp','$chq','$cb','$daf','$autre','$du','0','')",mysqli_real_escape_string($db,$nom_complet),mysqli_real_escape_string($db,$praticien),mysqli_real_escape_string($db,$remarque));
//echo $sql_inserer_recette;
	$connect=mysqli_select_db($db,$base_compta);
	$resultat_inserer_recette=mysqli_query($db,$sql_inserer_recette);
  $date_tableau=explode ("-",$date);
  if ($date_format=='fr')
    $date=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
      $date=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];

	header('location: saisie_hono.php?date='.$date);
	exit;
}
// modification d'honoraires
//guid_patient=b0fa85e1-917c-4bc7-aac7-2737ff2e40f9&patient=xxx&payeur=xxx&date=2009-09-19&nom_acte[]=C&total=22&type_paiement=Espèces&Bouton_enregistrer_modification=Modifier
if (isset($_REQUEST['Bouton_enregistrer_modification']))
{
  $id_hono=$_REQUEST['id_hono'];
	$sql_modifier_recette=sprintf("UPDATE honoraires SET patient='%s',date='$date',acte='$actes',remarque='%s',esp='$esp',chq='$chq',cb='$cb',daf='$daf',autre='$autre',du='$du' WHERE id_hono='$id_hono'",mysqli_real_escape_string($db,$nom_complet),mysqli_real_escape_string($db,$remarque));
//echo $sql_modifier_recette;
	$connect=mysqli_select_db($db,$base_compta);
	$resultat_modifier_recette=mysqli_query($db,$sql_modifier_recette);
//on repasse en format de date local
  $date_tableau=explode ("-",$date);
  if ($date_format=='fr')
    $date=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
  elseif ($date_format=='en')
      $date=$date_tableau[1]."-".$date_tableau[2]."-".$date_tableau[0];
	header('location: saisie_hono.php?date='.$date );
	exit;
}

if (isset($_REQUEST['bouton_modifier_hono']))
{
    $id_hono="";
    $connect=mysqli_select_db($db,$base_compta);
//echo "pas fait";
    $id_hono=$_REQUEST['bouton_modifier_hono'];
    $cet_hono=$id_hono[0];
    $sql_chercher_date="SELECT date FROM honoraires WHERE id_hono='$cet_hono'";
    $resultat_chercher_date=mysqli_query($db,$sql_chercher_date);
    $ligne_chercher_date=mysqli_fetch_array($resultat_chercher_date);
    $date=$ligne_chercher_date['date'];
  header('location: saisie_hono.php?radio_selectionner_hono='.$id_hono.'&date='.$date );
//  header('location: saisie_hono.php?radio_selectionner_hono='.$id_hono );
  exit;
}

if (isset($_REQUEST['bouton_supprimer_hono']))
{
  $connect=mysqli_select_db($db,$base_compta);
  if (isset($_REQUEST['coche_confirmation_suppression']))
  {
    if (isset($_REQUEST['coche_selectionner_hono']))
    {
      $id_hono=$_REQUEST['coche_selectionner_hono'];
      foreach ($id_hono AS $cet_hono)
      {
	$sql_supprimer_hono="DELETE FROM honoraires WHERE id_hono='$cet_hono'";
	$resultat_supprimer_hono=mysqli_query($db,$sql_supprimer_hono);

      }
    }
    else
      $id_hono="";
	  header('location: saisie_hono.php' );
	  exit;
  }
  else
	header('location: saisie_hono.php' );
	exit;
}

//echo "fin";

?>
