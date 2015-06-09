<?php
session_start() ;
include("config.php");
//Si on n'est pas logue
if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php' );
  exit;
}
if (!$_GET['intervenant'])
{
  header('location: correspondant.php' );
  exit;
}
//Si un identifiant de correspondant n'est pas donne par l'URL, on renvoie sur le choix des correspondants
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
$date=date('Y-m-d', date('U'));

$envoyer_date="";
$montrer_patients="";

if (isset($_GET['envoyer_date']))
  $envoyer_date=$_GET['envoyer_date'];

if (isset($_GET['montrer_patients']))
  $montrer_patients=$_GET['montrer_patients'];

if(!$db)
{
  print "Erreur de connexion &agrave; $db<br/>";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br/>";
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

include("inc/header.php");

$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$utilisateur_autorisant=$tab_login[1];
$intervenant=$_GET['intervenant'];
$sql="SELECT * FROM Personnes WHERE ID_PrimKey=$intervenant"; //La personne dont on traite la fiche

$resultat=mysqli_query($db,$sql);
$ligne=mysqli_fetch_array($resultat);

//recherche droits du user connecte
$sql_user_droits="SELECT * FROM Personnes WHERE Login='$user'"; //l'utilisateur
$resultat_user_droits=mysqli_query($db,$sql_user_droits);
$count_user_droits=mysqli_num_rows($resultat_user_droits);
$ligne_user_droits=mysqli_fetch_array($resultat_user_droits);

$guid_user=$ligne_user_droits['GUID'];
?>
    <title>
      Intervenant <?php echo $ligne['Nom']." ".$ligne['Prenom'] ?> - Utilisateur <?php echo $_SESSION['login']?>
    </title>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen"/>
    <link rel="stylesheet" href="css/print.css" type="text/css" media="print"/>
    <!-- Ne pas afficher les colonnes techniques en mode impression -->
    <style type="text/css"  media="print">
	    td:nth-child(2) { display: none; }  
	    td:nth-child(3) { display: none; }  
	    td:nth-child(4) { display: none; }  
    </style>
    <script type="text/javascript" src="oXHR.js"></script>

    <script type="text/javascript">
//<![CDATA[
function request(callback) {
//AJAX pour verifier qu'un intervenant n'existe pas deja
  var xhr = getXMLHttpRequest();
  
  xhr.onreadystatechange = function() {
    if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 0)) {
      callback(xhr.responseText);
    }
  };
  var IdUser = encodeURIComponent(document.getElementById("login").value);
//on envoie a la page de verification le nom de login

  xhr.open("GET", "handlingData.php?User=" + IdUser, true);
  xhr.send(null);
}

function readData(sData) 
{
//fonction donnee en argument a request
  if (sData!="OK\n") // la chaine arrive terminee par un retour chariot.
  {
    alert (sData); //on affiche la chaine envoyee par handlingData.php
    donner_focus('login'); //on revient sur la zone de saisie qui a échoué
  }
}
//]]>
    </script>
    <script type="text/javascript">
//<![CDATA[
function assistant_droits(guid_sign,guid_friend,mode) 
{
  window.open('assistant_droits.php?guid_sign='+guid_sign+'&guid_friend='+guid_friend+'&mode='+mode,'Déterminer les droits','width=800,height=550,top=50,left=50,toolbar=no,scrollbars=yes,resizable=yes,location=no'); 
}
//]]>
    </script>

<script type="text/javascript">
<!-- 
function verif_vide(pwd)
{
  if (pwd == "")
  { 
  alert("Le champ n'est pas rempli\nMettez un mot de passe");
  return false;
  }
  return true;
}
-->
</script>
    <script type="text/javascript">
//<![CDATA[

function selectSave(t)
{
alert ('test');
selected = new Array();
for (var i = 0; i < t.options.length; i++)
selected.push(t.options[i].selected);
return selected;
}

function selectRestore(t,old)
{
alert ('test2');
selected = new Array();
for (var i = 0; i < old.length; i++)
t.options[i].selected=old[i];
return selected;
}
//]]>
    </script>

<?php
include 'calendar_javascript.php';
?>

  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" >
      <div class='noScreen'>
<!-- en-tete pour impression -->
<?php	
//    $user=$_SESSION['login'];
//On utilise l'en-tete de l'utilisateur autorisant
$sql_sign="SELECT * FROM Personnes WHERE Login ='$utilisateur_autorisant'";
$resultat_sign=mysqli_query($db,$sql_sign);
$ligne_sign=mysqli_fetch_array($resultat_sign);

echo "
	<strong>".$ligne_sign["Titre"]." ".$ligne_sign["Nom"]." ".$ligne_sign["Prenom"]."</strong><br />";
echo '
	'.$ligne_sign["Adresse"]."<br />";
