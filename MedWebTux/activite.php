<?php
session_start() ;
include("config.php");
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$signuser=$tab_login[1];
//error_reporting(-1);

if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=activite' );
  exit;
}

if (isset($_GET['utilisateur_autorisant'])) //si on recupere un UA
{
  $_SESSION['login'] = $user."::".$_GET['utilisateur_autorisant'];
  $tab_login=explode("::",$_SESSION['login']);
  $signuser=$tab_login[1];
}

include("inc/header.php");
?>
    <title>
      Activit&eacute; de <?php echo $_SESSION['login']?>
    </title>

    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
    <link rel="stylesheet" href="css/print.css" type="text/css" media="print" />
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt"  >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor="Mode_Activité";
include("inc/menu-horiz.php");
?>
<?php
//les dates de recherche selon la langue
if ($date_format=='fr')
  $date=date('d-m-Y', date('U'));
elseif ($date_format=='en')
  $date=date('m-d-Y', date('U'));
else 
 $date=date('Y-m-d', date('U')); // la date du jour

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; ".$host."<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
//on se connecte à la compta
if (!mysqli_select_db($db,$base_compta))
{
  print "Erreur ".mysqli_error()."<br />";
  mysqli_close($db);
  exit;
}
$groupby="";
$critere_recherche="aucun";

if (isset($_GET['critere_recherche']))
  $critere_recherche=$_GET['critere_recherche'];

if (isset($_GET['groupby']))
{
  if ($_GET['groupby']=="Grouper par nom")
    $groupby=" GROUP BY FchGnrl_IDDos ";
}
  
//Recherche si l'utilisateur a des droits admin
$sql_droit_admin="SELECT Droits FROM Personnes WHERE Login='$user'";
$connect=mysqli_select_db($db,$base);
$resultat_droit_admin=mysqli_query($db,$sql_droit_admin);
$ligne_droit_admin=mysqli_fetch_array($resultat_droit_admin);
?>

 <div class="groupe">
      <h1>
	MedWebTux - Activit&eacute; de 

<?php
if (stripos($ligne_droit_admin["Droits"],'adm')) //si oui, on l'autorise a chercher l'activite des autres
{
?>
	  <select name="utilisateur_autorisant" id="utilisateur_autorisant" onchange="form.submit()">
	    <option value=""></option>
<?php
//  echo "<a href=\"activite.php?utilisateur=tous\">tous</a>";
  $sql_utilisateurs="SELECT Login,Droits from Personnes";
  $connect=mysqli_select_db($db,$base);
  $resultat_utilisateurs=mysqli_query($db,$sql_utilisateurs);
  if (isset ($_GET['utilisateur'])) //On ne recupere l'utilisateur demande que si l'on a le droit admin
    $utilisateur=$_GET['utilisateur'];
  else
    $utilisateur=$signuser;
  while ($ligne_utilisateurs=mysqli_fetch_array($resultat_utilisateurs))
  {
    if ($ligne_utilisateurs["Login"] AND preg_match('/sgn/',$ligne_utilisateurs['Droits']) AND preg_match('/obc/',$ligne_utilisateurs['Droits']))
    {
?>
	      <option value="<?php echo $ligne_utilisateurs["Login"] ?>" <?php if ($ligne_utilisateurs["Login"]==$utilisateur) echo "selected='selected'" ?>>
		<?php echo $ligne_utilisateurs["Login"] ?>
	      </option>
<?php
    }
  }
?>
	    </select>
<?php
}
else
{
	$utilisateur=$signuser;
 echo $utilisateur ;
}
?>
      </h1>
<?php
	
