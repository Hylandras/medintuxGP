<?php
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

$sql_droit_admin="SELECT Droits,GUID FROM Personnes WHERE Login='$user'";
$resultat_droit_admin=mysqli_query($db,$sql_droit_admin);
$ligne_droit_admin=mysqli_fetch_array($resultat_droit_admin);
$user_GUID=$ligne_droit_admin['GUID'];

$auth=1;
//On verifie que l'utilisateur peut voir la liste
if (!preg_match("`plv`",$ligne_droit_admin["Droits"])) //si l'utilisateur n'a pas les droits directs de voir la liste des patients, on verifie les droits delegues
{
  $auth=0;
  $sql_sign="SELECT Droits,GUID FROM Personnes WHERE Login='$sign'";
  $resultat_sign=mysqli_query($db,$sql_sign);
  $ligne_sign=mysqli_fetch_array($resultat_sign);
  $sign_GUID=$ligne_sign['GUID'];
  $sign_droits=$ligne_sign['Droits'];
  //On cherche s'il a des droits delegues par n'importe quel utilisateur signataire
  if (preg_match("`plv`",$sign_droits)) //on verifie d'abord que le signataire a les droits
  {
  //On cherche si le delegue a les droits
    $sql_droits_delegues="SELECT FriendUserDroits FROM user_perms WHERE SignataireGUID='$sign_GUID' AND FriendUserGUID='$user_GUID'";
    $resultat_droits_delegues=mysqli_query($db,$sql_droits_delegues);
    $ligne_droits_delegues=mysqli_fetch_array($resultat_droits_delegues);
    if (preg_match("`plv`",$ligne_droits_delegues['Droits'])) //Si le delegue a le droit 
    {
      $auth=1;
    }
  }
}
if ($auth==0)
{
  header ('location:index.php?message=message4');
  exit;
}
//$date_format='fr';//other possible value =en or nothing
//vient de config.php
$format_date['fr']='JJ-MM-AAAA';
$format_date['en']='MM-JJ-AAAA';
$format_date['iso']='AAAA-MM-JJ';

//$cle="&nbsp;";

$cle="";
$critere="";
$Prenom="";
$Nom="";
$Tel="";

$exact=0;

if (isset($_GET['cle']))
  $cle=$_GET['cle'];
  
if (isset($_GET['critere_recherche']))
  $critere=$_GET['critere_recherche'];
  
if (isset($_GET['envoyer']))
  $envoyer=$_GET['envoyer'];
if (isset($_GET['Prenom']))
  $Prenom=$_GET['Prenom'];
  
if (isset($_GET['Nom']))
  $Nom=$_GET['Nom'];
  
if (isset($_GET['Tel']))
  $Tel=$_GET['Tel'];
  
if (isset($_GET['exact']))
  $exact=$_GET['exact'];

$count='';
$i=0;
$tableau_criteres['FchGnrl_NomDos']="Nom";
$tableau_criteres['FchGnrl_Prenom']="Pr&eacute;nom";
$tableau_criteres['FchPat_NomFille']="Nom de jeune fille";
$tableau_criteres['FchPat_Adresse']="Adresse";
$tableau_criteres['FchPat_Ville']="Ville";
$tableau_criteres['FchPat_CP']="Code Postal";
$tableau_criteres['FchPat_NumSS']="Num&eacute;ro de s&eacute;cu";
$tableau_criteres['FchPat_Nee']="Date de naissance";
$tableau_criteres['FchPat_Profession']="Profession";
$tableau_criteres['FchPat_Tel1']="T&eacute;l&eacute;phone";

