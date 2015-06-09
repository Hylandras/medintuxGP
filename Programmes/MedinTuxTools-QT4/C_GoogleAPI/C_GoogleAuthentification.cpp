/*********************** C_GoogleAuthentification.cpp *********************
* #include "C_GoogleAuth.h"           QT4-ready                           *
* .                                                                       *
*                                                                         *
* This file is part of CalendarGadget Copyright (C) 2010-2011 B.D. Mihai  *
* modified for MedinTux                                                   *
* Many thanks for  B.D. Mihai (CalendarGadget) to help me to understand   *
* Google API                                                              *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QEventLoop>
#include <QUrl>
#include <QCoreApplication>
#include <QList>

#include "C_GoogleAuthentification.h"

//------------------------------------ C_GoogleAuthentification ----------------------------------
/*!
Create a new instance of the C_GoogleAuthentification using a network access manager.
*/
C_GoogleAuthentification::C_GoogleAuthentification(QNetworkAccessManager *netAccMan, const QString &service)
{
  this->m_netAccMan = netAccMan;
  this->m_service   = service;
  m_error           = "";
  m_auth            = "";
  m_lsid            = "";
  m_sid             = "";
  m_loop            = new QEventLoop();
}

//------------------------------------ C_GoogleAuthentification ----------------------------------
/*!
Clean up.
*/
C_GoogleAuthentification::~C_GoogleAuthentification()
{delete m_loop;
}

//------------------------------------ google_login ----------------------------------------------
/*!
This function try to login into the Google account and obtain the authentication
code. The function will block until the request is finalized.
\param mail the full mail address.
\param password the password for the provided mail account.
\return true if the login was performed successfully.
*/
bool C_GoogleAuthentification::google_login(const QString& mail, const QString& password)
{
  QUrl address("https://www.google.com/accounts/ClientLogin");

  QNetworkRequest request = QNetworkRequest();
  request.setUrl(address);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QByteArray data;
  data += "accountType=HOSTED_OR_GOOGLE";
  data += "&Email="   + mail;
  data += "&Passwd="  + password;
  data += "&service=" + m_service;
  data += "&source="  + QCoreApplication::organizationName() + "-" +
                        QCoreApplication::applicationName()  + "-" +
                        QCoreApplication::applicationVersion();

  m_reply = m_netAccMan->post(request, data);
  connect(m_reply, SIGNAL(finished()), this, SLOT(Slot_loginFinished()));
  m_loop->exec();
  return m_error.isEmpty();
}

//------------------------------------ getError --------------------------------------------------
/*!
Get the error text if it is the case.
*/
QString C_GoogleAuthentification::getError()
{ return m_error;
}

//------------------------------------ getAuth ---------------------------------------------------
/*!
Get the authentication code.
*/
QString C_GoogleAuthentification::getAuth()
{ return m_auth;
}

//------------------------------------ getLsid ---------------------------------------------------
QString C_GoogleAuthentification::getLsid()
{ return m_lsid;
}

//------------------------------------ getSid ----------------------------------------------------
QString C_GoogleAuthentification::getSid()
{ return m_sid;
}

//------------------------------------ Slot_loginFinished ----------------------------------------
/*!
This function handle the response of the google authentication server.
*/
void C_GoogleAuthentification::Slot_loginFinished()
{
  if (!m_reply->error())
     {
       QList<QByteArray> response = m_reply->readAll().split('\n');
       m_error = "";
       for (int i = 0; i < response.size(); ++i)
           { const QByteArray& responseToken = response.at(i);
             if (responseToken.startsWith("SID="))
                  m_sid = responseToken.mid(4);
             else if (responseToken.startsWith("LSID="))
                  m_lsid = responseToken.mid(5);
             else if (responseToken.startsWith("Auth="))
                  m_auth = responseToken.mid(5);
             else if (responseToken.startsWith("Error="))
                  m_error = responseToken.mid(6);
           } //end for (int i = 0; i < response.size(); ++i)
     }
  else
     {
       m_error = m_reply->errorString();
       m_sid   = "";
       m_lsid  = "";
       m_auth  = "";
     }
  m_reply->deleteLater();
  m_reply = 0;
  m_loop->exit();
}
