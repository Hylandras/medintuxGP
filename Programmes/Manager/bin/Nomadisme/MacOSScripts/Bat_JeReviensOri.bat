REM Echo off

set AdresseM=%1
set LoginM=%2
set MPM=%3
set PortM=%4

set AdresseN=%5
set LoginN=%6
set MPN=%7
set PortN=%8

set workPath=%9
cd %workPath%


Echo Retour du Nomade WVP-MAL sur le réseau local du serveur : %date% %time% >>Out_JeReviens.txt

rem Etat du Maître sur le Nomade (base locale)
Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Etat du Maître sur le Nomade WVP-MAL : %date% %time% >>Out_JeReviens.txt
mysql -h%AdresseN% -u%LoginN% -p%MPN% -P%PortN% <ScriptShowMaster.txt >>Out_JeReviens.txt


rem Insertion d'un drapeau à la fin du log binaire du Nomade
rem mysql -hlocalhost -uroot -P3306 <ScriptInsertFlagVR.txt >>Out_JeReviens.txt

rem Connexion au serveur pour démarrage de
rem l'Esclave pour réplication des opérations Nomades
Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Démarrage de l'Esclave du serveur pour récupérer les opérations du Nomade WVP-MAL : %date% %time% >>Out_JeReviens.txt
mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptJeReviens.txt >>Out_JeReviens.txt

set /a count = 0
:boucle
set /a count = count + 1

rem Test de l'arrivée du drapeau sur la base référentielle du seveur
rem mysql -hWXP-MAXDATA -uslave101 -paixact -P3306 <ScriptTestFlagVR.txt >>Out_JeReviens.txt

if %count%==10000 goto finboucle
goto boucle
:finboucle

Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Nouvel état de l'Esclave du serveur par rapport au Nomade WVP-MAL : %date% %time% >>Out_JeReviens.txt
rem Etat de l'Esclave du serveur après réplication
mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptShowSlave.txt >>Out_JeReviens.txt

rem Arrêt de l'Esclave du serveur
mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptStopSlave.txt >>Out_JeReviens.txt

set /a count = 0
:boucle2
set /a count = count + 1
if %count%==5000 goto finboucle2
goto boucle2
:finboucle2

Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Nouvel état de l'Esclave arrêté du serveur WXP-MAXDATA : %date% %time% >>Out_JeReviens.txt
Echo terminated >ScriptStatus.txt

rem Etat de l'esclave après arrêt
mysql -h%AdresseM% -u%LoginM% -p%MPM% -P%PortM% <ScriptShowSlave.txt >>Out_JeReviens.txt


rem Réinitialisation des logs du Nomade WVP-MAL
rem Mais avant la réinitialisation, sauvegarde des fichiers logs binaires

REM Echo .>>Out_JeReviens.txt
REM Echo .>>Out_JeReviens.txt
REM Echo Arret du serveur MySQL du nomade WVP-MAL : %date% %time% >>Out_JeReviens.txt
REM NET STOP MySQL >>Out_JeReviens.txt
REM Echo .>>Out_JeReviens.txt
REM Echo .>>Out_JeReviens.txt
REM Echo Sauvegarde des fichiers logs binaires du nomade WVP-MAL : %date% %time% >>Out_JeReviens.txt
"C:\Program Files\7-Zip\7z.exe" a -t7z  "D:\DataMedintux\SCMGP\SCMGP\LogBin\LogBinSave%date:~6,4%%date:~3,2%%date:~0,2%%time:~0,2%%time:~3,2%%time:~6,2%" "D:\DataMedintux\SCMGP\SCMGP\LogBin\LogBin*.*" >>Out_JeReviens.txt
REM Echo .>>Out_JeReviens.txt
REM Echo .>>Out_JeReviens.txt
REM Echo Redémarrage du serveur MySQL du nomade WVP-MAL : %date% %time% >>Out_JeReviens.txt
REM NET START MySQL >>Out_JeReviens.txt

Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Connexion à la base locale du Nomade WVP-MAL pour réinit les logs de WVP-MAL : %date% %time% >>Out_JeReviens.txt
Echo Etat Avant / Après >>Out_JeReviens.txt
mysql -h%AdresseN% -u%LoginN% -p%MPN% -P%PortN% <ScriptResetMaster.txt >>Out_JeReviens.txt

Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Connexion de la base du Nomade WVP-MAL au serveur pour relancer la réplication Serveur->Nomade : %date% %time% >>Out_JeReviens.txt
Echo Etat Avant / Après >>Out_JeReviens.txt
rem Il faut maintenant reconnecter l'Esclave du Nomade au Serveur
mysql -h%AdresseN% -u%LoginN% -p%MPN% -P%PortN% <ScriptStartSlave.txt >>Out_JeReviens.txt

set /a count = 0
:boucle3
set /a count = count + 1
if %count%==5000 goto finboucle3
goto boucle3
:finboucle3

Echo .>>Out_JeReviens.txt
Echo .>>Out_JeReviens.txt
Echo Nouvel état de l'Esclave du Nomade WVP-MAL par rapport au serveur : %date% %time% >>Out_JeReviens.txt
rem Etat de l'Esclave du nomade
mysql -h%AdresseN% -u%LoginN% -p%MPN% -P%PortN% <ScriptShowSlave.txt >>Out_JeReviens.txt


Echo ############################################################################>>Out_JeReviens.txt
Echo *>>Out_JeReviens.txt
Echo *>>Out_JeReviens.txt
Echo on