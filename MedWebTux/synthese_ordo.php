<?php
session_start() ;
//Page qui sert a afficher une ligne pour chaque medicament different prescrit dans le cadre d'ordonnances structurees, puis de creer le cas echeant, une nouvelle ordonnance contenant tous les medicaments selectionnes.

include("config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
$basemed="DatasempTest";

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

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
}

if (isset($_GET['numeroID']))
{
  $patient=$_GET['numeroID'];
  //On cherche dans la table IndexNomPrenom la fiche dont l'ID a ete demande
  $sql_chercher_nom="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
  $resultat_chercher_nom=mysqli_query($db,$sql_chercher_nom);
  $ligne_chercher_nom=mysqli_fetch_array($resultat_chercher_nom);
  $nom=$ligne_chercher_nom["FchGnrl_NomDos"];
  $prenom=$ligne_chercher_nom["FchGnrl_Prenom"];
}

//chercher les droits d'utilisateur
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];

$sql_chercher_droits="SELECT * FROM Personnes WHERE Login='$user'";
$resultat_chercher_droits=mysqli_query($db,$sql_chercher_droits);
$ligne_chercher_droits=mysqli_fetch_array($resultat_chercher_droits);
$GUID_user=$ligne_chercher_droits['GUID'];
$droits_user=$ligne_chercher_droits['Droits'];
$qualite_user=$ligne_chercher_droits['Qualite'];

//on cherche les medicaments
$sql_chercher_ordo="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesHead.RbDate_IDDOs='$patient' AND (RubriquesHead.RbDate_TypeRub='20020100')"; //on cherche les ordonnances structurees de patient
$resultat_chercher_ordo=mysqli_query($db,$sql_chercher_ordo);
//si aucun medicament, on va directement en page interactions
if (!$count_chercher_ordo=mysqli_num_rows($resultat_chercher_ordo))
{
  header ('location: interactions.php?numeroID='.$patient);
  exit;
}
include("inc/header.php");
?>
    <title>
      Ordonnances de <?php echo $nom.' '.$prenom ?> - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
    <link rel="stylesheet" href="css/print.css" type="text/css" media="print" />
    <style type="text/css"  media="print">
	td:nth-child(1) { display: none; }  
	th:nth-child(1) { display: none; }  
    </style>

    <script type="text/javascript">
<!--
//pour inverser les coches 
function Invers()
{
  temp = document.getElementById('form1').elements.length-3 ;
  for (i=0; i < temp; i++)
  {
    if (document.getElementById('form1').elements[i].checked == 1)
    {
      document.getElementById('form1').elements[i].checked = 0;
    }
    else 
    {
      document.getElementById('form1').elements[i].checked = 1;
    }
  }
}
-->
</script>

  </head>

  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Interactions';
//include("inc/menu-horiz.php");		
?>
      <div class="groupe">
	<h1>
	  Historique des médicaments de <?php echo $nom." ".$prenom ?>
	</h1>
	<div class='noPrint'>
	  <div class="notice">
	    Cette page permet d'afficher l'historique de tous les médicaments prescrits par le praticien ou ceux dont il est utilisateur délégué au moyen de MedinTux.<br />
	    Elle affiche une seule occurrence de chaque médicament, avec le nombre d'occurrences de prescription pour chacun d'entre eux.<br />
	    En sélectionnant les médicaments désirés au moyen des cases à cocher, puis en validant le bouton <i>Nouvelle ordonnance</i>, on crée une ordonnance structurée de MedinTux contenant une occurrence de chaque médicament sélectionné en tenant compte des interactions médicamenteuses, du traitement de fond et des caractéristiques du patient.
	  </div>
	</div>
