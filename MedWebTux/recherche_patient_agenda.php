<?php
session_start() ;
//Page fille de l'agenda destinee a choisir un patient pour attribuer un rendez-vous
include("config.php");
include("inc/header.php");
if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=liste' );
  exit;
}
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

$Prenom="%";
$Nom="%";
$from=""; //transmis par le bouton reattribuer de consultation.php
if (isset($_GET['from']))
{
  $from=$_GET['from'];
  $Prenom='';
  $Nom='';
}

$Prenom='';
if (isset($_GET['Prenom']))
  $Prenom='%'.$_GET['Prenom'].'%';
$Nom='';
if (isset($_GET['Nom']))
  $Nom='%'.$_GET['Nom'].'%';
$DNaiss='';
if (isset($_GET['DNaiss'])) //format local
  $DNaiss=$_GET['DNaiss'];

if ($date_format=='fr')
{
//on repasse en iso les dates qui arrivent en francais
  $format='jj-mm-aaaa';
}
elseif ($date_format=='en')
  $format='mm-jj-aaaa';
else
  $format='aaaa-mm-jj';

include 'calendar_javascript.php';

?>
    <title>
      Agenda - Utilisateur <?php echo $_SESSION['login'] ?>
    </title>
    <script type="text/javascript">
<!--
function donner_focus(chp)
{
  document.getElementById(chp).focus();
}
-->
    </script>

    <script type="text/javascript">
//<![CDATA[
function choisir(nom,prenom,tel,GUID)
// on affecte la valeur (.value) dans :
// window.opener : la fenêtre appelante (celle qui a fait la demande)
// .document : son contenu
// .forms_x : le formulaire nomme
// .le champ 
// les valeurs attribuees proviennent du formulaire plus bas
{ 
  window.opener.document.forms['form_jour'].Nom.value = nom;
  window.opener.document.forms['form_jour'].Prenom.value = prenom;
  window.opener.document.forms['form_jour'].Tel.value = tel;
  window.opener.document.forms['form_jour'].GUID.value = GUID;
  // on se ferme
  self.close(); 
}
//]]>
    </script>


    <script type="text/javascript">
//<![CDATA[
function choisir_guid(nom,prenom,GUID) //pour la reattribution dans consultation.php
// on affecte la valeur (.value) dans :
// window.opener : la fenêtre appelante (celle qui a fait la demande)
// .document : son contenu
// .forms_x : le formulaire nomme
// .le champ 
// les valeurs attribuees proviennent du formulaire plus bas
{ 
  window.opener.document.forms['form_jour'].numeroID.value = GUID;
  window.opener.document.forms['form_jour'].Nom.value = nom;
  window.opener.document.forms['form_jour'].Prenom.value = prenom;
  window.opener.document.getElementById("submit_confirm").style.display = "inline"; //pour afficher le bouton de confirmation
  // on se ferme
  self.close(); 
}
//]]>
    </script>


  </head>
	
  <body style="font-size:<?php echo $fontsize; ?>pt"  onload="donner_focus('Nom')">
    <div class="conteneur">
    <div class="groupe">
      <h1>MedWebTux - Patients</h1>

<?php

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

