/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *   gest_user.cpp                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//====================================== INCLUDES ========================================================================
// il est indispensable de rajouter au fichier:  src.pro  les deux lignes suivantes pour que <qwidgetfactory.h> fonctionne
//                           unix:LIBS          += -lqui -L$$QT_BUILD_TREE/lib
//                           win32:LIBS         += $$QT_BUILD_TREE/lib/qui.lib
//
// Pour disposer de CMoteur_Base avec une seule et unique définition de la classe en:
//                              ../../drtux/scr/CMoteur_Base.h       et
//                              ../../drtux/scr/CMoteur_Base.cpp
//
// Creation des liens suivants:
//          ln -s /home/ro/Developpement/drtux/src/CMoteur_Base.h CMoteur_Base.h
//          ln -s /home/ro/Developpement/drtux/src/CMoteur_Base.cpp CMoteur_Base.cpp
//
//
#include "gest_user.h"

#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/ThemePopup.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "../../MedinTuxTools/CMaskedLineEdit.h"

#include <stdlib.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtextedit.h>
#include <qfile.h>
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
#include <qdir.h>
#include <qsplitter.h>
#include <qlistbox.h>
#include <qcheckbox.h>
#include <qprocess.h>
#include <qtimer.h>
#include <qprocess.h>

#include "CApp.h"

#include "ui/DLG_InsertList.h"
#include "ui/Dlg_PermsUserChoice.h"
#include "ui/DlgPassChange.h"


#include "Rubriques.h"

#include "../../MedinTuxTools/CPreview.h"
#include "CDlg_IntervList.h"


#ifdef Q_WS_X11
    #define F_EXT  ""
    #define PROG_NAME "gest_user"
#else
    #define F_EXT  ".exe"
    #define PROG_NAME "gest_user.exe"
#endif

#define POS_NAME    0
#define POS_TYPE    1
#define POS_PRIMKEY 2

#define USER_LOGIN    0
#define USER_NOM      1
#define USER_PRENOM   2
#define USER_PRIMKEY  3
#define USER_GUID     4

void GlobalActiverWidget(QWidget *pQWidget);
extern char CHANGEMENTS[];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== Gest_User ===============================================================================

//---------------------------------------------- ~Gest_User ----------------------------------------------------------------------
Gest_User::~Gest_User()
{
}

//---------------------------------------------- Gest_User ------------------------------------------------------------
Gest_User::Gest_User()
    : QMainWindow( 0, "Gest_User", WDestructiveClose )
{   m_pCMoteurBase       = G_pCApp->m_pCMoteurBase;
    m_pActionEnregistrer = 0;
    m_PluginRun          = "";
    m_Apropos_Proc       = 0;
    m_ImagePath          = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true), "Ressources/RubriquesTextes" );
    //...........................barre et menu texte..................................................................
    setupFileActions();
    setupTextActions();
    setupEditActions();
    //.................................................................................................
    m_pCMoteurBase->GotoDebug();

    //......................... Docker dialogue des Utilisateurs ......................................
    QDockWindow *pDock;
    pDock = new QDockWindow (this, "DockFormUserList");
    pDock->setResizeEnabled(TRUE);
    pDock->setFixedExtentWidth (250);
    pDock->setCloseMode (QDockWindow::Never);
    pDock->setCaption(tr("Liste des Utilisateurs"));
    addToolBar( pDock, Qt::DockLeft );
    //................Y inserer le dialogue ........................................................
    m_pDockFormUserList = new DockFormUserList(pDock, "UserList");
    pDock->setWidget( m_pDockFormUserList );

    //............... connecter la liste des user .................................................
    connect( m_pDockFormUserList,                                SIGNAL( Sign_UserSelected(const char*)), this,   SLOT  ( OnUserChanged(const char*))  );
    connect( m_pDockFormUserList->pushButtonNewUser,             SIGNAL( clicked()),                      this,   SLOT  ( OnNewUserClicked()) );
    connect( m_pDockFormUserList->pushButtonUserDelete,          SIGNAL( clicked()),                      this,   SLOT  ( OnUserDeleteClicked()));
    connect( m_pDockFormUserList->pushButton_NewWhithExistant,   SIGNAL( clicked()),                      this,   SLOT  ( OnNewWhithExistantClicked()));
    connect( m_pDockFormUserList->pushButton_PlaceProfil,        SIGNAL( clicked()),                      this,   SLOT  ( On_pushButton_PlaceProfilClicked()) );
    connect( m_pDockFormUserList->pushButton_SaveIdentOnDisk,    SIGNAL( clicked()),                      this,   SLOT  ( On_pushButton_SaveIdentOnDiskClicked()));
    connect( m_pDockFormUserList->pushButton_NewFromFileOnDisk,  SIGNAL( clicked()),                      this,   SLOT  ( On_pushButton_NewFromFileOnDiskClicked()) );
    connect(G_pCApp,                                             SIGNAL( Sign_QuitterRequired()),         this,   SLOT  ( Slot_SauverLesMeubles()));

    //.................................................................................................
    //......................... Docker dialogue des Documents ......................................
    pDock = new QDockWindow (this, "DockFormUserListDoc");
    pDock->setResizeEnabled(TRUE);
    pDock->setFixedExtentWidth (250);
    pDock->setCloseMode (QDockWindow::Never);
    pDock->setCaption(tr("Documents Utilisateurs"));
    addToolBar( pDock, Qt::DockLeft );
    //................Y inserer le dialogue ........................................................
    m_pDockFormUserListDoc = new DockFormUserListDoc(pDock, "UserListDoc");
    m_pDockFormUserListDoc->m_pCMoteurBase = m_pCMoteurBase;
    pDock->setWidget( m_pDockFormUserListDoc );
    m_pDockFormUserListDoc->setEnabled(FALSE);
    connect( m_pDockFormUserListDoc->pushButton_ExportProfil, SIGNAL( clicked()),  this,  SLOT  ( On_pushButton_ExportProfilClicked()) );
    connect( m_pDockFormUserListDoc->pushButton_InportProfil, SIGNAL( clicked()),  this,  SLOT  ( On_pushButton_InportProfilClicked()) );

    //............... connecter le glossaire au DrTux pour placement du texte ..........................
    connect( m_pDockFormUserListDoc, SIGNAL( Sign_UserDocSelected(const char*, const char* , const char* , const char*)),
             this,                   SLOT(OnUserListDocClicked(const char*, const char* , const char* , const char* ))
           );
    //.............................................................................................
    //.......................... barre de menus Aide ........................................................
    QPopupMenu * help = new QPopupMenu( this );
    menuBar()->insertItem( tr("Ai&de"), help );
    help->insertItem( tr("&A propos "),             this, SLOT(Slot_actionApropos()), Key_F2 );
    help->insertItem( tr("A&ide"),                  this, SLOT(HelpMe()), Key_F1 );
    help->insertSeparator();
    help->insertItem( tr("&Qu'est-ce que c'est ?"), this, SLOT(whatsThis()),          SHIFT+Key_F1 );

    //................... creation du QVbox central ...............................................
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 5 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    //................... y placer une barre d'onglets .............................................
    //                    qui gerera la disposition des fenetres MDI
    FormOngletRubrique *pFormOngletRubrique = new FormOngletRubrique(pQVCentral, "TabPanelRubrique");
    m_pFormOngletRubrique = pFormOngletRubrique; // noter dans DrTux le pointeur sur ce widget d'onglets

    connect( pFormOngletRubrique->tabWidget, SIGNAL( currentChanged ( QWidget * )),   this,           SLOT(   OnOngletRubriquesChanged(QWidget * )) );
    connect( pFormOngletRubrique, SIGNAL( message(const QString&, int )),             statusBar(),    SLOT(   message(const QString&, int )) );
    //................... y placer un QWorkspace ..................................................
    //                    qui contiendra et gerera les fenetres MDI
    QWorkspace   *pQWorkspace     = new QWorkspace(pQVCentral);
    m_pQWorkSpaceRub = pQWorkspace;
    pQWorkspace->setScrollBarsEnabled( TRUE );
    setCentralWidget(  pQVCentral );
    pQWorkspace->setPaletteBackgroundPixmap (Theme::getIcon("ImageDeFond.png") );
    //.................. y connecter les onglets rubriques .........................................
    //                   pour qu'il mettent à jour celui actif
    //                   lorsqu'une fenetre est cliquée
    connect( pQWorkspace,          SIGNAL( windowActivated ( QWidget * )),
             pFormOngletRubrique,  SLOT(   OnRubriqueActived(QWidget * ))
           );
    //.............. Placer l'interface sur le bon utilisateur ........................
#ifdef SESAMVITALE_VERSION
    if (G_pCApp->IsAppStartWithUser()||G_pCApp->m_pCps != 0)
       {if (G_pCApp->m_pCps )
           {OnNewUserClicked();
           }
        else
           {
            OnUserChanged(G_pCApp->m_pCMoteurBase->GetUserPrimKey(G_pCApp->m_User));
           }
       }
#else
    if (G_pCApp->IsAppStartWithUser())
       {OnUserChanged(G_pCApp->m_pCMoteurBase->GetUserPrimKey(G_pCApp->m_User));
       }
#endif
    setIcon(Theme::getIcon("32x32/gest_user.png"));
    G_pCApp->AdjustGlobalDroits(m_pCMoteurBase->GetUserPermisions( G_pCApp->m_User ));
    statusBar()->message( tr("Un petit bonjour de la part du Gest_User"), 10000 );
    SetInterfaceOnDroits();
    resize( 950, 700 );
}

//--------------------------------- Slot_SauverLesMeubles --------------------------------------------------------------------------------
void Gest_User::Slot_SauverLesMeubles()
{tryToStopAPropos();
}

//----------------------------------- Slot_actionApropos -----------------------------------------------------------------------
void Gest_User::Slot_actionApropos()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}

//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void Gest_User::Slot_actionAproposDisplay()
{        CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"Images/Changements.html", QString::fromUtf8 ( CHANGEMENTS) );
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).dirPath (true));
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).dirPath (true))+"Ressources/";
         QStringList argList;

         //......................... completer les autres arguments .........................................

         if (m_Apropos_Proc==0)
            {//m_action_A_Propos->setDisabled(TRUE);
             m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->addArgument( pathExeAPropos);                                                   // 0  chemin de l'executable APropos
             m_Apropos_Proc->addArgument("gest_user");                                                       // 1  nom du module dont il faut afficher infos
             m_Apropos_Proc->addArgument("Module de gestion des profils utilisateurs");                      // 2  description courte
             m_Apropos_Proc->addArgument(G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("="));        // 3  numero de version
             m_Apropos_Proc->addArgument(G_pCApp->m_PathAppli+"Images/Changements.html");                    // 4  fichiers decrivant les changements
             m_Apropos_Proc->addArgument(Theme::getPath(Theme::WithSeparator)+"32x32/gest_user.png");        // 5  Icone du programme
             m_Apropos_Proc->addArgument(QDir::cleanDirPath (G_pCApp->m_PathAppli+"../../Doc/GestUser.html"));     // 6  documentation
             m_Apropos_Proc->start();
             SLEEP(1);
             G_pCApp->processEvents ();
             while ( m_Apropos_Proc->isRunning () /* && QFile::exists(pathBinRessources+"~A_propos.html")*/)
                   { G_pCApp->processEvents ( QEventLoop::WaitForMore );
                   }
             if (m_Apropos_Proc) delete m_Apropos_Proc;
             m_Apropos_Proc = 0;
             QFile::remove(pathBinRessources+"~A_propos.html");
             //m_action_A_Propos->setDisabled(FALSE);
            }
}

//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void Gest_User::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}


//---------------------------------------- On_pushButton_SaveIdentOnDiskClicked -------------------------------------------------------------------------------
void Gest_User::On_pushButton_SaveIdentOnDiskClicked()
{doYouVantSaveAllBeforeAction();
 QString       data = SerializeAll();           // recuperer les données du profil en cours
 if (data.length() == 0) return;

    QFileDialog *fd = new QFileDialog (G_pCApp->m_PathAppli+"Pieds-Entetes",
                                       tr( "Fichiers de texte (*.asc *.txt);;"
                                           "Fichiers HTML (*.htm *.html);;"
                                           "Fichiers ASCII (*.cpp *.h *.ui.h *.ui *.c);;"
                                           "Tous les fichiers (*)"),
                                       this,
                                       "SaveDlg"
                                      ) ;
    if (fd==0)                                             return;
    fd->setMode(QFileDialog::AnyFile );    // selection multiple
    if ( fd->exec() == QDialog::Accepted )
       {QString fn = fd->selectedFile();
        if ( !fn.isEmpty() )
           {CGestIni::Param_UpdateToDisk(fn, data);
           }
       }
    delete fd;
}


//---------------------------------------- On_pushButton_NewFromFileOnDiskClicked -------------------------------------------------------------------------------
void Gest_User::On_pushButton_NewFromFileOnDiskClicked()
{doYouVantSaveAllBeforeAction();
 QFileDialog *fd = new QFileDialog (G_pCApp->m_PathAppli+"Pieds-Entetes",
                                       tr( "Fichiers de texte (*.asc *.txt);;"
                                           "Fichiers de source (*.c *.cpp *.h *.ui.h *.ui);;"
                                           "Fichiers texte (*.htm *.html *.HTM *.HTML *.TXT *.txt *.rtf *.RTF);;"
                                           "Tous les fichiers (*)" ),
                                       this,
                                       "OuvrirDlg"
                                      ) ;
    if (fd==0)                                             return;
    if ( fd->exec() == QDialog::Accepted )
       { //OnNewUserClicked();
        if (UnSerializeIdentite(fd->selectedFile())==0)
           {//OnNewUserAbort();
            delete fd;                                     return;
           }
        UnSerializeDocuments(fd->selectedFile());
        //................. tout mettre en mode modification ..............................;
        CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
        if (pCMDI_Ident) pCMDI_Ident->m_pFormRubIdent->NotifyIdentChangedToGestUser("");
       }
    delete fd;
}

//---------------------------------------- SerializeAll -------------------------------------------------------------------------------
QString  Gest_User::SerializeIdentite()
{QString serializeData = tr(";------------- profil complet avec identite-------------------\r\n");
 CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident == 0) return serializeData;
 CGestIni::Param_WriteParam(&serializeData, "identite", "nom" ,           pCMDI_Ident->m_pFormRubIdent->lineEdit_Nom->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "prenom" ,        pCMDI_Ident->m_pFormRubIdent->lineEdit_Prenom->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "titre" ,         pCMDI_Ident->m_pFormRubIdent->lineEdit_UserTitre->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "login" ,         pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "qualification" , pCMDI_Ident->m_pFormRubIdent->lineEdit_Qualif->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "sexe" ,          pCMDI_Ident->m_pFormRubIdent->comboBoxSexe->currentText());
 CGestIni::Param_WriteParam(&serializeData, "identite", "numero ordinal" ,pCMDI_Ident->m_pFormRubIdent->lineEdit_NumOrdinal->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "numero rpps" ,   pCMDI_Ident->m_pFormRubIdent->lineEdit_NumRpps->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "convention" ,    pCMDI_Ident->m_pFormRubIdent->lineEdit_Convention->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "droits" ,        pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->text());
 QString str = pCMDI_Ident->m_pFormRubIdent->textEdit_Adresse->text().remove("\r");
 str = str.replace("\n", "%@%");
 CGestIni::Param_WriteParam(&serializeData, "identite", "adresse" ,str);
 CGestIni::Param_WriteParam(&serializeData, "identite", "code postal" ,pCMDI_Ident->m_pFormRubIdent->lineEdit_CdPostal->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "ville" ,      pCMDI_Ident->m_pFormRubIdent->lineEdit_Ville->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "email" ,      pCMDI_Ident->m_pFormRubIdent->lineEdit_Email->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "tel_1" ,      pCMDI_Ident->m_pFormRubIdent->lineEdit_Tel_1->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "tel_2" ,      pCMDI_Ident->m_pFormRubIdent->lineEdit_Tel_2->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "tel_3" ,      pCMDI_Ident->m_pFormRubIdent->lineEdit_Tel_3->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "tel_Type_1" , pCMDI_Ident->m_pFormRubIdent->comboBoxTelTyp_1->currentText());
 CGestIni::Param_WriteParam(&serializeData, "identite", "tel_Type_2" , pCMDI_Ident->m_pFormRubIdent->comboBoxTelTyp_2->currentText());
 CGestIni::Param_WriteParam(&serializeData, "identite", "tel_Type_3" , pCMDI_Ident->m_pFormRubIdent->comboBoxTelTyp_3->currentText());
 CGestIni::Param_WriteParam(&serializeData, "identite", "TelAbr_1"   , pCMDI_Ident->m_pFormRubIdent->lineEdit_Code_1->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "TelAbr_2"   , pCMDI_Ident->m_pFormRubIdent->lineEdit_Code_2->text());
 CGestIni::Param_WriteParam(&serializeData, "identite", "TelAbr_3"   , pCMDI_Ident->m_pFormRubIdent->lineEdit_Code_3->text());
 return serializeData.prepend("<DocIdentite_GestUser>\r\n")+("</DocIdentite_GestUser>\r\n");
}

//---------------------------------------- UnSerializeIdentite -------------------------------------------------------------------------------
int  Gest_User::UnSerializeIdentite(const QString& file)
{QString serializeData;
 if ( file.length())
    {CGestIni::Param_UpdateFromDisk(file , serializeData);
     return UnSerializeIdentite(serializeData, " ");
    }
 return 0;
}

//---------------------------------------- UnSerializeIdentite -------------------------------------------------------------------------------
int  Gest_User::UnSerializeIdentite(const QString &serializeData_in, const QString & )
{
 CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident == 0) return 0;
