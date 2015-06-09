<?php 

header("Content-Type: text/plain");

$nick = (isset($_GET["Nick"])) ? $_GET["Nick"] : NULL;
$name = (isset($_GET["Name"])) ? $_GET["Name"] : NULL;

if ($nick && $name) {
	echo "Bonjour " . $name . " ! Je vois que votre pseudo est " . $nick;
} else {
	echo "FAIL";
}

?>

