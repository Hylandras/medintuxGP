/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
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
//
//    mysqldump --quote-name --compatible=mysql323 CCAMTest | gzip > /home/ro/CCAMTest.gz
//    Les spectatrices ne veulent pas qu'on les ennuie dans la file.
#include <qdir.h>
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qprocess.h>
#include <qeventloop.h>

#include "CApp.h"
#include "../../MedinTuxTools/cdlg_passwordget.h"
#include "../../MedinTuxTools/Theme.h"
CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application
static char NUM_VERSION[]     = "==##@@==2.15.000==@@##==";
QString    CApp::m_PathAppli  = "";

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::~CApp()
{if (m_pCMoteurBase) delete m_pCMoteurBase;
 if (m_pCMedicaBase) delete m_pCMedicaBase;

 #ifdef SESAMVITALE_VERSION
    if (m_pCps) delete m_pCps;
 #endif
}

//--------------------------------------------- quit -------------------------------------------------------------------
/*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
*/
void CApp::quit()
{emit Sign_QuitterRequired();    // se connecter a tous ceux connectes a ce signal pour qu'il puissent Sauver le meubles.
 QApplication::quit();
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
    : C_MUI_App(mui_name,  argc, argv )
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

m_NUM_VERSION           = NUM_VERSION;
   
//............................ pointeurs sur les objets principaux generaux Uniques ...................................................
//                             l'interdependance de ces differentes classes rend
//                             plus simple de les rendre accesible par un pointeur global
//                             Initialisé lors de la creation de ces classes qui doivent être
//                             instanciées qu'une seule fois pendant toute l'application
//                             la remise à zero se fait lors de la destruction de la classe.

//.................. les deux  objets suivants sont INDISPENSABLES à l'application ....................................................
//                   et seule une instance de ces objets doit exister par application

m_pCMoteurBase              = 0;             // classe d'outils de gestion des données en base de donnée
m_pCMedicaBase              = 0;             // moteur de la base de donnée medicaments
m_pMed_stat                 = 0;             // classe widget principal et parentes de toutes != 0 si constructeur OK
//................... les objets suivants sont crees à la demande dans l'application..............................................
//                    mais seule une instance de ces objets peut exister par application
m_PathAppli                 = "";                // chemin de l'Applicatif
m_CriptedPassWord           = "";
m_User                      = "";
m_SignUser                  = "";
//................. pour faire jonction avec agenda .........................................
m_LastNom                   = "";
m_LastPrenom                = "";
m_LastGUID                  = "";
m_LastPK                    = "";
m_Droits                    = "";
m_LastTel                   = "";
m_LastType                  = "";
m_LastDuree                 = -1;
m_LocalParam                = "";
m_IsAppStartWithUser        = FALSE;
m_ExportFileName            = "";
m_ExportDebLine             = "";
m_ExportEndLine             = "EOL";
m_ExportDebField            = "";
m_ExportEndField            = "|";
//......................... carte PS .............................
#ifdef SESAMVITALE_VERSION
     m_pCps = new C_Cps;
     qDebug ("With Sesam Vitale");
#else
     qDebug ("Without Sesam Vitale");
#endif

    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    //.................... recuperer path de demarrage de l'appli .......................................
    QFileInfo qfi(argv[0]);
    m_PathAppli    =  CGestIni::Construct_PathBin_Module("med_stat", qfi.dirPath (true));
    m_PathIni      =  CGestIni::Construct_Name_File_Ini("med_stat",  qfi.dirPath (true), "");

    QString driver, DataBaseName, user, password, hostname, port, baseCfg, qstr;
    //....................... charger le fichier de configuration des bases .............................
    CGestIni::Param_UpdateFromDisk(m_PathAppli + "../../Manager/bin/DataBase.cfg" , baseCfg);
    //.................... verifier si instance unique ..................................................
    QString localParam , isUnique, isRun;
    CGestIni::Param_UpdateFromDisk(m_PathIni, m_LocalParam);
    if (CGestIni::Param_ReadParam( m_LocalParam, "Derniere Session", "Instance", &isUnique, &isRun) == 0 &&   // zero = pas d'erreur
        isUnique.lower() == QObject::tr("unique")                                                        &&
        sendMessage("Wake up!")
       )
       { activateMainWidget ();
         return ;                   // ok cette appli est déjà active, la mettre au premier plan (sort avec G_pCApp = 0 )
       }
    //.............................. initialiser l'application ..........................................
    initialize();
    //.............................. initialiser le theme ..........................................
    if (CGestIni::Param_ReadParam( m_LocalParam, "Theme", "Path", &m_PathTheme) !=0 )  // zero = pas d'erreur
       { m_PathTheme     =  "../../Themes/Default/";                                   // valeur par défaut si pas de theme explicite
       }
    if (!QDir(m_PathTheme).exists())    m_PathTheme     =  "../../Themes/Default/";        // valeur par défaut
    if (QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
    m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
    Theme::setPath(m_PathTheme);

    //.......................Charger les parametres de connexion ........................................
    //                       Local au programme (pas celui de l'utilisateur)
    if (CGestIni::Param_ReadParam( m_LocalParam, "Connexion", "Parametres", &driver, &DataBaseName, &user, &password, &hostname, &port) !=0 )  // zero = pas d'erreur
       { driver           = "QMYSQL3";
         DataBaseName     = "DrTux";
         user             = "root";
         password         = "";
         hostname         = "";
         port             = "3306";
       }
    //....................... ouvrir et initialiser le moteur de base de données ....................................
    m_pCMoteurBase = new CMoteurBase( driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                      DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                      user,          // = "root"
                                      password,      // = ""
                                      hostname,
                                      port,
                                      baseCfg,
                                      "BASE_MEDSTAT",
                                      &qstr
                                  );
    //.................. si message d'erreur construire un QLabel pour l'afficher ..........................
    m_LastError = qstr;
    if (m_pCMoteurBase==0)
       {    QMessageBox::critical (0,   "MedinTux med_stat" ,
                                         QObject::tr("CMoteurBase ne peut s'instancier \r\n ") + qstr ,
                                         QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
            return;
       }
    if (m_pCMoteurBase->m_DataBase ==0)
       {   QMessageBox::critical (0,   "MedinTux med_stat" ,
                                         QObject::tr("CMoteurBase->m_DataBase ne peut s'instancier \r\n ") + qstr ,
                                         QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
           delete m_pCMoteurBase;
           m_pCMoteurBase=0;
           return;
       }
    //......................... initialiser la base therapeutique ................................................
   m_pCMedicaBase = new CMedicaBase(m_PathAppli + "../../drtux/bin/MedicaBase.ini", m_PathAppli + "../../drtux/bin/MedicaBase.cfg");
   if (m_pCMedicaBase==0)
      {delete m_pCMoteurBase; m_pCMoteurBase=0;
       QMessageBox::critical (0,   "MedinTux med_stat" ,
                                   QObject::tr("m_pCMedicaBase-ne peut s'instancier \r\n ") + qstr ,
                                   QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
       return;
      }
    //....................... parametres d'exporation .................................................
    if ( CGestIni::Param_ReadParam( m_LocalParam, "Export config", "Delimiteurs", &m_ExportDebLine, &m_ExportEndLine,&m_ExportDebField, &m_ExportEndField) !=0 ) 
       { m_ExportDebLine             = "";
         m_ExportEndLine             = "EOL";
         m_ExportDebField            = "";
         m_ExportEndField            = "|";
       }
    if (CGestIni::Param_ReadParam( m_LocalParam, "Export config","File Name", &m_ExportFileName ) !=0 )  // zero = pas d'erreur
       { m_ExportFileName            = "../bin/Resultats/ExportationResultat.txt";
       }
    if (CGestIni::Param_ReadParam( m_LocalParam, "Export config","nb_max_results", &m_nb_max_results ) !=0 )  // zero = pas d'erreur
       { m_nb_max_results            = "150";
       }
    m_ExportDebLine  = convertToText(m_ExportDebLine);
    m_ExportEndLine  = convertToText(m_ExportEndLine);
    m_ExportDebField = convertToText(m_ExportDebField);
    m_ExportEndField = convertToText(m_ExportEndField);
    //....................... verifier le mot de passe ...........................................................................
    //..................................... user name password & cie .............................................
    CGestIni::Param_ReadParam( m_LocalParam, "Derniere Session", "Utilisateur", &m_User, &m_SignUser);
    CGestIni::Param_ReadParam( m_LocalParam, "Derniere Session", "Password",    &m_CriptedPassWord);

    if (m_CriptedPassWord.left(1)=="#") m_CriptedPassWord = m_CriptedPassWord.mid(1);                       //  si crypté (precede de #)
    else                                CMoteurBase::PassWordEncode(m_CriptedPassWord);                     //  si non crypté (precede de rien) alors le crypter
    if (   (m_User.length() != 0 &&  m_SignUser.length() != 0   &&              // si non vérifié :  user defini avec
            m_pCMoteurBase->VerifyUserPassWord(m_User, m_CriptedPassWord)       //                   mot de passe fourni correct
           )                                                                    // alors appeler dialogue de login
       )
       {m_IsAppStartWithUser = TRUE;
       }
    PositionneBonDroits();
    G_pCApp                  =    this;
}
//--------------------------------------------- get_CustomRubNameList --------------------------------------
QStringList  CApp::get_CustomRubNameList()
{   QString manager_ini;
    //....................... charger le fichier de configuration des bases .............................
    CGestIni::Param_UpdateFromDisk(m_PathAppli + "../../Manager/bin/Manager.ini" , manager_ini);
    
    //.......................recuperer les rubriques automatiques definies par l'utilisateur.............................................
    //    [Rubriques Automatiques]
    //         1= Biologie|bi|20070000
    //         2= Imagerie|im|20040000
    QStringList list;
    CGestIni::Param_GetList(manager_ini, "Rubriques Automatiques", "",  list , 1);  // 1 pour on les veut stripes
    //.......................y ajouter les rubriques automatiques obligatoires .............................................
    list.prepend ("Vigie|cl|20050000");
    list.prepend ("Documents|do|20080000");
    return list;
}
//--------------------------------------------- IsAppStartWithUser -------------------------------------------------------------------
bool CApp::IsAppStartWithUser()
{return m_IsAppStartWithUser;
}

//--------------------------------------------- CApp -------------------------------------------------------------------
//  Oh dieux qui etes aux cieux
//  Reglez vos comptes en cieux
//
int CApp::AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool passwordMustBeRecorded /* = 1 */)
{int                          ok = 0;
 QString        cripted_password = "";
 QString        userPk           = m_pCMoteurBase->GetUserPrimKey( newUser, &cripted_password );
 //........................ verifier si des fois que sans mot de passe ..................................
 if (m_pCMoteurBase->VerifyUserPassWord(newUser, ""))
    { m_User            = newUser;
      m_SignUser        = newSignUser;
      m_CriptedPassWord = "";
      //saveLastUserOn_Ini(newUser, newSignUser, "", passwordMustBeRecorded);
      ok = 1;
    }
 //................ sinon demander le mot de passe ..................................
 else
    {CDlg_PasswordGet *dlg = new CDlg_PasswordGet(&cripted_password, parent);
    if (dlg == 0)             return ok;

     if (dlg->exec()==QDialog::Accepted && m_pCMoteurBase->VerifyUserPassWord(newUser, cripted_password) )
        {m_CriptedPassWord = cripted_password;
         m_User            = newUser;
         m_SignUser        = newSignUser;
         saveLastUserOn_Ini(newUser, newSignUser, cripted_password, dlg->IsPasswordMustBeRecord());
         ok = 1;
        }
     delete dlg;
    }
 PositionneBonDroits();
 return ok;
}
//
//---------------------------------------------- PositionneBonDroits ------------------------------------------------------------
void CApp::PositionneBonDroits()
{//............................. si l'utilisateur est le signataire alors les droit sont les siens ......................................................
    if (m_User == m_SignUser)
       {m_Droits   = m_pCMoteurBase->GetFieldValue( m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_DROITS,   // field to get
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,    // field to match
                                                    m_User);                                               // value to match
       }
    //............................. si l'utilisateur n'est pas le signataire alors les droits sont ceux accordés par le signataire ........................
    else
       {m_Droits = m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  m_SignUser , m_User);
       }
}
//--------------------------------------------- IsThisDroitExist -------------------------------------------------------------------
// ACTION vérifie si le droit : droitToFind  (trois caractères style med adm sgn ....) existe dans la chaine de droits : listDroits
//        6 fois plus rapide que : listDroits.find(droitToFind) != -1
bool CApp::IsThisDroitExist(const char *listDroits, const char *droitToFind)
{
 if (listDroits==0)   return FALSE;
 if (droitToFind==0)  return FALSE;
 char *pt  = (char*)listDroits;
 while (*pt)
    {if (pt[1]==droitToFind[0] && pt[2]==droitToFind[1] &&pt[3]==droitToFind[2]) return TRUE;
     pt += 4;
    }
 return FALSE;
}
//---------------------------------------------- saveLastUserOn_Ini ------------------------------------------------------------
void CApp::saveLastUserOn_Ini(const QString &user, const QString &signUser, const QString &cripted_password, bool passwordMustBeRecorded /* = 1 */)
{
     if ( passwordMustBeRecorded )
        {CGestIni::Param_WriteParam( &m_LocalParam, "Derniere Session", "Password",    QString("#") + cripted_password);
         CGestIni::Param_WriteParam( &m_LocalParam, "Derniere Session", "Utilisateur", user, signUser);
         CGestIni::Param_UpdateToDisk(m_PathIni, m_LocalParam);
        }
     else
        {CGestIni::Param_WriteParam( &m_LocalParam, "Derniere Session", "Password", "");
         CGestIni::Param_WriteParam( &m_LocalParam, "Derniere Session", "Utilisateur", "", "");
         CGestIni::Param_UpdateToDisk(m_PathIni, m_LocalParam);
        }
}
//--------------------------------------- convertToIni --------------------------------------------------------------
QString CApp::convertToIni(const QString &text)
{  QString result = text;
   //result = result.replace(",","&#44");
   result = result.replace("=","&#61");
   result = result.replace("[","&#91");
   result = result.replace("]","&#93");
   result = result.replace(";","&#59");
   //result = result.replace("/","&#47;");
   return result;
}
//--------------------------------------- convertToText --------------------------------------------------------------
QString CApp::convertToText(const QString &text)
{  QString result = text;
   //result = result.replace("&#44",",");
   result = result.replace("&#61","=");
   result = result.replace("&#91","[");
   result = result.replace("&#93","]");
   result = result.replace("&#59",";");
   //result = result.replace("&#47","/");
   return result;
}