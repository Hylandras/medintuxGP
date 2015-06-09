<?php
session_start() ;
include("config.php");

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
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
if (isset($_GET['city']))
	$city=$_GET['city'];
else
  $city="";
include("inc/header.php");

$sql_select_city="SELECT code_postal FROM codes_postaux WHERE ville='$city'";
$resultat_select_city=mysqli_query($db,$sql_select_city);
?>
    <title>
      MedWebTux - Code postal
    </title>

<script type="text/javascript">
<!--
function choisir(zipcode)
// on affecte la valeur (.value) dans :
// window.opener : la fenêtre appelante (celle qui a fait la demande)
// .document : son contenu
// .forms_x : le formulaire nomme
// .le champ 
// les valeurs attribuees proviennent du formulaire plus bas
//tester si fenêtre deja ouverte

{ 
window.opener.document.getElementById('form_general').CP.value = zipcode;
// on se ferme
self.close(); 
}
-->
    </script>
    <link rel="stylesheet" href="css/screen.css" type="text/css" media="screen" />

  </head>
	
  <body style="font-size:<?php echo $fontsize; ?>pt">
    <div class="conteneur">
   <div class="groupe">
      <h1>
	MedWebTux - Code postal
      </h1>
      <h2>
	<?php echo $city ?>
      </h2>
      <form action="city.php" method="get">
	<div>
<?php	
$i=0;
while ($liste_select_city=mysqli_fetch_array($resultat_select_city))
{
  $i++;
  echo "
	  <input type=\"radio\" name=\"city[]\" id=\"city_".$i."\" value=\"".$liste_select_city['code_postal']."\" onclick=\"choisir(this.value)\"/><label for=\"city_".$i."\" >".$liste_select_city['code_postal']."</label><br />";
}
?>
	</div>
      </form>
    </div>
<?php
include("inc/footer.php");
?>
