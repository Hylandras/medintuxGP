<?php
session_start() ;

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
$basemed="DatasempTest";

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

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header ('location: index.php?page=liste' );
  exit;
}
//chercher les droits d'utilisateur
$tab_login=explode("::",$_SESSION['login']); //attention chrome pour anroid ajoute des espaces a la fin

$user=$tab_login[0];
$signuser=str_replace(" ","",$tab_login[1]); //pour chrome

$sql_chercher_droits="SELECT GUID,Droits,Qualite FROM Personnes WHERE Login='$user'";
$resultat_chercher_droits=mysqli_query($db,$sql_chercher_droits);
$ligne_chercher_droits=mysqli_fetch_array($resultat_chercher_droits);
$GUID_user=$ligne_chercher_droits['GUID'];
$droits_user=$ligne_chercher_droits['Droits'];
$qualite_user=$ligne_chercher_droits['Qualite'];

//pour convertir les formats de dates
function local_to_iso($date,$date_format)
{
  if (count(explode ("-",$date))==3) //verifier si la date est bien formatee
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
  else
    return "invalid";
}

function iso_to_local($date,$date_format)
{
  if ($date!='%%') //pas "tous"
  {
    $list_date=explode ("-",$date);
    if ($date_format=='fr')
    {
    //on repasse en iso les dates qui arrivent en francais
      $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
    }
    elseif ($date_format=='en')
      $date=$list_date[1].'-'.$list_date[2].'-'.$list_date[0];
  }
  return $date;
}

//le textarea de nouveau document ou document modifie
if (isset($_REQUEST['obs_modifiee'])) //on recupere le contenu de l'observation s'il existe
{
  $patient=$_REQUEST['numeroID'];
  $obs_modifiee=stripslashes($_REQUEST['obs_modifiee']);
  $obs_modifiee=str_replace('\n',"<br />",$obs_modifiee); // remplace les fins de ligne par du html
  $obs_modifiee=str_replace('<br>',"<br />",$obs_modifiee); // remplace les balises par du html propre
  $obs_modifiee=str_replace('border: 1px dashed #AAAAAA','border:0px dashed #AAAAAA',$obs_modifiee); //On efface les bordures pointillees des tableaux openwysiwig
  $titre_observation=$_REQUEST['titre_observation'];
  $date_modifiee=$_REQUEST['date_modifiee']; //format local

  $heure_observation_modifiee=$_REQUEST['heure_observation_modifiee'];
}

if (isset($_REQUEST['ordostruct'])) //modification d'ordonnance pour enregistrement
{
  $ordostruct=$_REQUEST['ordostruct'];

  $obs_modifiee='<?xml version="1.0" encoding="ISO-8859-1" standalone="yes" ?>
<ordotext>
<html><head><meta name="qrichtext" content="charset=utf-8" /></head><body text="#000000"  style="font-size:10pt;font-family:MS Shell Dlg">'.$obs_modifiee.'
</body></html></ordotext>'.$ordostruct.'
';
//echo $obs_modifiee;
}

if (isset ($_REQUEST['rubrique'])) //deroulant des rubriques ou ordonnance structuree
{
  $type=$_REQUEST['rubrique'];

  if ($type!='20020100') // les ordonnances structurees sont traitees ailleurs
  {
    $obs_modifiee='<html><head><meta name="qrichtext" content="charset=utf-8" /></head><body text="#000000"  style="font-size:10pt;font-family:MS Shell Dlg"><p>'.$obs_modifiee.'</p>
</body></html>
';
    $obs_modifiee=str_replace('border:1px dashed #AAAAAA','border:0px dashed #AAAAAA',$obs_modifiee); //On efface les bordures pointillees des tableaux openwysiwig
    $obs_modifiee=str_replace('<hr>','<hr />',$obs_modifiee);
    $obs_modifiee=str_replace('<br>','<br />',$obs_modifiee);
    $obs_modifiee=str_replace('<p></p>','',$obs_modifiee);
  }
  $date_complete=local_to_iso($date_modifiee,$date_format).' '.$heure_observation_modifiee; //format iso
  if ($_REQUEST['button_valider']=="Valider")//bouton enregistrer 
  {
  //requetes d'insertion
  //on insere le texte
    $sql_insert_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$patient')",mysqli_real_escape_string($db,$obs_modifiee));
    $resultat_insert_blob=mysqli_query($db,$sql_insert_blob);
	    
  //on recupere l'ID du blob
    $id_blob=mysqli_insert_id($db);
  //on insere l'en-tete
    $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$patient','$type','%s','$date_complete','$user','$signuser','$id_blob')",mysqli_real_escape_string($db,$titre_observation));
    $resultat_insert_header=mysqli_query($db,$sql_insert_header);
  }
  elseif ($_REQUEST['button_valider']=="Modifier")
  {
    $observation_ID=$_REQUEST['observation_ID'];
    $sql_blobs=sprintf("UPDATE RubriquesBlobs SET RbDate_DataRub='%s' WHERE RbDate_PrimKey='$observation_ID'",mysqli_real_escape_string($db,$obs_modifiee));
    $sql_header=sprintf("UPDATE RubriquesHead SET RbDate_NomDate='%s',RbDate_Date='$date_complete', RbDate_TypeRub=$type WHERE RbDate_RefBlobs_PrimKey='$observation_ID'",mysqli_real_escape_string($db,$titre_observation));
    $resultat_blobs=mysqli_query($db,$sql_blobs);
    $resultat_header=mysqli_query($db,$sql_header);	
  }
    //On enleve le verrou a l'enregistrement		

  $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$patient'";
  $resultat_enlever_verrou=mysqli_query($db,$sql_enlever_verrou);
  $date=substr($date_complete,0,10);
  header ('location: consultation.php?numeroID='.$patient.'&date='.$date );
  exit;
}

