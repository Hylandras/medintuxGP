#include "MyFtp.h"
#include "Global.h"
#include "CFtpInfo.h"
#include <qmessagebox.h>
#include <qdatetime.h>
#include <qnamespace.h>
#include <qdir.h>

//***************************** CONSTRUCTEUR ET DESTRUCTEUR *************************************

MyFtp::MyFtp(QObject * parent,
	     const char * name /* = 0 */,
	     bool log /* = FALSE */,
	     bool remove/* = FALSE */)
    : QObject(parent,name)
{
    // ouverture du fichier log
    m_LogFile = 0;
    m_removeDistantFile = remove;
    if (log) {
	Log(TRUE);
    } else {
	Log(FALSE);
    }
    
    // Création de l'objet FTP
    m_FtpTools = new CFtpTools(this);
    
    // Création des timers pour le scan
    m_ScanTimer     = new QTimer(this);
    m_DownloadTimer = new QTimer(this);
    
    // si la création du FTP à réussi alors on se connecte au serveur
    if (m_FtpTools) {
	// *** connection des signaux et slots du FTP ***
	// permet de connaitre l'état d'avancement de la connection
	connect( m_FtpTools, SIGNAL(stateChanged(int)), this, SLOT(stateFTP_changed(int)) );
	// permet de connaitre le lancement d'une nouvelle commande
	connect( m_FtpTools, SIGNAL( commandStarted(int) ) , this , SLOT( Started(int) ) );
	// permet de connaitre la fin d'une commande
	connect( m_FtpTools, SIGNAL(commandFinished(int,bool)) , this , SLOT(Finished(int,bool)));
	// *** LECTURE DU FICHIER INI ***
	QString adress , port , user , pwd , folder;
	int i = 0;
	if (Global_pCGestIni->Param_ReadParam("ConnexionFTP", "Serveur", &adress, &port)==0) {
	    i++;
	}
	if (Global_pCGestIni->Param_ReadParam("ConnexionFTP", "Utilisateur", &user, &pwd)==0) {
	    i++;
	}
	if (Global_pCGestIni->Param_ReadParam("ConnexionFTP", "Repertoire", &folder)==0) {
	    i++;
	}
	// *** CONNECTION ***
	if (i == 3) {
	    m_FtpTools->ConnectFTP(adress , port.toUInt() , user , pwd , folder);
	}
    }	
}

MyFtp::~MyFtp()
{
    // si on utilise le fichier log
    if (m_Log) {
	// on le ferme
	m_LogFile->close();
	// on l'efface de la mémoire
	delete m_LogFile;
	m_LogFile = 0;
    }
}

//----------------------------------------- Log() -----------------------------------------------
void MyFtp::Log(bool log /*= FALSE*/)
{
    // enregistre si le fichier journal doit être utilisé
    if (log) { // si oui
	if (!m_LogFile) { // et qu'il n'est pas déja ouvert
	    // création du fichier log avec la date et l'heure dans le nom du fichier
	    QString datejournal = QDateTime::currentDateTime().toString("-yyyyMMdd-hmmss");
	    m_LogFile = new QFile( GlobalPathAppli +  LOG_FOLDER + QDir::separator() + "journal" + datejournal + ".log" );
	    // si on ne peut pas l'ouvrir en écriture
	    if ( !m_LogFile->open(IO_WriteOnly)) {
		// on l'efface de la mémoire
		delete m_LogFile;
		m_LogFile = 0;
		// on stocke FAUX
		m_Log = FALSE;
		return;
	    }
	    // si on peut l'ouvrir en écriture on stocke VRAI
	    m_Log = log;
	    return;
	}
    } else { // si on n'utilise pas
	if (m_LogFile) { // si on l'utilisait
	    // on le ferme
	    m_LogFile->close();
	    // on l'efface de la mémoire
	    delete m_LogFile;
	    m_LogFile = 0;
	}
	// on stocke FAUX
	m_Log = log;
    }
    
    return;
}

//------------------------------------ removeDistantFile() --------------------------------------
void MyFtp::removeDistantFile(bool remove /*= FALSE*/)
{
    // on stocke le désir de vouloir effacer le fichier distant une fois téléchargé en local
    m_removeDistantFile = remove;
}

