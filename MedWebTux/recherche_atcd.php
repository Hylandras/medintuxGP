<?php
/*page utilisee par AJAX pour chercher les antecedents pathologiques CIM-X a ajouter au terrain
t4N
f0=identifiant, lie a t55.f1
f2=libelle
f3,f4=codecim 10
*/
header("Content-Type: text/plain");

$atcd = (isset($_GET["atcd"])) ? $_GET["atcd"] : NULL;
$num = (isset($_GET["num"])) ? $_GET["num"] : NULL; //Le numero du terrain a modifier
$basemed="DatasempTest";

include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$basemed))
{
  echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />";
  echo "<div class=\"erreur\">
<b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
</div>";
  exit;
}

function detectUTF8($string) 
{
  return preg_match('%(?:
  [\\xC2-\\xDF][\\x80-\\xBF] # non-overlong 2-byte
  |\\xE0[\\xA0-\\xBF][\\x80-\\xBF] # excluding overlongs
  |[\\xE1-\\xEC\\xEE\\xEF][\\x80-\\xBF]{2} # straight 3-byte
  |\\xED[\\x80-\\x9F][\\x80-\\xBF] # excluding surrogates
  |\\xF0[\\x90-\\xBF][\\x80-\\xBF]{2} # planes 1-3
  |[\\xF1-\\xF3][\\x80-\\xBF]{3} # planes 4-15
  |\\xF4[\\x80-\\x8F][\\x80-\\xBF]{2} # plane 16
  )+%xs', $string);
}

if (strlen($atcd) > 2) 
{
  $sql="SELECT f2,f3 FROM t4N WHERE CAST(f2 AS CHAR) LIKE '".mysqli_real_escape_string($db,$atcd)."%'"; 
//f2 = atcd en clair
//f3= code cim10
  $resultat=mysqli_query($db,$sql);
  $atcds='';
  if ($num_rows=mysqli_num_rows($resultat))
  {
    while ($ligne=mysqli_fetch_array($resultat))
    {
/*      if (detectUTF8($ligne['f2']))
        $atcds=$atcds.'|'.utf8_decode($ligne['f2']).' code cim10 '.$ligne['f3']; //code cim10 = separateur pour transmettre a la fois le code et le nom
      else*/
       $atcds=$atcds.'|'.$ligne['f2'].' code cim10 '.$ligne['f3'];
    }
    echo $atcds."_".$num;
  }
  else
    echo "NOP_".$num; 
}
else
  echo "NOP_".$num; 

?>