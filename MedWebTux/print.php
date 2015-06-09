<?php
session_start() ;
/*
Page destinee a creer une page sans accessoires HTML afin d'imprimer un document quelconque (documents, observation, etc.)
Accessible depuis les documents affiches en mode journee (consultation.php)
*/
include("config.php");
include("inc/pi_barcode.php");

if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=liste' );
  exit;
}

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$base))
{
  echo "
    <link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />
    <div class=\"erreur\">
      <b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
      Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
      <a href=\"setup/index.php\">Renseigner les donn&eacute;es de connexion au serveur Mysql</a>
    </div>";
	@rename("set_up", "setup");
	exit;
}

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
include("inc/header.php");
?>
    <title>
      MedWebTux - Impression de document par <?php echo $_SESSION['login'];  ?>
    </title>
  <script type="text/javascript">
//<![CDATA[
function stripslashes (str) {
  // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   improved by: Ates Goral (http://magnetiq.com)
  // +      fixed by: Mick@el
  // +   improved by: marrtins
  // +   bugfixed by: Onno Marsman
  // +   improved by: rezna
  // +   input by: Rick Waldron
  // +   reimplemented by: Brett Zamir (http://brett-zamir.me)
  // +   input by: Brant Messenger (http://www.brantmessenger.com/)
  // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
  // *     example 1: stripslashes('Kevin\'s code');
  // *     returns 1: "Kevin's code"
  // *     example 2: stripslashes('Kevin\\\'s code');
  // *     returns 2: "Kevin\'s code"
  return (str + '').replace(/\\(.?)/g, function (s, n1) {
    switch (n1) {
    case '\\':
      return '\\';
    case '0':
      return '\u0000';
    case '':
      return '';
    default:
      return n1;
    }
  });
}
//]]>
  </script>

  <script type="text/javascript">
//<![CDATA[
function ajouter_destinataire(titre,nom,prenom,adresse,codepostal,ville) //pour remplir la zone de destinataire avec une adresse choisie dans la liste radio
{
nom=stripslashes(nom);
prenom=stripslashes(prenom);
adresse=stripslashes(adresse);
titre=stripslashes(titre);
ville=stripslashes(ville);
 document.getElementById('cadre_destinataire').innerHTML =titre+" "+nom+" "+prenom+"<br />"+adresse+"<br />"+codepostal+" "+ville;
}
//]]>
  </script>
  <script type="text/javascript">
//<![CDATA[
function fill_radio() //pour remplir le bouton radio avec la zone de texte libre
{
  text=document.getElementById('free_text').value;
  document.getElementById('radio_free_text').value =text;
  ajouter_destinataire(text,'','','','','');
// document.getElementById('radio_free_text').checked =true; //coche bien le bouton, mais ne lance pas la fonction
}
//]]>
  </script>

  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" onload="javascript:window.print()" >
<?php
$anchor="Finalisati";
//include("inc/menu-horiz.php");
$username=$loginbase."@".$host;

$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$utilisateur_autorisant=$tab_login[1];

$sql="SELECT * FROM Personnes WHERE Login ='$utilisateur_autorisant'";
$resultat=mysqli_query($db,$sql);
$ligne=mysqli_fetch_array($resultat);
//formulaire pour ajouter un destinataire
if (isset($_REQUEST['id_document']))
{
  $id_document=$_REQUEST['id_document'];
  $sql_document="SELECT * FROM RubriquesHead INNER JOIN RubriquesBlobs ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RbDate_RefBlobs_PrimKey='$id_document'";
  $resultat_document=mysqli_query($db,$sql_document);
  $ligne_document=mysqli_fetch_array($resultat_document);

  $patient=$ligne_document['RbDate_IDDos'];
}

$sql_destinataire="SELECT * FROM fchpat_Intervenants INNER JOIN Personnes ON fchpat_Intervenants.fchpat_Intervenants_IntervPK=Personnes.ID_PrimKey WHERE fchpat_Intervenants_PatGUID='$patient'";
$resultat_destinataire=mysqli_query($db,$sql_destinataire);

//Chercher les coordonnees du patient
//On cherche dans la table IndexNomPrenom la fiche dont l'ID a ete demande
$sql_chercher_nom="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
$resultat_chercher_nom=mysqli_query($db,$sql_chercher_nom);
$ligne_chercher_nom=mysqli_fetch_array($resultat_chercher_nom);
$nom=$ligne_chercher_nom["FchGnrl_NomDos"];
$prenom=$ligne_chercher_nom["FchGnrl_Prenom"];

