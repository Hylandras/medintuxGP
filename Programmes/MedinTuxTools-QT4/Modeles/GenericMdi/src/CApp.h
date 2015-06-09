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
#ifndef C_APP_H
#define C_APP_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QTextEdit>
#include <QMap>

#include "../../MedinTuxTools-QT4/C_AppCore.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#include "C_SqlBase.h"

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    if fatal error in class initialisation occured  CApp::pCApp()==0
 *    so all publics functions will be accessibles by CApp::pCApp()->publicFonctions().
 */

class CApp : public C_AppCore
{
   Q_OBJECT
public:
     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();
     //--------------------------------------------- DB_Init-------------------------------------------------------------------
     /*! \brief intialize datadabase coonnexions on .ini settings parameters.
       *  \param C_SqlBase *pC_SqlBase database to connect
       *  \return zero if error / error string if error C_SqlBase * if no error
      */
     C_SqlBase *DB_Init( C_SqlBase *pC_SqlBase );

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
     bool DB_ChangeConnexions(const QString &_driver , const QString &_schema, const QString &_user, const QString &_password , const QString &_hostname , const QString &_port );

     //--------------------------------------------- getRubriquesMap-------------------------------------------------------------------
     /*! \brief return rubriques Map form [RubName]
       *  \return true in all is ok otherwise false
      */
     QMap <QString,QString> getRubriquesMap(){ return m_RubriquesMap;}

     static CApp         *pCApp();                                  /*!< retourne l'instance de l'application          */
     static C_SqlBase    *pCDb();                                   /*!< retourne l'instance de la base de donnee      */
private :
     QMap <QString,QString> m_RubriquesMap;
signals:
    void Sign_DataBaseConnexionChanged( const QString &driver, const QString &schema, const QString &user, const QString &password, const QString &hostname, const QString &port);

};

#endif

