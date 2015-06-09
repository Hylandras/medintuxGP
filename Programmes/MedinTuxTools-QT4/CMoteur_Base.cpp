/********************************* CMoteur_Base.cpp *******************************
 *  #include "CMoteur_Base.h"                                                     *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
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

//=============================================== INCLUDES ============================================================
#define VERSION_BASE    "02.16.000"
#define MAX_READ        32000
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSqlDriver>
#include <QFile>
#include <QHostInfo>
#include <stdlib.h>
#include <q3process.h>
#include <qwaitcondition.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QSqlQuery>
#include <QLabel>
#include <QPixmap>
#include <Q3SqlCursor>
#include <QSqlError>
#include <Q3CString>
#include <math.h>
#include <quuid.h>
#include <qeventloop.h>
#include "uuid/C_MyUuid.h"
#include <QDebug>

#include "CHtmlTools.h"
#include "CMoteur_Base.h"
#include "CPrtQListViewItem.h"

//=============================================== IMPLEMENTATION =====================================================
//-----------------------------------------------------  CMoteurBase -------------------------------------------
/**
 *
 * @param driver nom du driver : "QODBC3" "QMYSQL3" "QPSQL7"
 * @param dataBaseToConnect    :  nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
 * @param user                 :  login de connection �  la base de donn\303\251es
 * @param password             :  mot de passe de connection �  la base de donn\303\251es
 * @param hostname             :  adresse (IP, non d'hote, ou localhost si local) de connection �  la base de donn\303\251es
 * @param port                 :  port de connection �  la base de donn\303\251es (en g\303\251n\303\251ral 3306)
 * @param confData             :  donn\303\251es de configuration des bases (contenu du fichier 'DataBase.cfg')
 * @param baseLabel            :  nom d'identification de la base de donn\303\251es exemple "DRTUX_BASE"
 * @param errMess              :  pointeur sur une QString qui si pas \303\251gal �  z\303\251ro recevra les messages d'erreur
 * @param parent               :  pointeur sur un  QObject parent de cette classe (peut être nul alors faudra d\303\251truire CMoteurBase apres usage)
 * @param name                 :  nom de la l'objet (par d\303\251faut 'CMoteurBase')
 * @return
 */
// on verivfie deux coherences :
//     1) celle du numero de version du fichier DataConfig.cfg  m_VERSION_NUMBER qui doit etre la meme que celle exigee par ce code : VERSION_BASE
//     2) celle du numero de version de la base de donnee en cours qui doit etre la meme que celle exigee par ce code : VERSION_BASE

CMoteurBase::CMoteurBase(const QString & driver,             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                         const QString & dataBaseToConnect,  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                         const QString & user,               // = "root"
                         const QString & password,           // = ""
                         const QString & hostname,
                         const QString & port,
                               QString & confData,
                         const QString & baseLabel,
                         QString*        errMess,
                         QObject*        parent,
                         const QString & name,
                         verifyMode /* verifyMode  = CMoteurBase::verifyBaseStruct */)
: QObject(parent, name),                   // initialisation classique de l'objet
 C_DBVarDrTux(confData , errMess)           // initialiser les nom des variables champs de la base de donnee
{  int versionWhish  = normaliseVersion(VERSION_BASE);                               // version exigee de la base avec le programme actuel
   int versionInUse  = 0;
   //.............. initialiser les variables .....................................................
   m_DestBaseLabel   = "";
   m_DriverName      = driver;
   m_BaseName        = dataBaseToConnect;
   m_UserName        = user;
   m_PassWord        = password;
   m_HostName        = hostname;
   m_Port            = port;
   m_ConvertRun      = FALSE;
   m_BaseLabel       = baseLabel;
   m_pReconnectTimer = 0;
   m_GUID_CreateMode = CMoteurBase::byQT;
   //.............. verifier si les noms des champs de variables se sont initialisees correctement .........
   //               lors de l'appel du constructeur : C_DBVarDrTux(confFile ,errMess)
   //               une erreur possible est le fichier de nommage DataBase.cfg absent ou vide
   m_IsValid         = areFieldsNamesValids();   if ( !m_IsValid) return;
   //.............. initialiser et connecter la base .....................................................
   m_IsValid         = BaseConnect( driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                    dataBaseToConnect,  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                                    user,          // = "root"
                                    password,      // = ""
                                    hostname,      // = "localhost"
                                    port,
                                    errMess,
                                    baseLabel);
  if ( !m_IsValid) return;
  //................ verifier l'integrite des bases ...................................
  QString         mess = "";
  if (versionWhish    != normaliseVersion(m_VERSION_NUMBER,".") )  // on compare des chaines car bug de conversion 2.139999 etc... version de definition du fichier de configuration des bases
     {mess +=  tr("\r\n Incorrect 'DataBase.cfg' Configuration file:");
      mess +=  tr("\r\n 'DataBase.cfg' mandatory version: %1").arg(VERSION_BASE);
      mess +=  tr("\r\n 'DataBase.cfg' current version : %1").arg(m_VERSION_NUMBER);
      m_IsValid = 0;
     }
  else
     {QString versUsedStr = "";
      versionInUse    = GetMedinTuxNormalisedVersion(versUsedStr,".");    // retourne versUsedStr = 02.14.011
      if (versionInUse < versionWhish)
         {mess     +=  tr("\r\n Installed database: %1 not up to date, mandatory version: %2").arg(versUsedStr, m_VERSION_NUMBER);
          m_IsValid = verifyBaseIntegrity(confData, &mess);
         } // endif (versionInUse < versionWhish)
     } // endelseif(versionWhish   != m_VERSION_NUMBER.toDouble())
  if (m_IsValid)
     {m_pReconnectTimer       = new QTimer(this);
      if (m_pReconnectTimer)
         {m_pReconnectTimer->start (DB_TIME_TO_RECONNECT);
          connect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(On_ReconnectTimerDone()) );
         }
      }
  if (errMess) (*errMess) += mess;
}

//-----------------------------------------------------  ~CMoteurBase -------------------------------------------
CMoteurBase::~CMoteurBase()
{killBase();
}
//-----------------------------------------------------  killBase -------------------------------------------
void CMoteurBase::killBase()
{if (m_pReconnectTimer)
    {m_pReconnectTimer->stop();
     disconnect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(On_ReconnectTimerDone()) );
    }
 QSqlDatabase::database( m_BaseLabel ).close();
 QSqlDatabase::removeDatabase (m_BaseLabel);
}

//-----------------------------------------------------  GetMedinTuxVersion -------------------------------------------
double CMoteurBase::GetMedinTuxVersion()
{QString version;
 return GetMedinTuxVersion(version);
}

//-----------------------------------------------------  GetMedinTuxVersion -------------------------------------------
double CMoteurBase::GetMedinTuxVersion(QString &version)
{   GetMedinTuxNormalisedVersion(version, ".");   // 02.15.000
    QString tmp = version;
    return tmp.remove(5,1).toDouble();
}

//-------------------------------------- GetMedinTuxNormalisedVersion( ---------------------------------------------------
int CMoteurBase::GetMedinTuxNormalisedVersion()
{QString version="";
 return GetMedinTuxNormalisedVersion(version);
}

//-------------------------------------- GetMedinTuxNormalisedVersion( ---------------------------------------------------
int CMoteurBase::GetMedinTuxNormalisedVersion(QString &version, const QString &sep /*=""*/)
{if (OpenBase()==0)  return 0;
    version = "0000000";
    int ret =  0;
    QString tmp;
    QSqlQuery sqlQuery ( QString ("SELECT NumVers FROM %2 ").arg(m_VERSION_TBL_NAME) , QSqlDatabase::database(m_BaseLabel) );
    if (sqlQuery.isActive())
       {while (sqlQuery.next())
              {tmp     = sqlQuery.value(0).toString();
               if (tmp[1]=='.') tmp = tmp.prepend('0'); // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
               tmp  = tmp.remove('.');
               if ( tmp.length() <= 5 ) tmp +="000";
               if (tmp.toInt() > version.toInt())  version = tmp;  // attention  .toFloat() bug sous VC++
              }
       }
    CloseBase();
    ret     = version.toInt();
    if (sep.length()) version = version.left(2)+"."+version.mid(2,2)+"."+version.mid(4,3);
    return ret;
}

//-------------------------------------- normaliseVersion ---------------------------------------------------
int CMoteurBase::normaliseVersion(const QString &version, const QString &sep /*=""*/ )
{QString vers = version;
 return normaliseVersion(vers, sep  );
}
//-------------------------------------- normaliseVersion ---------------------------------------------------
int CMoteurBase::normaliseVersion(QString &version, const QString &sep /*=""*/ )
{if (version[1]=='.') version = version.prepend('0');   // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
 version = version.remove('.');
 if (version.length() <= 5 ) version +="000";
 int ret = version.toInt();
 if (sep.length()) version = version.left(2)+"."+version.mid(2,2)+"."+version.mid(4,3);
 return ret;
}




/*
//-----------------------------------------------------  GetMedinTuxVersion -------------------------------------------
double CMoteurBase::GetMedinTuxVersion()
{QString version;
 return GetMedinTuxVersion(version);
}
//-----------------------------------------------------  GetMedinTuxVersion -------------------------------------------
double CMoteurBase::GetMedinTuxVersion(QString &version)
{   if (OpenBase()==0)  return 0;
    version = "0.0";
    QString tmp;
    QSqlQuery sqlQuery ("SELECT NumVers FROM version " , QSqlDatabase::database(m_BaseLabel) );
    if (sqlQuery.isActive())
       {while (sqlQuery.next())
              {tmp     = sqlQuery.value(0).toString();
               if (tmp.toDouble() > version.toDouble())  version = tmp;  // attention  .toFloat() bug sous VC++
              }
       }
    CloseBase();
    return version.toDouble();
}
*/
//-----------------------------------------------------  SetMedinTuxVersion -------------------------------------------
void CMoteurBase::SetMedinTuxVersion(const QString &version)
{   if (OpenBase()==0)  return;
    QString requete =  QString("INSERT INTO ") + m_VERSION_TBL_NAME   + " (NumVers) VALUES ('" + version +"' )";
    QSqlQuery query(requete, QSqlDatabase::database(m_BaseLabel));
    CloseBase();
}

//-----------------------------------------------------  verifyBaseIntegrity -------------------------------------------
int  CMoteurBase::verifyBaseIntegrity(const QString &confFile, QString *errMess)
{QString mess;
 QString     tableNameWhish = "";
 QStringList fieldNameWhish;
 QString     tableName = "";
 QString     tbl       = "";
 int         majOk     = 1;
 int         nf        = 0;
 QStringList fieldName;
 QSqlDriver *pQSqlDriver =  QSqlDatabase::database(m_BaseLabel).driver();
 if ( pQSqlDriver == 0) { (*errMess) += tr("\r\n verifyBaseIntegrity() Driver not started"); return 0;}


 //..................... verifier si les tables deja en place correspondent avec celles ..................
 //                      indiquees par le fichier de configuration
 QStringList tablesList       = pQSqlDriver->tables(QSql::Tables);           tablesList.sort();
 QStringList tablesListWish   = getTableListFromCfgFile(confFile, errMess);  tablesListWish.sort();

 if (tablesList != tablesListWish)
    {//.................. recuperer le fichier SQL de creation initial des bases ............................
     //                   servira a regenerer les tables absentes
     QFileInfo qfi(qApp->argv()[0]);
     QString     fileSql    =  CGestIni::Construct_Name_File_Ini("set_bases", qfi.dirPath (true), "SqlCreateTable/Dump_DrTuxTest.sql" );
     //.................. premier niveau (benin) enumerer les tables presentes ........................................
     //                   dans la base et non indiquees par le fichier de config : confFile
     mess += tr("\r\n verifyBaseIntegrity() tables do not match  \r\n");
     if (tablesList.count() > tablesListWish.count())
        {    mess += tr("\r\n WARNING: these tables are not indicated in the configuration file: ");
             for ( QStringList::Iterator it = tablesList.begin(); it != tablesList.end(); ++it )
                 { tbl = *it;
                   if (tablesListWish.findIndex (tbl)==-1)
                      {mess += "\r\n    = " + tbl;
                      }
                 }
        }
      //.................. deuxieme niveau (fatal) enumerer les tables indiquees par le fichier ........................................
      //                   de config : confFile et non presentes en base et les regenerer
      for ( QStringList::Iterator it = tablesListWish.begin(); it != tablesListWish.end(); ++it )
          { tbl = *it;
            if (tablesList.findIndex (tbl.lower())==-1)
               {++nf;
                mess += tr("\n    = ") + tbl;
                if(tryToSetThisTable(tbl, fileSql, m_BaseLabel)==0)
                  {mess += tr(" ==> Failure attempting to create");
                   majOk = 0;
                  }
                else
                  {mess += tr(" ==> Success when attempting to create");
                  }
               }
          }
      if (majOk){mess = mess.prepend(tr("\n SUCCESS when updating missing tables."));}
      if (nf)   {mess = mess.prepend(tr("\n ERROR %1 table(s) not found in the present base: ").arg(nf));}
      (*errMess) += mess;
    }
 if (majOk) SetMedinTuxVersion(m_VERSION_NUMBER);
 return majOk;
}

//-----------------------------------------------------  tryToSetThisTable -------------------------------------------
int CMoteurBase::tryToSetThisTable(const QString &tbl, const QString fileSQL, const QString &baseLabel)
{   int tryCreate = 1;
    int tryInsert = 0;
    int ok        = 0;
    QByteArray   ba;
    QString toFindCreate = QString("CREATE TABLE `")+tbl+"`"; toFindCreate = toFindCreate.upper();
    QString toFindInsert = QString("INSERT INTO `") +tbl+"`"; toFindInsert = toFindInsert.upper();

    //....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fileSQL ) )          return 0;
    QFile file( fileSQL );
    if ( !file.open( QIODevice::ReadOnly  ) ) return 0;

    //.......... on d\303\251clare une variable pour lire le fichier ligne �  ligne ..................
    char *pt;
    QString requete       = "";
    QString line          = "";
    //long    line_number   = 0;

    QSqlQuery query(QString::null, QSqlDatabase::database(baseLabel));
    while (!file.atEnd() && (tryCreate || tryInsert>0))
          {
           ba = file.readLine(MAX_READ); line=ba; line=line.stripWhiteSpace();
           //.............................>>>>> CREATION  .....................
           if (tryCreate && line.left(toFindCreate.length()).upper()==toFindCreate)
              {  requete = line;
                 while (!file.atEnd())
                       { ba = file.readLine(MAX_READ); line=ba; line=line.stripWhiteSpace();
                         line.remove ("collate utf8_unicode_ci");
                         line.remove ("collate utf8_bin");
                         line.remove ("character set utf8");
                         requete += line;
                         if (line.left(13) == ") TYPE=MyISAM" || line.left(15) == ") ENGINE=MyISAM" )  // ne surtout pas inclure ';' dans la comparaison
                            { query.exec(requete);
                              tryInsert = 20;  // chercher si insertions pour cette table dans les 20 lignes apres
                              tryCreate = 0;    // on cherche plus la creation (c'est fait)
                              ok        = 1;
                              break;
                            }
                       } // end while (!file.atEnd())
              } // endif (line.left(toFindCreate.length()).upper()==toFindCreate)

           //...............................>>>>> INSERTION ........................
           //   INSERT INTO `RubriquesHead` VALUES (1,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20030000,'Asthme sai','2004-12-08 19:23:41','admin',0,0);
           //   INSERT INTO `RubriquesHead` VALUES (2,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20020200,'Bilan labo complet','2004-12-08 19:23:41','admin',0,0);
           //   INSERT INTO `ACCES2` VALUES ('A','�  foyer ouvert'),('A','par ... abord '),('A','par dissection ...');
           //   INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
           //   (26662,62153,'ABLAIN ST NAZAIRE'),
           //   (35076,80320,'ABLAINCOURT PRESSOIR');

           else if (tryInsert && line.left(toFindInsert.length()).upper()==toFindInsert)
              { int pos_i = line.find("VALUES");
                if (pos_i !=-1 )
                   { //............ isoler le verbe .................................................................
                     long len_line;
                     requete   ="";
                     //..................... isoler les donn\303\251es �  inserer ............................................
                     do
                      {
                        len_line = line.length();
                        pt = (char*)(const char*)line;
                        if (len_line>2 && line.mid(len_line-2) == ");")  // ne surtout pas inclure ';' dans la comparaison
                           {
                            requete += line;
                            ParseSQL_InsertInto(requete, baseLabel );
                            tryInsert=0;
                            break;
                           }
                        requete += line;
                        ba = file.readLine(MAX_READ); line=ba; line=line.stripWhiteSpace();
                      } while (!file.atEnd());
                   } //endif (pos_i !=-1 )
              } //endelse if (line.left(toFindInsert.length()).upper()==toFindInsert)
           if (tryCreate==0) --tryInsert;
          } // endwhile (!file.atEnd() && (tryCreate || tryInsert))
  return ok;
}
//-----------------------------------------------------  ParseSQL_InsertInto -------------------------------------------
// INSERT INTO `RubriquesHead` VALUES (1,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20030000,'Asthme sai','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `RubriquesHead` VALUES (2,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20020200,'Bilan labo complet','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `ACCES2` VALUES ('A','�  foyer ouvert'),('A','par ... abord'),('A','par dissection ...');
// INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
//  (26662,62153,'ABLAIN ST NAZAIRE'),
//  (35076,80320,'ABLAINCOURT PRESSOIR');
void  CMoteurBase::ParseSQL_InsertInto(QString &text, const QString &baseLabel)
{   int pos,deb;
    QSqlQuery query(QString::null, QSqlDatabase::database(baseLabel));
    deb = text.find("INSERT INTO");
    pos = text.find("VALUES");
    if (pos ==-1 || deb==-1)
       {   //Datasemp_OutMessage(logWidget,QObject::tr(" Erreur de syntaxe  INSERT INTO VALUES : ligne  ")+ QString::number(line_number));
        return;
       }
    //............ isoler le verbe .................................................................
    pos         += 6;
    QString verb = text.mid(deb, pos-deb)+" ";  // "INSERT INTO `ACCES2` VALUES "
    //............ parser le reste pour les valeurs ................................................
    long end = -1;
    bool run = TRUE;
    //...................... se mettre sur premiere parenthese de donn\303\251es ........................................
    deb      = text.find("(", pos);
    if ( deb==-1)
       {   //Datasemp_OutMessage(logWidget,QObject::tr(" Erreur de syntaxe  INSERT INTO VALUES : ligne ")+ QString::number(line_number));
        return;
       }
    //....................... parser ................................................
    while (run)
    {end = text.find("),(", deb);                      // chercher fin des donn\303\251es par le s\303\251parateur
        if (end==-1)                                      // si pas trouv\303\251 chercher derniere insertion
        {end = text.find(");", deb);
            if (end==-1)
               {   //Datasemp_OutMessage(logWidget,QObject::tr(" Erreur de syntaxe  INSERT INTO VALUES : ligne :  ")+ QString::number(line_number));
                return;
               }
            else
               {run = FALSE;
               }
        }
        //........................ si donn\303\251es valide les ins\303\251rer ...................................................
        if (end!=-1)
           {  QString line = verb + text.mid(deb, end-deb+1);
              query.exec(line);
              //OutSQL_error(query, QObject::tr("ERREUR INSERT INTO : ParseSQL_Dump()"), line );
              if (run) deb = end + 2;
           }
    } //end while (run)
}

//-----------------------------------------------------  getTableListFromCfgFile -------------------------------------------
QStringList  CMoteurBase::getTableListFromCfgFile(const QString &confFile, QString *  /*errMess */)
{QStringList list_Tables;
 int         isComment = 0;
 QString     tableName = "";
 int         eol;
 int pos_deb = confFile.find("_TBL_NAME");
 while (pos_deb != -1)
    {pos_deb     += 9;
     int deb_name = confFile.find("'", pos_deb);
     if (deb_name != -1)
        {//.............. verifier si pas dans un commentaire ...................
         int pos   =  pos_deb - 9; isComment = 0;
         while(isComment == 0 && pos>2 && ( ! (confFile[pos-1]=='\n'|| confFile[pos-1]=='\r')))
              {if (confFile[pos-2]=='/' && confFile[pos-1]=='/') isComment = 1;
               --pos;
              }
         if (isComment)
            {pos_deb  = confFile.find("_TBL_NAME", pos_deb);
            }
         else
            {
              ++deb_name;
              int end_name = confFile.find("'", deb_name);
              if (end_name != -1)
                 {tableName = confFile.mid(deb_name,end_name-deb_name).lower();
                  int f =     list_Tables.findIndex (tableName);
                  if (f==-1)  list_Tables.append(tableName);   // faut verifier car la table peut etre citee plusieurs fois

                  eol = confFile.find("\n", end_name); if (eol==-1) {eol = confFile.find("\r", end_name);}
                  if (eol != -1) end_name = eol;
                  pos_deb  = confFile.find("_TBL_NAME", end_name);
                 }
              else
                 {pos_deb = -1;
                 }
             }
         }
     else
         {pos_deb = -1;
         }
    }
 return list_Tables;
}

//-----------------------------------------------------  BaseConnect -------------------------------------------
int            CMoteurBase::BaseConnect(const QString & driver,              // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                        const QString & dataBaseToConnect,   // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                                        const QString & user,                // = "root"
                                        const QString & password,            // = ""
                                        const QString & hostname,            // = "localhost"
                                        const QString & port,
                                        QString*    errMess,
                                        QString     baseLabel)
    {   QSqlDatabase::removeDatabase (baseLabel);
        QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, baseLabel);
        defaultDB.setDatabaseName( dataBaseToConnect );
        defaultDB.setUserName    ( user );
        defaultDB.setPassword    ( password );
        defaultDB.setHostName    ( hostname);
        defaultDB.setPort        ( port.toInt());

        m_DriverName      = driver;
        m_BaseName        = dataBaseToConnect;
        m_UserName        = user;
        m_PassWord        = password;
        m_HostName        = hostname;
        m_Port            = port;
        m_BaseLabel       = baseLabel;

        if ( ! defaultDB.open() )
           {QString qstr = "";
            qstr += tr("Failed to open database : ")    + baseLabel + " Driver : " +  QString(driver) + "  " + QString(dataBaseToConnect) + "\r\n" +
                     defaultDB.lastError().driverText() + "\r\n" + defaultDB.lastError().databaseText();
            qWarning()<<qstr.toLatin1();
            if (errMess) *errMess += qstr;
            return 0;
           }
        return 1;
    }

//-----------------------------------------------------  OpenBase -------------------------------------------
int   CMoteurBase::OpenBase()
{return 1;
}
//-----------------------------------------------------  CloseBase -------------------------------------------
void   CMoteurBase::CloseBase()
{
}

//-----------------------------------------------------  On_ReconnectTimerDone -------------------------------------------
void CMoteurBase::On_ReconnectTimerDone()
{QSqlDatabase::database(m_BaseLabel).close();
 QSqlDatabase::database(m_BaseLabel).open();
 QSqlQuery query(QString::null , QSqlDatabase::database(m_BaseLabel) );
 query.exec(DUMMY_TEST_SQL_COMMAND);
}

//-----------------------------------------------------  GotoDebug -------------------------------------------
void CMoteurBase::GotoDebug()
{
}
//----------------------------------------------- GetNbRecord ---------------------------------------------------
QString CMoteurBase::GetNbRecord(const QString &table)
{ QString result = "";
  if (OpenBase()==0) return QString::null;

  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete = QString("SELECT COUNT(*) FROM ") + table;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
     {result = query.value(0).toString();
     }
  CloseBase();
  return result;
}

//--------------------------------- Utf8_Query ------------------------------------------------------------
QString CMoteurBase::Utf8_Query(QSqlQuery &cur, int field)
{Q3CString    cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 return CGestIni::fromMyUTF8(ptr);
 /*
 Q3CString    cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (CGestIni::IsUtf8( ptr ))
    {//return CGestIni::fromMyUTF8(ptr);
     return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }
 */
}

//************************************************************************************************************************
//=================================== GESTION DES CONVERSIONS DE BASE ET INTEGRITE ======================================
//************************************************************************************************************************
void CMoteurBase::RenameTableWhithCorrectName()
{if (OpenBase()==0)  return;
     QSqlQuery query(QString::null, QSqlDatabase::database(m_BaseLabel));
     //.................. renommer correctement les tables .....................................
     //                   (ces foutus dump de windows ne respectent pas la casse)
     RenameTableWhithCorrectName(m_DOSS_INDEX_TBL_NAME,            &query);
     RenameTableWhithCorrectName(m_DOSS_IDENT_TBL_NAME,            &query);
     RenameTableWhithCorrectName(m_DOSS_NOTE_TBL_NAME,             &query);
     RenameTableWhithCorrectName(m_DOSS_INTERVENANTS_TBL_NAME,     &query);
     RenameTableWhithCorrectName(m_CIM10_USER_FAV_TBL_NAME,        &query);
     RenameTableWhithCorrectName(m_INTERVENANTS_TBL_NAME,          &query);
     RenameTableWhithCorrectName(m_DOSS_RUB_HEAD_TBL_NAME,         &query);
     RenameTableWhithCorrectName(m_DOSS_RUB_DATA_TBL_NAME,         &query);
     RenameTableWhithCorrectName(m_USER_IDENT_TBL_NAME,            &query);
     RenameTableWhithCorrectName(m_USER_PERMS_TBL_NAME,            &query);
     RenameTableWhithCorrectName(m_USER_DATA_TBL_NAME,             &query);
     RenameTableWhithCorrectName(m_VERROU_TBL_NAME,                &query);
     RenameTableWhithCorrectName(m_EVNT_LIST_TBL_NAME,             &query);
     RenameTableWhithCorrectName(m_AGENDA_TBL_NAME,                &query);
     RenameTableWhithCorrectName(m_AGENDA_MASK_TBL_NAME,           &query);
     RenameTableWhithCorrectName(m_AGENDA_COLOR_PROFILS_TBL_NAME,  &query);
     RenameTableWhithCorrectName(m_VERSION_TBL_NAME,               &query);
 CloseBase();
}

//---------------------------------------- RenameTableWhithCorrectName ------------------------------------------------
void CMoteurBase::RenameTableWhithCorrectName(const QString &correct_name_table, QSqlQuery *pQSqlQuery)
{QStringList list = getDatabase().tables();
 QStringList::Iterator it = list.begin();
 while( it != list.end() )
      {if (correct_name_table.lower()== (*it).lower())
          {pQSqlQuery->exec(QString("ALTER TABLE `") + (*it) + "` RENAME `"+correct_name_table+"` ;");
          }
       ++it;
      }
}

//-----------------------------------------------------  ConvertToNew -------------------------------------------
int CMoteurBase::ConvertToNew(QLabel* pQlabelMess /* = 0 */, Q3ProgressBar* pQProgressBar /* = 0 */)
{int i=0;
 QString qstr;
 m_ConvertRun = TRUE;

 m_ConvertRun = 1;
 if (pQProgressBar)  pQProgressBar->setTotalSteps ( GetNbRecord(m_DOSS_RUB_HEAD_TBL_NAME).toLong() );

 //................. Preparer la requete .....................................
 if (OpenBase()==0)
    {if (pQlabelMess) pQlabelMess->setText(tr("ConvertToNew(): database can't be open"));
     return i;
    }

 //................. Preparer la requete .....................................
 QString      requete  = "SELECT ";
              requete += m_DOSS_RUB_HEAD_PRIMKEY + "," + m_DOSS_RUB_HEAD_GUID + " FROM " + m_DOSS_RUB_HEAD_TBL_NAME;
 QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 if (sqlQuery.isActive())
    { QSqlQuery     queryTarget (QString::null, QSqlDatabase::database(m_BaseLabel)  );
      //............................ Pour tous les enregistrements maitres ...............................
      while (sqlQuery.next() && m_ConvertRun)
        {//......................... Recuperer Infos maitres .............................................
         QString primKey_Master     = sqlQuery.value( 0 ).toString();   // Recuperer PrimKey maitre
         QString GUID_Master        = sqlQuery.value( 1 ).toString();   // Recuperer GUID    maitre
         requete                    = "";
         requete                   += "UPDATE "                     + m_DOSS_RUB_HEAD_TBL_NAME   + " SET "    +
                                       m_DOSS_RUB_HEAD_REF_BLOBS_PK + " = '" + primKey_Master    + "' WHERE " +
                                       m_DOSS_RUB_HEAD_PRIMKEY      + " ='"  + primKey_Master    + "'";
         queryTarget.exec (requete);
         if (queryTarget.isActive())
            {requete                    = "";
             requete                   += "UPDATE "                     + m_DOSS_RUB_DATA_TBL_NAME   + " SET "    +
                                          m_DOSS_RUB_DATA_GUID          + " = '" + GUID_Master       + "' WHERE " +
                                          m_DOSS_RUB_DATA_PRIMKEY       + " ='"  + primKey_Master    + "'";
             queryTarget.exec (requete);
             if (queryTarget.isActive())
                {
                 if (pQlabelMess)
                    {pQlabelMess->setText(qstr.sprintf("indexation NÂ°: %i", i));
                     if (pQProgressBar)  pQProgressBar->setProgress ( ++m_ConvertRun );
                     qApp->processEvents();qApp->processEvents();
                    }
                }
            }
         else
            {qstr   = "";
                 qstr  += requete;
             qstr  += tr("\r\nIndexation error: ");
             switch(queryTarget.lastError().type())
               {case QSqlError::None :        qstr  += "no error occurred";          break;
                case QSqlError::Connection :  qstr  += "connection error";           break;
                case QSqlError::Statement :   qstr  += "SQL statement syntax error"; break;
                case QSqlError::Transaction : qstr  += "transaction failed error";   break;
                case QSqlError::Unknown :     qstr  += "unknown error";              break;
               }
             if (pQlabelMess)
                 {pQlabelMess->setText(qstr);
                  qApp->processEvents();qApp->processEvents();
                 }
             return i;
            }
         i++;
        } //end while (pSqlQuery->next())
    } //end if (pSqlQuery && pSqlQuery->isActive())
 CloseBase();
 if (pQlabelMess) pQlabelMess->setText(qstr.sprintf("Operation terminee Â°: %i enregistrements traites", i));
 if (pQProgressBar)  pQProgressBar->setTotalSteps ( 0 );
 return i;
}

  //................. DOSS_INDEX ............................
  //QString      m_DOSS_INDEX_TBL_NAME;         // non de la table DU FICHIER GENERAL D'INDEX DES DOSSIERS
  //QString      m_DOSS_INDEX_GUID;             // un champ GUID unique qualifiant le numero de dossier
  //QString      m_DOSS_INDEX_PRIM_KEY;         // une clef primaire (doublon avec le GUID) mais permettant acces rapide

   //................. DOSS_IDENT ............................
   //QString      m_DOSS_IDENT_TBL_NAME;      // nom de la table d' IDENTITE SIGNALETIQUE
   //QString      m_DOSS_IDENT_GUID;          // GUID du dossier auquel l'enregistrement fait reference
   //QString      m_DOSS_IDENT_REF_PK;        // reference a la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX

   //................. DOSS_NOTE ............................
   //QString      m_DOSS_NOTE_TBL_NAME;               // nom de la table de la note du patient
   //QString      m_DOSS_NOTE_PAT_GUID;               // GUID du dossier auquel l'enregistrement fait reference
   //QString      m_DOSS_NOTE_PAT_PK ;                // Clef primaire du dossier auquel fait reference cette note

   //................. DOSS_INTERVENANTS ............................
   //QString      m_DOSS_INTERVENANTS_TBL_NAME;      // nom de la table des intervenants d'un patient
   //QString      m_DOSS_INTERVENANTS_PAT_GUID;      // GUID du dossier auquel l'enregistrement fait reference
   //QString      m_DOSS_INTERVENANTS_PAT_PK ;       // Clef primaire du dossier auquel fait reference cet intervenant

 //................. DOSS_RUB_HEAD ............................
  // QString      m_DOSS_RUB_HEAD_TBL_NAME;        // nom de la table de l'header des RUBRIQUES
  // QString      m_DOSS_RUB_HEAD_GUID;            // GUID du dossier auquel l'enregistrement fait reference
  // QString      m_DOSS_RUB_HEAD_REF_DOSS_PK;     // Clef primaire du dossier auquel l'enregistrement fait reference (doublon avec m_DOSS_RUB_HEAD_GUID)
  // QString      m_DOSS_RUB_HEAD_REF_BLOBS_PK;    // reference au Pk du blobs

  //................. EVNT_LIST ............................
  // QString      m_EVNT_LIST_TBL_NAME;          // nom de la table des droits des utilisateurs non prescripteurs
  // QString      m_EVNT_LIST_PK;                // clef primaire de cet enregistrement
  // QString      m_EVNT_DOSS_PK;                // clef primaire du dossier patient duquel depend ce patient
  // QString      m_EVNT_LIST_PERE;              // clef primaire d'un enr pere de la table m_EVNT_LIST_TBL_NAME
  // QString      m_EVNT_LIST_PKDOC;             // clef primaire d'un enr fils de la table DOSS_RUB_HEAD_TBL_NAME

  //................. DOSS_RUB_DATA ............................
  // QString      m_DOSS_RUB_DATA_TBL_NAME;        // nom de la table des donnees
  // QString      m_DOSS_RUB_DATA_GUID;            // GUID du dossier auquel l'enregistrement fait reference

