/************************************** med_stat.cpp *******************************
 *                                                                                *
 *   #include"med_stat.h"                                                          *
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
#include <qvbox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qeventloop.h>
#include <qlistview.h>

#include "CApp.h"
#include "med_stat.h"

#include "../../MedinTuxTools/cdlg_passwordget.h"
#include "../../MedinTuxTools/Theme.h"

#include "DlgPassChange.h"
#include "Dlg_GetCodePorteur.h"

#include "Dlg_NewUser.h"
//#include "Wdg_Monitor.h"
#include "Wdg_User.h"
#include "Wdg_Central.h"

//---------------------------------------------- ~med_stat ----------------------------------------------------------------------
med_stat::~med_stat()
{
}
//----------------------------------------------- med_stat -------------------------------------------------------------
med_stat::med_stat()
    : QMainWindow( 0, "med_stat", WDestructiveClose )
{
    m_Apropos_Proc = 0;

    //.............................. OK la base est initialisée on peut continuer ...............................
    setCaption("");   // si appelé avec chaine vide, cela affiche les infos complêtes de connexion
    setIcon( G_pCApp->m_PathAppli + "Images/med_stat.png" );

    //.................................................................................................
    QDockWindow *pDock;

    //......................... Docker dialogue User......................................
    pDock = new QDockWindow (this, "DockFormUser");
    pDock->setResizeEnabled(TRUE);
    pDock->setCloseMode (QDockWindow::Always);
    pDock->setCaption(tr("Utilisateur"));
    addToolBar( pDock, Qt::DockLeft );
    m_pWdg_User          = new Wdg_User(pDock, "Wdg_User");
    pDock->setWidget( m_pWdg_User );
    //m_pWdg_User->SetUser(G_pCApp->m_User, G_pCApp->m_SignUser);
    pDock->setFixedExtentWidth (200);
    connect( m_pWdg_User, SIGNAL( Sign_UserSelected ( QListViewItem *, QListViewItem * )),  this,   SLOT(   OnUserSelected(QListViewItem *, QListViewItem * ))  );
    connect( m_pWdg_User, SIGNAL( Sign_UserMustBeEdited (  )),                              this,   SLOT(   Slot_EditUser())   );
    connect( m_pWdg_User, SIGNAL( Sign_ComboUserTypClicked (const QString&  )),             this,   SLOT(   Slot_ComboUserTypClicked(const QString &))   );
    connect( G_pCApp,     SIGNAL( Sign_QuitterRequired()),                                  this,   SLOT(   Slot_SauverLesMeubles()));

    /*
    //......................... Docker dialogue du monitor ......................................
    pDock = new QDockWindow (this, "DockFormMonitor");
    pDock->setResizeEnabled(TRUE);
    pDock->setFixedExtentHeight ( 80 );
    pDock->setCloseMode (QDockWindow::Always);
    pDock->setCaption(tr("Monitor"));
    addToolBar( pDock, Qt::DockBottom );
    m_pWdg_Monitor          = new Wdg_Monitor(pDock, "Wdg_Monitor");
    pDock->setWidget( m_pWdg_Monitor );
    */
    //................... creation du QVbox central ...............................................
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin( 0 );
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //................... y placer une barre d'onglets .............................................
    //................... y placer un QWorkspace ..................................................
    //                    qui contiendra et gerera les fenetres MDI
    m_pQWorkSpace     = new QWorkspace(pQVCentral);
    m_pQWorkSpace->setScrollBarsEnabled( TRUE );
    setCentralWidget(  pQVCentral );

    m_CMDI_Identification = new CMDI_Identification (m_pQWorkSpace, "CMDI_Identification", WDestructiveClose, G_pCApp->m_pCMoteurBase);

    //.................. connecter carte CPS au widget central .............................................................
#ifdef SESAMVITALE_VERSION
    connect(m_pWdg_User ,                   SIGNAL( Sign_CPS_IsClicked ()),
                  this,                     SLOT(   Slot_CPS_IsClicked ())
           );
