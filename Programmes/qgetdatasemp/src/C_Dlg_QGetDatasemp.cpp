/********************************* C_Dlg_QGetDatasemp.h ********************************
 *                                                                                *
 *                                                                                *
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
 *   Commissariat ? l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F?d?ration, 75752 PARIS cedex 15.   *
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
// mysqldump --quote-name --compatible=mysql323 MedicaTuxTest IndexProd IndexDiet IndexVeterinaire IndexParapharmacie IndexAccessoire IndexDivers IndexHomeopathie > /home/ro/Dump_IndexMedica-0001-oo-0-oo-119A02.2.sql
// mysqldump --quote-name --compatible=mysql323 DatasempTest > /home/ro/Dump_Datasemp-0001-oo-0-oo-119A02.2.sql

//................................ règles de nommage des fichiers de mise à jour ................................
// Dl-1144774740-Dump_Datasemp-0001-oo-1234-oo-112B02.2.sql
// ^  ^          ^                     ^       ^_____________ Datasemp version
// |  |          |_____ Ident Patern   |___ nb progress
// |  |                 Le nom du fichier sur le FTP commence ici
// |  |________ DateTime from FTP upload
// |__________________ Le nom du fichier téléchargé commence ici (précédé de 'Dl-1144774740-')
//                                                                            ^
//                                                                            |________ DateTime from FTP upload
//
#include "C_Dlg_QGetDatasemp.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qeventloop.h>
#include <qdatetime.h>
#include <qmime.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfiledialog.h> 

#include "../../MedinTuxTools/CW_EditText.h"
#include "CApp.h"


#define  ERR_PASS_MAX       1
#define  ERR_PASS_ZERO      2
#define  ERR_PASS_OK        3

//----------------------------------------- C_Dlg_QGetDatasemp --------------------------------------------
// clef = 005E00540041000E005E005B00590049005400400016005F004400510015 P 005E0054 W 0001000000170049000F0008000C000600160016 X Q 000A00080016004400050007001E0012001D O 000A000800550044005D001D003B001F004F00070050
//        005E00540041000E005E005B00590049005400400016005F004400510015 P 005E0054 W 0001000000170049000F0008000C000600160016   Q 000A00080016004400050007001E0012001D O 000A000800550044005D001D003B001F004F00070050
//        005E00540041000E005E005B00590049005400400016005F004400510015 P 005E0054 W 0001000000170049000F0008000C000600160016   Q 000A00080016004400050007001E0012001D O 000A000800550044005D001D003B001F004F00070050
C_Dlg_QGetDatasemp::C_Dlg_QGetDatasemp(QWidget* parent, const char* name, bool modal, WFlags fl)
: Dlg_QGetDatasemp(parent,name, modal,fl)
{m_Debug                   = "";
 m_DefaultStringClefAcces  = tr("Ne renseigner que sur indication et à la première utilisation");
 QString clef              = "";
 CGestIni::Param_ReadParam(G_pCApp->m_ParamData, "Authentification", "clef",     &clef,    &m_Debug);  // hostname W port X folder P clef
 clef.replace('P','|');    //     --> P m_FtpAdress
 clef.replace('W','|');    //  P <--> W m_FtpPort
 clef.replace('X','|');    //  W <--> X m_FtpFolder
 clef.replace('Q','|');    //  X <--> Q m_DatasempKey
 clef.replace('O','|');    //  Q <--> O m_FtpLogName
                           //  O <---   m_FtpLogPass

 QStringList lst = QStringList::split('|' , clef, TRUE);
 m_FtpAdress   = lst[0];
 m_FtpPort     = lst[1];
 m_FtpFolder   = lst[2];
 m_DatasempKey = lst[3];
 m_FtpLogName  = lst[4];
 m_FtpLogPass  = lst[5];

 textLabel_Adresse->hide();
 lineEdit_FtpAdress->hide();
 textLabel_Repertoire->hide();
 lineEdit_FtpRepertoire->hide();
 textLabel_Port->hide();
 lineEdit_FtpPort->hide();

 m_FtpAdress   = PassWordDecode(m_FtpAdress);
 m_FtpPort     = PassWordDecode(m_FtpPort);
 m_FtpFolder   = PassWordDecode(m_FtpFolder);
 m_FtpLogName  = PassWordDecode(m_FtpLogName);
 m_FtpLogPass  = PassWordDecode(m_FtpLogPass);
 m_DatasempKey = PassWordDecode(m_DatasempKey);

 lineEdit_AccessKey->setEchoMode ( QLineEdit::Normal );

 //G_pCApp->Param_ReadParam("Autentification", "clef",     &m_FtpAdress,    &m_FtpPort);
 //G_pCApp->Param_ReadParam("ConnexionFTP", "Utilisateur", &m_DatasempKey, &m_Debug);
 //G_pCApp->Param_ReadParam("ConnexionFTP", "Repertoire",  &m_FtpFolder);
 //............ top secret ................................
 if (m_Debug=="lesudadusel")
    {lineEdit_LogName->show();
     lineEdit_LogPass->show();
     textLabel_LogName->show();
     textLabel_LogPass->show();
     textLabel_Port->show();
     lineEdit_FtpPort->show();
     textLabel_Adresse->show();
     lineEdit_FtpAdress->show();
     textLabel_Repertoire->show();
     lineEdit_FtpRepertoire->show();
     lineEdit_UserName->setEchoMode ( QLineEdit::Normal );
     lineEdit_LogPass->setEchoMode ( QLineEdit::Normal );
     lineEdit_AccessKey->setText(lst[0]+"P"+lst[1]+"W"+lst[2]+"Q"+lst[4]+"O"+lst[5]);
     pushButton_ComputeClefAcces->show();
     pushButton_tester->show();
     //m_DroitsOk        = 1;
    }
 else
    {lineEdit_LogName->hide();
     lineEdit_LogPass->hide();
     textLabel_LogName->hide();
     textLabel_LogPass->hide();
     pushButton_ComputeClefAcces->hide();
     pushButton_tester->hide();
     lineEdit_AccessKey->setText(m_DefaultStringClefAcces);   // "Ne renseigner que sur indication et à la première utilisation"
     //m_DroitsOk        = 0;
    }
 m_UsersList = "";
 lineEdit_LogName->setText(m_FtpLogName);
 lineEdit_LogPass->setText(m_FtpLogPass);
 lineEdit_FtpAdress->setText(m_FtpAdress);
 lineEdit_FtpPort->setText(m_FtpPort);
 lineEdit_FtpRepertoire->setText(m_FtpFolder);
 lineEdit_UserName->setText(m_DatasempKey);

 //lineEdit_LogPass->setText(m_FtpLogPass);
 //........................ classe medicamenteuse ..........................................
 m_pCMedicaBase = 0;
 //....................... positionner parametres du sequenceur d'actions ..................
 cW_EditText_Log->mimeSourceFactory()->addFilePath(G_pCApp->m_PathAppli +   "Ressources" );
 ResetSequenceur();
 //................. placer le sequenceur sur les actions à réaliser .............................
 FtpActionList_Set();

//................. connecter les boutons .............................
 connect( pushButton_Connexion,         SIGNAL( clicked() ), this, SLOT( pushButton_Connexion_clicked() ) );
 connect( pushButton6,                  SIGNAL( clicked() ), this, SLOT( pushButton6_clicked() ) );
 connect( pushButtonLocal,              SIGNAL( clicked() ), this, SLOT( pushButtonLocal_clicked() ) );
 connect( pushButton_ComputeClefAcces,  SIGNAL( clicked() ), this, SLOT( pushButton_ComputeClefAcces_clicked() ) );
 connect( pushButton_Upload,            SIGNAL( clicked() ), this, SLOT( pushButton_Upload_clicked() ) );
 connect( pushButton_tester,            SIGNAL( clicked() ), this, SLOT( pushButton_tester_clicked() ) );
 //................. connecter le timer sur le sequenceur d'actions .............................
 connect( &m_ActionTimer, SIGNAL(timeout()), this , SLOT(slot_DoSequenceur()));
 //.................. créer le FTP et le connecter sur ses slots ................................
 m_pFtpTools = new CFtpTools(this);
 connect( m_pFtpTools, SIGNAL(stateChanged(int)),              this, SLOT(slotFTP_state_changed(int)) );
        // permet de connaitre le lancement d'une nouvelle commande
 connect( m_pFtpTools, SIGNAL( commandStarted(int) ) ,         this , SLOT(slotFTP_Started(int) ) );
        // permet de connaitre la fin d'une commande
 connect( m_pFtpTools, SIGNAL(commandFinished(int,bool)) ,     this , SLOT(slotFTP_Finished(int,bool)));
        // chaque fois qu'un fichier est listé
 connect( m_pFtpTools, SIGNAL(listInfo(const QUrlInfo &)) ,    this , SLOT(slot_LlistInfo(const QUrlInfo &)));
connect( m_pFtpTools,  SIGNAL(dataTransferProgress(int,int)),  this,  SLOT(setProgress(int,int)) );
 //........................ icone ................................................................
 setIcon( G_pCApp->m_PathRessources + "qgetdatasemp.png" );
 lineEdit_Message->setText("");

 progressBar->hide();
 setCaption(tr("Mise à jour du Vidal Datasemp pour MedinTux"));
 setIcon ( G_pCApp->m_PathAppli +   "Ressources/QGetDataemp.png" );
 lineEdit_FtpAdress->setFocus();
 cW_EditText_Log->setBackgroundMode(Qt::FixedColor);
 cW_EditText_Log->setPaletteBackgroundColor (QColor("black") );
 cW_EditText_Log->setWordWrap ( QTextEdit::NoWrap );
 cW_EditText_Log->setColor (QColor("#00e413") );
#if   defined(Q_WS_WIN)
   if (!QFile::exists("C:/MedinTuxRo.txt"))                     pushButton_Upload->hide();
#elif defined(Q_WS_X11)
   if (!QFile::exists(QDir::homeDirPath ()+"/MedinTuxRo.txt"))  pushButton_Upload->hide();
#elif defined(Q_WS_MAC)
   if (!QFile::exists(QDir::homeDirPath ()+"/MedinTuxRo.txt"))  pushButton_Upload->hide();
#endif

if (G_pCApp->m_Command=="UPLOAD_LAST_FILE") pushButton_Upload_clicked();
}

//----------------------------------------- ~C_Dlg_QGetDatasemp --------------------------------------------
C_Dlg_QGetDatasemp::~C_Dlg_QGetDatasemp()
{if (m_pFtpTools)    delete m_pFtpTools;
 Save_Param();
 if (m_pCMedicaBase) delete m_pCMedicaBase;
}

/*$SPECIALIZATION$*/
//----------------------------------------- ResetSequenceur --------------------------------------------
void C_Dlg_QGetDatasemp::ResetSequenceur()
{//........... remettre à zero le sequenceur d'actions ...........................
 cW_EditText_Log->setText("");
 toLog(CW_EditText::fillWith ("<b>", "&nbsp;",37 ) + tr("<font color=\"#ff7e00\">Bienvenue sur la mise à jour du Vidal Datasemp</font></b><br>")+
 // toLog(tr("<b>============== <font color=\"#ff7e00\">Bienvenue sur la mise à jour Vidal Datasemp</font> ==================</b><br>")+
       CW_EditText::fillWith ("<b>", "&nbsp;",0 ) + "<img source=\"./logo.png\"><img source=\"./DataSempLogo.png\">");
 toLog(CW_EditText::fillWith ("<b>", "&nbsp;",37 ) + tr("<font color=\"#ff5400\">QGetDatasemp</font></b>"));
 toLog(CW_EditText::fillWith ("<b>", "&nbsp;",37 ) + tr("<font color=\"#fdff5a\">par Data Medical Design</font></b>"));
 toLog(CW_EditText::fillWith ("<b>", "&nbsp;",37 ) + tr("<font color=\"#5a66ff\">roland-sevin@medintux.org</font></b>"));
 toLog( tr("<b>================================================================</b>"));
 m_QVL_FileInfo.clear();        // liste des fichiers présents sur le FTP
 m_CommandMap.clear();          // liste des commandes FTP en cours
 m_CurrentAction       = "";    // verou pour signaler qu'une action FTP est en cours
 m_ActionIndex         = 0;     // index d'avancement dans la liste d'actions : m_ActionList
 m_LastMedicaFile      = "";    // fichier MedicaTux le plus récent sur le FTP
 m_LastDatasempFile    = "";    // fichier Datasemp  le plus récent sur le FTP
 m_LastMedinTuxFile    = "";    // fichier MedinTux  le plus récent sur le FTP
 m_LastVersionMedica   = 0;     // QDateTime().toUInt() du fichier MedicaTux le plus récent sur le FTP
 m_LastVersionDatasemp = 0;     // QDateTime().toUInt() du fichier Datasemp  le plus récent sur le FTP
 m_LastVersionMedinTux = 0;     // QDateTime().toUInt() du fichier Datasemp  le plus récent sur le FTP
 m_DroitsOk            = 1;
}

