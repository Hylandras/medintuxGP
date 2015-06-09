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
//On cherche dans la table IndexNomPrenom la fiche dont le nom a ete demande pour la barre de titre
$sql="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$patient'";
$resultat=mysqli_query($db,$sql);
$ligne=mysqli_fetch_array($resultat);
$nom=$ligne["FchGnrl_NomDos"];
$prenom=$ligne["FchGnrl_Prenom"];
$date=date('Y-m-d'); //la date courante 
include("inc/header.php");
?>
    <title>
       <?php echo $nom.' '.$prenom ?> Utilisateur <?php echo $_SESSION['login'] ?>
    </title>

<frameset rows="40,*"> 
  <frame src="topframe.php" name="top" id="top" title="menu horizontal" noresize frameborder="0">
  <frameset cols="305, *">
    <frame src="patient.php?GUID=<?php echo $patient?>" name="patient" title="partie constante" noresize frameborder="0">
    <frame src="consultation.php?numeroID=<?php echo $patient?>&amp;date=<?php echo $date?>&amp;edition=Aujourd'hui" name="droit_bas" title="partie variable" noresize frameborder="0">
  </frameset> 
</frameset>

<noframes>
<body>
Votre navigateur doit accepter les frames.
</body>
</noframes>
</html>