#endif
    setupFileActions();
    statusBar()->message( tr("Prêt"), 2000 );
    statusBar()->hide();
    G_pCApp->m_pMed_stat = this;    // si on en arrive là c'est que tout est OK
   QString val1, val2, val3, val4;
   if (CGestIni::Param_ReadParam(G_pCApp->m_LocalParam,"ManagerPos", "Positions", &val1, &val2, &val3, &val4)==0)  // zero = pas d'erreur
       {//setGeometry (QRect (val1.toInt(),val2.toInt(),val3.toInt(),val4.toInt()));
        move(val1.toInt(), val2.toInt());
        resize(val3.toInt(), val4.toInt());
        show();
       }
    else
       {QRect rect =  QApplication::desktop()->geometry();   // juste pour faire bouger les choses ajustement du bas)
        setGeometry ( rect );
        showMaximized () ;
       }
   if (G_pCApp->IsAppStartWithUser())
       {StartWithUser(G_pCApp->m_User, G_pCApp->m_SignUser);
        //if (CMoteurBase::IsThisDroitExist(G_pCApp->m_Droits, "agv")) m_pWdg_Agenda->setEnabled(TRUE);
        //else                                                         m_pWdg_Agenda->setEnabled(FALSE);
       }
   else
       {//..................... tant que pas identificaion tout inactiver ................................................................
        SetNoUserState();
       }
}
//----------------------------------- Slot_actionApropos -----------------------------------------------------------------------
void med_stat::Slot_actionApropos()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}
//--------------------------------- Slot_SauverLesMeubles --------------------------------------------------------------------------------
void med_stat::Slot_SauverLesMeubles()
{tryToStopAPropos();
}
//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void med_stat::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}
//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void med_stat::Slot_actionAproposDisplay()
{        //CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"SqlCreateTable/Changements.html",textEdit_Changements->text());
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).dirPath (true));
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).dirPath (true))+"Ressources/";
         QStringList argList;

         //......................... completer les autres arguments .........................................

         if (m_Apropos_Proc==0)
            {//m_action_A_Propos->setDisabled(TRUE);
             m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->addArgument( pathExeAPropos);                                               // 1  nom du module
             m_Apropos_Proc->addArgument("med_stat");                                                    // 1  nom du module
             m_Apropos_Proc->addArgument(tr("Module de gestion des statistiques"));                      // 2  description courte
             m_Apropos_Proc->addArgument(G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("=") + " Qt : "+QT_VERSION_STR);    // 3  numero de version
             m_Apropos_Proc->addArgument(G_pCApp->m_PathAppli+"Images/Changements.html");            // 4  fichiers decrivant les changements
             m_Apropos_Proc->addArgument(G_pCApp->m_PathAppli+"Images/med_stat.png");                // 5  Icone du programme
             m_Apropos_Proc->addArgument("");                                                            // 6  aide en ligne (vide pour prendre celle par defaut)
             m_Apropos_Proc->addArgument("");                                                            // 7  apropos (on met une chaine vide pour qu'il prenne celui par d�faut)
             m_Apropos_Proc->addArgument("");                                                            // 8  version de la base de donnee

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
//---------------------------------------------- StartWithUser -----------------------------------------------------------------------
void med_stat::StartWithUser(const QString &user, const QString &sign_user)
{ QListViewItem * pQListViewItem = m_pWdg_User->getUserItem(user, sign_user);
  if (pQListViewItem == 0) return;
  m_pWdg_User->SetUserItem(pQListViewItem);
}

//---------------------------------------------- SetNoUserState -----------------------------------------------------------------------
void med_stat::SetNoUserState()
{  m_CMDI_Identification->setEnabled(FALSE);
   m_pWdg_User->SetUnLogged();
}
//---------------------------------------------- DemarrerAccessoires -----------------------------------------------------------------------
void med_stat::DemarrerAccessoires()
{
  //.................. recuperer la liste des programmes Accessoires a demarrer en meme temps ................................................
  //                                              et les lancer
  // [Accessoires]
  // QLaboFTP = ../../QLaboFTP/bin/QLaboFTP
 QStringList accessoiresList;
 CGestIni::Param_GetList(G_pCApp->m_LocalParam, tr("Accessoires"), "", accessoiresList );
 if (accessoiresList.count())
    {for ( QStringList::Iterator it = accessoiresList.begin(); it != accessoiresList.end(); ++it )
         { DemarrerUnAccessoire(*it);
         }
    }
}
//---------------------------------------------- DemarrerUnAccessoire -----------------------------------------------------------------------
void med_stat::DemarrerUnAccessoire(QString accessoire_path)
{

 QString  pathExe = QDir::cleanDirPath (QFileInfo (qApp->argv()[0]).dirPath (true) + "/" + accessoire_path);
 #ifdef Q_WS_WIN
    pathExe +=".exe";
 #else
 #endif
 QProcess  proc ( this );
 proc.addArgument( pathExe );
 if ( proc.start() )
    {
    }
}
//---------------------------------------------- Slot_ComboUserTypClicked -----------------------------------------------------------------------
void med_stat::Slot_ComboUserTypClicked(const QString &)
{    QListViewItem *pQListViewItem = m_pWdg_User->getUserItem(G_pCApp->m_User, G_pCApp->m_SignUser);
     //................ si user en cours n'est plus dans la liste ....................
     //                 se mettre en unlogued
     if (! pQListViewItem )
        {SetNoUserState();
        }
     else
        {m_pWdg_User->SetUserItem(pQListViewItem);
         m_pWdg_User->SetLogged();
         m_CMDI_Identification->setEnabled(TRUE);
        }
}

//---------------------------------------------- Slot_EditUser -----------------------------------------------------------------------
void med_stat::Slot_EditUser()
{QProcess*  proc = new QProcess( this );
 if (proc==0) return;
 QString dumy;
 QString pathExe                  = CGestIni::Construct_Name_Exe("gest_user",QFileInfo (qApp->argv()[0]).dirPath (true));
 QListViewItem *oldpQListViewItem = m_pWdg_User->getSignUser();
 proc->addArgument( pathExe );
 proc->addArgument( G_pCApp->m_User);                           // Nom de l'utilisateur
 proc->addArgument( QString("#")+G_pCApp->m_CriptedPassWord );               // passWord
 if ( proc->start() )
    {
     qApp->processEvents ();
     while (proc->isRunning () )
           { QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     //.............. rebatir liste des utillisateurs au cas où .....................
     //               la gestion utilisateur  l'aurait modifiée
     QString   userQualite = m_pWdg_User->InitComboBoxUserTyp();
     if (userQualite==tr("Tout utilisateur"))userQualite="";     // si vide alors c'est tousles utilisateurs
     G_pCApp->m_pCMoteurBase->GetUserList( m_pWdg_User->ListView_UserList, dumy, dumy,TRUE, &userQualite);
     QListViewItem *pQListViewItem = m_pWdg_User->getUserItem(G_pCApp->m_User, G_pCApp->m_SignUser);
     //................ si user en cours n'est plus dans la liste (car detruit)....................
     //                 se mettre en unlogued
     if (! pQListViewItem )
        {SetNoUserState();
        }
     else
        {m_pWdg_User->SetUserItem(pQListViewItem);
        }
    G_pCApp->PositionneBonDroits();
    m_CMDI_Identification->m_pWdg_Central->SetInterfaceOnDroits(G_pCApp->m_Droits, 0);
    }
}
//---------------------------------------------- Slot_CPS_IsClicked -----------------------------------------------------------------------
void med_stat::Slot_CPS_IsClicked ()
{
#ifdef SESAMVITALE_VERSION
 QString          userPk = "";
 QString            user = "";
 QString            pass = "";
 Dlg_GetCodePorteur *dlg = new Dlg_GetCodePorteur(this);
 if (dlg == 0)             return;
 int ret                 = dlg->exec();
 QString            str  = dlg->lineEditGetCode->text();
 delete dlg;

 if (ret == QDialog::Accepted)
    {G_pCApp->m_pCps->SetCodePorteur(str);
     QString pathPlugin, pathLog;
     CGestIni::Param_ReadParam(G_pCApp->m_LocalParam, "Sesam-Vitale", "pathPlugin",       &pathPlugin);
     CGestIni::Param_ReadParam(G_pCApp->m_LocalParam, "Sesam-Vitale", "pathLog",          &pathLog);
     //..................................... ne pas rajouter "/" car c'est un nom de fichier ..........................
     if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanDirPath (pathPlugin.prepend(G_pCApp->m_PathAppli));
     //..................................... rajouter "/" car le chemin doit être donné avec ..........................
     if ( QDir::isRelativePath ( pathLog ) )     pathLog    = QDir::cleanDirPath (pathLog.prepend(G_pCApp->m_PathAppli) )+"/";

     QString pluginScript  = "[Execute]\r\n";
             pluginScript += "pathPlugin="      + pathPlugin                                           + "\r\n" +
                             "pathIni=****\r\n" +                //**** car indique d'utiliser le pathIni du plugin
                             "maskExch=Bonjour tout le monde\r\n"
                             "[Parametres]\r\n"
                             "m_Command       = READ_CPS\r\n"
                             "m_PathLog       =" + pathLog                                             + "\r\n" +
                             "m_CodePorteur   =" + G_pCApp->m_pCps->GetCodePorteur()                   + "\r\n" +
                             "m_DateConsult   =" + QDate::currentDate().toString("yyyyMMdd");

     if (pathPlugin.length())
        {G_pCApp->m_pCps->UnSerialize(PluginExe(this, pluginScript, CMoteurBase::endWait));
         userPk = G_pCApp->m_pCMoteurBase->GetUserPrimKeyFromNumOrdre(G_pCApp->m_pCps->m_NIR);
        }
     //.......................... si existe déjà le selectionner ..................................
     if (userPk.length())
        {QString       user = G_pCApp->m_pCMoteurBase->GetFieldValue(G_pCApp->m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                                     G_pCApp->m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                                     G_pCApp->m_pCMoteurBase->m_USER_IDENT_PRIMKEY,
                                                                     userPk);
         m_CMDI_Identification->SetUser(user,"");
         //m_pWdg_User->SetUser(user);
         G_pCApp->m_User = user;
         G_pCApp->m_pCMoteurBase->GetUserPrimKey( G_pCApp->m_User, &G_pCApp->m_CriptedPassWord );
         //.............. sauver dernier utilisateur .....................................
         CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Derniere Session", "Utilisateur", G_pCApp->m_User, G_pCApp->m_SignUser);
         CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli + "Manager.ini", G_pCApp->m_LocalParam);
        }
     //.......................... sinon le créer .........................................
     else if ( G_pCApp->m_pCps->m_NIR.length() && G_pCApp->m_pCps->m_Nom.length() && G_pCApp->m_pCps->m_Prenom.length())
        { ret = QMessageBox::information( this, tr("Utilisateur inconnu"),
                                             tr ( "L'utilisateur : <b>«" + G_pCApp->m_pCps->m_Nom + "  " + G_pCApp->m_pCps->m_Prenom +
                                                  "»</b> n'est pas connu de MedinTux, <br/>" +
                                                  "faut-il le créer ?"
                                                 ) ,
                                             tr("&Créer"), tr("&Annuler"), 0,
                                             1, 1 );
         if (ret==0)
            {   Dlg_NewUser *dlg = new Dlg_NewUser(this);
                if (dlg == 0)             return;
                user = G_pCApp->m_pCps->m_Nom;
                pass = "";
                dlg->initDialog(G_pCApp->m_pCMoteurBase, G_pCApp->m_pCps->Serialize(), &user, &pass);
                if (dlg->exec() == QDialog::Accepted)
                   {G_pCApp->m_pCMoteurBase->PassWordEncode(pass);
                    G_pCApp->m_pCMoteurBase->CreateNewUser(G_pCApp->m_pCps->m_Nom, G_pCApp->m_pCps->m_Prenom, user, "med", pass, G_pCApp->m_pCps->m_NIR);
                    m_pWdg_User->SetData();
                    m_CMDI_Identification->SetUser(user,"tochange");
                    //m_pWdg_User->SetUser(user);
                    G_pCApp->m_User = user;
                    G_pCApp->m_pCMoteurBase->GetUserPrimKey( G_pCApp->m_User, &G_pCApp->m_CriptedPassWord );
                    //.............. sauver dernier utilisateur ....................................
                    CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Derniere Session", "Utilisateur", G_pCApp->m_User, G_pCApp->m_SignUser);
                    CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli + "Manager.ini", G_pCApp->m_LocalParam);
                   }
                delete dlg;
            }
        }
   }
#endif
}


