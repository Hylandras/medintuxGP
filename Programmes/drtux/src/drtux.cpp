 /********************************* drtux.cpp **************************************
 *                                                                                *
 *   #include "drtux.h"                                                           *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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

//====================================== INCLUDES ========================================================================
// il est indispensable de rajouter au fichier:  src.pro  les deux lignes suivantes pour que <qwidgetfactory.h> fonctionne
//                           unix:LIBS                += -lqui -L$$QT_BUILD_TREE/lib
//                           win32:LIBS                += $$QT_BUILD_TREE/lib/qui.lib
//#include <qwidgetfactory.h>

#include "drtux.h"

#ifdef Q_WS_WIN
   #include <windows.h>
#endif

#include <stdlib.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qmenubar.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qtabwidget.h>
#include <qworkspace.h>
#include <qtimer.h>
#include <qcombobox.h>
#include <qfontdatabase.h>
#include <qlineedit.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qdir.h>
#include <qsimplerichtext.h>
#include <qsplitter.h>
#include <qprocess.h>
#include <qsize.h>
#include <qclipboard.h>
#include <qeventloop.h>
#include <qthread.h>
#include <qurl.h>
#include <qtoolbox.h>
#include <qtextcodec.h>

#include "CApp.h"

#include "ui/Dlg_MedicaTux.h"
#include "ui/DlgPrint.h"
#include "ui/Dlg_PermsUserChoice.h"

#include "C_TokenInterpret.h"

#include "C_Dlg_GestionATCD.h"
#include "C_Dlg_ChoixUser.h"
#include "CDevilCrucible.h"
#include "CMDI_Observation.h"
#include "CMDI_Prescription.h"
#include "CMDI_Terrain.h"
#include "CMDI_ChoixPatient.h"
#include "CMDI_Ident.h"

#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/GestRPU.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/C_Organiseur.h"
#include "../../drtux/src/C_DlgListFieldMngr.h"



#define W_DOCK_LEFT 200

class C_PatientRootItem;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== DrTux ===============================================================================

//------------------------------ ~DrTux -----------------------
/*! \brief Destructeur de la classe. */
DrTux::~DrTux()
{   if (m_IsModifiable) G_pCApp->m_pCMoteurBase->DeVerrouilleDossier (G_pCApp->m_NumGUID);
    m_RubList.clear();  // effacer toutes les données reservées
    m_EvnList.clear();                                      // effacer la liste

    if (m_pTimerVerrou) delete m_pTimerVerrou;
    if (m_DataBaseCIM10)
       {if ( m_DataBaseCIM10->isOpen() ) m_DataBaseCIM10->close();
        QSqlDatabase::removeDatabase( m_DataBaseCIM10);
       }
    G_pCApp->m_pDrTux = 0;
}

//---------------------------------- DrTux ------------------------------------
/*! \brief Constructeur de la classe.
 * Initialisation des variables, création de la fenêtre principale avec les docks et tout le reste selon paramètres utilisateurs.
*/
DrTux::DrTux()
    : QMainWindow( 0, PROG_NAME, WDestructiveClose )
{   QString val1           = "";
    QString val2           = "";
    QString val3           = "";
    QString val4           = "";
    m_Apropos_Proc         = 0;
    m_Lap_Proc             = 0;
    m_CCAM_Run             = FALSE;
    m_PluginRun            = "";
    m_pQWorkSpaceRub       = 0;
    m_IsModifiable         = 0;
    m_pTimerVerrou         = 0;
    m_DataBaseCIM10        = 0;
    m_pCMDI_Ident          = 0;
    m_pCMDI_Prescription   = 0;
    m_pCMDI_Observation    = 0;
    m_pCMDI_ChoixPatient   = 0;
    m_pCMDI_Terrain        = 0;
    m_pActionDossierSave   = 0;
    m_pActionAlignRight    = 0;
    m_pActionTextColor     = 0;
    m_pActionFastTextColor = 0;
    m_pActionListPatDelete = 0;
    m_pActionTextItalic    = 0;
    m_pActionAlignCenter   = 0;
    m_pActionAlignJustify  = 0;
    m_pActionAlignLeft     = 0;
    m_pActionVidal         = 0;
    m_pActionModifieListe  = 0;
    m_pActionTextUnderline = 0;
    m_pActionCIM10         = 0;
    m_pActionTextBold      = 0;
    m_pActionLabo          = 0;
    m_pActionCreateListe   = 0;
    m_pActionCCAM          = 0;
    m_pMenuTerrain         = 0;
    m_Custom_Menu_ID       = 1;
    m_pFormOngletRubrique  = 0;
    m_VidalRun             = FALSE;
    m_Custom_Menu          = 0;
    m_Custom_ToolBar       = 0;
    m_pToolBarRubMenu      = 0;
    m_idToolMenu           = -1;
    //................................ les docks .......................
    m_pDock_Menu           = 0;
    m_pDockGlossaire       = 0;
    m_pDock_TabRubrique    = 0;
    m_pForm_Menu           = 0;
    m_pFormGlossaire       = 0;
    m_pForm_TabRubrique    = 0;
    m_pDock_Organiseur     = 0;
    m_pC_Organiseur        = 0;

    SetTitle();
    //....................... verrouiller lire et initialiser la liste des rubriques ..........................

    QString usedBy;
    QString value;
    m_IsModifiable = G_pCApp->m_pCMoteurBase->VerrouilleDossier (G_pCApp->m_NumGUID, G_pCApp->m_pCMoteurBase->m_UserName+"@"+ G_pCApp->m_pCMoteurBase->m_HostName , usedBy);
    CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Gestion des dossiers", "Acces Concurrent", &value);   // path editeur de texte non d�fini
    if (m_IsModifiable==0)
       {if ( value.lower() == tr("non possible") )
           {QMessageBox::information( this, tr(  "OUVERTURE CONFLICTUELLE"),
                                 tr(  " <b><b><u>ATTENTION</b></u> ! Ce dossier <font color=\"#e80d0d\"><b>")          + G_pCApp->m_DossNom     + " " + G_pCApp->m_DossPrenom  +
                                 tr(  "</b></font> est actuellement ouvert sur le poste suivant : <b>" )               + usedBy                 +
                                 tr(  "</b><br> Son accès ne sera possible sur votre poste, qu'après fermeture sur le poste où il est actuellement ouvert") ,
                                 tr("Annu&ler"),0 , 0,
                                 1, 1 );
            return;
           }
        else if ( value.lower() == value.lower() == tr("lecture"))
           {QMessageBox::information( this, tr(  "OUVERTURE CONFLICTUELLE"),
                                 tr(  " <b><b><u>ATTENTION</b></u> ! Ce dossier <font color=\"#e80d0d\"><b>")          + G_pCApp->m_DossNom     + " " + G_pCApp->m_DossPrenom  +
                                 tr(  "</b></font> est actuellement ouvert sur le poste suivant : <b>" )               + usedBy                 +
                                 tr(  "</b><br> Son accès n'est possible <b><u>qu'en lecture seulement</u></b>,<br>"
                                      "il se mettra automatiquement en mode écriture<br>"
                                      "lorsqu'il sera disponible sur votre poste,<br><b><font color=\"#e80d0d\"><u>ce qui impose sa fermeture sur le poste<br>où il est actuellement utilisé.</u></font></b>") ,
                                 tr("Accès en lecture seule"),0 , 0,
                                 1, 1 );
           }
        else
           { if (AlertVerrou(usedBy)==1) return;
           }
       }
    G_pCApp->m_pCMoteurBase->initRubriquesList( &m_RubList,   G_pCApp->m_NumGUID);
    G_pCApp->m_pCMoteurBase->Evnmt_InitList(    &m_EvnList,   G_pCApp->m_ID_Doss);

    //___________________________________________________________________________________________________
    //....................... Recuperer parametres DE LA BASE DE DONNEE................................................
    //                            correspondant a l'user en cours
    // G_pCApp->m_pCMoteurBase->Param_GetParam( &USER_PARAM , G_pCApp->m_User); // deplace dans CApp
    //___________________________________________________________________________________________________
#ifdef Q_WS_X11
    //.................... ajuster resolution d'impression .........................................
    if (READ_USER_PARAM(USER_PARAM, "Impression", "Resolution", &val1, &val2)==0)  // zero = pas d'erreur
       {QPaintDevice::x11SetAppDpiX(atoi(val1));
        QPaintDevice::x11SetAppDpiY(atoi(val2));
       }
#endif
    //................. recuperer la taille des polices des menus ...................................
    //                 surcharge celui de Theme.ini et Manager.ini
    if (READ_USER_PARAM(USER_PARAM, "Theme config", "PopupFntSize", &val1)==0)  // zero = pas d'erreur
       {int size = val1.toInt();
        if (size !=-1)
           {Theme::setFontSize_Menu( size );
            QFont fnt (menuBar()->font());
            fnt.setPointSize( size ); menuBar()->setFont(fnt);
           }
       }
    //....................... tool bar .............................................................
    setupFileActions();
    if (m_pActionListPatDelete) m_pActionListPatDelete->setEnabled(m_IsModifiable);
    if (m_pActionDossierSave)   m_pActionDossierSave->setEnabled(m_IsModifiable);

    //...........................barre et menu texte..................................................................
    setupTextActions();
    setupEditActions();
    setupToolsActions();
    setupToolsTerrain();
    //.......................... barre de menus fenetres ........................................................
    m_fenetres = new ThemePopup( this );
    menuBar()->insertItem( tr("&Fenêtres"), m_fenetres );

    m_pToolBarRubMenu = new QToolBar( this );
    m_pToolBarRubMenu->setLabel( tr("Liste des rubriques") );
    m_pActionRubMenu = new QAction( tr( "Liste des rubriques" ),    Theme::getIcon("16x16/ThereIsMenuHere.png"), tr( "Liste des rubriques" ), Key_F9,this,  "Liste des rubriques" );
    connect( m_pActionRubMenu, SIGNAL( activated() ), this, SLOT( Slot_MenuRubriques() ) );
    m_pActionRubMenu->addTo( m_pToolBarRubMenu );
    m_pActionRubMenu->addTo( m_fenetres );

    m_pActionActiverDockVigie = new QAction( "?", 0, this, "ActionActiverVigie" );
    connect( m_pActionActiverDockVigie, SIGNAL( activated() ), this, SLOT( Slot_SetDockMenu_Organiseur() ) );
    setStateActivationDockMenu(m_pActionActiverDockVigie, "Vigie",tr("La Vigie"));

    m_pActionActiverDockButts = new QAction( "?", 0, this, "ActionMenuRubriques" );
    connect( m_pActionActiverDockButts, SIGNAL( activated() ), this, SLOT( Slot_SetDockMenu_RubriquesButton() ) );
    setStateActivationDockMenu(m_pActionActiverDockButts, "Rubriques",tr("Les boutons d'activation des rubriques"));

    m_pActionActiverDockGloss = new QAction( "?", 0, this, "ActionActiverGlossaire" );
    connect( m_pActionActiverDockGloss, SIGNAL( activated() ), this, SLOT( Slot_SetDockMenu_Glossaire() ) );
    setStateActivationDockMenu(m_pActionActiverDockGloss, "Glossaire",tr("Le Glossaire"));

    m_pActionActiverDockResum = new QAction( "?", 0, this, "ActionActiverAccesRapide" );
    connect( m_pActionActiverDockResum, SIGNAL( activated() ), this, SLOT( Slot_SetDockMenu_AccesRapide() ) );
    setStateActivationDockMenu(m_pActionActiverDockResum, "Rapid_Access", tr("Le Résumé patient (Accès rapide)"));
    connect(G_pCApp,    SIGNAL(Sign_QuitterRequired()),      this, SLOT(Slot_SauverLesMeubles()));


    m_pActionActiverDockVigie->addTo( m_fenetres );
    m_pActionActiverDockButts->addTo( m_fenetres );
    m_pActionActiverDockGloss->addTo( m_fenetres );
    m_pActionActiverDockResum->addTo( m_fenetres );
    m_fenetres->insertItem( tr("&Enregistrer la position des fenêtres"),                   this, SLOT(savePos()), 0 );
    m_fenetres->insertItem( tr("Enregistrer la position des fenêtres &filles seulement"),  this, SLOT(saveOnlyChildPos()), 0 );
    m_fenetres->insertItem( tr("&Rétablir la position des fenêtres"),                      this, SLOT(Fenetres_RetablirPosition()),0);
    m_fenetres->insertItem( tr("Fenêtres en &cascade"),                                    this, SLOT(Fenetres_en_cascade()),  0);
    m_fenetres->insertItem( tr("Fenêtres en &mosaïque"),                                   this, SLOT(Fenetres_en_mosaique()), 0 );
    m_fenetres->insertItem( tr("Aligner toutes les fenêtres sur la fenêtre active"),       this, SLOT(Fenetres_Identiques()), 0 );

    setupVigieActions();    // doit absolument etre apres initialisation de m_fenetres

    /*
    fenetres->insertSeparator();
    fenetres->insertItem( tr("Zoom 8  pts"), this, SLOT(setZoom8()), 0 );
    fenetres->insertItem( tr("Zoom 9  pts"), this, SLOT(setZoom9()), 0 );
    fenetres->insertItem( tr("Zoom 10 pts"), this, SLOT(setZoom10()), 0 );
    fenetres->insertItem( tr("Zoom 11 pts"), this, SLOT(setZoom11()), 0 );
    fenetres->insertItem( tr("Zoom 12 pts"), this, SLOT(setZoom12()), 0 );
    fenetres->insertItem( tr("Zoom 14 pts"), this, SLOT(setZoom14()), 0 );
    fenetres->insertItem( tr("Zoom 16 pts"), this, SLOT(setZoom16()), 0 );
    fenetres->insertItem( tr("Zoom 18 pts"), this, SLOT(setZoom18()), 0 );
    fenetres->insertItem( tr("Zoom 20 pts"), this, SLOT(setZoom20()), 0 );
    */
    //.......................... barre de menus Aide ........................................................
    ThemePopup * help = new ThemePopup( this );
    menuBar()->insertItem( tr("Ai&de"), help );

    //help->insertItem( tr("&À propos"),        this, SLOT(about()),  Key_F11 );
    help->insertItem( tr("&À propos et aide"),        this, SLOT(Slot_actionApropos()),  Key_F1 );
    //help->insertItem( tr("À propos de &Qt"),  this, SLOT(aboutQt()) );
    help->insertSeparator();
    help->insertItem( tr("&Qu'est-ce que c'est ?"), this, SLOT(whatsThis()), SHIFT+Key_F1 );

    //................... creation du QVbox central ...............................................
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 0 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //................... y placer une barre d'onglets .............................................
    //                    qui gerera la disposition des fenetres MDI
    READ_USER_PARAM(USER_PARAM, "Tab Rubriques", tr("Display"), &val1);
    if (val1 != tr("false"))
       {FormOngletRubrique *pFormOngletRubrique = new FormOngletRubrique(pQVCentral, "TabPanelRubrique");
        m_pFormOngletRubrique = pFormOngletRubrique; // noter dans DrTux le pointeur sur ce widget d'onglets
        connect( m_pFormOngletRubrique->tabWidget, SIGNAL( currentChanged ( QWidget * )), this,        SLOT(   OnOngletRubriquesChanged(QWidget *)));
        connect( m_pFormOngletRubrique,            SIGNAL( message(const QString&, int )),statusBar(), SLOT(   message(const QString&, int )) );
       }
    //................... y placer un QWorkspace ..................................................
    //                    qui contiendra et gerera les fenetres MDI
    m_pQWorkSpaceRub = new QWorkspace(pQVCentral);
    if (!m_pQWorkSpaceRub) return;

    m_pQWorkSpaceRub->setScrollBarsEnabled( TRUE );
    setCentralWidget(  pQVCentral );
    m_pQWorkSpaceRub->setPaletteBackgroundPixmap (Theme::getIcon( "ImageDeFond.png") );
    //.................. y connecter les onglets rubriques .........................................
    //                   pour qu'il mettent à jour celui actif
    //                   lorsqu'une fenetre est cliquée
    if (m_pFormOngletRubrique) connect( m_pQWorkSpaceRub,          SIGNAL( windowActivated ( QWidget * )), m_pFormOngletRubrique,  SLOT(   OnRubriqueActived(QWidget * )));

    //........................................................................................
    //................Créér les Docks ........................................................
    //G_pCApp->m_pDrTux = this;
    Get_RecordDispos();
    setupDocks();
    InstallCustomMenu();
    //................... creer les  fenetres MDI (fenetres Identité,prescription, observation) ..................

    SetInterfaceOnDroits(G_pCApp->m_Droits);
    AfficheRubriques();
    Get_RecordDispos();

    //............................... un peu de placement ........................................................
    if (CGestIni::Param_ReadParam(USER_PARAM, "DrTuxPos", "Positions", &val1, &val2, &val3, &val4)==0)  // zero = pas d'erreur
       {int x = atoi(val1);
        int y = atoi(val2);
#ifdef Q_OS_MACX
        if (y<20) y=20;
#endif
        if (x>=0 && y>=0)
            {move(y,   y);
             resize(val3.toInt(), val4.toInt());
             show();
            }
        else
            {showMaximized ();
            }
       }
    else
       {showMaximized ();
       }
    m_idToolMenu = m_fenetres->insertItem (tr("Cacher montrer outils"),createDockWindowMenu () );
    //
    //............................... afficher le gadjet de bienvenue ...................................
    QTimer::singleShot( 500, this, SLOT(AutoSetActiveWindow()) );

    setIcon( Theme::getIcon( "32x32/DrTux.png" ));
    statusBar()->hide();
    //................... couleur fixe valerie ...............
    QString truc;
    CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Valerie", "Couleur rapide", &truc);   // path editeur de texte non d�fini
    // qDebug(QString("value : %1").arg(truc));
    if (truc.length())
       {QPixmap pix2( 4, 16 );
        m_LasTextColorChoice = QColor(truc);
        pix2.fill( m_LasTextColorChoice );
        m_pActionFastTextColor->setIconSet( pix2 );
       }

    G_pCApp->m_pDrTux = this;
}
//--------------------------------- Slot_SauverLesMeubles --------------------------------------------------------------------------------
void DrTux::Slot_SauverLesMeubles()
{tryToStopAPropos();
 LapClose();
}
/*
//--------------------------------- doExportForMedicatux --------------------------------------------------------------------------------
QString DrTux::doExportForMedicatux()
{
}
*/
//----------------------------------- Slot_actionApropos -----------------------------------------------------------------------
void DrTux::Slot_actionApropos()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}
//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void DrTux::Slot_actionAproposDisplay()
{        //CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"Ressources/Changements.html",textEdit_Changements->text());
         QStringList argList;
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).dirPath (true));
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).dirPath (true))+"Ressources/";
         QString versionBase        = "";
         G_pCApp->m_pCMoteurBase->GetMedinTuxNormalisedVersion(versionBase,".");
         //......................... completer les autres arguments .........................................

         if (m_Apropos_Proc==0)
            {//m_action_A_Propos->setDisabled(TRUE);
             m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->addArgument( pathExeAPropos);                                               // 1  nom du module
             m_Apropos_Proc->addArgument("DrTux");                                                       // 1  nom du module
             m_Apropos_Proc->addArgument("Module de gestion du dossier patient");                        // 2  description courte
             m_Apropos_Proc->addArgument(G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("=") + " Qt : "+QT_VERSION_STR);    // 3  numero de version
             m_Apropos_Proc->addArgument(G_pCApp->m_PathAppli+"Ressources/Changements.html");            // 4  fichiers d�crivant les changements
             m_Apropos_Proc->addArgument(Theme::getPath(Theme::WithSeparator)+"32x32/DrTux.png");        // 5  Icone du programme
             m_Apropos_Proc->addArgument(G_pCApp->m_PathAppli+"../../Doc/DrTux.html");                   // 6  aide en ligne
             m_Apropos_Proc->addArgument("");                                                            // 7  fichier template de APropos (vide pour celui par defaut)
             m_Apropos_Proc->addArgument(versionBase);                                                   // 8  numero de version de la base de donneee
             m_Apropos_Proc->start();
             SLEEP(1);
             G_pCApp->processEvents ();
             while ( m_Apropos_Proc->isRunning () /* && QFile::exists(pathBinRessources+"~A_propos.html")*/)
                   { //qDebug(QString::number(procState).toAscii());
                     G_pCApp->processEvents ( QEventLoop::WaitForMore );
                   }
             if (m_Apropos_Proc) delete m_Apropos_Proc;
             m_Apropos_Proc = 0;
             QFile::remove(pathBinRessources+"~A_propos.html");
             //m_action_A_Propos->setDisabled(FALSE);
            }
}
//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void DrTux::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}
//------------------------------ Slot_MenuRubriques ------------------------
/*! \brief active le popup menu des fenetres de rubriques et active la fenetre si selection de l'une d'elle
*/
void DrTux::Slot_MenuRubriques()
{QStringList listOptions;
 //.......... creer liste des rubriques autorisées a etre vues .................................
 for (int i = 0; i< (int)G_pCApp->m_ListRubName.count(); ++i)
     {QString rubName = G_pCApp->m_ListRubName[i];
      if (G_pCApp->IsThisDroitExist( G_pCApp->m_Droits, G_pCApp->m_mapNameRubInfos[rubName]+"v"))
          listOptions.append(QString("#16x16/type_")+rubName+".png"+"#"+rubName);
      //else
      //    listOptions.append(QString("~#16x16/type_")+rubName+".png"+"#"+rubName);
     }
 QString ret = G_pCApp->DoPopupList(listOptions);
 if (ret.length()==0) return;
 OnActiverOrCreateRubrique(ret);
}

//------------------------------ SlotMakeVisible ------------------------
void DrTux::SlotMakeVisible(const QString&)
{showMinimized () ;
 qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
 setWindowState(windowState() & ~WindowMinimized | WindowActive);
}

//------------------------------ Slot_ImportMenuModulesLocalize ------------------------
void DrTux::recordPathModulesMenus(QString path)
{QString temp = CGestIni::AbsoluteToRelativePath(G_pCApp->m_PathAppli, path);  // ne peut pas etre reference par rapport au glossaire
  WRITE_USER_PARAM(&USER_PARAM, "MenuContextuel", "PathLib", temp);
  UPDATE_USER_PARAM(&USER_PARAM, G_pCApp->m_User);
  temp         = path.replace(G_pCApp->m_PathGlossaire,"$Glossaire");
  CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam,  "MenuContextuel", "PathLib",    temp);
  CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, G_pCApp->m_DrTuxParam);
}

//---------------------------------------------- Get_RecordDispos ------------------------------------------------------------
/*! \brief recupere dans les parametres utilisateur la chaine de caracteres dans laquelle sont contenues les positions des barres d'outils et fenetres des docks, et la passe a QMainWindow
*/
void DrTux::Get_RecordDispos()
{   QString str;
    if (READ_USER_PARAM(USER_PARAM, "DrTuxDockPos", "Positions", &str )==0)  // zero = pas d'erreur
       {    CGestIni::retrieveStringForParam(str);
            QCString cStr  = str.utf8 ();
            long  len_data =  cStr.length ();
            QByteArray array( len_data ) ;
            memcpy( array.data(), (const char*) cStr, len_data);
            QTextStream tsa( array, IO_ReadOnly );
            tsa >> *this;
       }
}

//---------------------------------------------- Slot_RecordDispos ------------------------------------------------------------
/*! \brief Cree la chaine de caracteres dans laquelle sont contenues les positions des barres d'outils et fenetres des docks. Cette chaine est stocke dans les parametres utilisateur
*/
QString  DrTux::Slot_RecordDispos()
{
    QByteArray  array ;
    QTextStream tsa( array, IO_WriteOnly);
    tsa << *this;

    QString str = "";
    str = str.insert(0,array);
    CGestIni::formatStringForParam(str);
    return str;
}

//--------------------------------------- Slot_SetDockMenu_AccesRapide --------------------------------
/*! \brief gère si oui ou non il faut afficher Dock_Menu. Ce slot est appelé par le menu de DrTux (Afficher/Cacher le menu d'accès rapide).
 * Si Dock_Menu n'existe pas, le créer et gérer le ini ; et vice-versa
*/
void DrTux::Slot_SetDockMenu_AccesRapide()
{if (  (m_pDock_Menu = SetDockMenu_OnOff(m_pDock_Menu, QString("Rapid_Access")+ tr("@Vue rapide")))!=0)
    {   m_pForm_Menu = new Dock_Menu(m_pDock_Menu, "Rapid_Access");
       // Fonctions sélection de documents
        connect( m_pForm_Menu, SIGNAL( Sign_ActiverDocument(QString ,  QString, int )),   this,   SLOT ( Slot_ActiverDocument(QString ,  QString, int )) );
        connect( m_pForm_Menu, SIGNAL( Sign_ActiverRubrique(const char*, int)),           this,   SLOT ( OnActiverOrCreateRubrique(const char*, int)) );
        // Fonctions de Glossaire du Dock_Menu
        connect( m_pForm_Menu, SIGNAL( Sign_GlossaireFileSelected(const char*, int)),     this,   SLOT ( OnGlossaireFileClicked(const char*, int))   );
        // Fonction de création de nouveau dossier patient
        connect( m_pForm_Menu,   SIGNAL( Sign_CreateNewIdent()),                          this,   SLOT ( OnCreateNewIdent())  );
        connect (m_pForm_Menu,   SIGNAL ( Sign_NoteToBeSynchronized(int, const char*) ),  this,   SLOT ( Slot_NoteToBeSynchronized(int, const char*)   )       );

        // Teste l'existence des paramètres du Menu sinon les créer
        if (READ_USER_PARAM(USER_PARAM, "Dock_Rapid_Access","ClickOnParent") != 0)
           { m_pForm_Menu->createIniParams( USER_PARAM );
           }
        m_pForm_Menu->getIniParams( USER_PARAM );
        UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User);
        m_pForm_Menu->setModifiableState(m_IsModifiable);
        m_pForm_Menu->setParams(G_pCApp->m_pCMoteurBase, &m_RubList,
                                G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User,
                                G_pCApp->m_Droits, G_pCApp->m_pAtcd_Code);
        m_pForm_Menu->show();
        m_pDock_Menu->setWidget( m_pForm_Menu );
     } // Fin if Dock_Menu
 else m_pForm_Menu = 0;    // car aura été détruit avec le dock
 setStateActivationDockMenu(m_pActionActiverDockResum, "Rapid_Access", tr("Le Résumé patient (Accès rapide)"));
}

//--------------------------------------- Slot_SetDockMenu_Glossaire --------------------------------
/*! \brief gère si oui ou non il faut afficher m_pDockGlossaire ou l'ancienne version de TabRubrique. Ce slot est appelé par le menu de DrTux (Afficher/Cacher le menu d'accès rapide).
 * Si m_pDockGlossaire n'existe pas, le créer et gérer le ini ; et vice-versa
*/
void DrTux::Slot_SetDockMenu_Glossaire()
{ if ( (m_pDockGlossaire = SetDockMenu_OnOff(m_pDockGlossaire, "Glossaire"))!=0)
     {
      //................Y inserer le dialogue ........................................................
      m_pFormGlossaire = new C_FormGlossaire(m_pDockGlossaire, "Form_Glossaire");
      m_pDockGlossaire->setWidget( m_pFormGlossaire );
      m_pFormGlossaire->show();
      //............... connecter le glossaire au DrTux pour placement du texte ..........................
      connect( m_pFormGlossaire, SIGNAL( Sign_FileSelected(const char*, int)),     this,        SLOT(OnGlossaireFileClicked(const char* , int))  );
      connect( m_pFormGlossaire, SIGNAL( Sign_FileChanged(const char*, int)),      this,        SIGNAL (Sign_OnGlossaireFileClicked(const char* , int))  );
      //............... connecter le glossaire au DrTux pour edition du texte ..........................
      connect( m_pFormGlossaire, SIGNAL( Sign_FileToEdit(QString, QString, int)),  this,        SLOT(OnGlossaireFileToEdit(QString, QString, int)) );
      //............... connecter le DrTux au Glossaire pour mise à jour ..........................
      connect( this,             SIGNAL( Sign_GlossaireRefresh()),       m_pFormGlossaire ,     SLOT(Slot_GlossaireRefresh()) );
      //.................. connecter le glossaire ........................................................
      //                   pour que le glossaire puisse mette à jour le type
      //                   de masque à afficher selon rubrique (prescription ou observation)
      connect( m_pQWorkSpaceRub, SIGNAL( windowActivated ( QWidget *  )),  m_pFormGlossaire,    SLOT(OnGlossaireRubriqueActived(QWidget * )));
      //.................. connecter le bouton de changement ........................................................
      //                   de repertoire du Glossaire
      connect( m_pFormGlossaire->pushButtonLocalise,   SIGNAL( clicked ()), SLOT(   Slot_OnGlossaireLocalise()) );
      if (m_pC_Organiseur) m_pC_Organiseur->DoConnexionOnGossaire(m_pFormGlossaire);
      // methode independante d'une liste de fenetres codee en dur
      QWidgetList windows = m_pQWorkSpaceRub->windowList();
      for ( int u = 0; u < int(windows.count()); ++u )
          {CMDI_Generic *cmdi = (CMDI_Generic*) windows.at(u);    // on  tombe sur la CMDI
           if (cmdi) cmdi->DoConnexionOnGossaire(m_pFormGlossaire);
          }
      //.................. connecter la fermeture par la croix de fermeture du dock ........................
      //       connect( m_pDockGlossaire, SIGNAL(visibilityChanged (bool)), this, SLOT( Slot_SetDockMenu_Glossaire() )  );
     }
 else m_pFormGlossaire = 0;    // car aura été détruit avec le dock
 setStateActivationDockMenu(m_pActionActiverDockGloss, "Glossaire",tr("Le Glossaire"));
}

//--------------------------------------- Slot_SetDockMenu_RubriquesButton --------------------------------
/*! \brief Affiche/Cache le menu des rubriques
*/
void DrTux::Slot_SetDockMenu_RubriquesButton()
{if ( (m_pDock_TabRubrique = SetDockMenu_OnOff(m_pDock_TabRubrique, "Rubriques"))!=0)
     {
      //................Y inserer le dialogue ........................................................
      m_pForm_TabRubrique = new Dock_TabRubrique(m_pDock_TabRubrique, "TabRubrique");
      //m_pForm_TabRubrique->initDialog(0);
      m_pDock_TabRubrique->setWidget( m_pForm_TabRubrique );
      m_pForm_TabRubrique->show();
      connect( m_pForm_TabRubrique, SIGNAL( Sign_ActiverOrCreateRubrique(const char*, int)),   this,    SLOT(   OnActiverOrCreateRubrique(const char*, int)) );
     }
 else m_pForm_TabRubrique = 0;    // car aura été détruit avec le dock
 setStateActivationDockMenu(m_pActionActiverDockButts, "Rubriques",tr("Les boutons d'activation des rubriques"));
}

//--------------------------------------- isThisDockMenu_Desired --------------------------------
/*! \brief Affiche/Cache le menu des rubriques
*/
int DrTux::isThisDockMenu_Desired(QString name)
{ QString        state  = "";
  QString        d_Name = "";
  int               pos;
  if ( (pos=name.find("@"))!=-1)
     {d_Name = tr("Dock_") + name.left(pos);
      name   = name.mid(pos+1);
     }
  else
     {d_Name = tr("Dock_") + name;
     }
  d_Name.replace(" ","_");

  //................. lire l'état de Dock_Menu dans le fichier INI ........................
  if ( READ_USER_PARAM(USER_PARAM ,d_Name, tr("Montrer"), &state)==0)
     {if      (state== tr ("Cacher"))   return 0;
      else if (state== tr ("Normal"))   return 1;
     }
  return 2; // indetermine
}

//--------------------------------------- Slot_SetDockMenu_Organiseur --------------------------------
/*! \brief Affiche/Cache le menu de l'organiseur
*/
//    QDockWindow         *m_pDock_Organiseur;		/*!< widget dock de l'organiseur */
//    C_Organiseur        *m_pC_Organiseur;            /*!< widget  de l'organiseur */

void DrTux::Slot_SetDockMenu_Organiseur()
{if  ( (m_pDock_Organiseur = SetDockMenu_OnOff(m_pDock_Organiseur, "Vigie"))!=0)
     {m_pActionVigie->setIconSet ( Theme::getIcon("16x16/Vigie.png") ) ;
      m_pActionVigie->setMenuText ( tr("Cacher la vigie") );
      m_pActionVigie->setText ( tr("Cacher la vigie") );
      m_pActionVigie->setEnabled(TRUE);
      //................Y inserer le dialogue ........................................................
      m_pC_Organiseur = new C_Organiseur(&m_EvnList, &m_RubList, G_pCApp->m_ID_Doss ,m_pDock_Organiseur, "TabRubrique");

      m_pDock_Organiseur->setWidget( m_pC_Organiseur );
      m_pDock_Organiseur->show();

      connect( m_pC_Organiseur, SIGNAL( Sign_ActiverDocument(QString, QString, int)),         this,    SLOT(   Slot_ActiverDocument(QString, QString, int)) );
      connect( m_pC_Organiseur, SIGNAL( Sign_CreateNewRootInOrganiseur(QString)),             this,    SLOT(   On_CreateNewRootInOrganiseur(QString)) );
      connect( m_pC_Organiseur, SIGNAL( Sign_DragFileInOrganiseur(QString, QString *)),       this,    SLOT(   On_DragFileInOrganiseur(QString, QString *)) );
      connect( this,            SIGNAL( Sign_OnIdentChange(const char*,  const char*, const char*,  const char* )),
               m_pC_Organiseur, SLOT(   On_IdentChange(const char*,  const char*, const char*,  const char* ))  );
      connect( m_pC_Organiseur,            SIGNAL( Sign_IdentChangeInOrganiseur(const char*,  const char*, const char*,  const char* )),
               this,                       SLOT(   OnDrTuxIdentChange(const char*,  const char*, const char*,  const char* ))  );
      connect( m_pDock_Organiseur, SIGNAL( visibilityChanged ( bool  ) ),  this, SLOT(   On_Vigie_visibilityChanged ( bool  ))  );

      m_pC_Organiseur->DoConnexionOnGossaire(m_pFormGlossaire);
      m_pC_Organiseur->SetInterfaceOnDroits(G_pCApp->m_Droits);

     }
 else
     {m_pC_Organiseur = 0;
      m_pActionVigie->setIconSet ( Theme::getIcon("16x16/VigieHide.png") );
      m_pActionVigie->setMenuText (tr("Montrer la vigie"));
      m_pActionVigie->setEnabled(FALSE);
     }    // car aura été détruit avec le dock
 setStateActivationDockMenu(m_pActionActiverDockVigie, "Vigie",tr("La Vigie"));
}

