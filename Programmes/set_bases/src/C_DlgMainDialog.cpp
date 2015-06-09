/***************************************************************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
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


#include "C_DlgMainDialog.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qeventloop.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qdir.h>

#include "CApp.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CW_EditText.h"
#include "../../MedinTuxTools/C_Huffman.h"
#include "../../MedinTuxTools/C_Zlib/C_Zlib.h"
#include "../../MedinTuxTools/CGestIni.h"
#include <qdir.h>
#include <qfileinfo.h>
#include <qcolor.h>

#define INSTALL_BASE_GET               FALSE
#define INSTALL_BASE_DATASEMP          TRUE
#define INSTALL_VIDAL_RUN              TRUE
#define INSTALL_VIDAL_NOTRUN           FALSE
#define MAKE_EXE_RUN                   1
#define MAKE_EXE_NOTRUN                0

#ifdef Q_WS_X11
  #define F_EXE
#endif

#ifdef Q_WS_WIN
 #define F_EXE ".exe"
#endif

#ifdef  Q_WS_MAC
  #define F_EXE
#endif

#define MAGIC_STR "leslingesquisechentmouillentlescordes"

C_DlgMainDialog::C_DlgMainDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
    : Dlg_MainDialog(parent,name, modal,fl)
{
    m_Apropos_Proc = 0;
    QString   ret  = "";
    QString   path = "";
    m_SousAppleSeMettreEnModeAdmin = "<u><font color=\"#d6001e\"><b>Sous Apple se mettre en mode Adminitrateur</b></font></u>";
    m_SousLinuxSeMettreEnModeAdmin = "<u><font color=\"#d6001e\"><b>Sous Linux se mettre en mode Adminitrateur</b></font></u>";
    connect( quitPushButton,               SIGNAL( clicked() ),         this, SLOT( Slot_close() ) );
    connect( pushButton_SetBases,          SIGNAL( clicked() ),         this, SLOT( pushButton_SetBases_clicked() ) );
    connect( pushButton_LancerDrTux,       SIGNAL( clicked() ),         this, SLOT( pushButton_LancerDrTux_clicked() ) );
    connect( pushButton_SetDatasemp,       SIGNAL( clicked() ),         this, SLOT( pushButton_SetDatasemp_clicked() ) );
    connect( pushButtonJustOneTable,       SIGNAL( clicked() ),         this, SLOT( pushButtonJustOneTable_clicked() ) );
    connect( pushButton_Load_SQL,          SIGNAL( clicked() ),         this, SLOT( pushButton_Load_SQL_clicked() ) );
    connect( checkBox_STD_MAJ,             SIGNAL( stateChanged(int) ), this, SLOT( checkBox_STD_MAJ_stateChanged(int) ) );
    connect( checkBox_SQL_MAJ,             SIGNAL( stateChanged(int) ), this, SLOT( checkBox_SQL_MAJ_stateChanged(int) ) );
    connect( pushButtonSwitchGetDatasemp,  SIGNAL( clicked() ),         this, SLOT( pushButtonSwitchGetDatasemp_clicked() ) );
    connect( pushButtonConnexionAppl,      SIGNAL( clicked() ),         this, SLOT( pushButtonConnexionAppl_clicked() ) );
    connect( pushButton_compilation,       SIGNAL( clicked() ),         this, SLOT( pushButton_compilation_clicked() ) );
    connect( pushButton_Qt3,               SIGNAL( clicked() ),         this, SLOT( pushButton_Qt3_clicked() ) );
    connect( pushButton_Qt4Sdk,            SIGNAL( clicked() ),         this, SLOT( pushButton_Qt4Sdk_clicked() ) );
    connect( pushButton_MajBases,          SIGNAL( clicked() ),         this, SLOT( pushButton_MajBases_clicked() ) );
    connect( pushButton_APropos,           SIGNAL( clicked() ),         this, SLOT( Slot_actionApropos() ) );

    connect(G_pCApp,                       SIGNAL(Sign_QuitterRequired()), this, SLOT(Slot_SauverLesMeubles()));

    connect( comboBox_IP_Fav,              SIGNAL( activated(const QString&) ),   this, SLOT( comboBox_IP_Fav_activated(const QString&) ) );
    connect( comboBox_IP_Fav,              SIGNAL( highlighted(const QString&) ), this, SLOT( comboBox_IP_Fav_highlighted(const QString&) ) );
    connect( pushButton_FavPlus,           SIGNAL( clicked() ),                   this, SLOT( pushButton_FavPlus_clicked() ) );
    connect( pushButtonRemove,             SIGNAL( clicked() ),                   this, SLOT( pushButtonRemove_clicked() ) );

    connect( comboBox_IP_Fav_2,            SIGNAL( activated(const QString&) ),   this, SLOT( comboBox_IP_Fav_2_activated(const QString&) ) );
    connect( comboBox_IP_Fav_2,            SIGNAL( highlighted(const QString&) ), this, SLOT( comboBox_IP_Fav_2_highlighted(const QString&) ) );
    connect( pushButton_FavPlus_2,         SIGNAL( clicked() ),                   this, SLOT( pushButton_FavPlus_2_clicked() ) );
    connect( pushButtonRemove_2,           SIGNAL( clicked() ),                   this, SLOT( pushButtonRemove_2_clicked() ) );

    connect( comboBox_IP_Fav_3,            SIGNAL( activated(const QString&) ),   this, SLOT( comboBox_IP_Fav_3_activated(const QString&) ) );
    connect( comboBox_IP_Fav_3,            SIGNAL( highlighted(const QString&) ), this, SLOT( comboBox_IP_Fav_3_highlighted(const QString&) ) );
    connect( pushButton_FavPlus_3,         SIGNAL( clicked() ),                   this, SLOT( pushButton_FavPlus_3_clicked() ) );
    connect( pushButtonRemove_3,           SIGNAL( clicked() ),                   this, SLOT( pushButtonRemove_3_clicked() ) );

    connect( checkBox_Nomadisme,           SIGNAL( stateChanged(int ) ),          this, SLOT( checkBox_Nomadisme_stateChanged(int )) );

    connect( pushButtonImagesCopy,         SIGNAL( clicked() ), this, SLOT( pushButtonImagesCopy_clicked() ) );
    connect( pushButtonMakeSQL,            SIGNAL( clicked() ), this, SLOT( pushButtonMakeSQL_clicked() ) );
    connect( pushButton_LireVitale,        SIGNAL( clicked() ), this, SLOT( pushButton_LireVitale_clicked() ) );
    connect( pushButton_InstallSesam,      SIGNAL( clicked() ), this, SLOT( pushButton_InstallSesam_clicked()) );
    connect( pushButton_IndexerDatasemp,   SIGNAL( clicked() ), this, SLOT( pushButton_IndexerDatasemp_clicked()) );
    connect( pushButton_setGalsINI,        SIGNAL( clicked() ), this, SLOT( Slot_pushButton_setGalsINI_clicked()) );

    pushButton_APropos->setPixmap(Theme::getIcon( "22x22/help.png"));
    textLabel_AdminLinux->hide();
    pushButton_setGalsINI->hide();

 progressBar_Load->setIndicatorFollowsStyle(TRUE);
 //.......................... fenetre de log ................................
 textEdit_Message->mimeSourceFactory()->addFilePath(G_pCApp->m_PathAppli +"SqlCreateTable" );
 textEdit_Message->setBackgroundMode(Qt::FixedColor);
 textEdit_Message->setPaletteBackgroundColor (QColor("black") );
 textEdit_Message->setWordWrap ( QTextEdit::NoWrap );
 textEdit_Message->setColor (QColor("#00e413") );
    m_compil_In          = 0;
    m_proc               = 0;
    m_pCUpdateThread     = 0;
    m_pCIndexMakeThread  = 0;
    m_pQTimer            = 0;
    m_pCindexQTimer      = 0;
    m_ProgressTotal      = 0;
    m_DatasempDir        = "";
    m_DatasempBase       = 0;
    m_pCMedicaBase       = 0;
    m_pCMoteurBase       = 0;
    m_TestBasesProc      = 0;
    m_PluginRun          = "";
    m_CurrentCompil      = "";
    m_testBaseProcRun    = FALSE;
    m_BaseToInstallFlag  = INSTALL_BASE_GET;
    m_BaseVidalRun       = INSTALL_VIDAL_NOTRUN;
    m_MakeExeRun         = MAKE_EXE_NOTRUN;
    m_pTabQWidgetMaster          = tabWidget_Nomadisme->page ( 0 );
    m_pTabQWidgetNomade          = tabWidget_Nomadisme->page ( 1 );
    m_pTabQWidgetReverse         = tabWidget_Nomadisme->page ( 2 );

    m_pTabQWidgetBases           = tabWidget_Instal->page( 0 );
    m_pTabQWidgetTherap          = tabWidget_Instal->page( 1 );
    m_pTabQWidgetCompil          = tabWidget_Instal->page( 2 );
    m_pTabQWidgetSesam           = tabWidget_Instal->page( 3 );

    int mode             = GetDatasempMode();
    m_MessageNoConnect   = tr("\n================= CONNEXION INTERROMPUE==================\n"
                              "\n Veuillez choisir de nouveaux paramètres de connexion au serveur "
                              "\n de données MySQL soit : le nom de connexion, le mot de passe, "
                              "\n et l'adresse IP ou le nom de machine sur laquelle se trouve"
                              "\n le serveur MySQL  (localhost si c'est la machine sur laquelle"
                              "\n vous êtes actuellement)\n"
#ifdef  Q_WS_X11
                              "\n Vérifiez que le paquet : libqt3-mysql est bien installé sur votre système."
                              "\n Si il est installé le fichier suivant : /usr/lib/qt3/plugins/lib/sqldrivers/libqsqlmysql.so"
                              "\n devrait être présent\n"
#endif
                              "\n Puis appuyez sur le bouton 'Se connecter à la base de données''");


    if ( G_pCApp->m_justInstallBase )
       {//textLabel_MedinTux->setPixmap ( Theme::getIcon( "MedinTuxInstallBases.png") );
        //textLabel_MedinTux->setScaledContents ( TRUE );
       }
    else
       {textLabel_MedinTux->setPixmap ( Theme::getIcon( "medintux_logo.png") );
       }

    if (mode&(CApp::Remove|CApp::Insert|CApp::Update))    checkBox_STD_MAJ->setChecked(TRUE);
    else                                                  checkBox_STD_MAJ->setChecked(FALSE);
    if (mode&CApp::SqlFiles)                              checkBox_SQL_MAJ->setChecked(TRUE);
    else                                                  checkBox_SQL_MAJ->setChecked(FALSE);
    G_pCApp->GotoDebug();
    //..................... recuperer les paramètres de connexion ....................................
    QString  driverName , baseName , userName , passWord, hostName, port;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "DrTuxTest", "Nomade",
                                  &driverName, &baseName, &userName, &passWord, &hostName, &port) !=0
        )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "DrTuxTest";
        userName    = "root";
        passWord    = "";
        hostName    = "localhost";
        port        = "3306";
    }
    lineEdit_UserName_2->setText(userName);
    lineEdit_PassWord_2->setText(passWord);
    lineEdit_Hostname_2->setText(hostName);
    lineEdit_Sql_Port_2->setText(port);

    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "DrTuxTest", "Master",
                                  &driverName, &baseName, &userName, &passWord, &hostName, &port) !=0
        )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "DrTuxTest";
        userName    = "root";
        passWord    = "";
        hostName    = "localhost";
        port        = "3306";
    }
    lineEdit_UserName->setText(userName);
    lineEdit_PassWord->setText(passWord);
    lineEdit_Hostname->setText(hostName);
    lineEdit_Sql_Port->setText(port);

    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "DrTuxTest", "Reverse",
                                  &driverName, &baseName, &userName, &passWord, &hostName, &port) !=0
        )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "DrTuxTest";
        userName    = "root";
        passWord    = "";
        hostName    = "localhost";
        port        = "3306";
    }
    lineEdit_UserName_3->setText(userName);
    lineEdit_PassWord_3->setText(passWord);
    lineEdit_Hostname_3->setText(hostName);
    lineEdit_Sql_Port_3->setText(port);

    //...................... lire parametrage base Get ou Datasemp .................................

    path = CGestIni::Construct_Name_File_Ini("drtux", QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.ini");
    // Affichage dans la Box
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================= Lecture Base Get ou Datasemp %1 =================").arg(path) );

    if (  ! QFile::exists (path) )
    {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n le fichier d'initialisation : %1 n'existe pas").arg(path));
        //return ;
    }

    QString param, parametreBase;
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam(param, "Connexion" , "InUseBase" ,  &parametreBase);
    if ( parametreBase == "Parametres MedicaBase")
    {   pushButtonSwitchGetDatasemp->setText(tr("Activer la &base Get"));
        m_BaseToInstallFlag  = INSTALL_BASE_GET;
    }
    else
    {   pushButtonSwitchGetDatasemp->setText(tr("Activer la &base Datasemp"));
        m_BaseToInstallFlag  = INSTALL_BASE_DATASEMP;
    }
    reinitLog();
    //................. ça devient le foutoir ce programme ...................
    //                  vivement la reecriture en QT5
    if ( G_pCApp->m_justCompilation )
       { toLog(tr(" Veuillez choisir le programme à compiler dans la liste "
             "\n située à droite du bouton 'Compilation des exécutables' ou, "
             "\n choisissez dans cette liste l'option 'Tous les programmes' "
             "\n afin de recompiler tous les programmes de MedinTux."
             "\n Puis appuyez sur le bouton 'Compilation des exécutables'"));
         toLog( tr("<b>================================================================</b>"));
       }
    else
       { toLog(tr(" Veuillez choisir les paramètres de connexion au serveur de"
             "\n données MySQL soit : le nom de connexion, le mot de passe, "
             "\n et l'adresse IP ou le nom de machine sur laquelle se trouve"
             "\n le serveur MySQL  (localhost si c'est la machine sur laquelle"
             "\n vous êtes actuellement)\n"
             "\n Puis appuyez sur le bouton 'Se connecter à la base de données'"));
         toLog( tr("<b>================================================================</b>"));
       }
    //..................... remplir combo des executables ...............................................
    CGestIni::Param_GetList(G_pCApp->m_ParamData, "Compilables", "",  m_compilList , 1);
    comboBox_Executables->clear();

#ifdef Q_WS_WIN
    QStringList::Iterator it = m_compilList.find("guinch");
    if (it != m_compilList.end())
       {m_compilList.remove(it);
       }
#endif

    comboBox_Executables->insertStringList(m_compilList);
    comboBox_Executables->insertItem( tr("Tous les programmes"),0);
    //..................... remplir combo des bases .....................................................
    comboBox_Bases->insertItem( tr("Toutes les bases"));
    comboBox_Bases->insertStringList (G_pCApp->GetBasesListToSet());
    pushButtonMakeSQL->hide();  // pas de bash sous W
    textLabel_MacOS->hide();
	
#ifdef Q_WS_WIN
    textLabel_RootField->hide();
    lineEdit_RootPass->hide();
    comboBox_PortLecteur->hide();
    lineEdit_SesamPath->setText("C:\\");
    lineEdit_BinaryPath->setText("C:\\WINDOWS");
    lineEdit_Qt3->setText(tryToFindQt3());
    lineEdit_Qt4Sdk->setText(tryToFindSdkQt4());
    textLabel_WindPathField->setText(tr("Répertoire WINDOWS "));
#endif

#ifdef Q_WS_X11
    if (QFile::exists(QDir::homeDirPath ()+"/MedinTuxRo.txt"))  pushButtonMakeSQL->show();
    textLabel_WindPathField->setText(tr("Répertoire système Sesam  "));
    lineEdit_SesamPath->hide();
    textLabel_SesamPath->hide();
    comboBox_PortLecteurWin->hide();
    lineEdit_BinaryPath->setText("/usr/local");
    QStringList lst;
    CGestIni::Param_GetList(G_pCApp->m_ParamData, "io_comm", "",  lst );
    for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
        {(*it) = (*it).replace("@","=");
        }
    if (lst.count())
       {comboBox_PortLecteur->clear();
        comboBox_PortLecteur->insertStringList ( lst );
       }
    lineEdit_Qt4Sdk->setText(tryToFindSdkQt4());
    lineEdit_Qt3->setText(tryToFindQt3());
    textLabel_AdminLinux->setText(m_SousLinuxSeMettreEnModeAdmin);
    textLabel_AdminLinux->show();
#endif
                         //md/pr/sb/bn/
#ifdef Q_OS_MACX        //../../../../.MedinTuxLogoFondBleu.png
    if (QFile::exists(QDir::homeDirPath ()+"/MedinTuxRo.txt")) pushButtonMakeSQL->show();
    textLabel_WindPathField->setText(tr("Répertoire système Sesam  "));
    lineEdit_SesamPath->hide();
    textLabel_SesamPath->hide();
    comboBox_PortLecteurWin->hide();
    lineEdit_BinaryPath->setText("/usr/local");
    textLabel_AdminLinux->setText(m_SousAppleSeMettreEnModeAdmin);
    textLabel_AdminLinux->show();
    lineEdit_Qt4Sdk->setText(tryToFindSdkQt4());
    lineEdit_Qt3->setText(tryToFindQt3());
    textLabel_MacOS->setText(tr("<u><font color=\"#d6001e\"><b>Sous Apple se mettre en mode Adminitrateur</b></font></u> (et lancer set_bases en mode console avec le script : <b>set_basesMac.sh</b>"));
    textLabel_MacOS->show();
#endif
    //............................. titre du group box de compilation .....................................
    QString version;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "CONFIG", "Version", &version) !=0 )  // zero = pas d'erreur
    {version = "0.09";
    }
    QString title = tr("Installation de la suite logicielle : MedinTux © 2005-2011 DATA MEDICAL DESIGN Version Libre");
    if ( IsSesamVersionExist()  )
    {   title = tr("Installation de la suite logicielle : MedinTux © 2005-2011 DATA MEDICAL DESIGN  Version Sesam Vitale");
        comboBox_Executables->insertItem ( "QFseVitale" );
        tabWidget_Instal->setTabEnabled ( m_pTabQWidgetSesam, TRUE );
    }
    else
    {   tabWidget_Instal->setTabEnabled ( m_pTabQWidgetSesam, TRUE );
    }
    // Vérification existence du repertoir module tmp
    path = CGestIni::Construct_PathBin_Module("set_bases", QFileInfo (qApp->argv()[0]).dirPath (true)) + "../../tmp";
    QDir dir(path);
    if (!dir.exists()) dir.mkdir (path);

    //groupBoxMedinTuxInstall->setTitle( title );
    if ( !G_pCApp->m_justCompilation )
       { ret = GetMAJVersion();
         if (ret[2] == '.') SetTitle(ret);
         else { G_pCApp->Datasemp_OutMessage(textEdit_Message, QString (tr("============ Erreur  : GetMAJVersion()==============\n")) + ret);
                SetTitle("?,??");
              }
    
         fillFavoris(comboBox_IP_Fav);
         fillFavoris(comboBox_IP_Fav_2);
         fillFavoris(comboBox_IP_Fav_3);
         comboBox_IP_Fav_insertItem("QMYSQL3,root,,localhost,3306", comboBox_IP_Fav);   // faut s'assurer qu'il y est
         comboBox_IP_Fav_insertItem("QMYSQL3,root,,localhost,3306", comboBox_IP_Fav_2); // faut s'assurer qu'il y est
         comboBox_IP_Fav_insertItem("QMYSQL3,root,,localhost,3306", comboBox_IP_Fav_3); // faut s'assurer qu'il y est
       }
    tabWidget_Instal->setTabEnabled ( m_pTabQWidgetBases,  FALSE );   
    tabWidget_Instal->setTabEnabled ( m_pTabQWidgetTherap, FALSE );

    pushButton_SetBases->hide();
    pushButton_MajBases->hide();
    comboBox_Bases->hide();

    //...............   cacher le panneau nomadisme ...................................
    tabWidget_Nomadisme->removePage(m_pTabQWidgetNomade);
    tabWidget_Nomadisme->removePage(m_pTabQWidgetReverse);
    setIcon( Theme::getIcon( "32x32/set_bases.png") );
     if ( G_pCApp->m_justCompilation )
       {      
         groupBox3->hide();  
         pushButton_SetBases->hide();         
         pushButton_LancerDrTux->hide(); 
         tabWidget_Instal->setTabEnabled ( m_pTabQWidgetSesam, false );
         tabWidget_Instal->setTabEnabled ( m_pTabQWidgetCompil,  true );
         tabWidget_Instal->setCurrentPage(tabWidget_Instal->indexOf(m_pTabQWidgetCompil));
         QRect rect=this->geometry();
         rect.setWidth ( Theme::getIcon( "MedinTuxInstallBases.png").width() );
         this->setGeometry(rect);
         QTimer::singleShot( 100, this, SLOT(Slot_TimedCompilation()) );
       }  
    if ( G_pCApp->m_justInstallBase )
       {        
         groupBox3->hide();  
         tabWidget_Instal->hide(); 
         pushButton_SetBases->show();         
         pushButton_LancerDrTux->hide(); 
         tabWidget_Instal->setTabEnabled ( m_pTabQWidgetBases,  TRUE );
         tabWidget_Instal->setCurrentPage(tabWidget_Instal->indexOf(m_pTabQWidgetBases));
         textLabel_MedinTux->setPixmap ( Theme::getIcon( "MedinTuxInstallBases.png") );
         textLabel_MedinTux->setScaledContents ( TRUE );
         QRect rect=this->geometry();
         rect.setWidth ( Theme::getIcon( "MedinTuxInstallBases.png").width() );
         this->setGeometry(rect);
         QTimer::singleShot( 100, this, SLOT(Slot_TimedSetBase()) );
       }  
 putInFront();

}

C_DlgMainDialog::~C_DlgMainDialog()
{
}

/*$SPECIALIZATION$*/
//----------------------------------------- putInFront --------------------------------------------------------------------
void C_DlgMainDialog::putInFront()
{   this->setWindowState(windowState() & WindowActive);
    this->raise();
}
//----------------------------------------- Slot_TimedCompilation --------------------------------------------------------------------
void C_DlgMainDialog::Slot_TimedCompilation()
{  QStringList rejectedList = QStringList::split ( "|", G_pCApp->m_rejectedStr, false);
   QStringList unCompiledList;
   IsExecutablesExist(unCompiledList);
   //............ pour tous les modules non trouves ....................
   //             verifier si ils ne font pas partie de la liste
   //             de ceux a exclure (fournie sur la ligne de commande)
   //             COMPILATION(param_view|qfselib|makedistrib)
   //
   int isCompilationRequired  = 0;
   for (int i=0; i< (int)unCompiledList.count(); ++i)
       { int isInNotToCompile = 0;
         //..... si celui non trouve est present ................
         //      dans la liste de ceux a ne pas compiler
         for (int u=0; u<(int)rejectedList.count(); ++u)
             { if (unCompiledList[i] == rejectedList[u])  {isInNotToCompile = 1; break;}  // Ok il n'est pas a compiler
             }
         //........... si il n'a pas ete trouve dans la liste des exclus ..............
         //            un seul des modules non compile, il est a compiler.
         if (isInNotToCompile==0)  {isCompilationRequired = 1; break;}   
       }
   if ( isCompilationRequired == 0) QDialog::close();
}
//----------------------------------------- Slot_TimedSetBase --------------------------------------------------------------------
void C_DlgMainDialog::Slot_TimedSetBase()
{  setBase();
   QDialog::close();
   //Slot_close();
}

//--------------------------------- Slot_close --------------------------------------------------------------------------------
void C_DlgMainDialog::Slot_close()
{Slot_SauverLesMeubles();
 QDialog::close();
}

//--------------------------------- Slot_SauverLesMeubles --------------------------------------------------------------------------------
void C_DlgMainDialog::Slot_SauverLesMeubles()
{tryToStopAPropos();
}

