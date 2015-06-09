-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.0.51a


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema DrTuxTest
--

CREATE DATABASE IF NOT EXISTS DrTuxTest;
USE DrTuxTest;

--
-- Definition of table `DrTuxTest`.`synopt_box`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_box`;
CREATE TABLE  `DrTuxTest`.`synopt_box` (
  `BO_Code` varchar(45) collate utf8_unicode_ci NOT NULL,
  `BO_Libelle` varchar(45) collate utf8_unicode_ci default NULL,
  `BO_CouleurBG` varchar(45) collate utf8_unicode_ci default NULL,
  `BO_CouleurTitre` varchar(45) collate utf8_unicode_ci default NULL,
  `BO_Nb_Maxi_Pat` int(10) unsigned default NULL,
  `BO_Type` varchar(45) collate utf8_unicode_ci default NULL,
  PRIMARY KEY  (`BO_Code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `DrTuxTest`.`synopt_box`
--

/*!40000 ALTER TABLE `synopt_box` DISABLE KEYS */;
LOCK TABLES `synopt_box` WRITE;
INSERT INTO `DrTuxTest`.`synopt_box` VALUES  ('S1','Box S5','#aa00ff','#ffaa00',1,'Soins'),
 ('S2','Box S6','#aa00ff','#ffaa00',3,'Soins'),
 ('A1','Salle attente','#00aaff','#EEDDAA',4,'Attente'),
 ('SR','SORTIES','#eb0000','#FFFFFF',200,'Sortie'),
 ('A2','Attente Couloir','#7ca580','#556a4c',10,'Attente'),
 ('S3','Box S7','#aa00ff','#ffaa7f',2,'Soins'),
 ('D4','Déchocage S8','#ff0000','#ffaa7f',1,'Soins'),
 ('R4','Suture S4','#00ff00','#ffaa00',1,'Soins'),
 ('R1','Traumato S1','#00aa00','#ffaa00',1,'Soins'),
 ('R2','Traumato S2','#00aa00','#ffaa00',1,'Soins'),
 ('R3','Suture S3','#00aa00','#ffaa00',1,'Soins'),
 ('L1','Lit 1','#aa5500','#ffaa7f',1,'Soins'),
 ('L2','Lit 2','#aa5500','#ffaa7f',1,'Soins'),
 ('L3','Lit 3','#aa5500','#ffaa7f',1,'Soins'),
 ('L4','Lit 4','#aa5500','#ffaa7f',1,'Soins'),
 ('L5','Lit 5','#aa5500','#ffaa7f',1,'Soins'),
 ('L6','Lit 6','#aa5500','#ffaa7f',1,'Soins');
UNLOCK TABLES;
/*!40000 ALTER TABLE `synopt_box` ENABLE KEYS */;