//---------------------------------------- toLog() ----------------------------------------------
void MyFtp::toLog(/*const QString &*/ QString stringlog)
{
    if (m_Log) { // si on utilise le fichier log (alors il est ouvert)
	if (stringlog != QString::null) { // si la chaine n'est pas vide
	    // on se positionne en fin de fichier
	    m_LogFile->atEnd();
	    // on ecrit la donnée
	    m_LogFile->writeBlock(stringlog,stringlog.length());
	    // on rajoute un retour à la ligne
	    m_LogFile->writeBlock("\r\n",2);
	    // on force la mise à jour du fichier log sur le disque
	    m_LogFile->flush();
	}
    }
    return;
}

//----------------------------------- disconnectFTP() -------------------------------------------
void MyFtp::disconnectFTP()
{
    m_FtpTools->close();
}

//-------------------------------------- LetScanFTP() -------------------------------------------
void MyFtp::LetScanFTP()
{
    // envoi la commande de listage du répertoire
    emit ftpStateChanged("Transfert en cours");
    m_FtpTools->GetListFTP();
    return;
}

//------------------------------------ stateFTP_changed() ---------------------------------------
void MyFtp::stateFTP_changed(int etat)
{
    switch ((CFtp::State)etat) {
    case CFtp::Unconnected:
	// gestion de la couleur de la led
	emit ftpStateChanged("Deconnecté");
	break;
    case CFtp::HostLookup:
	emit ftpStateChanged("Recherche de l'hote");
	break;
    case CFtp::Connecting:
	emit ftpStateChanged("Connexion en cours");
	break;
    case CFtp::Connected:
	// gestion de la couleur de la led
	emit ftpStateChanged("Connecté");
	break;
    case CFtp::LoggedIn: // l'utilisateur est authentifié
	emit ftpStateChanged("Utilisateur connecté");
	// connection des signaux et slots du FTP
	connect( m_FtpTools, SIGNAL(listInfo(const QUrlInfo &)) , this , SLOT(ListChanged(const QUrlInfo &)));
	// connection des signaux et slots du timer
	connect( m_ScanTimer, SIGNAL(timeout()), this , SLOT(LetScanFTP()));
	connect( m_DownloadTimer, SIGNAL(timeout()), this , SLOT(checkDownload()));
	// demarrage des timers
	m_ScanTimer->start(120000,FALSE); // 2 minutes soit 120000 millisecondes
	m_DownloadTimer->start(500,FALSE); // 0.5 seconde soit 500 millisecondes
	break;
    case CFtp::Closing:
	emit ftpStateChanged("Fermeture connexion");
	break;
    }
    return;
}

//---------------------------------------- Finished() -------------------------------------------
void MyFtp::Finished(int id,bool error)
{
    // A REORGANISER
    // en effet il est plus judicieux de tester si une erreur c'est produite
    // d'enrichir le journal
    // de fermer le fichier local
    // si aucune erreur alors on efface le fichier distant
    
    // recuperation de la commande
    COMMAND *command = m_CommandMap.find(id).data();
    
    // si c'est un téléchargement de fichier vers le client
    if (command->m_Command == "Téléchargement vers Client") {
	
	// Création d'un pointeur sur le fichier qui a été téléchargé
	QFile *p_File = m_FileMap.find(id).data();
	
	// si le fichier existe
	if (p_File) {
	    // on récupère son nom pour le fichier journal
	    command->m_nomFichier = p_File->name();
	    // on ferme le fichier local
	    p_File->close();
	    // on le retrouve dans la QValueList FileToDownload et on le transfert dans FileToAnalyse
	    CFtpInfo info = retrieveFileToDownloadWithIndex(id);
	    m_FileToAnalyse.append(info);
	    // on l'efface de FileToDownload
	    m_FileToDownload.remove(info);
	    // on emet le signal qui passe le nom du fichier pour la procédure d'analyse
	    emit newFileDownloaded(p_File->name());
	    
	    // on l'efface de la mémoire
	    delete p_File;
	    // on supprime sa référence du QMap
	    m_FileMap.erase(id);
	    
	}    
    }
    
    if (command->m_Command == "Effacement d'un fichier") {
	// on retrouve les references du fichier par son index
	CFtpInfo info = retrieveFileToRemoveWithIndex(id);
	// on l'efface de la QMap m_RemoveMap et de la QValueList m_FileToRemove
	m_FileMap.erase(info.serial());
	m_FileToRemove.remove(info);
    }
    
    // si c'est un changement de répertoire on verifie si Liste au format Unix
    // si oui on passe la commande de changement de format et on diffère le premier scan
    // si non on fait le premier scan
    if (command->m_Command == "Changement de répertoire") {
	QString format_liste_unix;
	if (Global_pCGestIni->Param_ReadParam("ConnexionFTP", "ListFormatUnix", &format_liste_unix)==0) {
	    if (format_liste_unix.lower() == "oui") {
		m_FtpTools->rawCommand("SITE LISTFMT 1");
	    } else {
		LetScanFTP();
	    }
	}
    }
    
    // si la commande est personnalisée (pour l'instant changement de format de liste)
    // on fait le premier scan
    if (command->m_Command == "Commande personnalisée") {
	LetScanFTP();
    }
    
    // Puis pour toutes les commandes
    // on complete les renseignements pour le journal puis ecriture dans le journal
    command->m_dateFin = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    // si une erreur c'est porduite
    if (error) {
	// on l'a récupère
	command->m_Resultat = m_FtpTools->errorString();
    } else {
	// si on récupère la réponse de la fin de la commande
	command->m_Resultat = errorToString(m_FtpTools->error());
    }
    // on écrit dans le fichier journal
    toLog("Fin   " + QString::number(id) + " : " + command->m_dateFin + " : " + command->m_Resultat + " : " + command->m_nomFichier);
    // on efface la référence de la commande dans le QMap
    m_CommandMap.erase(id);
    delete command;
    // gestion de la couleur de la led
    if (m_FtpTools->state() == CFtp::LoggedIn) {
	emit ftpStateChanged("Utilisateur connecté");
    }
    return;
}

