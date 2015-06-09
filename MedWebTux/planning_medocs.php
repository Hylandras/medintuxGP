<?php
session_start() ;
/*page cense etre appelee par ce formulaire dans consultation.php
<form action="planning_medocs.php" method="get" id='form_planning'>
<input type='submit' value='Planning' name='button_planning' />
<input type='hidden' value='<?php echo $patient ?>' name='GUID' />
<input type='hidden' value='<?php echo $ligne3['RbDate_RefBlobs_PrimKey'] ?>' name='primkey_blob' />
</form>
*/

include("config.php");
$basemed="DatasempTest";
if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=liste' );
  exit;
}
include("inc/header.php");
?>
  <title>MedWebTux - M&eacute;dicaments - Utilisateur <?php echo $_SESSION['login'] ?></title>

</head>
<body style="font-size:<?php echo $fontsize ?>pt"  onload="donner_focus('nom_medoc')" >
<div class="conteneur">
<?php	
// insertion du menu d'en-tete	
include("inc/menu-horiz.php");


// on choisit la bonne base
$db=mysql_connect($host, $loginbase, $pwd);
$codage=mysql_query("SET NAMES 'UTF8'");

if (!mysql_select_db($base,$db))
{
	print "Erreur ".mysql_error()."<br />";
	mysql_close($db);
	exit;
}

$variables=extract($_GET); //on recupere GUID et pirmkey_blob
//trouver le patient
$sql="SELECT * FROM IndexNomPrenom WHERE FchGnrl_IDDos='$GUID'";

$resultat=mysql_query($sql,$db);
$ligne=mysql_fetch_array($resultat);
$nom=$ligne["FchGnrl_NomDos"];
$prenom=$ligne["FchGnrl_Prenom"];


?>
<h1> Tableau de prise des médicaments de <a href="patient.php?GUID=<?php echo $GUID ?>"><?php echo $nom.' '.$prenom ?></a></h1>
<table border="1">
<tr>
<th>Sans précision</th>
<th>Matin</th>
<th>Midi</th>
<th>Après-midi</th>
<th>Soir</th>
<th>Au coucher</th>
</tr>
<?php


$sql3="SELECT RbDate_DataRub FROM RubriquesBlobs WHERE RbDate_PrimKey='$primkey_blob'";

$resultat3=mysql_query($sql3);
$ligne3=mysql_fetch_array($resultat3);

$ordo_struct = preg_replace('@<ordotext[^>]*?>.*?</ordotext>@si', '', utf8_decode($ligne3["RbDate_DataRub"]));
//on charge le reste pour analyse xml
$ordo_xml=simplexml_load_string($ordo_struct);
//print_r($ordo_xml);
$sp='';
$matin='';
$am='';
$soir='';
$midi='';
$coucher='';

if (!mysql_select_db($basemed,$db))
{
  print "Erreur ".mysql_error()."<br />";
  mysql_close($db);
  exit;
} 
print_r($ordo_xml); //test
echo "<br>";