//formatage des dates pour les petites cases
if (isset($_GET['envoyer_date'])) 
{
  $envoyer_date=$_GET['envoyer_date'];
  if ($envoyer_date==">>" or $envoyer_date=="<<")//recuperation des fleches
  {
    $debut=$_GET['debut']." 00:00:00";
    $fin=$_GET['debut']." 23:59:59";
  }
  if (isset($_GET['debut'])) // on vient d'une recherche de date
  {
    $debut=$_GET['debut'];
    $fin=$_GET['fin'];
//On modifie la date de fin = date début si elle etait anterieure
    $list_debut=explode("-",$debut);
    $list_fin=explode("-",$fin);
    if ($date_format=='fr')
    {
      $date_debut=$list_debut[2].'-'.$list_debut[1].'-'.$list_debut[0];
      $date_fin=$list_fin[2].'-'.$date_debut=$list_fin[1].'-'.$date_debut=$list_fin[0];
    }
    elseif ($date_format=='en')
    {
      $date_debut=$list_debut[2].'-'.$list_debut[0].'-'.$list_debut[1];
      $date_fin=$date_debut=$list_fin[2].'-'.$date_debut=$list_fin[0].'-'.$date_debut=$list_fin[1];
    }
//si deja iso, on ne fait rien
    if ($date_debut > $date_fin)
      $fin=$debut;
    $debut=$debut." 00:00:00";
    $fin=$fin." 23:59:59";
  }
}
else //au lancement sans argument
{
//les dates de recherche selon la langue
  if ($date_format=='fr')
    $date=date('d-m-Y', date('U'));
  elseif ($date_format=='en')
    $date=date('m-d-Y', date('U'));
  else 
    $date=date('Y-m-d', date('U')); // la date du jour
  $debut=$date." 00:00:00";
  $fin=$date." 23:59:59";
}
$debut_court=substr($debut,0,10);
$fin_court=substr($fin,0,10);

//le tri par nom
$tri="RbDate_Date";
if (isset($_GET['tri']))
  $tri="FchGnrl_NomDos";

//On ajoute les jokers de recherche
$cle="";
if (isset($_GET['cle']))
{
  $cle_joker="%".$_GET['cle']."%";
  $cle=$_GET['cle'];
}

$afficher_tout="";
if (isset($_GET['bouton_afficher_tout']))
  $afficher_tout=$_GET['bouton_afficher_tout'];

?>
   
<div class="noPrint tableau">
<!--Formulaire de choix de date-->
	<form action="activite.php" method="get" id="form_jour">
	  <div>
	    <label for="datepicker">
	      D&eacute;but&nbsp;: 
	    </label>
	    <input name="utilisateur" type="hidden" value="<?php echo $utilisateur ?>" />
	    <input name="debut" id="datepicker" type="text" value="<?php echo $debut_court; ?>" size="10" maxlength="10" />
	    <label for="datepickeur">
	      Fin&nbsp;: 
	    </label>
	    <input name="fin" id="datepickeur" type="text" value="<?php echo $fin_court; ?>" size="10" maxlength="10" />
	    <label for="critere_recherche">
	      Filtre :	
	    </label>
	    <select name="critere_recherche" id="critere_recherche" title="Si Aucun (filtre), on ne tient pas compte de la case de droite mais seulement de la p&eacute;riode de dates"><!--Liste des cles de recherche-->
	      <option selected="selected" value="aucun">
		Aucun
	      </option>
	      <option value="consultation" <?php if ($critere_recherche=="consultation") echo "selected='selected'"?> >
		Consultation
	      </option>
	      <option value="ordonnance" <?php if ($critere_recherche=="ordonnance") echo "selected='selected'"?> >
		Ordonnance
	      </option>
	      <option value="courrier" <?php if ($critere_recherche=="courrier") echo "selected='selected'"?> >
		Courrier
	      </option>
	      <option value="document" <?php if ($critere_recherche=="document") echo "selected='selected'"?> >
		Documents
	      </option>
	      <option value="notes" <?php if ($critere_recherche=="notes") echo "selected='selected'"?> >
		Notes
	      </option>
	      <option value="notes_avancees" <?php if ($critere_recherche=="notes_avancees") echo "selected='selected'"?> >
		Notes avancées
	      </option>
	    </select>
	    <input name="cle" type="text" value="<?php echo $cle ?>" size="40" title="Mot &agrave; chercher dans l'ensemble des dossiers en fonction du filtre choisi &agrave; gauche et de la p&eacute;riode" />
	    <input name="envoyer_date" type="submit" value="Chercher" />
	  </div>
	</form>
