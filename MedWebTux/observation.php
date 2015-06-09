<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
}
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$signuser=$tab_login[1];
$signuser=str_replace('\r','',$signuser);
//print_r($tab_login);  Array ( [0] => ines [1] => delafond ) 

include("config.php");
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; $host<br />";
  exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
  print "Erreur ".mysql_error()."<br />";
  mysql_close($db);
  exit;
}

if (isset ($_REQUEST['GUID']))
{
  $GUID=$_REQUEST['GUID'];
}
else
  $GUID="";

if (isset ($_REQUEST['observation_ID'])) //mode Modifier ou dupliquer
{
  $observation_ID=$_REQUEST['observation_ID'];
//On trouve le possesseur de l'observation
  $sql_trouver_possesseur="SELECT RbDate_CreateSignUser FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$observation_ID'";
  $resultat_trouver_possesseur=mysql_query($sql_trouver_possesseur,$db);
  $ligne_trouver_possesseur=mysql_fetch_array($resultat_trouver_possesseur);
  $ID_possesseur=$ligne_trouver_possesseur['RbDate_CreateSignUser'];
//On trouve le GUID du possesseur de l'observation 
  $sql_trouver_GUID_possesseur="SELECT GUID FROM Personnes WHERE Login='$ID_possesseur'";
  $resultat_trouver_GUID_possesseur=mysql_query($sql_trouver_GUID_possesseur,$db);
  $ligne_trouver_GUID_possesseur=mysql_fetch_array($resultat_trouver_GUID_possesseur);
  $GUID_possesseur=$ligne_trouver_GUID_possesseur['GUID'];
}
else
  $observation_ID="";


if (isset($_REQUEST['obs_modifiee'])) //on recupere le contenu de l'observation s'il existe
{
  $obs_modifiee=$_REQUEST['obs_modifiee'];
  $obs_modifiee=str_replace('\n',"<br />",$obs_modifiee); // remplace les fins de ligne par du html
  $obs_modifiee=str_replace('<br>',"<br />",$obs_modifiee); // remplace les balises par du html propre

  $date_modifiee=$_REQUEST['date_modifiee']; //format local
  $heure_observation_modifiee=$_REQUEST['heure_observation_modifiee'];
}
if (isset($_REQUEST['titre_observation'])) //on recupere le titre de l'observation s'il existe
  $titre_observation=$_REQUEST['titre_observation'];
if (isset($_REQUEST['obs']))
{
  $obs=$_REQUEST['obs'];  //nouveau texte apres creation ou modification
  $obs=str_replace('\n',"<br />",$obs); // remplace les fins de ligne par du html
}
if (isset($_REQUEST['ordostruct'])) //modification d'ordonnance pour enregistrement
{
  $ordostruct=$_REQUEST['ordostruct'];
//echo $ordostruct;
  $obs_modifiee='<?xml version="1.0" encoding="ISO-8859-1" standalone="yes" ?>
<ordotext>
<html><head><meta name="qrichtext" content="charset=utf-8" /></head><body text="#000000"  style="font-size:10pt;font-family:MS Shell Dlg"><p>'.$obs_modifiee.'</p>
</body></html></ordotext>'.$ordostruct.'\0\0';
//echo $obs_modifiee;
}
//S'il manque au moins un argument, on est redirige automatiquement sur la page de recherche des patients
if (!$GUID AND !$observation_ID)
{
  header('location: liste.php' );
  exit;
}

