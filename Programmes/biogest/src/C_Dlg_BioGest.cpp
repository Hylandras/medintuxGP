#include "C_Dlg_BioGest.h"
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QTreeWidgetItemIterator>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>
#include <QWebSettings>
#include <QDesktopServices>
#include <QPrintDialog>
#include <QPrinter>
#include <QVariant>
#include <QDate>
#include <QWebFrame>
#include <QList>
#include <QTimer>
#include <QNetworkDiskCache>
#include <QFileInfo>
#include <QWebElementCollection>
#include <QWebElement>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QSignalMapper>
#include <QTextDocument>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSpacerItem>
#include <QHBoxLayout>

#include "CApp.h"
#include "C_ScruteFTP.h"
#include "C_ScruteDB.h"
#include "C_ClickableLed.h"

#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"

//------------------------------------ C_Dlg_BioGest -------------------------------
C_Dlg_BioGest::C_Dlg_BioGest(QWidget *parent) :
    QMainWindow(parent, Qt::WindowStaysOnTopHint),
    m_pGUI(new Ui::MainWindow)
{   QString        defStr = "";
    //QString        testStr = CApp::pCApp()->iniParam();
    m_pGUI->setupUi(this);
    //......... initialisations des variables membres .............................
    m_Apropos_Proc    = 0;
    m_MaskEditor_Proc = 0;
    m_pC_ScruteFTP    = 0;
    m_pC_ScruteDB     = 0;
    //............... relever les onglets ................................
    m_Wdg_HprimHtml =	m_pGUI->tabWidget_Views->widget ( 0 );
    m_Wdg_SitesLabo =   m_pGUI->tabWidget_Views->widget ( 1 );
    m_Wdg_HprimBrut =	m_pGUI->tabWidget_Views->widget ( 2 );
    m_Wdg_ConfigTab =   m_pGUI->tabWidget_Views->widget ( 3 );
    m_Wdg_ChangeTab =   m_pGUI->tabWidget_Views->widget ( 4 );
    //................ effacer les trucs non utiles ..............................................
    m_pGUI->tabWidget_Views->removeTab(m_pGUI->tabWidget_Views->indexOf(m_Wdg_SitesLabo));
    m_pGUI->tabWidget_Views->removeTab(m_pGUI->tabWidget_Views->indexOf(m_Wdg_ChangeTab));
    m_pGUI->tabWidget_Views->setCurrentWidget ( m_Wdg_HprimHtml );

    //...................... le label d'info la led d'aide la led de connexion ftp ..................................................
    m_pQLabel        = new QLabel(this);
    m_pQLabel->show();
    m_pQLabel->setText(tr("Welcome on biogest application"));
    statusBar()->insertPermanentWidget(0,m_pQLabel,1);

    m_HelpIndicator = new C_ClickableLed(this);
    m_HelpIndicator->setLedPixmap(Theme::getIcon("22x22/help.png"));
    m_HelpIndicator->setText(tr("Click me for help"));
    connect( m_HelpIndicator , SIGNAL( clicked() ) , this , SLOT( Slot_m_HelpIndicator_clicked() ) );
    statusBar()->insertPermanentWidget(1,m_HelpIndicator);

    m_FtpStateIndicator = new C_ClickableLed(this);
    m_FtpStateIndicator->setText(tr("Ftp connection status"));
    connect( m_FtpStateIndicator , SIGNAL( clicked() ) , this , SLOT( Slot_timerStateIndicator_clicked() ) );
    statusBar()->insertPermanentWidget(2,m_FtpStateIndicator);

    //...................... les dates ..................................................
    m_pGUI->dateEdit_Deb->setDate ( QDate::currentDate() );
    m_pGUI->dateEdit_Fin->setDate ( QDate::currentDate().addDays(1) );
    m_pGUI->checkBox_filtrerDates->setCheckState(Qt::Checked);

    //...................... charger le icones dans les boutons ........................
    m_pGUI->pushButton_ftp_fav_add->setIcon(Theme::getIcon("16x16/add.png"));
    m_pGUI->pushButton_ftp_fav_del->setIcon(Theme::getIcon("16x16/Vigie_annule.png"));
    m_pGUI->pushButton_ip_fav_add->setIcon(Theme::getIcon("16x16/add.png"));
    m_pGUI->pushButton_ip_fav_del->setIcon(Theme::getIcon("16x16/Vigie_annule.png"));
    m_pGUI->pushButtonDateDeb->setIcon(Theme::getIcon("16x16/calendrierDeb.png"));
    m_pGUI->pushButtonDateFin->setIcon(Theme::getIcon("16x16/calendrierFin.png"));
    m_pGUI->pushButton_MaskEdit->setIcon(Theme::getIcon("16x16/configure.png"));
    //................. initialiser le viewer html des sites du labo ....................
    delete m_pGUI->webView_SiteLabo;
    m_webView = new C_QWebView(m_pGUI->tabWidget_Views);
    m_webView->setObjectName(QString::fromUtf8("m_webView"));
    m_webView->setUrl(QUrl("about:blank"));
    m_pGUI->gridLayout_15->addWidget(m_webView, 3, 0, 1, 2);

    //................. initialiser les outils de gestion html ....................
    CApp::pCApp()->initUtilsHtml(this ,m_webView->page()->networkAccessManager() , m_pGUI->textEdit_Monitor );
    //................. initialiser les outils de configuration selon .ini ....................

    connect( m_pGUI->checkBox_justPatientsWithResults,   SIGNAL(stateChanged ( int  ) ),   this, SLOT(Slot_checkBox_justPatientsWithResults_stateChanged( int )) );
    m_pGUI->checkBox_justPatientsWithResults->setCheckState(CApp::pCApp()->justPatientsWithResults()?Qt::Checked:Qt::Unchecked);

    connect( m_pGUI->checkBox_filtrerDates,   SIGNAL(stateChanged ( int  ) ),   this, SLOT(Slot_checkBox_filtrerDates_stateChanged( int )) );
    m_pGUI->checkBox_filtrerDates->setCheckState(CApp::pCApp()->isDateFilterEnabled()?Qt::Checked:Qt::Unchecked);

    connect( m_pGUI->checkBox_ftp_daemon_enabled,   SIGNAL(stateChanged ( int  ) ),   this, SLOT(Slot_checkBox_ftp_daemon_enabled_stateChanged( int )) );
    m_pGUI->checkBox_ftp_daemon_enabled->setCheckState(CApp::pCApp()->isFtpDaemonEnabled()?Qt::Checked:Qt::Unchecked);

    connect( m_pGUI->checkBox_traceEnabled,   SIGNAL(stateChanged ( int  ) ),   this, SLOT(Slot_checkBox_traceEnabled_stateChanged( int )) );
    m_pGUI->checkBox_traceEnabled->setCheckState(CApp::pCApp()->readUniqueParam("biogest session", "LogFTP").toLower()[0]!='n'?Qt::Checked:Qt::Unchecked);

    connect( m_pGUI->checkBox_ftp_distantsDelete,   SIGNAL(stateChanged ( int  ) ),   this, SLOT(Slot_checkBox_ftp_distantsDelete_stateChanged( int )) );
    m_pGUI->checkBox_ftp_distantsDelete->setCheckState(CApp::pCApp()->readUniqueParam("biogest session", "EffacementDistant").toLower()[0]!='n'?Qt::Checked:Qt::Unchecked);

    connect( m_pGUI->checkBox_ftp_localsDelete,   SIGNAL(stateChanged ( int  ) ),   this, SLOT(Slot_checkBox_ftp_localsDelete_stateChanged( int )) );
    m_pGUI->checkBox_ftp_localsDelete->setCheckState(CApp::pCApp()->readUniqueParam("biogest session", "EffacementLocal").toLower()[0]!='n'?Qt::Checked:Qt::Unchecked);

    m_pGUI->lineEdit_ftp_scrutation_time->setText(CApp::pCApp()->readUniqueParam("biogest session", "Ftp scrutation"));
    m_pGUI->lineEdit_ip_scrutation_time->setText(CApp::pCApp()->readUniqueParam("biogest session", "DataBase scrutation"));
    m_pGUI->lineEdit_resultsLimit->setText(CApp::pCApp()->getResultsLimit());

    //............... positionner le menu contextuel ...........................................
    m_pGUI->treeWidget_Patients->setContextMenuPolicy ( Qt::CustomContextMenu );
    //............... charger la combo des serveurs TCP IP et la placer sur celui par defaut ...........................................
    //   Parametres  = QMYSQL3 , DrTuxTest , root ,  , localhost , 3306
    QStringList      lst;
    QStringList paramList = CApp::pCApp()->getProfilsList("_Parametres","Connexion");
    for (int i=0; i<paramList.size();++i)
        { lst    = paramList[i].split(",");
          //       driver               schema                log                    passw                hostname              port
          defStr = lst[0].trimmed()+","+lst[1].trimmed()+","+lst[2].trimmed()+","+encodedPassword(lst[3].trimmed())+","+lst[4].trimmed()+","+lst[5].trimmed();
          m_pGUI->comboBox_ip_fav->insertItem(0, defStr );    // comboBox_ip_fav
        }
    defStr = CApp::pCApp()->getdefaultDB_Connexion();
    lst    = defStr.split(",");
    defStr = lst[0].trimmed()+","+lst[1].trimmed()+","+lst[2].trimmed()+","+encodedPassword(lst[3].trimmed())+","+lst[4].trimmed()+","+lst[5].trimmed();
    connect( m_pGUI->comboBox_ip_fav ,  SIGNAL(activated ( const QString &  )), this, SLOT(Slot_comboBox_ip_fav_Activated(const QString & )) );
    m_pGUI->comboBox_ip_fav->setCurrentIndex(m_pGUI->comboBox_ip_fav->findText(defStr));
    Slot_comboBox_ip_fav_Activated( defStr );

    //............... charger la combo des serveurs ftp et la placer sur celui par defaut ...............................................
    paramList = CApp::pCApp()->getProfilsList("_ServeurFTP");
    for (int i=0; i<paramList.size();++i)
        { lst    = paramList[i].split(",");
          //       hostname             port                 login                 pass                                 distant rep
          defStr = lst[0].trimmed()+","+lst[1].trimmed()+","+lst[2].trimmed()+","+encodedPassword(lst[3].trimmed())+","+lst[4].trimmed();
          m_pGUI->comboBox_ftp_fav->insertItem(0, defStr );
        }
    defStr = CApp::pCApp()->getdefaultFtp();
    lst    = defStr.split(",");
    defStr = lst[0].trimmed()+","+lst[1].trimmed()+","+lst[2].trimmed()+","+encodedPassword(lst[3].trimmed())+","+lst[4].trimmed();
    connect( m_pGUI->comboBox_ftp_fav ,  SIGNAL(activated ( const QString &  )), this, SLOT(Slot_comboBox_ftp_fav_Activated(const QString & )) );
    m_pGUI->comboBox_ftp_fav->setCurrentIndex(m_pGUI->comboBox_ftp_fav->findText(defStr));
    Slot_comboBox_ftp_fav_Activated( defStr );

    //................. sauvegarder les listes avec password encodes ..................................................
    CApp::pCApp()->replaceComboboxListInParam(m_pGUI->comboBox_ftp_fav,  "biogest session", "_ServeurFTP");
    CApp::pCApp()->replaceComboboxListInParam(m_pGUI->comboBox_ip_fav,   "Connexion",       "_Parametres");
    CApp::pCApp()->updateIniParamToDisk();

    //............... charger la combo des services et la placer sur celui par defaut .................
    m_servicesList = CApp::pCApp()->getProfilsList("_Service");
    m_pGUI->comboBox_Service->insertItems(0, m_servicesList);
    defStr = CApp::pCApp()->getdefaultService();
    m_pGUI->comboBox_Service->setCurrentIndex(m_pGUI->comboBox_Service->findText(defStr));

    //............... charger la combo des templates et la placer sur celui par defaut .................
    paramList = CGestIni::listDirectory(CApp::pCApp()->getPathTemplates(), ".txt");
    m_pGUI->comboBox_modeles->insertItems(0,paramList);
    defStr = CApp::pCApp()->getCurrentTemplateFileName();
    m_pGUI->comboBox_modeles->setCurrentIndex(m_pGUI->comboBox_modeles->findText(defStr));
    connect( m_pGUI->comboBox_modeles ,  SIGNAL(activated ( const QString &  )), this, SLOT(Slot_comboBox_modeles_Activated(const QString & )) );

    //........................... connecter les trucs et machins ............................
    connect( m_pGUI->lineEdit_ip_scrutation_time, SIGNAL( editingFinished ()),                                              this, SLOT(Slot_lineEdit_ip_scrutation_time_editingFinished()) );
    connect( m_pGUI->lineEdit_ftp_scrutation_time,SIGNAL( editingFinished ()),                                              this, SLOT(Slot_lineEdit_ftp_scrutation_time_editingFinished()) );
    connect( m_pGUI->lineEdit_resultsLimit,       SIGNAL( editingFinished ()),                                              this, SLOT(Slot_lineEdit_resultsLimit_editingFinished()) );
    connect( m_pGUI->pushButton_logErase,         SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_logErase_Clicked()) );
    connect( m_pGUI->pushButton_playHprim ,       SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_playHprim_Clicked()) );
    connect( m_pGUI->pushButton_MaskEdit  ,       SIGNAL(clicked()),                                                        this, SLOT(Slot_pushpushButton_MaskEdit_Clicked()) );
    connect( m_pGUI->pushButton_ip_connect,       SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_ip_connect_Clicked()) );
    connect( m_pGUI->treeWidget_Patients,         SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                         this, SLOT(Slot_treeWidget_Patients_itemClicked( QTreeWidgetItem * , int)) );
    connect( m_pGUI->treeWidget_Patients,         SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                   this, SLOT(Slot_treeWidget_Patients_itemDoubleClicked( QTreeWidgetItem * , int)) );
    connect( m_pGUI->treeWidget_Patients ,        SIGNAL(itemSelectionChanged ()),                                          this, SLOT(Slot_treeWidget_Patients_itemSelectionChanged ()) );
    connect( m_pGUI->treeWidget_Patients ,        SIGNAL(customContextMenuRequested( const QPoint &)),                      this, SLOT(Slot_treeWidget_Patients_customContextMenuRequested (const QPoint &)) );
    connect( m_pGUI->pushButtonDateDeb ,          SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonDateDeb_Clicked()) );
    connect( m_pGUI->pushButtonDateFin ,          SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonDateFin_Clicked()) );
    connect( m_pGUI->comboBox_Service ,           SIGNAL(activated ( const QString &  )),                                   this, SLOT(Slot_comboBox_Service_Activated(const QString & )) );
    connect( m_pGUI->lineEdit_PatientSearch ,     SIGNAL(textChanged (const QString & )),                                   this, SLOT(Slot_lineEdit_PatientSearch_textChanged(const QString &)) );
    connect( m_pGUI->dateEdit_Deb,                SIGNAL(dateChanged ( const QDate &  )),                                   this, SLOT(Slot_dateEdit_Deb_dateChanged ( const QDate &  )) );
    connect( m_pGUI->dateEdit_Fin,                SIGNAL(dateChanged ( const QDate &  )),                                   this, SLOT(Slot_dateEdit_Fin_dateChanged ( const QDate &  )) );
    connect( CApp::pCApp() ,                      SIGNAL(Sign_CouCouDestroy()),                                             this, SLOT(Slot_CouCouDestroy())) ;

    /*
    comboBox_modeles
    pushButton_templ_add
    pushButton_templ_del
    textEdit_hprim
    //................. to hide when ftp on/of .................
    pushButton_ftp_fav_add
    pushButton_ftp_fav_del
    comboBox_ftp_fav
    */
    Slot_checkBox_ftp_daemon_enabled_stateChanged( m_pGUI->checkBox_ftp_daemon_enabled->checkState());
    //.................. afficher le chemin du template dans le log.........................................
    CApp::pCApp()->outMessage(tr("template en cours : <font color=#ff5400><b>%1</b></font>").arg(CApp::pCApp()->getPathTemplates()+CApp::pCApp()->getCurrentTemplateFileName()));
    CApp::pCApp()->setWindowIcon (Theme::getIcon("32x32/biogest.png") );
    setWindowTitle(CApp::pCApp()->applicationName());
    QTimer::singleShot ( 10, this, SLOT(Slot_lauch_C_ScruteFTP()) );    // creer les demons
}