//............... decouper aux données utiles .............................
 QString serializeData = serializeData_in;
 int deb = 0;
 int end = 0;
 if ( (deb=serializeData.find("<DocIdentite_GestUser>", end)) != -1)
    { deb += 17;
      if ( (end=serializeData.find("</DocIdentite_GestUser>", deb)) != -1 )
         { serializeData = serializeData.mid(deb, end-deb);
         }
      else return 0;
    }

 pCMDI_Ident->m_pFormRubIdent->lineEdit_Nom->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "nom"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Prenom->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "prenom"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_UserTitre->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "titre"));
 //pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "login"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Qualif->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "qualification"));

 QString str = CGestIni::Param_ReadUniqueParam(serializeData, "identite", "sexe");
 if (str=="M") pCMDI_Ident->m_pFormRubIdent->comboBoxSexe->setCurrentItem (0);
 else          pCMDI_Ident->m_pFormRubIdent->comboBoxSexe->setCurrentItem (1);

 pCMDI_Ident->m_pFormRubIdent->lineEdit_NumOrdinal->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "numero ordinal"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_NumRpps->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "numero rpps"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Convention->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "convention"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "droits"));

 str = CGestIni::Param_ReadUniqueParam(serializeData, "identite", "adresse");
 str =  str.replace("%@%","\n"); pCMDI_Ident->m_pFormRubIdent->textEdit_Adresse->setText(str);

 pCMDI_Ident->m_pFormRubIdent->lineEdit_CdPostal->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "code postal"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Ville->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "ville"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Email->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "email"));

 pCMDI_Ident->m_pFormRubIdent->lineEdit_Tel_1->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "tel_1"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Tel_2->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "tel_2"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Tel_2->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "tel_3"));

 pCMDI_Ident->m_pFormRubIdent->comboBoxTelTyp_1->lineEdit()->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "tel_Type_1"));
 pCMDI_Ident->m_pFormRubIdent->comboBoxTelTyp_2->lineEdit()->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "tel_Type_1"));
 pCMDI_Ident->m_pFormRubIdent->comboBoxTelTyp_3->lineEdit()->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "tel_Type_1"));

 pCMDI_Ident->m_pFormRubIdent->lineEdit_Code_1->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "TelAbr_1"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Code_2->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "TelAbr_2"));
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Code_3->setText(CGestIni::Param_ReadUniqueParam(serializeData, "identite", "TelAbr_3"));
 pCMDI_Ident->OnSign_IdentiteChanged();

 return 1;
}
//---------------------------------------- SerializeAll -------------------------------------------------------------------------------
//
QString  Gest_User::SerializeAll()
{QString serializeData = QString("<!DOCTYPE PROFILE_GestUser>\r\n") + SerializeIdentite()+SerializeDataDocument();
  return serializeData;
}

//---------------------------------------- On_pushButton_ExportProfilClicked -------------------------------------------------------------------------------
void Gest_User::On_pushButton_ExportProfilClicked()
{doYouVantSaveAllBeforeAction();
 QString       data = SerializeDocOnly();           // recuperer les données du profil en cours
 if (data.length() == 0) return;

    QFileDialog *fd = new QFileDialog (G_pCApp->m_PathAppli+"Pieds-Entetes",
                                       tr( "Fichiers de texte (*.asc *.txt);;"
                                           "Fichiers HTML (*.htm *.html);;"
                                           "Fichiers ASCII (*.cpp *.h *.ui.h *.ui *.c);;"
                                           "Tous les fichiers (*)"),
                                       this,
                                       "SaveDlg"
                                      ) ;
    if (fd==0)                                             return;
    fd->setMode(QFileDialog::AnyFile );    // selection multiple
    if ( fd->exec() == QDialog::Accepted )
       {QString fn = fd->selectedFile();
        if ( !fn.isEmpty() )
           {CGestIni::Param_UpdateToDisk(fn, data);
           }
       }
    delete fd;
}

//---------------------------------------- On_pushButton_InportProfilClicked -------------------------------------------------------------------------------
void Gest_User::On_pushButton_InportProfilClicked()
{   QFileDialog *fd = new QFileDialog (G_pCApp->m_PathAppli+"Pieds-Entetes",
                                       tr( "Fichiers de texte (*.asc *.txt);;"
                                           "Fichiers de source (*.c *.cpp *.h *.ui.h *.ui);;"
                                           "Fichiers texte (*.htm *.html *.HTM *.HTML *.TXT *.txt *.rtf *.RTF);;"
                                           "Tous les fichiers (*)" ),
                                       this,
                                       "OuvrirDlg"
                                      ) ;
    if (fd==0)                                             return;
    if ( fd->exec() == QDialog::Accepted )
       {UnSerializeDocuments(fd->selectedFile());
       }
    delete fd;
}

//---------------------------------------- SerializeDocOnly -------------------------------------------------------------------------------
//
QString  Gest_User::SerializeDocOnly()
 {return QString("<!DOCTYPE PROFILE_GestUser>\r\n") + SerializeDataDocument();
 }

//---------------------------------------- SerializeDataDocument -------------------------------------------------------------------------------
QString  Gest_User::SerializeDataDocument()
{ QString serializeData   = "";
  QString droits          = G_pCApp->m_Droits;
  CMDI_Ident* pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
  if (pCMDI_Ident!=0)       {droits = pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->text();
                            }
  QListViewItemIterator it( m_pDockFormUserListDoc->ListView_Doc );
  while ( it.current() )
        { QListViewItem *pQListViewItem = it.current();
          QString name            = pQListViewItem->text(POS_NAME);
          QString type            = pQListViewItem->text(POS_TYPE);
          QString prim_key        = pQListViewItem->text(POS_PRIMKEY);
          QString data       = G_pCApp->m_pCMoteurBase->GetUserDataFromPrimKey(prim_key);
          serializeData     += QString("<DocDefinition_GestUser>\r\n")+
                                       "      <DocType_GestUser>"  +type+   "</DocType_GestUser>\r\n"
                                       "      <DocName_GestUser>"  +name+   "</DocName_GestUser>\r\n"
                                       "      <Data_GestUser>"     +data+   "</Data_GestUser>\r\n"
                                       "</DocDefinition_GestUser>\r\n\r\n";
          ++it;
        }
  serializeData     +=  "      <Droits_GestUser>"   +droits+ "</Droits_GestUser>\r\n\r\n";
 return serializeData;
}

//---------------------------------------- UnSerializeDocuments -------------------------------------------------------------------------------
void  Gest_User::UnSerializeDocuments(const QString& file)
{   QString droits   = "";
    QString data     = "";
    QString prim_key = "";
    if ( file.length())
       {CGestIni::Param_UpdateFromDisk(file , data);
        QListViewItem *pQListViewItem =  m_pDockFormUserList->ListView_UserList->currentItem();
        if (pQListViewItem)
           { prim_key = pQListViewItem->text(USER_PRIMKEY);
             G_pCApp->m_pCMoteurBase->DeleteUserDataFromRefUserPrimKey( prim_key );  // effacer les anciennes données
             droits = UnSerializeDocuments( data, prim_key) ;                                 // placer  les données nouvelles
             m_pDockFormUserListDoc->SetData(prim_key);
             pQListViewItem = m_pDockFormUserListDoc->ListView_Doc->firstChild();
             if (pQListViewItem)
                {m_pDockFormUserListDoc->ListView_Doc->setCurrentItem ( pQListViewItem );
                 m_pDockFormUserListDoc->ListView_Doc->setSelected ( pQListViewItem, TRUE );
                 m_pDockFormUserListDoc->ListView_Doc_clicked( pQListViewItem );
                }
             //........... mettre à jour l'identité avec les nouveaux droits .......................
             //            et la mettre en etat non modifiee car tout est à jour
             CMDI_Ident* pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
             if (pCMDI_Ident!=0)       {pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->setText(droits);
                                        pCMDI_Ident->m_pFormRubIdent->pushButtonSaveNew->setEnabled(FALSE);
                                        pCMDI_Ident->m_IsModified = FALSE;
                                        if (m_pActionEnregistrer) m_pActionEnregistrer->setEnabled(FALSE);
                                       }
           }
       }
}

//---------------------------------------- UnSerializeDocuments -------------------------------------------------------------------------------
QString  Gest_User::UnSerializeDocuments(const QString& data, const QString & primKey)
{int deb = 0;
 int end = -1;
 bool ok = TRUE;
 QString type, name, dataDoc;
 QString droits = "";
 while ( ok &&  (deb=data.find("<DocType_GestUser>", deb)) != -1)
      { ok   = FALSE;
        deb += 18;
        if (  (end=data.find("</DocType_GestUser>", deb)) != -1 )
           { type = data.mid(deb, end-deb);
             if ( (deb=data.find("<DocName_GestUser>", end)) != -1)
                { deb += 18;
                  if ( (end=data.find("</DocName_GestUser>", deb)) != -1 )
                     { name = data.mid(deb, end-deb);
                       if ( (deb=data.find("<Data_GestUser>", end)) != -1)
                          { deb += 15;
                            if ( (end=data.find("</Data_GestUser>", deb)) != -1 )
                               { dataDoc = data.mid(deb, end-deb);
                                 deb     = end + 16;
                                 G_pCApp->m_pCMoteurBase->CreateUserDataFromPrimKey(type.toInt(), primKey ,name, dataDoc);
                                 ok      = TRUE;
                               }
                          }
                     }
                }
           }
      } // end while ( ok && (deb=data.find("<DocType_GestUser>", deb) != -1))
 //............... les droits ...................
 if ( (deb=data.find("<Droits_GestUser>", end)) != -1)
    { deb += 17;
      if ( (end=data.find("</Droits_GestUser>", deb)) != -1 )
         { droits = data.mid(deb, end-deb);
           G_pCApp->m_pCMoteurBase->SetUserPermisions( primKey, droits );
         }
    }
 return droits;
}

//---------------------------------------- On_pushButton_PlaceProfilClicked -------------------------------------------------------------------------------
void Gest_User::On_pushButton_PlaceProfilClicked()
{    //.................. creer la liste des utilisateurs deja presents .......................
     doYouVantSaveAllBeforeAction();
     QString data = SerializeDocOnly();        // recuperer les données du profil en cours
     QStringList userList;
     userList.clear();
     //.................. lancer le dialogue sans les utilisateurs déjà présents .......................
     Dlg_PermsUserChoice *pDlg_PermsUserChoice = new Dlg_PermsUserChoice(0, "Dlg_PermsUserChoice", TRUE);
     if ( pDlg_PermsUserChoice )
        {
         pDlg_PermsUserChoice->initDialog(G_pCApp->m_pCMoteurBase, &userList,"");
         pDlg_PermsUserChoice->textLabel_Entete->setText(tr("Veuillez sélectionner les utilisateurs pour lesquels vous désirez enregistrer le profil actuel"));
         pDlg_PermsUserChoice->setListViewOnUser(G_pCApp->m_User);
         int ret = pDlg_PermsUserChoice->exec();   // en retour la liste : userList est remplie avec les PK des user selectionnes
         delete pDlg_PermsUserChoice;
         if (ret==QDialog::Rejected)  return;
         for ( QStringList::Iterator list_it = userList.begin(); list_it != userList.end(); ++list_it )
             { QString   prim_key  =  *list_it;
               G_pCApp->m_pCMoteurBase->DeleteUserDataFromRefUserPrimKey( prim_key );  // effacer les anciennes données
               UnSerializeDocuments( data, prim_key) ;                                          // placer  les données du profil en cours
             } //end for ( QStringList::Iterator list_it = userList.begin(); it != userList.end(); ++it )
        } //endif ( pDlg_PermsUserChoice )
}

//---------------------------------------- HelpMe -------------------------------------------------------------------------------
void Gest_User::HelpMe()
{QString pathPlugin   = CGestIni::Construct_Name_Exe("manu",QFileInfo (qApp->argv()[0]).dirPath (true));
 QString  rubName  = "";
 QString  maskExch = "";
 QString startDoc  = "index.html";

 QStringList paramList;
 startDoc = tr("GestUser.html");
 paramList.append(startDoc);                         // 13
 paramList.append( "../../Doc");                     // 14
 paramList.append( "../../Doc");                     // 15
 paramList.append( "index.lst");                     // 16
 paramList.append( tr("GestUser.html"));  // 17
 paramList.append("local");                          // 18
 int pos = pathPlugin.find(".exe");
 if (pos != -1) pathPlugin = pathPlugin.left(pos);
 /*plugin = */                       PluginExe(this,
                                              pathPlugin,                       // Chemin de l'executable plugin à actionner sans l'extension .exe
                                              G_pCApp->m_PathAppli,             // Chemin de l'executable appelant (DrTux)
                                              G_pCApp->m_PathIni,               // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                              maskExch,                         // texte du fichier d'exchange
                                              "numGUID",
                                              "obsPk",
                                              "ordPk",
                                              "terPk" ,
                                              paramList,
                                              CMoteurBase::noWait );
}
//------------------------------------------------------- PluginExe --------------------------------------------------
// un plungin est un executable communicant avec DrTux par l'intermediare du protocole suivant:
// DrTux appelle le plugin avec dans la ligne de commande:
// en :
//   0  Chemin de l'executable plugin à actionner
//   1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
//   2  Chemin de l'executable appelant
//   3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour
//                        ce fichier est retourné modifié par le plugin, ce fichier peut etre un masque html ou
//                        tout autre délire en accord avec la syntaxe du plugin d�s fois que ...
//                        si ce fichier est vide, où n'existe pas le plugin retournera son résultat dans ce fichier
//                        d'échange que l'appelant récupera
//                        Le nom de ce fichier d'echange comporte : NomDuPlugin-GUID_Patient-
//   4  GUID du patient
//   5  PrimKey de l'observation en cours
//   6  PrimKey du terrain en cours
//   7  PrimKey de l'ordonnance en cours
//   8  Reservé
//   9  Reservé
//   10 Reservé
//   11 Reservé
//   12 Nb autres parametres d'appels specifiques au plugin
//   13 --> 13+Nb parametres

QString Gest_User::PluginExe(       QObject     *pQObject,
                                const QString    &pathPlugin,  // Chemin de l'executable plugin à actionner sans l'extension .exe
                                const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                const QString    &maskExch,    // texte du fichier d'exchange
                                const QString    &guid,
                                const QString    &dosPk,
                                const QString    &terPk,
                                const QString    &ordPk ,
                                QStringList      &param,
                                int              waitFlag /* = CMoteurBase::endWait */)
{
 QString      ret = "";
 //..................... creer le process ...........................................................
 QProcess*  proc =  new QProcess( pQObject );
 if (proc==0)       return QString(QObject::tr("Erreur:  PluginExe() the process can't be created"));

 //..................... copier le masque de retour dans le dossier temporaire ......................

 QString pathExe  = pathPlugin;
 if ( QDir::isRelativePath ( pathExe ) ) pathExe = QDir::cleanDirPath (pathExe.prepend(pathAppli) );
 QString nameExch = QFileInfo(pathExe).fileName()+"-"+ guid +".exc";

 QString      dst = "-";
 pathExe         = pathExe + F_EXT;
 //......................... completer les autres arguments .........................................
 proc->addArgument( pathExe );                   // 0  Chemin de l'executable plugin à actionner
 proc->addArgument( pathIni );                         // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
 proc->addArgument( pathAppli + PROG_NAME );           // 2  Chemin de l'executable appelant
 proc->addArgument( dst );                             // 3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour
 proc->addArgument(guid );                             // 4  GUID du patient
 proc->addArgument(dosPk);                             // 5  PrimKey de l'observation en cours
 proc->addArgument(terPk);                             // 6  PrimKey du terrain en cours
 proc->addArgument(ordPk);                             // 7  PrimKey de l'ordonnance en cours
 proc->addArgument("-");                               // 8  reservé
 proc->addArgument("-");                               // 9  reservé
 proc->addArgument("-");                               // 10 reservé
 proc->addArgument("-");                               // 11 reservé
 proc->addArgument(QString::number(param.count()));    // 12 Nb autres parametres d'appels specifiques au plugin
 for ( QStringList::Iterator it = param.begin(); it != param.end(); ++it )  // 13 --> 13+Nb parametres
     {proc->addArgument(*it);
     }
 //................................. executer le process .............................................
 if ( m_PluginRun == "" && proc->start() )
    { m_PluginRun = pathExe;
     qApp->processEvents ();
     while (waitFlag==CMoteurBase::endWait && proc->isRunning () )
           {QTimer timer;               // pour etre sur qu'un evenement se produise
            timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
            QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     m_PluginRun = "";
    }
  delete proc;
  return ret;
}

//------------------------------------------------ OnNewWhithExistantClicked -------------------------------------------------------
void Gest_User::OnNewWhithExistantClicked()
{QStringList listGUIDAlreadyPresent;

 QListViewItemIterator it( m_pDockFormUserList->ListView_UserList );
 while ( it.current() )
       {    QListViewItem *item = it.current();
            listGUIDAlreadyPresent.append(item->text(3));
            ++it;
        }
 QString userPk       = "";
 CDlg_IntervList *dlg = new CDlg_IntervList(listGUIDAlreadyPresent, this);
 if (dlg == 0)             return;
 if (dlg->exec()==QDialog::Accepted  ) userPk = dlg->m_Pk_Returned;
 delete dlg;

 if (userPk.length()==0) return;
 OnNewUserClicked();
 CMDI_Ident* pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident!=0)
    {pCMDI_Ident->m_pFormRubIdent->GetUserIdentiteFromPrimKey(userPk);
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setFocus();
    }
}

//------------------------------------------------ SetInterfaceOnDroits -------------------------------------------------------
void Gest_User::SetInterfaceOnDroits()
{CMDI_Ident*                pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (G_pCApp->m_Droits.find("adm")==-1)
    {m_pDockFormUserList->pushButtonUserDelete->setEnabled(FALSE);
     m_pDockFormUserList->pushButtonNewUser->setEnabled(FALSE);
     m_pDockFormUserList->pushButton_NewWhithExistant->setEnabled(FALSE);
     m_pDockFormUserList->pushButton_Appartenance->setEnabled(FALSE);
     m_pDockFormUserList->pushButton_PlaceProfil->setEnabled(FALSE);
     m_pDockFormUserListDoc->pushButton_ExportProfil->setEnabled(FALSE);
     m_pDockFormUserListDoc->pushButton_InportProfil->setEnabled(FALSE);
     m_pDockFormUserList->pushButton_SaveIdentOnDisk->setEnabled(FALSE);
     m_pDockFormUserList->pushButton_NewFromFileOnDisk->setEnabled(FALSE);

     if (pCMDI_Ident!=0)
        {  pCMDI_Ident->m_pFormRubIdent->pushButtonAddDroit->setEnabled(FALSE);
           pCMDI_Ident->m_pFormRubIdent->pushButtonRemoveDroit->setEnabled(FALSE);
           pCMDI_Ident->m_pFormRubIdent->pushButtonAppartenance->setEnabled(FALSE);
        }
    }
 else
   {m_pDockFormUserList->pushButtonUserDelete->setEnabled(TRUE);
    m_pDockFormUserList->pushButtonNewUser->setEnabled(TRUE);
    m_pDockFormUserList->pushButton_NewWhithExistant->setEnabled(TRUE);
    m_pDockFormUserList->pushButton_Appartenance->setEnabled(TRUE);
    m_pDockFormUserList->pushButton_PlaceProfil->setEnabled(TRUE);
    m_pDockFormUserListDoc->pushButton_ExportProfil->setEnabled(TRUE);
    m_pDockFormUserListDoc->pushButton_InportProfil->setEnabled(TRUE);
    m_pDockFormUserList->pushButton_SaveIdentOnDisk->setEnabled(TRUE);
    m_pDockFormUserList->pushButton_NewFromFileOnDisk->setEnabled(TRUE);

    if (pCMDI_Ident!=0)
        { pCMDI_Ident->m_pFormRubIdent->pushButtonAddDroit->setEnabled(TRUE);
          pCMDI_Ident->m_pFormRubIdent->pushButtonRemoveDroit->setEnabled(TRUE);
          pCMDI_Ident->m_pFormRubIdent->pushButtonAppartenance->setEnabled(TRUE);
        }
   }
}