if (isset ($_REQUEST['bouton_annuler'])) //on enleve le verrou
{
  $date=$_REQUEST['date'];
  $patient=$_REQUEST['numeroID'];
  $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$patient'";
  $resultat_enlever_verrou=mysqli_query($db,$sql_enlever_verrou);
  header ('location: consultation.php?numeroID='.$patient.'&date='.$date );
  exit;
}
$afficher_boutons=1; //On affichera les boutons de modification-impression seulement si on n'est pas en train d'éditer un document

if (isset($_REQUEST['date']))
  $date=$_REQUEST['date']; //format iso
elseif (isset($_REQUEST['date_modifiee']))
  $date=local_to_iso(substr($_REQUEST['date_modifiee'],0,10),$date_format);
else //ID mais pas de date, on revient sur la page patient
{
  header ('location: patient.php?numeroID='.$patient );
  exit;
}
if (isset($_REQUEST['numeroID']))
{
  $patient=$_REQUEST['numeroID'];
//On cherche dans la table IndexNomPrenom la fiche dont l'ID a ete demande
  $sql_chercher_nom="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
  $resultat_chercher_nom=mysqli_query($db,$sql_chercher_nom);
  $ligne_chercher_nom=mysqli_fetch_array($resultat_chercher_nom);
  $nom=$ligne_chercher_nom["FchGnrl_NomDos"];
  $prenom=$ligne_chercher_nom["FchGnrl_Prenom"];

  $sql_chercher_caracteristiques_patient="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$patient'";
  $resultat_chercher_caracteristiques_patient=mysqli_query($db,$sql_chercher_caracteristiques_patient);
  $ligne_chercher_caracteristiques_patient=mysqli_fetch_array($resultat_chercher_caracteristiques_patient);
  $naissance_full=$ligne_chercher_caracteristiques_patient['FchPat_Nee'];
  $naissance_tableau=explode(' ',$naissance_full);
  $naissance=local_to_iso($naissance_tableau[0],$date_format);
  $adresse=$ligne_chercher_caracteristiques_patient['FchPat_Adresse'].' '.$ligne_chercher_caracteristiques_patient['FchPat_CP'].' '.$ligne_chercher_caracteristiques_patient['FchPat_Ville'];
  $secu=$ligne_chercher_caracteristiques_patient['FchPat_NumSS'];
  $profession=$ligne_chercher_caracteristiques_patient['FchPat_Profession'];
  $sexe='I'; //inutile, juste pour eviter erreur a inclusion template
  $genre['I']='';
  $titre=$ligne_chercher_caracteristiques_patient['FchPat_Titre'];

  include("templates.php");

  $sql_chercher_ville="SELECT Ville FROM Personnes WHERE Login='$signuser'";

  $resultat_chercher_ville=mysqli_query($db,$sql_chercher_ville);
  $ligne_chercher_ville=mysqli_fetch_array($resultat_chercher_ville);
  $ville=$ligne_chercher_ville['Ville'];
$type_nouveau='';
  if (isset ($_REQUEST['titre_indice'])) //si on a transmis un indice de titre (nouveau document)
  {
    $afficher_boutons=0;
    $titre_indice=$_REQUEST['titre_indice'];
    $titre=$titre_default[$titre_indice];
    $type_nouveau=$type_default[$titre_indice];
    $fiche_mieux=$obs_default[$titre_indice];
  }
  if (isset($_REQUEST['nouveau']))
  {
    if ($_REQUEST['nouveau'] =="Modifier") //ne pas afficher les boutons si en mode modification d'un document
    {	  
      $afficher_boutons=0;
    }
  }
}
else //pas d'ID,on retourne en page de recherche
{
  header ('location: liste.php' );
  exit;
}

if (!$date)
{
  header ('location: patient.php?numeroID='.$patient );
  exit;
}
if (isset($_REQUEST['nouveau']))
{
  if ($_REQUEST['nouveau']=='Confirmer la suppression') //On supprime definitivement le document
  {
    $observation_ID=$_REQUEST['observation_ID'];
    $sql_recup_obs="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesBlobs.RbDate_PrimKey='$observation_ID'";
    $resultat_recup_obs=mysqli_query($db,$sql_recup_obs);
    $ligne_recup_obs=mysqli_fetch_array($resultat_recup_obs);
    $patient=$ligne_recup_obs['RbDate_IDDos'];
    $date_complete=$ligne_recup_obs['RbDate_Date'];

    $sql_delete_blob="DELETE FROM RubriquesBlobs WHERE RbDate_PrimKey='$observation_ID'";
    $sql_delete_header="DELETE FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$observation_ID'";
    $resultat_delete_blob=mysqli_query($db,$sql_delete_blob);
    $resultat_delete_header=mysqli_query($db,$sql_delete_header);

  //On retire le verrou sur la fiche
    $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$patient'";
    $resultat_enlever_verrou=mysqli_query($db,$sql_enlever_verrou);
    $date=substr($date_complete,0,10);
    $patient=$_REQUEST['numeroID'];
    header ('location:consultation.php?numeroID='.$patient.'&date='.$date);
    exit;
  }
  elseif ($_REQUEST['nouveau']=='Confirmer le changement de patient')
  {
  //On retire le verrou sur la fiche initiale
    $patient_orig=$_REQUEST['patient_orig'];
    $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$patient_orig'";
    $resultat_enlever_verrou=mysqli_query($db,$sql_enlever_verrou);

    $observation_ID=$_REQUEST['ID_document'];
    $numeroID=$_REQUEST['numeroID'];
    $date=$_REQUEST['date'];

    $sql_modifier_blob="UPDATE RubriquesBlobs SET RbDate_IDDos='$numeroID' WHERE RbDate_PrimKey='$observation_ID'";
    $resultat_modifier_blob=mysqli_query($db,$sql_modifier_blob);
    $sql_modifier_head="UPDATE RubriquesHead SET RbDate_IDDos='$numeroID' WHERE RbDate_RefBlobs_PrimKey='$observation_ID'";
    $resultat_modifier_head=mysqli_query($db,$sql_modifier_head);

    echo("<script language=\"javascript\">");
    echo("top.location.href = \"frame_patient.php?GUID=".$numeroID."\"");
    echo("</script>");  
    exit;
  }
}
$date=$date."%"; //iso

