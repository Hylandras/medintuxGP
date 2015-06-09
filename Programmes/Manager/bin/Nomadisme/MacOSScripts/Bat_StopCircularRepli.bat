Echo off

set workPath=%1

set AdresseM=%2
set LoginM=%3
set MPM=%4
set PortM=%5

set AdresseN=%6
set LoginN=%7
set MPN=%8
set PortN=%9

set LogPath=C:\OpenDevMedintux\trunk\Programmes\Manager\bin\Manager\bin\Nomadisme\FilesLogs
set NomMySQL=test du deuxieme parametre
set NomLogBin={{param 2}}

cd %workPath%
Echo .
Echo .
Echo Demarrage de la procedure de Synchro
Echo du Nomade : %AdresseN% - %PortN% vers Réseau cabinet
Echo ...

Echo Synchro du Nomade %AdresseN% - %PortN% sur le Réseau Cabinet  : %date% %time% >>Out_JeReviens.txt

rem Arrêt de l'Esclave du Serveur
rem rem  mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptStopSlave.txt >>Out_JeReviens.txt

rem Etat de l'Esclave du Serveur du Réseau
Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Etat de l'Escalve du Serveur du Réseau %AdresseM% - %PortM% : %date% %time% >>Out_JeReviens.txt
rem rem mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptShowSlave.txt >>Out_JeReviens.txt

rem rem mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptShowSlave.txt >>Out_PosRepliLoop.txt

Echo terminated >ScriptStatus.txt

Echo Fin de la procedure d'arret de la Réplication circulaire : %AdresseN% - %PortN%
Echo ###########################
Echo ###########################

Echo ############################################################################>>Out_JeReviens.txt
Echo *>>Out_JeReviens.txt
Echo *>>Out_JeReviens.txt
Echo on

EXIT
