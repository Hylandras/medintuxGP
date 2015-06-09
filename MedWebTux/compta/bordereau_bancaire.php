<?php
// On demarre la session
session_start();
$loginOK = false;  // cf Astuce
if ( !isset( $_SESSION['login'] ) )
{
	header('location: ../index.php?page=liste' );
	exit;
}
//$user=$_SESSION['login'];

$tab_login=explode("::",$_SESSION['login']);
//$user=$tab_login[0];
$sign=$tab_login[1];

include("../config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
	print "Erreur de connexion &agrave; la base<br>";
	exit;
}

// on se connecte a drtuxtest
if (!mysqli_select_db($db,$base))
{
	print "Erreur ".mysqli_error($db)."<br>";
	mysqli_close($db);
	exit;
}

//On recupere les ID du medecin
$sql_chercher_user="SELECT * FROM Personnes WHERE Login='$sign'";
$resultat_chercher_user=mysqli_query($db,$sql_chercher_user);
$ligne_chercher_user=mysqli_fetch_array($resultat_chercher_user);
$id_drtux_user=$ligne_chercher_user['ID_PrimKey'];
//on se connecte à la compta
if (!mysqli_select_db($db,$base_compta))
{
	print "Erreur ".mysqli_error($db)."<br>";
	mysqli_close($db);
	exit;
}
$sql_chercher_utilisateur="SELECT * FROM utilisateurs WHERE id_drtux_usr='$id_drtux_user'";
$resultat_chercher_utilisateur=mysqli_query($db,$sql_chercher_utilisateur);
$count_chercher_utilisateur=mysqli_num_rows($resultat_chercher_utilisateur);
$ligne_chercher_utilisateur=mysqli_fetch_array($resultat_chercher_utilisateur);
$id_user=$ligne_chercher_utilisateur['id_usr'];
$praticien=$ligne_chercher_utilisateur['nom_usr'];

//URL
//bordereau_bancaire.php?id_hono[]=1033&id_hono[]=1034&
if (isset($_REQUEST['liste_comptes']))
{
//echo "test";
//$connect=mysqli_select_db($db,$base_compta);
  $liste_comptes=$_REQUEST['liste_comptes'];
//echo $liste_comptes;
  $sql_chercher_comptes="SELECT * FROM comptes_bancaires WHERE id_compte='$liste_comptes'";
  $resultat_chercher_comptes=mysqli_query($db,$sql_chercher_comptes);
  $ligne_chercher_comptes=mysqli_fetch_array($resultat_chercher_comptes);

}
if (isset($_REQUEST['id_hono']))
{
  $id_hono=$_REQUEST['id_hono']; // le tableau des lignes d'honoraires transmis

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
  <head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
    <meta name="generator" content="php" />
    <title>
      MedWebTux - Bordereau bancaire
    </title>
    <link rel="stylesheet" media="print" href="css/impression.css" />
  </head>
  <body style="font-size:10pt" >
  <!-- <div id="printhead"> -->
    <table style="font-size:10pt">
      <tr>
	<td>
	  <?php echo $ligne_chercher_user['Titre']." ".$ligne_chercher_user['Nom']." ".$ligne_chercher_user['Prenom']?>
	</td>
      </tr>
      <tr>
	<td>
	  <?php echo $ligne_chercher_user['Adresse'] ?>
	</td>
      </tr>
      <tr>
	<td>
	  <?php echo $ligne_chercher_user['CodePostal']." ".$ligne_chercher_user['Ville']?>
	</td>
      </tr>
    </table>
    <p>
<?php
  echo "
      ".$ligne_chercher_comptes['nom_banque']."<br />Compte n° ".$ligne_chercher_comptes['rib_numcompte'];
?>
    </p>
<!-- </div> -->
<!-- <div id="printbody"> -->
    <h1>
      Bordereau de remise de chèques
    </h1>
    <p>
<?php
$date=date('d-m-Y', date('U')); // la date du jour
echo "
      Remise de chèques le ".$date;
?>
    </p>
    <table style="font-size:15px">

<?php
$connect=mysqli_select_db($db,$base_compta);
$i=0;
$total=0;
  foreach ($id_hono AS $cet_hono)
  {
    $i++;
    $sql_afficher_hono="SELECT * FROM honoraires  WHERE id_hono='$cet_hono'";
    $resultat_afficher_hono=mysqli_query($db,$sql_afficher_hono);
//$count_afficher_hono=mysqli_num_rows($resultat_afficher_hono);
    $ligne_afficher_hono=mysqli_fetch_array($resultat_afficher_hono);
    $total=$total+$ligne_afficher_hono['chq'];
    $remarque=$ligne_afficher_hono['remarque'];
    $elements_remarque =explode("=",$remarque) ;
    //echo $elements_remarque[1]."<br>";
$date_tableau=explode ("-",$ligne_afficher_hono['date']);
$date_francaise=$date_tableau[2]."-".$date_tableau[1]."-".$date_tableau[0];
    echo "
      <tr>
	<td>
	  ".$date_francaise."
	</td>
	<td>
      ".$elements_remarque[1];
    if ($elements_remarque[1]!=$ligne_afficher_hono['patient'])
      echo " (".$ligne_afficher_hono['patient'].")";
    echo "
	</td>
	<td>
	  ".$ligne_afficher_hono['chq']."
	</td>
      </tr>";
  }
?>
    </table>
    <p>
      Nombre de chèques : <?php echo $i ?>
    </p>
    <p>
      Total : <?php echo $total ?> €
    </p>
<!-- </div> -->
  </body>
</html>
<?php
} // recherche hono
?>