//----------------------------------------- SetIniFileOnDatasemp --------------------------------------------
void C_Dlg_QGetDatasemp::SetIniFileOnDatasemp()
{   toLog(tr("=========================================== Activation d'une base ===================================================="));
    QString path = CGestIni::Construct_Name_File_Ini("drtux", QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.ini");
    // Affichage dans la Box
    toLog(tr("\n================= Lecture Base Get ou Datasemp \n ") +  path );

    if (  ! QFile::exists (path) )
       {toLog(tr("le fichier de configuration : '%1' n'existe pas").arg(path));
        return ;
       }
    QString param;
    CGestIni::Param_UpdateFromDisk(path, param);
    CGestIni::Param_WriteParam(&param, "Connexion" , "InUseBase" ,  "Parametres MedicaBase" ,  "noMedicaBase");
    toLog( tr("\n ====> La base Vidal Datasemp vient d'être activée.") );
    CGestIni::Param_UpdateToDisk(path, param);
}

//----------------------------------------- FtpActionList_Set --------------------------------------------
void C_Dlg_QGetDatasemp::FtpActionList_Set()
{m_ActionList.clear();        // liste de sequencement des actions à faire
 m_ActionList.append("0000 CONNECT");
 m_ActionList.append("0001 LOGIN");
 m_ActionList.append("0002 CHDIR");
 m_ActionList.append("0003 GET_FILE_USERLIST");
 m_ActionList.append("0004 VERIFY_USER_DROITS");
 m_ActionList.append("0005 LIST");
 m_ActionList.append("0006 GET_NUM_LAST_VERSION_INDEX_MEDICA");
 m_ActionList.append("0007 GET_NUM_LAST_VERSION_DATASEMP");
 m_ActionList.append("0008 GET_FILE_MEDICA");
 m_ActionList.append("0009 GET_FILE_DATASEMP");
 m_ActionList.append("0010 INTEGRE_FILE_MEDICA");
 m_ActionList.append("0011 INTEGRE_FILE_DATASEMP");
 m_ActionList.append("0012 DECONNEXION");
}
//----------------------------------------- FtpActionUpload_Set --------------------------------------------
void C_Dlg_QGetDatasemp::FtpActionUpload_Set()
{m_ActionList.clear();                                             // liste de sequencement des actions à faire
 m_ActionList.append("0000 SET_NBSTEEPS_INDEX_MEDICA");            // force a mettre à jour a partir du fichier local pour que le nombre de pas s'inscrive
 m_ActionList.append("0001 SET_NBSTEEPS_DATASEMP");          // force a mettre à jour a partir du fichier local pour que le nombre de pas s'inscrive
 m_ActionList.append("0002 CONNECT");
 m_ActionList.append("0003 LOGIN");
 m_ActionList.append("0004 CHDIR");
 m_ActionList.append("0005 LIST");
 m_ActionList.append("0006 GET_NUM_LAST_VERSION_INDEX_MEDICA");    // on recupere la derniere version sur le serveur
 m_ActionList.append("0007 GET_NUM_LAST_VERSION_DATASEMP");  // on recupere la derniere version sur le serveur
 m_ActionList.append("0008 PUT_FILE_MEDICA");
 m_ActionList.append("0009 PUT_FILE_DATASEMP");
 m_ActionList.append("0010 DECONNEXION");
}

//----------------------------------------- FtpMedintuxActionList_Set --------------------------------------------
void C_Dlg_QGetDatasemp::FtpMedintuxActionList_Set()
{m_ActionList.clear();        // liste de sequencement des actions à faire
 m_ActionList.append("0000 CONNECT");
 m_ActionList.append("0001 LOGIN");
 m_ActionList.append("0002 CHDIR");
 m_ActionList.append("0003 GET_FILE_USERLIST");
 m_ActionList.append("0004 VERIFY_USER_DROITS");
 m_ActionList.append("0005 LIST");
 m_ActionList.append("0006 GET_NUM_LAST_VERSION_INDEX_MEDICA");
 m_ActionList.append("0007 GET_NUM_LAST_VERSION_DATASEMP");
 m_ActionList.append("0008 GET_FILE_MEDICA");
 m_ActionList.append("0009 GET_FILE_DATASEMP");
 m_ActionList.append("0010 INTEGRE_FILE_MEDICA");
 m_ActionList.append("0011 INTEGRE_FILE_DATASEMP");
 m_ActionList.append("0012 DECONNEXION");
}

//----------------------------------------- LocalMedintuxActionList_Set --------------------------------------------
void C_Dlg_QGetDatasemp::LocalMedintuxActionList_Set()
{m_ActionList.clear();        // liste de sequencement des actions à faire
 m_ActionList.append("0000 INTEGRE_FILE_MEDINTUX");
}

//----------------------------------------- LocalActionList_Set --------------------------------------------
void C_Dlg_QGetDatasemp::LocalActionList_Set()
{m_ActionList.clear();        // liste de sequencement des actions à faire
 m_ActionList.append("0000 INTEGRE_FILE_MEDICA");
 m_ActionList.append("0001 INTEGRE_FILE_DATASEMP");
}
//----------------------------------------- slot_DoSequenceur --------------------------------------------
void C_Dlg_QGetDatasemp::slot_DoSequenceur()
{if (m_CurrentAction.length()>0 ) return;
    /*
    {if (m_CurrentAction.mid(9) == commandToString(CFtp::Get))   // chaque fois que slotFTP_Started est appele, l'action m_CurrentAction = "ACTION : " + commandToString(CFtp::Get)
        {QFile f(m_DownloadFile.name());
         f.open( IO_ReadOnly );
         progressBar->setProgress(f.size());
         f.close();
        }
     return;
    }
    */
 for ( QStringList::Iterator it = m_ActionList.begin(); it != m_ActionList.end(); ++it )
     {int id         =  (*it).left(4).toInt();
      QString action =  (*it).mid(5);
      if (id == m_ActionIndex)
         {if      (action=="CONNECT")                             Connecter(action);
          else if (action=="LOGIN")                               Login(action);
          else if (action=="CHDIR")                               Chdir(action);
          else if (action=="GET_FILE_USERLIST")                   TelechargeUserList(action,  "DataSempUsers.lst");
          else if (action=="VERIFY_USER_DROITS")                  VerifyUserDroits(action,    "DataSempUsers.lst");
          else if (action=="LIST")                                ListerContenu(action);
          else if (action=="GET_NUM_LAST_VERSION_INDEX_MEDICA")   m_LastVersionMedica   = RecupererLastFTP_VersionFile(action,     m_LastMedicaFile,   "Dump_IndexMedica-", m_LastIndexMedica);
          else if (action=="GET_NUM_LAST_VERSION_DATASEMP")       m_LastVersionDatasemp = RecupererLastFTP_VersionFile(action,     m_LastDatasempFile, "Dump_Datasemp-",    m_LastDatasempInfo);
          else if (action=="SET_NBSTEEPS_INDEX_MEDICA")           setNbSteepFromLastVersionDatasempLocalFile(action, "Dump_IndexMedica-");
          else if (action=="SET_NBSTEEPS_DATASEMP")               setNbSteepFromLastVersionDatasempLocalFile(action, "Dump_Datasemp-");
          else if (action=="GET_FILE_MEDICA")                     TelechargeFile(action,       m_LastMedicaFile,   m_LastVersionMedica,      m_LastIndexMedica);
          else if (action=="GET_FILE_DATASEMP")                   TelechargeFile(action,       m_LastDatasempFile, m_LastVersionDatasemp,    m_LastDatasempInfo);
          else if (action=="PUT_FILE_MEDICA")                     UploaderFile(action,         m_LastMedicaFile,   "Dump_IndexMedica-");
          else if (action=="PUT_FILE_DATASEMP")                   UploaderFile(action,         m_LastDatasempFile, "Dump_Datasemp-");
          else if (action=="INTEGRE_FILE_MEDICA")                 IntegrerFileMedica(action,   m_LastMedicaFile,   m_LastVersionMedica);
          else if (action=="INTEGRE_FILE_DATASEMP")               IntegrerFileDatasemp(action, m_LastDatasempFile, m_LastVersionDatasemp);
          else if (action=="INTEGRE_FILE_MEDICA_FORCE")           IntegrerFileMedica(action,   m_LastMedicaFile,   m_LastVersionMedica,    C_Dlg_QGetDatasemp::forceIfSame);
          else if (action=="INTEGRE_FILE_DATASEMP_FORCE")         IntegrerFileDatasemp(action, m_LastDatasempFile, m_LastVersionDatasemp,  C_Dlg_QGetDatasemp::forceIfSame);
          else if (action=="DECONNEXION")                         Deconnecter(action);
          ++m_ActionIndex;             // incrémenter le pointeur d'actions
          return;
         }
     }
}

//----------------------------------------- Connecter --------------------------------------------
void C_Dlg_QGetDatasemp::Connecter(const QString &action)
{m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 m_pFtpTools->connectToHost( lineEdit_FtpAdress->text() , lineEdit_FtpPort->text().toUInt() );
 //toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>");
}
//----------------------------------------- Deconnecter --------------------------------------------
void C_Dlg_QGetDatasemp::Deconnecter(const QString &action)
{m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 m_pFtpTools->close();
 //toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>");
}
//----------------------------------------- Login --------------------------------------------
void C_Dlg_QGetDatasemp::Login(const QString &action)
{m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 m_pFtpTools->login( lineEdit_LogName->text() , lineEdit_LogPass->text() );
 //toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>");
}

//----------------------------------------- Chdir --------------------------------------------
void C_Dlg_QGetDatasemp::Chdir(const QString &action)
{m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 m_pFtpTools->cd( lineEdit_FtpRepertoire->text() );
 //toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>");
}
//----------------------------------------- pushButton_tester_clicked --------------------------------------------
void C_Dlg_QGetDatasemp::pushButton_tester_clicked()
{InitCMedicaBase();
 QString sqlDataFileName = QFileDialog::getOpenFileName(
                    G_pCApp->m_PathAppli +   "Ressources/",
                    "Files (*.sql *.txt *.*)",
                    this,
                    "open file dialog",
                    "Choose a file" );
 if (sqlDataFileName.length()==0) return;
 G_pCApp->SetProgressPosition(0);
 G_pCApp->ParseSQL_Dump(m_pCMedicaBase->m_MedicaBase, sqlDataFileName, progressBar, cW_EditText_Log );
}
//----------------------------------------- pushButton_Upload_clicked --------------------------------------------
void C_Dlg_QGetDatasemp::pushButton_Upload_clicked()
{//...................sauvegarder les valeurs .................................
 CApp::GotoDebug();
 Save_Param();
 //.................. relancer le sequenceur ..........................................
 SetIniFileOnDatasemp();          // placer le fichier ini sur le VidalDatasemp
 ResetSequenceur();               // remettre à zero le sequenceur d'actions
 FtpActionUpload_Set();           // placer la liste des actions à faire
 m_ActionTimer.start ( 200 );     // lancer le timer du sequenceur
}

//----------------------------------------- TelechargeFile --------------------------------------------
void C_Dlg_QGetDatasemp::TelechargeFile(const QString &action, QString &fileName, long last_version, CFtpInfo &  /*last_info*/ )
{if (m_LastVersionMedica==0) return ;
 m_CurrentAction = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 if (m_DroitsOk==0)
     { toLog("==> ::        \t: droits insuffisants pour cette action " );
       toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
       m_CurrentAction      = "";
       return;
     }
 QString    name = G_pCApp->m_PathRessources + "Dl-" +QString::number(last_version) + "-" + fileName;
 m_DownloadFile.setName( name );
 if ( m_DownloadFile.exists())
    {toLog(tr("\t\t° Le fichier : <font color=\"#fdff5a\">") + fileName + tr("</font> existe déjà, téléchargement inutile et donc interrompu.") );
     toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: "   + m_CurrentAction  + CW_EditText::fillWith ("<br>", "-",127 ));
     m_CurrentAction = "";
     return ;
    }
 if ( !m_DownloadFile.open( IO_WriteOnly ) )
    {
     toLog(tr("\t\t° ERREUR : Le fichier : <font color=\"#fdff5a\">") + fileName + tr("</font> ne peut être créé, téléchargement interrompu.") );
     toLog( "<font color=\"#ff5400\">==> :: FIN ERREUR</font>    \t: "   + m_CurrentAction  +  CW_EditText::fillWith ("<br>", "-",127 ));
     m_CurrentAction = "";
     return ;
    }
 lineEdit_Message->setText(tr("Téléchargement en cours de : ")+fileName);
 progressBar->show();
 m_pFtpTools->get( fileName, &m_DownloadFile );
}

//----------------------------------------- setNbSteepFromLaVersionDatasempLocalFile --------------------------------------------
QString C_Dlg_QGetDatasemp::setNbSteepFromLastVersionDatasempLocalFile(const QString &action, QString ident_patern)
{
 m_CurrentAction = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 QString   fileName =    GetLastDatasempNumVersionFromLocalFile(ident_patern);
 if (fileName.length()==0)
     { toLog(tr("==> ::        \t: pas de fichier .zql trouvé au niveau local." ));
       toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
       m_CurrentAction      = "";
       return QString("0");
     }
 QString nb = QString::number(GetNbProgressFromFileName(fileName));
 if (nb.toInt())
    {toLog(tr("              \t: nombre de pas : '%1' pour le fichier : '%2' ").arg(nb, fileName ));
     toLog(tr("              \t: déjà connu et déjà inscrit, calcul et inscription du nombre inutile." ));
     toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
     m_CurrentAction      = "";
    }
 else
    {toLog(tr("              \t: nombre de pas : '%1' pour le fichier : '%2'" ).arg(nb, fileName ));
     toLog(tr("              \t: intégration pour calcul et inscription du nombre en cours..." ));
     toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
     m_CurrentAction      = "";
     if (ident_patern.contains("Datasemp"))
        IntegrerFileDatasemp("INTEGRE_FILE_DATASEMP_FORCE", fileName, m_LastVersionDatasemp, C_Dlg_QGetDatasemp::forceIfSame);
     else
        IntegrerFileMedica("INTEGRE_FILE_MEDICA_FORCE",     fileName, m_LastVersionMedica,   C_Dlg_QGetDatasemp::forceIfSame );
    }
 return nb;
}

//----------------------------------------- UploaderFile --------------------------------------------
void C_Dlg_QGetDatasemp::UploaderFile(const QString &action, QString &lastFileOnServeur, QString ident_patern)
{
 m_CurrentAction  = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 QString fileName = GetLastDatasempNumVersionFromLocalFile(ident_patern);
 int pos =  fileName.find("Dump_");
 if (pos>0) fileName = fileName.mid(pos);
 if (fileName.length()==0)
     { toLog(tr("==> ::        \t: Absence de fichier à uploader " ));
       toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
       m_CurrentAction      = "";
       return;
     }
 if (lastFileOnServeur.length() && lastFileOnServeur==fileName)
     { toLog(tr("==> ::        \t: Ce fichier est déjà présent sur le serveur " ));
       toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
       m_CurrentAction      = "";
       return;
     }

 m_UploadFile.setName( G_pCApp->m_PathRessources + fileName );
 if ( ! m_UploadFile.exists())
    {toLog(tr("\t\t° Le fichier : <font color=\"#fdff5a\">") + fileName + tr("</font> n'existe pas, upload vers site distant interrompu.") );
     toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: "   + m_CurrentAction  + CW_EditText::fillWith ("<br>", "-",127 ));
     m_CurrentAction = "";
     return ;
    }
if ( ! m_UploadFile.open( IO_ReadOnly ))
    {toLog(tr("\t\t° Le fichier : <font color=\"#fdff5a\">") + fileName + tr("</font> n'a pu être ouvert, upload vers site distant interrompu.") );
     toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: "   + m_CurrentAction  + CW_EditText::fillWith ("<br>", "-",127 ));
     m_CurrentAction = "";
     return ;
    }

 lineEdit_Message->setText(tr("Téléchargement vers le serveur en cours de : ") + fileName);
 progressBar->show();
 m_pFtpTools->put(&m_UploadFile, fileName );
}

//----------------------------------------- TelechargeUserList --------------------------------------------
void C_Dlg_QGetDatasemp::TelechargeUserList(const QString &action, const QString &fileName)
{m_CurrentAction = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 QString    name = G_pCApp->m_PathRessources + fileName;
 m_DownloadFile.setName( name );
 lineEdit_Message->setText(tr("Téléchargement en cours de : ")  + fileName);
 if ( m_DownloadFile.exists())
    {toLog(tr("\t\t° EFFACEMENT : Le fichier : <font color=\"#fdff5a\">") + fileName + tr("</font> existe déjà, il va être éffacé") );
     m_DownloadFile.remove();
    }
 if ( !m_DownloadFile.open( IO_WriteOnly ) )
    {toLog(tr("\t\t° ERREUR : Le fichier : <font color=\"#fdff5a\">") + fileName + tr("</font> ne peut être créé, téléchargement interrompu.") );
     toLog( "<font color=\"#ff5400\">==> :: FIN ERREUR</font>    \t: "   + m_CurrentAction  +  CW_EditText::fillWith ("<br>", "-",127 ));
     m_CurrentAction = "";
     return ;
    }
 lineEdit_Message->setText(tr("Téléchargement en cours de : ") + fileName);
 m_pFtpTools->get( fileName, &m_DownloadFile );
}

//-------------------------------------- VerifyUserDroits -----------------------------------------------------
void C_Dlg_QGetDatasemp::VerifyUserDroits(const QString &action, const QString &fileName)
{m_CurrentAction = action;
 m_DroitsOk      = 0;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 QString userStr;
 QString    name = G_pCApp->m_PathRessources + fileName;
 CGestIni::Param_UpdateFromDisk(name, userStr);
 QFile::remove(name);
 if (m_Debug=="lesudadusel") toLog (userStr);
 int pos = userStr.find(QString("~#=")+m_DatasempKey+QString("=#~"));
 //............. tester clef secrete developpeur .......................................
 if ( QFile::exists ( QDir::homeDirPath()+"/MedinTuxRo.txt" ))
    { toLog( tr("==> ::  Succès \t: Date d'abonnement toujours valide : MEDINTUX MAGICIAN"));
      toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
      m_DroitsOk         = 1;
      m_CurrentAction    = "";
      return;
    }
 //............. tester clef secrete normale .......................................
 if (pos==-1)
    {
     QMessageBox::warning( this,        tr("Clef invalide"),
                                        tr("La clef fournie n'est pas pas valide."
                                           "<br>Veuillez vous renseigner auprès de : <b>Data Médical Design</b>"
                                           "<br>pour en obtenir une à l'adresse mail suivante : "
                                           "<br><b>roland-sevin@medintux.org</b>"),
                                        tr("&Annuler"),
                                        0, 0, 0, 0 ) ;
     toLog( tr("==> ::   Echec  \t: Clef invalide.  ") );
    }
 if (pos != -1)
    {int end = userStr.find('\n',pos);
     if (end==-1) end = userStr.find('\r',pos);
     if (end==-1) end = userStr.length();
     userStr = userStr.mid (pos,end-pos).stripWhiteSpace();
     userStr = userStr.remove('\r');
     userStr = userStr.remove('\n');
     userStr = userStr.remove('\t');
     // SEVIN_ROLAND|12-07-2007|Sevin|Roland|166 avenue françois Miterrand|13170|Les Pennes Mirabeau|France|upsis@club-internet.fr
     // USER_TEST|12-12-2007|Sevin|Roland|166 avenue françois Miterrand|13170|Les Pennes Mirabeau|France|upsis@club-internet.fr
     QStringList list = QStringList::split("|",userStr,TRUE);
     textLabel_Expiration->setText(tr("  Expire le : " + list[1]));
     if (m_Debug=="lesudadusel")
        {toLog (tr("Utilisateur  : ") + list[0].remove("~#=").remove("=#~"));
         toLog (tr("Date limite  : ") + list[1]);
         toLog (tr("Nom          : ") + list[2]);
         toLog (tr("Prénom       : ") + list[3]);
         toLog (tr("Adresse      : ") + list[4]);
         toLog (tr("Code postal  : ") + list[5]);
         toLog (tr("Ville        : ") + list[6]);
         toLog (tr("Pays         : ") + list[7]);
         toLog (tr("Mail         : ") + list[8]);
        }
     QDate currentDate = QDate::currentDate();
     QDate registrDate = QDate ( list[1].mid(6,4).toInt(), list[1].mid(3,2).toInt(), list[1].left(2).toInt() ) ;  //  12-12-2007
     if ( ! registrDate.isValid())
        { toLog( tr("==> ::  Echec  \t: format de date invalide : ") + list[1] );
        }
     if ( currentDate > registrDate)
        {  QMessageBox::warning( this,  tr("Date invalide"),
                                        tr("L'utilisateur : <b>") + list[2] + " "    + list[3] +
                                        tr("</b><br> a dépassé la date limite <b>")  + list[1] + ("</b> d'abonnement au <b>Vidal&nbsp;Datasemp</b>") +
                                        tr("<br>"
                                           "<br>Veuillez le renouvelez auprès de <b>Data&nbsp;Médical&nbsp;Design</b>"
                                           "<br>Email : <b>medintux@medintux.org</b>"),
                                        tr("&Annuler"),
                                        0, 0, 0, 0 ) ;
         toLog( tr("==> ::  Echec  \t: Date de validité dépassée : ") + list[1] );
        }
    else
        {toLog( tr("==> ::  Succès \t: Date d'abonnement valide : ")  + list[1] );
         m_DroitsOk = 1;
        }
    }
 toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
 m_CurrentAction      = "";
}

//-------------------------------------- IntegrerFileMedica -----------------------------------------------------
void C_Dlg_QGetDatasemp::IntegrerFileMedica(const QString &action, QString &fileName, long last_version, ASK_MODE askMode /*  = C_Dlg_QGetDatasemp::askIfSame*/ )
{//
 //m_CurrentAction = "";
 //return;
 m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 if (m_DroitsOk==0)
     { toLog( "==> ::        \t: droits insuffisants pour cette action." );
       toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
       m_CurrentAction      = "";
       return;
     }

 InitCMedicaBase();
 // Dl-1144686120-Dump_IndexMedica-0001-oo-1234-oo-112B02.2.sql
 // Dump_IndexMedica-0001-oo-0-oo-135B02.2.zql
 QString name                   = G_pCApp->m_PathRessources + "Dl-" + QString::number(last_version) + "-" + fileName;
 QString DatasempTelechargVers  = GetDatasempVersionFromFileName(name);
 QString DatasempInstalledVers  = GetDatasempIndexVersion();
 QString sqlDataFileName        = "";                                // donnees reelles à integrer apres eventuelle decompression
 int                         ok = 1;

 if (! DatasempTelechargVers.length() )
    {last_version           = GetLastVersionLocalDownloadFile(fileName, "Dump_IndexMedica-");   // fileName sera avec la derniere version du fichier trouvee
     name                   = G_pCApp->m_PathRessources + fileName;
     DatasempTelechargVers  = GetDatasempVersionFromFileName(name);
    }
 if (DatasempTelechargVers.length() && DatasempTelechargVers==DatasempInstalledVers)
    {if (askMode == C_Dlg_QGetDatasemp::forceIfSame)
        ok = 1;
     else
        ok =  QMessageBox::warning( this,  tr("Installation du fichier téléchargé"),
                                        tr("La version téléchargée de la base de données : <br><b>") +fileName +
                                           "</b> Version : <b>"     +  DatasempTelechargVers +
                                        tr("</b>\r<br> est identique à  celle déjà en place. "
                                           "<br>\r\n faut-il forcer l'installation ?<br>\r\n "),
                                        tr("&Annuler"),
                                        tr("&Forcer la mise à jour"), 0, 0, 0 ) ;
    }
 if (!QFile::exists ( name ))  // on peut avoir un truc du type : Dl-0-DumpToutCeQuiEstApresDumpEstValide
    {int pos = name.find("Dump_");
     if (pos != -1) name = G_pCApp->m_PathRessources + name.mid(pos);
    }
 //name = "/home/ro/MedinTux-V2.14-Synth-src/Programmes/qgetdatasemp/bin/Ressources/Dump_IndexMedica-0001-oo-0-oo-174A02.2.sql";
 if (ok)
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     int pos         = 0;
     sqlDataFileName = name;
     if (sqlDataFileName.endsWith(".zql"))           // si fichier coompresse le decompresser et chager nom de sqlDataFileName
        {QString f_nametemp = sqlDataFileName;
         pos                = sqlDataFileName.find(".zql");
         if (pos != -1)
            {f_nametemp.truncate(pos);
             sqlDataFileName.truncate(pos);
             sqlDataFileName += ".sql";
            }
         if (!QFile::exists ( sqlDataFileName ))
            {pos  = f_nametemp.findRev(QDir::separator());
             if (pos != -1) f_nametemp = f_nametemp.mid(pos + 1);
             toLog(tr("==> ::        \t: Décompression de : <font color=\"#fdff5a\">" ) + f_nametemp + ".zql</font>  en cours ...(patientez)...");
             G_pCApp->Decompresse( G_pCApp->m_PathRessources + f_nametemp);
             toLog(tr("==> ::        \t: Décompression de : <font color=\"#fdff5a\">" ) + f_nametemp + tr(".zql</font>  terminée."));
            }
        }
     lineEdit_Message->setText(tr(" Intégration de : ") + fileName + tr(" en cours... "));
     toLog( tr("==> ::        \t: Intégration de : <font color=\"#fdff5a\">")   + fileName + tr("</font> en cours... "));
     bool isOpen =                  m_pCMedicaBase->m_MedicaBase->isOpen();
     if (isOpen == FALSE) isOpen =  m_pCMedicaBase->m_MedicaBase->open();
     if (isOpen)
        {long nb = GetNbProgressFromFileName(fileName);
         if (nb)
           {progressBar->setTotalSteps(nb/1000);
            progressBar->show();
           }
          else
           {progressBar->hide();
           }
         G_pCApp->SetProgressPosition(0);
         G_pCApp->ParseSQL_Dump(m_pCMedicaBase->m_MedicaBase, sqlDataFileName, progressBar, cW_EditText_Log );
         SetDatasempIndexVersion(DatasempTelechargVers);
         nb = G_pCApp->GetProgressPosition()*1000;
         toLog( tr("==> ::        \t: Intégration de : <font color=\"#fdff5a\">")     + fileName + tr("</font> terminée."));
         toLog( tr("\t\t° Nombre de lignes intégrées : \t\t<font color=\"#fdff5a\">") + QString::number(nb) + "</font>" );
         //................ placer le nombre de lignes integrees dans les noms de fichiers ................
         toLog( tr("° Inscription du nombre de lignes dans le nom des fichiers") );
         sqlDataFileName = SetFileNameToNbProgress(sqlDataFileName, nb);                           // fichier sql
         if (name.endsWith(".zql"))  name = SetFileNameToNbProgress(name, nb);                     // fichier zql
         progressBar->hide();
        }
     if (name.endsWith(".zql") && QFile::exists ( sqlDataFileName ))
        {QFile::remove(sqlDataFileName);
         toLog( tr("\t\t° Effacement de : <font color=\"#fdff5a\">") + sqlDataFileName + "</font>");
        }
     lineEdit_Message->setText(tr(" Intégration de : ") + fileName + tr(" terminée."));
     QApplication::restoreOverrideCursor();
    }
 else
     {toLog(tr("° Opération d'intégration annulée par l'utilisateur."));
     }
 toLog( tr("\t\t° Version du fichier MedicaTux téléchargé : \t\t<font color=\"#fdff5a\">") + DatasempTelechargVers + "</font>");
 toLog( tr("\t\t° Version de la base MedicaTux en cours   : \t\t<font color=\"#fdff5a\">") + DatasempInstalledVers + "</font>");
 toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
 m_CurrentAction      = "";
}

