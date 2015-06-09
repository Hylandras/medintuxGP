<?php
session_start() ;
include("config.php");
$basemed="DatasempTest";

if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=medocs' );
  exit;
}
include("inc/header.php");
?>
    <title>
      MedWebTux - M&eacute;dicaments - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <script type="text/javascript">
//<![CDATA[
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
//]]>
    </script>
    <style type="text/css" media="all">

    #fixe-haut
    {
        background      : white ;
        height          : 30px;
        position        : fixed;
        top             : 0;
        width           : 100%;
        left            : 0;
    }
        #conteneur
        {
/*                 padding                 : 30px 0 42px 0; */
                margin                  : auto;
        }
    </style>
  </head>
  <body style="font-size:<?php echo $fontsize ?>pt" onload="donner_focus('nom_medoc')" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Mode_Vidal';
include("inc/menu-horiz.php");

if (!$db=mysqli_connect($host, $loginbase, $pwd))
{
  print "Erreur de connexion &agrave; ".$host."<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$basemed))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}

function detectUTF8($string) //pour les lignes dont l'encodage est toujours imprévisible
{
  return preg_match('%(?:
  [\\xC2-\\xDF][\\x80-\\xBF] # non-overlong 2-byte
  |\\xE0[\\xA0-\\xBF][\\x80-\\xBF] # excluding overlongs
  |[\\xE1-\\xEC\\xEE\\xEF][\\x80-\\xBF]{2} # straight 3-byte
  |\\xED[\\x80-\\x9F][\\x80-\\xBF] # excluding surrogates
  |\\xF0[\\x90-\\xBF][\\x80-\\xBF]{2} # planes 1-3
  |[\\xF1-\\xF3][\\x80-\\xBF]{3} # planes 4-15
  |\\xF4[\\x80-\\x8F][\\x80-\\xBF]{2} # plane 16
  )+%xs', $string);
}

function fix_utf8($string)
{
  if (detectUTF8($string))
  {
    return utf8_decode($string);
//    return ($string);
  }
  else
  {
    return $string;
  }
}
function stripAccents($string)
{
  return strtr($string,'àáâãäçèéêëìíîïñòóôõöùúûüýÿÀÁÂÃÄÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝ',
'aaaaaceeeeiiiinooooouuuuyyAAAAACEEEEIIIINOOOOOUUUUY');
}

$nom_medoc="";
$radio_classe='1'; //on selectionne par defaut le bouton qui choisit les medicaments
$radio_distri='4'; //medicaments de ville par defaut
$dispo='';
if (isset ($_GET['radio_classe']))
{
  $radio_classe=$_GET['radio_classe'];
}

if (isset ($_GET['radio_distri']))
{
  $radio_distri=$_GET['radio_distri'];
}


if (isset($_GET['id_medoc'])) //si on a l'ID du produit
{
  $id_medoc=$_GET['id_medoc'];
  $sql_chercher_medoc="SELECT t4B.f0 AS cip,t4B.f2 AS nom,f5, t42.f2 AS classe_nom FROM t4B INNER JOIN t42 ON t4B.fA=t42.f0 WHERE t4B.f0 = '$id_medoc'";
  $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
}

if (isset($_GET['cuv_medoc'])) //code d'unite de vente (on a clique sur un medoc selectionne, par ex. ou on vient des interactions) Entraine l'affichage de la fiche.
{
?>
    <p id="fixe-haut">
      <a href="#composants">Composants</a>  <a href="#Propriétés_thérapeutiques">Propriétés thérapeutiques</a>  <a href="#Précautions_d_emploi">Précautions d'emploi</a>  <a href="#Indications">Indications</a>  <a href="#Contre-indications">Contre-indications</a>  <a href="#Effets_indésirables">Effets indésirables</a>  <a href="#Surdosage">Surdosage</a>  <a href="#Posologie">Posologie</a>  <a href="#Perturbation_des_examens_cliniques">Perturbation des examens cliniques</a>  <a href="#Composition_texte_structuré">Composition</a>  <a href="#Precautions_delivrance">Précautions de délivrance</a>  <a href="#Prix">Prix</a>  <a href="#duree">Durée maxi de prescription</a> <a href="#event">Évènements</a><a href="#tfr">TFR</a>  <a href="#labo">Laboratoire</a>
    </p>
<p>
<i>Page en travaux destinée à explorer la structure du Vidal Data Semp. Pour les Références du Produit (RCP), cliquez sur le lien "Fiche Vidal"</i>
</p>
<?php
  $cuv_medoc=$_GET['cuv_medoc'];
  $sql_chercher_medoc="SELECT  f0 AS cpg, f1 AS cuv, f2 AS cip, f3 AS nom,fJ FROM t00 WHERE f1 = '$cuv_medoc'";
  $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
}
elseif (isset($_GET['nom_medoc'])) //si on fait la recherche par nom ou DCI ou indication
{
  if ($_GET['nom_medoc'] !="") //pas de joker si nom vide
  {
    $nom_medoc="%".stripAccents($_GET['nom_medoc'])."%";
  }
//URL en mode DCI
//MedWebTux/medocs.php?radio_dispo=yes&radio_distri=4&radio_classe=1&nom_medoc=allopurinol&envoyer_composition_medoc=DCI

//URL en mode classe
//MedWebTux/medocs.php?classe=M&classe1=M04&classe2=M04A&classe3=M04AA&groupe=M04AA01&button_classification=Chercher

/*t1A = modes de commercialisation
    f0 = code Lie t02.f2 et t63
    f1 = libelle. 
      2 hopitaux
      3 Sommeil
      4 Officines
      5 Toxico
      6 Dialyse
      8 Ville hopital
      9 SMUR
*/

  $fK=$radio_classe."%";
  $sql_chercher_medoc="SELECT t00.f0 AS cpg, t00.f1 AS cuv, t00.fG AS nom, t00.fJ FROM t00 INNER JOIN t02 ON t00.f1=t02.f0  WHERE CAST(t00.f3 AS CHAR) LIKE '$nom_medoc' AND t00.fK LIKE '$fK' AND t02.f2 = '$radio_distri' ORDER BY t00.f3";
  $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
//recherche par DCI
  $sql_chercher_composition="SELECT * FROM t54 WHERE CAST(f2 AS CHAR) LIKE '$nom_medoc' GROUP BY f0 ORDER BY f2";
  $resultat_chercher_composition=mysqli_query($db,$sql_chercher_composition);
  $nom_medoc=$_GET['nom_medoc']; // on reintialise le nom sans joker ni bidouille sur les accents pour retourner en zone de saisie
}

$sql_chercher_version="SELECT * FROM tFB";
$resultat_chercher_version=mysqli_query($db,$sql_chercher_version);
while ($ligne_chercher_version=mysqli_fetch_array($resultat_chercher_version))
{
  if ($ligne_chercher_version['f0']=='EDITION NUMERO')
  {
    $num_version=$ligne_chercher_version['f1'];
  }
  elseif ($ligne_chercher_version['f0']=='ARRETE PACK IR')
  {
    $dateIR=$ligne_chercher_version['f1'];
  }
}

$checked_yes='checked="checked"';
$checked_no='';
$radio_dispo='';

if (isset($_GET['radio_dispo']))
{
  if ($_GET['radio_dispo']=="no")
  {
    $checked_yes='' ;
    $checked_no='checked="checked"' ;
  }
  $radio_dispo=$_GET['radio_dispo'];
}

?>
   <div class="groupe">
      <h1>
	Vidal Data Semp version <?php echo $num_version ?> du <?php echo $dateIR ?>
      </h1>

<!-- <form action="medocs.php" method="get" title="Tout ou partie du nom" onsubmit="return verif_champ(this.composition_medoc.value);"> -->
      <form action="medocs.php" method="get"  id="form_general">
	<fieldset>
	  <legend>
	    Options
	  </legend>
	  <label for="radio_dispo_yes">
	    Seulement les commercialis&eacute;s
	  </label>
	  <input type="radio" value="yes" name="radio_dispo" id="radio_dispo_yes" <?php echo $checked_yes ?>/>
          <label for="radio_dispo_no">
            Tous
          </label>
          <input type="radio" value="no" name="radio_dispo" id="radio_dispo_no" <?php echo $checked_no ?> />
          <br />
          <label for="radio_distri_2">
            Hôpitaux
          </label>
          <input type="radio" value="2" name="radio_distri" id="radio_distri_2" <?php if ($radio_distri=="2") echo 'checked="checked"' ?> />
<!-- Seuls les types 2 et 4 existent en vrai -->
<!--           <label for="radio_distri_3"> -->
<!--             Sommeil -->
<!--           </label> -->
<!--           <input type="radio" value="3" name="radio_distri" id="radio_distri_3" <?php if ($radio_distri=="3") echo 'checked="checked"' ?> /> -->
          <label for="radio_distri_4">
            Officines
          </label>
          <input type="radio" value="4" name="radio_distri" id="radio_distri_4" <?php if ($radio_distri=="4") echo 'checked="checked"' ?>/>
<!--           <label for="radio_distri_5"> -->
<!--             Centres de toxicomanie -->
<!--           </label> -->
<!--           <input type="radio" value="5" name="radio_distri" id="radio_distri_5" <?php if ($radio_distri=="5") echo 'checked="checked"' ?>/> -->
<!--           <label for="radio_distri_6"> -->
<!--             Centres de dialyse -->
<!--           </label> -->
<!--           <input type="radio" value="6" name="radio_distri" id="radio_distri_6" <?php if ($radio_distri=="6") echo 'checked="checked"' ?>/> -->
<!--           <label for="radio_distri_8"> -->
<!--             Ville hôpital -->
<!--           </label> -->
<!--           <input type="radio" value="8" name="radio_distri" id="radio_distri_8" <?php if ($radio_distri=="8") echo 'checked="checked"' ?>/> -->
<!--           <label for="radio_distri_9"> -->
<!--             SMUR -->
<!--           </label> -->
<!--           <input type="radio" value="9" name="radio_distri" id="radio_distri_9" <?php if ($radio_distri=="9") echo 'checked="checked"' ?> /> -->
          <br />
          <label for="radio_classe_1">
            Spécialité
          </label>
          <input type="radio" value="1" name="radio_classe" id="radio_classe_1" <?php if ($radio_classe=="1") echo 'checked="checked"' ?> />
          <label for="radio_classe_2">
            Diététique
          </label>
          <input type="radio" value="2" name="radio_classe" id="radio_classe_2" <?php if ($radio_classe=="2") echo 'checked="checked"' ?> />
          <label for="radio_classe_3">
            Vétérinaire
          </label>
          <input type="radio" value="3" name="radio_classe" id="radio_classe_3" <?php if ($radio_classe=="3") echo 'checked="checked"' ?> />
          <label for="radio_classe_4">
            Parapharmacie
          </label>
          <input type="radio" value="4" name="radio_classe" id="radio_classe_4" <?php if ($radio_classe=="4") echo 'checked="checked"' ?> />
          <label for="radio_classe_5">
            Accessoires
          </label>
          <input type="radio" value="5" name="radio_classe" id="radio_classe_5" <?php if ($radio_classe=="5") echo 'checked="checked"' ?> />
          <label for="radio_classe_6">
            Divers
          </label>
          <input type="radio" value="6" name="radio_classe" id="radio_classe_6" <?php if ($radio_classe=="6") echo 'checked="checked"' ?> />
<!--           <label for="radio_classe_7"> 
            Homéopathie
          </label>
          <input type="radio" value="7" name="radio_classe" id="radio_classe_7" <?php if ($radio_classe=="7") echo 'checked="checked"' ?> />-->
          <label for="radio_classe_8">
            Thermalisme
          </label>
          <input type="radio" value="8" name="radio_classe" id="radio_classe_8" <?php if ($radio_classe=="8") echo 'checked="checked"' ?> />

	</fieldset>
	<fieldset title="Tout ou partie du nom">
	  <legend>
	    Recherche par nom du m&eacute;dicament, composition ou indication :
	  </legend> 
	  <input name="nom_medoc" id="nom_medoc" type="text" value="<?php echo str_replace ("%","",$nom_medoc)?>" title="Nom commercial ou scientifique ou indication" />
	  <input name="envoyer_nom_medoc" type="submit" value="Nom commercial" />
	  <input name="envoyer_composition_medoc" type="submit" value="DCI"/>
	  <input name="envoyer_indication_medoc" type="submit" value="Indication" />
	</fieldset>
<?php
if (isset($_GET['num_substance'])) //on vient du tableau de resultat des DCI
{
  echo "<div class=\"information\">";
/*t07.
f2 = num substance 
f0=code produit cip */
  $num_substance=$_GET['num_substance'];
//  $sql_recherche_par_substance="SELECT t54.f0 AS num_subst,t00.f3 AS nom_medoc,t00.f0 AS num_medoc ,t00.f1 AS cuv_medoc FROM t54 INNER JOIN t07 ON t54.f0=t07.f2 INNER JOIN t00 ON t07.f0=t00.f0 WHERE t54.f0='$num_substance'";
  $sql_recherche_par_substance="SELECT t27.f0 AS num_subst,t00.f3 AS nom_medoc,t00.f0 AS num_medoc ,t00.f1 AS cuv_medoc FROM t27 INNER JOIN t07 ON t27.f0=t07.f2 INNER JOIN t00 ON t07.f0=t00.f0 WHERE t27.f0='$num_substance'";
  $resultat_recherche_par_substance=mysqli_query($db,$sql_recherche_par_substance);
  while ($ligne_recherche_par_substance=mysqli_fetch_array($resultat_recherche_par_substance))
  {
    if ($radio_dispo=='yes')
    {
  //on cherche si le medoc est commercialise
      $cuv=$ligne_recherche_par_substance['cuv_medoc'];//code unite de vente
  //On cherche le code de dispo selon le CUV
      $sql_dispo="SELECT * FROM t02 WHERE f0='$cuv'";
      $resultat_dispo=mysqli_query($db,$sql_dispo);
      $ligne_dispo=mysqli_fetch_array($resultat_dispo);
      if ($ligne_dispo['f3']==0) // medicament commercialise seulement
      {
	$display_list=1;
      }
      else
      {
	$display_list=0;
      }
    }
    else
      $display_list=1;

    if ($display_list)
    {
//      if (detectUTF8($ligne_chercher_composition['f2']))
	echo "
	  &nbsp;&nbsp;<a href=\"medocs.php?cuv_medoc=".$ligne_recherche_par_substance['cuv_medoc']."&amp;id_medoc=".$ligne_recherche_par_substance['num_medoc']."&amp;nom_medoc=".urlencode($ligne_recherche_par_substance['nom_medoc']).$dispo."\">".utf8_encode($ligne_recherche_par_substance['nom_medoc'])."</a><br />";
 /*     else 
	echo "
	  &nbsp;&nbsp;<a href=\"medocs.php?cuv_medoc=".$ligne_recherche_par_substance['cuv_medoc']."&amp;id_medoc=".$ligne_recherche_par_substance['num_medoc']."&amp;nom_medoc=".urlencode(utf8_encode($ligne_recherche_par_substance['nom_medoc'])).$dispo."\">".utf8_encode($ligne_recherche_par_substance['nom_medoc'])."</a><br />";*/
    }
  }
echo "</div>";
}
/*
Recherche d'un medicament par indication
t1C.f0=2994 pour f1=indication
t52.f2=t1C.f0
t52.f1 = CIP ->t00.f0
t52.f0=t50.f0 
t50.f1= texte de l'indication en clair (en binaire, donc sensible à la casse et aux accents)
*/
 if (isset($_GET['envoyer_indication_medoc'])) //recherche par indication
{
  if ($_GET['nom_medoc']!='')
  {
    $text_indication='%'.addslashes(($_GET['nom_medoc'])).'%';
  }
  $sql_cip_indication="SELECT t52.f1 AS cip, t00.f3 AS libelle, t00.f1 AS cuv FROM t52 , t50 , t00  WHERE t52.f2='2994' AND Lower(t50.f1) LIKE Lower('$text_indication') AND t52.f1=t00.f0 AND t52.f0=t50.f0";
  $resultat_cip_indication=mysqli_query($db,$sql_cip_indication);
//  $text_indication=$_GET['text_indication'];
  while ($ligne_cip_indication=mysqli_fetch_array($resultat_cip_indication))
  {
?>
<a href="medocs.php?cuv_medoc=<?php echo $ligne_cip_indication['cuv'] ?>&amp;radio_dispo=yes&amp;radio_distri=4&amp;radio_classe=1">
  <?php if (detectUTF8($ligne_cip_indication['libelle'])) echo $ligne_cip_indication['libelle']; else echo utf8_encode($ligne_cip_indication['libelle']); ?>
<br /></a>
<?php
  }
}

if ((isset($_GET['nom_medoc']) AND isset($_GET['envoyer_nom_medoc'])) OR isset($_GET['cuv_medoc']) OR isset($_GET['id_medoc']))
{ 
  while ($ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc))
  {
//on verifie si coche dispo
    if (isset($_GET['radio_dispo']))
    {
      if ($radio_dispo=='yes')
      {
    //on cherche si le medoc est commercialise
	$cuv=$ligne_chercher_medoc['cuv'];//code unite de vente
    //On cherche le code de dispo selon le CUV
	$sql_dispo="SELECT * FROM t02 WHERE f0='$cuv'";
	$resultat_dispo=mysqli_query($db,$sql_dispo);
	$ligne_dispo=mysqli_fetch_array($resultat_dispo);
	if ($ligne_dispo['f3']==0) // medicament commercialise seulement
	{
	  $display_list=1;
	}
	else
	{
	  $display_list=0;
	}
      }
      else
	$display_list=1;
    }
    else
      $display_list=1;

    if ($display_list)
    {
      if (detectUTF8($ligne_chercher_medoc['nom']))
	echo "
	<p>
	  <a href=\"medocs.php?cuv_medoc=".$ligne_chercher_medoc['cuv']."&amp;radio_dispo=".$radio_dispo."&amp;radio_distri=".$radio_distri."&amp;radio_classe=".$radio_classe."\">".$ligne_chercher_medoc['nom']."</a> 	<a href=\"fiche_vidal.php?cuv_medoc=".$ligne_chercher_medoc['cuv']."\" >Fiche Vidal</a>
	</p>";
      else
	echo "
	<p>
	  <a href=\"medocs.php?cuv_medoc=".$ligne_chercher_medoc['cuv']."&amp;radio_dispo=".$radio_dispo."&amp;radio_distri=".$radio_distri."&amp;radio_classe=".$radio_classe."\">".utf8_encode($ligne_chercher_medoc['nom'])."</a> 	<a href=\"fiche_vidal.php?cuv_medoc=".$ligne_chercher_medoc['cuv']."\" >Fiche Vidal</a>
	</p>";
    }

    if (isset($_GET['cuv_medoc']))
    {
      if ($_GET['cuv_medoc']==$ligne_chercher_medoc['cuv'])
      {
?>
        <p>
          <a href="interactions.php?select_medoc=<?php echo $ligne_chercher_medoc['cuv'] ?>&amp;nom_medoc=&amp;button_select_medoc=Ajouter+à+la+liste">Chercher les interactions</a>
	</p>
<?php
      }
    }
  }
}
?>

