#include "C_OAuth2.h"
#include <QDebug>
#include <QApplication>
#include "C_Dlg_logindialog.h"
#include <QSettings>
#include <QMessageBox>
#include <QByteArray>
#include <QList>
#include <QJson/Parser>
#include <QJson/Serializer>
#include "C_Dlg_GetSecrets.h"
#include "../../../MedinTuxTools-QT4/CGestIni.h"
//........................... planquette binaire ............................................
char    C_OAuth2::m_ClientSecret_Safe[] = "==|@|=={{                                                                                                                                                              }}==|@|==";
char    C_OAuth2::m_ClientID_Safe[]     = "==|#|=={{                                                                                                                                                              }}==|#|==";
//------------------------------- C_OAuth2 ----------------------------------------------------
C_OAuth2::C_OAuth2( QWidget* parent, const QString &login /*=""*/, const QString &pass /*=""*/, bool isAutoLog /* = false */)
{
    m_pParent             = parent;
    m_networkManager      = 0;
    m_isAutoLog           = isAutoLog;
    m_strClientID         = clientID_Safe();          // Google param plan A (param Google inscrits dans le binaire)
    m_clientSecret        = clientSecret_Safe();      // Google param plan A (param Google inscrits dans le binaire)
    //................ login plan A (le login et pass absents de parametres d'appel) ................
    m_strLogin            = login;
    m_strPassw            = pass;
    m_DateTokenExpiration = QDateTime();
    //................ login plan B (le login et pass absents de parametres d'appel) ................
    //                 on cherche dans le fichier developpeur
    if ( m_strLogin.length()==0||m_strPassw.length()==0 )    // GoogleAuth=machin.truc|monpasswword|1=GoogleAuth
       {  m_isAutoLog  = 0;
          QString config = CGestIni::Param_UpdateFromDisk(QDir::homePath()+"/MedinTuxRo.txt");
          if ( config.length() )
             {  QString result =CGestIni::getTextBetweenTwoTags( config, "GoogleAuth=","=GoogleAuth");
                if ( result.length() )
                   { QStringList pair = result.split('|');
                     if ( pair.size()==3 )
                        { m_strLogin      = pair[0].trimmed();
                          m_strPassw      = pair[1].trimmed();
                          m_isAutoLog     = pair[2].trimmed().toInt();
                        }
                   }
             }
       }
    //................ Google param plan B et C (param Google non inscrits dans le binaire) ................
    if ( m_strClientID.length()==0||m_clientSecret.length()==0 )
       { inputSecrets();
       }
    m_pLoginDialog     = new C_Dlg_logindialog(m_pParent,this, m_strLogin, m_strPassw, m_isAutoLog);
   // sous linux QT4 32 bit je suis passe directement par le pointeur car sinon cela plante : pourquoi ??? ........................
   // connect(m_pLoginDialog,  SIGNAL(Sign_sendSecret_Clicked(const QString &)), this, SLOT(Slot_sendSecret_Clicked(const QString &)));
    QTimer::singleShot(500, this, SLOT(Slot_ActivateLoginWindow())); // pour avoir la fenetre en avant plan sur ubuntu unity
}
//------------------------------- inputSecrets ----------------------------------------------------
int C_OAuth2::inputSecrets()
{   QStringList argList                    = qApp->arguments();
    int ret                                = 0;
    QString strClientID                    = "";
    QString strClientSecret                = "";
    //................ Google param plan B ( param Google dans parametres appel ) ................
    if (argList.size()>4) strClientID      = qApp->arguments()[4].trimmed();
    if (argList.size()>5) strClientSecret  = qApp->arguments()[5].trimmed();
    //................ Google param plan C ( verifier si pas dans fichier developpeur ) ................
    if ( strClientSecret.length()==0 && strClientID.length()==0 )
       {
         QString config = CGestIni::Param_UpdateFromDisk(QDir::homePath()+"/MedinTuxRo.txt");
         if ( config.length() )
            {  QString result =CGestIni::getTextBetweenTwoTags( config, "MedinTuxOAuth=","=MedinTuxOAuth");
               if ( result.length() )
                  { QStringList pair = result.split('|');
                    if ( pair.size()==2 )
                       { strClientID      = pair[0].trimmed();
                         strClientSecret  = pair[1].trimmed();
                       }
                  }
            }
       }
    //................ Google param plan D ( saisie manuelle ) ................
    C_Dlg_GetSecrets *pC_Dlg_GetSecrets    = new C_Dlg_GetSecrets( m_pParent );
    if ( pC_Dlg_GetSecrets ==0 ) return ret;
    pC_Dlg_GetSecrets->setClient_ID( strClientID );
    pC_Dlg_GetSecrets->setClient_Secret( strClientSecret );
    int out = pC_Dlg_GetSecrets->exec();
    if ( out==QDialog::Accepted )
       {   m_strClientID  = pC_Dlg_GetSecrets->client_ID();
           m_clientSecret = pC_Dlg_GetSecrets->client_Secret();
           ret = 1;
       }
    delete pC_Dlg_GetSecrets;
    return ret;
}

