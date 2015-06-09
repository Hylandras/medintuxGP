ALTER TABLE `DrTuxTest`.`Personnes` ADD COLUMN `NumRPPS` VARCHAR(10) AFTER `NumOrdre`;
ALTER TABLE `DrTuxTest`.`Personnes` ADD COLUMN `ClefRPPS` VARCHAR(40) AFTER `NumOrdre`;
INSERT INTO `DrTuxTest`.`version`(`NumVers`)VALUES ('2.16.000');
ALTER TABLE `DrTuxTest`.`fchPat` DROP COLUMN `FchPat_NomMedTT`;
ALTER TABLE `DrTuxTest`.`fchPat` DROP COLUMN `FchPat_PrenomMedTT`;
ALTER TABLE `DrTuxTest`.`fchPat` ADD COLUMN `FchPat_LoginPatient` varchar(32) AFTER `FchPat_Titre`;
ALTER TABLE `DrTuxTest`.`fchPat` ADD COLUMN `FchPat_PasswPatient` varchar(60) AFTER `FchPat_Titre`;
