/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 ->2009 by Sevin Roland  and Data Medical Design *
 *   E-Mail: roland-sevin@medintux.org                                            *
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
#include <qfiledialog.h>
#include <qclipboard.h>
#include <qdragobject.h>


#include "CPreview.h"
#include "CApp.h"
#include "../../MedinTuxTools/cdlg_passwordget.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "../../MedinTuxTools/ThemePopup.h"  // Gestion du thème de l'appli

#include "Rubriques.h"
#include "ui/Dlg_MedicaTux.h"
//#include "CMDI_GroupedView.h"
#include "CMDI_Prescription.h"
#include "CMDI_Observation.h"
#include "CMDI_Terrain.h"
//#include "CMDI_Documents.h"
#include "CMDI_Ident.h"
#include "CMDI_ChoixPatient.h"
#include "drtux.h"
#include "CMDI_Generic.h"

/*! \class CApp
 * Instance générale de l'application
*/

CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application
static char NUM_VERSION[]     = "==##@@==2.16.007==@@##==";
//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::~CApp()
{
 #ifdef SESAMVITALE_VERSION
    if (m_pCps)
       {delete m_pCps;
        m_pCps = 0;
       }
 #endif
if (m_pCCoolPopup) delete m_pCCoolPopup;
if (m_pCMoteurBase)
  {delete m_pCMoteurBase;
   m_pCMoteurBase = 0;
  }

if (m_pCMedicaBase)
   {delete m_pCMedicaBase;
    m_pCMedicaBase = 0;
   }
QMimeSourceFactory::removeFactory ( &m_C_ImageList );      // ne rend plus dispo la liste d'images au système (car va être détruite)
}

//--------------------------------------------- CApp -------------------------------------------------------------------......................
CApp::CApp(QString mui_name, int & argc, char ** argv)
    : C_MUI_App(mui_name,  argc, argv )
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

    m_NUM_VERSION            = NUM_VERSION;
    //........................... iniitaliser variables à zero ...............................................................................
    m_pCMoteurBase           = 0;          // moteur de la base de donnée
    m_pCMedicaBase           = 0;          // moteur de la base de donnée medicaments
    m_pCCoolPopup            = 0;
    m_CCoolPopupTempo        = 1000;
    m_UserDataParam          = "";
    m_IsNomadeActif          = FALSE;
#ifdef SESAMVITALE_VERSION
    m_pCps                   = 0;          // moteur de la carte vitale
#endif
    //............................ ok cette appli est déjà active, la mettre au premier plan (sort avec G_pCApp = 0 ) .......................
    //                             mettre absoluement aprés initialisation des variables m_pCMoteurBase et  m_pCMedicaBase m_pCps à zero
    //                             car testées si zero lors passage dans destructeur  ~CApp()
    QFileInfo qfi(argv[0]);
    m_PathAppli              =  CGestIni::Construct_PathBin_Module("drtux",  qfi.dirPath (true));
    if (sendMessage("Wake up!") )  { activateMainWidget ();  return ; }

    //.................... recuperer path de demarrage de l'appli ...........................
    m_pDrTux                 = 0;
    m_pCMoteurBase           = 0;          // moteur de la base de donnée
    m_pCMedicaBase           = 0;          // moteur de la base de donnée medicaments
    m_pAtcd_Code             = 0;          // moteur des antécédents
    m_User                   = "";
    m_SignUser               = "";
    m_CriptedPassWord        = "";
    m_Droits                 = "";
    m_MessVisuDocNotPossible = TR("Vous ne possédez pas les droits\nvous permettant de voir ce document");
    m_IsAppStartWithUser     = FALSE;
    m_TimerVerrouDelay       = 0;
    m_LastError              = "";
    m_DrTuxParam             = "";
    m_ID_Doss                = "";
    m_NumGUID                = "";
    m_DossNom                = "";
    m_DossPrenom             = "";
    QMimeSourceFactory::addFactory ( &m_C_ImageList );            // rend dispo la liste d'images au système
    //.............................. initialiser variables locales ..........................................
    QString qstr           = "";
    QString val1           = "";
    QString val2           = "";
    QString val3           = "";
    QString val4           = "";
    QString qstr2          = "";
    QString port           = "";
    QString baseCfg        = "";

    m_Atcd_SortBy      = CApp::SortByRubrique;           // Valeur par défaut pour trie des ATCD : par rubrique
    m_Var_SortBy       = CApp::SortByDate;               // Valeur par défaut pour trie des variables : par date
    m_VarValue_SortBy  = CApp::SortByDate;               // Valeur par défaut pour trie des variables : par date


    //              const char* user    /* = 0*/,  const char *sign_user    /* = 0*/, const char* num_GUID   /* = 0*/,  const char* id_doss /* = 0*/,
    //              const char* dossNom /* = 0*/,  const char *dossPrenom   /* = 0 */,
    //              const char *iniFile /* = 0*/,  const char *cfgBaseFile  /* = 0*/

    //....................... charger le fichier de configuration des bases ..................................
    //                        il peut etre soit donne dans les arguments soit à aller chercher en local
    if (argc >= 4)                  m_ID_Doss      =     argv[4];
    if (argc >= 8 && argv[8])      {m_PathCfg_Base =     argv[8];
                                   }
    else                           {m_PathCfg_Base =     m_PathAppli + "DataBase.cfg";
                                   }
    CGestIni::Param_UpdateFromDisk(m_PathCfg_Base, baseCfg);

    //.......................Charger le chemin du fichier de configuration .............................................
    //                       il peut etre soit donne dans les arguments soit à aller chercher en local
    if (argc >= 7 &&  argv[7])      m_PathDrTuxIni =     argv[7];
    else                            m_PathDrTuxIni =     m_PathAppli + "drtux.ini";
    /*
    QString mess = "";
    for (int i=0; i<argc;++i)
        { QString arg_str = tr("Argument Numero %1 : '%2' \n").arg(QString::number(i),argv[i]);
          mess += arg_str;
          qDebug (tr("Argument Numero %1 : '%2'").arg(QString::number(i),argv[i]));
        }
    QMessageBox::warning ( 0, tr(" arguments"), mess ,
                              tr("Ok"), 0, 0,
                              1, 1 );
    */
    //.......................Charger les parametres .ini de l'application .............................................
    CGestIni::Param_UpdateFromDisk(m_PathDrTuxIni, m_DrTuxParam);
    QStringList list = get_PossiblesRubNameList();
    //.......................y recuperer les rubriques automatiques .............................................
    /*
    //    [Rubriques Automatiques]
    //         1= Biologie|bi|20070000
    //         2= Imagerie|im|20040000
    QStringList list;
    CGestIni::Param_GetList(m_DrTuxParam, "Rubriques Automatiques", "",  list , 1);  // 1 pour on les veut stripes
    //.......................y ajouter les rubriques automatiques obligatoires .............................................
    list.prepend ("Vigie|cl|20050000");
    list.prepend ( CMDI_Ident::S_GetRubName()        +"|ie|"+QString::number(CMDI_Ident::S_GetType()));
    list.prepend ("Documents|do|20080000");
    list.prepend ( CMDI_Prescription::S_GetRubName() +"|or|"+QString::number(CMDI_Prescription::S_GetType()));
    list.prepend ( CMDI_Terrain::S_GetRubName()      +"|at|"+QString::number(CMDI_Terrain::S_GetType()));
    list.prepend ("Observation|ob|20030000");
    */
    // m_mapDroitPrefix[key] = prefix; // ???
    //--------------------------- mapper les prefix de droit avec nom des rubriques --------------------------
    //  nom rubrique|prefix droit|type rubrique
    for (int i=0; i< (int)list.count(); ++i)
        {QString data = list[i];
         QStringList subData = QStringList::split("|",data);
         m_ListRubName.append(subData[0]);
         // qDebug (tr("append rubrique : '%1' type : '%2' prefixDroit : '%3'").arg(subData[0],subData[2],subData[1]));
         m_mapNameRubType[subData[0]]  =  subData[2].toInt();
         m_mapNameRubInfos[subData[0]] =  subData[1];   // pour l'instant on ne place que le prefix des droits
        }
    //................................ initialiser les différents chemins Globaux ............................
    m_PathGlossaireIsLocal = "";
    if (CGestIni::Param_ReadParam( m_DrTuxParam, "Glossaire", "Path", &m_PathGlossaire, &m_PathGlossaireIsLocal) !=0 ) // zero = pas d'erreur
       { m_PathGlossaire =  m_PathAppli + "../../Glossaire/";
       }
    m_PathGlossaire = ResolvPathGlossaire(m_PathGlossaire);

    if (CGestIni::Param_ReadParam( m_DrTuxParam, "Theme", "Path", &m_PathTheme) !=0 )  // zero = pas d'erreur
       { m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
       }
    if (!QDir(m_PathTheme).exists())    m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
    if (QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
    m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
    Theme::setPath(m_PathTheme);
    //...................... police de caracteres ...................................
    QFont font = this->font();
    CGestIni::Param_ReadParam( m_DrTuxParam, "Font", "PointSize", &val2 );
    CGestIni::Param_ReadParam( m_DrTuxParam, "Font", "Family", &val1 );
    if (val1.length())
       font.setFamily(val1);
    if (val2.length())
       {int size = val2.toInt();
        font.setPointSize( size );
        Theme::setFontSize_Menu( size );
       }
    setFont (font);
    //.................... je nettoie le path et rajoute / à la fin ..........................................
    m_PathGlossaire         =  QDir::cleanDirPath(m_PathGlossaire) + "/";
    m_PathTerrain           =  m_PathGlossaire + "Terrain/";
    m_PathImages            =  m_PathAppli     + "Ressources/";

    //.............................. delay de verification du verrrou .................................
    CGestIni::Param_ReadParam( m_DrTuxParam, "Connexion", "Verification Verrou", &val1 );  // splash utilisé provisoirement
    if (val1.length()) m_TimerVerrouDelay = val1.toInt();
    //...................... nomadisme ......................................................
    QString connexionVar = "";
    if (CGestIni::Param_ReadParam(m_DrTuxParam,"Connexion", "nomadisme", &val1)==0 && val1.lower()[0]=='a')  // zero = pas d'erreur
       {m_IsNomadeActif = TRUE;
       }
    if (m_IsNomadeActif) connexionVar     = "Nomade";
    else                 connexionVar     = "Master";
    //................................lire les parametres de connexion ...................................................
    //                          ouvrir et initialiser le moteur de base de données
    if (CGestIni::Param_ReadParam( m_DrTuxParam, "Connexion", connexionVar, &val1, &val2, &val3, &val4, &qstr, &port) ==0)  // zero = pas d'erreur
       { m_pCMoteurBase = new CMoteurBase( val1 ,                   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                           val2,                    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                           val3,                    // = "root"       user
                                           val4,                    // = ""           password
                                           qstr,                    // = "localhost"  hostname  qstr
                                           port,
                                           baseCfg,
                                           "DRTUX_BASE",
                                           &m_LastError);
       }
   else
       {m_LastError = TR("<u><br>Les données de connexion :</u><br>          Section : <b>Connexion</b>, <br>          Variable : <b>")+connexionVar+tr("</b> : Driver, BaseName,User, Password, Hostname <br> n'ont pas été trouvées dans le fichier : \n<b>") + m_PathDrTuxIni + "</b>";
       }

   if (m_pCMoteurBase==0)                {                                         m_LastError.prepend(TR("<u>ECHEC DE LA CONNEXION A LA BASE DE DONNÉE : </u>     m_pCMoteurBase ==0 <br>"));              return;}
   if (m_pCMoteurBase->m_DataBase ==0)   {delete m_pCMoteurBase; m_pCMoteurBase=0; m_LastError.prepend(TR("<u>ECHEC DE LA CONNEXION A LA BASE DE DONNÉE : </u>     m_pCMoteurBase->m_DataBase == 0 <br>")); return;}
   //....................... positionner le bon mode openClose ...........................................................................
   m_pCMoteurBase->setOpenCloseMode(CGestIni::Param_ReadUniqueParam(m_DrTuxParam, "Connexion","Fermer requetes").toInt());
  //......................... initialiser la base therapeutique ................................................
   m_pCMedicaBase = new CMedicaBase(m_PathAppli + "MedicaBase.ini", m_PathAppli + "MedicaBase.cfg");
   if (m_pCMedicaBase==0) {delete m_pCMoteurBase; m_pCMoteurBase=0;
                           m_LastError = TR("La base de données thérapeutique n'a pu être initialisée correctement");
                           return;
                          }
    //..................................... utilisateurs ...........................................................
    //                        il peut etre soit donne dans les arguments soit à aller chercher en local
    if (argc >= 1 && argv[1])       m_User     =     argv[1];
    else                            m_User     =     "";
    if (argc >= 2 && argv[2])       m_SignUser =     argv[2];
    else                            m_SignUser =     "";
    //....................... identification utilisateur .............................................................
    //                        si vide, (=="") aller chercher dans fichier DrTux.ini
    m_IsAppStartWithUser = FALSE;
    if ( m_User.length() == 0 )
       {CGestIni::Param_ReadParam( m_DrTuxParam, "Derniere Session", "Utilisateur", &m_User, &m_SignUser);  // zero = pas d'erreur
        CGestIni::Param_ReadParam( m_DrTuxParam, "Derniere Session", "Password",    &m_CriptedPassWord);
        if (m_CriptedPassWord.left(1)=="#") m_CriptedPassWord = m_CriptedPassWord.mid(1);                       //  si crypté (precede de #)
        else                                CMoteurBase::PassWordEncode(m_CriptedPassWord);                     //  si non crypté (precede de rien) alors le crypter
        if ( m_User.length()     != 0 &&  m_SignUser.length() != 0   &&    m_pCMoteurBase->VerifyUserPassWord(m_User, m_CriptedPassWord) )       //                   mot de passe fourni correct
           { m_IsAppStartWithUser = TRUE;
           }
       }
    else
       {m_IsAppStartWithUser = TRUE;
       }
    //............................. si l'utilisateur est le signataire alors les droit sont les siens ......................................................
    Map_Prefix_Droits();            // initialiser la map des prefix droits type de document

    if (m_User == m_SignUser)
       {m_Droits = m_pCMoteurBase->GetFieldValue(   m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_DROITS,   // field to get
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,    // field to match
                                                    m_User);                                               // value to match
       }
    //............................. si l'utilisateur n'est pas le signataire alors les droits sont ceux accordés par le signataire ........................
    else
       {m_Droits = m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  m_SignUser , m_User);
       }
    //............................. mode debug de la base de donnees ............................
    m_pCMoteurBase->m_Debug = CGestIni::Param_ReadUniqueParam( m_DrTuxParam, "Connexion", "DebugMode").toInt();
    qDebug(QString(" DebugMode=")+QString::number(m_pCMoteurBase->m_Debug));
    //............................. recuperer le message de page non visualisable ............................
    CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/RubriquesTextes/VisuDocNotPossible.html", m_MessVisuDocNotPossible);
    //................................ initialiser le système d'icones commun ..............................................................
    SetIconTherapeutique();       // positionne le bon symbole selon Vidal ou Medicabase

    //......................... carte PS ....................................................................................................
