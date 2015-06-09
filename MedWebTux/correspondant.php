<?php
session_start() ;	
include("config.php");
if ( isset( $_SESSION['login'] ) )
{

  $db=mysqli_connect($host, $loginbase, $pwd);
  $codage=mysqli_query($db,"SET NAMES 'UTF8'");
  if(!$db)
  {
    print "Erreur de connexion &agrave; $db<br />";
    exit;
  }
  // on choisit la bonne base
  if (!mysqli_select_db($db,$base))
  {
    print "Erreur ".mysqli_error($db)."<br />";
    mysqli_close($db);
    exit;
  }
  $Qualite='';
  $critere_recherche='';
  $exact=0;
  $cle='';

  if (isset($_REQUEST['envoyer'])) //ne pas faire de recherche si rien a chercher
  {
    if (isset($_GET['exact']))
      $exact=$_GET['exact'];
    $intervenant_user=array();
    if (isset($_REQUEST['intervenant_user']))
    {
      $intervenant_user=$_REQUEST['intervenant_user'];
      $type=" AND (";
      foreach ($intervenant_user AS $cet_intervenant_user)
      {
	if ($type==" AND (")
	{
	  if ($cet_intervenant_user=="users")
	    $type=$type." Login!=''";
	  if ($cet_intervenant_user=="no_users")
	    $type=$type." Login =''";
	}
	else
	{
	  if ($cet_intervenant_user=="users")
	    $type=$type." OR Login !=''";
	  if ($cet_intervenant_user=="no_users")
	    $type=$type." OR Login =''";
	}
      }
      $type=$type.")";
    }
    else //deux decoches
      $type=" AND 0";

    if (isset($_GET['cle']))
      $cle=$_GET['cle'];
    if (isset($_GET['critere_recherche']))	
      $critere_recherche=$_GET['critere_recherche'];
    if (isset($_GET['Qualite']))	
      $Qualite=$_GET['Qualite'];
  }
  include("inc/header.php");
?>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
    <style type="text/css"  media="print">
	td:nth-child(8) { display: none; }  
	td:nth-child(9) { display: none; }  
	td:nth-child(10) { display: none; }  
	th:nth-child(8) { display: none; }  
	th:nth-child(9) { display: none; }  
	th:nth-child(10) { display: none; }  
    </style>
    <title>
      MedWebTux - Intervenants - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>

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
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
//]]>
    </script>
  </head>
  <body style="font-size:<?php echo $fontsize ?>pt"  onload="donner_focus('cle')" >
    <div class="conteneur">
<?php
if (isset($_REQUEST['envoyer'])) //ne pas faire de recherche si rien a chercher
{
?>
      <div class='noScreen'>
<!-- en-tete pour impression -->
<?php	
    $tab_login=explode("::",$_SESSION['login']);
    $user=$tab_login[0];

    $sql="SELECT * FROM Personnes WHERE Login ='$user'";
    $resultat=mysqli_query($db,$sql);
    $ligne=mysqli_fetch_array($resultat);

    echo "
	<strong>".$ligne["Titre"]." ".$ligne["Nom"]." ".$ligne["Prenom"]."</strong><br />";
    echo "
	".$ligne["Adresse"]."<br />";
    echo "
	".$ligne["CodePostal"]." ".$ligne["Ville"]."<br />";
    if ($ligne["Convention"])
      echo "
	".$ligne["Convention"]."<br />";
    echo "
	".$ligne["Qualite"]."<br />";
    if ($ligne["NumOrdre"])
      echo "
	<strong>Num&eacute;ro d'ordre</strong>&nbsp;: " .$ligne["NumOrdre"]."<br />";
    if ($ligne["Tel_1"])
      echo "
	<strong>T&eacute;l</strong>&nbsp;: " .$ligne["Tel_1"]."<br />";
    /*if ($ligne["Tel_2"])
	    echo "<strong>T&eacute;l&eacute;phone 2</strong> : " .$ligne["Tel_2"]."<br />";
    if ($ligne["Tel_3"])
	    echo "<strong>T&eacute;l&eacute;phone 3</strong> : " .$ligne["Tel_3"]."<br />";
    */
    if ($ligne["EMail"])
      echo "
	<strong>E-Mail</strong>&nbsp;: " .$ligne["EMail"]."<br />";
 //fin en-tete medecin
  // insertion du menu d'en-tete et du formulaire de recherche	
?>
      </div><!--fin du noscreen d'en tete -->
<?php
} //fin du mode recherche
    $anchor='Mode_Intervenants';
    include("inc/menu-horiz.php");
?>
    <div class="noPrint">
   <div class="groupe">
      <h1>MedWebTux - Intervenants</h1>
<!-- Le formulaire de recherche d'intervenants -->
   
    <div class="tableau">
    <table> 
	<tr>
	   <td>
     <fieldset class="fieldset_formu">
	<legend>Rechercher un intervenant</legend>
	    <form action="correspondant.php" method="get">
	      <p>
		<label for="Qualite"><strong>Sp&eacute;cialit&eacute;&nbsp;: </strong></label>
		<select name="Qualite" id="Qualite">
		  <option value="%%">
		    Toutes
		  </option>
<?php
    $sql_specialite="SELECT Qualite FROM Personnes GROUP BY Qualite ORDER BY Qualite";
    $resultat_specialite=mysqli_query($db,$sql_specialite);

    while ($ligne_specialite=mysqli_fetch_array($resultat_specialite))//recherche des specialites pour en faire un deroulant
    { 
      if ($ligne_specialite['Qualite'])//suppression des lignes vides
      {
?>
		  <option value="<?php echo $ligne_specialite['Qualite'];?>" <?php if ($ligne_specialite['Qualite']==$Qualite) echo 'selected="selected"'?> >
		    <?php echo $ligne_specialite['Qualite']; ?>
		  </option>
<?php
      } //fin if pas ligne vide
    } //fin while
?>
		</select><!--Fin recherche qualite -->
		</p>

	    	<p>
		<select name="critere_recherche"><!--Liste des cles de recherche-->
		  <option selected="selected" value="Nom" <?php if ($critere_recherche=="Nom") ?> >Nom</option>
		  <option value="Prenom" <?php if ($critere_recherche=="Prenom") echo 'selected="selected"'?> >Pr&eacute;nom</option>
		  <option value="Adresse" <?php if ($critere_recherche=="Adresse") echo 'selected="selected"'?> >Adresse</option>
		  <option value="Ville" <?php if ($critere_recherche=="Ville") echo 'selected="selected"'?> >Ville</option>
		  <option value="CodePostal" <?php if ($critere_recherche=="CodePostal") echo 'selected="selected"'?> >Code Postal</option>
		</select>
		<input name="cle" id="cle" type="text" value="<?php echo str_replace('%','',$cle) ?>" size="30" />
		<br />
		<input name="exact" id="exact" type="checkbox" value="1" <?php if ($exact=="1") echo 'checked="checked"' ?> />
		<label for="exact" >
		  Correspondance exacte
		</label>
		<br />
<!-- //Retour du mode supprimer = critere_recherche=Nom&cle=azerty&envoyer=Chercher -->
		<input name="intervenant_user[]" id="intervenant_user_oui" type="checkbox" <?php if (isset ($_REQUEST['envoyer'])) { if (in_array('users',$intervenant_user)) echo 'checked="checked"'; } else  echo 'checked="checked"' ?> value="users" />
		<label for="intervenant_user_oui" >
		  Utilisateurs
		</label>
		<br />
		<input name="intervenant_user[]" id="intervenant_user_non" type="checkbox" <?php if (isset ($_REQUEST['envoyer'])) { if (in_array('no_users',$intervenant_user)) echo 'checked="checked"'; } else  echo 'checked="checked"' ?> value="no_users" />
		<label for="intervenant_user_non" >
		  Non utilisateurs
		</label>
		<br />
		<input name="envoyer" type="submit" value="Chercher" />
	      </p>
	    </form>
	</fieldset>

	<fieldset class="fieldset_formu">
	<legend>Importer des vcard...</legend>
	    <form action="validation_correspondant.php" method="post" enctype="multipart/form-data">
		<div>
		<input type="file" name="selection" id="selection" onchange="showButton('button_place')" />
		  <div id="button_place" style="display:none"> 
		    <input name="button_envoyer_vcard" type="submit" value="Ajouter" />
		  </div>
		</div>
	    </form>
	 </fieldset>

<?php
//ne pas afficher le bouton d'export si rien a exporter (faire une recherche d'abord)
if (isset($_REQUEST['envoyer'])) 
{
?>
<!-- Le bouton vcard renvoie les criteres de recherche pour n'exporter que les intervenants affiches -->
     <fieldset class="fieldset_formu">
	<legend title="Exporte le résultat de la recherche en format vcard pour intégration à un carnet d'adresses de téléphone ou de messagerie">Exporter format vcard...</legend>
	    <form action='vcard.php' method='post' id='form_export_vcard'>
	    <div>
		<input type="hidden" name="Qualite" value="<?php if (isset($_REQUEST['Qualite'])) echo $_REQUEST['Qualite']?>"/>
		<input type="hidden" name="critere_recherche" value="<?php if (isset($_REQUEST['critere_recherche'])) echo $_REQUEST['critere_recherche']?>"/>
		<input type="hidden" name="cle" value="<?php  if (isset($_REQUEST['cle'])) echo $_REQUEST['cle']?>"/>
		<input type="hidden" name="exact" value="<?php  if (isset($_REQUEST['exact'])) echo $_REQUEST['exact']?>"/>
<?php
  foreach ($intervenant_user AS $cet_intervenant_user)
  {
    if ($cet_intervenant_user=="users")
    {
?>
		<input type="hidden" name="intervenant_user[]" value="users"/>
<?php
    }
    elseif ($cet_intervenant_user=="no_users")
    {
?>
		<input type="hidden" name="intervenant_user[]" value="no_users"/>
<?php
    }
  }
?>
		<input type="hidden" name="envoyer" value="envoyer"/>
		<input name='vcard' value='Exporter en vcard' type='submit' />
	    </div>
<?php
	if (isset ($_REQUEST['vcard'])) {
		echo '<p><a href="log/vcard_intervenants.vcf">Enregistrez ce lien pour avoir toutes les vcards</a></p>';
	}
?>
	    </form>
	</fieldset>
	
<?php 
  }
?>
	 </td>
      </tr>
  </table>
     </div>
   </div>
</div>
<!-- fin de la table de recherche ou d'ajout de correspondant -->
<?php
if (isset($_REQUEST['envoyer'])) //ne pas faire de recherche si rien a chercher
{
  $critere['Nom']="Nom";
  $critere['Prenom']="Pr&eacute;nom";
  $critere['Adresse']="Adresse";
  $critere['Ville']="Ville";
  $critere['CodePostal']="Code Postal";
  if (isset($cle))
  {
    if ($exact==1)//Recherche par cle exacte des intervenants
      $sql=sprintf("SELECT * FROM Personnes WHERE $critere_recherche = '%s' AND Qualite LIKE '$Qualite' $type ORDER BY Qualite,Nom",mysqli_real_escape_string($cle));	
    else // si pas exact
    {
      $cle_courte=$cle;
      $cle="%".$cle."%";//On ajoute les Jokers
      $sql=sprintf("SELECT * FROM Personnes WHERE $critere_recherche LIKE '%s' AND Qualite LIKE '$Qualite' $type ORDER BY Qualite,Nom",mysqli_real_escape_string($db,$cle));	
    }
    $resultat=mysqli_query($db,$sql);
    $count=mysqli_num_rows($resultat);
?>

<div class="groupe">
 <h1>Recherche sur <strong><?php echo $cle ?></strong> par <strong><?php echo $critere[$critere_recherche] ?></strong> dans la sp&eacute;cialit&eacute; <strong><?php echo $Qualite ?></strong>&nbsp;: <?php echo $count ?> r&eacute;ponse<?php if ($count>1) echo "s" ?>
  </h1>
<?php
    if ($count) //s'il y a des correspondants trouves
    {
?>
      <table>
	<col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col><col></col>
	<tr>
	  <th class="fond_th">
	    Nom
	  </th>
	  <th class="fond_th">
	    Pr&eacute;nom
	  </th>
	  <th class="fond_th">
	    Sp&eacute;cialit&eacute;
	  </th>
	  <th class="fond_th">
	    Adresse
	  </th>
	  <th class="fond_th">
	    Code postal
	  </th>
	  <th class="fond_th">
	    Ville
	  </th>
	  <th class="fond_th">
	    T&eacute;l&eacute;phone
	  </th>
	  <th colspan="2" class="fond_th">
	    Intervenant
	  </th>
	  <th class="fond_th">
	    Patient
	  </th>
	</tr>
<?php

      while ($ligne=mysqli_fetch_array($resultat)) //boucle des fiches trouvees
      {
	$adr=preg_replace("`\n`","<br />",$ligne['Adresse']);

	echo "
	<tr style='vertical-align:top;'>
	  <td class=\"fond_td\">
	    <a href=\"fiche_intervenant.php?intervenant=".$ligne["ID_PrimKey"]."\">".stripslashes($ligne["Nom"])."</a>
	  </td>
	  <td class=\"fond_td\">
	    ".stripslashes($ligne["Prenom"])."
	  </td>
	  <td class=\"fond_td\">
	    ".stripslashes($ligne["Qualite"])."
	  </td>
	  <td class=\"fond_td\">
	    ".stripslashes($adr)."
	  </td>
	  <td class=\"fond_td\">
	    ".$ligne["CodePostal"]."
	  </td>
	  <td class=\"fond_td\">
	    ".stripslashes($ligne["Ville"])."
	  </td>
	  <td class=\"fond_td\">
	    ".$ligne["Tel_1"]."
	  </td>";
?>
	  <td class="noPrint fond_td">
	    <form action="formulaire_correspondant.php"  method="get"> <!--Bouton de modification de fiche--> 
	      <div>
		<input name="ID_corresp" type="hidden" value="<?php echo $ligne['ID_PrimKey']?>" />		
		<input name="envoyer" type="submit" value="Modifier" />
	      </div>
	    </form>
	  </td>
	  <td class="noPrint fond_td">
	    <form action="formulaire_correspondant.php"  method="get"><!--Bouton de suppression de fiche--> 
	      <div>
		<input name="ID_corresp" type="hidden" value="<?php echo $ligne['ID_PrimKey']?>" />
		<input name="envoyer" type="submit" value="Supprimer" />
	      </div>
	    </form>
	  </td>
	  <td class="noPrint fond_td">
	    <form action="liaison_correspondant.php"  method="get"><!--Bouton de liaison a un patient--> 
	      <div>
		<input name="ID_corresp" type="hidden" value="<?php echo $ligne['ID_PrimKey']?>" />
		<input name="envoyer" type="submit" value="Lier un nouveau patient" />
	      </div>
	    </form>
	  </td>
  <!-- fin zone non imprimable -->
	</tr>
<?php
      } //fin boucle des fiches trouvees
?>
      </table>

<?php
    }  //fin if count
//echo  '</fieldset>
echo  '</div>';
  } //fin $cle
} //fin si bouton chercher
} //fin si session

else //si pas logue
{
//On renvoie automatiquement sur la page de login
  header('location: index.php?page=correspondant' );
  exit;
}
  include("inc/footer.php");
?>

