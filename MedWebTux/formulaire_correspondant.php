<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=correspondant' );
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
if (isset($_GET['ID_corresp']))
{	
  $ID_corresp=$_GET['ID_corresp'];
  $sql="select * from Personnes where ID_PrimKey='$ID_corresp'";
  $resultat=mysqli_query($db,$sql);
  $ligne=mysqli_fetch_array($resultat);
}
if (isset($_GET['envoyer']))
  $Envoyer=$_GET['envoyer'];
if (isset($_GET['modif']))
  $modif=$_GET['modif'];
if ($Envoyer=="Modifier" or $Envoyer=="Supprimer")
{
  $nom=$ligne['Nom'];
  $prenom=$ligne['Prenom'];
  $Adresse=$ligne['Adresse'];
  $Tel_1=$ligne['Tel_1'];
  $Tel_2=$ligne['Tel_2'];
  $Tel_3=$ligne['Tel_3'];
  $Tel_Type1=$ligne['Tel_Type1'];
  $Tel_Type2=$ligne['Tel_Type2'];
  $Tel_Type3=$ligne['Tel_Type3'];
  $Tel_Abr_1=$ligne['Tel_Abr_1'];
  $Tel_Abr_2=$ligne['Tel_Abr_2'];
  $Tel_Abr_3=$ligne['Tel_Abr_3'];
  $CP=$ligne['CodePostal'];
  $Ville=$ligne['Ville'];
  $EMail=$ligne['EMail'];
  $Note=$ligne['Note'];
  $Qualite=$ligne['Qualite'];
  $Titre=$ligne['Titre'];
  $ID_PrimKey=$ligne['ID_PrimKey'];
  $OrdreCorresp=$ligne['NumOrdre'];
  $RPPSCorresp=$ligne['NumRPPS'];
  $sexe_intervenant=$ligne['Sexe'];
  $politesse=$ligne['Cher'];
}
elseif ($Envoyer=="Nouveau")
{
  $nom="";
  $prenom="";
  $Adresse="";
  $Tel_1="";
  $Tel_2="";
  $Tel_3="";
  $Tel_Type1="";
  $Tel_Type2="";
  $Tel_Type3="";
  $Tel_Abr_1="";
  $Tel_Abr_2="";
  $Tel_Abr_3="";

  $CP=$cp_default;
  $Ville=$ville_default;
  $EMail="";
  $Note="";
  $Qualite="";
  $Titre="";
  $ID_PrimKey="";
  $OrdreCorresp="";
  $RPPSCorresp="";
  $sexe_intervenant="";
  $politesse="";
}

	include("inc/header.php");
?>
    <title>
      MedWebTux - Intervenants - Utilisateur <?php echo $_SESSION['login'] ?>
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
<!--
function zip_code(value)
// on ouvre dans une fenêtre le fichier passé en paramètre.
{
window.open('zipcode.php?code='+value,'Codepostal','width=450,height=250,top=50,left=50,toolbar=yes, scrollbars=yes, location=no');
}
-->
</script>
<script type="text/javascript">
<!--
function city(value)
// on ouvre dans une fenêtre le fichier passé en paramètre.
{
window.open('city.php?city='+value,'Ville','width=450,height=250,top=50,left=50,toolbar=yes, scrollbars=yes, location=no');
}
-->
</script>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" onload="donner_focus('titre_corresp')" >
    <div class="conteneur">
<?php	
// insertion du menu d'en-tete	
$anchor='modification_de_fiche_d_intervenant';
include("inc/menu-horiz.php");
?>
    <div class="groupe">
      <h1>MedWebTux - Intervenants</h1>
