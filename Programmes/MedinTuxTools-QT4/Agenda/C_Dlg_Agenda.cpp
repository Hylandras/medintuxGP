 /********************************* C_Dlg_Agenda.cpp ******************************
 *  #include "C_Dlg_Agenda.h"                                                     *
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

#include "C_Dlg_Agenda.h"
#include "../../MedinTuxTools-QT4/CHtmlTools.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include <QPrintDialog>                         // PRINTRDV
#include <QPlainTextEdit>                       // PRINTRDV
#include <QPrinter>                             // PRINTRDV
#include <QToolTip>
#include <QObject>
#include <QWhatsThis>
#include <QFileInfo>
#include <QCursor>
#include <QFont>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QMap>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QPalette>
#include <QHelpEvent>
#include <QByteArray>
#include <QPoint>
#include <QRect>
#include <QGradient>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QThread>
#include <QSslSocket>

#define LOOKREFRESH          if (m_RefreshTimer) Slot_StopTimer(1)    // bloquer le raffraississement
#define UNLOOKREFRESH        if (m_RefreshTimer) Slot_StopTimer(0)
#include "C_Dlg_RdvTypeConfig.h"
#include "C_Dlg_ChercheRDV.h"                   // CZE 2
#include "C_Dlg_ImprimerRDV.h"                  // CZF

/*
#ifndef AGENDA_IN_GUI
//====================================== C_Dlg_MainWindow ==========================================================
//------------------------ C_Dlg_MainWindow ---------------------------------------
C_Dlg_MainWindow::C_Dlg_MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{   ui->setupUi(this);
    ui->mle_debut->setInputMask ("99-99-9999");
    ui->pushButtonDateDeb->setIcon ( Theme::getIcon("Left.png") );
    connect( ui->pushButtonDateDeb, SIGNAL( clicked ()  ),  this ,  SLOT(   On_pushButtonDateDebClicked()  ));
    //........... le logo de DMD .........................................
    ui->textLabelPixMap->setPixmap ( Theme::getIcon("DMD-Logo-03.png") );
    //........... creer et afficher la liste des jours ....................
    delete ui->scrollAreaWidgetContents;                                                  // virer le widget plac\303\251 dans la QScrollArea par designer car polue
    m_pMultiAgendaFrame = new QFrame ( ui->scrollArea_Days);
    m_pAgendaQLayout    = new QBoxLayout ( QBoxLayout::LeftToRight, m_pMultiAgendaFrame );
    m_pC_Frm_Agenda     = new C_Frm_Agenda(QDate::currentDate(), m_pMultiAgendaFrame, G_pCApp->m_PathAppli, G_pCApp->m_LocalParam, &m_C_RendezVousCopy, G_pCApp->m_SignUser);   // creer la liste des jours
    m_pC_Frm_Agenda->QFrame::move(300,0);
    //m_pAgendaQLayout->addWidget(m_pC_Frm_Agenda);
    //m_pAgendaQLayout->addWidget(m_pC_Frm_Agenda2);

    ui->scrollArea_Days->setWidget ( m_pMultiAgendaFrame );        // le placer en widget central de la QScrollArea pour le rendre scrollable
    //m_pC_Frm_Agenda  = new C_Frm_Agenda(QDate::currentDate(), ui->scrollArea_Days, G_pCApp->m_PathAppli, G_pCApp->m_LocalParam, G_pCApp->m_SignUser);   // creer la liste des jours
    //ui->scrollArea_Days->setWidget ( m_pC_Frm_Agenda );        // le placer en widget central de la QScrollArea pour le rendre scrollable
}
//------------------------ C_Dlg_MainWindow ---------------------------------------
C_Dlg_MainWindow::C_Dlg_MainWindow( const QString &pathAppli, const QString &localParam, const QString &signUser, QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindowClass)
{   ui->setupUi(this);
    ui->mle_debut->setInputMask ("99-99-9999");
    ui->pushButtonDateDeb->setIcon ( Theme::getIcon("Left.png") );
    connect( ui->pushButtonDateDeb, SIGNAL( clicked ()  ),  this ,  SLOT(   On_pushButtonDateDebClicked()  ));
    //........... le logo de DMD .........................................
    ui->textLabelPixMap->setPixmap ( Theme::getIcon("DMD-Logo-03.png") );
    //........... creer et afficher la liste des jours ....................
    delete ui->scrollAreaWidgetContents;                                                  // virer le widget plac\303\251 dans la QScrollArea par designer car polue

    m_pMultiAgendaFrame = new QFrame ( ui->scrollArea_Days);
    m_pAgendaQLayout    = new QBoxLayout ( QBoxLayout::LeftToRight, m_pMultiAgendaFrame );
    m_pC_Frm_Agenda     = new C_Frm_Agenda(QDate::currentDate(), m_pMultiAgendaFrame, G_pCApp->m_PathAppli, G_pCApp->m_LocalParam, &m_C_RendezVousCopy, G_pCApp->m_SignUser);   // creer la liste des jours
    m_pC_Frm_Agenda->QFrame::move(10,0);
    m_pAgendaQLayout->addWidget(m_pC_Frm_Agenda);

    ui->scrollArea_Days->setWidget ( m_pMultiAgendaFrame );        // le placer en widget central de la QScrollArea pour le rendre scrollable
    //m_pC_Frm_Agenda = new C_Frm_Agenda(QDate::currentDate(), ui->scrollArea_Days, pathAppli, localParam, signUser);   // creer la liste des jours
    //ui->scrollArea_Days->setWidget ( m_pC_Frm_Agenda );        // le placer en widget central de la QScrollArea pour le rendre scrollable
}

//------------------------ ~C_Dlg_MainWindow ---------------------------------------
C_Dlg_MainWindow::~C_Dlg_MainWindow()
{delete ui;
}
//------------------------ reinitAgendaOnDate ---------------------------------------
void C_Dlg_MainWindow::On_pushButtonDateDebClicked()
{//m_pC_Frm_Agenda->clear();
}
#endif
*/


//====================================== C_Frm_Agenda ==========================================================
//------------------------ C_Frm_Agenda ---------------------------------------------
C_Frm_Agenda::C_Frm_Agenda(const QDate &date,
                           QWidget *parent               /*=0*/,
                           const QString &pathAppli      /*=""*/,
                           const QString &localParamIn   /*=""*/ ,
                           const QString &signUser       /*="admin"*/,
                           const QString &user           /*="admin"*/,
                           const QString &userNomPrenom  /*=""*/,
                           const QString &droits         /*="-sgn-agc-agm"*/,
                           const QString &googleUser     /*=""*/,
                           const QString &googlePass     /*=""*/
                           )
    : QFrame(parent)
{   m_PathAppli                    = pathAppli;
    m_SignUser                     = signUser;
    m_User                         = user;
    m_UserNomPrenom                = userNomPrenom;
    m_pC_GoogleAPI                 = 0;
    m_googleUser                   = googleUser;
    m_googlePass                   = googlePass;
    m_StartDate                    = date;
    m_Magnetisme                   = 5;
    m_Droits                       = droits;
    if (m_User.length()==0) m_User = m_SignUser;
    m_pQWebView                    = 0;
    m_BackgroundMessage            = "";
    m_AjustGoogleAgendaTime        = -7200;   // on est en france apres tout
    m_StylePopup                   = " border: 1px solid #8f8f91; border-radius: 6px; font-size: 11px;";
    m_PaintMode                    = C_Frm_Agenda::DISABLED;
    m_syncGoogle                   = C_Frm_Agenda::noSynchro;
    if (m_Droits.indexOf("agm")!=-1) m_PaintMode |= C_Frm_Agenda::FOR_OTHERS;
    if (m_Droits.indexOf("agc")!=-1) m_PaintMode |= C_Frm_Agenda::NORMAL;
    setAcceptDrops ( TRUE );


    //.......................... si mode normal alors activer et tout initialiser .........................................
    if (m_PaintMode >= C_Frm_Agenda::NORMAL)
    {
    //........................ on va peut etre en avoir besoin pour determiner path appli ........................
    //                         ( si non fourni en entree)
    QFileInfo qfi(qApp->argv()[0]);
    //.................... recuperer le path de l'application si pas fourni en entree ................................
    if (m_PathAppli.length()==0)
       {m_PathAppli    =  CGestIni::Construct_PathBin_Module("Agenda", qfi.path ());
       }
    //....................... si pas de parametre locaux fourni aller les chercher ...................................
    //                        dans Agenda.ini
    m_LocalParam   = localParamIn;
    if (m_LocalParam.length()==0)
       {CGestIni::Param_UpdateFromDisk(m_PathAppli+"Agenda.ini", m_LocalParam);
       }
    //...................... nomadisme ......................................................
    QString val1, connexionVar;
    m_IsGestionNomadisme       = FALSE;
    m_IsNomadeActif            = FALSE;
    if (CGestIni::Param_ReadParam(m_LocalParam.toLatin1(),"Connexion", "Gestion nomadisme", &val1)==QString::null && (val1=val1.lower())!= "inactif")  // zero = pas d'erreur
       {if (val1=="fullreplication")
            m_IsGestionNomadisme = C_Frm_Agenda::fullReplication;
        else if (val1=="progmasterupdate")
            m_IsGestionNomadisme = C_Frm_Agenda::progMasterUpdate;
        else if (val1=="fullsynchro")
            m_IsGestionNomadisme = C_Frm_Agenda::fullSynchro;
       }
    if (m_IsGestionNomadisme && CGestIni::Param_ReadParam(m_LocalParam.toLatin1(),"Connexion", "nomadisme", &val1)==QString::null && val1.lower()[0]=='a')  // zero = pas d'erreur
       {m_IsNomadeActif = TRUE;
       }
    // =========== TEST CZA ======= Pour agenda multibase ==========================
    // S'il existe dans manager.ini un parametrage  de connexion pour le user, on le prend.
    if (CGestIni::Param_ReadParam(m_LocalParam.toLatin1(),"Connexion", signUser, &val1)==QString::null )
        connexionVar     = signUser;
    else
        {
        if (m_IsNomadeActif) connexionVar     = "Nomade";
        else                 connexionVar     = "Master";
        }
    // ======================================= CZA
    //.............................. recuperer les parametres de connexion ..........................................
    //                               dans les parametres locaux
    QString driver, base, baseUser, password, hostname, port;
    if (CGestIni::Param_ReadParam(m_LocalParam.toLatin1(), "Connexion", connexionVar, &driver, &base, &baseUser, &password, &hostname, &port ) != QString::null)
       { driver       = "QMYSQL3";
         base         = "DrTuxTest";
         baseUser     = "root";
         password     = "";
         hostname     = "localhost";
         port         = "3306";
        }
    //.............................. recuperer le fichier des noms des bases et champ ..........................................
    QString  baseCfg;
    CGestIni::Param_UpdateFromDisk(m_PathAppli + "DataBaseAgenda.cfg" , baseCfg);
    //.............................. creer le moteur de base de donnee de l'agenda ..........................................
    QString errMess;
    m_pCMoteurAgenda = new CMoteurAgenda(driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                         base,          // nom de la base: si QODBC3 -> nom de la source de donn\303\251es (userDSN)
                                         baseUser,      // = "root"
                                         password,      // = ""
                                         hostname,
                                         port,
                                         baseCfg.toLatin1(),
                                         QString("BASE_AGENDA_%1").arg(signUser),   // important car lors fermeture l'utilisateur ne detruit que sa base
                                         &errMess,
                                         this);         // passer le parent afin que le moteur de base soit detruit avec l'agenda
    m_pCMoteurAgenda->SetPathAppli(m_PathAppli);
    //....................... positionner le nombre de jours visible et l'ofset de jour de  depart  ............................................................
    QString val;
    m_StartBefore   = BEFORE_DAYS;          // valeurs par defaut
    int nbDayToSee  = NB_DAYS_TO_SEE;       // valeurs par defaut
    if (CGestIni::Param_ReadParam(m_LocalParam.toLatin1(), "Agenda", "Nombre de jours visibles", &val)              == QString::null) nbDayToSee    = qMin(qMax(val.toInt(),10),400);
    if (CGestIni::Param_ReadParam(m_LocalParam.toLatin1(), "Agenda", "Nombre de jours avant le premier jour", &val) == QString::null) m_StartBefore = qMin(qMax(val.toInt(),0),20);
    m_pCMoteurAgenda->SetNbDayInModeDay(nbDayToSee + m_StartBefore);
    m_pCMoteurAgenda->setOpenCloseMode(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(), "Connexion","Fermer requetes").toInt());
    m_MonthLabelCss = CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                                "Agenda", "Style du label mois").replace("_",",");
    setResoPixByMinutes(qMin(qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                    "Agenda", "Nombre pixels par minute").toInt(),1),15));
    //....................... positionner le bon mode openClose ................................................................................................
    m_Magnetisme = qMax(1,CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                            "Agenda", "Magnetisme").toInt());
    //....................... recuperer les reglages des dimensions graphiques ds rdv ..........................................................................
    m_pCMoteurAgenda->SetRepresentation(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                              "Agenda", "Representation").toInt());
    m_pCMoteurAgenda->SetModifConfirm (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                               "Agenda", "Modif confirm").toInt());
    m_pCMoteurAgenda->SetDebDay (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                     "Agenda", "Heure limite basse"));
    m_pCMoteurAgenda->SetEndDay (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                     "Agenda", "Heure limite haute"));
    m_pCMoteurAgenda->SetRafraichissement (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                           "Agenda", "Rafraichissement").toInt());
    m_pCMoteurAgenda->SetMinDaysHeight(qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                          "Agenda", "Hauteur mini jour").toInt(),3));
    m_pCMoteurAgenda->SetAgendaMode_WeekOrDayOrMonth(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                 "Agenda", "Affichage Jour ou Semaine"));
    m_pCMoteurAgenda->SetTitleHeight(qMax(20,qMin(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                    "Agenda", "Hauteur titre").toInt(),150)));
    m_pCMoteurAgenda->SetAgendaButtonBoxHeight(qMax(20,qMin(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),          "Agenda", "Hauteur bouton").toInt(),150)));
    m_pCMoteurAgenda->SetAgendaWidth(qMin(600,qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                   "Agenda", "Largeur").toInt(),100)));
    m_pCMoteurAgenda->SetWeeksToSee(qMin(104,qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                    "Agenda", "Nombre de semaines visibles").toInt(),4)));            // CZA
    m_pCMoteurAgenda->SetAgendaWeekWidth(qMin(1800,qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),              "Agenda", "Largeur semaine").toInt(),150)));                      // CZA
    m_pCMoteurAgenda->SetNbDayInWeek(qMin(7,qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                     "Agenda", "Nombre de jours par semaine").toInt(),1)));            // CZA
    m_pCMoteurAgenda->SetEditNoteMode(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                "Agenda", "Edition de la note").toInt());
    m_pCMoteurAgenda->SetVerboseMode(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                 "Agenda", "VerboseMode").toInt());
    m_pCMoteurAgenda->SetHeightDaysHeaderInExpandMode(qMin(150,qMax(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),  "Agenda", "Hauteur bandeau jour ouvert").toInt(),20)));
    m_pCMoteurAgenda->SetDeleteMode(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                  "Agenda", "Activer bouton delete sur les rdv").toInt());
    m_pCMoteurAgenda->SetTitleTemplate(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                               "Agenda", "Template titre"));
    m_pCMoteurAgenda->SetDayOfMonthToBeDisplay(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                       "Agenda", "Afficher jour du mois").toInt());
    m_pCMoteurAgenda->SetFormatDateInResume(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                          "Agenda", "Format date dans le resume"));
    m_pCMoteurAgenda->SetFontDateInResume(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                            "Agenda", "Attributs de la date dans le resume"));
    m_pCMoteurAgenda->SetHtmlTemplateTitleMonth(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                      "Agenda", "Format nom du mois"));
    m_pCMoteurAgenda->SetAnimationAuthorisation(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                      "Agenda", "Animation active"));
    m_pCMoteurAgenda->SetWeekDeploymentMode(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                          "Agenda", "Deployer semaine toujours sur une ligne"));
    m_pCMoteurAgenda->SetBaseDayHeight(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                               "Agenda", "Hauteurs de base du bandeau des jours"));
    m_pCMoteurAgenda->SetTimeGradHeight(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                              "Agenda", "Hauteur police graduation"));
    m_pCMoteurAgenda->SetOfsetGraduation(CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                             "Agenda", "Ofset graduation"));

    // CZE debut
    m_pCMoteurAgenda->SetDebPM (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                     "Agenda", "Heure debut PM"));
    m_pCMoteurAgenda->SetEndAM (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                     "Agenda", "Heure fin AM"));
    m_pCMoteurAgenda->SetDispo (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                     "Agenda", QString("Disponibilite_") + signUser));
    if (m_pCMoteurAgenda->GetDispo().length() < 1)   // si pas de dispo definie pour cet utilisateur particulier, voir si pas une par defaut pour le cabinet de groupe
        m_pCMoteurAgenda->SetDispo (CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(),                                 "Agenda", "Disponibilite"));
    // CZE fin
    m_pCMoteurAgenda->COL_Get_List(m_ColorProfils);
    //..................... positionner la largeur des bit map selon semaine ou jour .........................
    int dayWitdth;
    if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() != "DAY")
       {dayWitdth     = m_pCMoteurAgenda->GetAgendaWeekWidth()/m_pCMoteurAgenda->GetNbDayInWeek();
       }
    else
       {dayWitdth     = m_pCMoteurAgenda->GetAgendaWidth();
       }
    m_pBMC     = new C_BitMapCollection(Theme::getPath(TRUE)+"Agenda/", dayWitdth, m_pCMoteurAgenda->GetRepresentation()?DOUBLE_DAY_HEIGHT:DAY_HEIGHT);
    m_viewport = this;
    createAnimations();
    resizeBitMapHeader(m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth(), dayWitdth);
    reinitAgendaOnDate(m_StartDate);
   }
 else
   { //.............. voir si texte dans le theme .....................................
     int agendaWidth = CGestIni::Param_ReadUniqueParam(m_LocalParam.toLatin1(), "Agenda", "Largeur").toInt();
     agendaWidth = qMin(600,qMax(agendaWidth,150));
     QString themeDefAgenda   = Theme::getPath(TRUE)+"Agenda/agenda_inaccessible.html";
     if (m_PaintMode >= C_Frm_Agenda::FOR_OTHERS) themeDefAgenda   = Theme::getPath(TRUE)+"Agenda/agenda_multiple.html";
     CGestIni::Param_UpdateFromDisk(themeDefAgenda, m_BackgroundMessage);
     if (m_BackgroundMessage.length()==0) m_BackgroundMessage = tr("Schedule not available for this user: %1").arg(m_SignUser);
     //.............. positionner les widgets .....................................
     QDesktopWidget *pDesktop = QApplication::desktop();
     int h = pDesktop->availableGeometry().height();
     m_pQWebView = new QWebView(this);
     m_pQWebView->setObjectName(QString::fromUtf8("webView_Agenda"));
     m_pQWebView->setStyleSheet ( "background-color: white;" );
     m_pQWebView->setUrl(QUrl(themeDefAgenda));
     m_pQWebView->resize (agendaWidth, h );
     setFixedHeight (h);
     setFixedWidth(agendaWidth);
   }
}

//---------------------------- C_Frm_Agenda ------------------------------------------------
C_Frm_Agenda::~C_Frm_Agenda()
{ delete m_pBMC;
  if (m_pC_GoogleAPI) delete m_pC_GoogleAPI;
}

//---------------------------- getWebView ------------------------------------------------
QWebView *C_Frm_Agenda::getWebView()
{return m_pQWebView;
}

//---------------------------- baseReConnect ------------------------------------------------
void C_Frm_Agenda::baseReConnect(         const QString &driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                          const QString &baseToConnect, // nom de la base: si QODBC3 -> nom de la source de donn\303\251es (userDSN)
                                          const QString &user,          // = "root"
                                          const QString &pasword,       // = ""
                                          const QString &hostname,      // = "localhost"
                                          const QString &port           // = "port"
                                )
{if (m_PaintMode < C_Frm_Agenda::NORMAL) return;
 m_pCMoteurAgenda->BaseConnect(driver, baseToConnect, user, pasword, hostname, port, 0, m_pCMoteurAgenda->GetDataBaseLabel());
}
//------------------------ changeGUID ---------------------------------------
void C_Frm_Agenda::changeGUID (const QString &old_guid, const QString &new_guid)
{   C_Frm_Day *pC_Frm_Day = 0;
    int                 i = 0;
    for (i = 0; i < C_Frm_DayList::size(); ++i)
        { pC_Frm_Day = at(i);
          pC_Frm_Day->changeGUID (old_guid, new_guid);
        }
}

//------------------------ getFreeSpace ---------------------------------------
void C_Frm_Agenda::getFreeSpace()
{   //............. on cherche une plage libre avec le dialogue .................
    C_Dlg_PlagesDispo *Dlg_PlagesDispo = new C_Dlg_PlagesDispo;
    Dlg_PlagesDispo->setWindowTitle(tr("Available dates for %1").arg(m_UserNomPrenom));

    m_pCMoteurAgenda->chargeListePlagesDisponibles(Dlg_PlagesDispo->ui->TreeWidget_PlagesDispo, m_SignUser); // CZA
    if (!Dlg_PlagesDispo->ui->TreeWidget_PlagesDispo->topLevelItemCount() )
        {delete Dlg_PlagesDispo;
        return;
        }
    if (Dlg_PlagesDispo->exec() != QDialog::Accepted)
        {delete Dlg_PlagesDispo;
        return;
        }
    int col = Dlg_PlagesDispo->m_Col;
    QString   sdatePlage   = Dlg_PlagesDispo->m_sDatePlage;                    // jj-mm-aaaa
    QString   sheurePlage  = Dlg_PlagesDispo->m_sHeurePlage.replace('h',':');   // 08:30
    QDateTime  qdatePlage  = QDateTime::fromString(sdatePlage + " " + sheurePlage,"dd-MM-yyyy hh:mm");
    if (!sdatePlage.length())   return;
    //.................. exploitation des resultats .....................................................
    //  rechercher si un jour deja affiche est concerne par cette date
    C_Frm_Day *pC_Frm_Day = 0;
    int                 i = 0;
    for (i = 0; i < C_Frm_DayList::size(); ++i)
        { pC_Frm_Day = at(i);
          if (pC_Frm_Day->getDate()==qdatePlage.date()) break;
        }
    if (i >= C_Frm_DayList::size()) pC_Frm_Day = 0;              // si pas trouve on met a zero
    //.......... si semaine cliquee on y va ..........................................
    if (col == 4)
       {On_AgendaMustDisplayFromThisDate(qdatePlage.date());
        return;
       }
    //.............. si jour affiche on le cree directement dans le jour...........
    if (pC_Frm_Day)
       {pC_Frm_Day->newRDVAtThisDate(qdatePlage, 15);
        return;
       }
    //............... si non le prendre en base directement .............
    C_RendezVous rdv (qdatePlage, 15,"","","","","",m_SignUser, m_User);
    C_Dlg_RdvTypeConfig *pC_Dlg_RdvTypeConfig   = new C_Dlg_RdvTypeConfig(&m_ColorProfils, m_pCMoteurAgenda, this,&rdv, 1);
    if (pC_Dlg_RdvTypeConfig)
       {pC_Dlg_RdvTypeConfig->setCaption(tr("Set new appointment for %1 on the %2").arg(m_UserNomPrenom, qdatePlage.toString("dd MMMM yyyy")));
        if (pC_Dlg_RdvTypeConfig->exec() == QDialog::Accepted) m_pCMoteurAgenda->RDV_Create(rdv);
       }
    delete pC_Dlg_RdvTypeConfig;

}
//------------------------ createAnimations ---------------------------------------
/*!
This function creates the required animations.
*/
void C_Frm_Agenda::createAnimations()
{ if (m_pCMoteurAgenda->GetAnimationAuthorisation()==0)  return;
  QPropertyAnimation *animation;

  m_animationGroup = new QSequentialAnimationGroup(this);

  animation = new QPropertyAnimation(m_viewport, "geometry");
  animation->setDuration(400);
  animation->setEasingCurve(QEasingCurve::InQuad);
  m_animationGroup->addAnimation(animation);

  animation = new QPropertyAnimation(m_viewport, "geometry");
  animation->setDuration(400);
  animation->setEasingCurve(QEasingCurve::OutQuad);
  m_animationGroup->addAnimation(animation);

}

//------------------------ animateBottom ---------------------------------------
/*!
This function performs a top to bottom slide animation.
*/
void C_Frm_Agenda::animateBottom()
{   if (m_pCMoteurAgenda->GetAnimationAuthorisation()==0)  return;
    QPropertyAnimation *animation1;
    QPropertyAnimation *animation2;

  if (m_animationGroup->state() != QAbstractAnimation::Running)
  {
    animation1 = dynamic_cast<QPropertyAnimation*>(m_animationGroup->animationAt(0));
    animation1->setStartValue( m_viewport->geometry());
    animation1->setEndValue(   m_viewport->geometry().adjusted(0,-m_viewport->geometry().height(),0,-m_viewport->geometry().height()));
    //doAnimation(animation1, 1);   // on attend

    animation2 = dynamic_cast<QPropertyAnimation*>(m_animationGroup->animationAt(1));
    animation2->setStartValue( m_viewport->geometry().adjusted(0,m_viewport->geometry().height(),0,m_viewport->geometry().height()));
    animation2->setEndValue(   m_viewport->geometry());
    //doAnimation(animation2,0);    // on attend pas
    doAnimation(0);  // on fait toutes les animations et on attend la fin
  }
}

//------------------------ animateLeft ---------------------------------------
/*!
This function performs a right to left slide animation.
*/
void C_Frm_Agenda::animateLeft()
{ if (m_pCMoteurAgenda->GetAnimationAuthorisation()==0)  return;
  QPropertyAnimation *animation1;
  QPropertyAnimation *animation2;

  if (m_animationGroup->state() != QAbstractAnimation::Running)
  {
    animation1 = dynamic_cast<QPropertyAnimation*>(m_animationGroup->animationAt(0));
    animation1->setStartValue( m_viewport->geometry());
    animation1->setEndValue(   m_viewport->geometry().adjusted(m_viewport->geometry().width(),0,m_viewport->geometry().width(),0));
    //doAnimation(animation1, 1);   // on attend

    animation2 = dynamic_cast<QPropertyAnimation*>(m_animationGroup->animationAt(1));
    animation2->setStartValue( m_viewport->geometry().adjusted(-m_viewport->geometry().width(),0,-m_viewport->geometry().width(),0));
    animation2->setEndValue(   m_viewport->geometry());
    //doAnimation(animation2,0);    // on attend pas
    doAnimation(0);  // on fait toutes les animations et on attend la fin
  }
}