include("inc/header.php");
?>

  <script type="text/javascript">
//<![CDATA[
function info_verrou()
{
  alert ('Le système de verrous est présent essentiellement pour assurer la comptabilité avec MedinTux.\nEn effet, alors que MedWebTux enregistre les documents modifiés à l\'unité, MedinTux enregistre plusieurs documents d\'un coup. Il est alors impossible de travailler à plusieurs simultanément sur un même dossier patient.\nSur MedWebTux, hormis le rare cas où deux personnes travaillent sur le même document simultanément, aucun écrasement ne se produira.\nUn verrou est posé lors de l\'édition d\'un document et est retiré lors de sa validation ou de son annulation.\nLes verrous n\'empêchent aucune action, ils sont juste informatifs.\nIl est fréquent qu\'un verrou soit posé accidentellement, par exemple lorsqu\'une fenêtre d\'édition a été fermée sans validation. Dans ce cas, vous avez juste à ne pas en tenir compte.');
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
  window.open('recherche_patient_agenda.php?Nom='+nom+'&Prenom='+prenom+'&from=consultation','Choisir','width=800,height=550,top=50,left=50,toolbar=no,scrollbars=yes,resizable=yes,location=no'); 
}
//]]>
  </script>

    <script type="text/javascript">
//<![CDATA[
function showButton()
{
  document.getElementById('button_place').style.display = 'inline';
}
//]]>
    </script>

    <script type="text/javascript">
//<![CDATA[
function showButtons(number)
{
  document.getElementById('buttons'+number).style.display = 'inline';
  document.getElementById('actions'+number).style.display = 'none';
}
//]]>
    </script>

<?php
//Nom=TARTEMPION&Prenom=MARCEL&nouveau=Confirmer+le+changement+de+patient&numeroID=80AEF051-923C-034B-ABAE-1003257EE9A8&date=2013-04-17&ID_document=321798
if ((isset($_REQUEST['observation_ID']) OR isset($_REQUEST['nouveau'])) AND !isset($_REQUEST['button_valider'])) //afficher la zone de saisie-modification (ancien observation.php si besoin seulement)
{
  //Duplication de document. On trouve le contenu du document, on met la date courant, on enregistre et on retourne sur la page de journee avec la nouvelle date.
    if ($_REQUEST['nouveau']=='Dupliquer') 
    {
      $observation_ID=$_REQUEST['observation_ID'];
      $sql_recup_obs="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesBlobs.RbDate_PrimKey='$observation_ID'";
      $resultat_recup_obs=mysqli_query($db,$sql_recup_obs);
      $ligne_recup_obs=mysqli_fetch_array($resultat_recup_obs);
      $obs=$ligne_recup_obs['RbDate_DataRub'];

      $date_complete=date('Y-m-d H:i:s',date('U') ); //la date courante iso
      $sql_insert_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$patient')",mysqli_real_escape_string($db,$obs));
      $resultat_insert_blob=mysqli_query($db,$sql_insert_blob);
      $titre=$ligne_recup_obs['RbDate_NomDate'];
      $patient=$ligne_recup_obs['RbDate_IDDos'];
      $obs=$ligne_recup_obs['RbDate_DataRub'];
      $type_document_code=$ligne_recup_obs['RbDate_TypeRub'];
	      
    //on recupere l'ID du blob
      $id_blob=mysqli_insert_id($db);
    //on insere l'en-tete
      $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$patient','$type_document_code','%s','$date_complete','$user','$signuser','$id_blob')",mysqli_real_escape_string($db,$titre));
      $resultat_insert_header=mysqli_query($db,$sql_insert_header);
      header ('location:consultation.php?numeroID='.$patient.'&date='.substr($date_complete,0,10));
      exit;
    }
    elseif ($_REQUEST['nouveau']=='Confirmer la suppression') 
    {
//ne pas afficher le wysiwyg
    }
    else //ni dupliquer, ni confirmer la suppression
    {
?>
<!-- 
	Include the WYSIWYG javascript files
-->
    <script type="text/javascript" src="scripts/wysiwyg.js"></script>
    <script type="text/javascript" src="scripts/wysiwyg-settings.js"></script>
<!-- 
	Attach the editor on the textareas
-->
    <script type="text/javascript">
//<![CDATA[
	// Use it to attach the editor to all textareas with full featured setup
	//WYSIWYG.attach('all', full);
	var mysettings = new WYSIWYG.Settings();
	mysettings.Width = "100%"; 
	full.ReplaceLineBreaks = true; //remplacer les retours chariot du text area par des br

	// Use it to attach the editor directly to a defined textarea
	WYSIWYG.attach('obs_modifiee',full); // small setup
	
	// Use it to display an iframes instead of a textareas
	//WYSIWYG.display('all', full);  
//]]>
    </script>

<?php
  }
}
?>
  <title>
    Journée de <?php echo stripslashes($nom).' '.stripslashes($prenom) ?> - Utilisateur <?php echo $_SESSION['login'] ?>
  </title>

</head>

<body style="font-size:<?php echo $fontsize; ?>pt" >
  <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor="Mode_consultation";
//include("inc/menu-horiz.php");

$date_locale=iso_to_local(substr($date,0,10),$date_format);

?>
 <div class="groupe">
    <h1>
      Journée du <?php echo $date_locale ;
