/********************************* C_AppCore.cpp **********************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: roland-sevin@medintux.org                                            *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/
#include "C_AppCore.h"
#include <QFileInfo>
#include <QFile>

#include <QStringList>
#include <QTextStream>
#include <QIODevice>
#include <QProcess>
#include <QDateTime>
#include <QTextCodec>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkInterface>

#include "Theme/Theme.h"
#include "CGestIni.h"
//================================================= C_SplashScreen> ==============================================================
/*! \class C_SplashScreen
 *  \brief base class derived from QSplashScreen.
 *  - this Class display a splash screen
 */

void	C_SplashScreen::mousePressEvent ( QMouseEvent *e )
{   int ret = 0;
    emit Sign_OnClicked(e, ret);
    if (ret) QSplashScreen::mousePressEvent ( e );
}
bool C_SplashScreen::event ( QEvent * e )
{    int ret = 0;
     if (e->type()==QEvent::MouseButtonPress) // marche po
        {emit Sign_OnClicked( (QMouseEvent*)e, ret);
        }
     return QSplashScreen::event ( e );
}

//================================================= CApp ==============================================================
/*! \class C_AppCore
 *  \brief Main base class derived from QApplication.
 *  - this Class must be derived and derived classe instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    if fatal error in class initialisation occured  CApp::pCApp()==0
 *    so all publics functions will be accessibles by CApp::pCApp()->publicFonctions().
 */