//------------------------ decodedPassword ---------------------------------------
QString C_Dlg_BioGest::decodedPassword(QString pass)
{if (pass.startsWith('#')) return CGestIni::PassWordDecode(pass.mid(1));
 return    pass;
}

//------------------------ encodedPassword ---------------------------------------
QString C_Dlg_BioGest::encodedPassword(QString pass)
{if (pass.startsWith('#'))return pass;
 return QString("#")+CGestIni::PassWordEncode(pass);
}

//------------------------ Slot_lauch_C_ScruteFTP ---------------------------------------
// on le fait par un QTimer::singleShot afin de laisser l'interface principale s'installer
// afin que les messages de la pompe a hprim generes par C_ScruteFTP puissent etre affiches
void C_Dlg_BioGest::Slot_lauch_C_ScruteFTP()
{ showMinimized ();
  m_FtpStateIndicator->setLedColor(C_ClickableLed::Gray);
  //.................. creer le demon de scrutation du ftp .........................
  m_pC_ScruteFTP = new C_ScruteFTP (this);
  if ( m_pC_ScruteFTP )
     {connect( m_pC_ScruteFTP ,  SIGNAL(Sign_FilesAnalyseFinished()), this, SLOT(Slot_FilesAnalyseFinished()) );
      connect( m_pC_ScruteFTP ,  SIGNAL(Sign_FTP_status(int )),       this, SLOT(Slot_FTP_status( int)) );
      if (CApp::pCApp()->isFtpDaemonEnabled())  m_pC_ScruteFTP->startScrutation();
     }

  //.................. creer le demon de scrutation de la base .........................
  m_pC_ScruteDB  = new C_ScruteDB ( this, this);
  if ( m_pC_ScruteDB )
     { connect( m_pC_ScruteDB ,  SIGNAL(Sign_DB_AnalyseFinished(const QString &)), this, SLOT(Slot_DB_AnalyseFinished(const QString &)) );
       m_pC_ScruteDB->startScrutation();
     }
}