<?php
if (isset($_GET['nom_medoc']) AND isset($_GET['envoyer_composition_medoc']))
{
?>
	<table>
	  <tr>
	    <th>
	      Num&eacute;ro
	    </th>
	    <th>
	      Nom
	    </th>
	    <th>
	      DCI
	    </th>
	  </tr>
<?php
  while ($ligne_chercher_composition=mysqli_fetch_array($resultat_chercher_composition))
  {
?>
	  <tr>
	    <td valign="top">
	      <?php echo $ligne_chercher_composition['f0'] ?>
	    </td>
	    <td>
	      <p>
		<a href="medocs.php?num_substance=<?php echo $ligne_chercher_composition['f0'] ?>&amp;envoyer_composition_medoc=envoyer&amp;nom_medoc=<?php echo urlencode($_GET['nom_medoc']);
    if ($radio_dispo=="yes")
      echo '&amp;radio_dispo=yes';
    else
      echo '&amp;radio_dispo=no';
    ?>"><?php if (detectUTF8($ligne_chercher_composition['f2'])) echo $ligne_chercher_composition['f2']; else echo utf8_encode($ligne_chercher_composition['f2']).'</a>';
    if (isset($_GET['num_substance']))
    {
      echo '
	      <br />';
      if ($ligne_chercher_composition['f0']==$_GET['num_substance'])
      {
	if ($_GET['radio_dispo'])
	{
	  $dispo='&amp;radio_dispo='.$radio_dispo;
	}
	while ($ligne_recherche_par_substance=mysqli_fetch_array($resultat_recherche_par_substance))
	{
	  if ($radio_dispo=='yes')
	  {
	//on cherche si le medoc est commercialise
	    $cuv=$ligne_recherche_par_substance['cuv_medoc'];//code unite de vente
	//On cherche le code de dispo selon le CUV
	    $sql_dispo="SELECT f3 FROM t02 WHERE f0='$cuv'";
	    $resultat_dispo=mysqli_query($db,$sql_dispo);
	    $ligne_dispo=mysqli_fetch_array($resultat_dispo);
	    if ($ligne_dispo['f3']==0) // medicament commercialise seulement
	    {
	      $display_list=1;
	    }
	    else
	    {
	      $display_list=0;
	    }
	  }
	  else
	    $display_list=1;

	  if ($display_list)
	  {
	    if (detectUTF8($ligne_recherche_par_substance['nom_medoc']))
	      echo "
		&nbsp;&nbsp;<a href=\"medocs.php?cuv_medoc=".$ligne_recherche_par_substance['cuv_medoc']."&amp;id_medoc=".$ligne_recherche_par_substance['num_medoc']."&amp;nom_medoc=".urlencode($ligne_recherche_par_substance['nom_medoc']).$dispo."\">".$ligne_recherche_par_substance['nom_medoc']."</a><br />";
	    else
	      echo "
		&nbsp;&nbsp;<a href=\"medocs.php?cuv_medoc=".$ligne_recherche_par_substance['cuv_medoc']."&amp;id_medoc=".$ligne_recherche_par_substance['num_medoc']."&amp;nom_medoc=".urlencode(utf8_encode($ligne_recherche_par_substance['nom_medoc'])).$dispo."\">".utf8_encode($ligne_recherche_par_substance['nom_medoc'])."</a><br />";
	  }
	}
      }
    }
?>
	      </p>
	    </td>
	    <td valign="top">
	      <?php   if (detectUTF8($ligne_chercher_composition['f2'])) echo $ligne_chercher_composition['f2']; else echo utf8_encode($ligne_chercher_composition['f2']); ?>
	    </td>
	  </tr>
<?php
  } //fin while ligne composition
?>
	</table>
<?php
} //fin if composition medoc
?>
	<fieldset>
	  <legend>
	    Classification des m&eacute;dicaments
	  </legend>
	  <input type="submit" value="Chercher" name="button_classification" />
