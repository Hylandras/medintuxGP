
/**************************** C_BDM_Control.cpp ***************************
*  #include "C_BDM_Control.h"         QT4-ready                           *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
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

#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Control.h"
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QTextEdit>

//=================== C_BDM_Control ===================================
/*! \class C_Gest_BDM_Pool
 *  \brief Class to maintain all the DB element for API
 */
//-------------------------------- C_BDM_Control ------------------------------------------------------
/*! \brief  the constructor intialize BDM structures (each thread must have separate QSqlDatabase)
 *  \param  const QString &pathAppli     general application path
 *  \param  const QString &pathIni       general configuration file application path
 *  \param  const QString &pathTheme     icones and pixmap folder
 *  \param  QTextEdit       *pLogWidget  pointer QTextEdit log widget (can be set to zero).
 *  \param  QObject *parent              parent object
 */

C_BDM_Control::C_BDM_Control(const QString &pathAppli       ,
                             const QString &pathIni         ,
                             const QString &pathTheme       ,
                             QTextEdit *logWidget           ,
                             QObject *parent)
   : QObject(parent)
{   m_pathAppli     = pathAppli;
    m_pathIni       = pathIni;
    m_dataIni       = CGestIni::Param_UpdateFromDisk( m_pathIni );
    m_pathTheme     = pathTheme;
    m_lastMessage   = "";
    QString dataCfg = CGestIni::Param_UpdateFromDisk( m_pathAppli +"C_BDM_Api.cfg") ;
    m_pC_BDM_Api    = new C_BDM_Api( dataCfg     ,
                                     "THREAD_"   ,
                                     m_dataIni   ,
                                     m_pathTheme ,
                                     m_pathAppli ,
                                     m_dataIni   ,
                                     0,                // no parent
                                     logWidget,        // log widget
                                     0);               // no user monographie (we need just interaction controls)
    // m_pC_BDM_Api->outMessage(tr("C_BDM_Control::C_BDM_Control() setLogWidget(%1) is initialised").arg( (logWidget?logWidget->objectName():tr("No Log Widget")) ));
    if ( ! m_pC_BDM_Api->isValid() )
       { m_pC_BDM_Api->outMessage(tr("C_BDM_Control::C_BDM_Control() m_pC_BDM_Api is invalid"));
         if ( ! QFile::exists(m_pathAppli +"C_BDM_Api.cfg"))   m_pC_BDM_Api->outMessage(tr("C_BDM_Control::C_BDM_Control() %1 not found").arg(m_pathAppli +"C_BDM_Api.cfg"));
         if (dataCfg.length()==0)                              m_pC_BDM_Api->outMessage(tr("C_BDM_Control::C_BDM_Control() %1 empty").arg(m_pathAppli +"C_BDM_Api.cfg"));
         return;
       }
    this->setObjectName("C_BDM_Api_CTRL_"+m_pC_BDM_Api->database().connectionName());
    QString namePlugin = CGestIni::Param_ReadUniqueParam(m_dataIni.toLatin1(), "medicatux data source", "name");
    set_BDM_Plugin(namePlugin,0);
    m_pC_BDM_Api->outMessage(tr("C_BDM_Control::C_BDM_Control() plugin %1 is initialised").arg(namePlugin));
}

//-------------------------------- ~C_BDM_Control ------------------------------------------------------
C_BDM_Control::~C_BDM_Control()
{ C_BDM_PluginI *pC_BDM_PluginI = 0;
 if (m_pC_BDM_Api) delete m_pC_BDM_Api;
  QMap<QString, C_BDM_PluginI*>::const_iterator it = m_pC_BDM_PluginI_Map.constBegin();
  while (it != m_pC_BDM_PluginI_Map.constEnd())
        { pC_BDM_PluginI = it.value();
          delete pC_BDM_PluginI;
          ++it;
        }
}

//-------------------------------- setPatientAndUserContexte ------------------------------------------------------
/*! \brief set user and patient context
 *  \param  C_PatientCtx *pC_PatientCtx pointer on patient context (must be persistant while C_BDM_Control exists)
 *  \param  C_UserCtx *pC_UserCtx       pointer on user context (must be persistant while C_BDM_Control exists)
 */
void C_BDM_Control::setPatientAndUserContexte(C_PatientCtx *pC_PatientCtx, C_UserCtx *pC_UserCtx)
{
  m_pC_BDM_Api->setPatientContexte(pC_PatientCtx);
  m_pC_BDM_Api->setUserContexte(pC_UserCtx);
}

//-------------------------------- set_BDM_Plugin ------------------------------------------------------
/*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
 *  \param QString namePlugin of C_BDM_InitPluginI data source ( 'theriaque' 'datasemp')
 *  \param int oldMustBeDeleted
 *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
 *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
 *  \return C_BDM_PluginI * pointer on plugin or zero if error
 */

