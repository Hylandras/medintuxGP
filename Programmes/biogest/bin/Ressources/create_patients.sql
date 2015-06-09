# Host: localhost
# Database: QLaboFTPTest
# Table: 'patients'
# 
CREATE TABLE `patients` (
  `service` varchar(16) default '',
  `nbOBX` bigint(20) default '0',
  `nomExpediteur` varchar(40) default '',
  `codeExpediteur` varchar(20) default '',
  `nomDestinataire` varchar(40) default '',
  `codeDestinataire` varchar(20) default '',
  `dateEmission` varchar(10) default '',
  `heureEmission` varchar(4) default '',
  `nom` varchar(40) NOT NULL default '',
  `prenom` varchar(40) NOT NULL default '',
  `dateNaissance` varchar(10) NOT NULL default '',
  `sexe` char(1) NOT NULL default '',
  `dateExamen` varchar(14) default '',
  `heureExamen` varchar(4) default '',
  `HprimSep` varchar(8) NOT NULL default '',
  `definitionExamen` varchar(40) NOT NULL default '',
  `fileNameRef` varchar(64) default '',
  `prim_key` bigint(20) NOT NULL auto_increment,
  PRIMARY KEY  (`prim_key`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

