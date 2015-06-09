<?php
error_reporting(E_ALL & ~E_NOTICE);
@session_start();

if ( !isset( $_SESSION['login'] ) ) {
  exit;
}
//-----------------------------------------------------------------------------------------
	include_once("config.php");
	include_once("inc/maj.php");
//-----------------------------------------------------------------------------------------

//---------------------------- controle du fichier maj ------------------------------------
	$maj_serveur = "".URL3."/".REPERTOIRES."/MAJ?dl=1&token_hash=AAE-3VI7LuxBERIm2A8ttftNx9pyhbMc88bVoKmL40v67g";
  	$maj_serveur = @file_get_contents($maj_serveur);
  	
  	$maj_locale = "".DOSSIER_SOURCES."MAJ";
  	$maj_locale = @file_get_contents($maj_locale);
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
 if ($maj_serveur == $maj_locale) {
	echo "<link rel=\"stylesheet\" href=\"".URL2."/netupdate/style.css\" />";
 	echo "<div class='admin_liste'>";
	echo "<b>La version de votre logiciel est &agrave; jour</b>";
	echo "</div>";
	
		function clearDir($dossier) {
			$ouverture=@opendir($dossier);
			if (!$ouverture) return;
			while($fichier=readdir($ouverture)) {
				if ($fichier == '.' || $fichier == '..') continue;
					if (is_dir($dossier."/".$fichier)) {
						$r=clearDir($dossier."/".$fichier);
						if (!$r) return false;
					}
					else {
						$r=@unlink($dossier."/".$fichier);
						if (!$r) return false;
					}
			}
		closedir($ouverture);
		$r=@rmdir($dossier);
		if (!$r) return false;
			return true;
		}
		
		$dossier = "packages";
		clearDir($dossier);
		
	echo("<script>window.parent.location.reload();</script>"); 
 }
//-----------------------------------------------------------------------------------------
 else {
//-----------------------------------------------------------------------------------------
include_once("sources/dc_constante.php");

$step = !empty($_REQUEST['step']) ? (integer)$_REQUEST['step'] : 1;
$got_php5	= version_compare(PHP_VERSION, '5', '>=');
if (!$got_php5 && $step != 2)
{
	$step = 1;
}

function l10n($str)
{
	global $lang;

	return isset($lang[$str]) ? $lang[$str] : $str;
}

function fetchRemote($src,&$dest,$step=0)
{
	if ($step > 3)
	{
		return false;
	}

	// Essayons de lire le fichier avec la fonction curl() à distance
	if (function_exists('curl_init'))
	{
		$ch = @curl_init(); 
		@curl_setopt($ch, CURLOPT_URL, $src); 
		@curl_setopt($ch, CURLOPT_HEADER, 0); 
		@curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); 
		@curl_setopt($ch, CURLOPT_USERAGENT, 'IGSW Net-Update'); 
		$content = @curl_exec($ch);
		@curl_close($ch);
		if ($content !== false)
		{
			write_dest($content, $dest);
			return true;
		}
	}

	// Essayons de lire le fichier avec la fonction file_get_contents() à distance
	if ((boolean)ini_get('allow_url_fopen'))
	{
		$content = @file_get_contents($src);
		if ($content !== false)
		{
			write_dest($content, $dest);
			return true;
		}
	}

	// Essayons de lire le fichier avec la fonction fsockopen() à distance
	$src = parse_url($src);
	$host = $src['host'];
	$path = $src['path'];
	
	if (($s = @fsockopen($host,80,$errno,$errstr,5)) === false)
	{
		return false;
	}

	fwrite($s,
		'GET '.$path." HTTP/1.0\r\n"
		.'Host: '.$host."\r\n"
		."User-Agent: IGSW Net Update\r\n"
		."Accept: text/xml,application/xml,application/xhtml+xml,text/html,text/plain,image/png,image/jpeg,image/gif,*/*\r\n"
		."\r\n"
	);

	$i = 0;
	$in_content = false;
	while (!feof($s))
	{
		$line = fgets($s,4096);

		if (rtrim($line,"\r\n") == '' && !$in_content)
		{
			$in_content = true;
			$i++;
			continue;
		}
		if ($i == 0)
		{
			if (!preg_match('/HTTP\/(\\d\\.\\d)\\s*(\\d+)\\s*(.*)/',rtrim($line,"\r\n"), $m))
			{
				fclose($s);
				return false;
			}
			$status = (integer) $m[2];
			if ($status < 200 || $status >= 400)
			{
				fclose($s);
				return false;
			}
		}
		if (!$in_content)
		{
			if (preg_match('/Location:\s+?(.+)$/',rtrim($line,"\r\n"),$m))
			{
				fclose($s);
				return fetchRemote(trim($m[1]),$dest,$step+1);
			}
			$i++;
			continue;
		}
		write_dest($line, $dest);
		$i++;
	}
	fclose($s);
	return true;
}

