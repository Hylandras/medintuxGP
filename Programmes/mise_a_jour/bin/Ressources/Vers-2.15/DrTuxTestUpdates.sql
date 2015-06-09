ALTER TABLE `DrTuxTest`.`Personnes` ADD COLUMN `Couleur` VARCHAR(10) AFTER `NumOrdre`;
ALTER TABLE `DrTuxTest`.`Personnes` MODIFY COLUMN `Droits` VARCHAR(600);
ALTER TABLE `DrTuxTest`.`agenda` ADD COLUMN `Adresse` VARCHAR(256) default '' AFTER `status`;
INSERT INTO `DrTuxTest`.`version`(`NumVers`)VALUES ('2.15.000');