<?php
//page utilisee par AJAX pour chercher les intolerances medicamenteuses a ajouter au terrain
//URL de type recherche_drug.php?nom_medoc=chaine&type_requete=DCI&radio_distri=2&tri_par=n&select_type=1
header("Content-Type: text/plain");


$nom_medoc = (isset($_GET["nom_medoc"])) ? $_GET["nom_medoc"] : NULL; //la cle de recherche :nom, DCI, indication ou classe ATC
$type_requete = (isset($_GET["type_requete"])) ? $_GET["type_requete"] : NULL; //recherche par Nom commercial, DCI, Indication ou ATC
$tri_par = (isset($_GET["tri_par"])) ? $_GET["tri_par"] : NULL; //tri par nom ou prix = n ou p
$select_type = (isset($_GET["select_type"])) ? $_GET["select_type"] : NULL; //medicaments, accessoires, dietetique... 1 à 6
$list_select='NOP';
//echo $type_requete;

$basemed="DatasempTest";

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$basemed))
{
echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />";
echo "<div class=\"erreur\">
<b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
</div>";
	exit;
}

function stripAccents($string)
{
  return strtr($string,'àáâãäçèéêëìíîïñòóôõöùúûüýÿÀÁÂÃÄÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝ',
'aaaaaceeeeiiiinooooouuuuyyAAAAACEEEEIIIINOOOOOUUUUY');
}

function prix_boite ($cuv,$db)
{
	      $sql_prix="SELECT f3 FROM t01 WHERE f0='$cuv'"; //Par ucd
	      $resultat_prix=mysqli_query($db,$sql_prix);
	      $ligne_prix=mysqli_fetch_array($resultat_prix);
	      return $ligne_prix['f3'];
}

function dispo_ville_hopital($cuv,$db,$name)
{
//On cherche si commercialise en ville ou hopital
//  
  $radio_distri = (isset($_GET["radio_distri"])) ? $_GET["radio_distri"] : NULL;  //hopitaux ou officines = 2 ou 4
  $data='';
  $sql_ville_hopital="SELECT f2 FROM t02 WHERE f0='$cuv'";
  $resultat_ville_hopital=mysqli_query($db,$sql_ville_hopital);
  $ligne_ville_hopital=mysqli_fetch_array($resultat_ville_hopital);

  if ($ligne_ville_hopital['f2']==$radio_distri) //On ne cherche pas si commercialise si pas dans la bonne rubrique ville-hopital
  {
//On cherche le code de dispo selon le CUV
    $sql_dispo="SELECT f3 FROM t02 WHERE f0='$cuv'";
    $resultat_dispo=mysqli_query($db,$sql_dispo);
    $ligne_dispo=mysqli_fetch_array($resultat_dispo);
    if ($ligne_dispo['f3']=='0') // medicament commercialise seulement
    {
      $data=$name.' Prix : '.prix_boite($cuv,$db).' €'.'_'.$cuv;
    }
  } //fin ville-hopital
  return ($data);
}

