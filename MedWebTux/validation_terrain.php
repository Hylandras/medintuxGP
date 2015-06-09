<?php
/*
Page non affichable destinee a enregistrer les nouvelles donnees du terrain, envoyees par la page tableaux.php. Renvoie a tableaux.
*/
session_start() ;
include("config.php");

if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=liste' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db){
  print "Erreur de connexion &agrave; $host<br>";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base)){
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}

function local_to_fr($date,$date_format) //fonction pour convertir les dates du terrain en fr, car stockees ainsi 
{
  $list_date=explode ("-",$date);
  if ($date_format=='fr')
  {
    $date=$date;
  }
  elseif ($date_format=='en')
    $date=$list_date[1].'-'.$list_date[0].'-'.$list_date[2];
  else //iso
    $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
  return $date;
}

$basemed="DatasempTest";
//on cherche les droits de l'utilisateur
$sql_chercher_droits="SELECT * FROM Personnes WHERE Login='$user'";
$resultat_chercher_droits=mysqli_query($db,$sql_chercher_droits);
$ligne_chercher_droits=mysqli_fetch_array($resultat_chercher_droits);
$GUID_user=$ligne_chercher_droits['GUID'];
$droits_user=$ligne_chercher_droits['Droits'];
$qualite_user=$ligne_chercher_droits['Qualite'];

if (isset($_GET['utilisateur_autorisant']))
  $utilisateur_autorisant=$_GET['utilisateur_autorisant'];
else
  $utilisateur_autorisant=$_SESSION['login'];

//recuperation des donnees de l'url
$id_terrain=$_REQUEST['id_terrain'];
$ddr='';
if (isset($_REQUEST['ddr'])) //pas de ddr pour les hommes
  $ddr=$_REQUEST['ddr'];
$coche_allaitement='';
if (isset($_REQUEST['coche_allaitement']))
  $coche_allaitement=$_REQUEST['coche_allaitement'];
$date=$_REQUEST['date']; //date d'une nouvelle ligne de tableau
$tableau_variables=$_REQUEST['tableau_variables']; //les valeurs a ajouter pour les tableaux
$text_atcd_libre=''; //atcd libre 
$select_intolerance=array();
$intolerance_active=''; //atcd medicamenteux actif yes-no
$intolerance_ald='';
/*$select_family_intolerance='';
$select_genre_intolerance='';
$intolerance_ald='';
$comment_intolerance='';
$date_intolerance='';
*/
$select_atcd=array();
/*select_family_atcd
select_genre_atcd
atcd_ald */
$atcd_active='';//atcd de pathologie actif yes-no
$atcd_ald='';

if ($_REQUEST['select_atcd'])
{
  $select_atcd=explode(' code cim10 ',$_REQUEST['select_atcd']);
  if ($_REQUEST['atcd_active']=='yes')
    $atcd_active='Actif';
  elseif ($_REQUEST['atcd_active']=='no')
    $atcd_active='Passé';
  if ($_REQUEST['atcd_ald']=='yes')
    $atcd_ald='ALD';
  $atcd_full=$select_atcd[0].' = '.$_REQUEST['select_family_atcd'].'('.$_REQUEST['select_genre_atcd'].') , '.$atcd_active.' , ~'.$select_atcd[1].'~ , '.$_REQUEST['comment_atcd'].' , '.local_to_fr($_REQUEST['date_atcd'],$date_format).','.$atcd_ald.'
';
}
if ($_REQUEST['select_intolerance'])
{
  $select_intolerance=explode(' code substance ',$_REQUEST['select_intolerance']);
  if ($_REQUEST['intolerance_active']=='yes')
    $intolerance_active='Actif';
  elseif ($_REQUEST['intolerance_active']=='no')
    $intolerance_active='Passé';

  if ($_REQUEST['intolerance_ald']=='yes')
    $intolerance_ald='ALD';

  $intolerance_full=$select_intolerance[0].' = '.$_REQUEST['select_family_intolerance'].'('.$_REQUEST['select_genre_intolerance'].') , '.$intolerance_active.' ,-'.$select_intolerance[1].'- , '.$_REQUEST['comment_intolerance'].' , '.local_to_fr($_REQUEST['date_intolerance'],$date_format).','.$intolerance_ald.'
';

}
if (isset($_REQUEST['text_atcd_libre']))
{
  $text_atcd_libre=$_REQUEST['text_atcd_libre'];
  $active='';
  if (isset($_REQUEST['text_active']))
  {
    if ($_REQUEST['text_active']=='yes')
      $active='Actif';
    elseif ($_REQUEST['text_active']=='no')
      $active='Passé';
  }
  $text_ald='';
  if (isset($_REQUEST['text_ald']))
  {
    if ($_REQUEST['text_ald']=='yes')
      $text_ald='ALD';
  }
  $text_atcd_full=$_REQUEST['text_atcd_libre'].' = '.$_REQUEST['select_family'].'('.$_REQUEST['select_genre'].') , '.$active.' , , '.$_REQUEST['comment_atcd_libre'].' , '.local_to_fr($_REQUEST['date_atcd_libre'],$date_format).','.$text_ald.'
';
}

