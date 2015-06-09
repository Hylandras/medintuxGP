/********************************* CMoteur_Base.cpp *******************************
 *                                                                                *
 *   #include "CMoteur_Base.h"                                                    *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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
#include <stdlib.h>

#include <qapplication.h>
#include <qprocess.h>
#include <qwaitcondition.h>
#include <qmessagebox.h>
#include <math.h>
#include <quuid.h>
#include <qeventloop.h>
#include <qsqldriver.h>


#include "../../MedinTuxTools/CHtmlTools.h"
#include "CMoteur_Base.h"
#include "Global.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
#include "../../MedinTuxTools/CPrtQListViewItem.h"
#include "../../MedinTuxTools/CGestIni.h"


#define TR  QObject::tr
#define VERSION_BASE "02.16.000"
#define MAX_READ      32000
//=============================================== IMPLEMENTATION =====================================================
//-----------------------------------------------------  CMoteurBase -------------------------------------------
/**
 *
 * @param driver nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
 * @param DataBaseName
 * @param user
 * @param password
 * @param hostname
 * @param confFile
 * @param baseLabel
 * @param errMess
 * @return
 */
CMoteurBase::CMoteurBase(const QString & driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                         const QString & DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                         const QString & user,          // = "root"
                         const QString & password,      // = ""
                         const QString & hostname,
                         const QString & port,
                         const QString & confFile,
                         const QString & baseLabel,
                         QString*    errMess,
                         QObject*    parent,
                         const QString & name)
:QObject(parent,name)
{ initBase   ( driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
               DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
               user,          // = "root"
               password,      // = ""
               hostname,
               port,
               confFile,
               baseLabel,
               errMess);
}

//-----------------------------------------------------  ~CMoteurBase -------------------------------------------
CMoteurBase::~CMoteurBase()
{killBase();
}
//-----------------------------------------------------  killBase -------------------------------------------
void CMoteurBase::killBase()
{if (m_DataBase)
    {if (m_DataBase->isOpen()) m_DataBase->  close();
     QSqlDatabase::removeDatabase (m_BaseLabel);
 //    delete m_DataBase;
 //    m_DataBase = 0;
    }
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
    QSqlQuery sqlQuery ( QString ("SELECT NumVers FROM %2 ").arg(m_VERSION_TBL_NAME) , m_DataBase );
    if (sqlQuery.isActive())
       {while (sqlQuery.next())
              {tmp     = sqlQuery.value(0).toString();
               if (tmp[1]=='.')     tmp = tmp.prepend('0'); // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
               tmp  = tmp.remove('.');
               if (sep.length())
                  {if (tmp.mid(1,1)==sep) tmp = tmp.prepend('0'); // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
                   tmp  = tmp.remove(sep);
                  }
               if ( tmp.length() <= 5 ) tmp +="000";
               if ( tmp.toInt() > version.toInt() )  version = tmp;  // attention  .toFloat() bug sous VC++
              }
       }
    CloseBase();
    ret     = version.toInt();
    if (sep.length()) version = version.left(2)+sep+version.mid(2,2)+sep+version.mid(4,3);
    return ret;
}
//-------------------------------------- normaliseVersion ---------------------------------------------------
int CMoteurBase::normaliseVersion(const QString &version, const QString &sep /*=""*/ )
{QString vers = version;
 return normaliseVersion(vers, sep  );
}
//-------------------------------------- normaliseVersion ---------------------------------------------------
int CMoteurBase::normaliseVersion(QString &version, const QString &sep /*=""*/ )
{if (version[1]=='.') version = version.prepend('0');     // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
 version = version.remove('.');
 if (sep.length())
    {if (version.mid(1,1)==sep) version = version.prepend('0'); // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
     version = version.remove(sep);
    }
 if (version.length() <= 5 ) version +="000";
 int ret = version.toInt();
 if (sep.length()) version = version.left(2)+sep+version.mid(2,2)+sep+version.mid(4,3);
 return ret;
}
//-----------------------------------------------------  SetMedinTuxVersion -------------------------------------------
void CMoteurBase::SetMedinTuxVersion(const QString &version)
{   if (OpenBase()==0)  return;
    QString requete =  QString("INSERT INTO ") + m_VERSION_TBL_NAME   + " (NumVers) VALUES ('" + version +"' )";
    QSqlQuery query(requete, m_DataBase);
    CloseBase();
}

//-----------------------------------------------------  initBase -------------------------------------------
/**
 *
 * @param driver
 * @param DataBaseName
 * @param user
 * @param password
 * @param hostname
 * @param port
 * @param confFile
 * @param baseLabel
 * @param errMess
 */
void CMoteurBase::initBase   (const QString & driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                              const QString & DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                              const QString & user,          // = "root"
                              const QString & password,      // = ""
                              const QString & hostname,
                              const QString & port,
                              const QString & confFile,
                              const QString & baseLabel,
                              QString*    errMess)
{ int ret;
  m_DriverName      = driver;
  m_BaseName        = DataBaseName;
  m_UserName        = user;
  m_PassWord        = password;
  m_HostName        = hostname;
  m_Port            = port;
  m_DataBase        = 0;
  m_ConvertRun      = FALSE;
  m_CloseAfterQuery = 0;
  m_pReconnectTimer = 0;
  m_Debug           = 0;
  QString mess;
  //.................... se connecter � la base ........................................
  m_DataBase    = BaseConnect(driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                              DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                              user,          // = "root"
                              password,      // = ""
                              hostname,      // = "localhost"
                              port,
                              errMess,
                              baseLabel);

  //................ charger fichier de config des bases et verifier sa coherence .............................................
  if (confFile.length()==0)        {ret = 0; mess +=  "\r\n ConfFile absent: ";  }
  else                             {ret = SetConfBase(confFile, &mess);          } // met a jour m_VERSION_NUMBER
  int versionWhish = normaliseVersion(VERSION_BASE);                               // version exigee de la base avec le programme actuel
  //................ verifier l'integrite des bases ...................................
  if (m_DataBase==0)  ret = 0;
  if (ret && baseLabel[0] != '*')
     {  if ( versionWhish   != normaliseVersion(m_VERSION_NUMBER, ".") )  // m_VERSION_NUMBER est normalise (vient du fichier de configuration des bases)
           {mess +=  TR("\r\n Configuration du fichier 'DataBase.cfg' incorrecte :");
            mess +=  TR("\r\n       Version exigée par drtux  du fichier 'DataBase.cfg' : %1").arg(VERSION_BASE);
            mess +=  TR("\r\n       Version actuelle du fichier 'DataBase.cfg' : %1").arg(m_VERSION_NUMBER);
            ret   = 0;
           }
        else
           { QString versUsedStr = "";
             int versionInUse    = GetMedinTuxNormalisedVersion(versUsedStr,".");    // retourne versUsedStr = 02.14.011
             if (versionInUse < versionWhish)
                {mess +=  TR("\r\n Version de la base installée: %1 non à jour, version souhaitée : %2").arg(versUsedStr, VERSION_BASE);
                 int ok = verifyBaseIntegrity(confFile, &mess);
                 if (ok==0)
                    {ret   = 0;
                    }
                 else
                    {qDebug (mess);
                    }
                } // endif (versionInUse < versionWhish)
          } // endelseif(versionWhish   != m_VERSION_NUMBER.toDouble())
     } // endif (ret)

  if (ret==0)
     {//qDebug(mess);
      if (errMess) *errMess = mess;
      ret   = 0; killBase(); m_DataBase=0; return;
      return;
     }
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
 QSqlDriver *pQSqlDriver =  m_DataBase->driver();
 if ( pQSqlDriver == 0) { (*errMess) += TR("\r\n verifyBaseIntegrity() Driver non instancié"); return 0;}


 //..................... verifier si les tables deja en place correspondent avec celles ..................
 //                      indiquees par le fichier de configuration
 QStringList tablesList      = pQSqlDriver->tables("1");                    tablesList.sort();
 QStringList tablesListWish  = getTableListFromCfgFile(confFile, errMess);  tablesListWish.sort();

 if (tablesList != tablesListWish)
    {//.................. recuperer le fichier SQL de creation initial des bases ............................
     //                   servira a regenerer les tables absentes
     QFileInfo qfi(qApp->argv()[0]);
     QString     fileSql    =  CGestIni::Construct_Name_File_Ini("set_bases", qfi.dirPath (true), "SqlCreateTable/Dump_DrTuxTest.sql" );
     //.................. premier niveau (benin) enumerer les tables presentes ........................................
     //                   dans la base et non indiquees par le fichier de config : confFile
     mess += TR("\r\n verifyBaseIntegrity() les tables ne correspondent pas  \r\n");
     if (tablesList.count() > tablesListWish.count())
        {    mess += TR("\r\n ATTENTION : ces table(s) sont non indiquée(s) dans le fichier de configuration : ");
             for ( QStringList::Iterator it = tablesList.begin(); it != tablesList.end(); ++it )
                 { tbl = *it;
                   if (tablesListWish.findIndex (tbl)==-1)
                      {mess += TR("\r\n    = ") + tbl;
                      }
                 }
        }
      //.................. deuxieme niveau (fatal) enumerer les tables indiquees par le fichier ........................................
      //                   de config : confFile et non presentes en base et les regenerer
      for ( QStringList::Iterator it = tablesListWish.begin(); it != tablesListWish.end(); ++it )
          { tbl = *it;
            if (tablesList.findIndex (tbl.lower())==-1)
               {++nf;
                mess += TR("\r\n    = ") + tbl;
                if(tryToSetThisTable(tbl, fileSql, m_DataBase)==0)
                  {mess += TR(" ==> Echec de la tentative de création");
                   majOk = 0;
                  }
                else
                  {mess += TR(" ==> Réussite de la tentative de création");
                  }
               }
          }
      if (majOk){mess = mess.prepend(TR("\r\n SUCCES de la mise à jour des tables manquantes."));}
      if (nf)   {mess = mess.prepend(TR("\r\n ERREUR %1 table(s) non trouvé dans la base actuelle : ").arg(nf));}
      (*errMess) += mess;
    }
 if (majOk) SetMedinTuxVersion(m_VERSION_NUMBER);
 return majOk;
}

