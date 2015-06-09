<?php
session_start() ;

include("config.php");
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

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=liste' );
  exit;
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

if (isset($_GET['numeroID']))
{
  $patient=$_GET['numeroID'];
  //On cherche dans la table IndexNomPrenom la fiche dont l'ID a ete demande
  $sql_chercher_nom="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
  $resultat_chercher_nom=mysqli_query($db,$sql_chercher_nom);
  $ligne_chercher_nom=mysqli_fetch_array($resultat_chercher_nom);
  $nom=$ligne_chercher_nom["FchGnrl_NomDos"];
  $prenom=$ligne_chercher_nom["FchGnrl_Prenom"];
}
else //S'il manque le patient, on est redirige automatiquement sur la page de recherche des patients
{
  header('location: liste.php' );
  exit;
}


if (isset($_GET['type_info']))
{
//type_info[]=observ&type_info[]=ordo&type_info[]=terrain&type_info[]=docu&type_info[]=courrier&type_info[]=notes&type_info[]=classeurs&type_info[]=90010000
  $type_info=$_REQUEST['type_info'];
}
else
  $type_info="";


//chercher les droits d'utilisateur
$tab_login=explode("::",$_SESSION['login']);
$user=$tab_login[0];
$sign_user=$tab_login[1];

//on cherche le droit direct si on est le sign_user
$sql_chercher_droits="SELECT * FROM Personnes WHERE Login='$user'";
$resultat_chercher_droits=mysqli_query($db,$sql_chercher_droits);
$ligne_chercher_droits=mysqli_fetch_array($resultat_chercher_droits);
$GUID_user=$ligne_chercher_droits['GUID'];
$droits_user=$ligne_chercher_droits['Droits'];

//on nettoie les balises html non valides en xhtml
$find=array('<BR>','<br>','<hr>','<u>','</u>');
$replace=array('<br />','<br />','<br />','<em>','</em>');

include("inc/header.php");
?>
    <title>
      MedWebTux - Dossier complet d'un patient - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <link rel="stylesheet" href="css/print.css" type="text/css" media="print"/>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen"/>
  </head>

  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
  $anchor='dossier_complet';
//  include("inc/menu-horiz.php");
?>
  <div class="groupe">
      <h1>
	Dossier Patient Complet
      </h1>

      <table>
<?php
//On cherche dans la table IndexNomPrenom la fiche dont le nom a ete demande
$sql_nom="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
$resultat_nom=mysqli_query($db,$sql_nom);
$count_nom=mysqli_num_rows($resultat_nom);
$ligne_nom=mysqli_fetch_array($resultat_nom);
$nom=$ligne_nom["FchGnrl_NomDos"];
$primkey=$ligne_nom["ID_PrimKey"];

//On cherche dans la table fchpat la fiche dont le nom a ete demande
$sql_fchpat="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$patient'";

$resultat_fchpat=mysqli_query($db,$sql_fchpat);
$count_fchpat=mysqli_num_rows($resultat_fchpat);
$ligne_fchpat=mysqli_fetch_array($resultat_fchpat);

//On cherche les notes
$sql_notes="SELECT * FROM fchpat_Note WHERE fchpat_Note_PatPK='$primkey'";
$resultat_notes=mysqli_query($db,$sql_notes);
$count_notes=mysqli_num_rows($resultat_notes);
$ligne_notes=mysqli_fetch_array($resultat_notes);

//On cherche les correspondants
$sql_corresp="SELECT * FROM fchpat_Intervenants INNER JOIN Personnes ON fchpat_Intervenants.fchpat_Intervenants_IntervPK=Personnes.ID_PrimKey WHERE fchpat_Intervenants_PatGUID='$patient'";
$resultat_corresp=mysqli_query($db,$sql_corresp);
$count_corresp=mysqli_num_rows($resultat_corresp);

//On accorde la phrase selon le genre
if ($ligne_fchpat["FchPat_Sexe"]=="M")
{
  $ne="n&eacute;";
}
else 
{
  $ne="n&eacute;e";
}