echo '
	'.$ligne_sign["CodePostal"]." ".$ligne_sign["Ville"]."<br />";
if ($ligne_sign["Convention"])
  echo '
	'.$ligne_sign["Convention"]."<br />";
echo '
	'.$ligne_sign["Qualite"]."<br />";
if ($ligne_sign["NumOrdre"])
  echo "
	  <strong>Num&eacute;ro d'ordre</strong>&nbsp;: " .$ligne_sign["NumOrdre"]."<br />";
if ($ligne_sign["NumRPPS"])
  echo "
	  <strong>Num&eacute;ro RPPS</strong>&nbsp;: " .$ligne_sign["NumRPPS"]."<br />";
if ($ligne_sign["Tel_1"])
  echo "
	  <strong>T&eacute;l</strong>&nbsp;: " .$ligne_sign["Tel_1"]."<br />";
/*if ($ligne_sign["Tel_2"])
	echo "<strong>T&eacute;l&eacute;phone 2</strong> : " .$ligne_sign["Tel_2"]."<br />";
if ($ligne_sign["Tel_3"])
	echo "<strong>T&eacute;l&eacute;phone 3</strong> : " .$ligne_sign["Tel_3"]."<br />";
*/
if ($ligne_sign["EMail"])
  echo "
	  <strong>E-Mail</strong>&nbsp;: " .$ligne_sign["EMail"]."<br />";
 //fin en-tete medecin
?>
	</div><!-- fin noscreen en tete -->
<?php	
  // insertion du menu d'en-tete et du formulaire de recherche	
$anchor='Fiche_d_intervenant';
include("inc/menu-horiz.php");
?>
    <div class="groupe">
	<h1 class="noPrint" >
	  MedWebTux - Intervenant
	</h1>

<?php
//Si une date de debut a ete demandee, on la recupere et on la formate 
if (isset($_GET['debut_court'])) //format local
{
  $debut_court=$_GET['debut_court']; //pour utilisation dans les zones de saisie
  $debut=local_to_iso($debut_court,$date_format)." 00:00:00";
}
else//Sinon, on prend la date du jour
{
  $debut_court=iso_to_local(date('Y-m-d', date('U')),$date_format);
  $debut=date('Y-m-d', date('U'))." 00:00:00"; //pour requetes sql
}
//Si une date de fin a ete demandee, on la recupere et on la formate 
if (isset($_GET['fin_court'])) //local
{
  $fin_court=$_GET['fin_court'];
  $fin=local_to_iso($fin_court,$date_format)." 23:59:59";
}
//Sinon, on prend la date du jour
else 
{
  $fin_court=iso_to_local(date('Y-m-d', date('U')),$date_format);
  $fin=date('Y-m-d', date('U'))." 23:59:59";
}
?>

<?php
$qrcode_string="BEGIN:VCARD\nVERSION:2.1\n";
/*  BEGIN:VCARD
  VERSION:2.1
  N;CHARSET=UTF-8:'.$Nom.';'.$Prenom.';;'.$Titre.'
  FN;CHARSET=UTF-8:'.$Prenom.' '.$Nom.'
  TITLE;CHARSET=UTF-8:'.$Qualite.'
  ADR;WORK;CHARSET=UTF-8:;'.$Adresse.';'.$Ville.';;'.$CodePostal.';
  TEL;WORK:'.$Tel_1.'
  EMAIL;PREF;INTERNET:'.$EMail.'
  NOTE;CHARSET=UTF-8:'.$Note.'
  END:VCARD';
*/
?>
	  <fieldset class="fieldset_intervenant"  style="float:left;">
	    <legend>
	      Fiche intervenant
	    </legend>
<?php
echo "
	    <strong>".$ligne['Titre']." ".stripslashes($ligne['Nom'])." ".stripslashes($ligne['Prenom'])."</strong><br />";
//correspondant.php?Qualite=ALCOOLOGUE&critere_recherche=Nom&cle=&intervenant_user[]=users&intervenant_user[]=no_users&envoyer=Chercher
echo '
	    <a href="correspondant.php?Qualite='.$ligne['Qualite'].'&critere_recherche=Nom&cle=&intervenant_user[]=users&intervenant_user[]=no_users&envoyer=Chercher">'.$ligne['Qualite']."</a><br />";
$qrcode_string.='N;CHARSET=UTF-8:'.$ligne['Nom'].';'.$ligne['Prenom'].';;'.$ligne['Titre']."\nFN;CHARSET=UTF-8:".$ligne['Prenom'].' '.$ligne['Nom']."\nTITLE;CHARSET=UTF-8:".$ligne['Qualite']."\n";