//------------------------------- clientSecret_Safe ----------------------------------------------------
QString C_OAuth2::clientSecret_Safe()
{ QString clientSecret_Safe = m_ClientSecret_Safe;
  int   d = clientSecret_Safe.indexOf(QString("..|@|..{{").replace('.','='));
  if  ( d==-1 ) return "";
  d += 9;
  int   e = clientSecret_Safe.indexOf(QString("}}..|@|..").replace('.','='),d);
  if  ( e==-1 ) return "";
  return clientSecret_Safe.mid(d,e-d).trimmed();
}
//------------------------------- clientID_Safe ----------------------------------------------------
QString C_OAuth2::clientID_Safe()
{   QString clientID_Safe = m_ClientID_Safe;
    int   d = clientID_Safe.indexOf(QString("..|#|..{{").replace('.','='));
    if  ( d==-1 ) return "";
    d += 9;
    int   e = clientID_Safe.indexOf(QString("}}..|#|..").replace('.','='),d);
    if  ( e==-1 ) return "";
    return clientID_Safe.mid(d,e-d).trimmed();
}

//------------------------------- Slot_ActivateLoginWindow ----------------------------------------------------
void C_OAuth2::Slot_ActivateLoginWindow()
{   m_pLoginDialog->show();
    m_pLoginDialog->setWindowState(m_pLoginDialog->windowState() & ~Qt::WindowMinimized);
    m_pLoginDialog->activateWindow();
    m_pLoginDialog->raise();
}
//------------------------------- loginUrl ----------------------------------------------------
QString C_OAuth2::loginUrl()
{
     QString str = QString("%1?"
                           "client_id=%2&"
                           "redirect_uri=%3&"
                           "response_type=%4&"
                           "scope=%5")
             .arg("https://accounts.google.com/o/oauth2/auth")  // url
             .arg(m_strClientID)                                // client ID
             .arg("urn:ietf:wg:oauth:2.0:oob")                  // redirect_uri
             .arg("code")                                       // response type
             .arg("https://www.googleapis.com/auth/calendar");  // scope (important to specify here url of service that we want to use)

    qDebug() << "Login URL" << str;
    return str;
}
//------------------------------- Slot_refreshToken----------------------------------------------------
void C_OAuth2::Slot_refreshToken()
{
    // many thanks to :
    //     http://stackoverflow.com/questions/25109873/google-oauth-2-0-installed-application-using-qt-missing-required-parameter-co?answertab=oldest#tab-top
    QUrl      serviceUrl = QUrl("https://accounts.google.com/o/oauth2/token");
    QByteArray postData  = "?refresh_token={{refresh_token}}&"
                           "client_id={{client_id}}&"
                           "client_secret={{client_secret}}&"
                           "redirect_uri={{redirect_uri}}&"
                           "grant_type={{grant_type}}";
    postData.replace("{{refresh_token}}",  m_refresh_token.toLatin1());
    postData.replace("{{client_id}}",      m_strClientID.toLatin1());
    postData.replace("{{client_secret}}",  m_clientSecret.toLatin1());
    postData.replace("{{redirect_uri}}",   "urn:ietf:wg:oauth:2.0:oob");
    postData.replace("{{grant_type}}",     "refresh_token");
    QUrl  url(postData.data());
    postData = url.toEncoded(QUrl::RemoveFragment);

    QNetworkAccessManager networkManager(this);
    QObject::connect(&networkManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), m_pLoginDialog, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
    QObject::connect(&networkManager, SIGNAL(finished(QNetworkReply*)),                                  this,           SLOT( Slot_finished_getRefreshTokenAccess(QNetworkReply*)));
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply *reply = networkManager.post(request, postData.mid(1));  // mid(1) pour virer le ? du debut : The shorter authorization code will work. The problem is with the question mark in the http post. The http post should not be a query.
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    Slot_finished_getRefreshTokenAccess(reply);

}
//------------------------------- Slot_sendSecret_Clicked ----------------------------------------------------
void C_OAuth2::Slot_sendSecret_Clicked( QString code, QNetworkAccessManager* pQNetworkAccessManager)
{
    // many thanks to :
    //     http://stackoverflow.com/questions/25109873/google-oauth-2-0-installed-application-using-qt-missing-required-parameter-co?answertab=oldest#tab-top
Q_UNUSED( pQNetworkAccessManager );

#if QT_VERSION >= 0x050000
    QUrl      serviceUrl = QUrl("https://accounts.google.com/o/oauth2/token");
    QByteArray postData  = "?code={{code}}&"
                           "client_id={{client_id}}&"
                           "client_secret={{client_secret}}&"
                           "redirect_uri={{redirect_uri}}&"
                           "grant_type={{grant_type}}";
    postData.replace("{{code}}",          code.toLatin1());
    postData.replace("{{client_id}}",     m_strClientID.toLatin1());
    postData.replace("{{client_secret}}", m_clientSecret.toLatin1());
    postData.replace("{{redirect_uri}}",  "urn:ietf:wg:oauth:2.0:oob");
    postData.replace("{{grant_type}}",    "authorization_code");
    QUrl  url(postData.data());
    postData = url.toEncoded(QUrl::RemoveFragment);

    if ( m_networkManager ) delete m_networkManager;
    m_networkManager = new QNetworkAccessManager(this);
    QObject::connect(m_networkManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), m_pLoginDialog, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
    QObject::connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),                                  this,           SLOT( Slot_finished_getTokenAccess(QNetworkReply*)));
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    m_networkManager->post(request, postData.mid(1));  // mid(1) pour virer le ? du debut : The shorter authorization code will work. The problem is with the question mark in the http post. The http post should not be a query.
#else
    QUrl      serviceUrl = QUrl("https://accounts.google.com/o/oauth2/token");
    QByteArray postData  = "?code={{code}}&"
                           "client_id={{client_id}}&"
                           "client_secret={{client_secret}}&"
                           "redirect_uri={{redirect_uri}}&"
                           "grant_type={{grant_type}}";
    postData.replace("{{code}}",          code.toLatin1());
    postData.replace("{{client_id}}",     m_strClientID.toLatin1());
    postData.replace("{{client_secret}}", m_clientSecret.toLatin1());
    postData.replace("{{redirect_uri}}",  "urn:ietf:wg:oauth:2.0:oob");
    postData.replace("{{grant_type}}",    "authorization_code");
    QUrl  url(postData.data());
    postData = url.toEncoded(QUrl::RemoveFragment);

    // if ( m_networkManager ) delete m_networkManager;
    // m_networkManager = new QNetworkAccessManager(this);
    QObject::connect(pQNetworkAccessManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), m_pLoginDialog, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
    QObject::connect(pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)),                                  this,           SLOT( Slot_finished_getTokenAccess(QNetworkReply*)));
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    pQNetworkAccessManager->post(request, postData.mid(1));  // mid(1) pour virer le ? du debut : The shorter authorization code will work. The problem is with the question mark in the http post. The http post should not be a query.
#endif
}
//------------------------------------Slot_sslErrors ---------------------------------------------
/*!  Slot called to ignore ssl errors (the process continue)
\param QNetworkReply * reply from request ( placed on ignore errors).
\param QList<QSslError> errors list
*/
void C_OAuth2::Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors )
{ for (int i = 0; i < errors.size(); ++i)
      { qDebug() << "C_GoogleAPI::Slot_sslErrors () "<< errors.at(i).errorString ();
      }
  reply->ignoreSslErrors();
}
//------------------------------- Slot_finished_getRefreshTokenAccess ----------------------------------------------------
void C_OAuth2::Slot_finished_getRefreshTokenAccess(QNetworkReply *reply)
{ qDebug() << "***** Slot_finished_getRefreshTokenAccess *****";
  finished_getTokenAccess( reply, C_OAuth2::refresh );
}