<?php
if (isset($_GET['button_classification']))
{
  $ajout_sql_dispo='';
  if ($_GET['radio_dispo'])
  {
    $dispo='&amp;radio_dispo='.$radio_dispo;
    if ($radio_dispo=='yes')
      $ajout_sql_dispo=" AND t02.f3='0'";
  }

//classification par ATC
/*
t42 : classes therapeutiques
f0= code ->t43.f0, t4B.fa, t70.f4
premier chiffre= classe racine, etc.
f1=libelle anglais
f2:libelle francais
*/
  $classe="";
  $classe1="";
  $classe2="";
  $classe3="";
  $groupe="";
  if (isset($_GET['classe']))
  {
    $classe=$_GET['classe'];
    $classe_joker=$_GET['classe']."%";
    $sql_chercher_classe1="SELECT * FROM t42 WHERE f0 LIKE '$classe_joker'";
    $resultat_chercher_classe1=mysqli_query($db,$sql_chercher_classe1);
  }
  if (isset($_GET['classe1']))
  {
    $classe1=$_GET['classe1'];
    $classe1_joker=$_GET['classe1']."%";
    $sql_chercher_classe2="SELECT * FROM t42 WHERE f0 LIKE '$classe1_joker'";
    $resultat_chercher_classe2=mysqli_query($db,$sql_chercher_classe2);
  }
  if (isset($_GET['classe2']))
  {
    $classe2=$_GET['classe2'];
    $classe2_joker=$_GET['classe2']."%";
    $sql_chercher_classe3="SELECT * FROM t42 WHERE f0 LIKE '$classe2_joker'";
    $resultat_chercher_classe3=mysqli_query($db,$sql_chercher_classe3);
  }
  if (isset($_GET['classe3']))
  {
    $classe3=$_GET['classe3'];
    $classe3_joker=$_GET['classe3']."%";
    $sql_chercher_groupe="SELECT * FROM t42 WHERE f0 LIKE '$classe3_joker'";
    $resultat_chercher_groupe=mysqli_query($db,$sql_chercher_groupe);
  }
  if (isset($_GET['groupe']))
  {
    $groupe=$_GET['groupe'];
    $groupe_joker=$_GET['groupe']."%";
    $sql_chercher_medoc="SELECT f0 FROM t4B WHERE fA = '$groupe'";
    $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
  }
  echo '
	  <ul>';
  $sql_chercher_racine="SELECT * FROM t42";
  $resultat_chercher_racine=mysqli_query($db,$sql_chercher_racine);
  while ($ligne_chercher_racine=mysqli_fetch_array($resultat_chercher_racine))
  {
    if (strlen($ligne_chercher_racine['f0'])==1) //classe racine
    {
//radio_dispo=yes&radio_distri=4&radio_classe=1
?>
	    <li>
	      <a href="medocs.php?classe=<?php echo $ligne_chercher_racine['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $radio_distri ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo $ligne_chercher_racine['f2'] ?></a>
	    </li>
<?php
      if ($ligne_chercher_racine['f0']==$classe)
      {
	echo "
	    <li>
	      <ul>";
	while ($ligne_chercher_classe1=mysqli_fetch_array($resultat_chercher_classe1))
	{
	  if (strlen($ligne_chercher_classe1['f0'])==3)
	  {
?>
		<li>
		  <a href="medocs.php?classe=<?php echo $classe ?>&amp;classe1=<?php echo $ligne_chercher_classe1['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php   if (detectUTF8($ligne_chercher_classe1['f2'])) echo $ligne_chercher_classe1['f2']; else echo utf8_encode($ligne_chercher_classe1['f2']); ?></a>
		</li>
<?php
	    if ($ligne_chercher_classe1['f0']==$classe1)
	    {
	      echo "
		<li>
		  <ul>";
	      while ($ligne_chercher_classe2=mysqli_fetch_array($resultat_chercher_classe2))
	      {
		if (strlen($ligne_chercher_classe2['f0'])==4)
		{
?>
		    <li>
		      <a href="medocs.php?classe=<?php echo $classe ?>&amp;classe1=<?php echo $classe1 ?>&amp;classe2=<?php echo $ligne_chercher_classe2['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_classe2['f2']) ?></a>
		    </li>
<?php
		  if ($ligne_chercher_classe2['f0']==$classe2)
		  {
		    echo "
		    <li>
		      <ul>";
		    while ($ligne_chercher_classe3=mysqli_fetch_array($resultat_chercher_classe3))
		    {
		      if (strlen($ligne_chercher_classe3['f0'])==5)
		      {
?>
			<li>
			  <a href="medocs.php?classe=<?php echo $classe ?>&amp;classe1=<?php echo $classe1 ?>&amp;classe2=<?php echo $classe2 ?>&amp;classe3=<?php echo $ligne_chercher_classe3['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_classe3['f2']) ?></a>
			</li>
<?php
			if ($ligne_chercher_classe3['f0']==$classe3)
			{
			  echo "
			<li>
			  <ul>";
			  while ($ligne_chercher_groupe=mysqli_fetch_array($resultat_chercher_groupe))
			  {
			    if (strlen($ligne_chercher_groupe['f0'])==7)
			    {
?>
			    <li>
			      <a href="medocs.php?classe=<?php echo $classe ?>&amp;classe1=<?php echo $classe1 ?>&amp;classe2=<?php echo $classe2 ?>&amp;classe3=<?php echo $classe3 ?>&amp;groupe=<?php echo $ligne_chercher_groupe['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_groupe['f2']) ?></a>
			    </li>
<?php
                              if ($ligne_chercher_groupe['f0']==$groupe)
                              {
                                echo "
			    <li>
			      <ul>";
                                $fK=$radio_classe.'%';
                                $compteur=0;
                                while ($ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc))
                                {
                                  $this_cip=$ligne_chercher_medoc['f0'];
                                  //chercher les medocs trouves dans t00 + dispo + distribues + classe
//                                  $sql_chercher_specialite_dispo="SELECT t00.f1 AS cuv,t00.f3 AS nom FROM t00 INNER JOIN t02 ON t00.f1=t02.f0 WHERE t00.f0='$this_cip' $ajout_sql_dispo AND t00.fK LIKE '$fK' AND t02.f2 = '$radio_distri' ";
//                                 $sql_chercher_specialite_dispo="SELECT t00.f1 AS cuv,t00.f3 AS nom FROM t00 , t02 WHERE t00.f1=t02.f0 AND t00.f0='$this_cip' $ajout_sql_dispo AND t00.fK LIKE '$fK' AND t02.f2 = '$radio_distri' ";

				  $sql_chercher_specialite_dispo = "SELECT t00.f1 AS cuv,t00.f3 AS nom FROM t00  WHERE t00.f0='$this_cip'  AND t00.fK LIKE '$fK' ";


//echo "<br /> $sql_chercher_specialite_dispo<br />";
//$timestart=microtime(true);

                                  $resultat_chercher_specialite_dispo=mysqli_query($db,$sql_chercher_specialite_dispo);

                                  while ($ligne_chercher_specialite_dispo=mysqli_fetch_array($resultat_chercher_specialite_dispo))
                                  {
                                    $compteur++;
				    $cuv=$ligne_chercher_specialite_dispo['cuv'];
				    $sql_complement="SELECT * FROM  t02 WHERE f0='$cuv' $ajout_sql_dispo";
//				    echo $sql_complement;
				    if ($resultat_complement=mysqli_query($db,$sql_complement))
				    {
?>
				<li>
				  <a href="medocs.php?classe=<?php echo $classe ?>&amp;classe1=<?php echo $classe1 ?>&amp;classe2=<?php echo $classe2 ?>&amp;classe3=<?php echo $classe3 ?>&amp;groupe=<?php echo $groupe ?>&amp;cuv_medoc=<?php echo $ligne_chercher_specialite_dispo['cuv'].$dispo ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php if (detectUTF8($ligne_chercher_specialite_dispo['nom'])) echo $ligne_chercher_specialite_dispo['nom']; else  echo utf8_encode($ligne_chercher_specialite_dispo['nom']);  ?></a>
				</li>
<?php
				    }
				  }
/*
$timeend=microtime(true);
$time=$timeend-$timestart;

//Afficher le temps d'éxecution
$page_load_time = number_format($time, 3);
echo "Debut du script: ".date("H:i:s", $timestart);
echo "<br>Fin du script: ".date("H:i:s", $timeend);
echo "<br>Script execute en " . $page_load_time . " sec";
*/

                                }

                                if ($compteur==0)
                                {
?>
                                <li>
                                  Aucune réponse correspondant aux critères de recherche définis dans le cadre <i>Options</i>.
                                </li>
<?php
                                }
                                echo "
			      </ul>
			    </li>";
                              }
                            }
                          }
                          echo "
			  </ul>
			</li>";
                        }
                      }
                    }
                  echo "
		      </ul>
		    </li>";
                  }
                }
              }
              echo "
		  </ul>
		</li>";
            }
          }
        }
        echo "
	      </ul>
	    </li>";
      }
    }
  }
  echo '
	  </ul>';
} //fin if button_classification
echo '
	</fieldset>';
