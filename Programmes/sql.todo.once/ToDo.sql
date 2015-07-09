-- phpMyAdmin SQL Dump
-- version 4.2.12deb2
-- http://www.phpmyadmin.net
--
-- Client :  localhost
-- Généré le :  Jeu 09 Juillet 2015 à 16:26
-- Version du serveur :  5.6.24-0ubuntu2
-- Version de PHP :  5.6.4-4ubuntu6.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données :  `DrTuxTest`
--

-- --------------------------------------------------------

--
-- Structure de la table `ToDo`
--

CREATE TABLE IF NOT EXISTS `ToDo` (
`PrimKey` bigint(20) unsigned NOT NULL,
  `DateReminder` date NOT NULL DEFAULT '0000-00-00' COMMENT 'Date for reminding calendar',
  `DueDate` date NOT NULL DEFAULT '0000-00-00' COMMENT 'Date for todo list',
  `Title` varchar(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `Todo` longblob,
  `Note` longblob,
  `Memo_PK` bigint(20) unsigned NOT NULL,
  `Observation_PK` bigint(20) unsigned DEFAULT NULL,
  `Agenda_PK` bigint(20) unsigned DEFAULT NULL,
  `GUID` varchar(40) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Index pour les tables exportées
--

--
-- Index pour la table `ToDo`
--
ALTER TABLE `ToDo`
 ADD PRIMARY KEY (`PrimKey`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `ToDo`
--
ALTER TABLE `ToDo`
MODIFY `PrimKey` bigint(20) unsigned NOT NULL AUTO_INCREMENT;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
