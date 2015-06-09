CREATE DATABASE  IF NOT EXISTS `drtuxtest` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `DrTuxTest`;
-- MySQL dump 10.13  Distrib 5.6.13, for osx10.6 (i386)
--
-- Host: localhost    Database: DrTuxTest
-- ------------------------------------------------------
-- Server version	5.5.10

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `global_settings`
--

DROP TABLE IF EXISTS `global_settings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `global_settings` (
  `global_settings_Section` varchar(255) DEFAULT NULL,
  `global_settings_pk` int(11) NOT NULL AUTO_INCREMENT,
  `global_settings_Key` varchar(255) DEFAULT NULL,
  `global_settings_Value_0` varchar(255) DEFAULT NULL,
  `global_settings_Value_1` varchar(255) DEFAULT NULL,
  `global_settings_Value_2` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`global_settings_pk`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `global_settings`
--

LOCK TABLES `global_settings` WRITE;
/*!40000 ALTER TABLE `global_settings` DISABLE KEYS */;
INSERT INTO `global_settings` VALUES ('Definition Rubriques',1,'Identification','20090000','ie',NULL),('Definition Rubriques',2,'Observation','20030000','ob',NULL),('Definition Rubriques',3,'Documents','20080000','do',NULL),('Definition Rubriques',4,'Prescription','20020000','or',NULL),('Definition Rubriques',5,'Terrain','20060000','at',NULL),('Definition Rubriques',6,'Vigie','20050000','cl',NULL),('Definition Rubriques',7,'Ophtalmologie','20070000','op',NULL),('Definition Rubriques',8,'Courrier','90010000','co',NULL),('Version',9,'DB_Version','2.16.000',NULL,NULL),('ImgConfig',10,'W_ImgMax','5000',NULL,NULL),('ImgConfig',11,'H_ImgMax','5000',NULL,NULL),('Mime',12,'text/medintux-ord','asynchronous','../../medicatux/bin/medicatux',NULL),('Definition Rubriques',13,'Corbeille','-10000000','--',NULL);
/*!40000 ALTER TABLE `global_settings` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-11-17  9:50:52