//------------------------------- Slot_finished_getTokenAccess ----------------------------------------------------
void C_OAuth2::Slot_finished_getTokenAccess(QNetworkReply *reply)
{   qDebug() << "***** Slot_finished_getTokenAccess *****";
    finished_getTokenAccess( reply ,C_OAuth2::initial);
    delete m_networkManager;
    m_networkManager = 0;
}
//------------------------------- finished_getTokenAccess ----------------------------------------------------
int  C_OAuth2::finished_getTokenAccess(QNetworkReply *reply, int mode)
{   // Reading attributes of the reply
    // e.g. the HTTP status code
    int done = 0;
    QVariant statusCodeV          = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this
    // no error received?
    QByteArray ba = reply->readAll();  // bytes
    if (reply->error() == QNetworkReply::NoError)
       { //........... ca va jaser dans le JASON  ------------------------
         if ( ba.length() > 50 )
            {
               QJson::Parser parser;
               bool ok;
               QVariant result = parser.parse (ba, &ok);    // json is a QByteArray containing the data to convert
               if ( ok &&  result.toMap().contains("access_token"))
                  {  m_access_token        = result.toMap()["access_token"].toString();
                     m_token_type          = result.toMap()["token_type"].toString();
                     m_expires_in          = result.toMap()["expires_in"].toString().toInt();
                     m_id_token            = result.toMap()["id_token"].toString();
                     m_DateTokenExpiration = QDateTime::currentDateTime().addSecs(m_expires_in-5*60);
                     //..... in refresh Token mode 'refresh_token' response is empty (not returned)..........
                     //      so don't change it's value in this case is IMPERATIVE
                     //      so we have to change it's value only in 'OAuth2::initial' mode
                     //      ( when get full token authentification )
                     if ( mode == C_OAuth2::initial )        // OAuth2::initial login mode ( get  access_token and refresh_token )
                        { m_refresh_token = result.toMap()["refresh_token"].toString();
                          QString  logRet = m_pLoginDialog->login();
                          int           p = logRet.indexOf('@'); if ( p !=-1 ) logRet.truncate(p);
                          QString  logIn  = m_strLogin;
                                        p = logIn.indexOf('@');  if ( p !=-1 ) logIn.truncate(p);
                          if ( logRet != logIn )   // le login n'est plus celui de depart
                          m_strLogin      = logRet + "@gmail.com";
                          m_strPassw      = "";
                          emit loginDone();
                        }
                     m_pLoginDialog->hide();
                     done = 1;
                  }
            } // endif ( ba.length() > 50 )
       } // endif (reply->error() == QNetworkReply::NoError)
    // Some http error received
    else
       {
         qDebug() << "***** reply http error =" << reply->error();
         m_pLoginDialog->Slot_setAutoLogin( false );
         m_pLoginDialog->show();
         done = 0;
       }
    qDebug() << "***** response *****" << ba.data();
    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->deleteLater();
    return done;
}