//-------------------------------------- IntegrerFileDatasemp -----------------------------------------------------
void C_Dlg_QGetDatasemp::IntegrerFileDatasemp(const QString &action, QString &fileName, long last_version, ASK_MODE askMode /*  = C_Dlg_QGetDatasemp::askIfSame*/)
{m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 if (m_DroitsOk==0)
     { toLog("==> ::        \t: droits insuffisants pour cette action " );
       toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
       m_CurrentAction      = "";
       return;
     }
 InitCMedicaBase();
 // Dl-1144686120-Dump_Datasemp-0001-oo-1234-oo-112B02.2.sql
 // Dump_Datasemp-0001-oo-0-oo-135B02.2.zql

 QString sqlDataFileName        = "";                                // donnees reelles à integrer apres eventuelle decompression
 QString name                   = G_pCApp->m_PathRessources + "Dl-" +QString::number(last_version) + "-" + fileName;
 QString DatasempTelechargVers  = GetDatasempVersionFromFileName(name);
 QString DatasempInstalledVers  = "";
 int                         ok = 1;
 QSqlDatabase    *pQSqlDatabase = m_pCMedicaBase->m_DataBaseSrce;

 if (pQSqlDatabase==0)  DatasempInstalledVers = "";                         // car existe dans base Get meme si Datasemp non installé
 else                   DatasempInstalledVers = GetDatasempDataVersion();   // fonctionne meme si pQSqlDatabase==0

 if ( !DatasempTelechargVers.length() )
    {last_version           = GetLastVersionLocalDownloadFile(fileName, "Dump_Datasemp-"); // fileName sera avec la derniere version du fichier trouvee
     name                   = G_pCApp->m_PathRessources + fileName;
     DatasempTelechargVers  = GetDatasempVersionFromFileName(name);        // pour l'integration
    }
 if (DatasempTelechargVers.length() && DatasempTelechargVers==DatasempInstalledVers)
    {if (askMode == C_Dlg_QGetDatasemp::forceIfSame)
        ok = 1;
     else
        ok =  QMessageBox::warning( this,  tr("Installation du fichier téléchargé"),
                                        tr("La version téléchargée de la base de données : <br><b>") +fileName +
                                           "</b> Version : <b>"     +  DatasempTelechargVers +
                                        tr("</b>\r<br> est identique à  celle déjà en place. "
                                           "<br>\r\n faut-il forcer l'installation ?<br>\r\n "),
                                        tr("&Annuler"),
                                        tr("&Forcer la mise à jour"), 0, 0, 0 ) ;
    } //endif (DatasempTelechargVers==DatasempInstalledVers)
  if (!QFile::exists ( name ))  // on peut avoir un truc du type : Dl-0-DumpToutCeQuiEstApresDumpEstValide
     {int pos = name.find("Dump_");
      if (pos != -1) name = G_pCApp->m_PathRessources + name.mid(pos);
     }
 if (ok)
    {while (pQSqlDatabase==0 )
       {bool isOpen =                  m_pCMedicaBase->m_MedicaBase->isOpen();
        if (isOpen == FALSE) isOpen =  m_pCMedicaBase->m_MedicaBase->open();
        QString driverName, baseName, userName, passWord, hostName, port, local_param;
        QSqlQuery query(QString::null, m_pCMedicaBase->m_MedicaBase);
        toLog(tr("La base Datasemp n'existe pas, Création de la base : DatasempTest"));
        query.exec("CREATE DATABASE DatasempTest;");
        G_pCApp->OutSQL_error( query, tr("IntegrerFileDatasemp() : echec de la création base DataSemp : "), "CREATE DATABASE DatasempTest;");
        CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathRessources + "MedicaBase.ini", local_param);
        if (CGestIni::Param_ReadParam(  local_param, "Connexion",  "Parametres SrceBase",
                                        &driverName, &baseName, &userName, &passWord, &hostName, &port) == 0
           )
           {pQSqlDatabase =  G_pCApp->BaseConnect(driverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                                  baseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                                  userName,     // = "root"
                                                  passWord,     // = ""
                                                  hostName,     // = "localhost"
                                                  port,
                                                  "DATASEMP_BASE");
           } //endif (CGestIni::Param_ReadParam()==0
        } //endif (pQSqlDatabase==0)
     } //endif (m_pCMedicaBase->m_DataBaseSrce==0)
 else
     {toLog(tr("° Opération d'intégration annulée par l'utilisateur."));
     }
 if (ok && pQSqlDatabase)
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     sqlDataFileName = name;
     int pos        = 0;
     if (sqlDataFileName.endsWith(".zql"))
        {QString f_nametemp = sqlDataFileName;
         pos                = sqlDataFileName.find(".zql");
         if (pos != -1)
            {f_nametemp.truncate(pos);
             sqlDataFileName.truncate(pos);
             sqlDataFileName += ".sql";
            }
         if (!QFile::exists ( sqlDataFileName ))
            {pos  = f_nametemp.findRev(QDir::separator());
             if (pos != -1) f_nametemp = f_nametemp.mid(pos + 1);
             toLog(tr("==> ::        \t: Décompression de : <font color=\"#fdff5a\">" ) + f_nametemp + ".zql</font>  en cours ...(patientez)...");
             G_pCApp->Decompresse( G_pCApp->m_PathRessources + f_nametemp);
             toLog(tr("==> ::        \t: Décompression de : <font color=\"#fdff5a\">" ) + f_nametemp + tr(".zql</font>  terminée."));
            }
        }

     lineEdit_Message->setText(tr(" Intégration de : ") + fileName + tr(" en cours... "));
     toLog( tr("==> ::        \t: Intégration de : <font color=\"#fdff5a\">") + fileName + tr("</font> en cours... "));
     bool isOpen =                  m_pCMedicaBase->m_MedicaBase->isOpen();
     if (isOpen == FALSE) isOpen =  m_pCMedicaBase->m_MedicaBase->open();
     if (isOpen)
        { long nb = GetNbProgressFromFileName(fileName);
          if (nb)
            {progressBar->setTotalSteps(nb/1000);
             progressBar->show();
            }
          else
            {progressBar->hide();
            }
          G_pCApp->SetProgressPosition(0);
          G_pCApp->ParseSQL_Dump(pQSqlDatabase, sqlDataFileName, progressBar, cW_EditText_Log );
          SetDatasempDataVersion(DatasempTelechargVers);
          nb = G_pCApp->GetProgressPosition()*1000;
          toLog( tr("==> ::        \t: Intégration de : <font color=\"#fdff5a\">")   + fileName + tr("</font> terminée."));
          toLog( tr("\t\t° Nombre de lignes intégrées : \t\t<font color=\"#fdff5a\">") + QString::number(nb)  + "</font>");
          //................ placer le nombre de lignes integrees dans les noms de fichiers ................
          toLog( tr("° Inscription du nombre de lignes dans le nom des fichiers.") );
          sqlDataFileName = SetFileNameToNbProgress(sqlDataFileName, nb);                           // fichier sql
          if (name.endsWith(".zql"))  name = SetFileNameToNbProgress(name, nb);                     // fichier zql
          progressBar->hide();
        }
     if (name.endsWith(".zql") && QFile::exists ( sqlDataFileName ))
        {QFile::remove(sqlDataFileName);
         toLog( tr("\t\t° Effacement de : <font color=\"#fdff5a\">") + sqlDataFileName  + "</font>");
        }
    lineEdit_Message->setText(tr(" Intégration de : ") + fileName + tr(" terminée."));
    QApplication::restoreOverrideCursor();
   }
 toLog(tr("\t\t° Version du fichier Datasemp téléchargé : \t\t<font color=\"#fdff5a\">") + DatasempTelechargVers + "</font>");
 toLog(tr("\t\t° Version de la base Datasemp en cours   : \t\t<font color=\"#fdff5a\">") + DatasempInstalledVers + "</font>");
 toLog("<font color=\"#ff5400\">==> :: FIN</font>    \t: "    + m_CurrentAction + "<br>" + CW_EditText::fillWith ("", "-",127 ));
 m_CurrentAction      = "";
}