//----------------------------------- Slot_actionApropos -----------------------------------------------------------------------
void C_DlgMainDialog::Slot_actionApropos()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}
//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void C_DlgMainDialog::Slot_actionAproposDisplay()
{        //CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"SqlCreateTable/Changements.html",textEdit_Changements->text());
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).dirPath (true));
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).dirPath (true))+"Ressources/";
         QStringList argList;
         QString dataBaseVersion    = GetMAJVersion();
         if (dataBaseVersion[2] != '.') dataBaseVersion = tr("not retrieved");
         //......................... completer les autres arguments .........................................

         if (m_Apropos_Proc==0)
            {//m_action_A_Propos->setDisabled(TRUE);
             m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->addArgument( pathExeAPropos);                                               // 1  nom du module
             m_Apropos_Proc->addArgument("set_bases");                                                   // 1  nom du module
             m_Apropos_Proc->addArgument("Module de gestion des bases");                                 // 2  description courte
             m_Apropos_Proc->addArgument(G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("=") + " Qt : "+QT_VERSION_STR);    // 3  numero de version
             m_Apropos_Proc->addArgument(G_pCApp->m_PathAppli+"SqlCreateTable/Changements.html");        // 4  fichiers decrivant les changements
             m_Apropos_Proc->addArgument(Theme::getPath(Theme::WithSeparator)+"32x32/set_bases.png");    // 5  Icone du programme
             m_Apropos_Proc->addArgument("");                                                            // 6  aide en ligne (vide pour prendre celle par defaut)
             m_Apropos_Proc->addArgument("");                                                            // 7  apropos (on met une chaine vide pour qu'il prenne celui par défaut)
             m_Apropos_Proc->addArgument(dataBaseVersion);                                               // 8  version de la base de donnee

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
void C_DlgMainDialog::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}

//----------------------------------------------- reinitLog ---------------------------------------
void  C_DlgMainDialog::reinitLog(int mode)
{textEdit_Message->setText("");
 if (mode==0)        toLog(CW_EditText::fillWith ("<b>", "&nbsp;",0 )  + "<img source=\"./logo.png\">");
 else if (mode==1)   toLog(CW_EditText::fillWith ("<b>", "&nbsp;",0 )  + "<img source=\"./logoOk.png\">");
}
//----------------------------------------------- toLog ---------------------------------------------------------------------
void  C_DlgMainDialog::toLog(  const QString &mess)
{
    textEdit_Message->append( mess );
    textEdit_Message->scrollToBottom ();
    //logWidget->moveCursor(QTextEdit::MoveEnd);
    //logWidget->ensureCursorVisible ();
    //hasPendingEvents ();
    //qApp->flush();
    //qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
}

//----------------------------------------- saveFavoris ---------------------------------------------
void C_DlgMainDialog::checkBox_Nomadisme_stateChanged(int /* state */ )
{if (checkBox_Nomadisme->isChecked ())
    {tabWidget_Nomadisme->insertTab   ( m_pTabQWidgetNomade ,  "Serveur nomade");
     tabWidget_Nomadisme->insertTab   ( m_pTabQWidgetReverse , "Serveur reverse");

    }
 else
    {tabWidget_Nomadisme->removePage  ( m_pTabQWidgetNomade  );
     tabWidget_Nomadisme->removePage  ( m_pTabQWidgetReverse );

    }
}

//----------------------------------------- saveFavoris ---------------------------------------------
void C_DlgMainDialog::SetTitle(QString n_Version)
{QString numVers = G_pCApp->m_NUM_VERSION;
         numVers = numVers.remove("==").remove("@@").remove("##");
 setCaption( tr("DATA MEDICAL DESIGN (Configuration de MedinTux) V : %1").arg(numVers) + " BD : " + n_Version);
}

//----------------------------------------- saveFavoris ---------------------------------------------
void C_DlgMainDialog::saveFavoris(QComboBox *pQComboBox)
{
   QString                              sectionToMatch  = "Favoris";
   if (pQComboBox == comboBox_IP_Fav_2) sectionToMatch  = "Favoris_2";
   if (pQComboBox == comboBox_IP_Fav_3) sectionToMatch  = "Favoris_3";

    //
    QString        val1;
    QString      section   = "";
    QString     var_name   = "";
    const char      *txt   = G_pCApp->m_ParamData;
    char             *pt   = (char*) txt;
    // char     *prop_section = 0;       // permet de noter le pointeur sur la section propriete renouvelable/intercurent des produits

    //.................................. virer la section des variables ........................................................;
    while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {
        if (section==sectionToMatch)
        {char* pt_deb_var = pt;          //pointe à la ligne du mot '[Favoris]' donc au debut des variables
            while (*pt && *pt != '[')
            {pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &val1);
            }
            G_pCApp->m_ParamData.remove(pt_deb_var - txt, pt - pt_deb_var);
            break;
        }
    }
    //.................................. remplir la section des variables ........................................................;
    int nb =  comboBox_IP_Fav->count();
    for (int i=0; i<nb; ++i)
    {// pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &driver, &user, &pass, &hostname);
        CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  sectionToMatch, QString::number(i) , comboBox_IP_Fav->text(i));
    }
    CGestIni::Param_UpdateToDisk(G_pCApp->m_ParamPath, G_pCApp->m_ParamData);   // ecrire et mettre à jour le fichier sur le disque
}

//----------------------------------------- fillFavoris ---------------------------------------------
void C_DlgMainDialog::fillFavoris(QComboBox *pQComboBox)
{
   QString                               sectionToMatch = "Favoris";
   if (pQComboBox == comboBox_IP_Fav_2)  sectionToMatch = "Favoris_2";
   if (pQComboBox == comboBox_IP_Fav_3)  sectionToMatch = "Favoris_3";

    // 1 = QMYSQL3,hp_ro, ,192.168.2.10
    QString        driver, user, pass,hostname, port;
    QString      section   = "";
    QString     var_name   = "";
    const char      *txt   = G_pCApp->m_ParamData;
    char             *pt   = (char*) txt;
    // char     *prop_section = 0;       // permet de noter le pointeur sur la section propriete renouvelable/intercurent des produits
    while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {//.................................. ATCD ........................................................;
        if (section==sectionToMatch)
        {   pQComboBox->clear();
            while (*pt && *pt != '[')
            {   pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &driver, &user, &pass, &hostname, &port);
                pass = pass.stripWhiteSpace();
                if (pass.length() && pass[0]!='@') pass = QString("@")+PassWordEncode(pass);
                comboBox_IP_Fav_insertItem(driver.stripWhiteSpace()     +","+      // 0
                                           user.stripWhiteSpace()       +","+      // 1
                                           pass                         +","+      // 2
                                           hostname.stripWhiteSpace()   +","+      // 3
                                           port.stripWhiteSpace(), pQComboBox);    // 4

            }
            break;
        }
    }
}

//----------------------------------------- comboBox_IP_Fav_activated ---------------------------------------------
void C_DlgMainDialog::comboBox_IP_Fav_activated( const QString &text )
{   QStringList list = QStringList::split ( ",", text, TRUE);
    lineEdit_Hostname->setText( list[3] );
    lineEdit_UserName->setText( list[1] );
    QString pass = list[2];
    if (pass[0]=='@')
       {pass = pass.remove("@");
        pass = PassWordDecode(pass);
       }
    lineEdit_PassWord->setText( pass );
    lineEdit_Sql_Port->setText( list[4] );
}
//----------------------------------------- comboBox_IP_Fav_2_activated ---------------------------------------------
void C_DlgMainDialog::comboBox_IP_Fav_2_activated( const QString &text )
{   QStringList list = QStringList::split ( ",", text, TRUE);
    lineEdit_Hostname_2->setText( list[3] );
    lineEdit_UserName_2->setText( list[1] );
    QString pass = list[2];
    if (pass[0]=='@')
       {pass = pass.remove("@");
        pass = PassWordDecode(pass);
       }
    lineEdit_PassWord_2->setText( pass );
    lineEdit_Sql_Port_2->setText( list[4] );
}
//----------------------------------------- comboBox_IP_Fav_3_activated ---------------------------------------------
void C_DlgMainDialog::comboBox_IP_Fav_3_activated( const QString &text )
{   QStringList list = QStringList::split ( ",", text, TRUE);
    lineEdit_Hostname_3->setText( list[3] );
    lineEdit_UserName_3->setText( list[1] );
    QString pass = list[2];
    if (pass[0]=='@')
       {pass = pass.remove("@");
        pass = PassWordDecode(pass);
       }
    lineEdit_PassWord_3->setText( pass );
    lineEdit_Sql_Port_3->setText( list[4] );
}

//----------------------------------------- comboBox_IP_Fav_highlighted ---------------------------------------------
void C_DlgMainDialog::comboBox_IP_Fav_highlighted( const QString &text )
{comboBox_IP_Fav_activated( text );
}
//----------------------------------------- comboBox_IP_Fav_2_highlighted ---------------------------------------------
void C_DlgMainDialog::comboBox_IP_Fav_2_highlighted( const QString &text )
{comboBox_IP_Fav_2_activated( text );
}
//----------------------------------------- comboBox_IP_Fav_3_highlighted ---------------------------------------------
void C_DlgMainDialog::comboBox_IP_Fav_3_highlighted( const QString &text )
{comboBox_IP_Fav_3_activated( text );
}

//----------------------------------------- pushButton_FavPlus_clicked ---------------------------------------------
void C_DlgMainDialog::pushButton_FavPlus_clicked()
{   QString pass = lineEdit_PassWord->text().stripWhiteSpace();
    int i = comboBox_IP_Fav_insertItem(QString("QMYSQL3,")                       +
                                    lineEdit_UserName->text().stripWhiteSpace()  +","+
                                    QString("@")+PassWordEncode(pass)            +","+
                                    lineEdit_Hostname->text().stripWhiteSpace()  +","+
                                    lineEdit_Sql_Port->text().stripWhiteSpace() , comboBox_IP_Fav);
   if (i==-1) return;
    comboBox_IP_Fav->setCurrentItem ( i );
    saveFavoris(comboBox_IP_Fav);
}
//----------------------------------------- pushButton_FavPlus_2_clicked ---------------------------------------------
void C_DlgMainDialog::pushButton_FavPlus_2_clicked()
{   QString pass = lineEdit_PassWord_2->text().stripWhiteSpace();
    int i = comboBox_IP_Fav_insertItem(QString("QMYSQL3,")                         +
                                    lineEdit_UserName_2->text().stripWhiteSpace()  +","+
                                    QString("@")+PassWordEncode(pass)              +","+
                                    lineEdit_Hostname_2->text().stripWhiteSpace()  +","+
                                    lineEdit_Sql_Port_2->text().stripWhiteSpace() , comboBox_IP_Fav_2);
    if (i==-1) return;
    comboBox_IP_Fav_2->setCurrentItem ( i );
    saveFavoris(comboBox_IP_Fav_2);
}
//----------------------------------------- pushButton_FavPlus_3_clicked ---------------------------------------------
void C_DlgMainDialog::pushButton_FavPlus_3_clicked()
{   QString pass = lineEdit_PassWord_3->text().stripWhiteSpace();
    int i = comboBox_IP_Fav_insertItem(QString("QMYSQL3,")                         +
                                    lineEdit_UserName_3->text().stripWhiteSpace()  +","+
                                    QString("@")+PassWordEncode(pass)              +","+
                                    lineEdit_Hostname_3->text().stripWhiteSpace()  +","+
                                    lineEdit_Sql_Port_3->text().stripWhiteSpace() , comboBox_IP_Fav_3);
    if (i==-1) return;
    comboBox_IP_Fav_3->setCurrentItem ( i );
    saveFavoris(comboBox_IP_Fav_3);
}

//----------------------------------------- pushButtonRemove_clicked ---------------------------------------------
void C_DlgMainDialog::pushButtonRemove_clicked()
{if (comboBox_IP_Fav->text(comboBox_IP_Fav->currentItem())=="QMYSQL3,root,,localhost,3306") return;
    comboBox_IP_Fav->removeItem ( comboBox_IP_Fav->currentItem() );
    if (comboBox_IP_Fav->count()<=0) comboBox_IP_Fav_insertItem("QMYSQL3,root,,localhost,3306", comboBox_IP_Fav); // si  il doit en rester un c'est lui
    comboBox_IP_Fav->setCurrentItem (0 );
    comboBox_IP_Fav_activated( comboBox_IP_Fav->text(0) );
    saveFavoris(comboBox_IP_Fav);
}
//----------------------------------------- pushButtonRemove_2_clicked ---------------------------------------------
void C_DlgMainDialog::pushButtonRemove_2_clicked()
{if (comboBox_IP_Fav_2->text(comboBox_IP_Fav_2->currentItem())=="QMYSQL3,root,,localhost,3306") return;
    comboBox_IP_Fav_2->removeItem ( comboBox_IP_Fav_2->currentItem() );
    if (comboBox_IP_Fav_2->count()<=0) comboBox_IP_Fav_insertItem("QMYSQL3,root,,localhost,3306", comboBox_IP_Fav_2); // si  il doit en rester un c'est lui
    comboBox_IP_Fav_2->setCurrentItem (0 );
    comboBox_IP_Fav_2_activated( comboBox_IP_Fav_2->text(0) );
    saveFavoris(comboBox_IP_Fav_2);
}
//----------------------------------------- pushButtonRemove_3_clicked ---------------------------------------------
void C_DlgMainDialog::pushButtonRemove_3_clicked()
{if (comboBox_IP_Fav_3->text(comboBox_IP_Fav_3->currentItem())=="QMYSQL3,root,,localhost,3306") return;
    comboBox_IP_Fav_3->removeItem ( comboBox_IP_Fav_3->currentItem() );
    if (comboBox_IP_Fav_3->count()<=0) comboBox_IP_Fav_insertItem("QMYSQL3,root,,localhost,3306", comboBox_IP_Fav_3); // si  il doit en rester un c'est lui
    comboBox_IP_Fav_3->setCurrentItem (0 );
    comboBox_IP_Fav_3_activated( comboBox_IP_Fav_3->text(0) );
    saveFavoris(comboBox_IP_Fav_3);
}

//----------------------------------------- comboBox_IP_Fav_insertItem ---------------------------------------------
int C_DlgMainDialog::comboBox_IP_Fav_insertItem(const QString &item_txt, QComboBox *pQComboBox)
{   QListBox *pQListBox = pQComboBox->listBox ();
    if (pQListBox==0)  return -1;
    QListBoxItem *pQListBoxItem = pQListBox->findItem ( item_txt, Qt::ExactMatch );
    if (pQListBoxItem) return -1;                  // si il existe déjà cassos

    pQComboBox->insertItem ( item_txt );
    return pQComboBox->count()-1;
}
//----------------------------------------- destroy ---------------------------------------------
void C_DlgMainDialog::destroy(bool destroyWindow, bool destroySubWindows)
{
    Q_UNUSED(destroyWindow);
    Q_UNUSED(destroySubWindows);
    StopCUpdateThread();
    StopCIndexMakeThread();
    if (m_proc)
    {if (m_proc->isRunning())
        {m_proc->tryTerminate();
            QTimer::singleShot( 5000, m_proc, SLOT( kill() ) );
        }
    }
    if (m_pCMoteurBase!=0) delete m_pCMoteurBase;
    if (m_pCMedicaBase!=0) delete m_pCMedicaBase;
    if (m_TestBasesProc)
    {m_TestBasesProc->tryTerminate();
        QTimer::singleShot( 4000, m_TestBasesProc, SLOT( Kill() ) );
    }
}
//----------------------------------------- pushButton_Qt4Sdk_clicked ---------------------------------------------
void C_DlgMainDialog::pushButton_Qt4Sdk_clicked()
{QString optDir =  QFileDialog::getExistingDirectory (lineEdit_Qt4Sdk->text(), this, "findqt4sdk", tr("Chercher le répertoire du Sdk QT4") ) ;
#ifdef Q_WS_WIN

 if (optDir.length() && QFile::exists(optDir + "/bin/qmake.exe"))
#else
 if (optDir.length() && QFile::exists(optDir + "/bin/qmake"))
#endif
    { lineEdit_Qt4Sdk->setText(optDir);
#ifdef Q_WS_WIN
      QString key = "WinQt4Dir";
#endif
#ifdef Q_WS_X11
      QString key = "X11Qt4Dir";
#endif
#ifdef Q_OS_MACX
      QString key = "MacQt4Dir";
#endif
      CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Compilation", key ,optDir);
      CGestIni::Param_UpdateToDisk(G_pCApp->m_ParamPath, G_pCApp->m_ParamData);   // ecrire et mettre à jour le fichier sur le disque
    }
 else
    {QMessageBox::information( this, tr( "MedinTux demande votre attention" ),
                                     tr( "Ce chemin '%1' n'est pas celui de Qt. \n"
                                         "La compilation avec celui-ci est impossible.").arg(optDir) ,
                                     tr( "&ok"), 0, 0, 1, 1
                             ) ;
    }
}
//----------------------------------------- pushButton_Qt3_clicked ---------------------------------------------
void C_DlgMainDialog::pushButton_Qt3_clicked()
{QString optDir =  QFileDialog::getExistingDirectory (lineEdit_Qt3->text(), this, "findqt3", tr("Chercher le répertoire de QT3") ) ;
#ifdef Q_WS_WIN
 if (optDir.length() && QFile::exists(optDir + "/bin/qmake"))
#else
 if (optDir.length() && QFile::exists(optDir + "/bin/qmake.exe"))
#endif
    { lineEdit_Qt3->setText(optDir);
#ifdef Q_WS_WIN
      QString key = "WinQt3Dir";
#endif
#ifdef Q_WS_X11
      QString key = "X11Qt3Dir";
#endif
#ifdef Q_OS_MACX
      QString key = "MacQt3Dir";
#endif
      CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Compilation", key ,optDir);
      CGestIni::Param_UpdateToDisk(G_pCApp->m_ParamPath, G_pCApp->m_ParamData);   // ecrire et mettre à jour le fichier sur le disque
    }
 else
    {QMessageBox::information( this, tr( "MedinTux demande votre attention" ),
                                     tr( "Ce chemin '%1' n'est pas celui de Qt. \n"
                                         "La compilation avec celui-ci est impossible.").arg(optDir) ,
                                     tr( "&ok"), 0, 0, 1, 1
                             );
    }
}

//----------------------------------------- keepOnlyNumber ---------------------------------------------
QString  C_DlgMainDialog::keepOnlyNumber(const QString &strIn)
{int       i = 0;
 int     end = strIn.length();
 QString ret = "";
 while (i<end)
     {if (strIn[i]>='0' && strIn[i]<='9')  {ret += strIn[i];}
      ++i;
     }
 return ret;
}
//----------------------------------------- pushButton_compilation_clicked ---------------------------------------------
void C_DlgMainDialog::pushButton_compilation_clicked()
{   QString target(comboBox_Executables->currentText ());
    QString mode = tr("de tous les exécutables ");
    if (m_MakeExeRun == MAKE_EXE_RUN)
       {m_compil_In = 0;
        if (m_proc==0) return;
        if (m_proc->isRunning())
           {   m_proc->tryTerminate();
               QTimer::singleShot( 5000, m_proc, SLOT( kill() ) );
           }
        pushButton_compilation->setText(tr("&Compilation des exécutables"));
        m_MakeExeRun = MAKE_EXE_NOTRUN;
        QApplication::restoreOverrideCursor();
        return;
       }
    reinitLog();
    if (comboBox_Executables->currentItem ()==0)
       {//AllCompil();
        QStringList exeNotFoundList;

        if (IsExecutablesExist(exeNotFoundList))
           {if (QMessageBox::information( this, tr( "MedinTux demande votre attention" ),
                                                tr( "Tous les fichiers exécutables semblent déjà exister. \r\n"
                                                    "La compilation de ceux-ci est une opération longue.\r\n"
                                                    "Voulez-vous toujours la démarrer ?\r\n") ,
                                                    tr("&Oui"), tr("&Non"), 0,
                                                    1, 1 )==1) return ;
            exeNotFoundList = m_compilList;      // on compile tout
           }
        else
           {switch (QMessageBox::information( this, tr( "MedinTux demande votre attention" ),
                                                tr( "Une partie des fichiers exécutables semble déjà exister. \r\n"
                                                    "La compilation de tous les exécutables est une opération longue.\r\n"
                                                    "Voulez-vous ne compiler que ceux qui manquent ou tous les exécutables ?\r\n") ,
                                                    tr("&Que les manquants"), tr("&Tous"), tr("&Annuler"),
                                                    1, 1 )

                   )
                   { case 0: {mode = tr("des exécutables manquants "); break;}
                     case 1: {exeNotFoundList = m_compilList;          break;}
                     case 2: return;
                   }
           }

        ++m_compil_In;
        QTime timeDeb = QTime:: currentTime();
        toLog( tr ("<br>===== Compilation <font color=#ffa800><b> %1 </b></font>"
                   "commencée à : <font color=#0000FF><b>%2</b></font> en cours ... =====").arg(mode, timeDeb.toString("hh:mm:ss")));
        int nbTotal = exeNotFoundList.size();
        progressBar_Load->setTotalSteps(nbTotal*2);
        for( int i=0; i<nbTotal; ++i)
           {QString target = exeNotFoundList[i];        //<font color=#ffa800><b> %1 </b></font>
            textLabel_Progress->setText(tr("Compilation de <b>'%1'</b> en cours ... <font color=#ff0000><b>%2 sur %3</b></font> ").arg(target,QString::number(i+1),QString::number(nbTotal)) );
            progressBar_Load->setProgress(i*2+1); qApp->processEvents();qApp->processEvents();
            Compilation(getTargetSrcPath(target), target);  while (m_MakeExeRun == MAKE_EXE_RUN)  {qApp->processEvents();}; if (m_compil_In==0) break;
            progressBar_Load->setProgress(i*2+2); qApp->processEvents();qApp->processEvents();
           }
        long    s = timeDeb.secsTo (QTime:: currentTime());
        long    h = s/3600; s -=  h*3600;
        long    m = s/60;   s -=  m*60;
        toLog( tr ("<br>===== Compilation <font color=#ffa800><b> %1 </b></font>"
                   "terminée en : <font color=#0000FF><b>%2h%3mn%4s</b></font> ========").arg(mode, QString::number(h) , QString::number(m) , QString::number(s) ));
        progressBar_Load->setProgress(0);
       }
    else
       {textLabel_Progress->setText(tr("Compilation de <b>'%1'</b> en  cours ... ").arg(target) );
        Compilation(getTargetSrcPath(target), target);
       }
}

//----------------------------------------- IsSesamVersionExist ---------------------------------------------
QString C_DlgMainDialog::IsSesamVersionExist()
{QString path = G_pCApp->m_PathAppli + QDir::separator() + ".." + QDir::separator() + ".." + QDir::separator() +
                "SesamVitale-QT3" + QDir::separator() +"qtsingleapplication.pri";
    path   = QDir::cleanDirPath (path);
    if ( QFile::exists (path) ) return path;
    else                        return QString::null;
}

//----------------------------------------- getTargetSrcPath -----------------------------------------------------------------
QString C_DlgMainDialog::getTargetSrcPath(const QString &target)
{    QString main_path(G_pCApp->m_PathAppli + "../..");
     QDir qDir(main_path);
     qDir.convertToAbs();
     //QString target = target_in.stripWhiteSpace();
     if (target=="compta-plugins")
        {main_path = qDir.path() + "/comptabilite/compta-plugins";
        }
     else if ( target=="check_dus" )
        {main_path = qDir.path() + "/comptabilite/check_dus";
        }
     else if ( target=="comptabilite" )
        {main_path = qDir.path() + "/comptabilite/comptabilite";
        }
     else
          {main_path = qDir.path() + QDir::separator() + target + QDir::separator() +"src";
          }
    return main_path;
}
//----------------------------------------- tryToFindQt3 ---------------------------------------------
QString  C_DlgMainDialog::tryToFindQt3()
{
QString qt3Path = "";

#ifdef Q_WS_WIN
    qt3Path = CGestIni::Param_ReadUniqueParam(G_pCApp->m_ParamData,  "Compilation", "WinQt3Dir");
    if ( QFile::exists(qt3Path + "/bin/qmake.exe") ) return qt3Path;
    else                                             return "C:/Qt3/3.3.4";
#endif
#ifdef Q_OS_MACX
    qt3Path = CGestIni::Param_ReadUniqueParam(G_pCApp->m_ParamData,  "Compilation", "MacQt3Dir");
    if ( QFile::exists(qt3Path + "/bin/qmake") )     return qt3Path;
    QString ret = QString::null;
    if        (QFile::exists(QDir::cleanDirPath(G_pCApp->m_PathAppli+"../../Qt/bin/qmake")))   ret = QDir::cleanDirPath(G_pCApp->m_PathAppli+"../../Qt");
    else if   (QFile::exists("/usr/lib/qt3/bin/qmake"))  ret = "/usr/lib/qt3";
    return    ret;
#endif
#ifdef Q_WS_X11
    qt3Path = CGestIni::Param_ReadUniqueParam(G_pCApp->m_ParamData,  "Compilation", "X11Qt3Dir");
    if ( QFile::exists(qt3Path + "/bin/qmake") )     return qt3Path;
    QString ret    = QString::null;
    QString toTest = QDir::cleanDirPath(G_pCApp->m_PathAppli + "../../../qt3_l64");
    if        (QFile::exists(toTest+"/bin/qmake"))       return toTest;
    if        (QFile::exists("/usr/lib/qt/bin/qmake"))   ret = "/usr/lib/qt";
    else if   (QFile::exists("/usr/lib/qt3/bin/qmake"))  ret = "/usr/lib/qt3";
    else if   (QFile::exists("/usr/bin/qmake"))          ret = "/usr";
    return    ret;
#endif
}

