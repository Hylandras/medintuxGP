#ifndef C_BASECOMMON_H
#define C_BASECOMMON_H
#include <QString>
#include <QObject>
#include <QStringList>
#include "C_Utils_Log.h"

class QSqlQuery;
class QSqlError;
class QSqlDatabase;
class QTimer;
class QTextEdit;
class QProgressBar;
class QFile;
class QProgressBar;
class QLabel;
#define   DB_TIME_TO_RECONNECT     3600000  //reconnexion toutes les heures
//=================================================== C_BaseCommon ====================================================
class C_BaseCommon: public QObject, public C_Log
{

Q_OBJECT


public:
    enum flags  {WhereMustBeAdd = 0,
                 WhereAlreadyIn = 1
                };
    enum GUID_Mode {
            byQT        = 0,        // algorithme de QT
            byMyself    = 1         // algorithme interne a MedinTux
        };
    enum Create_Mode {
            DropIfExists     = 1,        //
            OnlyIfNotExits   = 0         //
        };

    C_BaseCommon(QObject *parent);
    ~C_BaseCommon();
    //---------------------------------------- BaseConnect -------------------------------------------------------
    bool               BaseConnect(const QString &param, const QString &baseLabel, const QString &dataBase="", QString * errMess = 0, const QString &section   = "Connexion" , const QString &key = "Parametres") ;
    bool               BaseConnect(const QString &baseLabel,    // 0
                                   const QString &driver,       // 1
                                   const QString &dataBase,     // 2
                                   const QString &user ,        // 3
                                   const QString &_password ,   // 4
                                   const QString &hostname ,    // 5
                                   int port,                    // 6
                                   int      timeToReconnect = DB_TIME_TO_RECONNECT,
                                   QString *errMess         = 0);

    //---------------------------------------- EncodePassword_InConnectionParam [static]-------------------------------------------------------
    static bool        EncodePassword_InConnectionParam(const QString &param, const QString& section, const QString &key="Parametres", QString *errMess=0);
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
    int changeDataBaseConnexion(const QString &driver, const QString &schema, const QString &user, const QString &password, const QString &hostname, const QString &port);
    //---------------------------------  isThisSchemaExists [static]-------------------------------------------
     /*! \brief  test if a data base schema exists by trying a connexion on it. If no error, empty message will be returned
      *  \param  const QString &schema schema database name to test
      *  \return empty string if no error / error string if error
      */
    QString isThisSchemaExists( const QString &schema );
    //---------------------------------  isThisSchemaExists [static]-------------------------------------------
     /*! \brief  test if a data base schema exists by trying a connexion on it. If no error, empty message will be returned
      *  \param  const QString &driver driver name QDB2 QIBASE QMYSQL QOCI QODBC QPSQL QSQLITE QSQLITE2 QTDS
      *  \param  const QString &schema schema database name to test
      *  \param  const QString &user connexion user name
      *  \param  const QString &_password password : encoded (begin by '#')  or not encoded (don't begin by '#')
      *  \param  const QString &hostname IP adress or domain name
      *  \param  const QString &port port on wich connexion must be opened
      *  \return empty string if no error / error string if error
      */
    static QString isThisSchemaExists( const QString &driver, const QString &schema, const QString &user, const QString &_password, const QString &hostname, const QString &port );
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
    static QString createBase( const QString &driver, const QString &schemaForConnexion,const QString &user,const QString &_password,const QString &hostname,const QString &port,
                               const QString &schemaToCreate, int createMode = C_BaseCommon::OnlyIfNotExits );
    //-----------------------------------------------------  isThisTableExist -------------------------------------------
     /*! \brief database().tables(QSql::Tables ); return all tables of all databases not just the tables list
      * of our connected database. The way is to see if table selection is possible.
      *  \param  table_name the table name
      *  \return 1/all is ok 0/table not exists
      */
    bool         isThisTableExist(const QString &table);
    //-----------------------------------------------------  databasesList -------------------------------------------
     /*! \brief  return databases list  of all databases
      *  \return databases list
      */
    QStringList databasesList();
    //-----------------------------------------------------  tablesList -------------------------------------------
     /*! \brief database().tables(QSql::Tables ); return all tables of all databases not just the tables list
      *  of our connected database. A way is to see if table selection is possible. Its not very fast ...
      *  \return tablesList
      */
    QStringList   tablesList();

    QSqlDatabase  database();
    //............ utilities .............................
    QString     isThisValueLikeInTable(       const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QString fieldToRetrieve = "" ,  const QString &wand  = "" );
    QStringList isThisValueLikeInTable_ToList(const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QString fieldToRetrieve = "" ,  const QString &wand  = "" );
    QString     isThisValueInTable(           const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QString fieldToRetrieve = "" ,  const QString &wand  = "" );
    QStringList retrieveValuesInTable(        const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QString     &fieldsToRetrieve , const QString &wand  = "" );
    QStringList retrieveValuesInTable(        const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QStringList &fieldsToRetrieve , const QString &wand  = "" );
    QVariant    retrieveQVariantInTable(      const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QString     &fieldToRetrieve  , const QString &wand  = "" );

