/********************************* C_App.cpp **************************************
 *  #include "C_App.h"             QT4-ready                                      *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a  l'Energie Atomique                                           *
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
 #include "C_App.h"
 #include <QByteArray>
 #include <QSqlQuery>
 #include <QSqlDatabase>
 #include <QUuid>
 #include <QSqlError>
 #include <QMessageBox>
 #include <QFileInfo>
 #include <QIcon>
 #include "../../MedinTuxTools-QT4/CGestIni.h"
 #include "../../MedinTuxTools-QT4/Theme/Theme.h"
  #include <QDebug>

#define   DUMMY_TEST_SQL_COMMAND   "SELECT 1"
#define   DB_TIME_TO_RECONNECT     3600000  //reconnexion toutes les heures
#define   TR                       QObject::tr
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++

C_App* G_pCApp = 0;  // contiendra l'instance globale de l'application
static char NUM_VERSION[]     = "==##@@==2.16.000==@@##==";

//--------------------------------------------- C_App -------------------------------------------------------------------
C_App::C_App( int & argc, char ** argv)
        : QApplication(argc, argv)
    {//QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
     m_NUM_VERSION     = NUM_VERSION;
     QString driver;             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
     QString dataBaseToConnect;  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
     QString user;               // = "root"
     QString password;           // = ""
     QString hostname;           // = "localhost"
     QString port;               // = "3306"
     QString errMess        = "";
     m_Start_ID             = "";
     m_Droits               = "";
     m_pReconnectTimer      = 0;
     if (argc>3) m_Start_ID = argv[3];
     if (argc>4) m_Droits   = argv[4];
     //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............     
     m_PathAppli   = CGestIni::Construct_PathBin_Module("personnes", QFileInfo (argv[0]).canonicalPath());
     QString    fn = CGestIni::Construct_Name_File_Ini("personnes",  QFileInfo (argv[0]).canonicalPath(),"");
     m_LocalParam  = CGestIni::Param_UpdateFromDisk(fn);
     if (CGestIni::Param_ReadParam(  m_LocalParam.toAscii(), "Connexion", "Parametres", &driver, &dataBaseToConnect, &user, &password, &hostname, &port) !=0 )  // zero = pas d'erreur
        {   driver            = "QODBC3";                   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
            dataBaseToConnect = "DataSrcODBC-MySQL-DrTux";  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
            user              = "root";                     // = "root"
            password          = "";                         // = ""
            hostname          = "localhost";                // = "localhost"
            port              = "3306";
        }
      m_IsValid         = BaseConnect( driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                       dataBaseToConnect,  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                                       user,          // = "root"
                                       password,      // = ""
                                       hostname,      // = "localhost"
                                       port,
                                       &errMess,
                                       "PERSONNES_BD");
     if (m_IsValid ==0)
       {    QMessageBox::critical (0,   "Personnes" ,
                                         QObject::tr("Database cannot instanciate")+ "\r\n " + errMess ,
                                         QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
            return;
       }
     //................. fichier de config des bases.......................................
     fn        = CGestIni::Construct_Name_File_Ini("personnes", QFileInfo (argv[0]).canonicalPath(),"DataBase.cfg");
     QByteArray ba;
     CGestIni::Param_UpdateFromDisk(fn, ba);
     m_IsValid = initValues( ba, &errMess);
     if (m_IsValid ==0)
       {    QMessageBox::critical (0,   "Personnes" ,
                                         QObject::tr("Error in configuration file 'DataBase.cfg'")+"\r\n " + errMess ,
                                         QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
            return;
       }
    m_pReconnectTimer = new QTimer (this);
    if (m_pReconnectTimer)
       {m_pReconnectTimer->setInterval(DB_TIME_TO_RECONNECT);
        m_pReconnectTimer->start();
        connect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(Slot_ReconnectTimerDone()) );
       }
    //.............................. initialiser le theme ..........................................
    m_PathTheme     =  "../../Themes/Default/";
    if (CGestIni::Param_ReadParam( m_LocalParam.toAscii(), "Theme", "Path", &m_PathTheme) != QString::null )  // zero = pas d'erreur
       { m_PathTheme     =  "../../Themes/Default/";                                   // valeur par défaut si pas de theme explicite
       }
    if (!QDir(m_PathTheme).exists())     m_PathTheme     =  "../../Themes/Default/";        // valeur par défaut
    if ( QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
    m_PathTheme = QDir::cleanPath(m_PathTheme) + "/";
    Theme::setPath(m_PathTheme);

    G_pCApp->setWindowIcon (QIcon ( m_PathAppli + "/Ressources/personnes.png") );
    G_pCApp = this;
}
//-----------------------------------------------------  ~C_App -------------------------------------------
C_App::~C_App()
{killBase();
}

//-----------------------------------------------------  getBDVersionNumber -------------------------------------------
QString C_App::getBDVersionNumber()
{return m_VERSION_NUMBER;
}
//-----------------------------------------------------  killBase -------------------------------------------
void C_App::killBase()
{if (m_pReconnectTimer)
    {m_pReconnectTimer->stop();
     disconnect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(Slot_ReconnectTimerDone()) );
    }
 QSqlDatabase::database( m_BaseLabel ).close();
 QSqlDatabase::removeDatabase (m_BaseLabel);
}

//-----------------------------------------------------  getDatabase -------------------------------------------
QSqlDatabase C_App::getDatabase()
{return QSqlDatabase::database(m_BaseLabel);
}

//-----------------------------------------------------  Slot_ReconnectTimerDone -------------------------------------------
void C_App::Slot_ReconnectTimerDone()
{QSqlQuery query(QString::null , QSqlDatabase::database(m_BaseLabel) );
 query.exec(DUMMY_TEST_SQL_COMMAND);
}
//-----------------------------------------------------  BaseConnect -------------------------------------------
int C_App::BaseConnect(const QString & driver,              // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                       const QString & dataBaseToConnect,   // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                       const QString & user,                // = "root"
                       const QString & password,            // = ""
                       const QString & hostname,            // = "localhost"
                       const QString & port,
                       QString       * errMess,
                       QString         baseLabel)
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
            qstr += TR("Failed to open database : ")    + baseLabel + " Driver : " +  QString(driver) + "  " + QString(dataBaseToConnect) + "\r\n" +
                     defaultDB.lastError().driverText() + "\r\n" + defaultDB.lastError().databaseText();
            qWarning() << qstr.toLatin1();
            if (errMess) *errMess += qstr;
            return 0;
           }
        return 1;
    }

//-----------------------------------------------------  OpenBase -------------------------------------------
int   C_App::OpenBase()
{return 1;
}

//-----------------------------------------------------  CloseBase -------------------------------------------
void   C_App::CloseBase()
{
}

//--------------------------------------------- IsThisDroitExist ----------------------------------------------
/*! \brief Verifie si le droit : droitToFind  (trois caracteres style med adm sgn ....) existe dans la chaine de droits : listDroits 6 fois plus rapide que : listDroits.find(droitToFind) != -1
*/
bool C_App::IsThisDroitExist(const char *droitToFind)
{return IsThisDroitExist(m_Droits.toAscii(), droitToFind);
}

