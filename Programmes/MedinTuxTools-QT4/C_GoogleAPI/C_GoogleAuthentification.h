/*********************** C_GoogleAuthentification.h ***********************
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

#ifndef C_GOOGLE_AUTH_H
#define C_GOOGLE_AUTH_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
//============================== C_GoogleAuthentification ===================================
/*!
This class represents the authentication service necessary for accessing the
other google services. For this class to work a SSL service is required since
this feature is not compiled by default in Qt. OpenSSL is a good option for
this and a precompiled version for Windows can be found here:
http://www.openssl.org/related/binaries.html
The necessary documentation for using this service can be found at Google:
http://code.google.com/intl/en/apis/gdata/docs/auth/overview.html#ClientLogin
*/
class C_GoogleAuthentification : public QObject
{
  Q_OBJECT

  public:
    C_GoogleAuthentification(QNetworkAccessManager *netAccMan, const QString &service);
    virtual ~C_GoogleAuthentification();

    virtual bool google_login(const QString &mail, const QString &password);

    QString getError();
    QString getAuth();
    QString getLsid();
    QString getSid();

  private slots:
    void Slot_loginFinished();

  private:
    QString                m_service;
    QString                m_error;
    QString                m_auth;
    QString                m_lsid;
    QString                m_sid;
    QNetworkAccessManager *m_netAccMan;
    QNetworkReply         *m_reply;
    QEventLoop            *m_loop;

};

#endif