//--------------------------------------- setStateActivationDockMenu --------------------------------
/*! \brief positionne le bon texte des menus d'activation desactivation des dock
*/
void DrTux::setStateActivationDockMenu(QAction *pQAction, const QString &dockName, const QString text)
{
 int state = isThisDockMenu_Desired(dockName) ;
 if      (state==1) {pQAction->setMenuText(tr("Désactiver %1").arg(text));            pQAction->setIconSet (Theme::getIcon("16x16/CheckBoxOn.png") ); }
 else if (state==0) {pQAction->setMenuText(tr("Activer %1").arg(text));               pQAction->setIconSet (Theme::getIcon("16x16/CheckBoxOff.png") ); }
 else               {pQAction->setMenuText(tr("Etat intéterminé pour %1").arg(text)); pQAction->setIconSet (Theme::getIcon("16x16/CheckBoxIndeter.png") ); }
}
//--------------------------------------- On_Vigie_visibilityChanged --------------------------------
/*! \brief réagit au message visibilityChanged() envoyé par la vigie lorsqu'elle modifie sa visibilite
*/
void DrTux::On_Vigie_visibilityChanged ( bool visible )
{if (visible) {m_pActionVigie->setIconSet ( Theme::getIcon("16x16/Vigie.png") ) ;      m_pActionVigie->setMenuText ( tr("Cacher la vigie") );  m_pActionVigie->setText ( tr("Cacher la vigie") );}
 else         {m_pActionVigie->setIconSet ( Theme::getIcon("16x16/VigieHide.png") ) ;  m_pActionVigie->setMenuText ( tr("Montrer la vigie") ); m_pActionVigie->setText ( tr("Montrer la vigie"));}
}

//--------------------------------------- ActiverVigie --------------------------------
/*! \brief SLOT qui active et desactive la vigie réagit au bouton d'activation m_pActionVigie
*/
void DrTux::ActiverVigie()
{if (m_pDock_Organiseur==0 || m_pC_Organiseur==0) return;
 if (m_pDock_Organiseur->isVisible())
    { m_pDock_Organiseur->hide();//removeDockWindow ( m_pDock_Organiseur );
      m_pActionVigie->setIconSet ( Theme::getIcon("16x16/VigieHide.png") ) ;
    }
 else
    { m_pDock_Organiseur->show();//addDockWindow ( m_pDock_Organiseur,  Qt::DockLeft);
      m_pActionVigie->setIconSet ( Theme::getIcon("16x16/Vigie.png") ) ;
    }
}

//--------------------------------------- On_CreateNewRootInOrganiseur --------------------------------
/*! \brief recoit de l'organiseur ou tout autre truc un fichier et doit creer un document avec.
*/
void DrTux::On_DragFileInOrganiseur(QString file_in, QString *pDocSerialize)
{QString name   = "";
 *pDocSerialize = "";
 int pos = file_in.find(G_pCApp->m_PathGlossaire);
 if (pos != -1)
    {//............ rechercher le nom de la rubrique ou doit ..............................
     //             etre cree le documen à partir de son chemin
     QString file = file_in.mid(pos);
     pos         += G_pCApp->m_PathGlossaire.length();
     name         = file_in.mid(pos);
     name         = name.replace('\\','/'); name = name.replace("//","/");
     int end = name.find('/');
     if (end != -1)
       {name.truncate(end);
        int type = G_pCApp->RubNameToType(name );
        if (type==0)
           {name = RUBNAME_DOCUMENTS;
            type = TYP_DOCUMENTS;
           }
        QUrl::decode(file);
        CRubRecord* pCRubRecord = OnGlossaireFileClicked(file, type, 1 );
        if (pCRubRecord) *pDocSerialize = pCRubRecord->serialize();
       }
   }
}

//--------------------------------------- On_CreateNewRootInOrganiseur --------------------------------
/*! \brief met à jour le document place dans l'organiseur afin qu'il tienne compte de sa position de racine
*/
void DrTux::On_CreateNewRootInOrganiseur(QString /*primKey*/)
{//CRubRecord* pCRubRecord = DocPrimKeyToRubRecord(primKey);
 //if (pCRubRecord==0) return;
}

//--------------------------------------- SetDockMenu_OnOff --------------------------------
/*! \brief Non documentée
*/
QDockWindow *DrTux::SetDockMenu_OnOff(QDockWindow* pQDockWindow,  QString name)
{ QString  valDock_Menu = "";
  QString        d_Name = "";
  int               pos;
  if ( (pos=name.find("@"))!=-1)
     {d_Name = tr("Dock_") + name.left(pos);
      name   = name.mid(pos+1);
     }
  else
     {d_Name = tr("Dock_") + name;
     }
  d_Name.replace(" ","_");
    // Récupère le Dock parent
  if (pQDockWindow != 0)
  { // Effacer le dock
    removeDockWindow ( pQDockWindow );
    delete  pQDockWindow;
    //delete m_pForm_Menu;  // inutile et plantage car déjà détruit lors destruction du père
    pQDockWindow = 0;
  }
  else
  { //................................. un peu de geometrie ........................................
    //                                  pour positionner le dock gauche
    int   w_dock = getDockMenu_Width();
    pQDockWindow = new QDockWindow (this, name);
    if (pQDockWindow)
       {pQDockWindow->setResizeEnabled(TRUE);
        pQDockWindow->setFixedExtentWidth (w_dock);
        pQDockWindow->setCloseMode (QDockWindow::Always);
        pQDockWindow->setCaption(name);
        pQDockWindow->setMargin( 0 );
        addDockWindow ( pQDockWindow, Qt::DockLeft );
       }
  }
  if (pQDockWindow == 0) valDock_Menu="Cacher";
  else                   valDock_Menu="Normal";
  //................. Sauvegarder l'état de Dock_Menu dans le fichier INI ........................
  WRITE_USER_PARAM  (&USER_PARAM, d_Name , tr("Montrer"), valDock_Menu);
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User );
  return pQDockWindow;
}


//--------------------------------------- getDockMenu_Width --------------------------------
/*! \brief Définit la largeur des docks
*/
int DrTux::getDockMenu_Width()
{ int w_dock = W_DOCK_LEFT;
  QString val1, val2, val3, val4;
  if (READ_USER_PARAM(USER_PARAM, "Dock_Glossaire", tr("Position"), &val1, &val2, &val3, &val4)==0 )  w_dock = atoi(val3);
  return w_dock;
}

//--------------------------------------- setupDocks --------------------------------
/*! \brief Prépare l'affichage des docks dans la fenêtre (TabRubrique, Dock_Menu, Glossaire)
*/
void DrTux::setupDocks()
{ QString val1 = "";
  if (READ_USER_PARAM(USER_PARAM, "Dock_Rapid_Access", tr("Montrer"), &val1)!=0)
  { // Afficher par defaut
    Slot_SetDockMenu_AccesRapide();
  }
  else
  { // Selon le INI
    if (val1 == tr("Normal")) Slot_SetDockMenu_AccesRapide();
  }

  if (READ_USER_PARAM(USER_PARAM,"Dock_Glossaire", tr("Montrer"), &val1)!=0)
  { // Afficher par defaut
    Slot_SetDockMenu_Glossaire();
  }
  else
  { // Selon le INI
    if (val1 == tr("Normal")) Slot_SetDockMenu_Glossaire();
  }

  if (READ_USER_PARAM(USER_PARAM,"Dock_Rubriques", tr("Montrer"), &val1)!=0)
  { // Afficher par defaut
    Slot_SetDockMenu_RubriquesButton();
  }
  else
  { // Selon le INI
    if (val1 == tr("Normal")) Slot_SetDockMenu_RubriquesButton();
  }
 if (READ_USER_PARAM(USER_PARAM,"Dock_Vigie", tr("Montrer"), &val1)!=0)
  { // Afficher par defaut
    Slot_SetDockMenu_Organiseur();
  }
  else
  { // Selon le INI
    if (val1 == tr("Normal")) Slot_SetDockMenu_Organiseur();
  }
}
//------------------------------ savePos ----------------------
void DrTux::savePos()
{savePos(1);
}
//------------------------------ saveOnlyChildPos ----------------------
void DrTux::saveOnlyChildPos()
{savePos(0);
}

//------------------------------ savePos ----------------------
void DrTux::savePos(int mode)
{
    QPoint p ;    // store position
    QSize  s ;    // store size
    CMDI_Generic*  widget ;           // nom doit etre: "Observation" "Prescription" "Choix patient" "Identité"

    for ( QStringList::Iterator it = G_pCApp->m_ListRubName.begin(); it != G_pCApp->m_ListRubName.end(); ++it )
     {if ( (widget  = (CMDI_Generic*)IsExistRubrique(*it)) )
           {widget->saveGeometry();
           }
        else
           {WRITE_USER_PARAM(&USER_PARAM, *it, tr("Montrer"), tr("Non"));
           }
     }
    //..................................fenetre dock glossaire ..............................................
    /*                     deja fait avec la sauvegarde de la main window principale
    if ( m_pDockGlossaire )
       {p = m_pDockGlossaire->pos() ;   // store position
        s = m_pDockGlossaire->size();   // store size
        WRITE_USER_PARAM(&USER_PARAM,    "Dock_Glossaire", tr("Montrer"), tr("Normal"));
        WRITE_USER_PARAM(&USER_PARAM,    "Dock_Glossaire", tr("Position"),
                                         QString::number(p.x()),     QString::number(p.y()),
                                         QString::number(s.width()), QString::number(s.height()));
       }
    */
    QString rub_name="";
    if (GetCurrentRubrique(&rub_name))
       {WRITE_USER_PARAM(&USER_PARAM,  "DrTuxPos", tr("Rubrique active"), rub_name);
       }
    //..................................liste des fenetres ..............................................
    QWidgetList windows = m_pQWorkSpaceRub->windowList();
    for ( int u = 0; u < int(windows.count()); ++u )
       {if (windows.at(u))
           {if (windows.at(u)->parentWidget ())
               {p = windows.at(u)->parentWidget ()->pos() ;   // store position
                s = windows.at(u)->parentWidget ()->size();   // store size
                WRITE_USER_PARAM(&USER_PARAM,    "Windows List", windows.at(u)->caption(),
                                                  QString::number(p.x()),               QString::number(p.y()),
                                                  QString::number(s.width()),           QString::number(s.height()));
               }
           }
       }

    if (mode)
        {WRITE_USER_PARAM(&USER_PARAM,              "DrTuxPos", tr("Positions"),
                                                     QString::number(this->x()),               QString::number(this->y()),
                                                     QString::number(this->width()),           QString::number(this->height()));
        }
     else
        {WRITE_USER_PARAM(&USER_PARAM,  "DrTuxPos", tr("Positions"),
                                                     "-1",           "-1",
                                                     "-1",           "-1");
        }
 //......................... sauver les parametres dans données utilisateur ....................
    WRITE_USER_PARAM(&USER_PARAM,  "DrTuxDockPos", tr("Positions"), Slot_RecordDispos());
 //_________________________________________________________________________________________________________________
    UPDATE_USER_PARAM(&USER_PARAM, G_pCApp->m_User);
 //_________________________________________________________________________________________________________________
}

//--------------------------------------- Fenetres_en_cascade --------------------------------
void DrTux::Fenetres_en_cascade()
{m_pQWorkSpaceRub->cascade ();
}

//--------------------------------------- Fenetres_en_mosaique --------------------------------
void DrTux::Fenetres_en_mosaique()
{m_pQWorkSpaceRub->tile ();
}
//--------------------------------------- Fenetres_Identiques --------------------------------
void DrTux::Fenetres_Identiques()
{   CMDI_Generic*  widget = G_pCApp->GetCurrentRubrique( );
    if (widget == 0) return;
    int x = widget->parentWidget()->x();
    int y = widget->parentWidget()->y();
    int w = widget->parentWidget()->width();
    int h = widget->parentWidget()->height();
    // methode independante d'une liste de fenetres codee en dur
    QWidgetList windows = m_pQWorkSpaceRub->windowList();
    for ( int u = 0; u < int(windows.count()); ++u )
       {CMDI_Generic *cmdi = (CMDI_Generic*) windows.at(u);    // on  tombe sur la CMDI
        if (cmdi)
           {cmdi->parentWidget()->move   ( x,  y );
            cmdi->parentWidget()->resize (  w,  h );
           }
       }
}

//--------------------------------------- Fenetres_RetablirPosition --------------------------------
void DrTux::Fenetres_RetablirPosition()
{int x = 0;
 int y = 0;
 QString val1, val2, val3, val4, v_al1, v_al2, v_al3, v_al4;

 CMDI_Generic *cmdi = 0;
 for ( QStringList::Iterator it = G_pCApp->m_ListRubName.begin(); it != G_pCApp->m_ListRubName.end(); ++it )
     {if ( (cmdi  = (CMDI_Generic*)IsExistRubrique(*it)) )
         {if (READ_USER_PARAM(USER_PARAM,  cmdi->m_RubName, tr("Position"), &val1, &val2, &val3, &val4, &v_al1, &v_al2, &v_al3, &v_al4)==0 )
             {x = val1.toInt();
              y = val2.toInt();
              cmdi->move(   v_al1.toInt(), v_al2.toInt() );          // restore position
              cmdi->resize( v_al3.toInt(), v_al4.toInt());
             }
          cmdi->parentWidget()->move( x,y );
         }
      else
         {READ_USER_PARAM(USER_PARAM, *it, tr("Montrer"), &val1);
          if (val1==tr("Normal")) OnActiverOrCreateRubrique(*it);
         }
     }
}

//--------------------------------------- AutoSetActiveWindow --------------------------------
void DrTux::AutoSetActiveWindow()
{QString val;
 CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam,  "Connexion", "SplashScreen", &val );
 if (val.lower()==tr("oui")) G_pCApp->CouCou( "", "$Theme/MedinTux.png", 5000);
 QString rub_name;
 if (READ_USER_PARAM(USER_PARAM,  "DrTuxPos",  tr("Rubrique active"), &rub_name)==0)
    {OnActiverRubrique(rub_name, 0);
    }
 else if (m_pCMDI_Observation)
    {OnActiverRubrique(m_pCMDI_Observation->GetRubName(), 0);
    }
 SetDefaultDoc();
 connectAllCustomQAction();
}

//--------------------------------------- SetDefaultDoc --------------------------------
/*! \brief Ajoute les documents par défaut dans le dossier si les rubriques sont vides (observation, terrain).
*/
void DrTux::SetDefaultDoc()
{QString path;
 int verbose=0;
 QString rubName;
 for (int i=0; i< (int)G_pCApp->m_ListRubName.count(); ++i)
     {rubName = G_pCApp->m_ListRubName[i];
      if (rubName==tr("Vigie") && m_pDock_Organiseur&&m_pC_Organiseur)
         { if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, rubName, tr("Defaut"), &path)==0)
              {
                if (path[0]!=':') {verbose = 0;}
                path = path.remove(":").stripWhiteSpace();
                if (path.length() && path[0]!='*')
                   {path = G_pCApp->resolvePath(path);
                    if (QFile::exists( path  ))
                       {//G_pCApp->CouCou( tr("Classeurs par défaut en cours de création"), "$Theme/MessagePopVigie.png", 3000);
                        m_pC_Organiseur->m_ListView_MonitorPatient->setUpdatesEnabled( FALSE );
                        OnGlossaireFileClicked(path, G_pCApp->RubNameToType(rubName));
                        m_pC_Organiseur->makeListeMonitor();
                        m_pC_Organiseur->setListviewOnDossier(G_pCApp->m_ID_Doss);
                        m_pC_Organiseur->m_ListView_MonitorPatient->setUpdatesEnabled( TRUE );
                        m_pC_Organiseur->m_ListView_MonitorPatient->repaint();
                       }
                    else if (verbose) G_pCApp->CouCou( tr("Fichier par défaut  non trouvé, lors ouverture de la liasse par défaut de la Vigie"), "$Theme/MessagePopError-01.png", 30000);
                   } // if (path[0]!='*')
              }
           else G_pCApp->CouCou( tr("Erreur lors de la lecture du fichier de paramètres, mention \"Vigie\" non trouvée ou non indiqué, lors ouverture du document par défaut"), "$Theme/MessagePopError-01.png", 30000);
         }
      else
         {
           CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) IsExistRubrique(rubName);
           if (pCMDI_Generic)
              {RUBREC_LIST::iterator it = pCMDI_Generic->Current_RubList_Iterator();
               if ( it == m_RubList.end()  )
                  { path=""; verbose = 1;
                    if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, rubName, tr("Defaut"), &path)==0)
                       { if (path[0]!=':') { verbose = 0;}
                         path = path.remove(":").stripWhiteSpace();
                         if (path.length() && path[0]!='*')
                            {path = G_pCApp->resolvePath(path);
                             if (QFile::exists( path  ))  OnGlossaireFileClicked(path, G_pCApp->RubNameToType(rubName) );
                             else  if (verbose)           G_pCApp->CouCou( tr("Fichier par défaut non trouvé ou non indiqué, lors ouverture de l'%1 par défaut").arg(rubName), "$Theme/MessagePopError-01.png", 30000);
                            }
                       }
                     else
                       {G_pCApp->CouCou( tr("Erreur : mention \"%1\" non trouvée dans le fichier de paramètres, lors de l'ouverture du fichier initial").arg(rubName), "$Theme/MessagePopError-01.png", 30000);
                       } //if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, rubName, tr("Defaut"), &path)==0)
                  } //if ( it == m_RubList.end()  )
              } //if (pCMDI_Generic)
          }  //for (int i=0; i<G_pCApp->m_ListRubName.count(); ++i)
     }
}

//------------------------------ Slot_OnGlossaireLocalise ------------------------
void DrTux::Slot_OnGlossaireLocalise()
{int     ret = QDialog::Accepted;
 QString dir = QFileDialog::getExistingDirectory ( G_pCApp->m_PathGlossaire, this, "OuvrirDlg", tr( " Rechercher le répertoire du Glossaire" ) );
 #if defined(Q_WS_WIN)
 ret = CGenTools::DialogGetString(this, tr("Valider le chemin du Glossaire"), dir);
 #endif

 if (dir.length() && ret == QDialog::Accepted)
    {if (G_pCApp->m_PathGlossaireIsLocal.length())
         CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam, "Glossaire", "Path",    dir, G_pCApp->m_PathGlossaireIsLocal);
     else
         CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam, "Glossaire", "Path",    dir);
     CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni,                       G_pCApp->m_DrTuxParam);
     QString path = CGestIni::AbsoluteToRelativePath(G_pCApp->m_PathAppli, dir);
     WRITE_USER_PARAM(&USER_PARAM, "Glossaire", "Path", path);  // zero = pas d'erreur
     UPDATE_USER_PARAM(&USER_PARAM, G_pCApp->m_User);
     OnChangePathGlossaire(dir);
    }
}

//------------------------------ OnChangePathGlossaire ------------------------
void DrTux::OnChangePathGlossaire(QString & dir)
{    G_pCApp->SetPathGlossaire(dir);
     if (m_pFormGlossaire)
        {QString     rubName = "";
         QWidgetList windows = m_pQWorkSpaceRub->windowList();

         for ( int u = 0; u < int(windows.count()); ++u )
             { CMDI_Generic *cmdi = (CMDI_Generic*) windows.at(u);    // on  tombe sur la CMDI
               if (cmdi) {rubName = cmdi->m_RubName; cmdi->On_DirectoryGlossaireChanged(G_pCApp->m_PathGlossaire+rubName, "?");}
             }
         CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) GetCurrentRubrique(&rubName);
         if ( pCMDI_Generic )
            { m_pFormGlossaire->SetCurrentPath(pCMDI_Generic->GetCurrentMaskDirectory(), "?");  //  ? force m_DefaultMaskDirectory=m_curPath
            }
         if (m_pC_Organiseur) m_pC_Organiseur->initialisePathGlossaire(G_pCApp->m_PathGlossaire+"Vigie", G_pCApp->m_PathGlossaire+"Vigie");
        }
}

//--------------------------------------- AlertVerrou --------------------------------
int  DrTux::AlertVerrou(const QString &userHostName)
{   /*
    switch( QMessageBox::information( this, tr(PROG_NAME" : OUVERTURE CONFLICTUELLE"),
                                             tr ( " <b><u>ATTENTION</b></u> ! Ce dossier <b>")            + G_pCApp->m_DossNom     + " " + G_pCApp->m_DossPrenom  +
                                             tr(  "</b> est déjà ouvert sur le poste suivant : <b>" )   + userHostName  +
                                             tr(  "</b><br> Faut-il : <br>"
                                                  "&nbsp;&nbsp;&nbsp;&nbsp; °  <b>L'ouvrir en <u>lecture</u> seule</b> (recommandé) afin de laisser "
                                                  "l'utilisateur actuel finir les modifications sur ce dossier<br> "
                                                  "&nbsp;&nbsp;&nbsp;&nbsp; ° <b>L'ouvrir en <u>écriture</u></b> (non recommandé) et alors "
                                                  "ce sont les modifications du dernier utilisateur à enregistrer le dossier "
                                                  "qui seront retenues, celles de l'autre (peut-être vous) seront perdues.<br>") ,
                                                  //"&nbsp;&nbsp;&nbsp;&nbsp; ° <b>Quitter pour <u>Annuler</u></b> l'opération en cours" ) ,
                                             tr("&Lecture"), tr("Écri&ture"), 0 / *tr("Annuler")* /,
                                             0, 1 ) )
            {case 0:
                   if (G_pCApp->m_TimerVerrouDelay && m_pTimerVerrou==0)
                      {m_pTimerVerrou = new QTimer(this);
                       if (m_pTimerVerrou!=0) connect( m_pTimerVerrou, SIGNAL(timeout()), this, SLOT(Slot_TestVerrou()) );
                      }
                   if (m_pTimerVerrou==0)    return 0;
                   m_pTimerVerrou->start(G_pCApp->m_TimerVerrouDelay, FALSE);
                   return 0;       // lecture seule
             case 1:
                   ForceWritting();
                   return 0;       // ecriture
             case 2:
             default:                                    // just for sanity
                   return 1;           // quitter
            }
    */
 int ret = QMessageBox::information( this, tr(PROG_NAME" : OUVERTURE CONFLICTUELLE"),
                                             tr ( " <b><u>ATTENTION</b></u> ! Ce dossier <b>")            + G_pCApp->m_DossNom     + " " + G_pCApp->m_DossPrenom  +
                                             tr(  "</b> est déjà ouvert sur le poste suivant : <b>" )   + userHostName  +
                                             tr(  "</b><br> Faut-il : <br>"
                                                  "&nbsp;&nbsp;&nbsp;&nbsp; °  <b> Il ne sera ouvert qu'en <u>lecture</u> seule</b> afin de laisser "
                                                  "l'utilisateur actuel finir les modifications sur ce dossier.<br> "
                                                  "Dès fermeture de ce dossier par cet utilisateur il sera disponible en écriture sur<br>"
                                                  "votre poste de travail <br>") ,
                                                  //"&nbsp;&nbsp;&nbsp;&nbsp; ° <b>Quitter pour <u>Annuler</u></b> l'opération en cours" ) ,
                                             tr("&Accéder en lecture"), tr("&Quiter"), 0 /*tr("Annuler")*/,
                                             0, 1 );
             if (ret==1) return 1;
             if (G_pCApp->m_TimerVerrouDelay && m_pTimerVerrou==0)
                 {m_pTimerVerrou = new QTimer(this);
                  if (m_pTimerVerrou!=0) connect( m_pTimerVerrou, SIGNAL(timeout()), this, SLOT(Slot_TestVerrou()) );
                 }
             if (m_pTimerVerrou==0)    return 0;
             m_pTimerVerrou->start(G_pCApp->m_TimerVerrouDelay, FALSE);
                                       return 0;       // lecture seule
}

//------------------------------------------ Slot_TestVerrou ----------------------------
/*! \brief teste le dossier pour savoir s'il est verrouillé ou non
*/
void DrTux::Slot_TestVerrou()
{  //....................... verrouller lire et initialiser la liste des rubriques ..........................
    QString usedBy;
    int isModifiable = G_pCApp->m_pCMoteurBase->VerrouilleDossier (G_pCApp->m_NumGUID, G_pCApp->m_pCMoteurBase->m_UserName+"@"+ G_pCApp->m_pCMoteurBase->m_HostName , usedBy);
    if (isModifiable==0) return;
    if (m_pTimerVerrou)  delete m_pTimerVerrou;
    m_pTimerVerrou = 0;
    //................... noter le verrou ...............................
    m_IsModifiable = isModifiable;
    //................... noter le verrou ...............................
    SetModifiableState(m_IsModifiable);
    //.............. mettre à jour la liste des rubriques dessus ................
    RubListMakeWhithNewDoss();
    //............. la retransmettre aux fenetres filles .......................
    // ne pas utiliser: num_GUID,  id_doss, dossNom, dossPrenom
    // car peut provenir de: FormRubIdent lors de la creation d'un nouveau dossier
    // et: Sign_OnIdentChange (y est aussi connecté) donc reference circulaire... bug...
    emit Sign_OnIdentChange(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom );
}

//------------------------------------------ ForceWritting ----------------------------
void DrTux::ForceWritting()
{m_IsModifiable = 1;
}

//------------------------------------------ SetModifiableState ----------------------------
void DrTux::SetModifiableState(int state)
{if (m_pCMDI_Observation)     m_pCMDI_Observation->SetModifiableState(state);
 if (m_pCMDI_Prescription)    m_pCMDI_Prescription->SetModifiableState(state);
 if (m_pCMDI_Terrain)         m_pCMDI_Terrain->SetModifiableState(state);
 if (m_pCMDI_Ident)           m_pCMDI_Ident->SetModifiableState(state);
 if (m_pForm_Menu)            m_pForm_Menu->setModifiableState(state);
 //................ rubriques automatiques .......................
 QMap<QString, CMDI_Observation*>::Iterator it;
 for ( it = G_pCApp->m_mapNameRubPtr.begin(); it != G_pCApp->m_mapNameRubPtr.end(); ++it )
     {  CMDI_Observation *pCMDI_Observation = it.data();
        if (pCMDI_Observation) pCMDI_Observation->SetModifiableState(state);
     }

 if (m_pActionListPatDelete)  m_pActionListPatDelete->setEnabled(state);
 if (m_pActionDossierSave)    m_pActionDossierSave->setEnabled(state);

}

//------------------------------ setZoom1 setZoom2 setZoom3 setZoom4 setZoom5 --------------------------------------------------------
void DrTux::setZoom8()  {SetZoom(8);}
void DrTux::setZoom9()  {SetZoom(9);}
void DrTux::setZoom10() {SetZoom(10);}
void DrTux::setZoom11() {SetZoom(11);}
void DrTux::setZoom12() {SetZoom(12);}
void DrTux::setZoom14() {SetZoom(14);}
void DrTux::setZoom16() {SetZoom(16);}
void DrTux::setZoom18() {SetZoom(18);}
void DrTux::setZoom20() {SetZoom(20);}

void DrTux::SetZoom(int zoom)
{    QString pathParam(G_pCApp->m_PathAppli + "drtux.ini");
     QString rubName;
     int     rubType;
     /*QWidget *pQWidget = */GetCurrentRubrique(&rubName , &rubType );
     switch(rubType)
        {case TYP_PRESCRIPTION:
         case TYP_ORDONNANCE:
         case TYP_ORDO_CALC:
         case TYP_COURRIER:
         case TYP_CERTIFICAT:
              CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam, rubName, "Zoom", QString::number(zoom));
              CGestIni::Param_UpdateToDisk(pathParam, G_pCApp->m_DrTuxParam);
              m_pCMDI_Prescription->SetZoom(zoom);
              break;
         case TYP_OBSERVATION:
              CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam, rubName, "Zoom", QString::number(zoom));
              CGestIni::Param_UpdateToDisk(pathParam, G_pCApp->m_DrTuxParam);
              m_pCMDI_Observation->SetZoom(zoom);
              break;
        }
}

//------------------------------ Slot_Renouveler_TTT_Fond -----------------------------------------------------------------
void DrTux::Slot_Renouveler_TTT_Fond()
{emit Sign_DrTux_Renouveler_TTT_Fond();
}

//------------------------------ Slot_PutPrescriptionInTerrain -----------------------------------------------------------------
void DrTux::Slot_PutPrescriptionInTerrain(RUBREC_LIST::iterator it)
{emit Sign_DrTux_PutPrescriptionInTerrain(it);
}

//------------------------------ Slot_NoteToBeSynchronized ------------------------------------------------
/*! \brief slot appelé par CMDI_Ident ou Dock_Menu par le biais du signal Sign_NoteToBeSynchronized(int, const char*) pour synchroniser les notes entre elles.
*/
void DrTux::Slot_NoteToBeSynchronized(int emetteur, const char* txt)
{ // Besoin de synchroniser ?
  if (!m_pCMDI_Ident) return;
  if (!m_pForm_Menu) return;

  // Déconnecter tout le monde
  m_pCMDI_Ident->m_pFormRubIdent->disconnect( SIGNAL(Sign_NoteToBeSynchronized(int, const char*) ) );
  m_pForm_Menu->disconnect( SIGNAL(Sign_NoteToBeSynchronized(int, const char*) ) );

  // Synchroniser la note
  switch (emetteur)
  {case 0 : // C'est Dock_Menu l'émetteur du signal
             if (m_pCMDI_Ident)
             { if (m_pCMDI_Ident->m_pFormRubIdent)
               {
                 m_pCMDI_Ident->m_pFormRubIdent->synchronizeNoteWith( txt );
               }
             }
             break;
    case 1 : // C'est CMDI_Ident l'émetteur
             if (m_pForm_Menu)
             { m_pForm_Menu->synchronizeNoteWith( txt );
             }
             break;
  }

  // Reconnecter tout le monde
  connect( m_pCMDI_Ident->m_pFormRubIdent,SIGNAL( Sign_NoteToBeSynchronized(int, const char* )  ),
           this,                          SLOT  ( Slot_NoteToBeSynchronized(int, const char* )  ) );
  connect (m_pForm_Menu,   SIGNAL ( Sign_NoteToBeSynchronized(int, const char*) ),
           this,           SLOT   ( Slot_NoteToBeSynchronized(int, const char*) )    );

}


//------------------------------ ExeAnotherDrTux -------------------------------------------------------------
void DrTux::OnDrTuxExeAnother(QString num_GUID,  QString id_doss,
                              QString dossNom ,  QString dossPrenom)
{    QProcess*  proc = new QProcess( this );
     if (proc==0) return;
     QString    pathExe = CGestIni::Construct_Name_Exe("drtux", QFileInfo (G_pCApp->argv()[0]).dirPath (true));
     pathExe = QDir::cleanDirPath(pathExe);
     proc->addArgument( pathExe );
     proc->addArgument( G_pCApp->m_User );      // Nom de l'utilisateur
     proc->addArgument( G_pCApp->m_SignUser ); // Nom de l'utilisateur de confiance
     proc->addArgument( num_GUID );             // GUID dossier
     proc->addArgument( id_doss );              // numero PrimKey
     proc->addArgument( dossNom.replace("'","~@~") );              // Nom assuré
     proc->addArgument( dossPrenom.replace("'","~@~") );           // Prenom assuré
     proc->addArgument( G_pCApp->m_PathDrTuxIni );           // fichier ini
     proc->addArgument( G_pCApp->m_PathCfg_Base );           // fichier baseCfg

     //if ( !proc->launch ( QString ("/home/ro/Developpement/medin_tux_desk/bin/drtux") ))
     if ( !proc->start() )
        {//text_Label_Status->setText(qstr.sprintf("ListView_Patient_doubleClicked(): Echec de l'ouverture du process"));
        }
     delete proc;

    /*
    QString pathExe  = CGestIni::Construct_Name_Exe("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
    pathExe = QDir::cleanDirPath(pathExe);
    proc.addArgument( pathExe );
    proc.addArgument( user);                                        // login de l'utilisateur
    proc.addArgument( sign_user );                                  // login du signataire
    proc.addArgument( guid );                                       // GUID dossier
    proc.addArgument( primKey);                                     // numero PrimKey
    proc.addArgument( nom.replace("'","~@~") );                                        // Nom assuré
    proc.addArgument( prenom.replace("'","~@~") );                                     // Prenom assuré
    / *
    QStringList arg_lst = proc.arguments ();
    for ( QStringList::Iterator it = arg_lst.begin(); it != arg_lst.end(); ++it )
        { qDebug( *it );
        }
    * /
    QString str = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true), "");
    proc.addArgument(str );
    // fichier ini
    str = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true), "DataBase.cfg");
    proc.addArgument( str );
    // fichier cfgBase
    if ( !proc.start() )
       {qDebug("ListView_Patient_doubleClicked(): Echec de l'ouverture du process");
       }
   */
}

//------------------------------ AfficheRubriques -----------------------------------------------------------------
/*! \brief Fonction appellée par le constructeur de la classe pour préparer les fenêtres CMDI dans le workspace central selon les paramètres sauvegardés dans le fichier ini */
void DrTux::AfficheRubriques()
{QString val1;

 for ( QStringList::Iterator it = G_pCApp->m_ListRubName.begin(); it != G_pCApp->m_ListRubName.end(); ++it )
     {CGestIni::Param_ReadParam(USER_PARAM, *it, tr("Montrer"), &val1);
      if (val1==tr("Normal")) OnActiverOrCreateRubrique(*it);
     }
}