#ifdef SESAMVITALE_VERSION
    m_pCps = new C_Cps;
    qDebug (TR("Gestion Sesam Vitale active"));
#else
    qDebug (TR("Gestion Sesam Vitale inactive"));
#endif
    //.......................... initialiser la liste des menus contextuels .............................
    //                           specifiques a chaque rubrique
    CGestIni::Param_GetMap(m_DrTuxParam, "MenuContextuel", "",  m_MapMenuRubriques , 1);   // liste des menus permanents de chaque rubrique

    //......................... declarer l'application au ...............................................
    //                          système d'instance unique
    initialize();
    setGlobalMouseTracking( TRUE );
    m_pCMoteurBase->GetPatientNomPrenomPk( 1, m_ID_Doss,  &m_DossNom,   &m_DossPrenom, &m_NumGUID);
    //qDebug (TR("FROM arg  Nom : '%1' Prenom : '%2' pk : '%3' GUID : '%4'").arg(m_DossNom,m_DossPrenom,m_ID_Doss,m_NumGUID));
    //......................  si démarrage du bureau rechercher le dernier dossier ...................................
    if (m_NumGUID.length() == 0)
       {READ_LOCAL_PARAM(m_DrTuxParam, "Derniere Session", "Patient", &m_NumGUID, &m_ID_Doss, &m_DossNom, &m_DossPrenom );  // zero = pas d'erreur
        //qDebug (TR("FROM drtux.ini Nom : '%1' Prenom : '%2' pk : '%3' GUID : '%4'").arg(m_DossNom,m_DossPrenom,m_ID_Doss,m_NumGUID));
        m_pCMoteurBase->GetPatientNomPrenomPk( 1, m_ID_Doss,  &m_DossNom,   &m_DossPrenom, &m_NumGUID);
        //qDebug (TR("FROM pk Nom : '%1' Prenom : '%2' pk : '%3' GUID : '%4'").arg(m_DossNom,m_DossPrenom,m_ID_Doss,m_NumGUID));
       }

    G_pCApp                  =    this;
    //..............   IMPORTANT : a ne faire qu'apres initialisation de G_pCApp ........................
    //___________________________________________________________________________________________________
    //....................... Recuperer parametres DE LA BASE DE DONNEE..................................
    //                            correspondant a l'user en cours
    m_pCMoteurBase->Param_GetParam( &USER_PARAM , m_User);
    //___________________________________________________________________________________________________
    if ( m_PathGlossaireIsLocal.length() )       // si le chemin local doit etre utilise
       { if ( !QFile::exists(m_PathGlossaire) )  // si n'existe pas tenter avec path glossaire defini dans USER_PARAM
            { qDebug(tr("Local glossaire path '%1' don't exists trying with user's glossaire path").arg(m_PathGlossaire));
              m_PathGlossaireIsLocal = "";
              m_PathGlossaire        = "";
            }
         else
            { qDebug(tr("Local glossaire path '%1' will be used.").arg(m_PathGlossaire));
            }
       }
    if (m_PathGlossaire.length()==0 || m_PathGlossaireIsLocal.length()==0)
       { 
         if (READ_USER_PARAM( USER_PARAM, "Glossaire", "Path", &val1)==0)  // zero = pas d'erreur
            {m_PathGlossaire = ResolvPathGlossaire(val1);
             qDebug(tr("User's glossaire path '%1' will be used.").arg(m_PathGlossaire));
            }
       }
    // ............................... initialiser le pointeur de gestion des antécédents ..................................................
    //
    // qui sera utilisé par Dock_Menu, CMDI_Terrain et CDevilCrucible
    m_pAtcd_Code = new Atcd_Code(m_pCMoteurBase);
}
//------------------------------- delAccents-----------------------------------------------------
/*! \brief converti les caracteres accentues d'une chaine en caracteres non accentues
*/
QString CApp::delAccents(const QString &src)
{/*
 //........... methode bourin mais sure ................
 QString dst = src.latin1();
 dst.replace(QRegExp("[ÁÀÄÂ]"), "A");
 dst.replace(QRegExp("[áàäâ]"), "a");
 dst.replace(QRegExp("[ÉÈËÊ]"), "E");
 dst.replace(QRegExp("[éèëê]"), "e");
 dst.replace(QRegExp("[ÍÌÏÎ]"), "I");
 dst.replace(QRegExp("[íìïî]"), "i");
 dst.replace(QRegExp("[ÓÒÖÔ]"), "O");
 dst.replace(QRegExp("[óòöô]"), "o");
 dst.replace(QRegExp("[ÚÙÜÛ]"), "U");
 dst.replace(QRegExp("[úùüû]"), "u");
 return dst;
*/

//     static const char m_accentCharacters[]  = "ÀÁÂÃÄÅàáâãäåÒÓÔÕÖØòóôõöøÈÉÊËèéêëÌÍÎÏìíîïÙÚÛÜùúûüÿÑñÇç°";
// Represents the letters which have no accents.
//    static const char m_cleanCharacters[]   = "AAAAAAaaaaaaOOOOOOooooooEEEEeeeeIIIIiiiiUUUUuuuuyNnCc-";
// Map the letters which have accents and the letters which haven't.
//    static  QMap <char, char> m_AccentDic;
typedef QMap<ushort, char> accentMap;
int i = 0;
if (m_AccentDic.size()==0)   // si lors de la premiere utilisation le dico est vide --> le remplir
   { // Represents the letters which have accents.
     QString accentCharacters  = QString::fromUtf8 ("ÉÈÊËÀÁÂÃÄÅàáâãäåÒÓÔÕÖØòóôõöøèéêëÌÍÎÏìíîïÙÚÛÜùúûüÿÑñÇç°");
     // Represents the letters which have no accents.
     QString cleanCharacters   = "EEEEAAAAAAaaaaaaOOOOOOooooooeeeeIIIIiiiiUUUUuuuuyNnCc-";

     for (i=0; i< (int) accentCharacters.length(); ++i)
          { ushort u         = accentCharacters.at(i).unicode();
            char   c         = cleanCharacters.at(i).latin1();
            m_AccentDic[u]   = c;
          }
   }
QString dst = "";
for ( i=0; i < (int)src.length() ;++i)
    { ushort  u  = src.at(i).unicode();
      accentMap::Iterator it = m_AccentDic.find ( u );   // on recherche si une clef de caractere accentue existe pour ce caractere
      if (it != m_AccentDic.end())
         { dst += it.data();
         }
      else
         { dst += src.at(i);
         }
    }
 return dst; 
}

//--------------------------------------------- debugMode -------------------------------------------------------------------
/*! \brief retourne si l'application est  en mode debug. Le mode debug est determine dans la section [Connexion] par la variable DebugMode = 1
*/
int CApp::debugMode()
{if (m_pCMoteurBase==0) return 0;
 return m_pCMoteurBase->m_Debug;
}
//--------------------------------------------- quit -------------------------------------------------------------------
/*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
*/
void CApp::quit()
{emit Sign_QuitterRequired();    // se connecter a tous ceux connectes a ce signal pour qu'il puissent Sauver le meubles.
 QApplication::quit();
}
//--------------------------------- GetIDCurrentDoc -----------------------------------------------------
/*! \brief Retourne le CRubRecord correspondant au document actuellement affiché dans la CMDI gérant le doc_type.
*/
CRubRecord  *CApp::GetIDCurrentDoc(const QString &doc_type_in, const DOCUMENT_DISPLAY_MAP  *currentDocDisplayMap)
{CRubRecord  *pCRubRecordRet = 0;
 int                    pos  = -1;
 QString            libelle  = "";
 QString            doc_type = doc_type_in;

 if (!G_pCApp->m_pDrTux) return pCRubRecordRet;

 //................ si le type contient le sous type .................................
 if (doc_type.length() && (pos=doc_type.find('|')) != -1)
    {libelle  = doc_type.mid(pos+1).stripWhiteSpace();
     doc_type = doc_type.left(pos).stripWhiteSpace();
    }

 //................... si non précisé retourner la rubrique courante..............................
 if (doc_type.length()==0||doc_type==TR("*"))
    { CMDI_Generic *pCMDI_Generic   =  G_pCApp->GetCurrentRubrique();
      if (pCMDI_Generic)
         {RUBREC_LIST::iterator rit = pCMDI_Generic->Current_RubList_Iterator();
          if (rit != G_pCApp->m_pDrTux->m_RubList.end())
             { pCRubRecordRet = &(*rit);
               if (libelle.length())   // si libelle fourni on verifie si il correspond
                  {if (pCRubRecordRet->m_Libelle==libelle) return pCRubRecordRet;
                  }
               else
                  {                                        return pCRubRecordRet;
                  }
             }
         }
    }

 //................... on cherche celui de la map (ceux en cours d'affichage)..............................
 //                    si libelle fourni on verifie si il correspond
 DOCUMENT_DISPLAY_MAP::ConstIterator  it = currentDocDisplayMap->find ( doc_type );
 if (! (it == currentDocDisplayMap->end()) )
    {pCRubRecordRet = it.data();
     if (libelle.length())
        {if (pCRubRecordRet->m_Libelle==libelle)           return pCRubRecordRet;
        }
     else
        {                                                  return pCRubRecordRet;
        }
    }
 //...................................... si pas trouve on va rechercher le dernier .........................
 //                                       correspondant au type numerique dans la liste generale
 QString doc_type_num = doc_type;
 if ( ! (doc_type[0]>='0' && doc_type[0]<='9') ) // si pas exprime sous forme numerique alors on convertit
    {doc_type_num = G_pCApp->RubNameToStringType(doc_type);
    }
 //................... si pas trouve on cherche le dernier correspondant...................................
 RUBREC_LIST::Iterator ut ;
 CRubRecord  *pCRubRecord = 0;   // iterera sur tous les record
 pCRubRecordRet           = 0 ;  // retiendra le dernier Ok de la liste
 for ( ut = G_pCApp->m_pDrTux->m_RubList.begin(); ut != G_pCApp->m_pDrTux->m_RubList.end(); ++ut )
     {pCRubRecord = &(*ut);
      if (libelle.length())   // si libelle fourni on verifie aussi si il correspond
         {if ( pCRubRecord->m_Type==doc_type_num && pCRubRecord->m_Libelle==libelle) pCRubRecordRet = pCRubRecord;
         }
      else                    // sinon on ne verifie que le type
         {if ( pCRubRecord->m_Type==doc_type_num)                                    pCRubRecordRet = pCRubRecordRet;
         }
     }
 return pCRubRecordRet;
}
//------------------------------------- getListNameRubriqueMenu -----------------------------------------------------
/*! \brief retourne le nom de la liste du menu permanent affecte a une rubrique. Dans le fichier d'initialisation .ini de l'application\
     se trouve une section [MenuContextuel] dans laquelle doit etre indiquee : Nom de la rubrique = liste a activer pour cette rubrique
 *  \param typeRubrique : le type de la rubrique.
 *  \return le nom de la liste du menu permanent affecte a une rubrique ou vide si non trouve (QString::null)
*/
QString CApp::getListNameRubriqueMenu(int typeRubrique)
{QString rubName  = G_pCApp->TypeToRubName( typeRubrique );
 return getListNameRubriqueMenu(rubName);
}
//------------------------------------- getListNameRubriqueMenu -----------------------------------------------------
/*! \brief retourne le nom de la liste du menu permanent affecte a une rubrique. Dans le fichier d'initialisation .ini de l'application\
     se trouve une section [MenuContextuel] dans laquelle doit etre indiquee : Nom de la rubrique = liste a activer pour cette rubrique
 *  \param typeRubrique : le type de la rubrique.
 *  \return le nom de la liste du menu permanent affecte a une rubrique ou vide si non trouve (QString::null)
*/
QString CApp::getListNameRubriqueMenu(const QString &rubName)
{QMap <QString, QString>::Iterator it = m_MapMenuRubriques.find(rubName);
 if (it != m_MapMenuRubriques.end())
    {QString listName = it.data();
     return listName;
    }
 return QString::null;
}