$ecritureOK=2;
 /*
if ($observation_ID) //verification des droits d'ecriture
{
  if ($ID_possesseur==$user)
  {
    $sql_verif_droits="SELECT Droits FROM Personnes WHERE Login='$user'";
    $resultat_verif_droits=mysql_query($sql_verif_droits,$db);
    $ligne_verif_droits=mysql_fetch_array($resultat_verif_droits);
    $droits=$ligne_verif_droits['Droits'];
    if (preg_match("`obc`",$droits) )
    {
      $ecritureOK=1;
    }
    else
      $ecritureOK="";
  }
  else //droits delegues
  {
    $sql_utilisateur_autorise="SELECT * FROM user_perms WHERE FriendUserGUID='$user' AND SignataireGUID='$ID_possesseur'";
    $resultat_utilisateur_autorise=mysql_query($sql_utilisateur_autorise,$db);
    $count_utilisateur_autorise=mysql_num_rows($resultat_utilisateur_autorise);
    if ($count_utilisateur_autorise)//S'il existe un utilisateur autorise
    {
      $liste_utilisateur_autorise=mysql_fetch_array($resultat_utilisateur_autorise);
      if (preg_match("`obv`",$liste_utilisateur_autorise['FriendUserDroits']))//On verifie qu'il a les droits
	$ecritureOK=1;
      else
	$ecritureOK="";
    }
    else
      $ecritureOK="";
  }
}
*/
//mode formulaire de modification
$type_document_code='';
$type_document_clair['20020100']='Ordonnance structurée';
$type_document_clair['20080000']='Document';
$type_document_clair['20030000']='Observation';
$type_document_clair['20020200']='Prescription';
$type_document_clair['20020300']='Certificat';
$type_document_clair['20020500']='Courrier';
$type_document_clair['20090000']='Notes avancées';

