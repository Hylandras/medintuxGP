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
//on ajoute le champ lock si n'existe pas

$sql_ajouter_lock="ALTER TABLE `agenda` ADD `lock` BOOLEAN NOT NULL DEFAULT '0'";
$resultat_ajouter_lock=mysqli_query($db,$sql_ajouter_lock);

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

//les dates de recherche selon la langue
$date=iso_to_local(date('Y-m-d', date('U')),$date_format);

//initialisation des jours de la semaine
$text_jsemaine=array(0=>"Dimanche",1=>"&nbsp;&nbsp;Lundi&nbsp;&nbsp;",2=>"&nbsp;&nbsp;Mardi&nbsp;&nbsp;",3=>"Mercredi",4=>"&nbsp;&nbsp;Jeudi&nbsp;&nbsp;",5=>"Vendredi",6=>"&nbsp;Samedi&nbsp;");

//On recupere les variables dans l'URL
$Type="%%";
if (isset($_GET['Type'])) //consultation, visite, etc.
{
  $Type=$_GET["Type"];
  if ($Type=="Tous")
    $Type='%%';
}

$status="%%";
if (isset($_GET['status'])) //Rdv termine, annule, etc.
{
  $status=stripslashes($_GET["status"]);
  if ($status=="Tous")
    $status='%%';
}
//echo $status;

$intervenants='seul';  
if (isset($_GET['intervenants']))
  $intervenants=$_GET['intervenants'];

if (isset($_GET['critere_recherche']))
  $critere_recherche=$_GET['critere_recherche'];
else
  $critere_recherche="Nom";

if (isset($_GET['tri']))
  $tri=$_GET['tri'];
else
  $tri="Date_Time";

if (isset($_GET['id_rdv']))
  $id_rdv=$_GET['id_rdv'];
else 
  $id_rdv='';

if (isset($_GET['GUID']))
	$GUID="%".$_GET['GUID']."%";
else
  $GUID='%';

if (isset($_GET['RDV_PrisPar']))
  $RDV_PrisPar=$_GET['RDV_PrisPar'];
if (isset($_GET['Duree']))
  $Duree=$_GET['Duree'];
else
  $Duree='';
if (isset($_GET['Nom']))
  $Nom=$_GET['Nom'];
else
  $Nom="";
if (isset($_GET['nom'])) //provient de la fiche patient
  $nom=$_GET['nom'];
else
  $nom="";
if (isset($_GET['Prenom']))
  $Prenom=$_GET['Prenom'];
else
  $Prenom='';
if (isset($_GET['Tel']))
  $Tel=$_GET['Tel'];
else
  $Tel='';
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

if (isset($_GET['Note']))
  $Note=$_GET['Note'];
else
  $Note='';

if (isset($_GET['modif_rdv']))
  $modif_rdv=$_GET['modif_rdv'];

$envoyer='';
$bouton_envoyer='';
$Date='';

$critere_tri['Date_Time']="Date";
$critere_tri['Nom']="Nom";

$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
if (isset($_GET['utilisateur_autorisant'])) //si on recupere un UA
{
//on le met en session
  if (!preg_match('`specialite`',$_GET['utilisateur_autorisant']))
    $_SESSION['login'] = $user."::".$_GET['utilisateur_autorisant'];
  $tab_login=explode("::",$_SESSION['login']);
}
$utilisateur_autorisant=$tab_login[1];

if (isset($_GET['bouton_envoyer']) OR isset($_GET['envoyer']))
{
  if (isset($_GET['bouton_envoyer']))
  {
    $bouton_envoyer=$_GET['bouton_envoyer'];
  //Initialisation des dates
    if ($bouton_envoyer=="Modifier") // si on est en mode de modification ou d'ajout de rdv
  //la cle Ajouter semble ne servir a rien ici
    {
      $debut=$fin=$_GET['debut']; // on recupere la date dans l'URL
    }
    elseif ($bouton_envoyer=="Rendez-vous") // Si on vient du fichier patient ou de l'agenda
    {
      if (isset($_GET['jour']))
	$debut=$fin=$Date=$_GET['jour'];
      else
	$debut=$fin=$Date=$_GET['debut'];
      $Nom=$_GET['Nom'];
    }
    elseif ($bouton_envoyer=="Annuler")
    {
      $debut=$_GET['debut']; //locale
      $fin=$_GET['fin'];

      $sql_chercher_caracteristiques="SELECT * FROM agenda WHERE PrimKey='$id_rdv'";
      $resultat_chercher_caracteristiques=mysqli_query($db,$sql_chercher_caracteristiques);
      $ligne_chercher_caracteristiques=mysqli_fetch_array($resultat_chercher_caracteristiques);
      $horodatage=$ligne_chercher_caracteristiques['Date_Time'];
      $Duree=$ligne_chercher_caracteristiques['Duree'];
      $Type=$ligne_chercher_caracteristiques['Type'];
      $status="%";
      $Nom='';
      $Prenom='';
      $GUID='';
      $Tel='';
      $Note='';
      $sql_inserer_vierge="INSERT INTO agenda (Date_Time,Duree,RDV_PrisPar,RDV_PrisAvec,Type,status) VALUES ('$horodatage','$Duree','$user','$utilisateur_autorisant','$Type','Non attribué')";
      $resultat_inserer_vierge=mysqli_query($db,$sql_inserer_vierge);

      $sql_annuler="UPDATE agenda SET status='Annulé', RDV_PrisPar='$user' WHERE  PrimKey='$id_rdv'";
      $resultat_annuler=mysqli_query($db,$sql_annuler);
    }
  }
  if   (isset($_GET['envoyer']))
  {
    if ($envoyer=="Chercher") //Si on a appuye sur le bouton de recherche ou si on vient d'une suppression
    {
      $debut=$_GET['debut']; //locale
      $fin=$_GET['fin'];
    }
    elseif ($envoyer=="Rendez-vous") 
    {
      $debut=$_GET['debut'];
      $fin=$_GET['debut'];
      $Date=$_GET['debut'];
      $Nom=$_GET['Nom'];
    }
    else //boutons fleches
    {
      $debut=$_GET['debut'];
      $fin=$_GET['fin'];
    }
  }
}
else //lancement sans argument
{
  $envoyer="";
  $debut=$fin=date('d-m-Y', date('U')); // date du jour
}