    QStringList isThisValueInTable_ToList(    const QString &tableName, const QString &fieldToTest, const QString &test_value,     const QString fieldToRetrieve = "" , int keepEmpty = 1, const QString &wand  = "");
    bool        updateValueInTable( const QString &tableName, const QString &fieldToUpdate, const QString &value, const QString &valueToTest, const QString &fieldToTest="" );
    bool        deleteRecordInTable(const QString &tableName, const QString &valueToTest,   const QString &fieldToTest="" );

    int         dropBase(const QString &baseName);
    int         createBase(const QString &baseName, int createMode = C_BaseCommon::OnlyIfNotExits);
    int         dropTable(const QString &tableName);
    int         createTable(const QString &requete);
    int         eraseTable(const QString &tableName);
    QString     getLastPrimaryKey(    const QString &table, const QString &pk_field_name, const QString &uniqueValue ="", const QString &fieldWhereIsUniqueValue ="");
    QString     getLastError();
    long        countRecords(const QString & tableName, const QString &whereMention = "" );
    C_BaseCommon::GUID_Mode get_GUID_CreateMode();
    QString       GUID_Create();
    //------------------------------------------- signalisateurs graphiques -------------------------------------------
    void         setStatusWidget(QLabel *statusWidget);                   // statusWidget
    QLabel      *statusWidget();
    void         statusWidgetSetText(const QString & text);
    void         setProgressWidget(QProgressBar *pQProgressBar);  //progressWidget
    QProgressBar *progressWidget();

    //............... insertion SQL .......................................
    static QString  getBaseNameToSetFromSQLFile(const QString &fname);
    unsigned long   readLine(QFile *pQFile, char *buffer, QString &outParam, unsigned long nbMax);
    //-----------------------------------------------------  executeSQL -------------------------------------------
    /*! \brief  execute SQL instructions file
    *   \param  const QString &fname                nom du fichier SQL a interpreter
    *   \return
    */
    void            executeSQL(const QString &fname);
    //-----------------------------------------------------  parseValues -------------------------------------------
    /*! \brief  extract a list which is all datas fields from a SQL INSERT INTO string  value1 , 'value' , 'machin(', 'truc''chose', bidule\(.
     *  \param  txt the text to evalated
     *  \param  l_number  current line number
     *  \param  err if error it's will be returned in in
     *  \return QString list of each fields content
     */
    QStringList     parseValues(const QString &txt, int l_number, QString &err);
    //-----------------------------------------------------  insertValues -------------------------------------------
    /*! \brief  insert values in a database table from a QStringList. This QStringList must be have exactly the same number datas fields than table definition
     *  \param  valuesList datas field list
     *  \param  tableAndFieldDef database table to be populate with datas list can be with fields to populate: `chapter` (`chap`,`SID`,`rom`) or without fields indications : `chapter`
     *  \param  l_number  current line number
     *  \return 0 if error / 1 if all is Ok
     */
    int             insertValues(const QStringList &valuesList, const QString &table, int l_number);
    //-----------------------------------------------------  gotoEndEnClosedValue -------------------------------------------
    /*! \brief  goto end of mysql enclosed value by '
     *  \param  txt the text to evalated
     *  \param  pos position of the first open symbol must be just before first openSymbol;
     *  \return position of the last symbol -1 if not found
     */
    int             gotoEndEnClosedValue(const QString &txt, int pos);
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
    int           gotoClosedSymbol(const QString &txt, int pos, const QChar &openSymbol, const QChar &closedSymbol, int *posOpenSymbol = 0 , int l_number=-1);
    int           posFirstNonSpace(const QString &line);
    int           posLastEnclosedSymbol(const QString &str,   const QString &symbolOuvrant, const QString &symbolFermant, int *posFermant_out=0);
    //............ gestion des owner ......................
    QStringList ownerList();
    QString     ownersSelectMention( const QString &tableName,  C_BaseCommon::flags = C_BaseCommon::WhereAlreadyIn );
    bool        isInOwnerList(const QString &owner);
    void        addOwner     (const QString &owner);
    QString     addOwnerWandMention(QString &requete, const QString &tableName, const QString &wand);
protected:
    long          m_position;
    QLabel       *m_StatusWidget;
    QProgressBar *m_pQProgressBar;
    void          setLastError(const QString &error);

private:
    QStringList   m_ownerList;
    QString       m_BaseLabel;
    QString       m_LastError;
    int           m_LineNum;
    QTimer       *m_pReconnectTimer;
    GUID_Mode     m_GUID_CreateMode;
    int           m_TimeToReconnect;

private slots:
    void Slot_ReconnectTimerDone();
};
#endif // C_BASECOMMON_H