//------------------------ ~C_Dlg_BioGest ---------------------------------------
C_Dlg_BioGest::~C_Dlg_BioGest()
{if (m_pC_ScruteFTP) delete m_pC_ScruteFTP;
 if (m_pC_ScruteDB)  delete m_pC_ScruteDB;
}

//------------------------ Slot_retranslateUi ---------------------------------------
void C_Dlg_BioGest::Slot_retranslateUi()
{
}

//------------------------ closeEvent ---------------------------------------
void C_Dlg_BioGest::closeEvent(QCloseEvent *event)
{       QMessageBox msgBox(this);
        QPushButton *repliedButton  = msgBox.addButton(tr("Replier"), QMessageBox::ActionRole);
        QPushButton *cancelButton   = msgBox.addButton(tr("Annuler"), QMessageBox::ActionRole);
        QPushButton *quitButton     = msgBox.addButton(tr("Quitter"), QMessageBox::ActionRole);
        msgBox.setText ( tr("Quitter ce magnifique programme ou simplement le replier ?"));
        msgBox.exec();
        if (msgBox.clickedButton()      == repliedButton )
           {showMinimized ();
            event->ignore();
           }
        else if (msgBox.clickedButton() == cancelButton    )
           {event->ignore();
           }
        else if (msgBox.clickedButton() == quitButton    )
           {CApp::pCApp()->CouCouKill();
            event->accept();
           }
}
//------------------------ Slot_onQuit ---------------------------------------
void C_Dlg_BioGest::Slot_onQuit()
{
}
//------------------------ Slot_pushpushButton_MaskEdit_Clicked ---------------------------------------
void C_Dlg_BioGest::Slot_pushpushButton_MaskEdit_Clicked()
{   QString pathExeMaskEditor     = CGestIni::Construct_Name_Exe("QLightPad", QFileInfo (qApp->argv()[0]).path());
    QString pathMask              = CApp::pCApp()->getPathTemplates()+m_pGUI->comboBox_modeles->currentText();
    //......................... completer les autres arguments .........................................
    QStringList argList;
    argList << pathMask;      // 1  chemin du txte a editer
    if (m_MaskEditor_Proc==0)
       { bool ws = isMinimized();
         showMinimized ();
         m_MaskEditor_Proc = new QProcess(this);
         if (m_MaskEditor_Proc)
            {m_MaskEditor_Proc->start(pathExeMaskEditor, argList);
             m_MaskEditor_Proc->waitForStarted  (4000);
             m_MaskEditor_Proc->waitForFinished (-1);
             delete m_MaskEditor_Proc;
             m_MaskEditor_Proc = 0;
             CApp::pCApp()->setTemplate(CGestIni::Param_UpdateFromDisk(pathMask));
             Slot_treeWidget_Patients_itemSelectionChanged();
             //QFile::remove(pathBinRessources+"~A_propos.html");
            }
         if ( !ws) showNormal();
       }
}