//------------------------------------------------------- PluginExe --------------------------------------------------
QString med_stat::PluginExe(     QObject         * pQObject,
                                const char      * pluginScript,
                                int               waitFlag /* = CMoteurBase::endWait */)
{
      QString     pathPlugin;  // Chemin de l'executable plugin à actionner sans l'extension .exe
      QString     pathIni;  // Chemin de l'executable plugin à actionner sans l'extension .exe
      QString     maskExch;    // texte du fichier d'exchange
      //QString     obsPk;
      //QString     terPk;
      //QString     ordPk;
      QStringList param;
      char             *pt   = (char*)(const char*) pluginScript;
      CGestIni::Param_ReadParam(pt,"Execute","pathPlugin",&pathPlugin);
      CGestIni::Param_ReadParam(pt,"Execute","pathIni",   &pathIni);
      CGestIni::Param_ReadParam(pt,"Execute","maskExch",  &maskExch);
      //.................................. PARAMETRES ...........................................
      QString     val1;
      QString     var_name   = "";
      QString      section   = "";
      while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
           {if (section=="Parametres")
               {while (*pt && *pt != '[')
                      {pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &val1);
                       if (val1.length()) param.append(val1);
                      }
               }
            section="";
           }

      return                      PluginExe(  pQObject,
                                              pathPlugin,                       // Chemin de l'executable plugin à actionner sans l'extension .exe
                                              G_pCApp->m_PathAppli,                  // Chemin de l'executable appelant (DrTux)
                                              pathIni,                          // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                              maskExch,                         // texte du fichier d'exchange
                                              G_pCApp->m_pCMoteurBase->GUID_Create()+"-New",
                                              " ",
                                              " ",
                                              " " ,
                                              param,
                                              CMoteurBase::endWait );
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
//                        tout autre délire en accord avec la syntaxe du plugin dès fois que ...
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
//   13 --> 13+Nb paramètres

