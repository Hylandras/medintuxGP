<?php
session_start() ;
include("config.php");

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header ('location: index.php?page=liste' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']); //attention chrome pour android ajoute des espaces a la fin
$user=$tab_login[0];

//redirige sur la liste si le choix d'un patient n'est pas renseigne
if (!$_GET['GUID'])
{
  header('location: liste.php' );
  exit;
}
else //On recupere l'identifiant du patient envoye par l'URL
  $patient=$_GET['GUID'];

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; $host<br />";
  exit;
}
//echo "test";

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
//on cherche les droits de l'utilisateur
$sql_chercher_droits="SELECT GUID,Droits,Qualite FROM Personnes WHERE Login='$user'";
//echo $sql_chercher_droits;
$resultat_chercher_droits=mysqli_query($db,$sql_chercher_droits);
$ligne_chercher_droits=mysqli_fetch_array($resultat_chercher_droits);
$GUID_user=$ligne_chercher_droits['GUID'];
//echo $GUID_user;
$droits_user=$ligne_chercher_droits['Droits'];
$qualite_user=$ligne_chercher_droits['Qualite'];

$lectureOK="";
$writeOK="";

include("inc/header.php");

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

?>
    <title>
      Tableaux patient pour MedWebTux
    </title>

    <script type="text/javascript" src="oXHR.js">
    </script>

<?php
include 'calendar_javascript.php';
?>
    <script type="text/javascript">
//<![CDATA[
function showButton(name)
{
  document.getElementById(name).style.display = 'block';
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function options_genre(number,type) 
{
//Pour remplir le deroulant des types d'antecedents
//number = numero du terrain
//type=intolerance, atcd ou text

  var famille;
  var select_family;
  if (type=='text')
    select_family='select_family'+number;
  else if (type=='atcd')
    select_family='select_family_atcd'+number;
  else if (type=='intolerance')
    select_family='select_family_intolerance'+number;

  var form_terrain;
  form_terrain='form_terrain'+number;
  var select_genre;
  if (type=='text')
    select_genre='select_genre'+number;
  else if (type=='atcd')
    select_genre='select_genre_atcd'+number;
  else if (type=='intolerance')
    select_genre='select_genre_intolerance'+number;

  famille=document.getElementById(select_family).value;

<?php
foreach($atcd_types AS $this_type=>$un_type)
{
  echo 'if (famille=="'.$this_type.'")
  {
    list = new Array(""';
  foreach ($atcd_types[$this_type] AS $this_genre)
  {
    echo ',"'.$this_genre.'"'; //les options de genre
  }
  echo ')
  }
';
}
?>
//pour vider les listes d'options secondaires avant remplissage
  if (type=='text')
  {
    var select = document.forms[form_terrain].select_genre;
  }
  else if (type=='atcd')
  {
    var select = document.forms[form_terrain].select_genre_atcd;
  }
  else if (type=='intolerance')
  {
    var select = document.forms[form_terrain].select_genre_intolerance;
  }
  while(0 < select.length)
  {
	  select.options[0] = null;
  }

  for (var i in list) //creation d'une option de menu pour tous les utilisateurs signataires
  {
    var listElement=list[i];
    if (type=='text')
    {
    document.forms[form_terrain].select_genre.options[document.forms[form_terrain].select_genre.options.length] = new Option(listElement,listElement); 
    }
    else if (type=='atcd')
      document.forms[form_terrain].select_genre_atcd.options[document.forms[form_terrain].select_genre_atcd.options.length] = new Option(listElement,listElement); 
    else if (type=='intolerance')
      document.forms[form_terrain].select_genre_intolerance.options[document.forms[form_terrain].select_genre_intolerance.options.length] = new Option(listElement,listElement); 
  }
} //fin fonction option genre
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function request(callback,num) {
//AJAX pour chercher les intolerances medicamenteuses
	var xhr = getXMLHttpRequest();
	
	xhr.onreadystatechange = function() {
		if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 0)) {
			callback(xhr.responseText);
		}
	};
	var substance = encodeURIComponent(document.getElementById("text_chercher_intolerance"+num).value);
//on envoie a la page de recherche le nom de la substance
	xhr.open("GET", "recherche_intolerances.php?substance=" + substance+"&num="+num, true);
	xhr.send(null);
}

function readData(sData) 
{
  donnees=sData.split('_'); 
  num_long=donnees[1];
  num_court=num_long.replace('\n',''); //On nettoie le retour de recherche intolerance
  form_terrain='form_terrain'+num_court;
  select_intolerance='select_intolerance'+num_court;
   //effacer l'ancien menu deroulant
  document.forms[form_terrain].select_intolerance.length=0;
//  if (donnees[0]!="NOP")
  if (donnees[0].indexOf("|") !== -1) //pas d'affichage si pas de reponse renvoyee par ajax
  {
    substances=donnees[0].split('|'); 
    for (var i in substances) //creation d'une option de menu pour toutes les substances
    {
      var substance=substances[i];
      document.forms[form_terrain].select_intolerance.options[document.forms[form_terrain].select_intolerance.options.length] = new Option(substance,substance); 
    }
  }
}
//]]>
    </script>
    <script type="text/javascript">
//<![CDATA[
function request_atcd(callback,num) {
//AJAX pour chercher les antecedents pathologiques pour le terrain
	var xhr = getXMLHttpRequest();
	
	xhr.onreadystatechange = function() {
		if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 0)) {
			callback(xhr.responseText);
		}
	};
	var atcd = encodeURIComponent(document.getElementById("text_chercher_atcd"+num).value);
//on envoie a la page de recherche le nom de l'atcd

	xhr.open("GET", "recherche_atcd.php?atcd=" + atcd+"&num="+num, true);
	xhr.send(null);
}