//-----------------------------------------------------  ResoudreDoublons -------------------------------------------
int CMoteurBase::ResoudreDoublons(QTreeWidgetItem *pQListViewItem_Src, QTreeWidget *pQListView, int mode /*= CMoteurBase::esclave*/)
{

 //................. recuperer toutes les notes et les fusionner.....................................
 QString note     = "";
 QString str      = "";
 QString src_Prmk = pQListViewItem_Src->text(2);
 QString src_guid = pQListViewItem_Src->text(3);
 QString dst_Prmk = "";
 QString dst_guid = "";

 //................... concatener les notes .....................
 QTreeWidgetItemIterator ut( pQListView );
 while ( *ut )
    {QTreeWidgetItem *pQListViewItem = *(ut);
     PatientNoteGetData(pQListViewItem->text(2) ,  str);
     note += "\r\n" + str;
     ++ut;
    }

 //..................... s'occuper du reste ............................
 if (OpenBase()==0) return 0;

 QString requete;
 QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 QTreeWidgetItemIterator it( pQListView );
 while ( *it )
       { QTreeWidgetItem *pQListViewItem = *it;
         dst_Prmk = pQListViewItem->text(2);
         dst_guid = pQListViewItem->text(3);
         if (dst_guid != src_guid)
            {
             //............. pour tous les elements de la liste replacer partout le GUID et PK ......................
             //....................... l'agenda ............................
             requete                    = "";
             requete                   += "UPDATE  agenda  SET "
                                           "Nom   = '" + pQListViewItem_Src->text(0)   + "' WHERE " +
                                           "GUID  ='"  + dst_guid                      + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             requete                    = "";
             requete                   += "UPDATE  agenda  SET "
                                           "Prenom  = '" + pQListViewItem_Src->text(1) + "' WHERE " +
                                           "GUID    ='"  + dst_guid                    + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             requete                    = "";
             requete                   += "UPDATE  agenda  SET "
                                           "GUID  = '" + src_guid            + "' WHERE " +
                                           "GUID  ='"  + dst_guid            + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

             //....................... les intervenants ............................
             requete                    = "";
             requete                   += "UPDATE "                       + m_DOSS_INTERVENANTS_TBL_NAME + " SET "    +
                                           m_DOSS_INTERVENANTS_PAT_GUID   + " = '" + src_guid            + "' WHERE " +
                                           m_DOSS_INTERVENANTS_PAT_GUID   + " ='"  + dst_guid            + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             requete                    = "";
             requete                   += "UPDATE "                      + m_DOSS_INTERVENANTS_TBL_NAME   + " SET "    +
                                           m_DOSS_INTERVENANTS_PAT_PK    + " = '" + src_Prmk              + "' WHERE " +
                                           m_DOSS_INTERVENANTS_PAT_PK    + " ='"  + dst_Prmk              + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             //....................... les rubriques ............................
             requete                    = "";
             requete                   += "UPDATE "                    + m_DOSS_RUB_HEAD_TBL_NAME  + " SET "    +
                                           m_DOSS_RUB_HEAD_GUID        + " = '" + src_guid         + "' WHERE " +
                                           m_DOSS_RUB_HEAD_GUID        + " ='"  + dst_guid         + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             requete                    = "";
             requete                   += "UPDATE "                    + m_DOSS_RUB_HEAD_TBL_NAME  + " SET "    +
                                           m_DOSS_RUB_HEAD_REF_DOSS_PK + " = '" + src_Prmk         + "' WHERE " +
                                           m_DOSS_RUB_HEAD_REF_DOSS_PK + " ='"  + dst_Prmk         + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             //....................... les classeurs ............................
             requete                    = "";
             requete                   += "UPDATE "              + m_EVNT_LIST_TBL_NAME           + " SET "    +
                                           m_EVNT_DOSS_PK        + " = '" + src_Prmk              + "' WHERE " +
                                           m_EVNT_DOSS_PK        + " ='"  + dst_Prmk              + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             //....................... les blobs ............................
             requete                    = "";
             requete                   += "UPDATE "                    + m_DOSS_RUB_DATA_TBL_NAME  + " SET "    +
                                           m_DOSS_RUB_DATA_GUID        + " = '" + src_guid         + "' WHERE " +
                                           m_DOSS_RUB_DATA_GUID        + " ='"  + dst_guid         + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;
             //....................... l'agenda (tant pis c'est en dur) ............................
             requete                    = "";
             requete                   += "UPDATE agenda SET GUID = '" + src_guid         + "' WHERE " +
                                          "                  GUID = '" + dst_guid         + "'";
             sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

             //....................... effacer l'identite ............................
            sqlQuery.exec(QString("DELETE FROM ")      + m_DOSS_IDENT_TBL_NAME + " WHERE " + m_DOSS_IDENT_GUID    + "='" + dst_guid + "'");
            if (! sqlQuery.isActive()) return 0;
             //....................... effacer l'index ............................
            sqlQuery.exec(QString("DELETE FROM ")      + m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_GUID    + "='" + dst_guid + "'");
            if (! sqlQuery.isActive()) return 0;
            //..................... effacer la notes .......................................
            sqlQuery.exec(QString("DELETE FROM ")      + m_DOSS_NOTE_TBL_NAME  + " WHERE " + m_DOSS_NOTE_PAT_GUID + "='" + dst_guid + "'");
            if (! sqlQuery.isActive()) return 0;
           }
        ++it;
       }
   CloseBase();
   PatientNoteWrite( src_Prmk , src_guid,  note, mode);
   return 1;
}

// SIGEMS DEB
//------------------------------------------------------------------------------------
// Affectation d'un ENTRANT SIGEMS a un PATIENT EXISTANT.
// On remplace le GUID du patient par l'ID SIGEMS dans toutes les tables
//--------------------------------------------------------------------------------------
int CMoteurBase::Replace_GUID_Patient_By_ID_Sigems_Everywhere(QTreeWidgetItem *pQListViewItem_Src, QString ID_Sigems)
{
 QString guid_Pat = pQListViewItem_Src->text(3);
 // QString prmk_Pat = pQListViewItem_Src->text(2);

    if (guid_Pat == ID_Sigems)return 0;
    if (OpenBase()==0) return 0;

    QString requete;
    QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );

    // ................. la table IndexNomPrenom patient ..............................
    requete     = "UPDATE " + m_DOSS_INDEX_TBL_NAME   +
                  " SET "   + m_DOSS_INDEX_GUID   + " ='" + ID_Sigems + "'" +
                  " WHERE " + m_DOSS_INDEX_GUID   + " ='" + guid_Pat  + "'";
    sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

    // ................. la fiche patient ..............................
    requete     = "UPDATE " + m_DOSS_IDENT_TBL_NAME   +
                  " SET "   + m_DOSS_IDENT_GUID   + " ='" + ID_Sigems + "'" +
                  " WHERE " + m_DOSS_IDENT_GUID   + " ='" + guid_Pat  + "'";
    sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

    // ................. les notes ......................................
    requete     = "UPDATE " + m_DOSS_NOTE_TBL_NAME   +
                  " SET "   + m_DOSS_NOTE_PAT_GUID   + " ='" + ID_Sigems + "'" +
                  " WHERE " + m_DOSS_NOTE_PAT_GUID   + " ='" + guid_Pat  + "'";
    sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

    // agenda ................ mise a jour eventuelle du nom dans l'agenda
    requete     = "UPDATE  agenda  "
                  " SET Nom    = '" + pQListViewItem_Src->text(0) + "'" +
                  "  , Prenom  = '" + pQListViewItem_Src->text(1) + "'" +
                  " WHERE GUID ='"  + guid_Pat         + "'";
    sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

    requete     = "UPDATE  agenda  "
                  " SET GUID  = '"  + ID_Sigems   + "'" +
                  " WHERE GUID  ='" + guid_Pat    + "'";
    sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

    //....................... les intervenants ............................
   requete      = "UPDATE " + m_DOSS_INTERVENANTS_TBL_NAME +
                  " SET "   + m_DOSS_INTERVENANTS_PAT_GUID + " = '" + ID_Sigems  + "'" +
                  " WHERE " + m_DOSS_INTERVENANTS_PAT_GUID + " ='"  + guid_Pat   + "'";
   sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

   //....................... les rubriques ............................
   requete      = "UPDATE " + m_DOSS_RUB_HEAD_TBL_NAME  +
                  " SET "   + m_DOSS_RUB_HEAD_GUID      + " = '" + ID_Sigems     + "'" +
                  " WHERE " + m_DOSS_RUB_HEAD_GUID      + " ='"  + guid_Pat      + "'";
   sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

   //....................... les blobs ............................
   requete      = "UPDATE " + m_DOSS_RUB_DATA_TBL_NAME  +
                  " SET "   + m_DOSS_RUB_DATA_GUID      + " = '" + ID_Sigems     + "'" +
                  " WHERE " + m_DOSS_RUB_DATA_GUID      + " ='"  + guid_Pat      + "'";
   sqlQuery.exec (requete); if (! sqlQuery.isActive()) return 0;

   CloseBase();
   return 1;
}
// SIGEMS - FIN

//************************************************************************************************************************
//=================================== GESTION CODE POSTAL ================================================================
//************************************************************************************************************************

//-----------------------------------------------------  GetVilleFromCodePostal -------------------------------------------
QString CMoteurBase::GetVilleFromCodePostal(const QString &codePostal)
{ QString result  ("");
  QString requete ("SELECT ville FROM codes_postaux WHERE code_postal = ");
          requete +=  codePostal ;
  if (OpenBase()==0)   return result;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
     {result = Utf8_Query(query, 0);
     }
  CloseBase();
  return result;
}

//-----------------------------------------------------  GetCodePostalFromVille -------------------------------------------
QString CMoteurBase::GetCodePostalFromVille(const QString &ville)
{ QString result  ("");
  QString requete ("SELECT code_postal FROM codes_postaux WHERE ville = '");
          requete +=  ville + "'";
  if (OpenBase()==0)   return result;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
     {result = Utf8_Query(query,0);
     }
  CloseBase();
  return result;
}

//-----------------------------------------------------  GetVilleFromCodePostal -------------------------------------------
void CMoteurBase::GetVilleFromCodePostal(const QString &codePostal, QStringList &villeList)
{ QString cdPost (codePostal);
  if (cdPost[0]=='0') cdPost.remove(0,1);
  villeList.clear();
  QString requete ("SELECT ville FROM codes_postaux WHERE code_postal = ");
          requete +=  cdPost ;
  if (OpenBase()==0)   return ;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while(query.next()) villeList.append( Utf8_Query(query,0));
     }
  CloseBase();
  return;
}

//-----------------------------------------------------  GetCodePostalFromVille -------------------------------------------
void CMoteurBase::GetCodePostalFromVille(const QString &ville, QStringList &codePostalList)
{ codePostalList.clear();
  QString requete ("SELECT code_postal , ville FROM codes_postaux WHERE ville  LIKE '");
          requete +=  ville + "%'";
  if (OpenBase()==0)   return ;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while(query.next())
         {QString codePostal (query.value(0).toString());
          if (codePostal.length()<5) codePostal.prepend("0");
          codePostalList.append( codePostal +" "+ Utf8_Query(query,1));
         }
     }
  CloseBase();
  return;
}

//************************************************************************************************************************
//=================================== GESTION DOSSIER PATIENT ============================================================
//************************************************************************************************************************
//-----------------------------------------------------  GetPatientNomPrenomPk -------------------------------------------
void CMoteurBase::GetPatientNomPrenomPk( bool mustBeOpenClose, const QString   &pk,  QString   *nom,    QString   *prenom,   QString  *guid)
{ if (mustBeOpenClose && OpenBase()==0) return;
  QString requete = "SELECT ";
  requete        += m_DOSS_INDEX_NOM         + ", " + m_DOSS_INDEX_PRENOM + ", "      + m_DOSS_INDEX_GUID +
                    " FROM "                 + m_DOSS_INDEX_TBL_NAME      + " WHERE " +
                    m_DOSS_INDEX_PRIM_KEY    + " = '"                     + pk        + "' ";
  if (nom)    *nom    = "";
  if (prenom) *prenom = "";
  if (guid)   *guid   = "";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())
            { QString guid_i   = query.value(2).toString();
              if (nom)
                 { QString nom_i    = Utf8_Query(query, 0 );
                   QString prenom_i = Utf8_Query(query, 1 );
                   if       (prenom==0) *nom = nom_i + " " + prenom_i;
                   else     {*nom = nom_i; *prenom = prenom_i;}
                 }
              if (guid)      *guid = guid_i;
            }
    }
  if (mustBeOpenClose) CloseBase();
}

//-----------------------------------------------------  GetPatientNomPrenomPkFromGuid -------------------------------------------
int CMoteurBase::GetPatientNomPrenomPkFromGuid( bool mustBeOpenClose, const QString   &guid,  QString   *nom,    QString   *prenom,   QString  *pk)
{ if (mustBeOpenClose && OpenBase()==0) return 0;
  QString requete = "SELECT ";
  requete        += m_DOSS_INDEX_NOM         + ", " + m_DOSS_INDEX_PRENOM     + ", "      + m_DOSS_INDEX_PRIM_KEY +
                    " FROM "                 +        m_DOSS_INDEX_TBL_NAME   + " WHERE " +
                    m_DOSS_INDEX_GUID        + " = '"                         + guid      + "' ";
  if (nom)    *nom    = "";
  if (prenom) *prenom = "";
  if (pk)     *pk     = "";
  int          nb     = 0;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())
            { QString pk_i   = query.value(2).toString();
              if (nom)
                 { QString nom_i    = Utf8_Query(query, 0 );
                   QString prenom_i = Utf8_Query(query, 1 );
                   if       (prenom==0) *nom = nom_i + " " + prenom_i;
                   else     {*nom = nom_i; *prenom = prenom_i;}
                 }
              if (pk)      *pk = pk_i;
              ++nb;
            }
    }
  if (mustBeOpenClose) CloseBase();
  return nb;
}
//-----------------------------------------------------  GetPatientList -------------------------------------------
/*! \brief Remplit la QListView avec les patients retrouves dans la base de donnees.
 *  \param pQlistView : ListView qui recevra les patients recherches
 *  \param qsrt_nom : Nom a rechercher
 *  \param qsrt_prenom : Prenom a rechercher
 *  \param statusMess : message de retour
 *  \param errMess : Message d'erreur.
 *  \return nombre d'enregistrements inseres dans la QListView passee en parametre
*/
long CMoteurBase::GetPatientList(       QTreeWidget     *pQlistView,
                                  const QString         &qstr_nom,
                                  const QString         &qstr_prenom,
                                        QLabel          *statutMess, /* = 0  */
                                        QString         *errMess     /* = 0  */
                                )
{QString   q_nom    = "";
 QString   q_prenom = "";
 QString   tmp      = "";
 //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = tr("CMoteurBase::GetPatientList(): data base can't be open");
      return 0;
     }
  QString requete;
  //QString requete_ident;
  if (qstr_nom != ""||qstr_prenom!="")
     {q_nom    = qstr_nom;
      q_prenom = qstr_prenom;
     }
  q_nom.replace("\'","\\\'");
  q_prenom.replace("\'","\\\'");
  int nb_columns = pQlistView->columnCount();
  requete        = "SELECT ";
  requete       += m_DOSS_INDEX_NOM + ", " + m_DOSS_INDEX_PRENOM      + ", \n";
  requete       += m_DOSS_INDEX_PRIM_KEY   + ", " + m_DOSS_INDEX_GUID + " \n";
  requete       += " FROM "                + m_DOSS_INDEX_TBL_NAME    + " \nWHERE ";
  if (qstr_nom.startsWith("_"))
     {if (qstr_nom.startsWith("_GUID="))
         { requete       += m_DOSS_INDEX_GUID       + " = '"+ q_nom.mid(6)         + "' \n ";
         }
      else if (qstr_nom.startsWith("_PK="))
         { requete       += m_DOSS_INDEX_PRIM_KEY   + " = '"+ q_nom.mid(4)         + "%' \n ";
         }
      else return 0;
     }
  else
     {
       requete       += m_DOSS_INDEX_NOM        + " LIKE '"+ q_nom         + "%' \nAND ";
       requete       += m_DOSS_INDEX_PRENOM     + " LIKE '"+ q_prenom      + "%' \n";
     }
  requete       += " ORDER BY "            + m_DOSS_INDEX_NOM + ","   + m_DOSS_INDEX_PRENOM + " LIMIT "+ ST_LIST_PATIENT_MAX;

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //QSqlQuery query_ident;

  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview

  int i  = 0;
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
  {pQlistView->clear();
   while (query.next())
   { //if (i < 200)
     { QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
       if (pQTreeWidgetItem)
          {
           //CPrtQListViewItem* line = 0;
           //............... on rempli la listbox selon son nombre de colonnes .....................
           switch (nb_columns)
           {case 1:
                pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));
                break;
            case 2:
                pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));
                pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));
                break;
            case 3:
                pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));
                pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));
                pQTreeWidgetItem->setText(2, Utf8_Query(query, 2 ));
                break;
            case 4:
                pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));   // Nom
                pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));   // Prenom
                pQTreeWidgetItem->setText(2, Utf8_Query(query, 2 ));   // PK
                pQTreeWidgetItem->setText(3, Utf8_Query(query, 3 ));   // GUID
                break;
            case 8 : // Recupere les elements d'identite en plus
               { /*
                 requete_ident        = "SELECT " + m_DOSS_IDENT_RUE + ", " + m_DOSS_IDENT_CODE_POST + ", \n";
                 requete_ident        += m_DOSS_IDENT_VILLE + ", " + m_DOSS_IDENT_JFNOM + ", \n";
                 requete_ident        += m_DOSS_IDENT_TEL1 + ", " + m_DOSS_IDENT_TEL2 + " \n";
                 requete_ident        += " FROM " + m_DOSS_IDENT_TBL_NAME + " \nWHERE ";
                 requete_ident        += m_DOSS_IDENT_REF_PK    + " = "+ query.value(2).toString() + "; \n";
                 query_ident = QSqlQuery(requete_ident , QSqlDatabase::database(m_BaseLabel) );
                 if ( query_ident.isActive() && query_ident.next())
                 { line = new CPrtQListViewItem( pQlistView , line, Utf8_Query(query, 0 )); // Nom
                   // Si pas de CP enregistre alors il est == 0 ==> ne pas l'afficher
                   tmp = Utf8_Query(query_ident, 1) ;
                   if (tmp == "0") tmp ="";
                   line->setText( 1, Utf8_Query(query_ident, 3) );        // Nom JF
                   line->setText( 2, Utf8_Query(query, 1 ) );                // Prenom
                   line->setText( 3, Utf8_Query(query_ident, 0) );        // Rue
                   line->setText( 4, tmp );        // CP
                   line->setText( 5, Utf8_Query(query_ident, 2));        // Ville
                   line->setText( 6, Utf8_Query(query_ident, 4));        // Tel 1
                   line->setText( 7, Utf8_Query(query_ident, 5));        // Tel 2
                   line->setMultiLinesEnabled ( FALSE );                //Plusieurs lignes par item si necessaire
                   //......... Dichotomiser Affichage et donnees a exploiter ..........
                   line->setNom(    Utf8_Query(query, 0 ) );                // Nom
                   line->setPrenom( Utf8_Query(query, 1 ) );                // Prenom
                   line->setPkDoc(  Utf8_Query(query, 2 ) );                // PK
                   line->setGUID(   Utf8_Query(query, 3 ) );                // GUID
                 }
                 */
                break;
               }
            } // end switch (nb_columns)
           pQlistView->addTopLevelItem(pQTreeWidgetItem);
          } //endif (pQTreeWidgetItem)
       //else if (line) pQlistView->insertItem ( line );
       ++i;
     } // // fin if i < 200
     //++nb;
   } //end while (pSqlQuery->next())

   // Cacher les contenus des colonnes 8 et 9 si 10 demandes
   if (nb_columns == 10)
   { //pQlistView->hideColumn ( 8 );
     //pQlistView->hideColumn ( 9 );
   }
  } //endif (pSqlQuery && pSqlQuery->isActive())

  //ListViewPatient->setCurrentItem(0);   // se placer sur le premier enregistrement de la liste
  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     {int nb      = GetNbRecord(m_DOSS_INDEX_TBL_NAME).toInt();
      QString txt = tr("Displayed %1 among %2").arg(i).arg(nb);
      //QString txt = QApplication::translate("CMoteurBase", "Displayed %1 among %2", 0, QApplication::UnicodeUTF8).arg(i).arg(nb);
      if ( i>=NB_LIST_PATIENT_MAX)
         {txt.prepend("<html><head><meta name=\"qrichtext\" content=\"1\" /> "
                      "</head><body><span style=\"font-weight:600; color:#ff0000;\">");
          txt +=      "</span></body></html>";
         }
      statutMess->setText( txt );
     }
  CloseBase();
  return i;
}

//--------------------------------- ComputeClef_Secu ----------------------------
QString CMoteurBase::ComputeClef_Secu(const char *txt)
{if (strlen(txt) !=13) return QString("  ");
    QString result (QString::number( 97 - fmod(atof(txt) , 97)));
    if (result.length()<2) result.prepend('0');
    return result;
}

//-----------------------------------------------------  GetPatientListFromNomPrenomSecuDtNssGeme -------------------------------------------
/*! \brief Remplit la QListView avec les patients retrouves dans la base de donnees.
 *  \param pQlistView : QTreeWidget qui recevra les patients recherches
 *  \param qsrt_nom : Nom a rechercher
 *  \param qsrt_prenom : Prenom a rechercher
 *  \param statusMess : message de retour
 *  \param errMess : Message d'erreur.
 *  \return nombre d'enregistrements inseres dans la QListView passee en parametre
*/
long CMoteurBase::GetPatientListFromNomPrenomSecuDtNssGeme( QTreeWidget     *pQlistView,
                                                            const QString   &qstr_nom,
                                                            const QString   &qstr_prenom,
                                                            const QString   &qstr_DtNss,
                                                            const QString   &qstr_Secu,
                                                            const QString   &qstr_Geme,
                                                            const QString   &typeAssure,       // 0 c'est l'assure pour le reste c'est les ayant droit
                                                            const QString   &adresse,
                                                            const QString   &codePost,
                                                            const QString   &ville,
                                                            const QString   &sexe,
                                                            const QString   &profession,
                                                            const QString   &nomJfille,
                                                            const QString   &nomAss,
                                                            const QString   &prenomAss,
                                                            const QString   &tel1,
                                                            const QString   &tel2,
                                                            const QString   &tel3,
                                                            const QString   &email,
                                                            const QString   &egal_like,
                                                            QLabel          *statutMess,       /* = 0  */
                                                            QString         *errMess           /* = 0  */
                                                           )
{
 QString   q_nom      = "";
 QString   q_prenom   = "";
 QString   tmp        = "";
 QString   and_where  = " WHERE ";

 //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = TR ("CMoteurBase::GetPatientListFromNomPrenomSecuDtNssGeme(): data base can't be open");
      return 0;
     }

  int jour, mois, annee;
  jour  = qstr_DtNss.mid(8,2).toInt();
  mois  = qstr_DtNss.mid(5,2).toInt();
  annee = qstr_DtNss.mid(0,4).toInt();

  QString requete, requete_ident;
  if (qstr_nom != ""||qstr_prenom!="")
     {q_nom    = qstr_nom;
      q_prenom = qstr_prenom;
     }
  q_nom.replace("\'","\\\'");
  q_prenom.replace("\'","\\\'");

  requete        = "SELECT ";
  requete       += m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_NOM      + ", " +
                   m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_PRENOM   + ", " +
                   m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_PRIM_KEY + ", " +
                   m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_GUID     + "  " +
                   " FROM  "  + m_DOSS_INDEX_TBL_NAME+ " INNER JOIN " + m_DOSS_IDENT_TBL_NAME +
                   " ON    "  + m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_PRIM_KEY+ "="  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_REF_PK;// + " LIMIT 0,50";

  if (q_nom.length())
     {requete   += and_where  + m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_NOM        + egal_like+ "'" + q_nom       +"'";                            and_where = " AND ";}
  if (q_prenom.length())
     {requete   += and_where  + m_DOSS_INDEX_TBL_NAME+"."+m_DOSS_INDEX_PRENOM     + egal_like+ "'" + q_prenom    +"'";                            and_where = " AND ";}
  if (qstr_Secu.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_NUMSS      + egal_like+ "'" + qstr_Secu   +"'";                            and_where = " AND ";}
  if (QDate::isValid ( annee, mois, jour ) ) // ne requeter que si la date de la CV est valide
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_NSS        + egal_like+ "'" + qstr_DtNss  +"'";                            and_where = " AND ";}       // 1993-06-01 00:00:00
  else if (typeAssure.length() && typeAssure.toInt()==0)            // ne requeter que si l'ayant droit est l'assure (le reste peut Ãªtre indetermine car autre anciennes valeurs peu sures )
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_IS_ASS     + egal_like+ "'" + QString::number(typeAssure.toInt()+1)  +"'"; and_where = " AND ";}   // sur le disuqe les ayant droit sont stokes avec +1
  if (qstr_Geme.length() )
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_GEME       +          " ='" + qstr_Geme   +"'";                            and_where = " AND ";}
  if (adresse.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_RUE        + egal_like+ "'" + adresse     +"'";                            and_where = " AND ";}
  if (codePost.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_CODE_POST  + egal_like+ "'" + codePost    +"'";                            and_where = " AND ";}
  if (ville.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_VILLE      + egal_like+ "'" + ville       +"'";                            and_where = " AND ";}
  if (sexe.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_SEX        + egal_like+ "'" + sexe        +"'";                            and_where = " AND ";}
  if (profession.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_PROF       + egal_like+ "'" + profession  +"'";                            and_where = " AND ";}
  if (nomJfille.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_JFNOM      + egal_like+ "'" + nomJfille   +"'";                            and_where = " AND ";}
  if (nomAss.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_NOM_ASS    + egal_like+ "'" + nomAss      +"'";                            and_where = " AND ";}
  if (prenomAss.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_PRENOM_ASS + egal_like+ "'" + prenomAss   +"'";                            and_where = " AND ";}
  if (tel1.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_TEL1       + egal_like+ "'" + tel1        +"'";                            and_where = " AND ";}
  if (tel2.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_TEL2       + egal_like+ "'" + tel2        +"'";                            and_where = " AND ";}
  if (tel3.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_TEL3       + egal_like+ "'" + tel3        +"'";                            and_where = " AND ";}
  if (email.length())
     {requete   += and_where  + m_DOSS_IDENT_TBL_NAME+"."+m_DOSS_IDENT_EMAIL      + egal_like+ "'" + email       +"'";                            and_where = " AND ";}

  //int pos =  requete.findRev("AND"); if (pos != -1) requete.truncate(pos);  //enlever le dernier 'AND' surtout pas !!!!
  //CGestIni::Param_UpdateToDisk("/home/ro/sql.txt", requete);
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  QSqlQuery query_ident;

  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview
  int i  = 0;
  int nb = 0;
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {int nb_columns = 0;
      if (pQlistView) {pQlistView->clear(); nb_columns = pQlistView->columnCount();}
      while (query.next())
         {if (i < 200)
             {if (pQlistView)
                 {QTreeWidgetItem *element = new QTreeWidgetItem();
                  if (element)
                     {//CPrtQListViewItem*  line = 0;
                      //............... on rempli la listbox selon son nombre de colonnes .....................
                      switch (nb_columns)
                             {case 1:
                                   {element->setText(0, Utf8_Query(query, 0 ));
                                    break;
                                   }
                              case 2:
                                   {element->setText(0, Utf8_Query(query, 0 ));
                                    element->setText(1, Utf8_Query(query, 1 ));
                                    break;
                                   }
                                case 3:
                                   {element->setText(0, Utf8_Query(query, 0 ));
                                    element->setText(1, Utf8_Query(query, 1 ));
                                    element->setText(2, Utf8_Query(query, 2 ));
                                    break;
                                   }
                                case 4:
                                   {element->setText(0, Utf8_Query(query, 0 ));   // Nom
                                    element->setText(1, Utf8_Query(query, 1 ));   // Prenom
                                    element->setText(2, Utf8_Query(query, 2 ));   // PK
                                    element->setText(3, Utf8_Query(query, 3 ));   // GUID
                                    break;
                                   }
                                case 8 : // Recupere les elements d'identite en plus
                                   {/*
                                    requete_ident        = "SELECT " + m_DOSS_IDENT_RUE + ", " + m_DOSS_IDENT_CODE_POST + ", \n";
                                    requete_ident       += m_DOSS_IDENT_VILLE + ", " + m_DOSS_IDENT_JFNOM + ", \n";
                                    requete_ident       += m_DOSS_IDENT_TEL1 + ", " + m_DOSS_IDENT_TEL2 + " \n";
                                    requete_ident       += " FROM " + m_DOSS_IDENT_TBL_NAME + " \nWHERE ";
                                    requete_ident       += m_DOSS_IDENT_REF_PK    + " = "+ query.value(2).toString() + "; \n";
                                    query_ident = QSqlQuery(requete_ident , QSqlDatabase::database(m_BaseLabel) );
                                    if ( query_ident.isActive() && query_ident.next())
                                       {line = new CPrtQListViewItem( pQlistView , line, Utf8_Query(query, 0 )); // Nom
                                        // Si pas de CP enregistre alors il est == 0 ==> ne pas l'afficher
                                        tmp = Utf8_Query(query_ident, 1) ;
                                        if (tmp == "0") tmp ="";
                                        line->setText( 1, Utf8_Query(query_ident, 3) );          // Nom JF
                                        line->setText( 2, Utf8_Query(query, 1 ) );               // Prenom
                                        line->setText( 3, Utf8_Query(query_ident, 0) );          // Rue
                                        line->setText( 4, tmp );        // CP
                                        line->setText( 5, Utf8_Query(query_ident, 2));           // Ville
                                        line->setText( 6, Utf8_Query(query_ident, 4));           // Tel 1
                                        line->setText( 7, Utf8_Query(query_ident, 5));           // Tel 2
                                        line->setMultiLinesEnabled ( FALSE );                    //Plusieurs lignes par item si necessaire
                                        // Dichotomiser Affichage et donnees a exploiter +++
                                        line->setNom(    Utf8_Query(query, 0 ) );                // Nom
                                        line->setPrenom( Utf8_Query(query, 1 ) );                // Prenom
                                        line->setPkDoc(  Utf8_Query(query, 2 ) );                // PK
                                        line->setGUID(   Utf8_Query(query, 3 ) );                // GUID
                                       }
                                     */
                                    break;
                                   }
                             } // end switch (nb_columns)
                       /*if (element)*/   pQlistView->addTopLevelItem(element);
                       //else if (line) pQlistView->insertItem ( line );
                     }   // endif (element)
                 } //endif (pQlistView)
              i++;
             }    // fin if i < 200
          nb++;
         } //end while (pSqlQuery->next())

      // Cacher les contenus des colonnes 8 et 9 si 10 demandes
      if (nb_columns == 10)
         { pQlistView->hideColumn ( 8 );
           pQlistView->hideColumn ( 9 );
         }
     }  //endif (pSqlQuery && pSqlQuery->isActive())

  //ListViewPatient->setCurrentItem(0);   // se placer sur le premier enregistrement de la liste
  //....................... sortir le message d'erreur si besoin ..........................................

  if (statutMess)
     { statutMess->setText( QString( tr("Displayed %1 among %2") ).arg(i).arg(nb) );
     }
  CloseBase();
  return i;
}

//-------------------------------------- GetIdentiteFromPrimKey ----------------------------------------------------------------------------
/*! \brief Selectionne l'enregistrement de la table: DOSS_IDENT  correspondant au bon primary key
*   \return QSqlQuery*  zero si echec valide sinon et AUQUEL CAS IL FAUDRA DETRUIRE ce pointeur par: delete  pSqlQuery et AUQUEL CAS IL FAUDRA FERMER la base par:       CloseBase()
*/
QSqlQuery*  CMoteurBase::GetIdentiteFromPrimKey(const char *primKey )
{QString requete;

 requete   = "SELECT * FROM "    + m_DOSS_IDENT_TBL_NAME + " WHERE ";
 requete  += m_DOSS_IDENT_REF_PK + " ='"                 + primKey + "'";

 if (OpenBase()==0)
     {return 0;
     }
 QSqlQuery* pSqlQuery = new QSqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
    {return pSqlQuery;
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return 0;
}

//-------------------------------------- GetIdentiteFromGUID ----------------------------------------------------------------------------
/*! \brief Selectionne l'enregistrement de la table: DOSS_IDENT  correspondant au bon primary key
*   \return QSqlQuery*  zero si echec valide sinon et AUQUEL CAS IL FAUDRA DETRUIRE ce pointeur par: delete  pSqlQuery et AUQUEL CAS IL FAUDRA FERMER la base par:       CloseBase()
*/
QSqlQuery*  CMoteurBase::GetIdentiteFromGUID(const char *guid )
{QString requete;

 requete   = "SELECT * FROM "    + m_DOSS_IDENT_TBL_NAME + " WHERE ";
 requete  += m_DOSS_IDENT_GUID   + " ='"                 + guid + "'";

 if (OpenBase()==0)      return 0;
 QSqlQuery* pSqlQuery = new QSqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
    {return pSqlQuery;
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return 0;
}

//-------------------------------------- reindexerFichePatient ----------------------------------------------------------------------------
/*! \brief replace le bon Pk de l'index patient sur tous les enregistrement concernes a partir du GUID patient
 *  \param gui : guid du patient
 *  \return QString pk_du dossier si erreur(pas de dosseir pour ce pk) est vide
*/
QString CMoteurBase::reindexerFichePatient( QString guid )
{  QString requete;
   QString index_pk = GetDossPrimKeyFromGUID(guid, 0);
   if (index_pk.length()==0) return "";

   QSqlQuery query( QSqlDatabase::database(m_BaseLabel) );
   //............. pour tous les elements de la liste replacer partout le GUID et PK ......................
   //....................... la fiche patient ............................
   requete                    = "UPDATE "               + m_DOSS_IDENT_TBL_NAME    + " SET "    +
                                  m_DOSS_IDENT_REF_PK   + " = '" + index_pk        + "' WHERE " +
                                  m_DOSS_IDENT_GUID     + " ='"  + guid            + "'";
   query.exec (requete);      if (! query.isActive()) return "";

   //....................... les notes ............................
   requete                    = "UPDATE "                + m_DOSS_NOTE_TBL_NAME   + " SET "    +
                                 m_DOSS_NOTE_PAT_PK      + " = '" + index_pk      + "' WHERE " +
                                 m_DOSS_NOTE_PAT_GUID    + " ='"  + guid          + "'";
   query.exec (requete);      if (! query.isActive()) return "";

   //....................... les intervenants ............................
   requete                    = "UPDATE "                      + m_DOSS_INTERVENANTS_TBL_NAME   + " SET "    +
                               m_DOSS_INTERVENANTS_PAT_PK      + " = '" + index_pk      + "' WHERE " +
                               m_DOSS_INTERVENANTS_PAT_GUID    + " ='"  + guid          + "'";
   query.exec (requete);      if (! query.isActive()) return "";

   //....................... les internevants ............................
   requete                    = "UPDATE "                   + m_DOSS_RUB_HEAD_TBL_NAME   + " SET "    +
                              m_DOSS_RUB_HEAD_REF_DOSS_PK   + " = '" + index_pk          + "' WHERE " +
                              m_DOSS_RUB_HEAD_GUID          + " ='"  + guid              + "'";
   query.exec (requete);      if (! query.isActive()) return "";

   //....................... les internevants ............................
   requete                    = "UPDATE "               + m_EVNT_LIST_TBL_NAME       + " SET "    +
                              m_EVNT_DOSS_PK            + " = '" + index_pk          + "' WHERE " +
                              m_DOSS_RUB_HEAD_GUID      + " ='"  + guid              + "'";
   query.exec (requete);      if (! query.isActive()) return "";
   return index_pk;
}

//************************************************************************************************************************
//=================================== GESTION DES FUSIONS ================================================================
//************************************************************************************************************************


//------------------------------------------------------- GetFieldValue --------------------------------------------------
QString CMoteurBase::GetFieldValue(QString table, QString fieldToGet, QString fieldToMatch, QString valueToMatch,  QVariant *pQVariant /*= 0*/ )
{QString   ret("");
 //............... ouvrir la base  ..................................................
 if  (OpenBase()==0)  return ret;
 ret = GetFieldValue_WithoutOpenCloseBase( table,  fieldToGet,  fieldToMatch,  valueToMatch,  pQVariant);
 CloseBase();
 return ret;
 }

//------------------------------------------------------- GetFieldValue_WithoutOpenCloseBase --------------------------------------------------
QString CMoteurBase::GetFieldValue_WithoutOpenCloseBase(QString table, QString fieldToGet, QString fieldToMatch, QString valueToMatch,  QVariant *pQVariant /*= 0*/ )
{QString   ret("");
 //............... crer un curseur SQL ..................................................
 Q3SqlCursor cur(table, TRUE, QSqlDatabase::database(m_BaseLabel));                                          // ouvrir un curseur sur la table
 if (!cur.select( fieldToMatch+"='"+valueToMatch+"'" )) goto  GetFieldValueError;       // se placer sur le bon record
 if (cur.next())                             {Q3CString     cString  =  cur.value(fieldToGet).toCString ();
                                              char *ptr             =  cString.data();
                                              if (pQVariant)  *pQVariant = cur.value( fieldToGet);

                                              if (CGestIni::IsUtf8( ptr ))
                                                 {ret = QString::fromUtf8 ( ptr );
                                                 }
                                              else
                                                 {ret = QString(ptr) ;
                                                 }
                                              return ret;
                                             }
GetFieldValueError:
     OutSQL_error(cur, "GetFieldValue()", cur.filter());
     return QString::null;
 }

//************************************************************************************************************************
//=================================== GESTION DES NOTES ET INTERVENANTS PATIENT ==========================================
//************************************************************************************************************************

//-------------------------------------- PatientIntervenantsGetData ----------------------------------------------------------------------------
/*! \brief Fonction qui filtre, recupere et renseigne une QTreeWidget avec la liste des intervenants correspondant au patient dont la primKeyPat est donnee en entree
 *  \param pQlistView     QTreeWidget*  : pointeur sur la QTreeWidget a remplir avec la liste des intervenants
 *  \param primKeyPat     const char*   : pointeur sur la clef primaire du patient
 *  \param errMess        QString *    : pointeur (par defaut nul) sur une QString dans laquelle retourner un eventuel message d'erreur
*/

void CMoteurBase::PatientIntervenantsGetData(const char *primKeyPat , QTreeWidget *pQListView, QString *errMess /*=0*/)
{ pQListView->clear();
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = "CMoteurBase::PatientIntervenantsGetData(): database can't be opened";
      return ;
     }

  //................. creer la requete .....................................
  QString requete("");
  requete += "SELECT " + m_INTERVENANTS_TBL_NAME+"."+m_INTERVENANTS_SPEC               + ","  +     // 0
                         m_INTERVENANTS_TBL_NAME+"."+m_INTERVENANTS_NOM                + ","  +     // 1
                         m_INTERVENANTS_TBL_NAME+"."+m_INTERVENANTS_PRENOM             + ","  +     // 2
                         m_DOSS_INTERVENANTS_TBL_NAME+"."+m_DOSS_INTERVENANTS_TYPE     + ","  +     // 3
                         m_DOSS_INTERVENANTS_TBL_NAME+"."+m_DOSS_INTERVENANTS_INTERVPK +            // 4
             " FROM "  + m_DOSS_INTERVENANTS_TBL_NAME+" INNER JOIN " + m_INTERVENANTS_TBL_NAME   +
             " ON "    + m_DOSS_INTERVENANTS_TBL_NAME+"."+m_DOSS_INTERVENANTS_INTERVPK + " = "   + m_INTERVENANTS_TBL_NAME+"."+m_INTERVENANTS_PK +
             " WHERE " + m_DOSS_INTERVENANTS_TBL_NAME+"."+m_DOSS_INTERVENANTS_PAT_PK   + " = '"  + primKeyPat  + "' ";
  //................. executer la requete .....................................
  QSqlQuery  query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     { while (query.next())
             { QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
               if (pQTreeWidgetItem)
                  {pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));
                   pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));
                   pQTreeWidgetItem->setText(2, Utf8_Query(query, 2 ));
                   pQTreeWidgetItem->setText(3, Utf8_Query(query, 3 ));
                   pQTreeWidgetItem->setText(4, Utf8_Query(query, 4 ));
                   pQListView->addTopLevelItem(pQTreeWidgetItem);
                  }
             }
     }
  CloseBase();
  return ;
}

