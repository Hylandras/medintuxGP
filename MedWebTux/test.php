<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">

  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=<?php echo $encodage ?>"/>
    <meta name="GENERATOR" content="PHP"/>
    <title>Fiche test</title>
  </head>
  <body>
    <p>
      Ceci est un texte en HTML. Un texte en php doit s'afficher la ligne plus bas</p>
    <p>
<?php
    echo "
	Si cette ligne s'affiche, php fonctionne (mais sans pr&eacute;juger de l'acc&egrave;s aux BDD)<br />";
?>
    </p>
    <form action="phpinfo.php" method="get">
      <p>
	<input type="submit" value="Afficher les donn&eacute; compl&egrave;tes de phpinfo"/>
      </p>
    </form>
    <p class='noPrint'>
	<a href="http://validator.w3.org/check?uri=referer"><img
	  src="pics/valid-xhtml10.png" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
    </p>
  </body>
</html>