//------------------------ animateRight ---------------------------------------
/*!
This function performs a left to right slide animation.
*/
void C_Frm_Agenda::animateRight()
{   if (m_pCMoteurAgenda->GetAnimationAuthorisation()==0)  return;
    QPropertyAnimation *animation1;
    QPropertyAnimation *animation2;

  if (m_animationGroup->state() != QAbstractAnimation::Running)
  {
    animation1 = dynamic_cast<QPropertyAnimation*>(m_animationGroup->animationAt(0));
    animation1->setStartValue(m_viewport->geometry());
    animation1->setEndValue(  m_viewport->geometry().adjusted(-m_viewport->geometry().width(),0,-m_viewport->geometry().width(),0));
    //doAnimation(animation1, 1);   // on attend

    animation2 = dynamic_cast<QPropertyAnimation*>(m_animationGroup->animationAt(1));
    animation2->setStartValue(m_viewport->geometry().adjusted(m_viewport->geometry().width(),0,m_viewport->geometry().width(),0));
    animation2->setEndValue(  m_viewport->geometry());
    //doAnimation(animation2, 0);  // on attend pas
    doAnimation(0);  // on fait toutes les animations et on attend la fin
  }
}

//------------------------ doAnimation ---------------------------------------
void C_Frm_Agenda::doAnimation(QPropertyAnimation *pQPropertyAnimation /*=0 */, int vaitForEnd  /* = 1 */)
{
    if (pQPropertyAnimation==0)
       {m_animationGroup->start();
        if (vaitForEnd)
           {while (m_animationGroup->state() == QAbstractAnimation::Running) qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
           }
       }
    else
       { pQPropertyAnimation->start();
         if (vaitForEnd)
            {while (pQPropertyAnimation->state() == QAbstractAnimation::Running) qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
            }
       }
}
//--------------------------------------Chercher_les_RDV_dun_patient-----------------------------------------
void C_Frm_Agenda::Chercher_les_RDV_dun_patient (QString nom_prenom_guid /* ="" */ )
{   C_Dlg_ChercheRDV *Dlg_ChercheRDV = new C_Dlg_ChercheRDV (m_pCMoteurAgenda, nom_prenom_guid, this);
    if (Dlg_ChercheRDV->exec() != QDialog::Accepted)
        {delete Dlg_ChercheRDV;
        }
}
//--------------------------------------Imprimer_les_RDV_dun_medecin-----------------------------------------
void C_Frm_Agenda::Imprimer_les_RDV_dun_medecin(QDateTime date_rdv, QString code_user )
{
    C_Dlg_ImprimerRDV *Dlg_ImprimerRDV = new C_Dlg_ImprimerRDV (m_pCMoteurAgenda, date_rdv, code_user, this);
    if (Dlg_ImprimerRDV->exec() != QDialog::Accepted)
        {delete Dlg_ImprimerRDV;
         return;
        }
}
//------------------------ creerRDVFactices ---------------------------------------
void C_Frm_Agenda::creerRDVFactices(const QString &user)
{m_pCMoteurAgenda->creerRDVFactices(user, getStartDate());
}

//------------------------ getAgendaMode_WeekOrDayOrMonth ---------------------------------------
QString C_Frm_Agenda::getAgendaMode_WeekOrDayOrMonth()
{return(m_pCMoteurAgenda->m_WeekOrDay);
}

//------------------------ getAgendaButtonBoxHeight ---------------------------------------
int C_Frm_Agenda::getAgendaButtonBoxHeight()
{return m_pCMoteurAgenda->GetAgendaButtonBoxHeight();
}
//------------------------ getTitleHeight ---------------------------------------
int C_Frm_Agenda::getTitleHeight()
{return m_pCMoteurAgenda->GetTitleHeight();
}
//------------------------ getTitleTemplate ---------------------------------------
QString C_Frm_Agenda::getTitleTemplate()
{return m_pCMoteurAgenda->GetTitleTemplate();
}
//------------------------ setGoogleLoginParam ---------------------------------------
void C_Frm_Agenda::setGoogleLoginParam (const QString &googleUser, const QString &googlePass )
{m_googleUser = googleUser;
 m_googlePass = googlePass;
}
//------------------------ changePixelParMinute ---------------------------------------
void C_Frm_Agenda::changePixelParMinute ( int pixelParMinute )
{setResoPixByMinutes(pixelParMinute);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ changeStartTime ---------------------------------------
void C_Frm_Agenda::changeStartTime ( const QString &time )
{m_pCMoteurAgenda->SetDebDay (time);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ changeEndTime ---------------------------------------
void C_Frm_Agenda::changeEndTime ( const QString &time )
{m_pCMoteurAgenda->SetEndDay (time);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ changeMagnetisme ---------------------------------------
void C_Frm_Agenda::changeMagnetisme ( int magnetisme )
{setMagnetisme(magnetisme);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ changeRafraichissement ---------------------------------------
void C_Frm_Agenda::changeRafraichissement(int rafraichissement)
{m_pCMoteurAgenda->SetRafraichissement(rafraichissement);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ changeModifConfirm ---------------------------------------
void C_Frm_Agenda::changeModifConfirm(int value)
{m_pCMoteurAgenda->SetModifConfirm(value);
}
//------------------------ changeRepresentation ---------------------------------------
void C_Frm_Agenda::changeRepresentation(int representation)
{m_pCMoteurAgenda->SetRepresentation(representation);
 int dayWitdth;
 if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() != "DAY")
    {dayWitdth     = m_pCMoteurAgenda->GetAgendaWeekWidth()/m_pCMoteurAgenda->GetNbDayInWeek();
    }
 else
    {dayWitdth     = m_pCMoteurAgenda->GetAgendaWidth();
    }
 resizeBitMapHeader(m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth(), dayWitdth);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ changeAgendaWidth ---------------------------------------
void C_Frm_Agenda::changeAgendaWidth(int value)
{m_pCMoteurAgenda->SetAgendaWidth(value);
 resizeBitMapHeader(m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth(), value);
 reinitAgendaOnDate(m_StartDate);
}
//------------------------ setAgendaMode_WeekOrDayOrMonth --------------------------------------- CZA
// Affiche l'agenda selon le parametre DAY ou WEEK ou MONTH
void C_Frm_Agenda::setAgendaMode_WeekOrDayOrMonth(QString WeekOrDay)
{
    int dayWitdth    = 0;
    if (WeekOrDay == "DAY")
       {dayWitdth     = m_pCMoteurAgenda->GetAgendaWidth();
       }
    else
       {dayWitdth     = m_pCMoteurAgenda->GetAgendaWeekWidth()/m_pCMoteurAgenda->GetNbDayInWeek();
       }
    resizeBitMapHeader(WeekOrDay, dayWitdth);
    m_pCMoteurAgenda->m_WeekOrDay = WeekOrDay;
    reinitAgendaOnDate(m_StartDate);
}

//------------------------ resizeBitMapHeader ---------------------------------------
void C_Frm_Agenda::resizeBitMapHeader(const QString &WeekOrDay, int dayWitdth)
{if (WeekOrDay != "WEEK")
    {if (m_pCMoteurAgenda->GetRepresentation())  m_pBMC->resizeBitMapToDayHeight(dayWitdth, DOUBLE_DAY_HEIGHT);
     else                                        m_pBMC->resizeBitMapToDayHeight(dayWitdth, DAY_HEIGHT);
    }
 else
    {m_pBMC->resizeBitMapToDayHeight(dayWitdth, m_pCMoteurAgenda->GetHeightDaysHeaderInExpandMode());
    }
}

//------------------------ reinitAgendaOnUser ---------------------------------------
void C_Frm_Agenda::reinitAgendaOnUser(const QString &signUser, const QString &droits)
{m_SignUser  = signUser;
 m_Droits    = droits;
 m_PaintMode = C_Frm_Agenda::DISABLED;
 if (m_Droits.indexOf("agm")!=-1) m_PaintMode |= C_Frm_Agenda::FOR_OTHERS;
 if (m_Droits.indexOf("agc")!=-1) m_PaintMode |= C_Frm_Agenda::NORMAL;
 if (m_PaintMode < C_Frm_Agenda::NORMAL) return;
 reinitAgendaOnDate(m_StartDate);
}

//------------------------ reinitAgendaOnDate ---------------------------------------
void C_Frm_Agenda::reinitAgendaOnDate(QDate dateDeb)
{int        y    = 0;     // ofset de demarrage en haut de la fenetre agenda
 clear();         // effacer les jours
 //............ effacer les labels des mois ......................
 m_pCMoteurAgenda->creer_Liste_Jours_Feries(dateDeb);
 QMapIterator<int, QLabel*> it(m_MonthLabelList);
 while (it.hasNext())
       { it.next();
         QLabel* pQLabel = it.value();
         if (pQLabel) delete pQLabel;
       }
 m_MonthLabelList.clear();
 if (m_PaintMode >= C_Frm_Agenda::NORMAL)
    {
     if (m_pCMoteurAgenda->m_WeekOrDay == "DAY")           // CZA
        { int day_expand; // CZA
          int nbDayToSee  = m_pCMoteurAgenda->GetNbDayInModeDay();
          m_StartDate     = dateDeb;
          dateDeb         = dateDeb.addDays (-m_StartBefore );
          int lastMonth   = 0;
          QLabel *pQLabel = 0;
          for (int i=0;  i<nbDayToSee; ++i)
              { if ( lastMonth==0 || (dateDeb.month() != lastMonth && dateDeb.day()==1) )
                   { int h_label        = DAY_HEIGHT;                 // hauteur de base lorsque l'on ne peut pas utiliser l'espace libre a gauche du premier jour
                     int keyExpandMapY  = dateDeb.year()*10000;       // au debut de chaque mois on met a jour le truc de l'annee
                     lastMonth          = dateDeb.month();

                     //........... ne creer les labels que si besoin................
                     //            sinon le recuperer dans la liste
                     QMap<int, QLabel*>::iterator it = m_MonthLabelList.find(keyExpandMapY + lastMonth);
                     if (it == m_MonthLabelList.end())
                        { pQLabel       = new QLabel(this);
                          pQLabel->setStyleSheet(m_MonthLabelCss);
                          pQLabel->setText( m_pCMoteurAgenda->GetHtmlTemplateTitleMonth().replace("{{MONTH_NAME}}", dateDeb.toString("MMMM yyyy").upper() ));
                          m_MonthLabelList[ keyExpandMapY + lastMonth ] = pQLabel;         // stocker le label (on va en avoir besoin dans le rearange()
                          pQLabel->show();
                        }
                     else
                        { pQLabel       = it.value();
                        }
                     pQLabel->setGeometry(0, y+1    ,            m_pCMoteurAgenda->GetAgendaWidth(), h_label);  // il a deja ete fait y+= h;
                     y += h_label;
                   }
                day_expand = 0; // CZA
                if (dateDeb==QDate::currentDate() || y == 0) day_expand = 1;  // CZA
                C_Frm_Day* pC_Frm_Day = new C_Frm_Day(
                                                        m_pCMoteurAgenda,
                                                        &m_ColorProfils,
                                                        m_pBMC,
                                                        dateDeb,
                                                        m_SignUser,
                                                        m_User,
                                                        m_UserNomPrenom,
                                                        this,
                                                        getMagnetisme(),
                                                        W_OFSET,
                                                        y,
                                                        getResoPixByMinutes(),
                                                        day_expand              // CZA
                                                     );
                if (pC_Frm_Day)
                   {y += appendDay (pC_Frm_Day  );
                    connect( pC_Frm_Day, SIGNAL( Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &)),
                             this,       SIGNAL( Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &)));
                    connect( pC_Frm_Day, SIGNAL( Sign_LauchPatient(const QString &, C_RendezVous *)),
                             this,       SIGNAL( Sign_LauchPatient(const QString &, C_RendezVous *)));

                   }
                dateDeb = dateDeb.addDays (1);
                pC_Frm_Day->show();
              } // end for (int i=0;  i<nbDayToSee; ++i)
          setFixedHeight (y);
          setFixedWidth(m_pCMoteurAgenda->GetAgendaWidth());
        } // end if (m_pCMoteurAgenda->m_WeekOrDay != "WEEK")
     else if (m_pCMoteurAgenda->m_WeekOrDay == "WEEK")
        {
             int nbDayToSee         = m_pCMoteurAgenda->GetNbDayInWeek();
             int widthDay           = m_pCMoteurAgenda->GetAgendaWeekWidth() / nbDayToSee;  // CZA
             m_StartDate            = dateDeb;
             dateDeb                = dateDeb.addDays (- m_StartDate.dayOfWeek() +1 );
             int maxHeightDayOfWeek = 0;                   // CZA
             int      x             = W_OFSET;             // CZA position du jour

             for (int i=0;  i<nbDayToSee; ++i)
                 {
                   C_Frm_Day* pC_Frm_Day = new C_Frm_Day(
                                           m_pCMoteurAgenda,
                                           &m_ColorProfils,
                                           m_pBMC,
                                           dateDeb,
                                           m_SignUser,
                                           m_User,
                                           m_UserNomPrenom,
                                           this,
                                           getMagnetisme(),
                                           x,
                                           0,        // CZA en mode semaine les jours commencent a zero
                                           getResoPixByMinutes(),
                                           1              // jour toujours deploye pour mode semaine
                                          );
                   if (pC_Frm_Day)
                      { x += widthDay; // A REVOIR
                        y  = appendDay (pC_Frm_Day  );
                        connect( pC_Frm_Day, SIGNAL( Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &)),
                                 this,       SIGNAL( Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &)));
                        connect( pC_Frm_Day, SIGNAL( Sign_LauchPatient(const QString &, C_RendezVous *)),
                                 this,       SIGNAL( Sign_LauchPatient(const QString &, C_RendezVous *)));
                      }
                   dateDeb = dateDeb.addDays (1);
                   pC_Frm_Day->show();
                   // on conserve la plus haute longueur de jour affiche de toute la semaine.
                   if (maxHeightDayOfWeek < y) maxHeightDayOfWeek = y;
                 } // end for (int i=0;  i<nbDayToSee; ++i)
             setFixedHeight (maxHeightDayOfWeek);
             setFixedWidth(x);
        }
     else       //    MONTH
        { QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
          int jourDeLaSemaine     = 0;
          int nbDayInWeek         = m_pCMoteurAgenda->GetNbDayInWeek();                     // CZA
          int widthDay            = m_pCMoteurAgenda->GetAgendaWeekWidth() / nbDayInWeek;  // CZA
          m_StartDate             = dateDeb;
          dateDeb                 = dateDeb.addDays (- m_StartDate.dayOfWeek()+1 );
          int      x              = W_OFSET;             // CZA position du jour
          int      h              = 0;
          int      h_weekMax      = 0;
          int      lastMonth      = 0;
          int      nbDayToSee     = nbDayInWeek * m_pCMoteurAgenda->GetWeeksToSee();
          int      i              = 0;
          int      w_monthLabel   = widthDay*nbDayInWeek;
          int      keyExpandMapY  = dateDeb.year()*10000;
          int      expand         = m_WeekExpandMapState.value(keyExpandMapY+dateDeb.weekNumber());
          QLabel  *pQLabel        = 0;
          int    nextY_posNotOk   = 0;
          int      dayInWeek      = 0;
          while ( i<nbDayToSee)
             {
              //........ si premier jour du mois afficher l'entete de mois ..........
              if ( (dateDeb.day()  <= 7 && dateDeb.dayOfWeek() <= nbDayInWeek && dateDeb.month() != lastMonth) || nextY_posNotOk==1)
                 { int h_label      = (m_pCMoteurAgenda->GetRepresentation())?DOUBLE_DAY_HEIGHT:DAY_HEIGHT;         // hauteur de base lorsque l'on ne peut pas utiliser l'espace libre a gauche du premier jour
                   lastMonth        = dateDeb.month();
                   keyExpandMapY    = dateDeb.year()*10000;       // au debut de chaque mois on met a jour le truc de l'annee
                   dayInWeek        = dateDeb.dayOfWeek();
                   bool isEndDeploy = FALSE;

                   //........... ne creer les labels que si besoin................
                   //            sinon le recuperer dans la liste
                   QMap<int, QLabel*>::iterator it = m_MonthLabelList.find(keyExpandMapY + lastMonth);
                   if (it == m_MonthLabelList.end())
                      {pQLabel       = new QLabel(this);
                       pQLabel->setStyleSheet(m_MonthLabelCss);
                       pQLabel->setText( m_pCMoteurAgenda->GetHtmlTemplateTitleMonth().replace("{{MONTH_NAME}}", dateDeb.toString("MMMM yyyy").upper() ));
                       m_MonthLabelList[ keyExpandMapY + lastMonth ] = pQLabel;         // stocker le label (on va en avoir besoin dans le rearange()
                       pQLabel->show();
                      }
                   else
                      {pQLabel       = it.value();
                      }
                   //..................... si une semaine deployee de debut de mois se termine ...........
                   //                      et qu'elle debordait sur l'autre mois
                   if (nextY_posNotOk == 1)          // on est au bout du reliquat de semaine
                      { if ( jourDeLaSemaine==0 )   pQLabel->setGeometry(0, y+1    ,            w_monthLabel, h_label);  // il a deja ete fait y+= h;
                        else                        pQLabel->setGeometry(0, y+1 + h_weekMax,    w_monthLabel, h_label);  // il faut aller apres la hauteur du deploiement
                        nextY_posNotOk = 0;
                        isEndDeploy    = TRUE;
                        y             += h_label;
                      }
                   //..................... si une semaine de debut de mois deployee commence ...........
                   //                      et qu'elle deborde sur l'autre mois
                   else if ( expand && dateDeb.day()==1)
                      { nextY_posNotOk = nbDayInWeek-dayInWeek + 2;    // nb de jours du mois suivant a continuer a afficher pour finir la semaine
                        if (nextY_posNotOk==8) nextY_posNotOk = 0;     // cas du permier jour du mois aussi premier jour de la semaine (ne pas traiter de reliquat)
                      }
                   if (m_pCMoteurAgenda->GetWeekDeploymentMode()==0)   nextY_posNotOk = 0; // on annule le deployement sur une ligne
                   if (isEndDeploy==FALSE && nextY_posNotOk<=0)  // SI l'on doit afficher normalement le label
                      { if (    dayInWeek == 1 )                      // cas particulier ou pas d'espace dessous (tout demarre a 1) alors utiliser l'espace libre d'avant
                           { pQLabel->setGeometry(0, y+1,    w_monthLabel, h_label);  // on creer la place d'affichage
                             y            += h_label;
                           }
                        else                                         // on utilise l'espace libre a gauche du premier jour a afficher
                           { y            += h;
                             pQLabel->setGeometry(0, y+1,     x,            h);  // x est la prochaine position du prochain jour
                           }
                      }
                 }
              --nextY_posNotOk;
               if (jourDeLaSemaine<nbDayInWeek)
                  { C_Frm_Day* pC_Frm_Day = new C_Frm_Day(
                                        m_pCMoteurAgenda,
                                        &m_ColorProfils,
                                        m_pBMC,
                                        dateDeb,
                                        m_SignUser,
                                        m_User,
                                        m_UserNomPrenom,
                                        this,
                                        getMagnetisme(),
                                        x,
                                        y,
                                        getResoPixByMinutes(),
                                        expand                // dans le mode mois les jours sont replies en mode resume (sauf si semaine deployee)
                                       );
                   if (pC_Frm_Day)
                      { x        += widthDay; // A REVOIR
                        h         = appendDay (pC_Frm_Day  );
                        h_weekMax = qMax(h, h_weekMax);

                        connect( pC_Frm_Day, SIGNAL( Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &)),
                                 this,       SIGNAL( Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &)));
                        connect( pC_Frm_Day, SIGNAL( Sign_LauchPatient(const QString &, C_RendezVous *)),
                                 this,       SIGNAL( Sign_LauchPatient(const QString &, C_RendezVous *)));
                        pC_Frm_Day->show();
                      }

                  }
                ++jourDeLaSemaine;
                //.......... jour suivant si debut de semaine repartir de la gauche .......
                dateDeb = dateDeb.addDays (1);
                if (jourDeLaSemaine==7)
                   {jourDeLaSemaine = 0;
                    x               = W_OFSET;
                    expand          = m_WeekExpandMapState.value(keyExpandMapY+dateDeb.weekNumber());
                    h               = h_weekMax;
                    y              += h;
                    h_weekMax       = 0;
                   }
                ++i;
             } // end while ( i<nbDayToSee)
          setFixedHeight (y);
          setFixedWidth(widthDay*nbDayInWeek);
          QApplication::restoreOverrideCursor ();
        } //enf else if (m_pCMoteurAgenda->m_WeekOrDay != "WEEK")
    } // else if (m_PaintMode >= C_Frm_Agenda::NORMAL)
}
//---------------------------------------- OnButtonGoogleClickedPtr --------------------------------------------
void C_Frm_Agenda::OnButtonGoogleClickedPtr (const char*, void *ptrOnDay)
{  m_ptrOnDay   = ptrOnDay;
   m_syncGoogle = C_Frm_Agenda::selectedDay;    // pour que le Slot_GoogleAPI_Sync() sache quelle synchro operer
   if ( m_pC_GoogleAPI==0 )
      { m_pC_GoogleAPI = new C_GoogleAPI(this, m_googleUser, m_googlePass);
        if ( m_pC_GoogleAPI )
           { connect( m_pC_GoogleAPI, SIGNAL( Sign_GoogleAPI_Ready()), this, SLOT( Slot_GoogleAPI_Sync()));
           }
      }
   else
      { if ( m_pC_GoogleAPI->loginFaillure() )
          { m_pC_GoogleAPI->start_login(m_googleUser, m_googlePass);
            return;
          }
        GoogleAPI_Sync_CurrentDay();     // a la mano le bins est deja initialise
      }
}
//---------------------------------------- Slot_GoogleAPI_Sync --------------------------------------------
void C_Frm_Agenda::Slot_GoogleAPI_Sync()
{ if ( m_syncGoogle == C_Frm_Agenda::severalMonth )  GoogleAPI_Sync_BetweenTwoDate();
  else                                               GoogleAPI_Sync_CurrentDay();
}

//---------------------------------------- GoogleAPI_Sync_CurrentDay --------------------------------------------
void C_Frm_Agenda::GoogleAPI_Sync_CurrentDay()
{   C_Frm_Day *pC_Frm_Day = (C_Frm_Day*) m_ptrOnDay;
    if (pC_Frm_Day) pC_Frm_Day->toGoogle(m_pC_GoogleAPI);
}
//---------------------------------------- GoogleAPI_Sync_BetweenTwoDate --------------------------------------------
void C_Frm_Agenda::GoogleAPI_Sync_BetweenTwoDate()
{  toGoogle( m_dateDebSync, m_dateEndSync );
}
//---------------------------------------- toGoogleSynchonization --------------------------------------------
void C_Frm_Agenda::toGoogleSynchonization(int months)
{m_dateDebSync = QDateTime(m_StartDate.addDays (-m_StartBefore ), QTime::fromString("00:00:00","hhmmss"));
 m_dateEndSync = m_dateDebSync.addMonths( months );
 m_syncGoogle  = C_Frm_Agenda::severalMonth;    // pour que le Slot_GoogleAPI_Sync() sache quelle synchro operer

 if ( m_pC_GoogleAPI==0 )
    { m_pC_GoogleAPI = new C_GoogleAPI(this, m_googleUser, m_googlePass);
      if ( m_pC_GoogleAPI )
         { connect( m_pC_GoogleAPI, SIGNAL( Sign_GoogleAPI_Ready()), this, SLOT( Slot_GoogleAPI_Sync()));
         }
    }
 else
    { if ( m_pC_GoogleAPI->loginFaillure() )
        { m_pC_GoogleAPI->start_login(m_googleUser, m_googlePass);
          return;
        }
      GoogleAPI_Sync_BetweenTwoDate();   // a la mano le bins est deja initialise
    }
}

//---------------------------------------- toGoogle --------------------------------------------
void C_Frm_Agenda::toGoogle(  const QDateTime &dateDeb, const QDateTime &dateEnd)
{

 if ( m_pC_GoogleAPI==0 )
    { m_pC_GoogleAPI = new C_GoogleAPI(this, m_googleUser, m_googlePass );
      connect( m_pC_GoogleAPI, SIGNAL( Sign_GoogleAPI_Ready()), this, SLOT( Slot_GoogleAPI_Ready()));
    }
 if ( m_pC_GoogleAPI==0 ) return;
 if ( m_pC_GoogleAPI->loginFaillure() )
    { m_pC_GoogleAPI->start_login(m_googleUser, m_googlePass);
      return;
    }

 QString errMess       = "";
 QString reqDateDebStr = dateDeb.toString("yyyyMMddhhmmss");
 QString reqDateEndStr = dateEnd.toString("yyyyMMddhhmmss");

 RDV_LIST rdvList;
 int nb = m_pCMoteurAgenda->RDV_Get_List( reqDateDebStr,  reqDateEndStr, m_SignUser, rdvList, CMoteurAgenda::OnlyDateWhithName, &errMess );
 if (nb==0) {QApplication::restoreOverrideCursor (); return;}
 //.................. on efface les RDV deja presents entre ces deux dates ...................
 m_pC_GoogleAPI->deleteAllEventsBetweenTwoDates(dateDeb, dateEnd);
 //.................. on prepare la liste des RDV google ........................
 C_GoogleEventList eventsList;
 for (int i  = 0; i < nb; ++i)
     { C_RendezVous *pRdv = dynamic_cast<C_RendezVous*>(rdvList[i]);
       eventsList.append( C_GoogleEvent( pRdv->m_Nom+";"+pRdv->m_Prenom                        ,
                                         pRdv->m_date                                          ,
                                         pRdv->m_date.addSecs(pRdv->m_Duree*60)                ,
                                         CHtmlTools::HtmlToAscii(pRdv->m_Note)                 ,
                                         pRdv->m_Where                                         ,
                                         C_RendezVous::getRdvColor(*pRdv, &m_ColorProfils)     ,
                                         pRdv->m_Tel
                                        ));
     }
 //............ on cree dans google les rdv en mode batch ..................
 m_pC_GoogleAPI->createSeveralEvents(eventsList);
 QApplication::restoreOverrideCursor();
}