$debut_sql=local_to_iso($debut,$date_format);
$fin_sql=local_to_iso($fin,$date_format);

if ($fin_sql<$debut_sql)
{
  $fin_sql=$debut_sql; 
}
$debut_sql=$debut_sql." 00:00:00";
$fin_sql=$fin_sql." 23:59:59";

if ($intervenants=='specialite')
{
  $sql_specialite_intervenant="SELECT Qualite FROM Personnes WHERE Login ='$user'"; //on cherche la specialite de l'utilisateur
  $resultat_specialite_intervenant=mysqli_query($db,$sql_specialite_intervenant);
  $ligne_specialite_intervenant=mysqli_fetch_array($resultat_specialite_intervenant);
  $specialite=$ligne_specialite_intervenant['Qualite'];
  $sql_chercher_specialiste="SELECT * FROM Personnes WHERE Qualite LIKE '$specialite' AND Login !='' ";
  $resultat_chercher_specialiste=mysqli_query($db,$sql_chercher_specialiste);
  $liste_intervenants_pour_sql='';
  $compteur=0;
  while ($ligne_chercher_specialiste=mysqli_fetch_array($resultat_chercher_specialiste))
  {
    $user_spe=sprintf("%s",mysqli_real_escape_string($db,$ligne_chercher_specialiste['Login']));
    $compteur++;
    if ($compteur==1)
      $liste_intervenants_pour_sql='AND ( RDV_PrisAvec =\''.$user_spe.'\' ';
    else //autres tours de la boucle
      $liste_intervenants_pour_sql=$liste_intervenants_pour_sql.' OR RDV_PrisAvec=\''.$user_spe.'\'';
  }
  $liste_intervenants_pour_sql=$liste_intervenants_pour_sql.')';
}
elseif ($intervenants=='tous')
{
  $liste_intervenants_pour_sql='';
}
else // intervenant = seul
{
  $user=sprintf("%s",mysqli_real_escape_string($db,$user));
  $liste_intervenants_pour_sql="AND RDV_PrisAvec='".$utilisateur_autorisant."'" ;
}
if (isset($_REQUEST['from_page'])) //si on vient du fichier patient ou de la liste - ne tient pas compte de l'identite
{
  $sql_liste_rdv=sprintf("SELECT * FROM agenda WHERE Date_Time BETWEEN '$debut_sql' AND '$fin_sql' $liste_intervenants_pour_sql ORDER BY Date_Time;",mysqli_real_escape_string($db,$user));
}
else //si on vient de la page agenda
{
  //Si l'on est en mode de recherche par le bouton Chercher ou les boutons Aujourd'hui et fleches
  if (isset($_GET['envoyer']))
  {
    $envoyer=$_GET['envoyer'];
//les fleches
    if ($envoyer=='Précédent') //»
    {
      $debut=$fin=iso_to_local($_GET['precedent'],$date_format);
      $debut_sql=$_GET['precedent'].' 00:00:00';
      $fin_sql=$_GET['precedent'].' 23:59:59';
    }
    elseif ($envoyer=='Suivant') //«
    {
      $debut=$fin=iso_to_local($_GET['suivant'],$date_format);
      $debut_sql=$_GET['suivant'].' 00:00:00';
      $fin_sql=$_GET['suivant'].' 23:59:59';
    }
    elseif (preg_match('`Aujourd`',$envoyer,$tab))
    {

      $debut=iso_to_local(date('Y-m-d', date('U')),$date_format); // date du jour
      $debut_sql=date('Y-m-d', date('U')).' 00:00:00';
      $fin_sql=date('Y-m-d', date('U')).' 23:59:59';
    }

    $Nom="%$Nom%";
    $Prenom="%$Prenom%";
    if (isset ($_REQUEST['check_name'])) //on tient compte du nom dans la recherche de rendez-vous seulement si specifie
{
      $sql_liste_rdv=sprintf("SELECT * FROM agenda WHERE Date_Time BETWEEN '$debut_sql' AND '$fin_sql' AND Nom LIKE '%s' AND Prenom LIKE '%s' AND GUID LIKE '%s' $liste_intervenants_pour_sql AND Type LIKE '$Type' AND status LIKE '%s' ORDER BY $tri;",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom),mysqli_real_escape_string($db,$GUID),mysqli_real_escape_string($db,$status));
}
    else
{
      $sql_liste_rdv=sprintf("SELECT * FROM agenda WHERE Date_Time BETWEEN '$debut_sql' AND '$fin_sql' $liste_intervenants_pour_sql AND Type LIKE '$Type' AND status LIKE '%s' ORDER BY $tri;",mysqli_real_escape_string($db,$status));
}
  }
  else //au lancement sans argument
  {
    $sql_liste_rdv=sprintf("SELECT * FROM agenda WHERE Date_Time BETWEEN '$debut_sql' AND '$fin_sql' $liste_intervenants_pour_sql ORDER BY Date_Time;",mysqli_real_escape_string($db,$user));
  }
}
$resultat_liste_rdv=mysqli_query($db,$sql_liste_rdv);
$count_liste_rdv=mysqli_num_rows($resultat_liste_rdv);

//Recherche des couleurs de RdV
$sqlcouleur="SELECT * FROM color_profils GROUP BY Name";
$resultatcouleur=mysqli_query($db,$sqlcouleur);

	include("inc/header.php");
