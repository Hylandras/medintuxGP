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
 *   Commissariat Ã   l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la FÃ©dÃ©ration, 75752 PARIS cedex 15.   *
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

#include <QFileInfo>
#include <QFile>

#include <QStringList>
#include <QTextStream>
#include <QIODevice>
#include <QProcess>
#include <QDateTime>
#include <QTextCodec>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include <QComboBox>

#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"
#include "../../MedinTuxTools-QT4/CCoolPopup.h"                             // Gestion des popups surgissants
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
static CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application accessible avec CApp::pCApp()


//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{CouCouKill();
 writeParam((m_NameAppli+" session").toAscii(),"closed", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz").toAscii());
 updateIniParamToDisk();
}


//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
: C_AppCore( mui_name, argc, argv )
{   m_pC_Utils_Html      = 0;
    m_pathTemp           = "";
    m_pCCoolPopup        = 0;
    m_CCoolPopupTempo    = 2000;          /*!< temps de persistance du menu ?  l'ecran en microsecondes */
    m_pC_BaseSigEntrees  = 0;
    G_pCApp              =    this;       // le faire ici car on s'en sert apres
    controlDb();  // si ok m_pC_BaseSigEntrees est instancie sinon est sur zero
    setPathTemp(readUniqueParam("sigentrees session", "Repertoire temporaire"));
    setQuitOnLastWindowClosed(TRUE); // ???????????????
}


//--------------------------------------------- pCApp -------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
*/
CApp* CApp::pCApp()
{return G_pCApp;
}
//------------------------------------ controlDb -------------------------------
/*! \brief controle si la table Sig entree est bien creee dans base drtuxtest
 */
bool CApp::controlDb()
{   //.............................. recuperer config de la base ..........................................
    QString paramCfgData = CGestIni::Param_UpdateFromDisk(m_PathAppli + m_NameAppli+"Bases.cfg");

    if (m_pC_BaseSigEntrees) delete m_pC_BaseSigEntrees;
    m_pC_BaseSigEntrees = 0;
    if (m_pC_BaseSigEntrees==0)
       {m_pC_BaseSigEntrees      = new C_BaseSigEntrees(m_IniParam, paramCfgData, this);
        if (m_pC_BaseSigEntrees == 0)
           {qDebug () << tr("CApp::controlDb() echec de new C_BaseSigEntrees");
            return false;
           }
        if (m_pC_BaseSigEntrees->getLastError().length())
           {qDebug() << m_pC_BaseSigEntrees->getLastError();
            QMessageBox::warning(0, m_NameAppli,
                             tr("CApp::controlDb() Echec de l'ouverture de la base : \n%1.").arg(m_pC_BaseSigEntrees->getLastError()),
                             QMessageBox::Cancel
                            );
            delete m_pC_BaseSigEntrees;
            m_pC_BaseSigEntrees = 0;
            return false;
           }
      }
    //....................... verifier existence base et si non la creer ..................................
    QString sqlFiles = pathAppli()+"Ressources/SqlFiles/CreateTables_SigEntrees.sql";
    if ( m_pC_BaseSigEntrees->isThisTableExist("entrees_sigems")==-1 &&
         QFile::exists(sqlFiles))
       {qDebug () << tr("CApp::controlDb() table 'entrees_sigems' don't exists so i try to create it");
        m_pC_BaseSigEntrees->executeSQL(sqlFiles);
       }
    return true;
}

//------------------------------------ replaceComboboxListInParam -------------------------------
/*! \brief replace list variables in section parameters by datas from combobox list
 *  \param QComboBox *pQComboBox  combobox from which list must be replace
 *  \param const QString &section section where to find list of variables
 *  \param lconst QString &variable   variable name of each list elements
 */

void CApp::replaceComboboxListInParam(QComboBox *pQComboBox, const QString &section, const QString &variable)
{
 QStringList list ;
 for (int i=0; i < pQComboBox->count(); ++i) list.append(pQComboBox->itemText (i));
 CGestIni::replaceList(m_IniParam, section, variable,  list );
}