foreach ($ordo_xml->PosologieList->Posologie as $value) //un tour pour chaque medoc
{
  $this_medoc=$value->cip; //bon
  $sql_medoc_t00="SELECT f0,f7 FROM t00 WHERE f2='$this_medoc'";
  $resultat_medoc_t00=mysql_query($sql_medoc_t00);
  $ligne_medoc_t00=mysql_fetch_array($resultat_medoc_t00);
  $this_cip=$ligne_medoc_t00['f0'];
  $this_unit=$ligne_medoc_t00['f7'];

  $sql_medoc="SELECT f2 FROM t4B WHERE f0='$this_cip'";
//  echo 'sql'.$sql_medoc.'<br />'; //bon
  $resultat_medoc=mysql_query($sql_medoc);
  $ligne_medoc=mysql_fetch_array($resultat_medoc);
  $schemaPrise=$value->schemaPrise;
$elements_prise=explode(';',$schemaPrise);
//  echo 'xxx'.$schemaPrise.'xxx<br />'; //vide
echo "<br>Schemaprise";
print_r($schemaPrise);
echo "<br>";
echo 'sizeofprises'.sizeof($elements_prise);
//echo 'scountschemaprise'.count($schemaPrise);
/*if (sizeof($prises) > 1)
  $elements_prise=explode (';',$prises);
else
  $elements_prise=$schemaPrise;
print_r($elements_prise);
*/
echo "<br>";

//  print_r($elements_prise); //bon
  $nombre_elements_prise=sizeof($elements_prise);
//echo $nombre_elements_prise;
  //echo $value->doseMax;

//l'unite de prise
  $sql_unit="SELECT f1 FROM t24 WHERE f0='$this_unit'";
  $resultat_unit=mysql_query($sql_unit);
  $ligne_unit=mysql_fetch_array($resultat_unit);
echo 'dosemax'.$value->doseMax;
  if ($value->doseMax=='0' OR $value->doseMax=='') //si dose max = 0  dose min=dose exacte
  {
    $dose_quotidienne=$value->doseMin;
   if ($nombre_elements_prise > 1) //mode de prise precise
    {
      $dose_elementaire=$dose_quotidienne/$nombre_elements_prise;
//print_r ($elements_prise);
      foreach ($elements_prise AS $cet_element_prise)
      {
	if ($cet_element_prise == 'SO')
	{
	  $soir.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'MA')
	{
	  $matin.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'MI')
	{
	  $midi.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'AM')
	{
	  $am.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'CO')
	{
	  $coucher.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
      }
    }
    else // pas d'horaire precise, mais dose precisee
    {
      $sp.=$ligne_medoc['f2'].' : '.$dose_quotidienne.' '.$ligne_unit['f1'].'<br />';
    }
  }
  else  //fourchette de doses quotidiennes
  {
       $dose_quotidienne_min=$value->doseMin;
  //echo "dose_quotidienne_min $dose_quotidienne_min";

      $dose_quotidienne_max=$value->doseMax;
   $dose_quotidienne="$dose_quotidienne_min à $dose_quotidienne_max"; 
    if ($nombre_elements_prise > 1) //mode de prise precise
    {
echo "test";
//echo  "dose_quotidienne_max $dose_quotidienne_max";
  //    echo $dose_quotidienne.' sans précision';

      $dose_elementaire_min=$dose_quotidienne_min/$nombre_elements_prise;
      $dose_elementaire_max=$dose_quotidienne_max/$nombre_elements_prise;
      $dose_elementaire="$dose_elementaire_min à $dose_elementaire_max";

//echo "dose elementaire $dose_elementaire";
//print_r ($elements_prise);
      foreach ($elements_prise AS $cet_element_prise)
      {
//echo "prise $cet_element_prise";
	if ($cet_element_prise == 'SO')
	{
  //	echo $dose_elementaire.' le soir';
	  $soir.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'MA')
	{
  //	echo $dose_elementaire.' le matin';
	  $matin.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'MI')
	{
  //	echo $dose_elementaire.' le midi';
	  $midi.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'AM')
	{
  //	echo $dose_elementaire.' après-midi';
	  $am.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
	elseif ($cet_element_prise == 'CO')
	{
  //	echo $dose_elementaire.' après-midi';
	  $coucher.=$ligne_medoc['f2'].' : '.$dose_elementaire.' '.$ligne_unit['f1'].'<br />';
	}
      }

    }
else //fourchette de doses sans precision d'horaire
{
  $sp.=$ligne_medoc['f2'].' : '.$dose_quotidienne.' '.$ligne_unit['f1'].'<br />';
}

  }
}

?>
<tr>
<td><?php echo $sp ?></td>
<td><?php echo $matin ?></td>
<td><?php echo $midi ?></td>
<td><?php echo $am ?></td>
<td><?php echo $soir ?></td>
<td><?php echo $coucher ?></td>
</tr>
</table>
<?php
include("inc/footer.php");
?>