if ($afficher_boutons AND stripos($date,'%')) //pas de bouton ajouter un honoraire si on est en mode édition ou si on ne connait pas la date
{
?>
       <form action="compta/saisie_hono.php" method="get" style="float:right" target="_top">
	<div>
	  <input name="guid_patient" type="hidden" value="<?php echo $patient ?>"/>
	  <input name="date" type="hidden" value="<?php echo substr($date_locale,0,10) ?>"/>
	  <input name="nouveau" type="submit" value="Ajouter un honoraire" />
	</div>
      </form>
<?php
}
?>
    </h1>
<?php
  //verification des verrous
  $sql_verifier_verrou="SELECT * FROM IndexNomPrenom INNER JOIN Verrous ON IndexNomPrenom.FchGnrl_IDDos=Verrous.DossGUID WHERE IndexNomPrenom.FchGnrl_IDDos='$patient'";
  $resultat_verifier_verrou=mysqli_query($db,$sql_verifier_verrou);
  $ligne_verifier_verrou=mysqli_fetch_array($resultat_verifier_verrou);//message erreur
  $count_verifier_verrou=mysqli_num_rows($resultat_verifier_verrou);

  if ($count_verifier_verrou) //Un verrou existe sur ce dossier. On ne fait rien et on affiche une alerte
  {
    echo " 
     <div class='notice'>
	<strong>Attention !</strong> Dossier verrouill&eacute; par ".$ligne_verifier_verrou['UserName'];
    if ($ligne_verifier_verrou['UserName']==$loginbase."@".$host)
    {
?>
       (vous-m&ecirc;me)
<?php
    }
?>
	<input type="button" value="Informations" onclick="info_verrou()" />
      </div>
<?php
  }

if (isset($_REQUEST['titre_indice']) OR isset ($_REQUEST['observation_ID'])) //pose de verrou si nouvelle observation ou modification ou suppression
{
    $datime=date('YmdHis', date('U'))."000";
    $mysqli_user=$loginbase."@".$host;

    $sql_poser_verrou="INSERT INTO Verrous VALUES (NULL,'$datime','All Rubriques','0','$patient','$mysqli_user',NULL,'0')";//pose du verrou
    $resultat_poser_verrou=mysqli_query($db,$sql_poser_verrou);
}

//On cherche le contenu de tout
$rubriques=array();
$sql_global="SELECT * FROM RubriquesHead INNER JOIN RubriquesBlobs ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesHead.RbDate_Date LIKE '$date' AND RubriquesHead.RbDate_IDDOs='$patient' ORDER BY RbDate_TypeRub";
$resultat_global=mysqli_query($db,$sql_global);
$count_global=mysqli_num_rows($resultat_global);

//boucle pour compter le nombre de rubriques differentes
while ($ligne_global=mysqli_fetch_array($resultat_global))
{
  if (!in_array( $ligne_global['RbDate_TypeRub'],$rubriques))
  {
    if ($ligne_global['RbDate_TypeRub'] != '20060000') //on exclut les terrains
    {    
      $rubriques[]=$ligne_global['RbDate_TypeRub'];
    } 
  }
}
  $nb_col=count($rubriques);
?>
  <div class="tableau">
    <table>
      <tr>
	<th class="fond_th">
	  Dates
	</th>
<?php

if ((isset($_REQUEST['observation_ID']) OR isset($_REQUEST['nouveau'])) AND !isset($_REQUEST['button_valider']))
{
?>
	<th class="fond_th">
	  Édition
	</th>
<?php
}

//$rubrique_perso[] = "Alertes|al|90010000";

//boucle pour creer les colonnes en fonction des rubriques
for ($i=0;$i< $nb_col;$i++)
{
  $found=0;
  if ($rubriques[$i] != '20060000') //Pas les terrains
  {
    echo '
	<th class="fond_th">';
    foreach ($rubrique_perso AS $this_rubrique)
    {
      $elts_rubrique=explode("|",$this_rubrique);
      if ($elts_rubrique[2]==$rubriques[$i])
      {
	echo "
	".$elts_rubrique[0]; //trouver le titre de la rubrique en fonction du numero
	$found=1;
      }
    }
    if ($found==0)
      echo 'Non défini (type '.$rubriques[$i].' à mettre dans config.php)';
    echo "
	</th>";
  }
}
?>
      </tr>
      <tr>
	<td class="fond_td" valign="top" title="Un clic sur une date envoie sur cette date">
<?php
if ($afficher_boutons)
{
?>
	  <a href="consultation.php?numeroID=<?php echo $patient ?>&amp;date=%&amp;nom=<?php echo $patient ?>&amp;prenom=<?php echo $patient ?>">
<?php
}
?>
	    Toutes
<?php
if ($afficher_boutons)
{
?>
	  </a>
<?php
}
?>
	  <br />
<?php
//recherche des dates
$sql1="SELECT * FROM RubriquesHead WHERE RbDate_IDDos='$patient' ORDER BY RbDate_Date DESC";
$resultat1=mysqli_query($db,$sql1);
$count1=mysqli_num_rows($resultat1);
$date_unique="";

//on nettoie les balises html non valides en xhtml
$find=array('<BR>','<br>','<hr>','<u>','</u>');
$replace=array('<br />','<br />','<br />','<em>','</em>');