C_BDM_PluginI *C_BDM_Control::set_BDM_Plugin(const QString &_namePlugin, int oldMustBeDeleted )   // theriaque datasemp
{   QTime debTime                 = QTime::currentTime();
    QTime debGTime                = QTime::currentTime();
    QString namePlugin            = _namePlugin.toUpper().trimmed();
    C_BDM_PluginI *pC_BDM_PluginI = 0;
    m_lastMessage                 = "";
    //.............. si besoin (oldMustBeDeleted !=0)  effacer l'eventuelle instance ..........................
    if ( oldMustBeDeleted !=0 )
       { m_lastMessage += tr("\n=============== C_BDM_Control::set_BDM_Plugin() delete if exists '%1' =====================").arg(_namePlugin );
         QMap<QString, C_BDM_PluginI*>::const_iterator it = m_pC_BDM_PluginI_Map.find(_namePlugin.toUpper());
         if (it != m_pC_BDM_PluginI_Map.constEnd() )   // >>>>> si trouve le tuer et recreer une instance
            { C_BDM_PluginI* pC_BDM_PluginI = it.value();
              if ( pC_BDM_PluginI )
                 { m_pC_BDM_PluginI_Map.remove(_namePlugin.toUpper());
                   delete pC_BDM_PluginI;
                   m_lastMessage += tr("\n.           '%1' is deleted").arg(_namePlugin );
                 }
           }
       }

    //.............. creer ou recuperer l'eventuelle instance ..........................
    m_lastMessage += tr("\n=============== C_BDM_Control::set_BDM_Plugin() SET DATABASE '%1' =====================").arg(namePlugin );
    //.............. on cherche le plugin de base de donnee dans la liste.................
    QMap<QString, C_BDM_PluginI*>::const_iterator it = m_pC_BDM_PluginI_Map.find(namePlugin);
    //.............. si non trouve on initialise le plugin ...............................
    if (it == m_pC_BDM_PluginI_Map.constEnd() )  // si pas trouve le creer et le rajouter a la liste
       {    QString confData =   QString("[BDM_Configuration]\n"
                                         "     PathAppli     = %2\n"
                                         "     PathIni       = %3\n").arg( m_pathAppli , m_pathIni );
            if (namePlugin=="THERIAQUE")
               { debTime        = QTime::currentTime();
                 pC_BDM_PluginI = new C_BDM_TheriaquePlugin(confData,"THREAD_", 0, 0, 0);
                 m_lastMessage += tr("\nDatabase '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") );
               }
            else if (namePlugin=="C_BDM_GOUVPLUGIN")
              { debTime  = QTime::currentTime();
                pC_BDM_PluginI =  new C_BDM_GouvPlugin(confData, "THREAD_", 0, 0, 0);
                m_lastMessage += tr("Database '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") );
              }
            else if (namePlugin=="C_BDM_GENERICPLUGIN")
              { debTime  = QTime::currentTime();
                pC_BDM_PluginI =  new C_BDM_GenericPlugin(confData, "THREAD_", 0, 0, 0);
                m_lastMessage += tr("Database '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") );
              }
            else if (namePlugin=="DATASEMP")
               { debTime        = QTime::currentTime();
                 pC_BDM_PluginI =  new C_BDM_DatasempPlugin(confData,"THREAD_", 0, 0, 0);
                 m_lastMessage += tr("\nDatabase '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") );
               }
            else if (namePlugin.length()==0)
               { m_lastMessage += tr("\nfailure in data source initialisation :  <b>empty plugin name</b>");
               }
            else
               { m_lastMessage += tr("\nfailure in data source <b>%1</b> unknow name").arg(namePlugin);
               }

            //............................... CASSOS SI NOM INCONNU ................................
            if (pC_BDM_PluginI==0)  return 0;
            if (pC_BDM_PluginI->isInitialised())
               {  m_pC_BDM_PluginI_Map[namePlugin] = pC_BDM_PluginI;
               }
            else
               {  pC_BDM_PluginI = 0;
                  m_lastMessage += tr("\nfailure in drug data source <b>%1</b> initialisation").arg(namePlugin);
               }
       }
    //.............. si trouve on prend le plugin ...............................
    else
      {pC_BDM_PluginI = it.value();
      }

    //....................... activer ce plugin dans tous les objets qui en dependent ..................
    if ( pC_BDM_PluginI && pC_BDM_PluginI->isInitialised() && m_pC_BDM_Api)
       {
          m_pC_BDM_Api->connectToDataSource( pC_BDM_PluginI );             // donner au controleur le plugin
          m_pC_BDM_Api->addOwner(pC_BDM_PluginI->owner());                 // n'est rajoute que si n'existe pas
       }
    else
       {pC_BDM_PluginI = 0;
       }
    m_lastMessage += tr("\nDatabase '%1' connectee en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debGTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") );

    return pC_BDM_PluginI;
}