//--------------------------------------------- IsThisDroitExist ----------------------------------------------
/*! \brief Verifie si le droit : droitToFind  (trois caracteres style med adm sgn ....) existe dans la chaine de droits : listDroits 6 fois plus rapide que : listDroits.find(droitToFind) != -1
*/
bool C_App::IsThisDroitExist(const char *listDroits, const char *droitToFind)
{char *pt  = (char*)listDroits;
 while (*pt)
    {if (pt[1]==droitToFind[0] && pt[2]==droitToFind[1] &&pt[3]==droitToFind[2]) return TRUE;
     pt += 4;
    }
 return FALSE;
}
//-----------------------------------------------------  initValues -------------------------------------------
int C_App::initValues(QString &data, QString *errMess)
{ if (data.length()==0) {if (errMess) (*errMess) +=  TR("\r\n Database configuration file not found or empty "); return 0;}
  return initValues(data.toAscii(),errMess);
}
 //-----------------------------------------------------  initValues -------------------------------------------
int C_App::initValues(const char *pt_in, QString *errMess)
{
  QString   err = "";
  char      *pt = (char*) pt_in;   // n'est qu'en lecture
  int      line = 0;
  //.............................. NUMERO DE VERSION DE LA BASE .........................................................
  pt = SetConfBase_SetProperties(pt,  m_VERSION_NUMBER,                "m_VERSION_NUMBER",                       &line , err); if (err.length())     goto SetConfBase_Error;
  //.............................. DOSS_INDEX_TBL_NAME .........................................................
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TBL_NAME,         "m_INTERVENANTS_TBL_NAME",                &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_GUID,             "m_INTERVENANTS_GUID",                    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_LOGIN_SIGEMS,     "m_INTERVENANTS_LOGIN_SIGEMS",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_LOGIN,            "m_INTERVENANTS_LOGIN",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_PASSWORD_SIGEMS,  "m_INTERVENANTS_PASSWORD_SIGEMS",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_PASSWORD,         "m_INTERVENANTS_PASSWORD",                &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_DROITS,           "m_INTERVENANTS_DROITS",                  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_CODE_SPEC,        "m_INTERVENANTS_CODE_SPEC",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_NUM_ORDRE,        "m_INTERVENANTS_NUM_ORDRE",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_NOM,              "m_INTERVENANTS_NOM",                     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_PRENOM,           "m_INTERVENANTS_PRENOM",                  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_ADRESSE,          "m_INTERVENANTS_ADRESSE",                 &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_CONVENTION,       "m_INTERVENANTS_CONVENTION",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_CODE_POST,        "m_INTERVENANTS_CODE_POST",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_VILLE,            "m_INTERVENANTS_VILLE",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_1,            "m_INTERVENANTS_TEL_1",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_2,            "m_INTERVENANTS_TEL_2",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_3,            "m_INTERVENANTS_TEL_3",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_EMAIL,            "m_INTERVENANTS_EMAIL",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_PK,               "m_INTERVENANTS_PK",                      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_NOTE,             "m_INTERVENANTS_NOTE",                    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_QUALITE,          "m_INTERVENANTS_QUALITE",                 &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_CODE_REGIME,      "m_INTERVENANTS_CODE_REGIME",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_SEXE,             "m_INTERVENANTS_SEXE",                    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TITRE,            "m_INTERVENANTS_TITRE",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_CHER,             "m_INTERVENANTS_CHER",                    &line , err); if (err.length())     goto SetConfBase_Error;           // rang gemellaire
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_TYPE_1,       "m_INTERVENANTS_TEL_TYPE_1",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_TYPE_2,       "m_INTERVENANTS_TEL_TYPE_2",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_TYPE_3,       "m_INTERVENANTS_TEL_TYPE_3",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_ABR_1,        "m_INTERVENANTS_TEL_ABR_1",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_ABR_2,        "m_INTERVENANTS_TEL_ABR_2",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_TEL_ABR_3,        "m_INTERVENANTS_TEL_ABR_3",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_INTERVENANTS_COULEUR ,         "m_INTERVENANTS_COULEUR",                 &line , err); if (err.length())     goto SetConfBase_Error;              // couleur associee a cet intervenant

  pt = SetConfBase_SetProperties(pt,  m_CODEPOST_TBL_NAME,             "m_CODEPOST_TBL_NAME",                    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_CODEPOST_PK,                   "m_CODEPOST_PK",                          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_CODEPOST_CODE_POST,            "m_CODEPOST_CODE_POST",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_CODEPOST_VILLE,                "m_CODEPOST_VILLE",                       &line , err); if (err.length())     goto SetConfBase_Error;

  return 1;