//--------------------------------------Slot_startToDate---------------------------------------------------
void C_Frm_Agenda::Slot_startToDate()
{ emit Sign_ReinitModeWeekDayMonth_OnDate(m_tmpDateToStart, m_pCMoteurAgenda->m_WeekOrDay, this);
}

//--------------------------------------On_AgendaMustDisplayFromThisDate---------------------------------------------------
void C_Frm_Agenda::On_AgendaMustDisplayFromThisDate(const QDate & newDate )        // CZA
{emit Sign_ReinitModeWeekDayMonth_OnDate(newDate, m_pCMoteurAgenda->m_WeekOrDay, this);
}

//----------------------------------- Get_C_Frm_Day_FromDate -------------------------------------------
int C_Frm_Agenda::Get_C_Frm_Day_FromDate( QDate dt, int firstDayInWeek /* = 0 */ )
{
    if (firstDayInWeek)   dt = dt.addDays (- dt.dayOfWeek() + 1 );   // premier jour de la semaine est diamanche donc +1 pour le lundi
    for (int i = 0; i < C_Frm_DayList::size(); ++i)
        {C_Frm_Day *pC_Frm_Day = at(i);
         if (pC_Frm_Day->getDate()==dt) return i;
        }
    return -1;
}

//----------------------------------- Slot_ExpandWeek -------------------------------------------
void C_Frm_Agenda::Slot_ExpandWeek()
{QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 QDate dateWeek         = m_tmpDateToStart;
 int      keyExpandMapY = dateWeek.year()*10000 + dateWeek.weekNumber();
 int      expand        = m_WeekExpandMapState[keyExpandMapY] ^ 1;                 // sert a noter les semaines a deployer lors redessin global

 int indexFirstDay      = Get_C_Frm_Day_FromDate( m_tmpDateToStart , 1);
 int nbDayInWeek        = m_pCMoteurAgenda->GetNbDayInWeek();
 int indexLastDay       = indexFirstDay + nbDayInWeek;

 m_WeekExpandMapState[keyExpandMapY] = expand;

 for (int i=indexFirstDay; i<indexLastDay; ++i)
     { C_Frm_Day *pC_Frm_Day = at(i);
       pC_Frm_Day->ExpandDialog();
     }
 On_AgendaMustBeReArange();
 QApplication::restoreOverrideCursor();
}

//----------------------------------- On_AgendaMustBeReArange -------------------------------------------
void C_Frm_Agenda::On_AgendaMustBeReArange()
{   if  (m_PaintMode < C_Frm_Agenda::NORMAL) return;
    int         y  = 0;        // ofset de demarrage en haut de la fenetre agenda

    if (m_pCMoteurAgenda->m_WeekOrDay == "DAY")           // CZA
       { QLabel  *pQLabel       = 0;
         int      lastMonth     = 0;
         QDate dateDeb          = m_StartDate ;
         dateDeb                = dateDeb.addDays (-m_StartBefore );
         for (int i = 0; i < C_Frm_DayList::size(); ++i)
           { //........... afficher le label des mois ......................
             if ( lastMonth==0 || (dateDeb.month() != lastMonth && dateDeb.day()==1) )
                { int h_label        = DAY_HEIGHT;                 // hauteur de base lorsque l'on ne peut pas utiliser l'espace libre a gauche du premier jour
                  int keyExpandMapY  = dateDeb.year()*10000;       // au debut de chaque mois on met a jour le truc de l'annee
                  lastMonth          = dateDeb.month();
                  pQLabel            = m_MonthLabelList[ keyExpandMapY + lastMonth ];
                  if (pQLabel)
                     { pQLabel->setGeometry(0, y+1    ,            m_pCMoteurAgenda->GetAgendaWidth(), h_label);  // il a deja ete fait y+= h;
                       y += h_label;
                     }
                }
             C_Frm_Day *pC_Frm_Day = at(i);
             pC_Frm_Day->moveWidget( W_OFSET, y );
             y += pC_Frm_Day->getHeight();
             dateDeb = dateDeb.addDays (1);
          }
       setFixedHeight (y);
      }
   else  if (m_pCMoteurAgenda->m_WeekOrDay == "WEEK") // Reaffichage mode semaine                             // CZA
      {
       int widthDay = m_pCMoteurAgenda->GetAgendaWeekWidth() / m_pCMoteurAgenda->GetNbDayInWeek();
       int x = W_OFSET;
       int maxHeightDayOfWeek = 0;
       for (int i = 0; i < C_Frm_DayList::size(); ++i)
          {C_Frm_Day *pC_Frm_Day = at(i);
                      pC_Frm_Day->moveWidget( x, y );
                 x += widthDay;
                 if (maxHeightDayOfWeek < pC_Frm_Day->getHeight())
                     maxHeightDayOfWeek = pC_Frm_Day->getHeight();
          }
       setFixedHeight (maxHeightDayOfWeek+FIRST_DAY_POS_Y);
      }
   else
      {
       QDate dateDeb          = m_StartDate ;
       dateDeb                = dateDeb.addDays (-m_StartBefore );
       int jourDeLaSemaine    = 0;
       int nbDayInWeek        = m_pCMoteurAgenda->GetNbDayInWeek();                     // CZA
       int widthDay           = m_pCMoteurAgenda->GetAgendaWeekWidth() / nbDayInWeek;  // CZA
       m_StartDate            = dateDeb;
       dateDeb                = dateDeb.addDays (- m_StartDate.dayOfWeek()+1 );
       int      x             = W_OFSET;             // CZA position du jour
       int      h             = 0;
       int      h_weekMax     = 0;
       int      nbDayToSee    = nbDayInWeek * m_pCMoteurAgenda->GetWeeksToSee();
       int      lastMonth     = 0;
       int      i             = 0;
       int      w_monthLabel  = widthDay*nbDayInWeek;
       int      keyExpandMapY = dateDeb.year()*10000;
       QLabel  *pQLabel       = 0;
       int      dayIndex      = 0;
       int      expand        = m_WeekExpandMapState.value(keyExpandMapY+dateDeb.weekNumber());
       int   nextY_posNotOk   = 0;
       int      dayInWeek     = 0;
       //QString strMonth       = "";
       while ( i<nbDayToSee)
           {
            //........ si premier jour du mois afficher l'entete de mois ..........
            if ( (dateDeb.day()  <= 7 && dateDeb.dayOfWeek() <= nbDayInWeek && dateDeb.month() != lastMonth) || nextY_posNotOk==1)
               { int h_label      = (m_pCMoteurAgenda->GetRepresentation())?DOUBLE_DAY_HEIGHT:DAY_HEIGHT;         // hauteur de base lorsque l'on ne peut pas utiliser l'espace libre a gauche du premier jour
                 bool isEndDeploy = FALSE;
                 lastMonth        = dateDeb.month();
                 keyExpandMapY    = dateDeb.year()*10000;       // au debut de chaque mois on met a jour le truc de l'annee
                 dayInWeek        = dateDeb.dayOfWeek();
                 pQLabel          = m_MonthLabelList[ keyExpandMapY + lastMonth ];
                 //strMonth         = pQLabel->text();
                 //..................... si une semaine deployee de debut de mois se termine ...........
                 //                      et qu'elle debordait sur l'autre mois
                 if (nextY_posNotOk == 1)          // on est au bout du reliquat de semaine
                    { if ( jourDeLaSemaine==0 )   pQLabel->setGeometry(0, y+1    ,            w_monthLabel, h_label);  // il a deja ete fait y+= h;
                      else                        pQLabel->setGeometry(0, y+1 + h_weekMax,    w_monthLabel, h_label);  // il faut aller apres la hauteur du deploiement
                      nextY_posNotOk = 0;
                      isEndDeploy    = TRUE;
                      y             += h_label;
                    }
                 //..................... si une semaine de debut de mois deployee commence ...........
                 //                      et qu'elle deborde sur l'autre mois
                 else if ( expand && dateDeb.day()==1)
                    { nextY_posNotOk        = nbDayInWeek-dayInWeek + 2;    // nb de jours du mois suivant a continuer a afficher pour finir la semaine
                      if (nextY_posNotOk==8) nextY_posNotOk = 0;            // cas du permier jour du mois aussi premier jour de la semaine (ne pas traiter de reliquat)
                    }
                 if (m_pCMoteurAgenda->GetWeekDeploymentMode()==0)   nextY_posNotOk = 0; // on annule le deployement sur une ligne
                 if (isEndDeploy==FALSE && nextY_posNotOk<=0) // SI l'on doit afficher normalement
                    { if (    dayInWeek == 1 )          // cas particulier ou pas d'espace dessous (tout demarre a 1) alors utiliser l'espace libre d'avant
                         { pQLabel->setGeometry(0, y+1,    w_monthLabel, h_label);  // on creer la place d'affichage
                           y            += h_label;
                         }
                      else                                // on utilise l'espace libre a gauche du premier jour a afficher
                         { y            += h;
                           pQLabel->setGeometry(0, y+1,     x,            h);  // x est la prochaine position du prochain jour
                         }
                     }
               }
            --nextY_posNotOk;
            if (jourDeLaSemaine < nbDayInWeek)
               {C_Frm_Day* pC_Frm_Day = this->at(dayIndex++);
                if (pC_Frm_Day)
                   { pC_Frm_Day->moveWidget( x, y );
                     x         += widthDay; // A REVOIR
                     h          = pC_Frm_Day->height();
                     h_weekMax  = qMax(h,h_weekMax);
                   }
               }
             ++jourDeLaSemaine;
             //.......... jour suivant si debut de semaine repartir de la gauche .......
             dateDeb = dateDeb.addDays (1);
             if ( jourDeLaSemaine==7 )
                {jourDeLaSemaine = 0;
                 x               = W_OFSET;
                 h               = h_weekMax;
                 y              += h;
                 h_weekMax       = 0;
                 expand          = m_WeekExpandMapState.value(keyExpandMapY+dateDeb.weekNumber());
                }
             ++i;
           } // end while ( i<nbDayToSee)
       setFixedHeight (y);
       setFixedWidth(widthDay*nbDayInWeek);
      }
}

//====================================== C_Frm_Day ==========================================================
//-------------------------------------- C_Frm_Day ----------------------------------------------------------
C_Frm_Day::C_Frm_Day(CMoteurAgenda       * pCMoteurAgenda ,
                     MAP_COLOR           * pcolorProfils,
                     C_BitMapCollection  * pC_BitMapCollection,
                     QDate                 date,
                     const QString       & signUser,
                     const QString       & user,
                     const QString       & userNomPrenom,
                     QWidget             * parent,
                     int                   timeGranu,
                     int x, int y, int resoPix, int day_expand)
    : QFrame(parent )
{
 QFrame::move ( x, y );
 m_pBMC                = pC_BitMapCollection;
 m_pCMoteurAgenda      = pCMoteurAgenda;
 m_Magnetisme          = timeGranu;
 m_pColorProfils       = pcolorProfils;
 m_Date                = date;
 m_SignUser            = signUser;
 m_User                = user;
 m_PixByMinute         = resoPix;
 m_DayLF               = m_pCMoteurAgenda->GetRepresentation();
 m_BaseDayHeight       = m_DayLF?DOUBLE_DAY_HEIGHT:DAY_HEIGHT;
 m_Height              = m_BaseDayHeight;
 m_rafraich            = m_pCMoteurAgenda->GetRafraichissement();
 m_RefreshTimer        = 0;
 m_DontRecreateWidget  = 0;
 m_UserNomPrenom       = userNomPrenom;
 m_pQRubberBand        = new QRubberBand(QRubberBand::Rectangle, this); // ne sera active que lors des l'agrandissement deplacement du widget
 m_IsDayExpand         = 0;
 m_Width               = m_pCMoteurAgenda->GetAgendaWidth();
 if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() != "DAY")            // CZA
    { m_Width          = m_pCMoteurAgenda->GetAgendaWeekWidth() / m_pCMoteurAgenda->GetNbDayInWeek(); // CZA
      if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() == "WEEK")
         {m_Height        = m_pCMoteurAgenda->GetHeightDaysHeaderInExpandMode();
          m_BaseDayHeight = m_Height;
          // CZC - Ici on remplit l'entete des colonnes jours.
          // Faire apparaitre le frame des titres de colonnes jours
          // qu'une fois : le lundi
          if (m_Date.dayOfWeek() == 1)
              {QFrame *pQFrameDays = parent->parent()->parent()->parent()->findChild<QFrame *>("FrameDesJours_" + signUser);
              if (pQFrameDays > 0)
                  {pQFrameDays->show();
                  }
              }
          QLabel *pLabelDays = parent->parent()->parent()->parent()->findChild<QLabel *>("pLabelDateJour_" + QString::number(m_Date.dayOfWeek()) +"_"+ signUser);
          if (pLabelDays > 0)
             { QString color      = "#000000";
               int verticalAdjust = -2;
               QFont     fnt      = m_pCMoteurAgenda->GetFontDateInResume(color, verticalAdjust);
               pLabelDays->setFont(fnt);
               pLabelDays->setText(QString("<font color=\"%1\">").arg(color)+m_Date.toString(m_pCMoteurAgenda->GetFormatDateInResume('W'))+"</font>");
             }
          // CZ fin
         }
    }
 // si pas WEEK masquer le frame des titres de colonnes jour CZC debut
 if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() != "WEEK")
    {QFrame *pQFrameDays = parent->parent()->parent()->parent()->findChild<QFrame *>("FrameDesJours_" + signUser);
     if (pQFrameDays > 0)
        pQFrameDays->hide();
    } // fin if pas WEEK CZC

 if (m_rafraich>=4)
    {m_RefreshTimer        = new QTimer(this);
     m_RefreshTimer->setInterval(m_rafraich*1000);
    }
 setAcceptDrops (TRUE);
 m_HeureDeb = m_pCMoteurAgenda->GetDebDay(); if (m_HeureDeb.length()==0) {m_HeureDeb = "07:00"; m_pCMoteurAgenda->SetDebDay("07:00");}
 m_HeureFin = m_pCMoteurAgenda->GetEndDay(); if (m_HeureFin.length()==0) {m_HeureFin = "20:00"; m_pCMoteurAgenda->SetEndDay("20:00");}

 m_ButtonExpand = new CMyButton(m_pBMC->m_ButtonExpand_Pixmap, this, "", this);
 m_ButtonExpand->setGeometry(2,(m_BaseDayHeight/2)-m_pBMC->m_ButtonExpand_Pixmap.height()/2,m_pBMC->m_ButtonExpand_Pixmap.width()+2,m_pBMC->m_ButtonExpand_Pixmap.height()+2);
 if (m_DayLF)
    {int y =   LINE_RESUME_HEIGHT;
     m_ButtonExpand->setGeometry(2,y,m_pBMC->m_ButtonExpand_Pixmap.width()+2,m_pBMC->m_ButtonExpand_Pixmap.height()+2);
    }
 m_ButtonExpand->setFlat( TRUE );
 m_ButtonExpand->setToolTip ( "<font color=\"#000000\">"+tr("Opens and closes a page of the schedule.")+"</font>" );



 m_ButtonNewRDV = new CMyButton(m_pBMC->m_ButtonNewRDV_Pixmap, this, "", this);
 m_ButtonNewRDV->setGeometry(m_Width - 22,-1,20, m_BaseDayHeight);
 m_ButtonNewRDV->setFlat( TRUE );
 m_ButtonNewRDV->setToolTip ( "<font color=\"#000000\">"+tr("Add a new appointment on this date.")+"</font>" );

 m_ButtonSave = new CMyButton(m_pBMC->m_ButtonSave_Pixmap, this, "", this);
 m_ButtonSave->setGeometry(m_Width - 42,-1,20, m_BaseDayHeight);
 m_ButtonSave->setFlat( TRUE );
 m_ButtonSave->setToolTip ( "<font color=\"#000000\">"+tr("Save this day as a template.</font>") );

 m_ButtonGoogle = new CMyButton(m_pBMC->m_ButtonGoogle_Pixmap, this, "", this);
 m_ButtonGoogle->setGeometry(m_Width - 62,-1,20, m_BaseDayHeight);
 m_ButtonGoogle->setFlat( TRUE );
 m_ButtonGoogle->setToolTip ( "<font color=\"#000000\">"+tr("Push to Google Agenda.</font>") );

 resize(m_Width,     m_Height );
 connect( m_ButtonNewRDV,  SIGNAL( Sign_ButtonClickedPtr (const char*, void *)  ),          this ,     SLOT(   OnButtonNewRDVClickedPtr (const char*, void *)  )  );
 connect( m_ButtonSave,    SIGNAL( Sign_ButtonClickedPtr (const char*, void *)  ),          this ,     SLOT(   OnButtonSaveClickedPtr (const char*, void *)  ) );
 connect( m_ButtonGoogle,  SIGNAL( Sign_ButtonClickedPtr (const char*, void *)  ),          parent ,   SLOT(   OnButtonGoogleClickedPtr (const char*, void *)  ) );
 connect( m_ButtonExpand,  SIGNAL( Sign_ButtonClickedPtr (const char*, void *)  ),          this ,     SLOT(   OnButtonExpandClickedPtr (const char*, void *)  ) );
 connect( this,            SIGNAL( Sign_AgendaMustBeReArange ()  ),                         parent ,   SLOT(   On_AgendaMustBeReArange()  ) );
 connect( this,            SIGNAL( Sign_AgendaMustDisplayFromThisDate (const QDate & ) ),   parent ,   SLOT(   On_AgendaMustDisplayFromThisDate(const QDate & )  ) );

 if (m_RefreshTimer)
    {connect(m_RefreshTimer, SIGNAL(timeout()), this, SLOT(Slot_RefreshView()));
     m_RefreshTimer->start();
    }

 /*
 if (CMoteurBase::IsThisDroitExist(G_pCApp->m_Droits, "agv")) {m_ButtonExpand->setEnabled(TRUE); }
 else                                                         {m_ButtonExpand->setEnabled(FALSE);}
 if (CMoteurBase::IsThisDroitExist(G_pCApp->m_Droits, "agc")) {m_ButtonSave->setEnabled(TRUE);  m_ButtonNewRDV->setEnabled(TRUE);}
 else                                                         {m_ButtonSave->setEnabled(FALSE); m_ButtonNewRDV->setEnabled(FALSE);}
 m_Default_labelColor = m_pQlabel_titre->paletteBackgroundColor();
 */
 m_pCMoteurAgenda->RDV_Get_List(m_Date, m_SignUser, m_cacheRDV_List);    // si il existe des rdv pour ce jour aller en chercher la liste
 m_ButtonNewRDV->hide();
 m_ButtonSave->hide();
 m_ButtonGoogle->hide();

 if (day_expand == 1) ExpandDialog();
 if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() == "MONTH")
    {m_ButtonExpand->setToolTip(tr("go to this week"));
     if (day_expand == 1)     m_ButtonExpand->setPixmap(m_pBMC->m_ButtonExpand_Close_Pixmap);
     else                     m_ButtonExpand->setPixmap(m_pBMC->m_ButtonGotoWeek_Pixmap);
     if (date.dayOfWeek()!=1) m_ButtonExpand->hide();
    }
 setMouseTracking (TRUE );
}

//---------------------------- ~C_Frm_Day ------------------------------------------------
C_Frm_Day::~C_Frm_Day()
{
}
//------------------------ changeGUID ---------------------------------------
void C_Frm_Day::changeGUID (const QString &old_guid, const QString &new_guid)
{LOOKREFRESH;
 for (int i  = 0; i < m_cacheRDV_List.count(); ++i)
    { C_RendezVous *pRdv = dynamic_cast<C_RendezVous*>(m_cacheRDV_List[i]);
      if (pRdv && pRdv->m_GUID==old_guid)
         {  pRdv->m_GUID = new_guid;
            QString errMess;
            if ( ! m_pCMoteurAgenda->RDV_Update(*pRdv, &errMess) )
               { qDebug()<< errMess;
               }
         }
    }
  C_Frm_Rdv *pC_Frm_Rdv  = 0;
  for (int i = 0; i < C_Frm_RdvList::count(); ++i)
      { pC_Frm_Rdv = at(i);
        if (pC_Frm_Rdv->m_GUID==old_guid)
           { pC_Frm_Rdv->m_GUID==new_guid;
             // QString errMess;
             // if ( ! m_pCMoteurAgenda->RDV_Update(*pC_Frm_Rdv, &errMess) )
             //   { qDebug()<< errMess;
             //   }
           }
      }
  UNLOOKREFRESH;
}
//---------------------------- toGoogle ------------------------------------------------
void C_Frm_Day::toGoogle(C_GoogleAPI *pC_GoogleAPI)
{   if (pC_GoogleAPI==0) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    LOOKREFRESH;
    //.................. on efface les RDV deja presents ce jour ...................
    pC_GoogleAPI->deleteAllEventsBetweenTwoDates( QDateTime(getDate()             , QTime::fromString("00:00","hh:mm")),
                                                  QDateTime(getDate().addDays(1)  , QTime::fromString("00:00","hh:mm"))
                                                );
    //.................. on prepare la liste des RDV google ........................
    C_GoogleEventList eventsList;
    for (int i  = 0; i < m_cacheRDV_List.count(); ++i)
        { C_RendezVous *pRdv = dynamic_cast<C_RendezVous*>(m_cacheRDV_List[i]);
          eventsList.append( C_GoogleEvent(pRdv->m_Nom+";"+pRdv->m_Prenom         ,
                                           pRdv->m_date                           ,
                                           pRdv->m_date.addSecs(pRdv->m_Duree*60) ,
                                           CHtmlTools::HtmlToAscii(pRdv->m_Note)  ,
                                           pRdv->m_Where                          ,
                                           getRdvColor(*pRdv)                     ,
                                           pRdv->m_Tel
                                           ));
        }
    //............ on cree dans google les rdv en mode batch ..................
    pC_GoogleAPI->createSeveralEvents(eventsList);
    UNLOOKREFRESH;
    QApplication::restoreOverrideCursor ();
}
//---------------------------- dropEvent ------------------------------------------------
void C_Frm_Day::dropEvent(QDropEvent *event)
 {  if (event->mimeData()->hasFormat("text/medintux_rdv_drag"))
        {event->acceptProposedAction ();
         QToolTip::hideText();
         QTimer::singleShot(10, this, SLOT(Slot_Drop_Rdv ()));
        }
 }
//---------------------------- dragEnterEvent ------------------------------------------------
void C_Frm_Day::dragEnterEvent(QDragEnterEvent *event)
{if (event->mimeData()->hasFormat("text/medintux_rdv_drag"))
    {event->acceptProposedAction ();
    }
 }
//---------------------------- dragMoveEvent ------------------------------------------------
void C_Frm_Day::dragMoveEvent(QDragMoveEvent * event)
 {  if (event->mimeData()->hasFormat("text/medintux_rdv_drag"))
       {QPoint     pos = mapFromGlobal ( QCursor::pos() );
        QString tm_str = "";
        C_RendezVous rdv; rdv.m_Duree = -2;
        if (isDayExpand())
           { QTime  tm   =  posY_toTime(pos.y());
             tm_str      =  tm.toString("hh:mm");
           }
        else
           { rdv         = getResumeRdvFromXY (  pos.x(),  pos.y() );;
             int mn      = XYToMinutesInResume ( pos.x(),pos.y());
             if (mn==-1) {QToolTip::hideText();      // cassos si pas dans une zone exploitable
                          event->ignore();
                          m_DropedRdv.m_GUID = "";   // effacer les donnees du rdv venant du drag and drop
                          return;
                         }
             int h       = mn/60;
             mn          = mn - (h*60);
             tm_str      = QString("%1:%2").arg(QString::number(h).rightJustified (2, '0'),QString::number(mn).rightJustified (2, '0'));
           }
        m_DropedRdv  = C_RendezVous::unSerialize(QString(event->mimeData()->data ( "text/medintux_rdv_drag" ))); // intialiser le rdv sur les donnees venant du drag and drop

        if (rdv.m_GUID.length()||rdv.m_Nom.length()||rdv.m_Prenom.length())
           {tm_str = "<table cellSpacing=\"0\"  cellpadding=\"4\" width=100% border=\"1\">"    // #FF8000
                        "<tbody>"
                        "<tr>"
                        "<td width=100% align=\"left\" bgcolor=\"#FEFFDD\">"+
                        QString(tr("On")+"&nbsp;<b><font color=\"#0000FF\">%1</font></b>&nbsp;"+tr("at")+"&nbsp;<b><font color=\"#e80d0d\">%2</font></b>&nbsp;"+tr("(duration")+"&nbsp;:&nbsp;<b><font color=\"#e80d0d\">%3</b></font>&nbsp;mn) <b>"+tr("Occupied")+"</b>&nbsp;par&nbsp;<font color=\"#FF8000\"><b>%4</b></font></b>"
                        "</TD>"
                        "</TBODY>"
                        "</TABLE>").arg(m_Date.toString("dd-MM-yyyy"),rdv.m_date.time().toString("hh:mm"), QString::number(rdv.m_Duree), rdv.m_Nom.replace(" ","&nbsp;")+"&nbsp;"+rdv.m_Prenom.replace(" ","&nbsp;")) ;
           }
        else if (rdv.m_PrimKey.length())
           {tm_str = "<TABLE cellSpacing=\"0\"  cellpadding=\"4\" width=100% border=\"1\">"
                        "<TBODY>"
                        "<TR>"
                        "<TD width=100% align=\"left\" bgcolor=\"#FEFFDD\">"+
                        QString(tr("The")+"&nbsp;<b><font color=\"#0000FF\">%1</font></b>&nbsp;"+tr("at")+"&nbsp;<b><font color=\"#e80d0d\">%2</font></b>&nbsp;"+tr("(duration")+"&nbsp;:&nbsp;<b><font color=\"#e80d0d\">%3</b></font>&nbsp;mn <b>"+tr("available")+"</b>&nbsp;pour&nbsp;"
                        "<font color=\"#FF8000\"><b>%4</b></font>&nbsp;"+tr("with")+"&nbsp;<b><font color=\"#0000FF\">%5</font></b>"
                        "</TD>"
                        "</TBODY>"
                        "</TABLE>").arg( m_Date.toString("dd-MM-yyyy"), rdv.m_date.time().toString("hh:mm"), QString::number(rdv.m_Duree), m_DropedRdv.m_Nom.replace(" ","&nbsp;") + "&nbsp;" + m_DropedRdv.m_Prenom.replace(" ","&nbsp;") ,m_UserNomPrenom.replace(" ","&nbsp;")) ;
           }
        else
           {tm_str = "<TABLE cellSpacing=\"0\"  cellpadding=\"4\" width=100% border=\"1\">"
                        "<TBODY>"
                        "<TR>"
                        "<TD width=100% align=\"left\" bgcolor=\"#FEFFDD\">"+
                        tr("The")+QString("&nbsp;<b><font color=\"#0000FF\">%1</font></b>&nbsp;"+tr("at")+"&nbsp;<b><font color=\"#e80d0d\">%2</font> "+tr("Creation")+"</b>&nbsp;"+tr("for")+"&nbsp;<font color=\"#FF8000\"><b>%3</b></font>"
                        "&nbsp;"+tr("with")+"&nbsp;<b><font color=\"#0000FF\">%4</font></b> "
                        "</TD>"
                        "</TBODY>"
                        "</TABLE>").arg(m_Date.toString("dd-MM-yyyy"),tm_str, m_DropedRdv.m_Nom.replace(" ","&nbsp;") + "&nbsp;" + m_DropedRdv.m_Prenom.replace(" ","&nbsp;"), m_UserNomPrenom.replace(" ","&nbsp;")) ;
           }
        //qDebug() << tm_str;
        pos = QCursor::pos(); pos.setX(pos.x()+30);
        QToolTip::showText (pos,  tm_str, this, this->rect() );
        event->acceptProposedAction ();
       }
 }

