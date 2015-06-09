<?php
session_start() ;
include("config.php");
$user=$_SESSION['login'];
if ( !isset( $_SESSION['login'] ) )
{
	header('location: ../MedWebTux/index.php' );
	exit;
}
//	include("inc/header.php");
if (!$dia_compta=mysql_connect($host, $loginbase, $pwd))
{
	print "Erreur de connexion &agrave; ".$host."<br>";
	exit;
}
if (!mysql_select_db($base_dia,$dia_compta))
{
	print "Erreur ".mysql_error()."<br>";
	mysql_close($dia_compta);
	exit;
}
$vir_banc=0;
$banque=0;
//VIR. BANC.
$annee=date('Y', date('U'));
$date_debut=$annee."-01-01 00:00:00";
$date_fin=$annee."-12-31 23:59:59";
if (isset ($_REQUEST['date_debut']))
{
$date_debut=$_REQUEST['date_debut'];
$date_fin=$_REQUEST['date_fin'];
}
$tableau_debut_court=explode (" ",$date_debut);
$debut_court= $tableau_debut_court[0];
$tableau_fin_court=explode (" ",$date_fin);
$fin_court=$tableau_fin_court[0];
?>
<script language="Javascript">
// {{{ docs <-- this is a VIM (text editor) text fold

/**
 * Popup Calendar {VERSION}
 *
 * Summary: Popup Calendar is a date selector script that can be associated with
 *          an image next to a text form element that requires a date. The calendar
 *          pops up, at which point a date can be selected, and it will close the
 *          calendar and pass the date down to the input field. It has customizable
 *          colors and full year/month navigation. It works on all browsers (Konqueror,
 *          IE, Netscape 4, Mozilla, Opera) and makes choosing dates in forms much more
 *          pleasant.
 *
 * Maintainer: Dan Allen <dan@mojavelinux.com>
 *
 * License: LGPL - however, if you use this library, please post to my forum where you
 *          use it so that I get a chance to see my baby in action.  If you are doing
 *          this for commercial work perhaps you could send me a few Starbucks Coffee
 *          gift dollars to encourage future developement (NOT REQUIRED).  E-mail me
 *          for and address.
 *
 * Homepage: http://www.mojavelinux.com/forum/viewtopic.php?t=6
 *
 * Freshmeat Project: http://freshmeat.net/projects/popupcalendar/?topic_id=92
 *
 * Updated: {UPDATED}
 *
 * Supported Browsers: Mozilla (Gecko), IE 5+, Konqueror, Opera 7, Netscape 4
 *
 * Usage: 
 * Bascially, you need to pay attention to the paths and make sure
 * that the function getCalendar is looking in the right place for calendar.html,
 * which is the parent frame of calendar_body.html.  
 * 
 * The colors are configured as an associative array in the parent window.  I
 * haven't had a chance to document this yet, but you should be able to see what I
 * am going for in the calendar.js file.  All you have to do when calling
 * getCalendar is specify the full object to that form element, such as
 * 
 * return getCalendar(document.formName.elementName);
 * 
 * You will need to put killCalendar() in the body to make it go away if it is still open
 * when the page changes.
**/

// }}}
// {{{ settings (Editable)

var calendarWindow = null;
var calendarColors = new Array();
calendarColors['bgColor'] = '#CAFFFF';
calendarColors['borderColor'] = '#333366';
calendarColors['headerBgColor'] = '#143464';
calendarColors['headerColor'] = '#FFFFFF';
calendarColors['dateBgColor'] = '#99FFC0';
calendarColors['dateColor'] = '#004080';
calendarColors['dateHoverBgColor'] = '#FFFFFF';
calendarColors['dateHoverColor'] = '#8493A8';
var calendarMonths = new Array('Janvier', 'Février', 'Mars', 'Avril', 'Mai', 'Juin', 'Juillet', 'Août', 'Septembre', 'Octobre', 'Novembre', 'Décembre');
var calendarWeekdays = new Array('D', 'L', 'M', 'M', 'J', 'V', 'S', 'D');
var calendarUseToday = true;
var calendarFormat = 'y-m-d';
var calendarStartMonday = true;
var calendarScreenX = 100; // either 'auto' or numeric
var calendarScreenY = 100; // either 'auto' or numeric

