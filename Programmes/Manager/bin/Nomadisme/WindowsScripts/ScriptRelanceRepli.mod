# Etat de l'esclave de la base locale
SHOW SLAVE STATUS \G

# Par sécurité forçage de l'arrêt de l'Esclave
# avant de le redémarrer

STOP SLAVE;

# Positionnement des paramètres de l'Esclave
# par rapport à son Maître principal
CHANGE MASTER TO MASTER_HOST='{{Master_Host}}',
MASTER_PORT='{{Master_Port}}',
MASTER_USER='{{Master_User}}',
MASTER_PASSWORD='{{MasterKey}}',
MASTER_LOG_FILE='{{Master_Log_File}}',
MASTER_LOG_POS='{{Read_Master_Log_Pos}}';

# Démarrage de l'Esclave locale pour qu'il
# se connecte à son Maître de réplication
# et y récupère les logs binaires pour les
# éxécuter sur la base locale

START SLAVE;