<?php
$toutes_cip= array() ;
while ($ligne_chercher_ordo=mysqli_fetch_array($resultat_chercher_ordo))
{
  $cette_ordo=$ligne_chercher_ordo['RbDate_DataRub'];
  $login_possesseur_ordonnance=$ligne_chercher_ordo['RbDate_CreateUser'];
  if ($login_possesseur_ordonnance==$user) //On verifie si l'ordonnance appartient a l'utilisateur
  {
    if (preg_match("`obv`",$droits_user))//si oui, on verifie qu'il a les droits de lecture d'ordonnance
    {
      $lecture_ordoOK=1;
    }
    else
      $lecture_ordoOK="";
    }
  else //sinon on verifie qu'il est utilisateur autorise
  {
    $sql_GUID_possesseur_ordonnance="SELECT GUID FROM Personnes WHERE Login='$login_possesseur_ordonnance'";
    $resultat_GUID_possesseur_ordonnance=mysqli_query($db,$sql_GUID_possesseur_ordonnance);
    $ligne_GUID_possesseur_ordonnance=mysqli_fetch_array($resultat_GUID_possesseur_ordonnance);
    $GUID_possesseur_ordonnance=$ligne_GUID_possesseur_ordonnance['GUID'];

    $sql_utilisateur_autorise_lire_ordonnances="SELECT * FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_possesseur_ordonnance'";
    $resultat_utilisateur_autorise_lire_ordonnances=mysqli_query($db,$sql_utilisateur_autorise_lire_ordonnances);
    $count_utilisateur_autorise_lire_ordonnances=mysqli_num_rows($resultat_utilisateur_autorise_lire_ordonnances);
    if ($count_utilisateur_autorise_lire_ordonnances)//S'il existe un utilisateur autorise
    {
      $liste_utilisateur_autorise_lire_ordonnances=mysqli_fetch_array($resultat_utilisateur_autorise_lire_ordonnances);
      if (preg_match("`orv`",$liste_utilisateur_autorise_lire_ordonnances['FriendUserDroits']))//On verifie qu'il a les droits
	$lecture_ordoOK=1;
      else
	$lecture_ordoOK="";
    }
    else
      $lecture_ordoOK="";
  }
  if ($lecture_ordoOK)
  {
    $expr4='`<cip>(.*)<\/cip>`sU';

    $les_cip_de_cette_ordo=preg_match_all($expr4,$cette_ordo,$poso); //on cherche le CIP de cette poso

    foreach ($poso[1] as $cette_poso) //on analyse les posos de cette ordo (code CIP)
    {
      if (isset($compteur[$cette_poso]))
	$compteur[$cette_poso]=$compteur[$cette_poso]+1; //pour le nombre d'occurrences
      else
	$compteur[$cette_poso]=1;
      $vrai=1; //flag pour dire si on a trouve une correspondance entre les medics deja prescrits et les nouveaux
      foreach ($toutes_cip as $cette_cip) //on analyse le tableau des cip deja trouvees pour comparaison
      {
	if ($cette_poso==$cette_cip)
	{
	  $vrai=0; //medoc de l'ordo deja trouve = on n'ajoute pas
	}
      }
      if ($vrai==1) //si aucun medoc n'a ete trouve dans la liste
      {
	$toutes_cip[]=$cette_poso; //on ajoute cette nouvelle poso au tableau
      }
    }
  }
}
?>
	<div class="tableau">
	  <form action="interactions.php" method="get" id="form1">
	    <table style="width:auto;margin-left:auto;margin-right:auto;">
	      <col></col>
	      <tr>
		<th class="fond_th">
		</th>
		<th class="fond_th">
		  Libellé
		</th>
		<th class="fond_th">
		  CIP
		</th>
		<th class="fond_th">
		  Nombre d'occurrences
		</th>
	      </tr>
<?php
if (!mysqli_select_db($db,$basemed))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($dbmed);
  exit;
}
$j=0;
if ($toutes_cip)
{
  foreach ($toutes_cip as $cette_cip) //on analyse le tableau des cip deja trouvees
  {
    $j++;
    $sql_chercher_medoc="SELECT f1,f3 FROM t00 WHERE f2='$cette_cip'";
    $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
    $ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc);

    $this_drug=$ligne_chercher_medoc['f3'];
?>
	      <tr>
		<td class="fond_td">
		  <input name="cip_medoc[]" id="coche<?php echo $j ?>" type="checkbox" value="<?php echo $cette_cip ?>" />
		</td>
		<td class="fond_td">
		  <label for="coche<?php echo $j ?>">
		    <a href="medocs.php?cuv_medoc=<?php echo $ligne_chercher_medoc['f1'] ?>&amp;check_dispo=check_dispo"><?php echo $this_drug ?></a>
		  </label>
		</td>
		<td class="fond_td">
		  <?php echo $cette_cip ?>
		</td>
		<td  class="fond_td" style="text-align:center">
		  <?php echo $compteur[$cette_cip]?>
		</td>
	      </tr>
<?php
  }//fin foreach
}//fin if
?>
	      <tr class='noPrint'>
		<td>
		  <input name="button_invert_selection" onclick="Invers()" type="button" value=" Inverser la sélection " />
		</td>
		<td>
		  <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		  <input name="button_validate_form" type="submit" value="Nouvelle ordonnance" />
		</td>
		<td>
		</td>
		<td>
		</td>
	      </tr>
	    </table>
	  </form>
	</div>
      </div>
<?php
include("inc/footer.php");
?>