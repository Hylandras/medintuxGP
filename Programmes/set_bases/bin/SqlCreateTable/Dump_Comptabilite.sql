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
-- Create schema Comptabilite
--

CREATE DATABASE IF NOT EXISTS Comptabilite;
USE Comptabilite;

--
-- Definition of table `Comptabilite`.`actes_disponibles`
--

DROP TABLE IF EXISTS `Comptabilite`.`actes_disponibles`;
CREATE TABLE  `Comptabilite`.`actes_disponibles` (
  `id_acte_dispo` int(10) unsigned NOT NULL auto_increment,
  `nom_acte` varchar(50) collate utf8_unicode_ci NOT NULL,
  `desc_acte` varchar(200) collate utf8_unicode_ci NOT NULL,
  `type` varchar(100) collate utf8_unicode_ci NOT NULL,
  `montant_total` double NOT NULL,
  `montant_tiers` double NOT NULL,
  PRIMARY KEY  (`id_acte_dispo`)
) ENGINE=MyISAM AUTO_INCREMENT=46 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`actes_disponibles`
--

/*!40000 ALTER TABLE `actes_disponibles` DISABLE KEYS */;
LOCK TABLES `actes_disponibles` WRITE;
INSERT INTO `Comptabilite`.`actes_disponibles` VALUES  (1,'C','Consultation','Consultation',22,6.6),
 (2,'MGE','Enfant de 2 Ã  6 ans','Majoration',3,1),
 (3,'MNO','Enfant de moins de deux ans','Majoration',5,1.5),
 (6,'MCG','Soins itÃ©ratifs en consultation par un autre mÃ©decin que le mÃ©decin traitant, sous rÃ©serve d\'un retour d\'information.','Majoration',3,1),
 (26,'VRN','Visite de 20h-24h et 6h-8h (appel aprÃ¨s 19h)','Majoration Gardes RÃ©gulÃ©es',46,13.8),
 (25,'VRD','Visite dimanche et fÃ©riÃ©s de jour','Majoration Gardes RÃ©gulÃ©es',30,9),
 (23,'CRN','Consultation de 20h-24h et 6h-8h (appel aprÃ¨s 19h)\n','Majoration Gardes RÃ©gulÃ©es',42.5,12.75),
 (18,'Dep2','DÃ©passement de deux euros','DÃ©passement',2,0),
 (24,'CRM','Consultation de 00h Ã  06h','Majoration Gardes RÃ©gulÃ©es',51.5,15.45),
 (22,'CRD','Consultation dimanche et fÃ©riÃ©s de jour','Majoration Gardes RÃ©gulÃ©es',26.5,7.95),
 (17,'CA','Consultation annuelle des ALD','Consultation',26,22),
 (19,'Dep3','DÃ©passement de trois euros','DÃ©passement',3,0);
INSERT INTO `Comptabilite`.`actes_disponibles` VALUES  (20,'V','Visite Ã  domicile (ajouter MD si la visite est justifiÃ©e)','Visite',22,15.4),
 (21,'MD','Si visite justifiÃ©e ou ECG en visite','Majoration',10,7),
 (27,'VRM','Visites de 00h Ã  06h','Majoration Gardes RÃ©gulÃ©es',55,16.5),
 (28,'FPE','Examens obligatoires du nourrisson jusqu\'au 24Ã¨me mois','Majoration',5,1.5),
 (29,'ID','Actes techniques Ã  domicile (non utilisable avec le V seul)','Visite',3.5,1.05),
 (32,'Dep1','DÃ©passement d\'honoraire d\'un euro','DÃ©passement',1,0),
 (33,'RÃ©trocession d\'honoraires','Enregistrez ici les rÃ©trocessions d\'honoraires pour intÃ©gration Ã  la comptabilitÃ©','RÃ©trocession',0,0),
 (34,'Garde - Samedi 12h-20h','Forfait de garde : pour les gardes rÃ©gulÃ©es le samedi de 12h Ã  20h.','Forfaits gardes / RÃ©quisitions',100,0),
 (35,'Garde - Samedi 20h-00h','Forfaits gardes pour les gardes rÃ©gulÃ©es le samedi de 20h Ã  00h.','Forfaits gardes / RÃ©quisitions',50,0),
 (36,'Garde - Nuit 00h-08h','Forfaits gardes rÃ©gulÃ©es de nuit 00h Ã  08h.','Forfaits gardes / RÃ©quisitions',100,0);