//--------------------------------------------- get_PossiblesRubNameList --------------------------------------
QStringList  CApp::get_PossiblesRubNameList()
{
    //.......................recuperer les rubriques automatiques definies par l'utilisateur.............................................
    //    [Rubriques Automatiques]
    //         1= Biologie|bi|20070000
    //         2= Imagerie|im|20040000
    QStringList list;
    CGestIni::Param_GetList(m_DrTuxParam, "Rubriques Automatiques", "",  list , 1);  // 1 pour on les veut stripes
    //.......................y ajouter les rubriques automatiques obligatoires .............................................
    list.prepend (CMDI_ChoixPatient::S_GetRubName() + "|pl|"+QString::number(CMDI_ChoixPatient::S_GetType()));
    list.prepend ("Vigie|cl|20050000");
    list.prepend ( CMDI_Ident::S_GetRubName()        +"|ie|"+QString::number(CMDI_Ident::S_GetType()));
    list.prepend ("Documents|do|20080000");
    list.prepend ( CMDI_Prescription::S_GetRubName() +"|or|"+QString::number(CMDI_Prescription::S_GetType()));
    list.prepend ( CMDI_Terrain::S_GetRubName()      +"|at|"+QString::number(CMDI_Terrain::S_GetType()));
    list.prepend ("Observation|ob|20030000");
    return list;
}

//--------------------------------------------- SetPathGlossaire --------------------------------------
QString  CApp::SetPathGlossaire(QString pathGlossaire)
{  if (pathGlossaire[0] == '.')
      {  if (QDir(pathGlossaire).isRelative())
            {pathGlossaire.prepend(m_PathAppli);
            }
      }
   if ( !QFile::exists( pathGlossaire  ))
      {pathGlossaire = m_PathAppli + "../../Glossaire/";
      }
   m_PathGlossaire = QDir::cleanDirPath(pathGlossaire)+"/";
   return m_PathGlossaire;
}

//--------------------------------------------- ResolvPathGlossaire --------------------------------------------
QString CApp::ResolvPathGlossaire(QString pathGlossaire)
{
/*
#ifdef Q_WS_X11
    qDebug(pathGlossaire);
    //.................... ajuster resolution d'impression .........................................
    if (pathGlossaire[0] == '/' && pathGlossaire[1] == '/')  // zero = pas d'erreur
       {pathGlossaire.replace('/','\\');
       }

    //.................... ajuster resolution d'impression .........................................
    if (pathGlossaire[0] == '\\' && pathGlossaire[1] == '\\')  // zero = pas d'erreur
       {
        pathGlossaire.prepend("smb:");
       }

    qDebug(pathGlossaire);
#endif
*/
  if (pathGlossaire[0] == '.')
      {  if (QDir(pathGlossaire).isRelative())
            {pathGlossaire.prepend(m_PathAppli);
            }
      }
   else if ( (pathGlossaire[0] == '~') || (pathGlossaire[0] == '$') )
      {  pathGlossaire.replace("~", QDir::homeDirPath() );
         pathGlossaire.replace("$home", QDir::homeDirPath(), false);
      }
   if ( !QFile::exists( pathGlossaire  ))
      {pathGlossaire = QDir::cleanDirPath(m_PathAppli + "../../Glossaire/");
      }
   if ( !QFile::exists( pathGlossaire  ))
      {pathGlossaire = QDir::cleanDirPath(m_PathAppli + "../../../Glossaire/");
      }
   return QDir::cleanDirPath(pathGlossaire)+"/";

}

//--------------------------------------------- PathDocumentToType ---------------------------------------------
/*! \brief retourne le type de rubrique associé avec le chemin du document type du Glossaire fourni en entrée
 *  \param path : chemin du document type dont on cherche le type de rubrique auquel il est associé
 *  \return Renvoie le type de rubrique, zéro si pas de rubrique trouvée
*/

int CApp::PathDocumentToType( const QString &path , QString *pRubNameToFind)
{QString toFind = "";
 for ( QStringList::Iterator it = m_ListRubName.begin(); it != m_ListRubName.end(); ++it )
     {toFind = m_PathGlossaire + *it;
      if  (path.find(toFind )!=-1)
          { if ( pRubNameToFind ) *pRubNameToFind = *it;
            return RubNameToType(*it);
          }
     }
 if ( pRubNameToFind ) *pRubNameToFind = "";
 return 0;
}
//----------------------------------------------- putCurrentRubriqueOnModifiedState ------------------------------------------------------------------------
/*! \brief place la rubrique courante en etat modifie
*/
void CApp::putCurrentRubriqueOnModifiedState()
{CMDI_Generic  *pCMDI_Generic = GetCurrentRubrique();
 if (pCMDI_Generic) pCMDI_Generic->setDataToModifiedState();
}
//--------------------------------------------- ApproximativeRubNameToType ----------------------------------------------
/*! \brief retourne le type de rubrique le dont le nom le plus probable contenu dans le texte fourni en entrée
 *  \param path : texte dont on cherche le type de rubrique auquel il est associé
 *  \return Renvoie le type de rubrique
*/

int CApp::ApproximativeRubNameToType( const QString &texte , QString *pRubNameToFind)
{QString toFind = "";
 for ( QStringList::Iterator it = m_ListRubName.begin(); it != m_ListRubName.end(); ++it )
     {toFind = (*it).left(5);             // on se sert des 5 premieres lettres du nom de la rubrique
      if  (texte.find(toFind )!=-1)
          { if ( pRubNameToFind ) *pRubNameToFind = *it;
            return RubNameToType(*it);
          }
     }
 if ( pRubNameToFind ) *pRubNameToFind = "";
 return 0;
}

//--------------------------------------------- RubNameToStringType --------------------------------------
/*! \brief retourne sous forme textuelle le type de rubrique associée au nom de rubrique fourni en entrée
 *  \param rubName : const QString & nom de la rubrique dont on recherche le type
 *  \param f_typ : FIND_TYPE  SI  FIND_TYPE::Always alors le type est toujours retourné  SI FIND_TYPE::ifExist alors le type n'est retourné que si la rubrique est présente dans DrTux
 *  \return Renvoie le type de rubrique
*/
QString CApp::RubNameToStringType( const QString &rubName , FIND_TYPE f_typ /*= FIND_TYPE::Always*/)
{return QString::number(RubNameToType( rubName , f_typ ));
}
//--------------------------------------------- resolvePath --------------------------------------
/*! \brief resoud un chemin comportant les eventuelles macros $Glossaire $Vigie par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie
*/
QString CApp::resolvePath(QString pathFile)
{ return  CGenTools::resolvePath(pathFile, m_PathAppli, m_PathGlossaire);
}

//--------------------------------------------- RubNameToType --------------------------------------
/*! \brief retourne sous forme numérique le type de rubrique associée au nom de rubrique fourni en entrée
 *  \param rubName : const QString & nom de la rubrique dont on recherche le type
 *  \param f_typ : FIND_TYPE  SI  FIND_TYPE::Always alors le type est toujours retourné  SI FIND_TYPE::ifExist alors le type n'est retourné que si la rubrique est présente dans DrTux
 *  \return Renvoie le type de rubrique
*/
int CApp::RubNameToType( const QString &rubName , FIND_TYPE f_typ /*= FIND_TYPE::Always*/)
{if (f_typ == CApp::Always)
    {return m_mapNameRubType[rubName];
    }
 else
    { QWidgetList windowsList = m_pDrTux->m_pQWorkSpaceRub->windowList(QWorkspace::CreationOrder);
      int nbW = windowsList.count();
      if ( nbW == 0 ) return 0;
      for ( int i = 0; i < int(windowsList.count()); ++i )
          {CMDI_Generic *pCMDI_Generic = (CMDI_Generic *) windowsList.at(i);
           if ( pCMDI_Generic->GetRubName() == rubName) return pCMDI_Generic->GetType();
          }
    }
 return 0;
}

//--------------------------------------------- RubNameToType ---------------------------------------------
/*! \brief retourne le nom d'une rubrique associée au type de rubrique fourni en entrée
 *  \param typRub : type de la rubrique dont on recherche le nom
 *  \return Renvoie le nom de la rubrique
*/
QString CApp::TypeToRubName( int typRub )
{       QMap<QString, int>::Iterator it;
        for ( it = m_mapNameRubType.begin(); it != m_mapNameRubType.end(); ++it )
            {int typRef = it.data();
             if (typRub >= typRef && typRub <= typRef + 999) return it.key();
            }
        return QString::null;
}

//--------------------------------------------- GetCurrentRubrique --------------------------------------------
/*! \brief retourne un pointeur sur la rubrique courante (m_pDrTux doit avoir été initialisé ce qui est forcément le cas lors de l'appel)
 *  \param pRubName : pointeur sur une QString qui si il n'est pas nul contiendra le nom de la rubrique courante
 *  \param rub_type_ret : pointeur sur un int qui si il n'est pas nul contiendra le type de la rubrique
 *  \param ppCRubRecord : pointeur sur un pointeur sur CRubRecord  qui si il n'est pas nul contiendra un pointeur sur l'enregistrement en cours
 *  \return Renvoie un pointeur sur la rubrique en cours sinon vide (NULL)
*/

CMDI_Generic *CApp::GetCurrentRubrique(QString *pRubName /*=0*/, int* rub_type_ret  /*=0*/ , CRubRecord **ppCRubRecord/*=0*/ )
{if (m_pDrTux)
    {return (CMDI_Generic*) m_pDrTux->GetCurrentRubrique(pRubName,  rub_type_ret , ppCRubRecord);
    }
 return 0;
}

//--------------------------------------------- IsExistRubrique --------------------------------------------
/*! \brief retourne un pointeur sur la rubrique courante (m_pDrTux doit avoir été initialisé ce qui est forcément le cas lors de l'appel)
 *  \param pRubName : pointeur sur une QString qui si il n'est pas nul contiendra le nom de la rubrique courante
 *  \return Renvoie un pointeur sur la rubrique en cours sinon vide (NULL)
*/

CMDI_Generic *CApp::IsExistRubrique(const QString &pRubName)
{if (m_pDrTux)
    {return (CMDI_Generic*) m_pDrTux->IsExistRubrique(pRubName);
    }
 return 0;
}
//--------------------------------------------- IsExistRubrique --------------------------------------------
/*! \brief teste si une des fenêtres rubriques dont le type est fourni en entrée est ouverte dans le WorkSpace des rubriques.
 *  \return pointeur sur la rubrique si elle existe ou zero.
*/
CMDI_Generic * CApp::IsExistRubrique(int rubTyp)
{ return (CMDI_Generic *) IsExistRubrique(TypeToRubName( rubTyp ));
}

//--------------------------------------------- IsThisDroitExist ----------------------------------------------
/*! \brief Vérifie si le droit : droitToFind  (trois caractères style med adm sgn ....) existe dans la chaine de droits : listDroits 6 fois plus rapide que : listDroits.find(droitToFind) != -1
*/
bool CApp::IsThisDroitExist(const char *listDroits, const char *droitToFind)
{if (listDroits==0) return FALSE;
 char *pt  = (char*)listDroits;
 while (*pt)
    {if (pt[1]==droitToFind[0] && pt[2]==droitToFind[1] &&pt[3]==droitToFind[2]) return TRUE;
     pt += 4;
    }
 return FALSE;
}