QString med_stat::PluginExe(       QObject    *pQObject,
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
 //..................... creer le process ...........................................................
 QProcess*  proc =  new QProcess( pQObject );
 if (proc==0)       return QString(QObject::tr("Erreur:  PluginExe() the process can't be created"));

 //..................... copier le masque de retour dans le dossier temporaire ......................

 QString pathExe  = pathPlugin;
 if ( QDir::isRelativePath ( pathExe ) ) pathExe = QDir::cleanDirPath (pathExe.prepend(pathAppli) );
 QString nameExch = QFileInfo(pathExe).fileName()+"-"+ guid +".exc";

 QString      dst = "";
 if (CGestIni::Param_ReadParam(G_pCApp->m_LocalParam, "Repertoire Temporaire", "Repertoire", &dst)!=0)
     return QObject::tr("Error:  PluginExe()  \"Repertoire Temporaire\", \"Repertoire\" can't find in ini File");         // path editeur de texte non défini
 if ( QDir::isRelativePath ( dst ) ) dst = QDir::cleanDirPath (dst.prepend(G_pCApp->m_PathAppli) );
 dst +=  QDir::separator() + nameExch;
 QFile::remove (dst);

 //qDebug (QString("nameExch : ")  + dst);
 //qDebug (QString("pathAppli : ") + pathAppli);
 //qDebug (QString("pathExe : ") + pathExe);

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
     delete proc;
     return QObject::tr("Error:  PluginExe() destination file can't be opened");
    }
 qfdst.writeBlock (maskExch, maskExch.length());
 qfdst.close();
 /*
 #define BUFF_LEN 2048
 char *buf = new char[BUFF_LEN];
 long len;
 if (buf==0)
    {delete proc;
     return QObject::tr("Error:  PluginExe() bufer can't be created");
    }
 do{len = qfsrc.readBlock (buf, BUFF_LEN);
          qfdst.writeBlock (buf, len);
   }while (len==BUFF_LEN);
 delete [] buf;
 */
 //......................... completer les autres arguments .........................................
 proc->addArgument( pathExe + F_EXE );                   // 0  Chemin de l'executable plugin à actionner
 proc->addArgument( pathIni );                         // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
 proc->addArgument( pathAppli + PROG_NAME );           // 2  Chemin de l'executable appelant
 proc->addArgument( dst );                             // 3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour
 proc->addArgument(guid );                             // 4  GUID du patient
 proc->addArgument(dosPk);                             // 5  PrimKey de l'observation en cours
 proc->addArgument(terPk);                             // 6  PrimKey du terrain en cours
 proc->addArgument(ordPk);                             // 7  PrimKey de l'ordonnance en cours
 proc->addArgument(" ");                               // 8  reservé
 proc->addArgument(" ");                               // 9  reservé
 proc->addArgument(" ");                               // 10 reservé
 proc->addArgument(" ");                               // 11 reservé
 proc->addArgument(QString::number(param.count()));    // 12 Nb autres parametres d'appels specifiques au plugin
 for ( QStringList::Iterator it = param.begin(); it != param.end(); ++it )  // 13 --> 13+Nb paramètres
     {proc->addArgument(*it);
     }
 //................................. executer le process .............................................
 if ( m_PluginRun == "" && proc->start() )
    { m_PluginRun = pathExe;
     qApp->processEvents ();
     while (waitFlag==CMoteurBase::endWait && proc->isRunning () )
           { QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     m_PluginRun = "";
     //............lire le fichier d'echange ..........................
     if ( !QFile::exists( dst ) )     return  ret;
     QFile file( dst );
     if ( !file.open( IO_ReadOnly ) ) return  ret;
     QTextStream ts( &file );
     ret = ts.read();
    }
  delete proc;
  QFile::remove (dst);
  return ret;
}