<?php
function fleches($db,$utilisateur,$debut_day,$debut_month,$debut_year,$fin_day,$fin_month,$fin_year,$critere_recherche,$afficher_tout,$cle,$format)
{
?>
      <div class='noPrint'>
    <!--Les fleches jour suivant et jour precedent-->
	<div class="center_precsuiv">
	<div class="align_precsuiv">
	  <form action="activite.php" method="get"><!--Formulaire de recherche de date-->
	    <div>
	      <input name="utilisateur" type="hidden" value="<?php echo $utilisateur ?>" />
	      <input name="debut" type="hidden" value="<?php echo date($format, mktime(0, 0, 0,$debut_month,$debut_day-1,$debut_year)); ?>" />
	      <input name="fin" type="hidden" value="<?php echo date($format, mktime(0, 0, 0,$debut_month,$debut_day-1,$debut_year)); ?>" />
	      <input name="cle" type="hidden" value="<?php echo $cle; ?>" />
	      <input name="critere_recherche" type="hidden" value="<?php if (isset($critere_recherche)) echo $critere_recherche;
//bouton_afficher_tout=Montrer+les+consultations
else
echo 'aucun'; ?>" />
	      <input name="bouton_afficher_tout" type="hidden" value="<?php echo $afficher_tout?>" />
	      <input name="envoyer_date" type="submit" value="&lt;&lt;" title="jour pr&eacute;c&eacute;dent" />
	    </div>
	  </form>
	</div>
	<div class="align_precsuiv">
	  <form action="activite.php" method="get"><!--Formulaire de recherche de date-->
	    <div>
	      <input name="debut" type="hidden" value="<?php echo date($format, mktime(0, 0, 0,$fin_month,$fin_day+1,$fin_year)); ?>" />
	      <input name="fin" type="hidden" value="<?php echo date($format, mktime(0, 0, 0,$fin_month,$fin_day+1,$fin_year)); ?>" />
	      <input name="cle" type="hidden" value="<?php echo $cle ?>" />
	      <input name="critere_recherche" type="hidden" value="<?php if (isset($critere_recherche))
echo $critere_recherche;
else
echo 'aucun'; ?>" />
	      <input name="bouton_afficher_tout" type="hidden" value="<?php echo $afficher_tout?>" />
	      <input name="envoyer_date" type="submit" value="&gt;&gt;" title="jour suivant" />
	    </div>
	  </form>
      </div>
     </div>
    </div>
   <!--Les fleches jour suivant et jour precedent-->
   </div>

<?php
} //fin function fleches
//formatage des dates
list($debut_day_complete,$debut_time)=explode (" ",$debut);
list($fin_day_complete,$fin_time)=explode (" ",$fin);
$list_debut=explode ("-",$debut_day_complete);
$list_fin=explode ("-",$fin_day_complete);

if ($date_format=='fr')
{
  $debut_day=$list_debut[0];
  $debut_month=$list_debut[1];
  $debut_year=$list_debut[2];
  $fin_day=$list_fin[0];
  $fin_month=$list_fin[1];
  $fin_year=$list_fin[2];
  $format="d-m-Y";
}
 elseif ($date_format=='en')
{
  $debut_day=$list_debut[1];
  $debut_month=$list_debut[0];
  $debut_year=$list_debut[2];
  $fin_day=$list_fin[0];
  $fin_month=$list_fin[1];
  $fin_year=$list_fin[2];
  $format="m-d-Y";  
}
else //iso
{
  $debut_day=$list_debut[2];
  $debut_month=$list_debut[1];
  $debut_year=$list_debut[0];
  $fin_day=$list_fin[0];
  $fin_month=$list_fin[1];
  $fin_year=$list_fin[2];
  $format="Y-m-d";
}
$debut="$debut_year-$debut_month-$debut_day $debut_time";
$fin="$fin_year-$fin_month-$fin_day $fin_time";
fleches($db,$utilisateur,$debut_day,$debut_month,$debut_year,$fin_day,$fin_month,$fin_year,$critere_recherche,$afficher_tout,$cle,$format,$date_format);
?>
      </div>
<!-- fin noprint -->
</div>
<?php
/*$typeRub['ordonnance']="20020200";
$typeRub['consultation']="20030000";
$typeRub['courrier']="20020300";
$type_document_clair['20090000']='Notes avancées';
*/

if (stripos($ligne_droit_admin["Droits"],'adm') and $utilisateur=="tous")
  $utilisateurs_recherches="";//on cherche pour tous les utilisateurs
else
  $utilisateurs_recherches="AND RbDate_CreateSignUser='$utilisateur'";//on ne cherche que pour l'utilisateur demande