if (isset ($_GET['cuv_medoc']))
{
  $cuv_medoc=$_GET['cuv_medoc'];
  $sql_chercher_medoc="SELECT f0 AS cpg, f1 AS cuv,f2 AS cip, fG AS nom, f7 AS unite, f8 AS nombre,fJ FROM t00 WHERE f1 = '$cuv_medoc'";
  $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
  $ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc);
  $id_medoc=$ligne_chercher_medoc['cpg'];

//Images associees
/*
t48.fO -> t48.f1 = code image ->t46.f0 t46.f1=nom image
      t5D.f1 ->f0
	t00.f1
*/
  $sql_chercher_image="SELECT t46.f1 AS nom_image FROM t5D INNER JOIN t48 ON t5D.f1=t48.f0 INNER JOIN t46 ON t48.f1=t46.f0 WHERE t5D.f0='$cuv_medoc'";
  $resultat_chercher_image=mysqli_query($db,$sql_chercher_image);
  $ligne_chercher_image=mysqli_fetch_array($resultat_chercher_image);
  $image=$ligne_chercher_image['nom_image'];
  $image='vidal_goodies/IMAGES/'.substr($image,0,3).'/'.substr($image,3,3).'/'.substr($image,6);

//chercher le code voie administration dans t0A
  $sql_t0A="SELECT * FROM t0A WHERE f0='$id_medoc'";
  $resultat_t0A=mysqli_query($db,$sql_t0A);
  $ligne_t0A=mysqli_fetch_array($resultat_t0A);
//  $galenique=$ligne_t0A['f1'];

  $sql_ce_medoc="SELECT t4B.f0 AS numero,CAST(t4B.fF AS unsigned integer) AS psy, CAST(t4B.fG AS unsigned integer) AS dopant, t4B.fE AS vigi,t42.f0 AS classe_num,t4B.f2 AS nom,f5, t42.f2 AS classe_nom  FROM t4B INNER JOIN t42 ON t4B.fA=t42.f0 WHERE t4B.f0 = '$id_medoc'"; //id_medoc= code produit global
  $resultat_ce_medoc=mysqli_query($db,$sql_ce_medoc);
  $ligne_ce_medoc=mysqli_fetch_array($resultat_ce_medoc);

