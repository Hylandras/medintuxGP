<?php
include("../config.php");
//moulinette destinee a reparer la perte des headers perdus
//on recree un header pour chaque blob orphelin, en attribuant un utilisateur arbitraire et une date arbitraire
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");
if(!$db)
{
	print "Erreur de connexion &agrave; $db<br>";
	exit;
}

// on choisit la bonne base
if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($db);
	exit;
}

?>
	<title>MedWebTux - Réparation MedinTux</title>
  <META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=utf-8">
</head>
<body style="font-size:<?php echo $fontsize ?>pt"  >
	<h1>MedWebTux -Réparation de headers perdus</h1>
	<br>
<div class="conteneur">
Cette page permet de reconstituer des headers perdus, afin de réattribuer les documents orphelins.
La date et l'utilisateur sont arbitraires (01-01-1970 et admin)<br>
<?php	
$compteur_total=0;
$compteur_recup=0;
$sql_chercher_blobs="SELECT * FROM RubriquesBlobs";
$resultat_chercher_blobs=mysql_query($sql_chercher_blobs);
echo "nombre blobs=".mysql_num_rows($resultat_chercher_blobs)."<br>";
$sql_chercher_headers="SELECT * FROM RubriquesHead";
$resultat_chercher_headers=mysql_query($sql_chercher_headers);
echo "nombre headers=".mysql_num_rows($resultat_chercher_headers)."<br>";
//on analyse toutes les consultations
while ($ligne_chercher_blobs=mysql_fetch_array($resultat_chercher_blobs))
{
  $compteur_total++;
  $guid_patient=$ligne_chercher_blobs['RbDate_IDDos'];
//code test pour voir s'il existe des blobs orphelins
  $sql_chercher_indexnomprenom="SELECT * FROM IndexNomPrenom";
  $resultat_chercher_indexnomprenom=mysql_query($sql_chercher_indexnomprenom,$db);
  $ligne_chercher_indexnomprenom=mysql_fetch_array($resultat_chercher_indexnomprenom);
  $guid_patient==$ligne_chercher_indexnomprenom['FchGnrl_IDDos']; // pas termine
  $id_blob=$ligne_chercher_blobs['RbDate_PrimKey']; // la cle de liaison blob-head
  $sql_chercher_si_existe_deja="SELECT * FROM RubriquesHead WHERE RbDate_RefBlobs_PrimKey='$id_blob'";
  echo $sql_chercher_si_existe_deja."<br>";
  $resultat_chercher_si_existe_deja=mysql_query($sql_chercher_si_existe_deja);
  if (!$count_chercher_si_existe_deja=mysql_num_rows($resultat_chercher_si_existe_deja)) //si le header n'existe pas et que le blob existe
  {

    $sql_chercher_num_dossier="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$guid_patient'";
    echo $sql_chercher_num_dossier."<br>";
    $resultat_chercher_num_dossier=mysql_query($sql_chercher_num_dossier,$db);
    $ligne_chercher_num_dossier=mysql_fetch_array($resultat_chercher_num_dossier);
    $num_dossier=$ligne_chercher_num_dossier['ID_PrimKey'];

    $sql_inserer_head="INSERT INTO RubriquesHead (RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_Ref_NumDoss,RbDate_RefBlobs_PrimKey) VALUES ('$guid_patient','20030000','MMG14','1970-01-01 00:00:00','admin','$num_dossier','$id_blob')";
    echo $sql_inserer_head."<br>";
    $compteur_recup++;
//    $resultat_inserer_head=mysql_query($sql_inserer_head,$db);
  } //fin if : on n'effectue rien d'autre s'il existe deja un header
}
echo "récupération terminée<br>";
echo "total base : ".$compteur_total."<br>";
echo "recuperes : ".$compteur_recup;
  include("../inc/footer.php");
?>
</div>
</body>
</html>