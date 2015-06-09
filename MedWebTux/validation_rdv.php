<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=agenda' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);

extract($_POST);
$RDV_PrisPar=$tab_login[0];
$RDV_PrisAvec=$tab_login[1];

//on recupere toutes les variables de l'URL
//Valeurs recuperees
//Array ( [utilisateur_autorisant] => ines [RDV_PrisAvec] => delafond [intervenants] => seul [Date] => 16-01-2013 [Heure] => 00 [Minutes] => 00 [Type] => Consultation [Duree] => 15 [status] => Statut non défini [GUID] => [Nom] => [Prenom] => [Tel] => [RDV_PrisPar] => delafond [Note] => [fin] => 16-01-2013 [precedent] => 2013-01-15 [today_debut] => 2013-01-16 [suivant] => 2013-01-17 ) 

//on va tout de suite en page de rdv repetes si bouton repeter
//transmettre
//Type  Status  Nom  Prenom GUID Duree Tel Time Date Notes
//Transfert vers la page des repetitions
if (isset($_POST['button_repeat']))
{
  header("location:rdv_repetes.php?Type=$Type&status=$status&Nom=$Nom&Prenom=$Prenom&GUID=$GUID&Duree=&Tel=$Tel&Time=&Date=$Date&Notes=$Notes");
  exit;
}
$pris_avec=0;
if (isset($_POST['utilisateur_autorisant']))
{
    $RDV_PrisAvec=$_POST['utilisateur_autorisant'];
    $pris_avec=1;
}

include("config.php");

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

if (isset($_REQUEST['id_rdv']))
  $id_rdv=$_REQUEST['id_rdv'];
$envoyer='';
if (isset($_REQUEST['bouton_envoyer']))
  $envoyer=$_REQUEST['bouton_envoyer'];

$Nom='';
$Prenom='';
$Tel='';
$GUID="";
$intervenants="";
$status="";

$check_name_string='';
if (isset($_REQUEST['check_name']))
  $check_name_string="&check_name=";

if (isset($_REQUEST['Nom']))
  $Nom=$_REQUEST['Nom'];
if (isset($_REQUEST['Prenom']))
  $Prenom=$_REQUEST['Prenom'];
if (isset($_REQUEST['Tel']))
{
  $Tel=$_REQUEST['Tel'];
  $Tel=str_replace(' ','',$Tel);
}
if (isset($_REQUEST['Note']))
  $Note=$_REQUEST['Note'];
if (isset($_REQUEST['Type']))
  $Type=$_REQUEST['Type'];
if (isset($_REQUEST['GUID']))
  $GUID=$_REQUEST['GUID'];
if (isset($_REQUEST['intervenants']))
  $intervenants=$_REQUEST['intervenants'];
if (isset($_REQUEST['status']))
  $status=$_REQUEST['status'];

if (isset($_REQUEST['Heure']))
	$Heure=$_REQUEST['Heure'];
if (isset($_REQUEST['Minutes']))
	$Minutes=$_REQUEST['Minutes'];
if (isset($_REQUEST['Duree']))
	$Duree=$_REQUEST['Duree'];
$Time=$Heure.':'.$Minutes;
//echo $date_format;
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

$DateTime=local_to_iso(substr($Date,0,10),$date_format).' '.$Time.':00'; // On s'assure que la date ne comporte pas l'heure et on ajoute l'heure - mode un seul champ pour toute la date

$debut=substr($Date,0,10);

//transfert des boutons de recherche de rendez-vous
if (isset($_POST['envoyer']) OR isset($_POST['button_change_user']) OR isset($_POST['select_user']))
{
  $envoyer=$_REQUEST['envoyer'];
  header("location:agenda.php?Type=$Type&status=$status&Nom=$Nom&Prenom=$Prenom&GUID=$GUID&Heure=$Heure&Minutes=$Minutes&Duree=$Duree&Tel=$Tel&Time=$Time&debut=$Date&fin=$fin&Notes=$Notes&envoyer=$envoyer&utilisateur_autorisant=$RDV_PrisAvec&intervenants=$intervenants&precedent=$precedent&suivant=$suivant&today_debut=$today_debut $check_name_string");
  exit;
}
if (isset($_REQUEST['Date'])) //mode provenance de l'agenda
{
  $Date_debut=$Date_fin=$Date=$_REQUEST['Date']; //format local
}