// }}}
// {{{ getCalendar()

function getCalendar(in_dateField) 
{
    if (calendarWindow && !calendarWindow.closed) {
        alert('Calendar window already open.  Attempting focus...');
        try {
            calendarWindow.focus();
        }
        catch(e) {}
        
        return false;
    }

    var cal_width = 415;
    var cal_height = 310;

    // IE needs less space to make this thing
    if ((document.all) && (navigator.userAgent.indexOf("Konqueror") == -1)) {
        cal_width = 410;
    }

    calendarTarget = in_dateField;
    calendarWindow = window.open('calendar.html', 'dateSelectorPopup','toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=no,resizable=0,dependent=no,width='+cal_width+',height='+cal_height + (calendarScreenX != 'auto' ? ',screenX=' + calendarScreenX : '') + (calendarScreenY != 'auto' ? ',screenY=' + calendarScreenY : ''));

    return false;
}

// }}}
// {{{ killCalendar()

function killCalendar() 
{
    if (calendarWindow && !calendarWindow.closed) {
        calendarWindow.close();
    }
}

// }}}

    </script> 

<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></meta>
  <title>MedWebTux - Compta Dia- recettes</title>
<script language="JavaScript">
<!--
function donner_focus(chp)
{
document.getElementById(chp).focus();
}
-->
</script>
</head>
<body style="font-size:<?php echo $fontsize ?>pt" onload="donner_focus('date_debut')" >
<h1>Recettes DIA</h1>
<form name="form_dates" action="dia_rec.php" method="GET">

Début <input type="text" name="date_debut" id="date_debut" value="<?php echo $debut_court ?>"/><a href="#" onclick="return getCalendar(document.form_dates.date_debut);"><img src="pics/calendar.png" border="0" /></a><br>
Fin <input type="text" name="date_fin" id="date_fin" value="<?php echo $fin_court ?>"/><a href="#" onclick="return getCalendar(document.form_dates.date_fin);"><img src="pics/calendar.png" border="0" /></a><br>
<input type="submit" name="bouton_envoyer" value="Envoyer" />
</form>

<?php
if (isset($_REQUEST['bouton_envoyer']))
{
?>
<table><tr><th>Date</th><th>Payeur</th><th>Paiement</th><th>Montant</th></tr>
<?php
$sql_chercher_recettes="SELECT * FROM intrec WHERE DATE >='$date_debut' AND DATE <='$date_fin' ORDER BY DATE";
$resultat_chercher_recettes=mysql_query($sql_chercher_recettes);
while ($ligne_chercher_recettes=mysql_fetch_array($resultat_chercher_recettes))
{
  if ($ligne_chercher_recettes['PAIE']=="VIR. BANC.")
  {
  $vir_banc=$vir_banc+$ligne_chercher_recettes['MONT'];
//echo "test".$ligne_chercher_recettes['MONT'];
  }
  if ($ligne_chercher_recettes['PAIE']=="BANQUE")
  {
  $banque=$banque+$ligne_chercher_recettes['MONT'];
//echo "test".$ligne_chercher_recettes['MONT'];
  }
$date=explode(" ",$ligne_chercher_recettes['DATE']);
?>
<tr><td><?php echo $date[0] ?></td><td><?php echo $ligne_chercher_recettes['LIBEL']?></td><td><?php echo $ligne_chercher_recettes['PAIE']?></td><td><?php echo $ligne_chercher_recettes['MONT']?></td></tr>
<?php
}
?>
</table>
Total virements = <?php echo $vir_banc ?><br>
Total banque = <?php echo $banque ?><br>
<?php
}
?>
</body>
</html>