if (strlen($nom_medoc) > 2) 
{
  $nom_medoc=stripAccents(urldecode($_REQUEST['nom_medoc']));
  if ($nom_medoc)
    $nom_medoc="%".$nom_medoc."%";
  $data=array(); //initialiation du tableau des medicaments trouves
  $select_type=$_REQUEST['select_type'];
  if ($type_requete=='Nom commercial')
  {
//rechercher le cpg du medoc et sa classe
//t4B=table generale - f0=cpg f2=nom fA= numero de classe
//t42 = table des classes - f0= num de classe, 
/*
    $sql_chercher_medoc="SELECT t4B.f0 AS numero,t4B.f2 AS nom FROM t4B WHERE CAST(t4B.f2 AS CHAR) LIKE '$nom_medoc' ORDER BY t4B.f2"; //classement par nom
    $resultat_chercher_medoc=mysqli_query($db,$sql_chercher_medoc);
    $display_list=0;

    while ($ligne_chercher_medoc=mysqli_fetch_array($resultat_chercher_medoc)) //on cree le lien pour les medocs correspondant a la chaine recherchee
    {
  //on cherche si le medoc est commercialise
      $id_medoc=$ligne_chercher_medoc['numero']; //le cpg
  //on cherche le CUV = code unite de vente
//f1=numero
//fG=nom long
//fK=classement
*/
      $sql_chercher_cuv="SELECT f1,fG FROM t00 WHERE CAST(fG AS CHAR) LIKE '$nom_medoc' AND fK LIKE '$select_type%'"; //on renvoie fG dans la liste (libelle complet)
      $resultat_chercher_cuv=mysqli_query($db,$sql_chercher_cuv); //Plusieurs cuv possibles pour 1 cip

      while($ligne_chercher_cuv=mysqli_fetch_array($resultat_chercher_cuv)) //on balaye toutes les cuv pour trouver celles qui sont commercialisees
      {
	$cuv=$ligne_chercher_cuv['f1'];
	$libelle_complet=$ligne_chercher_cuv['fG'];

	if ($data=dispo_ville_hopital($cuv,$db,$libelle_complet))
	{
	  if ($list_select=='NOP')
	    $list_select='';
	  $list_select.='|'.$data;
	}
      }
//    }
  } //fin nom commercial
  elseif ($type_requete=='DCI') 
  {
    $nom_medoc="%".$_REQUEST['nom_medoc']."%"; //On ajoute les jokers 
//T54.f2= nom substance, t54.f0 = numero substance
//t07.f0 =t00.f0 = cpg 
      $sql_recherche_par_substance="
SELECT t54.f0 AS num_subst,
t00.fG AS nom_medoc,
t00.f0 AS num_medoc ,
t00.f1 AS cuv_medoc 
FROM t54 
INNER JOIN t07 ON t54.f0=t07.f2 
INNER JOIN t00 ON t07.f0=t00.f0 
WHERE CAST(t54.f2 AS CHAR) LIKE '$nom_medoc' 
AND t54.f1='FRA'  
AND fK LIKE '$select_type%' 
ORDER BY t00.f3"; //On supprime la recherche par DCI anglaise
      $resultat_recherche_par_substance=mysqli_query($db,$sql_recherche_par_substance); 
      $list_select='';
      while ($ligne_recherche_par_substance=mysqli_fetch_array($resultat_recherche_par_substance))
      {
	$cuv=$ligne_recherche_par_substance['cuv_medoc'];//code unite de vente
	$this_line=$ligne_recherche_par_substance['nom_medoc'];
//	  echo $cuv;
//On cherche si commercialise en ville ou hopital
	if ($data=dispo_ville_hopital($cuv,$db,$this_line))
	{
	  if ($list_select=='NOP')
	    $list_select='';
	  $list_select.='|'.$data;
	}
      }
    } //fin DCI
    elseif ($type_requete=='Indication')
    {
      if ($_REQUEST['nom_medoc']!='')
      {
	$text_indication='%'.addslashes($_REQUEST['nom_medoc']).'%';
      }
/*
t52 = etats pathologiques selon produit
  f0 = code du terme ->t50.f0 (glossaire) et ->t55.f0 (liaison CIM-X)
  f1 = CIP ->t00.f0
  f2 = nature du lien (contre-indique...) -> t1C.f0 (liste des codes) ou t51.f2
  f3 = frequence
  f5 = libelle en clair de l'effet
  f6 = code document ->t45.f0
*/
    $sql_cip_indication="SELECT t52.f1 AS cip, t00.fG AS libelle, t00.f1 AS cuv FROM t52 , t50 , t00  WHERE t52.f2='2994' AND Lower(t50.f1) LIKE Lower('$text_indication') AND t52.f1=t00.f0 AND t52.f0=t50.f0 AND fK LIKE '$select_type%' ORDER BY t00.f3";
    $resultat_cip_indication=mysqli_query($db,$sql_cip_indication);
    $text_indication=$_REQUEST['text_indication'];
    while ($ligne_cip_indication=mysqli_fetch_array($resultat_cip_indication))
    {
      $cuv=$ligne_cip_indication['cuv'];//code unite de vente
      $this_line=$ligne_cip_indication['libelle'];

      if ($data=dispo_ville_hopital($cuv,$db,$this_line))
      {
	if ($list_select=='NOP')
	  $list_select='';
	$list_select.='|'.$data;
      }
    }
  } //fin indication
  elseif ($type_requete=='ATC')
  {
    $text_atc=$_REQUEST['nom_medoc'].'%';
    $sql_cip_atc="SELECT f0,f2 FROM t4B WHERE fA LIKE '$text_atc' ORDER BY f2";

    $resultat_cip_atc=mysqli_query($db,$sql_cip_atc);
    while ($ligne_cip_atc=mysqli_fetch_array($resultat_cip_atc))
    {
//recherche du CUV
      $cip=$ligne_cip_atc['f0'];
      $sql_cuv="SELECT t00.f1 AS cuv t00.fG AS libelle_complet FROM t00 WHERE f0='$cip' AND fK LIKE '$select_type%'";
      $resultat_cuv=mysqli_query($db,$sql_cuv);
      $ligne_cuv=mysqli_fetch_array($resultat_cuv);
      $cuv=$ligne_cuv['cuv'];
      $libelle_complet=$ligne_cuv['libelle_complet'];
//recherche ville-hopital
      if ($data=dispo_ville_hopital($cuv,$db,$libelle_complet))
      {
	if ($list_select=='NOP')
	  $list_select='';
	$list_select.='|'.$data;
      }
    }
  } //fin ATC
}

echo $list_select;
?>