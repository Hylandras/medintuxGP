/********************************* C_GoogleEvent.cpp *********************
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

#include "C_GoogleEvent.h"
//------------------------------ C_GoogleEvent -----------------------------------------
/*!
Create a new instance of the Event class.
*/
C_GoogleEvent::C_GoogleEvent( const QString &title                     ,
                              const QString &dateDeb                   ,
                              const QString &dateEnd                   ,
                              const QString &content   /* = ""*/       ,
                              const QString &where     /* ="" */       ,
                              const QString &color     /* ="#5779db"*/ ,
                              const QString &phone     /* =""*/        ,
                              const QString &etag      /* =""*/        ,
                              const QString &event_ID  /* =""*/
                            )
{setDateDeb(dateDeb);
 setDateEnd(dateEnd);
 m_title    = title;
 m_content  = content;
 m_where    = where;
 m_color    = color;
 m_etag     = etag;
 m_event_ID = event_ID;
 m_phone    = phone;
}

//------------------------------ C_GoogleEvent -----------------------------------------
/*!
Create a new instance of the Event class.
*/
C_GoogleEvent::C_GoogleEvent( const QString   &title                     ,
                              const QDateTime &dateDeb                   ,
                              const QDateTime &dateEnd                   ,
                              const QString   &content   /* = ""*/       ,
                              const QString   &where     /* ="" */       ,
                              const QString   &color     /* ="#5779db"*/ ,
                              const QString   &phone     /* =""*/        ,
                              const QString   &etag      /* =""*/        ,
                              const QString   &event_ID  /* =""*/
                            )
{setDateDeb(dateDeb);
 setDateEnd(dateEnd);
 m_title    = title;
 m_content  = content;
 m_where    = where;
 m_color    = color;
 m_etag     = etag;
 m_event_ID = event_ID;
 m_phone    = phone;
}

//------------------------------ C_GoogleEvent -----------------------------------------
/*!
Create a new instance of the Event class.
*/
C_GoogleEvent::C_GoogleEvent()
{ m_title    = "";
  m_content  = "";
  m_dateDeb  = QDateTime::currentDateTime();
  m_dateEnd  = QDateTime::currentDateTime();
  m_event_ID = "";
  m_etag     = "";
  m_phone    = "";
}

//------------------------------ C_GoogleEvent -----------------------------------------
/*!
Create a new instance of the Event class using another Event.
*/
C_GoogleEvent::C_GoogleEvent(const C_GoogleEvent &other)
{(*this) = other;
}

//------------------------------ ~C_GoogleEvent -----------------------------------------
/*!
Clean up.
*/
C_GoogleEvent::~C_GoogleEvent()
{
}
//------------------------------ getContent -----------------------------------------
/*!
Get the event content.
*/
QString C_GoogleEvent::getContent()
{return m_content;
}

//------------------------------ setContent -----------------------------------------
/*!
set the event content.
*/
void  C_GoogleEvent::setContent(const QString &content)
{m_content = content;
}
//------------------------------ getEtag -----------------------------------------
/*!
Get the event Etag.
*/
QString C_GoogleEvent::getEtag()
{return m_etag;
}
//------------------------------ setPhone -----------------------------------------
/*!
set the event phone.
*/
void  C_GoogleEvent::setPhone(const QString &phone)
{m_phone = phone;
}
//------------------------------ getPhone -----------------------------------------
/*!
Get the event phone.
*/
QString C_GoogleEvent::getPhone()
{return m_phone;
}
//------------------------------ setEtag -----------------------------------------
/*!
Set the event Etag.
*/
void C_GoogleEvent::setEtag(const QString &etag)
{m_etag = etag;
}
//------------------------------ getWhere -----------------------------------------
/*!
Get the event Where.
*/
QString C_GoogleEvent::getWhere()
{return m_where;
}
//------------------------------ setWhere -----------------------------------------
/*!
Set the event Where.
*/
void C_GoogleEvent::setWhere(const QString &where)
{m_where = where;
}
//------------------------------ getColor -----------------------------------------
/*!
Get the event Color.
*/
QString C_GoogleEvent::getColor()
{return m_color;
}
//------------------------------ setColor -----------------------------------------
/*!
Set the event Color.
*/
void C_GoogleEvent::setColor(const QString &color)
{m_color = color;
}
//------------------------------ getEvent_ID -----------------------------------------
/*!
Get the event ID.
*/
QString C_GoogleEvent::getEvent_ID()
{return m_event_ID;
}
//------------------------------ setEvent_ID -----------------------------------------
/*!
set the event ID.
*/
void C_GoogleEvent::setEvent_ID(const QString &id)
{m_event_ID = id;
}
//------------------------------ getDateDeb -----------------------------------------
/*!
Get the event start date.
*/
QDateTime C_GoogleEvent::getDateDeb()
{return m_dateDeb;
}
//------------------------------ getDateEnd -----------------------------------------
/*!
Get the event end date.
*/
QDateTime C_GoogleEvent::getDateEnd()
{return m_dateEnd;
}