?>
  <meta name="keywords" lang="fr" content="Agenda pour MedinTux" />
  <title>Agenda de <?php echo $utilisateur_autorisant ?></title>
        
  <script type="text/javascript">
//<![CDATA[
function Invers()//pour inverser les coches 
{
  temp = document.forms['form_coches'].elements.length -2 ;
  for (i=0; i < temp; i++)
  {
    if(document.forms['form_coches'].elements[i].checked == 1)
    {
      document.forms['form_coches'].elements[i].checked = 0;
    }
    else 
    {
      document.forms['form_coches'].elements[i].checked = 1
    }
  }
}
//]]>
  </script>

  <script type="text/javascript">
//<![CDATA[
function change_status_this_appointment(string,number)
{
//fonction  pour changer en tâche de fond les statuts des rdv de la liste
  f1.location.href="modif_status.php?id="+number+"&status="+string;
  return false;
}
//]]>
  </script>
  <script type="text/javascript">
//<![CDATA[
function vide_form()//pour effacer le patient
{
  document.forms['form_jour'].elements['GUID'].value= '';
  document.forms['form_jour'].elements['Prenom'].value= '';
  document.forms['form_jour'].elements['Nom'].value = '';
  document.forms['form_jour'].elements['Tel'].value = '';
}
//]]>
  </script>

  <script type="text/javascript">
//<![CDATA[
function verif_tous()//pour ne pas enregistrer une fiche avec le type Tous, on substitue a la volee les valeurs par defaut
{
  // si la valeur du champ nom est vide
  if (document.forms['form_jour'].elements['Nom'].value == "") 
  {
    // sinon on affiche un message
    alert("Saisissez au moins le nom");
    // et on indique de ne pas envoyer le formulaire
    return false;
  }
  else 
  {
//OK alert ('testvrai');

    // les données sont ok, on continue    
    if (document.forms['form_jour'].elements['Type'].value== 'Tous')
    {
      document.forms['form_jour'].elements['Type'].value= '<?php echo $rdv_default[$utilisateur_autorisant] ?>';
    }
    if (document.forms['form_jour'].elements['status_saisie'].value== 'Tous')
    {
      document.forms['form_jour'].elements['status_saisie'].value= 'Statut non défini';
    }
    return true;
  }
}
//]]>
  </script>

<?php
include 'calendar_javascript.php';
?>
  <script type="text/javascript">
//<![CDATA[
function mettre_valeur(value)
{
//duree des rdv automatique selon le type de rdv
  var type=new Array;
<?php
//creation de variables javascript pour les durees des rdv

  $resultatcouleur=mysqli_query($db,$sqlcouleur);
  while ($lignecouleur=mysqli_fetch_array($resultatcouleur))
  {
    $name=$lignecouleur['Name'];
    echo "type[\"".$name."\"]=\"".$lignecouleur['Duree']."\";"; 
  }
?>
  document.forms['form_jour'].Duree.value = type[value]; // on ecrit le resultat dans Duree
}
//]]>
  </script>
  <script type="text/javascript">
//<![CDATA[
function change_status(value)
{
//passer automatiquement le statut du rdv en statut non defini si remplissage du nom et statut non attribue initialement
  if (document.getElementById('status_saisie').value=='Non attribué')
  {
    if (value.length>0)
    {
    //suppose que le statut Non defini est le 4e dans la liste
    //plus elegant avec une moulinette qui cherche tous les statuts connus et trouve le numero de Non defini
    document.getElementById('status_saisie').selectedIndex=4;
    }
  }
}
//]]>
  </script>

  <script type="text/javascript">
//<![CDATA[
function chercher_patient()
// on ouvre dans une fenêtre le fichier passé en paramètre.
{ 
  nom=document.getElementById('Nom').value;
  prenom=document.getElementById('Prenom').value;
  window.open('recherche_patient_agenda.php?Nom='+nom+'&Prenom='+prenom,'Choisir','width=800,height=550,top=50,left=50,toolbar=no,scrollbars=yes,resizable=yes,location=no'); 
  if (document.getElementById('status_saisie').value=='Non attribué')
  {
    document.getElementById('status_saisie').selectedIndex=4; //remettre le statut en "non défini"
  }
}
//]]>
  </script>

<!-- css special pour le tableau des rdv en mode impression -->
  <style type="text/css"  media="print">
	  td:nth-child(12) { display: none; }  
	  td:nth-child(13) { display: none; }  
	  th:nth-child(12) { display: none; }  
	  th:nth-child(13) { display: none; }  
  </style>


</head>

<body style="font-size:<?php echo $fontsize; ?>pt" >
<!-- Zone virtuelle invisible pour permettre l'execution d'une page PHP de mise a jour dynamique des statuts de rendez-vous -->
<div style="display:none">
<iframe name="f1" id="f1"> </iframe>
</div>
  <div class="conteneur">
	
<!-- // insertion du menu d'en-tete	 -->
<?php
$anchor="Agenda";
include("inc/menu-horiz.php");
//recherche des autres agendas lisibles
$sql_utilisateur_delegue="SELECT * FROM Personnes INNER JOIN user_perms ON Personnes.GUID=user_perms.FriendUserGUID WHERE Login='$user' ORDER BY Qualite";

$resultat_utilisateur_delegue=mysqli_query($db,$sql_utilisateur_delegue);
$nombre_utilisateurs="";
$Qualite='';

?>
    <div class="groupe noPrint">
      <form action="validation_rdv.php" method="post" id="form_jour">
	<h1>
	  Agenda de 
	  <select name="utilisateur_autorisant" id="utilisateur_autorisant" onchange="select_user=form.submit()">