//--------------------------------------------- SetIconTherapeutique ---------------------------------------------
/*! \brief Selon que l'utilisateur va utiliser DataSemp ou une base thérapeutique l'icone change. L'icone est stocké dans m_IconTherapeutique.
*/
int CApp::SetIconTherapeutique()
{
 if ((int)(m_pCMedicaBase->Medica_GetBaseMode() & CMedicaBase::Datasemp))
    { m_IconTherapeutique = Theme::getIconVidal();
      return CMedicaBase::Datasemp;
    }
 else
    { m_IconTherapeutique = Theme::getIconMedicaments();
      return CMedicaBase::MedicaBase;
    }
}

//--------------------------------------------- Map_Prefix_Droits -------------------------------------------------------------------
/*! \brief Creation d'une map type_document   prefix du droit allant avec ce prefix sera suffixé par 'v' pour voir 'c' pour créer 'm' pour modifier 'p' pour imprimer  'r' pour renouveler
*/
void CApp::Map_Prefix_Droits()
{QString key           =    "";
 QString prefix        =    "";
 QString defaut_map    =    "";
 CGestIni::Param_UpdateFromDisk(m_PathAppli+"Ressources/prefix_map.txt", defaut_map);       // charger le fichier de map
 if (defaut_map.length()==0)
    {defaut_map     =    "#define TYP_ORDO_CALC    |20020100|or\n"
                         "#define TYP_ORDO_LAP     |20020150|or\n"
                         "#define TYP_ORDONNANCE   |20020200|or\n"
                         "#define TYP_CERTIFICAT   |20020300|ce\n"
                         "#define TYP_COURRIER     |20020500|co\n";
    }
 defaut_map.replace("\r\n",  "\n");
 defaut_map.replace("\r"  ,  "\n");
 QStringList lines_list = QStringList::split("\n", defaut_map);
 for ( QStringList::Iterator it = lines_list.begin(); it != lines_list.end(); ++it )
     {QStringList param_list = QStringList::split('|', *it);
      key                   = param_list[1];
      prefix                = param_list[2];
      m_mapDroitPrefix[key] = prefix;
     }
}
//------------------------------ loadContentsIfBeginByTokenFile --------------------------------------------------
/*! \brief Teste si la chaine arg commence par $File et si c'est le cas la charge avec le contenu du fichier.
 *  \param  QString& arg : argument a evaluer et eventuellement charger avec le contenu d'un fichier
*/
QString CApp::loadContentsIfBeginByTokenFile(QString &arg)
{if (arg.startsWith("$File"))
    {arg = resolvePath(arg.mid(5));
     arg = CGestIni::Param_UpdateFromDisk(arg);
    }
 return arg;
}

//--------------------------------------------- GetMap_Prefix_Droits -------------------------------------------------------------------
QString CApp::GetMap_Prefix_Droits(const QString &doc_Type)
{//QMap<QString, QString>::iterator mt = G_pCApp->m_mapDroitPrefix.find (doc_Type);
 //return *mt;
 return *(m_mapDroitPrefix.find (doc_Type));
}

//--------------------------------------------- IsAppStartWithUser -------------------------------------------------------------------
bool CApp::IsAppStartWithUser()
{return m_IsAppStartWithUser;
}

//--------------------------------------------- AuthentifyAndSelectUser -------------------------------------
/*! \brief non documenté */
//  Oh dieux qui etes aux cieux
//  Reglez vos comptes en cieux
//
int CApp::AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool /*passwordMustBeRecorded  = 1 */)
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
 return ok;
}
//---------------------------------------------- saveLastUserOn_Ini -----------------------------------------
/*! \brief non documenté */
void CApp::saveLastUserOn_Ini(const QString &user, const QString &signUser, const QString &cripted_password, bool passwordMustBeRecorded /* = 1 */)
{
     if ( passwordMustBeRecorded )
        {CGestIni::Param_WriteParam( &m_DrTuxParam, "Derniere Session", "Password",    QString("#") + cripted_password);
         CGestIni::Param_WriteParam( &m_DrTuxParam, "Derniere Session", "Utilisateur", user, signUser);
         CGestIni::Param_UpdateToDisk(m_PathDrTuxIni, m_DrTuxParam);
        }
     else
        {CGestIni::Param_WriteParam( &m_DrTuxParam, "Derniere Session", "Password", "");
         CGestIni::Param_WriteParam( &m_DrTuxParam, "Derniere Session", "Utilisateur", "", "");
         CGestIni::Param_UpdateToDisk(m_PathDrTuxIni, m_DrTuxParam);
        }
}

//---------------------------------- GetImageFile -----------------------------------------------------------------------------
/*! \brief recupère avec le selecteur de fichier un non de fichier d'image
 *  \return une QString qui contient le nom du fichier d'image ou vide si annulation ou fichier inexistant.
*/

QString CApp::GetImageFileName(QString *last_path /* = 0 */, QString stringPreselect /*=""*/, int prewiew /*=1*/, QWidget *pQWidget /*=0*/)
{   //.................................. selectionner le ficher ..................................
    QString filename  = "";
    QString        fn = "";
    QString  outParam = "";

    if (fn .length()==0 && last_path)     fn = *last_path;
    if (fn .length()==0)                  fn = QFileInfo (qApp->argv()[0]).dirPath (true);

    if (stringPreselect.length()==0)
       {stringPreselect = TR( "Fichiers IMAGE (*.jpg *.jpeg *.png *.PNG *.gif *.GIF *.JPEG *.JPG);;"
                                             "Tous les fichiers (*)");
       }
    //................. choisir le fichier image src à insérer .................................................
	//const QString & dirName, const QString & filter = QString::null, QWidget * parent = 0, const char * name = 0, bool modal = FALSE
    QFileDialog *fd = new QFileDialog  ( fn,
                                         stringPreselect ,
                                         pQWidget,
                                         "OuvrirDlg"
                                        ) ;
    if (fd==0)  return filename;
    CPreview*     p = 0;
    if (prewiew)  p = new CPreview;
    if (p)
       { fd->setInfoPreviewEnabled( TRUE );
         fd->setInfoPreview( p, p );
         fd->setPreviewMode( QFileDialog::Info );
       }
    if ( (fd->exec() == QDialog::Accepted) )
       {filename = fd->selectedFile();
        if ( QFile::exists( filename )==0 ) filename = "";
        if ( filename.length() )
           { if (last_path) *last_path = QFileInfo(filename).dirPath ( TRUE );
           }
       }
    delete fd;
    return filename;
}
//------------------------------------ addPopupHierarchique --------------------------------------------------
/*! \brief Ajoute le menu de selection du type Hierarchique a un menu quelconque a partir d'un fichier
 *  \param path :        QString qui contient le chemin du fichier .ini structurant les options du menu
 *  \param pQPopupMenu : QPopupMenu* est le menu auquel rajouter ce menu hierarchique
 *  \param pRetVar :     QString* est un pointeur sur la chaine de caractre dans laquelle retourner l'option selectionnee
 *  \note  avant appel de cette fonction il convient de la connecter comme suit a
 *                                un SLOT a actionner lors selection d'une option:
 *                     connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
 *         puis dans le slot de la deconnecter :
 *                  disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
 *  LIMITATION : il ne peut y avoir qu'un menu de ce type dans un menu principal
*/
void CApp::addPopupHierarchique(const QString& path, QPopupMenu* pQPopupMenu, QString* pRetVar)  //, void  (*functionToActivateInRetSlot)()
{
 m_PopupHeadMap.clear();      //QMap<int id, ThemePopup* >
 int menuPrincipal_ID           = 0;
 QString menuStr                = "";
 QString section                = "";
 CGestIni::Param_UpdateFromDisk(path, menuStr);
 int posDebSection              = -1;
 int posEndSection              =  0;
 while ( (posDebSection  = menuStr.find("[",posEndSection)) != -1 )
    {++posDebSection;
     if ( (posEndSection = menuStr.find("]",posDebSection)) != -1 )
        {section = menuStr.mid(posDebSection,posEndSection-posDebSection).stripWhiteSpace();
         if (section.length())
            {QStringList optionsList;
             CGestIni::Param_GetList(menuStr, section, "",  optionsList);
             addPopupHierarchique(optionsList, pQPopupMenu, pRetVar, section, &menuPrincipal_ID);
             ++menuPrincipal_ID;
            }
        }
    }
}
//------------------------------------ addPopupHierarchique --------------------------------------------------
/*! \brief Ajoute le menu de selection du type Hierarchique a un menu quelconque a partir d'un fichier
 *  \param pQPopupMenu : QPopupMenu* est le menu auquel rajouter ce menu hierarchique
 *  \param optionsList : QStringListliste des options du menu
 *  \param pRetVar :     QString* est un pointeur sur la chaine de caractere dans laquelle retourner l'option selectionnee
 *  \param subName :     QString& chaine de caractre du nom du sous menu
 *  \note  avant appel de cette fonction il convient de la connecter comme suit a
 *                                un SLOT a actionner lors selection d'une option:
 *                     connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
 *         puis dans le slot de la deconnecter :
 *                  disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
 *  LIMITATION : il ne peut y avoir qu'un menu de ce type dans un menu principal
*/

void CApp::addPopupHierarchique(QStringList optionsList, QPopupMenu* pQPopupMenu, QString* pRetVar, const QString &subName, int *pMenuPrincipalID /* =0 */ )  //, void  (*functionToActivateInRetSlot)()
{ if (optionsList.count()==0) return;
  m_pOptionHierarch_Selectionne           = pRetVar;
  int menuOption_ID                       = 0;
  int menuPrincipal_ID                    = 0;
  if (pMenuPrincipalID)  menuPrincipal_ID = *pMenuPrincipalID;  // la gestion de m_PopupHeadMap[menuPrincipal_ID] se fait en dehors de la fonction il peut y avoir plusieurs options principales avec sous menu
  else                   m_PopupHeadMap.clear();                // la gestion de m_PopupHeadMap[menuPrincipal_ID] se fait dans la fonction et il n'y a qu'une option principale avec sous menu
  ThemePopup* pQPopupMenuSection          = new ThemePopup( pQPopupMenu, subName);
  pQPopupMenuSection->insertItem ( subName, this, SLOT(Slot_popup_HierarchOptionSelected()),0, menuPrincipal_ID*1000 + menuOption_ID );
  pQPopupMenuSection->insertSeparator();
  ++menuOption_ID;
  for ( QStringList::Iterator it = optionsList.begin(); it != optionsList.end(); ++it )
      {pQPopupMenuSection->insertItem ( (*it).stripWhiteSpace(), this, SLOT(Slot_popup_HierarchOptionSelected()),0, menuPrincipal_ID*1000 + menuOption_ID );
       ++menuOption_ID;
      }
  pQPopupMenu->insertItem( subName,  pQPopupMenuSection);
  connect( pQPopupMenuSection, SIGNAL( activated( int)),  this, SLOT(Slot_popup_HierarchActivated( int)) );
  m_PopupHeadMap[menuPrincipal_ID] = pQPopupMenuSection;
}

//------------------------------------ Slot_popup_HierarchOptionSelected --------------------------------------------------
void CApp::Slot_popup_HierarchOptionSelected()
{ emit Sign_popup_HierarchOptionSelected();
}

//------------------------------------ Slot_popup_HierarchActivated --------------------------------------------------
void CApp::Slot_popup_HierarchActivated(int id)
{ int menuPrincipal_ID           = id / 1000;
  QString secName                = "";
  QString text                   = "";

  ThemePopup* pQPopupMenuSection = m_PopupHeadMap[menuPrincipal_ID];
  if (pQPopupMenuSection)
     {secName = pQPopupMenuSection->name();
      text    = pQPopupMenuSection->text(id);
      if (pQPopupMenuSection->name()==pQPopupMenuSection->text(id)) *m_pOptionHierarch_Selectionne = pQPopupMenuSection->name();
      else                                                          *m_pOptionHierarch_Selectionne = QString(pQPopupMenuSection->name()) + " (" + pQPopupMenuSection->text(id) + ")";
     }
}

