<?php
session_start() ;
include("../config.php");
//cette page n'est pas accessible depuis les pages habituelles.
//elle ne sert qu'à fusionner toutes les ordonnances de meme titre (ORDONNANCE) et de meme horodatage en une seule
//utile pour les migrations en provenance de certains logiciels où chaque ligne d'une ordonnance a sa propre entree dans la BDD.
if ( !isset( $_SESSION['login'] ) )
{
	header('location: index.php?page=fusion' );
	exit;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//Dtd XHTML 1.0 Strict//FR"
		"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="fr" xml:lang="fr">
  <head>
    <meta name="GENERATOR" content="PHP"></meta>
    <META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=UTF-8">
    <title>MedinTux - Fusion des ordos</title>
  </head>
	
  <body>
<table>
	<tr>
		<td style="background:#FFe7f3;color:#003366;"><a href="agenda.php">Agenda</a></td>
		<td>Choix d'un patient</td>
		<td style="background:#BAE7FF;color:#003366;"><a href="correspondant.php">Intervenants</a></td>
		<td style="background:#DCF9FF;color:#003366;"><a href="activite.php">Activité</a></td>
		<td style="background:#FFEAD5;color:#003366;"><a href="backup.php">Sauvegarde</a></td>
		<td style="background:#FFFFAA;color:#003366;"><a href="lisezmoi.html">À propos</a></td>
		<td style="background:#DFDAFF;color:#003366;"><a href="index.php">Se déconnecter</a></td>
		<td>MedWebTux version <?php echo $version ?></td>
	</tr>
</table>

<?php

$db=mysql_connect($host, $loginbase, $pwd);

if(!$db)
{
	print "Erreur de connexion à la base<br>";
	exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}
?>

<center><h1><b>MedinTux - Fusion des ordos</b></h1></center>

<?php
 
//On cherche tous les patients
$sql_chercher_patients="SELECT * FROM IndexNomPrenom";
//$sql_chercher_patients="SELECT * FROM IndexNomPrenom WHERE ID_PrimKey=2780";
$resultat_chercher_patients=mysql_query($sql_chercher_patients,$db);
$count_chercher_patients=mysql_num_rows($resultat_chercher_patients);
echo "nombre de patients".$count_chercher_patients."<br>";//pour info
$expr="(<body[^>]*>)(.*)(<\/body>)";
//On analyse les patients un par un
$compteur_patient=0;
while ($ligne_chercher_patients=mysql_fetch_array($resultat_chercher_patients))
{
  $ce_patient_id=$ligne_chercher_patients['FchGnrl_IDDos'];
  echo "ce patient".$ce_patient_id."<br>";//pour info
  $ce_patient_num_dossier=$ligne_chercher_patients['ID_PrimKey'];
//On cherche toutes les dates pour ce patient
//on ne regroupera que les dates où l'horodatage complet est identique, c'est-a dire avec un timestamp de 00:00:00, autrement dit les ordos d'avant MedinTux
  $sql_chercher_dates="SELECT RbDate_Date FROM RubriquesHead WHERE RbDate_NomDate=\"ORDONNANCE\" AND  RbDate_TypeRub='20020200' AND RbDate_IDDos='$ce_patient_id' GROUP BY RbDate_Date";
  $resultat_chercher_dates=mysql_query($sql_chercher_dates,$db);
  $count_chercher_dates=mysql_num_rows($resultat_chercher_dates);
  echo "nombre_de dates pour ce patient".$count_chercher_dates."<br>";//pour info
  $compteur_patient++;
//  if ($compteur_patient==10) // pour tests. On quitte apres les 10 premiers dossiers
//    break;  
//On analyse chaque date pour ce patient
  while($ligne_chercher_dates=mysql_fetch_array($resultat_chercher_dates))
  {
    $cette_date=$ligne_chercher_dates['RbDate_Date'];
    echo $cette_date;//pour info
    $sql_chercher_les_ordos_de_ce_patient_cette_date="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesHead.RbDate_NomDate=\"ORDONNANCE\" AND  RubriquesHead.RbDate_TypeRub='20020200' AND RubriquesHead.RbDate_IDDos='$ce_patient_id' AND RubriquesHead.RbDate_Date='$cette_date'";
    $resultat_chercher_les_ordos_de_ce_patient_cette_date=mysql_query($sql_chercher_les_ordos_de_ce_patient_cette_date,$db);
    $count_chercher_les_ordos_de_ce_patient_cette_date=mysql_num_rows($resultat_chercher_les_ordos_de_ce_patient_cette_date);
    echo "nombre_d'ordos pour cette date pour ce patient".$count_chercher_les_ordos_de_ce_patient_cette_date."<br>";//pour info      
    if ($count_chercher_les_ordos_de_ce_patient_cette_date > 1) //on ne traite que si plus d'une ordonnance ce jour pour ce patient
    {
      $ordonnance_complete="";
  //on colle ensemble toutes les ordonnances de ce jour pour ce patient
      $tab="";
      while ($ligne_chercher_les_ordos_de_ce_patient_cette_date=mysql_fetch_array($resultat_chercher_les_ordos_de_ce_patient_cette_date))
      {
	$fiche_mieux=(eregi($expr,$ligne_chercher_les_ordos_de_ce_patient_cette_date["RbDate_DataRub"],$tab));
	$fiche_encore_mieux=eregi_replace("<body.*>", " ",$tab[2]);
//	$synthese=$synthese."<br>".$fiche_encore_mieux;
	$ordonnance_complete=$ordonnance_complete.$fiche_encore_mieux;
      }
      echo $ordonnance_complete;
      $insertion="<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:10pt;font-family:Arial\">".$ordonnance_complete."</body></html>\n\0\0";
//on recharge la liste des ordos
      $resultat_chercher_les_ordos_de_ce_patient_cette_date=mysql_query($sql_chercher_les_ordos_de_ce_patient_cette_date,$db);
//On insere la nouvelle ordo, qui ne sera pas dans la liste des ordos a supprimer
      $sql_inserer_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$ce_patient_id')",mysql_real_escape_string($insertion));
echo "inserer_blob ".$sql_inserer_blob."<br>"; //pour info
      $resultat=mysql_query($sql_inserer_blob,$db);
      $id_blob=mysql_insert_id();
      $sql_inserer_header="INSERT INTO RubriquesHead (`RbDate_IDDos`,`RbDate_TypeRub`,`RbDate_NomDate`,`RbDate_Date`, `RbDate_CreateUser`,`RbDate_Duree`,`RbDate_Ref_NumDoss`,`RbDate_RefBlobs_PrimKey`) VALUES('$ce_patient_id','20020200','Ordo','$cette_date','delafond','0','$ce_patient_num_dossier','$id_blob')";
echo "inserer_header ".$sql_inserer_header."<br>";//pour info
//on supprime toutes les anciennes ordos qui ont servi a generer la nouvelle
      $resultat=mysql_query($sql_inserer_header,$db);
      while ($ligne_chercher_les_ordos_de_ce_patient_cette_date=mysql_fetch_array($resultat_chercher_les_ordos_de_ce_patient_cette_date))
      {
	$numero_ordo=$ligne_chercher_les_ordos_de_ce_patient_cette_date['RbDate_PrimKey'];
	$sql_supprimer_header="DELETE FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$numero_ordo'";
echo "supprimer header ".$sql_supprimer_header."<br>";//pour info
	$resultat=mysql_query($sql_supprimer_header,$db);
	$sql_supprimer_blob="DELETE FROM RubriquesBlobs WHERE RbDate_PrimKey='$numero_ordo'";
echo "supprimer blob".$sql_supprimer_blob."<br>";
	$resultat=mysql_query($sql_supprimer_blob,$db);//pour info
      }
    }
  }
}

// on ferme la base
mysql_close($db);

?>

</body>

</html>