//-------------------------------------- GetNbProgressFromFileName -----------------------------------------------------
//................................ règles de nommage des fichiers de mise à jour ................................
// Dl-1144774740-Dump_Datasemp-0001-oo-1234-oo-112B02.2.sql
// ^  ^          ^                     ^       ^_____________ Datasemp version
// |  |          |_____ Ident Patern   |___ nb progress
// |  |                 Le nom du fichier sur le FTP commence ici
// |  |________ DateTime from FTP upload
// |__________________ Le nom du fichier téléchargé commence ici (précédé de 'Dl-1144774740-')
//                                                                            ^
//                                                                            |________ DateTime from FTP upload
//
long  C_Dlg_QGetDatasemp::GetNbProgressFromFileName(const QString &fileName)
{// Dl-1144686120-Dump_Datasemp-0001-oo-1234-oo-112B02.2.sql
 int pos_deb = fileName.find("-oo-");
 if (pos_deb == -1) return 0;
 pos_deb    += 4;
 int pos_end = fileName.find("-oo-", pos_deb);
 if (pos_end == -1) return 0;
 return( fileName.mid(pos_deb, pos_end - pos_deb)).toLong();
}



//-------------------------------------- SetFileNameToNbProgress -----------------------------------------------------
QString  C_Dlg_QGetDatasemp::SetFileNameToNbProgress(QString filename, long nb)
{if (nb==0)          return filename;
 QDir dir(filename);
 QString new_name = filename;
 int pos_deb      = new_name.find("-oo-");
 if (pos_deb == -1)  return filename;
 pos_deb    += 4;
 int pos_end      = new_name.find("-oo-", pos_deb);
 if (pos_end == -1)  return filename;
 new_name.replace(pos_deb, pos_end - pos_deb, QString::number(nb));
 dir.rename ( filename, new_name);
 return new_name;
}