//----------------------------------------- tryToFindSdkQt4 ---------------------------------------------
QString  C_DlgMainDialog::tryToFindSdkQt4()
{
QString qt4Path = "";
#ifdef Q_WS_WIN
    qt4Path = CGestIni::Param_ReadUniqueParam(G_pCApp->m_ParamData,  "Compilation", "WinQt4Dir");
    if ( QFile::exists(qt4Path + "/bin/qmake.exe") ) return qt4Path;
    else                                             return "C:/Qt4W/qt";
#endif
#ifdef Q_OS_MACX
 qt4Path = CGestIni::Param_ReadUniqueParam(G_pCApp->m_ParamData,  "Compilation", "MacQt4Dir");
 if ( QFile::exists(qt4Path + "/bin/qmake") ) return qt4Path;
 QStringList listOptDir = CGestIni::listDirectory("/usr/local/Trolltech", "", "Qt-", "|", FALSE, TRUE);
 int         lastIndex  = -1;
 int             lastN  = 0;
 int                  n = 0;
 for (int i=0; i<(int)listOptDir.count(); ++i)
     {if ( (n = keepOnlyNumber(listOptDir[i]).toInt())>lastN)
         {lastN     = n;
          lastIndex = i;
         }
     }
 if (lastIndex != -1)   return listOptDir[lastIndex].prepend("/usr/local/Trolltech/");
 else                   return QString::null;
#endif
#ifdef Q_WS_X11
 qt4Path =CGestIni::Param_ReadUniqueParam(G_pCApp->m_ParamData,  "Compilation", "X11Qt4Dir");
 if ( QFile::exists(qt4Path + "/bin/qmake") ) return qt4Path;
 //............ rechercher bon path qt ...................................
 QString           prefix  = QDir::cleanDirPath(G_pCApp->m_PathAppli + "../../../qt4_l64");
 QString           sufix   = "";
 QStringList    listOptDir = CGestIni::listDirectory(prefix, "", "Qt-", "|", FALSE, TRUE);
 prefix                   += "/";
 if ( listOptDir.count()==0)                     // si pas trouvee on teste si cette version du sdk Qt4 (ubuntu ou autres 4.8.5) existe
    { prefix  = "/opt/";            // on teste si cette version du sdk Qt4 (mandriva) existe
      sufix   = "/qt/";             // on teste si cette version du sdk Qt4 (mandriva) existe
      listOptDir = CGestIni::listDirectory("/opt", "", "qtsdk-", "|", FALSE, TRUE);

      if ( listOptDir.count()==0)                     // si pas trouvee on teste si cette version du sdk Qt4 (ubuntu ou autres 4.8.5) existe
         { prefix     = "/usr/local/Trolltech";
           listOptDir = CGestIni::listDirectory(prefix, "", "Qt-", "|", FALSE, TRUE);
           prefix    += "/";
           sufix      = "";
         }
  }
 int         lastIndex  = -1;
 int             lastN  = 0;
 int                  n = 0;
 for (int i=0; i< (int)listOptDir.count(); ++i)
     {if ( (n = keepOnlyNumber(listOptDir[i]).toInt())>lastN)
         {lastN     = n;
          lastIndex = i;
         }
     }
 if (lastIndex != -1)   return listOptDir[lastIndex].prepend(prefix)+sufix;
 else                   return QString::null;
 #endif
}
//----------------------------------------- Compilation -----------------------------------------------------------------
void C_DlgMainDialog::Compilation(const QString &path, const QString & target)
{   pushButton_SetBases->hide();
    pushButton_LancerDrTux->hide();
    comboBox_Bases->hide();
    comboBox_Executables->hide();
    int isQT3          =  0;
    QString sdkDir     =  lineEdit_Qt4Sdk->text().stripWhiteSpace(); if (!sdkDir.endsWith("/")) sdkDir += "/";
    QString os         = "unknow";
#ifdef  Q_WS_MAC
    bool isQT4Here     =  QFile::exists(sdkDir+"bin/qmake");
    os                 =  "Mac Os";
#endif
#ifdef Q_WS_X11
    bool isQT4Here     =  QFile::exists(sdkDir+"bin/qmake");            // pour GG
    os                 =  "Linux";
#endif

#ifdef Q_WS_WIN
    bool isQT4Here     = QFile::exists(sdkDir+"bin/qmake.exe");
    os                 =  "Windows";
#endif
    QString modulePath =  QDir::cleanDirPath(G_pCApp->m_PathAppli + "../../" + target)+"/";
    if (target=="compta-plugins"||target=="check_dus"||QFile::exists(modulePath+target+".kdevelop")) isQT3 = 1;
    //....................... corriger version des UI de MedinTuxTools...............................
    QString toolsPath(G_pCApp->m_PathAppli + "../..");
    QDir qDir(toolsPath);
    qDir.convertToAbs();
    toolsPath = qDir.path() + "/MedinTuxTools";
    G_pCApp->RemoveAllDesignerVersion(toolsPath);
    G_pCApp->RemoveAllDesignerVersion(toolsPath+"/Calendar");
    G_pCApp->RemoveAllDesignerVersion(toolsPath+"/medicabase");

    //.................... corriger les versions des UI du module en cours .....................
    QDir::setCurrent ( path );
    if ( (target == "QFseVitale") && IsSesamVersionExist() )
       {if (isQT3) G_pCApp->RemoveAllDesignerVersion(getTargetSrcPath("QFseVitale"));
       }
    else
       {if (isQT3) G_pCApp->RemoveAllDesignerVersion(path);
       }
    //................. relever date ..........................................
    m_dtDebComp = QDateTime::currentDateTime();
    QString script     = "";
    m_CurrentCompil    = target;
    //............................ QT3 QT3 QT3 QT3 QT3 QT3 QT3 QT3 QT3 QT3 ........................................................
   if (isQT3)
       {    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
            toLog( tr ("<br>============= Compilation de <font color=#ffa800><b> '%1' </b></font> sur <font color=#00e5e8><b>%2</b></font> <font color=#ff0000><b>(librairie QT3)</b></font> en cours ... =============").arg(target,os));
            QString script_file = "";
         #ifdef Q_WS_WIN
            QString qtdir = lineEdit_Qt3->text().stripWhiteSpace();
            QString disk  = G_pCApp->m_PathAppli.left(2);
            script  =  QString ("echo \"================= compilation de %4 ===========================\"\r\n"
                                "%1\r\n"
                                "cd %2\r\n"
                                "set QMAKE=%3\\bin\\qmake.exe\r\n"
                                "set QMAKESPEC=win32-msvc\r\n"
                                "set MODULE=%4\r\n").arg(disk,G_pCApp->m_PathAppli,qtdir, target);
            script +=           "cd ../../%MODULE%/src\r\n"
                                "echo \"================= se placer dans %MODULE%/src ===========================\"\r\n"
                                "erase %MODULE%.dsw\r\n"
                                "erase %MODULE%.dsp\r\n"
                                "if exist %MODULE%.pro %QMAKE% -t vcapp -o %MODULE%.dsp %MODULE%.pro\r\n"
                                "if exist src.pro %QMAKE% -t vcapp -o %MODULE%.dsp src.pro\r\n"
                                "cd ../../mise_a_jour/bin\r\n"
                                "mise_a_jour.exe MSDEV  ../../%MODULE%/src/%MODULE%.dsp %MODULE%.exe\r\n";
            if  (target=="comptabilite")
                {script +=      "cd ../../comptabilite/comptabilite\r\n";
                }
            else if  (target=="check_dus")
                {script +=      "cd ../../comptabilite/check_dus\r\n";
                }
            else if  (target=="compta-plugins")
                {script +=      "cd ../../comptabilite/compta-plugins\r\n";
                }
            else
                {script +=      "cd ../../%MODULE%/src\r\n";
                }
            script +=           "msdev %MODULE%.dsp /MAKE /CLEAN\r\n"
                                "msdev %MODULE%.dsp /MAKE ALL\r\n";
            script_file = G_pCApp->m_PathAppli+"makeModuleQt3.bat";
            script_file = script_file.replace("/","\\");
         #endif
         #ifdef  Q_WS_MAC
            QString qtdir = lineEdit_Qt3->text().stripWhiteSpace();
            script  =  "#! /bin/sh\n"
                       "#----- DEB FOR MAC --------\n"
                       "  QTDIR=\"";
            script += qtdir + "\"\n export QTDIR\n";
            script += "  PATH=$PATH\":"  + qtdir +"/bin\"\n export PATH"        // si on met \r\n   le bash se bache
            "\n#----- END FOR MAC --------\n"
            "DYLD_LIBRARY_PATH=$QTDIR/lib:$DYLD_LIBRARY_PATH\n"
            "DYLD_LIBRARY_PATH=$QTDIR/plugins/sqldrivers:$DYLD_LIBRARY_PATH\n"
            "DYLD_LIBRARY_PATH=/usr/local/mysql/lib:$DYLD_LIBRARY_PATH\n"
            "INCLUDE=/usr/local/mysql/include:$INCLUDE\n"
            "QMAKESPEC=$QTDIR/mkspecs/macx-g++\n"
            "export QMAKESPEC\n"
            "export DYLD_LIBRARY_PATH\n"
            "export INCLUDE\n"
            "cd '"+G_pCApp->m_PathAppli+"'\n"
            "./MakeAllMac.sh '" + m_CurrentCompil + "' '" + lineEdit_Qt3->text() + "'";
            script_file = G_pCApp->m_PathAppli + "makeModule.sh";
        #endif
        #ifdef Q_WS_X11
            QString qtdir = lineEdit_Qt3->text().stripWhiteSpace();
            script  =  "#! /bin/sh\n"
                       "#----- DEB FOR LINUX --------\n"
                       "  QTDIR=\"";
            script += qtdir + "\"\n export QTDIR\n";
            script += "  PATH=$PATH\":"+ qtdir +"/bin\"\n export PATH"        // si on met \r\n   le bash se bache
            "\n#----- END FOR LINUX --------\n"
            "QMAKESPEC=$QTDIR/mkspecs/linux-g++\n"
            "export QMAKESPEC\n"
            "cd '"+G_pCApp->m_PathAppli+"'\n"
            "./MakeAllMac.sh '" + m_CurrentCompil + "' '" + lineEdit_Qt3->text() + "'";
            script_file = G_pCApp->m_PathAppli + "makeModule.sh";
        #endif
        CGestIni::Param_UpdateToDisk(script_file, script);
        execute(script_file);      
        pushButton_compilation->setText(tr("&Interrompre la compilation des exécutables en cours"));
        m_MakeExeRun = MAKE_EXE_RUN;
       }

   //............................ QT4 QT4 QT4 QT4 QT4 QT4 QT4 QT4 QT4 ........................................................
   else if (QFile::exists(modulePath + "src/"+target+".pro"))
       {if (isQT4Here)
           {toLog( tr ("<br>============= Compilation de <font color=#ffa800><b> '%1' </b></font> sur <font color=#00e5e8><b>%2</b></font> <font color=#ff0000><b>(librairie QT4)</b></font> en cours ... =============").arg(target,os));
            QFile::remove ( modulePath+"src/makeQT4Module.sh" );
            QFile::remove ( modulePath+"src/Makefile" );
            //toLog( tr ("<br> Remove <font color=#ff0000><b>QT4</b></font> de<font color=#ffa800><b> %1 </b></font>en cours ...").arg(modulePath+"src/makeQT4Module.sh"));
            QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
            QDir::setCurrent ( modulePath + "src" );
            QString script_file = "";
        #ifdef  Q_WS_WIN
            QString disk        = G_pCApp->m_PathAppli.left(2);
            QString pathMingw   = QDir::cleanDirPath(sdkDir+"../mingw");
            script = "echo \"================= compilation de {{target}} ===========================\"\r\n"
                     "{{disk}}\r\n"
                     "cd {{pathAppli}}\r\n"
                     "set MODULE={{target}}\r\n"
                     "set QTDIR={{sdkDir}}\r\n"
                     "set QMAKE=%QTDIR%/bin/qmake.exe\r\n"
                     "set QMAKESPEC=%QTDIR%/mkspecs/win32-g++\r\n"
                     "set PATH=%PATH%;%QTDIR%/bin;%QTDIR%/../mingw/bin\r\n"
                     "cd ../../%MODULE%/src\r\n"
                     "if exist %MODULE%.pro %QMAKE% -spec %QMAKESPEC% -win32 CONFIG+=release -o Makefile %MODULE%.pro\r\n"
                     "if exist src.pro %QMAKE% -spec %QMAKESPEC% -win32 CONFIG+=release -o Makefile src.pro\r\n"
                     "%QTDIR%/../mingw/bin/mingw32-make.exe clean\r\n"
                     "%QTDIR%/../mingw/bin/mingw32-make.exe\r\n";
            script.replace("{{sdkDir}}",   sdkDir);
            script.replace("{{target}}",   target);
            script.replace("{{disk}}",     disk);
            script.replace("{{pathAppli}}",G_pCApp->m_PathAppli);
            script_file = G_pCApp->m_PathAppli+"makeModuleQt4.bat";
            script_file = script_file.replace("/","\\");
        #endif
        #ifdef  Q_WS_MAC
            script = "#!/bin/bash\n"
                     "LD_LIBRARY_PATH={{sdkDir}}lib\n"
                     "PATH={{sdkDir}}bin:/usr/bin:/bin:/usr/X11R6/bin/:\n"
                     "QTDIR={{sdkDir}}\n"
                     "QT_PLUGIN_PATH={{sdkDir}}plugins\n"
                     "QMAKESPEC=$QTDIR/mkspecs/macx-g++\n"
                     "export QMAKESPEC\n"
                     "export QT_PLUGIN_PATH\n"
                     "export QTDIR\n"
                     "export PATH\n"
                     "export LD_LIBRARY_PATH\n";
                      script += "cd '" + modulePath + "src'\n"
                     "qmake '{{target}}.pro'\n"
                     "make clean\n"
                     "make\n"
                     "make clean\n"
                     "cd '"+ G_pCApp->m_PathAppli + "'\n"
                     "./modulepatch-Qt4.sh {{target}}\n"
                     "cd '"+ G_pCApp->m_PathAppli + "'\n";
             if (target=="guinch")   script += "strip ../../Manager/bin/{{target}}.app/Contents/MacOS/{{target}}\n";   // sous mac pas de -s pour strip
             else                    script += "strip ../../{{target}}/bin/{{target}}.app/Contents/MacOS/{{target}}\n";
             script.replace("{{sdkDir}}",sdkDir);
             script.replace("{{target}}",target);
             script_file = modulePath+"src/makeQT4Module.sh";
        #endif
        #ifdef  Q_WS_X11
            script = "#!/bin/bash\n"
                     "cd '../../{{target}}/src'\n"
                     "LD_LIBRARY_PATH={{sdkDir}}lib/qtcreator:\n"
                     "PATH={{sdkDir}}bin:/usr/bin:/bin:/usr/X11R6/bin/:\n"
                     "QTDIR={{sdkDir}}\n"
                     "QT_PLUGIN_PATH={{sdkDir}}plugins\n"
                     "QMAKESPEC=$QTDIR/mkspecs/linux-g++\n"
                     "PWD={{sdkDir}}bin\n"
                     "export QMAKESPEC\n"
                     "export QT_PLUGIN_PATH\n"
                     "export PWD\n"
                     "export QTDIR\n"
                     "export PATH\n"
                     "export LD_LIBRARY_PATH\n"
                     "qmake {{target}}.pro\n"
                     "make clean\n"
                     "make\n"
                     "make clean\n";
            if (target=="guinch")   script += "strip -s ../../Manager/bin/{{target}}\n";
            else                    script += "strip -s ../bin/{{target}}\n";
            script.replace("{{sdkDir}}",sdkDir);
            script.replace("{{target}}",target);
            script_file = modulePath+"src/makeQT4Module.sh";
        #endif
            CGestIni::Param_UpdateToDisk(script_file, script); 
            execute(script_file);
            pushButton_compilation->setText(tr("&Interrompre la compilation des ex\303\251cutables en cours"));
            m_MakeExeRun = MAKE_EXE_RUN;
           }
         else
           {toLog( tr ("<br>============= Compilation <font color=#ff0000><b>QT4</b></font> de<font color=#ffa800><b> %1 </b></font>Avort\303\251e (sdk QT4 Absent) ... =============").arg(target));
            pushButton_SetBases->show();
            comboBox_Executables->show();
            comboBox_Bases->show();
            pushButton_LancerDrTux->show();
            progressBar_Load->show();
           }
       }
}

//----------------------------------------- IsExecutablesExist --------------------------------------------------------------------
bool C_DlgMainDialog::IsExecutablesExist(QStringList &exeNotFoundList)
{   toLog( tr("============ Vérification de la présence des exécutables ============= "));
    exeNotFoundList.clear();
    for( int i=0; i<(int)m_compilList.size(); ++i)
       {QString target = m_compilList[i].stripWhiteSpace();
        if ( (target == "QFseVitale"||target == "qgetdatasemp"))
           {if ( IsSesamVersionExist() && ! IsThisExecutableExist(target) )
               {exeNotFoundList.append(target);
               }
           }
        else
           {if ( ! IsThisExecutableExist(target) )
               {exeNotFoundList.append(target);
               }
           }
       }
    return (exeNotFoundList.size()==0);
}

//----------------------------------------- IsThisExecutableExist --------------------------------------------------------------------
bool C_DlgMainDialog::IsThisExecutableExist(const QString &exe_name)
{   QString folder = G_pCApp->m_PathAppli; if (folder.endsWith("/")) {folder = folder.left(folder.length()-1);};
    QString path   = "";
	int pos        = -1;
	//............... la formation du path est alambiquee pour .....................
	//                pouvoir etre juste aussi bien qaund set_bases demarre de .app que de MacOS
    if (exe_name.left(6)=="guinch")
       { path   = G_pCApp->m_PathAppli +"../../Manager/bin/guinch";
         path   = QDir::cleanDirPath(path);
#ifdef Q_OS_MACX
         path += ".app/Contents/MacOS/guinch";
#endif
#ifdef Q_WS_WIN
         path += ".exe";
         toLog( tr("Cet exécutable <font color=#ff0000><b>'%1'</b></font> est inutile sous Windows ").arg(exe_name) );
         return true;
#endif
       }
	else if (exe_name.left(14) == "compta-plugins")
	   { path   = G_pCApp->m_PathAppli +"../../comptabilite/bin/compta-plugins";
         path   = QDir::cleanDirPath(path);
#ifdef Q_OS_MACX
         path += ".app/Contents/MacOS/compta-plugins";
#endif
#ifdef Q_WS_WIN
         path += ".exe";
#endif
       }
	else if (exe_name.left(9) == "check_dus")
	   { path   = G_pCApp->m_PathAppli +"../../comptabilite/bin/check_dus";
         path   = QDir::cleanDirPath(path);
#ifdef Q_OS_MACX
         path += ".app/Contents/MacOS/check_dus";
#endif
#ifdef Q_WS_WIN
         path += ".exe";
#endif
       }
    else
       { path   =  CGestIni::Construct_Name_Exe(exe_name, folder);
         pos    =  path.find(exe_name);
         if (pos==-1) return FALSE;
         path   =  G_pCApp->m_PathAppli +"../../" + path.mid(pos);
         path   =  QDir::cleanDirPath(path);
       }
    if (  ! QFile::exists (path) )
       { toLog( tr("Cet exécutable <font color=#ff0000><b>'%1'</b></font> n'existe pas ").arg(exe_name) );
         return FALSE;
       }
    toLog( tr("Cet exécutable <font color=#ffa800><b>'%1'</b></font> existe déjà ").arg(exe_name) );
    return TRUE;
}


//----------------------------------------- execute ---------------------------------------------------------------------
void C_DlgMainDialog::execute(const QString &command, const QString curDir /*=""*/)
{   if (curDir.length()==0) QDir::setCurrent ( G_pCApp->m_PathAppli );
    else                    QDir::setCurrent ( curDir );
    m_proc = new QProcess(this);  if (m_proc == 0 ) return;

#ifdef Q_WS_WIN
    QString path_cmd  = "";
    if      (QFile::exists("C:\\Windows\\SysWOW64\\cmd.exe"))  path_cmd = "C:\\Windows\\SysWOW64\\cmd.exe";
    else if (QFile::exists("C:\\Windows\\system32\\cmd.exe"))  path_cmd = "C:\\Windows\\system32\\cmd.exe";
	m_proc->addArgument(path_cmd);
	m_proc->addArgument("/c");
    m_proc->addArgument(command);
#else
    m_proc->addArgument("bash");
    m_proc->addArgument(command);

#endif
    connect(m_proc, SIGNAL(readyReadStderr()), this, SLOT(updateError()));
    connect(m_proc, SIGNAL(readyReadStdout()), this, SLOT(updateText()));
    connect(m_proc, SIGNAL(processExited()),   this, SLOT(updateExit()));
    m_proc->start();
}

//----------------------------------------- updateError -------------------------------------------------------------------
void C_DlgMainDialog::updateError()
{logOutCompil(m_proc->readStderr());
}

//----------------------------------------- updateText --------------------------------------------------------------------
void C_DlgMainDialog::updateText()
{logOutCompil(m_proc->readStdout());
}

//----------------------------------------- logOutCompil --------------------------------------------------------------------
void C_DlgMainDialog::logOutCompil(const QByteArray &data)
{   QString  data_str = "";
    QString   retrait = "";
    int           pos = 0;
    int  len          = (int) data.size ();
    char *pt          = (char*)(const char*)data.data();
    char *buffer      = new char[len+1]; if (buffer==0) return;
    for (int i=0; i<len; ++i)
        {if (pt[i]=='\n')             {buffer[pos]=pt[i]; ++pos;}
         if (pt[i]>=32 && pt[i]<=126) {buffer[pos]=pt[i]; ++pos;}
        }
    buffer[pos] = 0;
    data_str    = buffer;
    delete [] buffer;
    pos = -1;
    pos = data_str.find(tr("attention :"));
    if (pos==-1)     pos = data_str.find(tr("Warning:"));
    if (pos !=-1)
       {data_str = QString("<font color=#fffea5>"+retrait+" ATTENTION : ") + data_str + "</font><font color=#00e413></font>";
        toLog(data_str) ;
        return;
       }
    if (pos==-1) pos = data_str.find(tr("error:"));
    if (pos==-1) pos = data_str.find(tr("erreur:"));
    if (pos==-1) pos = data_str.find(tr("Failure"));
    if (pos==-1) pos = data_str.find(tr("Erreur"));
    if (pos != -1)
       {data_str = QString("<font color=#ff0000>"+retrait+" ERREUR : ") + data_str + "</font><font color=#00e413></font>";
       }
    if (data_str.length()) toLog(data_str) ;
}

//----------------------------------------- updateExit --------------------------------------------------------------------
void C_DlgMainDialog::updateExit()
{
    if (m_proc->normalExit())
       {
        QDateTime dtEnd= QDateTime::currentDateTime();
        long    s = m_dtDebComp.secsTo (dtEnd);
        long    h = s/3600; s -=  h*3600;
        long    m = s/60;   s -=  m*60;
        QApplication::restoreOverrideCursor();
        toLog( tr ("=== Compilation terminée en :  <font color=#0000FF>")+
                                  QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s</font> ===\n");
        textLabel_Progress->setText("" );
        pushButton_SetBases->show();
        comboBox_Bases->show();
        comboBox_Executables->show();
        pushButton_LancerDrTux->show();
        progressBar_Load->show();
        pushButton_compilation->setText(tr( "&Compilation des exécutables" ));
        //.................. couper le log au module concerne ........................
        QString toWriteOnDisk = textEdit_Message->text();
        QString toFind        = "========== Compilation de '";
                toFind        = toFind + m_CurrentCompil + "'";
        int pos               = toWriteOnDisk.find(toFind);
        if (pos != -1) toWriteOnDisk = toWriteOnDisk.mid(pos);
        CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"Compilation_"+m_CurrentCompil+".log", toWriteOnDisk);
        m_MakeExeRun = MAKE_EXE_NOTRUN;
   #ifdef Q_WS_WIN
   #else
        //............. strip de l'executable ....................
        QDir::setCurrent ( G_pCApp->m_PathAppli );
        QString script =  "#! /bin/sh\r\n ./stripall.sh " + m_CurrentCompil;
        CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"stripModule.sh", script);
        QProcess proc(this);
        proc.addArgument("bash");
        proc.addArgument(G_pCApp->m_PathAppli + "stripModule.sh");
        proc.start();
   #endif  
    }
    else {
        toLog("Error");
        pushButton_SetBases->show();
        comboBox_Executables->show();
        comboBox_Bases->show();
        pushButton_LancerDrTux->show();
        progressBar_Load->show();
    }


}
//----------------------------------------- pushButton_SetBases_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButton_SetBases_clicked()
{setBase();
}