$sql_patient="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$patient'";
$resultat_patient=mysqli_query($db,$sql_patient);
$ligne_patient=mysqli_fetch_array($resultat_patient);
$titre=stripslashes($ligne_patient['FchPat_Titre']);
$adresse=$ligne_patient['FchPat_Adresse'];
$codepostal=$ligne_patient['FchPat_CP'];
$ville=$ligne_patient['FchPat_Ville'];
$find=array("\n");
$replace=array("<br />");

?>
    <div class='noPrint' style="position:absolute; left:130mm; top:35mm;">
      <fieldset>
	<legend>
	  Destinataire
	</legend>
	<input type="radio" name="destinataire" id="radio_free_text" value="" onchange="ajouter_destinataire(this.value,'','','','','')"/>
	<input type="text" name="destinataire" id="free_text" value="" onkeyup="fill_radio()"/><br />
	<input type="radio" name="destinataire" id="hismelf" value="<?php echo stripslashes($nom) ?>" onchange="ajouter_destinataire('<?php echo addslashes($titre) ?>','<?php echo addslashes($nom) ?>','<?php echo addslashes($prenom) ?>','<?php echo addslashes(str_replace($find,$replace,$adresse)) ?>','<?php echo $codepostal ?>','<?php echo addslashes($ville) ?>')"/><?php echo stripslashes($nom) ?><br />
<?php
$i=0;
while ($ligne_destinataire=mysqli_fetch_array($resultat_destinataire))
{
$i++;
?>
	<input type="radio" name="destinataire" id="praticien_<?php echo $patient ?>_<?php echo $i ?>" value="un medecin" onchange="ajouter_destinataire('<?php  echo addslashes($ligne_destinataire['Titre']) ?>','<?php echo addslashes($ligne_destinataire['Nom']) ?>','<?php  echo addslashes($ligne_destinataire['Prenom']) ?>','<?php  echo addslashes(str_replace($find,$replace,$ligne_destinataire['Adresse'])) ?>','<?php  echo $ligne_destinataire['CodePostal'] ?>','<?php  echo addslashes($ligne_destinataire['Ville']) ?>')"/><?php  echo stripslashes($ligne_destinataire['Nom']) ?><br />
<?php
}
?>
      </fieldset>
    </div>

    <div id="cadre_destinataire" style="position:absolute; left:100mm; top:35mm;"></div>
<?php
function entete($ligne)
{
echo '
      <p style="display: block;
		position: fixed;
		top: 10mm;
		margin-left: 5mm;">';
  //Affichage de l'en-tete
  if ($_GET['radio_header']=="radio_header")
  {
    echo "
	<strong>".stripslashes($ligne["Titre"])." ".stripslashes($ligne["Nom"])." ".stripslashes($ligne["Prenom"])."</strong><br />";
    echo '
	'.stripslashes($ligne["Adresse"])."<br />";
    echo '
	'.$ligne["CodePostal"]." ".$ligne["Ville"]."<br />";
    echo '
	'.$ligne["Qualite"]."<br />";
    if ($ligne["Convention"])
      echo 
	$ligne["Convention"]."<br />";
    if ($ligne["Tel_1"])
      echo "
	<strong>T&eacute;l&eacute;phone</strong> : " .$ligne["Tel_1"]."<br />";
  // Decommenter si on veut afficher tous les tel dans l'en-tete
    if ($ligne["Tel_2"])
    {
      if ($ligne["Tel_Type2"]=="Fax")
	echo "<strong>Fax</strong> : " .$ligne["Tel_2"]."<br />";
    }
    if ($ligne["Tel_3"])
    {
      if ($ligne["Tel_Type3"]=="Fax")
      echo "<strong>Fax</strong> : " .$ligne["Tel_3"]."<br />";
    } 
    if ($ligne["EMail"])
      echo "
	<strong>E-Mail</strong> : " .$ligne["EMail"]."<br />";
    echo "
	<br /><br />";
  }
  elseif ($_GET['radio_header']=="radio_empty_header") //pour impression sur papier a en-tete preimprime
    echo "<br /><br /><br /><br /><br /><br /><br /><br /><br />";
// //rien du tout si pleine page
  echo '</p>';
}

entete ($ligne);
//Lieu, date
$ville=$ligne["Ville"];