//interference etats pathologiques
/*
t52
  f0 = code du terme ->t50.f0
  f1 = CIP ->t00.f0
  f2 = nature du lien (contre-indique...) -> t1C.f0 (liste des codes) ou t51.f2
  f3 = frequence
  f5 = libelle en clair de l'effet
  f6 = code document ->t45.f0
*/
  $sql_etat_patho="SELECT t50.f1 AS patho,t52.f5 AS commentaire,t1C.f1 AS nature,t50.f0 AS code_unite, t52.f3 AS frequence_effet FROM t52 INNER JOIN t50 on t52.f0=t50.f0 INNER JOIN t1C ON t52.f2=t1C.f0 WHERE t52.f1='$id_medoc'";
  $resultat_etat_patho=mysqli_query($db,$sql_etat_patho);

  //requete chercher classe racine
  $debut_num_classe=substr($ligne_ce_medoc['classe_num'],0,1);
  $sql_chercher_classe_racine="SELECT * FROM t42 WHERE f0='$debut_num_classe'";
  $resultat_chercher_classe_racine=mysqli_query($db,$sql_chercher_classe_racine);
  $ligne_chercher_classe_racine=mysqli_fetch_array($resultat_chercher_classe_racine);

  $num_classe_1=substr($ligne_ce_medoc['classe_num'],0,3);
  $sql_chercher_classe_1="SELECT * FROM t42 WHERE f0='$num_classe_1'";
  $resultat_chercher_classe_1=mysqli_query($db,$sql_chercher_classe_1);
  $ligne_chercher_classe_1=mysqli_fetch_array($resultat_chercher_classe_1);

  $num_classe_2=substr($ligne_ce_medoc['classe_num'],0,4);
  $sql_chercher_classe_2="SELECT * FROM t42 WHERE f0='$num_classe_2'";
  $resultat_chercher_classe_2=mysqli_query($db,$sql_chercher_classe_2);
  $ligne_chercher_classe_2=mysqli_fetch_array($resultat_chercher_classe_2);

  $num_classe_3=substr($ligne_ce_medoc['classe_num'],0,5);
  $sql_chercher_classe_3="SELECT * FROM t42 WHERE f0='$num_classe_3'";
  $resultat_chercher_classe_3=mysqli_query($db,$sql_chercher_classe_3);
  $ligne_chercher_classe_3=mysqli_fetch_array($resultat_chercher_classe_3);

  //t63=precautions precription
    //f0=code CUV
    //f2= Code registre d'incription
    //f3 = liste I ou II
    //f4 duree maxi pour une delivrance unique
    //f7 sites de prescription si ordinaire ou renouvellement de restreinte
    //f8 site de prescription initiale
    //fB Portee d'une prescription initiale apres laquelle le renouvellement n'est plus possible
    //fD ordonnance securisee ou chevauchemnet impossible, selon t1C.f0->f1

  $sql_regles_delivrance="SELECT * FROM t63  WHERE f0='$cuv_medoc'";
  $resultat_regles_delivrance=mysqli_query($db,$sql_regles_delivrance);
  $ligne_regles_delivrance=mysqli_fetch_array($resultat_regles_delivrance);

  $fD=$ligne_regles_delivrance['fD'];
  $sql_delivrance_2="SELECT f1 FROM t1C WHERE f0='$fD'";
  $resultat_delivrance_2=mysqli_query($db,$sql_delivrance_2);
  $ligne_delivrance_2=mysqli_fetch_array($resultat_delivrance_2);

//t01 tarifs
  //f0 = CUV
  //f3 = prix ttc
  //f7= code remboursement
  $sql_prix="SELECT * FROM t01 WHERE f0='$cuv_medoc'";
  $resultat_prix=mysqli_query($db,$sql_prix);
  $ligne_prix=mysqli_fetch_array($resultat_prix);
  $rembt=$ligne_prix['f7'];
//Codes remboursement
//t1D = liste des codes
  //f0 = code
  //f1=libelle
  $sql_rembt="SELECT * FROM t1D WHERE f0='$rembt'";
  $resultat_rembt=mysqli_query($db,$sql_rembt);
  $ligne_rembt=mysqli_fetch_array($resultat_rembt);

/*
t03 Codes AMM
  f0 = CUV
  f1 = base de remboursement
  f2 = regime SS. Utiliser 01.7
  f3 = restriction de delivrance. Preferer 63.3
  f4 = date AMM
  f5 = code de delivrance, 6 bits utilises
    Bit 0 : "Spécialité pharmaceutique"
    Bit 1 : "Délivré seulement sur ordonnance"
    Bit 2 : "Délivrance soumise au contrôle médical de la Sécurité Sociale
    Bit 3 : "Vente interdite"
    Bit 4 : "Prescription restreinte" 
    Bit 5 : "Médicament d'exception" 
  f6 = agemeents divers-fait double emploi avec les evenements 00L
  f7 = sesam vitale B2 Utiliser 4E.6
  f9 = code commentaire sur la delivrance ->1C. Utiliser t61 et t 62 
  fC = duree maximale
*/

//evenements
//t4C
  //f0 = code CUV
  //f1 = type d'evenement
  //f2 date
//t1C libelles des evenements
  //f0 = num d'evenement
  //f1 = libelle evenement
  $sql_evnt="SELECT * FROM t4C INNER JOIN t1C ON t4C.f1=t1C.f0 WHERE t4C.f0='$cuv_medoc'";
  $resultat_evnt=mysqli_query($db,$sql_evnt);

//fabricants
//t02 
  //f1 = code fabricant ->t17.f0-t18.f0
  //f3 disponibilite ->code en t34
//t17 liste des fabricants
  //f2 nom
//t18 adresse des fabricants. Meme f0 que t17
  //f3 - f4 adresse
  //f5 code postal
  //f6 ville
  //f7 Tel
  //f9 tel2
  //fC mail
  //fD web

  $sql_fabricant="SELECT * FROM t02 INNER JOIN t17 ON t02.f1=t17.f0 where t02.f0='$cuv_medoc'";
  $resultat_fabricant=mysqli_query($db,$sql_fabricant);
  $ligne_fabricant=mysqli_fetch_array($resultat_fabricant);

  $num_fabricant=$ligne_fabricant['f0'];

  $sql_adresse="SELECT * FROM t18 WHERE F0='$num_fabricant'";
  $resultat_adresse=mysqli_query($db,$sql_adresse);
  $ligne_adresse=mysqli_fetch_array($resultat_adresse);

  //composants t07
    //f0 = code cpg
    //f1 = N° de section, associant les composants d'un même produit en groupes homogènes, décrits dans la table 08. Par exemple : Section 00 = Principes actifs, Section 01 = Excipients du noyau, Section 02 = Excipients de l'enrobage.Attention ! Ne pas confondre le numéro de section avec son qualificatif : les principes actifs ne coïncident pas nécessairement avec la section 00. -lie au f1 de t08
    //f2 = code substance -lie à t27-f0 ->libelle en f2
    //f4 = quantite
  //$sql_substance="SELECT * FROM t07 INNER JOIN t27 ON t07.f2=t27.f0 INNER JOIN t08 ON t07.f1=t08.f1 WHERE t07.f0='$id_medoc'";
  $sql_substance="SELECT * FROM t07 INNER JOIN t27 ON t07.f2=t27.f0 WHERE t07.f0='$id_medoc'";
  $resultat_substance=mysqli_query($db,$sql_substance);

//Interactions
//t4B-f4 = classe d'interactions->t10 f0 et f1 
//t10-f2= degre de gravite entre 1 et 4
//t10-f5 = libelle complet XML

  $find=array('&#x3C;','&#x3E;','<BR/>');
  $replace=array('&amp;lt;','&amp;gt;','\n');
  $ce_medoc_affichable=str_replace ($find,$replace,$ligne_ce_medoc['f5']);

  $xml=simplexml_load_string($ce_medoc_affichable);
//simpleXML remplace les entites < et > en balises
//il bouffe aussi les br
//et il convertit en utf-8

//chercher les documents lies
/*
t45
  f0= cuv
  f1=lien t49.f1
t49
  f0=lien t45
  f1=categorie de document (PGR...)
  f2=type de document (PDF...)
  f5=URL du document
*/
  $sql_documents_lies="SELECT * FROM t49 INNER JOIN t45 ON t49.f1=t45.f0 WHERE t49.f0='$id_medoc'";
  $resultat_documents_lies=mysqli_query($db,$sql_documents_lies);
  //echo $sql_documents_lies;
/*DOCUMENT LIE - ALD 	
DOCUMENT LIE - PRG 	
DOCUMENT LIE - FIT
DOCUMENT LIE - BUM
*/
  while ($ligne_documents_lies=mysqli_fetch_array($resultat_documents_lies))
  {
    if ('DOCUMENT LIE - RBU'==$ligne_documents_lies['f1'])
    {
      $link=substr($ligne_documents_lies['f5'],0,7).'/'.substr($ligne_documents_lies['f5'],7,3).'/'.strtolower(substr($ligne_documents_lies['f5'],10,3)).'/0'.substr($ligne_documents_lies['f5'],14,21);
echo $ligne_documents_lies['f5'].'<br />';
echo substr($ligne_documents_lies['f5'],13,21).'<br />';
      echo '<a href="vidal_goodies/'.$link.'">'.$ligne_documents_lies['f1'].'</a><br />';
    }
    elseif ('DOCUMENT LIE - ALD'==$ligne_documents_lies['f1'])
    {
 //	DS_DOCSALD76b8016d3696f8d61d786c02fa110bb6.pdf
      $link=substr($ligne_documents_lies['f5'],0,7).'/'.substr($ligne_documents_lies['f5'],7,3).'/'.substr($ligne_documents_lies['f5'],10,36);
echo $ligne_documents_lies['f5'].'<br />';
//echo substr($ligne_documents_lies['f5'],13,21).'<br />';
      echo '<a href="vidal_goodies/'.$link.'">'.$ligne_documents_lies['f1'].'</a><br />';
    }
  }
  $nb_rubr=0;
  /*t24 = codes unite
f0=code
f1=unite en clair
    */

  function unite_t24($unite_code,$db) //fonction pour trouver l'unite en clair en fonction de son code
  {
    if ($unite_code=="")
      return ;
    else
    {	
      $sql_unite="SELECT f1 FROM t24 WHERE f0='$unite_code'"; //f1 singulier, f5 pluriel
      $resultat_unite=mysqli_query($db,$sql_unite);
      $ligne_unite=mysqli_fetch_array($resultat_unite);
//return fix_utf8($ligne_unite['f1']);
return $ligne_unite['f1'];

/*      if (detectUTF8($ligne_unite['f1']))
	return utf8_encode($ligne_unite['f1']);
      else 
	return utf8_encode($ligne_unite['f1']); */
    }
  }