function readData_atcd(sData) 
{
  donnees=sData.split('_'); 
  num_long=donnees[1];
  num_court=num_long.replace('\n',''); //On nettoie le retour de recherche intolerance
  form_terrain='form_terrain'+num_court;
  select_atcd='select_atcd'+num_court;
   //effacer l'ancien menu deroulant
 document.forms[form_terrain].select_atcd.length=0;
// if (sData!="NOP")
  if (donnees[0].indexOf("|") !== -1) //pas d'affichage si pas de reponse renvoyee par ajax
  {
    atcds=donnees[0].split('|'); //les utilisateurs signataires renvoyes par la page php, + l'utilisateur en premier
    for (var i in atcds) //creation d'une option de menu pour tous les utilisateurs signataires
    {
      var atcd=atcds[i];
      document.forms[form_terrain].select_atcd.options[document.forms[form_terrain].select_atcd.options.length] = new Option(atcd,atcd); 
    }
  }
}
//]]>
    </script>

  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">

<?php

//On cherche dans la table IndexNomPrenom la fiche dont le nom a ete demande
$sql="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";

$resultat=mysqli_query($db,$sql);
$ligne=mysqli_fetch_array($resultat);
$nom=$ligne["FchGnrl_NomDos"];
$prenom=$ligne["FchGnrl_Prenom"];
// OK echo $prenom;

//On cherche dans la table fchpat la fiche dont le numero a ete demande
$sql2="SELECT FchPat_Sexe FROM fchpat WHERE FchPat_GUID_Doss='$patient'";
$resultat2=mysqli_query($db,$sql2);
$ligne2=mysqli_fetch_array($resultat2);
$naissance='';
$adresse='';
$secu='';
$profession='';
$sexe=$ligne2['FchPat_Sexe'];
$titre='';
$genre[$sexe]='';
$date='';
//echo $sexe;
include("templates.php"); //ne pas inclure avant de renseigner nom et prenom

//On cherche les documents du patient
$resultat_global=mysqli_query($db,"SELECT * FROM RubriquesHead WHERE RbDate_IDDos='$patient' ORDER BY RbDate_Date DESC");


if (isset($_GET['affichage']))
  $affichage=$_GET['affichage']; //= Montrer
else
  $affichage="";

//redirige sur la liste si le choix d'un patient n'est pas renseigne
if (!$_GET['GUID'])
{
  header('location: liste.php' );
  exit;
}
else //On recupere l'identifiant du patient envoye par l'URL
  $patient=$_GET['GUID'];

$critere_recherche='';
if (isset($_GET['critere_recherche']))
  $critere_recherche=$_GET['critere_recherche'];

$cle='';
if (isset($_GET['cle']))
{
  $cle=$_GET['cle'];
}

$mode_recherche='';
if (isset($_GET['envoyer_cle']))
  $mode_recherche=1;

$rubriques_presentes=array ();
mysqli_data_seek($resultat_global,0) ; //remise a zero tableau requete

while ($ligne_global=mysqli_fetch_array($resultat_global))
{
  if (!in_array ($ligne_global['RbDate_TypeRub'],$rubriques_presentes))
  {
    $rubriques_presentes[]=$ligne_global['RbDate_TypeRub'];
  }
}