//---------------------------------------------- SlotMakeVisible -----------------------------------------------------------------------
void med_stat::SlotMakeVisible(const QString&)
{showMaximized () ;
 //show () ;
}

//---------------------------------------------- setCaption -----------------------------------------------------------------------
void med_stat::setCaption(const QString &caption)
{if (caption.length())
    {QMainWindow::setCaption(caption);
     return;
    }
 QMainWindow::setCaption( tr("MedStat utilisé par : ") + G_pCApp->m_pCMoteurBase->m_UserName   + "@" + G_pCApp->m_pCMoteurBase->m_HostName + tr(" connecté à : ")  +
                                                         G_pCApp->m_pCMoteurBase->m_DriverName + "." + G_pCApp->m_pCMoteurBase->m_BaseName + tr(" Utilisateur : ") +
                                                         G_pCApp->m_User + "::"+G_pCApp->m_SignUser);
}

//---------------------------------------------- OnUserSelected -----------------------------------------------------------------------
 //pQListViewItem->text(4);    // GUID
 //pQListViewItem->text(1);    // Nom
 //pQListViewItem->text(2);    // Prenom
 //pQListViewItem->text(3);    // PrimKey
void med_stat::OnUserSelected( QListViewItem *pQListViewItem , QListViewItem *pQListViewItemParent)
{if (pQListViewItem==0) return;
 m_CMDI_Identification->setEnabled(TRUE);
 setCaption("");
 m_CMDI_Identification->m_pWdg_Central->SetInterfaceOnDroits(G_pCApp->m_Droits, 0);
}




