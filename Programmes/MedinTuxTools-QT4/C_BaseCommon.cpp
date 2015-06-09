#include "C_BaseCommon.h"
#include <QByteArray>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QUuid>
#include <QSqlError>
#include <QString>
#include <QTimer>
#include <QObject>
#include <QFile>
#include <QProgressBar>
#include <QTextStream>
#include <QApplication>
#include <QMap>
#include <QMapIterator>
#include <QLabel>
#include <QVariantList>
#include <QDebug>
#include <QVariant>

#include "../MedinTuxTools-QT4/CGestIni.h"
#include "../MedinTuxTools-QT4/C_Utils_Log.h"
#include "uuid/C_MyUuid.h"


#define   DUMMY_TEST_SQL_COMMAND   "SELECT 1"

//---------------------------------------- C_BaseCommon -------------------------------------------------------
C_BaseCommon::C_BaseCommon(QObject *parent)
    : QObject(parent), C_Log()
{m_BaseLabel       = "";
 m_LastError       = "";
 m_StatusWidget    = 0;
 m_pQProgressBar   = 0;
 m_pReconnectTimer = 0;
 m_GUID_CreateMode = C_BaseCommon::byMyself;
 m_TimeToReconnect = DB_TIME_TO_RECONNECT;
 m_ownerList.clear();
}

//---------------------------------------- C_BaseCommon -------------------------------------------------------
C_BaseCommon::~C_BaseCommon()
{if (m_pReconnectTimer)
    {m_pReconnectTimer->stop();
     disconnect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(Slot_ReconnectTimerDone()) );
    }
 // QSqlDatabase::database( m_BaseLabel ).close();
 // QSqlDatabase::removeDatabase (m_BaseLabel);
}

//---------------------------------------- EncodePassword_InConnectionParam [static]-------------------------------------------------------
bool C_BaseCommon::EncodePassword_InConnectionParam(const QString &param, const QString &section, const QString &key /* = "Parametres"*/ , QString* errMess /* =0*/)
{

 QString driver;             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
 QString dataBaseToConnect;  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
 QString user;               // = "root"
 QString password;           // = ""
 QString hostname;           // = "localhost"
 QString port;               // = "3306"
 if ( CGestIni::Param_ReadParam(  param.toLatin1(), section.toLatin1(), key.toLatin1(), &driver, &dataBaseToConnect, &user, &password, &hostname, &port ) !=0 )  // zero = pas d'erreur
    { if (errMess) *errMess  = QObject::tr("C_BaseCommon::EncodePassword_InConnectionParam() : error in parameters : CGestIni::Param_ReadParam() may be : key 'Parametres' omitted in section 'Connexion'");
      return 0;
    }
 if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
 password = CGestIni::PassWordEncode(password);
 password.prepend('#');
 QString tmp = param;
 CGestIni::Param_WriteParam(  &tmp, section.toLatin1(), key.toLatin1(), driver.toLatin1(), dataBaseToConnect.toLatin1(), user.toLatin1(), password.toLatin1(), hostname.toLatin1(), port.toLatin1());
 return 1;
}