//------------------------------ SetTitle --------------------------
void DrTux::SetTitle()
{//................. titre nouvelle identité .........................................................................................
 if (G_pCApp->m_NumGUID != "")
     setCaption(   G_pCApp->m_DossNom + " " + G_pCApp->m_DossPrenom + "  " + tr(PROG_NAME" Dossier N°: ") + G_pCApp->m_NumGUID + "=" + G_pCApp->m_ID_Doss + "  User: " + G_pCApp->m_User + "::" + G_pCApp->m_SignUser);
 else
     setCaption(  tr(PROG_NAME) + "  User: " + G_pCApp->m_User + "::" + G_pCApp->m_SignUser );
}


//------------------------------ OnDrTuxUserChange -----------------------------------------------------------
void DrTux::OnDrTuxUserChange()
{
}

//------------------------------ OnDrTuxPatientListMustBeUpdated ----------------------------------------------
/*! \brief Slot appelé en cas de modification des données identitaires du patient et se charge de retransmettre à la liste des patient concerné par ça, l'information afin qu'il adapte son contenu.
 * RAPPEL:
 * La rubrique "Identité" envoie ces messages lors création d'un nouveau dossier...........................
 *     emit  Sign_PatientListMustBeUpdated( numGUID,        G_pCApp->m_ID_Doss,
 *                                          nom,            prenom
 *                                        );
 *     emit  Sign_IdentChange( numGUID,        G_pCApp->m_ID_Doss,
 *                             nom,            prenom
 *                           );
 * La rubrique "Identité" envoie ces messages lors modification de l'identité  d'un dossier ..............
 *     emit  Sign_IdentModified( numGUID,        G_pCApp->m_ID_Doss,
 *                               nom,            prenom
 *                             );
*/
void DrTux::OnDrTuxPatientListMustBeUpdated(const char* num_GUID,  const char* id_doss,
                                            const char* dossNom ,  const char* dossPrenom )
{// Mise à jour de Dock_Menu
 if (m_pForm_Menu) m_pForm_Menu->Slot_ChangeIdentity(num_GUID, id_doss, dossNom, dossPrenom, &m_RubList);
 if (m_pCMDI_ChoixPatient == 0) return;
 m_pCMDI_ChoixPatient->m_pFormRubChoixPatient->reinitList(num_GUID, id_doss, dossNom, dossPrenom);
 QString tmp = num_GUID;
 if (tmp != "") SetDefaultDoc();
}

//------------------------------ OnDrTuxIdentModified ---------------------------------------------------------------
/*! \brief Slot appelé en cas de modification des données identitaires du patient et se charge de retransmettre à toutes les fenetres concernées par ça, l'information afin qu'elles adaptent leur contenu.
*/
void DrTux::OnDrTuxIdentModified(const char* /*num_GUID*/,  const char* /*id_doss*/,
                                 const char* dossNom ,  const char* dossPrenom )
{G_pCApp->m_DossNom    = dossNom;
 G_pCApp->m_DossPrenom = dossPrenom;
 SetTitle();
 SaveLastSessionInfo();
 OnDrTuxPatientListMustBeUpdated(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom );
 // Mise à jour du Dock_Menu
 if (m_pForm_Menu) m_pForm_Menu->Slot_ChangeIdentity( G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom, &m_RubList);
}

//------------------------------ OnDrTuxIdentChange ---------------------------------------------------------------
/*! \brief Slot appelé en cas de changement de patient et se charge de retransmettre à toutes les fenetres concernées par ça, l'information afin qu'elles adaptent leur contenu.
*/
void DrTux::OnDrTuxIdentChange(const char* num_GUID,  const char* id_doss,
                               const char* dossNom ,  const char* dossPrenom )
{
 if (SauverDossierAvantNouvelleAction() == 0) return;

 //................... liberer l'ancien verrou ...............................
 if (m_IsModifiable) G_pCApp->m_pCMoteurBase->DeVerrouilleDossier (G_pCApp->m_NumGUID);

 //............. noter la nouvelle identité ..................................
 G_pCApp->m_NumGUID    = num_GUID;
 G_pCApp->m_ID_Doss    = id_doss;
 G_pCApp->m_DossNom    = dossNom;
 G_pCApp->m_DossPrenom = dossPrenom;
 SetTitle();
 //................... verrouiller le dossier .................................
 QString usedBy;
 m_IsModifiable = G_pCApp->m_pCMoteurBase->VerrouilleDossier (G_pCApp->m_NumGUID, G_pCApp->m_pCMoteurBase->m_UserName+"@"+ G_pCApp->m_pCMoteurBase->m_HostName , usedBy);
 if (m_IsModifiable==0) AlertVerrou(usedBy);
 SetModifiableState(m_IsModifiable);
 //.............. mettre à jour la liste des rubriques dessus ................
 RubListMakeWhithNewDoss();
  //............. la retransmettre aux fenetres filles .......................
  // ne pas utiliser: num_GUID,  id_doss, dossNom, dossPrenom
  // car peut provenir de: FormRubIdent lors de la creation d'un nouveau dossier
  // et: Sign_OnIdentChange (y est aussi connecté) donc reference circulaire... bug...

 emit Sign_OnIdentChange(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom );
 // Mise à jour du Dock_Menu (qui reste prioritaire pour la mise à jour des ATCD)
 if (m_pForm_Menu) m_pForm_Menu->Slot_ChangeIdentity( G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom, &m_RubList);

 SaveLastSessionInfo();
}

//------------------------------ SaveLastSessionInfo --------------------------
/*! \brief Sauvegarde les informations de la session dans le fichier INI. */
void DrTux::SaveLastSessionInfo()
{//.............. sauver dernier patient ....................................
 CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam, "Derniere Session", "Utilisateur", G_pCApp->m_User, G_pCApp->m_SignUser);
 CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam, "Derniere Session", "Patient",     G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom, G_pCApp->m_DossPrenom);
 CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, G_pCApp->m_DrTuxParam);
}

//------------------------------ OnDrTuxIdentCurrentDelete ---------------------------------------------------------------
void DrTux::OnDrTuxIdentCurrentDelete()
{OnDrTuxIdentDelete(G_pCApp->m_NumGUID,  G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom );
}

//------------------------------ OnDrTuxIdentDelete ---------------------------------------------------------------
/*! \brief Slot est appelé pour effacer un dossier. */
void DrTux::OnDrTuxIdentDelete(const char* num_GUID,  const char* id_doss,
                               const char* dossNom ,  const char* dossPrenom )
{if (num_GUID==0   ||id_doss==0)    return;
 if (num_GUID[0]==0||id_doss[0]==0) return;
 //............. noter la nouvelle identité ..................................

 switch( QMessageBox::warning (    this, tr(PROG_NAME" Effacement d'un dossier patient :"),
                                   tr ( " ATTENTION !\r\n      VOUS VOUS APPRÊTEZ À EFFACER LE DOSSIER : \r\n      ")     +
                                   dossNom + " " + dossPrenom  +
                                   tr(  "\r\n      CETTE ACTION EST IRRÉVERSIBLE\r\n"
                                   "      ET LES DONNÉES DU DOSSIER NE SERONT\r\n"
                                   "      PAS RÉCUPÉRABLES PAR LA SUITE") ,
                                   tr("&Effacer"), tr("&NON"), 0,
                                             1, 1 )
         )
         {case 1: return ;
         }
 //.................. effacer la note ...................................
 QString pk = G_pCApp->m_pCMoteurBase->PatientNoteIsThisExist(id_doss);
 G_pCApp->m_pCMoteurBase->PatientNoteDelete(pk);
 G_pCApp->m_pCMoteurBase->PatientIntervenantsAllDelete( id_doss);
 //.................. effacer le dossier ...................................
 G_pCApp->m_pCMoteurBase->DossIdentDelete(&m_RubList, num_GUID, id_doss);
 //.................. reinitialiser liste des rubriques ....................
 m_RubList.clear();                                        // effacer la liste elle meme
 m_EvnList.clear();

 //................... liberer l'ancien verrou ...............................
 if (m_IsModifiable) G_pCApp->m_pCMoteurBase->DeVerrouilleDossier (G_pCApp->m_NumGUID);

 G_pCApp->m_NumGUID    = "";
 G_pCApp->m_ID_Doss    = "";
 G_pCApp->m_DossNom    = "";
 G_pCApp->m_DossPrenom = "";
 //................ titre ...................................................
 SetTitle();
  //............. la retransmettre aux fenetres filles .......................
  // ne pas utiliser: num_GUID,  id_doss, dossNom, dossPrenom
  // car peut provenir de: FormRubIdent lors de la creation d'un nouveau dossier
  // et: Sign_OnIdentChange (y est aussi connecté) donc reference circulaire... bug... etc...
  // emit Sign_OnIdentChange(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom );
  emit Sign_OnIdentChange(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom );

  //.............. sauver dernier patient ....................................
 SaveLastSessionInfo();
 OnDrTuxPatientListMustBeUpdated(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss,  G_pCApp->m_DossNom,  G_pCApp->m_DossPrenom );
}

//--------------------------------- OnCreateNewIdent ------------------------------
/*! \brief Fait apparaitre un dialogue vierge de la rubrique identité, qui crée si besoin le nouveau dossier et renvoie à DrTux les messages de mise à jour voir: OnDrTuxPatientListMustBeUpdated().
*/
void DrTux::OnCreateNewIdent()
{if (SauverDossierAvantNouvelleAction() == 0) return;
 OnActiverOrCreateRubrique(CMDI_Ident::S_GetRubName());
 if (m_pCMDI_Ident == 0)                      return;
 m_pCMDI_Ident->m_pFormRubIdent->SetData(NEW_IDENTITE);
}

//------------------------------ updateAllRubriquesEditorsInRubList ---------------------------------------
/*! \brief toutes les données en cours d'édition sont replacées dans la structure de liste , prêtes à etre enregistrées */
void DrTux::updateAllRubriquesEditorsInRubList()
{QWidgetList windowsList       = m_pQWorkSpaceRub->windowList();
 int id  = -1;
 int  i  =  0;

 // Dock_Menu a-t-il qq chose à sauvegarder ?
 if (m_pForm_Menu) m_pForm_Menu->checkItemToSave();

 //.............. mettre à jour la liste des rubriques avec le contenu des éditeurs ...................
 for ( i =  0; i < int(windowsList.count()); ++i )
     {CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) windowsList.at(i);
      if ( (id = pCMDI_Generic->GetCurrent_RubList_ID()) !=-1)  pCMDI_Generic->IfModified_SaveInRubList();
     }
}
//------------------------------ OnDrTuxSaveRubList --------------------------------------------------------
/*! \brief Slot appelé en cas de de demande de sauvegarde d'une rubrique. */
void DrTux::OnDrTuxSaveRubList()
{QString                pk                = "";
 int                    i                 =  0;
 CPrtQListBoxItem      *pCPrtQListBoxItem =  0;
 QWidgetList            windowsList       =  m_pQWorkSpaceRub->windowList();
 RUBREC_LIST::iterator           it;
 QMap<int,QString>::Iterator     mit;
 QMap <int, QString>             map_activeRubTypeAndPk;

 // Dock_Menu a-t-il qq chose à sauvegarder ?
 if (m_pForm_Menu) m_pForm_Menu->checkItemToSave();

 //.............. mettre à jour la liste des rubriques avec le contenu des éditeurs ...................
 for ( i =  0; i < int(windowsList.count()); ++i )
     { CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) windowsList.at(i);
       pCPrtQListBoxItem           = pCMDI_Generic->GetCurrentDocItem();
       if ( pCPrtQListBoxItem )          // si c'est un enregistrement de rubrique active
       // if ( (id = pCMDI_Generic->GetCurrent_RubList_ID()) !=-1)  
          { //............. on mape le type de la rubrique avec le pk des rubriques actives .....................
            //              les Pk peuvent etre provisoires et seront changes lors de l'enregistement par les vrais
            if (pCPrtQListBoxItem) map_activeRubTypeAndPk[pCMDI_Generic->GetType()] = pCPrtQListBoxItem->GetUser();  // OUI c'est le boxon le Pk est dans le user de CPrtQListBoxItem.
            pCMDI_Generic->IfModified_SaveInRubList();
          }
     }
 //............................ mettre à jour la base en fonction de la liste .......................................
 //                             toutes les situations provisoires seront mise a jour sur les definitives
 G_pCApp->m_pCMoteurBase->RubListSave(&m_RubList, &m_EvnList,  G_pCApp->m_NumGUID, map_activeRubTypeAndPk, G_pCApp->m_IsNomadeActif);
 //............... reinitialiser toute la liste .................................................
 // effacer la liste elle meme
 m_RubList.clear();
 m_EvnList.clear();

 // lire recharger et reinitialiser la liste des rubriques
 G_pCApp->m_pCMoteurBase->initRubriquesList( &m_RubList,   G_pCApp->m_NumGUID);
 G_pCApp->m_pCMoteurBase->Evnmt_InitList(    &m_EvnList,   G_pCApp->m_ID_Doss);

 //.............. réinitialiser les affichages des rubriques  ...................
 for ( i =  0; i < int(windowsList.count()); ++i )
     { QString pkToActivate                        =  "";
       CMDI_Generic *pCMDI_Generic                 =  (CMDI_Generic*) windowsList.at(i);
       //....... chercher si le Pk actif de cette rubrique dans la liste construite..............................
       //        et relevee plus haut de la map liant le type de rubrique avec le pk de l'enregistrement actif 
       mit                                         =  map_activeRubTypeAndPk.find(pCMDI_Generic->GetType());
       //....... un record est identifie comme actif si : ....................
       //        son type fait partie des types de rubriques actives
       //        le Pk (provisoire ou pas) associe a cette rubrique active
       //        est le meme que le record
       if ( mit  != map_activeRubTypeAndPk.end() )
          { pk = mit.data ();
            if (pk!="-1") pkToActivate = pk;       // si ne satisfait pas aux conditions on ne retient pas ce record comme celui actif dans une rubrique
          }
       pCMDI_Generic->reinitComboBoxWithRubList(&m_RubList, pkToActivate);
     }
  if(m_pC_Organiseur) m_pC_Organiseur->makeListeMonitor();
  // Informer Dock_Menu du changement
  if (m_pForm_Menu) m_pForm_Menu->getRubListAndUpdateView( &m_RubList );
  /*
  if (G_pCApp->m_pCMoteurBase->m_Debug)
     {G_pCApp->m_pCMoteurBase->m_debugStr += "----------------- APRES RELECTURE : vidage RUBREC_LIST ----------------\n";
      G_pCApp->m_pCMoteurBase->m_debugStr += m_RubList.serialize(0) + "\n";
      G_pCApp->m_pCMoteurBase->m_debugStr += "----------------- APRES RELECTURE : vidage EVNT_LIST ------------------\n";
      G_pCApp->m_pCMoteurBase->m_debugStr += m_EvnList.serialize(0) + "\n";
     }
  CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"RecordLog.log", G_pCApp->m_pCMoteurBase->m_debugStr);
  */
}
//------------------------------ initAllRubriquesComboWithRubList ------------------------------------------------------------
// si pour une raison ou une autre il faut mettre a jour la liste des combo box de chaque fenetre
// exemple la date d'une rubrique est modifiee par une macro donc ll'ordre
//
void DrTux::initAllRubriquesComboWithRubList()
{ //.............. réinitialiser les affichages des rubriques  ...................
 QString                pk           = "";
 int                    i            =  0;
 QWidgetList   windowsList           =  m_pQWorkSpaceRub->windowList();
 CPrtQListBoxItem *pCPrtQListBoxItem =  0;
 QMap<int,QString>::Iterator            mit;
 QMap <int, QString>                    map_activeRubTypeAndPk;
 
 // Dock_Menu a-t-il qq chose à sauvegarder ?
 if (m_pForm_Menu) m_pForm_Menu->checkItemToSave();

 //.............. mettre à jour la liste des rubriques avec le contenu des éditeurs ...................
 for ( i =  0; i < int(windowsList.count()); ++i )
     { CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) windowsList.at(i);
       pCPrtQListBoxItem           = pCMDI_Generic->GetCurrentDocItem();
       if ( pCPrtQListBoxItem )          // si c'est un enregistrement de rubrique active
       // if ( (id = pCMDI_Generic->GetCurrent_RubList_ID()) !=-1)  
          { //............. on mape le type de la rubrique avec le pk des rubriques actives .....................
            //              les Pk peuvent etre provisoires et seront changes lors de l'enregistement par les vrais
            if (pCPrtQListBoxItem) map_activeRubTypeAndPk[pCMDI_Generic->GetType()] = pCPrtQListBoxItem->GetUser();  // OUI c'est le boxon le Pk est dans le user de CPrtQListBoxItem.
            pCMDI_Generic->IfModified_SaveInRubList();
          }
     }
 //.................... recharger les combo de chaque rubrique ..................................................
 //
 for ( i =  0; i < int(windowsList.count()); ++i )
     { QString pkToActivate                        =  "";
       CMDI_Generic *pCMDI_Generic                 =  (CMDI_Generic*) windowsList.at(i);
       //....... chercher si le Pk actif de cette rubrique dans la liste construite..............................
       //        et relevee plus haut de la map liant le type de rubrique avec le pk de l'enregistrement actif 
       mit                                         =  map_activeRubTypeAndPk.find(pCMDI_Generic->GetType());
       //....... un record est identifie comme actif si : ....................
       //        son type fait partie des type de rubriques actives
       //        le Pk (provisoire ou pas) associe a cette rubrique active
       //        est le meme que le record
       if ( mit  != map_activeRubTypeAndPk.end() )
          { pk = mit.data ();
            if (pk!="-1") pkToActivate = pk;       // si ne satisfait pas aux conditions on ne retient pas ce record comme celui actif dans une rubrique
          }
       pCMDI_Generic->reinitComboBoxWithRubList(&m_RubList, pkToActivate);
     }
}
//------------------------------ RubListMakeWhithNewDoss ------------------------------------------------------------
void DrTux::RubListMakeWhithNewDoss()
{m_RubList.clear();                                      // effacer la liste
 m_EvnList.clear();
 G_pCApp->m_pCMoteurBase->initRubriquesList( &m_RubList,   G_pCApp->m_NumGUID);   // lire et initialiser la liste des rubriques
 G_pCApp->m_pCMoteurBase->Evnmt_InitList(    &m_EvnList,   G_pCApp->m_ID_Doss);
 if(m_pC_Organiseur) m_pC_Organiseur->makeListeMonitorFromList();

}

//------------------------------ OnOngletRubriquesChanged ------------------------------------------------------------
/*! \brief reponse au signal: SIGNAL( currentChanged ( QWidget * ) de la barre d'onglets des rubriques afin d'activer la fenêtre CMDI correspondante.
 *  \param QWidget * pQwidget;  fenetre CMDI qui vient d'être activée.
*/
void DrTux::OnOngletRubriquesChanged(QWidget * pCMDI_Qwidget)
{OnActiverRubrique(pCMDI_Qwidget->name(), 0);
}

//------------------------------ OnActiverRubrique ------------------------------------------------------------
/*! \brief Réponse au signal:  SIGNAL(Sign_ActiverRubrique(const char*))) provenant des Onglets des rubriques afin d'activer la bonne fenetre CMDI lorsqu'un onglet a ete cliqué.
 *  Cette fonction emet elle meme un signal à chaque fenetre CMDI qui doivent s'activer, si c'est leur onglet qui est responsable du signal (reperable par le parametre: const char* rubName qui le texte de l'onglet.
 * \param const char* rubName: texte (celui visible) de l'onglet cliqué.
*/
void DrTux::OnActiverRubrique(const char* rubName, int mode /*=0*/)
{ // SetDefaultDoc();
  emit Sign_ActiverRubrique(rubName, mode);   // emettre signal aux rubriques filles
}

//------------------------------ OnActiverOrCreateRubrique ------------------------------------------------------------
/*! \brief Réponse au signal:  SIGNAL(Sign_ActiverOrCreateRubrique(const char*))) provenant des boutons de rubriques afin d'activer la bonne fenetre CMDI lorsqu'un onglet a ete cliqué, ou la creer si non présente.
 * Cette fonction émet elle meme un signal à chaque fenetre CMDI qui doivent s'activer, si c'est leur onglet qui est responsable du signal (reperable par le parametre: const char* rubName qui le texte de l'onglet.
 * \param rubName: const char* = texte (celui visible) de l'onglet cliqué.
*/
void DrTux::OnActiverOrCreateRubrique(const char* rubName, int mode)
{CMDI_Generic *pCMDI_Generic;
 if ( (pCMDI_Generic = (CMDI_Generic*) IsExistRubrique(rubName))==0)  // parse toutes les fenetres à la recherche de la rubrique
    {if (strncmp(rubName, CMDI_Prescription::S_GetRubName() , 5)==0)
        {CMDI_PrescriptionCreate ( G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User, rubName);
        }
     else if (strncmp(rubName, RUBNAME_OBSERVATION ,5)==0)
        {m_pCMDI_Observation  = CMDI_RubriqueCreate ( G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User,  rubName, "ob");
        }
     else if (strncmp(rubName,CMDI_ChoixPatient::S_GetRubName(),5)==0 && QFile::exists (  "/home/ro/MedinTuxRo.txt" ))
        {CMDI_ChoixPatientCreate(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User,   rubName);
        }
     else if (strncmp(rubName,CMDI_Ident::S_GetRubName(),5)==0)
        {CMDI_IdentCreate(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User,          rubName);
        }
     else if (strncmp(rubName,CMDI_Terrain::S_GetRubName(),5)==0)
        {CMDI_TerrainCreate(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User,        rubName);
        }
     else
        {QString prDroit = G_pCApp->m_mapNameRubInfos[rubName];
         //int        type = G_pCApp->m_mapNameRubType[rubName];
         CMDI_Observation *pCMDI_Rubrique  = CMDI_RubriqueCreate(G_pCApp->m_NumGUID, G_pCApp->m_ID_Doss, G_pCApp->m_DossNom , G_pCApp->m_DossPrenom, G_pCApp->m_User, rubName, prDroit);
         if (pCMDI_Rubrique==0) return;
         G_pCApp->m_mapNameRubPtr[rubName] = pCMDI_Rubrique;
         //............... si pas de document en placer un ........................
         RUBREC_LIST::iterator it = pCMDI_Rubrique->Current_RubList_Iterator();
         if ( it == m_RubList.end()  )
            { QString path = CGestIni::Param_ReadUniqueParam( G_pCApp->m_DrTuxParam, rubName, "Defaut");
              if (path.length())
                 { int verbose  = 1;
                   if (path[0] !=':' ) verbose = 0;
                   path = path.remove(":").stripWhiteSpace();
                   if ( path.length() && path[0] !='*' )
                      { path = G_pCApp->resolvePath(path);
                        if (QFile::exists( path  ))  QTimer::singleShot ( 10, this, SLOT(SetDefaultDoc()) );
                        else  if (verbose)           G_pCApp->CouCou( tr("Fichier par défaut non trouvé ou non indiqué, lors ouverture de l'%1 par défaut").arg(rubName), "$Theme/MessagePopError-01.png", 30000);
                      }
                 }
              else
                 { G_pCApp->CouCou( tr("Erreur : mention \"%1\" non trouvée dans le fichier de paramètres, lors de l'ouverture du fichier initial").arg(rubName), "$Theme/MessagePopError-01.png", 30000);
                 } //if (C
            }
        }
    }

 if (pCMDI_Generic && m_pFormOngletRubrique)
    {
     QString tabName = m_pFormOngletRubrique->tabWidget->label (m_pFormOngletRubrique->tabWidget->currentPageIndex());
     if (tabName==rubName) return;
     int id = getCurrentPageID_ByTitle(m_pFormOngletRubrique->tabWidget, rubName);
     if (id != -1 )
        { // action eventuelle sur le tabWidget
        }

    }
 OnActiverRubrique(rubName, mode);
}

//------------------------------------------- getCurrentPageID_ByTitle -------------------------
int DrTux::getCurrentPageID_ByTitle(QTabWidget *pQTabWidget, const QString &title)
{int nb = pQTabWidget->count();
 for ( int i=0; i<nb; ++i)
     {if (pQTabWidget->label(i) == title) return i;
     }
 return -1;
}

//------------------------------ OnDeleteRubrique ------------------------------------------------------------
/*! \brief Réponse au signal: Sign_RubIsDeleted envoyé par le destructeur d'une rubrique. Le pointeur de la rubrique effacée est mis à zéro par son destructeur (en passant par G_pCApp->m_pDrTux->m_pCMDI_xxxxxx = 0 ).
 * Fonction un peu obsolète toujours utilisée par OnActiverOrCreateRubrique(), savePos().
*/
void  DrTux::OnDeleteRubrique(const char* rubName)
{  if (m_pFormOngletRubrique) m_pFormOngletRubrique->removePage(rubName);
   //................ rubriques automatiques .......................
   QMap<QString, CMDI_Observation*>::Iterator it;
   for ( it = G_pCApp->m_mapNameRubPtr.begin(); it != G_pCApp->m_mapNameRubPtr.end(); ++it )
       {  //CMDI_Observation *pCMDI_Observation = it.data();
          QString                        name = it.key();
          if ( name == rubName) {G_pCApp->m_mapNameRubPtr.remove(it); break;}   // break indispensable car liste peut plus etre iteree
       }
}

//------------------------------ IsExistRubrique -------------------------------------------------------------
/*! \brief Teste si une des fenêtres rubriques dont le nom est fourni en entrée est ouverte dans le WorkSpace des rubriques.
 *  \param rubName doit etre: "Observation" "Prescription" "Choix patient" "Identité" "Terrain"
 * \return pointeur sur la rubrique si elle existe ou zero
*/
QWidget * DrTux::IsExistRubrique(const char* rubName)
{

 QWidgetList windows = m_pQWorkSpaceRub->windowList(QWorkspace::CreationOrder);
 if ( windows.count() == 0 ) return 0;
 for ( int i = 0; i < int(windows.count()); ++i )
     {QWidget *pQwdgRub = windows.at(i);
      QString nom       =  pQwdgRub->name();  // pWdgRub->name() = Observation" "Identé"
      if ( rubName == nom) return pQwdgRub;
    }
 return 0;

}

//---------------------------------------------- IsExistRubrique ---------------------------------------------------
/*! \brief teste si une des fenêtres rubriques dont le type est fourni en entrée est ouverte dans le WorkSpace des rubriques.
 *  \return pointeur sur la rubrique si elle existe ou zero.
*/
CMDI_Generic * DrTux::IsExistRubrique(int rubTyp)
{ return (CMDI_Generic *) IsExistRubrique(G_pCApp->TypeToRubName( rubTyp ));
}

//------------------------------ OnGlossaireFileClicked ----------------------
/*! \brief fonction appelée en reponse au signal: Sign_FileSelected(const char *path, int typ) emis par le glosssaire lors d'un click sur un fichier il faut creer un nouveau document avec le fichier selectionne.
 *  \sa DrTux::AddNewRecordToRubrique et CMDI_XXXXX::AddNewRecordToRubrique; Dock_Menu::Sign_GlossaireFileSelected
*/
CRubRecord*    DrTux::OnGlossaireFileClicked(const char *path, int typ, int noInquire /*=0*/)
{QString                    strDst       = "";
 //..................... charger le texte du masque ........................................
 if (typ != CMDI_Terrain::S_GetType())
     CGenTools::FileToQString (path,  strDst, 2, 1 ); // 0-> ne pas remplacer les \n par des <br />
                                                      // 1-> inclure le header HTML
                                                      // 2-> remplacer les \n par des <br />  si extension est .TXT ou .txt
 else
     CGenTools::FileToQString (path,  strDst, 0, 0 );
 return AddNewRecordToRubrique(strDst,  typ, path,0,0,0,noInquire);

}


//------------------------------ OnGlossaireFileToEdit ----------------------
/*! \brief Fonction appelée en reponse au signal: Sign_FileToEdit(QString)   emis par le glosssaire lors d'un clic sur le bouton d'édition d'un fichier il faut appeler l'editeur de textes pour editer le fichier
 *  action "Créer"
 *  action "Ajouter"
 *  action "Modifier"
 *  action "Détruire"
*/
void DrTux::OnGlossaireFileToEdit(QString pathDoc, QString action, int typ)
 {
  if (action==tr("Modifier"))
     {QString pathExe, pathListe;
      pathExe = CGestIni::Construct_Name_Exe("QLightPad",QFileInfo (qApp->argv()[0]).dirPath (true));
      pathExe = QDir::cleanDirPath(pathExe);

      if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Editeur de masques", "PathFusionListe", &pathListe)!=0)   return;         // path liste des champs de fusion non defini

      /*if (pathExe.find(G_pCApp->m_PathAppli)!=0)   pathExe   = QDir::cleanDirPath (pathExe.prepend(G_pCApp->m_PathAppli) );*/

      if (QDir(pathListe).isRelative()) pathListe.prepend(G_pCApp->m_PathAppli); //qDebug(QString("pathListe : ") + pathListe);
      pathListe = QDir::cleanDirPath (pathListe);
      QProcess*  proc = new QProcess( this );
      if (proc==0) return;
      proc->addArgument( pathExe );
      proc->addArgument( pathDoc);
      proc->addArgument( QString("$InsertList=") + pathListe );
      proc->addArgument( QString("$Glossaire=")  + G_pCApp->m_PathGlossaire );
      proc->addArgument( QString("$DrTux=")      + G_pCApp->m_PathAppli );
      if ( !proc->start() )
         {//text_Label_Status->setText(qstr.sprintf("OnGlossaireFileToEdit(): Echec de l'ouverture du process: QLightPad"));
         }
      delete proc;
     }
  else if (action==tr("Renommer"))
     {QString   fName = QFileInfo(pathDoc).fileName();
      QDir      dir;
      if (CGenTools::DialogGetString(this, tr("Veuillez donner le nouveau nom de ce document."), fName) ==  QDialog::Accepted)
         {if (fName=="")
             {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Le nouveau nom est vide.<br>Opération annulée"), tr("Annu&ler"));
              return;
             }
          QString new_name = QFileInfo(pathDoc).dirPath () + "/" + fName;
          if (dir.exists(new_name ))
             {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Un fichier portant ce nom existe déjà.<br>Opération annulée"), tr("Annu&ler"));
              return;
             }
          if (dir.rename ( pathDoc, new_name)==FALSE)
             {QString mess  = tr("Cet élément<br><b>");
                      mess += new_name ;
                      mess += tr("</b><br> n'a pu être renommé");
              CGenTools::MessageAlert(this, tr("Erreur"),mess, tr("Annu&ler"));
              return;
             }
          emit Sign_GlossaireRefresh();
         }
     }
  else if (action==tr("Créer"))
     {OnGlossaireFileClicked(pathDoc, typ);
     }
  else if (action==tr("Détruire"))
     {if ( QMessageBox::warning( this, QString(PROG_NAME)+tr(" Destruction de fichier"),
                                             tr ( " La destruction de ce fichier : <br><b>")            +
                                             QFileInfo(pathDoc).fileName()                       +
                                             tr(  "</b><br> sera <u>DÉFINITIVE</u><br>"
                                                  " Confirmez-vous la destruction ?<br>") ,
                                             tr("&Détruire"), tr("Annu&ler"), 0,
                                             1, 1 ) == 0)
          {QFile::remove (pathDoc);
           emit Sign_GlossaireRefresh();
          }
     }
   else if (action==tr("Tout") || action==tr("Ajouter"))
     {QDateTime              qdtp = QDateTime::currentDateTime();
      QString                  fn = QFileInfo(pathDoc).fileName();
      CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) IsExistRubrique(typ);
      if (pCMDI_Generic) pCMDI_Generic->DoActionWithThisDocument(action, typ, pathDoc, &qdtp, &fn);
     }//end else if (action=="Remplacer" || action=="Ajouter")
 }

//------------------------------ FusionneDocument ---------------------------------------------
void DrTux::FusionneDocument(QString  &document, const QString &user_doc, RUBREC_LIST::iterator it, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)
{FusionneDocument(&document, user_doc, &(*it), currentRubIdMap);
}

