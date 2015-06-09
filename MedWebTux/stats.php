<?php
include("config.php");
//outil destine a faire des statistiques
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
if(!$db)
{
	print "Erreur de connexion au serveur<br />";
	exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
	print "Erreur ".mysqli_error($db)."<br />";
	mysqli_close($db);
	exit;
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

$date= local_to_iso(date('Y-m-d', date('U')),$date_format);
$jour_debut=$date;
$jour_fin=$date;
$anchor='stats';

$cle=array();

  include("inc/header.php");
?>

  <title>MedWebTux - Statistiques</title>
</head>
<body>
  <div class="conteneur">
<?php
  include("inc/menu-horiz.php");
if (isset ($_REQUEST['periode']))
{
  $jour_debut=$_REQUEST['jour_debut'];
  $jour_fin=$_REQUEST['jour_fin'];
}
?>
<div class="groupe">
    <h1>Période</h1>

    <form id="form_montrer" action="stats.php" method="get">
      <table border="1">
	<tr class="titre_tableau">
	  <th title="Format : aaaa-mm-jj" >D&eacute;but</th>
	  <th title="Format : aaaa-mm-jj" >Fin</th><th ></th>
	</tr>
	<tr>
	  <td><input type="text" name="jour_debut" id="datepicker" value="<?php echo $jour_debut ?>" maxlength="10" size="10" /></td>
	  <td><input type="text" name="jour_fin" id="datepickeur" value="<?php echo $jour_fin ?>" maxlength="10" size="10" /></td>
	  <td><input type="submit" name="periode" value="Montrer" /></td>
	</tr>
      </table>
    </form>
</div>
<?php
if (isset ($_REQUEST['periode']))
{
  $jour_debut=local_to_iso($_REQUEST['jour_debut'],$date_format)." 00:00:00";
  $jour_fin=local_to_iso($_REQUEST['jour_fin'],$date_format)." 23:59:59";
}
else
{
  $jour_debut=local_to_iso($jour_debut,$date_format)." 00:00:00";
  $jour_fin=local_to_iso($jour_fin,$date_format)." 23:59:59";
}
?>
<div class="groupe">
<h1>Résultats</h1>
<p>
<?php

  $sql_chercher_dates="SELECT * FROM RubriquesHead WHERE RbDate_Date >='$jour_debut' AND RbDate_Date <='$jour_fin' AND RbDate_TypeRub='20030000' ";

  $resultat_chercher_dates=mysqli_query($db,$sql_chercher_dates);
  echo "Nombre total : ".mysqli_num_rows($resultat_chercher_dates)."<br />";

  $compteur_dept=0;
  $compteur_ardt=0;
$compteur_16=0;
  $compteur_couronne=0;
  $compteur_ardt_vide=0;
  $compteur_age_moins_3=0;
  $compteur_age_entre_3_et_15=0;
  $compteur_pas_age=0;
  $compteur_envoyeur_regule=0;
  $compteur_envoyeur_police=0;
  $compteur_envoyeur_gmp=0;
  $compteur_envoyeur_pharmacie=0;
  $compteur_envoyeur_cabinet=0;
  $compteur_envoyeur_deja_venu=0;
  $compteur_envoyeur_voisinage=0;
$compteur_envoyeur_internet=0;
  $compteur_CMU=0;
  $compteur_ALD=0;
  $compteur_biologie=0;
  $compteur_radio=0;
  $compteur_orientation_domicile=0;
  $compteur_orientation_SMUR=0;
  $compteur_orientation_urgences=0;
  $compteur_orientation_hospitalisation=0;
  $compteur_orientation_DC=0;
  $compteur_ORL=0;
  $compteur_digestif=0;
  $compteur_Autres=0;
  $compteur_Psy=0;
  $compteur_Ped=0;
  $compteur_Dent=0;
  $compteur_Trauma=0;
  $compteur_Cardio=0;
$compteur_gyneco=0;
  $compteur_envoyeur_accueil=0;
  
  while ($ligne_chercher_dates=mysqli_fetch_array($resultat_chercher_dates))
  {
    //chercher dans les blobs
    $blob_primkey=$ligne_chercher_dates['RbDate_RefBlobs_PrimKey'];
    $sql_chercher_blobs="SELECT RbDate_DataRub FROM RubriquesBlobs WHERE RbDate_PrimKey='$blob_primkey'";
    $resultat_chercher_blobs=mysqli_query($db,$sql_chercher_blobs);
    $ligne_chercher_blobs=mysqli_fetch_array($resultat_chercher_blobs);
    $blob=$ligne_chercher_blobs['RbDate_DataRub'];
    //echo $blob;
    $ligne_blob=explode ("<br />",$blob);
    foreach ($ligne_blob AS $cette_ligne)
    {
	if (preg_match ('`Diagnostic`',$cette_ligne) OR preg_match ('`Spécialité`',$cette_ligne))
	{
	  if (preg_match ('`ORL`',$cette_ligne))
	    $compteur_ORL++;
	  elseif (preg_match ('`DIGESTIF`',$cette_ligne) OR preg_match('`Troubles digestifs`',$cette_ligne))
	    $compteur_digestif++;
	  elseif (preg_match ('`Autres`',$cette_ligne))
	    $compteur_Autres++;
	  elseif (preg_match ('`Troubles psychologiques`',$cette_ligne))
	    $compteur_Psy++;
	  elseif (preg_match ('`Pédiatrie`',$cette_ligne))
	    $compteur_Ped++;
	  elseif (preg_match ('`Dentaires`',$cette_ligne))
	    $compteur_Dent++;
	  elseif (preg_match ('`Traumatisme`',$cette_ligne))
	    $compteur_Trauma++;
	  if (preg_match ('`Cardio-vasculaire`',$cette_ligne))
	    $compteur_Cardio++;
	  if (preg_match ('`Génito-urinaires `',$cette_ligne))
		$compteur_gyneco++;
	}


      if (preg_match ('`Demande de radio`',$cette_ligne) OR preg_match ('`Radio`',$cette_ligne))
      { 
	if (preg_match ('`1`',$cette_ligne) OR preg_match ('`oui`',$cette_ligne))
	  $compteur_radio++;
      }

      if (preg_match ('`Demande de biologie`',$cette_ligne) OR preg_match ('`Labo`',$cette_ligne))
      { 
	if (preg_match ('`1`',$cette_ligne) OR preg_match ('`oui`',$cette_ligne))
	  $compteur_biologie++;
      }
      if (preg_match ('`CMU`',$cette_ligne))
      { 
	if (preg_match ('`1`',$cette_ligne) OR preg_match ('`oui`',$cette_ligne))
	  $compteur_CMU++;
      }
      if (preg_match ('`ALD`',$cette_ligne))
      { 
	if (preg_match ('`1`',$cette_ligne) OR preg_match ('`oui`',$cette_ligne))
	  $compteur_ALD++;
      }
//echo $cette_ligne.'<br />';
// if (preg_match('`Action`',$cette_ligne))
      if (preg_match('`Action`',$cette_ligne) OR preg_match('`Destination`',$cette_ligne))
      {
//echo 'azerty';
	if (preg_match('`domicile`',$cette_ligne))
	{
	  $compteur_orientation_domicile++;
//echo $compteur_orientation_domicile.'compteur';
	}
	elseif (preg_match('`SMUR`',$cette_ligne))
	{
	  $compteur_orientation_SMUR++;
	}
	elseif (preg_match('`Urgences hospitalières`',$cette_ligne) OR preg_match('`Urgences hôpital`',$cette_ligne))
	{
	  $compteur_orientation_urgences++;
	}
	elseif (preg_match('`Hospitalisation`',$cette_ligne))
	{
	  $compteur_orientation_hospitalisation++;
	}
	elseif (preg_match('`Décès`',$cette_ligne))
	{
	  $compteur_orientation_DC++;
	}
      }
      elseif (preg_match('`Envoyeur`',$cette_ligne) OR preg_match('`Adress`',$cette_ligne))
      {
//echo "envoyeur";
	if (preg_match('`Le 15`',$cette_ligne) OR preg_match('`centre 15`',$cette_ligne) )
	{
	  $compteur_envoyeur_regule++;
	}
	elseif (preg_match('`Police`',$cette_ligne) OR preg_match('`Pompiers`',$cette_ligne))
	{
	  $compteur_envoyeur_police++;
	}
	elseif (preg_match('`harmacie`',$cette_ligne))
	{
//echo "test".$compteur_envoyeur_pharmacie;
	  $compteur_envoyeur_pharmacie++;
	}
	elseif (preg_match('`Cabinet`',$cette_ligne) OR preg_match('`médecin traitant`',$cette_ligne) )
	{
	  $compteur_envoyeur_cabinet++;
	}
	elseif (preg_match('`accueil`',$cette_ligne))
	{
	  $compteur_envoyeur_accueil++;
	}
	elseif (preg_match('`GMP`',$cette_ligne) OR preg_match('`SOS`',$cette_ligne) OR preg_match('`UMP`',$cette_ligne))
	{
	  $compteur_envoyeur_gmp++;
	}
	elseif (preg_match('`internet`',$cette_ligne))
	{
	  $compteur_envoyeur_internet++;
	}
	elseif (preg_match('`accueil`',$cette_ligne))
	{
	  $compteur_envoyeur_accueil++;
	}
	elseif (preg_match('`déjà venu`',$cette_ligne))
	{
	  $compteur_envoyeur_deja_venu++;
	}
	elseif (preg_match('`voisinage`',$cette_ligne))
	{
	  $compteur_envoyeur_voisinage++;
	}

//	echo $cette_ligne;
      }
    }
//les codes postaux
    $codepostal="";
    $id_patient=$ligne_chercher_dates['RbDate_IDDos'];
    $sql_chercher_identites="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$id_patient'";
    $resultat_chercher_identites=mysqli_query($db,$sql_chercher_identites);
    $ligne_chercher_identites=mysqli_fetch_array($resultat_chercher_identites);
    //patient < 3 ans
    if ($ligne_chercher_identites["FchPat_Nee"]=="1970-01-01 00:00:00")
      $compteur_pas_age++;
    else
    {
      $day_age  = substr($ligne_chercher_dates['RbDate_Date'],8,2) - substr($ligne_chercher_identites["FchPat_Nee"],8,2);
      $mon_age = substr($ligne_chercher_dates['RbDate_Date'],5,2) - substr($ligne_chercher_identites["FchPat_Nee"],5,2);
      $year_age = substr($ligne_chercher_dates['RbDate_Date'],0,4) - substr($ligne_chercher_identites["FchPat_Nee"],0,4);
      if ($day_age < 0)
      {
	$mon_age=$mon_age - 1;
      }
      if ($mon_age < 0)
      {
	$year_age = $year_age - 1;
	$mon_age = 12 + $mon_age;
      }
      if ($year_age<3)
	$compteur_age_moins_3++;
      elseif ($year_age>=3 AND $year_age<15)
	$compteur_age_entre_3_et_15++;
    }    //patient parisien
    $codepostal=$ligne_chercher_identites['FchPat_CP'];
    if ($codepostal)
    {
      if (preg_match('`^75`',$codepostal))
      {
	$compteur_dept++;
	if (preg_match('`14$`',$codepostal) OR preg_match('`15$`',$codepostal))
	{
	  $compteur_ardt++;
	}
elseif (preg_match('`16$`',$codepostal))
	$compteur_16++;
      }
      elseif (preg_match('`^94`',$codepostal) OR preg_match('`^92`',$codepostal) OR preg_match('`^93`',$codepostal)) 
      {
	$compteur_couronne++;
      }
      //echo $codepostal."<br />";
    }
    else
      $compteur_ardt_vide++;
  }

?>
</p>
<h2>Envoyé par</h2>
<ul>
<li>Accès régulé : <?php echo $compteur_envoyeur_regule ?></li>
<li>Envoyeur Police/pompiers : <?php echo $compteur_envoyeur_police ?></li>
<li>Envoyeur GMP/SOS/UMP : <?php echo $compteur_envoyeur_gmp ?></li>
<li>Envoyeur pharmacie : <?php echo $compteur_envoyeur_pharmacie ?></li>
<li>Envoyeur cabinet : <?php echo $compteur_envoyeur_cabinet ?></li>
<li>Envoyeur internet : <?php echo $compteur_envoyeur_internet ?></li>
<li>Déjà venu :  <?php echo $compteur_envoyeur_deja_venu ?></li>
<li>Envoyeur voisinage :  <?php echo $compteur_envoyeur_voisinage ?></li>
<li>Accueil : <?php echo $compteur_envoyeur_accueil ?></li>
</ul>

<h2>Classe d'âge</h2>

<ul>
<li>Moins de 3 ans : <?php echo $compteur_age_moins_3 ?></li>
<li>Entre 3 et 15 ans : <?php echo $compteur_age_entre_3_et_15 ; 

  if ($compteur_pas_age)
{
?>
    <li>Âge inconnu : <?php echo $compteur_pas_age ; 
}
?></li>
</ul>
<h2>Pathologie</h2>
<ul>
  <li>ORL : <?php echo $compteur_ORL ?></li>
  <li>Digestif : <?php echo $compteur_digestif ?></li>
  <li>Cardio vasculaire : <?php echo $compteur_Cardio ?></li>
  <li>Pédiatrie : <?php echo $compteur_Ped ?></li>
  <li>Dentaire : <?php echo $compteur_Dent ?></li>
  <li>Psy : <?php echo $compteur_Psy ?></li>
<li>Gyneco-uro : <?php echo $compteur_gyneco ?></li>
  <li>Autres : <?php echo $compteur_Autres ?></li>
</ul>
<h2>Ville</h2>
<ul>
 <li>  Paris : <?php echo $compteur_dept ?></li>
 <li>
    <ul>
      <li>  dont 14e et 15e : <?php echo $compteur_ardt ?></li>
      <li>dont 16e : <?php echo $compteur_16 ?></li>
    </ul>
  </li>
  <li>  Banlieue :  </li>
  <li>
    <ul><?php echo "<li>$compteur_couronne</li>" ;
  if ($compteur_ardt_vide)
{
?>
      <li> Arrondissement non renseigné :  <?php echo $compteur_ardt_vide ;
}
?>
    </ul>
  </li>
</ul>
<h2>Action</h2>
<ul>
  <li>Retour à domicile : <?php echo $compteur_orientation_domicile ?></li>
  <li>SMUR : <?php echo $compteur_orientation_SMUR ?></li>
  <li>Urgences : <?php echo $compteur_orientation_urgences ?></li>
  <li>Hospitalisation : <?php echo $compteur_orientation_hospitalisation ?></li>
  <li>Décès : <?php echo  $compteur_orientation_DC ?></li>
</ul>
<h2>Examens</h2>
<ul>
  <li>Radio : <?php echo  $compteur_radio ?></li>
  <li>Biologie : <?php echo  $compteur_biologie ?></li>
</ul>
<h2>Terrain</h2>
<ul>
  <li>ALD : <?php echo  $compteur_ALD ?></li>
  <li>CMU : <?php echo  $compteur_CMU ?></li>
</ul>
</div>
<?php
include("inc/footer.php");
?>