//---------------------------------------- BaseConnect -------------------------------------------------------
bool C_BaseCommon::BaseConnect(const QString &param, const QString &baseLabel, const QString &dataBase /* ="" */, QString* errMess /* =0*/, const QString &section  /* = "Connexion" */, const QString &key  /* = "Parametres" */)
{
 QString driver;             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
 QString dataBaseToConnect;  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
 QString user;               // = "root"
 QString password;           // = ""
 QString hostname;           // = "localhost"
 QString port;               // = "3306"

 m_LastError       = "";
 m_BaseLabel       = "";
 if (CGestIni::Param_ReadParam(  param.toLatin1(), section.toLatin1(), key.toLatin1(), &driver, &dataBaseToConnect, &user, &password, &hostname, &port) !=0 )  // zero = pas d'erreur
    { m_LastError = tr("ERROR : C_BaseCommon::BaseConnect() : error in parameters : CGestIni::Param_ReadParam() may be : key 'Parametres' omitted in section 'Connexion'");
      if (errMess) *errMess = m_LastError;
      outMessage( m_LastError);
      return 0;
    }
 if (dataBase.length()) dataBaseToConnect = dataBase; // si le nom de la base de donnees a laquelle se connecter est precisee, alors on le prend au lieu de celui des parametres ini
 int timeToReconnect =  DB_TIME_TO_RECONNECT;
 QString value       = CGestIni::Param_ReadUniqueParam(  param.toLatin1(), section.toLatin1(), "TimeToReconnect");
 if (value.length())   timeToReconnect = value.toInt();

 return BaseConnect(baseLabel,
                    driver,
                    dataBaseToConnect,
                    user ,
                    password ,
                    hostname ,
                    port.toInt(),
                    timeToReconnect,
                    errMess);
}

 //---------------------------------------- BaseConnect -------------------------------------------------------
 bool C_BaseCommon::BaseConnect(const QString &baseLabel,    // 0
                                const QString &driver,       // 1
                                const QString &dataBase,     // 2
                                const QString &user ,        // 3
                                const QString &_password ,   // 4
                                const QString &hostname ,    // 5
                                int port,                    // 6
                                int timeToReconnect /* = DB_TIME_TO_RECONNECT */,
                                QString* errMess    /* = 0*/)
 {
  QString password = _password;
  if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
  QSqlDatabase::removeDatabase (baseLabel);
  QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, baseLabel);
  defaultDB.setDatabaseName( dataBase );
  defaultDB.setUserName    ( user     );
  defaultDB.setPassword    ( password );
  defaultDB.setHostName    ( hostname );
  defaultDB.setPort        ( port     );

  if ( ! defaultDB.open() )
     { m_LastError = tr("ERROR : C_BaseCommon::BaseConnect() : Failed to open database : ")    +
                         baseLabel   + " Driver : " +  QString(driver) + "  " +
                         QString(dataBase) + "\n" +
                         defaultDB.lastError().driverText() + "\n" + defaultDB.lastError().databaseText();
       if (errMess) *errMess = m_LastError;
       outMessage(m_LastError);
       return false;
    }
  if (m_pReconnectTimer) delete m_pReconnectTimer;
  m_BaseLabel       = baseLabel;
  m_TimeToReconnect = timeToReconnect;
  m_pReconnectTimer = new QTimer (this);
  if (m_pReconnectTimer)
     { m_pReconnectTimer->setInterval(m_TimeToReconnect);
       m_pReconnectTimer->start();
       connect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(Slot_ReconnectTimerDone()) );
     }
  return true;
}
 //-----------------------------------------------------  databasesList -------------------------------------------
  /*! \brief  return databases list  of all databases
   *  \return databases list
   */
 QStringList C_BaseCommon::databasesList()
 {   QSqlQuery query( database() );
     query.exec("SHOW DATABASES");
     QStringList basesList;
     if (query.isActive())
        {while (query.next())  basesList.append (query.value(0).toString());
       }
     return basesList;
}
 //------------------------ changeDataBaseConnexion -----------------------------------------
 /*! \brief change database connexion.
  *  \param  const QString &driver driver name QDB2 QIBASE QMYSQL QOCI QODBC QPSQL QSQLITE QSQLITE2 QTDS
  *  \param  const QString &schema schema database name to coonect
  *  \param  const QString &user connexion user name
  *  \param  const QString &_password password : encoded (begin by '#')  or not encoded (don't begin by '#')
  *  \param  const QString &hostname IP adress or domain name
  *  \param  const QString &port port on wich connexion must be opened
  *  \return true in all is ok otherwise false
  */
 int C_BaseCommon::changeDataBaseConnexion(const QString &driver, const QString &schema, const QString &user, const QString &password, const QString &hostname, const QString &port)
 {   database().close();
     return BaseConnect(m_BaseLabel,
                        driver,
                        schema,
                        user ,
                        password ,
                        hostname ,
                        port.toInt(),
                        m_TimeToReconnect);
 }
 //---------------------------------  isThisSchemaExists [static]-------------------------------------------
  /*! \brief  test if a data base schema exists by trying a connexion on it. If no error, empty message will be returned
   *  \param  const QString &schema schema database name to test
   *  \return empty string if no error / error string if error
   */
 QString  C_BaseCommon::isThisSchemaExists(const QString &schema )
 { return C_BaseCommon::isThisSchemaExists(database().driverName(),schema,database().userName(),database().password(),database().hostName(), QString::number(database().port()) );
 }

 //---------------------------------  isThisSchemaExists [static]-------------------------------------------
  /*! \brief  test if a data base schema exists by trying a connexion to it. If no error, empty message will be returned
   *  \param  const QString &driver driver name QDB2 QIBASE QMYSQL QOCI QODBC QPSQL QSQLITE QSQLITE2 QTDS
   *  \param  const QString &schema schema database name to test
   *  \param  const QString &user connexion user name
   *  \param  const QString &_password password : encoded (begin by '#')  or not encoded (don't begin by '#')
   *  \param  const QString &hostname IP adress or domain name
   *  \param  const QString &port port on wich connexion must be opened
   *  \return empty string if no error / error string if error
   */
 QString C_BaseCommon::isThisSchemaExists(const QString &driver,const QString &schema,const QString &user,const QString &_password,const QString &hostname,const QString &port )
 {     QString retError  = "";
       QString password  = _password;
       QString baseLabel = "C_BaseCommon_DB_JUSTFORTEST";
       if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
       QSqlDatabase::removeDatabase (baseLabel);
       //.........  bloc for correct destroy defaultDB (it is local and is destroyed a end of bloc) ............
       //           so a QSqlDatabase::removeDatabase (baseLabel)  can be doing.
       {
          QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, baseLabel);
          defaultDB.setDatabaseName( schema       );
          defaultDB.setUserName    ( user         );
          defaultDB.setPassword    ( password     );
          defaultDB.setHostName    ( hostname     );
          defaultDB.setPort        ( port.toInt() );

          if ( ! defaultDB.open() )
             {        retError = tr("C_BaseCommon::isThisSchemaExist() :\n Failed to open database : ")    +
                      baseLabel                                              +
                      "\n Driver : "    +  QString(driver)                   +
                      "\n Schema : "    +  QString(schema)                   +
                      "\n Message  : "  + defaultDB.lastError().driverText() +
                      "\n Last error :" + defaultDB.lastError().databaseText();
             }
          else
             {defaultDB.close();
             }
       }
       //.........  end bloc for correct destroy defaultDB (it is local an destroy a end of bloc) ............
      QSqlDatabase::removeDatabase (baseLabel);
      return retError;
 }
 //--------------------------- createBase -----------------------------------------------------------
 /*! \brief  test if a data base schema exists by trying a connexion to it. If no error, empty message will be returned
  *  \param  const QString &driver driver name QDB2 QIBASE QMYSQL QOCI QODBC QPSQL QSQLITE QSQLITE2 QTDS
  *  \param  const QString &schemaForConnexion database schema just for access SQL
  *  \param  const QString &user connexion user name
  *  \param  const QString &_password password : encoded (begin by '#')  or not encoded (don't begin by '#')
  *  \param  const QString &hostname IP adress or domain name
  *  \param  const QString &port port on wich connexion must be opened
  *  \param  const QString &schemaToCreate schema to create
  *  \param  int  createMode by default  = C_BaseCommon::OnlyIfNotExits
  *  \return empty string if no error / error string if error
  */
 QString C_BaseCommon::createBase(const QString &driver, const QString &schemaForConnexion,const QString &user,const QString &_password,const QString &hostname,const QString &port,
                                  const QString &schemaToCreate, int createMode /* = C_BaseCommon::OnlyIfNotExits */)
 {   QString retError  = "";
     QString password  = _password;
     QString baseLabel = "C_BaseCommon_DB_JUSTFORTEST";
     if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
     QSqlDatabase::removeDatabase (baseLabel);
     //.........  bloc for correct destroy defaultDB (it is local and is destroyed a end of bloc) ............
     //           so a QSqlDatabase::removeDatabase (baseLabel)  can be doing.
     {
        QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, baseLabel);
        defaultDB.setDatabaseName( schemaForConnexion       );
        defaultDB.setUserName    ( user                     );
        defaultDB.setPassword    ( password                 );
        defaultDB.setHostName    ( hostname                 );
        defaultDB.setPort        ( port.toInt()             );

        if ( ! defaultDB.open() )
           {        retError = tr("C_BaseCommon::createBase() :\n Failed to open database : ")    +
                    baseLabel                                              +
                    "\n Driver : "    + QString(driver)                    +
                    "\n Schema : "    + QString(schemaForConnexion)        +
                    "\n Message  : "  + defaultDB.lastError().driverText() +
                    "\n Last error :" + defaultDB.lastError().databaseText();
           }
        else
           { QSqlQuery query(QString::null , defaultDB );
             QString requete = "";
             if (createMode== C_BaseCommon::OnlyIfNotExits )
                { requete = QString("CREATE DATABASE IF NOT EXISTS %1").arg(schemaToCreate);
                }
             else
                {requete = QString("DROP DATABASE   %1\n"
                                   "CREATE DATABASE %1").arg(schemaToCreate);
                }
             if (!query.exec(requete))
                {  retError = tr("\nERROR : C_BaseCommon::createBase() \n%1\nREQUEST : %2").arg(query.lastError().text(), requete).toLatin1();
                   return retError;
                }
             defaultDB.close();
           }
     }
     //.........  end bloc for correct destroy defaultDB (it is local an destroy a end of bloc) ............
    QSqlDatabase::removeDatabase (baseLabel);
    return retError;
 }

 //--------------------------- createBase -----------------------------------------------------------
 int C_BaseCommon::createBase(const QString &baseName, int createMode /* = C_BaseCommon::OnlyIfNotExits */)
 {   QSqlQuery query(QString::null , database() );
     QString requete = "";
     if (createMode== C_BaseCommon::OnlyIfNotExits )
        { requete = QString("CREATE DATABASE IF NOT EXISTS %1").arg(baseName);
        }
     else
        {requete = QString("DROP DATABASE   %1\n"
                           "CREATE DATABASE %1").arg(baseName);
        }
  if (!query.exec(requete))
     {m_LastError = tr("ERROR : C_BaseCommon::createBase() \n%1\nREQUEST : %2").arg(query.lastError().text(), requete).toLatin1();
      outMessage(m_LastError);
      //CGestIni::Param_UpdateToDisk("/home/ro/sql.txt",tmp_schema);
      return 0;
     }
  return 1;
 }
