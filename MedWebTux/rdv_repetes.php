<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=agenda' );
  exit;
}
include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
//les dates de recherche selon la langue
if ($date_format=='fr')
  $date=date('d-m-Y', date('U'));
elseif ($date_format=='en')
  $date=date('m-d-Y', date('U'));
else 
 $date=date('Y-m-d', date('U')); // la date du jour

//initialisation des jours de la semaine
//$text_jsemaine=array(0=>"Dimanche",1=>"&nbsp;&nbsp;Lundi&nbsp;&nbsp;",2=>"&nbsp;&nbsp;Mardi&nbsp;&nbsp;",3=>"Mercredi",4=>"&nbsp;&nbsp;Jeudi&nbsp;&nbsp;",5=>"Vendredi",6=>"&nbsp;Samedi&nbsp;");

//URL nom=&prenom=&tel=&type=&status=%25%25&GUID=&duree=15&notes=
//On recupere les variables dans l'URL
/*if (isset($_GET['utilisateur_autorisant'])) //si on recupere un UA
	$utilisateur_autorisant=$_GET['utilisateur_autorisant'];
else
	$utilisateur_autorisant=$_SESSION['login']; //sinon, il vaut l'utilisateur connecte

if ($utilisateur_autorisant==$_SESSION['login'])
	$user=$_SESSION['login'];
else
	$user=$utilisateur_autorisant;
*/
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$utilisateur_autorisant=$tab_login[1];

$Type="";
$Status="";
$Nom="";
$Prenom="";
$GUID='';
$Duree='';
$Tel='';
$Notes='';

if (isset($_GET['Type']))
  $Type=$_GET['Type'];

if (isset($_GET['Status']))
  $Status=$_GET['Status'];

if (isset($_GET['Nom']))
  $Nom=$_GET['Nom'];

if (isset($_GET['Prenom']))
  $Prenom=$_GET['Prenom'];

if (isset($_GET['GUID']))
	$GUID=$_GET['GUID'];

if (isset($_GET['Duree']))
	$Duree=$_GET['Duree'];

if (isset($_GET['Tel']))
	$Tel=$_GET['Tel'];

if (isset($_GET['Time']))
{
  $Time=$_GET['Time'];
  $Heure=substr($Time,0,2);
  $Minutes=substr($Time,3,2);
}
else
{
  $Heure='00';
  $Minutes='00';
}

if (isset($_GET['Date']))
  $Date=$_GET['Date'];
if (isset($_GET['Notes']))
  $Notes=$_GET['Notes'];

//Recherche des couleurs de RdV
$sqlcouleur="SELECT * FROM color_profils GROUP BY Name";

include("inc/header.php");
?>
    <title>
      MedWebTux - Rendez-vous répétés -<?php echo $_SESSION['login'] ?>
    </title>
    <script type="text/javascript" >
//<![CDATA[
function verif_champ(champ)
{
  if (champ < 10)
  { 
    alert("La durée des rendez-vous répétés doit être de 10 minutes au moins");
    return false;
  }
  return true;
}
//]]>
    </script>
    <script type="text/javascript" >
//<![CDATA[
function donner_focus(chp)
{
	document.getElementById(chp).focus();
}
//]]>
    </script>
    <script type="text/javascript" >
//<![CDATA[
function mettre_valeur(value)
{
  var type=new Array;
<?php
  //creation de variables javascript pour les durees des rdv

  $resultatcouleur=mysqli_query($db,$sqlcouleur);
  while ($lignecouleur=mysqli_fetch_array($resultatcouleur))
  {
    $name=$lignecouleur['Name'];
    echo "type[\"".$name."\"]=\"".$lignecouleur['Duree']."\";"; //OK
  }
?>
  document.forms['form_jour'].Duree.value = type[value]; // on ecrit le resultat dans Duree
}
//]]>
    </script>
<?php
include 'calendar_javascript.php';
?>
    <script type="text/javascript" >
//<![CDATA[
function chercher_patient()
// on ouvre dans une fenêtre le fichier passé en paramètre.
{ 
  window.open('recherche_patient_agenda.php','Choisir','width=700,height=450,top=50,left=50,toolbar=yes, scrollbars=yes, location=no'); 
}
//]]>
    </script>
  </head>

  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor="rafale";
include("inc/menu-horiz.php");		
?>
      <div class="groupe">	
	<h1>
	  MedWebTux - Rendez-vous répétés de <?php echo $utilisateur_autorisant; ?>
	</h1>
<?php

//formatage de la date
if (isset($_GET['Date']))
{
  $jour=substr($_GET['Date'],8,2);
  $mois=substr($_GET['Date'],5,2);
  $annee=substr($_GET['Date'],0,4);
}
else
{
  if (isset($_GET['jour']))
  {
    $jour=$_GET['jour'];
  }
}

?>
      <form action="validation_rdv.php" method="get" id="form_jour" onsubmit='return verif_champ(this.Duree.value);'>
      <fieldset>
       <legend>Rendez-vous répétés</legend>
	<table>
	  <tr>
	    <td>
	      <table>
		<tr>
		  <th>
		    <label for="Type">
		      Type : 
		    </label>
		  </th>
		  <td>
		    <input name="RDV_PrisAvec" type="hidden" value="<?php echo $user ?>" />
		    <select name="Type" id="Type" onchange="mettre_valeur(this.value)">