//------------------------------ FusionneDocument ---------------------------------------------
void DrTux::FusionneDocument(QString  *pDocument, const QString &user_doc, CRubRecord *pCRubCurrentRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)
{
 //............. creer la liste des documents à l'affichage ...........................
 // DOCUMENT_DISPLAY_MAP currentRubIdMap;

 // /*QString user   = */ MapActiveID_Doc(currentRubIdMap);   // retour = utilisateur le plus probable
 QString userPk    = G_pCApp->m_pCMoteurBase->GetUserPrimKey(user_doc);
 CDevilCrucible *pfusion = new CDevilCrucible(pDocument                      ,          // 0 texte du document à fusionner
                                               G_pCApp->m_pCMoteurBase       ,          // 1 moteur de base de données (faut bien accéder aux fonctions)
                                               G_pCApp->m_pCMedicaBase       ,          // 2 moteur de base de données medicamenteuses(faut bien accéder aux fonctions)
                                              &m_RubList                     ,          // 3 liste des documents composant le dossier patient
                                               G_pCApp->m_ID_Doss            ,          // 4 il faut les renseignements sur le patient
                                              &currentRubIdMap               ,          // 5 liste des rubriques courantes (affichées)
                                               userPk                        ,          // 6 utilisateur responsable du document
                                               pCRubCurrentRecord            ,          // 7 pointeur sur la rubrique en cours de modif (si c'est une rubrique) zero sinon
                                               &G_pCApp->m_VAR_MAP                      // 8 pointeur sur la liste des variables (zero par defaut)
                                             );
 if (  pfusion )
    {pfusion-> DoMixture();
     delete pfusion;
    }
}
//------------------------------ GetMapActiveID_Doc ---------------------------------------------------------------
/*! \brief cree une liste mappant l'adresse du CRubRecord relatif aux documents actuellement a l'affichage avec les types de document associes. cette liste permet de retrouver dans la liste des documents celui affiche en fonction de son type cela permet de retrouver par exemple l'observation en cours d'affichage, ou la presscription juste avec le type a rechercher :
 *  \return DOCUMENT_DISPLAY_MAP
*/
DOCUMENT_DISPLAY_MAP DrTux::GetMapActiveID_Doc()
{DOCUMENT_DISPLAY_MAP mapDoc;
 MapActiveID_Doc(mapDoc);
 return mapDoc;
}
//------------------------------ MapActiveID_Doc ---------------------------------------------------------------
/*! \brief cree une liste mappant l'adresse du CRubRecord relatif aux documents actuellement a l'affichage avec les types de document associes. cette liste permet de retrouver dans la liste des documents celui affiche en fonction de son type cela permet de retrouver par exemple l'observation en cours d'affichage, ou la presscription juste avec le type a rechercher :
 *  \param DOCUMENT_DISPLAY_MAP &mapId   reference sur la liste  a renseigner
 *  \return utilisateur du document actuellement affiché si possible dans la rubrique observation sinon celui affiché dans la rubrique prescription sinon celui de la rubrique terrain
*/
QString DrTux::MapActiveID_Doc(DOCUMENT_DISPLAY_MAP &mapDoc)
{if (m_pQWorkSpaceRub==0) return "";
 QString              user_obs = "";
 QString              user     = "";
 QWidgetList windowsList       = m_pQWorkSpaceRub->windowList();
 int id                        = -1;
 int  i                        = 0;
 for ( i = 0; i < int(windowsList.count()); ++i )
     {CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) windowsList.at(i);
      if ( (id = pCMDI_Generic->GetCurrent_RubList_ID()) !=-1)
         {pCMDI_Generic->IfModified_SaveInRubList();
          mapDoc[pCMDI_Generic->m_RubName] = &(*m_RubList.at(id));
          if ( user_obs.length() <=0  && pCMDI_Generic->m_RubName == RUBNAME_OBSERVATION ) user_obs = (*m_RubList.at(id)).m_User;
          if ( user.length() <=0 )                                                         user     = (*m_RubList.at(id)).m_User;
         }
     }
 if (user_obs.length()) return user_obs;
 return user;
}

//------------------------------ GetPossibleObservationDate ---------------------------------------------
int DrTux::GetPossibleObservationDate(const QString &date)
{int    i        = 0;
 QString dateCmp =  date.left(10);
 long max        = G_pCApp->RubNameToType(RUBNAME_OBSERVATION)+999;
 long min        = G_pCApp->RubNameToType(RUBNAME_OBSERVATION);
 long type       = 0;
 RUBREC_LIST::iterator it;
 for (it = m_RubList.begin(); it !=  m_RubList.end(); ++it )
     {// 2003-0000 à 2003-9999   --> Observations
      type = atol((*it).m_Type);
      if (type >= min && type <= max) // a effacer
         {if ((*it).m_Date.left(10) == dateCmp) return i;
         }
      ++i;
     } //end while (pSqlQuery->next())
 return -1;
}

//------------------------------ DuplicateRecordWithNewDate ----------------------
void DrTux::DuplicateRecordWithNewDate(RUBREC_LIST::iterator it, QDateTime &qdt, const QString &libelle_in, const QString &subType)
{QByteArray           data;
 G_pCApp->m_pCMoteurBase->GetDataFromRubList(data, it);       // recuperer données soit dans liste cache soit sur disque
 char *ptr        = data.data();
 long  len        = data.size();
 QString libelle  = libelle_in ; libelle.prepend("==::==");   // pour annuler la verification de conflit de date dans CMDI_Prescription::AddNewDocument()
 //......................... tenir compte des RTF venant de promethée .................
 for (int i=0; i<len; ++i)
     {if (ptr[i]=='{' && ptr[i+1]=='\\' && ptr[i+2]=='r' && ptr[i+3]=='t' && ptr[i+4]=='f')
          {QString stringDST;
           CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len-4, stringDST, 1);
           AddNewRecordToRubrique(stringDST, (*it).m_Type.toInt(), 0, &qdt, &libelle, &subType);
           return;
          }
     };
 if (CGestIni::IsUtf8( ptr , len))
    {AddNewRecordToRubrique(QString::fromUtf8 ( ptr ), (*it).m_Type.toInt(), 0, &qdt, &libelle, &subType);
    }
 else
    {AddNewRecordToRubrique(ptr, (*it).m_Type.toInt(), 0, &qdt, &libelle, &subType);
    }
}

//------------------------------ AddNewRecordToRubrique ----------------------
/*! \brief fonction de creation et ajout d'un nouvel enregistrement des données de rubrique creation d'une nouvelle ordonnance, observation etc ...
*/
/*
 if ( mustBeReadOnly ) 
    { m_pMyEditText->setReadOnly(true);     // les ordonnances securisees ne peuvent etre modifiees que dans le LAP
      m_pMyEditText->setPaper ( QBrush ( QColor("#fffef1"), Qt::SolidPattern   )  );
    }
*/
CRubRecord*  DrTux::AddNewRecordToRubrique(const QString &strData, int typ, const char *path /*=0*/,
                                   const QDateTime *pQDateTime/*=0*/, const QString *libelle /*=0*/,
                                   const QString *subType/*=0*/,
                                   int noInquire /*=0*/)
{CMDI_Generic *pCMDI_Generic       = (CMDI_Generic*) IsExistRubrique(typ);  // ne concerne que les rubriques afficheess
 CRubRecord* pCRubRecord = 0;
 if (pCMDI_Generic )
    {pCRubRecord = pCMDI_Generic->AddNewDocument( strData, typ, path , pQDateTime, libelle, subType  , noInquire);
     //............... ajout de tous les documents dans un classeur ( historique ) dedie de la vigie ...................................
     //                si ce classeur n'existe pas il est cree a la volee
     //                ce classeur dedie est indique dans la section  : [Vigie]
     //                                                      variable : Classement par defaut = Historique des nouveaux documents
     //
     //                le document de ce classeur doit se trouver dans le repertoire :
     //                                              /Ressources/Documents par defaut/variablexxxxx.html dans notre exemple
     //                                              /Ressources/Documents par defaut/Historique des nouveaux documents.html
     //                le nom de fichier du document sert de nom pour le classeur ainsi cree

     if (pCRubRecord && m_pC_Organiseur)
        {QString rubriqueCible = "Vigie";
         CMDI_Generic *pCMDI_GenericClasseur = (CMDI_Generic*) IsExistRubrique(rubriqueCible);
         if (pCMDI_GenericClasseur==0)                         OnActiverOrCreateRubrique(rubriqueCible);
         pCMDI_GenericClasseur = (CMDI_Generic*)               IsExistRubrique(rubriqueCible);
         //.................... Classeur par defaut dans lequel se mettrons tous les documents ................................
         QString classeur      = CGestIni::Param_ReadUniqueParam( G_pCApp->m_DrTuxParam, "Vigie", "Classement par defaut").remove(":");
         if (classeur.length() && classeur[0] != '*' &&  pCMDI_GenericClasseur)
            {//.......... rechercher la rubrique cible de creation d'un document de la vigie .......................
             QString toTest = classeur;    //  protege la variable classeur afin d'eviter que la fonction isThisHierarchieExist() ne rajoute '/'
             if ( ! m_pC_Organiseur->isThisHierarchieExist(toTest) )
                {QString toImport;
                 QString filename = G_pCApp->m_PathAppli + "Ressources/Documents par defaut/"+ classeur + ".html";
                 CGestIni::Param_UpdateFromDisk(filename, toImport);
                 if (toImport.length())
                    {pCMDI_GenericClasseur->AddNewDocument(toImport,  pCMDI_GenericClasseur->GetType(), filename, 0,0,0, 1);
                     //if (pCMDI_GenericClasseur->GetCurrentEditor())   pCMDI_GenericClasseur->GetCurrentEditor()->setModified(TRUE);
                    }
                }
             m_pC_Organiseur->Append_Doc_In_Hierarchie(pCRubRecord, classeur);
            }
         // ..................... classement des ordos par defaut ........................................
         classeur      = CGestIni::Param_ReadUniqueParam( G_pCApp->m_DrTuxParam, "Vigie", "Classement Ordonnances par defaut");
         if (classeur.length() && classeur[0] != '*' &&  pCMDI_GenericClasseur)
            {//.......... rechercher la rubrique cible de creation d'un document de la vigie .......................
             QString toTest = classeur;    //  protege la variable classeur afin d'eviter que la fonction isThisHierarchieExist() ne rajoute '/'
             if ( ! m_pC_Organiseur->isThisHierarchieExist(toTest) )
                {QString toImport;
                 QString filename = G_pCApp->m_PathAppli + "Ressources/Documents par defaut/"+ classeur + ".html";
                 CGestIni::Param_UpdateFromDisk(filename, toImport);
                 if (toImport.length()) pCMDI_GenericClasseur->AddNewDocument(toImport,  pCMDI_GenericClasseur->GetType(), filename, 0,0,0, 1);
                }
             if (typ==20020100) m_pC_Organiseur->Append_Doc_In_Hierarchie(pCRubRecord, classeur);
            }
        }
    }
 if (m_pForm_Menu)  m_pForm_Menu->getRubListAndUpdateView( &m_RubList );
 return pCRubRecord;
}

//------------------------------ DateUserConflictResolv ----------------------------------------------------------
/*! \brief  Si on en est là c'est qu'une prescription est en demande d'élaboration. ACTION : règles : l'utilisateur signataire et l'utilisateur sont les mêmes. il peut y avoir.
*/
int DrTux::DateUserConflictResolve(QString &user, QString &sign_user, QString &date, const QString &obs_sign_user, const QString &obs_date)
{int ret = 0;
 //.............. 2005-03-24T04:56:20 ne comparer que la date  et non les heures minutes  secondes ................
 if ( obs_sign_user == user  &&   obs_date.left(10) != date.left(10) &&
     (ret = QMessageBox::warning ( this, tr ("ATTENTION ! ") + QString(PROG_NAME),
                                         tr ("La date du jour : <b>")           + date.left(10)     + tr("</b> n'est pas la même que celle<br>")  +
                                         tr ("de l'observation en cours : <b>") + obs_date.left(10) + tr("</b> le document à créer doit-il <br>") +
                                         tr ("l'être à la date du jour ou à celle de l'observation ? <br>") ,
                                         tr ("Date du &jour"), tr("Date de l'&observation"), tr("Annu&ler"),
                                         0, 1 )) == 1
    )
    {date = obs_date;
    }

  if (obs_sign_user != user  &&  obs_date.left(10) != date.left(10))                                                   // le signataire de l'observation n'est pas l'utilisateur en cours
     {QString droits = G_pCApp->m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  obs_sign_user , user);        // l'utilisateur a t-il des droits pour le signataire de cette observation
      if (droits.length()!=0 && G_pCApp->IsThisDroitExist(droits, "orc") )                                                              // ==> il a des droits de prescription pour qui prescrit - il ?
         {if ( (ret = QMessageBox::warning ( this,  tr (" Création d'un nouveau document "),
                                          tr("L'utilisateur actuel : <b><font color=\"#e80d0d\">") + user + tr("</font></b> a la confiance de l'utilisateur : <b><font color=\"#e80d0d\">")  + obs_sign_user + "</font></b><br>" +
                                          tr ("signataire de l'observation affichée à la date du : <b>")  + obs_date.left(10) + ".<hr><br></b>"    +
                                          tr ("°&nbsp;&nbsp;&nbsp;Faut-il créer ce document avec cet utilisateur signataire <b><font color=\"#e80d0d\"><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + user + "::" + obs_sign_user + tr("</font></b> et à la date de l'observation <b>") + obs_date.left(10) + "</b> <br><br>"+
                                          tr ("°&nbsp;&nbsp;&nbsp;ou avec l'utilisateur en cours et l'utilisateur signataire en cours: <b><font color=\"#e80d0d\"><br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + user + "::"+ sign_user + tr("</font></b> à la date de ce jour  <b>")  + date.left(10) + ".</b>",
                                          tr ("Utilisateur en &cours"), tr("Utilisateur de l'&observation"), tr("Annu&ler"),
                                          0, 1 )) == 1
              )
              {date      = obs_date;              // si choix du signataire de l'observation alors prendre la datede l'observation
               sign_user = obs_sign_user;         // et le signataire
              }
         }
      else                                                                                                             // sinon la date ne change pas et le signataire non plus
         {/*
          QMessageBox::warning ( this,  tr ("ATTENTION ! ") + QString(PROG_NAME),
                                          tr ("L'observation actuelle a été créée par l'utilisateur :<br><b>") +
                                          obs_sign_user + tr("</b> en date du : <b>")   + obs_date.left(10) +
                                          tr ("<br></b> l'utilisateur actuel :<br><b>") + user
                                          tr ("<br></b> ne poss�de pas les droits pour cette action.") ,
                                          tr ("&Annuler"),
                                          0, 0 )
          */
         }

      }
 return ret;
}

//------------------------------ Slot_ActiverDocument -----------------------
/*! \brief  active le document dont le primary key est donne en entree
* \param QString docPk :   primary key du document dont on veut afficher la rubrique
* \param QString rubName : nom de la rubrique a laquelle est associee l'affichage de ce document
*/
bool DrTux::Slot_ActiverDocument(QString docPk,  QString rubName, int mode /*=0*/)
{
  if (rubName.length()==0 && docPk.length() !=0 )  rubName = DocPrimKeyToRubName(docPk);
  if (rubName.length()==0)  return 0;
  OnActiverOrCreateRubrique(rubName, mode);

  //..... se positionner sur le bon document ..........................................
  CMDI_Generic *pCMDI_Generic = G_pCApp->GetCurrentRubrique();
  if (pCMDI_Generic) pCMDI_Generic->SetCurentDocByPrimkey(docPk);
  return 1;
}

//------------------------------ DocPrimKeyToRubName -----------------------
/*! \brief  retourne le nom de la rubrique a laquelle le document dont le primary key est donne en entre depend
* \param QString docPk :   primary key du document dont on veut le nom de la rubrique
*/
QString DrTux::DocPrimKeyToRubName(QString docPk)
{     QString rubName="";
      RUBREC_LIST::iterator it;
      for (it = m_RubList.begin(); it !=  m_RubList.end(); ++it )
          {if ( docPk == (*it).m_PrimKey )
              {rubName = G_pCApp->TypeToRubName( ((*it).m_Type).toInt() );
               return rubName;
              }
          } //end while (pSqlQuery->next())
      return rubName;
}

//------------------------------ DocPrimKeyToRubName -----------------------
/*! \brief  retourne un pointeur sur un enregistrement de la liste des document dont le primary key est donne en entree
* \param QString docPk :   primary key du document dont on veut le pointeur sur l'enregistrement
*/
CRubRecord* DrTux::DocPrimKeyToRubRecord(const QString &docPk)
{
      RUBREC_LIST::iterator it;
      for (it = m_RubList.begin(); it !=  m_RubList.end(); ++it )
          {if ( docPk == (*it).m_PrimKey ) return &(*it);
          } //end while (pSqlQuery->next())
      return 0;
}

//------------------------------ filePrint -----------------------
void DrTux::filePrint()
{
#ifndef QT_NO_PRINTER
 if ( !currentEditor() ) return;
 DlgPrint *dlg = new DlgPrint(this, "Print_Dial", TRUE);
 if (dlg ==0)            return;
 //........................ recuperer info sur la rubrique en cours .......................................................
 QString             rubName;
 int                 rubType;
 CRubRecord     *pCRubRecord;
 CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) GetCurrentRubrique(&rubName, &rubType, &pCRubRecord);
 //......................... si rubrique non imprimable CASSOS .............................................................
 if (pCMDI_Generic && pCMDI_Generic->IsPrintable()==FALSE)          return;
 RUBREC_LIST::iterator  it   = pCMDI_Generic->Current_RubList_Iterator();
 if (it ==  m_RubList.end() )                                       return;

 //............................ fusionner les entetes et pieds .........................................
 int                          rubOb_id   = -1;
 QTextEdit           *pQTextEditObserv   = 0;
 if (m_pCMDI_Observation)
 { pQTextEditObserv   = m_pCMDI_Observation->GetCurrentEditor();
   rubOb_id           = m_pCMDI_Observation->m_pC_RubObservation->Current_RubList_ID();
 }

 int rubPr_id   = pCMDI_Generic->GetCurrent_RubList_ID();

 //............. là on devrait tout avoir pour initialiser l'impression ................................
 if (dlg->initDialog(pCMDI_Generic->GetCurrentEditor(),               // editeur et texte du corps à
                 G_pCApp->m_ID_Doss,                                       // il faut les renseignements sur le patient
                 G_pCApp->m_pCMoteurBase,                         // moteur de base de donneés
                 &m_RubList,                                      // liste des rubriques en cours
                 rubOb_id,                                        // avec id de l'observ en cours pour extraction renseignemnts
                 rubPr_id,                                        // avec de la rubrique à imprimer
                 pQTextEditObserv ? pQTextEditObserv->text() : 0, // avec texte de l'observation en cours si y en a une
                 G_pCApp->m_pCMoteurBase->GetUserPrimKey((*it).m_SignUser),   //m_User
                 rubName
                )
    )
    {dlg->exec();
    }
 delete dlg;

#endif
}


//------------------------------ setupFileActions ------------------------------------------------------------

void DrTux::setupFileActions()
{
    m_pToolBarFile   = new QToolBar( this );
    m_pMenuFile      = new ThemePopup( this );
    menuBar()->insertItem( tr( "&Fichier" ), m_pMenuFile );
    m_pToolBarFile->setLabel( tr("Outils Fichiers") );
    //if (G_pCApp->IsThisDroitExist( droits, G_pCApp->m_mapNameRubInfos[CMDI_ChoixPatient::S_GetRubName()]+"c"))

    m_pActionListPatCreate = new QAction( tr( "Nouveau dossier..." ), Theme::getIconNewFolder(), tr( "&Nouveau dossier..." ), CTRL + Key_N, this, "fileNew" );
    connect( m_pActionListPatCreate, SIGNAL( activated() ), this, SLOT( OnCreateNewIdent() ) );
    //m_pActionListPatCreate->addTo( m_pToolBarFile );
    //m_pActionListPatCreate->addTo( m_pMenuFile );


    m_pActionListPatDelete = new QAction( tr( "Détruire le dossier" ),  Theme::getIconDelete() , tr( "&Détruire le dossier" ), 0, this, "fileSave" );
    connect( m_pActionListPatDelete, SIGNAL( activated() ), this, SLOT( OnDrTuxIdentCurrentDelete() ) );
    //m_pActionListPatDelete->addTo( m_pToolBarFile );
    //m_pActionListPatDelete->addTo( m_pMenuFile );

    //m_pMenuFile->insertSeparator();

    QAction *a;
    a = new QAction ( tr( "Nouveau document..." ), Theme::getIconNewDoc(), tr( "&Nouveau document..." ), CTRL + Key_O, this, "docNew" );
    connect( a, SIGNAL( activated() ), this, SLOT( OnCreateNewDoc() ) );
    a->addTo( m_pToolBarFile );
    a->addTo( m_pMenuFile );

    a = new QAction( tr( "Enregistrer le dossier" ),  Theme::getIconSave() , tr( "&Enregistrer le dossier" ), CTRL + Key_S, this, "fileSave" );
    connect( a, SIGNAL( activated() ), this, SLOT( OnDrTuxSaveRubList() ) );
    a->addTo( m_pToolBarFile );
    a->addTo( m_pMenuFile );
    m_pActionDossierSave = a;

    m_pMenuFile->insertSeparator();
    a = new QAction( tr( "Changer d'utilisateur" ), Theme::getIcon( "pingoin_tete.png"), tr( "&Changer d'utilisateur" ), CTRL + Key_T, this, "userChange" );
    connect( a, SIGNAL( activated() ), this, SLOT( UserChange() ) );
    a->addTo( m_pToolBarFile );
    a->addTo( m_pMenuFile );

    m_pMenuFile->insertSeparator();
    a = new QAction( tr( "Imprimer la rubrique active..." ),  Theme::getIconPrint() , tr( "&Imprimer la rubrique active..." ), CTRL + Key_P, this, "filePrint" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrint() ) );
    a->addTo( m_pToolBarFile );
    a->addTo( m_pMenuFile );

    m_pMenuFile->insertSeparator();

    a = new QAction( tr( "Enregistrer les profils" ), Theme::getIcon("16x16/ProfilGroupe.png"), tr( "&Enregistrer les profils" ), 0, this, "recordProfil" );
    connect( a, SIGNAL( activated() ), this, SLOT( recordProfil() ) );
    a->addTo( m_pToolBarFile );
    a->addTo( m_pMenuFile );

    if (G_pCApp->IsThisDroitExist( G_pCApp->m_Droits, "adm") )
       { a = new QAction( tr( "Préférences" ), Theme::getIcon("16x16/configure.png"), tr( "&Préférences" ), Key_F12, this, "getFormConfig" );
         connect( a, SIGNAL( activated() ), this, SLOT( getFormConfig() ) );
         a->addTo( m_pToolBarFile );
         a->addTo( m_pMenuFile );
       }

    m_pMenuFile->insertSeparator();
    m_pActionQuitDrTux = new QAction( tr( "Quitter "PROG_NAME ), Theme::getIcon( "stop.png"), tr( "&Quitter "PROG_NAME ), CTRL + Key_Q, this, "fileClose" );
    connect( m_pActionQuitDrTux, SIGNAL( activated() ), this, SLOT( OnMenuQuitterDrTux() ) );
    //m_pActionQuitDrTux->addTo( m_pToolBarFile );
    //m_pActionQuitDrTux->addTo( m_pMenuFile );
    moveDockWindow ( m_pToolBarFile, Qt::DockRight );

}
//---------------------------------- recordProfil ----------------------------
/*! \brief Met en place la barre de gestion des attributs texte avec les menus correspondant en utilisant les icones par défaut de CApp.
*/
void DrTux::recordProfil()
{//CGestIni::Param_SavParam(pk, &USER_PARAM);
     //.................. creer la liste des utilisateurs deja presents .......................
     QStringList userList;
     userList.clear();
     bool isAdmin = G_pCApp->IsThisDroitExist( G_pCApp->m_Droits, "adm");
     //.................. lancer le dialogue sans les utilisateurs déjà présents .......................
     Dlg_PermsUserChoice *pDlg_PermsUserChoice = new Dlg_PermsUserChoice(this, "Dlg_PermsUserChoice", TRUE);
     if ( pDlg_PermsUserChoice )
        {if (isAdmin )
             {pDlg_PermsUserChoice->initDialog(G_pCApp->m_pCMoteurBase, &userList, "");
             }
         else
             {pDlg_PermsUserChoice->initDialog(G_pCApp->m_pCMoteurBase, &userList, G_pCApp->m_User);
             }
         pDlg_PermsUserChoice->textLabel_Entete->setText(tr("Veuillez sélectionner les utilisateurs pour lesquels vous désirez enregistrer le profil actuel"));
         pDlg_PermsUserChoice->setListViewOnUser(G_pCApp->m_User);
         int ret = pDlg_PermsUserChoice->exec();   // en retour la liste : userList est remplie avec les PK des user selectionnes
         delete pDlg_PermsUserChoice;
         if (ret==QDialog::Rejected)  return;
         savePos();          // enregistrer la position des fenetres
         QString path = CGestIni::AbsoluteToRelativePath(G_pCApp->m_PathAppli,G_pCApp->m_PathGlossaire);
         WRITE_USER_PARAM(&USER_PARAM, "Glossaire", "Path", path);  // zero = pas d'erreur
         for ( QStringList::Iterator list_it = userList.begin(); list_it != userList.end(); ++list_it )
             { QString   pk = QString("%") + *list_it;
               if (m_pC_Organiseur)
                  {m_pC_Organiseur->setIniParamFromContext(USER_PARAM);
                  }
               G_pCApp->m_pCMoteurBase->Param_SavParam(&USER_PARAM, pk);
              } //end for ( QStringList::Iterator list_it = userList.begin(); it != userList.end(); ++it )
        G_pCApp->m_pCMoteurBase->CloseBase();
      } //endif ( pDlg_PermsUserChoice )
}

//---------------------------------- setupVigieActions ----------------------------
/*! \brief Met en place la barre de gestion des attributs texte avec les menus correspondant en utilisant les icones par défaut de CApp.
*/
void DrTux::setupVigieActions()
{   QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils Vigie") );
    m_pActionVigie = new QAction( tr( "Activer la vigie" ),    Theme::getIcon("16x16/Vigie.png"), tr( "Activer la vi&gie" ), Key_F4,this,  "Activer la vigie" );
    connect( m_pActionVigie, SIGNAL( activated() ), this, SLOT( ActiverVigie() ) );
    m_pActionVigie->addTo( tb );
    m_pActionVigie->addTo( m_fenetres );
    if (isThisDockMenu_Desired("Vigie")==0) m_pActionVigie->setEnabled(FALSE);
}

//---------------------------------- setupTextActions ----------------------------
/*! \brief Met en place la barre de gestion des attributs texte avec les menus correspondant en utilisant les icones par défaut de CApp.
*/
void DrTux::setupTextActions()
{   //............................... tool bar des polices de caractere ................................................
    QToolBar     *tb = new QToolBar( this );
    ThemePopup *menu = new ThemePopup( this );
    menuBar()->insertItem( tr( "For&mat" ), menu );
    tb->setLabel( tr("Attributs Texte") );
    //...................... combo style .............................
    m_comboStyle = new QComboBox( FALSE, tb );
    m_comboStyle->insertItem( tr("Standard") );
    m_comboStyle->insertItem( tr("Liste à puces (Disque)") );
    m_comboStyle->insertItem( tr("Liste à puces (Cercle)") );
    m_comboStyle->insertItem( tr("Liste à puces (Carré)") );
    m_comboStyle->insertItem( tr("Liste ordonnée (Décimal)") );
    m_comboStyle->insertItem( tr("Liste ordonnée (Alpha minuscule)") );
    m_comboStyle->insertItem( tr("Liste ordonnée (Alpha majuscule)") );
    connect( m_comboStyle, SIGNAL( activated( int ) ),
             this,         SLOT( textStyle( int ) ) );

    //...................... combo font .............................
    m_comboFont = new QComboBox( TRUE, tb );
    QFontDatabase db;
    m_comboFont->insertStringList( db.families() );
    connect( m_comboFont, SIGNAL( activated( const QString & ) ),
             this,        SLOT( textFamily( const QString & ) ) );
    m_comboFont->lineEdit()->setText( QApplication::font().family() );

    //...................... combo size .............................
    m_comboSize = new QComboBox( TRUE, tb );
    QValueList<int> sizes         = db.standardSizes();
    QValueList<int>::Iterator it  = sizes.begin();
    for ( ; it != sizes.end(); ++it )
         m_comboSize->insertItem( QString::number( *it ) );
    connect( m_comboSize,  SIGNAL( activated( const QString & ) ),
             this,         SLOT( textSize( const QString & ) ) );
    m_comboSize->lineEdit()->setText( QString::number( QApplication::font().pointSize() ) );

    //............................ attributs gras italique souligné .....................
    m_pActionTextBold = new QAction( tr( "Gras" ), Theme::getIconTextBold(), tr( "&Gras" ),                   CTRL + Key_B, this, "textBold" );
    connect( m_pActionTextBold, SIGNAL( activated() ), this, SLOT( textBold() ) );
    m_pActionTextBold->addTo( tb );
    m_pActionTextBold->addTo( menu );
    m_pActionTextBold->setToggleAction( TRUE );
    m_pActionTextItalic = new QAction( tr( "Italique" ), Theme::getIconTextItalic(), tr( "&Italique" ),       CTRL + Key_I, this, "textItalic" );
    connect( m_pActionTextItalic, SIGNAL( activated() ), this, SLOT( textItalic() ) );
    m_pActionTextItalic->addTo( tb );
    m_pActionTextItalic->addTo( menu );
    m_pActionTextItalic->setToggleAction( TRUE );
    m_pActionTextUnderline = new QAction( tr( "Souligné" ), Theme::getIconTextUnderline(), tr( "So&uligné" ), CTRL + Key_U, this, "textUnderline" );
    connect( m_pActionTextUnderline, SIGNAL( activated() ), this, SLOT( textUnderline() ) );
    m_pActionTextUnderline->addTo( tb );
    m_pActionTextUnderline->addTo( menu );
    m_pActionTextUnderline->setToggleAction( TRUE );
    menu->insertSeparator();

    //............................ justification .......................................
    m_grp = new QActionGroup( this );
    m_grp->setExclusive( TRUE );
    connect( m_grp, SIGNAL( selected( QAction* ) ), this, SLOT( textAlign( QAction* ) ) );

    m_pActionAlignLeft = new QAction( tr( "Gauche" ), Theme::getIconTextLeft(), tr( "&Gauche" ),           CTRL + Key_L, m_grp, "textLeft" );
    m_pActionAlignLeft->addTo( tb );
    m_pActionAlignLeft->addTo( menu );
    m_pActionAlignLeft->setToggleAction( TRUE );
    m_pActionAlignCenter = new QAction( tr( "Centré" ), Theme::getIconTextCenter(), tr( "C&entré" ),       CTRL + Key_H, m_grp, "textCenter" );
    m_pActionAlignCenter->addTo( tb );
    m_pActionAlignCenter->addTo( menu );
    m_pActionAlignCenter->setToggleAction( TRUE );
    m_pActionAlignRight = new QAction( tr( "Droite" ), Theme::getIconTextRight(), tr( "D&roite" ),         CTRL + Key_R, m_grp, "textRight" );
    m_pActionAlignRight->addTo( tb );
    m_pActionAlignRight->addTo( menu );
    m_pActionAlignRight->setToggleAction( TRUE );
    m_pActionAlignJustify = new QAction( tr( "Justifié" ), Theme::getIconTextJustify(), tr( "&Justifié" ), CTRL + Key_J, m_grp, "textjustify" );
    m_pActionAlignJustify->addTo( tb );
    m_pActionAlignJustify->addTo( menu );
    m_pActionAlignJustify->setToggleAction( TRUE );
    menu->insertSeparator();

    //............................ couleur texte ...........................
    QPixmap pix( 16, 16 );
    pix.fill( black );
    m_pActionTextColor = new QAction( tr( "Couleur" ), pix, tr( "&Couleur..." ), 0, this, "textColor" );
    connect( m_pActionTextColor, SIGNAL( activated() ), this, SLOT( textColor() ) );
    m_pActionTextColor->addTo( tb );
    m_pActionTextColor->addTo( menu );

    m_LasTextColorChoice = QColor(black);
    QPixmap pix2( 4, 16 );
    pix2.fill( m_LasTextColorChoice );
    m_pActionFastTextColor = new QAction( tr( "Couleur rapide" ), pix2, tr( "&Couleur rapide" ), CTRL + Key_K, this, "textColorFast" );
    connect( m_pActionFastTextColor, SIGNAL( activated() ), this, SLOT( fast_textColor() ) );
    m_pActionFastTextColor->addTo( tb );
    m_pActionFastTextColor->addTo( menu );
}

//---------------------------------- InstallCustomMenu ----------------------------
/*! \brief Met en place la barre de gestion des options de menu facultatives.
*/
void DrTux::InstallCustomMenu()
{
    QDir                     dir;
    C_UserQAction *pC_UserQAction     = 0;
    QString                  path     = "";

    //........ on reinitialise tout le tableau a zero ...................
    m_pCustomActionTab.clear();

    //............ Parcours le répertoire, crée les C_UserQAction et les place dans le tableau ...........................
    path                                = G_pCApp->m_PathImages + "Menus";
    dir                                 = QDir(path);
    const QFileInfoList *pQFileInfoList = 0;
    if (dir.exists())
       {dir.setFilter( QDir::All | QDir::NoSymLinks );
        dir.setSorting(QDir::DirsFirst | QDir::Name);
        pQFileInfoList = dir.entryInfoList();
       }
    if (pQFileInfoList!=0)
       {QFileInfoListIterator it( *pQFileInfoList );
        QFileInfo     *fi;
        QString fname="";
        while ( (fi = it.current()) != 0 )
          {  fname = fi->fileName();
             if (fname == ".." || fname == ".")
                {
                }
             else if (fi->isDir() && fname != "." && fname.left(1) != "-" && fname.lower() != "cvs" )            // c'est un autre répertoire
                {        // Prépare le WhatsThis qui contient le path complet du fichier
                 QString optionDir   = path + "/" + fname + "/";
                 pC_UserQAction      = Create_CustomAction(optionDir);
                 if (pC_UserQAction)
                    {appendCustomQAction(pC_UserQAction);
                    }
                }
            else  // c'est un fichier
                {  // QString ext = fi->extension();
                }  // end else if (fi->isFile())
            ++it;
          } // fin while
       }
    if (m_pCustomActionTab.count())
       {if ( m_Custom_ToolBar){ delete m_Custom_ToolBar;
                                m_Custom_ToolBar = 0;
                              }
        if ( m_Custom_Menu )  { if (m_Custom_Menu_ID != -1) menuBar()-> removeItem ( m_Custom_Menu_ID );
                                m_Custom_Menu_ID = -1;
                                delete m_Custom_Menu;
                                m_Custom_Menu    = 0;
                              }
        m_Custom_ToolBar     = new QToolBar( this );
        m_Custom_Menu        = new ThemePopup( this );
        m_Custom_Menu_ID     = menuBar()->insertItem( tr( "Menu &utilisateur" ), m_Custom_Menu );
        m_Custom_ToolBar->setLabel( tr("Menu utilisateur") );
       }
    //................. prend tous les C_UserQAction du tableau et les ajoute au menu ................
    for ( pC_UserQAction = m_pCustomActionTab.first(); pC_UserQAction; pC_UserQAction = m_pCustomActionTab.next() )
        {pC_UserQAction->addTo( m_Custom_ToolBar );
         pC_UserQAction->addTo( m_Custom_Menu );
        }
}