if (isset ($_REQUEST['observation_ID'])) //bouton Modifier de la fiche du patient - remplissage du formulaire de saisie avec les valeurs existantes ou mode Dupliquer
{
  $sql_recup_obs="SELECT * FROM RubriquesBlobs INNER JOIN RubriquesHead ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesBlobs.RbDate_PrimKey='$observation_ID'";
  $resultat_recup_obs=mysql_query($sql_recup_obs,$db);
  $ligne_recup_obs=mysql_fetch_array($resultat_recup_obs);
  $titre=$ligne_recup_obs['RbDate_NomDate'];
  $GUID=$ligne_recup_obs['RbDate_IDDos'];
  $obs=$ligne_recup_obs['RbDate_DataRub'];
  $type_document_code=$ligne_recup_obs['RbDate_TypeRub'];
//Duplication de document. On trouve le contenu du document, on met la date courant, on enregistre et on retourne sur la page de journee avec la nouvelle date.
  if ($_REQUEST['nouveau']=='Dupliquer') 
  {
    $date_complete=date('Y-m-d H:i:s',date('U') );
    $sql_insert_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$GUID')",mysql_real_escape_string($obs));
    $resultat_insert_blob=mysql_query($sql_insert_blob,$db);
	    
  //on recupere l'ID du blob
    $id_blob=mysql_insert_id();
  //on insere l'en-tete
    $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$GUID','$type_document_code','%s','$date_complete','$user','$signuser','$id_blob')",mysql_real_escape_string($titre));
    $resultat_insert_header=mysql_query($sql_insert_header,$db);

    header ('location:consultation.php?numeroID='.$GUID.'&date='.substr($date_complete,0,10));
    exit;
  }
  if ($_REQUEST['nouveau']=='Confirmer la suppression') //On supprime definitivement le document
  {
    $sql_delete_blob="DELETE FROM RubriquesBlobs WHERE RbDate_PrimKey='$observation_ID'";
    $sql_delete_header="DELETE FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$observation_ID'";
    $resultat_delete_blob=mysql_query($sql_delete_blob);
    $resultat_delete_header=mysql_query($sql_delete_header);

//On retire le verrou sur la fiche
    $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$GUID'";
    $resultat_enlever_verrou=mysql_query($sql_enlever_verrou,$db);
    header ('location:consultation.php?numeroID='.$GUID.'&date='.date ('Y-m-d', date('U')));
    exit;
  }

  if ($type_document_code=='20020100') //ordonnance structuree : on separe le xml du html pour edition
  {
    $struct=preg_match('/<ordoMedicaStruct>(.*)<\/ordoMedicaStruct>/si', $obs, $tab_struct);
    $ordotext=preg_match('/<ordotext(.*)<\/ordotext>/si', $obs, $tab_text);
    $obs=$tab_text[0];
    $struct=$tab_struct[0];
  
 // if ($xml=simplexml_load_string($obs))                                                                                                                        
  }

  $date_longue=$ligne_recup_obs['RbDate_Date'];
  if ($date_format=='fr')
    $date=substr($date_longue,8,2)."-".substr($date_longue,5,2)."-".substr($date_longue,0,4);
  elseif ($date_format=='en')
    $date=substr($date_longue,5,2)."-".substr($date_longue,8,2)."-".substr($date_longue,0,4);
  else
    $date=substr($date_longue,0,9);
  $heure_observation_modifiee=substr($date_longue,11,8);

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
else //Initialisation des champs si nouvelle observation
{
  if ($date_format=='fr')
  {
    $date=date('d-m-Y');
  }
    elseif ($date_format=='en') //
  {
    $date=date('m-d-Y');
  }
  else
    $date=date('Y-m-d');
  $heure=date("H:i:s");
$sql_chercher_ville="SELECT Ville FROM Personnes WHERE Login='$signuser'";

$resultat_chercher_ville=mysql_query($sql_chercher_ville);
$ligne_chercher_ville=mysql_fetch_array($resultat_chercher_ville);
$ville=$ligne_chercher_ville['Ville'];
include("templates.php"); //certains templates seront remplis avec la date ou la ville
}
//On cherche dans la table IndexNomPrenom la fiche patient dont le nom a ete demande
$sql="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$GUID'";
$resultat=mysql_query($sql,$db);
$ligne=mysql_fetch_array($resultat);
$nom=$ligne["FchGnrl_NomDos"];
$prenom=$ligne["FchGnrl_Prenom"];
$primkey=$ligne["ID_PrimKey"];

include("templates.php");

if (isset ($_REQUEST['titre_indice'])) //si on a transmis un indice de titre
{
  $titre_indice=$_REQUEST['titre_indice'];
  $titre=$titre_default[$titre_indice];
  $fiche_mieux=$obs_default[$titre_indice];
}

if (isset ($_REQUEST['rubrique'])) //deroulant des rubriques ou ordonnance structuree
{
  $type=$_REQUEST['rubrique'];
/*  if ($type=="20080000") //documents
  {
    $obs_modifiee="<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>
    <MedinTux_Multimedia_data>
	<HTML_Data>
    <html><head><meta name=\"qrichtext\" content=\"charset=utf-8\" /></head><body style=\"font-size:10pt;font-family:Arial\">".$obs_modifiee."</body></html>
	</HTML_Data>
	<ImageListe>
	</ImageListe>
    </MedinTux_Multimedia_data>\n\0\0";
  }
  else*/
  if ($type!='20020100') // les ordonnances structurees sont traitees ailleurs
  {
    $obs_modifiee="<html><head><meta name=\"qrichtext\" content=\"charset=utf-8\" /></head><body text=\"#000000\"  style=\"font-size:10pt;font-family:MS Shell Dlg\"><p>".$obs_modifiee."</p>
</body></html>";
  }
  if ($date_format=='fr')
	  $date_complete=substr($date_modifiee,6,4)."-".substr($date_modifiee,3,2)."-".substr($date_modifiee,0,2).' '.$heure_observation_modifiee;
  elseif ($date_format=='en')
	  $date_complete=substr($date_modifiee,6,4)."-".substr($date_modifiee,3,2)."-".substr($date_modifiee,0,2).' '.$heure_observation_modifiee;  
  else 
	  $date_complete=$date_modifiee.' '.$heure_observation_modifiee;

  if ($_REQUEST['button_valider']=="Valider")//bouton enregistrer 
  {
  //requetes d'insertion
  //on insere le texte
    $sql_insert_blob=sprintf("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('%s','$GUID')",mysql_real_escape_string($obs_modifiee));
    $resultat_insert_blob=mysql_query($sql_insert_blob,$db);
	    
  //on recupere l'ID du blob
    $id_blob=mysql_insert_id();
  //on insere l'en-tete
    $sql_insert_header=sprintf("INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_PrimKey) VALUES ('$GUID','$type','%s','$date_complete','$user','$signuser','$id_blob')",mysql_real_escape_string($titre_observation));
    $resultat_insert_header=mysql_query($sql_insert_header,$db);
  }
  elseif ($_REQUEST['button_valider']=="Modifier")
  {
    $sql_blobs=sprintf("UPDATE RubriquesBlobs SET RbDate_DataRub='%s' WHERE RbDate_PrimKey='$observation_ID'",mysql_real_escape_string($obs_modifiee));
//echo  $sql_blobs;
    $sql_header=sprintf("UPDATE RubriquesHead SET RbDate_NomDate='%s',RbDate_Date='$date_complete', RbDate_TypeRub=$type WHERE RbDate_RefBlobs_PrimKey='$observation_ID'",mysql_real_escape_string($titre_observation));
    $resultat_blobs=mysql_query($sql_blobs,$db);
    $resultat_header=mysql_query($sql_header,$db);	
  }
    //On enleve le verrou a l'enregistrement		

  $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$GUID'";
  $resultat_enlever_verrou=mysql_query($sql_enlever_verrou,$db);

  header('location: consultation.php?numeroID='.$GUID.'&date='.substr($date_complete,0,10) );
  exit;
}

if (isset ($_REQUEST['bouton_annuler'])) //on enleve le verrou
{
  $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$GUID'";
  $resultat_enlever_verrou=mysql_query($sql_enlever_verrou,$db);

  header('location: consultation.php?numeroID='.$GUID.'&date='.substr($date_longue,0,10) );
  exit;
}

include("inc/header.php");
?>
    <title>
      Document de <?php echo $nom.' '.$prenom ?> - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <script type="text/javascript">
//<![CDATA[
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
//]]>
    </script>
<!-- 		<link rel="stylesheet" href="docs/style.css" type="text/css"> -->

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
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
<?php
include 'calendar_javascript.php';
?>
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" onload="donner_focus('titre_observation')" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Mode_observation';
include("inc/menu-horiz.php");
if ($_REQUEST['nouveau']=='Supprimer')
{
?>
      <form action="observation.php" method="get">
	<fieldset>
	<legend>
	  Suppression définitive
	</legend>
	Voulez-vous vraiment supprimer ce document ? <br />
	Il n'y aura aucun moyen de le récupérer. <br />
	<input name="nouveau" type="submit" value="Confirmer la suppression" />
	<input name="bouton_annuler" type="submit" value="Annuler" title="Ce document sera abandonné et le verrou sur la fiche retiré"/>
	<input name="observation_ID" type="hidden" value="<?php echo $observation_ID ?>" />
	</fieldset>
      </form>
<?php
}
if ($ecritureOK) //si droits d'ecriture
{
  //verification des verrous
  $sql_verifier_verrou="SELECT * FROM IndexNomPrenom INNER JOIN Verrous ON IndexNomPrenom.FchGnrl_IDDos=Verrous.DossGUID WHERE IndexNomPrenom.FchGnrl_IDDos='$GUID'";
  $resultat_verifier_verrou=mysql_query($sql_verifier_verrou,$db);
  $ligne_verifier_verrou=mysql_fetch_array($resultat_verifier_verrou);//message erreur
  $count_verifier_verrou=mysql_num_rows($resultat_verifier_verrou);
  if (isset ($_REQUEST['observation_ID'])) //bouton Modifier de la fiche du patient
  {
?>
  <div class="groupe">
      <h1>
	MedWebTux - Modification <?php echo $type_document_clair[$type_document_code]?> de <a href="patient.php?GUID=<?php echo $GUID ?>"><?php echo $ligne['FchGnrl_NomDos']." ".$ligne['FchGnrl_Prenom'] ?></a>
      </h1>
<?php
    $heure=$heure_observation_modifiee;
  }
  else //mode creation
  {
?>
  <div class="groupe">
      <h1>
	MedWebTux - Document de <a href="patient.php?GUID=<?php echo $GUID ?>"><?php echo $ligne['FchGnrl_NomDos']." ".$ligne['FchGnrl_Prenom'] ?></a>
      </h1>
<?php
  }
  if ($count_verifier_verrou) //Un verrou existe sur ce dossier. On ne fait rien et on afiche une alerte
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
	<ul>
	  <li>
	    Soit le programme a &eacute;t&eacute; arr&ecirc;t&eacute; brutalement sans avoir referm&eacute; le dossier, et vous pouvez continuer normalement
	  </li>
	  <li>
	    Soit vous avez ouvert la fiche en mode Modification et vous &ecirc;tes retourn&eacute; sur la page pr&eacute;c&eacute;dente par les fl&egrave;ches du navigateur et vous pouvez continuer normalement
	  </li>
<?php
	if ($ligne_verifier_verrou['UserName']!=$loginbase."@".$host) //C'est nous
	{
?>
	  <li>
	    Soit quelqu'un d'autre est en train de modifier ce dossier. Dans ce cas, c'est le dernier &agrave; enregistrer qui gagnera.
	  </li>
<?php
	}
	else
	{
?>
	  <li>
	    Soit un autre programme est en train de modifier ce dossier. Dans ce cas, c'est le dernier &agrave; enregistrer qui gagnera.
	  </li>
<?php
	}
?>
	</ul>
      </div>
<?php
  }
  else //On pose un verrou pour cause d'observation neuve ou existante
  {
    $datime=date('YmdHis', date('U'))."000";
    $mysql_user=$loginbase."@".$host;

    $sql_poser_verrou="INSERT INTO Verrous VALUES (NULL,'$datime','All Rubriques','0','$GUID','$mysql_user',NULL,'0')";//pose du verrou
    $resultat_poser_verrou=mysql_query($sql_poser_verrou,$db);
    $enlever_verrou=1;
  }
?>
      <div class="notice">
	<strong>Avertissement</strong> : les données saisies dans ce cadre ne sont pas exploitées pour le contrôle de sécurité des ordonnances médicamenteuses.
      </div>
      <form action="observation.php" method="post" id="form_observation">
	<table  style="width:auto;margin-left:auto;margin-right:auto;">
<!-- 	<table> -->
	  <tr>
	    <th>
	      <input name="GUID" type="hidden" value="<?php echo $GUID ?>" />
	      <label for="titre_observation">
		Titre :
	      </label>
	      <input name="titre_observation" id="titre_observation" type="text" value="<?php echo $titre ?>" size="30" maxlength="40"/>
	    </th>
	    <th>
	      <label for="date_modifiee">
		Date :
	      </label>
	    <input name="date_modifiee" id="date_modifiee" type="text" value="<?php echo $date?>" size="10" maxlength="10" />
	    <input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_observation'].date_modifiee);" /></th>
	    <th>
	      <label for="heure_observation_modifiee">
		Heure :
	      </label>
	      <input name="heure_observation_modifiee" id="heure_observation_modifiee" type="text" value="<?php echo $heure?>" size="10" maxlength="8"/>
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
//droits directs
    $sql_chercher_droits="SELECT * FROM Personnes WHERE Login='$user'";
    $resultat_chercher_droits=mysql_query($sql_chercher_droits);
    $ligne_chercher_droits=mysql_fetch_array($resultat_chercher_droits);
    //droits delegues
    $droits_delegues='';
    if ($user!=$signuser)
    {
	$sql_GUID_signuser="SELECT GUID FROM Personnes WHERE Login='$signuser'";
	$resultat_GUID_signuser=mysql_query($sql_GUID_signuser);
	$ligne_GUID_signuser=mysql_fetch_array($resultat_GUID_signuser);

	$sql_GUID_user="SELECT GUID FROM Personnes WHERE Login='$user'";
	$resultat_GUID_user=mysql_query($sql_GUID_user);
	$ligne_GUID_user=mysql_fetch_array($resultat_GUID_user);

	$GUID_user=$ligne_GUID_user['GUID'];
	$GUID_signuser=$ligne_GUID_signuser['GUID'];

	$sql_droits_delegues="SELECT FriendUserDroits FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_signuser'";
	$resultat_droits_delegues=mysql_query($sql_droits_delegues);
    //    $count_utilisateur_autorise=mysql_num_rows($resultat_utilisateur_autorise);
	$ligne_droits_delegues=mysql_fetch_array($resultat_droits_delegues);
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
		<option value="<?php echo $elts_rubrique[2] ?>" <?php if ($type_document_code==$elts_rubrique[2]) echo "selected=\"selected\"" ?> >
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
	      <input name="bouton_annuler" type="submit" value="Annuler" title="Ce document sera abandonné et le verrou sur la fiche retiré"/>
	    </th>
	  </tr>
	</table>
      </form>
<?php
}
else
{
  echo "
      <p style=\"text-align:center\">
	<br /><br /><strong>Vous n'avez pas les droits d'&eacute;criture sur cette observation.</strong><br />Elle appartient &agrave; $ID_possesseur
      </p>";
}
echo "</div>";

include("inc/footer.php");
?>
    </div>
    <p>
      <a href="http://validator.w3.org/check?uri=referer"><img
	  src="pics/valid-xhtml10.png" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
    </p>
  </body>
</html>