<?php
//recuperation des differents types de RDV
$resultatcouleur=mysqli_query($db,$sqlcouleur);
while ($lignecouleur=mysqli_fetch_array($resultatcouleur))
{
?>
		      <option value="<?php echo $lignecouleur["Name"]?>"<?php if ($Type==$lignecouleur["Name"])  echo 'selected="selected"' ;?>>
			<?php echo $lignecouleur["Name"];?>
		      </option>
<?php
}
?>
		    </select>
		  </td>
		</tr>
		<tr>
		  <th>
		    <label for="Duree">
		      Dur&eacute;e : 
		    </label>
		  </th>
		  <td>
		    <input name="Duree" id="Duree" type="text" value="<?php echo $Duree; ?>" size="5" maxlength="11" />
		  </td>
		</tr>
		<tr>
		  <th>
		    <label for="status">
		    Statut : 
		    </label>
		  </th>
		  <td>
		    <select name="status" id="status" >
<?php
  foreach ($status_rdv AS $ce_rdv)
  {
?>
		      <option value="<?php echo $ce_rdv ?>" 
<?php 
    if ($ce_rdv==$Status) //si le statut est recupere dans l'URL
	echo "selected='selected'"; 
    else
    {
      if (!$Nom AND $ce_rdv=="Non attribué") //si pas de nom dans l'URL, on selectionne non attribue
	echo "selected='selected'"; 
    }
?>
 >
<?php echo $ce_rdv ?>
		      </option>
<?php
  }
?>
		    </select>
		  </td>
		</tr>
		<tr>
		  <td>
		    <a href="#" onclick="chercher_patient()">Choisir un patient</a>
		  </td>
		  <td>
		  </td>
		</tr>
		<tr>
		  <th>
		    <label for="GUID">
		      ID : 
		    </label>
		  </th>
		  <td>
		    <input name="GUID" id="GUID" type="text" value="<?php echo $GUID ?>" />
		  </td>
		</tr>
		<tr>
		  <th>
		    <label for="Nom">
		      Nom : 
		    </label>
		  </th>
		  <td>
		    <input name="Nom" id="Nom" type="text" value="<?php echo $Nom ?>" size="20" maxlength="40" />
		  </td>
		</tr>
		<tr>
		  <th>
		    <label for="Prenom">
		      Pr&eacute;nom : 
		    </label>
		  </th>
		  <td>
		    <input name="Prenom" id="Prenom" type="text" value="<?php echo $Prenom ?>" size="20" maxlength="40" />
		  </td>
		</tr>
		<tr>
		  <td>
		    <label for="Tel">
		      <b>T&eacute;l&eacute;phone&nbsp;: </b>
		    </label>
		  </td>
		  <td>
		    <input name="Tel" id="Tel" type="text" value="<?php echo $Tel ?>" size="10" maxlength="10" />
		  </td>
		</tr>
		<tr>
		  <td>
		    <label for="Note">
		      <b>Notes :</b> 
		    </label>
		  </td>
		  <td>
		    <input name="RDV_PrisPar" type="hidden" value="<?php $user ?>" size="20" maxlength="20" />
		    <input name="Note" id="Note" type="text" value="<?php echo $Notes ?>" size="30" maxlength="100" />
		  </td>
		</tr>
	      </table>
	    </td>
	    <td>
	      <table>
		<tr>
		  <th valign="top" >
		    Répéter tous les
		  </th>
		  <td>
		    <select name="jours[]" title="options multiples possibles (touche Ctrl ou Maj)" multiple="multiple" size="7">
		      <option>
			Lundi
		      </option>
		      <option>
			Mardi
		      </option>
		      <option>
			Mercredi
		      </option>
		      <option>
			Jeudi
		      </option>
		      <option>
			Vendredi
		      </option>
		      <option>
			Samedi
		      </option>
		      <option>
			Dimanche
		      </option>
		    </select>
		  </td>
		</tr>
		<tr>
		  <th colspan="2" align="center">
		    Période
		  </th>
		</tr>
		<tr>
		  <th>
		    D&eacute;but : 
		  </th>
		  <td>
		    <input name="jour_debut_plage" id="datepicker" type="text" value="<?php echo $date; ?>" size="10" maxlength="10"/>
<!--
		    <input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_jour'].jour_debut_plage);" />
-->
		  </td>
		</tr>
		<tr>
		  <th>
		    Fin : 
		  </th>
		  <td>
		    <input name="jour_fin_plage" id="datepickeur" type="text" value="<?php echo $date; ?>" size="10" maxlength="10" />
<!--
		    <input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_jour'].jour_fin_plage);" />
-->
		  </td>
		</tr>
		<tr>
		  <th colspan="2" align="center" >
		    Plage horaire
		  </th>
		</tr>
		<tr>
		  <th>
		    <label for="Heure_debut_plage">
		      Heure de début : 
		    </label>
		  </th>
		  <td>
		    <input name="Heure_debut_plage" id="Heure_debut_plage" type="text" value="" size="2" maxlength="2" />
		    <input name="Minutes_debut_plage" type="text" value="00" size="2" maxlength="2" />
		  </td>
		</tr>
		<tr>
		  <th>
		    <label for="Heure_fin_plage">
		      Heure de fin : 
		    </label>
		  </th>
		  <td>
		    <input name="Heure_fin_plage" id="Heure_fin_plage" type="text" value="" size="2" maxlength="2" />
		    <input name="Minutes_fin_plage" type="text" value="00" size="2" maxlength="2" />
		  </td>
		</tr>
		<tr>
		  <th>
		  </th>
		  <td>
		    <input name="bouton_envoyer" id="Heure" type="submit" value="Répéter" />
		  </td>
		</tr>
	      </table>
	    </td>
	  </tr>
	</table>
	</fieldset>
      </form>
   </div>
<?php
include("inc/footer.php");
?>
