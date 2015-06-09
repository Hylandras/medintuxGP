<?php
//Page destinee a modifier en tache de fond les statuts des rendez-vous dynamiquement depuis la liste
session_start() ;
if ( !isset( $_SESSION['login'] ) ) 
{
  header('location: index.php?page=agenda' );
  exit;
}

include("config.php");

$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br>";
  mysqli_close($db);
  exit;
}
$id=$_REQUEST['id'];
$status=$_REQUEST['status'];

$sql_update_appointment_status="UPDATE agenda SET `status`='$status' WHERE `PrimKey`='$id'";

$resultat_update_appointment_status=mysqli_query($db,$sql_update_appointment_status);

?>