//---------------------------------------------- fileSave -----------------------------------------------------------------------
void Gest_User::fileSave()
{CMDI_Ident*                pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident!=0)       {pCMDI_Ident->SaveDocument();
                            pCMDI_Ident->m_pFormRubIdent->pushButtonSaveNew->setEnabled(FALSE);
                           }
 CMDI_Ordo*                 pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique("", TYP_ORDONNANCE);
 if (pCMDI_Ordo!=0)         {pCMDI_Ordo->SaveDocument();
                            }

 CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique("", TYP_CERTIFICAT);
 if (pCMDI_Certificat!=0)   {pCMDI_Certificat->SaveDocument();
                            }
 CMDI_Observation*          pCMDI_Observation = (CMDI_Observation*) IsExistRubrique("", TYP_OBSERVATION);
 if (pCMDI_Observation!=0)  {pCMDI_Observation->SaveDocument();
                            }

 CMDI_Param*               pCMDI_Param   = (CMDI_Param*) IsExistRubrique("", TYP_PARAM);
 if (pCMDI_Param!=0)       {pCMDI_Param->SaveDocument();
                           }
 if (m_pActionEnregistrer) m_pActionEnregistrer->setEnabled(FALSE);

}

//------------------------------------------------ OnPassWordChanged -------------------------------------------------------
void Gest_User::OnPassWordChanged(const char* password)
{G_pCApp->m_CriptedPassWord   = password;
 emit Sign_PassWordChanged(G_pCApp->m_CriptedPassWord);   // envoyer changement de password pour les fenetres filles
}

//------------------------------------------------ OnUserDeleteClicked -------------------------------------------------------
void Gest_User::OnUserDeleteClicked()
{QListViewItem *pcurrentItem =  m_pDockFormUserList->ListView_UserList->currentItem();
 if (pcurrentItem==0)           return;
 QString droits = m_pCMoteurBase->GetUserPermisions( G_pCApp->m_User );
 if (droits.find("adm")!=-1 && m_pCMoteurBase->UserDroitsCount("adm")<=1)
    {QMessageBox::warning( this, tr("Retirer un utilisateur "),
                                 tr("<b><u>ATTENTION</u></b> : Il ne reste plus qu'un administrateur\n"
                                    "           autorisé à utiliser MedinTux,le retirer\n"
                                    "           est impossible car il faut au moins un\n"
                                    "           utilisateur administrateur pour gérer MedinTux."),
                                 tr("Annuler"));

     return;
    }
 if (QMessageBox::warning( this, tr("Retirer un Utilisateur "),
                                 tr("ATTENTION : vous êtes sur le point\n"
                                    "            de retirer cet utilisateur\n"
                                    "            de ceux pouvant utiliser MedinTux"),
                                 tr("Annuler"),tr("Retirer")
                         ) ==1
    )
    {CMDI_Ident*    pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
     if (pCMDI_Ident!=0)
        {pCMDI_Ident->SaveDocument();
         pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setText("");
         pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->setText("");
         pCMDI_Ident->m_pFormRubIdent->UpdateUserDataRecord(0);
         ClearInterface();
         m_pDockFormUserList->InitUserList();
         m_pDockFormUserList->SetUnLogged();
         m_pDockFormUserListDoc->setEnabled(FALSE);
         OnRubIsUpdated();
        }
     /*
     m_pCMoteurBase->DeleteUserFromPrimKey(pcurrentItem->text(3));
     m_pCMoteurBase->PermsUserRefToUserDelete( pcurrentItem->text(4));          // si cet utilisateur est réferencé comme ami dans des listes tuer les références
     m_pCMoteurBase->PermsUserListDelete(pcurrentItem->text(4));                // si cet utilisateur a une liste d'amis la tuer
     ClearInterface();
     m_pDockFormUserList->InitUserList();
     m_pDockFormUserList->SetUnLogged();
     m_pDockFormUserListDoc->setEnabled(FALSE);
     OnRubIsUpdated()
     */;
    }
}

//---------------------------------------------- ClearInterface ------------------------------------------------------------
void Gest_User::ClearInterface()
{CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident)          { delete pCMDI_Ident; }

//................. effacer les ordos si y en a .....................................
 CMDI_Ordo*                 pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique("", TYP_ORDONNANCE);
 if (pCMDI_Ordo)            {delete pCMDI_Ordo; }

 //................. effacer les Certif si y en a .....................................
 CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique("", TYP_CERTIFICAT);
 if (pCMDI_Certificat)      {delete pCMDI_Certificat;  }

 //................. effacer les Observ si y en a .....................................
 CMDI_Observation*          pCMDI_Observation = (CMDI_Observation*) IsExistRubrique("", TYP_OBSERVATION);
 if (pCMDI_Observation)     {delete pCMDI_Observation;  }

//................. effacer les param si y en a .....................................
 CMDI_Param*                pCMDI_Param = (CMDI_Param*) IsExistRubrique("", TYP_PARAM);
 if (pCMDI_Param)           {delete pCMDI_Param;  }
}

//------------------------------------------- GetUserPrimKey -----------------------------------------------------------------
QString Gest_User::GetUserPrimKey()
{QListViewItem *pcurrentItem =  m_pDockFormUserList->ListView_UserList->currentItem();
 if (pcurrentItem==0)           return "";
 return pcurrentItem->text(3);
}

//---------------------------------------------- OnNewUserAbort ------------------------------------------------------------
void Gest_User::OnNewUserAbort()
{QString oldUserPk = m_pCMoteurBase->GetUserPrimKey(m_OldUser);
 G_pCApp->m_User   = m_OldUser;
 CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident)
    {pCMDI_Ident->m_pFormRubIdent->pushButtonSaveNew->setText(tr("En&register les modifications"));
     pCMDI_Ident->m_pFormRubIdent->pushButtonAnnulerCreation->hide();
     pCMDI_Ident->m_pFormRubIdent->lineCreation->hide();
     pCMDI_Ident->m_pFormRubIdent->textLabelCreation->hide();
     pCMDI_Ident->m_pFormRubIdent->pushButtonAppartenance->show();
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->setText("");
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Qualif->setText("");
     pCMDI_Ident->OnIdentChange(oldUserPk);
     pCMDI_Ident->m_IsModified = FALSE;
    }
 if (m_pDockFormUserList)      m_pDockFormUserList->setEnabled(TRUE);
 if (m_pDockFormUserListDoc)   {m_pDockFormUserListDoc->setEnabled(TRUE);
                                m_pDockFormUserList->SetUser(m_OldUser);
                                m_pDockFormUserListDoc->SetData(oldUserPk);
                               }
 if (m_pActionEnregistrer) m_pActionEnregistrer->setEnabled(FALSE);
 setCaption(tr("Gest_User pour MédinTux utilisé par : ") + m_OldUser);

 #ifdef SESAMVITALE_VERSION
 if (G_pCApp->m_pCps)
    {m_pDockFormUserList->SetUnLogged();

     delete G_pCApp->m_pCps; G_pCApp->m_pCps = 0;
    }
 #endif

}

//---------------------------------------------- OnNewUserClicked ------------------------------------------------------------
// ACTION: Slot activé en réponse au signal: pDockFormUserList,
//         SIGNAL( Sign_UserDocSelected(const char* ref_UserPrimKey, const char* name, const char*  type, const char* prim_key)

void Gest_User::OnNewUserClicked()
{doYouVantSaveAllBeforeAction();
 QString prim_key("");
 //.................. ouvrir la rubrique identite ...................................
 CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident==0)   pCMDI_Ident = CMDI_IdentCreate("");
 else                  GlobalActiverWidget(pCMDI_Ident);
 if (pCMDI_Ident==0)   return;
 //.................................. noter l'&ncien utilisateur pour ...........................
 //  pouvoir le restaurer si annulation création
 // si confirmation c'est le nouveau qui sera actif
 m_OldUser = G_pCApp->m_User;

 //.................. effacer les données si deja y en a .............................
 pCMDI_Ident->ClearData();                                 // effacer les données de la boite de saisie identitaire
 pCMDI_Ident->m_pFormRubIdent->pushButtonPassWord->setEnabled(TRUE);
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Droits->setText( G_pCApp->GetListDroitsPossibles().remove("-adm") );
 pCMDI_Ident->m_pFormRubIdent->pushButtonSaveNew->setEnabled(FALSE);
 pCMDI_Ident->m_IsModified = FALSE;

 //................. effacer les ordos si y en a .....................................
 CMDI_Ordo*           pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique("", TYP_ORDONNANCE);
 if (pCMDI_Ordo)      {delete pCMDI_Ordo; } //ORDO_NAME CERTIF_NAME PARAM_NAME OBSERV_NAME
 //................. effacer les Certif si y en a .....................................
 CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique("", TYP_CERTIFICAT);
 if (pCMDI_Certificat)      {delete pCMDI_Certificat; }
 //................. effacer les Observ si y en a .....................................
 CMDI_Observation*           pCMDI_Observation = (CMDI_Observation*) IsExistRubrique("", TYP_OBSERVATION);
 if (pCMDI_Observation)      {delete pCMDI_Observation; }
//................. effacer les param si y en a .....................................
 CMDI_Param*           pCMDI_Param = (CMDI_Param*) IsExistRubrique("", TYP_PARAM);
 if (pCMDI_Param)      {delete pCMDI_Param; }

 m_pDockFormUserListDoc->ListView_Doc->clear();

 pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setEnabled(TRUE);   // seul cas ou il est possible de modifier le login est lors création
 pCMDI_Ident->m_pFormRubIdent->lineEdit_Nom->setFocus();

 pCMDI_Ident->m_pFormRubIdent->pushButtonSaveNew->setText(tr("Con&firmer la création d'un nouvel utilisateur"));
 pCMDI_Ident->m_pFormRubIdent->pushButtonAnnulerCreation->show();
 pCMDI_Ident->m_pFormRubIdent->lineCreation->show();
 pCMDI_Ident->m_pFormRubIdent->textLabelCreation->show();
 pCMDI_Ident->m_pFormRubIdent->pushButtonAppartenance->hide();

 if (m_pDockFormUserList)      m_pDockFormUserList->setEnabled(FALSE);
 if (m_pDockFormUserListDoc)   m_pDockFormUserListDoc->setEnabled(FALSE);
 OnRubIsUpdated();
 setCaption(tr("Gest_User pour MédinTux (Utilisateur en cours de création) ") );
 #ifdef SESAMVITALE_VERSION
 if (G_pCApp->m_pCps)
    {pCMDI_Ident->m_pFormRubIdent->lineEdit_Nom->setText(G_pCApp->m_pCps->m_Nom);
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Prenom->setText(G_pCApp->m_pCps->m_Prenom);
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setText(G_pCApp->m_pCps->m_Nom);
     pCMDI_Ident->m_pFormRubIdent->lineEdit_NumOrdinal->setText(G_pCApp->m_pCps->m_NIR);
     G_pCApp->m_pCps->m_NumIdentStruct + " "+ G_pCApp->m_pCps->m_NumIdentStructClef;
     /*
     if (G_pCApp->m_pCps->m_TypIdentStruct=="4" && G_pCApp->m_pCps->m_TypeIdNat=="3")
        {pCMDI_Ident->m_pFormRubIdent->lineEdit_NumRpps->setText(G_pCApp->m_pCps->m_NumIdentStruct + "-"+ G_pCApp->m_pCps->m_NumIdentStructClef);
        }
     else
        {pCMDI_Ident->m_pFormRubIdent->lineEdit_NumRpps->setText(G_pCApp->m_pCps->m_NumIdentStruct + "-"+ G_pCApp->m_pCps->m_NumIdentStructClef);
        }
     */
     //m_CriptedPassWord = m_pCMoteurBase->Utf8_Query(query,4);
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Qualif->setText(G_pCApp->m_pCps->codeSpecialiteToString(G_pCApp->m_pCps->m_CodeSpecialite.toInt()));
     pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setFocus();
    }
 #endif
}

//---------------------------------------------- OnSign_NewUserIsCreated ------------------------------------------------------------
// ACTION: Slot activé en réponse au signal: pDockFormUserList,
//         SIGNAL( Sign_NewUserIsCreated(CMDI_Ident* pCMDI_Ident, const QString &ident_prim_key) venant aprés sauvegarde  nouvelle identité.
//         CAD appelé apres un click sur Nouvel utilisateur.
void Gest_User::OnSign_NewUserIsCreated(CMDI_Ident* pCMDI_Ident, const QString &ident_prim_key)
{if (pCMDI_Ident==0)   return;
 if (pCMDI_Ident->m_pFormRubIdent->m_NumGUID !="")
   {OnUserListDocClicked(ident_prim_key, tr("* Observation"), QString::number(TYP_OBSERVATION),"0");
    OnUserListDocClicked(ident_prim_key, tr("* Ordonnance"),  QString::number(TYP_ORDONNANCE),"0");
    OnUserListDocClicked(ident_prim_key, tr("* Certificat"),  QString::number(TYP_CERTIFICAT),"0");
    OnUserListDocClicked(ident_prim_key, tr("Parametres"),    QString::number(TYP_PARAM),"0");

    //................. creer les Ordos  par defaut .....................................
    CMDI_Ordo*           pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique("", TYP_ORDONNANCE);
    if (pCMDI_Ordo)      {pCMDI_Ordo->CreateDocStandard ();              pCMDI_Ordo->SaveDocument();}
    //................. creer les Certif  par defaut .....................................
    CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique("", TYP_CERTIFICAT);
    if (pCMDI_Certificat)      {pCMDI_Certificat->CreateDocStandard ();  pCMDI_Certificat->SaveDocument();
                                pCMDI_Certificat->CreateDocStandard ("[Documents] * Impression Standard");  pCMDI_Certificat->SaveDocument();
                               }
    //................. creer les Observ par defaut.....................................
    CMDI_Observation*           pCMDI_Observation = (CMDI_Observation*) IsExistRubrique("", TYP_OBSERVATION);
    if (pCMDI_Observation)      {pCMDI_Observation->CreateDocStandard (); pCMDI_Observation->SaveDocument();}
    //................. creer les param par defaut .....................................
    CMDI_Param*           pCMDI_Param = (CMDI_Param*) IsExistRubrique("", TYP_PARAM);
    if (pCMDI_Param)     {pCMDI_Param->CreateDocStandard ();              pCMDI_Param->SaveDocument();}
    //............... selectionner cett utilisateur pour l'afficher ....................................................
    QString user =  m_pCMoteurBase->GetFieldValue( m_pCMoteurBase->m_USER_IDENT_TBL_NAME,     //  dans cette table je veux retourver
                                                   m_pCMoteurBase->m_USER_IDENT_LOGIN,        //  la valeur de ce champ
                                                   m_pCMoteurBase->m_USER_IDENT_PRIMKEY,      //  pour ce champ egal
                                                   ident_prim_key );                          //  a cette valeur

    m_pCMoteurBase->SaveUserPassWord(user, pCMDI_Ident->m_pFormRubIdent->m_CriptedPassWord );   // le sauvegarder dans la base
    m_pDockFormUserList->InitUserList();
    m_pActionEnregistrer->setEnabled(FALSE);  // faut le laisser avant SetUser(user);
    m_pDockFormUserList->SetUser(user);
    pCMDI_Ident->m_pFormRubIdent->lineEdit_Login->setEnabled(FALSE);   // seul cas ou il est possible de modifier le login est lors création

    pCMDI_Ident->m_pFormRubIdent->pushButtonSaveNew->setText(tr("En&register les modifications"));
    pCMDI_Ident->m_pFormRubIdent->pushButtonAnnulerCreation->hide();
    pCMDI_Ident->m_pFormRubIdent->lineCreation->hide();
    pCMDI_Ident->m_pFormRubIdent->textLabelCreation->hide();
    pCMDI_Ident->m_pFormRubIdent->pushButtonAppartenance->show();

    pCMDI_Ident->m_IsModified = FALSE;
    if (m_pDockFormUserList)      m_pDockFormUserList->setEnabled(TRUE);
    if (m_pDockFormUserListDoc)   m_pDockFormUserListDoc->setEnabled(TRUE);
    OnRubIsUpdated();
   }
}

//---------------------------------------------- OnUserListDocClicked ------------------------------------------------------------
// ACTION: Slot activé en réponse au signal: pDockFormUserList,
//         SIGNAL( Sign_UserDocSelected(const char* ref_UserPrimKey, const char* name, const char*  type, const char* prim_key)
//         CAD appelé apres un click sur la liste des DOCUMENTS d'un Utilisateur.
void Gest_User::OnUserListDocClicked(const char* ref_UserPrimKey, const char* name, const char*  type, const char* prim_key)
{
 // #define TYP_ORDONNANCE  2
 // #define TYP_CERTIFICAT  3

 switch (atoi(type))
    {case  TYP_ORDONNANCE:
       {CMDI_Ordo*           pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique(ref_UserPrimKey, TYP_ORDONNANCE);
        if (pCMDI_Ordo==0)   pCMDI_Ordo = CMDI_OrdoCreate (ref_UserPrimKey);
        if (pCMDI_Ordo==0)   return;
        pCMDI_Ordo->SetUserPrimKey( ref_UserPrimKey );
        pCMDI_Ordo->SetNameDoc(name);
        pCMDI_Ordo->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(prim_key));
        pCMDI_Ordo->m_IsModified = FALSE;
        GlobalActiverWidget(pCMDI_Ordo);
        break;
       }
     case TYP_CERTIFICAT:
       {
        CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique(ref_UserPrimKey, TYP_CERTIFICAT);
        if (pCMDI_Certificat==0)   pCMDI_Certificat = CMDI_CertificatCreate (ref_UserPrimKey);
        if (pCMDI_Certificat==0)   return;
        pCMDI_Certificat->SetUserPrimKey( ref_UserPrimKey );
        pCMDI_Certificat->SetNameDoc(name);
        pCMDI_Certificat->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(prim_key));
        pCMDI_Certificat->m_IsModified = FALSE;
        GlobalActiverWidget(pCMDI_Certificat);
        break;
       }
    case TYP_OBSERVATION:
       {
        CMDI_Observation*           pCMDI_Observation = (CMDI_Observation*) IsExistRubrique(ref_UserPrimKey, TYP_OBSERVATION);
        if (pCMDI_Observation==0)   pCMDI_Observation = CMDI_ObservationCreate (ref_UserPrimKey);
        if (pCMDI_Observation==0)   return;
        pCMDI_Observation->SetUserPrimKey( ref_UserPrimKey );
        pCMDI_Observation->SetNameDoc(name);
        pCMDI_Observation->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(prim_key));
        pCMDI_Observation->m_IsModified = FALSE;
        GlobalActiverWidget(pCMDI_Observation);
        break;
       }
    case TYP_PARAM:
       {
        CMDI_Param*           pCMDI_Param = (CMDI_Param*) IsExistRubrique(ref_UserPrimKey, TYP_PARAM);
        if (pCMDI_Param==0)   pCMDI_Param = CMDI_ParamCreate (ref_UserPrimKey);
        if (pCMDI_Param==0)   return;
        pCMDI_Param->SetUserPrimKey( ref_UserPrimKey );
        pCMDI_Param->SetNameDoc(name);
        pCMDI_Param->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(prim_key));
        pCMDI_Param->m_IsModified = FALSE;
        GlobalActiverWidget(pCMDI_Param);
        break;
       }
    }
 OnRubIsUpdated();
}
//---------------------------------------------- doYouVantSaveAllBeforeAction ------------------------------------------------------------
// ACTION: Enregistre les modifications avant une nouvelle action.
void Gest_User::doYouVantSaveAllBeforeAction()
{
  if (m_pActionEnregistrer->isEnabled() &&
      QMessageBox::warning( this, tr("Enregistrer les modifications "),
                                 tr("ATTENTION :  Les documents en cours ont été modifiés\n"
                                    "            désirez vous les enregistrer avant d'effectuer\n"
                                    "            Cette action?"),
                                 tr("Enregistrer"),tr("Annuler")
                         ) ==0
    )
    {fileSave();
    }
}

