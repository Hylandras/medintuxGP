<?php
session_start() ;
include("config.php");
$user=$_SESSION['login'];
if ( !isset( $_SESSION['login'] ) )
{
	header('location: ../MedWebTux/index.php' );
	exit;
}
//	include("inc/header.php");
if (!$dia_compta=mysql_connect($host, $loginbase, $pwd))
{
	print "Erreur de connexion &agrave; ".$host."<br>";
	exit;
}
if (!mysql_select_db($base_dia,$dia_compta))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($dia_compta);
	exit;
}
$annee=date('Y', date('U'));
?>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></meta>
  <title>MedWebTux - Synthèse espèces</title>
<script language="JavaScript">
<!--
function donner_focus(chp)
{
document.getElementById(chp).focus();
}
-->
</script>
</head>
<body style="font-size:<?php echo $fontsize ?>pt" onload="donner_focus('date_debut')" >
<h1>Recettes espèces</h1>


<?php
echo "Année : ".$annee."<br>";
$i=1;
for ($i=1;$i<=12;$i++)
echo $i."<br>";
?>
</body>
</html>