//------------------------ Slot_m_HelpIndicator_clicked ---------------------------------------
void C_Dlg_BioGest::Slot_m_HelpIndicator_clicked()
{
    CGestIni::Param_UpdateToDisk(CApp::pCApp()->pathAppli()+"Ressources/Changements.html",m_pGUI->textEdit_changeLog->toHtml());
    QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).path());
    QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).path())+"Ressources/";
    QStringList argList;
    //......................... completer les autres arguments .........................................
    argList << "biogest";                                                     // 1  nom du module
    argList << tr("Module for FTP Hprim biologics results");                  // 2  description courte
    argList << CApp::pCApp()->ApplicationAndQtVersion();                      // 3  numero de version
    argList << CApp::pCApp()->pathAppli()+"Ressources/Changements.html";      // 4  fichiers decrivant les changements
    argList <<"";                                                                                           // 5  Icone par defaut
    argList <<"";                                                                                           // 6  aide en ligne (vide pour prendre celle par defaut)
    argList <<"";                                                                                           // 7  apropos (on met une chaine vide pour qu'il prenne celui par d?faut)
    argList << (CApp::pCApp()->getDB()?CApp::pCApp()->getDB()->getVersionConfig():tr("DB not available"));  // 8  numero de version de la base de donnee
    if (m_Apropos_Proc==0)
       { bool ws = isMinimized();
         showMinimized ();
         m_Apropos_Proc = new QProcess(this);
         if (m_Apropos_Proc)
            {m_Apropos_Proc->start(pathExeAPropos, argList);
             m_Apropos_Proc->waitForStarted  (4000);
             m_Apropos_Proc->waitForFinished (-1);
             delete m_Apropos_Proc;
             m_Apropos_Proc = 0;
             QFile::remove(pathBinRessources+"~A_propos.html");
            }
         if ( !ws) showNormal();
       }
}
//------------------------ Slot_timerStateIndicator_clicked ---------------------------------------
void C_Dlg_BioGest::Slot_timerStateIndicator_clicked()
{if (CApp::pCApp()->isFtpDaemonEnabled())
    {m_pC_ScruteFTP->startScrutation();
    }
 else
    {m_FtpStateIndicator->setLedColor(C_ClickableLed::Gray);
     CApp::pCApp()->outMessage(tr("===> FTP deamon is disabled."));
    }
}