//----------------------------------------- setBase --------------------------------------------------------------------
void C_DlgMainDialog::setBase(const QString &sqlFile /* = "" */)
{
    QDateTime dtDeb = QDateTime::currentDateTime();
    textEdit_Message->clear();
    //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............
    QString  driverName , baseName , userName , passWord, hostName, port;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "DrTuxTest", "Master", &driverName, &baseName) !=0
        )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "DrTuxTest";
    }
   userName = lineEdit_UserName->text().stripWhiteSpace();
   passWord = lineEdit_PassWord->text().stripWhiteSpace();
   hostName = lineEdit_Hostname->text().stripWhiteSpace();
   port     = lineEdit_Sql_Port->text().stripWhiteSpace();
    //.....................................  liste des bases de donnees ..........................................................
    QString err;
    QStringList basesListToSet           = G_pCApp->GetBasesListToSet();
    QStringList basesListAlreadyPresents = GetListBasesInMySql(&err, basesListToSet);
    QStringList basesListNotFound;
    QStringList basesListFound;
    if (err.length())
       {toLog(tr("============ Erreur  : GetBasesListToSet()==============") + err);
       }

    for (int i=0; i<(int)basesListToSet.size(); ++i)
        {
         baseName = basesListToSet[i];
         if (basesListAlreadyPresents.findIndex (baseName) == -1)
            { basesListNotFound.append(basesListToSet[i]);
            }
         else
            { basesListFound.append(basesListToSet[i]);
            }
        }
    //.....................................  on affiche un peu ..........................................................
    if (basesListFound.size())
       {toLog(tr("============ Liste des bases déjà initialisées =============="));
        for (int i=0; i<(int)basesListFound.size(); ++i)    G_pCApp->Datasemp_OutMessage(textEdit_Message, QString(".        <font color=#ff7601>") +    basesListFound[i]    + "</font>\n");
       }
    if (basesListNotFound.size())
       {toLog(tr("============ Liste des bases non initialisées =============="));
        for (int i=0; i<(int)basesListNotFound.size(); ++i) G_pCApp->Datasemp_OutMessage(textEdit_Message, QString(".        <font color=#ff0000>")  +   basesListNotFound[i] + "</font>\n");
       }
    //.....................................  on se pose les bonnes questions ..........................................................
    if  (comboBox_Bases->currentItem()==0 && sqlFile.length()==0)
    // ............... si toutes les base sont concernees ("Toutes les bases" selectionne dans la combobox) ......................
    //                    si tout est complet cassos,
    //                    si juste une partie manque poser la question,
    //                    si tout manque on installe tout sans poser de qestion car pas de risque d'ecraser quoi que ce soit
        { if (basesListNotFound.size()==0)            // toutes les bases sont trouvee on se casse pour NE PAS ECRASER L'EXISTANT
              { if ( G_pCApp->m_justInstallBase ) return;   // ne pas poser la question (automatiser la sortie)
                QMessageBox::information( this, tr( "INSTALLATION DES BASES" ),
                                               tr(  "L'installation des bases de données semble COMPLETE. \r\n"
                                                 ) , tr("&Annuler"),0,0,  1, 1 );
                return;
              }
          else if (basesListFound.size())             // il y a des bases deja installee on propose que les manquantes pour NE PAS ECRASER L'EXISTANT
              { if ( ! G_pCApp->m_justInstallBase )   // si sortie non automatisee alors poser la question
                   { switch (QMessageBox::information( this, tr( "INSTALLATION DES BASES" ),
                                                             tr( "Une partie des bases de données semble déjà exister. \r\n"
                                                             "La réinstallation des bases ne concernera que celles \r\n"
                                                             "manquantes.\r\n") ,
                                                              tr("Que les bases manquantes"), tr("&Annuler"),0,
                                                             1, 1 )

                            )
                            { case 1: return;
                            }
                   }   // end if ( ! G_pCApp->m_justInstallBase )
              }
         //......... a ce stade toutes les bases de la liste 'basesListNotFound' seront installees
        }
    else
    //......... si seulement une base est concernée : donc selectionnee dans la combobox .........................
    //          on efface la liste des bases non trouvees a creer, et on ne met que celle de la combobox
    //          la question de validation sera posee dans la fonction : G_pCApp->restaureBase() lors de detection
    //          d'une base portant ce nom et deja presente
        {basesListNotFound.clear();
         basesListNotFound.append(comboBox_Bases->currentText ());
        }

    if (sqlFile.length())
    {//bool setButtonVidal = FALSE;
     // textLabel_Progress->setText( tr("Restauration de la base de données : ") + sqlFile + tr(" en cours ..." ));
     G_pCApp->restaureBase(sqlFile,             // en sera extrait le nom de la base
                           driverName,          // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                           userName,            // = "root"
                           passWord,            // = ""
                           hostName,            // = "localhost"
                           port,
                           textEdit_Message,
                           progressBar_Load,
                           textLabel_Progress
                          );
    }
    else
    {   bool setButtonVidal = FALSE;
        for (int i=0; i<(int)basesListNotFound.size(); ++i)
            {baseName               = basesListNotFound[i];
             QString fileToRestore  = baseName; fileToRestore = fileToRestore.prepend("Dump_") +".sql"; fileToRestore = fileToRestore.prepend(G_pCApp->m_PathAppli+"SqlCreateTable/");//Dump_DrTuxTest.sql
             textLabel_Progress->setText( tr("Installation de la base de données : <b>") + baseName + tr("</b> en cours ..." ));
             G_pCApp->restaureBase(fileToRestore,       // en sera extrait le nom de la base
                                   driverName,          // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                   userName,            // = "root"
                                   passWord,            // = ""
                                   hostName,            // = "localhost"
                                   port,
                                   textEdit_Message,
                                   progressBar_Load,
                                   textLabel_Progress
                                  );
            if (baseName=="MedicaTuxTest"||baseName=="BaseGetTest") setButtonVidal = TRUE;
           }
        //QSqlDatabase::removeDatabase (dataBase);  // plante sous linux
        if (setButtonVidal) SetButtonIndexVidal(0);
    }

    QDateTime dtEnd = QDateTime::currentDateTime();
    long    s = dtDeb.secsTo (dtEnd);
    long    h = s/3600; s -=  h*3600;
    long    m = s/60;   s -=  m*60;

    QString text   = tr (" === Installation des bases de données terminée en : <font color=#ffd201><b>") +
                     QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s </b></font>===\n";
    toLog(text);
    textLabel_Progress->setText( "" );
    progressBar_Load->setProgress(0);
    int ret = setMajVersionButton();
    if (ret==1)
        { pushButton_MajBases_clicked();
        }
    else if (ret==2)
        { tabWidget_Instal->setTabEnabled ( m_pTabQWidgetBases,  FALSE );
          tabWidget_Instal->setTabEnabled ( m_pTabQWidgetTherap, FALSE );

          pushButton_SetBases->hide();
          pushButton_MajBases->hide();
          comboBox_Bases->hide();
          SetButtonIndexVidal(0);
          pushButtonConnexionAppl->setText(tr("&Se connecter à la base de données"));
          return;
       }
    pushButton_SetBases->show();
    pushButton_MajBases->hide();           // CAR LA MISE A JOUR EST OK DONC NE PAS MONTRER LE BOUTON
    pushButton_LancerDrTux->show();
    pushButton_compilation->show();
    comboBox_Executables->show();
}

#define MAX_READ 0XFFFFFFF
//--------------------------------- pushButton_Load_SQL_clicked --------------------------------------------------------------------------------
void C_DlgMainDialog::pushButton_Load_SQL_clicked()
{    QString fname = QFileDialog::getOpenFileName(
                                                   G_pCApp->m_PathAppli+"SqlCreateTable",
                                                   tr("Ficher SQL (*.sql *.SQL)"),
                                                   this,
                                                   tr("open file dialog"),
                                                   tr("Choose a file to open") );
     if (fname.length()) setBase(fname);
}

//----------------------------------------- pushButton_LancerDrTux_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButton_LancerDrTux_clicked()
{
    QString path = CGestIni::Construct_Name_Exe("Manager",QFileInfo (qApp->argv()[0]).dirPath (true));
	QDir::setCurrent (path );
#ifdef  Q_WS_MAC
	//path += ".app/Contents/MacOS/Manager";
	//qDebug(path);
#endif
    // Affichage dans la Box

    QProcess*  proc = new QProcess( this );
    if (proc==0) return;
    proc->addArgument( path );
    if ( !proc->start() )
    {//text_Label_Status->setText(qstr.sprintf("OnGlossaireFileToEdit(): Echec de l'ouverture du process: QLightPad"));
    }
    delete proc;

}

//----------------------------------------- DataSempBaseInit--------------------------------------------------------------------
QSqlDatabase* C_DlgMainDialog::DataSempBaseInit()
{   if (m_DatasempBase) return m_DatasempBase;

    QString  driverName , baseName , userName , passWord, hostName, port;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "Connexion",
                                  &driverName, &baseName, &userName, &passWord, &hostName, &port) !=0
       )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "Datasemp";
        userName    = "root";
        passWord    = "";
        hostName    = "";
    }
    m_DatasempBase   = G_pCApp->BaseConnect(   driverName  ,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                               baseName    ,    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                               userName,            // = "root"
                                               passWord,        // = ""
                                               hostName,        // = "localhost"
                                               port,
                                               "Datasemp");


    return m_DatasempBase;
}
//----------------------------------------- pushButtonJustOneTable_clicked--------------------------------------------------------------------
void C_DlgMainDialog::pushButtonJustOneTable_clicked()
{ //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............
    // if (pushButton_SetDatasemp->text()== tr("Interrompre l'installation de Datasemp en cours")) return;
    if (m_BaseVidalRun  == INSTALL_VIDAL_RUN) return;
    QString  driverName , baseName , userName , passWord, hostName, port;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "Connexion",
                                  &driverName, &baseName, &userName, &passWord, &hostName, &port) !=0
        )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "Datasemp";
        userName    = "root";
        passWord    = "";
        hostName    = "";
    }
    if (m_DatasempBase)
    {   m_DatasempBase->close();
        QSqlDatabase::removeDatabase ("Datasemp");
        m_DatasempBase = 0;
    }
    int isThisBaseExist;
    if (m_DatasempBase==0)
        m_DatasempBase = G_pCApp->Datasemp_CreateBases( driverName  ,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                                        baseName,            // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                                        userName,            // = "root"
                                                        passWord,            // = ""
                                                        hostName,            // = "localhost"
                                                        port,
                                                        textEdit_Message,
                                                        &isThisBaseExist
                                                        );
    if (m_DatasempBase == 0 ||  (m_DatasempBase->isOpen()==FALSE && m_DatasempBase->open()==FALSE) )
    {textEdit_Message->append( tr (" === La base Datasemp : ") + driverName + "." + baseName + tr(" n'existe pas ou n'a pu être créée ou ouverte ====\n") );
        textEdit_Message->scrollToBottom ();
        return;
    }


    //m_DatasempDir      = QFileDialog::getExistingDirectory ( QString::null, this, "OuvrirDlg", tr(" Localisez les données Datasemp") );
    m_dtDeb            = QDateTime::currentDateTime();
    QString file_name  = QFileDialog::getOpenFileName(m_DatasempDir  + "tables" + QDir::separator() + "data" + QDir::separator(),
                                                      "Base SQL(*.sql *.SQL)", this, "open file dialog", tr( "Choix d'une table SQL à générer" ) );
    if (file_name.length()==0)                            return;

    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
    QDateTime dtDeb = QDateTime::currentDateTime();
    QString toFind  = QString(" ") + QDir::separator() + "tables";
    toFind          = toFind.mid(1);
    int pos         = file_name.find(toFind);
    m_DatasempDir   = file_name.left(pos+1);
    if (m_DatasempDir.length()==0)                        return;
    //t5c_s.sql
    if ( !QFile::exists(file_name ) )                     return;
    QFile file(file_name );
    if ( !file.open( IO_ReadOnly ) )                      return;
    QFileInfo fileInfo(file_name);
    QString table = fileInfo.fileName().left(1) + fileInfo.fileName().mid(1,2).upper();
    if (G_pCApp->CreationTables(m_DatasempBase, m_DatasempDir + "tables" + QDir::separator() + "struct" + QDir::separator() + fileInfo.fileName().left(3) + "_s.sql")!=0)
    {textEdit_Message->append( tr ("  Création de la table ") + driverName + "." + baseName + "." + table ) ;
        textEdit_Message->scrollToBottom ();
        int nb = G_pCApp->ExecSQL_File(m_DatasempBase, m_DatasempDir + "tables" + QDir::separator() + "data" + QDir::separator() + fileInfo.fileName().left(3) + "_d.sql");
        if (nb>0)
        {textEdit_Message->append( tr( " Insertion de " ) + QString::number(nb) + tr( " enregistrements." ));
            textEdit_Message->scrollToBottom ();
        }
        else
        {textEdit_Message->append( tr( " échec de l'insertion des enregistrements de ") + m_DatasempDir + "tables" + QDir::separator() + "data" + QDir::separator() + fileInfo.fileName().left(3) + "_d.sql");
            textEdit_Message->scrollToBottom ();
        }
    }
    else
    {textEdit_Message->append( tr( " échec de la création de la table ") + m_DatasempDir + "tables" + QDir::separator() + "struct" + QDir::separator() + fileInfo.fileName().left(3) + "_s.sql") ;
        textEdit_Message->scrollToBottom ();
    }

    QDateTime dtEnd= QDateTime::currentDateTime();
    long    s = dtDeb.secsTo (dtEnd);
    long    h = s/3600; s -=  h*3600;
    long    m = s/60;   s -=  m*60;
    QApplication::restoreOverrideCursor();
    textEdit_Message->append( tr( " === Installation de la table " ) + table + tr( "   terminée en " ) +
                              QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s ===\n");
    textEdit_Message->scrollToBottom ();

    textLabel_Progress->setText("" );
    progressBar_Load->setProgress(0);
}

//----------------------------------------- pushButton_SetDatasemp_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButton_SetDatasemp_clicked()
{//[DatasempTest]
    // Connexion      = QMYSQL3 , DatasempTest , Medintux , ,localhost

    if (m_BaseVidalRun  == INSTALL_VIDAL_RUN)
        //if (pushButton_SetDatasemp->text()== tr("Interrompre l'installation de Datasemp en cours"))
    {   StopCUpdateThread();
        pushButton_SetDatasemp->setText( tr( "&Installer les Bases Vidal Datasemp" ) );
        pushButton_IndexerDatasemp->show();
        m_BaseVidalRun  = INSTALL_VIDAL_NOTRUN;
        return;
    }


    if (m_DatasempBase)
    {   m_DatasempBase->close();
        QSqlDatabase::removeDatabase ("Datasemp");
        m_DatasempBase = 0;
    }

    //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............
    QString  driverName , baseName , userName , passWord, hostName, port;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "Connexion",
                                  &driverName, &baseName, &userName, &passWord, &hostName, &port) != QString::null
        )  // zero = pas d'erreur
    {   driverName  = "QMYSQL3";
        baseName    = "Datasemp2";
        userName    = "root";
        passWord    = "";
        hostName    = "";
        port        = "3306";
    }
    int isThisBaseExist;
    m_DatasempBase = G_pCApp->Datasemp_CreateBases( driverName  ,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                                    baseName,            // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                                    userName,            // = "root"
                                                    passWord,            // = ""
                                                    hostName,            // = "localhost"
                                                    port,
                                                    textEdit_Message,
                                                    &isThisBaseExist
                                                    );
    if (m_DatasempBase != 0 )
    {int ok = 1;
        if (isThisBaseExist)
        {
            QString numEdition, packBCArrete, packIRArrete;
            QString ods = G_pCApp->Datasemp_GetBaseVersion(m_DatasempBase , &numEdition, &packBCArrete, &packIRArrete);
            ok =  QMessageBox::warning( this,   tr( "Installation du Vidal Datasemp"),
                                        tr( "Il semble qu'une version du <b><u>Vidal Datasemp</b></u> :")+  //S041115
                                        tr("<br>    \r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° édition     N° : <b>")  + numEdition            +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Arrêté Pack BC : <b>")  + packBCArrete /*.mid(4,2) + "-" + packBCArrete.mid(2,2) + "-20" + packBCArrete.mid(0,2) */ +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Arrêté Pack IR : <b>")  + packIRArrete /*.mid(4,2) + "-" + packIRArrete.mid(2,2) + "-20" + packIRArrete.mid(0,2) */ +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Version : <b>")         + ods.mid(6,2)          + "." + ods.mid(7,1)          +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° N° de Livraison : <b>") + ods.mid(9)            +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Livraison ODS : <b>")   + ods                   +
                                        tr("</b><br>\r\n est déjà installée au niveau de la base <b>") + driverName  + "." + baseName +
                                        tr("</b><br>\r\nContinuer l'installation tout de même ?\n "
                                           "Ou installer juste les éventuelles mises à jour ?\n"),
                                        tr( "Tout annu&ler" ),
                                        tr( "Tout &Installer" ),
                                        tr( "Installer les &mises à jour" ),
                                        0 ) ;
        }
        if (ok !=0) m_DatasempDir = QFileDialog::getExistingDirectory ( m_DatasempDir  , this, "OuvrirDlg", tr( " Recherche des données Datasemp" ) );
        //........................ relever noms des sous repertoires données SQL Vidal Datasemp .........................
        QString mainDir, dataDir, docsDir, structDir;
        if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "CD_Dir", &mainDir, &dataDir, &docsDir, &structDir) !=0 )  // zero = pas d'erreur
        {   mainDir      = "plain";
            dataDir      = "data/std89";
            docsDir      = "data/docs";
            structDir    = "struct/mysql";
        }

        //........................ corriger si version MAJUSCULE (CD) ou Minuscules (FTP) .........................
        verifieDatasempDir(""     , mainDir);
        verifieDatasempDir(mainDir, dataDir);
        verifieDatasempDir(mainDir, docsDir);
        verifieDatasempDir(mainDir, structDir);

        //........................ verifier directement dans le fichier SQL les numeros de version .........................
        if (ok==1)
        {   QString path = m_DatasempDir + "/" + mainDir + "/" + dataDir + "/TFB_D.SQL";
            QString numEdition, packBCArrete, packIRArrete;
            QString ods = G_pCApp->Datasemp_GetDiskVersion(path , &numEdition, &packBCArrete, &packIRArrete);
            ok =  QMessageBox::warning( this,   tr( "Installation du Vidal Datasemp"),
                                        tr( "La version du <b><u>Vidal Datasemp</b></u> :")+  //S041115
                                        tr("<br>    \r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° édition     N° : <b>")  + numEdition            +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Arrêté Pack BC : <b>")  + packBCArrete /*.mid(4,2) + "-" + packBCArrete.mid(2,2) + "-20" + packBCArrete.mid(0,2) */ +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Arrêté Pack IR : <b>")  + packIRArrete /*.mid(4,2) + "-" + packIRArrete.mid(2,2) + "-20" + packIRArrete.mid(0,2) */ +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Version : <b>")         + ods.mid(6,2)          + "." + ods.mid(7,1)          +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° N° de Livraison : <b>") + ods.mid(9)            +
                                        tr("</b><br>\r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ° Livraison ODS : <b>")   + ods                   +
                                        tr("</b><br>\r\n est celle que vous allez installer dans <b>") + driverName  + "." + baseName +
                                        tr("</b><br>\r\nContinuer l'installation tout de même ?\n "),
                                        tr( "Tout annu&ler" ),
                                        tr( "&Installer" ),
                                        0,
                                        0 ) ;
        }
        m_dtDeb                   = QDateTime::currentDateTime();


        if (ok==1) //......................................... installation de tout .....................................
        {

            m_pCUpdateThread  = new CUpdateThread;
            qDebug(m_DatasempDir);
            if (m_pCUpdateThread==0)  return;
            long nb_pas = m_pCUpdateThread->Init(m_DatasempBase, m_DatasempDir ,mainDir, dataDir, docsDir, structDir, textEdit_Message);
            if (nb_pas!=0)
            {
                progressBar_Load->setTotalSteps(nb_pas);
                m_pCUpdateThread->start();
                pushButton_SetDatasemp->setText(tr("&Interrompre l'installation de Datasemp en cours"));
                pushButton_IndexerDatasemp->hide();
                m_BaseVidalRun  = INSTALL_VIDAL_RUN;
                m_pQTimer= new QTimer(this);
                if (m_pQTimer)
                {   connect(m_pQTimer, SIGNAL(timeout()), this, SLOT(timerDisplayDone()) );
                    m_pQTimer->start(400, FALSE);
                }
            }
            //................. si echec initialisation thread ........................
            else
            {   toLog(m_pCUpdateThread->GetCurrentLog());
                m_pCUpdateThread->StopSetBases();
                m_pCUpdateThread->wait();
                delete m_pCUpdateThread;
                m_pCUpdateThread = 0;
            }
        }
        else if (ok==2) //......................................... installation que des mises à jour .....................................
        {QString baseODS = G_pCApp->Datasemp_GetBaseVersion( m_DatasempBase );
            QStringList list;
            int nb =  G_pCApp->Datasemp_Maj_GetMajList(m_DatasempDir , baseODS, list);    // extraire la liste des fichiers de mise à jour
            if (list.count()==0)
            {   toLog(tr ("  Aucun fichier de mise à jour trouvé dans ce répertoire !!!!: "));
                return;
            }
            ok = 2;                 //
            if (nb != (int)list.count())
            {QString list_ok, list_no;
                for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
                {QString name = (*it);
                    if (name[0]=='+')  list_ok += QString("<br>    \r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>") + name.mid(1) +"</b>";
                    else               list_no += QString("<br>    \r\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>") + name.mid(1) +"</b>";
                }
                ok =  QMessageBox::warning( this,  tr("Installation de Vidal Datasemp"),
                                            tr("Des fichiers de mise à jour ne correspondant pas")       +  //S041115
                                            tr("<br> à cette livraison de la base ont été trouvés : ")   + list_no +
                                            ((list_ok.length()) ? tr("<br> ceux valides étant :"):QString(""))     + list_ok +
                                            tr("</b><br>\r\n faut-il forcer l'installation des fichiers ne correspondant pas à cette livraison ?\n "
                                               "ou installer juste les éventuelles mises à jour valides ?\n"),
                                            tr("&Tout annuler"),
                                            tr("&Forcer la mise à jour"),
                                            tr("N'installer que les fichiers &valides"),
                                            0, 0 ) ;
                if (ok==0) return;
            }
            int mode = 0;
            if ( checkBox_STD_MAJ->isChecked()) mode |= CApp::MajFiles;
            if ( checkBox_SQL_MAJ->isChecked()) mode |= CApp::SqlFiles;

            // int mode = GetDatasempMode() &~ mode|CApp::Force; // recuperer mode de mise à jour (avec flag pour fichiers SQL)
            if      (ok==1)  G_pCApp->Datasemp_Maj_Update(m_DatasempBase , m_DatasempDir, baseODS , (CApp::UPDATE_MODE)(mode|CApp::Force),  textEdit_Message);
            else if (ok==2)  G_pCApp->Datasemp_Maj_Update(m_DatasempBase , m_DatasempDir, baseODS , (CApp::UPDATE_MODE) mode,               textEdit_Message);
        }
    }

}
//----------------------------------------- verifieDatasempDir--------------------------------------------------------------------
void C_DlgMainDialog::verifieDatasempDir(const QString &mainDir, QString &dir)
{   QString mainDirToInsert = mainDir; if (mainDirToInsert.length()) mainDirToInsert += "/";
    if      ( QFile::exists( m_DatasempDir + mainDirToInsert +  dir.lower() ) )      dir = dir.lower();
    else if ( QFile::exists( m_DatasempDir + mainDirToInsert +  dir.upper() ) )      dir = dir.upper();
    else  G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n ERREUR le répertoire : \n          \"")  + m_DatasempDir +  mainDirToInsert +  dir.upper() + tr("\" ou\n          \"")
                                       + m_DatasempDir +  mainDirToInsert +  dir.lower() + tr("\" n'a pas été trouvé.\n") );
}

