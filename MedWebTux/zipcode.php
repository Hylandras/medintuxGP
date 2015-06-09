<?php
session_start() ;
include("config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; $host<br>";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}
if (isset($_GET['code']))
  $zipcode=$_GET['code'];
else
  $zipcode="";
include("inc/header.php");

$sql_select_zipcode="SELECT ville FROM codes_postaux WHERE code_postal='$zipcode'";
$resultat_select_zipcode=mysqli_query($db,$sql_select_zipcode);

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

{ 
  window.opener.document.getElementById('form_general').Ville.value = zipcode;
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
	<?php echo $zipcode?>
      </h2>
      <form action="zipcode.php" method="get">
    <div>
<?php	
$i=0;
while ($liste_select_zipcode=mysqli_fetch_array($resultat_select_zipcode))
{
$i++;
echo "
	  <input type=\"radio\" name=\"zipcode[]\" id=\"zipcode_".$i."\" value=\"".$liste_select_zipcode['ville']."\" onclick=\"choisir(this.value)\"/><label for=\"zipcode_".$i."\" >".$liste_select_zipcode['ville']."</label><br />";
}
?>
	</div>
      </form>
     </div>
<?php
include("inc/footer.php");
?>