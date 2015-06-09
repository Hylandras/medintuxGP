/*********************************** C_GoogleAPI.cpp **********************
* #include "C_GoogleAPI.h"           QT4-ready                            *
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
#include "C_GoogleAPI.h"

#include <QSslSocket>
#include <QNetworkRequest>
#include <QXmlStreamReader>
#include <QApplication>
#include <QDir>
#include <QSslError>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QDebug>
#include <QMessageBox>

#include "C_GoogleAPI.h"
#include "C_GoogleAPI.h"
#include "C_CalendarManager.h"

#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_GoogleOAuth2/src/C_OAuth2.h"
//------------------------------------ C_GoogleAPI ---------------------------------------------
/*!
Create a new instance of the C_GoogleAPI class.
*/
C_GoogleAPI::C_GoogleAPI(QWidget *parent, QString login /* = ""*/, QString password /* = "" */) :
    QObject(parent)
{ m_strSelectedCalID      = "";                          // pas de calendrier actif
  m_strSummary            = "";                          // nom du calendrier
  m_parent                = parent;
  //........... creer le gestionnaire d'authenfication ...........................
  m_pC_OAuth2             = new C_OAuth2( m_parent, login, password, false );
  connect(m_pC_OAuth2, SIGNAL( loginDone() ), this, SLOT( Slot_loginDone() ));

  //........... creer le gestionnaire d''authenfication'agendas ...........................
  m_pC_CalendarManager    = new C_CalendarManager(this);
  connect( m_pC_CalendarManager, SIGNAL( errorOccured(QString) ), this, SLOT ( Slot_errorOccured(QString)) );
  connect( m_pC_CalendarManager, SIGNAL( calendarListReady() ),   this, SLOT ( Slot_calendarListReady())   );
  connect( m_pC_CalendarManager, SIGNAL( calendarListChanged() ), this, SLOT ( Slot_loginDone())           ); // calendarListChanged() emit when new MedinTux calendar is created --> Slot_loginDone() to get new calendar list with new created calendar
  //.............. on demarre le truc .........................................
  start_login(  login,  password );
}
//------------------------------------ ~C_GoogleAPI ---------------------------------------------
/*!
Clean up.
*/
C_GoogleAPI::~C_GoogleAPI()
{ delete m_pC_OAuth2;
  delete m_pC_CalendarManager;
}

//------------------------------------- Slot_errorOccured ----------------------------------
/*!  Slot called when error occured
\param error : error to display
*/
void C_GoogleAPI::Slot_errorOccured(const QString& error)
{
    if ( error == "Invalid Credentials" )
       { m_pC_OAuth2->startLogin();
       }
    else
       { qDebug() << tr("********  Error in C_GoogleAPI : ") << error << " ***********";
       }
}

//------------------------------------start_login ---------------------------------------------
/*!
 * \brief This function try to login into the Google account. If success the loginDone signal is
 *   emit and you have to continue action in Slot_loginDone connected to this signal just after this.
 *   succces login is conditionned by all this  calls :
 *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
 * \param login the full mail address.
 * \param password the password for the provided mail account.
*/
void C_GoogleAPI::start_login( QString login, QString password )
{   if ( m_pC_OAuth2==0 ) return;
    m_strSelectedCalID = "";   // pas de calendrier actif
    m_pC_OAuth2->startLogin(login,password);
}
//------------------------------------Slot_loginDone ---------------------------------------------
/*! \brief called when login is Ok, in this Slot we get calendars list, this Slot emit
 *  via replyFinished() , a calendarListReady() signal connected to Slot_calendarListReady()
 *  just after this.
 *  succces login is conditionned by all this  calls :
 *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
 *  \param qs_text : QString qui contient le bloc allant du curseur texte jusqu'Ã  la fin du texte
 *  \return Renvoie un QString contenant le nom de l'image collÃ©e au curseur sinon vide
*/

void C_GoogleAPI::Slot_loginDone()
{    m_strSelectedCalID = "";                                           // pas de calendrier en cours
     m_pC_CalendarManager->getCalendars( m_pC_OAuth2->accessToken() );  // emit calendarListReady() --> Slot_calendarListReady()
}

//------------------------------------- Slot_calendarListReady ----------------------------------
/*!  Slot called when calendars list is done and ready. We compare All calendars ID to
 *   user email Google acount url to get the good calendar m_strSelectedCalID.
 *   m_strSelectedCalID not empty permits to get events and it is the real element to
 *   evaluate succes login.
 *   succces login is conditionned by all this  calls :
 *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
*/
void C_GoogleAPI::Slot_calendarListReady()
{
    m_calendars           = m_pC_CalendarManager->getCalendars();
    m_strSelectedCalID    = "";
    m_strSummary          = "";
    for ( int i = 0; i < m_calendars.count(); ++i )
        { QString calName = m_calendars[i].toMap()["id"].toString();
          QString summary = m_calendars[i].toMap()["summary"].toString();
          if ( summary   != "MedinTux") continue;
          m_strSummary       = summary;
          m_strSelectedCalID = calName; break;
        }
    if ( m_strSelectedCalID.length() )
       { emit Sign_GoogleAPI_Ready();
         qDebug() << tr("*********** Access to Google Agenda '%1'  ID : '%2' is valid ****************").arg(m_strSummary, m_strSelectedCalID);
         return;
       }
    int ret   = QMessageBox::warning ( m_parent, tr("Create MedinTux Goggle Calendar :"),
                                                     QString( "<u><b> WARNING</u> :<br /></b> <font color=\"#1200ff\">MedinTux Google Calendar</font> don't exists"
                                                              "Do you want create it ?"),
                                                     tr("&Modify"), tr("&Cancel"), 0, 1, 1
                                    );
    if (ret==1)                  return;
    m_pC_CalendarManager->newCalendar( m_pC_OAuth2->accessToken(), "MedinTux" );
}