<?php
$sql_select_droits="SELECT * FROM Personnes WHERE Login='$user'";
$resultat_select_droits=mysqli_query($db,$sql_select_droits);
$ligne_select_droits=mysqli_fetch_array($resultat_select_droits);
if (preg_match('/sgn/',$ligne_select_droits['Droits'])) //ne pas proposer un utilisateur qui ne serait pas signataire.
{
?>
	    <option value="<?php echo $user ?>" >
	      <?php echo $user ?>
	    </option>
<?php
}
while ($ligne_utilisateur_delegue=mysqli_fetch_array($resultat_utilisateur_delegue))
{
  $utilisateur_autorisant_GUID=$ligne_utilisateur_delegue['SignataireGUID'];
  $sql_utilisateur_autorisant="SELECT Login,Qualite,Nom,Prenom FROM Personnes WHERE GUID='$utilisateur_autorisant_GUID'";
  $resultat_utilisateur_autorisant=mysqli_query($db,$sql_utilisateur_autorisant);
  $ligne_utilisateur_autorisant=mysqli_fetch_array($resultat_utilisateur_autorisant);

  $sql_select_droits="SELECT * FROM Personnes WHERE GUID='$utilisateur_autorisant_GUID'";
  $resultat_select_droits=mysqli_query($db,$sql_select_droits);
  $ligne_select_droits=mysqli_fetch_array($resultat_select_droits);

  if (preg_match('/agc/',$ligne_utilisateur_delegue['FriendUserDroits']) AND preg_match('/sgn/',$ligne_select_droits['Droits']))//peut prendre les rdv et est signataire
  {
    $nombre_utilisateurs++;
    if ($Qualite!=$ligne_utilisateur_delegue['Qualite'])
    {
//les intertitres de specialite
      echo "
	    <option value=\"specialite-".$ligne_utilisateur_autorisant['Login']."\"";
      if ($intervenants=='specialite')
      {
	if ($specialite==$ligne_utilisateur_autorisant['Qualite'])
	  echo " selected='selected' "; //on positionne le deroulant par defaut pour la specialite en cours
      }
      echo ">
			  --".$ligne_utilisateur_autorisant['Qualite']."--
	    </option>";
      $Qualite=$ligne_utilisateur_autorisant['Qualite']; //on change la valeur de la variable pour que le sous-titre de specialite n'apparaisse plus pour les intervenants suivants de la specialite
    }
?>
	    <option value="<?php echo $ligne_utilisateur_autorisant['Login'];?>" <?php if ($ligne_utilisateur_autorisant['Login']==$utilisateur_autorisant) echo 'selected="selected"' ?> >
	      <?php echo $ligne_utilisateur_autorisant['Login'].' ('.$ligne_utilisateur_autorisant['Nom'].' '.$ligne_utilisateur_autorisant['Prenom'];?>)
	    </option>
<?php
  }
}
?>
	  </select>
<?php
  if($intervenants=='specialite') echo ' et toute sa spécialité'; elseif ($intervenants=='tous') echo ' - Tous agendas' 
?>
	</h1>
<!--
<fieldset>
<legend>Agenda</legend>
-->
<!-- 	<div class="login"> -->
<!-- 	  <table> -->
<!--Table principale-->
<!--	  <tr>

	</tr>
	<tr>

	  <td>
	    <fieldset class="fieldset_agenda">
	      <legend>
		Rendez-vous en rafale
	      </legend>
	      <table>
-->
<!--Table du bouton repeter-->
<!--		<tr>
		  <td>
		    <input type="submit" name="button_repeat" value="Répéter" />
		  </td>
		</tr>
	      </table>
-->
<!--Table du bouton repeter-->
<!--	    </fieldset>
	  </td>
	</tr>
 -->
   <!-- Table des caracteristiques du rendez-vous -->
<!-- 	    <tr style="vertical-align:top;"> -->
<!-- 	      <td> -->
<!-- </div> -->
<div class="login">
		<fieldset class="fieldset_login">
		  <legend>
<?php
if ($bouton_envoyer=="Modifier") // si on a appelé le formulaire par le bouton modifier
{
?>
		Modification d'un rendez-vous
<?php
}
else
{
?>
		Saisie d'un nouveau rendez-vous
<?php
}
?>
		  </legend>
<!--Formulaire d'ajout-modification de RDV -->
			<label for="Date">
			  Jour&nbsp;: 
			</label>
			<input name="RDV_PrisAvec" type="hidden" value="<?php echo $user ?>" />
			<input name="intervenants" type="hidden" value="<?php echo $intervenants ?>" />
			<input name="Date" id="datepicker" type="text" value="<?php echo substr($debut,0,10) ?>" size="10" maxlength="10" />
			
<!--
			<input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_jour'].Date);" />
-->
			<br />
			<b>
			  <label for="Heure">
			    Heure&nbsp;: 
			  </label>
			</b>
			<input name="Heure" id="Heure" type="text" value="<?php echo $Heure ?>" size="2" maxlength="2" />
			<input name="Minutes" type="text" value="<?php echo $Minutes ?>" size="2" maxlength="2" /><br />
			<label for="Type">
			  Type&nbsp;: 
			</label>
			<select name="Type" id="Type" onchange="mettre_valeur(this.value)" title="Valeur par défaut <?php echo $rdv_default[$utilisateur_autorisant];  ?> pour l'enregistrement des rendez-vous">
			  <option value='Tous'>
			    Tous
			  </option>
<?php
//recuperation des couleurs pour les differents types de RDV
$resultatcouleur=mysqli_query($db,$sqlcouleur);