function verif_login($login_possesseur_doc,$droits_user,$GUID_user,$num_rubrique,$user,$rubrique_perso,$db,$base)
{
  $droit_r='toto'; //eviter les warnings pour les rubriques non definies
  $droit_w='toto';
  $droit_p='toto';

  foreach ($rubrique_perso AS $this_rubrique_perso) //On analyse les rubriques prises dans config.php
  {
    $elts_rubrique=explode("|",$this_rubrique_perso); //Premier element = nom du droit, deuxieme=nom abrege, troisieme = numero de code

    if ($elts_rubrique[2]==$num_rubrique) //ne marche pas pour toutes dates !
    {
      $droit_r=$elts_rubrique[1]."v"; //see
      $droit_w=$elts_rubrique[1]."c"; //write
      $droit_p=$elts_rubrique[1]."p"; //print
    }
  }
  if ($login_possesseur_doc==$user) //On verifie si l'observation appartient a l'utilisateur
  {
    if (preg_match("/".$droit_r."/",$droits_user))//si oui, on verifie qu'il a les droits
    {
      $droit['r']="1";
    }
    else //si pas possesseur
      $droit['r']="";
    if (preg_match("/".$droit_w."/",$droits_user))//si oui, on verifie qu'il a les droits
    {
      $droit['w']="1";
    }
    else //si pas possesseur
      $droit['w']="";
    if (preg_match("/".$droit_p."/",$droits_user))//si oui, on verifie qu'il a les droits
    {
      $droit['p']="1";
    }
    else //si pas possesseur
      $droit['p']="";
  }
  else //sinon on verifie qu'il est utilisateur autorise
  {
    $sql_GUID_possesseur_doc="SELECT GUID FROM Personnes WHERE Login='$login_possesseur_doc'";
    $resultat_GUID_possesseur_doc=mysqli_query($db,$sql_GUID_possesseur_doc);
    $ligne_GUID_possesseur_doc=mysqli_fetch_array($resultat_GUID_possesseur_doc);
    $GUID_possesseur_doc=$ligne_GUID_possesseur_doc['GUID'];
    $sql_utilisateur_autorise="SELECT * FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_possesseur_doc'";
    $resultat_utilisateur_autorise=mysqli_query($db,$sql_utilisateur_autorise);
    $count_utilisateur_autorise=mysqli_num_rows($resultat_utilisateur_autorise);
    if ($count_utilisateur_autorise)//S'il existe un utilisateur autorise
    {
      $liste_utilisateur_autorise=mysqli_fetch_array($resultat_utilisateur_autorise);
      if (preg_match("/".$droit_r."/",$liste_utilisateur_autorise['FriendUserDroits']))//On verifie qu'il a les droits
	$droit['r']=1;
      else //si connu pas autorise a lire
	$droit['r']="";
       if (preg_match("/".$droit_w."/",$liste_utilisateur_autorise['FriendUserDroits']))//On verifie qu'il a les droits
	$droit['w']=1;
      else //si connu pas autorise a ecrire
	$droit['w']="";
       if (preg_match("/".$droit_p."/",$liste_utilisateur_autorise['FriendUserDroits']))//On verifie qu'il a les droits
	$droit['p']=1;
      else //si connu pas autorise a ecrire
	$droit['p']="";
    }
    else //si pas connu
    {
      $droit['r']="";
      $droit['w']="";
      $droit['p']="";
    }
  }
  return $droit;
}

while ($ligne1=mysqli_fetch_array($resultat1))//chercher les dates
{
  $date_courte=substr($ligne1['RbDate_Date'],0,10); //iso
  {
    if ($date_courte!=$date_unique)//une seule occurrence par date
    {
      $date_unique=$date_courte;
      if ($afficher_boutons)
      {
	echo "
	    <a href=\"consultation.php?numeroID=".$patient."&amp;date=".$date_unique."&amp;nom=".$nom."&amp;prenom=".$prenom."\">
	      ".local_to_iso($date_courte,$date_format)."
	    </a><br />";
      }
      else
	echo local_to_iso($date_courte,$date_format)."<br />";
    }
  }			
} //fin de la colonne des dates
  echo '
	  </td>';
$type_document_code='';
  //mode formulaire de modification

$type_document_clair['20020100']='Ordonnance structurée';
$type_document_clair['20080000']='Document';
$type_document_clair['20030000']='Observation';
$type_document_clair['20020200']='Prescription';
$type_document_clair['20020300']='Certificat';
$type_document_clair['20020500']='Courrier';
$type_document_clair['20090000']='Notes avancées';