//--------------------------------- DoPopupList -----------------------------------------------------
/*! \brief cree et active un popup menu apartir d'une liste d'items
 *  \param list : QStringList qui contient tous les item si icone associee du theme commence par #iconefile#reste du texte
 *  \return une QString qui contient le nom de l'item selectionne.
*/
QString CApp::DoPopupList(QStringList &list)
{ThemePopup *pThemePopup = new ThemePopup(list,0, "MyPopupMenu");
 if (pThemePopup == 0)     return QString::null;
 QString ret = pThemePopup->DoPopupList();
 delete  pThemePopup;
 return  ret;
}
//------------------------------------------------- CouCou ----------------------------------------------------
void CApp::CouCou(const QString &message, const QColor &color, const QFont &ft , int tempo /* = 1000 */, const QString &imagePath /* ="" */)
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 m_CCoolPopupTempo = tempo;
 QString path      = "";
 if (imagePath.length()==0 || !QDir(imagePath).exists()) path  = Theme::getPath() + "MessagePop.png";
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
 if (tempo==0) m_CCoolPopupTempo    = 3000;   // deux secondes
 else          m_CCoolPopupTempo    = tempo;
 QString path                       = imagePath;
 QString val                        = "";
 QString message                    = message_in;
 path.replace("$Theme",Theme::getPath(Theme::WithNoSeparator));

 if (path.length()==0 || !QFile::exists( path ))
    {CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "image", &path);
     path.replace("$Theme",Theme::getPath(Theme::WithNoSeparator));
     if (!QFile::exists( path )) path  = Theme::getPath() + "/MessagePop.png";
    }

 QPixmap qpm(path);
 m_pCCoolPopup = new CCoolPopup( path , CCoolPopup::WithTransLabel, 0, 0, qpm.width(), qpm.height(), message);
 //......... regarder si pas d'autres param ...............................................
 QFont ft = font();
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "font_family", &val)==0) ft.setFamily(val);
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "font_size",   &val)==0) ft.setPointSizeFloat(val.toDouble());
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "font_weight", &val)==0) ft.setWeight(Min(99,val.toInt()));
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "font_bold",   &val)==0) ft.setBold((bool)val.toInt());
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "font_italic", &val)==0) ft.setItalic((bool)val.toInt());
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "tempo",       &val)==0) if (tempo==0) m_CCoolPopupTempo = val.toInt();
 m_pCCoolPopup->setFont(ft);
 if (CGestIni::Param_ReadParam( m_DrTuxParam, "Popup Message", "font_color", &val)==0)  m_pCCoolPopup->setTextColor(QColor(val));
 connect( m_pCCoolPopup,  SIGNAL( Sign_Clicked(CCoolPopup *) ),   this, SLOT(CouCouStop(CCoolPopup *))  ) ;
 connect( m_pCCoolPopup,  SIGNAL( Sign_PopupUp(CCoolPopup *) ),   this, SLOT(CouCouUp (CCoolPopup *))  ) ;
 connect( m_pCCoolPopup,  SIGNAL( Sign_PopupDown(CCoolPopup *) ), this, SLOT(CouCouStop(CCoolPopup *))  );
}

//--------------------------------------- CouCouUp -------------------------------------------------------------
/*! \brief Appelé lorsque le popup est en haut, attend le tempo puis fait descendre le popup */
void CApp::CouCouUp(CCoolPopup * pCCoolPopup)
{QTimer::singleShot(m_CCoolPopupTempo, pCCoolPopup, SLOT(Disappear()) );
}
//--------------------------------------- CouCouStop -----------------------------------------------------------
/*! \brief Appelé pour détruire le popup */
void CApp::CouCouStop(CCoolPopup * pCCoolPopup)
{if (m_pCCoolPopup==pCCoolPopup)
    {pCCoolPopup->disconnect( SIGNAL(Sign_PopupUp(CCoolPopup *)) );
     pCCoolPopup->disconnect( SIGNAL(Sign_PopupDown(CCoolPopup *)) );
     QTimer::singleShot(0, this, SLOT(CouCouDestroy()) );
    }
}
//--------------------------------------- CouCouDestroy --------------------------------------------------------
/*! \brief Appelé pour détruire le popup */
void CApp::CouCouDestroy()
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 m_pCCoolPopup = 0;
}

//=====================================================================================================================
///////////////////////////////////////////// FONCTIONS COMMUNES AUX RUBRIQUES AVEC ///////////////////////////////////
/////////////////////////////////////////////       EDITEUR TEXTE MULTIMEDIA        ///////////////////////////////////
/////////////////////////////////////////   (a restructurer dans une classe commune ) /////////////////////////////////
//_____________________________________________________________________________________________________________________
//--------------------------------------- decodeMedinTuxData --------------------------------------------------------
/*! \brief decode les donnees medintux et les place dans une  MyEditText pretes à l'affichage*/
int CApp::decodeMedinTuxData(QByteArray  &data, MyEditText* pMyEditText, int /*from*/)
{    char *ptr  = data.data();
     long len   = data.size()-4;
     if (ptr && len>6)
        {if ( strncmp(ptr, "<?xml", 5)==0/* || strstr (ptr,"</ImageListe>") */)
            {if (CGestIni::IsUtf8( ptr ,data.size()))
                {dataToInterface( QString::fromUtf8 ( ptr ),pMyEditText);     //setText(QString::fromUtf8 ( ptr ));
                }
             else
                {dataToInterface( ptr, pMyEditText );
                }
             pMyEditText->setModified (FALSE);
            }
         else if (strncmp(ptr, "<html>", 6))
            {QString stringDST;
             CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len, stringDST,1);
             if (pMyEditText!=0)
                {pMyEditText->setText(stringDST);
                 pMyEditText->setModified (FALSE);
                }
            }
         else
            {if (pMyEditText!=0)
                {pMyEditText->setText( ptr );
                 pMyEditText->setModified (FALSE);
               }
            }
         return 1;
        }
 return 0;
}

//----------------------------- interfaceToData -----------------------------------
/*! \brief Fonction crée la chaine de caractères des données du MyEditText à enregistrer
*  \param len : un pointeur sur int qui si il n'est pas egal à zero sera initialisé sur la longueur des données
 *  \return Renvoie un QString des données
*/
QString CApp::interfaceToData(MyEditText* pMyEditText, int *len /*=0*/)
{QString text = pMyEditText->text();
 CHtmlTools::setTextDefaultAtribut(text, "text",              pMyEditText->m_DefaultTextColor);
 CHtmlTools::setTextDefaultAtribut(text, "bgcolor",           pMyEditText->m_DefaultBackgroundColor);
 return interfaceToData(text, len );
}

//----------------------------- interfaceToData -----------------------------------
/*! \brief Fonction crée la chaine de caractères des données du MyEditText à enregistrer
*  \param len : un pointeur sur int qui si il n'est pas egal à zero sera initialisé sur la longueur des données
 *  \return Renvoie un QString des données
*/
QString CApp::interfaceToData(const QString &text, int *len /*=0*/)
{
 QString     imgName;
 QString     encodedData, html_Data;
 QStringList imgListAlreadyDump;
 QPixmap     retPixMap;
 QString     imageFile;
 if (text.find("<img " , 0, FALSE)==-1) return text;          // si pas d'image pas besoin de placer une liste avec le barnum de tag allant avec on reste en html
 //................. cette version ne place que les images de la liste ....................
 //                  auquelles fait référence le texte
 long       from   = 0;
 long    deb_tag   = 0;
 int        nb_img = 0;     // comptera le nombre d'images non présentes dans le repertoire /texte et Glossaire
 while (from>=0 && from < (long)text.length() )
    {deb_tag     = CHtmlTools::IsThisTagImage(text, from , &imgName );   // rechercher le prochain tag image du texte
     if (deb_tag>=0                                   &&                 // SI trouvé un tag image dans le texte
         ! isThisImageInCache(imgName)                &&                 // ne doit pas etre dans le cache disque local (car alors pas besoin de le maintenir dans une liste en base)
         imgListAlreadyDump.findIndex(imgName)==-1    &&                 // ne doit pas déjà avoir été dumpé (peut y avoir plusieurs références à la même image dans le meme document)
         m_C_ImageList.getPixmap( imgName , retPixMap, &imageFile)       // doit exister dans la liste des images (car incorporé dedans lors insertion image)
        )
        { imgListAlreadyDump.append(imgName);                       // placer dans la liste des images déjà dumpées
          encodedData.append("      <name>\r\n          ");
          encodedData.append(imgName+"\r\n");
          encodedData.append("      </name>\r\n");
          encodedData.append("      <fileName>\r\n          ");
          encodedData.append(imageFile+"\r\n");
          encodedData.append("      </fileName>\r\n");
          encodedData.append("      <ext_Type>\r\n          ");
          encodedData.append("PNG\r\n");
          encodedData.append("      </ext_Type>\r\n");
          encodedData.append("      <base64>\r\n");
          encodedData.append(encodePixmapToBase64(retPixMap)+"\r\n");
          encodedData.append("      </base64>\r\n");
          ++ nb_img;
        }
     //<img height=400 src=Image-0001h400w500 width=500 >
    } //end while (from>=0 && from<txt.length())     essai2.html
 if (nb_img==0)                        return text;        // si pas d'image pas besoin de placer une liste avec le barnum de tag allant avec on reste en html
 encodedData.append("    </ImageListe>\r\n");
 encodedData.append("</MedinTux_Multimedia_data>\r\n");
 //.................. le construire à la fin car les noms ..................
 //                   des images sont modifiés
 html_Data.append("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\r\n<MedinTux_Multimedia_data>\r\n");
 html_Data.append("    <HTML_Data>\r\n");
 html_Data.append(text);
 html_Data.append("    </HTML_Data>\r\n");
 html_Data.append("    <ImageListe>\r\n");
 encodedData.prepend(html_Data);
 if (len) *len = encodedData.length();
 return encodedData;
}
//----------------------------- isThisImageInCache -----------------------------------
/*! \brief verifie si un image est presente dans les caches /Glossaire/ImagesFolder et m_PathAppli/Ressources/RubriquesTextes*/
bool  CApp::isThisImageInCache(const QString imgName)
{if (QFile::exists ( m_PathAppli + "Ressources/RubriquesTextes/" + imgName )) return TRUE;
 if (QFile::exists ( m_PathGlossaire + "ImagesFolder/" + imgName ))           return TRUE;
 return FALSE;
}

//----------------------------- encodePixmapToBase64 -----------------------------------// si pas d'image pas besoin de placer une liste avec le barnum de tag allant avec on reste en html
/*! \brief Encode en base64 la QPixmap passée en paramètre */
QString  CApp::encodePixmapToBase64(const QPixmap& pixIn)
{ QByteArray ba_enc;
  QBuffer buffer( ba_enc );
  buffer.open( IO_WriteOnly );
  pixIn.save( &buffer, "PNG" ); // writes pixmap into ba in PNG format
  return QString ( QCodecs::base64Encode( ba_enc , TRUE) );
}
//----------------------------- dataToInterface -----------------------------------
/*! \brief Fonction qui place les images dans la liste d'images et initialise un QString avec les donnees html
*/
void CApp::dataToInterface(const QString &data, QString &dstString)
{//clearDocument();
 int pos_deb = 0;
 int pos_end = 0;
 dstString   = "";
 if (dataToInterface(data, pos_deb, pos_end))
    {dstString=data.mid(pos_deb, pos_end-pos_deb);
    }
}

