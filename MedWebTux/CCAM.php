<?php
session_start() ;
include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiant et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br />";
  exit;
}

$baseCCAM="CCAMTest";
if (!mysqli_select_db($db,$baseCCAM)) //On se connecte a la base CCAM
{
	print "Erreur ".mysqli_error($db)."<br />";
	mysqli_close($db);
	exit;
}

if ( isset( $_SESSION['login'] ) ) 
{
  $tab_login=explode("::",$_SESSION['login']);
  $signuser=$tab_login[1];
  if (isset($_REQUEST['bouton_ajouter_au_thesaurus']))
  {
    $deroulant_categories=$_REQUEST['deroulant_categories'];
    if ($deroulant_categories=="Autre")
      $categorie=$_REQUEST['categorie'];
    else
      $categorie=$deroulant_categories;
    $libusuel=$_REQUEST['libusuel'];
    $acte=$_REQUEST['acte'];
    $sql_ajouter_au_thesaurus="INSERT INTO THESAURUS (CODEMEDECIN,CATEGORIE,LIBUSUEL,CODEACTE) VALUES ('$signuser','$categorie','$libusuel','$acte')";
    $resultat_ajouter_au_thesaurus=mysqli_query($db,$sql_ajouter_au_thesaurus);
  }

  if (isset($_REQUEST['bouton_supprimer_thesaurus']) AND isset($_REQUEST['coche_supprimer']))
  {
    $acte_a_supprimer=$_REQUEST['acte_a_supprimer'];
    $sql_supprimer_thesaurus="DELETE FROM THESAURUS WHERE SERIE='$acte_a_supprimer'";
    $resultat_supprimer_thesaurus=mysqli_query($db,$sql_supprimer_thesaurus);
  }
  $sql_TOPOGRAPHIE1="SELECT * FROM TOPOGRAPHIE1";
  $resultat_TOPOGRAPHIE1=mysqli_query($db,$sql_TOPOGRAPHIE1);

  include("inc/header.php");
?>
    <title>
      MedWebTux - CCAM - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <script type="text/javascript">
//<![CDATA[
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
//]]>
    </script>
  </head>

  <body style="font-size:<?php echo $fontsize; ?>pt"  onload="donner_focus('mot1')"  >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
  $anchor="Mode_CCAM";
  include("inc/menu-horiz.php");

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


  $sql_version="SELECT MAX(DATECREATION) AS DATECREATION FROM VERSION "; //On prend la plus grande date

  $resultat_version=mysqli_query($db,$sql_version);

  $ligne_version=mysqli_fetch_assoc($resultat_version);
 
  $version_CCAM=$ligne_version['DATECREATION'];
  $version_CCAM=  substr($version_CCAM,0,4).'-'.substr($version_CCAM,4,2).'-'.substr($version_CCAM,6,2); //On met les tirets a la date
?>
  <div class="groupe">
      <h1>MedWebTux - CCAM version du <?php echo iso_to_local($version_CCAM,$date_format) ?></h1>

   <fieldset>
    <legend> Recherche par classification </legend>
      <form action="CCAM.php" method="get">
	<div>
	  <select name="TOPOGRAPHIE1" onchange="form.submit()" >
<?php
  $TOPOGRAPHIE1="";
  if (isset ($_REQUEST['TOPOGRAPHIE1']))
    $TOPOGRAPHIE1=$_REQUEST['TOPOGRAPHIE1'];
  if (isset ($_REQUEST['acte']))
    $TOPOGRAPHIE1=substr($_REQUEST['acte'], 0, 1);

  while ($ligne_TOPOGRAPHIE1=mysqli_fetch_array($resultat_TOPOGRAPHIE1))
  {
?>
	    <option value="<?php echo $ligne_TOPOGRAPHIE1['CODE'] ?>" <?php if ($ligne_TOPOGRAPHIE1['CODE']==$TOPOGRAPHIE1) echo "selected='selected'"?> >
	      <?php echo $ligne_TOPOGRAPHIE1['CODE']." ".str_replace("&","&amp;",$ligne_TOPOGRAPHIE1['LIBELLE'])?>
	    </option>
<?php
  }
?>
	  </select>
	  <input name="bouton_valider_topographie" type="submit" value="Envoyer" />
	</div>
      </form>
<?php
    if ($TOPOGRAPHIE1)
    {
?>
      <form action="CCAM.php" method="get">
	<p>
	  <input name="TOPOGRAPHIE1" value="<?php echo $TOPOGRAPHIE1 ?>" type="hidden" />
	</p>
	<div class="tableau">
	<table>
	  <tr>
	    <td>
	      Topographie
	    </td>
	    <td>
	      <select name="topographie2" >
<?php
      if (isset ($_REQUEST['acte']))
      {
	$topographie2=substr($_REQUEST['acte'], 0, 2);
      }
      else
      {
	if (isset($_REQUEST['topographie2']))
	  $topographie2=$_REQUEST['topographie2'];
	else
	  $topographie2='';
      }

      $sql_TOPOGRAPHIE2="SELECT * FROM TOPOGRAPHIE2 WHERE PERE='$TOPOGRAPHIE1'";
      $resultat_TOPOGRAPHIE2=mysqli_query($db,$sql_TOPOGRAPHIE2);
      while ($ligne_TOPOGRAPHIE2=mysqli_fetch_array($resultat_TOPOGRAPHIE2))
      {
?>
		<option value="<?php echo $ligne_TOPOGRAPHIE2['CODE'] ?>" <?php if ($topographie2==$ligne_TOPOGRAPHIE2['CODE']) echo "selected='selected'" ?> >
		  <?php echo $ligne_TOPOGRAPHIE2['CODE']." ".$ligne_TOPOGRAPHIE2['LIBELLE'] ?>
		</option>
<?php
      }
?>
	      </select>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      Action
	    </td>
	    <td>
	      <select name="action1">
		<option value="">
		</option>
<?php
      if (isset ($_REQUEST['acte']))
      {
	$action1=substr($_REQUEST['acte'], 2, 1);
      }
      elseif (isset($_REQUEST['action1']))
      {
	$action1=$_REQUEST['action1'];
      }
      else
	$action1='';
      $sql_action1="SELECT * FROM ACTION1";
      $resultat_action1=mysqli_query($db,$sql_action1);
      while ($ligne_action1=mysqli_fetch_array($resultat_action1))
      {
?>
		<option value="<?php echo $ligne_action1['CODE'] ?>" <?php if ($action1==$ligne_action1['CODE']) echo "selected='selected'"?> >
		  <?php echo $ligne_action1['CODE']." ".$ligne_action1['VERBE']?>
		</option>
<?php
      }
?>
	      </select>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      Accès
	    </td>
	    <td>
	      <select name="ACCES1">
		<option value="">
		</option>
<?php
      if (isset ($_REQUEST['acte']))
      {
	$ACCES1=substr($_REQUEST['acte'], 3, 1);
      }
      elseif (isset($_REQUEST['ACCES1']))
      {
	  $ACCES1=$_REQUEST['ACCES1'];
      }
      else
	$ACCES1='';
      $sql_ACCES1="SELECT * FROM ACCES1";
      $resultat_ACCES1=mysqli_query($db,$sql_ACCES1);
      while ($ligne_ACCES1=mysqli_fetch_array($resultat_ACCES1))
      {
?>
		<option value="<?php echo $ligne_ACCES1['CODE']?>" <?php if ($ACCES1==$ligne_ACCES1['CODE'] ) echo "selected='selected'" ?> >
		  <?php echo $ligne_ACCES1['CODE']." ".$ligne_ACCES1['ACCES']?>
		</option>
<?php
      }
?>
	      </select>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      <input name="bouton_valider" type="submit" value="Valider" />
	    </td>
	  </tr>
	</table>
	</div>
      </form>
   </fieldset>

 <?php
      if (isset($_REQUEST['bouton_valider']) )
      {
	echo "<div class=\"information\">"; 
	$debut_ccam=$topographie2.$action1.$ACCES1;
	$debut_ccam;
	$code2long=$debut_ccam."%";
	$sql_actes="SELECT * FROM ACTES WHERE CODE LIKE '$code2long'";
	$resultat_actes=mysqli_query($db,$sql_actes);

	while ($ligne_actes=mysqli_fetch_array($resultat_actes))
	{
	  echo "
      <br /><a href=\"CCAM.php?acte=".$ligne_actes['CODE']."\">".$ligne_actes['CODE']."</a> ".$ligne_actes['LIBELLELONG'];
	  $ce_code=$ligne_actes['CODE'];
	  $sql_histo_phase="SELECT * FROM HISTO_PHASE WHERE CODEACTE='$ce_code'";
	  $resultat_histo_phase=mysqli_query($db,$sql_histo_phase);
	  $ligne_histo_phase=mysqli_fetch_array($resultat_histo_phase);
	  echo "
      ".$ligne_histo_phase['PU'];
	}
	  echo "</div>";
      }
    } //fin si TOPOGRAPHIE1
    else //si pas de valeur pour le premier deroulant, on cloture le fieldset
    {
?>
    </fieldset>
<?php
    }
//acces a l'acte complet
?>
    <fieldset>
     <legend>Recherche par mot-clé </legend>
      <table>
	<tr>
	  <td>
	    <form title="plusieurs mots séparés par des %" action="CCAM.php" method="get">
	      <div>
		<input name="mot1" id="mot1" type="text" />
		<input name="bouton_envoyer_mots" type="submit" value="Envoyer" />
	      </div>
	    </form>
	  </td>
	</tr>
      </table>
    </fieldset>
  </div>

<?php
    if (isset($_REQUEST['mot1']))
    {
      $mot_joker="%".$_REQUEST['mot1']."%";
      $sql_mot="SELECT * FROM ACTES WHERE LIBELLELONG LIKE '$mot_joker'";
      $resultat_mot=mysqli_query($db,$sql_mot) or die(mysqli_error);
?>
<div class="information">
<?php
      echo "
      Recherche sur ".$mot_joker."<br />";

      while ($ligne_mot=mysqli_fetch_array($resultat_mot))
      {
	echo "
      <br /><a href=\"CCAM.php?acte=".$ligne_mot['CODE']."\">".$ligne_mot['CODE']."</a> ".$ligne_mot['LIBELLELONG'];	
      }
?>
</div>
<?php
    }
//La fiche de l'acte	
    if (isset($_REQUEST['acte']))
    {      
    $acte=$_REQUEST['acte'];
    $sexe[0]="Tous";
    $sexe[1]="Masculin";
    $sexe[2]="Féminin";
    $sql_acte="SELECT * FROM ACTES WHERE CODE='$acte'";
    $resultat_acte=mysqli_query($db,$sql_acte);
?>
	<div class="groupe">
	  <div class="tableau">
	    <table>
	      <tr>
		<th class="fond_th">
		  Acte
		</th>
		<th class="fond_th">
		  Thesaurus
		</th>
		<th class="fond_th">
		  Modificateurs
		</th>
		<th class="fond_th">
		  Associabilités
		</th>
		<th class="fond_th">
		  Incompatibilités
		</th>
	      </tr>
	      <tr>
		<td valign="top" class="fond_td">
<?php
    while ($ligne_acte=mysqli_fetch_array($resultat_acte))
    {
      $ce_sexe=$ligne_acte['SEXE'];
      echo "
      <strong>".$ligne_acte['CODE']."</strong><br />".$ligne_acte['LIBELLELONG']."
      <br /><b>Sexe :</b> ".$sexe[$ce_sexe]."
      <br /><b>Déplacement : </b>".$ligne_acte['DEPLACEMENT'];
      $ce_code=$ligne_acte['CODE'];
      $sql_histo_phase="SELECT * FROM HISTO_PHASE WHERE CODEACTE='$ce_code'";
      $resultat_histo_phase=mysqli_query($db,$sql_histo_phase);
      $ligne_histo_phase=mysqli_fetch_array($resultat_histo_phase);
      echo "
      <br /><b>Valeur </b>: ".$ligne_histo_phase['PU']." €";
//AFFICHER AJOUTER THESAURUS
      $sql_chercher_thesaurus="SELECT * FROM THESAURUS WHERE CODEMEDECIN='$signuser' AND CODEACTE='$ce_code' ORDER BY CATEGORIE,LIBUSUEL";
      $resultat_chercher_thesaurus=mysqli_query($db,$sql_chercher_thesaurus);
      $count_chercher_thesaurus=mysqli_num_rows($resultat_chercher_thesaurus);
?>
		  </td>
		  <td valign="top" class="fond_td">
<?php
      if ($count_chercher_thesaurus)
      {
	echo "
      <br /><b>Existe dans le thesaurus de $signuser</b>";
      }
      else //le mot n'est pas dans le thesaurus : on propose de l'ajouter
      {
//POUR LE DEROULANT DES CATEGORIES	
	$sql_chercher_categories="SELECT CATEGORIE FROM THESAURUS WHERE CODEMEDECIN='$signuser' GROUP BY CATEGORIE ORDER BY CATEGORIE";
	$resultat_chercher_categories=mysqli_query($db,$sql_chercher_categories);
?>
      <form action="CCAM.php" method="get">
	<table>
	  <tr>
	    <td valign="top">
	      <b>Catégorie&nbsp;: </b>
	    </td>
	    <td>
	      <input name="acte" type="hidden" value="<?php echo $acte ?>" />
	      <select name="deroulant_categories">
		<option value="Autre">
		  Autre
		</option>
<?php
	while ($ligne_chercher_categories=mysqli_fetch_array($resultat_chercher_categories))
	{
	  echo "
		<option value=\"".$ligne_chercher_categories['CATEGORIE']."\">
		  ".$ligne_chercher_categories['CATEGORIE']."
		</option>";
	}
?>
	      </select>
	      Si Autre, préciser&nbsp;: <input name="categorie" type="text" value="" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      <b>Libellé&nbsp;: </b>
	    </td>
	    <td>
	      <input name="libusuel" type="text" value="" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	    </td>
	    <td>
	      <input name="bouton_ajouter_au_thesaurus" type="submit" value="Ajouter au thesaurus" />
	    </td>
	  </tr>
	</table>
      </form>
<?php
		}
//les modificateurs
?>
		  </td>
		  <td valign="top" class="fond_td">
<?php
	  $sql_afficher_modificateurs="SELECT * FROM MODIFICATEURACTE INNER JOIN MODIFICATEUR ON MODIFICATEURACTE.MODIFICATEUR=MODIFICATEUR.CODE WHERE CODEACTE='$acte' GROUP BY MODIFICATEUR"; //METHODE simpliste pour se debarrasser des dates d'effet

	  $resultat_afficher_modificateurs=mysqli_query($db,$sql_afficher_modificateurs);
	  while ($ligne_afficher_modificateurs=mysqli_fetch_array($resultat_afficher_modificateurs))
	  {
	    echo "
      ".$ligne_afficher_modificateurs['MODIFICATEUR']." ".$ligne_afficher_modificateurs['LIBELLE']."<br />";
	  }
?>
</td>
<!-- Associabilités -->
		  <td valign="top" class="fond_td">    
		  <?php
	  $sql_afficher_associabilite="SELECT * FROM ASSOCIABILITE WHERE CODEACTE='$acte' GROUP BY ACTEASSO"; 
	  $resultat_afficher_associabilite=mysqli_query($db,$sql_afficher_associabilite);
	  while ($ligne_afficher_associabilite=mysqli_fetch_array($resultat_afficher_associabilite))
	  {
	    echo "
      <a href=\"CCAM.php?acte=".$ligne_afficher_associabilite['ACTEASSO']."\">".$ligne_afficher_associabilite['ACTEASSO']."</a><br />";
	  }
?>
		  </td>
		  <td valign="top" class="fond_td">
<!-- les incompatibilites -->
<?php
	$sql_incompatibilite="SELECT * FROM INCOMPATIBILITE WHERE CODEACTE='$acte'";
	$resultat_incompatibilite=mysqli_query($db,$sql_incompatibilite);
	while ($ligne_incompatibilite=mysqli_fetch_array($resultat_incompatibilite))
	{
	  echo "
      <a href=\"CCAM.php?acte=".$ligne_incompatibilite['INCOMPATIBLE']." \">".$ligne_incompatibilite['INCOMPATIBLE']."</a><br />";
	}
?>
</td>
</tr>
</table>
<?php     		
    }
 echo "</div></div>";
  } //fin de si resultat

//Thesaurus
    if (isset($_REQUEST['bouton_afficher_thesaurus']))
    {
      $sql_chercher_thesaurus="SELECT * FROM THESAURUS WHERE CODEMEDECIN='$signuser' ORDER BY CATEGORIE,LIBUSUEL";
      $resultat_chercher_thesaurus=mysqli_query($db,$sql_chercher_thesaurus);

     echo "<div class=\"groupe\">
      <h1>
	Thesaurus de $signuser
      </h1>";

      echo "<div class=\"tableau\">
      <table>";
      while ($ligne_chercher_thesaurus=mysqli_fetch_array($resultat_chercher_thesaurus))
      {
	echo "
	<tr>
	  <td class=\"fond_td\">
	    <a href=\"CCAM.php?acte=".$ligne_chercher_thesaurus['CODEACTE']."\">".$ligne_chercher_thesaurus['CODEACTE']."</a>
	  </td>
	  <td class=\"fond_td\">
	    ".$ligne_chercher_thesaurus['CATEGORIE']."
	  </td>
	  <td class=\"fond_td\"> 
	    ".$ligne_chercher_thesaurus['LIBUSUEL']."
	  </td>";
?>
	  <td class="fond_td">
	    <form action="CCAM.php" method="get">
	      <div>
		<input name="bouton_afficher_thesaurus" type="hidden" value="xx" />
		<input name="acte_a_supprimer" type="hidden" value="<?php echo $ligne_chercher_thesaurus['SERIE'] ?>" />
		<b>Supprimer</b> <input name="coche_supprimer" type="checkbox" />
		<input name="bouton_supprimer_thesaurus" type="submit" value="Confirmer" />
	      </div>
	    </form>
	  </td>
	</tr>
<?php
      }
?>
      </table>
      </div>
    </div>
<?php
    }
    else //on affiche le bouton du thesaurus
    {
?>
     <div class="groupe">
      <form action="CCAM.php" method="get">
	<fieldset>
	<legend>
	  Thesaurus
	</legend>
	  <input name="bouton_afficher_thesaurus" type="submit" value="Afficher le thesaurus" />
	</fieldset>
      </form>  
    </div>
<?php
    } //fin thesaurus
  } //FIN SESSION
  else // pas identifie
  {
    header('location: index.php?page=CCAM' );
    exit;
  }
include("inc/footer.php");
?>