//-----------------------------------------------------  isThisTableExist -------------------------------------------
 /*! \brief database().tables(QSql::Tables ); return all tables of all databases not just the tables list
  * of our connected database. The way is to see if table selection is possible.
  *  \param  table_name the table name
  *  \return 1/all is ok 0/table not exists
  */
bool C_BaseCommon::isThisTableExist(const QString &table)
{   QSqlQuery query( database() );
    return query.exec( QString("SELECT 1 FROM %1  limit 0").arg(table));
}

//-----------------------------------------------------  tablesList -------------------------------------------
 /*! \brief database().tables(QSql::Tables ); return all tables of all databases not just the tables list
  *  of our connected database. The way is to see if table selection is possible. Its not very fast ...
  *  \return tablesList
  */
QStringList C_BaseCommon::tablesList()
{   QStringList tableList   = database().tables(QSql::Tables );
    QString     tableToTest = "";
    QStringList retList;
    QSqlQuery   query( database() );
    for (int i=0; i<tableList.size();++i)
        { tableToTest = tableList[i];
          if ( retList.indexOf(tableToTest) == -1 &&              // if another database has the same table name ...
               query.exec( QString("SELECT 1 FROM %1  limit 0 ").arg(tableToTest) )
             ) retList.append(tableToTest);
        }
    return retList;
}

//-----------------------------------------------------  setStatusWidget -------------------------------------------
void C_BaseCommon::setStatusWidget(QLabel *statusWidget)
{m_StatusWidget = statusWidget;
 statusWidget->setText("");
}
//-----------------------------------------------------  statusWidget -------------------------------------------
QLabel  *C_BaseCommon::statusWidget()
{return m_StatusWidget;
}
//-----------------------------------------------------  statusWidgetSetText -------------------------------------------
void C_BaseCommon::statusWidgetSetText(const QString & text)
{if (m_StatusWidget) m_StatusWidget->setText(text);
}

//-----------------------------------------------------  setProgressWidget -------------------------------------------
void C_BaseCommon::setProgressWidget(QProgressBar *pQProgressBar)
{m_pQProgressBar = pQProgressBar;
}
//-----------------------------------------------------  progressWidget -------------------------------------------
QProgressBar * C_BaseCommon::progressWidget()
{return m_pQProgressBar;
}

//-----------------------------------------------------  Slot_ReconnectTimerDone -------------------------------------------
void C_BaseCommon::Slot_ReconnectTimerDone()
{if (!database().isOpen()) database().open();
 QSqlQuery query(QString::null , database() );
 query.exec(DUMMY_TEST_SQL_COMMAND);
}

//---------------------------------------- database -------------------------------------------------------
QSqlDatabase C_BaseCommon::database()
{ return QSqlDatabase::database(m_BaseLabel);
}

//--------------------------- setLastError -----------------------------------------------------------
void        C_BaseCommon::setLastError(const QString &error){ m_LastError = error;}
//--------------------------- getLastError -----------------------------------------------------------
QString     C_BaseCommon::getLastError(){return m_LastError;}
//............ gestion des owner ......................
//--------------------------- ownerList -----------------------------------------------------------
QStringList C_BaseCommon::ownerList()   {return m_ownerList;}
//--------------------------- isInOwnerList -----------------------------------------------------------
bool        C_BaseCommon::isInOwnerList(const QString &owner) { return m_ownerList.indexOf(owner) != -1;}
//--------------------------- addOwner -----------------------------------------------------------
void        C_BaseCommon::addOwner     (const QString &owner) { if (m_ownerList.indexOf(owner)==-1) m_ownerList<<owner;}

//--------------------------- getLastPrimaryKey -----------------------------------------------------------
QString C_BaseCommon::getLastPrimaryKey( const QString &table, const QString &pk_field_name, const QString &uniqueValue /*=""*/ , const QString &fieldWhereIsUniqueValue /*=""*/)
{   QString   ret     ="";
    QString   requete ="";
    QSqlQuery query( database() );
    if (uniqueValue.length()==0||fieldWhereIsUniqueValue.length()==0)
       {requete = QString("SELECT LAST_INSERT_ID() FROM %1 WHERE %2  = LAST_INSERT_ID()").arg( table, pk_field_name );
       }
    else
       {requete = QString("SELECT %1 FROM %2 WHERE %3  = '%4'").arg(pk_field_name, table, fieldWhereIsUniqueValue, uniqueValue);
       }
    if ( !query.exec(requete) )
       {m_LastError = tr("\nERROR : C_BaseCommon::getLastPrimaryKey() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toLatin1();
        outMessage(m_LastError);
        return QString::null;
       }
    if ( query.next() ) ret = query.value(0).toString();
    else                ret = QString::null;
    return ret;
}
//--------------------------- dropBase -----------------------------------------------------------
int C_BaseCommon::dropBase(const QString &baseName)
{QString requete = QString("DROP DATABASE IF EXISTS %1"  ).arg(baseName);
 QSqlQuery query(QString::null , database() );
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR : C_BaseCommon::dropBase() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return 0;
    }
 return 1;
}

//--------------------------- dropTable -----------------------------------------------------------
int C_BaseCommon::dropTable(const QString &tableName)
{QString requete = QString("DROP TABLE IF EXISTS `%1`"  ).arg(tableName);
 QSqlQuery query(QString::null , database() );
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR : C_BaseCommon::dropTable() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return 0;
    }
 return 1;
}

