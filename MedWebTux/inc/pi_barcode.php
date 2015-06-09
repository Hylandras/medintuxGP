<?php

// ******************************************************* ©2008 Pitoo.com *****
// *****                   CODES A BARRES - Php script                     *****
// *****************************************************************************
// *****              (c) 2002 - pitoo.com - mail@pitoo.com                *****
// *****************************************************************************
// *****************************************************************************
// ***** Ce script est "FREEWARE",  il  peut  etre  librement copie et reutilise
// ***** dans vos propres pages et applications.  Il peut egalement etre modifie
// ***** ou ameliore.
// ***** CEPENDANT :  par  respect  pour l'auteur,  avant d'utiliser,  recopier, 
// ***** modifier ce code vous vous engagez a :
// ***** - conserver intact l'entete de ce fichier ( les commentaires comportant
// *****   Le nom du script,  le copyright le nom de l'auteur et son e-mail,  ce
// *****   texte et l'historique des mises a jour ).
// ***** - conserver intact la  mention  'pitoo.com'  imprimee aleatoirement sur 
// *****   l'image du code genere dans environ 2% des cas.
// ***** - envoyer un  e-mail  a l'auteur  mail(a)pitoo.com  lui indiquant votre
// *****   intention d'utiliser le resultat de son travail.
// *****************************************************************************
// ***** Toute remarque, tout commentaire, tout rapport de bug, toute recompense
// ***** sont la bienvenue : mail(a)pitoo.com
// ***** faire un don sur PayPal : paypal(a)pitoo.com
// *****************************************************************************
// *****************************************************************************
// *****                       Historique des versions                     *****
// *****************************************************************************
$last_version = "V2.11" ;
// ***** V2.11 - 11/08/2010 - pitoo.com
// *****       - Correction : Ligne 1003 : Déclaration des variables pour éviter le "Warning" PHP
// ***** V2.10 - 08/12/2009 - pitoo.com
// *****       - Correction : Ligne 998 : Sur un serveur IIS 6, problème rencontré avec la variable REQUEST_URI retournée vide.
// ***** 	     Remplacée par PHP_SELF, ca fonctionne. merci à Jean-Christophe BARON - www.cc-web.fr
// ***** V2.9  - 25/09/2008 - pitoo.com
// *****       - Corrections pour eviter l'affichage de messages "Notice" de PHP
// ***** V2.8  - 10/07/2008 - pitoo.com
// *****       - Correction de bogue
// ***** V2.7  - 10/07/2008 - pitoo.com
// *****       - Ajout du format JPG
// ***** V2.6  - 10/07/2008 - pitoo.com
// *****       - Petites corrections de bugs d'affichage et de positionnement
// ***** V2.5  - 08/07/2008 - pitoo.com
// *****       - Réécriture/Encapsulation de toutes les fonctions dans la Classe
// *****       - Ajout d'une fonction permettant d'utiliser le script pour 
// *****         enregistrer l'image sur le disque au lieu de l'afficher
// *****       - Ajout de la possibilité de colorer le code
// *****       - Ajout de la posibilité de générer deux formats PNG ou GIF
// *****       - correction d'un bug dans le checksum (10='-') du C11
// *****	   - corrections majeures de structures de code
// ***** V2.05 - 13/06/2006 - pitoo.com
// *****       - Suppression des fonctions inutiles (V1)
// *****       - Ajout de commentaires
// ***** V2.04 - 23/01/2006 - pitoo.com
// *****       - Correction erreur codage Lettre A du code 39
// ***** V2.03 - 20/11/2004 - pitoo.com
// *****       - Supression de messages warning php
// ***** V2.02 - 07/04/2004 - pitoo.com
// *****       - Suppression du checksum et des Start/Stop sur le code KIX
// ***** V2.01 - 18/12/2003 - pitoo.com
// *****       - Correction de bug pour checksum C128 = 100 / 101 / 102
// ***** V2.00 - 19/06/2003 - pitoo.com
// *****       - Réécriture de toutes les fonctions pour génération directe de
// *****         l'image du code barre en PNG plutôt que d'utiliser une 
// *****         multitude de petits fichiers GIFs
// ***** V1.32 - 21/12/2002 - pitoo.com
// *****       - Ecriture du code 39
// *****       - Amelioration des codes UPC et 25 ()
// ***** V1.31 - 17/12/2002 - pitoo.com
// *****       - Amelioration du code 128 (ajout du Set de caracteres C)
// *****       - Amelioration du code 128 (ajout du code lisible en dessous )
// ***** V1.3  - 12/12/2002 - pitoo.com
// *****       - Ecriture du code 128 B
// ***** V1.2  - 01/08/2002 - pitoo.com
// *****       - Ecriture du code UPC / EAN
// ***** V1.0  - 01/01/2002 - pitoo.com
// *****       - Ecriture du code 25