//------------------------ setStatusLabeMessage ---------------------------------------
void C_Dlg_BioGest::setStatusLabeMessage(const QString & mess)
{m_pQLabel->setText(mess);
}

//------------------------ Slot_FTP_status ---------------------------------------
void C_Dlg_BioGest::Slot_FTP_status(int status)
{   if (status==QFtp::NoError)
        m_FtpStateIndicator->setLedColor(C_ClickableLed::Green);
    else
        m_FtpStateIndicator->setLedColor(C_ClickableLed::Red);
}
//------------------------ Slot_pushButton_logErase_Clicked ---------------------------------------
void C_Dlg_BioGest::Slot_pushButton_logErase_Clicked()
{m_pGUI->textEdit_Monitor->clear();
}

//------------------------ Slot_comboBox_modeles_Activated ---------------------------------------
void C_Dlg_BioGest::Slot_comboBox_modeles_Activated(const QString &str )
{QString path = CApp::pCApp()->getPathTemplates()+str;
 CApp::pCApp()->setCurrentTemplateFileName(str);
 CApp::pCApp()->setTemplate(CGestIni::Param_UpdateFromDisk(path));
 CApp::pCApp()->updateIniParamToDisk();
 Slot_treeWidget_Patients_itemSelectionChanged();
 CApp::pCApp()->outMessage(tr("template en cours : <font color=#ff5400><b>%1</b></font>").arg(CApp::pCApp()->getPathTemplates()+CApp::pCApp()->getCurrentTemplateFileName()));
}

//------------------------ Slot_comboBox_ftp_fav_Activated ---------------------------------------
void C_Dlg_BioGest::Slot_comboBox_ftp_fav_Activated(const QString &str )
{   QStringList paramList = str.split(",");
    m_pGUI->lineEdit_ftp_hostname->setText(paramList[0].trimmed());
    m_pGUI->lineEdit_ftp_port->setText(paramList[1].trimmed());
    m_pGUI->lineEdit_ftp_login->setText(paramList[2].trimmed());

    QString password = paramList[3].trimmed();
    if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
    password = CGestIni::PassWordEncode(password);
    password.prepend('#');
    m_pGUI->lineEdit_ftp_password->setText(password);
    m_pGUI->lineEdit_ftp_distantDir->setText(paramList[4].trimmed());
    CApp::pCApp()->writeParam("biogest session", "ServeurFTP",paramList[0].toAscii(),paramList[1].toAscii(),paramList[2].toAscii(),password.toAscii(),paramList[4].toAscii());
    CApp::pCApp()->writeParam("biogest session", "Ftp scrutation",m_pGUI->lineEdit_ftp_scrutation_time->text().toAscii());
    CApp::pCApp()->updateIniParamToDisk();
    if (m_pC_ScruteFTP && CApp::pCApp()->isFtpDaemonEnabled()) m_pC_ScruteFTP->startScrutation();
}
//------------------------ Slot_comboBox_ip_fav_Activated ---------------------------------------
//       driver               shema                 log                    passw                hostnaùe              port
void C_Dlg_BioGest::Slot_comboBox_ip_fav_Activated(const QString &str )
{   QStringList paramList = str.split(",");
    m_pGUI->lineEdit_ip_driver->setText(paramList[0].trimmed());
    m_pGUI->lineEdit_ip_schema->setText(paramList[1].trimmed());
    m_pGUI->lineEdit_ip_login->setText(paramList[2].trimmed());
    QString password = paramList[3].trimmed();
    if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
    password = CGestIni::PassWordEncode(password);
    password.prepend('#');

    m_pGUI->lineEdit_ip_password->setText(password);
    m_pGUI->lineEdit_ip_hostname->setText(paramList[4].trimmed());
    m_pGUI->lineEdit_ip_port->setText(paramList[5].trimmed());
    CApp::pCApp()->writeParam("Connexion", "Parametres",paramList[0].toAscii(),paramList[1].toAscii(),paramList[2].toAscii(),password.toAscii(),paramList[4].toAscii(),paramList[5].toAscii());
    CApp::pCApp()->writeParam("biogest session", "DataBase scrutation",m_pGUI->lineEdit_ip_scrutation_time->text().toAscii());
    CApp::pCApp()->updateIniParamToDisk();
    if (m_pC_ScruteDB)
       {m_pC_ScruteDB->setScrutationInterval(m_pGUI->lineEdit_ip_scrutation_time->text().toInt());
       }
}
//------------------------ Slot_lineEdit_resultsLimit_editingFinished ---------------------------------------
void C_Dlg_BioGest::Slot_lineEdit_resultsLimit_editingFinished()
{CApp::pCApp()->writeParam("biogest session", "Limiter le nombre de resultats",m_pGUI->lineEdit_resultsLimit->text().toAscii());
 CApp::pCApp()->updateIniParamToDisk();
}
//------------------------ Slot_lineEdit_ftp_scrutation_time_editingFinished ---------------------------------------
void C_Dlg_BioGest::Slot_lineEdit_ftp_scrutation_time_editingFinished()
{CApp::pCApp()->writeParam("biogest session", "Ftp scrutation",m_pGUI->lineEdit_ftp_scrutation_time->text().toAscii());
 if (m_pC_ScruteFTP) m_pC_ScruteFTP->setScrutationInterval(m_pGUI->lineEdit_ftp_scrutation_time->text().toInt());
 CApp::pCApp()->updateIniParamToDisk();
}
//------------------------ Slot_lineEdit_ip_scrutation_time_editingFinished ---------------------------------------
void C_Dlg_BioGest::Slot_lineEdit_ip_scrutation_time_editingFinished()
{CApp::pCApp()->writeParam("biogest session", "DataBase scrutation",m_pGUI->lineEdit_ip_scrutation_time->text().toAscii());
 if (m_pC_ScruteDB) m_pC_ScruteDB->setScrutationInterval(m_pGUI->lineEdit_ip_scrutation_time->text().toInt());
 CApp::pCApp()->updateIniParamToDisk();
}

