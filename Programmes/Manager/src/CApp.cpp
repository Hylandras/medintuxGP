/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
//
//    mysqldump --quote-name --compatible=mysql323 CCAMTest | gzip > /home/ro/CCAMTest.gz
//    Les spectatrices ne veulent pas qu'on les ennuie dans la file.

#include "CApp.h"
#include <QtGlobal>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

#include <QStringList>
#include <QTextStream>
#include <QIODevice>
#include <QProcess>
#include <QTextCodec>
#include <QMessageBox>
#include <QCoreApplication>
#include "../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"                       // Gestion du theme de l'appli
#include "../../MedinTuxTools-QT4/CCoolPopup.h"                             // Gestion des popups surgissants

CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application

//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 #ifdef SESAM_VERSION
    if (m_pCps)    delete m_pCps;
    if (m_pVitale) delete m_pVitale;
 #endif
 #ifdef ENTREES_SIGEMS
    if (m_pC_DSigemsVar) delete m_pC_DSigemsVar;
 #endif
}
//--------------------------------------------- CApp -------------------------------------------------------------------
void CApp::quit()
{emit Sign_QuitterRequired();    // se connecter a tous ceux connectes a ce signal pour qu'il puissent Sauver le meubles.
 QApplication::quit();
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
    : C_AppCore(mui_name,argc,argv)
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    QString driver, baseToConnect, sqlUserName, sqlPass, hostName, port, qstr;
    QString baseCfg;
    m_PluginRun          = "";
    m_pCCoolPopup        = 0;
    m_IsNomadeActif      = 0;
    m_IsGestionNomadisme = 0;
    m_GuiFont            = QApplication::font();
    m_NUM_VERSION        = NUM_VERSION;
    m_pQSplashScreen     = new C_SplashScreen(Theme::getIcon("splash_Manager.png"));
    if ( m_pQSplashScreen )
       { m_pQSplashScreen->show();
         m_pQSplashScreen->showMessage(QObject::tr("Initialising connexions ........."),
                                                Qt::AlignCenter | Qt::AlignCenter, Qt::black);  //This line represents the alignment of text, color and position
         // connect( m_pQSplashScreen, SIGNAL(Sign_OnClicked(QMouseEvent *, int &)), this, SLOT(Slot_OnSplash_Clicked(QMouseEvent *, int &)) ); // marche pas
         processEvents(); //This is used to accept a click on the screen so that user can cancel the screen
       }
//......................... va le falloir ..................................
QFileInfo qfi(argv[0]);
//......................... carte PS .......................................
#ifdef SESAM_VERSION
     m_pCps    = new C_Cps;
     m_pVitale = new C_Vitale;
     qDebug ()<< tr("Version with SesamVitale usage").toLatin1();
#else
     qDebug() << tr("Version without SesamVitale usage").toLatin1();
#endif
//........................ initialisation de la base SigEntrees ............
#ifdef ENTREES_SIGEMS
    m_pC_DSigemsVar = 0;
    QString sigEntreesFileCfg = CGestIni::Construct_PathBin_Module("SigEntrees", qfi.path ())+"/SigEntreesBases.cfg";
    if (QFile::exists(sigEntreesFileCfg))
       {/*
        QMessageBox::critical (0,  "MedinTux Manager" ,
                               getConfigContext() + tr("<b><u>C_DSigemsVar not initialised</u> %1 line %2 </b><br/>'%3' don't exists").arg(Q_FUNC_INFO, QString::number(__LINE__), sigEntreesFileCfg) ,
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
                 return;  // tous les delete se feront lors destruction du parent (QT oblige !!)
        */
        // qDebug() << getConfigContext() + tr("C_DSigemsVar not initialised %1 line %2 '%3' don't exists").arg(Q_FUNC_INFO, QString::number(__LINE__), sigEntreesFileCfg);
        CGestIni::Param_UpdateFromDisk(sigEntreesFileCfg , baseCfg);
        m_pC_DSigemsVar        = new C_DSigemsVar(baseCfg);
        if (m_pC_DSigemsVar->getLastError().length())
           {
             QMessageBox::critical (0,  "MedinTux Manager" ,
                                    getConfigContext() + tr("<b><u>C_DSigemsVar not initialised</u> %1 line%2 </b><br/>%3").arg(Q_FUNC_INFO, QString::number(__LINE__), m_pC_DSigemsVar->getLastError()) ,
                                    QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
             return;  // tous les delete se feront lors destruction du parent (QT oblige !!)
           }
        qDebug ()<< tr("Version with SigEntrees usage").toLatin1();
       }
    else
       { qDebug() << tr("m_pC_DSigemsVar not initialised %1 line %2 '%3' don't exists").arg(Q_FUNC_INFO, QString::number(__LINE__), sigEntreesFileCfg);
       }
#else
    qDebug ()<< tr("Version without SigEntrees usage").toLatin1();
#endif

    //.................... recuperer path de demarrage de l'appli ...........................
    m_PathSigemsPA =  CGestIni::Construct_PathBin_Module("InterfaceSigems", qfi.path ());

    //....................... charger le fichier de configuration des bases .............................
    CGestIni::Param_UpdateFromDisk(pathAppli() + "DataBase.cfg" , baseCfg);
    m_SignUser = "admin";
    //...................... nomadisme ......................................................
    if (readParam("Connexion", "Gestion nomadisme", &qstr)==QString::null && (qstr=qstr.lower())!= "inactif")  // zero = pas d'erreur
       {if (qstr=="fullreplication")
            m_IsGestionNomadisme = CMoteurBase::fullReplication;
        else if (qstr=="progmasterupdate")
            m_IsGestionNomadisme = CMoteurBase::progMasterUpdate;
        else if (qstr=="fullsynchro")
            m_IsGestionNomadisme = CMoteurBase::fullSynchro;
       }
    QString connexionVar = "";
    if (m_IsGestionNomadisme && readParam("Connexion", "nomadisme", &qstr)==QString::null && qstr.lower()[0]=='a')  // zero = pas d'erreur
       {m_IsNomadeActif = TRUE;
       }
    if (m_IsNomadeActif) connexionVar     = "Nomade";
    else                 connexionVar     = "Master";

    //.......................Charger les parametres de connexion ........................................
    //                       Local au programme (pas celui de l'utilisateur medintux)
    if (readParam( "Connexion", connexionVar, &driver, &baseToConnect, &sqlUserName, &sqlPass, &hostName, &port) != QString::null )  // zero = pas d'erreur
       { driver            = "QMYSQL3";
         baseToConnect     = "DrTuxTest";
         sqlUserName       = "root";
         sqlPass           = "";
         hostName          = "";
         port              = "3306";
       }
    if (m_pQSplashScreen)
       { m_pQSplashScreen->showMessage( QObject::tr("%1::%2;;%3::%4::%5").arg(driver,baseToConnect,sqlUserName,hostName,port),
                                                         Qt::AlignCenter | Qt::AlignCenter, Qt::black);  //This line represents the alignment of text, color and position
         processEvents(); //This is used to accept a click on the screen so that user can cancel the screen
       }
     //SLEEP(5);
    //if (m_IsNomadeActif)
       {changeAllModuleConnectionParam(driver, baseToConnect, sqlUserName, sqlPass, hostName, port);
       }
    m_MySql_Pass  = sqlPass;  //GetMySqlPass();
     //....................... ouvrir et initialiser le moteur de base de donnees ....................................
    CMoteurBase::verifyMode baseVerification = readUniqueParam("Connexion", "Verification Integrite Bases").lower()=="oui" ? CMoteurBase::verifyBaseStruct : CMoteurBase::withoutVerifyBaseStruct;
    qstr = "";        // contiendra les messages d'erreurs et warnings
    m_pCMoteurBase = new CMoteurBase(driver ,                 // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                     baseToConnect,           // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                                     sqlUserName,             // = "root"       user
                                     sqlPass,                 // = ""           password
                                     hostName,                // = "localhost"  hostname
                                     port,
                                     baseCfg,
                                     "DRTUX_BASE",
                                     &qstr,
                                     this,
                                     "Manager CMoteurBase",
                                     baseVerification);

    //.................. si message d'erreur construire un QLabel pour l'afficher ..........................
    m_LastError = qstr;
    if (m_pCMoteurBase==0)
       {   QMessageBox::critical (0,   "MedinTux Manager" ,
                                         getConfigContext() + tr("<b><u>CMoteurBase cannot start</b></u><br/> ") + qstr ,
                                         QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
            return;  // tous les delete se feront lors destruction du parent (QT oblige !!)
       }
    if (m_pCMoteurBase->m_IsValid == FALSE)
       {
            QMessageBox::critical (0,   "MedinTux Manager" ,
                                         getConfigContext() + tr("<b><u>CMoteurBase->m_DataBase cannot start</b></u><br/> ") + qstr ,
                                         QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
           return;   // tous les delete se feront lors destruction du parent (QT oblige !!)
       }
    //....................... Assurer le nom des bases ...........................................................................
    if (baseVerification==CMoteurBase::verifyBaseStruct) m_pCMoteurBase->RenameTableWhithCorrectName();
    //....................... mode de creation des GUID .........................................................
    if (readUniqueParam ( "Gestion des dossiers", "GUID_createMode").toLower()=="myself") m_pCMoteurBase->set_GUID_CreateMode(CMoteurBase::byMyself);
    else                                                                                  m_pCMoteurBase->set_GUID_CreateMode(CMoteurBase::byQT);
    //..................................... user name password & cie .............................................
    m_IsAppStartWithUser = FALSE;
    readParam ( "Derniere Session", "Utilisateur", &m_User, &m_SignUser);
    readParam ( "Derniere Session", "Password",    &m_CriptedPassWord);
    m_User            = m_User.trimmed();
    m_SignUser        = m_SignUser.trimmed();
    m_CriptedPassWord = m_CriptedPassWord.trimmed();
    if (m_CriptedPassWord.startsWith('#')) m_CriptedPassWord = m_CriptedPassWord.remove('#');                       //  si crypte (precede de #)
    else                                   CMoteurBase::PassWordEncode(m_CriptedPassWord);                          //  si non crypte (precede de rien) alors le crypter
    if (   (m_User.length() != 0 &&  m_SignUser.length() != 0   &&              // si non verifie :  user defini avec
            m_pCMoteurBase->VerifyUserPassWord(m_User, m_CriptedPassWord)       //                   mot de passe fourni correct
           )                                                                    // alors appeler dialogue de login
       )
       {m_IsAppStartWithUser = TRUE;
       }
    else
       {m_User     = "";
        m_SignUser = "";
       }
    lireDroitsUtilisateurs();
    clearCurrentIdentite();
    //.............................. positionner une eventuelle fonte personnelle ..............................
    /*
    Family = Arial
    Italic = 0
    Underline = 0
    PointSize = 10
    Weight = 50
    */
    qstr      = readUniqueParam ( "Font", "Family");
    if (qstr.length()) m_GuiFont.setFamily(qstr);

    int value = readUniqueParam ( "Font", "PointSize").toInt();
    if (value)         m_GuiFont.setPointSize(value);

    value = readUniqueParam ( "Font", "Italic").toInt();
                       m_GuiFont.setItalic(value);

    value = readUniqueParam ( "Font", "Underline").toInt();
                       m_GuiFont.setUnderline(value);

    value = readUniqueParam ( "Font", "Weight").toInt();
    setStyleSheet( "QToolTip { border: 2px solid darkkhaki; padding: 5px; border-radius: 5px; opacity: 250; background-color: \"#fff8dc\"; color: \"#000000\";}");

    if (value)         m_GuiFont.setWeight(value);
    QApplication::setFont(m_GuiFont);
    G_pCApp   =    this;
}
//------------------------ isCurrentIdentiteSame ---------------------------------------
bool   CApp::isCurrentIdentiteSame(QTreeWidgetItem *pQListViewItem)
{ if (pQListViewItem==0)  return m_LastGUID.length()==0;
  QString guid = pQListViewItem->text(LIST_GUID);
  bool result = guid==m_LastGUID;
  return result;
}
//------------------------ isSigemsValid ---------------------------------------
int CApp::isSigemsValid()
{ return m_pC_DSigemsVar != 0;
}

//------------------------ Slot_OnSplash_Clicked ---------------------------------------
void CApp::Slot_OnSplash_Clicked(QMouseEvent *, int &)
{QCoreApplication::quit ();
}

//------------------------ getConfigContext ---------------------------------------
QString CApp::getConfigContext()
{ QString macAdr;
    QString ipAdr   = get_Current_IP_Adr(&macAdr);
    return  tr("<b><u>Network context</b></u><br/>"
               "&nbsp;&nbsp;&nbsp;&nbsp;<b>IP</b> : %1 <br/>"
               "&nbsp;&nbsp;&nbsp;&nbsp;<b>HD</b> : %2 <br/>"
               "&nbsp;&nbsp;&nbsp;&nbsp;<b>ini&nbsp;file</b>&nbsp;:&nbsp;%3<br/>").arg(ipAdr, macAdr, pathIni().replace(" ","&nbsp;"));
}
//------------------------ getNumVers ---------------------------------------
QString CApp::getNumVers()
{ return m_NUM_VERSION.remove("@").remove("#").remove("=");    // ==##@@==
}

//------------------------ changeAllModuleConnectionParam ---------------------------------------
void CApp::changeAllModuleConnectionParam(     const QString & /*driver*/,            // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                               const QString & /*baseToConnect*/,     // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                                               const QString &userName,          // = "root"
                                               const QString &passWord,          // = ""
                                               const QString &hostName,          // = "localhost"
                                               const QString &port               // = "port",
                                              )
{   QString param;
    QString path;
    QString baseName;
    QString driverName;
    //.....................Manager....................................................................................
    // bien que le principal est gere par le nomadisme, il reste  [Codage CIM10]
    path = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true),"Manager.ini");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("Manager.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param,  "Codage CIM10"  , "Connexion" ,  &driverName   ,  &baseName);
         CGestIni::Param_WriteParam(&param, "Codage CIM10"  , "Connexion" ,   driverName    ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
        }

    //.....................drtux-ccamview.ini..........................................................................
    path = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"Ressources/ccam/ccamview.ini");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("drtux-ccamview.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param,  "Connexion"  , "Parametres" ,  &driverName   ,  &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion"  , "Parametres" ,  driverName    ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param,  "Praticiens" , "Connexion"  ,  &driverName   ,  &baseName);
         CGestIni::Param_WriteParam(&param, "Praticiens" , "Connexion"  ,  driverName    ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
        }
    //.............................. MedicaBase.ini ..................................................................
    // [Connexion]
    //     Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root,  , localhost
    //     Parametres SrceBase   = QMYSQL3 , DatasempTest , root,  , localhost
    //     Parametres Get_Base   = QMYSQL3 , BaseGetTest, root,  , localhost
    //     InUseBase             = Parametres Get_Base , MedicaBase
    path = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.ini");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("MedicaBase.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param, "Connexion" , "Parametres MedicaBase" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres MedicaBase" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param, "Connexion" , "Parametres SrceBase"   , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres SrceBase"   ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param, "Connexion" , "Parametres Get_Base"   , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres Get_Base"   ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
      }
    //.............................. ccamview.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("ccamview",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("ccamview.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param,  "Connexion"  , "Parametres" ,  &driverName  ,  &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion"  , "Parametres" ,  driverName   ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param,  "Praticiens" , "Connexion"  ,  &driverName  ,  &baseName);
         CGestIni::Param_WriteParam(&param, "Praticiens" , "Connexion"  ,  driverName   ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }

    //.............................. personnes.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("personnes",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("personnes.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam ( param, "Connexion" , "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }

    //.............................. gest_user.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("gest_user",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path ))
       { changeAllModuleConnectionParamMessage("gest_user.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam ( param, "Connexion" , "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }
    //.............................. med_stat.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("med_stat",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("med_stat.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param,  "Connexion" ,    "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" ,    "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }

    //..............................biogest.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("biogest",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("biogest.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param,  "ConnexionBase" ,    "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "ConnexionBase" ,    "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }

    //..............................medicatux.ini ..................................................................
    path = CGestIni::Construct_Name_File_Ini("medicatux",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("medicatux.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param,  "Connexion_C_BDM_GenericPlugin" ,  "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion_C_BDM_GenericPlugin" ,  "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param,  "Connexion_Patient_Base" ,         "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion_Patient_Base" ,         "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param,  "Connexion_Theriaque" ,            "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion_Theriaque" ,            "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param,  "Connexion_Datasemp" ,             "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion_Datasemp" ,             "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }


    //..............................qgetdatasemp ..................................................................
    path = CGestIni::Construct_Name_File_Ini("qgetdatasemp",QFileInfo (qApp->argv()[0]).dirPath (true),"");
    if (  ! QFile::exists (path) )
       { changeAllModuleConnectionParamMessage("qgetdatasemp.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (  param,  "ConnexionBase" , "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam( &param,  "ConnexionBase" , "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (  param,  "Connexion" ,     "Parametres" , &driverName , &baseName);
         CGestIni::Param_WriteParam( &param,  "Connexion" ,     "Parametres" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam(   param,  "DrTuxTest",      "Parametres",  &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "DrTuxTest",      "Parametres",   driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_ReadParam(   param,  "MedicaTuxTest",  "Connexion",   &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "MedicaTuxTest",  "Connexion",    driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_ReadParam(   param,  "BaseGetTest",    "Connexion",   &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "BaseGetTest",    "Connexion",    driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_ReadParam(   param,  "CIM10Test",      "Connexion",   &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "CIM10Test",      "Connexion",    driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_ReadParam(   param,  "CCAMTest",       "Connexion",   &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "CCAMTest",       "Connexion",    driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_ReadParam(   param,  "Datasemp",       "Connexion",   &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "Datasemp",       "Connexion",    driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_ReadParam(   param,  "QLaboFTPTest",   "Connexion",   &driverName, &baseName);
         CGestIni::Param_WriteParam( &param,  "QLaboFTPTest",   "Connexion",    driverName,  baseName,  userName,  passWord,  hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
        }

    path = CGestIni::Construct_Name_File_Ini("qgetdatasemp",QFileInfo (qApp->argv()[0]).dirPath (true),"Ressources/MedicaBase.ini");
    if (  ! QFile::exists (path) )
       {changeAllModuleConnectionParamMessage("qgetdatasemp:MedicaBase.ini",path);
       }
    else
       { CGestIni::Param_UpdateFromDisk(path, param);
         CGestIni::Param_ReadParam (param, "Connexion" , "Parametres MedicaBase" , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres MedicaBase" ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param, "Connexion" , "Parametres SrceBase"   , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres SrceBase"   ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_ReadParam (param, "Connexion" , "Parametres Get_Base"   , &driverName , &baseName);
         CGestIni::Param_WriteParam(&param, "Connexion" , "Parametres Get_Base"  ,  driverName ,  baseName, userName , passWord , hostName, port);
         CGestIni::Param_UpdateToDisk(path, param);
       }
}
//--------------------------------------------- changeAllModuleConnectionParamMessage -------------------------------------------------------------------
void CApp::changeAllModuleConnectionParamMessage(const QString &place, const QString &path)
{   QColor color("red");
    QFont ft("sans",8,0,0);
    CouCou (tr("::changeAllModule\nConnectionParam()\n::%1\n::This file:\n%2\ndoes not exist").arg(place, path),color,ft,2000,Theme::getPath() + "MessagePopStrange.png");
}

//--------------------------------------------- GetMySqlPass -------------------------------------------------------------------
QString CApp::GetMySqlPass()
{//............ voir si pas dans la ligne d'argument .............................

 //............ voir si le .INI ne donne pas un serveur de password ..............
 return QString::null;
}

//--------------------------------------------- setCurrentIdentite -------------------------------------------------------------------
void CApp::setCurrentIdentite(QTreeWidgetItem *pQListViewItem)
{if (pQListViewItem==0) clearCurrentIdentite();
 else                   {m_LastNom    = pQListViewItem->text(LIST_NOM);
                         m_LastPrenom = pQListViewItem->text(LIST_PRENOM);
                         m_LastGUID   = pQListViewItem->text(LIST_GUID);
                         m_LastPK     = pQListViewItem->text(LIST_PK_PATIENT);
                       }
}

//--------------------------------------------- IsAppStartWithUser -------------------------------------------------------------------
bool CApp::IsAppStartWithUser()
{return m_IsAppStartWithUser;
}

//--------------------------------------------- AuthentifyAndSelectUser -------------------------------------------------------------------
//  Oh dieux qui etes aux cieux
//  Reglez vos comptes en cieux
//
int CApp::AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool /*passwordMustBeRecorded  = 1 */)
{int                          ok = 0;
 QString        cripted_password = "";
 m_pCMoteurBase->GetUserPrimKey( newUser, &cripted_password );
 //........................ verifier si des fois que sans mot de passe ..................................
 if (m_pCMoteurBase->VerifyUserPassWord(newUser, ""))
    { m_User            = newUser;
      m_SignUser        = newSignUser;
      m_CriptedPassWord = "";
      ok                = 1;
    }
 //................ sinon demander le mot de passe ..................................
 else
    {QString       login = newUser;
     QString       passw = "";
     C_Dlg_Login *dlg    = new C_Dlg_Login(login, passw, parent, (C_Dlg_Login::Visibility)(C_Dlg_Login::Show|C_Dlg_Login::LoginReadOnly));
     if (!dlg)             return ok;
     dlg->setFocusOnPass();
     dlg->setDefaultOnOk();
     if (dlg->exec()==QDialog::Accepted && m_pCMoteurBase->VerifyUserPassWord(newUser, dlg->get_Password()))
        { login             = dlg->get_Login();
          m_CriptedPassWord = dlg->get_Password();          // la il est en clair
          CMoteurBase::PassWordEncode(m_CriptedPassWord);   // la on le code a la mode CMoteurBase (pas la meme que CGestIni)
          m_User            = newUser;
          m_SignUser        = newSignUser;
          ok                = 1;
          saveLastUserOn_Ini(dlg->IsPasswordMustBeRecord());
        }
     delete dlg;
    }
 lireDroitsUtilisateurs();
 return ok;
}

//--------------------------------- DoPopupList -----------------------------------------------------
/*! \brief cree et active un popup menu apartir d'une liste d'items
 *  \param list : QStringList qui contient tous les item si icone associee du theme commence par #iconefile#reste du texte
 *  \return une QString qui contient le nom de l'item selectionne.
*/
QString CApp::DoPopupList(QStringList &list, const QString& sep /*= "|"*/, int posToHide /* = 0*/ )
{ThemePopup *pThemePopup = new ThemePopup(list,0, "", sep, posToHide);
 if (pThemePopup == 0)     return QString::null;
 QString ret = pThemePopup->DoPopupList();
 delete  pThemePopup;
 return  ret;
}

//---------------------------------------------- lireDroitsUtilisateurs ------------------------------------------------------------
void CApp::lireDroitsUtilisateurs()
{//............................. si l'utilisateur est le signataire alors les droit sont les siens ......................................................
    if (m_User == m_SignUser)
       {m_Droits   = m_pCMoteurBase->GetFieldValue( m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_DROITS,   // field to get
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,    // field to match
                                                    m_User);                                               // value to match
       }
    //............................. si l'utilisateur n'est pas le signataire alors les droits sont ceux accordes par le signataire ........................
    else
       {m_Droits = m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  m_SignUser , m_User);
       }
}
//--------------------------------------------- IsThisDroitExist -------------------------------------------------------------------
// ACTION verifie si le droit : droitToFind  (trois caract�res style med adm sgn ....) existe dans la chaine de droits : listDroits
//        6 fois plus rapide que : listDroits.find(droitToFind) != -1
bool CApp::IsThisDroitExist(const char *listDroits, QString droitToFind_in)
{if (listDroits==0) return FALSE;
 droitToFind_in.remove('-');
 //const char *droitToFind = droitToFind_in.toLatin1();
 char droitToFind[5];
 strncpy(droitToFind, droitToFind_in.toAscii(),5);
 char *pt  = (char*)listDroits;
 while (*pt)
    {if (pt[1]==droitToFind[0] && pt[2]==droitToFind[1] &&pt[3]==droitToFind[2]) return TRUE;
     pt += 4;
    }
 return FALSE;
}
//--------------------------------------------- IsThisDroitExistForThisUser -------------------------------------------------------------------
bool  CApp::IsThisDroitExistForThisUser(const QString &user, const char *droitToFind)
{QString droits =  m_pCMoteurBase->GetUserPermisions( user);
 return (droits.indexOf(droitToFind)!=-1);
}


//---------------------------------------------- saveLastUserOn_Ini ------------------------------------------------------------
void CApp::saveLastUserOn_Ini( bool passwordMustBeRecorded )
{  saveLastUserOn_Ini(m_User, m_SignUser, m_CriptedPassWord,  passwordMustBeRecorded );
}
//---------------------------------------------- saveLastUserOn_Ini ------------------------------------------------------------
void CApp::saveLastUserOn_Ini(const QString &user, const QString &signUser, const QString &cripted_password, bool passwordMustBeRecorded /* = 1 */)
{
     if ( passwordMustBeRecorded )
        {writeParam( "Derniere Session", "Password",    QString("#") + cripted_password);
         writeParam( "Derniere Session", "Utilisateur", user, signUser);
        }
     else
        {writeParam( "Derniere Session", "Password", "");
         writeParam( "Derniere Session", "Utilisateur", "", "");
        }
     updateIniParamToDisk();
}

//------------------------------------------------- CouCou ----------------------------------------------------
void CApp::CouCou(const QString &message, const QColor &color, const QFont &ft , int tempo /* = 1000 */, const QString &imagePath /* ="" */)
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 m_CCoolPopupTempo = tempo;
 QString path      = "";

 if (imagePath.length()==0 || !QFile::exists(imagePath)) path  = Theme::getPath() + "MessagePop.png";
 else                                                    path  = imagePath;
 QPixmap qpm(path);
 m_pCCoolPopup = new CCoolPopup( path , CCoolPopup::WithTransLabel, 0, 0, qpm.width(), qpm.height(), message);
 m_pCCoolPopup->setFont ( ft );
 m_pCCoolPopup->setTextColor(color);
 connect( m_pCCoolPopup,  SIGNAL( Sign_Clicked(CCoolPopup *) ),   this, SLOT(CouCouStop(CCoolPopup *))  ) ;
 connect( m_pCCoolPopup,  SIGNAL( Sign_PopupUp(CCoolPopup *) ),   this, SLOT(CouCouUp (CCoolPopup *))  ) ;
 connect( m_pCCoolPopup,  SIGNAL( Sign_PopupDown(CCoolPopup *) ), this, SLOT(CouCouStop(CCoolPopup *))  );
}
//------------------------------------------------- CouCou ----------------------------------------------------
/*! \brief Affiche un popup amusant pour afficher des messages */
void CApp::CouCou(const QString &message_in, const QString &imagePath /* ="" */, int tempo/* = 1000 */)
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 if (tempo==0) m_CCoolPopupTempo    = 2000;   // deux secondes
 else          m_CCoolPopupTempo    = tempo;
 QString path                       = imagePath;
 QString val                        = "";
 QString message                    = message_in;
 path.replace("$Theme",Theme::getPath(Theme::WithNoSeparator));

 if (path.length()==0 || !QFile::exists( path ))
    {readParam ("Popup Message", "image", &path);
     path.replace("$Theme",Theme::getPath(Theme::WithNoSeparator));
     if (!QFile::exists( path )) path  = Theme::getPath() + "MessagePop.png";
    }

 QPixmap qpm(path);
 m_pCCoolPopup = new CCoolPopup( path , CCoolPopup::WithTransLabel, 0, 0, qpm.width(), qpm.height(), message);
 //......... regarder si pas d'autres param ...............................................
 QFont ft = font();
 if (readParam ( "Popup Message", "font_family", &val)==QString::null) ft.setFamily(val);
 if (readParam ( "Popup Message", "font_size",   &val)==QString::null) ft.setPointSizeFloat(val.toDouble());
 if (readParam ( "Popup Message", "font_weight", &val)==QString::null) ft.setWeight(Min(99,val.toInt()));
 if (readParam ( "Popup Message", "font_bold",   &val)==QString::null) ft.setBold((bool)val.toInt());
 if (readParam ( "Popup Message", "font_italic", &val)==QString::null) ft.setItalic((bool)val.toInt());
 if (readParam ( "Popup Message", "tempo",       &val)==QString::null) if (tempo==0) m_CCoolPopupTempo = val.toInt();
 m_pCCoolPopup->setFont(ft);
 if (readParam ( "Popup Message", "font_color",  &val)==QString::null)  m_pCCoolPopup->setTextColor(QColor(val));
 connect( m_pCCoolPopup,  SIGNAL( Sign_Clicked(CCoolPopup *) ),   this, SLOT(CouCouStop(CCoolPopup *))  ) ;
 connect( m_pCCoolPopup,  SIGNAL( Sign_PopupUp(CCoolPopup *) ),   this, SLOT(CouCouUp (CCoolPopup *))  ) ;
 connect( m_pCCoolPopup,  SIGNAL( Sign_PopupDown(CCoolPopup *) ), this, SLOT(CouCouStop(CCoolPopup *))  );
}

//--------------------------------------- CouCouUp -------------------------------------------------------------
/*! \brief Appele lorsque le popup est en haut, attend le tempo puis fait descendre le popup */
void CApp::CouCouUp(CCoolPopup * pCCoolPopup)
{QTimer::singleShot(m_CCoolPopupTempo, pCCoolPopup, SLOT(Disappear()) );
}
//--------------------------------------- CouCouStop -----------------------------------------------------------
/*! \brief Appele pour detruire le popup */
void CApp::CouCouStop(CCoolPopup * pCCoolPopup)
{if (m_pCCoolPopup==pCCoolPopup)
    {pCCoolPopup->disconnect( SIGNAL(Sign_PopupUp(CCoolPopup *)) );
     pCCoolPopup->disconnect( SIGNAL(Sign_PopupDown(CCoolPopup *)) );
     QTimer::singleShot(0, this, SLOT(CouCouDestroy()) );
    }
}
//--------------------------------------- CouCouDestroy --------------------------------------------------------
/*! \brief Appele pour detruire le popup */
void CApp::CouCouDestroy()
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 m_pCCoolPopup = 0;
}
//------------------------------------ launchSpecificJob --------------------------------------
void CApp::launchSpecificJob(QString nameOfJob) // CZB
{
    QString data_ini, Titre_Job, Job, Param_job, pathJob;
    QStringList listParam;
    CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathIni , data_ini);
    CGestIni::Param_ReadParam( data_ini, "Specifiques", nameOfJob, &Titre_Job, &Job,  &Param_job);
    listParam << Param_job << m_User ;
    pathJob = QApplication::applicationDirPath()+ "/" + Job;
#ifdef Q_WS_WIN
    if (!pathJob.contains(".exe"))
        pathJob +=".exe";
#endif
#ifdef Q_WS_X11
       pathJob += "";
#endif
#ifdef Q_WS_MAC
       int pos = pathJob.lastIndexOf("/");
       if (pos != -1) pathJob = pathJob+".app/Contents/MacOS/"+pathJob.mid(pos+1);
#endif
    QProcess::startDetached (pathJob, listParam);
}
//---------------------------------------------- execCalendrier -----------------------------------------------------------------------
QString CApp::execCalendrier(const QDate &dateIn)
{     //............... lancer le calendrier .................................................
    QString pathPlugin;
    readParam ("Gestion du calendrier", "pathPlugin",       &pathPlugin);
    //..................................... ne pas rajouter "/" car c'est un nom de fichier ..........................
    if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanDirPath (pathPlugin.prepend(m_PathAppli));
    QString date   = dateIn.toString("ddMMyyyy");
    QString presel = Theme::getPath(TRUE)+"Agenda/MenuPreselDate.txt";
    QString pluginScript  = "[Execute]\r\n";
            pluginScript += "pathPlugin="      + pathPlugin                                           + "\r\n" +
                            "pathIni=****\r\n" +                //**** car indique d'utiliser le pathIni du plugin
                            "maskExch= non utilise\r\n"
                            "[Parametres]\r\n"
                            "m_StartDate        = " + date      + "\r\n"     +
                            "m_ReturnFormat    = ddMMyyyy\r\n" +
                            "m_InfDate         = " + date      + "\r\n"     +     // date lim inf
                            "m_SupDate         = " + date      + "\r\n"     +     // date lim sup
                            "m_PreselMenu      = " + presel    + "\r\n"     +
                            "m_modeToShow      = days" //debDate endDate hours
                            ;

   return PluginExe(this, pluginScript, CApp::endWait);
}

//------------------------------------------------------- PluginExe --------------------------------------------------
QString CApp::PluginExe(        QObject         * pQObject,
                                const char      * pluginScript,
                                int               /*waitFlag*/)
{
      QString     pathPlugin;  // Chemin de l'executable plugin ?  actionner sans l'extension .exe
      QString     pathIni;     // Chemin de l'executable plugin ?  actionner sans l'extension .exe
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
                                              pathPlugin,                       // Chemin de l'executable plugin ?  actionner sans l'extension .exe
                                              m_PathAppli,                      // Chemin de l'executable appelant (DrTux)
                                              pathIni,                          // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                              maskExch,                         // texte du fichier d'exchange
                                              m_pCMoteurBase->GUID_Create()+"-New",
                                              " ",
                                              " ",
                                              " " ,
                                              param,
                                              CMoteurBase::endWait );
}

//------------------------------------------------------- PluginExe --------------------------------------------------
// un plugin est un executable communicant avec DrTux par l'intermediaire du protocole suivant:
// DrTux appelle le plugin avec dans la ligne de commande:
// en :
//   0  Chemin de l'executable plugin ?  actionner
//   1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
//   2  Chemin de l'executable appelant
//   3  Chemin du fichier d'echange texte masque d'entree dont l'executable devra modifier les valeurs de retour
//                        ce fichier est retourne modifie par le plugin, ce fichier peut etre un masque html ou
//                        tout autre delire en accord avec la syntaxe du plugin d�s fois que ...
//                        si ce fichier est vide, o� n'existe pas le plugin retournera son resultat dans ce fichier
//                        d'echange que l'appelant recupera
//                        Le nom de ce fichier d'echange comporte : NomDuPlugin-GUID_Patient-
//   4  GUID du patient
//   5  PrimKey de l'observation en cours
//   6  PrimKey du terrain en cours
//   7  PrimKey de l'ordonnance en cours
//   8  Reserve
//   9  Reserve
//   10 Reserve
//   11 Reserve
//   12 Nb autres parametres d'appels specifiques au plugin
//   13 --> 13+Nb param�tres

QString CApp::PluginExe(        QObject         */*pQObject*/,
                                const QString    &pathPlugin,  // Chemin de l'executable plugin ?  actionner sans l'extension .exe
                                const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                const QString    &maskExch,    // texte du fichier d'exchange
                                const QString    &guid,
                                const QString    &dosPk,
                                const QString    &terPk,
                                const QString    &ordPk ,
                                QStringList      &param,
                                int               waitFlag /* = CApp::endWait */)
{
 QString     ret  = "";
 //..................... copier le masque de retour dans le dossier temporaire ......................
 QString pathExe  = pathPlugin;

 if ( QDir::isRelativePath ( pathExe ) ) pathExe = QDir::cleanDirPath (pathExe.prepend(pathAppli) );
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
    {CouCou(tr("Path to plugin not found: ").arg(pathExe));
    }
 QString nameExch = QFileInfo(pathExe).fileName()+"-"+ guid +".exc";

 QString      dst = "";
 if (readParam ( "Repertoire Temporaire", "Repertoire", &dst)!=QString::null)
     return QObject::tr("Error:  PluginExe()  \"Temporary Directory\", \"Directory\" can't find in ini File");         // path editeur de texte non defini
 if ( QDir::isRelativePath ( dst ) ) dst = QDir::cleanDirPath (dst.prepend(m_PathAppli) );
 dst +=  QDir::separator() + nameExch;
 QFile::remove (dst);

 //qDebug (QString("nameExch : ")  + dst);
 //qDebug (QString("pathAppli : ") + pathAppli);
 //qDebug (QString("pathExe : ") + pathExe);


 QFile qfdst(dst);
 if (qfdst.open(QIODevice::ReadWrite)==FALSE)
    {QMessageBox::critical (0, QObject::tr("PluginExe(): write pathExch"),
                               QObject::tr("The file\n'")+ dst + QObject::tr("'\ncan't be opened"),
                               QMessageBox::Abort, Qt::NoButton, Qt::NoButton );
     return QObject::tr("Error:  PluginExe() destination file can't be opened");
    }
 qfdst.writeBlock (maskExch, maskExch.length());
 qfdst.close();

 QStringList argList;
 //......................... completer les autres arguments .........................................
 //argList <<( pathExe + F_EXE );                 // 0  Chemin de l'executable plugin ?  actionner
 argList <<( pathIni );                         // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
 argList <<( pathAppli + m_NameAppli );         // 2  Chemin de l'executable appelant
 argList <<( dst );                             // 3  Chemin du fichier d'echange texte masque d'entree dont l'executable devra modifier les valeurs de retour
 //argList <<( "/home/ro/QFseVitale-53671d5a-52c0-42ff-a39c-bed207109033-New.exc");
 argList <<(guid );                             // 4  GUID du patient
 argList <<(dosPk);                             // 5  PrimKey de l'observation en cours
 argList <<(terPk);                             // 6  PrimKey du terrain en cours
 argList <<(ordPk);                             // 7  PrimKey de l'ordonnance en cours
 argList <<(" ");                               // 8  reserve
 argList <<(" ");                               // 9  reserve
 argList <<(" ");                               // 10 reserve
 argList <<(" ");                               // 11 reserve
 argList <<(QString::number(param.count()));    // 12 Nb autres parametres d'appels specifiques au plugin
 for ( QStringList::Iterator it = param.begin(); it != param.end(); ++it )  // 13 --> 13+Nb param�tres
     {argList << (*it);
     }
 //................................. executer le process .............................................
 if ( m_PluginRun == ""  )
    {

      m_PluginRun  = pathExe;
      if (waitFlag!=CApp::endWait)
         {QProcess::startDetached (m_PluginRun, argList);
          return QString("Process paralelle");    // on attend rien on se casse
         }
      QProcess*   proc = new QProcess();
      connect( proc,        SIGNAL(error ( QProcess::ProcessError  )),     this, SLOT(Slot_error ( QProcess::ProcessError  )) );

      proc->start(m_PluginRun, argList);
      proc->waitForStarted  (4000);
      proc->waitForFinished (-1); //crash crash non si -1 tralala
      //QByteArray ba = proc->readAllStandardError ();
      //qDebug(ba);
      /*
      processEvents ();
      while (waitFlag==CApp::endWait && proc->state()==QProcess::Running )
           { QApplication::processEvents ( QEventLoop::ExcludeUserInput );
           }
      */
      m_PluginRun = "";
      //............lire le fichier d'echange ..........................
      //dst  = "/home/ro/QFseVitale-53671d5a-52c0-42ff-a39c-bed207109033-New.exc";
      if ( !QFile::exists( dst ) )     return  ret;
      CGestIni::Param_UpdateFromDisk(dst,ret);
      delete proc;
    }
  QFile::remove (dst);
  return ret;
}

void CApp::Slot_error ( QProcess::ProcessError err )
{QString erreur = "no se";
 switch (err)
  {
   case  QProcess::FailedToStart: erreur = tr ("The process failed to start. Either the requested program is missing, or you may have insufficient permission to launch the program."); break;
   case  QProcess::Crashed:       erreur = tr ("The process crashed sometime after starting successfully."); break;
   case  QProcess::Timedout:      erreur = tr ("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again."); break;
   case  QProcess::WriteError:    erreur = tr ("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel."); break;
   case  QProcess::ReadError:     erreur = tr ("An error occurred when attempting to read from the process. For example, the process may not be running."); break;
   case  QProcess::UnknownError:  erreur = tr ("Unknown Error"); break;
   }
 CouCou(erreur);
}