if ($ddr) 
  $grossesse=$ddr;
else
  $grossesse="non";

if ($coche_allaitement)
  $allaitement="oui";
else
  $allaitement="non";

$compteur_date=1;
$bonne_date=array();
foreach ($date AS $cette_date)
{
  $chaine[$compteur_date]=$cette_date." = ";
  $compteur_variables=1;
  foreach ($tableau_variables[$compteur_date] AS $cette_variable) //On analyse les differentes dates pour un tableau de variables
  {
    if ($cette_variable)
    {
      $bonne_date[$compteur_date] = 1; // on cherche si au moins une valeur de cette variable est non vide
      $cette_variable=preg_replace("`,`",".",$cette_variable); //on remplace les eventuelles virgules dans les valeurs par des points pour ne pas se melanger avec les virgules separatrices
    }
    if ($compteur_variables == 1) //on traite differemment le premier cas au niveau des virgules
      $chaine[$compteur_date]=$chaine[$compteur_date].$cette_variable;
    else
      $chaine[$compteur_date]=$chaine[$compteur_date]." , ".$cette_variable;
    $compteur_variables++;
  }
  if (isset($bonne_date[$compteur_date]))
  {
    $inserer[$compteur_date]=1;
  }
  else
  {
    $inserer[$compteur_date]="";
  }
  $compteur_date++;
}

$sql_chercher_terrain="SELECT * FROM RubriquesBlobs WHERE RbDate_PrimKey='$id_terrain'";
$resultat_chercher_terrain=mysqli_query($db,$sql_chercher_terrain);
$ligne_chercher_terrain=mysqli_fetch_array($resultat_chercher_terrain);
$terrain=$ligne_chercher_terrain['RbDate_DataRub'];
$terrain=str_replace("\r","",$terrain);

$GUID_patient=$ligne_chercher_terrain['RbDate_IDDos'];
$split_terrain=explode("\n", $terrain);//tableau dont chaque valeur est une ligne du terrain
$num_terrain=0;

$chaine_proprietes='';
if (isset($_REQUEST['select_ordo']))
{
  $cip_fond=array();
  $select_ordo=$_REQUEST['select_ordo']; //le numero d'ordonnance a mettre en traitement de fond ou vide si pas de tt de fond ou X si inchange
  if ($select_ordo!="X") //X = inchange
  {
    $sql_chercher_ordo="SELECT * FROM RubriquesBlobs WHERE RbDate_PrimKey='$select_ordo'"; //on cherche l'ordo par son numero
    $resultat_chercher_ordo=mysqli_query($db,$sql_chercher_ordo);
    $ligne_chercher_ordo=mysqli_fetch_array($resultat_chercher_ordo);
    $ordo=$ligne_chercher_ordo['RbDate_DataRub']; //le contenu de l'ordo

    if (!mysqli_select_db($db,$basemed))
    {
      print "Erreur ".mysqli_error()."<br />";
      mysqli_close($db);
      exit;
    }

  //On cherche les noms des medocs pour la rubrique Propriétés Ordonnance
    $expr="`(<cip>)(.*)(<\/cip>)`i"; //rechercher les CIP du traitement de fond pour les interactions
    $split_ordo=explode("\n", $ordo); //on recupere chaque ligne de l'ordo

    foreach ($split_ordo as $ligne_brut)
    {
      if(preg_match($expr,$ligne_brut,$tab))
      {
        $cip_fond[]=$tab[2]; //On met toutes les cip dans un tableau
      }
    }
    $compteur_medocs=0;
    foreach ($cip_fond AS $this_cip_fond) //On recherche les libelles des medocs de l'ordo pour les mettre en traitement de fond
    {
      $sql_chercher_libelle="SELECT f3 FROM t00 WHERE f2='$this_cip_fond'"; 
      $resultat_chercher_libelle=mysqli_query($db,$sql_chercher_libelle);
      $ligne_chercher_libelle=mysqli_fetch_array($resultat_chercher_libelle);
      $chaine_proprietes.=$ligne_chercher_libelle['f3'].' = Renouvelable , '.$compteur_medocs.'
  ';
//$chaine_proprietes contient la prescription standard de tous les medocs en clair pour affichage
      $compteur_medocs++;
    }
  } //fin mode nouvelle ordonnance
}

