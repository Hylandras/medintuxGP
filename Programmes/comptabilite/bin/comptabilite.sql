
CREATE TABLE IF NOT EXISTS `actes_disponibles` (
  `id_acte_dispo` int(10) unsigned NOT NULL auto_increment,
  `nom_acte` varchar(50) collate utf8_bin NOT NULL,
  `desc_acte` varchar(200) collate utf8_bin NOT NULL,
  `type` varchar(100) collate utf8_bin NOT NULL,
  `montant_total` double NOT NULL,
  `montant_tiers` double NOT NULL,
  PRIMARY KEY  (`id_acte_dispo`)
) ENGINE=MyISAM;

INSERT INTO `actes_disponibles` (`id_acte_dispo`, `nom_acte`, `desc_acte`, `type`, `montant_total`, `montant_tiers`) VALUES
(1, 'C', 'Consultation', 'Consultation', 22, 6.6),
(2, 'MGE', 'Enfant de 2 à 6 ans', 'Majoration', 3, 1),
(3, 'MNO', 'Enfant de moins de deux ans', 'Majoration', 5, 1.5),
(6, 'MCG', 'Soins itératifs en consultation par un autre médecin que le médecin traitant, sous réserve d''un retour d''information.', 'Majoration', 3, 1),
(26, 'VRN', 'Visite de 20h-24h et 6h-8h (appel après 19h)', 'Majoration Gardes Régulées', 46, 13.8),
(25, 'VRD', 'Visite dimanche et fériés de jour', 'Majoration Gardes Régulées', 30, 9),
(23, 'CRN', 'Consultation de 20h-24h et 6h-8h (appel après 19h)\n', 'Majoration Gardes Régulées', 42.5, 12.75),
(18, 'Dep2', 'Dépassement de deux euros', 'Dépassement', 2, 0),
(24, 'CRM', 'Consultation de 00h à 06h', 'Majoration Gardes Régulées', 51.5, 15.45),
(22, 'CRD', 'Consultation dimanche et fériés de jour', 'Majoration Gardes Régulées', 26.5, 7.95),
(17, 'CA', 'Consultation annuelle des ALD', 'Consultation', 26, 22),
(19, 'Dep3', 'Dépassement de trois euros', 'Dépassement', 3, 0),
(20, 'V', 'Visite à domicile (ajouter MD si la visite est justifiée)', 'Visite', 22, 15.4),
(21, 'MD', 'Si visite justifiée ou ECG en visite', 'Majoration', 10, 7),
(27, 'VRM', 'Visites de 00h à 06h', 'Majoration Gardes Régulées', 55, 16.5),
(28, 'FPE', 'Examens obligatoires du nourrisson jusqu''au 24ème mois', 'Majoration', 5, 1.5),
(29, 'ID', 'Actes techniques à domicile (non utilisable avec le V seul)', 'Visite', 3.5, 1.05),
(32, 'Dep1', 'Dépassement d''honoraire d''un euro', 'Dépassement', 1, 0),
(33, 'Rétrocession d''honoraires', 'Enregistrez ici les rétrocessions d''honoraires pour intégration à la comptabilité', 'Rétrocession', 0, 0),
(34, 'Garde - Samedi 12h-20h', 'Forfait de garde : pour les gardes régulées le samedi de 12h à 20h.', 'Forfaits gardes / Réquisitions', 100, 0),
(35, 'Garde - Samedi 20h-00h', 'Forfaits gardes pour les gardes régulées le samedi de 20h à 00h.', 'Forfaits gardes / Réquisitions', 50, 0),
(36, 'Garde - Nuit 00h-08h', 'Forfaits gardes régulées de nuit 00h à 08h.', 'Forfaits gardes / Réquisitions', 100, 0),
(37, 'Garde - Dimanche 08h-20h', 'Forfaits gardes régulées de dimanche journée : 08h à 20h.', 'Forfaits gardes / Réquisitions', 150, 0),
(38, 'Garde - Dimanche 20h-00h', 'Forfaits gardes régulées de dimanche entre 20h et 00h', 'Forfaits gardes / Réquisitions', 50, 0),
(39, 'Garde - Dimanche Nuit 00h à 08h', 'Forfaits gardes régulées de dimanche nuit : 00h à 08h.', 'Forfaits gardes / Réquisitions', 100, 0),
(40, 'Réquisition garde à vue', 'Envoyer l''ordre de réquisition et la fiche d''honoraire au tribunal de grande instance.', 'Forfaits gardes / Réquisitions', 50, 0),
(41, 'Bilan mémoire', 'ALQP006 (69,12)', 'Cs Gériatriques', 75, 0),
(42, 'RMT (forfait annuel)', '', 'Forfaits', 40, 0),
(43, 'Samedi AM no n régulé', 'F', 'Majoration Gardes Régulées', 19.06, 0),
(44, 'SAmedi Nuit après 20h', 'N', 'Majoration Gardes Régulées', 35, 0),
(45, 'HAD', '', 'Forfaits', 50, 0);