SetConfBase_Error:
  if (errMess) *errMess += err;
  return 0;

}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *C_App::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
{int nLine      = *line;
 err            = "";
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
                 {propertie=QString::fromLatin1 ( pt, end-pt);
                  if (line) *line = nLine;
                  return end +1;
                 }
              else
                {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not found at line #: ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not found at line #: ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)

     NEXT_LINE(pt);
     ++nLine;
    }
 err  = TR("\r\nSyntax error : C_DBVarDrTux::SetConfBase()"
           "\r\ndatabase configuration file 'DataBase.cfg'"
           "\r\nhas following errors: \r\n");
 err += TR("      ==> mention %1 is not found at line # N�: %2").arg(token, QString::number(nLine));
 if (line) *line = nLine;
 return pt;
}

//-----------------------------------------------------  GUID_Create --------------------------------------------------
QString C_App::GUID_Create()
{ QString guid ( QUuid::createUuid ().toString());
   return  guid.mid(1, 36);
}

//-------------------------------------------------- OutSQL_error -----------------------------------------------
QString C_App::OutSQL_error(const QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/)
{   return OutSQL_error(cur.lastError(), messFunc, requete );
}

//-------------------------------------------------- OutSQL_error -----------------------------------------------
QString C_App::OutSQL_error(const QSqlError &error, const char *messFunc /*=0*/, const char *requete /*=0*/)
{    QString   qserr("");

     if (error.type() != QSqlError::NoError)
        {switch (error.type())
           { case    QSqlError::NoError:             qserr = TR(" - SQL no error occurred: ")          ; break;
             case    QSqlError::ConnectionError:     qserr = TR(" - SQL connection error: ")           ; break;
             case    QSqlError::StatementError:      qserr = TR(" - SQL statement syntax error: ")     ; break;
             case    QSqlError::TransactionError:    qserr = TR(" - SQL transaction failed error: ")   ; break;
             case    QSqlError::UnknownError:        qserr = TR(" - SQL unknown error: ")              ; break;
             default:                                qserr = TR(" - unknown SQL type error: ")         ; break;
           }
         if (messFunc) qDebug()<<messFunc;
         qDebug() << qserr.append(error.databaseText()).toLatin1();
         if (requete) qDebug()<<requete;
        }
     return error.databaseText();
}