//----------------------------------------- timerDisplayDone--------------------------------------------------------------------
void C_DlgMainDialog::timerDisplayDone()
{if (m_pCUpdateThread==0) return;
    if (m_pCUpdateThread->IsFini())
    {   QDateTime dtEnd= QDateTime::currentDateTime();
        long    s = m_dtDeb.secsTo (dtEnd);
        long    h = s/3600; s -=  h*3600;
        long    m = s/60;   s -=  m*60;
        StopCUpdateThread();
        //.......................... installation de l'update ..................................................................
        int mode = 0;
        if ( checkBox_STD_MAJ->isChecked()) mode |= CApp::MajFiles;
        if ( checkBox_SQL_MAJ->isChecked()) mode |= CApp::SqlFiles;
        QString baseODS = G_pCApp->Datasemp_GetBaseVersion( m_DatasempBase );
        G_pCApp->Datasemp_Maj_Update(m_DatasempBase , m_DatasempDir, baseODS  , (CApp::UPDATE_MODE) mode, textEdit_Message);
        toLog( tr (" === Installation de Datasemp terminée en : ") +
                   QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s ===\n");
        pushButton_SetDatasemp->setText(tr("Installer le &Vidal Datasemp..."));
        m_BaseVidalRun  = INSTALL_VIDAL_NOTRUN;
        textLabel_Progress->setText("" );
        progressBar_Load->setProgress(0);
        copyDatasempImages();
        SetButtonIndexVidal(1);
        return;
    }


    progressBar_Load->setProgress( m_pCUpdateThread->GetCurrentProgress());
    textLabel_Progress->setText( m_pCUpdateThread->GetCurrentStatus() + QString::number( m_pCUpdateThread->GetCurrentLine()));
    toLog(m_pCUpdateThread->GetCurrentLog());
}

//----------------------------------------- GetDatasempMode--------------------------------------------------------------------
int C_DlgMainDialog::GetDatasempMode()
{    // Mise a jour SQL     = non
    // Mise a jour STD     = Forcer
    QString isMajSQL(""), isMajSTD("");
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "Mise a jour SQL", &isMajSQL);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "Mise a jour STD", &isMajSTD);
    isMajSQL = isMajSQL.lower();
    isMajSTD = isMajSTD.lower();
    int mode = 0;
    if (isMajSQL.find("oui") != -1) mode |= CApp::SqlFiles;
    if (isMajSTD.find("oui") != -1) mode |= (CApp::Remove|CApp::Insert|CApp::Update);
    return mode;
}

//----------------------------------------- checkBox_STD_MAJ_stateChanged--------------------------------------------------------------------
void C_DlgMainDialog::checkBox_STD_MAJ_stateChanged( int )
{if (checkBox_STD_MAJ->isChecked())
    {CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Datasemp", "Mise a jour STD", "oui");
    }
    else
    {CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Datasemp", "Mise a jour STD", "non");
    }
    CGestIni::Param_UpdateToDisk(G_pCApp->m_ParamPath, G_pCApp->m_ParamData);
}


//----------------------------------------- checkBox_SQL_MAJ_stateChanged--------------------------------------------------------------------
void C_DlgMainDialog::checkBox_SQL_MAJ_stateChanged( int )
{if (checkBox_SQL_MAJ->isChecked())
    {CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Datasemp", "Mise a jour SQL", "oui");
    }
    else
    {CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Datasemp", "Mise a jour SQL", "non");
    }
    CGestIni::Param_UpdateToDisk(G_pCApp->m_ParamPath, G_pCApp->m_ParamData);
}

//----------------------------------------- StopCUpdateThread--------------------------------------------------------------------
void C_DlgMainDialog::StopCUpdateThread()
{if (m_pCUpdateThread==0) return;
    if (m_pQTimer)
    { m_pQTimer->stop();
        delete m_pQTimer;
        m_pQTimer = 0;
    }
    m_pCUpdateThread->StopSetBases();
    m_pCUpdateThread->wait();
    delete m_pCUpdateThread;
    m_pCUpdateThread = 0;
}

//----------------------------------------- pushButtonSwitchGetDatasemp_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButtonSwitchGetDatasemp_clicked()
{   toLog(tr("=========================================== Activation d'une base ===================================================="));
    QString path = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.ini");
    // Affichage dans la Box
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================= Lecture Base Get ou Datasemp \n ") +  path );

    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("Ce fichier n'existe pas"));
        return ;
    }
    QString param;
    CGestIni::Param_UpdateFromDisk(path, param);
    if (m_BaseToInstallFlag == INSTALL_BASE_GET)
    {CGestIni::Param_WriteParam(&param, "Connexion" , "InUseBase" ,  "Parametres Get_Base" ,  "MedicaBase");
        pushButtonSwitchGetDatasemp->setText(tr("Activer la &base Datasemp"));
        m_BaseToInstallFlag  = INSTALL_BASE_DATASEMP;
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n ====> Activation de la base Get"));
    }
    else
    {CGestIni::Param_WriteParam(&param, "Connexion" , "InUseBase" ,  "Parametres MedicaBase" ,  "noMedicaBase");
        pushButtonSwitchGetDatasemp->setText(tr("Activer la &base Get"));
        m_BaseToInstallFlag  = INSTALL_BASE_GET;
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n ====> Activation de la base Vidal Datasemp"));
    }
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
}

//----------------------------------------- pushButtonConnexionAppl_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButtonConnexionAppl_clicked()
{
    if (pushButtonConnexionAppl->text().left(11)==tr("Interrompre"))
    {//KillTestBasesProc();
        m_testBaseProcRun = FALSE;
        return;
    }
    QString userName   = lineEdit_UserName->text();
    QString passWord   = lineEdit_PassWord->text();
    QString hostName   = lineEdit_Hostname->text();
    QString port       = lineEdit_Sql_Port->text();
    QString userName_2 = lineEdit_UserName_2->text();
    QString passWord_2 = lineEdit_PassWord_2->text();
    QString hostName_2 = lineEdit_Hostname_2->text();
    QString port_2     = lineEdit_Sql_Port_2->text();
    QString userName_3 = lineEdit_UserName_3->text();
    QString passWord_3 = lineEdit_PassWord_3->text();
    QString hostName_3 = lineEdit_Hostname_3->text();
    QString port_3     = lineEdit_Sql_Port_3->text();
    QString driverName, baseName;
    QString param;
    QString path;

    //.........................lancement crypt avec parametres ...........................................
    QString pathsetbases = QFileInfo (qApp->argv()[0]).dirPath (true);
    fichiersinipmd(userName, passWord, hostName, pathsetbases);


    /*
    QString str =                   QString("QMYSQL3,")                          +
                                    lineEdit_UserName->text().stripWhiteSpace()  +","+
                                    lineEdit_PassWord->text().stripWhiteSpace()  +","+
                                    lineEdit_Hostname->text().stripWhiteSpace());
    comboBox_IP_Fav_activated( str ) ;
    */
    //.............................. DrTux.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    // Affichage dans la Box
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") + path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Master" ,    &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Master" ,    driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" ,driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Nomade" ,   &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Nomade" ,    driverName ,  baseName, userName_2 , passWord_2 , hostName_2, port_2);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Reverse" ,  &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Reverse" ,   driverName ,  baseName, userName_3 , passWord_3 , hostName_3, port_3);
    CGestIni::Param_ReadParam (param,  "Codage CIM10" , "Connexion" ,&driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Codage CIM10" , "Connexion" , driverName ,  baseName, userName , passWord , hostName);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
   }

    //drtux-ccamview.ini
    path = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"Ressources/ccam/ccamview.ini");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") + path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, "\nCe fichier n'existe pas");
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion"  , "Parametres" ,  &driverName   ,  &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion"  , "Parametres" ,  driverName    ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_ReadParam (param,  "Praticiens" , "Connexion"  ,  &driverName   ,  &baseName);
    CGestIni::Param_WriteParam(&param, "Praticiens" , "Connexion"  ,  driverName    ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
   }
    //.............................. MedicaBase.ini ..................................................................
    // [Connexion]
    //     Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root,  , localhost
    //     Parametres SrceBase   = QMYSQL3 , DatasempTest , root,  , localhost
    //     Parametres Get_Base   = QMYSQL3 , BaseGetTest, root,  , localhost
    //     InUseBase             = Parametres Get_Base , MedicaBase
    path = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.ini");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") + path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param, "Connexion" , "Parametres MedicaBase" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres MedicaBase" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_ReadParam (param, "Connexion" , "Parametres SrceBase"   , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres SrceBase"   ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_ReadParam (param, "Connexion" , "Parametres Get_Base"   , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres Get_Base"   ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
   }

    //.............................. ccamview.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("ccamview",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") + path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion"  , "Parametres" ,  &driverName  ,  &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion"  , "Parametres" ,  driverName   ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_ReadParam (param,  "Praticiens" , "Connexion"  ,  &driverName  ,  &baseName);
    CGestIni::Param_WriteParam(&param, "Praticiens" , "Connexion"  ,  driverName   ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
   }

    //.............................. personnes.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("personnes",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") + path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam ( param, "Connexion" , "Parametres" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
   }

    //.............................. gest_user.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("gest_user",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +  path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam ( param, "Connexion" , "Parametres" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
   }

    //.............................. Manager.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +  path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Master" ,     &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Master" ,     driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" , driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Nomade" ,    &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Nomade" ,     driverName ,  baseName, userName_2 , passWord_2 , hostName_2, port_2);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Reverse" ,   &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Reverse" ,    driverName ,  baseName, userName_3 , passWord_3 , hostName_3, port_3);
    CGestIni::Param_ReadParam (param,  "Codage CIM10" , "Connexion" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Codage CIM10" , "Connexion" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
   }

    //.............................. med_stat.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("med_stat",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +  path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Parametres" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
   }

    //..............................QLaboFTP.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("QLaboFTP",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +   path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Parametres" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
   }
    //..............................synoptux.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("synoptux",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +   path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Parametres" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
   }

//..............................comptabilite.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("comptabilite",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +   path + "==================================");
    if (  ! QFile::exists (path) ) {
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
    }
    else    {
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_ReadParam (param,  "Connexion" ,    "Parametres" , &driverName , &baseName);
    CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
    CGestIni::Param_UpdateToDisk(path, param);
    G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
   }

    //..............................qgetdatasemp ..................................................................
    if ( IsSesamVersionExist()  )
    {path = CGestIni::Construct_Name_File_Ini("qgetdatasemp",QFileInfo (qApp->argv()[0]).dirPath (true),"");
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +  path + "==================================");
        if (  ! QFile::exists (path) ) {
            G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
        }
        else    {
        CGestIni::Param_UpdateFromDisk(path, param);
        CGestIni::Param_ReadParam (  param,  "Connexion" ,     "Parametres" , &driverName , &baseName);
        CGestIni::Param_WriteParam( &param,  "Connexion" ,     "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
        CGestIni::Param_UpdateToDisk(path, param);
        G_pCApp->Datasemp_OutMessage(textEdit_Message,param);
       }

        path = CGestIni::Construct_Name_File_Ini("qgetdatasemp",QFileInfo (qApp->argv()[0]).dirPath (true),"Ressources/MedicaBase.ini");
        G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +  path + "==================================");
        if (  ! QFile::exists (path) ) {
            G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));
        }
        else    {
        CGestIni::Param_UpdateFromDisk(path, param);
        CGestIni::Param_ReadParam (param, "Connexion" , "Parametres MedicaBase" , &driverName , &baseName);
        CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres MedicaBase" ,  driverName ,  baseName, userName , passWord , hostName, port);
        CGestIni::Param_ReadParam (param, "Connexion" , "Parametres SrceBase"   , &driverName , &baseName);
        CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres SrceBase"   ,  driverName ,  baseName, userName , passWord , hostName, port);
        CGestIni::Param_ReadParam (param, "Connexion" , "Parametres Get_Base"   , &driverName , &baseName);
        CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres Get_Base"   ,  driverName ,  baseName, userName , passWord , hostName, port);
        CGestIni::Param_UpdateToDisk(path, param);
        G_pCApp->Datasemp_OutMessage(textEdit_Message, param);
       }
    }
    //............................. setBases.ini .......................................................................
    //  [DrTuxTest]
    //     Connexion      = QMYSQL3 , DrTuxTest , root , ,localhost

    //     [MedicaTuxTest]
    //       Connexion      = QMYSQL3 , MedicaTuxTest , root , ,localhost

    //     [BaseGetTest]
    //       Connexion      = QMYSQL3 , BaseGetTest , root , ,localhost

    //     [CIM10Test]
    //       Connexion      = QMYSQL3 , CIM10Test , root , ,localhost

    //     [CCAMTest]
    //       Connexion      = QMYSQL3 , CCAMTest , root , ,localhost

    //     [Datasemp]
    //       Connexion            = QMYSQL3 , DatasempTest , root , ,localhost

    //     [QLaboFTPTest]
    //       Connexion = QMYSQL3 , QLaboFTPTest , root ,  , localhost

    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") +  G_pCApp->m_ParamPath + "==================================");
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,     "DrTuxTest",     "Master",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "DrTuxTest",     "Master",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,     "DrTuxTest",     "Nomade",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "DrTuxTest",     "Nomade",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,     "DrTuxTest",     "Parametres",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "DrTuxTest",     "Parametres",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,   "MedicaTuxTest", "Connexion",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "MedicaTuxTest", "Connexion",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,   "CIM10Test",     "Connexion",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "CIM10Test",     "Connexion",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,   "CCAMTest",      "Connexion",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "CCAMTest",      "Connexion",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,   "Datasemp",      "Connexion",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "Datasemp",      "Connexion",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,   "qlaboftp",  "Connexion",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "qlaboftp",  "Connexion",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,   "BaseGetTest",   "Connexion",  &driverName, &baseName);
    CGestIni::Param_WriteParam( &G_pCApp->m_ParamData,  "BaseGetTest",   "Connexion",   driverName,  baseName,  userName,  passWord,  hostName, port);
    CGestIni::Param_UpdateToDisk(G_pCApp->m_ParamPath, G_pCApp->m_ParamData);
    G_pCApp->Datasemp_OutMessage(textEdit_Message, param);

    QString ret ="";
    tabWidget_Instal->setTabEnabled ( m_pTabQWidgetBases,  FALSE );
    tabWidget_Instal->setTabEnabled ( m_pTabQWidgetTherap, FALSE );

    pushButton_SetBases->hide();
    pushButton_MajBases->hide();
    comboBox_Bases->hide();

    //................... on teste toutes les bases : test, mysql, DrTuxTest.................................
    baseName = "test";
    pushButtonConnexionAppl->setText(tr("Interrompre le test de connexion à la base de données en cours ... "));
    ret = TestBases(driverName, baseName, userName,  passWord,  hostName, port);
    if (ret == "ABORTED")           {   abortedMessDisplay(baseName);      return; }
    if (ret !="OK")
       { //toLog ( tr("\n (==> erreur lors du test de l'accès à la base : "+baseName+")\n" + ret + "\n" ));
         baseName = "mysql";
         ret = TestBases(driverName, baseName, userName,  passWord,  hostName, port);
         if (ret == "ABORTED")      {   abortedMessDisplay(baseName);      return; }
         if (ret !="OK")
            {//toLog ( tr("\n (==> erreur lors du test de l'accès à la base : "+baseName+")\n" + ret + "\n" ));
             CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "DrTuxTest",     "Master",   &driverName,  &baseName,  &userName,  &passWord,  &hostName, &port);
             ret = TestBases(driverName, baseName, userName,  passWord,  hostName, port);
             if (ret == "ABORTED")  {   abortedMessDisplay(baseName);      return; }
            }
       }
    //................... on a teste toutes les bases .................................
    //                    baseName contient le nom de la base a laquelle il a ete possible de se connecter
    if (ret =="OK")
    {   toLog (tr("\n (==> Succès lors du test de l'accès à la base : " + baseName + ")\n" ));
        tabWidget_Instal->setTabEnabled ( m_pTabQWidgetBases,  TRUE );
        tabWidget_Instal->setTabEnabled ( m_pTabQWidgetTherap, TRUE );
        pushButton_SetBases->show();
        comboBox_Bases->show();
        pushButtonConnexionAppl->setText(tr("&Se connecter à la base de données"));
        SetButtonIndexVidal(0);
        int retm = setMajVersionButton();
        if (retm==2)  // bases nons installées
           {pushButton_MajBases->hide();
            pushButton_LancerDrTux->hide();
            pushButton_compilation->hide();
            comboBox_Executables->hide();
           }
        else
          {if ( pushButton_MajBases->isVisible() ) pushButton_MajBases_clicked();
          }
    }
    else
    {   //textEdit_Message->setText("");
        toLog ( tr("\n (==> erreur lors du test de l'accès à la base : "+baseName+")\n" + ret + "\n" ));
        toLog ( tr("\n (====> les bases de données ne sont pas installées, ou incorrectement installées) <=====\n") + m_MessageNoConnect );
        pushButtonConnexionAppl->setText(tr("&Se connecter à la base de données"));
    }
}
//----------------------------------------- abortedMessDisplay ---------------------------------------------
void C_DlgMainDialog::abortedMessDisplay(const QString &base)
{       textEdit_Message->setText("");
        pushButtonConnexionAppl->setText(tr("&Se connecter à la base de données"));
        toLog ( m_MessageNoConnect + tr("\n\n (==> test de l'accès à la base %1 interrompu par l'utilisateur)").arg(base));
}

