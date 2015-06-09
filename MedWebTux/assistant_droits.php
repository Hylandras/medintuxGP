<?php
session_start() ;
//Page fille de la page intervenant destinee a choisir les droits delegues
include("config.php");
include("inc/header.php");
if ( !isset( $_SESSION['login'] ) )
{
  header('location: index.php?page=liste' );
  exit;
}
$db=mysqli_connect($host, $loginbase, $pwd);
$codage=mysqli_query($db,"SET NAMES 'UTF8'");

if(!$db)
{
  print "Erreur de connexion &agrave; $host<br />";
  exit;
}

// on choisit la bonne base

if (!mysqli_select_db($db,$base))
{
  print "Erreur ".mysqli_error($db)."<br />";
  mysqli_close($db);
  exit;
}
//guid_sign=02519c01-5e1d-451b-a710-10afb6b9b183&guid_friend=cc44a7eb-acf2-446a-8f33-846c758bcf1d&mode=signataire
$mode=$_REQUEST['mode'];
if ($mode=='delegue')
{
  $guid_sign=$_REQUEST['guid_sign']; //la personne qui donne les droits (signataire)
  $guid_friend=$_REQUEST['guid_friend']; //la personne qui recoit les droits (delegue)
}
elseif ($mode=='signataire') //on inverse pour les signataires
{
  $guid_sign=$_REQUEST['guid_friend']; //la personne qui donne les droits (signataire)
  $guid_friend=$_REQUEST['guid_sign']; //la personne qui recoit les droits (delegue)
  $sql_login_delegue="SELECT Login FROM Personnes WHERE GUID ='$guid_friend'";
  $resultat_login_delegue=mysqli_query($db,$sql_login_delegue);
  $ligne_login_delegue=mysqli_fetch_array($resultat_login_delegue);
}

$sql_select_droits="SELECT Droits,Login FROM Personnes WHERE GUID='$guid_sign'"; //permet de connaitre les droits personnels du signataire.
$resultat_select_droits=mysqli_query($db,$sql_select_droits);
$ligne_select_droits=mysqli_fetch_array($resultat_select_droits);
$droits=$ligne_select_droits['Droits'];
$Login=$ligne_select_droits['Login'];
if ($mode=='signataire')
  $Login=$ligne_login_delegue['Login'];

$sql_select_droits_delegues="SELECT FriendUserDroits FROM user_perms WHERE SignataireGUID='$guid_sign' AND FriendUserGUID='$guid_friend'";
$resultat_select_droits_delegues=mysqli_query($db,$sql_select_droits_delegues);
$ligne_select_droits_delegues=mysqli_fetch_array($resultat_select_droits_delegues);
$droits_delegues=$ligne_select_droits_delegues['FriendUserDroits'];

?>
    <title>
      Assistant de gestion des droits ?>
    </title>
    <script type="text/javascript">