//---------------------------------------------- OnUserChanged ------------------------------------------------------------
// ACTION: Slot activé en réponse au signal: pDockFormUserList, SIGNAL( Sign_UserSelected(const char*)
//         CAD appelé apres un click sur la liste des Utilisateurs.

void Gest_User::OnUserChanged(const char* user_prim_key)
{     if (m_pDockFormUserListDoc) m_pDockFormUserListDoc->setEnabled(TRUE);
      doYouVantSaveAllBeforeAction();
      SetOnUserChanged(user_prim_key);
}
void Gest_User::SetOnUserChanged(const char* user_prim_key)
{
      QListViewItem *item = 0;
      CMDI_Ident *pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
      if (pCMDI_Ident==0)
         {QObject::disconnect( m_pDockFormUserList, SIGNAL( Sign_UserSelected(const char*)),
                          this,                 SLOT  (OnUserChanged(const char*))
                         );
          pCMDI_Ident = CMDI_IdentCreate(user_prim_key);
          QTimer::singleShot( 20, this, SLOT(RestaureUserListConnexion()) );     // obligatoire sinon boucle ....
         }
      if (pCMDI_Ident==0)   return;
      pCMDI_Ident->SetPassWord(G_pCApp->m_CriptedPassWord);   // postionner m_PassWord dans: m_pFormRubIdent
      //........................ actualiser titre de la fenetre .......................................................
      QString login =  m_pCMoteurBase->GetFieldValue( m_pCMoteurBase->m_USER_IDENT_TBL_NAME,     //  dans cette table je veux retourver
                                                      m_pCMoteurBase->m_USER_IDENT_LOGIN,        //  la valeur de ce champ
                                                      m_pCMoteurBase->m_USER_IDENT_PRIMKEY,      //  pour ce champ egal
                                                      user_prim_key );                           //  a cette valeur
      setCaption(tr("Gest_User pour MedinTux utilisé par : ") + login);
      G_pCApp->m_User = login;
      G_pCApp->AdjustGlobalDroits(m_pCMoteurBase->GetUserPermisions( G_pCApp->m_User ));
      SetInterfaceOnDroits();
      //..................... mettre à jour la rubriques sous jacentes Identité.......................................
      pCMDI_Ident->OnIdentChange(user_prim_key);
      G_pCApp->m_CriptedPassWord = pCMDI_Ident->m_pFormRubIdent->m_CriptedPassWord;
      m_pDockFormUserListDoc->SetData(user_prim_key);    // initialise la liste des documents de cet utilisateur
      pCMDI_Ident->m_IsModified = FALSE;                 // mettre ici et pas plus haut !!!
       //..................... mettre à jour la rubriques sous jacentes Ordonnance.......................................
      CMDI_Ordo*                 pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique("", TYP_ORDONNANCE);
      if (pCMDI_Ordo!=0)         pCMDI_Ordo->OnIdentChange(user_prim_key);
      item                     = m_pDockFormUserListDoc->ListViewDoc_FirstTypeFind(TYP_ORDONNANCE, 0);
      if (pCMDI_Ordo)
         {//.......... attribuer l'ordo affichée au nouvel uilisateur (permet copie de l'un à l'autre) .................
          pCMDI_Ordo->SetUserPrimKey( user_prim_key );
          if (item)
             {pCMDI_Ordo->m_pFormRubOrdo->lineEdit_NameDoc->setText(item->text(0));
              pCMDI_Ordo->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(item->text(2)));
              pCMDI_Ordo->m_IsModified = FALSE;
             }
         }

      //..................... mettre à jour la rubriques sous jacentes Certificat.......................................
      CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique("", TYP_CERTIFICAT);
      if (pCMDI_Certificat!=0)   pCMDI_Certificat->OnIdentChange(user_prim_key);
      item                     = m_pDockFormUserListDoc->ListViewDoc_FirstTypeFind(TYP_CERTIFICAT, 0);
      if (pCMDI_Certificat)
         {//........ attribuer le 'certif affiché au nouvel uilisateur (permet copie de l'un à l'autre) ................
          pCMDI_Certificat->SetUserPrimKey( user_prim_key );
          if (item)
             {pCMDI_Certificat->m_pFormRubCertificat->lineEdit_NameCertifDoc->setText(item->text(0));
              pCMDI_Certificat->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(item->text(2)));
              pCMDI_Certificat->m_IsModified = FALSE;
             }
         }
      //..................... mettre à jour la rubriques sous jacentes Observation.......................................
      CMDI_Observation*           pCMDI_Observation = (CMDI_Observation*) IsExistRubrique("", TYP_OBSERVATION);
      if (pCMDI_Observation!=0)   pCMDI_Observation->OnIdentChange(user_prim_key);
      item                     = m_pDockFormUserListDoc->ListViewDoc_FirstTypeFind(TYP_OBSERVATION, 0);
      if (pCMDI_Observation)
         {//........ attribuer le 'certif affiché au nouvel uilisateur (permet copie de l'un à l'autre) ................
          pCMDI_Observation->SetUserPrimKey( user_prim_key );
          if (item)
             {pCMDI_Observation->m_pFormRubObservation->lineEdit_NameDoc->setText(item->text(0));
              pCMDI_Observation->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(item->text(2)));
              pCMDI_Observation->m_IsModified = FALSE;
             }
         }

      //..................... mettre à jour la rubriques sous jacentes Param.......................................
      CMDI_Param*           pCMDI_Param = (CMDI_Param*) IsExistRubrique("", TYP_PARAM);
      if (pCMDI_Param!=0)   pCMDI_Param->OnIdentChange(user_prim_key);
      item                = m_pDockFormUserListDoc->ListViewDoc_FirstTypeFind(TYP_PARAM, 0);
      if (pCMDI_Param)
         {//........ attribuer le 'certif affiché au nouvel uilisateur (permet copie de l'un à l'autre) ................
          pCMDI_Param->SetUserPrimKey( user_prim_key );
          if (item)
             {pCMDI_Param->m_pFormRubParam->lineEdit_NameDoc->setText(item->text(0));
              pCMDI_Param->SetText(m_pCMoteurBase->GetUserDataFromPrimKey(item->text(2)));
              pCMDI_Param->m_IsModified = FALSE;
             }
         }
 if (m_pActionEnregistrer)  m_pActionEnregistrer->setEnabled(FALSE);
}
//---------------------------------------------- RestaureUserListConnexion -------------------------------------------------------------
void Gest_User::RestaureUserListConnexion()
{QObject::connect( m_pDockFormUserList, SIGNAL( Sign_UserSelected(const char*)),
                  this,                 SLOT  (OnUserChanged(const char*))
                );
}

//---------------------------------------------- IsExistRubrique -------------------------------------------------------------
// ACTION: teste si une des fenêtres rubriques dont le nom est fourni en entrée est ouverte dans le WorkSpace des
//         rubriques.
// RETOUR: CMDI_Generic*: qui est  0 si pas de rubrique de ce nom ouverte dans le WorkSpace des rubriques
//                      un pointeur sur cette rubrique dans la liste des rubriques du WorkSpace des rubriques

CMDI_Generic* Gest_User::IsExistRubrique(const char* user_prim_key, int type)     // rubName doit etre: "Observation" "Prescription" "Choix patient" "Identité"
{QWidgetList windows = m_pQWorkSpaceRub->windowList();
 if ( !windows.count() )        return 0;
 QString qsUserprim_key("");                 // reference sur la clef primaire de l'utilisateur proprietaire de la rubrique
 int typ = -1;
 for ( int i = 0; i < int(windows.count()); ++i )
     {QWidget *pQwdgRub   = windows.at(i);
      CMDI_Generic *pCMDI = (CMDI_Generic*)pQwdgRub;
      qsUserprim_key      = pCMDI->GetUserPrimKey();
      typ                 = pCMDI->GetType();
      if (user_prim_key[0]==0 && typ==type)           return pCMDI;
      if (qsUserprim_key==user_prim_key && typ==type) return pCMDI;
     }
 return 0;
}
//---------------------------------------------- OnOngletRubriquesChanged ------------------------------------------------------------
// ACTION: reponse au signal: SIGNAL( currentChanged ( QWidget * ) de la barre d'onglets des rubriques afin d'activer
//         la fenêtre CMDI correspondante.
// ENTREE: QWidget * pQwidget;  fenetre CMDI qui vient d'être activée.

void Gest_User::OnOngletRubriquesChanged(QWidget * pCMDI_Qwidget)
{OnActiverRubrique(pCMDI_Qwidget->name());
}

//---------------------------------------------- OnActiverRubrique ------------------------------------------------------------
// ACTION: reponse au signal:  SIGNAL(Sign_ActiverRubrique(const char*))) provenant des Onglets des rubriques afin
//         d'activer la bonne fenetre CMDI lorsqu'un onglet a ete cliqué.
//         Cette fonction emet elle meme un signal à chaque fenetre CMDI qui doivent s'activer, si c'est leur onglet
//         qui est responsable du signal (reperable par le parametre: const char* rubName qui le texte de l'onglet.
// ENTREE: const char* rubName: texte (celui visible) de l'onglet cliqué.

void Gest_User::OnActiverRubrique(const char* rubName)
{emit Sign_ActiverRubrique(rubName);   // emettre signal aux rubriques filles
}

//---------------------------------------------- OnDeleteRubrique ------------------------------------------------------------
// ACTION: reponse au signal: Sign_RubIsDeleted envoyé par le destructeur d'une rubrique.

void  Gest_User::OnDeleteRubrique(const char* rubName)
{m_pFormOngletRubrique->removePage(rubName);
}

//---------------------------------------------- closeEvent ------------------------------------------------------------
void Gest_User::closeEvent( QCloseEvent* ce )
{doYouVantSaveAllBeforeAction();
  Slot_SauverLesMeubles();
 ce->accept();
}

//---------------------------------------------- setupFileActions ------------------------------------------------------------
void Gest_User::setupFileActions()
{
    QToolBar *tb = new QToolBar( this );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Fichier" ), menu );
    tb->setLabel( tr("Outils Fichiers") );

    QAction *a;
    //a = new QAction( tr( "New" ), QPixmap( "textdrawing/filenew.png" ), tr( "&New..." ), CTRL + Key_N, this, "fileNew" );
    //connect( a, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    //a->addTo( tb );
    //a->addTo( menu );
    //a = new QAction( tr( "Open" ), QPixmap( "textdrawing/fileopen.png" ), tr( "&Open..." ), CTRL + Key_O, this, "fileOpen" );
    //connect( a, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    //a->addTo( tb );
    //a->addTo( menu );
    //menu->insertSeparator();
    m_pActionEnregistrer = new QAction( tr( "Enregistrer" ), Theme::getIcon( "filesave.png"), tr( "Enregi&strer..." ), CTRL + Key_S, this, "fileSave" );
    connect( m_pActionEnregistrer, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    m_pActionEnregistrer->addTo( tb );
    m_pActionEnregistrer->addTo( menu );
    m_pActionEnregistrer->setEnabled(FALSE);
    //a = new QAction( tr( "Save As" ), QPixmap(), tr( "Save &As..." ), 0, this, "fileSaveAs" );
    //connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    //a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Importer une image" ), Theme::getIcon( "ic-flower3.png"), tr( "&Importer une image..." ), 0, this, "InsererImage" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererImage() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    // a = new QAction( tr( "Print" ), QPixmap( fileprint2 ), tr( "&Print..." ), CTRL + Key_P, this, "filePrint" );
    // connect( a, SIGNAL( activated() ), this, SLOT( filePrint() ) );
    // a->addTo( tb );
    // a->addTo( menu );
    a = new QAction( tr( "Quitter" ), Theme::getIcon( "exit.png"), tr( "&Quitter" ), 0, this, "fileClose" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileClose() ) );
    a->addTo( menu );
}


//---------------------------------- setupTextActions ---------------------------------------------------------------------------
// ACTION: met en place la barre de gestion des attributs texte avec les menus correspondant
void Gest_User::setupTextActions()
{   //............................... tool bar des polices de caractere ................................................
    QToolBar     *tb = new QToolBar( this );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "For&mat" ), menu );
    tb->setLabel( tr("Attributs Texte") );
    //...................... combo style .............................
    m_comboStyle = new QComboBox( FALSE, tb );
    m_comboStyle->insertItem( tr("Standard") );
    m_comboStyle->insertItem( tr("Bullet List (Disc)") );
    m_comboStyle->insertItem( tr("Bullet List (Circle)") );
    m_comboStyle->insertItem( tr("Bullet List (Square)") );
    m_comboStyle->insertItem( tr("Ordered List (Decimal)") );
    m_comboStyle->insertItem( tr("Ordered List (Alpha lower)") );
    m_comboStyle->insertItem( tr("Ordered List (Alpha upper)") );
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
    m_pActionTextBold = new QAction( tr( "Gras" ), Theme::getIcon( "textbold.png"), tr( "&Gras" ), CTRL + Key_B, this, "textBold" );
    connect( m_pActionTextBold, SIGNAL( activated() ), this, SLOT( textBold() ) );
    m_pActionTextBold->addTo( tb );
    m_pActionTextBold->addTo( menu );
    m_pActionTextBold->setToggleAction( TRUE );
    m_pActionTextItalic = new QAction( tr( "Italique" ), Theme::getIcon( "textitalic.png"), tr( "&Italique" ), CTRL + Key_I, this, "textItalic" );
    connect( m_pActionTextItalic, SIGNAL( activated() ), this, SLOT( textItalic() ) );
    m_pActionTextItalic->addTo( tb );
    m_pActionTextItalic->addTo( menu );
    m_pActionTextItalic->setToggleAction( TRUE );
    m_pActionTextUnderline = new QAction( tr( "Souligné" ), Theme::getIcon( "textunderline.png"), tr( "So&uligné" ), CTRL + Key_U, this, "textUnderline" );
    connect( m_pActionTextUnderline, SIGNAL( activated() ), this, SLOT( textUnderline() ) );
    m_pActionTextUnderline->addTo( tb );
    m_pActionTextUnderline->addTo( menu );
    m_pActionTextUnderline->setToggleAction( TRUE );
    menu->insertSeparator();

    //............................ justification .......................................
    QActionGroup *grp = new QActionGroup( this );
    grp->setExclusive( TRUE );
    connect( grp, SIGNAL( selected( QAction* ) ), this, SLOT( textAlign( QAction* ) ) );

    m_pActionAlignLeft = new QAction( tr( "Gauche" ), Theme::getIcon( "textleft.png"), tr( "&Gauche" ), CTRL + Key_L, grp, "textLeft" );
    m_pActionAlignLeft->addTo( tb );
    m_pActionAlignLeft->addTo( menu );
    m_pActionAlignLeft->setToggleAction( TRUE );
    m_pActionAlignCenter = new QAction( tr( "Centré" ), Theme::getIcon( "textcenter.png"), tr( "C&entré" ), CTRL + Key_M, grp, "textCenter" );
    m_pActionAlignCenter->addTo( tb );
    m_pActionAlignCenter->addTo( menu );
    m_pActionAlignCenter->setToggleAction( TRUE );
    m_pActionAlignRight = new QAction( tr( "Droite" ), Theme::getIcon( "textright.png"), tr( "&Droite" ), CTRL + Key_R, grp, "textRight" );
    m_pActionAlignRight->addTo( tb );
    m_pActionAlignRight->addTo( menu );
    m_pActionAlignRight->setToggleAction( TRUE );
    m_pActionAlignJustify = new QAction( tr( "Justifié" ), Theme::getIcon( "textjustify.png"), tr( "Justi&fié" ), CTRL + Key_J, grp, "textjustify" );
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

}

//---------------------------------- setupEditActions ---------------------------------------------------------------------------
// ACTION: met en place la barre de gestion des action texte (couper coller etc ...) avec les menus correspondant
void Gest_User::setupEditActions()
{
    QToolBar *tb = new QToolBar( this );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Edition" ), menu );
    tb->setLabel( tr("Outils Texte") );

    QAction *a;
    a = new QAction( tr( "Annuler" ), Theme::getIcon( "undo.png"), tr( "&Annuler" ), CTRL + Key_Z, this, "editUndo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Refaire" ), Theme::getIcon( "redo.png"), tr( "&Refaire" ), CTRL + Key_Y, this, "editRedo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Couper" ), Theme::getIcon( "editcut.png"), tr( "&Couper" ), CTRL + Key_X, this, "editCut" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCut() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Copier" ), Theme::getIcon( "editcopy.png"), tr( "Co&pier" ), CTRL + Key_C, this, "editCopy" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Coller" ), Theme::getIcon( "editpaste.png"), tr( "Co&ller" ), CTRL + Key_V, this, "editPaste" );
    connect( a, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Insérer Un Champ de fusion" ), Theme::getIcon("22x22/InsererChampInsertion.png"), tr( "Insérer Un Cha&mp de fusion..." ), 0, this, "InsererChamp" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererChamp() ) );
    a->addTo( tb );
    a->addTo( menu );
}

//-------------------------------------------- FonctionsAttributsTexte ------------------------------------------------------------