//---------------------------------------------- OnOngletRubriquesChanged ------------------------------------------------------------
// ACTION: reponse au signal: SIGNAL( currentChanged ( QWidget * ) de la barre d'onglets des rubriques afin d'activer
//         la fenêtre CMDI correspondante.
// ENTREE: QWidget * pQwidget;  fenetre CMDI qui vient d'être activée.

void med_stat::OnOngletRubriquesChanged(QWidget * pCMDI_Qwidget)
{
 OnActiverRubrique(pCMDI_Qwidget->name());
}

//---------------------------------------------- OnActiverRubrique ------------------------------------------------------------
// ACTION: reponse au signal:  SIGNAL(Sign_ActiverRubrique(const char*))) provenant des Onglets des rubriques afin
//         d'activer la bonne fenetre CMDI lorsqu'un onglet a ete cliqué.
//         Cette fonction emet elle meme un signal à chaque fenetre CMDI qui doivent s'activer, si c'est leur onglet
//         qui est responsable du signal (reperable par le parametre: const char* rubName qui le texte de l'onglet.
// ENTREE: const char* rubName: texte (celui visible) de l'onglet cliqué.

void med_stat::OnActiverRubrique(const char* rubName)
{emit Sign_ActiverRubrique(rubName);   // emettre signal aux rubriques filles
}