//Affichage de la fiche de base
$lignes_adresse=explode("\n",$ligne_fchpat["FchPat_Adresse"]);
if (count(explode ("-",$ligne_fchpat["FchPat_Nee"]))==3) //verifier si la date est bien formatee
{
  $table_naissance_full=explode (" ",$ligne_fchpat["FchPat_Nee"]);
  $table_naissance=explode ("-",$table_naissance_full[0]);
  if ($date_format=='fr')
  {
    $naissance=$table_naissance[2].'-'.$table_naissance[1].'-'.$table_naissance[0];
  }
  elseif ($date_format=='en')
  {
    $naissance=$table_naissance[1].'-'.$table_naissance[2].'-'.$table_naissance[0];
  }
  else
  {
    $naissance=$table_naissance[0].'-'.$table_naissance[1].'-'.$table_naissance[2];
  }
}
else
  $naissance='invalid';

echo '
	<tr>
	  <td>
	    '.$ligne_fchpat["FchPat_Titre"]." <strong><a href=\"patient.php?GUID=$patient&amp;affichage=rien>\">".$nom." ".$ligne_nom["FchGnrl_Prenom"]."</a></strong>, ".$ne." le ".$naissance."<br />";
foreach ($lignes_adresse as $ligne_adresse)
{
  echo "
	    ".$ligne_adresse."<br />";
}
echo $ligne_fchpat["FchPat_CP"]." ".$ligne_fchpat["FchPat_Ville"]."<br />";
//Affichage des numeros de telephone si possible
if ($ligne_fchpat["FchPat_Tel1"])
{
  echo "
	    <strong>T&eacute;l&eacute;phone 1</strong> : ".$ligne_fchpat["FchPat_Tel1"]."<br />";
}
if ($ligne_fchpat["FchPat_Tel2"])
{
  echo "
	    <strong>T&eacute;l&eacute;phone 2</strong> : ".$ligne_fchpat["FchPat_Tel2"]."<br />";
}
if ($ligne_fchpat["FchPat_Tel3"])
{
  echo "
	    <strong>T&eacute;l&eacute;phone 3</strong> : ".$ligne_fchpat["FchPat_Tel3"]."<br />";
}
//Affichage du nom de jeune fille si possible
if ($ligne_fchpat["FchPat_NomFille"])
{
  echo "
	    <strong>Nom de jeune fille</strong> : ".$ligne_fchpat["FchPat_NomFille"]."<br />";
}
//Affichage du mail si possible
if ($ligne_fchpat["FchPat_Email"])
{
  echo "
	    <strong>e-mail</strong> : <a href=\"mailto:".$ligne_fchpat["FchPat_Email"]."\">".$ligne_fchpat["FchPat_Email"]."</a><br />";
}
//Affichage du num de secu si possible
if ($ligne_fchpat["FchPat_NumSS"])
{
  echo "
	    <strong>Num&eacute;ro de s&eacute;cu</strong> : ".substr($ligne_fchpat["FchPat_NumSS"],0,1)." ".substr($ligne_fchpat["FchPat_NumSS"],1,2)." ".substr($ligne_fchpat["FchPat_NumSS"],3,2)." ".substr($ligne_fchpat["FchPat_NumSS"],5,2)." ".substr($ligne_fchpat["FchPat_NumSS"],7,3)." ".substr($ligne_fchpat["FchPat_NumSS"],10,3)." ".substr($ligne_fchpat["FchPat_NumSS"],13,2)."<br />";
}
//Affichage de la profession si possible
if ($ligne_fchpat["FchPat_Profession"])
{
  echo "
	    <strong>Profession</strong> : ".$ligne_fchpat["FchPat_Profession"]."<br />";
}

//Affichage des notes si possible
if ($ligne_notes["fchpat_Note_Html"])
{
    if (detectUTF8($ligne_notes["fchpat_Note_Html"]))
      $Note=($ligne_notes["fchpat_Note_Html"]);
    else
      $Note=utf8_encode($ligne_notes["fchpat_Note_Html"]);


//Recherche des notes pour ce patient
  echo "
	    <strong>Notes </strong>:
	    <ul>
	      <li>
		".preg_replace("`\n`","
	      </li>
	      <li>
		",$Note)."
	      </li>
	    </ul>";
}
?>
	  </td>
	</tr>