//------------------------------ setDateDeb -----------------------------------------
/*!
Set the event occurrence start date.
*/
void C_GoogleEvent::setDateDeb(const QDateTime &dateDeb)
{m_dateDeb = dateDeb;
}
//------------------------------ setDateEnd -----------------------------------------
/*!
Set the event occurrence end date.
*/
void C_GoogleEvent::setDateEnd(const QDateTime &dateEnd)
{m_dateEnd = dateEnd;
}
//------------------------------ setDateDeb -----------------------------------------
/*!
Set the event occurrence start date.
*/
void C_GoogleEvent::setDateDeb(const QString &dateDeb)
{m_dateDeb = QDateTime::fromString(dateDeb,"yyyy-MM-ddThh:mm:ss");
}
//------------------------------ setDateEnd -----------------------------------------
/*!
Set the event occurrence end date.
*/
void C_GoogleEvent::setDateEnd(const QString &dateEnd)
{m_dateEnd = QDateTime::fromString(dateEnd,"yyyy-MM-ddThh:mm:ss");
}
//------------------------------ getTitle -----------------------------------------
/*!
Get the event title.
*/
QString C_GoogleEvent::getTitle()
{return m_title;
}

//------------------------------ setTitle -----------------------------------------
/*!
Set the event title.
*/
void C_GoogleEvent::setTitle(const QString &newTitle)
{ m_title = newTitle;
}
//------------------------------ serialize -----------------------------------------
/*!
 serialize event to google json format
*/
QString C_GoogleEvent::serialize()
{
     QString str;
     QString strDeb   = m_dateDeb.toUTC().toString(Qt::ISODate);
     QString strEnd   = m_dateEnd.toUTC().toString(Qt::ISODate);
     if ( !strDeb.endsWith("Z") )   strDeb += "Z";
     if ( !strEnd.endsWith("Z") )   strEnd += "Z";
     str =  QString("{\n") +
            QString("\"summary\": \"%1\",\n").arg(m_title) +
            QString("\"description\": \"%1\",\n").arg(m_content);
            if (m_etag.length())     str += QString("\"etag\": \"%1\",\n").arg(m_etag);
            if (m_event_ID.length()) str += QString("\"id\": \"%1\",\n").arg(m_event_ID);
            if (m_where.length())    str += QString("\"location\": \"%1\",\n").arg(m_where);
            str += QString("\"start\": { \"dateTime\": \"%1\" },\n").arg(strDeb) +
                   QString("\"end\": { \"dateTime\": \"%1\" },\n")   .arg(strEnd);

            str += QString("\"extendedProperties\": {\n"
                    "\"shared\": {\n"
                    "\"isFromMedinTux\": \"ok\",\n"
                    "\"color\": \"%1\",\n"
                    "\"phone\": \"%2\"\n"
                    "  }\n"
                    " }\n").arg(m_color,m_phone);

           str +=         "}\n";

    return str;
}
//------------------------------ operator =  -----------------------------------------
/*!
Copy operator.
*/
C_GoogleEvent & C_GoogleEvent::operator = (const C_GoogleEvent &other)
{ this->m_title     = other.m_title;
  this->m_dateDeb   = other.m_dateDeb;
  this->m_dateEnd   = other.m_dateEnd;
  this->m_content   = other.m_content;
  this->m_event_ID  = other.m_event_ID;
  this->m_etag      = other.m_etag;
  this->m_where     = other.m_where;
  this->m_color     = other.m_color;
  this->m_phone     = other.m_phone;
  return *this;
}