//----------------------------------------- TestBases ---------------------------------------------
QString C_DlgMainDialog::TestBases(  const QString &driver, const QString &base, const QString &userName,  const QString &passWord,  const QString &hostName,  const QString &port)
{   QString path     = CGestIni::Construct_Name_Exe("mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true));
    QString exchFile = CGestIni::Construct_Name_File_Ini("mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true), "Ressources/Version.txt");
    CGestIni::Param_UpdateToDisk(exchFile , "début");

    QProcess*  m_TestBasesProc  = new QProcess( this );
    if (m_TestBasesProc==0) return "";
    m_TestBasesProc->addArgument( path );
    m_TestBasesProc->addArgument("Tester SQL Connexion");             // - 1 - Commande
    m_TestBasesProc->addArgument(driver);                             // - 2 - Driver
    m_TestBasesProc->addArgument(base);                               // - 3 - Base
    m_TestBasesProc->addArgument(userName);                           // - 4 - userName
    if (passWord.length()<=0) m_TestBasesProc->addArgument("_=OO=_"); // - 5 - passWord
    else                      m_TestBasesProc->addArgument(passWord); // - 5 - passWord
    m_TestBasesProc->addArgument(hostName);                           // - 6 - hostName
    m_TestBasesProc->addArgument(port);                               // - 6 - hostName
    m_testBaseProcRun = TRUE;
    if ( m_TestBasesProc->start())
    {   qApp->processEvents ();
        QTimer timer;               // pour etre sur qu'un evenement se produise
        timer.start(400, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
        while(m_TestBasesProc->isRunning () && m_testBaseProcRun==TRUE)
        {   QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
            putInFront();
        }
    }
    QString val, baseName;
    m_TestBasesProc->kill();
    delete m_TestBasesProc;
    m_TestBasesProc = 0;
    if (m_testBaseProcRun==FALSE)
       {val = "ABORTED";
       }
    else
       {CGestIni::Param_UpdateFromDisk(exchFile , val);
        if (val=="OK")
           {reinitLog(1);
            QString err;
            QStringList basesListToSet           = G_pCApp->GetBasesListToSet();
            QStringList basesListAlreadyPresents = GetListBasesInMySql(&err, basesListToSet);
            QStringList basesListNotFound;
            QStringList basesListFound;
            int i;
            for (i=0; i<(int)basesListToSet.size(); ++i)
                {baseName = basesListToSet[i];

                 if (basesListAlreadyPresents.findIndex (baseName) == -1)
                    { basesListNotFound.append(basesListToSet[i]);
                    }
                 else
                    { basesListFound.append(basesListToSet[i]);
                    }
                }
            if (basesListNotFound.size())
               {toLog (tr("Bases de données manquantes : %1").arg( QString::number( basesListNotFound.size() ) ));
                for (i=0; i<(int)basesListNotFound.size(); ++i) toLog(QString(".        <font color=#ff0000><b>'"+basesListNotFound[i]+"'</b></font>"));
               }
            toLog (tr("Bases de données trouvées : %1").arg( QString::number( basesListFound.size() ) ));
            for (i=0; i<(int)basesListFound.size(); ++i) toLog(QString(".        <font color=#ffae00><b>'"+basesListFound[i]+"'</b></font>"));
            tabWidget_Instal->setCurrentPage(0);
           }
        else
           {toLog (tr("\n (====> ECHEC de la connexion SQL : %1 <=====\n").arg(base) + val+"\n" );
           }
       }
    m_testBaseProcRun=FALSE;
    return val;
}

//------------------------------------------- pushButton_IndexerDatasemp_clicked -----------------------------------------
void C_DlgMainDialog::pushButton_IndexerDatasemp_clicked()
{

    QString path =  G_pCApp->m_PathAppli + "../../drtux/bin/";

    if (m_pCMedicaBase==0) m_pCMedicaBase = new CMedicaBase(path + "MedicaBase.ini", path +"MedicaBase.cfg");
    if (m_pCMedicaBase==0) return ;
    if (m_pCMedicaBase->m_DataBaseSrce->isOpen()==FALSE && m_pCMedicaBase->m_DataBaseSrce->open()==FALSE) return;

    m_IndexStatus = "";
    //................. si thread déjà lancé l'arreter et cassos .......................................
    if (m_pCIndexMakeThread)
       {   StopCIndexMakeThread();
           QString str = "0.00";             // on met un numéro d'index bidon pour qu'il ne soit pas à jour
           //SetDatasempIndexVersion(str);
           m_pCMedicaBase->Medica_SetDatasempVersion(str, CMedicaBase::IndexAndData);
           SetButtonIndexVidal(0);
           return;
       }
    //................. lancer le thread ...............................................................
    m_pCIndexMakeThread  = new CIndexMakeThread(m_pCMedicaBase, textEdit_Message);
    if (m_pCIndexMakeThread == 0) return;
    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
    //..................positionner l'interface ..........................................
    pushButton_SetDatasemp->hide();
    pushButtonJustOneTable->hide();
    m_pCIndexMakeThread->start();
    pushButton_IndexerDatasemp->setText( tr("Arrêter la &Reindexation du VidalDatasemp ...") );
    //................. lancer le timer de lecture de l'état d'avancement ..............................
    m_pCindexQTimer = new QTimer(this);
    if (m_pCindexQTimer)
       {   connect(m_pCindexQTimer, SIGNAL(timeout()), this, SLOT(IndexTimerDisplayDone()) );
           m_pCindexQTimer->start(600, FALSE);
       }
}

//----------------------------------------- IndexTimerDisplayDone --------------------------------------------------------------------
void C_DlgMainDialog::IndexTimerDisplayDone()
{   if (m_pCIndexMakeThread==0) return;
    if (m_IndexStatus.length() > 0 && !m_pCIndexMakeThread->IsRun())
       {StopCIndexMakeThread();
        return;
       }
    QString curStatus = m_pCIndexMakeThread->GetCurrentStatus();
    //................................ noter temps de départ ........................................;
    //  curStatus = ""                                                                   rien en cours
    //  curStatus = tr( "Index accélérateur en cours de création ... " )                 routine creation index
    //  curStatus = tr( "Table de spécifications produits en cours de création ... " )   routine creation specifications
    //  on reactualise la date de depart si status change car changement de routine et
    //  la durée et progress bar doit s'appliquer à la nouvelle routine
    if (m_IndexStatus != curStatus )
    {   m_IndexStatus = curStatus;
        m_DateDeb     = QDateTime::currentDateTime();
        if (m_IndexStatus.left(3)  == "-1-")
           {m_ProgressTotal = m_pCIndexMakeThread->GetTotalProgress();
           }
        if (m_IndexStatus.left(3)  == "-2-")
           {m_ProgressTotal = m_pCIndexMakeThread->GetTotalProgress();
           }
        progressBar_Load->setTotalSteps(m_ProgressTotal); // 103237
    }
    //................................ calcul du temps restant ......................................;
    double currentProgress    = m_pCIndexMakeThread->GetCurrentProgress();
    QDateTime dtEnd           = QDateTime::currentDateTime();  // date actuelle
    long    s                 = m_DateDeb.secsTo (dtEnd);      // temps écoulé depuis le debut
    double  tpsPrevuPourTout  = ((double)s*(double)m_ProgressTotal)/currentProgress;
    long    sr                = (long)tpsPrevuPourTout - s;
    long    h  = s/3600;  s  -=  h*3600;
    long    m  = s/60;    s  -=  m*60;
    long    hr = sr/3600; sr -=  hr*3600;
    long    mr = sr/60;   sr -=  mr*60;
    textLabel_Progress->setText(m_IndexStatus +
                                QString::number(currentProgress)  + tr(" Produits traités sur ") + QString::number(m_ProgressTotal)    +
                                tr(" Temps écoulé ")  + QString::number(h)  + "h" + QString::number(m)  + "mn" + QString::number(s)    + "s"    +
                                tr(" Temps restant ") + QString::number(hr) + "h" + QString::number(mr) + "mn" + QString::number(sr)   + "s"
                                );
    progressBar_Load->setProgress((long)currentProgress);
}

//----------------------------------------- StopCIndexMakeThread --------------------------------------------------------------------
void C_DlgMainDialog::StopCIndexMakeThread()
{if (m_pCIndexMakeThread==0) return;
    if (m_pCindexQTimer)
    {   m_pCindexQTimer->stop();
        delete m_pCindexQTimer;
        m_pCindexQTimer = 0;
    }
    m_pCIndexMakeThread->stop();
    m_pCIndexMakeThread->wait();
    delete m_pCIndexMakeThread;
    m_pCIndexMakeThread = 0;
    pushButton_IndexerDatasemp->setText( tr("&Reindexer le VidalDatasemp ...") );
    pushButton_SetDatasemp->show();
    pushButtonJustOneTable->show();
    QApplication::restoreOverrideCursor();
    progressBar_Load->setTotalSteps(0);
    textLabel_Progress->setText(tr("Reindexation du VidalDatasemp terminée"));
    //QString str = "";             // numero vide ==> ce sera celui en cours de datasemp qui sera placé
    //SetDatasempIndexVersion(str); // place dans l'index créé le N° de version afin de savoir si correspond avec Datasemp
    SetButtonIndexVidal(0);
}

//----------------------------------------- pushButton_MajBases_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButton_MajBases_clicked()
{
    QString path     = CGestIni::Construct_Name_Exe("mise_a_jour",QFileInfo (qApp->argv()[0]).dirPath (true));
    QString exchFile = CGestIni::Construct_Name_File_Ini("mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true), "Ressources/Version.txt");
    QProcess*  proc  = new QProcess( this );
    if (proc==0) return;
    proc->addArgument( path );
    proc->addArgument("Do Update");
    if ( proc->start() )
    {qApp->processEvents ();
        QTimer timer;               // pour etre sur qu'un evenement se produise
        timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
        while(proc->isRunning () )
        {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
        }
    }
    delete proc;
    QString val;
    CGestIni::Param_UpdateFromDisk(exchFile , val);
    if (val == tr("Erreur 00"))
       {
       }
    else if (setMajVersionButton()==1) SetButtonIndexVidal(0);
    QString ret = GetMAJVersion();
    if (ret[2] == '.') SetTitle(ret);
    else {G_pCApp->Datasemp_OutMessage(textEdit_Message, QString (tr("============ Erreur  : GetMAJVersion()==============\n")) + ret);
          SetTitle("?,??");
         }
}

//----------------------------------------- setMajVersionButton ---------------------------------------------
int C_DlgMainDialog::setMajVersionButton()
{
    pushButton_MajBases->hide();
    QString val = GetMAJVersion();
    if (val[2] != '.')
       {return 2;//val = "0.00";
       }
    QString version;
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "CONFIG", "VersBase", &version) !=0 )  // zero = pas d'erreur
       {version = "00.09.000";
       }
    pushButton_SetBases->setPaletteForegroundColor ( QColor("black") );


    //..................... Gros mystere  sous VC++ ...................................................
    //                      en mode compilàuniquement !!!
    //                      les deux textes : version et val contiennent la meme valeur àsavoir "1.20"
    //                      donc les conversions en flottant : fversion et fval devraient contenir
    //                      elles aussi 1.20   donc le test if (fversion > fval)  devrait etre FAUX.
    //                      et c'est bien ce qu'il se passe en mode DEBUG logique et rasurant.
    //                      mais pas en mode    RELEASE    !!! ou le test retourne VRAI !!! ben à alors.
    //
    //                      le plus fou est que si je rajoute les lignes d'affichage suivantes:
    //
    //                           qDebug(QString("\n version de 'mise_a_jour'   : ") + QString::number(fval));
    //                           qDebug(QString("\n version de 'set_bases.ini' : ") + QString::number(fversion));
    //
    //                      tout rentre dans l'ordre, du coup je les laisse (faut bien que à marche)!!!
    //                      cela sent l'option d'optimisation foireuse en mode RELEASE.
    //
    //float fversion =  version.toFloat();
    //float fval     =  val.toFloat() ;

    //qDebug(QString("\n version de 'mise_a_jour'   : ") + QString::number(fval));
    //qDebug(QString("\n version de 'set_bases.ini' : ") + QString::number(fversion));

    if (CMoteurBase::normaliseVersion(version) > CMoteurBase::normaliseVersion(val))
       {   pushButton_MajBases->show();
           if (val=="0.00.000") pushButton_SetBases->setPaletteForegroundColor ( QColor("red") );
           return 1;
       }
    else
       {   pushButton_MajBases->hide();
           if (val=="0.00.000") pushButton_SetBases->setPaletteForegroundColor ( QColor("blue") );
           return 0;
       }
}

//----------------------------------------- GetListBasesInMySql ------------------------------------------------
QStringList C_DlgMainDialog::GetListBasesInMySql(QString *err, const QStringList &baseListToSet)
{       //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............
    QString  driverName , baseName , user , passWord, hostName, port;
    QString result = "";
    QStringList basesAlreadyPresents;
    basesAlreadyPresents.clear();
    if (CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "DrTuxTest", "Master", &driverName, &baseName) !=0 )  // zero = pas d'erreur
       {   driverName  = "QMYSQL3";
           baseName    = "DrTuxTest";
       }
   user     = lineEdit_UserName->text().stripWhiteSpace();
   passWord = lineEdit_PassWord->text().stripWhiteSpace();
   hostName = lineEdit_Hostname->text().stripWhiteSpace();
   port     = lineEdit_Sql_Port->text().stripWhiteSpace();
   QSqlDatabase* mysql_base  = G_pCApp->GetMySQL_DataBase(user, passWord, hostName, port);       // base MySQL pour creer les bases
   if (mysql_base ==0)
      {if (err) *err = tr("GetListBasesInMySql() Échec de la connexion à la base 'mysql'.");
       else     toLog( tr("============== GetListBasesInMySql() Échec de la connexion à la base 'mysql' ============================================="));
       return 0;
      }
   if (!mysql_base->isOpen())
       { if (mysql_base->open()==FALSE)
            {if (err) *err =  tr("GetListBasesInMySql(), La base  'mysql' n'a pu être ouverte.") ;
             else     toLog ( tr("============== GetListBasesInMySql(), La base  'mysql' n'a pu être ouverte =============================================")) ;
             return 0;
            }
       }
    //.............. recherche des base de donnees a initialiser .......................
    //QStringList dataList;
    //dataList << "BaseGetTest" <<  "CCAMTest" << "CIM10Test" << "Comptabilite" << "DrTuxTest" << "MedicaTuxTest" << "qlaboftp";
    //dataList  = G_pCApp->GetBasesListToSet();
    for (int i=0; i < (int) baseListToSet.size(); ++i)
        {  if (G_pCApp->tryToConnectToDB(baseListToSet[i],mysql_base))  basesAlreadyPresents.append (baseListToSet[i]);
        }

     /*
    QSqlQuery query(QString::null, mysql_base);
    query.exec(QString("SHOW DATABASES"));
    if (query.isActive())
       {//QString result = query.value(0).toString();
        while (query.next())
              {QString result = query.value(0).toString();
               basesAlreadyPresents.append (result);                    // on a la liste des utilisateurs signataires
              }
       }
     */
    mysql_base->close();
    return basesAlreadyPresents;

}

//----------------------------------------- GetMAJVersion ------------------------------------------------
QString C_DlgMainDialog::GetMAJVersion()
{   QString path = CGestIni::Construct_Name_Exe("mise_a_jour",QFileInfo (qApp->argv()[0]).dirPath (true));
    QProcess  proc( this );
    proc.addArgument( path );
    proc.addArgument("Get Version");
    if ( proc.start() )
    {qApp->processEvents ();
        QTimer timer;               // pour etre sur qu'un evenement se produise
        timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
        while(proc.isRunning () )
        {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
        }
    }
    //............ recuperer infos de 'mise_a_jour' ....................
    QString version,val;
    QString exchFile = CGestIni::Construct_Name_File_Ini("mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true), "Ressources/Version.txt");
    CGestIni::Param_UpdateFromDisk(exchFile , val);
    if (val[2] !='.') G_pCApp->Datasemp_OutMessage(textEdit_Message, QString (tr("============ Erreur  : GetMAJVersion()==============\n")) + val);
    return val;
}

//----------------------------------------- SetButtonIndexVidal ------------------------------------------------
void C_DlgMainDialog::SetButtonIndexVidal(int /* mode */)
{   if (DataSempBaseInit())
    {   pushButton_IndexerDatasemp->show();
        pushButton_SetDatasemp->setPaletteForegroundColor ( QColor("black") );
        QString indexVers     = GetDatasempIndexVersion();
        QString datasempVers  = GetDatasempBaseVersion();
        if (indexVers != datasempVers)
        {pushButton_IndexerDatasemp->setPaletteForegroundColor ( QColor("red") );
            pushButton_IndexerDatasemp->setText(tr("&Reindexer le VidalDatasemp ..."));
        }
        else
        {pushButton_IndexerDatasemp->setPaletteForegroundColor ( QColor("black") );
            pushButton_IndexerDatasemp->setText(tr("&Index à jour (cliquez pour forcer)"));
        }
        toLog(tr ("Index Version : ") + indexVers + tr ("\r\nDatasemp Version : ")+datasempVers);
    }
    else
    {   pushButton_SetDatasemp->setPaletteForegroundColor ( QColor("red") );
        pushButton_IndexerDatasemp->hide();
    }
}

//----------------------------------------- GetDatasempIndexVersion ---------------------------------------------
QString C_DlgMainDialog::GetDatasempIndexVersion()
{

    QString path     = CGestIni::Construct_Name_Exe("mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true) );
    QProcess*  proc  = new QProcess( this );
    if (proc==0) return 0;
    proc->addArgument( path );
    proc->addArgument("Get Datasemp Version");
    if ( proc->start() )
    {qApp->processEvents ();
        QTimer timer;               // pour etre sur qu'un evenement se produise
        timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
        while(proc->isRunning () )
        {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
        }
    }
    delete proc;

    QString version,val;
    QString exchFile = CGestIni::Construct_Name_File_Ini("mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true), "Ressources/Version.txt");
    CGestIni::Param_UpdateFromDisk(exchFile , val);
    return val;
}

//----------------------------------------- SetDatasempIndexVersion ---------------------------------------------
void C_DlgMainDialog::SetDatasempIndexVersion(QString &version)
{
    if (version.length()<=0)
       {version     =   GetDatasempBaseVersion();
       }
    QString path = CGestIni::Construct_Name_Exe("mise_a_jour",QFileInfo (qApp->argv()[0]).dirPath (true));

    QProcess*  proc  = new QProcess( this );
    if (proc==0) return ;
    proc->addArgument( path );
    proc->addArgument("Set Datasemp Version");
    proc->addArgument(version);
    if ( proc->start() )
    {   qApp->processEvents ();
        QTimer timer;               // pour etre sur qu'un evenement se produise
        timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
        while(proc->isRunning () )
        {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
        }
    }
    delete proc;
}

//----------------------------------------- GetDatasempBaseVersion ---------------------------------------------
QString C_DlgMainDialog::GetDatasempBaseVersion()
{if (m_DatasempBase==0) return QString("0.00");
    QString numEdition,packBCArrete,packIRArrete;
    QString ods         = G_pCApp->Datasemp_GetBaseVersion(m_DatasempBase , &numEdition, &packBCArrete, &packIRArrete);
    if (ods.length()==0)   return QString("0.00");
    QString version     =   numEdition + ods.mid(6,2)+ "." + ods.mid(7,1);
    return version;
}



//----------------------------------------- pushButtonImagesCopy_clicked ---------------------------------------------
void C_DlgMainDialog::pushButtonImagesCopy_clicked()
{       //................. copier dans la destination ...........................................
    m_DatasempDir = QFileDialog::getExistingDirectory ( m_DatasempDir  , this, "OuvrirDlg", tr( " Recherche des données Datasemp" ) );
    copyDatasempImages();
}
//----------------------------------------- copyDatasempImages ---------------------------------------------
void C_DlgMainDialog::copyDatasempImages()
{       //................. repérer directory source ...........................................
    if (!m_DatasempDir.endsWith("/") ) m_DatasempDir += "/";
    QString images_SrcDir = m_DatasempDir+"images";
    if ( !QFile::exists( images_SrcDir  ) )
    {images_SrcDir = m_DatasempDir+"IMAGES";
        if ( !QFile::exists( images_SrcDir  ) )
        {G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nLe répertoire : ") + images_SrcDir + tr(" n'existe pas "));
            return;
        }
    }
    QString src_PrefixDir = images_SrcDir;

    //................. effacer la destination ................................................
    QString images_DstDir = CGestIni::Construct_Name_File_Ini("drtux", QFileInfo (qApp->argv()[0]).dirPath (true),  "Ressources/ImgVidalDgl") + "/";
    if ( QFile::exists( images_DstDir + "images" ) )
        images_DstDir = images_DstDir + "images";
    else
        images_DstDir = images_DstDir + "IMAGES";
    if ( QFile::exists( images_DstDir  ) ) eraseDirectory(images_DstDir);

    //................. copier dans la destination ...........................................
    //                  la norme vidal est que les noms des images des mononographies sont en majuscules : src="IMAGES/B00/C01/D35.GIF"
    images_DstDir = CGestIni::Construct_Name_File_Ini("drtux", QFileInfo (qApp->argv()[0]).dirPath (true),  "Ressources/ImgVidalDgl/IMAGES") ;
    copyDirectory(src_PrefixDir, images_SrcDir, images_DstDir);
}

//----------------------------------------- copyDirectory ---------------------------------------------
void C_DlgMainDialog::copyDirectory(QString &src_PrefixDir, QString dir_to_explore, QString images_DstDir)
{   int f = dir_to_explore.find(src_PrefixDir);
    QString dir_dest;
    if (f !=-1)
    {dir_dest = dir_to_explore.mid(f+src_PrefixDir.length()).upper();      // MAJUSCULER le répertoire
        dir_dest.prepend(images_DstDir);
    }
    //........................ explorer la source .......................................
    QDir cur_dir( dir_to_explore );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("----------- Création de ce répertoire en cours : ") + dir_dest + "-----------");
    cur_dir.mkdir(dir_dest);
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {copyDirectory(src_PrefixDir, dir_to_explore+"/" + fname, images_DstDir);
        }
        else if (fi->isFile())
        {QString dst_fname = fname.upper();                       // MAJUSCULER le fichier
            G_pCApp->Datasemp_OutMessage(textEdit_Message, QString (tr("Fichier en cours de copie : ")) + dir_dest + "/" + dst_fname);
            CHtmlTools::Copy_File(dir_to_explore+"/" + fname, dir_dest, &dst_fname);
        }
        ++it;
    }
}

//----------------------------------------- eraseDirectory ---------------------------------------------
void C_DlgMainDialog::eraseDirectory( QString dir_to_erase)
{   //........................ petit message .......................................
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("----------- Effacement de ce répertoire en cours : ") + dir_to_erase + "-----------");
    QDir cur_dir( dir_to_erase );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {eraseDirectory(dir_to_erase + "/" + fname);
        }
        else if (fi->isFile())
        {G_pCApp->Datasemp_OutMessage(textEdit_Message, QString (tr("Fichier en cours d'effacement : ")) + dir_to_erase + "/" + fname);
            cur_dir.remove(dir_to_erase + "/" + fname);
        }
        ++it;
    }
    cur_dir.rmdir(dir_to_erase );
}

//----------------------------------------- pushButtonMakeSQL_clicked ---------------------------------------------
// mysqldump --quote-name --compatible=mysql323 MedicaTuxTest IndexProd IndexDiet IndexVeterinaire IndexParapharmacie IndexAccessoire IndexDivers IndexHomeopathie > /home/ro/Dump_IndexMedica-0001-oo-0-oo-119A02.2.sql
// mysqldump --quote-name --compatible=mysql323 DatasempTest > /home/ro/Dump_Datasemp-0001-oo-0-oo-119A02.2.sql

//................................ regles de nommage des fichiers de mise à jour ................................
// Dl-1144774740-Dump_Datasemp-0001-oo-1234-oo-112B02.2.sql
// ^  ^          ^                     ^       ^_____________ Datasemp version
// |  |          |_____ Ident Patern   |___ nb progress
// |  |                 Le nom du fichier sur le FTP commence ici
// |  |________ DateTime from FTP upload
// |__________________ Le nom du fichier téléchargé commence ici (précédé de 'Dl-1144774740-')
//                                                                            ^
//                                                                            |________ DateTime from FTP upload
void C_DlgMainDialog::pushButtonMakeSQL_clicked()
{   //................... lancer process ......................................
    //    FILE *in  = stdin;
    //    FILE *out = stdout;
    QString dump_exe = "";
#ifdef Q_WS_X11
    dump_exe = "mysqldump";
#endif
    
#ifdef Q_WS_WIN
    dump_exe = "mysqldump.exe";
#endif
    
#ifdef  Q_WS_MAC
    dump_exe = "/usr/local/mysql/bin/mysqldump";
#endif
    QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
    QString dump_opt   = "--net_buffer_length=4096";
    if (QFile::exists(G_pCApp->m_PathAppli+"Dump_IndexMedica_options.opt"))
    {dump_opt = CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Dump_IndexMedica_options.opt").remove('\r').remove('\n');
    }
    else
    {CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"Dump_IndexMedica_options.opt", dump_opt);
    }
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("===============  Dump du Vidal Datasemp en cours...") + " =================");
    QString file_name  = QString("Dump_IndexMedica-0001-oo-0-oo-") + GetDatasempBaseVersion();
    QString main_path  = G_pCApp->m_PathAppli ;
    QDir::setCurrent ( main_path );
    //......... on cree le dump que si il n'existe pas ..............
    if ( ! QFile::exists(main_path+file_name+".sql") )
    { G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("==>       Ecriture en cours <b>%1<b/> options <font color=#23cbcd><b>%2<b/><font/>").arg(file_name + ".sql", dump_opt) );
        QString command    = QString("#!/bin/sh \n%1 -u%3 %2 "
                                     "MedicaTuxTest "
                                     "IndexProd "
                                     "IndexDiet "
                                     "IndexVeterinaire "
                                     "IndexParapharmacie "
                                     "IndexAccessoire "
                                     "IndexDivers "
                                     "IndexHomeopathie "
                                     "SpecProd "
                                     "ClassesATC "
                                     "FormesAD "
                                     "FormesPA "
                                     "FormesST "
                                     "FormesUP > %4").arg(dump_exe,dump_opt, lineEdit_UserName->text(), file_name + ".sql");
        
        CGestIni::Param_UpdateToDisk(main_path + "/makeSqlIndex.sh", command);
        QProcess proc(this);
        proc.addArgument("chmod");
        proc.addArgument("777");
        proc.addArgument("makeSqlIndex.sh");
        proc.start();
        while(proc.isRunning());
        QProcess proc2(this);
        proc2.addArgument("bash");
        proc2.addArgument("makeSqlIndex.sh");
        proc2.start();
        while(proc2.isRunning());
    }
    //............................... compresser le fichier ........................................
    QFileInfo qfi(G_pCApp->argv()[0]);
    QString pathDest = QDir::cleanDirPath (G_pCApp->m_PathAppli+"../../qgetdatasemp/bin/Ressources/"); //CGestIni::Construct_PathBin_Module("qgetdatasemp", qfi.dirPath (true));
    if ( ! pathDest.endsWith("/") ) pathDest += "/";
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("          Compression en cours : ") + file_name + ".zql");
    C_Zlib czlib;
    czlib.Compresse(file_name + ".sql", file_name, ".zql");   //   ".zip"   est ajouté
    //QFile::remove(file_name + ".sql");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("          Copie en cours vers : ") + pathDest + file_name + ".zql");
    CHtmlTools::Copy_File(G_pCApp->m_PathAppli+file_name+".zql", pathDest);
    //QFile::remove(file_name + ".zql");
    
    dump_opt   = "--quote-name --net_buffer_length=64000";
    if (QFile::exists(G_pCApp->m_PathAppli+"Dump_Datasemp_options.opt"))
    {dump_opt = CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli+"Dump_Datasemp_options.opt").remove('\r').remove('\n');
    }
    else
    {CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"Dump_Datasemp_options.opt", dump_opt);
    }
    
    file_name = QString("Dump_Datasemp-0001-oo-0-oo-")+ GetDatasempBaseVersion();
    //......... on cree le dump que si il n'existe pas ..............
    if ( ! QFile::exists(main_path+file_name+".sql") )
    { G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("==>       Ecriture en cours <b>%1<b/> options <font color=#23cbcd><b>%2<b/><font/>").arg(file_name + ".sql", dump_opt) );
        QString command = QString("#!/bin/sh \n%1 -u%3 %2 DatasempTest > %4").arg(dump_exe, dump_opt, lineEdit_UserName->text(), file_name + ".sql");
        
        CGestIni::Param_UpdateToDisk(main_path + "/makeSql.sh", command);
        QProcess proc3(this);
        proc3.addArgument("chmod");
        proc3.addArgument("777");
        proc3.addArgument("makeSql.sh");
        proc3.start();
        while(proc3.isRunning());
        QProcess proc4(this);
        proc4.addArgument("bash");
        proc4.addArgument("makeSql.sh");
        proc4.start();
        while(proc4.isRunning());
    }
    //............................... compresser le fichier ........................................
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("          Compression en cours : ") + file_name + ".zql");
    czlib.Compresse(file_name + ".sql", file_name, ".zql");   //   ".zip"   est ajouté
    //QFile::remove(file_name + ".sql");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("          Copie en cours vers : ")  + pathDest + file_name + ".zql");
    CHtmlTools::Copy_File(G_pCApp->m_PathAppli+file_name+".zql", pathDest+"Ressources");
    //QFile::remove(file_name + ".zql");
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("----------- Dump du Vidal Datasemp terminée ") + "-----------");
    //QFile::remove("makeSql.sh");
    //QFile::remove("makeSqlIndex.sh");
    QApplication::restoreOverrideCursor();
    
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("----------- Upload du Vidal Datasemp avec qgetdatasemp ") + "-----------");

    QString pathExe = QDir::cleanDirPath ( G_pCApp->m_PathAppli+"../../qgetdatasemp/bin/qgetdatasemp" ); //CGestIni::Construct_PathBin_Module("qgetdatasemp", qfi.dirPath (true));
#ifdef Q_WS_X11
    //pathExe = "mysqldump";
#endif
#ifdef Q_WS_WIN
    pathExe += ".exe";
#endif
#ifdef  Q_WS_MAC
    pathExe += ".app/Contents/MacOS/qgetdatasemp";
#endif

    QProcess proc5(this);
    proc5.addArgument( pathExe );                         // 0  Chemin de l'executable plugin àactionner
    proc5.addArgument( "UPLOAD_LAST_FILE" );              // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
    proc5.start();
}