if ((isset($_REQUEST['observation_ID']) OR isset($_REQUEST['nouveau'])) AND !isset($_REQUEST['button_valider'])) //afficher la zone de saisie-modification (ancien observation.php si besoin seulement)
{
?>
	  <td class="fond_td" valign="top" >
<?php
  if (isset($_REQUEST['observation_ID'])) //document fourni = modifier ou supprimer
  {
    $observation_ID=$_REQUEST['observation_ID'];
  //On trouve le possesseur de l'observation
    $sql_trouver_possesseur="SELECT RbDate_CreateSignUser FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$observation_ID'";

    $resultat_trouver_possesseur=mysqli_query($db,$sql_trouver_possesseur);
    $ligne_trouver_possesseur=mysqli_fetch_array($resultat_trouver_possesseur);
    $ID_possesseur=$ligne_trouver_possesseur['RbDate_CreateSignUser'];
  //On trouve le GUID du possesseur de l'observation 
    $sql_trouver_GUID_possesseur="SELECT GUID FROM Personnes WHERE Login='$ID_possesseur'";
    $resultat_trouver_GUID_possesseur=mysqli_query($db,$sql_trouver_GUID_possesseur);
    $ligne_trouver_GUID_possesseur=mysqli_fetch_array($resultat_trouver_GUID_possesseur);
    $GUID_possesseur=$ligne_trouver_GUID_possesseur['GUID'];


    $sql_recup_obs="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesBlobs.RbDate_PrimKey='$observation_ID'";
    $resultat_recup_obs=mysqli_query($db,$sql_recup_obs);
    $ligne_recup_obs=mysqli_fetch_array($resultat_recup_obs);
    $titre=$ligne_recup_obs['RbDate_NomDate'];
    $patient=$ligne_recup_obs['RbDate_IDDos'];
    $date_complete=$ligne_recup_obs['RbDate_Date'];
    $obs=stripslashes($ligne_recup_obs['RbDate_DataRub']);
    $type_document_code=$ligne_recup_obs['RbDate_TypeRub'];
 
    if ($type_document_code=='20020100') //ordonnance structuree : on separe le xml du html pour edition
    {
      $struct=preg_match('/<ordoMedicaStruct>(.*)<\/ordoMedicaStruct>/si', $obs, $tab_struct);
      $ordotext=preg_match('/<ordotext(.*)<\/ordotext>/si', $obs, $tab_text);
      $obs=$tab_text[0];
      $struct=$tab_struct[0];
    }

    $date_longue=$ligne_recup_obs['RbDate_Date']; //iso
    $date=iso_to_local(substr($date_longue,0,10),$date_format);

    $heure=substr($date_longue,11,8);

  //extraction du contenu des balises body
    if ($fiche_mieux=preg_match('/<body[^>]*>(.*)<\/body>/si', $obs, $tab))
    {
      $fiche_mieux=$tab[1];
    }
    else //si pas de balises body
      $fiche_mieux=$obs;
    $find=array('<p>','</p>',"\0");
    $replace=array('','','');
    $obs=str_replace($find,$replace,$obs);
  }
  elseif (isset($_REQUEST['nouveau'])) //Initialisation des champs si nouvelle observation
  {
    if ($_REQUEST['nouveau']=="Ajouter")
    {
	$date=iso_to_local(date('Y-m-d'),$date_format);
	$heure=date("H:i:s");
    }
  }
}
if ((isset($_REQUEST['observation_ID']) OR isset($_REQUEST['nouveau'])) AND !isset($_REQUEST['button_valider']))
{
  if ($_REQUEST['nouveau']=='Supprimer')
  {
?>
	    <form action="consultation.php" method="get">
	      <fieldset>
	      <legend>
		Suppression définitive
	      </legend>
	      Voulez-vous vraiment supprimer ce document ? <br />
	      Il n'y aura aucun moyen de le récupérer. <br />
	      <input name="nouveau" type="submit" value="Confirmer la suppression" />
	      <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
	      <input name="date" type="hidden" value="<?php echo local_to_iso($date,$date_format) ?>" />
	      <input name="bouton_annuler" type="submit" value="Annuler" title="Ce document sera abandonné et le verrou sur la fiche retiré"/>
	      <input name="observation_ID" type="hidden" value="<?php echo $observation_ID ?>" />
	      </fieldset>
	    </form>
<?php
  }
  elseif ($_REQUEST['nouveau']=='Réattribuer')
  {
?>
	  <form action="consultation.php" method="get" id="form_jour">
	    <fieldset>
	      <legend>
		Changement de patient
	      </legend>
	      <table>
		<tr>
		  <td style="align:right;">
		    <label for="Nom">
		      Nom
		    </label>
		  </td>
		  <td>
		    <input name="Nom" id="Nom" type="text" value="" />
		  </td>
		</tr>
		<tr>
		  <td>
		    <label for="Prenom">Prénom</label>
		  </td>
		  <td>
		    <input name="Prenom" id="Prenom" type="text" value="" />
		  </td>
		</tr>
		<tr>
		  <td>
		  </td>
		  <td>
		    <input type="button" value="Choisir un patient" onclick="chercher_patient()" />
		  </td>
		</tr>
		<tr>
		  <td>
		    <div id="submit_confirm" style="display:none;" >
		      <input name="patient_orig" type="hidden" value="<?php echo $_REQUEST['numeroID'] ?>"/>
		      <input name="nouveau" type="submit" value="Confirmer le changement de patient" />
		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>"/>
		      <input name="date" type="hidden" value="<?php echo local_to_iso($date,$date_format) ?>" />
		      <input name="ID_document" type="hidden" value="<?php echo $observation_ID ?>" />
		    </div>
		  </td>
		</tr>
	      </table>
	    </fieldset>
	  </form>
<?php
  }
?>
	    <div class="notice" style="width: 400px;">
	      <strong>Avertissement</strong>&nbsp;: les données saisies dans ce cadre ne sont pas exploitées pour le contrôle de sécurité des ordonnances médicamenteuses.
	    </div>

	    <form action="consultation.php" method="post" id="form_observation">
	      <table  style="width:auto;margin-left:auto;margin-right:auto;">
		<tr>
		  <th colspan="2">
		    <input name="numeroID" id="numeroID" type="hidden" value="<?php echo $patient ?>" />
		    <label for="titre_observation">
		      Titre&nbsp;:
		    </label>
		    <input name="titre_observation" id="titre_observation" type="text" value="<?php echo $titre ?>" size="20" maxlength="40"/>
		  </th>
		</tr>
		<tr>
		  <th>
		    <label for="datepicker">
		      Date&nbsp;:
		    </label>
		    <input type="text" name="date_modifiee" id="datepicker" value="<?php echo $date?>" size="13" /></th>
		  <th>
		    <label for="heure_observation_modifiee">
		      Heure&nbsp;:
		    </label>
		    <input name="heure_observation_modifiee" id="heure_observation_modifiee" type="text" value="<?php echo $heure?>" size="6" maxlength="8"/>
		  </th>
		</tr>
		<tr>
		  <th colspan="3">
		  <textarea name="obs_modifiee" id="obs_modifiee" rows="25" cols="60"  ><?php echo $fiche_mieux ?></textarea>
		  </th>
		</tr>
		<tr>
		  <th colspan="3">
<?php
  if (isset ($_REQUEST['observation_ID'])) //Bouton Modifier de la fiche patient
  {
    if ($type_document_code!='20020100') //pas de "en tant que" pour les ordonnances structurees
    {
?>
		    Modifier en tant que : 
<?php
    }
?>
		    <input type="hidden" name="observation_ID" value="<?php echo $observation_ID ?>" />
<?php
  }
  else
  {
?>
		    Enregistrer en tant que : 
<?php
  }
  if ($type_document_code!='20020100') //pas de select si ordonnance structuree
  {
?>
		    <select name="rubrique">
<?php
//Chercher si l'utilisateur a des droits en ecriture dans les differentes rubriques
    $droits_delegues='';
    if ($user!=$signuser)
    {
	$sql_GUID_signuser="SELECT GUID FROM Personnes WHERE Login='$signuser'";
	$resultat_GUID_signuser=mysqli_query($db,$sql_GUID_signuser);
	$ligne_GUID_signuser=mysqli_fetch_array($resultat_GUID_signuser);

	$sql_GUID_user="SELECT GUID FROM Personnes WHERE Login='$user'";
	$resultat_GUID_user=mysqli_query($db,$sql_GUID_user);
	$ligne_GUID_user=mysqli_fetch_array($resultat_GUID_user);

	$GUID_user=$ligne_GUID_user['GUID'];
	$GUID_signuser=$ligne_GUID_signuser['GUID'];

	$sql_droits_delegues="SELECT FriendUserDroits FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_signuser'";
	$resultat_droits_delegues=mysqli_query($db,$sql_droits_delegues);

	$ligne_droits_delegues=mysqli_fetch_array($resultat_droits_delegues);
	$droits_delegues= $ligne_droits_delegues['FriendUserDroits'];
    }

    // Champs personnalisé
    if (isset ($rubrique_perso))
    {
      foreach ($rubrique_perso AS $this_rubrique) //pris dans le config.php
      {
	$elts_rubrique=explode("|",$this_rubrique);
	if ($elts_rubrique[2]!='20050000')
	{
	  if (stripos($ligne_chercher_droits['Droits'],$elts_rubrique[1].'c') OR (stripos($droits_delegues,$elts_rubrique[1].'c'))) //Ne pas proposer d'enregistrer si l'utilisateur n'a pas les droits directs ou delegues
	  {
?>
		      <option value="<?php echo $elts_rubrique[2] ?>" <?php if ($type_document_code==$elts_rubrique[2] OR $type_nouveau==$elts_rubrique[2]) echo "selected=\"selected\"" ?> >
			<?php echo $elts_rubrique[0] ?>
		      </option>
<?php
	  }
	}
      }
    }
?>
		    </select>
<?php
  } //fin du deroulant du type
  else //pas de deroulant, mais valeur pour ordo structuree
  {
?>
		    <input name="rubrique" type="hidden" value="20020100" />
		    <input name="ordostruct" type="hidden" value="<?php echo $struct ?>" />
<?php
  }
?>
		    <input name="button_valider" type="submit" value="<?php if (isset ($_REQUEST['observation_ID'])) echo "Modifier"; else echo "Valider" ?>" />
		  </th>
		</tr>
	      </table>
	    </form>
	    <form action="consultation.php" method="post" >
	      <div>
		<input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		<input name="date" type="hidden" value="<?php echo local_to_iso($date,$date_format) ?>" />
		<input name="bouton_annuler" type="submit" value="Annuler" title="Ce document sera abandonné et le verrou sur la fiche retiré"/>
	      </div>
	    </form>
	  </td>
<?php
}
$zone=0; //compteur pour les zones des boutons
//print_r($rubriques);
foreach ($rubriques AS $this_rubrique) //le tableau des differents documents
{
?>
	  <td class="fond_td" valign="top" >
<?php
  mysqli_data_seek($resultat_global,0) ; //remise a zero tableau requete
  while ($ligne_global=mysqli_fetch_array($resultat_global)) //on cherche les differents documents de la rubrique
  {
    $afficher=1;
    $zone++;
    $id_document=$ligne_global['RbDate_RefBlobs_PrimKey'];
    if (isset($_REQUEST['observation_ID']))
    {
      if ($_REQUEST['observation_ID'] == $id_document)
      {
	$afficher=0;
      }
    }
    if ($afficher) //ne pas afficher le document s'il est en mode edition
    {
      $login_possesseur_doc=$ligne_global['RbDate_CreateSignUser'];
      $login_createur_doc=$ligne_global['RbDate_CreateUser'];
      $droits=array();
      $droits=verif_login($login_possesseur_doc,$droits_user,$GUID_user,$this_rubrique,$user,$rubrique_perso,$db,$base);

      if ($ligne_global['RbDate_TypeRub'] == $this_rubrique) //on affiche les contenus dans la colonne de la rubrique predefinie
      {
	$date_seule_iso=explode (' ',$ligne_global['RbDate_Date']);
	$date_document=iso_to_local($date_seule_iso[0],$date_format);
	echo "
	      <strong>".str_replace($find,$replace,$ligne_global['RbDate_NomDate'])."</strong> (créé par $login_createur_doc pour $login_possesseur_doc) le $date_document<br />";
	if ($droits['r'] AND $this_rubrique!='20060000')
	{
	  if ($this_rubrique=='20020100') //ordo structuree
	  {
	    if ($fiche_mieux=preg_match('/<body[^>]*>(.*)<\/body>/si', $ligne_global["RbDate_DataRub"], $tabordo))
	    {
	      $fiche_encore_mieux=stripslashes($tabordo[1]);
	    }
	    else //au cas ou le texte n'est pas entoure de balises body
	      $fiche_encore_mieux=stripslashes($ligne_global["RbDate_DataRub"]);
	    echo str_replace($find,$replace,$fiche_encore_mieux);
	  }
	  else //documents non ordo
	  {
	    if (stripos($ligne_global["RbDate_DataRub"],'</HTML_Data>')) //documents multimedia
	    {
	      $string=str_replace ('</HTML_Data>','<HTML_Data>',$ligne_global["RbDate_DataRub"]);
	      $expl=explode('<HTML_Data>',$string);
	      $html=strip_tags($expl[1],'<br /><img>');
	      $elements=explode(' ',$html);
	      $i=0;
	      foreach ($elements as $this_element)
	      {
		if (preg_match('`src`',$this_element))
		{
		  echo 'src="afficher_image.php?RbDate_PrimKey='.$ligne_global["RbDate_RefBlobs_PrimKey"].'&amp;compteur_image='.$i.'" alt="Image" ';
		  $i++;
		}
		else
		  echo $this_element.' ';
	      }
	    }
	    else // document non image
	    {
	    //Nettoyage des balises html
	      $fiche_mieux=preg_match('/<body[^>]*>(.*)<\/body>/si', $ligne_global["RbDate_DataRub"], $tab);
	      $page=str_replace($find,$replace,$tab[1]);
	      echo str_replace('__________________','_',$page); //le contenu de l'observation sans les balises body et en raccourcissant les ____ pour que ca tienne mieux
	    }
	  }
	}
	else //pas les droits de lecture
	{
	  echo "
	    <b>Acc&egrave;s interdit</b> (r&eacute;serv&eacute; &agrave; <b>".$login_possesseur_doc."</b> ou &agrave; ses utilisateurs d&eacute;l&eacute;gu&eacute;s)<br />";
	}
?>
<div id='actions<?php echo $zone ?>' style="display:inline" >
<input type="button" value="Actions" onclick="showButtons(<?php echo $zone ?>)" onmouseover="showButtons(<?php echo $zone ?>)" />
</div>
<div id='buttons<?php echo $zone ?>' style="display:none" >
	    <table>
	      <tr><!-- bouton interactions ou modifier -->
		<td>
<?php
	if ($this_rubrique==20020100 AND $afficher_boutons) //ordo structuree
	{
	  if ($acl=preg_match('/<ordoMedicaStruct>(.*)<\/ordoMedicaStruct>/si', $ligne_global["RbDate_DataRub"], $tab_acl))
	  {
	    if (!mysqli_select_db($db,$basemed))
	    {
	      print "Erreur ".mysqli_error()."<br />";
	      mysqli_close($dbmed);
	      exit;
	    }
	  }
?>
		  <form action="interactions.php">
		    <div>
<?php
	  $xml_string=$tab_acl[0];
	  $xml=simplexml_load_string($xml_string); //marche
	  $num=0;
	  foreach ($xml->PosologieList->Posologie  AS $poso)
	  {
	    foreach ($xml->PosologieList->Posologie[$num]->cip  AS $cette_acl)
	    {
?>
		      <input name="cip_medoc[]" type="hidden" value="<?php echo $cette_acl ?>" />
<?php
	    }
	    $num++;
	  }
?>
		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		      <input name="button_validate_form" type="submit" value="Modifier le contenu" />
		    </div>
		  </form>
<?php
	  if (!mysqli_select_db($db,$base)) //on remet la base sur drtuxtest
	  {
	    print "Erreur ".mysqli_error()."<br />";
	    mysqli_close($db);
	    exit;
	  }
	}//fin ordo structuree
?>
		<br />
<?php
	if ($droits['w'] AND $droits['r'] AND $afficher_boutons) //Pas de bouton Modifier si pas droit de lire ou ecrire
	{
?>		  <form action="consultation.php" method="get">
<div>
		      <input name="observation_ID" type="hidden" value="<?php echo $ligne_global['RbDate_RefBlobs_PrimKey'] ?>"/>
<?php
	  if (!stripos($ligne_global["RbDate_DataRub"],'</HTML_Data>')) //pas de bouton Modifier si le document contient une image
	  {
?>
		      <input name="nouveau" type="submit" value="Modifier"/>  <br />	
<?php
	  }
?>
		      <input name="nouveau" type="submit" value="Supprimer"/>  	<br />
		      <input name="nouveau" type="submit" value="Dupliquer"/><br />
		      <input name="nouveau" type="submit" value="Réattribuer" />
		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>"/>
		      <input name="date" type="hidden" value="<?php echo (substr ($date,0,10)) //iso ?>"/>
		    </div>
		  </form>
<?php
	}
?>
		</td>
		<td>
<?php
	if ($droits['r'] AND $droits['p'] AND $afficher_boutons) //pas de bouton Imprimer si pas droit lecture et impression
	{
?>		  
		  <form action="print.php" method="get">
		    <div>
		      <input name="id_document" type="hidden" value="<?php echo $id_document ?>"/>
		      <input name="nouveau" type="submit" value="Imprimer"/>
		      <br />
		      <input name="nouveau" type="submit" value="Imprimer en duplicata"/>
		      <br />
		      <input name="radio_header" id="radio_header<?php echo $id_document ?>" value="radio_header" type="radio" checked="checked" />
		      <label for="radio_header<?php echo $id_document ?>">Ajouter un en-tête</label><br />
		      <input name="radio_header" id="radio_empty_header<?php echo $id_document ?>" value="radio_empty_header"  type="radio" />
		      <label for="radio_empty_header<?php echo $id_document ?>">Ajouter l'emplacement d'un en-tête</label><br />
		      <input name="radio_header" id="radio_no_header<?php echo $id_document ?>" value="radio_no_header"  type="radio" />
		      <label for="radio_no_header<?php echo $id_document ?>">Imprimer pleine page sans en-tête</label>
		    </div>
		  </form>
<?php
	}
?>
		</td>
	      </tr>
	    </table>
</div>
<!-- fin de la zone des boutons -->
	    <hr />
<?php
      } //fin if lectureOK
    } //fin afficher si pas edition
  } //fin while
?>
      </td>
<?php
} //fin foreach types rubriques
?>
    </tr>
  </table>
 </div>
</div>
<?php
include("inc/footer.php");
?>