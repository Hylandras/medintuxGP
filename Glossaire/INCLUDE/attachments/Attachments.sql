-- phpMyAdmin SQL Dump
-- version 4.2.12deb2
-- http://www.phpmyadmin.net
--
-- Client :  localhost
-- Généré le :  Sam 25 Juillet 2015 à 14:28
-- Version du serveur :  5.6.25-0ubuntu0.15.04.1
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
-- Structure de la table `Attachments`
--

CREATE TABLE IF NOT EXISTS `Attachments` (
`PrimKey` bigint(20) unsigned NOT NULL,
  `Doc_PK` bigint(20) unsigned NOT NULL,
  `File_Name` mediumtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `Doc_Name` mediumtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `Date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `File_Type` varchar(3) NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Index pour les tables exportées
--

--
-- Index pour la table `Attachments`
--
ALTER TABLE `Attachments`
 ADD PRIMARY KEY (`PrimKey`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `Attachments`
--
ALTER TABLE `Attachments`
MODIFY `PrimKey` bigint(20) unsigned NOT NULL AUTO_INCREMENT;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
