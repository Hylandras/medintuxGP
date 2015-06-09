<?php
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
	header('location: index.php?page=agenda' );
	exit;
}
include("config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if (!mysqli_select_db($db,$base))
{
echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />";
echo "<div class=\"erreur\">
<b>Erreur de connexion &agrave; la base de donn&eacute;es h&eacute;berg&eacute; sur ".$_SERVER["HTTP_HOST"]."<br /><br /></b>
Veuillez vous assurer que la base de donn&eacute;es est active sur le serveur, que vous avez acc&egrave;s au serveur, que le port qu'utilise MySQL n'est pas bloqu&eacute; par un pare-feu, et que vos identifiants et mot de passe d'administration de la base de donn&eacute;es sont bien renseign&eacute;s. <br /><br />
<a href=\"setup/index.php\">Renseigner les donn&eacute;es de connexion au serveur Mysql</a>
</div>";
	@rename("set_up", "setup");
	exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}

if (isset($_REQUEST['intervenant']))
  $intervenant= $_REQUEST['intervenant']; //le GUID
if (isset($_REQUEST['login']))
  $login= $_REQUEST['login'];
if (isset($_REQUEST['pwd']))
{	
  $pwd= $_REQUEST['pwd'];
  $len = strlen($pwd);
  $pass_crypt = '';

  for ($i=0; $i<$len; $i++)
  {
    $val = ord($pwd{$i});
    $val = ($val << (($i + 1) & 0x000F)) ^ $val;
    $pass_crypt .= sprintf("%04X", $val);
  }
}
//Les droits directs
if (isset($_GET['submit_user']))
{
  $liste_droits="-";
  if (isset($_GET['droits']))
  {
    $les_droits=$_GET['droits'];
    foreach ($les_droits as $ce_droit)
    {
      $liste_droits=$liste_droits.$ce_droit.'-';
    }
  }
  //intervenant=254&login=xxx&pwd=yyy&droits[]=idd&droits[]=med&droits[]=plv&droits[]=biv&droits[]=sgn&submit_user=Valider
  $intervenant=$_REQUEST['intervenant'];
  $sql_update_personnes="UPDATE Personnes SET Login='$login',PassWord='$pass_crypt',Droits='$liste_droits'  WHERE ID_PrimKey='$intervenant'";
  $resultat_update_personnes=mysqli_query($db,$sql_update_personnes);
}
//echo $sql_update_personnes.'<br />';
//Les droits herites
if (isset($_REQUEST['submit_delegue'])) 
{
  $GUID_intervenant=$_REQUEST['GUID_intervenant'];
//echo $GUID_intervenant;
//On verifie tous les utilisateurs signataires un par un
  $sql_select_signataires="SELECT GUID,Login FROM Personnes WHERE Droits LIKE '%sgn%'";
  $resultat_select_signataires=mysqli_query($db,$sql_select_signataires);
  while ($ligne_select_signataires=mysqli_fetch_array($resultat_select_signataires))
  {
    $SignataireGUID= $ligne_select_signataires['GUID'];
//  echo $ligne_select_signataires['Login'].'<br />';
    $login_signataire=$ligne_select_signataires['Login'];
    $droits=$_REQUEST['droits_'.$login_signataire];
  //echo $droits.'<br />';
  //Pour chacun on verifie la coche.
    if (isset($_REQUEST['check_sign'])) //ne marche pas si pas pas de coche du tout
    {

      $coches=$_REQUEST['check_sign'];
    $count_select_friend=0;
      if (in_array($ligne_select_signataires['GUID'],$coches)) //si on est sur la ligne de l'utilisateur coche
      {
	$sql_select_friend="SELECT * FROM user_perms WHERE SignataireGUID='$SignataireGUID' AND  FriendUserGUID='$GUID_intervenant'"; //On cherche s'il existe une ligne avec le signataire et le friend
//  echo $sql_select_friend.'<br />';
	$resultat_select_friend=mysqli_query($db,$sql_select_friend);
	$count_select_friend=mysqli_num_rows($resultat_select_friend);
	$ligne_select_friend=mysqli_fetch_array($resultat_select_friend);
	if ($count_select_friend)//Si il existe deja un lien entre signataire et delegue
	{
  //On verifie si les droits sont identiques, sinon on met a jour le champ
	  if ($ligne_select_friend['FriendUserDroits']!=$droits)    
	  {
  //	  echo "update<br />".$ligne_select_friend['FriendUserDroits'].'<br />'.$droits.'<br />';
	    $sql_update="UPDATE user_perms SET FriendUserDroits='$droits' WHERE SignataireGUID='$SignataireGUID' AND FriendUserGUID='$GUID_intervenant'"; //On met a jour les droits delegues
	    $resultat_update=mysqli_query($db,$sql_update);
 // 	  echo $sql_update.'<br />';
	  }
	}
	else
	{
	  $sql_insert_link="INSERT INTO user_perms (SignataireGUID,FriendUserGUID,FriendUserDroits) VALUES ('$SignataireGUID','$GUID_intervenant','$droits')";
	  $resultat_insert_link=mysqli_query($db,$sql_insert_link);
//  	echo $sql_insert_link."<br />";
	}
      }
      else //si le signataire n'est pas coche, on verifie s'il est lie
      {
	$sql_select_friend="SELECT * FROM user_perms WHERE SignataireGUID='$SignataireGUID' AND  FriendUserGUID='$GUID_intervenant'";
  echo $sql_select_friend.'<br />';
	$resultat_select_friend=mysqli_query($db,$sql_select_friend);
	$count_select_friend=mysqli_num_rows($resultat_select_friend);
	if ($count_select_friend)//Si il existe deja un lien entre signataire et delegue
	{  //si oui, on efface le lien
	  $sql_delete_link="DELETE FROM user_perms WHERE SignataireGUID='$SignataireGUID' AND  FriendUserGUID='$GUID_intervenant'";
	  $resultat_delete_link=mysqli_query($db,$sql_delete_link);
    echo $sql_delete_link.'<br />';
	}
      }
    }
else //aucune coche, on supprime toutes les delegations
{
	  $sql_delete_link="DELETE FROM user_perms WHERE FriendUserGUID='$GUID_intervenant'";
//echo $sql_delete_link;
	  $resultat_delete_link=mysqli_query($db,$sql_delete_link);
}

  }
  $sql_intervenant="SELECT ID_Primkey FROM Personnes WHERE GUID='$GUID_intervenant'";
  //echo $sql_intervenant;
  $resultat_intervenant=mysqli_query($db,$sql_intervenant);
  $ligne_intervenant=mysqli_fetch_array($resultat_intervenant);
  $intervenant=$ligne_intervenant['ID_Primkey'];
}
//Si coche et pas de lien, on en cree un et on ajoute les droits
//Si pas coche et lien, on supprime la ligne
//Si coche et lien, on verifie les droit et on modifie si necessaire

if (isset($_REQUEST['submit_sign'])) //la validation des droits signataires
{
  $coches[]="";
  $GUID_intervenant=$_REQUEST['GUID_intervenant'];

  if (isset($_REQUEST['check_sign']))
  {
    $coches=$_REQUEST['check_sign'];
//onlit les droits pour les cases cochees
    foreach ($coches AS $this_coche)
    {
      $sql_select_signataires="SELECT Login FROM Personnes WHERE GUID='$this_coche'";
      $resultat_select_signataires=mysqli_query($db,$sql_select_signataires);
      $ligne_select_signataires=mysqli_fetch_array($resultat_select_signataires);
      $SignataireLogin= $ligne_select_signataires['Login'];

      $droits[$this_coche]=$_REQUEST['droits_sign_'.$SignataireLogin];
    //echo $droits[$this_coche].'<br />';
    }
  }
//on efface les liaisons non cochees
  $sql_delete_perms="DELETE FROM user_perms WHERE SignataireGUID='$GUID_intervenant' AND FriendUserGUID NOT IN ('".implode("','", $coches)."')";
  $resultat_delete_perms=mysqli_query($db,$sql_delete_perms);
  //on met a jour les liaisons existantes mais differentes
  reset($coches);
  foreach ($coches AS $this_coche)
  {
    $this_droit=$droits[$this_coche];
    $sql_update_perms="UPDATE user_perms SET FriendUserDroits='$this_droit'  WHERE SignataireGUID='$GUID_intervenant' AND FriendUserGUID='$this_coche' AND FriendUserDroits!='$this_droit'";
    $resultat_update_perms=mysqli_query($db,$sql_update_perms);
  }
//On ajoute les coches manquantes
  reset($coches);
  foreach ($coches AS $this_coche)
  {
    $this_droit=$droits[$this_coche];
    $sql_link="SELECT * FROM user_perms WHERE SignataireGUID='$GUID_intervenant' AND FriendUserGUID='$this_coche'";
    $resultat_link=mysqli_query($db,$sql_link);
    $count_link=mysqli_num_rows($resultat_link);
    if ($count_link==0)
    {
      $sql_insert_perms="INSERT INTO user_perms (SignataireGUID,FriendUserGUID,FriendUserDroits)  VALUES('$GUID_intervenant','$this_coche','$this_droit')";
      $resultat_insert_perms=mysqli_query($db,$sql_insert_perms);
//echo $sql_insert_perms;
    }
  }
//if ()
  $sql_intervenant="SELECT ID_Primkey FROM Personnes WHERE GUID='$GUID_intervenant'";
  //echo $sql_intervenant;
  $resultat_intervenant=mysqli_query($db,$sql_intervenant);
  $ligne_intervenant=mysqli_fetch_array($resultat_intervenant);
  $intervenant=$ligne_intervenant['ID_Primkey'];
}

header ('location:fiche_intervenant.php?intervenant='.$intervenant);
exit;
?>
