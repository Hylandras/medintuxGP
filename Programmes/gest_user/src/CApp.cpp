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

#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "../../MedinTuxTools/CGestIni.h"
#include "CApp.h"
#include "DlgPassChange.h"
#include "Dlg_PermsUserChoice.h"
#include "../../MedinTuxTools/cdlg_passwordget.h"
//#include <string.h>



CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application
static char NUM_VERSION[]     = "==##@@==2.16.000==@@##==";

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::~CApp()
{  if (m_pCMoteurBase)  delete m_pCMoteurBase;
   #ifdef SESAMVITALE_VERSION
   if (m_pCps)          delete m_pCps;
   #endif
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(int & argc, char ** argv, QString &erreur, const char*user,  const char*password)
    : QApplication (  argc, argv )
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

    m_NUM_VERSION    = NUM_VERSION;
    QString data_ini = "";
    QString baseCfg  = "";
    QString qstr     = "";
    QString val1     = "";
    QString val2     = "";
    QString val3     = "";
    QString val4     = "";
    QString qstr2    = "";
    QString port     = "";
    m_CriptedPassWord        =  tr("----- m_CriptedPassWord no initialised-----");
    G_pCApp                  =  0;
    m_PathAppli              =  CGestIni::Construct_PathBin_Module( "gest_user", QFileInfo (argv[0]).dirPath (true));
    m_PathIni                =  CGestIni::Construct_Name_File_Ini(  "gest_user", QFileInfo (argv[0]).dirPath (true),"");
    m_Contacts_Run           = FALSE;
    m_PermsChoice            = "";
    m_IsAppStartWithUser     = FALSE;
    m_LastError              = "";
    //.......................Charger les parametres .ini ........................................
    m_PathIni = CGestIni::Construct_Name_File_Ini("gest_user", QFileInfo (argv[0]).dirPath (true),"");
    CGestIni::Param_UpdateFromDisk(m_PathIni , data_ini);

    //......................................... positionner les parametres d'entrée ....................................
    if (user)           m_User      = user;
    else if (argc>=2)   m_User      = qApp->argv()[1]; //argv[1];
    else                m_User      = "";
   #ifdef SESAMVITALE_VERSION
    m_pCps = 0;
    if (argc>=4)
       { QString cps_data = qApp->argv()[3];
         cps_data.replace("__*$*__", "," );
         m_pCps = new C_Cps("", cps_data);
         //CGestIni::Param_UpdateToDisk("/home/ro/cps.txt" , cps_data);
         //m_pCps->UnSerialize(cps_data);
       }
    #endif
    //...............................................voir si  password est donnée en entrée ou dans ini................
    QString s_password;
    if (password)                                                         // SOIT en parametres d'appel : crypte (precede de #) ou non crypte et alors va falloir le crypter
       {s_password = password;
       }
    else if (argc>=2)
       { s_password  = qApp->argv()[2];               //argv[2];          // SOIT dans argv  d'entrée : crypte (precede de #) ou non crypte et alors va falloir le crypter
       }
    else if (CGestIni::Param_ReadParam( data_ini, "last login", "login data", &val1, &val2) ==0 )  // zero = pas d'erreur
       { m_User       = val1;
         s_password   = val2;
       }

    if (s_password.left(1)=="#")    {m_CriptedPassWord = s_password.mid(1);                       //  crypté (precede de #)
                                    }
    else                            {m_CriptedPassWord = password;                                // non crypté et alors on le crypte
                                     CMoteurBase::PassWordEncode(m_CriptedPassWord);
                                    }
    //......................................... path des images des rubriques ....................................
    //m_ImagePath = CGestIni::Construct_Name_File_Ini("drtux", QFileInfo (argv[0]).dirPath (true),"Ressources/RubriquesTextes/");
    //....................... charger le fichier de configuration ......................................
    //                        de la base de donnée
    CGestIni::Param_UpdateFromDisk(CGestIni::Construct_Name_File_Ini("Manager", QFileInfo (argv[0]).dirPath (true), "DataBase.cfg"), baseCfg);
    //.......................Charger les parametres de connexion ........................................
    if (CGestIni::Param_ReadParam( data_ini, "Connexion", "Parametres", &val1, &val2, &val3, &val4, &qstr2,&port) !=0 )  // zero = pas d'erreur
       { val1     = "QMYSQL3";     // driver name
         val2     = "DrTuxTest";   // base name
         val3     = "root";        // user
         val4     = "";            // password
         qstr2    = "";            // hostname
         port     = "3306";        // port
       }

    if (CGestIni::Param_ReadParam( data_ini, "Theme", "Path", &m_PathTheme) !=0 )  // zero = pas d'erreur
       { m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
       }
    if (!QDir(m_PathTheme).exists()) m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
    if (QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
    m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
    Theme::setPath(m_PathTheme);

    //....................... creer le moteur de base de données .....................................
    m_pCMoteurBase = new CMoteurBase(val1 ,                   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                     val2,                    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                     val3,                    // = "root"       user
                                     val4,                    // = ""           password
                                     qstr2,                   // = "localhost"  hostname
                                     port,
                                     baseCfg,
                                     "GEST_USER",
                                     &m_LastError);

    if ( m_pCMoteurBase==0 )               {erreur = tr("m_pCMoteurBase  n'a pu être initialisé");            return;}
    if ( m_pCMoteurBase->m_DataBase ==0)   {erreur = tr("m_pCMoteurBase->m_DataBase n'a pu être initialisé"); return;}
    //....................... positionner le bon mode openClose ...........................................................................
    m_pCMoteurBase->setOpenCloseMode(CGestIni::Param_ReadUniqueParam(data_ini, "Connexion","Fermer requetes").toInt());

    //....................... verifier le mot de passe .....................................
    if (   (m_User.length() != 0 &&                                             // si non vérifié :  user defini avec
            m_pCMoteurBase->VerifyUserPassWord(m_User, m_CriptedPassWord)       //                   mot de passe fourni correct
           )                                                                    // alors appeler dialogue de login
       )
       {m_IsAppStartWithUser = TRUE;
       }
    m_Droits         = m_pCMoteurBase->GetUserPermisions( m_User );
    G_pCApp          = this;
}

//--------------------------------------------- quit -------------------------------------------------------------------
/*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
*/
void CApp::quit()
{emit Sign_QuitterRequired();    // se connecter a tous ceux connectes a ce signal pour qu'il puissent Sauver le meubles.
 QApplication::quit();
}

//--------------------------------------------- IsAppStartWithUser -------------------------------------------------------------------
bool CApp::IsAppStartWithUser()
{return m_IsAppStartWithUser;
}

//--------------------------------------------- AdjustGlobalDroits -------------------------------------------------------------------
// ACTION lors d'un changement d'utilisateurs aprés avoir bien sur entré l'éventuel mot de passe
// si les droits du nouvel utilisateur ne sont pas presents au niveau du programme on les rajoute
 QString CApp::AdjustGlobalDroits(const QString &droits)
{QStringList list = QStringList::split("-", droits);
 for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
     {if (m_Droits.find(*it)==-1)  m_Droits +=  QString("-") + *it;
     }
 return m_Droits;
}

//--------------------------------------------- AddDroit -------------------------------------------------------------------
// ACTION si il n'est pas présent, ajoute un droit à la chaine de droits.
// si les droits du nouvel utilisateur ne sont pas presents au niveau du programme on les rajoute
void CApp::AddDroit( QString &listDroits, QString droitToadd)
{if (listDroits.find(droitToadd) == -1) listDroits += "-" + droitToadd;
}

//--------------------------------------------- RemoveDroit -------------------------------------------------------------------
// ACTION enelve de la liste des droits un droit
void CApp::RemoveDroit( QString &listDroits, QString droitToRemove)
{if (droitToRemove.find("med") != -1) return;                                  // on ne peut pas enlever le droit medintux !!!
 QStringList list = QStringList::split("-", listDroits);
 listDroits       = "";
 for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
     {if ((*it) != droitToRemove)  listDroits +=  "-" + (*it);
     }
}
//--------------------------------------------- IsAdministrator -------------------------------------------------------------------
// ACTION lors d'un changement d'utilisateurs aprés avoir bien sur entré l'éventuel mot de passe
// si les droits du nouvel utilisateur ne sont pas presents au niveau du programme on les rajoute
bool CApp::IsAdministrator()
{if (m_Droits.find("adm") != -1) return TRUE;
 return FALSE;
}

//--------------------------------------------- IsThisDroitExist -------------------------------------------------------------------
// ACTION vérifie si le droit  (trois caractères style med adm sgn ....) existent
//        6 fois plus rapide que : listDroits.find(droitToFind) != -1
bool CApp::IsThisDroitExist(const char *listDroits, const char *droitToFind)
{char *pt  = (char*)listDroits;
 while (*pt)
    {if (pt[1]==droitToFind[0] && pt[2]==droitToFind[1] &&pt[3]==droitToFind[2]) return TRUE;
     pt += 4;
    }
 return FALSE;
}

//--------------------------------------------- GetDroitsPossibles -------------------------------------------------------------------
// ACTION
QString CApp::GetListDroitsPossibles(QStringList *plist_droits, int clearList /*= 1*/)
{QString chaineDroit = "";
 QString str         = "";
 QString allDroitsStr;

 CGestIni::Param_UpdateFromDisk(m_PathAppli + "Images/ListeDroits.txt" , allDroitsStr);
 if (allDroitsStr.length()<=0)
    {allDroitsStr    = "Peut utiliser MedinTux (med)-"
                       "Peut administrer MedinTux (adm)-"
                       "Utilisateur signataire (sgn)-"
                       "Peut créer des ordonnances (orc)-"
                       "Peut imprimer des ordonnances (orp)-"
                       "Peut créer des courriers (coc)-"
                       "Peut imprimer des courriers (cop)-"
                       "Peut créer des certificats (cec)-"
                       "Peut imprimer des certificats (cep)-"
                       "Peut créer des observations (obc)-"
                       "Peut imprimer des observations (obp)";
    }
 allDroitsStr         = allDroitsStr.stripWhiteSpace();
 allDroitsStr         = allDroitsStr.remove("\n");
 allDroitsStr         = allDroitsStr.remove("\r");
 QStringList tmp_list = QStringList::split("-", allDroitsStr);
 if (plist_droits)
    { if (clearList) plist_droits->clear();
      *plist_droits += tmp_list;
    }

 for ( QStringList::Iterator it = tmp_list.begin(); it != tmp_list.end(); ++it )
     { int pos = (*it).find("(");
       if (pos != -1) chaineDroit += "-" + (*it).mid(pos+1,3);
     }
 return chaineDroit;
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
 if (IsAdministrator()||m_pCMoteurBase->VerifyUserPassWord(newUser, ""))
    { m_User            = newUser;
      //m_SignUser        = newSignUser;
      m_CriptedPassWord = "";
      //saveLastUserOn_Ini(passwordMustBeRecorded);
      ok = 1;
    }
 //................ sinon demander le mot de passe ..................................
 else
    {CDlg_PasswordGet *dlg = new CDlg_PasswordGet(&cripted_password, parent);
    if (dlg == 0)             return ok;

     if (dlg->exec()==QDialog::Accepted && m_pCMoteurBase->VerifyUserPassWord(newUser, cripted_password) )
        {m_CriptedPassWord = cripted_password;
         m_User            = newUser;
         //m_SignUser        = newSignUser;
         saveLastUserOn_Ini( dlg->IsPasswordMustBeRecord() );
         ok = 1;
        }
     delete dlg;
    }
 return ok;
}
//---------------------------------------------- saveLastUserOn_Ini ------------------------------------------------------------
void CApp::saveLastUserOn_Ini( bool passwordMustBeRecorded /* = 1 */)
{    QString data_ini="";
     CGestIni::Param_UpdateFromDisk(m_PathIni , data_ini);

     if ( passwordMustBeRecorded )
        {CGestIni::Param_WriteParam( &data_ini, "last login", "login data",  m_User, QString("#") + m_CriptedPassWord);
         CGestIni::Param_UpdateToDisk(m_PathIni, data_ini);
        }
     else
        {CGestIni::Param_WriteParam( &data_ini, "last login", "login data",  "", "");
         CGestIni::Param_UpdateToDisk(m_PathIni, data_ini);
        }
}