//-------------------------------------- PatientIntervenantsGetData ----------------------------------------------------------------------------
QStringList CMoteurBase::PatientIntervenantsGetData(const QString &qualite, const QString &sep/*="|"*/, QString *errMess /*=0*/)
{ QStringList intervList;
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = "CMoteurBase::PatientIntervenantsGetData(): database can't be opened";
      return intervList;
     }

  //................. creer la requete .....................................
  QString requete("");
  requete += "SELECT " + m_INTERVENANTS_NOM                + ","     +     // 0
                         m_INTERVENANTS_PRENOM             + ","     +     // 1
                         m_INTERVENANTS_SPEC               + ","     +     // 2
                         m_INTERVENANTS_PK                 + " "     +     // 3
             " FROM "  + m_INTERVENANTS_TBL_NAME           +
             " WHERE " + m_INTERVENANTS_SPEC               + " = '"  + qualite  + "' ORDER BY " + m_INTERVENANTS_NOM;
  //CGestIni::Param_UpdateToDisk("C:/requete.txt",requete);
  //................. executer la requete .....................................
  QSqlQuery  query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     { while (query.next())
             {intervList.append(Utf8_Query(query, 0  ) + sep +  // nom
                                Utf8_Query(query, 1  ) + sep +  // prenomn
                                Utf8_Query(query, 2  ) + sep +  // specialite
                                Utf8_Query(query, 3  ));        // pk
              }
     }
  CloseBase();
  return intervList;
}

//-------------------------------------- PatientIntervenantsGetData ----------------------------------------------------------------------------
void CMoteurBase::PatientIntervenantsGetData(const char *primKeyPat , QStringList &intervList, const QString &specialite, const char *validStr, const char *decalage) //Medecin generaliste
{ QStringList pkIntervList;
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  return ;

  //................. recuperer tous les PK des intervenants sur ce patient ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_INTERVENANTS_INTERVPK  + " FROM " + m_DOSS_INTERVENANTS_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_INTERVENANTS_PAT_PK    + " = '"   + primKeyPat                     + "' ";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     { while (query.next())
         {pkIntervList.append( query.value( 0 ).toString());
         }
     }
  intervList.clear();
  QString fieldToRetrieve = "SELECT ";
  char *pt = (char *) validStr;
  while (*pt)
      {if (pt[0]!= ',')
          {fieldToRetrieve += IdentValidCarToFieldName(pt[0]);
           ++pt;
           if (*pt) fieldToRetrieve += ",";
          }
       else ++pt;
      }
  if (pkIntervList.count()==0)  {CloseBase(); return;}
  for ( QStringList::Iterator it = pkIntervList.begin(); it != pkIntervList.end(); ++it )
      {requete   = fieldToRetrieve;
       requete  += " FROM " + m_INTERVENANTS_TBL_NAME+ " WHERE ";
       requete  +=   m_INTERVENANTS_PK  + " = '"   + *(it) + "' ";
       if (specialite.length()) requete  += "AND " + m_INTERVENANTS_SPEC + " = '" + specialite + "'";
       query.exec( requete );
       if (query.isActive())
          {QString ofset = decalage ;
           while (query.next())
              {QString str = ofset;        // on commence au decalage
               int       i = 0;
                        pt = (char *) validStr;
               while (*pt)
                     {if (pt[0]== ',') str +=  QString("<br>") + ofset ; // la prochaine ligne commence au decalage
                      else
                                      {QString  result = Utf8_Query(  query, i++  );
                                       result = result.replace("\r\n", "\n");
                                       result = result.replace("\n",   QString("<br>") + ofset) ;
                                       str   += result + " ";
                                      }
                      ++pt;
                     }
              intervList.append(str);
              }
          }
      }
  CloseBase();
  return ;
}

//-------------------------------------- IdentValidCarToFieldName ----------------------------------------------------------------------------
QString CMoteurBase::IdentValidCarToFieldName(int c)
{switch (c)
   {case 'N': return m_INTERVENANTS_NOM;
    case 'P': return m_INTERVENANTS_PRENOM;
    case 'A': return m_INTERVENANTS_RUE;
    case 'C': return m_INTERVENANTS_CODE_POST;
    case 'V': return m_INTERVENANTS_VILLE;
    case 'T': return m_INTERVENANTS_TITRE;
    case 'S': return m_INTERVENANTS_SPEC;
   }
 return QString("");
}

//-------------------------------------- PatientIntervenantsWrite ----------------------------------------------------------------------------
int CMoteurBase::PatientIntervenantsWrite( const char *primKeyPat , const char* guidPat,  const char* intervPk, const QString &qualite)
{QString pk = PatientIntervenantsIsThisExist(intervPk, primKeyPat);
 if (pk.length()>0) return PatientIntervenantsUpdate(pk, primKeyPat , guidPat,   intervPk, qualite);                                                   // existe deja donc a ne pas creer
 else               return PatientIntervenantsCreate(primKeyPat , guidPat,   intervPk, qualite);     // n'existe pas donc creer
}

//---------------------------------------------------- PatientIntervenantsIsThisExist -----------------------------------------------
// RETOUR:    la primKey des donnees si elles existent sinon ""
QString CMoteurBase::PatientIntervenantsIsThisExist( const char* intervPk, const char *primKeyPat, QLabel *errMess )
{ QString ret("");
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::PatientIntervenantIsThisExist(): database can't be opened");
      return ret;
     }
  //................. filtrer selon donnee d'entree ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_INTERVENANTS_PK          + " FROM " + m_DOSS_INTERVENANTS_TBL_NAME   + "  WHERE ";
  requete  +=             m_DOSS_INTERVENANTS_INTERVPK    + " = '"   + intervPk                       + "' AND ";
  requete  +=             m_DOSS_INTERVENANTS_PAT_PK      + " = '"   + primKeyPat                     + "'  ";

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //.................. si la requete a un resultat ..............................................
  if (query.isActive() && query.next())
     {ret = Utf8_Query(query, 0 );
     }
  CloseBase();
  return ret;
}
//-------------------------------------- PatientIntervenantsUpdate ----------------------------------------------------------------------------
int CMoteurBase::PatientIntervenantsUpdate(const QString &pk, const char *primKeyPat , const char* guidPat,  const char* intervPk, const QString &qualite)
{
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //............... crer un curseur SQL ..................................................

  Q3SqlCursor cur(m_DOSS_INTERVENANTS_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(m_DOSS_INTERVENANTS_PK  + "='" + pk  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  if ( cur.next() )
    { //............... si OK on recupere le buffer et on le charge avec les donnees .........
      QSqlRecord *buffer = cur.primeUpdate();                         // recuperer le buffer d'insertion
      buffer->setValue( m_DOSS_INTERVENANTS_PAT_PK,        primKeyPat );  // y placer les donnees
      buffer->setValue( m_DOSS_INTERVENANTS_PAT_GUID,      guidPat );     // y placer les donnees
      buffer->setValue( m_DOSS_INTERVENANTS_INTERVPK,      intervPk    );     // y placer les donnees
      buffer->setValue( m_DOSS_INTERVENANTS_TYPE,          qualite    );     // y placer les donnees
      ok = cur.update();                                              // ecrire le buffer avec les donnees du header
    }
  if (ok==0) OutSQL_error(cur, "PatientIntervenantsUpdate()");
                                        // ecrire le buffer avec les donnees du header
  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//-------------------------------------- PatientIntervenantsCreate ----------------------------------------------------------------------------
int CMoteurBase::PatientIntervenantsCreate(const char *primKeyPat , const char* guidPat,  const char* intervPk, const QString &qualite)
{
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_DOSS_INTERVENANTS_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  buffer->setValue( m_DOSS_INTERVENANTS_PAT_PK,        primKeyPat );  // y placer les donnees
  buffer->setValue( m_DOSS_INTERVENANTS_PAT_GUID,      guidPat );     // y placer les donnees
  buffer->setValue( m_DOSS_INTERVENANTS_INTERVPK,      intervPk    );     // y placer les donnees
  buffer->setValue( m_DOSS_INTERVENANTS_TYPE,          qualite    );     // y placer les donnees
  ok = cur.insert();                                          // ecrire le buffer avec les donnees du header
  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//-------------------------------------- PatientIntervenantsDelete ----------------------------------------------------------------------------
int CMoteurBase::PatientIntervenantsDelete( const char *primKey, const char *patient_GUID /* = 0 */)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;

  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_DOSS_INTERVENANTS_TBL_NAME  + " WHERE ";
          requete += m_DOSS_INTERVENANTS_INTERVPK  + "= '" + primKey + "' ";
  if (patient_GUID)
         {requete += " AND " + m_DOSS_INTERVENANTS_PAT_GUID  + "= '" + patient_GUID + "' ";
         }
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//-------------------------------------- PatientIntervenantsAllDelete ----------------------------------------------------------------------------
int CMoteurBase::PatientIntervenantsAllDelete( const char *primKeyPat)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_DOSS_INTERVENANTS_TBL_NAME  + " WHERE ";
          requete += m_DOSS_INTERVENANTS_PAT_PK    + "= '" + primKeyPat + "' ";
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//---------------------------------------------------- PatientNoteIsThisExist -----------------------------------------------
/*! \brief  RETOUR:    la primKey des donnees si elles existent sinon "" */
QString CMoteurBase::PatientNoteIsThisExist(  const char *primKeyPat,
                                              QLabel          *errMess          /* = 0  */
                                           )
{ QString ret("");
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::IsThisPatientNoteExist(): database can't be opened");
      return ret;
     }
  //................. filtrer selon donnee d'entree ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_NOTE_PK        + " FROM " + m_DOSS_NOTE_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_NOTE_PAT_PK    + " = '"   + primKeyPat             + "' ";

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //.................. si la requete a un resultat ..............................................
  if (query.isActive() && query.next())
     {ret = Utf8_Query(query, 0 );
     }
  CloseBase();
  return ret;
}

//-------------------------------------- PatientNoteDelete ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteDelete( const char *primKey,  int  /*= CMoteurBase::esclave*/)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_DOSS_NOTE_TBL_NAME + " WHERE ";
          requete += m_DOSS_NOTE_PK       + "= '" + primKey                  + "' ";
 QSqlQuery sqlQuery (QString::null , QSqlDatabase::database(m_BaseLabel) );
 //if (mode==CMoteurBase::progMasterUpdate) synchroWriteMofidication(m_DOSS_NOTE_TBL_NAME, primKey, m_DOSS_NOTE_PK, "D");
 ok = sqlQuery.exec(requete);
 CloseBase();
 return ok;
}

//-------------------------------------- PatientNoteCreate ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteCreate(const char *primKeyPat , const char* guidPat,  const char* data,  int  /*= CMoteurBase::esclave*/)
{ QString pk  = "";
  int     ok  = FALSE;
  if (OpenBase()==0)  return 0;

  QString prepare  = "";
  prepare         += "INSERT INTO "  + m_DOSS_NOTE_TBL_NAME  + " ( \n";
  prepare         += m_DOSS_NOTE_PAT_PK   + " ,\n";
  prepare         += m_DOSS_NOTE_PAT_GUID + " ,\n";
  prepare         += m_DOSS_NOTE_HTML     + " )\n  VALUES (?,?,?) ";
  QSqlQuery query(QSqlDatabase::database(m_BaseLabel));
  query.prepare(prepare);
  query.bindValue(0, primKeyPat);
  query.bindValue(1, guidPat);
  query.bindValue(2, data);
  if ( (ok = query.exec()))
     { //requete  = "SELECT LAST_INSERT_ID() FROM ";
      QString   requete  = QString("SELECT ") +  m_DOSS_NOTE_PK +" FROM " + m_DOSS_NOTE_TBL_NAME + " WHERE " + m_DOSS_NOTE_PAT_PK + " = '"+primKeyPat+"'";
      QSqlQuery queryPK(requete , QSqlDatabase::database(m_BaseLabel) );
      if (queryPK.isActive() && queryPK.next())  pk = queryPK.value(0).toString();
      //if (pk.length() && mode==CMoteurBase::progMasterUpdate) synchroWriteMofidication(m_DOSS_NOTE_TBL_NAME, pk, m_DOSS_NOTE_PK, "C");
    }
  CloseBase();
  return ok;
}

//-------------------------------------- PatientNoteWrite ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteWrite( const char *primKeyPat , const char* guidPat,  const char* data,  int mode /*= CMoteurBase::esclave*/)
{QString pk = PatientNoteIsThisExist(primKeyPat);
 if (pk.length()>0)
    {if (data[0]==0) return PatientNoteDelete(pk, mode);
     else            return PatientNoteUpdate(pk, primKeyPat , guidPat, data, mode);
    }
 else
    {if (data[0]!=0) return PatientNoteCreate(primKeyPat , guidPat, data, mode);
    }
 return 0;
}

//-------------------------------------- PatientNoteUpdate ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteUpdate(const char* primKey, const char *primKeyPat , const char* guidPat,  const char* data,  int  /*= CMoteurBase::esclave*/)
{
  if (OpenBase()==0)  return FALSE;
  QString prepare  = QString("UPDATE ")  + m_DOSS_NOTE_TBL_NAME   + " SET \n";
  prepare         += m_DOSS_NOTE_PAT_PK   + " = ? ,\n";
  prepare         += m_DOSS_NOTE_PAT_GUID + " = ? ,\n";
  prepare         += m_DOSS_NOTE_HTML     + " = ?  \n";
  prepare         += " WHERE " + m_DOSS_NOTE_PK   + " ='"  + primKey  + "'";
  QSqlQuery query(QSqlDatabase::database(m_BaseLabel));
  query.prepare(prepare);
  query.bindValue(0, primKeyPat);
  query.bindValue(1, guidPat);
  query.bindValue(2, data);
  int ok =  query.exec();
  //if ( (ok = query.exec()) && mode==CMoteurBase::progMasterUpdate)
  //   {synchroWriteMofidication(m_DOSS_NOTE_TBL_NAME, primKey, m_DOSS_NOTE_PK, "U");
  //   }
  CloseBase();
  return ok;
}

//-------------------------------------- PatientNoteGetData ----------------------------------------------------------------------------
void CMoteurBase::PatientNoteGetData(const char *primKeyPat ,  QString& data, QString *errMess /*=0*/)
{ data = "";
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = "CMoteurBase::PatientNoteGetData(): database can't be opened";
      return ;
     }
  //................. filtrer selon donnee d'entree ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_NOTE_HTML        + " FROM " + m_DOSS_NOTE_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_NOTE_PAT_PK      + " = '"   + primKeyPat             + "' ";

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //.................. si la requete a un resultat ..............................................
  if (query.isActive() && query.next())
     {data = Utf8_Query(query, 0 );
     }
  CloseBase();
  return ;
}

//************************************************************************************************************************
//=================================== GESTION DES UTILISATEURS ===========================================================
//************************************************************************************************************************
//************************************************************************************************************************
//=================================== GESTION DES PARAMETRES =============================================================
//************************************************************************************************************************

 //................. USER_PERMS ............................
 //  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue a cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorises avec les droits)
//-------------------------------------- PermsUserCreateOrUpdate ----------------------------------------------------------------------------
QString CMoteurBase::PermsUserCreateOrUpdate(const QString &primKey , const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs)
{if (primKey.length()<=0)
    {return PermsUserCreate(droits , signGUID, permsGUID,  blobs);
    }
 PermsUserUpdate(primKey , droits , signGUID, permsGUID,  blobs);
 return primKey;
}

//-------------------------------------- PermsUserUpdate ----------------------------------------------------------------------------
int CMoteurBase::PermsUserUpdate(const QString &primKey , const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;

  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_USER_PERMS_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canUpdate ()) {/*DeVerrouilleTable(QSqlDatabase::database(m_BaseLabel)); */CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_USER_PERMS_PK + "='" + primKey + "'");
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer ( pas de delete a la fin !!)
     buffer->setValue( m_USER_PERMS_FRIEND_DROITS,        droits );      // y placer les donnees
     buffer->setValue( m_USER_PERMS_SIGN_GUID,            signGUID );    // y placer les donnees
     buffer->setValue( m_USER_PERMS_FRIEND_GUID,          permsGUID );   // y placer les donnees
     buffer->setValue( m_USER_PERMS_FRIEND_BLOBS,         blobs );       // y placer les donnees
     ok = cur.update();                                                  // ecrire le buffer avec les donnees du header
    }
  //......................... fermer la base ..............................................
  //DeVerrouilleTable(QSqlDatabase::database(m_BaseLabel));
  CloseBase();
  return ok;
}

//-------------------------------------- PermsUserCreate ----------------------------------------------------------------------------
QString CMoteurBase::PermsUserCreate(const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs)
{
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_USER_PERMS_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  QSqlRecord *buffer = cur.primeInsert();                             // recuperer le buffer d'insertion
  buffer->setValue( m_USER_PERMS_FRIEND_DROITS,        droits );      // y placer les donnees
  buffer->setValue( m_USER_PERMS_SIGN_GUID,            signGUID );    // y placer les donnees
  buffer->setValue( m_USER_PERMS_FRIEND_GUID,          permsGUID );   // y placer les donnees
  buffer->setValue( m_USER_PERMS_FRIEND_BLOBS,         blobs );       // y placer les donnees
  ok = cur.insert();                                                  // ecrire le buffer avec les donnees du header

  //................. reselectionner l'enregistrement cree pour retrouver son PK avec le GUID temporaire........................
  QString prim_key = QString::null;
  QString   requete("SELECT ");
  requete += m_USER_PERMS_PK            + " FROM " + m_USER_PERMS_TBL_NAME   + " WHERE "
          +  m_USER_PERMS_SIGN_GUID     + "='"     + signGUID                + "' AND  "
          +  m_USER_PERMS_FRIEND_GUID   + "='"     + permsGUID               + "' "; // retrouver le Pk de l'enregistrement cree avec le GUID temporaire
  QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
  if (sqlQuery.isActive() && sqlQuery.next()) prim_key = sqlQuery.value(0).toString();
  //......................... fermer la base ..............................................
  CloseBase();
  return prim_key;
}

//-------------------------------------- PermsUserDeleteFromPk ----------------------------------------------------------------------------
int CMoteurBase::PermsUserDeleteFromPk( const char *primKey)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_USER_PERMS_TBL_NAME  + " WHERE ";
          requete += m_USER_PERMS_PK        + "= '" + primKey + "' ";
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}
//-------------------------------------- PermsUserRefToUserDelete ----------------------------------------------------------------------------
int  CMoteurBase::PermsUserRefToUserDelete( const char *userGUID)
{//................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_USER_PERMS_TBL_NAME       + " WHERE ";
          requete += m_USER_PERMS_FRIEND_GUID    + "= '" + userGUID + "' ";
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//-------------------------------------- PermsUserListDelete ----------------------------------------------------------------------------
int CMoteurBase::PermsUserListDelete( const char *signUserGUID)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_USER_PERMS_TBL_NAME     + " WHERE ";
          requete += m_USER_PERMS_SIGN_GUID    + "= '" + signUserGUID + "' ";
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//-----------------------------------------------------  PermsUserGetList -------------------------------------------
//   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
//   QString      m_USER_IDENT_NOM;                // Nom
//   QString      m_USER_IDENT_PRENOM;             // Prenom

long CMoteurBase::PermsUserGetList(          Q3ListView *pQlistView,
                                       const QString   &signGUID,
                                       const QString   &imagePath,
                                        QLabel    *statutMess, /* = 0  */
                                        QLabel    *errMess     /* = 0  */
                             )
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::GetUserPermsList(): database can't be opened");
      return 0;
     }
  pQlistView->clear();
  QString requete = "SELECT "  + m_USER_PERMS_PK                + ", "                                                // 0
                               + m_USER_PERMS_SIGN_GUID         + ", "                                                // 1
                               + m_USER_PERMS_FRIEND_GUID       + ", "                                                // 2
                               + m_USER_PERMS_FRIEND_DROITS     + ", "                                                // 3
                               + m_USER_PERMS_FRIEND_BLOBS      + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_SIGN_GUID         + " = '"   + signGUID +"'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //Param_UpdateToDisk("/home/ro/TestSql.txt", requete);
  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int       nb = 0;
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {QString requetePermsUser = "SELECT "  + m_USER_IDENT_LOGIN                + ", "                                                 // 0
                                               + m_USER_IDENT_NOM                  + ", "                                                 // 1
                                               + m_USER_IDENT_PRENOM               + " FROM " + m_USER_IDENT_TBL_NAME + " WHERE  "        // 2
                                               + m_USER_IDENT_GUID                 + " = '"   + Utf8_Query(query, 2 ) + "'";              // m_USER_PERMS_FRIEND_GUID
         QSqlQuery queryPermsUser(requetePermsUser , QSqlDatabase::database(m_BaseLabel) );
          if (queryPermsUser.isActive())
            {
             while (queryPermsUser.next())
                   { Q3ListViewItem *pQListViewItem = new Q3ListViewItem( pQlistView ,
                                                                        Utf8_Query(queryPermsUser, 0 ),       // login
                                                                        Utf8_Query(queryPermsUser, 1 ),       // nom
                                                                        Utf8_Query(queryPermsUser, 2 ),       // prenom
                                                                        "",                                   // +
                                                                        "",                                   // -
                                                                        Utf8_Query(query,          3 ),       // droits
                                                                        Utf8_Query(query,          0 ),       // pk
                                                                        Utf8_Query(query,          2 )        // GUID du permsUser
                                                                      );
                     if (pQListViewItem)
                        {pQListViewItem->setText(8,Utf8_Query(query,          4 ) );          // BLOBS du permsUser
                         pQListViewItem->setPixmap (3, QPixmap(imagePath+"Plus.png") ) ;
                         pQListViewItem->setPixmap (4, QPixmap(imagePath+"Moins.png") ) ;
                        }

                     nb++;
                  }  //endwhile (queryPermsUser.next())
           } //endif (queryPermsUser.isActive())
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())

  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { statutMess->setText(tr("Found: ") + QString::number(nb));
     }
  CloseBase();
  return nb;
}
//-----------------------------------------------------  TypeDoc_To_PrintTypeDoc -------------------------------------------
int  CMoteurBase::TypeDoc_To_PrintTypeDoc(int typeDoc)
{if (typeDoc >= TYP_ORDO_CALC && typeDoc <= TYP_ORDONNANCE)
    {return TYP_ORDONNANCE;  // sub type 0->RECTO   1->VERSO
    }
 else if (typeDoc == TYP_COURRIER)
    {return TYP_COURRIER;
    }
 else if (typeDoc == TYP_CERTIFICAT)
    {return TYP_CERTIFICAT;
    }
 else if (typeDoc == TYP_OBSERVATION)
    {return TYP_OBSERVATION;
    }
 else if (typeDoc >= TYP_PRESCRIPTION &&  typeDoc <= TYP_PRESCRIPTION+999)
    {return  TYP_ORDONNANCE;
    }
 else if (typeDoc >= TYP_DOCUMENTS &&  typeDoc <= TYP_DOCUMENTS+999)
    {return  TYP_DOCUMENTS;
    }
 return 0;
}

//-----------------------------------------------------  GetUserPrimKeyFromNumOrdre -------------------------------------------
QString  CMoteurBase::GetUserPrimKeyFromNumOrdre(QString nir, QString droit /* = QString::null*/, QStringList *pFiedList /*=0*/)
{ //................. Preparer la requete .....................................
  QString ret = "";
  int nbField = 0;
  int       i = 0;
  if (OpenBase()==0)  { return QString::null; }
  nir = nir.remove(' ').left(9);      // corriger les evntuels espace parasites rentres a la main et infos suplementaires
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
  requete += m_USER_IDENT_PRIMKEY + "," + m_USER_IDENT_NUM_ORDRE;
  if (pFiedList)
     {nbField = pFiedList->count();
      for ( QStringList::Iterator it = pFiedList->begin(); it != pFiedList->end(); ++it )
          {requete += "," + *it;
          }
     }
  requete += " FROM " + m_USER_IDENT_TBL_NAME;
  if (droit.length())
     {if (droit[0]!='-') droit.prepend('-');
      requete += " WHERE " + m_USER_IDENT_DROITS + " LIKE'%" +droit +"%'";
     }
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())
           {QString numOrdre = query.value( 1 ).toString();
            numOrdre         = numOrdre.remove(' ').left(9);
            if (numOrdre == nir)
               {if (nbField)
                   {pFiedList->clear();
                    for ( i = 0; i < nbField; ++i ) pFiedList->append(query.value( 2+i ).toString());
                   }
                ret = query.value( 0 ).toString();
                break;
               }
           }
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return ret;
}
//-----------------------------------------------------  GetUserPrimKeyFromGUID -------------------------------------------
QString  CMoteurBase::GetUserPrimKeyFromGUID(const QString &guid)
{ //................. Preparer la requete .....................................
  QString ret = "";
  if (OpenBase()==0)  { return QString::null; }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
  requete += m_USER_IDENT_PRIMKEY + " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_GUID + "='" + guid +"'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
     {ret = query.value( 0 ).toString();
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return ret;
}

//-----------------------------------------------------  TypeDoc_To_PrintTypeDoc -------------------------------------------
QString  CMoteurBase::TypeDoc_To_PrintTypeDoc(const QString &typeDoc)
{return QString::number(TypeDoc_To_PrintTypeDoc(typeDoc.toInt()));
}

//-----------------------------------------------------  GetUserPrimKey -------------------------------------------
QString CMoteurBase::GetUserPrimKey(QString user, QString *cripted_password   /* = 0  */)
{if (cripted_password) *cripted_password = "";
 QString   ret="";
 //............... chaines d'erreurs  ..................................................
 if  (OpenBase()==0)  return ret;

 //............... crer un curseur SQL ..................................................
 Q3SqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));                     // ouvrir un curseur sur la table
 if (!cur.select( m_USER_IDENT_LOGIN  + "='" + user + "'" ))  goto GetUserPrimKeyError;

 if (cur.next())
    {ret = cur.value( m_USER_IDENT_PRIMKEY).toString();
     if (cripted_password) *cripted_password = cur.value( m_USER_IDENT_PASSWORD).toString();
     CloseBase();
     return ret;
    }

GetUserPrimKeyError:
   OutSQL_error(cur, "GetUserPrimKey()");
   //CloseBase();
   return ret;
}

//-----------------------------------------------------  SaveUserPassWord --------------------------------------------------
int CMoteurBase::VerifyUserPassWord(QString user, QString password)
{QString  cripted_password;
 if (GetUserPrimKey(user, &cripted_password )   == "")             return FALSE;
 if (password.length()==0 && cripted_password.length()==0)         return TRUE;
 if (cripted_password == password)                                 return TRUE;
 if (PassWordDecode(cripted_password, password) == ERR_PASS_MAX)   return FALSE;
 return TRUE;
}

//-----------------------------------------------------  SaveUserPassWord --------------------------------------------------
int CMoteurBase::SaveUserPassWord(QString user, QString cripted_password)
{//............... chaines d'erreurs  ..................................................
 if  (OpenBase()==0)  return FALSE;

 //............... crer un curseur SQL ..................................................
 Q3SqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));                      // ouvrir un curseur sur la table
 if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}                  // cassos si peut pas etre mis a jour
 if ( !cur.select( m_USER_IDENT_LOGIN  + "='" + user + "'" ))  goto SaveUserPassWordError;

 if (cur.next())
    {QSqlRecord *buffer = cur.primeUpdate();                                   // recuperer le buffer ( pas de delete a la fin !!)
     buffer->setValue( m_USER_IDENT_PASSWORD,   cripted_password);
     cur.update();                                                     // ecrire le buffer avec les donnees
    }
 CloseBase();
   return TRUE;

SaveUserPassWordError:
   OutSQL_error(cur, "SaveUserPassWord()");
   CloseBase();
   return FALSE;
}

//-----------------------------------------------------  GetUserDocument -------------------------------------------
QString CMoteurBase::GetUserDocument(const char*     user,
                                     const char*     nameDocType,
                                     int             type,        // TYP_ORDONNANCE 2  TYP_CERTIFICAT 3 TYP_OBSERVATION 4 TYP_OBSERVATION  5 TYP_PARAM
                                     QString*        pFoot,       // adresse de la chaine oÃ¹ l'on veut recuperer le pied de page
                                     int             sub_type,    // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                     QString*        pk_record)   // = 0;
{QString       ret = "";
 if (pFoot) *pFoot = "";
 //..................... rechercher PK de l'utilisateur ...................
 QString userPk = GetUserPrimKey(user);
 if (userPk == "")   return ret;

 //..................... rechercher PK des donnees ........................
 QString dataPk = IsThisUserDocExist(type, userPk, nameDocType);
 if (pk_record)  *pk_record = dataPk;
 if (dataPk == "")   return ret;
 return GetUserDocument(dataPk, type,  pFoot,  sub_type );
}

//-----------------------------------------------------  GetUserDocument -------------------------------------------
QString CMoteurBase::GetUserDocument(QString         dataPk,
                                     int             type,
                                     QString*        pFoot,       // adresse de la chaine oÃ¹ l'on veut recuperer le pied de page
                                     int             sub_type     // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                     )
{QString       ret = "";
 //..................... rechercher les donnees ...........................
 QString text = GetUserDataFromPrimKey(dataPk);
 if (text == "")     return ret;

 //..................... retirer les bonnes donnees ........................
 //                     a priori on considere que le recto
 if (type == 0)
    {if (text.find("}} /r/n{{") != -1)           type = TYP_ORDONNANCE;
     else if (text.find("}}==/r/n=={{") != -1)   type = TYP_CERTIFICAT;
     else  return text;
    }
 int pos_end;
 int pos_deb ;
 if (type==TYP_PARAM)     return text;
 if (type==TYP_ORDONNANCE)
    {//............... RECTO ....................
     pos_end  = text.find("}} /r/n{{");
     pos_deb  = 2;
     if (pos_end == -1)    return ret;
     ret = text.mid (pos_deb, pos_end-pos_deb );

     pos_deb      = text.find("{{", pos_end);
     if (pos_deb  == -1)   return ret;
     pos_deb     += 2;
     pos_end      = text.find("}} /r/n{{", pos_deb);
     if (pos_end  ==-1)    return ret;
     if (pFoot) *pFoot = text.mid (pos_deb, pos_end-pos_deb );
     //............... DUPLICATA ....................
     if (sub_type)
        {ret = "";
         pos_deb      = text.find("{{", pos_end);
         if (pos_deb  == -1)   return ret;
         pos_deb     += 2;
         pos_end      = text.find("}} /r/n{{", pos_deb);
         if (pos_end  ==-1)    return ret;
         ret = text.mid (pos_deb, pos_end-pos_deb );

         pos_deb      = text.find("{{", pos_end);
         if (pos_deb  == -1)   return ret;
         pos_deb     += 2;
         pos_end      = text.find("}} /r/n", pos_deb);
         if (pos_end  ==-1)    return ret;
         if (pFoot) *pFoot = text.mid (pos_deb, pos_end-pos_deb );
        }
    }
 else
   {pos_end  = text.find("}}==/r/n=={{");            // header
    pos_deb  = 4;
    if (pos_end == -1)    return ret;
    ret = text.mid (pos_deb, pos_end-pos_deb );

    pos_deb      = text.find("=={{", pos_end);       // foot
    if (pos_deb  == -1)   return ret;
    pos_deb     += 4;
    pos_end      = text.find("}}==/r/n", pos_deb);
    if (pos_end  ==-1) return ret;
    if (pFoot) *pFoot = text.mid (pos_deb, pos_end-pos_deb );
   }
 return ret;
}
//-------------------------------------------------- OutSQL_error -----------------------------------------------
QString CMoteurBase::OutSQL_error(const QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/)
{   return OutSQL_error(cur.lastError(), messFunc, requete );
}

//-------------------------------------------------- OutSQL_error -----------------------------------------------
QString CMoteurBase::OutSQL_error(const QSqlError &error, const char *messFunc /*=0*/, const char *requete /*=0*/)
{    QString   qserr("");

     if (error.type() != QSqlError::NoError)
        {switch (error.type())
           { case    QSqlError::NoError:             qserr = tr(" - no SQL error occurred: ")          ; break;
             case    QSqlError::ConnectionError:     qserr = tr(" - SQL connection error: ")           ; break;
             case    QSqlError::StatementError:      qserr = tr(" - SQL statement syntax error: ")     ; break;
             case    QSqlError::TransactionError:    qserr = tr(" - SQL transaction failed; error: ")   ; break;
             case    QSqlError::UnknownError:        qserr = tr(" - unknown SQL error: ")              ; break;
             default:                                qserr = tr(" - unknown SQL error type: ")         ; break;
           }
         if (messFunc) qDebug()<<messFunc;
         qDebug()<< qserr.append(error.databaseText()).toLatin1();
         if (requete) qDebug()<<requete;
        }
     return error.databaseText();
}
//-----------------------------------------------------  GetUserSpecialite -------------------------------------------
/*! \brief Fonction qui recupere la specialite d'un medecin utilisateur
*  \param user     &QString  : referrence sur la QString qui contient le nom de l'utilisateur
*  \param errMess   QString* : (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
*  \return QString   chaine de caracteres du texte d'une eventuelle erreur (vide sinon).
*/
QString CMoteurBase::GetUserSpecialite( QString &user, QString    *errMess /* = 0 */ )
{ if (user.length()==0)  return QString::null;
  //................. Preparer la requete .....................................
  if (OpenBase()==0)    {if (errMess) *errMess = tr("CMoteurBase::GetUserSpecialite(): database can't be opened");       return QString::null; }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString specialite = "";
  QString requete("SELECT ");
  requete       += m_USER_IDENT_QUALITE  +  " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_LOGIN + " = '" + user +"'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())  specialite = Utf8_Query(query, 0 );
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return specialite;
}

  //------------------------------------------ GetUserNomPrenom -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere le nom et prenom a d'un medecin utilisateur a partir de son login
   *  \param user     &QString  : reference sur la QString qui contient le nom de l'utilisateur
   *  \param nom      &QString  : reference sur la QString qui contiendra le nom de l'utilisateur
   *  \param prennom   &QString  : reference sur la QString qui contiendra le prenom de l'utilisateur
   *  \return neant
   */
void  CMoteurBase::GetUserNomPrenom( const QString &user, QString &nom, QString &prenom, QString    *errMess /* = 0 */)
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = tr("CMoteurBase::GetUserNomPrenom(): database can't be opened");       return ; }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
  requete       += m_USER_IDENT_NOM + "," + m_USER_IDENT_PRENOM  +  " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_LOGIN + " = '" + user +"'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())
            {nom    = Utf8_Query(query, 0 );
             prenom = Utf8_Query(query, 1 );
            }
     } //endif (pSqlQuery && pSqlQuery->isActive())
  //OutSQL_error(query, "GetUserNomPrenom()", requete);
  CloseBase();
}