//----------------------------------------- RecupererLastFTP_VersionFile --------------------------------------------
long C_Dlg_QGetDatasemp::RecupererLastFTP_VersionFile(const QString &action, QString &fileName, const QString &ident_patern, CFtpInfo &last_info)
{m_CurrentAction      = action;
 toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
 uint version      = 0;
 uint last_version = 0;
 QString name      = "";
 int           len = ident_patern.length();
 QDateTime date;
 QVL_FileInfo::iterator it;
 for ( it     = m_QVL_FileInfo.begin(); it != m_QVL_FileInfo.end(); ++it )
     {name    = (*it).name(); //Dump_IndexMedica-oo-1234-oo-112B02.2.sql Dump_IndexMedica-oo-1234-oo-112B02.2.zql
      if (name.left(len) == ident_patern)
         {version = (*it).lastModified().toTime_t();
          if (version>last_version)
             {last_version = version;
              fileName     = name;
              date         = (*it).lastModified();
              last_info    = (*it);
             }
         }
     }
 if (last_version) toLog(tr("\t\tDernierFichier : \t\t<font color=\"#fdff5a\">") + fileName + tr("</font> Daté du : ") + date.toString("dd/MM/yyyy hh:mm"));
 toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
 m_CurrentAction      = "";
 return version;
}
//-------------------------------------- GetLastDatasempNumVersionFromLocalFile -----------------------------------------------------
// ACTION : retourne le fichier identifie par le patern d'entree dont la version datasemp est la plus recente
//          QString ident_patern   : motif d'identification du fichier dont on recherche la version la plus recente
//                                  ident_patern = "Dump_IndexMedica-"        fichiers d'index
//                                  ident_patern = "Dump_Datasemp-"           fichiers de donnees