//---------------------------------- Create_CustomAction ----------------------------
/*! \brief crée une options de menu facultative à partir de son repertoire de définition
*/
C_UserQAction *DrTux::Create_CustomAction(QString path)
{ C_UserQAction *pC_UserQAction = 0;
  QString            definition = "";
  CGestIni::Param_UpdateFromDisk(path+"definition.txt", definition);       // charger le fichier de map
  if (definition.length()==0)
     {qDebug ( tr("DrTux::Create_CustomAction : missed or empty file: '%1definition.txt'").arg(path));    return 0;}

  //............. parser le fichier de definition de l'option menu ...........................................
  //<actions>
  //  <action>
  //        <property name="name">
  //            <cstring>Acces_Vidal</cstring>
  //        </property>
  //        <property name="iconSet">
  //            <iconset>vidal.png</iconset>
  //        </property>
  //        <property name="text">
  //            <string>AcceVidal</string>
  //        </property>
  //        <property name="menuText">
  //            <string>&amp;AcceVidal</string>
  //        </property>
  //        <property name="accel">
  //            <string>Ctrl+*</string>
  //        </property>
  //        <property name="script">
  //            <string>{{:: exe_process (WaitEnd ,/usr/bin/kooka)}}</string>
  //        </property>
  //    </action>
  // </actions>
  QString label        = "";
  QString pathIcone    = "";
  QString text         = "";
  QString menuText     = "";
  QString accel        = "";
  QString script       = "";
  QString period       = "";
  int deb_value        = -1;
  int end_value        = -1;

  //........................ label de l'option ......................................
  deb_value = definition.find ( "<cstring>" , 0);
  if (deb_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed first  <cstring>  after property name= \"name\" \nin '%1'").arg(path));    return 0;}
  deb_value += 9;
  end_value  = definition.find ( "</cstring>" , deb_value);
  if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </cstring> after property name= \"name\" \nin '%1'").arg(path));    return 0;}

  label     = definition.mid(deb_value , end_value - deb_value);

  //........................ icone de l'option ......................................
  deb_value = definition.find ( "<iconset>" , end_value + 10);
  if (deb_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed first  <iconset>  after property name= \"iconset\" \nin '%1'").arg(path)); return 0;}
  deb_value += 9;
  end_value  = definition.find ( "</iconset>" , deb_value);
  if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </iconset> after property name= \"iconset\" \nin '%1'").arg(path)); return 0;}

  pathIcone = definition.mid(deb_value , end_value - deb_value);

  //........................ texte de l'option ......................................
  deb_value = definition.find ( "<string>" , end_value + 10);
  if (deb_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed first  <string>  after property name= \"text\" \nin '%1'").arg(path)); return 0;}
  deb_value += 8;
  end_value  = definition.find ( "</string>" , deb_value);
  if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </string> after property name= \"text\" \nin '%1'").arg(path)); return 0;}

  text      = definition.mid(deb_value , end_value - deb_value);

  //........................ texte du menu de l'option ......................................
  deb_value = definition.find ( "<string>" , end_value + 9);
  if (deb_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed first  <string>  after property name= \"menuText\" \nin '%1'").arg(path)); return 0;}
  deb_value += 8;
  end_value  = definition.find ( "</string>" , deb_value);
  if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </string> after property name= \"menuText\" \nin '%1'").arg(path)); return 0;}

  menuText = definition.mid(deb_value , end_value - deb_value);

  //........................ accelerateur de l'option ......................................
  deb_value = definition.find ( "<string>" , end_value + 9);
  if (deb_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed first  <string>  after property name= \"accel\" \nin '%1'").arg(path)); return 0;}
  deb_value += 8;
  end_value  = definition.find ( "</string>" , deb_value);
  if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </string> after property name= \"accel\" \nin '%1'").arg(path)); return 0;}

  accel   = definition.mid(deb_value , end_value - deb_value);

  //........................ script de l'option ......................................
  deb_value = definition.find ( "<string>" , end_value + 9);
  if (deb_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed first  <string>  after property name= \"accel\" \nin '%1'").arg(path)); return 0;}
  deb_value += 8;
  end_value  = definition.find ( "</string>" , deb_value);
  if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </string> after property name= \"accel\" \nin '%1'").arg(path)); return 0;}

  script   = definition.mid(deb_value , end_value - deb_value);
  //.........................OPTIONS FACULTATIVES .....................................................................................
  //........................ periode de l'option ......................................................................................
  deb_value = definition.find ( "<string>" , end_value + 9);
  if (deb_value != -1)
     {deb_value += 8;
      end_value  = definition.find ( "</string>" , deb_value);
      if (end_value == -1)  {qDebug ( tr("DrTux::Create_CustomAction : missed end    </string> after property name in facultatives options \nin '%1'").arg(path));}
      else                  {period = definition.mid(deb_value , end_value - deb_value);}
     }

  //..................... on a tout et on cree l'option ................................................
  pC_UserQAction = new C_UserQAction( label, QPixmap(path+pathIcone), menuText.replace("&amp;","&"), accel, this, text);
  if (pC_UserQAction) {pC_UserQAction->setScript(script);
                       pC_UserQAction->setTimer(period.toInt());
                      }
  return pC_UserQAction;
}

//---------------------------------- On_CustomAction ----------------------------
/*! \brief slot active lorsque le timer d'un custom menu est arrive au bout de sa periode
*/
QString DrTux::On_CustomAction(QString& script, C_UserQAction *pQAction)
{pQAction->pauseTimer(TRUE);
 ExeScript(script);
 int deb       = script.find("$setMenuTimer");
 int debAction = deb;
 if (deb != -1)
    {deb += 13;
     if ( (deb = script.find("["),deb) != -1)
        {++deb;
         int end = script.find("]",deb);
         if (end != -1)
            {int new_period = script.mid(deb,end-deb).stripWhiteSpace().toInt();
             ++end;
             pQAction->setTimer(new_period);
             pQAction->startTimer();
             script = script.remove(debAction, end-debAction);
            }
        }
    }
 MyEditText  *pMyEditText = currentEditor();
 if (pMyEditText)  CHtmlTools::insertHtml(pMyEditText, script);
 pQAction->pauseTimer(FALSE);
 return script;
}

//---------------------------------- Slot_On_UserQAction_TimerDone ----------------------------
/*! \brief slot active lorsque le timer d'un custom menu est arrive au bout de sa periode
*/
QString DrTux::Slot_On_UserQAction_TimerDone(const QString& scriptIn, C_UserQAction *pQAction)
{QString script      = scriptIn;
 return On_CustomAction(script, pQAction);
}
//---------------------------------- Slot_On_UserQAction_Activated ----------------------------
/*! \brief slot active lorsque le timer d'un custom menu est arrive au bout de sa periode
*/
QString DrTux::Slot_On_UserQAction_Activated(const QString& scriptIn, C_UserQAction *pQAction)
{if (scriptIn.find("{{EXECUTE_ON_EXIT}}") != -1 && scriptIn.find("{{EXIT}}") != -1)
    {OnMenuQuitterDrTux();
     return QString::null;
    }
 else
    {return Slot_On_UserQAction_TimerDone(scriptIn, pQAction);
    }
}
//-----------------------------------------------------  ExeScript -----------------------------------------------------
/*! \brief execute le script présenté par le texte
 * \sa CDevilCrucible
*/
void DrTux::ExeScript(QString &text)
{text.remove("\\<br />");
 int pos_end       =  0;
 int len           =  0;
 int pos_deb       =  0;
 int pos_max       =  text.length();
 QString command   = "";
 int initialLen    =  0;
 int maxBoucle     = 30000;
 //........ il vaut mieux recuperer la rubrique avant Slot_ExeMixture () .......
 //         car si zero Slot_ExeMixture() la recherche a chaque fois
 //         et l'operation est longue
 int  rub_type               = 0;
 CRubRecord *pCRubRecord       = 0;
 QString     rubName          = "";
 DOCUMENT_DISPLAY_MAP  map_id = GetMapActiveID_Doc();
 GetCurrentRubrique(&rubName,&rub_type, &pCRubRecord);   // operation couteuse qu'il vaut mieux eviter de faire a chaque Slot_ExeMixture ou Slot_ExePlugin
 while ( (pos_deb        = text.find("{{")) != -1 && pos_deb < pos_max && --maxBoucle>0)
       {  pos_end        =  CGestIni::findFermant(&text, pos_deb + 2, pos_max, "{{", "}}");
          len            =  pos_end - pos_deb +2;
          command        =  text.mid(pos_deb, len);
          initialLen     =  command.length();
          if (initialLen)
             {Slot_ExePlugin(command, pCRubRecord, map_id);
              if (command==tr("!=!"))
                 {command =  text.mid(pos_deb, len);  // recuperer la commande
                  Slot_ExeMixture(command, pCRubRecord, map_id);
                 }
              text.replace(pos_deb, len, command);
             }
          pos_max       +=  command.length() - initialLen;    //  delta_len
       }
  if (maxBoucle == 0) text = tr("<br> <b>Erreur : </b> il semblerai que l'objet receveur du menu actuel n'existe pas"
                                "<br>surement un défaut de connexion au message : Sign_Exe_Plugin()");
}

//---------------------------------- appendCustomQAction ----------------------------
/*! \brief dispatcheur permettant d'attribuer une connection aux  C_UserQAction de chaque option du menu
*/
void DrTux::appendCustomQAction(C_UserQAction *pQAction)
{ m_pCustomActionTab.append(pQAction);
}

//---------------------------------- connectAllCustomQAction ----------------------------
/*! \brief dispatcheur permettant d'attribuer une connection timer aux  C_UserQAction de chaque option du menu
*/
void DrTux::connectAllCustomQAction()
{
    for (C_UserQAction *pC_UserQAction = m_pCustomActionTab.first(); pC_UserQAction; pC_UserQAction = m_pCustomActionTab.next() )
        {connect( pC_UserQAction, SIGNAL( Sign_UserQAction_Activated(const QString&, C_UserQAction *) ), this, SLOT( Slot_On_UserQAction_Activated(const QString&, C_UserQAction *) ) );
         connect( pC_UserQAction, SIGNAL( Sign_UserQAction_TimerDone(const QString&, C_UserQAction *) ), this, SLOT( Slot_On_UserQAction_TimerDone(const QString&, C_UserQAction *) ) );
         pC_UserQAction->startTimer();
        }
}

//---------------------------------- LancerVidal ----------------------------
void DrTux::LancerVidal(int delay)
{QTimer::singleShot(delay, this, SLOT(VidalDataSemp()));
}

//---------------------------------- setupEditActions ----------------------------
/*! \brief Met en place la barre de gestion des attributs texte avec les menus correspondant en utilisant les icones par défaut de CApp.
*/
void DrTux::setupEditActions()
{
    QToolBar *tb = new QToolBar( this );
    ThemePopup *menu = new ThemePopup( this );
    menuBar()->insertItem( tr( "&Édition" ), menu );
    tb->setLabel( tr("Outils Texte") );

    QAction *a;
    a = new QAction( tr( "Annuler" ), Theme::getIconUndo(), tr( "Ann&uler" ), CTRL + Key_Z, this, "editUndo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Refaire" ), Theme::getIconRedo(), tr( "&Refaire" ), CTRL + Key_Y, this, "editRedo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Couper" ), Theme::getIconCut(), tr( "Co&uper" ), CTRL + Key_X, this, "editCut" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCut() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Copier" ), Theme::getIconCopy(), tr( "Cop&ier" ), CTRL + Key_C, this, "editCopy" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Coller" ), Theme::getIconPaste(), tr( "C&oller" ), CTRL + Key_V, this, "editPaste" );
    connect( a, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    a->addTo( tb );
    a->addTo( menu );

    a = new QAction( tr( "Tout sélectionner" ), Theme::getIconSelectAll(), tr( "Tout &Sélectionner" ), CTRL + Key_A, this, "selectAll" );
    connect( a, SIGNAL( activated() ), this, SLOT( editSelectAll() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
}

//---------------------------------- getFormConfig ----------------------------
/*! \brief affihe la fenêtre de gestion des paramètres et gère si nécessaire la sauvegarde des paramètres.
*/
void DrTux::getFormConfig()
{ if (!G_pCApp->IsThisDroitExist( G_pCApp->m_Droits, "adm") ) return;
  FormConfig* pFormConfig = new FormConfig(this);
  if (pFormConfig)
  { pFormConfig->setParams( G_pCApp->m_DrTuxParam , USER_PARAM ,G_pCApp->m_PathAppli);
    pFormConfig->exec();
    if (pFormConfig->result() == QDialog::Accepted)
       { pFormConfig->getParams(G_pCApp->m_DrTuxParam,  USER_PARAM );
         // Sauvegarde les paramètres utilisateurs et appli
         UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User);
         CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, G_pCApp->m_DrTuxParam);
         //.......................... mettre �jour la config de drtux ..........................................
         if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Glossaire", "Path", &G_pCApp->m_PathGlossaire) ==0 ) // zero = pas d'erreur
            { if (m_pFormGlossaire) m_pFormGlossaire->SetCurrentPath(G_pCApp->m_PathGlossaire);
            }
       }
    delete pFormConfig;
  }
 //................. recuperer la taille des polices des menus ...................................
QString val1;
 if (READ_USER_PARAM(USER_PARAM, "Theme config", "PopupFntSize", &val1)==0)  // zero = pas d'erreur
    {int size = val1.toInt();
     Theme::setFontSize_Menu( size );
     QFont fnt (menuBar()->font());
     fnt.setPointSize( size ); menuBar()->setFont(fnt);
    }
}

//---------------------------------- setupToolsTerrain ----------------------------
void DrTux::setupToolsTerrain()
{   m_pMenuTerrain = new ThemePopup( this );
    menuBar()->insertItem( tr( "&Terrain" ), m_pMenuTerrain );
    m_pMenuTerrain->insertItem( tr("Ajouter un antécédent à partir des favoris"),      this, SLOT(add_ATCD_Favori()),                     CTRL+Key_M   );
    m_pMenuTerrain->insertItem( tr("Ajouter un antécédent allergique"),                this, SLOT(add_ATCD_Allergique()),                 CTRL+Key_Exclam   );
    m_pMenuTerrain->insertItem( tr("Ajouter un antécédent CIM10"),                     this, SLOT(add_CIM10()),                           CTRL+Key_Dollar   );
    m_pMenuTerrain->insertItem( tr("Ajouter un antécédent Cisp"),                      this, SLOT(add_Cisp()),                            CTRL+Key_Percent   );
    m_pMenuTerrain->insertItem( tr("Ajouter un antécédent textuel libre"),             this, SLOT(add_ATCD_Libre()),                      CTRL+Key_E    );
    m_pMenuTerrain->insertItem( tr("Modifier le traitement de fond"),                  this, SLOT(modifier_TTT_Fond()),                   CTRL+Key_Asterisk   );
}
void DrTux::add_ATCD_Favori()
{if (m_pCMDI_Terrain)
    {m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewFromFavori();
    }
}
void DrTux::add_ATCD_Allergique()
{if (m_pCMDI_Terrain)
    {m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewAllergie();
    }
}
void DrTux::add_Cisp()
{if (m_pCMDI_Terrain)
    {m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewCISP();
    }
}
void DrTux::add_CIM10()
{if (m_pCMDI_Terrain)
    {m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewCIM10();
    }
}
void DrTux::add_ATCD_Libre()
{if (m_pCMDI_Terrain)
    {m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewTexteLibre();
    }
}
void DrTux::modifier_TTT_Fond()
{if (m_pCMDI_Terrain)  m_pCMDI_Terrain->m_pFormRubTerrain->pushButtonNewTTT_clicked();
}

//------------------------------ setupToolsActions ------------------------------------------------------------

void DrTux::setupToolsActions()
{
    QToolBar *tb = new QToolBar( this );                                // QDockArea *rightDock()
    ThemePopup *menu = new ThemePopup( this );
    menuBar()->insertItem( tr( "&Outils Divers" ), menu );
    tb->setLabel( tr("Outils Divers") );
    QAction *a;

    a = new QAction(  tr( "Outil de gestion des listes  déroulantes..." ), Theme::getIcon( "ToolGestList_Item.png" ), tr( "Outil de &gestion des listes  déroulantes..." ), Key_F8, this, "ListManager" );
    connect( a, SIGNAL( activated() ), this, SLOT( ListManager() ) );
    a->addTo( tb );
    a->addTo( menu );
    m_pActionModifieListe = a;

    a = new QAction( tr( "Créer une liste avec la sélection" ), Theme::getIcon( "CreateList_Item.png"), tr( "Créer une liste avec la &sélection" ), Key_F10, this, "CreateList" );
    connect( a, SIGNAL( activated() ), this, SLOT( CreateList() ) );
    a->addTo( tb );
    a->addTo( menu );
    m_pActionCreateListe = a;

    a = new QAction( tr( "Afficher Popup contextuel" ), Theme::getIcon( "PopupList_Item.png"), tr( "&Afficher le menu contextuel..." ), Key_F2, this, "PopupContextuel" );
    connect( a, SIGNAL( activated() ), this, SLOT( DisplayContextListPopup() ) );
    a->addTo( tb );
    a->addTo( menu );

    m_pActionCIM10 = new QAction( tr( "Cim10" ), Theme::getIcon( "Cim10All_Icon.png"), tr( "&Codage CIM10..." ), Key_F3, this, "CodageCim10" );
    connect( m_pActionCIM10, SIGNAL( activated() ), this, SLOT( CodageCim10() ) );
    m_pActionCIM10->addTo( tb );
    m_pActionCIM10->addTo( menu );

   // m_pActionActesClassants = new QAction( tr( "ActesClassants" ), Theme::getIcon( "ActesClassants_Icon.png"), tr( "&Actes classants..." ), Key_F4, this, "ActesClassants" );
   // connect( m_pActionActesClassants, SIGNAL( activated() ), this, SLOT( ActesClassants() ) );
   // m_pActionActesClassants->addTo( tb );
   // m_pActionActesClassants->addTo( menu );

    m_pActionVidal = new QAction( tr( "Base thérapeutique" ), G_pCApp->m_IconTherapeutique, tr( "&Base thérapeutique..." ), Key_F5, this, "VidalDataSemp" );
    connect( m_pActionVidal, SIGNAL( activated() ), this, SLOT( VidalDataSemp() ) );
    m_pActionVidal->addTo( tb );
    m_pActionVidal->addTo( menu );

    m_pActionCCAM = new QAction( tr( "CCAM" ), Theme::getIcon( "CodageCCAM.png"), tr( "Co&dage CCAM..." ), Key_F6, this, "CodageCCAM" );
    connect( m_pActionCCAM, SIGNAL( activated() ), this, SLOT( Codage_CCAM() ) );
    m_pActionCCAM->addTo( tb );
    m_pActionCCAM->addTo( menu );

    m_pActionLabo = new QAction( tr( "Labo" ), Theme::getIcon( "MoleculeMod00.png" ), tr( "&Placer les derniers résultats Labo" ), Key_F7, this, "PasteLabo" );
    connect( m_pActionLabo, SIGNAL( activated() ), this, SLOT( PasteLabo() ) );
    m_pActionLabo->addTo( tb );
    m_pActionLabo->addTo( menu );
    moveDockWindow ( tb, Qt::DockRight );
}

//------------------------------ OnCreateNewDoc ---------------------------
void DrTux::OnCreateNewDoc()
{QMessageBox::warning ( this, tr(PROG_NAME" Création d'un nouveau document"),
                              tr ( "La création d'un nouveau document se fait :\r\n"
                                   "      en double-cliquant sur un modèle du glossaire (ordonnance type, certificat type ou observation type) \r\n"
                                   "      ou, pour une prescription, en cliquant sur l'icône Thérapeutique dans la barre de menu à droite (touche F5)"),
                                   tr("Ok"), 0, 0,
                                             1, 1 );
}

//------------------------------ PasteLabo ---------------------------
void DrTux::PasteLabo()
{MyEditText *pMyEditText    = currentEditor();
 if (pMyEditText==0) return;
 QString  path;
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Biologie", "PathLastCopy", &path)!=0)   return;         // path liste des champs de fusion non defini
 if (path.find(G_pCApp->m_PathAppli)!=0)   path   = QDir::cleanDirPath (path.prepend(G_pCApp->m_PathAppli) );
 QString  result;
 CGestIni::Param_UpdateFromDisk(path, result);
 if (result.length())
    {CHtmlTools::insertHtml(pMyEditText, result);
    }
}
//------------------------------ SetInterfaceOnDroits ---------------------------
/*! \brief Analyse et gestion des droits pour toute l'application.
 * Cette fonction ajuste l'interface générale de DrTux selon les droits utilisateurs, et fournit à chaque CMDI et Docks les droits de l'utilisateur actuel.
*/
void DrTux::SetInterfaceOnDroits(const QString &droits)
{if (G_pCApp->IsThisDroitExist(droits,"glc"))
    {m_pActionCreateListe->setEnabled (  TRUE );
     m_pActionModifieListe->setEnabled ( TRUE );
    }
 else
    {m_pActionCreateListe->setEnabled (  FALSE );
     m_pActionModifieListe->setEnabled ( FALSE );
    }
 if (m_pFormGlossaire)  m_pFormGlossaire->setInterfaceOnPermisions(droits);

 if (G_pCApp->IsThisDroitExist(droits, "bmc") )      m_pActionVidal->setEnabled(TRUE);
 else                                                m_pActionVidal->setEnabled(FALSE);
 if (G_pCApp->IsThisDroitExist(droits, "cic"))       m_pActionCIM10->setEnabled(TRUE);
 else                                                m_pActionCIM10->setEnabled(FALSE);
 if (G_pCApp->IsThisDroitExist(droits, "lac") )      m_pActionLabo->setEnabled(TRUE);
 else                                                m_pActionLabo->setEnabled(FALSE);
 if (G_pCApp->IsThisDroitExist(droits, "ccc"))       m_pActionCCAM->setEnabled(TRUE);
 else                                                m_pActionCCAM->setEnabled(FALSE);
 //if (G_pCApp->IsThisDroitExist(droits, "acc"))       m_pActionActesClassants->setEnabled(TRUE);
 //else                                                m_pActionActesClassants->setEnabled(FALSE);
 if (m_pForm_TabRubrique) m_pForm_TabRubrique->SetInterfaceOnDroits(droits);
 //............................ fenetre filles .....................................................
 QWidgetList windowsList       = m_pQWorkSpaceRub->windowList();
 int  i  = 0;
 for ( i = 0; i < int(windowsList.count()); ++i )
     {((CMDI_Generic*) windowsList.at(i))->SetInterfaceOnDroits(droits);
     }
 if (m_pCMDI_Ident) {delete   m_pCMDI_Ident; m_pCMDI_Ident=0;}

 // Informer Dock_Menu des droits
 if (m_pForm_Menu)    m_pForm_Menu->SetInterfaceOnDroits(droits);
 if (m_pC_Organiseur) m_pC_Organiseur->SetInterfaceOnDroits(droits);

 //........ virer m_pActionQuitDrTux .............
 //         pour etre sur de le remettre en fin de liste
 m_pActionQuitDrTux->removeFrom( m_pToolBarFile );
 m_pActionQuitDrTux->removeFrom( m_pMenuFile );
 if (G_pCApp->IsThisDroitExist( droits, G_pCApp->m_mapNameRubInfos[CMDI_ChoixPatient::S_GetRubName()]+"c") &&
     G_pCApp->IsThisDroitExist( droits, "idc"))  // Peut créer et modifier l'identité du patient (idc)
    {m_pActionListPatCreate->removeFrom( m_pToolBarFile ); // on vire car si deja en place et que la condition est la meme
     m_pActionListPatCreate->removeFrom( m_pMenuFile );    // cela rajoute deux fois
     m_pActionListPatCreate->addTo( m_pToolBarFile );
     m_pActionListPatCreate->addTo( m_pMenuFile );
    }
 else
    {m_pActionListPatCreate->removeFrom( m_pToolBarFile );
     m_pActionListPatCreate->removeFrom( m_pMenuFile );
    }
 if (G_pCApp->IsThisDroitExist( droits, G_pCApp->m_mapNameRubInfos[CMDI_ChoixPatient::S_GetRubName()]+"c") &&
     ! G_pCApp->IsThisDroitExist( droits, "idd"))   // droit negatif : Ne peut pas effacer un dossier patient (idd)
    {m_pActionListPatDelete->removeFrom( m_pToolBarFile );
     m_pActionListPatDelete->removeFrom( m_pMenuFile );
     m_pActionListPatDelete->addTo( m_pToolBarFile );
     m_pActionListPatDelete->addTo( m_pMenuFile );
    }
 else
    {m_pActionListPatDelete->removeFrom( m_pToolBarFile );
     m_pActionListPatDelete->removeFrom( m_pMenuFile );
    }
 m_pActionQuitDrTux->addTo( m_pToolBarFile );
 m_pActionQuitDrTux->addTo( m_pMenuFile );

}

//------------------------------ UserChange ---------------------------
/*! \brief Slot utilisé pour changer d'utilisateur. Gère l'interface, les CMDI, les Docks et les droits. Sauvegarde dans le fichier INI l'utilisateur de la dernière session.
*/
void DrTux::UserChange()
{QString qstr;
 C_Dlg_ChoixUser  *dlg  = new C_Dlg_ChoixUser(G_pCApp->m_pCMoteurBase, G_pCApp->m_User, G_pCApp->m_SignUser, &G_pCApp->m_DrTuxParam, this, "UserChoice_Dial", TRUE);
 if (dlg ==0)  return;
 //dlg->initDialog(G_pCApp->m_pCMoteurBase, G_pCApp->m_User, G_pCApp->m_SignUser, &G_pCApp->m_DrTuxParam);
 if (dlg->exec()== QDialog::Accepted)
    {G_pCApp->m_User     = dlg->m_User;
     G_pCApp->m_SignUser = dlg->m_SignUser;

     //................ recuperer l'eventuel chemin lie à l'utilisateur .......................
     G_pCApp->m_pCMoteurBase->Param_GetParam( &USER_PARAM , G_pCApp->m_User);
     if (READ_USER_PARAM(USER_PARAM, "Glossaire", "Path", &qstr)==0)  // zero = pas d'erreur
        {G_pCApp->m_PathGlossaire = G_pCApp->ResolvPathGlossaire(qstr);
         OnChangePathGlossaire(qstr);
        }

     emit Sign_OnUserChange(G_pCApp->m_User , G_pCApp->m_SignUser);
     SetTitle();
     //........................... sauvegarder le dernier utilisateur .........................
     CGestIni::Param_WriteParam( &G_pCApp->m_DrTuxParam,  "Derniere Session", "Utilisateur", G_pCApp->m_User, G_pCApp->m_SignUser);
     CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, G_pCApp->m_DrTuxParam);
     G_pCApp->m_Droits  = G_pCApp->m_pCMoteurBase->GetUserPermisions(G_pCApp->m_User );

     SetInterfaceOnDroits(G_pCApp->m_Droits);
    }
 delete dlg;
}

//------------------------------ Lap_setContext ----------------------
/*! \brief creer l'argument contexte du lap
*/
QString DrTux::Lap_getPrescripteurContext()
{

 QString context =   "<?xml version=\"1.0\" encoding=\"UTF-8\"?>                \n"
                     "<!-- Projet    : MedinTux                              -->\n"
                     "<!-- Module    : medicatux                             -->\n"
                     "<!-- Objet     : C_PrescripteurCtx                     -->\n"
                     "<!-- guru mail : roland-sevin@medintux.org             -->\n"
                     "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n"
                     "<C_UserCtx>\n"
                     "   <m_version>1</m_version>\n"
                     "   <m_id>{{USER_GUID}}</m_id>\n"
                     "   <m_id_type>GUID</m_id_type>\n"
                     "   <m_Titre>{{USER_TITRE_PS}}</m_Titre>\n"
                     "   <m_Specialite>{{USER_SPECIALITE_MEDECIN}}</m_Specialite>\n"
                     "   <m_usual_name>{{USER_NOM_MEDECIN}}</m_usual_name>\n"
                     "   <m_forename>{{USER_PRENOM_MEDECIN}}</m_forename>\n"
                     "   <m_sexe>{{USER_SEXE_MEDECIN}}</m_sexe>\n"
                     "   <m_Nu_ordinal>{{USER_IDENTIFICATION_MEDECIN}}</m_Nu_ordinal>\n"
                     "   <m_Nu_RPPS>{{USER_RPPS}}</m_Nu_RPPS>\n"
                     "   <m_Nu_ClefRPPS>{{USER_CLEF_RPPS}}</m_Nu_ClefRPPS>\n"
                     "   <m_Adresse>{{USER_ADRESSE_MEDECIN}}</m_Adresse>\n"
                     "   <m_CodePostal>{{USER_CP_MEDECIN}}</m_CodePostal>\n"
                     "   <m_Ville>{{USER_VILLE_MEDECIN}}</m_Ville>\n"
                     "   <m_Tel1>{{USER_TEL_1_MEDECIN}}</m_Tel1>\n"
                     "   <m_Tel2>{{USER_TEL_2_MEDECIN}}</m_Tel2>\n"
                     "   <m_Tel3>{{USER_TEL_3_MEDECIN}}</m_Tel3>\n"
                     "   <m_Email>{{USER_EMAIL_MEDECIN}}</m_Email>\n"
                     "   <m_Login>{{USER_LOGIN_MEDECIN}}</m_Login>\n"
                     "</C_UserCtx>\n";
 //........ il vaut mieux recuperer la rubrique avant Slot_ExeMixture () .......
 //         car si zero Slot_ExeMixture() la recherche a chaque fois
 //         et l'operation est longue
 int  rub_type               = 0;
 CRubRecord *pCRubRecord     = 0;
 QString     rubName         = "";
 GetCurrentRubrique(&rubName,&rub_type, &pCRubRecord);   // operation couteuse qu'il vaut mieux evierer
 DOCUMENT_DISPLAY_MAP  map_id = GetMapActiveID_Doc();
 Slot_ExeMixture( context , pCRubRecord, map_id);
 //CGestIni::Param_UpdateToDisk("/home/ro/PrescripteurContext.txt", context);
 return context;
 // return QString (QCodecs::base64Encode(context.local8Bit(),true ));
}
//------------------------------ Lap_setContext ----------------------
/*! \brief creer l'argument contexte du lap
*/
QString DrTux::Lap_getPatientContext(const QString &prescriptions)
{
 // DOCUMENT_DISPLAY_MAP  currentDocDisplayMap;
 // QString user      = G_pCApp->m_pDrTux->MapActiveID_Doc(currentDocDisplayMap);   // retour = utilisateur le plus probable
 // QString  str_data = G_pCApp->m_pCMoteurBase->GetDataFromRubList( G_pCApp->GetIDCurrentDoc( tr("Terrain") ));
 QStringList atcds_allergiques;
 QStringList atcds_episodevie;
 QString str_allergiques  ="";
 QString str_episodevie   ="";
 int                     i=0;
 if (m_pCMDI_Terrain)
    {atcds_episodevie   = m_pCMDI_Terrain->m_pFormRubTerrain->getAtcdList( "pathologies" );
     atcds_allergiques  = m_pCMDI_Terrain->m_pFormRubTerrain->getAtcdList( "allergies" );
    }
 for ( i=0; i<(int)atcds_episodevie.size(); ++i)
     {str_episodevie   += atcds_episodevie[i]  + "\n";
     }
 for ( i=0; i<(int)atcds_allergiques.size(); ++i)
     {str_allergiques  += atcds_allergiques[i] + "\n";
     }
 QString context   =   "<?xml version=\"1.0\" encoding=\"UTF-8\"?>                \n"
                       "<!-- Projet    : MedinTux                              -->\n"
                       "<!-- Module    : medicatux                             -->\n"
                       "<!-- Objet     : C_PatientCtx                          -->\n"
                       "<!-- guru mail : roland-sevin@medintux.org             -->\n"
                       "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n"
                       "<C_PatientCtx>\n"
                       "   <m_version>1</m_version>\n"
                       "   <m_id>{{DOSS_GUID}}</m_id>\n"
                       "   <m_id_type>GUID</m_id_type>\n"                           // PROFESSION_PATIENT 
                       "   <m_usual_name>{{NOM_PATIENT}}</m_usual_name>\n"              
                       "   <m_forename>{{PRENOM_PATIENT}}</m_forename>\n"
                       "   <m_young_name>{{NOM_NAISSANCE_PATIENT}}</m_young_name>\n"
                       "   <m_sexe>{{SEXE_PATIENT}}</m_sexe>\n"
                       "   <m_dateFormat>dd-MM-yyyy</m_dateFormat>\n"
                       "   <m_ageOrDateOfBirth>{{DATE_NAISSANCE_PATIENT}}</m_ageOrDateOfBirth>\n"
                       "   <m_poids>{{POIDS_PATIENT}}</m_poids>\n"
                       "   <m_taille>{{TAILLE_PATIENT}}</m_taille>\n"
                       "   <m_bio_hprim></m_bio_hprim>\n"
                       "   <m_allergies>\n{{ALLERGIES}}"
                       "   </m_allergies>\n"
                       "   <m_antecedents>\n{{ATCDS}}"
                       "   </m_antecedents>\n";
   context = context.replace("{{ATCDS}}"      , str_episodevie);
   context = context.replace("{{ALLERGIES}}"  , str_allergiques);
   //........ il vaut mieux recuperer la rubrique avant Slot_ExeMixture () .......
   //         car si zero Slot_ExeMixture() la recherche a chaque fois
   //         et l'operation est longue
   int  rub_type               = 0;
   CRubRecord *pCRubRecord     = 0;
   QString     rubName         = "";
   G_pCApp->m_pDrTux->GetCurrentRubrique(&rubName,&rub_type, &pCRubRecord);   // operation couteuse qu'il vaut mieux evierer
   DOCUMENT_DISPLAY_MAP map_id = GetMapActiveID_Doc();
 
   Slot_ExeMixture( context , pCRubRecord, map_id);
   context += "   <m_prescriptions>\n"+prescriptions+"\n   </m_prescriptions>\n"
              "</C_PatientCtx>\n";

   return context;
   // return QString (QCodecs::base64Encode(context.local8Bit(),true ));
}

