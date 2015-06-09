#include "C_CalendarManager.h"
#include <QJson/Parser>
#include <QJson/Serializer>

#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include <QSslError>

#include "../../MedinTuxTools-QT4/CGestIni.h"

//------------------------------------- C_CalendarManager ---------------------------------------------------
C_CalendarManager::C_CalendarManager(QObject* parent) : QObject(parent)
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_pNetworkAccessManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), this, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
    connect(m_pNetworkAccessManager, SIGNAL(finished  ( QNetworkReply * )),                             this, SLOT( replyFinished(QNetworkReply*)));
}
//------------------------------------Slot_sslErrors ---------------------------------------------
/*!  Slot called to ignore ssl errors (the process continue)
\param QNetworkReply * reply from request ( placed on ignore errors).
\param QList<QSslError> errors list
*/
void C_CalendarManager::Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors )
{ for (int i = 0; i < errors.size(); ++i)
      { qDebug() << "C_GoogleAPI::Slot_sslErrors () "<< errors.at(i).errorString ();
      }
  reply->ignoreSslErrors();
}
//------------------------------------- replyFinished ---------------------------------------------------
void C_CalendarManager::replyFinished(QNetworkReply * reply)
{
    QApplication::restoreOverrideCursor();
    qDebug() << __FUNCTION__;
    QByteArray ba_json = reply->readAll();
    QString json       = CGestIni::CharStrToQStringUtf8( ba_json.data() );
    qDebug() << "Reply = " << json;
    qDebug() << "URL = "   << reply->url();
    QString strUrl = reply->url().toString();

    if ( json.isEmpty() )
    {
        //Empty reply.
        qDebug() << "Empty answer";
        //Deletion of calendars or events
        QRegExp reg("calendars/*/events", Qt::CaseSensitive, QRegExp::Wildcard);
        if ( strUrl.indexOf(reg) != -1 )
           {
             emit eventChanged("");
           }
        else
           {
             emit calendarListChanged();
           }
        return;
    }

    QJson::Parser parser;
    bool ok;

    // json is a QByteArray containing the data to convert
    QVariant result = parser.parse (ba_json, &ok);
    if ( !ok )
       {
         emit errorOccured(QString("Cannot convert to QJson object: %1").arg(json));
         return;
       }

    if (result.toMap().contains("error"))
       {
         qDebug() << "ERROR occured:\n" << strUrl;
         emit errorOccured(result.toMap()["error"].toMap()["message"].toString());
         return;
       }
    if (result.toMap()["kind"].toString() == "calendar#calendarList")
       {
         m_calendars = result.toMap()["items"].toList();
         emit calendarListReady();
       }
    else if (result.toMap()["kind"].toString() == "calendar#calendar")
       {
         emit calendarListChanged();
       }
    else if (result.toMap()["kind"].toString() == "calendar#events")
       {
         m_events = result.toMap()["items"].toList();
         emit eventsReady();
       }
    else if (result.toMap()["kind"].toString() == "calendar#event")
       {
         emit eventChanged(result.toMap()["id"].toString());
       }
  reply->deleteLater();
}

//------------------------------------- getCalendars ---------------------------------------------------
void C_CalendarManager::getCalendars(const QString& access_token)
{   qDebug() << Q_FUNC_INFO;
    QString s = QString("https://www.googleapis.com/calendar/v3/users/me/calendarList?access_token=%1").arg(access_token);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}

//------------------------------------- getEventsForCalendar ---------------------------------------------------
int  C_CalendarManager::getEventsForCalendar(const QString& access_token, const QString & calID, const QDateTime &deb_dateTime, const QDateTime &end_dateTime)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_events.clear();

    qDebug() << Q_FUNC_INFO;   // 2012-09-30T09:11:13.562Z
    QString deb   = deb_dateTime.toString("yyyy-MM-ddThh:mm:ss.zzz")+"Z";
    QString end   = end_dateTime.toString("yyyy-MM-ddThh:mm:ss.zzz")+"Z";
    QString s = QString("https://www.googleapis.com/calendar/v3/calendars/%1/events?maxResults=2400&timeMin=%2&timeMax=%3").arg(calID,deb,end); //?access_token=%2").arg(calID).arg(access_token);
    QUrl url;