while ($lignecouleur=mysqli_fetch_array($resultatcouleur))
{
  $couleur[$lignecouleur["Name"]]=$lignecouleur["Color"];
?>
			  <option <?php 
  if (isset($_REQUEST['Type'])) //modifier
  {
    if ($Type==$lignecouleur["Name"]) //si mode modifier
      echo " selected='selected'" ;
  }
/*
  else //mode normal
  {
    if ($rdv_default==$lignecouleur["Name"]) 
    {
      echo " selected='selected'";
      $Duree=$lignecouleur["Duree"]; //pour le champ duree
      $Type=$lignecouleur["Name"];
    }
  }
*/
?> value="<?php echo $lignecouleur["Name"]; ?>">
		  <?php echo $lignecouleur["Name"]?>
			  </option>
<?php
}
?>
			</select><br />
			<label for="Duree">
			  Dur&eacute;e : 
			</label>
			<input name="Duree" id="Duree" type="text" value="<?php echo $Duree; ?>" size="5" maxlength="11" /><br />
			<label for="status_saisie">
			  Statut : 
			</label>
			<select name="status" id="status_saisie"  title="Valeur par défaut Statut non défini pour l'enregistrement des rendez-vous">
			  <option value='Tous' <?php if ($status=='Tous') echo "selected='selected'" ?> >
			    Tous
			  </option>
<?php
foreach ($status_rdv AS $ce_rdv)
{
?>
			  <option value="<?php echo mysqli_real_escape_string($db,$ce_rdv) ?>"<?php 
  if (isset($_REQUEST['status'])) // si on a appelé le formulaire par le bouton modifier
  {
    if ($ce_rdv==$status) 
      echo " selected='selected'"; 
  }
/*  else
  {
    if ($ce_rdv=="Statut non défini")
      echo " selected='selected'";
  }
*/
?> >
<?php 
  echo $ce_rdv 
?>
			  </option>
<?php
}
?>
			</select><br />
			<b><label for="Note">Notes : </label></b>
			<input name="RDV_PrisPar" type="hidden" value="<?php echo $user ?>" size="20" maxlength="20" />
			<input name="Note" id="Note" type="text" value="<?php echo $Note ?>" size="30" maxlength="100" /><br />
<?php
if ($bouton_envoyer=="Modifier") // si on a appelé le formulaire par le bouton modifier
{
  //On verifie si verrou existe
  $sql_chercher_verrou="SELECT `lock` FROM agenda WHERE `PrimKey`='$id_rdv'";
  $resultat_chercher_verrou=mysqli_query($db,$sql_chercher_verrou);
  $ligne_chercher_verrou=mysqli_fetch_array($resultat_chercher_verrou);
  if ($ligne_chercher_verrou['lock']==1)
    echo "
		  <div class=\"notice\" style=\"width:300px;\">Fiche en utilisation. Ne pas modifier <br />sauf si vous êtes certain de ne pas être en conflit <br />avec un autre utilisateur</div>";
  else
  {
  //on pose un verrou
    $sql_poser_verrou="UPDATE agenda SET `lock`='1' WHERE `PrimKey`='$id_rdv'";
    $resultat_poser_verrou=mysqli_query($db,$sql_poser_verrou);
  }
?>
			<input name="bouton_envoyer" type="submit" value="Modifier" onclick="return verif_tous()" />
			<input name="id_rdv" type="hidden" value="<?php echo $id_rdv ?>" />
			<input name="Reset" type="reset" value="R&eacute;initialiser" />
<?php
}
else
{
?>
			<input name="bouton_envoyer" type="submit" value="Ajouter" title="Créer un rendez-vous avec toutes les données ci-dessus" onclick="return verif_tous()"/>
<?php
}
?>
	  <!-- Fin zone saisie du rdv -->
		</fieldset>
	      </div>
	      <div class="login">
		<fieldset class="fieldset_login">
		  <legend>
		    Patient
		  </legend>
		    <label for="Nom">
		      Nom&nbsp;: 
		    </label>
		    <input name="GUID" id="GUID" type="hidden" value="<?php echo str_replace('%','',$GUID) ?>" readonly="readonly" />
		    <input name="Nom" id="Nom" type="text" value="<?php echo str_replace('%','',$Nom) ?>" size="20" maxlength="40" onchange="change_status(this.value)"/><br />
		    <label for="Prenom">
		      Pr&eacute;nom&nbsp;: 
		    </label>
		    <input name="Prenom" id="Prenom" type="text" value="<?php echo str_replace('%','',$Prenom) ?>" size="20" maxlength="40" /><br />
		    <b><label for="Tel">T&eacute;l&eacute;phone&nbsp;: </label>
		    </b>
		    <input name="Tel" id="Tel" type="text" value="<?php echo $Tel ?>" size="10" maxlength="10" /><br />
		    <input type="checkbox" name="check_name" id="check_name" value="check" <?php if (isset($_REQUEST['check_name'])) echo 'checked="checked"' ?>/>

		    <label for="check_name">
		      Utiliser l'identité pour la recherche de rendez-vous
		    </label><br />
		    <input type="button" value="Choisir un patient" onclick="chercher_patient()" />
		    <input type="button" value="Vider" onclick="vide_form()" />
		</fieldset>
	      </div>
	      <div class="login">
		<fieldset class="fieldset_login">
		  <legend>
		    Rechercher
		  </legend>
	  <!-- table des recherches -->
			<label for="fin">
			  Fin&nbsp;: 
			</label>
			<input name="fin" id="datepickeur" type="text" value="<?php echo $fin; ?>" size="10" maxlength="10" />
<!--
<input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_jour'].fin);" />
-->
			<br />
			<label for="praticien">
			  Praticien 
			</label>
			<input type="radio" name="intervenants" id="praticien" value="seul" <?php if ($intervenants=='seul') echo 'checked="checked"' ?>  onchange="select_user=form.submit()"/>
			<label for="specialite">
			  Spécialité 
			</label>
			<input type="radio" name="intervenants"  id="specialite" value="specialite" <?php if ($intervenants=='specialite') echo 'checked="checked"' ?>  onchange="select_user=form.submit()"/>
			<label for="tous">Tous </label>
			<input type="radio" name="intervenants" id="tous"  value="tous" <?php if ($intervenants=='tous') echo 'checked="checked"' ?> onchange="select_user=form.submit()" /><br />
			<input name="envoyer" id="button_chercher" type="submit" value="Chercher" />
			<input name="envoyer" type="submit" value="Cocher" title="Permet de sélectionner des fiches qui seront définitivement effacées"/>
