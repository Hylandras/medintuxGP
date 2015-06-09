<?php
session_start() ;
/*
Page des ordonnances Vidal
Tables utilisees
t00 : tous les conditionnements bases sur CUV, CIP et CPG
t01 : prix
t02 : commercialisation, liaison CUV
t07 : substance par numero
t08 : composants par numeros
t10 : appariement 2 a 2 des sous-classes d'interactions
t11 : appariement CIP - composition (signification t27)
t24 : unites
t26 : description des classes
t27 : substances en clair
t37 : determination des sous-classes d'interactions (liaison t4B) plusieurs sous-classes pour une classe
t42 : les classes
t45 : adresse des documents PGR BUM ALD et autres
t49 : liaison CUV -> T45
t50 : glossaire d'etats pathologiques
t52 : interferences avec etats pathologiques
t54 : nom de substance (lie t07)
t55 : liaison t4N (cim-X) - t50 (pathologies vidal) et t52 (patho Vidal selon produit)
t56 : tableau des hypersensibilites
t57 : tableau des allergies croisees
t58 : code allergie - substance (f0 ->t56.f0, f1 ->t27.f0)
t59 : substance enfant et parent (uniformisation des substances)
t63 : precautions de prescription (site de prescription, quantite max delivree, liste stup...)
t1C : types de composition
t1G : interpretation des niveaux de gravite donnes dans t10
t2A : Age poids amenorrhee creatinine allaitement
t2G : Relations Mesures bio / Lexique
t2L : bornes posologies
t2P : posologies
t3G : pas compris
t4B : definition de medoc et classe d'interaction base sur Code Produit Global
t4N : CIM-X - liee t55
t5A : Codes de vigilance
t4S : simplification des pathologies par groupes
tFB : Version
*/

include("config.php");
$basemed="DatasempTest";

if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=interactions' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$signuser=$tab_login[1];

if (isset($drug_order[$signuser])) //si existe une variable pour trier les medicaments par prix
  $order=$drug_order[$signuser];
else
  $order='n';

include("inc/header.php");
//Connexion au serveur
if (!$db=mysqli_connect($host, $loginbase, $pwd))
{
  print "Erreur de connexion &agrave; ".$host."<br />";
  exit;
}
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
// on choisit la base vidal

if (!mysqli_select_db($db,$basemed))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}

$sql_chercher_version="SELECT * FROM tFB";
$resultat_chercher_version=mysqli_query($db,$sql_chercher_version);
while ($ligne_chercher_version=mysqli_fetch_array($resultat_chercher_version))
{
  if ($ligne_chercher_version['f0']=='EDITION NUMERO')
  {
    $VersionVidal=$ligne_chercher_version['f1'];
  }
  elseif ($ligne_chercher_version['f0']=='ARRETE PACK IR')
  {
    $dateIR=$ligne_chercher_version['f1'];
  }
}

function unified_component($component,$db) //pour trouver un numero unique pour les divers noms d'une substance
{
  $sql_unification_dci="SELECT f1 FROM t59 WHERE f0='$component'";
  $resultat_unification_dci=mysqli_query($db,$sql_unification_dci);
  $count_unification_dci=mysqli_num_rows($resultat_unification_dci);
  if ($count_unification_dci)
  {
    $ligne_unification_dci=mysqli_fetch_array($resultat_unification_dci);
    $component=$ligne_unification_dci['f1'];
  }
  return $component;
}

function find_document($cpg,$nature,$db)
{
//$nature=PGR ou BUM ou ALD
  $document_pgr='';
  $cle='DS_DOCS'.$nature.'%';
  $sql_pgr="SELECT t45.f5 AS document FROM `t45`,`t49` WHERE t49.f0='$cpg' AND t49.f1=t45.f0 AND t45.f5 LIKE '$cle'";
//echo $sql_pgr;
  $resultat_pgr=mysqli_query($db,$sql_pgr);
  while ($ligne_pgr=mysqli_fetch_array($resultat_pgr))
  {
    $document_pgr=$ligne_pgr['document'];
  }
//echo "doc $document_pgr";
  return($document_pgr);
}
$check_dci='';
if (isset($_REQUEST['check_dci']))
  $check_dci='1';

$cip_medoc=array();
$cpg_medoc=array();
$cuv_medoc=array();
$a='';
$radio_tri='n'; //si pas precise, tri alphabetique
if (isset($_REQUEST['radio_tri']))
  $radio_tri=$_REQUEST['radio_tri'];

//Generation d'une ordonnance structuree dans le cas de recuperation du bouton button_ordo
//Enregistrement de l'ordonnance, puis redirection sur la page de journee consultation.php
if (isset($_REQUEST['button_ordo']))
{
  $ordotext_avec_ald='';
  $ordotext_sans_ald='';
  $ordostruct='';
  $pendant='';
  $cip_medoc=$_REQUEST['cip_medoc']; 
  $i=0;
  $iald=0;
  $nom_medoc='';
  foreach ($cip_medoc AS $this_cip_medoc)
  {
//il faut enlever les _ pour enregistrer correctement les CIP
    $temp='name_medoc'.$this_cip_medoc;
    $nom_medoc_from_url=$_REQUEST[$temp];
    $temp='liste'.$this_cip_medoc;
    $liste=$_REQUEST[$temp];
    $temp='value_min'.$this_cip_medoc;
    $value_min=$_REQUEST[$temp];
    $temp='unit_min'.$this_cip_medoc;
    $unit_min=$_REQUEST[$temp];
    $temp='period_min'.$this_cip_medoc;
    $period_min=$_REQUEST[$temp];
    $temp='value_max'.$this_cip_medoc;
    $value_max=$_REQUEST[$temp];
    $temp='unit_max'.$this_cip_medoc;
    $unit_max=$_REQUEST[$temp] ;
    $temp='period_max'.$this_cip_medoc;
    $period_max=$_REQUEST[$temp];
    $temp='text_duree'.$this_cip_medoc;
    $text_duree=$_REQUEST[$temp];
    $select_duree='';
    $pendant='';
    if($text_duree)
    {
      $pendant='&nbsp;pendant&nbsp;';
      $temp='select_duree'.$this_cip_medoc;
      $select_duree=$_REQUEST[$temp];
    }
    $a='';
    $temp='a'.$this_cip_medoc;
    if (isset($_REQUEST[$temp]))
      $a=$_REQUEST[$temp];
    $note='';
    $temp='note'.$this_cip_medoc;
    if ($_REQUEST[$temp])
      $note='<br />'.$_REQUEST[$temp];
    $numeroID=$_REQUEST['numeroID'];
    $date_complete=date('Y-m-d h:i:s');

    if ($liste=='3') //passer les stups en toutes lettres
    {
      $formatter = new NumberFormatter('fr_FR', NumberFormatter::SPELLOUT);
      $text_duree= $formatter->format($text_duree);
      $value_min= $formatter->format($value_min);
      $value_max= $formatter->format($value_max);
    }
    $max='';
    if ($a)
    {
      $max=" à ".$value_max." ".$unit_max." ".$period_max." ";
    }
    $ns='';//ajouter quelque chose en debut de ligne si non substituable
    $temp='check_ns'.$this_cip_medoc;
    if (isset($_REQUEST[$temp]))
    {
      $ns='&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
    }

    $temp='check_ald'.$this_cip_medoc;
    if (isset($_REQUEST[$temp]))
    {
      $iald++;
      $ordotext_avec_ald.='<p>'.$ns.'<span style="font-family:Arial;color:#0000ff"><b>'.$iald.')&nbsp;<u>'.$nom_medoc_from_url.'</u></b>&nbsp;&nbsp;&nbsp;<i><br />'.$value_min.' '.$unit_min.' '.$period_min.' '. $max.$pendant.' '.$text_duree." ".$select_duree.$note.'</i></span><br /></p>';
      $ald="1";
    }
    else
    {
      $i++;
      $ald="0";
      $ordotext_sans_ald.='<p>'.$ns.'<span style="font-family:Arial;color:#0000ff"><b>'.$i.')&nbsp;<u>'.$nom_medoc_from_url.'</u></b>&nbsp;&nbsp;&nbsp;<i><br />'.$value_min.' '.$unit_min.' '.$period_min.' '. $max.$pendant.' '.$text_duree." ".$select_duree.$note.'</i></span><br /></p>';
    }
    $list_period_min['jours']=1;
    $list_period_min['semaines']=7;
    $list_period_min['mois']=30;

    $periode_seconds=$value_min*$list_period_min[$period_min];
/*
numVersion 
pk correspondant a table Posologies Medicatux
extraPK correspondant a table Posologies perso Medicatux
libelle Libelle poso perso
sexe d'application de la poso
terrain = vient de la fiche medoc
$AG:1:180:mois$PS:0:0:Kg medica_posologie_terrain_to_html dans cmedicabase.cpp ligne 1937
P_
*/
    $tab=explode('_',$this_cip_medoc);
    $ordostruct.='<Posologie>
<numVersion>1</numVersion>
<cip>'.$tab[0].'</cip>
<pk>4262</pk>
<libelle></libelle>
<sexe></sexe>
<terrain>$AG:1:180:mois$PS:0:0:Kg</terrain>
<doseMin>'.$value_min.'</doseMin>
<doseMax>'.$value_max.'</doseMax>
<doseUnit>48</doseUnit>
<doseLimitMax>0</doseLimitMax>
<factCorpQU>1</factCorpQU>
<factCorpUnit>6</factCorpUnit>
<periode>'.$periode_seconds.'</periode>
<equiCoeff>40</equiCoeff>
<unitCoeff>2</unitCoeff>
<nbPrises>2</nbPrises>
<schemaPrise>MA;SO</schemaPrise>
<divers>No E: 0</divers>
<extraPk>0</extraPk>
<note>'.$note.'</note>
<posoAdjust>50</posoAdjust>
<secabilite></secabilite>
<numOrdre>'.$i.'</numOrdre>
<posoTextuelle></posoTextuelle>
<ald>'.$ald.'</ald>
<posoType>2</posoType>
<duree>691200</duree>
<IdTable>1</IdTable>
</Posologie>
';
/* Nouvelle structure
<OrdoLine>
<gph>
 <smr>4</smr>
 <smr_l>SMR important</smr_l>
 <gph_html><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
<html><head><meta name="qrichtext" content="1" /><style type="text/css">
p, li { white-space: pre-wrap; }
</style></head><body style=" font-family:'Lucida Grande'; font-weight:400; font-style:normal;">
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'arial,verdana,sans-serif'; font-weight:600; color:#ff0000;">1)</span> <span style=" font-family:'arial,verdana,sans-serif'; font-weight:600; text-decoration: underline; color:#637867;">ALLOPURINOL  200 milligrammes</span> <span style=" font-family:'arial,verdana,sans-serif'; font-style:italic; color:#a6c9ad;">(ZYLORIC 200mg Cpr B/28)</span> <span style=" font-family:'arial,verdana,sans-serif'; font-weight:600; color:#637867;">comprimé par voie orale<br /></span> <span style=" font-family:'arial,verdana,sans-serif'; color:#637867;">1 comprimé au coucher,  traitement à poursuivre pendant 1 mois</span><br /></p></body></html></gph_html>
 <gph_html></gph_html>
 <gph_ald></gph_ald>
 <gph_pk></gph_pk>
 <gph_id>3272727</gph_id>
 <gph_it>CIP</gph_it>
 <gph_dr>27-09-2013 14:54:39</gph_dr>
 <gph_dt>27-09-2013 14:54:39</gph_dt>
 <gph_df>27-10-2013 14:54:39</gph_df>
 <gph_na>ZYLORIC 200mg Cpr B/28</gph_na>
 <gph_dcl>
  <gph_dc>
    <gph_dcn>ALLOPURINOL</gph_dcn>  <gph_dcp>200</gph_dcp>  <gph_dcu>milligramme</gph_dcu>
  </gph_dc>
 </gph_dcl>
 <gph_cy>M1R0</gph_cy>
 <gph_sb>1</gph_sb>
 <gph_dci>2</gph_dci>
 <gph_uf>comprimé</gph_uf>
 <gph_voie>orale</gph_voie>
 <gph_fmin></gph_fmin>
 <gph_fmax></gph_fmax>
 <gph_funit></gph_funit>
 <gph_pmin></gph_pmin>
 <gph_pmax></gph_pmax>
 <gph_punit>milligramme</gph_punit>
 <gph_pfc></gph_pfc>
 <gph_pfcunit>Kg</gph_pfcunit>
 <gph_pqbyuf>200</gph_pqbyuf>
 <gph_dmin></gph_dmin>
 <gph_dmax></gph_dmax>
 <gph_dunit></gph_dunit>
 <gph_nm>0</gph_nm>
 <gph_cm>NOT_LITERAL</gph_cm>
 <gph_in>  <gph_il>4|SMR|· Traitement des hyperuricémies symptomatiques primitives ou secondaires (hémopathies, néphropathies, hyperuricémie iatrogène),· Traitement de la goutte : goutte tophacée, crise de goutte récidivante, arthropathie uratique même lorsqu'elle s'accompagne d'hyperuraturie, de lithiase urique ou d'insuffisance rénale,· Traitement des hyperuricuries et hyperuraturies,· Traitement et prévention de la lithiase urique,· Prévention des récidives de lithiase calcique chez les patients hyperuricémiques ou hyperuricuriques, en complément des précautions diététiques habituelles, portant notamment sur les rations protidique et calcique.</gph_il>
</gph_in>
 <gph_co></gph_co>
</gph>
<gps>
 <gpp>comprimé[1;c]</gpp>
</gps>
<gpi_engine>Datasemp Version : 200A  date d'extraction : 05/08/2013</gpi_engine>
<gpi_user>
    <m_Login>admin</m_Login>
    <m_usual_name>Sevin</m_usual_name>
    <m_forename>Roland</m_forename>
    <m_Nu_RPPS>222222222222222</m_Nu_RPPS>
    <m_Nu_ClefRPPS>35</m_Nu_ClefRPPS>
</gpi_user>
</OrdoLine>
*/
/* 	Ancienne structure 
<?xml version="1.0" encoding="ISO-8859-1" standalone="yes" ?>\r\n

<ordotext>\r\n
<html><head><meta name="qrichtext" content="charset=utf-8" /><title>Prescription</title></head><body style="font-size:10pt;font-family:Arial Narrow"><center><span style="font-family:Arial Narrow;font-size:10pt;color:#000000">
<p><hr size="2" />Prescriptions relatives au traitement de l'affection de longue durée reconnue (liste ou hors liste)<br>(AFFECTION EXONÉRANTE)<p><hr size="2" /><p></span></center><br><span style="font-family:Arial;color:#0000ff"><b>1)&nbsp;<u>ABBE CHAUPITRE 01 S buv rhume Fl/20ml</u></b>&nbsp;&nbsp;&nbsp;<i><br>10&nbsp;gouttes&nbsp; par jour&nbsp;pendant&nbsp; 8 jours </i></span><br><center><span style="font-family:Arial Narrow;font-size:10pt;color:#000000"><p>
<hr size="2" />
Prescriptions SANS RAPPORT avec l'affection de longue durée<br>(MALADIES INTERCURRENTES)<p>
<hr size="2" />
<p></span></center><br><span style="font-family:Arial;color:#0000ff"><b>2)&nbsp;<u>BACTRIM 200mg/40mg Susp buv tutti frutti pédi</u></b>&nbsp;&nbsp;&nbsp;<i><br>0.075&nbsp;cuillère-mesure&nbsp; le matin, et le soir&nbsp;pendant&nbsp; 8 jours </i></span><br></body>
</html>\r\n</ordotext>\r\n\r\n

<ordoMedicaStruct>\r\n<PosologieList>\r\n
<Posologie>\r\n<numVersion>1</numVersion>\r\n<cip>3130536</cip>\r\n<pk>4262</pk>\r\n<libelle></libelle>\r\n<sexe></sexe>\r\n<terrain>$AG:1:180:mois$PS:0:0:Kg</terrain>\r\n<doseMin>6</doseMin>\r\n<doseMax>0</doseMax>\r\n<doseUnit>48</doseUnit>\r\n<doseLimitMax>0</doseLimitMax>\r\n<factCorpQU>1</factCorpQU>\r\n<factCorpUnit>6</factCorpUnit>\r\n<periode>86400</periode>\r\n<equiCoeff>40</equiCoeff>\r\n<unitCoeff>2</unitCoeff>\r\n<nbPrises>2</nbPrises>\r\n<schemaPrise>MA;SO</schemaPrise>\r\n<divers>No E: 0</divers>\r\n<extraPk>0</extraPk>\r\n<note></note>\r\n<posoAdjust>50</posoAdjust>\r\n<secabilite>1</secabilite>\r\n<numOrdre>1</numOrdre>\r\n<posoTextuelle></posoTextuelle>\r\n<ald>0</ald>\r\n<posoType>2</posoType>\r\n<duree>691200</duree>\r\n<IdTable>1</IdTable>\r\n</Posologie>\r\n
<Posologie>\r\n<numVersion>1</numVersion>\r\n<cip>3307851</cip>\r\n<pk>4</pk>\r\n<libelle>Draineur général. Rhume.</libelle>\r\n<sexe>I</sexe>\r\n<terrain>$AG:0:0:mois$PS:0:0:Kg</terrain>\r\n<doseMin>15</doseMin>\r\n<doseMax>6</doseMax>\r\n<doseUnit>4</doseUnit>\r\n<doseLimitMax>0</doseLimitMax>\r\n<factCorpQU>0</factCorpQU>\r\n<factCorpUnit>6</factCorpUnit>\r\n<periode>86400</periode>\r\n<equiCoeff>0</equiCoeff>\r\n<unitCoeff>28</unitCoeff>\r\n<nbPrises>1</nbPrises>\r\n<schemaPrise></schemaPrise>\r\n<divers></divers>\r\n<extraPk>15</extraPk>\r\n<note></note>\r\n<posoAdjust>50</posoAdjust>\r\n<secabilite>1</secabilite>\r\n<numOrdre>1</numOrdre>\r\n<posoTextuelle></posoTextuelle>\r\n<ald>1</ald>\r\n<posoType>2</posoType>\r\n<duree>691200</duree>\r\n<IdTable>1</IdTable>\r\n</Posologie>\r\n
</PosologieList>\r\n</ordoMedicaStruct>\r\n\0
*/

  } //fin du foreach
//Synthese des lignes d'ordonnances + des lignes de structure + mise en forme XML
  $avec_ald=''; //chaine destinee a ecrire le separateur haut d'ALD
  $sans_ald='';
  if ($ordotext_avec_ald) //seulement si existe ALD
  {
    $avec_ald='<hr /><p style="text-align:center;" >Prescriptions relatives au traitement de l\'affection de longue durée reconnue (liste ou hors liste)<br />(AFFECTION EXONÉRANTE)</p><p><hr /></p>';
    $sans_ald='<hr /><p style="text-align:center;" >Prescriptions SANS RAPPORT avec l\'affection de longue durée<br />(MALADIES INTERCURRENTES)</p><p><hr /></p>';
  }
  if (isset($_REQUEST['note_intolerances']))
    $note_intolerances=$_REQUEST['note_intolerances'];

  $blob='<?xml version="1.0" encoding="ISO-8859-1" standalone="yes" ?>
<ordotext>
<html><head><meta name="qrichtext" content="charset=utf-8" /><title>Prescription</title></head><body style="font-size:10pt;font-family:Arial Narrow">'.$avec_ald.$ordotext_avec_ald.$sans_ald.$ordotext_sans_ald.$note_intolerances.'</body></html>
</ordotext>
<ordoMedicaStruct>
<PosologieList>
<VersionVidal>
'.$VersionVidal.'
</VersionVidal>
'.$ordostruct.'
</PosologieList>
</ordoMedicaStruct>
';
  mysqli_select_db($db,$base);
//mettre un update ici si on vient de modification d'ordo
  $sql_insert_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$numeroID')",mysqli_real_escape_string($db,$blob));
  $resultat_insert_blob=mysqli_query($db,$sql_insert_blob);
	  
  //on recupere l'ID du blob
  $id_blob=mysqli_insert_id($db);
  //on insere l'en-tete
  $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$numeroID','20020100','%s','$date_complete','$user','$signuser','$id_blob')",mysqli_real_escape_string($db,'Ordonnance structurée'));
  $resultat_insert_header=mysqli_query($db,$sql_insert_header);

  header ('location:consultation.php?numeroID='.$numeroID.'&date='.date('Y-m-d'));
} //fin de recuperation de button_ordo de validation de l'ordonnance