//------------------------------- accessToken ----------------------------------------------------
QString C_OAuth2::secretKey()
{ return m_clientSecret;
}

//------------------------------- accessToken ----------------------------------------------------
QString C_OAuth2::accessToken()
{   if ( QDateTime::currentDateTime() > m_DateTokenExpiration )
       { qDebug() << QString("***** '%1' OAuth2::accessToken() token out of date '%2' : refresh token request *****")
                     .arg( QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"),
                           m_DateTokenExpiration.toString("dd-MM-yyyy hh:mm:ss")
                         );
         Slot_refreshToken();
       }
    return m_access_token;
}
//------------------------------- tokenType ----------------------------------------------------
QString C_OAuth2::tokenType()
{ return m_token_type;
}
//------------------------------- isAuthorized ----------------------------------------------------
bool C_OAuth2::isAuthorized()
{ return m_access_token != "";
}

//------------------------------- startLogin ----------------------------------------------------
void C_OAuth2::startLogin(const QString &login /* = "" */, const QString &passw /* =""*/ )
{
    qDebug() << "OAuth2::startLogin";
    if ( m_strClientID == "YOUR_CLIENT_ID_HERE" )
       {
         QMessageBox::warning(m_pParent, "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-calendar/wiki/HowToRegisterYourApplicationInGoogle'>here</a>");
         return;
       }
  if (login.length()) m_strLogin = login;
  if (passw.length()) m_strPassw = passw;

   m_pLoginDialog->setLoginUrl( loginUrl(), m_strLogin , m_strPassw );

   m_pLoginDialog->show();
   m_pLoginDialog->Slot_setAutoLogin( false);
}