//---------------------------------------------- OnDeleteRubrique ------------------------------------------------------------
// ACTION: reponse au signal: Sign_RubIsDeleted envoyé par le destructeur d'une rubrique.

void  med_stat::OnDeleteRubrique(const char* /*rubName*/)
{//m_pFormOngletRubrique->removePage(rubName);
}

//----------------------------------------------- closeEvent -------------------------------------------------------------
void med_stat::closeEvent( QCloseEvent* ce )
{
   int ret = QMessageBox::information( this, tr("Quitter MedinTux MedStat"),
                                        tr ( "Voulez-vous vraiment quitter MedinTux MedStat ?\n"
                                             "Ou simplement le réduire ?"
                                           ) ,
                                             tr("&Quitter"), tr("&Réduire"), tr("Annu&ler"),
                                             1, 1 );
       if      (ret== 1) {showMinimized();  ce->ignore(); return;}
       else if (ret==2)  {                  ce->ignore(); return;}
    ce->accept();
}

//---------------------------------------------- setupFileActions ------------------------------------------------------------
//#include "../../drtux/src/Pixmap/filesave.xpm"
//#include "../../drtux/src/Pixmap/fileopen.xpm"
//#include "../../drtux/src/Pixmap/fileprint.xpm"
//#include "../../drtux/src/Pixmap/fileprint2.xpm"