?>
	<table>
	  <tr style="vertical-align:top">
	    <th>
	      États pathologiques
	    </th>
	  <td>
<?php
while ($ligne_etat_patho=mysqli_fetch_array($resultat_etat_patho))
{
  if (detectUTF8($ligne_etat_patho['commentaire']))
    echo $ligne_etat_patho['commentaire'];
  else
    echo utf8_encode($ligne_etat_patho['commentaire']);
  echo ' <b> ';
  if (detectUTF8($ligne_etat_patho['nature']))
    echo $ligne_etat_patho['nature'];
  else
    echo utf8_encode($ligne_etat_patho['nature']);
  echo ' : </b> ';
  if (detectUTF8($ligne_etat_patho['patho']))
    echo $ligne_etat_patho['patho'];
  else 
    echo utf8_encode($ligne_etat_patho['patho']);

  if ($ligne_etat_patho['frequence_effet'])
{
  if (detectUTF8($ligne_etat_patho['frequence_effet']))
    echo ' <b>fréquence</b> : '.$ligne_etat_patho['frequence_effet'];
  else
    echo ' <b>fréquence</b> : '.utf8_encode($ligne_etat_patho['frequence_effet']);
}
$code_unite=$ligne_etat_patho['code_unite'];
$sql_unite_etat="SELECT t2A.f1 AS type_valeur, t24.f1 AS unite_valeur, t2G.f1 AS valeur_min, t2G.f2 AS valeur_max FROM t2G INNER JOIN t2A ON t2G.f0=t2A.f0 INNER JOIN t24 ON t2G.f3=t24.f0 WHERE t2G.f4='$code_unite'";
$resultat_unite_etat=mysqli_query($db,$sql_unite_etat);
$ligne_unite_etat=mysqli_fetch_array($resultat_unite_etat);
if ($ligne_unite_etat['type_valeur'])
{
  if (detectUTF8($ligne_unite_etat['type_valeur']))
    echo ' '.$ligne_unite_etat['type_valeur'];
  else
    echo ' '.utf8_encode($ligne_unite_etat['type_valeur']);
}
if ($ligne_unite_etat['valeur_min'])
  echo " min ".$ligne_unite_etat['valeur_min'];
if ($ligne_unite_etat['valeur_max'])
  echo " max : ".$ligne_unite_etat['valeur_max'];
if ($ligne_unite_etat['unite_valeur'])
{
  if (detectUTF8($ligne_unite_etat['unite_valeur']))
    echo " ".$ligne_unite_etat['unite_valeur'];
  else
    echo " ".utf8_encode($ligne_unite_etat['unite_valeur']);
}
echo '<br />';
}
?>
	  </td>
	</tr>

<?php
if ($ligne_chercher_image['nom_image'])
{
?>
	  <tr>
	    <th>
	      Image
	    </th>
	    <td>
	      <p class="info">Boîte<span><img src="<?php echo $image; ?>" alt="image du produit" /></span></p>
	    </td>
	  </tr>
<?php
}
?>
	  <tr valign="top">
	    <th>
	      Num&eacute;ro
	    </th>
	    <td>
	      <strong>CPG</strong> : <?php echo $ligne_ce_medoc['numero'] ?> <strong>CUV ou CIS</strong> : <?php echo $cuv_medoc ?> <strong>CIP</strong> : <?php echo $ligne_chercher_medoc['cip'] ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Nom
	    </th>
	    <td>
	      <?php  if (detectUTF8($ligne_chercher_medoc['nom'])) echo $ligne_chercher_medoc['nom']; else echo utf8_encode($ligne_chercher_medoc['nom']); ?>
	    </td>
	  </tr>
	  <tr valign="top"> 
	    <th>
	      <a name="composants"></a>Composants
	    </th>
	    <td>
<?php 
/*composants t08
f0->t4B.f0 = cpg
f2->t1C.f0 = descripteur de composant (f1)//libelles dans t1C
f6->t24.f0 = unite (f1)
f8->t24.f0
fA->t24.f0
*/

  $cette_ligne="";
  while ($ligne_substance=mysqli_fetch_array($resultat_substance))
  {

    $cette_ligne=$ligne_substance['f1'];
    $sql_section_composant="SELECT * FROM t08  WHERE f1='$cette_ligne'";
//echo $sql_section_composant;
    $resultat_section_composant=mysqli_query($db,$sql_section_composant);
    $ligne_section_composant=mysqli_fetch_array($resultat_section_composant);

    if ($ligne_section_composant['fB']=='0') 
    {
      echo '<strong>Principe actif </strong>'; 
      $principe_actif=$ligne_substance['f2'];
      $quantite_substance=$ligne_substance['f4'];
      $unite_substance=$ligne_substance['f5'];
    }
    else 
      echo '
	      <strong>Excipient</strong> ';
    if (detectUTF8($ligne_substance['f2']))
      echo ' <a href="medocs.php?radio_dispo='.$radio_dispo.'&amp;nom_medoc='.urlencode($ligne_substance['f2']).'&amp;envoyer_composition_medoc=DCI">'.$ligne_substance['f2'].'</a> '.$ligne_substance['f4'].' '.unite_t24($ligne_substance['f5'],$db).'<br />';
    else
      echo ' <a href="medocs.php?radio_dispo='.$radio_dispo.'&amp;nom_medoc='.urlencode(utf8_encode($ligne_substance['f2'])).'&amp;envoyer_composition_medoc=DCI">'.utf8_encode($ligne_substance['f2']).'</a> '.$ligne_substance['f4'].' '.unite_t24($ligne_substance['f5'],$db).'<br />';
  }
  if ($xml)
  {
    foreach ($xml->RUBRIQUE as $value)
    {
//=utf8_decode($value);
//print_r($value);
      //on enleve les espaces et apostrophes et autres trucs pour faire des liens
      $find_link=array(' ',"'","(",")");
      $replace_link=array('_',"_","","");
      $titre_link=str_replace($find_link,$replace_link,$value['TITRE']);
?>
            </td>
          </tr>
          <tr valign="top">
            <th>
              <a name="<?php echo $titre_link ?>"></a>Caractéristiques
            </th>
            <td>
	      <h2>
		<?php echo $value['TITRE'] ?>
	      </h2> 
<!-- titre premier niveau -->
<?php
	      str_replace('\n','<br />',$value); //contenu de premier niveau- on remet les retours chariot
      $nb_ssrubr=0;
      foreach ($xml->RUBRIQUE[$nb_rubr] as $value_ssrubr)
      {
        if ($value_ssrubr['TITRE'])
          echo  '
	      <h3>
		'.$value_ssrubr['TITRE'].'
	      </h3>'. //titre deuxieme niveau
	      str_replace('\n','<br />',$value_ssrubr); //contenu de ss rubr
        if ($xml->RUBRIQUE[$nb_rubr]->SSRUBR[$nb_ssrubr])
        {
          foreach ($xml->RUBRIQUE[$nb_rubr]->SSRUBR[$nb_ssrubr] as $value_cas)//warning si vide
          {
            echo '
	      <h4>
		'.$value_cas['TITRE'].'
	      </h4>'. //titre troisieme niveau
	      str_replace('\n','<br />',$value_cas); //contenu de 3eme niveau
          }
        }
        $nb_ssrubr++;
      }
      $nb_rubr++;
    }
  }
 ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Posologie journalière
	    </th>
<?php
/*t0A = voie d'adminstration selon code medicament
    f0=code medoc
    f1=code admin (t15.f0)

t15 = codes voie administration
  f0=code  (18...)
  f1=libelle (dentaire...)
*/
  $sql_voie="SELECT t15.f1,t15.f0 FROM t0A INNER JOIN t15 ON t0A.f1=t15.f0 WHERE t0A.f0='$id_medoc'";
  $resultat_voie=mysqli_query($db,$sql_voie);
  $ligne_voie=mysqli_fetch_array($resultat_voie); 
  $voie_code=$ligne_voie['f0']; //dans t15
  $voie_clair=$ligne_voie['f1'];
  /*t43=poso
      f0= code medoc
      f1=voie ->t15.f0
      f2=dose quotidienne
      f3=code unite ->t24.f1
      f4=substance ->t27.f0
  */
  $this_group=$ligne_ce_medoc['classe_num'];
  $sql_poso="SELECT * FROM t43 WHERE f0='$this_group' AND f1='$voie_code'";
  $resultat_poso=mysqli_query($db,$sql_poso);
  $ligne_poso=mysqli_fetch_array($resultat_poso);
  $substance=$ligne_poso['f4'];

  $unite_code=$ligne_chercher_medoc['unite'];
  $ligne_unite_prise=unite_t24($unite_code,$db);

