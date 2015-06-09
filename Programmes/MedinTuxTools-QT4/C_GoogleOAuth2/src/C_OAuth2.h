#ifndef C_OAUTH2_H
#define C_OAUTH2_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>

//============================= C_OAuth2 ============================
class C_Dlg_logindialog;

class C_OAuth2 : public QObject
{
    Q_OBJECT
public:

    C_OAuth2(QWidget* parent = 0, const QString &login="", const QString &pass="", bool isAutoLog=false );
    QString accessToken();
    bool isAuthorized();
    void startLogin(const QString &login="", const QString &passw="");
    int  inputSecrets();
    QString login()  {return m_strLogin;}
    QString passw()  {return m_strPassw;}

public slots:
    void Slot_refreshToken();
    void Slot_sendSecret_Clicked( QString secret, QNetworkAccessManager* pQNetworkAccessManager);

private slots:
    void Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors );
    void Slot_ActivateLoginWindow();
    void Slot_finished_getRefreshTokenAccess(QNetworkReply *reply);
    void Slot_finished_getTokenAccess(QNetworkReply*reply);

private:
    enum getTokenMode { refresh = 0,
                        initial = 1
                      };
    QString loginUrl();
    int     finished_getTokenAccess(QNetworkReply *reply, int mode);
    QString tokenType();
    QString secretKey();
    QString clientSecret_Safe();
    QString clientID_Safe();

    //......... token property ............
    QString   m_access_token;
    QString   m_id_token;
    QString   m_token_type;
    QString   m_refresh_token;
    int       m_expires_in;
    QDateTime m_DateTokenExpiration;
    QNetworkAccessManager *m_networkManager;
    //......... authentification datas ....
    int     m_isAutoLog;
    QString m_strClientID;
    QString m_clientSecret;
    QString m_strLogin;
    QString m_strPassw;
    //......... login widgets .............
    C_Dlg_logindialog *m_pLoginDialog;
    QWidget           *m_pParent;

    static char m_ClientSecret_Safe[];           /*!<  to cache client secret            */
    static char m_ClientID_Safe[];               /*!<  to cache client ID                */

signals:
    void loginDone();
};

#endif // C_OAUTH2_H
