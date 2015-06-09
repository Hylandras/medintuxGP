<?php
// On demarre la session
session_start();
$loginOK = false;  // cf Astuce
include("config.php");
$db=mysqli_connect($host, $loginbase, $pwd);

if(!$db)
{
//renvoie sur index avec message d'erreur adapte
	header('location: index.php?message=message2' );
	exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
	print "Erreur ".mysqli_error($db)."<br>";
	mysqli_close($db);
	exit;
}

//Transmis par MedinTux
//login={{USER LOGIN MEDECIN}}&pass=&ID
if (isset($_GET['pass']))
{
  if ($login=$_GET['login'] AND $pass=$_GET['pass']) //on evite les login vides qui autrement connecteraient n'importe qui
  {
    if (isset ($_GET['ID'])) //le GUID du patient
      $ID=$_GET['ID'];
    else
      $ID='';
    if (preg_match('`::`',$login)) //si on a recu login::signataire
    {
      $tab=explode('::',$login);
      $login =$tab[0];
      $signataire=$tab[1];
    }
    else
    {
      $signataire=$login;
    }

    $sql = "SELECT Login, PassWord FROM Personnes WHERE Login = '$login' AND PassWord='$pass'";
    $resultat=mysqli_query($db,$sql);
    $count=mysqli_num_rows($resultat);
    if ($count) //on a un utilisateur qui correspond
    {
      $data = mysqli_fetch_assoc($resultat);
	$_SESSION['login'] = $data['Login']."::".$signataire;//On cree la session
      if (isset($_REQUEST['cuv_medoc'])) //Pour medocs.php
      {
	header('location: medocs.php?cuv_medoc='.$_REQUEST['cuv_medoc'].'&radio_dispo=yes&radio_distri=4&radio_classe=1' ); //on va sur la page du medoc
	exit;
      }
      header('location: frame_patient.php?GUID='.$ID ); //on va sur la page du patient
      exit;
    }
    else //On arrive de MedinTux sans mot de passe valable -> page de login
    {
      header('location: index.php' );
      exit;
    }
  }
}
//Fin de l'acces par MedinTux

 // On n'effectue les traitement qu'a la condition que 
 // les informations aient ete effectivement postees
 if ( isset($_POST) && (!empty($_POST['login'])) && (!empty($_POST['password'])) ) 
{
  extract($_POST);  // login et password
//	$password=$_POST['password']; //ne sert a rien
//On convertit le mot de passe en crypte pour le comparer au mot de passe crypte dans la base de donnees
  $len = strlen($password);
  $pass_crypt = '';

  for ($i=0; $i<$len; $i++)
  {
    $val = ord($password{$i});
    $val = ($val << (($i + 1) & 0x000F)) ^ $val;
    $pass_crypt .= sprintf("%04X", $val);
  }
// On va chercher le mot de passe afferent a ce login
  $sql = "SELECT Login, PassWord FROM Personnes WHERE Login = '".addslashes($login)."'";
  $resultat=mysqli_query($db,$sql);
  $count=mysqli_num_rows($resultat);
//	$req = mysqli_query($db,$sql) or die('Erreur SQL : <br />'.$sql);

  $req = mysqli_query($db,$sql) or die('<b><center>L\'identifiant ou le mot de passe est incorrect.</center></b>');

// On verifie que l'utilisateur existe bien
  if (mysqli_num_rows($req) > 0) 
  {
    $data = mysqli_fetch_assoc($req);
// On verifie que son mot de passe est correct
    if ($pass_crypt == $data['PassWord']) 
    {
      $loginOK = true;
    }
  }
}
 
// Si le login a ete valide on met les donnees en session
if ($loginOK) 
{
  if (isset($_REQUEST['select_sign']))
  {
    if ($_REQUEST['select_sign'])  //le signataire est renseigne
    {
      $select_sign=$_REQUEST['select_sign']; //en numero
      if ($select_sign==$login) //le signataire est l'utilisateur
	$sign='OK';
    //on verifie que le signataire existe bien
      $sql_utilisateur_delegue="SELECT * FROM Personnes INNER JOIN user_perms ON Personnes.GUID=user_perms.FriendUserGUID WHERE Login='". mysqli_real_escape_string($db,$login)."'";
      $resultat_utilisateur_delegue=mysqli_query($db,$sql_utilisateur_delegue);
      while ($ligne_utilisateur_delegue=mysqli_fetch_array($resultat_utilisateur_delegue))
      {
	$signataire=$ligne_utilisateur_delegue['SignataireGUID'];
	$sql_utilisateur_signataire="SELECT * FROM Personnes WHERE GUID='$signataire'";
	$resultat_utilisateur_signataire=mysqli_query($db,$sql_utilisateur_signataire);
	$ligne_utilisateur_signataire=mysqli_fetch_array($resultat_utilisateur_signataire);
	if ($select_sign==$ligne_utilisateur_signataire['Login']) //nom de login
	{
	  $sign='OK';
	  break;
	}    
      }
      if ($sign!='OK')
      {
      //le signataire n'est pas valable : on renvoie sur la page de login
	  header('location: index.php' );
	  exit;
echo "signataire $select_sign";
      }
    }
    else //le signataire est vide
    {
      header('location: index.php' );
      exit;
    }
  }
  else //le signataire n'est pas donne
  {
    header('location: index.php' );
    exit;
  }
  $_SESSION['login'] = $data['Login']."::".$select_sign;
//$page est le nom de la page qui a ete initialement demandee
  $page=$_REQUEST['page'];

  if ($page)
  {
//echo "signataire $select_sign";
    header('location: '.$page.'.php' );
    exit;
  }
}
else //le login n'est pas valide
{
  if (empty($_POST['password']))
  //echo "Le mot de passe ne peut etre vide";
  {
    header('location: index.php?message=message3' );
    exit;
  } 
//	Le mot de passe ou le login est faux. Cause possible : verrouillage numerique ou majuscule actif
  header('location: index.php?message=message1' );
  exit;
}
?>
