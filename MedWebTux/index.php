<?php
session_start() ;
include("config.php");
$db='';
if (file_exists ('connexion_serveur.php'))
{
  $db=mysqli_connect($host, $loginbase, $pwd);
  $codage=mysqli_query($db,"SET NAMES 'UTF8'");
}
include_once ("inc/header.php");
?>
    <title>
      MedWebTux - Connexion utilisateur
    </title>
<?php
if (file_exists ('connexion_serveur.php'))
{
  if (!mysqli_select_db($db,$base))
  {
?>
  <link rel="stylesheet" type="text/css" href="css/style.css" />
  <div class="erreur">
    <b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute;e sur <?php echo $_SERVER["HTTP_HOST"] ?>"<br /><br /></b>
    Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
    <a href="setup/index.php">Renseigner les donn&eacute;es de connexion au serveur Mysql</a>
<?php
    include("inc/footer.php");
	@rename("set_up", "setup");
	exit;
  }
}
else
{
?>
  <link rel="stylesheet" type="text/css" href="css/style.css" />
  <div class="erreur">
    <b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute;e sur <?php echo $_SERVER["HTTP_HOST"] ?>"<br /><br /></b>
    Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
    <a href="setup/index.php">Renseigner les donn&eacute;es de connexion au serveur Mysql</a>
<?php
    include("inc/footer.php");
	@rename("set_up", "setup");
	exit;
}
?>
    <script type="text/javascript" >
<!--
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
-->
    </script>

    <script type="text/javascript" src="oXHR.js">
    </script>

    <script type="text/javascript">
//<![CDATA[
function request(callback) 
{//AJAX pour chercher les utilisateurs signataires
	var xhr = getXMLHttpRequest();
	
	xhr.onreadystatechange = function() {
		if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 0)) {
			callback(xhr.responseText);
		}
	};
	var IdUser = encodeURIComponent(document.getElementById("login").value);
//on envoie a la page de verification le nom de login

	xhr.open("GET", "recherche_sign.php?User=" + IdUser, true);
	xhr.send(null);
	document.forms['form_login'].select_sign.onclick=null; //pour ne plus reinitialiser la liste une fois constituee, sinon impossible de choisir un element autre que le premier sur la liste a la souris
}