//------------------------------------------ GetUserNomPrenomFromGUID -------------------------------------------------------------------------
/*! \brief Fonction qui recupere le nom et prenom a d'un medecin utilisateur a partir de son login
 *  \param guid      &QString  : reference sur la QString qui contient le guid de l'utilisateur
 *  \param nom       *QString  : pointeur sur la QString qui contiendra le nom de l'utilisateur
 *  \param prennom   *QString  : pointeur sur la QString qui contiendra le prenom de l'utilisateur
 *  \return QString login de l'utilisateur
 */
QString  CMoteurBase::GetUserNomPrenomFromGUID( const QString &guid, QString *nom /* = 0 */, QString *prenom /* = 0 */,  QString    *errMess /* = 0 */)
{ //................. Preparer la requete .....................................
QString user = "";
if (OpenBase()==0)  {if (errMess) *errMess = tr("CMoteurBase::GetUserNomPrenom(): database can't be opened");       return ""; }
//................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
QString requete("SELECT ");
requete       += m_USER_IDENT_NOM + "," + m_USER_IDENT_PRENOM  + "," +  m_USER_IDENT_LOGIN + " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_GUID + " = '" + guid +"'";
QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
if (query.isActive())
   {while (query.next())
          {if (nom)    *nom    = Utf8_Query(query, 0 );
           if (prenom) *prenom = Utf8_Query(query, 1 );
           user                = Utf8_Query(query, 2 );
          }
   } //endif (pSqlQuery && pSqlQuery->isActive())
//OutSQL_error(query, "GetUserNomPrenom()", requete);
CloseBase();
return user;
}
//-----------------------------------------------------  GetUserPermisions -------------------------------------------
QString CMoteurBase::GetUserPermisions( const QString &user, QString    *errMess /* = 0 */ )
/*! \brief Fonction qui recupere la chaine de caracteres des droit utilisateurs
 *  \param user     &QString  : reference sur la QString qui contient le nom de l'utilisateur
 *  \param errMess   QString* : (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
 *  \return          QString qui est la chaine des droits utilisateur (exple '-med-agh-agm-adm' )
*/
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = tr("CMoteurBase::GetUserPermisions(): database can't be opened");       return QString::null; }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString droits = "";
  QString requete("SELECT ");
  requete       += m_USER_IDENT_DROITS  +  " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_LOGIN + " = '" + user +"'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())  droits = Utf8_Query(query, 0 );
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return droits;
}

//-----------------------------------------------------  SetUserPermisions -------------------------------------------
bool CMoteurBase::SetUserPermisions( const QString &primkey, const QString &droits, QString    *errMess /* = 0 */ )
/*! \brief Fonction qui positionne les droits sur un utilisateur
 *  \param primkey  &QString  : reference sur la QString qui contient la clef primaire de l'identit\303\251 de l'utilisateur
 *  \param droits   &QString  : reference sur la QString qui contient les droits de l'utilisateur
 *  \param errMess   QString* : (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
 *  \return          bool  TRUE si tout est ok FALSE dans le cas contraire
 */

{ bool ok = FALSE;
  QString requete            = "";
  requete                   += "UPDATE "                  + m_USER_IDENT_TBL_NAME    + " SET ";
  requete                   +=  m_USER_IDENT_DROITS       + " = '" + droits          + "' WHERE ";
  requete                   +=  m_USER_IDENT_PRIMKEY      + " ='"  + primkey         + "'";
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = tr("CMoteurBase::SetUserPermisions(): database can't be opened");       return ok; }
  QSqlQuery query(QSqlDatabase::database(m_BaseLabel) );
  query.exec(requete);
  CloseBase();
  return ok;
}

//-----------------------------------------------------  UserDroitsCount -------------------------------------------
int CMoteurBase::UserDroitsCount(const QString &droitToFind , QString    *errMess)
{ int nb = 0;
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = tr("CMoteurBase::UserDroitsCount(): database can't be opened");       return -1; }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString droits = "";
  QString requete("SELECT ");
  requete       += m_USER_IDENT_DROITS  +  " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_DROITS + " LIKE '%"+droitToFind+"%'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())  ++nb;
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return nb;
}

  //------------------------------------------ GetUserList -------------------------------------------------------------------------
  /*! \brief Fonction qui filtre, recupere et renseigne une QTreeWidget avec le nom des utilisateurs  filtree par certains criteres
   *  \param pQlistView    *QTreeWidget  : pointeur sur la QTreeWidget a remplir avec le nom des utilisateurs
   *  \param listUserAlreadyPresent       const QStringList& :  reference sur une liste d'utilisateurs deja presents a ne pas retourner
   *  \param specialite    const QString& : (par defaut vide) si contient quelque chose alors filtrer sur la Qualite de l'utilsateur (specialite)
   *  \return QString   chaine de caracteres du texte d'une ventuelle erreur (vide sinon).
   */
QString CMoteurBase::GetUserList(  QTreeWidget *pQlistView, const QStringList &listUserAlreadyPresent, const QString &specialite /* = "" */ )
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  return tr( "CMoteurBase::GetUserList(): database can't be opened" );
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
  requete       += m_USER_IDENT_LOGIN    + "," +            // 0
                   m_USER_IDENT_NOM      + "," +            // 1
                   m_USER_IDENT_PRENOM   + "," +            // 2
                   m_USER_IDENT_PRIMKEY  + "," +            // 3
                   m_USER_IDENT_GUID     + " FROM  " +      // 4
                   m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_DROITS  + " LIKE '%-med%' ";
  if (specialite.length()) requete       += " AND "  + m_USER_IDENT_QUALITE + " = '"  + specialite  + "' ";
  requete       +=  " ORDER BY " + m_USER_IDENT_LOGIN;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {
      while (query.next())
            {QString str = Utf8_Query(query, 0 );
             if (listUserAlreadyPresent.findIndex(str)==-1)
                {QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem( );

                 if (pQTreeWidgetItem)
                    {pQlistView->addTopLevelItem(pQTreeWidgetItem);
                     pQTreeWidgetItem->setText(0, str);
                     pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));
                     pQTreeWidgetItem->setText(2, Utf8_Query(query, 2 ));
                     pQTreeWidgetItem->setText(3, Utf8_Query(query, 3 ));
                     pQTreeWidgetItem->setText(4, Utf8_Query(query, 4 ));
                    }
                }
            }
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return QString::null;
}

  //------------------------------------------ GetUserList -------------------------------------------------------------------------
  /*! \brief Fonction qui filtre, recupere et renseigne une QTreeWidget avec le nom des utilisateurs filtree par certains criteres
   *  \param pQlistView    *QTreeWidget  : pointeur sur la QTreeWidget a remplir avec le nom des utilisateurs
   *  \param qstr_nom       const QString& : (par defaut vide) caracteres pour filtrer les nom des utilisateurs debutant par ces caracteres
   *  \param qstr_prenom    const QString& : (par defaut vide) caracteres pour filtrer le prenom des utilisateurs debutant par ces caracteres
   *  \param childsMustBeFind    BOOL :  (par defaut TRUE) si TRUE remplir les utilisateurs retrouves avec ses utilisateurs de confiance
   *  \param qstr_UserQualite    const QString& : (par defaut vide) si contient quelque chose alors filtrer sur la Qualite de l'utilsateur (specialite)
   *  \param statutMess    const QString* : (par defaut zero) si renseigne (non a zero) il y est retourne le message de statut
   *  \param errMess    const QString* :    (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
   *  \return long : nombre d'utilisateurs.
   */
long CMoteurBase::GetUserList(    QTreeWidget     *pQlistView,
                                  const QString    &qstr_nom,         /* = ""   */
                                  const QString    &qstr_prenom,      /* = ""   */
                                  const bool       childsMustBeFind,  /* = TRUE */
                                  const QString   &qstr_UserQualite,  /* = ""   */
                                        QString   *statutMess,        /* = 0    */
                                        QString   *errMess            /* = 0    */
                             )
{//................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = tr("CMoteurBase::GetUserList(): database can't be opened");
      return 0;
     }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete;
  requete        = "SELECT ";
  requete       += m_USER_IDENT_LOGIN    + "," +      // 0
                   m_USER_IDENT_NOM      + "," +      // 1
                   m_USER_IDENT_PRENOM   + "," +      // 2
                   m_USER_IDENT_PRIMKEY  + "," +      // 3
                   m_USER_IDENT_GUID     + "," +      // 4
                   m_USER_IDENT_DROITS;

  if (qstr_nom != ""||qstr_prenom!="")
     {requete       += " FROM "            + m_USER_IDENT_TBL_NAME        + " WHERE ";
      requete       += m_USER_IDENT_NOM    + " LIKE '"+ qstr_nom          + "%' AND ";
      requete       += m_USER_IDENT_PRENOM + " LIKE '"+ qstr_prenom       + "%' AND ";
      if (qstr_UserQualite.length())
         {requete   += m_USER_IDENT_QUALITE + " = '"  + qstr_UserQualite  + "' AND ";
         }
      requete       += m_USER_IDENT_DROITS + " LIKE '%-med%'";
      requete       += " ORDER BY "        + m_USER_IDENT_NOM;
     }
  //................. SINON pas de filtre ...........................................................
  else
     {requete       += " FROM "      + m_USER_IDENT_TBL_NAME     + " WHERE ";
      requete       += m_USER_IDENT_DROITS + " LIKE '%-med%'";
      if (qstr_UserQualite.length())
         {requete   += " AND " + m_USER_IDENT_QUALITE + " = '"  + qstr_UserQualite  + "' ";
         }
      requete       += " ORDER BY "  + m_USER_IDENT_NOM;
     }

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int nb = 0;
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {QString droits         = query.value( 5 ).toString();
         //............... on rempli la listbox selon son nombre de colonnes .....................
         //if (  /*droits.find("adm")!=-1 ||  (droits.find("med") != -1  ) */)
            {if (childsMustBeFind )
                {if (droits.find("-sgn") != -1)
                    {QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
                     pQlistView->addTopLevelItem(pQTreeWidgetItem);
                     if (pQTreeWidgetItem)
                        {pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));
                         pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));
                         pQTreeWidgetItem->setText(2, Utf8_Query(query, 2 ));
                         pQTreeWidgetItem->setText(3, Utf8_Query(query, 3 ));
                         pQTreeWidgetItem->setText(4, Utf8_Query(query, 4 ));
                         PermsUserChildGetList(pQTreeWidgetItem, pQTreeWidgetItem->text(4));
                         //element->setOpen ( TRUE );
                         ++nb;
                       }
                   } // endif (droits.find("sgn") != -1)
               }
            else
               {QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem( );
                pQlistView->addTopLevelItem(pQTreeWidgetItem);
                if (pQTreeWidgetItem)
                   {pQTreeWidgetItem->setText(0, Utf8_Query(query, 0 ));
                    pQTreeWidgetItem->setText(1, Utf8_Query(query, 1 ));
                    pQTreeWidgetItem->setText(2, Utf8_Query(query, 2 ));
                    pQTreeWidgetItem->setText(3, Utf8_Query(query, 3 ));
                    pQTreeWidgetItem->setText(4, Utf8_Query(query, 4 ));
                    ++nb;
                  }
               } //endif (childsMustBeFind )
           }
          //if (element) pQlistView->insertItem ( element );
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())

  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess) *statutMess = tr("Found: %1").arg(nb);
  CloseBase();
  return nb;
}

//-----------------------------------------------------  GetUserList -------------------------------------------
/*! \brief Fonction qui recupere et renseigne une QStringList avec le nom des utilisateurs
 *  \param qstringList    &QStringList  : reference sur la QStringList �  remplir avec le nom des utilisateurs
 *  \param filterOnDroits &QStringList  : reference sur une chaine contenant les droits dont on veut les utilisateurs (vide si pas utilis\303\251e)
 *  \return QString   retourne une chaine contenant une eventuelle erreur (vide si pas d'erreur)
 */
QString CMoteurBase::GetUserList( QStringList &qstringList, const QString &filterOnDroits /*="-sgn"*/)
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  {return tr("CMoteurBase::GetUserList(): database can't be opened");   }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
  requete       += m_USER_IDENT_LOGIN  +  " FROM " + m_USER_IDENT_TBL_NAME     + " WHERE ";
  if (filterOnDroits.length())
     {QStringList list = filterOnDroits.split("-", QString::SkipEmptyParts);
      for (int i=0; i<list.count(); ++i)
          {if (i) requete   += " AND ";
           requete   += m_USER_IDENT_DROITS  + " LIKE '%-"+list[i]+"%'";
          }
     }
  requete   += " ORDER BY " + m_USER_IDENT_LOGIN;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {qstringList.clear();
      while (query.next())  qstringList.append(Utf8_Query(query, 0 ));
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return QString::null;
}

//-----------------------------------------------------  PermsUserFatherGetQlistView -------------------------------------------
//   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
//   QString      m_USER_IDENT_NOM;                // Nom
//   QString      m_USER_IDENT_PRENOM;             // Prenom

void CMoteurBase::PermsUserFatherGetQlistView( Q3ListView* pQlistView, const QString  &friendUser , const QString  &imagePath , QString    *errMess /* = 0 */)
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue a cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorises avec les droits)
  //QString signGUID   = GetFieldValue( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, signUser);
  QString friendGUID = GetFieldValue( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, friendUser);

  if (OpenBase()==0)  {if (errMess) *errMess = "CMoteurBase::PermsUserFatherGetQlistView(): database can't be opened";       return ; }

  QString requete ="SELECT "  + m_USER_IDENT_LOGIN                + ", "    // 0                                             // 0
                              + m_USER_IDENT_NOM                  + ", "    // 1                                             // 1
                              + m_USER_IDENT_PRENOM               + ", "    // 2
                              + m_USER_IDENT_PRIMKEY              + ", "    // 3
                              + m_USER_IDENT_GUID                 + "  "    // 4
                              + " FROM " + m_USER_IDENT_TBL_NAME  + " WHERE " + m_USER_IDENT_DROITS + " LIKE '%-sgn%' ORDER BY " + m_USER_IDENT_LOGIN;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {while (query.next())
        {QString login = Utf8_Query(query, 0 );
         if (login != friendUser)
            { Q3ListViewItem *pQListViewItem = new Q3ListViewItem(  pQlistView ,
                                                                  Utf8_Query(query, 0 ),         // login
                                                                  Utf8_Query(query, 1 ),         // nom
                                                                  Utf8_Query(query, 2 ),         // prenom
                                                                  "",                            // droits delegues
                                                                  query.value( 3 ).toString(),   // pk
                                                                  query.value( 4 ).toString()    // GUID du permsUser
                                                               );
             if (pQListViewItem)
                {QString requetePermsUser = "SELECT "  + m_USER_PERMS_FRIEND_DROITS  + ", "
                                                       + m_USER_PERMS_PK             + " FROM " + m_USER_PERMS_TBL_NAME       + " WHERE  "
                                                       + m_USER_PERMS_SIGN_GUID      + " = '"   + query.value( 4 ).toString() + "' AND "
                                                       + m_USER_PERMS_FRIEND_GUID    + " = '"   + friendGUID                  + "'";
                 QSqlQuery queryPermsUser(requetePermsUser , QSqlDatabase::database(m_BaseLabel) );
                 if (queryPermsUser.isActive() && queryPermsUser.next())
                    {QString droits = queryPermsUser.value( 0 ).toString();
                     droits = droits.remove("-sgn");
                     droits = droits.remove("-med");
                     pQListViewItem->setText(3, droits );          //  droits delegues
                     pQListViewItem->setText(6, queryPermsUser.value( 1 ).toString() );          //  pk de la table des permissions
                     pQListViewItem->setPixmap (0, QPixmap(imagePath+"CheckBoxOn.png") ) ;
                    }
                else
                    {pQListViewItem->setText(3, "" );                                            //  droits delegues
                     pQListViewItem->setText(6, "" );                                            //  pk de la table des permissions
                     pQListViewItem->setPixmap (0, QPixmap(imagePath+"CheckBoxOff.png") ) ;
                    }
               } // endif (pQListViewItem)
           } // endif (login != friendUser)
       }  //endwhile (query.next())
    } //endif (query.isActive())
  CloseBase();
}
  //------------------------------------------ PermsUserChildGetList -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere et renseigne une QTreeWidgetItem avec le nom des utilisateurs delegues d'un utilisateur principal
   *  \param pQlistViewItem    QTreeWidgetItem*  : item dans lequel il faut rajouter les utilisateurs delegues
   *  \param signGUID          const QString& : GUID de l'utilisateur principal dont il faut retrouver les utilisteurs deleges
   *  \return neant
   */
void CMoteurBase::PermsUserChildGetList(  QTreeWidgetItem *pQlistViewItem,  const QString    &signGUID )
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue a cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorises avec les droits)

  QString requete = "SELECT "  + m_USER_PERMS_FRIEND_GUID       + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_SIGN_GUID         + " = '"   + signGUID              + "'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //Param_UpdateToDisk("/home/ro/TestSql.txt", requete);
  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {
      while (query.next())
        {QString requetePermsUser = "SELECT "  + m_USER_IDENT_LOGIN                + ", "    // 0                                             // 0
                                               + m_USER_IDENT_NOM                  + ", "    // 1                                             // 1
                                               + m_USER_IDENT_PRENOM               + ", "    // 2
                                               + m_USER_IDENT_PRIMKEY              + ", "    // 3
                                               + m_USER_IDENT_GUID                 + "  "    // 4
                                               + " FROM " + m_USER_IDENT_TBL_NAME  + " WHERE  " + m_USER_IDENT_GUID + " = '"   + Utf8_Query(query, 0 ) + "'";              // m_USER_PERMS_FRIEND_GUID
         QSqlQuery queryPermsUser(requetePermsUser , QSqlDatabase::database(m_BaseLabel) );
          if (queryPermsUser.isActive())
            {
             while (queryPermsUser.next())
                   { QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
                     if (pQTreeWidgetItem)
                        {pQTreeWidgetItem->setText(0, Utf8_Query(queryPermsUser, 0 ));       // login
                         pQTreeWidgetItem->setText(1, Utf8_Query(queryPermsUser, 1 ));       // nom
                         pQTreeWidgetItem->setText(2, Utf8_Query(queryPermsUser, 2 ));       // prenom
                         pQTreeWidgetItem->setText(3, Utf8_Query(queryPermsUser, 3 ));       // PK
                         pQTreeWidgetItem->setText(4, Utf8_Query(queryPermsUser, 4 ));       // GUID
                         pQlistViewItem->addChild(pQTreeWidgetItem);
                        }
                  }  //endwhile (queryPermsUser.next())
           } //endif (queryPermsUser.isActive())
        } //end while (pSqlQuery->next())4
     } //endif (pSqlQuery && pSqlQuery->isActive())
}

//-----------------------------------------------------  PermsUserIsThisUserFriendFromName -------------------------------------------
// ACTION : retourne les droits autorises par signUser pour friendUser
//
QString CMoteurBase::PermsUserIsThisUserFriendFromName(   const QString  &signUser, const QString  &friendUser)
{
 QString signGUID   = GetFieldValue( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, signUser);
 QString friendGUID = GetFieldValue( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, friendUser);
 return PermsUserIsThisUserFriendFromGUID(   signGUID, friendGUID);
}

//-----------------------------------------------------  PermsUserSignUserListForThisUser -------------------------------------------
// ACTION : retourne la liste :userList des utilisateurs signataires ayant autorise : user pour le droit : pDroit
//
int CMoteurBase::PermsUserSignUserListForThisUser(   const QString  &user, QStringList  &userList,  const char  *pDroit  /* = 0 */)
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue a cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorises avec les droits)
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  { return 0; }

  QString userGUID   = GetFieldValue_WithoutOpenCloseBase( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, user);

  QString requete = "SELECT "  + m_USER_PERMS_SIGN_GUID + ", "    +  m_USER_PERMS_FRIEND_DROITS   + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_FRIEND_GUID         + " = '"                        + userGUID                         + "' ;";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //.................. si la requete a un resultat ..............................................
  QString sign_guid       ="";
  QString friend_droits   ="";
  QString sign_name       ="";
  if (query.isActive())
     {userList.clear();
      while (query.next())
        { sign_guid          = query.value(0).toString();
          sign_name          = GetFieldValue_WithoutOpenCloseBase( m_USER_IDENT_TBL_NAME, m_USER_IDENT_LOGIN, m_USER_IDENT_GUID, sign_guid);
          if (pDroit)
             {friend_droits  = query.value(1).toString();
              if (IsThisDroitExist(friend_droits, pDroit)) userList.append(sign_name);
             }
          else
             {userList.append(sign_name);
             }
        } //end while (pSqlQuery->next())4
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return userList.count();
}

//--------------------------------------------- IsThisDroitExist -------------------------------------------------------------------
// ACTION verifie si le droit : droitToFind  (trois caracteres style med adm sgn ....) existe dans la chaine de droits : listDroits
//        6 fois plus rapide que : listDroits.find(droitToFind) != -1
bool CMoteurBase::IsThisDroitExist(const char *listDroits, const char *droitToFind)
{if (listDroits==0) return FALSE;
 char *pt  = (char*)listDroits;
 while (*pt)
    {if (pt[1]==droitToFind[0] && pt[2]==droitToFind[1] &&pt[3]==droitToFind[2]) return TRUE;
     pt += 4;
    }
 return FALSE;
}
//-----------------------------------------------------  PermsUserIsThisUserFriendFromGUID -------------------------------------------
// ACTION : retourne les droits autorises par signGUID pour friendGUID
//
QString CMoteurBase::PermsUserIsThisUserFriendFromGUID(   const QString  &signGUID, const QString  &friendGUID)
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue a cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorises avec les droits)
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  { return NULL; }
  QString droits  = "";
  QString requete = "SELECT "  + m_USER_PERMS_FRIEND_DROITS       + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_SIGN_GUID           + " = '"   + signGUID              + "' AND  "
                               + m_USER_PERMS_FRIEND_GUID         + " = '"   + friendGUID            + "';";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {while (query.next())
        {droits = Utf8_Query(query, 0 );
        } //end while (pSqlQuery->next())4
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return droits;
}

//-----------------------------------------------------  GetUserListType -------------------------------------------
  /*! \brief Fonction qui recupere et renseigne une QStringList avec le nom des specialites
   *  \param typList    &QStringList  : referrence sur la QStringList a remplir avec le nom des specialites
   *  \param errMess         QString* : (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
   *  \return neant
   */
void CMoteurBase::GetUserListType( QStringList &typList , QString    *errMess /* = 0 */ )
{//................. Preparer la requete .....................................
  if (OpenBase()==0) {if (errMess) *errMess = "CMoteurBase::GetUserList(): database can't be opened"; return ; }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString        requete;
  requete        = "SELECT ";
  requete       += m_USER_IDENT_QUALITE ; // 0
  requete       += " FROM "      + m_USER_IDENT_TBL_NAME     + " WHERE ";
  requete       += m_USER_IDENT_DROITS + " LIKE '%-med%'";
  requete       += " ORDER BY "  + m_USER_IDENT_QUALITE;

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {typList.clear();
      while (query.next())
        {QString str = Utf8_Query(query, 0 );
         if (typList.findIndex ( str)==-1) typList.append(str);
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
}


//-----------------------------------------------------  InitComboWithUserDocList -------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
//   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
//   QString      m_USER_DATA_TYPE;               // type de donnees
//   QString      m_USER_DATA_BLOB;               // donnees proprement dites
//   ACTION: cette fonction rempli la combo box de choix du type de masque d'impression a utiliser
/*
long CMoteurBase::InitComboWithUserDocList( Q3ComboBox       *pQ3ComboBox,
                                            QString         user,
                                            QString       rubName,
                                            QString         type1, // = QString::null  ou RUBNAME alors type2 contient le nom de la rubrique
                                            QString         type2, // = QString::null  ou nom de la rubrique
                                            int clear              // = 1
                                          )
{ //................. tester l'utilisateur .....................................
  QString primKey = GetUserPrimKey(user);
  bool       oldSyntax = TRUE;
  if (primKey=="") return 0;

  //................. tester disponibilite des bases .....................................
  if (OpenBase()==0)  return 0;

  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
          requete       += m_USER_DATA_NAMEDOC +","+
                           m_USER_DATA_TYPE    +","+
                           m_USER_DATA_PK;
          requete       += " FROM "       + m_USER_DATA_TBL_NAME       + " WHERE ";
  //................. forme particuliere des nouvelles rubriques a venir oÃ¹ ..................................
  //                  le libelle du document d'entete et de pied de page doit Ãªtre
  //                  de la forme : [NOM DE LA RUBRIQUE] LIBELLE DE LA RUBRIQUE (avec le # ou * et eventuels prefixages)
  if (type1.left(7) == "RUBNAME")
     {oldSyntax      = FALSE;
      requete       += m_USER_DATA_REF_PK     + " = '"   + primKey  + "' ";
      requete       += " ORDER BY "           + m_USER_DATA_NAMEDOC;
     }
  else if (type1 != QString::null && type2 != QString::null)
     {requete       += m_USER_DATA_REF_PK     + " =  '"   + primKey         + "' AND ";
      requete       += m_USER_DATA_TYPE       + " >= '"   + type1           + "' AND ";
      requete       += m_USER_DATA_TYPE       + " <= '"   + type2           + "'    ";
      requete       += " ORDER BY "           + m_USER_DATA_NAMEDOC;
     }
  else if (type1 != QString::null && type2 == QString::null)
     {requete       += m_USER_DATA_REF_PK     + " =  '"   + primKey         + "' AND ";
      requete       += m_USER_DATA_TYPE       + " = '"    + type1           + "' ";
      requete       += " ORDER BY "           + m_USER_DATA_NAMEDOC;
     }
  //................. SINON pas de filtre ...........................................................
  else
     {requete       += m_USER_DATA_REF_PK     + " = '"   + primKey  + "' ";
      requete       += " ORDER BY "           + m_USER_DATA_NAMEDOC;
     }

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int nb   = 0;
  int pos  = -1;
  QString text;
  QString type_doc;
  QString rubNameInTitle;
  QString subTypeInTitle;
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {if (clear) pQ3ComboBox->clear();
      while (query.next())
        {text = Utf8_Query(query, 0 );        // libelle du document d'entete
         if (text[0] == '[' )          //>>>>>>>............... NOUVELLE SYNTAXE : [NOM DE LA RUBRIQUE @ sous type ] .....................
            {rubNameInTitle          = "";
             subTypeInTitle          = "ComboDef SubType";
             int deb_rubNameInTitle  = text.find('[');
             int end_rubNameInTitle  = text.find(']');
             if (deb_rubNameInTitle != -1 && end_rubNameInTitle != -1)
                {pos = text.find('@');
                 ++deb_rubNameInTitle;         // passer le premier crochet
                 if (pos != -1)                // si '@' c'est que le nom de la rubrique est suivi du sous type
                    {rubNameInTitle = text.mid(deb_rubNameInTitle, pos - deb_rubNameInTitle).stripWhiteSpace();
                     ++pos;                    // passer '@'
                     subTypeInTitle = text.mid(pos, end_rubNameInTitle - pos).stripWhiteSpace();
                    }
                 else
                    {rubNameInTitle = text.mid(deb_rubNameInTitle, end_rubNameInTitle - deb_rubNameInTitle).stripWhiteSpace();
                    }
                 text = text.mid(end_rubNameInTitle+1).stripWhiteSpace();
                } // endif (deb_rubNameInTitle != -1 && end_rubNameInTitle != -1)
             } // endif ( text[0] == '[' )
          else if ( oldSyntax )                      //>>>>>>>.... la on est sur l'ancien systeme donc ne filtrer qu'avec la requete SQL .......................
             {rubNameInTitle = rubName;
              subTypeInTitle = "ComboDef SubType";
             }
          if ( rubNameInTitle == rubName)   // si on trouve le nom de la rubrique [NOM DE LA RUBRIQUE]
             {new CPrtQListBoxItem(pQ3ComboBox->listBox() ,                                 // list box du combobox
                                          text ,      // libelle
                                          Utf8_Query(query, 2 ) ,          // Pk document
                                          Utf8_Query(query, 1 ) ,          // type document
                                          user,                            // user
                                          subTypeInTitle );             // sous type (non utilise ici)
                     nb++;

             }
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return nb;
}
*/
//-----------------------------------------------------  GetUserDocList -------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
//   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
//   QString      m_USER_DATA_TYPE;               // type de donnees
//   QString      m_USER_DATA_BLOB;               // donnees proprement dites

long CMoteurBase::GetUserDocList( Q3ListView       *pQlistView,
                                  const char      *primKey,
                                  const char      *qstr_nom,  /* = 0 */
                                  QLabel    *statutMess,      /* = 0  */
                                  QLabel    *errMess          /* = 0  */
                                 )
{//................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::GetUserDocList(): database can't be opened");
      return 0;
     }
  //................. SI un nom ou prenom est donne en entree alors filtrer selon ...................
  QString requete("SELECT ");
          requete       += m_USER_DATA_NAMEDOC + ","+
                           m_USER_DATA_TYPE    + ","+
                           m_USER_DATA_PK;
          requete       += " FROM "            + m_USER_DATA_TBL_NAME      + " WHERE ";

  if (qstr_nom && qstr_nom[0])
     {requete       += m_USER_DATA_NAMEDOC    + " LIKE '"+ qstr_nom        + "%' AND ";
      requete       += m_USER_DATA_REF_PK     + " = '"   + primKey         + "' ";
      requete       += " ORDER BY "           + m_USER_DATA_NAMEDOC;
     }
  //................. SINON pas de filtre ...........................................................
  else
     {requete       += m_USER_DATA_REF_PK     + " = '"   + primKey  + "' ";
      requete       += " ORDER BY "           + m_USER_DATA_NAMEDOC;
     }
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int nb = 0;
  //.................. si la requete a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {//............... on rempli la listbox selon son nombre de colonnes .....................
         new Q3ListViewItem( pQlistView,
                            Utf8_Query(query, 0 ),   ////Utf8_Query(query,0);
                            Utf8_Query(query, 1 ),
                            Utf8_Query(query, 2 )
                          );
          nb++;
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { statutMess->setText(tr("Found: ") + QString::number(nb));
     }
  CloseBase();
  return nb;
}

//-------------------------------------- CreateNewUser ----------------------------------------------------------------------------
// ACTION: cree un nouvel utilisateur et retourne son GUID
//m_pCMoteurBase->CreateNewUser(m_pCps->m_Nom, m_pCps->m_Prenom, user, "med", pass, m_pCps->m_NIR);

QString CMoteurBase::CreateNewUser(const QString &nom,
                                   const QString &prenom,
                                   const QString &login,
                                   const QString &droit /*="med" */,
                                   const QString &pass/*=""*/,
                                   const QString &nir/*=""*/ )
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  QString ok("");
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return ok;
  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canInsert ())                                        return ok;
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  QString       guid = GUID_Create();
  buffer->setValue( m_USER_IDENT_GUID,   guid);                   // y placer les donnees
  buffer->setValue( m_USER_IDENT_NOM,    nom);                    // y placer les donnees
  buffer->setValue( m_USER_IDENT_PRENOM, prenom);                 // y placer les donnees
  buffer->setValue( m_USER_IDENT_LOGIN,  login);                  // y placer les donnees
  if (droit.length())buffer->setValue( m_USER_IDENT_DROITS,     droit);                  // y placer les donnees
  if (pass.length())
     {QString pass_cripted = pass;
      PassWordEncode(pass_cripted);
      buffer->setValue(  m_USER_IDENT_PASSWORD, pass_cripted);                   // y placer les donnees
     }
  if (nir.length())buffer->setValue(   m_USER_IDENT_NUM_ORDRE,  nir);                    // y placer les donnees


  if (cur.insert()) ok = guid;                                    // ecrire le buffer

  if (ok=="") OutSQL_error(cur, "CreateNewUser()");

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}
//---------------------------------------------------- IsThisUserDocExist -----------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
//   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
//   QString      m_USER_DATA_TYPE;               // type de donnees
//   QString      m_USER_DATA_BLOB;               // donnees proprement dites
// RETOUR:    la primKey des donnees si elles existent sinon ""
QString CMoteurBase::IsThisUserDocExist(  int              type,
                                          const char      *refUserPrimKey,
                                          const char      *nom,
                                          QLabel          *errMess          /* = 0  */
                                       )
{ QString ret("");
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::IsThisUserDocExist(): database can't be opened");
      return 0;
     }
  //................. filtrer selon donnee d'entree ..................................................
  QString  requete("SELECT " );
           requete       += m_USER_DATA_PK;
           requete       += " FROM "               + m_USER_DATA_TBL_NAME              + " WHERE ";
  if (nom) requete       += m_USER_DATA_NAMEDOC    + " = '"   + nom                    + "' AND ";
           requete       += m_USER_DATA_REF_PK     + " = '"   + refUserPrimKey         + "' AND ";
           requete       += m_USER_DATA_TYPE       + " = '"   + QString::number(type)  + "'";

  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );

  //.................. si la requete a un resultat ..............................................

  if (query.isActive() && query.next())
     {ret = query.value(0).toString();
     }
  //if (ret.length()<=0) ret = IsThisUserDocExist( type, refUserPrimKey, QString (nom).utf8 (), errMess ); // essayer en utf8
  CloseBase();
  return ret;
}

//---------------------------------------------------- IsThisUserDocExist -----------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
//   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
//   QString      m_USER_DATA_TYPE;               // type de donnees
//   QString      m_USER_DATA_BLOB;               // donnees proprement dites
// RETOUR:    la primKey des donnees si elles existent sinon ""

QString CMoteurBase::User_GetBestHeadFoot_ToPrintDoc( RUBREC_LIST::iterator it,
                                                      const QString &rubName,
                                                      const char      *refUserPrimKey,
                                                      QLabel          *errMess          /* = 0  */
                                                    )
{ QString ret("");
  QString defaultRet(""); // retiendra le document du type '* Nom de la rubrique' exple : '* Documents' '* Prescription'
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::IsThisUserDocExist(): database can't be opened");
      return 0;
     }
  //................. filtrer selon donnee d'entree ..................................................
  QString  requete("SELECT " );
           requete       += m_USER_DATA_PK   + "," + m_USER_DATA_NAMEDOC         + "," + m_USER_DATA_TYPE;
           requete       += " FROM "               + m_USER_DATA_TBL_NAME              + " WHERE ";
           requete       += m_USER_DATA_REF_PK     + " = '"   + refUserPrimKey         + "' ";


  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  //.................. si la requete a un resultat ..............................................
  QString type_modele      = "";
  QString libelle_modele   = "";
  QString type_document    = TypeDoc_To_PrintTypeDoc(  (*it).m_Type );

  QString libelle_document = (*it).m_Libelle.lower();
  if (query.isActive())               //Utf8_Query(query,0);
     {
      while (query.next())
             {libelle_modele = Utf8_Query(query,1);        // nom du document utilisateur type
              type_modele    = Utf8_Query(query,2);
              if (type_modele == type_document)
                 {ret = Utf8_Query(query,0);
                  break;
                 }
              else if ( libelle_modele.at(0)=='#')
                 {libelle_modele = libelle_modele.remove('#');;            // se placer apres le '# '
                  libelle_modele = libelle_modele.stripWhiteSpace().lower();
                  if (libelle_document.find(libelle_modele) != -1)           // si on trouve le nom du modele utilisateur dans le
                     {ret = Utf8_Query(query,0);                             // nom du document de la liste patient
                      break;
                     }
                 }
              else if ( libelle_modele.at(0)=='*')
                 {libelle_modele = libelle_modele.remove('*');
                  libelle_modele = libelle_modele.stripWhiteSpace().lower();
                  if (rubName == libelle_modele)           // si on trouve le nom du modele utilisateur dans le
                     {defaultRet = Utf8_Query(query,0);                             // nom du document de la liste patient
                     }
                 }
             }
     }
  CloseBase();
  if (ret.length()==0) return defaultRet;
  return ret;
}