if ($critere_recherche=="ordonnance" or $critere_recherche=="consultation" or $critere_recherche=="courrier" or $critere_recherche=="document" OR  $critere_recherche=="notes_avancees")
{
  if ($critere_recherche=="ordonnance")
    $champ_recherche="(RbDate_TypeRub='20020200' OR RbDate_TypeRub='20020100')";
  elseif ($critere_recherche=="consultation")
    $champ_recherche="RbDate_TypeRub='20030000'";
  elseif ($critere_recherche=="courrier")
    $champ_recherche="RbDate_TypeRub='20020300'";
  elseif ($critere_recherche=="document")
    $champ_recherche="RbDate_TypeRub='20080000'";
  elseif ($critere_recherche=="notes_avancees")
    $champ_recherche="RbDate_TypeRub='20090000'";
  
  $sql=sprintf("SELECT * FROM RubriquesHead INNER JOIN IndexNomPrenom ON RubriquesHead.RbDate_IDDos=IndexNomPrenom.FchGnrl_IDDos INNER JOIN RubriquesBlobs ON RubriquesHead.RbDate_RefBlobs_PrimKey=RubriquesBlobs.RbDate_PrimKey WHERE RbDate_Date >= '$debut' AND RbDate_Date <= '$fin' AND $champ_recherche $utilisateurs_recherches AND CAST(RubriquesBlobs.RbDate_DataRub AS CHAR) LIKE '%s' $groupby ORDER BY '$tri'",mysqli_real_escape_string($db,$cle_joker));
}
elseif ($critere_recherche=="notes")
{
  $sql=sprintf("SELECT * FROM RubriquesHead INNER JOIN IndexNomPrenom ON RubriquesHead.RbDate_IDDos=IndexNomPrenom.FchGnrl_IDDos INNER JOIN  fchpat_Note ON IndexNomPrenom.FchGnrl_IDDos=fchpat_Note.fchpat_Note_PatGUID WHERE RbDate_Date >= '$debut' AND RbDate_Date <= '$fin' $utilisateurs_recherches AND CAST(fchpat_Note_Html AS CHAR) LIKE '%s' GROUP BY FchGnrl_IDDos",mysqli_real_escape_string($db,$cle_joker));
}
elseif ($critere_recherche=="aucun")
{
  $sql="SELECT * FROM RubriquesHead INNER JOIN IndexNomPrenom ON RubriquesHead.RbDate_IDDos=IndexNomPrenom.FchGnrl_IDDos INNER JOIN RubriquesBlobs ON RubriquesHead.RbDate_RefBlobs_PrimKey=RubriquesBlobs.RbDate_PrimKey  WHERE RbDate_Date >= '$debut' AND RbDate_Date <= '$fin' AND RbDate_TypeRub='20030000' $utilisateurs_recherches $groupby ORDER BY $tri";
}

$connect=mysqli_select_db($db,$base);
$resultat=mysqli_query($db,$sql) or die(mysqli_error);
$count=mysqli_num_rows($resultat);

//si on demande un correspondant
if ($critere_recherche=="correspondant")
{
  echo "
      Recherche par intervenant<br />";
  $intervenant=$_GET['intervenant'];
  //requete pour chercher, parmi les fiches trouvees, celles qui correspondent au correspondant
  $sql1="SELECT * FROM Personnes WHERE ID_PrimKey=$intervenant";
}
?>
    <div class='noPrint'>
     <div class="groupe">
	<h1>Nombre de contacts trouv&eacute;s

<?php
if ($debut_court==$fin_court)//Affichage si debut = fin
  echo " le <b>".$debut_court;