//---------------------------- dragLeaveEvent ------------------------------------------------
void C_Frm_Day::dragLeaveEvent ( QDragLeaveEvent *  )
{   QToolTip::hideText();
    m_DropedRdv.m_GUID = "";   // effacer les donnees du rdv venant du drag and drop
}
//------------------------ Slot_Drop_Rdv -----------------------------------------------------
void C_Frm_Day::Slot_Drop_Rdv ()
{  QMouseEvent mouseEvent(QEvent::MouseButtonPress, mapFromGlobal ( QCursor::pos() ), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
   On_Day_mousePressEvent ( &mouseEvent );
}
//------------------------ Slot_RefreshView ---------------------------------------
void  C_Frm_Day::Slot_RefreshView()
{
 if (m_DontRecreateWidget) return;

 m_pCMoteurAgenda->RDV_Get_List(m_Date, m_SignUser, m_cacheRDV_List);    // si il existe des rdv pour ce jour aller en chercher la liste
 if (isDayExpand())
    {if (m_cacheRDV_List.count())
        { RecreateRendezVousListWidget(m_cacheRDV_List);                 // recreer les widgets avec
        }
     else
        {clear();
        }
    }
 ReArangeIfDayHeightChange();                 // reajuster eventuellement la hauteur du jour et reorganiser les jours sous jacents et mettre a jour la hauteur du jour
}

//---------------------------------------- computeDayHeight --------------------------------------------
int  C_Frm_Day::computeDayHeight()
{if (isDayExpand())  return (getStartTime().secsTo(getStopTime())/60*getResoPixByMinutes()) + m_BaseDayHeight +  FIRST_DAY_POS_Y*2+10;
 else                return m_BaseDayHeight;
}

//---------------------------------------- ReArangeIfDayHeightChange --------------------------------------------
// verifie si la hauteur calculee du jour est modifiee (exemple apres deplacement d'un rdv apres la limite sup ou inf)
// si elle a ete modifie remet a jour la variable de hauteur m_Height avec la nouvelle hauteur calculee
// et reorganise les jours sous jacents avec la ouvelle hauteur
void  C_Frm_Day::ReArangeIfDayHeightChange()
{ //................. si jour replie la hauteur est fixe ..........................
  //                  donc on a juste a raffraichier l'affichage du resume
  if (!isDayExpand())
     {update();
     }
  else
    { //............. on note la hauteur actuelle .................................
      int actualHeight    = m_Height;                // on note la hauteur actuellement en cours
      m_Height            = computeDayHeight();      // on calcule la hauteur souhaitable
      //............ si pas la meme que celle en place ..................
      //             on reajuste les widgets de rdv sous jacents
      if (actualHeight!=m_Height)
         {resize(m_Width, m_Height );
          emit Sign_AgendaMustBeReArange();
         }
      else
         {update();      // si hauteur non modifie il n'y a juste qu'a raffraichir l'affichage
         }
    }
}

//---------------------------------------- ExpandDialog --------------------------------------------
void  C_Frm_Day::ExpandDialog()
{if (!isDayExpand())
    //............................... SI deja replie alors expandre et construire la liste des widgets rendez vous .........................
    {m_IsDayExpand = 1;
     int nb = m_cacheRDV_List.count(); //m_pCMoteurAgenda->RDV_Get_List(m_Date, m_SignUser, m_cacheRDV_List);    // si il existe des rdv pour ce jour aller en chercher la liste
     if (nb==0)  nb = MaskDayToAgendaDay (m_cacheRDV_List);                // soit il en existe pas et on place le masque de la semaine pr\303\251vu (si il existe)
     if (nb) RecreateRendezVousListWidget(m_cacheRDV_List);                // --- IMPORTANT --- recreer les widgets avec la liste des rendez vous

     if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() == "WEEK")
        {m_ButtonExpand->hide();
         m_ButtonNewRDV->hide();
         m_ButtonSave->hide();
         m_ButtonGoogle->hide();
        }
     else if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() == "MONTH")
        {m_ButtonNewRDV->show();
         m_ButtonSave->show();
         m_ButtonGoogle->show();
        }
     else
        {
         m_ButtonNewRDV->show();
         m_ButtonSave->show();
         m_ButtonGoogle->show();
        }
     m_ButtonExpand->setIcon(m_pBMC->m_ButtonExpand_Close_Pixmap); // icone fleche basse pour le jour ouvert ( pareil pour tous les modes )  \/
    }
 else
    //............................... SI deja expande alors replier et detruire la liste des rendez vous .........................
    {m_IsDayExpand = 0;
     m_ButtonNewRDV->hide();
     m_ButtonSave->hide();
     m_ButtonGoogle->hide();
     clear();  // effacer les widgets
     if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() == "MONTH") m_ButtonExpand->setIcon(m_pBMC->m_ButtonGotoWeek_Pixmap);    // icone fleche horizontale verte pour le jour ferme  |>
     else                                                               m_ButtonExpand->setIcon(m_pBMC->m_ButtonExpand_Open_Pixmap); // icone fleche horizontale bleue pour le jour ferme  |>
    }
 m_Height = computeDayHeight();
 resize(m_Width, m_Height );
}

//------------------------ paintEvent ---------------------------------------
void C_Frm_Day::paintEvent ( QPaintEvent * /*event*/)
{
  QPainter p(this);
  QFont    f             = font();
  int nbRdv              = 0;
  QDate curDt            = QDate::currentDate();
  int timePosPixmapX     = 0;
  int timePosPixmapY     = 0;
  char  mdw              = m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth().toLatin1()[0];
  int nextTmpPosX        = DAY_OFS_X-15;
  QString colorDate      = "#000000";
  int    verticalAdjust  = -2;
  QFont   fntDate        = m_pCMoteurAgenda->GetFontDateInResume(colorDate, verticalAdjust);
  QFontMetrics fntMetric = QFontMetrics(fntDate);
  int           ofsTxtY  = m_BaseDayHeight - fntMetric.descent() + verticalAdjust;
  QString tmpStr         = getDate().toString(m_pCMoteurAgenda->GetFormatDateInResume(mdw));  // date du jour en cours d'affichage
  QRect          br;

  //////////////////////////////////////////////////// JOUR EXPANSE ////////////////////////////////////////////////////////
  if (isDayExpand())    // si jour expand
     { QFont            fGrd  = font(); fGrd.setPixelSize(m_pCMoteurAgenda->GetTimeGradHeight()); fGrd.setBold(true);
       QFontMetrics fntMetGrd = QFontMetrics(fGrd);
       nbRdv                  = C_Frm_RdvList::size();
       QTime tps              = getStartTime();
       int y_der              = tps.secsTo(getStopTime())/60*getResoPixByMinutes();

       //............................ pixmap du jour (date courante)..............
       if (m_pCMoteurAgenda->isFreeDay(m_Date))
          {p.drawPixmap (0, 0, m_pBMC->m_HeadUnWorkDay);
          }
       else
          {if      (m_Date.dayOfWeek() == Qt::Saturday)  p.drawPixmap (0, 0 , m_pBMC->m_HeadSatDay_Pixmap );
           else if (m_Date.dayOfWeek() == Qt::Sunday)    p.drawPixmap (0, 0 , m_pBMC->m_HeadSunDay_Pixmap );
           else                                          p.drawPixmap (0, 0 , m_pBMC->m_HeadOpenDay_Pixmap );
          }
       if (m_Date==curDt && m_rafraich>=4)
          {//........ indicateur graphique de l'heure courante bitmap horizontal.........
           QTime       tm        = QTime::currentTime();
           if (tm>tps && tm<getStopTime())
              {int    mnDeb      = tps.hour()*60 + tps.minute();   // initialise au debut de  if (isDayExpand())
               int    mnCur      = tm.hour()*60+tm.minute(); mnCur -= mnDeb;
               int     posY      = mnCur*getResoPixByMinutes();
               p.drawPixmap (0, posY+m_BaseDayHeight+FIRST_DAY_POS_Y - (m_pBMC->m_AgendaHourIndic_Pixmap.height()>>1), m_pBMC->m_AgendaHourIndic_Pixmap);
              }
          }
       //.................. graduations verticales ........................
       QSize grd_size = fntMetGrd.size ( Qt::TextSingleLine, "==:==");
       int posGrdX    = grd_size.width(); //fntMetGrd.width("44:44"); //fntMetGrd.maxWidth();
       for (int y     = 0; y <= y_der; y += 15*getResoPixByMinutes())
           { int    posY  = y+FIRST_DAY_POS_Y+m_BaseDayHeight;
             QString txt  = tps.toString("hh:mm");
             if (txt==m_HeureDeb)       {p.setPen (Qt::darkRed);   fGrd.setBold(TRUE);   p.setFont ( fGrd );}
             else if (txt==m_HeureFin)  {p.setPen (Qt::darkRed);   fGrd.setBold(TRUE);   p.setFont ( fGrd );}
             else if (txt.mid(3)=="00") {p.setPen (Qt::darkGray);  fGrd.setBold(TRUE);   p.setFont ( fGrd );}
             else                       {p.setPen (Qt::lightGray); fGrd.setBold(FALSE);  p.setFont ( fGrd );}
             //    TODO couleur liee au .ini
             p.drawLine (posGrdX, posY, m_Width,posY);
             p.drawText ( QPoint(0,posY + (grd_size.height()>>1)), tps.toString(txt) );
             tps=tps.addSecs(60*15);
           }  //endfor (int y = 0; y < y_der; y += 15)
       //........... trait vertical a droite ...............
       p.setPen (Qt::lightGray);
       p.drawLine (m_Width-1 , 0, m_Width-1,y_der+m_BaseDayHeight+10);
     } // endif (pC_Frm_Day->isDayExpand())
  //////////////////////////////////////////////////// JOUR REPLIE ////////////////////////////////////////////////////////
  else
     {nbRdv             = m_cacheRDV_List.count();
      QTime tpsDeb      = getStartTime();
      QTime tpsEnd      = getStopTime();
      int nbMnToSee     = getNbMinutesToseeInResume(tpsDeb, tpsEnd);
      int    mnDeb      = tpsDeb.hour()*60 + tpsDeb.minute();
      int   ofsetX      = DAY_OFS_X;
      int     posX      = 0;
      int     posY      = 0;
      int    mnCur      = 0;
      int        h      = 0;
      int        w      = 0;
      QRect     rp      = m_pBMC->m_HeadResume_Pixmap.rect();
      C_RendezVous *rdv = 0;
      //..................... bitmap de deco ........................
      if (m_pCMoteurAgenda->isFreeDay(m_Date))                  // si jours feries
         {p.drawPixmap (0, 0 , m_pBMC->m_HeadUnWorkDay);
         }
      else
         {if      (m_Date.dayOfWeek() == Qt::Saturday)  p.drawPixmap (0, 0 , m_pBMC->m_HeadSatDay_Pixmap );
          else if (m_Date.dayOfWeek() == Qt::Sunday)    p.drawPixmap (0, 0 , m_pBMC->m_HeadSunDay_Pixmap );
          else                                          p.drawPixmap (0, 0 , m_pBMC->m_HeadCloseDay_Pixmap);
         }
      // ....... bitmap du resume et cadres du resume (deux si double ligne) ..........
      if (nbRdv) p.drawPixmap (ofsetX-5, HEAD_RESUME_OFY, m_pBMC->m_HeadResume_Pixmap );
      p.setPen ("#6b6a6a");
      rp.translate(ofsetX-5, HEAD_RESUME_OFY);                      // positionner le rectangle du haut
      p.drawRoundedRect (rp, 4, 4);
      if  (m_DayLF>0)
          {if (nbRdv) p.drawPixmap (ofsetX-5, LINE_RESUME_HEIGHT + (HEAD_RESUME_OFY << 1) , m_pBMC->m_HeadResume_Pixmap );
           rp.translate(0, LINE_RESUME_HEIGHT + HEAD_RESUME_OFY);   // positionner le rectangle du bas
           p.drawRoundedRect (rp, 4, 4);
          }
      //....... heure courante indicateur graphique trait vertical ..........
      if (m_Date==curDt && m_rafraich>=4 )
         {QTime tm           = QTime::currentTime();
          if ( tm>tpsDeb &&  tm<tpsEnd )  // on calcule maintenant et on affiche le curseur des heures en dernier pour ne pas se faire recouvrir
             {mnCur          = tm.hour()*60+tm.minute();
              QPoint point   = computeResumePosXYFromTimeInMinute(mnCur, mnDeb, nbMnToSee, m_pBMC->m_HeadResume_Pixmap.width()-LINE_RESUME_XMARG, LINE_RESUME_HEIGHT+HEAD_RESUME_OFY, m_DayLF?2:1);
              timePosPixmapX = point.x() + ofsetX;
              timePosPixmapY = point.y() - m_pBMC->m_AgendaTimeArrow_Pixmap.height();
             }
         }
      //..................... graduations horaires horizontales........................
      f.setPointSize(6); f.setBold(FALSE); p.setFont ( f );
      for (int i = 0; nbRdv && i <= nbMnToSee; i += 15)
          {mnCur        = mnDeb + i;
           h            = mnCur/60;
           QPoint point = computeResumePosXYFromTimeInMinute(mnCur, mnDeb, nbMnToSee, m_pBMC->m_HeadResume_Pixmap.width()-LINE_RESUME_XMARG, LINE_RESUME_HEIGHT+HEAD_RESUME_OFY, m_DayLF?2:1);
           posX         = point.x() + ofsetX;
           posY         = point.y()-8;
           if (h*60==mnCur)
              {p.setPen (QColor ( "#ff0000" ));
               p.drawLine (posX , posY, posX, posY-2);
               p.setPen (QColor ( Qt::darkGray ));
               if (h==0)         p.drawText ( QPoint(posX+2, posY+7), QString::number(h) );
               else if (h>9)     p.drawText ( QPoint(posX-5, posY+7), QString::number(h) );
               else              p.drawText ( QPoint(posX-2, posY+7), QString::number(h) );
              }
           else
              {p.setPen (QColor ( Qt::white ));
               p.drawLine (posX , posY, posX, posY-2);
              }
          }
      //................... lire la liste des rendez-vous et afficher le resume colore .....................
      for (int i = 0; i < nbRdv; ++i)
          {//.......... creer et ajouter les rectangles de couleurs des rendez vous .................................
           rdv         = m_cacheRDV_List[i];
           QRect rect2 = QRect(-1,-1,-1,-1);     // est initialise si un rectangle s'etend sur deux lignes (un bout a la ligne d'avant, l'autre sur l'autre ligne)
           QRect rect  = computeResumeRdvGeometry(rdv, mnDeb, nbMnToSee, m_pBMC->m_HeadResume_Pixmap.width()-LINE_RESUME_XMARG, LINE_RESUME_HEIGHT+HEAD_RESUME_OFY, m_DayLF?2:1,rect2);
           w           = rect.width();
           h           = rect.height();
           posX        = rect.x() + ofsetX;
           posY        = rect.y();

           //............ tracer le rectangle normal du rdv (ou du haut) ............
           QColor color(getRdvColor(*rdv));                  // couleur rdv
           p.fillRect ( posX, posY, w, 11, color);           // tracer rectangle plein
           p.setPen (color.darker(120));                     // tracer le contour en le foncant
           p.drawRoundedRect ( posX, posY, w, h, 0, 0);      // radius 0 ->rectangle

           //............ tracer les elements relatifs a un rendez vous sur deux lignes ............
           if (rect2.x()!=-1)           // si le rectangle est coupe en deux (un bout a la ligne d'avant, l'autre sur l'autre ligne) on trace l'autre bout
              {//............ tracer le triangle de droite en haut qui signale la coupure ............
               posX += w;          // se mettre a droite du rectangle
               //p.setPen (color.darker(180));
               //p.drawLine (posX , posY, posX, posY + h);
               ++posX;
               p.setPen (QColor("#FF0000"));
               p.drawLine (posX , posY+2, posX, posY + h -2);
               ++posX;
               p.drawLine (posX , posY+4, posX, posY + h -4);
               ++posX;
               p.drawLine (posX , posY+5, posX, posY + h -5);

               //............ tracer le rectangle du bas ............
               w           = rect2.width();
               posX        = rect2.x() + ofsetX;
               posY        = rect2.y();
               p.fillRect ( posX, posY, w, 11, color);           // tracer rectangle plein
               p.setPen (color.darker(120));                     // tracer le contour en le foncant
               p.drawRoundedRect ( posX, posY, w, h, 0, 0);      // radius 0 ->rectangle

               //............ tracer le triangle de gauche en bas qui signale la coupure ............
               //p.setPen (color.darker(180));
               //p.drawLine (posX , posY , posX, posY + h);
               --posX;
               p.setPen (QColor("#FF0000"));
               p.drawLine (posX , posY+2, posX, posY + h -2);
               --posX;
               p.drawLine (posX , posY+4, posX, posY + h -4);
               --posX;
               p.drawLine (posX , posY+5, posX, posY + h -5);
               //........ retablir valeurs pour tracage du comedon ...................
               //         on trace le point rouge dans le plus grand des rectangles)
               if (rect2.width()>rect.width())
                  {w     = rect2.width();
                   posX  = rect2.x() + ofsetX;
                   posY  = rect2.y() + 4;
                  }
               else
                  {w     = rect.width();
                   posX  = rect.x() + ofsetX;
                   posY  = rect.y() + 4;
                  }
              }
           else // if (rect2.x()!=-1)
              {posY  = rect.y() + 4;             // positionner valeur de l'eventuel tracage du comedon
              }
           //........... si occupe le signaler avec un point noir un comedon quoi heu! rouge............
           //            il est un peu plus gros si double ligne
           if (rdv->m_GUID.length()||rdv->m_Nom.length()||rdv->m_Prenom.length())
              {if (m_DayLF)
                  {p.fillRect ( posX+w/2-1, posY, 4, 4, QColor(Qt::red)); // tracer rectangle plein
                   p.fillRect ( posX+w/2-1, posY, 2, 2, QColor(Qt::red).lighter(160));
                  }
               else
                  {p.fillRect ( posX+w/2, posY, 2, 2, QColor(Qt::red)); // tracer rectangle plein
                   p.fillRect ( posX+w/2, posY, 1, 1, QColor(Qt::red).lighter(160));
                  }
              } // endif commedon a dessiner if (rdv->m_GUID.length()||rdv->m_Nom.length()||rdv->m_Prenom.length())
         }// endif affichage du resume color
     }  // end else if (isDayExpand())

  ////////////////////////////////// COMMON /////////////////////////////////
  //............................ date du jour ................................................
  tmpStr     = getDate().toString(m_pCMoteurAgenda->GetFormatDateInResume(mdw));   // mdw = 'D' or 'W' or 'M'
  p.setFont(fntDate); p.setPen (colorDate);
  br  = fntMetric.boundingRect (tmpStr+"_");
  p.drawText ( QPoint(nextTmpPosX, ofsTxtY ), tmpStr );
  nextTmpPosX += br.width();
  p.setPen (Qt::black);
  //..................... nb rdv........................
  if (nbRdv)
     {tmpStr = QString::number(nbRdv);
      p.setPen (QColor("#FF0000"));
      p.drawText ( QPoint(nextTmpPosX, ofsTxtY), tmpStr);
      nextTmpPosX += fntMetric.boundingRect (tmpStr).width();
      p.setPen (colorDate);
      tmpStr = tr(" Appointment");
      p.drawText ( QPoint(nextTmpPosX, ofsTxtY), tmpStr );
      nextTmpPosX += fntMetric.boundingRect (tmpStr).width();
     }
  // .........si jour courant le signaler par le cadre rouge et l'heure ................
  //  a ce stade nextTmpPosX est la position la plus a droite de la date affichee en bas
  if (m_Date==curDt)       // tracer les rectangles interieurs rouges du jour courant
     { QColor color("#d22f2f");
       p.setPen (color.lighter(180));
       p.drawRoundedRect (2, 2, m_Width-4, m_BaseDayHeight-4, 2, 2);
       p.setPen (color.lighter(195));
       p.drawRoundedRect (3, 3, m_Width-6, m_BaseDayHeight-6, 2, 2);
       p.setPen (color);
       if (m_rafraich>=4)  // tracer le texte de l'heure courante en bas a droite (que si place)
          {
           tmpStr       = QTime::currentTime().toString("hh:mm:ss");
           p.setPen (QColor ( "#d22f2f" ));                                 // couleur est la meme pour mode deploye ou pas
           QSize        ts = fntMetric.size(Qt::TextSingleLine,tmpStr);
           int        posX = width()-ts.width()-4;
           if (posX > nextTmpPosX-4)
               p.drawText ( QPoint(posX, ofsTxtY), tmpStr );
          }
     }
  else
     { p.setPen (Qt::black);
     }

  //............. cadre noir du jour .....................................
  p.drawRoundedRect (1, 1, m_Width-2, m_BaseDayHeight-2, 2, 2);
  if (timePosPixmapX) p.drawPixmap (timePosPixmapX, timePosPixmapY, m_pBMC->m_AgendaTimeArrow_Pixmap );
  //........................ affichage jour du mois ....................................
  if (mdw == 'M'&& m_pCMoteurAgenda->isDayOfMonthToBeDisplay())
     {f.setPointSize(8); f.setBold(TRUE); p.setFont ( f );
      p.setPen (QColor ( "#ae0d0d" ));
      p.drawText ( QPoint(2, 12), QString::number(this->getDate().day()) );
     }
}
//---------------------------------------- generate_cacheRDV_List_FromDayWidget --------------------------------------------
void C_Frm_Day::generate_cacheRDV_List_FromDayWidget()
{m_cacheRDV_List.clear();                      // detruit aussi les objets pointes par les pointeurs
 for (int i = 0; i < C_Frm_RdvList::size(); ++i)
      {C_RendezVous *pC_RendezVous = new C_RendezVous();
       *pC_RendezVous = *at(i);                // on recupere et copie les valeurs du widget
        m_cacheRDV_List.append(pC_RendezVous);  // ne surtout pas ajouter directement le pointeur sur l'instance du rdv du widget
      }
}

//------------------------ Slot_StopTimer ---------------------------------------
void  C_Frm_Day::Slot_StopTimer(int state)
{if (state) ++m_DontRecreateWidget;
 else       --m_DontRecreateWidget;
 if(m_RefreshTimer==0) return;
 if (state)  {m_RefreshTimer->stop();  if (m_pCMoteurAgenda->GetVerboseMode()) qDebug()<< tr("DontCreate set to : %1").arg(m_DontRecreateWidget);}
 else        {m_RefreshTimer->start(); if (m_pCMoteurAgenda->GetVerboseMode()) qDebug()<< tr("DontCreate set to : %1").arg(m_DontRecreateWidget);}
}

//------------------------ event ---------------------------------------
bool C_Frm_Day::event(QEvent * event )
{   int x,y;
    switch ((int)event->type())
    {/*  marche pas ??? clavier non recu ??
     case QEvent::KeyPress:
       {QKeyEvent *pQKeyEvent = (QKeyEvent*) event;
        if (x>= 22 && y>= HEAD_RESUME_OFY && y<= HEAD_RESUME_OFY + 20 && x<=m_Width-4)
           {switch (pQKeyEvent->key ())
               {case Qt::Key_Left:
                     { QCursor::setPos ( QCursor::pos().x()-1, QCursor::pos().y() );
                     }  return TRUE;
                case Qt::Key_Right:
                     { QCursor::setPos ( QCursor::pos().x()+1, QCursor::pos().y() );
                     }  return TRUE;
               }
           }
       }
     */
    case QEvent::ToolTip:
       {QToolTip::showText (QCursor::pos(), toolTip () , this);
        return TRUE;
       }
    case QEvent::MouseMove:
       {
        QMouseEvent *pQMouseEvent = (QMouseEvent*) event;
        x = pQMouseEvent->pos().x();
        y = pQMouseEvent->pos().y();
        C_RendezVous rdv;
        if (isDayExpand())      rdv = getRdvUnderMouse(y);
        else                    rdv = getResumeRdvFromXY ( x, y );
        if (rdv.getDuree() != 0)
           { //QToolTip::setPalette(QPalette(QColor("#fff8dc")));             
             setToolTip (rdv.getQWhatsThisString(Theme::getPath(TRUE), getRdvColor(rdv)));
           }
        else
           {int mn  = -1;
            int nbMnToEnd;
            if (isDayExpand())     mn = posY_toMinutes(y);
            else                   mn = XYToMinutesInResume (x,y, &nbMnToEnd);
            if (mn!=-1)
               {int mnNext;
                getRdvAfterThisTimeInMinutes( mn, &mnNext);
                if (mnNext==-1) mnNext  = nbMnToEnd;    // 24h00
                else            mnNext -= mn;
                int h   = mn/60;
                mn      = mn - (h*60);
                int hD  = mnNext/60;
                mnNext  = mnNext - (hD*60);
                //QToolTip::setPalette(QPalette(QColor("#fff8dc")));             
                setToolTip ("<font color=\"#000000\">"+QString(tr("Free slot at : %1 h %2 mn \nmaximum possible duration:")+ "%3 h %4 mn</font>").arg(QString::number(h),QString::number(mn),QString::number(hD),QString::number(mnNext)));
               }
           }
       }
    } //endswitch event->type()()
    QFrame::event(event);
    return TRUE;
}
//------------------------ getRdvColor ---------------------------------------
QString C_Frm_Day::getRdvColor(const C_RendezVous & rdv)
{   return C_RendezVous::getRdvColor(rdv, m_pColorProfils);
}