$update="";
$flag_atcd='';
$effacer_ordo='';
$flag_ancienne_ordo='';
//on fabrique le nouveau terrain par concatenation des lignes d'origine + 
//remplacement des lignes + grossesse allaitement + traitement de fond
//+ajout des nouvelles lignes de tableaux
$effacer_prop='';
$var='';
foreach ($split_terrain as $ligne_brut)
{
  if (preg_match("`Grossesse =`",$ligne_brut))
  {
    $update=$update."Grossesse = ".$grossesse.'
';
  }
  elseif (preg_match("`Allaitement =`",$ligne_brut))
  {
    $update=$update."Allaitement = ".$allaitement.'
';
  }
  elseif (preg_match("`\[Obst`",$ligne_brut))
  {
    $flag_atcd=''; //On arrete la MAJ des ATCD et pas besoin de flag car le contenu deja mis a jout par les lignes Grossesse et allaitement
    $update=$update.$ligne_brut.'
'; //le titre
  }
  elseif (preg_match("`\[VAR`",$ligne_brut)) //On analyse les tableaux variables, numerotes par $num_terrain
  {
    $var='1';
    if (isset($inserer[$num_terrain])) 
      $update=$update.$chaine[$num_terrain].'
'; //On ajoute une ligne de date - La variable $chaine represente une date pour une variable
    $num_terrain++;
    $update=$update.$ligne_brut.'
'; //la ligne de titre d'un tableau
  }
  elseif (preg_match("`\[Propr`",$ligne_brut)) //insertion du dernier tableau avant Proprietes
  {
    $var=''; //On quitte le mode tableaux
    $effacer_prop='1'; //On entre en mode proprietes
    if ($inserer[$num_terrain])
      $update=$update.$chaine[$num_terrain].'
'; //On insere le tableau 
    if ($select_ordo == 'X') //on remplace l'ancienne ordo par une nouvelle
    {
      $flag_ancienne_ordo='1';
    }
    $num_terrain++;
    $update=$update.$ligne_brut; //On insere le titre Propriétés Ordo
//la ligne de titre d'un tableau
  }
  elseif (preg_match("`\[Traitement`",$ligne_brut)) //insertion du dernier tableau avant Proprietes
  {
    $effacer_prop='';
    $effacer_ordo='1';
    $ligne_brut=trim($ligne_brut); //La chaine [Traitement]
    $update.='
'.$chaine_proprietes.$ligne_brut;
    if ($select_ordo != 'X') //on remplace l'ancienne ordo par une nouvelle
    {
      $update.=$ordo.'
';
    } //On ajoute la ligne [Traitement] et les proprietes juste avant
  }
  elseif (preg_match("`\[Ant`",$ligne_brut))
  {
//    $select_ordo='';
//    $flag_ancienne_ordo='';
    $flag_atcd='1';
    $count_atcd=0;
    $update=$update.$ligne_brut.'
'; //le titre
    if ($text_atcd_libre) //mettre en debut de chaine un eventuel nouvel atcd libre 
    {
      $update=$update.$text_atcd_full.'
    ';
    }
    if ($select_intolerance)//on ajoute la nouvelle intolerance si besoin
    {
      $select_intolerance_active=$_REQUEST['select_intolerance_active'];
      $update=$update.$intolerance_full.'
'; //On ajoute la nouvelle intolerance medicamenteuse en debut de liste
    }
    if ($select_atcd)//on ajoute la nouvelle intolerance si besoin
    {

//on recherche le code CIM-X
      $sql_chercher_cim10="SELECT f3 FROM t4N WHERE CAST(f2 AS CHAR) LIKE '".mysqli_real_escape_string($db,$select_atcd)."'";
      $resultat_chercher_cim10=mysqli_query($db,$sql_chercher_cim10);
      $ligne_chercher_cim10=mysqli_fetch_array($resultat_chercher_cim10);
      $update=$update.$atcd_full.'
'; //On ajoute le nouvel antecedent en debut de liste
    }

  }
  elseif (preg_match("`Modèle =`",$ligne_brut))
  {
    $update=$update.$ligne_brut.'
'; //les colonnes des tableaux variables
  }
  else
  {
    if ($effacer_prop!='1' AND $chaine_proprietes AND $effacer_ordo!='1' AND $ligne_brut!='' AND $flag_atcd!='1' AND $var!='1')  //on n'inscrit pas le terrain existant si nouveau terrain //Pour grossesse et allaitement
    {
      $update.=$ligne_brut.'
';
    }
    if ($flag_ancienne_ordo)
    {
      $update.=$ligne_brut.'
';
    }
    if ($var=='1')
    {
      $update=$update.$ligne_brut.'
'; //Les lignes de modeles et les lignes de tableau deja existantes
    }
    if ($flag_atcd=='1') //On ajoute les antecedents
    {
      if ($ligne_brut) //eviter les lignes vides
      {
	if ($_REQUEST['check_atcd'.$count_atcd]=="on") //n'enregistrer que les lignes qui ont une coche et la ligne de titre
	{
	    $update=$update.$ligne_brut.'
	'; //Le titre antecedents, suit l'en-tête html
	}
	$count_atcd++;
      }
    }
  }
}
if (!mysqli_select_db($db,$base)){
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}
$sql_modifier_terrain=sprintf("UPDATE RubriquesBlobs SET RbDate_DataRub='%s' WHERE RbDate_PrimKey='$id_terrain'",mysqli_real_escape_string($db,$update));
//echo $sql_modifier_terrain;
$resultat_modifier_terrain=mysqli_query($db,$sql_modifier_terrain);
//on renvoie sur la page
header ('location:tableaux.php?GUID='.$GUID_patient.'&affichage=terrains');
exit;
?>
