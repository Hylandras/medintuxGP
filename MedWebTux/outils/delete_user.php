<?php
session_start() ;
//page destinee a supprimer un utilisateur et tous ses documents

include("../config.php");
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion &agrave; $host<br>";
	exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

if ( !isset( $_SESSION['login'] ) )
{
//On renvoie automatiquement sur la page de login
  header('location: ../index.php?page=delete_user' );
  exit;
}
$user=$_SESSION['login']; //administrateur seulement !
$sql_recherche_droits="SELECT Droits FROM Personnes WHERE Login='$user'";
$resultat_recherche_droits=mysql_query($sql_recherche_droits,$db);
$ligne_recherche_droits=mysql_fetch_array($resultat_recherche_droits);
//	if (ereg("adm",$ligne_recherche_droits['Droits']))
$number_intervenants=0;

if (preg_match('/adm/',$ligne_recherche_droits['Droits']))
{
  if (isset ($_REQUEST['checkbox_delete'])) //on recupere les utilisateurs coches = a supprimer
  {
    $checkbox_delete=$_REQUEST['checkbox_delete'];
    $patients_detruits=0;
    foreach ($checkbox_delete AS $this_checkbox) //on examine les medecins un par un
    {
      echo $this_checkbox."<br>";
      $number_intervenants++;
      $number_doc=0;
  //on chercher les en-tetes des documents appartenant a cet utilisateur
      $sql_select_headers="SELECT * FROM RubriquesHead WHERE RbDate_CreateUser='$this_checkbox'";
//      echo $sql_select_headers.'<br>';
      $resultat_select_headers=mysql_query($sql_select_headers);
      $count_select_headers =mysql_num_rows($resultat_select_headers);
//      echo $count_select_headers.'<br>';
      while ($ligne_select_headers=mysql_fetch_array($resultat_select_headers)) //on analyse les en-tetes lies a ce medecin
      {
	$number_doc++;
	$this_header=$ligne_select_headers['RbDate_RefBlobs_PrimKey'];
	$sql_select_blobs="DELETE FROM RubriquesBlobs WHERE RbDate_PrimKey='$this_header'"; //on detruit le blob lie a l'en-tete
	$resultat_select_blobs=mysql_query($sql_select_blobs);
//	echo $sql_select_blobs.'<br>';
//	$count_select_blobs=mysql_num_rows($resultat_select_blobs); 
//	echo $count_select_blobs.'<br>';
	$guid=$ligne_select_headers['RbDate_IDDos'];
//on detruit le header
	$sql_delete_headers="DELETE FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$this_header'";
	$resultat_delete_headers=mysql_query($sql_delete_headers);
  //on cherche si ce patient a encore des documents
	$sql_select_headers2="SELECT * FROM RubriquesHead WHERE RbDate_IDDos='$guid'";
	$resultat_select_headers2=mysql_query($sql_select_headers2); 
	$count_select_headers2=mysql_num_rows($resultat_select_headers2);
	if ($count_select_headers==0) //si on n'a plus un seul document pour ce patient, on supprime le patient
	{
	  $patients_detruits++;
  //	echo "fiche a supprimer";
	  $sql_delete_patient="DELETE FROM IndexNomPrenom WHERE FchGnrl_IDDos='$guid'";
	  $resultat_delete_patient=mysql_query($sql_delete_patient);
	  $sql_delete_fchpat="DELETE FROM fchpat WHERE FchPat_GUID_Doss='$guid'";
	  $resultat_delete_fchpat=mysql_query($sql_delete_fchpat);
	  $sql_delete_fchpat_intervenants="DELETE FROM fchpat_Intervenants WHERE fchpat_Intervenants_PatGUID='$guid'";
	  $resultat_delete_fchpat_intervenants=mysql_query($sql_delete_fchpat_intervenants);
	  $sql_delete_fchpat_note="DELETE FROM fchpat_Note WHERE fchpat_Note_PatGUID='$guid'";
	  $resultat_delete_fchpat_note=mysql_query($sql_delete_fchpat_note);
	}
      }
      echo "docs détruits=$number_doc";
    $sql_delete_intervenants="DELETE FROM Personnes WHERE Login='$this_checkbox'";
echo $sql_delete_intervenants;
    $resultat_delete_intervenants=mysql_query($sql_delete_intervenants);
    }
//on detruit l'intervenant pour finir
  }
echo "patients détruits =$patients_detruits <br>";
echo "intervenants détruits= $number_intervenants<br>";

//module pour effacer des patients
//$i=0;
/*
for ($i=0;$i<4081;$i++)
{
echo "$i<br>";

$sql_delete_patient="DELETE FROM IndexNomPrenom WHERE FchGnrl_IDDos='$i'";
$resultat_delete_patient=mysql_query($sql_delete_patient);
$sql_delete_fchpat="DELETE FROM fchpat WHERE FchPat_GUID_Doss='$i'";
	  $resultat_delete_fchpat=mysql_query($sql_delete_fchpat);

}
*/
}
?>
<html>
<head>
	<META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=UTF-8">
	<META NAME="GENERATOR" CONTENT="PHP">
<title>MedWebTux - Conversion de base64</title>
</head>
<body>
<b>Attention !</b>
Cette page permet de supprimer totalement un utilisateur et ses documents attachés.<br>
Cochez les utilisateurs à supprimer
<?php
if (isset($REQUEST['checkbox_delete']))
{
$checkbox_delete=$REQUEST['checkbox_delete'];

}
      $sql="SELECT * FROM Personnes WHERE Login!=''";	//on cherche tous les utilisateuers
//echo $sql;
    $resultat=mysql_query($sql,$db);
    $count=mysql_num_rows($resultat);
    if ($count) //s'il y a des correspondants trouves
    {
?>	
<FORM ACTION="delete_user.php"  METHOD="GET"> <!--Bouton de modification de fiche--> 

    <center><table border=1>
<col><col><col><col><col><col><col><col><col><col>
    <tr>
      <th>Login</th>
      <th>Nom</th>
      <th>Pr&eacute;nom</th>
      <th>Sp&eacute;cialit&eacute;</th>
      <th>Adresse</th>
      <th>Code postal</th>
      <th>Ville</th>
      <th>T&eacute;l&eacute;phone</th>
      <th>Supprimer</th>

    </tr>
<?php

      while ($ligne=mysql_fetch_array($resultat)) //boucle des fiches trouvees
      {
	$adr=preg_replace("`\n`","<br>",$ligne['Adresse']);

	echo "<TR><td>".$ligne["Login"]."</td><td><a href=\"fiche_intervenant.php?intervenant=".$ligne["ID_PrimKey"]."\">".$ligne["Nom"]."</a></td><td>".$ligne["Prenom"]."</td><td>".$ligne["Qualite"]."</td><td>".$adr."</td><td>".$ligne["CodePostal"]."</td><td>".$ligne["Ville"]."</td><td>".$ligne["Tel_1"]."</td>";
?>
<td>
		<input name="checkbox_delete[]" type="checkbox" value="<?php echo $ligne["Login"]?>" />		
</td>
<!-- fin zone non imprimable -->
</tr>
<?php
      } //fin boucle des fiches trouvees
}// fin if count
?>
	</table></center>		
<input type="submit" value="Valider" />
	</form>

</body>
</html>