//-----------------------------------------------------  tryToSetThisTable -------------------------------------------
int CMoteurBase::tryToSetThisTable(const QString &tbl, const QString fileSQL, QSqlDatabase* dataBase)
{   int tryCreate = 1;
    int tryInsert = 0;
    int ok        = 0;
    QString toFindCreate = QString("CREATE TABLE `")+tbl+"`"; toFindCreate = toFindCreate.upper();
    QString toFindInsert = QString("INSERT INTO `") +tbl+"`"; toFindInsert = toFindInsert.upper();

    //....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fileSQL ) )         return 0;
    QFile file( fileSQL );
    if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return 0;

    //.......... on déclare une variable pour lire le fichier ligne à ligne ..................
    char *pt;
    QString requete       = "";
    QString line          = "";

    while (!file.atEnd() && (tryCreate || tryInsert>0))
          {
           file.readLine(line,MAX_READ); line=line.stripWhiteSpace();
           //.............................>>>>> CREATION  .....................
           if (tryCreate && line.left(toFindCreate.length()).upper()==toFindCreate)
              {  requete = line;
                 while (!file.atEnd())
                       { file.readLine(line, MAX_READ); line=line.stripWhiteSpace();
                         line.remove ("collate utf8_unicode_ci");
                         line.remove ("collate utf8_bin");
                         line.remove ("character set utf8");
                         requete += line;
                         if (line.left(13) == ") TYPE=MyISAM" || line.left(15) == ") ENGINE=MyISAM" )  // ne surtout pas inclure ';' dans la comparaison
                            { QSqlQuery query(QString::null, dataBase);
                              query.exec(requete);
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
           //   INSERT INTO `ACCES2` VALUES ('A','à foyer ouvert'),('A','par ... abord '),('A','par dissection ...');
           //   INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
           //   (26662,62153,'ABLAIN ST NAZAIRE'),
           //   (35076,80320,'ABLAINCOURT PRESSOIR');

           else if (tryInsert && line.left(toFindInsert.length()).upper()==toFindInsert)
              { int pos_i = line.find("VALUES");
                if (pos_i !=-1 )
                   { //............ isoler le verbe .................................................................
                     long len_line;
                     requete   ="";
                     //..................... isoler les données à inserer ............................................
                     do
                      {
                        len_line = line.length();
                        pt = (char*)(const char*)line;
                        if (len_line>2 && line.mid(len_line-2) == ");")  // ne surtout pas inclure ';' dans la comparaison
                           {
                            requete += line;
                            QSqlQuery query(QString::null, dataBase);
                            ParseSQL_InsertInto(requete,dataBase );
                            tryInsert=0;
                            break;
                           }
                        requete += line;
                        file.readLine(line,MAX_READ); line=line.stripWhiteSpace();
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
// INSERT INTO `ACCES2` VALUES ('A','à foyer ouvert'),('A','par ... abord'),('A','par dissection ...');
// INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
//  (26662,62153,'ABLAIN ST NAZAIRE'),
//  (35076,80320,'ABLAINCOURT PRESSOIR');
void  CMoteurBase::ParseSQL_InsertInto(QString &text, QSqlDatabase *dataBase)
{   int pos,deb;
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
    //...................... se mettre sur premiere parenthese de données ........................................
    deb      = text.find("(", pos);
    if ( deb==-1)
       {   //Datasemp_OutMessage(logWidget,QObject::tr(" Erreur de syntaxe  INSERT INTO VALUES : ligne ")+ QString::number(line_number));
        return;
       }
    //....................... parser ................................................
    while (run)
    {end = text.find("),(", deb);                      // chercher fin des données par le séparateur
        if (end==-1)                                      // si pas trouvé chercher dernière insertion
        {end = text.find(");", deb);
            if (end==-1)
               {   //Datasemp_OutMessage(logWidget,QObject::tr(" Erreur de syntaxe  INSERT INTO VALUES : ligne :  ")+ QString::number(line_number));
                return;
               }
            else
               {run = FALSE;
               }
        }
        //........................ si données valide les insérer ...................................................
        if (end!=-1)
           {  QSqlQuery query(QString::null, dataBase);
              QString line = verb + text.mid(deb, end-deb+1);
              query.exec(line);
              //OutSQL_error(query, QObject::tr("ERREUR INSERT INTO : ParseSQL_Dump()"), line );
              if (run) deb = end + 2;
           }
    } //end while (run)
}

//-----------------------------------------------------  getTableListFromCfgFile -------------------------------------------
QStringList  CMoteurBase::getTableListFromCfgFile(const QString &confFile, QString * /*errMess*/ )
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
QSqlDatabase*  CMoteurBase::BaseConnect(const QString & driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                        const QString & DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                        const QString & user,          // = "root"
                                        const QString & pasword,       // = ""
                                        const QString & hostname,      // = "localhost"
                                        const QString & port,
                                        QString*    errMess,
                                        QString     baseLabel)
    {
        m_BaseLabel             = baseLabel;
        QSqlDatabase::removeDatabase ( baseLabel );
        QSqlDatabase *defaultDB = QSqlDatabase::addDatabase( driver , baseLabel);
        if (defaultDB<=0) return 0;
        defaultDB->setDatabaseName( DataBaseName );
        defaultDB->setUserName( user );
        defaultDB->setPassword( pasword );
        defaultDB->setHostName( hostname );
        defaultDB->setPort( port.toInt() );
        if ( ! defaultDB->open() )
           {QString qstr = "";
            qstr += TR("Failed to open database: ") + QString(driver) + "  " + QString(DataBaseName) + "\r\n" +
                     defaultDB->lastError().driverText() + "\r\n" + defaultDB->lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
            return 0;
           }
        if (m_CloseAfterQuery==0 && m_DataBase &&  m_DataBase->isOpen()==FALSE) m_DataBase->open();
        if (m_pReconnectTimer==0) m_pReconnectTimer       = new QTimer(this, "reconnectTimerDrTux" );
        if (m_pReconnectTimer)
           {m_pReconnectTimer->start (DB_TIME_TO_RECONNECT); //reconnexion toutes ls heures
            connect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(On_ReconnectTimerDone()) );
           }
        m_DataBase = defaultDB;
        return defaultDB;
    }

//-----------------------------------------------------  SetConfBase -------------------------------------------
int CMoteurBase::SetConfBase(const char* confFile, QString *errMess)
{
  QString  err = "";
  char    *deb = (char*) confFile;
  char     *pt = (char*) confFile;
  int     line = 0;
  //.............................. NUMERO DE VERSION DE LA BASE .........................................................
  pt = SetConfBase_SetProperties(deb, m_VERSION_NUMBER,           "VERSION_NUMBER",            &line , err); if (err.length())     goto SetConfBase_Error;
  //.............................. DOSS_INDEX_TBL_NAME .........................................................
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_TBL_NAME,      "DOSS_INDEX_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_GUID,          "DOSS_INDEX_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_NOM,           "DOSS_INDEX_NOM",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_PRENOM,        "DOSS_INDEX_PRENOM",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_PRIM_KEY,      "DOSS_INDEX_PRIM_KEY",       &line , err); if (err.length())     goto SetConfBase_Error;

  //............................ DOSS_IDENT_TBL_NAME ...........................................................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TBL_NAME,     "DOSS_IDENT_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_GUID,         "DOSS_IDENT_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_JFNOM,        "DOSS_IDENT_JFNOM",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NSS,          "DOSS_IDENT_NSS",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_SEX,          "DOSS_IDENT_SEX",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NB_ENF,       "DOSS_IDENT_NB_ENF",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_RUE,          "DOSS_IDENT_RUE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_CODE_POST,    "DOSS_IDENT_CODE_POST",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_VILLE,        "DOSS_IDENT_VILLE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_CEDEX,        "DOSS_IDENT_CEDEX",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_PAYS,         "DOSS_IDENT_PAYS",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL1,         "DOSS_IDENT_TEL1",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL1_TYP,     "DOSS_IDENT_TEL1_TYP",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL2,         "DOSS_IDENT_TEL2",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL2_TYP,     "DOSS_IDENT_TEL2_TYP",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL3,         "DOSS_IDENT_TEL3",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL3_TYP,     "DOSS_IDENT_TEL3_TYP",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_EMAIL,        "DOSS_IDENT_EMAIL",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NUMSS,        "DOSS_IDENT_NUMSS",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_IS_ASS,       "DOSS_IDENT_IS_ASS",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_GEME,         "DOSS_IDENT_GEME",           &line , err); if (err.length())     goto SetConfBase_Error;           // rang gemellaire
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NOM_ASS,      "DOSS_IDENT_NOM_ASS",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_PRENOM_ASS,   "DOSS_IDENT_PRENOM_ASS",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NSS_ASS,      "DOSS_IDENT_NSS_ASS",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_PROF,         "DOSS_IDENT_PROF",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TITRE,        "DOSS_IDENT_TITRE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_REF_PK,       "DOSS_IDENT_REF_PK",         &line , err); if (err.length())     goto SetConfBase_Error;

  //................. DOSS_NOTE_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_TBL_NAME,       "DOSS_NOTE_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_PK,             "DOSS_NOTE_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_PAT_GUID,       "DOSS_NOTE_PAT_GUID",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_PAT_PK,         "DOSS_NOTE_PAT_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_HTML,           "DOSS_NOTE_HTML",             &line , err); if (err.length())     goto SetConfBase_Error;

  //................. DOSS_INTERVENANTS_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_TBL_NAME,       "DOSS_INTERVENANTS_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_PK,             "DOSS_INTERVENANTS_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_PAT_GUID,       "DOSS_INTERVENANTS_PAT_GUID",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_PAT_PK,         "DOSS_INTERVENANTS_PAT_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_INTERVPK,       "DOSS_INTERVENANTS_INTERVPK",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_TYPE,           "DOSS_INTERVENANTS_TYPE",             &line , err); if (err.length())     goto SetConfBase_Error;

  //................. INTERVENANTS_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_TBL_NAME,       "INTERVENANTS_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_PK,             "INTERVENANTS_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_NOM,            "INTERVENANTS_NOM",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_PRENOM,         "INTERVENANTS_PRENOM",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_SPEC,           "INTERVENANTS_SPEC",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_TITRE,          "INTERVENANTS_TITRE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_RUE,            "INTERVENANTS_RUE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_CODE_POST,      "INTERVENANTS_CODE_POST",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_VILLE,          "INTERVENANTS_VILLE",            &line , err); if (err.length())     goto SetConfBase_Error;

  //............................ CIM10_USER_FAV_TBL_NAME ...........................................................
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_TBL_NAME,     "CIM10_USER_FAV_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_PK,           "CIM10_USER_FAV_PK "  ,          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_USER,         "CIM10_USER_FAV_USER" ,          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_DATA,         "CIM10_USER_FAV_DATA",           &line , err); if (err.length())     goto SetConfBase_Error;

  //............................ DOSS_RUB_HEAD_TBL_NAME ...........................................................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_TBL_NAME,      "DOSS_RUB_HEAD_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PRIMKEY,       "DOSS_RUB_HEAD_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_GUID,          "DOSS_RUB_HEAD_GUID",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_TYPE,          "DOSS_RUB_HEAD_TYPE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_SUBTYPE,       "DOSS_RUB_HEAD_SUBTYPE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_NOM_DATE,      "DOSS_RUB_HEAD_NOM_DATE",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_DATE,          "DOSS_RUB_HEAD_DATE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_USER,          "DOSS_RUB_HEAD_USER",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_SIGN_USER,     "DOSS_RUB_HEAD_SIGN_USER",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_REF_BLOBS_PK,  "DOSS_RUB_HEAD_REF_BLOBS_PK",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_FIN,           "DOSS_RUB_HEAD_FIN",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_DUREE_MOD,     "DOSS_RUB_HEAD_DUREE_MOD",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_REF_DOSS_PK,   "DOSS_RUB_HEAD_REF_DOSS_PK",      &line , err); if (err.length())     goto SetConfBase_Error;

  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_0,        "DOSS_RUB_HEAD_PROP_0",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_1,        "DOSS_RUB_HEAD_PROP_1",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_2,        "DOSS_RUB_HEAD_PROP_2",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_3,        "DOSS_RUB_HEAD_PROP_3",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_4,        "DOSS_RUB_HEAD_PROP_4",           &line , err); if (err.length())     goto SetConfBase_Error;

  //................. DOSS_RUB_DATA_TBL_NAME ......................................................................................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_TBL_NAME,      "DOSS_RUB_DATA_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_GUID,          "DOSS_RUB_DATA_GUID",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_PRIMKEY,       "DOSS_RUB_DATA_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_BLOB,          "DOSS_RUB_DATA_BLOB",             &line , err); if (err.length())     goto SetConfBase_Error;

  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_TBL_NAME,          "EVNT_LIST_TBL_NAME",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_PK,                "EVNT_LIST_PK",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_DOSS_PK,                "EVNT_DOSS_PK",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_PERE,              "EVNT_LIST_PERE",                 &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_PKDOC,             "EVNT_LIST_PKDOC",                &line , err); if (err.length())     goto SetConfBase_Error;


  //................. USER_IDENT_TBL_NAME ......................................................................................
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TBL_NAME,      "USER_IDENT_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NOM,           "USER_IDENT_NOM",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PRENOM,        "USER_IDENT_PRENOM",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TITRE,         "USER_IDENT_TITRE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_LOGIN,         "USER_IDENT_LOGIN",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PASSWORD,      "USER_IDENT_PASSWORD",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_DROITS,        "USER_IDENT_DROITS",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_SEXE,          "USER_IDENT_SEXE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NUM_ORDRE,     "USER_IDENT_NUM_ORDRE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NUM_RPPS,      "USER_IDENT_NUM_RPPS",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CLEF_RPPS,     "USER_IDENT_CLEF_RPPS",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PRIMKEY,       "USER_IDENT_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_GUID,          "USER_IDENT_GUID",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CONVENTION,    "USER_IDENT_CONVENTION",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CODE_POST,     "USER_IDENT_CODE_POST",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_VILLE,         "USER_IDENT_VILLE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_RUE,           "USER_IDENT_RUE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_QUALITE,       "USER_IDENT_QUALITE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_1,         "USER_IDENT_TEL_1",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_2,         "USER_IDENT_TEL_2",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_3,         "USER_IDENT_TEL_3",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_EMAIL,         "USER_IDENT_EMAIL",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NOTE,          "USER_IDENT_NOTE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CODE_REGIME,   "USER_IDENT_CODE_REGIME",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CHER,          "USER_IDENT_CHER",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_TYPE_1,    "USER_IDENT_TEL_TYPE_1",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_TYPE_2,    "USER_IDENT_TEL_TYPE_2",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_TYPE_3,    "USER_IDENT_TEL_TYPE_3",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_ABR_1,     "USER_IDENT_TEL_ABR_1",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_ABR_2,     "USER_IDENT_TEL_ABR_2",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_ABR_3,     "USER_IDENT_TEL_ABR_3",        &line , err); if (err.length())     goto SetConfBase_Error;

  //................. USER_DATA_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_TBL_NAME,      "USER_DATA_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_PK,            "USER_DATA_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_REF_PK,        "USER_DATA_REF_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_NAMEDOC,       "USER_DATA_NAMEDOC",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_TYPE,          "USER_DATA_TYPE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_BLOB,          "USER_DATA_BLOB",             &line , err); if (err.length())     goto SetConfBase_Error;

  //................. VERROUS ............................
  pt = SetConfBase_SetProperties(pt,   m_VERROU_TBL_NAME,         "VERROU_TBL_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_PK,               "VERROU_PK",                  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_DOSS_PK,          "VERROU_DOSS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_DOSS_GUID,        "VERROU_DOSS_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_RUB_NAME,         "VERROU_RUB_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_RUB_PK,           "VERROU_RUB_PK",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_USER_NAME,        "VERROU_USER_NAME",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_SIGN_USER_NAME,   "VERROU_SIGN_USER_NAME",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_DATE_TIME,        "VERROU_DATE_TIME",           &line , err); if (err.length())     goto SetConfBase_Error;

 //................. USER_PERMS ............................
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)

  pt = SetConfBase_SetProperties(pt,   m_CODES_POSTAUX_TBL_NAME,        "CODES_POSTAUX_TBL_NAME",          &line , err); if (err.length())     goto SetConfBase_Error;  // nom de la table des codes postaux
  pt = SetConfBase_SetProperties(pt,   m_AGENDA_MASK_TBL_NAME,          "AGENDA_MASK_TBL_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;  // table des masques agenda
  pt = SetConfBase_SetProperties(pt,   m_AGENDA_TBL_NAME,               "AGENDA_TBL_NAME"  ,               &line , err); if (err.length())     goto SetConfBase_Error;  // table de l'agenda
  pt = SetConfBase_SetProperties(pt,   m_AGENDA_COLOR_PROFILS_TBL_NAME, "AGENDA_COLOR_PROFILS_TBL_NAME" ,  &line , err); if (err.length())     goto SetConfBase_Error;  // table des couleurs de profils agenda
  pt = SetConfBase_SetProperties(pt,   m_VERSION_TBL_NAME,              "VERSION_TBL_NAME" ,               &line , err); if (err.length())     goto SetConfBase_Error;  // table des versions du programme

return 1;

SetConfBase_Error:
  if (errMess) *errMess = err;
  qDebug(err);
  qDebug(confFile);
  return 0;
}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *CMoteurBase::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
{int nLine = *line;
 err  = "";
 long len_token = strlen (token);
 while(*pt)
    {SKIP_BLANK_CAR(pt);
     if (strncmp(pt, token, len_token)==0)
        {//return pt+len_token;
         pt         += len_token;
         err         = "";
         propertie   = "";
         //............. chercher premier delimiteur ...............................
          while(*pt && *pt!='\r' && *pt !='\n' && *pt !='\'' ) pt++;
          if (*pt=='\'')         // si delimiteur pas trouve erreur
             {pt++;               // passer le delimiteur
              char *end = pt;
              //............. chercher deuxiéme delimiteur ...............................
              while(*end && *end!='\r' && *end !='\n' && *end !='\'')end++;
              if (*end=='\'')
                 {propertie.setLatin1(pt, end-pt);
                  if (line) *line = nLine;
                  return end +1;
                 }
              else
                {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not find at line N°: ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not find at line N°: ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)

     NEXT_LINE(pt);
     ++nLine;

    }
 err  = "Syntax Error: SetConfBase()\r\n";
 err += token;
 err += TR(" non trouvé à la ligne N°: ");
 err += QString::number(nLine);
 if (line) *line = nLine;
 return pt;
}

//-----------------------------------------------------  OpenBase -------------------------------------------
int   CMoteurBase::OpenBase()
{//if (m_DataBase==0)                                            return 0;
 if (m_CloseAfterQuery==0)                                     return 1;
 if (m_DataBase->isOpen()==FALSE && m_DataBase->open()==FALSE) return 0;
 return 1;
}
//-----------------------------------------------------  CloseBase -------------------------------------------
void   CMoteurBase::CloseBase() //m_CloseAfterQuery
{if (m_CloseAfterQuery==0) return;
 if (m_DataBase->isOpen()) m_DataBase->close();
}

//-----------------------------------------------------  On_ReconnectTimerDone -------------------------------------------
void CMoteurBase::On_ReconnectTimerDone()
{if (m_DataBase->isOpen()==FALSE && m_DataBase->open()==FALSE) return;
 QSqlQuery query(QString::null , m_DataBase );
 query.exec(DUMMY_TEST_SQL_COMMAND);
 if (m_CloseAfterQuery) m_DataBase->close();
}

//-----------------------------------------------------  GotoDebug -------------------------------------------
void CMoteurBase::GotoDebug()
{
}
//----------------------------------------------- GetNbRecord ---------------------------------------------------
QString CMoteurBase::GetNbRecord(const QString &table)
{ QString result = "";
  if (OpenBase()==0) return QString::null;

  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete = QString("SELECT COUNT(*) FROM ") + table;
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive() && query.next())
     {result = query.value(0).toString();
     }
  CloseBase();
  return result;
}

//--------------------------------- Utf8_Query ------------------------------------------------------------
QString CMoteurBase::Utf8_Query(QSqlQuery &cur, int field)
{QCString     cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (CGestIni::IsUtf8( ptr , cString.length()))
    {return QString::fromUtf8 ( ptr);
    }
 else
    {return QString(ptr) ;
    }
}

//************************************************************************************************************************
//=================================== GESTION DES CONVERSIONS DE BASE ET INTEGRITE ======================================
//************************************************************************************************************************
void CMoteurBase::RenameTableWhithCorrectName()
{if (OpenBase()==0)  return;
     QSqlQuery query(QString::null, m_DataBase);
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
{QStringList list = m_DataBase->tables();
 QStringList::Iterator it = list.begin();
 while( it != list.end() )
      {if (correct_name_table.lower()== (*it).lower())
          {pQSqlQuery->exec(QString("ALTER TABLE `") + (*it) + "` RENAME `"+correct_name_table+"` ;");
          }
       ++it;
      }
}

//-----------------------------------------------------  ConvertToNew -------------------------------------------
int CMoteurBase::ConvertToNew(QLabel* pQlabelMess /* = 0 */, QProgressBar* pQProgressBar /* = 0 */)
{int i=0;
 QString qstr;
 m_ConvertRun = TRUE;

 m_ConvertRun = 1;
 if (pQProgressBar)  pQProgressBar->setTotalSteps ( GetNbRecord(m_DOSS_RUB_HEAD_TBL_NAME).toLong() );

 //................. Preparer la requete .....................................
 if (OpenBase()==0)
    {if (pQlabelMess) pQlabelMess->setText(qstr.sprintf(TR("ConvertToNew(): database can't be open")));
     return i;
    }

 //................. Preparer la requete .....................................
 QString      requete  = "SELECT ";
              requete += m_DOSS_RUB_HEAD_PRIMKEY + "," + m_DOSS_RUB_HEAD_GUID + " FROM " + m_DOSS_RUB_HEAD_TBL_NAME;
 QSqlQuery sqlQuery(requete , m_DataBase );
 if (sqlQuery.isActive())
    { QSqlQuery     queryTarget (QString::null, m_DataBase  );
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
                    {pQlabelMess->setText(TR("indexation N°: %1").arg(i));
                     if (pQProgressBar)  pQProgressBar->setProgress ( ++m_ConvertRun );
                     qApp->processEvents();qApp->processEvents();
                    }
                }
            }
         else
            {qstr   = "";
                 qstr  += requete;
             qstr  += TR("\r\nIndexation error: ");
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
 if (pQlabelMess) pQlabelMess->setText(TR("Opération terminée °: %1 enregistrements traités").arg(i));
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
   //QString      m_DOSS_IDENT_REF_PK;        // reference à la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX

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
  // QString      m_DOSS_RUB_DATA_TBL_NAME;        // nom de la table des données
  // QString      m_DOSS_RUB_DATA_GUID;            // GUID du dossier auquel l'enregistrement fait reference

//-----------------------------------------------------  ResoudreDoublons -------------------------------------------
int CMoteurBase::ResoudreDoublons(QListViewItem *pQListViewItem_Src, QListView *pQListView)
{

 //................. recuperer toutes les notes et les fusionner.....................................
 QString note     = "";
 QString str      = "";
 QString src_Prmk = pQListViewItem_Src->text(2);
 QString src_guid = pQListViewItem_Src->text(3);
 QString dst_Prmk = "";
 QString dst_guid = "";

 QListViewItemIterator ut( pQListView );
 while ( ut.current() )
    {QListViewItem *pQListViewItem = ut.current();
     PatientNoteGetData(pQListViewItem->text(2) ,  str);
     note += "\r\n" + str;
     ++ut;
    }

 //..................... s'occuper du reste ............................
 if (OpenBase()==0) return 0;

 QString requete;
 QSqlQuery sqlQuery(requete , m_DataBase );
 QListViewItemIterator it( pQListView );
 while ( it.current() )
       { QListViewItem *pQListViewItem = it.current();
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

             //....................... effacer l'identité ............................
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
   PatientNoteWrite( src_Prmk , src_guid,  note);
   return 1;
}
//************************************************************************************************************************
//=================================== GESTION FAVORIS CIM10 ==============================================================
//************************************************************************************************************************

//-----------------------------------------------------  initThesaurusListe -------------------------------------------
//  1 = HTA,Medical (cardio vasculaire), ancien, découverte lors d'un AIT,12-12-2009,I10,hypertension essentielle (primitive)

void CMoteurBase::initThesaurusListe(QStringList &list, const QString &user)
{ if (OpenBase()==0)   return;
  QString data;
  QString requete = QString(" SELECT ") + m_CIM10_USER_FAV_PK       +", "    + m_CIM10_USER_FAV_DATA +
                            " FROM "    + m_CIM10_USER_FAV_TBL_NAME +
                            " WHERE "   + m_CIM10_USER_FAV_USER     + " = '" + user + "' ";
  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {data = Utf8_Query(query, 1 );
      CGestIni::Param_GetList(data, "ATCD Thesaurus", "",  list );
     }
  CloseBase();
}
//-----------------------------------------------------  saveThesaurus -------------------------------------------
void CMoteurBase::saveThesaurus(const QString &dataToExport, const QString &user)
{    if (OpenBase()==0)   return;
     QSqlQuery query(QString::null, m_DataBase);
     QString requete = QString(" DELETE FROM  ") + m_CIM10_USER_FAV_TBL_NAME +
                               " WHERE "         + m_CIM10_USER_FAV_USER     + " = '" + user + "' ";
     query.exec(requete);  // on y va bourrin en effacant (tres peu de donnees)
             requete = QString(" INSERT INTO  ") + m_CIM10_USER_FAV_TBL_NAME +
                               " (  "            + m_CIM10_USER_FAV_USER     +","+m_CIM10_USER_FAV_DATA+" )"+
                               "VALUES (?,?) ";
     query.prepare(requete);
     query.bindValue(0, user);
     query.bindValue(1, dataToExport);
     query.exec();
     CloseBase();
}

//************************************************************************************************************************
//=================================== GESTION CODE POSTAL ================================================================
//************************************************************************************************************************

//-----------------------------------------------------  GetVilleFromCodePostal -------------------------------------------
QString CMoteurBase::GetVilleFromCodePostal(const QString &codePostal)
{ QString result  ("");
  QString requete ("SELECT ville FROM codes_postaux WHERE code_postal = ");
          requete +=  codePostal ;
  if (OpenBase()==0)   return result;
  QSqlQuery query(requete , m_DataBase );
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
  QSqlQuery query(requete , m_DataBase );
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
  QSqlQuery query(requete , m_DataBase );
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
  QSqlQuery query(requete , m_DataBase );
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
  QSqlQuery query(requete , m_DataBase );
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


//-----------------------------------------------------  GetPatientList -------------------------------------------
/*! \brief Remplit la QListView avec les patients retrouvés dans la base de données.
 *  \param pQlistView : ListView qui recevra les patients recherchés
 *  \param qsrt_nom : Nom à rechercher
 *  \param qsrt_prenom : Prenom à rechercher
 *  \param statusMess : message de retour
 *  \param errMess : Message d'erreur.
 *  \return nombre d'enregistrements insérés dans la QListView passée en paramètre
*/
long CMoteurBase::GetPatientList(       QListView *pQlistView,
                                  const QString   &qstr_nom,
                                  const QString   &qstr_prenom,
                                        QLabel    *statutMess, /* = 0  */
                                        QLabel    *errMess     /* = 0  */
                                )
{QString   q_nom    = "";
 QString   q_prenom = "";
 QString tmp = "";
 //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::GetPatientList(): data base can't be open");
      return 0;
     }
  QString requete, requete_ident;
  if (qstr_nom != ""||qstr_prenom!="")
     {q_nom    = qstr_nom;
      q_prenom = qstr_prenom;
     }
  q_nom.replace("\'","\\\'");
  q_prenom.replace("\'","\\\'");
  int nb_columns = pQlistView->columns();
  requete        = "SELECT ";
  requete       += m_DOSS_INDEX_NOM + ", " + m_DOSS_INDEX_PRENOM + ", \n";
  requete       += m_DOSS_INDEX_PRIM_KEY + ", " + m_DOSS_INDEX_GUID + " \n";
  requete       += " FROM "            + m_DOSS_INDEX_TBL_NAME   + " \nWHERE ";
  requete       += m_DOSS_INDEX_NOM    + " LIKE '"+ q_nom        + "%' \nAND ";
  requete       += m_DOSS_INDEX_PRENOM + " LIKE '"+ q_prenom     + "%' \n";
  requete       += " ORDER BY "        + m_DOSS_INDEX_NOM;// + " LIMIT 0,50";

  QSqlQuery query(requete , m_DataBase );
  QSqlQuery query_ident;

  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview

  int i  = 0;
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
  {pQlistView->clear();
   while (query.next())
   { if (i < 200)
     { QListViewItem *element = 0;
       CPrtQListViewItem* line = 0;
       //............... on rempli la listbox selon son nombre de colonnes .....................
       switch (nb_columns)
       {case 1:
                element = new QListViewItem( pQlistView,
                                                Utf8_Query(query, 0 ) );
                break;
        case 2:
                element = new QListViewItem( pQlistView,
                                                Utf8_Query(query, 0 ),
                                                Utf8_Query(query, 1 ) );
                        break;
        case 3:
                element = new QListViewItem( pQlistView,
                                                Utf8_Query(query, 0 ),
                                                Utf8_Query(query, 1 ),
                                                Utf8_Query(query, 2 ) );
                break;
        case 4:
                element = new QListViewItem( pQlistView,
                                                Utf8_Query(query, 0 ),                 // Nom
                                                Utf8_Query(query, 1 ),          // Prenom
                                                Utf8_Query(query, 2 ),                // PK
                                                Utf8_Query(query, 3 ) );        // GUID
                break;
        case 8 : // Récupère les élèments d'identité en plus
        { requete_ident        = "SELECT " + m_DOSS_IDENT_RUE + ", " + m_DOSS_IDENT_CODE_POST + ", \n";
          requete_ident        += m_DOSS_IDENT_VILLE + ", " + m_DOSS_IDENT_JFNOM + ", \n";
          requete_ident        += m_DOSS_IDENT_TEL1 + ", " + m_DOSS_IDENT_TEL2 + " \n";
          requete_ident        += " FROM " + m_DOSS_IDENT_TBL_NAME + " \nWHERE ";
          requete_ident        += m_DOSS_IDENT_REF_PK    + " = "+ query.value(2).toString() + "; \n";
          query_ident = QSqlQuery(requete_ident , m_DataBase );
          if ( query_ident.isActive() && query_ident.next())
          { line = new CPrtQListViewItem( pQlistView , line, Utf8_Query(query, 0 )); // Nom
            // Si pas de CP enregistré alors il est == 0 ==> ne pas l'afficher
            tmp = Utf8_Query(query_ident, 1) ;
            if (tmp == "0") tmp ="";
            line->setText( 1, Utf8_Query(query_ident, 3) );        // Nom JF
            line->setText( 2, Utf8_Query(query, 1 ) );                // Prenom
            line->setText( 3, Utf8_Query(query_ident, 0) );        // Rue
            line->setText( 4, tmp );        // CP
            line->setText( 5, Utf8_Query(query_ident, 2));        // Ville
            line->setText( 6, Utf8_Query(query_ident, 4));        // Tél 1
            line->setText( 7, Utf8_Query(query_ident, 5));        // Tél 2
            line->setMultiLinesEnabled ( FALSE );                //Plusieurs lignes par item si nécessaire
            // Dichotomiser Affichage et données à exploiter +++
            line->setNom(    Utf8_Query(query, 0 ) );                // Nom
            line->setPrenom( Utf8_Query(query, 1 ) );                // Prenom
            line->setPkDoc(  Utf8_Query(query, 2 ) );                // PK
            line->setGUID(   Utf8_Query(query, 3 ) );                // GUID

          }
          break;
        }
       } // end switch (nb_columns)
       if (element) pQlistView->insertItem ( element );
       else if (line) pQlistView->insertItem ( line );
       i++;
     } // // fin if i < 200
     nb++;
   } //end while (pSqlQuery->next())

   // Cacher les contenus des colonnes 8 et 9 si 10 demandés
   if (nb_columns == 10)
   { pQlistView->hideColumn ( 8 );
     pQlistView->hideColumn ( 9 );
   }
  } //endif (pSqlQuery && pSqlQuery->isActive())

  //ListViewPatient->setCurrentItem(0);   // se placer sur le premier enregistrement de la liste
  //....................... sortir le message d'erreur si besoin ..........................................

  if (statutMess)
     { statutMess->setText( QString( TR("Trouvé(s) %1 parmi %2") ).arg(i).arg(nb) );
     }
  CloseBase();
  return i;
}
//--------------------------------- ComputeClef_Secu ----------------------------
QString CMoteurBase::ComputeClef_Secu(const QString &txt)
{   if (txt.length() !=13) return QString("");
    QString result (QString::number( 97 - fmod(txt.toDouble() , 97)));
    if (result.length()<2) result.prepend('0');
    return result;
}

//-----------------------------------------------------  GetPatientListFromNomPrenomSecuDtNssGeme -------------------------------------------
/*! \brief Remplit la QListView avec les patients retrouvés dans la base de données.
 *  \param pQlistView : ListView qui recevra les patients recherchés
 *  \param qsrt_nom : Nom à rechercher
 *  \param qsrt_prenom : Prenom à rechercher
 *  \param statusMess : message de retour
 *  \param errMess : Message d'erreur.
 *  \return nombre d'enregistrements insérés dans la QListView passée en paramètre
*/
long CMoteurBase::GetPatientListFromNomPrenomSecuDtNssGeme( QListView *pQlistView,
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
                                                            QLabel          *errMess           /* = 0  */
                                                           )
{
 QString   q_nom      = "";
 QString   q_prenom   = "";
 QString   tmp        = "";
 QString   and_where  = " WHERE ";

 //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::GetPatientListFromNomPrenomSecuDtNssGeme(): data base can't be open");
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
  else if (typeAssure.length() && typeAssure.toInt()==0)            // ne requeter que si l'ayant droit est l'assuré (le reste peut être indetermine car autre anciennes valeurs peu sures )
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
  QSqlQuery query(requete , m_DataBase );
  QSqlQuery query_ident;

  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview
  int i  = 0;
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {int nb_columns = 0;
      if (pQlistView) {pQlistView->clear(); nb_columns = pQlistView->columns();}
      while (query.next())
         {if (i < 200)
             {if (pQlistView)
                 {
                  QListViewItem   *element = 0;
                  CPrtQListViewItem*  line = 0;
                  //............... on rempli la listbox selon son nombre de colonnes .....................
                  switch (nb_columns)
                      {case 1:
                           element = new QListViewItem( pQlistView,
                                                        Utf8_Query(query, 0 ) );
                           break;
                       case 2:
                           element = new QListViewItem( pQlistView,
                                                        Utf8_Query(query, 0 ),
                                                        Utf8_Query(query, 1 ) );
                           break;
                       case 3:
                           element = new QListViewItem( pQlistView,
                                                        Utf8_Query(query, 0 ),
                                                        Utf8_Query(query, 1 ),
                                                        Utf8_Query(query, 2 ) );
                           break;
                       case 4:
                           element = new QListViewItem( pQlistView,
                                                        Utf8_Query(query, 0 ),          // Nom
                                                        Utf8_Query(query, 1 ),          // Prenom
                                                        Utf8_Query(query, 2 ),          // PK
                                                        Utf8_Query(query, 3 ) );        // GUID
                           break;
                       case 8 : // Récupère les élèments d'identité en plus
                          {requete_ident        = "SELECT " + m_DOSS_IDENT_RUE + ", " + m_DOSS_IDENT_CODE_POST + ", \n";
                           requete_ident       += m_DOSS_IDENT_VILLE + ", " + m_DOSS_IDENT_JFNOM + ", \n";
                           requete_ident       += m_DOSS_IDENT_TEL1 + ", " + m_DOSS_IDENT_TEL2 + " \n";
                           requete_ident       += " FROM " + m_DOSS_IDENT_TBL_NAME + " \nWHERE ";
                           requete_ident       += m_DOSS_IDENT_REF_PK    + " = "+ query.value(2).toString() + "; \n";
                           query_ident = QSqlQuery(requete_ident , m_DataBase );
                           if ( query_ident.isActive() && query_ident.next())
                              {line = new CPrtQListViewItem( pQlistView , line, Utf8_Query(query, 0 )); // Nom
                               // Si pas de CP enregistré alors il est == 0 ==> ne pas l'afficher
                               tmp = Utf8_Query(query_ident, 1) ;
                               if (tmp == "0") tmp ="";
                               line->setText( 1, Utf8_Query(query_ident, 3) );          // Nom JF
                               line->setText( 2, Utf8_Query(query, 1 ) );               // Prenom
                               line->setText( 3, Utf8_Query(query_ident, 0) );          // Rue
                               line->setText( 4, tmp );        // CP
                               line->setText( 5, Utf8_Query(query_ident, 2));           // Ville
                               line->setText( 6, Utf8_Query(query_ident, 4));           // Tél 1
                               line->setText( 7, Utf8_Query(query_ident, 5));           // Tél 2
                               line->setMultiLinesEnabled ( FALSE );                    //Plusieurs lignes par item si nécessaire
                               // Dichotomiser Affichage et données à exploiter +++
                               line->setNom(    Utf8_Query(query, 0 ) );                // Nom
                               line->setPrenom( Utf8_Query(query, 1 ) );                // Prenom
                               line->setPkDoc(  Utf8_Query(query, 2 ) );                // PK
                               line->setGUID(   Utf8_Query(query, 3 ) );                // GUID
                              }
                           break;
                          }
                      } // end switch (nb_columns)
                  if (element)   pQlistView->insertItem ( element );
                  else if (line) pQlistView->insertItem ( line );
                 }   // fin if pQlistView
              i++;
             }    // fin if i < 200
          nb++;
         } //end while (pSqlQuery->next())

      // Cacher les contenus des colonnes 8 et 9 si 10 demandés
      if (nb_columns == 10)
         { pQlistView->hideColumn ( 8 );
           pQlistView->hideColumn ( 9 );
         }
     }  //endif (pSqlQuery && pSqlQuery->isActive())

  //ListViewPatient->setCurrentItem(0);   // se placer sur le premier enregistrement de la liste
  //....................... sortir le message d'erreur si besoin ..........................................

  if (statutMess)
     { statutMess->setText( QString( TR("Trouvé(s) %1 parmi %2") ).arg(i).arg(nb) );
     }
  CloseBase();
  return i;
}

//-------------------------------------- GetIdentiteFromPrimKey ----------------------------------------------------------------------------
/*! \brief Sélectionne l'enregistrement de la table: DOSS_IDENT  correspondant au bon primary key
*   \return QSqlQuery*  zero si echec valide sinon et AUQUEL CAS IL FAUDRA DETRUIRE ce pointeur par: delete  pSqlQuery et AUQUEL CAS IL FAUDRA FERMER la base par:       CloseBase()
*/
QSqlQuery*  CMoteurBase::GetIdentiteFromPrimKey(const char *primKey )
{QString requete;

 requete   = "SELECT * FROM "    + m_DOSS_IDENT_TBL_NAME + " WHERE ";
 requete  += m_DOSS_IDENT_REF_PK + " ='"                 + primKey + "'";

 if (OpenBase()==0)
     {return 0;
     }
 QSqlQuery* pSqlQuery = new QSqlQuery(requete , m_DataBase );
 if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
    {return pSqlQuery;
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return 0;
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
 QSqlCursor cur(table, TRUE, m_DataBase);                                          // ouvrir un curseur sur la table
 if (!cur.select( fieldToMatch+"='"+valueToMatch+"'" )) goto  GetFieldValueError;       // se placer sur le bon record
 if (cur.next())                             {QCString     cString  =  cur.value(fieldToGet).toCString ();
                                              char *ptr             =  cString.data();
                                              if (pQVariant)  *pQVariant = cur.value( fieldToGet);

                                              if (CGestIni::IsUtf8( ptr, cString.length() ))
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
void CMoteurBase::PatientIntervenantsGetData(const char *primKeyPat , QListView *pQListView, QString *errMess /*=0*/)
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
  QSqlQuery  query(requete , m_DataBase );
  if (query.isActive())
     { while (query.next())
             { new QListViewItem(pQListView,
                                 Utf8_Query(query, 0  ),
                                 Utf8_Query(query, 1  ),
                                 Utf8_Query(query, 2  ),
                                 Utf8_Query(query, 3  ),
                                 Utf8_Query(query, 4  )
                                );
              }
     }
  CloseBase();
  return ;
}
//-------------------------------------- PatientIntervenantsGetListData ----------------------------------------------------------------------------
void CMoteurBase::PatientIntervenantsGetListData(const QString &primKeyPat , QStringList &list, const QString &qualite /*=""*/, const QString &sep/*="|"*/, QString *errMess /*=0*/)
{ list.clear();
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
  if (qualite.length())
     {requete += " AND " + m_INTERVENANTS_TBL_NAME+"."+m_INTERVENANTS_SPEC  + " = '"  + qualite  + "' ";
     }
  //................. executer la requete .....................................
  QSqlQuery  query(requete , m_DataBase );
  if (query.isActive())
     { while (query.next())
             { list.append(      Utf8_Query(query, 0  )+sep+           // specialite
                                 Utf8_Query(query, 1  )+sep+           // nom
                                 Utf8_Query(query, 2  )+sep+           // prenom
                                 Utf8_Query(query, 3  )+sep+           // type
                                 Utf8_Query(query, 4  )                // pk
                          );
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
  //................. executer la requete .....................................
  QSqlQuery  query(requete , m_DataBase );
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
void CMoteurBase::PatientIntervenantsGetData(const char *primKeyPat , QStringList &intervList, const QString &specialite, const char *validStr, const char *decalage) //Médecin generaliste
{ QStringList pkIntervList;
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  return ;

  //................. recuperer tous les PK des intervenants sur ce patient ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_INTERVENANTS_INTERVPK  + " FROM " + m_DOSS_INTERVENANTS_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_INTERVENANTS_PAT_PK    + " = '"   + primKeyPat                     + "' ";
  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
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
 if (pk.length()>0) return PatientIntervenantsUpdate(pk, primKeyPat , guidPat,   intervPk, qualite);                                                   // existe déjà donc à ne pas créer
 else               return PatientIntervenantsCreate(primKeyPat , guidPat,   intervPk, qualite);     // n'existe pas donc créer
}

//---------------------------------------------------- PatientIntervenantsIsThisExist -----------------------------------------------
// RETOUR:    la primKey des données si elles existent sinon ""
QString CMoteurBase::PatientIntervenantsIsThisExist( const char* intervPk, const char *primKeyPat, QLabel *errMess )
{ QString ret("");
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::PatientIntervenantIsThisExist(): database can't be opened");
      return ret;
     }
  //................. filtrer selon donnée d'entrée ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_INTERVENANTS_PK          + " FROM " + m_DOSS_INTERVENANTS_TBL_NAME   + "  WHERE ";
  requete  +=             m_DOSS_INTERVENANTS_INTERVPK    + " = '"   + intervPk                       + "' AND ";
  requete  +=             m_DOSS_INTERVENANTS_PAT_PK      + " = '"   + primKeyPat                     + "'  ";

  QSqlQuery query(requete , m_DataBase );

  //.................. si la requète a un resultat ..............................................
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

  QSqlCursor cur(m_DOSS_INTERVENANTS_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(m_DOSS_INTERVENANTS_PK  + "='" + pk  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les données .........
  if ( cur.next() )
    { //............... si OK on recupere le buffer et on le charge avec les données .........
      QSqlRecord *buffer = cur.primeUpdate();                         // recuperer le buffer d'insertion
      buffer->setValue( m_DOSS_INTERVENANTS_PAT_PK,        primKeyPat );  // y placer les données
      buffer->setValue( m_DOSS_INTERVENANTS_PAT_GUID,      guidPat );     // y placer les données
      buffer->setValue( m_DOSS_INTERVENANTS_INTERVPK,      intervPk    );     // y placer les données
      buffer->setValue( m_DOSS_INTERVENANTS_TYPE,          qualite    );     // y placer les données
      ok = cur.update();                                              // ecrire le buffer avec les données du header
    }
  if (ok==0) OutSQL_error(cur, "PatientIntervenantsUpdate()");
                                        // ecrire le buffer avec les données du header
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
  QSqlCursor cur(m_DOSS_INTERVENANTS_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  buffer->setValue( m_DOSS_INTERVENANTS_PAT_PK,        primKeyPat );  // y placer les données
  buffer->setValue( m_DOSS_INTERVENANTS_PAT_GUID,      guidPat );     // y placer les données
  buffer->setValue( m_DOSS_INTERVENANTS_INTERVPK,      intervPk    );     // y placer les données
  buffer->setValue( m_DOSS_INTERVENANTS_TYPE,          qualite    );     // y placer les données
  ok = cur.insert();                                          // ecrire le buffer avec les données du header
  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//-------------------------------------- PatientIntervenantsDelete ----------------------------------------------------------------------------
int CMoteurBase::PatientIntervenantsDelete( const char *primKey, const char *path_GUID /* = 0 */)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;

  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_DOSS_INTERVENANTS_TBL_NAME  + " WHERE ";
          requete += m_DOSS_INTERVENANTS_INTERVPK  + "= '" + primKey + "' ";
  if (path_GUID)
         {requete += " AND " + m_DOSS_INTERVENANTS_PAT_GUID  + "= '" + path_GUID + "' ";
         }
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
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
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//---------------------------------------------------- PatientNoteIsThisExist -----------------------------------------------
/*! \brief  RETOUR:    la primKey des données si elles existent sinon "" */
QString CMoteurBase::PatientNoteIsThisExist(  const char *primKeyPat,
                                              QLabel          *errMess          /* = 0  */
                                           )
{ QString ret("");
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::IsThisPatientNoteExist(): database can't be opened");
      return ret;
     }
  //................. filtrer selon donnée d'entrée ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_NOTE_PK        + " FROM " + m_DOSS_NOTE_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_NOTE_PAT_PK    + " = '"   + primKeyPat             + "' ";

  QSqlQuery query(requete , m_DataBase );

  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next()) ret = query.value(0).toString();
  CloseBase();
  return ret;
}

//-------------------------------------- PatientNoteDelete ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteDelete( const char *primKey)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  QString requete("DELETE FROM ");
          requete += m_DOSS_NOTE_TBL_NAME + " WHERE ";
          requete += m_DOSS_NOTE_PK       + "= '" + primKey                  + "' ";
  QSqlQuery sqlQuery (QString::null , m_DataBase );
  ok = sqlQuery.exec(requete);
  CloseBase();
  return ok;
}

//-------------------------------------- PatientNoteCreate ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteCreate(const char *primKeyPat , const char* guidPat,  const char* data)
{ QString pk  = "";
  int     ok  = FALSE;
  if (OpenBase()==0)  return 0;

  QString prepare  = "";
  prepare         += "INSERT INTO "  + m_DOSS_NOTE_TBL_NAME  + " ( \n";
  prepare         += m_DOSS_NOTE_PAT_PK   + " ,\n";
  prepare         += m_DOSS_NOTE_PAT_GUID + " ,\n";
  prepare         += m_DOSS_NOTE_HTML     + " )\n  VALUES (?,?,?) ";
  QSqlQuery query(m_DataBase);
  query.prepare(prepare);
  query.bindValue(0, primKeyPat);
  query.bindValue(1, guidPat);
  query.bindValue(2, data);
  query.exec();
  CloseBase();
  return ok;
/*

  //............... crer un curseur SQL ..................................................
  QSqlCursor cur(m_DOSS_NOTE_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  buffer->setValue( m_DOSS_NOTE_PAT_PK,        primKeyPat );  // y placer les données
  buffer->setValue( m_DOSS_NOTE_PAT_GUID,      guidPat );     // y placer les données
  buffer->setValue( m_DOSS_NOTE_HTML,          data    );     // y placer les données
  ok = cur.insert();                                          // ecrire le buffer avec les données du header
  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
 */
}

//-------------------------------------- PatientNoteWrite ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteWrite( const char *primKeyPat , const char* guidPat,  const char* data)
{QString pk = PatientNoteIsThisExist(primKeyPat);
 if (pk.length()>0)
    {if (data[0]==0) return PatientNoteDelete(pk);
     else            return PatientNoteUpdate(pk, primKeyPat , guidPat,   data);
    }
 else
    {if (data[0]!=0) return PatientNoteCreate(primKeyPat , guidPat,   data);
    }
 return 0;
}

//-------------------------------------- PatientNoteUpdate ----------------------------------------------------------------------------
int CMoteurBase::PatientNoteUpdate(const char* primKey, const char *primKeyPat , const char* guidPat,  const char* data)
{
  if (OpenBase()==0)  return FALSE;
  QString prepare  = QString("UPDATE ")  + m_DOSS_NOTE_TBL_NAME   + " SET \n";
  prepare         += m_DOSS_NOTE_PAT_PK   + " = ? ,\n";
  prepare         += m_DOSS_NOTE_PAT_GUID + " = ? ,\n";
  prepare         += m_DOSS_NOTE_HTML     + " = ?  \n";
  prepare         += " WHERE " + m_DOSS_NOTE_PK   + " ='"  + primKey  + "'";
  QSqlQuery query(m_DataBase);
  query.prepare(prepare);
  query.bindValue(0, primKeyPat);
  query.bindValue(1, guidPat);
  query.bindValue(2, data);
  int ok =  query.exec();
  CloseBase();
  return ok;
  /*
 //................................. methode QSqlCursor .................................
 //                         ne pose pas de PB avec valeur avec une apostrophe
 //............... crer un curseur SQL ..................................................
 if (OpenBase()==0)  return FALSE;
 int ok = FALSE;
 QSqlCursor cur(m_DOSS_NOTE_TBL_NAME, TRUE, m_DataBase);
 if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
 //............... placer ce curseur sur le bon enregistrement ..........................
 cur.select(m_DOSS_NOTE_PK  + "='" + primKey  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                             // recuperer le buffer ( pas de delete à la fin !!)
     buffer->setValue( m_DOSS_NOTE_PAT_PK,        primKeyPat );  // y placer les données
     buffer->setValue( m_DOSS_NOTE_PAT_GUID,      guidPat );     // y placer les données
     buffer->setValue( m_DOSS_NOTE_HTML,          data    );     // y placer les données
     ok    = cur.update();                                               // ecrire le buffer avec les données du header
    }
 if (ok==0) OutSQL_error(cur, "PatientNoteUpdate()");

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
 */
}

//-------------------------------------- PatientNoteGetData ----------------------------------------------------------------------------
void CMoteurBase::PatientNoteGetData(const char *primKeyPat ,  QString& data, QString *errMess /*=0*/)
{ data = "";
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) *errMess = "CMoteurBase::PatientNoteGetData(): database can't be opened";
      return ;
     }
  //................. filtrer selon donnée d'entrée ..................................................
  QString requete("");
  requete  += "SELECT " + m_DOSS_NOTE_HTML        + " FROM " + m_DOSS_NOTE_TBL_NAME   + " WHERE ";
  requete  +=             m_DOSS_NOTE_PAT_PK      + " = '"   + primKeyPat             + "' ";

  QSqlQuery query(requete , m_DataBase );

  //.................. si la requète a un resultat ..............................................
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
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)
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
  QSqlCursor cur(m_USER_PERMS_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canUpdate ()) {/*DeVerrouilleTable(m_DataBase); */CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_USER_PERMS_PK + "='" + primKey + "'");
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer ( pas de delete à la fin !!)
     buffer->setValue( m_USER_PERMS_FRIEND_DROITS,        droits );      // y placer les données
     buffer->setValue( m_USER_PERMS_SIGN_GUID,            signGUID );    // y placer les données
     buffer->setValue( m_USER_PERMS_FRIEND_GUID,          permsGUID );   // y placer les données
     buffer->setValue( m_USER_PERMS_FRIEND_BLOBS,         blobs );       // y placer les données
     ok = cur.update();                                                  // ecrire le buffer avec les données du header
    }
  //......................... fermer la base ..............................................
  //DeVerrouilleTable(m_DataBase);
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
  QSqlCursor cur(m_USER_PERMS_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                             // recuperer le buffer d'insertion
  buffer->setValue( m_USER_PERMS_FRIEND_DROITS,        droits );      // y placer les données
  buffer->setValue( m_USER_PERMS_SIGN_GUID,            signGUID );    // y placer les données
  buffer->setValue( m_USER_PERMS_FRIEND_GUID,          permsGUID );   // y placer les données
  buffer->setValue( m_USER_PERMS_FRIEND_BLOBS,         blobs );       // y placer les données
  ok = cur.insert();                                                  // ecrire le buffer avec les données du header

  //................. reselectionner l'enregistrement créé pour retrouver son PK avec le GUID temporaire........................
  QString prim_key = QString::null;
  QString   requete("SELECT ");
  requete += m_USER_PERMS_PK            + " FROM " + m_USER_PERMS_TBL_NAME   + " WHERE "
          +  m_USER_PERMS_SIGN_GUID     + "='"     + signGUID                + "' AND  "
          +  m_USER_PERMS_FRIEND_GUID   + "='"     + permsGUID               + "' "; // retrouver le Pk de l'enregistrement créé avec le GUID temporaire
  QSqlQuery sqlQuery(requete , m_DataBase );
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
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
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
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
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
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//-----------------------------------------------------  PermsUserGetList -------------------------------------------
//   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_IDENT_NOM;                // Nom
//   QString      m_USER_IDENT_PRENOM;             // Prenom

long CMoteurBase::PermsUserGetList(          QListView *pQlistView,
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
//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)
 /*
 QString requete ="SELECT " + m_USER_IDENT_TBL_NAME  + "." + m_USER_IDENT_LOGIN                + ", "       // 0
                            + m_USER_IDENT_TBL_NAME  + "." + m_USER_IDENT_NOM                  + ", "       // 1
                            + m_USER_IDENT_TBL_NAME  + "." + m_USER_IDENT_PRENOM               + ", "       // 2
                            + m_USER_PERMS_TBL_NAME  + "." + m_USER_PERMS_FRIEND_DROITS        + ", "       // 3
                            + m_USER_PERMS_TBL_NAME  + "." + m_USER_PERMS_PK                   + ", "       // 4
                            + m_USER_IDENT_TBL_NAME  + "." + m_USER_IDENT_GUID                 + " FROM "   // 5
                            + m_USER_PERMS_TBL_NAME  + " INNER JOIN " + m_USER_IDENT_TBL_NAME  + " ON "
                            + m_USER_IDENT_TBL_NAME  + "." + m_USER_IDENT_GUID + " = " + m_USER_PERMS_TBL_NAME + "." + m_USER_PERMS_SIGN_GUID + " WHERE "
                            + m_USER_PERMS_SIGN_GUID + " = '" + signGUID  + "' ORDER BY " +  m_USER_IDENT_TBL_NAME  + "." + m_USER_IDENT_LOGIN;
*/
pQlistView->clear();
QString requete = "SELECT "  + m_USER_PERMS_PK                + ", "                                                // 0
                             + m_USER_PERMS_SIGN_GUID         + ", "                                                // 1
                             + m_USER_PERMS_FRIEND_GUID       + ", "                                                // 2
                             + m_USER_PERMS_FRIEND_DROITS     + ", "                                                // 3
                             + m_USER_PERMS_FRIEND_BLOBS      + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                             + m_USER_PERMS_SIGN_GUID         + " = '"   + signGUID +"'";
  QSqlQuery query(requete , m_DataBase );
  //Param_UpdateToDisk("/home/ro/TestSql.txt", requete);
  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int       nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {QString requetePermsUser = "SELECT "  + m_USER_IDENT_LOGIN                + ", "                                                 // 0
                                               + m_USER_IDENT_NOM                  + ", "                                                 // 1
                                               + m_USER_IDENT_PRENOM               + " FROM " + m_USER_IDENT_TBL_NAME + " WHERE  "        // 2
                                               + m_USER_IDENT_GUID                 + " = '"   + Utf8_Query(query, 2 ) + "'";              // m_USER_PERMS_FRIEND_GUID
         QSqlQuery queryPermsUser(requetePermsUser , m_DataBase );
          if (queryPermsUser.isActive())
            {
             while (queryPermsUser.next())
                   { QListViewItem *pQListViewItem = new QListViewItem( pQlistView ,
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
     { statutMess->setText(TR("Trouvés: ") + QString::number(nb));
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
  nir = nir.remove(' ').left(9);      // corriger les évntuels espace parasites rentrés à la main et infos suplémentaires
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
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
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {while (query.next())
           {QString numOrdre = query.value( 1 ).toString();
            if (numOrdre.remove(' ').startsWith(nir))
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
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete("SELECT ");
  requete += m_USER_IDENT_PRIMKEY + " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_GUID + "='" + guid +"'";
  QSqlQuery query(requete , m_DataBase );
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
 QSqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, m_DataBase);                     // ouvrir un curseur sur la table
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
 QSqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, m_DataBase);                      // ouvrir un curseur sur la table
 if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}                  // cassos si peut pas etre mis a jour
 if ( !cur.select( m_USER_IDENT_LOGIN  + "='" + user + "'" ))  goto SaveUserPassWordError;

 if (cur.next())
    {QSqlRecord *buffer = cur.primeUpdate();                                   // recuperer le buffer ( pas de delete à la fin !!)
     buffer->setValue( m_USER_IDENT_PASSWORD,   cripted_password);
     cur.update();                                                     // ecrire le buffer avec les données
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
                                     QString*        pFoot,       // adresse de la chaine où l'on veut recuperer le pied de page
                                     int             sub_type,    // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                     QString*        pk_record)   // = 0;
{QString       ret = "";
 if (pFoot) *pFoot = "";
 //..................... rechercher PK de l'utilisateur ...................
 QString userPk = GetUserPrimKey(user);
 if (userPk == "")   return ret;

 //..................... rechercher PK des données ........................
 QString dataPk = IsThisUserDocExist(type, userPk, nameDocType);
 if (pk_record)  *pk_record = dataPk;
 if (dataPk == "")   return ret;
 return GetUserDocument(dataPk, type,  pFoot,  sub_type );
}

//-----------------------------------------------------  GetUserDocument -------------------------------------------
QString CMoteurBase::GetUserDocument(QString         dataPk,
                                     int             type,
                                     QString*        pFoot,       // adresse de la chaine où l'on veut recuperer le pied de page
                                     int             sub_type     // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                     )
{QString       ret = "";
 //..................... rechercher les données ...........................
 QString text = GetUserDataFromPrimKey(dataPk);
 if (text == "")     return ret;

 //..................... retirer les bonnes données ........................
 //                     a priori on considère que le recto
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
void CMoteurBase::OutSQL_error(QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/)
{    QSqlError error = cur.lastError();
     QString   qserr("");

     if (error.type() != QSqlError::None)
        {switch (error.type())
           { case    QSqlError::None:           qserr =" - no error occurred: "          ; break;
             case    QSqlError::Connection:     qserr =" - connection error: "           ; break;
             case    QSqlError::Statement:      qserr =" - SQL statement syntax error: " ; break;
             case    QSqlError::Transaction:    qserr =" - transaction failed error: "   ; break;
             case    QSqlError::Unknown:        qserr =" - unknown error: "              ; break;
             default:                           qserr =" - unknown error: "              ; break;
           }
         if (messFunc) qDebug(messFunc);
         qDebug(qserr + error.databaseText ());
         if (requete) qDebug(requete);
        }
}


//-----------------------------------------------------  GetUserPermisions -------------------------------------------
QString CMoteurBase::GetUserPermisions( QString &user, QString    *errMess /* = 0 */ )
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = TR("CMoteurBase::GetUserPermisions(): database can't be opened");       return QString::null; }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString droits = "";
  QString requete("SELECT ");
  requete       += m_USER_IDENT_DROITS  +  " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_LOGIN + " = '" + user +"'";
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {while (query.next())  droits = Utf8_Query(query, 0 );
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return droits;
}

//-----------------------------------------------------  SetUserPermisions -------------------------------------------
bool CMoteurBase::SetUserPermisions( const QString &primkey, const QString &droits, QString    *errMess /* = 0 */ )
{ bool ok = FALSE;
  QString requete            = "";
  requete                   += "UPDATE "                  + m_USER_IDENT_TBL_NAME    + " SET ";
  requete                   +=  m_USER_IDENT_DROITS       + " = '" + droits          + "' WHERE ";
  requete                   +=  m_USER_IDENT_PRIMKEY      + " ='"  + primkey         + "'";
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = TR("CMoteurBase::SetUserPermisions(): database can't be opened");       return ok; }
  QSqlQuery query(m_DataBase );
  query.exec(requete);
  CloseBase();
  return ok;
}

//-----------------------------------------------------  UserDroitsCount -------------------------------------------
int CMoteurBase::UserDroitsCount(const QString &droitToFind , QString    *errMess)
{ int nb = 0;
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = TR("CMoteurBase::GetUserPermisions(): database can't be opened");       return -1; }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString droits = "";
  QString requete("SELECT ");
  requete       += m_USER_IDENT_DROITS  +  " FROM " + m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_DROITS + " LIKE '%"+droitToFind+"%'";
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {while (query.next())  ++nb;
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return nb;
}
//-----------------------------------------------------  GetUserList -------------------------------------------
void CMoteurBase::GetUserList( QListView *pQlistView, const QStringList &listUserAlreadyPresent, const QString &specialite /* = "" */, const QString &uniqueUser/* = "" */, QString    *errMess /* = 0 */ )
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) *errMess = "CMoteurBase::GetUserList(): database can't be opened";       return ; }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete("SELECT ");
  requete       += m_USER_IDENT_LOGIN    + "," +            // 0
                   m_USER_IDENT_NOM      + "," +            // 1
                   m_USER_IDENT_PRENOM   + "," +            // 2
                   m_USER_IDENT_PRIMKEY  + "," +            // 3
                   m_USER_IDENT_GUID     + " FROM  " +      // 4
                   m_USER_IDENT_TBL_NAME + " WHERE " + m_USER_IDENT_DROITS  + " LIKE '%-med%' ";
  if (specialite.length()) requete       += " AND "  + m_USER_IDENT_QUALITE + " = '"  + specialite  + "' ";
  if (uniqueUser.length()) requete       += " AND "  + m_USER_IDENT_LOGIN   + " = '"  + uniqueUser  + "' ";
  requete       +=  " ORDER BY " + m_USER_IDENT_LOGIN;
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {
      while (query.next())
            {QString str = Utf8_Query(query, 0 );
             if (listUserAlreadyPresent.findIndex(str)==-1) new QListViewItem(     pQlistView,
                                                                                   str,
                                                                                   Utf8_Query(query, 1 ),
                                                                                   Utf8_Query(query, 2 ),
                                                                                   Utf8_Query(query, 3 ),
                                                                                   Utf8_Query(query, 4 )
                                                                              );
            }
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
}

//-----------------------------------------------------  GetUserList -------------------------------------------
void CMoteurBase::GetUserList( QStringList &qstringList, QLabel    *errMess /* = 0 */ )
{ //................. Preparer la requete .....................................
  if (OpenBase()==0)  {if (errMess) errMess->setText("CMoteurBase::GetUserList(): database can't be opened");       return ; }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete("SELECT ");
  requete       += m_USER_IDENT_LOGIN + ", " + m_USER_IDENT_DROITS  +  " FROM " +
                   m_USER_IDENT_TBL_NAME     + " WHERE " + m_USER_IDENT_DROITS + " LIKE '%-sgn%' ORDER BY " + m_USER_IDENT_LOGIN;
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {qstringList.clear();
      while (query.next())  qstringList.append(Utf8_Query(query, 0 ));
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
}

//-----------------------------------------------------  PermsUserFatherGetQlistView -------------------------------------------
//   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_IDENT_NOM;                // Nom
//   QString      m_USER_IDENT_PRENOM;             // Prenom

void CMoteurBase::PermsUserFatherGetQlistView( QListView* pQlistView, const QString  &friendUser , const QString  &imagePath , QString    *errMess /* = 0 */)
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)
  //QString signGUID   = GetFieldValue( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, signUser);
  QString friendGUID = GetFieldValue( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, friendUser);

  if (OpenBase()==0)  {if (errMess) *errMess = "CMoteurBase::GetUserList(): database can't be opened";       return ; }

  QString requete ="SELECT "  + m_USER_IDENT_LOGIN                + ", "    // 0                                             // 0
                              + m_USER_IDENT_NOM                  + ", "    // 1                                             // 1
                              + m_USER_IDENT_PRENOM               + ", "    // 2
                              + m_USER_IDENT_PRIMKEY              + ", "    // 3
                              + m_USER_IDENT_GUID                 + "  "    // 4
                              + " FROM " + m_USER_IDENT_TBL_NAME  + " WHERE " + m_USER_IDENT_DROITS + " LIKE '%-sgn%' ORDER BY " + m_USER_IDENT_LOGIN;
  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {while (query.next())
        {QString login = Utf8_Query(query, 0 );
         if (login != friendUser)
            { QListViewItem *pQListViewItem = new QListViewItem(  pQlistView ,
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
                 QSqlQuery queryPermsUser(requetePermsUser , m_DataBase );
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
//-----------------------------------------------------  PermsUserChildGetList -------------------------------------------
//   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_IDENT_NOM;                // Nom
//   QString      m_USER_IDENT_PRENOM;             // Prenom

void CMoteurBase::PermsUserChildGetList(          QListViewItem *pQlistViewItem,
                                                  const QString  &signGUID )
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)

  QString requete = "SELECT "  + m_USER_PERMS_FRIEND_GUID       + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_SIGN_GUID         + " = '"   + signGUID              + "'";
  QSqlQuery query(requete , m_DataBase );
  //Param_UpdateToDisk("/home/ro/TestSql.txt", requete);
  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {
      while (query.next())
        {QString requetePermsUser = "SELECT "  + m_USER_IDENT_LOGIN                + ", "    // 0                                             // 0
                                               + m_USER_IDENT_NOM                  + ", "    // 1                                             // 1
                                               + m_USER_IDENT_PRENOM               + ", "    // 2
                                               + m_USER_IDENT_PRIMKEY              + ", "    // 3
                                               + m_USER_IDENT_GUID                 + "  "    // 4
                                               + " FROM " + m_USER_IDENT_TBL_NAME  + " WHERE  " + m_USER_IDENT_GUID + " = '"   + Utf8_Query(query, 0 ) + "'";              // m_USER_PERMS_FRIEND_GUID
         QSqlQuery queryPermsUser(requetePermsUser , m_DataBase );
          if (queryPermsUser.isActive())
            {
             while (queryPermsUser.next())
                   { /*QListViewItem *pQListViewItem = */ new QListViewItem( pQlistViewItem ,
                                                                        Utf8_Query(queryPermsUser, 0 ),       // login
                                                                        Utf8_Query(queryPermsUser, 1 ),       // nom
                                                                        Utf8_Query(queryPermsUser, 2 ),       // prenom
                                                                        Utf8_Query(queryPermsUser, 3 ),       // PK
                                                                        Utf8_Query(queryPermsUser, 4 )        // GUID
                                                                       );
                  }  //endwhile (queryPermsUser.next())
           } //endif (queryPermsUser.isActive())
        } //end while (pSqlQuery->next())4
     } //endif (pSqlQuery && pSqlQuery->isActive())
}

//-----------------------------------------------------  PermsUserIsThisUserFriendFromName -------------------------------------------
// ACTION : retourne les droits autorisés par signUser pour friendUser
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
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  { return 0; }

  QString userGUID   = GetFieldValue_WithoutOpenCloseBase( m_USER_IDENT_TBL_NAME, m_USER_IDENT_GUID, m_USER_IDENT_LOGIN, user);

  QString requete = "SELECT "  + m_USER_PERMS_SIGN_GUID + ", "    +  m_USER_PERMS_FRIEND_DROITS   + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_FRIEND_GUID         + " = '"                        + userGUID                         + "' ;";
  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
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
// ACTION vérifie si le droit : droitToFind  (trois caractères style med adm sgn ....) existe dans la chaine de droits : listDroits
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
// ACTION : retourne les droits autorisés par signGUID pour friendGUID
//
QString CMoteurBase::PermsUserIsThisUserFriendFromGUID(   const QString  &signGUID, const QString  &friendGUID)
{//  pt = SetConfBase_SetProperties(deb,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue à cet utilisateur
 //  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)
  //................. Preparer la requete .....................................
  if (OpenBase()==0)  { return NULL; }
  QString droits  = "";
  QString requete = "SELECT "  + m_USER_PERMS_FRIEND_DROITS       + " FROM " + m_USER_PERMS_TBL_NAME + " WHERE "        // 4
                               + m_USER_PERMS_SIGN_GUID           + " = '"   + signGUID              + "' AND  "
                               + m_USER_PERMS_FRIEND_GUID         + " = '"   + friendGUID            + "';";
  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {while (query.next())
        {droits = Utf8_Query(query, 0 );
        } //end while (pSqlQuery->next())4
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return droits;
}

//-----------------------------------------------------  GetUserListType -------------------------------------------
void CMoteurBase::GetUserListType( QStringList &typList , QString    *errMess /* = 0 */ )
{//................. Preparer la requete .....................................
  if (OpenBase()==0) {if (errMess) *errMess = "CMoteurBase::GetUserList(): database can't be opened"; return ; }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString        requete;
  requete        = "SELECT ";
  requete       += m_USER_IDENT_QUALITE ; // 0
  requete       += " FROM "      + m_USER_IDENT_TBL_NAME     + " WHERE ";
  requete       += m_USER_IDENT_DROITS + " LIKE '%-med%'";
  requete       += " ORDER BY "  + m_USER_IDENT_QUALITE;

  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {typList.clear();
      while (query.next())
        {QString str = Utf8_Query(query, 0 );
         if (typList.findIndex ( str)==-1) typList.append(str);
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
}

//-----------------------------------------------------  GetUserList -------------------------------------------
//   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_IDENT_NOM;                // Nom
//   QString      m_USER_IDENT_PRENOM;             // Prenom

long CMoteurBase::GetUserList(          QListView *pQlistView,
                                  const QString   &qstr_nom,
                                  const QString   &qstr_prenom,

                                  const bool       childsMustBeFind,  /* = TRUE */
                                  const QString   *qstr_UserQualite,  /* = 0 */
                                  QLabel    *statutMess, /* = 0  */
                                  QLabel    *errMess     /* = 0  */
                             )
{//................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText("CMoteurBase::GetUserList(): database can't be opened");
      return 0;
     }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
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
      if (qstr_UserQualite && qstr_UserQualite->length())
         {requete   += m_USER_IDENT_QUALITE + " = '"  + *qstr_UserQualite  + "' AND ";
         }
      requete       += m_USER_IDENT_DROITS + " LIKE '%-med%'";
      requete       += " ORDER BY "        + m_USER_IDENT_NOM;
     }
  //................. SINON pas de filtre ...........................................................
  else
     {requete       += " FROM "      + m_USER_IDENT_TBL_NAME     + " WHERE ";
      requete       += m_USER_IDENT_DROITS + " LIKE '%-med%'";
      if (qstr_UserQualite && qstr_UserQualite->length())
         {requete   += " AND " + m_USER_IDENT_QUALITE + " = '"  + *qstr_UserQualite  + "' ";
         }
      requete       += " ORDER BY "  + m_USER_IDENT_NOM;
     }

  QSqlQuery query(requete , m_DataBase );

  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {QListViewItem *element = 0;
         QString droits         = query.value( 5 ).toString();
         //............... on rempli la listbox selon son nombre de colonnes .....................
         //if (  /*droits.find("adm")!=-1 ||  (droits.find("med") != -1  ) */)
            {if (childsMustBeFind )
                {if (droits.find("-sgn") != -1)
                    {element = new QListViewItem(     pQlistView,
                                                      Utf8_Query(query, 0 ),
                                                      Utf8_Query(query, 1 ),
                                                      Utf8_Query(query, 2 ),
                                                      Utf8_Query(query, 3 ),
                                                      Utf8_Query(query, 4 )
                                                 );
                    if (element)
                        {PermsUserChildGetList(element, element->text(4));
                         //element->setOpen ( TRUE );
                         ++nb;
                        }
                   } // endif (droits.find("sgn") != -1)
               }
            else
               {element = new QListViewItem(          pQlistView,
                                                      Utf8_Query(query, 0 ),
                                                      Utf8_Query(query, 1 ),
                                                      Utf8_Query(query, 2 ),
                                                      Utf8_Query(query, 3 ),
                                                      Utf8_Query(query, 4 )
                                                 );
                if (element) ++nb;
               }
           }
          //if (element) pQlistView->insertItem ( element );
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())

  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { statutMess->setText(TR("Trouvés: ") + QString::number(nb));
     }
  CloseBase();
  return nb;
}

//-----------------------------------------------------  InitComboWithUserDocList -------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
//   QString      m_USER_DATA_NAMEDOC;            // nom des données
//   QString      m_USER_DATA_TYPE;               // type de données
//   QString      m_USER_DATA_BLOB;               // données proprement dites
//   ACTION: cette fonction rempli la combo box de choix du type de masque d'impression à utiliser
long CMoteurBase::InitComboWithUserDocList( QComboBox       *pQComboBox,
                                            QString         user,
                                            QString       rubName,
                                            QString         type1, /* = QString::null  ou RUBNAME alors type2 contient le nom de la rubrique*/
                                            QString         type2, /* = QString::null  ou nom de la rubrique */
                                            int clear             /* = 1 */
                                          )
{ //................. tester l'utilisateur .....................................
  QString primKey = GetUserPrimKey(user);
  bool       oldSyntax = TRUE;
  if (primKey=="") return 0;

  //................. tester disponibilite des bases .....................................
  if (OpenBase()==0)  return 0;

  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete("SELECT ");
          requete       += m_USER_DATA_NAMEDOC +","+
                           m_USER_DATA_TYPE    +","+
                           m_USER_DATA_PK;
          requete       += " FROM "       + m_USER_DATA_TBL_NAME       + " WHERE ";
  //................. forme particulière des nouvelles rubriques à venir où ..................................
  //                  le libelle du document d'entete et de pied de page doit être
  //                  de la forme : [NOM DE LA RUBRIQUE] LIBELLE DE LA RUBRIQUE (avec le # ou * et eventuels préfixages)
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

  QSqlQuery query(requete , m_DataBase );

  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int nb   = 0;
  int pos  = -1;
  QString text;
  QString type_doc;
  QString rubNameInTitle;
  QString subTypeInTitle;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {if (clear) pQComboBox->clear();
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
             {new CPrtQListBoxItem(pQComboBox->listBox() ,                                 // list box du combobox
                                          text ,      // libelle
                                          Utf8_Query(query, 2 ) ,          // Pk document
                                          Utf8_Query(query, 1 ) ,          // type document
                                          user,                            // user
                                          subTypeInTitle );             // sous type (non utilisé ici)
                     nb++;

             }
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  CloseBase();
  return nb;
}

//-----------------------------------------------------  GetUserDocList -------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
//   QString      m_USER_DATA_NAMEDOC;            // nom des données
//   QString      m_USER_DATA_TYPE;               // type de données
//   QString      m_USER_DATA_BLOB;               // données proprement dites

long CMoteurBase::GetUserDocList( QListView       *pQlistView,
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
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
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
  QSqlQuery query(requete , m_DataBase );

  //................ scanner les enregistrements ....................................................
  //                 pour remplir la listview
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {//............... on rempli la listbox selon son nombre de colonnes .....................
         new QListViewItem( pQlistView,
                            Utf8_Query(query, 0 ),   ////Utf8_Query(query,0);
                            Utf8_Query(query, 1 ),
                            Utf8_Query(query, 2 )
                          );
          nb++;
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { statutMess->setText(TR("Trouvés: ") + QString::number(nb));
     }
  CloseBase();
  return nb;
}

//-------------------------------------- CreateNewUser ----------------------------------------------------------------------------
// ACTION: crée un nouvel utilisateur et retourne son GUID
//m_pCMoteurBase->CreateNewUser(m_pCps->m_Nom, m_pCps->m_Prenom, user, "med", pass, m_pCps->m_NIR);

QString CMoteurBase::CreateNewUser(const QString &nom,
                                   const QString &prenom,
                                   const QString &login,
                                   const QString &droit /*="med" */,
                                   const QString &pass/*=""*/,
                                   const QString &nir/*=""*/
                                  )
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  QString ok("");
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return ok;
  //............... crer un curseur SQL ..................................................
  QSqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ())                                        return ok;
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  QString       guid = GUID_Create();
  buffer->setValue( m_USER_IDENT_GUID,   guid);                   // y placer les données
  buffer->setValue( m_USER_IDENT_NOM,    nom);                    // y placer les données
  buffer->setValue( m_USER_IDENT_PRENOM, prenom);                 // y placer les données
  buffer->setValue( m_USER_IDENT_LOGIN,  login);                  // y placer les données
  if (droit.length())buffer->setValue( m_USER_IDENT_DROITS,     droit);                  // y placer les données
  if (pass.length())
     {QString pass_cripted = pass;
      PassWordEncode(pass_cripted);
      buffer->setValue(  m_USER_IDENT_PASSWORD, pass_cripted);                   // y placer les données
     }
  if (nir.length())buffer->setValue(   m_USER_IDENT_NUM_ORDRE,  nir);                    // y placer les données
  if (cur.insert()) ok = guid;                                    // ecrire le buffer

  if (ok=="") OutSQL_error(cur, "CreateNewUser()");

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}
//---------------------------------------------------- IsThisUserDocExist -----------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
//   QString      m_USER_DATA_NAMEDOC;            // nom des données
//   QString      m_USER_DATA_TYPE;               // type de données
//   QString      m_USER_DATA_BLOB;               // données proprement dites
// RETOUR:    la primKey des données si elles existent sinon ""
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
  //................. filtrer selon donnée d'entrée ..................................................
  QString  requete("SELECT " );
           requete       += m_USER_DATA_PK;
           requete       += " FROM "               + m_USER_DATA_TBL_NAME              + " WHERE ";
  if (nom) requete       += m_USER_DATA_NAMEDOC    + " = '"   + nom                    + "' AND ";
           requete       += m_USER_DATA_REF_PK     + " = '"   + refUserPrimKey         + "' AND ";
           requete       += m_USER_DATA_TYPE       + " = '"   + QString::number(type)  + "'";

  QSqlQuery query(requete , m_DataBase );

  //.................. si la requète a un resultat ..............................................

  if (query.isActive() && query.next())
     {ret = query.value(0).toString();
     }
  //if (ret.length()<=0) ret = IsThisUserDocExist( type, refUserPrimKey, QString (nom).utf8 (), errMess ); // essayer en utf8
  CloseBase();
  return ret;
}

//---------------------------------------------------- IsThisUserDocExist -----------------------------------------------
//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
//   QString      m_USER_DATA_REF_PK;             // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
//   QString      m_USER_DATA_NAMEDOC;            // nom des données
//   QString      m_USER_DATA_TYPE;               // type de données
//   QString      m_USER_DATA_BLOB;               // données proprement dites
// RETOUR:    la primKey des données si elles existent sinon ""

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
  //................. filtrer selon donnée d'entrée ..................................................
  QString  requete("SELECT " );
           requete       += m_USER_DATA_PK   + "," + m_USER_DATA_NAMEDOC         + "," + m_USER_DATA_TYPE;
           requete       += " FROM "               + m_USER_DATA_TBL_NAME              + " WHERE ";
           requete       += m_USER_DATA_REF_PK     + " = '"   + refUserPrimKey         + "' ";


  QSqlQuery query(requete , m_DataBase );
  //.................. si la requète a un resultat ..............................................
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
                 {libelle_modele = libelle_modele.remove('#');;            // se placer après le '# '
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
{   //......................... sauver les parametres dans données utilisateur ....................
    //if (pParam==0) pParam = &m_ParamData;
    QString us_pk         = "";
    if (user[0]=='%') us_pk  = user.mid(1);
    else              us_pk  = GetUserPrimKey(user);
    QString docPrimKey       = IsThisUserDocExist(TYP_PARAM, us_pk, TR("Parametres"));
     if (docPrimKey!="")
        {UpdateUserDataFromPrimKey(docPrimKey, TYP_PARAM, us_pk, TR("Parametres"), *pParam);
        }
     else
        {CreateUserDataFromPrimKey(TYP_PARAM, us_pk ,TR("Parametres"), *pParam);
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
 QSqlCursor cur(m_USER_DATA_TBL_NAME, TRUE, m_DataBase);
 if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
 //............... placer ce curseur sur le bon enregistrement ..........................
 cur.select(m_USER_DATA_PK  + "='" + primKey  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                                                 // recuperer le buffer ( pas de delete à la fin !!)
     if (refUserPrimKey) buffer->setValue( m_USER_DATA_REF_PK,    refUserPrimKey         );  // y placer les données
     if (name)           buffer->setValue( m_USER_DATA_NAMEDOC,   name                   );  // y placer les données
     if (type)           buffer->setValue( m_USER_DATA_TYPE,      type                   );  // y placer les données
     buffer->setValue( m_USER_DATA_BLOB,      data                   );  // y placer les données
     ok    = cur.update();                                               // ecrire le buffer avec les données du header
    }

 if (ok==0) OutSQL_error(cur, "UpdateUserDataFromPrimKey()");

  //......................... fermer la base ..............................................
  CloseBase();
  return ok;
}

//------------------------------------------ ReadDrTuxUserParametres ---------------------------------------------------------------
QString  CMoteurBase::ReadDrTuxUserParametres(const QString &user)
{return GetUserDocument(user, TR("Parametres"), TYP_PARAM );
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
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
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
 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}
//-------------------------------------- CreateUserDataFromPrimKey ----------------------------------------------------------------------------
int CMoteurBase::CreateUserDataFromPrimKey(int type, const char *primKey , const char* name,  const char* data)
{//  "USER_DATA_TBL_NAME        'UtilisateursDocuments'        \r\n"      // nom de la table des données des RUBRIQUES
 //  "USER_DATA_REF_PK          'ID_PrimKeyFromUser'      '0' \r\n"       // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
 //  "USER_DATA_NAMEDOC         'NameDoc'                 '1' \r\n"       // nom des données
 //  "USER_DATA_TYPE            'TypeDocuments'           '2' \r\n"       // type de données
 //  "USER_DATA_BLOB            'DataDocuments'           '3' \r\n";      // données proprement dites
 //   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
 //   QString      m_USER_DATA_REF_PK;             // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
 //   QString      m_USER_DATA_NAMEDOC;            // nom des données
 //   QString      m_USER_DATA_TYPE;               // type de données
 //   QString      m_USER_DATA_BLOB;               // données proprement dites

  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //............... crer un curseur SQL ..................................................
  QSqlCursor cur(m_USER_DATA_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ()) {CloseBase(); return FALSE;}
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  buffer->setValue( m_USER_DATA_REF_PK,         primKey );  // y placer les données
  buffer->setValue( m_USER_DATA_NAMEDOC,        name    );  // y placer les données
  buffer->setValue( m_USER_DATA_TYPE,           type    );  // y placer les données
  buffer->setValue( m_USER_DATA_BLOB,           data    );  // y placer les données
  ok = cur.insert();                                               // ecrire le buffer avec les données du header

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
  //................................. effacer tous les documents liés à l'utilisateur.....................................
          requete  = "DELETE FROM ";
          requete += m_USER_DATA_TBL_NAME + " WHERE ";
          requete += m_USER_DATA_REF_PK       + "= '" + refUserPrimKey + "' ";
 pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 if (ok==0)             {CloseBase();  return 0;}  // si erreur on arrete là

 //................................. effacer l'utilisateur .....................................
          requete  = "DELETE FROM ";
          requete += m_USER_IDENT_TBL_NAME      + " WHERE ";
          requete += m_USER_IDENT_PRIMKEY       + "= '" + refUserPrimKey + "' ";
 pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 CloseBase();
 return ok;
}

//-------------------------------------- GetUserDataFromPrimKey ----------------------------------------------------------------------------
QString CMoteurBase::GetUserDataFromPrimKey(const char *primKey)
{//   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
 //   QString      m_USER_DATA_PK;                 // clef primaire des données
 //   QString      m_USER_DATA_NAMEDOC;            // nom des données
 //   QString      m_USER_DATA_TYPE;               // type de données
 //   QString      m_USER_DATA_BLOB;               // données proprement dites

 QString requete("SELECT ");
 requete  += m_USER_DATA_BLOB      + " FROM "      + m_USER_DATA_TBL_NAME  + " WHERE ";
 requete  += m_USER_DATA_PK        + " ='"         + primKey + "'";


 if (OpenBase()==0)  return 0;

 QSqlQuery query(requete , m_DataBase );
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
 //................... ajouter les champs à selectionner .......................
 for ( QStringList::Iterator it = fiedList.begin(); it != fiedList.end(); ++it )
     {requete  += *(it) + ",";
     }
 //.............. virer la dernière virgule ....................................
 int pos = requete.findRev(',');
 if (pos !=-1) requete.remove (pos, 1);
 //............... continuer à construire la requete ...........................
 requete  += " FROM "     + m_USER_IDENT_TBL_NAME + " WHERE ";
 requete  += m_USER_IDENT_PRIMKEY + " ='"         + primKey + "'";

 if (OpenBase()==0)
     {return 0;
     }
 QSqlQuery* pSqlQuery = new QSqlQuery(requete , m_DataBase );
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
 QSqlQuery* pSqlQueryTarget = new QSqlQuery(requete , m_DataBase  );

 if (pSqlQueryTarget && pSqlQueryTarget ->isActive())
    {if (errMess) *errMess = TR("Changement d'utilisateur réussi");
    }
 else
   {qstr   = "";
    qstr  += requete;
    qstr  += TR("\r\nÉchec du changement d'utilisateur : CMoteur_Base. ");
    switch(pSqlQueryTarget->lastError().type())
          {case QSqlError::None :        qstr  += TR("no error occurred");          break;
           case QSqlError::Connection :  qstr  += TR("connection error");           break;
           case QSqlError::Statement :   qstr  += TR("SQL statement syntax error"); break;
           case QSqlError::Transaction : qstr  += TR("transaction failed error");   break;
           case QSqlError::Unknown :     qstr  += TR("unknown error");              break;
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
QString  CMoteurBase::DossIdentIndexCreate(QString& nom, QString& prenom,    QString& numGUID,
                                           PtrListIdentFieldData *plist_field, int mode /*= CMoteurBase::esclave*/)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  QString ok  = "";
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return ok;

  //............... creer un curseur SQL de creation de l'enregistrement dans la table d'index des identités .................
  QSqlCursor cur(m_DOSS_INDEX_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ()) return ok;
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  buffer->setValue( m_DOSS_INDEX_GUID,     numGUID         );     // y placer les données
  buffer->setValue( m_DOSS_INDEX_NOM,      nom.upper()     );     // y placer les données
  buffer->setValue( m_DOSS_INDEX_PRENOM,   prenom.upper()  );     // y placer les données
  if (cur.insert())                                               // ecrire le buffer avec les données du header
     {ok = "TRUE";                                                  // s'occuper des données binaires BLOB
     }
  //......................... Creer l'enregistrement dans la table des données des identités .................................
  QSqlCursor cur_refPk(m_DOSS_INDEX_TBL_NAME, TRUE, m_DataBase);
  if (ok.length())
     {if (!cur_refPk.select( m_DOSS_INDEX_GUID+"='"+numGUID+"'" )) goto  DossIndexCreateError;
      if (cur_refPk.next()) {//..... ok prend la valeur du primary key qui sera retourné ............
                             ok = cur_refPk.value( m_DOSS_INDEX_PRIM_KEY).toString();
                             DossIdentDataCreate(ok, numGUID, plist_field, mode);
                            }
      else                  {ok = "";
                            }
     }
  CloseBase();
  //........................ recuperer la clef ...........................................
  return ok;

DossIndexCreateError:
  OutSQL_error(cur_refPk, "DossIndexCreate()", cur_refPk.filter());
  CloseBase();
  return QString("");
}

//-------------------------------------- DossIdentDataCreate --------------------------------------------------------------------
// ATTENTION: doit toujours etre appelée avec la base ouverte !!!
//
long  CMoteurBase::DossIdentDataCreate(QString& refDossPk,               QString& numGUID,
                                       PtrListIdentFieldData *plist_field, int /* mode = CMoteurBase::esclave*/)
{QString ref_pk;
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... creer un curseur SQL de creation de l'enregistrement dans la table d'index des identités .................
  QSqlCursor cur(m_DOSS_IDENT_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canInsert ()) return FALSE;
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
  //................... y placer les valeurs des champs ...................................
  //......... inserer les valeurs fondamentales ..................
  buffer->setValue( m_DOSS_IDENT_REF_PK, refDossPk );
  buffer->setValue( m_DOSS_IDENT_GUID,   numGUID );
  //......... inserer les autres valeurs .........................
  DossIdentBufferSetValue(buffer, plist_field );

  if (cur.insert())                                               // ecrire le buffer avec les données du header
     {ok = TRUE;                                                  // s'occuper des données binaires BLOB
     }
  //........................ recuperer la clef ...........................................
  return ok;
}

//-------------------------------------- DossIdentDelete --------------------------------------------------------------------
void  CMoteurBase::DossIdentDelete(RUBREC_LIST *pRubList, QString /*num_GUID*/, QString id_doss, int mode /*= CMoteurBase::esclave*/)
{//....................... addapter le comportement à un éventuel nomadisme ...............................................
 QString requete = "";
 //........................ on commence par effacer les rubriques ..................................
 RUBREC_LIST::iterator it;
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {if ((*it).m_PrimKey != "" && (*it).m_PrimKey != "-1" && (*it).m_PrimKey != "0" && (*it).m_PrimKey[0] != '0')
         {RubListDelete(it, mode);                                      // effacer la rubrique
         }
     } //end while (pSqlQuery->next())

 //........................... effacement du header et données de l'identité ................................
 int          ok  = FALSE;
 //.............................. ouvrir la base ............................................................
 if (OpenBase()==0)  return ;

 //............................. effacer les données identitaires ...........................................
 requete = "DELETE FROM ";
 requete +=  m_DOSS_IDENT_TBL_NAME + " WHERE " + m_DOSS_IDENT_REF_PK + "='" + id_doss + "'";

 QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_DataBase );
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 if (ok == FALSE) {CloseBase(); return ;}        // si erreur cassos

 //............................. effacer les blobs ...........................................................
 requete  = "DELETE FROM ";
 requete +=  m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_PRIM_KEY + "='" + id_doss + "'";

 pSqlQuery = new QSqlQuery(QString::null, m_DataBase);
 if (pSqlQuery) ok = pSqlQuery->exec(requete);
 if (pSqlQuery) delete  pSqlQuery;
 //............................. fermer la base .............................................................
 CloseBase();
 return ;
}

//-------------------------------------- DossIdentIndexUpdate --------------------------------------------------------------------
// POUR QUE CELA MARCHE il faut imperativement une primarykey dans la table sinon: cur.update() ne fonctionne pas !!!
// SI plist_field est egal à zero alors seulement le nom et prenom sont mis à jour.

long  CMoteurBase::DossIdentIndexUpdate(QString& refDossPk, QString& nom, QString& prenom,
                                        PtrListIdentFieldData *plist_field /* =0 */, int mode /*= CMoteurBase::esclave*/)
{ int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return FALSE;
  //............... crer un curseur SQL ..................................................
  QSqlCursor cur(m_DOSS_INDEX_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canUpdate ()) {CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_DOSS_INDEX_PRIM_KEY + "='" + refDossPk+"'");
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                                // recuperer le buffer ( pas de delete à la fin !!)
     buffer->setValue( m_DOSS_INDEX_NOM,     nom.upper()    );              // y placer les données
     buffer->setValue( m_DOSS_INDEX_PRENOM,  prenom.upper() );              // y placer les données
     cur.update();      // ecrire le buffer avec les données du header
     ok  = TRUE;
    }
  //......................... fermer la base ..............................................
  if (ok && plist_field)  // si
     {ok = DossIdentDataUpdate(refDossPk, plist_field, mode);
     }
  CloseBase();
  return ok;
}

//-------------------------------------- DossIdentDataUpdate --------------------------------------------------------------------
// POUR QUE CELA MARCHE il faut imperativement une primarykey dans la table sinon: cur.update() ne fonctionne pas !!!
// ATTENTION: doit toujours etre appelée avec la base ouverte !!!
//
long  CMoteurBase::DossIdentDataUpdate(QString& refDossPk, PtrListIdentFieldData *plist_field , int /*mode = CMoteurBase::esclave*/)
{ //............... crer un curseur SQL ..................................................
  QSqlCursor cur(m_DOSS_IDENT_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canUpdate ()) return FALSE;
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_DOSS_IDENT_REF_PK + "='" + refDossPk+"'");
  //............... si OK on recupere le buffer et on le charge avec les données .........
  if ( cur.next() )
     {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer ( pas de delete à la fin !!)
      DossIdentBufferSetValue(buffer, plist_field );
      cur.update();
      QString lastErr= cur. lastError ().text();
      qDebug(lastErr);
      return TRUE;
     }
  return FALSE;
}

//-------------------------------------- DossIdentBufferSetValue ---------------------------------------------------------------------

long  CMoteurBase::DossIdentBufferSetValue(QSqlRecord *buffer, PtrListIdentFieldData *plist_field  )
{//......... inserer les autres valeurs .........................
 CIdentFieldData *pCIdentFieldData;
 for ( pCIdentFieldData = plist_field->first(); pCIdentFieldData; pCIdentFieldData = plist_field->next() )
     {switch(pCIdentFieldData->m_Type)
            {case FTN_MAJ_STRING:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value.upper() );     // y placer les données
                  break;
             case FTN_STRING:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value);              // y placer les données
                  break;
             case FTN_NUM_STRING:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value);              // y placer les données
                  break;
             case FTN_INTEGER:
                  buffer->setValue( pCIdentFieldData->m_Name, pCIdentFieldData->m_Value.toInt());              // y placer les données
                  break;
             case FTN_CHAR:
                  {char c =  pCIdentFieldData->m_Value.at(1).latin1();
                   buffer->setValue( pCIdentFieldData->m_Name, c);
                  }
                  break;
             case FTN_DATETIME:   // "ddMMyyyy"
                  {QDate  qDteNss (pCIdentFieldData->m_Value.mid(4).toInt(),
                                           pCIdentFieldData->m_Value.mid(2,2).toInt(),
                                           pCIdentFieldData->m_Value.mid(0,2).toInt()
                                  );
                   buffer->setValue( pCIdentFieldData->m_Name, qDteNss);
                  }
              break;
              case FTN_DATETIME_TIRET:   // "yyyy-MM-dd hh:mn:ss"
                  {QDate  qDteNss (pCIdentFieldData->m_Value.mid(0,4).toInt(),
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
                QSqlQuery query(requete , m_DataBase );
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
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {while (query.next())
            {  QString evnmt_Pk = query.value( 1 ).toString();           // servira pour rechercher les fils
               //const char* pere, const char* pkDoc, const char* pk, const char* dossPk
               pEvnmtList->append(CRubEvntPair(evnmt_pere, query.value( 0 ).toString(), evnmt_Pk, query.value( 2 ).toString()));
               while (Evnmt_GetFils( pEvnmtList, evnmt_Pk));             // rechercher si ce fils n'est pas le pere d'autres
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
               QSqlQuery query(requete , m_DataBase );
              }
           //........ par contre les fils peuvent trés bien exister si deplaces d'ailleurs................
           Evnmt_EraseAllFils(pEvnmtList, (*it).m_Pk);
           (*it).setState(TO_DESTROY);
           pEvnmtList->Evnmt_Set_StateOn_AllFils(pk, TO_DESTROY);
           ++it;
          }
       else if ( (*it).IsStatePlaced(TO_CREATE))
          {QString oldRefPk = (*it).m_Pk;
           //............... crer un curseur SQL ..................................................
           QSqlCursor cur(m_EVNT_LIST_TBL_NAME, TRUE, m_DataBase);
           if ( !cur.canInsert ()) {CloseBase(); return;}
           //............... si OK on recupere le buffer et on le charge avec les données .........
           QSqlRecord *buffer = cur.primeInsert();                             // recuperer le buffer d'insertion
           buffer->setValue( m_EVNT_LIST_PKDOC,       (*it).m_PkDoc );         // y placer les données
           buffer->setValue( m_EVNT_LIST_PERE,        (*it).m_Pere );          // y placer les données
           buffer->setValue( m_EVNT_DOSS_PK,          (*it).m_DossPk );        // y placer les données
           int ok = cur.insert();                                                  // ecrire le buffer avec les données du header
            //................. reselectionner l'enregistrement créé pour retrouver son PK avec le GUID temporaire........................
           if (ok)
              { //................. recuperer dernier primKey insere ..............................
                requete  = "SELECT LAST_INSERT_ID() FROM ";
                requete += m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_LIST_PK + " = LAST_INSERT_ID()";
                QSqlQuery sqlQuery(requete , m_DataBase );
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
           QSqlQuery query(requete , m_DataBase );
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
           QSqlQuery query(requete , m_DataBase );
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
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {while (query.next())
            {  QString evnmt_PrimKey = query.value( 0 ).toString();           // servira pour rechercher les fils
               if (evnmt_pere[0] != '#') QSqlQuery query2(QString (" DELETE FROM ") + m_EVNT_LIST_TBL_NAME + " WHERE " + m_EVNT_LIST_PERE + "='" + evnmt_pere + "'" , m_DataBase );
               while (Evnmt_EraseAllFils( pEvnmtList, evnmt_PrimKey));             // rechercher si ce fils n'est pas le pere d'autres
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
//-------------------------------------------- save_ListAtcd ---------------------------------------------------------
int CMoteurBase::save_ListAtcd(const QString &pkDoss,
                                const QString &guidDoss,
                                const QString &user,
                                const QString &sign_user,
                                const QString tableName,
                                QListView*   listView_ATCD)
{ //................. parcourir tous les items et les selectionner ............
  QString requete = QString (" DELETE FROM %2 WHERE %2_ref_human_pk='%1'").arg(pkDoss, tableName);
  QSqlQuery query(requete , m_DataBase );
  int n = 0;
  //......................... on renseigne la liste ...................................
  QString prepare  =      QString  (" INSERT INTO %1( "
                                     "%1_owner,"        // 0
                                     "%1_lang,"         // 1
                                     "%1_status,"       // 2
                                     "%1_human_guid,"   // 3
                                     "%1_ref_human_pk," // 4
                                     "%1_sign_user,"    // 5
                                     "%1_user,"         // 6
                                     "%1_datedeb,"      // 7
                                     "%1_datefin,"      // 8
                                     "%1_libelle,"      // 9
                                     "%1_code,"         // 10
                                     "%1_code_type,"    // 11
                                     "%1_level,"        // 12
                                     "%1_classe,"       // 13
                                     "%1_note, "        // 14
                                     "%1_ald "          // 15
                                     ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)").replace("%1",tableName);
   QSqlQuery queryInsert(QString::null , m_DataBase );
   QListViewItemIterator it( listView_ATCD );
   while ( it.current() )
       { queryInsert.prepare(prepare);
         CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
         QString code = pCPrt->text(6);
         if ( tableName=="allergies")
            { if (!code.startsWith("(")) {++it;continue;}
            }
         else
            { if ( code.startsWith("(")) {++it;continue;}
            }
         queryInsert.bindValue(0,  "MDT");                   // 0 owner
         queryInsert.bindValue(1,  "FR");                    // 1 lang
         queryInsert.bindValue(2,  pCPrt->text(2));          // 2 statut
         queryInsert.bindValue(3,  guidDoss);                // 3 guid
         queryInsert.bindValue(4,  pkDoss);                  // 4 pk
         queryInsert.bindValue(5,  sign_user);               // 5 sign_user
         queryInsert.bindValue(6,  user);                    // 6 user
         //................ les dates de debut ...........................
         QString date = CGenTools::NormaliseDateToIso(pCPrt->text(4)) ;
         QDate                     dateDeb = QDate::fromString(date,Qt::ISODate);
         if (! dateDeb.isValid ()) dateDeb = QDate::currentDate().addDays (-2);
         //................ les dates de fin ...........................
         //                 pCPrt->text(2) = soit 'date au format dd-MM-yyyy' 'Passé' 'Actif'
         QDate  dateEnd;
         if (pCPrt->text(2).upper()[0]=='P')                // passe mais date de fin non fixee
            {dateEnd = QDate::currentDate().addDays (-1);   // on met une date de fin valide anterieure a ce jour ==> atcd inactif
            }
         else if (pCPrt->text(2).upper()[0]=='A')           // actif mais date de fin non fixee
            {dateEnd = QDate();                             // on met une date de fin invalide donc non determinee  ==> atcd encore actif
            }
         else    // peut etre il y a une date valide
            { date      = CGenTools::NormaliseDateToIso(pCPrt->text(2));
              dateEnd   = QDate::fromString(date,Qt::ISODate);
              if (! dateDeb.isValid())  dateEnd = QDate(); // on met une date de fin invalide donc non determinee  ==> atcd encore actif
            }
         queryInsert.bindValue(7,  dateDeb);                 // 7 date deb
         queryInsert.bindValue(8,  dateEnd);                 // 8 date end
         queryInsert.bindValue(9,  pCPrt->text(0));          // 9 libelle
         //.................. code de l'atcd ou allergie ........................................
         if (code.startsWith("-("))
            { queryInsert.bindValue(10, code.remove('-').remove('(').remove(')') );     // 10 code
              queryInsert.bindValue(11, "CISP" );                                       // 11 code type
            }
         else if (code.startsWith("(-"))
            { queryInsert.bindValue(10, code.remove('-').remove('(').remove(')') );     // 10 code
              queryInsert.bindValue(11, "CIP" );                                        // 11 code type
            }
         else if (code.startsWith("~"))
            { queryInsert.bindValue(10, code.remove('~') );                             // 10 code
              queryInsert.bindValue(11, "CIM" );                                        // 11 code type
            }
         else if (code.startsWith("(."))
            { queryInsert.bindValue(10, code.remove("(.").remove(".)") );               // 10 code
              queryInsert.bindValue(11, "ATC" );                                        // 11 code type
            }
         else if (code.startsWith("("))
            { queryInsert.bindValue(10, code.remove('(').remove(')') );                 // 10 code
              queryInsert.bindValue(11, "VID_ALRG" );                                   // 11 code type
            }
         else
            { queryInsert.bindValue(10, "" );                                           // 10 code
              queryInsert.bindValue(11, "MD" );                                         // 11 code type
            }
         queryInsert.bindValue(12, "");                                                 // 12 level
         queryInsert.bindValue(13, pCPrt->text(1).replace("(","[").replace(")","]"));   // 13 classe Chirurgical(vasculaire)                    //
         queryInsert.bindValue(14, pCPrt->text(3));                                     // 14 commentaire
         queryInsert.bindValue(15, pCPrt->text(5));                                     // 15 A/ald  S/relatif au sport
         if ( !queryInsert.exec())
            { OutSQL_error(queryInsert, "CMoteurBase::save_ListAtcd()" , prepare /*=0*/);
              return 0;
            }
        ++n;
        ++it;
       }
    return n;
}

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
//qDebug(str_pk);
 return  str_pk;
}

//-------------------------------------------- initRubriquesList ---------------------------------------------------------
long  CMoteurBase::initRubriquesList(RUBREC_LIST *pRubList, QString numGUID, const QString &sqlFilter /*="" */)
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
 requete +=  m_DOSS_RUB_HEAD_TBL_NAME               + " WHERE ";
 if (sqlFilter.length()) requete += "( "+ sqlFilter + ") AND (";
 requete +=  m_DOSS_RUB_HEAD_GUID                   + "='"     + numGUID   + "'";
 if (sqlFilter.length()) requete +=       sqlFilter + "      ) AND  " + m_DOSS_RUB_HEAD_TYPE + " != '20110010' AND " + m_DOSS_RUB_HEAD_TYPE + " != '20110011' ";
 requete += " ORDER BY ";                               //RbDate_Date RbDate_TypeRub='20030000'
 requete +=  m_DOSS_RUB_HEAD_DATE;

 QSqlQuery query(requete , m_DataBase );
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
                                       Utf8_Query(query,9),               // Propriete N°0
                                       Utf8_Query(query,10),              // Propriete N°1
                                       Utf8_Query(query,11),              // Propriete N°2
                                       Utf8_Query(query,12),              // Propriete N°3
                                       Utf8_Query(query,13)               // Propriete N°4
                                      )
                            );
         ++nb;
        }
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 CloseBase();
 return nb;
}

//-------------------------------------------- GetRecorFromHeadPk ---------------------------------------------------------
long  CMoteurBase::GetRecorFromHeadPk(CRubRecord &record, const QString &headPk)
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
 requete +=      m_DOSS_RUB_HEAD_PRIMKEY          + "='"  + headPk   + "'";                               //RbDate_Date RbDate_TypeRub='20030000'

 QSqlQuery query(requete , m_DataBase );
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
           record.m_Prop_0      = Utf8_Query(query,9);               // Propriete N°0
           record.m_Prop_1      = Utf8_Query(query,10);              // Propriete N°1
           record.m_Prop_2      = Utf8_Query(query,11);              // Propriete N°2
           record.m_Prop_3      = Utf8_Query(query,12);              // Propriete N°3
           record.m_Prop_4      = Utf8_Query(query,13);              // Propriete N°4
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
 QString   requete ("SELECT " + m_DOSS_INDEX_PRIM_KEY + " FROM " + m_DOSS_INDEX_TBL_NAME + " WHERE " + m_DOSS_INDEX_GUID + "= '" + guid + "'");
 QSqlQuery sqlQuery(requete , m_DataBase );
 if ( sqlQuery.isActive() && sqlQuery.next() )
    {if (isBaseMusBeClosed==1) CloseBase();
     return sqlQuery.value(0).toString();;
    }
 OutSQL_error(sqlQuery, "CMoteurBase::GetDossPrimKeyFromGUID()");
 if (isBaseMusBeClosed==1) CloseBase();
 return ret;
}

//-------------------------------------- initConboBoxWithRubList -------------------------------------------------
/*! \brief Initialise le QComboBox passé en paramètre avec les documents de la liste selon leur type.
 * L'utilisation de la classe CPrtQListBoxItem permet et d'insérer un item dans le QComboBox et de lier ces items avec quelques données non affichées.

 * Types de documents :
 * 2001-0000 à 2001-9999   --> Courier
 * 2002-0000 à 2002-9999   --> Prescription avec: 2002-0000 à 2002-0000  --> OldPrescription
 *                                                2002-0200 à 2002-0299  --> NewPrescription
 *                                                2002-0300 à 2002-0399  --> NewCertificat
 * 2003-0000 à 2003-9999   --> Observations

 * \return Nombre de documents insérés dans le QComboBox.
 * \sa CPrtQListBoxItem
*/
long  CMoteurBase::initConboBoxWithRubList(RUBREC_LIST *pRubList, QComboBox* pComboBox, const QString &type, const QString &pkToDisplay /*=""*/, int *pIdComboItemToActivate /*=0*/)
{    RUBREC_LIST::iterator it;
     long  i    = 0;
     int pos    = 0;
     if (pIdComboItemToActivate) *pIdComboItemToActivate = -1;
     pComboBox->clear();
     int num_type = atoi(type);
     for(it = pRubList->begin(); it !=  pRubList->end(); ++it )   // it != m_RecSpeclist.end();
        {
          if ((*it).m_Type >= type && atoi((*it).m_Type) <= num_type+999)
             {QString qstr    = (*it).m_Date.left(10) + " " +(*it).m_Libelle;
              //  CPrtQListBoxItem( QListBox * listbox , 
              //                    const QString &text, 
              //                    const QString &pk, 
              //                    const QString &type, 
              //                    const QString &user, 
              //                    const QString &subType );
              new CPrtQListBoxItem(pComboBox->listBox() ,           // list box du combobox
                                   qstr ,                           // libelle
                                   QString::number(pos) ,           //  position du document dans la liste RUBREC_LIST
                                   (*it).m_Date ,                   //  date document
                                   (*it).m_PrimKey,                 //  pk du document
                                   (*it).m_SubType    );            //  sous type du document
              if ( pIdComboItemToActivate && 
                   pkToDisplay.length()   && 
                   (*it).m_PrimKey==pkToDisplay
                 )
                 {*pIdComboItemToActivate = i;
                 }
              ++i;
             }
         ++pos;
        } //end while (pSqlQuery->next())
     return i;
}

//-------------------------------------- GetAllRubriqueFromDate -------------------------------------------------
// 2001-0000 à 2001-9999   --> Courier
// 2002-0000 à 2002-9999   --> Prescription avec: 2002-0000 à 2002-0000  --> OldPrescription
//                                                2002-0200 à 2002-0299  --> NewPrescription
//                                                2002-0300 à 2002-0399  --> NewCertificat
// 2003-0000 à 2003-9999   --> Observations
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
// ACTION: place des données sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const QString & qString, RUBREC_LIST *pRubList, int id)
{  RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant à id
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
// ACTION: place des données sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const QString & qString, RUBREC_LIST::iterator it)
{  //RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant à id
   QCString cs              = qString.utf8 ();
   const char *pt = cs;
   if (CGestIni::IsUtf8(  pt , cs.length()))
      {(*it).SetData (cs.data(), cs.count());
      }
   else
      {(*it).SetData (qString, qString.length());
      }
}

//-------------------------------------- ReplaceDataInRubList ---------------------------------------------------------
// ACTION: place des données sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const char* text, long len_data, RUBREC_LIST *pRubList, int id)
{RUBREC_LIST::iterator it = pRubList->at(id);           // recuperer pointeur sur le CRubRecord correspondant à id

 QString debugStr;
 /*
 if (m_Debug)
    {CGestIni::Param_UpdateFromDisk("./ReplaceDataInRubList-txtbefore.txt", debugStr);
     debugStr += QString("----------------- ReplaceDataInRubList() AVANT ACTION : vidage RUBREC_LIST --- id %1 date %2-------------\n").arg(QString::number(id), QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
     debugStr += pRubList->serialize(0)   + "\n";
     CGestIni::Param_UpdateToDisk("./ReplaceDataInRubList-txtbefore.txt",debugStr);
    } */
 (*it).SetData (text, len_data);
/*
if (m_Debug)
    {debugStr += QString("----------------- ReplaceDataInRubList() APRES ACTION : vidage RUBREC_LIST --- id %1 date %2-------------\n").arg(QString::number(id), QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
     debugStr += pRubList->serialize(0)   + "\n";
     CGestIni::Param_UpdateToDisk("./ReplaceDataInRubList.log",debugStr);
     CGestIni::Param_UpdateToDisk("./ReplaceDataInRubList-txtAfter.txt",debugStr);
    } */
}

//-------------------------------------- ReplaceDataInRubList ---------------------------------------------------------
// ACTION: place des données sur un des elements de la liste des rubriques
//
void  CMoteurBase::ReplaceDataInRubList(const char* text, long len_data,  RUBREC_LIST::iterator it)
{(*it).SetData (text, len_data);
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

    QSqlQuery query (requete , m_DataBase );
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
 *  \param stringDST : QString où seront placées les donnees
 *  \note cette fonction place les donnees dans l'espace pCRubRecord->data() et donc si elle est utilisee
 *        pour recuperer les donnees d'un  RubRecord appartenant a la liste des rubriques : RUBREC_LIST de drtux
 *        ce RubRecord sera considere comme modifie donc à enregistrer.
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

    QSqlQuery sqlQuery (requete , m_DataBase );
    if (sqlQuery.isActive() && sqlQuery.next())
       { // QMemArray<type> & QMemArray::operator= ( const QMemArray<type> & a )
         // Assigns a shallow copy of a to this array and returns a reference to this array.
         // Equivalent to assign( a ).
         pCRubRecord->assign(sqlQuery.value(0).toByteArray());
         //data = sqlQuery.value(0).toByteArray()/*.copy()*/;
         ret = RUB_READ_ON_DISK;
       }
   }
 //....................... SI c'est une rubrique nouvellement crée ou en édition ..............................
 //                        on se sert directement des données stockées dans: CRubRecord
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
        {if (CGestIni::IsUtf8( ptr,len  )) stringDST = QString::fromUtf8 ( ptr );
         else                              stringDST = ptr;
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

 if (ret == RUB_READ_ON_DISK || ret == RUB_READ_IN_LIST)      // en fait cela veut dire que ça été lu sur le disque
    {char *ptr     = data.data();
     long  len     = data.size();
     if (ptr)
        {if ( len>=10 && CHtmlTools::IsRtf (ptr, 10) )
            {CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len-4, stringDST, 1);
            }
         else
            {if (CGestIni::IsUtf8( ptr, len )) stringDST = QString::fromUtf8 ( ptr );
             else                              stringDST = ptr;
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
 //....................... SI c'est une rubrique nouvellement crée ou en édition ..............................
 //                        on se sert directement des données stockées dans: CRubRecord
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

    QSqlQuery sqlQuery (requete , m_DataBase );
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
         {//.... si nouvelles données et mises sur effacer ..............
          //     alors on conptabilise pas
          if ((*it).m_PrimKey !=  "0" && (*it).m_PrimKey[0]!='#') ++nb_to_save;
         }
      else if ((*it).m_PrimKey == "0" || (*it).m_PrimKey[0]=='#')      // nouvelles données
         {if (! (*it).isEmpty())
             {++nb_to_save;
             }
         }
      else if (! (*it).isEmpty() )          // si les datas ont ete misee en cache c'est qu'elles on ete modifiées
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
long  CMoteurBase::RubListSave( RUBREC_LIST *pRubList, EVNT_LIST *pEvnmtList, QString numGUID, QMap<int,QString> &map_activeRubTypeAndPk, int mode /*= CMoteurBase::esclave*/)
{m_debugStr = "";
 long     i = 0;
 RUBREC_LIST::iterator           it;
 QMap<int,QString>::Iterator     mit;
 QMap<QString,QString>::Iterator it_pk;
 /*
 if (m_Debug)
    {m_debugStr += "----------------- AVANT ENREGISTREMENT : vidage RUBREC_LIST ----------------\n";
     m_debugStr += pRubList->serialize(0)   + "\n";
     m_debugStr += "----------------- AVANT ENREGISTREMENT : vidage EVNT_LIST ------------------\n";
     m_debugStr += pEvnmtList->serialize(0) + "\n";
    }
 */
 for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
     {//...... on recherche si ce record fait partie de ceux signales ................
      //       comme en cours
      int type  = (*it).m_Type.toInt();
      mit       = map_activeRubTypeAndPk.find(type);
      //....... un record est identifie comme actif si : ....................
      //        son type fait partie des types des rubriques actives
      //        le Pk (provisoire ou pas) associe a cette rubrique active
      //        est le meme que le record
      if ( ! (mit  != map_activeRubTypeAndPk.end() && mit.data()==(*it).m_PrimKey) )
         { mit = map_activeRubTypeAndPk.end();       // si ne satisfait pas aux conditions on ne retient pas ce record comme celui actif dans une rubrique
         }
      //.......... attribuer les eventuels vrais Pk ...................................
      //          si -1 alors le record est efface
      if ((*it).m_State & RUB_IS_TO_DELETE)
         {QString provPk = (*it).m_PrimKey;
          RubListDelete(it, mode);                                   // ==> effacer la rubrique
          pEvnmtList->Evnmt_SetToDelete_Pk(provPk);                  // marquer à effacer tous les enregistrements contenant ce document
          (*it).m_PrimKey = "-1";
         }
      else if ((*it).m_PrimKey == "0"||(*it).m_PrimKey[0]=='#')      // ==>  pk provisoire CAD il faut créer
         {if (! (*it).isEmpty() )
             {QString provPk = (*it).m_PrimKey;
              QString realPk = RubListCreate(it, numGUID, mode);     // creer la rubrique
              pEvnmtList->Evnmt_SetDefinitivePkDoc( provPk, realPk);     // remplacer le pk provisoire par le vrai dans la liste des liens
              (*it).m_PrimKey = realPk;
             }
         }
      else if (! (*it).isEmpty() )
         {RubListUpdate(it, mode);                                         // ==>  updater la rubrique
         }
      //........... si le map est celui d'un record actif ..................
      //            mettre a jour le vrai Pk
      if ( mit  != map_activeRubTypeAndPk.end() )
         { map_activeRubTypeAndPk[type] = (*it).m_PrimKey;
         }
      ++i;
     } //end while (pSqlQuery->next())
 Evnmt_UpdateToDisk(pEvnmtList);
 /*
 if (m_Debug)
    {m_debugStr += "----------------- APRES ENREGISTREMENT : vidage RUBREC_LIST ----------------\n";
     m_debugStr += pRubList->serialize(0)   + "\n";
     m_debugStr += "----------------- APRES ENREGISTREMENT : vidage EVNT_LIST ------------------\n";
     m_debugStr += pEvnmtList->serialize(0) + "\n";
    }
 */
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
   QSqlQuery sqlQuery_1(requete_1 , m_DataBase );

   QString   requete_2;
   QSqlQuery sqlQuery_2( m_DataBase );

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
QString  CMoteurBase::RubListCreate(RUBREC_LIST::iterator it, QString numGUID, int /*mode = CMoteurBase::esclave*/)
{
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return QString::null;
  //............... recuperer le pk de l'index dossier .......................................................
  QString dossIndexPk = GetDossPrimKeyFromGUID(numGUID, 0);
  //if (VerrouilleTable(m_DOSS_RUB_DATA_TBL_NAME, m_DataBase)==0)  return 0;
  //if (VerrouilleTable(m_DOSS_RUB_HEAD_TBL_NAME, m_DataBase)==0)  return 0;


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
      QSqlCursor cur(m_DOSS_RUB_HEAD_TBL_NAME, TRUE, m_DataBase);
      if ( !cur.canInsert ()) {deleteRubBlobFromPk(blobs_Pk);
                               CloseBase();
                               return ret;
                              }
      (*it).m_Ref_Blobs_PK = blobs_Pk;
      //............... si OK on recupere le buffer et on le charge avec les données .........
      QSqlRecord *buffer = cur.primeInsert();                               // recuperer le buffer d'insertion
      buffer->setValue( m_DOSS_RUB_HEAD_GUID,           numGUID               );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_REF_DOSS_PK,    dossIndexPk           );  // y placer les données

      buffer->setValue( m_DOSS_RUB_HEAD_USER,           (*it).m_User          );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_SIGN_USER,      (*it).m_SignUser      );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_NOM_DATE,       (*it).m_Libelle       );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_DATE,           (*it).m_Date          );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_TYPE,           (*it).m_Type          );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_SUBTYPE,        (*it).m_SubType       );  // y placer les données
      buffer->setValue( m_DOSS_RUB_HEAD_REF_BLOBS_PK,   (*it).m_Ref_Blobs_PK  );

      buffer->setValue( m_DOSS_RUB_HEAD_FIN,            (*it).m_Fin           );      // date de fin l'enregistrement
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_0,         (*it).m_Prop_0        );      // Propriete N°0
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_1,         (*it).m_Prop_1        );      // Propriete N°1
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_2,         (*it).m_Prop_2        );      // Propriete N°2
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_3,         (*it).m_Prop_3        );      // Propriete N°3
      buffer->setValue( m_DOSS_RUB_HEAD_PROP_4,         (*it).m_Prop_4        );      // Propriete N°4
      buffer->setValue( m_DOSS_RUB_HEAD_DUREE_MOD,      (*it).m_DureeMod      );      // duree d'acces en modification du document
      if (!cur.insert())  {deleteRubBlobFromPk(blobs_Pk);
                           CloseBase();
                           return ret;
                          }
      //................. recuperer dernier primKey insere (methode SELECT blobs_Pk )...........................................
      //................. reselectionner l'enregistrement pour retrouver son PK avec le pk du blob ........................
      QString   requete ("SELECT " + m_DOSS_RUB_HEAD_PRIMKEY + " FROM " + m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_REF_BLOBS_PK + "= '" + blobs_Pk + "'");
      QSqlQuery sqlQuery(requete , m_DataBase );
      //................. replacer une fois le Pk retrouvee, le bon GUID.......................................................
      if ( sqlQuery.isActive() && sqlQuery.next() )
         {ret = sqlQuery.value(0).toString();
          if (!ret.length()) deleteRubBlobFromPk(blobs_Pk);
         }
      //................. recuperer dernier primKey insere (methode LAST_INSERT_ID() ) ........................................
      /*
      QString requete ("SELECT LAST_INSERT_ID() FROM ");
      requete += m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_PRIMKEY + " = LAST_INSERT_ID()";
      QSqlQuery sqlQuery(requete , m_DataBase );
      if ( sqlQuery.next() )  ret =  sqlQuery.value(0).toString();
      */
     }
  //......................... fermer la base ..............................................
  CloseBase();
  return ret;
}

//-------------------------------------- RubListBlobCreate ----------------------------------------------------------------------
QString  CMoteurBase::RubListBlobCreate(RUBREC_LIST::iterator it, QString numGUID)
{QString tmpGUID = GUID_Create();
 QSqlCursor cur(m_DOSS_RUB_DATA_TBL_NAME, TRUE, m_DataBase);
 if ( !cur.canInsert() )                                           return QString::null;
 //............... se creer un GUID temporaire permettant par la suite de retrouver le Pk de l'enregistrement créé .................
 //............... si OK on recupere le buffer et on le charge avec les données ..........................................
 QSqlRecord *buffer = cur.primeInsert();                        // recuperer le buffer d'insertion
 buffer->setValue( m_DOSS_RUB_DATA_BLOB, (*it) );               // y placer les données
 buffer->setValue( m_DOSS_RUB_DATA_GUID, tmpGUID );             // y placer le GUID temporaire (permettra de retrouver l'enregistrement créé)
 if (!cur.insert())                                                return QString::null;                   // ecrire les données

 //................. recuperer dernier primKey insere (methode SELECT tmpGUID )....................................................................
 //................. reselectionner l'enregistrement pour retrouver son PK avec le GUID temporaire........................
 QString   requete ("SELECT " + m_DOSS_RUB_DATA_PRIMKEY + " FROM " + m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_GUID + "= '" + tmpGUID + "'");
 QSqlQuery sqlQuery(requete , m_DataBase );
 QString   prim_key = QString::null;
 //................. replacer une fois le Pk retrouvee, le bon GUID........................
 if ( sqlQuery.isActive() && sqlQuery.next() )
    {prim_key = sqlQuery.value(0).toString();
     requete  = QString (" UPDATE ") + m_DOSS_RUB_DATA_TBL_NAME + " SET " + m_DOSS_RUB_DATA_GUID + " = '" + numGUID  + "' "
                         " WHERE "   + m_DOSS_RUB_DATA_PRIMKEY  + " =  '" + prim_key  + "'";
     sqlQuery.exec(requete);
    }
 return prim_key;
 /*
 //................. recuperer dernier primKey insere (methode LAST_INSERT_ID() )....................................................................
 //QString requete ("SELECT LAST_INSERT_ID() FROM ");
 //requete += m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_PRIMKEY + " = LAST_INSERT_ID()";
 QSqlQuery sqlQuery(requete , m_DataBase );
 QString prim_key = QString::null;
 if ( sqlQuery.next() )
    {prim_key =  sqlQuery.value(0).toString();
    }
 return prim_key;
 */
}

//-------------------------------------- RubListDelete ----------------------------------------------------------------------
int  CMoteurBase::RubListDelete(RUBREC_LIST::iterator it, int /* mode = CMoteurBase::esclave*/)
{int ok  = FALSE;
 //..................... si mode nomade .......................
 //                      on n'efface par l'enregistrement mais on le rend invisible
 //                      en lui collant un utilisateur non possible
 /* on a trouve le fameux bu de zab
 if (mode==CMoteurBase::nomade)
    {
     if (OpenBase()==0)  return ok;
     QSqlQuery queryD(m_DataBase);
     QString prepare   = "";
     prepare          += "UPDATE " + m_DOSS_RUB_HEAD_TBL_NAME    + " SET " +
                                     m_DOSS_RUB_HEAD_USER        + " = ?," +
                                     m_DOSS_RUB_HEAD_SIGN_USER   + " = ? " +
                         " WHERE " + m_DOSS_RUB_HEAD_PRIMKEY     + " ='"   + (*it).m_PrimKey + "'";
     queryD.prepare(prepare);
     queryD.bindValue(0, "--");
     queryD.bindValue(1, "--");
     if (queryD.exec()) {ok = TRUE;}
     else               {QString mess = TR("Erreur : RubListDelete::CMoteurBase::nomade \r\n") + prepare + "\r\n";
                         OutSQL_error(queryD, prepare);      // rajouter le message sql
                         ok = FALSE;
                        }
     //synchroWriteMofidication(m_DOSS_RUB_HEAD_TBL_NAME, (*it).m_PrimKey,      m_DOSS_RUB_HEAD_PRIMKEY, "D");
     //synchroWriteMofidication(m_DOSS_RUB_DATA_TBL_NAME, (*it).m_Ref_Blobs_PK, m_DOSS_RUB_DATA_PRIMKEY, "D");
     //......................... fermer la base ..............................................
     CloseBase();
     return ok;
    }

  */
 //.............................. ouvrir la base ..........................................................
 if (OpenBase()==0)  return 0;
 //if (VerrouilleTable(m_DOSS_RUB_DATA_TBL_NAME, m_DataBase)==0)  return 0;
 //if (VerrouilleTable(m_DOSS_RUB_HEAD_TBL_NAME, m_DataBase)==0)  return 0;
 //............................. effacer le header .........................................................
  QString requete("DELETE FROM ");
          requete +=  m_DOSS_RUB_HEAD_TBL_NAME + " WHERE " + m_DOSS_RUB_HEAD_PRIMKEY + "='" +(*it).m_PrimKey + "'";

 QSqlQuery sqlQuery(QString::null , m_DataBase );
 ok = sqlQuery.exec(requete);
 if (ok == FALSE) {/*DeVerrouilleTable(m_DataBase);*/ CloseBase(); return FALSE;}        // si erreur cassos cas social va

 //............................. effacer le blob ...........................................................
 ok = deleteRubBlobFromPk((*it).m_Ref_Blobs_PK);
 /*
 requete  = "DELETE FROM ";
 requete +=  m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_PRIMKEY + "='" +(*it).m_Ref_Blobs_PK + "'";
 ok = sqlQuery.exec(requete);
 */
 //............................. fermer la base .............................................................
 //DeVerrouilleTable(m_DataBase);
 CloseBase();
 return ok;
}
//-------------------------------------- deleteRubBlobFromPk ----------------------------------------------------------------------
int  CMoteurBase::deleteRubBlobFromPk(const QString &pk)
{QSqlQuery sqlQuery(QString::null , m_DataBase );
 return  sqlQuery.exec(QString ("DELETE FROM ") +  m_DOSS_RUB_DATA_TBL_NAME + " WHERE " + m_DOSS_RUB_DATA_PRIMKEY + "='" + pk + "'");
}
//-------------------------------------- RubListUpdate ----------------------------------------------------------------------
/*
               m_PrimKey        = primKey;
               m_Libelle        = libelle;
               m_Type           = type;
               m_Date           = date;
               m_User           = user;
               m_pCharData      = 0;        // n'est initialisé avec des données d'une rubrique que lors modification
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
   QString      m_DOSS_RUB_HEAD_USER;            // utilisateur associé à rubrique

*/
int  CMoteurBase::RubListUpdate(RUBREC_LIST::iterator it, int /*mode = CMoteurBase::esclave*/)
{ /*............................... methode requete ............................................
  //                       pose un PB avec les valeurs litérales comportant une apostrophe
  //                       car sont évaluées comme delimiteurs de valeurs
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

  QSqlQuery* pSqlQuery = new QSqlQuery(requete , m_DataBase );
  if (pSqlQuery && pSqlQuery ->isActive())  ok = TRUE;
  if (ok) ok = RubListBlobUpdate(it);                    // au tour des données binaires
  if (pSqlQuery) delete  pSqlQuery;
  CloseBase();

  */

  if ( (*it).m_Type  == 20110010  || (*it).m_Type== 20110011) 
     { 
       QString debugStr = "";
       /*if (m_Debug)
          {CGestIni::Param_UpdateFromDisk("./ReplaceDataInRubList-txtbefore.txt", debugStr);
           debugStr += QString("----------------- RubListUpdate 2011()  %1-------------\n").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
           debugStr += (*it).serialize(1)+ "\n";
           CGestIni::Param_UpdateToDisk("./ReplaceDataInRubList-txtbefore.txt",debugStr);
          }*/
        return FALSE;
     }
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if (OpenBase()==0)  return 0;
  //if (VerrouilleTable(m_DOSS_RUB_DATA_TBL_NAME, m_DataBase)==0)  return 0;
  //if (VerrouilleTable(m_DOSS_RUB_HEAD_TBL_NAME, m_DataBase)==0)  return 0;

  //............... crer un curseur SQL ..................................................
  QSqlCursor cur(m_DOSS_RUB_HEAD_TBL_NAME, TRUE, m_DataBase);
  if ( !cur.canUpdate ()) {/*DeVerrouilleTable(m_DataBase); */CloseBase(); return FALSE;}
  //............... placer ce curseur sur le bon enregistrement ..........................
  cur.select(  m_DOSS_RUB_HEAD_PRIMKEY + "=" + (*it).m_PrimKey);
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer ( pas de delete à la fin !!)
     buffer->setValue( m_DOSS_RUB_HEAD_USER,       (*it).m_User     ); // y placer les données
     buffer->setValue( m_DOSS_RUB_HEAD_SIGN_USER,  (*it).m_SignUser ); // y placer les données
     buffer->setValue( m_DOSS_RUB_HEAD_NOM_DATE,   (*it).m_Libelle  ); // y placer les données
     buffer->setValue( m_DOSS_RUB_HEAD_DATE,       (*it).m_Date     ); // y placer les données
     buffer->setValue( m_DOSS_RUB_HEAD_TYPE,       (*it).m_Type     ); // y placer les données
     buffer->setValue( m_DOSS_RUB_HEAD_SUBTYPE,    (*it).m_SubType  ); // y placer les données

     buffer->setValue( m_DOSS_RUB_HEAD_FIN,            (*it).m_Fin           );      // date de fin l'enregistrement
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_0,         (*it).m_Prop_0        );      // Propriete N°0
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_1,         (*it).m_Prop_1        );      // Propriete N°1
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_2,         (*it).m_Prop_2        );      // Propriete N°2
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_3,         (*it).m_Prop_3        );      // Propriete N°3
     buffer->setValue( m_DOSS_RUB_HEAD_PROP_4,         (*it).m_Prop_4        );      // Propriete N°4
     buffer->setValue( m_DOSS_RUB_HEAD_DUREE_MOD,      (*it).m_DureeMod      );      //duree d'acces en modification du document
     cur.update();                                                  // ecrire le buffer avec les données du header
     ok = RubListBlobUpdate(it);                                    // s'occuper des données binaires BLOB
    }
  //......................... fermer la base ..............................................
  //DeVerrouilleTable(m_DataBase);
  CloseBase();
  return ok;
}

//-------------------------------------- RubListBlobUpdate ----------------------------------------------------------------------
/*
   QString      m_DOSS_RUB_DATA_TBL_NAME;        // nom de la table des données des RUBRIQUES
   QString      m_DOSS_RUB_DATA_PRIMKEY;         // clef primaire de cet enregistrement  (le meme que le header)
   QString      m_DOSS_RUB_DATA_BLOB;            // données binaires
   int          m_DOSS_RUB_DATA_FP_PRIMKEY;      // position du champ clef primaire de cet enregistrement
   int          m_DOSS_RUB_DATA_FP_BLOB;         // position du champ données binaires  de l'enregistrement
*/

int  CMoteurBase::RubListBlobUpdate(RUBREC_LIST::iterator it)
{//............... crer un curseur SQL ..................................................
 QSqlCursor cur(m_DOSS_RUB_DATA_TBL_NAME, TRUE, m_DataBase);
 if ( !cur.canUpdate ()) return FALSE;
 cur.select(  m_DOSS_RUB_DATA_PRIMKEY + "=" + (*it).m_Ref_Blobs_PK);
 //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                        // recuperer le buffer
     //char *pt = (char*)(const char*)(*it);
     buffer->setValue( m_DOSS_RUB_DATA_BLOB, (*it) );               // y placer les données
     cur.update();                                                  // ecrire les données
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
{QSqlQuery query(QString("LOCK TABLES ") + tableName + " WRITE", dataBase);
 if (query.isActive()) return 1;
 return 0;
}

//------------------------------------------------ DeVerrouilleTable  --------------------------------------------------------------
int CMoteurBase::DeVerrouilleTable(QSqlDatabase* dataBase)
{QSqlQuery query("UNLOCK TABLES  ", dataBase);
 if (query.isActive()) return 1;
 return 0;
}

//------------------------------------------------ IsDossierVerrouille  --------------------------------------------------------------
int CMoteurBase::IsDossierVerrouille   ( const QString &dossGUID, QString &usedBy )
{ if (OpenBase()==0)  return -1;  // si point = erreur d'ouverture de la base
  //QString dateTimeR;
  if (dossGUID.length()==0) return -1; // si GUID vide (cas d'un nouveau dossier) retourner modif valide
  //.................. verifier si pas déjà utilisé ......................................................................................
  //                   et si c'est le cas retourner le proprietaire
  QString requete("");
  requete += QString("SELECT ") + m_VERROU_USER_NAME + " FROM " + m_VERROU_TBL_NAME + " WHERE " + m_VERROU_DOSS_GUID  + " = '" +   dossGUID + "'";
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive() && query.next())
    {usedBy = Utf8_Query(query,0);    // retourne l'utilisateur déjà propriétaire
     return 1;                         // et cassos car c'est deja verrouillé par un autre
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
 //.................. verifier si pas déjà utilisé ......................................................................................
 //                   et si c'est le cas retourner le proprietaire
 QString requete("");
 requete += QString("SELECT ") + m_VERROU_USER_NAME + " FROM " + m_VERROU_TBL_NAME + " WHERE " + m_VERROU_DOSS_GUID  + " = '" +   dossGUID + "'";
 QSqlQuery query(requete , m_DataBase );
 if (query.isActive() && query.next())
    {ret = query.value(0).toString();    // retourne l'utilisateur déjà propriétaire
     return ret;                         // et cassos car c'est deja verrouillé par un autre
    }

 //.................. si pas dèjà verrouillé inserer notre verrou .......................................................................
 //.................. verouiller la table ................................................
 query.exec(QString("LOCK TABLES ") + m_VERROU_TBL_NAME + " WRITE");
 //.................. crer un curseur SQL ................................................
 QSqlCursor cur(m_VERROU_TBL_NAME, TRUE, m_DataBase);
 if ( !cur.canInsert ())                                    return QString(".");
 QDateTime dt ( QDate::currentDate (), QTime::currentTime() );
 if (dateTime.length()==0) dateTimeR = dt.toString("yyyyMMddhhmmsszzz");
 else                      dateTimeR = dateTime;
 //............... si OK on recupere le buffer et on le charge avec les données ..........
 QSqlRecord *buffer = cur.primeInsert();
 buffer->setValue( m_VERROU_DOSS_PK,     dossPk.toInt()                     );  // y placer les données
 buffer->setValue( m_VERROU_DOSS_GUID,   dossGUID                           );  // y placer les données
 buffer->setValue( m_VERROU_RUB_NAME,    rubName                            );  // y placer les données
 buffer->setValue( m_VERROU_RUB_PK,      rubPk.toInt()                      );  // y placer les données
 buffer->setValue( m_VERROU_USER_NAME,   userName                           );  // y placer les données
 buffer->setValue( m_VERROU_DATE_TIME,   dateTimeR                          );  // y placer les données
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
 QSqlQuery query(QString("LOCK TABLES ") + m_VERROU_TBL_NAME + " WRITE", m_DataBase);
 //............................. effacer le header .........................................................
 query.exec(QString("DELETE FROM ")      + m_VERROU_TBL_NAME + " WHERE " + m_VERROU_DOSS_GUID + "='" + dossGUID + "'");
 //................. deverouiller la table ................................................................................................
 query.exec("UNLOCK TABLES ");
}

//------------------------------------------------ DeVerrouilleTout --------------------------------------------------------------
void    CMoteurBase::DeVerrouilleTout(  )
{if (OpenBase()==0)  return ;  // si point = erreur d'ouverture de la base
 //................. verouiller la table ..................................................................................................
 QSqlQuery query(QString("LOCK TABLES ") + m_VERROU_TBL_NAME + " WRITE", m_DataBase);
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
int CMoteurBase::PassWordEncode(const QString &passIn, QString &passEncoded)
{QString pass = passIn;
 passEncoded  = "";
 char str[34];
 INT16    val;
 int len         = pass.length();

 if (len>32)       return ERR_PASS_MAX;
 if (len==0)       return ERR_PASS_ZERO;
 strcpy(str , pass);
 pass = "";
 for (int i=0; i<len; ++i)
     { val    =  str[i];
       val    =  (val << ((i+1)&0x000F) )^val;
       pass  += UINTtoHex(val);
     }
 passEncoded = pass;
 return ERR_PASS_OK;
}

//-----------------------------------------------------  PassWordEncode --------------------------------------------------
// Algo minimal pour l'instant
// chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
// resultat final en hexa;
int CMoteurBase::PassWordEncode(QString &pass)
{char str[34];
 INT16    val;
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
 UINT16   val;
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
      val = HexToUINT(buf);                 // les convertir en UINT16
      val = val ^ str_pass[i];              // xor avec le mot de passe
      val = val >> ((i+1)&0x000F);          // rotation inverse maxi de 8 et on devrait avoir le code ASCII
      if (val != str_pass[i])      return ERR_PASS_MAX;    // erreur si pas egal
     }
 return ERR_PASS_OK;
}

//-----------------------------------------------------  UINTtoHex --------------------------------------------------------
QString CMoteurBase::UINTtoHex( UINT16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   // là on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
UINT16 CMoteurBase::HexToUINT( const char *str)
{int i   = strlen(str);
 if (i==0) return 0;
 UINT16 val = 0;
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
QString CMoteurBase::GUID_Create(/*const char *format   ="%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" */)
{
    QString guid ( QUuid::createUuid ().toString());
    return  guid.mid(1, 36);

/*
#ifdef Q_WS_X11
   uuid_t out;
   uuid_generate(out);
   return GuidToString((GUID*) out, format);
#else
   GUID out;
   ::CoCreateGuid(&out);
   return GuidToString(&out, format);
#endif
*/
}
/*
//-----------------------------------------------------  GuidToString -------------------------------------------------
QString CMoteurBase::GuidToString(GUID *guid  , const char *format / * ="%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" * /)
{QString rString;
    rString.sprintf(format,
                        guid->Data1, guid->Data2, guid->Data3,
                        guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
                        guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);
    return      rString;
}
*/

//-----------------------------------------------------  CalculeDiffAge_Num -------------------------------------------------
// ACTION: calcule l'age entre deux dates de début et de fin.
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
         //........Le jour de l'année .................
         dayYearDebut = pDebut->dayOfYear ();
         dayYearFin   = pFin->dayOfYear ();
         //................................ bisextiles and cie ..................................................
         //ATTENTION A L'ANNEE BISEXTILLE (sauf en l'an 1900, car les débuts de siecles ne sont pas bisextille sauf les débuts de millénaire
         //60eme jour => 29 Février
         if (pDebut->year()%4==0 && pDebut->year()%100 && pDebut->year()%1000    &&  dayYearDebut>60 )
             dayYearDebut--;
         if (pFin->year()%4==0   && pFin->year()%100   && pFin->year()%1000      &&  dayYearFin>60)
             dayYearFin--;

          //....................Position des jours l'un par rapport à l'autre ....................................
          if      (dayYearFin<dayYearDebut)     lEtatDayYear = -1;
          else if (dayYearFin>dayYearDebut)     lEtatDayYear =  1;
          else                                  lEtatDayYear =  0;

          //...................Calcul année (-1 si le jour de l'année en cours est <) .............................
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
// ACTION: calcule l'age entre deux dates de début et de fin.
// ENTREE: QDate *pDebut: pointe sur une QDate de debut (exemple date de naissance)
//         QDate *pFin:   pointe sur une QDate de  fin (peut etre zero alors il sera pris la date en cours)
//         bool bTexte:   TRUE   alors la date sera exprimée comme: 4 ans 6 mois 3 jours
//                        FALSE  alors la date sera exprimée comme un chiffre
//         int bAbrege:   2  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        1  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        0  et alors les sorties seront detaillées avec si age<3 avec les mois et jours:
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
         buffer = QString::number(an) + TR(" an") + ((an>1) ? "s " : " ");
     else
         buffer.sprintf("%d ", an);
     sRes = buffer;
    } //if (an)
          //.................... si bAbrege==2 alors on ne sort que les années .......................................
 if (bAbrege==2)
    {if (an==0) sRes="0";
     return sRes;
    }
 if (!bAbrege || an < 3 /* || an<3*/) // age Inferieur à 3 ans et version detaillée
    {
     if (mois || !bTexte)
        {
         if (bTexte)
         //buffer.sprintf("%d mois ", mois);  // en fançais " mois " à déjà un s en [4] donc ne pas rajouter si plusieurs
            {QString ms = TR(" mois");  // va bien y avoir un dialecte où ça ne va pas marcher
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
            buffer = QString::number(jour) + TR(" jour") + ((jour>1) ? "s " : " ");
         else
            buffer.sprintf("%d ", jour);
         sRes += buffer;
        } // if  (jour || !bTexte)
     }// if (!bAbrege || an<3) //Inferieure à 3 ans
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

 /*............ aller à la premiere acolade ..............*/
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
             else if (strncmp(pt+1,"tab",3)==0    // et le tag 'tab' est rencontré
                     )
                {stringDST += "\t";
                 pt    += 4;
                 if (*pt==' ') pt++;
                }
             else if (pt[4] !='d'&&               // faut pas que ce soit 'pard'
                      strncmp(pt+1,"par",3)==0    // mais 'par' isolé
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

