<?php
// on initialise et utilise la session
session_start();

// on detruit la session
session_destroy();
unset($_SESSION);
//On renvoie automatiquement sur la page de login
	header('location: index.php' );
	exit;
?>