//---------------------------------------- Started() --------------------------------------------
void MyFtp::Started(int id)
{
    // on créé un pointeur sur la commande
    COMMAND *command = new COMMAND;
    // on récupère le libellé de la commande en cours
    command->m_Command = commandToString(m_FtpTools->currentCommand());
    // on récupère la date de debut
    command->m_dateDebut = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    // on écrit la commande dans le fichier log
    QString complementCommand = "";
    QString newName = "";
    QString ancienName = "";
    if (command->m_Command == "Téléchargement vers Client") {
	
	CFtpInfo info = retrieveFileToDownloadWithIndex(id);
	newName    = info.newName();
	ancienName = info.name();
	complementCommand  = " de " + ancienName + " renommé en " + newName;
	emit ftpStateChanged("Transfert en cours");
	
    } else if (command->m_Command == "Effacement d'un fichier") {
	
	CFtpInfo info = retrieveFileToRemoveWithIndex(id);
	ancienName = info.name();
	complementCommand  = " nommé " + ancienName;
	emit ftpStateChanged("Transfert en cours");
	
    }
    toLog("Debut " + QString::number(id) + " : " + command->m_dateDebut + " : " + command->m_Command + complementCommand);
    
    // on insère sa référence dans le QMap
    m_CommandMap.insert(id,command);
    
    return;
}

//------------------------------------- commandToString() ---------------------------------------
QString MyFtp::commandToString(CFtp::Command command)
{
    QString valeur = "";
    switch(command) {
    case CFtp::None:
	valeur = "Aucune";
	break;
    case CFtp::ConnectToHost:
	valeur = "Ouverture Connection";
	break;
    case CFtp::Login:
	valeur = "Authentification";
	break;
    case CFtp::Close:
	valeur = "Fermeture Connection";
	break;
    case CFtp::List:
	valeur = "Listage";
	break;
    case CFtp::Cd:
	valeur = "Changement de répertoire";
	break;
    case CFtp::Get:
	valeur = "Téléchargement vers Client";
	break;
    case CFtp::Put:
	valeur = "Téléchargement vers Serveur";
	break;
    case CFtp::Remove:
	valeur = "Effacement d'un fichier";
	break;
    case CFtp::Mkdir:
	valeur = "Création d'un répertoire";
	break;
    case CFtp::Rmdir:
	valeur = "Effacement d'un répertoire";
	break;
    case CFtp::Rename:
	valeur = "Renommage d'un fichier";
	break;
    case CFtp::RawCommand:
	valeur = "Commande personnalisée";
	break;
    }
    return valeur;
}

//-------------------------------------- errorToString() ----------------------------------------
QString MyFtp::errorToString(CFtp::Error error)
{
    QString valeur = "";
    switch(error) {
    case CFtp::NoError:
	valeur = "Aucune erreur";
	break;
    case CFtp::HostNotFound:
	valeur = "Serveur non trouvé";
	break;
    case CFtp::ConnectionRefused:
	valeur = "Connection refusée";
	break;
    case CFtp::NotConnected:
	valeur = "Non connectée";
	break;
    case CFtp::UnknownError:
	valeur = "Erreur inconnue";
	break;
    }
    return valeur;
}