//--------------------------------------- pushButton_LireVitale_clicked ----------------------------------
void C_DlgMainDialog::pushButton_LireVitale_clicked()
{    Slot_pushButton_setGalsINI_clicked();
     QString pluginScript  = "[Execute]\r\n";
             pluginScript += "pathPlugin="      + CGestIni::Construct_Name_Exe("QFseVitale", QFileInfo (qApp->argv()[0]).dirPath (true))    + "\r\n" +
                             "pathIni=****\r\n" +                //**** car indique d'utiliser le pathIni du plugin
                             "maskExch=Bonjour tout le monde\r\n"
                             "[Parametres]\r\n"
                             "m_Command       = READ_CONFIG\r\n"
                             "m_PathLog       = " +  G_pCApp->m_PathAppli + "/ExchangeVitale.txt\r\n" +
                             "m_CodePorteur   = 1234 \r\n" +
                             "m_DateConsult   = " + QDate::currentDate().toString("yyyyMMdd");
      QString data = PluginExe(this, pluginScript, CMoteurBase::endWait);
      toLog( tr("\n================================== Resultat test Sesam-Vitale =================================="));
      toLog( data) ;
      if ( data.length()<= 200 || data.find("Retour SIS_SIS_LireConfig :  Erreur N") != -1 || data.find("tout le monde") != -1 )
         {QMessageBox::information( this, tr("Le test a échoué"),
                                          tr("Le test a échoué : \nVeuillez essayer avec d'autres paramètres.\n"
                                             "Lisez le message de retour pour tenter\nde déterminer les raisons de l'échec." ));
          toLog( tr("<font color=#FF0000><b>Il semble que le test d'accès au lecteur Sesam-Vitale ait échoué</b></font>"));   //<font color=#0000FF><b>%2h%3mn%4s</b></font>
         }
      else
         {toLog( tr("<font color=#FF0000><b>Félicitations, votre lecteur de carte Sesam-Vitale semble fonctionner correctement</b></font>"));
         }
}

//------------------------------------------------------- PluginExe --------------------------------------------------
QString C_DlgMainDialog::PluginExe(     QObject         * pQObject,
                                        const char      * pluginScript,
                                        int             /*  waitFlag  = CMoteurBase::endWait */)
{
      QString     pathPlugin;  // Chemin de l'executable plugin àactionner sans l'extension .exe
      QString     pathIni;     // Chemin de l'executable plugin àactionner sans l'extension .exe
      QString     maskExch;    // texte du fichier d'exchange
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
                                              pathPlugin,                       // Chemin de l'executable plugin àactionner sans l'extension .exe
                                              G_pCApp->m_PathAppli,             // Chemin de l'executable appelant (DrTux)
                                              pathIni,                          // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                              maskExch,                         // texte du fichier d'exchange
                                              CMoteurBase::GUID_Create()+"-New",
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
//   0  Chemin de l'executable plugin àactionner
//   1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
//   2  Chemin de l'executable appelant
//   3  Chemin du fichier d'echange texte masque d'entrà dont l'executable devra modifier les valeurs de retour
//                        ce fichier est retournàmodifiàpar le plugin, ce fichier peut etre un masque html ou
//                        tout autre dàire en accord avec la syntaxe du plugin dà fois que ...
//                        si ce fichier est vide, o n'existe pas le plugin retournera son ràultat dans ce fichier
//                        d'àhange que l'appelant ràupera
//                        Le nom de ce fichier d'echange comporte : NomDuPlugin-GUID_Patient-
//   4  GUID du patient
//   5  PrimKey de l'observation en cours
//   6  PrimKey du terrain en cours
//   7  PrimKey de l'ordonnance en cours
//   8  Reservà
//   9  Reservà
//   10 Reservà
//   11 Reservà
//   12 Nb autres parametres d'appels specifiques au plugin
//   13 --> 13+Nb paramàres

QString C_DlgMainDialog::PluginExe(       QObject    *pQObject,
                                          const QString    &pathPlugin,  // Chemin de l'executable plugin àactionner sans l'extension .exe
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
 QString nameExch = "VitaleSetBase-"+ guid +".exc";

 QString      dst = "";
 if ( QDir::isRelativePath ( dst ) )  dst = QDir::cleanDirPath (G_pCApp->m_PathAppli+"../../tmp") ;
 dst +=  QDir::separator() + nameExch;
 QFile::remove (dst);
 QFile qfdst(dst);
 if (qfdst.open(IO_ReadWrite)==FALSE)
    {QMessageBox::critical (0, QObject::tr("PluginExe(): write pathExch"),
                               QObject::tr("The file\r\n")+ dst + QObject::tr("\r\ncan't be opened"),
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
 proc->addArgument( pathExe );                         // 0  Chemin de l'executable plugin àactionner
 proc->addArgument( pathIni );                         // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
 proc->addArgument( pathAppli + "set_bases"F_EXE ); // 2  Chemin de l'executable appelant
 proc->addArgument( dst );                             // 3  Chemin du fichier d'echange texte masque d'entrà dont l'executable devra modifier les valeurs de retour
 proc->addArgument(guid );                             // 4  GUID du patient
 proc->addArgument(dosPk);                             // 5  PrimKey de l'observation en cours
 proc->addArgument(terPk);                             // 6  PrimKey du terrain en cours
 proc->addArgument(ordPk);                             // 7  PrimKey de l'ordonnance en cours
 proc->addArgument(" ");                               // 8  reservà
 proc->addArgument(" ");                               // 9  reservà
 proc->addArgument(" ");                               // 10 reservà
 proc->addArgument(" ");                               // 11 reservà
 proc->addArgument(QString::number(param.count()));    // 12 Nb autres parametres d'appels specifiques au plugin
 for ( QStringList::Iterator it = param.begin(); it != param.end(); ++it )  // 13 --> 13+Nb paramàres
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
//----------------------------------------- Slot_pushButton_setGalsINI_clicked --------------------------------------------------------------------
void C_DlgMainDialog::Slot_pushButton_setGalsINI_clicked()
{   reinitLog();
    setGalsINI();
}
//----------------------------------------- setGalsINI --------------------------------------------------------------------
void C_DlgMainDialog::setGalsINI()
{
    QString path_appli      = G_pCApp->m_PathAppli; if (path_appli.endsWith("/"))  { path_appli = path_appli.left(path_appli.length()-1); }
    QString api_src_path    = path_appli + "/../../Sesam-API-Local/ApiLinux";
    QString cpp_dst_path    = path_appli + "/../../qfselib/src";
    QString mylib_src_path  = path_appli + "/../../qfselib/bin";
    QString system_dst_path = lineEdit_BinaryPath->text();           // "/usr/local"
    QString sesam_dst_Path  = system_dst_path + "/sesam";                // "/usr/local/sesam"
    //.................................. fichier galss.ini reparametrer le port COM.........................................................
    QString galss = "";
    CGestIni::Param_UpdateFromDisk(system_dst_path + "/galss/galss.ini" , galss);
    int pos  = -1;
    int end  = -1;
    pos  = galss.find("MedinTux le :"); if (pos == -1) galss.prepend(tr("; Fichier galss.ini sous Linux set by MedinTux le :\n")); pos  = galss.find("MedinTux le :");
    if (pos != -1)
       {pos += 13;  // passer 'MedinTux le :'
        end  = galss.find("\n", pos);
        if (end != -1) galss.replace(pos, end-pos, QDate::currentDate().toString("dd-MM-yyyy"));
       }
    pos  = galss.find("[CANAL1]");
    if (pos != -1)
       {pos += 7;
        pos  = galss.find("Index=",pos);
        if (pos != -1)
           {pos += 6;
            end  = pos; while(end<(int)galss.length() && galss[end] != '\r' && galss[end] != '\n') ++end;
            QString portCom  = comboBox_PortLecteur->currentText();
            if (portCom.length())
               {int pSepEnd  = portCom.find("=");
                int pSepDeb  = portCom.find("COM");
                if (pSepEnd != -1 && pSepDeb != -1)
                   {pSepDeb += 3;
                    galss.replace(pos, end-pos, QString::number(portCom.mid(pSepDeb,pSepEnd-pSepDeb).toInt()));
                   }
               }
           }
       }
    CGestIni::Param_UpdateToDisk(system_dst_path + "/galss/galss.ini" , galss);
    toLog(  tr ("----------- Positionnement de : ") + system_dst_path + "/galss/galss.ini" + " sur le bon port COM -----------");
    toLog(  galss);

    //.................................. fichier io_comm.ini .........................................................
    // QString io_comm =tr(";Fichier d'équivalence des ports séries sous Linux set by MedinTux\r\n"   // pyxsistem plante si commentaire
    QString io_comm = QString("[MAP_DEVICE]\r\n") + comboBox_PortLecteur->currentText().stripWhiteSpace()+"\r\nCOM2=/dev/ttyS1\r\n";
    CGestIni::Param_UpdateToDisk(system_dst_path + "/galss/io_comm.ini" , io_comm);
    toLog(   tr ("----------- Mise en place du fichier : ") + system_dst_path + "/galss/io_comm.ini -----------");
    toLog(   tr ("\n") + io_comm);
}

//----------------------------------------- pushButton_InstallSesam_clicked --------------------------------------------------------------------
void C_DlgMainDialog::pushButton_InstallSesam_clicked()
{   reinitLog();
#ifdef Q_WS_X11

    QString path_appli      = G_pCApp->m_PathAppli; if (path_appli.endsWith("/"))  { path_appli = path_appli.left(path_appli.length()-1); }
    QString api_src_path    = path_appli + "/../../Sesam-API-Local/ApiLinux";
    QString cpp_dst_path    = path_appli + "/../../qfselib/src";
    QString mylib_src_path  = path_appli + "/../../qfselib/bin";
    QString system_dst_path = lineEdit_BinaryPath->text();           // "/usr/local"
    QString sesam_dst_Path  = system_dst_path + "/sesam";                // "/usr/local/sesam"

    toLog(tr("--------------- Journal d'installation du ") + QDate::currentDate().toString("dd-MM-yyyy") + tr(" à ")+ QTime::currentTime().toString("hh:mm:ss") + tr(" de l'API Sesam-Vitale  sur plateforme LINUX ") );
    toLog(tr("      Path d'installation des composants binaires :  ") + lineEdit_BinaryPath->text());
    toLog(tr("      Path d'installation des tables :  ")              + sesam_dst_Path);


    //................. copier les tables sesam ................................................
    eraseDirectory( sesam_dst_Path) ;      //effacer la destination
    copyTables_Api( api_src_path, sesam_dst_Path);
    //................. creer le repertoire galss ................................................
    if ( ! QFile::exists( system_dst_path + "/galss"  ) )
    {   QDir cur_dir(     system_dst_path + "/galss" );
        cur_dir.mkdir(    system_dst_path + "/galss");
        toLog( tr ("----------- Création de ce répertoire en cours : ") + system_dst_path + "/galss" + "-----------");
    }
    //................. creer le repertoire des lib MedinTux ................................................
    if ( ! QFile::exists( system_dst_path+"/galss/LibMedinTux"  ) )
    {   QDir cur_dir( system_dst_path + "/galss/LibMedinTux" );
        cur_dir.mkdir(system_dst_path + "/galss/LibMedinTux");
        toLog( tr ("----------- Création de ce répertoire en cours : ") + system_dst_path + "/galss/LibMedinTux" + "-----------");
    }
    //............... on va chercher les lib directement où elle sont crées ...............................
    QString dstFnameLib = "libqfselib.so.1";
    //                pour les copier dans /Sesam-API-Local/ApiLinux/LibMedinTux/     pour mettre à jour l'archive de depart
    toLog(  tr ("     Copie de : ")  + mylib_src_path + tr("/libqfselib.so.1.0.0  vers : ") + api_src_path + "/LibMedinTux/libqfselib.so.1.0.0" );
    toLog(  tr ("     Copie de : ")  + mylib_src_path + tr("/libqfselib.so.1.0.0  vers : ") + api_src_path + "/LibMedinTux/libqfselib.so.1" );
    CHtmlTools::Copy_File(mylib_src_path + "/libqfselib.so.1.0.0", api_src_path + "/LibMedinTux");                // pas de param 3 car nom idem que param 1
    CHtmlTools::Copy_File(mylib_src_path + "/libqfselib.so.1.0.0", api_src_path + "/LibMedinTux", &dstFnameLib);  // on pourrait faire un lien symbolique

    //                pour les copier dans /usr/local/galss/LibMedinTux/             pour mettre à jour l'archive de destination
    toLog(  tr ("     Copie de : ")  + mylib_src_path + tr("/libqfselib.so.1.0.0  vers : ") + system_dst_path + "/galss/LibMedinTux/libqfselib.so.1.0.0" );
    toLog(  tr ("     Copie de : ")  + mylib_src_path + tr("/libqfselib.so.1.0.0  vers : ") + system_dst_path + "/galss/LibMedinTux/libqfselib.so.1" );
    CHtmlTools::Copy_File(mylib_src_path + "/libqfselib.so.1.0.0", system_dst_path + "/galss/LibMedinTux");                // pas de param 3 car nom idem que param 1
    CHtmlTools::Copy_File(mylib_src_path + "/libqfselib.so.1.0.0", system_dst_path + "/galss/LibMedinTux", &dstFnameLib);  // on pourrait faire un lien symbolique
    //                pour disposer des libs libstdc++ avec lesquelles l'api a été compilée
    //                (lgconfig sur /local/galss/LibMedinTux devrait suffire)
    //                afin de ne pas polluer l'install du systeme hote on ne les recopie pas dans /usr/lib

    //dstFnameLib = "libstdc++-libc6.1-1.so.2";
    //toLog(  tr ("     Copie de : ")  + api_src_path + tr("/LibMedinTux/libstdc++-3-libc6.2-2-2.10.0.so  vers : ") +  "/usr/lib/libstdc++-3-libc6.2-2-2.10.0.so" );
    //CHtmlTools::Copy_File(api_src_path + "/LibMedinTux/libstdc++-3-libc6.2-2-2.10.0.so", "/usr/lib");                 // pas de param 3 car nom idem que param 1
    //CHtmlTools::Copy_File(api_src_path + "/LibMedinTux/libstdc++-3-libc6.2-2-2.10.0.so", "/usr/lib", &dstFnameLib);   // on pourrait faire un lien symbolique
    toLog(  tr ("     Copie de : ")  + api_src_path + tr("/LibMedinTux/  vers : ") +  "/usr/lib/" );                    // faut y aller bourinas
    CHtmlTools::Copy_Dir(api_src_path + "/LibMedinTux",  "/usr/lib");
    //................. creer le repertoire galsssrv ................................................
    if ( ! QFile::exists( system_dst_path+"/galsssrv"  ) )
    {   QDir cur_dir(     system_dst_path + "/galsssrv" );
        cur_dir.mkdir(    system_dst_path + "/galsssrv");
        toLog(  tr ("----------- Création de ce répertoire en cours : ") + system_dst_path + "/galsssrv" + "-----------");
    }
    //................. creer le repertoire adm ................................................
    if ( ! QFile::exists( sesam_dst_Path + "/adm"  ) )
    {   QDir cur_dir(     sesam_dst_Path + "/adm" );
        cur_dir.mkdir(    sesam_dst_Path + "/adm");
        toLog(  tr ("----------- Création de ce répertoire en cours : ") + sesam_dst_Path + "/adm" + "-----------");
    }
    //................. creer le repertoire etc ................................................
    if ( ! QFile::exists( sesam_dst_Path + "/etc"  ) )
    {   QDir cur_dir(     sesam_dst_Path + "/etc" );
        cur_dir.mkdir(    sesam_dst_Path + "/etc");
        toLog(  tr ("----------- Création de ce répertoire en cours : ") + sesam_dst_Path + "/etc" + "-----------");
    }
    //................. creer le repertoire lib ................................................
    if ( ! QFile::exists( sesam_dst_Path + "/lib"  ) )
    {   QDir cur_dir(     sesam_dst_Path + "/lib" );
        cur_dir.mkdir(    sesam_dst_Path + "/lib");
        toLog(  tr ("----------- Création de ce répertoire en cours : ") + sesam_dst_Path + "/lib" + "-----------");
    }

    copyTables_Api( path_appli + "/../../Sesam-API-Local/adm", sesam_dst_Path + "/adm");
    copyLinux_Api(  api_src_path,  system_dst_path + "/galss",  cpp_dst_path, sesam_dst_Path);
    //.................................. fichier galss.ini reparametrer le port COM.........................................................
    setGalsINI();
    /*
    QString galss = "";
    CGestIni::Param_UpdateFromDisk(system_dst_path + "/galss/galss.ini" , galss);
    int pos  = -1;
    int end  = -1;
    pos  = galss.find("MedinTux le :"); if (pos == -1) galss.prepend(tr("; Fichier galss.ini sous Linux set by MedinTux le :\n")); pos  = galss.find("MedinTux le :");
    if (pos != -1)
       {pos += 13;  // passer 'MedinTux le :'
        end  = galss.find("\n", pos);
        if (end != -1) galss.replace(pos, end-pos, QDate::currentDate().toString("dd-MM-yyyy"));
       }
    pos  = galss.find("[CANAL1]");
    if (pos != -1)
       {pos += 7;
        pos  = galss.find("Index=",pos);
        if (pos != -1)
           {pos += 6;
            end  = pos; while(end<galss.length() && galss[end] != '\r' && galss[end] != '\n') ++end;
            QString portCom  = comboBox_PortLecteur->currentText();
            if (portCom.length())
               {int pSepEnd  = portCom.find("=");
                int pSepDeb  = portCom.find("COM");
                if (pSepEnd != -1 && pSepDeb != -1)
                   {pSepDeb += 3;
                    galss.replace(pos, end-pos, QString::number(portCom.mid(pSepDeb,pSepEnd-pSepDeb).toInt()));
                   }
               }
           }
       }
    CGestIni::Param_UpdateToDisk(system_dst_path + "/galss/galss.ini" , galss);
    toLog(  tr ("----------- Positionnement de : ") + system_dst_path + "/galss/galss.ini" + " sur le bon port COM -----------");
    toLog(  galss);

    //.................................. fichier io_comm.ini .........................................................
    // QString io_comm =tr(";Fichier d'équivalence des ports séries sous Linux set by MedinTux\r\n"   // pyxsistem plante si commentaire
    QString io_comm = QString("[MAP_DEVICE]\r\n") + comboBox_PortLecteur->currentText().stripWhiteSpace()+"\r\nCOM2=/dev/ttyS1\r\n";
    CGestIni::Param_UpdateToDisk(system_dst_path + "/galss/io_comm.ini" , io_comm);
    toLog(   tr ("----------- Mise en place du fichier : ") + system_dst_path + "/galss/io_comm.ini -----------");
    toLog(   tr ("\n") + io_comm);
    */
    //................................ modifier le fichier de ld.so.conf ............................................
    QString etc;
    CGestIni::Param_UpdateFromDisk("/etc/ld.so.conf" , etc);
    if (etc.length()==0)
       {toLog(   tr("\n    Erreur installation API Sesam :  /etc/ld.so.conf   non trouvé " ));
       }
    else
       {
        if (etc.find(system_dst_path + "/galss")==-1)
           {   etc += "\n" + system_dst_path + "/galss";
               toLog(   system_dst_path + "/galss" +tr("   rajouté /etc/ld.so.conf" ));
           }
        if (etc.find(system_dst_path + "/galss/LibMedinTux")==-1)
           {   etc += "\n" + system_dst_path + "/galss/LibMedinTux";
               toLog(   system_dst_path + "/galss/LibMedinTux" +tr("   rajouté /etc/ld.so.conf" ));
           }
        if (etc.find(sesam_dst_Path + "/lib")==-1)
           {   etc += "\n" + sesam_dst_Path + "/lib";
               toLog(   sesam_dst_Path + "/lib" +tr("   rajouté à /etc/ld.so.conf" ));
           }

        toLog(   tr ("----------- Mise en place du fichier : /etc/ld.so.conf  -----------"));
        toLog(   etc + "\r");
        CGestIni::Param_UpdateToDisk("/etc/ld.so.conf" , etc);
       }
    //................................ modifier les droits de l'API .................................................
    QDir::setCurrent ( path_appli );
    QString script   = "#! /bin/sh\n"
                       "echo \".\"\n"
                       "echo \" ... Modifier droits de l'API Sesam-Vitale .....\"\n";
            script  += "chmod -R 777 " + system_dst_path + "/sesam\n";
            script  += "chmod -R 777 " + system_dst_path + "/galss\n";
            script  += "chmod -R 777 " + system_dst_path + "/galsssrv\n";
            script  += "ln -s /usr/lib/libssl.so.0.9.8 /usr/lib/libssl.so.4\n";
            //# Debian : ln -s /usr/lib/libstdc++-3-libc6.2-2-2.10.0.so /usr/lib/libstdc++-libc6.1-1.so.2
            script  += "ln -s /usr/lib/libstdc++-3-libc6.2-2-2.10.0.so /usr/lib/libstdc++-libc6.1-1.so.2\n";  // Debian
            script  += "chmod 777 /dev/ttyS0\n";    // Autoriser le port COM
            script  += "chmod 777 /dev/ttyS1\n";
            script  += "ldconfig\n";

            //# Problèmes avec SELinux monté en "strict" (réalloc dans libjs.so...)
            //#chcon -t textrel_shlib_t /usr/local/sesam/lib/*

            //# Autoriser le port COM
            //chmod 777 /dev/ttyS0
            //script  += "chmod -R 777 " + system_path + "/LibMedinTux\n";
    toLog(   tr ("----------- Execution du script : ") + path_appli + "/PostInstall-SesamAPI.sh  -----------");
    toLog(   "\n" + script);
    CGestIni::Param_UpdateToDisk(path_appli + "/PostInstall-SesamAPI.sh" , script);

    //......................... changer les droits du script ........................................................
    QProcess proc(this);
    proc.addArgument("chmod");
    proc.addArgument("777");
    proc.addArgument("PostInstall-SesamAPI.sh");
    proc.start();
    while(proc.isRunning());
    //......................... execution du script lui meme........................................................
    QProcess proc2(this);
    proc2.addArgument("bash");
    proc2.addArgument("PostInstall-SesamAPI.sh");
    proc2.start();
    while(proc2.isRunning());
    toLog(   tr("--------------- Fin du journal d'installation du ") + QDate::currentDate().toString("dd-MM-yyyy") + tr(" à ")+ QTime::currentTime().toString("hh:mm:ss") + tr(" de l'API Sesam-Vitale  sur plateforme LINUX ") );

    CGestIni::Param_UpdateToDisk(path_appli + "/Journal-Installation-SesamVitale.txt" , textEdit_Message->text());
#endif

#ifdef Q_OS_MACX
#endif

#ifdef Q_WS_WIN
    QString path        = G_pCApp->m_PathAppli; if (path.endsWith("/"))  { path = path.left(path.length()-1); }
    QString api_path    = path + "/../../Sesam-API-Local/ApiWin";
    QString cpp_path    = path + "/../../qfselib/src";
    QString sesamPath   = lineEdit_SesamPath->text() + "\\sesam";         // "C:"
    QString system_path = lineEdit_BinaryPath->text();                    // "C:\\WINDOWS"

    sesamPath.replace("\\\\","\\");                                                                  // des fois que il soit plac�par l'utilisateur "C:\" et rajout� "\sesam"
    if (system_path.endsWith("\\")) system_path.truncate(system_path.length()-1);       // des fois que il soit plac�par l'utilisateur un \ en fin de chemin"
    textEdit_Message->setText("");
    toLog(   tr("--------------- Journal d'installation de l'API Sesam-Vitale ---- en date du : ") + QDate::currentDate().toString("dd-MM-yyyy"));
    toLog(   tr("      Path d'installation des composants binaires :  ") + system_path );
    toLog(   tr("      Path d'installation des tables :  ")              + sesamPath   );
    //................. copier les dll et .lib et .ini et .h et .exe ............................
    copyWindw_Api(api_path,  system_path, cpp_path, sesamPath);
    //................. copier les tables sesam ................................................
    eraseDirectory(sesamPath);      //effacer la destination
    copyTables_Api( path + "/../../Sesam-API-Local/ApiWin/", sesamPath);
    //................. creer le repertoire adm ................................................
    if ( ! QFile::exists( sesamPath + "/adm"  ) )
    {   QDir cur_dir( sesamPath + "/adm" );
        toLog(   tr ("----------- Création de ce répertoire en cours : ") + sesamPath + "/adm" + "-----------");
        cur_dir.mkdir(sesamPath + "/adm");
    }
    copyTables_Api( path + "/../../Sesam-API-Local/adm", sesamPath + "/adm");
        //.................................. fichier galss.ini reparametrer le port COM.........................................................
    QString galss = "";
    CGestIni::Param_UpdateFromDisk(system_path + "/galss.ini" , galss);
    int pos  = -1;
    int end  = -1;
    pos  = galss.find("MedinTux le :"); if (pos == -1) galss.prepend(tr("; Fichier galss.ini sous Linux set by MedinTux le :\n")); pos  = galss.find("MedinTux le :");
    if (pos != -1)
       {pos += 13;  // passer 'MedinTux le :'
        end  = galss.find("\n", pos);
        if (end != -1) galss.replace(pos, end-pos, QDate::currentDate().toString("dd-MM-yyyy"));
       }
    pos  = galss.find("[CANAL1]");
    if (pos != -1)
       {pos += 7;
        pos  = galss.find("Index=",pos);
        if (pos != -1)
           {pos += 6;
            end  = pos; while(end< (int) galss.length() && galss[end] != '\r' && galss[end] != '\n') ++end;
            QString portCom  = comboBox_PortLecteurWin->currentText();
            if (portCom.length())
               {//int pSepEnd  = portCom.find("=");
                int pSepDeb  = portCom.find("COM");
                if (pSepDeb != -1)
                   {pSepDeb += 3;
                    galss.replace(pos, end-pos, QString::number(portCom.mid(pSepDeb).toInt()));
                   }
               }
           }
       }
    CGestIni::Param_UpdateToDisk(system_path + "/galss.ini" , galss);
    toLog(   tr ("----------- Positionnement de : ") + system_path + "/galss/galss.ini" + " sur le bon port COM -----------");
    toLog(   galss);

    CGestIni::Param_UpdateToDisk(path +"/Journal-Installation-SesamVitale.txt" , textEdit_Message->text());      // enregistrer le ficher modifie dans la bonne destination
#endif

#ifdef  Q_WS_MAC

#endif

}

//----------------------------------------- copyLinux_Api -------------------------------------------------
//      QString api_path    = path + "/../../Sesam-API-Local/ApiLinux";
//      QString cpp_path    = path + "/../../qfselib/src";
//      QString system_path = lineEdit_BinaryPath->text();           // "/usr/local"
//      QString sesamPath   = system_path + "/sesam";                // "/usr/local/sesam"
//copyLinux_Api(api_path,  system_path + "/galss",  cpp_path, sesamPath);
void C_DlgMainDialog::copyLinux_Api(QString src_Dir,  QString bin_Dir,  QString cpp_Dir, QString sesam_Dir)
{
    //........................ explorer la source .......................................
    QDir cur_dir( src_Dir );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {if (fname.left(4).lower() != "sgal") copyLinux_Api(src_Dir + "/" + fname, bin_Dir, cpp_Dir, sesam_Dir);
        }
        else if (fi->isFile())
        {   QString ext = getExtension(fname).lower();
            if (ext==".gz")
            {
            }
            else if (src_Dir.endsWith("LibMedinTux"))
            {   G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie de : ")  + src_Dir +"/" + fname + " vers : " + cpp_Dir + "/" + fname);
                CHtmlTools::Copy_File(src_Dir +"/" + fname, bin_Dir+"/LibMedinTux");
            }

            else if (fname =="libstslux.so" || fname =="libssvlux.so" || fname =="libsrtlux.so" || fname =="libsgdlux.so" )
            {   G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie de : ")  + src_Dir +"/" + fname + " vers : " + cpp_Dir + "/" + fname);
                CHtmlTools::Copy_File(src_Dir +"/" + fname, cpp_Dir);     // pour compilation des sources
                CHtmlTools::Copy_File(src_Dir +"/" + fname, bin_Dir);     // pour execution es binaires
                toLog( tr ("     Copie de : ") + src_Dir +"/" + fname + " vers : " + sesam_Dir + "/lib" + fname);
                CHtmlTools::Copy_File(src_Dir +"/" + fname, sesam_Dir + "/lib");
            }
            //....................... fichiers a placer dans le repertoire binaire ....................................
            else if (ext ==".so" || fname =="galsvlux" || fname =="cpgeslux" || ext ==".gip"|| ext ==".ini")
            {   //.............. si fname = sesam.ini le modifie et le corrige .........................
                if (! TestAndSet_SesamVIniFiles(fname, sesam_Dir, src_Dir, sesam_Dir + "/etc" ))
                   {   toLog(  tr ("     Copie de : ") + src_Dir +"/" + fname + " vers : " + bin_Dir + "/" + fname);
                       CHtmlTools::Copy_File(src_Dir +"/" + fname, bin_Dir);
                   }
                if (ext ==".so" && (fname.startsWith("libsts")||
                                    fname.startsWith("libsm") ||
                                    fname.startsWith("libssv")||
                                    fname.startsWith("libsrt")||
                                    fname.startsWith("libsgd")||
                                    fname.startsWith("libjs"))
                   )
                   {   toLog( tr ("     Copie de : ") + src_Dir +"/" + fname + " vers : " + sesam_Dir + "/lib" + fname);
                       CHtmlTools::Copy_File(src_Dir +"/" + fname, sesam_Dir + "/lib");
                   }
            }
            //....................... fichiers .h à modifier avant de recopier ....................................
            else if (TestAndSet_HeaderDevFiles(fname, src_Dir, cpp_Dir)) //true si .h & cie
                    {
                    }

            //....................... repertoire de developpement ....................................
            else if (ext ==".h")
            {   G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie de : ")  + src_Dir +"/" + fname + " vers : " + cpp_Dir + "/" + fname);
                CHtmlTools::Copy_File(src_Dir +"/" + fname, cpp_Dir);
            }
        }
        ++it;
    } // endwhile ( (fi = it.current()) != 0 )
}


//----------------------------------------- copyWindw_Api -------------------------------------------------
void C_DlgMainDialog::copyWindw_Api(QString src_Dir,  QString bin_Dir,  QString cpp_Dir, QString sesam_Dir)
{ //........................ explorer la source .......................................
    QDir cur_dir( src_Dir );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {if (fname.left(4).lower() != "sgal") copyWindw_Api(src_Dir + "/" + fname, bin_Dir, cpp_Dir, sesam_Dir);
        }
        else if (fi->isFile())
        {   QString ext = getExtension(fname).lower();
            if (ext==".gz")
            {
            }
            else if (ext ==".dll" || ext ==".exe" || ext ==".ini" || ext ==".gip")
            {  if (! TestAndSet_SesamVIniFiles(fname, sesam_Dir, src_Dir, bin_Dir))
                  {  G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie de : ") + src_Dir +"/" + fname + " vers : " + bin_Dir + "/" + fname);
                     CHtmlTools::Copy_File(src_Dir +"/" + fname, bin_Dir);
                  }
            }
            else if (TestAndSet_HeaderDevFiles(fname, src_Dir, cpp_Dir))
                    {
                    }
            else if (ext ==".lib" || ext ==".h")
            {   G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie de : ")  + src_Dir +"/" + fname + " vers : " + cpp_Dir + "/" + fname);
                CHtmlTools::Copy_File(src_Dir +"/" + fname, cpp_Dir);
            }
        }
        ++it;
    } // endwhile ( (fi = it.current()) != 0 )
}

//----------------------------------------- TestAndSet_SesamVIniFiles  ---------------------------------------------
bool C_DlgMainDialog::TestAndSet_SesamVIniFiles(const QString &fname, const QString &sesam_Dir, const QString &src_Dir, const QString &dest_Dir)
{if (fname=="sesam.ini")
    {G_pCApp->Datasemp_OutMessage(textEdit_Message, tr (".......... Modification de : ")  +  fname + " ......................");
     QString str_ini;
     CGestIni::Param_UpdateFromDisk(src_Dir +"/" + fname , str_ini);    // charger le ficher et le modifier
     CGestIni::Param_WriteParam(&str_ini,"COMMUN", "RepertoireTravail", sesam_Dir + QDir::separator()+"adm");
     CGestIni::Param_WriteParam(&str_ini,"COMMUN", "RepertoireTable"  , sesam_Dir + QDir::separator()+"ssv");
     CGestIni::Param_WriteParam(&str_ini,"SSV",    "RepertoireTable"  , sesam_Dir + QDir::separator()+"ssv");
     CGestIni::Param_WriteParam(&str_ini,"SRT",    "RepertoireTable"  , sesam_Dir + QDir::separator()+"srt");
     CGestIni::Param_WriteParam(&str_ini,"STS",    "RepertoireTable"  , sesam_Dir + QDir::separator()+"sts");
     str_ini.prepend(tr ("; Installation de Sesam-Vitale le : ") + QDate::currentDate().toString("dd-MM-yyyy") + tr(" par set_bases pour MedinTux\r\n"));
     str_ini.replace(" = ","=");   // sinon le ini de Sesam Vitale ne fonctionne pas !!!
     G_pCApp->Datasemp_OutMessage(textEdit_Message, str_ini);
     G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie de : ") + src_Dir +"/" + fname + tr(" modifié vers : ") + dest_Dir + "/" + fname );
     CGestIni::Param_UpdateToDisk(dest_Dir +"/" + fname , str_ini);      // enregistrer le ficher modifie dans la bonne destination
     return TRUE;
    }
  return FALSE;
}

//----------------------------------------- TestAndSet_HeaderDevFiles  ---------------------------------------------
bool C_DlgMainDialog::TestAndSet_HeaderDevFiles(const QString &fname, const QString &src_Dir, const QString &cpp_Dir)
{ if (fname == "sgd.h" || fname == "srt.h" || fname == "ssv.h"|| fname == "sts.h")
     {
      G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("     Copie et modification de : ")  + src_Dir + "/" + fname + " vers : " + cpp_Dir + "/" + fname);
      QString cpp;
      CGestIni::Param_UpdateFromDisk(src_Dir +"/" + fname , cpp);    // charger le ficher et le modifier
      int pos = cpp.find(tr ("(## add by and for MedinTux)"));
      if (pos ==-1) cpp.replace("etc. */",
                             tr("etc. */\r\n//......... gérer les spécificités selon OS (## add by and for MedinTux) ...............\r\n"
                                " #if   defined(Q_WS_WIN)\r\n"
                                "   #include \"win32def.h\"\r\n"
                                "#elif defined(Q_WS_X11)\r\n"
                                "   #include \"unixdef.h\"\r\n"
                                "#elif defined(Q_WS_MAC)\r\n"
                                "   #include \"macosdef.h\"\r\n"
                                "#endif\r\n"));
       CGestIni::Param_UpdateToDisk(cpp_Dir +"/" + fname , cpp);      // enregistrer le ficher modifie dans la bonne destination
       return TRUE;
     }
  return FALSE;
}


//----------------------------------------- getExtension  ---------------------------------------------
QString C_DlgMainDialog::getExtension(const QString &file)
{   int pos = file.findRev('.');
    if (pos ==-1) return QString::null;
    return file.mid(pos);
}

//----------------------------------------- copyTables_Api ---------------------------------------------
void C_DlgMainDialog::copyTables_Api(QString src_Dir,  QString dest_Dir)
{
    //........................ explorer la source .......................................
    QDir cur_dir( src_Dir );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );

    if ( ! QFile::exists( dest_Dir  ) )
    { G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("----------- Création de ce répertoire en cours : ") + dest_Dir + "-----------");
        cur_dir.mkdir(dest_Dir);
    }
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo  *fi;
while ( (fi = it.current()) != 0 )
    {   QString   ext = fi->extension( FALSE);  // false car on veut les dernier .ext
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {   QString prefix = fname.left(3).lower();
            if (prefix == "srt" || prefix == "ssv" || prefix == "sts"|| prefix == "lib")
               {  copyTables_Api( src_Dir + "/" + fname, dest_Dir + "/" + prefix);
               }
        }
        else if (fi->isFile())
        {
            if (ext != "h" && ext != "dll" && ext != "lib" && ext != "exe" && ext != "so" && ext != "gz")
               {   G_pCApp->Datasemp_OutMessage(textEdit_Message, tr(" Table Sesam-Vitale en cours de copie : ") + src_Dir + "/" + fname + " vers : " + dest_Dir + "/" + fname);
                   CHtmlTools::Copy_File(src_Dir + "/" + fname, dest_Dir);
               }
        }
        ++it;
    }
}