<?php
//On passe la date en composantes pour le calcul

$debut_iso=local_to_iso($debut,$date_format);

$list_date=explode ("-",$debut_iso);
?>
			<div class="precsuiv">
			  <input name="precedent" type="hidden" value="<?php echo date("Y-m-d", mktime(0, 0, 0,$list_date[1],$list_date[2]-1,$list_date[0])); ?>" />
			  <input name="envoyer" type="submit" value="Précédent" title="Jour pr&eacute;c&eacute;dent le premier jour de la période" />
			  <input name="today_debut" type="hidden" value="<?php echo date('Y-m-d', date('U')) ?>" />
			  <input name="envoyer" type="submit" value="Aujourd'hui" />
			  <input name="suivant" type="hidden" value="<?php echo date("Y-m-d", mktime(0, 0, 0,$list_date[1],$list_date[2]+1,$list_date[0])); ?>" />
			  <input name="envoyer" type="submit" value="Suivant" title="Jour suivant le dernier jour de la période" />
			</div>
<!-- 		  zone des recherches -->
		</fieldset>
	</div><!-- Fin class=tableau -->
      </form>
    </div><!-- Fin class=groupe noprint titre et tableau principal -->

    <div class="groupe">
      <h1>

<?php

//Si l'on est en mode de recherche par date
if (isset($_GET['envoyer']))
{
  $envoyer=$_GET['envoyer'];
//les fleches
  if ($envoyer=='&raquo;') //»
  {
    $debut=$_GET['raquo_debut'];
    $fin=$_GET['raquo_fin'];
  }
  elseif ($envoyer=='&laquo;') //«
  {
    $debut=$_GET['laquo_debut'];
    $fin=$_GET['laquo_fin'];
  }
  elseif ($envoyer=='Aujourd\'hui')
  {
    $debut=$fin=$_GET['today_debut'];
  }
  //Affichage du resume si les dates de debut et de fin sont les memes
  $status_propre=stripslashes($status);

  $string_request_key='';
  if (isset($_REQUEST['check_name']))
    $string_request_key= "pour $critere_recherche = $Nom ,";

  if (substr($debut_sql,0,10)==substr($fin_sql,0,10))
  {
    if ($count_liste_rdv==0)
    {
      echo "
	  Aucun rendez-vous trouv&eacute; le $debut $string_request_key de type $Type, de statut $status_propre.";
    }
    elseif ($count_liste_rdv==1)
      echo "
	  1 rendez-vous trouv&eacute; le $debut $string_request_key de type $Type, de statut $status_propre.";
    else
      echo "
	  ".$count_liste_rdv." rendez-vous trouv&eacute;s le $debut $string_request_key de type $Type, de statut $status_propre, tri&eacute;s par $critere_tri[$tri].";
  }
//Si deux dates differentes
  else
  {
    if ($count_liste_rdv==0)
      echo "
	  Aucun rendez-vous trouv&eacute; entre le $debut et le $fin $string_request_key de type $Type, de statut $status_propre.";
    elseif ($count_liste_rdv==1)
      echo "
	  1 rendez-vous trouv&eacute; entre le $debut et le $fin $string_request_key de type $Type, de statut $status_propre.";
    else
      echo "
	  $count_liste_rdv rendez-vous trouv&eacute;s entre le $debut et le $fin $string_request_key de type $Type, de statut $status_propre, tri&eacute;s par $critere_tri[$tri]" ;
  }
}
//si rien n'est renseigne
else
{
  if ($count_liste_rdv==0)
    echo "
	  Aucun rendez-vous trouv&eacute; le $debut.";
  elseif ($count_liste_rdv==1)
    echo "
	  1 rendez-vous trouv&eacute; le $debut.";
  else
    echo "
	  $count_liste_rdv rendez-vous trouv&eacute;s le $debut, tri&eacute;s par heure";
}

echo "
	</h1>"; //fin  nombre resultats

if ($count_liste_rdv) //afficher les en-tetes seulement si resultat
{
  if ($Nom=="")
  {
    $Nom="%";
  }
  if ($envoyer=="Cocher") //initialisation du formulaire des cases a cocher
{
	echo "
	<form id=\"form_coches\" action=\"suppr_rdv.php\" method=\"get\">
	  <div>
	    <input name=\"debut\" type=\"hidden\" value=\"". $debut ."\" />
	    <input name=\"fin\" type=\"hidden\" value=\"". $fin ."\" />
	  </div>";
}

?>
<br />
	  <div class="tableau">
	    <table title="Vous pouvez ordonner par nom ou date/heure en cliquant sur la t&ecirc;te de colonne">
	      <col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col>
		<tr><!--Les en-tetes du tableau d'affichage des RDV -->
<?php if ($envoyer=="Cocher")
{
?>
		  <th class="fond_th">
		    <input name="button_invert_selection" onclick="Invers()" type="button" value="Inverser la sélection" />
		  </th>
<?php
}
?>
		  <th class="fond_th">
		    <a href="agenda.php?debut=<?php echo $debut ?>&amp;fin=<?php echo $fin ?>&amp;critere_recherche=<?php echo $critere_recherche?>&amp;nom=<?php echo $Nom ?>&amp;Type=<?php echo $Type ?>&amp;envoyer=Chercher&amp;tri=Date_Time">
		      Date
		    </a>
		  </th>
		  <th class="fond_th">
		    Heure
		  </th>
		  <th class="fond_th">
		    Dur&eacute;e
		  </th>
		  <th class="fond_th">
		    <a href="agenda.php?debut=<?php echo $debut ?>&amp;fin=<?php echo $fin ?>&amp;critere_recherche=<?php echo $critere_recherche?>&amp;nom=<?php echo $nom ?>&amp;Type=<?php echo $Type ?>&amp;envoyer=Chercher&amp;tri=Nom">
		      Nom
		    </a>
		  </th>
		  <th class="fond_th">
		    Pr&eacute;nom
		  </th>
		  <th class="fond_th">
		    T&eacute;l
		  </th>
		  <th class="fond_th">
		    Type
		  </th>
		  <th class="fond_th">
		    Statut
		  </th>
		  <th class="fond_th">
		    Notes
		  </th>
		  <th class="fond_th">
		    Pris avec
		  </th>
<?php if ($envoyer!="Cocher") echo "
		  <th colspan=\"2\" class=\"fond_th\">
		    Rendez-vous
		  </th>";
?>
		</tr>
<?php
}