if ($affichage=="listes" or $mode_recherche)
{
  if (isset ($rubrique_perso))
  {
      $compteur_types=0;
      foreach ($rubrique_perso AS $this_rubrique_perso)
      {
	$compteur_types++;
      }

    for ($i=0;$i< $compteur_types;$i++)
    {
      $this_rubrique_perso=explode('|',$rubrique_perso[$i]);
      $doc_perso[$this_rubrique_perso[2]]="";
    }
  }

  mysqli_data_seek($resultat_global,0) ; //remise a zero tableau requete de tous les documents
  while ($ligne_global=mysqli_fetch_array($resultat_global))
  {
    $date_courte=substr($ligne_global["RbDate_Date"],0,10);
    $date_bon_ordre=iso_to_local($date_courte,$date_format);

 //valeurs personnalisees et standard
    foreach ($rubrique_perso AS $this_rubrique_perso)
    {
      $tableau_rubrique_perso=explode ('|',$this_rubrique_perso);
      $type_num=$tableau_rubrique_perso[2];
      if ($ligne_global["RbDate_TypeRub"]==$type_num)
      {
	if ($critere_recherche) 
	{
	  if ($critere_recherche==$type_num)
	  {
	    $primkey=$ligne_global["RbDate_RefBlobs_PrimKey"];
echo $primkey;
	    $resultat_chercher_contenu=mysqli_query($db,"SELECT RbDate_DataRub FROM RubriquesBlobs WHERE RbDate_PrimKey LIKE '$primkey'");
	    $ligne_chercher_contenu=mysqli_fetch_array($resultat_chercher_contenu);
	    if (stripos($ligne_chercher_contenu['RbDate_DataRub'],$cle))
	    {
	      $doc_perso[$type_num]=$doc_perso[$type_num]."<a href=\"consultation.php?numeroID=".$patient."&amp;date=".$date_courte."&amp;nom=".$nom."&amp;prenom=".$prenom."\">".$date_bon_ordre.'</a> '.$ligne_global["RbDate_NomDate"].'<br />';
	    }
	  }
	}
	else //mode sans filtre de recherche
	{
	  $doc_perso[$type_num]=$doc_perso[$type_num]."<a href=\"consultation.php?numeroID=".$patient."&amp;date=".$date_courte."&amp;nom=".$nom."&amp;prenom=".$prenom."\">".$date_bon_ordre.'</a> '.$ligne_global["RbDate_NomDate"].'<br />';
	}
      }
    }
  }

?>
	  <table>
	    <!-- table du bas (2eme niveau) en mode listes -->
	      <tr>
<?php 
  if ($affichage=="listes")
  {
?>
		<th class="fond_th">
		  Honoraires
		</th>
<?php
  }
//on ajoute les colonnes personnalisees si besoin
//recherche si champs personnalises

  if (isset ($rubrique_perso))
  {
      $compteur_types=0;
      foreach ($rubrique_perso AS $this_rubrique_perso)
      {
	$compteur_types++;
      }
    if ($affichage=="listes") 
    {
      for ($i=0;$i< $compteur_types;$i++)
      {
	$this_rubrique_perso=explode('|',$rubrique_perso[$i]);
	if (in_array($this_rubrique_perso[2],$rubriques_presentes))
	{
?>
		<th class="fond_th">
		  <?php echo $this_rubrique_perso[0] ?>
		</th>
<?php
	}
      }
    }
    else //mode cle de recherche
    {
      foreach ($rubrique_perso AS $this_rubrique_perso)
      {
	$rubriques=explode('|',$this_rubrique_perso);
	if (in_array($critere_recherche,$rubriques))
	{
	  echo "
		<th class=\"fond_th\">
		  $rubriques[0] contenant $cle
		</th>";
	}
      }
    }
  }
?>
	      </tr>
	    <!--affichage de la liste des contacts avec ce patient-->
	      <tr>
<?php
  if ($affichage=="listes") //pas les  honoraires en mode recherche
  {
    echo "
		<td style='vertical-align:top' class=\"fond_td\">";
// Liste des honoraires

    $connect=mysqli_select_db($db,$base_compta);

    $sql_chercher_hono="SELECT * FROM honoraires WHERE GUID='$patient' ORDER BY date DESC";
    $resultat_chercher_hono=mysqli_query($db,$sql_chercher_hono);
    $count_chercher_hono=mysqli_num_rows($resultat_chercher_hono);
    if ($count_chercher_hono)
    {
	while ($ligne_chercher_hono=mysqli_fetch_array($resultat_chercher_hono)):
	  echo '<a href="compta/saisie_hono.php?bouton_modifier_hono='.$ligne_chercher_hono['id_hono'].'">'.iso_to_local($ligne_chercher_hono['date'],$date_format)."</a> - ";
	  if ($ligne_chercher_hono['esp']) echo $ligne_chercher_hono['esp']." (esp)<br />";
	  if ($ligne_chercher_hono['chq']) echo $ligne_chercher_hono['chq']." (ch)<br />";
	  if ($ligne_chercher_hono['cb']) echo $ligne_chercher_hono['cb']." (cb)<br />";
	  if ($ligne_chercher_hono['daf']) echo $ligne_chercher_hono['daf']." (DAF)<br />";
	  if ($ligne_chercher_hono['autre']) echo $ligne_chercher_hono['autre']." (autre)<br />";
	  if ($ligne_chercher_hono['du']) echo $ligne_chercher_hono['du']." (dû)<br />";
	endwhile;
    }
?>
		</td>
<?php
  }
//recherche si champs personnalises
//le tableau en mode liste'

  if (isset ($rubrique_perso))
  {
    for ($i=0;$i< $compteur_types;$i++)
    {
      $this_rubrique_perso=explode('|',$rubrique_perso[$i]);
      $type_num=$this_rubrique_perso[2];
      if ($doc_perso[$type_num])
      {
?>
		  <td valign="top" align="left" class="fond_td">
<?php 
      echo "
		  ".$doc_perso[$type_num];
?>
		    </td>
<?php
      }
    }
  }
?>
	      </tr>
	    </table>
	
  <!-- fin de la table en mode listes -->
<?php
} //fin du if affichage==listes
elseif ($affichage=="tableau")
{
//on initialise les variables personnalisees si besoin
//recherche si champs personnalises
  if (isset ($rubrique_perso))
  {
    $compteur_types=0;
    foreach ($rubrique_perso AS $this_rubrique_perso)
    {
      $compteur_types++;
    }
    for ($i=0;$i< $compteur_types;$i++)
    {
      $this_rubrique_perso=explode('|',$rubrique_perso[$i]);
      $doc_perso[$this_rubrique_perso[2]]="";
    }
  }
 $date_unique= array ();
?>
	  <table>
	    <tr>
	      <th class="fond_th">
		Date
	      </th>
<?php
  mysqli_data_seek($resultat_global,0) ; //remise a zero tableau requete

  while ($ligne_global=mysqli_fetch_array($resultat_global))
  {
    $date_courte=substr($ligne_global["RbDate_Date"],0,10); //format iso_to_local
    $date_bon_ordre=iso_to_local($date_courte,$date_format);

//On cree un tableau des dates uniques
    if (!in_array ($date_bon_ordre,$date_unique))
    {
      $date_unique[]=$date_bon_ordre;
    }
//ajouter ici les valeurs personnalisees
    if (isset ($rubrique_perso))
    {
      $compteur_types=0;
      foreach ($rubrique_perso AS $this_rubrique_perso)
      {
	$tableau_rubrique_perso=explode ('|',$this_rubrique_perso);
	$type_num=$tableau_rubrique_perso[2];
	if ($ligne_global["RbDate_TypeRub"]==$type_num)
	{
	  $doc_perso[$type_num]=$doc_perso[$type_num]."<a href=\"consultation.php?numeroID=".$patient."&amp;date=".$date_courte."&amp;nom=".$nom."&amp;prenom=".$prenom."\">".$date_bon_ordre.'</a> '.$ligne_global["RbDate_NomDate"].'<br />';
	  $rubrique[$type_num][$date_bon_ordre][]=$ligne_global["RbDate_NomDate"];
	}
	$compteur_types++;
      }
    }
  }  
//on ajoute les colonnes personnalisees si besoin
//recherche si champs personnalises

  if (isset ($rubrique_perso))
  {
    for ($i=0;$i< $compteur_types;$i++)
    {
      $this_rubrique_perso=explode('|',$rubrique_perso[$i]);
      if (in_array($this_rubrique_perso[2],$rubriques_presentes))
      {
  ?>
		<th class="fond_th">
		  <?php echo $this_rubrique_perso[0] ?>
		</th>
  <?php
      }
    }
  }

?>
	      </tr>

<?php
  foreach ($date_unique AS $this_date)
  {
	if ($date_format=='fr')
	  $date_iso=substr($this_date,6,4)."-".substr($this_date,3,2)."-".substr($this_date,0,2);
	elseif ($date_iso=='en')
	  $date_iso=substr($this_date,6,4)."-".substr($this_date,0,2)."-".substr($this_date,3,2);  
	else 
	  $date_iso=$this_date;
?>
	      <tr class="test">
		<td class="fond_td">
		  <a href="consultation.php?numeroID=<?php echo $patient ?>&amp;date=<?php echo $date_iso ?>"><?php echo $this_date ?></a> 
		</td>
<?php
//recherche si champs personnalises
      if (isset ($rubrique_perso))
      {
	foreach ($rubrique_perso AS $this_rubrique_perso)
	{
	  $tableau_rubrique_perso=explode('|',$this_rubrique_perso);
	  $this_rubrique_presente=$tableau_rubrique_perso[2];
	  if (isset($rubrique[$this_rubrique_presente][$this_date]))
	  {
?>
		<td class="fond_td">
		  <?php foreach ($rubrique[$this_rubrique_presente][$this_date] AS $this_title) echo $this_title.'<br />'  ?>
		</td>
<?php
	  }
	  else
	  {
	    if ($doc_perso[$this_rubrique_presente])
	    {
	      echo '
		<td>
		</td>';
	    }
	  }
	}
      }
?>
	      </tr>
<?php
   }
?>
	    </table>

      <!-- fin de la table du bas en mode tableur -->


<?php
}//fin affichage tableau