/*t27 substances
    f0=code substance ->t07.f2,t11.f1,t28.f0,t28.f1,t3G.f0,t43.f4,t54.f0,t58.f1,t59.f0,t59.f1,t5I.f0 et f1
    f2= nom substance
    f6=interaction ->t26.f0
    f8=code OMS
    fB=dopant MJS

*/
  $sql_substance_poso="SELECT * FROM t27 WHERE f0='$substance'";
  $resultat_substance_poso=mysqli_query($db,$sql_substance_poso);
  $ligne_substance_poso=mysqli_fetch_array($resultat_substance_poso);

  /*t25 codes galeniques
    f0= code lie a  ?? (pas t4b.f1)
    f1=libelle
    f4=libelle abrg
  */
  $galenique=$ligne_chercher_medoc['fJ'];
  $sql_galenique="SELECT * FROM t25 WHERE f0='$galenique'";
  $resultat_galenique=mysqli_query($db,$sql_galenique);
  $ligne_galenique=mysqli_fetch_array($resultat_galenique);

/*dosage t07
f0=code produit
f1=code section de la table 8
f2=code substance (t27.0)
f3=ordre dans la section (t8)
f4=quantite
f5=code unite (voir t24.f0)
*/

  $ligne_unite=unite_t24($unite_substance,$db);
  $unite_jour=$ligne_poso['f3'];

?>
	    <td>
	      <a name="poso"></a><?php echo $ligne_poso['f2'].' '.unite_t24($unite_jour,$db).' de '.$ligne_substance_poso['f2'].' (voie '.utf8_encode($ligne_voie['f1']).' en  '.utf8_encode($ligne_unite_prise).' '.$ligne_galenique['f1'].' '.$quantite_substance.' '.unite_t24($unite_substance,$db).') '?> - boîtes de <?php echo $ligne_chercher_medoc['nombre']?> (posologie pour un adulte mâle de 75 Kg) 
	    </td>
	  </tr>
          <tr>
            <th>
              Code Dopage MJS
            </th>
            <td>
              <?php echo $ligne_substance_poso['fB']?>
            </td>
          </tr>
<?php
/*poso = t2P
f0=code lie a t2M.f0
f1=code produit = CIP
f2=quantite minimum
f3=unite quantite minimum f2 (t24.f0 ->t24.f1)
f4=duree quantite minimum (en clair = par jour)
f5=frequence des prises minimum
f6=code unite de frequence ->t24.f0 ->f1 en clair
f7=quantite maxi
f8=unite
f9=duree qt maxi
fA=freq prises
fB=unite
fC=dose maximale
fD=code unite dose maxi
fE=duree dose maxi
fF=freq des prises a doses maxi
fG=code unite
fH=qtte min par prise
fI=code unite
fJ=qtt max par prise
fK=code unite
fL=duree min tt
fM=code unite
fN=duree maxi
fO=code unite
fP=poso fixe O/N
fQ=duree min absolue
fR=code unite
fS=duree maxi
fT=code unite
*/

/*
t2L = bornes de prescription
f1= borne inferieure
f2=borne sup
f3=code unite (t24.f0)
f6=descripteur = A (llaitement), F(emme)
*/
  $sql_posologies="SELECT * FROM t2P WHERE f1='$id_medoc'"; //cpg
  $resultat_posologies=mysqli_query($db,$sql_posologies);
  while ($ligne_posologies=mysqli_fetch_array($resultat_posologies))
  {
    $code_liaison_T2L=$ligne_posologies['f0'];
    $sql_bornes="SELECT * FROM t2L WHERE f4='$code_liaison_T2L'";
    $resultat_bornes=mysqli_query($db,$sql_bornes);
    $ligne_bornes=mysqli_fetch_array($resultat_bornes);
    $borne_inf=$ligne_bornes['f1'];
    $borne_sup=$ligne_bornes['f2'];
    $borne_unit=$ligne_bornes['f3'];

    $unite_borne=unite_t24($borne_unit,$db);

    $unite_min=$ligne_posologies['f3'];
    $unite_max=$ligne_posologies['f8'];
    $frequence_mini_code=$ligne_posologies['f6'];
    $frequence_maxi_code=$ligne_posologies['fF'];
  //unite de la poso mini

    $unite_min_clair=unite_t24($unite_min,$db);

  //unite de la poso maxi

    $unite_max_clair=unite_t24($unite_max,$db);
  //unite de la frequence mini
    $frequence_mini_clair='';
    if ($frequence_mini_code) //ne rien afficher si non renseigne
    {
      $frequence_mini_clair=unite_t24($frequence_mini_code,$db);
    }

?>
	  <tr valign="top">
	    <th>
	      Fourchette des posologies
	    </th>
	    <td>
	      <a name="poso"></a><?php echo 'de '.$ligne_posologies['f2'].' '.utf8_encode($unite_min_clair).' '.$ligne_posologies['f4'].' à '.$ligne_posologies['f7'].' '.utf8_encode($unite_max_clair).' '.$ligne_posologies['f9'];?> de <?php echo $borne_inf?> à <?php echo $borne_sup.' '.$unite_borne ?>
	    </td>
	  </tr>
<?php
  }
?>
	  <tr valign="top">
	    <th>
	      Classe racine
	    </th>
	    <td>
	      <a href="medocs.php?classe=<?php echo $ligne_chercher_classe_racine['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_classe_racine['f2']).'</a> ('.$ligne_chercher_classe_racine['f0'].')' ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Classe 1
	    </th>
	    <td>
	      <a href="medocs.php?classe=<?php echo $ligne_chercher_classe_racine['f0'] ?>&amp;classe1=<?php echo $ligne_chercher_classe_1['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_classe_1['f2']) ?></a><?php echo $ligne_chercher_classe_1['f0'] ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Classe 2
	    </th>
	    <td>
	      <a href="medocs.php?classe=<?php echo $ligne_chercher_classe_racine['f0'] ?>&amp;classe1=<?php echo $ligne_chercher_classe_1['f0'] ?>&amp;classe2=<?php echo $ligne_chercher_classe_2['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_classe_2['f2']) ?></a><?php echo $ligne_chercher_classe_2['f0'] ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Classe 3
	    </th>
	    <td>
	      <a href="medocs.php?classe=<?php echo $ligne_chercher_classe_racine['f0'] ?>&amp;classe1=<?php echo $ligne_chercher_classe_1['f0'] ?>&amp;classe2=<?php echo $ligne_chercher_classe_2['f0'] ?>&amp;classe3=<?php echo $ligne_chercher_classe_3['f0'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_chercher_classe_3['f2']) ?></a><?php echo $ligne_chercher_classe_3['f0'] ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Groupe
	    </th>
	    <td>
	      <a href="medocs.php?classe=<?php echo $ligne_chercher_classe_racine['f0'] ?>&amp;classe1=<?php echo $ligne_chercher_classe_1['f0'] ?>&amp;classe2=<?php echo $ligne_chercher_classe_2['f0'] ?>&amp;classe3=<?php echo $ligne_chercher_classe_3['f0'] ?>&amp;groupe=<?php echo $ligne_ce_medoc['classe_num'] ?>&amp;button_classification=Chercher&amp;radio_dispo=<?php echo $radio_dispo?>&amp;radio_distri=<?php echo $_GET['radio_distri'] ?>&amp;radio_classe=<?php echo $radio_classe ?>"><?php echo utf8_encode($ligne_ce_medoc['classe_nom']).'</a> ('.$ligne_ce_medoc['classe_num'].')'  ?>
	    </td>
	  </tr>
	  <tr>
<!-- //t63=precautions precription
      //f0=code CUV
      //f2= libelle precaution francais
      //f3 = liste I ou II
      //f4 duree maxi pour une delivrance unique
      //f6 sites de prescription si ordinaire ou renouvellement de restreinte
      //f8 site de prescription initiale
      //fB Portee d'une prescription initiale apres laquelle le renouvellement n'est plus possible
      //fD ordonnance securisee ou chevauchement impossible-->

	    <th>
              Précautions de délivrance
            </th>
	    <td>
	      <a name="Precautions_delivrance"></a><?php echo str_replace ('\n','<br />',utf8_encode($ligne_regles_delivrance['f2'])).'<br />'; if ($ligne_regles_delivrance['f3']) echo "Liste ".$ligne_regles_delivrance['f3'] ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Précautions
	    </th>
	    <td>
	      <a name="Precautions"></a><?php echo utf8_encode($ligne_ce_medoc['vigi']); if ($ligne_ce_medoc['psy']==1) echo ' : <br /><strong>Psychotrope</strong>'; if ($ligne_ce_medoc['dopant']==1) echo '<br /><strong>Dopant</strong>'; if ( $ligne_regles_delivrance['f1']) echo '<br />Liste'. $ligne_regles_delivrance['f1'];  if ( bin2hex($ligne_regles_delivrance['fB'])=='01') echo '<br /><strong>Ordonnance produit d\'exception</strong>';  if ( bin2hex($ligne_regles_delivrance['fC'])=='01') echo '<br /><strong>Déconditionnement autorisé</strong>'; echo '<br />'.utf8_encode($ligne_delivrance_2['f1']) ?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Prix
	    </th>
	    <td>
	      <a name="Prix"></a>TTC : <?php echo $ligne_prix['f3']; ?> € taux <?php echo utf8_encode($ligne_rembt['f1']) ?>
	    </td>
	  </tr>
	  <tr>
	    <th>
	      Durée maxi
	    </th>
	    <td>
	      <a name="duree"></a><?php if ( $ligne_regles_delivrance['fA']) echo '<strong>Durée maximale pour une prescription initiale</strong> '.$ligne_regles_delivrance['fA'].' jours';if ( $ligne_regles_delivrance['f4']) echo '<strong>Durée maximale pour une délivrance unique</strong> '.$ligne_regles_delivrance['f4'].' jours';?>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Évènements
	    </th>
	    <td>
	      <a name="event"></a>
	      <ul>