//------------------------ getResumeRdvFromXY ---------------------------------------
//  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus,
//         car lors d'un jour replie les widgets sont non disponibles
C_RendezVous C_Frm_Day::getResumeRdvFromXY ( int x, int y )
{   C_RendezVous rdvNull;   rdvNull.m_Duree = 0;
    C_RendezVous *pRdv =  getRdvUnderMouseInResume(x,y);
    if (pRdv) return *pRdv;
    return rdvNull;
}
//------------------------ getNbMinutesToseeInResume ---------------------------------------
int C_Frm_Day::getNbMinutesToseeInResume()
{QTime tpsDeb      = getStartTime();
 QTime tpsEnd      = getStopTime();
 return getNbMinutesToseeInResume(tpsDeb,tpsEnd);
}
//------------------------ getNbMinutesToseeInResume ---------------------------------------
int C_Frm_Day::getNbMinutesToseeInResume(const QTime &tpsDeb, const QTime &tpsEnd)
{return (tpsDeb.secsTo(tpsEnd)/60)+3;
}
 //------------------------ getRdvUnderMouseInResume ---------------------------------------
 //  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus,
 //         car lors d'un jour replie les widgets sont non disponibles
C_RendezVous *C_Frm_Day::getRdvUnderMouseInResume ( int x, int y , int *index /* = 0 */)
{   QTime tpsDeb         = getStartTime();
    QTime tpsEnd         = getStopTime();
    int nbMnToSee        = getNbMinutesToseeInResume(tpsDeb, tpsEnd);
    int nbRdv            = m_cacheRDV_List.count();
    int    mnDeb         = 0;
    int    mnEnd         = 0;
    C_RendezVous *rdv    = 0;
    int   ofsetX         = DAY_OFS_X;
    mnDeb                = tpsDeb.hour()*60 + tpsDeb.minute();
    mnEnd                = mnDeb  + nbMnToSee;
    //................... lire la liste des rendez-vous et afficher le resume colore .....................
    for (int i = 0; i < nbRdv; ++i)
        {//.......... creer et ajouter le widget du rendez vous .................................
         rdv         = m_cacheRDV_List[i];
         QRect rect2 = QRect(-1,-1,-1,-1);  // est initialise si un rectangle s'etend sur deux lignes (un bout a la ligne d'avant, l'autre sur l'autre ligne)
         QRect rect  = computeResumeRdvGeometry(rdv, mnDeb, nbMnToSee, m_pBMC->m_HeadResume_Pixmap.width()-LINE_RESUME_XMARG, LINE_RESUME_HEIGHT+HEAD_RESUME_OFY, m_DayLF?2:1, rect2, LINE_RESUME_HEIGHT + 10);
         rect.setX(rect.x()-1);rect.setWidth(rect.width()+2);   // on agrandi un peu
         if (rect.contains(x - ofsetX , y , TRUE)) {if (index) *index = i; return  rdv;}
         if (rect2.x() != -1)               // si le rectangle est coupe en deux (un bout a la ligne d'avant, l'autre sur l'autre ligne) on verifie aussi si souris dedans l'autre bout
            {rect2.setX(rect2.x()-1);rect2.setWidth(rect2.width()+2);   // on agrandi un peu
             if (rect2.contains(x - ofsetX , y , TRUE)) {if (index) *index = i; return  rdv;}
            }
        }
    if (index) *index = -1;
    return 0;
}

//---------------------------------------- computeResumeRdvGeometry ---------------------------------------------------------------
//....startTimeInMinutes........>|......[1]..........[2].....................[3].............[4]..................|             line 0  <|
//                               |............[5]............[6]............[ 7 ]..............................[ 8|             line 1  <|-- lineH (in pixels)
//                               |  ]............[9]....[10]............[11]......................................|<...EndTime  line 2
//                               ^.........................(widthSegment)        in pixels  ......................^
//                                                         (segmentInMinutes)    in minutes
//                               ^..............^
//                                            (debSegmentInMn) for rdv[9]
//                         from >|            (nbTotalMinutesToSee)            Line 0 + Line 1 + Line 2           |< to EndTime
//

QRect C_Frm_Day::computeResumeRdvGeometry(C_RendezVous *pRdv, int startTimeInMinutes, int nbTotalMinutesToSee, int widthSegment, int lineH, int nbLine, QRect &rect2, int rectH /*=6*/)
{QRect rect;
 int reliquat          = 0;          // reliquat a placer sur l'autre ligne si un rdv deborde sur l'autre ligne
 int segmentInMinutes  = nbTotalMinutesToSee / nbLine;
 int posInMinutes      = pRdv->m_date.time().hour()*60 + pRdv->m_date.time().minute() - startTimeInMinutes;
 int line              = (posInMinutes)     / segmentInMinutes;    // + pRdv->m_Duree pour passer a la ligne si deborde a droite
 int debSegmentInMn    = posInMinutes - line * segmentInMinutes;
 rect.setX (     (widthSegment * debSegmentInMn)  / segmentInMinutes );
 rect.setY(       lineH        * (line+1)-LINE_RESUME_HEIGHT+2);
 //........... verifier si deborde pas a droite ..........................
 //            auquel cas generer le deuxieme rectangle
 reliquat =  (debSegmentInMn + pRdv->m_Duree)-segmentInMinutes;
 if (reliquat>0)
    {rect.setWidth(  ((widthSegment * (pRdv->m_Duree - reliquat) )  / segmentInMinutes) );
     rect2.setWidth( ((widthSegment * (reliquat) )                  / segmentInMinutes) );
     rect2.setY(       lineH        * (line + 2)-LINE_RESUME_HEIGHT+2);
     rect2.setX (0);
     rect2.setHeight(rectH);
    }
 else
    {rect.setWidth( ((widthSegment * pRdv->m_Duree )  / segmentInMinutes) );
    }
 rect.setHeight(rectH);
 return rect;
}

//---------------------------------------- computeResumePosXYFromTimeInMinute ---------------------------------------------------------------
//....startTimeInMinutes........>|......[1]..........[2].....................[3].............[4]..................|             line 0  <|
//                               |............[5]............[6]............[ 7 ]..............................[ 8|             line 1  <|-- lineH (in pixels)
//                               |  ]............[9]....[10]............[11]......................................|<...EndTime  line 2
//                               ^.........................(widthSegment)        in pixels  ......................^
//                                                         (segmentInMinutes)    in minutes
//                               ^..............^
//                                            (debSegmentInMn) for rdv[9]
//                         from >|            (nbTotalMinutesToSee)            Line 0 + Line 1 + Line 2           |< to EndTime
//

QPoint C_Frm_Day::computeResumePosXYFromTimeInMinute(int timeInMinutes, int startTimeInMinutes, int nbTotalMinutesToSee, int widthSegment, int lineH, int nbLine)
{QPoint point;
 int segmentInMinutes  = nbTotalMinutesToSee / nbLine;
 int posInMinutes      = timeInMinutes - startTimeInMinutes;
 int line              = posInMinutes   / segmentInMinutes;
 int debSegmentInMn    = posInMinutes - line * segmentInMinutes;
 point.setX (    (widthSegment * debSegmentInMn)  / segmentInMinutes );
 point.setY(      lineH        * (line+1));
 return point;
}

//------------------------ XYToMinutesInResume ---------------------------------------
int C_Frm_Day::XYToMinutesInResume ( int x, int y, int *retNbMnToEnd /* = 0 */)
{QTime tpsDeb                = getStartTime();
 QTime tpsEnd                = getStopTime();
 int nbTotalMinutesToSee     = getNbMinutesToseeInResume(tpsDeb, tpsEnd);
 int    nbLine               = m_DayLF?2:1;
 int segmentInMinutes        = nbTotalMinutesToSee / nbLine;
 int lineH                   = LINE_RESUME_HEIGHT + HEAD_RESUME_OFY;

 int   ofsetX      = DAY_OFS_X;
 int   posY        = y - HEAD_RESUME_OFY;
 int   posX        = x - ofsetX;         // relativier au pixel signifiant
 //........... determiner la ligne pointee .............................
 int   curY        = 0;
 int     i         = 0;
 for (i=0; i<nbLine; ++i)
     {//qDebug() << tr("Inf=%1 Sup=%2 PosY=%3 i=%4").arg(QString::number(curY),QString::number(curY + lineH),QString::number(posY),QString::number(i));
      if (posY >= curY && posY <= curY + LINE_RESUME_HEIGHT) break;   // si dans fourchette on break
      curY     += lineH;
     }
 if (i==nbLine)  return -1;    // on est pas dans une fourchette de ligne (soit trop bas soit trop haut) cassos avec -1
 //............... calcul ...................................
 int   mnDeb       = tpsDeb.hour()*60 + tpsDeb.minute();
 int   mnCur       = ((posX * segmentInMinutes) / ( m_pBMC->m_HeadResume_Pixmap.width()-LINE_RESUME_XMARG))+(segmentInMinutes*i); mnCur = mnCur/getMagnetisme(); mnCur *=getMagnetisme(); // un peut de granulometrie que diable
 if (retNbMnToEnd) *retNbMnToEnd = nbTotalMinutesToSee-mnCur;
 return mnDeb + mnCur;
}

//---------------------------------------- get_C_Frm_RdvBeforeAndAfter --------------------------------------------
int C_Frm_Day::get_C_Frm_RdvBeforeAndAfter(int y_pos, C_Frm_Rdv **pC_Frm_RdvBefore,  C_Frm_Rdv **pC_Frm_RdvAfter /*=0*/)
{ //................ rechercher le Rdv d'avant et d'apres ............................
  int nearYinf                           = -1;
  int nearYsup                           = 400000;
  *pC_Frm_RdvBefore                      = 0;
  if (pC_Frm_RdvAfter) *pC_Frm_RdvAfter  = 0;
  int  nb          = C_Frm_RdvList::size();
  for (int i = 0; i < nb; ++i)
     {C_Frm_Rdv *pC_Frm_Rdv  = C_Frm_RdvList::at(i);
      int frmYinf = pC_Frm_Rdv->y();
      int frmYsup = pC_Frm_Rdv->y() + pC_Frm_Rdv->getHeight();
      if ( (y_pos    >= frmYinf - 1 && y_pos <= frmYsup + 1) ) return 0;
      if (frmYinf  >= nearYinf && frmYinf <= y_pos)  {nearYinf = frmYinf; *pC_Frm_RdvBefore = pC_Frm_Rdv;}
      if (frmYsup  <= nearYsup && frmYsup >= y_pos)  {nearYsup = frmYsup; if (pC_Frm_RdvAfter) *pC_Frm_RdvAfter  = pC_Frm_Rdv;}
     }
  return 1;
}

//---------------------------- getRdvAfterThisTimeInMinutes ------------------------------------------------
//  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus pour creer la liste des widgets de rendez-vous
//         fait appel a m_cacheRDV_List car appelee en mode replie
C_RendezVous C_Frm_Day::getRdvAfterThisTimeInMinutes(int minutesRef, int *mnRet /* =0 */)
{C_RendezVous rdvNull; rdvNull.m_Duree = -1;
 int nb   = m_cacheRDV_List.count();
 int deb  = -1;
 for (int i  = 0; i < nb; ++i)
     {
      deb = m_cacheRDV_List[i]->m_date.time().hour()*60+m_cacheRDV_List[i]->m_date.time().minute();
       if (deb>minutesRef)
          {if (mnRet) *mnRet = deb;
           return *m_cacheRDV_List[i];
          }
      }
 if (mnRet) *mnRet = -1;
 return rdvNull;
}

//---------------------------- getRdvBeforeThisTimeInMinutes ------------------------------------------------
//  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus pour creer la liste des widgets de rendez-vous
//         fait appel a m_cacheRDV_List car appelee en mode replie
C_RendezVous C_Frm_Day::getRdvBeforeThisTimeInMinutes(int minutesRef, int *mnRet /* =0 */)
{C_RendezVous rdvRet; rdvRet.m_Duree = -1;
 int nb   = m_cacheRDV_List.count();
 if (nb==0) {  if (mnRet) *mnRet = -1;     return rdvRet;}
 int deb  = -1;
 int   i  = nb;
 int last = -1;
 while (i)
    {--i;
     deb = m_cacheRDV_List[i]->m_date.time().hour()*60+m_cacheRDV_List[i]->m_date.time().minute();
     if (deb<minutesRef && deb>=last)
        {last   = deb;
         rdvRet = *m_cacheRDV_List[i];
        }
     }
 if (mnRet) *mnRet = last;
 return rdvRet;
}

//---------------------------------------- isCopyExist --------------------------------------------
bool C_Frm_Day::isCopyExist()
{ return QApplication::clipboard()->mimeData()->hasFormat("text/medintux_rdv");
}

//---------------------------------------- getCopy --------------------------------------------
C_RendezVous C_Frm_Day::getCopy()
{ C_RendezVous rdv;
  const QMimeData *mimeData   = QApplication::clipboard()->mimeData();
  if (mimeData->hasFormat ("text/medintux_rdv" ))
     {rdv = C_RendezVous::unSerialize(QString(mimeData->data ( "text/medintux_rdv" )));
     }
  return rdv;
}

//---------------------------------------- Slot_ReplaceRdvByDroppedRdv --------------------------------------------
void C_Frm_Day::Slot_ReplaceRdvByDroppedRdv(C_Frm_Rdv *pC_Frm_Rdv_Dst)
{ LOOKREFRESH;
  C_RendezVous rdvSrc;
  getLastDroppedData(rdvSrc.m_Nom, rdvSrc.m_Prenom, rdvSrc.m_Tel, rdvSrc.m_GUID, rdvSrc.m_Where);
  if (rdvSrc.m_GUID.length()) replaceRdvByRdv(pC_Frm_Rdv_Dst, rdvSrc);
  UNLOOKREFRESH;
}
//---------------------------------------- replaceRdvByRdv --------------------------------------------
void C_Frm_Day::replaceRdvByRdv(C_Frm_Rdv *pC_Frm_Rdv_Dst, const C_RendezVous &rdvSrc)
{

  if (pC_Frm_Rdv_Dst == 0)        return;
  if (pC_Frm_Rdv_Dst->m_GUID.length()||pC_Frm_Rdv_Dst->m_Nom.length()||pC_Frm_Rdv_Dst->m_Prenom.length()) // si rendez-vous d\303\251j\303\240 attribu\303\251 demander confirmation du changement
     {int ret   = QMessageBox::warning ( this, tr("Modify an appointment :"),
                                         QString( "<u><b>"+tr("WARNING")+"</u> :<br /></b>"+tr("The current appointment taken for:")+"<br />"
                                             " <b><font color=\"#1200ff\">%1</font></b><hr>").arg( pC_Frm_Rdv_Dst->m_Nom  + " " + pC_Frm_Rdv_Dst->m_Prenom ) +
                                         QString(tr( "be changed and be given to:")+"<br />"
                                             "<b><font color=\"#ff0000\">%1</font></b><br />").arg(rdvSrc.m_Nom+' '+rdvSrc.m_Prenom) +
                                         tr("Confirm modification ?"),
                                         tr("&Modify"), tr("&Cancel"), 0, 1, 1
                                       );
      if (ret==1)                  return;
     }
  //............. modifier le widget ................................................
  pC_Frm_Rdv_Dst->m_GUID   = rdvSrc.m_GUID;
  pC_Frm_Rdv_Dst->m_Nom    = rdvSrc.m_Nom;
  pC_Frm_Rdv_Dst->m_Prenom = rdvSrc.m_Prenom;
  pC_Frm_Rdv_Dst->m_Tel    = rdvSrc.m_Tel;
  pC_Frm_Rdv_Dst->m_Where  = rdvSrc.m_Where;
  if (pC_Frm_Rdv_Dst->m_textLabel_Nom) pC_Frm_Rdv_Dst->m_textLabel_Nom->setText(rdvSrc.m_Nom + " " + rdvSrc.m_Prenom);
  pC_Frm_Rdv_Dst->m_ButtonAcceder->setEnabled(TRUE);
  pC_Frm_Rdv_Dst->m_ButtonAcceder->setIcon(m_pBMC->m_ButtonAcceder_Pixmap);
  pC_Frm_Rdv_Dst->RDV_Update(); // mettre a jour le RDV la liste du cache et enregistrer dans la base
}

//---------------------------------------- getRdvUnderMouse --------------------------------------------
C_RendezVous C_Frm_Day::getRdvUnderMouse(int y_pos)
{   //................... parcourir la liste des rendez vous ........................
    //                    verifier si souris  sur un des rendez vous
    C_RendezVous rdvNull; rdvNull.m_Duree = 0;
    int y_last     = -2;
    int i          = 0;
    C_Frm_Rdv *pC_Frm_Rdv     = 0;
    int                   nb  = C_Frm_RdvList::size();
    if (nb==0)   { return rdvNull;}
    for ( i = 0; i < nb; ++i )
        {pC_Frm_Rdv = C_Frm_RdvList::at(i);
         y_last     = pC_Frm_Rdv->y() + pC_Frm_Rdv->getHeight();
         if (y_pos >= pC_Frm_Rdv->y()-1 && y_pos <=  y_last+1)  break;
        }
    //.................. si souris sur rendez-vous cassos .............................................
    if (i>=nb)    { return rdvNull;}
    else          { return *pC_Frm_Rdv->rdv_instance();}
}

//---------------------------------------- getLastDroppedData --------------------------------------------
int C_Frm_Day::getLastDroppedData(QString &nom, QString &prenom, QString &tel, QString &guid, QString &where)
{nom      = "";
 prenom   = "";
 tel      = "";
 guid     = "";
 if (m_DropedRdv.m_GUID.length()==0) return 0;
 nom      = m_DropedRdv.m_Nom.replace("&nbsp;"," ").replace("&NBSP;"," ");
 prenom   = m_DropedRdv.m_Prenom.replace("&nbsp;"," ").replace("&NBSP;"," ");
 tel      = m_DropedRdv.m_Tel;
 guid     = m_DropedRdv.m_GUID;
 where    = m_DropedRdv.m_Where;
 m_DropedRdv.m_GUID = ""; // on ne sert des donnees qu'une fois
 return 1;
}

//---------------------------- adjustToMagnetisme ------------------------------------------------
int    C_Frm_Day::adjustToMagnetisme(int value)
{value       -= (FIRST_DAY_POS_Y + m_BaseDayHeight);        // enlever les ofset non significatifs pour le temps
 int pixMag   = getMagnetisme() * getResoPixByMinutes();
 int midleMag = pixMag / 2;
 value        = (value+midleMag)/pixMag;
 return (value*pixMag) + (FIRST_DAY_POS_Y + m_BaseDayHeight);
}

//---------------------------------------- mousePressEvent --------------------------------------------
void C_Frm_Day::mousePressEvent ( QMouseEvent * event )
{On_Day_mousePressEvent ( event );
}