void Gest_User::textFamily( const QString & f)             //............... positionner le texte sur les attributs textes ..........................
{   if ( !currentEditor() )  return;
    currentEditor()->setFamily( f );
    currentEditor()->viewport()->setFocus();
}
void Gest_User::textItalic()
{   if ( !currentEditor() )  return;
    currentEditor()->setItalic( m_pActionTextItalic->isOn() );
}
void Gest_User::textBold()
{   if ( !currentEditor() )  return;
    currentEditor()->setBold( m_pActionTextBold->isOn() );
}
void Gest_User::textUnderline()
{   if ( !currentEditor() )  return;
    currentEditor()->setUnderline( m_pActionTextUnderline->isOn() );
}
void Gest_User::textColor()
{   MyEditText    *pEdit = currentEditor();  // il faut relever editeur courant avant apparition dialogue de couleur !!!
    if ( !pEdit )  return;
    QColor col = QColorDialog::getColor( currentEditor()->color(), this );
    if ( !col.isValid() )    return;
    pEdit->setColor( col );
    QPixmap pix( 16, 16 );
    pix.fill( col );
    m_pActionTextColor->setIconSet( pix );
}
void Gest_User::textSize( const QString & p)
{   if ( !currentEditor() )  return;
    currentEditor()->setPointSize( p.toInt() );
    currentEditor()->viewport()->setFocus();
}
void Gest_User::textAlign( QAction* a)
{   if ( !currentEditor() )  return;
    if ( a == m_pActionAlignLeft )             currentEditor()->setAlignment( AlignLeft );
    else if ( a == m_pActionAlignCenter )      currentEditor()->setAlignment( AlignHCenter );
    else if ( a == m_pActionAlignRight )       currentEditor()->setAlignment( AlignRight );
    else if ( a == m_pActionAlignJustify )     currentEditor()->setAlignment( AlignJustify );
}
void Gest_User::textStyle( int i)
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
void Gest_User::editUndo()
{   if ( !currentEditor() ) return;
    currentEditor()->undo();
}
void Gest_User::editRedo()
{   if ( !currentEditor() ) return;
    currentEditor()->redo();
}
void Gest_User::editCut()
{   if ( !currentEditor() ) return;
    currentEditor()->cut();
}
void Gest_User::editCopy()
{   if ( !currentEditor() ) return;
    currentEditor()->copy();
}
void Gest_User::editPaste()
{   if ( !currentEditor() ) return;
    currentEditor()->paste();
}
void Gest_User::editorChanged( QWidget * )                 //........... Positionner les boutons sur les attributs texte ...............
{   if ( !currentEditor() ) return;
    fontChanged( currentEditor()->font() );
    colorChanged( currentEditor()->color() );
    alignmentChanged( currentEditor()->alignment() );
}
void Gest_User::fontChanged( const QFont &f )
{   m_comboFont->lineEdit()->setText( f.family() );
    m_comboSize->lineEdit()->setText( QString::number( f.pointSize() ) );
    m_pActionTextBold->setOn( f.bold() );
    m_pActionTextItalic->setOn( f.italic() );
    m_pActionTextUnderline->setOn( f.underline() );
}
void Gest_User::colorChanged( const QColor &c )
{   QPixmap pix( 16, 16 );
    pix.fill( c );
    m_pActionTextColor->setIconSet( pix );
}
void Gest_User::alignmentChanged( int a )
{   if      ( ( a == AlignAuto ) || ( a & AlignLeft ))  m_pActionAlignLeft->setOn( TRUE );
    else if ( ( a & AlignHCenter ) )                    m_pActionAlignCenter->setOn( TRUE );
    else if ( ( a & AlignRight ) )                      m_pActionAlignRight->setOn( TRUE );
    else if ( ( a & AlignJustify ) )                    m_pActionAlignJustify->setOn( TRUE );
}
//---------------------------------------------- doConnections --------------------------------------------------------------------
// ACTION: connecte les signaux de changement d'attribut texte d'un editeur de texte aux boutons d'attributs texte afin qu'ils
//         puissent mettre à jour leur etat.

void Gest_User::doConnections( QTextEdit *e )
{
    connect( e,     SIGNAL( currentFontChanged( const QFont & ) ),
             this,  SLOT( fontChanged( const QFont & ) ) );
    connect( e,     SIGNAL( currentColorChanged( const QColor & ) ),
             this,  SLOT( colorChanged( const QColor & ) ) );
    connect( e,     SIGNAL( currentAlignmentChanged( int ) ),
             this,  SLOT( alignmentChanged( int ) ) );

}
//---------------------------------------------- currentEditor ----------------------------------------------------------------------------
// ACTION: retourne un pointeur sur l'editeur de texte de la fenetre MDI courante ou zero si elle n'en a pas.

MyEditText *Gest_User::currentEditor() const
{ CMDI_Generic *pCMDI   = (CMDI_Generic*) m_pQWorkSpaceRub->activeWindow();
  if (pCMDI==0)        return 0;
  return pCMDI->GetEditor();
}
//----------------------------------------------- fileClose -------------------------------------------------------------------------------
void Gest_User::fileClose()
{G_pCApp->quit();
}

//---------------------------------------------------------------------------- InsererChamp ---------------------------------------
void Gest_User::InsererChamp()
{
 MyEditText *edit = (MyEditText*)currentEditor();
 if (!edit)                             return;
 Dlg_InsertList *dlg = new Dlg_InsertList(this, "MyDlgList",TRUE);
 if (dlg == 0) return;
 //........................... recuperer chemin du glossaire dans drtux.ini.................................................
 QString path   = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"");
 QString str_param;
 QString pathFusionListe;
 CGestIni::Param_UpdateFromDisk(path, str_param);   //PathFusionListe
 CGestIni::Param_ReadParam(str_param, "Editeur de masques", "PathFusionListe", &pathFusionListe);
 //........................... former chemin de la liste des champs d'insertion.................................................
  if (QDir(pathFusionListe).isRelative())
    {path   = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true), pathFusionListe);
    }
 else
    {path   = pathFusionListe;
    }
 QString str;
 CGestIni::Param_UpdateFromDisk(path, str);

 str.replace("\r\n","\n");
 QStringList list = QStringList::split ("\n", str);
 dlg->initDialog(list);
 if (dlg->exec() != QDialog::Accepted)   {delete dlg; return;}
 str = dlg->listBox_InsertList->selectedItem()->text();
 CHtmlTools::insertHtml(edit, str,0);
 delete dlg;
}

//---------------------------------------------------------------------------- InsererImage ---------------------------------------
void Gest_User::InsererImage()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  //................. choisir le fichier image src à insérer .................................................
  m_ImagePath = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true), "Ressources/RubriquesTextes" );
  QFileDialog *fd = new QFileDialog  ( m_ImagePath,
                                       tr( "IMG-Files (*.png *.xpm *.jpg *.gif *.bmp );;All Files (*)" ),
                                       this,
                                       "OuvrirDlg"
                                      ) ;
  if (fd==0)                                            return;
  CPreview* p = new CPreview;
  fd->setInfoPreviewEnabled( TRUE );
  fd->setInfoPreview( p, p );
  fd->setPreviewMode( QFileDialog::Info );
  if ( ! (fd->exec() == QDialog::Accepted) )
     {delete fd;
      return;
     }
  QString filename = fd->selectedFile();
  delete fd;
  if ( !filename.isEmpty() && QFile::exists( filename ))
     {filename = CHtmlTools::Copy_File(filename, m_ImagePath);
      CHtmlTools::insertHtml(edit, QString("<img src=\"") + filename + "\">", 0);
     }
}

//---------------------------------- about ---------------------------------------------------------------------------
void Gest_User::about()
{QMessageBox::about( this, tr("Qt Application Example"),
                        tr("This example demonstrates simple use of "
                        "QMainWindow,\nQMenuBar and QToolBar."));
}

//---------------------------------- aboutQt ---------------------------------------------------------------------------
void Gest_User::aboutQt()
{QMessageBox::aboutQt( this, tr("Qt Application Example") );
}

//---------------------------------- OnSign_RubChanged ---------------------------------------------------------------------------
void Gest_User::OnSign_RubChanged()
{if (m_pActionEnregistrer)  m_pActionEnregistrer->setEnabled(TRUE);
}

//---------------------------------- OnRubIsUpdated ---------------------------------------------------------------------------
void Gest_User::OnRubIsUpdated()
{if (m_pActionEnregistrer==0)  return;
 bool isModified = 0;
 CMDI_Ident*                pCMDI_Ident = (CMDI_Ident*)IsExistRubrique("", TYP_IDENTITE);
 if (pCMDI_Ident!=0)        {isModified |= pCMDI_Ident->m_IsModified;}

 CMDI_Ordo*                 pCMDI_Ordo = (CMDI_Ordo*) IsExistRubrique("", TYP_ORDONNANCE);
 if (pCMDI_Ordo!=0)         {isModified |= pCMDI_Ordo->m_IsModified;}

 CMDI_Certificat*           pCMDI_Certificat = (CMDI_Certificat*) IsExistRubrique("", TYP_CERTIFICAT);
 if (pCMDI_Certificat!=0)   {isModified |= pCMDI_Certificat->m_IsModified;}

 CMDI_Observation*          pCMDI_Observation = (CMDI_Observation*) IsExistRubrique("", TYP_OBSERVATION);
 if (pCMDI_Observation!=0)  {isModified |= pCMDI_Observation->m_IsModified;}

 CMDI_Param*                pCMDI_Param   = (CMDI_Param*) IsExistRubrique("", TYP_PARAM);
 if (pCMDI_Param!=0)        {isModified |= pCMDI_Param->m_IsModified;}

 m_pActionEnregistrer->setEnabled(isModified);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Ident ============================================================================

//---------------------------------------------- CMDI_IdentCreate --------------------------------------------------------------
CMDI_Ident* Gest_User::CMDI_IdentCreate (const char* prim_key)
{//................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
    CMDI_Ident* pCMDI_Ident = new CMDI_Ident(m_pQWorkSpaceRub,"CMDI_Ident", WDestructiveClose, m_pCMoteurBase, prim_key);
    connect(pCMDI_Ident, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );
    pCMDI_Ident->setCaption(tr("Identité"));
    //.................. creer tabulation correspondante ...............................................
    m_pFormOngletRubrique->addPanel ( tr("Identité") );
    //.................. connecter DrTux aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par DrTux pour modifier leur contenu
    connect( this,           SIGNAL( Sign_OnIdentChange( const char* )),
             pCMDI_Ident,    SLOT( OnIdentChange( const char* ))
           );
    //.................. connecter DrTux aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par DrTux
    connect( this,           SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_Ident,    SLOT( ActiverRubrique(const char*))
           );
    //.................. connecter les filles au DrTux.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au DrTux qui l'enl�vera de la barre d'onglets
    connect( pCMDI_Ident,   SIGNAL( Sign_RubIsDeleted(const char* )),
             this,          SLOT  ( OnDeleteRubrique (const char* ))
           );

    connect( pCMDI_Ident->m_pFormRubIdent,                    SIGNAL(Sign_SaveButtonClicked()),
             pCMDI_Ident,                                     SLOT  (SaveDocument())
           );
    //....................... Changement de password dans rubrique identite ...........................
    //       repercuté au niveau du DrTux (qui le repercutera ensuite sur les rubriques filles)
    connect( pCMDI_Ident ,                                    SIGNAL(Sign_RubIsUpdated()),
             this,                                            SLOT  (OnRubIsUpdated())
           );
    //....................... Changement de password dans rubrique identite ...........................
    //       repercuté au niveau du DrTux (qui le repercutera ensuite sur les rubriques filles)
    connect( pCMDI_Ident->m_pFormRubIdent,                    SIGNAL(Sign_PassWordChanged(const char*)),
             this,                                            SLOT  (OnPassWordChanged(const char*))
           );

    connect( pCMDI_Ident->m_pFormRubIdent,   SIGNAL(Sign_CancelButtonClicked()),
             this,                           SLOT  (OnNewUserAbort())
           );

    //....................... Changement de password dans DrTux ........................................
    //                        connecté à la fille
    connect( this,                                            SIGNAL(Sign_PassWordChanged(const char*)),
             pCMDI_Ident,                                     SLOT  (SetPassWord(const char* ))
           );
    //....................... Changement d'etat ........................................
    connect( pCMDI_Ident->m_pFormRubIdent,                                     SIGNAL(Sign_RubChanged()),
             pCMDI_Ident,                                                      SLOT  (OnSign_IdentiteChanged())
           );
    connect( pCMDI_Ident,                                                      SIGNAL(Sign_RubChanged()),
             this,                                                             SLOT  (OnSign_RubChanged())
           );
    //....................... Changement de rubrique ........................................
    //                        connecté à la fille
    connect( pCMDI_Ident,                                     SIGNAL(Sign_NewUserIsCreated(CMDI_Ident*, const QString &)),
             this,                                            SLOT  (OnSign_NewUserIsCreated(CMDI_Ident*, const QString &))
           );

    pCMDI_Ident->setGeometry ( QRect( 0, 0, 600, 390 ) );
    pCMDI_Ident->showMaximized();
    return pCMDI_Ident;
}

//---------------------------------- SetPassWord ---------------------------------------------------------------------------
void CMDI_Ident::SetPassWord(const char* password)
{m_pFormRubIdent->m_CriptedPassWord = password;
}
//---------------------------------- CMDI_Observation -----------------------------------------------------------------------------
void CMDI_Ident::OnSign_IdentiteChanged()
{m_IsModified = TRUE;
 emit Sign_RubChanged();
}
//---------------------------------- CMDI_Ident --------------------------------------------------------------------------------
CMDI_Ident::CMDI_Ident( QWidget* parent, const char* name, int wflags,  CMoteurBase *pCMoteurBase, const char* user_prim_key)
    : CMDI_Generic( parent, name, wflags, pCMoteurBase)
{   SetUserPrimKey( user_prim_key);
    SetType(TYP_IDENTITE);
    m_pFormRubIdent = new FormRubIdent(this, "RubIdentite" );
    if (m_pFormRubIdent)
       {m_pFormRubIdent->m_pCMoteurBase = pCMoteurBase;
        if (user_prim_key)    m_pFormRubIdent->m_ID_Doss    = user_prim_key;
        else                  m_pFormRubIdent->m_ID_Doss    = "";
        m_pFormRubIdent->SetData();
        m_pFormRubIdent->pushButtonAnnulerCreation->hide();
        m_pFormRubIdent->lineCreation->hide();
        m_pFormRubIdent->textLabelCreation->hide();
        m_pFormRubIdent->lineCreation->setProperty( "frameShape", "HLine" );    //corriger bug QT de petite ligne
       }

    setFocusProxy( m_pFormRubIdent );
    setCentralWidget( m_pFormRubIdent );
}

//---------------------------------- ~CMDI_Ident -------------------------------------------------------------------------------
CMDI_Ident::~CMDI_Ident()
{emit Sign_RubIsDeleted(tr("Identité"));
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Ident::OnIdentChange(const char* prim_key)
{ m_pFormRubIdent->m_ID_Doss    = prim_key;
  SetUserPrimKey( prim_key);
  m_pFormRubIdent->SetData();
}

//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Ident::ActiverRubrique(const char *rubName)
{if (strncmp(rubName,"Ident",5)!=0) return;
 GlobalActiverWidget(this);
}

//---------------------------------- ClearData ---------------------------------------------------------------------------
void CMDI_Ident::ClearData()
{m_pFormRubIdent->clearData();
}

//---------------------------------- PrepareNewGUID ---------------------------------------------------------------------------
void CMDI_Ident::PrepareNewGUID()
{ m_pFormRubIdent->m_NumGUID  = m_pFormRubIdent->m_pCMoteurBase->GUID_Create();
}

//---------------------------------- IsValidToCreateOrUpdate ---------------------------------------------------------------------------
int CMDI_Ident::IsValidToCreateOrUpdate()
{QString mess(tr("Les Champs suivants: "));
 if (m_pFormRubIdent->lineEdit_Nom->text()     == "")    mess += tr("Nom , ");
 if (m_pFormRubIdent->lineEdit_Prenom->text()  == "")    mess += tr("Prénom , ");
 if (m_pFormRubIdent->lineEdit_Login->text()   == "")    mess += tr("Login , ");
 if (mess==tr("Les Champs suivants: ")) return TRUE;
 mess += tr(" sont vides veuillez les compléter");
 QMessageBox::critical( this, tr("Erreur: Saisie imcomplète"),
                              mess,
                              tr("Annuler")
                       ) ;
 return FALSE;
}

//---------------------------------- IsToCreate ---------------------------------------------------------------------------
int CMDI_Ident::IsToCreate()
{return m_pFormRubIdent->m_NumGUID=="";
}

//---------------------------------- VerifyPassWordAndSaveDocument ---------------------------------------------------------------------------
int  CMDI_Ident::VerifyPassWordAndSaveDocument()
{ return SaveDocument();
}

//---------------------------------- SaveDocument ---------------------------------------------------------------------------
int  CMDI_Ident::SaveDocument()
{
 if (!IsValidToCreateOrUpdate()) return 0;
 //............... Si valide creer l'enregistrement ............................
 if (IsToCreate())
    {
     CreateNewUser();
     //................. recuperer prim key de ce nouvel utilisateur ........................................................

     QString pk =    m_pFormRubIdent->m_pCMoteurBase->GetUserPrimKeyFromGUID(m_pFormRubIdent->m_NumGUID);
     m_pFormRubIdent->SavePermsUserList(m_pFormRubIdent->m_NumGUID);
     //................. emettre signal pour créer documents par défaut ........................................................
     emit Sign_NewUserIsCreated(this, pk);
     return 2;
    }
 m_pFormRubIdent->SavePermsUserList(m_pFormRubIdent->m_NumGUID);
 UpdateUser();
 //................. cas de creation avec existant .......................................
 if ( m_pFormRubIdent->lineEdit_Login->isEnabled() )
    {QString pk =    m_pFormRubIdent->m_pCMoteurBase->GetUserPrimKeyFromGUID(m_pFormRubIdent->m_NumGUID);
     emit Sign_NewUserIsCreated(this, pk);
    }
 return 1;
}

//---------------------------------- CreateNewUser ---------------------------------------------------------------------------
int CMDI_Ident::CreateNewUser()
{//............... ultra important d'assigner ici: m_NumGUID à la nouvelle valeur de creation .........................
 m_pFormRubIdent->m_NumGUID = m_pFormRubIdent->m_pCMoteurBase->CreateNewUser(m_pFormRubIdent->lineEdit_Nom->text(),
                                                                             m_pFormRubIdent->lineEdit_Prenom->text(),
                                                                             m_pFormRubIdent->lineEdit_Login->text()
                                                                            );
 return UpdateUser();
}

//---------------------------------- UpdateUser ---------------------------------------------------------------------------
int CMDI_Ident::UpdateUser()
{if (m_pFormRubIdent->m_NumGUID !="")
    {m_IsModified = 0;
     int ret      = m_pFormRubIdent->UpdateUserDataRecord(1);
     emit Sign_RubIsUpdated();
     return ret;
    }
 return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Observation =======================================================================
//---------------------------------------------- CMDI_ObservationCreate --------------------------------------------------------------
CMDI_Observation* Gest_User::CMDI_ObservationCreate (const char* prim_key)
{//................... creer la  MDI (fenetres Certificat) ...........................
    CMDI_Observation* pCMDI_Observation = new CMDI_Observation(m_pQWorkSpaceRub,"CMDI_"OBSERV_NAME, WDestructiveClose, m_pCMoteurBase, prim_key);
    //.................. creer tabulation correspondante ...............................................
    m_pFormOngletRubrique->addPanel ( OBSERV_NAME );

    //.................. connecter Gest_User aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par Gest_User pour modifier leur contenu
    connect( this,                  SIGNAL( Sign_OnIdentChange( const char* )),
             pCMDI_Observation,     SLOT( OnIdentChange( const char* ))
           );
    //.................. connecter Gest_User aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par Gest_User
    connect( this,                  SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_Observation,     SLOT( ActiverRubrique(const char*))
           );
    //.................. connecter les filles au Gest_User.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au Gest_User qui l'enl�vera de la barre d'onglets
    connect( pCMDI_Observation,   SIGNAL( Sign_RubIsDeleted(const char* )),
             this,                SLOT  ( OnDeleteRubrique (const char* ))
           );
    //....................... Changement de password dans rubrique identite ...........................
    //       repercuté au niveau du DrTux (qui le repercutera ensuite sur les rubriques filles)
    connect( pCMDI_Observation ,                                    SIGNAL(Sign_RubIsUpdated()),
             this,                                                  SLOT  (OnRubIsUpdated())
           );
    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( pCMDI_Observation,              SIGNAL(Sign_RubListMustBeUpdated(const char*)),
             m_pDockFormUserListDoc,         SLOT(SetData(const char* ))
           );
    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( m_pDockFormUserListDoc,                    SIGNAL(Sign_RubListMustBeDeleted(const char*)),
             pCMDI_Observation,                         SLOT(DeleteDocument(const char* ))
           );

    doConnections(pCMDI_Observation->m_pMyEditTextHead);  // connecter l'editeur de texte aux boutons d'indication d'etat
    doConnections(pCMDI_Observation->m_pMyEditTextFoot);  // connecter l'editeur de texte aux boutons d'indication d'etat
    pCMDI_Observation->m_pMyEditTextHead->mimeSourceFactory()->addFilePath(m_ImagePath );
    pCMDI_Observation->m_pMyEditTextFoot->mimeSourceFactory()->addFilePath(m_ImagePath );

    connect( pCMDI_Observation->m_pMyEditTextHead, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                                 SLOT(InsererChamp())
           );
    connect( pCMDI_Observation->m_pMyEditTextFoot, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                                 SLOT(InsererChamp())
           );
    //.................................. detection des changements au sein de la classe.........................................................
    connect( pCMDI_Observation->m_pFormRubObservation->lineEdit_NameDoc,               SIGNAL(textChanged(const QString &)),
             pCMDI_Observation,                                                        SLOT  (OnSign_ObservationChanged())
           );
    connect( pCMDI_Observation->m_pMyEditTextFoot,                                     SIGNAL(textChanged()),
             pCMDI_Observation,                                                        SLOT  (OnSign_ObservationChanged())
           );
    connect( pCMDI_Observation->m_pMyEditTextHead,                                     SIGNAL(textChanged()),
             pCMDI_Observation,                                                        SLOT  (OnSign_ObservationChanged())
           );
    //.................................. detection des changements au sein de gest_user.........................................................
    connect( pCMDI_Observation,                                                SIGNAL(Sign_RubChanged()),
             this,                                                             SLOT  (OnSign_RubChanged())
           );

    pCMDI_Observation->setGeometry ( QRect( 0, 0, 530, 340 ) );
    pCMDI_Observation->showMaximized();
    return pCMDI_Observation;
}
//---------------------------------- CMDI_Observation -----------------------------------------------------------------------------
void CMDI_Observation::OnSign_ObservationChanged()
{m_IsModified = TRUE;
 emit Sign_RubChanged();
}

//---------------------------------- CMDI_Observation -----------------------------------------------------------------------------
CMDI_Observation::CMDI_Observation( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* user_prim_key /* = 0 */)
    : CMDI_Generic( parent, name, wflags , pCMoteurBase)
{   //.............................. placer attributs ....................................................
    SetUserPrimKey( user_prim_key);
    SetType(TYP_OBSERVATION);
    setCaption(tr(OBSERV_NAME));

    m_LastMyEditFocus = 0;

    //............... placer en premier un gestionnaire vertical de QWidget ..............................
    //                en fils de la fenetre m�re:
    //                les enfants de ce gestionnaire seront:
    //                             le dialogue: m_pFormRubPrescription    qui sera en haut
    //                             l'éditeur de texte: m_pMyEditText      qui sera en dessous
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 5 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //......................... creation du dialogue:  m_pFormRubObservation .............................
    m_pFormRubObservation = new FormRubObservation(pQVCentral, "Rub"OBSERV_NAME );
    if (m_pFormRubObservation)
       {int pos_y = 0;
        m_pFormRubObservation->move(QPoint ( 2,  2));
        m_pFormRubObservation->setGeometry ( QRect( 2, 2, m_pFormRubObservation->frameSize().width(), 40 ) );
        pos_y +=m_pFormRubObservation->frameSize().height () + 1;

        connect( m_pFormRubObservation->pushButtonNew,   SIGNAL( clicked ()),
                 this,                                   SLOT  ( CreateDocStandard ())
               );

        connect( m_pFormRubObservation->but_File,        SIGNAL( clicked ()),
                 this,                                   SLOT  ( CreateDocFromFile ())
               );

        m_pFormRubObservation->pushButtonNew->setIconSet ( QIconSet (Theme::getIcon("16x16/NewDoc.png"))) ;
        m_pFormRubObservation->but_File->setIconSet ( QIconSet (Theme::getIcon("fileopen.png"))) ;

        //.................... creation de l'éditeur de texte: m_pMyEditText...............................
        //                     à ratacher au membre: MyEditText *m_pMyEditText de :  m_pFormRubCertificat
        //                     afin d'en disposer à l'interieur de sa classe.
        QSplitter *splitter = new QSplitter( pQVCentral, "splitter52" );
        splitter->setOrientation( QSplitter::Vertical );

        QVBox *pQVBoxRectoHead = new QVBox( splitter );
        QLabel *pQlabel = new QLabel(pQVBoxRectoHead, "pQlabel_1");
        pQlabel->setText( tr("----------------------------------- OBSERVATION  (En-tête) -----------------------------------"));
        pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubObservation->frameSize().width()-20, 15 ) );
        pQlabel->setAlignment (Qt::AlignHCenter);
        pos_y +=pQlabel->frameSize().height () + 1;

        m_pMyEditTextHead = new MyEditText( pQVBoxRectoHead);
        m_pMyEditTextHead->setWordWrap ( QTextEdit::NoWrap );
        pos_y += m_pMyEditTextHead->frameSize().height () + 3;

        QVBox *pQVBoxRectoFoot = new QVBox( splitter );
        pQlabel = new QLabel(pQVBoxRectoFoot, "pQlabel_2");
        pQlabel->setText( tr("----------------------------------- OBSERVATION (Pied) --------------------------------------"));
        pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubObservation->frameSize().width()-20, 15 ) );
        pQlabel->setAlignment (Qt::AlignHCenter);
        pos_y +=pQlabel->frameSize().height () + 1;

        m_pMyEditTextFoot = new MyEditText( pQVBoxRectoFoot);
        m_pMyEditTextFoot->setWordWrap ( QTextEdit::NoWrap );

        pos_y += m_pMyEditTextFoot->frameSize().height () + 3;

        CMDI_DoConnection(m_pMyEditTextHead);
        CMDI_DoConnection(m_pMyEditTextFoot);
        m_LastMyEditFocus = m_pMyEditTextHead;
       }
    setCentralWidget( pQVCentral );
}