//--------------------------- createTable -----------------------------------------------------------
int C_BaseCommon::createTable(const QString &schema)
{QSqlQuery query(QString::null , database() );
 QString tmp_schema = schema;

 #ifdef SQLSYNTAX_MYSQL
         tmp_schema += "ENGINE = MyISAM ";
 #endif

 if ( !query.exec(schema) )
    { m_LastError = tr("\nERROR : C_BaseCommon::createTable() \n%1\nREQUEST : %2").arg(query.lastError().text(), tmp_schema).toLatin1();
      outMessage(m_LastError);
      //CGestIni::Param_UpdateToDisk("/home/ro/sql.txt",tmp_schema);
      return 0;
    }
 return 1;
}

//-----------------------------------------------------  getBaseNameToSetFromSQLFile -------------------------------------------
QString C_BaseCommon::getBaseNameToSetFromSQLFile(const QString &fname)
{   QString baseName   = "";
    QString line       = "";
    long        nbMax  = 9000000;
    int  nbLignesToDo  = 200;
     if ( !QFile::exists( fname ) )            return baseName;
     QFile file( fname );
     if ( !file.open( QIODevice::ReadOnly  ) ) return baseName;

    //.......... lit le fichier ligne à ligne ..................
    while (!file.atEnd() && nbLignesToDo-- && baseName.length()==0)
    {   line = file.readLine(nbMax).trimmed();
        int pos = -1;
        if (line.left(8)=="-- Host:" && (pos=line.indexOf("Database:",8))!= -1)
           { pos += 9;
             baseName =  line.mid(pos).remove(";").remove("\n").remove("\r").trimmed();
           }
        else if (line.left(15)=="-- Base de donn" && (pos=line.indexOf("es:",15))!= -1)
           { pos += 3;
             baseName =  line.mid(pos).remove(";").remove("\n").remove("\r").trimmed().remove('`');
           }
        else if (line.left(29)=="CREATE DATABASE IF NOT EXISTS")
           { baseName =  line.mid(29).remove(";").remove("\n").remove("\r").trimmed();
           }
        else if (line.left(16)=="-- Create schema")              //-- Create schema CIM10Test
           { baseName =  line.mid(16).remove(";").remove("\n").remove("\r").trimmed();
           }
    }
   file.close();
   return baseName;
}
//-----------------------------------------------------  executeSQL -------------------------------------------
/*! \brief  execute SQL instructions file
*   \param  const QString &fname   nom du fichier SQL a interpreter
*   \return nothing
*/
#define INSERT_TO_CLOSED      0
#define INSERT_TO_OPENED      1
#define INSERT_VALUES_OPENED  2
void  C_BaseCommon::executeSQL( const QString &fname)
{//....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fname ) )            return;
    QFile file( fname );
    if ( !file.open( QIODevice::ReadOnly  ) ) return;
    if (m_pQProgressBar)
       {m_pQProgressBar->setValue(0);
        m_pQProgressBar->setRange(0,(int)(file.size()/1000));
       }
    long           nbMax         = 9000000;                // tenir compte del'UF8
    char          *buffer        = new char[nbMax+5];      // un peu de marge
    QString requete              = "";
    QString line                 = "";
    QString lineTrm              = "";
    QString tableAndFieldDef     = "";
    bool constraint_key_removed  = false;
    m_LineNum                    = 0;
    m_position                   = 0;
    int        isInInsert        = INSERT_TO_CLOSED;
    QSqlQuery query(QString::null, database());
    //................... lire ligne a ligne ....................................
    while (!file.atEnd())
    {
        m_position += readLine(&file, buffer, line, nbMax); ++m_LineNum;
        if (m_pQProgressBar)  {m_pQProgressBar->setValue(m_position/1000); qApp->processEvents();qApp->processEvents();}
        //.................creation de la table .....................
        if (line.left(12)=="CREATE TABLE")
        {   int pos_deb          = 12;
            int pos              = line.indexOf("EXISTS",12);
            if (pos!=-1) pos_deb = pos + 6;
            pos = line.indexOf("(",pos_deb);
            if ( pos != -1 )
               {tableAndFieldDef   = line.mid(pos_deb, pos-pos_deb).trimmed();
                requete            = QString("DROP TABLE IF EXISTS %1;").arg(tableAndFieldDef);
                query.exec( requete );
                outSQL_error(query, ("<font color=#ff0000><b>"+tr("ERROR:")+ "ParseSQL_Dump() </b></font>"), requete, __FILE__, __LINE__  );
                outMessage((tr("Creating table")+ "<font color=#01eaff><b>%1</b></font>").arg( tableAndFieldDef)) ;
               }
            requete = line;
            while (!file.atEnd())
            {   m_position += readLine(&file, buffer, line, nbMax);   ++m_LineNum;
                lineTrm     = line.trimmed();
                if ( lineTrm.startsWith("KEY") )         {constraint_key_removed=true; continue;}  // on vire ces mentions pas toujours acceptees selon le moteur
                if ( lineTrm.startsWith("CONSTRAINT") )  {constraint_key_removed=true; continue;}  // on vire ces mentions pas toujours acceptees selon le moteur

                line.remove("USING BTREE");
                if (m_pQProgressBar)  {m_pQProgressBar->setValue(m_position/1000); qApp->processEvents();qApp->processEvents();}
                if (line[0]==')' && (line[2]=='T' || line[2]=='E'))
                   {   if (line.left(7) == ") TYPE=" || line.left(9) == ") ENGINE=" )  // ne surtout pas inclure ';' dans la comparaison
                          {
                            //....... si des lignes du type CONSTRAINT ou KEY ont ete enlevees ............
                            //        alors il se peut qu'une virgule soit a enlever en fin
                            //        de la liste des champs
                            requete = requete.trimmed();
                            if (requete.endsWith(',')) requete.chop(1);
                            requete += "\n";
                            //......... retablir le bon type de moteur ....................................
                            if       (line.contains("=MyISAM"))   requete += ") ENGINE=MyISAM;";
                            else if  (line.contains("=InnoDB"))   requete += ") ENGINE=InnoDB;";
                            else                                  requete += ");";
                            //......... laissons le moteur choisir ........................................
                            requete.remove ("collate utf8_unicode_ci");
                            requete.remove ("collate utf8_bin");
                            requete.remove ("character set utf8");
                            //......... croisons les doigts ...............................................
                            query.exec(requete);
                            outSQL_error(query, "<font color=#ff0000><b> "+tr("ERROR:")+" parseSQL_Dump()</b></font>", requete ,__FILE__, __LINE__);
                            constraint_key_removed = false;
                            break;
                          }
                   }
                requete += line;
            } //endwhile (!file.atEnd())
        }
// INSERT INTO `RubriquesHead` VALUES (1,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20030000,'Asthme sai','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `RubriquesHead` VALUES (2,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20020200,'Bilan labo complet','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `ACCES2` VALUES ('A','a foyer ouvert'),('A','par ... abord '),('A','par dissection ...');
// INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
//  (26662,62153,'ABLAIN ST NAZAIRE'),
//  (35076,80320,'ABLAINCOURT PRESSOIR');
// INSERT INTO `BDM_CIP` (`CIP`, `CIP7`, `CIP_UCD`, `NATURE`, `NOM_COURT`, `INDIC_COND`, `DEBUT_REMB`, `FIN_REMB`, `CODE_LISTE`, `CODE_FORME`, `FORME`, `CODE_CPLT`, `CPLT_FORME`, `DOSAGE_SA`, `UNITE_SA`, `NB_UNITES`, `CODE_ATC`, `CLASSE_ATC`, `CODE_EPH`, `CLASSE_EPH`, `LABO`, `NOM_LONG1`, `NOM_LONG2`, `DATE_EFFET`, `SEUIL_ALER`, `SEUIL_REJE`, `PRESC_REST`, `EXCEPTIONS`, `TYPE`, `SEXE`, `INTERACT`, `PIH`, `PECP`) VALUES
// ('3400930000649', '3000064', '9000059', 'A', 'A 313 50 000 UI CAPSULE 30', '**', '1962-03-22', '0000-00-00', 1, '9', 'CAPSULE MOLLE', '320', 'CAPSULE MOLLE ORAL(E)', '50000', 'UI', '30', 'A11CA01', 'RETINOL (VIT A)', 'A11C1', 'VITAMINE A', 'PHARMA DEVELOPPEMENT', 'A 313 50 000 UI (VITAMINE A SYNTHETIQUE) 1 BOITE DE 30, CAPSULES MOLLES', '', '1999-03-02', 9, 21, 'N', 'N', '', 0, 0, '00', '00'),

        else if (isInInsert || line.left(11)=="INSERT INTO")
        {   int  len_line           = line.length();
            int       pos           = 0;
            int   posDebTable       = -1;
            int   posEndTable       = -1;
            if ( isInInsert != INSERT_VALUES_OPENED )                             // SI INSERTION NON OUVERTE
               { if (line.left(11)=="INSERT INTO")
                    { pos             += 11;
                      posDebTable      = pos;
                      isInInsert       = INSERT_TO_OPENED;                        // ON OUVRE L'INSERTION GENERALE
                    }
                 //........... determner ci cette ligne est une specifiant les values ou non .....................
                 while ( isInInsert != INSERT_VALUES_OPENED &&  pos<len_line )
                  { if      (line[pos]=='(')     // >>> passer l'eventuelle mention des champs a traiter type :  INSERT INTO `BDM_CIP` (`CIP`, `CIP7`)
                            { pos  = gotoClosedSymbol(line, pos, '(', ')');
                              if (pos==-1)
                                 { outMessage(QObject::tr("    <font color=#ff0000><b>Syntax Error  line : %1 INSERT INTO VALUES : ParseSQL_InsertInto() first '('' found, but not last ')' )</b></font>").arg(QString::number(m_LineNum)));
                                 }
                            }
                    else if ( (pos+6<len_line) && line[pos]=='V' && line[pos+1]=='A' && line[pos+2]=='L'  && line[pos+3]=='U' && line[pos+4]=='E' && line[pos+5]=='S' )
                            {  posEndTable             = pos;
                               tableAndFieldDef        = line.mid(posDebTable, posEndTable-posDebTable).trimmed();
                               pos                    += 6;
                               isInInsert              = INSERT_VALUES_OPENED;    // ON OUVRE L'INSERTION DES VALEURS  desormais les lignes suivantes doivent traiter des insertions
                            }
                    else ++pos;
                  } // while (pos<len_line)
               }  // if ( isInInsert != INSERT_NOT_IN_VALUES )
            if ( isInInsert != INSERT_VALUES_OPENED ) continue;

            // si on en est la , la ligne ne concerne que des valeurs a inserer
            // ('A','a foyer ouvert'),('A','par ... abord '),('A','par dissection ...')
            // (26662,62153,'ABLAIN ST NAZAIRE'),
            // (35076,80320,'ABLAINCOURT PRESSOIR')
            // avancer
             QString values     = "";
             int posDebValues   = -1;
             QStringList valuesList;
             QString  err;
             while ( pos != -1 && pos < len_line )
                   { pos   = gotoClosedSymbol( line, pos,'(', ')', &posDebValues, m_LineNum );
                     if ( pos<len_line && posDebValues !=  -1 )
                        { ++posDebValues;
                          values        = line.mid(posDebValues, pos-posDebValues);   // interieur
                          valuesList    =  parseValues(values, m_LineNum, err);
                          insertValues( valuesList, tableAndFieldDef, m_LineNum );
                          /*
                          requete       = "INSERT INTO " +  table + " VALUES (" + values + ")";
                          query.exec(requete);
                          outSQL_error(query, tr("<font color=#ff0000><b>ERROR : parseSQL_Dump() line %1 </b></font>").arg(QString::number(m_LineNum)), requete , __FILE__, __LINE__);
                          */
                        }
                   }
              //......... detection de la fin des INSERT INTO .......................
              if (len_line>2) //........ se placer jusqu'au dernier caracter signifiant de la ligne;
                 { while (line[len_line-1]=='\r' || line[len_line-1]=='\n' || line[len_line-1]=='\t') --len_line;
                   if (line[len_line-2]==')' && line[len_line-1]==';')
                      { isInInsert        = INSERT_TO_CLOSED;
                        tableAndFieldDef  = "";
                      }
                 }
            }  // else if (isInInsert || line.left(11)=="INSERT INTO")
    } // while (!file.atEnd())
 delete buffer;
}
//-----------------------------------------------------  insertValues -------------------------------------------
/*! \brief  insert values in a database table from a QStringList. This QStringList must be have exactly the same number datas fields than table definition
 *  \param  valuesList datas field list
 *  \param  tableAndFieldDef database table to be populate with datas list can be with fields to populate: `chapter` (`chap`,`SID`,`rom`) or without fields indications : `chapter`
 *  \param  l_number  current line number
 *  \return 0 if error / 1 if all is Ok
 */
