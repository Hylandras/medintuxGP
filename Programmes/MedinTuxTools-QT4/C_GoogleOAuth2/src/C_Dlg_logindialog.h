#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QWebView>
#include <QUrl>
#include "../../../MedinTuxTools-QT4/C_Utils_Html.h"
namespace Ui {
    class C_Dlg_logindialog;
}
class C_OAuth2;
class C_Dlg_logindialog : public QDialog
{
    Q_OBJECT

public:
    explicit C_Dlg_logindialog(QWidget *parent = 0, C_OAuth2 *pC_OAuth2= 0,const QString &login="", const QString &passw="", bool isAutoLog=true);
    ~C_Dlg_logindialog();
    void setLoginUrl(const QString& url, const QString &login  = "" , const QString &passw ="" );
    QString accessToken();
    QString login(){return m_login;}
    QString passw(){return m_passw;}
public slots:
    void Slot_setAutoLogin( bool state);
    void Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors );
private slots:
    void urlChanged(const QUrl& url);
    void loadStarted();
    void loadFinished(bool);
    void Slot_sendSecret_clicked();

signals:
    void accessTokenObtained();
    void Sign_sendSecret_Clicked(const QString & secret);

private:
    Ui::C_Dlg_logindialog *ui;
    QWidget  *m_parent;
    C_OAuth2 *m_pC_OAuth2;
    QString m_strAccessToken;
    QString m_pathAppli;
    QString m_login;
    QString m_passw;
    bool    m_isAutoLog;
    int     m_page;
    QString m_code;
    QNetworkAccessManager *m_networkManager;
    int     m_url;

};

#endif // LOGINDIALOG_H
