/***************************************************************************
 *   Copyright (C) 2009 by Docteur Pierre-Marie Desombre.   *
 *   pm.desombre@medsyn.fr  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CONNEXION_H
#define CONNEXION_H

#include <QtCore>
#include <QtSql>
#include <QSqlDatabase>
#include <QSettings>
#include <QMessageBox>

#include "CApp.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"

//lance la connection a DrTuxTest
bool CreateConnection() 
{   QString baseLabel = "DB_FOR_APITUX";
    QString hostname;
    QString driver;
    QString user;
    QString password;
    QString port = "3306" ;
    QString file = CApp::pCApp()->pathAppli()+"Ressources/apitux.ini";
    QFile fichier(file);
    if(!fichier.open(QIODevice::ReadOnly | QIODevice::Text)){
    QMessageBox::warning(0,"erreur", QObject::trUtf8("le fichier %1 n'est pas accessible").arg(file),QMessageBox::Ok);}
    QSettings set(file,QSettings::IniFormat);

    driver      = set.value("connexion/driver").toString().trimmed();
    hostname    = set.value("connexion/host").toString().trimmed();
    user        = set.value("connexion/utilisateur").toString().trimmed();
    password    = set.value("connexion/motdepassemysql").toString().trimmed();
    port        = set.value("connexion/port").toString().trimmed();
    if(port.isEmpty()) port = "3306";

    if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
    QSqlDatabase::removeDatabase (baseLabel);
    QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, baseLabel);
    defaultDB.setDatabaseName( "DrTuxTest" );
    defaultDB.setUserName    ( user     );
    defaultDB.setPassword    ( password );
    defaultDB.setHostName    ( hostname );
    defaultDB.setPort        ( port.toInt() );

    if ( ! defaultDB.open() ) {
            QMessageBox::critical(0,QString("Erreur ouverture de base : %1<br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>host</b>   : %2 <br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>driver</b> : %3 <br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>user</b>   : %4 <br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>port</b>   : %5<br/> "
                                            "----------------").arg("DrTuxTest",hostname,driver,user,port), defaultDB.lastError().text());
	    return false;
        }
  return true;
}

#endif
