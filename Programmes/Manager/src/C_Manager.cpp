/********************************* C_Manager.cpp *********************************
*  #include "C_Manager.h"                                                        *
* ...............................................................................*
*   Project   :  MedinTux  (typographie made in france)                          *
*   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
*   by        :  Sevin Roland     from   MedinTux project                        *
*   E-Mail    : roland-sevin@medintux.org                                        *
*   Web site  : www.medintux.org                                                 *
* ...............................................................................*
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
*   License (GPL compatible)                                                     *
*                              http://www.cecill.info/                           *
*   as published by :                                                            *
*                                                                                *
*   Commissariat \303\240 l'Energie Atomique                                     *
*   - CEA,                                                                       *
*              31-33 rue de la F\303\251d\303\251ration, 75752 PARIS cedex 15.   *
*                            FRANCE                                              *
*   Centre National de la Recherche Scientifique                                 *
*   - CNRS,                                                                      *
*                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
*                            FRANCE                                              *
*   Institut National de Recherche en Informatique et en Automatique             *
*   - INRIA,                                                                     *
*                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
*                            FRANCE                                              *
*                                                                                *
*         This program is distributed in the hope that it will be useful,        *
*         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
*         CeCILL  License (GPL compatible) for more details.                     *
*                                                                                *
**********************************************************************************/

#include "C_Manager.h"
#include "ui_C_Manager.h"

#include "CApp.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"
#include "../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h"
#include "../../MedinTuxTools-QT4/Agenda/C_Dlg_Agenda.h"
#include "../../MedinTuxTools-QT4/C_DragQTreeWidget/C_DragQTreeWidget.h"
#include "../../MedinTuxTools-QT4/UserList/C_Frm_UserList.h"
#include "../../MedinTuxTools-QT4/C_Dlg_DoList/C_Dlg_DoList.h"
#include "../../MedinTuxTools-QT4/CHtmlTools.h"
#include "../../SesamVitale-QT4/C_Vitale.h"
#include "C_Dlg_Assure.h"

#include <QString>
#include <QStringList>
#include <QAction>
#include <QTextStream>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QProcess>
#include <QHeaderView>
#include <QPixmap>
#include <QComboBox>
#include <QKeyEvent>
#include <QWhatsThis>
#include <QCursor>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QBoxLayout>
#include <QFrame>
#include <QProcess>
#include <QFileInfo>
#include <QSqlError>
#include <QLineEdit>
#include <QUrl>
#include <QtWebKit/QWebView>
#include <QWebSettings>
#include <QHostInfo>
#include <QPrintDialog>
#include <QPrinter>
#include <QInputDialog>
#include <QFontDialog>
#include <QSettings>                    //Cz_Pyxvital
#include <QDebug>
#include <QPainter>
#include <QPaintEngine>
#include <QAbstractButton>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QBrush>
#include <QPalette>
#include <QColor>
#include <QStyleOptionProgressBar>

#define MODE_SELECTION_PATIENT 0
#define MODE_CREATION_PATIENT  1
#define MODE_MULTICRITERE      2

//#define SESAM_VERSION
#define AGENDA_SUB_TITLE_H    20
#define W_DATE_EDIT           70
#define ID_INTERV              4
#define HEIGHT_USER_AGENDA_LABEL              20
//====================================== C_KeyPressControl ==========================================================
/*
Qt::Key_0	0x30
Qt::Key_1	0x31
Qt::Key_2	0x32
Qt::Key_3	0x33
Qt::Key_4	0x34
Qt::Key_5	0x35
Qt::Key_6	0x36
Qt::Key_7	0x37
Qt::Key_8	0x38
Qt::Key_9	0x39
*/
/*
bool C_KeyPressControl::eventFilter(QObject *obj, QEvent *event)
{if (event->type() == QEvent::KeyRelease)
    {
     QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
     int key = keyEvent->key();
     qDebug("Ate key press %d", key);
     //QEvent my_event(QEvent::KeyPress);
     switch(key)
     {
       case Qt::Key_Ampersand:  key = Qt::Key_1; break;
       case Qt::Key_Eacute:     key = Qt::Key_2; break;
       case Qt::Key_QuoteDbl:   key = Qt::Key_3; break;
       case Qt::Key_Apostrophe: key = Qt::Key_4; break;
       case Qt::Key_ParenLeft:  key = Qt::Key_5; break;
       case Qt::Key_section:    key = Qt::Key_6; break;
       case Qt::Key_Egrave:     key = Qt::Key_7; break;
       case Qt::Key_Exclam:     key = Qt::Key_8; break;
       case Qt::Key_cedilla:    key = Qt::Key_9; break;
       case Qt::Key_Aacute:     key = Qt::Key_0; break;
     }
     //QKeyEvent ( QEvent::KeyPress,  key, keyEvent->modifiers(), const QString & text = QString(), bool autorep = false, ushort count = 1 )

     //QKeyEvent myKeyEvent( QEvent::KeyRelease,  key, keyEvent->modifiers(),"1");
     //return QObject::eventFilter(obj, &myKeyEvent);
     keyEvent->setModifiers(Qt::ShiftModifier);
     return QObject::eventFilter(obj, keyEvent);
     //return true;
    }
    else  if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        qDebug("Ate key press %d", key);
        //QEvent my_event(QEvent::KeyPress);
        switch(key)
        {
          case Qt::Key_Ampersand:  key = Qt::Key_1; break;
          case Qt::Key_Eacute:     key = Qt::Key_2; break;
          case Qt::Key_QuoteDbl:   key = Qt::Key_3; break;
          case Qt::Key_Apostrophe: key = Qt::Key_4; break;
          case Qt::Key_ParenLeft:  key = Qt::Key_5; break;
          case Qt::Key_section:    key = Qt::Key_6; break;
          case Qt::Key_Egrave:     key = Qt::Key_7; break;
          case Qt::Key_Exclam:     key = Qt::Key_8; break;
          case Qt::Key_cedilla:    key = Qt::Key_9; break;
          case Qt::Key_Aacute:     key = Qt::Key_0; break;
        }
        //QKeyEvent ( QEvent::KeyPress,  key, keyEvent->modifiers(), const QString & text = QString(), bool autorep = false, ushort count = 1 )

        //QKeyEvent myKeyEvent( QEvent::KeyPress,  key, keyEvent->modifiers(),"1");
        //return QObject::eventFilter(obj, &myKeyEvent);
        keyEvent->setModifiers(Qt::ShiftModifier);
        return QObject::eventFilter(obj, keyEvent);

        //return true;
    } else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
*/

//====================================== C_Manager ==========================================================
//--------------------------------------------------------- C_Manager ---------------------------------------
C_Manager::C_Manager(CMoteurBase *pCMoteurBase,  QWidget *parent, const QString & name)
   : QMainWindow(parent, name), m_pGUI(new Ui::C_ManagerClass), m_pCMoteurBase(pCMoteurBase)
{
   if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising Widgets and menus........."), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
   m_Apropos_Proc      = 0;
   m_List_GUI_Mode[0]  = "MODE_SELECTION_PATIENT";
   m_List_GUI_Mode[1]  = "MODE_CREATION_PATIENT";
   m_List_GUI_Mode[2]  = "MODE_MULTICRITERE";
   m_pGUI->setupUi(this);
   m_pGUI->textEdit_Changements->hide(); // juste present pour contenir le texte des changements
   m_pC_Frm_UserList     = 0;
   m_Ident_IsModified    = 0;
   m_Contacts_Run        = FALSE;
   m_NomadismeToolBar    = 0;
   m_oruCriticalWait     = 4;
   m_pListUpdateTimer    = 0;
   m_FSEenCours          = FALSE;            //Cz_Pyxvital
   m_timerFSE            = 0;                //Cz_Pyxvital
   m_Facture_Seule       = "non";
   m_strGotoAgenda       = tr("  >> Goto Schedule              ");
   m_strGotoPatientList  = tr("  >> Goto Patient list          ");
   m_oruContextActivated = 0;                                                                                                 // rs has
   m_HasLapCompatibility = 1;                                                                                                 // rs has
   m_SexControl          = 1;                                                                                                 // rs has
   if (G_pCApp->readUniqueParam("Manager session", "Compatibility Lap Has").lower().left(1)=="n") m_HasLapCompatibility = 0;  // rs has
   if (G_pCApp->readUniqueParam("Manager session", "Sex control").lower().left(1)=="n")           m_SexControl          = 0;  // rs has
   m_Facture_Seule       = G_pCApp->readUniqueParam("Sesam-Vitale", "Facture_Seule");                                               // Cz_Pyxvital

   // ................ listview patient (promotion de C_DragQTreeWidget)  .................................
   m_pGUI->listView_Patient->setSortingEnabled(true);
   m_pGUI->listView_Patient->setMimeType("text/medintux_rdv_drag");
   QHeaderView *pQHeaderView = m_pGUI->listView_Patient->header();     // cacher les colonnes du Pk et GUID
   //pQHeaderView->hideSection (2);
   //pQHeaderView->hideSection (3);
   m_pGUI->listView_Patient->setAlternatingRowColors ( TRUE );
   m_pGUI->listView_Patient->setContextMenuPolicy ( Qt::CustomContextMenu );  //Qt::CustomContextMenu
   //m_pGUI->listView_Patient->setDragEnabled (TRUE );                           // ne surtout pas activer car active les mecanismes de la classe de base
   //m_pGUI->listView_Patient->setDragDropMode ( QAbstractItemView::DragOnly );  // et c'est le boxon

   //qDebug(QHostInfo::localHostName ()+"."+QHostInfo::localDomainName ());
   //QString background_color = "#1200ff";
   //QString styles           = QString("background-color: %1; border: 1px solid #8f8f91; border-radius: 6px; font-size: 9px;").arg(background_color);
   //QString styles           = QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 white, stop: 0.4 #00e4ff, stop:1 #007eff); border: 1px solid #8f8f91; border-radius: 6px; font-size: 9px;");
   QString styles           = QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 white, stop: 0.4 #00e4ff, stop:1 #007eff); border: 1px solid #8f8f91; border-radius: 1px; font-size: 9px;");
   //..............................//////// MENUS ////////.................................................
   //................................. options de menus ....................
   m_actionRecordWindowsPos = new QAction(this);
   m_actionRecordWindowsPos->setObjectName(QString::fromUtf8("actionRecordWindowsPos"));
   m_actionSetGlobalFont = new QAction(this);
   m_actionSetGlobalFont->setObjectName(QString::fromUtf8("actionSetGlobalFont"));
   m_actionHideShowLogo  = new QAction(this);
   m_actionHideShowLogo->setObjectName(QString::fromUtf8("actionHideShowLogo"));
   //m_actionRecordWindowsPos->setText(QApplication::translate("C_ManagerClass", "Enregistrer la position des fen\303\252tres", 0, QApplication::UnicodeUTF8));

   m_action_ModeNomade = new QAction(this);
   m_action_ModeNomade->setObjectName(QString::fromUtf8("action_ModeNomade"));
   //m_action_ModeNomade->setText(QApplication::translate("C_ManagerClass", "Nomadisme", 0, QApplication::UnicodeUTF8));

   m_action_ToModeConnect = new QAction(this);
   m_action_ToModeConnect->setObjectName(QString::fromUtf8("action_ToModeConnect"));
   //m_action_ToModeConnect->setText(QApplication::translate("C_ManagerClass", "J'arrive et me reconnecte au serveur", 0, QApplication::UnicodeUTF8));

   m_action_UserParams = new QAction(this);
   m_action_UserParams->setObjectName(QString::fromUtf8("action_GestionProfils"));

   m_action_DebloquerBases = new QAction(this);
   m_action_DebloquerBases->setObjectName(QString::fromUtf8("action_DebloquerBases"));
   //m_action_DebloquerBases->setText(QApplication::translate("C_ManagerClass", "D\303\251bloquer les bases et les verrous", 0, QApplication::UnicodeUTF8));

   m_action_AccesWebMedTux = new QAction(this);
   m_action_AccesWebMedTux->setObjectName(QString::fromUtf8("action_AccesWebMedTux"));

   m_actionImprimer = new QAction(this);
   m_actionImprimer->setObjectName(QString::fromUtf8("actionImprimer"));
   m_actionImprimer->setShortcutContext(Qt::ApplicationShortcut);

   // CZB
   m_actionSpecifique = new QAction(this);
   m_actionSpecifique->setObjectName(QString::fromUtf8("actionSpecifique"));
   m_actionSpecifique->setShortcutContext(Qt::ApplicationShortcut);


   m_actionQuitter = new QAction(this);
   m_actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
   m_actionQuitter->setShortcutContext(Qt::ApplicationShortcut);


   m_action_NouveauDossier = new QAction(this);
   m_action_NouveauDossier->setObjectName(QString::fromUtf8("actionNouveauDossier"));
   m_action_NouveauDossier->setShortcutContext(Qt::ApplicationShortcut);
   //m_actionQuitter->setShortcut(QApplication::translate("C_ManagerClass", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
   //m_actionQuitter->setText(QApplication::translate("C_ManagerClass", "Quitter", 0, QApplication::UnicodeUTF8));

   //..........//////// barre de menus ////////....................
#ifdef  Q_WS_MAC
   m_menuBar = new QMenuBar(0);
#else
   m_menuBar = new QMenuBar(this);
#endif
   m_menuBar->setObjectName(QString::fromUtf8("menuBar"));
   m_menuBar->setGeometry(QRect(0, 0, 1015, 23));
   //................. menu fichier ..........................
   m_menuFichiers = new QMenu(m_menuBar);
   m_menuFichiers->setObjectName(QString::fromUtf8("menuFichiers"));
   //m_menuFichiers->setTitle(QApplication::translate("C_ManagerClass", "Fichiers", 0, QApplication::UnicodeUTF8));
   m_menuFichiers->addAction(m_action_AccesWebMedTux);
   m_menuFichiers->addSeparator();
   m_menuFichiers->addAction(m_action_NouveauDossier);
   m_menuFichiers->addSeparator();
   m_menuFichiers->addAction(m_action_ModeNomade);
   m_menuFichiers->addAction(m_action_DebloquerBases);
   m_menuFichiers->addAction(m_action_UserParams);
   m_menuFichiers->addSeparator();
   m_menuFichiers->addAction(m_actionImprimer);
   m_menuFichiers->addSeparator();
   m_menuFichiers->addAction(m_actionSpecifique);       // CZB
   m_menuFichiers->addSeparator();                      // CZB
   m_menuFichiers->addAction(m_actionQuitter);
   m_menuFichiers->addSeparator();
   m_menuBar->addAction(m_menuFichiers->menuAction());

   m_pGUI->webView_MedWebTux_h->hide();

   m_webView_MedWebTux = new C_QWebView(m_pGUI->tab_Web);
   m_webView_MedWebTux->setObjectName(QString::fromUtf8("webView_MedWebTux"));
   m_webView_MedWebTux->setUrl(QUrl("about:blank"));
   m_pGUI->gridLayout_2->addWidget(m_webView_MedWebTux, 0, 0, 1, 1);

   //.................. menu Affichage et fenetres ................................................
   m_menuFenetre = new QMenu(m_menuBar);
   m_menuFenetre->addAction(m_actionRecordWindowsPos);
   m_menuFenetre->addAction(m_actionSetGlobalFont);
   m_menuFenetre->addAction(m_actionHideShowLogo);
   m_action_dockMenu = m_menuFenetre->insertMenu ( m_actionRecordWindowsPos, createPopupMenu () );
   m_menuBar->addAction(m_menuFenetre->menuAction());

   //................... outils onglet web ..........................................
   m_action_WebPrint = new QAction(this);
   m_action_WebPrint->setObjectName(QString::fromUtf8("action_WebPrint"));
   m_action_WebPrint->setIconSet ( Theme::getIcon("22x22/fileprint.png") ) ;

   m_action_WebHelp = new QAction(this);
   m_action_WebHelp->setObjectName(QString::fromUtf8("action_WebHelp"));
   m_action_WebHelp->setIconSet ( Theme::getIcon("22x22/help.png") ) ;

   m_action_WebHome = new QAction(this);
   m_action_WebHome->setObjectName(QString::fromUtf8("action_WebHome"));
   m_action_WebHome->setIconSet ( Theme::getIcon("22x22/WebMedTux.png") ) ;

   m_action_WebFind = new QAction(this);
   m_action_WebFind->setObjectName(QString::fromUtf8("action_WebFind"));
   m_action_WebFind->setIconSet ( Theme::getIcon("22x22/edit-find.png") ) ;

   m_action_WebStop = new QAction(this);
   m_action_WebStop->setObjectName(QString::fromUtf8("action_WebStop"));
   m_action_WebStop->setIcon( m_webView_MedWebTux->pageAction(QWebPage::Stop)->icon() ) ;


   m_locationEdit = new QLineEdit(this);
   m_locationEdit->setSizePolicy(QSizePolicy::Expanding, m_locationEdit->sizePolicy().verticalPolicy());

   m_WebFindEdit = new QLineEdit(this);
   m_WebFindEdit->setSizePolicy(QSizePolicy::Expanding, m_locationEdit->sizePolicy().verticalPolicy());
   m_WebToolBar = addToolBar(tr("Browsing"));

   //..........//////// navigateur internet ////////....................
   QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,TRUE);
   QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,TRUE);
   QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
   QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
   QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
   QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);

   m_WebToolBar->addAction(m_webView_MedWebTux->pageAction(QWebPage::Back));
   m_WebToolBar->addAction(m_webView_MedWebTux->pageAction(QWebPage::Forward));
   m_WebToolBar->addAction(m_webView_MedWebTux->pageAction(QWebPage::Reload));

   m_WebToolBar->addAction(m_action_WebStop);
   m_WebToolBar->addAction(m_action_WebPrint);
   m_WebToolBar->addAction(m_action_WebHome);
   m_WebToolBar->addAction(m_action_WebHelp);
   m_WebToolBar->addWidget(m_locationEdit);
   m_WebToolBar->addAction(m_action_WebFind);
   m_WebToolBar->addWidget(m_WebFindEdit);
   // SIGEMS_DEB ...............................................................
   m_pGUI->listView_Sigems->hide();
   m_pSuspendEntrantsTimer = 0;

#ifdef ENTREES_SIGEMS
   // recup des parametres dans Manager.ini
   QString  m_delayEntrantsTimer;
   m_Entrants_Sigems            = G_pCApp->readUniqueParam ( "Sigems", "Gestion des entrants").toUpper();
   m_delayEntrantsTimer         = G_pCApp->readUniqueParam ( "Sigems", "Timer actualisation liste entrants en secondes");
   m_delaySuspendEntrantsTimer  = G_pCApp->readUniqueParam ( "Sigems", "Timer attente actualisation liste entrants en secondes");
   if (m_delaySuspendEntrantsTimer == "") m_delaySuspendEntrantsTimer = m_delayEntrantsTimer;
   m_Nb_Heures_Derniers_Entrants= G_pCApp->readUniqueParam ( "Sigems", "Nombre heures derniers entrants");
   m_Heure_Deb_Main_Courante    = G_pCApp->readUniqueParam ( "Sigems", "Heure debut main courrante (hh:mm)");

   // mise en forme de la lisView_Sigems
   m_pGUI->listView_Sigems->setContextMenuPolicy ( Qt::CustomContextMenu );
   if (m_Entrants_Sigems == "OUI")
       {// lancement du timer d'actualisation de la listView des entrants
       m_pGUI->listView_Sigems->show();
       m_pGUI->listView_Sigems->hideColumn(9);
       m_pGUI->listView_Sigems->hideColumn(10);

       m_pListUpdateTimer       = new QTimer(this);
       if (m_pListUpdateTimer)
          {m_pListUpdateTimer->start (m_delayEntrantsTimer.toInt() * 1000 );
           connect( m_pListUpdateTimer, SIGNAL(timeout()), this, SLOT(Slot_OnTimerEntrants()) );

          }
       }
#endif // fin ENTREES_SIGEMS
   // SIGEMS_FIN ...............................................................
   //...................... menu a propos ............................
   m_menuInfo        = new QMenu(m_menuBar);
   m_action_A_Propos = new QAction(this);
   m_action_A_Propos->setObjectName(QString::fromUtf8("actionApropos"));
   m_action_Aide     = new QAction(this);
   m_action_Aide->setObjectName(QString::fromUtf8("action_Aide"));
   m_menuInfo->addAction(m_action_A_Propos);
   m_menuInfo->addAction(m_action_Aide);
   m_menuBar->addAction(m_menuInfo->menuAction());

   m_actionTypeAffichage = new QAction (this);                                  // CZA
   m_actionTypeAffichage->setObjectName(QString::fromUtf8("type_Affichage"));   // CZA
   m_actionTypeAffichage->setText("Agenda");                                    // CZA
   //m_menuBar->addAction(m_actionTypeAffichage);                                 // CZA
   //........ on cree une fausse barre de menu pour occuper le terrain .........................
   /*
   QMenuBar *dumy_menuBar = new QMenuBar(this);
   dumy_menuBar->setGeometry(QRect(200, 0, 1015, 23));
   QMenu    *dumy_menu    = new QMenu(dumy_menuBar);
   QAction  *dumy_action  = new QAction(this);
   dumy_action->setObjectName(QString::fromUtf8("actionApropos"));
   dumy_action->setText(QApplication::translate("C_ManagerClass", "Quit", 0, QApplication::UnicodeUTF8));
   dumy_menuBar->addAction(dumy_menu->menuAction());
   dumy_menu->addAction(dumy_action);
   setMenuBar(dumy_menuBar);
   */
   //setMenuBar(m_menuBar);
   //............. on place notre bouton a gauche ................
   m_Button_Affichage_EnCours = new Wdg_ButtonPtr(this, "Button_affichageEnCours");
   m_Button_Affichage_EnCours->move(0 /*m_menuBar->width()*/ ,0);
   //m_Button_Affichage_EnCours->setMaximumHeight(m_menuBar->height());
   //........ on  place la vraie barre de menu la ou l'on veut .........................
   // m_menuBar->move(m_Button_Affichage_EnCours->width(),0);
   setMenuBar(m_menuBar);

   //...........//////// DOCK de la liste des utilisateurs ////////.........................................
   m_pC_Frm_UserList = new C_Frm_UserList(m_pCMoteurBase, this);
   QLabel * wdg_labelDocWidget_Title_User = new QLabel(this);
   wdg_labelDocWidget_Title_User->setStyleSheet(styles);
   wdg_labelDocWidget_Title_User->setText(tr("User list"));
   wdg_labelDocWidget_Title_User->setAlignment ( Qt::AlignHCenter|Qt::AlignVCenter );
   m_pGUI->wdg_DockWidget_User->setTitleBarWidget ( wdg_labelDocWidget_Title_User );
   m_pGUI->wdg_DockWidget_User->setWidget(m_pC_Frm_UserList);

   //..........////////.DOCK de l'Agenda le logo de DMD ////////.........................................
   m_pGUI->textLabelPixMap->setPixmap ( Theme::getIcon("DMD-Logo-03.png") );

   //...........  Titre l'agenda ........................................................
   m_AgendaDockTitle = new QLabel(this);
   m_AgendaDockTitle->setStyleSheet(styles);
   m_AgendaDockTitle->setText(tr("Schedule"));
   m_AgendaDockTitle->setAlignment ( Qt::AlignHCenter|Qt::AlignVCenter );
   m_pGUI->wdg_DockWidget_Agenda->setTitleBarWidget ( m_AgendaDockTitle );
   //........... combobox ajouter un agenda utilisateur....................
   if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising Agendas........."), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
   initComboBoxAgendaUser();
   //............ liste des jours ....................
   m_pAgendaQLayout = new QHBoxLayout ( m_pGUI->frame_MultiAgenda );
   m_pAgendaQLayout->setSpacing(3);
   m_pAgendaQLayout->setMargin(0);
   m_pAgendaQLayout->setObjectName(QString::fromUtf8("m_pAgendaQLayout"));
   m_pAgendaQLayout->setContentsMargins(0, 0, 0, 0);

   setComboBoxOnValue(m_pGUI->comboBoxAgendaUser, G_pCApp->m_SignUser);
   //........................ customiser quelques widgets ..............................
   pQHeaderView = m_pGUI->listView_Doublons->header();                 // cacher les colonnes du Pk et GUID
   pQHeaderView->hideSection (2);
   pQHeaderView->hideSection (3);
   m_pGUI->listView_Doublons->setContextMenuPolicy ( Qt::CustomContextMenu );
   m_pGUI->listView_Doublons->hide();

   pQHeaderView = m_pGUI->listView_Vitale->header();                   // cacher les colonnes du Pk et GUID
   pQHeaderView->hideSection (2);
   pQHeaderView->hideSection (3);
   pQHeaderView->hideSection (4);
   pQHeaderView->hideSection (5);
   pQHeaderView->hideSection (6);
   pQHeaderView->hideSection (7);
   pQHeaderView->hideSection (8);

   m_pGUI->listView_Vitale->setContextMenuPolicy ( Qt::CustomContextMenu );
   m_pGUI->listView_Vitale->hide();

   pQHeaderView = m_pGUI->listView_Praticiens->header();               // cacher colonne ID
   pQHeaderView->hideSection (4);
   pQHeaderView->resizeSection ( 0,100 );
   m_pGUI->listView_Praticiens->setContextMenuPolicy ( Qt::CustomContextMenu );
   QList <int> list;                                                   // reduire le spliter des listes
   list.append(140);
   list.append(width()-140);
   m_pGUI->splitter_SepListIdent->setSizes(list);
   //................ bouton de creation de dossier ......................
   /*
   QString style =        "QPushButton {"
                          "border: 2px solid #8f8f91;"
                          "border-radius: 6px;"
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                          "color: red;"
                          "font-weight: bold;"
                          "min-width: 80px;}"
                          "QPushButton:pressed {"
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}";
   m_pGUI->pushButtonNew->setStyleSheet(style);
   m_pGUI->pushButtonCreate->setStyleSheet(style);
   m_pGUI->pushButtonCancelCreate->setStyleSheet(style);
   */
   m_pGUI->pushButtonMenuNew->setPixmap ( Theme::getIcon("BoutonNewWithout.png") );
   /*
   m_ButtonNew_PopMenu = new QMenu ( tr("New folder"),  m_pGUI->pushButtonMenuNew );
   if (m_ButtonNew_PopMenu)
      {m_ButtonNew_PopMenu->addAction ( tr("New folder, using existing data"), this, SLOT(Slot_NewDossWithoutErase()), QKeySequence (Qt::CTRL + Qt::Key_P) );
       m_pGUI->pushButtonMenuNew->setMenu ( m_ButtonNew_PopMenu );
      }
   */
   //.............................. verifier si PA sigems doit etre active ...........................................

   m_pGUI->pushButton_SigemsPA->setIcon ( QIcon (Theme::getIcon( "22x22/InterSigems.png"))) ;
   if (QDir().exists(G_pCApp->m_PathSigemsPA)) m_pGUI->pushButton_SigemsPA->show();
   else                                        m_pGUI->pushButton_SigemsPA->hide();

   //................. positionner les masques de saisie ..........................
   m_pGUI->lineEdit_DtNss->setInputMask ("99-99-9999");
   m_pGUI->lineEdit_DtNss_Assure->setInputMask ("99-99-9999");
   m_pGUI->lineEdit_Tel1->setInputMask ("99 99 99 99 99 99 99");
   m_pGUI->lineEdit_Tel2->setInputMask ("99 99 99 99 99 99 99");
   m_pGUI->lineEdit_Tel3->setInputMask ("99 99 99 99 99 99 99");
   m_pGUI->lineEdit_NumSecu->setInputMask ("9 99 99 99 999 999");    //2  30  19  62  965  231
   m_pGUI->lineEdit_CdPostal->setInputMask ("99 999");
   m_pGUI->lineEdit_RangGeme->setInputMask ("99");

   //C_KeyPressControl *pC_KeyPressControl = new C_KeyPressControl(this);
   //m_pGUI->lineEdit_DtNss->installEventFilter(pC_KeyPressControl);

   //........................ images de la carte vitale ...............................
   QPixmap px = Theme::getIcon("UserList/Carte_CPS_Butt.png");
   m_pGUI->pushButtonVitale->setIcon        (px );
   m_pGUI->pushButtonVitale->setIconSize(QSize(px.width(),px.height()));
   m_pGUI->pushButtonVitale->setIcon ( QIcon (Theme::getIcon( "VitaleButt.png"))) ;
   //QRect rect = m_pGUI->pushButtonNew->geometry();
   //m_pGUI->pushButtonNew->setMaximumHeight(m_pGUI->pushButtonVitale->height());
   //m_pGUI->pushButtonNew->setAttribute(Qt::WA_MacBrushedMetal);
   //.................... initialiser la liste patients ...............................
   m_pGUI->pushButton_ReinitListe->setIcon ( Theme::getIcon("listview_renouv.png") );
   m_pGUI->pushButtonDetruire->setIcon ( Theme::getIcon("EffaceDossier.png") );
   m_pGUI->pushButtonInfo->setIcon ( Theme::getIcon("help.png") );

if (G_pCApp->readUniqueParam ( "Sesam-Vitale", "ModuleName")!= "PYXVITAL")
   {m_pGUI->pushButtonVitaleMenu->hide();
   }
 else
  {
   px = Theme::getIcon("VitaleButtMenu.png");
   m_pGUI->pushButtonVitaleMenu->setIcon        (px );
   m_pGUI->pushButtonVitaleMenu->setIconSize(QSize(px.width(),px.height()));
   m_pGUI->pushButtonVitaleMenu->setIcon ( QIcon (px)) ;
  }

 //.................. remplir la liste des patients......................
   if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising patients list........."), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
   initListePatient("","");
   //.................. masquer le bouton Solde Patient si on a pas la compta......................         CZ_Cpta
   if (G_pCApp->readUniqueParam ( "Comptabilite", "Controle solde").toLower() != "oui")
       m_pGUI->pushButton_solde->hide();

   //.................. recuperer la qualite des ayants droits......................
   QString param;
   CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli() + "Ressources/QualiteBeneficiaire.tbl" , param);
   param = param.trimmed();
   param = param.remove('\r');
   param = param.replace(',',' ');
   m_pGUI->comboBoxQualiteAyantDroit->clear();
   QStringList lst = param.split('\n', QString::SkipEmptyParts);
   if (lst.count())
      { lst.removeFirst ();  // virer les lignes de commentaires
        lst.removeFirst ();
      }
   else
      {qDebug()<<tr("Ressources/QualiteBeneficiaire.tbl not found");
      }
   if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising widgets connexions........."), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
   m_pGUI->comboBoxQualiteAyantDroit->insertItems(0, lst);    //Qt::CaseSensitivity
   //........................ connecter les slot de verification ...........................................................................................
   connect( m_pGUI->lineEdit_DtNss,    SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),         this,     SLOT(   Slot_OutFocuslineEdit_DtNss(QFocusEvent*, int &)));
   connect( m_pGUI->lineEditVille,     SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),         this,     SLOT(   Slot_OutFocusVille(QFocusEvent*, int &))  );
   connect( m_pGUI->lineEdit_CdPostal, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),         this,     SLOT(   Slot_OutFocusCodePostal(QFocusEvent*, int &))  );
   connect( m_pGUI->lineEdit_CdPostal, SIGNAL( Sign_keyPressEvent(QKeyEvent *, int &)),          this,     SLOT(   Slot_CharEventCodePostal(QKeyEvent *, int &)) );

   connect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_keyPressEvent(QKeyEvent *, int &)),           this,     SLOT(   Slot_CharEventNumSecu(QKeyEvent *, int &)) );
   connect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_focusInEvent(QFocusEvent*, int &)),           this,     SLOT(   Slot_InFocusNumSecu(QFocusEvent*, int &))  );
   connect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),          this,     SLOT(   Slot_OutFocusNumSecu(QFocusEvent*, int &))  );
   //........................ connecter les slot de changement ............................................................................................
   connect( m_pGUI->lineEdit_NomDossier,         SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomDossier(const QString & )    ));
   connect( m_pGUI->lineEdit_PrenomDossier,      SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_PrenomDossier(const QString & ) ));
   connect( m_pGUI->lineEdit_NomDeNss,           SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomDeNss(const QString & )      ));
   connect( m_pGUI->lineEdit_DtNss,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_DtNss(const QString & )         ));
   connect( m_pGUI->lineEdit_DtNss_Assure,       SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_DtNss_Assure(const QString &)   ));
   connect( m_pGUI->lineEdit_Titre,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Titre(const QString & )         ));
   connect( m_pGUI->lineEdit_Profession,         SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Profession(const QString &)     ));
   connect( m_pGUI->textEdit_Adresse,            SIGNAL( textChanged ( ) ),                        this, SLOT(   Slot_TextChanged_textEdit_Adresse( ))  );
   connect( m_pGUI->lineEdit_Tel1,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel1(const QString & ))  );
   connect( m_pGUI->lineEdit_Tel2,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel2(const QString & ))  );
   connect( m_pGUI->lineEdit_Tel3,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel3(const QString & ))  );
   connect( m_pGUI->lineEdit_CdPostal,           SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_CdPostal(const QString & ))  );
   connect( m_pGUI->lineEditVille,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEditVille(const QString & )) );
   connect( m_pGUI->lineEdit_Email,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Email(const QString & )) );
   connect( m_pGUI->lineEdit_NumSecu,            SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NumSecu(const QString & )) );
   connect( m_pGUI->lineEdit_NomAssure,          SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomAssure(const QString & ))  );
   connect( m_pGUI->lineEdit_PrenomAssure,       SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_PrenomAssure(const QString & ))  );
   connect( m_pGUI->textEdit_Note,               SIGNAL( textChanged ( ) ),                        this, SLOT(   Slot_TextChanged_textEdit_Note( )) );
   connect( m_pGUI->lineEdit_RangGeme,           SIGNAL( textChanged (const QString & ) ),         this, SLOT(   Slot_TextChanged_lineEdit_RangGeme(const QString &)) );
   connect( m_pGUI->comboBoxSexe,                SIGNAL( currentIndexChanged (const QString & ) ), this, SLOT(   Slot_TextChanged_comboBoxSexe( const QString &))  );
   connect( m_pGUI->comboBoxQualiteAyantDroit,   SIGNAL( currentIndexChanged (const QString & ) ), this, SLOT(   Slot_TextChanged_comboBoxQualiteAyantDroit( const QString &))  );
   connect( m_pGUI->comboBoxAgendaUser,          SIGNAL( activated( const QString & )),            this, SLOT(   Slot_comboBoxAgendaUser_activated( const QString& )) );
   connect(m_pGUI->buttonBox_agendas,            SIGNAL( clicked(QAbstractButton *)),              this, SLOT(   Slot_OnFastAccesAgendaButtonClicked(QAbstractButton *)) );        // CZA

   //................... connexions ....................................................
   connect( m_pC_Frm_UserList,                SIGNAL(Sign_UserSelected( QTreeWidgetItem * , QTreeWidgetItem * )),       this, SLOT(Slot_UserSelected_InListUser( QTreeWidgetItem * , QTreeWidgetItem *)) );
   connect( m_pC_Frm_UserList,                SIGNAL(Sign_UserMustBeEdited( const QString&, const QString& )),          this, SLOT(Slot_EditUser( const QString&, const QString&)) );
   connect( m_action_UserParams,              SIGNAL(triggered()),                                                      this, SLOT(Slot_pushButton_action_UserParams()) );

   connect( m_pC_Frm_UserList,                SIGNAL(Sign_UserMustBeUnloged(int& )),                                    this, SLOT(Slot_UserMustBeUnloged(int&)) );
   connect( m_pGUI->listView_Praticiens,      SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                   this, SLOT(Slot_listView_Praticiens_DoubleClicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Praticiens ,     SIGNAL(customContextMenuRequested( const QPoint &)),                      this, SLOT(Slot_listView_Praticiens_contextMenuRequested (const QPoint &)) );
   connect( m_pGUI->pushButtonAcceder,        SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonAcceder_Clicked()) );
   connect( m_pGUI->pushButton_AddMedTTT,     SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_AddMedTTT_clicked()) );
   connect( m_pGUI->pushButton_AddMedTTT_Fast,SIGNAL(clicked ()),                                                       this, SLOT(Slot_pushButton_AddMedTTT_Fast_clicked ()) );
   connect( m_pGUI->pushButton_DelMedTTT,     SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_DelMedTTT_clicked()) );
   connect( m_pGUI->pushButtonCreate,         SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonCreate()) );
   connect( m_pGUI->pushButtonSavModif,       SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonSavModifWithoutConfirmation()) );
   connect( m_pGUI->pushButton_MultiCritere,  SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_MultiCritere()) );
   connect( m_pGUI->pushBut_MultiCritFind,    SIGNAL(clicked()),                                                        this, SLOT(Slot_pushBut_MultiCritFind()) );
   connect( m_pGUI->pushButton_Effacer,       SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_Effacer_clicked()) );
   connect( m_pGUI->pushBut_ExitMultiCritere, SIGNAL(clicked()),                                                        this, SLOT(Slot_pushBut_ExitMultiCritere()) );
   connect( m_pGUI->pushButtonInfo,           SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonInfoClicked( )) );
   connect( m_pGUI->pushButtonDetruire,       SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonDetruire_clicked( )) );
   connect( m_pGUI->pushButtonNew,            SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonNew_clicked( )) );
   connect( m_pGUI->pushButtonMenuNew,        SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonMenuNew_clicked( )) );
   connect( m_pGUI->pushButtonCancelCreate,   SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonCancelCreate_clicked( )) );
   connect( m_pGUI->lineEditAutoLocator,      SIGNAL(textChanged(const QString &)),                                     this, SLOT(Slot_lineEditAutoLocator_textChanged(const QString &)) );
   connect( m_pGUI->lineEditAutoLocator,      SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &)),                           this, SLOT(Slot_lineEditAutoLocator_keyPressEvent(QKeyEvent *, int &)) );

   connect( m_pGUI->pushButtonVitale,         SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButtonVitale_clicked()) );

   connect( m_pGUI->pushButton_ReinitListe,   SIGNAL(clicked()),                                                        this, SLOT(Slot_pushButton_ReinitListe_clicked()) );
   connect( m_pGUI->listView_Patient,         SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                         this, SLOT(Slot_listView_Patient_Clicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Patient,         SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                   this, SLOT(Slot_listView_Patient_DoubleClicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Patient ,        SIGNAL(itemSelectionChanged ()),                                          this, SLOT(Slot_ListView_itemSelectionChanged ()) );
   connect( m_pGUI->listView_Patient ,        SIGNAL(customContextMenuRequested( const QPoint &)),                      this, SLOT(Slot_ListView_ContextMenuRequested (const QPoint &)) );
   connect( m_pGUI->listView_Patient,         SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )),                   this, SLOT(Slot_giveDraglistView_PatientItemData(QString &, QTreeWidgetItem*)));

   connect( m_pGUI->listView_Doublons ,       SIGNAL(itemSelectionChanged ()),                                          this, SLOT(Slot_listView_DoublonsSelectionChanged ()) );
   connect( m_pGUI->listView_Doublons ,       SIGNAL(customContextMenuRequested( const QPoint &)),                      this, SLOT(Slot_listView_Doublons_contextMenuRequested (const QPoint &)) );
   connect( m_pGUI->listView_Vitale,          SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                         this, SLOT(Slot_listView_Vitale_Clicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Vitale,          SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                   this, SLOT(Slot_listView_Vitale_DoubleClicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Vitale ,         SIGNAL(itemSelectionChanged ()),                                          this, SLOT(Slot_listView_VitaleSelectionChanged ()) );
   connect( m_pGUI->listView_Vitale ,         SIGNAL(customContextMenuRequested( const QPoint &)),                      this, SLOT(Slot_listView_Vitale_ContextMenuRequested (const QPoint &)) );

   // SIGEMS_DEB ------------------------------------------------------------------
   connect( m_pGUI->listView_Sigems,          SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                      this, SLOT(Slot_listView_Sigems_Clicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Sigems,          SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                this, SLOT(Slot_listView_Sigems_DoubleClicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->listView_Sigems ,         SIGNAL(itemSelectionChanged ()),                                       this, SLOT(Slot_listView_SigemsSelectionChanged ()) );
   connect( m_pGUI->listView_Sigems ,         SIGNAL(customContextMenuRequested( const QPoint &)),                   this, SLOT(Slot_listView_Sigems_ContextMenuRequested (const QPoint &)) );
   // SIGEMS_FIN ------------------------------------------------------------------

   connect( m_actionRecordWindowsPos,         SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionRecordWindowsPos_triggered (bool)) );
   connect( m_actionSetGlobalFont,            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionSetGlobalFont_triggered (bool)) );
   connect( m_actionHideShowLogo,             SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionHideShowLogo_triggered (bool)) );
   connect( m_action_DebloquerBases,          SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_action_DebloquerBases (bool)) );
   connect( m_actionImprimer,                 SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_action_Imprimer (bool)) );

   connect( m_actionQuitter,                  SIGNAL(triggered ()),                                                  G_pCApp, SLOT(quit()) );   // revient faire le menage par le signal  Sign_QuitterRequired
   connect(G_pCApp,                           SIGNAL(Sign_QuitterRequired()),                                        this, SLOT(Slot_SauverLesMeubles()));
   connect( m_action_A_Propos,                SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionApropos (bool)) );
   connect( m_action_Aide,                    SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_action_Aide (bool)) );

   connect( m_pGUI->pushButton_FSE,           SIGNAL(clicked()),                                                     this, SLOT(Slot_pushButton_FSE()) );                                      //Cz_Pyxvital
   connect( m_pGUI->pushButtonVitale,         SIGNAL(customContextMenuRequested( const QPoint &)),                   this, SLOT(Slot_Button_Vitale_ContextMenuRequested (const QPoint &)) );   //Cz_Pyxvital
   connect( m_pGUI->pushButtonVitaleMenu,     SIGNAL(clicked()),                                                     this, SLOT(Slot_Button_Vitale_MenuRequested ()) );

   connect( m_pGUI->tabWidget_Central,        SIGNAL(currentChanged ( int  )),                                       this, SLOT(Slot_tabWidget_Central_currentChanged (int)) );
   connect( m_locationEdit,                   SIGNAL(returnPressed()),                                               this, SLOT(Slot_LocationEdit_returnPressed()));
   connect( m_webView_MedWebTux,              SIGNAL(loadStarted (  )),                                              this, SLOT(Slot_On_webView_MedWebTux_loadStarted ()) );
   connect( m_webView_MedWebTux,              SIGNAL(loadProgress (int)),                                            this, SLOT(Slot_On_webView_MedWebTux_loadProgress (int)) );
   connect( m_webView_MedWebTux,              SIGNAL(loadFinished (bool  )),                                         this, SLOT(Slot_On_webView_MedWebTux_loadFinished (bool)) );
   connect( m_WebFindEdit,                    SIGNAL(returnPressed()),                                               this, SLOT(Slot_WebFindEdit_returnPressed()));
   connect( m_WebFindEdit,                    SIGNAL(textChanged ( const QString &  ) ),                             this, SLOT(Slot_WebFindEdit_textChanged (const QString & )) );
   connect( m_webView_MedWebTux,              SIGNAL(urlChanged ( const QUrl &  )),                                  this, SLOT(Slot_webView_MedWebTux_urlChanged ( const QUrl &  )));
   connect( m_action_WebPrint,                SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionWebPrint (bool)) );
   connect( m_action_WebHome,                 SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionWebHome (bool)) );
   connect( m_action_WebHelp,                 SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionWebHelp (bool)) );
   connect( m_action_WebFind,                 SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionWebFind (bool)) );
   connect( m_action_WebStop,                 SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionWebStop (bool)) );
   connect( m_action_AccesWebMedTux,          SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_action_AccesWebMedTux (bool)) );
   connect( m_action_NouveauDossier,          SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionMenuNew_clicked( bool)) );
   // test saisie reglt direct CZ_Cpta.
   connect( m_pGUI->pushButton_solde,         SIGNAL(clicked()),                                                     this, SLOT(Slot_Saisie_Reglement ()) );
   connect( m_pGUI->pushButton_SigemsPA,      SIGNAL(clicked()),                                                     this, SLOT(Slot_pushButton_SigemsPA_Clicked()) );
   connect( m_Button_Affichage_EnCours,       SIGNAL(clicked()),                                                     this, SLOT(Slot_Type_Affichage_Change()) );
   connect( m_actionSpecifique,               SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_launchSpecificJob (bool)) );

   //ORU_DEB ------------------------------------------------------------------
   connect( m_pGUI->treeWidget_oru,          SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                      this, SLOT(Slot_listView_Oru_Clicked( QTreeWidgetItem * , int)) );
   connect( m_pGUI->treeWidget_oru,          SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                this, SLOT(Slot_listView_Oru_DoubleClicked( QTreeWidgetItem * , int)) );
   //connect( m_pGUI->listView_Sigems ,         SIGNAL(itemSelectionChanged ()),                                       this, SLOT(Slot_listView_SigemsSelectionChanged ()) );
   connect( m_pGUI->treeWidget_oru ,         SIGNAL(customContextMenuRequested( const QPoint &)),                   this, SLOT(Slot_listView_Oru_ContextMenuRequested (const QPoint &)) );


   //....................... nomadisme ....................................................
   if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising nomad status........."), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
   if (G_pCApp->m_IsGestionNomadisme)
      {m_NomadismeToolBar = addToolBar("NomadisemetoolBar"); //new QToolBar(this);
       m_NomadismeToolBar->setObjectName(QString::fromUtf8("m_NomadismeToolBar"));
       m_NomadismeToolBar->addAction(m_action_ModeNomade);         // ajouter l'option a la barre d'outils
       //addToolBar(Qt::TopToolBarArea, m_NomadismeToolBar);         // ajouter la barre d'outils a la fenetre principale
       if (G_pCApp->m_IsNomadeActif)
          {m_action_ModeNomade->setText(tr("Quit itinerant mode and reconnect to main server"));
           m_action_ModeNomade->setIconSet ( Theme::getIcon("32x32/unconnected.png") ) ;
           G_pCApp->CouCou(tr("Itinerant Mode Active "));
          }
       else
          {m_action_ModeNomade->setText(tr("Disconnect from main server and activate itinerant mode"));
           m_action_ModeNomade->setIconSet ( Theme::getIcon("32x32/connected.png") ) ;
           G_pCApp->CouCou(tr("Master mode active"));
          }
       m_NomadismeToolBar->addAction(m_action_ModeNomade);
       connect( m_action_ModeNomade,        SIGNAL(triggered ( bool )),                                               this, SLOT(Slot_action_ModeConnect_triggered (bool)) );
      // connect( m_pGUI->action_ToModeConnect,     SIGNAL(triggered ( bool )),                                               this, SLOT(Slot_action_ToModeConnect_triggered (bool)) );
     }
  else
     {m_action_ModeNomade->setText(tr("Itinerant mode managing not active"));
      m_action_ModeNomade->setVisible ( FALSE );
     }
  testInterface();
   //.......................... tenir compte de la version .............................
#ifdef SESAM_VERSION
   connect( m_pC_Frm_UserList,    SIGNAL(Sign_CPS_IsClicked()), this, SLOT(Slot_CPS_IsClicked()) );
#endif
   //................... position sur l'ecran ....................................................

   QString val1, val2, val3, val4;
   if (G_pCApp->readParam ("ManagerPos", "Positions", &val1, &val2, &val3, &val4)==QString::null)  // zero = pas d'erreur
      {move(val1.toInt(),   val2.toInt());
       resize(val3.toInt(), val4.toInt());
      }
   else
      {QRect rect =  QApplication::desktop()->availableGeometry();   // juste pour faire bouger les choses ajustement du bas)
       setGeometry ( rect );
      }

   setInterfaceOnProfil(MODE_SELECTION_PATIENT);
   if (G_pCApp->readUniqueParam ( "Agenda", "Affichage logo Data Medical Design").toLower()== "non")  // CZA
      {m_pGUI->textLabelPixMap->hide();
      }
   get_RecordDispos(G_pCApp->readUniqueParam ( "Agenda", "Affichage Liste ou Agenda"));
   if (m_pGUI->wdg_DockWidget_Agenda->isHidden()) m_AgendaInitialVisbility   = 0;
   else                                           m_AgendaInitialVisbility   = 1;
   if (m_pGUI->wdg_DockWidget_User->isHidden())   m_UserListInitialVisbility = 0;
   else                                           m_UserListInitialVisbility = 1;
   //.............. positionner le premier focus ...................................................
   setIdentModifiedState(FALSE); // car pas d'identite selectionne
   setIdentiteDisabled();        // car pas d'identite selectionne
   setVerrou(FALSE);             // car pas d'identite ni lancee
   if (!G_pCApp->m_IsAppStartWithUser)
      {m_pGUI->tabWidget_Central->setEnabled(FALSE);
       m_pGUI->wdg_DockWidget_Agenda->setEnabled(FALSE);
       m_pGUI->wdg_DockWidget_Agenda->hide();
       m_pGUI->wdg_DockWidget_User->show();
      }
   else         setMedWebTuxOnUser();
   m_pGUI->lineEditAutoLocator->setFocus();
   //G_pCApp->CouCou("Coucou", Theme::getPath(Theme::WithSeparator)+"MessagePopVigie.png");
   //CGestIni::Param_UpdateToDisk("/home/ro/Tabstyle.txt", G_pCApp->styleSheet ());
   m_pGUI->pushButton_AddMedTTT_Fast->setIcon ( QIcon (Theme::getIcon( "16x16/Left.png" ))) ;;
   m_pMasterSocket = 0;
   m_pSlaveSocket  = 0;
   #ifdef  Q_WS_MAC
       statusBar()->addWidget(m_Button_Affichage_EnCours);
   #else
       statusBar()->hide();
   #endif

   setTitle();
   G_pCApp->setWindowIcon (Theme::getIcon("32x32/Manager.png") );
   //...................... timer de verification p\303\251riodique ...............................
   m_pReconnectTimer       = new QTimer(this);
   if (m_pReconnectTimer)
      {m_pReconnectTimer->start (3000);
       connect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(Slot_OnTimer()) );
      }
   demarrerAccessoires();
   Slot_retranslateUi();
   m_pGUI->tabWidget_Central->setCurrentIndex (0);
   Slot_tabWidget_Central_currentChanged (0);
   //setUnifiedTitleAndToolBarOnMac(true);  // pas bon car les toolbars ne sont pas deplacables
   //............. placer les fontes de caractere sur les widgets .........
   initWidgetsList();
   setAllWidgetsOnFont(G_pCApp->m_GuiFont);
   //................. ceration des agendas ......................................................
   //                   teste si plusieurs agendas ouverts au lancement .
   QString          listagendas = G_pCApp->readUniqueParam ( "Agenda", "Liste des agendas a ouvrir");
   QStringList listeUsersAgenda = listagendas.split(';',QString::SkipEmptyParts);;
   bool     isCurrentUserInList = false;
   if (listeUsersAgenda.count() > 0)
      {for (int i = 0; i < listeUsersAgenda.count(); i++)
           {QString user = listeUsersAgenda[i].trimmed();
            bool forceUserToBeUsed = user.startsWith('+');
            user = user.remove('+');
            if ( isCurrentUserInList==false && user == G_pCApp->m_SignUser) isCurrentUserInList = true;
            if ( forceUserToBeUsed || m_pGUI->comboBoxAgendaUser->findText (user) != -1 )
               { //if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising users '%1' agendas.........").arg(user), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
                 addUserAgenda(user, QDate::currentDate());
               }
           }
      }
   if ( ! isCurrentUserInList )
      {//if (G_pCApp->m_pQSplashScreen)G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising users '%1' agendas.........").arg(G_pCApp->m_SignUser), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
       addUserAgenda(G_pCApp->m_SignUser, QDate::currentDate());
      }
   if (G_pCApp->readUniqueParam ( "Agenda", "Affichage logo Data Medical Design").toLower()== "non")  // CZA
      {m_pGUI->textLabelPixMap->hide();
      }
   //.................. le truc de l'ORU Paca ...................................
   m_pathOru = G_pCApp->readUniqueParam(  "ORU", "path");
   if (m_pathOru.length() && G_pCApp->isSigemsValid())
      {  if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising Oru paca list........."), Qt::AlignCenter | Qt::AlignCenter, Qt::black);
         if ( QDir::isRelativePath ( m_pathOru ) )  m_pathOru = QDir::cleanDirPath (m_pathOru.prepend(G_pCApp->pathAppli()));
         m_oruCriticalWait =  G_pCApp->readUniqueParam(  "ORU", "critical wait").toInt();
         m_oruMethodIsSQL  = (G_pCApp->readUniqueParam(  "ORU", "method").toUpper()[0]!='N');
         if (m_oruCriticalWait==0) m_oruCriticalWait = 4;

         QString coul = G_pCApp->readUniqueParam(  "ORU", "listColor");
         if ( coul.length()==0 ) coul = "#ff7821";
         QColor   col(coul);
         QPalette palette;
         QBrush brush(col);
         brush.setStyle(Qt::SolidPattern);
         palette.setBrush(QPalette::Active,   QPalette::Base, brush);
         palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
         QBrush brush1(QColor(255, 255, 255, 255));
         brush1.setStyle(Qt::SolidPattern);
         palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
         QHeaderView *pQHeaderView = m_pGUI->treeWidget_oru->header();     // cacher les colonnes du Pk et GUID
         pQHeaderView->hideSection (3);
         pQHeaderView->hideSection (4);
         pQHeaderView->hideSection (6);
         pQHeaderView->hideSection (7);
         pQHeaderView->hideSection (8);
         pQHeaderView->hideSection (9);
         pQHeaderView->hideSection (10);
         pQHeaderView->hideSection (11);
         //m_pGUI->treeWidget_oru->setAlternatingRowColors ( TRUE );
         m_pGUI->treeWidget_oru->setPalette(palette);
         m_pGUI->treeWidget_oru->sortItems (4, Qt::AscendingOrder );
         m_pGUI->treeWidget_oru->setContextMenuPolicy ( Qt::CustomContextMenu );

         C_LevelOruItemDelegate *pC_LevelItemDelegate = new C_LevelOruItemDelegate(m_pGUI->treeWidget_oru);
         m_pGUI->treeWidget_oru->setItemDelegate(pC_LevelItemDelegate);

         GetOruList( m_pGUI->treeWidget_oru );

         int period = G_pCApp->readUniqueParam(  "ORU", "actualisation en secondes").toInt();
         if (period==0)  period = 30;
         period = qMax(15,period);
         if ( m_pListUpdateTimer==0 )                              // si le timer (sert a l'ORU et SIGEMS) n'a pas ete active par SIGEMS (sigems non activeee)
            {  m_pListUpdateTimer  = new QTimer(this);            // alors on l'active pour l'ORU
               if ( m_pListUpdateTimer )
                  { m_pListUpdateTimer->start (period * 1000 );
                    connect( m_pListUpdateTimer, SIGNAL(timeout()), this, SLOT(Slot_GetOruList()) );    // Slot_OnTimerEntrants()
                  }
            }
      }
   else
      {  m_pGUI->treeWidget_oru->hide();
      }
}

//------------------------ ~C_Manager ---------------------------------------
C_Manager::~C_Manager()
{   if (m_pReconnectTimer)
      {m_pReconnectTimer->stop();
       disconnect( m_pReconnectTimer, SIGNAL(timeout()), this, SLOT(Slot_OnTimer()) );
      }
   if (m_pMasterSocket) delete m_pMasterSocket;
   if (m_pSlaveSocket)  delete m_pSlaveSocket;
   delete m_pGUI;
}

//------------------------ setAllWidgetsOnFont ---------------------------------------
void C_Manager::setAllWidgetsOnFont(const QFont &font)
{for (int i = 0; i < G_pCApp->m_widgetList.size(); ++i)
    { G_pCApp->m_widgetList.at(i)->setFont(font);
      //qDebug() << G_pCApp->m_widgetList.at(i)->name();
    }
 //............ ne pas oublier les boutons d'acces rapide aux  agendas .................................
 QList<QAbstractButton *> buttonsList =	m_pGUI->buttonBox_agendas->buttons();
 for (int i=0; i<buttonsList.size(); ++i)
     {buttonsList[i]->setFont(font);
     }
 //............ mention speciale pour le bouton de bascule agenda .................................
 SetModeOn_Button_Affichage_EnCours(m_Type_Affichage_EnCours);
}
//------------------------ initWidgetsList ---------------------------------------
void C_Manager::initWidgetsList()
{
 G_pCApp->m_widgetList.append(m_pGUI->wdg_DockWidget_Agenda);
 G_pCApp->m_widgetList.append(m_pGUI->wdg_DockWidget_User);
 G_pCApp->m_widgetList.append(m_pGUI->comboBoxAgendaUser);
 G_pCApp->m_widgetList.append(m_pGUI->labelAgendaSelect);
 G_pCApp->m_widgetList.append(m_pGUI->textLabelPixMap);
 G_pCApp->m_widgetList.append(m_pGUI->frame_MultiAgenda);
 G_pCApp->m_widgetList.append(m_pGUI->tabWidget_Central);
 G_pCApp->m_widgetList.append(m_pGUI->groupBoxPatient);
 G_pCApp->m_widgetList.append(m_pGUI->label_nom);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_NomDossier);
 G_pCApp->m_widgetList.append(m_pGUI->label_prenom);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_PrenomDossier);
 G_pCApp->m_widgetList.append(m_pGUI->label_nomnss);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_NomDeNss);
 G_pCApp->m_widgetList.append(m_pGUI->label_sexe);
 G_pCApp->m_widgetList.append(m_pGUI->comboBoxSexe);
 G_pCApp->m_widgetList.append(m_pGUI->label_titre);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_Titre);
 G_pCApp->m_widgetList.append(m_pGUI->label_dtnss);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_DtNss);
 G_pCApp->m_widgetList.append(m_pGUI->label_rangem);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_RangGeme);
 G_pCApp->m_widgetList.append(m_pGUI->label_profession);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_Profession);
 G_pCApp->m_widgetList.append(m_pGUI->label_adresse);
 G_pCApp->m_widgetList.append(m_pGUI->textEdit_Adresse);
 G_pCApp->m_widgetList.append(m_pGUI->label_tel1);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_Tel1);
 G_pCApp->m_widgetList.append(m_pGUI->label_tel2);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_Tel2);
 G_pCApp->m_widgetList.append(m_pGUI->label_tel3);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_Tel3);
 G_pCApp->m_widgetList.append(m_pGUI->label_Ville);
 G_pCApp->m_widgetList.append(m_pGUI->lineEditVille);
 G_pCApp->m_widgetList.append(m_pGUI->label_codepostal);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_CdPostal);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_SigemsPA);
 G_pCApp->m_widgetList.append(m_pGUI->label_email);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_Email);
 G_pCApp->m_widgetList.append(m_pGUI->groupBoxAssure);
 G_pCApp->m_widgetList.append(m_pGUI->label_nss);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_NumSecu);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_NssClef);
 G_pCApp->m_widgetList.append(m_pGUI->label_beneficiaire);
 G_pCApp->m_widgetList.append(m_pGUI->comboBoxQualiteAyantDroit);
 G_pCApp->m_widgetList.append(m_pGUI->label_nomassure);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_NomAssure);
 G_pCApp->m_widgetList.append(m_pGUI->label_prenomassure);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_PrenomAssure);
 G_pCApp->m_widgetList.append(m_pGUI->label_dnssassure);
 G_pCApp->m_widgetList.append(m_pGUI->lineEdit_DtNss_Assure);
 G_pCApp->m_widgetList.append(m_pGUI->tabWidgetNotes);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_AddMedTTT);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_AddMedTTT_Fast);
 G_pCApp->m_widgetList.append(m_pGUI->listView_Praticiens);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_DelMedTTT);
 G_pCApp->m_widgetList.append(m_pGUI->pushBut_ExitMultiCritere);
 G_pCApp->m_widgetList.append(m_pGUI->pushBut_MultiCritFind);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_Effacer);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_MultiCritere);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonAcceder);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_FSE);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonCreate);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonSavModif);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonCancelCreate);
 G_pCApp->m_widgetList.append(m_pGUI->textEdit_Note);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonNew);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonMenuNew);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonVitale);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonVitaleMenu);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonDetruire);
 G_pCApp->m_widgetList.append(m_pGUI->pushButtonInfo);
 G_pCApp->m_widgetList.append(m_pGUI->listView_Vitale);
 G_pCApp->m_widgetList.append(m_pGUI->listView_Doublons);
 G_pCApp->m_widgetList.append(m_pGUI->label_rechercherliste);
 G_pCApp->m_widgetList.append(m_pGUI->pushButton_ReinitListe);
 G_pCApp->m_widgetList.append(m_pGUI->lineEditAutoLocator);
 G_pCApp->m_widgetList.append(m_pGUI->listView_Patient);
 G_pCApp->m_widgetList.append(m_pGUI->textLabel_NumGuid);
 G_pCApp->m_widgetList.append(m_pGUI->textLabel_NumDoss);

 G_pCApp->m_widgetList.append(m_Button_Affichage_EnCours);
 G_pCApp->m_widgetList.append(m_menuBar);
 G_pCApp->m_widgetList.append(m_menuFichiers);
 G_pCApp->m_widgetList.append(m_menuFenetre);
 G_pCApp->m_widgetList.append(m_menuInfo);

}
//------------------------ Slot_retranslateUi ---------------------------------------
void C_Manager::Slot_retranslateUi()
{m_actionRecordWindowsPos->setText(QApplication::translate("C_ManagerClass", "Save Windows position", 0, QApplication::UnicodeUTF8));
m_actionSetGlobalFont->setText(QApplication::translate("C_ManagerClass", "Choose a font for Manager...", 0, QApplication::UnicodeUTF8));
m_actionHideShowLogo->setText(QApplication::translate("C_ManagerClass", "Hide Show Data Medical Design Logo", 0, QApplication::UnicodeUTF8));
m_action_ModeNomade->setText(QApplication::translate("C_ManagerClass", "Itinerant mode", 0, QApplication::UnicodeUTF8));
m_action_ToModeConnect->setText(QApplication::translate("C_ManagerClass", "I come back an reconnect to the server", 0, QApplication::UnicodeUTF8));
m_action_WebPrint->setText(QApplication::translate("C_ManagerClass", "Print the contents of this page", 0, QApplication::UnicodeUTF8));
m_action_WebHome->setText(QApplication::translate("C_ManagerClass", "Access Medical data from MedWebTux", 0, QApplication::UnicodeUTF8));
m_action_WebHelp->setText(QApplication::translate("C_ManagerClass", "Access to the help and the documentation of MedinTux", 0, QApplication::UnicodeUTF8));
m_action_WebFind->setText(QApplication::translate("C_ManagerClass", "Find text entered in the right field", 0, QApplication::UnicodeUTF8));
m_action_DebloquerBases->setText(QApplication::translate("C_ManagerClass", "Remove all locks", 0, QApplication::UnicodeUTF8));
m_action_dockMenu->setText(QApplication::translate("C_ManagerClass", "Hide Show Users and Schedules", 0, QApplication::UnicodeUTF8));
m_action_NouveauDossier->setText(QApplication::translate("C_ManagerClass", "New file", 0, QApplication::UnicodeUTF8));
m_action_NouveauDossier->setShortcut(QApplication::translate("C_ManagerClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
m_actionQuitter->setText(QApplication::translate("C_ManagerClass", "Quit", 0, QApplication::UnicodeUTF8));
m_actionQuitter->setShortcut(QApplication::translate("C_ManagerClass", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
#ifdef  Q_WS_MAC
    m_menuFichiers->setTitle(QApplication::translate("C_ManagerClass", "Files", 0, QApplication::UnicodeUTF8));
#else
    {QFontMetrics fntMetric = QFontMetrics(G_pCApp->m_GuiFont);
     int               wbut = qMax(fntMetric.boundingRect(m_strGotoAgenda).width(),fntMetric.boundingRect(m_strGotoPatientList).width())+20;
     QString   spacesToAdd  = ".";
     while (fntMetric.boundingRect(spacesToAdd).width() < wbut)
           {spacesToAdd +="  ";
           }

     m_menuFichiers->setTitle(spacesToAdd + QApplication::translate("C_ManagerClass", "Files", 0, QApplication::UnicodeUTF8));
    }
#endif
m_menuFenetre->setTitle(QApplication::translate("C_ManagerClass", "Display and Windows", 0, QApplication::UnicodeUTF8));
m_action_A_Propos->setText(QApplication::translate("C_ManagerClass", "&About Manager", 0, QApplication::UnicodeUTF8));
m_menuInfo->setTitle(QApplication::translate("C_ManagerClass", "&Help", 0, QApplication::UnicodeUTF8));
m_action_AccesWebMedTux->setText(QApplication::translate("C_ManagerClass", "Access to medical file by MedWebTux", 0, QApplication::UnicodeUTF8));
m_action_Aide->setText(QApplication::translate("C_ManagerClass", "MedinTux Documentation", 0, QApplication::UnicodeUTF8));
m_action_UserParams->setText(QApplication::translate("C_ManagerClass", "Users management", 0, QApplication::UnicodeUTF8));
m_action_UserParams->setShortcut(QApplication::translate("C_ManagerClass", "Ctrl+G", 0, QApplication::UnicodeUTF8));
m_actionImprimer->setText(QApplication::translate("C_ManagerClass", "Print", 0, QApplication::UnicodeUTF8));
m_actionImprimer->setShortcut(QApplication::translate("C_ManagerClass", "Ctrl+P", 0, QApplication::UnicodeUTF8));
if (m_NomadismeToolBar) m_NomadismeToolBar->setWindowTitle(QApplication::translate("C_ManagerClass", "Tools for itinerant mode", 0, QApplication::UnicodeUTF8));
// CZB
QString titreJob;
if (G_pCApp->readParam ("Specifiques", "Menu_Files", &titreJob)==QString::null)  // zero = pas d'erreur
    m_actionSpecifique->setText(titreJob);
}
//--------------------------------- closeEvent -----------------------------------------------------------------------
void C_Manager::closeEvent(QCloseEvent *event)
{   Slot_SauverLesMeubles();
    event->accept();
    /*
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
   */
}
///////////////////////////////////////// WEB PAGE /////////////////////////////////////////////////////
//--------------------------------- Slot_On_webView_MedWebTux_loadStarted -----------------------------------------------------------------------
void C_Manager::Slot_On_webView_MedWebTux_loadStarted()
{m_pGUI->progressBarWebView->setValue(0);
 m_pGUI->progressBarWebView->show();
}
//--------------------------------- Slot_On_webView_MedWebTux_loadProgress -----------------------------------------------------------------------
void C_Manager::Slot_On_webView_MedWebTux_loadProgress(int progress)
{m_pGUI->progressBarWebView->setValue(progress);
 m_action_WebStop->setEnabled(TRUE);
}

//--------------------------------- Slot_On_webView_MedWebTux_loadFinished -----------------------------------------------------------------------
void C_Manager::Slot_On_webView_MedWebTux_loadFinished(bool)
{QString url = m_webView_MedWebTux->url().toString();
 m_pGUI->progressBarWebView->hide();
 m_action_WebStop->setEnabled(FALSE);

 if (url.indexOf("about:blank")!=-1) m_pGUI->tabWidget_Central->setCurrentIndex ( 0 );
}

//--------------------------------- Slot_actionWebStop -----------------------------------------------------------------------
void C_Manager::Slot_actionWebStop (bool)
{m_pGUI->progressBarWebView->hide();
m_webView_MedWebTux->stop();
m_action_WebStop->setEnabled(FALSE);
}

//--------------------------------- Slot_LocationEdit_returnPressed -----------------------------------------------------------------------
void C_Manager::Slot_LocationEdit_returnPressed()
{
   QUrl url = QUrl(m_locationEdit->text());
   m_webView_MedWebTux->load(url);
   m_webView_MedWebTux->setFocus();
}
//--------------------------------- Slot_WebFindEdit_returnPressed -----------------------------------------------------------------------
void C_Manager::Slot_WebFindEdit_returnPressed()
{Slot_actionWebFind (true);
}
// [Web_Browser]
//    StartPage = ../../Doc/index.html
// QString page =  G_pCApp->readParam ( "Web_Browser", "StartPage");

//--------------------------------- Slot_WebFindEdit_textChanged -----------------------------------------------------------------------
void C_Manager::Slot_WebFindEdit_textChanged (const QString &text )
{m_webView_MedWebTux->findText ("",   QWebPage::HighlightAllOccurrences );  // clear the selection
 m_webView_MedWebTux->findText (text, QWebPage::HighlightAllOccurrences );
}

//--------------------------------- Slot_actionWebFind -----------------------------------------------------------------------
void C_Manager::Slot_actionWebFind (bool)
{m_webView_MedWebTux->findText (m_WebFindEdit->text(), QWebPage::FindWrapsAroundDocument );
}

//--------------------------------- Slot_actionWebPrint -----------------------------------------------------------------------
void C_Manager::Slot_actionWebPrint (bool)
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted)
       {     m_webView_MedWebTux->print ( &printer );
       }
#endif // QT_NO_PRINTER
}
//--------------------------------- Slot_action_Aide -----------------------------------------------------------------------
void C_Manager::Slot_action_Aide (bool)
{Slot_actionWebHelp (TRUE);
m_pGUI->tabWidget_Central->setCurrentIndex ( 1 );
}

//--------------------------------- Slot_action_AccesWebMedTux -----------------------------------------------------------------------
void C_Manager::Slot_action_AccesWebMedTux (bool)
{setMedWebTuxOnUser();
m_pGUI->tabWidget_Central->setCurrentIndex ( 1 );
}

//--------------------------------- Slot_actionWebHome -----------------------------------------------------------------------
void C_Manager::Slot_actionWebHome (bool)
{setMedWebTuxOnUser();
}

//--------------------------------- Slot_actionWebHelp -----------------------------------------------------------------------
void C_Manager::Slot_actionWebHelp (bool)
{
   QString page =  "../../Doc/index.html";
   if ( QDir(page).isRelative()&& !page.startsWith("http"))
    {page.prepend(G_pCApp->pathAppli());
     page = QDir::cleanPath(page);
    }
 m_webView_MedWebTux->setUrl(QUrl(page));
 m_locationEdit->setText(page);
}
//--------------------------------- setMedWebTuxOnUser -----------------------------------------------------------------------
void C_Manager::setMedWebTuxOnUser()
{
 //   [Web_Browser]
 //       MedWebTuxUrl = http://localhost/MedWebTux/
 QString medWebTuxUrl =  G_pCApp->readUniqueParam ( "Web_Browser", "MedWebTuxUrl");
 QString cripted_password;
 G_pCApp->m_pCMoteurBase->GetUserPrimKey(G_pCApp->m_SignUser, &cripted_password   /* = 0  */);
 medWebTuxUrl = medWebTuxUrl.replace("{{HOSTNAME}}",G_pCApp->m_pCMoteurBase->m_HostName);
 QString page =                    medWebTuxUrl           +"select.php"    +
                                                          +"?adr="         + G_pCApp->m_pCMoteurBase->m_HostName
                                                          + "&loginbase="  + G_pCApp->m_pCMoteurBase->m_UserName
                                                          + "&pwd="        + G_pCApp->m_pCMoteurBase->m_PassWord
                                                          + "&login="      + G_pCApp->m_SignUser
                                                          + "&pass="       + cripted_password
                                                          + "&ID="; //+&ID=xxx
m_webView_MedWebTux->setUrl(QUrl(page));
m_locationEdit->setText(page);
}

//--------------------------------- Slot_webView_MedWebTux_urlChanged -----------------------------------------------------------------------
void C_Manager::Slot_webView_MedWebTux_urlChanged ( const QUrl &url  )
{m_locationEdit->setText(url.toString());
}

//--------------------------------- Slot_action_DebloquerBases -----------------------------------------------------------------------
void C_Manager::Slot_action_DebloquerBases (bool)
{G_pCApp->m_pCMoteurBase->DeVerrouilleTout();
}

///////////////////////////////////////// ACCESSOIRE ///////////////////////////////////////////////////
//---------------------------------------------- demarrerAccessoires -----------------------------------------------------------------------
void C_Manager::demarrerAccessoires()
{
 //.................. recuperer la liste des programmes Accessoires a demarrer en meme temps ................................................
 //                                              et les lancer
 // [Accessoires]
 // QLaboFTP = ../../QLaboFTP/bin/QLaboFTP
QStringList accessoiresList;
G_pCApp->getParamList(tr("Accessories"), "", accessoiresList );
if (accessoiresList.count())
   {for ( QStringList::Iterator it = accessoiresList.begin(); it != accessoiresList.end(); ++it )
        { demarrerUnAccessoire(*it);
        }
   }
}

//---------------------------------------------- demarrerUnAccessoire -----------------------------------------------------------------------
void C_Manager::demarrerUnAccessoire(QString accessoire_path)
{
QStringList argList;
QString  pathExe = QDir::cleanDirPath (QFileInfo (qApp->argv()[0]).dirPath (true)) + "/" + accessoire_path.trimmed();
#ifdef Q_WS_WIN
   pathExe +=".exe";
#endif
#ifdef Q_OS_MACX
   int pos = pathExe.lastIndexOf("/");
   if (pos != -1) pathExe = pathExe+".app/Contents/MacOS/"+pathExe.mid(pos+1);
#else
#endif
QProcess::startDetached (pathExe, argList);
}

//--------------------------------- Slot_action_DebloquerBases -----------------------------------------------------------------------
void C_Manager::Slot_tabWidget_Central_currentChanged (int index)
{if (index==0)
   {m_WebToolBar->hide();
   }
else
   {m_WebToolBar->show();
   }
}

//--------------------------------- Slot_SauverLesMeubles --------------------------------------------------------------------------------
void C_Manager::Slot_SauverLesMeubles()
{tryToStopAPropos();
}

//----------------------------------- Slot_actionApropos -----------------------------------------------------------------------
void C_Manager::Slot_actionApropos(bool)
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}

//------------------------ Slot_Apropos_Proc_finished -----------------------------------------
void C_Manager::Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)
{if (m_Apropos_Proc)
    {m_Apropos_Proc->terminate();
     m_Apropos_Proc->waitForFinished (5000);
     delete m_Apropos_Proc;
     m_Apropos_Proc = 0;
     m_action_A_Propos->setDisabled(FALSE);
    }
}
//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void C_Manager::Slot_actionAproposDisplay()
{       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QString macAdr;
        QString ipAdr  = G_pCApp->get_Current_IP_Adr(&macAdr);

        CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli()+"Ressources/Changements.html",m_pGUI->textEdit_Changements->toHtml()
                                                                                        .replace("{{IP_ADR}}",  ipAdr)
                                                                                        .replace("{{MAC_ADR}}", macAdr)
                                                                                        .replace("{{PATH_INI}}",G_pCApp->pathIni()));
        QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).path());
        //QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).path())+"Ressources/";
        QStringList argList;

        //......................... completer les autres arguments .........................................
        QString dataBaseVersion = "";
        m_pCMoteurBase->GetMedinTuxNormalisedVersion(dataBaseVersion, ".");
        argList << "Manager";                                                                  // 1  nom du module
        argList << tr("Schedule management and patient list module");                          // 2  description courte
        argList << G_pCApp->ApplicationAndQtVersion(tr("%1:%2<br/>").arg(__DATE__,__TIME__));  // 3  numero de version
        argList << G_pCApp->pathAppli()+"Ressources/Changements.html";                         // 4  fichiers d?crivant les changements
        argList << Theme::getPath(Theme::WithSeparator)+"32x32/Manager.png";                   // 5  Icone du programme
        argList << QDir::cleanPath(G_pCApp->pathAppli()+"../../Doc/index.html") ;              // 6  aide en ligne
        argList << "";                                                                         // 7  apropos (on met une chaine vide pour qu'il prenne celui par dï¿½faut)
        argList << dataBaseVersion ;                                                           // 8  version de la base de donnee
        //QProcess::startDetached (pathExeAPropos, argList);

        if (m_Apropos_Proc==0)
           {m_action_A_Propos->setDisabled(TRUE);
            m_Apropos_Proc = new QProcess(this);
            connect( m_Apropos_Proc, SIGNAL(finished ( int,  QProcess::ExitStatus)),  this, SLOT(Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)) );
            connect( m_Apropos_Proc,  SIGNAL(error ( QProcess::ProcessError  )), G_pCApp, SLOT(Slot_error ( QProcess::ProcessError  )) );
            m_Apropos_Proc->start(pathExeAPropos, argList);
            m_Apropos_Proc->waitForStarted  (4000);
            //m_Apropos_Proc->waitForFinished (-1);     // crash crash bug QT connu sauf si -1 comme paramètre
            //..... pour contourner le bug on fait une boucle d'attente un peu sale ....
            /*
            G_pCApp->processEvents ();
            while ( (procState = m_Apropos_Proc->state())== QProcess::Running ) // && QFile::exists(pathBinRessources+"~A_propos.html")
                  { //qDebug(QString::number(procState).toAscii());
                    QApplication::processEvents ( QEventLoop::ExcludeUserInput );
                  }
            */
            //if (m_Apropos_Proc) delete m_Apropos_Proc;
            //m_Apropos_Proc = 0;
            //QFile::remove(pathBinRessources+"~A_propos.html");
            //m_action_A_Propos->setDisabled(FALSE);
           }
       QApplication::restoreOverrideCursor();
}

//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void C_Manager::tryToStopAPropos()
{Slot_Apropos_Proc_finished (0,  QProcess::NormalExit);
 //if (m_Apropos_Proc==0) return;
 //m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}

//--------------------------------- Slot_OnTimer ---------------------------------------------------------------------------------------
void C_Manager::Slot_OnTimer()
{if (G_pCApp->m_pCMoteurBase->OpenBase()==0) return;
if ( m_pCMoteurBase->IsDossierVerrouille(G_pCApp->identitePatientGUID()))
   {setVerrou(TRUE);
   }
else
   {setVerrou(FALSE);
   }
G_pCApp->m_pCMoteurBase->CloseBase();
}

///////////////////////////////////////// SIGEMS ///////////////////////////////////////////////////
//--------------------------------- Slot_pushButton_SigemsPA_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_action_Imprimer (bool)
{
#ifndef QT_NO_PRINTER
QString   modele = "";
QString     path = G_pCApp->pathAppli() + "Ressources/identiteModele"; //.html";
CGestIni::Param_UpdateFromDisk(path+".html" , modele);
if (modele.length()==0)
   {G_pCApp->CouCou(tr("Print template '%1' empty").arg(path));
    return;
   }
modele.replace("{{DATE}}",                    QDate::currentDate().toString("dd/MM/yyyy"));
modele.replace("{{TIME}}",                    QTime::currentTime().toString("hh:mm"));
modele.replace("{{NOM PATIENT}}",             m_pGUI->lineEdit_NomDossier->text());
modele.replace("{{PRENOM PATIENT}}",          m_pGUI->lineEdit_PrenomDossier->text());
modele.replace("{{NOM NAISSANCE PATIENT}}",   m_pGUI->lineEdit_NomDeNss->text());
modele.replace("{{NUM. TEL1 PATIENT}}",       m_pGUI->lineEdit_Tel1->text());
modele.replace("{{NUM. TEL2 PATIENT}}",       m_pGUI->lineEdit_Tel2->text());
modele.replace("{{NUM. TEL3 PATIENT}}",       m_pGUI->lineEdit_Tel3->text());
modele.replace("{{EMAIL PATIENT}}",           m_pGUI->lineEdit_Email->text());
modele.replace("{{DATE NAISSANCE PATIENT}}",  m_pGUI->lineEdit_DtNss->text());
modele.replace("{{SEXE PATIENT}}",            m_pGUI->comboBoxSexe->currentText());
modele.replace("{{TITRE PATIENT}}",           m_pGUI->lineEdit_Titre->text());
modele.replace("{{PROFESSION PATIENT}}",      m_pGUI->lineEdit_Profession->text());
modele.replace("{{VILLE PATIENT}}",           m_pGUI->lineEditVille->text());
modele.replace("{{CODE POSTAL PATIENT}}",     m_pGUI->lineEdit_CdPostal->text());
modele.replace("{{ADRESSE PATIENT}}",         m_pGUI->textEdit_Adresse->text());
modele.replace("{{NUM. SECU PATIENT}}",       m_pGUI->lineEdit_NumSecu->text()+" "+ m_pGUI->lineEdit_NssClef->text());
modele.replace("{{NOM ASSURE}}",              m_pGUI->lineEdit_NomAssure->text());
modele.replace("{{PRENOM ASSURE}}",           m_pGUI->lineEdit_PrenomAssure->text());
modele.replace("{{DATE NAISSANCE ASSURE}}",   m_pGUI->lineEdit_DtNss_Assure->text());
modele.replace("{{DEBUT DROITS}}",            m_pGUI->lineEdit_DroitsDateDeb->text());
modele.replace("{{FIN DROITS}}",              m_pGUI->lineEdit_DroitsDateFin->text());
QString    cas = m_pGUI->lineEdit_CodeRegime->text()+m_pGUI->lineEdit_CaisseGest->text()+m_pGUI->lineEdit_CentreGest->text();
modele.replace("{{CAS}}",                     cas);
modele.replace("{{CODE REGIME}}",             m_pGUI->lineEdit_CodeRegime->text());
modele.replace("{{CODE GESTION}}",            m_pGUI->lineEdit_CodeGest->text());
modele.replace("{{CAISSE GESTIONNAIRE}}",     m_pGUI->lineEdit_CaisseGest->text());
modele.replace("{{CENTRE GESTIONNAIRE}}",     m_pGUI->lineEdit_CentreGest->text());
modele.replace("{{RANG GEM}}",                m_pGUI->lineEdit_RangGeme->text());
modele.replace("{{QUALITE AYANT DROIT}}",     m_pGUI->comboBoxQualiteAyantDroit->currentText());
CGestIni::Param_UpdateToDisk(path+"-P.html" , modele);

QWebView *pQWebView = new QWebView(this); if (pQWebView==0)   return;
pQWebView->load(QUrl(path + "-P.html"));

QPrinter printer(QPrinter::HighResolution);
//printer.setOutputFormat(QPrinter::PdfFormat);
//QFileInfo fi(path);

//if (destPdfFile.length()==0) destPdfFile = fi.path()+"/"+fi.baseName()+".pdf";                // name = "archive.tar.gz"
//printer.setOutputFileName (destPdfFile);
//pQWebView->show();
//if (QFile::exists(destPdfFile)) QFile::remove(destPdfFile);
QString printerName =  G_pCApp->readUniqueParam( "Print", "Printer name");
if (printerName.length()) printer.setPrinterName(printerName);
QPrintDialog *printDialog = new QPrintDialog(&printer, pQWebView);
if (printDialog->exec() == QDialog::Accepted)
   {pQWebView->print ( &printer );
    printerName = printer.printerName();
    G_pCApp->writeParam( "Print", "Printer name", printerName);
    G_pCApp->updateIniParamToDisk();
   }
delete pQWebView;
#endif // QT_NO_PRINTER
}

//--------------------------------- Slot_pushButton_SigemsPA_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_SigemsPA_Clicked()
{// {{:: exe_process (noWaitEnd  ,../../InterfaceSigems/bin/interfacesigems,{{NOM PATIENT}},{{PRENOM PATIENT}},{{GET_DOSS_GUID}},{{USER LOGIN MEDECIN}} )}}
   //............ controle du sexe ...............................
   if (m_pGUI->comboBoxSexe->currentText().toUpper()[0]=='I')
      {QMessageBox  msgBox;
       QPushButton *femininButton  = msgBox.addButton(tr("&Female"),  QMessageBox::ActionRole);
       /*QPushButton *masculinButton = */ msgBox.addButton(tr("&Male"),    QMessageBox::ActionRole);
       msgBox.setText(tr("Entering sex is important an good"));
       msgBox.exec();

       if (msgBox.clickedButton() == femininButton)
          {m_pGUI->comboBoxSexe->setCurrentItem ( 1 );
          }
       else
          {m_pGUI->comboBoxSexe->setCurrentItem ( 0 );
          }
       m_pGUI->pushButtonSavModif->setEnabled(TRUE);
      }
   //................... controle de la coherence Nss et sexe ....................
   if (m_pGUI->comboBoxQualiteAyantDroit->currentItem()==0)
      {if ( (m_pGUI->comboBoxSexe->currentText().toUpper()[0]=='F'&&m_pGUI->lineEdit_NumSecu->text()[0]!='2') ||
            (m_pGUI->comboBoxSexe->currentText().toUpper()[0]=='M'&&m_pGUI->lineEdit_NumSecu->text()[0]!='1')
          )
          {QMessageBox  msgBox;
           /*QPushButton *annuler = */ msgBox.addButton(tr("Cancel"), QMessageBox::ActionRole);
           msgBox.setText(tr("Inconsistency between the patient's sex and the first number of social insurance number"));
           msgBox.exec();
           return;
          }
      }

   QString pathExe =  G_pCApp->m_PathSigemsPA+"InterfaceSigems";
   #ifdef  Q_WS_WIN
          pathExe += ".exe";
   #endif
   #ifdef Q_WS_X11
          pathExe += "";
   #endif
   #ifdef  Q_WS_MAC
          int pos = pathExe.lastIndexOf("/");
          if (pos != -1) pathExe = pathExe+".app/Contents/MacOS/"+pathExe.mid(pos+1);
   #endif
   if (!QFile::exists (pathExe ))
      {G_pCApp->CouCou(tr("Path to InterfaceSigems plugin not found: ").arg(pathExe));
       return;
      }

   if( getInterfaceMode()==MODE_CREATION_PATIENT)
     {onCreateAction(0);
     }
   else if ( getInterfaceMode()==MODE_SELECTION_PATIENT && m_pGUI->pushButtonSavModif->isEnabled())
     {Slot_pushButtonSavModif();
     }
   QStringList             argList;
   argList << G_pCApp->m_LastNom;
   argList << G_pCApp->m_LastPrenom;
   argList << G_pCApp->m_LastGUID;
   argList << G_pCApp->m_SignUser;
   QProcess::startDetached (pathExe, argList);
}

///////////////////////////////////////// NOMADISME ///////////////////////////////////////////////////
//--------------------------------- Slot_action_ToModeNomade_triggered -----------------------------------------------------------------------
void C_Manager::Slot_action_ToModeNomade_triggered (bool)
{if (m_pMasterSocket) delete m_pMasterSocket;
if (m_pSlaveSocket)  delete m_pSlaveSocket;
QAbstractSocket *m_pMasterSocket = new QAbstractSocket( QAbstractSocket::TcpSocket,    this );
connect(m_pMasterSocket,      SIGNAL(hostFound ()),                                    this, SLOT(Slot_hostMasterFound ()) );
connect(m_pMasterSocket,      SIGNAL(error ( QAbstractSocket::SocketError  )),         this, SLOT(Slot_hostMasterError (QAbstractSocket::SocketError )) );
connect(m_pMasterSocket,      SIGNAL(stateChanged ( QAbstractSocket::SocketState  )),  this, SLOT(Slot_hostMasterStateChanged (QAbstractSocket::SocketState )) );

QAbstractSocket *m_pSlaveSocket = new QAbstractSocket( QAbstractSocket::TcpSocket,     this );
connect(m_pSlaveSocket,       SIGNAL(hostFound ()),                                    this, SLOT(Slot_hostSlaveFound ()) );
connect(m_pSlaveSocket,       SIGNAL(error ( QAbstractSocket::SocketError  )),         this, SLOT(Slot_hostSlaveError (QAbstractSocket::SocketError )) );
connect(m_pSlaveSocket,       SIGNAL(stateChanged ( QAbstractSocket::SocketState  )),  this, SLOT(Slot_hostSlaveStateChanged (QAbstractSocket::SocketState )) );
m_pMasterSocket->connectToHost ( "192.168.10.10", 3306);
if (m_pMasterSocket->waitForConnected(2000)) qDebug()<<QString("stateMaster  : connected").toLatin1();
//else                                         qDebug(QString("stateMaster  : not connected").toLatin1());
m_pSlaveSocket->connectToHost ( "localhost", 3306);
if (m_pSlaveSocket->waitForConnected(2000))  qDebug()<<QString("stateSlave  : connected").toLatin1();
//else                                         qDebug(QString("stateSlave  : not connected").toLatin1());
}

//--------------------------------- socketStateToString -----------------------------------------------------------------------
QString  C_Manager::socketStateToString(QAbstractSocket::SocketState socketState, const QString& prefix )
{switch (socketState)
       { case QAbstractSocket::UnconnectedState: return tr("%1 The socket is not connected.").arg(prefix);
         case QAbstractSocket::HostLookupState:  return tr("%1 The socket is performing a host name lookup.").arg(prefix);
         case QAbstractSocket::ConnectingState:  return tr("%1 The socket has started establishing a connection.").arg(prefix);
         case QAbstractSocket::ConnectedState:   return tr("%1 A connection is established.").arg(prefix);
         case QAbstractSocket::BoundState:       return tr("%1 The socket is bound to an address and port (for servers).").arg(prefix);
         case QAbstractSocket::ClosingState:     return tr("%1 The socket is about to close (data may still be waiting to be written).").arg(prefix);
         case QAbstractSocket::ListeningState:   return tr("%1 For internal use only.").arg(prefix);
       }
return QString::null;
}
//--------------------------------- Slot_hostMasterStateChanged -----------------------------------------------------------------------
void C_Manager::Slot_hostMasterStateChanged (QAbstractSocket::SocketState socketState)
{qDebug()<<socketStateToString(socketState, tr("Master status: ")).toLatin1();
}
//--------------------------------- Slot_hostSlaveStateChanged -----------------------------------------------------------------------
void C_Manager::Slot_hostSlaveStateChanged (QAbstractSocket::SocketState socketState)
{qDebug()<<socketStateToString(socketState, tr("Itinerant status: ")).toLatin1();
}

//--------------------------------- Slot_action_ToModeConnect_triggered -----------------------------------------------------------------------
void C_Manager::Slot_hostMasterFound ()
{//qDebug(QString("host MasterFound").toLatin1());
}
//--------------------------------- Slot_action_ToModeConnect_triggered -----------------------------------------------------------------------
void C_Manager::Slot_hostSlaveFound ()
{//qDebug(QString("host SlaveFound").toLatin1());
}

//--------------------------------- testInterface -----------------------------------------------------------------------
void C_Manager::testInterface()
{   // m_pGUI->textEdit_Adresse->setText(G_pCApp->getConfigContext());
}

//--------------------------------- Slot_hostMasterError -----------------------------------------------------------------------
void C_Manager::Slot_hostMasterError (QAbstractSocket::SocketError socketError)
{


    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "MasterError",
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "MasterError",
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct.  %1"));
        break;
    default:
        QMessageBox::information(this, "MasterError",
                                 tr("The following error occurred: %1.")
                                 .arg(m_pMasterSocket->errorString()));

    }
}
//--------------------------------- Slot_hostSlaveError -----------------------------------------------------------------------
void C_Manager::Slot_hostSlaveError (QAbstractSocket::SocketError socketError)
{switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "SlaveError",
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "SlaveError",
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, "SlaveError",
                                 tr("The following error occurred: %1.")
                                 .arg(m_pSlaveSocket->errorString()));
    }
}
//--------------------------------- setAndSaveNomadeState -----------------------------------------------------------------------
void C_Manager::setAndSaveNomadeState (int nomadismeStateToSet)
{ G_pCApp->writeParam("Connexion", "nomadisme", nomadismeStateToSet==TRUE?"actif":"non actif");
  G_pCApp->updateIniParamToDisk();
  if (nomadismeStateToSet)
   {//............ positionner le bouton et icone sur la nouvelle action possible ......................
    //             CAD puisqu'on est actuellement en mode nomade l'action possible
    //                 est de le quitter pour passer en mode Master
    m_action_ModeNomade->setText(tr("Quit itinerant mode and reconnect to main server"));
    m_action_ModeNomade->setIconSet ( Theme::getIcon("32x32/unconnected.png") ) ;
   }
  else
   {m_action_ModeNomade->setText(tr("Disconnect from main server and activate itinerant mode"));
    m_action_ModeNomade->setIconSet ( Theme::getIcon("32x32/connected.png") ) ;
   }
}

//---------------------------------------------- testConnection -----------------------------------------------------------------------
int C_Manager::testConnection(QString connexionVar,  QString *errMess )
{
        QString param;
        QString  driver, baseToConnect, sqlUserName, sqlPass, hostName, port;
        CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli()+"Manager.ini", param);
        if (CGestIni::Param_ReadParam( param, "Connexion", connexionVar, &driver, &baseToConnect, &sqlUserName, &sqlPass, &hostName, &port) != QString::null )  // zero = pas d'erreur
           {return FALSE;
           }
       QSqlDatabase::removeDatabase ("TEST_CONNECTION");
       QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, "TEST_CONNECTION");
       defaultDB.setDatabaseName( baseToConnect );
       defaultDB.setUserName    ( sqlUserName );
       defaultDB.setPassword    ( sqlPass );
       defaultDB.setHostName    ( hostName);
       defaultDB.setPort        ( port.toInt());
       if ( ! defaultDB.open() )
          {QString qstr = "";
           qstr += tr("Database failed to open: ")    + "TEST_CONNECTION  Driver : " +  QString(driver) + "  " + QString(baseToConnect) + "\r\n" +
                    defaultDB.lastError().driverText() + "\r\n" + defaultDB.lastError().databaseText();
           qWarning()<<qstr.toLatin1();
           if (errMess) *errMess = qstr;
           QSqlDatabase::removeDatabase ("TEST_CONNECTION");
           return 0;
          }
       defaultDB.close();
       QSqlDatabase::removeDatabase ("TEST_CONNECTION");
       return 1;
}

//--------------------------------- Slot_action_ModeConnect_triggered -----------------------------------------------------------------------
void C_Manager::Slot_action_ModeConnect_triggered (bool)
{
if (identIsModified() && G_pCApp->isCurrentIdentiteValid() && G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idc") )
    { Slot_pushButtonSavModif();
    }
QApplication::setOverrideCursor ( QCursor(Qt::WaitCursor) );
if (G_pCApp->m_IsGestionNomadisme==CMoteurBase::fullReplication)
   {
    action_fullSwitchReplication_triggered ();
   }
else if (G_pCApp->m_IsGestionNomadisme==CMoteurBase::fullSynchro)
  {action_fullSynchroReplication_triggered ();
  }
QApplication::restoreOverrideCursor();
}
//--------------------------------- tryToFindConnectionFromList -----------------------------------------------------------------------
QString C_Manager::tryToFindConnectionFromList (QString connexionVar,  QString *errMess)
{       //..................... recuperer la liste des connections possibles ..................
        QString param;
        CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli()+"Manager.ini", param);
        QString  driver, baseToConnect, sqlUserName, sqlPass, hostName, port;
        //..................... tester la liste des connections possibles ..................
        //                      et s'arreter a la premiere possible
             if (CGestIni::Param_ReadParam( param, "Connexion", connexionVar, &driver, &baseToConnect, &sqlUserName, &sqlPass, &hostName, &port) == QString::null )  // zero = pas d'erreur
                {QSqlDatabase::removeDatabase ("TEST_CONNECTION");
                 QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, "TEST_CONNECTION");
                 defaultDB.setDatabaseName( baseToConnect );
                 defaultDB.setUserName    ( sqlUserName );
                 defaultDB.setPassword    ( sqlPass );
                 defaultDB.setHostName    ( hostName);
                 defaultDB.setPort        ( port.toInt());
                 if ( ! defaultDB.open() )
                    {QString qstr = "";
                     qstr += tr("Database failed to open: ")    + "TEST_CONNECTION  Driver : " +  QString(driver) + "  " + QString(baseToConnect) + "\r\n" +
                     defaultDB.lastError().driverText() + "\r\n" + defaultDB.lastError().databaseText();
                     qWarning()<<qstr.toLatin1();
                     if (errMess) *errMess = qstr;
                     QSqlDatabase::removeDatabase ("TEST_CONNECTION");
                     return QString::null;
                    }
                 defaultDB.close();
                 QSqlDatabase::removeDatabase ("TEST_CONNECTION");
                 return "Ok";
                }
return QString::null;
}
//--------------------------------- getScriptNomadePath -----------------------------------------------------------------------
QString C_Manager::getScriptNomadePath()
{
    #ifdef Q_WS_MAC
        QString scriptPath = G_pCApp->pathAppli() + "Nomadisme/MacOSScripts";
        return scriptPath.replace("/", QDir::separator ());
    #endif
    #ifdef Q_WS_WIN
        QString scriptPath = G_pCApp->pathAppli() + "Nomadisme/WindowsScripts";
        return   scriptPath.replace("/", QDir::separator());
    #endif
    #ifdef Q_WS_X11
        QString scriptPath = G_pCApp->pathAppli() + "Nomadisme/LinuxScripts";
        return scriptPath.replace("/", QDir::separator ());
    #endif
}

//--------------------------------- action_fullSynchroReplication_triggered -----------------------------------------------------------------------
void C_Manager::action_fullSynchroReplication_triggered ()
{QString errMess   = "";
QString paramConn = tryToFindConnectionFromList ("Master", &errMess);
if (paramConn.length()==0)
   {QMessageBox::information( this, tr("CONNECTION TO MASTER SERVER FAILED"),
                                    tr( " <b><u>WARNING</b></u> ! Connection to master server <br />")+
                                    tr( "is impossible.<br /><u>Error ") + ":</u><br><pre>"  + errMess + "</pre><br />" +
                                    tr( "Please check your parameters and slots.") ,
                                    tr("&Cancel"), 0, 0,
                                    0, 0 );
    return;
   }
QString login_Master,   login_Nomade;
QString host_Master,    host_Nomade;
QString pass_Master,    pass_Nomade;
QString port_Master,    port_Nomade;
QString driver,         baseToOpen;
//............... recuperation des valeurs dans le .INI ...........
QString param;
CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli()+"Manager.ini", param);
CGestIni::Param_ReadParam( param, "Connexion", "Master",  &driver,  &baseToOpen, &login_Master,  &pass_Master,    &host_Master,  &port_Master);
CGestIni::Param_ReadParam( param, "Connexion", "Nomade",  &driver,  &baseToOpen, &login_Nomade,  &pass_Nomade,    &host_Nomade,  &port_Nomade);

QString scriptPath  = getScriptNomadePath();
QStringList paramList; CGestIni::Param_GetList(param, "Connexion", "listParam", paramList );
/*
    #ifdef Q_WS_MAC
    #endif
    #ifdef Q_WS_WIN
        setBatchParametres(scriptPath+"/Bat_StopCircularRepli.bat", paramList);
        QString scriptToDo  = QString("\"")+scriptPath  + "/Bat_StopCircularRepli.bat\" \"" + scriptPath + "\" \"" +
                              host_Master +"\" \""+ login_Master +"\" \"" +  pass_Master +"\" \"" +  port_Master +"\" \""  +
                              host_Nomade +"\" \""+ login_Nomade +"\" \"" +  pass_Nomade +"\" \"" +  port_Nomade + "\"";
    #endif
    #ifdef Q_WS_X11
    #endif
        //                                                        lancer le script ..............................................
        QStringList argList;
        putScriptWaitFlag(   scriptPath  + "/ScriptStatus.txt");
    #ifdef Q_WS_X11
            argList<<"777"<<scriptToDo;  QProcess::execute ("chmod", argList); argList.clear();      argList<<scriptToDo;
            QProcess::execute ("bash", argList);
    #endif
    #ifdef Q_WS_MAC
             argList<<"777"<<scriptToDo;  QProcess::execute ("chmod", argList); argList.clear();      argList<<scriptToDo;
             QProcess::execute ("bash", argList);
    #endif
    #ifdef Q_WS_WIN
             QProcess::startDetached (QString ("cmd /c start ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD
             //QProcess::startDetached (QString ("START.EXE /B /MIN ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD
    #endif
        //................. attendre la fin de l'execution du script .......................................................
        waitForEndScript( scriptPath + "/ScriptStatus.txt" );
        //................. apres mise a jour recuperer fichier ............................................................
        //                  etat de la repli circulaire apres arret boucle circulaire
        QString repliStatus;
        CGestIni::Param_UpdateFromDisk(scriptPath  + "/Out_PosRepliLoop.txt", repliStatus);

        //            Master_Host: aadmaison.homedns.org
        //            Master_User: server104
        //            Master_Port: 3306
        //            Master_Log_File: LogBin103.000001
        //            Read_Master_Log_Pos: 2008

        QString master_Host         ="";
        QString master_User         ="";
        QString master_Port         ="";
        QString master_Log_File     ="";
        QString read_Master_Log_Pos ="";
        QString masterKey           = CGestIni::Param_ReadUniqueParam( param, "Connexion", "MasterKey");
        //........................ les appels doivent etre dans l'ordre d'appartion des variables .................
        //                         dans le fichier a analyser
        int pos                             = 0;
                       master_Host          = extractVarFromMysqlStatus(repliStatus, "Master_Host:",         pos);
        if (pos != -1) master_User          = extractVarFromMysqlStatus(repliStatus, "Master_User:",         pos);
        if (pos != -1) master_Port          = extractVarFromMysqlStatus(repliStatus, "Master_Port:",         pos);
        if (pos != -1) master_Log_File      = extractVarFromMysqlStatus(repliStatus, "Master_Log_File:",     pos);
        if (pos != -1) read_Master_Log_Pos  = extractVarFromMysqlStatus(repliStatus, "Read_Master_Log_Pos:", pos);
        if (pos == -1) return;          // serait bon de relancer la repli circulaire
     */
        //.......................... synchro ..............................................
        setConnectParametres( scriptPath + "/ScriptStartSlave.mod");      // ---> transforme en .txt      // etape 2
        setConnectParametres( scriptPath + "/ScriptStartMasterSrc.mod");  // ---> transforme en .txt      // etape 2
        setConnectParametres( scriptPath + "/ScriptJeReviens.mod");       // ---> transforme en .txt      // etape 3
        setConnectParametres( scriptPath + "/ScriptRelanceRepli.mod");    // ---> transforme en .txt      // etape 6

        //                                                        lancer le script ..............................................
        putScriptWaitFlag(   scriptPath  + "/ScriptStatus.txt");
    #ifdef Q_WS_X11
        QStringList argList;
        setBatchParametres(scriptPath+"/Bat_JeSynchronise.sh", paramList);
        QString  scriptToDo  = QString("'")+scriptPath  + "/Bat_JeSynchronise.sh' '" + scriptPath + "' '" +
                               host_Master +"' '"+ login_Master +"' '" +  pass_Master +"' '" +  port_Master + "' '"  +
                               host_Nomade +"' '"+ login_Nomade +"' '" +  pass_Nomade +"' '" +  port_Nomade + "'";
        argList<<"777"<<scriptToDo;  QProcess::execute ("chmod", argList);// argList.clear();      argList<<scriptToDo;
        QProcess::startDetached (QString ("bash ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD

    #endif
    #ifdef Q_WS_MAC
        QStringList argList;
        setBatchParametres(scriptPath+"/Bat_JeSynchronise.sh", paramList);
        QString  scriptToDo  = QString("'")+scriptPath  + "/Bat_JeSynchronise.sh' '" + scriptPath + "' '" +
                               host_Master +"' '"+ login_Master +"' '" +  pass_Master +"' '" +  port_Master + "' '"  +
                               host_Nomade +"' '"+ login_Nomade +"' '" +  pass_Nomade +"' '" +  port_Nomade + "'";
        argList<<"777"<<scriptToDo;  QProcess::execute ("chmod", argList);// argList.clear();      argList<<scriptToDo;
        QProcess::startDetached (QString ("bash ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD
    #endif
    #ifdef Q_WS_WIN
        setBatchParametres(scriptPath+"/Bat_JeSynchronise.bat", paramList);
        QString  scriptToDo  = QString("\"")+scriptPath  + QDir::separator()+"Bat_JeSynchronise.bat\" \"" + scriptPath + "\" \"" +
                               host_Master +"\" \""+ login_Master +"\" \"" +  pass_Master +"\" \"" +  port_Master +"\" \""  +
                               host_Nomade +"\" \""+ login_Nomade +"\" \"" +  pass_Nomade +"\" \"" +  port_Nomade + "\"";
        QProcess::startDetached (QString ("cmd /c start ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD

    #endif
        //................. attendre la fin de l'execution du script .......................................................
        waitForEndScript( scriptPath + "/ScriptStatus.txt" );

        allAgendasReconnect(driver, baseToOpen, login_Nomade, pass_Nomade, host_Nomade, port_Nomade);
        G_pCApp->changeAllModuleConnectionParam(driver, baseToOpen, login_Nomade, pass_Nomade, host_Nomade, port_Nomade);
        G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, login_Nomade, pass_Nomade, host_Nomade, port_Nomade,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel());
        G_pCApp->CouCou(tr("Itinerant mode activated\nconnected: ") + G_pCApp->m_pCMoteurBase->GetDataBaseLabel() + QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, login_Nomade, host_Nomade+":"+port_Nomade),"", 4000);
}

//--------------------------------- extractVarFromMysqlStatus -----------------------------------------------------------------------
QString C_Manager::extractVarFromMysqlStatus(const QString &repliSatus, QString var, int &pos)
{int end = -1;
//if (!var.endsWith(':')) var += ":";
if (pos==-1) pos = 0;
pos = repliSatus.indexOf(var, pos);
if (pos != -1)
   {pos += var.length();
    end  = repliSatus.indexOf("\n",pos);
    if (end != -1)
        {int deb = pos;
         pos     = end;
         return repliSatus.mid(deb,end-deb).remove("\r").trimmed();
        }
   }
pos = -1;
return QString::null;
}

//--------------------------------- action_fullSwitchReplication_triggered -----------------------------------------------------------------------
void C_Manager::action_fullSwitchReplication_triggered ()
{
  QString err = "";
  int nomadismeStateToSet = G_pCApp->m_IsNomadeActif; nomadismeStateToSet ^= 1;
  if (nomadismeStateToSet)  //.......... Master vers Nomade (JE PARS)..................................
     { if (testConnection("Nomade",&err)==TRUE)
          { reconnectToNomade(G_pCApp->m_IsGestionNomadisme);          // CZ 22.03.2011
          }
       else
          { QMessageBox::information( this, tr("CONNECTION AU SERVEUR NOMADE NON POSSIBLE"),
                                            tr ( " <b><u>ATTENTION</b></u> ! la connexion au serveur nomade <br>")+
                                            tr ( "est non possible.<br><u>Erreur :</u><br><pre>" ) + err + "</pre><br>" +
                                            tr(  "Veuillez v\303\251rifier vos branchements et param\303\250tres.") ,
                                            tr("Annu&ler"), 0, 0,
                                            0, 0 );
               return;
          }
     }
  else                           //..........Nomade vers Master (J'ARRIVE) ..............................
     { QString err;
       if (testConnection("Master",&err)==TRUE)
          { if (reconnectToMaster(G_pCApp->m_IsGestionNomadisme) == -1)    // CZ 22.03.2011
                return;
          }
       else
          { QMessageBox::information( this, tr("CONNECTION TO MASTER SERVER FAILED"),
                                            tr ( " <b><u>WARNING</b></u> ! Connection to master server <br />")+
                                            tr ( "is impossible.<br /><u>Error ") + ":</u><br><pre>"  + err + "</pre><br />" +
                                            tr(  "Please, check your parameters and slots.") ,
                                            tr("&Cancel"), 0, 0,
                                            0, 0 );
            return;
          }
     }
//............. \303\240 ce stade le changement de mode a reussi ...................
//              donc en prendre note et l'enteriner
G_pCApp->m_IsNomadeActif = nomadismeStateToSet;
setAndSaveNomadeState (nomadismeStateToSet);
//.............. replacer l'interface sur le nouveau mode .......................
QStringList userList; G_pCApp->m_pCMoteurBase->GetUserList( userList );
if (userList.indexOf ( G_pCApp->m_User )== -1)
   {G_pCApp->m_User      = userList[0];
    G_pCApp->m_SignUser  = userList[0];
   }
G_pCApp->lireDroitsUtilisateurs();
initComboBoxAgendaUser();
deleteAllAgendas();
if (m_pC_Frm_UserList) m_pC_Frm_UserList->setDefaultParameter();
reinitAgendaOnUser(G_pCApp->m_User , G_pCApp->m_SignUser);
setInterfaceOnProfil(MODE_SELECTION_PATIENT);
Slot_pushButton_ReinitListe_clicked();
setTitle();
}

//---------------------------------------------- reconnectToNomade -----------------------------------------------------------------------
int C_Manager::reconnectToNomade(int gestionNomadisme)
{   int ret = FALSE;
   QString login_Master,   login_Nomade;
   QString host_Master,    host_Nomade;
   QString pass_Master,    pass_Nomade;
   QString port_Master,    port_Nomade;
   QString driver, baseToOpen;
   //............... recuperation des valeurs dans le .INI ...........
   QString param;
   CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli()+"Manager.ini", param);
   CGestIni::Param_ReadParam( param, "Connexion", "Master",  &driver,  &baseToOpen, &login_Master,  &pass_Master,  &host_Master,  &port_Master);
   CGestIni::Param_ReadParam( param, "Connexion", "Nomade",  &driver,  &baseToOpen, &login_Nomade,  &pass_Nomade,  &host_Nomade,  &port_Nomade);

   QString scriptPath  = getScriptNomadePath();
   switch(gestionNomadisme)
   {case CMoteurBase::fullReplication:
       {QStringList paramList; CGestIni::Param_GetList(param, "Connexion", "listParam", paramList );
        setConnectParametres(scriptPath  + "/ScriptJePars.mod");
        putScriptWaitFlag(   scriptPath  + "/ScriptStatus.txt");
        #ifdef Q_WS_MAC
        #endif
        #ifdef Q_WS_WIN
           QString scriptMode = CGestIni::Param_ReadUniqueParam( param, "Connexion", "ScriptMode");
           if (scriptMode=="Spybase")
              {///////////////////// CZ A REVOIR 22.10.2010 //////////// debut
               QString  pathExe;
               QStringList argList;
               pathExe = QDir::cleanDirPath (QFileInfo (qApp->argv()[0]).dirPath (true) + "/../../Spybase/bin/Spybase.exe");
               argList << "JePars" ;
               QProcess::startDetached (pathExe, argList);
               ///////////////////// CZ A REVOIR 22.10.2010 ///////// fin

              }
           else
              {setBatchParametres(scriptPath+"/Bat_JePars.bat",paramList);
               QString scriptToDo  = QString("\"")+scriptPath  +QDir::separator()+"Bat_JePars.bat\" \"" + scriptPath + "\" \"" +
                                     host_Master +"\" \""+ login_Master +"\" \"" +  pass_Master +"\" \"" +  port_Master +"\" \""  +
                                     host_Nomade +"\" \""+ login_Nomade +"\" \"" +  pass_Nomade +"\" \"" +  port_Nomade + "\"";
               QProcess::startDetached (QString ("cmd /c start ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD
              }
        #endif
        #ifdef Q_WS_X11
           QStringList argList;
           setBatchParametres(scriptPath+"/Bat_JePars.sh",paramList);
           QString scriptToDo  = QString("'")+scriptPath  + "/Bat_JePars.sh' '" + scriptPath + "' '" +
                                 host_Master +"' '"+ login_Master +"' '" +  pass_Master +"' '" +  port_Master +"' '"  +
                                 host_Nomade +"' '"+ login_Nomade +"' '" +  pass_Nomade +"' '" +  port_Nomade + "'";
           argList<<"777"<<scriptToDo;  QProcess::execute ("chmod", argList);// argList.clear();      argList<<scriptToDo;
           QProcess::startDetached (QString ("bash ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD
           //QProcess::execute ("bash", argList);
        #endif
        //................. attendre la fin de l'execution du script ...............................
        waitForEndScript( scriptPath + "/ScriptStatus.txt" );
        //................. apres mise a jour se connecter au nomade .......................................................
        allAgendasReconnect(driver, baseToOpen, login_Nomade, pass_Nomade, host_Nomade, port_Nomade);
        G_pCApp->changeAllModuleConnectionParam(driver, baseToOpen, login_Nomade, pass_Nomade, host_Nomade, port_Nomade);
        G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, login_Nomade, pass_Nomade, host_Nomade, port_Nomade,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel());
        G_pCApp->CouCou(tr("Itinerant mode activated\nconnected\n: ") + G_pCApp->m_pCMoteurBase->GetDataBaseLabel() + QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, login_Nomade, host_Nomade+":"+port_Nomade),"", 4000);
       } break;
    case CMoteurBase::progMasterUpdate:
       {/*
        ret = FALSE;
        waitForsyncEmulDemonResponse(synchroMethode + " Stop ");
        //.............. se reconnecter \303\240 la base nomade locale...........................................
        QString driver, baseToOpen, user, password, hostname, port;
        if (CGestIni::Param_ReadParam( G_pCApp->m_LocalParam, "Connexion", "Nomade", &driver, &baseToOpen, &user, &password, &hostname, &port) != QString::null )
           { G_pCApp->CouCou(tr("Param\303\250tres \n[Connexion]\nNomade = \nnon trouv\303\251s dans Manager.ini: "));
           }
        else if (G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, user, password, hostname, port,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel()) == 0)
           { G_pCApp->CouCou(tr("Echec de la reconnexion au serveur Nomade : \ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, user, hostname+":"+port));
           }
        else
          { //.............. reinitialisation des tables de synchro (les vider) ...........................
            ret = TRUE;
            G_pCApp->m_pCMoteurBase->synchroInitTables();
            G_pCApp->CouCou(tr("Mode nomade activ\303\251\nconnect\303\251  : ") + G_pCApp->m_pCMoteurBase->GetDataBaseLabel() + QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, user, hostname+":"+port),"", 4000);
          */
          } break;
      } //end switch(G_pCApp->m_IsGestionNomadisme)
    return ret;
}
//---------------------------------------------- reconnectToMaster -----------------------------------------------------------------------
int C_Manager::reconnectToMaster(int gestionNomadisme)
{    int ret = FALSE;
    QString login_Master,   login_Nomade;
    QString host_Master,    host_Nomade;
    QString pass_Master,    pass_Nomade;
    QString port_Master,    port_Nomade;
    QString driver, baseToOpen;
    //............... recuperation des valeurs dans le .INI ...........
    QString param;
    CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli()+"Manager.ini", param);
    if (CGestIni::Param_ReadParam( param, "Connexion", "Nomade",  &driver,  &baseToOpen, &login_Nomade,  &pass_Nomade,  &host_Nomade,  &port_Nomade)!=QString::null)
       { G_pCApp->CouCou(tr("Parameters \n[Connexion]\nNomade = \nnot found in Manager.ini: "));
       }
    if (CGestIni::Param_ReadParam( param, "Connexion", "Master",  &driver,  &baseToOpen, &login_Master,  &pass_Master,  &host_Master,  &port_Master)!=QString::null)
       { G_pCApp->CouCou(tr("Parameters \n[Connexion]\nMaster = \nnot found in Manager.ini: "));
       }

    QString scriptPath  = getScriptNomadePath();
    switch(gestionNomadisme)
    {
     case CMoteurBase::fullReplication:
       {QStringList paramList; CGestIni::Param_GetList(param, "Connexion", "listParam", paramList );
        setConnectParametres( scriptPath + "/ScriptJeReviens.mod");
        putScriptWaitFlag(    scriptPath + "/ScriptStatus.txt");

        #ifdef Q_WS_MAC
        #endif
        #ifdef Q_WS_WIN
           QString scriptMode = CGestIni::Param_ReadUniqueParam( param, "Connexion", "ScriptMode");
           if (scriptMode=="Spybase")
              {///////////////////// CZ A REVOIR 22.10.2010 //////////// d?but
               QString  pathExe;
               QStringList argList;

               pathExe = QDir::cleanDirPath (QFileInfo (qApp->argv()[0]).dirPath (true) + "/../../Spybase/bin/Spybase.exe");
               argList << "JeReviens" ;
               QProcess::startDetached (pathExe, argList);
               ///////////////////// CZ A REVOIR 22.10.2010 ///////// fin
              }
           else
              { setBatchParametres(scriptPath+"/Bat_JeReviens.bat", paramList);
                QString scriptToDo  = QString("\"")+scriptPath  + QDir::separator() + "Bat_JeReviens.bat\" \""  + scriptPath + "\" \"" +
                                      host_Master +"\" \""+ login_Master + "\" \""  +  pass_Master +"\" \"" +  port_Master + "\" \"" +
                                      host_Nomade +"\" \""+ login_Nomade + "\" \""  +  pass_Nomade +"\" \"" +  port_Nomade + "\"";
                QProcess::startDetached (QString ("cmd /c start ") + scriptToDo );
              }
        #endif
        #ifdef Q_WS_X11
           QStringList argList;
           setBatchParametres(scriptPath+"/Bat_JeReviens.sh", paramList);
           QString scriptToDo  = QString("'")+scriptPath  + "/Bat_JeReviens.sh' '"  + scriptPath + "' '" +
                                 host_Master +"' '"+ login_Master +"' '" +  pass_Master +"' '" +  port_Master + "' '" +
                                 host_Nomade +"' '"+ login_Nomade +"' '" +  pass_Nomade +"' '" +  port_Nomade + "'";
           argList<<"777"<<scriptToDo;  QProcess::execute ("chmod", argList);// argList.clear();      argList<<scriptToDo;
           QProcess::startDetached (QString ("bash ") + scriptToDo ); //START.EXE /B /MIN leScriptAmasquer.CMD
        #endif
           //................. attendre la fin de l'execution du script ...............................
           /*-------------------------------CZ 22.10.2010-----------------------------------------------------
           waitForEndScript( scriptPath + "/ScriptStatus.txt" );
           //................. apres mise a jour se connecter au master .......................................................
           G_pCApp->changeAllModuleConnectionParam(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master);
           allAgendasReconnect(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master);
           G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel());
           G_pCApp->CouCou(tr("Mode master activ\303\251\nconnect\303\251 \303\240 : ") + G_pCApp->m_pCMoteurBase->GetDataBaseLabel() + QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, login_Master, host_Master+":"+port_Master),"", 4000);
           -------------------------------CZ 22.10.2010---------------------------------------------------*/
           /////////////////////// / CZ 22.10.2010 //////////// debut ///
           if (waitForEndScript( scriptPath + "/ScriptStatus.txt" ) == 0)
              {//................. apres mise a jour se connecter au master .......................................................
               G_pCApp->changeAllModuleConnectionParam(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master);
               allAgendasReconnect(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master);
               G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel());
               G_pCApp->CouCou(tr("Master mode active\nconnected to: ") + G_pCApp->m_pCMoteurBase->GetDataBaseLabel() + QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, login_Master, host_Master+":"+port_Master),"", 4000);
              }
           else
              {G_pCApp->CouCou(tr("Connection failed\nItinerant Mode kept.") );
               return -1;
              }
           /////////////////////// / CZ 22.10.2010 //////////// fin
       } break;
    case CMoteurBase::progMasterUpdate:
       {/*
        //......... se connecter au referentiel et le mettre \303\240 jour ...........................
        QString driver, baseToOpen, user, password, hostname, port;
        if (CGestIni::Param_ReadParam( G_pCApp->m_LocalParam, "Connexion", "Master", &driver, &baseToOpen, &user, &password, &hostname, &port) != QString::null )
           { G_pCApp->CouCou(tr("Param\303\250tres \n[Connexion]\nMaster = \nnon trouv\303\251s dans Manager.ini: "));
           }
        else if (G_pCApp->m_pCMoteurBase->synchroNomadeToMasterUpdate(driver, "MASTER_TUX", baseToOpen,  user,  password,  hostname, port ) == 0)
           {G_pCApp->CouCou(tr("Echec de la mise \303\240 jour\n et de la reconnexion au serveur ma\303\256tre \"MASTER_TUX\" : \ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, user, hostname+":"+port));
           }
        else
           {ret = TRUE;
            G_pCApp->CouCou(tr("Mise \303\240 jour\net reconnexion au serveur ma\303\256tre \"MASTER_TUX\" r\303\251ussie :\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, user, hostname+":"+port),"", 4000);
           }
        //.............. effacer les tables de synchro ...........................
        G_pCApp->m_pCMoteurBase->synchroDropTables();
        //........... ecrire l'action \303\240 effectuer par le demon de synchro dans le Manager.ini ........................
        waitForsyncEmulDemonResponse(synchroMethode + " Start ");
        //................. apres mise \303\240 jour se connecter au master .......................................................
        G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, user, password, hostname, port,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel());
        G_pCApp->CouCou(tr("Mode master activ\303\251\nconnect\303\251 \303\240 : ") + G_pCApp->m_pCMoteurBase->GetDataBaseLabel() + QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, user, hostname+":"+port),"", 4000);
        */
       }  break;
    case CMoteurBase::scriptOnly:
       {/*
        //.................se connecter au master .......................................................
        G_pCApp->m_pCMoteurBase->BaseConnect(driver, baseToOpen, login_Master, pass_Master, host_Master, port_Master,0, G_pCApp->m_pCMoteurBase->GetDataBaseLabel());
        //........... ecrire l'action \303\240 effectuer par le demon de synchro dans le Manager.ini ........................
        waitForsyncEmulDemonResponse(synchroMethode + " Start ");
        G_pCApp->CouCou(tr("Mode master activ\303\251\nconnect\303\251 \303\240 : ")     +
                        G_pCApp->m_pCMoteurBase->GetDataBaseLabel() +
                        QString("\ndriver : %1\nbase : %2\nuser : %3\nhost : %4").arg(driver, baseToOpen, login_Master, host_Master+":"+port_Master),"", 4000);
        */
       }
   }
    return ret;
}
//--------------------------------- setBatchParametres -----------------------------------------------------------------------
void C_Manager::setBatchParametres(const QString &scriptSqlIn, const QStringList &parmList)
{QString scriptStr;
QString path = QFileInfo(scriptSqlIn).path()+"/"+QFileInfo(scriptSqlIn).baseName()+".mod";
CGestIni::Param_UpdateFromDisk(path, scriptStr);
for (int i = 0; i < parmList.size(); ++i)
    {QString param =  parmList[i];
     if (param.left(6)=="$toAbs")
        {if ( QDir(param).isRelative()) param.prepend(G_pCApp->pathAppli());
         param = QDir::cleanPath(param); param.replace("/", QDir::separator ());
        }
     scriptStr.replace(QString("{{param ") + QString::number(i)+"}}",   param);
    }
CGestIni::Param_UpdateToDisk(scriptSqlIn, scriptStr);
}

//--------------------------------- setConnectParametres -----------------------------------------------------------------------
/*
CHANGE MASTER TO MASTER_HOST='{{reverse host}}',
MASTER_PORT={{reverse port}},
MASTER_USER='{{reverse user}}',
MASTER_PASSWORD='{{reverse pass}}',
MASTER_LOG_FILE='{{reverse log bin}}',

   Gestion nomadisme = active
   nomadisme   = actif
   Master      = QMYSQL3 , DrTuxTest , slave402  ,aixact  , WXP-MAXDATA    , 3306, LogBin
   Master      = QMYSQL3 , DrTuxTest , root      ,        , localhost      , 3306, LogBin
   Nomade      = QMYSQL3 , DrTuxTest , server401 , aixact , localhost      , 3306, LogBin
   Reverse     = QMYSQL3 , DrTuxTest , server401 , aixact , 192.168.69.101 , 3306, LogBinNomade.000001
   Parametres  = QMYSQL3 , DrTuxTest , root      ,        , localhost      , 3306, LogBin
*/
void C_Manager::setConnectParametres(const QString &scriptSqlIn,
                                    const QString master_Host         /*=""*/,
                                    const QString master_User         /*=""*/,
                                    const QString master_Port         /*=""*/,
                                    const QString masterKey_in        /*=""*/,
                                    const QString master_Log_File     /*=""*/,
                                    const QString read_Master_Log_Pos /*=""*/
                                   )
{QString login_Master,   login_masterSrcN,   login_masterSrcR,  login_Nomade,   login_Reverse;
QString host_Master,    host_masterSrcN,    host_masterSrcR,    host_Nomade,    host_Reverse;
QString pass_Master,    pass_masterSrcN,    pass_masterSrcR,    pass_Nomade,    pass_Reverse;
QString port_Master,    port_masterSrcN,    port_masterSrcR,    port_Nomade,    port_Reverse;
QString logBin_Master,  logBin_masterSrcN,  logBin_masterSrcR,  logBin_Nomade,  logBin_Reverse;
QString log_Pos_Master, log_Pos_masterSrcN, log_Pos_masterSrcR, log_Pos_Nomade, log_Pos_Reverse;
QString dumy;
QString masterKey = masterKey_in;
if (masterKey[0]=='$') masterKey = CGestIni::PassWordDecode(masterKey.remove('$'));
//............... recuperation des valeurs dans le .INI ...........
QString param;
CGestIni::Param_UpdateFromDisk(G_pCApp->pathAppli()+"Manager.ini", param);
CGestIni::Param_ReadParam( param, "Connexion", "Master",     &dumy,    &dumy,       &login_Master,    &pass_Master,        &host_Master,      &port_Master,      &logBin_Master,     &log_Pos_Master);
CGestIni::Param_ReadParam( param, "Connexion", "Nomade",     &dumy,    &dumy,       &login_Nomade,    &pass_Nomade,        &host_Nomade,      &port_Nomade,      &logBin_Nomade,     &log_Pos_Nomade);
CGestIni::Param_ReadParam( param, "Connexion", "Reverse",    &dumy,    &dumy,       &login_Reverse,   &pass_Reverse,       &host_Reverse,     &port_Reverse,     &logBin_Reverse,    &log_Pos_Reverse);
CGestIni::Param_ReadParam( param, "Connexion", "MasterSrcN", &dumy,    &dumy,       &login_masterSrcN, &pass_masterSrcN,   &host_masterSrcN,  &port_masterSrcN,  &logBin_masterSrcN, &log_Pos_masterSrcN);
CGestIni::Param_ReadParam( param, "Connexion", "MasterSrcR", &dumy,    &dumy,       &login_masterSrcR, &pass_masterSrcR,   &host_masterSrcR,  &port_masterSrcR,  &logBin_masterSrcR, &log_Pos_masterSrcR);

//............... remplacement des tags et tokens .................
QString scriptStr;
CGestIni::Param_UpdateFromDisk(scriptSqlIn, scriptStr);
scriptStr.replace("{{master user}}",      login_Master);
scriptStr.replace("{{masterSrcN user}}",  login_masterSrcN);
scriptStr.replace("{{masterSrcR user}}",  login_masterSrcR);
scriptStr.replace("{{nomade user}}",      login_Nomade);
scriptStr.replace("{{reverse user}}",     login_Reverse);

scriptStr.replace("{{master host}}",      host_Master);
scriptStr.replace("{{masterSrcN host}}",  host_masterSrcN);
scriptStr.replace("{{masterSrcR host}}",  host_masterSrcR);
scriptStr.replace("{{nomade host}}",      host_Nomade);
scriptStr.replace("{{reverse host}}",     host_Reverse);

scriptStr.replace("{{master pass}}",      pass_Master);
scriptStr.replace("{{masterSrcN pass}}",  pass_masterSrcN);
scriptStr.replace("{{masterSrcR pass}}",  pass_masterSrcR);
scriptStr.replace("{{nomade pass}}",      pass_Nomade);
scriptStr.replace("{{reverse pass}}",     pass_Reverse);

scriptStr.replace("{{master port}}",      port_Master);
scriptStr.replace("{{masterSrcN port}}",  port_masterSrcN);
scriptStr.replace("{{masterSrcR port}}",  port_masterSrcR);
scriptStr.replace("{{nomade port}}",      port_Nomade);
scriptStr.replace("{{reverse port}}",     port_Reverse);

scriptStr.replace("{{master logbin}}",       logBin_Master);
scriptStr.replace("{{masterSrcN logbin}}",   logBin_masterSrcN);
scriptStr.replace("{{masterSrcR logbin}}",   logBin_masterSrcR);
scriptStr.replace("{{nomade logbin}}",       logBin_Nomade);
scriptStr.replace("{{reverse logbin}}",      logBin_Reverse);

scriptStr.replace("{{master log_Pos}}",      log_Pos_Master);
scriptStr.replace("{{masterSrcN log_Pos}}",  log_Pos_masterSrcN);
scriptStr.replace("{{masterSrcR log_Pos}}",  log_Pos_masterSrcR);
scriptStr.replace("{{nomade log_Pos}}",      log_Pos_Nomade);
scriptStr.replace("{{reverse log_Pos}}",     log_Pos_Reverse);

//................... infos lues dans les fichers status ..................
scriptStr.replace("{{Master_Host}}",         master_Host);
scriptStr.replace("{{Master_User}}",         master_User);
scriptStr.replace("{{Master_Port}}",         master_Port);
scriptStr.replace("{{MasterKey}}",           masterKey);
scriptStr.replace("{{Master_Log_File}}",     master_Log_File);
scriptStr.replace("{{Read_Master_Log_Pos}}", read_Master_Log_Pos);

QString scriptSqlOut = QFileInfo(scriptSqlIn).path()+"/"+QFileInfo(scriptSqlIn).baseName()+".txt";
CGestIni::Param_UpdateToDisk(scriptSqlOut, scriptStr);
}

//--------------------------------- waitForsyncEmulDemonResponse -----------------------------------------------------------------------
void C_Manager::waitForsyncEmulDemonResponse(const QString &action)
{        QFileInfo qfi(qApp->argv()[0]);
        //........... recuperer le syncEmul.ini ........................
        QString pathSyncIni = CGestIni::Construct_Name_File_Ini("syncEmul",   qfi.dirPath (true), "");
        QString syncParam;
        CGestIni::Param_UpdateFromDisk(pathSyncIni, syncParam);
        //........... ecrire l'action \303\240 effectuer par le demon de synchro dans le Manager.ini ........................
        CGestIni::Param_WriteParam(&syncParam,"Connexion", "slaveSatus", action);
        CGestIni::Param_UpdateToDisk(pathSyncIni,syncParam);

        //........... attendre quel le demon de synchro ait fini son travail .............................................
        //            slaveSatus == "synchro Terminated" dans Manager.ini
        int         ok = 0;
        while (ok<2000)
           {SLEEP(1);
            CGestIni::Param_UpdateFromDisk(pathSyncIni,syncParam);
            QString status = CGestIni::Param_ReadUniqueParam( syncParam, "Connexion", "slaveSatus");
            if (status=="synchro Terminated") break;
            ++ok;
           }
}
//--------------------------------- putScriptWaitFlag -----------------------------------------------------------------------
void C_Manager::putScriptWaitFlag(const QString &scriptStatusFile)
{        //........... recuperer le syncEmul.ini ........................
        QString syncParam = "started";
        CGestIni::Param_UpdateToDisk(scriptStatusFile,   syncParam);
}
//--------------------------------- waitForEndScript -----------------------------------------------------------------------
int C_Manager::waitForEndScript(const QString &scriptStatusFile)
{        //........... recuperer le syncEmul.ini ........................
        QString syncParam;
        //........... attendre que le script ait fini son travail .............................................
        int  ok = 0;
        while (ok<2000)
           {SLEEP(1);
            CGestIni::Param_UpdateFromDisk(scriptStatusFile, syncParam);
            //if (syncParam.indexOf("terminated") != -1) break;            // CZ 22-10-2010
            if (syncParam.indexOf("terminated") != -1) return( 0);         // CZ 22-10-2010
            if (syncParam.indexOf("Incident") != -1)   return(-1);         // CZ 22-10-2010
            ++ok;
           }
        return -1;                                                         // RS 16-02-2011
}

///////////////////////////////////////// LANCEMENT DE DR TUX ////////////////////////////////////////
//--------------------------------- Slot_LauchPatient -----------------------------------------------------------------------
//  ACTION : ce slot est appel\303\251 lors d'un clic sur le bouton de lancement de dossier d'un rendez vous
void C_Manager::Slot_LauchPatient(const QString &guid, C_RendezVous *pC_RendezVous)
{
if (guid.length()== 0)
   { if  ( pC_RendezVous && pC_RendezVous->m_Nom.length() && pC_RendezVous->m_Prenom.length())
         { m_pGUI->lineEditAutoLocator->setText(pC_RendezVous->m_Nom+";"+pC_RendezVous->m_Prenom);
           if (m_pGUI->listView_Patient->topLevelItemCount()==1)
              {
               QTreeWidgetItemIterator it(m_pGUI->listView_Patient);
               QTreeWidgetItem *pQListViewItem = 0;
               while (*it) { pQListViewItem = (*it); if (pQListViewItem != 0) break;
                             ++it;
                           }
               if (pQListViewItem)
                  {pQListViewItem->setSelected(TRUE);      // pour mettre \303\240 jour la fiche identite
                   int ret = QMessageBox::information( this, tr("Launch a file"),
                                               tr( "A file exists for this name:")+" <br /><b>" + pC_RendezVous->m_Nom + "&nbsp;" + pC_RendezVous->m_Prenom +
                                                   "</b>" +
                                                   "<br />"+tr("Should we launch this file ?") ,
                                               tr("&Launch"), tr("&Cancel"), 0,
                                               1, 1 );
                   if (ret==0)
                      { Slot_LauchPatient(pQListViewItem->text(3));
                      }
                  } //if (pQListViewItem)
              }
           else if (m_pGUI->listView_Patient->topLevelItemCount()==0)
              {int ret = QMessageBox::information( this, tr("Create a file"),
                                               tr( "There is no file for this name:")+ "<br /><b>?" + pC_RendezVous->m_Nom + "  " + pC_RendezVous->m_Prenom +
                                                   "?</b>" +
                                                   "<br />"+ tr("should we create one ?"
                                                  ) ,
                                               tr("&Create a file"), tr("&Cancel"), 0,
                                               1, 1 );
              if (ret==0)
                 {Slot_pushButtonNew_clicked();
                  m_pGUI->lineEdit_NomDossier->setText(pC_RendezVous->m_Nom);
                  m_pGUI->lineEdit_PrenomDossier->setText(pC_RendezVous->m_Prenom);
                  m_pGUI->lineEdit_Tel1->setText(pC_RendezVous->m_Tel);
                 }
              }
         }
    else
         {int ret = QMessageBox::information( this, tr("File Creation"),
                                              tr( "Create a new file ?"  ) ,
                                              tr("Create a file"), tr("&Cancel"), 0,
                                              1, 1 );
             if (ret==0)
                {Slot_pushButtonNew_clicked();
                 if (pC_RendezVous)
                    {m_pGUI->lineEdit_NomDossier->setText(pC_RendezVous->m_Nom);
                     m_pGUI->lineEdit_PrenomDossier->setText(pC_RendezVous->m_Prenom);
                     m_pGUI->lineEdit_Tel1->setText(pC_RendezVous->m_Tel);
                    }
                }
         }
   }
else      //if (guid.length()== 0)
   { long nb = m_pCMoteurBase->GetPatientList(  m_pGUI->listView_Patient,  // appel GetPatientList(...) indispensable (BUG JLdajon)
                                                QString("_GUID=")+guid,
                                                "",
                                                m_pGUI->textLabel_StateList
                                             );
     if (nb) Slot_LauchPatient(guid);
   }
}

//--------------------------------- Slot_LauchPatient -----------------------------------------------------------------------
void C_Manager::Slot_LauchPatient(const QString &guid)
{if (guid.length()==0) return;
//........... si pr\303\251sent dans la liste patient...............
//            lancer le dossier
if (guid == G_pCApp->m_LastGUID)
   {Slot_pushButtonAcceder_Clicked();
    return;
   }
//........... si pas pr\303\251sent dans la liste patient...............
//            requeter pour recuperer les infos
if  (m_pCMoteurBase->OpenBase()==0) return;
QString requete  = "SELECT ";
        requete += m_pCMoteurBase->m_DOSS_INDEX_NOM                     + ", "  +     // 0
                   m_pCMoteurBase->m_DOSS_INDEX_PRENOM                  + ", "  +     // 1
                   m_pCMoteurBase->m_DOSS_INDEX_PRIM_KEY                + "  "  +     // 2
                   " FROM "   + m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME   + " WHERE " +
                                m_pCMoteurBase->m_DOSS_INDEX_GUID       + " = '"    + guid + "' ";
 QString nom,prenom,pk;   // on va \303\240 la peche de ?a
 QSqlQuery query(requete , m_pCMoteurBase->getDatabase() );
 //.................. si la requ\303\250te a un resultat ..............................................
 if (query.isActive())
    {while (query.next())
        { nom    =  m_pCMoteurBase->Utf8_Query(query, 0 );   // Nom
          prenom =  m_pCMoteurBase->Utf8_Query(query, 1 );   // Prenom
          pk     =  m_pCMoteurBase->Utf8_Query(query, 2 );   // PK
        } // while (query.next())
    } //endif (query.isActive())
 m_pCMoteurBase->CloseBase();
 //................. si infos retrouv\303\251es ......................................................
 if (pk.length())
    {accederDossier   (nom,         // Nom assur\303\251
                       prenom,      // Prenom assur\303\251
                       pk,          // numero PrimKey
                       guid,        // GUID dossier
                       G_pCApp->m_User,            // login de l'utilisateur en cours
                       G_pCApp->m_SignUser);       // login de l'utilisateur signataire en cours
    }
}

//--------------------------------- Slot_pushButtonAcceder_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonAcceder_Clicked()
{   if (sexControl()==false) return;
    accederDossier(G_pCApp->m_LastNom,         // Nom assur\303\251
                   G_pCApp->m_LastPrenom,      // Prenom assur\303\251
                   G_pCApp->m_LastPK,          // numero PrimKey
                   G_pCApp->m_LastGUID,        // GUID dossier
                   G_pCApp->m_User,            // login de l'utilisateur en cours
                   G_pCApp->m_SignUser);       // login de l'utilisateur signataire en cours

   //................. Patient List Mode = clear .........................................
   QString val;
   if (G_pCApp->readParam( "Derniere Session", "Patient List Mode", &val)==QString::null && (val.at(0).lower() == 'c'))   // = zero si pas d'erreur
      { m_pGUI->lineEditAutoLocator->clear();
        m_pGUI->listView_Patient->clear();
        clearIdentFields();
        m_pGUI->lineEditAutoLocator->setFocus();
      }
   else
      { m_pGUI->lineEditAutoLocator->setFocus();
      }
}

//--------------------------------- accederDossier -----------------------------------------------------------------------------
void C_Manager::accederDossier( QString nom,  QString prenom, const QString &primKey, const QString &guid, const QString &user, const QString &sign_user)
{  if (G_pCApp->IsThisDroitExist( "idg")==0)
      { G_pCApp->CouCou(tr( "Your rights are insufficient to launch a file"));
        return;
      }
   if (getInterfaceMode() != MODE_CREATION_PATIENT && m_pGUI->pushButtonSavModif->isEnabled() )  //Slot_pushButtonSavModif();
   /*
   if (pushButtonSavModif->isEnabled() && G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idc") && (!pushButton_QuitterMultiCritere->isShown()) )
      {int ret   = QMessageBox::warning ( this, tr("Enregistrer les modifications :"),
                                    tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp;le dossier :<br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                    G_pCApp->m_LastNom  + " " + G_pCApp->m_LastPrenom  +
                                    tr(  "</font></b>&nbsp;&nbsp; a \303\251t\303\251 modifi\303\251, faut il enregistrer les modifications avant de l'ouvrir"),
                                    tr("&Modifier"), tr("Annu&ler"), 0,
                                    1, 1
                                    );
      if (ret==0) */
         {Slot_pushButtonSavModif();
          nom       = G_pCApp->m_LastNom;
          prenom    = G_pCApp->m_LastPrenom;
         }
   //   }
   // pDrTux = new DrTux(argv[1],       // utilisateur
   //                    argv[2],       // num_doss (GUID dossier)
   //                    argv[3],       // ID_Doss (fchgnrl2002_nom_prenom WHERE ID_PrimKey = ID_Doss)
   //                    argv[4],       // nom dossier
   //                    argv[5]        // prenom dossier
   //                   );
   QString usedBy, value;
   if ( m_pCMoteurBase->IsDossierVerrouille( guid, usedBy ))
      {G_pCApp->readParam( "Gestion des dossiers", "Acces Concurrent", &value);   // path editeur de texte non d\303\251fini
       if (value.lower().left(3) == "exc")
          {QMessageBox::information( this, tr(  "FILE IS UNREACHABLE"),
                                     tr(  " <b><b><u>WARNING</b></u> ! This file ")+"<font color=\"#e80d0d\"><b>"          + nom     + " " + prenom  +
                                     tr(  "</b></font> is open on this terminal: <b>" )   + usedBy  +
                                     "</b>.<br />"+ tr( "Its acces is impossible on this terminal.<br />Please, get it closed on its terminal before opening it on this terminal.") ,
                                     tr("&Cancel"),0 , 0,
                                     1, 1 );
           return;
          }
       //.............. si l'utilisateur du dossier est le m\303\252me que celui du poste de travail...................................
       //               alors le dossier est d\303\251j\303\240 lanc\303\251 sur le poste de travail local
       //               on le lance et alors il se met en premier plan (car g\303\250re comme cela l'instance unique)
       // sinon on demande si l'on veut lancer le dossier pr\303\251sent sur un autre poste avec avertisement des
       // cons\303\251quences
       if (m_pCMoteurBase->m_UserName + "@" + m_pCMoteurBase->m_HostName != usedBy &&
           alertVerrou(usedBy, nom, prenom)!=0)
          {return;
          }
      }
   //................. met \303\240 jour le formulaire identite sur le patient ................................

   if (getInterfaceMode() != MODE_CREATION_PATIENT) DataToFormulaire(primKey, nom, prenom, guid);    //==0) return;   // met \303\240 jour  G_pCApp->m_LastNom, G_pCApp->m_LastPrenom, G_pCApp->m_LastGUID, G_pCApp->m_LastPK

   #ifdef Q_WS_WIN
      QString pathExe  = CGestIni::Construct_Name_Exe("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
   #else
      QString pathExe  = CGestIni::Construct_Name_Exe("Manager",QFileInfo (qApp->argv()[0]).dirPath (true), "guinch");
   #endif
   QStringList argList;
   //......................... completer les arguments de lancement.........................................
   argList <<( user );                            // 0
   argList <<( sign_user );                       // 1
   argList <<( guid );                            // 2
   argList <<( primKey );                         // 3
   argList <<( nom.replace("'","~@~") );          // 4
   argList <<( prenom.replace("'","~@~") );       // 5

   QString str  = "";
   argList <<(G_pCApp->pathIni());                // 6
   str = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true), "DataBase.cfg");
   argList <<( str ); // fichier cfgBase          // 7

   setVerrou(TRUE);

   QProcess::startDetached (pathExe, argList);

}

//--------------------------------------- alertVerrou -------------------------------------------------------------------------------
int  C_Manager::alertVerrou(const QString &userBy, const QString &dossNom, const QString &dossPrenom)
{  QString value;
   G_pCApp->readParam( "Gestion des dossiers", "Acces Concurrent", &value);   // path editeur de texte non d\303\251fini
   if (value.lower() == tr("non possible"))
   {  QMessageBox::information( this, tr("CONFLICTING OPENING"),
                                tr(  " <b><u>WARNING</b></u> ! This file ")+"<font color=\"#e80d0d\"><b>"          + dossNom     + " " + dossPrenom  +
                                tr(  "</b></font> is already open on this terminal: <b>" )   + userBy  +
                                tr(  "</b><br /> Its access is read-only,<br />it will turnt into write mode<br />as soon as it will be available on this terminal") ,
                                //"&nbsp;&nbsp;&nbsp;&nbsp; ? <b>Quitter pour <u>Annuler</u></b> l'op\303\251ration en cours" ) ,
                                tr("&Cancel"),0 , 0,
                                1, 1 );
       return 1;
   }
   return QMessageBox::information( this, tr("CONFLICTING OPENING"),
                                    tr ( " <b><u>WARNING</b></u> ! This file <b>")          + dossNom     + " " + dossPrenom  +
                                    tr(  "</b>is already open on this terminal: <b>" )   + userBy  +
                                    tr(  "</b><br> Should we try to open it anyway ?<br />") ,
                                    //"&nbsp;&nbsp;&nbsp;&nbsp; ? <b>Quitter pour <u>Annuler</u></b> l'op\303\251ration en cours" ) ,   1111b0
                                    tr("&Open"), tr("&Cancel"), 0,
                                    1, 1 );
}

///////////////////////////////////////// LECTURE VITALE ////////////////////////////////////////
//--------------------------------- Slot_listView_Vitale_DoubleClicked -----------------------------------------------------------------------
void C_Manager::Slot_listView_Vitale_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int)
{
#ifdef SESAM_VERSION
if (pQTreeWidgetItem==0) return;
Slot_listView_VitaleSelectionChanged (); // se repositionner sur cet ayant droit au cas ou liste modifi\303\251e
//QTreeWidgetItem *pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Vitale);
//if (pQListViewItem==0) return;

int nb = m_pGUI->listView_Patient->topLevelItemCount();
if (pQTreeWidgetItem->text(6).length()==0 /*nb==0*/)           // pas de patient trouv\303\251 pour cette carte vitale alors on le propose en cr\303\251ation
   {if (getInterfaceMode() != MODE_CREATION_PATIENT)
       {   Slot_NewDossWithoutErase(); // se Mettre en mode creation
       }
    // Placer les donn\303\251es SV
    setInterfaceOnVitaleData(  G_pCApp->m_pVitale,                  // donn\303\251es carte vitale
                               pQTreeWidgetItem->text(8).toInt()    // occurence
                            );
   }
else if (nb==1)      // un seul patient trouv\303\251 pour cette carte vitale alors on le lance
   { //Cz_Pyxvital     si on ne gï¿½re pas le dossier Patient on lance directement la FSE
    if (m_Facture_Seule == "oui")
        Slot_pushButton_FSE();
    else
        Slot_pushButtonAcceder_Clicked();
   }
#endif
}

// SIGEMS DEB
//--------------------------------- Slot_listView_Sigems_DoubleClicked -----------------------------------------------------------------------
void C_Manager::Slot_listView_Sigems_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int)
{
if (pQTreeWidgetItem==0) return;

#ifdef ENTREES_SIGEMS

Slot_listView_SigemsSelectionChanged (); // se repositionner sur cet ayant droit au cas ou liste modifiee

int nb = m_pGUI->listView_Patient->topLevelItemCount();
if (pQTreeWidgetItem->text(6).length()==0 /*nb==0*/)           // pas de patient trouv\303\251 pour cette carte vitale alors on le propose en cr\303\251ation
   {if (getInterfaceMode() != MODE_CREATION_PATIENT)
       {   Slot_NewDossWithoutErase(); // se Mettre en mode creation
       }
   // on garde l'ID SIGEM POUR GUID
   m_pGUI->textLabel_NumDoss->setText(pQTreeWidgetItem->text(14));
   // if (G_pCApp->readUniqueParam ( "Connexion", "DebugMode").trimmed()=="1")
   //   {CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli()+"vitale-1.txt",  G_pCApp->m_pVitale->m_VitaleData.toAscii());
   //   }
   // Placer les donnees SV
   setInterfaceOnVitaleData(  G_pCApp->m_pVitale,                  // donn\303\251es carte vitale
                               pQTreeWidgetItem->text(8).toInt()    // occurence
                           );
   }
else if (nb==1)      // un seul patient trouv\303\251 pour cette carte vitale alors on le lance
   {
   // G_pCApp->setCurrentIdentite(pQTreeWidgetItem); // AJOUTE POUR VOIR SI OK ?????????????????

    Slot_pushButtonAcceder_Clicked();
   }
#endif
}

//--------------------------------- Slot_listView_Sigems_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_listView_Sigems_Clicked( QTreeWidgetItem * /*pQListViewItem*/, int /* column*/  )
{
    #ifdef ENTREES_SIGEMS
    // On suspend le timer de refresh de la listView Sigems.
    // car si on change l'item selected pendant la fusion ... ca merde
    if (m_pListUpdateTimer->isActive())
        {m_pListUpdateTimer->stop();
        // On active un timer qui reactvera le timerde refresh au bout de x mn.
        if (m_pSuspendEntrantsTimer == 0)
            m_pSuspendEntrantsTimer       = new QTimer(this);
        if (m_pSuspendEntrantsTimer)
            {connect( m_pSuspendEntrantsTimer, SIGNAL(timeout()), this, SLOT(Slot_OnTimerSuspendEntrants()) );
            }
        m_pSuspendEntrantsTimer->start (m_delaySuspendEntrantsTimer.toInt() * 1000 );
        }
    #endif
}


//----------------------------------- Slot_OnTimerSuspendEntrants ----------------------------------
void C_Manager::Slot_OnTimerSuspendEntrants()
{
    #ifdef ENTREES_SIGEMS
    m_pListUpdateTimer->start();
    #endif
}

// SIGEMS FIN

//--------------------------------- Slot_listView_Vitale_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_listView_Vitale_Clicked( QTreeWidgetItem * /*pQListViewItem*/, int /* column*/  )
{
   #ifdef SESAM_VERSION
   Slot_listView_VitaleSelectionChanged ();
           /*
   if (pQListViewItem==0) return;
   C_Vitale       *pcVitale =  G_pCApp->m_pVitale;
   if (pcVitale && m_pGUI->lineEdit_NumSecu->text().remove(' ')==pcVitale->GetMember(8,  101, 1))
      {
       QString debDroits = pQListViewItem->text(9);
       QString finDroits = pQListViewItem->text(10);
       m_pGUI->lineEdit_DroitsDateDeb->setText(debDroits.mid(6,2)+"/"+debDroits.mid(4,2)+"/"+debDroits.mid(0,4));
       m_pGUI->lineEdit_DroitsDateFin->setText(finDroits.mid(6,2)+"/"+finDroits.mid(4,2)+"/"+finDroits.mid(0,4));
       m_pGUI->lineEdit_CAS->setText(pQListViewItem->text(11));
      }
   else
      {m_pGUI->lineEdit_DroitsDateDeb->setText("");
       m_pGUI->lineEdit_DroitsDateFin->setText("");
       m_pGUI->lineEdit_CAS->setText("");
      }

     //Slot_listView_VitaleSelectionChanged ();
      */
   #endif
}

// SIGEMS DEB
//--------------------------------- Slot_listView_VitaleSelectionChanged -----------------------------------------------------------------------
void C_Manager::Slot_listView_VitaleSelectionChanged ()
{   Slot_listView_VitaleOrSigemsSelectionChanged ("VITALE");
}
//--------------------------------- Slot_listView_SigemsSelectionChanged -----------------------------------------------------------------------
void C_Manager::Slot_listView_SigemsSelectionChanged ()
{   Slot_listView_VitaleOrSigemsSelectionChanged ("SIGEMS");
}

//--------------------------------- Slot_listView_VitaleOrSigemsSelectionChanged -----------------------------------------------------------------------
void C_Manager::Slot_listView_VitaleOrSigemsSelectionChanged (QString WhatListView)
{

#ifdef SESAM_VERSION
QTreeWidget     *pQlistView = m_pGUI->listView_Patient;
QTreeWidgetItem *pQListViewItem;
//if (WhatListView == "SESAM")
if (WhatListView == "VITALE")
    { pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Vitale);
      if (pQListViewItem==0) return;
    }
else
    { pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Sigems);
      if (pQListViewItem==0) return;
      create_CV_with_patient_Sigems();
    }
// SIGEMS FIN

C_Vitale *pcVitale = G_pCApp->m_pVitale;
int occurence      = pQListViewItem->text(8).toInt();
//if (G_pCApp->readUniqueParam ( "Connexion", "DebugMode").trimmed()=="1")
//   {CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli()+"vitale-Slot_listView_VitaleOrSigemsSelectionChanged.txt",  pcVitale->m_VitaleData.toAscii());
//   }
//................. determine si le nom prenom entrent dans la comparaison ..........................
//QString localParam = "";   CGestIni::Param_UpdateFromDisk(G_pCApp->pathIni(), localParam);
bool matchOnNom    = (G_pCApp->readUniqueParam( "Sesam-Vitale", "Compare_Nom").toLower()    == "ok");
bool matchOnPrenom = (G_pCApp->readUniqueParam( "Sesam-Vitale", "Compare_Prenom").toLower() == "ok");

//............ formater qq trucs pour les rendre compatble avec le match SQL ....................
QString dnss      = pQListViewItem->text(2).left(8);             //  199306010000   -->  19930601
        dnss      = dnss.insert(4,'-');                          //  19930601       -->  1993-0601
        dnss      = dnss.insert(7,'-');                          //  1993-0601      -->  1993-06-01
        dnss     += " 00:00:00";                                 //  1993-0601      -->  1993-06-01 00:00:00
QString numSecu   = pQListViewItem->text(3);
        numSecu  += CMoteurBase::ComputeClef_Secu(numSecu);
QString rangGem   = pQListViewItem->text(4);
QString debDroits = pQListViewItem->text(9);
QString finDroits = pQListViewItem->text(10);
//............. remplir la zone des droits quelque soit le numero de secu CV-GUI .............................
 setInterfaceOnDroitsSociaux(pcVitale, occurence, 1);
//QString    cas = pcVitale->GetMember(10,  101, 1)+pcVitale->GetMember(11,  101, 1)+pcVitale->GetMember(12,  101, 1).left(3);


//m_pGUI->lineEdit_CAS->setText(pQListViewItem->text(11));
//............ determine si le nom prenom entrent dans la comparaison .....................
QString     nom    = pQListViewItem->text(0);
QString     prenom = pQListViewItem->text(1);
if (QDate::isValid ( dnss.mid(0,4).toInt(), dnss.mid(5,2).toInt(), dnss.mid(8,2).toInt() ) )  // si date de naiss invalide on est oblig\303\251 de se servir du nom prenom
   { if (!matchOnNom)     nom    = "";
     if (!matchOnPrenom)  prenom = "";
   }
if (rangGem.length()==1) rangGem = rangGem.prepend("0");
//.................... lors de cet appel la listview est \303\240 rebattir .....................
//                     et donc appel \303\240 SelectionChanged
//                     donc modification de L'identit\303\251 globale
//                     donc appel \303\240 sauvegarde avant
if (getInterfaceMode()==MODE_SELECTION_PATIENT && identIsModified() && G_pCApp->isCurrentIdentiteValid() && G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idc") )
   {savModifications(G_pCApp->identitePatientNom(), G_pCApp->identitePatientPrenom(), G_pCApp->identitePatientPk(), G_pCApp->identitePatientGUID());
   }

int nb = G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme( pQlistView,
                                                                            nom,                         // nom
                                                                            prenom,                      // prenom
                                                                            dnss,                        // date Nss // doit etre 1993-06-01 00:00:00
                                                                            numSecu,                     // num secu // 160196296519557
                                                                            rangGem,                     // rang gemellaire
                                                                            pQListViewItem->text(5),     // qualite d'ayant droit
                                                                            "",      //  adresse,
                                                                            "",      //  codePost,
                                                                            "",      //  ville,
                                                                            "",      //  sexe,
                                                                            "",      //  profession,
                                                                            "",      //  nomJfille
                                                                            "",      //  nomAss,
                                                                            "",      //  prenomAss,
                                                                            "",      //  tel1,
                                                                            "",      //  tel2,
                                                                            "",      //  tel3,
                                                                            "",      //  email,
                                                                            " = ",   //  egal_like  " = ",
                                                                            m_pGUI->textLabel_StateList
                                                                          );
// if (nb==0)                // pas de patient trouv\303\251 pour cette carte vitale
// else if (nb==1)          // un seul patient trouv\303\251 pour cette carte vitale alors on efface l'\303\251ventuel patient en cours de cr\303\251ation
switch(getInterfaceMode())
      {case MODE_SELECTION_PATIENT:
            {if (nb==1)                    // un seul patient trouv\303\251 pour cette carte vitale alors on efface l'\303\251ventuel patient en cours de cr\303\251ation
                {QTreeWidgetItem* pQTreeWidgetItem = pQlistView->topLevelItem(0) ;// il n'y en a qu'un 'if (nb==1)' donc le premier est le bon
                 if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
                }
             else //if (nb==0)                // pas de patient trouv\303\251 pour cette carte vitale ou plus d'un
                {setIdentiteDisabled();
                 clearIdentFields();
                 QString dtnss = pQListViewItem->text(2).left(8);
                 if (dtnss.length()) listPatientsByDateNss(dtnss);
                }
             setInterfaceOnDroitsSociaux(pcVitale, occurence);
            }break;
       case MODE_CREATION_PATIENT:
            {/*
             int ret   = QMessageBox::warning ( this, tr("S\303\251lection de la liste de la carte vitale :"),
                                    tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp;voulez utiliser les donn\303\251es de l'ayant droit suivant :<br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                    pQListViewItem->text(0)  + " " + pQListViewItem->text(1)  +
                                    tr(  "</font></b><br>&nbsp;&nbsp; pour remplir les donn\303\251es du patient en cours de cr\303\251ation ?"),
                                    tr("&Utiliser"), tr("Annu&ler"), 0,
                                    0, 1
                                    );
             if (ret==0)
                {setInterfaceOnVitaleData(G_pCApp->m_pVitale,                        // donn\303\251es carte vitale
                                          pQListViewItem->text(8).toInt()            // occurence de l'ayant droit
                                         );
                }
              */
              //setInterfaceOnDroitsSociaux(pcVitale);
             }break;
       case MODE_MULTICRITERE:
            {
            }break;
       } //endswitch(getInterfaceMode())
#endif
}

//------------------------------------------------------- compareListVitaleWithBase --------------------------------------------------
//    met \303\240 jour la listview vitale sur patient trouv\303\251 dans la base des patient ou pas
void  C_Manager:: compareListVitaleWithBase()
{
#ifdef SESAM_VERSION
       //................. determine si le nom prenom entrent dans la comparaison ..........................
       //QString localParam = "";   CGestIni::Param_UpdateFromDisk(G_pCApp->pathIni(), localParam);
       bool matchOnNom    = (G_pCApp->readUniqueParam( "Sesam-Vitale", "Compare_Nom").toLower()    == "ok");
       bool matchOnPrenom = (G_pCApp->readUniqueParam( "Sesam-Vitale", "Compare_Prenom").toLower() == "ok");

       QTreeWidgetItemIterator it( m_pGUI->listView_Vitale );
       while ( *it )
          {
             QTreeWidgetItem *pQListViewItem = *it;
             //............ formater qq trucs pour les rendre compatble avec le match SQL ....................
             QString dnss     = pQListViewItem->text(2).left(8);             //  199306010000   -->  19930601
                     dnss     = dnss.insert(4,'-');                          //  19930601       -->  1993-0601
                     dnss     = dnss.insert(7,'-');                          //  1993-0601      -->  1993-06-01
                     dnss    += " 00:00:00";                                 //  1993-0601      -->  1993-06-01 00:00:00
             QString numSecu  = pQListViewItem->text(3);
                     numSecu += CMoteurBase::ComputeClef_Secu(numSecu);
             QString rangGem  = pQListViewItem->text(4);
             if (rangGem.length()==1) rangGem = rangGem.prepend("0");
             //............ determine si le nom prenom entrent dans la comparaison .....................
             QString nom    = pQListViewItem->text(0);
             QString prenom = pQListViewItem->text(1);
             if (QDate::isValid ( dnss.mid(0,4).toInt(), dnss.mid(5,2).toInt(), dnss.mid(8,2).toInt() ) )  // si date de naiss invalide on est oblig\303\251 de se servir du nom prenom
             {
                 if (!matchOnNom)     nom    = "";
                 if (!matchOnPrenom)  prenom = "";
             }
             if (G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme( 0,                           // pas de listview car on veut juste savoir si trouve
                                                                                    nom,        // nom
                                                                                    prenom,     // prenom
                                                                                    dnss,                        // date Nss // doit etre 1993-06-01 00:00:00
                                                                                    numSecu,                     // num secu // 160196296519557
                                                                                    rangGem,                     // rang gemellaire
                                                                                    pQListViewItem->text(5),     // qualite d'ayant droit
                                                                                    "",      //  adresse,
                                                                                    "",      //  codePost,
                                                                                    "",      //  ville,
                                                                                    "",      //  sexe,
                                                                                    "",      //  profession,
                                                                                    "",      //  nomJfille
                                                                                    "",      //  nomAss,
                                                                                    "",      //  prenomAss,
                                                                                    "",      //  tel1,
                                                                                    "",      //  tel2,
                                                                                    "",      //  tel3,
                                                                                    "",      //  email,
                                                                                    " = ",   //  egal_like  " = ",
                                                                                    m_pGUI->textLabel_StateList
                                                                                  )
                )
                {pQListViewItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/VitaleOk.png")) );
                 pQListViewItem->setText(6, "-");                                           // 6 pk doss
                }
             else
                {pQListViewItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/VitaleNotOk.png")) );
                 pQListViewItem->setText(6, "");
                }
           ++it;
          }
#endif
}

//--------------------------------- Slot_listView_Vitale_MenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_listView_VitaleOrSigems_MenuRequested (QString WhatTreeView) // SIGEMS
{ //QTreeWidget         *pQlistView = m_pGUI->listView_Patient;
#ifdef SESAM_VERSION
    QTreeWidgetItem *pQListViewItem;
    if (WhatTreeView == "SESAM")
    {
    pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Vitale);
    if (pQListViewItem==0)     return;
    if (G_pCApp->m_pVitale==0) return;
    }
    else
    {
    pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Sigems);
    if (pQListViewItem==0)     return;
    create_CV_with_patient_Sigems();
    if (G_pCApp->m_pVitale==0) return;
    }

C_Vitale       *pcVitale =  G_pCApp->m_pVitale;
int               action = -1;
QString       val_actual = "";
QString              tmp = "";
int            occurence = pQListViewItem->text(8).toInt();
int  qualiteAyantDroit_V = pcVitale->GetMember(14,  104, occurence).toInt();
int  qualiteAyantDroit_P = m_pGUI->comboBoxQualiteAyantDroit->currentItem ();


QString p_nom, p_prenom;  pcVitale->GetNomPrenomAyanDroit(occurence, ";", &p_nom, &p_prenom);
QStringList list;

//....................... si l'identite de la CV selectionnee correspondant \303\240 un dossier d\303\251ja en base .................
//                        on propose de completer le dossier avec les donn\303\251es qui diff\303\250rent
//                        que si le patient selectionne dans liste des patients est le m\303\252me que celui de la CV
if (pQListViewItem->text(6).length()                                      &&
    pcVitale->Compare(occurence, m_pGUI->lineEdit_NumSecu->text().remove(' '),
                                 m_pGUI->lineEdit_DtNss->text().remove('-'),
                                 m_pGUI->lineEdit_RangGeme->text())==0  &&
                                 getInterfaceMode()==MODE_SELECTION_PATIENT
   )
   {tmp = pcVitale->GetMember(1,  104, occurence) + " " + pcVitale->GetMember(3,  104, occurence);
    if (m_pGUI->lineEdit_NomDossier->text()+" "+m_pGUI->lineEdit_PrenomDossier->text() != tmp)
        list.append(tr("=1=Mettre \303\240 jour l'identit\303\251 avec le nom et pr\303\251nom de l'ayant droit : ") + tmp);

    tmp = pcVitale->GetMember(2,  104, occurence).trimmed();
    if (tmp.length() && m_pGUI->lineEdit_NomDeNss->text().remove(' ') != tmp)
        list.append(tr("=2=Mettre \303\240 jour l'identit\303\251 avec le nom de naissance : ") + tmp);
    /*
    tmp =  pcVitale->GetMember(8,  101, 1);                      // ce menu ne devrait pas etre possible car match obligatoire pour cette donn\303\251e
    if (tmp.length()>1 && tmp != m_pGUI->lineEdit_NumSecu->text())
        list.append(tr("3- Mettre \303\240 jour l'identit\303\251 avec le num\303\251ro de SS : ") + tmp + " "+CMoteurBase::ComputeClef_Secu(tmp));
    */
    tmp =  pcVitale->GetAdresseAyanDroit(occurence);
    if (tmp.length()==0) tmp =  pcVitale->GetAdresseAssure();
    val_actual = m_pGUI->textEdit_Adresse->text();
    if (tmp.length() &&  val_actual != tmp)
        list.append(tr("=4=Mettre \303\240 jour l'identit\303\251 avec l'adresse : ") + tmp);

    pcVitale->GetCodePostalVilleAyanDroit(occurence, &tmp,0);  // code postal ville
    if (tmp.length()==0) pcVitale->GetCodePostalVilleAssure(&tmp, 0);
    if (tmp.length() && m_pGUI->lineEdit_CdPostal->text().remove(' ') != tmp)
        list.append(tr("=5=Mettre \303\240 jour l'identit\303\251 avec le code postal : ") + tmp);

    pcVitale->GetCodePostalVilleAyanDroit(occurence,0, &tmp);  // code postal ville
    if (tmp.length()==0) pcVitale->GetCodePostalVilleAssure(0 ,&tmp);
    if (tmp.length() && m_pGUI->lineEditVille->text() != tmp)
        list.append(tr("=6=Mettre \303\240 jour l'identit\303\251 avec la ville : ") + tmp);

    tmp = pcVitale->GetNomPrenomAssure();
    if (tmp.length() && m_pGUI->lineEdit_NomAssure->text()+" "+m_pGUI->lineEdit_PrenomAssure->text() != tmp)
        list.append(tr("=7=Mettre \303\240 jour l'identit\303\251 avec le nom et pr\303\251nom de l'assur\303\251 : ") + tmp);
    /*
    tmp = pcVitale->GetRangGemellaire(occurence);                  // ce menu ne devrait pas etre possible car match obligatoire pour cette donn\303\251e
    if (tmp.length() && m_pGUI->lineEdit_RangGeme->text() != tmp)
        list.append(tr("=8=Mettre \303\240 jour l'identit\303\251 avec le rang g\303\251mellaire : ") + tmp);

    tmp = pcVitale->GetDateNaissanceAyantDroit(occurence,0,"/");  // ce menu ne devrait pas etre possible car match obligatoire pour cette donn\303\251e
    if (tmp.length() && m_pGUI->lineEdit_DtNss->getMixText() != tmp)
        list.append(tr("=9=Mettre \303\240 jour l'identit\303\251 avec la date de naissance : ") + tmp);
    */
    if (qualiteAyantDroit_V !=  qualiteAyantDroit_P)
        list.append(tr("=10=Mettre \303\240 jour l'identit\303\251 avec le type d'ayant droit : ") + pcVitale->CodeQualiteToName( QString::number(qualiteAyantDroit_V )));

    if (list.count()) list.append(tr("=11=Mettre \303\240 jour l'identit\303\251 avec toutes les donn\303\251es de la carte vitale ") );
   }

//............... Menu recherche pour toutes les identites de la CV ..........................
list.append("----------");
tmp =  pcVitale->GetMember(8,  101, 1); tmp += " "+CMoteurBase::ComputeClef_Secu(tmp);
if (tmp.length()>1) list.append(tr("=23=Rechercher avec le num\303\251ro de SS : ") + tmp);

tmp = pcVitale->GetNomPrenomAyanDroit(occurence);
if (tmp.length()) list.append(tr("=27=Rechercher avec le nom et pr\303\251nom de l'ayant droit : ") + tmp);

pcVitale->GetNomPrenomAyanDroit(occurence, ";", &p_nom, &p_prenom);
if (p_nom.length())    list.append(tr("=28=Rechercher avec le nom de l'ayant droit    : ")    + p_nom);
if (p_prenom.length()) list.append(tr("=29=Rechercher avec le pr\303\251nom de l'ayant droit : ")    + p_prenom);

tmp = pcVitale->GetDateNaissanceAyantDroit(occurence);
if (tmp.length()) list.append(tr("=30=Rechercher avec la date de naissance : ") + tmp);

//............... que si identite de la CV ne correspond pas \303\240 un dossier en base ........................
//                on propose de l'attribuer au dossier selectionne
if (pQListViewItem->text(6).length()==0)
   {list.append("-------------");
    if ( getInterfaceMode()==MODE_CREATION_PATIENT)
       { list.append("-------------");
         list.append(tr("=33=Utiliser les donn\303\251es de la carte Vitale : %1 pour remplir celles du patient en cours de cr\303\251ation").arg(p_nom + " " + p_prenom));
       }
    else
       { list.append(tr("=31=Cr\303\251er un nouveau dossier avec l'identit\303\251 s\303\251lectionn\303\251e de la carte vitale : ") + p_nom + " " + p_prenom);
         list.append("-------------");
         QTreeWidgetItem *pQListViewItemPatient =  getSelectedListViewItem(m_pGUI->listView_Patient);
         if (pQListViewItemPatient)
            {list.append(tr("=32=Attribuer les \303\251l\303\251ments de cette carte vitale au patient s\303\251lectionn\303\251 : ")+ pQListViewItemPatient->text(0) + " " + pQListViewItemPatient->text(1));
            }
       }
   }

//.................. actionner le menu et les actions .................................................
action = G_pCApp->DoPopupList(list).toInt();
switch(action)
       {case 1:
             {m_pGUI->lineEdit_NomDossier->setText(      pcVitale->GetMember(1,  104, occurence));
              m_pGUI->lineEdit_PrenomDossier->setText(   pcVitale->GetMember(3,  104, occurence));
             } break;
        case 2:
             {m_pGUI->lineEdit_NomDeNss->setText(   pcVitale->GetMember(2,  104, occurence));
             } break;
        case 3:
             {tmp =  pcVitale->GetMember(8,  101, 1);
              m_pGUI->lineEdit_NumSecu->setText(tmp);
              m_pGUI->lineEdit_NssClef->setText(CMoteurBase::ComputeClef_Secu(tmp));
             } break;
        case 4:
             {tmp =  pcVitale->GetAdresseAyanDroit(occurence);
              if (tmp.length()==0) tmp =  pcVitale->GetAdresseAssure();
              m_pGUI->textEdit_Adresse->setText(tmp);
             } break;
        case 5:
             {pcVitale->GetCodePostalVilleAyanDroit(occurence, &tmp,0);  // code postal
              if (tmp.length()==0) pcVitale->GetCodePostalVilleAssure(&tmp, 0);
              m_pGUI->lineEdit_CdPostal->setText(tmp);
              tryToSetVilleFromCodePostal();
             } break;
        case 6:
             {pcVitale->GetCodePostalVilleAyanDroit(occurence,0, &tmp);  // ville
              if (tmp.length()==0) pcVitale->GetCodePostalVilleAssure( 0, &tmp);
              m_pGUI->lineEditVille->setText(tmp);
             tryToSetCodePostalFromVille();
             } break;
        case 7:
             {pcVitale->GetNomPrenomAssure(" ", &p_nom, &p_prenom);
              m_pGUI->lineEdit_NomAssure->setText(p_nom);
              m_pGUI->lineEdit_PrenomAssure->setText(p_prenom);
             } break;
        case 8:
             {tmp = pcVitale->GetRangGemellaire(occurence);
              m_pGUI->lineEdit_RangGeme->setText(tmp);
             } break;
        case 9:
             {tmp = pcVitale->GetDateNaissanceAyantDroit(occurence,0,"");
              m_pGUI->lineEdit_DtNss->setText(tmp);
             } break;
        case 10:
             {m_pGUI->comboBoxQualiteAyantDroit->setCurrentItem ( qualiteAyantDroit_V );
              //m_pGUI->OnActivated_comboBoxQualiteAyantDroit( qualiteAyantDroit_V );
              if (m_pGUI->comboBoxSexe->currentItem()==2)  tryToSetInterfaceFromAyantDroit( m_pGUI->comboBoxQualiteAyantDroit->currentIndex(), 0);
             } break;
        case 11:
             {setInterfaceOnVitaleData(G_pCApp->m_pVitale,occurence );
             } break;
        case 23:
             {tmp =  pcVitale->GetMember(8,  101, 1); tmp +=CMoteurBase::ComputeClef_Secu(tmp);
              if (G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme(m_pGUI->listView_Patient,         // pas de listview car on veut juste savoir si trouve
                                                                                    "",         // nom
                                                                                    "",         // prenom
                                                                                    "",         // date Nss // doit etre 1993-06-01 00:00:00
                                                                                    tmp,        // num secu // 160196296519557
                                                                                    "",         // rang gemellaire
                                                                                    "",         // qualite d'ayant droit
                                                                                    "",         // adresse,
                                                                                    "",         // codePost,
                                                                                    "",         // ville,
                                                                                    "",         // sexe,
                                                                                    "",         // profession,
                                                                                    "",      //  nomJfille
                                                                                    "",      //  nomAss,
                                                                                    "",      //  prenomAss,
                                                                                    "",      //  tel1,
                                                                                    "",      //  tel2,
                                                                                    "",      //  tel3,
                                                                                    "",      //  email,
                                                                                    " LIKE ",   // egal_like  " = ",
                                                                                    m_pGUI->textLabel_StateList
                                                                                  )
                 )
                 {
                 }
             } break;
        case 27:
             {tmp = pcVitale->GetNomPrenomAyanDroit(occurence,";");
              m_pGUI->lineEditAutoLocator->setText(tmp);
              //m_pGUI->lineEditAutoLocator_textChanged( tmp );
             } break;
        case 28:
             {pcVitale->GetNomPrenomAyanDroit(occurence, ";", &p_nom, &p_prenom);
              m_pGUI->lineEditAutoLocator->setText(p_nom);
              //m_pGUI->lineEditAutoLocator_textChanged( p_nom );
             } break;
        case 29:
             {pcVitale->GetNomPrenomAyanDroit(occurence, ";", &p_nom, &p_prenom);
              m_pGUI->lineEditAutoLocator->setText(p_prenom.prepend(";%"));
              //m_pGUI->lineEditAutoLocator_textChanged( p_prenom );
             } break;
        case 30:
             {listPatientsByDateNss(pcVitale->GetMember(12, 104, occurence).left(8));
             } break;
        case 31:
             {Slot_NewDossWithoutErase();
              setInterfaceOnVitaleData(G_pCApp->m_pVitale,                // donn\303\251es carte vitale
                                       occurence                          // occurence
                                      );
             } break;
        case 33:
             {setInterfaceOnProfil(MODE_CREATION_PATIENT);
              setInterfaceOnVitaleData(G_pCApp->m_pVitale,                // donn\303\251es carte vitale
                                       occurence                          // occurence
                                      );
             } break;

        case 32:
             {setCVonPatient (pcVitale,  occurence, "VITALE");
             } break;
       }
#endif
}
//--------------------------------- setCVonPatient -----------------------------------------------------------------------
void C_Manager::setCVonPatient (C_Vitale *pcVitale, int occurence, QString VitaleOrSigems)
{   QString str_mess1, str_mess2;
    if (VitaleOrSigems == "VITALE")
     {  str_mess1 = "la carte Vitale";
        str_mess2 = "Vitaliser un patient";
     }
    else
    {  str_mess1 = "l'entr\303\251e Sigems";
       str_mess2 = "Association Sigems";
    }
#ifdef SESAM_VERSION
    // SIGEMS DEB
    // SIGEMS FIN
   QString tmp      = pcVitale->GetNomPrenomAyanDroit(occurence, " ").replace('\'',' ').replace('-',' ');
   tmp              = tmp.simplified();   tmp   = tmp.trimmed();   tmp   = tmp.upper();
   QString p_nom    = m_pGUI->lineEdit_NomDossier->text() + " "+m_pGUI->lineEdit_PrenomDossier->text();
   p_nom            = p_nom=p_nom.replace('\'',' ').replace('-',' '); p_nom.simplified(); p_nom = p_nom.trimmed(); p_nom = p_nom.upper();
   QString dnss_1   = pcVitale->GetDateNaissanceAyantDroit(occurence,0,"-");
   QString dnss_2   = m_pGUI->lineEdit_DtNss->text();
   QString cd_p_1   = ""; pcVitale->GetCodePostalVilleAssure(&cd_p_1, 0);
   QString cd_p_2   = m_pGUI->lineEdit_CdPostal->text().remove(' ');
   QString secu_1   = pcVitale->GetMember(8,  101, 1);
   QString secu_2   = m_pGUI->lineEdit_NumSecu->text().remove(' ');
   int nb_criteres  = 0;
   QString mess_Cri = "";
   if (tmp    != p_nom)                                    {mess_Cri += tr( "<br>&nbsp;&nbsp;&nbsp;&nbsp;- le nom pr\303\251nom doit changer pour  : ")  + "<b><font color=\"#e80d0d\">"  + pcVitale->GetNomPrenomAyanDroit(occurence, " ")  + "</font></b>,"; ++nb_criteres;}
   if (dnss_1 != dnss_2)                                   {mess_Cri += tr( "<br>&nbsp;&nbsp;&nbsp;&nbsp;- la date de naissance doit changer pour  :")   + "<b><font color=\"#e80d0d\">"   + dnss_1       + "</font></b>,"; ++nb_criteres;}
   if (cd_p_1.toInt() && cd_p_1.toInt() != cd_p_2.toInt()) {mess_Cri += tr( "<br>&nbsp;&nbsp;&nbsp;&nbsp;- le code postal doit changer pour  :")         + "<b><font color=\"#e80d0d\">"         + cd_p_1       + "</font></b>,"; ++nb_criteres;}
   if (secu_1 != secu_2)                                   {mess_Cri += tr( "<br>&nbsp;&nbsp;&nbsp;&nbsp;- le num\303\251ro de SS doit changer pour  :") +" <b><font color=\"#e80d0d\">" + secu_1       + "</font></b>."; ++nb_criteres;}
   QString message  =  "<b><font color=\"#e80d0d\">"+tr("ATTENTION OP\303\211RATION \303\200 HAUT RISQUE </font></b>")                           +
                      tr(  "<hr><br> La personne : <b><font color=\"#1200ff\">") + m_pGUI->lineEdit_NomDossier->text() + " "+m_pGUI->lineEdit_PrenomDossier->text()          + "</font></b>" +
                      tr(  "<br> n\303\251e le : <b><font color=\"#1200ff\">" )  + dnss_2                                                                                    + "</font></b>";
   if (cd_p_2.toInt())
       message += tr(  "<br> habitant : <b>%1 %2</b>").arg(m_pGUI->lineEdit_CdPostal->text(), m_pGUI->lineEditVille->text());

   message += tr(  "<br> ayant pour num\303\251ro de SS : <b>")              + secu_2                       + "</b><br>";
   if (nb_criteres==1)
      {message += "<hr><br><img src=%1/32x32/warning.png> "+tr("diff\303\250re <b><font color=\"#e80d0d\">du</font></b> crit\303\250re suivant \303\240 modifier :").arg(Theme::getPath(Theme::WithNoSeparator));
       message += mess_Cri;
      }
   else if (nb_criteres)
      {message += "<hr><br><img src=%1/32x32/warning.png>"+tr(" diff\303\250re d'au moins des <b><font color=\"#e80d0d\">").arg(Theme::getPath(Theme::WithNoSeparator))+ QString::number(nb_criteres) + tr("</font></b> crit\303\250res suivants \303\240 modifier :");
       message += mess_Cri;
      }
   else
      {message += tr(  "<hr><br>semble \303\252tre la m\303\252me que celle de la carte Vitale<br>nom, pr\303\251nom, num\303\251ro de SS, date de naissance identiques.");
      }
   message += tr("<br><hr>Confirmez-vous tout de m\303\252me cette op\303\251ration ? ");
   if (m_pGUI->lineEdit_RangGeme->text().length())
       message += tr("<br>&nbsp;&nbsp;&nbsp;&nbsp;=> en sachant que <b>ce patient a d\303\251j\303\240 probablement \303\251t\303\251 cr\303\251\303\251 par une carte vitale</b>. ");
   if (tmp    != p_nom)
      {message += tr("<br>&nbsp;&nbsp;&nbsp;&nbsp;=> en sachant qu'un changement de nom et pr\303\251nom est suspect,");
       message += tr(" car tous <b>les documents du dossier en cours vont \303\252tre attribu\303\251s au nouveau nom</b>. ");
      }
   if (dnss_1 != dnss_2)
       message += tr("<br>&nbsp;&nbsp;&nbsp;&nbsp;=> en sachant qu'un changement de date de naissance est suspect. ");

   int sel = QMessageBox::information( this, tr("Vitaliser un patient"), message ,
                                      //"&nbsp;&nbsp;&nbsp;&nbsp; ? <b>Quitter pour <u>Annuler</u></b> l'op\303\251ration en cours" ) ,
                                      tr("&Cancel"), tr("Confirm"), 0 /*tr("Annuler")*/,
                                      0, 0 );
   if (sel != 1) return;
   message  = "<b><font color=\"#e80d0d\">"+tr("DO YOU CONFIRM THIS OPERATION ? </font></b>") +
              tr(  "<hr><br> il est encore temps d'annuler cette op\303\251ration.") +
              tr(  "<br> qui sera non r\303\251versible apr\303\250s confirmation." ) +
              tr(  "<br><hr>confirmez vous de nouveau cette op\303\251ration ? ");
   sel = QMessageBox::information( this, tr("Attribution d'une carte Vitale \303\240 une identit\303\251 patient"), message,
                                         tr("&Attribuer la carte Vitale"),tr("&Cancel") , 0 /*tr("Annuler")*/,
                                         1, 1 );
   if (sel == 1) return;
   setInterfaceOnVitaleData(G_pCApp->m_pVitale,occurence );
   Slot_pushButtonSavModifWithoutConfirmation();
#endif
}

// SIGEMS DEB
//--------------------------------- Slot_listView_Vitale_ContextMenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_listView_Vitale_ContextMenuRequested (const QPoint &)
{Slot_listView_VitaleOrSigems_MenuRequested("SESAM");
}
//--------------------------------- Slot_listView_Vitale_ContextMenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_listView_Sigems_ContextMenuRequested (const QPoint &)
{Slot_listView_VitaleOrSigems_MenuRequested("SIGEMS");
}
// SIGEMS FIN

//--------------------------------- Slot_pushButtonVitale_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonVitale_clicked()
{
#ifdef SESAM_VERSION
int last_occurence    = 0;

QTreeWidget *pQlistView = m_pGUI->listView_Vitale;
pQlistView->show();                                    // afficher la liste de la carte vitale

QString pathPlugin, pathLog, moduleName;
G_pCApp->readParam( "Sesam-Vitale", "pathPlugin",       &pathPlugin);
G_pCApp->readParam( "Sesam-Vitale", "pathLog",          &pathLog);
G_pCApp->readParam( "Sesam-Vitale", "ModuleName",       &moduleName);

//Cz_Pyxvital -------------------------DEBUT -----------------------------------------------------------------
if (pathPlugin.length())
     {
     pQlistView->clear();
     QApplication::processEvents();     // PYX_11_2014

     if (moduleName=="PYXVITAL")
       {
       if (m_timerFSE != 0 && m_timerFSE->isActive() && !m_FSEenCours)     // arret du controle du Patient en cours sur Pyxvital
           m_timerFSE->stop();
       // On lance Aixvital qui appelera Pyxvital pour lire la Carte Vitale en aveugle et retourne le fichier Patient.par
       m_ficPatientPar = appelPyxvital ( "m_Command = CVH\r\n");
       // au retour de Pyxvital on alimente la structure m_pVitale avec les donn\303\251es de Patient.par
       // pour continuer ensuite avec le traitement standard ...
       QSettings *settings = new QSettings(m_ficPatientPar, QSettings::IniFormat);
       QString    nss = settings->value(tr("B\303\251n\303\251ficiaire/Num\303\251ro")).toString();
       if (G_pCApp->m_pVitale) delete G_pCApp->m_pVitale;
       G_pCApp->m_pVitale = new C_Vitale;
       G_pCApp->m_pVitale->SetMember(8, 101, nss, nss.length(), 1);                        // Num S\303\251cu Assur\303\251
       int i;
       QString     zitem = "toto";
       QStringList zamo;
       QString numBenef, zdat;

       // for (i=1; i<100; i++)     // PYX_11_2014
       for (i=1; i<2; i++)          // PYX_11_2014
           {
                  //numBenef = QString("B\303\251n\303\251ficiaire_%1").arg(i) ;
                  numBenef = "B\303\251n\303\251ficiaire" ;                         // PYX_11_2014

                                                                                 // BUG : 1er champ pas pris en compte
                  G_pCApp->m_pVitale->SetMember(8, 104, "", 0, i);               // CP bidon pour BUG ?????
                                                                                 // REvoir initialisation de m_pVitale
                  zitem = settings->value(tr(numBenef) + tr("/Nom")).toString();
                  if (zitem.length() < 1) break;
                  G_pCApp->m_pVitale->SetMember(1, 104, zitem, zitem.length(), i);               // Nom
                  zitem = settings->value(tr(numBenef) + tr("/Code_qualit\303\251")).toString();
                  //if (zitem.length()==1) zitem = zitem.prepend("0");
                  G_pCApp->m_pVitale->SetMember(14, 104, zitem, zitem.length(), i);              // Qualit\303\251 Ayant Droit
                  zitem = settings->value(tr(numBenef) + tr("/Pr\303\251nom")).toString() ;
                  G_pCApp->m_pVitale->SetMember(3, 104, zitem, zitem.length(), i);               // Pr\303\251nom
                  zitem = settings->value(tr(numBenef) + tr("/Date_de_naissance")).toString() ;          // 15/02/1958
                  QString dnss     = zitem.mid(6,4) + zitem.mid(3,2) + zitem.mid(0,2) + "0000";  // 195802150000
                  G_pCApp->m_pVitale->SetMember(12, 104, dnss, dnss.length(), i);                // Date naissance
                  zitem = settings->value(tr(numBenef) + tr("/Rang_g\303\251mellaire")).toString();
                  G_pCApp->m_pVitale->SetMember(13, 104, zitem, zitem.length(), i);              // Rang G\303\251mellaire

                  zitem = settings->value(tr("M\303\251dico_administratif/Code_r\303\251gime")).toString();
                  G_pCApp->m_pVitale->SetMember(10, 101, zitem, zitem.length(), i);              // Code Regime
                  zitem = settings->value(tr("M\303\251dico_administratif/Caisse_gestionnaire")).toString();
                  G_pCApp->m_pVitale->SetMember(11, 101, zitem, zitem.length(), i);              // Caisse_gestionnaire
                  zitem = settings->value(tr("M\303\251dico_administratif/Centre_gestionnaire")).toString();
                  G_pCApp->m_pVitale->SetMember(12, 101, zitem, zitem.length(), i);              // Centre_gestionnaire
                  zitem = settings->value(tr("M\303\251dico_administratif/Code_gestion")).toString();
                  G_pCApp->m_pVitale->SetMember(13, 101, zitem, zitem.length(), i);              // Code_gestion
                  zdat  = "";
                  zitem = settings->value(tr("B\303\251n\303\251ficiaire/P\303\251riodes_AMO_1")).toString();
                  zamo  = zitem.split('+', QString::SkipEmptyParts);
                  if (zamo.size() > 0)
                      zdat  = zamo[zamo.size()-1];
                  if (zdat.length() != 10)
                      zitem = "";
                  else
                      zitem = zdat.mid(6,4) + zdat.mid(3,2) + zdat.mid(0,2);
                  G_pCApp->m_pVitale->SetMember(1, 105, zitem, zitem.length(), i);              // Debut droits AMO
                  zitem = settings->value(tr("B\303\251n\303\251ficiaire/P\303\251riodes_AMO_2")).toString();
                  zamo  = zitem.split('+', QString::SkipEmptyParts);
                  if (zamo.size() > 0)
                      zdat  = zamo[zamo.size()-1];
                  if (zdat.length() != 10)
                      zitem = "";
                  else
                      zitem = zdat.mid(6,4) + zdat.mid(3,2) + zdat.mid(0,2);
                  G_pCApp->m_pVitale->SetMember(2, 105, zitem, zitem.length(), i);              // Fin droits AMO
           }   // fin du for i
          last_occurence = i;      // A revoir ??
       //Cz_Pyxvital ---------------------------FIN-----------------------------------------------------
   }   // fin if PYXVITAL
else   // cas standard du QFSEVITALE
   { //..................................... ne pas rajouter "/" car c'est un nom de fichier ..........................
     if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanDirPath (pathPlugin.prepend(G_pCApp->pathAppli()));
     //..................................... rajouter "/" car le chemin doit ï¿½tre donn\303\251 avec ..........................
     if ( QDir::isRelativePath ( pathLog ) )     pathLog    = QDir::cleanDirPath (pathLog.prepend(G_pCApp->pathAppli()) )+"/";

     QString pluginScript  = "[Execute]\r\n";
             pluginScript += "pathPlugin="      + pathPlugin                                           + "\r\n" +
                        "pathIni=****\r\n" +                   //**** car indique d'utiliser le pathIni local du plugin
                        "maskExch=Bonjour tout le monde\r\n"
                        "[Parametres]\r\n"
                        "m_Command       = READ_VITALE\r\n"
                        "m_PathLog       =" + pathLog                                             + "\r\n" +
                        "m_DateConsult   =" + QDate::currentDate().toString("yyyyMMdd");

     if (pathPlugin.length())
         {G_pCApp->m_pVitale->UnSerialize(G_pCApp->PluginExe(this, pluginScript, CMoteurBase::endWait));
          last_occurence = G_pCApp->m_pVitale->GetNbOccurences(104) + 1;  //Cz_Pyxvital ?????? Marche mal si pyxvital ???
         }
       } // fin else  - cas QFSEVITALE

       //.................................... La suite du traitement est commune ...........................

         //................. determine si le nom prenom entrent dans la comparaison ..........................
         QString localParam = "";   CGestIni::Param_UpdateFromDisk(G_pCApp->pathIni(), localParam);
         bool matchOnNom    = (CGestIni::Param_ReadUniqueParam(localParam, "Sesam-Vitale", "Compare_Nom")    == "ok");
         bool matchOnPrenom = (CGestIni::Param_ReadUniqueParam(localParam, "Sesam-Vitale", "Compare_Prenom") == "ok");

         QString    nss = G_pCApp->m_pVitale->GetMember(8,  101, 1);
         // last_occurence = G_pCApp->m_pVitale->GetNbOccurences(104) + 1;  //Cz_Pyxvital
         int i;
         // pQlistView->clear();   //Cz_Pyxvital  -- remont\303\251 avant l'appel au plugin
         for (i=1; i<last_occurence; ++i)
             {QTreeWidgetItem *pQListViewItem = new QTreeWidgetItem();
              if (pQListViewItem)
                 {pQListViewItem->setText(0,  G_pCApp->m_pVitale->GetMember(1,  104, i));    // 0  nom
                  pQListViewItem->setText(1,  G_pCApp->m_pVitale->GetMember(3,  104, i));    // 1  prenom
                  pQListViewItem->setText(2,  G_pCApp->m_pVitale->GetMember(12, 104, i));    // 2  date Nss
                  pQListViewItem->setText(3,  nss);                                          // 3  num secu
                  pQListViewItem->setText(4,  G_pCApp->m_pVitale->GetMember(13,  104, i));   // 4  rang geme
                  pQListViewItem->setText(5,  G_pCApp->m_pVitale->GetMember(14,  104, i));   // 5  qualite ayant-droit
                  pQListViewItem->setText(6,  "");                                           // 6  pk doss
                  pQListViewItem->setText(7,  "");                                           // 7  GUID
                  pQListViewItem->setText(8,  QString::number(i));                           // 8  occurence
                  pQListViewItem->setText(9,  G_pCApp->m_pVitale->GetMember(1,   105, i));   // 9  date ouverture droits
                  pQListViewItem->setText(10, G_pCApp->m_pVitale->GetMember(2,   105, i));   // 10 date fin droits
                  pQlistView->addTopLevelItem(pQListViewItem);
                  //............ formater qq trucs pour les rendre compatible avec le match SQL ....................
                  QString dnss     = pQListViewItem->text(2).left(8);             //  199306010000   -->  19930601
                          dnss     = dnss.insert(4,'-');                          //  19930601       -->  1993-0601
                          dnss     = dnss.insert(7,'-');                          //  1993-0601      -->  1993-06-01
                          dnss    += " 00:00:00";                                 //  1993-0601      -->  1993-06-01 00:00:00
                  QString numSecu  = pQListViewItem->text(3);
                          numSecu += CMoteurBase::ComputeClef_Secu(numSecu);
                  QString rangGem  = pQListViewItem->text(4);
                  if (rangGem.length()==1) rangGem = rangGem.prepend("0");
                  //............ determine si le nom prenom entrent dans la comparaison .....................
                  QString        nom = pQListViewItem->text(0);
                  QString     prenom = pQListViewItem->text(1);
                  if (QDate::isValid ( dnss.mid(0,4).toInt(), dnss.mid(5,2).toInt(), dnss.mid(8,2).toInt() ) ) // si date de naiss invalide on est oblig? de se servir du nom prenom
                     { if (!matchOnNom)     nom    = "";
                       if (!matchOnPrenom)  prenom = "";
                     }
                  if (G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme( 0,                      // pas de listview car on veut juste savoir si trouve
                                                                                    nom,                         // nom
                                                                                    prenom,                      // prenom
                                                                                    dnss,                        // date Nss // doit etre 1993-06-01 00:00:00
                                                                                    numSecu,                     // num secu // 160196296519557
                                                                                    rangGem,                     // rang gemellaire
                                                                                    pQListViewItem->text(5),     // qualite d'ayant droit
                                                                                    "",      //  adresse,
                                                                                    "",      //  codePost,
                                                                                    "",      //  ville,
                                                                                    "",      //  sexe,
                                                                                    "",      //  profession,
                                                                                    "",      //  nomJfille
                                                                                    "",      //  nomAss,
                                                                                    "",      //  prenomAss,
                                                                                    "",      //  tel1,
                                                                                    "",      //  tel2,
                                                                                    "",      //  tel3,
                                                                                    "",      //  email,
                                                                                    " = ",   //  egal_like  " = ",
                                                                                    m_pGUI->textLabel_StateList
                                                                                  )
                     )
                     {pQListViewItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/VitaleOk.png")) );
                      pQListViewItem->setText(6, "-");                                           // 6 pk doss
                     }
                  else
                     {pQListViewItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/VitaleNotOk.png")) );
                      pQListViewItem->setText(6, "");
                     }
                 }  // else if (pQListViewItem)
             } //for (i=1; i<last_occurence; ++i)
        } //if (pathPlugin.length())
     if (last_occurence<2) pQlistView->hide();
     else
        {//.......Redimensionner la longueur de la liste des b\303\251n\303\251ficiiaires en fct du nbre d'Item.........
         int nb = pQlistView->topLevelItemCount ();
         if (nb)
            {QTreeWidgetItem *pQTreeWidgetItem = pQlistView->topLevelItem(0);
             QRect                        rect = pQlistView->visualItemRect (pQTreeWidgetItem );
             int                             h = rect.height()* (nb + 3);
             QList<int> list = m_pGUI->splitter_listPatient->sizes();
             int delta = h - list[0];  list[0] += delta; list[1] -= delta;
             m_pGUI->splitter_listPatient->setSizes ( list );
             //........... si un seul ayant droit le selectionner............
             if (nb==1)
                {pQlistView->setCurrentItem ( pQTreeWidgetItem );
                 pQTreeWidgetItem->setSelected ( TRUE );
                }
            }
        }
   // } // if (moduleName=="PIXVITALE")  >>>Cz_Pyxvital -- d\303\251coupage modifi\303\251 pour ptofiter du code commun

#endif
}

//--------------------------------- setInterfaceOnVitaleData ----------------------------------------------------------------------------------------------
void C_Manager::setInterfaceOnVitaleData(void *pcvitale_in, int occurence)
{
/*
#ifdef SESAM_VERSION

       if (pcvitale_in==0) return;
       C_Vitale  *pcVitale = (C_Vitale*) pcvitale_in;

       //.............. nom prenom patient ...........................
       m_pGUI->lineEdit_NomDossier->setText(      pcVitale->GetMember(1,  104, occurence));
       m_pGUI->lineEdit_PrenomDossier->setText(   pcVitale->GetMember(3,  104, occurence));

       //.............. adresse ...........................
       QString adresse = pcVitale->GetAdresseAyanDroit(occurence);
       if (m_pGUI->textEdit_Adresse->text().length()==0 ||       // si adresse determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          ) m_pGUI->textEdit_Adresse->setText(         adresse       );

       //lineEdit_Tel1->setText(            ToNum(query.value( 3).toString())    );
       //lineEdit_Tel2->setText(            ToNum(query.value( 4).toString())    );
       //lineEdit_Tel3->setText(            ToNum(query.value( 5).toString())    );
       //lineEdit_Email->setText(           CGestIni::Utf8_Query(query , 6)      );
       //.............. code postal ville ...........................
       QString codePostLocalite = pcVitale->GetMember(8,  104, occurence);
       if (m_pGUI->lineEdit_CdPostal->text().remove(' ').toInt()==0 ||       // si codePostal determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          ) m_pGUI->lineEdit_CdPostal->setText(        codePostLocalite.left(5) );
       if (m_pGUI->lineEditVille->text().length()==0 ||                      // si ville determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          ) m_pGUI->lineEditVille->setText(    codePostLocalite.mid(6)  );

       //QString sexe                  =    CGestIni::Utf8_Query(query , 9)       ;
       //if (sexe.left(1).lower() == "m")   comboBoxSexe->setCurrentItem ( 0 ) ;
       //else                               comboBoxSexe->setCurrentItem ( 1 ) ;
       //.............. rang gemellaire ...........................
       QString rangGem  = pcVitale->GetMember(13,  104, occurence);
       if (rangGem.length()==0) rangGem = rangGem.prepend("0");
       if (rangGem.length()==1) rangGem = rangGem.prepend("0");
       m_pGUI->lineEdit_RangGeme->setText( rangGem );

       //lineEdit_Sexe->setText(            CGestIni::Utf8_Query(query , 9)      );
       //textLabel_NumDoss->setText(        CGestIni::Utf8_Query(query , 10)     );
       if (m_pGUI->lineEdit_NomDeNss->text().length()==0 ||           // si ville determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          )m_pGUI->lineEdit_NomDeNss->setText(        pcVitale->GetMember(2,  104, occurence)     );
       //lineEdit_Titre->setText(           CGestIni::Utf8_Query(query , 12)     );
       //lineEdit_Profession->setText(      CGestIni::Utf8_Query(query , 13)     );
       //.......................... ayant droit ........................................
       int qualiteAyantDroit = pcVitale->GetMember(14,  104, occurence).toInt();
       m_pGUI->comboBoxQualiteAyantDroit->setCurrentItem ( qualiteAyantDroit );
       tryToSetInterfaceFromAyantDroit( qualiteAyantDroit, 0 );
       //............. rechercher l'assure .............................................
       int last_occurence = pcVitale->GetNbOccurences(104) + 1;
       int i;
       for (i=1; i<last_occurence; ++i)
           {if (pcVitale->GetMember(14,  104, i).toInt()==0)    //  qualite ayant-droit si zero c'est l'assure
               {m_pGUI->lineEdit_NomAssure->setText(       pcVitale->GetMember(1,  104, i)      );   // recuperer son nom
                m_pGUI->lineEdit_PrenomAssure->setText(    pcVitale->GetMember(3,  104, i)      );   // recuperer son prenom
                //............ si adresse ayant droit est non d\303\251finie ..............................
                //             proposer celle de l'assure
                if (adresse.trimmed().length()==0)
                   {adresse =pcVitale->GetAdresseAyanDroit(i) ;
                    if (m_pGUI->textEdit_Adresse->text().length()==0 ||       // si adresse determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
                        m_pGUI->pushButtonCreate->isShown()
                       )m_pGUI->textEdit_Adresse->setText(adresse);
                    codePostLocalite = pcVitale->GetMember(8,  104, i);
                    if (m_pGUI->lineEdit_CdPostal->text().remove(' ').toInt()==0 ||       // si codePostal determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
                        m_pGUI->pushButtonCreate->isShown()
                       )m_pGUI->lineEdit_CdPostal->setText(        codePostLocalite.left(5) );
                    if (m_pGUI->lineEditVille->text().length()==0 ||           // si ville determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
                        m_pGUI->pushButtonCreate->isShown()
                       )m_pGUI->lineEditVille->setText(            codePostLocalite.mid(6)  );
                   }
                break;
               }
           }

       //....................... trucs speciaux ........................................
       QString qDteNss =    pcVitale->GetMember(12, 104, occurence);                   // 199306010000
               qDteNss =    qDteNss.mid(6,2)+qDteNss.mid(4,2)+qDteNss.mid(0,4);        // 01061993
       m_pGUI->lineEdit_DtNss->setText(qDteNss);
       int jour, mois, annee;
       jour  = qDteNss.mid(0,2).toInt();
       mois  = qDteNss.mid(2,2).toInt();
       annee = qDteNss.mid(4,4).toInt();
       if (QDate::isValid ( annee, mois, jour ) )
          {QDate dt(annee, mois, jour);
           / *textLabel_Age->setText(m_pCMoteurBase->CalculeDiffAge(&dt, 0, TRUE, 0)); * / m_pGUI->groupBoxPatient->setTitle(tr("Patient \303\242g\303\251 de : ")+m_pCMoteurBase->CalculeDiffAge(&dt, 0, TRUE, 0));
          }
       else
          { / *textLabel_Age->setText(tr("Date de naissance non valide")); * / m_pGUI->groupBoxPatient->setTitle(tr("Patient avec une date de naissance non valide"));
           m_pGUI->lineEdit_Titre->setText("");
          }
       //....................... numero secu ........................................
       QString   nss =  pcVitale->GetMember(8,  101, 1);
       m_pGUI->lineEdit_NumSecu->setText(nss);
       m_pGUI->lineEdit_NssClef->setText(CMoteurBase::ComputeClef_Secu(nss));
       //................ sexe ......................................................
       if (m_pGUI->comboBoxSexe->currentItem()==2 ||       // si sexe determine et pas mode creation alors ne pas toucher au sexe
           m_pGUI->pushButtonCreate->isShown()
          )setSexFromQualiteAyantDroit(qualiteAyantDroit);
#endif
*/
#ifdef SESAM_VERSION

       if (pcvitale_in==0) return;
       C_Vitale  *pcVitale = (C_Vitale*) pcvitale_in;

       //.............. nom prenom patient ...........................
       m_pGUI->lineEdit_NomDossier->setText(      pcVitale->GetMember(1,  104, occurence));
       m_pGUI->lineEdit_PrenomDossier->setText(   pcVitale->GetMember(3,  104, occurence));

       //.............. adresse ...........................
       QString adresse = pcVitale->GetAdresseAyanDroit(occurence);
       if (m_pGUI->textEdit_Adresse->text().length()==0 ||       // si adresse determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          ) m_pGUI->textEdit_Adresse->setText(         adresse       );

       //lineEdit_Tel1->setText(            ToNum(query.value( 3).toString())    );
       //lineEdit_Tel2->setText(            ToNum(query.value( 4).toString())    );
       //lineEdit_Tel3->setText(            ToNum(query.value( 5).toString())    );
       //lineEdit_Email->setText(           CGestIni::Utf8_Query(query , 6)      );
       //.............. code postal ville ...........................
       QString codePostLocalite = pcVitale->GetMember(8,  104, occurence);
       if (m_pGUI->lineEdit_CdPostal->text().remove(' ').toInt()==0 ||       // si codePostal determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          ) m_pGUI->lineEdit_CdPostal->setText(        codePostLocalite.left(5) );
       if (m_pGUI->lineEditVille->text().length()==0 ||                      // si ville determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          ) m_pGUI->lineEditVille->setText(    codePostLocalite.mid(6)  );

       //QString sexe                  =    CGestIni::Utf8_Query(query , 9)       ;
       //if (sexe.left(1).lower() == "m")   comboBoxSexe->setCurrentItem ( 0 ) ;
       //else                               comboBoxSexe->setCurrentItem ( 1 ) ;
       //.............. rang gemellaire ...........................
       QString rangGem  = pcVitale->GetMember(13,  104, occurence);
       if (rangGem.length()==0) rangGem = rangGem.prepend("0");
       if (rangGem.length()==1) rangGem = rangGem.prepend("0");
       m_pGUI->lineEdit_RangGeme->setText( rangGem );

       //lineEdit_Sexe->setText(            CGestIni::Utf8_Query(query , 9)      );
       //textLabel_NumDoss->setText(        CGestIni::Utf8_Query(query , 10)     );
       if (m_pGUI->lineEdit_NomDeNss->text().length()==0 ||           // si ville determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
           m_pGUI->pushButtonCreate->isShown()
          )m_pGUI->lineEdit_NomDeNss->setText(        pcVitale->GetMember(2,  104, occurence)     );
       //lineEdit_Titre->setText(           CGestIni::Utf8_Query(query , 12)     );
       //lineEdit_Profession->setText(      CGestIni::Utf8_Query(query , 13)     );
       //.......................... ayant droit ........................................
       int qualiteAyantDroit = pcVitale->GetMember(14,  104, occurence).toInt();
       m_pGUI->comboBoxQualiteAyantDroit->setCurrentItem ( qualiteAyantDroit );
       tryToSetInterfaceFromAyantDroit( qualiteAyantDroit, 0 );
       //............. rechercher l'assure .............................................
       QString qDteNss;
       int last_occurence = pcVitale->GetNbOccurences(104) + 1;
       int i;
       bool assureFind = FALSE;
       for (i=1; i<last_occurence; ++i)
           {
            if (pcVitale->GetMember(14,  104, i).toInt()==0)    //  qualite ayant-droit si zero c'est l'assure
               {
                assureFind = TRUE;
                m_pGUI->lineEdit_NomAssure->setText(       pcVitale->GetMember(1,  104, i)      );   // recuperer son nom
                m_pGUI->lineEdit_PrenomAssure->setText(    pcVitale->GetMember(3,  104, i)      );   // recuperer son prenom
                qDteNss =    pcVitale->GetMember(12, 104, i);                                // recuperer sa date de naissance
                qDteNss =    qDteNss.mid(6,2)+qDteNss.mid(4,2)+qDteNss.mid(0,4);             // 01061993
                m_pGUI->lineEdit_DtNss_Assure->setText(qDteNss);   //lineEdit_DtNss_Assure

                //............ si adresse ayant droit est non d\303\251finie ..............................
                //             proposer celle de l'assure
                if (adresse.trimmed().length()==0)
                   {adresse =pcVitale->GetAdresseAyanDroit(i) ;
                    if (m_pGUI->textEdit_Adresse->text().length()==0 ||       // si adresse determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
                        m_pGUI->pushButtonCreate->isShown()
                       )m_pGUI->textEdit_Adresse->setText(adresse);
                    codePostLocalite = pcVitale->GetMember(8,  104, i);
                    if (m_pGUI->lineEdit_CdPostal->text().remove(' ').toInt()==0 ||       // si codePostal determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
                        m_pGUI->pushButtonCreate->isShown()
                       )m_pGUI->lineEdit_CdPostal->setText(        codePostLocalite.left(5) );
                    if (m_pGUI->lineEditVille->text().length()==0 ||           // si ville determine et pas mode creation alors ne pas y toucher car la CV peut etre non renseignee
                        m_pGUI->pushButtonCreate->isShown()
                       )m_pGUI->lineEditVille->setText(            codePostLocalite.mid(6)  );
                   }
                break;
               }
           }
       if (assureFind==FALSE)
          {if (getInterfaceMode() == MODE_CREATION_PATIENT)
              {m_pGUI->lineEdit_DtNss_Assure->setText("");
               m_pGUI->lineEdit_PrenomAssure->setText("");
              }
           G_pCApp->CouCou(tr( "Carte vitale sans les donn\303\251es de l'assur\303\251, v\303\251rifiez les manuellement."));
           C_Dlg_Assure *dlg = new C_Dlg_Assure(this, m_pGUI->lineEdit_NomAssure->text(), m_pGUI->lineEdit_PrenomAssure->text(), m_pGUI->lineEdit_DtNss_Assure->text());
           if (dlg)
              {if (dlg->exec()==QDialog::Accepted)
                  {m_pGUI->lineEdit_NomAssure->setText(dlg->getNom());
                   m_pGUI->lineEdit_PrenomAssure->setText(dlg->getPrenom());
                   m_pGUI->lineEdit_DtNss_Assure->setText(dlg->getDateNss());
                  }
               delete dlg;
              }
          }
       //....................... trucs speciaux ........................................
       qDteNss =    pcVitale->GetMember(12, 104, occurence);                   // 199306010000
       qDteNss =    qDteNss.mid(6,2)+qDteNss.mid(4,2)+qDteNss.mid(0,4);        // 01061993
       m_pGUI->lineEdit_DtNss->setText(qDteNss);   //lineEdit_DtNss_Assure
       int jour, mois, annee;
       jour  = qDteNss.mid(0,2).toInt();
       mois  = qDteNss.mid(2,2).toInt();
       annee = qDteNss.mid(4,4).toInt();
       if (QDate::isValid ( annee, mois, jour ) )
          {QDate dt(annee, mois, jour);
           /*textLabel_Age->setText(m_pCMoteurBase->CalculeDiffAge(&dt, 0, TRUE, 0)); */ m_pGUI->groupBoxPatient->setTitle(tr("Patient (b\303\251n\303\251ficiaire) \303\242g\303\251 de : ")+m_pCMoteurBase->CalculeDiffAge(&dt, 0, TRUE, 0));
          }
       else
          { /*textLabel_Age->setText(tr("Date de naissance non valide")); */ m_pGUI->groupBoxPatient->setTitle(tr("Patient (b\303\251n\303\251ficiaire) avec une date de naissance non valide"));
           m_pGUI->lineEdit_Titre->setText("");
          }
       //....................... numero secu ........................................
       //XXXXXXXXXXXXXXX  A VERIFIER
       // SIGEMS les num de ss contiennent la cle

       QString   nss =  pcVitale->GetMember(8,  101, 1);
       m_pGUI->lineEdit_NumSecu->setText(nss);
       m_pGUI->lineEdit_NssClef->setText(CMoteurBase::ComputeClef_Secu(nss.left(13))); // SIGEMS
       //................ sexe ......................................................
       int i_sexe = 2;
       if (m_pGUI->comboBoxSexe->currentItem()==2 ||       // si sexe determine et pas mode creation alors ne pas toucher au sexe
           m_pGUI->pushButtonCreate->isShown()
          ) i_sexe = setSexFromQualiteAyantDroit(qualiteAyantDroit);

       // Cz_Pyxvital -------------------------- DEBUT ---------------------------------------
       // Si on est en mode facture seule, sans gestion du dossier patient
       // on lance directement la cr\303\251ation du patient puis la saisie FSE.
       if (m_Facture_Seule  == "oui")
           {Slot_pushButtonCreate();             // Cr\303\251ation du patient dans la base.
            Slot_pushButton_FSE();               // lancement saisie FSE.
           }
       setInterfaceOnDroitsSociaux(pcVitale, occurence);
      // if (G_pCApp->readUniqueParam ( "Connexion", "DebugMode").trimmed()=="1")
      //    {CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli()+"vitale-setInterfaceOnVitaleData.txt",  pcVitale->m_VitaleData.toAscii());
      //    }
       QString telSigems = pcVitale->GetMember(111, 111, occurence).trimmed();
       if (telSigems.length())
          {m_pGUI->lineEdit_Tel1->setText(telSigems);
          }
       QString sexeSigems = pcVitale->GetMember(111, 112, occurence).trimmed();
       if (i_sexe==2 && sexeSigems.length())
          {if (sexeSigems=="M")  m_pGUI->comboBoxSexe->setCurrentItem ( 0 ) ;
           else                  m_pGUI->comboBoxSexe->setCurrentItem ( 1 ) ;
           tryToSetTitreFromDateNss();  // regler le TITRE
          }
       //Cz_Pyxvital -------------------------- FIN ----------------------------------------
#endif
}

//------------------------------------------------------- setInterfaceOnDroitsSociaux --------------------------------------------------
//    met \303\240 jour les elements de l'interface relatifs au droits sociaux sur ceux de la carte vitale
void  C_Manager::setInterfaceOnDroitsSociaux(C_Vitale       *pcVitale, int occurence, int anyWay /* = 0 */)
{
#ifdef SESAM_VERSION
if (anyWay==0 && pcVitale && m_pGUI->lineEdit_NumSecu->text().remove(' ') != pcVitale->GetMember(8,  101, 1))
   {clearDroitsFields();
    return;
   }
QString debDroits = pcVitale->GetMember(1,   105, occurence);
QString finDroits = pcVitale->GetMember(2,   105, occurence);
m_pGUI->lineEdit_DroitsDateDeb->setText(debDroits.mid(6,2)+"/"+debDroits.mid(4,2)+"/"+debDroits.mid(0,4));
m_pGUI->lineEdit_DroitsDateFin->setText(finDroits.mid(6,2)+"/"+finDroits.mid(4,2)+"/"+finDroits.mid(0,4));
m_pGUI->lineEdit_CodeRegime->setText(pcVitale->GetMember(10,  101, 1));
m_pGUI->lineEdit_CaisseGest->setText(pcVitale->GetMember(11,  101, 1));
m_pGUI->lineEdit_CentreGest->setText(pcVitale->GetMember(12,  101, 1));
m_pGUI->lineEdit_CodeGest->setText(pcVitale->GetMember(13,  101, 1));
#endif
}
///////////////////////////////////////// MEDECIN TRAITANT ////////////////////////////////////////
//--------------------------------- Slot_listView_Praticiens_contextMenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_listView_Praticiens_contextMenuRequested (const QPoint &)
{
QTreeWidgetItem *pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Praticiens);
if (pQListViewItem==0)            return;
//............... menu des qualites ..........................
QStringList lst;
QString removeProp = tr("Remove property 'As'");
G_pCApp->getParamList( "Qualite intervenant", "",  lst );
if (lst.count()==0)               return;
lst.append ( "-------------" );
lst.append ( removeProp );
QString ret = G_pCApp->DoPopupList(lst);
if (ret.length()==0)              return;
if (ret == removeProp) ret="";
//............. placer le retour du menu ...................
pQListViewItem->setText(3, ret);
if (getInterfaceMode()==MODE_CREATION_PATIENT )  return;     // on se contente de renseigner uniquement la listview (l'enregistrement se fera apres click sur enregistrer)
//............ enregistrer l'info ..........................
QString numGUID   = G_pCApp->identitePatientGUID();
QString iD_Doss   = G_pCApp->identitePatientPk();
m_pCMoteurBase->PatientIntervenantsWrite( iD_Doss , numGUID, pQListViewItem->text(4),  pQListViewItem->text(3));
}

//--------------------------------- Slot_pushButton_AddMedTTT_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_AddMedTTT_Fast_clicked ()
{  QString numGUID   = "";
   QString iD_Doss   = "";
   QTreeWidgetItem *pQListViewItem =  m_pGUI->listView_Patient->currentItem();
   if (getInterfaceMode()==MODE_CREATION_PATIENT)
      {   numGUID    = m_pCMoteurBase->GUID_Create();
          iD_Doss    = "";
      }
   else if ( pQListViewItem)
      {   numGUID    = G_pCApp->identitePatientGUID();
          iD_Doss    = G_pCApp->identitePatientPk();
      }
   else return;

QString defEnQualiteDe = "";
G_pCApp->readParam(  "Gestion des contacts", "qualite par defaut", &defEnQualiteDe );         // path editeur de texte non d\303\251fini
QString defSpecialite        = G_pCApp->readUniqueParam("Gestion des contacts", "specialite par defaut").trimmed();         // path editeur de texte non d\303\251fini
QStringList listIntervenants = m_pCMoteurBase->PatientIntervenantsGetData(defSpecialite,"|");
if (listIntervenants.count()==0) return;
m_pGUI->pushButton_AddMedTTT_Fast->setIcon ( QIcon (Theme::getIcon( "16x16/Down.png" ))) ;

C_Dlg_DoList* dlg = new C_Dlg_DoList(listIntervenants, "|", 2, C_Dlg_DoList::SingleSelection, this);
QString ret       = "";
if (dlg)
   {if (dlg->exec()==QDialog::Accepted) ret = dlg->getStringSingleSelectedResult();
    delete dlg;
   }
m_pGUI->pushButton_AddMedTTT_Fast->setIcon ( QIcon (Theme::getIcon( "16x16/Left.png" ))) ;

if (ret.length()==0)             return;
QStringList listArg =ret.split("|",QString::KeepEmptyParts);
if (listArg.count()<4)           return;
QString nom      = listArg[0];
QString prenom   = listArg[1];
QString spec     = listArg[2];
QString id       = listArg[3];
QTreeWidgetItem *pQTreeWidgetItemInterv = 0;
if (getInterfaceMode()==MODE_CREATION_PATIENT)
   { pQTreeWidgetItemInterv = new QTreeWidgetItem();
   }
else if (m_pCMoteurBase->PatientIntervenantsWrite( iD_Doss , numGUID, id, defEnQualiteDe) != 0)
   { pQTreeWidgetItemInterv = new QTreeWidgetItem();
   }
if (pQTreeWidgetItemInterv)
   {pQTreeWidgetItemInterv->setText(0,spec);
    pQTreeWidgetItemInterv->setText(1,nom);
    pQTreeWidgetItemInterv->setText(2,prenom);
    pQTreeWidgetItemInterv->setText(3,defEnQualiteDe);
    pQTreeWidgetItemInterv->setText(4,id);
    m_pGUI->listView_Praticiens->addTopLevelItem(pQTreeWidgetItemInterv);
   }
}

//--------------------------------- Slot_pushButton_DelMedTTT_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_DelMedTTT_clicked()
{  if (G_pCApp->IsThisDroitExist( "idc")==0)
     {G_pCApp->CouCou(tr( "Your rights are insufficient to modify a file"));
      return;
     }
   QTreeWidgetItem *pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Praticiens);
   if (pQListViewItem==0) return;

   //...... si MODE CREATION ..................
   // les intervenants ne sont justes que cr\303\251\303\251s dans la listview
   // et n'existent pas encore en base donc juste effacer l'item
   //...... si pas MODE CREATION ..................
   // les intervenants sont en base donc effacer l'item
   // et la donn\303\251e correspondante en base
   if (getInterfaceMode() != MODE_CREATION_PATIENT)
      {  m_pCMoteurBase->PatientIntervenantsDelete( pQListViewItem->text(ID_INTERV), G_pCApp->identitePatientGUID());
      }
   delete pQListViewItem;
}

//--------------------------------- Slot_listView_Praticiens_DoubleClicked -------------------------------------------------------------
void C_Manager::Slot_listView_Praticiens_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem, int )
{m_pGUI->listView_Praticiens->setEnabled(false);
 //disconnect( m_pGUI->listView_Praticiens, SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), 0, 0 );
 QString idInterv = "";
 if (pQTreeWidgetItem) idInterv = pQTreeWidgetItem->text(ID_INTERV);
 exeAnnuaire(idInterv);
 //connect( m_pGUI->listView_Praticiens, SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(Slot_listView_Praticiens_DoubleClicked( QTreeWidgetItem * , int)) );
 m_pGUI->listView_Praticiens->setEnabled(true);
}
//--------------------------------- Slot_pushButton_AddMedTTT_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_AddMedTTT_clicked()
{m_pGUI->pushButton_AddMedTTT->setEnabled(false);
 //disconnect(m_pGUI->pushButton_AddMedTTT, SIGNAL(clicked()), 0, 0);
 QString idInterv = "";
 //............ recuperer \303\251ventuel ID praticien en cours .......................................
 QTreeWidgetItem *pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Praticiens);
 if (pQListViewItem) idInterv = pQListViewItem->text(ID_INTERV);
 exeAnnuaire( idInterv );
 //connect( m_pGUI->pushButton_AddMedTTT,     SIGNAL(clicked()), this, SLOT(Slot_pushButton_AddMedTTT_clicked()) );
 m_pGUI->pushButton_AddMedTTT->setEnabled(true);
}

//--------------------------------- exeAnnuaire -------------------------------------------------------------
void C_Manager::exeAnnuaire(QString idInterv )
{ if (m_Contacts_Run)
     {G_pCApp->CouCou(tr( "The process 'personnes' is still running"));
      return;
     }
  if (G_pCApp->IsThisDroitExist( "idc")==0)
     {G_pCApp->CouCou(tr( "Your rights are insufficient to modify a file"));
      return;
     }
   //.............................. rechercher executable et son fichier d'echange ..........................
   QString pathExe = CGestIni::Construct_Name_Exe("personnes",QFileInfo (qApp->argv()[0]).dirPath (true));
   if ( !QFile::exists( pathExe ) ) {G_pCApp->CouCou(tr( "Binary is not found.").arg(pathExe));  return; }
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   QString pathExch;
   // si path executable non defini le fixer par defaut sur '../../tmp'
   if (G_pCApp->readParam(  "Repertoire Temporaire", "Repertoire", &pathExch)!=QString::null)
      {pathExch = "../../tmp";
      }
   // Adaptation a la plateforme
   if ( QDir(pathExch).isRelative()) pathExch.prepend(G_pCApp->pathAppli());
   pathExch += "/ManagerGetPersonnne"+  m_pCMoteurBase->GUID_Create() +".ech";   // on cree un nom de fichier unique
   pathExch  = QDir::cleanDirPath(pathExch);
   // l'effacer au ca ou ne l'aurait pas deja ete (euh avec un GUID c'est un peut idiot
   //QFile::remove (pathExch);

   QStringList argList;
   //......................... completer les autres arguments .........................................
   argList <<( pathExch );
   argList <<( "Record" );
   argList <<( idInterv );
   argList <<( G_pCApp->m_Droits );

   if (m_Contacts_Run==FALSE)
      { m_Contacts_Run = TRUE;
        QProcess   proc;
        connect( &proc,  SIGNAL(error ( QProcess::ProcessError  )), G_pCApp, SLOT(Slot_error ( QProcess::ProcessError  )) );
        proc.start(pathExe, argList);
        if (!proc.waitForStarted(4000))  {QApplication::restoreOverrideCursor(); m_Contacts_Run = FALSE;  return;};
        // if (!proc.waitForFinished())     {QApplication::restoreOverrideCursor(); m_Contacts_Run = FALSE;  return;}; // crash
        // on fait une boucle d'attente car crash du process appele avec waitForFinished() !!!
        G_pCApp->processEvents ();
        while (proc.state()==QProcess::Running )
             { QApplication::processEvents ( QEventLoop::ExcludeUserInput );
             }

        //......... recuperer fichier d'echange .............
        //          genere par l'executable
        QString ret;
        CGestIni::Param_UpdateFromDisk(pathExch, ret);
        //............... analyse du retour ................
        QString id, spec,nom, prenom;
        int deb =  0;
        int end = ret.find('|');
        id      = ret.mid(deb, end-deb);
        deb     = end + 1;
        end     = ret.find('|', deb);
        nom     = ret.mid(deb, end-deb);
        deb     = end + 1;
        end     = ret.find('|', deb);
        prenom  = ret.mid(deb, end-deb);
        deb     = end + 1;
        spec    = ret.mid(deb);
        //................... si le dossier est en cr\303\251ation ajout uniquement dans la listview ...........................
        QTreeWidgetItem* pQTreeWidgetItemInterv = 0;
        if (getInterfaceMode() == MODE_CREATION_PATIENT)
           { pQTreeWidgetItemInterv = new QTreeWidgetItem();
           }
        //................... si le dossier est deja cree on enregistre  ...........................
        //                    l'intervenant avec les identifiants patients connus
        else if (m_pCMoteurBase->PatientIntervenantsWrite( G_pCApp->identitePatientPk() , G_pCApp->identitePatientGUID(), id, "") != 0)
           { pQTreeWidgetItemInterv = new QTreeWidgetItem();
           }
       if (pQTreeWidgetItemInterv)
          { pQTreeWidgetItemInterv->setText(0,spec);
            pQTreeWidgetItemInterv->setText(1,nom);
            pQTreeWidgetItemInterv->setText(2,prenom);
            pQTreeWidgetItemInterv->setText(3,"");      // en qualite de
            pQTreeWidgetItemInterv->setText(4,id);
            m_pGUI->listView_Praticiens->addTopLevelItem(pQTreeWidgetItemInterv);
          }
        QFile::remove (pathExch);
        m_Contacts_Run = FALSE;
      }
        QApplication::restoreOverrideCursor();
}


///////////////////////////////////////// PROFILS D'INTERFACE ////////////////////////////////////////
//--------------------------------- setInterfaceOnCurrentProfil -----------------------------------------------------------------------
// #define MODE_SELECTION_PATIENT 1
// #define MODE_CREATION_PATIENT  2
// #define MODE_MULTICRITERE      3
void C_Manager::setInterfaceOnCurrentProfil()
{setInterfaceOnProfil(getInterfaceMode());
}

//--------------------------------- setTitle -----------------------------------------------------------------------
void C_Manager::setTitle()
{QString title = "";
QString nom, prenom;
if (G_pCApp->m_SignUser.length())
   {m_pCMoteurBase->GetUserNomPrenom( G_pCApp->m_SignUser, nom, prenom);
    title   = tr("Manager-V %1 used by: ").arg(G_pCApp->getNumVers());
    title  += nom + " " + prenom + tr("  connected to server: ")+ m_pCMoteurBase->m_DriverName + " "+ m_pCMoteurBase->m_UserName+"@"+m_pCMoteurBase->m_HostName+":"+m_pCMoteurBase->m_Port;
   }
else
   {title  += tr("Manager-V %1 connected to server: ").arg(G_pCApp->getNumVers())+ m_pCMoteurBase->m_DriverName + " "+ m_pCMoteurBase->m_UserName+"@"+m_pCMoteurBase->m_HostName+":"+m_pCMoteurBase->m_Port;
   }
setCaption(title);
}
//--------------------------------- Slot_actionMenuNew_clicked -----------------------------------------------------------------------
void C_Manager::Slot_actionMenuNew_clicked( bool )
{Slot_pushButtonNew_clicked();
}

//--------------------------------- Slot_pushBut_ExitMultiCritere -----------------------------------------------------------------------
void C_Manager::Slot_pushBut_ExitMultiCritere()
{setInterfaceOnProfil(MODE_SELECTION_PATIENT);
}
//--------------------------------- Slot_pushButton_Effacer_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_Effacer_clicked()
{clearIdentFields();
}
//--------------------------------- Slot_pushButton_MultiCritere -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_MultiCritere()
{ if (identIsModified() && G_pCApp->isCurrentIdentiteValid() && G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idc") )
    { Slot_pushButtonSavModif();
    }
setInterfaceOnProfil(MODE_MULTICRITERE);
}
//--------------------------------- Slot_pushBut_MultiCritFind -----------------------------------------------------------------------
void C_Manager::Slot_pushBut_MultiCritFind()
{ //............... faire la recherche multicritere .......................
QString dtNss = m_pGUI->lineEdit_DtNss->text().remove('-');
if (dtNss.length()==8) dtNss = dtNss.mid(4,4)+"-"+dtNss.mid(2,2)+"-"+dtNss.mid(0,2)+" 00:00:00";
else                   dtNss = "";
QString numSecu  = m_pGUI->lineEdit_NumSecu->text().remove(' ');
if (numSecu.length()==0)           numSecu  = "";
else  if (numSecu.length()==13)    numSecu += CMoteurBase::ComputeClef_Secu(numSecu);
else                               numSecu += "%";
QString sexe     = m_pGUI->comboBoxSexe->currentText().left(1).upper();
QString cdPost   = m_pGUI->lineEdit_CdPostal->text().remove(' ');
if (cdPost.length()==0 || cdPost.toInt()==0)   cdPost  = "";
else if (cdPost.length()<5)                    cdPost += "%";

         G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme( m_pGUI->listView_Patient,
                                                                            m_pGUI->lineEdit_NomDossier->text().trimmed(),                         // nom
                                                                            m_pGUI->lineEdit_PrenomDossier->text().trimmed(),                      // prenom
                                                                            dtNss,                                            // date Nss // doit etre 1993-06-01 00:00:00
                                                                            numSecu,                               // num secu // 160196296519557
                                                                            m_pGUI->lineEdit_RangGeme->text().trimmed(),                     // rang gemellaire
                                                                            "255",     // qualite d'ayant droit 255 pour ne pas requetter dessus
                                                                            m_pGUI->textEdit_Adresse->text().trimmed(),      //  adresse,
                                                                            cdPost,                                          //  codePost,
                                                                            m_pGUI->lineEditVille->text().trimmed(),         //  ville,
                                                                            sexe,              //  sexe,
                                                                            m_pGUI->lineEdit_Profession->text().trimmed(),      //  profession,
                                                                            m_pGUI->lineEdit_NomDeNss->text().trimmed(),      //  nomJfille
                                                                            m_pGUI->lineEdit_NomAssure->text().trimmed(),      //  nomAss,
                                                                            m_pGUI->lineEdit_PrenomAssure->text().trimmed(),      //  prenomAss,
                                                                            m_pGUI->lineEdit_Tel1->text().trimmed(),      //  tel1,
                                                                            m_pGUI->lineEdit_Tel2->text().trimmed(),      //  tel2,
                                                                            m_pGUI->lineEdit_Tel3->text().trimmed(),      //  tel3,
                                                                            m_pGUI->lineEdit_Email->text().trimmed(),      //  email,
                                                                            " LIKE ",   //  egal_like  " = ",
                                                                            m_pGUI->textLabel_StateList
                                                                          );
}

//--------------------------------- SetInterfaceOnProfil -----------------------------------------------------------------------
// #define MODE_SELECTION_PATIENT 1
// #define MODE_CREATION_PATIENT  2
// #define MODE_MULTICRITERE      3
void C_Manager::setInterfaceOnProfil(int mode)
{
if (G_pCApp->IsThisDroitExist("idv"))
   {
   }
 //................... bouton creer et acceder ou creer ...........................
if (G_pCApp->readUniqueParam ( QString("Derniere Session"), QString("Mode de creation dossier")).lower()[0] == 'a' &&
    G_pCApp->IsThisDroitExist("idg")
   )
   {m_pGUI->pushButtonCreate->setText(tr("&Create and access"));
    m_pGUI->pushButtonCreate->setObjectName("Acceder et creer");
   }
else
   {m_pGUI->pushButtonCreate->setText(tr("&Create file"));
    m_pGUI->pushButtonCreate->setObjectName("Creer seulement");
   }
 if (G_pCApp->IsThisDroitExist("adm")) m_action_DebloquerBases->setVisible( TRUE );
 else                                  m_action_DebloquerBases->setVisible( FALSE );
 if (G_pCApp->IsThisDroitExist("nog")) m_action_ModeNomade->setEnabled ( G_pCApp->m_IsGestionNomadisme );
 else                                  m_action_ModeNomade->setEnabled ( FALSE );
 //......... surcharge par dans .ini ...................
 if (G_pCApp->readUniqueParam ( "Manager session", "Remove Locks").lower()[0] == 'n') m_action_DebloquerBases->setVisible( false );
//........................... MODE_SELECTION_PATIENT ...........................


switch (mode)
{case MODE_SELECTION_PATIENT:
     {QString style  = getBoxStyle(mode);
      m_pGUI->groupBoxPatient->setStyleSheet(style);
      m_pGUI->groupBoxAssure->setStyleSheet(style);
      //m_pGUI->tabWidgetNotes->setStyleSheet(style);

      m_pGUI->pushButtonCancelCreate->hide();
      m_pGUI->pushButtonCreate->hide();
      m_pGUI->pushBut_ExitMultiCritere->hide();
      m_pGUI->pushButton_Effacer->hide();
      m_pGUI->pushBut_MultiCritFind->hide();

       //........ creer et modifier un dossier .................
      if (G_pCApp->IsThisDroitExist("idc"))
         {m_pGUI->pushButtonNew->show();
          m_action_NouveauDossier->setEnabled(TRUE);
          m_pGUI->pushButtonMenuNew->show();
          m_pGUI->pushButtonSavModif->show();
         }
      else
         {m_pGUI->pushButtonNew->hide();
          m_action_NouveauDossier->setEnabled(FALSE);
          m_pGUI->pushButtonMenuNew->hide();
          m_pGUI->pushButtonSavModif->hide();
         }
      //.......... lancer un dossier ...............
      if (G_pCApp->IsThisDroitExist("idg"))
         {m_pGUI->pushButtonAcceder->show();
         }
      else
         {m_pGUI->pushButtonAcceder->hide();
         }
      //.......... detruire un dossier ..............
      if (G_pCApp->IsThisDroitExist("idd"))
         {m_pGUI->pushButtonDetruire->hide();
         }
      else
         {m_pGUI->pushButtonDetruire->show();
          m_pGUI->pushButtonDetruire->setEnabled(TRUE);
         }
      m_pGUI->pushButtonVitale->show();
      m_pGUI->pushButton_MultiCritere->show();
      //.......... lancer une FSE ...............          //Cz_Pyxvital ????    Ajouter un droit d'accï¿½s FSE
      if (G_pCApp->IsThisDroitExist("fse"))
         {m_pGUI->pushButton_FSE->show();
         }
      else
         {m_pGUI->pushButton_FSE->hide();
         }
     }break;
 //........................... MODE_CREATION_PATIENT ...............................
 case MODE_CREATION_PATIENT:
     {if (G_pCApp->IsThisDroitExist("idc"))
         { QString style  = getBoxStyle(mode);
           m_pGUI->groupBoxPatient->setStyleSheet(style);
           m_pGUI->groupBoxAssure->setStyleSheet(style);

           m_pGUI->pushButtonCancelCreate->show();
           m_pGUI->pushButtonCreate->show();


           m_pGUI->pushButtonSavModif->hide();
           m_pGUI->pushButtonAcceder->hide();
           m_pGUI->pushBut_ExitMultiCritere->hide();
           m_pGUI->pushButton_Effacer->hide();
           m_pGUI->pushButtonDetruire->hide();
           m_pGUI->pushButtonNew->hide();
           m_action_NouveauDossier->setEnabled(FALSE);
           //m_pGUI->pushButtonMenuNew->hide();
           //m_pGUI->pushButtonVitale->hide();
           m_pGUI->pushButton_MultiCritere->hide();
           m_pGUI->pushBut_MultiCritFind->hide();
           m_pGUI->pushButton_FSE->hide();                 //Cz_Pyxvital


           m_pGUI->groupBoxPatient->setEnabled ( TRUE );
           m_pGUI->groupBoxAssure->setEnabled ( TRUE );
           m_pGUI->tabWidgetNotes->setEnabled ( TRUE );

         }
     }break;
 //........................... MODE_MULTICRITERE ...........................
 case MODE_MULTICRITERE:
     {QString style  = getBoxStyle(mode);
      m_pGUI->groupBoxPatient->setStyleSheet(style);
      m_pGUI->groupBoxAssure->setStyleSheet(style);

      m_pGUI->pushBut_ExitMultiCritere->show();
      m_pGUI->pushButton_Effacer->show();
      m_pGUI->pushBut_MultiCritFind->show();

      m_pGUI->pushButtonCancelCreate->hide();
      m_pGUI->pushButtonCreate->hide();
      m_pGUI->pushButtonSavModif->hide();
      m_pGUI->pushButtonAcceder->hide();
      m_pGUI->pushButtonDetruire->hide();
      m_pGUI->pushButtonNew->hide();
      m_action_NouveauDossier->setEnabled(FALSE);
      m_pGUI->pushButtonMenuNew->hide();
      m_pGUI->pushButtonVitale->hide();
      m_pGUI->pushButton_MultiCritere->hide();
      m_pGUI->pushButton_FSE->hide();                 //Cz_Pyxvital

      m_pGUI->groupBoxPatient->setEnabled ( TRUE );
      m_pGUI->groupBoxAssure->setEnabled ( TRUE );
      m_pGUI->tabWidgetNotes->setEnabled ( TRUE );

     }break;
}
m_InterfaceMode = mode;
}
//------------------------------------------- setIdentiteEnabled -----------------------------------------------------------------------
void C_Manager::setIdentiteEnabled()
{m_pGUI->pushButton_FSE->setEnabled (TRUE );                 //Cz_Pyxvital
 if (m_pGUI->groupBoxPatient->isEnabled()) return;
 m_pGUI->groupBoxPatient->setEnabled ( TRUE );
 m_pGUI->groupBoxAssure->setEnabled ( TRUE );
 m_pGUI->tabWidgetNotes->setEnabled ( TRUE );
 m_pGUI->pushButtonAcceder->setEnabled (TRUE );
}

//------------------------------------------- setIdentiteDisabled -----------------------------------------------------------------------
void C_Manager::setIdentiteDisabled()
{if (! m_pGUI->groupBoxPatient->isEnabled()) return;
m_pGUI->groupBoxPatient->setEnabled (FALSE );
m_pGUI->groupBoxAssure->setEnabled (FALSE );
m_pGUI->tabWidgetNotes->setEnabled (FALSE );
m_pGUI->pushButtonAcceder->setEnabled ( FALSE );
m_pGUI->pushButton_FSE->setEnabled (FALSE );                 //Cz_Pyxvital

}

//--------------------------------- setIdentModifiedState -----------------------------------------------------------------------
void C_Manager::setIdentModifiedState(int state)
{if (G_pCApp->isCurrentIdentiteValid() && !isVerrou() )
   {m_Ident_IsModified = state;
    if (m_Ident_IsModified) m_pGUI->pushButtonSavModif->setEnabled(TRUE);
    else                    m_pGUI->pushButtonSavModif->setEnabled(FALSE);
   }
else  // si identite invalide ou verrouillee on dit que le dossier n'est pas modifie
   {m_Ident_IsModified = FALSE;
    m_pGUI->pushButtonSavModif->setEnabled(FALSE);
   }
}

//------------------------------------------- setVerrouForCurrentIdent -----------------------------------------------------------------------
void C_Manager::setVerrouForCurrentIdent()
{if (G_pCApp->isCurrentIdentiteValid())
   { setVerrou(m_pCMoteurBase->IsDossierVerrouille( G_pCApp->identitePatientGUID()));
   }
else
   { setVerrou(FALSE);
   }
}

//------------------------------------------- setVerrou -----------------------------------------------------------------------
void C_Manager::setVerrou(bool state)
{m_IsVerrouOn = state;
if (state){ m_pGUI->pushButtonAcceder->setIcon ( Theme::getIcon("listview_locked.png") );
            m_pGUI->pushButtonDetruire->setEnabled(FALSE);
          }
else      { m_pGUI->pushButtonAcceder->setIcon ( Theme::getIcon("listview_acceder.png") );
            m_pGUI->pushButtonDetruire->setEnabled(TRUE);
          }
}

///////////////////////////////////////// TRAITEMENT SPECIAUX DES CONTROLES DE CHAMP ////////////////////////////////////////
//--------------------------------- setSexFromQualiteAyantDroit -------------------------------------------------------------
int C_Manager::setSexFromQualiteAyantDroit(int qualiteAyantDroit)
{       QString nss =  m_pGUI->lineEdit_NumSecu->text();
       switch(qualiteAyantDroit)
             {case 0:    // assure
                       {if (nss[0]=='1')  m_pGUI->comboBoxSexe->setCurrentItem ( 0 ) ;
                        else              m_pGUI->comboBoxSexe->setCurrentItem ( 1 ) ;
                        tryToSetTitreFromDateNss();  // regler le TITRE
                       } break;
              case 2:   // conjoint
              case 3:   // conjoint divorc\303\251
              case 4:   // concubin
              case 5:   // conjoint separe
              case 7:   // conjoint veuf
                       {if (nss[0]=='1')  m_pGUI->comboBoxSexe->setCurrentItem ( 1 ) ;
                        else              m_pGUI->comboBoxSexe->setCurrentItem ( 0 ) ;
                         tryToSetTitreFromDateNss();  // regler le TITRE
                       } break;
              default: {m_pGUI->lineEdit_Titre->setText("");
                        m_pGUI->comboBoxSexe->setCurrentItem ( 2 ) ;
                       } break;
             }
   return m_pGUI->comboBoxSexe->currentItem();
}
//--------------------------------- Slot_pushButtonInfoClicked -------------------------------------------------------------
void C_Manager::Slot_pushButtonInfoClicked()
{QWhatsThis::showText (QCursor().pos(), "<font color=\"#000000\">"+tr("To search by family name and first name, you must separe the family name and the first name by a semicolon. Example : <b>MARTIN;JEAN</b><br /><br />"
                                         "The symbol % before the family name or the first name lets make a research by anything which contains the expression<br>"
                                         "Example:  <b>%MART;%JE</b> searches every person whose family name contains: <b>MART</b> and the first name : <b>JE</b>")+"</font>" );
}

//---- AYANT DROIT ------------- tryToSetInterfaceFromAyantDroit ---------------------------------------------------------------------------------
void  C_Manager::tryToSetInterfaceFromAyantDroit( int qualiteAssure, int focusMustBePotionned)
{if (qualiteAssure ==0)
   {m_pGUI->lineEdit_NomAssure->setEnabled(FALSE);
    m_pGUI->lineEdit_PrenomAssure->setEnabled(FALSE);
    m_pGUI->lineEdit_DtNss_Assure->setEnabled(FALSE);
    //QString        dtNss      = m_pGUI->lineEdit_DtNss->text().remove('-');
    //QDate        qDteNss(dtNss.mid(4,4).toInt(), dtNss.mid(2,2).toInt(),  dtNss.mid(0,2).toInt()  );
    //if (qDteNss.isValid () )    m_pGUI->lineEdit_DtNss_Assure->setText(lineEdit_DtNss_Assure->text());
   }
else
   {m_pGUI->lineEdit_NomAssure->setEnabled(TRUE);
    m_pGUI->lineEdit_PrenomAssure->setEnabled(TRUE);
     m_pGUI->lineEdit_DtNss_Assure->setEnabled(TRUE);
    if (m_pGUI->lineEdit_NumSecu->text().remove(' ').length()==13 && m_pGUI->lineEdit_NumSecu->hasFocus() &&
        focusMustBePotionned)      m_pGUI->lineEdit_NomAssure->setFocus();
    if (m_pGUI->lineEdit_NomAssure->text().trimmed().length()==0)                               m_pGUI->lineEdit_NomAssure->setText(m_pGUI->lineEdit_NomDossier->text());
    if (m_pGUI->lineEdit_PrenomAssure->text().trimmed().length()==0)                            m_pGUI->lineEdit_PrenomAssure->setText(m_pGUI->lineEdit_PrenomDossier->text());
   }
}

//-- DATE NAISSANCE --------------------------------- Slot_OutFocuslineEdit_DtNss -----------------------------------------------------------------------
void C_Manager::Slot_OutFocuslineEdit_DtNss(QFocusEvent*, int &)
{ tryToSetTitreFromDateNss();
}

//------------------------------------------- tryToSetTitreFromDateNss -----------------------------------------------------------------------
void C_Manager::tryToSetTitreFromDateNss()
{
   // `HistN0_Date`  >=  "2001-05-19T11:09:40" AND `HistN0_Date`  <=  "2001-05-20T11:09:41";
   QString resolvToken("");
   QString        dtNss      = m_pGUI->lineEdit_DtNss->text().remove('-');
   QDate        qDteNss(dtNss.mid(4,4).toInt(), dtNss.mid(2,2).toInt(),  dtNss.mid(0,2).toInt()  );
   if (qDteNss.isValid () )
      {QString sAge = m_pCMoteurBase->CalculeDiffAge(&qDteNss, 0, TRUE, 0);
       m_pGUI->groupBoxPatient->setTitle(tr("Patient (b\303\251n\303\251ficiaire) \303\242g\303\251 de : ")+sAge);
       int i = 0; while (sAge[i]>='0' && sAge[i]<='9') ++i; sAge.truncate(i);
       int               age      = sAge.toInt();

       if (age < 15)                                              resolvToken="L'enfant";
       if (age >= 15 && age <= 21)
          { if (m_pGUI->comboBoxSexe->currentItem()==1)           resolvToken="Mademoiselle";
            else if (m_pGUI->comboBoxSexe->currentItem()==0)      resolvToken="Monsieur";
            else                                                  resolvToken="";
          }
       if (age>21)
          {if (m_pGUI->comboBoxSexe->currentItem()==1)            resolvToken="Madame";
           else if (m_pGUI->comboBoxSexe->currentItem()==0)       resolvToken="Monsieur";
           else                                                   resolvToken="";
          }
       m_pGUI->lineEdit_Titre->setText(resolvToken);
      }
   else
      {m_pGUI->groupBoxPatient->setTitle(tr("Patient (b\303\251n\303\251ficiaire) avec une date de naissance non valide"));
       m_pGUI->lineEdit_Titre->setText("");
      }
}

//-- VILLE --------------------------------- Slot_OutFocusVille -----------------------------------------------------------------------
void C_Manager::Slot_OutFocusVille(QFocusEvent*, int &)
{tryToSetCodePostalFromVille();
}

//------------------------------------------- tryToSetCodePostalFromVille -----------------------------------------------------------------------
void C_Manager::tryToSetCodePostalFromVille()
{//if (m_pCMoteurBase==0) return;
   QString codePostal("");
   QString ville = m_pGUI->lineEditVille->text().trimmed();
   //.......... si pas de ville saisie on tente avec le code postal .........................
   if (ville.length()==0)
      {tryToSetVilleFromCodePostal();
       return;
      }
   //............... chercher liste de codepostaux pouvant correspondre \303\240 cette ville .............
   QStringList codePostalList;
   m_pCMoteurBase->GetCodePostalFromVille( ville.replace("SAINT ","ST "),  codePostalList);
   if (codePostalList.findIndex ( m_pGUI->lineEdit_CdPostal->text().remove(' ') + " " + m_pGUI->lineEditVille->text() ) != -1 ) return;

   int nb = codePostalList.count();
   if (nb==1)
      {codePostal = codePostalList[0];
      }
   else if (nb>1 && nb<35 )
      {//............... deconnecter car lors popup on va quitter le champ lineEditVille .................
       //                pour eviter de rerentrer ici en boucle
       QObject::disconnect( m_pGUI->lineEditVille, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)), this,     SLOT(   Slot_OutFocusVille(QFocusEvent*, int &))  );
       codePostal = G_pCApp->DoPopupList(codePostalList);
       connect( m_pGUI->lineEditVille,             SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)), this,     SLOT(   Slot_OutFocusVille(QFocusEvent*, int &))  );
      }
   //............ separer la ville du code postal ...................................................
   int pos = codePostal.find(' ');
   if (pos != -1)
      {ville      = codePostal.mid(pos+1);
       codePostal = codePostal.left(pos);
      }
   if (//lineEdit_CdPostal->text().length()<5           ||                               // si code postal incomplet
       (codePostal.length()!=0 /*&& lineEdit_CdPostal->text() != codePostal */)            // ou non conforme avec celui de la ville
      )
      {m_pGUI->lineEdit_CdPostal->setText(codePostal );                                        // placer celui de la ville
       m_pGUI->lineEditVille->setText(ville);
       return;
      }
   //.............. si pas de code postal trouv\303\251 pour cette ville tenter de placer la ville du code postal .............
   if (codePostal.length()==0 )
      {ville = m_pCMoteurBase->GetVilleFromCodePostal(m_pGUI->lineEdit_CdPostal->text().remove(' '));
       if (ville.length()!=0) m_pGUI->lineEditVille->setText(ville);
      }
}

//-- CODE POSTAL --------------------------------- Slot_OutFocusCodePostal -----------------------------------------------------------------------
void C_Manager::Slot_OutFocusCodePostal(QFocusEvent*, int &)
{tryToSetVilleFromCodePostal();
}

//------------------------------------------- Slot_CharEventCodePostal -----------------------------------------------------------------------
void C_Manager::Slot_CharEventCodePostal(QKeyEvent *event, int &ret)
{ // on incorpore le caractere dans la QLineEdit car ce n'est pas encore fait
 // lors de l'appel de ce Slot par  C_QLineEdit
 m_pGUI->lineEdit_CdPostal->doParentPressEvent(event);
 ret = 0;    // du coup plus besoin d'\303\252tre trait\303\251 en retour de slot par C_QLineEdit
 //.......... maintenant  on  traite le code postal .................
 //           en appelant directement la fonction de contr?le
 tryToSetVilleFromCodePostal();
}

//------------------------------------------- tryToSetVilleFromCodePostal -----------------------------------------------------------------------
void C_Manager::tryToSetVilleFromCodePostal()
{   QStringList villeList;
   QString codePostal = m_pGUI->lineEdit_CdPostal->text().remove(' ');
   if (codePostal.length()==5)
   {m_pCMoteurBase->GetVilleFromCodePostal(codePostal, villeList);
       int nb = villeList.count();
       if (nb==1)
          {m_pGUI->lineEditVille->setText(villeList[0]);
          }
       else if (nb>1 && nb<35 )
       {if (villeList.findIndex ( m_pGUI->lineEditVille->text()) == -1 )  // si la ville ne correspond pas au code postal
           {//............... deconnecter car lors popup on va quitter le champ lineEdit_CdPostal .................
            //                pour eviter de rerentrer ici en boucle
            QObject::disconnect( m_pGUI->lineEdit_CdPostal, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),    this,     SLOT(   Slot_OutFocusCodePostal(QFocusEvent*, int &))  );
            QString ville = G_pCApp->DoPopupList(villeList);
            if (ville.length()>0) m_pGUI->lineEditVille->setText(ville);
            connect( m_pGUI->lineEdit_CdPostal, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),                this,     SLOT(   Slot_OutFocusCodePostal(QFocusEvent*, int &))  );
           }
       }
   }
   else
   {m_pGUI->lineEditVille->clear();
   }
}

//-- NUM SECU --------------------------------- Slot_CharEventNumSecu -----------------------------------------------------------------------
void C_Manager::Slot_CharEventNumSecu(QKeyEvent *event, int &ret)
{ // on incorpore le caractere dans la QLineEdit car ce n'est pas encore fait
 // lors de l'appel de ce Slot par  C_QLineEdit
 m_pGUI->lineEdit_NumSecu->doParentPressEvent(event);
 ret = 0;    // du coup plus besoin d'\303\252tre trait\303\251 en retour de slot par C_QLineEdit
 //.......... maintenant  on  traite le code postal .................
 //           en appelant directement la fonction de contr?le
 QString numsecu = m_pGUI->lineEdit_NumSecu->text().remove(' ');  //18105m_IsModifiable = 0;
 if (numsecu.length()==13)
      {m_pGUI->lineEdit_NssClef->setText(m_pCMoteurBase->ComputeClef_Secu(numsecu));
      }
   else
      {m_pGUI->lineEdit_NssClef->clear();
      }
}
// ---------------------------------------------- Slot_InFocusNumSecu  --------------------------------------------------------------
void C_Manager::Slot_InFocusNumSecu(QFocusEvent*, int &)
{   QString numsecu = m_pGUI->lineEdit_NumSecu->text().remove(' ');  //18105m_IsModifiable = 0;

   if (numsecu.length()==0)
      { if (m_pGUI->comboBoxSexe->currentItem() != 1)  numsecu += "1";
        else                                           numsecu += "2";
        QString dtnss = m_pGUI->lineEdit_DtNss->text().remove('-');
        if (dtnss.length()==8)
           {numsecu += dtnss.mid(6);
            numsecu += dtnss.mid(2,2);
           }
        m_pGUI->lineEdit_NumSecu->setText(numsecu);
        m_pGUI->lineEdit_NumSecu->setCursorPosition (numsecu.length());
      }
}

//---------------------------------------------- Slot_OutFocusNumSecu ----------------------------------------------
void C_Manager::Slot_OutFocusNumSecu(QFocusEvent*, int &)
{   QString numsecu = m_pGUI->lineEdit_NumSecu->text().remove(' ');  //18105
   if (numsecu.length()==13)
      {m_pGUI->lineEdit_NssClef->setText(m_pCMoteurBase->ComputeClef_Secu(numsecu));
      }
   else
      {m_pGUI->lineEdit_NssClef->clear();
      }

   QString leftNumSecu("");
   if (m_pGUI->comboBoxSexe->currentItem() != 1) leftNumSecu += "1";
   else                                          leftNumSecu += "2";
   QString dtnss = m_pGUI->lineEdit_DtNss->text().remove('-');
   if (dtnss.length()==8)
      {   leftNumSecu += dtnss.mid(6);
          leftNumSecu += dtnss.mid(2,2);
      }
   if (numsecu.length()>=5 && leftNumSecu.length()>=5 && leftNumSecu != numsecu.left(5))
      { QObject::disconnect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_focusInEvent(QFocusEvent*, int &)),           this,     SLOT(   Slot_InFocusNumSecu(QFocusEvent*, int &))  );
        QObject::disconnect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),          this,     SLOT(   Slot_OutFocusNumSecu(QFocusEvent*, int &))  );
       if (m_pGUI->comboBoxQualiteAyantDroit->currentItem()==0) m_pGUI->comboBoxQualiteAyantDroit->setCurrentItem(9);  // autre ayant droit
       m_pGUI->lineEdit_NomAssure->setEnabled(TRUE);
       m_pGUI->lineEdit_PrenomAssure->setEnabled(TRUE);
       if (m_pGUI->lineEdit_NomAssure->text()=="")     m_pGUI->lineEdit_NomAssure->setText(m_pGUI->lineEdit_NomDossier->text());
       if (numsecu.length()==13 && m_pGUI->lineEdit_NumSecu->hasFocus())
          {m_pGUI->lineEdit_NomAssure->setFocus();
           m_pGUI->lineEdit_NomAssure->setCursorPosition (-1);
          }
       connect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_focusInEvent(QFocusEvent*, int &)),           this,     SLOT(   Slot_InFocusNumSecu(QFocusEvent*, int &))  );
       connect( m_pGUI->lineEdit_NumSecu, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),          this,     SLOT(   Slot_OutFocusNumSecu(QFocusEvent*, int &))  );
     }
   else
      {m_pGUI->comboBoxQualiteAyantDroit->setCurrentItem(0);
       m_pGUI->lineEdit_NomAssure->setEnabled(FALSE);
       m_pGUI->lineEdit_PrenomAssure->setEnabled(FALSE);
      }
}

////////////////////////////////////// DETECTION DES MODIFICATIONS DE LA FICHE IDENTITE ////////////////////////////////////////////////////////
//........................ Slot_TextChanged_lineEdit_NomDossier ............................................................................................
void C_Manager::Slot_TextChanged_lineEdit_NomDeNss(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_DtNss(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_Titre(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_Profession(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_textEdit_Adresse( )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_Tel1(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_Tel2(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_Tel3(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_CdPostal(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEditVille(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_Email(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_NumSecu(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_NomAssure(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_PrenomAssure(const QString & )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_textEdit_Note( )
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_DtNss_Assure(const QString &)
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_lineEdit_RangGeme(const QString &)
{setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_comboBoxSexe( const QString &)
{tryToSetTitreFromDateNss(); setIdentModifiedState(TRUE);
}
void C_Manager::Slot_TextChanged_comboBoxQualiteAyantDroit( const QString &)
{tryToSetInterfaceFromAyantDroit( m_pGUI->comboBoxQualiteAyantDroit->currentIndex(), 1); setIdentModifiedState(TRUE);
}

//////////////////////////////////////// LISTE DES PATIENTS ////////////////////////////////////////////////////////////////////////////////
//--------------------------------- Slot_giveDraglistView_PatientItemData -----------------------------------------------------------------------
void C_Manager::Slot_giveDraglistView_PatientItemData(QString &data, QTreeWidgetItem *pQTreeWidgetItem)
{   if (pQTreeWidgetItem==0) return;
   C_RendezVous rdv;
   rdv.m_GUID   = pQTreeWidgetItem->text(LIST_GUID);
   rdv.m_Nom    = pQTreeWidgetItem->text(LIST_NOM);
   rdv.m_Prenom = pQTreeWidgetItem->text(LIST_PRENOM);
   rdv.m_Tel    = "";
   if (G_pCApp->isCurrentIdentiteSame(pQTreeWidgetItem))
      {rdv.m_Tel    = m_pGUI->lineEdit_Tel1->text();
       rdv.m_Where  = m_pGUI->textEdit_Adresse->text()+" "+m_pGUI->lineEdit_CdPostal->text().remove(' ')+" "+m_pGUI->lineEditVille->text();
      }
   data = rdv.serialize(&rdv);
}

//--------------------------------- Slot_listView_Patient_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_listView_Patient_Clicked( QTreeWidgetItem *pQListViewItem , int)
{
#ifdef SESAM_VERSION
if (pQListViewItem==0) return;
clearDroitsFields();
#endif
}

//--------------------------------- Slot_listView_Patient_DoubleClicked -----------------------------------------------------------------------
void C_Manager::Slot_listView_Patient_DoubleClicked( QTreeWidgetItem *, int)
{//Slot_pushButtonAcceder_Clicked();
// Cz_Pyxvital     si on ne gï¿½re pas le dossier Patient on lance directement la FSE
if (m_Facture_Seule == "oui")
    Slot_pushButton_FSE();
else
    Slot_pushButtonAcceder_Clicked();
}

//--------------------------------- listPatientsByDateNss -----------------------------------------------------------------------
void C_Manager::listPatientsByDateNss(QString dateNss)
{
dateNss     = dateNss.insert(4,'-');                           //  19930601       -->  1993-0601
dateNss     = dateNss.insert(7,'-');                           //  1993-0601      -->  1993-06-01
dateNss    += " 00:00:00";                                     //  1993-0601      -->  1993-06-01 00:00:00
//m_pGUI->pushButtonCancel_clicked();
if (G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme(m_pGUI->listView_Patient,         // pas de listview car on veut juste savoir si trouve
                                                                     "",         // nom
                                                                     "",         // prenom
                                                                     dateNss,    // date Nss // doit etre 1993-06-01 00:00:00
                                                                     "",         // num secu // 160196296519557
                                                                     "",         // rang gemellaire
                                                                     "",         // qualite d'ayant droit
                                                                     "",         // adresse,
                                                                     "",         // codePost,
                                                                     "",         // ville,
                                                                     "",         // sexe,
                                                                     "",         // profession,
                                                                     "",         //  nomJfille
                                                                     "",         //  nomAss,
                                                                     "",         //  prenomAss,
                                                                     "",         //  tel1,
                                                                     "",         //  tel2,
                                                                     "",         //  tel3,
                                                                     "",         //  email,
                                                                     " LIKE ",   // egal_like  " = ",
                                                                     m_pGUI->textLabel_StateList
                                                                   )
  )
  {
  }
}
//--------------------------------- Slot_ListView_ContextMenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_ListView_ContextMenuRequested (const QPoint &)
{Slot_pushButtonMenuNew_clicked();
}

//--------------------------------- changeAllAgendasGuid -----------------------------------------------------------------------
void C_Manager::changeAllAgendasGuid(const QString &old_guid, const QString &new_guid)
{
 QMapIterator<QString, C_Frm_Agenda*> it(m_AgendaMap);
 while (it.hasNext())
       {it.next();
        C_Frm_Agenda *pC_Frm_Agenda = it.value();
        if (pC_Frm_Agenda)
           {pC_Frm_Agenda->changeGUID (old_guid, new_guid);
           }
       }
}
//--------------------------------- Slot_pushButtonMenuNew_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonMenuNew_clicked()
{
QStringList list;
QTreeWidgetItem *pQListViewItem         = getSelectedListViewPatientItem();

if (pQListViewItem){if( getInterfaceMode()==MODE_CREATION_PATIENT)
                        list.append(tr("=1=Use the data of %1 to fill the new form").arg(pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM)));
                    else
                        list.append(tr("=2=Create a new file using data from: ")+pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM));
                    list.append("-------------");
                   }
if (m_pGUI->listView_Doublons->isShown())  list.append(tr("=3=Close the duplicate resolution list"));
else if (G_pCApp->IsThisDroitExist("adr")) list.append(tr("=4=Open the duplicate resolution list"));
if (pQListViewItem){ //.............. verifier si pas verrouill\303\251 .................................if (!G_pCApp->IsThisDroitExist("adr")) return;
                     if (! m_pCMoteurBase->IsDossierVerrouille   ( pQListViewItem->text(LIST_GUID)) )
                        {if ( G_pCApp->IsThisDroitExist("adr")) list.append(tr("=5=Put this patient : %1 into the list of duplicates resolution").arg(pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM)));
                         list.append("-------------");
                        }
                     else if (!G_pCApp->IsThisDroitExist("nbv") && (G_pCApp->readUniqueParam ( "Manager session", "Remove Locks").lower()[0] != 'n'))
                        {list.append("-------------");
                         list.append(tr("=6=unlock this patient's file: %1 ").arg(pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM)));
                         list.append("-------------");
                        }

                     //list.append(tr("=7=Rechercher par le nom : ")+lineEdit_NomDossier->text());
                     //list.append(tr("=8=Rechercher par le pr\303\251nom : ")+lineEdit_PrenomDossier->text());
                     list.append(tr("=9=Research by birth date: ")+m_pGUI->lineEdit_DtNss->text());
                     //list.append(tr("=10=Rechercher par le n. de jeune fille : ")+lineEdit_NomDeNss->text());
                     list.append(tr("=11=Rechercher par le num\303\251ro de s\303\251curit\303\251 sociale : ")+m_pGUI->lineEdit_NumSecu->text());
                     list.append(tr("=12=Research by family name '%1', first name '%2' and birth date '%3' found in dialog's fields.").arg(m_pGUI->lineEdit_NomDossier->text(),m_pGUI->lineEdit_PrenomDossier->text(), m_pGUI->lineEdit_DtNss->text()));
                   }
 #ifdef SESAM_VERSION
QTreeWidgetItem *pQListViewItemVitale   = getSelectedListViewItem(m_pGUI->listView_Vitale);
if (pQListViewItemVitale && pQListViewItem)
   {QString dnss      = pQListViewItemVitale->text(2).left(8);             //  199306010000   -->  19930601
    dnss = dnss.mid(6,2)+"-"+dnss.mid(4,2)+"-"+dnss.mid(0,4);
    if (dnss==m_pGUI->lineEdit_DtNss->text())
       {list.prepend("-------------");
        list.prepend("=30=#22x22/VitaleButtLitle.png#"+tr("Associer la carte vitale s\303\251lectionn\303\251e : %1 \303\240 ce patient : %2").arg(pQListViewItemVitale->text(0)+" "+pQListViewItemVitale->text(1),pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM)));
       }
   }
#endif
#ifdef ENTREES_SIGEMS
QTreeWidgetItem *pQListViewItemSigems   = getSelectedListViewItem(m_pGUI->listView_Sigems);

if (pQListViewItemSigems && pQListViewItem)
   {QString dnss      = pQListViewItemSigems->text(2).left(8);             //  199306010000   -->  19930601
    dnss = dnss.mid(6,2)+"-"+dnss.mid(4,2)+"-"+dnss.mid(0,4);
    if (dnss==m_pGUI->lineEdit_DtNss->text())
       {list.prepend("-------------");
        list.prepend("=31=#22x22/Sigems.png#"+tr("Associer l'entr\303\251e SIGEMS s\303\251lectionn\303\251e : %1 \303\240 ce patient : %2").arg(pQListViewItemSigems->text(0)+" "+pQListViewItemSigems->text(1),pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM)));
       }
   }
#endif
// XXXXXXXXXXXXXXXXXXXXXX A REVOIR AVEC RS
// Le CLICK DROIT sur la listview Patients ne selectionne pas l'item....
// il faut faite click gauche puis clic droit pour ouvrit le menu contextuel.
// Forcer le Item selected + Itemchanged ....
//??????????????????????????????????????????

QString      ret = G_pCApp->DoPopupList(list,"");
bool pas_present = TRUE;
int       option = ret.toInt();
switch(option)
   {
   #ifdef SESAM_VERSION
    case 30:
      { setCVonPatient ( G_pCApp->m_pVitale, pQListViewItemVitale->text(8).toInt(),"VITALE");
      } break;
   #endif
   #ifdef ENTREES_SIGEMS
   case 31:
     { setCVonPatient ( G_pCApp->m_pVitale, pQListViewItemSigems->text(8).toInt(), "SIGEMS");
       // On remplace le GUID Patient par l'ID Sigems dans toutes les tables.
       m_pCMoteurBase->Replace_GUID_Patient_By_ID_Sigems_Everywhere(pQListViewItem, pQListViewItemSigems->text(14));
       // on deselecte l'entee sigems
       m_pGUI->listView_Sigems->currentItem()->setSelected(false);
       m_pGUI->textLabel_NumDoss->setText(pQListViewItemSigems->text(14));
       changeAllAgendasGuid(pQListViewItem->text(3), pQListViewItemSigems->text(14));  // changer les GUID de l'agenda
       pQListViewItem->setText(3, pQListViewItemSigems->text(14));

     } break;
   #endif
    case 6:
      {m_pCMoteurBase->DeVerrouilleDossier ( pQListViewItem->text(LIST_GUID));
       pQListViewItem->setIcon ( 0, QIcon() );
      } break;
    case 5:
      {m_pGUI->listView_Doublons->show();
       QTreeWidgetItem *pQListViewItem_D = 0;
       //.............. verifier si pas deja present .................................
       QTreeWidgetItemIterator it (m_pGUI->listView_Doublons);
       while (*it)
             { pQListViewItem_D = (*it);
               if (pQListViewItem->text(LIST_PK_PATIENT) == pQListViewItem_D->text(LIST_PK_PATIENT))
                  {pas_present = FALSE;
                   break;
                  }
               ++it;
             }
       if (pas_present)
          {pQListViewItem_D = new QTreeWidgetItem(*pQListViewItem);   // copie de pQListViewItem
           if (pQListViewItem_D) m_pGUI->listView_Doublons->addTopLevelItem(pQListViewItem_D);
          }
       else
          {G_pCApp->CouCou(tr("The patient %1 is already in the duplicates resolution list").arg(pQListViewItem->text(LIST_NOM)+" "+pQListViewItem->text(LIST_PRENOM)));
          }
       //.............. verifier si pas verrouill\303\251 .................................
       /*               inutile car normalement ne devrait pas etre possible
                        car si verrouille le menu ne permet pas d'ajout au doublon
       QString usedBy;
       if (m_pCMoteurBase->IsDossierVerrouille   ( pQListViewItem_D->text(LIST_GUID), usedBy ))
          {pQListViewItem_D->setIcon ( 0, QIcon(Theme::getIcon( "16x16/locked.png")) );
           pQListViewItem_D->setText ( 4, usedBy );
          }
       else
          {pQListViewItem_D->setIcon ( 0, QIcon() );
           pQListViewItem_D->setText ( 4, "" );
          }
       */
       break;
      }
    case 1:
     {m_pGUI->textEdit_Note->clear();
      m_pGUI->listView_Praticiens->clear();
      DataToFormulaire( pQListViewItem );
      break;
     }
    case 2:
     {m_pGUI->textEdit_Note->clear();
      m_pGUI->listView_Praticiens->clear();
      Slot_NewDossWithoutErase();
      break;
     }
    //case 'N':
    // {Slot_pushButtonNew_clicked();
    //  break;
    // }
    case 3:
      {m_pGUI->listView_Doublons->hide();
       break;
      }
    case 4:
      {m_pGUI->listView_Doublons->show();
       break;
      }
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      {QString nom     = "";
       QString prenom  = "";
       QString dtNss   = m_pGUI->lineEdit_DtNss->text().remove('-');
       QString numSecu = "";
       QString jeuneF  = "";
       if      (option==7||option==12)       nom        =   QString("%")+m_pGUI->lineEdit_NomDossier->text()+"%";
       if      (option==8||option==12)       prenom     =   QString("%")+m_pGUI->lineEdit_PrenomDossier->text()+"%";
       if      (option==9||option==12)       dtNss      =   dtNss.mid(4,4)+"-"+dtNss.mid(2,2)+"-"+dtNss.mid(0,2)+ " 00:00:00";
       if      (option==11)                  numSecu    =   QString("%")+m_pGUI->lineEdit_NumSecu->text().remove(' ')+"%";
       if      (option==10||option==12)      jeuneF     =   QString("%")+m_pGUI->lineEdit_NomDeNss->text()+"%";
       if (G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme(m_pGUI->listView_Patient,         // pas de listview car on veut juste savoir si trouve
                                                                                    nom,        // nom
                                                                                    prenom,     // prenom
                                                                                    dtNss,      // date Nss // doit etre 1993-06-01 00:00:00
                                                                                    numSecu,    // num secu // 160196296519557
                                                                                    "",         // rang gemellaire
                                                                                    "",         // qualite d'ayant droit
                                                                                    "",         // adresse,
                                                                                    "",         // codePost,
                                                                                    "",         // ville,
                                                                                    "",         // sexe,
                                                                                    "",         // profession,
                                                                                    jeuneF,     //  nomJfille
                                                                                    "",         //  nomAss,
                                                                                    "",         //  prenomAss,
                                                                                    "",         //  tel1,
                                                                                    "",         //  tel2,
                                                                                    "",         //  tel3,
                                                                                    "",         //  email,
                                                                                    " LIKE ",   // egal_like  " = ",
                                                                                    m_pGUI->textLabel_StateList
                                                                                  )
                 )
                 {
                 }
       break;
      }
   }
}
//--------------------------------- Slot_TextChanged_lineEdit_NomDossier -----------------------------------------------------------------------
void C_Manager::Slot_TextChanged_lineEdit_NomDossier(const QString &nom)
{if (getInterfaceMode()==MODE_CREATION_PATIENT) initListePatient( nom, m_pGUI->lineEdit_PrenomDossier->text());
else                                           setIdentModifiedState(TRUE);
}
//--------------------------------- Slot_TextChanged_lineEdit_PrenomDossier -----------------------------------------------------------------------
void C_Manager::Slot_TextChanged_lineEdit_PrenomDossier(const QString &prenom)
{if (getInterfaceMode()==MODE_CREATION_PATIENT) initListePatient(m_pGUI->lineEdit_NomDossier->text(), prenom);
else                                           setIdentModifiedState(TRUE);
}

//--------------------------------- Slot_lineEditAutoLocator_keyPressEvent -----------------------------------------------------------------------
void C_Manager::Slot_lineEditAutoLocator_keyPressEvent(QKeyEvent *event, int &callStandardEvent)
{QTreeWidgetItem * pQTreeWidgetItem = 0;
switch(event->key())
  {case Qt::Key_Return:
   case Qt::Key_Enter:
         pQTreeWidgetItem = getSelectedListViewPatientItem();
         if (pQTreeWidgetItem==0 && m_pGUI->listView_Patient->topLevelItemCount()==1)
            {pQTreeWidgetItem = m_pGUI->listView_Patient->topLevelItem(0);
             if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
            }
         if (getInterfaceMode()==MODE_SELECTION_PATIENT) Slot_pushButtonAcceder_Clicked();
         callStandardEvent = 0;
        break;
   case Qt::Key_Down:
        {pQTreeWidgetItem = getSelectedListViewPatientItem();
         if (pQTreeWidgetItem)
            {QTreeWidgetItem * pQTreeWidgetItemBelow = m_pGUI->listView_Patient->itemBelow( pQTreeWidgetItem );
             if (pQTreeWidgetItemBelow)
                {pQTreeWidgetItem->setSelected(FALSE);
                 pQTreeWidgetItemBelow->setSelected(TRUE);
                }
            }
         else if (m_pGUI->listView_Patient->topLevelItemCount ())
            {pQTreeWidgetItem = m_pGUI->listView_Patient->topLevelItem(0);
             if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
            }
         callStandardEvent = 0;
        }
        break;
   case Qt::Key_Up:
        {pQTreeWidgetItem = getSelectedListViewPatientItem();
         if (pQTreeWidgetItem)
            {QTreeWidgetItem * pQTreeWidgetItemAbove = m_pGUI->listView_Patient->itemAbove( pQTreeWidgetItem );
             if (pQTreeWidgetItemAbove)
                {pQTreeWidgetItem->setSelected(FALSE);
                 pQTreeWidgetItemAbove->setSelected(TRUE);
                }
            }
         else if (m_pGUI->listView_Patient->topLevelItemCount ())
            {pQTreeWidgetItem = m_pGUI->listView_Patient->topLevelItem(0);
             if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
            }
         callStandardEvent = 0;
        }
        break;
  }
}

//--------------------------------- Slot_pushButton_ReinitListe_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_ReinitListe_clicked()
{m_pGUI->lineEditAutoLocator->setText("");
m_pGUI->lineEditAutoLocator->setFocus();
Slot_lineEditAutoLocator_textChanged("");
}

//--------------------------------- getSelectedListViewPatientItem -------------------------------------------------------------
QTreeWidgetItem * C_Manager::getSelectedListViewPatientItem()
{   return getSelectedListViewItem(m_pGUI->listView_Patient);
}

//--------------------------------- getSelectedListViewItem -------------------------------------------------------------
QTreeWidgetItem * C_Manager::getSelectedListViewItem(QTreeWidget *pQTreeWidget)
{QList<QTreeWidgetItem *> list = pQTreeWidget->selectedItems ();
if (list.count()) return list[0];
return 0;
}

//--------------------------------- Slot_ListView_itemSelectionChanged -------------------------------------------------------------
void C_Manager::Slot_ListView_itemSelectionChanged()
{
QTreeWidgetItem *pQListViewItem = getSelectedListViewPatientItem();
if (G_pCApp->isCurrentIdentiteSame(pQListViewItem))               return;
if (pQListViewItem==0)
   {G_pCApp->setCurrentIdentite(pQListViewItem);
    switch ( getInterfaceMode() )
           {case MODE_SELECTION_PATIENT:
                {setIdentiteDisabled();
                 setVerrouForCurrentIdent();
                }break;
            case MODE_CREATION_PATIENT:
                {setVerrouForCurrentIdent();
                }break;
            case MODE_MULTICRITERE:
                {setVerrouForCurrentIdent();
                }break;
           }
    return;
   }
//................... si besoin positionner le cadenas sur l'item  ...........
if (m_pCMoteurBase->IsDossierVerrouille   ( pQListViewItem->text(LIST_GUID)))
   {//pQListViewItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/locked.png")) );
   }
 else
   {//pQListViewItem->setIcon ( 0, QIcon() );
   }
//................... action de la selection selon interface ...........
switch ( getInterfaceMode() )
       {
        case MODE_SELECTION_PATIENT:
             {
              bool id_mod    = identIsModified();
              bool id_valid  =  G_pCApp->isCurrentIdentiteValid();                    // return m_LastGUID != "";
              bool droit_ok  = G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idc");
              if ( id_mod && id_valid &&  droit_ok)
                 {/*  pas besoin de questionner c'est d\303\251j\303\240 fait dans la fonction : Slot_pushButtonSavModif()
                  int ret   = QMessageBox::warning ( this, tr("Enregistrer les modifications :"),
                                    tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp;le dossier :<br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                    G_pCApp->m_LastNom  + " " + G_pCApp->m_LastPrenom  +
                                    tr(  "</font></b>&nbsp;&nbsp; a \303\251t\303\251 modifi\303\251, faut il enregistrer les modifications avant de s\303\251lectionner un autre dossier"),
                                    tr("&Modifier"), tr("Annu&ler"), 0,
                                    1, 1
                                    );
                   if (ret==0)
                      {Slot_pushButtonSavModif();
                      }
                   */
                  Slot_pushButtonSavModif();
                 }
              DataToFormulaire( pQListViewItem);
            }break;
        case MODE_CREATION_PATIENT:
            {/*
             int ret   = QMessageBox::warning ( this, tr("S\303\251lection d'un patient dans liste des patients :"),
                                    tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp;voulez utiliser les donn\303\251es du patient suivant :<br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                    pQListViewItem->text(0)  + " " + pQListViewItem->text(1)  +
                                    tr(  "</font></b><br>&nbsp;&nbsp; pour remplir les donn\303\251es du patient en cours de cr\303\251ation ?"),
                                    tr("&Utiliser"), tr("Annu&ler"), 0,
                                    0, 1
                                    );
             if (ret==0)
                {DataToFormulaire( pQListViewItem);
                 return;
                }
                */
            }break;
        case MODE_MULTICRITERE:
            {DataToFormulaire( pQListViewItem);
            }break;
       } //end switch ( getInterfaceMode() )
 G_pCApp->setCurrentIdentite(pQListViewItem);
 if (G_pCApp->isCurrentIdentiteValid()) setIdentiteEnabled();
 setVerrouForCurrentIdent();
 setMedWebTuxOnPatient();
}

//--------------------------------- setMedWebTuxOnPatient -------------------------------------------------------------
void C_Manager::setMedWebTuxOnPatient()
{
  //if (m_locationEdit->text().contains("MedWebTux"))
     {  //   [Web_Browser]
        //       MedWebTuxUrl = http://localhost/MedWebTux/
        // http://medintux.synology.me/MedWebTux/frame_patient.php?GUID=30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A
        QString medWebTuxUrl =  G_pCApp->readUniqueParam ( "Web_Browser", "MedWebTuxUrl");
        QString cripted_password;
        G_pCApp->m_pCMoteurBase->GetUserPrimKey(G_pCApp->m_SignUser, &cripted_password   /* = 0  */);
        medWebTuxUrl  = medWebTuxUrl.replace("{{HOSTNAME}}",G_pCApp->m_pCMoteurBase->m_HostName);
        medWebTuxUrl +="frame_patient.php?GUID="+G_pCApp->m_LastGUID;
        m_webView_MedWebTux->setUrl(QUrl(medWebTuxUrl));
        m_locationEdit->setText(medWebTuxUrl);
     }
}
//--------------------------------- DataToFormulaire -------------------------------------------------------------
int C_Manager::DataToFormulaire( const QTreeWidgetItem *pQTreeWidgetItem )
{ if (pQTreeWidgetItem==0) return 0;
  return DataToFormulaire( pQTreeWidgetItem->text(LIST_PK_PATIENT), pQTreeWidgetItem->text(LIST_NOM), pQTreeWidgetItem->text(LIST_PRENOM), pQTreeWidgetItem->text(LIST_GUID));
}
//--------------------------------- DataToFormulaire -------------------------------------------------------------
int C_Manager::DataToFormulaire(const QString &_refToPrimkeyDoss, const QString &nom, const QString &prenom , const QString &guid)
{/*
  QString      m_DOSS_IDENT_GUID;        // GUID du dossier auquel l'enregistrement fait reference
  QString      m_DOSS_IDENT_NSS ;        // date de naissance
  QString      m_DOSS_IDENT_SEX;         // sexe
  QString      m_DOSS_IDENT_JFNOM;       // nom de jeune fille
  QString      m_DOSS_IDENT_NB_ENF;      // nombre d'enfants
  QString      m_DOSS_IDENT_RUE;         // adresse
  QString      m_DOSS_IDENT_CODE_POST;   // Code postal
  QString      m_DOSS_IDENT_VILLE;       // Ville
  QString      m_DOSS_IDENT_CEDEX;       // cedex
  QString      m_DOSS_IDENT_PAYS;        // pays
  QString      m_DOSS_IDENT_TEL1;        // Telephone 1
  QString      m_DOSS_IDENT_TEL2;        // Telephone 2
  QString      m_DOSS_IDENT_TEL3;        // Telephone 3
  QString      m_DOSS_IDENT_TEL1_TYP;    // Telephone 1  type (parent amis etc...)
  QString      m_DOSS_IDENT_TEL2_TYP;    // Telephone 2
  QString      m_DOSS_IDENT_TEL3_TYP;    // Telephone 3
  QString      m_DOSS_IDENT_NUMSS;       // Num\303\251ro de secu
  QString      m_DOSS_IDENT_EMAIL;       // Num\303\251ro de secu
  QString      m_DOSS_IDENT_IS_ASS;      // le patient est il l'assur\303\251
  QString      m_DOSS_IDENT_NOM_ASS;     // nom de l'assur\303\251
  QString      m_DOSS_IDENT_PRENOM_ASS;  // pr\303\251nom de l'assur\303\251
  QString      m_DOSS_IDENT_NSS_ASS;     // date de naissance de l'assure
  QString      m_DOSS_IDENT_PROF;        // profession
  QString      m_DOSS_IDENT_TITRE;       // Titre( Dr Mr Md Pr ...)
  QString      m_DOSS_IDENT_REF_PK;      // reference \303\240 la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX
*/
   QString refToPrimkeyDoss = _refToPrimkeyDoss;
   if (_DataToFormulaire(refToPrimkeyDoss, nom, prenom)==0)
      { QString index_pk =  m_pCMoteurBase->GetFieldValue( m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME,      // table
                                                           m_pCMoteurBase->m_DOSS_IDENT_REF_PK,        // field to get
                                                           m_pCMoteurBase->m_DOSS_IDENT_GUID,          // field to compare
                                                           guid                                        // with value to compare
                                                         );
        //........  si pas d'identite pour ce GUID  il faut en creer une automatique .........
        if (index_pk.length()==0)
           {  PtrListIdentFieldData list_field;    // on cree une liste vide
              list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS,    "",          FTN_MAJ_STRING));
              refToPrimkeyDoss = m_pCMoteurBase->DossIdentIndexCreate( nom, prenom,  guid, &list_field);

              if (_DataToFormulaire(refToPrimkeyDoss, nom, prenom)==0)
                 {   QMessageBox::warning ( this, tr("S\303\251lection d'un patient dans liste des patients :"),
                                                   tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp; Echec de la cr\303\251tion de la fiche patient : <br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                                   nom  + " " + prenom  + " Guid : " + guid +
                                                   tr(  "</font></b><br>&nbsp;&nbsp; base de donn303\251e corrompue."),
                                                   tr("&J'ai vu"), 0, 0,
                                                   0, 1
                                           );
                     return 0;

                 }
           }
        //........  sinon identite pour ce GUID mais pas de  _refToPrimkeyDoss correct  il faut en creer la reindexer .........
        else
           {  refToPrimkeyDoss = m_pCMoteurBase->GetDossPrimKeyFromGUID(guid, 0);   // recuperer l'index correct
              if (refToPrimkeyDoss.length()==0)
                 { QMessageBox::warning ( this, tr("S\303\251lection d'un patient dans liste des patients :"),
                                          tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp; pas d'index pour ce patient : <br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                          nom  + " " + prenom  + " Guid : " + guid +
                                          tr(  "</font></b><br>&nbsp;&nbsp; base de donn303\251e suspecte."),
                                          tr("&J'ai vu"), 0, 0,
                                          0, 1
                                  );
                    return 0;

                 }
              refToPrimkeyDoss = m_pCMoteurBase->reindexerFichePatient( guid );
              if ( _DataToFormulaire(refToPrimkeyDoss, nom, prenom)==0 )
                 { QMessageBox::warning ( this, tr("S\303\251lection d'un patient dans liste des patients :"),
                                          tr( "<u><b>ATTENTION :</u></b><br>&nbsp;&nbsp;&nbsp; echec de la r\303\251indexation de cette fiche patient: <br>&nbsp;&nbsp; <b><font color=\"#e80d0d\">") +
                                          nom  + " " + prenom  + " Guid : " + guid +
                                          tr(  "</font></b><br>&nbsp;&nbsp; base de donn303\251e suspecte."),
                                          tr("&J'ai vu"), 0, 0,
                                          0, 1
                                  );
                    return 0;
                 }
           }
      }
   return 1;
}
//--------------------------------- _DataToFormulaire -------------------------------------------------------------
int C_Manager::_DataToFormulaire(const QString &refToPrimkeyDoss, const QString &nom, const QString &prenom)
{/*
  QString      m_DOSS_IDENT_GUID;        // GUID du dossier auquel l'enregistrement fait reference
  QString      m_DOSS_IDENT_NSS ;        // date de naissance
  QString      m_DOSS_IDENT_SEX;         // sexe
  QString      m_DOSS_IDENT_JFNOM;       // nom de jeune fille
  QString      m_DOSS_IDENT_NB_ENF;      // nombre d'enfants
  QString      m_DOSS_IDENT_RUE;         // adresse
  QString      m_DOSS_IDENT_CODE_POST;   // Code postal
  QString      m_DOSS_IDENT_VILLE;       // Ville
  QString      m_DOSS_IDENT_CEDEX;       // cedex
  QString      m_DOSS_IDENT_PAYS;        // pays
  QString      m_DOSS_IDENT_TEL1;        // Telephone 1
  QString      m_DOSS_IDENT_TEL2;        // Telephone 2
  QString      m_DOSS_IDENT_TEL3;        // Telephone 3
  QString      m_DOSS_IDENT_TEL1_TYP;    // Telephone 1  type (parent amis etc...)
  QString      m_DOSS_IDENT_TEL2_TYP;    // Telephone 2
  QString      m_DOSS_IDENT_TEL3_TYP;    // Telephone 3
  QString      m_DOSS_IDENT_NUMSS;       // Num\303\251ro de secu
  QString      m_DOSS_IDENT_EMAIL;       // Num\303\251ro de secu
  QString      m_DOSS_IDENT_IS_ASS;      // le patient est il l'assur\303\251
  QString      m_DOSS_IDENT_NOM_ASS;     // nom de l'assur\303\251
  QString      m_DOSS_IDENT_PRENOM_ASS;  // pr\303\251nom de l'assur\303\251
  QString      m_DOSS_IDENT_NSS_ASS;     // date de naissance de l'assure
  QString      m_DOSS_IDENT_PROF;        // profession
  QString      m_DOSS_IDENT_TITRE;       // Titre( Dr Mr Md Pr ...)
  QString      m_DOSS_IDENT_REF_PK;      // reference \303\240 la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX
*/
   int ret = 0;
   //m_pGUI->lineEdit_CAS->setText("");
   if  (m_pCMoteurBase->OpenBase()==0) return ret;
   m_pCMoteurBase->PatientIntervenantsGetData(refToPrimkeyDoss , m_pGUI->listView_Praticiens);
   //............... ouvrir base   ..................................................
   disconnect( m_pGUI->lineEdit_NomDossier,         SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomDossier(const QString & )    ));
   disconnect( m_pGUI->lineEdit_PrenomDossier,      SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_PrenomDossier(const QString & ) ));
   disconnect( m_pGUI->lineEdit_NomDeNss,           SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomDeNss(const QString & )      ));
   disconnect( m_pGUI->lineEdit_DtNss,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_DtNss(const QString & )         ));
   disconnect( m_pGUI->lineEdit_DtNss_Assure,       SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_DtNss_Assure(const QString &)   ));
   disconnect( m_pGUI->lineEdit_Titre,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Titre(const QString & )         ));
   disconnect( m_pGUI->lineEdit_Profession,         SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Profession(const QString &)     ));
   disconnect( m_pGUI->textEdit_Adresse,            SIGNAL( textChanged ( ) ),                        this, SLOT(   Slot_TextChanged_textEdit_Adresse( ))  );
   disconnect( m_pGUI->lineEdit_Tel1,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel1(const QString & ))  );
   disconnect( m_pGUI->lineEdit_Tel2,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel2(const QString & ))  );
   disconnect( m_pGUI->lineEdit_Tel3,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel3(const QString & ))  );
   disconnect( m_pGUI->lineEdit_CdPostal,           SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_CdPostal(const QString & ))  );
   disconnect( m_pGUI->lineEditVille,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEditVille(const QString & )) );
   disconnect( m_pGUI->lineEdit_Email,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Email(const QString & )) );
   disconnect( m_pGUI->lineEdit_NumSecu,            SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NumSecu(const QString & )) );
   disconnect( m_pGUI->lineEdit_NomAssure,          SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomAssure(const QString & ))  );
   disconnect( m_pGUI->lineEdit_PrenomAssure,       SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_PrenomAssure(const QString & ))  );
   disconnect( m_pGUI->textEdit_Note,               SIGNAL( textChanged ( ) ),                        this, SLOT(   Slot_TextChanged_textEdit_Note( )) );
   disconnect( m_pGUI->lineEdit_RangGeme,           SIGNAL( textChanged (const QString & ) ),         this, SLOT(   Slot_TextChanged_lineEdit_RangGeme(const QString &)) );
   disconnect( m_pGUI->comboBoxSexe,                SIGNAL( currentIndexChanged (const QString & ) ), this, SLOT(   Slot_TextChanged_comboBoxSexe( const QString &))  );

   //............... crer un curseur SQL ..................................................
   QString requete  = QString( "SELECT ");
           requete += m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS          + ", " +       // 0
                      m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS       + ", " +       // 1
                      m_pCMoteurBase->m_DOSS_IDENT_RUE              + ", " +       // 2
                      m_pCMoteurBase->m_DOSS_IDENT_TEL1             + ", " +       // 3
                      m_pCMoteurBase->m_DOSS_IDENT_TEL2             + ", " +       // 4
                      m_pCMoteurBase->m_DOSS_IDENT_TEL3             + ", " +       // 5
                      m_pCMoteurBase->m_DOSS_IDENT_EMAIL            + ", " +       // 6
                      m_pCMoteurBase->m_DOSS_IDENT_CODE_POST        + ", " +       // 7
                      m_pCMoteurBase->m_DOSS_IDENT_VILLE            + ", " +       // 8
                      m_pCMoteurBase->m_DOSS_IDENT_SEX              + ", " +       // 9
                      m_pCMoteurBase->m_DOSS_IDENT_GUID             + ", " +       // 10
                      m_pCMoteurBase->m_DOSS_IDENT_JFNOM            + ", " +       // 11
                      m_pCMoteurBase->m_DOSS_IDENT_TITRE            + ", " +       // 12
                      m_pCMoteurBase->m_DOSS_IDENT_PROF             + ", " +       // 13
                      m_pCMoteurBase->m_DOSS_IDENT_NSS              + ", " +       // 14
                      m_pCMoteurBase->m_DOSS_IDENT_NUMSS            + ", " +       // 15
                      m_pCMoteurBase->m_DOSS_IDENT_IS_ASS           + ", " +       // 16
                      m_pCMoteurBase->m_DOSS_IDENT_NSS_ASS          + ", " +       // 17
                      m_pCMoteurBase->m_DOSS_IDENT_GEME             + " FROM "  +  // 18
                      m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME         + " WHERE " +
                      m_pCMoteurBase->m_DOSS_IDENT_REF_PK           + " = '"    + refToPrimkeyDoss + "';";

   QSqlQuery query( requete  , m_pCMoteurBase->getDatabase() );                     // ouvrir un curseur sur la table
   if ( query.isActive() && query.next())
      {
       m_pGUI->lineEdit_NomAssure->setText(       CGestIni::Utf8_Query(query , 0)      );
       m_pGUI->lineEdit_PrenomAssure->setText(    CGestIni::Utf8_Query(query , 1)      );
       m_pGUI->lineEdit_NomDossier->setText(nom);
       m_pGUI->lineEdit_PrenomDossier->setText(prenom);
       m_pGUI->textEdit_Adresse->setText(         CGestIni::Utf8_Query(query , 2)      );
       m_pGUI->lineEdit_Tel1->setText(            ToNum(query.value( 3).toString())    );
       m_pGUI->lineEdit_Tel2->setText(            ToNum(query.value( 4).toString())    );
       m_pGUI->lineEdit_Tel3->setText(            ToNum(query.value( 5).toString())    );
       m_pGUI->lineEdit_Email->setText(           CGestIni::Utf8_Query(query , 6)      );
       QString cd = ToNum(query.value( 7).toString());
       if (cd.toInt()==0) cd =""; if (cd.length()==4)cd.prepend("0");  // 6000 --> 06000
       m_pGUI->lineEdit_CdPostal->setText(        cd    );
       m_pGUI->lineEditVille->setText(            CGestIni::Utf8_Query(query , 8)      );

       QString sexe                       =    CGestIni::Utf8_Query(query , 9)    ;
       if       (sexe.left(1).lower() == "m")  m_pGUI->comboBoxSexe->setCurrentItem ( 0 ) ;
       else  if (sexe.left(1).lower() == "f")  m_pGUI->comboBoxSexe->setCurrentItem ( 1 ) ;
       else                                    m_pGUI->comboBoxSexe->setCurrentItem ( 2 ) ;
       m_pGUI->lineEdit_RangGeme->setText(        query.value( 18).toString()          );

       if (getInterfaceMode() != MODE_CREATION_PATIENT)  m_pGUI->textLabel_NumDoss->setText(        CGestIni::Utf8_Query(query , 10)     );
       m_pGUI->lineEdit_NomDeNss->setText(        CGestIni::Utf8_Query(query , 11)     );
       m_pGUI->lineEdit_Titre->setText(           CGestIni::Utf8_Query(query , 12)     );
       m_pGUI->lineEdit_Profession->setText(      CGestIni::Utf8_Query(query , 13)     );

       //....................... trucs speciaux ........................................
       QDate qDteNss =                  query.value( 14).toDate();    // dnss patient
       m_pGUI->lineEdit_DtNss->setText(CHtmlTools::yyyyMMdd_to_ddMMyyyy(query.value( 14).toString() ,"-"));
       if (qDteNss.isValid()) m_pGUI->groupBoxPatient->setTitle(tr("Patient (b\303\251n\303\251ficiaire) \303\242g\303\251 de : ") + m_pCMoteurBase->CalculeDiffAge(&qDteNss, 0, TRUE, 0));
       else                   m_pGUI->groupBoxPatient->setTitle(tr("Patient (b\303\251n\303\251ficiaire) avec une date de naissance non valide"));

       qDteNss =                  query.value( 17).toDate();          // dnss assure
       m_pGUI->lineEdit_DtNss_Assure->setText(CHtmlTools::yyyyMMdd_to_ddMMyyyy(query.value( 17).toString() ,"-"));
       //if ( !qDteNss.isValid()) m_pGUI->lineEdit_DtNss_Assure->setText(qDteNss.toString(QString("ddMMyyyy")));
       //else                     m_pGUI->lineEdit_DtNss_Assure->setText("");
       QString   nss =                  query.value( 15).toString();
       m_pGUI->lineEdit_NumSecu->setText( nss.left(13)  );
       m_pGUI->lineEdit_NssClef->setText( nss.mid(13)   );
       //.............. qualite assure ..........................
       int qualiteAssure = query.value( 16).toInt()-1;   // 0 l'assur\303\251 est le patient  (1 moins 1)
       if (qualiteAssure < 0) qualiteAssure = 9;
       if (qualiteAssure > 9) qualiteAssure = 9;
       m_pGUI->comboBoxQualiteAyantDroit->setCurrentItem(qualiteAssure);
       tryToSetInterfaceFromAyantDroit(qualiteAssure, 0);
       m_pCMoteurBase->CloseBase();
       setIdentModifiedState(FALSE);
       Controle_Solde_Patient(query.value( 10).toString());
       //QString oldTitre = m_pGUI->lineEdit_Titre->text();
       //tryToSetTitreFromDateNss();
       //if (m_pGUI->lineEdit_Titre->text() != oldTitre)  setIdentModifiedState(TRUE);
       ret = 1;
   }
else
   {   ret = 0;
   }
   //............................... recuperer les notes ............................................
   QString data;
   m_pCMoteurBase->PatientNoteGetData(refToPrimkeyDoss ,  data);
   m_pGUI->textEdit_Note->setText(data);
   clearDroitsFields();

   connect( m_pGUI->lineEdit_NomDossier,         SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomDossier(const QString & )    ));
   connect( m_pGUI->lineEdit_PrenomDossier,      SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_PrenomDossier(const QString & ) ));
   connect( m_pGUI->lineEdit_NomDeNss,           SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomDeNss(const QString & )      ));
   connect( m_pGUI->lineEdit_DtNss,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_DtNss(const QString & )         ));
   connect( m_pGUI->lineEdit_DtNss_Assure,       SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_DtNss_Assure(const QString &)   ));
   connect( m_pGUI->lineEdit_Titre,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Titre(const QString & )         ));
   connect( m_pGUI->lineEdit_Profession,         SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Profession(const QString &)     ));
   connect( m_pGUI->textEdit_Adresse,            SIGNAL( textChanged ( ) ),                        this, SLOT(   Slot_TextChanged_textEdit_Adresse( ))  );
   connect( m_pGUI->lineEdit_Tel1,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel1(const QString & ))  );
   connect( m_pGUI->lineEdit_Tel2,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel2(const QString & ))  );
   connect( m_pGUI->lineEdit_Tel3,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Tel3(const QString & ))  );
   connect( m_pGUI->lineEdit_CdPostal,           SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_CdPostal(const QString & ))  );
   connect( m_pGUI->lineEditVille,               SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEditVille(const QString & )) );
   connect( m_pGUI->lineEdit_Email,              SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_Email(const QString & )) );
   connect( m_pGUI->lineEdit_NumSecu,            SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NumSecu(const QString & )) );
   connect( m_pGUI->lineEdit_NomAssure,          SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_NomAssure(const QString & ))  );
   connect( m_pGUI->lineEdit_PrenomAssure,       SIGNAL( textChanged ( const QString & ) ),        this, SLOT(   Slot_TextChanged_lineEdit_PrenomAssure(const QString & ))  );
   connect( m_pGUI->textEdit_Note,               SIGNAL( textChanged ( ) ),                        this, SLOT(   Slot_TextChanged_textEdit_Note( )) );
   connect( m_pGUI->lineEdit_RangGeme,           SIGNAL( textChanged (const QString & ) ),         this, SLOT(   Slot_TextChanged_lineEdit_RangGeme(const QString &)) );
   connect( m_pGUI->comboBoxSexe,                SIGNAL( currentIndexChanged (const QString & ) ), this, SLOT(   Slot_TextChanged_comboBoxSexe( const QString &))  );
   return ret;
}

//--------------------------------- ToNum ---------------------------------------------------------------------------------
QString C_Manager::ToNum(QString in)
{for(int i=0; i<in.length(); )
   {if (in.at(i)<'0' || in.at(i)>'9') in.remove(i,1);
       else ++i;
   }
   return in;
}

//--------------------------------- initListePatient ---------------------------------------------------------------------------
void C_Manager::initListePatient( const QString & qstr_nom, const QString & qstr_prenom)
{   m_pCMoteurBase->GetPatientList(m_pGUI->listView_Patient,
                                           qstr_nom,
                                           qstr_prenom,
                                           m_pGUI->textLabel_StateList
                                          );
   m_pGUI->listView_Patient->sortItems (0, Qt::AscendingOrder );
   //m_pLastQListViewItem = 0;
}
//--------------------------------- Slot_lineEditAutoLocator_textChanged -------------------------------------------------------------
void C_Manager::Slot_lineEditAutoLocator_textChanged(const QString &)
{   QStringList lst = m_pGUI->lineEditAutoLocator->text().split(';',QString::KeepEmptyParts);
   //.................. rechercher le separateur de nom ; prenom .......................
   if (lst.count()==1) initListePatient( lst[0], "");
   else                initListePatient( lst[0], lst[1]);
}

//--------------------------------- Slot_pushButtonDetruire_clicked -------------------------------------------------------------
void C_Manager::Slot_pushButtonDetruire_clicked()
{   if ( !G_pCApp->isCurrentIdentiteValid() )
      {G_pCApp->CouCou(tr("Operation impossible: \nno valid file selected."));
       return;
      }
   QString nom       = G_pCApp->identitePatientNom() ;
   QString prenom    = G_pCApp->identitePatientPrenom();
   if (G_pCApp->IsThisDroitExist("idd"))
      {QMessageBox::warning ( this, tr("Patient's file removal:"),
                              tr ( " Your rights are insufficient do delete the file:")+ "<br><b> <font color=\"#e80d0d\"> "     +
                              nom  + " " + prenom  +  "</b></font>",
                              tr("OK"));
      return;
      }
   QTreeWidgetItem *pQListViewItem =  getSelectedListViewPatientItem();
   if ( pQListViewItem == 0)                 return;

   QString numGUID   = G_pCApp->identitePatientGUID();
   QString iD_Doss   = G_pCApp->identitePatientPk();

   //............. verifier si le dossier n'est pas deja verrouille ..................................
   //              car utilise par quelqu'un d'autre
   QString  usedBy;
   if (m_pCMoteurBase->VerrouilleDossier (numGUID, G_pCApp->getUser() , usedBy  )==0)
      {QMessageBox::warning ( this, tr("Patient's file removal:"),
                           tr ( " WARNING :\n      file removal impossible : \n      ")     +
                           nom  + " " + prenom  +
                           tr(  "\n      is used by: ") + usedBy ,
                           tr("OK"), 0, 0,
                           1, 1
                           );
       return;
      }

   //............. noter la nouvelle identit\303\251 ..................................
   switch  (QMessageBox::warning ( this, tr("Patient's file removal:"),
                                   tr ( "<u><b> WARNING !</u></b><br />      YOU ARE ABOUT TO DELETE A FILE: ")+"<br />      <font color=\"#e80d0d\"><b>"+
                                   nom  + " " +prenom  +
                                   tr(  "</b></font><br>&nbsp;&nbsp;&nbsp;THIS ACTION CANNOT BE UNDONE<br />&nbsp;&nbsp;&nbsp;AND DATA WILL BE&nbsp;&nbsp;&nbsp;<u><b>UNRECOVERABLE</b></u> LATER") ,
                                   tr("&Delete"), tr("&NO"), 0,
                                   1, 1 )
           )
           {case 1: m_pCMoteurBase->DeVerrouilleDossier (numGUID);
            return ;
           }
   //.................. effacer la note ...................................
   QString pk = m_pCMoteurBase->PatientNoteIsThisExist(iD_Doss);
   m_pCMoteurBase->PatientNoteDelete(pk,                       G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0 );
   m_pCMoteurBase->PatientIntervenantsAllDelete( iD_Doss);
   //.................. effacer le dossier ...................................
   RUBREC_LIST rubList;
   m_pCMoteurBase->initRubriquesList(&rubList, numGUID);     // lire et reinitialiser la liste des rubriques
   m_pCMoteurBase->DossIdentDelete(&rubList, numGUID, iD_Doss, G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0);
   //................... liberer l'ancien verrou ...............................
   m_pCMoteurBase->DeVerrouilleDossier (numGUID);

   //.............. ya plus de dossier en cours ...................................................
   G_pCApp->clearCurrentIdentite();

   //............. effacer l'item correspondant dans la liste .........................
   //              et selectionner le patient suivant .................................
   QList <QTreeWidgetItem *> list = m_pGUI->listView_Patient->findItems ( iD_Doss, Qt::MatchExactly, LIST_PK_PATIENT );
   if (list.count())
      {//............ on recupere l'item \303\240 effacer ..................................
       QTreeWidgetItem *qlistViewItem  =  list[0];  // on recupere l'item \303\240 effacer
       //............ on recupere le prochain ou si pas le precedent ................
       //             (pour eventuellement l'activer)
       QTreeWidgetItem           *newQListViewItem = m_pGUI->listView_Patient->itemBelow(qlistViewItem);
       if (newQListViewItem==0)   newQListViewItem = m_pGUI->listView_Patient->itemAbove(qlistViewItem);
       //............ on efface de la liste .........................................
       delete qlistViewItem;
       //............. on reactive le suivant ou precedent ..........................
       m_pGUI->listView_Patient->clearSelection ();
       if (newQListViewItem!=0)
          {newQListViewItem->setSelected ( TRUE );
           m_pGUI->listView_Patient->scrollToItem ( newQListViewItem );
          }
       else
          {setIdentModifiedState(FALSE); // car pas d'identite selectionne
           setIdentiteDisabled();        // car pas d'identite selectionne
           setVerrou(FALSE);             // car pas d'identite ni lancee
          }
      }
   compareListVitaleWithBase();
}
///////////////////////////////////////// GESTION DES DOUBLONS  ////////////////////////////////////////
//--------------------------------- Slot_listView_Doublons_contextMenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_listView_Doublons_contextMenuRequested( const QPoint & )
{Slot_listView_Doublons_MenuRequested();
}
//--------------------------------- Slot_listView_DoublonsSelectionChanged -----------------------------------------------------------------------
void C_Manager::Slot_listView_DoublonsSelectionChanged()
{QTreeWidgetItem *pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Doublons);
if (pQListViewItem) DataToFormulaire( pQListViewItem->text(LIST_PK_PATIENT), pQListViewItem->text(LIST_NOM), pQListViewItem->text(LIST_PRENOM), pQListViewItem->text(LIST_GUID));
}

//--------------------------------- Slot_listView_Doublons_MenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_listView_Doublons_MenuRequested()
{QTreeWidgetItem * pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Doublons);
QStringList list;
list.append(tr("=1=Close the duplicates resolution list"));
list.append(tr("=2=Delete the duplicates resolution list"));
if (pQListViewItem)
   {list.append(tr("=3=Remove this file from duplicates list: ")      + pQListViewItem->text(0)+" "+pQListViewItem->text(1));
    list.append(tr("=4=Test if the files are locked and usable"));
    list.append("------");
    list.append(tr("=5=Give all the files in this list to patient: ") + pQListViewItem->text(0)+" "+pQListViewItem->text(1));
   }

QString usedBy = "";
bool isVerrou  = FALSE;
int option     = G_pCApp->DoPopupList(list).toInt();
switch(option)
   {
    case 2:
     {m_pGUI->listView_Doublons->clear ();
      m_pGUI->listView_Doublons->hide();
      break;
     }
    case 3:
     {delete(pQListViewItem);
      break;
     }
    case 1:
     {m_pGUI->listView_Doublons->hide();
      break;
     }
    case 4:
      {//.............. verifier si verrous .................................
       QTreeWidgetItemIterator it( m_pGUI->listView_Doublons );
       while ( *it )
          {QTreeWidgetItem *pQListViewItem_D = (*it);
           if (m_pCMoteurBase->IsDossierVerrouille   ( pQListViewItem_D->text(3), usedBy ))
              {pQListViewItem_D->setIcon ( 0, QIcon(Theme::getIcon( "16x16/locked.png")) );
               pQListViewItem_D->setText ( 4, usedBy );
               isVerrou = TRUE;
              }
           else
              {pQListViewItem_D->setIcon ( 0, QIcon() );
               pQListViewItem_D->setText ( 4, "" );
              }
           ++it;
          }
       break;
      }
    case 5:
      {if (m_pGUI->listView_Doublons->topLevelItemCount() <=1) return;
       if ( (G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0)==CMoteurBase::progMasterUpdate)
          {G_pCApp->CouCou(tr("This itinerant mode does not allow duplicates resolution"));
           return;
          }
       QString message  = "<b><font color=\"#e80d0d\">"+tr ("WARNING ! MERGING FUNCTION AT HIGH RISK ! </font></b><hr><br> Person :")+" <b><font color=\"#1200ff\">" + pQListViewItem->text(0)+" "+pQListViewItem->text(1) + "</font></b>" +
                          tr(  "<br /> will be the one who will get all the data: <hr>" ) ;
       QTreeWidgetItemIterator ut( m_pGUI->listView_Doublons );
       while ( *ut )
              {QTreeWidgetItem *pQListViewItem_D = (*ut);
               if (pQListViewItem_D->text(2) != pQListViewItem->text(2))
                   message += tr(  "<br> &nbsp;&nbsp;&nbsp;&nbsp;=> File:")+ "<b><font color=\"#1200ff\">" + pQListViewItem_D->text(0)+" "+pQListViewItem_D->text(1) + "</font></b>";
               ++ut;
              }
               message += tr("<br><hr>Do you confirm this operation ? ");
       int sel = QMessageBox::information( this, tr("Merge several files"), message,
                                                 tr("&Cancel"), tr("&Merge files"), 0 /*tr("Annuler")*/,
                                                 0, 1 );
       if (sel != 1)                             return;
       message  = "<b><font color=\"#e80d0d\">"+tr("Do you confirm this merging")+ "</font></b>" +
                  tr(  "<hr><br /> You can still cancel this operation.") +
                  tr(  "<br /> which will be irreversible after confirmation." ) +
                  tr(  "<br /><hr>Are you sure ? ");
       sel = QMessageBox::information( this, tr("Merge several files"), message,
                                             tr("&Merge files"),tr("&Cancel") , 0 /*tr("Annuler")*/,
                                             1, 1 );
       if (sel == 1)                         return;
       //.............. verifier si verrous .................................
       QTreeWidgetItemIterator it( m_pGUI->listView_Doublons );
       while ( *it )
          {QTreeWidgetItem *pQListViewItem_D = (*it);
           if (m_pCMoteurBase->IsDossierVerrouille   ( pQListViewItem_D->text(3), usedBy ))
              {pQListViewItem_D->setIcon ( 0, QIcon(Theme::getIcon( "16x16/locked.png")) );
               pQListViewItem_D->setText ( 4, usedBy );
               isVerrou = TRUE;
              }
           else
              {pQListViewItem_D->setIcon ( 0, QIcon() );
               pQListViewItem_D->setText ( 4, "" );
              }
           ++it;
          }
       if (isVerrou==FALSE)
          {//............ poser les verrous .....................
           QTreeWidgetItemIterator it( m_pGUI->listView_Doublons );
           while ( *it )
               {QTreeWidgetItem *pQListViewItem_D = (*it);
                 m_pCMoteurBase->VerrouilleDossier ( pQListViewItem_D->text(3), G_pCApp->m_User , usedBy );
                 ++it;
                }
           m_pCMoteurBase->ResoudreDoublons(pQListViewItem, m_pGUI->listView_Doublons, G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0);
           m_pCMoteurBase->DeVerrouilleDossier ( pQListViewItem->text(3));
           QString nom    = pQListViewItem->text(0);
           QString prenom = pQListViewItem->text(1);
           m_pGUI->listView_Doublons->clear () ;
           initListePatient( nom, prenom);
          }
       break;
       }
   }
}

//////////////////////////////////////// ENREGISTREMENT CREATION MISE A JOUR //////////////////////////////////////////////////////
//--------------------------------- Slot_pushButtonSavModifWithoutConfirmation() -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonSavModifWithoutConfirmation()
{savModifications(G_pCApp->identitePatientNom(), G_pCApp->identitePatientPrenom(), G_pCApp->identitePatientPk(),G_pCApp->identitePatientGUID(), 0);
}
//--------------------------------- Slot_pushButtonSavModif -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonSavModif()
{savModifications(G_pCApp->identitePatientNom(), G_pCApp->identitePatientPrenom(), G_pCApp->identitePatientPk(),G_pCApp->identitePatientGUID());
}

//--------------------------------- sexControl -------------------------------------------------------------
bool C_Manager::sexControl()
{   if ( !m_SexControl) return true;
    int ret = -1;
    QString sexe;
    if       (m_pGUI->comboBoxSexe->currentItem() == 0)  sexe = "M";
    else  if (m_pGUI->comboBoxSexe->currentItem() == 1)  sexe = "F";
    else                                                 sexe = "I";
    if (sexe == "I")
       {/*
        ret = QMessageBox::warning ( this, tr("patient sexe:"),
                                     tr ( "<u><b>WARNING :</u> it is very important to give a good sex</b><br/>"
                                          "&nbsp;&nbsp;&nbsp;The sex of this patient is indetermined.<br/>"
                                          "&nbsp;&nbsp;&nbsp;please choose the correct sex for this patient."),
                                     tr("Male"), tr("Female"), 0,
                                     1, 1
                                   );
        */
        ret = QMessageBox::warning ( this, tr("Sexe du patient :"),
                                             tr ( "<u><b>ATTENTION :</u> Il est important de bien saisir le sexe</b><br/>"
                                                  "&nbsp;&nbsp;&nbsp;Le sexe de ce patient est ind\303\251termin\303\251.<br/>"
                                                  "&nbsp;&nbsp;&nbsp;Veuillez bien rentrer le sexe de ce patient."),
                                             tr("Masculin"), tr("F\303\251minin"), 0,
                                             1, 1
                                           );
        if (ret==0) m_pGUI->comboBoxSexe->setCurrentItem ( 0 );
        else        m_pGUI->comboBoxSexe->setCurrentItem ( 1 );
       }
  return true;
}
//--------------------------------- nameControl -------------------------------------------------------------
bool C_Manager::nameControl()
{   if (m_HasLapCompatibility==0) return true;
    //.................. date vide ..........................
    QString name = m_pGUI->lineEdit_NomDossier->text().trimmed();
    if (name.length()==0)
       {QMessageBox::warning ( this, tr("patient name:"),
                               tr ( "<u><b>WARNING :</u></b><br/>"
                                    "&nbsp;&nbsp;&nbsp;This Operation not possible because patient name is empty.<br/>"
                                    "&nbsp;&nbsp;&nbsp;Please give a correct patient name.<br/>"
                                    "&nbsp;&nbsp;&nbsp;If you don't know it <b>XXX</b> or <b>YYY</b> is a good name"),
                               tr("&Cancel"), 0, 0,
                               1, 1
                             );
         return false;
       }
   return true;
}

//--------------------------------- dateOfBirthControl -------------------------------------------------------------
bool C_Manager::dateOfBirthControl()
{
    if (m_HasLapCompatibility==0) return true;
    int           ret          = -1;
    int           mois         = 0;
    int           jours        = 0;
    QString       dnss         = m_pGUI->lineEdit_DtNss->text();
    QStringList   dnssList     = dnss.split('-');
    dnss.remove('-');
    QDate dt;
    //.................. date vide ..........................
    if (dnss.length()==0)
       {QMessageBox::warning ( this, tr("date of birth:"),
                               tr ( "<u><b>WARNING :</u></b><br/>"
                                    "&nbsp;&nbsp;&nbsp;This Operation not possible because date of birth is empty.<br/>"
                                    "&nbsp;&nbsp;&nbsp;Please give a correct date of birth or approximative age"),
                               tr("&Cancel"), 0, 0,
                               1, 1
                             );
         return false;
       }
    //.................. si premier chiffre est zero alors l'age peut etre exprime en mois-jours ..........................
    if      (dnssList[0].toInt()==0 && (dnssList[1].toInt()||dnssList[2].toInt()) )
            { mois  = dnssList[1].toInt();
              jours = dnssList[2].toInt();
            }
    //.................. completer le siecle ..........................
    if (dnssList[0].toInt() && dnss.length()==6 && dnssList[0].length()==2 && dnssList[1].length()==2 && dnssList[2].length()==2)
    {   QString dnss20 = dnssList[0]+"-"+dnssList[1]+"-20"+dnssList[2];
        QString dnss19 = dnssList[0]+"-"+dnssList[1]+"-19"+dnssList[2];
        dt  = QDate::fromString(dnss20,"dd-MM-yyyy");   // on part sur l'an 2000
        if (dt<QDate::currentDate())
            {ret = QMessageBox::warning ( this, tr("date of birth:"),
                               tr ( "<u><b>WARNING :</u></b><br/>"
                                    "&nbsp;&nbsp;&nbsp;The year is exprimed with only two numbers.<br/>"
                                    "&nbsp;&nbsp;&nbsp;The date can be <b>%1</b> or <b>%2</b> <br/>"
                                    "&nbsp;&nbsp;&nbsp;please choose the correct date of birth.").arg(dnss20,dnss19),
                               dnss20, dnss19, 0,
                               1, 1
                             );
            if (ret==0) dnss = dnss20;
            else        dnss = dnss19;
           }
        else
           {dnss = dnss19;
           }
        m_pGUI->lineEdit_DtNss->setText(dnss);
       }
    //.................. saisie avec l'age ..........................
    else if (dnss.length()<=4 || jours|| mois)
       { dt       = QDate::currentDate();
         if (dnssList[0].toInt())  dt  = dt.addYears( -dnssList[0].toInt());
         if (dnssList[1].toInt())  dt  = dt.addMonths(-dnssList[1].toInt());
         if (dnssList[2].toInt())  dt  = dt.addDays(  -dnssList[2].toInt());

         if (dt==QDate::currentDate())
            {QMessageBox::warning ( this, tr("date of birth:"),
                                    tr ( "<u><b>WARNING :</u></b><br/>"
                                         "&nbsp;&nbsp;&nbsp;This Operation not possible because age is invalid.<br/>"
                                         "&nbsp;&nbsp;&nbsp;Please give a correct date of birth or approximative age"),
                                    tr("&Cancel"), 0, 0,
                                    1, 1
                                  );
              m_pGUI->lineEdit_DtNss->clear();
              return false;
            }
         dnss     = dt.toString("dd-MM-yyyy");
         ret      = QMessageBox::warning ( this, tr("date of birth:"),
                                        tr ( "<u><b>WARNING :</u> (date of birth from age)</b><br/>"
                                             "&nbsp;&nbsp;&nbsp;This date of birth <b>%1</b> seems to be get from this age <b>%2</b>.<br/>"
                                             "&nbsp;&nbsp;&nbsp;Do you want this ?.").arg(dnss, m_pCMoteurBase->CalculeDiffAge(&dt, 0, TRUE, 0)),
                                        tr("&Not good"), tr("&I want this"), 0,
                                        1, 1
                                      );
         if (ret==0) {m_pGUI->lineEdit_DtNss->clear(); return false;}
         m_pGUI->lineEdit_DtNss->setText(dnss);
       }
    dt  = QDate::fromString(dnss.remove('-'),"ddMMyyyy");
    if (!dt.isValid())
       {QMessageBox::warning ( this, tr("date of birth:"),
                               tr ( "<u><b>WARNING :</u></b><br/>"
                                    "&nbsp;&nbsp;&nbsp;This Operation not possible because date of birth is invalid.<br/>"
                                    "&nbsp;&nbsp;&nbsp;Please give a correct date of birth or approximative age"),
                               tr("&Cancel"), 0, 0,
                               1, 1
                             );
         m_pGUI->lineEdit_DtNss->clear();
         return false;
       }
    return true;
}

//--------------------------------- savModifications -------------------------------------------------------------
void C_Manager::savModifications(const QString &oldNom, const QString &oldPrenom,  const QString &oldPk, const QString &oldGuid, int confirmModif /*=1*/)
{  if (G_pCApp->IsThisDroitExist("idc")==0)
      {G_pCApp->CouCou(tr( "Your rights are insufficient to save a folder."));
       return;
      }
   if ( !nameControl() )
      { m_pGUI->lineEdit_NomDossier->setFocus();
        m_pGUI->lineEdit_NomDossier->setCursorPosition ( 0 );
        return;
      }
   if ( !dateOfBirthControl() )
      {m_pGUI->lineEdit_DtNss->setFocus();
       m_pGUI->lineEdit_DtNss->setCursorPosition ( 0 );
       return;
      }
   if ( !sexControl() )
      {return;
      }

   int     ret       = 0;
   QString nom       = m_pGUI->lineEdit_NomDossier->text().stripWhiteSpace () ;
   QString prenom    = m_pGUI->lineEdit_PrenomDossier->text().stripWhiteSpace () ;
   QString numGUID   = G_pCApp->identitePatientGUID();

   if (oldNom != nom || oldPrenom != prenom)
   { ret   = QMessageBox::warning ( this, tr("Saving:"),
                                    tr( "<u><b>WARNING:</u></b><br />&nbsp;&nbsp;&nbsp;Are you sure this file:") +"<br>&nbsp;&nbsp; <font color=\"#e80d0d\"><b>"     +
                                    oldNom  + " " + oldPrenom  +
                                    tr(  "</b></font>&nbsp;&nbsp;must be modified with these new family name and first name:")+ "<font color=\"#1111b0\"><b>" +
                                    nom + " " + prenom + tr("</b></font>&nbsp;&nbsp;(don't you want to create a new file ?)"),
                                    tr("&Modify"), tr("&Cancel"), 0,
                                    1, 1
                                    );
   }
   else
   {if (confirmModif)
       {
        ret   = QMessageBox::warning ( this, tr("Saving:"),
                                       tr ( "<u><b>WARNING :</u></b><br>&nbsp;&nbsp;&nbsp;do you confirm the modifications&nbsp;&nbsp;on this file: ")+"<br>&nbsp;&nbsp;<font color=\"#e80d0d\"><b>"     +
                                       oldNom  + " " + oldPrenom  + "</b></font>",
                                       tr("&Modify"), tr("&Cancel"), 0,
                                       1, 1
                                       );
       }
    else
       {ret = 0;
       }
   }
   if (ret == 1)
      {//ListView_Patient_clicked( ListView_Patient->currentItem ());    // rafraichir l'ancien dossier
       DataToFormulaire(oldPk, oldNom, oldPrenom, oldGuid);
       return;
      }

   QString  usedBy;
   if (m_pCMoteurBase->VerrouilleDossier (numGUID, G_pCApp->getUser() , usedBy  )==0)
      {QMessageBox::warning ( this, tr("Saving:"),
                           tr ( " WARNING :\n      Impossible to save the file: \n      ")     +
                           nom  + " " +prenom  +
                           tr(  "\n    because it is in use by: ") + usedBy ,
                           tr("&Validate"), 0, 0,
                           1, 1
                           );
       return;
      }
   if (nom == "")
      {nom = tr("ANONYMOUS");
       m_pGUI->lineEdit_NomDossier->setText(nom);
      }
   PtrListIdentFieldData list_field;   //  cr\303\251er la liste: nom champs <-> valeurs
   setFieldListData(list_field);
   nom             = nom.upper();
   prenom          = prenom.upper();
   QString iD_Doss = m_pCMoteurBase->GetDossPrimKeyFromGUID(numGUID);
   m_pCMoteurBase->DossIdentIndexUpdate(iD_Doss, nom, prenom, &list_field ,           G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0);
   m_pCMoteurBase->PatientNoteWrite(iD_Doss ,numGUID,  m_pGUI->textEdit_Note->text(), G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0);
   m_pCMoteurBase->DeVerrouilleDossier (numGUID);

   //............. mettre \303\240 jour la liste au cas ou le nom change .................................
   QList <QTreeWidgetItem *> list = m_pGUI->listView_Patient->findItems ( iD_Doss, Qt::MatchExactly, LIST_PK_PATIENT );
   if (list.count())
      {//............ on recupere l'item \303\240 effacer ..................................
       QTreeWidgetItem *qlistViewItem  =  list[0];  // on recupere l'item \303\240 effacer
       if (qlistViewItem)
          {   qlistViewItem->setText(LIST_NOM , nom);
              qlistViewItem->setText(LIST_PRENOM ,prenom);
          }
      }
   list = m_pGUI->listView_Doublons->findItems ( iD_Doss, Qt::MatchExactly, LIST_PK_PATIENT );
   if (list.count())
      {QTreeWidgetItem *qlistViewItem  =  list[0];  // on recupere l'item \303\240 effacer
        if (qlistViewItem)
           {   qlistViewItem->setText(LIST_NOM , nom);
               qlistViewItem->setText(LIST_PRENOM ,prenom);
           }
      }
   //................... cela peut avoir change dans le formulaire ....................................
   G_pCApp->m_LastNom                   = nom;
   G_pCApp->m_LastPrenom                = prenom;
   compareListVitaleWithBase();
   setIdentModifiedState(FALSE);
}
//--------------------------------- Slot_pushButtonCreate -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonCreate()
{onCreateAction(1);
}

//--------------------------------- onCreateAction -----------------------------------------------------------------------
void C_Manager::onCreateAction(int lauchEnabled  /*  = 1 */)
{   QString nom        = m_pGUI->lineEdit_NomDossier->text().trimmed () ;
    QString prenom     = m_pGUI->lineEdit_PrenomDossier->text().trimmed () ;
    QString numGUID    = m_pGUI->textLabel_NumDoss->text();

    if ( !nameControl() )
       { m_pGUI->lineEdit_NomDossier->setFocus();
         m_pGUI->lineEdit_NomDossier->setCursorPosition ( 0 );
         return;
       }
    if ( !dateOfBirthControl() )
       { m_pGUI->lineEdit_DtNss->setFocus();
         m_pGUI->lineEdit_DtNss->setCursorPosition ( 0 );
         return;
       }
    if ( !sexControl() )
       { return;
       }

    if (nom == "") nom = tr("ANONYMOUS");
    //.............. rechercher les doublons ....................
    QString   dnss     = m_pGUI->lineEdit_DtNss->text().remove('-');                      //  05061956
    if (dnss.length()==8) dnss = dnss.mid(4,4)+"-"+dnss.mid(2,2)+"-"+dnss.mid(0,2)+" 00:00:00";
    else                  dnss = "";
    QString numSecu  = m_pGUI->lineEdit_NumSecu->text().remove(' ');
    if (numSecu.length()==13)     numSecu += CMoteurBase::ComputeClef_Secu(numSecu).remove(' ');
    else if (numSecu.length()==0) numSecu  = "";
    else                          numSecu += "%";

   int nb = G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme( m_pGUI->listView_Patient,
                                                                               nom,                         // nom
                                                                               prenom,                      // prenom
                                                                               dnss,                        // date Nss // doit etre 1993-06-01 00:00:00
                                                                               numSecu,              // num secu // 160196296519557
                                                                             "",                     // rang gemellaire
                                                                             "",     // qualite d'ayant droit
                                                                             "",      //  adresse,
                                                                             "",      //  codePost,
                                                                             "",      //  ville,
                                                                             "",      //  sexe,
                                                                             "",      //  profession,
                                                                             "",      //  nomJfille
                                                                             "",      //  nomAss,
                                                                             "",      //  prenomAss,
                                                                             "",      //  tel1,
                                                                             "",      //  tel2,
                                                                             "",      //  tel3,
                                                                             "",      //  email,
                                                                             " = ",   //  egal_like  " = ",
                                                                             m_pGUI->textLabel_StateList
                                                                           );
   if (nb)
      {      QString  message  = "<hr><b><font color=\"#e80d0d\">"+tr(  "POSSIBLE DUPLICATES ON THESE ELEMENTS")+"</font></b><br /><hr> "+tr("family name first name:")+ "<b><font color=\"#1200ff\">" + nom + " " +   prenom         + "</font></b>";
              if (dnss.length())
                      message += tr(  "<br /> birth date:")+ "<b><font color=\"#1200ff\">"          + dnss                         + "</font></b>";
              if (m_pGUI->lineEdit_CdPostal->text().remove(' ').toInt() !=0)
                      message += tr(  "<br /> postal code:")+" <b><font color=\"#1200ff\">"     + m_pGUI->lineEdit_CdPostal->text()    + "</font></b>";
              if (numSecu.length())
                      message += tr(  "<br /> num\303\251ro de SS : ")+"<b><font color=\"#1200ff\">"     + numSecu                      + "</font></b><br />";
                      message += tr(  "<br /><hr> of a person already in the files list");
                      message += tr(  "Do you still confirm this operation risking to create a duplicate ?");
             int sel = QMessageBox::information( this, tr("Possible duplicates"), message,
                                                 //"&nbsp;&nbsp;&nbsp;&nbsp; ? <b>Quitter pour <u>Annuler</u></b> l'op\303\251ration en cours" ) ,
                                            tr("&Cancel"), tr("Confirm"), 0 /*tr("Annuler")*/,
                                            0, 1 );
              if (sel != 1)
                 {setInterfaceOnProfil(MODE_SELECTION_PATIENT);
                  return;
                 }
      }
   else if ( (nb    = G_pCApp->m_pCMoteurBase->GetPatientListFromNomPrenomSecuDtNssGeme( m_pGUI->listView_Patient,
                                                                               nom,                         // nom
                                                                               prenom,                      // prenom
                                                                               "",                        // date Nss // doit etre 1993-06-01 00:00:00
                                                                               "",              // num secu // 160196296519557
                                                                               "",                     // rang gemellaire
                                                                               "",     // qualite d'ayant droit
                                                                               "",      //  adresse,
                                                                               "",      //  codePost,
                                                                               "",      //  ville,
                                                                               "",      //  sexe,
                                                                               "",      //  profession,
                                                                               "",      //  nomJfille
                                                                               "",      //  nomAss,
                                                                               "",      //  prenomAss,
                                                                               "",      //  tel1,
                                                                               "",      //  tel2,
                                                                               "",      //  tel3,
                                                                               "",      //  email,
                                                                               " = ",   //  egal_like  " = ",
                                                                               m_pGUI->textLabel_StateList
                                                                             )))
      {      QString  message  = QString(  "<hr><b><font color=\"#e80d0d\">"+tr("POSSIBLE DUPLICATES ON FAMILY NAME - FIRST NAME")+" </font></b>"             "<br /> "+tr("Person: ")+"<b><font color=\"#1200ff\">" + nom + " " +   prenom         + "</font></b>");
                      message += tr(  "<hr><br /> is similar to the name of a file in file list");
                      message += tr(  "<br /><hr>Do you confirm this operation which risks to create duplicates ?");
             int sel = QMessageBox::information( this, tr("Duplicates possible"), message,
                                                 //"&nbsp;&nbsp;&nbsp;&nbsp; ? <b>Quitter pour <u>Annuler</u></b> l'op\303\251ration en cours" ) ,
                                            tr("&Cancel"), tr("Confirm"), 0 /*tr("Annuler")*/,
                                            0, 1 );
              if (sel != 1)
                 {setInterfaceOnProfil(MODE_SELECTION_PATIENT);
                  return;
                 }
      }

   PtrListIdentFieldData list_field;    //  cr\303\251er la liste: nom champs <-> valeurs
   setFieldListData(list_field);

   QString pk_Doss = m_pCMoteurBase->DossIdentIndexCreate(nom, prenom, numGUID,  &list_field );
   if (pk_Doss != "")
   {   //.......................... ecrire la note ............................................
       //setVerrouForCurrentIdent();
       QString  usedBy;
       m_pCMoteurBase->VerrouilleDossier (numGUID, G_pCApp->getUser() , usedBy  );
       nom            = nom.upper();
       prenom         = prenom.upper();
       m_pCMoteurBase->PatientNoteWrite(pk_Doss ,numGUID,  m_pGUI->textEdit_Note->text(), G_pCApp->m_IsNomadeActif?G_pCApp->m_IsGestionNomadisme:0);
       //...................... ecrire les intervenants ................................................................
       QTreeWidgetItemIterator it(m_pGUI->listView_Praticiens);
       while (*it)
             { QTreeWidgetItem *pQListViewItem = (*it);
               m_pCMoteurBase->PatientIntervenantsWrite( pk_Doss , numGUID, pQListViewItem->text(POS_ID_INTERV),  pQListViewItem->text(POS_QUAL_INTERV));
               ++it;
             }
       //............. se remettre en mode modification .......................................
       m_pCMoteurBase->DeVerrouilleDossier (numGUID);
       setInterfaceOnProfil(MODE_SELECTION_PATIENT);
       //.............. creer un nouvel item correspondant au nouveau patient ...............
       //              comme on est en mode selection l'identite
       //              se repositionnera dessus
       QTreeWidgetItem * pQListViewItem = new QTreeWidgetItem();
       if (pQListViewItem)
          { pQListViewItem->setText(0,nom);
            pQListViewItem->setText(1,prenom);
            pQListViewItem->setText(2,pk_Doss);
            pQListViewItem->setText(3,numGUID);
            m_pGUI->listView_Patient->addTopLevelItem(pQListViewItem);
            m_pGUI->listView_Patient->scrollToItem(pQListViewItem);
            pQListViewItem->setSelected(TRUE);
            G_pCApp->setCurrentIdentite(pQListViewItem);
           }
   }
   //................. Patient List Mode = clear .........................................
   QString val;
   if (G_pCApp->readParam(  "Derniere Session", "Patient List Mode", &val)==QString::null)   // = zero si pas d'erreur
      {if (val.at(0).lower() == 'c')
          {m_pGUI->lineEditAutoLocator->setText("");
           m_pGUI->listView_Patient->clear();
           clearIdentFields();
          }
      }
   m_pGUI->lineEditAutoLocator->setFocus();
   m_pGUI->lineEditAutoLocator->selectAll ();
   compareListVitaleWithBase();
   m_pGUI->pushButton_FSE->setEnabled (TRUE );
   if (lauchEnabled && m_pGUI->pushButtonCreate->objectName() == "Acceder et creer" && m_Facture_Seule != "oui")  // Cz_Pyxvital
      {Slot_LauchPatient(numGUID);
      }

}
//--------------------------------- Slot_pushButtonNew_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonNew_clicked( )
{   //................... se mettre en mode creation en effa?ant les champs.................................
    // On revient en affichage FICHE, si on ?tait en AGENDA
    if (m_Type_Affichage_EnCours == "AGENDA")                  // CZA
        Slot_Type_Affichage_Change();                          // CZA

    //................... se mettre en mode creation en effa?ant les champs.................................
    if (identIsModified() && G_pCApp->isCurrentIdentiteValid() && G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idc") )
       { Slot_pushButtonSavModif();
       }
    clearIdentFields();
    Slot_NewDossWithoutErase();
}
//--------------------------------- Slot_NewDossWithoutErase -----------------------------------------------------------------------
void C_Manager::Slot_NewDossWithoutErase( )
{ //................... se mettre en mode creation sans effa?er les champs.................................
 setInterfaceOnProfil(MODE_CREATION_PATIENT);
 m_pGUI->textLabel_NumDoss->setText(m_pCMoteurBase->GUID_Create());   // TRES IMPORTANT SERA (si confirmation de la creation) LE GUID DU NOUVEAU DOSSIER
 m_pGUI->lineEdit_NomDossier->setFocus();
}
//--------------------------------- Slot_pushButtonCancelCreate_clicked -----------------------------------------------------------------------
void C_Manager::Slot_pushButtonCancelCreate_clicked()
{setInterfaceOnProfil(MODE_SELECTION_PATIENT);
m_pGUI->textLabel_NumDoss->setText("");
setIdentiteDisabled();        // car pas d'identite selectionne
setIdentModifiedState(FALSE); // pas de modif \303\240 enregistrer
}
//--------------------------------- clearIdentFields -----------------------------------------------------------------------
void C_Manager::clearIdentFields()
{  m_pGUI->lineEdit_NomAssure->clear();
   m_pGUI->lineEdit_PrenomAssure->clear();
   m_pGUI->lineEdit_NomDeNss->clear();
   m_pGUI->textEdit_Adresse->clear();
   m_pGUI->lineEdit_Tel1->clear();
   m_pGUI->lineEdit_Tel2->clear();
   m_pGUI->lineEdit_Tel3->clear();
   m_pGUI->lineEdit_CdPostal->clear();
   m_pGUI->lineEditVille->clear();
   if (getInterfaceMode() == MODE_SELECTION_PATIENT)  m_pGUI->textLabel_NumDoss->clear();
   m_pGUI->lineEdit_Email->clear();
   m_pGUI->lineEdit_NomDossier->clear();
   m_pGUI->lineEdit_PrenomDossier->clear();
   m_pGUI->lineEdit_Profession->clear();
   m_pGUI->lineEdit_DtNss->setText("");
   m_pGUI->lineEdit_Titre->setText("");
   m_pGUI->lineEdit_RangGeme->setText("");
   m_pGUI->lineEdit_NumSecu->clear();
   m_pGUI->lineEdit_NssClef->clear();
   m_pGUI->textEdit_Note->clear();
   m_pGUI->lineEdit_DtNss_Assure->clear();
   m_pGUI->listView_Praticiens->clear();
   clearDroitsFields();
   //m_pGUI->comboBoxSexe->setCurrentText ("");
}
//--------------------------------- clearDroitsFields ------------------------------------------------------------------------
void C_Manager::clearDroitsFields()
{  m_pGUI->lineEdit_DroitsDateDeb->setText("");
   m_pGUI->lineEdit_DroitsDateFin->setText("");
   m_pGUI->lineEdit_CodeRegime->setText("");
   m_pGUI->lineEdit_CaisseGest->setText("");
   m_pGUI->lineEdit_CentreGest->setText("");
   m_pGUI->lineEdit_CodeGest->setText("");
}
//--------------------------------- setFieldListData ------------------------------------------------------------------------
void C_Manager::setFieldListData(PtrListIdentFieldData &list_field)
{  list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS,    m_pGUI->lineEdit_NomAssure->text().trimmed(),          FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS, m_pGUI->lineEdit_PrenomAssure->text().trimmed(),       FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_JFNOM,      m_pGUI->lineEdit_NomDeNss->text().trimmed(),           FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TITRE,      m_pGUI->lineEdit_Titre->text().trimmed(),              FTN_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_RUE,        m_pGUI->textEdit_Adresse->text(),                      FTN_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TEL1,       m_pGUI->lineEdit_Tel1->text(),                         FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TEL2,       m_pGUI->lineEdit_Tel2->text(),                         FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TEL3,       m_pGUI->lineEdit_Tel3->text(),                         FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_EMAIL,      m_pGUI->lineEdit_Email->text(),                        FTN_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_CODE_POST,  m_pGUI->lineEdit_CdPostal->text().remove(' '),         FTN_INTEGER));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_VILLE,      m_pGUI->lineEditVille->text(),                         FTN_MAJ_STRING));
   QString sexe;
   if (m_pGUI->comboBoxSexe->currentItem() == 0)        sexe = "M";
   else  if (m_pGUI->comboBoxSexe->currentItem() == 1)  sexe = "F";
   else                                                 sexe = "I";
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_SEX,        sexe,                                                                FTN_MAJ_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NUMSS,      m_pGUI->lineEdit_NumSecu->text().remove(' ')+
                                                                                   m_pGUI->lineEdit_NssClef->text(),                                    FTN_NUM_STRING));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NSS,        m_pGUI->lineEdit_DtNss->text().remove('-'),                          FTN_STR_DATETIME));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_IS_ASS,     QString::number(m_pGUI->comboBoxQualiteAyantDroit->currentItem()+1), FTN_INTEGER));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NSS_ASS,    m_pGUI->lineEdit_DtNss_Assure->text().remove('-'),                   FTN_STR_DATETIME));
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_PROF,       m_pGUI->lineEdit_Profession->text(),                                 FTN_MAJ_STRING));
   QString                  rangGem  = m_pGUI->lineEdit_RangGeme->text().trimmed();
   //if (rangGem.length()==0) rangGem = rangGem.prepend("0");
   //if (rangGem.length()==1) rangGem = rangGem.prepend("0");
   list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_GEME,       rangGem,                                                             FTN_MAJ_STRING));
}

/////////////////////////////////////////////////// AGENDA ///////////////////////////////////////////////////////////////////////////

//------------------------------------------- initComboBoxAgendaUser -----------------------------------------------------------------------
void C_Manager::initComboBoxAgendaUser()
{
   //........... combobox ajouter un agenda utilisateur....................
   m_pGUI->comboBoxAgendaUser->clear();
   QStringList userList;
   G_pCApp->m_pCMoteurBase->GetUserList( userList , "-sgn-agc");
   m_pGUI->comboBoxAgendaUser->insertItems ( 0, userList );
   if (!G_pCApp->IsThisDroitExist("agm"))
      {m_pGUI->comboBoxAgendaUser->hide();
       m_pGUI->labelAgendaSelect->hide();
      }
}

//------------------------ allAgendasReconnect ---------------------------------------
void C_Manager::allAgendasReconnect(     const QString &driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                         const QString &baseToConnect, // nom de la base: si QODBC3 -> nom de la source de donn\303\251es (userDSN)
                                         const QString &user,          // = "root"
                                         const QString &pasword,       // = ""
                                         const QString &hostname,      // = "localhost"
                                         const QString &port           // = "port",
                                   )
{  QMapIterator<QString, C_Frm_Agenda*> it(m_AgendaMap);
   while (it.hasNext())
         {it.next();
          C_Frm_Agenda *pC_Frm_Agenda = it.value();
          if (pC_Frm_Agenda)
             {pC_Frm_Agenda->baseReConnect(driver, baseToConnect, user, pasword, hostname, port);
              break;          // une seule fois suffit car la base est toujours avec le m\303\252me label quelque soit l'agenda
             }
         }
}

//--------------------------------- setComboBoxOnValue ------------------------------------------------------------
int C_Manager::setComboBoxOnValue(QComboBox *pQComboBox, const QString& value)
{if (value.length()==0)return -1;
int index = pQComboBox->findText (value,  Qt::MatchExactly );  //Qt::MatchExactly | Qt::MatchCaseSensitive
if (index != -1) pQComboBox->setCurrentIndex  (index);
return index;
}

//------------------------ reinitAgendaOnUser ---------------------------------------
void C_Manager::reinitAgendaOnUser(const QString & /*user*/ , const QString & signUser)
{   //.................. version non fonctionnelle je ne sais pas pourquoi ........................
   //m_pC_Frm_Agenda->reinitAgendaOnUser(pUserItem, pSignUserItem);  // ne fonctionne pas !!! pourquoi ???
   //.................. version fonctionnelle mais detruit tout et refait tout  ........................
   setTitle();
   QDate  startDate              = QDate::currentDate();
   if (m_AgendaMap.count(signUser))
      {C_Frm_Agenda *pC_Frm_Agenda   = m_AgendaMap[signUser];
       if (pC_Frm_Agenda)
          {startDate  = pC_Frm_Agenda->getStartDate();  // recuperer la date de debut avant de reconstruire
           if (G_pCApp->IsThisDroitExist("agm")) {deleteAgenda(signUser);  m_pGUI->comboBoxAgendaUser->show();m_pGUI->labelAgendaSelect->show();}
           else                                  {deleteAllAgendas();      m_pGUI->comboBoxAgendaUser->hide();m_pGUI->labelAgendaSelect->hide();}
          }
      }
   else
      {if (G_pCApp->IsThisDroitExist("agm"))    {                          m_pGUI->comboBoxAgendaUser->show();m_pGUI->labelAgendaSelect->show();}
       else                                     { deleteAllAgendas();      m_pGUI->comboBoxAgendaUser->hide();m_pGUI->labelAgendaSelect->hide();}
      }
   addUserAgenda(signUser, startDate);
   setComboBoxOnValue(m_pGUI->comboBoxAgendaUser, signUser);

}

//------------------------ deleteAllAgendas ---------------------------------------
QStringList C_Manager::deleteAllAgendas()
{   QStringList agendasList = getAgendasList();
   //.................. les effacer ...............................
   for (int i = 0; i < agendasList.size(); ++i)
       {deleteAgenda(agendasList[i]);
       }
   m_AgendaMap.clear();
   return agendasList;
}

//------------------------ getAgendasList ---------------------------------------
QStringList C_Manager::getAgendasList()
{  QStringList agendasList;
   QMapIterator<QString, C_Frm_Agenda*> it(m_AgendaMap);
   while (it.hasNext())
         {it.next();
          agendasList<<it.key();
         }
   return agendasList;
}

//---------------------------------------------- eraseUserAgenda -----------------------------------------------------------------------
void C_Manager::eraseUserAgenda(C_Frm_Agenda* pC_Frm_Agenda)
{if (pC_Frm_Agenda)
   {deleteAgenda(pC_Frm_Agenda->getSignUser());
   }
}
//---------------------------------------------- Slot_OnFastAccesAgendaButtonClicked -----------------------------------------------------------------------
void C_Manager::Slot_OnFastAccesAgendaButtonClicked(QAbstractButton *pQAbstractButton)
{//Wdg_ButtonPtr *pWdg_ButtonPtr = (Wdg_ButtonPtr*) button;
 QString user = QString(pQAbstractButton->name()).remove("ButtonThisAgenda_");
 addUserAgenda(user, QDate::currentDate());
}

//---------------------------------------------- Slot_buttonAgendaDelete_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_buttonAgendaDelete_Clicked(Wdg_ButtonPtr *pWdg_ButtonPtr)
{QString name     = pWdg_ButtonPtr->objectName();
 QString signUser = name.remove("ButtonClose_");
 deleteAgenda(signUser);
}

//---------------------------------------------- Slot_buttonAgendaDelete_Clicked -----------------------------------------------------------------------
void C_Manager::deleteAgenda(const QString &signUser)
{QFrame *pQFrame = m_pGUI->frame_MultiAgenda->findChild<QFrame *>("agendaRacine_" + signUser);
if (pQFrame)
   {m_AgendaMap.remove ( signUser );
    m_pAgendaQLayout->removeWidget(pQFrame);
    delete pQFrame;
   }
 //............ effacer l'eventuel bouton d'acces rapide .................................
 //              associe ? cet agenda
 QList<QAbstractButton *> buttonsList =	m_pGUI->buttonBox_agendas->buttons();
 for (int i=0; i<buttonsList.size(); ++i)
     {QAbstractButton *pQAbstractButton = buttonsList[i];
      QString name = QString (pQAbstractButton->name()).remove("ButtonThisAgenda_");
      if (name==signUser) m_pGUI->buttonBox_agendas->removeButton(pQAbstractButton);
     }
}

//---------------------------------------------- Slot_comboBoxAgendaUser_activated -----------------------------------------------------------------------
void C_Manager::Slot_comboBoxAgendaUser_activated( const QString &signUser )
{
   if (!G_pCApp->IsThisDroitExist("agc") &&      // si pas d'agenda possible pour l'utilisateur en cours
       !G_pCApp->IsThisDroitExist("agm"))        // et qu'il n'est pas autorise a gerer des agendas multiples (ceux des autres)
      {G_pCApp->CouCou(tr("You do not have rights to use the appointment schedule"));
       return;
      }
 addUserAgenda(signUser, QDate::currentDate());
 //if (G_pCApp->IsThisDroitExistForThisUser(signUser, "agc")) m_pGUI->scrollArea_MultiAgenda->show(); // peut avoir ete cache par
}
//---------------------------------------------- Slot_pQPushButtonAgendaDate_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_pQPushButtonAgendaDate_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr)
{    /*
    if (!G_pCApp->IsThisDroitExist("agc") &&      // si pas d'agenda possible pour l'utilisateur en cours
        !G_pCApp->IsThisDroitExist("agm"))        // et qu'il n'est pas autorise a gerer des agendas multiples (ceux des autres)
       {G_pCApp->CouCou(tr("Vous n'avez pas les droits suffisants pour utiliser l'agenda"));
        return;
       }
    */
//............... recuperer un pointeur sur l'agenda .................................................
C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
QLineEdit     *pQLineEditDate = (QLineEdit*)    pWdg_ButtonPtr->getPtr_2();
if (pC_Frm_Agenda->getPaintMode() >= C_Frm_Agenda::NORMAL)
   {//............... recuperer la date de ce calendrier .................................................
    QDate   dateIn = pC_Frm_Agenda->getStartDate();
    //............... lancer le calendrier .................................................
    QString date  = G_pCApp->execCalendrier(dateIn); date = date.remove('-');
    QDate dateNew = QDate::fromString(date, "ddMMyyyy");
    //....................... analyse du retour ..........................................................................
    if (dateNew.isValid () && dateNew != dateIn )
       {pQLineEditDate->setText(date);
        pC_Frm_Agenda->reinitAgendaOnDate(dateNew);
       }
   }
else
   {QWebView *pQWebView = pC_Frm_Agenda->getWebView();
    if (pQWebView==0) return;
    pQWebView->back();
   }
}
//---------------------------------------------- Slot_pQPushButtonFreeSpace_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_pQPushButtonFreeSpace_Clicked (Wdg_ButtonPtr *pWdg_ButtonPtr)
{ C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
  pC_Frm_Agenda->getFreeSpace();
}

//---------------------------------------------- Slot_pQPushButtonPreviusDay_Clicked -----------------------------------------------------------------------
// Afficher le jour Suivant CZA
void C_Manager::Slot_pQPushButtonPreviusDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr)
{
C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
QLineEdit     *pQLineEditDate = (QLineEdit*)    pWdg_ButtonPtr->getPtr_2();
if (pC_Frm_Agenda->getPaintMode() >= C_Frm_Agenda::NORMAL)
   {//............... recuperer la date de ce calendrier .................................................
    QDate   dateIn = pC_Frm_Agenda->getStartDate();
    if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "DAY")
        dateIn = dateIn.addDays(-1);
    else if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "WEEK")
        dateIn = dateIn.addDays(-7);
    else if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "MONTH")
        dateIn = dateIn.addDays(-30);

    pQLineEditDate->setText(dateIn.toString("dd-MM-yyyy"));
    pC_Frm_Agenda->animateLeft();
    pC_Frm_Agenda->reinitAgendaOnDate(dateIn);
   }
}

//---------------------------------------------- Slot_pQPushButtonNextDay_Clicked -----------------------------------------------------------------------
// Afficher le jour Suivant CZA
void C_Manager::Slot_pQPushButtonNextDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr)
{
C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
QLineEdit     *pQLineEditDate = (QLineEdit*)    pWdg_ButtonPtr->getPtr_2();
if (pC_Frm_Agenda->getPaintMode() >= C_Frm_Agenda::NORMAL)
   {//............... recuperer la date de ce calendrier .................................................
    QDate   dateIn = pC_Frm_Agenda->getStartDate();
    if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "DAY")
        dateIn = dateIn.addDays(1);
    else if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "WEEK")
        dateIn = dateIn.addDays(7);
    else if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "MONTH")
        dateIn = dateIn.addDays(30);

    pQLineEditDate->setText(dateIn.toString("dd-MM-yyyy"));
    pC_Frm_Agenda->animateRight();
    pC_Frm_Agenda->reinitAgendaOnDate(dateIn);
   }
}
//---------------------------------------------- Slot_pQPushButtonPatientRdvList_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_pQPushButtonPatientRdvList_Clicked(Wdg_ButtonPtr* pWdg_ButtonPtr)
{C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
 pC_Frm_Agenda->Chercher_les_RDV_dun_patient ();
}
//---------------------------------------------- Slot_pQPushButtonUserRdvList_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_pQPushButtonUserRdvList_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr)
{C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
    pC_Frm_Agenda->Imprimer_les_RDV_dun_medecin(QDateTime::currentDateTime(), pC_Frm_Agenda->getSignUser());
}

//---------------------------------------------- Slot_pQPushButtonThisDay_Clicked -----------------------------------------------------------------------
// Afficher le jour Suivant CZA
void C_Manager::Slot_pQPushButtonThisDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr)
{
 C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
 if (pC_Frm_Agenda->getPaintMode() >= C_Frm_Agenda::NORMAL)
    {pC_Frm_Agenda->animateBottom();
     pC_Frm_Agenda->reinitAgendaOnDate(QDate::currentDate());
    }
}
//---------------------------------------------- Slot_pQPushButtonWeekDay_Clicked -----------------------------------------------------------------------
void C_Manager::Slot_ReinitModeWeekDayMonth_OnDate(const QDate &date , const QString & mode , C_Frm_Agenda *pC_Frm_Agenda)
{QString user             = pC_Frm_Agenda->getSignUser();
 QString buttonNameToFind = "";

 if      (mode=="DAY")     buttonNameToFind = "ButtonDay_"   + user;
 else if (mode=="MONTH")   buttonNameToFind = "ButtonMonth_" + user;
 else if (mode=="WEEK")    buttonNameToFind = "ButtonWeek_"  + user;

 Wdg_ButtonPtr *pQPushButtonWeek    = m_pGUI->frame_MultiAgenda->findChild<Wdg_ButtonPtr *>(buttonNameToFind);
 if (pQPushButtonWeek==0) return;
 pC_Frm_Agenda->setStartDate(date);
 Slot_pQPushButtonWeekDay_Clicked (pQPushButtonWeek);
}

//---------------------------------------------- Slot_pQPushButtonWeekDay_Clicked -----------------------------------------------------------------------
// Bascule affichage Semaine - Jour  CZA
// Redimentionnement du titre
void C_Manager::Slot_pQPushButtonWeekDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr)
{QString user                  = "";
 QString mode                  = pWdg_ButtonPtr->name();
 int pos                       = mode.indexOf('_'); if (pos !=-1) user = mode.mid(pos+1);
 C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
 QLabel *pQlabelNomUser        = (QLabel*) pWdg_ButtonPtr->getPtr_3();
 QFrame *pQFrameButtonAndTitle = (QFrame*) pWdg_ButtonPtr->getPtr_4();
 int posFirstButton            = 0;
 if (user.length()==0) return;
 if (pC_Frm_Agenda->getPaintMode() >= C_Frm_Agenda::NORMAL)
   { Wdg_ButtonPtr *pQPushButtonWeek    = m_pGUI->frame_MultiAgenda->findChild<Wdg_ButtonPtr *>("ButtonWeek_" + user);
     Wdg_ButtonPtr *pQPushButtonDay     = m_pGUI->frame_MultiAgenda->findChild<Wdg_ButtonPtr *>("ButtonDay_" + user);
     Wdg_ButtonPtr *pQPushButtonMonth   = m_pGUI->frame_MultiAgenda->findChild<Wdg_ButtonPtr *>("ButtonMonth_" + user);
     Wdg_ButtonPtr *pQPushButtonNextDay = m_pGUI->frame_MultiAgenda->findChild<Wdg_ButtonPtr *>("ButtonNextDay_" + user);
     posFirstButton                     = pQPushButtonNextDay->x() + pQPushButtonNextDay->width();

     if      (mode.contains("ButtonDay_"))   pC_Frm_Agenda->setAgendaMode_WeekOrDayOrMonth("DAY");
     else if (mode.contains("ButtonWeek_"))  pC_Frm_Agenda->setAgendaMode_WeekOrDayOrMonth("WEEK");
     else if (mode.contains("ButtonMonth_")) pC_Frm_Agenda->setAgendaMode_WeekOrDayOrMonth("MONTH");
     pC_Frm_Agenda->animateRight();
     pC_Frm_Agenda->reinitAgendaOnDate(pC_Frm_Agenda->getStartDate());

     QScrollArea     *pToSrcollArea = (QScrollArea*)    pWdg_ButtonPtr->getPtr_2();
     int newWidth;
     QString modeDayWeekMonth = pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth();
     if (modeDayWeekMonth == "DAY")
        {newWidth = G_pCApp->readUniqueParam ( "Agenda", "Largeur").toInt() + 20;
         pQPushButtonDay->hide();
         pQPushButtonWeek->setGeometry(posFirstButton,                              pQPushButtonWeek->y(),    pQPushButtonWeek->width(),  pQPushButtonWeek->height());
         pQPushButtonMonth->setGeometry(posFirstButton + pQPushButtonWeek->width(), pQPushButtonMonth->y(),   pQPushButtonMonth->width(), pQPushButtonMonth->height());
         pQPushButtonWeek->show();
         pQPushButtonMonth->show();
        }
     else if (modeDayWeekMonth == "WEEK")
        {newWidth = G_pCApp->readUniqueParam ( "Agenda", "Largeur semaine").toInt() + 20;
         pQPushButtonWeek->hide();
         pQPushButtonDay->setGeometry(posFirstButton,                            pQPushButtonDay->y(),     pQPushButtonDay->width(),   pQPushButtonDay->height());
         pQPushButtonMonth->setGeometry(posFirstButton+pQPushButtonDay->width(), pQPushButtonMonth->y(),   pQPushButtonMonth->width(), pQPushButtonMonth->height());
         pQPushButtonDay->show();
         pQPushButtonMonth->show();
        }
     else   //pC_Frm_Agenda->GetAgendaWidth() + 20;      // CZA Remplacer 20 / Largeur Scrollbar
        {newWidth = G_pCApp->readUniqueParam ( "Agenda", "Largeur semaine").toInt() + 20;
         pQPushButtonMonth->hide();
         pQPushButtonDay->setGeometry(posFirstButton,                           pQPushButtonDay->y(),     pQPushButtonDay->width(), pQPushButtonDay->height());
         pQPushButtonWeek->setGeometry(posFirstButton+pQPushButtonDay->width(), pQPushButtonWeek->y(),    pQPushButtonWeek->width(), pQPushButtonWeek->height());
         pQPushButtonDay->show();
         pQPushButtonWeek->show();
        }
     G_pCApp->writeParam(  "Agenda" , "Affichage Jour ou Semaine" , modeDayWeekMonth);
     G_pCApp->updateIniParamToDisk();

     int     actualWidthLabel = pQlabelNomUser->width();
     int     actuelWidthTitle = pQFrameButtonAndTitle->width();

     pToSrcollArea->setMaximumWidth(newWidth);
     pToSrcollArea->setMinimumWidth(newWidth);
     pQFrameButtonAndTitle->setMaximumWidth(newWidth);
     pQFrameButtonAndTitle->setMinimumWidth(newWidth);

     pQlabelNomUser->resize(newWidth - (actuelWidthTitle - actualWidthLabel),pQlabelNomUser->height());
     setUserAgendaVisible(user);
   }
}

//---------------------------------------------- setUserAgendaVisible -----------------------------------------------------------------------
void C_Manager::setUserAgendaVisible(const QString &signUser)
{m_tmpSignUser = signUser;
 QTimer::singleShot(10, this, SLOT(Slot_setUserAgendaVisible()));
}

//---------------------------------------------- Slot_setUserAgendaVisible -----------------------------------------------------------------------
void C_Manager::Slot_setUserAgendaVisible()
{QFrame *pQFrame = m_pGUI->frame_MultiAgenda->findChild<QFrame *>("agendaRacine_" + m_tmpSignUser);
if (pQFrame)
   { m_pGUI->scrollArea_MultiAgenda->ensureWidgetVisible ( pQFrame,0 , 0);
   }
Wdg_ButtonPtr *pQPushButton  = m_pGUI->frame_MultiAgenda->findChild<Wdg_ButtonPtr *>("ButtonClose_" + m_tmpSignUser);
if (pQPushButton)  m_pGUI->scrollArea_MultiAgenda->ensureWidgetVisible ( pQPushButton, 0, 0);
}

//---------------------------------------------- Slot_setUserAgendaVisible -----------------------------------------------------------------------
void C_Manager::Slot_pQPushButtonMenuAgenda_Clicked(Wdg_ButtonPtr* pWdg_ButtonPtr)
{ //  Theme::setFontSize_Menu(9);
   QStringList optionList;
   optionList<<tr("$Agenda");
   optionList<<"=1=#Agenda/h-01.png#"+tr("Display schedule with one pixel per minute.");
   optionList<<"=2=#Agenda/h-02.png#"+tr("Display schedule with 2 pixels per minute.");
   optionList<<"=3=#Agenda/h-03.png#"+tr("Display schedule with 3 pixels per minute.");
   optionList<<"=4=#Agenda/h-04.png#"+tr("Display schedule with 4 pixels per minute.");
   optionList<<"=5=#Agenda/h-05.png#"+tr("Display schedule with 5 pixels per minute.");
   optionList<<"-----------";
   optionList<<"=6=#Agenda/h-01.png#"+tr("Define appointments on a multiple of one minute.");
   optionList<<"=10=#Agenda/h-05.png#"+tr("Define appointments on a multiple of five minutes.");
   optionList<<"=15=#Agenda/h-10.png#"+tr("Define appointments on a multiple of ten minutes.");
   optionList<<"=20=#Agenda/p-15.png#"+tr("Define appointments on a multiple of fifteen minutes.");
   optionList<<"-----------";
   optionList<<"=21=#Agenda/agendaDebut.png#"+tr("Define the first visible hour.");
   optionList<<"=22=#Agenda/agendaFin.png#"+tr("Define the last visible hour.");
   optionList<<"-----------";
   optionList<<"=30=#Agenda/h-00.png#"+tr("Cancel automatic update.");
   optionList<<"=35=#Agenda/h-05.png#"+tr("Update every five seconds.");
   optionList<<"=40=#Agenda/h-10.png#"+tr("Update every ten seconds.");
   optionList<<"=50=#Agenda/p-20.png#"+tr("Update every twenty seconds.");
   optionList<<"=90=#Agenda/p-60.png#"+tr("Update every one minute");
   optionList<<"-----------";
   optionList<<"=100=#Agenda/AgendaSingleLine.png#"+tr("Show the miniature view (folded mode) on one line.");
   optionList<<"=101=#Agenda/AgendaDoubleLine.png#"+tr("Show the miniature view (folded mode) on two lines.");
   optionList<<"-----------";
   optionList<<"=110=#Agenda/changeWidth.png#"+tr("Modify schedule width.");
   optionList<<"-----------";
   optionList<<"=120=#Agenda/noConfirmModif.png#"+tr("Unactivate modifications confirmation.");
   optionList<<"=121=#Agenda/ConfirmModif.png#"+tr("Activate modifications confirmation.");
   optionList<<"-----------";
   optionList<<"=130=#Agenda/Google1.png#"+tr("One month  Google Agenda Synchronization.");
   optionList<<"=131=#Agenda/Google2.png#"+tr("Two months Google Agenda Synchronization.");
   optionList<<"=135=#Agenda/Google6.png#"+tr("Six months Google Agenda Synchronization.");
   optionList<<"-----------";
   if (G_pCApp->readUniqueParam ( "Agenda", "Factices")=="ok") optionList<<"=179=#Agenda/creerFactice.png#"+tr("Create several appointments for start of week.");
   optionList<<"-----------";
   optionList<<"=180=#Agenda/QuitterMenu.png#"+tr("Quit this menu.");

   pWdg_ButtonPtr->setIcon(Theme::getIcon("Agenda/AgendaMenuDown.png"));
   QString option = ThemePopup(optionList,this, " border: 1px solid #8f8f91; border-radius: 6px; font-size: 11px;").DoPopupList();
   pWdg_ButtonPtr->setIcon(Theme::getIcon("Agenda/AgendaMenu.png"));

   if (option.length()==0) return;
   int opt = option.toInt();
   C_Frm_Agenda  *pC_Frm_Agenda  = (C_Frm_Agenda*) pWdg_ButtonPtr->getPtr_1();
   if (opt>=1 && opt<=5)
      {pC_Frm_Agenda->changePixelParMinute(opt);
       G_pCApp->writeParam(  "Agenda" , "Nombre pixels par minute" , QString::number(opt));
       G_pCApp->updateIniParamToDisk( );
      }
   else if (opt>=6 && opt<=20)
      {pC_Frm_Agenda->changeMagnetisme(opt-5);
       G_pCApp->writeParam(  "Agenda" , "Magnetisme" , QString::number(opt-5));
       G_pCApp->updateIniParamToDisk( );
      }
   else if (opt>=100 && opt<=101)
      {opt = opt - 100;
       pC_Frm_Agenda->changeRepresentation(opt);
       G_pCApp->writeParam(  "Agenda" , "Representation" , QString::number(opt));
       G_pCApp->updateIniParamToDisk( );
      }
   else if (opt>=120 && opt<=121)
     {opt = (opt-120)*3;
      pC_Frm_Agenda->changeModifConfirm(opt);
      G_pCApp->writeParam(  "Agenda" , "Modif confirm" , QString::number(opt));
      G_pCApp->updateIniParamToDisk( );
     }
   else if (opt>=130 && opt<=135)
     {opt = (opt-129);
      pC_Frm_Agenda->toGoogleSynchonization(opt);
     }
   else if (opt>=21 && opt<=22)
     {bool ok;
      QString text = QInputDialog::getText(this, tr("Enter an hour formatted this way: hh:mm"), tr("Hour (hh:mm) :"), QLineEdit::Normal, opt==21?"08:00":"19:00", &ok).trimmed();
      if (ok && text.length())
         {QTime tm = QTime::fromString(text,"hh:mm");
          if (!tm.isValid()) return;
          if (opt==21)
             {pC_Frm_Agenda->changeStartTime ( text );
              G_pCApp->writeParam(  "Agenda" , "Heure limite basse" , text);
             }
          else
             {pC_Frm_Agenda->changeEndTime (text);
              G_pCApp->writeParam(  "Agenda" , "Heure limite haute" , text);
             }
          G_pCApp->updateIniParamToDisk( );
         }
      }
   else if (opt>=30 && opt<=90)
      {pC_Frm_Agenda->changeRafraichissement(opt-30);
       G_pCApp->writeParam(  "Agenda" , "Rafraichissement" , QString::number(opt-30));
       G_pCApp->updateIniParamToDisk( );
      }
   else if (opt==110)
     {bool ok;
      QString text = QInputDialog::getText(this, tr("Enter width of schedule"), tr("Width in pixels:"), QLineEdit::Normal, "300", &ok).trimmed();
      if (ok && text.length())
         {QScrollArea   *scrollArea_Days            = (QScrollArea*)pWdg_ButtonPtr->getPtr_2();
          QFrame        *agendaFrameDaysAndTitle    = (QFrame*)pWdg_ButtonPtr->getPtr_3();
          int value = qMin(600,qMax(text.toInt(),150));
          pC_Frm_Agenda->changeAgendaWidth( value );
          scrollArea_Days->resize(value+10, scrollArea_Days->height());

          agendaFrameDaysAndTitle->resize(value+10, agendaFrameDaysAndTitle->height());
          G_pCApp->writeParam(  "Agenda" , "Largeur" , text);
          G_pCApp->updateIniParamToDisk( );
         }
      }
   else if (opt==179)
      {pC_Frm_Agenda->creerRDVFactices(pC_Frm_Agenda->getSignUser());
      }
}

//---------------------------------------------- addUserAgenda -----------------------------------------------------------------------
C_Frm_Agenda *C_Manager::addUserAgenda(const QString &signUser, QDate date, QFrame **ppQFrame)
{  if (signUser.length()==0) return 0;
   if (m_AgendaMap.count(signUser))
      { setUserAgendaVisible(signUser);
        return 0;
      }
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   QString nom, prenom, googleUser, googlePass;
   //.................. recuperer google user et pot de masse .............................
   QString userParam = m_pCMoteurBase->ReadDrTuxUserParametres(signUser);
   googleUser        = CGestIni::Param_ReadUniqueParam(userParam.toLatin1(), "GoogleAgenda","user");
   googlePass        = CGestIni::Param_ReadUniqueParam(userParam.toLatin1(), "GoogleAgenda","pass");
   if (googlePass.left(1)=="#")
      { googlePass = CGestIni::PassWordDecode(googlePass);
      }
   else //.............. si pas encode on encode pour le cacher ...................................
      { QString encripted = CGestIni::PassWordEncode(googlePass).prepend("#");
        CGestIni::Param_WriteParam(&userParam, "GoogleAgenda","pass",encripted.toAscii());
        m_pCMoteurBase->Param_SavParam(&userParam ,signUser);
      }

   m_pCMoteurBase->GetUserNomPrenom( signUser, nom, prenom);
   if (G_pCApp->m_pQSplashScreen) G_pCApp->m_pQSplashScreen->showMessage(QObject::tr("Initialising agenda '%1' ....").arg(nom+" "+prenom), Qt::AlignCenter | Qt::AlignCenter, Qt::black);

   m_pGUI->wdg_DockWidget_Agenda->setUpdatesEnabled(false);
   QString        userDroits                 = G_pCApp->m_pCMoteurBase->GetUserPermisions(signUser);
   QString        imageFolder                = Theme::getPath()+"Agenda/";
   QString        styles                     = QString("border-radius: 6px; background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 white, stop: 0.4 #8baaae, stop:1 #2a4a6b); border: 1px solid #8f8f91; border-radius: 1px; font-size: 9px;");
   QFrame        *agendaFrameDaysAndTitle    = new QFrame(m_pGUI->frame_MultiAgenda);
                                               agendaFrameDaysAndTitle->setObjectName("agendaRacine_"+signUser);
                                               agendaFrameDaysAndTitle->setStyleSheet("font-size: 10px;");
   QFrame        *frameButtonAndTitle        = new QFrame(agendaFrameDaysAndTitle);

   QVBoxLayout   *agendaVBoxDaysAndTitle     = new QVBoxLayout(agendaFrameDaysAndTitle); agendaVBoxDaysAndTitle->setObjectName("agendaVBoxDaysAndTitle_" + signUser);
   QScrollArea   *scrollArea_Days            = new QScrollArea(agendaFrameDaysAndTitle);
   C_Frm_Agenda  *pC_Frm_Agenda              = new C_Frm_Agenda( date, scrollArea_Days, G_pCApp->pathAppli(), G_pCApp->iniParam(), signUser,G_pCApp->m_User, nom+" "+prenom, userDroits,googleUser,googlePass);        // creer la liste des jours
   QLabel        *pQLabel                    = new QLabel(frameButtonAndTitle); pQLabel->setStyleSheet(styles);
   QLineEdit     *pQLineEditDate             = new QLineEdit(frameButtonAndTitle); pQLineEditDate->setInputMask ("99-99-9999"); pQLineEditDate->setStyleSheet(styles);
   Wdg_ButtonPtr *pQPushButtonDate           = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonDate_"  + signUser); pQPushButtonDate->setPtr_1(pC_Frm_Agenda); pQPushButtonDate->setPtr_2(pQLineEditDate);
   Wdg_ButtonPtr *pQPushButtonClose          = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonClose_" + signUser); pQPushButtonClose->setPtr_1(pC_Frm_Agenda);
   Wdg_ButtonPtr *pQPushButtonMenu           = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonMenu_" + signUser);
   pQPushButtonMenu->setPtr_1(pC_Frm_Agenda);
   pQPushButtonMenu->setPtr_2(scrollArea_Days);
   pQPushButtonMenu->setPtr_3(agendaFrameDaysAndTitle);

   //............... n'installer les boutons d'acces rapide ...............................
   //                que pour les utilisateurs de la liste de prechargement
   QString     listagendas = G_pCApp->readUniqueParam ( "Agenda", "Liste des agendas a ouvrir");
   QStringList listeUsersAgenda;
   listeUsersAgenda = listagendas.split(';',QString::SkipEmptyParts);
   if (listeUsersAgenda.count() > 0)
      {for (int i = 0; i < listeUsersAgenda.count(); i++)
           {QString user = listeUsersAgenda[i].remove(' ');
            if (user==signUser)
               {Wdg_ButtonPtr *pQPushButtonAgendaSelect     = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonThisAgenda_" + signUser);
                pQPushButtonAgendaSelect->setPtr_1(pC_Frm_Agenda);
                pQPushButtonAgendaSelect->setText(nom+" "+prenom);
                pQPushButtonAgendaSelect->setMaximumHeight(m_pGUI->comboBoxAgendaUser->height());
             #ifdef  Q_WS_MAC
                pQPushButtonAgendaSelect->setFlat(true);
             #endif
                // changement de couleur du bouton par user si existe dans manager.ini
                QString colorUser = G_pCApp->readUniqueParam ( QString("Agenda"), "CouleurBouton_" + signUser);
                if (colorUser.length() > 1)
                   { colorUser.replace("_",",");
                     pQPushButtonAgendaSelect->setStyleSheet(colorUser);
                   }

                m_pGUI->buttonBox_agendas->addButton (pQPushButtonAgendaSelect, QDialogButtonBox::ActionRole );
               }
           }
      }

   Wdg_ButtonPtr *pQPushButtonPreviusDay     = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonPreviusDay_" + signUser);
   pQPushButtonPreviusDay->setPtr_1(pC_Frm_Agenda);
   pQPushButtonPreviusDay->setPtr_2(pQLineEditDate);

   Wdg_ButtonPtr *pQPushButtonNextDay        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonNextDay_" + signUser);
   pQPushButtonNextDay->setPtr_1(pC_Frm_Agenda);
   pQPushButtonNextDay->setPtr_2(pQLineEditDate);

   Wdg_ButtonPtr *pQPushButtonThisDay     = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonThisDay_" + signUser);
   pQPushButtonThisDay->setPtr_1(pC_Frm_Agenda);
   pQPushButtonThisDay->setPtr_2(pQLineEditDate);

   // changement de couleur du titre par user si existe dans manager.ini
   QString colorUser = G_pCApp->readUniqueParam ( QString("Agenda"), "Couleur_" + signUser);
   if (colorUser.length() > 1)
      { colorUser.replace("_",",");
        pQLabel->setStyleSheet(colorUser);
      }

   Wdg_ButtonPtr *pQPushButtonDay        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonDay_" + signUser);
   pQPushButtonDay->setText(tr("Day"));
   pQPushButtonDay->setPtr_1(pC_Frm_Agenda);
   pQPushButtonDay->setPtr_2(scrollArea_Days);
   pQPushButtonDay->setPtr_3(pQLabel);
   pQPushButtonDay->setPtr_4(frameButtonAndTitle);
   pQPushButtonDay->setToolTip ( "<font color=\"#000000\">"+tr("Display Day")+"</font>" );
   pQPushButtonDay->hide();

   Wdg_ButtonPtr *pQPushButtonWeek        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonWeek_" + signUser);
   pQPushButtonWeek->setText(tr("Week"));
   pQPushButtonWeek->setPtr_1(pC_Frm_Agenda);
   pQPushButtonWeek->setPtr_2(scrollArea_Days);
   pQPushButtonWeek->setPtr_3(pQLabel);
   pQPushButtonWeek->setPtr_4(frameButtonAndTitle);
   pQPushButtonWeek->setToolTip ( "<font color=\"#000000\">"+tr("Display Week")+"</font>" );
   pQPushButtonWeek->hide();

   Wdg_ButtonPtr *pQPushButtonMonth        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonMonth_" + signUser);
   pQPushButtonMonth->setText(tr("Month"));
   pQPushButtonMonth->setPtr_1(pC_Frm_Agenda);
   pQPushButtonMonth->setPtr_2(scrollArea_Days);
   pQPushButtonMonth->setPtr_3(pQLabel);
   pQPushButtonMonth->setPtr_4(frameButtonAndTitle);
   pQPushButtonMonth->setToolTip ( "<font color=\"#000000\">"+tr("Display Month")+"</font>" );
   pQPushButtonMonth->hide();

   Wdg_ButtonPtr *pQPushButtonFreeSpace        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonFreeSpace_" + signUser);
   pQPushButtonFreeSpace->setPtr_1(pC_Frm_Agenda);
   pQPushButtonFreeSpace->setToolTip ( "<font color=\"#000000\">"+tr("Choose free space for new appointment")+"</font>" );

   Wdg_ButtonPtr *pQPushButtonPatientRdvList        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonPatientRdvList_" + signUser);
   pQPushButtonPatientRdvList->setPtr_1(pC_Frm_Agenda);
   pQPushButtonPatientRdvList->setToolTip ( "<font color=\"#000000\">"+tr("Find appointments list for a patient")+"</font>" );

   Wdg_ButtonPtr *pQPushButtonUserRdvList        = new Wdg_ButtonPtr( frameButtonAndTitle , "ButtonUserRdvList_" + signUser);
   pQPushButtonUserRdvList->setPtr_1(pC_Frm_Agenda);
   pQPushButtonUserRdvList->setToolTip ( "<font color=\"#000000\">"+tr("Find appointments list for a user")+"</font>" );

   QScrollBar    *pQScrollBar                = scrollArea_Days->verticalScrollBar();   // juste pour la largeur
   int            wScroll                    = 14;
   int                  w                    = pC_Frm_Agenda->width() + wScroll+5;
   int agendaButtonHeight                    = pC_Frm_Agenda->getAgendaButtonBoxHeight();        // CZA
   int agendaTitleHeight                     = pC_Frm_Agenda->getTitleHeight();
   int pos_Obj_X                             = 0;

   pC_Frm_Agenda->setUser(signUser);

   pQPushButtonDay->setGeometry(   0, agendaTitleHeight,  agendaButtonHeight,agendaButtonHeight);
   pQPushButtonWeek->setGeometry(  0, agendaTitleHeight,  agendaButtonHeight,agendaButtonHeight);
   pQPushButtonMonth->setGeometry( 0, agendaTitleHeight,  agendaButtonHeight,agendaButtonHeight);


   QString        scrollStyle ="QScrollBar:vertical {"
                               "  border: 1px solid grey;"
                               "  background: #868786;"
                               "  width: %3px;"
                               "  margin: 15px 0 15px 0;"
                               "}"
                               "QScrollBar::handle:vertical {"
                               //"    background: white;"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 white, stop: 0.4 #8baaae, stop:1 #2a4a6b); border: 1px solid #8f8f91;"
                               "    min-height: 20px;"
                               "}"
                               "QScrollBar::add-line:vertical {"
                               "    border: 1px solid grey;"
                               //"    background: #32CC99;"
                               "background-image: url(%2);"
                               "    height: 12px;"
                               "    subcontrol-position: bottom;"
                               "    subcontrol-origin: margin;"
                               "}"

                               "QScrollBar::sub-line:vertical {"
                               "    border: 1px solid grey;"
                               //"    background: #32CC99;"
                               "background-image: url(%1);"
                               "    height: 12px;"
                               "    subcontrol-position: top;"
                               "    subcontrol-origin: margin;"
                               "}"
                               //"QScrollBar::up-arrow:vertical {"
                               //"    border: 0px solid grey;"
                               //"    width: 15px;"
                               //"    height: 15px;"
                               //"    background: white;"
                               //"background-image: url(%1);"
                               //"}"
                               //"QScrollBar::down-arrow:vertical {"
                               //"    border: 0px solid grey;"
                               //"    width: 15px;"
                               //"    height: 15px;"
                               //"    background: white;"
                               //"background-image: url(%2);"
                               //"}"
                               ;
   agendaVBoxDaysAndTitle->setSpacing(5);
   agendaVBoxDaysAndTitle->setMargin(0);
   agendaVBoxDaysAndTitle->setContentsMargins(0, 0, 0, 0);

   pQScrollBar->setStyleSheet(scrollStyle.arg(imageFolder+"SliderUp.png",imageFolder+"SliderDown.png", QString::number(wScroll)));
   scrollArea_Days->setWidget(pC_Frm_Agenda);

   // CZA ---------xx----------------------
   /***************** CZC
   if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "DAY")
       { w = G_pCApp->readParam ( "Agenda", "Largeur").toInt() + wScroll + 5;
         frameButtonAndTitle->setMinimumSize ( w , agendaButtonHeight + agendaTitleHeight);
         frameButtonAndTitle->setMaximumSize ( w , agendaButtonHeight + agendaTitleHeight);
       }
   else
       { w = G_pCApp->readParam ( "Agenda", "Largeur semaine").toInt() + wScroll + 5;
         frameButtonAndTitle->setMinimumSize ( w , agendaButtonHeight + agendaTitleHeight);
         frameButtonAndTitle->setMaximumSize ( w , agendaButtonHeight + agendaTitleHeight);
        }
   CZC **************************/

   // ------ CZC debut ---------------------
   int widthWeek        = qMax(G_pCApp->readUniqueParam ( "Agenda", "Largeur semaine").toInt(),200) ;
   int heightTitle      = qMax(G_pCApp->readUniqueParam ( "Agenda", "Hauteur bandeau jour ouvert").toInt(),10);
   int nbDayInWeek      = qMax(G_pCApp->readUniqueParam ( "Agenda", "Nombre de jours par semaine").toInt(),2);

   if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() == "DAY")
       w = G_pCApp->readUniqueParam ( "Agenda", "Largeur").toInt() + wScroll + 5;
   else
       w = widthWeek + wScroll + 5;
   frameButtonAndTitle->setMinimumSize ( w , agendaButtonHeight + agendaTitleHeight);
   frameButtonAndTitle->setMaximumSize ( w , agendaButtonHeight + agendaTitleHeight);

   // Creation d'un Frame qui contiendra les titres de colonnes jours
   QFrame *pQFrameDays = new QFrame(agendaFrameDaysAndTitle);
   pQFrameDays->setObjectName("FrameDesJours_" + signUser);
   //leCss = G_pCApp->readParam ( "Agenda", "Style du label jour");
   pQFrameDays->setStyleSheet(G_pCApp->readUniqueParam ( "Agenda", "Style du label jour").trimmed().replace("_",","));

   pQFrameDays->setGeometry(0,agendaButtonHeight + agendaTitleHeight + 6, widthWeek, heightTitle +2);
   // creation des 7 labels pour les 7 titres de jour
   QDate dateJW = date.addDays(-(date.dayOfWeek() - 1));
   int xlab = 0;
   int widthLabelDay = widthWeek / nbDayInWeek;
   for (int i = 1; i <= nbDayInWeek; i++)
        {QLabel *pLabelDateJour = new QLabel(pQFrameDays);
        pLabelDateJour->setObjectName("pLabelDateJour_" + QString::number(i) +"_"+ signUser);
        pLabelDateJour->setAlignment(Qt::AlignCenter);
        // utile que pour le 1er affichage si WEEK
        pLabelDateJour->setText(dateJW.toString(pC_Frm_Agenda->GetCMoteurAgenda()->GetFormatDateInResume('W'))); // RECUPERER FORMAT
        pLabelDateJour->setGeometry(xlab,0,widthLabelDay, heightTitle + 2);
        xlab += widthLabelDay;
        dateJW = dateJW.addDays(1);
        }
   if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() != "WEEK")
        pQFrameDays->hide();

  // -- fin CZC ------------
   pQPushButtonClose->resize( agendaTitleHeight , agendaTitleHeight);
   pQPushButtonClose->move  ( 0 ,          0);
   pQPushButtonClose->setIcon(Theme::getIcon("Agenda/CloseAgenda.png"));
   //pos_Obj_X += pQPushButtonClose->width();                         // CZA

       // bouton appel calendrier
   pQPushButtonDate->resize     ( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonDate->move       ( pos_Obj_X  ,         agendaTitleHeight);                  // CZA
   pos_Obj_X += pQPushButtonDate->width();                         // CZA
   if (pC_Frm_Agenda->getPaintMode() >= C_Frm_Agenda::NORMAL)
      { pQPushButtonDate->setIcon    ( Theme::getIcon("Agenda/dateChange.png"));
        pQPushButtonDate->setToolTip ( "<font color=\"#000000\">"+tr("Which date to start schedule")+"</font>" );
        pQPushButtonClose->setToolTip ("<font color=\"#000000\">"+tr("Close this schedule")+"</font>" );
      }
   else
      { pQPushButtonDate->setIcon    ( Theme::getIcon("Agenda/goBack.png"));
        pQPushButtonDate->setToolTip ( "<font color=\"#000000\">"+tr("Previous page")+"</font>" );
        pQPushButtonClose->setToolTip ("<font color=\"#000000\">"+tr("Close this page.")+"</font>" );
      }
   // CZA -----------------------
   QString SemOuJour = tr("Day");
   QString TipPreced = tr("Schedule of previus day");
   QString TipSuivan = tr("Schedule of next day");
   if (pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth() != "DAY")
      { SemOuJour = tr("Week");
        TipPreced = tr("Schedule of previus week");
        TipSuivan = tr("Schedule of next week");
      }
      // bouton jour precedent
   pQPushButtonPreviusDay->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonPreviusDay->move  ( pos_Obj_X ,          agendaTitleHeight);                  // CZA
   pQPushButtonPreviusDay->setIcon(Theme::getIcon("Agenda/Precedent.png"));
   pQPushButtonPreviusDay->setToolTip ( "<font color=\"#000000\">"+TipPreced+"</font>" );
   pos_Obj_X += pQPushButtonPreviusDay->width();                         // CZA
   // bouton ce jour
   pQPushButtonThisDay->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonThisDay->move  ( pos_Obj_X ,          agendaTitleHeight);                  // CZA
   pQPushButtonThisDay->setIcon(Theme::getIcon("Agenda/ThisDay.png"));
   pQPushButtonThisDay->setToolTip ( "<font color=\"#000000\">"+tr("this day")+"</font>" );
   pos_Obj_X += pQPushButtonThisDay->width();
   // QlineEdit Date du jour
   pQLineEditDate->hide();
   /*..... pas besoin de parametrer las positions car hide ............
   if ( !pC_Frm_Agenda->getPresentSimple())
      {pQLineEditDate->setText      (date.toString("ddMMyyyy"));
       pQLineEditDate->setStyleSheet(QString("border-radius: 6px; background-color: #fff9d8; border: 1px solid #8f8f91; border-radius: 1px; font-size: 9px;"));
       pQLineEditDate->setAlignment ( Qt::AlignHCenter );
       pQLineEditDate->resize       ( W_DATE_EDIT, agendaButtonHeight );
       pQLineEditDate->move         ( pos_Obj_X , ofsYbutton);
       pQLineEditDate->setToolTip   ("<font color=\"#000000\">"+tr("Which date to start schedule")+"</font>" );
       pQLineEditDate->setReadOnly  ( TRUE );
       // pos_Obj_X += pQLineEditDate->width();                         // CZA
      }
   else
       pQLineEditDate->hide();
   */
   // bouton jour suivant
   pQPushButtonNextDay->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonNextDay->move  ( pos_Obj_X ,          agendaTitleHeight);
   pQPushButtonNextDay->setIcon(Theme::getIcon("Agenda/Suivant.png"));
   pQPushButtonNextDay->setToolTip ( "<font color=\"#000000\">"+TipSuivan+"</font>" );
   pos_Obj_X += pQPushButtonNextDay->width();                         // CZA

   // bouton affichage jour
   QString dayWeekMonthMode = pC_Frm_Agenda->getAgendaMode_WeekOrDayOrMonth();
   if (dayWeekMonthMode != "DAY")
      { pQPushButtonDay->setGeometry(pos_Obj_X,agendaTitleHeight,agendaButtonHeight,agendaButtonHeight);
        pQPushButtonDay->show();
        pos_Obj_X += pQPushButtonDay->width();                         // CZA
      }
   // bouton affichage semaine
   if (dayWeekMonthMode != "WEEK")
      { pQPushButtonWeek->setGeometry(pos_Obj_X,agendaTitleHeight,agendaButtonHeight,agendaButtonHeight);
        pQPushButtonWeek->show();
        pos_Obj_X += pQPushButtonWeek->width();
      }
   // bouton affichage mois
   if (dayWeekMonthMode != "MONTH")
      { pQPushButtonMonth->setGeometry(pos_Obj_X,agendaTitleHeight,agendaButtonHeight,agendaButtonHeight);
        pQPushButtonMonth->show();
        pos_Obj_X += pQPushButtonMonth->width();
      }
   // bouton recherche espace libre
   pQPushButtonFreeSpace->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonFreeSpace->move  ( pos_Obj_X ,          agendaTitleHeight);
   pQPushButtonFreeSpace->setIcon    ( Theme::getIcon("Agenda/getFreeSpace.png"));
   pos_Obj_X += pQPushButtonFreeSpace->width();

   // bouton recherche rendez vous d'un patient
   pQPushButtonPatientRdvList->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonPatientRdvList->move  ( pos_Obj_X ,          agendaTitleHeight);
   pQPushButtonPatientRdvList->setIcon    ( Theme::getIcon("Agenda/GetPatientRdv.png"));
   pos_Obj_X += pQPushButtonPatientRdvList->width();

   // bouton recherche rendez vous d'un patient
   pQPushButtonUserRdvList->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonUserRdvList->move  ( pos_Obj_X ,          agendaTitleHeight);
   pQPushButtonUserRdvList->setIcon    ( Theme::getIcon("Agenda/PrintListRdv.png"));
   pos_Obj_X += pQPushButtonUserRdvList->width();

   // bouton menu parametres
   pQPushButtonMenu->resize( agendaButtonHeight , agendaButtonHeight);
   pQPushButtonMenu->move  ( pos_Obj_X ,          agendaTitleHeight);
   pQPushButtonMenu->setIcon(Theme::getIcon("Agenda/AgendaMenu.png"));
   pQPushButtonMenu->setToolTip ( "<font color=\"#000000\">"+tr("Various options for schedule")+"</font>" );
   pos_Obj_X += pQPushButtonMenu->width();                          // CZA

   // label nom utilisateur
   pQLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
   pQLabel->setText( pC_Frm_Agenda->getTitleTemplate().replace("{{TITLE}}", nom + " " + prenom) );
   pQLabel->resize ( w  , agendaTitleHeight );   // CZA
   pQLabel->move   ( 0  , 0);                           // CZA

   scrollArea_Days->setMaximumSize ( w , 16777215);
   agendaVBoxDaysAndTitle->addWidget(frameButtonAndTitle);
   agendaVBoxDaysAndTitle->addWidget(scrollArea_Days);
   if (ppQFrame) *ppQFrame = agendaFrameDaysAndTitle;
   m_pAgendaQLayout->addWidget(agendaFrameDaysAndTitle);
   m_AgendaMap[signUser]   = pC_Frm_Agenda;

   connect( pQPushButtonClose ,SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_buttonAgendaDelete_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonDate , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonAgendaDate_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonMenu , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonMenuAgenda_Clicked (Wdg_ButtonPtr*)) );

   connect( pQPushButtonPreviusDay      , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonPreviusDay_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonNextDay         , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonNextDay_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonThisDay         , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonThisDay_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonDay             , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonWeekDay_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonWeek            , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonWeekDay_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonMonth           , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonWeekDay_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonFreeSpace       , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonFreeSpace_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonPatientRdvList  , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonPatientRdvList_Clicked (Wdg_ButtonPtr*)) );
   connect( pQPushButtonUserRdvList     , SIGNAL(clicked (Wdg_ButtonPtr*)), this, SLOT(Slot_pQPushButtonUserRdvList_Clicked (Wdg_ButtonPtr*)) );
   // ------------- CZA

   connect( pC_Frm_Agenda,     SIGNAL(Sign_LauchPatient(const QString &, C_RendezVous *)),   this, SLOT( Slot_LauchPatient(const QString &, C_RendezVous *)));
   connect( pC_Frm_Agenda,     SIGNAL(Sign_ReinitModeWeekDayMonth_OnDate(const QDate & , const QString &, C_Frm_Agenda* )), this, SLOT( Slot_ReinitModeWeekDayMonth_OnDate(const QDate & , const QString &, C_Frm_Agenda*)));;
   G_pCApp->processEvents(QEventLoop::ExcludeUserInputEvents,10000);  // obligatoire si l'on veut que l'agenda se positionne sur le bon user
   m_tmpSignUser = signUser;
   Slot_setUserAgendaVisible();
   m_pGUI->wdg_DockWidget_Agenda->setUpdatesEnabled(TRUE);
   QApplication::restoreOverrideCursor();
   return pC_Frm_Agenda;
}

////////////////////////////////////// EDITION DES UTILISATEURS ////////////////////////////////////////////////////////////////////////////
//---------------------------------------------- Slot_pushButton_action_UserParams -----------------------------------------------------------------------
void C_Manager::Slot_pushButton_action_UserParams()
{editUser("");
}

//---------------------------------------------- Slot_EditUser -----------------------------------------------------------------------
void C_Manager::Slot_EditUser(const QString& /*user*/, const QString& /*signUser*/)
{editUser("");
}

//--------------------------------- Slot_UserMustBeUnloged --------------------------------------------------------------------------------
void C_Manager::Slot_UserMustBeUnloged(int &)
{//m_pC_Frm_UserList->reinitOn_SignUser_User_Name("", "");
setIdentiteDisabled();
m_pGUI->tabWidget_Central->setEnabled(FALSE);
m_pGUI->wdg_DockWidget_Agenda->setEnabled(FALSE);
}

//---------------------------------------------- editUser -----------------------------------------------------------------------
void C_Manager::editUser(QString cps)
{
QProcess  proc;
//........ recuperer l'ancienne liste des utilisateurs  .............................................
//          (pour comparer apres gest_user )
QStringList oldUserList;
m_pCMoteurBase->GetUserList(oldUserList);  // recuperer l'ancienne liste des utilisateurs (pour comparer apres gest_user )
//........ localiser gest_user .............................................
QString pathExe = CGestIni::Construct_Name_Exe("gest_user",QFileInfo (qApp->argv()[0]).dirPath (true));
//........ nourrir les arguments d'appel de gest_user .......................
QStringList argList;
argList <<( G_pCApp->m_User);                                        // Nom de l'utilisateur
argList <<( QString("#")+G_pCApp->m_CriptedPassWord );               // passWord
if (cps.length()) argList <<( cps.replace(",","__*$*__" ));
proc.start(pathExe, argList);
SLEEP(1);
QApplication::processEvents();
// positionne un flag dans l'application appelante
while (proc.state()==QProcess::Running )
      { QApplication::processEvents ( QEventLoop::WaitForMore );
      }
//................. soit creation d'un nouvel utilisateur et alors le selectionner ..........................
//                  soit effacement d'un utilisateur et alors se mettre en unlogued

//........ recuperer l'ancienne liste des utilisateurs  .............................................
//          (pour comparer apres gest_user )
QStringList newUserList;
m_pCMoteurBase->GetUserList(newUserList,"-sgn-agc");  // recuperer l'ancienne liste des utilisateurs (pour comparer apres gest_user )
m_pGUI->comboBoxAgendaUser->clear();
m_pGUI->comboBoxAgendaUser->insertItems ( 0, newUserList );
//............. si pas de changement cassos ...........................................................
if (newUserList == oldUserList) return;
//............. si cps -> selectionner si possible l'utilisateur de la cps ...............................
#ifdef SESAM_VERSION
if (cps.length())    // si cps active
   {QStringList fieldList;
    fieldList << m_pCMoteurBase->m_USER_IDENT_LOGIN;
    fieldList << m_pCMoteurBase->m_USER_IDENT_PASSWORD;
    QString userPk = m_pCMoteurBase->GetUserPrimKeyFromNumOrdre(G_pCApp->m_pCps->m_NIR, "-sgn", &fieldList);
    if (userPk.length())
       {QString userRet            = fieldList[0];
        G_pCApp->m_User            = userRet;
        G_pCApp->m_SignUser        = userRet;
        G_pCApp->m_CriptedPassWord = fieldList[1];     // m_USER_IDENT_PASSWORD
        Slot_reinitManagerOnUser(G_pCApp->m_User, G_pCApp->m_SignUser);
        return;
       }
   } // endif (cps.length())    // si cps active
#endif
 //............. verifier si l'utilisateur en cours est toujours l\303\240 ...............................
 //              si pas la reinitialiser la liste des utilisateurs
 //              et se mettre en unlogued
 QString userPk = m_pCMoteurBase->GetUserPrimKey(G_pCApp->m_User);
 if ( userPk.length()==0)
    {if (m_pC_Frm_UserList) m_pC_Frm_UserList->reinitOn_SignUser_User_Name("", "");
    }
}

//------------------------ Slot_CPS_IsClicked ---------------------------------------
void C_Manager::Slot_CPS_IsClicked()
{/*
#ifdef SESAM_VERSION

    QString          userPk = "";
    QString            user = "";
    QString            pass = "";
    QString      moduleName = "";
    int ret;

QString pathPlugin, pathLog;
G_pCApp->readParam(  "Sesam-Vitale", "pathPlugin",       &pathPlugin);
G_pCApp->readParam(  "Sesam-Vitale", "pathLog",          &pathLog);
G_pCApp->readParam(  "Sesam-Vitale", "ModuleName",       &moduleName);

if (moduleName == "PIXVITALE")
   {
   }
else
   {
    //..................................... ne pas rajouter "/" car c'est un nom de fichier ..........................
    if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanDirPath (pathPlugin.prepend(G_pCApp->pathAppli()));
    //..................................... rajouter "/" car le chemin doit \303\252tre donn\303\251 avec ..........................
    if ( QDir::isRelativePath ( pathLog ) )     pathLog    = QDir::cleanDirPath (pathLog.prepend(G_pCApp->pathAppli()) )+"/";

    QString pluginScript  = "[Execute]\r\n";
            pluginScript += "pathPlugin="      + pathPlugin                                           + "\r\n" +
                            "pathIni=****\r\n" +                // **** car indique d'utiliser le pathIni du plugin
                            "maskExch=Bonjour tout le monde\r\n"
                            "[Parametres]\r\n"
                            "m_Command       = READ_CPS\r\n"
                            "m_PathLog       =" + pathLog                                             + "\r\n" +
                            "m_DateConsult   =" + QDate::currentDate().toString("yyyyMMdd");
    //.............. liste des champs a extraire .......................................
    //               lors recherche de l'utilisateur indique par la Carte CPS
    QString     cps_data = "";
    QStringList fieldList;
    fieldList.append(m_pCMoteurBase->m_USER_IDENT_LOGIN);           // 0
    fieldList.append(m_pCMoteurBase->m_USER_IDENT_PASSWORD);        // 1
    if (pathPlugin.length())
       {cps_data = G_pCApp->PluginExe(this, pluginScript, CApp::endWait);
        G_pCApp->m_pCps->UnSerialize(cps_data);
        //CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli() + "test_ps.txt", ps_file);
        userPk = m_pCMoteurBase->GetUserPrimKeyFromNumOrdre(G_pCApp->m_pCps->m_NIR, "-sgn", &fieldList);
       }
    //.......................... si existe d\303\251j\303\240 le selectionner ..................................
    if (userPk.length())
       {if (G_pCApp->m_pCps->m_NIR.length())
           {user = fieldList[0];                           // m_USER_IDENT_LOGIN
            //m_CMDI_Identification->SetUser(user,"");
            G_pCApp->m_User            = user;
            G_pCApp->m_SignUser        = user;
            G_pCApp->m_CriptedPassWord = fieldList[1];     // m_USER_IDENT_PASSWORD
            G_pCApp->lireDroitsUtilisateurs();
            Slot_reinitManagerOnUser(G_pCApp->m_User, G_pCApp->m_SignUser);
           }
       }
    //.......................... sinon le cr\303\251er .........................................
    else if ( G_pCApp->m_pCps->m_NIR.length() && G_pCApp->m_pCps->m_Nom.length() && G_pCApp->m_pCps->m_Prenom.length())
       { ret = QMessageBox::information( this, tr("Utilisateur inconnu"),
                                               tr( "L'utilisateur : <b>?" + G_pCApp->m_pCps->m_Nom + "  " + G_pCApp->m_pCps->m_Prenom +
                                                   "?</b>de cette carte de praticien de sant\303\251<br/> n'est pas connu de MedinTux, <br/>" +
                                                   "faut-il cr\303\251er un nouvel utilisateur avec ?"
                                                  ) ,
                                               tr("&Cr\303\251er"), tr("&Annuler"), 0,
                                               1, 1 );
         if (ret==0)
            { editUser(cps_data);    // todo evaluer si au retour l'utilisateur est effac\303\251
            }
       }
   } // end else if (moduleName == "PIXVITALE")
#endif
   */
#ifdef SESAM_VERSION

    QString     userPk = "";
    QString     user = "";
    QString     pass = "";
    QString     moduleName = "";
    QString     cps_data = "";             // Cz_Pyxvital: d\303\251clarations sorties du if ...
    QStringList fieldList;                 // Cz_Pyxvital

    int ret;

QString pathPlugin, pathLog;
G_pCApp->readParam(  "Sesam-Vitale", "pathPlugin",       &pathPlugin);
G_pCApp->readParam(  "Sesam-Vitale", "pathLog",          &pathLog);
G_pCApp->readParam(  "Sesam-Vitale", "ModuleName",       &moduleName);

 if (moduleName == "PYXVITAL")
   {
    // Cz_Pyxvital ------------- DEBUT -----------------------------------------------------------------
    // Ex\303\251cution du programme Aixvitale avec le paramï¿½tre CPS (lecture carte CPS)
    // Aixvitale lancera lui mï¿½me la DLL de Pyxvital avec le paramï¿½tre P.
    // Aixvitale retourne le nom complet du fichier Praticien.par contenant les infos lues sur la carte
    QString     fic_PraticienPar = "";     // exemple de nom attendu : "C:/pyxvital/INTERF/Praticien.par";
    fieldList.append(m_pCMoteurBase->m_USER_IDENT_LOGIN);           // 0
    fieldList.append(m_pCMoteurBase->m_USER_IDENT_PASSWORD);        // 1

    fic_PraticienPar = appelPyxvital ( "m_Command = CPS\r\n");
    if (fic_PraticienPar.length())
       {
       // au retour de AIxvitale on traite le fichier Praticien.par pour r\303\251alimenter cps_data...
       QSettings *settings = new QSettings(fic_PraticienPar, QSettings::IniFormat);
       G_pCApp->m_pCps->m_TypePS               = settings->value(tr("PS/Type_carte")).toString();
       G_pCApp->m_pCps->m_TypeIdNat            = settings->value(tr("PS/ADELI")).toString();
       G_pCApp->m_pCps->m_NIR                  = settings->value(tr("PS/Num\303\251ro")).toString();
       G_pCApp->m_pCps->m_Clef_NIR             = settings->value(tr("PS/Cl\303\251")).toString();
       G_pCApp->m_pCps->m_Nom                  = settings->value(tr("PS/Nom")).toString();
       G_pCApp->m_pCps->m_Prenom               = settings->value(tr("PS/Pr\303\251nom")).toString();
       G_pCApp->m_pCps->m_CodeCivilite         = settings->value(tr("PS/Code_civilit\303\251")).toString();
       G_pCApp->m_pCps->m_SectActivite         = settings->value(tr("PS/Secteur_activit\303\251")).toString();
       G_pCApp->m_pCps->m_TypIdentStruct       = settings->value(tr("PS/Type_identif_structure")).toString();
       G_pCApp->m_pCps->m_NumIdentStruct       = settings->value(tr("PS/Num\303\251ro_identif_structure")).toString();
       G_pCApp->m_pCps->m_NumIdentStructClef   = settings->value(tr("PS/Cl\303\251_identif_structure")).toString();
       G_pCApp->m_pCps->m_RaisonSocialeStruct  = settings->value(tr("PS/Raison_sociale_structure")).toString();
       G_pCApp->m_pCps->m_CodeConvention       = settings->value(tr("PS/Code_convention")).toString();
       G_pCApp->m_pCps->m_CodeSpecialite       = settings->value(tr("PS/Code_sp\303\251cialit\303\251")).toString();
       G_pCApp->m_pCps->m_CodeZoneTarif        = settings->value(tr("PS/Code_zone_tarifaire")).toString();
       G_pCApp->m_pCps->m_CodeZoneIk           = settings->value(tr("PS/Code_zone_IK")).toString();
       G_pCApp->m_pCps->m_CodeAgr1             = settings->value(tr("PS/Code_agr\303\251ment_1")).toString();
       G_pCApp->m_pCps->m_CodeAgr2             = settings->value(tr("PS/Code_agr\303\251ment_2")).toString();
       G_pCApp->m_pCps->m_CodeAgr3             = settings->value(tr("PS/Code_agr\303\251ment_3")).toString();
       G_pCApp->m_pCps->m_HabilitationSignFact = settings->value(tr("PS/Habilitation_Facture")).toString();
       G_pCApp->m_pCps->m_HabilitationSignLot  = settings->value(tr("PS/Habilitation_Lot")).toString();
       cps_data = G_pCApp->m_pCps->Serialize();

       userPk = m_pCMoteurBase->GetUserPrimKeyFromNumOrdre(G_pCApp->m_pCps->m_NIR, "-sgn", &fieldList);
       }
   } // fin if PYXVITAL
   // Cz_Pyxvital ------------------------ FIN -----------------------------------------------------
else   // cas standard QFSEVITALE
   {
    //..................................... ne pas rajouter "/" car c'est un nom de fichier ..........................
    if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanDirPath (pathPlugin.prepend(G_pCApp->pathAppli()));
    //..................................... rajouter "/" car le chemin doit ï¿½tre donn\303\251 avec ..........................
    if ( QDir::isRelativePath ( pathLog ) )     pathLog    = QDir::cleanDirPath (pathLog.prepend(G_pCApp->pathAppli()) )+"/";

    QString pluginScript  = "[Execute]\r\n";
            pluginScript += "pathPlugin="      + pathPlugin                                           + "\r\n" +
                            "pathIni=****\r\n" +                //**** car indique d'utiliser le pathIni du plugin
                            "maskExch=Bonjour tout le monde\r\n"
                            "[Parametres]\r\n"
                            "m_Command       = READ_CPS\r\n"
                            "m_PathLog       =" + pathLog                                             + "\r\n" +
                            "m_DateConsult   =" + QDate::currentDate().toString("yyyyMMdd");
    //.............. liste des champs a extraire .......................................
    //               lors recherche de l'utilisateur indique par la Carte CPS
    // QString     cps_data = "";              // Cz_Pyxvital : d\303\251claration sortie du if
    // QStringList fieldList;                  // Cz_Pyxvital : d\303\251claration sortie du if
    fieldList.append(m_pCMoteurBase->m_USER_IDENT_LOGIN);           // 0
    fieldList.append(m_pCMoteurBase->m_USER_IDENT_PASSWORD);        // 1
    if (pathPlugin.length())
       {cps_data = G_pCApp->PluginExe(this, pluginScript, CApp::endWait);
        G_pCApp->m_pCps->UnSerialize(cps_data);
        //CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli() + "test_ps.txt", ps_file);
        userPk = m_pCMoteurBase->GetUserPrimKeyFromNumOrdre(G_pCApp->m_pCps->m_NIR, "-sgn", &fieldList);
       }
    } // end else Cas QFSEVITALE . Cz_Pyxvital ... La suite est commune .

    //.......................... si existe d\303\251j?  le selectionner ..................................
    if (userPk.length())
       {if (G_pCApp->m_pCps->m_NIR.length())
           {user = fieldList[0];                           // m_USER_IDENT_LOGIN
            //m_CMDI_Identification->SetUser(user,"");
            G_pCApp->m_User            = user;
            G_pCApp->m_SignUser        = user;
            G_pCApp->m_CriptedPassWord = fieldList[1];     // m_USER_IDENT_PASSWORD
            G_pCApp->lireDroitsUtilisateurs();
            Slot_reinitManagerOnUser(G_pCApp->m_User, G_pCApp->m_SignUser);
            //CZ_Cpta deb
            reinitAgendaOnUser(G_pCApp->m_User , G_pCApp->m_SignUser);
            setInterfaceOnProfil(MODE_SELECTION_PATIENT);
            m_pGUI->tabWidget_Central->setEnabled(TRUE);
            m_pGUI->wdg_DockWidget_Agenda->setEnabled(TRUE);
            //CZ_Cpta fin
           }
       }
    //.......................... sinon le cr\303\251er .........................................
    else if ( G_pCApp->m_pCps->m_NIR.length() && G_pCApp->m_pCps->m_Nom.length() && G_pCApp->m_pCps->m_Prenom.length())
       { ret = QMessageBox::information( this, tr("Unknown user"),
                                               tr( "L'utilisateur : <b> ?" + G_pCApp->m_pCps->m_Nom + "  " + G_pCApp->m_pCps->m_Prenom +
                                                   "?</b> de cette carte de praticien de sant\303\251<br/> n'est pas connu de MedinTux, <br/>" +
                                                   "faut-il cr\303\251er un nouvel utilisateur avec ?"
                                                  ) ,
                                               tr("&Cr\303\251er"), tr("&Annuler"), 0,
                                               1, 1 );
         if (ret==0)
            { editUser(cps_data);    // todo evaluer si au retour l'utilisateur est effac\303\251
            }
       }
   // } // end else if (moduleName == "PIXVITALE") Cz_Pyxvital .  fin du else remont\303\251e pour r\303\251cup code commun
#endif

}

//------------------------ Slot_UserSelected_InListUser ---------------------------------------
void C_Manager::Slot_UserSelected_InListUser(QTreeWidgetItem *pUserItem , QTreeWidgetItem *pSignUserItem)
{   G_pCApp->m_User     = pUserItem->text(0);
   G_pCApp->m_SignUser = pSignUserItem->text(0);
   // G_pCApp->lireDroitsUtilisateurs(); deja fait dans l'autentification

   reinitAgendaOnUser(G_pCApp->m_User , G_pCApp->m_SignUser);
   setInterfaceOnProfil(MODE_SELECTION_PATIENT);
   m_pGUI->tabWidget_Central->setEnabled(TRUE);
   m_pGUI->wdg_DockWidget_Agenda->setEnabled(TRUE);
   replaceInitialDockVisibility();
   setMedWebTuxOnUser();
   //addUserAgenda(G_pCApp->m_SignUser, QDate::currentDate());
   //............ ATTENTION AUX CONNEXIONS CIRCULAIRES .............
   // reinitListUserOnUser(G_pCApp->m_User , G_pCApp->m_SignUser); surtout pas car connexion circulaire !! m'a fait perdre une journ\303\251e ce truc
   // ==> ne pas appeler les trucs suivant :  Slot_reinitManagerOnUser
}

//------------------------ reinitListUserOnUser ---------------------------------------
void C_Manager::reinitListUserOnUser(const QString &user , const QString &signUser)
{  if (m_pC_Frm_UserList==0) return;
   m_pC_Frm_UserList->reinitOn_SignUser_User_Name(user, signUser);
}
//------------------------ replaceInitialDockVisibility ---------------------------------------
void C_Manager::replaceInitialDockVisibility()
{ if (m_AgendaInitialVisbility==1) m_pGUI->wdg_DockWidget_Agenda->show();
 if (m_AgendaInitialVisbility==0) m_pGUI->wdg_DockWidget_Agenda->hide();
 m_AgendaInitialVisbility   = 2;  // cet etat ne doit fonctionner qu'au debut lors login
 if (m_UserListInitialVisbility==1)  m_pGUI->wdg_DockWidget_User->show();
 if (m_UserListInitialVisbility==0)  m_pGUI->wdg_DockWidget_User->hide();
 m_UserListInitialVisbility = 2;  // cet etat ne doit fonctionner qu'au debut lors login
}
//------------------------ reinitManagerOnUser ---------------------------------------
void C_Manager::Slot_reinitManagerOnUser(QTreeWidgetItem *pUserItem , QTreeWidgetItem *pSignUserItem)
{   //............. prendre note des nouveaux utilisateurs .......................................
   Slot_reinitManagerOnUser(pUserItem->text(0) , pSignUserItem->text(0));
}

//------------------------ reinitManagerOnUser ---------------------------------------
void C_Manager::Slot_reinitManagerOnUser(const QString &user , const QString &signUser)
{reinitAgendaOnUser(user , signUser);
reinitListUserOnUser(user , signUser);
G_pCApp->m_User            = user;
G_pCApp->m_SignUser        = signUser;
G_pCApp->lireDroitsUtilisateurs();
setInterfaceOnProfil(MODE_SELECTION_PATIENT);
replaceInitialDockVisibility();
setMedWebTuxOnUser();
}

////////////////////////////////////////////// POSITIONS DES FENETRES ////////////////////////////////////////////////////////
//------------------------ Slot_actionRecordWindowsPos_triggered ---------------------------------------
void C_Manager::Slot_actionRecordWindowsPos_triggered(bool)
{Slot_RecordPos();
}
//------------------------ Slot_actionHideShowLogo_triggered ---------------------------------------
void C_Manager::Slot_actionHideShowLogo_triggered (bool)
{ QString AfficheLogo;
  if (m_pGUI->textLabelPixMap->isShown())
    {m_pGUI->textLabelPixMap->hide();
     AfficheLogo = "NON";
    }
 else
    {m_pGUI->textLabelPixMap->show();
     AfficheLogo = "OUI";
    }
 G_pCApp->writeParam(  "Agenda" , "Affichage logo Data Medical Design" , AfficheLogo);  // CZA
 G_pCApp->updateIniParamToDisk( ); // CZA
}

//------------------------ Slot_actionSetGlobalFont_triggered ---------------------------------------
void C_Manager::Slot_actionSetGlobalFont_triggered (bool)
{   //bool ok=FALSE;
   QFontDialog fdlg(G_pCApp->m_GuiFont, this);
   fdlg.setOption ( QFontDialog::DontUseNativeDialog ,TRUE );
   fdlg.setCurrentFont ( G_pCApp->m_GuiFont );
   fdlg.setCaption(tr("What is the police doing ?"));
   fdlg.setModal ( TRUE );
   //fdlg.setWindowFlags ((fdlg.windowFlags()&~(Qt::WindowCloseButtonHint|Qt::Dialog)|));
   if (fdlg.exec()==QDialog::Accepted)
   //QFont font = QFontDialog::getFont( &ok, G_pCApp->m_GuiFont, this, tr("Mais que fait la police ?"));
   // QFontDialog::getFont ( bool * ok, const QFont & initial, QWidget * parent, const QString & title, FontDialogOptions options )
   //if (ok )
      {/*
       Family = Arial
       Italic = 0
       Underline = 0
       PointSize = 10
       Weight = 50
       */
       QFont font = fdlg.selectedFont ();
       G_pCApp->writeParam(  "Font", "Family",     font.family() );
       G_pCApp->writeParam(  "Font", "PointSize",  QString::number(font.pointSize()) );
       G_pCApp->writeParam(  "Font", "Italic",     QString::number(font.italic()) );
       G_pCApp->writeParam(  "Font", "Underline",  QString::number(font.underline()) );
       G_pCApp->writeParam(  "Font", "Weight",     QString::number(font.weight()) );
       G_pCApp->m_GuiFont = font;
       setAllWidgetsOnFont(font);
       QApplication::setFont(font);
       G_pCApp->updateIniParamToDisk( );
      }
}
//---------------------------------------------- Slot_Type_Affichage_Change ------------------------------------------------------------
void C_Manager::Slot_Type_Affichage_Change()
{   if ( m_Type_Affichage_EnCours != "AGENDA" )
       { m_Type_Affichage_EnCours = "AGENDA";
       }
    else
       { m_Type_Affichage_EnCours = "FICHE";
       }
    get_RecordDispos(m_Type_Affichage_EnCours);
    G_pCApp->writeParam(  "Agenda", "Affichage Liste ou Agenda",    m_Type_Affichage_EnCours );
    G_pCApp->updateIniParamToDisk( );
}
//CZA
//---------------------------------------------- get_RecordDispos ------------------------------------------------------------
void C_Manager::get_RecordDispos(const QString & mode /* = "FICHE" */)
{   QString nomFicDPS     = "";
    if (mode!="AGENDA")
       {m_Type_Affichage_EnCours = "FICHE";
        nomFicDPS                = "Manager.dps";
       }
    else
       {m_Type_Affichage_EnCours = mode;
        nomFicDPS                = "Manager_Agenda.dps";
       }
    SetModeOn_Button_Affichage_EnCours(m_Type_Affichage_EnCours);
    //...... charge la config de presentation .................
    if ( !QFile::exists(G_pCApp->pathAppli() + nomFicDPS) )  return;
    QFile file( G_pCApp->pathAppli() + nomFicDPS );
    if ( !file.open( QIODevice::ReadOnly ) )                 return;
    restoreState (file.readAll());
    file.close();
}
//---------------------------------------------- SetModeOn_Button_Affichage_EnCours ------------------------------------------------------------
void C_Manager::SetModeOn_Button_Affichage_EnCours(const QString &mode)
{   QString textToDisplay = "";
    if (mode=="FICHE")
       {textToDisplay            = m_strGotoAgenda;
       }
    else
       {textToDisplay            = m_strGotoPatientList;
       }
    //...... charge la config de presentation .................
    //QRect rect = QFontMetrics ( this->font() ).boundingRect ( textToDisplay );
    QFontMetrics fntMetric( G_pCApp->m_GuiFont );
    QRect r1 = fntMetric.boundingRect(m_strGotoAgenda);
    QRect r2 = fntMetric.boundingRect(m_strGotoPatientList);
    int               wbut = qMax( r1.width(),  r2.width() )   + 20;
    int               hbut = qMax( r1.height(), r2.height() )  + 6;
    m_Button_Affichage_EnCours->setText(textToDisplay);
    m_Button_Affichage_EnCours->setGeometry(m_Button_Affichage_EnCours->x(),m_Button_Affichage_EnCours->y(),wbut, hbut);
}

//---------------------------------------------- Slot_RecordDispos ------------------------------------------------------------
void C_Manager::Slot_RecordDispos()
{   QString nomFicDPS = "Manager.dps";              // CZA
    if (m_Type_Affichage_EnCours == "AGENDA")       // CZA
        nomFicDPS = "Manager_Agenda.dps";           // CZA
    //................... enregistrer la config ..................
    QFile file( G_pCApp->pathAppli() + nomFicDPS ); // CZA
    if ( !file.open( QIODevice::WriteOnly ) )        return;
    file. write (saveState());
    file.close();
}

//---------------------------------------------- Slot_RecordPos ------------------------------------------------------------
void C_Manager::Slot_RecordPos()
{
   int x  =  this->x();
   int y  =  this->y();
   int w  =  this->width();
   int h  =  this->height();
   G_pCApp->writeParam(  "ManagerPos", "Positions",
                                 QString::number(x),      QString::number(y),
                                 QString::number(w),      QString::number(h));
   G_pCApp->updateIniParamToDisk( );
   Slot_RecordDispos();
}

//////////////////////////////// GESTION DU STYLE DES MODES (CREATION SELECTION MULTICRITERE) ////////////////////////////////////////////////////////
//---------------------------------------------- getBoxStyle -----------------------------------------------------------------------------------------
QString C_Manager::getBoxStyle(int mode)
{ QString folderDefStyle  = "";

 #ifdef Q_WS_WIN
    folderDefStyle = G_pCApp->pathAppli()+"Ressources/Win/BoxStyle_";
 #endif
 #ifdef Q_WS_MAC
    folderDefStyle = G_pCApp->pathAppli()+"Ressources/Mac/BoxStyle_";
 #endif
 #ifdef Q_WS_X11
    folderDefStyle = G_pCApp->pathAppli()+"Ressources/Lin/BoxStyle_";
 #endif
 QString style  = CGestIni::Param_UpdateFromDisk(folderDefStyle + m_List_GUI_Mode[mode]+".css");
 int pointSize  = G_pCApp->m_GuiFont.pointSize();
 if (style.length()!=0) return style.arg(pointSize);
 switch (mode)
    {default :
     case MODE_SELECTION_PATIENT:
          return QString("QGroupBox {"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
                              "border: 1px solid gray;"
                              "border-radius: 5px;"
                              "font-size: %1px;"
                              "font-weight: bold;"
                              "padding-top: 7ex;"    // marge int\303\251rieure au cadre pour placer le premier objet
                              "margin-top: 1ex;} "   // marge entre le cadre et le haut de l'objet
                 "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "subcontrol-position: top left;" /* position at the top center */
                              "left: 20px;"             // ofset de deplacement gauche
                              "padding-top: 2px;"       // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-left: 10px;"     // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-right: 10px;"    // marge int\303\251rieure droite au cadre pour placer le premier objet (texte)
                              "border-radius: 3px;"
                              "border: 1px solid gray;"
                              //"border-bottom-style: none;"
                              //"border-style: none;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8e8b8d, stop: 1 #FFFFFF);}").arg(pointSize);
     case MODE_CREATION_PATIENT:
          return QString("QGroupBox {"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6976f7, stop: 1 #FFFFFF);"
                              "border: 1px solid gray;"
                              "border-radius: 5px;"
                              "font-size: %1px;"
                              "font-weight: bold;"
                              "padding-top: 7ex;"    // marge int\303\251rieure au cadre pour placer le premier objet
                              "margin-top: 1ex;} "   // marge entre le cadre et le haut de l'objet
                 "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "subcontrol-position: top left;" /* position at the top center */
                              "left: 20px;"             // ofset de deplacement gauche
                              "padding-top: 2px;"       // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-left: 10px;"     // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-right: 10px;"    // marge int\303\251rieure droite au cadre pour placer le premier objet (texte)
                              "border-radius: 3px;"
                              "border: 1px solid gray;"
                              //"border-bottom-style: none;"
                              //"border-style: none;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4b55b0, stop: 1 #FFFFFF);}").arg(pointSize);
     case MODE_MULTICRITERE:
          return QString("QGroupBox {"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ff743e, stop: 1 #FFFFFF);"
                              "border: 1px solid gray;"
                              "border-radius: 5px;"
                              "font-size: %1px;"
                              "font-weight: bold;"
                              "padding-top: 7ex;"    // marge int\303\251rieure au cadre pour placer le premier objet
                              "margin-top: 1ex;} "   // marge entre le cadre et le haut de l'objet
                 "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "subcontrol-position: top left;" /* position at the top center */
                              "left: 20px;"             // ofset de deplacement gauche
                              "padding-top: 2px;"       // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-left: 10px;"     // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-right: 10px;"    // marge int\303\251rieure droite au cadre pour placer le premier objet (texte)
                              "border-radius: 3px;"
                              "border: 1px solid gray;"
                              //"border-bottom-style: none;"
                              //"border-style: none;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #af512d, stop: 1 #FFFFFF);}").arg(pointSize);
      }

}

//---------------------------------------------- getTabStyle ------------------------------------------------------------
QString C_Manager::getTabStyle(int mode)
{ QString folderDefStyle  = "";
#ifdef Q_WS_WIN
   folderDefStyle = G_pCApp->pathAppli()+"Ressources/Win/TabStyle_";
#endif
#ifdef Q_WS_MAC
   folderDefStyle = G_pCApp->pathAppli()+"Ressources/Mac/TabStyle_";
#endif
#ifdef Q_WS_X11
   folderDefStyle = G_pCApp->pathAppli()+"Ressources/Lin/TabStyle_";
#endif
QString style  = CGestIni::Param_UpdateFromDisk(folderDefStyle + m_List_GUI_Mode[mode]+".css");
int pointSize  = G_pCApp->m_GuiFont.pointSize();
if (style.length()!=0) return style.arg(pointSize);
 switch (mode)
    {default :
     case MODE_CREATION_PATIENT:
          return QString("QGroupBox {"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6976f7, stop: 1 #FFFFFF);"
                              "border: 1px solid gray;"
                              "border-radius: 5px;"
                              "font-size: %1px;"
                              "font-weight: bold;"
                              "padding-top: 2ex;"    // marge int\303\251rieure au cadre pour placer le premier objet
                              "margin-top: 1ex;} "   // marge entre le cadre et le haut de l'objet
                 "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "subcontrol-position: top left;" /* position at the top center */
                              "left: 20px;"             // ofset de deplacement gauche
                              "padding-top: 2px;"       // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-left: 10px;"     // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-right: 10px;"    // marge int\303\251rieure droite au cadre pour placer le premier objet (texte)
                              "border-radius: 3px;"
                              "border: 1px solid gray;"
                              //"border-bottom-style: none;"
                              //"border-style: none;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4b55b0, stop: 1 #FFFFFF);}").arg(pointSize);
     case MODE_MULTICRITERE:
          return QString("QGroupBox {"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ff743e, stop: 1 #FFFFFF);"
                              "border: 1px solid gray;"
                              "border-radius: 5px;"
                              "font-size: %1px;"
                              "font-weight: bold;"
                              "padding-top: 2ex;"    // marge int\303\251rieure au cadre pour placer le premier objet
                              "margin-top: 1ex;} "   // marge entre le cadre et le haut de l'objet
                 "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "subcontrol-position: top left;" /* position at the top center */
                              "left: 20px;"             // ofset de deplacement gauche
                              "padding-top: 2px;"       // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-left: 10px;"     // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-right: 10px;"    // marge int\303\251rieure droite au cadre pour placer le premier objet (texte)
                              "border-radius: 3px;"
                              "border: 1px solid gray;"
                              //"border-bottom-style: none;"
                              //"border-style: none;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #af512d, stop: 1 #FFFFFF);}").arg(pointSize);;
     case MODE_SELECTION_PATIENT:
          return QString("QGroupBox {"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E0E0E0, stop: 1 #FFFFFF);"
                              "border: 1px solid gray;"
                              "border-radius: 5px;"
                              "font-size: %1px;"
                              "font-weight: bold;"
                              "padding-top: 2ex;"    // marge int\303\251rieure au cadre pour placer le premier objet
                              "margin-top: 1ex;} "   // marge entre le cadre et le haut de l'objet
                 "QGroupBox::title {"
                              "subcontrol-origin: margin;"
                              "subcontrol-position: top left;" /* position at the top center */
                              "left: 20px;"             // ofset de deplacement gauche
                              "padding-top: 2px;"       // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-left: 10px;"     // marge int\303\251rieure gauche au cadre pour placer le premier objet (texte)
                              "padding-right: 10px;"    // marge int\303\251rieure droite au cadre pour placer le premier objet (texte)
                              "border-radius: 3px;"
                              "border: 1px solid gray;"
                              //"border-bottom-style: none;"
                              //"border-style: none;"
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #8e8b8d, stop: 1 #FFFFFF);}").arg(pointSize);;

      }
}
//Cz_Pyxvital ------------------------------- DEBUT -----------------------------------------------------
/************************************************************************ PYX_11_2014
//----------------------------------- Slot_saisieFSEenCours -------------------------------------------------------------------------
// Contrï¿½le si la saisie FSE par PYxvital est en cours .
// --> test de la pr\303\251sence du fichier Facture.par
// --> si seulement un acte a \303\251t\303\251 saisi, le fichier Facture.par contient un groupe "Prestation" ?? PAS SUR
// --> si la FSE est valid\303\251e le fichier Facture.par contient le groupe "Tarification" avec un "Taux".
// Contrï¿½le si le patient en cours dans Pyxvital est bien le patient s\303\251lectionn\303\251 dans Manager
void C_Manager::Slot_saisieFSEenCours()
{   bool finFSE = false;
   QString nomficErreur = m_ficFacturePar;                      // CZ_Cpta
   nomficErreur = nomficErreur.replace("Facture","Erreurs");
   if (QFile::exists(nomficErreur) )                           // il y a eu une erreur ou un Annuler dans Pyxvital
       {finFSE = true;
       }
   if (QFile::exists(m_ficFacturePar) )
       {QSettings *settings = new QSettings(m_ficFacturePar, QSettings::IniFormat);
       QString    numfact = settings->value(tr("Tarification/Taux")).toString();
       if (numfact.length() > 0)                               // la saisie est terminï¿½e : La FSE a ete validee.
         {finFSE = true;
         }
       else                                                  // au moins une ligne d'acte a ete saisie : La saisie est en cours.
         {m_FSEenCours = TRUE;
         m_pGUI->pushButton_FSE->setText("Annuler la facture en cours") ;
         }
       }
   if (finFSE)
       {           // soit : la FSE a ete validee.
                   // soit : on a saisit un autre patient dans Pyxvital >> ce qui est interdit.
                   // soit on a reselectionne un autre patient dans medintux sans saisiz d'actes dans Pyxvital
       m_FSEenCours = FALSE;
       m_pGUI->pushButton_FSE->setText(tr("Facturer"));
       m_pGUI->pushButton_FSE->setEnabled (FALSE );            // dï¿½sactiver ce bouton en plus ????
       m_timerFSE->stop();                                     // arr??ï¿½t du timer d'attente du fichier Facture.par
       QTreeWidget *pQlistView = m_pGUI->listView_Vitale;
       pQlistView->clear();
       Controle_Solde_Patient(G_pCApp->identitePatientGUID());                       // CZ_Cpta

       } // fin du FSE validï¿½e
}
****************************************************************************************/ // PYX_11_2014


//--------------------------------- Slot_pushButton_FSE -----------------------------------------------------------------------
// Lancement de Pyxvital pour saisie des actes et validation des FSE .
// Pour simplifier la proc\303\251dure :
// - manager g\303\251nï¿½re syst\303\251matiquement un fichier CV.par contenant toutes les infos du patient (+ les donn\303\251es AMC ?, AMO ?)
// - c'est Aixvitale qui testera si la CV a \303\251t\303\251 lue ...
// - si CV OK avec patient en cours, c'est la CV qui est prise en compte.
// - si pas de CV et D\303\251grad\303\251 OK , c'est CV.par qui sera pris par Pyxvital.
void C_Manager::Slot_pushButton_FSE()
{
#ifdef SESAM_VERSION
   QString zcvdns ;
   QString zcvnss ;
   QString zcvqual ;
   QString zcvrang ;
   QString zcvnom ;
   QString zcvprn ;
   QString numSecu;
   QString rangGem;
   QString qualite;

   QString moduleName;
   QString interf_Pyx;

   G_pCApp->readParam(  "Sesam-Vitale", "ModuleName",       &moduleName);
   G_pCApp->readParam(  "Sesam-Vitale", "Interface_module", &interf_Pyx);

  if (moduleName=="PYXVITAL")    // test inutile
     {
      // vï¿½rification qu'une FSE n'est pas dï¿½j??  en cours de saisie
      if (m_FSEenCours)
         {
          int ret    = QMessageBox::warning ( this, tr("FSE en cours"),
                       tr( "<u><b>ATTENTION :</u></b><br><br>Une FSE est en cours de saisie !<br>Voulez vous abandonner la FSE en cours ?"),
                       tr("&Abandonner la FSE en cours"), tr("Annu&ler"), 0, 1, 1);
          if (ret == 1) return;
          // voir comment effacer le formulaire Pyxvital si acte dï¿½j??  saisi...
          // la lecture de la CV remet tout a zï¿½ro...
         }

      // cherche si le patient sï¿½lectionnï¿½ dans la liste patient est bien dans la liste Vitale
      QTreeWidgetItemIterator it( m_pGUI->listView_Vitale );
      bool patientOK = false;
      m_typFact = "";
         while ( *it )
            {
               QTreeWidgetItem *pQListViewItem = *it;
               zcvnss   = pQListViewItem->text(3);
               zcvrang  = pQListViewItem->text(4);
               if (zcvrang.length()== 1) zcvrang = zcvrang.prepend("0");
               zcvqual  = pQListViewItem->text(5);
               zcvnom   = pQListViewItem->text(0);
               zcvprn   = pQListViewItem->text(1);

               numSecu  = m_pGUI->lineEdit_NumSecu->text(); numSecu.remove(" ");
               if (  zcvnss  ==  numSecu &&
                     zcvqual == qualite.setNum(m_pGUI->comboBoxQualiteAyantDroit->currentIndex())  &&
                     zcvrang == m_pGUI->lineEdit_RangGeme->text() &&
                     zcvnom  == m_pGUI->lineEdit_NomDossier->text() &&
                     zcvprn  == m_pGUI->lineEdit_PrenomDossier->text()
                     // Voir si tester la date de naissance ?????
                  )
                  { patientOK = true ;
                    m_typFact = "FSE";
                    break;
                  }
               ++it;
            }
     if (!patientOK)
         {
         // PAs de FSE possible car pas de CV -- on propose un autre choix !
         // Debut CZ_Cpta
         QMessageBox msgBox;
         msgBox.setText(tr("<b>La carte Vitale n'a pas \303\251t\303\251 lue pour ce patient !<br><br></b>"
                             "Que voulez-vous faire : <br><br>"
                             "- Annuler et lire la carte Vitale,<br>"
                             "- Continuer et saisir une feuille de soin en mode D\303\251grad\303\251,<br>"
                             "- Continuer et saisir une feuille de soin papier ,<br>"
                             "- Continuer et saisir une facture.<br>"));
         msgBox.addButton(tr("Cancel"),                 QMessageBox::HelpRole);
         msgBox.addButton(tr("Mode d\303\251grad\303\251"),            QMessageBox::HelpRole);
         msgBox.addButton(tr("Saisir une FSP"),          QMessageBox::HelpRole);
         msgBox.addButton(tr("Saisir une facture"),      QMessageBox::HelpRole);
         QFont ft("sans",10,0,0);
         msgBox.setFont(ft);
         msgBox.window()->setWindowTitle("Choix du type de facturation");
         int ret = msgBox.exec();
         if ( ret == 0) return;
         if ( ret == 1) m_typFact = "DEG";
         if ( ret == 2) m_typFact = "FSP";
         if ( ret == 3) m_typFact = "PFA";
         // fin CZ_Cpta

         zcvnom   = m_pGUI->lineEdit_NomDossier->text();
         zcvprn   = m_pGUI->lineEdit_PrenomDossier->text();

         // Crï¿½ation du fichier CV.par ...................................................................
         // Le fichier CV.Par ne sert que pour le mode Dï¿½gradï¿½, les FSP ou les prï¿½factures.
         // les infos fournies sont donc celles de la base Medintux et non pas celles de la Carte SV
         QString nomFicCVpar = interf_Pyx + "/CV.par";
         remove (nomFicCVpar);
         QSettings *settings = new QSettings(nomFicCVpar, QSettings::IniFormat);

         settings->beginGroup("Beneficiaire");
         settings->setValue("Nom",               m_pGUI->lineEdit_NomDossier->text());
         settings->setValue("Prenom",            m_pGUI->lineEdit_PrenomDossier->text());
         zcvqual.setNum(m_pGUI->comboBoxQualiteAyantDroit->currentIndex());
         if (zcvqual.length() == 1) zcvqual.prepend("0");
         settings->setValue("Code_qualite",      zcvqual);
         zcvdns = m_pGUI->lineEdit_DtNss->text();   zcvdns.replace(2,1,"/");  zcvdns.replace(5,1,"/");
         settings->setValue("date_de_naissance", zcvdns);
         zcvrang = m_pGUI->lineEdit_RangGeme->text();
         zcvrang.setNum(zcvrang.toInt());
         settings->setValue("Rang_gemellaire", zcvrang);                        // CZ_Cpta
         settings->endGroup();

         settings->beginGroup("Assure");
         settings->setValue("Nom",               m_pGUI->lineEdit_NomAssure->text());
         settings->setValue("Prenom",            m_pGUI->lineEdit_PrenomAssure->text());
         zcvnss = m_pGUI->lineEdit_NumSecu->text(); zcvnss.remove(" ");
         settings->setValue("Numero",            zcvnss);
         settings->setValue("Cle",               m_pGUI->lineEdit_NssClef->text());
         // pas de date de naissance de l'assure
         settings->endGroup();
         settings->sync();
         // ajouter la saisie et le stockage des champs dans les onglets AMC - AMO
         // pas obligatoire car on peut les saisir dans Pyxvital
         /*
         QString m_codeRegime;
         QString m_caisseGestionnaire;
         QString m_centreGestionnaire;
         m_codeRegime = "01";
         m_caisseGestionnaire = "999";
         m_centreGestionnaire = "9999";
         settings->beginGroup("Medico_administratif");
         settings->setValue("Code_regime",         m_codeRegime);
         settings->setValue("Caisse_gestionnaire", m_caisseGestionnaire);
         settings->setValue("Centre_gestionnaire", m_centreGestionnaire);
         settings->endGroup();
         */
         }

         // appel ??  AixVitale (qui lance Pyxvital) pour saisie des actes et generation de la FSE.
         //--- dï¿½but CZ_Cpta

         QString numPS = G_pCApp->m_pCps->m_NIR.length() > 6 ? G_pCApp->m_pCps->m_NIR : "PasLu";
         QString listParam =  "p_actionPyx =" + m_typFact                                               + "\r\n"
                              "p_logUser ="   + G_pCApp->m_User                                         + "\r\n"
                              "m_typFact ="   + m_typFact                                               + "\r\n"
                              "p_numSecu ="   + zcvnss                                                  + "\r\n"
                              "p_qualite ="   + zcvqual                                                 + "\r\n"
                              "p_rangGem ="   + zcvrang                                                 + "\r\n"
                              "p_nom ="       + zcvnom.replace("'"," ")                                 + "\r\n"
                              "p_prenom ="    + zcvprn.replace("'"," ")                                 + "\r\n"
                              "p_datnss ="    + m_pGUI->lineEdit_DtNss->text()                          + "\r\n"
                            //"p_datnss ="    + m_pGUI->lineEdit_DtNss_Assure->text()                   + "\r\n"
                              "p_GUIDPat ="   + G_pCApp->identitePatientGUID()                          + "\r\n"
                              "p_PKPat ="     + G_pCApp->identitePatientPk()                            + "\r\n"
                              "p_NumPS ="     + numPS                                                   + "\r\n"
                              "p_NomAssu ="   + m_pGUI->lineEdit_NomAssure->text().replace("'"," ")     + "\r\n"
                              "p_PreAssu ="   + m_pGUI->lineEdit_PrenomAssure->text().replace("'"," ")  + "\r\n";
         //--- fin CZ_Cpta
         m_ficFacturePar = appelPyxvital ( listParam);
        if (m_ficFacturePar.length())
           {
            // AU retour :
            // - soit on a pu lancer Pyxvital, et on a le nom du futur fichier Facture.par en retour.
            //   on boucle sur le test de la presence de ce fichier pour le traiter...
            // - soit on a le mot "Erreur" avec le message d'erreur.
             if (strncmp (m_ficFacturePar,"Erreur",6) != 0 )
                 {
                 if (m_timerFSE == 0)
                     { m_timerFSE = new QTimer(this);
                     connect(m_timerFSE, SIGNAL(timeout()), this, SLOT(Slot_saisieFSEenCours()));
                     }
                 if (!m_timerFSE->isActive())
                     m_timerFSE->start(1000);
                 }
            }
    } // fin if PYXVITAL
#endif
}
//-------------------------------------- appelPyxvital -------------------------------------
QString C_Manager::appelPyxvital (QString listeParametres)
{
   QString pathPlugin, pathLog, pluginScript;
   G_pCApp->readParam(  "Sesam-Vitale", "pathPlugin",       &pathPlugin);
   G_pCApp->readParam(  "Sesam-Vitale", "pathLog",          &pathLog);

   if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanDirPath (pathPlugin.prepend(G_pCApp->pathAppli()));
   if ( QDir::isRelativePath ( pathLog ) )     pathLog    = QDir::cleanDirPath (pathLog.prepend(G_pCApp->pathAppli()) )+"/";

   if (pathPlugin.length())
       {
        pluginScript  = "[Execute]\r\n";
        pluginScript += "pathPlugin="      + pathPlugin    + "\r\n" +
                   "pathIni=****\r\n" +                   //**** car indique d'utiliser le pathIni local du plugin
                   "maskExch=Bonjour tout le monde\r\n"
                   "[Parametres]\r\n" +
                   listeParametres;

   return (G_pCApp->PluginExe(this, pluginScript, CMoteurBase::endWait));
   }
   return QString::null;
}
//--------------------------------- Slot_Button_Vitale_ContextMenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_Button_Vitale_ContextMenuRequested (const QPoint &)
{Slot_Button_Vitale_MenuRequested();
}
//--------------------------------- Slot_Button_Vitale_MenuRequested -----------------------------------------------------------------------
void C_Manager::Slot_Button_Vitale_MenuRequested()
{

QStringList list;
list.append(tr("=1=Formater les fichiers et T\303\251l\303\251transmettre"));            // CZ_Cpta
list.append(tr("=2=T\303\251l\303\251transmission seule"));                              // CZ_Cpta
list.append(tr("=3=D\303\251charger le TLA"));                                           // CZ_Cpta
list.append("----------");
list.append(tr("=4=Liste des recettes"));                                                // CZ_Cpta2
list.append("----------");
list.append(tr("=5=Lancer Pyxvital"));
list.append("----------");

//.................. actionner le menu et les actions .................................................
int option = G_pCApp->DoPopupList(list).toInt();
switch(option)
{  case 1: appelPyxvital ( "m_Action = FTL\r\n" "m_logUser ="  + G_pCApp->m_User + "\r\n");  break;
   case 2: appelPyxvital ( "m_Action = TEL\r\n" "m_logUser ="  + G_pCApp->m_User + "\r\n");  break;
   case 4: appelCompta   ("TDB");                                                            break;
   case 5: appelPyxvital ( "m_Action = PYX\r\n");                                            break;
   case 3: appelPyxvital ( "m_Action = TLA\r\n" "m_logUser ="  + G_pCApp->m_User + "\r\n");  break;
}
//#endif
}

//-----------------------------------------------Controle_Solde_Patient----------------------------------------
void C_Manager::Controle_Solde_Patient(QString guidPatient)
{
double  soldePatient = 0;
double  soldePayeur  = 0;
QString requete;
//QPixmap iconeSolde;
//QString localParam  = "";

//    CGestIni::Param_UpdateFromDisk(G_pCApp->pathIni(), localParam);
if (G_pCApp->readUniqueParam ( "Comptabilite", "Controle solde").toLower() != "oui")
       return;
   if (G_pCApp->IsThisDroitExist("sld"))
       m_pGUI->pushButton_solde->setVisible ( true );
   else
       {m_pGUI->pushButton_solde->setVisible ( false ); return; }

   if  (m_pCMoteurBase->OpenBase()==0)                                                                    return;
   requete =  "SELECT "
              " FE_GUIDPayeur, ROUND(FE_TotAssure,2), ROUND(FE_TotRegltASS,2), ROUND(FE_MtPP,2) "  // 0-1-2-3
              " FROM fact_entete "
              " WHERE (( FE_GUIDPatient        = '" + guidPatient + "' ) "
              " or     ( FE_GUIDPayeur         = '" + guidPatient + "' )) "
              " and    (ROUND(FE_TotAssure,2) != ROUND(FE_TotRegltASS + FE_MtPP,2)) "
              " and    (FE_Soldee             != 'S' )" ;
 QSqlQuery query(requete , m_pCMoteurBase->getDatabase() );
 if (query.isActive())
    {while (query.next())
        {if (query.value(0).toString().length() > 1 && query.value(0).toString() != guidPatient)
               soldePayeur   +=  query.value(1).toDouble() - query.value(2).toDouble() - query.value(3).toDouble();
            else
                soldePatient +=  query.value(1).toDouble() - query.value(2).toDouble() - query.value(3).toDouble();
        } // while (query.next())
    } //endif (query.isActive())
 m_pCMoteurBase->CloseBase();
if (soldePatient > 0)                 // affiche drapeau Rouge
       m_pGUI->pushButton_solde->setIcon ( Theme::getIcon("SoldeRouge.png") );

   else
       if (soldePayeur  > 0)           // affiche drapeau Orange
           m_pGUI->pushButton_solde->setIcon ( Theme::getIcon("SoldeOrange.png") );
       else
           m_pGUI->pushButton_solde->setIcon ( Theme::getIcon("SoldeVert.png") );
}

//------------------------------------Slot_Saisie_Reglement--------------------------------------------------
void C_Manager::Slot_Saisie_Reglement()
{
   #ifdef SESAM_VERSION
   QString localParam  = "";

   CGestIni::Param_UpdateFromDisk(G_pCApp->pathIni(), localParam);
   if (CGestIni::Param_ReadUniqueParam(localParam, "Comptabilite", "Saisie reglement") != "oui")
       return;

   if (!G_pCApp->IsThisDroitExist("rgl"))
       return;

   QString numPS = G_pCApp->m_pCps->m_NIR.length() > 6 ? G_pCApp->m_pCps->m_NIR : "PasLu";
   QString zcvrang = m_pGUI->lineEdit_RangGeme->text();
   zcvrang.setNum(zcvrang.toInt());
   QString zcvnss  = m_pGUI->lineEdit_NumSecu->text(); zcvnss.remove(" ");
   QString zcvqual;    zcvqual.setNum(m_pGUI->comboBoxQualiteAyantDroit->currentIndex());
   if (zcvqual.length() == 1) zcvqual.prepend("0");
       QString listParam =  "p_actionPyx =" "REG"                                      "\r\n"
                            "p_logUser ="   + G_pCApp->m_User                        + "\r\n"
                            "m_typFact ="   "XXX"                                      "\r\n"
                            "p_numSecu ="   + zcvnss                                 + "\r\n"
                            "p_qualite ="   + zcvqual                                + "\r\n"
                            "p_rangGem ="   + zcvrang                                + "\r\n"
                            "p_nom ="       + m_pGUI->lineEdit_NomDossier->text()    + "\r\n"
                            "p_prenom ="    + m_pGUI->lineEdit_PrenomDossier->text() + "\r\n"
                            "p_datnss ="    + m_pGUI->lineEdit_DtNss->text()         + "\r\n"
                            //"p_datnss ="    + m_pGUI->lineEdit_DtNss_Assure->text()  + "\r\n"
                            "p_GUIDPat ="   + G_pCApp->identitePatientGUID()         + "\r\n"
                            "p_PKPat ="     + G_pCApp->identitePatientPk()           + "\r\n"
                            "p_NumPS ="     + numPS                                  + "\r\n"
                            "p_NomAssu ="   + m_pGUI->lineEdit_NomAssure->text()     + "\r\n"
                            "p_PreAssu ="   + m_pGUI->lineEdit_PrenomAssure->text()  + "\r\n";

      appelPyxvital ( listParam);
   #endif
}
//-------------------------------------- appelTDBCpta -------------------------------------
void C_Manager::appelCompta (QString /*nomProgCPta*/)
{
   QString  pathExe;
   QStringList argList;

   G_pCApp->readParam(  "Comptabilite", "pathTDB", &pathExe);
   pathExe = QDir::cleanDirPath (QFileInfo (qApp->argv()[0]).dirPath (true) + "/" + pathExe);
   // argList <<  pathExe;
   argList << G_pCApp->m_User;                             // CZ_Cpta2
   #ifdef Q_WS_WIN
       pathExe +=".exe";
    //#else
    #endif
    QProcess::startDetached (pathExe, argList);
}
//Cz_Pyxvital ------------------------------------- FIN ---------------------------------------------------------

//-------------------------------------- Slot_launchSpecificJob -------------------------------------
void C_Manager::Slot_launchSpecificJob (bool)       // CZB
{
    G_pCApp->launchSpecificJob("Menu_Files");
}

//-------------------------------------- Slot_OnTimerEntrants -------------------------------------
// Mise a jour listView des entrants SIGEMS par timer
void C_Manager::Slot_OnTimerEntrants()
{
#ifdef ENTREES_SIGEMS
    GetEntreesSigemsList(m_pGUI->listView_Sigems  );
    GetOruList(m_pGUI->treeWidget_oru);
#endif
}
#ifdef ENTREES_SIGEMS
//-----------------------------------------------------  GetEntreesSigemsList -------------------------------------------
/*! \brief Remplit la QListView avec les entrees SIGEMS retrouvees dans la base de donnees.
 *  \param pQlistView : ListView qui recevra les patients recherches
 *  \param qsrt_nom : Nom a rechercher
 *  \param qsrt_prenom : Prenom a rechercher
 *  \param statusMess : message de retour
 *  \param errMess : Message d'erreur.
 *  \return nombre d'enregistrements inseres dans la QListView passee en paramÃ¨tre
*/
// on affiche dans la listVieW Sigems:
// toutes les entrees non prises en charge sur les dernieres 24h ??
// toutes les entrees prises en charge mais dans la main courante.
long C_Manager::GetEntreesSigemsList( QTreeWidget     *pQlistView  )
{
    if (G_pCApp->m_pCMoteurBase->OpenBase()==0)
        {return 0;
        }
      QString   requete;
      // calcul de l'heure debut d'affichage des entrees
      QString   sdatelim, sdatemax;
      QDateTime qdatelim = QDateTime::currentDateTime();
                sdatelim = qdatelim.toString("yyyy-MM-dd hh:mm:ss");
      // si une heure debut est indiquee on la prend
      if (m_Heure_Deb_Main_Courante > "")
        { sdatelim.replace(11,5,m_Heure_Deb_Main_Courante);
          if (sdatelim > QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
          sdatelim = qdatelim.addDays(-1).toString("yyyy-MM-dd hh:mm:ss");
        }
      // calcul temps maxi d'affichage des entrees
      QDateTime qdatemax = QDateTime::currentDateTime();
                qdatemax =  qdatemax.addSecs(m_Nb_Heures_Derniers_Entrants.toInt() * -3600);
      sdatemax =  qdatemax.toString("yyyy-MM-dd hh:mm:ss");
      // Chargement de toutes les entrees depuis 24 h (parametrables)
      requete        = "SELECT ";
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_NOM          + ", " ;            // 0
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_PRENOM       + ", " ;            // 1
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_NSS     + ", " ;            // 2
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_NUM_SS       + ", " ;            // 3
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_RANG_GEME    + ", " ;            // 4
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_QUALITE      + ", " ;            // 5
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_PK           + ", " ;            // 6
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_GUID_PATIENT + ", " ;            // 7
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_ENTREE  + ", " ;            // 8
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_PEC     + ", " ;            // 9
      requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_NUM_PAT_SIGEMS ;                 // 10

      requete       += " FROM "               + G_pCApp->m_pC_DSigemsVar->m_SIG_ENTREES_TBL_NAME ;

      requete       += " WHERE ";
      requete       += " (" + G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_ENTREE  + " > '" + sdatelim + "') "; // ceux qui sont dans la main courrante
      requete       += " OR ("  ;
      requete       += " (" + G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_ENTREE  + " > '" + sdatemax + "') "; // ceux qui sont entres depuis 24 h et
      requete       += " AND "  ;
      requete       += " (" + G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_PEC     + " = '1111-11-11T11:11:11') ";  // qui n'ont pas ete pris en charge
      requete       += " )" ;
      requete       += " ORDER BY "           + G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_PK;

      QSqlQuery query(requete , m_pCMoteurBase->getDatabase() );

      int i  = 0;
      //.................. si la requete a un resultat ..............................................
      if (query.isActive())
        {pQlistView->clear();
        while (query.next())
            {
            QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
            if (pQTreeWidgetItem)
                {
                pQTreeWidgetItem->setText(0, m_pCMoteurBase->Utf8_Query(query, 0 ));   // Nom
                pQTreeWidgetItem->setText(1, m_pCMoteurBase->Utf8_Query(query, 1 ));   // Prenom
                pQTreeWidgetItem->setText(2, m_pCMoteurBase->Utf8_Query(query, 2 ).remove("-"));   // Date Naiss
                pQTreeWidgetItem->setText(3, m_pCMoteurBase->Utf8_Query(query, 3 ));   // Num SS
                pQTreeWidgetItem->setText(4, m_pCMoteurBase->Utf8_Query(query, 4 ));   // Rang GEM
                pQTreeWidgetItem->setText(5, m_pCMoteurBase->Utf8_Query(query, 5 ));   // Qualite
                // 6 selon guid
                pQTreeWidgetItem->setText(7, query.value(7).toString());   // guid pat
                pQTreeWidgetItem->setText(8, "1");                                     // occurence
                // 9  droit deb hide
                // 10 droit fin hide
                // 11 CAS       hide
                pQTreeWidgetItem->setText(12, query.value(6).toString());   // PK entree sigems
                pQTreeWidgetItem->setText(13, query.value(9).toString());   // date prise en charge
                pQTreeWidgetItem->setText(14, query.value(10).toString());  // Numero de patient sigems qui servira de GUID

                // Les pris en charge sont en bleu clair
                if (query.value(9).toString() != "1111-11-11T11:11:11")
                    {for (int i = 0; i < pQlistView->columnCount(); i++)
                         pQTreeWidgetItem->setBackgroundColor(i,QColor(204,204,255));
                    }
                // on met un icone different si le patient existe deja
                if (query.value(7).toString() > "")
                    {
                    pQTreeWidgetItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/VitaleOk.png")) );                                      // 6 pk doss
                    pQTreeWidgetItem->setText(6, "-");    // POUR VOIR SI CA MARCHE
                    }
                else
                    {
                    pQTreeWidgetItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/VitaleNotOk.png")) );
                    pQTreeWidgetItem->setText(6, "");
                    }
                // REVOIR LES COLONNES
                pQlistView->addTopLevelItem(pQTreeWidgetItem);
                // on repositionne l'eventuelle entree selectee
                   /*if (PK_Sigems_Selected == pQTreeWidgetItem->text(12))
                     pQTreeWidgetItem->setSelected(true);
                    */
                } //endif (pQTreeWidgetItem)
            ++i;
            } //end while (pSqlQuery->next())
        } //endif (pSqlQuery && pSqlQuery->isActive())
    G_pCApp->m_pCMoteurBase->CloseBase();

    return i;
}
#endif // fin if ENTREES SIGEMS

//================================ C_LevelOruItemDelegate =====================================================

C_LevelOruItemDelegate::C_LevelOruItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}
//---------------------------------------- paint -----------------------------------------------------------------
//
void C_LevelOruItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,  const QModelIndex &index) const
 {  bool drawProgress=false;
    static QStringList ratioColor = QStringList() << "#52ff00" <<       // 0
                                                     "#81ff00" <<       // 1
                                                     "#b4ff00" <<       // 2
                                                     "#ddff00" <<       // 3
                                                     "#fff700" <<       // 4
                                                     "#ffe600" <<       // 5
                                                     "#ffc400" <<       // 6
                                                     "#ff7000" <<       // 7
                                                     "#ff5000" <<       // 8
                                                     "#ff2900";         // 9
    if (index.column() == 2 )             // colonne reservee au progress
       { int                                   indexRow = index.row();                                            // recuperer a rangee de la case a dessiner
         const QAbstractItemModel * pQAbstractItemModel = index.model();                                          // recuperer le modele de la list view de cette case
         QModelIndex                    modelIndexRatio = pQAbstractItemModel->index ( indexRow, 10 );            // recuperer le modele de la case du ratio de cette rangee
         QModelIndex                       modelAttente = pQAbstractItemModel->index ( indexRow, 11 );            // recuperer le modele de la case de l'attente de cette rangee
         QString                                s_ratio = pQAbstractItemModel->data(modelIndexRatio).toString();
         QString                              s_attente = pQAbstractItemModel->data(modelAttente).toString();
         int                                      ratio = s_ratio.toInt();
         if (ratio)       // ne dessiner le progress que si i y a un contenu valide
            { ratio         = qMin(ratio,100);
              drawProgress  = true;
              int w_coul    = (option.rect.width()-2)/10;
              int i_couMax  = qMin(9,qMax(0,ratio/10-1));
              for (int i=0; i<=i_couMax;++i)
                 { QColor   col(ratioColor[i]);
                   QBrush brush(col);
                   painter->fillRect (option.rect.x()+1+i*w_coul, option.rect.y()+1, w_coul, option.rect.height()-2, brush );
                 }
              painter->drawText(option.rect.x()+1,option.rect.y()+1,option.rect.width()-2,option.rect.height()-2,Qt::AlignHCenter,s_attente);
            }
      }
    if (!drawProgress)  QStyledItemDelegate::paint(painter, option, index);
 }

//-----------------------------------------------------  Slot_listView_Oru_Clicked -------------------------------------------
void C_Manager::Slot_listView_Oru_Clicked( QTreeWidgetItem *pQTreeWidgetItem, int)
{   if (pQTreeWidgetItem==0) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    initListePatient( "_GUID="+pQTreeWidgetItem->text(6), "");
    if (m_pGUI->listView_Patient->topLevelItemCount())
    {  QTreeWidgetItem *pQTreeWidgetItemToSelect = m_pGUI->listView_Patient->topLevelItem (0);
       if ( pQTreeWidgetItemToSelect )
        {  pQTreeWidgetItemToSelect->setSelected    ( pQTreeWidgetItemToSelect );
           m_pGUI->listView_Patient->setCurrentItem ( pQTreeWidgetItemToSelect );
        }
    }
    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------  Slot_listView_Oru_DoubleClicked -------------------------------------------
void C_Manager::Slot_listView_Oru_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int)
{   if (pQTreeWidgetItem==0) return;
    Slot_LauchPatient(pQTreeWidgetItem->text(6));
}
//--------------------------------- Slot_listView_Oru_ContextMenuRequested -----------------------------------------------------------------------

void C_Manager::Slot_listView_Oru_ContextMenuRequested (const QPoint &)
{ m_oruContextActivated = 0;
  QTreeWidgetItem *pQTreeWidgetItem = getSelectedListViewItem(m_pGUI->treeWidget_oru);
  if (pQTreeWidgetItem==0)     return;
  m_oruContextActivated = 1;
  QString fileName      = pQTreeWidgetItem->text(9);                                                             // si m_oruMethodIsSQL==true contient directemenet les donnees XML
  QString dataXml       = m_oruMethodIsSQL ? fileName : CGestIni::Param_UpdateFromDisk(m_pathOru+"/"+fileName);  // sinom aller les chercher sur le disque
  if (dataXml.length()==0)
     { qDebug()<<tr(" Slot_listView_Oru_ContextMenuRequested () this file '%1' don't exists").arg(fileName);
       m_oruContextActivated =0;
       return;
     }
  CRubRecord  record;
  QStringList list;
  QString nom     = "";
  QString prenom  = "";
  QString pk      = "";
  int nextPos     = 0;
  QString dateDeb = CGestIni::getXmlData("entree",             dataXml, &nextPos);
  QString guid    = CGestIni::getXmlData("ilp_c",              dataXml, &nextPos);
  QString pkDoc   = CGestIni::getXmlData("idpass_c",           dataXml, &nextPos);

  if ( m_pCMoteurBase->GetPatientNomPrenomPkFromGuid( 0, guid, &nom, &prenom , &pk ) == 0 )
     { list.append( tr("=1=Supprimer car aucun dossier MedinTux ne correspond \303\240 ce passage "));
     }
  else  if ( m_pCMoteurBase->GetRecorFromHeadPk(record, pkDoc) == 0 )
     { list.append( tr("=1=Supprimer car aucun document MedinTux ne correspond \303\240 ce passage "));
     }
  else
     { list.append( tr("=2=Editer le dossier pour mettre \303\240 jour la date de fin de ce passage ") );
       list.append( tr("=4=Supprimer ce passage (ASSUREZ VOUS QU'IL SOIT VRAIMENT INUTILE)"));
     }
  int nb = 0;
  QTreeWidgetItemIterator it ( m_pGUI->treeWidget_oru );
  while (*it)
  {    if ( (*it)->text(6) == guid) ++nb;
       ++it;
  }
  if (nb>1) list.append( tr("=3=Supprimer ce doublon possible avec d'autres passages en cours.") );
  list.append( tr("=30000=rafraichir cette liste ") );
  //.................. actionner le menu et les actions .................................................

  int action = G_pCApp->DoPopupList(list).toInt();
  switch (action)
         {
          case 4:
           { int ret = QMessageBox::information( this, tr("Remove a record"),
                                  tr( "Removing a record :")+" <br /><b>" + nom + "&nbsp;" + prenom +
                                      "</b>" +
                                      "<br />"+tr("Are you sure to remove this record ?") ,
                                  tr("&Remove"), tr("&Cancel"), 0,
                                  1, 1 );
             if (ret==1) return;
            }
          case 1:
          case 3:
               { QDateTime dt = QDateTime::fromString(dateDeb,"yyyy-MM-dd hh:mm:ss");
                 if ( ! dt.isValid() )
                    { dateDeb = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }
                 CGestIni::setXmlData("entree", dateDeb, dataXml);                // un enregistrement est nul si la date de sortie est egale a la date d'entree
                 CGestIni::setXmlData("sortie", dateDeb, dataXml);                // CAD une duree de passage de zero
                 if (m_oruMethodIsSQL)
                    { QString het_pk  = CGestIni::getXmlData("het_pk",dataXml);    // recuperer le pk avant de le virer du xml
                      int pos         = dataXml.indexOf("</het_pk>\n");            // rechercher le tag de fin du pk du xml
                      if (pos        != -1 ) dataXml = dataXml.mid(pos+10);        // virer le tag le pk du xml
                      QStringList arg_list;
                      //          la table         la condition where                         le 1er champ            et sa valeur  le 2eme champ           et sa valeur  .............
                      arg_list << "het_passages" << QString ("`het_pk` = '%1'").arg(het_pk) << "het_encours_dt_entree" << dateDeb << "het_encours_dt_sortie" << dateDeb << "het_xml_blob" << dataXml;
                      m_pCMoteurBase->SQL_WRITE(arg_list);
                    }
                 else
                    {
                      QFile::remove ( m_pathOru+"/"+fileName );
                      CGestIni::Param_UpdateToDisk(m_pathOru+"/"+fileName,dataXml);
                    }
               } break;
          case 2:
               { Slot_listView_Oru_DoubleClicked( pQTreeWidgetItem, 0);
                 break;
               }
         }
  m_oruContextActivated = 0;
  Slot_GetOruList();
}
//-----------------------------------------------------  Slot_GetOruList -------------------------------------------
/*! \brief Remplit la QListView avec les entrees ORU retrouvees dans la base de donnees.
*/
void C_Manager::Slot_GetOruList( )
{GetOruList(m_pGUI->treeWidget_oru);
}
/*
     <passage>
         <entree>2014-02-26 17:53:00</entree>
         <sortie>2014-02-26 22:22:00</sortie>
         <ilp_c>30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A</ilp_c>
         <idpass_c>134</idpass_c>
         <uf>URGE</uf>
         <is_uhcd>0</is_uhcd>
         <dt_naissance>1970-01-01</dt_naissance>
         <sexe>M</sexe>
         <med_traitant></med_traitant>
         <date_examen>2014-02-26 17:53:00</date_examen>
         <date_uhcd></date_uhcd>
         <mode_admission>5</mode_admission>
         <transport_admission>SMUR</transport_admission>
         <medecin_urgentiste>6d6319d5-c6b9-4df5-aa91-69bb8c4580b5</medecin_urgentiste>
         <salle_exam></salle_exam>
         <categorie_recours>Accident  Ischémique Transitoire</categorie_recours>
         <motif_recours>athérosclérose des artères distales</motif_recours>
         <code_recours>I70.2</code_recours>
         <ccmu></ccmu>
         <dest_souhaitee></dest_souhaitee>
         <dest_attendue></dest_attendue>
         <ide>ALEG00</ide>
         <moyen_transport></moyen_transport>
         <motif_transfert></motif_transfert>
         <type_dest></type_dest>
         <actes></actes>
         <diags><diag is_ppal="1">I70.2</diag></diags>
         <is_evenement></is_evenement>
         <lib_evenement></lib_evenement>
     </passage>
 */

//-----------------------------------------------------  GetOruSql_XML_List -------------------------------------------
QStringList C_Manager::GetOruSql_XML_List()
{   QStringList retList;
    QSqlQuery query("SELECT `het_pk` , `het_xml_blob` FROM `het_passages` WHERE (`het_encours_dt_sortie` IS NULL) OR (`het_encours_dt_sortie` = '0000-00-00 00:00:00')" , m_pCMoteurBase->getDatabase() );

    if (!query.isActive()) return retList; // Voir message erreur ?
    while (query.next())
          {retList.append("<het_pk>"+query.value(0).toString()+"</het_pk>\n" + m_pCMoteurBase->Utf8_Query(query, 1 ));
          }
    return retList;
}

//-----------------------------------------------------  GetOruList -------------------------------------------
/*! \brief Remplit la QListView avec les entrees ORU retrouvees dans la base de donnees.
 *  \param pQlistView : ListView qui recevra les patients recherches
 *  \return nombre d'enregistrements inseres dans la QListView passee en parametre
*/
long C_Manager::GetOruList(QTreeWidget     *pQlistView )
{ if (m_oruContextActivated)    return 0;
  if ( m_pathOru.length() == 0) return 0;
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  int nextPos            = 0;
  QStringList filesList  = m_oruMethodIsSQL ? GetOruSql_XML_List() : CGestIni::listDirectory(m_pathOru);
  QString nom            = "";
  QString prenom         = "";
  QString pk             = "";
  CRubRecord record;
  pQlistView->clear();
  for (int i=0; i<filesList.size(); ++i)
  {
      QString dataXml = "";
      if (m_oruMethodIsSQL) dataXml = filesList[i];                                                       // contient directement le XML
      else                  dataXml = CGestIni::Param_UpdateFromDisk(m_pathOru+"/" + filesList[i]);       // aller le chercher a partir du nom du fichier
      nextPos         = 0;
      QString dateDeb = CGestIni::getXmlData("entree",             dataXml, &nextPos);
      QString dateFin = CGestIni::getXmlData("sortie",             dataXml, &nextPos);
      QString guid    = CGestIni::getXmlData("ilp_c",              dataXml, &nextPos);
      if  ( dateFin.length()==0 )     // si passage encore en cours (pas de date de fin) on l'affiche
          { QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem(pQlistView);
            if (pQTreeWidgetItem==0) continue;
            //............ recuperer les donnees .......................................
            int nbIndex         = m_pCMoteurBase->GetPatientNomPrenomPkFromGuid( 0, guid, &nom, &prenom , &pk );
            QString pkDoc       = CGestIni::getXmlData("idpass_c",           dataXml, &nextPos);
            QString is_uhcd     = CGestIni::getXmlData("is_uhcd",            dataXml, &nextPos);
            QString datePec     = CGestIni::getXmlData("date_examen",        dataXml, &nextPos);
            QString medecin     = CGestIni::getXmlData("medecin_urgentiste", dataXml, &nextPos);
            QString recours     = CGestIni::getXmlData("motif_recours",      dataXml, &nextPos);
            QString type_dest   = CGestIni::getXmlData("type_dest",          dataXml, &nextPos);
            //....... calcule de la progression du temps...............
            //        pour la date de debut
            int ratio           = 0;
            int attente_h       = 0;
            int attente_m       = 0;
            int attente_s       = 0;
            QString attente     = "";
            QDateTime dt_pec    = QDateTime::fromString(datePec,"yyyy-MM-dd hh:mm:ss");
            if ( ! dt_pec.isValid() )
               { QDateTime dt   = QDateTime::fromString(dateDeb,"yyyy-MM-dd hh:mm:ss");
                 if ( dt.isValid() )
                    { attente_s = dt.secsTo (QDateTime::currentDateTime());   // ATTENTION on commence avec le secondes
                      attente_h = attente_s/3600;
                      attente_m = (attente_s-(attente_h*3600))/60;
                      ratio     = attente_s /(36*m_oruCriticalWait);  // 100% correspond a un delai d'attente de 6 heures soit 3600*6 secondes => ratio = (nbSecondes/3600*6)x100
                      attente   = QString::number(attente_h)+"h"+QString::number(attente_m)+"mn";
                    }
               }
            medecin = m_pCMoteurBase->GetUserNomPrenomFromGUID( medecin );
            pQTreeWidgetItem->setText(0,  nom);          // Nom
            pQTreeWidgetItem->setText(1,  prenom);       // Prenom
            pQTreeWidgetItem->setText(2,  dateDeb);      // dateDeb
            pQTreeWidgetItem->setText(3,  datePec);      // datePec
            pQTreeWidgetItem->setText(4,  dateFin);      // datefin
            pQTreeWidgetItem->setText(5,  recours);      // recours
            pQTreeWidgetItem->setText(6,  guid);         // guid
            pQTreeWidgetItem->setText(9,  filesList[i]); // file
            pQTreeWidgetItem->setText(10, QString::number(ratio)); // ratio d'attente avant prise en charge
            pQTreeWidgetItem->setText(11, attente);      // ratio d'attente avant prise en charge
            pQTreeWidgetItem->setText(12, medecin);      // medecin prise en charge
            long       nbRecord = m_pCMoteurBase->GetRecorFromHeadPk(record, pkDoc, QString(" %1  = '%2'").arg( m_pCMoteurBase->m_DOSS_RUB_HEAD_DATE, dateDeb));
            if (nbRecord==0 || nbIndex==0)
               {  pQTreeWidgetItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/DelDoc.png")) );
                  //for (int i = 0; i < pQlistView->columnCount(); i++)
                  //     pQTreeWidgetItem->setBackgroundColor(i,QColor("#ff2121"));
               }
            else
               { if (is_uhcd=="1")         pQTreeWidgetItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/uhtcd.png")) );
                 else if (type_dest=="H")  pQTreeWidgetItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/hospi.png")) );  //
                 else                      pQTreeWidgetItem->setIcon ( 0, QIcon(Theme::getIcon( "16x16/destunknow.png")) );
               }
            pQlistView->addTopLevelItem(pQTreeWidgetItem);
         }
  }
 QApplication::restoreOverrideCursor();
 return filesList.size();
}

//-------------------------- DouJeViens ---------------------------------------------
// Pour savoir dans quelle liste on a clique
QString C_Manager::DouJeViens(QTreeWidgetItem *pQListViewItem)
{
if (pQListViewItem->treeWidget()->objectName() == "listView_Sigems")
    return ("SIGEMS");
else
    return ("SESAM");
}

// -------------------------------------------------------------------------------
// Alimentation d'une CV avec le patient Sigems selectionne
void C_Manager::create_CV_with_patient_Sigems()
{
#ifdef ENTREES_SIGEMS
  QString     zbid, zligHPR ;
  QString     requete;
  QStringList listChampsP, listChampsAP, listAdresse;
  //qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss:zzz ") + " - create_CV_with_patient_Sigems"; // SIGEMS A VIRER

  QTreeWidgetItem *pQListViewItem = getSelectedListViewItem(m_pGUI->listView_Sigems);
  if (!pQListViewItem) return;
     // relecture de l'Entree Sigems pour recup de toutes les infos patients
     if (G_pCApp->m_pCMoteurBase->OpenBase()==0)    {return ;}
     zbid   = pQListViewItem->text(12);      // PK de l'entree Sigems
     requete        = "SELECT ";
     requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_NOM          + ", " ;            // 0
     requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_PRENOM       + ", " ;            // 1
     requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_DATE_NSS     + ", " ;            // 2
     requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_NUM_SS       + ", " ;            // 3
     requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_BLOB_HPR            ;            // 4
     requete       += " FROM "               + G_pCApp->m_pC_DSigemsVar->m_SIG_ENTREES_TBL_NAME ;
     requete       += " WHERE ";
     requete       += G_pCApp->m_pC_DSigemsVar->m_SIG_ENT_PK       + " = " + zbid ;

     QSqlQuery query(requete , m_pCMoteurBase->getDatabase() );

     if (!query.isActive()) return ; // Voir message erreur ?
     if (!query.next())     return ; // Voir message erreur ?

     // Recup des donnees dans les lignes HPR
     int posP, posAP, posAC;
     zbid  = m_pCMoteurBase->Utf8_Query(query, 4); // recup du blob HPRIM
     posP  = zbid.indexOf("P|");
     posAP = zbid.indexOf("AP|");
     posAC = zbid.indexOf("AC|");
     if (posP >= 0 && posAP > 0)
         {
           if (posAC <= 0) posAC = zbid.length();
           zligHPR      = zbid.mid(posP, posAP - posP);
           listChampsP  = zligHPR.split('|');
           zligHPR      = zbid.mid(posAP, posAC - posAP);
           listChampsAP = zligHPR.split('|');
         }

    // Creation de la CV virtuelle avec donnees lues dans Entree Sigems
    // ?????  if (G_pCApp->m_pVitale) delete G_pCApp->m_pVitale;
    G_pCApp->m_pVitale = new C_Vitale;

    for (int i =1 ; i< 3; i++)
    {
    // Num SS
    zbid = m_pCMoteurBase->Utf8_Query(query, 3);
    zbid += CMoteurBase::ComputeClef_Secu(zbid);
    G_pCApp->m_pVitale->SetMember(8, 101, zbid, zbid.length(), i);                        // Num S\303\251cu Assur\303\251
    // Nom
    zbid = m_pCMoteurBase->Utf8_Query(query, 0 );
    G_pCApp->m_pVitale->SetMember(1, 104, zbid, zbid.length(), i);
    // Prenom
    zbid = m_pCMoteurBase->Utf8_Query(query, 1 );
    G_pCApp->m_pVitale->SetMember(3, 104, zbid, zbid.length(), i);
    // Date Naissance
    zbid = m_pCMoteurBase->Utf8_Query(query, 2 );
    zbid = zbid.left(10).remove("-") + "0000";
    G_pCApp->m_pVitale->SetMember(12, 104, zbid, zbid.length(), i);    // 195802150000
    //...................... sexe ..............................
    if (listChampsP.size()>8)
       {zbid = listChampsP[8];
        G_pCApp->m_pVitale->SetMember(111, 112, zbid, zbid.length(), i);
       }
    //........... Adresse : |RN 113 LA GARANNE ~   HAMEAU LA GARANNE ~ BERRE L ETANG  ~   ~ 13130~| .................
    if (listChampsP.size()>10)
       { listAdresse         = listChampsP[10].split( '~' , QString::KeepEmptyParts );
         if ( listAdresse.size()>0 )
            {  zbid = listAdresse[0];                // adress rue 1
               G_pCApp->m_pVitale->SetMember(4, 104, zbid, zbid.length(), i);
            }
         if ( listAdresse.size()>1 )
            {  zbid =  listAdresse[1];  // "\n" + listAdresse[1];                // adress rue 2
               G_pCApp->m_pVitale->SetMember(5, 104, zbid, zbid.length(), i);
            }
         if (listAdresse.size()>2)                 // Ville
            { zbid = listAdresse[2];
            if (listAdresse.size()>4)               // CP
                zbid = listAdresse[4] + " " + zbid;
            G_pCApp->m_pVitale->SetMember(8, 104, zbid, zbid.length(), i);
            }
       }
    //........ on cree un groupe et membre bidon pour le telephone ....................
    if (listChampsP.size()>12)
       { zbid = listChampsP[12];
         G_pCApp->m_pVitale->SetMember(111, 111, zbid, zbid.length(), i);
       }
    // Nom JF
    if (listChampsP.size()>6)
        {zbid = listChampsP[6];
        G_pCApp->m_pVitale->SetMember(2, 104, zbid, zbid.length(), i);
        }

    // Qualite ??????????????????? ASSURE ????????????????????
    if (listChampsAP.size()>6)
        {zbid = listChampsAP[6];
        G_pCApp->m_pVitale->SetMember(14, 104, zbid, zbid.length(), i);
        }
    // Rang geme
    if (listChampsAP.size()>7)
        {zbid = listChampsAP[7];
        if (zbid[0] == '0') zbid.remove("0");
        G_pCApp->m_pVitale->SetMember(13, 104, zbid, zbid.length(), i);
        }
    if (i == 2)
    {
     // on force Qualite assure
        zbid = "00";
        G_pCApp->m_pVitale->SetMember(14, 104, zbid, zbid.length(), i);

    // Nom Prenom assure
    if (listChampsAP.size()>13)
        {listAdresse         = listChampsAP[13].split( '~' , QString::KeepEmptyParts );
        if (listAdresse.size()>0)
          { zbid = listAdresse[0];                // nom assure
            G_pCApp->m_pVitale->SetMember(1, 104, zbid, zbid.length(), i);
          }
        if (listAdresse.size()>1)
          { zbid = listAdresse[1];                // prenom assure
            G_pCApp->m_pVitale->SetMember(3, 104, zbid, zbid.length(), i);
          }
        }
    }

    // Regime
    if (listChampsAP.size()>8)
          {zbid = listChampsAP[8];
          G_pCApp->m_pVitale->SetMember(10, 101, zbid, zbid.length(), i);
          }
      // code caisse gestionnaire
      if (listChampsAP.size()>9)
          {zbid = listChampsAP[9];
          G_pCApp->m_pVitale->SetMember(11, 101, zbid, zbid.length(), i);
          }
      // code centre
      if (listChampsAP.size()>10)
          {zbid = listChampsAP[10];
          G_pCApp->m_pVitale->SetMember(12, 101, zbid, zbid.length(), i);
          }
      // code gestion
      /*if (listChampsAP.size()>12)
          {zbid = listChampsAP[12];
          G_pCApp->m_pVitale->SetMember(13, 101, zbid, zbid.length(), i);
          }
          */
      // Date debut validitee
      if (listChampsAP.size()>3)
          {zbid = listChampsAP[3];
          G_pCApp->m_pVitale->SetMember(1, 105, zbid, zbid.length(), i);
          }
      // Date fin  validitee
      if (listChampsAP.size()>4)
          {zbid = listChampsAP[4];
          G_pCApp->m_pVitale->SetMember(2, 105, zbid, zbid.length(), i);
          }
    } // fin for i
      G_pCApp->m_pCMoteurBase->CloseBase();
     // if (G_pCApp->readUniqueParam ( "Connexion", "DebugMode").trimmed()=="1")
     //    {CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli()+"vitale-create_CV_with_patient_Sigems.txt",  G_pCApp->m_pVitale->m_VitaleData.toAscii());
     //    }
#endif // fin if ENTREES SIGEMS
}

