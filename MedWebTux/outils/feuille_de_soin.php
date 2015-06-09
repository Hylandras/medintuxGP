<?php
if (isset ($_REQUEST['date']))
$date=$_REQUEST['date'];
else
$date="";
if (isset ($_REQUEST['nom']))
$nom=$_REQUEST['nom'];
else
$nom="";
if (isset ($_REQUEST['numSS']))
$numSS=$_REQUEST['numSS'];
else
$numSS="";
if (isset ($_REQUEST['dateN']))
$dateN=$_REQUEST['dateN'];
else
$dateN="";
if (isset ($_REQUEST['nomAss']))
$nomAss=$_REQUEST['nomAss'];
else
$nomAss="";
if (isset ($_REQUEST['adresse']))
$adresse=$_REQUEST['adresse'];
else
$adresse="";

?>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $encodage ?>"></meta>
	<title>MedWebTux - Feuille de soins</title>
</head>

<body style="font-size:<?php echo $fontsize; ?>pt" >
<div style="position:absolute; left:160mm; top:5mm; "><?php echo $date ?></div>
<div style="position:absolute;left: 20mm;top: 11mm"><?php echo $nom ?></div>
<div style="position:absolute;left: 33mm;top: 20mm"><?php echo $numSS ?></div>
<div style="position:absolute;left: 33mm;top: 27mm"><?php echo $dateN ?></div>
<div style="position:absolute;left: 33mm;top: 38mm"><?php echo $nomAss ?></div>
<div style="position:absolute;left: 33mm;top: 50mm"><?php echo $adresse ?></div>
</body>
</html>