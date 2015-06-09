/********************************* CSetupBase.cpp *********************************
 *                                                                                *
 *   #include "CSetupBase.cpp"                                                    *
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

#include <qapplication.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include "CSetupBase.h"
#include "../../MedinTuxTools/CGestIni.h"

//-----------------------------------------------------  CSetupBase -------------------------------------------------

CSetupBase::CSetupBase()
{
}

//-----------------------------------------------------  ~CMedicaBase -------------------------------------------
CSetupBase::~CSetupBase()
{
}

QSqlDatabase*  CSetupBase::BaseConnect(const QString & driver,              // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                       const QString & DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                       const QString & user,          // = "root"
                                       const QString & pasword,       // = ""
                                       const QString & hostname,      // = "localhost"
                                       const QString & port,
                                       const char* nameBase,
                                       QString*    errMess,
                                       int*        errNum)
    {
        //QSqlDatabase::removeDatabase (nameBase);
        QSqlDatabase *defaultDB = 0;
        defaultDB = QSqlDatabase::addDatabase( driver , nameBase);
        qDebug(QString(driver) + "  " + nameBase);
        if (defaultDB<=0) return 0;
        defaultDB->setDatabaseName( DataBaseName );
        defaultDB->setUserName( user );
        defaultDB->setPassword( pasword );
        defaultDB->setHostName( hostname);
        defaultDB->setPort( port.toInt());
        if ( ! defaultDB->open() )
           {QString qstr = "";
            int valInt = defaultDB->lastError().number();
            if (errNum) *errNum = valInt;
            qstr += "Failed to open database: " + QString(driver) + "  " + QString(DataBaseName) + "\r\n" +
                    defaultDB->lastError().driverText() + "\r\n" + defaultDB->lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
            return 0;
           }
        return defaultDB;
    }


//---------------------------------------------------- CreationBase -----------------------------------------------------------
int CSetupBase::CreationBase(QSqlDatabase* mysqlbase, QString &nameBase, QString *errMess/*=0*/, int *errNum /*=0*/,
                             const QString &driver, const QString &user, const QString &passWord, const QString &hostName,  const QString &port)
{

    #ifdef Q_WS_X11

    #else

        //.................... verifie si la table existe ..................................................
        //                     retourne zero si c'est le cas
        QSqlDatabase *defaultDB = 0;
        defaultDB = QSqlDatabase::addDatabase( driver , "PipoBaseConnexionName");
        qDebug(QString(driver) + "  " + nameBase);
        if (defaultDB<=0) return 0;
        defaultDB->setDatabaseName( nameBase );
        defaultDB->setUserName( user );
        defaultDB->setPassword( passWord );
        defaultDB->setHostName( hostName);
        defaultDB->setPort( port.toInt());
        if (defaultDB>0 )
           { if (defaultDB->open())
                {defaultDB->close();
                 QSqlDatabase::removeDatabase(defaultDB);
                 return 0;                                            // elle existe déjà on se casse avec zero pour le signaler
                }
           }
        QSqlDatabase::removeDatabase(defaultDB);
    #endif
    //.................... création de la base de données ..................................................
    QString requete = "";
    requete = "CREATE DATABASE "               + nameBase + ";";

    if (!mysqlbase->isOpen()) mysqlbase->open();
    QSqlQuery query(QString::null, mysqlbase);

    if (query.exec(requete)==FALSE)
       {    QString qstr = "";
            int valInt = mysqlbase->lastError().number();
            if (errNum) *errNum = valInt;
            qstr += "Failed to create database: " + QString(nameBase) + "\r\n" +
                    mysqlbase->lastError().driverText() + "\r\n" + mysqlbase->lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
            mysqlbase->close();
            return 0;                        // Là ça craint
       }
    mysqlbase->close();
    return 1;
}

//---------------------------------------------------- DropBase -----------------------------------------------------------
int CSetupBase::DropBase(QSqlDatabase* mysqlbase, QString &nomBase, QString *errMess/*=0*/, int *errNum /*=0*/)
{
    if (!mysqlbase->isOpen()) mysqlbase->open();
    QString requete = "";
    QSqlQuery query(QString::null, mysqlbase);
    requete = "DROP DATABASE " + nomBase + ";";
    if (query.exec(requete)==FALSE)
       {    QString qstr = "";
            int valInt = mysqlbase->lastError().number();
            if (errNum) *errNum = valInt;
            qstr += "Failed to drop database: " + QString(nomBase) + "\r\n" +
                    mysqlbase->lastError().driverText() + "\r\n" + mysqlbase->lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
	    mysqlbase->close();
            return 0;
       }
    mysqlbase->close();
    return 1;
}