//----------------------------- dataToInterface -----------------------------------
/*! \brief Fonction qui place les images dans la liste d'images et initialise un QTextEdit avec les donnees html
*/
void CApp::dataToInterface(const QString &data, MyEditText *pMyEditText)
{//clearDocument();
 int pos_deb = 0;
 int pos_end = 0;
 pMyEditText->setText("");
 if (dataToInterface(data, pos_deb, pos_end))
    {pMyEditText->setText(data.mid(pos_deb, pos_end-pos_deb));
    }
/*
 pMyEditText->setText("");
 m_C_ImageList.resetRef();
 QString imgName, imageFile;
 QPixmap   pixmap;
 int pos_deb = data.find("<HTML_Data>",0);
 if (pos_deb==-1)         return;
 pos_deb += 11;
 int pos_end = data.find("</HTML_Data>", pos_deb);
 if (pos_end==-1)         return;

 int pos_deb_img = data.find("<ImageListe>", pos_deb+12);
 if (pos_deb_img==-1)     return;
 int pos_end_img;
 while ( (pos_deb_img  = data.find("<name>",  pos_deb_img)) != -1 )
       {  imageFile = "";
          pos_deb_img += 6;
          if ( (pos_end_img  = data.find("</name>", pos_deb_img)) != -1)
             {imgName     = data.mid(pos_deb_img, pos_end_img-pos_deb_img).stripWhiteSpace();
              int pos;
              if ( (pos = data.find("<fileName>", pos_deb_img)) != -1)
                 {pos += 10;
                  if ( (pos_end_img  = data.find("</fileName>", pos)) != -1)
                     {imageFile = data.mid(pos, pos_end_img-pos).stripWhiteSpace();
                     }
                 }
              if ( (pos_deb_img = data.find("<base64>", pos_deb_img)) != -1)
                 {pos_deb_img += 8;
                  if ( (pos_end_img  = data.find("</base64>", pos_deb_img)) != -1)
                     { if (m_C_ImageList.getNbRefForThisName(imgName) ==-1)           // est elle déjà dans la factory ??
                          {pixmap =  decodeStringTo_QPixmap( data.mid(pos_deb_img, pos_end_img-pos_deb_img ));
                           m_C_ImageList.appendToList( pixmap, imgName, imageFile );
                           pos_deb_img = pos_end_img + 9;
                          }
                     }
                  else return;
                 }
               else return;
             }
          else return;
       }
   //
 */
}
//----------------------------- dataToInterface -----------------------------------
/*! \brief Fonction qui place les images dans la liste d'images et initialise les bornes pos_deb et pos_end du texte html
*/
int CApp::dataToInterface(const QString &data, int &pos_deb, int &pos_end)
{m_C_ImageList.resetRef();
 QString imgName, imageFile;
 QPixmap   pixmap;
 pos_deb = data.find("<HTML_Data>",0);
 if (pos_deb==-1)         return 0;
 pos_deb += 11;
 pos_end = data.find("</HTML_Data>", pos_deb);
 if (pos_end==-1)         return 0;

 int pos_deb_img = data.find("<ImageListe>", pos_deb+12);
 if (pos_deb_img==-1)     return 0;
 int pos_end_img;
 while ( (pos_deb_img  = data.find("<name>",  pos_deb_img)) != -1 )
       {  imageFile = "";
          pos_deb_img += 6;
          if ( (pos_end_img  = data.find("</name>", pos_deb_img)) != -1)
             {imgName     = data.mid(pos_deb_img, pos_end_img-pos_deb_img).stripWhiteSpace();
              int pos;
              if ( (pos = data.find("<fileName>", pos_deb_img)) != -1)
                 {pos += 10;
                  if ( (pos_end_img  = data.find("</fileName>", pos)) != -1)
                     {imageFile = data.mid(pos, pos_end_img-pos).stripWhiteSpace();
                     }
                 }
              if ( (pos_deb_img = data.find("<base64>", pos_deb_img)) != -1)
                 {pos_deb_img += 8;
                  if ( (pos_end_img  = data.find("</base64>", pos_deb_img)) != -1)
                     { if (m_C_ImageList.getNbRefForThisName(imgName) ==-1)           // est elle déjà dans la factory ??
                          {pixmap =  decodeStringTo_QPixmap( data.mid(pos_deb_img, pos_end_img-pos_deb_img ));
                           m_C_ImageList.appendToList( pixmap, imgName, imageFile );
                           pos_deb_img = pos_end_img + 9;
                          }
                     }
                  else return 0;
                 }
               else return 0;
             }
          else return 0;
       }
  return 1;
}

//----------------------------- decodeStringTo_QPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image...
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap CApp::decodeStringTo_QPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return QPixmap ( ba_dec );
}

//----------------------------------------- makeData --------------------------------
/*! \brief Fonction à partir du nom du fichier à insérer, et du masque d'insertion, les données à placer en base...
 *  \param imageFile : &QString qui contient le nom du fichier de l'image à insérer
 *  \param  w : int contenant la largeur maximale d'affichage l'image à insérer
 *  \param  h : int contenant la hauteur maximale d'affichage l'image à insérer
 *  \param  fileMask : &QString contenant le chemin du fichier du masque d'insertion (si vide il sera utilisé m_InsertMask de la classe )
 *  \return QString qui contient les données.
*/

QString CApp::makeData(const QString &imageFile, int h, int w, int maxImageWidth /* = 1024 */, const QString &fileMask /* = "" */)
{
 QString html_Part     = "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\r\n"
                         "<MedinTux_Multimedia_data>\r\n"
                         "    <HTML_Data>\r\n";
 QString image_Part    = "    </HTML_Data>\r\n"
                         "    <ImageListe>\r\n";

 makeData ( imageFile,  h,  w, html_Part, maxImageWidth, &image_Part, fileMask  );

 html_Part            += image_Part;
 html_Part            += "    </ImageListe>\r\n"
                         "</MedinTux_Multimedia_data>\r\n";
 return html_Part;
}

//----------------------------------------- makeData --------------------------------
/*! \brief Fonction à partir du nom du fichier à insérer, et du masque d'insertion, les données à placer en base...
 *  \param imgName : &QString qui contient le nom de l'image à insérer
 *  \param  w : int contenant la largeur maximale d'affichage l'image à insérer
 *  \param  h : int contenant la hauteur maximale d'affichage l'image à insérer
 *  \param  html_Part :  une référence sur une QString à laquelle sera ajoutée  la partie html des données
 *  \param  image_Part : un pointeur sur une QString à laquelle (si pointeur non egal à zéro) sera ajoutée  la partie xml des données images (par défaut à zéro)
 *  \param  fileMask : &QString contenant le chemin du fichier du masque d'insertion (si vide il sera utilisé m_InsertMask de la classe ) (par défaut vide)
 *  \return néant.
*/

void CApp::makeData(const QString &imageFile, int h, int w, QString &html_Part, int maxImageWidth /* = 1024 */, QString *image_Part /* = 0 */, const QString &fileMask  /* = "" */ )
{QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
 QPixmap      *pImg  = new QPixmap(imageFile);     // format image deviné car 'const char * format = 0'
 //.................... mise à l'echelle de l'image si trop large pour l'impression .........................................
 if (pImg && pImg->width()> maxImageWidth)
    { int   val_w    = pImg->width();
      int   val_h    = (int) ( ((float)maxImageWidth / (float)val_w) * (float)pImg->height());
      QImage   qi    = pImg->convertToImage().smoothScale (maxImageWidth, val_h );
      *pImg          = QPixmap (qi );
    }

 QString    ext        = ".png";
 QString    imgName    = CMoteurBase::GUID_Create()+"-h"+ QString::number(h)+"w"+QString::number(w) + ext;                  // nom et clef pour retrouver l'image

 if (pImg &&  m_C_ImageList.appendToList( pImg , imgName , imageFile) == 1 ) // si reference est la première creer l'icone de l'image
    {
     html_Part        += MixMaskWhithImageName( imgName ,  pImg->width(),  pImg->height() , fileMask);
     QStringList imgListAlreadyDump;
     QPixmap     retPixMap;
     //................. cette version ne place que les images de la liste ....................
     //                  auquelles fait référence le texte
     long       from   = 0;
     long    deb_tag   = 0;
     while (from>=0 && from<(long)html_Part.length())
           {deb_tag    = CHtmlTools::IsThisTagImage(html_Part, from , &imgName );   // rechercher le prochain tag image du texte
            if (deb_tag>=0                                   &&                     // doit etre trouvé un tag image dans le texte
                imgListAlreadyDump.findIndex(imgName)==-1    &&                     // ne doit pas déjà avoir été dumpé (peut  y avoir plusieurs références à la même image)
                m_C_ImageList.getPixmap( imgName , retPixMap)              // doit exister dans la liste des images
               )
               { imgListAlreadyDump.append(imgName);                       // placer dans la liste des images déjà dumpées
                 if (image_Part)
                    {image_Part->append("      <name>\r\n          ");
                     image_Part->append(imgName+"\r\n");
                     image_Part->append("      </name>\r\n");
                     image_Part->append("      <fileName>\r\n          ");
                     image_Part->append(imageFile+"\r\n");
                     image_Part->append("      </fileName>\r\n");
                     image_Part->append("      <ext_Type>\r\n          ");
                     image_Part->append("PNG\r\n");
                     image_Part->append("      </ext_Type>\r\n");
                     image_Part->append("      <base64>\r\n");
                     image_Part->append(encodePixmapToBase64(retPixMap)+"\r\n");
                     image_Part->append("      </base64>\r\n");
                    }
               }
               //<img height=400 src=Image-0001h400w500 width=500 >
           } //end while (from>=0 && from<txt.length())     essai2.html
    }
 if (pImg) delete pImg;
 QApplication::restoreOverrideCursor();
}

//----------------------------- MixMaskWhithImage -----------------------------------
/*! \brief Fonction à partir du nom du fichier à insérer, et du masque d'insertion, crée la chaine HTML à insérer...
 *  \param imgName :  contient le nom de l'image à insérer
 *  \param w :  contient la largeur souhaitée pour afficher l'image
 *  \param h :  contient la hauteur souhaitée pour afficher l'image
 *  \param fileMask :  le fichier .html (avec les tags d'insertion) a utiliser pour insérer l'image. Peur etre non indique et alors sera utilise un tag image simplifie
 *  \return Renvoie un pointeur vers le fichier html a inserer dans la page .
*/
//   Image  {{IMAGE_NAME}}
//   {{IMAGE_WIDTH=300}} {{IMAGE_HEIGHT=100}}
//
//
QString CApp::MixMaskWhithImageName( QString imgName , int w, int h, const QString &fileMask /* = "" */)
{QString insertMask = "";
 QString toScale    = "";
 int pos            = imgName.findRev('h');
 int    pos_egal    = -1;
 QString     tag    = "";
 int         val_w  = -1;
 int         val_h  = -1;
 int     len_tag    =  0;

 CGestIni::Param_UpdateFromDisk(fileMask,     insertMask);
 if (insertMask.length()==0) // si pas de masque valide on insère une référence simple par défaut.
    { if (pos != -1)
         {toScale = "";
          toScale += "\" width=\""  + QString::number(w);
          toScale += "\" height=\"" + QString::number(h);
          toScale += "\"";
         }
      return (QString("<br><img src=\"") + imgName + toScale  + "><br>");
    }

 int deb_tag=0;
 int end_tag=0;
 while ( (deb_tag = insertMask.find("{{",deb_tag)) != -1)
  {end_tag = insertMask.find("}}", deb_tag + 2);
   if (end_tag==-1)          return tr("Erreur de syntaxe '}}' fermant non trouvé près de : ") + insertMask.mid(deb_tag + 2, 10);
   end_tag += 2;
   len_tag = end_tag - deb_tag;

   tag = insertMask.mid(deb_tag, len_tag);
   if ( tag.find("IMAGE_PICT") != -1 )
      {insertMask.replace( deb_tag, len_tag , "#~~°=|=°~~#" );  // ne surtout pas virer "#~~°=|=°~~#"
       end_tag = deb_tag + 12;
      }
   else if ( tag.find("IMAGE_NAME") != -1 )
      {insertMask.replace( deb_tag, len_tag , imgName );
       end_tag = deb_tag + imgName.length();
      }
   else if ( tag.find("IMAGE_MAX_WIDTH") != -1 )
      {pos_egal = tag.find ('=');
       if (pos_egal == -1)   return tr("Erreur de syntaxe signe '=' non trouvé près de : ") + insertMask.mid(deb_tag + 2, 10);
       ++pos_egal;
       val_w = tag.mid(pos_egal,tag.length()-2-pos_egal).stripWhiteSpace().toInt();
       val_w = Min(w, val_w);
       insertMask.remove( deb_tag, len_tag );
       end_tag = deb_tag;
      }
   else if ( tag.find("IMAGE_MAX_HEIGHT") != -1 )
      {pos_egal = tag.find ('=');
       if (pos_egal == -1)   return tr("Erreur de syntaxe signe '=' non trouvé près de : ") + insertMask.mid(deb_tag + 2, 10);
       ++pos_egal;
       val_h = tag.mid(pos_egal,tag.length()-2-pos_egal).stripWhiteSpace().toInt();
       val_h = Min(h, val_h);
       insertMask.remove( deb_tag, len_tag );
       end_tag = deb_tag;
      }
   else if ( tag.find("IMAGE_WIDTH") != -1 )
      {pos_egal = tag.find ('=');
       if (pos_egal == -1)   return tr("Erreur de syntaxe signe '=' non trouvé près de : ") + insertMask.mid(deb_tag + 2, 10);
       ++pos_egal;
       val_w = tag.mid(pos_egal,tag.length()-2-pos_egal).stripWhiteSpace().toInt();
       insertMask.remove( deb_tag, len_tag );
       end_tag = deb_tag;
      }
   else if ( tag.find("IMAGE_HEIGHT") != -1 )
      {pos_egal = tag.find ('=');
       if (pos_egal == -1)   return tr("Erreur de syntaxe signe '=' non trouvé près de : ") + insertMask.mid(deb_tag + 2, 10);
       ++pos_egal;
       val_h = tag.mid(pos_egal,tag.length()-2-pos_egal).stripWhiteSpace().toInt();
       insertMask.remove( deb_tag, len_tag );
       end_tag = deb_tag;
      }
   else
      {deb_tag = end_tag;
      }
  }
  //...................... Mise à l'echelle .........................................
 if (val_w == -1 && val_h == -1 )
    {val_w = w;
     val_h = h;
    }
 else if(val_w == -1 && val_h != -1 )
    {val_w = (int) ( ((float)val_h / (float)h) * (float)w);
    }
 else if(val_h == -1 && val_w != -1 )
    {val_h = (int) ( ((float)val_w / (float)w) * (float)h);
    }
 toScale += "";
 toScale += "\" width=\""  + QString::number(val_w);
 toScale += "\" height=\"" + QString::number(val_h);
 toScale += "\"";
 insertMask.replace("#~~°=|=°~~#",QString("<img src=\"") + imgName + toScale  + ">");
 return insertMask;
}

