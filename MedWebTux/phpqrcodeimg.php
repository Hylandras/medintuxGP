<?php
include("phpqrcode.php");
extract($_GET);
$qrcode_string=urldecode($qrcode_string);
QRcode::png($qrcode_string);
?>