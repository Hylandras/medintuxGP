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

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db).'<br />';
  mysqli_close($db);
  exit;
}
//on cherche les droits de l'utilisateur
$sql_chercher_droits="SELECT GUID,Droits,Qualite FROM Personnes WHERE Login='$user'";
$resultat_chercher_droits=mysqli_query($db,$sql_chercher_droits);
$ligne_chercher_droits=mysqli_fetch_array($resultat_chercher_droits);
$GUID_user=$ligne_chercher_droits['GUID'];
$droits_user=$ligne_chercher_droits['Droits'];
$qualite_user=$ligne_chercher_droits['Qualite'];

include("inc/header.php");

/*test explode
$string="--";
$table=explode("-",$string);
print_r($table);
fin test
*/
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
  if (count(explode ("-",$date))==3) //verifier si la date est bien formatee
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
  else
    return "invalid";
}

function detectUTF8($string) //pour les notes dont l'encodage est toujours imprévisible
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

//On cherche dans la table IndexNomPrenom la fiche dont le nom a ete demande
$sql="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
$resultat=mysqli_query($db,$sql);
$ligne=mysqli_fetch_array($resultat);
$nom=$ligne["FchGnrl_NomDos"];
$prenom=$ligne["FchGnrl_NomDos"];
$primkey=$ligne["ID_PrimKey"];
//On cherche dans la table fchpat la fiche dont le numero a ete demande
$sql2="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$patient'";

$resultat2=mysqli_query($db,$sql2);
$count2=mysqli_num_rows($resultat2);
$ligne2=mysqli_fetch_array($resultat2);
$naissance_full=$ligne2['FchPat_Nee'];
//verifier que date naissance valide
if (explode(' ',$naissance_full))
{
  $naissance_tableau=explode(' ',$naissance_full);
  $naissance=local_to_iso($naissance_tableau[0],$date_format);
}
else
  $naissance="invalid";
$adresse=$ligne2['FchPat_Adresse'].' '.$ligne2['FchPat_CP'].' '.$ligne2['FchPat_Ville'];
$secu=$ligne2['FchPat_NumSS'];
$profession=$ligne2['FchPat_Profession'];
$sexe='I';
if ($ligne2['FchPat_Sexe']=='M' OR $ligne2['FchPat_Sexe']=='F')
  $sexe=$ligne2['FchPat_Sexe'];
$genre['M']='';
$genre['I']='';
$genre['F']='e';
$titre=$ligne2['FchPat_Titre'];
$date='';

include("templates.php"); //ne pas inclure avant de renseigner nom et prenom

//On cherche les documents du patient
$resultat_global=mysqli_query($db,"SELECT * FROM RubriquesHead WHERE RbDate_IDDos='$patient' ORDER BY RbDate_Date DESC");
?>
    <title>
      <?php echo $nom.' '.$ligne["FchGnrl_Prenom"] ?> - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
<?php
include 'calendar_javascript.php';
?>
    <script type="text/javascript">