//------------------------------------------ Param_SavParam ---------------------------------------------------------------
void CMoteurBase::Param_SavParam(QString *pParam , QString user)
{   //......................... sauver les parametres dans donnees utilisateur ....................
    //if (pParam==0) pParam = &m_ParamData;
    QString us_pk         = "";
    if (user[0]=='%') us_pk  = user.mid(1);
    else              us_pk  = GetUserPrimKey(user);
    QString docPrimKey       = IsThisUserDocExist(TYP_PARAM, us_pk, "Parametres");
     if (docPrimKey!="")
        {UpdateUserDataFromPrimKey(docPrimKey, TYP_PARAM, us_pk, "Parametres", *pParam);
        }
     else
        {CreateUserDataFromPrimKey(TYP_PARAM, us_pk ,"Parametres", *pParam);
        }
}

//-------------------------------------- UpdateUserDataFromPrimKey ----------------------------------------------------------------------------
int CMoteurBase::UpdateUserDataFromPrimKey( const char *primKey, int type, const char *refUserPrimKey , const char* name,  const char* data)
{
 //................................. methode QSqlCursor .................................
 //                         ne pose pas de PB avec valeur avec une apostrophe
 //............... crer un curseur SQL ..................................................
 if (OpenBase()==0)  return FALSE;
 int ok = FALSE;
 Q3SqlCursor cur(m_USER_DATA_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
 if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
 //............... placer ce curseur sur le bon enregistrement ..........................
 cur.select(m_USER_DATA_PK  + "='" + primKey  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                                                 // recuperer le buffer ( pas de delete a la fin !!)
     if (refUserPrimKey) buffer->setValue( m_USER_DATA_REF_PK,    refUserPrimKey         );  // y placer les donnees
     if (name)           buffer->setValue( m_USER_DATA_NAMEDOC,   name                   );  // y placer les donnees
     if (type)           buffer->setValue( m_USER_DATA_TYPE,      type                   );  // y placer les donnees
     buffer->setValue( m_USER_DATA_BLOB,      data                   );  // y placer les donnees
     ok    = cur.update();                                               // ecrire le buffer avec les donnees du header
    }

 if (ok==0) OutSQL_error(cur, "UpdateUserDataFromPrimKey()");

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//------------------------------------------ ReadDrTuxUserParametres ---------------------------------------------------------------
QString  CMoteurBase::ReadDrTuxUserParametres(const QString &user)
{return GetUserDocument(user, "Parametres", TYP_PARAM );
}

//------------------------------------------ Param_GetParam ---------------------------------------------------------------
void  CMoteurBase::Param_GetParam( QString *pParam, QString user)
{//if (pParam==0) pParam = &m_ParamData;
 *pParam = ReadDrTuxUserParametres(user);
}

//-------------------------------------- DeleteUserDataFromPrimKey ----------------------------------------------------------------------------
int CMoteurBase::DeleteUserDataFromPrimKey( const char *primKey)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_USER_DATA_TBL_NAME + " WHERE ";
          requete += m_USER_DATA_PK       + "= '" + primKey                  + "' ";
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}
//-------------------------------------- DeleteUserDataFromRefUserPrimKey ----------------------------------------------------------------------------
int CMoteurBase::DeleteUserDataFromRefUserPrimKey( const char *refUserPrimKey)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_USER_DATA_TBL_NAME     + " WHERE ";
          requete += m_USER_DATA_REF_PK       + "= '" + refUserPrimKey                  + "' ";
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}
//-------------------------------------- CreateUserDataFromPrimKey ----------------------------------------------------------------------------
int CMoteurBase::CreateUserDataFromPrimKey(int type, const char *primKey , const char* name,  const char* data)
{//  "USER_DATA_TBL_NAME        'UtilisateursDocuments'        \r\n"      // nom de la table des donnees des RUBRIQUES
 //  "USER_DATA_REF_PK          'ID_PrimKeyFromUser'      '0' \r\n"       // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
 //  "USER_DATA_NAMEDOC         'NameDoc'                 '1' \r\n"       // nom des donnees
 //  "USER_DATA_TYPE            'TypeDocuments'           '2' \r\n"       // type de donnees
 //  "USER_DATA_BLOB            'DataDocuments'           '3' \r\n";      // donnees proprement dites
 //   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
 //   QString      m_USER_DATA_REF_PK;             // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
 //   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
 //   QString      m_USER_DATA_TYPE;               // type de donnees
 //   QString      m_USER_DATA_BLOB;               // donnees proprement dites

  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_USER_DATA_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  buffer->setValue( m_USER_DATA_REF_PK,         primKey );  // y placer les donnees
  buffer->setValue( m_USER_DATA_NAMEDOC,        name    );  // y placer les donnees
  buffer->setValue( m_USER_DATA_TYPE,           type    );  // y placer les donnees
  buffer->setValue( m_USER_DATA_BLOB,           data    );  // y placer les donnees
  ok = cur.insert();                                               // ecrire le buffer avec les donnees du header

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//-------------------------------------- DeleteUserFromPrimKey ----------------------------------------------------------------------------
int CMoteurBase::DeleteUserFromPrimKey(const char *refUserPrimKey )
{ //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QSqlQuery* pSqlQuery;
  QString requete;
  int ok  = FALSE;
  //................................. effacer tous les documents lies a l'utilisateur.....................................
          requete  = "DELETE FROM ";
          requete += m_USER_DATA_TBL_NAME + " WHERE ";
          requete += m_USER_DATA_REF_PK       + "= '" + refUserPrimKey + "' ";
 pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 if (ok==0)             {CloseBase();  return 0;}  // si erreur on arrete la

 //................................. effacer l'utilisateur .....................................
          requete  = "DELETE FROM ";
          requete += m_USER_IDENT_TBL_NAME      + " WHERE ";
          requete += m_USER_IDENT_PRIMKEY       + "= '" + refUserPrimKey + "' ";
 pSqlQuery = new QSqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//-------------------------------------- GetUserDataFromPrimKey ----------------------------------------------------------------------------
QString CMoteurBase::GetUserDataFromPrimKey(const char *primKey)
{//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
 //   QString      m_USER_DATA_PK;                 // clef primaire des donnees
 //   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
 //   QString      m_USER_DATA_TYPE;               // type de donnees
 //   QString      m_USER_DATA_BLOB;               // donnees proprement dites

 QString requete("SELECT ");
 requete  += m_USER_DATA_BLOB      + " FROM "      + m_USER_DATA_TBL_NAME  + " WHERE ";
 requete  += m_USER_DATA_PK        + " ='"         + primKey + "'";


 if (OpenBase()==0)  return 0;

 QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
 requete = "";
 if (query.isActive() && query.next())
    {requete = Utf8_Query(query,0);
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 CloseBase();
 return requete;
}

//-------------------------------------- GetUserIdentiteFromPrimKey ----------------------------------------------------------------------------
// ACTION: selectionne l'enregistrement de la table: DOSS_IDENT  correspondant au bon primary key
// RETOUR: QSqlQuery*  zero si echec
//                     valide sinon et AUQUEL CAS IL FAUDRA DETRUIRE ce pointeur par: delete  pSqlQuery
//                                  et AUQUEL CAS IL FAUDRA FERMER la base par:       CloseBase()
QSqlQuery*   CMoteurBase::GetUserIdentiteFromPrimKey(const char *primKey , QStringList &fiedList )
{QString requete("SELECT ");
 //................... ajouter les champs a selectionner .......................
 for ( QStringList::Iterator it = fiedList.begin(); it != fiedList.end(); ++it )
     {requete  += *(it) + ",";
     }
 //.............. virer la derniere virgule ....................................
 int pos = requete.findRev(',');
 if (pos !=-1) requete.remove (pos, 1);
 //............... continuer a construire la requete ...........................
 requete  += " FROM "     + m_USER_IDENT_TBL_NAME + " WHERE ";
 requete  += m_USER_IDENT_PRIMKEY + " ='"         + primKey + "'";

 if (OpenBase()==0)
     {return 0;
     }
 QSqlQuery* pSqlQuery = new QSqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
    {return pSqlQuery;
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return 0;
}


//----------------------------------- ChangeRubUserName ---------------------------------------------------------
// ACTION: change le nom d'utilisateur d'une rubrique.
//
int CMoteurBase::ChangeRubUserName(QString userName, QString rubPrimKey, QString *errMess /*=0 */)
{QString qstr;
 QString requete            = "";
 requete                   += "UPDATE "                  + m_DOSS_RUB_HEAD_TBL_NAME   + " SET ";
 requete                   +=  m_DOSS_RUB_HEAD_USER      + " = '" + userName          + "' WHERE ";
 requete                   +=  m_DOSS_RUB_HEAD_PRIMKEY   + " ='"  + rubPrimKey        + "'";
 if  (OpenBase()==0)
     {return 0;
     }
 QSqlQuery* pSqlQueryTarget = new QSqlQuery(requete , QSqlDatabase::database(m_BaseLabel)  );

 if (pSqlQueryTarget && pSqlQueryTarget ->isActive())
    {if (errMess) *errMess = tr("Success when changing user");
    }
 else
   {qstr   = "";
    qstr  += requete;
    qstr  += tr("\nFailure when changing user: CMoteur_Base. ");
    switch(pSqlQueryTarget->lastError().type())
          {case QSqlError::None :        qstr  += tr("no error occurred");          break;
           case QSqlError::Connection :  qstr  += tr("connection error");           break;
           case QSqlError::Statement :   qstr  += tr("SQL statement syntax error"); break;
           case QSqlError::Transaction : qstr  += tr("transaction failed; error");   break;
           case QSqlError::Unknown :     qstr  += tr("unknown error");              break;
          }
   if (errMess) *errMess = qstr;
   return FALSE;
  }
if (pSqlQueryTarget) delete  pSqlQueryTarget;
return TRUE;
}
//********************************************************************************************************************************
//=================================== GESTION DES IDENTITES PATIENT ==============================================================
//********************************************************************************************************************************



//-------------------------------------- DossIdentIndexCreate --------------------------------------------------------------------
// ACTION: cree un nouveau dosier: en retour le PrimaryKey de ce dossier
QString  CMoteurBase::DossIdentIndexCreate(const QString& nom, const QString& prenom,   const QString& numGUID, PtrListIdentFieldData *plist_field)
{   //....................ne pose pas de PB avec valeur avec une apostrophe ................
    QString        requete            = "";
    QString        requeteIsGUIDExist = "";
    QString                  pkIndex  = "";
    int                            i  = 0;
    int                     nbFields  = plist_field->size();
    CIdentFieldData *pCIdentFieldData = 0;
    //............... ouvrir la base .......................................................
    if (OpenBase()==0)      return  "";
    //................ verifier si ce GUID n'existe pas deja ...............................
    requeteIsGUIDExist  = "SELECT ";
    requeteIsGUIDExist += m_DOSS_INDEX_PRIM_KEY                + " FROM  "      // 0 pk
                       +  m_DOSS_INDEX_TBL_NAME                + " WHERE "
                       +  m_DOSS_INDEX_GUID                    + " = '" + numGUID + "'";
    QSqlQuery query (requeteIsGUIDExist, QSqlDatabase::database(m_BaseLabel));
    //................ si n'existe pas le creer...............................
    if ( ! (query.isActive() && query.next()) )
       { //................ creer l'index ...............................
         requete  = "INSERT INTO ";
         requete +=  m_DOSS_INDEX_TBL_NAME     + " ("
                   + m_DOSS_INDEX_GUID         + ","          // 0
                   + m_DOSS_INDEX_NOM          + ","          // 1
                   + m_DOSS_INDEX_PRENOM       + ") VALUES "; // 2
         requete +=  "('" + numGUID                                 + "', '";       //    0
         requete +=         nom.upper().replace("'","\\'")          + "', '";       //    1
         requete +=         prenom.upper().replace("'","\\'")       + "')  ";       //    2
         //.................. Executer la requete ...............................................
         query.exec (requete);
         OutSQL_error( query, "DossIndexCreate()", requete.toAscii() );
       }
    else
       { qDebug()<<tr("DossIdentIndexCreate() this GUID '%1' already exists \n").arg(requeteIsGUIDExist);
       }

    //................... recuperer le pk de l'index cree ou deja existant.................................
    query.exec (requeteIsGUIDExist);
    if (query.isActive() && query.next())    // si existe ok on releve la valeur
       {pkIndex = query.value(0).toString();
       }
    if (pkIndex.length()==0)
       {qDebug()<<tr("DossIdentIndexCreate() pk not found for this GUID '%1' , index not created \n%2").arg(numGUID, requete);
        CloseBase();
        return "";
       }
    //............... a ce stade on est sur que l'index nom premnom existe pour ce guid .............................
    //                verifier si une identite existe pour cet index nom prenom
    QString ident_pk =  GetFieldValue( m_DOSS_IDENT_TBL_NAME,      // table
                                       m_DOSS_IDENT_REF_PK,        // field to get
                                       m_DOSS_IDENT_GUID,          // field to compare
                                       numGUID                     // with value to compare
                                     );
    //................. si une identite existe ne pas la recreer .................................................
    if (ident_pk.length())
       { qDebug()<<tr("DossIdentIndexCreate() a pfcPat record already exists for this GUID '%1' , record will be not created").arg(numGUID);
         CloseBase();
         return  "";
       }
    //.............. ici on est sur que l'index est cree, et la  fiche fchPat n'existe pas  .................
    //               maintenant creer la fiche fchPat
    requete  = "INSERT INTO ";
    requete +=  m_DOSS_IDENT_TBL_NAME     + " ("
            +   m_DOSS_IDENT_REF_PK       + ","         // 0
            +   m_DOSS_IDENT_GUID         + ",";        // 1
    i  = 0;
    while ( i < nbFields )                 // pour tous les champs
        {requete += plist_field->at(i)->m_Name;
         if (++i < nbFields) requete +=        + ",";
         else                requete +=        + ") VALUES ('";
        }
    requete +=         pkIndex                 + "', '";       //    0
    requete +=         numGUID                 + "', '";       //    1

    i  = 0;
    while ( i < nbFields )                 // pour tous les champs
        {pCIdentFieldData = plist_field->at(i);
         switch(pCIdentFieldData->m_Type)
              { case FTN_MAJ_STRING: {requete  += pCIdentFieldData->m_Value.upper().replace("'","\\'");} break;
                case FTN_INTEGER:    {requete  += QString::number(pCIdentFieldData->m_Value.toUInt());}  break;
                case FTN_NUM_STRING:
                case FTN_STRING:
                case FTN_CHAR:       {requete  += pCIdentFieldData->m_Value.replace("'","\\'");}         break;
                case FTN_DATETIME:
                case  FTN_STR_DATETIME:   // 25191958  --> 1958-19-25 00:00:00
                     {               requete  +=   pCIdentFieldData->m_Value.mid(4)   +"-"+
                                                   pCIdentFieldData->m_Value.mid(2,2) +"-"+
                                                   pCIdentFieldData->m_Value.mid(0,2) +" 00:00:00";
                     }
                     break;
               } // end switch(pCIdentFieldData->m_Type)
         if (++i < nbFields)         requete +=        + "' , '";
         else                        requete +=        + "' );";
        } //while ( i < nbFields )                 // pour tous les champs
    query.exec (requete);
    OutSQL_error( query, "DossIndexCreate()", requete.toAscii() );
    //................ verifier si l'enregistrement a ete cree ...............................
    requeteIsGUIDExist  = "SELECT ";
    requeteIsGUIDExist += m_DOSS_IDENT_GUID                + " FROM  "      // 0 pk
                       +  m_DOSS_IDENT_TBL_NAME            + " WHERE "
                       +  m_DOSS_IDENT_GUID                + " = '" + numGUID + "'";
    query.exec(requeteIsGUIDExist);
    if (query.isActive() && query.next())    // si existe cassos
       {CloseBase();
        return pkIndex;
       }
    else
       {qDebug()<<tr("DossIdentIndexCreate() recording into %1' could not be created' \n%2").arg(m_DOSS_IDENT_TBL_NAME,requete);
        requete  = "DELETE FROM ";
        requete += m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_PRIM_KEY + "='" + pkIndex + "'";
        query.exec(requete);
        CloseBase();
        return QString::null;
       }
}

//-------------------------------------- DossIdentDataCreate --------------------------------------------------------------------
// ATTENTION: doit toujours etre appelee avec la base ouverte !!!
//
long  CMoteurBase::DossIdentDataCreate(QString& refDossPk,               QString& numGUID,
                                       PtrListIdentFieldData *plist_field, int  /*= CMoteurBase::esclave*/)
{QString ref_pk;
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... creer un curseur SQL de creation de l'enregistrement dans la table d'index des identites .................
  Q3SqlCursor cur(m_DOSS_IDENT_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canInsert ()) return FALSE;
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  //................... y placer les valeurs des champs ...................................
  //......... inserer les valeurs fondamentales ..................
  buffer->setValue( m_DOSS_IDENT_REF_PK, refDossPk );
  buffer->setValue( m_DOSS_IDENT_GUID,   numGUID );
  //......... inserer les autres valeurs .........................
  DossIdentBufferSetValue(buffer, plist_field );
  if (cur.insert())                                               // ecrire le buffer avec les donnees du header
     {ok = TRUE;                                                  // s'occuper des donnees binaires BLOB
     }
  //if (mode==CMoteurBase::progMasterUpdate) synchroWriteMofidication(m_DOSS_INDEX_TBL_NAME, refDossPk, m_DOSS_INDEX_PRIM_KEY, "C");
  //........................ recuperer la clef ...........................................
  return ok;
}

//-------------------------------------- DossIdentDelete --------------------------------------------------------------------
void  CMoteurBase::DossIdentDelete(RUBREC_LIST *pRubList, QString /*num_GUID*/, QString id_doss, int mode /*= CMoteurBase::esclave*/)
{//....................... addapter le comportement a un eventuel nomadisme ...............................................
 QString requete = "";
 /*
 if (mode==CMoteurBase::progMasterUpdate)
    {if (OpenBase()==0)  return ;
     QString status = synchroGetStatus(m_DOSS_INDEX_TBL_NAME, id_doss);
     //.........................
     //   si en creation on peut l'effacer du nomade car pas encore cree sur le referentiel
     //      mais alors detruire cette ligne de statut pour l'enlever de la liste des dossiers a creer
     //   sinon il faut le conserver et juste noter qu'il sera a effacer du referentiel plus tard lors de la synchro
     //         de maniere que lors de la replication referentiel --> esclave il le sera effectivment a ce moment la
     //         on change le nom pour ne plus l'avoir sous les yeux seul PB il est encore selectionable avec la vitale
     if (status != "C")
        {QString nom, prenom;
         GetPatientNomPrenomPk( FALSE, id_doss,  &nom);
         requete  = QString(" UPDATE ") + m_DOSS_INDEX_TBL_NAME   +
                              "  SET "  + m_DOSS_INDEX_NOM        + " = '" + nom.prepend("ZZZ_") +
                            "' WHERE "  + m_DOSS_INDEX_PRIM_KEY   + " = '" + id_doss + "'";
         QSqlQuery queryU(requete , QSqlDatabase::database(m_BaseLabel) );
         OutSQL_error(queryU, "DossIdentDelete() if (status != \"C\") ", requete);
         //............ tout mettre en effacement ............................................
         synchroSetDossierToDelete( id_doss );
         CloseBase();
         return;
        }
     else
        {synchroDeleteStatus(m_DOSS_INDEX_TBL_NAME, id_doss);
        }
     CloseBase();
    }
  */
 //........................ on commence par effacer les rubriques ..................................
 RUBREC_LIST::iterator it;
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {if ((*it).m_PrimKey != "" && (*it).m_PrimKey != "-1" && (*it).m_PrimKey != "0" && (*it).m_PrimKey[0] != '0')
         {RubListDelete(it, mode);                                      // effacer la rubrique
         }
     } //end while (pSqlQuery->next())

 //........................... effacement du header et donnees de l'identite ................................
 int          ok  = FALSE;
 //.............................. ouvrir la base ............................................................
 if (OpenBase()==0)  return ;

 //............................. effacer les donnees identitaires ...........................................
 requete = "DELETE FROM ";
 requete +=  m_DOSS_IDENT_TBL_NAME + " WHERE " + m_DOSS_IDENT_REF_PK + "='" + id_doss + "'";

 QSqlQuery sqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 ok = sqlQuery.exec(requete);
 if (ok == FALSE) {CloseBase(); return ;}        // si erreur cassos
 // en mode nomade les identites ne sont pas vraiment effacees et donc on ne
 // devrait jamais a avoir a marquer ca ici
 //if (mode==CMoteurBase::progMasterUpdate) synchroWriteMofidication(m_DOSS_IDENT_TBL_NAME, id_doss, "D");

 //............................. effacer les blobs ...........................................................
 requete  = "DELETE FROM ";
 requete +=  m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_PRIM_KEY + "='" + id_doss + "'";
 ok       = sqlQuery.exec(requete);
 //............................. fermer la base .............................................................
 CloseBase();
 return ;
}

//-------------------------------------- DossIdentIndexUpdate --------------------------------------------------------------------
// POUR QUE CELA MARCHE il faut imperativement une primarykey dans la table sinon: cur.update() ne fonctionne pas !!!
// SI plist_field est egal a zero alors seulement le nom et prenom sont mis a jour.

long  CMoteurBase::DossIdentIndexUpdate(QString& refDossPk, QString& nom, QString& prenom,
                                        PtrListIdentFieldData *plist_field /* =0 */, int mode /*= CMoteurBase::esclave*/)
{ int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return FALSE;
  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_DOSS_INDEX_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_DOSS_INDEX_PRIM_KEY + "='" + refDossPk+"'");
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                                // recuperer le buffer ( pas de delete a la fin !!)
     buffer->setValue( m_DOSS_INDEX_NOM,     nom.upper()    );              // y placer les donnees
     buffer->setValue( m_DOSS_INDEX_PRENOM,  prenom.upper() );              // y placer les donnees
     cur.update();      // ecrire le buffer avec les donnees du header
     QString lastErr= cur. lastError ().text();
     qDebug()<<lastErr;
     ok  = TRUE;
    }
  //......................... fermer la base ..............................................
  if (ok && plist_field)  // si
     {ok = DossIdentDataUpdate(refDossPk, plist_field , mode);
     }
  CloseBase();
  return ok;
}

//-------------------------------------- DossIdentDataUpdate --------------------------------------------------------------------
// POUR QUE CELA MARCHE il faut imperativement une primarykey dans la table sinon: cur.update() ne fonctionne pas !!!
// ATTENTION: doit toujours etre appelee avec la base ouverte !!!
//
long  CMoteurBase::DossIdentDataUpdate(QString& refDossPk, PtrListIdentFieldData *plist_field  , int  /*= CMoteurBase::esclave*/)
{ //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_DOSS_IDENT_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canUpdate ()) return FALSE;
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_DOSS_IDENT_REF_PK + "='" + refDossPk+"'");
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
  if ( cur.next() )
     {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer ( pas de delete a la fin !!)
      DossIdentBufferSetValue(buffer, plist_field );
      cur.update();
      QString lastErr= cur. lastError ().text();
      qDebug()<<lastErr;
      //if (mode==CMoteurBase::progMasterUpdate)
      //   {synchroWriteMofidication(m_DOSS_INDEX_TBL_NAME, refDossPk, m_DOSS_INDEX_PRIM_KEY, "U");
      //    synchroWriteMofidication(m_DOSS_IDENT_TBL_NAME, refDossPk, m_DOSS_IDENT_REF_PK,   "U");
      //   }
      return TRUE;
     }
  return FALSE;
}

//-------------------------------------- DossIdentBufferSetValue ---------------------------------------------------------------------

long  CMoteurBase::DossIdentBufferSetValue(QSqlRecord *buffer, PtrListIdentFieldData *plist_field )
{//......... inserer les autres valeurs .........................
 CIdentFieldData *pCIdentFieldData;
 for (int i = 0; i < plist_field->size(); ++i)                 // pour tous les champs
     {pCIdentFieldData = plist_field->at(i);
      switch(pCIdentFieldData->m_Type)
            {case FTN_MAJ_STRING:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value.upper() );     // y placer les donnees
                  break;
             case FTN_STRING:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value);              // y placer les donnees
                  break;
             case FTN_NUM_STRING:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value);              // y placer les donnees
                  break;
             case FTN_INTEGER:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value.toInt());              // y placer les donnees
                  break;
             case FTN_CHAR:
                  {char c =  pCIdentFieldData->m_Value.at(1).latin1();
                   buffer->setValue( pCIdentFieldData->m_Name, c);
                  }
                  break;
            case  FTN_STR_DATETIME:   // 25191958  --> 1958-19-25 00:00:00
                  {buffer->setValue(pCIdentFieldData->m_Name,
                                    pCIdentFieldData->m_Value.mid(4)   +"-"+
                                    pCIdentFieldData->m_Value.mid(2,2) +"-"+
                                    pCIdentFieldData->m_Value.mid(0,2) +" 00:00:00");
                  }
                  break;
             case FTN_DATETIME:   // "ddMMyyyy"
                  {QDate  qDteNss (        pCIdentFieldData->m_Value.mid(4).toInt(),
                                           pCIdentFieldData->m_Value.mid(2,2).toInt(),
                                           pCIdentFieldData->m_Value.mid(0,2).toInt()
                                  );
                   buffer->setValue( pCIdentFieldData->m_Name, qDteNss);
                  }
              break;
              case FTN_DATETIME_TIRET:   // "yyyy-MM-dd hh:mn:ss"
                  {QDate  qDteNss (        pCIdentFieldData->m_Value.mid(0,4).toInt(),
                                           pCIdentFieldData->m_Value.mid(5,2).toInt(),
                                           pCIdentFieldData->m_Value.mid(8,2).toInt()
                                  );
                   buffer->setValue( pCIdentFieldData->m_Name, qDteNss);
                  }
                  break;
            }
     }
 return 1;
}