//---------------------------------- ~CMDI_Observation ------------------------------------------------------------------------
CMDI_Observation::~CMDI_Observation()
{emit Sign_RubIsDeleted(OBSERV_NAME);
}

//---------------------------------- CMDI_DoConnection -----------------------------------------------------------------------------
// ACTION: permet de noter lors de chaque changement de focus de chaque editeur le dernier  actif
//         LE SIGNAL: Sign_EditGotFocus( MyEditText *)  est emis chaque fois que l'editeur de texte attrape
//         le focus et alors la connection sur le slot: Slot_EditGotFocus( MyEditText *) permet à la classe
//         CMDI_Ordo de noter le dernier editeur de texte ayant eu le focus

void CMDI_Observation::CMDI_DoConnection(MyEditText* e)
{connect( e,      SIGNAL( Sign_EditGotFocus( MyEditText *)),
          this,   SLOT  ( Slot_EditGotFocus( MyEditText *))
        );
}

//---------------------------------- GetEditor ------------------------------------------------------------------------------------
MyEditText *CMDI_Observation::GetEditor()
{return m_LastMyEditFocus;
}

//---------------------------------- CreateDocStandard ------------------------------------------------------------------------------------
void CMDI_Observation::CreateDocStandard ()
{QString entete, pied;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/ObservEntete.html" , entete);
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/ObservPied.html" ,   pied);
 entete.prepend("=={{");
 entete.append("}}==/r/n");
 pied.prepend("=={{");
 pied.append("}}==/r/n");
 SetText( entete+pied);
}


//---------------------------------- CreateDocFromFile ------------------------------------------------------------------------------------
void CMDI_Observation::CreateDocFromFile()
{
  QString file  = "";
  QString doc   = "";
  if (m_LastMyEditFocus)
     { QString message = "";
       if      (m_LastMyEditFocus == m_pMyEditTextHead) message = TR("Choisissez l'en-tête de l'observation");
       else if (m_LastMyEditFocus == m_pMyEditTextFoot) message = TR("Choisissez le pied de page de l'observation");
       file  = QFileDialog::getOpenFileName(
                    G_pCApp->m_PathAppli + "Pieds-Entetes",
                    "Fichiers HTML (*.htm *.html)",
                    this,
                    "open file dialog",
                    message );
       if (!file.isEmpty() )
          {CGestIni::Param_UpdateFromDisk(file, doc);
           m_LastMyEditFocus->setText(doc);
          }
     }
 else
    { QMessageBox::critical( this, tr("Chargement d'un fichier"),
                              tr("Veuillez auparavant positionner le curseur texte sur le texte pour lequel vous désirez charger un document.") ,
                              tr("Annuler")
                       ) ;
    }
}



//---------------------------------- SetText ------------------------------------------------------------------------------------
void CMDI_Observation::SetText(QString text)
{int pos_end  = text.find("}}==/r/n=={{");
 int pos_deb  = 4;
 if (pos_end == -1)    return;
 m_pMyEditTextHead->setText(text.mid (pos_deb, pos_end-pos_deb ));

 pos_deb      = text.find("=={{", pos_end);
 if (pos_deb  == -1)   return;
 pos_deb     += 4;
 pos_end      = text.find("}}==/r/n", pos_deb);
 if (pos_end  ==-1) return;
 m_pMyEditTextFoot->setText(text.mid (pos_deb, pos_end-pos_deb ));

}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Observation::OnIdentChange(const char* user_prim_key)
{SetUserPrimKey(user_prim_key);
}

//---------------------------------- SetNameDoc -----------------------------------------------------------------------------
void CMDI_Observation::SetNameDoc(QString text)
{m_pFormRubObservation->lineEdit_NameDoc->setText(text);
}

//---------------------------------- VerifyPassWordAndSaveDocument ---------------------------------------------------------------------------
void  CMDI_Observation::VerifyPassWordAndSaveDocument()
{
 QString       user = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                    m_pCMoteurBase->m_USER_IDENT_PRIMKEY,
                                                    GetUserPrimKey());
 if (m_pCMoteurBase->VerifyUserPassWord(user, G_pCApp->m_CriptedPassWord))
    {SaveDocument();
    }
 else
    { QMessageBox::critical( this, tr("Erreur : Mot de passe"),
                              tr("Echec de l'enregistrement car mot de passe incorrect") ,
                              tr("Annuler")
                       ) ;
    }
}