<?php
$resultat_global=mysqli_query($db,"SELECT * FROM RubriquesHead INNER JOIN RubriquesBlobs ON RubriquesBlobs.RbDate_PrimKey=RubriquesHead.RbDate_RefBlobs_PrimKey WHERE RubriquesHead.RbDate_IDDOs='$patient' ORDER BY RbDate_Date DESC");

foreach ($rubrique_perso AS $this_rubrique) //pris dans le config.php
{
  $elts_rubrique=explode("|",$this_rubrique);
  $num_rubrique=$elts_rubrique[2];
  $nom_rubrique[$num_rubrique]=$elts_rubrique[0];
}

function verif_login($login_possesseur_doc,$droits_user,$GUID_user,$num_rubrique,$user,$rubrique_perso,$db)
{
  foreach ($rubrique_perso AS $this_rubrique)
  {
    $elts_rubrique=explode("|",$this_rubrique);
    if ($elts_rubrique[2]==$num_rubrique)
    {
      $droit=$elts_rubrique[1]."v"; //trouver le droit de la rubrique en fonction du numero
    }
  }
  
  if ($login_possesseur_doc==$user) //On verifie si l'observation appartient a l'utilisateur
  {
    if (preg_match("/".$droit."/",$droits_user))//si oui, on verifie qu'il a les droits
    {
      $lectureOK="1";
    }
    else //si pas possesseur
      $lectureOK="";
    return $lectureOK; //marche pas
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
      if (preg_match("/".$droit."/",$liste_utilisateur_autorise['FriendUserDroits']))//On verifie qu'il a les droits
	$lectureOK=1;
      else //si connu pas autorise
	$lectureOK="";
    }
    else //si pas connu
      $lectureOK="";
  }
  return $lectureOK;
} //fin function verif_login

$expr="`(<body[^>]*>)(.*)(<\/body>)`i";
$expr2="`\<\?xml(.*)`";