if ($ligne['NumOrdre'])
{
  echo "
	    <strong>Ordre : </strong>".$ligne['NumOrdre']."<br />";
}
if ($ligne['NumRPPS'])
{
  echo "
	    <strong>RPPS : </strong>".$ligne['NumRPPS']."<br />";
}
$adr=preg_replace("`\n`","<br />",$ligne['Adresse']);
echo '
	    '.$adr."
	    <br />".$ligne['CodePostal']." ".$ligne['Ville']."
	    <br />";
$qrcode_string.='ADR;WORK;CHARSET=UTF-8:;'.$adr.';'.$ligne['Ville'].';;'.$ligne['CodePostal']."\n";

if ($ligne['Tel_1'])
{
  echo "
	    <strong>T&eacute;l&eacute;phone</strong>";
  if ($ligne['Tel_Type1'])
  {
    echo " (".$ligne['Tel_Type1'].")";
  }
  echo " : ".$ligne['Tel_1'];
  $qrcode_string.='TEL;WORK:'.$ligne['Tel_1']."\n";
  if ($ligne['Tel_Abr_1'])
	  echo " (".$ligne['Tel_Abr_1'].")";
echo "
	      <br />";
}
if ($ligne['Tel_2'])
{
  echo "
	      <strong>T&eacute;l&eacute;phone</strong>";
  if ($ligne['Tel_Type2'])
  {
    echo " (".$ligne['Tel_Type2'].")";
  }
  echo " : ".$ligne['Tel_2'];
  if ($ligne['Tel_Abr_2'])
    echo " (".$ligne['Tel_Abr_2'].")";
  echo "
	      <br />";
}
if ($ligne['Tel_3'])
{
  echo "
	      <strong>T&eacute;l&eacute;phone </strong>";
  if ($ligne['Tel_Type3'])
  {
    echo " (".$ligne['Tel_Type3'].")";
  }
  echo " : ".$ligne['Tel_3'];
  if ($ligne['Tel_Abr_3'])
    echo " (".$ligne['Tel_Abr_3'].")";
  echo "
	      <br />";
}
if ($ligne['EMail'])
{
  echo "
	      <strong>E-Mail</strong>&nbsp;: <a href=\"mailto:".$ligne['EMail']."\">".$ligne['EMail']."</a>
	      <br />";
  $qrcode_string.='EMAIL;PREF;INTERNET:'.$ligne['EMail']."\n";
}
if ($ligne['Note'])
{
  echo "
	      <strong>Notes</strong>&nbsp;: <br />".preg_replace("`\n`i","<br />",$ligne['Note'])."
	      <br />";
  $qrcode_string.='NOTE;CHARSET=UTF-8:'.$ligne['Note']."\n";
}
if ($ligne['Sexe'])
{
?>
	      <strong>Sexe&nbsp;:</strong> <?php echo $ligne['Sexe'] ?>
	      <br />
<?php
}
if ($ligne['Cher'])
{
?>
	      <strong>Politesse&nbsp;: </strong><?php echo $ligne['Cher'] ?>
	      <br />
<?php
}
$qrcode_string.='END:VCARD';
$qrcode_string=urlencode($qrcode_string);
?>
	      <img src="phpqrcodeimg.php?qrcode_string=<?php echo $qrcode_string;?>" alt="qrcode" width="125" title="Scannez cette image de QR-Code avec votre Smartphone pour ajouter cette fiche à vos contacts" />
	      <form action="formulaire_correspondant.php"  method="get"> <!--Bouton de modification de fiche--> 

		<div class='noPrint'>
		  <input name="ID_corresp" type="hidden" value="<?php echo $intervenant?>" />
		  <br />
		  <input name="envoyer" type="submit" value="Modifier" />
<?php
//ne pas supprimer une fiche intervenant si on n'est pas admin
if (stripos($ligne_user_droits['Droits'],"-adm-"))
{
?>
		  <input name="envoyer" type="submit" value="Supprimer" />
<?php
}
?>
		</div>
	      </form>
	    </fieldset>
	      <form action="fiche_intervenant.php" method="get" style="float:left;" class="noPrint" >
		<fieldset class="fieldset_intervenant">
		  <legend>
		    Patients li&eacute;s
		  </legend>
		  <input name="intervenant" type="hidden" value="<?php echo $intervenant ?>" />
		  <input name="montrer_patients" type="submit" value="<?php if ($montrer_patients=='Montrer') echo 'Cacher'; else echo 'Montrer' /*bouton a bascule */?>" />
		</fieldset>
	      </form>
			
	 <!-- les dates -->
	      <form action="fiche_intervenant.php" method="get" id="form_recherche_date" style="float:left;"  class="noPrint">
		<fieldset class="fieldset_intervenant" title="Pour rechercher toutes les consultations de patients liés à cet intervenant">
		  <legend>
		    Période de consultations
		  </legend>
		  <input name="intervenant" type="hidden" value="<?php echo $intervenant; ?>" />
		  <table>
		    <tr>
		      <td>
			<label for="debut_court">D&eacute;but :</label> 
		      </td>
		      <td>		  
			<input name="debut_court" id="datepicker" type="text" value="<?php echo $debut_court;?>" size="10" maxlength="10" />