//************************************************************************************************************************
//=================================== GESTION DU NOMADISME ===============================================================
//************************************************************************************************************************
/*
//----------------------------------------------- synchroGetTablesList ---------------------------------------------------
QStringList CMoteurBase::synchroGetTablesList()
{QStringList tablesList;
 tablesList.append(m_DOSS_INDEX_TBL_NAME);
 tablesList.append(m_DOSS_IDENT_TBL_NAME);
 tablesList.append(m_DOSS_NOTE_TBL_NAME);
 tablesList.append(m_DOSS_INTERVENANTS_TBL_NAME);
 tablesList.append(m_DOSS_RUB_HEAD_TBL_NAME);
 tablesList.append(m_DOSS_RUB_DATA_TBL_NAME);
 tablesList.append(m_EVNT_LIST_TBL_NAME);
 return tablesList;
}

//----------------------------------------------- synchroInitTables ---------------------------------------------------
void  CMoteurBase::synchroInitTables()
{synchroDropTables();
 QStringList tablesList = synchroGetTablesList();
 for (int i = 0; i < (int)tablesList.size(); ++i)
     {synchroInitTable(tablesList[i]);
     }
}

//----------------------------------------------- synchroDropTables ---------------------------------------------------
void  CMoteurBase::synchroDropTables()
{QStringList tablesList = synchroGetTablesList();
 for (int i = 0; i < (int)tablesList.size(); ++i)
     {synchroDropTable(tablesList[i]);
     }
}
//----------------------------------------------- synchroDropTable ---------------------------------------------------
int  CMoteurBase::synchroNomadeToMasterUpdate( const QString & driver,
                                               const QString & baseLabel,
                                               const QString & baseToOpen,
                                               const QString & user,
                                               const QString & pasword,
                                               const QString & hostname,
                                               const QString & port
                                              )
{ //............... ouvrir la base  de destination..................................................
  m_DestBaseLabel = baseLabel;
  //if (BaseConnect(driver,  baseToOpen,  user,   pasword, hostname,  port, 0,  baseLabel)==0) return 0;
  if (m_DestBaseLabel.length())
     {QSqlDatabase destMasterDB = QSqlDatabase::addDatabase( driver , m_DestBaseLabel);  // QSqlDatabase::database(m_DestBaseLabel)
      destMasterDB.setDatabaseName( baseToOpen );
      destMasterDB.setUserName(     user );
      destMasterDB.setPassword(     pasword );
      destMasterDB.setHostName(     hostname );
      destMasterDB.setPort(         port.toInt() );
      if ( ! destMasterDB.open() )
         {QString  qstr = "";
                   qstr += tr("Failed to open database: ") + QString(driver) + "  " + QString(baseToOpen) + "\r\n" +
                           destMasterDB.lastError().driverText() + "\r\n" + destMasterDB.lastError().databaseText();
                   qWarning(qstr);
          return 0;
         }
      if  (OpenBase()==0) { destMasterDB.close(); return 0;}
     }
  //............ OK la base de destination est ouverte .............................................
  //             a priori tous les pk doivent Ãªtre les memes
  //             pour l'update et le create

  //.............. on commence par les effacements des dossiers a effacer ........................................................
  {
    QString requete = QString("SELECT Pk_to_modified FROM ") + m_DOSS_INDEX_TBL_NAME + "_SYNC WHERE Modification = 'D'";
    QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
    if  (query.isActive())
        {while (query.next()) synchroSetDossierToDelete( query.value( 0 ).toString()); // si un index doit etre efface tout le dossier doit l'etre
        }
     else
        {OutSQL_error(query, "synchroWriteMofidication select( )" , requete );
        }
  }
 //................. effacer les tables ................................................................................................
 synchroTablesDelete();
 //.............. on poursuit par les mises a jour des tables a mettre a jour ........................................................
 synchroTablesUpdate();
 //.............. on poursuit par les creations des tables a creer ........................................................
 synchroTablesCreate();
 CloseBase();
 QSqlDatabase::database(m_DestBaseLabel).close();
 QSqlDatabase::removeDatabase ( m_DestBaseLabel );
 return 1;
}

//-------------------------------------- synchroTablesCreate ----------------------------------------------------------------------------
void  CMoteurBase::synchroTablesCreate()
{QStringList tablesList = synchroGetTablesList();
 //............... rechercher pour chaque table ..............................
 //               un eventuel create a faire
 for (int i = 0; i < (int)tablesList.size(); ++i)
     {synchroTablesCreateDispatcher(tablesList[i]);
     }
}

//-------------------------------------- synchroTablesCreateDispatcher ----------------------------------------------------------------------------
void  CMoteurBase::synchroTablesCreateDispatcher(const QString &table)
{     QSqlRecord rec  = QSqlDatabase::database(m_DestBaseLabel).record (table );
      QString requete = QString("SELECT Pk_to_modified, Pk_Name FROM ") + table + "_SYNC WHERE Modification = 'C'";
      QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
      if  (query.isActive())
          {while (query.next())
                 {if (table ==m_DOSS_INDEX_TBL_NAME)
                     {synchroTableIndexFichePatientCreate(table, rec, query.value(0).toString(), query.value(1).toString());
                     }
                  else if (table == m_DOSS_NOTE_TBL_NAME)
                     {synchroTableNoteCreate(table, rec, query.value(0).toString(), query.value(1).toString());
                     }
                  else if (table == m_DOSS_RUB_HEAD_TBL_NAME)
                     {synchroTableRubriqueHeadBlobsCreate(table, rec, query.value(0).toString(), query.value(1).toString());
                     }
                  else if (table != m_DOSS_RUB_DATA_TBL_NAME &&  // creation traitee avec les cas particuliers plus haut : synchroTableRubriqueHeadBlobsCreate
                           table != m_DOSS_IDENT_TBL_NAME )      // creation traitee avec les cas particuliers plus haut : synchroTableIndexFichePatientCreate
                     {synchroTableCreate(table, rec, query.value(0).toString(), query.value(1).toString());
                     }
                 }
          }
      else
         {OutSQL_error(query, "synchroTablesCreate select( )" , requete );
         }
}

//-------------------------------------- synchroTableNoteCreate ----------------------------------------------------------------------------
// cas particulier des rubriques ou les references pk du blobs lie et de l'index nom prenom lie doit etre inscrit
//              les pk  deja en place ne seront pas ceux crees sur le master.
void  CMoteurBase::synchroTableNoteCreate(const QString &table, QSqlRecord &rec, const QString &primKey, const QString &primKeyName)
{ QString dossDestPk      = "";
  QString requete         = "";
  QString guid            = "";


  //................... recuperer le guid du patient .........................................................................
  //                    dans le  header d'origine
  //                    car c'est lui qui va nous guider pour recuperer
  //                    la pk creee dans la destination
  requete      = QString ("SELECT ") + m_DOSS_NOTE_PAT_GUID  + " FROM " + table + " WHERE " + primKeyName + "= '" + primKey + "'";
  QSqlQuery querySrc(requete , QSqlDatabase::database(m_BaseLabel) );
  if (querySrc.isActive() && querySrc.next())  guid = querySrc.value(0).toString();
  if (guid.length()==0)            return;

  //.................. recuperer le pk de l'index nom prenom du dossier de destination .......................................
  //                   avec le guid car il doit etre place dans les donnees
  requete = QString ("SELECT ") + m_DOSS_INDEX_PRIM_KEY + " FROM " + m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_GUID + "= '" + guid + "'";
  QSqlQuery queryDst(requete, QSqlDatabase::database(m_DestBaseLabel));
  if (queryDst.isActive() && queryDst.next())  dossDestPk = queryDst.value(0).toString();
  if (dossDestPk.length()==0)       return;

  int nb          = rec.count ();
  int i           = 0;

  //.......... requete de selection des champs de la source ............................
  requete = "SELECT \n";
  for (i = 0; i<nb; ++i)
      { if (i!=nb-1) requete += rec.fieldName(i) + "     ,\n";
        else         requete += rec.fieldName(i) + " FROM \n";
      }
                     requete +=  table           +  " WHERE ";
                     requete +=  primKeyName     +  " =    '" + primKey + "'";

  querySrc.exec(requete);
  //................. preparation de la destination ..............................................
  int     pk_FieldIndex      = -1;
  QString fieldName          = "";
  QString prepare            = "";
  prepare                   += "INSERT INTO "  + table  + " ( \n";
                         for (i = 0; i<nb; ++i)
                             { fieldName = rec.fieldName(i);
                               if (fieldName==primKeyName)  // le pk  auto incrementable ne doit pas etre positionne
                                  {pk_FieldIndex = i;
                                  }
                               else
                                  { if (i!=nb-1) prepare += fieldName + " ,\n";
                                    else         prepare += fieldName + " )\n  VALUES ( ";
                                  }
                             }
                         for (i = 0; i<nb; ++i)
                             { if (i != pk_FieldIndex)
                                  {if (i!=nb-1) prepare +=  " ?,";
                                   else         prepare +=  " ?)\n";
                                  }
                             }
  queryDst.prepare(prepare);
  if (querySrc.isActive() && querySrc.next())
     {for (i = 0; i<nb; ++i)
          { if (i != pk_FieldIndex)
               {fieldName = rec.fieldName(i);
                if (fieldName==m_DOSS_NOTE_PAT_PK)          queryDst.bindValue(i, dossDestPk);
                else                                        queryDst.bindValue(i, querySrc.value(i));
               }
          }
     }
  if (!queryDst.exec())
     {OutSQL_error(queryDst, "synchroTableNoteCreate()" , requete );
      return ;
     }
}

//-------------------------------------- synchroTableRubriqueHeadBlobsCreate ----------------------------------------------------------------------------
// cas particulier des rubriques ou les references pk du blobs lie et de l'index nom prenom lie doit etre inscrit
//              les pk  deja en place ne seront pas ceux crees sur le master.
void  CMoteurBase::synchroTableRubriqueHeadBlobsCreate(const QString &table, QSqlRecord &rec, const QString &primKey, const QString &primKeyName)
{ QString newPk           = "";
  QString newPkDoss       = "";
  QString primKeyBlobsSrc = "";
  QString requete         = "";
  QString guid            = "";

  //................... recuperer le guid du patient .........................................................................
  //                    dans le  header d'origine
  //                    car c'est lui qui va nous guider pour recuperer
  //                    la pk creee dans la destination
  requete      = QString ("SELECT ") + m_DOSS_RUB_HEAD_GUID + " FROM " + table + " WHERE " + primKeyName + "= '" + primKey + "'";  // recuper
  QSqlQuery querySrc(requete , QSqlDatabase::database(m_BaseLabel) );
  if (querySrc.isActive() && querySrc.next())  guid = querySrc.value(0).toString();
  if (guid.length()==0)            return;

  //................... recuperer le pk du blobs d'origine a partir du header d'origine.........................................................................
  requete      = QString ("SELECT ") + m_DOSS_RUB_HEAD_REF_BLOBS_PK + " FROM " + m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_REF_BLOBS_PK + "= '" + primKey + "'";  // recuper
  querySrc.exec(requete );
  if (querySrc.isActive() && querySrc.next())  primKeyBlobsSrc = querySrc.value(0).toString();
  if (primKeyBlobsSrc.length()==0) return;

  //.................. creer normalement le blobs dans la destination ................................................
  //                   et recuperer sa pk
  newPk = synchroTableCreate(m_DOSS_RUB_DATA_TBL_NAME, QSqlDatabase::database(m_DestBaseLabel).record (m_DOSS_RUB_DATA_TBL_NAME ), primKeyBlobsSrc, m_DOSS_RUB_DATA_PRIMKEY);
  if (newPk.length()==0)           return;

  //.................. recuperer le pk de l'index nom prenom du dossier de destination .......................................
  //                   avec le guid car il doit etre place dans le header
  requete = QString ("SELECT ") + m_DOSS_INDEX_PRIM_KEY + " FROM " + m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_GUID + "= '" + guid + "'";
  QSqlQuery queryDst(requete, QSqlDatabase::database(m_DestBaseLabel));
  if (queryDst.isActive() && queryDst.next())  newPkDoss = queryDst.value(0).toString();
  if (newPkDoss.length()==0)       return;

  //.......... requete de selection des champs de la source (fiche header) ............................
  int nb                = rec.count ();
  int i                 = 0;

  requete = "SELECT \n";
  for (i = 0; i<nb; ++i)
      { if (i!=nb-1) requete += rec.fieldName(i) + "     ,\n";
        else         requete += rec.fieldName(i) + " FROM \n";
      }
                     requete +=  table                          +  " WHERE ";
                     requete +=  m_DOSS_RUB_HEAD_REF_BLOBS_PK   +  " =    '" + primKeyBlobsSrc + "'";    // on selectionne la source par la reference sur le pk du blobs
  querySrc.exec(requete );

  //................. preparation de la destination ..............................................
  QString fieldName          = "";
  QString prepare            = "";
  prepare                   += "INSERT INTO "  + table  + " ( \n";
                         for (i = 0; i<nb; ++i)
                             { fieldName = rec.fieldName(i);
                               if (i!=nb-1) prepare += fieldName + " ,\n";
                               else         prepare += fieldName + " )\n  VALUES ( ";
                             }
                         for (i = 0; i<nb; ++i)
                             {if (i!=nb-1) prepare +=  " ?,";
                              else         prepare +=  " ?)\n";
                             }
  queryDst.prepare(prepare);
  if (querySrc.isActive() && querySrc.next())
     {for (i = 0; i<nb; ++i)
          { fieldName = rec.fieldName(i);
            if (fieldName==m_DOSS_RUB_HEAD_REF_DOSS_PK)          queryDst.bindValue(i, newPkDoss);
            else if (fieldName == m_DOSS_RUB_HEAD_REF_BLOBS_PK)  queryDst.bindValue(i, newPk);
            else                                                 queryDst.bindValue(i, querySrc.value(i));
          }
     }
  if (!queryDst.exec())
     {OutSQL_error(querySrc, "synchroTableRubriqueHeadBlobsCreate()" , requete );
      OutSQL_error(queryDst, "synchroTableRubriqueHeadBlobsCreate()" , prepare );
      if (newPkDoss.length())
         {requete  = QString ("DELETE FROM ") + m_DOSS_RUB_DATA_TBL_NAME  + " WHERE "+ m_DOSS_RUB_DATA_PRIMKEY + "= '" + newPkDoss + "' ";
          queryDst.exec(requete);
         }
     }
}

//-------------------------------------- synchroTableIndexFichePatientCreate ----------------------------------------------------------------------------
// cas particulier car le Pk de la fiche patient doit etre force a celui de l'index nom prenom
void  CMoteurBase::synchroTableIndexFichePatientCreate(const QString &tableInd, const QSqlRecord &recInd, const QString &primKeyInd, const QString &primKeyNameInd)
{ QString newPk   = "";
  QString requete = "";
  //................... recuperer le guid du patient .........................................................................
  //                    car c'est lui qui va nous guider pour recuperer
  //                    la pk creee dans la destination
  QString nom, prenom, guid;
  GetPatientNomPrenomPk( FALSE, primKeyInd,  &nom, &prenom, &guid);
  if (guid.length()==0)   return;
  //.................. creer normalement la table d'index dans la destination ................................................
  QString truc = synchroTableCreate(tableInd, recInd, primKeyInd, primKeyNameInd);
  //.................. recuperer la pk de cette table d'index creee dans la destination.......................................
  //                   car c'est elle qui sera placee en pk de la fiche d'identite
  requete = QString ("SELECT ") + primKeyNameInd + " FROM " + tableInd + " WHERE " + m_DOSS_INDEX_GUID + "= '" + guid + "'";
  QSqlQuery queryDst(requete);
  if (queryDst.isActive() && queryDst.next())  newPk = queryDst.value(0).toString();
  if (newPk.length()==0)  return;


  //.......... requete de selection des champs de la source (fiche identite) ............................
  QString table         = m_DOSS_IDENT_TBL_NAME;
  QString primKeyName   = m_DOSS_IDENT_REF_PK;     // pour la fiche patient la Pk est celle de l'index Nom Prenom
  QSqlRecord rec  = QSqlDatabase::database(m_DestBaseLabel).record (table );
  int nb          = rec.count ();
  int i           = 0;

  requete = "SELECT \n";
  for (i = 0; i<nb; ++i)
      { if (i!=nb-1) requete += rec.fieldName(i)     + "     ,\n";
        else         requete += rec.fieldName(i)     + " FROM \n";
      }
                     requete +=  table               +  " WHERE ";
                     requete +=  m_DOSS_IDENT_GUID   +  " =    '" + guid + "'";    // on selectionne la source par le guid
  QSqlQuery querySrc(requete , QSqlDatabase::database(m_BaseLabel) );
  //................. preparation de la destination ..............................................
  int     pk_FieldIndex      = -1;
  QString fieldName          = "";
  QString prepare            = "";
  prepare                   += "INSERT INTO "  + table  + " ( \n";
                         for (i = 0; i<nb; ++i)
                             { fieldName = rec.fieldName(i);
                               if (fieldName==primKeyName)  // la pk de la table de la fiche patient n'est pas auto incrementable et doit etre positionnee
                                  {pk_FieldIndex = i;
                                  }
                               if (i!=nb-1) prepare += fieldName + " ,\n";
                               else         prepare += fieldName + " )\n  VALUES ( ";
                             }
                         for (i = 0; i<nb; ++i)
                             {if (i!=nb-1)  prepare +=  " ?,";
                              else          prepare +=  " ?)\n";
                             }
  queryDst.prepare(prepare);
  if (querySrc.isActive() && querySrc.next())
     {for (i = 0; i<nb; ++i)
          { if (i == pk_FieldIndex)  queryDst.bindValue(i, newPk);
            else                     queryDst.bindValue(i, querySrc.value(i));
          }
     }
  if (!queryDst.exec())
     {OutSQL_error(queryDst, "synchroTableIndexCreate()" , requete );
     }
}

//-------------------------------------- synchroTableCreate ----------------------------------------------------------------------------
QString  CMoteurBase::synchroTableCreate(const QString &table, const QSqlRecord &rec, const QString &primKey, const QString &primKeyName)
{ int nb          = rec.count ();
  int i           = 0;
  QString requete = "";

  //.......... requete de selection des champs de la source ............................
  requete = "SELECT \n";
  for (i = 0; i<nb; ++i)
      { if (i!=nb-1) requete += rec.fieldName(i) + "     ,\n";
        else         requete += rec.fieldName(i) + " FROM \n";
      }
                     requete +=  table           +  " WHERE ";
                     requete +=  primKeyName     +  " =    '" + primKey + "'";

  QSqlQuery querySrc(requete , QSqlDatabase::database(m_BaseLabel) );
  //................. preparation de la destination ..............................................
  int     pk_FieldIndex      = -1;
  QString fieldName          = "";
  QString prepare            = "";
  prepare                   += "INSERT INTO "  + table  + " ( \n";
                         for (i = 0; i<nb; ++i)
                             { fieldName = rec.fieldName(i);
                               if (fieldName==primKeyName)  // la pk de la table de la fiche patient n'est pas auto incrementable et doit etre positionnee
                                  {pk_FieldIndex = i;
                                  }
                               else
                                  { if (i!=nb-1) prepare += rec.fieldName(i) + " ,\n";
                                    else         prepare += rec.fieldName(i) + " )\n  VALUES ( ";
                                  }
                             }
                         for (i = 0; i<nb; ++i)
                             { if (i != pk_FieldIndex)
                                  {if (i!=nb-1) prepare +=  " ?,";
                                   else         prepare +=  " ?)\n";
                                  }
                             }
  QSqlQuery queryDst(QSqlDatabase::database(m_DestBaseLabel));
  queryDst.prepare(prepare);
  if (querySrc.isActive() && querySrc.next())
     {for (i = 0; i<nb; ++i)
          { if (i != pk_FieldIndex) queryDst.bindValue(i, querySrc.value(i));
          }
     }
  if (!queryDst.exec())
     {OutSQL_error(queryDst, "synchroTableCreate()" , requete );
      return QString::null;
     }

  //................. recuperer dernier primKey insere ..............................
  requete = QString("SELECT LAST_INSERT_ID() FROM ") + table + " WHERE " + primKeyName + " = LAST_INSERT_ID()";
  if (!queryDst.exec(requete))
     {OutSQL_error(queryDst, "synchroTableCreate()::LAST_INSERT_ID()" , requete );
      return QString::null;
     }
  if ( queryDst.next() ) return queryDst.value(0).toString();
  return QString::null;
}

//-------------------------------------- synchroTablesDelete ----------------------------------------------------------------------------
void  CMoteurBase::synchroTablesDelete()
{QStringList tablesList = synchroGetTablesList();
 QString          table = "";
 //............... rechercher pour chaque table ..............................
 //               un eventuel update a faire
 for (int i = 0; i < (int)tablesList.size(); ++i)
     {table           = tablesList[i];
      QSqlRecord rec  = QSqlDatabase::database(m_DestBaseLabel).record (table );
      QString requete = QString("SELECT Pk_to_modified, Pk_Name FROM ") + table + "_SYNC WHERE Modification = 'D'";
      QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
      if  (query.isActive())
          {while (query.next())
                 {synchroTableDelete(table, rec, query.value(0).toString(), query.value(1).toString());
                 }
          }
      else
         {OutSQL_error(query, "synchroTablesDelete select()" , requete );
         }
     }
}

//-------------------------------------- synchroTableDelete ----------------------------------------------------------------------------
void  CMoteurBase::synchroTableDelete(const QString &table, const QSqlRecord &, const QString &primKey, const QString &primKeyName)
{ //int nb = rec.count ();
  //int i  = 0;
  //.......... verifier que la table de destination existe encore ......................
  //           a pu etre effacee pendant l'absence du nomade
  //           donc on cherche si le pk est tjr valide
  QString requete("SELECT ");
  requete  +=  primKeyName   + " FROM  " + table   +  " WHERE " + primKeyName   +  " = '" + primKey  + "' ";
  QSqlQuery query(requete , QSqlDatabase::database(m_DestBaseLabel) );
  if ( !query.isActive()) {OutSQL_error(query, "synchroTableDelete()::Erreur recherche enregistrement de destination" ,  requete ); return;}
  if ( !query.next())     {qDebug(QString("synchroTableDelete()::enregistrement de destination non trouve\n")+           requete);  return;}

  //.......... requete de selection des champs de la source ............................
  QSqlQuery queryDst(QSqlDatabase::database(m_DestBaseLabel));
  requete =  QString ("DELETE FROM ") + table        + " WHERE " + primKeyName  + "='" + primKey + "'";
  if (!queryDst.exec(requete)) OutSQL_error(queryDst, "synchroTableDelete()" , requete );
}


//-------------------------------------- synchroTablesUpdate ----------------------------------------------------------------------------
void  CMoteurBase::synchroTablesUpdate()
{QStringList tablesList = synchroGetTablesList();
 QString          table = "";
 //............... rechercher pour chaque table ..............................
 //               un eventuel update a faire
 for (int i = 0; i < (int)tablesList.size(); ++i)
     {table           = tablesList[i];
      QSqlRecord rec  = QSqlDatabase::database(m_DestBaseLabel).record (table );
      QString requete = QString("SELECT Pk_to_modified, Pk_Name FROM ") + table + "_SYNC WHERE Modification = 'U'";
      QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
      if  (query.isActive())
          {while (query.next())
                 {synchroTableUpdate(table, rec, query.value(0).toString(), query.value(1).toString());
                 }
          }
      else
         {OutSQL_error(query, "synchroTablesUpdate select( )" , requete );
         }
     }
}
//-------------------------------------- synchroTableUpdate ----------------------------------------------------------------------------
void  CMoteurBase::synchroTableUpdate(const QString &table, const QSqlRecord &rec, const QString &primKey, const QString &primKeyName)
{ int nb = rec.count ();
  int i  = 0;
  //.......... verifier que la table de destination existe encore ......................
  //           a pu etre effacee pendant l'absence du nomade
  //           donc on cherche si le pk est tjr valide
  QString requete("SELECT ");
  requete  +=  primKeyName   + " FROM  " + table   +  " WHERE " + primKeyName   +  " = '" + primKey  + "' ";
  QSqlQuery query(requete , QSqlDatabase::database(m_DestBaseLabel) );
  if ( !query.isActive()) {OutSQL_error(query, "synchroTableUpdate()::Erreur recherche enregistrement de destination" ,  requete ); return;}
  if ( !query.next())     {synchroWriteMofidication(table, primKey, primKeyName, "C");            // si pas trouve  on le met en creation (a soumettre a une option ?)
                           //synchroTablesCreateDispatcher(table, destMasterDB);
                           qDebug(QString("synchroTableUpdate()::enregistrement de destination non trouve --> creation de l'enregistrement\n")+           requete);
                           return;
                          }

  //.......... requete de selection des champs de la source ............................
  requete = "SELECT \n";
  for (i = 0; i<nb; ++i)
      { if (i!=nb-1) requete += rec.fieldName(i) + "     ,\n";
        else         requete += rec.fieldName(i) + " FROM \n";
      }
                     requete +=  table           +  " WHERE ";
                     requete +=  primKeyName     +  " =    '" + primKey + "'";

  QSqlQuery querySrc(requete , QSqlDatabase::database(m_BaseLabel) );

  //................. preparation de la destination ..............................................
  QString prepare            = "";
  prepare                   += "UPDATE "  + table   + " SET \n";
                         for (i = 0; i<nb; ++i)
                             { if (i!=nb-1) prepare += rec.fieldName(i) + " = ? ,\n";
                               else         prepare += rec.fieldName(i) + " = ?  \n";
                             }
  prepare                   += " WHERE " + primKeyName   + " ='"  + primKey  + "'";

  QSqlQuery queryDst(QSqlDatabase::database(m_DestBaseLabel));
  queryDst.prepare(prepare);
  if (querySrc.isActive() && querySrc.next())
     {for (i = 0; i<nb; ++i)
          { queryDst.bindValue(i, querySrc.value(i));
          }
     }
  if (!queryDst.exec())
     {OutSQL_error(queryDst, "synchroTableUpdate()" , requete );
     }
}

//-------------------------------------- synchroSetDossierToDelete ----------------------------------------------------------------------------
// ACTION : prend tous les enregistrements concernes par le dossier et les place sur "D" afin d'eliminer les eventuels "U" precedemment places
void  CMoteurBase::synchroSetDossierToDelete( const QString &primKeyPat)
{QString nom, prenom, guid;
 GetPatientNomPrenomPk( TRUE, primKeyPat,  &nom, &prenom, &guid);
 if (guid.length()==0) return;
 if  (OpenBase()==0)  return ;

 QSqlQuery query(QString::null , QSqlDatabase::database(m_BaseLabel) );
 //................ index et fiche patient ..............................................
 synchroWriteMofidication(m_DOSS_INDEX_TBL_NAME, primKeyPat, m_DOSS_INDEX_PRIM_KEY, "D");
 synchroWriteMofidication(m_DOSS_IDENT_TBL_NAME, primKeyPat, m_DOSS_IDENT_REF_PK,   "D"); // la pk de l'identite est la meme que celle de l'index (place de force a la creation)
 //................ note de la fiche patient ..............................................
 QString requete =  QString ("SELECT ") + m_DOSS_NOTE_PK          + " FROM " + m_DOSS_NOTE_TBL_NAME     + " WHERE " + m_DOSS_NOTE_PAT_GUID            + "='" + guid + "'";
 query.exec(requete);
 if  (query.isActive())
     {while (query.next()) synchroWriteMofidication(m_DOSS_NOTE_TBL_NAME, query.value( 0 ).toString(), m_DOSS_NOTE_PK,   "D");
     }
  else
     {OutSQL_error(query, "synchroSetDossToDelete::notes " , requete / *=0* /);
     }
 //................ intervenants de la fiche patient ..............................................
 requete =  QString ("SELECT ") + m_DOSS_INTERVENANTS_PK   + " FROM " + m_DOSS_INTERVENANTS_TBL_NAME     + " WHERE " + m_DOSS_INTERVENANTS_PAT_GUID   + "='" + guid + "'";
 query.exec(requete);
 if  (query.isActive())
     {while (query.next()) synchroWriteMofidication(m_DOSS_INTERVENANTS_TBL_NAME, query.value( 0 ).toString(), m_DOSS_INTERVENANTS_PK,   "D");
     }
  else
     {OutSQL_error(query, "synchroSetDossToDelete::intervenants " , requete / *=0* /);
     }
 //................ header rubriques patient ..............................................
 requete =  QString ("SELECT ") + m_DOSS_RUB_HEAD_PRIMKEY + " FROM " + m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_GUID  + "='" + guid + "'";
 query.exec(requete);
 if  (query.isActive())
     {while (query.next()) synchroWriteMofidication(m_DOSS_RUB_HEAD_TBL_NAME, query.value( 0 ).toString(), m_DOSS_RUB_HEAD_PRIMKEY,   "D");
     }
  else
     {OutSQL_error(query, "synchroSetDossToDelete::rubHead " , requete / *=0* /);
     }
 //................ blobs rubriques patient ..............................................
 requete =  QString ("SELECT ") + m_DOSS_RUB_DATA_PRIMKEY + " FROM " + m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_GUID  + "='" + guid + "'";
 query.exec(requete);
 if  (query.isActive())
     {while (query.next()) synchroWriteMofidication(m_DOSS_RUB_DATA_TBL_NAME, query.value( 0 ).toString(), m_DOSS_RUB_DATA_PRIMKEY,   "D");
     }
  else
     {OutSQL_error(query, "synchroSetDossToDelete::blobs " , requete / *=0 * /);
     }
 //................ evenements vigie patient ..............................................
 requete =  QString ("SELECT ") + m_EVNT_LIST_PK + " FROM " + m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_DOSS_PK  + "='" + primKeyPat + "'";
 query.exec(requete);
 if  (query.isActive())
     {while (query.next()) synchroWriteMofidication(m_EVNT_LIST_TBL_NAME, query.value( 0 ).toString(), m_EVNT_LIST_PK,   "D");
     }
  else
     {OutSQL_error(query, "synchroSetDossToDelete::vigie " , requete / *=0 * /);
     }
 CloseBase();
}


//-------------------------------------- synchroDossierDelete ----------------------------------------------------------------------------
void  CMoteurBase::synchroDossierDelete( const QString &primKeyPat,  QSqlDatabase *destMasterDB)
{return;
 QString nom, prenom, guid;
 GetPatientNomPrenomPk( TRUE, primKeyPat,  &nom, &prenom, &guid);
 if (guid.length()==0) return;

 synchroSetDossierToDelete(primKeyPat);      // etre sur que tous les elements du dossier soient sur "D"  (afin que d'eventuels "U" n'y soient pas)

 QString requete = "";
 QSqlQuery sqlQuery(QString::null , destMasterDB );

 requete =  QString ("DELETE FROM ") + m_DOSS_RUB_HEAD_TBL_NAME        + " WHERE " + m_DOSS_RUB_HEAD_GUID            + "='" + guid + "'";
 sqlQuery.exec(requete);

 requete =  QString ("DELETE FROM ") + m_DOSS_RUB_DATA_TBL_NAME        + " WHERE " + m_DOSS_RUB_DATA_GUID            + "='" + guid + "'";
 sqlQuery.exec(requete);

 requete =  QString ("DELETE FROM ") + m_DOSS_INTERVENANTS_TBL_NAME    + " WHERE " + m_DOSS_INTERVENANTS_PAT_GUID    + "='" + guid + "'";
 sqlQuery.exec(requete);

 requete =  QString ("DELETE FROM ") + m_DOSS_NOTE_TBL_NAME            + " WHERE " + m_DOSS_NOTE_PAT_GUID            + "='" + guid + "'";
 sqlQuery.exec(requete);

 requete =  QString ("DELETE FROM ") + m_DOSS_INDEX_TBL_NAME           + " WHERE " + m_DOSS_INDEX_GUID               + "='" + guid + "'";
 sqlQuery.exec(requete);

 requete =  QString ("DELETE FROM ") + m_DOSS_IDENT_TBL_NAME           + " WHERE " + m_DOSS_IDENT_GUID               + "='" + guid + "'";
 sqlQuery.exec(requete);

 requete =  QString ("DELETE FROM ") + m_EVNT_LIST_TBL_NAME            + " WHERE " + m_EVNT_DOSS_PK                  + "='" + primKeyPat + "'";
 sqlQuery.exec(requete);
}

//----------------------------------------------- synchroIsThisIdentNoteExist ---------------------------------------------------
QString  CMoteurBase::synchroIsThisIdentNoteExist(const QString &primKeyPat, QSqlDatabase *destMasterDB)
{ QString ret = "";
  QString requete("");
  requete  += "SELECT " + m_DOSS_NOTE_PK        + " FROM " + m_DOSS_NOTE_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_NOTE_PAT_PK    + " = '"   + primKeyPat             + "' ";

  QSqlQuery query(requete , destMasterDB );

  //.................. si la requete a un resultat ..............................................
  if (query.isActive() && query.next())
     {ret = query.value(0).toString();
     }
  return ret;
}

//-------------------------------------- synchroIdentNoteDelete ----------------------------------------------------------------------------
void  CMoteurBase::synchroIdentNoteDelete( const QString &primKey,  QSqlDatabase *destMasterDB)
{QString  requete("DELETE FROM ");
          requete += m_DOSS_NOTE_TBL_NAME + " WHERE ";
          requete += m_DOSS_NOTE_PK       + "= '" + primKey + "' ";
 QSqlQuery query(requete , destMasterDB );
}
//-------------------------------------- synchroIdentIntervenantsDelete ----------------------------------------------------------------------------
void  CMoteurBase::synchroIdentIntervenantsDelete( const char *primKeyPat, QSqlDatabase *destMasterDB)
{ QString requete("DELETE FROM ");
          requete += m_DOSS_INTERVENANTS_TBL_NAME  + " WHERE ";
          requete += m_DOSS_INTERVENANTS_PAT_PK    + "= '" + primKeyPat + "' ";
  QSqlQuery query(requete , destMasterDB );
}


//----------------------------------------------- synchroDropTable ---------------------------------------------------
void  CMoteurBase::synchroDropTable(const QString &tableName)
{ //............... ouvrir la base  ..................................................
 if  (OpenBase()==0)  return ;
 QString requete = QString("DROP TABLE IF EXISTS `")+tableName+"_SYNC`;";
 QSqlQuery query_Drop(QSqlDatabase::database(m_BaseLabel));
 query_Drop.exec(requete);
 OutSQL_error(query_Drop, "synchroDropTable( )" , requete / *=0* /);
 CloseBase();
}

//----------------------------------------------- synchroGetStatus ---------------------------------------------------
QString  CMoteurBase::synchroGetStatus(QString tableName, const QString &pk_toGet, QString *pk_ret / *=0* /)
{tableName      += "_SYNC";
 QString status  = "";
 QString requete = QString("SELECT Pk, Modification FROM ") + tableName + " WHERE Pk_to_modified = '"+   pk_toGet + "'";
 QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
 if  (query.isActive())
     {while (query.next())
            { if (pk_ret) *pk_ret =  query.value( 0 ).toString();
              status              =  query.value( 1 ).toString();
            }
     }
  else
     {OutSQL_error(query, "synchroGetStatus select( )" , requete / *=0* /);
     }
 return status;
}
//----------------------------------------------- synchroWriteMofidication ---------------------------------------------------
void  CMoteurBase::synchroWriteMofidication(QString tableName, const QString &pk_toWrite, const QString &pkName, const QString &modif)
{ //............... ouvrir la base  ..................................................
 //if  (OpenBase()==0)  return ;
 QString pk;
 QString status = synchroGetStatus(tableName, pk_toWrite, &pk);
 if (status==modif) { / *CloseBase(); * /return;}

 //.................... si modification ou creation ............................................
 QString requete ="";
 if (pk.length())
    {if (status == "C" && modif=="U")  return;   // si en creation garder l'attribut : doit etre cree
     requete  = QString(" UPDATE ")     + tableName+ "_SYNC SET Modification = '" + modif + "' WHERE Pk = '" + pk + "'";
     QSqlQuery queryU(requete , QSqlDatabase::database(m_BaseLabel) );
     OutSQL_error(queryU, "synchroWriteMofidication update( )" , requete / *=0* /);
    }
 else
    {//requete =  QString("INSERT INTO ") + tableName   + "_SYNC (Pk_to_modified , Modification, TableName) VALUES ('" + pk_toWrite + "','" + modif + "','"+tableName+"' )";
     requete =  QString("INSERT INTO ") + tableName   + "_SYNC (Pk_to_modified , Pk_Name, Modification) VALUES ('" + pk_toWrite + "','" + pkName + "','" + modif +"' )";
     QSqlQuery query_I(requete, QSqlDatabase::database(m_BaseLabel));
     OutSQL_error(query_I, "synchroWriteMofidication insert()" , requete / *=0* /);
    }
 //CloseBase();
}

//----------------------------------------------- synchroDeleteStatus ---------------------------------------------------
void  CMoteurBase::synchroDeleteStatus(QString tableName, const QString &pk_toDelete)
{tableName       += "_SYNC";
 QString requete  = QString(" DELETE FROM `") + tableName + "` WHERE Pk_to_modified =`" + pk_toDelete + "`;";
 QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
 OutSQL_error(query, "synchroDeleteStatus () " , requete / *=0* /);
}

//----------------------------------------------- synchroInitTable ---------------------------------------------------
void  CMoteurBase::synchroInitTable(const QString &tableName)
{synchroDropTable(tableName);
  //............... ouvrir la base  ..................................................
 if  (OpenBase()==0)  return ;
 QString requete  = "";
 requete += "CREATE TABLE IF NOT EXISTS " + tableName + "_SYNC (";
 requete +=        " Pk BIGINT(20) NOT NULL auto_increment,";
 requete +=        " Pk_to_modified BIGINT(20) UNSIGNED NOT NULL,";
 requete +=        " Pk_Name        CHAR(40) ,";
 requete +=        " Modification   CHAR(2)    default NULL,";
 //requete +=        " TableName char(60) default NULL,";
 requete +=        " PRIMARY KEY (pk)";
 requete +=        ");";
 QSqlQuery query_sync(requete, QSqlDatabase::database(m_BaseLabel));
 OutSQL_error(query_sync, "initSynchroTable()" , requete / *=0* /);
 CloseBase();
}
*/

//************************************************************************************************************************
//=================================== GESTION DE LA LISTE DES EVENEMENTS ==================================================
//************************************************************************************************************************
//   QString      m_EVNT_LIST_TBL_NAME;          // nom de la table des droits des utilisateurs non prescripteurs
//   QString      m_EVNT_LIST_PK;                // clef primaire de cet enregistrement
//   QString      m_EVNT_DOSS_PK;                // clef primaire du dossier patient duquel depend ce patient
//   QString      m_EVNT_LIST_PERE;              // clef primaire d'un enr pere de la table DOSS_RUB_HEAD_TBL_NAME
//   QString      m_EVNT_LIST_PKDOC;             // clef primaire d'un enr fils de la table DOSS_RUB_HEAD_TBL_NAME



//----------------------------------------------- Evnmt_InitList ---------------------------------------------------
void  CMoteurBase::Evnmt_InitList(EVNT_LIST *pEvnmtList, QString dossPk)
{     QString requete = QString("SELECT ")            +
                          m_EVNT_LIST_PERE     + ", " +     // 0
                          m_EVNT_LIST_PKDOC    + ", " +     // 1
                          m_EVNT_LIST_PK       + ", " +     // 2
                          m_EVNT_DOSS_PK       + "  " +     // 3
                          " FROM "             + m_EVNT_LIST_TBL_NAME                         +
                          " WHERE "            + m_EVNT_LIST_PERE + " = 0"                       // Si pere =  0  alors c'est une racine
                          " AND "              + m_EVNT_DOSS_PK   + " =  " + dossPk           + ";" ;  // que ce qui concerne ce dossier
            if (OpenBase()==0)  return;  // si point = erreur d'ouverture de la base
                QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
                if (query.isActive())
                   {pEvnmtList->clear();
                    while (query.next())
                           {QString evnmt_pere  = query.value( 0 ).toString();                 // servira pour rechercher les fils
                            QString evnmt_PkDoc = query.value( 1 ).toString();                 // servira pour rechercher les fils
                            QString evnmt_Pk    = query.value( 2 ).toString();                 // servira pour rechercher les fils
                            //const char* pere, const char* pkDoc, const char* pk, const char* dossPk
                            pEvnmtList->append(CRubEvntPair(evnmt_pere                 ,       // pere
                                                            evnmt_PkDoc                ,       // fils
                                                            evnmt_Pk                   ,       // pk
                                                            query.value( 3 ).toString()        // dossPk
                                                            )
                                              );
                            Evnmt_GetFils(pEvnmtList , evnmt_Pk);                              // rechercher les fils du pere
                           }
                   }
                else
                   {OutSQL_error(query, "Evnmt_InitList( )" , requete /*=0*/);
                   }
            CloseBase();
}

//----------------------------------------------- Evnmt_GetFils ---------------------------------------------------
int CMoteurBase::Evnmt_GetFils(EVNT_LIST *pEvnmtList, const QString &evnmt_pere)
{ QString requete =QString("SELECT ")          +
                          m_EVNT_LIST_PKDOC    + ", " +     // 0
                          m_EVNT_LIST_PK       + ", " +     // 1
                          m_EVNT_DOSS_PK       + "  " +     // 2
  " FROM "              + m_EVNT_LIST_TBL_NAME +
  " WHERE "             + m_EVNT_LIST_PERE     + "="  +   evnmt_pere;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())
            {  QString evnmt_Pk = query.value( 1 ).toString();           // servira pour rechercher les fils
               //const char* pere, const char* pkDoc, const char* pk, const char* dossPk
               pEvnmtList->append(CRubEvntPair(evnmt_pere, query.value( 0 ).toString(), evnmt_Pk, query.value( 2 ).toString()));
               while (Evnmt_GetFils( pEvnmtList, evnmt_Pk)) ;             // rechercher si ce fils n'est pas le pere d'autres
            }
     }
  else
     {OutSQL_error(query, "Evnmt_GetFils( )" , requete /*=0*/);
     }
  return 0;
}
//----------------------------------------------- Evnmt_UpdateToDisk ---------------------------------------------------
void CMoteurBase::Evnmt_UpdateToDisk(EVNT_LIST *pEvnmtList)
{ if (OpenBase()==0)  return;
  QString requete="";
  QString pk;
  EVNT_LIST::iterator it  = pEvnmtList->begin();
  while ( it !=  pEvnmtList->end())
      {pk = (*it).m_Pk;
       if ( (*it).IsStatePlaced(TO_DELETE) )
          {if (pk[0] != '#')         // si c'est un pk provisoire faut pas l'effacer car nexiste pas
              {requete  = " DELETE FROM ";
               requete +=  m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_LIST_PK + "='" + pk + "'";
               QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
              }
           //........ par contre les fils peuvent tres bien exister si deplaces d'ailleurs................
           Evnmt_EraseAllFils(pEvnmtList, (*it).m_Pk);
           (*it).setState(TO_DESTROY);
           pEvnmtList->Evnmt_Set_StateOn_AllFils(pk, TO_DESTROY);
           ++it;
          }
       else if ( (*it).IsStatePlaced(TO_CREATE))
          {QString oldRefPk = (*it).m_Pk;
           //............... crer un curseur SQL ..................................................
           Q3SqlCursor cur(m_EVNT_LIST_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
           if ( !cur.canInsert ()) {CloseBase(); return;}
           //............... si OK on recupere le buffer et on le charge avec les donnees .........
           QSqlRecord *buffer = cur.primeInsert();                             // recuperer le buffer d'insertion
           buffer->setValue( m_EVNT_LIST_PKDOC,       (*it).m_PkDoc );         // y placer les donnees
           buffer->setValue( m_EVNT_LIST_PERE,        (*it).m_Pere );          // y placer les donnees
           buffer->setValue( m_EVNT_DOSS_PK,          (*it).m_DossPk );        // y placer les donnees
           int ok = cur.insert();                                                  // ecrire le buffer avec les donnees du header
            //................. reselectionner l'enregistrement cree pour retrouver son PK avec le GUID temporaire........................
           if (ok)
              { //................. recuperer dernier primKey insere ..............................
                requete  = "SELECT LAST_INSERT_ID() FROM ";
                requete += m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_LIST_PK + " = LAST_INSERT_ID()";
                QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
                if ( sqlQuery.next() )
                   {(*it).m_Pk =  sqlQuery.value(0).toString();   // placer le pk nouvellement cree sur l'enregistrement et
                    if (oldRefPk[0]=='#') pEvnmtList->Evnmt_SetDefinitivePkEvnt(oldRefPk,(*it).m_Pk); // ## 1 ## sur ceux qui y font reference
                   }
             }
           (*it).removeState(TO_CREATE);
           ++it;
          }
       else if ( (*it).IsStatePlaced(TO_UPDATE))
          {requete  = " UPDATE ";
           requete +=  m_EVNT_LIST_TBL_NAME       + " SET ";
           requete +=  m_EVNT_LIST_PKDOC          + " = '" + (*it).m_PkDoc   + "' ,";
           requete +=  m_EVNT_LIST_PERE           + " = '" + (*it).m_Pere    + "' ,";
           requete +=  m_EVNT_DOSS_PK             + " = '" + (*it).m_DossPk  + "'  ";
           requete +=  " WHERE ";
           requete +=  m_EVNT_LIST_PK             + " = '" + pk  + "'";
           QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
           (*it).removeState(TO_UPDATE);
           ++it;
          }
       else
          {++it;
          }
      }
  //.............. il faut reupdater les eventuels TO_UPDATE places en ## 1 ## par  ............................
  //              pEvnmtList->Evnmt_SetDefinitivePkEvnt(oldRefPk,(*it).m_Pk);
  it  = pEvnmtList->begin();
  while ( it !=  pEvnmtList->end())
      {pk = (*it).m_Pk;
       if ( (*it).IsStatePlaced(TO_UPDATE))
          {requete  = " UPDATE ";
           requete +=  m_EVNT_LIST_TBL_NAME       + " SET ";
           requete +=  m_EVNT_LIST_PKDOC          + " = '" + (*it).m_PkDoc   + "' ,";
           requete +=  m_EVNT_LIST_PERE           + " = '" + (*it).m_Pere    + "' ,";
           requete +=  m_EVNT_DOSS_PK             + " = '" + (*it).m_DossPk  + "'  ";
           requete +=  " WHERE ";
           requete +=  m_EVNT_LIST_PK             + " = '" + pk  + "'";
           QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
           (*it).removeState(TO_UPDATE);
          }
       ++it;
      }
  pEvnmtList->Evnmt_RemoveAllItemsToDestroy();
  CloseBase();
}

//----------------------------------------------- Evnmt_EraseAllFils ---------------------------------------------------
int CMoteurBase::Evnmt_EraseAllFils(EVNT_LIST *pEvnmtList, const QString &evnmt_pere)
{ QString requete = QString("SELECT ") + m_EVNT_LIST_PK + " FROM " + m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_LIST_PERE + "=" + evnmt_pere;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while (query.next())
            {  QString evnmt_PrimKey = query.value( 0 ).toString();           // servira pour rechercher les fils
               if (evnmt_pere[0] != '#') QSqlQuery query2(QString (" DELETE FROM ") + m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_LIST_PERE + "='" + evnmt_pere + "'" , QSqlDatabase::database(m_BaseLabel) );
               while (Evnmt_EraseAllFils( pEvnmtList, evnmt_PrimKey)) ;             // rechercher si ce fils n'est pas le pere d'autres
            }
     }
  else
     {OutSQL_error(query, "Evnmt_EraseFils( )" , requete /*=0*/);
     }
  return 0;
}