while ($ligne_global=mysqli_fetch_array($resultat_global))
{
  $id_document=$ligne_global['RbDate_RefBlobs_PrimKey'];

  $login_possesseur_doc=$ligne_global['RbDate_CreateSignUser'];
  $login_createur_doc=$ligne_global['RbDate_CreateUser'];


//On verifie que le document trouve est d'un type demande
  foreach ($type_info AS $this_type_info) //On balaye les types recus en URL
  {
    if ($ligne_global['RbDate_TypeRub']==$this_type_info)
    {
      $lectureOK=verif_login($login_possesseur_doc,$droits_user,$GUID_user,$this_type_info,$user,$rubrique_perso,$db); 
      $table_date_full=explode (" ",$ligne_global["RbDate_Date"]);
      $date=iso_to_local($table_date_full[0],$date_format);

//Gerer la mise en forme et les droits
     echo '
	<tr>
	  <th style="text-align:center">
	    <a href="consultation.php?numeroID='.$patient.'&amp;date='.$table_date_full[0].'">'.$date.'</a> '.$nom_rubrique[$this_type_info].' : '.$ligne_global['RbDate_NomDate'].'
	  </th>
	</tr>
	<tr>
	  <td>';
      if ($lectureOK)
      {
	if ($this_type_info==20020100) //ordo structuree
	{
	  if (preg_match('/<body[^>]*>(.*)<\/body>/si', $ligne_global["RbDate_DataRub"], $tabordo))
	  {
	    $fiche_encore_mieux=$tabordo[1];
	  }
	  else //au cas ou le texte n'est pas entoure de balises body
	    $fiche_encore_mieux=$ligne_global["RbDate_DataRub"];
	  echo '
	    '.str_replace($find,$replace,$fiche_encore_mieux);
	}
	elseif ($this_type_info==20080000) //documents
	{
	  $string=str_replace ('</HTML_Data>','<HTML_Data>',$ligne_global["RbDate_DataRub"]);
	  if (stripos ($string,"<HTML_Data>"))
	  {
	    $expl=explode('<HTML_Data>',$string);
	    $html=strip_tags($expl[1],'<br><img>');
	  }
	  else
	    $html=strip_tags($string,'<br><img>');
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
	      echo str_replace($find,$replace,$this_element).' ';
	  }
	}
	elseif ($this_type_info==20060000) //terrains
	{
	  $terrain=$ligne_global['RbDate_DataRub'];
	  $terrain=preg_replace($expr2,"",$terrain);
	  $terrain_html=preg_replace("`\n`","<br />",$terrain);
  //le terrain brut avant interpretation
	  $split_terrain=explode("\n", $terrain);//tableau dont chaque valeur est une ligne du terrain
	  $fin_ligne=""; //reperes pour terminer les tableaux de valeurs
	  $fin_table="";
	  $fin_titre="";
	  $atcd="";
	  $exp_date="`([0-9]{4}-[0-9]{2}-[0-9]{2})`";//rechercher si ligne contient une date
	  foreach ($split_terrain as $ligne_brut)
	  {
	    if (preg_match("`\[Ant`i",$ligne_brut))//detecter le titre ATCD
	    {
	      echo "
	    <strong>Ant&eacute;c&eacute;dents</strong><br />"; //affichage du titre sans les crochets.
	      $atcd=1; //initialisation pour afficher les colonnes
	    }
	    if ($atcd==1 and !(preg_match("`\[Ant`i",$ligne_brut)) and !(preg_match("`(\[Obst)`i",$ligne_brut)))
	    {
	      $lecture_atcd="";
	      $login_possesseur_terrain=$ligne_global['RbDate_CreateUser'];
	      $sql_recherche_possesseur="SELECT GUID FROM Personnes WHERE Login='$login_possesseur_terrain'";
	      $resultat_recherche_possesseur=mysqli_query($db,$sql_recherche_possesseur);
	      $ligne_recherche_possesseur=mysqli_fetch_array($resultat_recherche_possesseur);
	      $GUID_possesseur_terrain=$ligne_recherche_possesseur['GUID'];

	      if ($login_possesseur_terrain==$user) //On verifie si le terrain appartient a l'utilisateur
	      {
		if (preg_match("`atv`",$droits_user))//si oui, on verifie qu'il a les droits
		{
		  $lecture_atcd=1;
		}
	      }
	      else //sinon on verifie qu'il est utilisateur autorise
	      {
		$sql_utilisateur_autorise="SELECT * FROM user_perms WHERE FriendUserGUID='$GUID_user' AND SignataireGUID='$GUID_possesseur_terrain'";
		$resultat_utilisateur_autorise=mysqli_query($db,$sql_utilisateur_autorise);
		$count_utilisateur_autorise=mysqli_num_rows($resultat_utilisateur_autorise);
		if ($count_utilisateur_autorise)//S'il existe un utilisateur autorise
		{
		  $liste_utilisateur_autorise=mysqli_fetch_array($resultat_utilisateur_autorise);
		  if (preg_match("`atv`",$liste_utilisateur_autorise['FriendUserDroits']))//On verifie qu'il a les droits
		    $lecture_atcd=1;
		}
	      }

	      if ($lecture_atcd)
	      {
		echo $ligne_brut."<br />";//Les lignes d'ATCD
	      }
	      else
	      {
		echo "
	    Les droits de lecture des ant&eacute;c&eacute;dents appartiennent &agrave; ".$login_possesseur_terrain."<br />";
	      }
	    }
	    if (preg_match("`(\[Obst)`i",$ligne_brut)) //nettoyage et affichage du titre Obstetrique
	    {
	      $atcd="";//on sort de l'affichage des antecedents
	      if ($ligne_fchpat["FchPat_Sexe"]=="F")
	      {
		echo "
	    <strong>Obst&eacute;trique</strong><br />";
	      }
	    }
	    if (preg_match("`Grossesse`i",$ligne_brut) OR preg_match("`Allaitement`",$ligne_brut)) //On affiche les lignes Grossesse Allaitement
	    {
	      if ($ligne_fchpat["FchPat_Sexe"]=="F")//affichage de la gyneco seulement si feminin
	      {
		echo '
	    '.$ligne_brut."<br />";
	      }
	      if (preg_match("`Grossesse`i",$ligne_brut))
	      {
		      $grossesse=1;
	      }
	      if (preg_match("`Allaitement`i",$ligne_brut))
	      {
		      $allaitement=1;
	      }
	    }
	    if (preg_match("`(\[VAR )`i",$ligne_brut))//nettoyage et recuperation des titres des tableaux
	    {
	      if ($fin_ligne) //lu a partir du deuxieme tableau qui contient des valeurs pour cloturer le precedent
	      {
		echo "
	      </tr>";
		$fin_ligne="";
	      }
	      $titres=preg_replace("`\[VAR`i","",$ligne_brut);
	      $titre_propre=preg_replace("`]`","",$titres);
//cloturer le tableau precedent
	      if ($fin_titre)
	      {
		echo "
	      </tr>"; //fin de ligne du titre
		$fin_titre=""; //On ne traitera plus les titres apres la premiere valeur
	      } 
	      if ($fin_table)
	      {
		echo "
	    </table>";
		$fin_table="";
	      }
	      echo "
	    <strong>".$titre_propre."</strong><br />
	    <table>
	      <tr style=\"background:#DDFFDD\">
		<th>
		  Date
		</th>";//initialisation des tableaux de valeurs
	      }
	      if (preg_match ("`Mod`",$ligne_brut))//recuperer les noms des colonnes si la ligne contient Modele
	      {
		$split_modele=explode(",",$ligne_brut);
		echo "
		<th>
		  ".$split_modele[1]."
		</th>";//affichage des colonnes.
		$fin_titre=1;
		$fin_table=1;
	      }
	      if (preg_match($exp_date,$ligne_brut))//detection d'une date pour les valeurs des tableaux
	      {
		if ($fin_ligne)
		{
		  echo "
	      </tr>";
		  $fin_ligne="";
		}
		if ($fin_titre)
		{
		  echo "
	      </tr>"; //fin de ligne du titre
		  $fin_titre=""; //On ne traitera plus les titres apres la premiere valeur
		} 
		$split_ligne_valeurs=explode("=",$ligne_brut); //On coupe au signe = pour separer la date des valeurs
		echo "
	      <tr>
		<td>
		".$split_ligne_valeurs[0]."
		</td>";//affichage de la date en debut de ligne
		  $split_valeurs=explode(",",$split_ligne_valeurs[1]);//On separe les valeurs au niveau des virgules
		  foreach ($split_valeurs as $valeur)
		  {
		    echo "
		<td>
		  ".$valeur."
		</td>";
		    $fin_ligne=1;
		  }
		  $fin_ligne=1;
		  $fin_table=1;
		}
		if (preg_match("`\[Traitement]`",$ligne_brut)) 
		{
		  if ($fin_titre)
		  {
		    echo "
	      </tr>"; //fin de ligne du titre - fonctionne, seulement pour le dernier tableau
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
	</table>";
		    $fin_table="";
		  }
		} //fin traitement chronique
	      } //fin foreach split
	      $tab_chronique="";	
	      //On ajoute le contenu des ordonnances chroniques, epurees des balises
	      $ordo_mieux=(preg_match($expr,$terrain,$tab_chronique));
	      if ($tab_chronique) //eviter un message d'erreur si pas d'ordonnance chronique
	      {
		if ($ordo_chronique=preg_replace("`<body.*>`", " ",$tab_chronique[2]))
		  echo "
	<strong>Traitement habituel</strong>"; //titre si pas vide
		echo "
	".$ordo_chronique."<br />";
	      }
	}
	else //ni ordo structuree ni document image
	{
	//Nettoyage des balises html
	  $fiche_mieux=preg_match('/<body[^>]*>(.*)<\/body>/si', $ligne_global["RbDate_DataRub"], $tab);
	  $chaine=str_replace($find,$replace,$tab[1]);
	  echo str_replace('__________________','_',$chaine); //le contenu de l'observation sans les balises body et en raccourcissant les ____ pour que ca tienne mieux
	}
      }
      else
      {
	echo "
	    <b>Acc&egrave;s interdit</b> (r&eacute;serv&eacute; &agrave; <b>".$login_possesseur_doc."</b> ou &agrave; ses utilisateurs d&eacute;l&eacute;gu&eacute;s)<br />";
      }
      echo '
	  </td>
	</tr>';
    }
  }
}
?>
      </table>
  </div>

<?php
include("inc/footer.php");
?>