<!--
			<input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_recherche_date'].debut_court);" />
-->
		      </td>
		    </tr>
		    <tr>
		      <td>
			<label for="fin_court">Fin :</label>
		      </td>
		      <td>
			<input name="fin_court" id="datepickeur" type="text" value="<?php echo $fin_court;?>" size="10" maxlength="10" />
<!--
			<input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_recherche_date'].fin_court);" />
-->
		      </td>
		    </tr>
		    <tr>
		      <td colspan="2"><input name="envoyer_date" type="submit" value="Chercher" />
		      </td>
		    </tr>
		  </table>
		</fieldset>
	      </form>
<?php
//affichage des champs de login si administrateur ou utilisateur=fiche affichée

if ($ligne_user_droits['Login']==$ligne['Login'] OR stripos($ligne_user_droits['Droits'],"-adm-"))
{
?>
	      <form action="valid_user.php" id="form_user" method="get" style="float:left;" class="noPrint"  >
		<fieldset class="fieldset_intervenant">
		  <legend>
		    Intervenant en tant qu'utilisateur
		  </legend>
		  <div id="user">
		    <div>
		      <input type="hidden" value="<?php echo $intervenant ?>" name="intervenant"  />
		      <table>
			<tr>
			  <td>
			    <label for="login">Login : </label>
			  </td>
			  <td>
			    <input type="text" id="login" name="login" value="<?php echo $ligne['Login'] ?>"  <?php if ($ligne['Login']) echo 'readonly="readonly"'?> size='10' onchange="request(readData);" />
			    <span id="loader" style="display: none;">
			      <img src="images/loader.gif" alt="loading" />
			    </span>
			  </td>
			</tr>
			<tr>
			  <td>
			    <label for="pwd">Mot de passe : </label>
			  </td>
			  <td>
			    <input type="password" id="pwd" name="pwd" value="" maxlength="8" size="8" title="8 caractères maximum" />
			  </td>
			</tr>
		      </table>
		    </div>
<div <?php if (!stripos($ligne_user_droits['Droits'],"-adm-")) echo  "class='noScreen'" ?>>
		    <strong>Droits</strong>
		    <select name="droits[]" multiple="multiple" size="5" >
		      <option value="nbv" <?php if (preg_match("`nbv`",$ligne['Droits'])) echo "selected='selected'"; ?>>Ne peut pas débloquer les bases</option>
		      <option value="idd" <?php if (preg_match("`idd`",$ligne['Droits'])) echo "selected='selected'";  ?>>Ne peut pas effacer le dossier patient</option>
		      <option value="ata" <?php if (preg_match("`ata`",$ligne['Droits'])) echo "selected='selected'";  ?>>Accéder aux antécédents</option>
		      <option value="adm" <?php if (preg_match("`adm`",$ligne['Droits'])) echo "selected='selected'";  ?>>Administrer</option>
		      <option value="bic" <?php if (preg_match("`bic`",$ligne['Droits'])) echo "selected='selected'";  ?>>Créer biologie</option>
		      <option value="clc" <?php if (preg_match("`clc`",$ligne['Droits'])) echo "selected='selected'";  ?>>Créer classeur</option>
		      <option value="img" <?php if (preg_match("`img`",$ligne['Droits'])) echo "selected='selected'";  ?>>Crer imagerie</option>
		      <option value="gln" <?php if (preg_match("`gln`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer dossiers dans le glossaire</option>
		      <option value="cec" <?php if (preg_match("`cec`",$ligne['Droits'])) echo "selected='selected'";  ?>>Créer certificats</option>
		      <option value="coc" <?php if (preg_match("`coc`",$ligne['Droits'])) echo "selected='selected'";  ?>>Créer courriers</option>
		      <option value="doc" <?php if (preg_match("`doc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer des documents</option>
		      <option value="iec" <?php if (preg_match("`iec`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer documents dans l'identification</option>
		      <option value="obc" <?php if (preg_match("`obc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer observations</option>
		      <option value="orc" <?php if (preg_match("`orc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer prescriptions</option>
		      <option value="etc" <?php if (preg_match("`etc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer et effacer les antécédents</option>
		      <option value="idc" <?php if (preg_match("`idc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer et modifier l'identité</option>
		      <option value="plc" <?php if (preg_match("`plc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Créer un patient</option>
		      <option value="gld" <?php if (preg_match("`gld`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Déplacer le glossaire</option>
		      <option value="nog" <?php if (preg_match("`nog`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Disposer du nomadisme</option>
		      <option value="gls" <?php if (preg_match("`gls`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Effacer des dossiers du glossaire</option>
		      <option value="agm" <?php if (preg_match("`agm`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Gérer un agenda multiple</option>
		      <option value="bip" <?php if (preg_match("`bip`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer biologie</option>
		      <option value="clp" <?php if (preg_match("`clp`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer classeurs</option>
		      <option value="imp" <?php if (preg_match("`imp`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer imagerie</option>
		      <option value="cep" <?php if (preg_match("`cep`",$ligne['Droits'])) echo "selected='selected'";  ?>>Imprimer certificats</option>
		      <option value="cop" <?php if (preg_match("`cop`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer courriers</option>
		      <option value="dop" <?php if (preg_match("`dop`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer documents</option>
		      <option value="iep" <?php if (preg_match("`iep`",$ligne['Droits'])) echo "selected='selected'";  ?>>Imprimer documents de l'identification</option>
		      <option value="obp" <?php if (preg_match("`obp`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer observations</option>
		      <option value="orp" <?php if (preg_match("`orp`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer prescriptions</option>
		      <option value="plp" <?php if (preg_match("`plp`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Imprimer un patient</option>
		      <option value="idg" <?php if (preg_match("`idg`",$ligne['Droits'])) echo "selected='selected'";  ?>>Lancer un dossier patient</option>
		      <option value="cem" <?php if (preg_match("`cem`",$ligne['Droits'])) echo "selected='selected'";  ?>>Modifier les certificats</option>
		      <option value="com" <?php if (preg_match("`com`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Modifier les courriers</option>
		      <option value="iem" <?php if (preg_match("`iem`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Modifier documents de l'identification</option>
		      <option value="orm" <?php if (preg_match("`orm`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Modifier les prescriptions</option>
		      <option value="glc" <?php if (preg_match("`glc`",$ligne['Droits'])) echo "selected='selected'";  ?>>Modifier le glossaire</option>
		      <option value="atc" <?php if (preg_match("`atc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Modifier les antécédents</option>
		      <option value="tvc" <?php if (preg_match("`tvc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Modifier les tableaux variables</option>
		      <option value="ttc" <?php if (preg_match("`ttc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Modifier le traitement de fond</option>
		      <option value="agc" <?php if (preg_match("`agc`",$ligne['Droits'])) echo "selected='selected'";  ?>>Prendre des rendez-vous</option>
		      <option value="cer" <?php if (preg_match("`cer`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Renouveler un certificat</option>
		      <option value="cor" <?php if (preg_match("`cor`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Renouveler le courrier</option>
		      <option value="orr" <?php if (preg_match("`orr`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Renouveler une prescription</option>
		      <option value="adr" <?php if (preg_match("`adr`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Résoudre les doublons</option>
		      <option value="fse" <?php if (preg_match("`fse`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Saisir une FSE</option>
		      <option value="rgl" <?php if (preg_match("`rgl`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Saisir un règlement</option>
		      <option value="ccc" <?php if (preg_match("`ccc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Utiliser la base CCAM</option>
		      <option value="bmc" <?php if (preg_match("`bmc`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Utiliser la base médicamenteuse</option>
		      <option value="cic" <?php if (preg_match("`cic`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Utiliser la CIM 10</option>
		      <option value="lac" <?php if (preg_match("`lac`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Utiliser le labo</option>
		      <option value="med" <?php if (preg_match("`med`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Utiliser MedinTux</option>
		      <option value="plv" <?php if (preg_match("`plv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir la liste des patients</option>
		      <option value="biv" <?php if (preg_match("`biv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir la biologie</option>
		      <option value="clv" <?php if (preg_match("`clv`",$ligne['Droits'])) echo "selected='selected'";  ?>>Voir les classeurs</option>
		      <option value="imv" <?php if (preg_match("`imv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir l'imagerie</option>
		      <option value="atv" <?php if (preg_match("`atv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les antécédents</option>
		      <option value="cev" <?php if (preg_match("`cev`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les certificats</option>
		      <option value="cov" <?php if (preg_match("`cov`",$ligne['Droits'])) echo "selected='selected'";  ?>>Voir les courriers</option>
		      <option value="dov" <?php if (preg_match("`dov`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les documents</option>
		      <option value="iev" <?php if (preg_match("`iev`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les documents de l'identification</option>
		      <option value="obv" <?php if (preg_match("`obv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les observations</option>
		      <option value="orv" <?php if (preg_match("`orv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les prescriptions</option>
		      <option value="agv" <?php if (preg_match("`agv`",$ligne['Droits'])) echo "selected='selected'";  ?>>Voir les rendez-vous</option>
		      <option value="tvv" <?php if (preg_match("`tvv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir les tableaux variables</option>
		      <option value="sld" <?php if (preg_match("`sld`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir l'état du solde</option>
		      <option value="ttv" <?php if (preg_match("`ttv`",$ligne['Droits'])) echo "selected='selected'";  ?>>Voir le traitement de fond</option>
		      <option value="idv" <?php if (preg_match("`idv`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Voir l'identité</option>
		      <option value="sgn" <?php if (preg_match("`sgn`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Utilisateur signataire</option>
  <!-- 		    droits pour les rubriques arbitraires -->
<?php
/*$rubrique_perso[] = "Observation|ob|20030000";
$rubrique_perso[] = "Prescription libre|or|20020200";
$rubrique_perso[] = "Ordonnance structurée|or|20020100";
$rubrique_perso[] = "Terrain|tv|20060000";
$rubrique_perso[] = "Document|do|20080000";
$rubrique_perso[] = "Certificat|ce|20020300";
$rubrique_perso[] = "Courrier|co|20020500";
$rubrique_perso[] = "Notes avancées|ie|20090000";
$rubrique_perso[] = "Classeur|cl|20050000";
//les rubriques perso
$rubrique_perso[] = "Alertes|al|90010000";
*/
  foreach ($rubrique_perso AS $this_rubrique_perso)
  {
    if (!preg_match('`20030000`',$this_rubrique_perso) AND !preg_match('`20020200`',$this_rubrique_perso) AND !preg_match('`20020100`',$this_rubrique_perso) AND !preg_match('`20060000`',$this_rubrique_perso) AND !preg_match('`20080000`',$this_rubrique_perso) AND !preg_match('`20020300`',$this_rubrique_perso) AND !preg_match('`20020500`',$this_rubrique_perso) AND !preg_match('`20090000`',$this_rubrique_perso) AND !preg_match('`20050000`',$this_rubrique_perso) )
    {
      $tableau_rubrique=explode('|',$this_rubrique_perso);
      $see=$tableau_rubrique[1].'v';
      $create=$tableau_rubrique[1].'c';
      $print=$tableau_rubrique[1].'p';
?>
		      <option value="<?php echo $see ?>" <?php if (preg_match("`$see`",$ligne['Droits'])) echo "selected='selected'" ; ?>>Peut voir la rubrique <?php echo $tableau_rubrique[0] ?></option>
		      <option value="<?php echo $create ?>" <?php if (preg_match("`$create`",$ligne['Droits'])) echo "selected='selected'";  ?>>Peut écrire dans la rubrique <?php echo $tableau_rubrique[0] ?></option>
		      <option value="<?php echo $print ?>" <?php if (preg_match("`$print`",$ligne['Droits'])) echo "selected='selected'";  ?>>Peut imprimer la rubrique <?php echo $tableau_rubrique[0] ?></option>
<?php
    }
  }
?>
		    </select>
</div>
		    <br />
		    <input type="submit" value="Valider" name="submit_user" id="submit_user" onclick="return verif_vide(this.form.pwd.value);"/>
		  </div>
		</fieldset>
	      </form>
<?php
  if ($ligne['Login']) //pas besoin de droits délégués si intervenant pas utilisateur
  {
    if (preg_match('/adm/',$ligne_user_droits['Droits']) OR preg_match('/adm/',$droits) ) //l'utilisateur peut etre place comme delegue de si la personne connectee est admin.
    {
?>
	      <form action="valid_user.php" id="form_delegue" method="get" style="float:left;"  class="noPrint" >
		<fieldset>
		  <legend>
		    Délégué de
		  </legend>
<?php
    }
    $sql_chercher_users="SELECT * FROM Personnes WHERE Login !=''"; //Les intervenants utilisateurs - requetes qui servent aussi pour les signataires
    $resultat_chercher_users=mysqli_query($db,$sql_chercher_users);
    $count_chercher_users=mysqli_num_rows($resultat_chercher_users);

    $GUID_intervenant=$ligne['GUID'];
    $sql_select_delegue="SELECT * FROM Personnes INNER JOIN user_perms ON Personnes.GUID=user_perms.SignataireGUID WHERE user_perms.FriendUserGUID='$GUID_intervenant'";

    $resultat_select_delegue=mysqli_query($db,$sql_select_delegue);
    $count=mysqli_num_rows($resultat_select_delegue);
    if (preg_match('/adm/',$ligne_user_droits['Droits']) OR preg_match('/adm/',$droits) ) //l'utilisateur peut etre place comme delegue de si la personne connectee est admin.
    {
?>
		  <input type="hidden" name="GUID_intervenant" value="<?php echo $GUID_intervenant ?>" />
		  <input type="submit" name="submit_delegue" value="Valider"/>
		  <table>
<?php
    while ($ligne_chercher_users=mysqli_fetch_array($resultat_chercher_users))
    {
      $coche=0;
      $droits="";
      if ($count)
      {
	mysqli_data_seek($resultat_select_delegue,0);
      }
      while ($ligne_select_delegue=mysqli_fetch_array($resultat_select_delegue))
      {
	if ($ligne_select_delegue['SignataireGUID']==$ligne_chercher_users['GUID'])
	{
	  $coche=1;
	  $droits=$ligne_select_delegue['FriendUserDroits'];
	}
      }
      if ($ligne_chercher_users['Login'] !=$ligne['Login'] AND preg_match('/sgn/',$ligne_chercher_users['Droits'])) //utilisateur lui-meme et les utilisateurs non signataires
      {
?>
		    <tr>
		      <td>
			<input type="checkbox" name="check_sign[]" id="check_del_<?php echo $ligne_chercher_users['GUID']?>" value="<?php echo $ligne_chercher_users['GUID']; ?>" <?php if ($coche) echo "checked='checked'"?> />
			<label for="check_del_<?php echo $ligne_chercher_users['GUID']?>">
<?php 
    echo $ligne_chercher_users['Login'] ?>
			</label>
		      </td>
		      <td>
			<input type="text" name="droits_<?php echo $ligne_chercher_users['Login']; ?>" id="droits_<?php echo $ligne_chercher_users['Login']; ?>" value="<?php echo $droits ?>" 
<?php 
    if (!preg_match('/adm/',$ligne_user_droits['Droits']) AND !preg_match('/adm/',$droits) ) //les droits peuvent etre modifies si la personne connectee est admin.
      echo "readonly='readonly'";
?>
  />
		      </td>
		      <td>
			<input type="button" name="assistant" value="Assistant" onclick="assistant_droits('<?php echo $ligne_chercher_users['GUID'] ?>','<?php echo $ligne['GUID'] ?>','delegue')" />
		      </td>
		    </tr>
<?php
    }
  }
?>
		</table>
		</fieldset>
	      </form>
<?php
}
?>
	      <form action="valid_user.php" id="form_sign" method="get" style="float:left;"  class="noPrint" >
		<fieldset>
		  <legend>
		    Signataire pour
		  </legend>
<?php
if (preg_match('/sgn/',$ligne['Droits']))
{
?>
		  <input type="hidden" name="GUID_intervenant" value="<?php echo $GUID_intervenant ?>" />
		  <input name="submit_sign" type="submit" value="Valider" />
		  <table>
<?php
  $sql_select_signataire="SELECT * FROM Personnes INNER JOIN user_perms ON Personnes.GUID=user_perms.SignataireGUID WHERE user_perms.SignataireGUID='$GUID_intervenant'";
  $resultat_select_signataire=mysqli_query($db,$sql_select_signataire);
  $count_select_signataire=mysqli_num_rows($resultat_select_signataire);
  mysqli_data_seek($resultat_chercher_users,0);
  while ($ligne_chercher_users=mysqli_fetch_array($resultat_chercher_users))
  {
    $droits="";
    $coche=0;
    if ($count_select_signataire)
      mysqli_data_seek($resultat_select_signataire,0);
    while ($ligne_select_signataire=mysqli_fetch_array($resultat_select_signataire))
    {
      if ($ligne_select_signataire['FriendUserGUID']==$ligne_chercher_users['GUID'])
      {
	$coche=1;
	$droits=$ligne_select_signataire['FriendUserDroits'];
      }
    }
    if ($ligne_chercher_users['Login'] !=$ligne['Login'] ) //utilisateur lui-meme
    {
?>
		    <tr>
		      <td>
			<input type="checkbox" name="check_sign[]" id="check_sign_<?php echo $ligne_chercher_users['GUID'] ?>" value="<?php echo $ligne_chercher_users['GUID']; ?>" <?php if ($coche) echo "checked='checked'"?> />
			<label for="check_sign_<?php echo $ligne_chercher_users['GUID'] ?>">
			  <?php echo $ligne_chercher_users['Login'] ?>
			</label>
		      </td>
		      <td>
			<input type="text" name="droits_sign_<?php echo $ligne_chercher_users['Login'] ?>" id="droits_sign_<?php echo $ligne_chercher_users['Login'] ?>" value="<?php echo $droits ?>" />
			<input type="button" name="button_asssistant" value="Assistant" onclick="assistant_droits('<?php echo $ligne_chercher_users['GUID'] ?>','<?php echo $ligne['GUID'] ?>','signataire')"/>
		      </td>
		    </tr>
<?php
    }
  }
?>
		  </table>
<?php
}
else
  echo "Utilisateur non signataire";
?>
		</fieldset>
	      </form>
<?php
  }
}
?>
<?php
if ($envoyer_date=="Chercher")//On cherche les consultations de patients lies a cet intervenant durant cette periode
{
  $sql2="SELECT * FROM fchpat_Intervenants INNER JOIN IndexNomPrenom ON fchpat_Intervenants.fchpat_Intervenants_PatGUID=IndexNomPrenom.FchGnrl_IDDos INNER JOIN RubriquesHead ON IndexNomPrenom.FchGnrl_IDDos=RubriquesHead.RbDate_IDDos WHERE fchpat_Intervenants_IntervPK='$intervenant' AND RbDate_Date>='$debut' AND RbDate_Date<='$fin' AND RbDate_TypeRub=20030000 ORDER BY RbDate_Date,FchGnrl_NomDos,FchGnrl_Prenom";
}
else
  $sql2="SELECT * FROM fchpat_Intervenants INNER JOIN IndexNomPrenom ON fchpat_Intervenants.fchpat_Intervenants_PatGUID=IndexNomPrenom.FchGnrl_IDDos WHERE fchpat_Intervenants_IntervPK='$intervenant' ORDER BY FchGnrl_NomDos,FchGnrl_Prenom" ;

$resultat2=mysqli_query($db,$sql2);
$count2=mysqli_num_rows($resultat2);
if ($envoyer_date=="Chercher")
{
?>
		<div class="information"	title="Un clic sur la date affiche la consultation,
				  Un clic sur un nom affiche le dossier du patient">
		  <table>
<?php
  if ($debut_court==$fin_court)
    echo "
		    <tr>
		      <th>
			Consultations de patients li&eacute;s &agrave; cet intervenant le ".$debut_court." : ".$count2."
		      </th>
		    </tr>";
  else
    echo "
		    <tr>
		      <th>
			Consultations de patients li&eacute;s &agrave; cet intervenant  du ".$debut_court." au ".$fin_court." : ".$count2."
		      </th>
		    </tr>";
  while	($ligne2=mysqli_fetch_array($resultat2))
  {
    $date_court=local_to_iso(substr($ligne2['RbDate_Date'],0,10),$date_format);
    echo "
		    <tr>
		      <td>
			<a href=\"consultation.php?numeroID=".$ligne2['FchGnrl_IDDos']."&amp;date=".$ligne2['RbDate_Date']."&amp;nom=".$ligne2['FchGnrl_NomDos']."&amp;prenom=".$ligne2['FchGnrl_Prenom']."\">".$date_court."</a> <a href=\"patient.php?GUID=".$ligne2['FchGnrl_IDDos']."\">".$ligne2['FchGnrl_NomDos']." ".$ligne2['FchGnrl_Prenom']."</a>
		      </td>
		    </tr>";
  }
?>
		</table>
	      </div>

<?php
}
elseif ($montrer_patients=="Montrer")
{
?>
	      <div class="information">
		  <table>
		    <tr>
		      <th>
			Patients li&eacute;s &agrave; cet intervenant : <?php echo $count2 ?>
		      </th>
		      <td> <!--Nouveau lien -->
			<form action="liaison_correspondant.php" method="get"><!-- bouton Lier -->
			  <div>
			    <input name="ID_corresp" type="hidden" value="<?php echo $intervenant?>" />
			    <input name="envoyer" type="submit" value="Lier un nouveau patient" />
			  </div>
			</form>
		      </td>
		      <td valign="top">
			<br/>
		      </td>
		    </tr>
<?php
  while ($ligne2=mysqli_fetch_array($resultat2))//liste des patients lies
  {
    echo "
		  <tr>
		    <td>
		      <a href=\"patient.php?GUID=".$ligne2['FchGnrl_IDDos']."\">".$ligne2['FchGnrl_NomDos']."</a> ".$ligne2['FchGnrl_Prenom'];
?>
		    </td>
		    <td>
		      <form action="liaison_correspondant.php" method="get"><!--bouton de suppression de liaison -->
			<div>
			  <input name="from" type="hidden" value="intervenant" />
			  <input name="ID_corresp" type="hidden" value="<?php echo $intervenant ?>" />
			  <input name="patient_ID" type="hidden" value="<?php echo $ligne2['FchGnrl_IDDos'] ?>" />
			  <input name="ID_fiche_liaison" type="hidden" value="<?php echo $ligne2['fchpat_Intervenants_PK'] ?>" />
			  <input name="envoyer" type="submit" value="Annuler le lien" />
			</div>
		      </form>
		    </td>
		    <td>
		      
		    </td>
		  </tr>
<?php
  } //fin des patients lies
?>
		</table>
	    
	      </div>
<?php
}
?>
	    </div>
	<div class='noPrint'>
<?php
include("inc/footer.php");
?>
	