while ($ligne_liste_rdv=mysqli_fetch_array($resultat_liste_rdv)) 
{
  $tableau_date_time=explode(" ",$ligne_liste_rdv["Date_Time"]); //on separe la date de l'heure - format iso
  $date=iso_to_local($tableau_date_time[0],$date_format);
  $time=substr($ligne_liste_rdv["Date_Time"],11,5);
  $tableau_components=explode("-",$tableau_date_time[0]);

//Recuperation du type de rdv pour y affecter une couleur
  $type_RV=$ligne_liste_rdv["Type"];
  $couleur_ligne=$couleur["$type_RV"];
  $jd = date("w",mktime(0,0,0,$tableau_components[1],$tableau_components[2],$tableau_components[0]));//numero du jour de la semaine

//Affichage si le rendez-vous n'est pas dans le fichier
  if (!$ligne_liste_rdv["GUID"])
  {
    if ($ligne_liste_rdv["status"])
      $color=stripslashes($ligne_liste_rdv["status"]);
    else
      $color="Statut non défini";
    echo "
		<tr>";
    if ($envoyer=="Cocher") 
    {
      echo "
		  <td class=\"fond_td\">
		    <input type=\"checkbox\" name=\"coche[]\" value=\"".$ligne_liste_rdv["PrimKey"]."\" />
		  </td>";
    }
    echo "
		  <td class=\"fond_td\">
		    ".$text_jsemaine[$jd]." ".$date." 	  
		  </td>	  
		  <td class=\"fond_td\">
		    ".$time."
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Duree"].
		  "'</td>
		  <td class=\"fond_td\">
		    <a href=\"liste.php?critere_recherche=FchGnrl_NomDos&amp;envoyer=Chercher&amp;cle=".$ligne_liste_rdv["Nom"]."%&amp;Prenom=".$ligne_liste_rdv["Prenom"]."&amp;Tel=".$ligne_liste_rdv["Tel"]."&amp;Nom=".$ligne_liste_rdv["Nom"]."\">
		      ".$ligne_liste_rdv["Nom"]."
		    </a>
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Prenom"]."
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Tel"]."
		  </td>
		  <td style=\"background:$couleur_ligne;\" class=\"fond_td\">
		    ".$ligne_liste_rdv["Type"]."
		  </td>
		  <td style=\"background:$color_status[$color];\" class=\"fond_td\">
		    ";
?>
		    <select name="status" id="status_saisie<?php echo $ligne_liste_rdv["PrimKey"] ?>" onchange="change_status_this_appointment(this.value,<?php echo $ligne_liste_rdv["PrimKey"] ?>)" >
<?php
reset ($status_rdv);
foreach ($status_rdv AS $ce_rdv)
{
?>
			<option value="<?php echo mysqli_real_escape_string($db,$ce_rdv) ?>"<?php 
    if ($ce_rdv==stripslashes($ligne_liste_rdv["status"]))
      echo " selected='selected'"; 
?> >
<?php 
    echo $ce_rdv 
?>
			  </option>
<?php
}
?>
			</select><br />
<?php
echo "
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Note"]."
		  </td>
		  <td class=\"fond_td\" title=\"Pris par ".$ligne_liste_rdv["RDV_PrisPar"]." \">
		    ".$ligne_liste_rdv["RDV_PrisAvec"]."
		  </td>";
    if ($envoyer!="Cocher") 
    {
?>
		  <td class="fond_td">
		    <form action="agenda.php"  method="get"> <!--Bouton de modification de rendez-vous--> 
		      <div>
			<input name="nom" type="hidden" value="%" />
			<input name="debut" type="hidden" value="<?php echo $date ?>" />
			<input name="fin" type="hidden" value="<?php echo $date ?>" />
			<input name="id_rdv" type="hidden" value="<?php echo $ligne_liste_rdv['PrimKey']?>" />
			<input name="GUID" type="hidden" value="" />
			<input name="RDV_PrisPar" type="hidden" value="<?php echo $ligne_liste_rdv['RDV_PrisPar']?>" />
			<input name="Date" type="hidden" value="<?php echo $date ?>" />
			<input name="Time" type="hidden" value="<?php echo $time?>" />
			<input name="RDV_PrisAvec" type="hidden" value="<?php echo $ligne_liste_rdv['RDV_PrisAvec']?>" />
			<input name="Duree" type="hidden" value="<?php echo $ligne_liste_rdv['Duree']?>" />
			<input name="Nom" type="hidden" value="<?php echo $ligne_liste_rdv['Nom']?>" />
			<input name="Prenom" type="hidden" value="<?php echo $ligne_liste_rdv['Prenom']?>" />
			<input name="Tel" type="hidden" value="<?php echo $ligne_liste_rdv['Tel']?>" />
			<input name="Note" type="hidden" value="<?php echo $ligne_liste_rdv['Note']?>" />
			<input name="Type" type="hidden" value="<?php echo $ligne_liste_rdv['Type']?>" />
			<input name="status" type="hidden" value="<?php echo $ligne_liste_rdv['status']?>" />
			<input name="intervenants" type="hidden" value="<?php echo $intervenants ?>" />
			<input name="bouton_envoyer" type="submit" value="Modifier" />
<?php
//echo $ligne_liste_rdv["status"];
      if ($ligne_liste_rdv["status"]!='Annulé') //pas de bouton Annuler si déjà annulé
      {
?>
			<input name="bouton_envoyer" type="submit" value="Annuler" />
<?php
      }
?>
		      </div>
		    </form>
		  </td>
<?php
}
?>
		</tr>
<?php
	}
//Affichage si le rendez-vous est dans le fichier
	else
	{
	  $color="Statut non défini";
	  if ($ligne_liste_rdv["status"])
	    $color=stripslashes($ligne_liste_rdv["status"]);
	  echo "
		<tr>";
	  if ($envoyer=="Cocher") 
	  {
	    echo "
		  <td class=\"fond_td\">
		    <input type=\"checkbox\" name=\"coche[]\" value=\"".$ligne_liste_rdv["PrimKey"]."\" />
		  </td>";
	  }

	  echo "
		  <td class=\"fond_td\">
		    ".$text_jsemaine[$jd]." ".$date."
		  </td>
		  <td class=\"fond_td\">
		    ".$time."
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Duree"]."'
		  </td>
		  <td class=\"fond_td\">
		    <a href=\"frame_patient.php?GUID=".$ligne_liste_rdv["GUID"]."\">
		      ".$ligne_liste_rdv["Nom"]."
		    </a> 
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Prenom"]."
		  </td>
		  <td class=\"fond_td\">
		    ".$ligne_liste_rdv["Tel"]."
		  </td>
		  <td style=\"background:".$couleur_ligne.";\" class=\"fond_td\">
		    ".$ligne_liste_rdv["Type"]."
		  </td>
		  <td style=\"background:$color_status[$color];\" class=\"fond_td\">
		    ";
?>
		    <select name="status" id="status_saisie<?php echo $ligne_liste_rdv['PrimKey'] ?>" onchange="change_status_this_appointment(this.value,<?php echo $ligne_liste_rdv["PrimKey"] ?>)">
<?php
	  reset ($status_rdv);
	  foreach ($status_rdv AS $ce_rdv)
	  {
?>
			<option value="<?php echo mysqli_real_escape_string($db,$ce_rdv) ?>"
<?php 
	    if ($ce_rdv==stripslashes($ligne_liste_rdv["status"])) 
	      echo " selected='selected'"; 
?>
  >
<?php 
	    echo $ce_rdv.'
			</option>
';
	  }
?>
			</select>
		  </td>
		  <td class="fond_td">
<?php
	  echo $ligne_liste_rdv["Note"]."
		  </td>
		  <td class=\"fond_td\" title=\"Pris par ".$ligne_liste_rdv["RDV_PrisPar"]."\">
		    ".$ligne_liste_rdv["RDV_PrisAvec"]."
		  </td>";
	  if ($envoyer!="Cocher") 
	  {
?>
		  <td class="fond_td">
		    <form action="agenda.php"  method="get"> <!--Bouton de modification de rendez-vous--> 
		      <div>
			<input name="nom" type="hidden" value="%" />
			<input name="debut" type="hidden" value="<?php echo $date ?>" />
			<input name="fin" type="hidden" value="<?php echo $date ?>" />
			<input name="id_rdv" type="hidden" value="<?php echo $ligne_liste_rdv["PrimKey"]?>" />
			<input name="GUID" type="hidden" value="<?php echo $ligne_liste_rdv["GUID"]?>" />
			<input name="RDV_PrisPar" type="hidden" value="<?php echo $ligne_liste_rdv["RDV_PrisPar"]?>" />
			<input name="Date" type="hidden" value="<?php echo $date ?>" />
			<input name="Time" type="hidden" value="<?php echo $time?>" />
			<input name="RDV_PrisAvec" type="hidden" value="<?php echo $ligne_liste_rdv['RDV_PrisAvec']?>" />
			<input name="Duree" type="hidden" value="<?php echo $ligne_liste_rdv['Duree']?>" />
			<input name="Nom" type="hidden" value="<?php echo $ligne_liste_rdv['Nom']?>" />
			<input name="Prenom" type="hidden" value="<?php echo $ligne_liste_rdv['Prenom']?>" />
			<input name="Tel" type="hidden" value="<?php echo $ligne_liste_rdv['Tel']?>" />
			<input name="Note" type="hidden" value="<?php echo $ligne_liste_rdv['Note']?>" />
			<input name="Type" type="hidden" value="<?php echo $ligne_liste_rdv['Type']?>" />
			<input name="status" type="hidden" value="<?php echo $ligne_liste_rdv['status']?>" />
			<input name="intervenants" type="hidden" value="<?php echo $intervenants ?>" />
			<input name="bouton_envoyer" type="submit" value="Modifier" />
<?php
	    if ($ligne_liste_rdv["status"]!='Annulé') //pas de bouton Annuler si déjà annulé
	    {
?>
			<input name="bouton_envoyer" type="submit" value="Annuler" />
<?php
	    }
?>
		      </div>
		    </form>
		  </td>
<?php
	  }
?>
		</tr>
<?php
	}
      }
      if ($envoyer=="Cocher")
      {
?>
<!-- Derniere ligne coches -->
		<tr>
		  <td title="Ce bouton permet de supprimer définitivement tous les rendez-vous cochés ci-dessus en combinaison avec la coche.">
		    <input name="confirmer" type="checkbox" value="Supprimer" />
		    <input name="button_supprimer" type="submit" value="Supprimer" />
		  </td>
		</tr>
<?php
      }

// on ferme la base
      mysqli_close($db);
      if ($count_liste_rdv)
      {
?>
	      </table>
	    </div>
<?php
	if ($envoyer=="Cocher")
	{
?>
<!-- fin formulaire form_coches -->
	  </form>
<?php
	}
      }
?>
	</div>
<?php
include("inc/footer.php");
?>