static C_AppCore*           G_AppCore    = 0;  // contiendra l'instance globale de l'application accessible avec CApp::pCApp()
static QString              G_currentIp  = "";
static QString              G_currentMac = "";
//--------------------------------------------- C_AppCore -------------------------------------------------------------------
C_AppCore::~C_AppCore()
{writeParam((m_NameAppli+" session").toLatin1(),"closed", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz").toLatin1());
 updateIniParamToDisk();
}

//--------------------------------------------- C_AppCore -------------------------------------------------------------------
C_AppCore::C_AppCore(QString mui_name, int & argc, char ** argv)
    : QtSingleApplication(argc, argv, true) , m_NameAppli(mui_name)
{   // QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    //.............................. recuperer le numero de version du .pro ..........................................
    //                               le mettre dans un tableau static tague pour
    //                               qu'il soit reperable dans le binaire
    static char NUMTAB_VERSION[]     = "==##@@=="NUM_VERSION"==@@##==";    // defini dans le .pro
    Q_UNUSED(NUMTAB_VERSION);
    //................ initialisation de quelques reperes .........................
    setApplicationName    ( m_NameAppli );         //qDebug (QString("NomAppli : %1").arg(m_NameAppli).toLatin1());
    setApplicationVersion ( NUM_VERSION );         //qDebug (QString("NumVersion : %1").arg(NUM_VERSION).toLatin1());
    setOrganizationDomain ( NAME_ORGANISATION );   //qDebug (QString("NomOrganisation : %1").arg(NAME_ORGANISATION).toLatin1());
    setOrganizationName   ( NAME_DOMAINE );        //qDebug (QString("NomDomaine : %1").arg(NAME_DOMAINE).toLatin1());

    m_Version            = applicationVersion ();

    //.................... recuperer path de demarrage de l'appli ...........................
    QFileInfo qfi(argv[0]);
    m_PathAppli          = CGestIni::Construct_PathBin_Module(m_NameAppli, qfi.path ());
    //................... fabriquer le .ini ..................................................
    m_PathIni            = localizePathIni();
    QString  dateSession = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz");
    if (!QFile::exists(m_PathIni))
       {m_IniParam = "["+ m_NameAppli +" session]"
                        "\n  started = " + dateSession;
       }
    else
       {m_IniParam    = CGestIni::Param_UpdateFromDisk(m_PathIni);
        CGestIni::Param_WriteParam(&m_IniParam, (m_NameAppli+" session").toLatin1(),"started", dateSession.toLatin1());
       }
    updateIniParamToDisk();
    m_UniqueInstance     = (readUniqueParam( m_NameAppli+" session", QString("Instance")).toLower()[0]=='u');
    //.............................. initialiser le theme ..........................................
    m_PathTheme     =  "../../Themes/Default/";
    if (CGestIni::Param_ReadParam( m_IniParam.toLatin1(), "Theme", "Path", &m_PathTheme) != QString::null )  // zero = pas d'erreur
        { m_PathTheme     =  "../../Themes/Default";                                   // valeur par defaut si pas de theme explicite
        }
    if ( QDir(m_PathTheme).isRelative()) {m_PathTheme.prepend(m_PathAppli); m_PathTheme = QDir::cleanPath(m_PathTheme) + "/";}
    if (!QDir(m_PathTheme).exists())      m_PathTheme     =  "../../Themes/Default/";        // valeur par defaut
    Theme::setPath(m_PathTheme);

    //qDebug (QString("NomAppli : %1").arg(applicationName()).toLatin1());
    //qDebug (QString("NumVersion : %1").arg(applicationVersion()).toLatin1());
    //qDebug (QString("NomOrganisation : %1").arg(organizationDomain()).toLatin1());
    //qDebug (QString("NomDomaine : %1").arg(organizationName()).toLatin1());
    G_AppCore           =    this;
}
//------------------------ decodedPassword ---------------------------------------
/*! \brief return password in decoded form from a string
 *  \param const QString pass : a string encoded (begin by '#')  or not encoded (don't begin by '#')
 *  \return QString decoded password
*/
QString C_AppCore::decodedPassword(QString pass)
{if (pass.startsWith('#')) return CGestIni::PassWordDecode(pass.mid(1));
 return    pass;
}

//------------------------ encodedPassword ---------------------------------------
/*! \brief return password in encoded form from a string
 *  \param const QString pass : a string encoded (begin by '#')  or not encoded (don't begin by '#')
 *  \return QString encoded password
*/
QString C_AppCore::encodedPassword(QString pass)
{if (pass.startsWith('#'))return pass;
 return QString("#")+CGestIni::PassWordEncode(pass);
}
//------------------------ EncodePassword_InConnectionParam ---------------------------------------
/*! \brief encode pasword in file initialisation form must bee : "Connexion","Parametres", driver, dataBase, user, password, hostname, port
 * file is updated on disk.
 *  \param const QString& section : section where is password
 *  \param const QString& key : key where is password exple : Parametres = driver, dataBase, user, password, hostname, port
 *  \return QString with error if so, or empty string if all is ok
*/
QString   C_AppCore::EncodePassword_InConnectionParam(const QString& section /* ="Connexion" */, const QString &key /* ="Parametres" */)
{   QString driver;             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
    QString dataBaseToConnect;  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
    QString user;               // = "root"
    QString password;           // = ""
    QString hostname;           // = "localhost"
    QString port;               // = "3306"
    QString error = readParam(  section.toLatin1(), key.toLatin1(), &driver, &dataBaseToConnect, &user, &password, &hostname, &port );
    if ( error.length() )  // zero = pas d'erreur
       { return QObject::tr("C_AppCore::EncodePassword_InConnectionParam() : error in parameters : CGestIni::Param_ReadParam() may be : key 'Parametres' omitted in section 'Connexion'\n%1").arg(error);
       }
    if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
    password = CGestIni::PassWordEncode(password);
    password.prepend('#');
    writeParam(section.toLatin1(), key.toLatin1(), driver.toLatin1(), dataBaseToConnect.toLatin1(), user.toLatin1(), password.toLatin1(), hostname.toLatin1(), port.toLatin1());
    updateIniParamToDisk();
    return QString::null;
}
//--------------------------------------------- ApplicationAndQtVersion -------------------------------------------------------------------
/*! \brief return the Application version number  and Qt Version
 *  \param const QString &version_suffix : additional sqrint to add to application version defined in .pro;
*/
QString C_AppCore::ApplicationAndQtVersion(const QString &version_suffix /*=""*/)
{if (version_suffix.length())
    {QString toDisplay = m_Version+" "+ version_suffix;
             toDisplay = toDisplay + tr("<br/>Version Qt : %1<br/>").arg(QT_VERSION_STR);
     return  toDisplay;
    }
 return m_Version;
}
//--------------------------------------------- initSettingVar -------------------------------------------------------------------
/*! \brief met a jour le fichier de parametrage la ou il veut.
*/
void C_AppCore::initSettingVar(const QString &var)
{m_IniParam = var;
}

//--------------------------------------------- updateIniParamToDisk -------------------------------------------------------------------
/*! \brief met a jour le fichier de parametrage sur le disque.
*/
void C_AppCore::updateIniParamToDisk()
{CGestIni::Param_UpdateToDisk(m_PathIni, m_IniParam);
}

//--------------------------------------------- getParamList -------------------------------------------------------------------
/*! \brief retourne une liste de parametres dans le .ini
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a la premiere valeur entre = et virgule)
 *  \param lst :        liste a initialiser et construire
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
*/
void C_AppCore::getParamList(const QString &sectionToRetrieve, const QString &varToRetrieve,  QStringList &lst , int isToStrip /*=0*/, int entireDataLine /*=0*/)
{ CGestIni::Param_GetList(m_IniParam, sectionToRetrieve, varToRetrieve,  lst , isToStrip , entireDataLine );
}
//--------------------------------------------- getParamList -------------------------------------------------------------------
/*! \brief retourne une liste de parametres dans le .ini
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a la premiere valeur entre = et virgule)
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
*/
QStringList C_AppCore::getParamList(const QString &sectionToRetrieve, const QString &varToRetrieve, int isToStrip /*=0*/, int entireDataLine /*=0*/)
{QStringList lst;
 CGestIni::Param_GetList(m_IniParam, sectionToRetrieve, varToRetrieve,  lst , isToStrip , entireDataLine );
 return lst;
}
//----------------------------------------- replaceList ---------------------------------------------
/*! \brief replace a variables list in section parameters by datas from QStringList
 *  \param outParam : parameters data document destination.
 *  \param QStringList &list list data sources to place.
 *  \param const QString &sectionToSet section where to find list of variables to replace
 *  \param const QString &varToSet  variable name of each list elements to replace
 */
void C_AppCore::replaceList(const QString &sectionToSet, const QString &varToSet,  const QStringList &list)
{
  CGestIni::replaceList(m_IniParam, sectionToSet, varToSet, list );
}

//--------------------------------------------- writeParam -------------------------------------------------------------------
/*! \brief ecrit une serie de valeurs dans la variable de configuration
*/
QString C_AppCore::writeParam(const char *section, const char  *variable,
                              const char *val1, const char *val2, const char *val3, const char *val4, const char *val5,
                              const char *val6, const char *val7, const char *val8, const char *val9, const char *val10)
{ return CGestIni::Param_WriteParam(&m_IniParam, section, variable, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
}

//--------------------------------------------- criptedToUnCripted -------------------------------------------------------------------
/*! \brief met en clair le terme presente dans cripted
*/
QString C_AppCore::criptedToUnCripted(const QString &cripted)
{   QString str = cripted;
    str = CGestIni::PassWordDecode(str);
    return  str;
}
//--------------------------------------------- unCriptedToCripted -------------------------------------------------------------------
/*! \brief met en code le terme presente dans uncripted
*/
QString C_AppCore::unCriptedToCripted(const QString &uncripted, const QString &prefix /* ="" */ )
{   QString str = uncripted;
    str = CGestIni::PassWordEncode(str);
    return str.prepend(prefix);
}
//--------------------------------------------- readLine -------------------------------------------------------------------
/*! \brief lit les donnees situees apres le signe egal d'une varaible donnee pour une section donnee.
*/
QString C_AppCore::readLine(const char *section, const char  *variable)
{
 return CGestIni::Param_ReadLine( m_IniParam.toLatin1(),section, variable);
}

//--------------------------------------------- readParam -------------------------------------------------------------------
/*! \brief lit une serie de valeurs dans la variable de configuration
*/

QString C_AppCore::readParam(const char *section, const char  *variable,
                             QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                             QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{ return CGestIni::Param_ReadParam(m_IniParam.toLatin1(), section, variable, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
}
//-----------------------------------------------------  Param_ReadUniqueParam --------------------------------
/*! \brief lit une valeur dans un fichier de configuration.
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernee
 *  \param pos : position de la valeur a retourner si plusieurs valeurs separees par des virgules
 *  \return QString::null la variable n'existe pas. sinon retourne la valeur
*/
QString C_AppCore::readUniqueParam(const QString &section, const QString &variable, int pos /* = -1 */)
{ return CGestIni::Param_ReadUniqueParam(m_IniParam.toLatin1(),section.toLatin1(), variable.toLatin1(), pos);
}

//-----------------------------------------------------  Param_ReadUniqueParam --------------------------------
/*! \brief lit une valeur dans un fichier de configuration.
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernee
 *  \param pos : position de la valeur a retourner si plusieurs valeurs separees par des virgules
 *  \return QString::null la variable n'existe pas. sinon retourne la valeur
*/
QString C_AppCore::readUniqueParam(const char *section, const char  *variable, int pos /* = -1 */)
{ return CGestIni::Param_ReadUniqueParam(m_IniParam.toLatin1(),section, variable, pos);
}

//--------------------------------------------- removeSectionParam -------------------------------------------------------------------
/*! \brief retire une section du fichier de configuration
*/
QString C_AppCore::removeSectionParam(QString section)
{return CGestIni::Param_RemoveSection(m_IniParam, section);
}
//--------------------------------------------- appCore -------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
*/
C_AppCore* C_AppCore::appCore()
{return G_AppCore;
}

//--------------------------------------------- instance -------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
*/
C_AppCore* C_AppCore::instance()
{return G_AppCore;
}

//--------------------------------------------- localizePathIni -------------------------------------------------------------------
/*! \brief cherche le fichier de configuration au niveau de : l'applicatif,$home/.Data Medical Design/MedinTux/applicatif.ini, la racine de MedinTux dans repertoire config, la racine de MedinTux.
 * NOTE :
         . Le premier fichier de configuration recherche est : au niveau de l'executable, nom_de_lexecutable.ini puis on y  recherche :

          Une mention d'indirection du type :
              [PathIni]
                  RedirectPath =,../../IndirectConfig.ini      ou IndirectConfig.ini est le fichier dont il sera tenu compte.

          Une mention d'indirection permettant au poste de travail executant le binaire de rechercher ses fichiers de configuration dans un repertoire lie a son adresse IP ou adresse MAC.
          Cette disposition est utile lorsque des postes de travail disk less lancent l'applicatif a partir d'un serveur applicatif distant type PXE ou autre.
          Cela permet a chaque poste de travail distant disk less qui ne disposent pas d'espace de stokage permanent, de disposer de leur propre fichier de configuration sur le serveur applicatif distant.
          Ces espaces sont definis au niveau du serveur applicatif par une syntaxe obeissant aux regles suivantes :

          ;...... cas d'une indirection selon adresse IP .....
          [PathIni]
            PathOnIP = ../../IndirectConfig/ , _ ,  poste_

          ;...... cas d'une indirection selon adresse MAC .....
          [PathIni]
            PathOnMacAdr = ../../IndirectConfig/ , _ , poste_

          - le premier parametre est le repertoire (peut etre indique en mode relatif) dans lequel retrouver le repertoire des fichiers de configuration d'un poste de travail lies a son adresse IP ou MAC.
          - le deuxieme parametre est un separateur, qui remplacera les points composant l'adresse IP du poste de travail ou les : l'adresse MAC
          - le troisieme est le prefixe du nom du repertoire des fichiers de configuration lies au poste de travail .

             Au total le nom du repertoire de configuration lies a un poste distant sera : Prefixe + adresse IP ou MAC du poste,
             Les deux derniers chiffres lors d'une indirection par repertoire IP peuvent etre de la forme 192.168.100.210-to-219 pour indiquer une fourchette de valeurs IP
             dont les points (IP) ou deux points (MAC) sont remplaces par le deuxieme parametre)
             specifiant le repertoire dans lequel l'on recherchera les fichiers de configuration qui lui sont lies

             ../../IndirectConfig/               repertoire global des fichiers de configuration distants
                                                 (le point de depart etant sur le poste serveur applicatif le repertoire du binaire execute)

                  poste_192_168_100_79           repertoire des fichiers de configuration d'un poste distant

                          Manager.ini            les differents fichiers de configuration
                          drtux.ini
                          biogest.ini

         . Si echec le fichier de configuration sera recherche en :  ../../../config/nom_de_lexecutable.ini
         . Si echec le fichier de configuration sera recherche en :  homePath/.organizationName/organizationDomain/nom_de_lexecutable.ini
         . Si echec le fichier de configuration sera recherche en :  ../../../config/MedinTuxGlobal.ini
         . Si echec le fichier de configuration sera recherche en :  homePath/.organizationName/organizationDomain/MedinTuxGlobal.ini
         . Si echec le fichier de configuration sera recherche en :  ../../../MedinTuxGlobal.ini


*/

QString C_AppCore::localizePathIni()
{QString dataIni;
 QFileInfo qfi( QCoreApplication::arguments().at(0) );
 QString sep       = "";
 QString prefix    = "";
 QString nameAppli = applicationName();
 QString pathAppli = CGestIni::Construct_PathBin_Module(nameAppli, qfi.path ());
 QString path      = pathAppli + nameAppli + ".ini";
 QString typIndir  = "";

 if (QFile::exists(path))
    {QString redirectPath     = "";
     dataIni                  = CGestIni::Param_UpdateFromDisk(path);
     redirectPath             = CGestIni::Param_ReadUniqueParam( dataIni.toLatin1(), "PathIni", "RedirectPath");
     //...... rechercher d'abord une indirection explicite .........
     if (redirectPath.length())
        { if ( QDir::isRelativePath (redirectPath) ) { redirectPath.prepend(pathAppli); redirectPath = QDir::cleanPath(redirectPath); }
          if ( QFile::exists(redirectPath) )           return redirectPath;    // ====> CASSOS avec le chemin
        }
     //...... rechercher ensuite une indirection selon Ip ou Hw .........
     // [PathIni]
     //    PathOnMacAdr = ../../IndirectConfig/MacAdr/
     //    PathOnIP     =  ../../IndirectConfig/IpAdr/
     QString hw         = "";
     QString ip         = get_Current_IP_Adr(&hw);   // recuperer adresse ip et mac
     QStringList lst    = CGestIni::Param_GetList(dataIni, "PathIni", "PathOn*", CGestIni::strip, CGestIni::entireLine );
     //...... rechercher ensuite une indirection selon Ip ou Hw avec repertoire type 192.168.100.217-227.........
     if ( lst.count()  !=0 )
        { //.............. analyse de toutes les lignes ................
          for (int i=0; i<lst.length(); ++i)
              { CGestIni::Param_SplitValeurs(  lst[i], "," , typIndir, true, &redirectPath, &sep, &prefix);
                if (redirectPath.length()==0) continue;
                if (sep.length()==0) sep = ".";
                if ( QDir::isRelativePath (redirectPath) ) { redirectPath.prepend(pathAppli); redirectPath = QDir::cleanPath(redirectPath); }
                if (!redirectPath.endsWith("/")) redirectPath  += "/";
                //......... faire la liste des chemins d'indirection ....................
                //          ceux contenus dans PathOnMacAdr=
                //                        dans PathOnIP=
                QString pathToTest = "";
                if ( typIndir == "PathOnIP" )    pathToTest  += prefix + ip.replace(".",sep);    // 192.168.100.217
                else                             pathToTest  += prefix + hw.replace(":",sep);
                int pos_v  = pathToTest.lastIndexOf(sep);
                if (pos_v==-1)    continue;           // pas d'adresse de la forme IP ou HW
                int value = pathToTest.mid(pos_v+1).toInt();
                QStringList dirList;
                CGestIni::listDirectory(dirList, redirectPath, "", nameAppli+";");
                for (int i=0; i<dirList.length(); ++i)
                    { QString dirToTest   = dirList[i];
                      QString lastSegment = "";
                      int pos             = dirToTest.lastIndexOf(sep);
                      if (pos==-1) continue;         // si pas XXX sep XXX sep XXX sep ... on est pas dans la forme ip ou hw
                      lastSegment         = dirToTest.mid(pos+1);
                      int pos_to          = lastSegment.indexOf("-to-");   // fourchette d'adresse
                      if ( pos_to != -1 && pathToTest.left(pos_v) == dirToTest.left(pos) )  // fourchette d'adresse et debut de l'adresse semblables a celle de l'iP en cours.
                         { int min = lastSegment.left(pos_to).toInt();
                           int max = lastSegment.mid(pos_to+4).toInt();
                           if (min>max) {int tmp = min; min=max;max=tmp;}

                           if ( value>=min && value<=max )
                              { dirToTest =  redirectPath + dirToTest + "/"+ nameAppli + ".ini";
                                if ( QFile::exists(dirToTest)     )   return dirToTest;
                              }
                         }
                      else            // adresse unique
                         { if (dirToTest == pathToTest)
                              { dirToTest =  redirectPath + dirToTest + "/"+ nameAppli + ".ini";
                                if ( QFile::exists(dirToTest)     )   return dirToTest;
                              }
                         }
                    }
                if ( typIndir == "PathOnIP" )    redirectPath  += prefix + ip.replace(".",sep);    // ../../IndirectConfig/IpAdr/192.168.100.217
                else                             redirectPath  += prefix + hw.replace(":",sep);
                redirectPath += "/"+ nameAppli + ".ini";                                           // ../../IndirectConfig/IpAdr/192.168.100.217/Manager.ini
                if ( QDir(redirectPath).isRelative() ) { redirectPath.prepend(pathAppli); redirectPath = QDir::cleanPath(redirectPath); }
                if ( QFile::exists(redirectPath)     )   return redirectPath;    // ====> CASSOS avec le chemin
              }
        }
     return path;
    }
 path = QDir::cleanPath(pathAppli + "../../../config/" + nameAppli + ".ini");
 if (QFile::exists(path)) return path;
 path = QDir::homePath()+"/."+ organizationName() + "/" + organizationDomain()+"/"+nameAppli+".ini";
 if (QFile::exists(path)) return path;
 path = QDir::cleanPath(pathAppli + "../../../config/MedinTuxGlobal.ini");
 if (QFile::exists(path)) return path;
 path = QDir::homePath()+"/."+ organizationName() + "/" + organizationDomain()+"/MedinTuxGlobal.ini";
 if (QFile::exists(path)) return path;
 path = QDir::cleanPath(pathAppli + "../../../MedinTuxGlobal.ini");
 return path;
}

//--------------------------------------------- getAsyncModifiers -------------------------------------------------------------------
#ifdef Q_OS_LINUX
     #include <QX11Info>
     #include <X11/XKBlib.h>         //  paquet  libx11_6-devel
     #include <stdio.h>


     //--------------------------------------------- getAsyncModifiers -------------------------------------------------------------------
     Qt::KeyboardModifiers C_AppCore::getAsyncModifiers(int *capitalState /*=0 */)
     {
         Qt::KeyboardModifiers result;

         XkbStateRec state;

         XkbGetState(QX11Info::display(), XkbUseCoreKbd, &state);
         if (state.base_mods & 0010)
             result |= Qt::AltModifier;
         if (state.base_mods & 0004)
             result |= Qt::ControlModifier;
         if (state.base_mods & 0001)
             result |= Qt::ShiftModifier;
         if (state.base_mods & 0100)
             result |= Qt::MetaModifier;
         if (capitalState) *capitalState = state.locked_mods;

         return result;
     }
#elif defined(Q_OS_MAC)
     /*
     #include "/opt/X11/include/X11/Xlib.h"
     #include "/opt/X11/include/X11/extensions/XKBstr.h"
     #include "/opt/X11/include/X11/XKBlib.h"
     */
     //--------------------------------------------- getAsyncModifiers -------------------------------------------------------------------
     Qt::KeyboardModifiers C_AppCore::getAsyncModifiers(int *capitalState /*=0 */)
     {   Qt::KeyboardModifiers result = QApplication::keyboardModifiers();

         if (capitalState) *capitalState = 0;
         /*
         XkbStateRec state;
         Display * d = XOpenDisplay((char*)0);

         //unsigned n;
         // XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
         // caps_state = (n & 0x01) == 1;

         XkbGetState(d, XkbUseCoreKbd, &state);
         if (state.base_mods & 0010)
             result |= Qt::AltModifier;
         if (state.base_mods & 0004)
             result |= Qt::ControlModifier;
         if (state.base_mods & 0001)
             result |= Qt::ShiftModifier;
         if (state.base_mods & 0100)
             result |= Qt::MetaModifier;
         if (capitalState) *capitalState = state.locked_mods;

         */
         return result;
     }

#elif defined(Q_WS_WIN)

     #include <windows.h>
     #include <winuser.h>
     //--------------------------------------------- getAsyncModifiers -------------------------------------------------------------------
     Qt::KeyboardModifiers C_AppCore::getAsyncModifiers(int *capitalState /*=0 */)
     {
         Qt::KeyboardModifiers result;

         if (GetAsyncKeyState(VK_MENU)    & 0x8000)
             result |= Qt::AltModifier;
         if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
             result |= Qt::ControlModifier;
         if (GetAsyncKeyState(VK_SHIFT)   & 0x8000)
             result |= Qt::ShiftModifier;
         if ((GetAsyncKeyState(VK_LWIN)   & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000))
             result |= Qt::MetaModifier;
         if (capitalState)
            { *capitalState = 0;
              if (GetKeyState(VK_CAPITAL) ==1)
                   *capitalState |= 2;
            }
         return result;
     }

#endif // Q_WS_WIN

//--------------------------------- get_Current_HW_Adr -----------------------------------------------------------------------
QString C_AppCore::get_Current_HW_Adr()
{QString ret = "";
 get_Current_IP_Adr(&ret);
 return ret;
}



//--------------------------------- get_Current_IP_Adr -----------------------------------------------------------------------
QString C_AppCore::get_Current_IP_Adr(QString *pMacAdr /* = 0 */)
     { QString hdadr_toUse = "";
       QString ipadr_toUse = "";
       if (G_currentIp.length())
          { if ( pMacAdr && G_currentMac.length())
                  { *pMacAdr =  G_currentMac;
                     return     G_currentIp;
                  }
             else return G_currentIp;
          }
       // QList <QHostAddress> lst = QNetworkInterface::allAddresses();
       // for (int i=0; i<lst.size(); ++i)
       //    {  qDebug() << QString("interface : %1").arg(lst[i].toString());
       //    }
       QList<QNetworkInterface> lstI = QNetworkInterface::allInterfaces ();
       for (int i=0; i<lstI.size(); ++i)
           {  //qDebug() << QString("=== interface Name : %1 Hard adresse : %2 Human name : %3 flags : %4" ).arg(lstI[i].name(), lstI[i].hardwareAddress (),lstI[i].humanReadableName (), networkInterfaceFlagToString (lstI[i].flags()));
              if ( hdadr_toUse.length()==0                           &&
                   lstI[i].flags()    & QNetworkInterface::IsUp      &&
                   lstI[i].flags()    & QNetworkInterface::IsRunning &&
                   ! (lstI[i].flags() & QNetworkInterface::QNetworkInterface::IsLoopBack)
                 )
                 { QList<QNetworkAddressEntry> lstA = lstI[i].addressEntries ();
                   if (lstA.size()==0)  continue;
                   hdadr_toUse = lstI[i].hardwareAddress();
                   for ( int u=0; u<lstA.size() && ipadr_toUse.length()==0; ++u)
                       { QString str = lstA[u].ip().toString();
                         // qDebug() << QString(".             ip : %1 " ).arg(str);
                         if ( str.contains('.') ) ipadr_toUse = str;
                       }
                 }  // end if ( hdadr_toUse.length()==0                           &&
           }
       // if (ipadr_toUse.length()) qDebug() << QString("to use ip : %1 hd : %2").arg(ipadr_toUse , hdadr_toUse);
       if (pMacAdr) {*pMacAdr      = hdadr_toUse.toUpper();
                      G_currentMac = *pMacAdr;}
       G_currentIp  = ipadr_toUse;
       return ipadr_toUse;
     }
//--------------------------------- networkInterfaceFlagToString -----------------------------------------------------------------------
QString C_AppCore::networkInterfaceFlagToString (QNetworkInterface::InterfaceFlags flags)
     {   QString ret = "";
         if (flags& QNetworkInterface::IsUp)             ret += tr("the network interface is active ;");
         if (flags& QNetworkInterface::IsRunning)        ret += tr("the network interface has resources allocated ;");
         if (flags& QNetworkInterface::CanBroadcast)     ret += tr("the network interface works in broadcast mode ;");
         if (flags& QNetworkInterface::IsLoopBack)       ret += tr("the network interface is a loopback interface: that is, it's a virtual interface whose destination is the host computer itself ;");
         if (flags& QNetworkInterface::IsPointToPoint)   ret += tr("the network interface is a point-to-point interface: that is, there is one, single other address that can be directly reached by it. ;");
         if (flags& QNetworkInterface::CanMulticast)     ret += tr("the network interface supports multicasting ;");
         if (ret.endsWith(";")) ret = ret.left(ret.length()-1);
         return ret;
     }