else
{
  echo " entre le <b>".$debut_court."</b> et le <b>".$fin_court;
}
if ($count) //pas d'affichage des boutons d'options si pas de donnees
{
  echo  "</b> : <b>".$count."</b>";
  if ($critere_recherche!="aucun" and $critere_recherche!="")
  {
    echo " pour la valeur <b>".$cle."</b> sur le filtre <b>".$critere_recherche."</b>.<br />";
  }
?>
</h1>
<!-- Bouton de groupement par nom -->
<!-- utilisateur=delafond&jour_debut=21&mois_debut=12&annee_debut=2008&jour_fin=21&mois_fin=12&annee_fin=2008&critere_recherche=aucun&cle=&envoyer_date=Chercher -->
<!-- utilisateur=tous&jour_debut=21&mois_debut=12&annee_debut=2008&jour_fin=21&mois_fin=12&annee_fin=2008&critere_recherche=&cle=&envoyer_date=Chercher&group=Grouper+par+nom -->
<!-- utilisateur=tous&jour_debut=21&mois_debut=12&annee_debut=2008&jour_fin=21&mois_fin=12&annee_fin=2008&critere_recherche=aucun&cle=&envoyer_date=Chercher&groupby=Grouper+par+nom -->
	<form action="activite.php" method="get">
	  <div>
	    <input name="utilisateur" type="hidden" value="<?php echo $utilisateur?>" />
	    <input name="debut" type="hidden" value="<?php echo $debut_court; ?>" />
	    <input name="fin" type="hidden" value="<?php echo $fin_court; ?>" />
	    <input name="critere_recherche" type="hidden" value="<?php echo $critere_recherche?>" />
	    <input name="cle" type="hidden" value="<?php echo $cle?>" />
	    <input name="envoyer_date" type="hidden" value="<?php echo $envoyer_date?>" />
<?php
  if   ($groupby==" GROUP BY FchGnrl_IDDos ")
  {
?>
	    <input name="groupby" type="submit" value="Ne pas grouper par nom"  title="Une ligne pour chaque contact avec un patient"/>
<?php
  }
  else
  {
?>
	    <input name="groupby" type="submit" value="Grouper par nom" title="Une seule ligne pour chaque patient différent" />
<?php
  }
?>
	  </div>
	</form>
	<form action="activite.php" method="get">
	  <div>
	    <input name="utilisateur" type="hidden" value="<?php echo $utilisateur?>" />
	    <input name="debut" type="hidden" value="<?php echo $debut_court; ?>" />
	    <input name="fin" type="hidden" value="<?php echo $fin_court; ?>" />
	    <input name="critere_recherche" type="hidden" value="<?php echo $critere_recherche?>" />
	    <input name="cle" type="hidden" value="<?php echo $cle?>" />
	    <input name="envoyer_date" type="hidden" value="<?php echo $envoyer_date?>" />
<?php
  if (!$afficher_tout)
  { 
?>
	    <input name="bouton_afficher_tout" type="submit" value="Montrer les consultations" />
<?php
  }
  else
  {
?>
	    <input name="bouton_cacher_tout" type="submit" value="Cacher les consultations" />
<?php
  }
?>
	  </div>
	</form>
      </div>
     </div>

   <div class="groupe">
     <div class="tableau">
      <table title="Un clic sur le nom envoie sur la fiche du patient
			Un clic sur la date envoie sur l'agenda &agrave; la date du jour
			Un clic sur l'heure envoie sur la consultation
			Un clic sur la t&ecirc;te de colonne Nom force le tri par ordre alphab&eacute;tique">
	<tr>
	  <th class="fond_th">
	    Date
	  </th>
	  <th class="fond_th">
	    Heure
	  </th>
	  <th class="fond_th">
	    <a href="activite.php?debut=<?php echo $debut_court?>&amp;fin=<?php echo $fin_court ?>&amp;cle=<?php echo $cle ?>&amp;critere_recherche=<?php echo $critere_recherche ?>&amp;envoyer_date=>>&amp;tri=nom">Nom</a>
	  </th>
	  <th class="fond_th">
	    Titre
	  </th>
<?php
  if ($afficher_tout)
  {
?>
	  <th class="fond_th">
	    Contenu
	  </th>
<?php
  }
?>
	  <th class="fond_th">
	    Recettes
	  </th>
	</tr>
<?php	
//?debut=2007-05-30&fin=2007-05-30&cle=&critere_recherche=aucun&envoyer_date=%3C%3C
  $expr="`(<body[^>]*>)(.*)(<\/body>)`is"; //expression pour enlever les balises de chaque observation
  while ($ligne=mysqli_fetch_array($resultat))
  {
    $date_result=substr($ligne["RbDate_Date"],8,2)."/".substr($ligne["RbDate_Date"],5,2)."/".substr($ligne["RbDate_Date"],0,4);
    $heure_result=substr($ligne["RbDate_Date"],11,5);
?>
	<tr  style="vertical-align:top">
	  <td class="fond_td">
	    <a href="agenda.php?debut=<?php echo substr($ligne['RbDate_Date'],0,10)?>&amp;fin=<?php echo substr($ligne['RbDate_Date'],0,10)?>&amp;envoyer_date=Chercher+par+jour&amp;critere_recherche=Nom&amp;nom=%&amp;type=%"><?php echo $date_result ?></a>
	  </td>
	  <td class="fond_td">
	    <?php echo $heure_result?>
	  </td>
	  <td class="fond_td">
	    <a href="frame_patient.php?GUID=<?php echo $ligne['FchGnrl_IDDos']?>"><?php echo stripslashes($ligne['FchGnrl_NomDos'])." ".stripslashes($ligne['FchGnrl_Prenom'])?></a>
	  </td>
	  <td class="fond_td">
	    <?php echo stripslashes($ligne['RbDate_NomDate']) ?>
	  </td>
<?php
    if ($afficher_tout) 
    {
      $tab=""; //on vide $tab à chaque tour
      $fiche_mieux=(preg_match($expr,$ligne['RbDate_DataRub'],$tab));
      $obs=preg_replace("`<body.*>`i", " ",$tab[2]);
      echo "
	  <td class=\"fond_td\">
	    ".$obs."
	  </td>";
    }
// Les recettes
    $connect=mysqli_select_db($db,$base_compta);
    $date_hono=substr($ligne["RbDate_Date"],0,10);
    $GUID_patient=$ligne["FchGnrl_IDDos"];
    $sql_chercher_hono="SELECT * FROM honoraires WHERE GUID='$GUID_patient' AND date='$date_hono'";
    $resultat_chercher_hono=mysqli_query($db,$sql_chercher_hono);
    echo "
	  <td class=\"fond_td\">";
    while ($ligne_chercher_hono=mysqli_fetch_array($resultat_chercher_hono))://possibilite de plusieurs recettes le mm jour pourla mm personne
      if ($ligne_chercher_hono['esp']) echo $ligne_chercher_hono['esp']." (esp)";
      if ($ligne_chercher_hono['chq']) echo $ligne_chercher_hono['chq']." (ch)";
      if ($ligne_chercher_hono['cb']) echo $ligne_chercher_hono['cb']." (cb)";
      if ($ligne_chercher_hono['daf']) echo $ligne_chercher_hono['daf']." (DAF)";
      if ($ligne_chercher_hono['autre']) echo $ligne_chercher_hono['autre']." (autre)";
      if ($ligne_chercher_hono['du']) echo $ligne_chercher_hono['du']." (dû)";
    endwhile;
//    $total_patient=$ligne_chercher_hono['esp']+$ligne_chercher_hono['chq']+$ligne_chercher_hono['cb']+$ligne_chercher_hono['daf']+$ligne_chercher_hono['autre']+$ligne_chercher_hono['du'];
//si pas d'honoraire pour ce patient ce jour, afficher un bouton Ajouter
    if (!mysqli_num_rows($resultat_chercher_hono))
    {
      $date_hono=str_replace("/","-",$date_result);
?>
	    <form action="compta/saisie_hono.php" method="get">
	      <div>
		<input name="date" type="hidden" value="<?php echo $date_hono?>" />
		<input name="guid_patient" type="hidden" value="<?php echo $GUID_patient?>" />
		<input name="bouton_ajouter_hono" type="submit" value="Ajouter" class='noPrint' />
	      </div>
	    </form>
<?php
    }
    echo "
	  </td>";
//saisie_hono.php?guid_patient=fe7bdd37-cfd9-4d67-8661-43161e7f41e5&bouton_ajouter_hono=Ajouter  date=2009-06-09
?>
	</tr>
<?php
  } //fin tableau des résultats des titres
?>
      </table>
<?php
} //fin du if count consultation
else
{
echo " : <b>Aucun</b>";
  if ($critere_recherche!="aucun" and $critere_recherche!="")
  {
    echo " pour la valeur <b>".$cle."</b> sur le filtre <b>".$critere_recherche."</b>.<br />";
  }

}
?>
    </div>
  </div>
  <div class="groupe">
<?php
// on ferme la base
mysqli_close($db);
include("inc/footer.php");
?>
