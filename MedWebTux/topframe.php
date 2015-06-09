<?php
include("config.php");
include_once ("inc/header.php");
?>
<title>
for top frame set
</title>

    <script type="text/javascript">
//<![CDATA[
function resizeFrame() //pour dire au cadre parent d'ajuster sa taille a celle du menu horizontal
{      
//    var container = document.getElementById('conteneur');
//    frame.style.height = container.offsetHeight + 'px';  //renvoie la hauteur du menu non deroule
  parent.document.getElementsByTagName( 'frameset' )[ 0 ].rows="120,*";
} 
//]]>
    </script>

<script type="text/javascript">
//<![CDATA[
function minimizeFrame() //pour dire au cadre parent d'ajuster sa taille a celle du menu horizontal
{      
//    var container = document.getElementById('conteneur');
//    frame.style.height = container.offsetHeight + 'px';  //renvoie la hauteur du menu non deroule
  parent.document.getElementsByTagName( 'frameset' )[ 0 ].rows="40,*";
} 
//]]>
    </script>

  </head>
	
  <body style="font-size:<?php echo $fontsize; ?>pt" >
    <div class="conteneur" id="conteneur">
<?php	
// insertion du menu d'en-tete	
include("inc/menu-horiz.php");	
?>
</div>
  </body>
</html>