//************************************************************************************************************************
//=================================== GESTION DE LA LISTE DES RUBRIQUES ==================================================
//************************************************************************************************************************


//-------------------------------------------- GetPkDoc_Provisoire ---------------------------------------------------------
QString  CMoteurBase::GetPkDoc_Provisoire(RUBREC_LIST *pRubList)
{QString str_pk = "";
 int     lastPk = 0;
 int     pk     = 0;
 RUBREC_LIST::iterator it;
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {str_pk = (*it).m_PrimKey;
      if (str_pk[0]=='#')
         {pk = str_pk.mid(1).toInt();
          if ( pk > lastPk) lastPk = pk;
         }
     } //end while (pSqlQuery->next())
 str_pk  = "#";
 str_pk += QString::number(lastPk+1);
 return  str_pk;
}

//-------------------------------------------- initRubriquesList ---------------------------------------------------------
long  CMoteurBase::initRubriquesList(RUBREC_LIST *pRubList, QString numGUID)
{if  (OpenBase()==0)  return 0;

 QString requete("SELECT ");
 int nb   = 0;

 //............... requete utilisant la base virtuelle .....................................................
 requete +=      m_DOSS_RUB_HEAD_PRIMKEY          + "," +      // 0
                 m_DOSS_RUB_HEAD_NOM_DATE         + "," +      // 1
                 m_DOSS_RUB_HEAD_TYPE             + "," +      // 2
                 m_DOSS_RUB_HEAD_SUBTYPE          + "," +      // 3
                 m_DOSS_RUB_HEAD_DATE             + "," +      // 4
                 m_DOSS_RUB_HEAD_USER             + "," +      // 5
                 m_DOSS_RUB_HEAD_SIGN_USER        + "," +      // 6
                 m_DOSS_RUB_HEAD_REF_BLOBS_PK     + "," +      // 7

                 m_DOSS_RUB_HEAD_FIN              + "," +      // 8

                 m_DOSS_RUB_HEAD_PROP_0           + "," +      // 9
                 m_DOSS_RUB_HEAD_PROP_1           + "," +      // 10
                 m_DOSS_RUB_HEAD_PROP_2           + "," +      // 11
                 m_DOSS_RUB_HEAD_PROP_3           + "," +      // 12
                 m_DOSS_RUB_HEAD_PROP_4           + "," +      // 13
                 m_DOSS_RUB_HEAD_DUREE_MOD        + " " ;      // 14
 requete +=      " FROM ";  // 7
 requete +=  m_DOSS_RUB_HEAD_TBL_NAME         + " WHERE ";
 requete +=  m_DOSS_RUB_HEAD_GUID             + "='"  + numGUID   + "' ORDER BY ";                               //RbDate_Date RbDate_TypeRub='20030000'
 requete +=  m_DOSS_RUB_HEAD_DATE;

 QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
 if (query.isActive())
    {while (query.next())
          {QString sign_user = Utf8_Query(query,6);
           QString user      = Utf8_Query(query,5);
           if (sign_user.length()<=0) sign_user = user;
           pRubList->append(CRubRecord(query.value(0).toString(),         // PrimKey
                                       Utf8_Query(query,1),               // libelle
                                       Utf8_Query(query,2),               // type
                                       Utf8_Query(query,3),               // sous type
                                       query.value(4).toString(),         // date l'enregistrement
                                       user,                              // utilisateur
                                       sign_user,                         // utilisateur signataire
                                       0,                                 // data
                                       0,                                 // len data
                                       query.value(7).toString(),         // reference au PK du Blobs
                                       query.value(8).toString(),         // date de fin l'enregistrement
                                       query.value(14).toInt(),           // duree d'acces en modification du document
                                       Utf8_Query(query,9),               // Propriete NÂ°0
                                       Utf8_Query(query,10),              // Propriete NÂ°1
                                       Utf8_Query(query,11),              // Propriete NÂ°2
                                       Utf8_Query(query,12),              // Propriete NÂ°3
                                       Utf8_Query(query,13)               // Propriete NÂ°4
                                      )
                            );
         ++nb;
        }
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 CloseBase();
 return nb;
}

//-------------------------------------------- GetRecorFromHeadPk ---------------------------------------------------------
long  CMoteurBase::GetRecorFromHeadPk(CRubRecord &record, const QString &headPk, const QString &whereCondition /* = "" */)
{if  (OpenBase()==0)  return 0;
 long nb = 0;
 QString requete("SELECT ");
 //............... requete utilisant la base virtuelle .....................................................
 requete +=      m_DOSS_RUB_HEAD_PRIMKEY          + "," +      // 0
                 m_DOSS_RUB_HEAD_NOM_DATE         + "," +      // 1
                 m_DOSS_RUB_HEAD_TYPE             + "," +      // 2
                 m_DOSS_RUB_HEAD_SUBTYPE          + "," +      // 3
                 m_DOSS_RUB_HEAD_DATE             + "," +      // 4
                 m_DOSS_RUB_HEAD_USER             + "," +      // 5
                 m_DOSS_RUB_HEAD_SIGN_USER        + "," +      // 6
                 m_DOSS_RUB_HEAD_REF_BLOBS_PK     + "," +      // 7

                 m_DOSS_RUB_HEAD_FIN              + "," +      // 8
                 m_DOSS_RUB_HEAD_PROP_0           + "," +      // 9
                 m_DOSS_RUB_HEAD_PROP_1           + "," +      // 10
                 m_DOSS_RUB_HEAD_PROP_2           + "," +      // 11
                 m_DOSS_RUB_HEAD_PROP_3           + "," +      // 12
                 m_DOSS_RUB_HEAD_PROP_4           + "," +      // 13
                 m_DOSS_RUB_HEAD_DUREE_MOD        + " " ;      // 14
 requete +=      " FROM ";
 requete +=      m_DOSS_RUB_HEAD_TBL_NAME         + " WHERE ";
 requete +=      m_DOSS_RUB_HEAD_PRIMKEY          + "='"  + headPk   + "' ";                               //RbDate_Date RbDate_TypeRub='20030000'
 if (whereCondition.length())
    {requete += QString(" AND (%1)").arg( whereCondition );
    }
 QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
 if (query.isActive())
    {while (query.next())
          {QString sign_user = Utf8_Query(query,6);
           QString user      = Utf8_Query(query,5);
           if (sign_user.length()<=0) sign_user = user;
           record.m_PrimKey      = query.value(0).toString();         // PrimKey
           record.m_Libelle      = Utf8_Query(query,1);               // libelle
           record.m_Type         = Utf8_Query(query,2);               // type
           record.m_SubType      = Utf8_Query(query,3);               // sous type
           record.m_Date         = query.value(4).toString();         // date l'enregistrement
           record.m_User         = user;                              // utilisateur
           record.m_SignUser     = sign_user;                         // utilisateur signataire
           record.m_State        = 0;                                 // etat
           record.m_Ref_Blobs_PK = query.value(7).toString();         // reference au PK du Blobs

           record.m_Fin         = query.value(8).toString();         // date de fin l'enregistrement
           record.m_Prop_0      = Utf8_Query(query,9);               // Propriete NÂ°0
           record.m_Prop_1      = Utf8_Query(query,10);              // Propriete NÂ°1
           record.m_Prop_2      = Utf8_Query(query,11);              // Propriete NÂ°2
           record.m_Prop_3      = Utf8_Query(query,12);              // Propriete NÂ°3
           record.m_Prop_4      = Utf8_Query(query,13);              // Propriete NÂ°4
           record.m_DureeMod    = query.value(14).toInt();           // duree d'acces en modification du document
           //record.SetData(0, 0);
           GetDataFromRub(record, record.m_Ref_Blobs_PK);
           ++nb;
          }
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 CloseBase();
 return nb;
}

//-------------------------------------- GetDossPrimKeyFromGUID ---------------------------------------------------
QString CMoteurBase::GetDossPrimKeyFromGUID(const QString &guid, int isBaseMusBeClosed /* = 1 */)
{QString   ret("");
 //............... ouvrir la base  ..................................................
 if  (OpenBase()==0)  return ret;

 //............... crer un curseur SQL ..................................................
 Q3SqlCursor cur(m_DOSS_INDEX_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));                                     // ouvrir un curseur sur la table
 if (!cur.select( m_DOSS_INDEX_GUID+"='"+guid+"'" )) goto  GetDosPrimKeyFromGUID_Error;       // se placer sur le bon record
 if (cur.next())
    {ret = cur.value( m_DOSS_INDEX_PRIM_KEY).toString();
     if (isBaseMusBeClosed==1) CloseBase();
     return ret;
    }
GetDosPrimKeyFromGUID_Error:
     OutSQL_error(cur, "GetDossPrimKeyFromGUID()");
     if (isBaseMusBeClosed==1) CloseBase();
     return ret;
}

//-------------------------------------- initConboBoxWithRubList -------------------------------------------------
/*! \brief Initialise le Q3ComboBox passe en parametre avec les documents de la liste selon leur type.
 * L'utilisation de la classe CPrtQListBoxItem permet et d'inserer un item dans le Q3ComboBox et de lier ces items avec quelques donnees non affichees.

 * Types de documents :
 * 2001-0000 a 2001-9999   --> Courier
 * 2002-0000 a 2002-9999   --> Prescription avec: 2002-0000 a 2002-0000  --> OldPrescription
 *                                                2002-0200 a 2002-0299  --> NewPrescription
 *                                                2002-0300 a 2002-0399  --> NewCertificat
 * 2003-0000 a 2003-9999   --> Observations

 * \return Nombre de documents inseres dans le Q3ComboBox.
 * \sa CPrtQListBoxItem
*/
/*
long  CMoteurBase::initConboBoxWithRubList(RUBREC_LIST *pRubList, Q3ComboBox* pComboBox, QString type)
{    RUBREC_LIST::iterator it;
     long  i = 0;
     int pos = 0;
     pComboBox->clear();
     int num_type = atoi(type);
     for(it = pRubList->begin(); it !=  pRubList->end(); ++it )   // it != m_RecSpeclist.end();
        { if ((*it).m_Type >= type && atoi((*it).m_Type) <= num_type+999)
             {QString qstr = (*it).m_Date.left(10) + " " +(*it).m_Libelle;
              new CPrtQListBoxItem(pComboBox->listBox() ,           // list box du combobox
                                   qstr ,                           // libelle
                                   QString::number(pos) ,           //  position du document dans la liste RUBREC_LIST
                                   (*it).m_Date ,                   //  date document
                                   (*it).m_PrimKey,                 //  pk du document
                                   (*it).m_SubType    );            //  sous type du document
              ++i;
             }
         ++pos;
        } //end while (pSqlQuery->next())
     return i;
}
*/
//-------------------------------------- GetAllRubriqueFromDate -------------------------------------------------
// 2001-0000 a 2001-9999   --> Courier
// 2002-0000 a 2002-9999   --> Prescription avec: 2002-0000 a 2002-0000  --> OldPrescription
//                                                2002-0200 a 2002-0299  --> NewPrescription
//                                                2002-0300 a 2002-0399  --> NewCertificat
// 2003-0000 a 2003-9999   --> Observations
long  CMoteurBase::GetAllRubriqueFromDate(RUBREC_LIST *pRubList, const QDate &qdtRef, QString type/*="ALL_TYP"*/, QStringList *pRubNameList /*=0*/)
{    RUBREC_LIST::iterator it;
     long  i = 0;
     int pos = 0;
     if (pRubNameList) pRubNameList->clear();
     int num_type = atoi(type);
     for(it = pRubList->begin(); it !=  pRubList->end(); ++it )   // it != m_RecSpeclist.end();
        { if ( (type=="ALL_TYP" || ((*it).m_Type >= type && atoi((*it).m_Type) <= num_type+999)))
             { QDate qdtRub( atoi((*it).m_Date.mid(0,4)),
                             atoi((*it).m_Date.mid(5,2)),
                             atoi((*it).m_Date.mid(8,2))
                           );
                if ( qdtRub == qdtRef )
                   {if (pRubNameList)
                       {QString qstr = (*it).m_Date.left(10) + " " +(*it).m_Libelle;
                        qstr.replace ( QChar('\r'), QChar(' ') );
                        qstr.replace ( QChar('\n'), QChar(' ') );
                        qstr.replace ( QChar('\t'), QChar(' ') );
                        qstr += " " + QString::number(pos);
                        pRubNameList->append(qstr);
                       }
                    ++i;
                   }
             }
         ++pos;
        } //end while (pSqlQuery->next())
     return i;
}

//-------------------------------------- ReplaceDataInRubList ---------------------------------------------------------
// ACTION: place des donnees sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const QString & qString, RUBREC_LIST *pRubList, int id)
{  RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant a id
   ReplaceDataInRubList( qString, it );
   /*
   QCString cs    = qString.utf8 ();
   const char *pt = cs;
   if (IsUtf8(  pt ))
      {(*it).SetData (cs.data(), cs.count());
      }
   else
      {(*it).SetData (qString, qString.length());
      }
  */
}

//-------------------------------------- ReplaceDataInRubList ---------------------------------------------------------
// ACTION: place des donnees sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const QString & qString, RUBREC_LIST::iterator it)
{  //RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant a id
   Q3CString cs              = qString.utf8 ();
   const char *pt = cs;
   if (CGestIni::IsUtf8(  pt ))
      {(*it).SetData (cs.data(), cs.count());
      }
   else
      {(*it).SetData (qString, qString.length());
      }
}

//-------------------------------------- ReplaceDataInRubList ---------------------------------------------------------
// ACTION: place des donnees sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const char* text, long len_data, RUBREC_LIST *pRubList, int id)
{RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant a id
 (*it).SetData (text, len_data);
}

//-------------------------------------- ReplaceDataInRubList ---------------------------------------------------------
// ACTION: place des donnees sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const char* text, long len_data,  RUBREC_LIST::iterator it)
{//RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant a id
 (*it).SetData (text, len_data);
}
//-------------------------------------- RubList_SetItemDeleted ---------------------------------------------------------
void  CMoteurBase::RubList_SetItemDeleted(RUBREC_LIST *pRubList, int id)
{(*pRubList->at(id)).m_State |= RUB_IS_TO_DELETE;
}

//-------------------------------------- RubList_SetItemDeleted ---------------------------------------------------------
int  CMoteurBase::RubList_IsItemDeleted(RUBREC_LIST *pRubList, int id)
{return (*pRubList->at(id)).m_State & RUB_IS_TO_DELETE;
}

//-------------------------------------- RubList_SetItemNotDeleted ---------------------------------------------------------
void  CMoteurBase::RubList_SetItemNotDeleted(RUBREC_LIST *pRubList, int id)
{(*pRubList->at(id)).m_State &= ~RUB_IS_TO_DELETE;
}

//-------------------------------------- GetDataFromRub ---------------------------------------------------------
int CMoteurBase::GetDataFromRub(QByteArray &qByteArray, const QString &blobsPrimKey)  // query.value(1).toByteArray() -> m_DOSS_RUB_HEAD_PRIMKEY
{   if (OpenBase()==0)  return 0;
    QString   requete("SELECT ");
    requete += m_DOSS_RUB_DATA_BLOB     + " FROM " + m_DOSS_RUB_DATA_TBL_NAME + " WHERE ";
    requete += m_DOSS_RUB_DATA_PRIMKEY  + "='"     + blobsPrimKey             + "'";

    QSqlQuery query (requete , QSqlDatabase::database(m_BaseLabel) );
    if (query.isActive() && query.next())
       {qByteArray = query.value(0).toByteArray();
        return 1;
       }
    return 0;
}
//-------------------------------------- GetDataFromRubList ---------------------------------------------------------
QString  CMoteurBase::GetDataFromRubList( CRubRecord  *pCRubRecord )
{QString stringDST;
 GetDataFromRubList( stringDST, pCRubRecord);
 return  stringDST;
}

//-------------------------------------- GetDataFromRubList ---------------------------------------------------------
/*! \brief Recupere les donnees d'un enregistrement CRubRecord  *pCRubRecord en lisant la base de donnee.
 *  \param CRubRecord = pointeur vers l'enregistrement dont il faut recuperer les donnees
 *  \param stringDST : QString oÃ¹ seront placees les donnees
 *  \note cette fonction place les donnees dans l'espace pCRubRecord->data() et donc si elle est utilisee
 *        pour recuperer les donnees d'un  RubRecord appartenant a la liste des rubriques : RUBREC_LIST de drtux
 *        ce RubRecord sera considere comme modifie donc a enregistrer.
*/
int CMoteurBase::GetDataFromRubList( QString &stringDST, CRubRecord  *pCRubRecord)
{
 if (pCRubRecord==0) return 0;
 stringDST         = "";
 int           ret = 0;

 if ( pCRubRecord->isEmpty() )
   {QString   requete("SELECT ");
    requete += m_DOSS_RUB_DATA_BLOB     + " FROM " + m_DOSS_RUB_DATA_TBL_NAME + " WHERE ";
    requete += m_DOSS_RUB_DATA_PRIMKEY  + "='"     + pCRubRecord->m_Ref_Blobs_PK + "'";

    if (OpenBase()==0)                           return 0;

    QSqlQuery sqlQuery (requete , QSqlDatabase::database(m_BaseLabel) );
    if (sqlQuery.isActive() && sqlQuery.next())
       { // QMemArray<type> & QMemArray::operator= ( const QMemArray<type> & a )
         // Assigns a shallow copy of a to this array and returns a reference to this array.
         // Equivalent to assign( a ).
         pCRubRecord->append(sqlQuery.value(0).toByteArray());
         //data = sqlQuery.value(0).toByteArray()/*.copy()*/;
         ret = RUB_READ_ON_DISK;
       }
   }
 //....................... SI c'est une rubrique nouvellement cree ou en edition ..............................
 //                        on se sert directement des donnees stockees dans: CRubRecord
 else
   {ret = RUB_READ_IN_LIST;
   }

 char *ptr     = pCRubRecord->data();
 long  len     = pCRubRecord->size();
 if (ptr)
    {if ( len>=10 && CHtmlTools::IsRtf (ptr, 10) )
        {CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len-4, stringDST, 1);
        }
     else
        {if (CGestIni::IsUtf8( ptr )) stringDST = QString::fromUtf8 ( ptr );
         else                         stringDST = ptr;
        }
    }
 return ret;
}


//-------------------------------------- GetDataFromRubList ---------------------------------------------------------
int  CMoteurBase::GetDataFromRubList( QString &stringDST, RUBREC_LIST *pRubList, int id )
{QByteArray data;
 stringDST = "";
 if (id==-1) return -1;
 int   ret = GetDataFromRubList(data, pRubList->at(id));

 if (ret == RUB_READ_ON_DISK || ret == RUB_READ_IN_LIST)      // en fait cela veut dire que ca ete lu sur le disque
    {char *ptr     = data.data();
     long  len     = data.size();
     if (ptr)
        {if ( len>=10 && CHtmlTools::IsRtf (ptr, 10) )
            {CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len-4, stringDST, 1);
            }
         else
            {if (CGestIni::IsUtf8( ptr )) stringDST = QString::fromUtf8 ( ptr );
             else                         stringDST = ptr;
            }
        }
    }
 return ret;
}

//-------------------------------------- GetDataFromRubList ---------------------------------------------------------
int  CMoteurBase::GetDataFromRubList(QByteArray &pQByteArray, RUBREC_LIST *pRubList, int id)
{return GetDataFromRubList(pQByteArray, pRubList->at(id));
}

//-------------------------------------- GetDataFromRubList ---------------------------------------------------------
int  CMoteurBase::GetDataFromRubList(QByteArray &pQByteArray, RUBREC_LIST::iterator it)
{
 int       ret = 0;
 //....................... SI c'est une rubrique nouvellement cree ou en edition ..............................
 //                        on se sert directement des donnees stockees dans: CRubRecord
 if (!(*it).isEmpty())
    {pQByteArray = (*it)             /*.copy()*/;
     return RUB_READ_IN_LIST;
    }
 //....................... SINON on va chercher sur le disque ..............................
 else
   {QString   requete("SELECT ");
    requete += m_DOSS_RUB_DATA_BLOB     + " FROM " + m_DOSS_RUB_DATA_TBL_NAME + " WHERE ";
    requete += m_DOSS_RUB_DATA_PRIMKEY  + "='"     + (*it).m_Ref_Blobs_PK + "'";

    if (OpenBase()==0)  return 0;

    QSqlQuery sqlQuery (requete , QSqlDatabase::database(m_BaseLabel) );
    if (sqlQuery.isActive() && sqlQuery.next())
       {pQByteArray = sqlQuery.value(0).toByteArray()/*.copy()*/;
        ret = RUB_READ_ON_DISK;
       }
    return ret;
   }
}

//-------------------------------------- RubNbToSave ----------------------------------------------------------------------
long  CMoteurBase::RubNbToSave( RUBREC_LIST *pRubList)
{long nb_to_save = 0;
 long          i = 0;
 RUBREC_LIST::iterator it;
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {if ((*it).m_State & RUB_IS_TO_DELETE) // a effacer
         {//.... si nouvelles donnees et mises sur effacer ..............
          //     alors on conptabilise pas
          if ((*it).m_PrimKey !=  "0" && (*it).m_PrimKey[0]!='#') ++nb_to_save;
         }
      else if ((*it).m_PrimKey == "0" || (*it).m_PrimKey[0]=='#')      // nouvelles donnees
         {if (! (*it).isEmpty())
             {++nb_to_save;
             }
         }
      else if (! (*it).isEmpty() )          // si les datas ont ete misee en cache c'est qu'elles on ete modifiees
         {++nb_to_save;
         }
      ++i;
     } //end while (pSqlQuery->next())
 return nb_to_save;
}
//-------------------------------------- GetCRubRecordFromPk ----------------------------------------------------------------------
CRubRecord*  CMoteurBase::RubRecordFromPk( RUBREC_LIST *pRubList, const QString &pk)
{RUBREC_LIST::iterator it;
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {if ( (*it).m_PrimKey ==  pk ) return &(*it);
     } //end while (pSqlQuery->next())
 return 0;
}

//-------------------------------------- RubListSave ----------------------------------------------------------------------
long  CMoteurBase::RubListSave( RUBREC_LIST *pRubList, EVNT_LIST *pEvnmtList, QString numGUID, int mode)
{
 long  i = 0;
 RUBREC_LIST::iterator it;
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {if ((*it).m_State & RUB_IS_TO_DELETE)
         {QString provPk = (*it).m_PrimKey;
          RubListDelete(it, mode);                                         // ==> effacer la rubrique
          pEvnmtList->Evnmt_SetToDelete_Pk(provPk);                  // marquer a effacer tous les enregistrements contenant ce document
          (*it).m_PrimKey = "-1";
         }
      else if ((*it).m_PrimKey == "0"||(*it).m_PrimKey[0]=='#')      // ==>  pk provisoire CAD il faut creer
         {if (! (*it).isEmpty() )
             {QString provPk = (*it).m_PrimKey;
              (*it).m_PrimKey = RubListCreate(it, numGUID, mode);                                  // creer la rubrique
              pEvnmtList->Evnmt_SetDefinitivePkDoc( provPk, (*it).m_PrimKey);     // remplacer le pk provisoire par le vrai dans la liste des liens
             }
         }
      else if (! (*it).isEmpty() )
         {RubListUpdate(it, mode);                                         // ==>  updater la rubrique
         }
      ++i;
     } //end while (pSqlQuery->next())
 Evnmt_UpdateToDisk(pEvnmtList);
 return i;
}

//-------------------------------------- TesteAndCorrigeDossIndexPkFromHeaders ----------------------------------------------------------------------
long  CMoteurBase::TesteAndCorrigeDossIndexPkFromHeaders()
{

  long ok  = 0;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return ok;

   QString guid;
   QString pk;
   QString indexPk;

   QString   requete_1("SELECT ");
             requete_1 += m_DOSS_RUB_HEAD_GUID        + "," +
                          m_DOSS_RUB_HEAD_PRIMKEY     + "," +
                          m_DOSS_RUB_HEAD_REF_DOSS_PK + " FROM " + m_DOSS_RUB_HEAD_TBL_NAME ;
   QSqlQuery sqlQuery_1(requete_1 , QSqlDatabase::database(m_BaseLabel) );

   QString   requete_2;
   QSqlQuery sqlQuery_2( QSqlDatabase::database(m_BaseLabel) );

   if (sqlQuery_1.isActive())
      { while (sqlQuery_1.next())
              {guid    = sqlQuery_1.value(0).toString();
               pk      = sqlQuery_1.value(1).toString();
               if (sqlQuery_1.value(2).toInt()>0) {CloseBase(); return ok;}      // c'est deja convertit
               indexPk = GetDossPrimKeyFromGUID(guid ,0);

               requete_2  = "UPDATE ";
               requete_2 +=  m_DOSS_RUB_HEAD_TBL_NAME          + " SET ";
               requete_2 +=  m_DOSS_RUB_HEAD_REF_DOSS_PK       + " = '" + indexPk  + "' ";
               requete_2 +=  " WHERE ";
               requete_2 +=  m_DOSS_RUB_HEAD_PRIMKEY           + " = '" + pk  + "'";
               sqlQuery_2.exec(requete_2);
               ++ ok;
              }
      }

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//-------------------------------------- RubListCreate ----------------------------------------------------------------------
QString  CMoteurBase::RubListCreate(RUBREC_LIST::iterator it, QString numGUID, int  /*= CMoteurBase::esclave*/)
{
  //............... recuperer le pk de l'index dossier .......................................................
  QString dossIndexPk = GetDossPrimKeyFromGUID(numGUID);
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return QString::null;
  //if (VerrouilleTable(m_DOSS_RUB_DATA_TBL_NAME, QSqlDatabase::database(m_BaseLabel))==0)  return 0;
  //if (VerrouilleTable(m_DOSS_RUB_HEAD_TBL_NAME, QSqlDatabase::database(m_BaseLabel))==0)  return 0;


  /*
  const char *pt_m_User    = (*it).m_User;
  const char *pt_m_Libelle = (*it).m_Libelle;
  const char *pt_m_Date    = (*it).m_Date;
  const char *pt_m_Type    = (*it).m_Type;
  const char *pt_m_data    = (*it);
  */
  QString      ret    = QString::null;
  QString blobs_Pk    = RubListBlobCreate(it, numGUID);
  if (blobs_Pk.length())
     {//............... crer un curseur SQL ..................................................
      Q3SqlCursor cur(m_DOSS_RUB_HEAD_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
      if ( !cur.canInsert ()) {CloseBase();             return ret;}
      (*it).m_Ref_Blobs_PK = blobs_Pk;
      //............... si OK on recupere le buffer et on le charge avec les donnees .........
      QSqlRecord *buffer = cur.primeInsert();                               // recuperer le buffer d'insertion
      buffer->setValue( m_DOSS_RUB_HEAD_GUID,           numGUID               );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_REF_DOSS_PK,    dossIndexPk           );  // y placer les donnees

      buffer->setValue( m_DOSS_RUB_HEAD_USER,           (*it).m_User          );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_SIGN_USER,      (*it).m_SignUser      );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_NOM_DATE,       (*it).m_Libelle       );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_DATE,           (*it).m_Date          );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_TYPE,           (*it).m_Type          );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_SUBTYPE,        (*it).m_SubType       );  // y placer les donnees
      buffer->setValue( m_DOSS_RUB_HEAD_REF_BLOBS_PK,   (*it).m_Ref_Blobs_PK  );

      buffer->setValue( m_DOSS_RUB_HEAD_FIN,            (*it).m_Fin           );      // date de fin l'enregistrement
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_0,         (*it).m_Prop_0        );      // Propriete NÂ°0
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_1,         (*it).m_Prop_1        );      // Propriete NÂ°1
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_2,         (*it).m_Prop_2        );      // Propriete NÂ°2
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_3,         (*it).m_Prop_3        );      // Propriete NÂ°3
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_4,         (*it).m_Prop_4        );      // Propriete NÂ°4
      buffer->setValue( m_DOSS_RUB_HEAD_DUREE_MOD,      (*it).m_DureeMod      );      // duree d'acces en modification du document
      if (!cur.insert())  {CloseBase();             return ret;}

      //................. recuperer dernier primKey insere ..............................
      QString requete ("SELECT LAST_INSERT_ID() FROM ");
      requete += m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_PRIMKEY + " = LAST_INSERT_ID()";
      QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
      if ( sqlQuery.next() )  ret =  sqlQuery.value(0).toString();
      //if (mode==CMoteurBase::progMasterUpdate)
      //   {synchroWriteMofidication(m_DOSS_RUB_HEAD_TBL_NAME, ret,      m_DOSS_RUB_HEAD_PRIMKEY, "C");
      //    synchroWriteMofidication(m_DOSS_RUB_DATA_TBL_NAME, blobs_Pk, m_DOSS_RUB_DATA_PRIMKEY, "C");
      //   }
    }  //SELECT LAST_INSERT_ID() FROM table where id=LAST_INSERT_ID()

  //......................... fermer la base ..............................................
  CloseBase();
  return ret;
}