//---------------------------------------- On_Day_mousePressEvent --------------------------------------------
void C_Frm_Day::On_Day_mousePressEvent ( QMouseEvent * event )
{ int ymgn       = event->y();// sera ajuste au magnetisme
  QString   type   = "";
  QDateTime dt;
  QDateTime dtLast;
  QString   nom ,prenom ,tel, guid, where;
  int       isDropedData = getLastDroppedData(nom, prenom, tel, guid, where);
  LOOKREFRESH;
  if (isDayExpand())
     {int y_pos      =  ymgn;
      int i          =  0;
      int y_last     = -2;
      int y_lastRdv  = -1;
      //................... parcourir la liste des rendez vous ........................
      //                    verifier si souris  sur un des rendez vous
      C_Frm_Rdv *pC_Frm_Rdv     = 0;
      int                   nb  = C_Frm_RdvList::size();
      for ( i = 0; i < nb; ++i )
          {pC_Frm_Rdv = C_Frm_RdvList::at(i);
           y_last     = pC_Frm_Rdv->y() + pC_Frm_Rdv->getHeight();
           if (y_pos >= pC_Frm_Rdv->y()-1 && y_pos <=  y_last+1)  break;
          }
      //.................. si souris sur pC_Frm_Rdv-vous cassos car sera traite inside pC_Frm_Rdv .............................................
      if (i<nb)
         { UNLOOKREFRESH;    return;
         }

      //................ rechercher le Rdv d'avant et d'apres ............................
      C_Frm_Rdv *pC_Frm_RdvBefore = 0;
      get_C_Frm_RdvBeforeAndAfter(y_pos, &pC_Frm_RdvBefore);      // cassos si on est dans un rdv

      if (pC_Frm_RdvBefore==-0)  y_lastRdv = 0; // si cliqu\303\251 apr\303\251s le der des der (en a pas apres pour le test)
      else                       y_lastRdv = pC_Frm_RdvBefore->y() + pC_Frm_RdvBefore->height();      // si cliqu\303\251 apr\303\251s le der des der (en a pas apres pour le test)

      //................... souris sur espace libre .....................................................
      ymgn     = adjustToMagnetisme(y_pos);
      dt       = QDateTime(m_Date, posY_toTime(ymgn));     // recuperer la date li\303\251e \303\240 la position souris
      dtLast   = QDateTime(m_Date,posY_toTime(y_lastRdv)); // recuperer la date li\303\251e au dernier rendez-vous en arri303\250re de la souris
      if (pC_Frm_RdvBefore) type = pC_Frm_RdvBefore->m_Type;
     }
  else
     {int index          = -1;
      C_RendezVous *pRdv = getRdvUnderMouseInResume ( event->x(), event->y() , &index);
      if (pRdv)
         {if (isDropedData)
             {
              if (pRdv->m_GUID.length()||pRdv->m_Nom.length()||pRdv->m_Prenom.length()) // si rendez-vous d\303\251j\303\240 attribu\303\251 demander confirmation du changement
                 {int ret   = QMessageBox::warning ( this, tr("Modify an appointment :"),
                                                     tr( "<u><b>WARNING</u> :<br /></b> The current appointment taken for :")+QString("<br />"
                                                         " <b><font color=\"#1200ff\">%1</font></b><hr>").arg( pRdv->m_Nom  + " " + pRdv->m_Prenom ) +
                                                     tr( "will be given to:")+QString("<br />"
                                                         "<b><font color=\"#ff0000\">%1</font></b><br />").arg(nom+' '+prenom) +
                                                     tr( "Confirm modification ?"),
                                                     tr("&Modify"), tr("&Cancel"), 0, 1, 1
                                                   );
                  if (ret==1)  {UNLOOKREFRESH; return; }
                 }
              pRdv->m_GUID   = guid;
              pRdv->m_Nom    = nom;
              pRdv->m_Prenom = prenom;
              pRdv->m_Tel    = tel;
              pRdv->m_Where  = where;
              QString errMess; if ( ! m_pCMoteurAgenda->RDV_Update(*pRdv, &errMess) ) qDebug() << errMess;
              update();
              UNLOOKREFRESH;
              return;
             } // endif (isDropedData)
          //.............. actionner le menu des RDV ......................................
          else /*if (pRdv->m_GUID.length()||pRdv->m_Nom.length()||pRdv->m_Prenom.length())*/
             { QString ret = doRdvMenu(pRdv,1);
               // "Copy" "Replace" "Cut" "Type :" "Status :"  "Modify"  "Open" "Create" "Anomymize" "Quit"
               if (ret.length())
                  {if (ret.indexOf("Status :") != -1)
                      { QString statut = ret.remove("Status :");
                        pRdv->m_State  = statut.trimmed();
                      }
                   else if (ret.indexOf("Type :") != -1)
                      { QString type = ret.remove("Type :");
                        pRdv->m_Type = type.trimmed();
                      }
                   else if (ret.indexOf("Delete") != -1)  // il ne  peut pas se detruire lui meme et sortir ensuite de sa fonction donc QTIMER
                      {if (index != -1)
                          {if (m_pCMoteurAgenda->isModifToConfirm()&2  &&
                               QMessageBox::question ((QWidget*)parent(),
                                                      tr("Confirm deletion"),
                                                      tr("Confirm deletion this appointment "),
                                                      QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Cancel)
                              {UNLOOKREFRESH;
                               return;
                              }
                           if (m_pCMoteurAgenda->RDV_Delete(pRdv->m_PrimKey))
                             {m_cacheRDV_List.removeIndex(index);
                              update();
                              UNLOOKREFRESH;
                              return;
                             }
                          }
                      }
                   else if (ret.indexOf("Cut") != -1)  // il ne  peut pas se detruire lui meme et sortir ensuite de sa fonction donc QTIMER
                      {if (m_pCMoteurAgenda->isModifToConfirm()&2  &&
                           QMessageBox::question ((QWidget*)parent(),
                                                  tr("Confirm deletion"),
                                                  tr("Confirm deletion this appointment"),
                                                  QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Cancel)
                          {UNLOOKREFRESH;
                           return;
                          }
                       if (index != -1)
                          {copyRdv(*pRdv);
                           if (m_pCMoteurAgenda->RDV_Delete(pRdv->m_PrimKey))
                              {m_cacheRDV_List.removeIndex(index);
                               update();
                               UNLOOKREFRESH;
                               return;
                              }
                          }
                      }
                   else if (ret.indexOf("Copy") != -1)
                      { copyRdv(*pRdv);
                       UNLOOKREFRESH;
                       return;
                      }
                   else if (ret.indexOf("Replace") != -1)
                      {C_RendezVous    rdv = getCopy();    // on recupere le rendez-vous
                       pRdv->m_Nom     = rdv.m_Nom;
                       pRdv->m_Prenom  = rdv.m_Prenom;
                       pRdv->m_Note    = rdv.m_Note;
                       pRdv->m_Where   = rdv.m_Where;
                       pRdv->m_GUID    = rdv.m_GUID;
                       // pRdv->m_PrimKey = rdv.m_PrimKey; si on fait un replace la pk est celle du RDV original de destination et non celui du source
                       pRdv->m_Tel     = rdv.m_Tel;
                       pRdv->m_Type    = rdv.m_Type;
                       pRdv->m_State   = rdv.m_State;
                      }
                   else if (ret.indexOf("Modify") != -1)
                      {rdvPropertyDialog(pRdv);
                      }
                   else if (ret.indexOf("Anomymize") != -1)
                      {pRdv->m_Nom    ="";
                       pRdv->m_Prenom ="";
                       pRdv->m_GUID   ="";
                       pRdv->m_Tel    ="";
                       pRdv->m_Note   ="";
                       pRdv->m_Where  ="";
                      }
                   else if (ret.indexOf("Open") != -1)
                      {emit Sign_LauchPatient(pRdv->m_GUID, pRdv);
                      }
                   else if (ret.indexOf("Create") != -1)
                      {emit Sign_LauchPatient(pRdv->m_GUID, pRdv);
                      }
                   else if (ret.indexOf("Find Appointments") != -1)
                      {Chercher_les_RDV_dun_patient();
                      }
                   else if (ret.startsWith("AppointmentsListFor"))
                      {Chercher_les_RDV_dun_patient(ret.mid(19));
                      }
                   else if (ret.startsWith("PrintAppointments"))
                      {QDateTime Dtrdv = QDateTime::fromString(ret.mid(17,10) + " 00:00:00","yyyy-MM-dd hh:mm:ss");
                       Imprimer_les_RDV_dun_medecin(Dtrdv, ret.mid(28));
                      }
                   //PRINTRDV deb
                   else if (ret.startsWith("PrintThisAppointment"))
                      {
                       QString nomMed = ret.mid(ret.indexOf(";")+1);
                       QString keyRDV = ret.mid(20,ret.indexOf(";") -20);
                       Imprimer_ce_RDV(nomMed,keyRDV);
                      }
                   //PRINTRDV fin

                  }
               QString errMess; if ( ! m_pCMoteurAgenda->RDV_Update(*pRdv, &errMess) ) qDebug() << errMess;
               update();
               UNLOOKREFRESH;
               return;
             } // endif else (isDropedData)
         } // endif (pRdv)
      C_RendezVous rdv;
      int    mn  = XYToMinutesInResume (event->x(),event->y()); if (mn==-1) {UNLOOKREFRESH;    return;}
      int     h  = mn/60;
      rdv        = getRdvBeforeThisTimeInMinutes(mn);
      if (rdv.m_Duree>=0)
         {dtLast = rdv.getDateTime().addSecs(rdv.m_Duree*60); // ajouter duree pour se placer apres
          type   = rdv.m_Type;
         }
      else
         {dtLast = QDateTime(m_Date,getStartTime());
          type   = "";
         }
      mn         = mn - (h*60);
      dt         = QDateTime(m_Date, QTime(h,mn));     // recuperer la date li\303\251e \303\240 la position souris
     }

  //................ menu ....................
  //Theme::setFontSize_Menu(9);
  QStringList optionList;
  optionList<<"$Agenda";
  if (guid.length())
     {optionList<<"=8=#Agenda/NewDocWithIdentAfter.png#"+tr("Make an appointment after the previous existing one for the %2 at %3 an appointment with the patient '%1' selected in the list.").arg(nom+ " " +prenom,dtLast.date().toString("dd-MM-yyyy"),dtLast.time().toString("hh|mm").replace('|','h'));
      optionList<<"=3=#Agenda/NewDocWithIdent.png#"+tr("Make an appointment for the %2 at %3 with the patient '%1' selected in the list.").arg(nom+ " " +prenom, dt.date().toString("dd-MM-yyyy"),dt.time().toString("hh|mm").replace('|','h'));
      optionList<<"-----------";
     }
  optionList<<"=7=#Agenda/NewDocAfter.png#"+tr("Make an appointment for the %2 at %3 after the previous existing one").arg(dtLast.date().toString("dd-MM-yyyy"),dtLast.time().toString("hh|mm").replace('|','h'));
  optionList<<"=6=#Agenda/NewDoc.png#"+tr("Make an appointment for the %1 at  %2").arg(dt.date().toString("dd-MM-yyyy"),dt.time().toString("hh|mm").replace('|','h'));
  optionList<<"=1=#Agenda/NewDoc.png#"+tr("Create for the %1 at %2 an empty appointment with no identity").arg(dt.date().toString("dd-MM-yyyy"),dt.time().toString("hh|mm").replace('|','h'));

  //optionList<<tr("=11=#Agenda/NewDoc.png#Display this day");    // CZA inutile car en bouton
  optionList<<tr("=12=#Agenda/GetPatientRdv.png#Find appointments for a patient");   // CZE
  optionList<<tr("=13=#Agenda/PrintListRdv.png#Print appointments for the user:%1").arg(m_UserNomPrenom);          // CZF

  optionList<<"-----------";

  //.......................... menu coller si rendez-vous en buffer de copie existe ...............
  if (isCopyExist())
     {C_RendezVous rdv = getCopy();
      if (rdv.m_Nom.trimmed().length()||rdv.m_Prenom.trimmed().length())
         {optionList<<"=4=#Agenda/editpaste.png#"+tr("Paste appointment with the patient '%1' at this date : %2 at this hour: %3").arg(rdv.m_Nom.trimmed()+" "+rdv.m_Prenom.trimmed(),
                                                                                                                    dt.date().toString("dd-MM-yyyy"),
                                                                                                                    dt.time().toString("hh|mm").replace('|','h')
                                                                                                                   );
          optionList<<"=9=#Agenda/editpasteLast.png#"+tr("Paste after the previous existing one the appointment with the patient '%1' ").arg(rdv.m_Nom.trimmed()+" "+rdv.m_Prenom.trimmed());
         }
      else
         {optionList<<"=4=#Agenda/editpaste.png#"+tr("Paste at this date : %1 and at this hour : %2 the appointment from copy memory").arg(dt.date().toString("dd-MM-yyyy"),
                                                                                                       dt.time().toString("hh|mm").replace('|','h')
                                                                                                      );
          optionList<<"=9=#Agenda/editpasteLast.png#"+tr("Paste after the previous existing one the appointment from the copy memory.");
         }
     }

  optionList<<"=5=#Agenda/configure.png#"+tr("Configure the types of appointments");
  optionList<<"-----------";
  optionList<<"=2=#Agenda/QuitterMenu.png#"+tr("Quit this menu");
  int                     opt = ThemePopup(optionList,this, " border: 1px solid #8f8f91; border-radius: 6px; font-size: 11px;").DoPopupList().trimmed().toInt();
  C_RendezVous* pC_RendezVous = 0;        // si new apres --> ne pas deleter car append
  switch (opt)
     {case 5: {C_Dlg_RdvTypeConfig *pC_Dlg_RdvTypeConfig = new C_Dlg_RdvTypeConfig(m_pColorProfils, m_pCMoteurAgenda, this);
               if (pC_Dlg_RdvTypeConfig==0)                              {UNLOOKREFRESH;    return;}
               pC_Dlg_RdvTypeConfig->exec();
               delete pC_Dlg_RdvTypeConfig;                              {UNLOOKREFRESH;    return;}
              }
     case 6:  {newRDVAtThisDate(dt, 15, type);                           {UNLOOKREFRESH;    return;}
              }
     case 7:  {newRDVAtThisDate(dtLast, 15, type);                       {UNLOOKREFRESH;    return;}
              }
     case 8:  {newRDVAtThisDate(dtLast, 15, type,nom,prenom,tel,guid, where);   {UNLOOKREFRESH;    return;}
              }
     case 3:  {newRDVAtThisDate(dt, 15, type, nom, prenom, tel, guid, where);   {UNLOOKREFRESH;    return;}
              }
     case 4:  {C_RendezVous rdv = getCopy();       // paste
               pC_RendezVous    = new C_RendezVous(dt , rdv.m_Duree, rdv.m_Nom, rdv.m_Prenom, rdv.m_Tel, rdv.m_Note, rdv.m_GUID, m_SignUser, m_User, rdv.m_Type,"",rdv.m_State,rdv.m_Where);
              } break;
     case 9:  {C_RendezVous rdv = getCopy();       // paste last
               pC_RendezVous    = new C_RendezVous(dtLast , rdv.m_Duree, rdv.m_Nom, rdv.m_Prenom, rdv.m_Tel, rdv.m_Note, rdv.m_GUID, m_SignUser, m_User, rdv.m_Type,"",rdv.m_State,rdv.m_Where);
              } break;
     case 1:  {pC_RendezVous    = new C_RendezVous(dt , 15, "", "", "", "", "", m_SignUser, m_User, ""); // anonyme
              } break;
     case 11: { emit (QDate::currentDate());
                return;
              } break;
     case 12: { Chercher_les_RDV_dun_patient();         // CZE 2
                return;                                 // CZE 2
              } break;
     case 13: { Imprimer_les_RDV_dun_medecin(dt ,m_SignUser);         // CZF
                return;                                 // CZF
              } break;
     default: {UNLOOKREFRESH;
               return;
              }
     }
  if (pC_RendezVous==0)                                         {UNLOOKREFRESH;    return;}
  appendNewRDVItem(pC_RendezVous);    // ne pas effacer pC_RendezVous car appendNewRDVItem s'en charge si besoin
  UNLOOKREFRESH;
}

//---------------------------- doRdvMenu ------------------------------------------------
QString C_Frm_Day::doRdvMenu(C_RendezVous *pRdvDst, int isOptionDetruire  /* = 0 */)
{

    // "Copy" "Replace" "Cut" "Type :" "Status :"  "Modify"  "Open" "Anomymize" "Quit"
    C_QMenuRdv menu(tr("Appointment available"),(QWidget*)parent());
    menu.setStyleSheet("font-size: 12px");  // "border: 1px solid #8f8f91; border-radius: 6px; font-size: 11px; color:#000000;border-width: 3px;  border-style: solid;  border-color: blue; background: yellow; icon-size:16px"
    menu.addAction (m_pBMC->m_Configure_Pixmap,     tr("Modify the parameters of this appointment...")
                   )->setData ("Modify");
    menu.addSeparator ();
    //..............menu clasique copier coller couper ...............................
    menu.addAction (m_pBMC->m_Copier_Pixmap, tr("Copy the current appointment to the copy memory")
                   )->setData ("Copy");

    //.......................... menu coller si rendez-vous en buffer de copie existe ...............
    C_RendezVous rdvCopy = getCopy();
    if (isCopyExist())
    {
        if (rdvCopy.m_Nom.trimmed().length()||rdvCopy.m_Prenom.trimmed().length())
            menu.addAction (m_pBMC->m_Paste_Pixmap,
                            tr("Replace the current appointment with the one inside the copy memory with the name of the patient: '%1' ").arg(rdvCopy.m_Nom.trimmed()+" "+rdvCopy.m_Prenom.trimmed())
                           )->setData ("Replace");
        else
            menu.addAction (m_pBMC->m_Paste_Pixmap, tr("Replace the current appointment with the one inside the copy memory.")
                           )->setData ("Replace");
    }
    menu.addAction (m_pBMC->m_Cut_Pixmap, tr("Cut the current appointment and put into the copy memory")
                   )->setData ("Cut");
    //.............. option recherche de tous les RDV d'un patient ....................
    menu.addSeparator ();
    if (pRdvDst->m_Nom.length()||pRdvDst->m_Prenom.length())
       { menu.addAction (m_pBMC->m_GetPatientRdv, tr("List of all Appointments for: %1").arg(pRdvDst->m_Nom+" "+pRdvDst->m_Prenom)
                   )->setData (QString("AppointmentsListFor%1").arg(pRdvDst->m_Nom+";"+pRdvDst->m_Prenom+";"+pRdvDst->m_GUID));
       }
    else
       { menu.addAction (m_pBMC->m_GetPatientRdv, tr("Find Appointments")
                )->setData ("FindAppointments");
       }
    menu.addAction (m_pBMC->m_PrintListRdv, tr("Imprimer ce rendez-vous")
                    )->setData (QString("PrintThisAppointment%1").arg(pRdvDst->m_PrimKey +";"+pRdvDst->m_PrisAvec)); // PRINTRDV

    menu.addAction (m_pBMC->m_PrintListRdv, tr("Print appointments for the user:%1").arg(m_UserNomPrenom)
                    )->setData (QString("PrintAppointments%1").arg(pRdvDst->m_date.toString("yyyy-MM-dd")+";"+pRdvDst->m_PrisAvec));

    menu.addSeparator ();
    //............. creer le menu des types avec les types couleurs .....................
    QLabel grabLabel;
    grabLabel.setGeometry (0, 0, 16, 16);
    QMap<QString, C_ColorType>::const_iterator it = m_pColorProfils->constBegin();

    grabLabel.setStyleSheet(QString("border: 2px solid #010101; background-color: #FFFFFF;"));
    QPixmap pix = QPixmap::grabWidget (&grabLabel, 0, 0, 16, 16 );
    while (it != m_pColorProfils->constEnd())
       { C_ColorType ct = it.value();
         grabLabel.setStyleSheet(QString("border: 2px solid #010101; background-color: ")+ct.getColor()+";");
         pix = QPixmap::grabWidget (&grabLabel, 0, 0, 16, 16 );
         if (it.key().trimmed().length())
            {menu.addAction (QIcon(pix), tr("Appointment of type: %1").arg(it.key())
                            )->setData (QString("Type :%1").arg(it.key()));
            }
        ++it;
    }
    //............. creer le menu des statuts.....................
    menu.addSeparator ();
    QMapIterator<QString, QPixmap> ut(m_pBMC->m_StatutsPixmap);
    while (ut.hasNext())
       {ut.next();
        menu.addAction (ut.value(), tr("Appointment Status: %1").arg(ut.key())
                       )->setData (QString("Status :%1").arg(ut.key()));
       }
    menu.addSeparator ();
    if (pRdvDst->m_GUID.length())
       { menu.addAction (m_pBMC->m_ButtonAcceder_Pixmap, tr("Open file: %1").arg(pRdvDst->m_Nom+" "+pRdvDst->m_Prenom)
                        )->setData ("Open");
       }
    else
       { menu.addAction (m_pBMC->m_ButtonCreateDoss, tr("Create new file with: %1").arg(pRdvDst->m_Nom+" "+pRdvDst->m_Prenom)
                        )->setData ("Create");
       }

    menu.addSeparator ();
    menu.addAction (m_pBMC->m_IdentityDelete_Pixmap,  tr("Make this appointment anonymous and available")
                   )->setData ("Anomymize");
    if (isOptionDetruire) menu.addAction (m_pBMC->m_MenuRendezvousDel,    tr("Delete this appointment")
                                         )->setData ("Delete");
    menu.addSeparator ();
    menu.addAction (m_pBMC->m_QuitterMenu_Pixmap,   tr("Quit this menu")
                   )->setData ("Quit");
    grabLabel.hide();
    QPoint pt = QCursor::pos();
    //.......... si on est sur un rendez occupe vous afficher le titre .........................
    if (pRdvDst->m_Nom.length()||pRdvDst->m_Prenom.length())
    {menu.setLabelRdvText(QString("<b><font color=\"#ff0000\">%1</font>/<font color=\"#ff0000\">%2</font> <font color=\"#000000\">%3</font></b>").arg(pRdvDst->m_date.time().toString("hh:mm"), QString::number(pRdvDst->m_Duree), pRdvDst->m_Nom+" "+pRdvDst->m_Prenom),
                          pRdvDst->m_Type,
                          C_RendezVous::getRdvColor(*pRdvDst, m_pColorProfils)
                         );
    }
    //.......... si on est sur un rendez vous libre afficher disponible .........................
    else
    {menu.setLabelRdvText(QString("<b><font color=\"#ff0000\">%1</font>/<font color=\"#ff0000\">%2</font> <font color=\"#000000\">%3</font></b>").arg(pRdvDst->m_date.time().toString("hh:mm"), QString::number(pRdvDst->m_Duree), tr(" Appointment available ")),
                          pRdvDst->m_Type,
                          C_RendezVous::getRdvColor(*pRdvDst, m_pColorProfils)
                         );
    }
    QAction  *pQAction =  menu.exec (pt);
    QString        ret = "";
    if (pQAction)  ret = pQAction->data().toString();
    return ret;
}
//---------------------------------------- copyRdv --------------------------------------------
void C_Frm_Day::copyRdv(const C_RendezVous &rdv)
{QByteArray data(rdv.serialize().toLatin1());
 QMimeData *mimeData = new QMimeData;
 mimeData->setData("text/medintux_rdv", data);
 QClipboard *clipboard = QApplication::clipboard();
 clipboard->setMimeData ( mimeData );    // a ne pas deleter
}

//---------------------------------------- newRDVAtThisDate --------------------------------------------
void C_Frm_Day::newRDVAtThisDate(QDateTime dateTime, int duree, const QString &type /*=""*/, const QString &nom /*=""*/, const QString &prenom /*=""*/, const QString &tel /*=""*/, const QString &guid /*=""*/, const QString &where /*=""*/, const QString &pk /*=""*/)
{// ne pas looker c'est fait avant l'appel
    C_RendezVous *pC_RendezVous = new C_RendezVous(dateTime, duree,"","","","","",m_SignUser, m_User);
     if (type.length())   pC_RendezVous->m_Type    = type;
     if (nom.length())    pC_RendezVous->m_Nom     = nom;
     if (prenom.length()) pC_RendezVous->m_Prenom  = prenom;
     if (tel.length())    pC_RendezVous->m_Tel     = tel;
     if (guid.length())   pC_RendezVous->m_GUID    = guid;
     if (where.length())  pC_RendezVous->m_Where   = where;
     if (pk.length())     pC_RendezVous->m_PrimKey = pk;

     C_Dlg_RdvTypeConfig *pC_Dlg_RdvTypeConfig   = new C_Dlg_RdvTypeConfig(m_pColorProfils, m_pCMoteurAgenda, this, pC_RendezVous, 1);
     if (pC_Dlg_RdvTypeConfig)
        {pC_Dlg_RdvTypeConfig->setCaption(tr("Set new appointment for %1 on the %2").arg(m_UserNomPrenom, dateTime.toString("dd MMMM yyyy")));
         if (pC_Dlg_RdvTypeConfig->exec() == QDialog::Accepted)
            { appendNewRDVItem(pC_RendezVous);    // ne pas effacer pC_RendezVous car appendNewRDVItem
              if (pC_Dlg_RdvTypeConfig->m_ui->checkBox_ImprimerCeRDV->isChecked())        // SUITE PRINTRDV
                  Imprimer_ce_RDV(pC_RendezVous->m_PrisAvec,pC_RendezVous->m_PrimKey);    // SUITE PRINTRDV
              delete pC_Dlg_RdvTypeConfig;
              return;
            }
        }
     delete pC_RendezVous;                                           // effacer pC_RendezVous car pas appendNewRDVItem
}

//---------------------------- appendNewRDVItem ------------------------------------------------
// ATTENTION destruction de pC_RendezVous si echec
C_Frm_Rdv* C_Frm_Day::appendNewRDVItem(C_RendezVous *pC_RendezVous)
{     // ne pas looker c'est fait avant l'appel
      QString       errMess = "";
      C_Frm_Rdv *pC_Frm_Rdv = 0;
      //................... enregistrer le rendez vous dans la base ..........................................
      if ( (pC_RendezVous->m_PrimKey = m_pCMoteurAgenda->RDV_Create(*pC_RendezVous, &errMess)).length()==0)
         { delete pC_RendezVous;
           return 0;
         }
      //...................creer le widget rendez vous dans celui du jour .......................
      pC_Frm_Rdv = new C_Frm_Rdv( pC_RendezVous,
                                  getResoPixByMinutes(),                    // resolution en pixels
                                  getMagnetisme(),
                                  m_pQRubberBand,
                                  m_pCMoteurAgenda,
                                  m_pColorProfils,
                                  m_pBMC,
                                  m_pCMoteurAgenda->GetMinDaysHeight(),
                                  this
                                );
      if (pC_Frm_Rdv==0)
         {delete pC_RendezVous;
          return 0;
         }
      //generate_cacheRDV_List_FromDayWidget();    // synchroniser le cache --> surtout pas car si en mode replie pas de widgets dispos
      m_cacheRDV_List.appendRdv(pC_RendezVous);    // donc on ajoute donc simplement le rdv a la liste et ca le fait

      if (isDayExpand())                // SI jour deploye on ajoute le widget graphique et on ajuste les details graphiques
         { pC_Frm_Rdv->adjustWidgetToMagnetisme();      // faut recadrer au contraintes de magnetisme
           readjustListWidgetPositions();               // revoir toutes les positions car le debut peut avoir change si place avant la limite sup ou inf tout ca
           append_C_Frm_Rdv(pC_Frm_Rdv );
           pC_Frm_Rdv->show();
           ReArangeIfDayHeightChange();
         }
      else                              // SI jour replie on ajoute pas le widget graphique et meme on le detruit
         { delete pC_Frm_Rdv;
           pC_Frm_Rdv = 0;
           update();                                  // faut aussi reafficher le jour entier pour que le RDV soit visible
         }
      return pC_Frm_Rdv;
}

//---------------------------- Slot_ButtonAccederClicked ------------------------------------------------
void C_Frm_Day::Slot_ButtonAccederClicked(const char*, void *ptr)
{if (ptr==0) return;
 LOOKREFRESH;     // bloquer le raffraississement
 C_Frm_Rdv *pC_Frm_Rdv = (C_Frm_Rdv*)ptr;
 emit Sign_LauchPatient(pC_Frm_Rdv->m_GUID, pC_Frm_Rdv);
 UNLOOKREFRESH;
}

//---------------------------- Slot_ButtonRDVDeleteClicked ------------------------------------------------
void C_Frm_Day::Slot_ButtonRDVDeleteClicked(const char*, void *ptr)
{LOOKREFRESH;     // bloquer le raffraississement
 if (m_pCMoteurAgenda->isModifToConfirm()&2  &&
     QMessageBox::question ((QWidget*)parent(),
                            tr("Confirm deletion"),
                            tr("Confirm deletion this appointment "),
                            QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Cancel)
    {UNLOOKREFRESH;
     return;
    }


 if (ptr ==0)                                                  { UNLOOKREFRESH;    return; }
 C_Frm_Rdv *pC_Frm_Rdv = (C_Frm_Rdv*)ptr;
 if (m_pCMoteurAgenda->RDV_Delete(pC_Frm_Rdv->m_PrimKey)==0)   { UNLOOKREFRESH;    return; };    // cassos si echec de  l'effacement dans la base

 //................... lire la liste des widget rendez-vous du jour .....................
 //                    et effacer celui \303\240 effacer
 C_Frm_Rdv *pC_Frm_Rdv_inDay = 0;
 for (int i = 0; i < C_Frm_RdvList::size(); ++i)
     { pC_Frm_Rdv_inDay = at(i);
       if (pC_Frm_Rdv_inDay == pC_Frm_Rdv)
          {removeAt(i);
           delete pC_Frm_Rdv;
           pC_Frm_Rdv = 0;
           break;
          }
     }
 generate_cacheRDV_List_FromDayWidget();    // l'effacement se fait en mode deplie donc widgets presents synchroniser le cache
 //........... si effacement reussi ..........
 //           mettre \303\240 jour le nombre et le titre
 if (pC_Frm_Rdv==0)
    { ReArangeIfDayHeightChange();                  // reajuster eventuellement la hauteur du jour et reorganiser les jours sous jacents et mettre a jour la hauteur du jour
    }
 UNLOOKREFRESH;
}

//---------------------------------------- rdvPropertyDialog--------------------------------------------
int C_Frm_Day::rdvPropertyDialog(C_RendezVous *rdvDst)
{LOOKREFRESH;
 C_Dlg_RdvTypeConfig *pC_Dlg_RdvTypeConfig = new C_Dlg_RdvTypeConfig(m_pColorProfils, m_pCMoteurAgenda, this, rdvDst);
 int ret = pC_Dlg_RdvTypeConfig->exec();
 delete pC_Dlg_RdvTypeConfig;
 UNLOOKREFRESH;
 return ret;
}

//---------------------------------------- Slot_RendezVousChangeClicked --------------------------------------------
void C_Frm_Day::Slot_RendezVousChangeClicked(C_Frm_Rdv *pC_Frm_Rdv)
{LOOKREFRESH;
 C_RendezVous rdvModified = *pC_Frm_Rdv->rdv_instance();
 C_Dlg_RdvTypeConfig *pC_Dlg_RdvTypeConfig = new C_Dlg_RdvTypeConfig(m_pColorProfils, m_pCMoteurAgenda, this, &rdvModified);
 if (pC_Dlg_RdvTypeConfig && pC_Dlg_RdvTypeConfig->exec() == QDialog::Accepted)
    {pC_Frm_Rdv->setWidgetOnRdv(rdvModified);                // on reajuste le widget sur les nouvelles donn\303\251es
     pC_Frm_Rdv->setWidgetStyleOnRdv(rdvModified);
     QTime startTime = getStartTime();
     int       y_deb = FIRST_DAY_POS_Y + m_BaseDayHeight + startTime.secsTo(rdvModified.getDateTime().time())/60*getResoPixByMinutes();
     pC_Frm_Rdv->move( W_RDV_OFSET, y_deb);
     pC_Frm_Rdv->RDV_Update();                    // enregistrer en base, regenere la liste du cache des RDV, update graphique
     readjustListWidgetPositions();               // revoir toutes les positions car le debut peut avoir change si place avant la limite sup
     ReArangeIfDayHeightChange();                 // reajuster eventuellement la hauteur du jour et reorganiser les jours sous jacents et mettre a jour la hauteur du jour
    }
 delete pC_Dlg_RdvTypeConfig;
 UNLOOKREFRESH;
}

//---------------------------------------- OnButtonNewRDVClickedPtr --------------------------------------------
void C_Frm_Day::OnButtonNewRDVClickedPtr(const char*, void*)
{newRDVAtThisDate(QDateTime(QDate::currentDate(), QTime::currentTime()), 15);
}

//---------------------------------------- OnButtonExpandClickedPtr --------------------------------------------
void C_Frm_Day::OnButtonExpandClickedPtr (const char*, void*)
{if (m_pCMoteurAgenda->GetAgendaMode_WeekOrDayOrMonth() == "MONTH")
    { C_Frm_Agenda *pC_Frm_Agenda     = static_cast<C_Frm_Agenda*>(parent());
      pC_Frm_Agenda->m_tmpDateToStart = getDate();
      pC_Frm_Agenda->Slot_ExpandWeek();
    }
 else
    {ExpandDialog();
     emit Sign_AgendaMustBeReArange();   // si un jour est deplie ou replie il faut  redeplacer tous les widgets sous jacents
    }
}

//---------------------------------------- OnButtonSaveClickedPtr --------------------------------------------
void C_Frm_Day::OnButtonSaveClickedPtr (const char*, void *)
{
 //................ effacer toutes les entr\303\251es de ce jour de la semaine ........................
 m_pCMoteurAgenda->MASK_DayDelete( m_Date.dayOfWeek(), m_SignUser);
 //.................. enregistrer tous les rendez-vous de ce jour ..............................
 //                   comme modele pour ce jour
 for (int i = 0; i < C_Frm_RdvList::size(); ++i)
      {m_pCMoteurAgenda->MASK_Append(*at(i));
      }
}

//---------------------------------------- getStartTime --------------------------------------------
QTime C_Frm_Day::getStartTime()
{
 QTime  tpsDeb;
 tpsDeb = QTime::fromString(m_HeureDeb+":00");
 int minute, heure;
 if (m_cacheRDV_List.size()) tpsDeb   = qMin(getFirstRdv(m_cacheRDV_List)->getDateTime().time(), tpsDeb);
 minute =  tpsDeb.minute(); minute/=15; minute*=15;   // arondir au 1/4 heure (affichage graduations)
 heure  =  tpsDeb.hour();
 tpsDeb = QTime(heure,minute,0,0);
 return tpsDeb;
}

//---------------------------------------- getStopTime --------------------------------------------
QTime C_Frm_Day::getStopTime()
{
 QDateTime tpsCur;
 QDateTime tpsMax = QDateTime(m_Date, QTime::fromString("23:59:59"));
 QDateTime tpsEnd = QDateTime(m_Date, QTime::fromString(m_HeureFin+":00"));
 C_RendezVous *pRdv = getLastRdv();
 if (pRdv)
    {tpsCur   = QDateTime(pRdv->m_date.addSecs(pRdv->m_Duree*60 ));
     tpsEnd   = qMin(tpsMax, qMax(tpsCur, tpsEnd));
    }
 return tpsEnd.time();
}

//---------------------------------------- getFirstRdv --------------------------------------------
//  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus pour creer la liste des widgets de rendez-vous
C_RendezVous *C_Frm_Day::getFirstRdv(const C_RendezVousList &rendezVousList)
{    C_RendezVous *pRdv    = 0;
     C_RendezVous *pRdvMin = 0;
     QDateTime   minTps;
     QDateTime   current;
     for (int i = 0; i < rendezVousList.size(); ++i)
         {pRdv       = rendezVousList[i];
          current    = pRdv->m_date;
          // au debut faut bien affecter une valeur a minTps pour commencer
          // soit c'est d'emble la  + petite
          // soit elle le deviendra lors du parcours de la boucle
          if (i==0) {minTps          = current;
                    }
          if (current <= minTps)
             {minTps          = current;
              pRdvMin         = pRdv;
             }
         }
     return pRdvMin;
}

//---------------------------------------- getLastRdv --------------------------------------------
//  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus pour creer la liste des widgets de rendez-vous
C_RendezVous *C_Frm_Day::getLastRdv()
{
     C_RendezVous *pRdv    = 0;
     C_RendezVous *pRdvMax = 0;
     QDateTime   maxTps = QDateTime(m_Date,QTime(00,00,01));
     QDateTime   current;
     for (int i = 0; i < m_cacheRDV_List.size(); ++i)
         {pRdv       = m_cacheRDV_List[i];
          current    = pRdv->m_date.addSecs(pRdv->m_Duree*60);
          if (current >= maxTps)
             {maxTps          = current;
              pRdvMax   = pRdv;
             }
         }
     return pRdvMax;
}

//---------------------------------------- RecreateRendezVousListWidget --------------------------------------------
//  NOTE : suppose que m_cacheRDV_List est a jour car se fonde dessus pour creer la liste des widgets de rendez-vous
int C_Frm_Day::RecreateRendezVousListWidget(C_RendezVousList &rendezVousList)
{
         if (m_DontRecreateWidget && m_pCMoteurAgenda->GetVerboseMode()) {qDebug()<< tr("recreate blocked : %1 ").arg(m_DontRecreateWidget); return 0;}
         if (m_pCMoteurAgenda->GetVerboseMode())                         {qDebug()<< tr("recreate not blocked : %1").arg(m_DontRecreateWidget) ;}

         LOOKREFRESH;

         //// SURBOOKING GUYNEMER DEB
         int          iSurBooking = 0;
         QDateTime    datePrem, dateSurBook;
         C_RendezVous *pC_RendezVous1;
         C_RendezVous *pC_RendezVous2;
         //// SURBOOKING GUYNEMER FIN

         int posY              = 0;
         QTime tpsDeb          = getStartTime();   // QTime::fromString(m_HeureDeb+":00");
         int     y_deb         = FIRST_DAY_POS_Y + m_BaseDayHeight;
         C_Frm_Rdv *pC_Frm_Rdv = 0;
         int nb                =  rendezVousList.size();
         //................... lire la liste des rendez-vous et creer les widget .....................
         clear();
         for ( int i = 0; i < nb; ++i )
             { //.......... creer et ajouter le widget du rendez vous .................................
               // 11-06-2013 SURBOOKING POUR GUYNEMER : SOLUTION PROVISOIRE DEB
               // si 2 RDV a la meme heure on passe la duree a 5mn et on decalle l'heure debut de 5mn
               pC_RendezVous1 = rendezVousList.at(i);
               datePrem = pC_RendezVous1->m_date;
               if (i < nb-1)
                  { pC_RendezVous2 = rendezVousList.at(i+1);
                    dateSurBook = pC_RendezVous2->m_date;
                  }
               else
                    dateSurBook = pC_RendezVous1->m_date.addSecs(1);
               if (pC_RendezVous1->m_date == dateSurBook || iSurBooking > 0)
                  { iSurBooking += 1;
//                  pC_RendezVous1->m_Note.append("<br><font color=red><b>SURBOOKING : RDV &agrave; " + pC_RendezVous1->m_date.toString("hh:mm") + " de " + QString::number(pC_RendezVous1->m_Duree) + "mn");
                    pC_RendezVous1->m_Note.append("SURBOOKING : RDV a " + pC_RendezVous1->m_date.toString("hh:mm") + " de " + QString::number(pC_RendezVous1->m_Duree) + "mn");

                    pC_RendezVous1->m_Duree = 5;
                    pC_RendezVous1->m_date = pC_RendezVous1->m_date.addSecs((iSurBooking -1) * 5 * 60); // decallage = num rdv * 5 mn * 60s
                  }
               if (datePrem != dateSurBook )iSurBooking = 0;
               // SURBOOKING GUYNEMER FIN

               pC_Frm_Rdv = append_C_Frm_Rdv(new C_Frm_Rdv( rendezVousList.at(i),
                                                            getResoPixByMinutes(),                    // resolution en pixels
                                                            getMagnetisme(),
                                                            m_pQRubberBand,
                                                            m_pCMoteurAgenda,
                                                            m_pColorProfils,
                                                            m_pBMC,
                                                            m_pCMoteurAgenda->GetMinDaysHeight(),
                                                            this
                                                          )
                                            );  //int x = W_OFSET, int y = 0, QWidget *parent = 0));
               //.............. on postionne le rendez vous \303\240 sa place .............................................................
               //               \303\240 la suite de l'autre + l'espace libre
               //               \303\240 ce niveau m_ExpandHeight pointe apres le dernier RDV place .......................................
               posY = tpsDeb.secsTo(pC_Frm_Rdv->m_date.time())/60*getResoPixByMinutes();
               pC_Frm_Rdv->move( W_RDV_OFSET, posY + y_deb);
               pC_Frm_Rdv->show();
             }
         int y_fin      = m_BaseDayHeight;
         if (pC_Frm_Rdv)
            { y_fin  =  y_deb + tpsDeb.secsTo(getStopTime())/60*pC_Frm_Rdv->getResoPixByMinutes();
            }
         UNLOOKREFRESH;
         return y_fin;
}
//---------------------------------------- readjustListWidgetPositions --------------------------------------------
void C_Frm_Day::readjustListWidgetPositions()
{   C_Frm_Rdv *pC_Frm_Rdv  = 0;
    QTime      tpsDeb      = getStartTime();   // QTime::fromString(m_HeureDeb+":00");
    int        y_deb       = FIRST_DAY_POS_Y + m_BaseDayHeight;
    int posY;
    for (int i = 0; i < C_Frm_RdvList::count(); ++i)
        {pC_Frm_Rdv = at(i);
         posY = tpsDeb.secsTo(pC_Frm_Rdv->m_date.time())/60*getResoPixByMinutes();
         pC_Frm_Rdv->move( W_RDV_OFSET, posY + y_deb);
        }
}

//---------------------------------------- MaskDayToAgendaDay --------------------------------------------
// ACTION : recree une liste de RDV \303\240 partir du masque de RDV prevu pour ce jour et reinitiaise la liste
//          des rdv du jour dessus
int C_Frm_Day::MaskDayToAgendaDay (C_RendezVousList &rendezVousList)
{
 //............. recuperer le template de rdv pour ce jour de la semaine ...................
 int nb = m_pCMoteurAgenda->MASK_GetDay_RDV_List(m_Date,  m_SignUser, rendezVousList);
 if (nb==0) { return 0;}
 //.................. enregistrer tous les rendez-vous modeles de ce jour de la semaine ..............................
 //                   comme rendez-vous pour cette date
 for (int i = 0; i < rendezVousList.size(); ++i)
     {m_pCMoteurAgenda->RDV_Create(*rendezVousList.at(i));
     }
 return nb;
}

//---------------------------------------- time_toPosY --------------------------------------------
int C_Frm_Day::time_toPosY(const QTime &time)
{int posY              = 0;
 QTime tpsDeb          = getStartTime();   // QTime::fromString(m_HeureDeb+":00");
 int     y_deb         = FIRST_DAY_POS_Y + m_BaseDayHeight;
 posY                  = tpsDeb.secsTo(time)/60*getResoPixByMinutes();
 return posY + y_deb;
}

//---------------------------- posY_toTime ------------------------------------------------
QTime   C_Frm_Day::posY_toTime(int posY)
{QTime tpsDeb          = getStartTime();
 if (posY == -1) { return tpsDeb;}
 int     y_deb         = FIRST_DAY_POS_Y + m_BaseDayHeight;
 int     mn            = ((posY - y_deb)/getResoPixByMinutes()); mn /=getMagnetisme(); mn*=getMagnetisme();  // un peu de granulometrie
 return tpsDeb.addSecs(mn*60);
}

//---------------------------- posY_toMinutes ------------------------------------------------
int   C_Frm_Day::posY_toMinutes(int posY)
{QTime tpsDeb          = getStartTime();
 int mnST              = tpsDeb.hour()*60 + tpsDeb.minute();
 if (posY == -1) { return mnST;}
 int     y_deb         = FIRST_DAY_POS_Y + m_BaseDayHeight;
 int     mn            = ((posY - y_deb)/getResoPixByMinutes()); mn /=getMagnetisme(); mn*=getMagnetisme();  // un peu de granulometrie
 return mn + mnST;
}

//--------------------------------------Chercher_les_RDV_dun_patient-----------------------------------------
void C_Frm_Day::Chercher_les_RDV_dun_patient (QString nom_prenom_guid /* ="" */)
{   C_Frm_Agenda* pC_Frm_Agenda = (C_Frm_Agenda*) this->parent();
    pC_Frm_Agenda->Chercher_les_RDV_dun_patient (nom_prenom_guid);
}// PRINTRDV deb
//--------------------------------------Imprimer_les_RDV_dun_medecin-----------------------------------------
void C_Frm_Day::Imprimer_les_RDV_dun_medecin(QDateTime date_rdv, QString code_user )
{   C_Frm_Agenda* pC_Frm_Agenda = (C_Frm_Agenda*) this->parent();
    pC_Frm_Agenda->Imprimer_les_RDV_dun_medecin(date_rdv, code_user );
}
// PRINTRDV deb

//--------------------------------------Imprimer_le_RDv_selectionne-----------------------------------------
void C_Frm_Day::Imprimer_ce_RDV( QString loginMed, QString primkey_rdv )
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
//    QString printerName =  G_pCApp->readUniqueParam( "Print", "Printer name");
//    if (printerName.length()) printer.setPrinterName(printerName);
//    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
//    if (printDialog->exec() == QDialog::Accepted)
//    {
    QPlainTextEdit *pQPlainTextEdit;
    pQPlainTextEdit = new QPlainTextEdit(this); //ui->plainTextEdit_view;
    pQPlainTextEdit->hide();
    if (m_pCMoteurAgenda->paginer_ce_RDV(loginMed, primkey_rdv, pQPlainTextEdit) > 0)
        {pQPlainTextEdit->print(&printer);}
        //printerName = printer.printerName();
        //G_pCApp->writeParam( "Print", "Printer name", printerName);
        //G_pCApp->updateIniParamToDisk();
//    }
    //delete pQWebView;
#endif // QT_NO_PRINTER
}
// PRINTRDV fin
//====================================================== C_Frm_Rdv ======================================================================
//------------------------------------------------------ pC_RendezVous ----------------------------------------------------------------------
C_Frm_Rdv::C_Frm_Rdv (  C_RendezVous *pC_RendezVous,       // data
                      //........... gui .................
                      int                  resoPix ,
                      int                  timeGranu ,
                      QRubberBand         *pQRubberBand,
                      CMoteurAgenda       *pCMoteurAgenda ,
                      MAP_COLOR           *colorProfils,
                      C_BitMapCollection  *pC_BitMapCollection,
                      int minHeight,
                      QWidget *parent)
         ://........... gui .................
          QFrame(parent)
{   m_PixByMinute         = resoPix;
    m_pBMC                = pC_BitMapCollection;
    m_pCMoteurAgenda      = pCMoteurAgenda;
    m_pColorProfils       = colorProfils;
    m_MinHeight           = minHeight;
    m_GrabIsOn            = 0;
    m_pQRubberBand        = 0;
    m_EditTimerActive     = 0;
    m_button_HeureDuree   = 0;
    m_textLabel_Nom       = 0;
    m_InfoEdit            = 0;
    m_ButtonDelete        = 0;
    m_Magnetisme          = timeGranu;
    m_Width               = pCMoteurAgenda->GetAgendaWidth();
    if (m_pCMoteurAgenda->m_WeekOrDay != "DAY")                                    // CZA
            m_Width = pCMoteurAgenda->GetAgendaWeekWidth() / pCMoteurAgenda->GetNbDayInWeek();               // CZA
  //...................... style du cadre g\303\251n\303\251ral .......................................
 /*
 if (pC_RendezVous->m_Type.length())
     setStyleSheet(tr("background-color: ")+(*m_pColorProfils)[pC_RendezVous->m_Type]+"; border: 1px solid #8f8f91; border-radius: 6px;");  // background-color: #960101 background-image: url(images/welcome.png
 else
     setStyleSheet("background-color: #FFFFFF; border: 1px solid #8f8f91; border-radius: 6px;");  // background-color: #960101 background-image: url(images/welcome.png
 */
 //m_pQRubberBand = new QRubberBand(QRubberBand::Rectangle, parent); // ne sera active que lors des l'agrandissement deplacement du widget
 QString colorStr   = C_RendezVous::getRdvColor(*pC_RendezVous, m_pColorProfils);
 m_background_color = QColor(colorStr);

 m_pQRubberBand     = pQRubberBand;
 setMouseTracking (TRUE );
 setAcceptDrops   (TRUE );
 //...................... placer les labels et le texte ................................
 int  widget_h       = 14-1;                  // hauteur m_MinHeight
 m_widget_w          = m_Width - W_RDV_OFSET;
 int               y = 1;

 // m_button_HeureDuree = new C_RdvHeureButton(this);
 if (m_button_HeureDuree)
    {m_button_HeureDuree->setMouseTracking (TRUE );
     m_button_HeureDuree->installEventFilter(this);
     m_button_HeureDuree->setFlat(true);
     m_button_HeureDuree->setGeometry( 6,  2, 82,  widget_h-2);
     connect( m_button_HeureDuree,  SIGNAL( clicked ()  ),                                  this ,     SLOT(   Slot_textLabel_HeureClicked()  )  );
    }
 // m_textLabel_Nom     = new QLabel(this);
 if (m_textLabel_Nom)
    {m_textLabel_Nom->setGeometry( 137,  2, m_widget_w-137-5, widget_h-2);
     m_textLabel_Nom->setMouseTracking (TRUE );
     m_textLabel_Nom->installEventFilter(this);
    }
 if (pCMoteurAgenda->GetEditNoteMode())
    {m_InfoEdit = new C_AgendaEdit(this);
     m_InfoEdit->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
     m_InfoEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
     m_InfoEdit->setStyleSheet(QString("background-color: %1; border: 1px none #8f8f91;padding 6px; border-radius: 6px; font-size: 9px;").arg(colorStr));
     connect( m_InfoEdit,           SIGNAL( textChanged ()),                                this ,     SLOT(   Slot_InfoEdittextChanged()  )  );
     connect( m_InfoEdit,           SIGNAL( cursorPositionChanged ()),                      this ,     SLOT(   Slot_InfoEdit_cursorPositionChanged()  )  );
     connect( m_InfoEdit,           SIGNAL( Sign_focusOutEvent(QFocusEvent *)),             this ,     SLOT(   Slot_InfoEditFocusOutEvent(QFocusEvent *)  )  );
    }
//.................. style des bontons ...................................
//QString style = "QPushButton { border: 1px solid #8f8f91; border-radius: 0px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"    // style normal
//                "QPushButton:pressed {                    border-radius: 0px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}";   // style selectionne
if ( pCMoteurAgenda->GetDeleteMode() )
   {m_ButtonDelete = new CMyButton(m_pBMC->m_RdvCancel_Pixmap, this, "", this);
    if (m_button_HeureDuree) m_ButtonDelete->setGeometry(90, y, widget_h, widget_h);   // icone = carre donc widget_hxwidget_h
    else                     m_ButtonDelete->setGeometry(m_widget_w-(widget_h*3)-2, y, widget_h, widget_h);   // icone = carre donc widget_hxwidget_h
    m_ButtonDelete->setFlat( TRUE );
    m_ButtonDelete->setToolTip ( "<font color=\"#000000\">"+tr("Delete this appointment")+"</font>" );
    m_ButtonDelete->setMouseTracking (TRUE );
    m_ButtonDelete->installEventFilter(this);
    connect( m_ButtonDelete,       SIGNAL( Sign_ButtonClickedPtr (const char*, void *)  ),          parent ,   SLOT(   Slot_ButtonRDVDeleteClicked(const char*, void *)  ));
   }


m_ButtonChange = new CMyButton(m_pBMC->m_ButtonChange_Pixmap, this, "", this);
if (m_button_HeureDuree) m_ButtonChange->setGeometry(105, y, widget_h, widget_h);
else                     m_ButtonChange->setGeometry(m_widget_w-(widget_h*2)-2, y, widget_h, widget_h);
m_ButtonChange->setFlat( TRUE );
m_ButtonChange->setToolTip ( "<font color=\"#000000\">"+tr("Modify the appointment.")+"</font>" );


m_ButtonAcceder = new CMyButton(m_pBMC->m_ButtonChange_Pixmap, this, "", this);
if (m_button_HeureDuree) m_ButtonAcceder->setGeometry(120, 0, widget_h, widget_h);
else                     m_ButtonAcceder->setGeometry(m_widget_w-widget_h-2, y, widget_h, widget_h);
m_ButtonAcceder->setFlat( TRUE );
m_ButtonAcceder->setToolTip ( "<font color=\"#000000\">"+tr("Access the patient's file.")+"</font>" );  //AgendaCreateDoss.png
//m_ButtonAcceder->setStyleSheet(style);  // background-color: #960101 background-image: url(images/welcome.png

//.......................... installer le filtre evenementiel ....................................
m_ButtonChange->setMouseTracking (TRUE );
m_ButtonAcceder->setMouseTracking (TRUE );
m_ButtonChange->installEventFilter(this);
m_ButtonAcceder->installEventFilter(this);

setWidgetOnRdv(*pC_RendezVous);

connect( m_ButtonAcceder,      SIGNAL( Sign_ButtonClickedPtr (const char*, void *) ),           parent ,   SLOT(   Slot_ButtonAccederClicked(const char*, void *) ));
//................................ on emet ce signal en direction du pere pour qu'il efface le rendez-vous ......................................
connect( this,                 SIGNAL( Sign_DeleteRendezVous (const char*, void *)  ),          parent ,   SLOT(   Slot_ButtonRDVDeleteClicked(const char*, void *)  ));
connect( this,                 SIGNAL( Sign_RendezVousChangeClicked(C_Frm_Rdv*)),               parent ,   SLOT(   Slot_RendezVousChangeClicked(C_Frm_Rdv*)));
connect( this,                 SIGNAL( Sign_StopTimer(int )),                                   parent ,   SLOT(   Slot_StopTimer(int)));
connect( this,                 SIGNAL( Sign_ReplaceRdvByDroppedRdv(C_Frm_Rdv *)),               parent ,   SLOT(   Slot_ReplaceRdvByDroppedRdv(C_Frm_Rdv *)));
connect( m_ButtonChange,       SIGNAL( Sign_ButtonClickedPtr (const char*, void *)  ), this ,     SLOT(   Slot_ButtonChangeClicked(const char*, void *)  ));

 /*
  if (CMoteurBase::IsThisDroitExist(G_pCApp->m_Droits, "agc")) {m_ButtonDelete->setEnabled(TRUE);   m_ButtonChange->setEnabled(TRUE);}
  else                                                         {m_ButtonDelete->setEnabled(FALSE);  m_ButtonChange->setEnabled(FALSE);}
 */
}
//------------------------ ~C_Frm_Rdv ---------------------------------------
C_Frm_Rdv::~C_Frm_Rdv()
{
}
//------------------------ paintEvent ---------------------------------------
void C_Frm_Rdv::paintEvent ( QPaintEvent * /*event*/ )
{   QPainter p(this);
    p.setPen (m_background_color.darker(150));
    //p.drawRoundedRect ( this->rect(), 6, 6);          // radius 0 ->rectangle
    QPainterPath path;
    path.addRoundedRect (rect(), 6, 6);
    p.setBrush(m_background_color);
    p.drawPath(path);
    //............ dessin de la poignee de deplacement a gauche ...............
    p.setPen (Qt::gray);
    //QRectF rect (0,10,4,h-23);
    p.drawRoundedRect (rect(), 6, 6);p.setPen (QColor("#000000"));
    p.fillRect ( 1, 4, 4, height()-8, m_background_color.lighter(150) );
    p.drawText (6,11, QString(m_Nom+" "+m_Prenom).replace("&NBSP;","&nbsp;").replace("&nbsp;"," ") );
    //QFrame::paintEvent(event);

}
//---------------------------------------- getMagnetisme --------------------------------------------
int C_Frm_Rdv::getMagnetisme()
{return m_Magnetisme;
}

//---------------------------------------- setWidgetStyleOnRdv --------------------------------------------
void C_Frm_Rdv::setWidgetStyleOnRdv(const C_RendezVous &rdv) // to do : detecter si la date n'est pas en dehors du jour actuel
{QString colorStr   = C_RendezVous::getRdvColor(rdv, m_pColorProfils);
 m_background_color = QColor(colorStr);
 if (m_InfoEdit)
    {m_InfoEdit->setStyleSheet(QString("background-color: %1; border: 1px none #8f8f91;padding 6px; border-radius: 6px; font-size: 9px;").arg(colorStr));
    }
}

//---------------------------------------- setWidgetOnRdv --------------------------------------------
void C_Frm_Rdv::setWidgetOnRdv(const C_RendezVous &rdv) // to do : detecter si la date n'est pas en dehors du jour actuel
{m_date     = rdv.m_date;
 m_Duree    = rdv.m_Duree;
 m_Nom      = rdv.m_Nom;
 m_Prenom   = rdv.m_Prenom;
 m_Tel      = rdv.m_Tel;
 m_Note     = rdv.m_Note;
 m_GUID     = rdv.m_GUID;
 m_PrisPar  = rdv.m_PrisPar;
 m_PrisAvec = rdv.m_PrisAvec;
 m_Type     = rdv.m_Type;
 m_PrimKey  = rdv.m_PrimKey;
 m_State    = rdv.m_State;
 m_Where    = rdv.m_Where;

 if (m_InfoEdit)          m_InfoEdit->setText(m_Note);
 if (m_button_HeureDuree) m_button_HeureDuree->setText(computeTextButton());
 if (m_textLabel_Nom)     m_textLabel_Nom->setText(m_Nom + " " + m_Prenom);
 //......................... les tool tip ..................................
 if ( m_GUID.length()!=0) m_ButtonAcceder->setIcon(m_pBMC->m_ButtonAcceder_Pixmap);
 else                     m_ButtonAcceder->setIcon(m_pBMC->m_ButtonCreateDoss);

 //....................... rechercher le statut .............................
 QString statutRdv = rdv.m_State;

 if (statutRdv.length()==0) statutRdv = tr("Undefined status");
 else
    {QMap<QString, QPixmap>::const_iterator it = m_pBMC->m_StatutsPixmap.find(statutRdv);
     if (it != m_pBMC->m_StatutsPixmap.end())    m_ButtonChange->setIcon(it.value());
    }
 resize ( QSize ( m_widget_w, getHeight())) ;
 placeInfoEdit(m_Duree*m_PixByMinute);
}

//---------------------------- Slot_InfoEditFocusOutEvent ------------------------------------------------
void C_Frm_Rdv::Slot_InfoEditFocusOutEvent(QFocusEvent *)
{if (m_InfoEdit)
    {if (m_Note != m_InfoEdit->text())
        {m_Note = m_InfoEdit->text();
         RDV_Update();
        }
    }
}

//---------------------------------------- Slot_InfoEdit_cursorPositionChanged -----------------------------------------
void C_Frm_Rdv::Slot_InfoEdit_cursorPositionChanged ()
{Slot_InfoEdittextChanged();
}
//---------------------------- Slot_InfoEdittextChanged ------------------------------------------------
void C_Frm_Rdv::Slot_InfoEdittextChanged()
{//if (m_InfoEdit) m_Note     = m_InfoEdit->text();
 if (m_EditTimerActive==0)
    {emit Sign_StopTimer(1);
     QTimer::singleShot(2000, this, SLOT(Slot_EditTimeOut()));
     m_EditTimerActive = 1;
    }
}
//---------------------------- Slot_EditTimeOut ------------------------------------------------
void C_Frm_Rdv::Slot_EditTimeOut()
{if (m_InfoEdit)
    {if (m_Note != m_InfoEdit->text())
        {m_Note = m_InfoEdit->text();
         RDV_Update();
        }
    }
 m_EditTimerActive = 0;
 emit Sign_StopTimer(0);
}

//---------------------------- placeInfoEdit ------------------------------------------------
void C_Frm_Rdv::placeInfoEdit (int h)
 {  if (m_InfoEdit==0) return;
    int ofsetY = 15;
    if (h>28)
       {m_InfoEdit->show();
       }
    else
       {m_InfoEdit->hide();
       }
    m_InfoEdit->setGeometry ( 6, ofsetY, width()-4, h-ofsetY-4 );
 }
//---------------------------- setGeometry ------------------------------------------------
void C_Frm_Rdv::setGeometry ( int x, int y, int w, int h )
 { QFrame::setGeometry(x,y,w,h);
    placeInfoEdit (h);
 }
//---------------------------- dragEnterEvent ------------------------------------------------
void C_Frm_Rdv::dragEnterEvent(QDragEnterEvent *event)
 {  if (event->mimeData()->hasFormat("text/medintux_rdv_drag"))
       {event->acceptProposedAction();
       }
 }
//---------------------------- dragMoveEvent ------------------------------------------------
void C_Frm_Rdv::dragMoveEvent(QDragMoveEvent * event)
 {  if (event->proposedAction() == Qt::CopyAction && event->mimeData()->hasFormat("text/medintux_rdv_drag"))
        {event->acceptProposedAction ();
         /*
         QString tm_str = "<TABLE cellSpacing=\"0\"  cellpadding=\"4\" width=100% border=\"1\">"
                                "<TBODY>"
                                "<TR>"
                                "<TD width=100% align=\"left\" bgcolor=\"#FEFFDD\">"+
                                tr("The")+QString("&nbsp;<b><font color=\"#0000FF\">%1</font></b>&nbsp;"+tr("at")+"&nbsp;<b><font color=\"#e80d0d\">%2</font> "+tr("Creation")+"</b>&nbsp;"+tr("for")+"&nbsp;<font color=\"#FF8000\"><b>%3</b></font>"
                                "</TD>"
                                "</TBODY>"
                                "</TABLE>").arg(m_date.toString("dd-MM-yyyy"),tm_str, m_Nom.replace(" ","&nbsp;") + "&nbsp;" + m_Prenom.replace(" ","&nbsp;")) ;
         QPoint     pos = mapFromGlobal ( QCursor::pos() );
         pos = QCursor::pos(); pos.setX(pos.x()+30);
         QToolTip::showText (pos,  tm_str, this, this->rect() );
         */
        }
 }
//---------------------------- dropEvent ------------------------------------------------
void C_Frm_Rdv::dropEvent(QDropEvent *event)
 {  if (event->proposedAction() == Qt::CopyAction && event->mimeData()->hasFormat("text/medintux_rdv_drag"))
        {const QMimeData *mimeData            = event->mimeData();
         ((C_Frm_Day*)parent())->m_DropedRdv  = C_RendezVous::unSerialize(QString(mimeData->data ( "text/medintux_rdv_drag" )));
         QTimer::singleShot(10, this, SLOT(Slot_Drop_Rdv ()));
         event->acceptProposedAction ();
        }
 }
//---------------------------- dragLeaveEvent ------------------------------------------------
void C_Frm_Rdv::dragLeaveEvent ( QDragLeaveEvent *  )
{   ((C_Frm_Day*)parent())->m_DropedRdv.m_GUID = "";   // effacer les donnees du rdv venant du drag and drop
}

//------------------------ Slot_Drop_Rdv -----------------------------------------------------
void C_Frm_Rdv::Slot_Drop_Rdv ()
{  emit Sign_ReplaceRdvByDroppedRdv(this); // recuperer de l'info dans un eventel OBJET connect\303\251
}

//---------------------------- eventFilter ------------------------------------------------
bool C_Frm_Rdv::eventFilter(QObject *obj, QEvent *event)
     {
      if ( (m_textLabel_Nom && obj     == m_textLabel_Nom )        ||
          //obj == m_textLabel_Type    ||
           (m_button_HeureDuree && obj == m_button_HeureDuree)     ||
          obj == m_ButtonChange      ||
           (m_ButtonDelete && obj      == m_ButtonDelete)          ||
          obj == m_ButtonAcceder
         )
         {
          if (event->type() == QEvent::MouseMove)
             {QMouseEvent  *mouseEvent = static_cast<QMouseEvent*>(event);
              QWidget        *pQWidget = static_cast<QWidget*> (obj);
              QMouseEvent  modifiedMouseEvent(QEvent::MouseMove,
                                              pQWidget->mapToParent( mouseEvent->pos() ),
                                              mouseEvent->globalPos() ,
                                              mouseEvent->button(),
                                              mouseEvent->buttons(),
                                              Qt::NoModifier
                                              );
              mouseMoveEvent (&modifiedMouseEvent , pQWidget);
              return true;
             }
          else
             {return false;
             }
         }
      else
         {// pass the event on to the parent class
           return QFrame::eventFilter(obj, event);
         }
    }
//---------------------------- getRdvColor ------------------------------------------------
QString C_Frm_Rdv::getRdvColor()
{return C_RendezVous::getRdvColor(*this, m_pColorProfils);
}

//---------------------------- getQWhatsThisString ------------------------------------------------
QString C_Frm_Rdv::getQWhatsThisString()
{return C_RendezVous::getQWhatsThisString(Theme::getPath(TRUE), C_RendezVous::getRdvColor(*this, m_pColorProfils));
}

//---------------------------- Slot_textLabel_HeureClicked ------------------------------------------------
void C_Frm_Rdv::Slot_textLabel_HeureClicked()
{QWhatsThis::showText (QCursor::pos(), getQWhatsThisString());
}

//---------------------------- Slot_ButtonChangeClicked ------------------------------------------------
void C_Frm_Rdv::Slot_ButtonChangeClicked(const char*, void *ptr)
{if (ptr==0) return;
 emit Sign_RendezVousChangeClicked(this);
}

//---------------------------- computeTextButton ------------------------------------------------
QString C_Frm_Rdv::computeTextButton()
{QString str  = m_date.time().toString("hh:mm")+"/";
 if (m_Duree>60)
     str += QTime(0,0,0).addSecs(m_Duree*60).toString("hh:mm").replace(':','h');
 else
     str += QString::number(m_Duree)+" mn";
 return str;
}

//---------------------------- posY_toTime ------------------------------------------------
QTime   C_Frm_Rdv::posY_toTime(int posY)
{return ((C_Frm_Day*)parent())->posY_toTime(posY);
}
//---------------------------- time_toPosY ------------------------------------------------
int   C_Frm_Rdv::time_toPosY(const QTime &tm)
{return ((C_Frm_Day*)parent())->time_toPosY(tm);
}
//---------------------------- mouseMoveEvent ------------------------------------------------
void    C_Frm_Rdv::mouseMoveEvent ( QMouseEvent * event )
{mouseMoveEvent ( event , 0 );
}
//---------------------------- adjustToMagnetisme ------------------------------------------------
int    C_Frm_Rdv::adjustToMagnetisme(int value)
{return ((C_Frm_Day*)parent())->adjustToMagnetisme(value);
}

//---------------------------- mouseMoveEvent ------------------------------------------------
void    C_Frm_Rdv::mouseMoveEvent ( QMouseEvent * event , QWidget *pQWidget )
{int y         = event->pos().y();
 //int y         = adjustToMagnetisme(event->pos().y());
 if (m_GrabIsOn)
    {int deltaY    =  y - m_startPosY;
     if (qAbs( deltaY) )
        {QRect rect    = geometry ();
         if (m_GrabIsOn==TOP_START)
            {int posY = rect.y()+deltaY;
             m_pQRubberBand->setGeometry (rect.x(),  posY, width(), qMax(height() - deltaY, 3));
             m_date       = QDateTime(m_date.date(), posY_toTime(posY));
             m_Duree      = qMax((height() - deltaY)/getResoPixByMinutes(), getMagnetisme());
             if (m_button_HeureDuree) m_button_HeureDuree->setText(computeTextButton());
             rect = m_pQRubberBand->geometry ();
             setGeometry(rect.x(),posY,rect.width(),rect.height());
            }
         else if ( m_GrabIsOn==BOTTOM_START)
            {m_pQRubberBand->setGeometry (rect.x(), rect.y(), width(), qMax(height() + deltaY, 3));
             m_Duree      = qMax((height() + deltaY)/getResoPixByMinutes(), getMagnetisme());
             if (m_button_HeureDuree) m_button_HeureDuree->setText(computeTextButton());
             //............ marche pas ici (boucle reentrante)................
             //rect = m_pQRubberBand->geometry ();
             //setGeometry(rect.x(),rect.y(),rect.width(),rect.height());
            }
         else if ( m_GrabIsOn==MIDLE_START)
            {int posY = rect.y() + deltaY;
             posY     = adjustToMagnetisme(posY);
             m_pQRubberBand->setGeometry (rect.x(),  posY, width(), height());
             m_date       = QDateTime(m_date.date(), posY_toTime(posY));
             if (m_button_HeureDuree) m_button_HeureDuree->setText(computeTextButton());
             rect = m_pQRubberBand->geometry ();
             setGeometry(rect.x(),rect.y(),rect.width(),rect.height());
            }
        }
    }
  else
    { if (pQWidget)    // si on est au dessus d'un widget fils
         { setCursor(Qt::ArrowCursor);
         }
      else
         {if (y<=SIZE_BORDER_DAY || y>=height()-SIZE_BORDER_DAY)
             { setCursor(Qt::SizeVerCursor);
             }
          else if (event->x()<10)
             { setCursor(Qt::OpenHandCursor);
             }
          else
             {setCursor(Qt::ArrowCursor);
             }
         }
    }
  QFrame::mouseMoveEvent ( event );
}

//---------------------------- mouseReleaseEvent ------------------------------------------------
void C_Frm_Rdv::mouseReleaseEvent ( QMouseEvent * /*event*/ )
{int isPositionModified = 0;   // petite sophistication : ne poser la question que si changement de coordonnees sinon cassos
 if (m_GrabIsOn)
    {QRect rect          = m_pQRubberBand->geometry ();
     if (m_GrabIsOn==TOP_START)    // chichiterie indispensable pour reajuster la date en gardant le bas fixe
        {QTime tm = posY_toTime(rect.y() + rect.height());
         int   mn = tm.hour()*60+tm.minute()-m_Duree;
         int   hr = mn/60; mn = mn - hr*60;
         m_date   = QDateTime(m_date.date(),QTime(hr,mn));
        }
     isPositionModified  = m_PosYSav - y() + m_HeightSav- rect.height();
    }
 m_pQRubberBand->hide();
 releaseMouse();           // degraber la souris
 setCursor(Qt::SizeVerCursor);
 if (!m_pCMoteurAgenda->isModifToConfirm()&1) RDV_Update();
 if (isPositionModified)
    {
     if (m_pCMoteurAgenda->isModifToConfirm()&1 &&
         QMessageBox::question ((QWidget*)parent(),
                                tr("Confirm modification"),
                                tr("The features of this appointment have been modified \nConfirm modifications ?"),
                                QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Cancel)
        {m_date                      = m_C_RendezVousSav.m_date; // restituer la config initiale
         m_Duree                     = m_C_RendezVousSav.m_Duree;
         if (m_button_HeureDuree) m_button_HeureDuree->setText(computeTextButton());
         this->setGeometry(m_PosXSav,m_PosYSav,m_WidthSav,m_HeightSav);
        }
    }
 adjustWidgetToMagnetisme();                             // recadrer le widget aux contraintes du magnetisme
 RDV_Update();                                           // enregistrer en base
 ((C_Frm_Day*)parent())->readjustListWidgetPositions();  // revoir toutes les positions car le debut peut avoir change si place avant la limite sup
 ((C_Frm_Day*)parent())->ReArangeIfDayHeightChange();    // reajuster eventuellement la hauteur du jour et reorganiser les jours sous jacents et mettre a jour la hauteur du jour
 emit Sign_StopTimer(0);                                 // debloquer le timer de mise a jour
 m_GrabIsOn  = 0;
}

//---------------------------- adjustWidgetToMagnetisme ------------------------------------------------
void C_Frm_Rdv::adjustWidgetToMagnetisme(C_Frm_Rdv::adjustMode mode  /* = C_Frm_Rdv::TimeAndGraphic */)
{int mag           = getMagnetisme();
 int midleMag      = mag/2;
 m_Duree           = (m_Duree+midleMag)/mag; m_Duree = qMax((int)m_Duree * mag, qMax(m_MinHeight/getResoPixByMinutes(),mag));
 int   mn          = m_date.time().hour()*60+m_date.time().minute();    mn = (mn+midleMag) / mag; mn *= mag;
 int   hr          = mn / 60;
       mn          = mn - hr*60;
 QTime tm          = QTime(hr,mn);
 m_date            = QDateTime(m_date.date(), tm);
 if (mode==C_Frm_Rdv::JustTime) return;
 if (m_button_HeureDuree)    m_button_HeureDuree->setText(computeTextButton());
 setGeometry(0,0,m_widget_w, m_Duree*getResoPixByMinutes());
 move(W_RDV_OFSET, time_toPosY(tm));
}

//---------------------------- mousePressEvent ------------------------------------------------
void C_Frm_Rdv::mousePressEvent(QMouseEvent *event)
 {  if (event->button() == Qt::LeftButton)
        {if (m_GrabIsOn==0)
            {
             ((C_Frm_Day*)parent())->Slot_StopTimer(1);   // le timer sera debloque lors du release button
             //.............. ajuster le rectangle de depart au magnetisme .................
             //               au cas ou le rendez vous ne serait pas aligne
             m_C_RendezVousSav = *this;            // sauver la config initiale
             m_PosXSav         = x();
             m_PosYSav         = y();
             m_WidthSav        = width();
             m_HeightSav       = height();

             QRect rect        = geometry ();
             int top           = rect.y();          //adjustToMagnetisme(rect.y());
             int htr           = rect.height();     //rect.height()/getMagnetisme(); htr     = qMax(htr * getMagnetisme(), getMagnetisme()) ;
             int y             = event->pos().y();  //adjustToMagnetisme(event->pos().y());
             m_startPosY       = y;
             bool moveToNewPos = false;
             //..................... on demarre le rectangle elastique .....................
             if (m_startPosY<=SIZE_BORDER_DAY)
                { m_GrabIsOn = TOP_START;
                  m_pQRubberBand->setGeometry (rect.x(), top, width(), htr);
                  //m_pQRubberBand->show(); inutile car rajustement de la taille du widget cible se fait en temps reel
                  grabMouse (QCursor(Qt::SizeVerCursor));
                  moveToNewPos = true;
                }
             else if ( m_startPosY>=height()-SIZE_BORDER_DAY)
                { m_GrabIsOn = BOTTOM_START;
                  m_pQRubberBand->setGeometry (rect.x(), top, width(), htr);
                  m_pQRubberBand->show();   // oblige car le reajustemnt en temps reel ne fonctionne pas (boucle)
                  grabMouse (QCursor(Qt::SizeVerCursor));
                  m_pQRubberBand->show();
                  moveToNewPos = true;
                }
             else if (event->pos().x()<10)
                { m_GrabIsOn = MIDLE_START;
                  m_pQRubberBand->setGeometry (rect.x(), top, width(), htr);
                  //m_pQRubberBand->show(); //m_pQRubberBand->show(); inutile car rajustement de la taille du widget cible se fait en temps reel
                  setCursor(Qt::ClosedHandCursor);
                  moveToNewPos = true;
                }
             if (moveToNewPos)
                { rect = m_pQRubberBand->geometry ();
                  setGeometry(rect.x(),rect.y(),rect.width(),rect.height());
                }
             }
         else
             {/*
              QDrag *drag         = new QDrag(this);
              QMimeData *mimeData = new QMimeData;
              QRect rect(-4,0,width(),height());

              QPixmap pix = QPixmap::grabWidget (this, rect );
              mimeData->setText("Drag day");
              mimeData->setImageData(pix);
              drag->setMimeData(mimeData);
              drag->setPixmap(pix);
              Qt::DropAction dropAction = drag->exec();
              */
             }
         }
    else if ( event->button() == Qt::RightButton  && m_GrabIsOn==0)
         { //................ menu ....................
           ((C_Frm_Day*)parent())->Slot_StopTimer(1);   // le timer ser debloque lors du release button
           QString       ret =  ((C_Frm_Day*)parent())->doRdvMenu(this->rdv_instance(),1);
           // "Copy" "Replace" "Cut" "Type :" "Status :"  "Modify"  "Open" "Create" "Anomymize" "Quit"
           if (ret.length())
              {
                if (ret.indexOf("Modify") != -1)
                   {emit Sign_RendezVousChangeClicked(this);
                   }
               else if (ret.indexOf("Status :") != -1)
                  { QString statut = ret.remove("Status :");
                    m_State = statut.trimmed();
                    setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                    setWidgetStyleOnRdv(*this);
                    RDV_Update();
                  }
               else if (ret.indexOf("Type :") != -1)
                  { QString type = ret.remove("Type :");
                    m_Type = type.trimmed();
                    setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                    setWidgetStyleOnRdv(*this);
                    RDV_Update();
                  }
               else if (ret.indexOf("Cut") != -1)  // il ne  peut pas se detruire lui meme et sortir ensuite de sa fonction donc QTIMER
                  {QTimer::singleShot(10, this, SLOT(Slot_cut()) );
                  }
               else if (ret.indexOf("Copy") != -1)
                    Slot_copy();
               else if (ret.indexOf("Replace") != -1)
                    replaceWithCopy();
               else if (ret.indexOf("Modify") != -1)
                  {emit Sign_RendezVousChangeClicked(this);
                  }
               else if (ret.indexOf("Anomymize") != -1)
                  {m_Nom="";
                   m_Prenom="";
                   m_GUID="";
                   m_Tel="";
                   m_Note="";
                   m_Where="";
                   setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                   setWidgetStyleOnRdv(*this);
                   RDV_Update();
                  }
               else if (ret.indexOf("Delete") != -1)
                  {QTimer::singleShot(10, this, SLOT(Slot_delete()) );
                  }
               else if (ret.indexOf("Open") != -1)
                  {((C_Frm_Day*)parent())->Slot_ButtonAccederClicked(0, this);
                  }
               else if (ret.indexOf("Create") != -1)
                  {((C_Frm_Day*)parent())->Slot_ButtonAccederClicked(0, this);
                  }
               else if (ret.indexOf("Find Appointments") != -1)
                  {((C_Frm_Day*)parent())->Chercher_les_RDV_dun_patient();
                  }
               else if (ret.startsWith("AppointmentsListFor"))
                  {((C_Frm_Day*)parent())->Chercher_les_RDV_dun_patient(ret.mid(19));
                  }
               // CZF deb
               else if (ret.startsWith("PrintAppointments"))
                  {QDateTime Dtrdv = QDateTime::fromString(ret.mid(17,10) + " 00:00:00","yyyy-MM-dd hh:mm:ss");
                   ((C_Frm_Day*)parent())->Imprimer_les_RDV_dun_medecin(Dtrdv, ret.mid(28));
                  }
               // CZF fin
               //PRINTRDV deb
               else if (ret.startsWith("PrintThisAppointment"))
                  {
                   QString nomMed = ret.mid(ret.indexOf(";")+1);
                   QString keyRDV = ret.mid(20,ret.indexOf(";") -20);
                   ((C_Frm_Day*)parent())->Imprimer_ce_RDV(nomMed,keyRDV);
                  }
               //PRINTRDV fin
              }
         event->accept();
         ((C_Frm_Day*)parent())->Slot_StopTimer(0);
        }
 }

//---------------------------------------- RDV_Update --------------------------------------------
void C_Frm_Rdv::RDV_Update()
{ //............ enregistrer ...............
 QString errMess;
 if ( ! m_pCMoteurAgenda->RDV_Update(*this, &errMess) )
    {qDebug()<< errMess;
    }
 ((C_Frm_Day*)parent())->generate_cacheRDV_List_FromDayWidget();  // la on peut on est forcement en position ouverte (widgets presents)
 this->update();
}
//---------------------------------------- isCopyExist --------------------------------------------
bool C_Frm_Rdv::isCopyExist()
{ return QApplication::clipboard()->mimeData()->hasFormat("text/medintux_rdv");
}
//---------------------------------------- getCopy --------------------------------------------
C_RendezVous C_Frm_Rdv::getCopy()
{   C_RendezVous rdv;
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData   = clipboard->mimeData();
    if (mimeData->hasFormat ("text/medintux_rdv" ))
       {rdv = C_RendezVous::unSerialize(QString(mimeData->data ( "text/medintux_rdv" )));
       }
    return rdv;
}
//---------------------------------------- Slot_delete --------------------------------------------
void C_Frm_Rdv::Slot_delete()
{emit Sign_DeleteRendezVous ("d\303\251gage tu pues", (void*) this);  // ca va au pere (le jour) qui le tue
}

//---------------------------------------- Slot_copy --------------------------------------------
void C_Frm_Rdv::Slot_copy()
{QByteArray data(serialize(this).toLatin1());
 QMimeData *mimeData = new QMimeData;
 mimeData->setData("text/medintux_rdv", data);
 QClipboard *clipboard = QApplication::clipboard();
 clipboard->setMimeData ( mimeData );    // a ne pas deleter
}

//---------------------------------------- serialize --------------------------------------------
QString C_Frm_Rdv::serialize(C_Frm_Rdv *pC_Frm_Rdv)
{return C_RendezVous::serialize(pC_Frm_Rdv);
}

//---------------------------------------- unSerialize --------------------------------------------
C_RendezVous C_Frm_Rdv::unSerialize(const QString & data)
{return C_RendezVous::unSerialize(data);
}
//---------------------------------------- replaceWithCopy --------------------------------------------
void C_Frm_Rdv::replaceWithCopy()
{
 if ( !isCopyExist()) return;
 C_RendezVous    rdv = getCopy();    // on recupere le rendez-vous
 rdv.m_date          = m_date;       // on garde la date d'origine
 rdv.m_PrimKey       = m_PrimKey;    // on garde la primary key car on est sur le meme rendez-vous
 rdv.m_PrisPar       = m_PrisPar;    // on garde l'utilisateur qui a pris le RDV
 rdv.m_PrisAvec      = m_PrisAvec;   // on garde l'utilisateur en cours
 rdv.m_State         = m_State;

 setWidgetOnRdv(rdv);                // on reajuste le widget sur les nouvelles donn\303\251es
 setWidgetStyleOnRdv(rdv);
 RDV_Update();
}

//---------------------------------------- Slot_cut --------------------------------------------
void  C_Frm_Rdv::Slot_cut()
{Slot_copy();
 emit Sign_DeleteRendezVous ("d\303\251gage tu pues", (void*) this);  // ca va au pere (le jour) qui le tue
}

//---------------------------- GetHeight ------------------------------------------------
int C_Frm_Rdv::getHeight()
{int height  = m_Duree * m_PixByMinute; // height = qMax( m_Duree * m_PixByMinute,  (long)m_MinHeight);
 return qMax(height, m_MinHeight);   // il faut un minimum de visibilite
}


/*
//====================================== C_LabelDrag =======================================================
bool C_LabelDrag::event ( QEvent * e )
{ //QString txt = this->text();
  QRect rect = this->rect();
  if (e->type()==QEvent::Move)
     {//qDebug() << QString::number(e->type());
      //QToolTip::showText (QCursor::pos(), toolTip () , this);
      //QToolTip::showText (QCursor::pos(),  this->text(), this, rect );
      //setUpdatesEnabled(FALSE);
      //QTimer::singleShot(3,this,SLOT (repaint ()));
     }
  if (e->type()==QEvent::Paint)
     {
        paintEvent ( (QPaintEvent *) e);
     }
  return false;
}

// juste pour ajuster le widget a la largeur du texte
//---------------------------- paintEvent ------------------------------------------------
// juste pour ajuster le widget a la largeur du texte
void C_LabelDrag::paintEvent ( QPaintEvent * event )
    {QString txt = this->text();
     QPainter p(this);
     QRect rect = p.boundingRect (this->rect(), Qt::TextSingleLine, text());
     rect.setHeight(height());
     //if (text().length()>60)
     //if (0)
     //{
         rect.setWidth((int)((float)rect.width()*0.3));
         resize( rect.width(),height());
         p.drawText(3,rect.height(),this->text());
         qDebug() << txt;
     //}
     //QLabel::paintEvent(event);
    }
*/