//------------------------ Slot_pushButton_playHprim_Clicked ---------------------------------------
void C_Dlg_BioGest::Slot_pushButton_playHprim_Clicked()
{QStringList filesList = QFileDialog::getOpenFileNames (this,
                                                        tr("Choose hprim files to play"),
                                                        CApp::pCApp()->getPathTemp(),
                                                        tr("hprim Files (*.hpr *.txt *)")
                                                       );
 if (filesList.size()==0) return;
 QString localFileName = "";
 int               ret =  7777;
 for (int i = 0; i < filesList.size(); ++i)
     {   localFileName = filesList.at(i);
         ret =  CApp::pCApp()->getDB()->analyseFile(localFileName);
         QString mess = "";
         switch (ret)
             {
               case C_BaseBioGest::Pdf:                mess = tr("-%1- pdf file ").arg(QString::number(ret));            break;
               case C_BaseBioGest::FileCanNotOpen:     mess = tr("-%1- FileCanNotOpen ").arg(QString::number(ret));      break;
               case C_BaseBioGest::Not_Hprim:          mess = tr("-%1- Not_Hprim ").arg(QString::number(ret));           break;
               case C_BaseBioGest::SeparatorNotFound:  mess = tr("-%1- SeparatorNotFound ").arg(QString::number(ret));   break;
               case C_BaseBioGest::Error:              mess = tr("-%1- Error ").arg(QString::number(ret));               break;
               case C_BaseBioGest::Useless:            mess = tr("-%1- Useless ").arg(QString::number(ret));             break;
               case C_BaseBioGest::Analysed:           mess = tr("-%1- Analysed ").arg(QString::number(ret));            break;
               default:                                mess = tr("-%1- unknow ").arg(QString::number(ret));              break;
             }
         CApp::pCApp()->outMessage(tr("C_Dlg_BioGest::Slot_pushButton_playHprim_Clicked() analyse finished <font color=#ff5400><b>%1</b></font> code : %2 ").arg(localFileName, mess));
     }  // end for (int i = 0; i < filesList.size(); ++i)

}
//------------------------ Slot_dateEdit_Fin_dateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_dateEdit_Fin_dateChanged ( const QDate &  )
{Slot_fillPatientList();
}
//------------------------ Slot_dateEdit_Deb_dateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_dateEdit_Deb_dateChanged ( const QDate &  )
{Slot_fillPatientList();
}

//------------------------ Slot_checkBox_justPatientsWithResults_stateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_checkBox_justPatientsWithResults_stateChanged( int state)
{CApp::pCApp()->setJustPatientsWithResults( state );
 Slot_fillPatientList();
}
//------------------------ Slot_checkBox_filtrerDates_stateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_checkBox_filtrerDates_stateChanged( int state)
{CApp::pCApp()->setDateFilterEnabled( state );
 Slot_fillPatientList();
}

//------------------------ Slot_lineEdit_PatientSearch_textChanged ---------------------------------------
void C_Dlg_BioGest::Slot_lineEdit_PatientSearch_textChanged(const QString &)
{Slot_fillPatientList();
}

//------------------------ Slot_pushButton_ip_connect_Clicked ---------------------------------------
void C_Dlg_BioGest::Slot_pushButton_ip_connect_Clicked()
{CApp::pCApp()->outMessage(tr("================= C_Dlg_BioGest::Slot_pushButton_ip_connect_Clicked() ================================"));
 //..................................................... si m_pC_ScruteDB est non instancie c'est probablement .....................................................
 //                                                      lie au fait que CApp::m_pC_BaseBioGest n'a pu etre cree
 //                                                      le plus souvent hostname non valide, alors on tente une creation
 m_pC_ScruteDB->stopScrutation();
 m_pC_ScruteFTP->stopScrutation();
 m_pC_ScruteDB->setScrutationInterval(m_pGUI->lineEdit_ip_scrutation_time->text().toInt());
 if (CApp::pCApp()->getDB()==0 )
    { if (CApp::pCApp()->createDb()==false)   // on tente de creer la base de données
         CApp::pCApp()->outMessage(tr("°           C_Dlg_BioGest::Slot_pushButton_ip_connect_Clicked() <font color=#ff0000><b>Error: database is always not instancied. </b></font>"));
     else
         CApp::pCApp()->outMessage(tr("°           C_Dlg_BioGest::Slot_pushButton_ip_connect_Clicked() OK database is connected"));
    }
 else
    {CApp::pCApp()->getDB()->database().close();
     bool ret = CApp::pCApp()->connectDb();
     if (ret)
        {  CApp::pCApp()->outMessage(tr("°           C_Dlg_BioGest::Slot_pushButton_ip_connect_Clicked() OK database is connected"));
        }
     else
        {  CApp::pCApp()->outMessage(tr("°           C_Dlg_BioGest::Slot_pushButton_ip_connect_Clicked() <font color=#ff0000><b>Error: database is always not instancied. </b></font>")) ;
        }
    }
 m_pC_ScruteDB->startScrutation();
 m_pC_ScruteFTP->startScrutation();
}

