# Connection: Test
# Host: localhost
# Saved: 2005-03-15 08:06:48
# 
# Host: localhost
# Database: QLaboFTP
# Table: 'min_max'
# 
CREATE TABLE `min_max` (
  `sexe` char(2) NOT NULL default '',
  `test_name` varchar(32) NOT NULL default '',
  `val_min` varchar(10) default '0',
  `val_max` varchar(10) default '0'
) ENGINE=MyISAM CHARSET=latin1; 