if (isset($_REQUEST['id_document']))
{

  $sexe=$ligne_patient['FchPat_Sexe'];
  $sexe_litteral['M']="masculin";
  $sexe_litteral['F']="féminin";

  $date=$ligne_document['RbDate_Date'];
  $date_seule=explode(" ",$date);
  $date_tableau=explode("-",$date_seule[0]);
  $date_affichable=$date_tableau[2]."/".$date_tableau[1]."/".$date_tableau[0];
  if ($_GET['radio_header']!="radio_no_header") //pas de paris, le... si impression brute sur plein A4
  {
?>
   <p style="display: block;
	    position: fixed;
	    top: 55mm;
	    margin-left: 5mm;">
<?php
      echo $ville.", le ".$date_affichable."<br />
      </p>";
  }
  if ($ligne_document['RbDate_TypeRub']!='20020500' AND $ligne_document['RbDate_TypeRub']!='20020300' AND $_GET['radio_header']!="radio_no_header") //ne pas imprimer le nom du patient en tete des courriers ou les certificats et pour pleine page
  {
?>
 <p style="display: block;
	  position: fixed;
	  top: 10mm;
	  margin-left: 110mm;">
<?php
	echo stripslashes($titre)." ".stripslashes($nom)." ".stripslashes($prenom)."<br />";
  }
  $number_medocs=0;
  if ($ligne_document['RbDate_TypeRub']=='20020100') //ordonnance structuree
  {
    list ($jour_debut,$mois_debut,$annee_debut)=explode ("-",date('d-m-Y', date('U')));
    //calcul de l'age

    $day_age  = $jour_debut - substr($ligne_patient["FchPat_Nee"],8,2);
    $mon_age = $mois_debut - substr($ligne_patient["FchPat_Nee"],5,2);
    $year_age = $annee_debut - substr($ligne_patient["FchPat_Nee"],0,4);

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
      $first_date = mktime(12,0,0,substr($ligne_patient["FchPat_Nee"],5,2),substr($ligne_patient["FchPat_Nee"],8,2),substr($ligne_patient["FchPat_Nee"],0,4)); 
      $second_date = mktime(12,0,0,$mois_debut,$jour_debut,$annee_debut); 

      $offset = $second_date-$first_date; //difference en secondes entre les dates
      $nombre_semaines = floor($offset/60/60/24/7); //pas bon

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
	  $afficher_jours=$nombre_semaines.$semaines.$difference_jours.$jours; //ici
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
      echo $afficher_mois.", soit ".$afficher_jours;
    elseif ($mon_age < 1 and $year_age < 1) //moins d'1 mois
      echo $afficher_jours;
    else //plus de 6 mois
      echo $afficher_annees." ".$afficher_mois;
    echo ' Sexe '.$sexe_litteral[$sexe];
    $number_medocs=mb_substr_count($ligne_document["RbDate_DataRub"], "<Posologie>");
    echo "<br /><strong>Nombre de médicaments</strong> : ".$number_medocs;
  } //fin ordo structuree
  if ($ligne_document['RbDate_TypeRub']!='20020500' AND $ligne_document['RbDate_TypeRub']!='20020300') //ne pas imprimer le nom du patient en tete des courriers ou les certificats
  {
  echo "
  </p>"; //fin en-tete
  }
?>
  <div style="display: block;
	  position: fixed;
<?php
if ($_GET['radio_header']!="radio_no_header")
{
?>
  margin-top:70mm;
  margin-bottom: 100mm;
<?php
}
else
{
?>
  margin-top:0mm;
  margin-bottom: 0mm;
<?php
}
?>
  margin-left: 10mm;
  margin-right: auto;">
<?php
	//Nettoyage des balises html
  $expr="`(<body[^>]*>)(.*)(<\/body>)`si";
  $fiche_mieux=preg_match($expr,$ligne_document["RbDate_DataRub"],$tab);
  $fiche_encore_mieux=preg_replace("`<body.*>`", " ",$tab[2]);
  $haystack=array('<br>','<u>','</u>');
  $needle=array('<br />','<em>','</em>');
  $fiche_encore_mieux=str_replace($haystack,$needle,$fiche_encore_mieux); //Fiche en l'air la mise en page
  if (stripos($ligne_document["RbDate_DataRub"],'</HTML_Data>')) //documents multimedia
  {
    $string=str_replace ('</HTML_Data>','<HTML_Data>',$ligne_document["RbDate_DataRub"]);
    $expl=explode('<HTML_Data>',$string);
    $html=strip_tags($expl[1],'<br /><img>');
    $elements=explode(' ',$html);
    $i=0;
    foreach ($elements as $this_element)
    {
      if (preg_match('`src`',$this_element))
      {
	echo 'src="afficher_image.php?RbDate_PrimKey='.$ligne_document["RbDate_RefBlobs_PrimKey"].'&amp;compteur_image='.$i.'" alt="Image" ';
	$i++;
      }
      else
	echo $this_element.' ';
    }
  }
  else
  {
//Affichage du contenu

    echo stripslashes($fiche_encore_mieux); //le contenu
  }
?>
  </div>
<?php
  if ($_GET['radio_header']=="radio_header") //pas de pied de page si fourni par le papier imprime ou pleine page A4
  {
?>
  <p style="display: block;
  position: fixed;
  bottom: 0;
  margin-left: auto;
  margin-right: auto;">
<?php
    if ($ligne["NumOrdre"])
    {
      if (!file_exists('log/'.$ligne["NumOrdre"].'.png')) //On ne se fatigue pas a creer un nouveau png si existe deja
      {
	// instanciation
	$bc = new pi_barcode();
	  
	// Le code a générer
	$bc->setCode($ligne["NumOrdre"]);
	// Type de code : EAN, UPC, C39...
	$bc->setType('C128');
	// taille de l'image (hauteur, largeur, zone calme)
	//    Hauteur mini=15px
	//    Largeur de l'image (ne peut être inférieure a
	//        l'espace nécessaire au code barres
	//    Zones Calmes (mini=10px) à gauche et à droite
	//        des barres
	$bc->setSize(40, 150, 10);
	  
	// Texte sous les barres :
	//    'AUTO' : affiche la valeur du codes barres
	//    '' : n'affiche pas de texte sous le code
	//    'texte a afficher' : affiche un texte libre
	//        sous les barres
	$bc->setText('AUTO');
	  
	// Si elle est appelée, cette méthode désactive
	// l'impression du Type de code (EAN, C128...)
	$bc->hideCodeType();
	  
	// Couleurs des Barres, et du Fond au
	// format '#rrggbb'
	$bc->setColors('#000000', '#FFFFFF');
	// Type de fichier : GIF ou PNG (par défaut)
	$bc->setFiletype('PNG');
	  
	// envoie l'image dans un fichier
	$bc->writeBarcodeFile('log/'.$ligne["NumOrdre"].'.png');
	// ou envoie l'image au navigateur
	// $bc->showBarcodeImage(); //ne marche pas
      } //fin generation image si necessaire
?>
<img src="log/<?php echo $ligne['NumOrdre'] ?>.png" alt="code barre ADELI"/>
<?php
    } //fin afficher image
    if ($ligne["NumRPPS"]!="")
    {
      $RPPS_short=substr($ligne["NumRPPS"],1,11);  //On enleve le premier caractere qui n'est pas le RPPS mais le fournisseur du RPPS
      if (!file_exists('log/'.$RPPS_short.'.png'))
      {
        $bc = new pi_barcode();
        $bc->setCode($RPPS_short);
        $bc->setType('C128');
        $bc->setSize(40, 150, 10);
        $bc->setText('AUTO');
        $bc->hideCodeType();
        $bc->setColors('#000000', '#FFFFFF');
        $bc->setFiletype('PNG');
        $bc->writeBarcodeFile('log/'.$RPPS_short.'.png');
      }
?>
<img src="log/<?php echo $RPPS_short ?>.png" alt="code barre RPPS"/><br />
<?php
    }
    echo $pied_de_page;
  } //fin du if radio_header
?>
  </p>
<?php
  if ($_GET['nouveau']=="Imprimer en duplicata")
  {
?>
<p style=" page-break-after: always;">
</p>
<?php
    entete ($ligne);
    if ($_GET['radio_header']!="radio_no_header") //pas de paris, le... si impression brute sur plein A4
    {
?>
    <p>
      <br /><br /><br /><br /><br /><br /><br /><br /><br /><br /><br />
      <span style="font-family:Arial;text-shadow:#000000 1px 1px; color : white;font-size : 20px; -moz-opacity: 0;filter:alpha(opacity=0);">DUPLICATA</b></span>
    </p>
<?php
    }
  }//fin si duplicata
}
?>
  </body>
</html>