if ($envoyer=="Modifier")
{
  $sql=sprintf("UPDATE agenda SET Date_Time='$DateTime',Duree='$Duree',Nom='%s',Prenom='%s',RDV_PrisPar='%s',RDV_PrisAvec='%s',Tel='$Tel',Note='%s',Type='$Type',GUID='$GUID',status='%s',`lock`='0' WHERE PrimKey='$id_rdv'",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom),mysqli_real_escape_string($db,$RDV_PrisPar),mysqli_real_escape_string($db,$RDV_PrisAvec),mysqli_real_escape_string($db,$Note),mysqli_real_escape_string($db,$status));
  $resultat=mysqli_query($db,$sql);
}
elseif ($envoyer=="Ajouter")
{
  $sql=sprintf("INSERT INTO agenda (Date_Time,Duree,Nom,Prenom,Tel,RDV_PrisPar,RDV_PrisAvec,Note,Type,GUID,status) 
		      VALUES ('$DateTime','$Duree','%s','%s','$Tel','%s','%s','%s','$Type','$GUID','$status')",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom),mysqli_real_escape_string($db,$RDV_PrisPar),mysqli_real_escape_string($db,$RDV_PrisAvec),mysqli_real_escape_string($db,$Note));
  $resultat=mysqli_query($db,$sql);
}

elseif ($envoyer=="Répéter")
{
  $heures_a_rajouter=0;
  if ($Duree>59)
    $heures_a_rajouter=intval($Duree/60); //on trouve combien la duree contient d'heures

  $min_a_rajouter=$Duree-($heures_a_rajouter*60);//on retire x fois 60 min de la duree
  if ($heures_a_rajouter < 10) //on ajoute le 0 initial entre 0 et 9
    $heures_a_rajouter='0'.$heures_a_rajouter;
  if ($min_a_rajouter < 10)
    $min_a_rajouter='0'.$min_a_rajouter;
//&jour_debut_plage=16-01-2011&jour_fin_plage=17-01-2011&Heure_debut_plage=&Minutes_debut_plage=00&Heure_fin_plage=&Minutes_fin_plage=00&envoyer=Répéter
 //plages horaires pour les rdv repetes
  if (isset($_REQUEST['jour_debut_plage']))
    $Date_debut=$jour_debut_plage=$_REQUEST['jour_debut_plage'];
  if (isset($_REQUEST['jour_fin_plage']))
    $Date_fin=$jour_fin_plage=$_REQUEST['jour_fin_plage'];

  if (isset($_REQUEST['Heure_debut_plage']))
    $Heure_debut_plage=$_REQUEST['Heure_debut_plage'];
  if (isset($_REQUEST['Minutes_debut_plage']))
    $Minutes_debut_plage=$_REQUEST['Minutes_debut_plage'];
  if (isset($_REQUEST['Heure_fin_plage']))
    $Heure_fin_plage=$_REQUEST['Heure_fin_plage'];
  if (isset($_REQUEST['Minutes_fin_plage']))
    $Minutes_fin_plage=$_REQUEST['Minutes_fin_plage'];
  if (isset($_REQUEST['jours']))
    $jours=$_REQUEST['jours'];

//initialisation des jours de la semaine
  $text_jsemaine=array(0=>"Dimanche",1=>"Lundi",2=>"Mardi",3=>"Mercredi",4=>"Jeudi",5=>"Vendredi",6=>"Samedi");

  $heure_complete_debut_plage=$Heure_debut_plage.':'.$Minutes_debut_plage; //hh:mm
//echo  $heure_complete_fin_plage=$Heure_fin_plage.':'.$Minutes_fin_plage;//hh:mm
  $heure_debut_dernier_rdv=$Heure_fin_plage-$heures_a_rajouter;//hh:mm
  if ($heure_debut_dernier_rdv < 10)
    $heure_debut_dernier_rdv='0'.$heure_debut_dernier_rdv;

  $minutes_debut_dernier_rdv=$Minutes_fin_plage-$min_a_rajouter;
  if ($minutes_debut_dernier_rdv<10)
    $minutes_debut_dernier_rdv='0'.$minutes_debut_dernier_rdv;

  $horaire_debut_dernier_rdv=$heure_debut_dernier_rdv.':'.$minutes_debut_dernier_rdv;

  $list_jour_debut_plage=explode ("-",$jour_debut_plage);
  $list_jour_fin_plage=explode ("-",$jour_fin_plage);
  if ($date_format=='fr')
  {
    $day_debut=$list_jour_debut_plage[0];
    $month_debut=$list_jour_debut_plage[1];
    $year_debut=$list_jour_debut_plage[2];
    $day_fin=$list_jour_fin_plage[0];
    $month_fin=$list_jour_fin_plage[1];
    $year_fin=$list_jour_fin_plage[2];
  }
  elseif ($date_format=='en')
  {
    $day_debut=$list_jour_debut_plage[1];
    $month_debut=$list_jour_debut_plage[0];
    $year_debut=$list_jour_debut_plage[2];
    $day_fin=$list_jour_fin_plage[1];
    $month_fin=$list_jour_fin_plage[0];
    $year_fin=$list_jour_fin_plage[2];
  }//si deja iso, 
  else
  {
    $day_debut=$list_jour_debut_plage[2];
    $month_debut=$list_jour_debut_plage[1];
    $year_debut=$list_jour_debut_plage[0];
    $day_fin=$list_jour_fin_plage[2];
    $month_fin=$list_jour_fin_plage[1];
    $year_fin=$list_jour_fin_plage[0];
  }
  $jour_debut_plage_sql="$year_debut-$month_debut-$day_debut";
  $jour_fin_plage_sql="$year_fin-$month_fin-$day_fin";
    list ($year_debut,$month_debut,$day_debut)=explode('-',$jour_debut_plage_sql);

  while ($jour_debut_plage_sql<=$jour_fin_plage_sql) //on analyse les jours un par un
  {
    $jd = date("w",mktime(0,0,0,$month_debut,$day_debut,$year_debut));//numero du jour de la semaine
    $jour_debut_plage_sql=date("Y-m-d",mktime(0, 0, 0,$month_debut,$day_debut,$year_debut));
    //on analyse les heures dans ce jour - on reinitialise pour chaque jour
    $heure_complete_debut_rdv=$heure_complete_debut_plage; //on initialise le premier rendez-vous
    foreach ($jours AS $ce_jour)
    {
      if ($ce_jour==$text_jsemaine[$jd])
      {
	$somme_heure=0;
	$somme_minutes=0;

//$heure_debut_dernier_rdv=$heure_complete_fin_plage-$Duree;

	while ($heure_complete_debut_rdv <= $horaire_debut_dernier_rdv) 
	{
	  $DateTime=$jour_debut_plage_sql.' '.$heure_complete_debut_rdv;
//controle de doublons : ne pas inserer si ce rdv existe deja
	  $sql_chercher_doublon=sprintf("SELECT * FROM agenda WHERE Date_Time='$DateTime' AND Duree='$Duree' AND RDV_PrisAvec='%s'",mysqli_real_escape_string($db,$RDV_PrisAvec));
	  $resultat_chercher_doublon=mysqli_query($db,$sql_chercher_doublon);
	  $count_chercher_doublon=mysqli_num_rows($resultat_chercher_doublon);
	  if (!$count_chercher_doublon)
	  {
	    $sql=sprintf("INSERT INTO agenda (Date_Time,Duree,Nom,Prenom,Tel,RDV_PrisPar,RDV_PrisAvec,Note,Type,GUID,status) 
			      VALUES ('$DateTime','$Duree','%s','%s','$Tel','%s','%s','%s','$Type','$GUID','$status')",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom),mysqli_real_escape_string($db,$RDV_PrisPar),mysqli_real_escape_string($db,$RDV_PrisAvec),mysqli_real_escape_string($db,$Note));
	    $resultat=mysqli_query($db,$sql); 
	  }
	  list($heure_rdv,$min_rdv)=explode(":",$heure_complete_debut_rdv);
    //on cherche la fin du rendez-vous
	  $somme_heure=$heure_rdv+$heures_a_rajouter;
	  if ($somme_heure < 10) //on ajoute le 0 initial entre 0 et 9
	    $somme_heure='0'.$somme_heure;

	  $somme_minutes=$min_rdv+$min_a_rajouter;
	  if ($somme_minutes > 59) //on ajoute 1 heure si min > 60
	  {
	    $somme_minutes=$somme_minutes-60;
	    $somme_heure++;
	    if ($somme_heure < 10) //on ajoute le 0 initial entre 0 et 9
	      $somme_heure='0'.$somme_heure;
	    if ($somme_minutes < 10)
	      $somme_minutes='0'.$somme_minutes;
	  }
    //on cherche le debut du rendez-vous suivant
	  $heure_complete_debut_rdv=$somme_heure.':'.$somme_minutes;
	} //fin boucle heures
      } //fin jour semaine OK
    } //fin foreach jours semaine
    $day_debut=$day_debut+1;
  } //fin boucle jours
} //fin mode repeter

header("location: agenda.php?debut=".$Date_debut."&fin=".$Date_fin."&envoyer=Chercher&nom=%&critere_recherche=Nom&tri=Date_Time&intervenants=$intervenants&utilisateur_autorisant=$RDV_PrisAvec" );
exit;
?>