//---------------------------------- AddNewDocument -----------------------------------------------------------------------------
/*! \brief replace le document en cours en replaçant le contenu dans la liste des rubriques, et ajoute un nouveau document à la liste */
CRubRecord*   CApp::AddNewDocument(CMDI_Generic* pCMDI, const QString &strData_in, int typ, const char *path /*=0*/ , const QDateTime *pQDateTime/*=0*/, const QString * libelle, const QString * /*subType =0*/, int noInquire /*=0*/)
{if (pCMDI->IsModifiable()==FALSE)                return 0;
 if ( strData_in.length()==0 )                    return 0;
 QString                      date;
 int                           ret       = 0;
 QDateTime                     qdt       =  QDateTime::currentDateTime();
 if (pQDateTime)               qdt       = *pQDateTime;
 MyEditText           *pMyEditText       =  pCMDI->GetCurrentEditor();
 if (pMyEditText == 0)                            return 0;

 pCMDI->IfModified_SaveInRubList();
 //......... recuperer l'eventuel type de document si specifie inside le document ........................
 QString rubName         = pCMDI->GetRubName();
 int typeRub             = G_pCApp->detectTypeRubFromDoc(strData_in, path, &noInquire);   // affecter l'eventuel type specifie par le document
 if (typeRub==0) typeRub = pCMDI->GetType();                                              // sinon type de la rubrique
 else            rubName = TypeToRubName( typeRub );
 if (G_pCApp->IsThisDroitExist(m_Droits, pCMDI->m_PrefixDroit+"c"))
    {if (pCMDI->m_PrefixDroit != "ob") noInquire = 1;
     //................... demander confirmation pour creation d'une autre à la même date ...........................
     if (noInquire==0 && m_pCMoteurBase->GetAllRubriqueFromDate(pCMDI->m_pRubList, QDate::currentDate(), QString::number(pCMDI->GetType()) )>0)
        {ret = QMessageBox::warning ( pCMDI,          TR ("Création d'un nouveau document ") ,
                                                      TR ("Un document a déjà été créé et existe déjà pour aujourd'hui\r\n"
                                                          "Faut-il vraiment en créer un deuxième ?\r\n"
                                                          "ou ajouter les données à l'emplacement du curseur ?"),
                                                      TR ("&Créer"), TR("A&jouter"), TR("Annu&ler"),  1, 1 );
               if (ret == 2)                     return 0;
        }
     //............. placer les données de la rubrique en cours d'edition, si necessaire
     //              dans la liste des rubriques, donc à sauver (que si necessaire CAD modifiées)
     if (ret==1)
        {m_pDrTux->OnGlossaireFileToEdit(path, TR("Ajouter"), typ);
         return 0;
        }

  QString   filename     = path;
  QString   html_Data    = "";
  QString   image_Data   = "";
  //.................................. integrer le fichier image si besoin dans le document ..................................
  if    (strData_in.find("{{IMAGE_PICT}}") != -1)
        {image_Data   = "    <ImageListe>\r\n";
         QString outParam, lastFile;
         CGestIni::Param_UpdateFromDisk(m_PathDrTuxIni, outParam );
         CGestIni::Param_ReadParam( outParam, pCMDI->GetRubName(),   "Path documents", &lastFile);
         filename = GetImageFileName(&lastFile);
         CGestIni::Param_WriteParam( &outParam, pCMDI->GetRubName(), "Path documents", lastFile);
         CGestIni::Param_UpdateToDisk(m_PathDrTuxIni, outParam);
         //................. lire la valeur max des images dans le .ini....................................
         int maxImageWidth = 600;
         QString val;
         if (READ_LOCAL_PARAM(LOCAL_PARAM, pCMDI->GetRubName(), "MaxImageWidth", &val )==0) maxImageWidth =  val.toInt(); // splash utilisé provisoirement

         //............... crée la chaine html avec la référence à l'image, ajoute l'image à la liste d'images ...........................................
         if (filename.length() && QFile::exists( filename ))  makeData(filename, 400, 500, html_Data, maxImageWidth, &image_Data, path);   // ajoute les images à la liste et crée la structure html
         else                                                 return 0;
         image_Data           += "    </ImageListe>\r\n";
       }
  else
        { html_Data = strData_in;
        }
  //.................. Creer un nouveau document vide (les données seront replacées aprés fusion) dans la liste des rubriques .......................................
  QString     libelleToSet =  QFileInfo(filename).baseName(); if (libelle && libelle->length()) libelleToSet = *libelle;
  QDateTime    qdt         =  QDateTime::currentDateTime();
  RUBREC_LIST::iterator it =  pCMDI->m_pRubList->append( CRubRecord(m_pCMoteurBase->GetPkDoc_Provisoire(pCMDI->m_pRubList),        // PrimKey d'un truc juste dans la liste et non encore en base (faudre sauver)
                                                                    libelleToSet,                            // libelle
                                                                    QString::number(typeRub),                // type
                                                                    "Default SubType",                       // sous type
                                                                    qdt.toString ("yyyy-MM-ddThh:mm:ss"),    // date l'enregistrement
                                                                    m_User,                                  // utilisateur
                                                                    m_SignUser                               // utilisateur signataire
                                                                    // strData,                              // données texte
                                                                    // strData.length()                      // longueur des données
                                                            )
                                                 );
 //.......................... faire les fusions si nécessaire .............................................
 (*it).m_LastErr = "";
 DOCUMENT_DISPLAY_MAP mapDoc;
 m_pDrTux->MapActiveID_Doc(mapDoc);
 m_VAR_MAP.insert("$SCRIPT_STATUS", rubName + "_$FUSION_CREATE");
 m_pDrTux->FusionneDocument(html_Data, m_User, it, mapDoc); // data = data.replace('0xa0','0x20');
 m_VAR_MAP.insert("$SCRIPT_STATUS", rubName + "_$FUSION_END");

 if ((*it).m_LastErr[0]=='_'||html_Data.find("__ERREUR Fusion :__") != -1)
    {pCMDI->m_pRubList->remove ( it );
     return 0;
    }
 else
    { //.................. activer l'interface .......................................
      m_C_ImageList.resetRef();
      pMyEditText->show();                                      // des fois que pas visible
      //.......................... on replace les données dans la liste apres fusion ..................................
      if (image_Data.length())
         {html_Data.prepend( "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\r\n"
                             "<MedinTux_Multimedia_data>\r\n"
                             "    <HTML_Data>\r\n"  );
          html_Data.append ( "    </HTML_Data>\r\n" );
          html_Data.append ( image_Data );
          html_Data.append ( "</MedinTux_Multimedia_data>\r\n" );
         }
      m_pCMoteurBase->ReplaceDataInRubList(html_Data, it);


      return &(*it);
     }
   }
 return 0;
}

//---------------------------------- DoActionWithThisDocument -----------------------------------------------------------------------------
/*! \brief fait avec le document donné en entrée une action dépendante du verbe  */
void   CApp::DoActionWithThisDocument(CMDI_Generic* pCMDI, const QString &verbe, int /*typ*/, const char *path /* =0 */, const QDateTime * /*pQDateTime =0 */, const QString * /*libelle =0 */, const QString * /*subType =0 */)
{if (pCMDI->IsModifiable()==FALSE)                                return;
 if (! IsThisDroitExist(m_Droits,  pCMDI->m_PrefixDroit+"c"))     return;
 MyEditText  *pMyEditText       =  pCMDI->GetCurrentEditor();
 if (pMyEditText == 0)                                            return;
 QString strData = "";
 CGestIni::Param_UpdateFromDisk(path, strData );
 if (strData.length()==0)                                         return;
 if (verbe==TR("Tout") || verbe==TR("Ajouter"))
    {    CHtmlTools::ifNeed_Convert_RtftoHtml( strData );
         RUBREC_LIST::iterator it = pCMDI->Current_RubList_Iterator();
         //............... SI PAS DE  document affiché ...................................................
         if (it == pCMDI->m_pRubList->end())       // pas de document faut en créer un ?
            {if (QMessageBox::warning ( pCMDI, TR("MedinTux vous solicite"),
                                               TR ( "Il n'existe pas de document valide pour cette action.<br>"
                                                    "Faut-il en créer un avec celui :<br><b>«")
                                                    +QFileInfo(path).fileName()+tr(" »</b><br>sélectionné dans le Glossaire "),
                                               TR ("Annu&ler"), TR ("&Créer"), 0,
                                           1, 1 )==1
                )
                {AddNewDocument(pCMDI, strData, 0, path);
                }
             return;
            }

     if (strData.find("{{IMAGE_PICT}}") != -1)
        {QString outParam, lastFile;
         CGestIni::Param_UpdateFromDisk(m_PathDrTuxIni, outParam );
         CGestIni::Param_ReadParam( outParam, pCMDI->GetRubName(),   "Path documents", &lastFile);
         QString   filename = GetImageFileName(&lastFile);
         CGestIni::Param_WriteParam( &outParam, pCMDI->GetRubName(), "Path documents", lastFile);
         CGestIni::Param_UpdateToDisk(m_PathDrTuxIni, outParam);
         //................. lire la valeur max des images dans le .ini....................................
         int maxImageWidth = 600;
         QString val;
         if (READ_LOCAL_PARAM(LOCAL_PARAM, pCMDI->GetRubName(), "MaxImageWidth", &val )==0) maxImageWidth =  val.toInt(); // splash utilisé provisoirement

         //............... crée la chaine html avec la référence à l'image, ajoute l'image à la liste d'images ...........................................
         strData = "";      // car makeData ajoute à ce qui est déjà existant
         if (filename.length() && QFile::exists( filename ))  makeData(filename, 400, 500, strData, maxImageWidth, 0, path);   // ajoute les images à la liste et crée la structure html
         else                                                 return;                                                                     // annuler ou fichier invalide
        }
     //.......................... faire les fusions si nécessaire .............................................
     DOCUMENT_DISPLAY_MAP mapDoc;
     m_pDrTux->MapActiveID_Doc(mapDoc);
     m_VAR_MAP.insert("$SCRIPT_STATUS",pCMDI->GetRubName()+"_$FUSION_ADD_"+verbe);
     m_pDrTux->FusionneDocument(strData, m_User, it,  mapDoc); // data = data.replace('0xa0','0x20');
     m_VAR_MAP.insert("$SCRIPT_STATUS",pCMDI->GetRubName()+"_$FUSION_END");

     if (verbe == TR("Tout")) pMyEditText->selectAll();
     CHtmlTools::insertHtml(pMyEditText, strData);
    }//end if (verbe==tr("Tout") || verbe==tr("Ajouter"))
}

