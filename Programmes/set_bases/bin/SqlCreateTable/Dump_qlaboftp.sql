-- MySQL dump 10.2
--
-- Host: localhost    Database: qlaboftp
---------------------------------------------------------
-- Server version	4.1.0-alpha-log

--
-- Table structure for table 'examens'
--

DROP TABLE IF EXISTS `examens`;
CREATE TABLE `examens` (
  `ref_prim_key_Patient` bigint(20) default '0',
  `hprim_line` varchar(254) default '',
  `prim_key` bigint(20) NOT NULL auto_increment,
  PRIMARY KEY  (`prim_key`)
) TYPE=MyISAM CHARSET=latin1;


--
-- Table structure for table 'min_max'
--

DROP TABLE IF EXISTS `min_max`;
CREATE TABLE `min_max` (
  `sexe` char(2) NOT NULL default '',
  `test_name` varchar(32) NOT NULL default '',
  `val_min` varchar(10) default '0',
  `val_max` varchar(10) default '0'
) TYPE=MyISAM CHARSET=latin1;

--
-- Dumping data for table 'min_max'
--


LOCK TABLES `min_max` WRITE;
INSERT INTO `min_max` VALUES ('F','TNI  010',NULL,'0.10'),('M','TNI  010',NULL,'0.10'),('M','CPK  010','52','200'),('F','CPK  010','35','165'),('M','BNP  010',NULL,'100'),('F','BNP  010',NULL,'100'),('M','TP   020','70','100'),('F','TP   020','70','100'),('M','PLQV 020','150000','450000'),('F','PLQV 020','150000','450000'),('M','MYOG 010',NULL,'46'),('F','MYOG 010',NULL,'46'),('F','LIPA 010',NULL,'500'),('M','LIPA 010',NULL,'500'),('M','GGT  010',NULL,'50'),('F','GGT  010',NULL,'38'),('M','NV   030','4.1','5.7'),('F','NV   030','4.1','5.7');
UNLOCK TABLES;


--
-- Table structure for table 'patients'
--

DROP TABLE IF EXISTS `patients`;
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
  `fileNameRef` varchar(64) default '',
  `definitionExamen` varchar(40) NOT NULL default '',
  `prim_key` bigint(20) NOT NULL auto_increment,
  PRIMARY KEY  (`prim_key`)
) TYPE=MyISAM; 