//============================= C_InteractionsThread ==============================================
//-------------------------------- C_InteractionsThread ------------------------------------------------------
/*! \brief  the constructor intialize BDM structures (each thread must have separate QSqlDatabase)
 *  \param  const QString &pathAppli               general application path
 *  \param  const QString &pathIni                 general configuration file application path
 *  \param  const QString &pathTheme               icones and pixmap folder
 *  \param  const  C_PatientCtx  &C_PatientCtx     le contexte patient (doit obligatoirement etre par copie car le thread doit pouvoir continuer)
 *  \param  const  C_UserCtx     &C_UserCtx        le contexte utilisateur (doit obligatoirement etre par copie car le thread doit pouvoir continuer seul)
 *  \param  const QString &pathTheme               icones and pixmap folder
 *  \param  QObject *parent                        parent object
 */
C_InteractionsThread::C_InteractionsThread (  const QString &pathAppli       ,
                                              const QString &pathIni         ,
                                              const QString &pathTheme       ,
                                              C_PatientCtx  *pC_PatientCtx   ,
                                              C_UserCtx     *pC_UserCtx      ,
                                              QTextEdit     *logWidget       ,
                                              QObject       *parent
                                           )
    : QThread ( parent )
{ m_pC_BDM_Control      = new C_BDM_Control( pathAppli       ,
                                             pathIni         ,
                                             pathTheme       ,
                                             logWidget       ,
                                             0
                                           );
  m_pC_PatientCtx = pC_PatientCtx;    // on procede par copie car si le thread tourne encore alors que pC_PatientCtx est detruit --> BUG
  m_pC_UserCtx    = pC_UserCtx;       // on procede par copie car si le thread tourne encore alors que pC_UserCtx    est detruit --> BUG
}
//-------------------------------- ~C_InteractionsThread ------------------------------------------------------
C_InteractionsThread::~C_InteractionsThread()
{ m_mutex.lock();
  if (m_pC_BDM_Control && m_pC_BDM_Control->BDM_Api())
     { m_pC_BDM_Control->BDM_Api()->stopAllInteractions();
     }
  if ( wait(10000)==false )
     { terminate ();
       wait(100);
       qDebug() << "C_InteractionsThread() : forced to terminate";
     }
  else
     { qDebug() << "C_InteractionsThread() : standard terminated";
     }
  m_mutex.unlock();
 if (m_pC_BDM_Control) delete m_pC_BDM_Control;
}

//-------------------------------- set_Thread_BDM_Plugin ------------------------------------------------------
/*! \brief set user BDM plugin interface (class interface for unified commercial database).
 *  \param  const QString &_namePlugin name of plugin (Datasemp, Theriaque ...)
 *  \param int oldMustBeDeleted
 *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
 *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
 */
void C_InteractionsThread::set_Thread_BDM_Plugin(const QString &_namePlugin, int oldMustBeDeleted /*=0*/)
{ if (m_pC_BDM_Control==0) return;
   m_pC_BDM_Control->set_BDM_Plugin(_namePlugin, oldMustBeDeleted);
   this->setObjectName("CTRL_TRHEAD_"+_namePlugin);
}

//-------------------------------- setContext ------------------------------------------------------
/*! \brief set user and patient context
 *  \param  C_PatientCtx *pC_PatientCtx     pointer on patient context (must be persistant while C_BDM_Control exists)
 *  \param  C_UserCtx *pC_UserCtx           pointer on user context (must be persistant while C_BDM_Control exists)
 *  \param  const QStringList &grammarList  products list from current patient ordonnance to be modified
 */
void C_InteractionsThread::setContext (C_PatientCtx *pC_PatientCtx, C_UserCtx *pC_UserCtx, const QStringList &grammarList)
{if (m_pC_BDM_Control==0) return;

 //if (pC_PatientCtx) m_pC_PatientCtx = pC_PatientCtx;    // on procede par copie car si le thread tourne encore alors que pC_PatientCtx est detruit --> BUG
 //if (pC_UserCtx)    m_pC_UserCtx    = pC_UserCtx;       // on procede par copie car si le thread tourne encore alors que pC_UserCtx    est detruit --> BUG
 m_pC_BDM_Control->setPatientAndUserContexte(pC_PatientCtx, pC_UserCtx);
 m_grammarList = grammarList;
}

//------------------------------------ run --------------------------------------------------------------------
/*! \brief main methode called  when thread run
 */
void C_InteractionsThread::run()
{   m_HtmlResult = m_pC_BDM_Control->BDM_Api()->evaluateAllInteractions(m_grammarList);
}
//------------------------------------ html_result --------------------------------------------------------------------
/*! \brief   return  interaction result html
 *  \return  QString interaction result html
 */
QString C_InteractionsThread::html_result()
{   if ( isRunning() ) return tr("datas are not ready, work is in progress ...");
    return m_HtmlResult;
}

