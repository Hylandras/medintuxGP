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

Echo Arrêt de l'Esclave du serveur WVP-VR : %date% %time% >>Out_JePars.txt
Echo chemin des scripts et fichers de log : 'C:\OpenDevMedintux\trunk\Programmes\Manager\bin\Nomadisme' 

rem Arrêt de l'Esclave local
mysql -h%AdresseN% -u%LoginN% -p%MPN% -P%PortN% <ScriptJePars.txt >>Out_JePars.txt

Echo off
set /a count = 1
:boucle
set /a count = count + 1
if %count%==2000 goto finboucle
goto boucle
:finboucle
Echo on

Echo .>>Out_JePars.txt
Echo .>>Out_JePars.txt
Echo Nouvel état de l'Esclave arrêté du serveur WVP-VR : %date% %time% >>Out_JePars.txt
mysql -h%AdresseN% -u%LoginN% -p%MPN% -P%PortN% <ScriptShowSlave.txt >>Out_JePars.txt

Echo ############################################################################>>Out_JePars.txt
Echo *>>Out_JePars.txt
Echo *>>Out_JePars.txt
Echo terminated >ScriptStatus.txt