//************************************************************************************************************************
//=================================== GESTION CODE POSTAL ================================================================
//************************************************************************************************************************
//-----------------------------------------------------  GetVilleFromCodePostal -------------------------------------------
QString C_App::GetVilleFromCodePostal(const QString &codePostal)
{ QString result  ("");
  QString requete = QString("SELECT %1 FROM %2 WHERE %3 = %4").arg(m_CODEPOST_VILLE, m_CODEPOST_TBL_NAME , m_CODEPOST_CODE_POST, codePostal);
  if (OpenBase()==0)   return result;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
     {result = Utf8_Query(query, 0);
     }
  CloseBase();
  return result;
}

//-----------------------------------------------------  GetCodePostalFromVille -------------------------------------------
QString C_App::GetCodePostalFromVille(const QString &ville)
{ QString result  ("");
  QString requete = QString("SELECT %1 FROM %2 WHERE ville = '%3'").arg(m_CODEPOST_CODE_POST, m_CODEPOST_TBL_NAME, m_CODEPOST_VILLE, ville);
  if (OpenBase()==0)   return result;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive() && query.next())
     {result = Utf8_Query(query,0);
     }
  CloseBase();
  return result;
}

//-----------------------------------------------------  GetVilleFromCodePostal -------------------------------------------
void C_App::GetVilleFromCodePostal(const QString &codePostal, QStringList &villeList)
{ QString cdPost (codePostal);
  if (cdPost[0]=='0') cdPost.remove(0,1);
  villeList.clear();
  QString requete = QString("SELECT %1 FROM %2 WHERE %3 = %4").arg(m_CODEPOST_VILLE, m_CODEPOST_TBL_NAME, m_CODEPOST_CODE_POST, cdPost);
  if (OpenBase()==0)   return ;
  QSqlQuery query(requete , QSqlDatabase::database(m_BaseLabel) );
  if (query.isActive())
     {while(query.next()) villeList.append( Utf8_Query(query,0));
     }
  CloseBase();
  return;
}

//-----------------------------------------------------  GetCodePostalFromVille -------------------------------------------
void C_App::GetCodePostalFromVille(const QString &ville, QStringList &codePostalList)
{ codePostalList.clear();
  QString requete = QString("SELECT %1 , %2 FROM %3 WHERE %4  LIKE '%5'").arg(m_CODEPOST_CODE_POST, m_CODEPOST_VILLE, m_CODEPOST_TBL_NAME, m_CODEPOST_VILLE, ville+"%");
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
//--------------------------------- Utf8_Query ------------------------------------------------------------
QString C_App::Utf8_Query(QSqlQuery &cur, int field)
{QByteArray ba  = cur.value(field).toByteArray ();
 char *ptr      = ba.data();
 if (CGestIni::IsUtf8( ptr ))
    {return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }
}