INSERT INTO `Comptabilite`.`actes_disponibles` VALUES  (37,'Garde - Dimanche 08h-20h','Forfaits gardes rÃ©gulÃ©es de dimanche journÃ©e : 08h Ã  20h.','Forfaits gardes / RÃ©quisitions',150,0),
 (38,'Garde - Dimanche 20h-00h','Forfaits gardes rÃ©gulÃ©es de dimanche entre 20h et 00h','Forfaits gardes / RÃ©quisitions',50,0),
 (39,'Garde - Dimanche Nuit 00h Ã  08h','Forfaits gardes rÃ©gulÃ©es de dimanche nuit : 00h Ã  08h.','Forfaits gardes / RÃ©quisitions',100,0),
 (40,'RÃ©quisition garde Ã  vue','Envoyer l\'ordre de rÃ©quisition et la fiche d\'honoraire au tribunal de grande instance.','Forfaits gardes / RÃ©quisitions',50,0),
 (41,'Bilan mÃ©moire','ALQP006 (69,12)','Cs GÃ©riatriques',75,0),
 (42,'RMT (forfait annuel)','','Forfaits',40,0),
 (43,'Samedi AM no n rÃ©gulÃ©','F','Majoration Gardes RÃ©gulÃ©es',19.06,0),
 (44,'SAmedi Nuit aprÃ¨s 20h','N','Majoration Gardes RÃ©gulÃ©es',35,0),
 (45,'HAD','','Forfaits',50,0);
UNLOCK TABLES;
/*!40000 ALTER TABLE `actes_disponibles` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`comptes_bancaires`
--

DROP TABLE IF EXISTS `Comptabilite`.`comptes_bancaires`;
CREATE TABLE  `Comptabilite`.`comptes_bancaires` (
  `id_compte` int(10) unsigned NOT NULL auto_increment,
  `id_usr` int(10) unsigned NOT NULL,
  `libelle` varchar(150) collate utf8_unicode_ci default NULL,
  `titulaire` varchar(100) collate utf8_unicode_ci default NULL,
  `nom_banque` varchar(50) collate utf8_unicode_ci default NULL,
  `rib_code_banque` varchar(5) collate utf8_unicode_ci default NULL,
  `rib_code_guichet` varchar(5) collate utf8_unicode_ci default NULL,
  `rib_numcompte` varchar(11) collate utf8_unicode_ci default NULL,
  `rib_cle` varchar(2) collate utf8_unicode_ci default NULL,
  `solde_initial` double default '0',
  `remarque` blob,
  PRIMARY KEY  (`id_compte`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`comptes_bancaires`
--

/*!40000 ALTER TABLE `comptes_bancaires` DISABLE KEYS */;
LOCK TABLES `comptes_bancaires` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `comptes_bancaires` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`depots`
--