int C_BaseCommon::insertValues( const QStringList &valuesList, const QString &tableAndFieldDef, int l_number)
{  QSqlQuery query(QString::null, database());
   QString prepare = "INSERT INTO " +  tableAndFieldDef +" VALUES (";
   for (int i = 0; i<valuesList.size(); ++i)
       {  prepare += "?,";
       }
   prepare.chop(1);   // virer la derniere virgule
   prepare += ")";
   if ( !query.prepare(prepare))
      {   QString tableName     = tableAndFieldDef;               // peut etre : `chapter` (`chap`,`SID`,`rom`)
          int p                 = tableAndFieldDef.indexOf('(');  // donc dans ce cas le nom de la table doit etre extrait
          if (p!=-1) tableName  = tableAndFieldDef.left(p);
          outSQL_error(query, tr("<font color=#ff0000><b>ERROR : insertValues(%2) line %1 </b></font>").arg(QString::number(l_number),tableName), prepare , __FILE__, __LINE__);
          return 0;
      }
   for (int i = 0; i<valuesList.size(); ++i)
       {  QVariantList var_list;
          if (valuesList[i].left(2).toLower()=="0x")
             { var_list <<  QByteArray::fromHex(valuesList[i].mid(2).toLatin1());
             }
          else
             { QString vatToBind = valuesList[i];
               var_list << vatToBind.replace("\\r","\r").replace("\\n","\n").replace("\\0","\0");
             }
          query.addBindValue(var_list);
       }
   if ( !query.execBatch() )
      {   QString tableName     = tableAndFieldDef;               // peut etre : `chapter` (`chap`,`SID`,`rom`)
          int p                 = tableAndFieldDef.indexOf('(');  // donc dans ce cas le nom de la table doit etre extrait
          if (p!=-1) tableName  = tableAndFieldDef.left(p);
          outSQL_error(query, tr("<font color=#ff0000><b>ERROR : insertValues(%2) line %1 </b></font>").arg(QString::number(l_number),tableName), prepare , __FILE__, __LINE__);
          return 0;
      }
   return 1;
}