//<![CDATA[
function verif_champ(champ)
{
  if (champ == "")
  { 
    alert("Le champ n'est pas rempli");
    return false;
  }
  return true;
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
function verif_ordo(name,family_name,birth)
{
  if (name == "" || family_name == "" || birth == "")
  { 
    alert ("Les valeur Nom, prénom et date de naissance doivent être connues pour accéder à la rédaction d'ordonnances.");
    return false;
  }
  return true;
}
//]]>
    </script>

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
function showHidePannel(title)
{
// alert (title);
  document.getElementById('coordinates').style.display = 'none';
  document.getElementById('create').style.display = 'none';
  document.getElementById('complet').style.display = 'none';
  document.getElementById('schedule').style.display = 'none';
  document.getElementById('intervenant').style.display = 'none';
  document.getElementById('hono').style.display = 'none';
  document.getElementById('search').style.display = 'none';
  document.getElementById('tableau').style.display = 'none';
 //     document.getElementById('coordinates').style.display = 'inline';
  if (title==='coordinates')
  {
      document.getElementById('coordinates').style.display = 'block';
  }
  else if (title==='create')
  {
     document.getElementById('create').style.display = 'inline';
  }
  else if (title==='complet')
  {
//alert (title+'complet');
      document.getElementById('complet').style.display = 'inline';
  }
  else if (title==='schedule')
  {
      document.getElementById('schedule').style.display = 'inline';
  }
  else if (title==='intervenant')
  {
      document.getElementById('intervenant').style.display = 'inline';
  }
  else if (title==='hono')
  {
      document.getElementById('hono').style.display = 'inline';
  }
  else if (title==='tableau')
  {
      document.getElementById('tableau').style.display = 'inline';
  }
  else if (title==='search')
  {
      document.getElementById('search').style.display = 'inline';
  }
}
//]]>
    </script>
<!-- 	<script src='scripts/jquery/jquery.min.js'></script> -->
<!-- 	<script type='text/javascript' src='scripts/jquery/menu_jquery.js'></script> -->
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='Mode_patient';
//include("inc/menu-horiz.php");
		
$message['message_pas_admin_pour_export']="<strong>Vous devez avoir les droits administrateur pour exporter un dossier.</strong><br />";

?>
<div class="taille">
  <div class="groupe">
      <h1>
	Fiche patient de <?php echo stripslashes($nom) ?>
      </h1>

<?php
//les dates de recherche selon la langue
$debut=iso_to_local(date('Y-m-d', date('U')),$date_format); //pour les rdv
list ($jour_debut,$mois_debut,$annee_debut)=explode ("-",date('d-m-Y', date('U')));

$directory=explode("/",getenv("REQUEST_URI"));
$message['message_pas_dir_pour_export']="
      <p>
	L'exportation est impossible, car elle nécessite un droit en écriture pour l'utilisateur apache (parfois nommé www-data) dans le répertoire ".getenv("DOCUMENT_ROOT")."/".$directory[1]."/log/
      </p>";
if (isset($_REQUEST['message']))
{
  $ce_message=$_REQUEST['message'];
  echo $message[$ce_message];
}

if (isset($_REQUEST['export']))
{
?>
      <p>
	<a href="<?php echo "log/patient".$patient.".xml" ?>">Enregistrez ce lien pour sauvegarder la fiche sur votre disque. </a>
      </p>
<?php
}
// on verifie les verrous
if (isset($_GET['enlever_verrou']))
{
  $enlever_verrou=$_GET['enlever_verrou'];
  if ($enlever_verrou)
  {
    $sql_enlever_verrou="DELETE FROM Verrous WHERE DossGUID='$patient'";//on enleve le verrou
    $resultat_enlever_verrou=mysqli_query($db,$sql_enlever_verrou);
  }
}
$sql_verrous="SELECT * FROM Verrous WHERE DossGUID='$patient'";

$resultat_verrous=mysqli_query($db,$sql_verrous);
$count_verrous=mysqli_num_rows($resultat_verrous);
/*
if ($count_verrous)
{
  echo "
      <div class='notice'>
	<strong>Cette fiche est déjà en utilisation</strong>
      </div>";
}
*/
//On cherche les notes
$sql4="SELECT * FROM fchpat_Note WHERE fchpat_Note_PatPK='$primkey'";
$resultat4=mysqli_query($db,$sql4);
$count4=mysqli_num_rows($resultat4);
$ligne4=mysqli_fetch_array($resultat4);

//On cherche les correspondants
$sql5="SELECT * FROM fchpat_Intervenants INNER JOIN Personnes ON fchpat_Intervenants.fchpat_Intervenants_IntervPK=Personnes.ID_PrimKey WHERE fchpat_Intervenants_PatGUID='$patient'";
$resultat5=mysqli_query($db,$sql5);
$count5=mysqli_num_rows($resultat5);

//On cherche la date de  naissance pour modification de fiche
$date_naissance_iso=substr($ligne2["FchPat_Nee"],0,10);

$naissance=iso_to_local($date_naissance_iso,$date_format);

//On accorde la phrase selon le genre
if ($ligne2["FchPat_Sexe"]=="M")
{
  $ne="n&eacute;";
}
else 
{
  $ne="n&eacute;e";
}

/*Code pour mettre en journal tous les acces aux dossiers faits par un utilisateur particulier*/


if ($user=="mayer") //Mettre ici le login utilisateur - supprimer cette condition pour que les acces de tous les utilisateurs soient consignes.
{
	if (!$pointeur=fopen("log/mayer.txt",'a')) //necessite l'existence d'un dossier "log", avec les droits d'ecriture pour l'utilisateur apache (souvent www-data)
		echo "ouverture impossible<br />";
	else
	{
		$ajout="Patient vu par ".$user." : ".$nom." ".$ligne["FchGnrl_Prenom"]." le ".date('d-m-Y H:i', date('U'))."\n";
		$nb=fwrite ($pointeur,$ajout );
		$pointeur=fclose($pointeur);
	}
}

//affichage de l'identite du patient
?>
<div class="tableau">
<!-- table generale -->
      <table width="100%" style="align:left">
	<tr>
	  <td>

	<div id='cssmenu'>
	<ul>
		<li><a href="#" onclick="showHidePannel('coordinates'); return false"><span>Identité</span></a></li>
		<li><a href="#" onclick="showHidePannel('create'); return false"><span>Nouveau document</span></a></li>
		<li><a href="#" onclick="showHidePannel('complet'); return false"><span>Dossier complet</span></a></li>
		<li><a href="#" onclick="showHidePannel('schedule'); return false"><span>Rendez-vous</span></a></li>
		<li><a href="#" onclick="showHidePannel('intervenant'); return false"><span>Intervenants</span></a></li>
		<li><a href="#" onclick="showHidePannel('hono'); return false" >Honoraire</a></li>
         	<li><a href="#" onclick="showHidePannel('tableau'); return false"><span>Terrains</span></a></li>
         	<li><a href="#" onclick="showHidePannel('search'); return false"><span>Recherche</span></a></li>
	</ul>
	</div>
<!--
	    <a href="#" onclick="showHidePannel('create'); return false" >Nouveau document</a><br />
	    <a href="#" onclick="showHidePannel('complet'); return false" >Dossier complet</a><br />
	    <a href="#" onclick="showHidePannel('schedule'); return false" >Rendez-vous</a><br />
	    <a href="#" onclick="showHidePannel('intervenant'); return false" >Intervenants</a><br />
	    <a href="#" onclick="showHidePannel('hono'); return false" >Honoraire</a><br />
	    <a href="#" onclick="showHidePannel('tableau'); return false" >Terrains</a><br />
	    <a href="#" onclick="showHidePannel('search'); return false" >Recherche</a><br />
-->
	      <div id='coordinates' >		
		<fieldset class="fieldset_patient" >
		  <legend>
		    Coordonn&eacute;es
		  </legend>

<?php
//Affichage de la fiche de base
$qrcode_string="BEGIN:VCARD\nVERSION:2.1\n";
$lignes_adresse=explode("\n",$ligne2["FchPat_Adresse"]);

echo "
		  ".stripslashes($ligne2["FchPat_Titre"])." ".stripslashes($nom)." ".stripslashes($ligne["FchGnrl_Prenom"])."<br />".$ne." le ".$naissance;
$qrcode_string.='N;CHARSET=UTF-8:'.stripslashes($nom).';'.stripslashes($ligne["FchGnrl_Prenom"]).';;'.stripslashes($ligne2["FchPat_Titre"])."\nFN;CHARSET=UTF-8:".stripslashes($ligne["FchGnrl_Prenom"]).' '.stripslashes($nom)."\n";

//calcul de l'age

$day_age  = $jour_debut - substr($ligne2["FchPat_Nee"],8,2);
$mon_age = $mois_debut - substr($ligne2["FchPat_Nee"],5,2);
$year_age = $annee_debut - substr($ligne2["FchPat_Nee"],0,4);

if ($day_age < 0)
{
  $mon_age=$mon_age - 1;
}
if ($mon_age < 0)
{
  $year_age = $year_age - 1;
  $mon_age = 12 + $mon_age;
}
//finalisation de l'affichage
if ($year_age) //on n'affiche les ans que si >= 1 an
{
  if ($year_age==1)//pour la proprete de l'affichage
    $ans="an";
  else
    $ans="ans";
  $afficher_annees=$year_age." ".$ans;
}
else
  $afficher_annees="";

if ($mon_age)
  $afficher_mois=$mon_age." mois";
else
  $afficher_mois="";

if ($mon_age < 6 and !$year_age) //affichage des jours si moins de 6 mois
{
  $first_date = mktime(12,0,0,substr($ligne2["FchPat_Nee"],5,2),substr($ligne2["FchPat_Nee"],8,2),substr($ligne2["FchPat_Nee"],0,4)); 
  $second_date = mktime(12,0,0,$mois_debut,$jour_debut,$annee_debut); 

  $offset = $second_date-$first_date; //difference en secondes entre les dates
  $nombre_semaines = floor($offset/60/60/24/7);

  $nombre_jours=floor($offset/60/60/24);
  $difference_jours=$nombre_jours-7*$nombre_semaines;
  if ($difference_jours==1) //pour la proprete de l'affichage
    $jours=" jour";
  else
    $jours=" jours";
  if ($nombre_semaines) //au moins une semaine
  {
    if ($nombre_semaines==1)//pour la proprete de l'affichage
      $semaines=" semaine ";
    else
      $semaines=" semaines ";
    if ($difference_jours) //semaines + jours
    {
      $afficher_jours=$nombre_semaines.$semaines.$difference_jours.$jours;
    }
    else //nombre exact de semaines - pas besoin de mettre les jours puisque zero
      $afficher_jours=$nombre_semaines.$semaines;
  }
  else //moins d'une semaine
    $afficher_jours=$difference_jours.$jours;
}
else
  $afficher_jours="";
//affichage de l'age
if ($mon_age < 6 and $mon_age >=1 and $year_age < 1) //entre 1 et 6 mois
  echo " (".$afficher_mois.", soit ".$afficher_jours.")<br />";
elseif ($mon_age < 1 and $year_age < 1) //moins d'1 mois
  echo " (".$afficher_jours.")<br />";
else //plus de 6 mois
  echo " (".$afficher_annees." ".$afficher_mois.")<br />";
//Affichage du nom de jeune fille si possible
if ($ligne2["FchPat_NomFille"])
{
  echo "
		  <strong>Nom de jeune fille</strong> : ".stripslashes($ligne2["FchPat_NomFille"]).'<br />';
}

foreach ($lignes_adresse as $ligne_adresse)
{
  echo "
		  ".stripslashes($ligne_adresse)."<br />";
}
echo $ligne2["FchPat_CP"]." ".$ligne2["FchPat_Ville"].'<br />';
//on met la premiere ligne d'adresse en qrcode
$qrcode_string.='ADR;WORK;CHARSET=UTF-8:;'.$lignes_adresse[0].';'.$ligne2["FchPat_Ville"].';;'.$ligne2["FchPat_CP"]."\n";
//Affichage des numeros de telephone si possible
if ($ligne2["FchPat_Tel1"])
{
  echo "
	      <strong>T&eacute;l&eacute;phone 1</strong> : ".$ligne2["FchPat_Tel1"].'<br />';
  $qrcode_string.='TEL;WORK:'.$ligne2["FchPat_Tel1"]."\n";
}
if ($ligne2["FchPat_Tel2"])
{
  echo "
	      <strong>T&eacute;l&eacute;phone 2</strong> : ".$ligne2["FchPat_Tel2"].'<br />';
}
if ($ligne2["FchPat_Tel3"])
{
  echo "
	      <strong>T&eacute;l&eacute;phone 3</strong> : ".$ligne2["FchPat_Tel3"].'<br />';
}
//Affichage du mail si possible
if ($ligne2["FchPat_Email"])
{
	echo "
	      <strong>e-mail</strong> : <a href=\"mailto:".$ligne2["FchPat_Email"]."\">".$ligne2["FchPat_Email"]."</a><br />";
  $qrcode_string.='EMAIL;PREF;INTERNET:'.$ligne2["FchPat_Email"]."\n";
}
//Affichage du num de secu si possible
if ($ligne2["FchPat_NumSS"])
{
	echo "
	      <strong>Num&eacute;ro de s&eacute;cu</strong> : ".substr($ligne2["FchPat_NumSS"],0,1)." ".substr($ligne2["FchPat_NumSS"],1,2)." ".substr($ligne2["FchPat_NumSS"],3,2)." ".substr($ligne2["FchPat_NumSS"],5,2)." ".substr($ligne2["FchPat_NumSS"],7,3)." ".substr($ligne2["FchPat_NumSS"],10,3)." ".substr($ligne2["FchPat_NumSS"],13,2).'<br />';
//Affichage de la profession si possible
}
if ($ligne2["FchPat_PatientAss"]==0)
{
  echo '
	      <strong>Assuré : </strong><a href="liste.php?critere_recherche=FchGnrl_NomDos&amp;cle='.$ligne2["FchPat_NomAss"].'&amp;envoyer=Chercher">'.$ligne2["FchPat_NomAss"].' '.$ligne2["FchPat_PrenomAss"].'</a><br />';
}
if ($ligne2["FchPat_Profession"])
{
  echo "
	      <strong>Profession</strong> : ".$ligne2["FchPat_Profession"].'<br />';
  $qrcode_string.="TITLE;CHARSET=UTF-8:".$ligne2["FchPat_Profession"]."\n";
}
//Affichage des notes si possible

if ($ligne4["fchpat_Note_Html"])
{
//Recherche des notes pour ce patient
//on verifie si les notes sont en utf-8

  if (detectUTF8($ligne4["fchpat_Note_Html"]))
    $Notes=stripslashes($ligne4["fchpat_Note_Html"]);
  else
    $Notes=stripslashes(utf8_encode($ligne4["fchpat_Note_Html"]));
  echo "
		    <strong>Notes </strong>:
		    <ul>
		      <li>
			".preg_replace("`\n`","
		      </li>
		      <li>
			",$Notes)."
		      </li>
		    </ul>";
  $qrcode_string.='NOTE;CHARSET=UTF-8:'.$ligne4["fchpat_Note_Html"]."\n";
}
$qrcode_string.='END:VCARD';
$qrcode_string=urlencode($qrcode_string);
?>
		  <img src="phpqrcodeimg.php?qrcode_string=<?php echo $qrcode_string;?>" alt="qrcode" width="125" title="Scannez cette image de QR-Code avec votre Smartphone pour ajouter cette fiche à vos contacts" />

<!--ID=13306&Nee=2000-03-08-->
		  
		<form action="nouveau_dossier.php" method="get" target="_top" >
		   <div>
		      <input name="ID" type="hidden" value="<?php echo $primkey ?>" />
		      <input name="Nee" type="hidden" value="<?php echo $naissance ?>" />
		      <input name="envoyer" type="submit" value="Modifier" />
		      <input name="envoyer" type="submit" value="Dupliquer" />
		   </div>
		 </form>
		</fieldset>
</div>

</td>
</tr>
<tr>
		<td style="vertical-align:top">

<div id="create" style="display:none">
		  <form action="consultation.php" method="get" target="droit_bas">
		    <fieldset class="fieldset_patient">
		      <legend>
			Création de document
		      </legend>

		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		      <input name="nouveau" type="hidden" value="Ajouter" />
		      <input name="date" type="hidden" value="<?php echo local_to_iso($debut,$date_format) ?>" />
		      <select name="titre_indice" onchange="submit()">
<?php
foreach ($titre_default as $indice_titre => $ce_titre) //on recherche toutes les maquettes de documents
{ //on selectionne par defaut celle qui porte en indice le nom de la specialite si elle existe
?>
			  <option value="<?php echo $indice_titre;?>"<?php if ($indice_titre==$qualite_user) echo " selected='selected'" ?> >
			    <?php echo $ce_titre ?>
			  </option>
<?php
}
?>
		      </select><br />
		      <input name="nouveau" type="submit" value="Ajouter" />
		    </fieldset>
		  </form>
		  <form action="interactions.php" method="get" onsubmit="verif_ordo('<?php echo $prenom?>', '<?php echo $nom ?>', '<?php echo $naissance_full ?>')"  target="droit_bas">
		    <fieldset class="fieldset_patient">
		      <legend>
			Ordonnances
		      </legend>
		      <input name="submit_new_prescription" type="submit" value="Ordonnance médicamenteuse" />
		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		    </fieldset>
		  </form>
    <!-- Selecteur de fichier image pour creation de document -->
		  <form action="import_image.php" id="form_image" method="post" enctype="multipart/form-data"  target="droit_bas">
		    <fieldset class="fieldset_patient">
		      <legend>
			Insertion d'image
		      </legend>
		      <input name="guid" type="hidden" value="<?php echo $patient ?>" />
		      <input type="hidden" name="MAX_FILE_SIZE" value="300000000" />
		      <input type="file" name="selection" id="selection" size="13" onchange="showButton('button_place')" /><br />
<?php
  $date_image=iso_to_local(date('Y-m-d', date('U')),$date_format);
?>
		      <div id="button_place" style="display:none"> 
			<label for="datepicker">Date&nbsp;:</label>
			<input type="text" name="date" id="datepicker" value="<?php echo $date_image ?>" size="13" /><br />
<!-- 			
<input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_image'].date_image);" /><br /> 
-->
			Titre&nbsp;:<input type="text" name="title" value="Image" size="13" /><br />
			<input name="image" type="submit" value="Ajouter" title="Importer un fichier image" />
		      </div>
		    </fieldset>
		  </form>
</div>
		</td>
</tr>
<tr>
		<td  style="vertical-align:top">
    <!-- le dossier complet	 -->

		<div id="complet" style="display:none">
		  <form action="dossier_complet.php" method="get" id="form_coches" target="droit_bas">
		    <fieldset class="fieldset_patient">
		      <legend>
			Page unique
		      </legend>
<?php
$rubriques_presentes=array ();
mysqli_data_seek($resultat_global,0) ; //remise a zero tableau requete

while ($ligne_global=mysqli_fetch_array($resultat_global))
{
  if (!in_array ($ligne_global['RbDate_TypeRub'],$rubriques_presentes))
  {
    $rubriques_presentes[]=$ligne_global['RbDate_TypeRub'];
  }
}

//coches de recherche pour le dossier complet ou l'exportation
if (isset ($rubrique_perso))
{
  $compteur_types=0;
  foreach ($rubrique_perso AS $this_rubrique_perso)
  {
    $tableau_rubrique_perso=explode('|',$this_rubrique_perso);
    if (in_array($tableau_rubrique_perso[2],$rubriques_presentes))
    {
?>
			<input name="type_info[]" id="x<?php echo $tableau_rubrique_perso[2]; ?>" type="checkbox" value="<?php echo $tableau_rubrique_perso[2]; ?>"  checked="checked" />
			<label for="x<?php echo $tableau_rubrique_perso[2]; ?>">
			  <?php echo $tableau_rubrique_perso[0]; ?>
			</label><br />
<?php
    }
    $compteur_types++;
  }
}
?>
			<input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
			<input name="button_invert_selection" onclick="Invers()" type="button" value="Inverser la sélection" />
			<input name="dossier_complet" type="submit" value="Afficher" title="Affiche le contenu de tous les documents du dossier"/>
		      </fieldset>
		    </form>
		    <fieldset class="fieldset_patient">
		      <legend>
			Tableaux
		      </legend>
		      <form action="consultation.php" method="get" target="droit_bas">
			<div>
			  <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
			  <input name="date" type="hidden" value="<?php echo local_to_iso($debut,$date_format) ?>" />
			  <input name="edition" type="submit" value="Journées" />
			</div>
		      </form>
		      <form action="tableaux.php" method="get" target="droit_bas">
			<div>
			  <input name="GUID" type="hidden" value="<?php echo $patient ?>" />
			  <input type="submit" name="affichage" value="listes" /><br />
			  <input type="submit" name="affichage" value="tableau" /><br />
			  <input type="submit" name="affichage" value="consultations" /><br />
			</div>
		      </form>
		    </fieldset>
		  <form action="synthese_ordo.php" method="get" onsubmit="verif_ordo('<?php echo $prenom?>', '<?php echo $nom ?>', '<?php echo $naissance_full ?>')"  target="droit_bas">
		    <fieldset class="fieldset_patient">
		      <legend>
			Ordonnances
		      </legend>
		      <input name="bouton_fusionner_ordonnances" type="submit" value="Historique des traitements" />
		      <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
		    </fieldset>
		  </form>
		      <form action="export.php" method="get">
			<fieldset class="fieldset_patient">
			  <legend>
			    Exportation
			  </legend>
			  <input name="numeroID" type="hidden" value="<?php echo $patient ?>" />
			  <input name="export" type="submit" value="Exporter" title="Exporte le contenu de tout le dossier"/>
			</fieldset>
		      </form>
		    </div>
		  </td>
		</tr>
		<tr>
		  <td>
		  <div id="schedule" style="display:none">
		  <form action="agenda.php" method="get" id="form_recherche_date" target="_top"><!--Bouton de prise de rendez-vous -->
		    <fieldset class="fieldset_patient">
		      <legend>
			Nouveau rendez-vous
		      </legend>
		      <input name="GUID" type="hidden" value="<?php echo $patient ?>" />
		      <input name="from_page" type="hidden" value="patient" />
		      <input name="Nom" type="hidden" value="<?php echo $nom ?>" />
		      <input name="nom" type="hidden" value="%" />
		      <input name="Prenom" type="hidden" value="<?php echo $ligne["FchGnrl_Prenom"]?>" />
		      <input name="Tel" type="hidden" value="<?php echo $ligne2["FchPat_Tel1"]?>" />
		      <input name="jour" id="datepickeur" type="text" value="<?php echo $debut ?>" size="10" maxlength="10" />
<!--
		      <input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_recherche_date'].jour);" />
-->
<br />
		      <input name="bouton_envoyer" type="submit" value="Rendez-vous" />
		    </fieldset>
      <!--Saisie de la date de rdv -->
		  </form>
		  <form action="agenda_perso.php" target="droit_bas">
		    <fieldset class="fieldset_patient">
		      <legend>
			Rendez-vous déjà pris
		      </legend>
		      <input type="submit" name="button_history" value="Historique" />
		      <input type="hidden" name="GUID" value="<?php echo $patient?>" />
		    </fieldset>
		  </form>
</div>
		</td>
</tr>
<tr>
		<td>
<!--Formulaire general -->
	      <div id='intervenant' style="display:none" >	
		<fieldset class="fieldset_patient">
		  <legend>Intervenants</legend>
<?php
if ($count5) //affichage des intervenants
{
  echo "
	     <table>
<!-- Tableau de 3eme niveau intervenants-->"; 
  while ($ligne5=mysqli_fetch_array($resultat5))
  {	
?>
		    <tr>
		      <td>
			<a href="fiche_intervenant.php?intervenant=<?php echo $ligne5["fchpat_Intervenants_IntervPK"] ?>" target="_top" >
			  <?php echo stripslashes($ligne5["Nom"])?>
			</a> <?php echo stripslashes($ligne5["Prenom"]).", ".stripslashes($ligne5["Qualite"]); ?>
		      </td>
		      <td>
			<form action="liaison_correspondant.php" method="get" target="droit_bas"><!--BOUTON DE SUPPRESSION DE LIAISON -->
			<div>
			    <input name="from" type="hidden" value="patient" />
			    <input name="ID_corresp" type="hidden" value="<?php echo $ligne5['fchpat_Intervenants_IntervPK'] ?>" />
			    <input name="patient_ID" type="hidden" value="<?php echo $ligne['FchGnrl_IDDos'] ?>" />
			    <input name="ID_fiche_liaison" type="hidden" value="<?php echo $ligne5['fchpat_Intervenants_PK'] ?>" />
			    <input name="envoyer" type="submit" value="Annuler le lien" />  
			</div> 
			</form>
		      </td>
		    </tr>
<?php
  }
?>
		  </table> 
<!--formulaire des intervenants-->
<?php
}
?>
		<form action="liaison_correspondant.php" method="get" target="droit_bas">
		  <div>		
		    <input name="envoyer" type="hidden" value="patient" />
		    <input name="GUID" type="hidden" value="<?php echo $patient ?>" />
		    <input name="nouveau" type="submit" value="Cr&eacute;er un nouveau lien" />
		  </div>
		</form>
		</fieldset>
	      </div>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      <div id='hono' style="display:none">	
		<fieldset class="fieldset_patient">
		   <legend>Honoraire</legend>
		  <form action="compta/saisie_hono.php" method="get" target="_top">
		    <div>
		      <input name="guid_patient" type="hidden" value="<?php echo $patient ?>" />
		      <input name="bouton_ajouter_hono" type="submit" value="Ajouter"  />
		    </div>  
		  </form>
		</fieldset>
	      </div>
	    </td>
	  </tr>
	<tr>
	  <td> 
	    <div id='tableau' style="display:none">	
	    <form action="tableaux.php" method="get" target="droit_bas">
	      <fieldset class="fieldset_patient">
		<legend>
		  Terrains
		</legend>
		<input name="GUID" type="hidden" value="<?php echo $patient ?>" />
		<input type="submit" name="affichage" value="terrains" />
	      </fieldset>
	    </form>
	  </div>
	</td>
      </tr>
      <tr>
	<td>
	  <div id='search' style="display:none">
	    <form action="tableaux.php" method="get" onsubmit="return verif_champ(this.cle.value);" target="droit_bas">
	      <fieldset class="fieldset_patient">
		<legend>
		  Recherche dans les documents
		</legend>
		<input name="GUID" type="hidden" value="<?php echo $patient ?>" />
		<label for="cle">
		  Chaîne
		</label>
		<input name="cle" id="cle" type="text" value="" /><br />
		<label for="critere_recherche">
		  Filtre
		</label>
		<select name="critere_recherche" id="critere_recherche"><!--Liste des cles de recherche-->
<?php
  foreach ($rubrique_perso AS $this_rubrique_perso) {
    $tableau_rubrique_perso=explode('|',$this_rubrique_perso);
    	if (in_array($tableau_rubrique_perso[2],$rubriques_presentes)) {
?>
		  <option value="<?php echo $tableau_rubrique_perso[2]; ?>" >		     
		    <?php echo $tableau_rubrique_perso[0]; ?>
		  </option>
<?php
    	}
  }
?>
		</select><br />
		<input name="envoyer_cle" type="submit" value="Chercher" />
	      </fieldset>
	    </form>
</div>
	   </td>
	  </tr>
	</table>
<a name="tableaux"> </a> 
    </div>
  </div>

</div>
<?php
include("inc/footer.php");
?>