elseif ($affichage=="consultations")
{
  $sql_consultations="SELECT * FROM RubriquesHead INNER JOIN RubriquesBlobs ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesHead.RbDate_IDDOs='$patient' AND (RubriquesHead.RbDate_TypeRub='20030000' OR RubriquesHead.RbDate_TypeRub='20030001') ORDER BY RbDate_Date";
  $resultat_consultations=mysqli_query($db,$sql_consultations);	
  $expr="`(<body[^>]*>)(.*)(<\/body>)`is";
?>

	   <table>
	      <tr>
		<th class="fond_th">
		  Date
		</th>
		<th class="fond_th">
		  Titre
		</th>
		<th class="fond_th">
		  Consultations
		</th>
	      </tr>
<?php
  while ($ligne_consultations=mysqli_fetch_array($resultat_consultations))//chercher les observations
  {
    $login_possesseur_observation=$ligne_consultations['RbDate_CreateSignUser'];
    $id_document=$ligne_consultations['RbDate_PrimKey'];
    $sql_recherche_possesseur="SELECT GUID FROM Personnes WHERE Login='$login_possesseur_observation'";
    $resultat_recherche_possesseur=mysqli_query($db,$sql_recherche_possesseur);
    $ligne_recherche_possesseur=mysqli_fetch_array($resultat_recherche_possesseur);
    $GUID_possesseur_observation=$ligne_recherche_possesseur['GUID'];

    if ($login_possesseur_observation==$user) //On verifie si l'observation appartient a l'utilisateur
    {
      if (stripos($droits_user,"obv"))//si oui, on verifie qu'il a les droits
      {
	$lectureOK=1;
      }
      else
	$lectureOK=0;
      if (stripos($droits_user,"obc"))//si oui, on verifie qu'il a les droits
      {
	$writeOK=1;
      }
      else
	$writeOK=0;
      if (stripos($droits_user,"obp"))//si oui, on verifie qu'il a les droits
      {
	$printOK=1;
      }
      else
	$printOK=0;
    }
    else //sinon on verifie qu'il est utilisateur autorise
    {
      $sql_utilisateur_autorise="SELECT * FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_possesseur_observation'";
      $resultat_utilisateur_autorise=mysqli_query($db,$sql_utilisateur_autorise);
      $count_utilisateur_autorise=mysqli_num_rows($resultat_utilisateur_autorise);
      if ($count_utilisateur_autorise)//S'il existe un utilisateur autorise
      {
	$liste_utilisateur_autorise=mysqli_fetch_array($resultat_utilisateur_autorise);
	if (stripos($liste_utilisateur_autorise['FriendUserDroits'],"obv"))//On verifie qu'il a les droits
	  $lectureOK=1;
	else
	  $lectureOK=0;
	if (stripos($liste_utilisateur_autorise['FriendUserDroits'],"obc"))//On verifie qu'il a les droits
	  $writeOK=1;
	else
	  $writeOK=0;
      }
    }
    echo "
	      <tr>";
    $date_courte=substr($ligne_consultations['RbDate_Date'],0,10);
    $date_bon_ordre=iso_to_local($date_courte,$date_format);
    echo "
		<td valign=\"top\" class=\"fond_td\">
		  <a href=\"consultation.php?numeroID=$patient&date=$date_courte\">$date_bon_ordre</a>
		</td>
		<td valign=\"top\" class=\"fond_td\">
		  ".$ligne_consultations['RbDate_NomDate']."
		</td>";
    $fiche_mieux=(preg_match($expr,$ligne_consultations["RbDate_DataRub"],$tab));
    $fiche_encore_mieux=preg_replace("`<body.*>`i", " ",$tab[2]);
    if ($lectureOK)
    {
      echo "
		<td class=\"fond_td\">
		  ".$fiche_encore_mieux;
?>
		  <table>
	      <!-- table 3eme niveau pour les boutons imprimer et modifier document -->
		    <tr>
		      <td>
			<form action="consultation.php" method="post">
			  <div>
			    <input name="observation_ID" type="hidden" value="<?php echo $ligne_consultations['RbDate_PrimKey'] ?>" />
			    <input name="date" type="hidden" value="<?php echo $date_courte ?>" />
			    <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
<?php
      if ($writeOK)
      {
?>
			    <input name="nouveau" type="submit" value="Modifier" />
<?php
      }
?>
			  </div>
			</form>
		      </td>
		      <td>
<?php //pas de bouton imprimer si pas les droits
if ($printOK==1)
{
?>
			<form action="print.php" method="get">
			  <div>
			  <input name="id_document" type="hidden" value="<?php echo $id_document ?>" />
			  <input name="nouveau" type="submit" value="Imprimer" />
			  <label for="radio_header">Ajouter un en-tête</label>
			  <input name="radio_header" id="radio_header" value="radio_header" type="radio" checked="checked" /><br />
			  <label for="radio_empty_header">Ajouter l'emplacement d'un en-tête</label>
			  <input name="radio_header" id="radio_empty_header" value="radio_empty_header"  type="radio" /><br />
			  <label for="radio_no_header">Imprimer pleine page sans en-tête</label>
			  <input name="radio_header" id="radio_no_header" value="radio_no_header"  type="radio" />
			  </div>
			</form>
<?php
}
?>
		      </td>
		  </tr>
	    </table>

<!-- fin table 3eme niveau boutons imprimer -->
<?php
    }
    else
    {
      echo "
		<td align=\"left\" style=\"background:#ECFFEA;\">
		      Acc&egrave;s r&eacute;serv&eacute; &agrave; ".$login_possesseur_observation;
    }
?>
		</td>		
	      </tr>

<?php
  } //fin du while
?>
	    </table> 
<!--Fin du tableau des consultations-->
<?php
}//fin du mode consultations
elseif ($affichage=="terrains")
{
  list ($jour_debut,$mois_debut,$annee_debut)=explode ("-",date('d-m-Y', date('U')));
  $heure=date(date ('H:i:s',date('U')));
  $maintenant=$annee_debut.'-'.$mois_debut.'-'.$jour_debut."T".$heure;

?>
<div class="information">
Seul le terrain le plus récent est pris en compte dans les contrôles d'ordonnances médicamenteuses.
</div>
	   <table>	<!-- table encadrement terrains-->
	      <tr>
		<th class="fond_th">
		  Date
		</th>
		<th class="fond_th">
		  Titre
		</th>
		<th class="fond_th">
		  Terrains
		</th>
	      </tr>
<?php
  $expr="`(<body[^>]*>)(.*)(<\/body>)`i";//nettoyage des balises des ordonnances
  $exp_date="`([0-9]{4}-[0-9]{2}-[0-9]{2})`";//rechercher si ligne contient une date
	
  $sql3="SELECT * FROM RubriquesHead INNER JOIN RubriquesBlobs ON RubriquesHead.RbDate_RefBlobs_PrimKey=RubriquesBlobs.RbDate_PrimKey WHERE RubriquesHead.RbDate_IDDos='$patient' AND RbDate_TypeRub='20060000' ORDER BY RbDate_Date DESC";
  $resultat3=mysqli_query($db,$sql3);
  $count3=mysqli_num_rows($resultat3);
  if($count3) //Presence d'un terrain
  {
    $i=0;
    while ($ligne3=mysqli_fetch_array($resultat3)) //On cherche les differents terrains
    {
      $i++;
//Pour trouver l'ID du terrain pou rdebug
//echo $ligne3['RbDate_RefBlobs_PrimKey'];

//On coupe la fin du terrain pour eviter l'affichage des caracteres xml
      $expr2="`\<\?xml(.*)`i";
      $date_courte=substr($ligne3['RbDate_Date'],0,10);
      $date_bon_ordre=iso_to_local($date_courte,$date_format);
      $titre=$ligne3['RbDate_NomDate'];
      echo "
	      <tr>
		<td valign=\"top\" class=\"fond_td\">
		  ".$date_bon_ordre."
		</td>
		<td valign=\"top\" class=\"fond_td\">
		  ".$titre."
		</td>
		<td align=\"left\" valign=\"top\" class=\"fond_td\"><!-- debut colonne terrain -->";
?> 

		  <form action="validation_terrain.php" id="form_terrain<?php echo $i ?>" method="post">
<?php

      $terrain=$ligne3['RbDate_DataRub'];
//echo $terrain;
      $terrain=str_replace("\r","\n",$terrain);

      $terrain=preg_replace($expr2,"",$terrain);
      $split_terrain=explode("\n", $terrain);//tableau dont chaque valeur est une ligne du terrain
      $fin_ligne=''; //reperes pour terminer les tableaux de valeurs
      $fin_table='';
      $fin_titre='';
      $atcd='';
      $num_tableau=0; //le compteur de tableaux de variables pour servir de nom aux zones de saisie
      $check=0;
      foreach ($split_terrain as $ligne_brut)
      {
        if (preg_match("`\[Ant`",$ligne_brut))//detecter le titre ATCD
        {//affichage du titre sans les crochets.
?>
		    <p>
		      <strong>Ant&eacute;c&eacute;dents</strong>
		    </p>
		    <div>
		      <label for="text_chercher_intolerance<?php echo $i ?>">
			Intolérance médicamenteuse :
		      </label>
		      <input name="text_chercher_intolerance" id="text_chercher_intolerance<?php echo $i ?>" type="text" onkeyup="request(readData,<?php echo $i ?>);" />
		      <input name="button_chercher_intolerance" type="button" value="chercher"  onclick="request(readData,<?php echo $i ?>);"/>
		      <select name="select_intolerance" id="select_intolerance<?php echo $i ?>" onchange="showButton('hide_intolerance_details<?php echo $i ?>')">
		      <option></option>
		      </select>
		      <div id="hide_intolerance_details<?php echo $i ?>" style="display:none">
			<label for="select_family_intolerance<?php echo $i ?>">
			  Famille&nbsp;: 
			</label>
			<select name="select_family_intolerance" id="select_family_intolerance<?php echo $i ?>" onchange="options_genre(<?php echo $i ?>,'intolerance')">
			  <option></option>
<?php
	    foreach($atcd_types AS $this_type=>$un_type)
	    {
	      echo '
			  <option value="'.$this_type.'">'.$this_type.'</option>';
	    }
?>
			</select>
			<label for="select_genre_intolerance<?php echo $i ?>">
			  Genre&nbsp;: 
			</label>
			<select name="select_genre_intolerance" id="select_genre_intolerance<?php echo $i ?>">
			  <option></option>
			</select>
			ALD
			<label for="intolerance_ald_yes<?php echo $i ?>">
			  Oui
			</label>
			<input type="radio" value="yes" name="intolerance_ald" id="intolerance_ald_yes<?php echo $i ?>"/>
			<label for="intolerance_ald_no<?php echo $i ?>">
			  Non
			</label>
			<input type="radio" value="no" name="intolerance_ald" id="intolerance_ald_no<?php echo $i ?>"/>
			Actif
			<label for="intolerance_active_yes<?php echo $i ?>">
			  Oui
			</label>
			<input type="radio" value="yes" name="intolerance_active" id="intolerance_active_yes<?php echo $i ?>"/>
			<label for="intolerance_active_no<?php echo $i ?>">
			  Non
			</label>
			<input type="radio" value="no" name="intolerance_active" id="intolerance_active_no<?php echo $i ?>"/>
			<label for="comment_intolerance<?php echo $i ?>">
			  Commentaire
			</label>
			<input name="comment_intolerance" id="comment_intolerance<?php echo $i ?>" type="text" />
			<label for="date_intolerance<?php echo $i ?>">
			  Date
			</label>
			<input name="date_intolerance" id="date_intolerance<?php echo $i ?>" type="text" size="10" /><input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(this.form.date_intolerance<?php echo $i ?>);" /><br />
		      </div>
		    </div>
		    <div>
		      <label for="text_chercher_atcd<?php echo $i ?>">
			Antécédents pathologiques
		      </label>
		      <input name="text_chercher_atcd" id="text_chercher_atcd<?php echo $i ?>" type="text" onkeyup="request_atcd(readData_atcd,<?php echo $i ?>);" />
		      <input name="button_chercher_atcd" type="button" value="chercher"  onclick="request_atcd(readData_atcd,<?php echo $i ?>);"/>
		      <select name="select_atcd" id="select_atcd<?php echo $i ?>"  onchange="showButton('hide_atcd_details<?php echo $i ?>')">
		      <option></option>
		      </select>
		      <div id="hide_atcd_details<?php echo $i ?>" style="display:none">
			<label for="select_family_atcd<?php echo $i ?>">
			  Famille&nbsp;: 
			</label>
			<select name="select_family_atcd" id="select_family_atcd<?php echo $i ?>" onchange="options_genre('<?php echo $i ?>','atcd');">
			  <option></option>
<?php
	    foreach($atcd_types AS $this_type=>$un_type)
	    {
	      echo '
			  <option value="'.$this_type.'">'.$this_type.'</option>';
	    }
?>
			</select>
			<label for="select_genre_atcd<?php echo $i ?>">
			  Genre&nbsp;: 
			</label>
			<select name="select_genre_atcd" id="select_genre_atcd<?php echo $i ?>">
			  <option></option>
			</select>
			ALD
			<label for="atcd_ald_yes<?php echo $i ?>">
			  Oui
			</label>
			<input type="radio" value="yes" name="atcd_ald" id="atcd_ald_yes<?php echo $i ?>"/>
			<label for="atcd_ald_no<?php echo $i ?>">
			  Non
			</label>
			<input type="radio" value="no" name="atcd_ald" id="atcd_ald_no<?php echo $i ?>"/>
			Actif
			<label for="atcd_active_yes<?php echo $i ?>">
			  Oui
			</label>
			<input type="radio" value="yes" name="atcd_active" id="atcd_active_yes<?php echo $i ?>"/>
			<label for="atcd_active_no<?php echo $i ?>">
			  Non
			</label>
			<input type="radio" value="no" name="atcd_active" id="atcd_active_no<?php echo $i ?>"/>
			<label for="comment_atcd<?php echo $i ?>">
			  Commentaire
			</label>
			<input name="comment_atcd" id="comment_atcd<?php echo $i ?>" type="text" />
			<label for="date_atcd<?php echo $i ?>">
			  Date
			</label>
			<input name="date_atcd" id="date_atcd<?php echo $i ?>" type="text" size="10" /><input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(this.form.date_atcd<?php echo $i ?>);" />
		      </div><br />
		      <label for="text_atcd_libre<?php echo $i ?>">
			Antécédents libres
		      </label>
		      <input name="text_atcd_libre" id="text_atcd_libre<?php echo $i ?>" type="text"  onchange="showButton('hide_text_details<?php echo $i ?>')"/>
		      <div id="hide_text_details<?php echo $i ?>" style="display:none">
			<label for="select_family<?php echo $i ?>">
			  Famille&nbsp;: 
			</label>
			<select name="select_family" id="select_family<?php echo $i ?>" onchange="options_genre('<?php echo $i ?>','text')">
			  <option></option>
<?php
	    foreach($atcd_types AS $this_type=>$un_type)
	    {
	      echo '
			  <option value="'.$this_type.'">'.$this_type.'</option>';
	    }
?>
			</select>
			<label for="select_genre<?php echo $i ?>">
			  Genre&nbsp;: 
			</label>
			<select name="select_genre" id="select_genre<?php echo $i ?>">
			  <option></option>
			</select>
			ALD
			<label for="text_ald_yes<?php echo $i ?>">
			  Oui
			</label>
			<input type="radio" value="yes" name="text_ald" id="text_ald_yes<?php echo $i ?>"/>
			<label for="text_ald_no<?php echo $i ?>">
			  Non
			</label>
			<input type="radio" value="no" name="text_ald" id="text_ald_no<?php echo $i ?>"/>
			Actif
			<label for="text_active_yes<?php echo $i ?>">
			  Oui
			</label>
			<input type="radio" value="yes" name="text_active" id="text_active_yes<?php echo $i ?>"/>
			<label for="text_active_no<?php echo $i ?>">
			  Non
			</label>
			<input type="radio" value="no" name="text_active" id="text_active_no<?php echo $i ?>"/>
			<label for="comment_atcd_libre<?php echo $i ?>">
			  Commentaire
			</label>
			<input name="comment_atcd_libre" id="comment_atcd_libre<?php echo $i ?>" type="text" />
			<label for="date_atcd_libre<?php echo $i ?>">
			  Date
			</label>
			<input name="date_atcd_libre" id="date_atcd_libre<?php echo $i ?>" type="text" size="10" /><input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(this.form.date_atcd_libre<?php echo $i ?>);" />
		      </div>
		    </div>

<?php
          $atcd=1; //initialisation pour afficher les colonnes
        }
        elseif (preg_match("`(\[Obst)`",$ligne_brut)) //nettoyage et affichage du titre Obstetrique
        {
          $atcd="";//on sort de l'affichage des antecedents
          if ($ligne2["FchPat_Sexe"]=="F")
          { 
?>
		    <p>
			<strong>Obst&eacute;trique</strong>
		    </p>
<?php
          }
        }
	elseif ($atcd==1)
        {
          $lecture_atcd="";
          $login_possesseur_terrain=$ligne3['RbDate_CreateSignUser'];
          $sql_recherche_possesseur="SELECT GUID FROM Personnes WHERE Login='$login_possesseur_terrain'";
          $resultat_recherche_possesseur=mysqli_query($db,$sql_recherche_possesseur);
          $ligne_recherche_possesseur=mysqli_fetch_array($resultat_recherche_possesseur);
          $GUID_possesseur_terrain=$ligne_recherche_possesseur['GUID'];
			
          if ($login_possesseur_terrain==$user) //On verifie si le terrain appartient a l'utilisateur
          {
            if (stripos($droits_user,"atv"))//si oui, on verifie qu'il a les droits
            {
              $lecture_atcd=1;
            }
          }
          else //sinon on verifie qu'il est utilisateur autorise
          {
            $sql_utilisateur_autorise="SELECT FriendUserDroits FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_possesseur_terrain'";
            $resultat_utilisateur_autorise=mysqli_query($db,$sql_utilisateur_autorise);
            $count_utilisateur_autorise=mysqli_num_rows($resultat_utilisateur_autorise);
            if ($count_utilisateur_autorise)//S'il existe un utilisateur autorise
            {
              $liste_utilisateur_autorise=mysqli_fetch_array($resultat_utilisateur_autorise);
              if (stripos($liste_utilisateur_autorise['FriendUserDroits'],"atv"))//On verifie qu'il a les droits
                $lecture_atcd=1;
            }
          }
         if ($atcd)	
          {
	    if ($ligne_brut)
	    {
	      echo '<p><input type="checkbox"  name="check_atcd'.$check.'" checked="checked" />';
	      $check++;
	      echo '
		      '.$ligne_brut.'
		    </p>';//Les lignes d'ATCD  
	    }
          }
          if (!$lecture_atcd)
          {
            echo '
		    <p>
		      Les droits de lecture des ant&eacute;c&eacute;dents appartiennent &agrave; '.$login_possesseur_terrain.'
		    </p>';
          }
        }
	if (preg_match("`Grossesse`",$ligne_brut) AND $ligne2["FchPat_Sexe"]=="F") //On affiche la ligne Grossesse
        {
//echo $ligne_brut;
          list($x,$date_grossesse)=explode ("=",$ligne_brut);
          if (strpos($date_grossesse,'-')) //vaut une date ou "non" //on cherche si la chaine a analyser contient au moins un tiret. Pas ideal mais mieux que rien.
          {
/* Inutilise
	    if ($date_format=='fr')
	      list($jour_grossesse,$mois_grossesse,$annee_grossesse)=explode("-",$date_grossesse);
	    elseif  ($date_format=='en')
	      list($mois_grossesse,$jour_grossesse,$annee_grossesse)=explode("-",$date_grossesse);
	    else
	      list($annee_grossesse,$mois_grossesse,$jour_grossesse)=explode("-",$date_grossesse);
*/
            $grossesse=1; // a tout hasard
          }
          else
          {
             $date_grossesse="";
             $grossesse="";
          }
?>
		    <p>
		      <label for="ddr<?php echo $i ?>">
			<strong>DDR :</strong> 
		      </label>  
		      <input name="ddr" id="ddr<?php echo $i ?>" type="text" value="<?php echo $date_grossesse ?>" size="10" maxlength="10" />
		      <input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_terrain<?php echo $i?>'].ddr);" />
		    </p>
<?php
        }
        if (preg_match("`Allaitement`",$ligne_brut) AND $ligne2["FchPat_Sexe"]=="F")
         {
           echo "
		    <p>
		      <label for=\"coche_allaitement".$i."\" title=\"Cochez pour signaler un allaitement puis actionnez Modifier le terrain\">
			Allaitement : 
		      </label>
		      <input name=\"coche_allaitement\" id=\"coche_allaitement".$i."\" type=\"checkbox\" value=\"oui\" title=\"Cochez pour signaler un allaitement puis actionnez Modifier le terrain\" ";
          if (preg_match("`oui`i",$ligne_brut))
          {
            $allaitement=1;
            echo "checked='checked' ";
          }
          else
          {
            $allaitement="";
          }
	  echo " />
		    </p>"; 
        }// fin si allaitement
        else 
        {
          $allaitement="";
        }
        if (preg_match("`(\[VAR )`",$ligne_brut))//nettoyage et recuperation des titres des tableaux
        {
          if ($fin_ligne) //lu a partir du deuxieme tableau pour cloturer le precedent
          {
            $fin_ligne="";
          }
          if ($fin_table)
          {//cloturer la derniere ligne de valeurs ou la ligne de titre et afficher une ligne supplementaire a chaque tableau pour saisir nouvelles valeurs
            echo "
		    </tr>
		    <tr>
		      <td class=\"fond_td\">
			<input name=\"date[".$num_tableau."]\" type=\"text\" value=\"".$maintenant."\" size=\"19\" maxlength=\"19\"/>
		      </td>";
            for ($n=1;$n<=$nombre_de_colonnes[$num_tableau];$n++)
            {
              echo "
		      <td class=\"fond_td\">
			<input name=\"tableau_variables[".$num_tableau."][".$n."]\" type=\"text\" size=\"5\" maxlength=\"5\"/>
		      </td>"; //les zones de saisie pour nouvelles valeurs
            }
            echo "
		    </tr>
		  </table><!--si dollar fin table -->
";
            $fin_table="";
          } //fin du traitement de fin table a partir du deuxieme tableau sauf le dernier
          $titre_propre=preg_replace("`\[VAR`","",$ligne_brut);
          $titre_propre=preg_replace("`]`","",$titre_propre);
          echo "
		    <table><!-- table des valeurs -->
		      <tr>
			<th class=\"fond_th\">
			  ".$titre_propre."
			</th>
		      </tr>
		      <tr style=\"background:#DDFFDD\"><!-- affichage du titre du tableau-->
			<th class=\"fond_th\">
			  Date
			</th>";//initialisation des tableaux de valeurs
          $num_tableau++;
          $nombre_de_colonnes[$num_tableau]=0; //a chaque nouveau tableau, on cree une nouvelle variable qu'on reinitialise a 0
        }
        if (preg_match ("`Mod`i",$ligne_brut))//recuperer les noms des colonnes si la ligne contient Modele
        {
//mettre ici un compteur pour le nombre de champs de ce tableau tenant compte du numero du tableau
	  $nombre_de_colonnes[$num_tableau]=$nombre_de_colonnes[$num_tableau]+1;
          $split_modele=explode(",",$ligne_brut);
          echo "
			<th class=\"fond_th\">
			  ".$split_modele[1]."
			</th>";//affichage des colonnes.
          $fin_titre=1;
          $fin_table=1;
        }
        if (preg_match($exp_date,$ligne_brut))//detection d'une date pour les valeurs des tableaux
        {
          if ($fin_titre)
          {
            echo "
		      </tr>"; //fin de ligne du titre //indispensable
            $fin_titre=""; //On ne traitera plus les titres apres la premiere valeur
          } 
          if ($fin_ligne)
          {
            echo "
		      </tr>";
            $fin_ligne="";
          }
          $fin_table="";
          $split_ligne_valeurs=explode("=",$ligne_brut); //On coupe au signe = pour separer la date des valeurs
$split_date_heure=explode("T",$split_ligne_valeurs[0]);

          echo "
		      <tr>
			<td class=\"fond_td\">
			  ".iso_to_local($split_date_heure[0],$date_format)." ".$split_date_heure[1]."
			</td>";//affichage de la date en debut de ligne
          $split_valeurs=explode(",",$split_ligne_valeurs[1]);//On separe les valeurs au niveau des virgules
          foreach ($split_valeurs as $valeur)
          {
            echo "
			<td class=\"fond_td\">
			  ".$valeur."
			</td>"; //les valeurs de cette ligne de ce tableau
            $fin_ligne=1;
          }
          reset ($split_valeurs);
          $fin_ligne=1;
          $fin_table=1;
        } //  fin des tableaux de valeurs
        if (preg_match("`\[Traitement]`i",$ligne_brut)) 
        {
          if ($fin_titre)
          {
            echo "
		      </tr>"; //fin de ligne du titre
            $fin_titre=""; //On ne traitera plus les titres apres la premiere valeur
          } 
          if ($fin_ligne) //terminaison dernier tableau
          {
            echo "
		      </tr>";
            $fin_ligne="";
          }
          if ($fin_table)
          {
            echo "
		      <tr>"; //afficher une ligne supplementaire au dernier tableau pour saisir nouvelles valeurs
            echo "
			<td class=\"fond_td\">
			  <input name=\"date[".$num_tableau."]\" type=\"text\" value=\"".$maintenant."\" size=\"19\" maxlength=\"19\"/>
			</td>";
            for ($n=1;$n<=$nombre_de_colonnes[$num_tableau];$n++)
            {
              echo "
			<td class=\"fond_td\">
			  <input name=\"tableau_variables[".$num_tableau."][".$n."]\" type=\"text\" size=\"5\" maxlength=\"5\"/>
			</td>"; //les zones de saisie pour nouvelles valeurs
            }
?>
		      </tr>
		      <tr>
			<th class="fond_th">
			  Traitement habituel
			</th>
			<td>Nouveau traitement de fond
			  <select name="select_ordo">
			    <option value='X'>Inchangé
			    </option>
			    <option value=''>
			    </option><!-- ligne vide -->
<?php
	    mysqli_data_seek($resultat_global,0);
	    while ($ligne_global=mysqli_fetch_array($resultat_global))
	    {
	      if ($ligne_global['RbDate_TypeRub']=='20020100') //ordonnance structuree
	      {
?>
			    <option value="<?php echo $ligne_global['RbDate_RefBlobs_PrimKey']?>">
			      <?php echo $ligne_global['RbDate_NomDate'].' '.$ligne_global['RbDate_Date'] ?>
			    </option>
<?php
	      }
	    }
?>
			  </select>
			</td>
		      </tr>
<?php
 //fin du dernier tableau de valeurs
//On ajoute le contenu des ordonnances chroniques, epurees des balises
	  $ordo_mieux=(preg_match($expr,$ligne3['RbDate_DataRub'],$tab));
	  if (isset($tab[2]))
	  {
	    $ordo_chronique=preg_replace("`<body.*>`", " ",$tab[2]);
?>
		      <tr>
			<td class="fond_td">
			  <?php echo $ordo_chronique ?>
			  <input name="ordo_chronique" type="hidden" value="<?php echo htmlspecialchars($ligne3['RbDate_DataRub']) ?>" /><!-- pour transmettre a l'url -->
			</td>
		      </tr>
<?php
	  }
?>
		      <tr>
			<td class="fond_td">
			  <input name="id_terrain" type="hidden" value="<?php echo $ligne3['RbDate_PrimKey'] ?>" />
			  <input name="bouton_modifier_terrain" type="submit" value="Modifier le terrain" title="Les modifications tiennent compte des variables de suivi, de la grossesse et de l'allaitement"/>
			</td>
		      </tr>
		    </table><!-- fin table des valeurs -->
		  </form> <!-- fin formulaire pour valider les modifications du terrain -->  
<?php
            $fin_table="";
          }
        }
      }
?>
		</td>
	      </tr>
<?php
    }
  }
?>
	      <tr>
		<td colspan="3">
		  <form action="consultation.php" method="post">
<!-- Le bouton Nouveau terrain, base sur templates.php -->
		    <div>
		      Nouveau terrain
		      <input name="button_valider" type="submit" value="Valider" />
		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		      <input name="rubrique" type="hidden" value="20060000" />
		      <input name="date_modifiee" type="hidden" value="<?php echo iso_to_local(date('Y-m-d', date('U')),$date_format) ?>" />
		      <input name="heure_observation_modifiee" type="hidden" value="<?php echo date('h:i:s', date('U')) ?>" />
		      <input name="titre_observation" type="hidden" value="Terrain" />
		      <input name="obs_modifiee" type="hidden" value="<?php echo $terrain_modele ?>" />
		    </div>
		  </form>
		</td>
	      </tr>	    
	    </table>
<?php
// Fin du tableau des terrains
}

include("inc/footer.php");
?>