--
-- Definition of table `DrTuxTest`.`synopt_destinations`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_destinations`;
CREATE TABLE  `DrTuxTest`.`synopt_destinations` (
  `DE_PK` int(10) unsigned NOT NULL auto_increment,
  `DE_Destination` varchar(45) collate utf8_unicode_ci NOT NULL,
  `DE_TypeSortie` varchar(45) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`DE_PK`)
) ENGINE=MyISAM AUTO_INCREMENT=322 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `DrTuxTest`.`synopt_destinations`
--

/*!40000 ALTER TABLE `synopt_destinations` DISABLE KEYS */;
LOCK TABLES `synopt_destinations` WRITE;
INSERT INTO `DrTuxTest`.`synopt_destinations` VALUES  (294,'Retour domicile','Sortie'),
 (295,'Transfert Hôpital Nord','Sortie'),
 (296,'Transfert Hôpital de la Timone','Sortie'),
 (297,'Transfert Hôpital de Martigues','Sortie'),
 (298,'Transfert Hôpital Aix en Provence','Sortie'),
 (299,'Hospitalisé USIC','Sortie'),
 (300,'Hospitalisé USC rez de chaussé','Sortie'),
 (301,'Hospitalisé USC Troisième','Sortie'),
 (302,'Hospitalisé Service Ambulatoire','Sortie'),
 (303,'Hospitalisé Premier','Sortie'),
 (305,'Hospitalisé Deuxième','Sortie'),
 (306,'Hospitalisé Troisième','Sortie'),
 (307,'Hospitalisé Quatrième','Sortie'),
 (308,'Hospitalisé Cinquième','Sortie'),
 (309,'Transfert St François','Sortie'),
 (310,'Transfert Clairval','Sortie'),
 (311,'Transfert Vitrolles Maternité','Sortie'),
 (312,'Transfert Vitrolles Chirurgie','Sortie'),
 (313,'Transfert Clinique Martigues','Sortie'),
 (314,'Evasion','Sortie'),
 (315,'Sortie sans avoir vu le médecin','Sortie'),
 (316,'Transfert La Casamance','Sortie'),
 (317,'Police','Sortie');
INSERT INTO `DrTuxTest`.`synopt_destinations` VALUES  (318,'Décès','Sortie'),
 (319,'Sortie UTHCD','Sortie'),
 (320,'Transfert autre','Sortie'),
 (321,'Transfert Chanteclerc','Sortie');
UNLOCK TABLES;
/*!40000 ALTER TABLE `synopt_destinations` ENABLE KEYS */;


--
-- Definition of table `DrTuxTest`.`synopt_encours`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_encours`;
CREATE TABLE  `DrTuxTest`.`synopt_encours` (
  `EC_PK` int(10) unsigned NOT NULL auto_increment,
  `EC_CodeBox` varchar(45) collate utf8_unicode_ci NOT NULL,
  `EC_NomPatient` varchar(45) collate utf8_unicode_ci NOT NULL,
  `EC_HeureEntree` datetime NOT NULL,
  `EC_Medecin` varchar(45) collate utf8_unicode_ci default NULL,
  `EC_GUIDPatient` varchar(45) collate utf8_unicode_ci default NULL,
  `EC_Replier` tinyint(1) default '0',
  `EC_HeureSortie` datetime default NULL,
  `EC_Commentaire` varchar(45) collate utf8_unicode_ci default NULL,
  `EC_PrenomPatient` varchar(45) collate utf8_unicode_ci default NULL,
  `EC_ProgAnnexe` varchar(200) collate utf8_unicode_ci default NULL,
  `EC_ArgsAnnexe` varchar(400) collate utf8_unicode_ci default NULL,
  `EC_Destination` varchar(45) collate utf8_unicode_ci default NULL,
  `EC_Anonyme` tinyint(1) default NULL,
  PRIMARY KEY  (`EC_PK`)
) ENGINE=MyISAM AUTO_INCREMENT=4420 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