//------------------------------------loginSucces ---------------------------------------------
/*!  \brief return true if API is ready to used (all login process Ok)
 *   m_strSelectedCalID not empty permits to get events and it is the real element to
 *   evaluate succes login.
 *   succces login is conditionned by all this  calls :
 *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
*/
bool C_GoogleAPI::loginSucces()
{ return m_strSelectedCalID.length() != 0;
}
//------------------------------------loginFaillure ---------------------------------------------
/*!  \brief return true if API is not ready to used (all login process Ok)
 *   m_strSelectedCalID not empty permits to get events and it is the real element to
 *   evaluate succes login.
 *   succces login is conditionned by all this  calls :
 *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
*/
bool C_GoogleAPI::loginFaillure()
{ return m_strSelectedCalID.length() == 0;
}
//------------------------------------getEventsBetweenTwoDates ---------------------------------------------
/*!
This function starts the retrieval of all-day-events from the calendar
between two dates.
\param debDate is the start date.
\param endDate is the end date.
\return in number of retrived events between debDate and endDate .
*/

int C_GoogleAPI::getEventsBetweenTwoDates(const QDateTime &debDate, const QDateTime &endDate)
{ m_pC_CalendarManager->getEventsForCalendar(m_pC_OAuth2->accessToken(), m_strSelectedCalID, debDate, endDate);
  QVariantList json_events = m_pC_CalendarManager->getEvents();
  m_eventsList.clear();
  for ( int i = 0; i < json_events.count(); ++i )
      {   QVariantMap    event = json_events[i].toMap();
          QString title        = event.value("summary").toString();
          QString content      = event.value("description").toString();
          QString event_ID     = event.value("id").toString();
          QString where        = event.value("location").toString();
          QString etag         = event.value("etag").toString();
          QString color        = "#5779db";
          QString phone        = "";
          bool  isFromMedinTux = false;

          //....... traitement des dates et heures ...............
          QDateTime debDT, endDT;
          QVariantMap     mp_s = event.value("start").toMap();
          if ( mp_s.contains("dateTime") )
             {  debDT = QDateTime::fromString(mp_s.value("dateTime").toString(), Qt::ISODate);
             }
          QVariantMap     mp_e = event.value("end").toMap();
          if ( mp_e.contains("dateTime") )
             {  endDT = QDateTime::fromString(mp_e.value("dateTime").toString(), Qt::ISODate);
             }
          debDT  = debDT.toLocalTime();
          endDT  = endDT.toLocalTime();
          //....... traitement des extensions ...............
          QVariantMap     mp_x = event.value("extendedProperties").toMap();
          if ( mp_x.contains("shared") )
             {  QVariantMap mp_sh = mp_x.value("shared").toMap();
                if ( mp_sh.contains("isFromMedinTux") )
                   {   QString medintux_ok = mp_sh.value("isFromMedinTux").toString();
                       isFromMedinTux = (medintux_ok == "ok");
                   }
                if ( mp_sh.contains("phone") ) phone = mp_sh.value("phone").toString();
                if ( mp_sh.contains("color") ) color = mp_sh.value("color").toString();
             }
          if ( isFromMedinTux )
             { m_eventsList.append(C_GoogleEvent( title              ,
                                                  debDT              ,
                                                  endDT              ,
                                                  content            ,
                                                  where              ,
                                                  color              ,
                                                  phone              ,
                                                  etag               ,
                                                  event_ID
                                                )
                                  );
             }
      }  // end for ( int i = 0; i < json_events.count(); ++i )
  return m_eventsList.count();
}

//------------------------------------createSeveralEvents ---------------------------------------------
/*!
This function create events
\param startDate is the start date.
\param endDate is the end date.
*/
void C_GoogleAPI::createSeveralEvents(const C_GoogleEventList &eventsList)
{
    C_GoogleEvent event;
    if ( eventsList.size() <= 0 ) return;
    for ( int i = 0; i < eventsList.size(); ++i )
        { event    = eventsList[i];
          m_pC_CalendarManager->createEvent(m_pC_OAuth2->accessToken(), m_strSelectedCalID, event.serialize());
        }
}

//------------------------------------deleteAllEventsBetweenTwoDates ---------------------------------------------
/*!
This function erase all events already presents between two dates
\param debDate is the start date.
\param endDate is the end date.
*/
void C_GoogleAPI::deleteAllEventsBetweenTwoDates(const QDateTime &debDate, const QDateTime &endDate)
{//............ retrieve list (m_eventsList) of all events ................................
 //                     between debDate and endDate

      m_pC_CalendarManager->getEventsForCalendar(m_pC_OAuth2->accessToken(), m_strSelectedCalID, debDate, endDate);
      QVariantList json_events = m_pC_CalendarManager->getEvents();
      m_eventsList.clear();
      for ( int i = 0; i < json_events.count(); ++i )
          {   QVariantMap    event = json_events[i].toMap();
              QString event_ID     = event.value("id").toString();
              bool  isFromMedinTux = false;
              //....... traitement des extensions ...............
              QVariantMap     mp_x = event.value("extendedProperties").toMap();
              if ( mp_x.contains("shared") )
                 {  QVariantMap mp_sh = mp_x.value("shared").toMap();
                    if ( mp_sh.contains("isFromMedinTux") )
                       {   QString medintux_ok = mp_sh.value("isFromMedinTux").toString();
                           isFromMedinTux = (medintux_ok == "ok");
                       }
                 }
              if ( isFromMedinTux )
                 {  m_pC_CalendarManager->deleteEvent(m_pC_OAuth2->accessToken(), m_strSelectedCalID, event_ID);
                 }
          }
}