//------------------------ Slot_fillPatientList ---------------------------------------
void C_Dlg_BioGest::Slot_fillPatientList()
{if (m_pC_ScruteDB==0) return;
 m_pC_ScruteDB->scruteEtTraiteLaBaseDeDonnee(1);   // 1 pour effacer la liste des patients
}
//------------------------ Slot_fill_CPatientMap ---------------------------------------
int C_Dlg_BioGest::Slot_fill_CPatientMap(QMap <QString,CPatient> &mapPk_Cpatient)
{return CApp::pCApp()->getDB()->fill_CPatientMap(mapPk_Cpatient,
                                          m_pGUI->dateEdit_Deb->date().toString("yyyyMMdd") ,
                                          m_pGUI->dateEdit_Fin->date().toString("yyyyMMdd") ,
                                          m_pGUI->lineEdit_PatientSearch->text(),
                                          m_pGUI->comboBox_Service->currentText()
                                         );
}
//---------------------------------------------- Slot_comboBox_Service_Activated -----------------------------------------------------------------------
void C_Dlg_BioGest::Slot_comboBox_Service_Activated(const QString &text )
{Slot_fillPatientList();
 CApp::pCApp()->setdefaultService(text);
}

//---------------------------------------------- Slot_pushButtonDateDeb_Clicked -----------------------------------------------------------------------
void C_Dlg_BioGest::Slot_pushButtonDateDeb_Clicked ()
{
    //............... recuperer la date de ce calendrier .................................................
    QDate   dateIn = m_pGUI->dateEdit_Deb->date();
    //............... lancer le calendrier .................................................
    QString date   = CApp::pCApp()->execCalendrier(dateIn); date = date.remove('-');
    QDate dateNew  = QDate::fromString(date, "ddMMyyyy");
    //....................... analyse du retour ..........................................................................
    if (dateNew.isValid () && dateNew != dateIn )
       {m_pGUI->dateEdit_Deb->setDate ( dateNew );
       }
    Slot_fillPatientList();
}
//---------------------------------------------- Slot_pushButtonDateFin_Clicked -----------------------------------------------------------------------
void C_Dlg_BioGest::Slot_pushButtonDateFin_Clicked ()
{
    //............... recuperer la date de ce calendrier .................................................
    QDate   dateIn = m_pGUI->dateEdit_Fin->date();
    //............... lancer le calendrier .................................................
    QString date   = CApp::pCApp()->execCalendrier(dateIn); date = date.remove('-');
    QDate dateNew  = QDate::fromString(date, "ddMMyyyy");
    //....................... analyse du retour ..........................................................................
    if (dateNew.isValid () && dateNew != dateIn )
       {m_pGUI->dateEdit_Fin->setDate ( dateNew );
       }
    Slot_fillPatientList();
}
//------------------------ Slot_checkBox_ftp_distantsDelete_stateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_checkBox_ftp_distantsDelete_stateChanged( int state)
{   QString etat = (state==Qt::Checked)?"ok":"no";
    CApp::pCApp()->writeParam("biogest session", "EffacementDistant", etat.toAscii());
    CApp::pCApp()->updateIniParamToDisk();
    if (m_pC_ScruteFTP) m_pC_ScruteFTP->setDistantsFileBeErasedState((state==Qt::Checked));
}

//------------------------ Slot_checkBox_ftp_localsDelete_stateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_checkBox_ftp_localsDelete_stateChanged( int state)
{   QString etat = (state==Qt::Checked)?"ok":"no";
    CApp::pCApp()->writeParam("biogest session", "EffacementLocal", etat.toAscii());
    CApp::pCApp()->updateIniParamToDisk();
    if (m_pC_ScruteFTP) m_pC_ScruteFTP->setLocalsFileBeErasedState((state==Qt::Checked));
}

//------------------------ Slot_checkBox_traceEnabled_stateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_checkBox_traceEnabled_stateChanged( int state)
{m_pGUI->textEdit_Monitor->setEnabled(state==Qt::Checked);
 QString etat = (state==Qt::Checked)?"ok":"no";
 CApp::pCApp()->writeParam("biogest session", "LogFTP", etat.toAscii());
 CApp::pCApp()->updateIniParamToDisk();
}

//------------------------ Slot_checkBox_ftp_daemon_enabled_stateChanged ---------------------------------------
void C_Dlg_BioGest::Slot_checkBox_ftp_daemon_enabled_stateChanged( int state)
{
         m_pGUI->lineEdit_ftp_scrutation_time->setEnabled(state==Qt::Checked);
         m_pGUI->pushButton_ftp_fav_add->setEnabled(state==Qt::Checked);
         m_pGUI->pushButton_ftp_fav_del->setEnabled(state==Qt::Checked);
         m_pGUI->comboBox_ftp_fav->setEnabled(state==Qt::Checked);
         m_pGUI->lineEdit_ftp_distantDir->setEnabled(state==Qt::Checked);
         m_pGUI->lineEdit_ftp_password->setEnabled(state==Qt::Checked);
         m_pGUI->lineEdit_ftp_login->setEnabled(state==Qt::Checked);
         m_pGUI->lineEdit_ftp_port->setEnabled(state==Qt::Checked);
         m_pGUI->lineEdit_ftp_hostname->setEnabled(state==Qt::Checked);
         m_pGUI->checkBox_ftp_distantsDelete->setEnabled(state==Qt::Checked);
         m_pGUI->checkBox_ftp_localsDelete->setEnabled(state==Qt::Checked);
         //................. to hide when ftp on/of .................
         m_pGUI->gridLayout_ftp_fav->setEnabled(state==Qt::Checked);
         m_pGUI->label_ftp_hostname->setEnabled(state==Qt::Checked);
         m_pGUI->label_ftp_port->setEnabled(state==Qt::Checked);
         m_pGUI->label_ftp_login->setEnabled(state==Qt::Checked);
         m_pGUI->label_ftp_password->setEnabled(state==Qt::Checked);
         m_pGUI->label_ftp_distant_dir->setEnabled(state==Qt::Checked);
         m_pGUI->label_ftp_scrutation->setEnabled(state==Qt::Checked);
         m_FtpStateIndicator->setHidden(state!=Qt::Checked);
         CApp::pCApp()->setFtpDaemonEnabled(state);
         if ( m_pC_ScruteFTP )
            { if (state==Qt::Checked)
                 {m_pC_ScruteFTP->startScrutation();
                 }
              else
                {m_pC_ScruteFTP->stopScrutation();
                 m_FtpStateIndicator->setLedColor(C_ClickableLed::Gray);
                }
            }
}

