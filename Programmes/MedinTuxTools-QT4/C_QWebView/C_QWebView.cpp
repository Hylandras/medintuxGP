#include "C_QWebView.h"
#include <QMainWindow>
#include <QAction>
#include <QSslError>
#include <QDebug>
#include <QNetworkReply>
//==================================== C_QWebPage =========================================================
//----------------------------------------- QWebPage --------------------------------------------------
C_QWebPage::C_QWebPage(QObject *parent)
    :QWebPage(parent)
{ m_pC_QWebViewParent = (C_QWebView*) parent;
}

//----------------------------------------- createWindow --------------------------------------------------
QWebPage *C_QWebPage::createWindow ( QWebPage::WebWindowType type )
{   Q_UNUSED(type);
    C_QWebView   *view  = new C_QWebView(m_pC_QWebViewParent);
    QMainWindow    *mw  = new QMainWindow(m_pC_QWebViewParent);
    view->m_ToolBar     = mw->addToolBar(tr("Navigation"));

    view->m_action_Back = new QAction(mw);
    view->m_action_Back->setObjectName(QString::fromUtf8("m_action_Back"));
    view->m_action_Back->setIcon( view->pageAction(QWebPage::Back)->icon() ) ;
    view->m_ToolBar->addAction(view->m_action_Back);

    view->m_action_Reload = new QAction(mw);
    view->m_action_Reload->setObjectName(QString::fromUtf8("m_action_Reload"));
    view->m_action_Reload->setIcon( view->pageAction(QWebPage::Reload)->icon() ) ;
    view->m_ToolBar->addAction(view->m_action_Reload);

    view->m_action_Forward = new QAction(mw);
    view->m_action_Forward->setObjectName(QString::fromUtf8("m_action_Forward"));
    view->m_action_Forward->setIcon( view->pageAction(QWebPage::Forward)->icon() ) ;
    view->m_ToolBar->addAction(view->m_action_Forward);

    view->m_action_Stop = new QAction(mw);
    view->m_action_Stop->setObjectName(QString::fromUtf8("m_action_Stop"));
    view->m_action_Stop->setIcon( view->pageAction(QWebPage::Stop)->icon() ) ;
    view->m_ToolBar->addAction(view->m_action_Stop);

    connect( view->m_action_Back,         SIGNAL(triggered ()),       view, SLOT(back ()) );
    connect( view->m_action_Reload,       SIGNAL(triggered ()),       view, SLOT(reload ()) );
    connect( view->m_action_Forward,      SIGNAL(triggered ()),       view, SLOT(forward ()) );
    connect( view->m_action_Stop,         SIGNAL(triggered ()),       view, SLOT(stop ()) );

    mw->setCentralWidget(view);
    C_QWebPage *page = new C_QWebPage(view);
    view->setPage(page);
    mw->show();
    view->show();
    return page;
}


//==================================== C_QWebView =========================================================
//----------------------------------------- C_QWebView --------------------------------------------------
C_QWebView::C_QWebView(QWidget *parent)
    :QWebView(parent)
{   m_pC_QWebPage = new C_QWebPage(this);

    // Qpointer<QWebPage> _webPage = new QWebPage();
    // QPointer<QWebView> _webView = new QWebview();
    // _webPage -> setNetworkAccessManager(am);
    //  QWebView->setPage(_webPage);
    //............. pour la gestion des erreurs SSL ...............
    m_pAM = new QNetworkAccessManager(this);
    if (m_pAM && m_pC_QWebPage)
       { m_pC_QWebPage->setNetworkAccessManager(m_pAM);
         connect(m_pAM, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & ) ), this, SLOT( Slot_sslErrors ( QNetworkReply * , const QList<QSslError> & ) ));
       }
    setPage ( m_pC_QWebPage );
}

//----------------------------------------- ~C_QWebView --------------------------------------------------
void C_QWebView::displayHtml(const QString &html)
{
#ifdef Q_WS_WIN
   setHtml( html, QUrl(m_pathImage) );
#endif
#ifdef Q_OS_MAC
   setHtml(html,  QUrl(m_pathImage) );
#endif
#ifdef Q_WS_X11
   //pQWebView->load(QUrl(path));
   setHtml(html,  QUrl(m_pathImage) );
#endif
}

//----------------------------------------- ~C_QWebView --------------------------------------------------
C_QWebView::~C_QWebView()
{
    if (m_pAM) m_pAM->deleteLater();
}

//------------------------------------Slot_sslErrors ---------------------------------------------
/*!  Slot called to ignore ssl errors (the process continue)
\param QNetworkReply * reply from request ( placed on ignore errors).
\param QList<QSslError> errors list
*/
void C_QWebView::Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors )
{ for (int i = 0; i < errors.size(); ++i)
      { qDebug() << "C_QWebView::Slot_sslErrors () "<< errors.at(i).errorString ();
      }
  reply->ignoreSslErrors();
}

/*
QWebPage* WebPage::createWindow(QWebPage::WebWindowType type)
{
 Q_UNUSED(type);
 QWebView *view = new QWebView(mainwnd);
 QMainWindow *mw = new QMainWindow(mainwnd);
 mw->setCentralWidget(view);
 WebPage *page = new WebPage(view);
 view->setPage(page);
 //The window state is a OR'ed combination of Qt::WindowState: Qt::WindowMinimized, Qt::WindowMaximized, Qt::WindowFullScreen, and Qt::WindowActive.
 //mw->setWindowState(mainwnd->windowState());
 view->load(hiplink);
 mw->show();
 view->show();
 return page;

 //return QWebPage::createWindow(type);
}



// extending QWebView
void MyWebView::click(const QString &selectorQuery)
{
    QWebElement el = this->page()->mainFrame()->findFirstElement(selectorQuery);
    if (!el)
         return;

    el.setFocus();

    QMouseEvent pressEvent(QMouseEvent::MouseButtonPress, el.geometry().center(),
                    Qt::MouseButton::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(this, &pressEvent);

    QMouseEvent releaseEvent(QMouseEvent::MouseButtonRelease,
                             el.geometry().center(), Qt::MouseButton::LeftButton,
                             Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(this, &releaseEvent);
}

And you call it as so:
myWebView->click("a[href]");  // will click first link on page
myWebView->click("input[type=submit]"); // submits a form







*/
