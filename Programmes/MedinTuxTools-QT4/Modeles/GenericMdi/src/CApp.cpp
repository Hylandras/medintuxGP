/*********************************************************************************
 *                                                                                *
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
//
//    mysqldump --quote-name --compatible=mysql323 CCAMTest | gzip > /home/ro/CCAMTest.gz
//    Les spectatrices ne veulent pas qu'on les ennuie dans la file.

#include "CApp.h"

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
#include <QString>
#include <QComboBox>

#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
static CApp*      G_pCApp    = 0;  // contiendra l'instance globale de l'application accessible avec     CApp::pCApp()
static C_SqlBase* G_pDb      = 0;  // contiendra l'instance globale de la base de donnee accessible avec CApp::pDb()

//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
: C_AppCore( mui_name, argc, argv )
{   //.............. creer le moteur de base de donnees ..............................
    C_SqlBase *pC_SqlBase  = new C_SqlBase(this);
    //.............. si tout est Ok on affecte les variables globales ................
    G_pDb                  =  DB_Init( pC_SqlBase );
    G_pCApp                =  this;
}

//--------------------------------------------- DB_Init-------------------------------------------------------------------
/*! \brief intialize datadabase coonnexions on .ini settings parameters.
  *  \param C_SqlBase *pC_SqlBase database to connect
  *  \return zero if error / error string if error C_SqlBase * if no error
 */
C_SqlBase *CApp::DB_Init( C_SqlBase *pC_SqlBase )
{     if (pC_SqlBase==0) return 0;
      QString    driver      = "";     // 1
      QString    schema      = "";     // 2
      QString    user        = "";     // 3
      QString    password    = "";     // 4
      QString    hostname    = "";     // 5
      QString    port        = "";     // 6
      QString    section     = applicationName() + "_Connexion";
      QString    error       = readParam( section.toLatin1(),"Parametres", &driver, &schema, &user, &password, &hostname, &port);
      if ( error.length() )
         { qDebug() << error;
           return 0;
         }
      EncodePassword_InConnectionParam(section.toLatin1(), "Parametres");
      pC_SqlBase->BaseConnect        ( "DB_"+applicationName() ,    // 0
                                       driver,             // 1
                                       schema  ,           // 2
                                       user ,              // 3
                                       password ,          // 4
                                       hostname ,          // 5
                                       port.toInt(),       // 6
                                       3600000             // toutes les heures
                                     );
      if ( pC_SqlBase->getLastError().length() )
         {     qDebug()<< "driver   : " << driver;
               qDebug()<< "schema   : " << schema;
               qDebug()<< "user     : " << user;
               qDebug()<< "password : " << password;
               qDebug()<< "hostname : " << hostname;
               qDebug()<< "port     : " << port;
               qDebug()<< "error    : " << pC_SqlBase->getLastError();
               return 0;
         }
       m_RubriquesMap = pC_SqlBase->getGlobalSettings("Definition Rubriques");
       return pC_SqlBase;
}

//--------------------------------------------- DB_ChangeConnexions-------------------------------------------------------------------
/*! \brief change database connexions if not possible, old connexion is restoring
  * if succes a Sign_DataBaseConnexionChanged(_driver, _schema, _user, password, _hostname, _port) is emited.
  *  \param  const QString &driver driver name QDB2 QIBASE QMYSQL QOCI QODBC QPSQL QSQLITE QSQLITE2 QTDS
  *  \param  const QString &schema schema database name to coonect
  *  \param  const QString &user connexion user name
  *  \param  const QString &_password password : encoded (begin by '#')  or not encoded (don't begin by '#')
  *  \param  const QString &hostname IP adress or domain name
  *  \param  const QString &port port on wich connexion must be opened
  *  \return true in all is ok otherwise false
 */
bool CApp::DB_ChangeConnexions( const QString &_driver , const QString &_schema, const QString &_user, const QString &_password ,const QString &_hostname ,const QString &_port )
{   if ( pCDb()==0 ) return false;
    QString    driver      = "";     // 1
    QString    schema      = "";     // 2
    QString    user        = "";     // 3
    QString    password    = "";     // 4
    QString    hostname    = "";     // 5
    QString    port        = "";     // 6
    QString    section     = applicationName() + "_Connexion";
    QString    error       = readParam( section.toLatin1(),"Parametres", &driver, &schema, &user, &password, &hostname, &port);
    if ( error.length() )
       { qDebug() << error;
         return 0;
       }
    EncodePassword_InConnectionParam(section.toLatin1(), "Parametres");

    if ( pCDb()->changeDataBaseConnexion(_driver, _schema, _user, _password, _hostname, _port) )
       { password = _password;
         if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
         password = CGestIni::PassWordEncode(password);
         password.prepend('#');
         writeParam( section.toLatin1(),"Parametres", _driver.toLatin1(), _schema.toLatin1(), _user.toLatin1(), password.toLatin1(), _hostname.toLatin1(), _port.toLatin1());
         m_RubriquesMap = pCDb()->getGlobalSettings("Definition Rubriques");
         emit Sign_DataBaseConnexionChanged(_driver, _schema, _user, password, _hostname, _port);
         return true;
       }
    //.................. si echec afficher erreur ...............................
    if ( pCDb()->getLastError().length() )
       {     qDebug()<< "ERROR when trying to change new connections ";
             qDebug()<< "driver   : " << driver;
             qDebug()<< "dataBase : " << _schema;
             qDebug()<< "user     : " << user;
             qDebug()<< "password : " << password;
             qDebug()<< "hostname : " << hostname;
             qDebug()<< "port     : " << port;
             qDebug()<< "error    : " << pCDb()->getLastError();
       }
    //.................. si echec remettre l'ancienne connexion ...............................
    pCDb()->changeDataBaseConnexion(driver, schema, user, password, hostname, port);
    if (pCDb()->getLastError().length() )
       {     qDebug()<< "ERROR when trying to restore previous connections ";
             qDebug()<< "driver   : " << driver;
             qDebug()<< "dataBase : " << schema;
             qDebug()<< "user     : " << user;
             qDebug()<< "password : " << password;
             qDebug()<< "hostname : " << hostname;
             qDebug()<< "port     : " << port;
             qDebug()<< "error    : " << pCDb()->getLastError();
         }
       return false;
}

//--------------------------------------------- pCApp [static]-------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
 * l'acces ulterieur aux fonctions de la classe CApp se fait par CApp::pCApp()
 */
CApp* CApp::pCApp()
{ return G_pCApp;
}

//--------------------------------------------- pCDb [static]-------------------------------------------------------------------
/*! \brief retourne l'instance de la base de donnees
 */
C_SqlBase* CApp::pCDb()
{ return G_pDb;
}