// *****************************************************************************
// *****                        CLASSE pi_barcode                          *****
// *****************************************************************************
// ***** pi_barcode()               : Constructeur et ré-initialisation
// ***** 
// *****************************************************************************
// ***** Méthodes Publiques :
// *****************************************************************************
// ***** setSize($h, $w=0, $cz=0)   : Hauteur mini=15px
// *****                            : Largeur
// *****                            : Zones Calmes mini=10px
// ***** setText($text='AUTO')      : Texte sous les barres (ou AUTO ou '')
// ***** hideCodeType()             : Désactive l'impression du Type de code
// ***** setColors($fg, $bg=0)      : Couleur des Barres et du Fond
// ***** 
// ***** setCode($code)*            : Enregistre le code a générer
// ***** setType($type)*            : EAN, UPC, C39...
// ***** 
// ***** utiliser l'une ou l'autre de ces deux méthodes :
// ***** showBarcodeImage()**       : Envoie l'image PNG du code à l'affichage
// ***** writeBarcodeFile($file)**  : crée un fichier image du Code à Barres
// ***** 
// ***** * = appel requis
// ***** ** = appel requis pour l'un ou l'autre ou les 2
// ***** 
// *****************************************************************************
// ***** Méthodes Privées :
// *****************************************************************************
// ***** checkCode()                : Vérifie le CODE et positionne FULLCODE
// ***** encode()                   : Converti FULLCODE en barres
// ***** 
if ( !class_exists( "pi_barcode" ) ) {
	class pi_barcode
	{
		/**
		* ***** Définition des variables *****
		*/
		
		var $CODE;
		var $FULLCODE;
		var $TYPE;
		var $HEIGHT;
		var $WIDTH;
		var $CODEWIDTH;
		var $CALMZONE;
		var $HR;
		var $SHOWTYPE;
		var $BACKGROUND;
		var $FOREGROUND;
		var $FILETYPE;
		var $ENCODED;
		var $IH = NULL;
		
		/**
		* Définition des symbologies
		*/
		var $C128 = array(
					0 => "11011001100",     1 => "11001101100",     2 => "11001100110",
					3 => "10010011000",     4 => "10010001100",     5 => "10001001100",
					6 => "10011001000",     7 => "10011000100",     8 => "10001100100",
					9 => "11001001000",     10 => "11001000100",    11 => "11000100100",
					12 => "10110011100",    13 => "10011011100",    14 => "10011001110",
					15 => "10111001100",    16 => "10011101100",    17 => "10011100110",
					18 => "11001110010",    19 => "11001011100",    20 => "11001001110",
					21 => "11011100100",    22 => "11001110100",    23 => "11101101110",
					24 => "11101001100",    25 => "11100101100",    26 => "11100100110",
					27 => "11101100100",    28 => "11100110100",    29 => "11100110010",
					30 => "11011011000",    31 => "11011000110",    32 => "11000110110",
					33 => "10100011000",    34 => "10001011000",    35 => "10001000110",
					36 => "10110001000",    37 => "10001101000",    38 => "10001100010",
					39 => "11010001000",    40 => "11000101000",    41 => "11000100010",
					42 => "10110111000",    43 => "10110001110",    44 => "10001101110",
					45 => "10111011000",    46 => "10111000110",    47 => "10001110110",
					48 => "11101110110",    49 => "11010001110",    50 => "11000101110",
					51 => "11011101000",    52 => "11011100010",    53 => "11011101110",
					54 => "11101011000",    55 => "11101000110",    56 => "11100010110",
					57 => "11101101000",    58 => "11101100010",    59 => "11100011010",
					60 => "11101111010",    61 => "11001000010",    62 => "11110001010",
					63 => "10100110000",    64 => "10100001100",    65 => "10010110000",
					66 => "10010000110",    67 => "10000101100",    68 => "10000100110",
					69 => "10110010000",    70 => "10110000100",    71 => "10011010000",
					72 => "10011000010",    73 => "10000110100",    74 => "10000110010",
					75 => "11000010010",    76 => "11001010000",    77 => "11110111010",
					78 => "11000010100",    79 => "10001111010",    80 => "10100111100",
					81 => "10010111100",    82 => "10010011110",    83 => "10111100100",
					84 => "10011110100",    85 => "10011110010",    86 => "11110100100",
					87 => "11110010100",    88 => "11110010010",    89 => "11011011110",
					90 => "11011110110",    91 => "11110110110",    92 => "10101111000",
					93 => "10100011110",    94 => "10001011110",    95 => "10111101000",
					96 => "10111100010",    97 => "11110101000",    98 => "11110100010",
					99  => "10111011110",    // 99 et 'c' sont identiques ne nous sert que pour le checksum
					100 => "10111101110",    // 100 et 'b' sont identiques ne nous sert que pour le checksum
					101 => "11101011110",    // 101 et 'a' sont identiques ne nous sert que pour le checksum
					102 => "11110101110",    // 102 correspond à FNC1 ne nous sert que pour le checksum
					'c' => "10111011110",   'b' => "10111101110",   'a' => "11101011110",
					'A' => "11010000100",   'B' => "11010010000",   'C' => "11010011100",
					'S' => "1100011101011"
				);
		
		/**
		* Constructeur // est appelé automatiquement à l'instanciation de l'objet
		*/
		function pi_barcode()
		{
			$this->CODE = '';
			$this->FULLCODE = 'NO CODE SET';
			$this->TYPE = 'ERR';
			$this->HEIGHT = 15;
			$this->WIDTH = 0;
			$this->CALMZONE = 10;
			$this->HR = 'AUTO';
			$this->SHOWTYPE = 'Y';
			$this->FOREGROUND = hexdec('#000000');
			$this->BACKGROUND = hexdec('#FFFFFF');
			$this->FILETYPE = 'PNG';
			$this->ENCODED = '';
			// detruire eventuellement l'image existante
			if ($this->IH) imagedestroy($this->IH);
			$this->IH = NULL;
		}
		
		/**
		* Set Barcode Type
		*/
		function setType($type)
		{
			$this->TYPE = $type;
		}
		/**
		* Set Barcode String
		*/
		function setCode($code)
		{
			$this->CODE = $code;
		}
		/**
		* Set Image Height and Extra-Width
		*/
		function setSize($height, $width=0, $calmZone=0)
		{
			$this->HEIGHT = ($height > 15 ? $height : 15);
			$this->WIDTH = ($width > 0 ? $width : 0);
			$this->CALMZONE = ($calmZone > 10 ? $calmZone : 10);
		}
		/**
		* Set the Printed Text under Bars
		*/
		function setText($text='AUTO')
		{
			$this->HR = $text;
		}
		/**
		* Disable CodeType printing
		*/
		function hideCodeType()
		{
			$this->SHOWTYPE = 'N';
		}
		/**
		* Set Colors
		*/
		function setColors($fg, $bg='#FFFFFF')
		{
			$this->FOREGROUND = hexdec($fg);
			$this->BACKGROUND = hexdec($bg);
		}
		/**
		* Set File Type (PNG, GIF or JPG)
		*/
		function setFiletype($ft='PNG')
		{
			$ft = strtoupper($ft);
			$this->FILETYPE = ($ft == 'GIF' ? 'GIF' : ($ft == 'JPG' ? 'JPG' : 'PNG'));
		}
		
		/**
		* Vérification du Code
		* 
		* calcul ou vérification du Checksum
		*/
		function checkCode()
		{
			switch( $this->TYPE ) {
				case "C128C" :
				
					if (preg_match("/^[0-9]{2,48}$/", $this->CODE))
					{
						$tmp = strlen($this->CODE);
						if (($tmp%2)!=0) $this->FULLCODE = '0'.$this->CODE;
						else             $this->FULLCODE = $this->CODE;
					}
					else
					{
					  $this->TYPE = "ERR";
					  $this->FULLCODE = "CODE 128C REQUIRES DIGITS ONLY";
					  break;
					}
					
				case "C128" :
					
					$carok = true;
					$long = strlen( $this->CODE );
					$i = 0;
					while (($carok) && ($i<$long))
					{
						$tmp = ord( $this->CODE{$i} ) ;
						if (($tmp < 32) OR ($tmp > 126)) $carok = false;
						$i++;
					}
					if ($carok) $this->FULLCODE = $this->CODE;
					else
					{
					  $this->TYPE = "ERR";
					  $this->FULLCODE = "UNAUTHORIZED CHARS IN 128 CODE";
					}
				
				  break;
			}
		}
		
		/**
		* Encodage
		* 
		* Encode des symboles (a-Z, 0-9, ...) vers des barres
		*/
		function encode()
		{
			settype($this->FULLCODE, 'string');
			$lencode = strlen($this->FULLCODE);
			
			$encodedString = '';
			
			// Copie de la chaine dans un tableau
			$a_tmp = array();
			for($i = 0; $i < $lencode ; $i++) $a_tmp[$i] = $this->FULLCODE{$i};
	
			switch( $this->TYPE ) {
			
				case "C128C" :
					$encodedString = $this->C128['C']; //Start
					$checksum = 105 ;
					$j = 1 ;
					for ($i = 0; $i < $lencode; $i += 2)
					{
						$tmp = intval(substr($this->FULLCODE, $i, 2)) ;
						$checksum += ( $j++ * $tmp ) ;
						$encodedString .= $this->C128[$tmp];
					}
					$checksum %= 103 ;
					$encodedString .= $this->C128[$checksum];
					$encodedString .= $this->C128['S']; //Stop
				  break;
				case "C128" :
					$encodedString = $this->C128['B']; //Start
					$checksum = 104 ;
					$j = 1 ;
					for ($i = 0; $i < $lencode; $i++)
					{
						$tmp = ord($a_tmp[$i]) - 32 ;
						$checksum += ( $j++ * $tmp ) ;
						$encodedString .= $this->C128[$tmp];
					}
					$checksum %= 103 ;
					$encodedString .= $this->C128[$checksum];
					$encodedString .= $this->C128['S']; //Stop
				  break;
				  
			}
	
			$nb_elem = strlen($encodedString);
			$this->CODEWIDTH = max( $this->CODEWIDTH, $nb_elem );
			$this->WIDTH = max( $this->WIDTH, $this->CODEWIDTH + ($this->CALMZONE*2) );
			$this->ENCODED = $encodedString;
		
		
			/**
			* Création de l'image du code
			*/
			
			//Initialisation de l'image
			$txtPosX = $posX = intval(($this->WIDTH - $this->CODEWIDTH)/2); // position X
			$posY = 0; // position Y
			$intL = 1; // largeur de la barre
			
			// detruire eventuellement l'image existante
			if ($this->IH) imagedestroy($this->IH);
			
			$this->IH = imagecreate($this->WIDTH, $this->HEIGHT);
			
			// colors
            $color[0] = ImageColorAllocate($this->IH, 0xFF & ($this->BACKGROUND >> 0x10), 0xFF & ($this->BACKGROUND >> 0x8), 0xFF & $this->BACKGROUND);
            $color[1] = ImageColorAllocate($this->IH, 0xFF & ($this->FOREGROUND >> 0x10), 0xFF & ($this->FOREGROUND >> 0x8), 0xFF & $this->FOREGROUND);
			$color[2] = ImageColorAllocate($this->IH, 160,160,160); // greyed
			
			imagefilledrectangle($this->IH, 0, 0, $this->WIDTH, $this->HEIGHT, $color[0]);
			
			
			// Gravure du code
			for ($i = 0; $i < $nb_elem; $i++)
			{
			
				// Hauteur des barres dans l'image
				$intH = $this->HEIGHT; 
				if( $this->HR != '' )
				{
					switch ($this->TYPE)
					{
					  case "EAN" :
					  case "UPC" :
						if($i<=2 OR $i>=($nb_elem-3) OR ($i>=($nb_elem/2)-2 AND $i<=($nb_elem/2)+2)) $intH-=6; else $intH-=11;
					  break;
					  default :
						if($i>0 AND $i<($nb_elem-1)) $intH-=11;
					}
				}
				
				// Gravure des barres
				$fill_color = $this->ENCODED{$i};
				switch ($this->TYPE)
				{
				  default :
					if($fill_color == "1") imagefilledrectangle($this->IH, $posX, $posY, $posX, ($posY+$intH), $color[1]);
				}
				
				//Deplacement du pointeur
				$posX += $intL;
			}
			
			// Ajout du texte
			$ifw = imagefontwidth(3);
			$ifh = imagefontheight(3) - 1;
			
			$text = ($this->HR == 'AUTO' ? $this->CODE : $this->HR);
			
			switch ($this->TYPE)
			{
			  case "ERR" :
				$ifw = imagefontwidth(3);
				imagestring($this->IH, 3, floor( (($this->WIDTH)-($ifw * 7)) / 2 ), 1, "ERROR :", $color[1]); 
				imagestring($this->IH, 2, 10, 13, $this->FULLCODE, $color[1]); 
				$ifw = imagefontwidth(1);
				imagestring($this->IH, 1, ($this->WIDTH)-($ifw * 9)-2, $this->HEIGHT - $ifh, "Pitoo.com", $color[2]); 
			  break;
			  default :
				if ($text != '') imagestring($this->IH, 3, intval((($this->WIDTH)-($ifw * strlen($text)))/2)+1, $this->HEIGHT - $ifh, $this->CODE, $color[1]); 
			}
			
			// de temps a autres, ajouter pitoo.com *** Merci de ne pas supprimer cette fonction ***
			$ifw = imagefontwidth(1) * 9;
			if ((rand(0,50)<1) AND ($this->HEIGHT >= $ifw)) imagestringup($this->IH, 1, $nb_elem + 12, $this->HEIGHT - 2, "Pitoo.com", $color[2]);
			
			// impression du type de code (si demandé)
			if ($this->SHOWTYPE == 'Y')
			{
				if (($this->TYPE == "EAN") AND (strlen($this->FULLCODE) > 10) AND ($this->FULLCODE{0} > 0) AND ($text != ''))
				{
					imagestringup($this->IH, 1, 0, $this->HEIGHT - 12, $this->TYPE, $color[2]); 
				}
				elseif ($this->TYPE == "POSTNET")
				{
					imagestringup($this->IH, 1, 0, $this->HEIGHT - 2, "POST", $color[2]); 
				}
				elseif ($this->TYPE != "ERR")
				{
					imagestringup($this->IH, 1, 0, $this->HEIGHT - 2, $this->TYPE, $color[2]); 
				}
			}
		}
			
		
		/**
		* Show Image
		*/
		function showBarcodeImage()
		{
			$this->checkCode();
			$this->encode();

			if ($this->FILETYPE == 'GIF') 
			{
				Header( "Content-type: image/gif");
				imagegif($this->IH); 
			}
			elseif ($this->FILETYPE == 'JPG') 
			{
				Header( "Content-type: image/jpeg");
				imagejpeg($this->IH); 
			}
			else
			{
				Header( "Content-type: image/png");
				imagepng($this->IH); 
			}
		}
		
		/**
		* Save Image
		*/
		function writeBarcodeFile($file)
		{
			$this->checkCode();
			$this->encode();

			if ($this->FILETYPE == 'GIF')     imagegif($this->IH, $file);
			elseif ($this->FILETYPE == 'JPG') imagejpeg($this->IH, $file);
			else                              imagepng($this->IH, $file);
		}
				
	}
	//Fin de la classe
}