//------------------------ Slot_DB_AnalyseFinished ---------------------------------------
void C_Dlg_BioGest::Slot_DB_AnalyseFinished(const QString &message)
{ if (message.length())
     {CApp::pCApp()->outMessage(message);
      if (isMinimized()) CApp::pCApp()->CouCou(C_Utils_Log::removeHtmlTag(tr("Nouveaux résultats\ndisponibles")),Theme::getPath()+"PopupLabo.png",-1);
     }
  else
    CApp::pCApp()->outMessage(tr("no change to display"));
}

//------------------------ Slot_CouCouDestroy ---------------------------------------
void C_Dlg_BioGest::Slot_CouCouDestroy()
{ showNormal();
}

//------------------------ Slot_FilesAnalyseFinished ---------------------------------------
void C_Dlg_BioGest::Slot_FilesAnalyseFinished()
{
}

//------------------------ Slot_treeWidget_Patients_itemClicked ---------------------------------------
void C_Dlg_BioGest::Slot_treeWidget_Patients_itemClicked( QTreeWidgetItem *pQTreeWidgetItem, int)
{if (pQTreeWidgetItem==0) return;
 getHtmlResultsFromPatientPk(pQTreeWidgetItem->text(7));
 if ( m_pC_ScruteDB ) pQTreeWidgetItem->setIcon(0, m_pC_ScruteDB->m_IsAlreadyView);
}

//------------------------ getHtmlResultsFromPatientPk ---------------------------------------
QString C_Dlg_BioGest::getHtmlResultsFromPatientPk(const QString & pk)
{   QString     modele = CApp::pCApp()->getTemplate();
    QString   hprimTxt = "";
    QString htmlResult = "";
    QStringList serviceList = CApp::pCApp()->getProfilsList("_Service");
    QStringList examList;
    int nb = CApp::pCApp()->getDB()->getExamensListFromPatient(pk , examList);
    if (nb)
       { htmlResult = CApp::pCApp()->getDB()->FusionneModeleWithResultList(pk, examList, modele, serviceList);
         for (int i = 0; i < examList.size(); ++i)
             {hprimTxt  += QString("\r\n") + examList.at(i);
             }
       }
    m_pGUI->textEdit_hprim->setText(hprimTxt);
    m_pGUI->webView_HPRIM->setHtml(htmlResult);
    return htmlResult;
}

//------------------------ Slot_treeWidget_Patients_itemDoubleClicked ---------------------------------------
void C_Dlg_BioGest::Slot_treeWidget_Patients_itemDoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int)
{   if (pQTreeWidgetItem==0) return;
    QString pathLastCopy= CApp::pCApp()->readUniqueParam("biogest session", "PathLastCopy");
    if (QDir::isRelativePath(pathLastCopy))  pathLastCopy = QDir::cleanPath ( pathLastCopy.prepend(CApp::pCApp()->pathAppli()) );
    QString result = getHtmlResultsFromPatientPk( pQTreeWidgetItem->text(7) );
    CGestIni::Param_UpdateToDisk(pathLastCopy, result);
    showMinimized ();
}

//------------------------ Slot_treeWidget_Patients_itemSelectionChanged ---------------------------------------
void C_Dlg_BioGest::Slot_treeWidget_Patients_itemSelectionChanged ()
{   QList<QTreeWidgetItem *>  listItem = m_pGUI->treeWidget_Patients->selectedItems();
    if (listItem.size()) Slot_treeWidget_Patients_itemClicked( listItem[0],0);
}

//------------------------ Slot_treeWidget_Patients_customContextMenuRequested ---------------------------------------
void C_Dlg_BioGest::Slot_treeWidget_Patients_customContextMenuRequested (const QPoint &)
{QString ret = CApp::pCApp()->DoPopupList(m_servicesList,":",1);
 if (ret.length()==0) return;
 int p = ret.indexOf(':');
 if (p!=-1) ret = ret.mid(p+1).trimmed();

 QList<QTreeWidgetItem *>  listItem = m_pGUI->treeWidget_Patients->selectedItems();
 if (listItem.size())
    { QTreeWidgetItem *pQTreeWidgetItem = listItem[0];
      if ( CApp::pCApp()->getDB()->setService(pQTreeWidgetItem->text(7), ret ) )
         {pQTreeWidgetItem->setText(8,ret);
          m_pC_ScruteDB->setService( pQTreeWidgetItem->text(7), ret);
          Slot_treeWidget_Patients_itemClicked( pQTreeWidgetItem,0);
          //............ si pas meme service que celui affiche dans la combo faut effacer le pQTreeWidgetItem ........
          if ( m_pGUI->comboBox_Service->currentIndex())
             {QString text = m_pGUI->comboBox_Service->currentText();
              p = text.indexOf(':');
              if (p!=-1) text = text.mid(p+1).trimmed();
              if (text != ret) delete pQTreeWidgetItem;
             }
         }
    }
}