//On cherche les terrains pour poids, taille creatinine grossesse allaitement
mysqli_select_db($db,$base);
$tableau_patho_vidal=array();
$tableau_atcd_libres=array();
$intolerances_text=array();
$intolerances_code=array();

  $cip_fond=array();
  $age_jours ='';
   $sexe_patient ='';
  $note_intolerances  ='';
//On cherche uniquement le dernier terrain pour ce patient
if (isset($_REQUEST['numeroID']))
{
  $numeroID=$_REQUEST['numeroID'];

  $sql_terrain="SELECT RbDate_DataRub FROM RubriquesHead INNER JOIN RubriquesBlobs ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesHead.RbDate_TypeRub LIKE '20060000' AND RubriquesHead.RbDate_IDDOs='$numeroID' ORDER BY RbDate_Date DESC LIMIT 1" ;
  $resultat_terrain=mysqli_query($db,$sql_terrain);
  $count_terrain=mysqli_num_rows($resultat_terrain);

  $ligne_terrain=mysqli_fetch_array($resultat_terrain);

  $split_terrain=explode("\n", $ligne_terrain['RbDate_DataRub']);
  $grossesse='';
  $allaitement='';
  $antecedents='';
  $flag_actd=0;
  $cim10=array();
  $cim10_text=array();
  $poids='inconnu';
  $taille='inconnue';
  $creat='inconnue';
  $j=0;
  $analyse=0;
  $traitement_de_fond=0;
  $exp_date="`([0-9]{4}-[0-9]{2}-[0-9]{2})`";//rechercher si ligne contient une date
  $expr="`(<cip>)(.*)(<\/cip>)`i"; //rechercher les CIP du traitement de fond pour les interactions
  $surface=0;

  mysqli_select_db($db,$basemed);

  foreach ($split_terrain AS $ligne_brut)
  {
if ($ligne_brut)
{
    if (preg_match("`(\[Ant)`",$ligne_brut)) //On cherche les antecedents pathologiques et les intolerances medicamenteuses
    {
      $flag_actd=1;
    }
    elseif (preg_match("`(\[Obst)`",$ligne_brut))
  {
  //echo 'test';
      $flag_actd=0; //On remet a vide le marqueur d'atcd
  }
    elseif (preg_match("`Grossesse`",$ligne_brut))
    {
      if (strpos($ligne_brut,"non"))
	$grossesse='non';
      else
	$grossesse='oui';
      $analyse=0; // on remet le switch a 0

    }
    elseif (preg_match("`Allaitement`",$ligne_brut))
    {
      if (strpos($ligne_brut,"oui"))
	$allaitement='oui';
      else
	$allaitement='non';
	$analyse=0; // on remet le switch a 0
    }
    elseif (preg_match("`(\[VAR )`",$ligne_brut))
    {
      if (strpos($ligne_brut,"VAR Poids"))//On cherche le paragraphe nomme poids
      {
	$analyse=1; //On positionne le switch a 1 si on trouve Var Poids
      }
      else
      {
	$analyse=0; //Pour les autres VAR, on remet le switch a 0
      }
    }

    if ($analyse==1) // On n'analyse que le paragraphe nomme Poids
    {
      if (preg_match ("`Modèle`i",$ligne_brut))
      {
	if (preg_match("`Poids`",$ligne_brut))
	{
	  $pos_poids=$j;
	}
	if (preg_match("`Taille`",$ligne_brut))
	{
	  $pos_taille=$j;
	}
	if (preg_match("`Créatinine`",$ligne_brut))
	{
	  $pos_creat=$j;
	}
	else
	  $pos_creat='';
	$j++;
      }
      elseif (preg_match($exp_date,$ligne_brut)) //On est en mode analyse d'une date, ni sur une ligne var ni sur une ligne modele et dans le bon paragraphe
      {
	$valeurs=explode("=",$ligne_brut);
	$tableau_valeurs=explode(",",$valeurs[1]);

	if (strlen($tableau_valeurs[$pos_poids])>2) //On n'ecrase pas la variable si pas de resultat La chaine vide contient 2 espaces
	{
	    $poids=trim($tableau_valeurs[$pos_poids]); //On ecrase chaque ancienne valeur par nouvelle pour n'avoir que la dernière.
	}
	if (strlen($tableau_valeurs[$pos_taille])>2)
	{
	    $taille=trim($tableau_valeurs[$pos_taille]);
	}
  //calcul de la surface corporelle si poids et taille remplis
	if (is_numeric($poids) AND is_numeric($taille))
	{
	//Formule de Mosteller (1987)
	//    Surface corporelle (m²) =  [Taille(cm) x Poids(kg) /3600]^0,5 
  //(int)($nb_full*100)/100 pour couper après 2 décimales
	  $surface=(int)(sqrt($taille*$poids/3600)*100)/100;
	}
	if ($pos_creat)
	{
	  if (strlen($tableau_valeurs[$pos_creat])>2)
	  {
	      $creat=$tableau_valeurs[$pos_creat];
	  }
	}
      }
    }
    elseif (preg_match("`(\[Traitement)`",$ligne_brut)) //On trouve les CIP du traitement de fond
    {
      $traitement_de_fond=1;
    }
    if ($traitement_de_fond==1)
    {
      if(preg_match($expr,$ligne_brut,$tab))
      {
	$cip_fond[]=$tab[2];
      }
    }
    if ($flag_actd AND !preg_match("`(\[Ant)`",$ligne_brut)) //on analyse les premieres lignes contenant les antecedents libres, medicamenteux et pathologiques, mais pas le titre de rubriqueni les lignes vides
    {
      $values_line=explode('=',$ligne_brut);
      if (isset($values_line[1]))
	$data_array=explode(',',$values_line[1]);

      if (preg_match("`-(.*)-`",$data_array[2])) //les intolerances en cours
      {
  // O famille (genre)
  // 1 active
  // 2 code cim10
  // 3 commentaire
  // 4 date
  // 5 ALD
	$intolerances_text[]=$values_line[0]; //les intolerances sont entourees de tirets dans le terrain
	$intolerances_temp=str_replace('-','',$data_array[2]); //on enleve les tirets du code de substance
	$intolerances_code[]=str_replace(' ','',$intolerances_temp); //on enleve les espaces du code de substance
      }
      elseif (preg_match ("`~(.*)~`",$data_array[2])) //les pathololgies cim10
  //chercher les CIM-X

  // 2 code substance
  // 3 commentaire
  // 4 date
  // 5 ALD
      {
	$cim10_text[]=$values_line[0];

	$this_cim10=$cim10_code[]=$data_array[2];
	$this_cim10=str_replace('~','',$this_cim10); //enlever les ~
	$this_cim10=str_replace(' ','',$this_cim10); //enlever les espaces
  //chercher les pathologies vidal liees a ce cim10
	$sql_chercher_code_vidal_patho="SELECT t55.f0 AS code_vidal_patho FROM t55 INNER JOIN t4N ON t55.f1=t4N.f0 WHERE t4N.f3='$this_cim10'";
	$resultat_chercher_code_vidal_patho=mysqli_query($db,$sql_chercher_code_vidal_patho);
	while ($ligne_chercher_code_vidal_patho=mysqli_fetch_array($resultat_chercher_code_vidal_patho))
	{
	//on cree un tableau de valeurs uniques avec toutes les pathos vidal du patient deduites de ses cim10
	  if (!in_array($ligne_chercher_code_vidal_patho['code_vidal_patho'],$tableau_patho_vidal))
	  {
	    $tableau_patho_vidal[]= $ligne_chercher_code_vidal_patho['code_vidal_patho'];
	  }
	}
      }
      else //les antecedents libres - valeur [2] vide
      {
	if ($values_line[0]) //eliminer les lignes vides
	{
	  $tableau_atcd_libres[]=$values_line[0];
	}
      }
    }
  }
}
}

function detectUTF8($string) //si encodage est toujours imprévisible
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

?>
    <title>
      MedWebTux - M&eacute;dicaments - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>

  <script type="text/javascript">
//<![CDATA[
function invers_ald()//pour inverser les coches d'ALD
{
  var array = document.getElementsByTagName("input");

  for(var ii = 0; ii < array.length; ii++)
  {
    if(array[ii].className == 'check_ald') //On inverse toutes les coches de la classe check_ald
    {
      if (array[ii].checked == true)
	array[ii].checked = false;
      else
	array[ii].checked = true;
    }
  }
}
//]]>
  </script>
<!-- Recherche AJAX de medicament -->
    <script type="text/javascript">
