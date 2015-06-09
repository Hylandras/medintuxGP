#ifndef C_QWEBVIEW_H
#define C_QWEBVIEW_H

#include <QWebView>
#include <QWebPage>
#include <QToolBar>
#include <QAction>
#include <QNetworkAccessManager>
#include <QtGlobal>

class C_QWebView;
//============================================== C_QWebPage =====================================================
class C_QWebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit          C_QWebPage(QObject *parent = 0);
    virtual QWebPage *createWindow ( QWebPage::WebWindowType type );
    C_QWebView       *m_pC_QWebViewParent;
signals:
public slots:

};

//============================================== C_QWebView =====================================================
class C_QWebView : public QWebView
{
    Q_OBJECT
public:
    explicit              C_QWebView(QWidget *parent = 0);
    ~C_QWebView();
    C_QWebPage            *m_pC_QWebPage;

    QToolBar              *m_ToolBar;
    QAction               *m_action_Back;
    QAction               *m_action_Reload;
    QAction               *m_action_Forward;
    QAction               *m_action_Stop;
    QNetworkAccessManager *m_pAM;
    void setPathImage(const QString &path){m_pathImage=path; if (!m_pathImage.endsWith('/')) m_pathImage+='/';}
    QString pathImage(){return m_pathImage;}
    void displayHtml(const QString &html);


signals:
public slots:
    //------------------------------------Slot_sslErrors ---------------------------------------------
    /*!  Slot called to ignore ssl errors (the process continue)
    \param QNetworkReply * reply from request ( placed on ignore errors).
    \param QList<QSslError> errors list
    */
    virtual void Slot_sslErrors ( QNetworkReply * reply, const QList<QSslError> & errors );
private:
    QString m_pathImage;
};

#endif // C_QWEBVIEW_H