QString  C_Dlg_QGetDatasemp::GetLastDatasempNumVersionFromLocalFile(QString ident_patern)
{   QDir dir ( G_pCApp->m_PathRessources  );
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    int           pos = -1;
    //if (ident_patern.left(1) != "-") ident_patern.prepend("-"); // ds fichier downloaded le patern est précédé par '-'
    //................................ règles de nommage des fichiers de mise à jour ................................
    // Dump_IndexMedica-0001-oo-0-oo-135B02.2.zql
    // Dump_Datasemp-0001-oo-0-oo-135B02.2.zql

    // Dump_IndexMedica-0001-oo-14213000-oo-165A02.2.zql
    // Dump_Datasemp-0001-oo-711119000-oo-165A02.2.zql

    // Dl-1144774740-Dump_IndexMedica-0001-oo-1234-oo-112B02.2.zql
    // Dl-1144774740-Dump_Datasemp-0001-oo-1234-oo-112B02.2.zql
    // ^  ^          ^                     ^       ^_____________ Datasemp version
    // |  |          |_____ Ident Patern   |___ nb progress
    // |  |                 Le nom du fichier sur le FTP commence ici
    // |  |________ DateTime from FTP upload
    // |__________________ Le nom du fichier téléchargé commence ici (précédé de 'Dl-1144774740-')
    //                                                                            ^
    //                                                                            |________ DateTime from FTP upload
    //
    //............ exploration de la liste ..........................................................................
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    QString version     = "";
    QString lastVersion = "";
    QString f_Name      = "";
    while ( (fi = it.current()) != 0 )
          {QString fname = fi->fileName().latin1();
           if      (fname == "..")                   { }
           else if (fi->isDir() && fname != "." )    { }
           else if (fi->isFile() && fi->extension(FALSE)=="zql" &&
                    (fname.left(3)==("Dl-") || fname.left(4)==("Dump")) &&
                     (pos = fname.find(ident_patern)) != -1
                   )
                   {version = GetDatasempVersionFromFileName(fname);
                    if (version>lastVersion)
                       {f_Name      = fname;
                        lastVersion = version;
                       }
                   }
           ++it;
          }
   //............................. si fichier trouvé alors le placer ....................
   return f_Name;
}

//-------------------------------------- GetLastVersionLocalDownloadFile -----------------------------------------------------
// ACTION : retourne la version en terme de temps (long) du fichier le plus recent identifie par le patern d'entree
// ENTREE : QString &fileName      : il y sera place le nom du fichier le plus recent retrouve en local.
//          QString ident_patern   : motif d'identification du fichier dont on recherche la version la plus recente
//                                  ident_patern = "Dump_IndexMedica-"        fichiers d'index
//                                  ident_patern = "Dump_Datasemp-"           fichiers de donnees

long  C_Dlg_QGetDatasemp::GetLastVersionLocalDownloadFile(QString &fileName, QString ident_patern)
{   QDir dir ( G_pCApp->m_PathRessources  );
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    int           pos = -1;
    uint version      = 0;
    uint last_version = 0;
    QString f_Name    = "";
    //if (ident_patern.left(1) != "-") ident_patern.prepend("-"); // ds fichier downloaded le patern est précédé par '-'
    //................................ règles de nommage des fichiers de mise à jour ................................
    // Dump_IndexMedica-0001-oo-0-oo-135B02.2.zql
    // Dump_Datasemp-0001-oo-0-oo-135B02.2.zql

    // Dump_IndexMedica-0001-oo-14213000-oo-165A02.2.zql
    // Dump_Datasemp-0001-oo-711119000-oo-165A02.2.zql

    // Dl-1144774740-Dump_IndexMedica-0001-oo-1234-oo-112B02.2.zql
    // Dl-1144774740-Dump_Datasemp-0001-oo-1234-oo-112B02.2.zql
    // ^  ^          ^                     ^       ^_____________ Datasemp version
    // |  |          |_____ Ident Patern   |___ nb progress
    // |  |                 Le nom du fichier sur le FTP commence ici
    // |  |________ DateTime from FTP upload
    // |__________________ Le nom du fichier téléchargé commence ici (précédé de 'Dl-1144774740-')
    //                                                                            ^
    //                                                                            |________ DateTime from FTP upload
    //
    //............ exploration de la liste ..........................................................................
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
          {QString fname = fi->fileName().latin1();
           if      (fname == "..")                   { }
           else if (fi->isDir() && fname != "." )    { }
           else if (fi->isFile() && fi->extension(FALSE)=="zql" &&
                    (fname.left(3)==("Dl-") || fname.left(4)==("Dump")) &&
                     (pos = fname.find(ident_patern)) != -1
                   )
                   {
                    if (fname.left(3)=="Dl-")   version = fname.mid(3,pos-4).toUInt();
                    else                        version = fi->lastModified().toTime_t();
                    if (version > last_version)
                       {last_version = version;
                        f_Name       = fname;
                       }
                   }
           ++it;
          }
   //............................. si fichier trouvé alors le placer ....................
   if (f_Name.length()) fileName = f_Name;
   return last_version;
}



//-------------------------------------- GetDatasempVersionFromFileName -----------------------------------------------------
QString C_Dlg_QGetDatasemp::GetDatasempVersionFromFileName(QString name)
{// Dl-1144686120-Dump_IndexMedica-0001-112B02.2.sql
 int pos_tiret = name.findRev('-');
 int pos_point = name.findRev('.');
 if (pos_tiret == -1||pos_point == -1) return QString::null;
 return name.mid(pos_tiret+1, pos_point-pos_tiret-1);
}

//-------------------------------------- ListerContenu -----------------------------------------------------
void C_Dlg_QGetDatasemp::ListerContenu(const QString &action)
{   m_CurrentAction      = action;
    toLog("<font color=\"#ff5400\">==> :: DEBUT</font>  \t: " + m_CurrentAction );
    toLog(tr("\t\tEnvoi de la commande Listing fichiers"));
    m_QVL_FileInfo.clear();
    m_pFtpTools->list();
    return;
}

//----------------------------------------- slotFTP_state_changed --------------------------------------------
void C_Dlg_QGetDatasemp::slotFTP_state_changed(int state)
{switch ((CFtp::State)state)
   {case CFtp::Unconnected: toLog(tr("\t\tRéponse FTP : \t\tDeconnecté"));           break;
    case CFtp::HostLookup:  toLog(tr("\t\tRéponse FTP : \t\tRecherche de l'hote"));  break;
    case CFtp::Connecting:  toLog(tr("\t\tRéponse FTP : \t\tConnexion en cours"));   break;
    case CFtp::Connected:   toLog(tr("\t\tRéponse FTP : \t\tConnecté"));             break;
    case CFtp::LoggedIn:    toLog(tr("\t\tRéponse FTP : \t\tUtilisateur connecté")); break;
    case CFtp::Closing:     toLog(tr("\t\tRéponse FTP : \t\tFermeture connexion"));  break;
   }
}

//--------------------------------------- slot_LlistInfo() ------------------------------------------
// slot_ appellé lorsque le FTP liste un repertoire, pour chaque objet fichier rencontré
void C_Dlg_QGetDatasemp::slot_LlistInfo(const QUrlInfo &objet)
{   CFtpInfo info  = objet;
    QString fname  = objet.name();
    QString espace = "";
    QString taille = QString::number(objet.size());
    QString date   = objet.lastModified().toString("dd/MM/yyyy hh:mm");
    // si c'est un fichier
    if ( m_DroitsOk && objet.isFile() )
       {if (m_Debug.length())                       toLog( tr("\t\t -- Fichier    : <font color=\"#fdff5a\">") + fname + espace.fill(' ',6 - taille.length()) + taille + tr("</font> Octets, du ") + date);
        else  if (fname.right(3).lower()=="sql")    toLog( tr("\t\t -- Fichier    : <font color=\"#fdff5a\">") + fname + espace.fill(' ',6 - taille.length()) + taille + tr("</font> Octets, du ") + date);
        else  if (fname.right(3).lower()=="zql")    toLog( tr("\t\t -- Fichier    : <font color=\"#fdff5a\">") + fname + espace.fill(' ',6 - taille.length()) + taille + tr("</font> Octets, du ") + date);
        m_QVL_FileInfo.append(info);
       }
}

//----------------------------------------- toLog --------------------------------------------
void C_Dlg_QGetDatasemp::toLog(const QString &str)
{G_pCApp->Datasemp_OutMessage( cW_EditText_Log, str);
}

//----------------------------------------- slotFTP_Started --------------------------------------------
// ACTION : Appelé chaque fois que le FTP démarre une commande
//          là on place m_CurrentAction sur une valeur de façon à bloquer tout autre autre action.
//          lorque l'action en cours se termine alors FTP appelle slotFTP_Finished() qui replace
//          m_CurrentAction  sur vide de façon à autoriser les autres  actions FTP
//          façon de faire du séquentiel sur des routines asynchrones.

void C_Dlg_QGetDatasemp::slotFTP_Started(int id)
{   m_CurrentAction      = tr ("ACTION : ");
    COMMAND *command     = new COMMAND;                                                   // on créé un pointeur sur la commande
    command->m_Command   = commandToString(m_pFtpTools->currentCommand());                // on récupère le libellé de la commande en cours
    m_CurrentAction     +=  command->m_Command;
    command->m_dateDebut = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"); // on récupère la date de debut
    toLog("==> "+ QString::number(id) +tr(" Debut  \t: ") + command->m_dateDebut + " : " + command->m_Command );
    m_CommandMap.insert(id, command);
}
//----------------------------------------- setProgress --------------------------------------------
// ACTION : Appelé chaque fois que le FTP démarre une commande de transfert de données
void C_Dlg_QGetDatasemp::setProgress(int progress, int totalSteps)
{if (m_CurrentAction.mid(9) == commandToString(CFtp::Put) || m_CurrentAction.mid(9) == commandToString(CFtp::Get))
    {progressBar->setProgress ( progress, totalSteps );
    }
}

