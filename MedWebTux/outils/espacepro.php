<?php
@session_start();

if ( !isset( $_SESSION['login'] ) ) {
  exit;
}
?>
<div class="groupe">
<h1>Espace pro</h1>
<iframe src="https://espacepro.ameli.fr"  width="99%" height="660" scrolling="auto" frameborder="0"></iframe> 
</div>
<?php
include("inc/footer.php");
?>
    </div>
    <p>
      <a href="http://validator.w3.org/check?uri=referer"><img
	src="http://www.w3.org/Icons/valid-xhtml10" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
    </p>
  </body>
</html>