//---------------------------------- SaveDocument ---------------------------------------------------------------------------
void CMDI_Observation::SaveDocument()
{QString name = m_pFormRubObservation->lineEdit_NameDoc->text();
 if (name != "")
    {QString data("=={{");
     data += m_pMyEditTextHead->text() + "}}==/r/n=={{";
     data += m_pMyEditTextFoot->text()   + "}}==/r/n";

     QString docPrimKey  = m_pCMoteurBase->IsThisUserDocExist(TYP_OBSERVATION,  GetUserPrimKey(), name);
     if (docPrimKey!="")
        {m_pCMoteurBase->UpdateUserDataFromPrimKey(docPrimKey, TYP_OBSERVATION, GetUserPrimKey(), name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
     else
        {m_pCMoteurBase->CreateUserDataFromPrimKey(TYP_OBSERVATION, GetUserPrimKey() , name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
     m_IsModified = 0;
    }
}

//---------------------------------- DeleteDocument ---------------------------------------------------------------------------
void CMDI_Observation::DeleteDocument(const char* primKey)
{m_pCMoteurBase->DeleteUserDataFromPrimKey( primKey );
 emit Sign_RubListMustBeUpdated(GetUserPrimKey());
}

//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Observation::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, OBSERV_NAME, 5)!=0) return;
 GlobalActiverWidget(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Param =============================================================================
//---------------------------------------------- CMDI_ParamCreate --------------------------------------------------------------
CMDI_Param* Gest_User::CMDI_ParamCreate (const char* prim_key)
{   //................... creer la  MDI (fenetres Certificat) ...........................
    CMDI_Param* pCMDI_Param = new CMDI_Param(m_pQWorkSpaceRub,"CMDI_"PARAM_NAME, WDestructiveClose, m_pCMoteurBase, prim_key);
    //.................. creer tabulation correspondante ...............................................
    m_pFormOngletRubrique->addPanel ( PARAM_NAME );

    //.................. connecter Gest_User aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par Gest_User pour modifier leur contenu
    connect( this,                  SIGNAL( Sign_OnIdentChange( const char* )),
             pCMDI_Param,           SLOT( OnIdentChange( const char* ))
           );
    //.................. connecter Gest_User aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par Gest_User
    connect( this,                  SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_Param,           SLOT( ActiverRubrique(const char*))
           );
    //.................. connecter les filles au Gest_User.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au Gest_User qui l'enl�vera de la barre d'onglets
    connect( pCMDI_Param,         SIGNAL( Sign_RubIsDeleted(const char* )),
             this,                SLOT  ( OnDeleteRubrique (const char* ))
           );
    //....................... Changement de password dans rubrique identite ...........................
    //       repercuté au niveau du DrTux (qui le repercutera ensuite sur les rubriques filles)
    connect( pCMDI_Param ,                                          SIGNAL(Sign_RubIsUpdated()),
             this,                                                  SLOT  (OnRubIsUpdated())
           );
    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( pCMDI_Param,                    SIGNAL(Sign_RubListMustBeUpdated(const char*)),
             m_pDockFormUserListDoc,         SLOT(SetData(const char* ))
           );
    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( m_pDockFormUserListDoc,                    SIGNAL(Sign_RubListMustBeDeleted(const char*)),
             pCMDI_Param,                               SLOT(DeleteDocument(const char* ))
           );
    //.................................. detection des changements au sein de la classe.........................................................
    connect( pCMDI_Param->m_pMyEditText,                                               SIGNAL(textChanged()),
             pCMDI_Param,                                                              SLOT  (OnSign_ParamChanged())
           );
    //.................................. detection des changements au sein de gest_user.........................................................
    connect( pCMDI_Param,                                                      SIGNAL(Sign_RubChanged()),
             this,                                                             SLOT  (OnSign_RubChanged())
           );
    doConnections(pCMDI_Param->m_pMyEditText);  // connecter l'editeur de texte aux boutons d'indication d'etat

    pCMDI_Param->setGeometry ( QRect( 0, 0, 530, 340 ) );
    pCMDI_Param->showMaximized();
    return pCMDI_Param;
}

//---------------------------------- CMDI_Param -----------------------------------------------------------------------------
CMDI_Param::CMDI_Param( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* user_prim_key /* = 0 */)
    : CMDI_Generic( parent, name, wflags , pCMoteurBase)
{   //.............................. placer attributs ....................................................
    SetUserPrimKey( user_prim_key);
    SetType(TYP_PARAM);
    setCaption(tr(PARAM_NAME));

    m_LastMyEditFocus = 0;

    //............... placer en premier un gestionnaire vertical de QWidget ..............................
    //                en fils de la fenetre m�re:
    //                les enfants de ce gestionnaire seront:
    //                             le dialogue: m_pFormRubPrescription    qui sera en haut
    //                             l'éditeur de texte: m_pMyEditText      qui sera en dessous
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 5 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //......................... creation du dialogue:  m_pFormRubParam .............................
    m_pFormRubParam = new FormRubParam(pQVCentral, "Rub"PARAM_NAME );
    if (m_pFormRubParam)
       {int pos_y = 0;
        m_pFormRubParam->move(QPoint ( 2,  2));
        m_pFormRubParam->setGeometry ( QRect( 2, 2, m_pFormRubParam->frameSize().width(), 40 ) );
        pos_y +=m_pFormRubParam->frameSize().height () + 1;

        connect( m_pFormRubParam->pushButtonNew,   SIGNAL( clicked ()),
                 this,                             SLOT  ( CreateDocStandard ())
               );
        m_pFormRubParam->pushButtonNew->setIconSet ( QIconSet (Theme::getIcon("16x16/NewDoc.png"))) ;
        //but_File->setIconSet ( QIconSet (Theme::getIcon("fileopen.png"))) ;

        //.................... creation de l'éditeur de texte: m_pMyEditText...............................
        //                     à ratacher au membre: MyEditText *m_pMyEditText de :  m_pFormRubCertificat
        //                     afin d'en disposer à l'interieur de sa classe.
        m_pMyEditText = new MyEditText( pQVCentral);
        m_pMyEditText->setTextFormat ( Qt::PlainText );
        m_pMyEditText->setWordWrap ( QTextEdit::NoWrap );

        pos_y += m_pMyEditText->frameSize().height () + 3;
        CMDI_DoConnection(m_pMyEditText);
       }
    setCentralWidget( pQVCentral );
}



//---------------------------------- ~CMDI_Param ------------------------------------------------------------------------
CMDI_Param::~CMDI_Param()
{emit Sign_RubIsDeleted(PARAM_NAME);
}

//---------------------------------- OnSign_ParamChanged -----------------------------------------------------------------------------
void CMDI_Param::OnSign_ParamChanged()
{m_IsModified = TRUE;
 emit Sign_RubChanged();
}

//---------------------------------- CMDI_DoConnection -----------------------------------------------------------------------------
// ACTION: permet de noter lors de chaque changement de focus de chaque editeur le dernier  actif
//         LE SIGNAL: Sign_EditGotFocus( MyEditText *)  est emis chaque fois que l'editeur de texte attrape
//         le focus et alors la connection sur le slot: Slot_EditGotFocus( MyEditText *) permet à la classe
//         CMDI_Ordo de noter le dernier editeur de texte ayant eu le focus

void CMDI_Param::CMDI_DoConnection(MyEditText* e)
{connect( e,      SIGNAL( Sign_EditGotFocus( MyEditText *)),
          this,   SLOT  ( Slot_EditGotFocus( MyEditText *))
        );
}

//---------------------------------- GetEditor ------------------------------------------------------------------------------------
MyEditText *CMDI_Param::GetEditor()
{return m_LastMyEditFocus;
}

//---------------------------------- CreateDocStandard ------------------------------------------------------------------------------------
void CMDI_Param::CreateDocStandard ()
{QString text;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/Parametres.txt" , text);
 SetText( text);
}

//---------------------------------- SetText ------------------------------------------------------------------------------------
void CMDI_Param::SetText(QString text)
{m_pMyEditText->setText(text);
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.
void CMDI_Param::OnIdentChange(const char* user_prim_key)
{SetUserPrimKey(user_prim_key);
}

//---------------------------------- SetNameDoc -----------------------------------------------------------------------------
void CMDI_Param::SetNameDoc(QString text)
{m_pFormRubParam->lineEdit_NameDoc->setText(text);
}

//---------------------------------- VerifyPassWordAndSaveDocument ---------------------------------------------------------------------------
void  CMDI_Param::VerifyPassWordAndSaveDocument()
{
 QString       user = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                    m_pCMoteurBase->m_USER_IDENT_PRIMKEY,
                                                    GetUserPrimKey());
 if (m_pCMoteurBase->VerifyUserPassWord(user, G_pCApp->m_CriptedPassWord))
    {SaveDocument();
    }
 else
    { QMessageBox::critical( this, tr("Erreur : Mot de passe"),
                              tr("Echec de l'enregistrement car mot de passe incorrect") ,
                              tr("Annuler")
                       ) ;
    }
}

//---------------------------------- SaveDocument ---------------------------------------------------------------------------
void CMDI_Param::SaveDocument()
{QString name = m_pFormRubParam->lineEdit_NameDoc->text();
 if (name != "")
    {QString data        = m_pMyEditText->text();
     QString docPrimKey  = m_pCMoteurBase->IsThisUserDocExist(TYP_PARAM,  GetUserPrimKey(), name);
     if (docPrimKey!="")
        {m_pCMoteurBase->UpdateUserDataFromPrimKey(docPrimKey, TYP_PARAM, GetUserPrimKey(), name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
     else
        {m_pCMoteurBase->CreateUserDataFromPrimKey(TYP_PARAM, GetUserPrimKey() , name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
    m_IsModified = 0;
    }
}

//---------------------------------- DeleteDocument ---------------------------------------------------------------------------
void CMDI_Param::DeleteDocument(const char* primKey)
{m_pCMoteurBase->DeleteUserDataFromPrimKey( primKey );
 emit Sign_RubListMustBeUpdated(GetUserPrimKey());
}

//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Param::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, PARAM_NAME, 5)!=0) return;
 GlobalActiverWidget(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Certificat =======================================================================
//---------------------------------------------- CMDI_CertificatCreate --------------------------------------------------------------
CMDI_Certificat* Gest_User::CMDI_CertificatCreate (const char* prim_key)
{//................... creer la  MDI (fenetres Certificat) ...........................
    CMDI_Certificat* pCMDI_Certificat = new CMDI_Certificat(m_pQWorkSpaceRub,"CMDI_"CERTIF_NAME, WDestructiveClose, m_pCMoteurBase, prim_key);
    //.................. creer tabulation correspondante ...............................................
    m_pFormOngletRubrique->addPanel ( CERTIF_NAME );

    //.................. connecter Gest_User aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par Gest_User pour modifier leur contenu
    connect( this,                 SIGNAL( Sign_OnIdentChange( const char* )),
             pCMDI_Certificat,     SLOT( OnIdentChange( const char* ))
           );
    //.................. connecter Gest_User aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par Gest_User
    connect( this,                 SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_Certificat,     SLOT( ActiverRubrique(const char*))
           );
    //.................. connecter les filles au Gest_User.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au Gest_User qui l'enl�vera de la barre d'onglets
    connect( pCMDI_Certificat,   SIGNAL( Sign_RubIsDeleted(const char* )),
             this,               SLOT  ( OnDeleteRubrique (const char* ))
           );

    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( pCMDI_Certificat,              SIGNAL(Sign_RubListMustBeUpdated(const char*)),
             m_pDockFormUserListDoc,        SLOT(SetData(const char* ))
           );
    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( m_pDockFormUserListDoc,                    SIGNAL(Sign_RubListMustBeDeleted(const char*)),
             pCMDI_Certificat,                          SLOT(DeleteDocument(const char* ))
           );
    //....................... Changement de password dans rubrique identite ...........................
    //       repercuté au niveau du DrTux (qui le repercutera ensuite sur les rubriques filles)
    connect( pCMDI_Certificat ,                                          SIGNAL(Sign_RubIsUpdated()),
             this,                                                       SLOT  (OnRubIsUpdated())
           );
    doConnections(pCMDI_Certificat->m_pMyEditTextHead);  // connecter l'editeur de texte aux boutons d'indication d'etat
    doConnections(pCMDI_Certificat->m_pMyEditTextFoot);  // connecter l'editeur de texte aux boutons d'indication d'etat

    pCMDI_Certificat->setGeometry ( QRect( 0, 0, 530, 340 ) );
    pCMDI_Certificat->showMaximized();
    pCMDI_Certificat->m_pMyEditTextHead->mimeSourceFactory()->addFilePath(m_ImagePath );
    pCMDI_Certificat->m_pMyEditTextFoot->mimeSourceFactory()->addFilePath(m_ImagePath );
    connect( pCMDI_Certificat->m_pMyEditTextHead, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                                 SLOT(InsererChamp())
           );
    connect( pCMDI_Certificat->m_pMyEditTextFoot, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                                 SLOT(InsererChamp())
           );
    //.................................. detection des changements au sein de la classe.........................................................
    connect( pCMDI_Certificat->m_pFormRubCertificat->lineEdit_NameCertifDoc,          SIGNAL(textChanged(const QString &)),
             pCMDI_Certificat,                                                        SLOT  (OnSign_CertificatChanged())
           );
    connect( pCMDI_Certificat->m_pMyEditTextFoot,                                     SIGNAL(textChanged()),
             pCMDI_Certificat,                                                        SLOT  (OnSign_CertificatChanged())
           );
    connect( pCMDI_Certificat->m_pMyEditTextHead,                                     SIGNAL(textChanged()),
             pCMDI_Certificat,                                                        SLOT  (OnSign_CertificatChanged())
           );
    //.................................. detection des changements au sein de gest_user.........................................................
    connect( pCMDI_Certificat,                                                 SIGNAL(Sign_RubChanged()),
             this,                                                             SLOT  (OnSign_RubChanged())
           );
    return pCMDI_Certificat;
}

//---------------------------------- CMDI_Certificat -----------------------------------------------------------------------------
CMDI_Certificat::CMDI_Certificat( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* user_prim_key /* = 0 */)
    : CMDI_Generic( parent, name, wflags , pCMoteurBase)
{   //.............................. placer attributs ....................................................
    SetUserPrimKey( user_prim_key);
    SetType(TYP_CERTIFICAT);
    setCaption(tr(CERTIF_NAME));

    m_LastMyEditFocus = 0;

    //............... placer en premier un gestionnaire vertical de QWidget ..............................
    //                en fils de la fenetre m�re:
    //                les enfants de ce gestionnaire seront:
    //                             le dialogue: m_pFormRubPrescription    qui sera en haut
    //                             l'éditeur de texte: m_pMyEditText      qui sera en dessous
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 5 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //......................... creation du dialogue:  m_pFormRubObservation .............................
    m_pFormRubCertificat = new FormRubCertificat(pQVCentral, "Rub"CERTIF_NAME );
    if (m_pFormRubCertificat)
       {int pos_y = 0;
        m_pFormRubCertificat->move(QPoint ( 2,  2));
        m_pFormRubCertificat->setGeometry ( QRect( 2, 2, m_pFormRubCertificat->frameSize().width(), 40 ) );
        pos_y +=m_pFormRubCertificat->frameSize().height () + 1;

        connect( m_pFormRubCertificat->pushButtonNew,         SIGNAL( clicked ()),
                 this,                                        SLOT  ( CreateDocStandard ())
               );
        connect( m_pFormRubCertificat->but_File,              SIGNAL( clicked ()),
                 this,                                        SLOT  ( CreateDocFromFile ())
               );
        m_pFormRubCertificat->pushButtonNew->setIconSet ( QIconSet (Theme::getIcon("16x16/NewDoc.png"))) ;
        m_pFormRubCertificat->but_File->setIconSet ( QIconSet (Theme::getIcon("fileopen.png"))) ;

        //.................... creation de l'éditeur de texte: m_pMyEditText...............................
        //                     à ratacher au membre: MyEditText *m_pMyEditText de :  m_pFormRubCertificat
        //                     afin d'en disposer à l'interieur de sa classe.
        QSplitter *splitter = new QSplitter( pQVCentral, "splitter56" );
        splitter->setOrientation( QSplitter::Vertical );

        QVBox *pQVBoxRectoHead = new QVBox( splitter );
        QLabel *pQlabel = new QLabel(pQVBoxRectoHead, "pQlabel_1");
        pQlabel->setText( tr("----------------------------------- CERTIFICAT  (En-tête) -----------------------------------"));
        pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubCertificat->frameSize().width()-20, 15 ) );
        pQlabel->setAlignment (Qt::AlignHCenter);
        pos_y +=pQlabel->frameSize().height () + 1;

        m_pMyEditTextHead = new MyEditText( pQVBoxRectoHead);
        pos_y += m_pMyEditTextHead->frameSize().height () + 3;
        m_pMyEditTextHead->setWordWrap ( QTextEdit::NoWrap );

        QVBox *pQVBoxRectoFoot = new QVBox( splitter );
        pQlabel = new QLabel(pQVBoxRectoFoot, "pQlabel_2");
        pQlabel->setText( tr("----------------------------------- CERTIFICAT (Pied) --------------------------------------"));
        pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubCertificat->frameSize().width()-20, 15 ) );
        pQlabel->setAlignment (Qt::AlignHCenter);
        pos_y +=pQlabel->frameSize().height () + 1;

        m_pMyEditTextFoot = new MyEditText( pQVBoxRectoFoot);
        pos_y += m_pMyEditTextFoot->frameSize().height () + 3;
        m_pMyEditTextFoot->setWordWrap ( QTextEdit::NoWrap );

        CMDI_DoConnection(m_pMyEditTextHead);
        CMDI_DoConnection(m_pMyEditTextFoot);
        m_LastMyEditFocus = m_pMyEditTextHead;
       }
    setCentralWidget( pQVCentral );
}



//---------------------------------- ~CMDI_Certificat ------------------------------------------------------------------------
CMDI_Certificat::~CMDI_Certificat()
{emit Sign_RubIsDeleted(CERTIF_NAME);
}
//---------------------------------- OnSign_CertificatChanged -----------------------------------------------------------------------------
void CMDI_Certificat::OnSign_CertificatChanged()
{m_IsModified = TRUE;
 emit Sign_RubChanged();
}

//---------------------------------- CMDI_DoConnection -----------------------------------------------------------------------------
// ACTION: permet de noter lors de chaque changement de focus de chaque editeur le dernier  actif
//         LE SIGNAL: Sign_EditGotFocus( MyEditText *)  est emis chaque fois que l'editeur de texte attrape
//         le focus et alors la connection sur le slot: Slot_EditGotFocus( MyEditText *) permet à la classe
//         CMDI_Ordo de noter le dernier editeur de texte ayant eu le focus

void CMDI_Certificat::CMDI_DoConnection(MyEditText* e)
{connect( e,      SIGNAL( Sign_EditGotFocus( MyEditText *)),
          this,   SLOT  ( Slot_EditGotFocus( MyEditText *))
        );
}

//---------------------------------- GetEditor ------------------------------------------------------------------------------------
MyEditText *CMDI_Certificat::GetEditor()
{return m_LastMyEditFocus;
}

//---------------------------------- CreateDocStandard ------------------------------------------------------------------------------------
void CMDI_Certificat::CreateDocStandard (const QString &modeleName)
{QString entete, pied;
 if (modeleName.length()==0)
    {CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/CertifEntete.html" , entete);
     CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/CertifPied.html" ,   pied);
     entete.prepend("=={{");
     entete.append("}}==/r/n");
     pied.prepend("=={{");
     pied.append("}}==/r/n");
     SetText( entete+pied);
    }
 else
    {QString name = modeleName;
     SetNameDoc(modeleName);
     name = name.replace('*','%');
     name = name.replace('[','{');
     name = name.replace(']','}');
     // [Documents] * Impression Standard
     // {Documents} % Impression Standard~Entete.html
     // {Documents} % Impression Standard~Pied.html
     CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/" + name + "~Entete.html", entete);
     CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/" + name + "~Pied.html",   pied);
     entete.prepend("=={{");
     entete.append("}}==/r/n");
     pied.prepend("=={{");
     pied.append("}}==/r/n");
     SetText( entete+pied);
    }
}


//---------------------------------- CreateDocFromFile ------------------------------------------------------------------------------------
void CMDI_Certificat::CreateDocFromFile()
{ QString file  = "";
  QString doc   = "";
  if (m_LastMyEditFocus)
     { QString message = "";
       if      (m_LastMyEditFocus == m_pMyEditTextHead) message = TR("Choisissez l'en-tête à placer.");
       else if (m_LastMyEditFocus == m_pMyEditTextFoot) message = TR("Choisissez le pied de page à placer.");
       file  = QFileDialog::getOpenFileName(
                    G_pCApp->m_PathAppli + "Pieds-Entetes",
                    "Fichiers HTML (*.htm *.html)",
                    this,
                    "open file dialog",
                    message );
       if (!file.isEmpty() )
          {CGestIni::Param_UpdateFromDisk(file, doc);
           m_LastMyEditFocus->setText(doc);
          }
     }
 else
    { QMessageBox::critical( this, tr("Chargement d'un fichier"),
                              tr("Veuillez auparavant positionner le curseur texte sur le texte pour lequel vous désirez charger un document.") ,
                              tr("Annuler")
                       ) ;
    }
}


//---------------------------------- SetText ------------------------------------------------------------------------------------
void CMDI_Certificat::SetText(QString text)
{int pos_end  = text.find("}}==/r/n=={{");
 int pos_deb  = 4;
 if (pos_end == -1)    return;
 m_pMyEditTextHead->setText(text.mid (pos_deb, pos_end-pos_deb ));

 pos_deb      = text.find("=={{", pos_end);
 if (pos_deb  == -1)   return;
 pos_deb     += 4;
 pos_end      = text.find("}}==/r/n", pos_deb);
 if (pos_end  ==-1) return;
 m_pMyEditTextFoot->setText(text.mid (pos_deb, pos_end-pos_deb ));
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Certificat::OnIdentChange(const char* user_prim_key)
{SetUserPrimKey(user_prim_key);
}

//---------------------------------- SetNameDoc -----------------------------------------------------------------------------
void CMDI_Certificat::SetNameDoc(QString text)
{m_pFormRubCertificat->lineEdit_NameCertifDoc->setText(text);
}

//---------------------------------- VerifyPassWordAndSaveDocument ---------------------------------------------------------------------------
void  CMDI_Certificat::VerifyPassWordAndSaveDocument()
{
 QString       user = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                    m_pCMoteurBase->m_USER_IDENT_PRIMKEY,
                                                    GetUserPrimKey());
 if (m_pCMoteurBase->VerifyUserPassWord(user, G_pCApp->m_CriptedPassWord))
    {SaveDocument();
    }
 else
    { QMessageBox::critical( this, tr("Erreur : Mot de passe"),
                                   tr("Echec de l'enregistrement car mot de passe incorrect") ,
                                   tr("Annuler")
                       ) ;
    }
}

//---------------------------------- SaveDocument ---------------------------------------------------------------------------
void CMDI_Certificat::SaveDocument()
{QString name = m_pFormRubCertificat->lineEdit_NameCertifDoc->text();
 if (name != "")
    {QString data("=={{");
     data += m_pMyEditTextHead->text() + "}}==/r/n=={{";
     data += m_pMyEditTextFoot->text()   + "}}==/r/n";

     QString docPrimKey  = m_pCMoteurBase->IsThisUserDocExist(TYP_CERTIFICAT, GetUserPrimKey(), name);
     if (docPrimKey!="")
        {m_pCMoteurBase->UpdateUserDataFromPrimKey(docPrimKey, TYP_CERTIFICAT, GetUserPrimKey(), name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
     else
        {m_pCMoteurBase->CreateUserDataFromPrimKey(TYP_CERTIFICAT, GetUserPrimKey() , name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
     m_IsModified = 0;
    }
}

//---------------------------------- DeleteDocument ---------------------------------------------------------------------------
void CMDI_Certificat::DeleteDocument(const char* primKey)
{m_pCMoteurBase->DeleteUserDataFromPrimKey( primKey );
 emit Sign_RubListMustBeUpdated(GetUserPrimKey());
}

//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Certificat::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, CERTIF_NAME, 5)!=0) return;
 GlobalActiverWidget(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Ordo =======================================================================

//---------------------------------------------- CMDI_OrdoCreate --------------------------------------------------------------
CMDI_Ordo* Gest_User::CMDI_OrdoCreate (const char* prim_key)
{//................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
    CMDI_Ordo* pCMDI_Ordo = new CMDI_Ordo(m_pQWorkSpaceRub,"CMDI_"ORDO_NAME, WDestructiveClose, m_pCMoteurBase, prim_key);

    //.................. creer tabulation correspondante ...............................................
    m_pFormOngletRubrique->addPanel ( ORDO_NAME );
    //.................. connecter Gest_User aux fenetres filles:  Sign_OnIdentChange ......................................
    //                   elles pouront recevoir le changement d'identité
    //                   repercuté par Gest_User pour modifier leur contenu
    connect( this,           SIGNAL( Sign_OnIdentChange( const char* )),
             pCMDI_Ordo,     SLOT( OnIdentChange( const char* ))
           );
    //.................. connecter Gest_User aux fenetres filles: Sign_ActiverRubrique ......................................
    //                   elles pouront recevoir le message d'activation de leur fenetre
    //                   repercuté par Gest_User
    connect( this,           SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_Ordo,     SLOT( ActiverRubrique(const char*))
           );
    //.................. connecter les filles au Gest_User.............................................
    //                   elles pouront envoyer et notifier leur destruction
    //                   au Gest_User qui l'enl�vera de la barre d'onglets
    connect( pCMDI_Ordo,   SIGNAL( Sign_RubIsDeleted(const char* )),
             this,         SLOT  ( OnDeleteRubrique (const char* ))
           );

    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( pCMDI_Ordo,              SIGNAL(Sign_RubListMustBeUpdated(const char*)),
             m_pDockFormUserListDoc,  SLOT(SetData(const char* ))
           );
    //.................. connecter l'ordo a la liste des documents.............................................
    //                   qui poura se mettre à jour en cas de nouveau document
    connect( m_pDockFormUserListDoc,              SIGNAL(Sign_RubListMustBeDeleted(const char*)),
             pCMDI_Ordo,                          SLOT(DeleteDocument(const char* ))
           );
    //....................... Changement de password dans rubrique identite ...........................
    //       repercuté au niveau du DrTux (qui le repercutera ensuite sur les rubriques filles)
    connect( pCMDI_Ordo ,                                                SIGNAL(Sign_RubIsUpdated()),
             this,                                                       SLOT  (OnRubIsUpdated())
           );
    doConnections(pCMDI_Ordo->m_pMyEditRecto_Entete);  // connecter l'editeur de texte aux boutons d'indication d'etat
    doConnections(pCMDI_Ordo->m_pMyEditRecto_Pied);    // connecter l'editeur de texte aux boutons d'indication d'etat
    doConnections(pCMDI_Ordo->m_pMyEditDupli_Entete);  // connecter l'editeur de texte aux boutons d'indication d'etat
    doConnections(pCMDI_Ordo->m_pMyEditDupli_Pied);    // connecter l'editeur de texte aux boutons d'indication d'etat
    pCMDI_Ordo->m_pMyEditRecto_Entete->mimeSourceFactory()->addFilePath(m_ImagePath );
    pCMDI_Ordo->m_pMyEditRecto_Pied->mimeSourceFactory()->addFilePath(m_ImagePath );
    pCMDI_Ordo->m_pMyEditDupli_Entete->mimeSourceFactory()->addFilePath(m_ImagePath );
    pCMDI_Ordo->m_pMyEditDupli_Pied->mimeSourceFactory()->addFilePath(m_ImagePath );

    connect( pCMDI_Ordo->m_pMyEditRecto_Entete, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                              SLOT(InsererChamp())
           );
    connect( pCMDI_Ordo->m_pMyEditRecto_Pied, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                            SLOT(InsererChamp())
           );
    connect( pCMDI_Ordo->m_pMyEditDupli_Entete, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                              SLOT(InsererChamp())
           );
    connect( pCMDI_Ordo->m_pMyEditDupli_Pied, SIGNAL(Sign_EditFusionFieldRequest()),
             this,                            SLOT(InsererChamp())
           );

     //.................................. detection des changements au sein de la classe.........................................................
    connect( pCMDI_Ordo->m_pFormRubOrdo->lineEdit_NameDoc,                      SIGNAL(textChanged(const QString &)),
             pCMDI_Ordo,                                                        SLOT  (OnSign_OrdonnanceChanged())
           );
    connect( pCMDI_Ordo->m_pMyEditRecto_Entete,                                 SIGNAL(textChanged()),
             pCMDI_Ordo,                                                        SLOT  (OnSign_OrdonnanceChanged())
           );
    connect( pCMDI_Ordo->m_pMyEditRecto_Pied,                                  SIGNAL(textChanged()),
             pCMDI_Ordo,                                                        SLOT  (OnSign_OrdonnanceChanged())
           );
    connect( pCMDI_Ordo->m_pMyEditDupli_Entete,                                 SIGNAL(textChanged()),
             pCMDI_Ordo,                                                        SLOT  (OnSign_OrdonnanceChanged())
           );
    connect( pCMDI_Ordo->m_pMyEditDupli_Pied,                                   SIGNAL(textChanged()),
             pCMDI_Ordo,                                                        SLOT  (OnSign_OrdonnanceChanged())
           );
    //.................................. detection des changements au sein de gest_user.........................................................
    connect( pCMDI_Ordo,                                                       SIGNAL(Sign_RubChanged()),
             this,                                                             SLOT  (OnSign_RubChanged())
           );
    pCMDI_Ordo->showMaximized();

    return pCMDI_Ordo;
}


//---------------------------------- CMDI_Ordo -----------------------------------------------------------------------------
CMDI_Ordo::CMDI_Ordo( QWidget* parent, const char* name, int wflags,  CMoteurBase *pCMoteurBase, const char* user_prim_key)
    : CMDI_Generic( parent, name, wflags , pCMoteurBase)
{   SetUserPrimKey( user_prim_key);
    SetType(TYP_ORDONNANCE);
    setCaption(tr(ORDO_NAME));

    m_LastMyEditFocus  = 0;
    //............... placer en premier un contener de QWidget .............................................
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 5 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    //......................... creation du dialogue:  m_pFormRubObservation .............................
    int pos_y = 0;
    m_pFormRubOrdo = new FormRubOrdo(pQVCentral,"FormRubOrdo");
    if (!m_pFormRubOrdo)              return;
    m_pFormRubOrdo->move(QPoint ( 2,  2));
    m_pFormRubOrdo->setGeometry ( QRect( 2, 2, m_pFormRubOrdo->frameSize().width(), 40 ) );
    pos_y +=m_pFormRubOrdo->frameSize().height () + 1;

    connect( m_pFormRubOrdo->pushButtonNew,   SIGNAL( clicked ()),
                 this,                        SLOT  ( CreateDocStandard ())
               );
    connect( m_pFormRubOrdo->but_File,        SIGNAL( clicked ()),
                 this,                        SLOT  ( CreateDocFromFile ())
               );
    m_pFormRubOrdo->pushButtonNew->setIconSet ( QIconSet (Theme::getIcon("16x16/NewDoc.png"))) ;
    m_pFormRubOrdo->but_File->setIconSet ( QIconSet (Theme::getIcon("fileopen.png"))) ;
    //pushButtonCertifNew
    QSplitter *splitter = new QSplitter( pQVCentral, "splitter2" );
    splitter->setOrientation( QSplitter::Vertical );

    QVBox *pQVBoxRectoHead = new QVBox( splitter );
    QLabel *pQlabel    = new QLabel(pQVBoxRectoHead, "pQlabel1");
    pQlabel->setText( tr("----------------------------------- ORDONNANCE ORIGINAL (En-tête) -----------------------------------"));
    pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubOrdo->frameSize().width()-20, 15 ) );
    pQlabel->setAlignment (Qt::AlignHCenter);
    pos_y +=pQlabel->frameSize().height () + 1;

    m_pMyEditRecto_Entete = new MyEditText( pQVBoxRectoHead );
    if (m_pMyEditRecto_Entete)
       {m_pMyEditRecto_Entete->setGeometry ( QRect( 0, 0, m_pFormRubOrdo->frameSize().width()-20, 250 ) );
        m_pMyEditRecto_Entete->move(QPoint ( 10,  pos_y));
        m_pMyEditRecto_Entete->setWordWrap ( QTextEdit::NoWrap );
        pos_y += m_pMyEditRecto_Entete->frameSize().height () + 3;
       }

    QVBox *pQVBoxRectofoot = new QVBox( splitter );
    pQlabel = new QLabel(pQVBoxRectofoot, "pQlabel2");
    pQlabel->setText( tr("----------------------------------- ORDONNANCE ORIGINAL (Pied de page) -----------------------------------"));
    pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubOrdo->frameSize().width()-20, 15 ) );
    pQlabel->setAlignment (Qt::AlignHCenter);
    pos_y +=pQlabel->frameSize().height () + 1;

    m_pMyEditRecto_Pied = new MyEditText( pQVBoxRectofoot );
    if (m_pMyEditRecto_Pied)
       {m_pMyEditRecto_Pied->setGeometry ( QRect( 0, 0, m_pFormRubOrdo->frameSize().width()-20, 100 ) );
        m_pMyEditRecto_Pied->move(QPoint ( 10,  pos_y));
        m_pMyEditRecto_Pied->setWordWrap ( QTextEdit::NoWrap );
        pos_y += m_pMyEditRecto_Pied->frameSize().height () + 3;
       }

    QVBox *pQVBoxVersoHead = new QVBox( splitter );
    pQlabel = new QLabel(pQVBoxVersoHead, "pQlabel3");
    pQlabel->setText( tr("----------------------------------- ORDONNANCE DUPLICATA (En-tête) -----------------------------------"));
    pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubOrdo->frameSize().width()-20, 15 ) );
    pQlabel->setAlignment (Qt::AlignHCenter);
    pos_y +=pQlabel->frameSize().height () + 1;

    m_pMyEditDupli_Entete = new MyEditText( pQVBoxVersoHead );
    if (m_pMyEditDupli_Entete)
       {m_pMyEditDupli_Entete->setGeometry ( QRect( 0, 0, m_pFormRubOrdo->frameSize().width()-20, 250 ) );
        m_pMyEditDupli_Entete->move(QPoint ( 10,  pos_y));
        m_pMyEditDupli_Entete->setWordWrap ( QTextEdit::NoWrap );
        pos_y += m_pMyEditDupli_Entete->frameSize().height () + 3;
       }

    QVBox *pQVBoxVersoFoot = new QVBox( splitter );
    pQlabel = new QLabel(pQVBoxVersoFoot, "pQlabel4");
    pQlabel->setText( tr("----------------------------------- ORDONNANCE DUPLICATA (Pied de page) -----------------------------------"));
    pQlabel->setGeometry ( QRect( 3, pos_y, m_pFormRubOrdo->frameSize().width()-20, 15 ) );
    pQlabel->setAlignment (Qt::AlignHCenter);
    pos_y +=pQlabel->frameSize().height () + 1;

    m_pMyEditDupli_Pied = new MyEditText( pQVBoxVersoFoot );
    if (m_pMyEditDupli_Pied)
       {m_pMyEditDupli_Pied->setGeometry ( QRect( 0, 0, m_pFormRubOrdo->frameSize().width()-20, 100 ) );
        m_pMyEditDupli_Pied->move(QPoint ( 10,  pos_y));
        m_pMyEditDupli_Pied->setWordWrap ( QTextEdit::NoWrap );
        pos_y += m_pMyEditDupli_Pied->frameSize().height () + 3;
       }
    setGeometry ( QRect( 0, 0, m_pFormRubOrdo->frameSize().width()+25, 400 ) );
    setCentralWidget(pQVCentral);
    //............. connection pour que chaque editeru fasse connaitre quand ila a le focus ...................
    CMDI_DoConnection(m_pMyEditRecto_Entete);
    CMDI_DoConnection(m_pMyEditRecto_Pied);
    CMDI_DoConnection(m_pMyEditDupli_Entete);
    CMDI_DoConnection(m_pMyEditDupli_Pied);
    m_LastMyEditFocus = m_pMyEditRecto_Entete;
}


//---------------------------------- ~CMDI_Ordo -----------------------------------------------------------------------------
CMDI_Ordo::~CMDI_Ordo()
{emit Sign_RubIsDeleted(ORDO_NAME);
}

//---------------------------------- OnSign_OrdonnanceChanged -----------------------------------------------------------------------------
void CMDI_Ordo::OnSign_OrdonnanceChanged()
{m_IsModified = TRUE;
 emit Sign_RubChanged();
}

//---------------------------------- CMDI_DoConnection -----------------------------------------------------------------------------
// ACTION: permet de noter lors de chaque changement de focus de chaque editeur le dernier  actif
//         LE SIGNAL: Sign_EditGotFocus( MyEditText *)  est emis chaque fois que l'editeur de texte attrape
//         le focus et alors la connection sur le slot: Slot_EditGotFocus( MyEditText *) permet à la classe
//         CMDI_Ordo de noter le dernier editeur de texte ayant eu le focus

void CMDI_Ordo::CMDI_DoConnection(MyEditText* e)
{connect( e,      SIGNAL( Sign_EditGotFocus( MyEditText *)),
          this,   SLOT  ( Slot_EditGotFocus( MyEditText *))
        );
}

//---------------------------------- GetEditor ------------------------------------------------------------------------------------
MyEditText *CMDI_Ordo::GetEditor()
{return m_LastMyEditFocus;
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Ordo::OnIdentChange(const char* user_prim_key)
{SetUserPrimKey(user_prim_key);
}

//---------------------------------- SetNameDoc -----------------------------------------------------------------------------
void CMDI_Ordo::SetNameDoc(QString text)
{m_pFormRubOrdo->lineEdit_NameDoc->setText(text);
}

//---------------------------------- SetData ------------------------------------------------------------------------------------
void CMDI_Ordo::SetData()
{//if (m_ID_Doss)
 //   {m_pCMoteurBase->GetUserData(TYP_ORDONNANCE, m_ID_Doss);
 //   }
}

//---------------------------------- CreateDocStandard ------------------------------------------------------------------------------------
void CMDI_Ordo::CreateDocStandard ()
{QString entete, pied,dup_entete, dup_pied;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/OrdoEntete.html" , entete);
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/OrdoPied.html" ,   pied);
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/DupliEntete.html" , dup_entete);
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Images/DupliPied.html" ,   dup_pied);
 entete.prepend("{{");
 entete.append("}} /r/n");
 pied.prepend("{{");
 pied.append("}} /r/n");
 dup_entete.prepend("{{");
 dup_entete.append("}} /r/n");
 dup_pied.prepend("{{");
 dup_pied.append("}} /r/n");
 SetText( entete+pied+dup_entete+dup_pied);
}

//---------------------------------- CreateDocFromFile ------------------------------------------------------------------------------------
void CMDI_Ordo::CreateDocFromFile()
{ QString file  = "";
  QString doc   = "";
  if (m_LastMyEditFocus)
     { QString message = "";
       if      (m_LastMyEditFocus == m_pMyEditRecto_Entete) message = TR("Choisissez l'en-tête page recto  à placer.");
       else if (m_LastMyEditFocus == m_pMyEditRecto_Pied)   message = TR("Choisissez le pied de page recto à placer.");
       else if (m_LastMyEditFocus == m_pMyEditDupli_Entete) message = TR("Choisissez l'en-tête duplicata à placer.");
       else if (m_LastMyEditFocus == m_pMyEditDupli_Pied)   message = TR("Choisissez le pied de page duplicata à placer.");

       file  = QFileDialog::getOpenFileName(
                    G_pCApp->m_PathAppli + "Pieds-Entetes",
                    "Fichiers HTML (*.htm *.html)",
                    this,
                    "open file dialog",
                    message );
       if (!file.isEmpty() )
          {CGestIni::Param_UpdateFromDisk(file, doc);
           m_LastMyEditFocus->setText(doc);
          }
     }
 else
    { QMessageBox::critical( this, tr("Chargement d'un fichier"),
                              tr("Veuillez auparavant positionner le curseur texte sur le texte pour lequel vous désirez charger un document.") ,
                              tr("Annuler")
                       ) ;
    }
}


//---------------------------------- SetText ------------------------------------------------------------------------------------
void CMDI_Ordo::SetText(QString text)
{int pos_end  = text.find("}} /r/n{{");
 int pos_deb  = 2;
 if (pos_end == -1)    return;
 m_pMyEditRecto_Entete->setText(text.mid (pos_deb, pos_end-pos_deb ));

 pos_deb      = text.find("{{", pos_end);
 if (pos_deb  == -1)   return;
 pos_deb     += 2;
 pos_end      = text.find("}} /r/n{{", pos_deb);
 if (pos_end  ==-1) return;
 m_pMyEditRecto_Pied->setText(text.mid (pos_deb, pos_end-pos_deb ));

 pos_deb      = text.find("{{", pos_end);
 if (pos_deb  == -1)   return;
 pos_deb     += 2;
 pos_end      = text.find("}} /r/n{{", pos_deb);
 if (pos_end  ==-1) return;
 m_pMyEditDupli_Entete->setText(text.mid (pos_deb, pos_end-pos_deb ));

 pos_deb      = text.find("{{", pos_end);
 if (pos_deb  == -1)   return;
 pos_deb     += 2;
 pos_end      = text.find("}} /r/n", pos_deb);
 if (pos_end  ==-1) return;
 m_pMyEditDupli_Pied->setText(text.mid (pos_deb, pos_end-pos_deb ));
}

//---------------------------------- VerifyPassWordAndSaveDocument ---------------------------------------------------------------------------
void  CMDI_Ordo::VerifyPassWordAndSaveDocument()
{
 QString       user = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                    m_pCMoteurBase->m_USER_IDENT_PRIMKEY,
                                                    GetUserPrimKey());
 if (m_pCMoteurBase->VerifyUserPassWord(user, G_pCApp->m_CriptedPassWord))
    {SaveDocument();
    }
 else
    { QMessageBox::critical( this, tr("Erreur : Mot de passe"),
                              tr("Echec de l'enregistrement car mot de passe incorrect") ,
                              tr("Annuler")
                       ) ;
    }
}

//---------------------------------- SaveDocument ---------------------------------------------------------------------------
void CMDI_Ordo::SaveDocument()
{QString name = m_pFormRubOrdo->lineEdit_NameDoc->text();
 if (name != "")
    {QString data("{{");
     data += m_pMyEditRecto_Entete->text() + "}} /r/n{{";
     data += m_pMyEditRecto_Pied->text()   + "}} /r/n{{";
     data += m_pMyEditDupli_Entete->text() + "}} /r/n{{";
     data += m_pMyEditDupli_Pied->text()   + "}} /r/n";
     QString docPrimKey  = m_pCMoteurBase->IsThisUserDocExist(TYP_ORDONNANCE, GetUserPrimKey(), name);
     if (docPrimKey!="")
        {m_pCMoteurBase->UpdateUserDataFromPrimKey(docPrimKey, TYP_ORDONNANCE, GetUserPrimKey(), name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
     else
        {m_pCMoteurBase->CreateUserDataFromPrimKey(TYP_ORDONNANCE, GetUserPrimKey() , name, data);
         emit Sign_RubListMustBeUpdated(GetUserPrimKey());
        }
      m_IsModified = 0;
    }
}

//---------------------------------- DeleteDocument ---------------------------------------------------------------------------
void CMDI_Ordo::DeleteDocument(const char* primKey)
{m_pCMoteurBase->DeleteUserDataFromPrimKey( primKey );
 emit Sign_RubListMustBeUpdated(GetUserPrimKey());
}

//---------------------------------- ActiverRubrique ----------------------------------------------------------------------------
void CMDI_Ordo::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, ORDO_NAME, 5)!=0) return;
  GlobalActiverWidget(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== GLOBAL ============================================================================

//---------------------------------- GlobalActiverWidget -----------------------------------------------------------------------
void GlobalActiverWidget(QWidget *pQWidget)
{pQWidget->showMaximized();
}