//---------------------------------- On_pushButtonAppend -----------------------------------------------------------------------------
/*! \brief fonction qui répond à l'appui du bouton de chargement d'une image */
int  CApp::On_pushButtonAppend(CMDI_Generic* pCMDI)
{  MyEditText  *pMyEditText  =  pCMDI->GetCurrentEditor();
   if (pMyEditText == 0)                                                                           return 0;
   if (pCMDI->IsModifiable()==FALSE)  {G_pCApp->CouCou(tr("Cette rubrique n'est pas "
                                                          "modifiable (droits insuffisants)"));    return 0;}
   m_LastMainOption         = "";
   m_IdMenu                 = 0;
   m_SelectedFile           = "";
   int                 ret  = 0;
   int           nbItemNew  = 0;
   int           nbItemAdd  = 0;
   int           nbItemRep  = 0;
   QString            html  = "";
   RUBREC_LIST::iterator it = pCMDI->Current_RubList_Iterator();

   //............. sous popup secondaire des repertoires .................
   ThemePopup *pDirPopupNew =  new ThemePopup(pCMDI, tr("popupPourAddRubrique"));  if (pDirPopupNew==0)       return 0;
   connect( pDirPopupNew, SIGNAL( highlighted ( int)),  this, SLOT(Slot_OnMenuActivated(int)));
   ThemePopup *pDirPopupAdd =  new ThemePopup(pCMDI, tr("popupPourAddRubrique"));  if (pDirPopupAdd==0)       return 0;
   connect( pDirPopupAdd, SIGNAL( highlighted ( int)),  this, SLOT(Slot_OnMenuActivated(int)));
   ThemePopup *pDirPopupRep =  new ThemePopup(pCMDI, tr("popupPourAddRubrique"));  if (pDirPopupRep==0)       return 0;
   connect( pDirPopupRep, SIGNAL( highlighted ( int)),  this, SLOT(Slot_OnMenuActivated(int)));

   directoryToPopup(pDirPopupNew, m_PathGlossaire + pCMDI->GetRubName(), 0, 2000, &nbItemNew);
   directoryToPopup(pDirPopupAdd, m_PathGlossaire + pCMDI->GetRubName(), 0, 3000, &nbItemAdd);
   directoryToPopup(pDirPopupRep, m_PathGlossaire + pCMDI->GetRubName(), 0, 4000, &nbItemRep);

   //............. popup principal .......................................
   m_Popup = new ThemePopup(pCMDI, tr("popupPourRubrique"));         if (m_Popup==0)                return 0;
   m_Popup->insertItem(     Theme::getIcon("NewEmpty.png"),                          tr("1 - Créer une nouvelle page vierge"),                      m_Popup, SLOT( OnSelectPopupItem()),  0 , 1);
   if (nbItemNew|nbItemAdd|nbItemRep) m_Popup->insertSeparator ();
   if (nbItemNew) m_Popup->insertItem(     Theme::getIcon("NewDoc.png"),             tr("2 - Créer une nouvelle page avec ..."),                    pDirPopupNew);
   if (nbItemAdd) m_Popup->insertItem(     Theme::getIcon("NewDocPlusCurs.png"),     tr("3 - Ajouter à l'emplacement du curseur..."),               pDirPopupAdd);
   if (nbItemRep) m_Popup->insertItem(     Theme::getIcon("NewDocPlus.png"),         tr("4 - Tout remplacer par... "),                              pDirPopupRep);
   //m_Popup->insertSeparator ();
   //m_Popup->insertItem(     Theme::getIcon("Trombone.png"),           tr("4 - Créer un nouveau document avec un fichier quelconque... "),  m_Popup, SLOT( OnSelectPopupItem()),  0 , 4);
   m_Popup->insertSeparator ();
   m_Popup->insertItem(     Theme::getIcon("ic-flower3.png"),         tr("5 - Créer un nouveau document avec une image... "),              m_Popup, SLOT( OnSelectPopupItem()),  0 , 5);
   if (it != pCMDI->m_pRubList->end())
      {m_Popup->insertItem( Theme::getIcon("ic-flower3PlusCurs.png"), tr("6 - Ajouter à l'emplacement du curseur une image... "),          m_Popup, SLOT( OnSelectPopupItem()),  0 , 6);
      }
   connect( m_Popup, SIGNAL( highlighted ( int)),  this, SLOT(Slot_On_MainMenuActivated(int)));
   ret = m_Popup->exec( QCursor::pos());
   delete pDirPopupNew; delete pDirPopupAdd;  delete pDirPopupRep;  delete m_Popup;
   if (ret==-1)                                                                                    return 0;

   //qDebug(tr("fichier selectionne : %1  derniere option main : %3").arg(m_SelectedFile,  m_LastMainOption));
   int optionNumber = m_LastMainOption.find('-'); if (optionNumber != -1) optionNumber = m_LastMainOption.left(optionNumber).toInt();
   switch(optionNumber)
     {case 1:{
               QString path = m_PathAppli+"Ressources/Documents par defaut/PageVide.html";
               if ( !QFile::exists ( path )) CGestIni::Param_UpdateToDisk(path, "<html><head><meta name=\"qrichtext\" content=\"charset=utf-8\" /></head>"
                                                                                "<body style=\"font-size:10pt;font-family:Sans Serif\"><p>"
                                                                                "</body></html>");
               m_pDrTux->OnGlossaireFileToEdit(path, tr("Créer"), pCMDI->GetType());               return 0;
             }
      case 2: m_pDrTux->OnGlossaireFileToEdit(m_SelectedFile, tr("Créer"),     pCMDI->GetType());  return 0;
      case 3: m_pDrTux->OnGlossaireFileToEdit(m_SelectedFile, tr("Ajouter"),   pCMDI->GetType());  return 0;
      case 4: DoActionWithThisDocument(pCMDI, tr("Tout"), pCMDI->GetType(), m_SelectedFile);       return 0;
      case 5:
      case 6:
              QString outParam, lastFile, fileName;
              CGestIni::Param_UpdateFromDisk(m_PathDrTuxIni, outParam );
              CGestIni::Param_ReadParam( outParam, pCMDI->GetRubName(),   "Path documents", &lastFile);
              fileName = GetImageFileName(&lastFile);
              CGestIni::Param_WriteParam( &outParam, pCMDI->GetRubName(), "Path documents", lastFile);
              CGestIni::Param_UpdateToDisk(m_PathDrTuxIni, outParam);
              if (fileName.length()==0)                                                            return 0;
              //................. lire la valeur max des images dans le .ini....................................
              int maxImageWidth = 600;
              QString val;
              if (READ_LOCAL_PARAM(LOCAL_PARAM, pCMDI->GetRubName(), "MaxImageWidth", &val )==0) maxImageWidth =  val.toInt();
              //............ creer ................
              if (optionNumber == 5)
                 {
                  QString                       path = m_PathAppli + "Ressources/Documents par defaut/ImageMaskSTD.html";
                  if ( !QFile::exists ( path )) path = m_PathAppli + "Ressources/RubriquesTextes/ImageMaskSTD.html";
                  //if ( !QFile::exists ( path ))                                                     return 0;
                  html = makeData(fileName, 400, 500, maxImageWidth , path);
                  if (html.length()==0)                                                             return 0;

                  CRubRecord* pCRubRecord = AddNewDocument(pCMDI, html, pCMDI->GetType(), fileName , 0, 0, 0, 1);
                  pCMDI->initData();
                  if (pCRubRecord)                                                                  return 1;
                  else                                                                              return 0;
                 }
              //............ inserer une image a l'emplacement curseur.............
              //   Creer le tag html d'insertion en meme temps que rajouter l'image à la liste globale des images
              makeData(fileName, 400, 500, html,  maxImageWidth ,0 , "" );
              if (html.length()==0)                                                                 return 0;
              //   Inserer le tag d'insertion dans le texte en cours
              CHtmlTools::insertHtml(pMyEditText, "*=()#%#()=*", 0);      // on insere un motif au curseur
              QString html_Data = pMyEditText->text();                    // on recupere le texte
              html_Data = html_Data.replace("*=()#%#()=*",html);                                 // on le remplace par le tag d'insertion de l'image
              //   On cree la structure de donnee devant etre placee en base (pas la meme que celle du QTextEdit)
              pMyEditText->setText(html_Data);
              html_Data = interfaceToData(html_Data);
              //   on la remet en base
              m_pCMoteurBase->ReplaceDataInRubList(html_Data, it);
              //   on recharge le tout dans le texte
              //m_pFormRubIdent->pushButtonSave->show();
              pCMDI->initData();
              pMyEditText->setModified (FALSE);                                                     return 1;
              //break;
     }
 return 0;
}

//------------------------------------------- directoryToPopup ----------------------------------------
/*! \brief Créer l'arborescence d'un repertoire et l'ajoute à un menu.
 *  \param ThemePopup :   menu qui reçoit l'arborescence
 *  \param path :         Chemin à explorer
 *  \param numSousMenus : indice du sous menu dans la hierarchie (colonne).
 *  \param idMainMenu :   identifiant du menu principal.
 *  \param nb :           si indique ( !=0) comptera le nb item.
 * \return int :
*/

int CApp::directoryToPopup(ThemePopup* pPopup, QString path, int numSousMenus, int idMainMenu, int *nb /* =0 */ )
{ if (!pPopup)                         return 0;
  QDir dir (path);  if (!dir.exists()) return 0;

  dir.setFilter( QDir::All | QDir::NoSymLinks );
  dir.setSorting(QDir::DirsFirst | QDir::Name);

  const QFileInfoList * pQFileInfoList = dir.entryInfoList();
  if (pQFileInfoList==0)               return 0;
  //................ charger la BlackList ........................................
  QString black_str      = CGestIni::Param_UpdateFromDisk(m_PathGlossaire+"/ignoreList.txt");
  black_str              = black_str.replace('\r',"").stripWhiteSpace();
  QStringList black_list = QStringList::split('\n',black_str);


  QFileInfoListIterator it( *pQFileInfoList );
  QFileInfo     *fi;
  QString fname="";
  // Parcours le répertoire
  while ( (fi = it.current()) != 0 )
  {  fname = fi->fileName();
     if (fname == ".." || fname == ".")              {++it;   continue;}
     if (CGestIni::isInBlackList(black_list, fname)) {++it;   continue;}

     if (fi->isDir())            // c'est un autre répertoire
       {        // Prépare le WhatsThis qui contient le path complet du fichier
        QString wT = path +QDir::separator()+ fname;
        // Vérifie qu'on peut créer une nouvelle hiérarchie de menu
        int limite = numSousMenus + 1;
        if (limite < NBMAXSOUSMENU)
           { // OK On peut créer une nouvelle branche
             ThemePopup* sousMenuPop = new ThemePopup(pPopup);
             directoryToPopup(sousMenuPop, wT, limite, idMainMenu,nb);
             connect( sousMenuPop, SIGNAL( activated(int)),      this, SLOT(Slot_OnMenuActivated(int)));
             connect( sousMenuPop, SIGNAL( highlighted ( int)),  this, SLOT(Slot_OnMenuActivated(int)));
             // Insérer le popup dans popup de la fonction
             pPopup->insertItem( Theme::getIconListFolder(), fname, sousMenuPop);
           }
        }
     else              // c'est un autre répertoire
        { QString ext = fi->extension();
          if (ext.lower() == "txt" || ext.lower() == "html" || ext.lower() == "htm" || ext.lower() == "rtf" || ext == "")
             { int id = pPopup->insertItem( Theme::getIconListHTML(), fi->baseName(),  this, SLOT( Slot_FileSelected() ) );
               pPopup->setWhatsThis(id  , /*QString::number(idMainMenu) +"::"+*/ path + QDir::separator()+fi->fileName());
               if (nb) *nb = *nb+1;
             }
        }  // end else if (fi->isFile())
    ++it;
    } // fin while
  return 1;
}
//------------------------------------------- detectTypeRubFromDoc ----------------------------------------
/*! \brief verifie si dans le document il n'y a pas l'instruction {{DOC_TYPE=Observation,1}}"
 *  \param text :      const QString & texte du document si vide alors le document sera charge a partir de pathDoc
 *  \param pathDoc :   const QString & chemin du document
 *  \param noInquire : int * qui si different de zero, sera mis a jour avec le parametre noInquire
 * \return int :   le type du document
 */
int CApp::detectTypeRubFromDoc(const QString &text_in, const QString &pathDoc, int *noInquire /*=0*/)
{QString sType        = "";
 QString text         = "";
 int type             =  0;
 int pos              = -1;
 int end              = -1;
 int vrg              = -1;
 const QString *pText = 0;
 if (text_in.length())  pText = &text_in;
 else                  {CGestIni::Param_UpdateFromDisk(pathDoc, text);
                        pText = (const QString* )&text;
                       }
 if ( (pos = pText->find("{{DOC_TYPE")) != -1)
    {if ( (pos = pText->find("=",pos+10)) != -1)
        {++pos;
         if ( (end       = pText->find("}}",pos)) != -1)
            {  text      = pText->mid(pos, end-pos);  end  = text.length();
               vrg       = text.find(",");
               if (vrg != -1)  {sType      = text.mid(0,vrg).stripWhiteSpace();
                                if (sType[0] != '$') type  = RubNameToType( sType );
                                else                 type  = sType.mid(1).toInt();
                                ++vrg;
                                if (noInquire) *noInquire  = (text.mid(vrg,end-vrg).stripWhiteSpace()).toInt();
                               }
               else            {sType      = text.stripWhiteSpace();
                                if (sType[0] != '$') type  = RubNameToType( sType );
                                else                 type  = sType.mid(1).toInt();
                               }
            }
        }
    }
 return type;
}

//------------------------------------------- Slot_FileSelected ----------------------------------------
/*! \brief Slot appelé lors de la sélection d'un fichier dans le menu "Glossaire".
 * Emet  le signal Sign_ActiverRubrique() poour mettre au premier plan la CMDI concernée par ce document.
 * Puis émet le signal Sign_GlossaireFileSelected qui sera connecté dans drtux à OnGlossaireFileClicked pour signifier à DrTux qu'un nouveau document doit être créé.
*/
void CApp::Slot_FileSelected()
{
}

//------------------------------------------- Slot_OnMenuActivated ----------------------------------------
/*! \brief lorsque le popup est activé récupère le fichier sélectionné (le chemin est dans le whatsThis de l'item du popup.
*/
void CApp::Slot_OnMenuActivated(int id)
{if ((id != -1) && (m_Popup))
    { m_SelectedFile   = m_Popup->whatsThis(id);
      m_IdMenu         = id;
    }
  else m_SelectedFile = "";
  //qDebug(tr("fichier selectionne : %1   ID : %2").arg(m_SelectedFile, QString::number(id)));
}

//------------------------------------------- Slot_On_MainMenuActivated ----------------------------------------
void CApp::Slot_On_MainMenuActivated(int id)
{m_LastMainOption = m_Popup->text ( id );
}