function write_dest($str, &$dest)
{
  if (is_resource($dest))
	{
		fwrite($dest, $str);
	}
	else
	{
		$dest .= $str;
	}
}

function getLocation()
{
	$server_name = explode(':',$_SERVER['HTTP_HOST']);
	$server_name = $server_name[0];
	if ($_SERVER['SERVER_PORT'] == '443')
	{
		$scheme = 'https';
		$port = '';
	}
	elseif (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on')
	{
		$scheme = 'https';
		$port = ($_SERVER['SERVER_PORT'] != '443') ? ':'.$_SERVER['SERVER_PORT'] : '';
	}
	else
	{
		$scheme = 'http';
		$port = ($_SERVER['SERVER_PORT'] != '80') ? ':'.$_SERVER['SERVER_PORT'] : '';
	}
	$loc = preg_replace('#/$#','',str_replace('\\', '/', dirname($_SERVER['SCRIPT_NAME'])));

	return $scheme.'://'.$server_name.$port.$loc.'/';
}

function openPage()
{
//--------------------------------------- styles css ---------------------------------------
 echo "<link rel=\"stylesheet\" href=\"".URL2."/netupdate/style.css\" />";
//------------------------------------------------------------------------------------------
	echo "<div class='admin_liste'>";
}

function closePage()
{
	echo "</div>";
}

function initPHP5()
{
	$htaccess = dirname(__FILE__).'/.htaccess';
	if (file_exists($htaccess)) {
		if (!is_readable($htaccess) || !is_writable($htaccess))
		{
			return false;
		}
	}
	$rawdatas = '';
	if (!fetchRemote(DC_LOADER_SERVICE.'hosting.txt',$rawdatas))
	{
		return false;
	}
	$rawdatas = explode("\n",$rawdatas);
	if (!($my_hostname = @gethostbyaddr($_SERVER['SERVER_ADDR'])))
	{
		return false;
	}
	$found = false;
	foreach ($rawdatas as $line) {
		list($name,$hostname,$rule) = explode('|',trim($line));
		if (preg_match('!'.preg_quote($hostname).'$!',$my_hostname))
		{
			$found = $rule;
			break;
		}
	}
	if ($found) {
		if (false !== ($fh = @fopen($htaccess,"ab")))
		{
			fwrite($fh,"\n".$found);
			fclose($fh);
			return true;
		}
	}
	return false;
}

function grabFiles()
{
	$failed = true;
	$lib_files = @fopen(dirname(__FILE__).'/igsw_files.php','wb');
	$lib_unzip = @fopen(dirname(__FILE__).'/igsw_unzip.php','wb');
	$dc_zip    = @fopen(dirname(__FILE__).'/MEDWEBTUX.zip','wb');

	if (!$lib_files || !$lib_unzip || !$dc_zip)
	{
		return false;
	}

	if (fetchRemote(DC_LOADER_SERVICE.'lib.files.txt',$lib_files))
	{
		if (fetchRemote(DC_LOADER_SERVICE.'class.unzip.txt',$lib_unzip))
		{
			if (fetchRemote(DC_LOADER_ARCHIVE.'',$dc_zip))
			{
				$failed = false;
			}
		}
	}

	fclose($lib_files);
	fclose($lib_unzip);
	fclose($dc_zip);

	if ($failed)
	{
		cleanFiles();
		return false;
	}
	return true;
}

function writeMessage($level,$title,$lines)
{
	if (empty($lines))
	{
		return;
	}

	echo
	'<div class="msg '.$level.'">'."\n".
	'<h3>'.$title.'</h3>'."\n".
	'<p>'."\n";
	foreach ($lines as $line)
	{
		echo $line.'<br />'."\n";
	}
	echo '</p></div>'."\n";
}

function nextAction($label,$step,$more='')
{
	echo
	'<form action="'.$_SERVER['SCRIPT_NAME'].'" method="post">'."\n".
	$more."\n".
	'<p class="button"><input type="hidden" name="step" value="'.$step.'" />'."\n".
	'<input type="hidden" name="lang" value="'.DC_LOADER_LANG.'" />'."\n".
	'<input type="submit" name="submit" value="'.$label.'"/>'."\n".
	'</p></form>'."\n";
}

switch ($step)
{
	case 1 :
	{
		openPage();
		
		if (!is_writable(dirname(__FILE__)))
		{
			writeMessage('Attention: ',l10n('Un accès en écriture est obligatoire'), array(
				l10n('NetUpdate ne peut &eacute;crire dans le r&eacute;pertoire courant et cela est n&eacute;cessaire.'),
				l10n('Essayez de changer les permissions pour permettre un accès en écriture, puis recharger cette page en cliquant sur le bouton Actualiser.')
			));
			nextAction(l10n('Actualiser'),1);
		}
		elseif (!$got_php5)
		{
			writeMessage('NOTE: ',l10n('Une version PHP 5 est obligatoire'), array(
				sprintf(l10n('Il semble que votre h&eacute;bergeur est en version PHP %s.'), PHP_VERSION),
				l10n('NetUpdate peut essayer de changer votre configuration de PHP 5 par la creation ou la modification du fichier. Htaccess.'),
				l10n('Notez que vous pouvez changer votre configuration par vous-meme et red&eacute;marrez NetUpdate apres.')
			));
			nextAction(l10n('Essayez de configurer en PHP 5'),2);
		}
		else
		{
			nextAction(l10n('Installer les mises &agrave; jour'),3,
				'<h4>Packages disponibles</h4>
				 <i>Des mises &agrave; jour sont disponibles pour votre logiciel MedWebTux.</i><br />'.
				getLocation().
				'<input type="hidden" id="destination" name="destination" '.
				'value="packages" size="15" maxlength="100" /></center>'
			);
		}
		closePage();
		break;
	}

	case 2 :
	{
		if (!empty($_POST['submit']) && !$got_php5)
		{
			$got_php5 = initPHP5();
		}
		if ($got_php5)
		{
			header('Location: '.$_SERVER['SCRIPT_NAME'].'?step=1&language='.DC_LOADER_LANG);
		}
		else
		{
			openPage();
			writeMessage('Attention: ',l10n('D&eacute;sole!'),array(
				l10n('NetUpdate ne peut pas configurer votre serveur PHP en version 5.'),
				l10n("Vous pouvez demander &agrave votre h&eacute;bergeur comment vous pourriez passer &agrave; PHP 5.")	
			));
			closePage();
		}
		break;
	}

	case 3 :
	{
		$msg = array(l10n('Que faites vous ici de cette façon ?!'));
		$text = '';
		if (!empty($_POST['submit']) && isset($_POST['destination']))
		{
			$msg = array();
			$dest = preg_replace('/[^A-Za-z0-9_\/-]/','',$_POST['destination']);
			$dest = preg_replace('#/+#','/',$dest);

			if (grabFiles())
			{
				$lib_files = dirname(__FILE__).'/igsw_files.php';
				$lib_unzip = dirname(__FILE__).'/igsw_unzip.php';
				$dc_zip		= dirname(__FILE__).'/MEDWEBTUX.zip';
				if (!file_exists($lib_files) || !file_exists($lib_unzip) || !file_exists($dc_zip))
				{
					$msg[] = l10n('Les fichiers n&eacute;cessaires ne sont pas pr&eacute;sents.');
				}

				require $lib_files;
				require $lib_unzip;
				$uz = new fileUnzip($dc_zip);
				$files = $uz->getList();
				if (!is_array($files) or count($files) == 0)
				{
					$msg[] = "Fichier zip invalide.";
				}
				else
				{
					foreach ($files as $k => $v)
					{
						if ($v['is_dir'])
						{
							continue;
						}
					if (preg_match('#^[^/]*/_data#', $k))
					{
						continue;
					}
						$t = preg_replace('#^[^/]*/#','./'.$dest.'/',$k);
						$uz->unzip($k,$t);
					}
				}
				$uz->close;
				unset($uz);

				if (!is_dir(dirname(__FILE__).'/'.$dest))
				{
					$msg[] = "Impossible de d&eacute;compresser le zip.";
				}
				
				else
				{
					@unlink("MEDWEBTUX.zip");
					@unlink("igsw_files.php");
					@unlink("igsw_unzip.php");
					
					echo "<meta http-equiv=\"refresh\" content=\"1;url=packages/install-maj.php\" /></div><br />";	
				}
				
			}
			else
			{
				$msg[] = l10n('Une erreur est survenue !');
			}
		}
		openPage();
		writeMessage('Attention: ',l10n('Quelque chose a mal tourn&eacute;...'),$msg);
		echo $text;
		closePage();
		break;
	}
   }
}
?>