//-------------------------------------- Lap_RubListExport_Ordo ----------------------------------------------------------------------
/*! \brief exporte les lignes d'ordonnances LAP dont les produits sont encore actifs pour les redondances
 *  et interactions.
 *  \note il n'est pas tenu compte des ordonnances superieures a un an.
*/
QString  DrTux::Lap_RubListExport_Ordo(RUBREC_LIST *pRubList, const QString &path, int modifMode)
{
  //........ recuperer info sur la rubrique en cours .........................
  //         car si on est en mode modificatione il ne
  //         faut pas placer cette ordonnance dans la liste
  //         des redondances possibles

  CRubRecord     *pCRubRecord;
  GetCurrentRubrique(0, 0, &pCRubRecord);

  RUBREC_LIST::iterator  it;
  QByteArray           data;
  long                  len;
  QStringList ordoLinesList;
  QDateTime    dateFinPrise;
  QDateTime      curDate  = QDateTime::currentDateTime();
  QDateTime lastYearDate  = curDate.addDays ( -365 );         // une ordonnance ne doit pas depasser un an
  QDateTime lastTerrDate  = curDate.addDays ( -365*100 );     // la derniere ordonnance du traitement courant du terrain ne doit pas etre inf a 100 ans
  QString        ordoLine = "";
  //QString            id   = "";
  //QString        id_type  = "";
  QString  s_dateFinPrise = "";
  char               *ptr = 0;
  QString stringHasStruct = "";
  QString dest            = "";
  int            nextPos  = 0;
  // QMap <QString CRubRecord*> cip_map;
  //.................. recuperer les lignes d'ordonnance encore actives ................................
  for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
  {     if ( atoi((*it).m_Type)!= TYP_ORDO_CALC )                continue; // on ne tient pas compte de qui n'est pas une ordonnance
        if ( modifMode              == DrTux::Modification  &&
             pCRubRecord->m_PrimKey == (*it).m_PrimKey )         continue; // on ne tient pas compte de celle en cours si on est en mode modification
        if ( (*it).getDateTime() <= lastYearDate)                continue; // on ne tient pas compte des ordonnances vielles de plus d'un an
        if ( (*it).m_State & RUB_IS_TO_DELETE )                  continue; // on ne tient pas compte des ordonnances a effacer
        G_pCApp->m_pCMoteurBase->GetDataFromRubList( data, it ); // recuperer les donnees
        ptr        = data.data();
        len        = data.size();
        if ( CGestIni::IsUtf8( ptr, len ) )
           { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData ( QString::fromUtf8 ( ptr ) , 0, &stringHasStruct );
           }
        else 
           { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData ( data.data()               , 0, &stringHasStruct);
           }
       ordoLinesList           = CGestIni::getXmlDataList("OrdoLine", stringHasStruct);  // eclater l'ordonnance en lignes prescriptives
       for ( int i=0; i< (int)ordoLinesList.size(); ++i )
           { nextPos           = 0;
             ordoLine          = ordoLinesList[i].stripWhiteSpace();
             //cip               = CGestIni::getXmlData("gph_id", ordoLine, &nextPos, 1);
             //id_type           = CGestIni::getXmlData("gph_it", ordoLine, &nextPos, 1);
             s_dateFinPrise    = CGestIni::getXmlData("gph_df", ordoLine, &nextPos, 1);               // recuperer la date de fin de la ligne prescriptive
             dateFinPrise      = QDateTime::fromString (  CGenTools::NormaliseDateTimeToIso(s_dateFinPrise), Qt::ISODate);
             if (dateFinPrise>=curDate)                                               // si encore active alors la joindre a la liste
                { dest        += QString("\n<OrdoLine>\n") + ordoLine + "\n</OrdoLine>\n";
                  // cip_map[cip] = ordoLine;
                }
           }
     }
  //................. recuperer le dernier traitement de fond ....................................
  //                  on considere que le traitement de fond est toujours actif
  //                  quelque soit la date de fin
  RUBREC_LIST::iterator  last_it = pRubList->end();
  for (it = pRubList->begin(); it !=  pRubList->end(); ++it )
  {     if ( atoi((*it).m_Type)!= TYP_TERRAIN )                  continue; // on ne tient pas compte de qui n'est pas une ordonnance
        if ( modifMode              == DrTux::Modification  &&
             pCRubRecord->m_PrimKey == (*it).m_PrimKey )         continue; // on ne tient pas compte de celle en cours si on est en mode modification
        if ( (*it).getDateTime() < lastTerrDate)                 continue; // on ne tient pas compte des ordonnances plus vielles de plus de la derniere trouvee
        if ( (*it).m_State & RUB_IS_TO_DELETE )                  continue; // on ne tient pas compte des ordonnances a effacer
        last_it      = it;
        lastTerrDate = (*it).getDateTime();
  }
 // avec l'age les raideurs se deplacent  et ma femme me dirait et oui mon vieux complice
 if (last_it != pRubList->end())
    {   G_pCApp->m_pCMoteurBase->GetDataFromRubList( data, last_it ); // recuperer les donnees du dernier traitement de fond
        ptr        = data.data();
        len        = data.size();
        if ( CGestIni::IsUtf8( ptr, len ) )
           { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData ( QString::fromUtf8 ( ptr ) , 0, &stringHasStruct );
           }
        else 
           { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData ( data.data()               , 0, &stringHasStruct);
           }
       ordoLinesList        = CGestIni::getXmlDataList("OrdoLine", stringHasStruct);  // eclater l'ordonnance en lignes prescriptives
       for ( int i=0; i< (int)ordoLinesList.size(); ++i )
           { ordoLine       = ordoLinesList[i].stripWhiteSpace();
             // .......... On considere que le traitement de fond est toujours actif donc ............
             //            on ne tient pas compte de la date de fin des lignes prescriptives
             // s_dateFinPrise = CGestIni::getXmlData("gph_df", ordoLine);               // recuperer la date de fin de la ligne prescriptive
             // dateFinPrise   = QDateTime::fromString (  CGenTools::strToIsoStrDateTime(s_dateFinPrise), Qt::ISODate);
             // if (dateFinPrise>=curDate)                                                // si encore active alors la joindre a la liste
                { QString status =  CGestIni::getXmlData("status" , ordoLine,0,1);        // recuperer status de la ligne prescriptive
                  if (status.find("F")== -1)                                              // lui mettre le statut 'F' comme traitement de fond
                     { status = status+"F";
                       CGestIni::setXmlData("status" ,  status,  ordoLine);               // on place l'attribut traitement de Fond
                     }
                  dest += QString("\n<OrdoLine>\n") + ordoLine + "\n</OrdoLine>\n";
                }
           }
     }
 if (path.length()) CGestIni::Param_UpdateToDisk(path, dest);      // si le chemin est fourni ecrire le fichier d'echange
 return dest;
}