#if QT_VERSION >= 0x050000
    url.setUrl(QUrl::toPercentEncoding(s, "/:"));
#else
    url.setUrl(s);               //url.setUrl(QUrl::toPercentEncoding(s, "/:")); don't work in Qt4
#endif
    //............ fabriquer la requete ..........................
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("timeMin", deb.toLatin1());
    request.setRawHeader("timeMax", end.toLatin1());
    request.setRawHeader("Authorization", QString("OAuth %1").arg(access_token).toLatin1());
    request.setRawHeader("X-JavaScript-User-Agent", "Google APIs Explorer");
    //................. requeter .................................
    QNetworkAccessManager networkManager(this);
    QObject::connect(&networkManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), this, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
    QNetworkReply *reply = networkManager.get(request);

    //................. attendre la reponse .................................
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //................. analyser la reponse .................................
    QJson::Parser parser;
    QByteArray ba_json   = reply->readAll();
    QString json         = CGestIni::CharStrToQStringUtf8( ba_json.data() );
    if ( ! json.isEmpty() )
       {  bool ok;
          QVariant result      = parser.parse (ba_json, &ok);
         if ( ok )
            {     //................. recuperer les RDV .................................
              if ( result.toMap()["kind"].toString() == "calendar#events")
                 { m_events = result.toMap()["items"].toList();
                 }
            }
         else
            { emit errorOccured(QString("Cannot convert to QJson object: %1").arg(json));
            }
       }
    QApplication::restoreOverrideCursor();
    return m_events.count();
}

//------------------------------------- getCalendars ---------------------------------------------------
QVariantList C_CalendarManager::getCalendars()
{  return m_calendars;
}
//------------------------------------- getEvents ---------------------------------------------------
QVariantList C_CalendarManager::getEvents()
{  return m_events;
}
//------------------------------------- newCalendar ---------------------------------------------------
void C_CalendarManager::newCalendar(const QString& access_token, const QString & name)
{
    QString s = QString("https://www.googleapis.com/calendar/v3/calendars?access_token=%1").arg(access_token);
    QByteArray params;
    params.append(QString("{ \"summary\": \"%1\" }").arg(name).toUtf8());
    QNetworkRequest request;
    request.setUrl(QUrl(s));
    request.setRawHeader("Content-Type", "application/json");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->post(request, params);
}

//------------------------------------- createEvent ---------------------------------------------------
void C_CalendarManager::createEvent(const QString& access_token, const QString & calID, const QString &event_json)
{   qDebug() << " createEvent() = " << event_json;
    QByteArray params = event_json.toUtf8();
    QString         s = QString("https://www.googleapis.com/calendar/v3/calendars/%1/events").arg(calID);
    QUrl url;
#if QT_VERSION >= 0x050000
    url.setUrl(QUrl::toPercentEncoding(s, "/:"));
#else
    url.setUrl(s);               //url.setUrl(QUrl::toPercentEncoding(s, "/:")); don't work in Qt4
#endif
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("OAuth %1").arg(access_token).toLatin1());
    request.setRawHeader("Content-Type", "application/json");

    qDebug() << "Params to send" << params;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->post(request, params);
}

//------------------------------------- deleteEvent ---------------------------------------------------
void C_CalendarManager::deleteEvent(const QString& access_token, const QString & calID, const QString & eventID)
{
    QString s = QString("https://www.googleapis.com/calendar/v3/calendars/%1/events/%2").arg(calID).arg(eventID);
    QUrl url;
#if QT_VERSION >= 0x050000
    url.setUrl(QUrl::toPercentEncoding(s, "/:"));
#else
    url.setUrl(s);               //url.setUrl(QUrl::toPercentEncoding(s, "/:")); don't work in Qt4
#endif
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("OAuth %1").arg(access_token).toLatin1());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->deleteResource(request);
}