//-----------------------------------------------------  parseValues -------------------------------------------
/*! \brief  extract a list which is all datas fields from a SQL INSERT INTO string  value1 , 'value' , 'machin(', 'truc''chose', bidule\(.
 *  \param  txt the text to evalated
 *  \param  l_number  current line number
 *  \param  err if error it's will be returned in in
 *  \return QString list of each fields content
 */
QStringList  C_BaseCommon::parseValues(const QString &txt, int l_number, QString &err)
{ int pos          = 0;
  int lastPos      = 0;
  int len          = txt.length();
  err              = "";
  QString value    = "";
  //QString       t  = "";
  QStringList toBind;
  if (l_number==6033)
     { qDebug()<<QString::number(l_number);
     }
  while ( pos<=len )
        { // if (pos+1<len)
          //    { t = txt[pos];
          //    }
           if (pos+1<len && txt[pos]=='\'')       // si valeur encadree par '  aller au dernier en echappant l'interieur
              { pos = gotoEndEnClosedValue(txt, pos);
                if ( pos ==  -1 )
                   { err = QObject::tr("    <font color=#ff0000><b>Syntax Error  line : %1 INSERT INTO VALUES : parseValues() first ' found, but not last  )</b></font>").arg(QString::number(l_number));
                     return toBind;
                   }
                ++pos;    // passer le dernier '
              }
         //  if (pos+1<len)
         //     { t = txt[pos];
         //     }
           if ( pos==len || txt[pos]==',' ) // DANGER si pos==len on suppose que le 2eme test n'est pas fait car plantage lors txt[pos] si pos=len
              {   //                                                       (value1 , value , machin)
                  value = txt.mid(lastPos, pos-lastPos).trimmed();
                  if (value.startsWith('\'')) value = value.mid(1);
                  if (value.endsWith  ('\'')) value.chop(1);
                  toBind.append(value);
                  lastPos = pos+1;
              }
           ++pos;
        }
  return toBind;
}

//-----------------------------------------------------  gotoEndEnClosedValue -------------------------------------------
/*! \brief  goto end of mysql enclosed value by '
 *  \param  txt the text to evalated
 *  \param  pos position of the first open symbol must be just before first openSymbol;
 *  \return position of the last symbol -1 if not found
 */
int  C_BaseCommon::gotoEndEnClosedValue(const QString &txt, int pos)
{ int len    = txt.length();
  if (txt[pos] != '\'') return -1;   // le demarrage doit se faire juste devant le premier symbole
  ++pos;   // passer le symbole
  while ( pos<len )
        { if      ( pos+1<len && txt[pos]=='\''&& txt[pos+1]=='\''   ) ++pos;   // echappement mysql
          else if ( pos+1<len && txt[pos]=='\\' )                      ++pos;   // echappement standard
          else if ( txt[pos]=='\'' ) return pos;
          ++pos;
        }
  return -1;
}
//-----------------------------------------------------  gotoClosedSymbol -------------------------------------------
/*! \brief find la corespondant closed from a start symbol position in text.
 *  \param  txt the text to evalated
 *  \param  pos position of the first open symbol
 *  \param  startSymbol open open symbol
 *  \param  closedSymbol the closed symbol
 *  \param  posOpenSymbol if not null position of open symbol will be returned in
 *  \param  l_number  current line number
 *  \return position of the last symbol -1 if not found
 */
//  (25,'3400921617535','2161753','0000009377539','A','VALSARTAN/HCTZ CRT 80/12,5MG CPR 30','*','04/11/11','','1','15','COMPRIME','307','COMPRIME PELLICULE NE PAS ECRASER ORAL(E','80','MG','30','C09DA03','VALSARTAN ET DIURETIQUES','C09D1','ANTAGONISTES ANGIOTENSINE II EN ASSOC. AVEC ANTIHYPERTENS ET/OU DOURET','CRISTERS','VALSARTAN/HYDROCHLOROTHIAZIDE CRISTERS 80 MG/12.5 MG 1 BOITE DE 30, COMPRIMES PELLICULES','','0','01/12/11','1','9','N','N','','0','1','00',NULL),\n
int  C_BaseCommon::gotoClosedSymbol(const QString &txt, int pos, const QChar &openSymbol, const QChar &closedSymbol, int *posOpenSymbol /* = 0 */, int l_number /* = -1 */)
{
   Q_UNUSED(l_number);
   // if (l_number==6033)
   // { qDebug()<<QString::number(l_number);
   // }
  //QString  t = "";
  int inside = 0;
  int len    = txt.length();
  if (posOpenSymbol) *posOpenSymbol = -1;
  while ( pos<len && txt[pos]!=openSymbol ) ++pos;
  if (pos==len) return len;
  if (posOpenSymbol) *posOpenSymbol = pos;
  while ( pos<len )
        { //t = txt[pos];
          if (txt[pos]=='\'')   // si debut d'une enclosure de donnees aller jusqu'au bout
             { pos = gotoEndEnClosedValue(txt,pos);
               if (pos==-1) return -1;
               ++pos;  // passer la derniere enclosure
             }
          if      ( txt[pos]==openSymbol   ) ++inside;
          else if ( txt[pos]==closedSymbol ) --inside;
          if      ( inside==0 ) return pos;
          ++pos;
        }
  return -1;
}