//--------------------------------------- ListChanged() ------------------------------------------
void MyFtp::ListChanged(const QUrlInfo &objet)
{
    CFtpInfo info = objet;
    int trouve;
    QString espace = "";
    QString taille = QString::number(objet.size());
    QString date = objet.lastModified().toString("dd/MM/yyyy hh:mm");
    // si c'est un fichier
    if (objet.isFile()) {
	toLog("Fichier    : " + objet.name() + espace.fill(' ',6 - taille.length()) + taille + " Octets, du " + date);
	// on verifie qu'il n'est pas déjà dans la file d'attente des fichiers listés ou a télécharger ou a analyser
	// si oui cassos
	if ( (trouve = retrieveAncienNameFileList(objet.name())) != -1) return;
	if ( (trouve = retrieveAncienNameFileToDownload(objet.name())) != -1) return;
	if ( (trouve = retrieveAncienNameFileToAnalyse(objet.name())) != -1) return;
	// sinon on l'insère à la fin de la QValueList des fichiers listés
	m_FileList.append(info);
    }
}

//---------------------------- retrieveAncienNameFileToDownload() --------------------------------
int MyFtp::retrieveAncienNameFileToDownload(const QString &ancien_name)
{
    int result = -1;
    FileInfo::Iterator it;
    
    for ( it = m_FileToDownload.begin() ; it != m_FileToDownload.end() ; ++it) {
	if ( (*it).name() == ancien_name) {
	    result = (*it).serial();
	    break;
	}
    }
    
    return result;
}

//------------------------------- retrieveAncienNameFileList() -----------------------------------
int MyFtp::retrieveAncienNameFileList(const QString &ancien_name)
{
    int result = -1;
    FileInfo::Iterator it;
    
    for ( it = m_FileList.begin() ; it != m_FileList.end() ; ++it) {
	if ( (*it).name() == ancien_name) {
	    result = (*it).serial();
	    break;
	}
    }
    
    return result;
}

//----------------------------- retrieveFileToRemoveWithIndex() ----------------------------------
CFtpInfo MyFtp::retrieveFileToRemoveWithIndex(int index)
{
    CFtpInfo result;
    FileInfo::Iterator it;
    
    for ( it = m_FileToRemove.begin() ; it != m_FileToRemove.end() ; ++it) {
	if ( (*it).serial() == index) {
	    result = (*it);
	    break;
	}
    }
    
    return result;
}
	
//----------------------------- retrieveFileToDownloadWithIndex() ---------------------------------
CFtpInfo MyFtp::retrieveFileToDownloadWithIndex(int index)
{
    CFtpInfo result;
    FileInfo::Iterator it;
    
    for ( it = m_FileToDownload.begin() ; it != m_FileToDownload.end() ; ++it) {
	if ( (*it).serial() == index) {
	    result = (*it);
	    break;
	}
    }
    
    return result;
}

//---------------------------- retrieveAncienNameFileToAnalyse() --------------------------------
int MyFtp::retrieveAncienNameFileToAnalyse(const QString &ancien_name)
{
    int result = -1;
    FileInfo::Iterator it;
    
    for ( it = m_FileToAnalyse.begin() ; it != m_FileToAnalyse.end() ; ++it) {
	if ( (*it).name() == ancien_name) {
	    result = (*it).serial();
	    break;
	}
    }
    
    return result;
}

//------------------------------------- checkDownload() ------------------------------------------
void MyFtp::checkDownload()
{
    // si la liste est vide cassos
    if (m_FileList.isEmpty()) return;
    // sinon on télécharge le premier
    CFtpInfo element = m_FileList.first();
    // on change l'element de liste
    m_FileToDownload.append(element);
    m_FileList.remove(element);
    // on l'affiche dans le Log pour *** TEST ***
    toLog("checkDownload   " + element.name() + " : " + QString::number(element.size()) + " Octets");
    DownloadFile(element);
}

