--
-- Definition of table `entrees_sigems`
--

-- DROP TABLE IF EXISTS `entrees_sigems`;
CREATE TABLE `entrees_sigems` (
  `ES_id_primkey` int(10) unsigned NOT NULL DEFAULT '0',
  `ES_nom` varchar(45) DEFAULT NULL,
  `ES_prenom` varchar(45) DEFAULT NULL,
  `ES_sexe` varchar(1) DEFAULT NULL,
  `ES_date_naiss` varchar(25) DEFAULT NULL,
  `ES_num_ss` varchar(25) DEFAULT NULL,
  `ES_date_entree` datetime DEFAULT NULL,
  `ES_nom_fichier_entrees` varchar(45) DEFAULT NULL,
  `ES_guid_pat` varchar(45) DEFAULT NULL,
  `ES_blob_hpr` blob,
  `ES_rang_geme` varchar(3) DEFAULT NULL,
  `ES_qualite` tinyint(4) DEFAULT NULL,
  `ES_date_pec` datetime DEFAULT NULL,
  `ES_num_pat_sigems` varchar(25) DEFAULT NULL,
  PRIMARY KEY (`ES_id_primkey`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=32662 DEFAULT CHARSET=utf8;