//<![CDATA[
function request(callback) {
//AJAX pour chercher les intolerances medicamenteuses

//arreter les requetes non terminees
  $.xhrPool = [];
  $.xhrPool.abortAll = function() {
      $(this).each(function(idx, jqXHR) {
	  jqXHR.abort();
      });
      $.xhrPool = [];
  };

  $.ajaxSetup({
      beforeSend: function(jqXHR) {
	  $.xhrPool.push(jqXHR);
      },
      complete: function(jqXHR) {
	  var index = $.xhrPool.indexOf(jqXHR);
	  if (index > -1) {
	      $.xhrPool.splice(index, 1);
	  }
      }
  });
//lancer la nouvelle requete
	var xhr = getXMLHttpRequest();
	
	xhr.onreadystatechange = function() {
		if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 0)) {
			callback(xhr.responseText);
		}
	};
	var nom_medoc = encodeURIComponent(document.getElementById("nom_medoc").value);
	var type_requete=document.getElementById("select_type_requete").value; //recherche par nom, DCI, indication ou atc
	var radio_distri_array=document.getElementsByName("radio_distri"); //hopitaux ou officines
	var radio_distri;
	if (radio_distri_array[0].checked)
	  radio_distri=radio_distri_array[0].value;
	else if (radio_distri_array[1].checked)
	  radio_distri=radio_distri_array[1].value;

	var tri_par_array=document.getElementsByName("radio_tri"); //tri par nom ou prix
	var tri_par;
	if (tri_par_array[0].checked)
	  tri_par=tri_par_array[0].value;
	else if (tri_par_array[1].checked)
	  tri_par=tri_par_array[1].value;

	var select_type=document.getElementById("select_type").value; //medicaments, accessoires, dietetique...

//on envoie a la page de recherche toutes les valeurs
	xhr.open("GET", "recherche_drug.php?nom_medoc=" + nom_medoc+"&type_requete="+type_requete+"&radio_distri="+radio_distri+"&tri_par="+tri_par+"&select_type="+select_type, true);
	xhr.send(null);

}

function readData(sData) 
{
   //effacer l'ancien menu deroulant
  document.forms['form_global'].select_medoc.length=0;
  if (sData.indexOf("|") !== -1) //pas d'affichage si pas de reponse renvoyee par ajax
  {
    substances=sData.split('|'); 
    for (var i in substances) //creation d'une option de menu pour toutes les substances
    {
      var detail=substances[i].split('_');
      if (detail!='') //supprimer les entrees vides
	document.forms['form_global'].select_medoc.options[document.forms['form_global'].select_medoc.options.length] = new Option(detail[0],detail[1]); 
    }
  }
}
//]]>
    </script>
  <script type="text/javascript">
//<![CDATA[
function display_document(document)
// on ouvre dans une fenêtre le fichier passé en paramètre.
{ 
  window.open(document,'width=800,height=550,top=50,left=50,toolbar=no,scrollbars=yes,resizable=yes,location=no'); 
}
//]]>
  </script>


  <script type="text/javascript">
//<![CDATA[
function global_duration()//pour donner la même duree a toutes les prescriptions
{
  var array = document.getElementsByTagName("input");
  var text_duree=document.getElementById("text_duree").value ;

  for(var ii = 0; ii < array.length; ii++)
  {
    if(array[ii].className == 'text_duree') //On change la valeur de la classe text_duree
    {
      array[ii].value = text_duree;
    }
  }

  var array = document.getElementsByTagName("select");
  var select_duree=document.getElementById("select_duree").value ;

  for(var ii = 0; ii < array.length; ii++)
  {
    if(array[ii].className == 'select_duree') //On change la valeur de la classe select_duree
    {
	array[ii].value = select_duree ;
    }
  }
}
//]]>
  </script>


     <script type="text/javascript">