//------------------------------ VidalDataSemp ----------------------
/*! \brief lance 'assistant therapeutique
*/
void DrTux::VidalDataSemp()
{QString fileName       = "";
 QString mode           = "";
 QString exeName        = "";
 QString pathLap        = "";
 QString pathListOrdos  = "";
 if (m_pFormGlossaire)     m_pFormGlossaire->dragClose();
 if (m_pCMDI_Prescription) OnActiverOrCreateRubrique(CMDI_Prescription::S_GetRubName());

 if ( G_pCApp->m_DossPrenom.length() ==0 ||
      G_pCApp->m_DossNom.length()    ==0
    )
    {   QMessageBox::warning ( this, tr(PROG_NAME" Accès à la base Thérapeutique :"),
                              tr(" ERREUR :\n      L'accès à la base Thérapeutique impose\n"
                                 "     que le nom et prénom du patient soient remplis\n"
                                )  ,
                               tr("Annu&ler"), 0, 0, 1, 1 );
        return;
    }

 int retLap  =  Lap_Lauch("", "DRTUX");
 if (retLap == DrTux::IS_ALREADY_LAUCH) return;     // Ok tout s'est bien passe et le LAP repondra dans le Slot_LapExited()

 //............ mode degrade avec l'ancien assistant ...........................
//              soit LAP pas trouve soit pas lance soit .....
 if (m_VidalRun) return;
 m_VidalRun = TRUE;
 Dlg_MedicaTux          *dlg  = new Dlg_MedicaTux(this, "Therapeutique_Dial", TRUE);
 if (dlg ==0)                                                   return;
 if (dlg->initDialog(G_pCApp->m_pCMedicaBase) ==0)    { delete dlg; return; }
 if (dlg->exec()== QDialog::Accepted )
    {
     AddNewRecordToRubrique(dlg->Get_OrdoXmlStruct(),  CMDI_Prescription::S_GetType() + 100);
     emit Sign_ActiverRubrique(CMDI_Prescription::S_GetRubName(),0);
    }
 delete dlg;
 m_VidalRun = FALSE;
}
//------------------------------ Lap_Lauch ----------------------
/*! \brief lance l'assistant therapeutique  
 * \param ordo  QString de l'ordonnance a modifier  si vide alors une nouvelle ordo est a creer
 * \param from  TERRAIN  DRTUX  MODIFY_PRESCRIPTION provenance de l'appel
*/
int DrTux::Lap_Lauch(const QString &ordo, const QString &from)
{        if (m_Lap_Proc)                                                                               return  DrTux::IS_ALREADY_LAUCH;
         QString fileName          = "";
         QString mode              = "";
         QString exeName           = "";
         QString path              = "";
         // QString pathListOrdos     = "";
         QString stringStruct      = "";
         QString stringHasStruct   = "";
         int     export_mode       = DrTux::Modification;   // si on modifie une ordo elle ne doit pas etre listee dans le contexte des ordos actives
         QStringList  argList;
         //...................... rechercher l'adresse du LAP ...............................................
         if ( CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Lap", "path", &fileName, &mode) !=0 )  return DrTux::IS_NOT_FOUND;           // lap non trouve essayrer peut etre une forme degradee
         if ( fileName.length()==0 )                                                                   return DrTux::IS_NOT_FOUND;           // path liste des champs de fusion non defini
         if ( QDir(fileName).isRelative() ) fileName.prepend(G_pCApp->m_PathAppli);
         fileName = QDir::cleanDirPath(fileName);
         exeName  = fileName;
         #ifdef Q_OS_MACX
                QString moduleName = "";
                int            pos = fileName.findRev( "/" );
                if (pos != -1)  moduleName = fileName.mid(pos+1);
                exeName += ".app/Contents/MacOS/" + moduleName;
         #endif
         #ifdef Q_WS_WIN
                exeName += ".exe";
         #endif
         if ( !QFile::exists (exeName ))                                                               return DrTux::IS_NOT_FOUND;
         argList << exeName;
         //.................. creer liste d'arguments............................................
         //............ recuperer la liste de toutes les ordo pour le LAP .................
         //             et les joindre au contexte patient 
         updateAllRubriquesEditorsInRubList();                              // mettre a jour la la listes des donnees
         // pathListOrdos = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "ListOrdo-"+ G_pCApp->m_NumGUID+".xml"; // si "" alors pas ecrit sur disque
         path          = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "PatientContext-"+ G_pCApp->m_NumGUID+"-"+ G_pCApp->delAccents(G_pCApp->m_DossNom+"-"+G_pCApp->m_DossPrenom)+".xml";
         if ( from=="MODIFY_PRESCRIPTION" ) export_mode = DrTux::Modification;
         else                               export_mode = DrTux::NewDocument;
         if ( from=="CREATE_ALL_ACTIVE_PRESCR")
            { CGestIni::Param_UpdateToDisk(path, Lap_getPatientContext( "" ));
            }
         else
            { CGestIni::Param_UpdateToDisk(path, Lap_getPatientContext( Lap_RubListExport_Ordo(&m_RubList,"" /*pathListOrdos*/, export_mode ) ));
            }
         argList << path;              // - 0 - PARAM     PatientContext
         //.............. contexte utilisateur .......................
         path = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "UserContext-"+ G_pCApp->m_SignUser+".xml";
         CGestIni::Param_UpdateToDisk(path, Lap_getPrescripteurContext());
         argList << path;              // - 1 - PARAM      UserContext
         //............... fichier d'echange de l'ordo a modifier ....................................................
         if (ordo.length())
            { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData     (ordo, 0, &stringStruct);       // y isoler et recuperer les données calculables
              CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData (ordo, 0, &stringHasStruct);    // y isoler et recuperer les données calculables
              path = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "PatientContext-"+ G_pCApp->m_NumGUID+"-"+G_pCApp->delAccents(G_pCApp->m_DossNom+"-"+G_pCApp->m_DossPrenom)+".stc";
              CGestIni::Param_UpdateToDisk(path, stringHasStruct);   
              argList << path;         // - 2 - PARAM     PatientContext.stc
              if (stringHasStruct.length() == 0)                                                                           return  DrTux::OLD_ORDO_STRUCT;
            }
         //.................. creer process ............................................
         /*
QProcess ( QObject * parent = 0, const char * name = 0 ) 
QProcess ( const QString & arg0, QObject * parent = 0, const char * name = 0 ) 
QProcess ( const QStringList & args, QObject * parent = 0, const char * name = 0 ) 
          */
         m_Lap_Proc = new QProcess(argList, this , "LapForMedinTux");
         //m_Lap_Proc->setArguments( argList );
         //............... la fin justifie les moyens ..................................
         if (from=="DRTUX")                          // de par icone pour nouvelle ordo
            { connect( m_Lap_Proc, SIGNAL( processExited ()),         this,                                            SLOT(  Slot_LapExited()) );
            }
         else if (from=="MODIF_TERRAIN")                   // ce from=="TERRAIN"             n'est possible que si m_pCMDI_Terrain->m_pFormRubTerrain            existe
            { connect( m_Lap_Proc, SIGNAL( processExited ()),         m_pCMDI_Terrain->m_pFormRubTerrain,              SLOT(  TTT_Slot_LapExited()) );
            }
         else if (from=="MODIFY_PRESCRIPTION")       // ce from=="MODIFY_PRESCRIPTION" n'est possible que si m_pCMDI_Prescription->m_pFormRubPrescription  existe                   // ce from=="TERRAIN"             n'est possible que si m_pCMDI_Terrain->m_pFormRubTerrain            existe
            { connect( m_Lap_Proc, SIGNAL( processExited ()),         m_pCMDI_Prescription->m_pFormRubPrescription,    SLOT(  Slot_LapExited()) );
            }
         else if (from=="CREATE_ALL_ACTIVE_PRESCR")  // ce from=="MODIFY_PRESCRIPTION" n'est possible que si m_pCMDI_Prescription->m_pFormRubPrescription  existe
            { connect( m_Lap_Proc, SIGNAL( processExited ()),         this,                                            SLOT(  Slot_LapAllActivePrescExited()) );
            }
         else if (from=="RENOUV_TERRAIN")            // ce from=="RENOUV_TERRAIN" n'est possible que si m_pCMDI_Terrain->m_pFormRubTerrain  existe
            { connect( m_Lap_Proc, SIGNAL( processExited ()),         m_pCMDI_Terrain->m_pFormRubTerrain,              SLOT(  TTT_Slot_LapExitedRenouv()) );
            }
         if ( !m_Lap_Proc->start() )
            {delete m_Lap_Proc;
             m_Lap_Proc = 0;
             m_pActionVidal->setEnabled(true);
             return    DrTux::LAP_NOT_STARTED;
            }
         else
            {m_pActionVidal->setEnabled(false);
            }

         return    DrTux::IS_ALREADY_LAUCH;
}
//------------------------------ Lap_RemoveExchangeFiles ----------------------
/*! \brief efface les fichiers d'echange de ce patient
*/
void DrTux::Lap_RemoveExchangeFiles()
{ QString fileName;
  QString mode;
  CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Lap", "path", &fileName, &mode);          // path liste des champs de fusion non defini
  if (QDir(fileName).isRelative()) fileName.prepend(G_pCApp->m_PathAppli);
  fileName                 = QDir::cleanDirPath(fileName);
  QString nom_premon       = G_pCApp->delAccents(G_pCApp->m_DossNom+"-"+G_pCApp->m_DossPrenom);
  QString pathListOrdos    = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "ListOrdo-"       + G_pCApp->m_NumGUID+".xml"; // si "" alors pas ecrit sur disque
  QString pathPAtientCtx   = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "PatientContext-" + G_pCApp->m_NumGUID+"-"+ nom_premon +".xml";
  QString pathUserCtx      = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "UserContext-"    + G_pCApp->m_SignUser+".xml";
  QString xmlPath          = QString ( QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/Ordo_%1_.xml" ) .arg(G_pCApp->m_NumGUID) ;
  QString htmPath          = QString ( QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/Ordo_%1_.html" ).arg(G_pCApp->m_NumGUID) ;
  QString pathOrdoToModiy  = QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/" + "PatientContext-"+ G_pCApp->m_NumGUID+"-"+ nom_premon +".stc";
 // if (G_pCApp->debugMode()) return;
  QFile::remove(pathListOrdos);
  QFile::remove(pathPAtientCtx);
  QFile::remove(pathUserCtx);
  QFile::remove(xmlPath);
  QFile::remove(htmPath);
  QFile::remove(pathOrdoToModiy);
}

//------------------------------ Slot_LapExited ----------------------
/*! \brief l'assistant therapeutique externe a quitte
*/
void DrTux::Slot_LapExited()
{ Lap_StopProcess();
  QString stringDST = Lap_ExchangesFilesToDataBlob();
  if ( stringDST.length() )
    { AddNewRecordToRubrique(stringDST, TYP_ORDO_CALC);
    }
 m_pActionVidal->setEnabled(true);
}
//------------------------------ Slot_LapAllActivePrescExited ----------------------
/*! \brief l'assistant therapeutique externe a quitte
*/
void DrTux::Slot_LapAllActivePrescExited()
{ Lap_StopProcess();
  Lap_RemoveExchangeFiles();
  // QString stringDST = Lap_ExchangesFilesToDataBlob();
  // if ( stringDST.length() )
  //  { AddNewRecordToRubrique(stringDST, TYP_ORDO_CALC);
  //  }
 m_pActionVidal->setEnabled(true);
}
//------------------------------ Lap_StopProcess ----------------------
void DrTux::Lap_StopProcess()
{ delete m_Lap_Proc;
  m_Lap_Proc   = 0;
  m_pActionVidal->setEnabled(true);
}
//------------------------------ Lap_ExchangesFilesToDataBlob ----------------------
/*! \brief recupere les donnees du fichier d'echange apres appel du LAP puis le converti en donnees pretes a etre mises en BLOB, au passage si p_html != 0 il y est retourne la partie html
*/

QString DrTux::Lap_ExchangesFilesToDataBlob(QString *p_html /*=0*/)
{//.......... recuperer adresse du LAP et fichiers d'echanges ....................................
 QString fileName;
 QString mode;
 CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Lap", "path", &fileName, &mode);          // path liste des champs de fusion non defini
 if (QDir(fileName).isRelative()) fileName.prepend(G_pCApp->m_PathAppli);
 fileName        = QDir::cleanDirPath(fileName);
 QString xmlPath = QString ( QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/Ordo_%1_.xml" ) .arg(G_pCApp->m_NumGUID) ;
 QString htmPath = QString ( QFileInfo(fileName).dirPath() + "/Ressources/ComFiles/Ordo_%1_.html" ).arg(G_pCApp->m_NumGUID) ;
 //.......... recuperer fichiers d'echange ....................................
 QString xml     = "";
 QString htm     = "";
 CGestIni::Param_UpdateFromDisk(  xmlPath, xml);
 CGestIni::Param_UpdateFromDisk(  htmPath, htm);
 Lap_RemoveExchangeFiles();
 //........... composer le BLOB ...................................................
 QString stringDST = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>";
 stringDST.append ("\n<ordotext>\n");
 stringDST.append (htm);
 stringDST.append ( "\n</ordotext>\n");
 stringDST.append ("\n<ordoMedicaStruct>\n");
 stringDST.append (G_pCApp->m_pCMedicaBase->Medica_Has_StructDataToOldXmlStruct( xml ));
 stringDST.append ("<Posologie_LAP_List>\n\n");
 stringDST.append (xml);                    // ajouter la structure calculable non modifiée
 stringDST.append ("</Posologie_LAP_List>\n");
 stringDST.append ("\n</ordoMedicaStruct>\n");
 if ( p_html ) *p_html = htm;
 if ( xml.length() && htm.length() ) return stringDST;
 return "";
}
//------------------------------ LapClose ----------------------
void DrTux::LapClose()
{if (m_Lap_Proc==0) return;
 m_Lap_Proc->tryTerminate ();
 m_Lap_Proc->kill ();
 Slot_LapExited();
}
//------------------------------ Slot_CreateAllCurrentsTTT -----------------------------------------------------------------
void DrTux::Slot_CreateAllCurrentsTTT()
{ QString ordo = QString("<ordotext></ordotext><Posologie_LAP_List>") + Lap_RubListExport_Ordo(&m_RubList, "", DrTux::NewDocument ) + "</Posologie_LAP_List>";
  Lap_Lauch(ordo, "CREATE_ALL_ACTIVE_PRESCR");
}
//------------------------------ currentEditor ----------------------
/*! \brief Retourne un pointeur sur l'editeur de texte de la fenetre MDI courante ou zero si elle n'en a pas.
*/
MyEditText *DrTux::currentEditor() const
{   QWidget *pQwdgRub =  m_pQWorkSpaceRub->activeWindow();
    if (pQwdgRub==0)        return 0;
    return ((CMDI_Generic*) pQwdgRub)->GetCurrentEditor();
}

//------------------------------ GetCurrentRubrique ----------------------
/*! \brief retourne un pointeur sur la rubrique courante
 *  \param pRubName : pointeur sur une QString qui si il n'est pas nul contiendra le nom de la rubrique courante
 *  \param rub_type_ret : pointeur sur un int qui si il n'est pas nul contiendra le type de la rubrique
 *  \param ppCRubRecord : pointeur sur un pointeur sur CRubRecord  qui si il n'est pas nul contiendra un pointeur sur l'enregistrement en cours
 *  \return Renvoie un un pointeur sur la rubrique en cours sinon vide (NULL)
*/

QWidget *DrTux::GetCurrentRubrique(QString *pRubName /* = 0 */, int* rub_type_ret /* = 0 */, CRubRecord **ppCRubRecord/* = 0 */)
{   if (pRubName)     *pRubName         = "";
    if (rub_type_ret) *rub_type_ret     = 0;
    if (ppCRubRecord) *ppCRubRecord     = 0;
    QWidget *pQwdgRub                   =  m_pQWorkSpaceRub->activeWindow();
    if (pQwdgRub==0)        return 0;
    if (pRubName)             *pRubName = pQwdgRub->name();
    if (rub_type_ret)     *rub_type_ret = ((CMDI_Generic*)pQwdgRub)->GetType();
    int id                              = -1;

    if (ppCRubRecord)
       {if ( (id = ((CMDI_Generic*)pQwdgRub)->GetCurrent_RubList_ID()) != -1 )
           {  RUBREC_LIST::iterator  it = m_RubList.at(id);
             *ppCRubRecord = &(*it);
           }
       }
    return pQwdgRub;
}

//------------------------------ OnSign_Ident_KillMe ---------------------
/*! \brief Lorsque ce slot est activé il détruit la rubrique identité, mais comme le signal vient de la rubrique identité il faut le faite par l'intermediaire d'un timer.
*/
void DrTux::OnSign_Ident_KillMe()
{QTimer::singleShot(0, this, SLOT(TimedDeleteRubIdentite()) );
}
void DrTux::TimedDeleteRubIdentite()
{if (m_pCMDI_Ident) delete m_pCMDI_Ident;
}

//------------------------------ doConnections ---------------------
/*! \brief Connecte les signaux de changement d'attribut texte d'un editeur de texte aux boutons d'attributs texte afin qu'ils puissent mettre à jour leur etat.
*/
void DrTux::doConnections( QTextEdit *e )
{
    connect( e,     SIGNAL( currentFontChanged( const QFont & ) ),
             this,  SLOT( fontChanged( const QFont & ) ) );
    connect( e,     SIGNAL( currentColorChanged( const QColor & ) ),
             this,  SLOT( colorChanged( const QColor & ) ) );
    connect( e,     SIGNAL( currentAlignmentChanged( int ) ),
             this,  SLOT( alignmentChanged( int ) ) );
}


//-------------------------------------------- Slot_GetActiveGUID ---------------------
/*! \brief slot appelé pour récupérer le GUID en cours.
 *  \param QString guid qui recevra le GUID en cours
*/
void DrTux::Slot_GetActiveGUID(QString &guid)
{guid = G_pCApp->m_NumGUID;
}

//-------------------------------------------- CodageCim10 ---------------------
void DrTux::CodageCim10()
{CodageCim10(DrTux::InsertString);
}

//-------------------------------------------- CodageCim10 ---------------------
QString DrTux::CodageCim10(int mode, const QString &listCode, int tabToSet)
{int                                 nb  = 0;
 QString                libelleRubrique  = "";
 QString                    str_toReturn = "";
 QString                       configDlg = "CIM10";
 if (mode==DrTux::GestionATCD) configDlg = "ATCD";
 MyEditText                 *pMyEditText = currentEditor();
 C_Dlg_GestionATCD                 *dlg  = new C_Dlg_GestionATCD(tabToSet, configDlg, this, "CIM10_Dial", TRUE );
 if (dlg ==0)                                                                                                 return str_toReturn;
 if ( ! connectDlgAtcdDialogToDataBase(dlg) )                                                    {delete dlg; return str_toReturn;}
 if (mode==DrTux::ATCDString)
    {dlg->setListCode(listCode);  // initialiser avec la liste des codes
     dlg->setOnglet(0);
    }
 if ( ! ( dlg->exec() == QDialog::Accepted && (nb =dlg->listViewCim10_Choix->childCount()) ) )   {delete dlg; return str_toReturn;}  // si pas de ligne retournee ou 'annuler' cassos
 if (pMyEditText==0 && mode!=DrTux::ATCDString)                                                  {delete dlg; return str_toReturn;}  // si pas d'editeur trouvé cassos

 //................ iterer la liste de choix ........
 QListViewItemIterator it( dlg->listViewCim10_Choix);
 int lgn = 0;
 while  ( it.current() )
        {
         QListViewItem *item = it.current();
         ++it;
         QString codeCim10 = item->text(6).remove('~').remove('(').remove(')');
         QString libelle   = item->text(0);
         if (mode==DrTux::ATCDString)
            {str_toReturn += libelle + "::"+codeCim10+"@@";
            }
         else
            { //................... petits trucs archi chichiteux de mise en page ...........................
              if (nb>1) {if (lgn==0) {str_toReturn += QString("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + libelle + "  ~["+codeCim10+"]~    "; libelleRubrique = libelle;}
                         else        {str_toReturn += QString("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + libelle + "  ~["+codeCim10+"]~    ";}
                         ++lgn;
                        }
              else      {str_toReturn   += libelle + "  ~["+codeCim10+"]~    ";
                         libelleRubrique = libelle;
                        }
            }
        }
 str_toReturn.replace ("~[-","-[");
 str_toReturn.replace ("-]~","]-");
 delete dlg;
 if (str_toReturn.length() && mode==DrTux::ATCDString)   return str_toReturn;
 if (str_toReturn.length() && mode==DrTux::InsertString) CHtmlTools::insertHtml(pMyEditText, str_toReturn, 0 );
 //................. nom de la patho CIM 10 en nom de rubrique ......................................
 int rub_type;
 CMDI_Observation      *pQwdgRubObserv = (CMDI_Observation*) GetCurrentRubrique(0, &rub_type );
 if (pQwdgRubObserv && rub_type == G_pCApp->RubNameToType(RUBNAME_OBSERVATION))
    {pQwdgRubObserv->m_pC_RubObservation->SetRubDateName( libelleRubrique, "" );
    }
 return str_toReturn;
}

//-------------------------------------------- CodageCisp ---------------------
QString DrTux::CodageCisp(  const QString &chapiFilter, const QString &classFilter, const QString &templateStr, const QString &showCombos )
{QString               str_toReturn = "";
 QString                   tmpl_str = "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ::CISP_CHAPITRE:: ::CISP_LIBELLE:: ::CISP_COMMENT:: code Cisp = ::CISP_CODE::;";
 if (templateStr.length()) tmpl_str = templateStr;
 C_Dlg_GestionATCD         *dlg     = new C_Dlg_GestionATCD(4, "Cisp", this, "Cisp_Dial", TRUE );
 if (dlg ==0) return QString("");
 if (connectDlgAtcdDialogToDataBase(dlg))
    {//..................... executer le dialogue ...........................
     dlg->listView_Cisp_filter(chapiFilter     /* ="-FDABHKLNPRSTUWXYZ" */ ,
                               classFilter     /* = "SINTCD"            */ );
     if ( !showCombos.upper().contains('C') ) {dlg->comboBox_Cisp_filter_Chapi->hide(); dlg->textLabel_cisp_filtrer_Chapi->hide();}
     if ( !showCombos.upper().contains('U') ) {dlg->comboBox_Cisp_filter_Class->hide(); dlg->textLabel_cisp_filtrer_Class->hide();}
     if (dlg->exec() == QDialog::Accepted && dlg->listViewCim10_Choix->childCount () )
         { //................ iterer la liste de choix ........
           QListViewItemIterator it( dlg->listViewCim10_Choix);
           while ( it.current() )
              {
               QListViewItem *item = it.current();
               ++it;
               QString tmpl_resolv = tmpl_str;
               tmpl_resolv         = tmpl_resolv.replace( "::CISP_LIBELLE::",  item->text(0) )
                                                .replace( "::CISP_COMMENT::",  item->text(3) )
                                                .replace( "::CISP_CHAPITRE::", dlg->getCispChapitreFromCodeCisp(item->text(6)))
                                                .replace( "::CISP_CODE::",     item->text(6).remove('-').remove('(').remove(')') );
               str_toReturn += tmpl_resolv;
              }
        }
    }
 delete dlg;
 return str_toReturn;
}


//-------------------------------------------- connectDlgAtcdDialog ---------------------
bool DrTux::connectDlgAtcdDialogToDataBase(C_Dlg_GestionATCD *dlg)
{//..................... lire les parametres de connection à la base CIM10 ...........................
 QString qstr, driver, base, login, password, host, port, error;
 if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CIM10", "Connexion", &driver, &base, &login, &password, &host, &port) !=0 )  // zero = pas d'erreur
    { driver     = "QMYSQL3";
      base       = "CIM10";
      login      = "root";
      password   = "";
      host       = "localhost";
      port       = "3306";
    }
 //........................ ouvrir base CIM 10 si besoin ............................
 if ( m_DataBaseCIM10 == 0 )
    { m_DataBaseCIM10 = dlg->BaseConnect(driver,                  // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                         base,                    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                         login,                   // = "root"
                                         password,                // = ""
                                         host,                    // = "localhost"
                                         port,
                                         &error);
    }
 else
    {dlg->dataBaseSet(m_DataBaseCIM10);
    }
 if (m_DataBaseCIM10) return TRUE;

 QMessageBox::warning ( this, tr(PROG_NAME" Accès à la base CIM10 :"),
                              tr(" ERREUR :\n      L'accès à la base CIM10 est impossible : \n")     +
                              tr("Parametres :\n") +
                                 "      driver : " +driver + " base :" + base + " login :" + login + " host :" + host+":"+port + "\n" + "Erreur :\n      " + error + "\n" ,
                               tr("Annu&ler"), 0, 0,
                                             1, 1 );
 return FALSE;
}

//------------------------------ GetCurrentRubriquesPk -----------------------------------------------------------
/*! \brief Retourne le PkDoc affiché dans la fenêtre qui est au premier plan.
*/
void DrTux::GetCurrentRubriquesPk(QString &obsPk, QString &ordPk, QString &terPk)
{// Récuperer les pointeurs sur les rubriques  -> inutile ils sont dispo dans la classe (gain temps proc)
 //......................... placer les elements en cours de modification dans la liste des rubriques ...............
 int obserRubListCurID = -1;
 int prescRubListCurID = -1;
 int terraRubListCurID = -1;

 // noter rubrique affichée
 if (m_pCMDI_Observation)
     obserRubListCurID = m_pCMDI_Observation->m_pC_RubObservation->Current_RubList_ID();
 if (m_pCMDI_Prescription)
     prescRubListCurID = m_pCMDI_Prescription->m_pFormRubPrescription->Current_RubList_ID();
 if (m_pCMDI_Terrain)
     terraRubListCurID = m_pCMDI_Terrain->m_pFormRubTerrain->Current_RubList_ID();

  //............................ mettre à jour les rubriques ............................................
  //                             noter le primKey de chaque rubrique affichée avant de detruire
  //                             et reconstruire la liste afin de les retrouver par la suite
 RUBREC_LIST::iterator  it;
 obsPk = "";
 ordPk = "";
 terPk = "";
 if (m_pCMDI_Observation &&        // il faut la rubrique
     obserRubListCurID != -1        // et qu'elle ait un truc affiché
    )
    {it    = m_RubList.at(obserRubListCurID);
     obsPk = (*it).m_PrimKey;                  // noter le primKey de cette rubrique
    }
 if (m_pCMDI_Prescription &&         // il faut la rubrique
     prescRubListCurID != -1       // et qu'elle ait un truc affiché
    )
    {it    = m_RubList.at(prescRubListCurID);
     ordPk = (*it).m_PrimKey;                 // noter le primKey de cette rubrique
    }
 if (m_pCMDI_Terrain &&              // il faut la rubrique
     terraRubListCurID != -1       // et qu'elle ait un truc affiché
    )
    {it    = m_RubList.at(terraRubListCurID);
     terPk = (*it).m_PrimKey;                 // noter le primKey de cette rubrique
    }
}

//------------------------------ Slot_ExePlugin ---------------------------
/*! \brief Execute le plugins dont le nom est passé en paramètre.
 *  Plugins utilisables :
 *  - CCAM_View
 *  - CIM10Urg
 *  - ActesClassants
 *  - [Execute]
 *  - PlacerBiologie
 *  - PlacerPathologies, DefinirATCD_Cim10, DefinirATCD_Allergie,  (A virer ou réécrire en utilisant m_pAtcd_Code)
 *  - TraitementCourant, TraitementHabituel
 *  - InsérerOrdo
 *  \todo gérer présence du Dock_Menu et utiliser Atcd_Code
*/

void DrTux::Slot_ExePlugin(QString &plugin, CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)
{//MyEditText *pMyEditText = currentEditor();
 plugin = plugin.mid(2,plugin.length()-4);
 if (plugin=="CCAM_View")
    {plugin = Codage_CCAM(DrTux::ReturnString);
    }
 else if (plugin=="CIM10Urg")
    {plugin = CodageCim10(DrTux::ReturnString);
    }
 else if (plugin.left(9)=="[Execute]")
    {
      plugin = plugin.stripWhiteSpace();
      CHtmlTools::HtmlToAscii(plugin);
      QString     pathPlugin;  // Chemin de l'executable plugin à actionner sans l'extension .exe
      QString     maskExch;    // texte du fichier d'exchange
      QString     waitEnd = "y";    // texte du fichier d'exchange
      QString     obsPk;
      QString     terPk;
      QString     ordPk;
      QStringList param;
      CGestIni::Param_ReadParam(plugin,"Execute","pathPlugin",&pathPlugin);
      CGestIni::Param_ReadParam(plugin,"Execute","maskExch",  &maskExch);
      if (CGestIni::Param_ReadParam(plugin,"Execute","waitEnd",  &waitEnd) !=0) waitEnd = "y";
      GetCurrentRubriquesPk(obsPk, ordPk, terPk);
      //.................................. PARAMETRES ...........................................
      char             *pt   = (char*)(const char*) plugin;
      QString     val1;
      QString     var_name   = "";
      QString      section   = "";
      while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
           {if (section=="Parametres")
               {while (*pt && *pt != '[')
                      {pt          = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &val1);
                       int pos_max = val1.length();
                       if (pos_max)
                          {int delta_len     =  0;
                           int pos_end       =  0;
                           int pos_deb       =  val1.find("{{");
                           QString command   =  "";
                           int initialLen    =  0;

                           while (pos_deb != -1)
                                 {delta_len      = 0;
                                  pos_end        =  CGestIni::findFermant(&val1, pos_deb+2, pos_max, "{{", "}}");
                                  command        =  val1.mid(pos_deb, pos_end - pos_deb + 2);
                                  initialLen     =  command.length();
                                  if (initialLen)
                                     {  Slot_ExeMixture( command , pCRubRecord, currentRubIdMap);
                                        val1.replace(pos_deb, pos_end - pos_deb+2, command);
                                        delta_len   =  initialLen - command.length();
                                     }
                                  pos_max       +=  delta_len;
                                  pos_deb        =  val1.find("{{", pos_deb+2);
                                  if (pos_deb>=pos_max) break;
                                 }
                           param.append(val1);
                          }
                      }
               }
            section="";
           }
      plugin =                      PluginExe(this,
                                              pathPlugin,                       // Chemin de l'executable plugin à actionner sans l'extension .exe
                                              G_pCApp->m_PathAppli,        // Chemin de l'executable appelant (DrTux)
                                              G_pCApp->m_PathDrTuxIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                              maskExch,                         // texte du fichier d'exchange
                                              G_pCApp->m_NumGUID,
                                              obsPk,
                                              ordPk,
                                              terPk,
                                              param,
                                              (waitEnd.lower().at(0)=='n') ? CMoteurBase::noWait : CMoteurBase::endWait );
    }

 else if (plugin.left(14)=="PlacerBiologie")
    { plugin = "";
      QString  path;
      if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Biologie", "PathLastCopy", &path)!=0)   return;         // path liste des champs de fusion non defini
      if (path.find(G_pCApp->m_PathAppli)!=0)   path   = QDir::cleanDirPath (path.prepend(G_pCApp->m_PathAppli) );
       CGestIni::Param_UpdateFromDisk(path, plugin);
    }
 else if (plugin.left(17)=="PlacerPathologies" || plugin.left(12)=="DefinirATCD_")
    {if (m_pCMDI_Terrain==0) return;
     if (plugin.left(16)=="DefinirATCD_Cisp")      m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewCISP();
     if (plugin.left(17)=="DefinirATCD_Cim10")     m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewCIM10();
     if (plugin.left(20)=="DefinirATCD_Allergie")  m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewAllergie();
     if (plugin.left(19)=="DefinirATCD_Favoris")   m_pCMDI_Terrain->m_pFormRubTerrain->ATCD_MenuActionNewFromFavori();
     plugin = G_pCApp->m_pAtcd_Code->ATCD_To_Text(tr( "<b>°&nbsp;<u>::TYPE:: :</u></b>"),
                                                  tr("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;::LIBELLE:: ::NOTE:: ::ALD:: ::DATE::"),
                                                  tr("en date du : "),
                                                  tr("est lié à une pathologie ALD")
                                                 );
     plugin.prepend("<br>");
    }
 else if (plugin=="TraitementCourant"||plugin=="TraitementHabituel")
    {if (m_pCMDI_Terrain)
        {//............................... si pas de terrain en créer un par défaut .........................................
         if (m_pCMDI_Terrain->m_pFormRubTerrain->comboBox_RubName->count()<=0)
            {QString modele(TERRAIN_DEF_VAR);                 //  TERRAIN_DEF_VAR --> voir CMDI_Terrain.h
             AddNewRecordToRubrique(modele, CMDI_Terrain::S_GetType() );
            }

         if (plugin=="TraitementCourant")
            {plugin                  = "";
             //              ATTENTION LA CONFIG DE BOUCLES CI-DESSOUS est INDISPENSABLE           
             //............. on lance le LAP et on attend la fin ................
             if (m_pCMDI_Terrain->m_pFormRubTerrain->TTT_Modifier("MODIF_TERRAIN")==0) return;
             if (m_Lap_Proc)
                { SLEEP(1);
                  G_pCApp->processEvents();
                  while ( m_Lap_Proc->isRunning() )    // m_Lap_Proc est remis a zero apres recuperation des donnees dans le terrain
                   {
                     SLEEP(1);
                     QTimer timer;               // pour etre sur qu'un evenement se produise
                     timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
                     QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
                   } // while (m_Lap_Proc->isRunning ())
                } // if (m_Lap_Proc)
             //............. on attend la sortie du LAP de FormRubTerrain::TTT_Slot_LapExited() ................
             //              car c'est a ce momement qu'il renseigne le terrain          
             //              CETTE DEUXIEME BOUCLE EST INDISPENSABLE NE PAS TENTER DE LA 
             //              FUSIONNER AVEC CELLE DU DESSUS    
             while (m_Lap_Proc)
               { G_pCApp->processEvents();
                 QTimer timer;               // pour etre sur qu'un evenement se produise
                 timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
                 QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
               }
            } //if (plugin=="TraitementCourant")
         plugin                           = "";
         QPL_CPosologie       *pOrdo_List = &m_pCMDI_Terrain->m_pFormRubTerrain->m_OrdoList;
         int                           nb = 0;
         for (QPL_CPosologie::iterator it = pOrdo_List->begin(); it != pOrdo_List->end(); ++it )
             {++ nb;
              (*it).m_NumOrdre = nb; // (*it).m_MEDICA_POSO_DIVERS n'est rempli avec le nom du produit que lors des conversions des nouvelles ordos vers anciennes
              plugin += G_pCApp->m_pCMedicaBase->Medica_PosologieToSimpleText(*it, m_pCMDI_Terrain->GetPoids(), m_pCMDI_Terrain->GetTaille(), (*it).m_MEDICA_POSO_DIVERS );
             }
        } //if (m_pCMDI_Terrain)
    }
 else if (m_pCMDI_Prescription && plugin==tr("InsérerOrdo"))
    {QPL_CPosologie  ordo_List;
     QStringList::iterator it;
     plugin                  = "";
     if ( m_pCMDI_Prescription->m_pFormRubPrescription->GetCurrentOrdoList(ordo_List ))
        {int              nb = 0;
         for (QPL_CPosologie::iterator it = ordo_List.begin(); it != ordo_List.end(); ++it )
             {++ nb;
              (*it).m_NumOrdre = nb; // (*it).m_MEDICA_POSO_DIVERS n'est rempli avec le nom du produit que lors des conversions des nouvelles ordos vers anciennes
              plugin += G_pCApp->m_pCMedicaBase->Medica_PosologieToSimpleText(*it, m_pCMDI_Terrain->GetPoids(), m_pCMDI_Terrain->GetTaille() , (*it).m_MEDICA_POSO_DIVERS);
             }
        }
    }
 else
    {plugin = tr("!=!");
    }
}
//------------------------------ Lap_NewDataToSimpleText ---------------------------
QStringList DrTux::Lap_NewDataToSimpleText(const QString &ordo)
{QStringList lst;
 return lst;
}
//------------------------------ Slot_ExeMixture ---------------------------
/*! \brief Envoie le texte mixture à CDevilCrucible pour qu'il soit interprété.
*/
void DrTux::Slot_ExeMixture(QString &mixture , CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)
{ int     rub_type            = 0;
  CRubRecord *pCRubRecordTemp = 0;
  QString     rubName         = "";
  if ( pCRubRecord==0 &&
       GetCurrentRubrique(&rubName,&rub_type, &pCRubRecordTemp)   // operation couteuse qu'il vaut mieux evierer
     ) pCRubRecord = pCRubRecordTemp;
  FusionneDocument(&mixture, G_pCApp->m_SignUser, pCRubRecord, currentRubIdMap);
}

//------------------------------ DocTypeSave ---------------------------------------
void DrTux::DocTypeSave(MyEditText *edit, QString &f_name)
{           QString       ext ("");
            int           pos = f_name.findRev ('.');
            if (pos !=-1) ext = f_name.mid(pos+1);

            if (! (ext.left(3).lower()=="htm") )
               {if (pos==-1) f_name += ".html";
                else         f_name  = f_name.left(pos) + ".html";
               }

            QFile file( f_name);
            if ( !file.open( IO_WriteOnly ) )    return;

            QString txt (edit->text());
            CHtmlTools::ElimineAncresRedondantes(txt);
            //CHtmlTools::setTextDefaultAtribut(txt, "background",    edit->m_PathImage);
            CHtmlTools::setTextDefaultAtribut(txt, "text",          edit->m_DefaultTextColor);
            CHtmlTools::setTextDefaultAtribut(txt, "bgcolor",       edit->m_DefaultBackgroundColor);
            CHtmlTools::QRichTextToHtml(txt);
            //file.writeBlock (txt , txt.length());

            QTextStream ts( &file );
            //ts.setCodec(QTextCodec::codecForName ( "iso-8859-1" ) );
            ts << txt;
            //................... si le path d'ecriture du texte n'est pas le meme ...........................
            //                    que celui du texte actuel (celui du chargement) alors changement de repertoire
            //                    alors recopier les images
            //QString dstDir = QFileInfo(f_name).dirPath (true);
            //if (edit->m_Path != dstDir )
               {//.............. copier le fond ...............................
                //if (edit->m_PathImage.length())
                //   {CHtmlTools::CopyFile(edit->m_Path + QDir::separator() + edit->m_PathImage, dstDir);
                //   }
                //.............. copier les images ............................
                //QStringList imgList;
                //CHtmlTools::getTextImageList(txt, imgList);
                //for ( QStringList::Iterator it = imgList.begin(); it != imgList.end(); ++it )
                //    {CHtmlTools::CopyFile(edit->m_Path + QDir::separator()+*(it), dstDir);
                //    }
               }

}
//------------------------------ Codage_CCAM ---------------------------
#define CARTOCUT 70
void DrTux::Codage_CCAM()
{Codage_CCAM(DrTux::InsertString);
}

//------------------------------ Codage_CCAM ---------------------------

QString DrTux::Codage_CCAM(int mode)
{
 QString ligne    = "";
 QString libelle  = "";
 QString carToCut = "";
 QString nbSpc    = "";
 QString text     = CCAM_Exe(this,  G_pCApp->m_PathAppli, G_pCApp->m_ID_Doss, G_pCApp->m_NumGUID, G_pCApp->m_DrTuxParam, G_pCApp->m_SignUser);
 QString str      = "";
 MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)                                  { return  QString("");}  // si pas d'editeur trouvé cassos
 if (text.length()>0)
    {//   LibellePrefix = Libellé  :
     //   LibellePostfix = Libellé  :
     //   CodePrefix = ::[
     //   CodePostfix = ::]
     QString libellePrefix, libellePostfix, CodePrefix, CodePostfix;
     CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CCAM", "LibellePrefix",  &libellePrefix );          // zero = pas d'erreur
     CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CCAM", "LibellePostfix", &libellePostfix );         // zero = pas d'erreur
     CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CCAM", "CodePrefix",     &CodePrefix );             // zero = pas d'erreur
     CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CCAM", "CodePostfix",    &CodePostfix );            // zero = pas d'erreur
     CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CCAM", "MiseEnPage",     &carToCut, &nbSpc );         // zero = pas d'erreur

     //................ attributs texte .................
     pMyEditText->setUnderline(FALSE);
     pMyEditText->setColor(QColor(0x00,0x00,0x00));

     //............... formatter le texte à rajouter pour ...................
     //                qu'il soit b� quoi
     // AHQB021 | 1 | 0 |  |  |  |  | Macroélectromyographie de fibre unique du périnée avec étude de 20 paires de fibres, par électrode aiguille
     // AHQD002 | 1 | 0 |  |  |  |  | Électromyographie analytique du périnée, avec étude des latences des réflexes sacrés, des latences distales du nerf pudendal [nerf honteux] par stimulation intrarectale et des potentiels évoqués somesthésiques cérébraux par stimulation du nerf pudendal

     long len     = text.length();
     int  toCut   = carToCut.toInt();
     if (len==0)                          return QString("");
     long deb     = 0;
     for (long i=0; i<len; ++i)
         {char cc =  text.at(i).latin1();
          if (i==len || cc=='\r' || cc=='\n')
            {ligne    = text.mid(deb, i-deb);
             ligne.replace("\r","");
             ligne.replace("\n","");
             QStringList list = QStringList::split('|',ligne,TRUE);
             if (list.count())
                {libelle       = list[7].stripWhiteSpace();
                 //............. inserer le code ......................
                 str += CodePrefix + list[0] + CodePostfix;
                 str += "&nbsp;&nbsp;&nbsp;&nbsp;" + tr("Activité: ") + list[1];
                 str += tr(" Phase: ") + list[2];
                 if (list[3].stripWhiteSpace().length()) str += tr(" Mod 1: ") + list[3];
                 if (list[4].stripWhiteSpace().length()) str += tr(" Mod 2: ") + list[4];
                 if (list[5].stripWhiteSpace().length()) str += tr(" Mod 3: ") + list[5];
                 if (list[6].stripWhiteSpace().length()) str += tr(" Mod 4: ") + list[6];

                 if( toCut<=10)         toCut = 70;
                 if (nbSpc.length()<=0) nbSpc = "&nbsp;&nbsp;&nbsp;&nbsp;";
                 int debL      = 0;
                 int pos       = 0;
                 int pos_space = toCut;    //QString libellePrefix, libellePostfix, CodePrefix, CodePostfix;
                 libelle.prepend(libellePrefix + " ");
                 int n = 0;
                 for (;;) {pos = libelle.findRev(' ', pos_space);
                           if (pos != -1)
                              {if (n) str += "<br> ";
                               str += nbSpc + libelle.mid(debL, pos-debL+1);
                               debL      = pos  + 1;
                               pos_space = debL + toCut;
                               ++n;
                              }
                           if (pos_space >= len)
                              {if (n) str += "<br>";
                               str += nbSpc + libelle.mid(debL);
                               ++n;
                               break;
                              }
                          }
                 str += libellePostfix;
                } // endif (pos != -1)

             ++i;
             if (text.at(i+1)==QChar('\n')) ++i;
             if (text.at(i+1)==QChar('\r')) ++i;
             deb = i;
            } //endif (i==len || text[i]=='\r' || text[i]=='\n')
        } // end for (long i=0; i<len; ++i)
    } // endif (text.length()>0)
 if (str.length() && mode==DrTux::InsertString) CHtmlTools::insertHtml(pMyEditText, str);
 return str;
}
//--------------------------------------- PluginExe --------------------------------------------------
/*! \brief un plugin est un executable communicant avec DrTux par l'intermediare du protocole suivant: DrTux appelle le plugin avec dans la ligne de commande :  en :
 *   0  Chemin de l'executable plugin à actionner
 *   1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
 *   2  Chemin de l'executable appelant
 *   3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour ce fichier est retourné modifié par le plugin, ce fichier peut etre un masque html ou tout autre délire en accord avec la syntaxe du plugin d�s fois que ... si ce fichier est vide, où n'existe pas le plugin retournera son résultat dans ce fichier d'échange que l'appelant récupera Le nom de ce fichier d'echange comporte : NomDuPlugin-GUID_Patient-
 *   4  GUID du patient
 *   5  PrimKey de l'observation en cours
 *   6  PrimKey du terrain en cours
 *   7  PrimKey de l'ordonnance en cours
 *   8  Primary key de l'utilisateur en cours
 *   9  Reservé
 *   10 Reservé
 *   11 Reservé
 *   12 Nb autres parametres d'appels specifiques au plugin
 *   13 --> 13+Nb parametres
*/
QString DrTux::PluginExe(       QObject         *pQObject,
                                const QString    &pathPlugin,  // Chemin de l'executable plugin à actionner sans l'extension .exe
                                const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                const QString    &maskExch,    // texte du fichier d'exchange
                                const QString    &guid,
                                const QString    &dosPk,
                                const QString    &terPk,
                                const QString    &ordPk ,
                                QStringList      &param,
                                int               waitFlag /* = CMoteurBase::endWait */)
{
 QString ret = "";
 //..................... copier le masque de retour dans le dossier temporaire ......................
 QString pathExe  = pathPlugin;
 if ( QDir::isRelativePath ( pathExe ) ) pathExe = QDir::cleanDirPath (pathExe.prepend(pathAppli) );
 QString nameExch = QFileInfo(pathExe).fileName()+"-"+ guid +".exc";
 QString      dst = "";
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Repertoire Temporaire", "Repertoire", &dst)!=0)
     return QObject::tr("Error:  PluginExe()  \"Repertoire Temporaire\", \"Repertoire\" can't find in ini File");         // path editeur de texte non défini
 if ( QDir::isRelativePath ( dst ) ) dst = QDir::cleanDirPath (dst.prepend(G_pCApp->m_PathAppli) );
 dst +=  QDir::separator() + nameExch;
 QFile::remove (dst);

 /*
 QFile qfsrc(pathExch);
 if (qfsrc.open(IO_ReadOnly)==FALSE)
    {QMessageBox::critical (0, QObject::tr("PluginExe(): read pathExch"),
                               QObject::tr("The file\r\n«")+ pathExch + QObject::tr("»\r\ncan't be opened"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     delete proc;
     return QObject::tr("Error:  PluginExe() cant't read 'pathExch' file");
    }
 */
 QFile qfdst(dst);
 if (qfdst.open(IO_ReadWrite)==FALSE)
    {QMessageBox::critical (0, QObject::tr("PluginExe(): write pathExch"),
                               QObject::tr("The file\r\n«")+ dst + QObject::tr("»\r\ncan't be opened"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     return QObject::tr("Error:  PluginExe() destination file can't be opened");
    }
 qfdst.writeBlock (maskExch, maskExch.length());
 qfdst.close();
 QString userPk = G_pCApp->m_pCMoteurBase->GetUserPrimKey(G_pCApp->m_SignUser);
 //..................... creer le process ...........................................................
 QProcess   proc( pQObject );
 QString    str = "";
 //......................... completer les autres arguments .........................................
 #ifdef Q_WS_MAC
    str = pathExe + ".app/Contents/MacOS/" + QFileInfo(pathExe).baseName();
 #else
    str = pathExe + F_EXT;
 #endif
 proc.addArgument( str );
 if ( !QFile::exists( str ) )
    { QMessageBox::critical (0,     QObject::tr("PluginExe(): Erreur exécution"),
                                    QObject::tr("L'exécutable : %1 n'existe pas.").arg( str) ,
                                    QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
    }
 proc.addArgument( pathIni );                         // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
 proc.addArgument( pathAppli + PROG_NAME );           // 2  Chemin de l'executable appelant
 proc.addArgument( dst );                             // 3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour
 proc.addArgument(guid );                             // 4  GUID du patient
 proc.addArgument(dosPk);                             // 5  PrimKey de l'observation en cours
 proc.addArgument(terPk);                             // 6  PrimKey du terrain en cours
 proc.addArgument(ordPk);                             // 7  PrimKey de l'ordonnance en cours
 proc.addArgument(userPk);                            // 8  Primary key de l'utilisateur en cours
 proc.addArgument(" ");                               // 9  reservé
 proc.addArgument(" ");                               // 10 reservé
 proc.addArgument(" ");                               // 11 reservé
 proc.addArgument(QString::number(param.count()));    // 12 Nb autres parametres d'appels specifiques au plugin
 for ( QStringList::Iterator it = param.begin(); it != param.end(); ++it )  // 13 --> 13+Nb parametres
     {proc.addArgument(*it);
     }
 //................................. executer le process .............................................
 if ( m_PluginRun == "" && proc.start() )
    { m_PluginRun = pathExe;
     qApp->processEvents ();
     while (waitFlag==CMoteurBase::endWait && proc.isRunning ())
      { // Afficher une messagebox pour informer l'utilisateur que drtux attends apr�s le plugins
        //QMessageBox * box = new QMessageBox(this);
        //box->setCaption(TR("Attente du plug-ins"));
	//box->setText(TR("Vous avez demandé l'exécution d'un plug-ins. DrTux attend que ce plug-ins lui renvoie ses informations."));
	//box->setButtonText(0, QString::null );
	//box->exec();
	//for (uint zzz = 0; zzz < 30 ; ++zzz)
	//  { if ( proc.isRunning () ) sleep (1);
	//    else break;
        //   }

        SLEEP (1);
        QTimer timer;               // pour etre sur qu'un evenement se produise
        timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
        QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
      }
     m_PluginRun = "";
     //............lire le fichier d'echange ..........................
     if ( !QFile::exists( dst ) )     return  ret;
     CGestIni::Param_UpdateFromDisk(dst, ret);
    }
  return ret;
}


//--------------------------------------- CCAM_Exe --------------------------------------------------
QString DrTux::CCAM_Exe( QObject *pQObject, QString /*pathAppli*/, QString identPrimKey, QString guid, const QString &local_param, const QString &user)
{
 //      [Codage CCAM]
 //           PathExe = /home/ro/Developpement/ccamview/bin/ccamview
 //           PathCCAM_Ini = Glossaire/CCAM/ccamview.ini
 //           SigemsUser = SEVI00
 //           SigemsLog  = DEFAULT
    /*
     QString pathExe, pathCCAM_Ini,sigemsUser,sigemsLog;
     QVariant qvar;
     QString arg;
     QString adr;
     QString ret("");
     //............ lire DrTux.ini ..........................

     if (Param_ReadParam(local_param, "Codage CCAM", "PathExe",      &pathExe)      !=0)   return ret;         // path executable ccam_view
     if (Param_ReadParam(local_param, "Codage CCAM", "PathCCAM_Ini", &pathCCAM_Ini) !=0)   return ret;         // path fichier ini ccam_view
     pathCCAM_Ini = pathCCAM_Ini.replace("$Glossaire",GlobalPathGlossaire);
     if (Param_ReadParam( "Codage CCAM", "SigemsUser",   &sigemsUser)   !=0)   sigemsUser=" ";     // parametres utilisateurs
     if (Param_ReadParam( "Codage CCAM", "SigemsLog",    &sigemsLog)    !=0)   sigemsLog="";       // parametres utilisateurs

     if (pathExe.find(pathAppli)!=0)      pathExe       = QDir::cleanDirPath (pathExe.prepend(pathAppli) );
     pathCCAM_Ini  = QDir(pathCCAM_Ini.replace("$Glossaire",GlobalPathGlossaire) ).absPath();

     QProcess*  proc = new QProcess( pQObject );
     if (proc==0)                                                          return ret;
     proc->addArgument( pathExe+F_EXT );

     proc->addArgument( pathCCAM_Ini );
     proc->addArgument(" ");                                                                                               // code patient 1
     proc->addArgument(" ");                                                                                               // code patient 2

     arg = GetFieldValue(m_DOSS_INDEX_TBL_NAME, m_DOSS_INDEX_NOM,     m_DOSS_INDEX_PRIM_KEY , identPrimKey);               // nom usuel ou marital
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");

     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_JFNOM,     m_DOSS_IDENT_REF_PK , identPrimKey);               // nom usuel ou marital
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");
     // proc->addArgument(" ");                                                                                            // nom de jeune fille

     arg = GetFieldValue(m_DOSS_INDEX_TBL_NAME, m_DOSS_INDEX_PRENOM,  m_DOSS_INDEX_PRIM_KEY , identPrimKey);               // prénom
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");

     GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_NSS, m_DOSS_IDENT_REF_PK , identPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     proc->addArgument(qDteNss.toString(QString("ddMMyyyy")));                                                             // date naissance

     QString sexe = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_SEX, m_DOSS_IDENT_REF_PK , identPrimKey);
     if (sexe=="F")     proc->addArgument("F");                                                                            // sexe
     else               proc->addArgument("M");

     QString adresse = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_RUE,        m_DOSS_IDENT_REF_PK , identPrimKey);  // adresse 1
     long len        = adresse.length();                                                                                   // adresse 2
     if (len!=0)
        {adresse.replace("'","~@~");
         long deb    = 0;
         int   nb    = 0;
         for (long u=0; u<=len; ++u)
             {char cc = adresse.at(u).latin1();
              if (u==len || cc=='\r' || cc=='\n')
                     {adr = adresse.mid(deb, u-deb);
                          proc->addArgument(adr);
                          nb++;
                          if (nb==2) break;
                      ++u;
                      if (adresse.at(u+1)=='\n') ++u;
                      if (adresse.at(u+1)=='\r') ++u;
                      deb = u;
                     }
             }
         if (nb==0) proc->addArgument(" ");
         if (nb==1) proc->addArgument(" ");
       }
     else
       {proc->addArgument(QObject::tr(" Adresse invalide"));
        proc->addArgument(QObject::tr(" Adresse invalide"));
       }
     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_CODE_POST,  m_DOSS_IDENT_REF_PK , identPrimKey);               // code postal
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");

     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_VILLE,     m_DOSS_IDENT_REF_PK , identPrimKey);               //ville
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");

     proc->addArgument(" ");                                                                                               // ville naissance

     QString clef(" ");
     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME,  m_DOSS_IDENT_NUMSS,      m_DOSS_IDENT_REF_PK , identPrimKey);              // numero de secu
     if (arg.length() >=13)
        {clef = (QString::number( 97 - fmod(atof(arg.left(13)) , 97)));
         if (clef.length()<2) clef.prepend('0');
        }
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     proc->addArgument(clef);                                                                                               // clef N° secu

     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_TEL1, m_DOSS_IDENT_REF_PK , identPrimKey);                     // tel 1
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_TEL2, m_DOSS_IDENT_REF_PK , identPrimKey);                     // tel 2
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_TEL3, m_DOSS_IDENT_REF_PK , identPrimKey);                     // tel 3
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     arg = GetFieldValue(m_DOSS_IDENT_TBL_NAME, m_DOSS_IDENT_EMAIL, m_DOSS_IDENT_REF_PK , identPrimKey);                     // Email
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");

     proc->addArgument("01011800 ");                                                                                        // date Entree Doss
     proc->addArgument("01012100 ");                                                                                        // date Sortie Doss

     proc->addArgument(user+":"+sigemsUser+":"+sigemsLog);                                                                  // utilisateur
     proc->addArgument(" ");                                                                                                // numero de dossier sigems
     proc->addArgument(guid);                                                                                               // numero de dossier Patient
     //............ determiner path du fichier d'echange .......................................
     QString path = pathCCAM_Ini;
     long     pos = path.findRev('/');
     if (pos != -1) path = path.left(pos+1);
     long tempo = 0;
     path += guid + ".txt";
         QFile::remove (path);   // effacer le fichier d'echange au cas où il en trainerait encore un
     if (m_CCAM_Run==FALSE && proc->start() )
        {m_CCAM_Run = TRUE;
             qApp->processEvents ();
             while(proc->isRunning () )
                {if (tempo>1000000) {qApp->processEvents (); tempo=0;}
                 tempo ++;
                }
             m_CCAM_Run = FALSE;
             //................. lire le fichier d'echange .....................................
         if ( !QFile::exists( path ) )  return   ret;
         QFile file( path );
         if ( !file.open( IO_ReadOnly ) )               return ret;
         QTextStream ts( &file );
         ret = ts.read();
         file.close();
         file.remove();    // effacer le fichier d'echange
        }
     delete proc;
     return ret;

     */
 //      [Codage CCAM]
 //           PathExe = /home/ro/Developpement/ccamview/bin/ccamview
 //           PathCCAM_Ini = Glossaire/CCAM/ccamview.ini
 //           SigemsUser = SEVI00
 //           SigemsLog  = DEFAULT

     QString pathExe, pathCCAM_Ini,sigemsUser,sigemsLog;
     QVariant qvar;
     QString arg;
     QString adr;
     QString ret("");
     //............ lire DrTux.ini ..........................
     // ici on va ruser pour ne pas modifier la structure en place je vais juste rajouter
     // le chemin relatif de demarrage de l'application puis la concatener avant à pathCCAM_Ini
     // puis faire un cleanDirPath par contre je génére le nom exec réel de ccamview
     // CGestIni* ConstruPathRelative = new CGestIni();
     QString tempoPath = CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     pathExe = CGestIni::Construct_Name_Exe("ccamview",QFileInfo (qApp->argv()[0]).dirPath (true));

     if (CGestIni::Param_ReadParam(local_param, "Codage CCAM", "PathCCAM_Ini", &pathCCAM_Ini) !=0)   return ret;         // path fichier ini ccam_view
     if (CGestIni::Param_ReadParam(USER_PARAM, "Codage CCAM", "SigemsUser",   &sigemsUser)   !=0)   sigemsUser=" ";     // parametres utilisateurs
     if (CGestIni::Param_ReadParam(USER_PARAM, "Codage CCAM", "SigemsLog",    &sigemsLog)    !=0)   sigemsLog="";       // parametres utilisateurs

     pathCCAM_Ini = QDir::cleanDirPath (tempoPath + pathCCAM_Ini);

     QProcess*  proc = new QProcess( pQObject );
     if (proc==0)                                                          return ret;
     proc->addArgument(pathExe);

     proc->addArgument( pathCCAM_Ini );
     proc->addArgument(" ");                                                                                               // code patient 1
     proc->addArgument(" ");                                                                                               // code patient 2

     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_NOM,     G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_PRIM_KEY , identPrimKey);               // nom usuel ou marital
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");

     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_JFNOM,     G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);               // nom usuel ou marital
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");
     // proc->addArgument(" ");                                                                                            // nom de jeune fille

     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_PRENOM,  G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_PRIM_KEY , identPrimKey);               // prénom
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");

     G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NSS, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     proc->addArgument(qDteNss.toString(QString("ddMMyyyy")));                                                             // date naissance

     QString sexe = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_SEX, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);
     if (sexe=="F")     proc->addArgument("F");                                                                            // sexe
     else               proc->addArgument("M");

     QString adresse = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_RUE,        G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);  // adresse 1
     long len        = adresse.length();                                                                                   // adresse 2
     if (len!=0)
        {adresse.replace("'","~@~");
         long deb    = 0;
         int   nb    = 0;
         for (long u=0; u<=len; ++u)
             {char cc = adresse.at(u).latin1();
              if (u==len || cc=='\r' || cc=='\n')
                     {adr = adresse.mid(deb, u-deb);
                          proc->addArgument(adr);
                          nb++;
                          if (nb==2) break;
                      ++u;
                      if (adresse.at(u+1)=='\n') ++u;
                      if (adresse.at(u+1)=='\r') ++u;
                      deb = u;
                     }
             }
         if (nb==0) proc->addArgument(" ");
         if (nb==1) proc->addArgument(" ");
       }
     else
       {proc->addArgument(QObject::tr(" Adresse invalide"));
        proc->addArgument(QObject::tr(" Adresse invalide"));
       }
     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_CODE_POST,  G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);               // code postal
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");

     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_VILLE,     G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);               //ville
     if (arg.length()>0) proc->addArgument(arg.replace("'","~@~"));
     else                proc->addArgument(" ");

     proc->addArgument(" ");                                                                                               // ville naissance

     QString clef(" ");
     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME,  G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NUMSS,      G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);              // numero de secu
     if (arg.length() >=13)
        {clef = (QString::number( 97 - fmod(atof(arg.left(13)) , 97)));
         if (clef.length()<2) clef.prepend('0');
        }
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     proc->addArgument(clef);                                                                                               // clef N° secu

     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL1, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);                     // tel 1
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL2, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);                     // tel 2
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL3, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);                     // tel 3
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");
     arg = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_EMAIL, G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_REF_PK , identPrimKey);                     // Email
     if (arg.length()>0) proc->addArgument(arg);
     else                proc->addArgument(" ");

     proc->addArgument("01011800 ");                                                                                        // date Entree Doss
     proc->addArgument("01012100 ");                                                                                        // date Sortie Doss

     proc->addArgument(user+":"+sigemsUser+":"+sigemsLog);                                                                  // utilisateur
     proc->addArgument(" ");                                                                                                // numero de dossier sigems
     proc->addArgument(guid);                                                                                               // numero de dossier Patient
     //.......................... choix du panneau à activver lors dans le dialogue CCAM_View ....................................................
     if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Codage CCAM", "Panneau a activer", &arg )==0)   // zero = pas d'erreur
        proc->addArgument(arg);
     else
        proc->addArgument("&Code CCAM");

     //............ determiner path du fichier d'echange .......................................
     QString path = pathCCAM_Ini;
     long     pos = path.findRev('/');
     if (pos != -1) path = path.left(pos+1);
     //long tempo = 0;
     path += guid + ".txt";
     QFile::remove (path);   // effacer le fichier d'echange au cas où il en trainerait encore un
     if (m_CCAM_Run==FALSE && proc->start() )
        {m_CCAM_Run = TRUE;
             qApp->processEvents ();
             while(proc->isRunning () )
                {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
                }
             m_CCAM_Run = FALSE;
             //................. lire le fichier d'echange .....................................
         if ( !QFile::exists( path ) )                  return ret;
         QFile file( path );
         if ( !file.open( IO_ReadOnly ) )               return ret;
         CGestIni::Param_UpdateFromDisk(path, ret);
         file.close();
         file.remove();    // effacer le fichier d'echange
        }
     delete proc;
     return ret;
 }