--
-- Definition of table `DrTuxTest`.`synopt_encours_taches`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_encours_taches`;
CREATE TABLE  `DrTuxTest`.`synopt_encours_taches` (
  `EN_PK_encours` int(10) unsigned NOT NULL,
  `EN_Num_tache` int(10) unsigned NOT NULL auto_increment,
  `EN_Code_tache` varchar(45) collate utf8_unicode_ci default NULL,
  `EN_Comment` varchar(200) collate utf8_unicode_ci default NULL,
  `EN_Etat_en_cours` varchar(45) collate utf8_unicode_ci default NULL,
  `EN_HeureDebPrevue` datetime default NULL,
  `EN_HeureFinPrevue` datetime default NULL,
  `EN_HeureDebut` datetime default NULL,
  `EN_HeureFin` datetime default NULL,
  `EN_Priorite` int(10) unsigned default NULL,
  `EN_Note_tache` varchar(5000) collate utf8_unicode_ci default NULL,
  `EN_Code_acteur` varchar(45) collate utf8_unicode_ci default NULL,
  `EN_PrimKey_blob` int(10) unsigned default NULL,
  `EN_NomFicNote` varchar(200) collate utf8_unicode_ci default NULL,
  `EN_NomProgNote` varchar(200) collate utf8_unicode_ci default NULL,
  PRIMARY KEY  (`EN_PK_encours`,`EN_Num_tache`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;



--
-- Definition of table `DrTuxTest`.`synopt_etats`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_etats`;
CREATE TABLE  `DrTuxTest`.`synopt_etats` (
  `ET_Code_etat` varchar(45) collate utf8_unicode_ci NOT NULL,
  `ET_Libelle_etat` varchar(45) collate utf8_unicode_ci default NULL,
  `ET_Couleur_etat` varchar(45) collate utf8_unicode_ci default NULL,
  `ET_Tache_terminee` tinyint(1) default NULL,
  PRIMARY KEY  (`ET_Code_etat`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `DrTuxTest`.`synopt_etats`
--

/*!40000 ALTER TABLE `synopt_etats` DISABLE KEYS */;
LOCK TABLES `synopt_etats` WRITE;
INSERT INTO `DrTuxTest`.`synopt_etats` VALUES  ('CO','Commencé','#ffaa7f',0),
 ('EN','En Cours','#ffff7f',0),
 ('FI','Terminé','#55aa7f',1),
 ('AT','Attente','#00FF00',NULL),
 ('ET','Etat','#aa557f',0),
 ('RIEN','Aucun état','#d4d09e',0),
 ('ANU','Annulé','#bcbeb8',1),
 ('PR','Prescrit','#aa0000',0),
 ('DM','Demandé','#ff0000',0),
 ('HU0','Usc zéro','#ffaaff',0),
 ('HU3','Usc troisième','#ff55ff',0),
 ('HA','Ambulatoire','#ffaaff',0),
 ('HC','Usic','#ffaaff',0),
 ('H1','Premier','#ffaaff',0),
 ('H2','Deuxième','#ffaaff',0),
 ('H3','Troisième','#ffaaff',0),
 ('H4','Quatrième','#ffaaff',0),
 ('H5','Cinquième','#ffaaff',0);
UNLOCK TABLES;
/*!40000 ALTER TABLE `synopt_etats` ENABLE KEYS */;

--
-- Definition of table `DrTuxTest`.`synopt_etats_taches`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_etats_taches`;
CREATE TABLE  `DrTuxTest`.`synopt_etats_taches` (
  `ST_Code_tache` varchar(45) collate utf8_unicode_ci NOT NULL,
  `ST_Code_etat` varchar(45) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`ST_Code_tache`,`ST_Code_etat`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `DrTuxTest`.`synopt_etats_taches`
--

/*!40000 ALTER TABLE `synopt_etats_taches` DISABLE KEYS */;
LOCK TABLES `synopt_etats_taches` WRITE;
INSERT INTO `DrTuxTest`.`synopt_etats_taches` VALUES  ('A1','AT'),
 ('A1','EN'),
 ('A1','FI'),
 ('A2','CO'),
 ('A2','ET'),
 ('A3','ANU'),
 ('A3','CO'),
 ('A3','FI'),
 ('A4','ANU'),
 ('A4','AT'),
 ('A4','FI'),
 ('A5','AT'),
 ('A5','EN'),
 ('A5','FI'),
 ('BIO C','DM'),
 ('BIO C','EN'),
 ('BIO C','FI'),
 ('BIO C','PR'),
 ('BIO CH','DM'),
 ('BIO CH','EN'),
 ('BIO CH','FI'),
 ('BIO CH','PR'),
 ('BIO H','DM'),
 ('BIO H','EN'),
 ('BIO H','FI'),
 ('BIO H','PR'),
 ('Bio Hem','DM'),
 ('Bio Hem','EN'),
 ('Bio Hem','FI'),
 ('BIO P','DM'),
 ('BIO P','EN'),
 ('BIO P','FI'),
 ('BIO P','PR'),
 ('BIO S','DM'),
 ('BIO S','EN'),
 ('BIO S','FI'),
 ('BIO S','PR'),
 ('BIO TM','DM'),
 ('BIO TM','EN'),
 ('BIO TM','FI'),
 ('BU','DM'),
 ('BU','FI'),
 ('CBU','DM'),
 ('CBU','EN'),
 ('CBU','FI'),
 ('CBU','PR'),
 ('CS A','DM'),
 ('CS A','EN'),
 ('CS A','FI'),
 ('CS A','PR'),
 ('CS C','DM'),
 ('CS C','EN'),
 ('CS C','FI'),
 ('CS C','PR'),
 ('CS D','DM'),
 ('CS D','EN'),
 ('CS D','FI'),
 ('CS D','PR'),
 ('CS G','DM'),
 ('CS G','EN');
INSERT INTO `DrTuxTest`.`synopt_etats_taches` VALUES  ('CS G','FI'),
 ('CS G','PR'),
 ('CS O','DM'),
 ('CS O','EN'),
 ('CS O','FI'),
 ('CS O','PR'),
 ('CS Ortho','DM'),
 ('CS Ortho','FI'),
 ('CS P','DM'),
 ('CS P','EN'),
 ('CS P','FI'),
 ('CS P','PR'),
 ('CS Phlebo','DM'),
 ('CS Phlebo','EN'),
 ('CS Phlebo','FI'),
 ('CS U','DM'),
 ('CS U','EN'),
 ('CS U','FI'),
 ('CS V','DM'),
 ('CS V','EN'),
 ('CS V','FI'),
 ('CS V','PR'),
 ('CS Vasc','DM'),
 ('CS Vasc','FI'),
 ('DST AMB','DM'),
 ('DST AMB','FI'),
 ('ECG','DM'),
 ('ECG','FI'),
 ('ECHO A','DM'),
 ('ECHO A','EN'),
 ('ECHO A','FI'),
 ('ECHO A','PR'),
 ('ECHO C','DM'),
 ('ECHO C','EN'),
 ('ECHO C','FI'),
 ('ECHO D','DM'),
 ('ECHO D','EN'),
 ('ECHO D','FI'),
 ('ECHO D','PR'),
 ('ECHO M','DM'),
 ('ECHO M','EN'),
 ('ECHO M','FI'),
 ('ECHO M','PR'),
 ('ECHO P','DM'),
 ('ECHO P','EN'),
 ('ECHO P','FI'),
 ('ECHO P','PR'),
 ('ECHO V','DM'),
 ('ECHO V','EN'),
 ('ECHO V','FI'),
 ('ECHO V','PR'),
 ('ED A','DM'),
 ('ED A','EN'),
 ('ED A','FI'),
 ('ED A','PR'),
 ('ED T','DM');
INSERT INTO `DrTuxTest`.`synopt_etats_taches` VALUES  ('ED T','EN'),
 ('ED T','FI'),
 ('ED T','PR'),
 ('ED V','DM'),
 ('ED V','EN'),
 ('ED V','FI'),
 ('ED V','PR'),
 ('GAZO','DM'),
 ('GAZO','EN'),
 ('GAZO','FI'),
 ('GAZO','PR'),
 ('H 1','AT'),
 ('H 1','DM'),
 ('H 1','FI'),
 ('H 2','AT'),
 ('H 2','DM'),
 ('H 2','FI'),
 ('H 3','AT'),
 ('H 3','DM'),
 ('H 3','FI'),
 ('H 4','AT'),
 ('H 4','DM'),
 ('H 4','FI'),
 ('H 5','AT'),
 ('H 5','DM'),
 ('H 5','FI'),
 ('H A','AT'),
 ('H A','DM'),
 ('H A','FI'),
 ('H G','AT'),
 ('H G','DM'),
 ('H G','EN'),
 ('H G','FI'),
 ('H G','H1'),
 ('H G','H2'),
 ('H G','H3'),
 ('H G','H4'),
 ('H G','H5'),
 ('H G','HA'),
 ('H G','HC'),
 ('H G','HU0'),
 ('H G','HU3'),
 ('H USC0','AT'),
 ('H USC0','DM'),
 ('H USC0','FI'),
 ('H USC3','AT'),
 ('H USC3','DM'),
 ('H USC3','FI'),
 ('H USIC','AT'),
 ('H USIC','DM'),
 ('H USIC','FI'),
 ('IRM','DM'),
 ('IRM','EN'),
 ('IRM','FI'),
 ('IRM','PR'),
 ('RX ASP C','DM'),
 ('RX ASP C','EN'),
 ('RX ASP C','FI'),
 ('RX ASP C','PR'),
 ('RX ASP D','DM'),
 ('RX ASP D','EN');
INSERT INTO `DrTuxTest`.`synopt_etats_taches` VALUES  ('RX ASP D','FI'),
 ('RX ASP D','PR'),
 ('RX OS','DM'),
 ('RX OS','EN'),
 ('RX OS','FI'),
 ('RX OS','PR'),
 ('RX THO','DM'),
 ('RX THO','EN'),
 ('RX THO','FI'),
 ('RX THO','PR'),
 ('SCAN A','DM'),
 ('SCAN A','EN'),
 ('SCAN A','FI'),
 ('SCAN A','PR'),
 ('SCAN C','DM'),
 ('SCAN C','EN'),
 ('SCAN C','FI'),
 ('SCAN C','PR'),
 ('SCAN D','DM'),
 ('SCAN D','EN'),
 ('SCAN D','FI'),
 ('SCAN D','PR'),
 ('SCAN RC','DM'),
 ('SCAN RC','EN'),
 ('SCAN RC','FI'),
 ('SCAN RC','PR'),
 ('SCAN RD','DM'),
 ('SCAN RD','EN'),
 ('SCAN RD','FI'),
 ('SCAN RD','PR'),
 ('SCAN RL','DM'),
 ('SCAN RL','EN'),
 ('SCAN RL','FI'),
 ('SCAN RL','PR'),
 ('SCAN T','DM'),
 ('SCAN T','EN'),
 ('SCAN T','FI'),
 ('SCAN T','PR'),
 ('SCAN U','DM'),
 ('SCAN U','EN'),
 ('SCAN U','FI'),
 ('SCAN U','PR');
UNLOCK TABLES;
/*!40000 ALTER TABLE `synopt_etats_taches` ENABLE KEYS */;


--
-- Definition of table `DrTuxTest`.`synopt_historique`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_historique`;
CREATE TABLE  `DrTuxTest`.`synopt_historique` (
  `HI_PK` int(10) unsigned NOT NULL auto_increment,
  `HI_Code_box` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Nom_patient` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Code_resp` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Code_tache` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Date` datetime default NULL,
  `HI_Libelle_tache` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Action` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_NumEncours` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Libelle_etat` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_PrenomPatient` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Commentaire` varchar(45) collate utf8_unicode_ci default NULL,
  `HI_Code_acteur` varchar(45) collate utf8_unicode_ci default NULL,
  PRIMARY KEY  (`HI_PK`)
) ENGINE=MyISAM AUTO_INCREMENT=14743 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Definition of table `DrTuxTest`.`synopt_taches`
--

DROP TABLE IF EXISTS `DrTuxTest`.`synopt_taches`;
CREATE TABLE  `DrTuxTest`.`synopt_taches` (
  `TA_Code_tache` varchar(45) collate utf8_unicode_ci NOT NULL,
  `TA_Libelle_tache` varchar(45) collate utf8_unicode_ci NOT NULL,
  `TA_Couleur_tache` varchar(45) collate utf8_unicode_ci default NULL,
  `TA_Duree_maxi` int(10) unsigned default NULL,
  `TA_Couleur_alarme` varchar(45) collate utf8_unicode_ci default NULL,
  `TA_BoutonMenu` varchar(45) collate utf8_unicode_ci NOT NULL,
  `TA_Priorite` varchar(10) collate utf8_unicode_ci default NULL,
  `TA_Commentaire` varchar(256) collate utf8_unicode_ci default NULL,
  PRIMARY KEY  (`TA_Code_tache`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `DrTuxTest`.`synopt_taches`
--

/*!40000 ALTER TABLE `synopt_taches` DISABLE KEYS */;
LOCK TABLES `synopt_taches` WRITE;
INSERT INTO `DrTuxTest`.`synopt_taches` VALUES  ('RX THO','Radio du thorax','#aaff7f',10,'#ffaa00','M','',''),
 ('RX ASP D','ASP debout de face et coupôles','#aaff7f',10,'#ffaa00','M','',''),
 ('RX ASP C','ASP couché','#aaff7f',10,'#ffaa00','M','',''),
 ('RX OS','Radio traumato','#aaff7f',10,'#ffaa00','M','',''),
 ('ECHO A','Echographie abdominale','#aaaa7f',10,'#ffaa00','M','',''),
 ('ECHO M','Echographie musculaire','#aaaa7f',10,'#ffaa00','M','',''),
 ('ECHO V','Echographie vésico prostatique','#aaaa7f',10,'#ffaa00','M','',''),
 ('ECHO P','Echographie parties molles','#aaaa7f',10,'#ffaa00','M','',''),
 ('ECHO D','Echographie parties diverses','#aaaa7f',10,'#ffaa00','M','',''),
 ('BIO C','Bilan cardio','#aa55ff',10,'#ffaa00','M','',''),
 ('BIO CH','Bilan cardio hépatique','#aa55ff',10,'#ffaa00','M','',''),
 ('BIO H','Bilan hépatique','#aa55ff',10,'#ffaa00','M','',''),
 ('BIO S','Bilan standard','#aa55ff',10,'#ffaa00','M','',''),
 ('BIO P','Prélèvement','#aa55ff',10,'#ffaa00','M','',''),
 ('GAZO','Gazo','#aaaa7f',10,'#ffaa00','M','','');
INSERT INTO `DrTuxTest`.`synopt_taches` VALUES  ('BU','BU','#ffff7f',10,'#ffaa00','M','',''),
 ('CBU','CBU','#ffff7f',10,'#ffaa00','M','',''),
 ('ECG','ECG','#ffaa7f',0,'#aaff7f','M','',''),
 ('ED V','Echo doppler veineuse','#FFEE22',5,'#ff0000','M','',''),
 ('ED T','Echo doppler TSA','#FFEE22',5,'#ff0000','M','',''),
 ('ED A','Echo doppler artériel','#FFEE22',5,'#ff0000','M','',''),
 ('IRM','IRM','#aa007f',5,'#ff0000','M','',''),
 ('CS C','Consultation cardio','#FFEE22',5,'#ff0000','M','',''),
 ('CS V','Consultation viscérale','#FFEE22',5,'#ff0000','M','',''),
 ('CS G','Consultation gastro','#FFEE22',5,'#ff0000','M','',''),
 ('CS O','Consultation ORL','#FFEE22',5,'#ff0000','M','',''),
 ('CS P','Consultation pneumo','#FFEE22',5,'#ff0000','M','',''),
 ('CS A','Consultation anesthésie','#FFEE22',5,'#ff0000','M','',''),
 ('CS D','Consultation autre','#FFEE22',5,'#ff0000','M','',''),
 ('SCAN A','Scanner abdominal','#ff5500',5,'#ff0000','M','',''),
 ('SCAN C','Scanner cérébral','#ff5500',30,'#FF0000','M','',''),
 ('SCAN U','Uro scanner','#ff5500',30,'#ff0000','M','','');
INSERT INTO `DrTuxTest`.`synopt_taches` VALUES  ('SCAN RC','Scanner rachis cervical','#ff5500',30,'#ff0000','M','',''),
 ('SCAN RD','Scanner rachis dorsal','#ff5500',30,'#ff0000','M','',''),
 ('SCAN RL','Scanner rachis lombaire','#ff5500',30,'#ff0000','M','',''),
 ('SCAN D','Scanner','#ff5500',30,'#FF0000','M','',''),
 ('SCAN T','Scanner thoracique','#ff5500',10,'#ff0000','M','',''),
 ('CS Ortho','Consultation orthopédique','#FFEE22',180,'#ff0000','M','1',''),
 ('CS Vasc','Consultation vasculaire','#ffff00',180,'#ff0000','M','1','Caro. 1681 Gari. 1045'),
 ('Bio Hem','Hémocultures','#aa55ff',60,'#ffaa00','M','5',''),
 ('DST AMB','Transport Ambulance','#0000ff',20,'#55ffff','M','1','tél 1105'),
 ('H USC0','Hospitalisation USC 0','#0055ff',15,'#55ffff','M','5','4675 Rea 4638'),
 ('H USC3','Hospitalisation USC 3','#0055ff',15,'#55ffff','M','5','4675 Réa 4638'),
 ('H USIC','Hospitalisation USIC','#0000ff',15,'#55ffff','M','5','4675 Cardio 4690'),
 ('H 1','Hospitalisation premier','#0000ff',15,'#55ffff','M','5','4276 4644 4730'),
 ('H 2','Hospitalisation deuxième','#0000ff',15,'#00ffff','M','5','4645 4284 4732');
INSERT INTO `DrTuxTest`.`synopt_taches` VALUES  ('H 3','Hospitalisation troisième','#0000ff',15,'#00ffff','M','5','4711 4265 4734'),
 ('H 4','Hospitalisation quatrième','#0000ff',15,'#00ffff','M','5','4647 4250 4736'),
 ('H 5','Hospitalisation cinquième','#0000ff',15,'#00ffff','M','5','4639 4243 4738'),
 ('H A','Hospitalisation ambulatoire','#0000ff',15,'#00ffff','M','5','4609 4281'),
 ('H G','Hospitalisation','#0000ff',15,'#00ffff','M','5','Préciser l\'étage avec le menu'),
 ('ECHO C','Echodoppler cardiaque','#aaaa7f',10,'#ffaa00','M','',''),
 ('BIO TM','Contrôle troponine myoglobine','#aa55ff',20,'#ffaa00','M','10','Labo 1078'),
 ('CS U','Consultation d\'urologie','#FFEE22',30,'#FF0000','M','5',''),
 ('CS Phlebo','Phlébologie échodoppler','#FFEE22',30,'#FF0000','M','5','Demander au 4632');
UNLOCK TABLES;
/*!40000 ALTER TABLE `synopt_taches` ENABLE KEYS */;




/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