//-----------------------------------------------------  readLine -------------------------------------------
unsigned long  C_BaseCommon::readLine(QFile *pQFile, char *buffer, QString &outParam, unsigned long nbMax)
{
         unsigned long nb                              =  pQFile->readLine(buffer, nbMax);
         buffer[nb]                                    =  0;
         if (CGestIni::IsUtf8(buffer))     outParam    =  QString::fromUtf8 ( buffer ) ;
         else                              outParam    =  buffer;
         return nb;
}

//--------------------------- isThisValueLikeInTable -----------------------------------------------------------
QString  C_BaseCommon::isThisValueLikeInTable(const QString &tableName, const QString &fieldName, const QString &value, const QString fieldToRetrieve /* = "" */, const QString &wand /* = "" */)
{QSqlQuery query(QString::null , database() );
 QString field    = fieldToRetrieve; if (field.length()==0) field = tableName+"_pk";
 QString requete  = QString("SELECT %1 FROM %2 WHERE %3  LIKE ").arg( field, tableName, fieldName)+"'%" +value+"%'";
 addOwnerWandMention(requete, tableName, wand);

 if (!query.exec(requete))
    {m_LastError = tr("\nERROR: C_BaseCommon::isThisValueLikeInTable_ToPk( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return QString::null;
    }
 if ( query.next() ) return CGestIni::Utf8_Query(query, 0);
 else                return QString::null;
}

//--------------------------- isThisValueLikeInTable_ToList -----------------------------------------------------------
QStringList  C_BaseCommon::isThisValueLikeInTable_ToList(const QString &tableName, const QString &fieldName, const QString &value, const QString fieldToRetrieve /* = "" */, const QString &wand /* = "" */ )
{QSqlQuery query(QString::null , database() );
 QString field    = fieldToRetrieve; if (field.length()==0) field = tableName + "_pk";
 QString requete  = QString("SELECT %1 FROM %2 WHERE %3  LIKE ").arg( field, tableName, fieldName)+"'%" +value+"%'";
 addOwnerWandMention(requete, tableName, wand);
 QStringList list;
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR: C_BaseCommon::isThisValueLikeInTable_ToPkList() \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return list;
    }
 if (query.isActive())
    {while (query.next()) list << CGestIni::Utf8_Query(query, 0);
    }
 return list;
}
//--------------------------- retrieveValuesInTable -----------------------------------------------------------
QStringList C_BaseCommon::retrieveValuesInTable(const QString &tableName, const QString &fieldToTest, const QString &test_value, const QString &fieldsToRetrieve , const QString &wand /* = "" */)
   {QStringList retList;
    QSqlQuery query(QString::null , database() );
    int nb_fields               =  fieldsToRetrieve.count(',')+1;
    QString requete             =  QString( " SELECT %1 FROM %2 WHERE %3  = \"%4\"" ).arg( fieldsToRetrieve, tableName, fieldToTest, test_value );
    if (wand.length()) requete +=           " AND " + wand;

    if (!query.exec(requete))
       { m_LastError = tr("\nERROR: C_BaseCommon::retrieveValuesInTable( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
         outMessage(m_LastError);
         return retList;
       }
    while ( query.next() )
         {  for (int i=0; i<nb_fields; ++i)
                { retList << CGestIni::Utf8_Query(query, i);
                }
         }
    return retList;
}

//--------------------------- retrieveValuesInTable -----------------------------------------------------------
QStringList C_BaseCommon::retrieveValuesInTable(const QString &tableName, const QString &fieldToTest, const QString &test_value, const QStringList &fieldsToRetrieve , const QString &wand /* = "" */)
{QStringList retList;
 QSqlQuery query(QString::null , database() );
 QString fields   = fieldsToRetrieve.join(",");
 if (fields.endsWith(',')) fields.chop(1);

 QString requete  =      QString(" SELECT %1 FROM %2 WHERE %3  = \"%4\"").arg( fields, tableName, fieldToTest, test_value );
 if (wand.length()) requete +=   " AND " + wand;

 if (!query.exec(requete))
    { m_LastError = tr("\nERROR: C_BaseCommon::retrieveValuesInTable( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
      outMessage(m_LastError);
      return retList;
    }
 while ( query.next() )
      {  for (int i=0; i<fieldsToRetrieve.size(); ++i)
             { retList << CGestIni::Utf8_Query(query, i);
             }
      }
 return retList;
}
//--------------------------- isThisValueInTable -----------------------------------------------------------
QString C_BaseCommon::isThisValueInTable(const QString &tableName, const QString &fieldToTest, const QString &test_value, const QString fieldToRetrieve /* = "" */, const QString &wand /* = "" */)
{
 QSqlQuery query(QString::null , database() );
 QString field    = fieldToRetrieve; if (field.length()==0) field = tableName+"_pk";
 QString requete  = QString("SELECT %1 FROM %2 WHERE %3  = \"%4\"").arg( field, tableName, fieldToTest, test_value );
 addOwnerWandMention(requete, tableName, wand);
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR : C_BaseCommon::isThisValueInTable( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return QString::null;
    }
 if ( query.next() ) return CGestIni::Utf8_Query(query, 0);
 else                return QString::null;
}
//--------------------------- retrieveQVariantInTable -----------------------------------------------------------
QVariant C_BaseCommon::retrieveQVariantInTable(const QString &tableName, const QString &fieldToTest, const QString &test_value, const QString &fieldToRetrieve /* = "" */, const QString &wand /* = "" */)
{
 QSqlQuery query(QString::null , database() );
 QString field    = fieldToRetrieve; if (field.length()==0) field = tableName+"_pk";
 QString requete  = QString("SELECT %1 FROM %2 WHERE %3  = \"%4\"").arg( field, tableName, fieldToTest, test_value );
 addOwnerWandMention(requete, tableName, wand);
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR : C_BaseCommon::retrieveQVariantInTable( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return QVariant();
    }
 if ( query.next() ) return query.value(0);
 else                return QVariant();
}
//--------------------------- addOwnerWandMention -----------------------------------------------------------
QString C_BaseCommon::addOwnerWandMention(QString &requete, const QString &tableName, const QString &wand)
{ if (wand.length())
    {if (wand.startsWith("NO-OWNER-FILTER"))
        {requete += wand.mid(15);
        }
     else
        {requete += wand;
         requete += ownersSelectMention(tableName);
        }
    }
 else
   { requete += ownersSelectMention(tableName);
   }
 return requete;
}

//--------------------------- isThisValueInTable_ToList -----------------------------------------------------------
QStringList C_BaseCommon::isThisValueInTable_ToList(const QString &tableName, const QString &fieldName, const QString &value, const QString fieldToRetrieve /* = "" */, int keepEmpty /* = 1 */, const QString &wand /* = "" */)
{QSqlQuery query(QString::null , database() );
 QString            field    = fieldToRetrieve; if (field.length()==0) field = tableName+"_pk";
 QString            requete  = QString("SELECT %1 FROM %2 WHERE %3  = \"%4\"").arg( field, tableName, fieldName, value );
 addOwnerWandMention(requete, tableName, wand);
 QStringList list;
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR : C_BaseCommon::isThisValueLikeInTable_ToPkList() \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return list;
    }
 if (query.isActive())
    {if (keepEmpty)
        {while (query.next()) list << CGestIni::Utf8_Query(query, 0);
        }
     else
         {while (query.next())
                {QString value = CGestIni::Utf8_Query(query, 0).trimmed();
                 if (value.length()) list <<  value;
                }
         }
    }
 return list;
}
//--------------------------- updateValueInTable -----------------------------------------------------------
bool C_BaseCommon::updateValueInTable(const QString &tableName, const QString &fieldToUpdate, const QString &value, const QString &valueToTest, const QString &fieldToTest /* = ""*/ )
{QSqlQuery query( database() );
 QString field_test = fieldToTest; if (field_test.length()==0) field_test = tableName + "_pk";
 QString prepare  = "UPDATE ";
 prepare         +=  tableName                + " SET \n";
 prepare         +=  fieldToUpdate            + " =?  \n";
 prepare         += " WHERE "   + field_test  + " ='"  + valueToTest  + "'";

 if (!query.prepare(prepare))
    {outSQL_error( query, tr("ERROR: C_BaseCommon::updateValueInTable() prepare"), prepare, __FILE__, __LINE__);
     return false;
    }
 query.bindValue(0, value);
 if (!query.exec())
    {outSQL_error( query, tr("ERROR: C_BaseCommon::updateValueInTable() exec"), prepare, __FILE__, __LINE__);
     return false;
    }
 return true;
}

//--------------------------- deleteRecordInTable -----------------------------------------------------------
bool C_BaseCommon::deleteRecordInTable(const QString &tableName, const QString &valueToTest, const QString &fieldToTest  /*=""*/ )
{QSqlQuery query( database() );
 QString field_test = fieldToTest; if (field_test.length()==0) field_test = tableName + "_pk";
 QString requete    = QString("DELETE FROM %1 WHERE %2='%3' ").arg(tableName, field_test, valueToTest);

 if (!query.exec(requete))
    {outSQL_error( query, tr("ERROR: C_BaseCommon::deleteRecordInTable() "), requete, __FILE__, __LINE__);
     return false;
    }
 return true;
}

//--------------------------- ownersSelectMention -----------------------------------------------------------
QString C_BaseCommon::ownersSelectMention( const QString &tableName, C_BaseCommon::flags flags/*= C_BaseCommon::WhereAlreadyIn*/)
{QString str = "";
 int      nb = m_ownerList.size(); if (nb==0) return str;
 if (flags == C_BaseCommon::WhereAlreadyIn) str        += " AND   (";
 else                                       str        += " WHERE (";
 for (int i=0; i<nb; ++i)
     {if (i==0) str += QString("    `%1_owner`=\"%2\"").arg(tableName, m_ownerList[i]);
      else      str += QString(" OR `%1_owner`=\"%2\"").arg(tableName, m_ownerList[i]);
     }
 str        += " )";
 return str;
}
//--------------------------- countRecords -----------------------------------------------------------
long C_BaseCommon::countRecords(const QString & tableName, const QString &whereMention /* = "" */)
{QString requete = QString("SELECT COUNT(*) FROM %1 %2").arg(tableName, whereMention);
 QSqlQuery query(requete , database() );
 outSQL_error(query, tr("<font color=#ff0000><b>ERROR: C_BaseCommon::countRecords() </b></font>"), requete, __FILE__, __LINE__  );
   if (query.isActive() && query.next())
      {return (long) query.value(0).toDouble();
      }
 return 0;
}

//--------------------------- eraseTable -----------------------------------------------------------
int C_BaseCommon::eraseTable(const QString &tableName)
{QString requete = QString("DELETE FROM %1").arg(tableName);
 QSqlQuery query(QString::null , database() );
 if (!query.exec(requete))
    {m_LastError = tr("\nERROR: C_BaseCommon::eraseTable() \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
     outMessage(m_LastError);
     return 0;
    }
 return 1;
}

//--------------------------- posFirstNonSpace -----------------------------------------------------------
int C_BaseCommon::posFirstNonSpace(const QString &line)
{int nb = line.length();
 for (int i=0; i<nb;++i)
     {if (line[i] != ' ') return i;
     }
 return -1;
}

//--------------------------- posLastEnclosedSymbol -----------------------------------------------------------
int C_BaseCommon::posLastEnclosedSymbol(const QString &str, const QString &symbolOuvrant, const QString &symbolFermant, int *posFermant_out /*=0*/)
{int pos        = str.length();
 int posOuvrant = -1;
 //int posFermant = -1;
 int isIn       = 0;
 int lenOuvrant = symbolOuvrant.length();
 int lenFermant = symbolFermant.length();
 if (posFermant_out) *posFermant_out = -1;
 if (pos==0)     return -1;

 do
    {--pos;
     if      (str.mid(pos,lenFermant)==symbolFermant)
             {if (posFermant_out && isIn==0) *posFermant_out = pos;
              ++isIn;
             }
     else if (str.mid(pos,lenOuvrant)==symbolOuvrant)
             {--isIn;
              posOuvrant = pos;
             }
     if (posOuvrant != -1 && isIn<=0) break;
    } while (pos);
 return posOuvrant;
}

//-----------------------------------------------------  GUID_Create --------------------------------------------------
QString C_BaseCommon::GUID_Create()
{
    if (get_GUID_CreateMode()==C_BaseCommon::byQT)
       {QString guid ( QUuid::createUuid ().toString());
        return  guid.mid(1, 36);
       }
    else
       {
        return C_MyUuid::create();     // version personnelle de creation du GUID car celle de QT buguee? sur certaines plateformes
       }
}

//-----------------------------------------------------  get_GUID_CreateMode --------------------------------------------------
C_BaseCommon::GUID_Mode C_BaseCommon::get_GUID_CreateMode()
{return  m_GUID_CreateMode;
}