//-------------------------------------------- ListManager ------------------------
void DrTux::ListManager()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText)
    {pMyEditText->m_list_name     = pMyEditText->GetLastList();
     pMyEditText->ListManager();
    }
 else
   {C_DlgListFieldMngr *dlg = new C_DlgListFieldMngr(this, "ListManager_Dial", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
    if (dlg ==0) return;
    dlg->initDialog(G_pCApp->m_DrTuxParam, G_pCApp->m_PathGlossaire, G_pCApp->m_PathAppli);
    dlg->exec();
    recordPathModulesMenus(dlg->m_PathMenuLib);
    delete dlg;
   }
}

//-------------------------------------------- CreateList ------------------------
void DrTux::CreateList()
{//G_pCApp->m_pCMoteurBase->UpdateRubUserName();
 MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 pMyEditText->PopupCreateList();
}


//-------------------------------------------- DisplayContextListPopup ------------------------
void DrTux::DisplayContextListPopup()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 pMyEditText->DoContextListPopup();
}

//-------------------------------------------- FonctionsAttributsTexte ------------------------------------------------------------

void DrTux::textFamily( const QString & f)             //............... positionner le texte sur les attributs textes ..........................
{   if ( !currentEditor() )  return;
    currentEditor()->setFamily( f );
    currentEditor()->viewport()->setFocus();
}
void DrTux::textItalic()
{   if ( !currentEditor() )  return;
    currentEditor()->setItalic( m_pActionTextItalic->isOn() );
}
void DrTux::textBold()
{   if ( !currentEditor() )  return;
    currentEditor()->setBold( m_pActionTextBold->isOn() );
}
void DrTux::textUnderline()
{   if ( !currentEditor() )  return;
    currentEditor()->setUnderline( m_pActionTextUnderline->isOn() );
}
void DrTux::textColor()
{   if ( !currentEditor() )                                  return;
    m_LasTextColorChoice = QColorDialog::getColor( currentEditor()->color(), this );
    if ( !m_LasTextColorChoice.isValid() )                   return;
    currentEditor()->setColor( m_LasTextColorChoice );
    QPixmap pix( 16, 16 );
    pix.fill( m_LasTextColorChoice );
    m_pActionTextColor->setIconSet( pix );
    QString value;
    CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Valerie", "Couleur rapide", &value);   // path editeur de texte non d�fini
    if (value.length()==0)
       {QPixmap pix2( 4, 16 );
        pix2.fill( m_LasTextColorChoice );
        m_pActionFastTextColor->setIconSet( pix2 );
       }
}

//---------------------------------- fast_textColor ----------------------------
/*! \brief mise en place rapide de la couleur texte
*/
void DrTux::fast_textColor()
{if ( !currentEditor() )                   return;
 QString value;
 CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Valerie", "Couleur rapide", &value);   // path editeur de texte non d�fini
 if (value.length()) m_LasTextColorChoice = QColor(value);
 if ( !m_LasTextColorChoice.isValid() )    return;
    currentEditor()->setColor( m_LasTextColorChoice );
    QPixmap pix( 16, 16 );
    pix.fill( m_LasTextColorChoice );
    m_pActionTextColor->setIconSet( pix );
}

void DrTux::textSize( const QString & p)
{   if ( !currentEditor() )  return;
    currentEditor()->setPointSize( p.toInt() );
    currentEditor()->viewport()->setFocus();
}
void DrTux::textAlign( QAction* a)
{   if ( !currentEditor() )  return;
    if ( a == m_pActionAlignLeft )             currentEditor()->setAlignment( AlignLeft );
    else if ( a == m_pActionAlignCenter )      currentEditor()->setAlignment( AlignHCenter );
    else if ( a == m_pActionAlignRight )       currentEditor()->setAlignment( AlignRight );
    else if ( a == m_pActionAlignJustify )     currentEditor()->setAlignment( AlignJustify );
}
void DrTux::textStyle( int i)
{   if ( !currentEditor() )  return;
    if      ( i == 0 ) currentEditor()->setParagType( QStyleSheetItem::DisplayBlock,    QStyleSheetItem::ListDisc );
    else if ( i == 1 ) currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListDisc );
    else if ( i == 2 ) currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListCircle );
    else if ( i == 3 ) currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListSquare );
    else if ( i == 4 ) currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListDecimal );
    else if ( i == 5 ) currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListLowerAlpha );
    else if ( i == 6 ) currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListUpperAlpha );
    currentEditor()->viewport()->setFocus();
}
void DrTux::editUndo()
{   if ( !currentEditor() ) return;
    currentEditor()->undo();
}
void DrTux::editRedo()
{   if ( !currentEditor() ) return;
    currentEditor()->redo();
}
void DrTux::editCut()
{   if ( !currentEditor() ) return;
    currentEditor()->cut();
}
void DrTux::editCopy()
{   if ( !currentEditor() ) return;
    currentEditor()->copy();
}
void DrTux::editPaste()
{   if ( !currentEditor() ) return;
    currentEditor()->paste();
}
void DrTux::editSelectAll()
{   if ( !currentEditor() ) return;
    currentEditor()->selectAll();
}
void DrTux::editorChanged( QWidget * )                 //........... Positionner les boutons sur les attributs texte ...............
{   if ( !currentEditor() ) return;
    fontChanged( currentEditor()->font() );
    colorChanged( currentEditor()->color() );
    alignmentChanged( currentEditor()->alignment() );
}
void DrTux::fontChanged( const QFont &f )
{   m_comboFont->lineEdit()->setText( f.family() );
    m_comboSize->lineEdit()->setText( QString::number( f.pointSize() ) );
    m_pActionTextBold->setOn( f.bold() );
    m_pActionTextItalic->setOn( f.italic() );
    m_pActionTextUnderline->setOn( f.underline() );
}
void DrTux::colorChanged( const QColor &c )
{   QPixmap pix( 16, 16 );
    pix.fill( c );
    m_pActionTextColor->setIconSet( pix );
}
void DrTux::alignmentChanged( int a )
{   if      ( ( a == AlignAuto ) || ( a & AlignLeft ))        m_pActionAlignLeft->setOn( TRUE );
    else if ( ( a & AlignHCenter ) )                          m_pActionAlignCenter->setOn( TRUE );
    else if ( ( a & AlignRight ) )                            m_pActionAlignRight->setOn( TRUE );
    else if ( ( a & AlignJustify ) )                          m_pActionAlignJustify->setOn( TRUE );
}

//------------------------------- OnMenuQuitterDrTux ----------------------------
void DrTux::OnMenuQuitterDrTux()
{if (SauverDossierAvantNouvelleAction())  emit Sign_QuitterDrTux();
}

//------------------------------- OnQuitterDrTux ----------------------------
void DrTux::OnQuitterDrTux()
{ emit Sign_QuitterDrTux();
}

//------------------------------ closeEvent ---------------------------------
// Si ce->accept();  alors ensuite va dans: OnQuitterDrTux()
// à cause de : a.connect( &a,SIGNAL(lastWindowClosed()),  pDrTux, SLOT(OnQuitterDrTux()) );
void DrTux::closeEvent( QCloseEvent* ce )
{
 if (SauverDossierAvantNouvelleAction())  //  0-ne pas fermer  1-enregistrer et fermer  2-fermer sans enregistrer
    {Slot_SauverLesMeubles();
     ce->accept();
    }
 else
    {ce->ignore();
    }
}

//------------------------ SauverDossierAvantNouvelleAction -----------------------
int DrTux::SauverDossierAvantNouvelleAction(SAVE_Mode mode /*= DrTux::Inquire*/)
{  // Si n'a pas autorisation à la modification on sort
   if (m_IsModifiable==0) return 2;
   // Placer le texte en cours dans les CMDI dans la liste si besoin
   if (m_pCMDI_Observation)  m_pCMDI_Observation->IfModified_SaveInRubList();
   if (m_pCMDI_Prescription) m_pCMDI_Prescription->IfModified_SaveInRubList();
   if (m_pCMDI_Terrain)      m_pCMDI_Terrain->IfModified_SaveInRubList();
   if (m_pCMDI_Ident)        m_pCMDI_Ident->IfModified_SaveInRubList();
   //................ rubriques automatiques .......................
   QMap<QString, CMDI_Observation*>::Iterator it;
   for ( it = G_pCApp->m_mapNameRubPtr.begin(); it != G_pCApp->m_mapNameRubPtr.end(); ++it )
       {  CMDI_Observation *pCMDI_Observation = it.data();
          if (pCMDI_Observation) pCMDI_Observation->IfModified_SaveInRubList();
       }

   int nb = 0;
   if (G_pCApp->m_pCMoteurBase) nb = G_pCApp->m_pCMoteurBase->RubNbToSave(&m_RubList);
   //...................... executer les eventuels script de sortie .................................
   C_UserQAction *pC_UserQAction;
   QString       ret = "";
   int   isEndScript = 0;
   int   actionQuit  = 0;   //  ne pas quitter pQAction->m_Sate
   for ( pC_UserQAction = m_pCustomActionTab.first(); pC_UserQAction; pC_UserQAction = m_pCustomActionTab.next() )
       {QString  script = pC_UserQAction->getScript();
        if (script.find("{{EXECUTE_ON_EXIT}}") != -1)
           {      script = script.remove("{{EXIT}}"); // virer la mention car faut pas boucler sur l'appel recursif
                  ret  += On_CustomAction(script, pC_UserQAction);
                  ++isEndScript;
           }
       }
    //............. vérifier que les scripts de fin ne demandent pas à quitter
    if (isEndScript)
       { if (ret.find("QUIT") != -1)
            {if (ret.find("ET ENREGISTRER") != -1) actionQuit = 1;   // quitter et enregistrer
             else                                  actionQuit = 2;   // quitter sans enregistrer
            }
       }
    if (actionQuit==0)          // n'a pas ete modifiee par le script de sortie donc procedure de sortie classique
       {
        actionQuit = 1;         // quitter et enregistrer
        if (nb && mode==DrTux::Inquire)
           {actionQuit = QMessageBox::question( this, tr("Fermeture du dossier actif"),
                                                      tr( "Ce dossier en cours a été modifié, faut-il :\r\n"
                                                          "enregistrer les modifications ?"),
                                                      tr("&Ne pas fermer"),                            // 0
                                                      tr("&Enregistrer et fermer") ,                   // 1
                                                      tr("&Fermer sans enregistrer") , 0, 0 );         // 2
           }  // important : bouton par defaut lorsque abandon par escape ou case fermeture fenetre ____________^
        }
     if (actionQuit==1 && nb) OnDrTuxSaveRubList();
     return actionQuit;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////    CREATION DES CMDI    /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------ CMDI_ChoixPatientCreate  ------------------------------------------------
CMDI_ChoixPatient*  DrTux::CMDI_ChoixPatientCreate(const char* num_GUID,          const char* id_doss,
                                                   const char* dossNom ,          const char* dossPrenom, const char* user, QString rubName)
{
  m_pCMDI_ChoixPatient = new CMDI_ChoixPatient(m_pQWorkSpaceRub, rubName, WDestructiveClose, G_pCApp->m_pCMoteurBase, &m_RubList,
                                                                 num_GUID, id_doss, dossNom, dossPrenom, user, G_pCApp->RubNameToType(rubName) );
  if (m_pCMDI_ChoixPatient==0) return 0;
  //................................. un peu de geometrie ........................................
  //m_pCMDI_ChoixPatient->setGeometry (QRect (0,0,210,823));
  m_pCMDI_ChoixPatient->setGeometry (QRect (0,0,400,363) , CMDI_Generic::CFG_PARAM|CMDI_Generic::RECT_PARAM);
  //.................. creer tabulation correspondante ..........................................
  //if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( CMDI_ChoixPatient::S_GetRubName(), Theme::getIcon("16x16/type_patientlist.png") );
  if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( rubName , Theme::getIcon(QString("16x16/type_")+rubName+".png") );
  //.................. connecter DrTux aux fenetres filles: Sign_ActiverRubrique ......................................
  //                   elles pouront recevoir le message d'activation de leur fenetre
  //                   repercuté par DrTux
  connect( this,                    SIGNAL( Sign_ActiverRubrique(const char*, int)),  m_pCMDI_ChoixPatient,    SLOT( ActiverRubrique(const char*, int))  );
  //.................. connecter DrTux aux fenetres filles: Sign_OnUserChange ......................................
  //                   elles pouront recevoir le message de changement d'utilisateur

  connect( this,                    SIGNAL(Sign_OnUserChange(const char* , const char*)),
           m_pCMDI_ChoixPatient,    SLOT( RubOnUserChange(const char*, const char*))
         );
  //.................. connecter les filles au DrTux.............................................
  //                   elles pouront envoyer et notifier un changement d'identité
  //                   au DrTux qui le repercutera aux autres filles
  connect( m_pCMDI_ChoixPatient->m_pFormRubChoixPatient, SIGNAL( Sign_IdentChange(const char*,  const char*, const char*,  const char* )),
           this,                                         SLOT( OnDrTuxIdentChange(const char*,  const char*, const char*,  const char* ))
         );
  //.................. connecter au DrTux.............................................
  //                   pour qu'une demande d'effacement de dossier lui soit communique
  connect( m_pCMDI_ChoixPatient->m_pFormRubChoixPatient, SIGNAL( Sign_IdentDelete(const char*,  const char*, const char*,  const char* )),
           this,                                         SLOT( OnDrTuxIdentDelete(const char*,  const char*, const char*,  const char* ))
         );
  //.................. connecter au DrTux.............................................
  //                   pour qu'une demande de N° de dossier lui soit demandé par ChoixPatient
  connect( m_pCMDI_ChoixPatient->m_pFormRubChoixPatient, SIGNAL( Sign_GetActiveGUID(QString &)),
           this,                                         SLOT(   Slot_GetActiveGUID(QString &))
         );

  //.................. connecter au DrTux.............................................
  //                   pour qu'une demande de lancement de dossier lui soit communique
  connect( m_pCMDI_ChoixPatient->m_pFormRubChoixPatient, SIGNAL( Sign_DrTuxExeAnother(QString,  QString, QString,  QString )),
           this,                                         SLOT( OnDrTuxExeAnother(QString,  QString, QString,  QString ))
         );
  //.................. connecter les filles au DrTux.............................................
  //                   elles pouront envoyer et notifier leur destruction
  //                   au DrTux qui l'enl�vera de la barre d'onglets
  connect( m_pCMDI_ChoixPatient,   SIGNAL( Sign_RubIsDeleted(const char* )),
           this,                   SLOT  ( OnDeleteRubrique (const char* ))
         );
  //.................. connecter le bouton NouveauDossier au DrTux.............................................
  connect( m_pCMDI_ChoixPatient->m_pFormRubChoixPatient,   SIGNAL( Sign_CreateNewIdent()),
           this,                                           SLOT  ( OnCreateNewIdent())
         );

  //.................. connecter la recherche depuis Dock_Menu avec cette CMDI ................................
  if (m_pForm_Menu)
  { connect( m_pForm_Menu,   SIGNAL( Sign_SearchPatient( QString )),
           m_pCMDI_ChoixPatient->m_pFormRubChoixPatient, SLOT  ( Slot_SearchFor( QString ))
         );
  }
  //........................ connecter la statut bar ..............................................
  //connect( pCMDI_ChoixPatient, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

  return m_pCMDI_ChoixPatient;
}


//-------------------------------------- CMDI_RubriqueCreate  -------------------------------------
CMDI_Observation* DrTux::CMDI_RubriqueCreate (const char* num_GUID,          const char* id_doss,
                                              const char* dossNom ,          const char* dossPrenom, const char* user, QString rubName, QString prDroits)
{   if (!G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,prDroits+"v")) return 0;
    CMDI_Observation *pCMDI_Observation = new CMDI_Observation(m_pQWorkSpaceRub, rubName, WDestructiveClose, G_pCApp->m_pCMoteurBase, &m_RubList,
                                                               num_GUID, id_doss, dossNom, dossPrenom, user, G_pCApp->RubNameToType(rubName) , prDroits);

    if (pCMDI_Observation==0) return 0;
    QString val = "-1";
    if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, rubName, "Largeur visualisation", &val) !=0 )  // zero = pas d'erreur
       {pCMDI_Observation->setWordWrap(-1);
       }
    else
       {pCMDI_Observation->setWordWrap(val.toInt());
       }
    pCMDI_Observation->SetModifiableState(m_IsModifiable);

    //................................. un peu de geometrie ........................................
    pCMDI_Observation->setGeometry (QRect (0,0,400,363) , CMDI_Generic::CFG_PARAM|CMDI_Generic::RECT_PARAM);

    //.................. creer tabulation correspondante ...............................................
    if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( rubName , Theme::getIcon(QString("16x16/type_")+rubName+".png") );
    // connecter l'editeur de texte aux boutons d'indication d'etat
    doConnections( pCMDI_Observation->GetCurrentEditor() );
    //connect(pCMDI_Observation->m_pMyEditText, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

    //.................. connecter DrTux aux fenetres filles:  Sign_OnIdentChange ..................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par DrTux pour modifier leur contenu
    connect( this,                   SIGNAL( Sign_OnIdentChange(const char*,  const char*, const char*,  const char* )),
             pCMDI_Observation,      SLOT( OnIdentChange(const char*,  const char*, const char*,  const char* ))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_OnUserChange .......................
    //                   elles pouront recevoir le message de changement d'utilisateur

    connect( this,                  SIGNAL(Sign_OnUserChange(const char* , const char*)),  pCMDI_Observation,      SLOT( RubOnUserChange(const char*, const char*))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_ActiverRubrique ...................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par DrTux
    connect( this,                   SIGNAL( Sign_ActiverRubrique(const char*, int)),  pCMDI_Observation,    SLOT( ActiverRubrique(const char*, int))   );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au DrTux qui l'enlèvera de la barre d'onglets
    connect( pCMDI_Observation,   SIGNAL( Sign_RubIsDeleted(const char* )),  this,                SLOT  ( OnDeleteRubrique (const char* ))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de sauvegarde
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( pCMDI_Observation->m_pC_RubObservation,     SIGNAL( Sign_SaveButtonClicked()),
             this,                                       SLOT  ( OnDrTuxSaveRubList())
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir d'imprimer
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( pCMDI_Observation,   SIGNAL( Sign_Print()),   this,                  SLOT  ( filePrint()) );
    //.................. connecter les plugin de l'editeur au DrTux.............................................
    //                   pour qu'il puisse les executer

    // connect( pCMDI_Observation->m_pC_RubObservation->m_pMyEditText,   SIGNAL( Sign_Exe_Mixture(QString&, CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)),
    //         this,                                                    SLOT  ( Slot_ExeMixture(QString&,  CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap))
    //        );
    //...................................... zoom par defaut .....................................................
    //QString zoom;
    //if (CGestIni::Param_ReadParam(m_DrTuxParam, "Observation", "Zoom", &zoom)==0)  // zero = pas d'erreur
    //pCMDI_Observation->SetZoom(zoom.toInt());

    pCMDI_Observation->DoConnexionOnGossaire(m_pFormGlossaire);
    return pCMDI_Observation;
}



//------------------------------ CMDI_PrescriptionCreate  ------------
CMDI_Prescription* DrTux::CMDI_PrescriptionCreate (const char* num_GUID,          const char* id_doss,
                                                   const char* dossNom ,          const char* dossPrenom , const char* user, QString rubName)
{

    m_pCMDI_Prescription = new CMDI_Prescription(m_pQWorkSpaceRub, rubName, WDestructiveClose, G_pCApp->m_pCMoteurBase, &m_RubList,
                                                                   num_GUID, id_doss, dossNom, dossPrenom, user, G_pCApp->RubNameToType(rubName));
    if (m_pCMDI_Prescription==0) return 0;
    QString val = "-1";
    if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Prescription", "Largeur visualisation", &val) !=0 )  // zero = pas d'erreur
       {m_pCMDI_Prescription->setWordWrap(-1);
       }
    else
       {m_pCMDI_Prescription->setWordWrap(val.toInt());
       }

    m_pCMDI_Prescription->SetModifiableState(m_IsModifiable);
    //........................ connecter la statut bar ..............................................
    //connect( pCMDI_Prescription, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );
    //................................. un peu de geometrie ........................................
    //m_pCMDI_Prescription->setGeometry (QRect (0,423,600,400));
    m_pCMDI_Prescription->setGeometry (QRect (0,0,400,363) , CMDI_Generic::CFG_PARAM|CMDI_Generic::RECT_PARAM);

    //.................. creer tabulation correspondante ...............................................
    //if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( CMDI_Prescription::S_GetRubName() , Theme::getIcon("16x16/type_prescription.png"));
    if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( rubName , Theme::getIcon(QString("16x16/type_")+rubName+".png") );
    doConnections( m_pCMDI_Prescription->m_pMyEditText );  // connecter l'editeur de texte aux boutons d'indication d'etat
    //.................. connecter DrTux aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par DrTux pour modifier leur contenu
    connect( this,                    SIGNAL( Sign_OnIdentChange(const char*,  const char*, const char*,  const char* )),
             m_pCMDI_Prescription,    SLOT( OnIdentChange(const char*,  const char*, const char*,  const char* ))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_OnUserChange ......................................
    //                   elles pouront recevoir le message de changement d'utilisateur

    connect( this,                   SIGNAL(Sign_OnUserChange(const char* , const char*)),
            m_pCMDI_Prescription,    SLOT( RubOnUserChange(const char*, const char*))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par DrTux
    connect( this,                    SIGNAL( Sign_ActiverRubrique(const char*, int)),
             m_pCMDI_Prescription,    SLOT( ActiverRubrique(const char*, int))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au DrTux qui l'enl�vera de la barre d'onglets
    connect( m_pCMDI_Prescription,   SIGNAL( Sign_RubIsDeleted(const char* )),
             this,                   SLOT  ( OnDeleteRubrique (const char* ))
           );

    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de sauvegarde
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Prescription->m_pFormRubPrescription,   SIGNAL( Sign_SaveButtonClicked()),
             this,                                           SLOT  ( OnDrTuxSaveRubList())
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de duplication de données
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Prescription->m_pFormRubPrescription,   SIGNAL( Sign_DuplicateData(RUBREC_LIST::iterator,          QDateTime &, const QString &, const QString&)),
             this,                                           SLOT  ( DuplicateRecordWithNewDate(RUBREC_LIST::iterator , QDateTime &, const QString &, const QString&))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de duplication de données
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Prescription->m_pFormRubPrescription,   SIGNAL( Sign_PutPrescriptionInTerrain(RUBREC_LIST::iterator)),
             this,                                           SIGNAL( Sign_DrTux_PutPrescriptionInTerrain(RUBREC_LIST::iterator ))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de duplication de données
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Prescription->m_pFormRubPrescription,   SIGNAL( Sign_Renouveler_TTT_Fond()),
             this,                                           SIGNAL( Sign_DrTux_Renouveler_TTT_Fond())
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de duplication de données
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Prescription->m_pFormRubPrescription,   SIGNAL( Sign_CreateAllCurrentsTTT()),
             this,                                           SLOT  ( Slot_CreateAllCurrentsTTT())
           );

    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir d'imprimer
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Prescription,   SIGNAL( Sign_Print()),
             this,                   SLOT  ( filePrint())
           );
    //.................. connecter les plugin de l'editeur au DrTux.............................................
    //                   pour qu'il puisse les executer

   // connect( m_pCMDI_Prescription->m_pMyEditText,   SIGNAL( Sign_Exe_Mixture(QString&, CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)),
   //          this,                                  SLOT  ( Slot_ExeMixture(QString&,  CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap))
   //        );
    m_pCMDI_Prescription->DoConnexionOnGossaire(m_pFormGlossaire);
    //...................................... zoom par defaut .....................................................
    //QString zoom;
    //if (CGestIni::Param_ReadParam(m_DrTuxParam,  "Prescription", "Zoom", &zoom)==0)  // zero = pas d'erreur
    //pCMDI_Prescription->SetZoom(zoom.toInt());
    return m_pCMDI_Prescription;

}

//------------------------------ CMDI_TerrainCreate  ------------
CMDI_Terrain* DrTux::CMDI_TerrainCreate (const char* num_GUID,          const char* id_doss,
                                         const char* dossNom ,          const char* dossPrenom , const char* user, QString rubName)
{

    m_pCMDI_Terrain = new CMDI_Terrain(m_pQWorkSpaceRub, rubName, WDestructiveClose, G_pCApp->m_pCMoteurBase, &m_RubList,
                                       num_GUID, id_doss, dossNom, dossPrenom, user, G_pCApp->RubNameToType(rubName));
    if (m_pCMDI_Terrain==0) return 0;
    m_pCMDI_Terrain->SetModifiableState(m_IsModifiable);
    //........................ connecter la statut bar ..............................................
    //connect(pCMDI_Terrain, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

    //................................. un peu de geometrie ........................................
    //m_pCMDI_Terrain->setGeometry (QRect (0,0,600,400));
    m_pCMDI_Terrain->setGeometry (QRect (0,0,400,363) , CMDI_Generic::CFG_PARAM|CMDI_Generic::RECT_PARAM);

    //.................. creer tabulation correspondante ...............................................
    //if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( CMDI_Terrain::S_GetRubName() , Theme::getIcon("16x16/type_terrain.png"));
    if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( rubName , Theme::getIcon(QString("16x16/type_")+rubName+".png") );
    //doConnections( pCMDI_Terrain->m_pMyEditText );  // connecter l'editeur de texte aux boutons d'indication d'etat
    //connect(pCMDI_Terrain->m_pMyEditText, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

    //.................. connecter DrTux aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par DrTux pour modifier leur contenu
    connect( this,                 SIGNAL( Sign_OnIdentChange(const char*,  const char*, const char*,  const char* )),
             m_pCMDI_Terrain,      SLOT( OnIdentChange(const char*,  const char*, const char*,  const char* ))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_OnUserChange ......................................
    //                   elles pouront recevoir le message de changement d'utilisateur

    connect( this,            SIGNAL(Sign_OnUserChange(const char* , const char*)),
            m_pCMDI_Terrain,  SLOT( RubOnUserChange(const char*, const char*))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par DrTux
    connect( this,                 SIGNAL( Sign_ActiverRubrique(const char*, int)),
             m_pCMDI_Terrain,      SLOT( ActiverRubrique(const char*, int))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au DrTux qui l'enl�vera de la barre d'onglets
    connect( m_pCMDI_Terrain, SIGNAL( Sign_RubIsDeleted(const char* )),
             this,            SLOT  ( OnDeleteRubrique (const char* ))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de sauvegarde
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Terrain->m_pFormRubTerrain,     SIGNAL( Sign_SaveButtonClicked()),
             this,                                   SLOT  ( OnDrTuxSaveRubList())
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir d'imprimer
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Terrain,   SIGNAL( Sign_Print()),
             this,              SLOT  ( filePrint())
           );
    //.................. connecter le terrain au DrTux.............................................
    //                    pour renouveler ordo à partir du TTT de fond
    connect( m_pCMDI_Terrain->m_pFormRubTerrain,   SIGNAL( Sign_Renouveler(const QString &, int , const char * )),
             this,                                 SLOT  ( AddNewRecordToRubrique(const QString &, int , const char * ))
           );

    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de duplication de données
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( this,                                  SIGNAL( Sign_DrTux_PutPrescriptionInTerrain(RUBREC_LIST::iterator)),
             m_pCMDI_Terrain->m_pFormRubTerrain,    SLOT  ( TTT_PutPrescriptionInTerrain(RUBREC_LIST::iterator))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de duplication de données
    //                   de leur rubrique au DrTux qui fera ce que de droit
    // connect( this,                                           SIGNAL( Sign_DrTux_Renouveler_TTT_Fond()),
    //          m_pCMDI_Terrain->m_pFormRubTerrain,             SLOT( TTT_MenuActionRenouveler())
    //        );
    m_pCMDI_Terrain->DoConnexionOnGossaire(m_pFormGlossaire);
    return m_pCMDI_Terrain;
}

//------------------------------ CMDI_IdentCreate -------------------------------------
CMDI_Ident* DrTux::CMDI_IdentCreate (const char* num_GUID,          const char* id_doss,
                                     const char* dossNom ,          const char* dossPrenom, const char* user, QString rubName)
{   if (!G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"idv")) return 0;
    //................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
    m_pCMDI_Ident = new CMDI_Ident(m_pQWorkSpaceRub, CMDI_Ident::S_GetRubName(), WDestructiveClose, G_pCApp->m_pCMoteurBase, &m_RubList,
                                   num_GUID, id_doss, dossNom, dossPrenom, user, G_pCApp->RubNameToType(rubName));
    m_pCMDI_Ident->SetModifiableState(m_IsModifiable);
    //........................ connecter la statut bar ..............................................
    //connect( pCMDI_Ident, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

    //................................. un peu de geometrie ........................................
    //m_pCMDI_Ident->setGeometry (QRect (0,0,400,363),0);
    m_pCMDI_Ident->setGeometry (QRect (0,0,400,363) , CMDI_Generic::CFG_PARAM|CMDI_Generic::RECT_PARAM);

    m_pCMDI_Ident->m_pFormRubIdent->SetDefaultMaskFile(m_pCMDI_Ident->GetDefaultMaskFile());

    //.................. creer tabulation correspondante ...............................................
    //if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( CMDI_Ident::S_GetRubName() , Theme::getIcon("16x16/type_ident.png"));
    if (m_pFormOngletRubrique) m_pFormOngletRubrique->addPanel ( rubName , Theme::getIcon(QString("16x16/type_")+rubName+".png") );
    doConnections( m_pCMDI_Ident->GetCurrentEditor() );
    //.................. connecter DrTux aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par DrTux pour modifier leur contenu
    connect( this,           SIGNAL( Sign_OnIdentChange(const char*,  const char*, const char*,  const char* )),
             m_pCMDI_Ident,  SLOT( OnIdentChange(const char*,  const char*, const char*,  const char* ))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_OnUserChange ......................................
    //                   elles pouront recevoir le message de changement d'utilisateur

    connect( this,            SIGNAL(Sign_OnUserChange(const char*, const char* )),
            m_pCMDI_Ident,    SLOT( RubOnUserChange(const char*, const char*))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier un changement d'identité
    //                   au DrTux qui le repercutera aux autres filles
    connect( m_pCMDI_Ident->m_pFormRubIdent,       SIGNAL( Sign_IdentChange(const char*,  const char*, const char*,  const char* )),
             this,                                 SLOT( OnDrTuxIdentChange(const char*,  const char*, const char*,  const char* ))
           );
    connect( m_pCMDI_Ident->m_pFormRubIdent, SIGNAL( Sign_IdentModified(const char*,  const char*, const char*,  const char* )),
             this,                           SLOT( OnDrTuxIdentModified(const char*,  const char*, const char*,  const char* ))
           );
    connect( m_pCMDI_Ident->m_pFormRubIdent, SIGNAL( Sign_PatientListMustBeUpdated(const char*,  const char*, const char*,  const char* )),
             this,                           SLOT( OnDrTuxPatientListMustBeUpdated(const char*,  const char*, const char*,  const char* ))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par DrTux
    connect( this,             SIGNAL( Sign_ActiverRubrique(const char*, int)),
             m_pCMDI_Ident,    SLOT( ActiverRubrique(const char*, int))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au DrTux qui l'enl�vera de la barre d'onglets
    connect( m_pCMDI_Ident,   SIGNAL( Sign_RubIsDeleted(const char* )),
             this,            SLOT  ( OnDeleteRubrique (const char* ))
           );
    connect( m_pCMDI_Ident->m_pFormRubIdent,   SIGNAL( Sign_Ident_KillMe()),
             this,                             SLOT  ( OnSign_Ident_KillMe())
           );
    //.................. connecter signal de synchronisation des notes
    connect( m_pCMDI_Ident->m_pFormRubIdent,   SIGNAL( Sign_NoteToBeSynchronized(int, const char* )  ),
             this,                             SLOT  ( Slot_NoteToBeSynchronized(int, const char* )  )
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir d'imprimer
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Ident,         SIGNAL( Sign_Print()),
             this,                  SLOT  ( filePrint())
           );
        //.................. connecter les plugin de l'editeur au DrTux.............................................
    //                   pour qu'il puisse les executer

    //connect( m_pCMDI_Ident->m_pFormRubIdent->m_pMyEditText,         SIGNAL( Sign_Exe_Plugin(QString&, CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)),
    //         this,                                                  SLOT  ( Slot_ExePlugin(QString&,  CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap))
    //       );
    //.................. connecter les plugin de l'editeur au DrTux.............................................
    //                   pour qu'il puisse les executer

   // connect( m_pCMDI_Ident->m_pFormRubIdent->m_pMyEditText,         SIGNAL( Sign_Exe_Mixture(QString&, CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)),
   //          this,                                                  SLOT  ( Slot_ExeMixture(QString&,  CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap)));

    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur desir de sauvegarde
    //                   de leur rubrique au DrTux qui fera ce que de droit
    connect( m_pCMDI_Ident->m_pFormRubIdent      ,           SIGNAL( Sign_SaveButtonClicked()),
             this,                                           SLOT  ( OnDrTuxSaveRubList())
           );
    m_pCMDI_Ident->DoConnexionOnGossaire(m_pFormGlossaire);
    return m_pCMDI_Ident;
}