//----------------------------------------- slotFTP_Finished --------------------------------------------
// Appelé chaque fois que le FTP vient de finir une commande
//          là on place m_CurrentAction sur une valeur vide de façon à autoriser les autres actions.

void C_Dlg_QGetDatasemp::slotFTP_Finished(int id, bool error)
{
    COMMAND *command = m_CommandMap.find(id).data(); // recuperation de la commande
    command->m_dateFin = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    // si une erreur c'est produite
    if (error) command->m_Resultat  = m_pFtpTools->errorString();
    else       command->m_Resultat  = errorToString(m_pFtpTools->error());

    int pos = command->m_Resultat.find("host failed");
    if (pos != -1) {command->m_Resultat = command->m_Resultat.left(pos + 11) + tr(", vérifiez votre connexion internet FTP");};
    // on écrit dans le fichier journal
    toLog("==> "+ QString::number(id) +tr(" Fin  \t: ") + command->m_dateFin + " : " + command->m_Resultat + " : " + command->m_nomFichier);
    //.......... m_CurrentAction = "ACTION : Téléchargement vers Client" (get)............................
    //           faut fermer le fichier qui avait été ouvert au début de l'action
    if (m_CurrentAction.mid(9) == commandToString(CFtp::Get))
       {m_DownloadFile.close();
        lineEdit_Message->setText(tr("Téléchargement de : <font color=\"#fdff5a\">") + command->m_nomFichier + tr("</font> terminé "));
        progressBar->hide();
       }
    if (m_CurrentAction.mid(9) == commandToString(CFtp::Put))
       {m_UploadFile.close();
        lineEdit_Message->setText(tr("Upload de : <font color=\"#fdff5a\">") + command->m_nomFichier + tr("</font> terminé "));
        progressBar->hide();
       }
    // on efface la référence de la commande dans le QMap
    m_CommandMap.erase(id);
    delete command;
    toLog( "<font color=\"#ff5400\">==> :: FIN</font>    \t: " + m_CurrentAction  + "<br>" + CW_EditText::fillWith ("", "-",127 ));
    m_CurrentAction      = "";
}

//-----------------------------------------------------  pushButtonLocal_clicked --------------------------------------------------------
void C_Dlg_QGetDatasemp::pushButtonLocal_clicked()
{//...................sauvegarder les valeurs .................................
 CApp::GotoDebug();
 Save_Param();
 //.................. relancer le sequenceur ..........................................
 SetIniFileOnDatasemp();          // placer le fichier ini sur le VidalDatasemp
 ResetSequenceur();               // remettre à zero le sequenceur d'actions
 LocalActionList_Set();           // placer la liste des actions à faire
 m_ActionTimer.start ( 200 );     // lancer le timer du sequenceur
}

//----------------------------------------- pushButton_Connexion_clicked --------------------------------------------
void C_Dlg_QGetDatasemp::pushButton_Connexion_clicked()
{if (m_pFtpTools == 0) return;
 //...................sauvegarder les valeurs .................................
 Save_Param();
 //.................. relancer le sequenceur ..........................................
 SetIniFileOnDatasemp();          // placer le fichier ini sur le VidalDatasemp
 ResetSequenceur();               // remettre à zero le sequenceur d'actions
 FtpActionList_Set();             // placer la liste des actions à faire
 m_ActionTimer.start ( 200 );     // lancer le timer du sequenceur
}

//----------------------------------------- pushButton_ComputeClefAcces_clicked --------------------------------------------
void C_Dlg_QGetDatasemp::pushButton_ComputeClefAcces_clicked()
{QString dataFtpAccessKey = lineEdit_AccessKey->text().stripWhiteSpace();
 if (m_Debug=="lesudadusel")
    { toLog( "<font color=\"#ff5400\">==> :: DEBUG lineEdit_AccessKey(1) </font>    \t: " + dataFtpAccessKey );
    }
 //if (dataFtpAccessKey    != m_DefaultStringClefAcces)
    {//.............. si des données sont presentes alors on s'en sert pour renseigner les champs du ftp .............
     if (dataFtpAccessKey.length() && dataFtpAccessKey != m_DefaultStringClefAcces)
        {
         QString dataSempKey = "";
         int deb = dataFtpAccessKey.find("X");
         if (deb != -1)
            {int end = dataFtpAccessKey.find("Q");
             if (end != -1)
                {++deb;
                 dataSempKey = dataFtpAccessKey.mid(deb, end-deb).stripWhiteSpace();
                  if (m_Debug=="lesudadusel")
                     { toLog( "<font color=\"#ff5400\">==> :: DEBUG dataSempKey(1) </font>    \t: " + dataSempKey );
                     }
                }
             --deb;
             dataFtpAccessKey = dataFtpAccessKey.remove(deb, end-deb);
             if (m_Debug=="lesudadusel")
                { toLog( "<font color=\"#ff5400\">==> :: DEBUG dataFtpAccessKey(1) </font>    \t: " + dataFtpAccessKey );
                }
            }
         dataFtpAccessKey.replace('P','|');    //     --> P m_FtpAdress
         dataFtpAccessKey.replace('W','|');    //  P <--> W m_FtpPort
         dataFtpAccessKey.replace('Q','|');    //  X <--> Q m_DatasempKey
         dataFtpAccessKey.replace('O','|');    //  Q <--> O m_FtpLogName
                                               //  O <---   m_FtpLogPass
         if (m_Debug=="lesudadusel")
            { toLog( "<font color=\"#ff5400\">==> :: DEBUG dataFtpAccessKey </font>    \t: " + dataFtpAccessKey );
            }
         QStringList lst = QStringList::split('|' , dataFtpAccessKey, TRUE);
         if (lst[0].length() && lst[1].length()==8 && lst[2].length() && lst[3].length() && lst[4].length())
            {m_FtpAdress   = lst[0];
             m_FtpPort     = lst[1];
             m_FtpFolder   = lst[2];
             m_FtpLogName  = lst[3];
             m_FtpLogPass  = lst[4];
             m_FtpAdress   = PassWordDecode(m_FtpAdress);
             m_FtpPort     = PassWordDecode(m_FtpPort);
             m_FtpFolder   = PassWordDecode(m_FtpFolder);
             m_FtpLogName  = PassWordDecode(m_FtpLogName);
             m_FtpLogPass  = PassWordDecode(m_FtpLogPass);
             m_DatasempKey = PassWordDecode(dataSempKey);

             lineEdit_FtpAdress->setText(m_FtpAdress);
             lineEdit_FtpPort->setText(m_FtpPort);
             lineEdit_FtpRepertoire->setText(m_FtpFolder);
             lineEdit_LogName->setText(m_FtpLogName);
             lineEdit_LogPass->setText(m_FtpLogPass);
             if (m_Debug=="lesudadusel")
                { toLog( "<font color=\"#ff5400\">==> :: DEBUG m_FtpAdress </font>     \t: " + m_FtpAdress  + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG m_FtpPort </font>       \t: " + m_FtpPort    + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG m_FtpFolder </font>     \t: " + m_FtpFolder  + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG m_FtpLogName </font>    \t: " + m_FtpLogName + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG m_FtpLogPass </font>    \t: " + m_FtpLogPass + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG PassWordDecode(dataSempKey) </font>     \t: " + PassWordDecode(dataSempKey)  + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG dataSempKey </font>     \t: " + dataSempKey  + "" );
                  toLog( "<font color=\"#ff5400\">==> :: DEBUG m_DatasempKey </font>     \t: " + m_DatasempKey  + "" );
                }
             if (dataSempKey.length())
                {m_DatasempKey = PassWordDecode(dataSempKey);
                 lineEdit_UserName->setText(m_DatasempKey);
                 if (m_Debug=="lesudadusel")
                    { toLog( "<font color=\"#ff5400\">==> :: DEBUG PassWordDecode(dataSempKey) </font>     \t: " + PassWordDecode(dataSempKey)  + "" );
                      toLog( "<font color=\"#ff5400\">==> :: DEBUG dataSempKey </font>       \t: " + dataSempKey  + "" );
                      toLog( "<font color=\"#ff5400\">==> :: DEBUG m_DatasempKey </font>     \t: " + m_DatasempKey  + "" );
                    }
                }
            }
        }
     else
        {lineEdit_AccessKey->setText(PassWordEncode( m_FtpAdress)   + "P" +
                                     PassWordEncode( m_FtpPort)     + "W" +
                                     PassWordEncode( m_FtpFolder)   + "Q" +
                                     PassWordEncode( m_FtpLogName)  + "O" +
                                     PassWordEncode( m_FtpLogPass)
                                    );
        if (m_Debug=="lesudadusel")
            { toLog( "<font color=\"#ff5400\">==> :: DEBUG lineEdit_AccessKey </font>    \t: '" + lineEdit_AccessKey->text() + "'");
            }

        }
    }
}

