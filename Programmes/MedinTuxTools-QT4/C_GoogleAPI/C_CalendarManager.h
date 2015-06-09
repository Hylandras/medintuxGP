#ifndef CALENDAR_DATA_MANAGER_H
#define CALENDAR_DATA_MANAGER_H

#include <QObject>
#include <QVariantList>
#include <QStringList>

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDate>
#include <QTime>

class C_CalendarManager : public QObject
{
    Q_OBJECT
public:
    explicit C_CalendarManager(QObject *parent = 0);

    //Methods to get information from Google Calendar service.
    void getCalendars(const QString& access_token);
    int  getEventsForCalendar(const QString& access_token, const QString & calID, const QDateTime &deb_dateTime=QDateTime::currentDateTime().addMonths(-6), const QDateTime &end_dateTime=QDateTime::currentDateTime().addMonths(6));
    void newCalendar(const QString& access_token, const QString & name);
    void createEvent(const QString& access_token, const QString &calID, const QString &event_json);
    void deleteEvent(const QString& access_token, const QString & calID, const QString & eventID);

    //Methods to get retrieved Calendars and Events.
    QVariantList getCalendars();
    QVariantList getEvents();

signals:
    void errorOccured(const QString&);
    void calendarListReady();
    void eventsReady();
    void calendarListChanged();
    void eventChanged(const QString& id);

private slots:
    void replyFinished(QNetworkReply*);
    void Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors );

private:
    QNetworkAccessManager* m_pNetworkAccessManager;
    QVariantList           m_calendars;
    QVariantList           m_events;
};

#endif // CALENDAR_DATA_MANAGER_H