bool C_DlgMainDialog::fichiersinipmd(QString &userName,QString &passWord,QString &hostName,QString &pathsetbases)
{

 QString bazar          = passWord;//on squeeze cryptage
 QString pathabsolu     = pathsetbases;
 QString user           = userName;
 QString host           = hostName;
//-------------------remplissage motdepasse.ini------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////
/// renseigner tous les .ini nouvelle mouture
////////////////////////////////////////////////////////////////////////////////////////////////

  //pensebetux/Alarmes/bin/utilisateur.ini
  QStringList listealarme;
  QStringList listealarmenew;
  QFile filealarme(""+pathabsolu+"/../../pensebetux/Alarmes/bin/utilisateur.ini");
  if (!filealarme.open(IO_ReadOnly | IO_Truncate))   return false;
  QTextStream streamalarme(&filealarme);
  while(!streamalarme.atEnd())
       {  QString ligne = streamalarme.readLine();
          listealarme   << ""+ligne+"";
       }
  listealarmenew          = listealarme;
  QString hostalarmeligne = listealarme[0];
  QString useralarmeligne = listealarme[1];
  QString motalarmeligne  = listealarme[2];
  listealarme.clear();
  //QStringList motalarmeliste = motalarmeligne.split("=");
  QString hostalarmeligne0 = hostalarmeligne.section('=',0,0);
  QString useralarmeligne0 = useralarmeligne.section('=',0,0);
  QString motalarmeligne0  = motalarmeligne.section('=',0,0);
  QString lignetotale0     = hostalarmeligne0 +"="+ host;
  QString lignetotale1     = useralarmeligne0 +"="+ user;
  QString lignetotale2     = motalarmeligne0  +"="+ bazar;
  listealarme << lignetotale0 << lignetotale1 << lignetotale2 << listealarmenew[3];
  filealarme.flush();
  if (filealarme.remove())
     {QFile filealarme2(""+pathabsolu+"/../../pensebetux/Alarmes/bin/utilisateur.ini");
      if (!filealarme2.open(IO_WriteOnly | IO_Truncate)) return false;
      QTextStream streamalarme2(&filealarme2);
      for (int i=0 ; i < (int)listealarme.size() ; i++)
          { streamalarme2 << ""+listealarme[i]+"\n";
          }
     }

  //pensebetux/lecture_table_alarmes/bin/utilisateur.ini
  QStringList listelecture;
  QStringList listelecturenew;
  QFile filelecture(""+pathabsolu+"/../../pensebetux/lecture_table_alarmes/bin/utilisateur.ini");
  if (!filelecture.open(IO_ReadOnly | IO_Truncate))  return false;
  QTextStream streamlecture(&filelecture);
  while(!streamlecture.atEnd())
       { QString ligne = streamlecture.readLine();
         listelecture << ""+ligne+"";
       }
  listelecturenew          = listelecture;
  QString hostlectureligne = listelecture[0];
  QString userlectureligne = listelecture[1];
  QString motlectureligne  = listelecture[2];
  listelecture.clear();
  QString hostlectureligne0   = hostlectureligne.section('=',0,0);
  QString userlectureligne0   = userlectureligne.section('=',0,0);
  QString motlectureligne0    = motlectureligne.section('=',0,0);
  QString lignelecturetotale0 = hostlectureligne0 +"="+ host;
  QString lignelecturetotale1 = userlectureligne0 +"="+ user;
  QString lignelecturetotale2 = motlectureligne0 +"="+ bazar;
  listelecture << lignelecturetotale0 << lignelecturetotale1 << lignelecturetotale2 << listelecturenew[3];
  filelecture.flush();
  if (filelecture.remove())
     { QFile filelecture2(""+pathabsolu+"/../../pensebetux/lecture_table_alarmes/bin/utilisateur.ini");
       if (!filelecture2.open(IO_WriteOnly | IO_Truncate))   return false;
       QTextStream streamlecture2(&filelecture2);
       for (int i=0 ; i < (int)listelecture.size() ; i++)
           { streamlecture2 << ""+listelecture[i]+"\n";
           }
     }
//sauvegarde/bin/utilisateur.ini
  QStringList listesauvegarde;
  QStringList listesauvegardenew;
  QFile filesauvegarde(""+pathabsolu+"/../../sauvegarde/bin/utilisateur.ini");
  if (!filesauvegarde.open(IO_ReadOnly | IO_Truncate))  return false;
  QTextStream streamsauvegarde(&filesauvegarde);
  while (!streamsauvegarde.atEnd())
        { QString ligne = streamsauvegarde.readLine();
          listesauvegarde << ""+ligne+"";
        }
  listesauvegardenew          = listesauvegarde;
  QString hostsauvegardeligne = listesauvegarde[0];
  QString usersauvegardeligne = listesauvegarde[1];
  QString motsauvegardeligne  = listesauvegarde[2];
  listesauvegarde.clear();
  QString hostsauvegardeligne0   = hostsauvegardeligne.section('=',0,0);
  QString usersauvegardeligne0   = usersauvegardeligne.section('=',0,0);
  QString motsauvegardeligne0    = motsauvegardeligne.section('=',0,0);
  QString lignesauvegardetotale0 = hostsauvegardeligne0 +"="+ host;
  QString lignesauvegardetotale1 = usersauvegardeligne0 +"="+ user;
  QString lignesauvegardetotale2 = motsauvegardeligne0 +"="+ bazar;
  listesauvegarde << lignesauvegardetotale0 << lignesauvegardetotale1 << lignesauvegardetotale2  << listesauvegardenew[3];
  filesauvegarde.flush();
  if (filesauvegarde.remove())
     { QFile filesauvegarde2(""+pathabsolu+"/../../sauvegarde/bin/utilisateur.ini");
       if (!filesauvegarde2.open(IO_WriteOnly | IO_Truncate))   return false;
       QTextStream streamsauvegarde2(&filesauvegarde2);
       for (int i=0 ; i < (int)listesauvegarde.size() ; i++)
           { streamsauvegarde2 << ""+listesauvegarde[i]+"\n";
           }
     }
  //apitux/bin/utilisateur.ini
  QStringList listeapitux;
  QStringList listeapituxnew;
  QFile fileapitux(""+pathabsolu+"/../../apitux/bin/utilisateur.ini");
  if (!fileapitux.open(IO_ReadOnly | IO_Truncate))  return false;
  QTextStream streamapitux(&fileapitux);
  while (!streamapitux.atEnd())
        { QString ligne = streamapitux.readLine();
          listeapitux   << ""+ligne+"";
        }
  listeapituxnew          = listeapitux;
  QString hostapituxligne = listeapitux[0];
  QString userapituxligne = listeapitux[1];
  QString motapituxligne  = listeapitux[2];
  listeapitux.clear();
  QString hostapituxligne0   = hostapituxligne.section('=',0,0);
  QString userapituxligne0   = userapituxligne.section('=',0,0);
  QString motapituxligne0    = motapituxligne.section('=',0,0);
  QString ligneapituxtotale0 = hostapituxligne0 +"="+ host;
  QString ligneapituxtotale1 = userapituxligne0 +"="+ user;
  QString ligneapituxtotale2 = motapituxligne0 +"="+ bazar;
  listeapitux << ligneapituxtotale0 << ligneapituxtotale1 << ligneapituxtotale2 << listeapituxnew[3];
  fileapitux.flush();
  if (fileapitux.remove())
     {
      QFile fileapitux2(""+pathabsolu+"/../../apitux/bin/utilisateur.ini");
      if (!fileapitux2.open(IO_WriteOnly | IO_Truncate))  return false;
      QTextStream streamapitux2(&fileapitux2);
      for (int i=0 ; i < (int)listeapitux.size() ; i++)
          { streamapitux2 << ""+listeapitux[i]+"\n";
          }
     }
//comptabilite.ini--------------------------------------------------------------------
    QString path = CGestIni::Construct_Name_File_Ini("comptabilite",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    QString param;
    // Affichage dans la Box
    G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\n================================== Modification : ") + path + "==================================");
    if (  ! QFile::exists (path) )
       {G_pCApp->Datasemp_OutMessage(textEdit_Message, tr("\nCe fichier n'existe pas"));        return false;
       }
    CGestIni::Param_UpdateFromDisk(path, param);
 //   CGestIni::Param_ReadParam (param,  "Connexion" ,    "Pass" , &User , &Serv);
    CGestIni::Param_WriteParam(&param, "Connexion" ,  "Pass" , bazar);
    CGestIni::Param_WriteParam(&param, "Connexion" ,  "User" , user );
    CGestIni::Param_WriteParam(&param, "Connexion" ,  "Serv" , host );
    CGestIni::Param_WriteParam(&param, "Connection" , "Pass" , bazar);
    CGestIni::Param_WriteParam(&param, "Connection" , "User" , user );
    CGestIni::Param_WriteParam(&param, "Connection" , "Serv" , host );
    CGestIni::Param_WriteParam(&param, "passe"      , "motdepassecrypte(mettrelaphrasemagiqueobtenueparleprogrammecrypt)" ,  bazar);
    CGestIni::Param_UpdateToDisk(path, param);
//-----------------------------------------------------------------------------------------
return true;
}


//-----------------------------------------------------  PassWordEncode --------------------------------------------------
// Algo minimal pour l'instant
// chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
// resultat final en hexa;
// les linges qui sechent mouillent les cordes
// brouiller les cartes

QString C_DlgMainDialog::PassWordEncode(QString &pass)
{char encoded_car;
 char pt_magic_key[] = "les linges qui sechent mouillent les cordes";
 QString encoded_str = "";
 int len_pass        = pass.length();
 char    *pt         = (char*)(const char*)pass;
 for (int i=0; i<len_pass;++i)
     {encoded_car  =  pt[i]^pt_magic_key[i];
      encoded_str += UINTtoHex(encoded_car);
     }
 return encoded_str;

 /*
 char str[34];
 INT16    val;
 int len         = pass.length();

 if (len>32)       return ERR_PASS_MAX;
 if (len==0)       return ERR_PASS_ZERO;
 strcpy(str , pass);
 pass = "";
 for (int i=0; i<len; ++i)
     { val   =  str[i];
       val   =  (val << ((i+1)&0x000F) )^val;
      pass  += UINTtoHex(val);
     }
 return ERR_PASS_OK;
 */

}

//-----------------------------------------------------  PassWordDecode --------------------------------------------------
// Algo minimal pour l'instant
//
QString C_DlgMainDialog::PassWordDecode(QString str_to_decode)
{
 char decoded_car;
 char pt_magic_key[] = "les linges qui sechent mouillent les cordes";
 QString decoded_str = "";
 int len_pass        = str_to_decode.length();
 int      pos        = 0;
 while ( pos < len_pass)
     {decoded_car  = HexToUINT( str_to_decode.mid(pos,4));
      decoded_car  = decoded_car ^ pt_magic_key[pos/4];
      decoded_str += decoded_car;
      pos         += 4;
     }
 return decoded_str;

 /*
 char     buf[16];
 UINT16   val;
 if (str_to_decode == 0 || str_pass == 0) return ERR_PASS_MAX;
 int len_to_decode      = strlen(str_to_decode);
 if (len_to_decode==0)             return ERR_PASS_ZERO;
 int len_pass           = strlen(str_pass);
 if (len_pass != len_to_decode/4)  return ERR_PASS_MAX;
 QString result;
 int end = len_to_decode/4;
 buf[4 ] = 0;
 for (int i=0; i<end; ++i)
     {memcpy(buf, str_to_decode + i*4, 4);  // recuperer les 4 caractere hexa
      val = HexToUINT(buf);                 // les convertir en UINT16
      val = val ^ str_pass[i];              // xor avec le mot de passe
      val = val >> ((i+1)&0x000F);          // rotation inverse maxi de 8 et on devrait avoir le code ASCII
      if (val != str_pass[i])      return ERR_PASS_MAX;    // erreur si pas egal
     }
 return ERR_PASS_OK;
 */

}

//-----------------------------------------------------  UINTtoHex --------------------------------------------------------
QString C_DlgMainDialog::UINTtoHex( UINT16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   // là on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
UINT16 C_DlgMainDialog::HexToUINT( const char *str)
{int i   = strlen(str);
 if (i==0) return 0;
 UINT16 val = 0;
 int    dec = 0;
 while (i>0)
   {--i;
    unsigned char car = str[i] | 32;     // mettre en minuscules
    if      (car>='a'&& car<= 'f')  val  += (car-'W')<<dec;  //a (97) - W (87) --> 10
    else                            val  += (car-'0')<<dec;  //0 (48) - 0 (48) --> 0
    dec += 4;
   }
 return val;
}