<?php
if (isset($_GET['modif']))
{
  if ($modif=="oui")
  {
?>
    <h2 style="text-align:center;">
      Modification effectu&eacute;e
    </h2>
<?php
  }
}
if ($Envoyer=="Modifier")
{
?>
    <p  style="text-align:center;">
      <a href="fiche_intervenant.php?intervenant=<?php echo $ID_PrimKey ?>">Acc&eacute;der &agrave; la fiche de <?php echo $nom." ".$prenom ?></a>
    </p>
<?php
}
if ($Envoyer=="Nouveau" or $Envoyer=="Modifier")
{
?>
<!--
   <div class="tableau">
-->
    <fieldset>
      <legend>
	Nouvel intervenant
      </legend>
      <form action="validation_correspondant.php" method="get" id="form_general">
	<table>
<!--
	  <tr>
	    <td>
	      <table>
-->
		<tr>
		  <th>
		    <label for="titre_corresp">
		      Titre :
		    </label> 
		    <input name="titre_corresp" id="titre_corresp" type="text" value="<?php echo $Titre?>" size="4" maxlength="50" />
		  </th>
		</tr>
		<tr>
		  <th>
		    <label for="nom_corresp">
		      Nom : 
		    </label>
		    <input name="nom_corresp" id="nom_corresp" type="text" value="<?php echo $nom?>" size="20" maxlength="60" />
		  </th>
		</tr>
		<tr>
		  <th>
		    <label for="prenom_corresp">
		      Pr&eacute;nom : 
		    </label>
		    <input name="prenom_corresp" id="prenom_corresp" type="text" value="<?php echo $prenom?>" size="20" maxlength="60" />
		  </th>
		</tr>
<!--
	      </table>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      <table>
-->
		<tr>
		  <th>
		    <label for="liste_spe">
		      Sp&eacute;cialit&eacute; : 
		    </label>	
		    <select name="liste_spe" id="liste_spe" >
		      <option value="Autre">
			Autre
		      </option>
<?php
  $sql_specialite="SELECT Qualite FROM Personnes GROUP BY Qualite ORDER BY Qualite";
  $resultat_specialite=mysqli_query($db,$sql_specialite);
	
  while ($ligne_specialite=mysqli_fetch_array($resultat_specialite))
  { 
    if ($ligne_specialite['Qualite'])
    {
?>
		      <option value="<?php echo $ligne_specialite['Qualite'];?>" <?php if ($ligne_specialite['Qualite']==$Qualite) echo "selected='selected'" ?> >
			<?php echo $ligne_specialite['Qualite'] ?>
		      </option>
<?php
    }
  }
?>
		    </select>
		    <input name="specialite" type="text" value="" title="Ne sert que si le déroulant est sur Autre - 40 caractères maxi" size="20" maxlength="40" />
		  </th>
		</tr>
		<tr>
		  <th>
		    <label for="ordre_corresp">
		      Num&eacute;ro ADELI: 
		    </label>
		    <input name="ordre_corresp" id="ordre_corresp" type="text" value="<?php echo $OrdreCorresp ?>" size="10" maxlength="15" />
		</th>
		</tr>
		<tr>
		<th>
		    <label for="rpps_corresp">
		      RPPS : 
		    </label>
		    <input name="rpps_corresp" id="rpps_corresp" type="text" value="<?php echo $RPPSCorresp ?>" size="10" maxlength="15" />
		</th>
		</tr>
		<tr>
		<th>
		    <label for="sexe_intervenant">
		      Sexe : 
		    </label>
		    <input type="radio" <?php if ($sexe_intervenant=="M") echo "checked='checked'" ?> name="sexe_intervenant" id="sexe_intervenant" value="M" />Masculin
		    <input type="radio" <?php if ($sexe_intervenant=="F") echo "checked'checked'" ?> name="sexe_intervenant" value="F" />F&eacute;minin
		</th>
		</tr>
		<tr>
		<th>
		    <label for="politesse">
		      Politesse : 
		    </label>
		    <input name="politesse" id="politesse" type="text" value="<?php echo $politesse ?>" title="Exemple : Cher confrère"/>
		  </th>
		</tr>
<!--
	      </table>
	    </td>
	  </tr>	
-->	
	  <tr>
	    <th valign="top">
	      <label for="Adresse_corresp">
		Adresse : 
	      </label>
	      <textarea name="Adresse_corresp" id="Adresse_corresp" rows="3" cols="60" ><?php echo $Adresse ?></textarea>
	    </th>
	  </tr>
<!--
	  <tr>
	    <td>
	      <table>
-->
		<tr>
		  <th>
		    <label for="CP">
		      Code postal : 
		    </label>
		    <input name="CP" id="CP" type="text" value="<?php echo $CP ?>" size="5" maxlength="5" onchange="zip_code(this.value)"/>
		  </th>
		</tr>
		<tr>
		  <th>
		    <label for="Ville">
		      Ville : 
		    </label>
		    <input name="Ville" id="Ville" type="text" value="<?php echo $Ville ?>" size="40"  maxlength="128" onchange="city(this.value)" />
		  </th>
		</tr>
<!--
	      </table>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      <table>
-->
		<tr>
		  <th valign="top">
		    <label for="mail_corresp">
		      Mail : 
		    </label>
		    <input name="mail_corresp" id="mail_corresp" type="text" value="<?php echo $EMail ?>" size="40" maxlength="128" />
	<?php
	$sql_tel_type1="SELECT Tel_Type1 FROM Personnes GROUP BY Tel_Type1 ORDER BY Tel_Type1";
	$resultat_tel_type1=mysqli_query($db,$sql_tel_type1);
?>
		    <br />
		    <label for="tel1_corresp">
		      Tel1 : 
		    </label>
		    <input name="tel1_corresp" id="tel1_corresp" type="text" value="<?php echo $Tel_1 ?>" size="10" maxlength="20" />
		    <label for="tel1_abr_corresp">
		      Abr : 
		    </label>
		    <input name="tel1_abr_corresp" id="tel1_abr_corresp" type="text" value="<?php echo $Tel_Abr_1 ?>" title="Num&eacute;ro abr&eacute;g&eacute;" size="5" maxlength="20" />
		    <label for="liste_type_tel1">
		      Type :
		    </label>
		    <select name="liste_type_tel1" id="liste_type_tel1" >
		      <option  value="Autre">
			Autre
		      </option>
<?php
  while ($ligne_tel_type1=mysqli_fetch_array($resultat_tel_type1) )//On recupere les differents types de tel
  {
    if ($ligne_tel_type1['Tel_Type1'])//ON ELIMINE LES LIGNES VIDES
    {

?>
		      <option <?php if ($Tel_Type1==$ligne_tel_type1['Tel_Type1']) echo "selected='selected'" ?> value="<?php echo $ligne_tel_type1['Tel_Type1']?>">
			<?php echo $ligne_tel_type1['Tel_Type1']?>
		      </option>
<?php
    }
  }
?>				
		    </select>
		    <label for="tel1_type_corresp">
		      Si Autre, pr&eacute;ciser :
		    </label>
		    <input name="tel1_type_corresp" id="tel1_type_corresp" type="text" value="" size="5" maxlength="20" />
<br />
<?php
  $sql_tel_type2="SELECT Tel_Type2 FROM Personnes GROUP BY Tel_Type2 ORDER BY Tel_Type2";
  $resultat_tel_type2=mysqli_query($db,$sql_tel_type2);
?>

		    <label for="tel2_corresp">
		      Tel2 : 
		    </label>
		    <input name="tel2_corresp" id="tel2_corresp"  type="text" value="<?php echo $Tel_2 ?>" size="10" maxlength="20" />
		    <label for="tel2_abr_corresp">
		      Abr : 
		    </label>
		    <input name="tel2_abr_corresp" id="tel2_abr_corresp" type="text" value="<?php echo $Tel_Abr_2 ?>"  size="5" maxlength="20"/>
		    <label for="liste_type_tel2">
		      Type : 
		    </label>
		    <select name="liste_type_tel2" id="liste_type_tel2">
		      <option  value="Autre">
			Autre
		      </option>
<?php
  while ($ligne_tel_type2=mysqli_fetch_array($resultat_tel_type2) /* AND  $ligne_tel_type2*/ )
  {	
    if ($ligne_tel_type2['Tel_Type2'])//ON ELIMINE LES LIGNES VIDES
    {
?>
		      <option <?php if ($Tel_Type2==$ligne_tel_type2['Tel_Type2']) echo "selected='selected'" ?> value="<?php echo $ligne_tel_type2['Tel_Type2']?>">
			<?php echo $ligne_tel_type2['Tel_Type2']?>
		      </option>
<?php
    }
  }
?>				
		    </select>
		    <label for="tel2_type_corresp">
		      Si Autre, pr&eacute;ciser :
		    </label>
		    <input name="tel2_type_corresp" id="tel2_type_corresp" type="text" value="" size="5" maxlength="20" />
		    <br />
<?php
  $sql_tel_type3="SELECT Tel_Type3 FROM Personnes GROUP BY Tel_Type3 ORDER BY Tel_Type3";
  $resultat_tel_type3=mysqli_query($db,$sql_tel_type3);
?>

		    <label for="tel3_corresp">
		      Tel3 : 
		    </label>
		    <input name="tel3_corresp" id="tel3_corresp" type="text" value="<?php echo $Tel_3 ?>" size="10" maxlength="20" />
		    <label for="tel3_abr_corresp">
		      Abr : 
		    </label>
		    <input name="tel3_abr_corresp" id="tel3_abr_corresp" type="text" value="<?php echo $Tel_Abr_3 ?>"  size="5" maxlength="20"/>
		    <label for="liste_type_tel3">
		      Type : 
		    </label>
		    <select name="liste_type_tel3" id="liste_type_tel3" >
		      <option  value="Autre">
			Autre
		      </option>
<?php
  while ($ligne_tel_type3=mysqli_fetch_array($resultat_tel_type3) )
  {
    if ($ligne_tel_type3['Tel_Type3'])//ON ELIMINE LES LIGNES VIDES
    {
?>
		      <option <?php if ($Tel_Type3==$ligne_tel_type3['Tel_Type3']) echo "selected='selected'" ?> value="<?php echo $ligne_tel_type3['Tel_Type3']?>">
			<?php echo $ligne_tel_type3['Tel_Type3']?>
		      </option>
<?php
    }
  }
?>				
		    </select>
		    <label for="tel3_type_corresp">
		      Si Autre, pr&eacute;ciser :
		    </label>
		    <input name="tel3_type_corresp" id="tel3_type_corresp" type="text" value="" size="5" maxlength="20" />
		    <br />
		  </th>
		</tr>
<!--
	      </table>
	    </td>
	  </tr>
-->
	  <tr>
	    <th>
	      <label for="Notes_corresp">
		Notes : 
	      </label>
	      <textarea name="Notes_corresp" id="Notes_corresp" rows="3" cols="60"><?php echo $Note ?></textarea>
	    </th>
	  </tr>
	  <tr>
	    <td>
	      <input name="envoyer" type="submit" value="Ajouter" />
<?php
if ($Envoyer=="Modifier")
{
?>
	      <input name="ID_PrimKey" type="hidden" value="<?php echo $ID_PrimKey ?>" />
	      <input name="envoyer" type="submit" value="Modifier" />
	      <input name="reset" type="reset" value="R&eacute;initialiser" />
<?php
}
?>
	    </td>
	  </tr>
	</table>
      </form>
   </fieldset>
  </div>
<?php
}
elseif ($Envoyer=="Supprimer")
{
//On cherche si patients liés
  $sql2="SELECT * FROM fchpat_Intervenants INNER JOIN IndexNomPrenom ON fchpat_Intervenants.fchpat_Intervenants_PatGUID=IndexNomPrenom.FchGnrl_IDDos WHERE fchpat_Intervenants_IntervPK='$ID_corresp' ORDER BY FchGnrl_NomDos,FchGnrl_Prenom" ;

  $resultat2=mysqli_query($db,$sql2);
  $count2=mysqli_num_rows($resultat2);
//On cherche si l'intervenant est un utilisateur

?>
  <div class="groupe">
      <h1>
	Mode suppression
      </h1>
      <b>Confirmez-vous la suppression de la fiche de <a href="fiche_intervenant.php?intervenant=<?php echo $ID_PrimKey ?>"><?php echo stripslashes($nom)." ".stripslashes($prenom) ?> </a></b>, lié à <?php echo $count2 ?> patients ?
      <br />
<?php
  if ($ligne['Login'])
  {
?>
  <div class="information">
    Avertissement : cet intervenant est aussi un utilisateur du logiciel !<br />
<?php
//On cherche s'il possède des documents
    $intervenant_user=$ligne['Login'];
    $sql_documents_create="SELECT * FROM RubriquesHead WHERE RbDate_CreateUser='$intervenant_user'";
    $resultat_documents_create=mysqli_query($db,$sql_documents_create);
    $count_documents_create=mysqli_num_rows($resultat_documents_create);
    $sql_documents_sign="SELECT * FROM RubriquesHead WHERE RbDate_CreateSignUser='$intervenant_user'";
    $resultat_documents_sign=mysqli_query($db,$sql_documents_sign);
    $count_documents_sign=mysqli_num_rows($resultat_documents_sign);
?>
Nombre de documents créés : <?php echo $count_documents_create ?><br />
Nombre de documents signataire : <?php echo $count_documents_sign ?><br />
Ces documents n'auront plus de propriétaire !
  </div>
<?php
//On cherche s'il possède des rendez-vous
    $sql_rdv="SELECT * FROM agenda WHERE RDV_PrisAvec='$intervenant_user'";
    $resultat_rdv=mysqli_query($db,$sql_rdv);
    $count_rdv=mysqli_num_rows($resultat_rdv);

    if ($count_rdv)
    {
?>
  <div class="information">
Cet utilisateur a <?php echo $count_rdv ?> rendez-vous.
  </div>
<?php
    }
  }
  if ($count2)
  {
    echo "<br /><strong>Patients liés :</strong><br />";
    while ($ligne2=mysqli_fetch_array($resultat2))
    {
      echo "<a href=\"patient.php?GUID=".$ligne2['FchGnrl_IDDos']."\">".$ligne2['FchGnrl_NomDos']."</a> ".$ligne2['FchGnrl_Prenom']."<br />";
    }
  }
?>
<br /><br />
      <form action="validation_correspondant.php" method="get">
	<div>
	  <input name="ID_PrimKey" type="hidden" value="<?php echo $ID_PrimKey ?>" />
	  <input name="Nom" type="hidden" value="<?php echo $nom ?>" />
	  <input name="confirmer" type="submit" value="Supprimer" />
	  <input name="confirmer" type="submit" value="Conserver" />
	</div>
      </form>
      <p>
	<b>Attention ! Cette action est irr&eacute;versible et entra&icirc;ne une perte d&eacute;finitive de donn&eacute;es.</b>
      </p>
  </div>
 </div>
<?php
} //fin supprimer
?>

<?php
include("inc/footer.php");
?>
