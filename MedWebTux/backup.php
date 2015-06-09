<?php
// On demarre la session
session_start();
$loginOK = false;  // cf Astuce

include("config.php");
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");
//$base3="DatasempTest";
$baseCCAM="CCAMTest";


if(!$db)
{
  print "Erreur de connexion &agrave; la base<br />";
  exit;
}

// on choisit la bonne base

if (!mysql_select_db($base,$db))
{
  print "Erreur ".mysql_error()."<br />";
  mysql_close($db);
  exit;
}

include("inc/header.php");
?>
    <title>
      MedWebTux - Utilitaires pour Medintux
    </title>
<link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />
  </head>
  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur">
	<?php	
// insertion du menu d'en-tete	
include("inc/menu-horiz.php");
		
?>
      <h1>
	MedWebTux - Utilitaires d'administration
      </h1>
<?php
if (!fopen ('log/test','w')) //on fait un test d'ecriture. Si marche pas, on revoie un message d'erreur
{
  $directory=explode("/",getenv("SCRIPT_URL"));
  echo "La sauvegarde est impossible, car elle nécessite un droit en écriture pour l'utilisateur apache (parfois nommé www-data) dans le répertoire ".getenv("DOCUMENT_ROOT")."/".$directory[1]."/log/";
}
else //si le test d'ecriture a marche, on affiche la page
{
  if ( isset( $_SESSION['login'] ) )
  {
    $tab_login=explode("::",$_SESSION['login']);
    $user=$tab_login[0];
    $sql_recherche_droits="SELECT Droits FROM Personnes WHERE Login='$user'";
    $resultat_recherche_droits=mysql_query($sql_recherche_droits,$db);
    $ligne_recherche_droits=mysql_fetch_array($resultat_recherche_droits);
    if (stripos($ligne_recherche_droits['Droits'],'adm'))
    {
      
//SAUVEGARDE DE LA BASE patients  --------------------------------------------
      exec("mysqldump -h$host -u$loginbase -p$pwd $base > log/Dump_DrTuxTest.sql");
//SAUVEGARDE DE LA BASE CCAM  --------------------------------------------
      exec("mysqldump -h$host -u$loginbase -p$pwd $baseCCAM > log/Dump_CCAMTest.sql");
//Sauvegarde de la compta
      if (!mysql_select_db($base_compta,$db))
      {
	print "Erreur ".mysql_error()."<br />";
	mysql_close($db);
	exit;
      }

      exec("mysqldump -h$host -u$loginbase -p$pwd $base_compta > log/comptabilite.sql");

//sauvegarde parametres des médicaments
      if (!mysql_select_db($base2,$db))
      {
	print "Erreur ".mysql_error()."<br />";
	mysql_close($db);
	exit;
      }

      exec("mysqldump -h$host -u$loginbase -p$pwd $base2 > log/Dump_MedicaTuxTest.sql");
      mysql_close($db);
// 	exec("mysqldump -h$host -u$loginbase -p$pwd $base3 > log/Dump_DatasempTest.sql");
// 		mysql_close($db);

		
?>
      <a href="log/Dump_DrTuxTest.sql">Enregistrez ce lien pour sauvegarder les donn&eacute;es de MedinTux sur votre disque local</a>.<br /><br />
      Pour restaurer la base, utilisez la commande mysql suivante :<br />
      <code>$ mysql -u root --password=xxx DrTuxTest &lt; Dump_DrTuxTest.sql</code><br />
      Si vous avez un message d'erreur comme ERROR 1153 (08S01) at line 137: Got a packet bigger than 'max_allowed_packet' bytes, augmentez la valeur de max_allowed_packet                      = 32M (au lieu de 1)
      dans /etc/my.cnf.

<?php
    }
/*else
{
?>
<table align="center">
<tr>
<form action="backup.php" method="POST">
<td><input name="user" type="text" value="root" /></td></tr>
<tr><td><input name="pass" type="password" value="" /></td></tr>
<tr><td><input name="mdp" type="submit" value="Valider" /></td>
</tr>
</form>
</table>
<?php
}*/
?>

      <br /><br /><a href="log/Dump_CCAMTest.sql">Enregistrez ce lien pour sauvegarder les donn&eacute;es du thesaurus CCAM</a>.
      <br />
      Pour restaurer la base, utilisez la commande mysql suivante :
      <br /><code>$ mysql -u root --password=xxx CCAMTest &lt; Dump_CCAMTest.sql</code>

      <br /><a href="log/comptabilite.sql">Enregistrez ce lien pour sauvegarder la comptabilité sur votre disque local</a>
      <br />
      Pour restaurer la base, utilisez la commande mysql suivante :<br />
      <code>$ mysql -u root --password=xxx comptabilite &lt; comptabilite.sql</code><br />	
      <br /><a href="log/Dump_MedicaTuxTest.sql">Enregistrez ce lien pour sauvegarder les donn&eacute;es des m&eacute;dicaments sur votre disque local</a>
      <br />
      Pour restaurer la base, utilisez la commande mysql suivante :<br />
      <code>$ mysql -u root --password=xxx MedicaTuxTest &lt; Dump_MedicaTuxTest.sql</code><br />
<!-- <br /><a href="log/Dump_DatasempTest.sql">Cliquez ici pour sauvegarder les donn&eacute;es Vidal des m&eacute;dicaments sur votre disque local</a> -->
<?php
  }
  else
  {
    echo "
      Vous devez avoir les droits d'administration pour faire une sauvegarde.";
  }
} //fin du if repertoire accessible en ecriture
include("inc/footer.php");
?>
    </div>
    <p>
      <a href="http://validator.w3.org/check?uri=referer"><img
	src="http://www.w3.org/Icons/valid-xhtml10" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
    </p>
  </body>
</html>
