<?php
include("../config.php");
//page destinee a reaffecter en mode interactif les headers appartenant dates arbitrairement au 1-1-1970
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
if (isset($_REQUEST['bouton_valider']))
{
$Login=$_REQUEST['personne'];
$date=$_REQUEST['date']." 00:00:00";
$id_head=$_REQUEST['id_head'];
foreach ($id_head AS $cet_id_head)
{
$sql_modifier_head="UPDATE RubriquesHead SET Login='$Login',RbDate_Date='$date' WHERE RbDate_PrimKey='$cet_id_head' ";
echo $sql_modifier_head."<br>";
$resultat=mysql_query($sql_modifier_head);
}
}
?>
	<title>MedWebTux - Réparation MedinTux</title>
  <META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=utf-8">
</head>
<body style="font-size:<?php echo $fontsize ?>pt"  >
	<h1>MedWebTux -Réparation de headers perdus</h1>
Permet de changer le possesseur et la date du document où la date est 1er janvier 1970 par lots. Il suffit de donner une date dans la zone de saisie, de choisir un utilisateur dans le déroulant, de cocher les lignes à modifier, puis de 
	<br>
<form action="rename.php" method="GET" >
Nouvelle date : <input name="date" type="text" value="2010-01-01" /><br>
Nouvel utilisateur : 
<select name="personne">
<?php 
$sql_chercher_utilisateurs="SELECT * FROM Personnes WHERE Login !=''";
$resultat_chercher_utilisateurs=mysql_query($sql_chercher_utilisateurs);
while ($ligne_chercher_utilisateurs=mysql_fetch_array($resultat_chercher_utilisateurs))
{
?>
<option value="<?php echo $ligne_chercher_utilisateurs['Login'] ?>"><?php echo $ligne_chercher_utilisateurs['Login'] ?></option>
<?php
}
?>
</select>
<br>
<input name="bouton_valider" type="submit" value="Valider" />
<table>
<tr><th></th><th>nom</th><th>Prénom</th></tr>
<?php
$compteur=0;
$sql_chercher_date="SELECT * FROM RubriquesHead INNER JOIN IndexNomPrenom ON RubriquesHead.RbDate_IDDos=IndexNomPrenom.FchGnrl_IDDos WHERE RbDate_Date LIKE '1970-01-01%'";
$resultat_chercher_date=mysql_query($sql_chercher_date);
while ($ligne_chercher_date=mysql_fetch_array($resultat_chercher_date))
{
$compteur++;
?>
<tr><td><input type="checkbox" name="id_head[]" value="<?php echo$ligne_chercher_date['RbDate_PrimKey'] ?>"/></td><td><?php echo $ligne_chercher_date['FchGnrl_NomDos']?></td><td><?php echo $ligne_chercher_date['FchGnrl_Prenom']?></td></tr>
<?php
}
 ?>
</table>
<input name="bouton_valider" type="submit" value="Valider" />
</form>
</body>
</html>