//-------------------------------------- RubListBlobCreate ----------------------------------------------------------------------
QString  CMoteurBase::RubListBlobCreate(RUBREC_LIST::iterator it, QString numGUID)
{
 Q3SqlCursor cur(m_DOSS_RUB_DATA_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
 if ( !cur.canInsert() )                                           return QString::null;
 //............... se creer un GUID temporaire permettant par la suite de retrouver le Pk de l'enregistrement cree .................
 //............... si OK on recupere le buffer et on le charge avec les donnees ..........................................
 QSqlRecord *buffer = cur.primeInsert();                        // recuperer le buffer d'insertion
 buffer->setValue( m_DOSS_RUB_DATA_BLOB, (*it) );               // y placer les donnees
 buffer->setValue( m_DOSS_RUB_DATA_GUID, numGUID );           // y placer le GUID temporaire (permettra de retrouver l'enregistrement cree)
 if (!cur.insert())                                                return QString::null;                   // ecrire les donnees

 //................. reselectionner l'enregistrement pour retrouver son PK avec le GUID temporaire........................
 //................. recuperer dernier primKey insere ..............................
 QString requete ("SELECT LAST_INSERT_ID() FROM ");
 requete += m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_PRIMKEY + " = LAST_INSERT_ID()";
 QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 QString prim_key = QString::null;
 if ( sqlQuery.next() )  prim_key =  sqlQuery.value(0).toString();
 return prim_key;

 /*
 //............... crer un curseur SQL ...................................................................................
 QSqlCursor cur(m_DOSS_RUB_DATA_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
 if ( !cur.canInsert() )                                           return QString::null;
 //............... se creer un GUID temporaire permettant par la suite de retrouver le Pk de l'enregistrement cree .................
 QString temp_GUID  = GUID_Create();
 //............... si OK on recupere le buffer et on le charge avec les donnees ..........................................
 QSqlRecord *buffer = cur.primeInsert();                        // recuperer le buffer d'insertion
 buffer->setValue( m_DOSS_RUB_DATA_BLOB, (*it) );               // y placer les donnees
 buffer->setValue( m_DOSS_RUB_DATA_GUID, temp_GUID );           // y placer le GUID temporaire (permettra de retrouver l'enregistrement cree)
 if (!cur.insert())                                                return QString::null;                   // ecrire les donnees

 //................. reselectionner l'enregistrement pour retrouver son PK avec le GUID temporaire........................
 QString prim_key = QString::null;
 QString   requete("SELECT ");
 requete += m_DOSS_RUB_DATA_PRIMKEY  + " FROM " + m_DOSS_RUB_DATA_TBL_NAME + " WHERE ";
 requete += m_DOSS_RUB_DATA_GUID     + "='"     + temp_GUID                + "'"; // retrouver le Pk de l'enregistrement cree avec le GUID temporaire
 QSqlQuery sqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
 if (sqlQuery.isActive() && sqlQuery.next()) prim_key = sqlQuery.value(0).toString();

 //................. replacer le bon GUID dans l'enregistrement  .........................................................
 QSqlCursor cur_u(m_DOSS_RUB_DATA_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
 if ( !cur_u.canUpdate ())                                         return QString::null;
 cur_u.select(  m_DOSS_RUB_DATA_PRIMKEY + "=" + prim_key);
 //............... si OK on recupere le buffer et on le charge avec les donnees .........
 if ( cur_u.next() )
    {QSqlRecord *buffer = cur_u.primeUpdate();                      // recuperer le buffer
     buffer->setValue( m_DOSS_RUB_DATA_GUID, numGUID );             // y placer le vrai GUID
     if (cur_u.update()<=0)                                        return QString::null;         // ecrire les donnees
    }
 return prim_key;
 */
}

//-------------------------------------- RubListDelete ----------------------------------------------------------------------
int  CMoteurBase::RubListDelete(RUBREC_LIST::iterator it, int mode /*= CMoteurBase::esclave*/)
{
 int ok  = FALSE;
 //..................... si mode nomade .......................
 //                      on n'efface par l'enregistrement mais on le rend invisible
 //                      en lui collant un utilisateur non possible
 if (mode==CMoteurBase::progMasterUpdate)
    {
     if (OpenBase()==0)  return ok;
     QSqlQuery query(QSqlDatabase::database(m_BaseLabel));
     QString prepare   = "";
     prepare          += "UPDATE " + m_DOSS_RUB_HEAD_TBL_NAME    + " SET " +
                                     m_DOSS_RUB_HEAD_USER        + " = ?," +
                                     m_DOSS_RUB_HEAD_SIGN_USER   + " = ? " +
                         " WHERE " + m_DOSS_RUB_HEAD_PRIMKEY     + " ='"   + (*it).m_PrimKey + "'";
     query.prepare(prepare);
     query.bindValue(0, "--");
     query.bindValue(1, "--");
     if (query.exec())  {ok = TRUE;}
     else               {QString mess = tr("Error : RubListDelete::CMoteurBase::progMasterUpdate \n") + prepare + "\r\n";
                         OutSQL_error(query, prepare);      // rajouter le message sql
                         ok = FALSE;
                        }
     //synchroWriteMofidication(m_DOSS_RUB_HEAD_TBL_NAME, (*it).m_PrimKey,      m_DOSS_RUB_HEAD_PRIMKEY, "D");
     //synchroWriteMofidication(m_DOSS_RUB_DATA_TBL_NAME, (*it).m_Ref_Blobs_PK, m_DOSS_RUB_DATA_PRIMKEY, "D");
     //......................... fermer la base ..............................................
     CloseBase();
     return ok;
    }


 //.............................. ouvrir la base ..........................................................
 if (OpenBase()==0)  return 0;
 //if (VerrouilleTable(m_DOSS_RUB_DATA_TBL_NAME, m_DataBase)==0)  return 0;
 //if (VerrouilleTable(m_DOSS_RUB_HEAD_TBL_NAME, m_DataBase)==0)  return 0;
 //............................. effacer le header .........................................................
  QString requete("DELETE FROM ");
          requete +=  m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_PRIMKEY + "='" +(*it).m_PrimKey + "'";

 QSqlQuery sqlQuery(QString::null , QSqlDatabase::database(m_BaseLabel) );
 ok = sqlQuery.exec(requete);
 if (ok == FALSE) {/*DeVerrouilleTable(m_DataBase);*/ CloseBase(); return FALSE;}        // si erreur cassos

 //............................. effacer le blob ...........................................................
 requete  = "DELETE FROM ";
 requete +=  m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_PRIMKEY + "='" +(*it).m_Ref_Blobs_PK + "'";
 ok = sqlQuery.exec(requete);
 //............................. fermer la base .............................................................
 //DeVerrouilleTable(m_DataBase);
 CloseBase();
 return ok;
}

//-------------------------------------- RubListUpdate ----------------------------------------------------------------------
/*
               m_PrimKey        = primKey;
               m_Libelle        = libelle;
               m_Type           = type;
               m_Date           = date;
               m_User           = user;
               m_pCharData      = 0;        // n'est initialise avec des donnees d'une rubrique que lors modification
               m_LenData        = 0;
               m_State          = 0;

   QString      m_DOSS_RUB_HEAD_TBL_NAME;        // nom de la table de l'header des RUBRIQUES
   QString      m_DOSS_RUB_HEAD_PRIMKEY;         // clef primaire de cet enregistrement
   QString      m_DOSS_RUB_HEAD_GUID;            // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_RUB_HEAD_REF_DOSS_PK;     // Clef primaire du dossier auquel l'enregistrement fait reference (doublon avec m_DOSS_RUB_HEAD_GUID)
   QString      m_DOSS_RUB_HEAD_NOM_DATE;        // Court texte descriptif de l'enregistrement.
   QString      m_DOSS_RUB_HEAD_DATE;            // date de l'enregistrement
   QString      m_DOSS_RUB_HEAD_DUREE;           // duree de l'enregistrement
   QString      m_DOSS_RUB_HEAD_TYPE;            // Type de rubrique
   QString      m_DOSS_RUB_HEAD_USER;            // utilisateur associe a rubrique

*/
int  CMoteurBase::RubListUpdate(RUBREC_LIST::iterator it, int  /*= CMoteurBase::esclave*/)
{ /*............................... methode requete ............................................
  //                       pose un PB avec les valeurs literales comportant une apostrophe
  //                       car sont evaluees comme delimiteurs de valeurs
  int          ok  = FALSE;
  (*it).m_Libelle.replace( QChar('\''), " ");             // sinon erreur syntaxe sql avec "'"
  QString requete  = "UPDATE ";
          requete +=  m_DOSS_RUB_HEAD_TBL_NAME   + " SET ";
          requete +=  m_DOSS_RUB_HEAD_USER       + " = '" + (*it).m_User           + "' , ";
          requete +=  m_DOSS_RUB_HEAD_NOM_DATE   + " = '" + (*it).m_Libelle        + "' , ";
          requete +=  m_DOSS_RUB_HEAD_DATE       + " = '" + (*it).m_Date           + "' , ";
          requete +=  m_DOSS_RUB_HEAD_TYPE       + " = '" + (*it).m_Type           + "' , ";
          requete +=  m_DOSS_RUB_HEAD_USER       + " = '" + (*it).m_User           + "' WHERE ";
          requete +=  m_DOSS_RUB_HEAD_PRIMKEY    + " = '" + (*it).m_PrimKey        + "'";

  if (OpenBase()==0)  return 0;

  QSqlQuery* pSqlQuery = new QSqlQuery(requete , QSqlDatabase::database(m_BaseLabel) );
  if (pSqlQuery && pSqlQuery ->isActive())  ok = TRUE;
  if (ok) ok = RubListBlobUpdate(it);                    // au tour des donnees binaires
  if (pSqlQuery) delete  pSqlQuery;
  CloseBase();

  */
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //if (VerrouilleTable(m_DOSS_RUB_DATA_TBL_NAME, QSqlDatabase::database(m_BaseLabel))==0)  return 0;
  //if (VerrouilleTable(m_DOSS_RUB_HEAD_TBL_NAME, QSqlDatabase::database(m_BaseLabel))==0)  return 0;

  //............... crer un curseur SQL ..................................................
  Q3SqlCursor cur(m_DOSS_RUB_HEAD_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
  if ( !cur.canUpdate ()) {/*DeVerrouilleTable(QSqlDatabase::database(m_BaseLabel)); */CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_DOSS_RUB_HEAD_PRIMKEY + "=" + (*it).m_PrimKey);
  //............... si OK on recupere le buffer et on le charge avec les donnees .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer ( pas de delete a la fin !!)
     buffer->setValue( m_DOSS_RUB_HEAD_USER,       (*it).m_User     ); // y placer les donnees
     buffer->setValue( m_DOSS_RUB_HEAD_SIGN_USER,  (*it).m_SignUser ); // y placer les donnees
     buffer->setValue( m_DOSS_RUB_HEAD_NOM_DATE,   (*it).m_Libelle  ); // y placer les donnees
     buffer->setValue( m_DOSS_RUB_HEAD_DATE,       (*it).m_Date     ); // y placer les donnees
     buffer->setValue( m_DOSS_RUB_HEAD_TYPE,       (*it).m_Type     ); // y placer les donnees
     buffer->setValue( m_DOSS_RUB_HEAD_SUBTYPE,    (*it).m_SubType  ); // y placer les donnees

     buffer->setValue( m_DOSS_RUB_HEAD_FIN,            (*it).m_Fin           );      // date de fin l'enregistrement
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_0,         (*it).m_Prop_0        );      // Propriete NÂ°0
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_1,         (*it).m_Prop_1        );      // Propriete NÂ°1
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_2,         (*it).m_Prop_2        );      // Propriete NÂ°2
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_3,         (*it).m_Prop_3        );      // Propriete NÂ°3
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_4,         (*it).m_Prop_4        );      // Propriete NÂ°4
     buffer->setValue( m_DOSS_RUB_HEAD_DUREE_MOD,      (*it).m_DureeMod      );      //duree d'acces en modification du document
     cur.update();                                                  // ecrire le buffer avec les donnees du header
     ok = RubListBlobUpdate(it);                                    // s'occuper des donnees binaires BLOB
     //if (mode==CMoteurBase::progMasterUpdate)
     //   {synchroWriteMofidication(m_DOSS_RUB_HEAD_TBL_NAME, (*it).m_PrimKey,      m_DOSS_RUB_HEAD_PRIMKEY, "U");
     //    synchroWriteMofidication(m_DOSS_RUB_DATA_TBL_NAME, (*it).m_Ref_Blobs_PK, m_DOSS_RUB_DATA_PRIMKEY, "U");
     //   }

    }
  //......................... fermer la base ..............................................
  //DeVerrouilleTable(QSqlDatabase::database(m_BaseLabel));
  CloseBase();
  return ok;
}

//-------------------------------------- RubListBlobUpdate ----------------------------------------------------------------------
/*
   QString      m_DOSS_RUB_DATA_TBL_NAME;        // nom de la table des donnees des RUBRIQUES
   QString      m_DOSS_RUB_DATA_PRIMKEY;         // clef primaire de cet enregistrement  (le meme que le header)
   QString      m_DOSS_RUB_DATA_BLOB;            // donnees binaires
   int          m_DOSS_RUB_DATA_FP_PRIMKEY;      // position du champ clef primaire de cet enregistrement
   int          m_DOSS_RUB_DATA_FP_BLOB;         // position du champ donnees binaires  de l'enregistrement
*/

int  CMoteurBase::RubListBlobUpdate(RUBREC_LIST::iterator it)
{//............... crer un curseur SQL ..................................................
 Q3SqlCursor cur(m_DOSS_RUB_DATA_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
 if ( !cur.canUpdate ()) return FALSE;
 cur.select(  m_DOSS_RUB_DATA_PRIMKEY + "=" + (*it).m_Ref_Blobs_PK);
 //............... si OK on recupere le buffer et on le charge avec les donnees .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer
     //char *pt = (char*)(const char*)(*it);
     buffer->setValue( m_DOSS_RUB_DATA_BLOB, (*it) );               // y placer les donnees
     cur.update();                                                  // ecrire les donnees
     //delete buffer; A NE SURTOUT PAS FAIRE !!!!
    }
 return TRUE;
}

//**********************************************************************************************************************************
//=================================== GESTION DU VERROUILLAGE DES BASES ============================================================
//**********************************************************************************************************************************
//------------------------------------------------ VerrouilleDossier  --------------------------------------------------------------
int CMoteurBase::VerrouilleDossier     ( const QString &dossGUID,
                                         const QString &userName,
                                         QString &usedBy
                                       )
{ usedBy =  VerrouilleRubrique("", dossGUID, "All Rubriques", "", userName, "" );
  if (usedBy=="+") return 1;

  return 0;
}

//------------------------------------------------ VerrouilleTable  --------------------------------------------------------------
int CMoteurBase::VerrouilleTable(const QString &tableName, QSqlDatabase* dataBase)
{QSqlQuery query(QString("LOCK TABLES ") + tableName + " WRITE", *dataBase);
 if (query.isActive()) return 1;
 return 0;
}

//------------------------------------------------ DeVerrouilleTable  --------------------------------------------------------------
int CMoteurBase::DeVerrouilleTable(QSqlDatabase* dataBase)
{QSqlQuery query("UNLOCK TABLES  ", *dataBase);
 if (query.isActive()) return 1;
 return 0;
}

//------------------------------------------------ IsDossierVerrouille  --------------------------------------------------------------
int CMoteurBase::IsDossierVerrouille   ( const QString &dossGUID)
{ QString usedBy;
  return IsDossierVerrouille   ( dossGUID, usedBy );
}

//------------------------------------------------ IsDossierVerrouille  --------------------------------------------------------------
int CMoteurBase::IsDossierVerrouille   ( const QString &dossGUID, QString &usedBy )
{ if (OpenBase()==0)  return -1;  // si point = erreur d'ouverture de la base
  //QString dateTimeR;
  if (dossGUID.length()==0) return -1; // si GUID vide (cas d'un nouveau dossier) retourner modif valide
  //.................. verifier si pas deja utilise ......................................................................................
  //                   et si c'est le cas retourner le proprietaire
  QString requete("");
  requete += QString("SELECT ") + m_VERROU_USER_NAME + " FROM " + m_VERROU_TBL_NAME + " WHERE " + m_VERROU_DOSS_GUID  + " = '" +   dossGUID + "'";
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
    {usedBy = Utf8_Query(query,0);    // retourne l'utilisateur deja proprietaire
     return 1;                         // et cassos car c'est deja verrouille par un autre
    }
  usedBy = "";
  return 0;
}

//------------------------------------------------ VerrouilleRubrique --------------------------------------------------------------
QString CMoteurBase::VerrouilleRubrique( const QString &dossPk,
                                         const QString &dossGUID,
                                         const QString &rubName,
                                         const QString &rubPk,
                                         const QString &userName,
                                         const QString &dateTime
                                       )
{if (OpenBase()==0)  return QString(".");  // si point = erreur d'ouverture de la base
 QString ret("");
 QString dateTimeR;
 if (dossGUID.length()==0) return QString("+"); // si GUID vide (cas d'un nouveau dossier) retourner modif valide
 //.................. verifier si pas deja utilise ......................................................................................
 //                   et si c'est le cas retourner le proprietaire
 QString requete("");
 requete += QString("SELECT ") + m_VERROU_USER_NAME + " FROM " + m_VERROU_TBL_NAME + " WHERE " + m_VERROU_DOSS_GUID  + " = '" +   dossGUID + "'";
 QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
 if (query.isActive() && query.next())
    {ret = query.value(0).toString();    // retourne l'utilisateur deja proprietaire
     return ret;                         // et cassos car c'est deja verrouille par un autre
    }

 //.................. si pas deja verrouille inserer notre verrou .......................................................................
 //.................. verouiller la table ................................................
 query.exec(QString("LOCK TABLES ") + m_VERROU_TBL_NAME + " WRITE");
 //.................. crer un curseur SQL ................................................
 Q3SqlCursor cur(m_VERROU_TBL_NAME, TRUE, QSqlDatabase::database(m_BaseLabel));
 if ( !cur.canInsert ())                                    return QString(".");
 QDateTime dt ( QDate::currentDate (), QTime::currentTime() );
 if (dateTime.length()==0) dateTimeR = dt.toString("yyyyMMddhhmmsszzz");
 else                      dateTimeR = dateTime;
 //............... si OK on recupere le buffer et on le charge avec les donnees ..........
 QSqlRecord *buffer = cur.primeInsert();
 buffer->setValue( m_VERROU_DOSS_PK,     dossPk.toInt()                     );  // y placer les donnees
 buffer->setValue( m_VERROU_DOSS_GUID,   dossGUID                           );  // y placer les donnees
 buffer->setValue( m_VERROU_RUB_NAME,    rubName                            );  // y placer les donnees
 buffer->setValue( m_VERROU_RUB_PK,      rubPk.toInt()                      );  // y placer les donnees
 buffer->setValue( m_VERROU_USER_NAME,   userName+"::"+QHostInfo::localHostName ()+"::"+QHostInfo::localDomainName ()                           );  // y placer les donnees
 buffer->setValue( m_VERROU_DATE_TIME,   dateTimeR                          );  // y placer les donnees
 if (cur.insert())   ret = "+";
 else                ret = "-";
 //................. deverouiller la table ................................................................................................
 query.exec("UNLOCK TABLES ");
 return ret;
}

//------------------------------------------------ DeVerrouilleDossier --------------------------------------------------------------
void    CMoteurBase::DeVerrouilleDossier ( const QString &dossGUID)
{DeVerrouilleRubrique("",dossGUID,"", "","","" );
}

//------------------------------------------------ DeVerrouilleRubrique --------------------------------------------------------------
void    CMoteurBase::DeVerrouilleRubrique( const QString &/*dossPk*/,
                                           const QString &dossGUID,
                                           const QString &/*rubName*/,
                                           const QString &/*rubPk*/,
                                           const QString &/*userName*/,
                                           const QString &/*dateTime*/
                                       )
{if (OpenBase()==0)  return ;  // si point = erreur d'ouverture de la base
 //................. verouiller la table ..................................................................................................
 QSqlQuery query(QString("LOCK TABLES ") + m_VERROU_TBL_NAME + " WRITE", QSqlDatabase::database(m_BaseLabel));
 //............................. effacer le header .........................................................
 query.exec(QString("DELETE FROM ")      + m_VERROU_TBL_NAME + " WHERE " + m_VERROU_DOSS_GUID + "='" + dossGUID + "'");
 //................. deverouiller la table ................................................................................................
 query.exec("UNLOCK TABLES ");
}

//------------------------------------------------ DeVerrouilleTout --------------------------------------------------------------
void    CMoteurBase::DeVerrouilleTout(  )
{if (OpenBase()==0)  return ;  // si point = erreur d'ouverture de la base
 //................. verouiller la table ..................................................................................................
 QSqlQuery query(QString("LOCK TABLES ") + m_VERROU_TBL_NAME + " WRITE", QSqlDatabase::database(m_BaseLabel));
 //............................. effacer le header .........................................................
 query.exec(QString("DELETE FROM ")      + m_VERROU_TBL_NAME );
 //................. deverouiller la table ................................................................................................
 query.exec("UNLOCK TABLES ");

}
//**********************************************************************************************************************************
//=================================== GESTION DES IDENTIFICATEURS UNIQUES GUID, PASSWORD ===========================================
//**********************************************************************************************************************************

//-----------------------------------------------------  PassWordEncode --------------------------------------------------
// Algo minimal pour l'instant
// chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
// resultat final en hexa;
int CMoteurBase::PassWordEncode(QString &pass)
{char str[34];
 quint16    val;
 int len         = pass.length();

 if (len>32)       return ERR_PASS_MAX;
 if (len==0)       return ERR_PASS_ZERO;
 strcpy(str , pass);
 pass = "";
 for (int i=0; i<len; ++i)
     { val   =  str[i];
       val   =  (val << ((i+1)&0x000F) )^val;
      pass  += UINTtoHex(val);
     }
 return ERR_PASS_OK;
}

//-----------------------------------------------------  PassWordDecode --------------------------------------------------
// Algo minimal pour l'instant
int CMoteurBase::PassWordDecode(const char* str_to_decode, const char* str_pass)
{char     buf[16];
 quint16   val;
 if (str_to_decode == 0 || str_pass == 0) return ERR_PASS_MAX;
 int len_to_decode      = strlen(str_to_decode);
 if (len_to_decode==0)             return ERR_PASS_ZERO;
 int len_pass           = strlen(str_pass);
 if (len_pass != len_to_decode/4)  return ERR_PASS_MAX;
 QString result;
 int end = len_to_decode/4;
 buf[4 ] = 0;
 for (int i=0; i<end; ++i)
     {memcpy(buf, str_to_decode + i*4, 4);  // recuperer les 4 caractere hexa
      val = HexToUINT(buf);                 // les convertir en quint16
      val = val ^ str_pass[i];              // xor avec le mot de passe
      val = val >> ((i+1)&0x000F);          // rotation inverse maxi de 8 et on devrait avoir le code ASCII
      if (val != str_pass[i])      return ERR_PASS_MAX;    // erreur si pas egal
     }
 return ERR_PASS_OK;
}

//-----------------------------------------------------  UINTtoHex --------------------------------------------------------
QString CMoteurBase::UINTtoHex( quint16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   // la on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
quint16 CMoteurBase::HexToUINT( const char *str)
{int i   = strlen(str);
 if (i==0) return 0;
 quint16 val = 0;
 int    dec = 0;
 while (i>0)
   {--i;
    unsigned char car = str[i] | 32;     // mettre en minuscules
    if      (car>='a'&& car<= 'f')  val  += (car-'W')<<dec;  //a (97) - W (87) --> 10
    else                            val  += (car-'0')<<dec;  //0 (48) - 0 (48) --> 0
    dec += 4;
   }
 return val;
}

//-----------------------------------------------------  GUID_Create --------------------------------------------------
QString CMoteurBase::GUID_Create()
{

    if (get_GUID_CreateMode()==CMoteurBase::byQT)
       {QString guid ( QUuid::createUuid ().toString());
        return  guid.mid(1, 36);
       }
    else
       {
        return C_MyUuid::create();     // version personnelle de creation du GUID car celle de QT buguee? sur certaines plateformes
       }
}

//-----------------------------------------------------  get_GUID_CreateMode --------------------------------------------------
CMoteurBase::GUID_Mode CMoteurBase::get_GUID_CreateMode()
{return  m_GUID_CreateMode;
}

//-----------------------------------------------------  CalculeDiffAge_Num -------------------------------------------------
// ACTION: calcule l'age entre deux dates de debut et de fin.
// ENTREE: QDate *pDebut: pointe sur une QDate de debut (exemple date de naissance)
//         QDate *pFin:   pointe sur une QDate de  fin (peut etre zero alors il sera pris la date en cours)
int CMoteurBase::CalculeDiffAge_Num(QDate *pDebut, QDate *pFin /*=0*/, int *mois_ret /*=0*/, int *jours_ret /*=0*/)
{
 char finMois[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 int an   = 0;
 int mois = 0;
 int jour = 0;
 int dayYearDebut;
 int dayYearFin;

 bool bFinAlloue = FALSE;
 long lEtatDayYear;

 if (pDebut && pDebut->isValid ())
    {
     if (pFin==NULL)
        {pFin       = new QDate (QDate::currentDate ());
         bFinAlloue = TRUE;
        }//if

     if (pFin->isValid ())
        {
         //.........................................Partie calcul ..........................................
         //........Le jour de l'annee .................
         dayYearDebut = pDebut->dayOfYear ();
         dayYearFin   = pFin->dayOfYear ();
         //................................ bisextiles and cie ..................................................
         //ATTENTION A L'ANNEE BISEXTILLE (sauf en l'an 1900, car les debuts de siecles ne sont pas bisextille sauf les debuts de millenaire
         //60eme jour => 29 Fevrier
         if (pDebut->year()%4==0 && pDebut->year()%100 && pDebut->year()%1000    &&  dayYearDebut>60 )
             dayYearDebut--;
         if (pFin->year()%4==0   && pFin->year()%100   && pFin->year()%1000      &&  dayYearFin>60)
             dayYearFin--;

          //....................Position des jours l'un par rapport a l'autre ....................................
          if      (dayYearFin<dayYearDebut)     lEtatDayYear = -1;
          else if (dayYearFin>dayYearDebut)     lEtatDayYear =  1;
          else                                  lEtatDayYear =  0;

          //...................Calcul annee (-1 si le jour de l'annee en cours est <) .............................
          an = pFin->year() - pDebut->year() - (lEtatDayYear<0? 1 :0);

          if (lEtatDayYear!=0)
             {
              //.......................Calcul mois .....................................
              if (lEtatDayYear<0)
                  mois = 12 - pDebut->month() + pFin->month()   - (pFin->day()<pDebut->day()? 1 :0);
              else
                  mois = pFin->month()        - pDebut->month() - (pFin->day()<pDebut->day()? 1 :0);

              //...................... Calcul du jour ...................................
              if (pFin->day() < pDebut->day())
                  jour=(finMois[pFin->month()>1 ? pFin->month()-2 : 11] - pDebut->day()) + pFin->day();
              else
                  jour=pFin->day() - pDebut->day();
             } //if(lEtatDayYear!=0)
        } // endif (pFin->isValid ())
     }// endif (pDebut && pDebut->isValid ())
     if(bFinAlloue)  delete pFin;
     if (mois_ret)  *mois_ret  = mois;
     if (jours_ret) *jours_ret = jour;
     return an;
}



//-----------------------------------------------------  CalculeDiffAge -------------------------------------------------
// ACTION: calcule l'age entre deux dates de debut et de fin.
// ENTREE: QDate *pDebut: pointe sur une QDate de debut (exemple date de naissance)
//         QDate *pFin:   pointe sur une QDate de  fin (peut etre zero alors il sera pris la date en cours)
//         bool bTexte:   TRUE   alors la date sera exprimee comme: 4 ans 6 mois 3 jours
//                        FALSE  alors la date sera exprimee comme un chiffre
//         int bAbrege:   2  et alors seules les annees seront sorties sous la forme: "0" ou "35" etc ...
//                        1  et alors seules les annees seront sorties sous la forme: "0" ou "35" etc ...
//                        0  et alors les sorties seront detaillees avec si age<3 avec les mois et jours:
QString CMoteurBase::CalculeDiffAge(QDate *pDebut, QDate *pFin/*=NULL*/, bool bTexte/*=TRUE*/, int bAbrege/*=1*/)
{
 int an   = 0;
 int mois = 0;
 int jour = 0;
 QString buffer;
 QString sRes("");


 an = CalculeDiffAge_Num(pDebut, pFin , &mois, &jour);
 //.............................................. Partie texte .........................................
 if (an)
    {
     if (bTexte)
     //buffer.sprintf("%d an%s ", an, an>1 ? "s" : "");
         buffer = QString::number(an) + tr(" year") + ((an>1) ? "s " : " ");
     else
         buffer.sprintf("%d ", an);
     sRes = buffer;
    } //if (an)
          //.................... si bAbrege==2 alors on ne sort que les annees .......................................
 if (bAbrege==2)
    {if (an==0) sRes="0";
     return sRes;
    }
 if (!bAbrege || an < 3 /* || an<3*/) // age Inferieur a 3 ans et version detaillee
    {
     if (mois || !bTexte)
        {
         if (bTexte)
         //buffer.sprintf("%d mois ", mois);  // en fancais " mois " a deja un s en [4] donc ne pas rajouter si plusieurs
            {QString ms = tr(" month");  // va bien y avoir un dialecte oÃ¹ ca ne va pas marcher // C pour ca qu'on a invente les plural forms
             buffer     = QString::number(mois)+ ms + ((mois>1&&ms[4]!='s') ? "s " : " ");
            }
         else
             buffer.sprintf("%d ", mois);
         sRes+=buffer;
        } // if (mois || !bTexte)

     if (jour || !bTexte)
        {
         if (bTexte)
         //buffer.sprintf("%d jour%s ", jour, jour>1 ? "s" : "");
            buffer = QString::number(jour) + tr(" day") + ((jour>1) ? "s " : " ");
         else
            buffer.sprintf("%d ", jour);
         sRes += buffer;
        } // if  (jour || !bTexte)
     }// if (!bAbrege || an<3) //Inferieure a 3 ans
 return sRes;
}


//------------------------------ GotoNextDebLigne -----------------------------------------
char *CMoteurBase::GotoNextDebLigne(const char* txt)
{char *pt  = (char*) txt;
 while (*pt)
    {if      ( pt [0]=='\\'&& pt [1] !=0)            (pt+=2);
     else if ( pt [0]=='\r'&& pt [1] =='\n')  return (pt+=2);
     else if ( pt [0]=='\n'&& pt [1] =='\r')  return (pt+=2);
     else if ( pt [0]=='\n'|| pt [0] =='\r')  return (pt+1);
     else pt++;
    }
 return pt;
}

//------------------------------ GotoEndOfLigne -----------------------------------------
char *CMoteurBase::GotoEndOfLigne(const char* txt)
{char *pt  = (char*) txt;
 while (*pt)
    {if      ( pt [0]=='\\'&& pt [1] !=0)            (pt+=2);
     else if ( pt [0]=='\r'&& pt [1] =='\n')  return pt;
     else if ( pt [0]=='\n'&& pt [1] =='\r')  return pt;
     else if ( pt [0]=='\n'|| pt [0] =='\r')  return pt;
     else pt++;
    }
 return pt;
}

//------------------------------ GotoNextSignifiant -----------------------------------------
char *CMoteurBase::GotoNextSignifiant(const char* txt)
{char *pt  = (char*) txt;
 while (*pt && (*pt==' ' || *pt=='\t'))
    {pt++;
    }
 return pt;
}

/*------------------------------ RTF_ToASCII ---------------------------------------*/
QString CMoteurBase::RTF_ToASCII(const char *ptIn, long len)
{//........ une petite QString de destination ....................
 QString stringDST;
 int fonttbl=0;
 //........ recuperer un pointeur sur la source ...................
 char *pt = (char*) ptIn;
 if (pt==0)             return 0;
 if (len==0) len = strlen(pt);
 if (len<=0)            return 0;
 char *end_rtf   = pt + len;

 /*............ aller a la premiere acolade ..............*/
 while(pt<end_rtf && *pt != '{') pt++;
 pt++;
 /*........scan et analyse du texte .......................*/
 while(pt<end_rtf)
   {int car = *pt;
    switch(car)
       {case '\r':
        case '\n':
        case '}':
             if (fonttbl) fonttbl--;
             pt++;
             break;
        case '{':
             if (fonttbl) fonttbl++;
             pt++;
             break;
        case  '\\':                // anti Slash
             if (pt[1]=='o' && (strncmp(pt+1,"object",6)==0))
                {pt = JumpObject(pt, end_rtf);
                }
             else if (strncmp(pt+1,"fonttbl",7)==0)
                {fonttbl++;
                 pt += 7;
                }
             else if (strncmp(pt+1,"fnil",4)==0     ||
                      strncmp(pt+1,"froman",6)==0   ||
                      strncmp(pt+1,"fswiss",6)==0   ||
                      strncmp(pt+1,"fmodern",7)==0  ||
                      strncmp(pt+1,"fscript",7)==0  ||
                      strncmp(pt+1,"fdecor",6)==0   ||
                      strncmp(pt+1,"ftech",5)==0    ||
                      strncmp(pt+1,"snext",5)==0    ||
                      strncmp(pt+1,"colortbl",8)==0 ||
                      strncmp(pt+1,"red",3)==0      ||
                      strncmp(pt+1,"green",5)==0    ||
                      strncmp(pt+1,"blue",4)==0
                     )
                {while(pt<end_rtf && *pt != ';') pt++;
                 pt++;
                }
             else if (strncmp(pt+1,"tab",3)==0    // et le tag 'tab' est rencontre
                     )
                {stringDST += "\t";
                 pt    += 4;
                 if (*pt==' ') pt++;
                }
             else if (pt[4] !='d'&&               // faut pas que ce soit 'pard'
                      strncmp(pt+1,"par",3)==0    // mais 'par' isole
                     )
                {stringDST +="\r\n";
                 pt  += 4;
                 if (*pt==' ') pt++;
                }
             else if (pt[1]== '\'')     // apostrophe
                {char val;
                 if (pt[2]>'a'&&pt[2]<='f')      val  = (pt[2]-'W')*16;
                 else if (pt[2]>'A'&&pt[2]<='A') val  = (pt[2]-'W')*16;
                 else                            val  = (pt[2]-'0')*16;
                 if (pt[3]>'a'&&pt[3]<='f')      val += (pt[3]-'W');
                 else if (pt[2]>'A'&&pt[2]<='A') val += (pt[3]-'W');
                 else                            val += (pt[3]-'0');
                 stringDST += val;
                 pt  +=4;
                }
             else
                {pt++;
                 while  (pt<end_rtf && IsRtfControlCar(pt)) pt++;
                 if (*pt==' ') pt++;
                }
             break;
         default:
             if (fonttbl==0) stringDST += pt[0];
             pt++;
       }   /* endswitch */
   }
 return stringDST;
}

/*------------------------------ JumpObject ----------------------------------------*/
char* CMoteurBase::JumpObject(char *pt, char *end_rtf)
{long acco=1;
 while(pt < end_rtf && acco)
   {int car = *pt;
    switch(car)
       {case '}':
             acco--;
             pt++;
             break;
        case '{':
             acco++;
             pt++;
             break;
        default:
             pt++;
       } /* endswitch(car) */
   } /*endwhile(pt < end_rtf && acco) */
 return pt;
}

/*------------------------------ IsRtfControlCar --------------------------------------*/
int CMoteurBase::IsRtfControlCar(char *pt)
{if       (*pt >='a' && *pt<= 'z') return 1;
 else if  (*pt >='A' && *pt<= 'Z') return 1;
 else if  (*pt >='0' && *pt<= '9') return 1;
 return 0;
}

//-----------------------------------------------------  SQL_WRITE -------------------------------------------
/*! \brief Fonction qui permet de mettre a jour ou inserer des donnees dans une table SQL Si l'enregistrement existe deja il est mis a jour sinon il est insere
 *  \param arg_list const &QStringList  : reference sur la liste des arguments de la fonction
 *                       premier  argument : nom de la table a mettre a jour
 *                       deuxieme argument : condition WHERE (la mention WHERE ne doit pas etre fournie) pour selectionner les enregistrements a mettre a jour
 *                       troisieme arguement : nom du champ a mettre a jour doit etre suivi du
 *                       quatrieme arguement : valeur a mettre a jour dans ce champ.
 *                              .
 *                              .
 *                       nieme argument    : nom du champ a mettre a jour doit etre suivi du
 *                       nieme argument +1 : valeur a placer dans ce champ
 *  \return        une chaine vide si tout est Ok, sinon un message d'erreur.
 */
QString CMoteurBase::SQL_WRITE(const QStringList &arg_list)
{  int nbArg = arg_list.count();
   int     i = 0;
   if (nbArg<1)  return QObject::tr("Syntax error in  SQL_WRITE() first argument table name not found");
   if (nbArg<2)  return QObject::tr("Syntax error in  SQL_WRITE() second argument 'where condition' not found");
   if (nbArg<3)  return QObject::tr("Syntax error in  SQL_WRITE() third argument 'first field name' fnot found");
   if (nbArg<4)  return QObject::tr("Syntax error in  SQL_WRITE() forth argument 'first field value' fnot found");

   //.......... si la condition est vraie on update ....................................
   QString tbl_name  = arg_list[0];
   QString str_where = arg_list[1]; str_where.replace("&#44;",","); str_where.replace("&#40;","("); str_where.replace("&#41;",")");
   QString prepare   = "";                                                  //  first field , tbl name , where condition
   QString requete   = QString("SELECT `%1` FROM `%2` WHERE %3 LIMIT 0,1").arg( arg_list[2] , tbl_name , str_where );   // un seul resultat est suffisant pour evaluer si record existe deja
   QSqlQuery sqlQuery_i (requete , QSqlDatabase::database(m_BaseLabel) );
   bool update = (sqlQuery_i.isActive() && sqlQuery_i.next());
   if (update)
      { prepare          = QString("UPDATE `%1` SET     \n").arg(tbl_name);
              i          = 2;    // on part du nom du champ (0=nom table 1=where condition 2=nom premier champ 3=valeur du premier champ ..... )
        while (i<nbArg)
            { prepare   +=  arg_list[i].trimmed()  + "=?,\n";   // recuperer le nom du premier champ
              i         += 2;                                           // champ suivant
            }
        prepare.truncate(prepare.length()-2);   // on vire le dernier ",\n"
        prepare         += "\n";                // on le remplace par  "\n"
        prepare         += QString(" WHERE %1 ;").arg(str_where);
      }
   else
      { prepare          = QString("INSERT INTO `%1`  (\n").arg(tbl_name);
              i          = 2;     // on part du nom du champ (0=nom table 1=where condition 2=nom premier champ 3=valeur du premier champ ..... )
        while (i<nbArg)
            { prepare   +=  arg_list[i].trimmed()  + ",\n";    // recuperer le nom du premier champ
              i         += 2;                                          // champ suivant
            }
        prepare.truncate(prepare.length()-2);   // on vire le dernier ",\n"
        prepare         += "\n";                // on le remplace par "\n"
        prepare         += " )\n  VALUES (";
        i                = 2;
        while (i<nbArg)
             { prepare  += "?,";
               i        += 2;
             }
        prepare.truncate(prepare.length()-1);   // on vire le dernier ","
        prepare          += ");";
      }

   //......... les deux options UPDATE ou INSERT sont pretes pour le bind ..............
   QString err = "";
   QSqlQuery query( QSqlDatabase::database(m_BaseLabel) );
   if (!query.prepare(prepare))
      { err = QObject::tr("ERREUR  : SQL_WRITE (%1)  query.prepare <br/> %2").arg(update?"UPDATE":"INSERT",prepare);
        qDebug(err);
        return err;
      }
   i     = 3;   // on part de la valeur du champ (0=nom table 1=where condition 2=nom premier champ 3=valeur du premier champ ..... )
   int n = 0;
   while (i<nbArg)
         { query.bindValue(n, arg_list[i]); ++n;
           i += 2;
         }
   if (!query.exec())
      { err = QObject::tr("ERREUR  : SQL_WRITE (%1)  query.exec <br/> %2").arg(update?"UPDATE":"INSERT",prepare);
        qDebug(err);
        return err;
      }
   return "";
}