//----------------------------------------- Save_Param --------------------------------------------
void C_Dlg_QGetDatasemp::Save_Param()
{m_FtpAdress              = lineEdit_FtpAdress->text().stripWhiteSpace();
 m_FtpPort                = lineEdit_FtpPort->text().stripWhiteSpace();
 m_FtpFolder              = lineEdit_FtpRepertoire->text().stripWhiteSpace();
 m_FtpLogName             = lineEdit_LogName->text().stripWhiteSpace();
 m_FtpLogPass             = lineEdit_LogPass->text().stripWhiteSpace();
 m_DatasempKey            = lineEdit_UserName->text().stripWhiteSpace();
 QString dataFtpAccessKey = lineEdit_AccessKey->text().stripWhiteSpace().remove('X').remove(' ');

 if (dataFtpAccessKey != m_DefaultStringClefAcces)
    { QString dataSempKey = "";
      int deb = dataFtpAccessKey.find("X");
      if (deb != -1)
         {int end = dataFtpAccessKey.find("Q");
          if (end != -1)
             {++deb;
              dataSempKey = dataFtpAccessKey.mid(deb, end-deb).stripWhiteSpace();
             }
          --deb;
          dataFtpAccessKey = dataFtpAccessKey.remove(deb,end-deb);
         }
      dataFtpAccessKey.replace('P','|');    //     --> P m_FtpAdress
      dataFtpAccessKey.replace('W','|');    //  P <--> W m_FtpPort
      dataFtpAccessKey.replace('Q','|');    //  X <--> Q m_DatasempKey
      dataFtpAccessKey.replace('O','|');    //  Q <--> O m_FtpLogName
                                            //  O <---   m_FtpLogPass
      QStringList lst = QStringList::split('|' , dataFtpAccessKey, TRUE);
      if (lst[0].length() && lst[1].length()==8 && lst[2].length() && lst[3].length() && lst[4].length())
         {m_FtpAdress   = lst[0];
          m_FtpPort     = lst[1];
          m_FtpFolder   = lst[2];
          m_FtpLogName  = lst[3];
          m_FtpLogPass  = lst[4];
          m_FtpAdress   = PassWordDecode(m_FtpAdress);
          m_FtpPort     = PassWordDecode(m_FtpPort);
          m_FtpFolder   = PassWordDecode(m_FtpFolder);
          m_FtpLogName  = PassWordDecode(m_FtpLogName);
          m_FtpLogPass  = PassWordDecode(m_FtpLogPass);
          if (dataSempKey.length())
             {m_DatasempKey = PassWordDecode(dataSempKey);
              lineEdit_UserName->setText(m_DatasempKey);
             }
         }
    }
 //................ si clef est moimaide on ne l'enregistre pas dans le ini .........................
 QString datasempkey;
 if (m_DatasempKey=="moimaide") datasempkey = "";
 else                           datasempkey =  m_DatasempKey;
//.................. sauvegarder les paramètres FTP ...........................
 CGestIni::Param_WriteParam(&G_pCApp->m_ParamData, "Authentification", "clef",
                                                           PassWordEncode( m_FtpAdress)   + "P" +
                                                           PassWordEncode( m_FtpPort)     + "W" +
                                                           PassWordEncode( m_FtpFolder)   + "X" +
                                                           PassWordEncode( datasempkey)   + "Q" +
                                                           PassWordEncode( m_FtpLogName)  + "O" +
                                                           PassWordEncode( m_FtpLogPass) );
 CGestIni::Param_UpdateToDisk( G_pCApp->m_ParamPath, G_pCApp->m_ParamData);
}

//------------------------------------- commandToString() ---------------------------------------
QString C_Dlg_QGetDatasemp::commandToString(CFtp::Command command)
{
    QString valeur = "";
    switch(command)
    {
      case CFtp::None:          valeur = tr("Aucune");                       break;
      case CFtp::ConnectToHost: valeur = tr("Ouverture Connexion");          break;
      case CFtp::Login:         valeur = tr("Authentification");             break;
      case CFtp::Close:         valeur = tr("Fermeture Connexion");          break;
      case CFtp::List:          valeur = tr("Listage");                      break;
      case CFtp::Cd:            valeur = tr("Changement de répertoire");     break;
      case CFtp::Get:           valeur = tr("Téléchargement vers Client");   break;
      case CFtp::Put:           valeur = tr("Upload vers Serveur");          break;
      case CFtp::Remove:        valeur = tr("Effacement d'un fichier");      break;
      case CFtp::Mkdir:         valeur = tr("Création d'un répertoire");     break;
      case CFtp::Rmdir:         valeur = tr("Effacement d'un répertoire");   break;
      case CFtp::Rename:        valeur = tr("Renommage d'un fichier");       break;
      case CFtp::RawCommand:    valeur = tr("Commande personnalisée");       break;
    }
    return valeur;
}
//-------------------------------------- errorToString() ----------------------------------------
QString C_Dlg_QGetDatasemp::errorToString(CFtp::Error error)
{
    QString valeur = "";
    switch(error)
    {
     case CFtp::NoError:             valeur = tr("Aucune erreur");        break;
     case CFtp::HostNotFound:        valeur = tr("Serveur non trouvé");   break;
     case CFtp::ConnectionRefused:   valeur = tr("Connexion refusée");    break;
     case CFtp::NotConnected:        valeur = tr("Non connectée");        break;
     case CFtp::UnknownError:        valeur = tr("Erreur inconnue");      break;
   }
    return valeur;
}

//-------------------------------------- InitCMedicaBase -----------------------------------------
int C_Dlg_QGetDatasemp::InitCMedicaBase()
{if (m_pCMedicaBase) return 1;
 //......................... va falloir aussi avoir la base medicamenteuse ........................................
 QString fileNameIni = QDir::cleanDirPath (G_pCApp->m_PathRessources + "MedicaBase.ini");
 if ( QFileInfo ( fileNameIni ).size() <= 30)
    {toLog(tr("Le fichier %1 est vide ou n'existe pas").arg(fileNameIni));
     return 0;
     }
 QString fileNameCfg = QDir::cleanDirPath (G_pCApp->m_PathAppli + "../../drtux/bin/MedicaBase.cfg");
 if ( QFileInfo ( fileNameCfg ).size() <= 30)
    {toLog(tr("Le fichier %1 est vide ou n'existe pas").arg(fileNameCfg));
     return 0;
    }
 m_pCMedicaBase = new CMedicaBase( QDir::cleanDirPath ( fileNameIni ),   // ce .ini  est avec Datasemp actif
                                   QDir::cleanDirPath ( fileNameCfg )
                                 );
 if (m_pCMedicaBase->m_MedicaBase==0) {delete m_pCMedicaBase; m_pCMedicaBase=0; return 0;}
 if (m_pCMedicaBase->m_DataBaseSrce==0)
    {toLog(tr("La base Datasemp n'existe pas"));
     QSqlQuery sqlQuery ("SELECT DatasempVersion FROM version " , m_pCMedicaBase->m_DataGet_Base );
    }
 return 1;
}

//---------------------------------------- GetDatasempIndexVersion ------------------------------------------------
QString C_Dlg_QGetDatasemp::GetDatasempIndexVersion()
{   QString version = "";
    if (m_pCMedicaBase==0) return version;
    if (m_pCMedicaBase->m_DataGet_Base->isOpen()==FALSE && m_pCMedicaBase->m_DataGet_Base->open()==FALSE)  return version;

    QSqlQuery sqlQuery ("SELECT DatasempVersion FROM version " , m_pCMedicaBase->m_DataGet_Base );
    if (sqlQuery.isActive() && sqlQuery.next())
       {version = sqlQuery.value(0).toString();
        int pos = version.find('|');
        if (pos != -1) version = version.left(pos);
       }
    else               version = "";
    m_pCMedicaBase->m_DataGet_Base->close();
    return version;
}

//---------------------------------------- SetDatasempIndexVersion ------------------------------------------------
void C_Dlg_QGetDatasemp::SetDatasempIndexVersion( QString version)
{   version = version + "|"+ GetDatasempDataVersion();

    if (m_pCMedicaBase->m_DataGet_Base->isOpen()==FALSE && m_pCMedicaBase->m_DataGet_Base->open()==FALSE)  return;

    QString   requete  = "DELETE FROM version";
    QSqlQuery sqlQuery (requete, m_pCMedicaBase->m_DataGet_Base);
    m_pCMedicaBase->OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);

    requete = "INSERT INTO `version` (`DatasempVersion`) VALUES ('"+version+"')";
    sqlQuery.exec(requete);
    m_pCMedicaBase->OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);

    // QString requete  = "UPDATE version SET  DatasempVersion = \"";
    //         requete += version+"\"";
    // QSqlQuery sqlQuery (requete, m_pCMedicaBase->m_DataGet_Base);
    // m_pCMedicaBase->OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);
    m_pCMedicaBase->m_DataGet_Base->close();
}

//---------------------------------------- SetDatasempDataVersion ------------------------------------------------
void C_Dlg_QGetDatasemp::SetDatasempDataVersion( QString version)
{   version = GetDatasempIndexVersion() + "|" + version;
    if (m_pCMedicaBase->m_DataGet_Base->isOpen()==FALSE && m_pCMedicaBase->m_DataGet_Base->open()==FALSE)  return;

    QString   requete  = "DELETE FROM version";
    QSqlQuery sqlQuery (requete, m_pCMedicaBase->m_DataGet_Base);
    m_pCMedicaBase->OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);

    requete = "INSERT INTO `version` (`DatasempVersion`) VALUES ('"+version+"')";
    sqlQuery.exec(requete);
    m_pCMedicaBase->OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);

    // QString requete  = "UPDATE version SET  DatasempVersion = \"";
    //         requete += version+"\"";
    // QSqlQuery sqlQuery (requete, m_pCMedicaBase->m_DataGet_Base);
    // m_pCMedicaBase->OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);
    m_pCMedicaBase->m_DataGet_Base->close();
}

//---------------------------------------- GetDatasempDataVersion ------------------------------------------------
QString C_Dlg_QGetDatasemp::GetDatasempDataVersion()
{   QString version;
    if (m_pCMedicaBase->m_DataGet_Base->isOpen()==FALSE && m_pCMedicaBase->m_DataGet_Base->open()==FALSE)  return 0;

    QSqlQuery sqlQuery ("SELECT DatasempVersion FROM version " , m_pCMedicaBase->m_DataGet_Base );
    if (sqlQuery.isActive() && sqlQuery.next())
       {version = sqlQuery.value(0).toString();
        int pos = version.find('|');
        if (pos != -1) version = version.mid(pos + 1);
        else           version = "";
       }
    else               version = "";
    m_pCMedicaBase->m_DataGet_Base->close();
    return version;
}

//---------------------------------------- pushButton6_clicked ------------------------------------------------
void C_Dlg_QGetDatasemp::pushButton6_clicked()
{accept();
}
//-----------------------------------------------------  PassWordEncode --------------------------------------------------
// Algo minimal pour l'instant
// chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
// resultat final en hexa;
// les linges qui sechent mouillent les cordes
// brouiller les cartes

QString C_Dlg_QGetDatasemp::PassWordEncode(QString &pass)
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
QString C_Dlg_QGetDatasemp::PassWordDecode(QString str_to_decode)
{
 char decoded_car;
 char pt_magic_key[] = "les linges qui sechent mouillent les cordes";
 QString decoded_str = "";
 int len_pass        = str_to_decode.length();
 int      pos        = 0;
 while ( pos < len_pass)
     {decoded_car  = HexToUINT( str_to_decode.mid(pos,4));
      decoded_car  =  decoded_car ^ pt_magic_key[pos/4];
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
QString C_Dlg_QGetDatasemp::UINTtoHex( UINT16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   // là on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
UINT16 C_Dlg_QGetDatasemp::HexToUINT( const char *str)
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