<?php 
  while ($ligne_evnt=mysqli_fetch_array($resultat_evnt))
    echo '
		<li>
		  '.utf8_encode($ligne_evnt['f1']).' : '.substr($ligne_evnt['f2'],0,10).'
		</li>';
?>
	      </ul>
	    </td>
	  </tr>
	  <tr valign="top">
	    <th>
	      Laboratoire
	    </th>
	    <td>
	      <a name="labo"></a><?php echo utf8_encode($ligne_fabricant['f2']).'<br />'.utf8_encode($ligne_adresse['f3']).'<br />'.$ligne_adresse['f4'].'<br />'.$ligne_adresse['f5'].'<br />'.utf8_encode($ligne_adresse['f6']).'<br />Tél : '.str_replace(" ","&nbsp;",$ligne_adresse['f7']).'<br />'.$ligne_adresse['fC']; if ($ligne_adresse['fD']) echo '<br /><a href="http://'.$ligne_adresse['fD'].'">'.$ligne_adresse['fD'].'</a>';?>
	    </td>
	  </tr>
<?php
/*$sql_t4R="SELECT * FROM t4R WHERE f0=''";
t4R
  f0 = Code de la prescription restreinte réservée à la LPP
  f1= liaison t4E f0 ??
  f2=quantite
  f3=duree maxi
  f4=code unite duree tt -> 22.f0
  f5=delai avant renouvellt
  f6=code unite de f5 -> 22.f0
  f7=montant maxi
  f8=duree montant maxi
  f9=code unite de f8 -> 22.f0
  fA=commentaire
  fB=taille mini en cm
  fC=taille maximale
  fD=poids mini
  fE=poids maximal
  fF=pointure minimum
  fG=pointure maximal
  fH=age mini annees
  fI=age maxi
  fJ=femme enceinte

t4E entites remboursables
  f0=identifiant de classe. -> t4R.f1 ->>t4K.f1
  f1= nom en clair
  f2=noeud =null
  f3=classe mère ->  t4K.f1 ?
  f4=LPPR Liste des Prestations et Produits Remboursables
  f5=base de remboursement (TFR)
  f6=code prestation pour le remboursement
  f7= type de prestation (location)
  f8=code entente prealable O/N
  f8 = code entente préalable
  f9= prix public
  fE = remboursement indication

t4K = relation cuv-lppr
  f0 = CUV
  f1 = classe LPPR -> table codes lppr ->4E.f0
  f2 = nombre de classes par CUV

//Note : pour un meme CUV, il peut y avoir plusieurs classes LPPR, pas toutes liees a t4E.
*/
/*
  $sql1="SELECT * FROM t4K  WHERE f0='$cuv_medoc'";
  $resultat1=mysqli_query($db,$sql1);
  $ligne1=mysqli_fetch_array($resultat1);
*/
//  $sql="SELECT t4E.f0 AS id_class, t4E.f5 AS tfr FROM t4R INNER JOIN t4E ON t4R.f1=t4E.f3 INNER JOIN t4K ON t4K.f1=t4E.f0 WHERE t4K.f0='$cuv_medoc'";

  $sql="SELECT t4E.f0 AS id_class, t4E.f5 AS tfr, t4K.f1 AS lien FROM t4E INNER JOIN t4K ON t4K.f1=t4E.f0 WHERE t4K.f0='$cuv_medoc'";
  $resultat=mysqli_query($db,$sql);
//   $count =mysqli_num_rows($resultat);

?>
          <tr>
            <th>
              Classe LPPR
            </th>
            <td>
 <?php 
  $ligne=mysqli_fetch_array($resultat);
  echo 
	      $ligne['id_class'];
 ?>
            </td>
          </tr>
          <tr>
            <th>
              <a name="tfr"></a>Base remboursable (TFR)
            </th>
            <td>
              <?php echo $ligne['tfr'] ; ?>
            </td>
          </tr>
<?php
/*
Le SMR (service medical rendu)
t3C
  f0 identifiant unique SMR ou ASMR  ->3D.f1, 3D.f2
  f1 code produit ->t00.f0 = CIP ->t4Bf0
  f2 indications  S : SMR, A : ASMR 
  f3 relation 0 -> 106 -> t3E.f0
  f4 Indication SMR ASMR = texte en clair
  f5 = date
t3D
  f0 identifiant de la relation
  f1 identifiant SMR de la relation
  f2 identifiant ASMR de la relation 
t3E
  f0 identifiant, lien  sur 3C.f3
  f1 = type de SMR en clair 0->5 = colonne en clair 101 ->106 = code, voir en clair dans f2
  f2 = valeur en clair des types de f1 sup à 100
*/
$cip_medoc=$ligne_ce_medoc['numero'];

$sql_chercher_smr="SELECT t3C.f2 AS a_ou_s, t3C.f4 AS texte_smr, t3E.f1 AS code_smr, t3E.f2 AS asmr_en_clair FROM t3C INNER JOIN t3E ON t3C.f3=t3E.f0  WHERE t3C.f1='$cip_medoc'"; //en fait, cpg
$resultat_chercher_smr=mysqli_query($db,$sql_chercher_smr);
?>
	  <tr>
	    <th>
	      <a name="smr"></a>SMR
	    </th>
	    <td>
<?
while ($ligne_chercher_smr=mysqli_fetch_array($resultat_chercher_smr))
{
  echo $ligne_chercher_smr['a_ou_s'].' '.utf8_encode($ligne_chercher_smr['texte_smr']).'  '.utf8_encode($ligne_chercher_smr['code_smr']).'  '.utf8_encode($ligne_chercher_smr['asmr_en_clair']).'<br />';
}
?>
	    </td>
	  </tr>
<!-- 4D les generiques -->
<?php
/*
t3F
  f0= CIP
  f1= groupe de produits
  f2= groupe de reference (1,6,7 ou 8) ->t1F.f0, avec t1F.f1= libelle
  f3=glossaire ->t1C.f0n t1C.f1= libellé
  f5 excipients a effet notoire en XML
*/
//$sql_generique="SELECT t3F.f2 AS reference, t1F.f1 AS libelle FROM t3F INNER JOIN t1F ON t3F.f2=t1F.f0 WHERE f0='$cip_medoc'";
$sql_generique="SELECT t3F.f2 AS reference, t1F.f1 AS libelle, t3F.f5 AS excipient, t3F.f1 AS groupe_generique  FROM t3F INNER JOIN t1F ON t3F.f2=t1F.f0 WHERE t3F.f0='$cip_medoc' AND t3F.f2 !='8'";
$resultat_generique=mysqli_query($db,$sql_generique);
?>
	  <tr>
	    <th>
	      <a name="generique"></a>Générique
	    </th>
	    <td>
<?php
$groupe_generique='';
while ($ligne_generique=mysqli_fetch_array($resultat_generique))
{
?>
	      <b>Référence</b> <?php echo utf8_encode($ligne_generique['libelle']) ?><br />
	      <b>Excipient à effet notoire</b> <?php echo utf8_encode($ligne_generique['excipient']) ?><br />
<?php
  $groupe_generique=$ligne_generique['groupe_generique'];
}

?>
	    </td>
	  </tr>
	  <tr>
	  <th>
	    Génériques
	  </th>
	  <td>
<?php
//On cherche les medicaments qui sont dans le meme groupe generique (t3F.f1)
$sql_les_generiques="SELECT t4B.f2 AS ce_medicament FROM t4B INNER JOIN t3F ON t4B.f0=t3F.f0 WHERE t3F.f1='$groupe_generique'";
//echo $sql_les_generiques;
$resultat_les_generiques=mysqli_query($db,$sql_les_generiques);
while ($ligne_les_generiques=mysqli_fetch_array($resultat_les_generiques))
{
  echo utf8_encode($ligne_les_generiques['ce_medicament']).'<br />';
}
?>
	    </td>
	  </tr>
	</table>
<?php
//  }
} //fin fiche medicament
?>
      </form>
    </div>

<?php
include("inc/footer.php");
?>
    </div>
    <p>
      <a href="http://validator.w3.org/check?uri=referer"><img
	  src="pics/valid-xhtml10.png" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
    </p>
  </body>
</html>