//------------------------------------ isTraceEnabled -------------------------------
/*! \brief reconnecte la base de donnees
 */
bool CApp::connectDb()
{
    if (m_pC_BaseSigEntrees)
    {if ( m_pC_BaseSigEntrees->reConnect(m_IniParam) )
        { return true;
        }
     else
        { delete m_pC_BaseSigEntrees;
          m_pC_BaseSigEntrees = 0;
        }
    }
 return false;

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

//------------------------------------ getProfilsList -------------------------------
/*! \brief get list variables in section parameters
 *  \param const QString &section section where to find list of variables
 *  \param lconst QString &variable   variable name of each list elements
 *  \return variables list
 */
QStringList CApp::getProfilsList(const QString &varToFind, const QString &section /* = "sigentrees session" */)
{QStringList profilsList;
 CGestIni::Param_GetList(this->m_IniParam, section, varToFind, profilsList,1,1);
 if (profilsList.size()==0) profilsList.clear();
 return profilsList;
}
//------------------------------------ isTraceEnabled -------------------------------
/*! \brief retourne si le demon ftp doit etre active en lisant le fichier de configuration
 */
bool CApp::isTraceEnabled()
{return (readUniqueParam("sigentrees session", "LogFTP").toLower()[0]=='o'); // TODO lire une variable plutot qu'un fichier ...
}
//------------------------------------ isFtpDaemonEnabled -------------------------------
/*! \brief retourne si le demon ftp doit etre active en lisant le fichier de configuration
 */
bool CApp::isFtpDaemonEnabled()
{return (readUniqueParam("sigentrees session", "Ftp daemon enabled").toLower()[0]=='o');
}
//-------------------------- setFtpDaemonEnabled -------------------------------------------
/*! \brief place l'etat actif ou inactif du demon ftp
 */
void CApp::setFtpDaemonEnabled(int state /*=true */)
{QString etat = (state==Qt::Checked)?"ok":"no";
 writeParam("sigentrees session", "Ftp daemon enabled", etat.toAscii());
 updateIniParamToDisk();
}
//------------------------------------ justPatientsWithResults -------------------------------
/*! \brief retourne si les patients sans resultats doivent être affiches
 */
bool CApp::justPatientsWithResults()
{return (readUniqueParam("sigentrees session", "Just patients with results").toLower()[0]=='o');
}
//------------------------------------ getResultsLimit -------------------------------
/*! \brief retourne le nombre limite de resultats a afficher
 */
QString CApp::getResultsLimit()
{return readUniqueParam("sigentrees session", "Limiter le nombre de resultats");
}
//-------------------------- setFtpDaemonEnabled -------------------------------------------
/*! \brief place l'etat actif ou inactif si les patients sans resultats doivent être affiches
 */
void CApp::setJustPatientsWithResults(int state /*=true */)
{QString etat = (state==Qt::Checked)?"ok":"no";
 writeParam("sigentrees session", "Just patients with results", etat.toAscii());
 updateIniParamToDisk();
}
//------------------------------------ isDateFilterEnabled -------------------------------
/*! \brief retourne si le filtre des dates est actif en lisant le fichier de configuration
 */
bool CApp::isDateFilterEnabled()
{return (readUniqueParam("sigentrees session", "date filter").toLower()[0]=='o');
}
//-------------------------- setDateFilterEnabled -------------------------------------------
/*! \brief place l'etat actif ou inactif du filtre de dates
 */
void CApp::setDateFilterEnabled(int state /*=true */)
{QString etat = (state==Qt::Checked)?"ok":"no";
 writeParam("sigentrees session", "date filter", etat.toAscii());
 updateIniParamToDisk();
}
//-------------------------- setdefaultService -------------------------------------------
/*! \brief place le service par defaut
 */
void CApp::setdefaultService(const QString &serv)
{writeParam("sigentrees session", "Service", serv.toAscii());
 updateIniParamToDisk();
}
//-------------------------- setdefaultService -------------------------------------------
/*! \brief retourne le service par defaut
 */
QString CApp::getdefaultService()
{return readUniqueParam("sigentrees session", "Service");
}
//-------------------------- getdefaultFtp -------------------------------------------
/*! \brief retourne le ftp par defaut
 */
QString CApp::getdefaultFtp()
{return readLine("sigentrees session", "ServeurFTP");
}
//-------------------------- setdefaultService -------------------------------------------
/*! \brief place les parametres TC IP par defaut
 */
void  CApp::setdefaultDB_Connexion(const QString &param)
{writeParam("Connexion", "Parametres", param.toAscii());
 updateIniParamToDisk();
}
//-------------------------- getdefaultFtp -------------------------------------------
/*! \brief retourne les parametre tcp ip par defaut
 */
QString CApp::getdefaultDB_Connexion()
{return readLine("Connexion", "Parametres");
}
//--------------------------------------------- getDB -------------------------------------------------------------------
/*! \brief retourne l'instance du gestionnaire de base de données.
*/
C_BaseSigEntrees* CApp::getDB()
{
    return m_pC_BaseSigEntrees;
}

//-------------------------- initUtilsHtml -------------------------------------------
void CApp::initUtilsHtml(QObject *parent , QNetworkAccessManager *pQNetworkAccessManager , QTextEdit *pQTextEditLog )
{m_pC_Utils_Html = new C_Utils_Html(parent , pQNetworkAccessManager , pQTextEditLog );
}
//-------------------------- u_Html -------------------------------------------
C_Utils_Html *CApp::u_Html()
{return m_pC_Utils_Html;
}
//------------------------------------ outMessage -------------------------------
/*! \brief output a message on std error out put and logwidget if defined
 *  \param mess  const QString  message to display
 *  \param file  const QString source file from where the message is display
 *  \param line  const QString source line in file from where the message is display
 *  \return message diplayed
 */
void   CApp::outMessage( const QString &mess, const QString &file /*  = "" */ , const int line  /* = -1 */)
{if (!isTraceEnabled()) return;
 if (u_Html()) u_Html()->outMessage(mess, file, line);
 else          qDebug() << mess.toLatin1();
 //outMessage(mess, file, line);
}
//------------------------------------ setPathTemp -------------------------------
 /*! \brief positionne le chemin du repertoire temporaire
  *  \param const QString &pathTemp valeur que prendra le chemin du repertoire temporaire
  */
 void CApp::setPathTemp(const QString &pathTemp)
 { if (pathTemp.length()==0) return;
   m_pathTemp  =   pathTemp;
   if (QDir::isRelativePath(m_pathTemp))  m_pathTemp = QDir::cleanPath ( m_pathTemp.prepend(pathAppli()) );
 }
 //------------------------------------ getPathTemp -------------------------------
 /*! \brief retourne le chemin du repertoire temporaire
  *  \return QString  le chemin du repertoire temporaire
  */
 QString CApp::getPathTemp()
 {return m_pathTemp;
 }
  //----------------------------------- CouCouKill ---------------------------------------------------
  /*! \brief Detruit le popup amusant */
  void  CApp::CouCouKill()
  {if (m_pCCoolPopup) delete m_pCCoolPopup;
   m_pCCoolPopup = 0;
  }
  //------------------------------------------------- CouCou ----------------------------------------------------
  /*! \brief Affiche un popup amusant pour afficher des messages */
  void CApp::CouCou(const QString &message, const QColor &color, const QFont &ft , int tempo /* = 1000 */, const QString &imagePath /* ="" */)
  {if (m_pCCoolPopup) delete m_pCCoolPopup;
   m_CCoolPopupTempo = tempo;
   QString path      = "";

   if (imagePath.length()==0 || !QFile::exists(imagePath)) path  = Theme::getPath() + "MessagePop.png";
   else                                                    path  = imagePath;
   QPixmap qpm(path);
   m_pCCoolPopup = new CCoolPopup( path ,
                                   (int)CCoolPopup::WithTransLabel,
                                   0,
                                   0,
                                   qpm.width(),
                                   qpm.height(),
                                   message);
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
      {readParam(  "Popup Message", "image", &path);
       path.replace("$Theme",Theme::getPath(Theme::WithNoSeparator));
       if (!QFile::exists( path )) path  = Theme::getPath() + "MessagePop.png";
      }

   QPixmap qpm(path);
   m_pCCoolPopup = new CCoolPopup( path ,
                                   CCoolPopup::WithTransLabel,
                                   0,
                                   0,
                                   qpm.width(),
                                   qpm.height(), message);
   //......... regarder si pas d'autres param ...............................................
   QFont ft = font();
   if (readParam( "Popup Message", "font_family", &val)==QString::null) ft.setFamily(val);
   if (readParam( "Popup Message", "font_size",   &val)==QString::null) ft.setPointSizeF(val.toDouble());
   if (readParam( "Popup Message", "font_weight", &val)==QString::null) ft.setWeight(qMin(99,val.toInt()));
   if (readParam( "Popup Message", "font_bold",   &val)==QString::null) ft.setBold((bool)val.toInt());
   if (readParam( "Popup Message", "font_italic", &val)==QString::null) ft.setItalic((bool)val.toInt());
   if (readParam( "Popup Message", "tempo",       &val)==QString::null) if (tempo==0) m_CCoolPopupTempo = val.toInt();
   m_pCCoolPopup->setFont(ft);
   if (readParam( "Popup Message", "font_color",  &val)==QString::null)  m_pCCoolPopup->setTextColor(QColor(val));
   connect( m_pCCoolPopup,  SIGNAL( Sign_Clicked(CCoolPopup *) ),   this, SLOT(Slot_CouCouStop(CCoolPopup *))  ) ;
   connect( m_pCCoolPopup,  SIGNAL( Sign_PopupUp(CCoolPopup *) ),   this, SLOT(Slot_CouCouUp (CCoolPopup *))  ) ;
   connect( m_pCCoolPopup,  SIGNAL( Sign_PopupDown(CCoolPopup *) ), this, SLOT(Slot_CouCouStop(CCoolPopup *))  );
  }

  //--------------------------------------- CouCouUp -------------------------------------------------------------
  /*! \brief Appele lorsque le popup est en haut, attend le tempo puis fait descendre le popup */
  void CApp::Slot_CouCouUp(CCoolPopup * pCCoolPopup)
  {if (m_CCoolPopupTempo != -1) QTimer::singleShot(m_CCoolPopupTempo, pCCoolPopup, SLOT(Disappear()) );
  }
  //--------------------------------------- CouCouStop -----------------------------------------------------------
  /*! \brief Appele pour detruire le popup */
  void CApp::Slot_CouCouStop(CCoolPopup * pCCoolPopup)
  {if (m_pCCoolPopup==pCCoolPopup)
      {pCCoolPopup->disconnect( SIGNAL(Sign_PopupUp(CCoolPopup *)) );
       pCCoolPopup->disconnect( SIGNAL(Sign_PopupDown(CCoolPopup *)) );
       QTimer::singleShot(0, this, SLOT(Slot_CouCouDestroy()) );
      }
  }
  //--------------------------------------- CouCouDestroy --------------------------------------------------------
  /*! \brief Appele pour detruire le popup */
  void CApp::Slot_CouCouDestroy()
  {if (m_pCCoolPopup) delete m_pCCoolPopup;
   m_pCCoolPopup = 0;
   emit Sign_CouCouDestroy();
  }


  //---------------------------------------------- execCalendrier -----------------------------------------------------------------------
  QString CApp::execCalendrier(const QDate &dateIn)
  {     //............... lancer le calendrier .................................................
      QString pathPlugin = readUniqueParam("Gestion du calendrier", "pathPlugin");
      if (pathPlugin.length()==0){ pathPlugin  =   "../../calendrier/bin/calendrier"; }
      //..................................... ne pas rajouter "/" car c'est un nom de fichier ..........................
      if ( QDir::isRelativePath ( pathPlugin ) )  pathPlugin = QDir::cleanPath(pathPlugin.prepend(m_PathAppli));
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

     return PluginExe((QObject *)this, pluginScript.toAscii(), CApp::endWait);
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
                                                getDB()->GUID_Create()+"-New",
                                                " ",
                                                " ",
                                                " " ,
                                                param,
                                                CApp::endWait );
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
  //                        tout autre delire en accord avec la syntaxe du plugin dËs fois que ...
  //                        si ce fichier est vide, o? n'existe pas le plugin retournera son resultat dans ce fichier
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
  //   13 --> 13+Nb paramËtres

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

   if ( QDir::isRelativePath ( pathExe ) ) pathExe = QDir::cleanPath (pathExe.prepend(pathAppli) );
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
      {outMessage(tr("Path to plugin not found: ").arg(pathExe));
      }
   QString nameExch = QFileInfo(pathExe).fileName()+"-"+ guid +".exc";
   QString      dst = readUniqueParam ("Repertoire Temporaire", "Repertoire");
   if ( dst.length()==0 )  dst = "../../tmp";
   if ( QDir::isRelativePath (dst) )  dst = QDir::cleanPath (dst.prepend(m_PathAppli));

   dst +=  "/" + nameExch;
   /*
   QFile::remove (dst);
   QFile   qfdst (dst);
   if (qfdst.open(QIODevice::ReadWrite)==FALSE)
      {QMessageBox::critical (0, QObject::tr("PluginExe(): write pathExch"),
                                 QObject::tr("The file\n'")+ dst + QObject::tr("'\ncan't be opened"),
                                 QMessageBox::Abort, Qt::NoButton, Qt::NoButton );
       return QObject::tr("Error:  PluginExe() destination file can't be opened");
      }

   //qfdst.writeBlock (maskExch, maskExch.length());
   qfdst.writeData ( maskExch.toAscii(), maskExch.length() );
   qfdst.close();
   */
   CGestIni::Param_UpdateToDisk(dst, maskExch);
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
   for ( QStringList::Iterator it = param.begin(); it != param.end(); ++it )  // 13 --> 13+Nb paramËtres
       {argList << (*it);
       }
   //................................. executer le process .............................................
   if ( m_PluginRun == ""  )
      {
        m_PluginRun  = pathExe;
        if (waitFlag!=CApp::endWait)
           {QProcess::startDetached (m_PluginRun, argList);
            return QString(tr("paralel process"));    // on attend rien on se casse
           }
        QProcess* proc  = new QProcess();
        connect(  proc, SIGNAL(error ( QProcess::ProcessError  )), this, SLOT(Slot_error ( QProcess::ProcessError  )) );

        proc->start(m_PluginRun, argList);
        proc->waitForStarted  (4000);
        //proc->waitForFinished (); //crash crash
        //QByteArray ba = proc->readAllStandardError ();
        //qDebug(ba);

        processEvents ();
        while (waitFlag==CApp::endWait && proc->state()==QProcess::Running )
             { QApplication::processEvents ();
             }
        m_PluginRun = "";
        //............lire le fichier d'echange ..........................
        //dst  = "/home/ro/QFseVitale-53671d5a-52c0-42ff-a39c-bed207109033-New.exc";
        if ( !QFile::exists( dst ) )     return  ret;
        CGestIni::Param_UpdateFromDisk(dst, ret);
        delete proc;
      }
    QFile::remove (dst);
    return ret;
  }

  //----------------------------------------------------- Slot_error -------------------------------------------------------------------------
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
   outMessage( erreur );
  }