DROP TABLE IF EXISTS `Comptabilite`.`depots`;
CREATE TABLE  `Comptabilite`.`depots` (
  `id_depot` int(11) NOT NULL auto_increment,
  `id_usr` int(11) NOT NULL,
  `id_compte` int(10) unsigned NOT NULL,
  `type_depot` varchar(10) collate utf8_unicode_ci default NULL,
  `date` date NOT NULL,
  `periode_deb` date NOT NULL,
  `periode_fin` date NOT NULL,
  `blob_depot` blob NOT NULL,
  `remarque` blob,
  `valide` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id_depot`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`depots`
--

/*!40000 ALTER TABLE `depots` DISABLE KEYS */;
LOCK TABLES `depots` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `depots` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`honoraires`
--

DROP TABLE IF EXISTS `Comptabilite`.`honoraires`;
CREATE TABLE  `Comptabilite`.`honoraires` (
  `id_hono` int(11) unsigned NOT NULL auto_increment,
  `id_usr` int(11) NOT NULL,
  `id_drtux_usr` int(11) default NULL,
  `patient` varchar(75) collate utf8_unicode_ci NOT NULL,
  `GUID` varchar(36) collate utf8_unicode_ci default NULL,
  `praticien` varchar(75) collate utf8_unicode_ci NOT NULL,
  `date` date NOT NULL,
  `acte` blob,
  `remarque` blob,
  `esp` double NOT NULL,
  `chq` double NOT NULL,
  `cb` double NOT NULL,
  `daf` double NOT NULL,
  `autre` double NOT NULL,
  `du` double NOT NULL,
  `valide` tinyint(1) default '0',
  `tracabilite` blob,
  PRIMARY KEY  (`id_hono`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`honoraires`
--

/*!40000 ALTER TABLE `honoraires` DISABLE KEYS */;
LOCK TABLES `honoraires` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `honoraires` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`immobilisations`
--

DROP TABLE IF EXISTS `Comptabilite`.`immobilisations`;
CREATE TABLE  `Comptabilite`.`immobilisations` (
  `id_immob` int(10) unsigned NOT NULL auto_increment,
  `id_usr` int(10) unsigned NOT NULL,
  `id_compte` int(10) unsigned NOT NULL,
  `libelle` varchar(150) collate utf8_unicode_ci default NULL,
  `date_service` date NOT NULL,
  `duree` int(11) NOT NULL,
  `mode` tinyint(1) NOT NULL,
  `valeur` bigint(20) unsigned NOT NULL,
  `montant_tva` double default NULL,
  `valeur_residuelle` bigint(20) NOT NULL,
  `resultat` blob NOT NULL,
  `mouvements` blob,
  `remarque` blob,
  `tracabilite` blob,
  PRIMARY KEY  (`id_immob`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`immobilisations`
--

/*!40000 ALTER TABLE `immobilisations` DISABLE KEYS */;
LOCK TABLES `immobilisations` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `immobilisations` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`mouvements`
--

DROP TABLE IF EXISTS `Comptabilite`.`mouvements`;
CREATE TABLE  `Comptabilite`.`mouvements` (
  `id_mouvement` int(10) unsigned NOT NULL auto_increment,
  `id_mvt_dispo` int(11) NOT NULL,
  `id_usr` int(10) unsigned NOT NULL,
  `id_compte` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL default '0',
  `libelle` varchar(100) collate utf8_unicode_ci NOT NULL,
  `date` date NOT NULL,
  `date_valeur` date NOT NULL,
  `montant` double unsigned NOT NULL default '0',
  `remarque` blob,
  `valide` tinyint(4) NOT NULL default '0',
  `tracabilite` blob,
  PRIMARY KEY  (`id_mouvement`),
  KEY `montant` (`montant`),
  KEY `date_valeur` (`date_valeur`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`mouvements`
--

/*!40000 ALTER TABLE `mouvements` DISABLE KEYS */;
LOCK TABLES `mouvements` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `mouvements` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`mouvements_disponibles`
--

DROP TABLE IF EXISTS `Comptabilite`.`mouvements_disponibles`;
CREATE TABLE  `Comptabilite`.`mouvements_disponibles` (
  `id_mvt_dispo` int(10) unsigned NOT NULL auto_increment,
  `id_mvt_parent` int(11) default NULL,
  `type` tinyint(3) unsigned NOT NULL default '0',
  `libelle` varchar(100) collate utf8_unicode_ci NOT NULL,
  `remarque` blob,
  PRIMARY KEY  (`id_mvt_dispo`)
) ENGINE=MyISAM AUTO_INCREMENT=44 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`mouvements_disponibles`
--

/*!40000 ALTER TABLE `mouvements_disponibles` DISABLE KEYS */;
LOCK TABLES `mouvements_disponibles` WRITE;
INSERT INTO `Comptabilite`.`mouvements_disponibles` VALUES  (1,0,0,'Apports praticien',0x7C6C6E3D4146),
 (2,0,0,'Autres recettes',0x7C6C6E3D4146),
 (3,0,0,'Produits financiers',0x7C6C6E3D4145),
 (4,0,1,'TVA rÃ©cupÃ©rable',0x4E554C4C),
 (5,0,1,'PrÃ©lÃ¨vements du praticien',0x4E554C4C),
 (6,0,1,'Achats : fournitures et pharmacie',0x4E554C4C),
 (7,0,1,'Frais de personnel',0x4E554C4C),
 (8,6,1,'Salaires nets',0x7C6C6E3D4242),
 (9,6,1,'Charges sociales',0x7C6C6E3D4243),
 (10,0,1,'ImpÃ´ts et Taxes',0x4E554C4C),
 (11,10,1,'TVA PayÃ©e',0x4E554C4C),
 (12,10,1,'Taxe Professionnelle',0x7C6C6E3D4245),
 (13,10,1,'Autres impÃ´ts',0x7C6C6E3D4253),
 (14,0,1,'Travaux, Fournitures, Services ExtÃ©rieurs',0x4E554C4C),
 (15,14,1,'Loyers et charges',0x7C6C6E3D4246),
 (16,14,1,'Location matÃ©riel',0x7C6C6E3D4247),
 (17,14,1,'RÃ©paration et entretien',0x7C6C6E3D4248),
 (18,14,1,'Personnel intÃ©rim, secrÃ©tariat tÃ©lÃ©phonique',0x7C6C6E3D4248),
 (19,14,1,'Petit outillage',0x7C6C6E3D4248),
 (20,14,1,'Gaz, Ã©lectricitÃ©, chauffage, eau',0x7C6C6E3D4248);
INSERT INTO `Comptabilite`.`mouvements_disponibles` VALUES  (21,14,1,'Honoraires rÃ©trocÃ©dÃ©s',0x4E554C4C),
 (22,14,1,'Honoraires ne constituant pas de rÃ©trocession',0x7C6C6E3D4248),
 (23,14,1,'Assurances',0x7C6C6E3D4248),
 (24,0,1,'Transports et dÃ©placements',0x4E554C4C),
 (25,24,1,'Frais de voiture',0x7C6C6E3D424A),
 (26,24,1,'Frais moto',0x7C6C6E3D424A),
 (27,24,1,'Autres frais de dÃ©placement',0x7C6C6E3D424A),
 (28,0,1,'Charges sociales du praticien',0x4E554C4C),
 (29,28,1,'Vieillesse, Assurance Maladie, Alloc. Fam.',0x7C6C6E3D424D),
 (30,0,1,'Frais divers de gestion',0x7C6C6E3D424D),
 (31,30,1,'CongrÃ¨s',0x4E554C4C),
 (32,30,1,'Cadeaux, reprÃ©sentation et rÃ©ception',0x4E554C4C),
 (33,30,1,'Frais de bureau, documentation, et P et T',0x4E554C4C),
 (34,30,1,'Cotisation professionnelle et syndicales',0x7C6C6E3D424D),
 (35,30,1,'Divers',0x7C6C6E3D424D),
 (36,0,1,'Frais financiers',0x7C6C6E3D424E),
 (37,0,1,'Pertes',''),
 (38,0,1,'DÃ©bours payÃ©s pour le compte des clients',0x4E554C4C),
 (39,0,1,'Autres dÃ©penses',0x4E554C4C);
INSERT INTO `Comptabilite`.`mouvements_disponibles` VALUES  (40,39,1,'SCM ou partage de frais',0x4E554C4C),
 (41,39,1,'Immobilisations',0x4E554C4C),
 (42,39,1,'Divers Ã  rÃ©intÃ©grer',0x7C6C6E3D4343),
 (43,NULL,1,'Comptes d\'attente',NULL);
UNLOCK TABLES;
/*!40000 ALTER TABLE `mouvements_disponibles` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`utilisateurs`
--

DROP TABLE IF EXISTS `Comptabilite`.`utilisateurs`;
CREATE TABLE  `Comptabilite`.`utilisateurs` (
  `id_usr` int(10) unsigned NOT NULL auto_increment,
  `nom_usr` varchar(150) collate utf8_unicode_ci default NULL,
  `login` varchar(15) collate utf8_unicode_ci NOT NULL,
  `password` varchar(50) collate utf8_unicode_ci NOT NULL,
  `id_drtux_usr` int(11) default NULL,
  PRIMARY KEY  (`id_usr`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`utilisateurs`
--

/*!40000 ALTER TABLE `utilisateurs` DISABLE KEYS */;
LOCK TABLES `utilisateurs` WRITE;
INSERT INTO `Comptabilite`.`utilisateurs` VALUES  (1,'Mr Admin User','admin','',4);
UNLOCK TABLES;
/*!40000 ALTER TABLE `utilisateurs` ENABLE KEYS */;


--
-- Definition of table `Comptabilite`.`z_version`
--

DROP TABLE IF EXISTS `Comptabilite`.`z_version`;
CREATE TABLE  `Comptabilite`.`z_version` (
  `infos_version` blob
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `Comptabilite`.`z_version`
--

/*!40000 ALTER TABLE `z_version` DISABLE KEYS */;
LOCK TABLES `z_version` WRITE;
INSERT INTO `Comptabilite`.`z_version` VALUES  (0x0A5B56657273696F6E5D0A20206E6F6D202020203D20436F6D70746162696C6974C3A920706F7572204D6564696E5475780A20206E756D202020203D20312E31310A2020737562762020203D20737461626C652072656C656173650A2020646174652020203D203238204A75696E20323030380A20206372656174696F6E203D2053657074656D62726520323030370A2020617574657572203D2044722045726963204D41454B45520A20206C69656E3120203D20687474703A2F2F7777772E657269636D61656B65722E66722F0A20206C69656E3220203D20687474703A2F2F7777772E6D6564696E7475782E6F72672F0A2020696E666F3120203D2043657474652076657273696F6E207065757820636F6E74656E69722064657320657272657572732064652070726F6772616D6D6174696F6E206F752064652063616C63756C2E0A2020696E666F3220203D204C27617574657572206E65207065757420C3AA7472652074656E7520726573706F6E7361626C652064657320657272657572732065742F6F7520706572746573206C69C3A9657320C3A0206C277574696C69736174696F6E206465206365206C6F67696369656C2E0A2020696E666F3320203D204D6572636920646520666169726520766F7320726170706F727473206465206275677320646570756973203A200A2020696E666F3420203D206C65207369746520696E7465726E6574206465206C276175746575722028687474703A2F2F7777772E657269636D61656B65722E66722F290A2020696E666F3520203D206F7520737572206C6520736974652064274164756C6C616374202868747470733A2F2F6164756C6C6163742E6E65742F70726F6A656374732F6D6564696E7475782F292E0A20206C6963656E636520203D20436543494C4C2076657273696F6E20320A20206D6564696E747578203D2076322E303020657420737570C3A97269657572650A20206C616E6761676520203D20432B2B2F5174332F4D7953514C0A0A5B52656D65726369656D656E74735D0A20206C31203D20C38020526F6C616E6420536576696E20706F757220736F6E2061696465207072C3A96369657573652064616E73206C612070726F6772616D6D6174696F6E206574206C652064C3A962756767616765206465206C276170706C69636174696F6E2E0A20206C32203D20C3802047C3A9726172642044656C61666F6E6420706F757220736F6E20616964652061752064C3A9627567676167652C2073657320636F7272656374696F6E7320657420736573206170706F7274732E0A20206C33203D20C3802042656E6A616D696E20506F7175657420706F75722073657320696E666F726D6174696F6E7320636F6E6365726E616E74206C657320667569746573206DC3A96D6F697265732065742067657374696F6E20646520706F696E746575727320656E20432B2B2E0A0A);
UNLOCK TABLES;
/*!40000 ALTER TABLE `z_version` ENABLE KEYS */;




/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