if (($Nom!="%%" OR $Prenom!="%%") OR isset($_GET['DNaiss']))
{
  if ($DNaiss) //requetes si date de naissance fournie
  {
    $DNaiss=str_replace("/","-",$DNaiss); //si mauvais format fourni
    $list_date=explode ("-",$DNaiss);

    $date_iso=local_to_iso($DNaiss,$date_format);

    $DNaiss=$date_iso." 00:00:00";
    $sql_naissance="SELECT FchPat_GUID_Doss FROM fchpat WHERE FchPat_Nee='$DNaiss'";
    $resultat_naissance=mysqli_query($db,$sql_naissance);
    $count=mysqli_num_rows($resultat_naissance);

    echo "
      Recherche sur <b> ".$_GET['DNaiss']." </b> : ".$count." r&eacute;sultats";
  }
  else
  {
  //la requete si nom ou prenom
    $sql=sprintf("SELECT * FROM IndexNomPrenom WHERE FchGnrl_NomDos like '%s' AND FchGnrl_Prenom LIKE '%s' ORDER BY FchGnrl_NomDos,FchGnrl_Prenom",mysqli_real_escape_string($db,$Nom),mysqli_real_escape_string($db,$Prenom));
    $resultat=mysqli_query($db,$sql);
    $count=mysqli_num_rows($resultat);
    echo "
      Recherche sur <b>".$Nom." $Prenom </b> : ".$count." r&eacute;sultats";
  }
  if ($count) //affichage du tableau seulement si résultats
  {
?>
      <br /><br />
      <table>
	<tr>	
	  <th>
	    <form action="">
	      <div>	
		<input  value="" type="text"  size="8"/>
		<input  value="Nom" type="text"  size="10"/>
		<input  value="Pr&eacute;nom" type="text"  size="10"/>
		<input  value="Naissance" type="text"  size="5"/>
		<input  value="Adresse" type="text"  size="15"/>
		<input  value="Téléphone" type="text"  size="6"/>
	      </div>
	    </form>
	  </th>
	</tr>
<?php
  }
  else //Si aucun nom n'est fourni, on affiche un message d'aide
  { 
  ?>
	Veuillez donner un nom, un prénom ou une date de naissance valide<br />
<?php
  }
//  if (isset($_GET['DNaiss']) AND ) 
//  {
    if ($DNaiss)//si date de naissance fournie
    {
      while ($ligne_naissance=mysqli_fetch_array($resultat_naissance))
      {
	$GUID=$ligne_naissance['FchPat_GUID_Doss'];
	$sql="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$GUID'";
	$resultat=mysqli_query($db,$sql);
	$ligne=mysqli_fetch_array($resultat);
	$sql2="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$GUID'";
	$resultat2=mysqli_query($db,$sql2);
	$ligne2=mysqli_fetch_array($resultat2);
?>
	<tr>
	  <td>
	    <form id="form_identite" action="" >
	      <div>
<?php
	if ($from)//transmis par le bouton reattribuer de consultation.php
	{
?>
		<input type="submit" value="Valider" name="button_valid" onclick="choisir_guid(this.form.nom.value,this.form.prenom.value,this.form.GUID.value)" />
		<input type="hidden" value="consultation" name="from" />
<?php
	}
	else
	{
?>
		<input type="submit" value="Rendez-vous" name="rdv" onclick="choisir(this.form.nom.value,this.form.prenom.value,this.form.tel.value,this.form.GUID.value)" />
<?php
	}
?>
		<input readonly="readonly" type="text" value="<?php echo $ligne['FchGnrl_NomDos']?>" name="nom" size='10'/> 
		<input readonly="readonly" type="text" value="<?php echo $ligne['FchGnrl_Prenom']?>" name="prenom" size='10' />
		<input readonly="readonly" type="text" value="<?php echo$_GET['DNaiss'] ?>" size='5' />
		<input readonly="readonly" type="text" value="<?php echo str_replace ("\n","<br />",stripslashes($ligne2["FchPat_Adresse"]))?>" size='15' />
		<input readonly="readonly" type="text" value="<?php echo $ligne2['FchPat_Tel1']?>" name="tel" size='6' />
		<input type="text" value="<?php echo $ligne['FchGnrl_IDDos']?>" name="GUID" />
	      </form>
	    </div>
	  </td>
	</tr>
<?php
  }
  if ($count)
  {
?>
      </table>
 
<?php
  }
//}
}
else //mode nom-prenom
{
//marche pas car DNaiss fournie echo "test";
  $resultat=mysqli_query($db,$sql);
  $i=0;
  while ($ligne=mysqli_fetch_array($resultat)) 
  {
    $i++;
    $no_dossier=$ligne["FchGnrl_IDDos"];
    $sql2="SELECT * FROM fchpat WHERE FchPat_GUID_Doss='$no_dossier'";
    $resultat2=mysqli_query($db,$sql2);
    $ligne2=mysqli_fetch_array($resultat2);
    //formatage de la date de naissance
    $date_naissance_courte=explode (" ",$ligne2["FchPat_Nee"]);
    list ($annee,$mois,$jour) = explode ("-",$date_naissance_courte[0]);
?>
	<tr>
	  <td>
	    <form id="form_identite" action="">
	      <div>
<?php
if ($from)
{
?>
		<input type="submit" value="Changer" name="button_valid" onclick="choisir_guid(this.form.nom.value,this.form.prenom.value,this.form.GUID.value)" />
		<input type="hidden" name="from" value="consultation"/>
<?php
}
else
{
?>
		<input type="submit" value="Rendez-vous" name="rdv" onclick="choisir(this.form.nom.value,this.form.prenom.value,this.form.tel.value,this.form.GUID.value)" />
<?php
}
?>
		<input readonly="readonly" type="text" value="<?php echo $ligne['FchGnrl_NomDos']?>" name="nom" size='10'/>
		<input readonly="readonly" type="text" value="<?php echo $ligne['FchGnrl_Prenom']?>" name="prenom" size='10' />
		<input readonly="readonly" type="text" value="<?php echo $jour ?>-<?php echo $mois?>-<?php echo $annee?>" size='5' />
		<input readonly="readonly" type="text" value="<?php echo str_replace ("\n","<br />",stripslashes($ligne2["FchPat_Adresse"]))?>" size='15' />
		<input readonly="readonly" type="text" value="<?php echo $ligne2['FchPat_Tel1']?>" name="tel" size='6' />
		<input type="hidden" value="<?php echo $ligne['FchGnrl_IDDos']?>" name="GUID" />
	      </div>
	    </form>
	  </td>
	</tr>
<?php
  } //fin while
// on ferme la base
  mysqli_close($db);
  if ($count)
  {
?>
      </table>
    
<?php
    }
  } //fin mode nom
} //fin mode vide
else //Si aucun nom n'est fourni, on affiche un message d'aide
{ 
?>
      <p>Veuillez donner un nom ou un prénom valide</p>
<?php
}