//-------------------------------------- DownloadFile() ------------------------------------------
void MyFtp::DownloadFile(CFtpInfo info)
{
    // on recherche info dans la QValueList
    FileInfo::Iterator it = m_FileToDownload.find(info);
    // on génère un nom de fichier avec la référence du répertoire local de téléchargement
    QDateTime time = QDateTime::currentDateTime(Qt::LocalTime);
    QString NewName = GlobalPathAppli +  TEMP_FOLDER + QDir::separator() + "labo-" + time.toString("yyyyMMddhhmmsszzz") + ".hpr";
    // on créé un pointeur sur le fichier
    QFile *p_DownloadFile = new QFile( NewName );
    // si le fichier existe déja
    if ( p_DownloadFile->exists()) {
	// on efface le pointeur
	delete p_DownloadFile;
	// cassos sans télécharger
	return;
    }
    // si on ne peut pas ouvrir le fichier en écriture
    if ( !p_DownloadFile->open( IO_WriteOnly ) ) {
	QMessageBox::critical( 0, tr("Erreur de Téléchargement"),tr("Impossible de créer le fichier '%1'.").arg(info.name()) );	
	// on efface le pointeur
	delete p_DownloadFile;
	// cassos sans télécharger
	return;
    }
    // on télécharge et on récupère le numéro de la tache correspondante
    int tache_number = m_FtpTools->get( info.name(), p_DownloadFile );
    // on stocke le pointeur sur le fichier dans le QMap
    m_FileMap.insert(tache_number,p_DownloadFile);
    // on remet à jour les informations dans la QValueList
    (*it).setSerial(tache_number);
    (*it).setNewName(NewName);
    
}

//---------------------------------- signalFileDownloaded() --------------------------------------
// Fonction créée pour tester le programme
// elle est appelée depuis FeuillePrincipale pour emmettre le nom d'un fichier choisi dans un 
// QFileDialog
void MyFtp::signalFileDownloaded( QString filename )
{
    emit newFileDownloaded( filename );
}

//------------------------------------ removeFile() ----------------------------------------
// Fonction qui envoi la demande d'effacement d'un fichier du serveur distant
void MyFtp::removeFile( QString newName )
{
    // on recherche les references du fichier dans FileToAnalyse
    CFtpInfo info = retrieveFileToAnalyseWithNewName(newName);
    QString ancienName = info.name();
    int tache_number = info.serial();
    // si on doit, on efface le fichier distant du serveur en stockant son nom dans RemoveMap
    if (m_removeDistantFile) {
	tache_number = m_FtpTools->remove( ancienName );
	m_RemoveMap.insert(tache_number,ancienName);
    }
    // puis on transfert les références de FileToAnalyse vers FileToRemove en transformant son id
    m_FileToAnalyse.remove(info);
    info.setSerial(tache_number);
    m_FileToRemove.append(info);
}

//------------------------------------ removeFile() ----------------------------------------
// Fonction qui traite le cas des fichiers à zéro octets
void MyFtp::uselessFile( QString newName )
{
    // on recherche les references du fichier dans FileToAnalyse
    CFtpInfo info = retrieveFileToAnalyseWithNewName(newName);
    // on ecrit dans le log que le fichier est à zéro
    toLog("Erreur " + QString::number(info.serial()) + " : Fichier de taille 0 : " + info.name() + " -> " + info.newName());
    // on l'efface des fichiers à analyser
    m_FileToAnalyse.remove(info);
    // puis des fichiers locaux
}

//---------------------------- retrieveFileToAnalyseWithNewName() --------------------------------
CFtpInfo MyFtp::retrieveFileToAnalyseWithNewName(const QString &new_name)
{
    CFtpInfo result;
    FileInfo::Iterator it;
    
    for ( it = m_FileToAnalyse.begin() ; it != m_FileToAnalyse.end() ; ++it) {
	if ( (*it).newName() == new_name) {
	    result = (*it);
	    break;
	}
    }
    
    return result;
}

//---------------------------------- listFormatUnix() --------------------------------------
// Fonction qui active ou desactive le retour de la commande LIST au format Unix
void MyFtp::listFormatUnix( const QString &state)
{
    if (state == "ON") {
	m_FtpTools->rawCommand("SITE LISTFMT 1");
    }
    
    if (state == "OFF") {
	m_FtpTools->rawCommand("SITE LISTFMT 0");
    }
}

/*
    // enregistrement dans un fichier log pour debug du buffer de retour de la commande LIST FTP
    QFile *m_LogFile = new QFile( "F:/Programmation/MesProgrammes/LaboFTP/Log/parseDir.log" );
    m_LogFile->open(IO_WriteOnly | IO_Append);
    m_LogFile->atEnd();
    m_LogFile->writeBlock(buffer,buffer.length());
    m_LogFile->writeBlock("\r\n",2);
    m_LogFile->flush();
    m_LogFile->close();
    delete m_LogFile;
    
*/

