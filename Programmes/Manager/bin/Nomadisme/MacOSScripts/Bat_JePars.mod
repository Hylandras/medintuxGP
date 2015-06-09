REM Echo off

set workPath=%1
cd %workPath%

dir

set AdresseM=%2
set LoginM=%3
set MPM=%4
set PortM=%5

set AdresseN=%6
set LoginN=%7
set MPN=%8
set PortN=%9
set LogPath={{param 0}}
REM set toSee={{param 1}}

Echo Arret de la connexion au serveur maitre et passage en mode nomade : %date% %time% >>Out_JePars.txt
Echo -01- chemin des scripts : '%workPath%' >>Out_JePars.txt
Echo %workPath%\terminated>ScriptStatus.txt
Echo -10- chemin des log.    : '%LogPath%'  >>Out_JePars.txt
REM Echo -11- toSee    : '%toSee%'  >>Out_JePars.txt

Echo on
exit