//<![CDATA[
function calcul_total()
{
  total=0;
  var elementsTable = new Array();  
  for(var i=0 ; i<document.getElementsByTagName("input").length ; i++) 
  {  
    if(document.getElementsByTagName("input")[i].id && document.getElementsByTagName("input")[i].id.match("prix_ligne")) 
    { 
      total+=Number(document.getElementsByTagName("input")[i].value);  
    }  
  }  
  document.getElementById('total').innerHTML =total;
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
//]]>
    </script>
    <script type="text/javascript">
//<![CDATA[
function verif_poso(value,cip)
{
//value = nombre de jours, semaines ou mois, qu'on multipliera selon l'unite pour avoir le nombre de jours
  unite=eval("document.forms['form_global'].select_duree"+cip+".value");
  if (unite=='semaines')
  {
    value=value*7;
  }
  else if (unite=='mois')
  {
    value=value*30;
  }
  code_unite_duree_min=eval("document.forms['form_global'].code_unite_duree_min"+cip+".value");
  code_unite_duree_max=eval("document.forms['form_global'].code_unite_duree_max"+cip+".value");
  duree_min=eval("document.forms['form_global'].duree_min"+cip+".value");
  duree_max=eval("document.forms['form_global'].duree_max"+cip+".value");
  if (code_unite_duree_min=="42") //On convertit les durees en jours
  {
    duree_min=duree_min*7;
  }
  else if (code_unite_duree_min=="43")
  {
    duree_min=duree_min*30;
  }
  else if (code_unite_duree_min=="44")
  {
    duree_min=duree_min*365;
  }
  if (code_unite_duree_max=="42")
    duree_max=duree_max*7;
  else if (code_unite_duree_max=="43")
    duree_max=duree_max*30;
  else if (code_unite_duree_max=="44")
    duree_max=duree_max*365;

  if (parseInt(value)<parseInt(duree_min))
    alert ('Hors limites : minimum '+duree_min+' jours');
  else if (parseInt(value) > parseInt(duree_max))
    alert ('Hors limites : maximum '+duree_max+' jours');
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function strpos (haystack, needle, offset) {
//inutilisee - prevue pour calcul_ligne - comment ca marche ?
  // http://kevin.vanzonneveld.net
  // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   improved by: Onno Marsman
  // +   bugfixed by: Daniel Esteban
  // +   improved by: Brett Zamir (http://brett-zamir.me)
  // *     example 1: strpos('Kevin van Zonneveld', 'e', 5);
  // *     returns 1: 14
//alert (needle);
  var i = (haystack + '').indexOf(needle, (offset || 0));
  return i === -1 ? false : i;
}

function calcul_ligne(cip)
{
  var prix_boite=0;
  var nombre_boite=0;
  var text_duree=0;
  var value_min=0; //nombre d'unites de prise
  var value_max=0;
  var temp=0;
  var select_duree=''; //le deroulant mois/semaine/jour
  var periode=''; //type de prise par jour, mois...
  var diviseur=1;
  prix_boite=document.forms['form_global'].elements["prix_boite"+cip].value;
  text_duree=document.forms['form_global'].elements["text_duree"+cip].value;
  document.forms["form_global"].elements["prix_ligne"+cip].value=prix_boite;//pas de calcul compliqué si pas de duree = on met le prix d'une boite
  if (text_duree) 
  {
  //le multiplicateur de periode de temps
    select_duree=document.forms['form_global'].elements["select_duree"+cip].value;
    if (select_duree=='semaines')
      periode=7;
    else if (select_duree=='mois')
      periode=30;
    else if (select_duree=='semaines')
      periode=7;
    else if (select_duree=='an')
      periode=365;
    else
      periode=1;
  //on prend value_min, sauf si coche "a" cochee ->value_max
    if (document.forms['form_global'].elements["a"+cip].checked)
    {
      dose=document.forms['form_global'].elements["value_max"+cip].value;
      frequence=document.forms['form_global'].elements["period_max"+cip].value;
    }
    else
    {
      dose=document.forms['form_global'].elements["value_min"+cip].value;
      frequence=document.forms['form_global'].elements["period_min"+cip].value;
    }
    if (frequence.search("par 24 heures") != -1)
      diviseur=1;
    else if (frequence.search("par jour") != -1)
      diviseur=1;
    else if (frequence=="par 48 heures")
      diviseur=2;
    else if (frequence=="par semaine")
      diviseur=7;
    else if (frequence=="par mois")
      diviseur=30;
    else if (frequence=="par an")
      diviseur=365;
    else
      diviseur=0;
    if (diviseur) //seulement les frequences de prise exploitables
    {
      nombre_boite=document.forms['form_global'].elements["nombre_boite"+cip].value;
      if (nombre_boite) //ne pas calculer si le contenu de la boite n'est pas defini
      {
	document.forms["form_global"].elements["prix_ligne"+cip].value=      Math.ceil(dose*text_duree*periode/nombre_boite/diviseur)*prix_boite;
	document.forms["form_global"].elements["nombre_de_boites"+cip].value=Math.ceil(dose*text_duree*periode/nombre_boite/diviseur);
      }
      else //contenu de la boite inconnu
	document.forms["form_global"].elements["nombre_de_boites"+cip].value=1;
    }
    else //pas de frequence de prise valable
      document.forms["form_global"].elements["nombre_de_boites"+cip].value=1;
  }
  else //pas de duree
    document.forms["form_global"].elements["nombre_de_boites"+cip].value=1;
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function calcul_ligne_from_boite(cip)
{
  var nombre_boites=document.forms['form_global'].elements["nombre_de_boites"+cip].value;
  var prix_boite=document.forms['form_global'].elements["prix_boite"+cip].value;
  document.forms["form_global"].elements["prix_ligne"+cip].value=nombre_boites*prix_boite;
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function singulier_pluriel_value_min(singulier,pluriel,cip,minmax)
{
  var value_min=0; //nombre d'unites de prise
  value_min=document.forms['form_global'].elements["value_"+minmax+cip].value;
  if (value_min <=1)
    forme=singulier;
  else
    forme=pluriel;
  document.forms["form_global"].elements["unit_"+minmax+cip].value=forme; 
}
//]]>
    </script>
<!-- fonction non utilisee pour griser automatiquement les cases de poso suivant "à" -->
    <script type="text/javascript">
//<![CDATA[
function readonly(chp)
{
//Pas utilise
  document.getElementById('champ').setAttribute('readOnly', 'readOnly');
  document.getElementById('champ').removeAttribute('readOnly');
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function verif_champ(champ)
{
  if (document.forms['form_global'].nom_medoc.value == "")
  { 
    alert("Le champ n'est pas rempli\nMettez le signe % si vous voulez vraiment afficher tout le fichier");
    return false;
  }
  return true;
}
//]]>
    </script>


    <script type="text/javascript" src="oXHR.js">
    </script>
  </head>
  <body style="font-size:<?php echo $fontsize ?>pt"  onload="donner_focus('nom_medoc')" >
    <div class="conteneur" >
<?php	
// insertion du menu d'en-tete	
$anchor='Interactions';
//include("inc/menu-horiz.php");

function stripAccents($string)
{
  return strtr($string,'àáâãäçèéêëìíîïñòóôõöùúûüýÿÀÁÂÃÄÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝ',
'aaaaaceeeeiiiinooooouuuuyyAAAAACEEEEIIIINOOOOOUUUUY');
}
//On recupere tous les medocs deja enregistres
$anciens_medocs="";
if (isset($_REQUEST['id_medoc']))
{
  $id_medoc=$_REQUEST['id_medoc'];
  foreach ($id_medoc AS $this_medoc)
  {
    $anciens_medocs=$anciens_medocs.'&amp;id_medoc[]='.$this_medoc;
  }
}
elseif (isset($_REQUEST['cuv_medoc']))
{
  $cuv_medoc=$_REQUEST['cuv_medoc'];
  foreach ($cuv_medoc AS $this_medoc)
  {
    $anciens_medocs=$anciens_medocs.'&amp;cuv_medoc[]='.$this_medoc;
  }
}
$display_contents='';
?>
    <div class="groupe" >
      <h1>
	Création d'ordonnances - Vidal Data Semp <?php echo $VersionVidal.' du '.$dateIR ?>
      </h1>
<div class="information">
Ce logiciel n'est pas agréé par la HAS. Il est actuellement en développement et peut contenir des erreurs ou des insuffisances. Son utilisation est réservée aux prescripteurs qualifiés, sous leur propre responsabilité.
</div>
<?php 

if (!isset($drug_order[$signuser]))
{
?>
<div class="notice">
Vous n'avez pas défini de variable personnalisée pour l'utilisateur <?php echo $signuser ?>. Le tri des médicaments se fera par nom et non par prix.<br />
Pour un classement par prix, veuillez modifier le fichier config.php selon les instructions qui s'y trouvent.
</div>
<?php
}
$numeroID=""; 

if (isset($_REQUEST['numeroID']))//le patient si fourni
{
  mysqli_select_db($db,$base);
  $numeroID=$_REQUEST['numeroID'];
  $sql_patient="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$numeroID'";
  $resultat_patient=mysqli_query($db,$sql_patient);
  $ligne_patient=mysqli_fetch_array($resultat_patient);
  $sql_age="SELECT FchPat_Nee,FchPat_Sexe FROM fchpat WHERE FchPat_GUID_Doss='$numeroID'";
  $resultat_age=mysqli_query($db,$sql_age);
  $ligne_age=mysqli_fetch_array($resultat_age);
  $sexe_patient=$ligne_age['FchPat_Sexe'];
  $date_naissance=substr($ligne_age['FchPat_Nee'],0,10);
  $date_jour=date('Y-m-d', date('U'));
  function datediff($date1, $date2)  
  {
    $s = strtotime($date2)-strtotime($date1);
    $d = intval($s/86400)+1;  
    return "$d";
  }
//calcul de l'age

  $day_age  = substr($date_jour,8,2) - substr($ligne_age["FchPat_Nee"],8,2);
  $mon_age  = substr($date_jour,5,2) - substr($ligne_age["FchPat_Nee"],5,2);
  $year_age = substr($date_jour,0,4) - substr($ligne_age["FchPat_Nee"],0,4);

  if ($day_age < 0)
  {
    $mon_age=$mon_age - 1;
  }
  if ($mon_age < 0)
  {
    $year_age = $year_age - 1;
    $mon_age = 12 + $mon_age;
  }

  $age_jours=datediff($date_naissance,$date_jour);

//  $caracteristiques_patient='';

}

mysqli_select_db($db,$basemed);
?>
      <form id="form_global" action="interactions.php" method="post" style="width=100%">
	<div>
<?php
if (isset($_REQUEST['numeroID']))
{
?>
	  <fieldset>
	    <legend>
	      Patient
	    </legend>
	    <input name="numeroID" type="hidden" id="numero_ID" value="<?php echo $numeroID ?>" />
	    <strong>Sexe</strong>&nbsp;: <?php echo $sexe_patient.' - <strong>Poids</strong>&nbsp;= '.$poids.' (kg), - <strong>Taille</strong>&nbsp;='.$taille.' (cm), - <strong>Créatininémie</strong>&nbsp;= '.$creat.' (mg/l) ';
  if ($surface)
    echo " - <strong>Surface corporelle (Mosteller)</strong>&nbsp;= ".$surface;
  else
    echo " - Surface corporelle inconnue - poids et taille nécessaires";

  if ($sexe_patient=='F')
    echo " - <strong>grossesse</strong>&nbsp;= ".$grossesse." - <strong> Allaitement</strong>&nbsp;= ".$allaitement;
  echo '<br />';
  $note_intolerances='';

  if (count($intolerances_text))
  {
    echo ' 
<strong> Intolérances médicamenteuses</strong>&nbsp;:
<ul>';
    foreach ($intolerances_text as $cette_allergie)
    {
      echo '<li>'.$cette_allergie.'</li>';
      $note_intolerances.=$cette_allergie.' ';
    }
    $note_intolerances='<p>Intolérances : '.$note_intolerances.'</p>'; //Pour inscription sur l'ordonnance
    reset ($intolerances_text);
    echo '</ul>';
  }
  $note_atcd='';
  if (count($cim10_text))
  {
    echo ' 
<strong> Antécédents pathologiques</strong>&nbsp;:
<ul>';
    foreach ($cim10_text as $this_atcd)
    {
      echo '<li>'.$this_atcd.'</li>';
    }
    reset ($cim10_text);
    echo '</ul>';
  }
  if (count ($tableau_atcd_libres))
  {
    echo '
  <strong> Antécédents libres, non exploités par le mécanisme de contrôle de prescription</strong>&nbsp;:
<ul>';
    foreach ($tableau_atcd_libres as $this_atcd_libre)
    {
      echo '<li>'.$this_atcd_libre.'</li>';
    }
echo '</ul>';
  }
?>
	  </fieldset>
<?php
}
else
{
?>
  <div class='notice'>
    Pas de patient fourni. Il ne sera pas possible d'établir une ordonnance
  </div>
<?php
}
$nom_medoc='';
if (isset($_REQUEST['nom_medoc'])) 
{
  $nom_medoc=$_REQUEST['nom_medoc'];
}

$radio_distri='4'; //medicaments de ville par defaut
if (isset ($_REQUEST['radio_distri']))
{
  $radio_distri=$_REQUEST['radio_distri'];
}

function prix_boite ($cuv,$db)
{
	      $sql_prix="SELECT f3 FROM t01 WHERE f0='$cuv'"; //Par ucd
	      $resultat_prix=mysqli_query($db,$sql_prix);
	      $ligne_prix=mysqli_fetch_array($resultat_prix);
	      return $ligne_prix['f3'];
}
?>
	  <fieldset>
	    <legend>
	      Recherche de médicament
	    </legend> 
	    <input name="nom_medoc" type="text" id="nom_medoc" title="Tout ou partie du nom, ajouter le signe % pour compléter automatiquement" value="<?php echo $nom_medoc ?>" onkeyup="request(readData);"/>
<!--	    <input name="envoyer_nom_medoc" type="submit" value="Nom commercial" onclick="return verif_champ(document.forms['form_global'].nom_medoc.value)" />
	    <input name="envoyer_nom_medoc" type="submit" value="DCI" onclick="return verif_champ(this.value)" />
            <input name="envoyer_nom_medoc" type="submit" value="ATC" onclick="return verif_champ(this.value)" />
	    <input name="envoyer_nom_medoc" type="submit" value="Indication" onclick="return verif_champ(this.value)" />-->
	    <select name="select_type_requete" id="select_type_requete" onchange="request(readData);" />
	      <option value="Nom commercial">Nom commercial</option>
	      <option value="DCI">DCI</option>
	      <option value="ATC">ATC</option>
	      <option value="Indication">Indication</option>
	    </select>
	    <label for="radio_distri_2">
	      Hôpitaux
	    </label>
	    <input type="radio" value="2" name="radio_distri" id="radio_distri_2" <?php if ($radio_distri=="2") echo 'checked="checked"' ?>  onclick="request(readData);"/><!-- Seuls les types 2 et 4 existent en vrai -->
	    <label for="radio_distri_4">
	      Officines
	    </label>
	    <input type="radio" value="4" name="radio_distri" id="radio_distri_4" <?php if ($radio_distri=="4") echo 'checked="checked"' ?> onclick="request(readData);"/>
Tri par
	    <label for="radio_tri_n">
	      Nom
	    </label>
	    <input type="radio" value="n" name="radio_tri" id="radio_tri_n" <?php if ($order=='n') echo 'checked="checked"' ?>  onclick="request(readData);" /><!-- Seuls les types 2 et 4 existent en vrai -->
	    <label for="radio_tri_p">
	      Prix
	    </label>
	    <input type="radio" value="p" name="radio_tri" id="radio_tri_p" <?php if ($order=='p') echo 'checked="checked"' ?> onclick="request(readData);" />
	    <label for="select_type">
	      Type&nbsp;:
	    </label>
	    <select name="select_type" id="select_type"  onchange="request(readData);">
	      <option value="1">Spécialités humaines
	      </option>
	      <option value="2">Diététique
	      </option>
	      <option value="3">Vétérinaire
	      </option>
	      <option value="4">Parapharmacie
	      </option>
	      <option value="5">Accessoires
	      </option>
	      <option value="6">Divers
	      </option>
	    </select>
	    <input type="button" value="Chercher" onclick="request(readData);" />
	    <select name="select_medoc"> 
	    </select>
	    <input name="button_select_medoc" type="submit" value="Ajouter à la liste" />
	    <input name="check_dci" id="check_dci" type="checkbox" <?php if ($check_dci) echo "checked='checked'"?>/>
	    <label for="check_dci">
	      Prescription en DCI
	    </label>
<!--	    <input name="check_limit" id="check_limit" type="checkbox" />
	    <label for="check_limit">
	      Ignorer les restrictions d'AMM
	    </label>-->

	  </fieldset>
<?php
//   }
// }
?>
<?php
function unite_t24($unite_code,$db) //fonction pour trouver l'unite en clair en fonction de son code
{
  if ($unite_code=="")
    return ;
  else
  {	
    $sql_unite="SELECT f1 FROM t24 WHERE f0='$unite_code'";
    $resultat_unite=mysqli_query($db,$sql_unite);
    $ligne_unite=mysqli_fetch_array($resultat_unite);
    if (detectUTF8($ligne_unite['f1']))
      return $ligne_unite['f1'];
    else 
      return utf8_encode($ligne_unite['f1']);
  }
}

function unite_t24_plur($unite_code,$db) //fonction pour trouver l'unite en clair en fonction de son code
{
  if ($unite_code=="")
    return ;
  else
  {	
    $sql_unite="SELECT f5 FROM t24 WHERE f0='$unite_code'";
    $resultat_unite=mysqli_query($db,$sql_unite);
    $ligne_unite=mysqli_fetch_array($resultat_unite);
    if (detectUTF8($ligne_unite['f5']))
      return $ligne_unite['f5'];
    else 
      return utf8_encode($ligne_unite['f5']);
  }
}

if (isset($_REQUEST['cuv_medoc'])) //vient de la fiche vidal ou de la liste des medocs retenus pour l'ordonnance
{
  $cuv_medoc=$_REQUEST['cuv_medoc'];
}

if (isset($_REQUEST['cip_medoc'])) //vient de synthese_ordo 
{
  $cip_medoc=$_REQUEST['cip_medoc']; //On ajoute le medoc qui vient du deroulant
  $cuv_medoc=array();
  $cuv_medoc_from_cip=array();
  foreach ($cip_medoc AS $this_cip_medoc) //arrive eventuellement avec des _nb_poso a la fin
  {
    $sql_chercher_cuv="SELECT f1 FROM t00 WHERE f2='$this_cip_medoc'";
    $resultat_chercher_cuv=mysqli_query($db,$sql_chercher_cuv);
    $ligne_chercher_cuv=mysqli_fetch_array($resultat_chercher_cuv);
    $cuv_medoc[]=$ligne_chercher_cuv['f1'];
    $cuv_medoc_from_cip[]=$ligne_chercher_cuv['f1'];
  }
  reset($cip_medoc);
}

function afficher_medocs($cip_this_medoc,$this_cuv_medoc,$nom_medoc_from_url,$value_min,$unit_min,$period_min,$value_max,$unit_max,$period_max,$select_duree,$note,$a,$text_duree,$cip_fond,$sexe_poso,$sexe_patient,$regles_delivrance,$liste,$note_intolerances,$duree_min,$duree_max,$code_unite_duree_min,$code_unite_duree_max,$prix_boite,$nombre_boite,$prix_total,$nombre_de_boites,$unit_singulier,$unit_pluriel,$indications,$indication,$pgr,$doc_ald,$smr_array)
{
  $tab=explode('_',$cip_this_medoc);
?>
	    <input name="duree_max<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $duree_max ?>"/>
	    <input name="liste<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $liste ?>"/>
	    <input name="duree_min<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $duree_min ?>"/>
	    <input name="unit_singulier<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $unit_singulier ?>"/>
	    <input name="unit_pluriel<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $unit_pluriel ?>"/>
	    <input name="code_unite_duree_max<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $code_unite_duree_max ?>"/>
	    <input name="code_unite_duree_min<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $code_unite_duree_min ?>"/>
	    <input name="sexe_poso<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $sexe_poso ?>"/>
	    <input name="regles_delivrance<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo stripslashes($regles_delivrance) ?>"/>
	    <input name="note_intolerances" type="hidden" value="<?php echo stripslashes($note_intolerances) ?>"/>
	    <input name="cuv_medoc<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $this_cuv_medoc ?>"/>
	    <input name="cip_medoc[]" type="checkbox" value="<?php echo $cip_this_medoc ?>" checked="checked"/>
	    <input name="name_medoc<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $nom_medoc_from_url ?>" />
	    <a href="medocs.php?cuv_medoc=<?php echo $this_cuv_medoc ?>" onclick="window.open(this.href); return false;"><?php if (in_array($tab[0],$cip_fond)) echo "<strong>";echo $nom_medoc_from_url;if (in_array($tab[0],$cip_fond)) echo "</strong>"; ?>  </a> 
	    De 
	    <input name="value_min<?php echo $cip_this_medoc ?>" type="text" value="<?php echo $value_min; ?>" size="<?php if ($value_min) echo strlen($value_min) ; else echo '2' ?>" onchange="calcul_ligne('<?php echo $cip_this_medoc ?>'); singulier_pluriel_value_min('<?php echo $unit_singulier ?>','<?php echo $unit_pluriel ?>','<?php echo $cip_this_medoc ?>','min');calcul_total()" /> 
	    <input name="unit_min<?php echo $cip_this_medoc ?>" type="text" value="<?php echo $unit_min ?>" size="<?php echo strlen($unit_min) ?>" /> 
	    <input name="period_min<?php echo $cip_this_medoc ?>" type="text" value="<?php echo $period_min?>" size="<?php if (strlen($period_min)) echo strlen($period_min); else echo 2 ?>" onchange="calcul_ligne('<?php echo $cip_this_medoc ?>')" /> 
	    <input name="a<?php echo $cip_this_medoc ?>" type="checkbox" <?php $temp='a'.$cip_this_medoc ; if (isset($_REQUEST[$temp])) echo "checked='checked'" ?> onchange="calcul_ligne('<?php echo $cip_this_medoc ?>');calcul_total()" />
	    à 
	    <input name="value_max<?php echo $cip_this_medoc ?>" type="text" value="<?php echo  $value_max ?>"  size="<?php  if ($value_max) echo strlen($value_max) ; else echo '2' ?>" onchange="calcul_ligne('<?php echo $cip_this_medoc ?>'); singulier_pluriel_value_min('<?php echo $unit_singulier ?>','<?php echo $unit_pluriel ?>','<?php echo $cip_this_medoc ?>','max');calcul_total()" /> 
	    <input name="unit_max<?php echo $cip_this_medoc ?>" type="text" value="<?php echo $unit_max ?>" size="<?php echo strlen($unit_max) ?>"  /> 
	    <input name="period_max<?php echo $cip_this_medoc ?>" type="text" value="<?php echo $period_max ?>" size="<?php if (strlen ($period_max)) echo strlen ($period_max); else echo '2' ?>"  onchange="calcul_ligne('<?php echo $cip_this_medoc ?>')"/>

<!-- Répartition :  -->
<!-- <select name="select_repartition<?php // echo $cip_medoc ?>"> -->
<!--   <option value=""> -->
<!--     Matin, midi et soir -->
<!--   </option> -->
<!--   <option value=""> -->
<!--     Matin et soir -->
<!--   </option> -->
<!-- </select> -->
	    pendant
	    <input name="text_duree<?php echo $cip_this_medoc ?>" type="text" class="text_duree" value="<?php echo $text_duree ?>" size="2" onchange="verif_poso(this.value,'<?php echo $cip_this_medoc ?>') ; calcul_ligne('<?php echo $cip_this_medoc ?>');calcul_total()"/>
	    <select name="select_duree<?php echo $cip_this_medoc ?>" class="select_duree" onchange="verif_poso(this.form.text_duree<?php echo $cip_this_medoc ?>.value,'<?php echo $cip_this_medoc ?>') ; calcul_ligne('<?php echo $cip_this_medoc ?>');calcul_total()">
	      <option value="jours" <?php  if ($select_duree=="jours") echo "selected='selected'" ?>>
		jours
	      </option>
	      <option value="semaines" <?php  if ($select_duree=="semaines") echo "selected='selected'" ?>>
		semaines
	      </option>
	      <option value="mois" <?php  if ($select_duree=="mois") echo "selected='selected'" ?>>
		mois
	      </option>
	      <option value="an" <?php  if ($select_duree=="an") echo "selected='selected'" ?>>
		an
	      </option>
	    </select>
<?php
if (count($indications)> 0 )
{
  foreach ($indications AS $this_indication) //le deroulant select des indications pour ce medicament
  {
?>
	    <input name="indications<?php echo $cip_this_medoc ?>[]" type="hidden" value="<?php echo stripslashes($this_indication) ?>" />
<?php
  }
} //fin if 
?>
	    <label for="indication<?php echo $cip_this_medoc ?>">
	      Indication&nbsp;:
	    </label>

	    <select name="indication<?php echo $cip_this_medoc ?>" id="indication<?php echo $cip_this_medoc ?>">
<?php
reset ($indications);

if (count($indications)) //si exactement une
{

  foreach ($indications AS $this_indication) //le deroulant select des indications pour ce medicament
  {
    echo "
	      <option value='".stripslashes(urlencode($this_indication))."'";
    if (urldecode($indication)==$this_indication)
      echo " selected='selected'";
    echo ">".stripslashes($this_indication)."
	      </option>";
  }
}
?>
	    </select>
	    <label for="check_ns<?php echo $cip_this_medoc ?>" title="Non substituable">NS</label>
	    <input name="check_ns<?php echo $cip_this_medoc ?>" type="checkbox" id="check_ns<?php echo $cip_this_medoc ?>" <?php $temp='check_ns'.$cip_this_medoc ; if (isset($_REQUEST[$temp])) echo "checked='checked'" ?> />
	    <label for="check_ald<?php echo $cip_this_medoc ?>">ALD</label>
	    <input name="check_ald<?php echo $cip_this_medoc ?>" type="checkbox" id="check_ald<?php echo $cip_this_medoc ?>" class="check_ald" <?php $temp='check_ald'.$cip_this_medoc ; if (isset($_REQUEST[$temp])) echo "checked='checked'" ?> />
	    <strong>Note</strong> : 
	    <input name="note<?php echo $cip_this_medoc ?>" type="text" value="<?php echo stripslashes($note) ?>" size="<?php if (strlen($note)<100) echo strlen($note); else echo "60" ?>" title="<?php echo stripslashes($note) ?>" />
<!-- Passer en hidden ensuite -->
	    <input name="prix_boite<?php echo $cip_this_medoc ?>" id="prix_boite<?php echo $cip_this_medoc ?>"  type="hidden" value="<?php echo $prix_boite ?>" />
	    <input name="nombre_boite<?php echo $cip_this_medoc ?>" id="nombre_boite<?php echo $cip_this_medoc ?>"  type="hidden" value="<?php echo $nombre_boite ?>" />
	    <input name="nombre_de_boites<?php echo $cip_this_medoc ?>" id="nombre_de_boites<?php echo $cip_this_medoc ?>"  type="text" value="<?php echo $nombre_de_boites ?>" size="2" onchange="calcul_ligne_from_boite('<?php echo $cip_this_medoc ?>')" />
boîtes 
	    Prix : 
	    <input name="prix_ligne<?php echo $cip_this_medoc ?>" id="prix_ligne<?php echo $cip_this_medoc ?>" type="text" value="<?php echo $prix_total ?>" size="3" onchange="calcul_total()" /> €
	    <a href="fiche_vidal.php?cuv_medoc=<?php echo $this_cuv_medoc ?>" onclick="window.open(this.href); return false;" >Fiche Vidal</a>
<?php

  if ($sexe_patient=="M" AND $sexe_poso=="F") //alertes si contradiction sexe patient - medicament
    echo "<strong>Attention ! Spécialité réservée aux femmes</strong>";
  if ($sexe_patient=="F" AND $sexe_poso=="M")
    echo "<strong>Attention ! Spécialité réservée aux hommes</strong>";
  if ($sexe_poso AND $sexe_patient!='M' AND $sexe_patient!='F') //alerte si sexe patient necessaire mais inconnu
    echo "<strong>Attention ! Sexe patient nécessaire</strong>";
  echo stripslashes($regles_delivrance);
if ($pgr)
{
?>
	      <input name="pgr<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo substr_replace($pgr,'0',10,1)//on enleve le caractere surnumeraire et on met un 0 propre a la place ?>" />
<?php
  $pgr=str_replace('DS_DOCSPGR','',$pgr);
  $pgr = substr($pgr,1);
?>
	      <input name="pgr_bouton" type="button" value="PGR" onclick="display_document('<?php echo "DS_DOCS/PGR/0".$pgr ?>')"/>
<?php
}
if ($doc_ald)
{
?>
	      <input name="doc_ald<?php echo $cip_this_medoc ?>" type="hidden" value="<?php echo $doc_ald //on enleve le caractere surnumeraire et on met un 0 propre a la place ?>" />
<?php
  $doc_ald=str_replace('DS_DOCSALD','',$doc_ald);
?>
	      <input name="ald_button" type="button" value="ALD" onclick="display_document('<?php echo "DS_DOCS/ALD/".$doc_ald ?>')"/>
<?php
}
?>

<?php
if (count($smr_array))
{
  foreach ($smr_array AS $this_smr_array)
  {
?>
<input name='smr_array<?php echo $cip_this_medoc ?>[]' type="hidden" value="<?php echo $this_smr_array ?>" />
<?php
  }
?>
<select name='smr<?php echo $cip_this_medoc ?>[]' style="width:150px">
<?php
  reset ($smr_array);
  foreach ($smr_array AS $this_smr_array)
  {
    $this_smr_array_values=explode ('|',$this_smr_array);
    echo "
    <option value=\"$this_smr_array_values[1]\">$this_smr_array_values[0]</option>";
  }
?>
</select>
<?php
}
else //le produit n'a pas de smr_array
{
?>
<input name='smr_array<?php echo $cip_this_medoc ?>[]' type="hidden" value="" />
<?php
}

?>
	    <br />
  <?php //ajouter la substance sur la ligne
/*
    $substance=$ligne_chercher_medoc['f4']; echo $substance;
    $sql_substance="SELECT f4 FROM t26 WHERE f0='$substance'";
    $resultat_substance=mysqli_query($db,$sql_substance);
    $ligne_substance=mysqli_fetch_array( $resultat_substance );
    echo '('.utf8_encode($ligne_substance['f4']).')';
    $i++;
    $cip_this_medoc='';
    $nom_medoc_from_url='';
    $value_min='';
    $unit_min='';
    $period_min='';
    $value_max='';
    $unit_max='' ;
    $period_max='';
    $text_duree='';
    $select_duree='';
    $a='';
*/
} //fin function afficher_medocs

function fullUpper($string)
{
  return strtr(strtoupper($string), array(
      "à" => "À",
      "è" => "È",
      "ì" => "Ì",
      "ò" => "Ò",
      "ù" => "Ù",
      "á" => "Á",
      "é" => "É",
      "í" => "Í",
      "ó" => "Ó",
      "ú" => "Ú",
      "â" => "Â",
      "ê" => "Ê",
      "î" => "Î",
      "ô" => "Ô",
      "û" => "Û",
      "ç" => "Ç",
    ));
}
$multiplicateur['38']=1;
$multiplicateur['42']=7;
$multiplicateur['43']=30;
$multiplicateur['44']=365;
 
$prix_total=0;
//$cuv_medoc=array();
if (isset($_REQUEST['select_medoc']) AND isset($_REQUEST['button_select_medoc'])) //on vient du deroulant de selection
  $cuv_medoc[]=$_REQUEST['select_medoc']; //on ajoute le nouveau medoc en fin de liste
if ($cuv_medoc) //il y a des medicaments a mettre en ordonnance
{
?>
	  <fieldset>
	    <legend title="Les médicaments pris dans le traitement de fond sont écrits en gras">
	      Nouvelle ordonnance
	    </legend>
	    <div>
	      <input type="button" name="button_invers_ald" value="Inverser ALD" onclick="invers_ald() "/>
	      <input type="text" name="text_duree" id="text_duree" size="4" />
	      <select name="select_duree" id="select_duree">
		<option value="jours">
		  jours
		</option>
		<option value="semaines" >
		  semaines
		</option>
		<option value="mois" >
		  mois
		</option>
		<option value="an" >
		  an
		</option>
	      </select>
	      <input type="button" name="button_valider_date" value="Changer durée" onclick="global_duration() "/>
	    </div>
<?php
  $cpg_medoc=array();
  $smr_array=array();

  $i=0;

//   $check_limit='';
//   if (isset($_REQUEST['check_limit']))
//     $check_limit='1';
  foreach ($cuv_medoc AS $this_cuv_medoc)
  {
    $indications=array(); //la liste des indications de ce medicamebt
    $indication=''; //l'indication choisie - aucune pour le moment
    $select_duree='';
    $text_duree='';
    $regles_delivrance='';
    $liste='';
    $unite_min='';
    $unite_max='';
    if (isset($_REQUEST['button_select_medoc']) OR isset($_REQUEST['button_validate_form'])) //on vient du bouton de selection ou de la synthese ordo
    {
      $ce_medoc='';
      if (isset($_REQUEST['select_medoc']))
	$ce_medoc=$_REQUEST['select_medoc'];
      if ($this_cuv_medoc==$ce_medoc OR isset($_REQUEST['button_validate_form'])) //si le medoc est celui qui vient du deroulant ou on vient de synthese ordo, on analyse le vidal pour trouver les doses
      {
	$sql_chercher_medoc="SELECT * FROM t00 WHERE f1='$this_cuv_medoc'"; 
	$resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
	$ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc);
	$cip_this_medoc=$ligne_chercher_medoc['f2'];

	$this_cpg=$cpg_medoc[]=$ligne_chercher_medoc['f0'];

	$pgr=find_document($this_cpg,'PGR',$db); 
	$doc_ald=find_document($this_cpg,'ALD',$db); 
/*     $bum=str_replace('DS_DOCSBUM','',find_document($this_cpg,'BUM'));//bon usage du medicament
    $doc_ald=str_replace('DS_DOCSALD','',find_document($this_cpg,'ALD')); 

   if ($pgr)
      echo "PGR ".$pgr;
    if ($bum)
      echo " bum $bum";
    if ($doc_ald)
      echo "ald $doc_ald";
*/
  //t63=precautions precription
    //f0=code CUV
    //f2= Code registre d'incription
    //f3 = liste I ou II
    //f4 duree maxi pour une delivrance unique
    //f7 sites de prescription si ordinaire ou renouvellement de restreinte
    //f8 site de prescription initiale
    //fA Portee d'une prescription initiale apres laquelle le renouvellement n'est plus possible
    //fB Produit d'exception
    //fD ordonnance securisee ou chevauchemnet impossible, selon t1C.f0->f1

	$sql_regles_delivrance="SELECT * FROM t63  WHERE f0='$this_cuv_medoc'";
	$resultat_regles_delivrance=mysqli_query($db,$sql_regles_delivrance);
	$ligne_regles_delivrance=mysqli_fetch_array($resultat_regles_delivrance);
	$liste=$ligne_regles_delivrance['f1'];
	$fD=$ligne_regles_delivrance['fD'];

	$sql_delivrance_2="SELECT f1 FROM t1C WHERE f0='$fD'";
	$resultat_delivrance_2=mysqli_query($db,$sql_delivrance_2);
	$ligne_delivrance_2=mysqli_fetch_array($resultat_delivrance_2);

	$sql_ce_medoc="SELECT t4B.f0 AS numero,CAST(t4B.fF AS unsigned integer) AS psy, CAST(t4B.fG AS unsigned integer) AS dopant, t4B.fE AS vigi,t4B.fD AS code_vigi,t42.f0 AS classe_num,t4B.f2 AS nom,f5, t42.f2 AS classe_nom  FROM t4B INNER JOIN t42 ON t4B.fA=t42.f0 WHERE t4B.f0 = '$this_cpg'"; 
	$resultat_ce_medoc=mysqli_query($db,$sql_ce_medoc);
	$ligne_ce_medoc=mysqli_fetch_array($resultat_ce_medoc);

	if ( $ligne_regles_delivrance['fA']) //nombre de jours maxi ou NULL
	{
	  $text_duree=$ligne_regles_delivrance['fA'];
	  $regles_delivrance= '<strong>Durée maximale pour une prescription initiale</strong> '.$ligne_regles_delivrance['fA'].' jours.<br />';
	}
	if ( $ligne_regles_delivrance['f4'])//nombre de jours maxi ou NULL
	{
	  $text_duree=$ligne_regles_delivrance['f4'];
	  $regles_delivrance.= '<strong>Durée maximale pour une délivrance unique</strong> '.$ligne_regles_delivrance['f4'].' jours.<br />';
	}
	if ( bin2hex($ligne_regles_delivrance['fB'])=='01')
	{
	  $regles_delivrance.= '<strong>Ordonnance produit d\'exception</strong><br />';
	}
	if ( bin2hex($ligne_regles_delivrance['fC'])=='01')
	{
	  $regles_delivrance.= '<strong>Déconditionnement autorisé</strong><br />';
	}
	if ($ligne_ce_medoc['dopant']==1)
	{
	  $regles_delivrance.= '<strong>Dopant</strong><br />';
	}
	if ($ligne_ce_medoc['psy']==1)
	{
	  $regles_delivrance.= '<strong>Psychotrope</strong><br />';
	}
	if (isset($_REQUEST['numeroID']))
	{
//recherche des pathologies contre-indiquant ce produit, en croisant avec les pathologies du patient (CIM-X du terrain)
//On cherche les etats pathologiques et indications
	  $sql_patho="SELECT t50.f1 AS patho,t52.f5 AS commentaire, t50.f0 AS code_patho, t1C.f1 AS nature FROM t52 INNER JOIN t50 on t52.f0=t50.f0 INNER JOIN t1C ON t52.f2=t1C.f0 WHERE t52.f1='$this_cpg'";
	  $resultat_patho=mysqli_query($db,$sql_patho);
	  while ($ligne_patho=mysqli_fetch_array($resultat_patho))
	  {
	    if (count($tableau_patho_vidal)) //pas besoin de chercher si pas d'ATCD
	    { //on cherche les contre-indications du produit
	      if (in_array($ligne_patho['code_patho'],$tableau_patho_vidal) AND (stristr($ligne_patho['nature'],'Contre-indication') OR $ligne_patho['nature']=='Précaution d\'emploi'))
	      {
		$regles_delivrance.='<strong>'.$ligne_patho['nature'].'</strong>&nbsp;: '.$ligne_patho['patho'].' ('.$ligne_patho['commentaire'].')<br />';
	      }
	    } //on cherche les indications
	    if ($ligne_patho['nature']=='Indication')
	    {
	      $indications[]=$ligne_patho['patho'];
	    }
	  }
	  if (count($intolerances_code)) //les codes des intolerances du patient
	  {
  //amox = 0098Z
	    $tableau_intolerances_uniformisees=array(); //contiendra toutes les intolerances du patient, unifiees
  //on transforme l'intolerance en numero unique 
	    foreach($intolerances_code AS $this_intolerances_code)
	    {
	      $tableau_intolerances_uniformisees[]=unified_component($this_intolerances_code,$db);
	    }

  //on cherche les codes substance contenus dans ce medoc
	    $sql_intolerance="SELECT f2 FROM t07 WHERE f0='$this_cpg'"; //renvoie toutes les substances dans ce medicament prescrit
	    $resultat_intolerance=mysqli_query($db,$sql_intolerance);

	    while($ligne_intolerance=mysqli_fetch_array($resultat_intolerance)) //on analyse toutes les substances du medicament
	    {

	      $this_code_unifie=unified_component($ligne_intolerance['f2'],$db);
	      if (in_array($this_code_unifie,$tableau_intolerances_uniformisees))
	      {
		$key=array_search($ligne_intolerance['f1'],$tableau_intolerances_uniformisees);
		$component=$intolerances_text[$key];
		$regles_delivrance.='<strong>Intolérance à '.$component.'</strong> ';
	      }
	    }
  //************Les intolerances croisees************
	    reset ($intolerances_code); //intolerances du patient
	    mysqli_data_seek($resultat_intolerance,0) ;
	    $compare=""; //recupere le complementaire
	    while($ligne_intolerance=mysqli_fetch_array($resultat_intolerance)) //on analyse toutes les substances du medicament
	    {
	      $this_code_unifie=unified_component($ligne_intolerance['f2'],$db); //Les codes qu'on va tester avec t58
	      $sql_chercher_allergie_croisee="SELECT t58.f0 AS asked_component , t57.f0 AS component0,t57.f1 AS component1 FROM t58,t57 WHERE (t58.f0=t57.f0 OR t58.f0=t57.f1) AND t58.f1='$this_code_unifie'";
	      $resultat_chercher_allergie_croisee=mysqli_query($db,$sql_chercher_allergie_croisee);
	      while ($ligne_chercher_allergie_croisee=mysqli_fetch_array($resultat_chercher_allergie_croisee))
	      {
		if ($ligne_chercher_allergie_croisee['asked_component']==$ligne_chercher_allergie_croisee['component0'])
		{
		  $compare= $ligne_chercher_allergie_croisee['component1'];
		}
		else
		  $compare= $ligne_chercher_allergie_croisee['component0'];
	      }
  //chercher ici si les resultats trouves convergent avec la requete suivante
	    }
	    $tableau_intolerances_croisees=array();

	    foreach($intolerances_code AS $this_intolerances_code) //on rebalaye pour les intolerances croisees
	    {
  //on cherche si ce composant a des allergies croisees
	      $sql_chercher_allergie_croisee="SELECT t58.f0 AS intolerance_declaree,t57.f0 AS component0,t57.f1 AS component1 FROM t58,t57 WHERE (t58.f0=t57.f0 OR t58.f0=t57.f1) AND t58.f1='$this_intolerances_code'"; //t58.f1=amox 0098Z
	      $resultat_chercher_allergie_croisee=mysqli_query($db,$sql_chercher_allergie_croisee);

	      while ($ligne_chercher_allergie_croisee=mysqli_fetch_array($resultat_chercher_allergie_croisee))
	      {
		if ($compare==$ligne_chercher_allergie_croisee['intolerance_declaree'])
		{
		  $component0=$ligne_chercher_allergie_croisee['component0']; //12 peni
		  $component1=$ligne_chercher_allergie_croisee['component1']; //13 cefalo

		  $sql_explanation="SELECT f1 FROM `t56` WHERE f0='$component0' OR f0 = '$component1'";
		  $resultat_explanation=mysqli_query($db,$sql_explanation);
		  $cette_intolerance_croisee=array();
		  while ($ligne_explanation=mysqli_fetch_array($resultat_explanation))
		  {
		    $cette_intolerance_croisee[]=$ligne_explanation['f1'];
		  }
		  $tableau_intolerances_croisees[]="Risque d'intolérance croisée entre ".$cette_intolerance_croisee[0]." et ".$cette_intolerance_croisee[1];
		}
	      }
	    }
	    foreach($tableau_intolerances_croisees AS $this_intolerance_croisee)
	    {
	      $regles_delivrance.=$this_intolerance_croisee;
	    }
	  }
  //Recherche des informations de grossesse si femme entre 10 et 55 ans
	  if ($sexe_patient=="F" AND $year_age > 10 AND $year_age<55)
	  {
	    $sql_etat_patho="SELECT t50.f1 AS patho,t52.f5 AS commentaire,t1C.f1 AS nature,t50.f0 AS code_unite, t52.f3 AS frequence_effet FROM t52 INNER JOIN t50 on t52.f0=t50.f0 INNER JOIN t1C ON t52.f2=t1C.f0 WHERE t52.f1='$this_cpg'";
	    $resultat_etat_patho=mysqli_query($db,$sql_etat_patho);
	    while ($ligne_etat_patho=mysqli_fetch_array($resultat_etat_patho))
	    {
	      if (stristr ($ligne_etat_patho['patho'],'Grossesse') AND (stristr($ligne_etat_patho['nature'],'Contre-indication') OR $ligne_etat_patho['nature']=='Précaution d\'emploi'))
	      {
		$regles_delivrance.= '<strong>'.$ligne_etat_patho['nature'].'</strong>&nbsp;: '.$ligne_etat_patho['patho'].' ';
		$code_unite=$ligne_etat_patho['code_unite'];
  //remplacer ca par function t24 ?	     
		$sql_unite_etat="SELECT t2A.f1 AS type_valeur, t24.f5 AS unite_valeur, t2G.f1 AS valeur_min, t2G.f2 AS valeur_max FROM t2G INNER JOIN t2A ON t2G.f0=t2A.f0 INNER JOIN t24 ON t2G.f3=t24.f0 WHERE t2G.f4='$code_unite'";
		$resultat_unite_etat=mysqli_query($db,$sql_unite_etat);
		$ligne_unite_etat=mysqli_fetch_array($resultat_unite_etat);
  //ajouter ici l'age de la grossesse en semaines pour comparaison
		if ($ligne_unite_etat['type_valeur']=='Aménorrhée') //Aménorrhée
		  $regles_delivrance.= ' Aménorrhée';
		if ($ligne_unite_etat['valeur_min']) //nombre de semaines avant lequel grossesse n'interfere pas
		  $regles_delivrance.=  " min&nbsp;: ".$ligne_unite_etat['valeur_min'];
		if ($ligne_unite_etat['unite_valeur'])
		  $regles_delivrance.= " ".utf8_encode($ligne_unite_etat['unite_valeur']); //semaines

		if ($ligne_unite_etat['valeur_max'])//nombre de semaines après lequel grossesse n'interfere plus
		  $regles_delivrance.=  " max : ".$ligne_unite_etat['valeur_max'];
		if ($ligne_unite_etat['unite_valeur'])
		  $regles_delivrance.= " ".utf8_encode($ligne_unite_etat['unite_valeur']).'<br />'; //semaines
  //mettre ici un popup javascript si grossesse dans les bornes ?
	      }
	      elseif ($ligne_etat_patho['patho']=='Allaitement' AND $allaitement=='oui') //On met l'allaitement seulement si femme allaitante
	      {
		$regles_delivrance.='<strong>'.$ligne_etat_patho['nature']."</strong> Allaitement";
	      }
	    }
	  }
	}
	$sql_posologies="SELECT * FROM t2P WHERE f1='$this_cpg'";
	$resultat_posologies=mysqli_query($db,$sql_posologies);
	$trouve_poso=0; //flag pour dire si on trouve une poso pour l'age
	if ($count_posologies=mysqli_num_rows($resultat_posologies)) //Si le medoc a une ligne poso
	{
	  $nb_poso=0; //compteur a ajouter au cip pour ne pas melanger les poso multiples
	  while ($ligne_posologies=mysqli_fetch_array($resultat_posologies))
	  {
	    $duree_min=$ligne_posologies['fL'];
	    $duree_max=$ligne_posologies['fN'];
	    $code_unite_duree_min=$ligne_posologies['fM'];
	    $code_unite_duree_max=$ligne_posologies['fO'];
//mettre aussi les unites
	    $toutes_les_substances_en_clair=array();
	    $text_dure='';
	    $select_duree='';
	    $sexe_poso='';
	    $code_liaison_T2L=$ligne_posologies['f0'];
	    $sql_bornes="SELECT * FROM t2L WHERE f4='$code_liaison_T2L'"; //Plusieurs reponses selon la problematique (poso, sexe)
	    $resultat_bornes=mysqli_query($db,$sql_bornes);
	    $count_bornes=mysqli_num_rows($resultat_bornes);

	    $unite_min=$ligne_posologies['f3'];
	    $unite_max=$ligne_posologies['f8'];
	    $frequence_mini_code=$ligne_posologies['f6'];
	    $frequence_maxi_code=$ligne_posologies['fF'];

	  //unite de la frequence mini
	    $frequence_mini_clair='';
	    if ($frequence_mini_code) //ne rien afficher si non renseigne
	    {
	      $frequence_mini_clair=unite_t24($frequence_mini_code,$db);
	    }
	    $nom_medoc_from_url=$ligne_chercher_medoc['f3'];

/*t27 substances
    f0=code substance ->t07.f2,t11.f1,t28.f0,t28.f1,t3G.f0,t43.f4,t54.f0,t58.f1,t59.f0,t59.f1,t5I.f0 et f1
    f2= nom substance
    f6=interaction ->t26.f0
    f8=code OMS
    fB=dopant MJS

*/
//Zone qui ne devrait pas dependre du if poso
	    $sql_substance="SELECT * FROM t07 INNER JOIN t27 ON t07.f2=t27.f0 WHERE t07.f0='$this_cpg'"; //cpg
	    $resultat_substance=mysqli_query($db,$sql_substance);
	    $cette_ligne="";
	    if ($check_dci) //On transforme le nom en DCI et on ecrase le nom medoc
	    {
	      $nom_medoc_from_url='';
	    }
	    while ($ligne_substance=mysqli_fetch_array($resultat_substance))
	    {
/*composants t08
f0->t4B.f0 = cpg
f2->t1C.f0 = descripteur de composant (f1)//libelles dans t1C
f6->t24.f0 = unite (f1)
f8->t24.f0
fA->t24.f0
*/
	      $cette_ligne=$ligne_substance['f1'];
	      $sql_section_composant="SELECT * FROM t08 WHERE f1='$cette_ligne'";
	      $resultat_section_composant=mysqli_query($db,$sql_section_composant);
	      $ligne_section_composant=mysqli_fetch_array($resultat_section_composant);
	      if ($check_dci) //On transforme le nom en DCI et on ecrase le nom medoc
	      {
		if ($ligne_section_composant['fB']=='0') 
		{
		  if ($nom_medoc_from_url)
		    $nom_medoc_from_url=$nom_medoc_from_url.' + '.fullUpper($ligne_substance['f2'])." ".$ligne_substance['f4']." ".unite_t24($ligne_substance['f5'],$db);
		  else
		    $nom_medoc_from_url=fullUpper($ligne_substance['f2'])." ".$ligne_substance['f4']." ".unite_t24($ligne_substance['f5'],$db);
		}
	      }
	      $toutes_les_substances_en_clair[]=$ligne_substance['f2'];
	    }
//On reexamine les substance sans tenir compte de principe actif/excipient pour les intolerances
	    if (count($intolerances_text)) //pris dans le terrain
	    {
	      foreach ($intolerances_text as $cette_allergie)
	      {
		if (in_array($cette_allergie,$toutes_les_substances_en_clair))
		{
		  $regles_delivrance.='<strong>Intolérance à '.$cette_allergie.'</strong>';
		}
		else //au cas ou on ne trouve pas de correspondance exacte, on cherche en mode degrade
		{
		  foreach ($toutes_les_substances_en_clair AS $cette_substance_en_clair)
		  {
		    if (strpos($cette_substance_en_clair,$cette_allergie) OR strpos($cette_allergie,$cette_substance_en_clair))
		    {
		      $regles_delivrance.='<strong>Intolérance à '.$cette_allergie.'</strong> (recherche approximative)';
		    }
		  }
		}
	      }
	    }
//Fin de zone qui ne devrait pas dependre du if poso

	    $value_min=$ligne_posologies['f2'];
	    $period_min=$ligne_posologies['f4'];
	    $value_max=$ligne_posologies['f7'];
	    $period_max=$ligne_posologies['f9'];
//on nettoie les chaines inexploitables pour les periodes
	    $search= array("1 prise","type de fréquence non-spécifié","toutes les ");
	    $replace=array("par prise","","par jour, en espaçant les prises de ");
	    $period_max= str_replace($search,$replace,$period_max);
	    $period_min= str_replace($search,$replace,$period_min);


	    //unite de la poso en accordant le pluriel si necessaire
	    if ($value_min<=1)
	      $unit_min=unite_t24($unite_min,$db);
	    else
	      $unit_min=unite_t24_plur($unite_min,$db);
	    if ($value_max<=1)
	      $unit_max=unite_t24($unite_max,$db);
	    else
	      $unit_max=unite_t24_plur($unite_max,$db);
	    $unit_singulier=unite_t24($unite_min,$db); //les unites en clair

	    $unit_pluriel=unite_t24_plur($unite_min,$db);
	    if (strstr($unit_min,'/kg')) //On examine le cas ou la poso est donnee en unite/kg
	    {
	      if ($poids!='inconnu')
	      {
		$value_min=$ligne_posologies['f2'];
		$value_min=$value_min*$poids; //calcul des unites/kg
		$value_min=round($value_min, 0, PHP_ROUND_HALF_DOWN); //troncature pour doses entieres
		if ($value_min<=1)
		{
		  $unit_min=unite_t24($unite_min,$db); //singulier
		  $unit_min=str_replace('/kg','',$unit_min);
		}
		else
		{
		  $unit_min=unite_t24_plur($unite_min,$db); //pluriel
		  $unit_min=str_replace('/kg','s',$unit_min);
		}
	      $unit_pluriel=str_replace('/kg','s',unite_t24_plur($unite_min,$db));
	      $unit_singulier=str_replace('/kg','',unite_t24($unite_min,$db));
	      }
	      else
	      {
		$unit_min.=' - poids inconnu';
	      }
	    }
	    elseif (strstr($unit_min,'par mètre-carré'))
	    {
	      if ($surface)
	      {
		$unit_min=str_replace('par mètre-carré','',$unit_min);
		$value_min=$value_min*$surface;
	      }
	      else
		$unit_min.=' - taille nécessaire';
	    }
	    if (strstr($unit_max,'/kg'))
	    {
	      if ($poids!='inconnu')
	      {
		$value_max=$ligne_posologies['f7'];
		$value_max=$value_max*$poids;
		$value_max=round($value_max, 0, PHP_ROUND_HALF_DOWN); //troncature pour doses entieres
		if ($value_max<=1)
		{
		  $unit_max=unite_t24($unite_max,$db);
		  $unit_max=str_replace('/kg','',$unit_max);
		}
		else
		{
		  $unit_max=unite_t24_plur($unite_max,$db);
//la gestion du pluriel pour les mg/kg ne marche pas car pas renseigne correctement
		  $unit_max=str_replace('/kg','s',$unit_max);
		}
		$unit_pluriel=str_replace('/kg','s',unite_t24_plur($unite_max,$db));
		$unit_singulier=str_replace('/kg','',unite_t24($unite_max,$db));
	      }
	      else
	      {
		$unit_max.=' - poids inconnu';
	      }
	    }
	    elseif (strstr($unit_max,'par mètre-carré'))
	    {
	      if ($surface)
	      {
		$unit_max=str_replace('par mètre-carré','',$unit_max);
		$value_max=$value_max*$surface;
	      }
	      else
		$unit_min.=' - taille nécessaire';
	    }
	    $note='';
	    if ($ligne_ce_medoc['code_vigi']!='1') //voir t5A.f0
	      $note= $ligne_ce_medoc['vigi'];


	    $prix_boite=prix_boite($this_cuv_medoc,$db);
	    $nombre_boite=$ligne_chercher_medoc['f8']; //NULL pour les pommades...
	    $nombre_de_boites=1;
	    $prix_ligne=$prix_boite;
	    if ($text_duree) //rarement disponible pour le calcul initial. Pour le recalcul, voir la fonction javascript calcul_ligne
	    {
	      $prix_ligne=round($value_max*$text_duree/$nombre_boite,0,PHP_ROUND_HALF_UP)*$prix_boite; 
	      $nombre_de_boites=round($value_min*$text_duree/$nombre_boite,0,PHP_ROUND_HALF_UP);
	    }

//le SMR
  $sql_chercher_smr="SELECT t3C.f4 AS texte_smr, t3E.f0 AS code_smr, t3E.f1 AS smr_en_clair FROM t3C INNER JOIN t3E ON t3C.f3=t3E.f0  WHERE t3C.f1='$this_cpg' AND t3C.f2='S'"; 
//echo $sql_chercher_smr;
  $resultat_chercher_smr=mysqli_query($db,$sql_chercher_smr);
  while ($ligne_chercher_smr=mysqli_fetch_array($resultat_chercher_smr))
  {
      $smr_array[]=$ligne_chercher_smr['texte_smr'].'|'.$ligne_chercher_smr['code_smr'].'|'.$ligne_chercher_smr['smr_en_clair'];
  }
//print_r($smr_array);
    if ($count_bornes) //bornes dans t2L
	    {
	      while ($ligne_bornes=mysqli_fetch_array($resultat_bornes))
	      {
		if ($ligne_bornes['f6']!= '')
		{
		  $sexe_poso=$ligne_bornes['f6']; //La limitation au sexe M ou F pour cette poso. Le A n'est jamais renseigne
		}
  //Chercher les medocs ou t2L.f3=1 (poids)
  //SELECT * FROM `t2L` INNER JOIN t2P ON t2L.f4=t2P.f0 INNER JOIN t4B ON t2P.f1=t4B.f0 WHERE t2L.f3=1

		if ($ligne_bornes['f3']=='44' OR $ligne_bornes['f3']=='43' OR $ligne_bornes['f3']=='42' OR $ligne_bornes['f3']=='38') //les ages 44 = ans, 43 = mois 42=semaines 38=jours 1 = kg 0=rien //le zero est traite par ailleurs (juste le sexe)
		{
		  $borne_inf=$ligne_bornes['f1']*$multiplicateur[$ligne_bornes['f3']];
		  $borne_sup=$ligne_bornes['f2']*$multiplicateur[$ligne_bornes['f3']];
		  if ($age_jours>$borne_inf AND $age_jours<=$borne_sup OR !$age_jours) //on affiche la ligne trouvee si age correspond ou si pas d'age du tout
		  {
		    $trouve_poso=1; //pour eviter l'affichage du message d'erreur de poso non trouvee
		    afficher_medocs($cip_this_medoc.'_'.$nb_poso,$this_cuv_medoc,$nom_medoc_from_url,$value_min,$unit_min,$period_min,$value_max,$unit_max,$period_max,$select_duree,$note,$a,$text_duree,$cip_fond,$sexe_poso,$sexe_patient, $regles_delivrance,$liste,$note_intolerances,$duree_min,$duree_max,$code_unite_duree_min,$code_unite_duree_max,prix_boite($this_cuv_medoc,$db),$ligne_chercher_medoc['f8'],$prix_ligne,$nombre_de_boites,$unit_singulier,$unit_pluriel,$indications,$indication,$pgr,$doc_ald,$smr_array); //on concatene le numero de poso a la fin du CIP. Il faudra splitter a la lecture
		  }
		}
		elseif ($ligne_bornes['f3']=='1') //poso par poids
		{
		  $borne_inf=$ligne_bornes['f1'];
		  $borne_sup=$ligne_bornes['f2'];

		  if ($poids>  $borne_inf AND $poids<=$borne_sup)
		  {
		    $trouve_poso=1; //pour eviter l'affichage du message d'erreur de poso non trouvee
		    afficher_medocs($cip_this_medoc.'_'.$nb_poso,$this_cuv_medoc,$nom_medoc_from_url,$value_min,$unit_min,$period_min,$value_max,$unit_max,$period_max,$select_duree,$note,$a,$text_duree,$cip_fond,$sexe_poso,$sexe_patient, $regles_delivrance,$liste,$note_intolerances,$duree_min,$duree_max,$code_unite_duree_min,$code_unite_duree_max,prix_boite($this_cuv_medoc,$db),$ligne_chercher_medoc['f8'],$prix_ligne,$nombre_de_boites,$unit_singulier,$unit_pluriel,$indications,$indication,$pgr,$doc_ald,$smr_array); //on concatene le numero de poso a la fin du CIP. Il faudra splitter a la lecture
		  }
		}
		$nb_poso++;
	      }
	    }
	    else //cas ou il n'existe pas de ligne dans t2L, on affiche dans tous les cas d'age et de poids et de sexe
	    {
	      $trouve_poso=1; //pour eviter l'affichage du message d'erreur de poso non trouvee
	      afficher_medocs($cip_this_medoc.'_'.$nb_poso,$this_cuv_medoc,$nom_medoc_from_url,$value_min,$unit_min,$period_min,$value_max,$unit_max,$period_max,$select_duree,$note,$a,$text_duree,$cip_fond,$sexe_poso,$sexe_patient, $regles_delivrance,$liste,$note_intolerances,$duree_min,$duree_max,$code_unite_duree_min,$code_unite_duree_max,prix_boite($this_cuv_medoc,$db),$ligne_chercher_medoc['f8'],$prix_ligne,$nombre_de_boites,$unit_singulier,$unit_pluriel,$indications,$indication,$pgr,$doc_ald,$smr_array); //on concatene le numero de poso a la fin du CIP. Il faudra splitter a la lecture

	    }
	  } //fin while
	  if ($trouve_poso==0)
	  {
?>
	    <div class="information">
	      Pas de posologie pour cet âge ou ce poids pour <?php echo $nom_medoc_from_url ?>
	    </div>
<?php
	  }
	}
	else //pas de precision de posologie dans la fiche medoc
	{
	  $toutes_les_substances_en_clair=array();

	  $nom_medoc_from_url=$ligne_chercher_medoc['f3'];
	  $sql_substance="SELECT * FROM t07 INNER JOIN t27 ON t07.f2=t27.f0 WHERE t07.f0='$this_cpg'"; //cpg
	  $resultat_substance=mysqli_query($db,$sql_substance);
	  $cette_ligne="";
	  if ($check_dci) //On transforme le nom en DCI et on ecrase le nom medoc
	  {
	    $nom_medoc_from_url='';
	  }
	  while ($ligne_substance=mysqli_fetch_array($resultat_substance))
	  {
/*composants t08
f0->t4B.f0 = cpg
f2->t1C.f0 = descripteur de composant (f1)//libelles dans t1C
f6->t24.f0 = unite (f1)
f8->t24.f0
fA->t24.f0
*/
	    $cette_ligne=$ligne_substance['f1'];
	    $sql_section_composant="SELECT * FROM t08 WHERE f1='$cette_ligne'";
	    $resultat_section_composant=mysqli_query($db,$sql_section_composant);
	    $ligne_section_composant=mysqli_fetch_array($resultat_section_composant);
	    if ($check_dci) //On transforme le nom en DCI et on ecrase le nom medoc
	    {
	      if ($ligne_section_composant['fB']=='0') 
	      {
		if ($nom_medoc_from_url)
		  $nom_medoc_from_url=$nom_medoc_from_url.' + '.fullUpper($ligne_substance['f2'])." ".$ligne_substance['f4']." ".unite_t24($ligne_substance['f5'],$db);
		else
		  $nom_medoc_from_url=fullUpper($ligne_substance['f2'])." ".$ligne_substance['f4']." ".unite_t24($ligne_substance['f5'],$db);
	      }
	    }
	    $toutes_les_substances_en_clair[]=$ligne_substance['f2'];
	  }
//On reexamine les substance sans tenir compte de principe actif/excipient pour les intolerances
	  if (count($intolerances_text)) //pris dans le terrain
	  {
	    foreach ($intolerances_text as $cette_allergie)
	    {
	      if (in_array($cette_allergie,$toutes_les_substances_en_clair))
	      {
		$regles_delivrance.='<strong>Intolérance à '.$cette_allergie.'</strong>';
	      }
	      else //au cas ou on ne trouve pas de correspondance exacte, on cherche en mode degrade
	      {
		foreach ($toutes_les_substances_en_clair AS $cette_substance_en_clair)
		{
		  if (strpos($cette_substance_en_clair,$cette_allergie) OR strpos($cette_allergie,$cette_substance_en_clair))
		  {
		    $regles_delivrance.='<strong>Intolérance à '.$cette_allergie.'</strong> (recherche approximative)';
		  }
		}
	      }
	    }
	  }
	  $prix_ligne=prix_boite($this_cuv_medoc,$db);
	  afficher_medocs($cip_this_medoc.'_0',$this_cuv_medoc,$nom_medoc_from_url,'','','','','','','','',$a,'',$cip_fond,'',$sexe_patient, $regles_delivrance,$liste,'','','','','',prix_boite($this_cuv_medoc,$db),$ligne_chercher_medoc['f8'],$prix_ligne,1,'','',$indications,$indication,$pgr,$doc_ald,$smr_array); //en l'absence de precision sur la poso, on initialise le prix total a la valeur d'une boite
	}
      }
      else //On vient du bouton de validation de select, mais on parle des autres medocs
      {
	$tab_cip=array();
//on recupere les valeurs donnees par l'URL dans la liste deja presente
	$tab_cip=explode('_',$cip_medoc[$i]);
	$cip_this_medoc=$tab_cip[0]; //on enleve les _ de nb_poso
	$sql_chercher_medoc="SELECT * FROM t00 WHERE f2='$cip_this_medoc'"; 
	$resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
	$ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc);

	$cpg_medoc[]=$ligne_chercher_medoc['f0'];

	$temp='name_medoc'.$cip_this_medoc.'_'.$tab_cip[1];
	$nom_medoc_from_url=$_REQUEST[$temp];
	$temp='sexe_poso'.$cip_this_medoc.'_'.$tab_cip[1];
	$sexe_poso=$_REQUEST[$temp];
	$temp='value_min'.$cip_this_medoc.'_'.$tab_cip[1];
	$value_min=$_REQUEST[$temp];
	$temp='unit_min'.$cip_this_medoc.'_'.$tab_cip[1];
	$unit_min=$_REQUEST[$temp];
	$temp='period_min'.$cip_this_medoc.'_'.$tab_cip[1];
	$period_min=$_REQUEST[$temp];
	$temp='value_max'.$cip_this_medoc.'_'.$tab_cip[1];
	$value_max=$_REQUEST[$temp];
	$temp='unit_max'.$cip_this_medoc.'_'.$tab_cip[1];
	$unit_max=$_REQUEST[$temp] ;
	$temp='period_max'.$cip_this_medoc.'_'.$tab_cip[1];
	$period_max=$_REQUEST[$temp];
	$temp='text_duree'.$cip_this_medoc.'_'.$tab_cip[1];
	$text_duree=$_REQUEST[$temp];
	$temp='select_duree'.$cip_this_medoc.'_'.$tab_cip[1];
	$select_duree=$_REQUEST[$temp];
	$temp='a'.$cip_this_medoc.'_'.$tab_cip[1];
	$a='';
	if (isset($_REQUEST[$temp]))
	  $a=$_REQUEST[$temp];
	$temp='note'.$cip_this_medoc.'_'.$tab_cip[1];
	$note=$_REQUEST[$temp];
	$temp='regles_delivrance'.$cip_this_medoc.'_'.$tab_cip[1];
	$regles_delivrance=$_REQUEST[$temp];
	$temp='duree_min'.$cip_this_medoc.'_'.$tab_cip[1];
	$duree_min=$_REQUEST[$temp];
	$temp='duree_max'.$cip_this_medoc.'_'.$tab_cip[1];
	$duree_max=$_REQUEST[$temp];
	$temp='code_unite_duree_min'.$cip_this_medoc.'_'.$tab_cip[1];
	$code_unite_duree_min=$_REQUEST[$temp];
	$temp='code_unite_duree_max'.$cip_this_medoc.'_'.$tab_cip[1];
	$code_unite_duree_max=$_REQUEST[$temp];
	$temp='prix_boite'.$cip_this_medoc.'_'.$tab_cip[1];
	$prix_boite=$_REQUEST[$temp];
	$temp='nombre_boite'.$cip_this_medoc.'_'.$tab_cip[1];
	$nombre_boite=$_REQUEST[$temp];
	$temp='prix_ligne'.$cip_this_medoc.'_'.$tab_cip[1];
	$prix_ligne=$_REQUEST[$temp];
	$temp='nombre_de_boites'.$cip_this_medoc.'_'.$tab_cip[1];
	$nombre_de_boites=$_REQUEST[$temp];
	$temp='cuv_medoc'.$cip_this_medoc.'_'.$tab_cip[1];
	$this_cuv_medoc=$_REQUEST[$temp];
	$trouve_poso=1;
	$temp='unit_singulier'.$cip_this_medoc.'_'.$tab_cip[1];
	$unit_singulier=$_REQUEST[$temp];
	$temp='unit_pluriel'.$cip_this_medoc.'_'.$tab_cip[1];
	$unit_pluriel=$_REQUEST[$temp];
	$temp='indications'.$cip_this_medoc.'_'.$tab_cip[1];
	$indications=array();
	if (isset($_REQUEST[$temp]))
	{
	  $indications=$_REQUEST[$temp];
	}
	$temp='indication'.$cip_this_medoc.'_'.$tab_cip[1];
	$indication='';
	if (isset($_REQUEST[$temp]))
	{
	  $indication=$_REQUEST[$temp];
	}
	$smr_array='';
	$temp='smr_array'.$cip_this_medoc.'_'.$tab_cip[1];
	if (isset($_REQUEST[$temp]))
	{
	  $smr_array=$_REQUEST[$temp];
	}
	$temp='liste'.$cip_this_medoc.'_'.$tab_cip[1];
	$liste=$_REQUEST[$temp];
	$temp='pgr'.$cip_this_medoc.'_'.$tab_cip[1];
	$pgr='';
	if (isset($_REQUEST[$temp]))
	  $pgr=$_REQUEST[$temp];
	$temp='doc_ald'.$cip_this_medoc.'_'.$tab_cip[1];
	$doc_ald='';
	if (isset($_REQUEST[$temp]))
	  $doc_ald=$_REQUEST[$temp];
	afficher_medocs($cip_medoc[$i],$this_cuv_medoc,$nom_medoc_from_url,$value_min,$unit_min,$period_min,$value_max,$unit_max,$period_max,$select_duree,$note,$a,$text_duree,$cip_fond,$sexe_poso,$sexe_patient, $regles_delivrance,$liste,$note_intolerances,$duree_min,$duree_max,$code_unite_duree_min,$code_unite_duree_max,$prix_boite,$nombre_boite,$prix_ligne,$nombre_de_boites,$unit_singulier,$unit_pluriel,$indications,$indication,$pgr,$doc_ald,$smr_array);
     }
    }

    $i++;
    if ($trouve_poso==1) //ne pas incrementer le prix si pas trouve de poso
      $prix_total=$prix_total+$prix_ligne;
  }
?>
	    <br />
<?php
  if (isset($_REQUEST['numeroID'])) //Pas de bouton pour faire l'ordonnance si pas de patient
  {
?>
	    <div style="float:left" ><strong>Prix total</strong>&nbsp;: </div><div id="total" style="float:left" ><?php echo $prix_total ?></div><div style="float:left">€</div>
	    <input type="submit" value="Faire l'ordonnance" name="button_ordo"/>
<?php
  }
?>
	  </fieldset>
<?php
}
?>
	</div>
      </form>
<?php
//Ici commencent les interactions et les redondances
//On trouve les CPG et CUV du traitement de fond a partir des CIP
if(isset($_REQUEST['numeroID']))
{
  foreach ($cip_fond AS $this_cip_fond)
  {
    $sql_chercher_medoc="SELECT f1,f0 FROM t00 WHERE f2='$this_cip_fond'"; 
    $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
    $ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc); //besoin d'un seul resultat
  //On ajoute les produits du traitement de fond aux produits disponibles
    $cuv_medoc[]=$ligne_chercher_medoc['f1'];
    $cpg_medoc[]=$ligne_chercher_medoc['f0'];
  }
}
//On retire les doublons
$cpg_medoc=array_unique($cpg_medoc);
$compteur=0;
$nbr_medocs=0;
if ($cuv_medoc)//on n'analyse rien si pas de medoc du tout
{
?>
<fieldset>
  <legend>
    Redondances
  </legend>
<?php
  $redondances=array();
  $flag='';

  foreach ($cpg_medoc AS $this_medoc)
  {
    $sql_chercher_composants="SELECT f1 FROM t11 WHERE f0 LIKE '$this_medoc'";
    $resultat_chercher_composants=mysqli_query($db,$sql_chercher_composants);
    while ($ligne_chercher_composants=mysqli_fetch_array($resultat_chercher_composants))
    {
      if (in_array($ligne_chercher_composants['f1'],$redondances))
      {
//on cherche le nom de la substance
	$this_substance=$ligne_chercher_composants['f1'];
	$sql_chercher_nom_substance="SELECT f2 FROM t27 WHERE f0='$this_substance'";
	$resultat_chercher_nom_substance=mysqli_query($db,$sql_chercher_nom_substance);
	$ligne_chercher_nom_substance=mysqli_fetch_array($resultat_chercher_nom_substance);
	$flag='1';
	echo "<b>Redondance</b>&nbsp;: ".$ligne_chercher_nom_substance['f2'].'<br />';
      }
      else //on enrichit le tableau des substances
	$redondances[]=$ligne_chercher_composants['f1'];
    }
  }

  if ($flag!='1')
    echo "<b>Aucune redondance</b>";
?>
</fieldset>
<fieldset title="Les interactions sont testées pour les médicaments de l'ordonnance et du traitement de fond">
  <legend>
    Interactions
  </legend>
<?php
  reset($cuv_medoc);
  reset($cpg_medoc);
  $cuv_medoc=array_unique($cuv_medoc); //suppression des doublons
  $nbr_medocs=count($cuv_medoc);
  $medoc=array();
  if ($nbr_medocs > 1) //on ne tente de determiner les interactions que si plus d'un medoc
  {
    foreach ($cpg_medoc AS $this_medoc)
    {
      $sql_chercher_medoc="SELECT t37.f1 AS inter FROM t4B INNER JOIN t37 ON t4B.f4=t37.f0 WHERE t4B.f0 = '$this_medoc'"; //on cherche le code d'interactions pour ce medoc A PARTIR du CPG
      $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
      $count_chercher_medoc=mysqli_num_rows($resultat_chercher_medoc);
      if ($count_chercher_medoc) //ne pas incrementer le compteur si pas de reponse
      {
	while ($ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc)) //on cherche toutes les cles de la table 37
	  $medoc[$compteur][]=$ligne_chercher_medoc['inter'];
	$compteur++;
      }
    }
  }
  else //1 seul medoc
  {
    $sql_chercher_medoc="SELECT t37.f1 AS inter FROM t4B INNER JOIN t37 ON t4B.f4=t37.f0 WHERE t4B.f0 = '$cpg_medoc[0]'"; 
    $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
    while ($ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc)) //on cherche toutes les cles de la table 37
      $medoc[]=$ligne_chercher_medoc['inter'];
  }
//interactions
//t4B.f4 = classe d'interactions, pointe sur t37.f0, qui pointe sur t37.f1 (plusieurs valeurs possibles)
//t37.f1 pointe sur t10.f0 et f1 (f1 est la plus grande valeur et f0 la plus petite)
//t10.f5 est le libelle XML de l'interaction
//deux compteurs imbriques pour balayer toutes les combinaisons de medocs
  if ($nbr_medocs > 1) //on ne tente de determiner les interactions que si plus d'un medoc
  {

  $doublons_interactions=array(); //un tableau qui va contenir toutes les interactions differentes
  $display_message=1;
  for ($i=0; $i< $compteur; $i++) //on cherche le premier medoc
  {
    $size=sizeof($medoc[$i]); 
    for ($j=0;$j<$size; $j++) //toutes les occurrences du premier medoc
    {
      $medoc1=$medoc[$i][$j];
      for ($k=$i+1;$k<=$compteur-1;$k++) //On cherche le 2eme medoc en excluant les medocs deja balayes
      {
	$size2=sizeof($medoc[$k]);
	for ($l=0;$l<$size2;$l++) //on cherche les occurrences du 2eme medoc
	{
	  $medoc2=$medoc[$k][$l];
//t10 = les interactions.
//f1 et f2 sont les valeurs a comparer, provenant de t37.f1, provenant elles-memes de t4B.f4
//f5 est le libelle de l'interaction
	  $sql_interaction="SELECT * FROM t10 WHERE f0='".min($medoc1,$medoc2)."' AND f1='".max($medoc1,$medoc2)."'";
	  $resultat_interaction=mysqli_query($db,$sql_interaction);
	  $substances[]="";
	  while ($ligne_interaction=mysqli_fetch_array($resultat_interaction))
	  {
	    if (!in_array($ligne_interaction['ge'],$doublons_interactions)) //Ne pas afficher deux fois une interaction identique
	    {
	      $doublons_interactions[]=$ligne_interaction['ge']; //on ajoute le code d'interactions a la liste des 
	      $display_message=0;
      //t26 : substances et classes
      //f0= code de la classe d'interactions (lie a t4B.f4)
      //f1 = code de la substance
      //f4 = nom de la substance
      //f2 = valeur maxi d'alerte
	      $sql_substance="SELECT f1 FROM t26 WHERE f0='$medoc1' OR f0='$medoc2'";
	      $resultat_substance=mysqli_query($db,$sql_substance);
	      unset($substances);
	      echo '
      <h2>
	Interaction entre ';
	      while ($ligne_substance=mysqli_fetch_array($resultat_substance))
	      {
		$substances[]= $ligne_substance['f1']; //une seule valeur si meme substance
	      }
	      if (count($substances)==2)
		echo $substances[0].' et '.$substances[1];
	      else
		echo "$substances[0] dans les deux produits";
	      echo '
      </h2>';
	      $find=array('&#x3C;','&#x3E;','<BR/>');
	      $replace=array('&amp;lt;','&amp;gt;','\n');
	      $cette_inter_affichable=str_replace ($find,$replace,$ligne_interaction['f5']);

	      $xml=simplexml_load_string($cette_inter_affichable);
	      $nb_rubr=0;
	      foreach ($xml->RUBRIQUE as $value)
	      {
	      $display=0;
		foreach ($xml->RUBRIQUE[$nb_rubr] as $value_ssrubr) //on cherche s'il y a du contenu dans le titre ou les sous-titres pour afficher le titre
		{
		  if ($value_ssrubr['TITRE'])
		  {
		  if ($value_ssrubr[0]!="") 
		    $display=1;
		  }
		}
		if ($value[0]!="" OR $display==1)
		  echo '
      <h3>
	'.$value['TITRE'].'
      </h3>'; //titre premier niveau
		echo '
      '.utf8_decode(str_replace('\n','<br />',$value)); //contenu de premier niveau- on remet les retours chariot
		$nb_ssrubr=0;
		foreach ($xml->RUBRIQUE[$nb_rubr] as $value_ssrubr)
		{
		  if ($value_ssrubr['TITRE'])
		  {
		  if ($value_ssrubr[0]!="") 
		    {
		      if ($value_ssrubr['TITRE'] !="Au dispensateur")
		      {
			$display_contents='yes';
			echo  '
      <h4>
	'.utf8_decode($value_ssrubr['TITRE']).'
      </h4>'; //titre deuxieme niveau
		      }
		    }
		  }
		  if (  $display_contents=='yes')
		  {
		    echo '
      '.utf8_decode(str_replace('\n','<br />',$value_ssrubr)); //contenu de ss rubr
		    $display_contents='no';
		  }
		}
		$nb_rubr++;
	      }
	    //l'explication du niveau de gravite des interactions se trouve dans la table t1G
	      $gravite=$ligne_interaction['f2'];
	      $sql_gravite="SELECT f1 FROM t1G WHERE f0='$gravite'";
	      $resultat_gravite=mysqli_query($db,$sql_gravite);
	      $ligne_gravite=mysqli_fetch_array($resultat_gravite);
	      echo '
      <h3>
	Gravit&eacute; :
      </h3>
	niveau '.$gravite.' = '.$ligne_gravite['f1'];
	    }
	  }
	}
      }
    }
  }
  if ($display_message==1 AND $compteur>0) //on est en mode >1 medoc et pas d'interaction trouvee
    echo "
      <br />Aucune interaction trouv&eacute;e";
}

  if (count($cuv_medoc) == 1) //mode un seul medicament : on affiche toutes les interactions connues
  {
    foreach ($medoc AS $this_medoc) //$medoc est un tableau contenant toutes les sous-classes d'interactions
    {
      $sql_interaction="SELECT * FROM t10 WHERE f0='$this_medoc' OR f1='$this_medoc'"; //on cherche toutes les lignes qui concernent ce medoc, colonne 0 ou 1
      $resultat_interaction=mysqli_query($db,$sql_interaction);
      while ($ligne_interaction=mysqli_fetch_array($resultat_interaction))
      {
	$substance_code0=$ligne_interaction['f0'];
	$substance_code1=$ligne_interaction['f1'];
	if ($this_medoc==$substance_code0)
	  $substance_code=$substance_code1;
	else
	  $substance_code=$substance_code0;

	$sql_substance="SELECT f1 FROM t26 WHERE f0='$substance_code'"; //on cherche le nom du medoc dont on a trouve l'interaction

	$resultat_substance=mysqli_query($db,$sql_substance);
	echo '
      <h2>
	Interaction avec ';
	while ($ligne_substance=mysqli_fetch_array($resultat_substance))
	{
/*	  if (detectUTF8($ligne_substance['f1']))
	      echo utf8_decode($ligne_substance['f1']);
	  else */
	      echo $ligne_substance['f1'];
	  echo '</h2>';
	}
	$find=array('&#x3C;','&#x3E;','<BR/>');
	$replace=array('&amp;lt;','&amp;gt;','\n');
	$cette_inter_affichable=str_replace ($find,$replace,$ligne_interaction['f5']);

	$xml=simplexml_load_string($cette_inter_affichable);
	$nb_rubr=0;
	foreach ($xml->RUBRIQUE as $value)
	{
	$display=0;
	  foreach ($xml->RUBRIQUE[$nb_rubr] as $value_ssrubr) //on cherche s'il y a du contenu dans le titre ou les sous-titres pour afficher le titre
	  {
	    if ($value_ssrubr['TITRE'])
	    {
	      if ($value_ssrubr[0]!="")  //on n'affiche pas le titre si pas de contenu
		$display=1;
	    }
	  }
	  if ($value[0]!="" OR $display==1)
	    echo '
      <h3>
	'.$value['TITRE'].'
      </h3>'; //titre premier niveau
	  echo '
      '.utf8_decode(str_replace('\n','<br />',$value)); //contenu de premier niveau- on remet les retours chariot
	  $nb_ssrubr=0;
	  foreach ($xml->RUBRIQUE[$nb_rubr] as $value_ssrubr)
	  {
	    if ($value_ssrubr['TITRE'])
	    {
	      if ($value_ssrubr[0]!="")  //on n'affiche pas le titre si pas de contenu
	      {
		if ($value_ssrubr['TITRE'] !="Au dispensateur")
		{
		  $display_contents='yes';
		  echo  '
      <h4>
	'.$value_ssrubr['TITRE'].'
      </h4>'; //titre deuxieme niveau
		}
	      }
	    }
	    if ( $display_contents=='yes')
	    {
	      echo '
      '.utf8_decode(str_replace('\n','<br />',$value_ssrubr)); //contenu de ss rubr
	      $display_contents='no';
	    }
	  }
	  $nb_rubr++;
	}
      //le niveau de gravite des interactions se trouve dans la table t1G
	$gravite=$ligne_interaction['f2'];
	$sql_gravite="SELECT f1 FROM t1G WHERE f0='$gravite'";
	$resultat_gravite=mysqli_query($db,$sql_gravite);
	$ligne_gravite=mysqli_fetch_array($resultat_gravite);
	echo '
      <h3>
	Gravit&eacute; :
      </h3>
      niveau '.$gravite.' = '.$ligne_gravite['f1'];
      }
    }
  }
echo "
    </fieldset>";
}
?>
    <div class="notice">
      <strong>Note</strong> : les médicaments, posologies, indications, prix et toute autre information pharmaceutique sont basés sur le Vidal Data Semp.<br />
      Les auteurs du logiciel n'ont aucun lien avec l'industrie pharmaceutique.
    </div>
    <div class="notice">
      En cas d'effet indésirable, prière de faire une déclaration de pharmacovigilance à cette adresse : <a href="https://www.formulaires.modernisation.gouv.fr/gf/cerfa_10011.do">https://www.formulaires.modernisation.gouv.fr/gf/cerfa_10011.do</a>
    </div>
  </div>
<?php
include("inc/footer.php");
?>