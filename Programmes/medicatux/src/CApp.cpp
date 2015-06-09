/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: roland-sevin@medintux.org                                            *
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
//    Without deviation from the norm, progress is not possible. Frank Zappa (1940-1993)
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
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"                       // Gestion du theme de l'appli
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/medicabase/C_UserCtx.h"

static CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application accessible avec CApp::pCApp()


//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{writeParam((m_NameAppli+" session").toLatin1(),"closed", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz").toLatin1());

 updateIniParamToDisk();
 C_BDM_PluginI *pC_BDM_PluginI = 0;
 QMap<QString, C_BDM_PluginI*>::const_iterator it = m_pC_BDM_PluginI_Map.constBegin();
 while (it != m_pC_BDM_PluginI_Map.constEnd())
       { pC_BDM_PluginI = it.value();
         delete pC_BDM_PluginI;
         ++it;
       }
 if (m_pC_UserCtx)      delete m_pC_UserCtx;
 if (m_pC_PatientCtx )  delete m_pC_PatientCtx;
 if (m_pQSharedMemory)  delete m_pQSharedMemory;
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
: C_AppCore( mui_name, argc, argv ), C_Macro(this)
{
 #ifndef QT_5
     QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
 #endif
     m_pC_UserCtx                = 0;
     m_pC_PatientCtx             = 0;
     m_mode                      = "";
     m_pQSharedMemory            = 0;
     m_pQSharedMemoryCheckTimer  = 0;
     m_pQMainWindow              = 0;
     m_lastArg                   = "";
    // int                   debug = 0;
    //.............................. initialiser la base C_BaseMedica ..........................................
#ifdef QT_DEBUG
    // debug = 1;
#endif
    if (argc>1 && QString(argv[1])=="DEBUG")
       { m_PatientCtxFileName       =  pathAppli()+"Ressources/ComFiles/DEBUG_PatientContext.xml";
         m_UserCtxFileName          =  pathAppli()+"Ressources/ComFiles/DEBUG_UserContext.xml";
         m_GrammarFileName          =  pathAppli()+"Ressources/ComFiles/DEBUG_PatientContext.stc";
       }
    else
       { m_PatientCtxFileName       =  (argc>1)?argv[1]:"";    // %{sourceDir}/../bin/Ressources/ComFiles/C_PatientCtx.xml
         m_UserCtxFileName          =  (argc>2)?argv[2]:"";    // %{sourceDir}/../bin/Ressources/ComFiles/UserContext-ro-D.xml
         m_GrammarFileName          =  (argc>3)?argv[3]:"";    // %{sourceDir}/../bin/Ressources/ComFiles/OrdoDebug.xml
         m_mode                     =  (argc>4)?argv[4]:"";
       }
    m_PatientCtxFileName       = QDir::cleanPath (m_PatientCtxFileName);
    qDebug()<< Q_FUNC_INFO<<" m_PatientCtxFileName      : "<< m_PatientCtxFileName;
    qDebug()<< Q_FUNC_INFO<<" m_UserCtxFileName : "        << m_UserCtxFileName;
    qDebug()<< Q_FUNC_INFO<<" m_GrammarFileName : "        << m_GrammarFileName;
    setQuitOnLastWindowClosed(true);

    C_PosologieGrammar::set_QMapGenre( this->pathAppli()+"Ressources/GrammarGenreSexePlural.txt");
    G_pCApp           =    this;
    setWindowIcon (Theme::getIcon("medicatux/medicatux.png") );
     if (argc>0)
        {
          m_lastArg = argv[argc-1];
          if ( m_lastArg.startsWith("shared_memory_id=") )
             {
               /*  marche po
               QString name               =  m_lastArg.mid(17);
               m_pQSharedMemory           = new QSharedMemory(name, G_pCApp);
               m_pQSharedMemory->setKey(name);
               */
               m_pQSharedMemoryCheckTimer = new QTimer(this);
               connect(m_pQSharedMemoryCheckTimer, SIGNAL(timeout()), this, SLOT(Slot_QSharedMemoryCheckTimer()));
               m_pQSharedMemoryCheckTimer->start(1000);
             }
        }
}

//------------------------------------- Slot_QSharedMemoryCheckTimer ------------------------------------
/*! \brief this slot must be called to check shared memory contents
 */