function readData(sData) 
{
  document.forms['form_login'].select_sign.length=0;
  if (sData!="NOP\n")
  {
  //document.formulaire_test.choix.options[document.formulaire_test.choix.length-1]
  //effacer l'ancien menu deroulant
    document.forms['form_login'].select_sign.length=0;

    signuser=sData.split('|'); //les utilisateurs signataires renvoyes par la page php, + l'utilisateur en premier
    for (var i in signuser) //creation d'une option de menu pour tous les utilisateurs signataires
    {
//enver les \n en fin de mot
      var signataire=signuser[i];
      signataire=signataire.replace("\n","");
      document.forms['form_login'].select_sign.options[document.forms['form_login'].select_sign.options.length] = new Option(signataire,signataire); 
    }
  }
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function verif_champ(sign_user,login,password)
{
  if (sign_user == "")
  { 
    alert("Le signataire n'est pas rempli\nCliquez sur le déroulant pour activer la liste \net choisissez un signataire");
    return false;
  }
  if  (login=="" || password=="")
  { 
    alert("Le login et le mot de passe doivent être remplis");
    return false;
  }
  return true;
}
//]]>
    </script>
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt"  onload="donner_focus('login')">
    <div class="conteneur">
<?php
$username=$loginbase."@".$host;

if (isset ($_GET['supprimer_verrou']))
{
  $sql_supprimer_verrous="DELETE FROM Verrous WHERE UserName='$username'";
  $resultat_supprimer_verrous=mysqli_query($db,$sql_supprimer_verrous);
}
if (isset ($_GET['page']))
{
  $page=$_GET['page'];
}
else
{
  $page="index";
}
if (isset ($_GET['message']))
{
  $message=$_GET['message'];
}
$message_erreur['message1']="Le mot de passe ou le login est faux.<br />Cause possible : verrouillage num&eacute;rique ou majuscule actif.<br />";
$message_erreur['message2']="Erreur de connexion au serveur.<br />Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL (3306, habituellement) n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br />";
$message_erreur['message3']="Le mot de passe est obligatoire.<br />Si l'utilisateur n'a pas de mot de passe, il est temps d'en mettre un&nbsp;!<br />";
$message_erreur['message4']="Vous n'avez pas les droits sur la liste des patients";
if ( isset( $_SESSION['login'] ) ) 
{
// insertion du menu d'en-tete	
$anchor='Connexion_Déconnexion';
  include("inc/menu-horiz.php");
		
?>
      <div class="groupe" >
	<h1>
	  MedWebTux - Dossier M&eacute;dical Patient
	</h1>
	<div class="login">
<?php
  $tab_login=explode("::",$_SESSION['login']);
  $user=$tab_login[0];

  $sql="SELECT * FROM Personnes WHERE Login ='$user'";
  $resultat=mysqli_query($db,$sql);
  $ligne=mysqli_fetch_array($resultat);
?>
	  <fieldset class="fieldset_login">
	    <legend>
	      Utilisateur actuel
	    </legend>
<?php
//---------------------- recuperer le delegue --------------------------
if (isset($_GET['utilisateur_autorisant'])) 
{
  $_SESSION['login'] = $user."::".$_GET['utilisateur_autorisant'];
  $tab_login=explode("::",$_SESSION['login']);
  $signuser=$tab_login[1];
  echo "<meta http-equiv=\"refresh\" content=\"0;url=index.php\">";
}
//---------------------- recuperer le delegue --------------------------
  echo "
	    <b>Login</b> : ".$user."<br />";
  echo "
	    <b>Délégué de </b> : ".$tab_login[1]."<br />";
  echo "
        ".$ligne["Titre"]." ".$ligne["Nom"]." ".$ligne["Prenom"]."<br />";
  echo "
	    <b>Adresse</b> : ".$ligne["Adresse"]."<br />";
  echo $ligne["CodePostal"]." ".$ligne["Ville"]."<br />";
  if ($ligne["Convention"])
    echo "
	    <b>Convention</b> : ".$ligne["Convention"]."<br />";
  echo "
	    <b>Qualification</b> : ".$ligne["Qualite"]."<br />";
  if ($ligne["NumOrdre"])
    echo "
	    <b>Num&eacute;ro d'ordre</b> : " .$ligne["NumOrdre"]."<br />";
  if ($ligne['NumRPPS'])
    echo "
	    <b>Num&eacute;ro RPPS</b> : " .$ligne['NumRPPS']."<br />";
  if ($ligne["Tel_1"])
    echo "
	    <b>T&eacute;l&eacute;phone 1</b> : " .$ligne["Tel_1"]."<br />";
  if ($ligne["Tel_2"])
    echo "
	    <b>T&eacute;l&eacute;phone 2</b> : " .$ligne["Tel_2"]."<br />";
  if ($ligne["Tel_3"])
    echo "
	    <b>T&eacute;l&eacute;phone 3</b> : " .$ligne["Tel_3"]."<br />";
  if ($ligne["EMail"])
    echo "
	    <b>E-Mail</b> : " .$ligne["EMail"]."<br />";
  if ($ligne["Note"])
    echo "
	    <b>Notes</b> : " .$ligne["Note"]."<br />";
?>
<br />
<form action="fiche_intervenant.php" method="get">
  <div>
    <input type="submit" name="submit_password" value="Modifier mot de passe et droits" />
    <input type="hidden" name="intervenant" value="<?php echo $ligne["ID_PrimKey"]?>" />
  </div>
</form>
<br /><br />
<form action="formulaire_correspondant.php" method="get">
  <div>
    <input type="submit" name="envoyer" value="Modifier" /> mon identité
    <input type="hidden" name="ID_corresp" value="<?php echo $ligne["ID_PrimKey"]?>" />
  </div>
</form>
<br /><br />
<?php
//---------------------------------------------------------------------------------------------
if (file_exists ('auth_cps.php')) {
	 if (file_exists ('sources/auth_cps_non_auto.txt')) {
	 	echo "<a class=\"a_bouton\" href=\"index.php?desactive=OUI\">Activer l'enregistrement automatique de la CPS</a>";
		if (isset($_GET['desactive']))
		{
		  if ($_GET['desactive'] == "OUI") {
			  @unlink("sources/auth_cps_non_auto.txt");
			  echo "<meta http-equiv=\"refresh\" content=\"0;url=index.php\">";
		  }
		}
	 }
	 else {
	 	echo "<a class=\"a_bouton\" href=\"index.php?desactive=NON\">Désactiver l'enregistrement automatique de la CPS</a>";
		if (isset($_GET['desactive']))
		{
		  if ($_GET['desactive'] == "NON") {
			  $handle = @fopen("sources/auth_cps_non_auto.txt","w");
			  @fclose($handle);
			  echo "<meta http-equiv=\"refresh\" content=\"0;url=index.php\">";
		  }
		}
	 }
}
//---------------- option active ou pas l'enregistrement automatique de la CPS ----------------
?>
	  </fieldset>
	</div><!-- fin resume utilisateur -->
<?php
}
else //page si non connecte
{
?>
<div class="nav noPrint">
	<ul class="select">
		<li><a href="#">Aide</a>
			<div class="select_sub">
				<ul class="sub">
					<li><a href="doc/index.html">Aide de MedWebTux <?php echo $version ; ?></a></li>
				</ul>
			</div>
		</li>
	</ul>
</div>

      <div class="groupe">
	<h1>
	  MedWebTux - Dossier M&eacute;dical Patient
	</h1>
	<noscript>
	  <div class="notice">
	    Votre navigateur ne supporte pas JavaScript : vous ne pourrez pas vous connecter.
	  </div>
	</noscript>
<?php
}
if (isset ($message))
{
echo "
	    <div class='notice'>
	      $message_erreur[$message]
	    </div>";
}
//Zone de saisie
?>
	<form method="post" action="verifLogin.php" id="form_login" onsubmit="return verif_champ(this.select_sign.value,this.login.value,this.password.value);"><!-- login-mot de passe -->
	  <div class="login">    
	    <fieldset class="fieldset_login">
	      <legend>
		Se connecter comme...
	      </legend>
	      <label for="login" class="questionnaire" >
		Identifiant&nbsp;:
	      </label>
	      <input type="text" name="login" id="login" size="10" title="Ce sont les login et mot de passe des utilisateurs de MedinTux et non ceux de MySQL. Cr&eacute;ez-les dans MedinTux s'ils n'existent pas." onchange="request(readData);" /><br />
	      <label for="password" class="questionnaire">
		Mot de passe&nbsp;:
	      </label>
	      <input type="password" name="password" id="password" size="10" /><br />
	      <label for="select_sign" class="questionnaire" >
		Délégué de&nbsp;:
	      </label>
	      <select name="select_sign" id="select_sign" style="width: 120px" onclick="request(readData);">
	      <!-- Options remplies par AJAX (fonction request) -->
	      </select><br />
	      <input type="submit" name="submit" value="Se connecter" />
<?php
//------------------------------ authentification par CPS -----------------------------------
if (file_exists ('auth_cps.php')) {
	echo "<br />";
?>
		<fieldset class="fieldset_login">
	      <legend>Se connecter par CPS</legend>
	<?php  
		$url_ssl = str_replace("index.php", "", $_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI']);
	?>
	      <a href="<?php echo 'https://'.$url_ssl.'auth_cps.php'; ?>"><img src="pics/cps.png" title="Se connecter par CPS" alt="Se connecter par CPS" /></a>
	     </fieldset>
<?php
}
//------------------------------ authentification par CPS -----------------------------------
?>
	    </fieldset>
	  </div>
	  <div class="login">
	    <!-- Le choix de la page de demarrage -->
	    <fieldset class="fieldset_login">
	      <legend>Démarrer avec</legend>
	      <div><label for="accueil" class="questionnaire" >
		Accueil 
	      </label>
	      <input type="radio" name="page" id="accueil" value="index" <?php echo 'checked="checked"'?>/></div>
	      <div><label for="agenda" class="questionnaire" >
		Agenda 
	      </label>
	      <input type="radio" name="page" id="agenda" value="agenda" <?php if ($page=="agenda") echo 'checked="checked"'?>/></div>
	      <div><label for="rep" class="questionnaire" >
		Rendez-vous répétés 
	      </label>
	      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="page" id="rep" value="rdv_repetes" <?php if ($page=="rdv_repetes") echo 'checked="checked"'?>/></div>
	      <div><label for="act" class="questionnaire" >
		Activité 
	      </label>
	      <input type="radio" name="page" id="act" value="activite" <?php if ($page=="activite") echo 'checked="checked"'?>/></div>
	      <div><label for="list" class="questionnaire" >
		Patients 
	      </label>
	      <input type="radio" name="page" id="list" value="liste" <?php if ($page=="liste") echo 'checked="checked"'?>/></div>
<!-- 	      <div><label for="new_file" class="questionnaire" >
	Nouveau dossier  
	      </label> 
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="page" id="new_file" value="nouveau_dossier" <?php if ($page=="nouveau_dossier") echo 'checked="checked"'?>/></div> -->
	      <div><label for="corr" class="questionnaire" >
		Intervenants 
	      </label>
	      <input type="radio" name="page" id="corr" value="correspondant" <?php if ($page=="correspondant") echo 'checked="checked"'?>/></div>
	     <div><label for="vidal" class="questionnaire" >
		Vidal 
	      </label>
	      <input type="radio" name="page" id="vidal" value="medocs" <?php if ($page=="medocs") echo 'checked="checked"'?>/></div>
	      <div><label for="inter" class="questionnaire" >
		Interactions
	      </label>
	      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="page" id="inter" value="interactions" <?php if ($page=="interactions") echo 'checked="checked"'?>/></div>
	      <div><label for="ccam" class="questionnaire" >
		CCAM 
	      </label>
	      <input type="radio" name="page" id="ccam" value="CCAM" <?php if ($page=="CCAM") echo 'checked="checked"'?>/></div>
	</fieldset>
</div>
	 </form>
	</div><!-- fin choix page de demarrage -->

<?php
if ( isset( $_SESSION['login'] ) ) 
{
//   Les verrous
$sql_verrous="SELECT * FROM Verrous INNER JOIN IndexNomPrenom ON Verrous.DossGUID=IndexNomPrenom.FchGnrl_IDDos WHERE Verrous.UserName='$username'";
$resultat_verrous=mysqli_query($db,$sql_verrous);
$count_verrous=mysqli_num_rows($resultat_verrous);
if ($count_verrous)
{
?>
      <fieldset class="fieldset">
	<legend>
	  Verrous
	</legend>

	<div class="notice">
	  <form action="index.php" method="get">
	    <div>
<?php
    if ($count_verrous==1)
      $pluriel="";
    else
      $pluriel="s";
    echo "
	      Vous avez ".$count_verrous." dossier".$pluriel." verrouill&eacute;".$pluriel." ";
?>
	      <input name="supprimer_verrou" type="submit" value="Enlever les verrous"/><br />
<?php
    while ($ligne_verrous=mysqli_fetch_array($resultat_verrous))
    {
      echo "
	      <a href=\"frame_patient.php?GUID=".$ligne_verrous['FchGnrl_IDDos']."\">".$ligne_verrous['FchGnrl_NomDos']." ".$ligne_verrous['FchGnrl_Prenom']."</a> (verrouill&eacute; le ". substr($ligne_verrous['StartTime'],6,2)."-".substr($ligne_verrous['StartTime'],4,2)."-".substr($ligne_verrous['StartTime'],0,4)." &agrave; ".substr($ligne_verrous['StartTime'],8,2)."h".substr($ligne_verrous['StartTime'],10,2).")<br />";
    }
?>


	    </div><!-- fin contenu form -->
	  </form>
	</div><!-- fin notice -->
      </fieldset>
<?php
  }
//---------------------------- insertion iframe pour maj ----------------------------------
echo "<div class=\"netupdate\">";
  include_once("inc/maj.php");
  include_once("sources/src.php");
echo "</div>";
//---------------------------- insertion iframe pour maj ----------------------------------
} //fin si connecte
?>
<!-- </div> fin groupe -->
 <div class="footer">
<?php
//---------------- verifie si le repertoire set_up est encore present ---------------------
$install = "set_up";
if (is_dir($install)) {
	echo "<div class=\"erreur\"><b>ATTENTION:</b> Par mesure de s&eacute;curit&eacute; apr&egrave;s l'installation de MedWebTux, il est vivement conseill&eacute; de supprimer de votre serveur web le dossier <b>set_up</b>. <a href=\"index.php?del_setup=set_up\">Supprimer</a></div>";
}
//---------------- verifie si le repertoire set_up est encore present ---------------------
?>
</div>
<?php
include("inc/footer.php");
?>