include("inc/header.php");

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
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
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
function showButton(name)
{
  document.getElementById(name).style.display = 'block';
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function verif_champ(champ)
{
  if (champ == "")
  { 
    alert("Le champ n'est pas rempli\nMettez le signe % si vous voulez vraiment afficher tout le fichier");
    return false;
  }
  return true;
}
//]]>
    </script>

<?php
if (isset($_REQUEST['critere_recherche'])) //pas besoin de calendrier tant qu'on n'a rien recherche
{
  include 'calendar_javascript.php';
}
?>
    <style type="text/css"  media="print">
	td:nth-child(1) { display: none; }  
	td:nth-child(7) { display: none; }  
	td:nth-child(8) { display: none; }  
	th:nth-child(1) { display: none; }  
	th:nth-child(7) { display: none; }  
	th:nth-child(8) { display: none; }  
    </style>
    <title>
      Dossiers patients - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
  </head>
	
  <body style="font-size:<?php echo $fontsize; ?>pt"  onload="donner_focus('cle')">
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Fichier_des_patients';
include("inc/menu-horiz.php");	
$message['message_doublon']="Ce dossier ne peut être importé car il existe déjà";

if (isset($_REQUEST['message']))
{
  $ce_message=$_REQUEST['message'];
  echo $message[$ce_message];
}
?>
  <div class="groupe">
      <h1>MedWebTux - Patients</h1>
	<div class='noPrint'>
	<div class="tableau">
	  <table> 
	    <tr>
	      <td>

		<form id="form_choix_patient" action="" method="get" >
		  <fieldset class="fieldset_formu">
		    <legend>
		      Crit&egrave;res de choix
		    </legend>
		      <select name="critere_recherche" onchange="javascript:submit();">
			<!--Liste des cles de recherche-->
<!--			<option value="" selected="selected"
			   Choisir dans la liste 
			/option-->
			<option value="FchGnrl_NomDos" <?php if ($critere=='FchGnrl_NomDos') echo 'selected="selected"' ?>>
			  Nom
			</option>
			<option value="FchGnrl_Prenom" <?php if ($critere=='FchGnrl_Prenom') echo 'selected="selected"' ?>>
			  Pr&eacute;nom
			</option>
			<option value="FchPat_NomFille" <?php if ($critere=='FchPat_NomFille') echo 'selected="selected"' ?>>
			  Nom de jeune fille
			</option>
			<option value="FchPat_Adresse" <?php if ($critere=='FchPat_Adresse') echo 'selected="selected"' ?>>
			  Adresse
			</option>
			<option value="FchPat_Ville" <?php if ($critere=='FchPat_Ville') echo 'selected="selected"' ?>>
			  Ville
			</option>
			<option value="FchPat_CP" <?php if ($critere=='FchPat_CP') echo 'selected="selected"' ?>>
			  Code Postal
			</option>
			<option value="FchPat_NumSS" <?php if ($critere=='FchPat_NumSS') echo 'selected="selected"' ?>>
			  Num&eacute;ro de s&eacute;cu
			</option>
			<option value="FchPat_Nee" <?php if ($critere=='FchPat_Nee') echo 'selected="selected"' ?>>
			  Date de naissance
			</option>
			<option value="FchPat_Profession" <?php if ($critere=='FchPat_Profession') echo 'selected="selected"' ?>>
			  Profession
			</option>
			<option value="FchPat_Tel1" <?php if ($critere=='FchPat_Tel1') echo 'selected="selected"' ?>>
			  T&eacute;l&eacute;phone
			</option>
		      </select>
		   
<?php 
		if ($critere=='FchPat_Nee') {
?>
			<input name="cle" id="datepicker" type="text" value="<?php echo stripslashes($cle) ?>" size="17" maxlength="10"/>
<?php
		}
		else {
?>
			<input name="cle" id="cle" type="text" value="<?php echo stripslashes($cle) ?>" size="17" title="L'information à chercher. Ne fait pas de différence majuscule/minuscule ni caractère accentué ou non. Refuse la requête si laissé vide. Mettre juste % pour afficher tout le fichier (déconseillé)" />
<?php
		}

		if ($exact == "1") {
			echo "<br /><input name=\"exact\" id=\"exact\" type=\"checkbox\" value=\"$exact\" checked=\"true\" />";
		} else {
			echo "<br /><input name=\"exact\" id=\"exact\" type=\"checkbox\" value=\"1\" />";
		}
?>
			<label for="exact" title="Ne cochez que si vous voulez restreindre la recherche exactement à ce qui se trouve dans la zone de saisie. ">Correspondance exacte</label>
		    </fieldset>

		    <fieldset class="fieldset_formu">
		      <legend >
			Affichage du résultat
		      </legend>			
			  <input name="envoyer" type="submit" value="Chercher" title="Pour chercher les dossiers patients" />
			
<?php
//Recherche si l'utilisateur a des droits de resolution des doublons

if (!preg_match("`adr`",$ligne_droit_admin["Droits"])) //si l'utilisateur n'a pas les droits directs de resolution de doublons, on verifie les droits delegues
{
  $sql_sign="SELECT Droits,GUID FROM Personnes WHERE Login='$sign'";
  $resultat_sign=mysqli_query($db,$sql_sign);
  $ligne_sign=mysqli_fetch_array($resultat_sign);
  $sign_GUID=$ligne_sign['GUID'];
  $sign_droits=$ligne_sign['Droits'];
  //On cherche s'il a des droits delegues par n'importe quel utilisateur signataire
  if (preg_match("`adr`",$sign_droits)) //on verifie d'abord que le signataire a les droits
  {
  //On cherche si le delegue a les droits
    $sql_droits_delegues="SELECT FriendUserDroits FROM user_perms WHERE SignataireGUID='$sign_GUID' AND FriendUserGUID='$user_GUID'";
    $resultat_droits_delegues=mysqli_query($db,$sql_droits_delegues);
    $ligne_droits_delegues=mysqli_fetch_array($resultat_droits_delegues);
    if (preg_match("`adr`",$ligne_droits_delegues['Droits'])) //Si le delegue a le droit adr, on affiche le bouton selectionner
    {
?>
			  <input name="envoyer" type="submit" value="Sélectionner" title="Permet d'afficher une coche devant chaque dossier afin d'opérer des opérations concernant plusieurs fiches" />
			
		      </fieldset>
<?php
    }
  }
}
else //si oui, on affiche le bouton de selection multiple
{
?>
			  <input name="envoyer" type="submit" value="Sélectionner" title="Permet d'afficher une coche devant chaque dossier afin d'opérer des opérations concernant plusieurs fiches" />
		      </fieldset>
			</form>
				
<?php
}
?>  
		  </td>
		</tr>
	   </table>
	</div>
    </div>
 </div>
 
	<div class="groupe">
	   <h1>Création de dossier</h1>
	    <div class='noPrint'>
	    <div class="tableau">
	      <table> 
		<tr>
		  <td>

		    <form action="nouveau_dossier.php" method="post">
		      <fieldset class="fieldset_formu">
			<legend >
			  Création à partir d'un formulaire vierge
			</legend>
			<input name ="critere_recherche" type="hidden" value="<?php echo $critere ?>" />
			<input name ="cle" type="hidden" value="<?php echo $cle ?>" />
			<input name ="Prenom" type="hidden" value="<?php echo $Prenom ?>" />
			<input name ="Nom" type="hidden" value="<?php echo $Nom ?>" />
			<input name ="Tel" type="hidden" value="<?php echo $Tel ?>" />
			<input name="envoyer" type="submit" value="Nouveau" title="Attention ! Pas de contrôle de doublons."/>
		      </fieldset>
		    </form>

		    <form action="import.php" method="post" enctype="multipart/form-data" title="Choisissez un fichier xml à importer, puis validez. Sert à créer une fiche patient à partir d'une fiche existant sur un autre ordinateur. Ne peut se faire qu'une seule fois pour un patient donné.">
		      <fieldset class="fieldset_formu">
			<legend>
			  Importation de fiche patient MedinTux
			</legend>
			<input type="file" name="selection" size="17"  onchange="showButton('button_place')" />
			<div id="button_place" style="display:none"> 
			  <input name="envoyer" type="submit" value="Importer" title="Importer un dossier à partir d'un fichier XML de MedinTux"/>
			</div>
		      </fieldset>
		    </form>

		    </td>
		  </tr>
		</table>
		</div>
	    </div>
	  </div>
<?php
function display_line($envoyer,$cle,$guid,$titre,$nom,$prenom,$naissance,$adresse,$tel,$debut,$i,$prmikey)
{
?>
	  <tr>
<?php
  if ($envoyer=="Sélectionner") //on n'affiche pas les coches si mode liste
  {
?>
	    <td class="fond_td">
	      <input name="selection_patient[]" type="checkbox" value="<?php echo $guid ?>" title="Cochez toutes les fiches concernant le même patient, puis validez. Vous pourrez alors les fondre en une seule fiche contenant toutes les informations." />
	    </td>
<?php
  }
  if ($envoyer=="Chercher") //On n'affiche pas l'agenda si mode coches
  {
?>
	    <td class="fond_td" title="Le bouton Rendez-vous permet d'aller en mode de cr&eacute;ation de rendez-vous, avec le fiche pr&eacute;remplie. Il est pr&eacute;f&eacute;rable de donner le date ici, afin de pouvoir contr&ocirc;ler les plages horaires disponibles cette journ&eacute;e-l&agrave;"  style="margin:0;padding:0;" >
	      <form action="agenda.php" method="get" id="form_jour<?php echo $i?>"><!--Bouton de prise de rendez-vous -->
		<div>
		  <input name="from_page" type="hidden" value="liste" />
		  <input name="GUID" type="hidden" value="<?php echo $guid?>" />
		  <input name="Nom" type="hidden" value="<?php echo $nom ?>" />
		  <input name="nom" type="hidden" value="%" />
		  <input name="Prenom" type="hidden" value="<?php echo $prenom?>" />
		  <input name="Tel" type="hidden" value="<?php echo $tel?>" />
		</div>	
		<div class='noPrint'>
		    <input name="envoyer" type="submit" value="Rendez-vous" />
		    <input name="debut" type="hidden" value="<?php echo $debut; ?>" />
		    <input name="fin" type="hidden" value="<?php echo $debut; ?>"/>
		</div>
	      </form>
	    </td>
<?php
  } //fin si mode liste
?>	
	    <td class="fond_td">
	      <a href="frame_patient.php?GUID=<?php echo $guid ?>"><?php echo stripslashes($titre)." ". stripslashes($nom) ?></a>
	    </td>
	    <td class="fond_td">
	      <?php echo stripslashes($prenom) ?>
	    </td>
	    <td class="fond_td">
	      <?php echo $naissance?>
	    </td><!--Date de naissance -->
	    <td class="fond_td">
	      <?php echo str_replace ("\n","<br />",stripslashes($adresse))?>
	    </td>
	    <td class="fond_td">
	      <?php echo $tel ?>
	    </td>
<?php
  if ($envoyer=="Chercher") //On n'affiche pas les boutons si mode coches
  {
?>
	    <td class="fond_td"  style="margin:0;padding:0;">
	      <form action="nouveau_dossier.php" method="get" ><!--Les boutons de modification et suppression -->
		<div>
		  <input name="ID" type="hidden" value="<?php echo $prmikey ?>" />
		  <input name="Nee" type="hidden" value="<?php echo $naissance ?>" />
		</div>
		<div class='noPrint'>
		 <input name="envoyer" type="submit" value="Modifier" title="permet non seulement de modifier les donn&eacute;es administratives du patient, mais aussi de cr&eacute;er une nouvelle fiche &agrave; partir de la fiche s&eacute;lectionn&eacute;e (membre de la famille, par exemple)" /> 
		 <input name="envoyer" type="submit" value="Supprimer" />
		</div>
	      </form>
	    </td>
<?php
  }
?>
	  </tr>
<?php
} //fin function display line

if (isset($envoyer))
{
//la requete si nom ou prenom
  if ($critere=="FchGnrl_NomDos" OR $critere=="FchGnrl_Prenom")
  {
    if ($exact!=1)
      $cle='%'.$cle.'%';
    $sql=sprintf("SELECT * FROM IndexNomPrenom WHERE $critere LIKE '%s' ORDER BY FchGnrl_NomDos,FchGnrl_Prenom",mysqli_real_escape_string($db,$cle));
  }
  else if ($critere=="FchPat_NomFille" OR $critere=="FchPat_Adresse" OR $critere=="FchPat_Ville" OR $critere=="FchPat_CP" OR $critere=="FchPat_NumSS" OR $critere=="FchPat_Nee" OR $critere=="FchPat_Profession" OR $critere=="FchPat_Tel1")
//la requete si autre critere de recherche
  {
  //conversion format de date de naissance si pas iso
    if ($critere=="FchPat_Nee")
    {
      $list_date=explode ("-",$cle);
      if ($date_format=='fr')
      {
      //on repasse en iso les dates qui arrivent en francais
	$cle=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
      }
      elseif ($date_format=='en')
	$cle=$list_date[2].'-'.$list_date[0].'-'.$list_date[1];
    //si iso,on garde pareil
    }
    if ($exact!=1) //correspondance exacte
    {
      $cle='%'.$cle.'%';
    }
    if ($critere=="FchPat_Nee")
      $cle=$cle.' 00:00:00'; //on ajoute les minutes pour corresponcance exacte
    $sql=sprintf("SELECT * FROM fchpat WHERE $critere LIKE '%s' ORDER BY $critere",mysqli_real_escape_string($db,$cle));
  } //fin autres criteres recherche
  $resultat=mysqli_query($db,$sql);
  $count=mysqli_num_rows($resultat);
    
  $pluriel="";
  if ($count>1) //Si on trouve plusieurs resultats pour cette requete
  {
    $pluriel="s";
  }
  if ($count=="")
    $nombre="aucun";
  else
    $nombre=$count;

/*  echo "<div class=\"groupe\"><br />
	<div class=\"notice\">
    Recherche sur <strong>".$cle."</strong> selon <strong>".$tableau_criteres[$critere]."</strong> : ".$nombre." r&eacute;sultat".$pluriel;
  echo "</div>
	</div>";
*/
//  if ($count) //affichage du tableau seulement si résultats
  {
?>

<?php
if ($envoyer=="Sélectionner")
{
?>
	<form action="merge.php" method="get">
<?php
}
?>
   <div class="groupe">
      <a name="resultat"></a><h1>Recherche sur <strong><?php echo $cle?></strong> selon <strong><?php echo $tableau_criteres[$critere] ?></strong> : <?php echo $nombre ?> r&eacute;sultat<?php echo $pluriel ?></h1>
	<div class="tableau">
	  <table>
	    <col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col>
	    <tr>	
<?php
    if ($envoyer=="Sélectionner") //on n'affiche pas les coches si mode liste
    {
?>
	    <th class="fond_th">
	      <input name="bouton_valider_coches" type="submit" value="Valider" />
	    </th>
<?php
    } //fin mode coches
    if ($envoyer=="Chercher") //On n'affiche pas l'agenda si mode coches
    {
?>
	    <th class="fond_th">
	      <div class='noPrint'>
		Agenda
	      </div>
	    </th>
<?php
    } //fin mode boutons
?>
	    <th class="fond_th">
	      Nom
	    </th>
	    <th class="fond_th">
	      Pr&eacute;nom
	    </th>
	    <th class="fond_th">
	      Naissance
	    </th>
	    <th class="fond_th">
	      Adresse
	    </th>
	    <th class="fond_th">
	      Téléphone
	    </th>
<?php
    if ($envoyer=="Chercher") //On n'affiche pas les boutons modifier et supprimer si mode coches
    {
?>
	    <th colspan="2" class="fond_th">
	      <div class='noPrint'>
		Dossier
	      </div>
	    </th>
<?php
    }
?>
	  </tr>
    <?php
    }
    if ($critere=="FchGnrl_NomDos" OR $critere=="FchGnrl_Prenom")
    {
      $resultat=mysqli_query($db,$sql);
      $i=0;
      while ($ligne=mysqli_fetch_array($resultat)) 
      {
	$i++;
	$no_dossier=$ligne["FchGnrl_IDDos"];
	$sql2="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$no_dossier'";
	$resultat2=mysqli_query($db,$sql2);
	$ligne2=mysqli_fetch_array($resultat2);

	$titre=$ligne2["FchPat_Titre"];
	$nom=$ligne["FchGnrl_NomDos"];
	$guid=$ligne["FchGnrl_IDDos"];
	$prenom=$ligne["FchGnrl_Prenom"];
	$adresse=$ligne2["FchPat_Adresse"];
	$tel= $ligne2["FchPat_Tel1"];
	$prmikey=$ligne["ID_PrimKey"];
  //formatage de la date de naissance
	$table_naissance_full=explode (" ",$ligne2["FchPat_Nee"]);
	$naissance='';
	if (strpos($table_naissance_full[0],'-')) //eviter de traiter les dates de  naissance vides
	  $naissance=iso_to_local($table_naissance_full[0],$date_format);
	$debut=iso_to_local(date('Y-m-d', date('U')),$date_format);

	//affichage des lignes de resultat
	  //tableau
	display_line($envoyer,$cle,$guid,$titre,$nom,$prenom,$naissance,$adresse,$tel,$debut,$i,$prmikey);
      } //fin while analyse resultats
    } //fin if nomprenom
    elseif ($critere=="FchPat_NomFille" OR $critere=="FchPat_Adresse" OR $critere=="FchPat_Ville" OR $critere=="FchPat_CP" OR $critere=="FchPat_NumSS" OR $critere=="FchPat_Nee" OR $critere=="FchPat_Profession" OR $critere=="FchPat_Tel1")
//la requete si autre critere de recherche
    {
      while ($ligne=mysqli_fetch_array($resultat)) 
      {
	$titre=$ligne["FchPat_Titre"];
	$adresse=$ligne["FchPat_Adresse"];
	$tel= $ligne["FchPat_Tel1"];
	$guid=$ligne["FchPat_GUID_Doss"];
	$table_naissance_full=explode (" ",$ligne["FchPat_Nee"]);
	$naissance='';
	if (strpos($table_naissance_full[0],'-')) //eviter de traiter les dates de  naissance vides
	  $naissance=iso_to_local($table_naissance_full[0],$date_format);
	$debut=iso_to_local(date('Y-m-d', date('U')),$date_format);
	$sql2="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$guid'  ORDER BY FchGnrl_NomDos,FchGnrl_Prenom";
	$resultat2=mysqli_query($db,$sql2);
	$ligne2=mysqli_fetch_array($resultat2);

	$nom=$ligne2["FchGnrl_NomDos"];
	$prenom=$ligne2["FchGnrl_Prenom"];
	$prmikey=$ligne2["ID_PrimKey"];
	if ($critere=="FchPat_NomFille") //pas d'affichage si pas de nom de jeune fille trouve. Valable pour recherche sur %.
	{
	  if ($ligne["FchPat_NomFille"])
	    display_line($envoyer,$cle,$guid,$titre,$nom,$prenom,$naissance,$adresse,$tel,$debut,$i,$prmikey);
	}
	else
	  display_line($envoyer,$cle,$guid,$titre,$nom,$prenom,$naissance,$adresse,$tel,$debut,$i,$prmikey);
      }
    }
} //fin envoyer
// on ferme la base
mysqli_close($db);
if ($count)
{
?>
    </table>
   </div>
 </div>
<?php
  if ($envoyer=="Sélectionner") //fin du formulaire des coches
  {
    echo "
      </form>";
  }
}

include("inc/footer.php");
?>

