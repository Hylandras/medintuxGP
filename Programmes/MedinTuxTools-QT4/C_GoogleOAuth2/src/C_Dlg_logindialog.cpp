#include "C_Dlg_logindialog.h"
#include "ui_C_Dlg_logindialog.h"

#include <QDebug>
#include <QFileInfo>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QApplication>
#include "../../../MedinTuxTools-QT4/CGestIni.h"
#include "../../../MedinTuxTools-QT4/C_GoogleOAuth2/src/C_OAuth2.h"

//------------------------------- C_Dlg_logindialog ---------------------------------------------
C_Dlg_logindialog::C_Dlg_logindialog(QWidget *parent, C_OAuth2 *pC_OAuth2, const QString &login, const QString &passw, bool isAutoLog) :
    QDialog(parent),
    ui(new Ui::C_Dlg_logindialog)
{
    m_page           =  0;
    m_url            =  0;
    m_login          =  login;
    m_passw          =  passw;
    m_isAutoLog      =  isAutoLog;
    m_parent         =  parent;
    m_pC_OAuth2      =  pC_OAuth2;
    m_networkManager = new QNetworkAccessManager(this);

    ui->setupUi(this);
    ui->webView->page()->setNetworkAccessManager(m_networkManager);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)),         this, SLOT(urlChanged(QUrl)));
    connect(ui->webView, SIGNAL(loadStarted()),            this, SLOT(loadStarted()));
    connect(ui->webView, SIGNAL(loadFinished(bool)),       this, SLOT(loadFinished(bool)));
    connect(m_networkManager,          SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), this, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
    connect(ui->pushButton_sendSecret ,SIGNAL(clicked()),  this, SLOT(Slot_sendSecret_clicked()));

    ui->pushButton_sendSecret->hide();
    ui->lineEdit_secret->hide();
    QFileInfo qfi( qApp->arguments().at(0) );
    QString nameAppli   = qApp->applicationName();
    m_pathAppli         = CGestIni::Construct_PathBin_Module(nameAppli, qfi.path());

}

//------------------------------- ~C_Dlg_logindialog ---------------------------------------------
C_Dlg_logindialog::~C_Dlg_logindialog()
{   delete m_networkManager;
    delete ui;

}

//------------------------------- Slot_sendSecret_clicked ---------------------------------------------
void C_Dlg_logindialog::Slot_sendSecret_clicked()
{ emit Sign_sendSecret_Clicked(ui->lineEdit_secret->text());
}

//------------------------------- loadStarted ---------------------------------------------
void C_Dlg_logindialog::loadStarted()
{   //QApplication::setOverrideCursor(Qt::WaitCursor);
    qDebug() << "loadStarted";
}
//------------------------------- Slot_setAutoLogin ---------------------------------------------
void C_Dlg_logindialog::Slot_setAutoLogin( bool state)
     {
       m_isAutoLog = state;
       ui->webView->show();
     }
//------------------------------- loadFinished ---------------------------------------------
//  QWebElement we_loguin_form    = pMainQWebFrame->findFirstElement("#gaia_loginform");
//  qDebug() << "Login Form : " <<  we_loguin_form.evaluateJavaScript("this.id").toString();
//  QWebElement we_alert        = pMainQWebFrame->findFirstElement("#errormsg_0_Passwd");
//  qDebug() << "Alert : " <<  we_alert.evaluateJavaScript("this.id").toString();
//  if ( pMainQWebFrame->findFirstElement("#gaia_loginform").attribute("id")=="gaia_loginform")
//  if ( pMainQWebFrame->findFirstElement("#errormsg_0_Passwd").attribute("id") != "errormsg_0_Passwd")
void C_Dlg_logindialog::loadFinished(bool /* b */)
{    m_code                    = "";
     QString html              = ui->webView->page()->currentFrame()->documentElement().toOuterXml();
     QWebFrame *pMainQWebFrame = ui->webView->page()->mainFrame();
     QString title             = pMainQWebFrame->title();
     QWebElement we;
#ifdef QT_DEBUG
     QString fileName          = title;
     QString html_file         = m_pathAppli+fileName.remove('.').replace(' ','_').replace(":","_").replace("/","()")+".html";
     CGestIni::Param_UpdateToDisk( html_file, html );
#endif
    // recuperer le login qui peut avoir ete change dans le dialogue de saisie Google
    if ( ! (we=pMainQWebFrame->findFirstElement("div[class=gb_qa]")).isNull() )
       { m_login = we.toPlainText().trimmed();
         m_passw = "";
       }
    if ( ! pMainQWebFrame->findFirstElement("#gaia_loginform").isNull() )
       { QWebElement we_Login        = pMainQWebFrame->findFirstElement("#Email");
         QWebElement we_Passw        = pMainQWebFrame->findFirstElement("#Passwd");
         QWebElement we_button       = pMainQWebFrame->findFirstElement("#signIn");
         //............. si pas de message d'erreur placer le login et password .............
         if ( pMainQWebFrame->findFirstElement("#errormsg_0_Passwd").isNull() )          // html.indexOf("<span role=\"alert\" class=\"error-msg\" id=\"errormsg_0_Passwd\">") ==-1 )
            { /*
              QString login = we_Login.attribute("value").trimmed();
              QString passw = we_Passw.attribute("value").trimmed();
              if (login.length()==0)    login   = m_login;
              else                      m_login = login;
              if (passw.length()==0)    passw   = m_passw;
              else                      m_passw = passw;
              */
              we_Login.evaluateJavaScript(QString("this.value='%1'").arg(m_login));
              we_Passw.evaluateJavaScript(QString("this.value='%1'").arg(m_passw));
              // we_Passw.evaluateJavaScript(QString("this.type='%1'").arg("text"));
              if ( m_isAutoLog ) we_button.evaluateJavaScript("this.click()");
            }
       }
    else if ( ! pMainQWebFrame->findFirstElement("#connect-approve").isNull() )
       { QWebElement we_button_ok  = pMainQWebFrame->findFirstElement("#submit_approve_access");
         QVariant response;
         if ( m_isAutoLog ) response = we_button_ok.evaluateJavaScript("this.click()");
       }
    else if ( title.startsWith( "Success code=" ) )
       { ui->webView->hide();
         m_code = title.mid(13);
         m_pC_OAuth2->Slot_sendSecret_Clicked( m_code, m_networkManager );
         //ui->lineEdit_secret->setText(code);
       }
}

//------------------------------------Slot_sslErrors ---------------------------------------------
/*!  Slot called to ignore ssl errors (the process continue)
\param QNetworkReply * reply from request ( placed on ignore errors).
\param QList<QSslError> errors list
*/
void C_Dlg_logindialog::Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors )
{ for (int i = 0; i < errors.size(); ++i)
      { qDebug() << "C_GoogleAPI::Slot_sslErrors () "<< errors.at(i).errorString ();
      }
  reply->ignoreSslErrors();
}
//------------------------------- urlChanged ---------------------------------------------
void C_Dlg_logindialog::urlChanged(const QUrl &url)
{
    qDebug() << "URL =" << url;
}

//------------------------------- accessToken ---------------------------------------------
QString C_Dlg_logindialog::accessToken()
{  return m_strAccessToken;
}

//------------------------------- setLoginUrl ---------------------------------------------
void C_Dlg_logindialog::setLoginUrl(const QString& url, const QString &login /*=""*/,const QString &passw/*=""*/)
{    if (login.length()) m_login = login;
     if (passw.length()) m_passw = passw;
     ui->webView->setUrl(QUrl(url));
}