void CApp::Slot_QSharedMemoryCheckTimer()
{   m_pQMainWindow->raise();
    // outMessage( tr ("Slot_QSharedMemoryCheckTimer(%1) ").arg("RAISE"));
    return;
  /*
  if ( m_pQMainWindow == 0 )             return;
  if ( m_pQSharedMemory == 0 )           return;

  if (! m_pQSharedMemory->create ( 1024 ) )
     { outMessage( tr ("Slot_QSharedMemoryCheckTimer(%1) %2").arg(m_pQSharedMemory->key(), m_pQSharedMemory->errorString()));
     }
  outMessage( tr ("Slot_QSharedMemoryCheckTimer(%1) ").arg(m_pQSharedMemory->key()));
  if ( m_pQSharedMemory->isAttached() )
     { outMessage( tr ("Slot_QSharedMemoryCheckTimer(%1) already attached").arg(m_pQSharedMemory->key()));
     }
  else if ( !m_pQSharedMemory->attach(QSharedMemory::ReadOnly) )
     { if (m_pQSharedMemory->error()==QSharedMemory::PermissionDenied)
          { m_pQMainWindow->raise();
            outMessage( tr ("Slot_QSharedMemoryCheckTimer(%1) RAISE ").arg(m_pQSharedMemory->errorString()));
          }
       else outMessage( tr ("Slot_QSharedMemoryCheckTimer(%1) attach failled ").arg(m_pQSharedMemory->errorString()));
       //return;
     }
  QString str  = "";
  m_pQSharedMemory->lock();
  const char    *pt = (const char*) m_pQSharedMemory->constData();
  if (pt)
     { str = pt;
       // *pt = 0;
     }
  m_pQSharedMemory->unlock();
  //m_pQSharedMemory->detach();
  // if (str=="RAISE_WINDOW")
  outMessage( "shared_memory content :  " + str);
  */
}

//--------------------------------------------- pCApp -------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
*/
CApp* CApp::pCApp()
{return G_pCApp;
}
//--------------------------------------------- return_mode -------------------------------------------------------------------
/*! \brief retourne le mode de sortie desire
*/
QString CApp::return_mode()
{return m_mode;
}
//------------------------------------ waitForEndPageDisplay -------------------------------
/*! \brief wait while a web page finish display
 *  \param timeOut  long  loop number max
 *  \param pQWebView  QWebView * scroll to end
 *  \return number of loop out
 */
long CApp::waitForEndPageDisplay(long timeOut, QWebView *pQWebView /* =0 */)
{while(timeOut && isEndPageDisplay()==false)
       {--timeOut;
        processEvents();
        if (pQWebView) pQWebView->scroll ( 0, pQWebView->height() );
       }
 return timeOut;
}
//------------------------------------ outMessage -------------------------------
/*! \brief output a message on std error out put and logwidget if defined
 *  \param mess  const QString  message to display
 *  \param file  const QString source file from where the message is display
 *  \param line  const QString source line in file from where the message is display
 *  \return message diplayed
 */
void   CApp::outMessage( const QString &mess, const QString &file /*  = "" */ , const int line  /* = -1 */)
{if (u_Html()) u_Html()->outMessage(mess, file, line);
 else          qDebug() << mess;
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

/*------------------------------ resolvePath --------------------------------------------------------------------*/
/*! \brief resoud un chemin comportant les eventuelles macros $Glossaire $Vigie $ToAbsPath $EndToAbsPath par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie absolutise si necessaire
*/
QString CApp::resolvePath(QString pathFile, const QString &pathGlossaire  /* = ""*/ )
{    pathFile = resolvePathKeyWord(pathFile, pathGlossaire);
     if ( QDir::isRelativePath ( pathFile ) ) pathFile  = QDir::cleanPath ( pathFile.prepend(m_PathAppli) );
     return pathFile;
}
/*------------------------------ resolvePathKeyWord --------------------------------------------------------------------*/
/*! \brief resoud un chemin comportant les eventuelles macros $Glossaire $Vigie $ToAbsPath $EndToAbsPath par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie absolutise que lors des mots clefs et UNIQUEMENT DANS CE CAS (utile lors arguments de programmes externes)
*/
QString CApp::resolvePathKeyWord(QString pathFile, const QString &pathGlossaire /* = "" */)
{    pathFile  = pathFile.trimmed().replace("$Glossaire", pathGlossaire);
     pathFile  = pathFile.replace("$Vigie", pathGlossaire + "Vigie");
     //if (pathFile[1]==':') pathFile  = pathFile.replace (1, 1, '|' ); //proteger le : de  C:
     //if (pathFile.startsWith("//")) pathFile = QString("**") + pathFile.mid(2);  // prot?ger les doubles "//" du debut (cas des repertoires reseaux)
     //pathFile.replace("//","/");    pathFile.replace("*","/"); pathFile.replace("|",":");
     //............. gestion des motsclefs ..........
     int pos     = -1;
     int deb     = -1;
     QString tmp = "";
     if ((pos = pathFile.lastIndexOf("$EndToAbsPath")) != -1)
        {do
          { if ( (deb     = pathFile.lastIndexOf("$ToAbsPath"),pos) != -1)
               { pathFile = pathFile.remove(pos,13);   // virer le $EndToAbsPath
                 tmp      = QDir::cleanPath (m_PathAppli + pathFile.mid(deb+10, pos - deb -10).trimmed());
                 pathFile = pathFile.replace (deb, pos - deb, tmp );
                 pos      = pathFile.lastIndexOf("$EndToAbsPath", deb);
               }
            else
               { pos = -1;
               }
          } while (pos != -1);
        }
     else if ( (pos = pathFile.indexOf("$ToAbsPath")) != -1)
        { pathFile = pathFile.left(pos) + QDir::cleanPath (m_PathAppli + pathFile.mid(pos+10).trimmed());
        }
     return pathFile;
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
