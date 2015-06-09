/********************************* C_GoogleEvent.h ************************
* #include "C_GoogleEvent.h"           QT4-ready                          *
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


#ifndef C_GOOGLE_EVENT_H
#define C_GOOGLE_EVENT_H
#include <QList>
#include <QDate>
class QString;

//================================  C_GoogleEvent ==============================
/*!
This class represent a single all-day-event.
*/
class C_GoogleEvent  // : public QObject
{
  public:
    C_GoogleEvent();
    C_GoogleEvent(const QString &title                ,
                  const QString &dateDeb              ,
                  const QString &dateEnd              ,
                  const QString &content  = ""        ,
                  const QString &where    = ""        ,
                  const QString &color    = "#5779db" ,
                  const QString &phone    = ""        ,
                  const QString &etag     = ""        ,
                  const QString &event_ID = ""
                 );
    C_GoogleEvent(const QString   &title                ,
                  const QDateTime &dateDeb              ,
                  const QDateTime &dateEnd              ,
                  const QString   &content  = ""        ,
                  const QString   &where    = ""        ,
                  const QString   &phone    = ""        ,
                  const QString   &color    = "#5779db" ,
                  const QString   &etag     = ""        ,
                  const QString   &event_ID = ""
                 );
    C_GoogleEvent(const C_GoogleEvent &other);
    virtual ~C_GoogleEvent();

    QString     getTitle();
    void        setTitle(const QString &title);
    QString     getContent();
    void        setContent(const QString &content);
    QString     getEvent_ID();
    void        setEvent_ID(const QString &id);
    QString     getEtag();
    void        setEtag(const QString &etag);
    QString     getWhere();
    void        setWhere(const QString &where);
    QString     getColor();
    void        setColor(const QString &color);
    void        setPhone(const QString &phone);
    QString     getPhone();
    QDateTime   getDateDeb();
    void        setDateDeb(const QDateTime &dateDeb);
    void        setDateDeb(const QString   &dateDeb);
    QDateTime   getDateEnd();
    void        setDateEnd(const QDateTime &dateEnd);
    void        setDateEnd(const QString   &dateEnd);
    QString serialize();
    C_GoogleEvent & operator = (const C_GoogleEvent &other);

  private:
    QDateTime   m_dateDeb;
    QDateTime   m_dateEnd;
    QString     m_content;
    QString     m_title;
    QString     m_event_ID;
    QString     m_etag;
    QString     m_where;
    QString     m_color;
    QString     m_phone;
};

//! A type definition for a list of events
typedef QList<C_GoogleEvent> C_GoogleEventList;

#endif
