/*********************************** C_GoogleAPI.h ************************
* #include "C_GoogleAPI.h"           QT4-ready                            *
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
#ifndef C_GOOGLEAPI_H
#define C_GOOGLEAPI_H

#include <QObject>
#include <QNetworkProxy>
#include "C_GoogleEvent.h"
#include "C_GoogleAuthentification.h"


//=============================== C_GoogleAPI ===============================
/*!
This class represents the  service necessary for accessing the google API.
The necessary documentation for using this
service can be found at Google:
http://code.google.com/intl/en/apis/calendar/data/2.0/reference.html
*/
class C_OAuth2;
class C_CalendarManager;
class C_GoogleAPI : public QObject
{
    Q_OBJECT

    public:
      explicit C_GoogleAPI(QWidget *parent = 0, QString login="", QString password="");
      virtual ~C_GoogleAPI();

      //------------------------------------login ---------------------------------------------
      /*!
       * \brief This function try to login into the Google account. If success the loginDone signal is
       * emit and you have to continue action in Slot_loginDone connected to this signal
       * succces login is conditionned by all this  calls :
       * start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady() --> emit Sign_GoogleAPI_Ready()
       * \param login the full mail address.
       * \param password the password for the provided mail account.
       */
      void start_login(QString login, QString password);
      //------------------------------------loginSucces ---------------------------------------------
      /*!  \brief return true if API is ready to used (all login process Ok)
       *   m_strSelectedCalID not empty permits to get events and it is the real element to
       *   evaluate succes login.
       *   succces login is conditionned by all this  calls :
       *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
      */
      bool loginSucces();
      //------------------------------------loginFaillure ---------------------------------------------
      /*!  \brief return true if API is not ready to used (all login process Ok)
       *   m_strSelectedCalID not empty permits to get events and it is the real element to
       *   evaluate succes login.
       *   succces login is conditionned by all this  calls :
       *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady()
      */
      bool loginFaillure();

      //------------------------------------getEventsBetweenTwoDates ---------------------------------------------
      /*!
      This function starts the retrieval of all-day-events from the calendar
      between two dates.
      \param debDate is the start date.
      \param endDate is the end date.
      \return in number of retrived events between debDate and endDate .
      */
      int               getEventsBetweenTwoDates(const QDateTime &newStartDate, const QDateTime &newEndDate);

      //------------------------------------deleteAllEventsBetweenTwoDates ---------------------------------------------
      /*!
      This function erase all events already presents between two dates
      \param debDate is the start date.
      \param endDate is the end date.
      */
      void              deleteAllEventsBetweenTwoDates(const QDateTime &newStartDate, const QDateTime &newEndDate);
      //------------------------------------createSeveralEvents ---------------------------------------------
      /*!
      This function create events
      \param startDate is the start date.
      \param endDate is the end date.
      */
      void              createSeveralEvents(const C_GoogleEventList &eventsList);

    protected slots:
      //------------------------------------Slot_loginDone ---------------------------------------------
      /*! \brief called when login is Ok, in this Slot we get calendars list, this Slot emit
       *  via replyFinished() , a calendarListReady() signal connected to Slot_calendarListReady()
       *  just after this.
       *  succces login is conditionned by all this  calls :
       *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady() --> emit Sign_GoogleAPI_Ready()
       *  \param qs_text : QString qui contient le bloc allant du curseur texte jusqu'à la fin du texte
       *  \return Renvoie un QString contenant le nom de l'image collée au curseur sinon vide
      */
      void Slot_loginDone();
      //------------------------------------- Slot_calendarListReady ----------------------------------
      /*!  Slot called when calendars list is done and ready. We compare All calendars ID to
       *   user email Google acount url to get the good calendar m_strSelectedCalID.
       *   m_strSelectedCalID not empty permits to get events and it is the real element to
       *   evaluate succes login.
       *   succces login is conditionned by all this  calls :
       *     start_login() --> Slot_loginDone() --> getCalendars() --> Slot_calendarListReady() --> emit Sign_GoogleAPI_Ready()
      */
      void Slot_calendarListReady();

      //------------------------------------- Slot_errorOccured ----------------------------------
      /*!  Slot called when error occured
       * \param error : error to display
      */
      void Slot_errorOccured(const QString& error);

    protected:

      C_OAuth2                 *m_pC_OAuth2;
      C_CalendarManager        *m_pC_CalendarManager;
      QString                   m_strSelectedCalID;
      QString                   m_strSummary;
      QWidget                  *m_parent;
private:
      QVariantList              m_calendars;
      C_GoogleEventList         m_eventsList;
signals:
      void Sign_GoogleAPI_Ready();
};

#endif // C_GOOGLEAPI_H

