<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=stock' );
  exit;
}
include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");
error_reporting(-1);

if(!$db)
{
  print "Erreur de connexion &agrave; la base de donn&eacute;es sur $host. Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un firewall, et que vos login et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s dans config.php.<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
/*
CREATE TABLE IF NOT EXISTS `stock_data` (
  `stock_data_id` int(11) NOT NULL AUTO_INCREMENT,
  `stock_data_name` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `stock_data_number` int(11) DEFAULT NULL,
  `stock_data_available` tinyint(1) DEFAULT NULL,
  `stock_data_comment` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `stock_data_cat` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `stock_data_date` date DEFAULT NULL,
  `stock_data_command` tinyint(1) DEFAULT NULL COMMENT 'Pour dire qu''un produit manquant a été commandé',
  KEY `stock_data_id` (`stock_data_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Table du stock' AUTO_INCREMENT=55 ;

*/
function local_to_iso($date,$date_format)
{
  $list_date=explode ("-",$date);
  if ($date_format=='fr')
  {
    $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
  }
  elseif ($date_format=='en')
    $date=$list_date[2].'-'.$list_date[0].'-'.$list_date[1];
  return $date;
}

function iso_to_local($date,$date_format)
{
  $list_date=explode ("-",$date);
  if ($date_format=='fr')
  {
  //on repasse en iso les dates qui arrivent en francais
    $date=$list_date[2].'-'.$list_date[1].'-'.$list_date[0];
  }
  elseif ($date_format=='en')
    $date=$list_date[1].'-'.$list_date[2].'-'.$list_date[0];
  return $date;
}

if (isset($_REQUEST['submit_ajouter']))
{
  $category=$_REQUEST['category_select'];
  if ($_REQUEST['category_text'])
    $category=$_REQUEST['category_text'];
  $check_unavailable=FALSE;
  if (isset($_REQUEST['check_unavailable']))
    $check_unavailable=TRUE;
  $produit=$_REQUEST['produit'];
  $number=$_REQUEST['number'];
  $comment=$_REQUEST['comment_data'];
  $sql_inserer_produit=sprintf("INSERT INTO stock_data (stock_data_name,stock_data_number,stock_data_comment,stock_data_cat,stock_data_available) VALUES ('%s','$number','%s','%s','$check_unavailable')",mysqli_real_escape_string($db,$produit), mysqli_real_escape_string($db,$comment), mysqli_real_escape_string($db,$category));
  $resultat_inserer_produit=mysqli_query($db,$sql_inserer_produit);
  header ('location:stock.php');
  exit;
}
if (isset($_REQUEST['submit_line']))
{
  $date=local_to_iso($_REQUEST['date'],$date_format);
  $produit=$_REQUEST['produit'];
  $number=$_REQUEST['number'];
  $id=$_REQUEST['id'];
  $available=FALSE;
  if (isset($_REQUEST['check_unavailable']))
    $available=TRUE;
  $command=FALSE;
  if (isset($_REQUEST['check_command']))
    $command=TRUE;
  $category=$_REQUEST['category'];
  $resultat_update_line=mysqli_query($db,sprintf("UPDATE stock_data SET stock_data_name='%s',stock_data_cat='%s',stock_data_number='$number',stock_data_available='$available',stock_data_command='$command',stock_data_date='$date' WHERE stock_data_id='$id'",mysqli_real_escape_string($db,$produit), mysqli_real_escape_string($db,$category)));
  header ('location:stock.php');
  exit;
}
$fin_requete='';
$category='';
if (isset ($_REQUEST['submit_filter']))
{
  $category=$_REQUEST['category_select'];
  $produit=$_REQUEST['produit']."%";
  $unavailable=FALSE;
  if (isset ($_REQUEST['check_unavailable']))
  {
    $unavailable=TRUE;
  }
  $fin_requete=" WHERE stock_data_name LIKE'$produit' AND stock_data_cat LIKE '$category' AND stock_data_available='$unavailable' ";
}

	include("inc/header.php");
?>
  <meta name="keywords" lang="fr" content="Agenda pour MedinTux" />
  <title>Stock</title>
    <script type="text/javascript">
//<![CDATA[
function verif_champ(champ)
{
  if (champ == "")
  { 
    alert("Le champ produit n'est pas rempli");
    return false;
  }
  return true;
}
//]]>
    </script>

</head>

<body style="font-size:<?php echo $fontsize; ?>pt" >
  <div class="conteneur">
<?php
$anchor="stock";
include("inc/menu-horiz.php");
?>
<div class="groupe">

<h1>Stock</h1>


<table border ="1">
  <tr>
    <th style="width:20%;">
      Catégorie
    </th>
    <th style="width:20%;">
      Produit
    </th>
    <th style="width:6%;">
      Quantité
    </th>
    <th style="width:5%;">
      Manquant
    </th>
    <th style="width:5%;">
     Commandé
    </th>
    <th style="width:6%;">
      Date limite
    </th>
    <th  style="width:10%;">

    </th>
  </tr>
  <tr>
    <td colspan="7">
      <form action="stock.php" method="get" > 
	<table border="1">
	  <tr>
	    <td style="width:20%;">
	      <select name="category_select" id="category_select" >
<?php
//Boucle pour chercher les differentes categories et les afficher en deroulant
$resultat_chercher_cat=mysqli_query($db,"SELECT stock_data_cat FROM stock_data GROUP BY stock_data_cat");
while ($ligne_chercher_cat=mysqli_fetch_array($resultat_chercher_cat))
{
?>
		<option value="<?php echo $ligne_chercher_cat['stock_data_cat']; ?>" <?php if ($ligne_chercher_cat['stock_data_cat']==$category) echo 'selected="selected"'; ?> >
		  <?php echo $ligne_chercher_cat['stock_data_cat']; ?>
		</option>
<?php
}
?>
	      </select><br />
	      <label for="category_text" >Nouvelle&nbsp;catégorie</label>
	      <input type="text" name="category_text" id="category_text" value="" size="15" /> 
	    </td>
	    <td style="width:20%;">
	      <input name="produit" type="text" id="produit" value="" size="15" /><br />
	      <label for="comment_data">Commentaire</label>
	      <input name="comment_data" type="text" id="comment_data" value="" size="10" />
	    </td>
	    <td style="width:6%;">
	      <input type="text" name="number" value="" size="3" />
	    </td>
	    <td style="width:5%;">
	      <input type="checkbox" name="check_unavailable" <?php if (isset($_REQUEST['check_unavailable'])) echo 'checked="checked"' ?> />
	    </td>
	    <td style="width:5%;">
	    </td>
	    <td style="width:6%;">
	    </td>
	    <td style="width:10%;">
	      <input name="submit_ajouter" type="submit" value="Ajouter"/><br />
	      <input name="submit_filter" type="submit" value="Filtrer" /><br />
	    </td>
	  </tr>
	</table>
      </form>
    </td>
  </tr>

<?php
$sql_afficher_produits="SELECT * FROM stock_data $fin_requete";
//echo $sql_afficher_produits;
$resultat_afficher_produits=mysqli_query($db,$sql_afficher_produits);
$i=0;
while ($ligne_afficher_produits=mysqli_fetch_array($resultat_afficher_produits))
{
?>
  <tr>
    <td colspan="7">
      <form action="stock.php" method="get">
	<table border="1">
	  <tr>
	    <td style="width:20%;">
	      <input name="category" type="text" value="<?php echo stripslashes($ligne_afficher_produits['stock_data_cat']) ?>" size="15" />
	    </td>
	    <td title="<?php echo $ligne_afficher_produits['stock_data_comment'] ?>" style="width:20%;">
	      <input type="text" name="produit" value="<?php echo stripslashes($ligne_afficher_produits['stock_data_name']) ?>"  size="15" />
	      <input type="hidden" name="id" value="<?php echo $ligne_afficher_produits['stock_data_id'] ?>" />
	    </td>
	    <td style="width:6%;">
	      <input type="text" name="number" value="<?php echo $ligne_afficher_produits['stock_data_number'] ?>" size="3" />
	    </td>
	    <td  style="width:5%;">
	    <input type="checkbox" name="check_unavailable" <?php if ($ligne_afficher_produits['stock_data_available']) echo "checked='checked'"?> />
	    </td>
	    <td  style="width:5%;">
	    <input type="checkbox" name="check_command" <?php if ($ligne_afficher_produits['stock_data_command']) echo "checked='checked'"?> />
	    </td>
	    <td  style="width:6%;">
	      <input type="text" name="date" value="<?php if ($ligne_afficher_produits['stock_data_date']) echo iso_to_local($ligne_afficher_produits['stock_data_date'],$date_format) ?>"  size="7" />
	    </td>
	    <td  style="width:10%;">
	      <input type="submit" name="submit_line" value="Valider" />
	    </td>
	  </tr>
	</table>
      </form>
    </td>
  </tr>
<?php
$i++;
}
?>
</table>
</div>
</div>
</body>
</html>