void med_stat::setupFileActions()
{
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Fichier" ), menu );
    //QToolBar *tb = new QToolBar( this );
    //tb->setLabel( tr("Outils Fichiers") );

    QAction *a;
    a = new QAction( tr( "Ouvrir une requête..." ),  Theme::getIcon("fileopen.png" ), tr( "&Ouvrir une requête..." ), CTRL + Key_O, this, "fileOpen" );
    connect( a, SIGNAL( activated() ), this, SLOT( Slot_fileOpen() ) );
    a->addTo( menu );
    a = new QAction( tr( "Enregistrer une requête..." ), Theme::getIcon("filesave.png" ), tr( "&Enregistrer une requête..." ), CTRL + Key_S, this, "fileSave" );
    connect( a, SIGNAL( activated() ), this, SLOT( Slot_fileSave() ) );
    a->addTo( menu );
    a = new QAction( tr( "Exécuter une requête..." ), Theme::getIcon("run.png" ), tr( "&Exécuter une requête..." ), CTRL + Key_R, this, "fileExe" );
    connect( a, SIGNAL( activated() ), this, SLOT( Slot_run() ) );
    a->addTo( menu );
    menu->insertSeparator();
    G_pCApp->m_OptionExportRun = new QAction( tr( "Exporter les résultats..." ), Theme::getIcon("filesaveExport.png" ), tr( "&Exporter les résultats..." ), CTRL + Key_E, this, "resultExport" );
    connect( G_pCApp->m_OptionExportRun, SIGNAL( activated() ), this, SLOT( Slot_export() ) );
    G_pCApp->m_OptionExportRun->addTo( menu );

    menu->insertSeparator();

    a = new QAction( tr( "Enregistrer les positions" ), Theme::getIcon("16X16/SaveProfile.png" ), tr( "&Enregistrer positions" ), 0, this, "Enregistrerpositions" );
    connect( a, SIGNAL( activated() ), this, SLOT( Slot_RecordPos() ) );
    a->addTo( menu );
    a = new QAction( tr( "A propos de med_stat" ), Theme::getIcon("APropos.png" ), tr( "&A propos de med_stat" ), 0, this, "Apropos" );
    connect( a, SIGNAL( activated() ), this, SLOT( Slot_actionApropos() ) );
    a->addTo( menu );
    a = new QAction( tr( "Quitter MedStat" ), Theme::getIcon("exit.png" ), tr( "&Quitter MedStat" ), 0, this, "fileClose" );
    connect( a, SIGNAL( activated() ), qApp, SLOT( quit() ) );
    a->addTo( menu );

    //moveDockWindow ( tb, Qt::DockRight );
}
//---------------------------------------------- Slot_fileOpen ------------------------------------------------------------
void med_stat::Slot_fileOpen()
{m_CMDI_Identification->m_pWdg_Central->pushButton_EPP_Charger_clicked();
}
//---------------------------------------------- Slot_fileSave ------------------------------------------------------------
void med_stat::Slot_fileSave()
{m_CMDI_Identification->m_pWdg_Central->pushButton_EPP_Enregistrer_clicked();
}
//---------------------------------------------- Slot_run ------------------------------------------------------------
void med_stat::Slot_run()
{m_CMDI_Identification->m_pWdg_Central->EPP_Do();
}
//---------------------------------------------- Slot_export ------------------------------------------------------------
void med_stat::Slot_export()
{m_CMDI_Identification->m_pWdg_Central->Slot_ConfigExport();
}

//---------------------------------------------- Slot_RecordPos ------------------------------------------------------------
void med_stat::Slot_RecordPos()
{

    int x  =  this->x();
    int y  =  this->y();
    int w  =  this->width();
    int h  =  this->height();


    CGestIni::Param_WriteParam(&G_pCApp->m_LocalParam, "ManagerPos", "Positions",
                                  QString::number(x),      QString::number(y),
                                  QString::number(w),      QString::number(h));
    CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli + "med_stat.ini", G_pCApp->m_LocalParam);
}

//==================================================================== CMDI_Identification ===================================================================================
//---------------------------------------------- SetUser -----------------------------------------------------------------------
void CMDI_Identification::SetUser(const QString &user, const QString &signUser)
    {m_pWdg_Central->setUser( user, signUser);
    }

//---------------------------------------------- CMDI_Identification -----------------------------------------------------------------------
 CMDI_Identification::CMDI_Identification( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase)
                        :CMDI_Generic(parent, name,  wflags, pCMoteurBase)
    {QVBox *pQVCentral = new QVBox( this );
     pQVCentral->setMargin( 0 );
     pQVCentral->setSpacing(0);
     pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
     m_pWdg_Central = new Wdg_Central(pQVCentral, "Wdg_Central");
     setCentralWidget( pQVCentral );
     setIcon( G_pCApp->m_PathAppli+"Images"+QDir::separator()+"malehead.png" );
    }