?>
     <fieldset>
      <legend>Choix d'un dossier</legend>
      <table>
	<tr>
	  <td>
	    <form action="recherche_patient_agenda.php" method="get" id="form_recherche_patient">
	      <p>
		<label for="Nom">
		  <b>Nom : &nbsp;&nbsp;&nbsp;&nbsp;</b>
		</label>
		<input name="Nom" id="Nom" type="text" value="<?php echo str_replace('%','',$Nom) ?>" size="30" />
	      </p>
	      <p>
		<label for="Prenom">
		  <b>Prénom : </b>
		</label>
		<input name="Prenom" id="Prenom" type="text" value="<?php echo str_replace('%','',$Prenom) ?>" size="30" />
	      </p>
	      <p>
		Ou<br />
		<label for="DNaiss">
		  <b>Date de naissance (<?php echo $format ?>) : </b>
		</label>
		<input name="DNaiss" id="datepicker" type="text" value="<?php if (isset($_GET['DNaiss'])) echo $_GET['DNaiss']; ?>" size="10"  onchange="form.submit()" />
<!--
<input type="image" src="pics/calendar.png" alt="calendrier" onclick="return getCalendar(document.forms['form_recherche_patient'].DNaiss);" />
-->
<?php
if ($from)
{
?>
		<input type="hidden" name="from" value="consultation"/>
<?php
}
?>

	      </p>
	      <p>
		<input name="envoyer" type="submit" value="Chercher un patient" title="La recherche s'effectue sur n'importe quelle partie du nom et du prénom ou sur la date de naissance." />
	      </p>

	    </form>
	  </td>
	</tr>
      </table>
    </fieldset>
    </div>
<?php
include("inc/footer.php");
?>