//<![CDATA[
function envoyer_droits()
// on affecte la valeur (.value) dans :
// window.opener : la fenêtre appelante (celle qui a fait la demande)
// .document : son contenu
// .forms_x : le formulaire nomme
// .le champ 
// les valeurs attribuees proviennent du formulaire plus bas
{ 
  var droits='';
  var i = 0;
  av=document.getElementsByName("check_droit");

  for (i=0;i< av.length;i++)
  {
    if(av[i].checked==true)
    {
      droits=droits+'-'+document.getElementsByName('check_droit').item(i).value;
    }
  }
  droits=droits+'-';
  Login=document.getElementsByName("Login")[0].value;
<?php
if ($mode=='delegue')
{
?>
  var tmp=eval("window.opener.document.forms['form_delegue'].droits_"+Login);
<?php
}
else
{
?>
  var tmp=eval("window.opener.document.forms['form_sign'].droits_sign_"+Login);
<?php
}
?>
  tmp.value= droits; //on renvoie la chaine de droits au champ droits de l'utilisateur signataire

  // on se ferme
  self.close(); 
}
//]]>
    </script>

  </head>
	
  <body style="font-size:<?php echo $fontsize; ?>pt">
    <div class="conteneur">
      <div class="groupe">
	<h1>MedWebTux - Gestion des droits délégués (signataire = <?php echo $ligne_select_droits['Login']; ?>)</h1>
	  <div class="notice">
	    Cette liste de droits représente tous les droits de MedinTux. Certains de ces droits ne sont pas exploités dans MedWebTux.<br />
	    Comme il s'agit de définir des droits délégués, les droits qui ne sont pas présents chez le signataire n'apparaissent pas dans cette liste.
	  </div>
	  <form id="form_droits" action="#" onsubmit="return envoyer_droits(this)">
	    <fieldset>
	      <legend>
		Droits
	      </legend>
	      <input type="button" name="submit_valider" value="Valider" onclick="envoyer_droits()" /><br />
	      <input type="hidden" name="Login" value="<?php echo $Login ?>" />
      <?php if (preg_match("`nbv`",$droits)) { ?><input type="checkbox" name="check_droit" value="nbv" <?php if (preg_match("`nbv`",$droits_delegues)) echo "checked='checked'" ?> />Ne peut pas débloquer les bases<br /><?php } ?>
      <?php if (preg_match("`idd`",$droits)) { ?><input type="checkbox" name="check_droit" value="idd" <?php if (preg_match("`idd`",$droits_delegues)) echo "checked='checked'" ?> />Ne peut pas effacer le dossier patient<br /><?php } ?>
      <?php if (preg_match("`ata`",$droits)) { ?><input type="checkbox" name="check_droit" value="ata" <?php if (preg_match("`ata`",$droits_delegues)) echo "checked='checked'" ?> />Accéder aux antécédents<br /><?php } ?>
      <?php if (preg_match("`bic`",$droits)) { ?><input type="checkbox" name="check_droit" value="bic" <?php if (preg_match("`bic`",$droits_delegues)) echo "checked='checked'" ?> />Créer biologie<br /><?php } ?>
      <?php if (preg_match("`clc`",$droits)) { ?><input type="checkbox" name="check_droit" value="clc" <?php if (preg_match("`clc`",$droits_delegues)) echo "checked='checked'" ?> />Créer classeur<br /><?php } ?>
      <?php if (preg_match("`img`",$droits)) { ?><input type="checkbox" name="check_droit" value="img" <?php if (preg_match("`img`",$droits_delegues)) echo "checked='checked'" ?> />Crer imagerie<br /><?php } ?>
      <?php if (preg_match("`gln`",$droits)) { ?><input type="checkbox" name="check_droit" value="gln" <?php if (preg_match("`gln`",$droits_delegues)) echo "checked='checked'" ?> />Créer dossiers dans le glossaire<br /><?php } ?>
      <?php if (preg_match("`cec`",$droits)) { ?><input type="checkbox" name="check_droit" value="cec" <?php if (preg_match("`cec`",$droits_delegues)) echo "checked='checked'" ?> />Créer certificats<br /><?php } ?>
      <?php if (preg_match("`coc`",$droits)) { ?><input type="checkbox" name="check_droit" value="coc" <?php if (preg_match("`coc`",$droits_delegues)) echo "checked='checked'" ?> />Créer courriers<br /><?php } ?>
      <?php if (preg_match("`doc`",$droits)) { ?><input type="checkbox" name="check_droit" value="doc" <?php if (preg_match("`doc`",$droits_delegues)) echo "checked='checked'" ?> />Créer des documents<br /><?php } ?>
      <?php if (preg_match("`iec`",$droits)) { ?><input type="checkbox" name="check_droit" value="iec" <?php if (preg_match("`iec`",$droits_delegues)) echo "checked='checked'" ?> />Créer documents dans l'identification<br /><?php } ?>
      <?php if (preg_match("`obc`",$droits)) { ?><input type="checkbox" name="check_droit" value="obc" <?php if (preg_match("`obc`",$droits_delegues)) echo "checked='checked'" ?> />Créer observations<br /><?php } ?>
      <?php if (preg_match("`orc`",$droits)) { ?><input type="checkbox" name="check_droit" value="orc" <?php if (preg_match("`orc`",$droits_delegues)) echo "checked='checked'" ?> />Créer prescriptions<br /><?php } ?>
      <?php if (preg_match("`etc`",$droits)) { ?><input type="checkbox" name="check_droit" value="etc" <?php if (preg_match("`etc`",$droits_delegues)) echo "checked='checked'" ?> />Créer et effacer les antécédents<br /><?php } ?>
      <?php if (preg_match("`idc`",$droits)) { ?><input type="checkbox" name="check_droit" value="idc" <?php if (preg_match("`idc`",$droits_delegues)) echo "checked='checked'" ?> />Créer et modifier l'identité<br /><?php } ?>
      <?php if (preg_match("`plc`",$droits)) { ?><input type="checkbox" name="check_droit" value="plc" <?php if (preg_match("`plc`",$droits_delegues)) echo "checked='checked'" ?> />Créer un patient<br /><?php } ?>
      <?php if (preg_match("`gld`",$droits)) { ?><input type="checkbox" name="check_droit" value="gld" <?php if (preg_match("`gld`",$droits_delegues)) echo "checked='checked'" ?> />Déplacer le glossaire<br /><?php } ?>
      <?php if (preg_match("`nog`",$droits)) { ?><input type="checkbox" name="check_droit" value="nog" <?php if (preg_match("`nog`",$droits_delegues)) echo "checked='checked'" ?> />Disposer du nomadisme<br /><?php } ?>
      <?php if (preg_match("`gls`",$droits)) { ?><input type="checkbox" name="check_droit" value="gls" <?php if (preg_match("`gls`",$droits_delegues)) echo "checked='checked'" ?> />Effacer des dossiers du glossaire<br /><?php } ?>
      <?php if (preg_match("`agm`",$droits)) { ?><input type="checkbox" name="check_droit" value="agm" <?php if (preg_match("`agm`",$droits_delegues)) echo "checked='checked'" ?> />Gérer un agenda multiple<br /><?php } ?>
      <?php if (preg_match("`bip`",$droits)) { ?><input type="checkbox" name="check_droit" value="bip" <?php if (preg_match("`bip`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer biologie<br /><?php } ?>
      <?php if (preg_match("`clp`",$droits)) { ?><input type="checkbox" name="check_droit" value="clp" <?php if (preg_match("`clp`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer classeurs<br /><?php } ?>
      <?php if (preg_match("`imp`",$droits)) { ?><input type="checkbox" name="check_droit" value="imp" <?php if (preg_match("`imp`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer imagerie<br /><?php } ?>
      <?php if (preg_match("`cep`",$droits)) { ?><input type="checkbox" name="check_droit" value="cep" <?php if (preg_match("`cep`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer certificats<br /><?php } ?>
      <?php if (preg_match("`cop`",$droits)) { ?><input type="checkbox" name="check_droit" value="cop" <?php if (preg_match("`cop`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer courriers<br /><?php } ?>
      <?php if (preg_match("`dop`",$droits)) { ?><input type="checkbox" name="check_droit" value="dop" <?php if (preg_match("`dop`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer documents<br /><?php } ?>
      <?php if (preg_match("`iep`",$droits)) { ?><input type="checkbox" name="check_droit" value="iep" <?php if (preg_match("`iep`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer documents de l'identification<br /><?php } ?>
      <?php if (preg_match("`obp`",$droits)) { ?><input type="checkbox" name="check_droit" value="obp" <?php if (preg_match("`obp`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer observations<br /><?php } ?>
      <?php if (preg_match("`orp`",$droits)) { ?><input type="checkbox" name="check_droit" value="orp" <?php if (preg_match("`orp`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer prescriptions<br /><?php } ?>
      <?php if (preg_match("`plp`",$droits)) { ?><input type="checkbox" name="check_droit" value="plp" <?php if (preg_match("`plp`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer un patient<br /><?php } ?>
      <?php if (preg_match("`idg`",$droits)) { ?><input type="checkbox" name="check_droit" value="idg" <?php if (preg_match("`idg`",$droits_delegues)) echo "checked='checked'" ?> />Lancer un dossier patient<br /><?php } ?>
      <?php if (preg_match("`cem`",$droits)) { ?><input type="checkbox" name="check_droit" value="cem" <?php if (preg_match("`cem`",$droits_delegues)) echo "checked='checked'" ?> />Modifier les certificats<br /><?php } ?>
      <?php if (preg_match("`com`",$droits)) { ?><input type="checkbox" name="check_droit" value="com" <?php if (preg_match("`com`",$droits_delegues)) echo "checked='checked'" ?> />Modifier les courriers<br /><?php } ?>
      <?php if (preg_match("`orm`",$droits)) { ?><input type="checkbox" name="check_droit" value="orm" <?php if (preg_match("`orm`",$droits_delegues)) echo "checked='checked'" ?> />Modifier les prescriptions<br /><?php } ?>
      <?php if (preg_match("`iem`",$droits)) { ?><input type="checkbox" name="check_droit" value="iem" <?php if (preg_match("`iem`",$droits_delegues)) echo "checked='checked'" ?> />Modifier documents dans l'identification<br /><?php } ?>
      <?php if (preg_match("`glc`",$droits)) { ?><input type="checkbox" name="check_droit" value="glc" <?php if (preg_match("`glc`",$droits_delegues)) echo "checked='checked'" ?> />Modifier le glossaire<br /><?php } ?>
      <?php if (preg_match("`atc`",$droits)) { ?><input type="checkbox" name="check_droit" value="atc" <?php if (preg_match("`atc`",$droits_delegues)) echo "checked='checked'" ?> />Modifier les antécédents<br /><?php } ?>
      <?php if (preg_match("`tvc`",$droits)) { ?><input type="checkbox" name="check_droit" value="tvc" <?php if (preg_match("`tvc`",$droits_delegues)) echo "checked='checked'" ?> />Modifier les tableaux variables<br /><?php } ?>
      <?php if (preg_match("`ttc`",$droits)) { ?><input type="checkbox" name="check_droit" value="ttc" <?php if (preg_match("`ttc`",$droits_delegues)) echo "checked='checked'" ?> />Modifier le traitement de fond<br /><?php } ?>
      <?php if (preg_match("`agc`",$droits)) { ?><input type="checkbox" name="check_droit" value="agc" <?php if (preg_match("`agc`",$droits_delegues)) echo "checked='checked'" ?> />Prendre des rendez-vous<br /><?php } ?>
      <?php if (preg_match("`cer`",$droits)) { ?><input type="checkbox" name="check_droit" value="cer" <?php if (preg_match("`cer`",$droits_delegues)) echo "checked='checked'" ?> />Renouveler un certificat<br /><?php } ?>
      <?php if (preg_match("`cor`",$droits)) { ?><input type="checkbox" name="check_droit" value="cor" <?php if (preg_match("`cor`",$droits_delegues)) echo "checked='checked'" ?> />Renouveler le courrier<br /><?php } ?>
      <?php if (preg_match("`orr`",$droits)) { ?><input type="checkbox" name="check_droit" value="orr" <?php if (preg_match("`orr`",$droits_delegues)) echo "checked='checked'" ?> />Renouveler une prescription<br /><?php } ?>
      <?php if (preg_match("`adr`",$droits)) { ?><input type="checkbox" name="check_droit" value="adr" <?php if (preg_match("`adr`",$droits_delegues)) echo "checked='checked'" ?> />Résoudre les doublons<br /><?php } ?>
      <?php if (preg_match("`fse`",$droits)) { ?><input type="checkbox" name="check_droit" value="fse" <?php if (preg_match("`fse`",$droits_delegues)) echo "checked='checked'" ?> />Saisir une FSE<br /><?php } ?>
      <?php if (preg_match("`rgl`",$droits)) { ?><input type="checkbox" name="check_droit" value="rgl" <?php if (preg_match("`rgl`",$droits_delegues)) echo "checked='checked'" ?> />Saisir un règlement<br /><?php } ?>
      <?php if (preg_match("`ccc`",$droits)) { ?><input type="checkbox" name="check_droit" value="ccc" <?php if (preg_match("`ccc`",$droits_delegues)) echo "checked='checked'" ?> />Utiliser la base CCAM<br /><?php } ?>
      <?php if (preg_match("`bmc`",$droits)) { ?><input type="checkbox" name="check_droit" value="bmc" <?php if (preg_match("`bmc`",$droits_delegues)) echo "checked='checked'" ?> />Utiliser la base médicamenteuse<br /><?php } ?>
      <?php if (preg_match("`cic`",$droits)) { ?><input type="checkbox" name="check_droit" value="cic" <?php if (preg_match("`cic`",$droits_delegues)) echo "checked='checked'" ?> />Utiliser la CIM 10<br /><?php } ?>
      <?php if (preg_match("`lac`",$droits)) { ?><input type="checkbox" name="check_droit" value="lac" <?php if (preg_match("`lac`",$droits_delegues)) echo "checked='checked'" ?> />Utiliser le labo<br /><?php } ?>
      <?php if (preg_match("`med`",$droits)) { ?><input type="checkbox" name="check_droit" value="med" <?php if (preg_match("`med`",$droits_delegues)) echo "checked='checked'" ?> />Utiliser MedinTux<br /><?php } ?>
      <?php if (preg_match("`plv`",$droits)) { ?><input type="checkbox" name="check_droit" value="plv" <?php if (preg_match("`plv`",$droits_delegues)) echo "checked='checked'" ?> />Voir la liste des patients<br /><?php } ?>
      <?php if (preg_match("`biv`",$droits)) { ?><input type="checkbox" name="check_droit" value="biv" <?php if (preg_match("`biv`",$droits_delegues)) echo "checked='checked'" ?> />Voir la biologie<br /><?php } ?>
      <?php if (preg_match("`clv`",$droits)) { ?><input type="checkbox" name="check_droit" value="clv" <?php if (preg_match("`clv`",$droits_delegues)) echo "checked='checked'" ?> />Voir les classeurs<br /><?php } ?>
      <?php if (preg_match("`imv`",$droits)) { ?><input type="checkbox" name="check_droit" value="imv" <?php if (preg_match("`imv`",$droits_delegues)) echo "checked='checked'" ?> />Voir l'imagerie<br /><?php } ?>
      <?php if (preg_match("`atv`",$droits)) { ?><input type="checkbox" name="check_droit" value="atv" <?php if (preg_match("`atv`",$droits_delegues)) echo "checked='checked'" ?> />Voir les antécédents<br /><?php } ?>
      <?php if (preg_match("`cev`",$droits)) { ?><input type="checkbox" name="check_droit" value="cev" <?php if (preg_match("`cev`",$droits_delegues)) echo "checked='checked'" ?> />Voir les certificats<br /><?php } ?>
      <?php if (preg_match("`cov`",$droits)) { ?><input type="checkbox" name="check_droit" value="cov" <?php if (preg_match("`cov`",$droits_delegues)) echo "checked='checked'" ?> />Voir les courriers<br /><?php } ?>
      <?php if (preg_match("`dov`",$droits)) { ?><input type="checkbox" name="check_droit" value="dov" <?php if (preg_match("`dov`",$droits_delegues)) echo "checked='checked'" ?> />Voir les documents<br /><?php } ?>
      <?php if (preg_match("`iev`",$droits)) { ?><input type="checkbox" name="check_droit" value="iev" <?php if (preg_match("`iev`",$droits_delegues)) echo "checked='checked'" ?> />Voir les documents de l'identification<br /><?php } ?>
      <?php if (preg_match("`obv`",$droits)) { ?><input type="checkbox" name="check_droit" value="obv" <?php if (preg_match("`obv`",$droits_delegues)) echo "checked='checked'" ?> />Voir les observations<br /><?php } ?>
      <?php if (preg_match("`orv`",$droits)) { ?><input type="checkbox" name="check_droit" value="orv" <?php if (preg_match("`orv`",$droits_delegues)) echo "checked='checked'" ?> />Voir les prescriptions<br /><?php } ?>
      <?php if (preg_match("`agv`",$droits)) { ?><input type="checkbox" name="check_droit" value="agv" <?php if (preg_match("`agv`",$droits_delegues)) echo "checked='checked'" ?> />Voir les rendez-vous<br /><?php } ?>
      <?php if (preg_match("`tvv`",$droits)) { ?><input type="checkbox" name="check_droit" value="tvv" <?php if (preg_match("`tvv`",$droits_delegues)) echo "checked='checked'" ?> />Voir les tableaux variables<br /><?php } ?>
      <?php if (preg_match("`sld`",$droits)) { ?><input type="checkbox" name="check_droit" value="sld" <?php if (preg_match("`sld`",$droits_delegues)) echo "checked='checked'" ?> />Voir l'état du solde<br /><?php } ?>
      <?php if (preg_match("`ttv`",$droits)) { ?><input type="checkbox" name="check_droit" value="ttv" <?php if (preg_match("`ttv`",$droits_delegues)) echo "checked='checked'" ?> />Voir le traitement de fond<br /><?php } ?>
      <?php if (preg_match("`idv`",$droits)) { ?><input type="checkbox" name="check_droit" value="idv" <?php if (preg_match("`idv`",$droits_delegues)) echo "checked='checked'" ?> />Voir l'identité<br /><?php } ?>
  <!-- 		    Ajouter ici les droits pour les rubriques arbitraires -->
<?php
    foreach ($rubrique_perso AS $this_rubrique_perso)
    {
      if (!preg_match('`20030000`',$this_rubrique_perso) AND !preg_match('`20020200`',$this_rubrique_perso) AND !preg_match('`20020100`',$this_rubrique_perso) AND !preg_match('`20060000`',$this_rubrique_perso) AND !preg_match('`20080000`',$this_rubrique_perso) AND !preg_match('`20020300`',$this_rubrique_perso) AND !preg_match('`20020500`',$this_rubrique_perso) AND !preg_match('`20090000`',$this_rubrique_perso) AND !preg_match('`20050000`',$this_rubrique_perso) )
      {
	$tableau_rubrique=explode('|',$this_rubrique_perso);
	$see=$tableau_rubrique[1].'v';
	$create=$tableau_rubrique[1].'c';
	$print=$tableau_rubrique[1].'p';
?>
      <?php if (preg_match("`$see`",$droits)) { ?><input type="checkbox" name="check_droit" value="<?php echo $see ?>" <?php if (preg_match("`$see`",$droits_delegues)) echo "checked='checked'" ?> />Voir la rubrique <?php echo $tableau_rubrique[0] ?><br /><?php } ?>
      <?php if (preg_match("`$create`",$droits)) { ?><input type="checkbox" name="check_droit" value="<?php echo $see ?>" <?php if (preg_match("`$create`",$droits_delegues)) echo "checked='checked'" ?> />Écrire dans la rubrique <?php echo $tableau_rubrique[0] ?><br /><?php } ?>
      <?php if (preg_match("`$print`",$droits)) { ?><input type="checkbox" name="check_droit" value="<?php echo $see ?>" <?php if (preg_match("`$print`",$droits_delegues)) echo "checked='checked'" ?> />Imprimer la rubrique <?php echo $tableau_rubrique[0] ?><br /><?php } ?>
<?php
      }
    }
?>
<!-- <input type="submit" name="submit_valider" value="Valider" onclick="envoyer_droits(this.form.check_droit.value)" /> -->
	      <input type="submit" name="submit_valider" value="Valider" />
	    </fieldset>
	  </form>
	</div>
<?php
include("inc/footer.php");
?>