# Host: localhost
# Database: QLaboFTPTest
# Table: 'examens'
# 
CREATE TABLE `examens` (
  `ref_prim_key_Patient` bigint(20) default '0',
  `hprim_line` varchar(224) default '',
  `prim_key` bigint(20) NOT NULL auto_increment,
  PRIMARY KEY  (`prim_key`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1; 

