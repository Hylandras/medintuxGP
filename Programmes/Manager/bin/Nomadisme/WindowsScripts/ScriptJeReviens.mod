# Etat de l'esclave de la base locale
SHOW SLAVE STATUS \G

# Par sécurité forçage de l'arrêt de l'Esclave
# avant de le redémarrer

STOP SLAVE;


# Positionnement des paramètres de l'Esclave
# par rapport à son Maître principal
CHANGE MASTER TO MASTER_HOST='{{reverse host}}',
MASTER_PORT={{reverse port}},
MASTER_USER='{{reverse user}}',
MASTER_PASSWORD='{{reverse pass}}',
MASTER_LOG_FILE='{{reverse logbin}}',
MASTER_LOG_POS=98;

# Démarrage de l'Esclave locale pour qu'il
# se connecte à son Maître de réplication
# et y récupère les logs binaires pour les
# éxécuter sur la base locale

START SLAVE;