CREATE TABLE IF NOT EXISTS `comptes_bancaires` (
  `id_compte` int(10) unsigned NOT NULL auto_increment,
  `id_usr` int(10) unsigned NOT NULL,
  `libelle` varchar(150) collate utf8_bin default NULL,
  `titulaire` varchar(100) collate utf8_bin default NULL,
  `nom_banque` varchar(50) collate utf8_bin default NULL,
  `rib_code_banque` varchar(5) collate utf8_bin default NULL,
  `rib_code_guichet` varchar(5) collate utf8_bin default NULL,
  `rib_numcompte` varchar(11) collate utf8_bin default NULL,
  `rib_cle` varchar(2) collate utf8_bin default NULL,
  `solde_initial` double default '0',
  `remarque` blob,
  PRIMARY KEY  (`id_compte`)
) ENGINE=MyISAM;


CREATE TABLE IF NOT EXISTS `depots` (
  `id_depot` int(11) NOT NULL auto_increment,
  `id_usr` int(11) NOT NULL,
  `id_compte` int(10) unsigned NOT NULL,
  `type_depot` varchar(10) collate utf8_bin default NULL,
  `date` date NOT NULL,
  `periode_deb` date NOT NULL,
  `periode_fin` date NOT NULL,
  `blob_depot` blob NOT NULL,
  `remarque` blob,
  `valide` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id_depot`)
) ENGINE=MyISAM;


CREATE TABLE IF NOT EXISTS `honoraires` (
  `id_hono` int(11) unsigned NOT NULL auto_increment,
  `id_usr` int(11) NOT NULL,
  `id_drtux_usr` int(11) default NULL,
  `patient` varchar(75) collate utf8_bin NOT NULL,
  `GUID` varchar(36) collate utf8_bin default NULL,
  `praticien` varchar(75) collate utf8_bin NOT NULL,
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
) ENGINE=MyISAM;


CREATE TABLE IF NOT EXISTS `immobilisations` (
  `id_immob` int(10) unsigned NOT NULL auto_increment,
  `id_usr` int(10) unsigned NOT NULL,
  `id_compte` int(10) unsigned NOT NULL,
  `libelle` varchar(150) collate utf8_bin default NULL,
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
) ENGINE=MyISAM;


CREATE TABLE IF NOT EXISTS `mouvements` (
  `id_mouvement` int(10) unsigned NOT NULL auto_increment,
  `id_mvt_dispo` int(11) NOT NULL,
  `id_usr` int(10) unsigned NOT NULL,
  `id_compte` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL default '0',
  `libelle` varchar(100) collate utf8_bin NOT NULL,
  `date` date NOT NULL,
  `date_valeur` date NOT NULL,
  `montant` double unsigned NOT NULL default '0',
  `remarque` blob,
  `valide` tinyint(4) NOT NULL default '0',
  `tracabilite` blob,
  PRIMARY KEY  (`id_mouvement`),
  KEY `montant` (`montant`),
  KEY `date_valeur` (`date_valeur`)
) ENGINE=MyISAM;


CREATE TABLE IF NOT EXISTS `mouvements_disponibles` (
  `id_mvt_dispo` int(10) unsigned NOT NULL auto_increment,
  `id_mvt_parent` int(11) default NULL,
  `type` tinyint(3) unsigned NOT NULL default '0',
  `libelle` varchar(100) collate utf8_bin NOT NULL,
  `remarque` blob,
  PRIMARY KEY  (`id_mvt_dispo`)
) ENGINE=MyISAM;

INSERT INTO `mouvements_disponibles` (`id_mvt_dispo`, `id_mvt_parent`, `type`, `libelle`, `remarque`) VALUES
(1, 0, 0, 'Apports praticien', '|ln=AF'),
(2, 0, 0, 'Autres recettes', '|ln=AF'),
(3, 0, 0, 'Produits financiers', '|ln=AE'),
(4, 0, 1, 'TVA récupérable', 'NULL'),
(5, 0, 1, 'Prélèvements du praticien', 'NULL'),
(6, 0, 1, 'Achats : fournitures et pharmacie', 'NULL'),
(7, 0, 1, 'Frais de personnel', 'NULL'),
(8, 6, 1, 'Salaires nets', '|ln=BB'),
(9, 6, 1, 'Charges sociales', '|ln=BC'),
(10, 0, 1, 'Impôts et Taxes', 'NULL'),
(11, 10, 1, 'TVA Payée', 'NULL'),
(12, 10, 1, 'Taxe Professionnelle', '|ln=BE'),
(13, 10, 1, 'Autres impôts', '|ln=BS'),
(14, 0, 1, 'Travaux, Fournitures, Services Extérieurs', 'NULL'),
(15, 14, 1, 'Loyers et charges', '|ln=BF'),
(16, 14, 1, 'Location matériel', '|ln=BG'),
(17, 14, 1, 'Réparation et entretien', '|ln=BH'),
(18, 14, 1, 'Personnel intérim, secrétariat téléphonique', '|ln=BH'),
(19, 14, 1, 'Petit outillage', '|ln=BH'),
(20, 14, 1, 'Gaz, électricité, chauffage, eau', '|ln=BH'),
(21, 14, 1, 'Honoraires rétrocédés', 'NULL'),
(22, 14, 1, 'Honoraires ne constituant pas de rétrocession', '|ln=BH'),
(23, 14, 1, 'Assurances', '|ln=BH'),
(24, 0, 1, 'Transports et déplacements', 'NULL'),
(25, 24, 1, 'Frais de voiture', '|ln=BJ'),
(26, 24, 1, 'Frais moto', '|ln=BJ'),
(27, 24, 1, 'Autres frais de déplacement', '|ln=BJ'),
(28, 0, 1, 'Charges sociales du praticien', 'NULL'),
(29, 28, 1, 'Vieillesse, Assurance Maladie, Alloc. Fam.', '|ln=BM'),
(30, 0, 1, 'Frais divers de gestion', '|ln=BM'),
(31, 30, 1, 'Congrès', 'NULL'),
(32, 30, 1, 'Cadeaux, représentation et réception', 'NULL'),
(33, 30, 1, 'Frais de bureau, documentation, et P et T', 'NULL'),
(34, 30, 1, 'Cotisation professionnelle et syndicales', '|ln=BM'),
(35, 30, 1, 'Divers', '|ln=BM'),
(36, 0, 1, 'Frais financiers', '|ln=BN'),
(37, 0, 1, 'Pertes', ''),
(38, 0, 1, 'Débours payés pour le compte des clients', 'NULL'),
(39, 0, 1, 'Autres dépenses', 'NULL'),
(40, 39, 1, 'SCM ou partage de frais', 'NULL'),
(41, 39, 1, 'Immobilisations', 'NULL'),
(42, 39, 1, 'Divers à réintégrer', '|ln=CC'),
(43, NULL, 1, 'Comptes d''attente', NULL);

CREATE TABLE IF NOT EXISTS `utilisateurs` (
  `id_usr` int(10) unsigned NOT NULL auto_increment,
  `nom_usr` varchar(150) collate utf8_bin default NULL,
  `login` varchar(15) collate utf8_bin NOT NULL,
  `password` varchar(50) collate utf8_bin NOT NULL,
  `id_drtux_usr` int(11) default NULL,
  PRIMARY KEY  (`id_usr`)
) ENGINE=MyISAM;


CREATE TABLE IF NOT EXISTS `z_version` (
  `infos_version` blob
) ENGINE=MyISAM;

INSERT INTO `comptabilite`.`z_version` (`infos_version`) VALUES (0x0a5b56657273696f6e5d0a20206e6f6d202020203d20436f6d70746162696c6974c3a920706f7572204d6564696e5475780a20206e756d202020203d20312e31310a2020737562762020203d20737461626c652072656c656173650a2020646174652020203d203238204a75696e20323030380a20206372656174696f6e203d2053657074656d62726520323030370a2020617574657572203d2044722045726963204d41454b45520a20206c69656e3120203d20687474703a2f2f7777772e657269636d61656b65722e66722f0a20206c69656e3220203d20687474703a2f2f7777772e6d6564696e7475782e6f72672f0a2020696e666f3120203d2043657474652076657273696f6e207065757820636f6e74656e69722064657320657272657572732064652070726f6772616d6d6174696f6e206f752064652063616c63756c2e0a2020696e666f3220203d204c27617574657572206e65207065757420c3aa7472652074656e7520726573706f6e7361626c652064657320657272657572732065742f6f7520706572746573206c69c3a9657320c3a0206c277574696c69736174696f6e206465206365206c6f67696369656c2e0a2020696e666f3320203d204d6572636920646520666169726520766f7320726170706f727473206465206275677320646570756973203a200a2020696e666f3420203d206c65207369746520696e7465726e6574206465206c276175746575722028687474703a2f2f7777772e657269636d61656b65722e66722f290a2020696e666f3520203d206f7520737572206c6520736974652064274164756c6c616374202868747470733a2f2f6164756c6c6163742e6e65742f70726f6a656374732f6d6564696e7475782f292e0a20206c6963656e636520203d20436543494c4c2076657273696f6e20320a20206d6564696e747578203d2076322e303020657420737570c3a97269657572650a20206c616e6761676520203d20432b2b2f5174332f4d7953514c0a0a5b52656d65726369656d656e74735d0a20206c31203d20c38020526f6c616e6420536576696e20706f757220736f6e2061696465207072c3a96369657573652064616e73206c612070726f6772616d6d6174696f6e206574206c652064c3a962756767616765206465206c276170706c69636174696f6e2e0a20206c32203d20c3802047c3a9726172642044656c61666f6e6420706f757220736f6e20616964652061752064c3a9627567676167652c2073657320636f7272656374696f6e7320657420736573206170706f7274732e0a20206c33203d20c3802042656e6a616d696e20506f7175657420706f75722073657320696e666f726d6174696f6e7320636f6e6365726e616e74206c657320667569746573206dc3a96d6f697265732065742067657374696f6e20646520706f696e746575727320656e20432b2b2e0a0a);
