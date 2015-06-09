/********************************* C_ScruteFTP.cpp ************************
*  #include "CGestIni.h"         QT4-ready                                *
* -------------------------------------------..........................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* -------------------------------------------..........................*
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

#include <qglobal.h>
#include <QMessageBox>
#include "C_ScruteFTP.h"
#include "C_DSVar.h"
#include "CApp.h"
#include "C_BaseBioGest.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
//======================================= C_ScruteFTP ============================================================
//--------------------------------------- C_ScruteFTP ------------------------------------------------------------
C_ScruteFTP::C_ScruteFTP(QObject *parent) :
    QFtp(parent)
{m_pC_ScruteFTP_Thread = new C_ScruteFTP_Thread(this);
 m_vaitForAction       = "";
 m_hostname            = "";
 m_port                = "";
 m_pass                = "";
 m_login               = "";
 m_distantPath         = "";
 m_eraseDistantsFiles  = false;
 m_ScrutationTimer     = new QTimer(this);
 m_distantsFilesList.clear();

 m_eraseDistantsFiles = (CApp::pCApp()->readUniqueParam("biogest session" , "EffacementDistant").toLower()[0]=='o');
 m_eraseLocalsFiles   = (CApp::pCApp()->readUniqueParam("biogest session" , "EffacementLocal").toLower()[0]=='o');
 setScrutationInterval ((CApp::pCApp()->readUniqueParam("biogest session" , "Ftp scrutation").toInt()));

 connect(this,    SIGNAL(stateChanged ( int  ) )          , this, SLOT(Slot_stateChanged(int)));
 connect(this,    SIGNAL(commandStarted ( int  ) )        , this, SLOT(Slot_commandStarted(int)));
 connect(this,    SIGNAL(commandFinished ( int, bool  ) ) , this, SLOT(Slot_commandFinished(int, bool)));
 connect(this,    SIGNAL(listInfo(const QUrlInfo& ))      , this, SLOT(Slot_listInfo(const QUrlInfo&)));
 connect(m_ScrutationTimer,    SIGNAL(timeout())          , this, SLOT(Slot_on_ScrutationTimerOut()));

 setScrutationInterval ((CApp::pCApp()->readUniqueParam("biogest session" , "Ftp scrutation").toInt()));
}

//--------------------------------------- ~C_ScruteFTP -------------------------------------------
C_ScruteFTP::~C_ScruteFTP()
{if (m_ScrutationTimer)     delete m_ScrutationTimer;
 if (m_pC_ScruteFTP_Thread) delete m_pC_ScruteFTP_Thread;
 close();
}
//--------------------------------------- closeFTP -----------------------------------------------
void C_ScruteFTP::closeFTP()
{   m_vaitForAction = tr ("command : CLOSE");
    close();
    while (m_vaitForAction.length())  {qApp->processEvents(); }
}

//--------------------------------------- connectFTP -----------------------------------------------
void C_ScruteFTP::connectFTP()
{   clearPendingCommands();
    closeFTP();
    CApp::pCApp()->readParam("biogest session", "ServeurFTP" ,&m_hostname, &m_port, &m_login, &m_pass, &m_distantPath);
    //...................... se connecter au FTP ...............................
    m_vaitForAction = tr ("command : CONNECT TO HOST");
    QFtp::connectToHost ( m_hostname, (quint16) m_port.toUInt() );
    while (m_vaitForAction.length())  {qApp->processEvents(); }

    m_vaitForAction = tr ("command : LOGIN");
    if (m_pass.startsWith('#'))   m_pass = CGestIni::PassWordDecode(m_pass.mid(1));
    login(m_login, m_pass);
    m_pass = CGestIni::PassWordEncode(m_pass);
    m_pass.prepend('#');
    while (m_vaitForAction.length())  {qApp->processEvents(); }

    m_vaitForAction = tr ("command : CD : <font color=#ff5400><b>%1</b></font>").arg(m_distantPath);
    cd(m_distantPath);
    while (m_vaitForAction.length())  {qApp->processEvents(); }
}

//--------------------------------------- setScrutationInterval -----------------------------------------------
void C_ScruteFTP::setScrutationInterval(int interval)
{interval       =  qMax(interval,30);
 bool isActive = m_ScrutationTimer->isActive();
 m_ScrutationTimer->stop();
 m_ScrutationTimer->setInterval(interval*1000);
 if (isActive) m_ScrutationTimer->start();
}

//------------------------------------ startScrutation -------------------------------
void C_ScruteFTP::startScrutation()
{setScrutationInterval ((CApp::pCApp()->readUniqueParam("biogest session" , "Ftp scrutation").toInt()));
 connectFTP();
 scruteEtTraiteLesFichiersDistants();
 m_ScrutationTimer->start();
}

//------------------------------------ stopScrutation -------------------------------
void C_ScruteFTP::stopScrutation()
{m_ScrutationTimer->stop();
 CApp::pCApp()->outMessage(tr("================= stop FTP scrutation ================================"));
 closeFTP();
}

//--------------------------------------- eraseFtpDistantFile -----------------------------------------------
void C_ScruteFTP::eraseFtpDistantFile(const QString &distantFileName)
{m_vaitForAction = tr ("command : REMOVE : %1 <font color=#ff5400><b>%1</b></font>").arg(distantFileName);
 remove ( distantFileName );
 while (m_vaitForAction.length())  {qApp->processEvents(); }
}
//--------------------------------------- mustDistantsFileBeErased -------------------------------------------
bool C_ScruteFTP::mustDistantsFileBeErased()
{return m_eraseDistantsFiles;
}
//--------------------------------------- mustLocalsFileBeErased ---------------------------------------------
bool C_ScruteFTP::mustLocalsFileBeErased()
{return m_eraseLocalsFiles;
}
//--------------------------------------- setDistantsFileBeErasedState ---------------------------------------
void C_ScruteFTP::setDistantsFileBeErasedState(bool state)
{m_eraseDistantsFiles = state;
}
//--------------------------------------- setLocalsFileBeErasedState -----------------------------------------
void C_ScruteFTP::setLocalsFileBeErasedState(bool state)
{m_eraseLocalsFiles = state;
}
//--------------------------------------- scruteEtTraiteLesFichiersDistants ----------------------------------
void C_ScruteFTP::scruteEtTraiteLesFichiersDistants()
{ if (CApp::pCApp()->getDB()==0 )
     {CApp::pCApp()->outMessage(tr("°       <font color=#ff0000><b> C_ScruteFTP::scruteEtTraiteLesFichiersDistants() CApp::m_pC_BaseBioGest is not instancied, may be database server is unreachable. </b></font>."));    // <font color=#ff5400><b>%1</b></font>
      return;
     }
  //................. si travail précedent non fini on se casse .....................
  if (m_pC_ScruteFTP_Thread->isRunning())
     {CApp::pCApp()->outMessage(tr("m_pC_ScruteFTP_Thread is running"));
      return;
     }
  //................. recuperer la liste des fichiers distants ..................
  CApp::pCApp()->outMessage(tr("================= scruteEtTraiteLesFichiersDistants( interval = %1) ================================").arg(QString::number(m_ScrutationTimer->interval()/1000)));
  m_distantsFilesList.clear();
  m_vaitForAction = tr ("command : LIST");
  list ("");
  while (m_vaitForAction.length())  {qApp->processEvents(); }
  //................. traiter la liste des fichiers distants ..................
  if (m_distantsFilesList.size())
     {CApp::pCApp()->outMessage(tr(".     <font color=#00ccff><b>===> found %1 files to retrieve on Ftp server.</b></font>").arg(QString::number(m_distantsFilesList.size())));
      get_DistantsFiles();
      emit Sign_FilesAnalyseFinished();
     }
  else
     {CApp::pCApp()->outMessage(tr(".     ===> no more file available on Ftp server."));
     }
}

//--------------------------------------- connectToHost -------------------------------------------
void C_ScruteFTP::Slot_stateChanged(int state)
{switch(state)
       {
        case QFtp::Unconnected:		CApp::pCApp()->outMessage(tr("0-There is no connection to the host."));                              break;
        case QFtp::HostLookup:		CApp::pCApp()->outMessage(tr("1-A host name lookup is in progress."));                               break;
        case QFtp::Connecting:		CApp::pCApp()->outMessage(tr("2-An attempt to connect to the host is in progress."));                break;
        case QFtp::Connected:		CApp::pCApp()->outMessage(tr("3-Connection to the host has been achieved."));                        break;
        case QFtp::LoggedIn:		CApp::pCApp()->outMessage(tr("4-Connection and user login have been achieved."));                    break;
        case QFtp::Closing:		CApp::pCApp()->outMessage(tr("5-The connection is closing down, but it is not yet closed. "
                                                                     "(The state will be Unconnected when the connection is closed.)"));     break;
        default :                       CApp::pCApp()->outMessage(tr("%1-unknow message.)").arg(QString::number(state)));                    break;
      }

}
//--------------------------------------- Slot_commandStarted -------------------------------------------
void C_ScruteFTP::Slot_commandStarted ( int id  )
{CApp::pCApp()->outMessage(tr("commandStarted %1 ID = %2").arg(m_vaitForAction, QString::number(id)));
}

//--------------------------------------- Slot_commandFinished -------------------------------------------
void C_ScruteFTP::Slot_commandFinished( int id , bool error  )
{CApp::pCApp()->outMessage(tr("commandFinished %1 ID = %2 error: %3").arg(m_vaitForAction,QString::number(id), QString::number(error) ));
 if (error)
    {switch(QFtp::error())
           {case QFtp::NoError:           CApp::pCApp()->outMessage(tr("%1 - C_ScruteFTP::Slot_commandFinished() No error occurred.").arg(QString::number(error))); break;
            case QFtp::HostNotFound:      CApp::pCApp()->outMessage(tr("%1 - C_ScruteFTP::Slot_commandFinished() The host name lookup failed.").arg(QString::number(error))); break;
            case QFtp::ConnectionRefused: CApp::pCApp()->outMessage(tr("%1 - C_ScruteFTP::Slot_commandFinished() The server refused the connection.").arg(QString::number(error))); break;
            case QFtp::NotConnected:      CApp::pCApp()->outMessage(tr("%1 - C_ScruteFTP::Slot_commandFinished() Tried to send a command, but there is no connection to a server.").arg(QString::number(error))); break;
            case QFtp::UnknownError:      CApp::pCApp()->outMessage(tr("%1 - C_ScruteFTP::Slot_commandFinished() An error other than those specified above occurred.").arg(QString::number(error))); break;
            default :                     CApp::pCApp()->outMessage(tr("%1 - C_ScruteFTP::Slot_commandFinished() unknow error.)").arg(QString::number(error)));
           }
     emit Sign_FTP_status(QFtp::error());
    }
 else
    {emit Sign_FTP_status(QFtp::NoError);
    }
 m_vaitForAction = "";
}
//--------------------------------------- Slot_on_ScrutationTimerOut -------------------------------------------
void C_ScruteFTP::Slot_on_ScrutationTimerOut()
{scruteEtTraiteLesFichiersDistants();
}

//--------------------------------------- Slot_listInfo -------------------------------------------
void C_ScruteFTP::Slot_listInfo(const QUrlInfo &urlInfo)
{QString name = urlInfo.name();
 if (name!= ".."&&name!= ".") m_distantsFilesList.append(urlInfo.name());
}

//--------------------------------------- get_DistantsFiles -------------------------------------------
void C_ScruteFTP::get_DistantsFiles()
{
 if (m_pC_ScruteFTP_Thread==0) return;
 m_pC_ScruteFTP_Thread->run();
}

//--------------------------------------- download -------------------------------------------
QString C_ScruteFTP::download(const QString &fileFtpName)
{   QString localFileName = CApp::pCApp()->getPathTemp() + "/"+ (QDateTime::currentDateTime().toString("yyyy-dd-MM-hh-mm-ss-zzz_") + QString(fileFtpName));
    QFile   localFile (localFileName);
    if (!localFile.open(QIODevice::WriteOnly))
       {CApp::pCApp()->outMessage(tr("C_ScruteFTP::download() Unable to save the file <font color=#ff5400><b>%1</b></font> error: %2. ").arg(fileFtpName, localFile.errorString()));
        return QString::null;
       }
    m_vaitForAction = tr ("command : GET %1 to <font color=#ff5400><b>%2</b></font>").arg(fileFtpName, localFileName);
    get (fileFtpName, &localFile);
    while (m_vaitForAction.length())  {qApp->processEvents(); }
    //............... integration en base de donnee ---------------------------------------....
    return localFileName;
}

//======================================= C_ScruteFTP_Thread ============================================================
//--------------------------------------- C_ScruteFTP_Thread -------------------------------------------
C_ScruteFTP_Thread::C_ScruteFTP_Thread(C_ScruteFTP *pC_ScruteFTP)
{ m_pC_ScruteFTP = pC_ScruteFTP;
}

//--------------------------------------- run ----------------------------------------------------------
void C_ScruteFTP_Thread::run()
{   int ret               =  7777;
    QString localFileName = "";
    for (m_progress = 0; m_progress < m_pC_ScruteFTP->getDistantsFilesList().size(); ++m_progress)
        {QString distantFileName = m_pC_ScruteFTP->getDistantsFilesList().at(m_progress);
         CApp::pCApp()->outMessage(tr("C_ScruteFTP_Thread::run() getFile started <font color=#ff5400><b>%1</b></font>").arg(distantFileName));
         localFileName = m_pC_ScruteFTP->download(distantFileName);
         if ( localFileName.length() )
            {
             ret =  CApp::pCApp()->getDB()->analyseFile(localFileName);    // TODO traiter les retours (Pdf, FileCanNotOpen, Not_Hprim, SeparatorNotFound, Error , Useless , Analysed );
             if ( m_pC_ScruteFTP->mustDistantsFileBeErased() )
                { m_pC_ScruteFTP->eraseFtpDistantFile(distantFileName);
                }
             if ( m_pC_ScruteFTP->mustLocalsFileBeErased() )
                { QFile::remove(localFileName);
                  CApp::pCApp()->outMessage(tr(". --> Remove local file <font color=#ff5400><b>%1</b></font>").arg(localFileName));
                }
            }
         QString mess = "";
         switch (ret)
             {
               case C_BaseBioGest::Pdf:                mess = tr("-%1- pdf file ").arg(QString::number(ret));            break;
               case C_BaseBioGest::FileCanNotOpen:     mess = tr("-%1- FileCanNotOpen ").arg(QString::number(ret));      break;
               case C_BaseBioGest::Not_Hprim:          mess = tr("-%1- Not_Hprim ").arg(QString::number(ret));           break;
               case C_BaseBioGest::SeparatorNotFound:  mess = tr("-%1- SeparatorNotFound ").arg(QString::number(ret));   break;
               case C_BaseBioGest::Error:              mess = tr("-%1- Error ").arg(QString::number(ret));               break;
               case C_BaseBioGest::Useless:            mess = tr("-%1- Useless ").arg(QString::number(ret));             break;
               case C_BaseBioGest::Analysed:           mess = tr("-%1- Analysed ").arg(QString::number(ret));            break;
               default:                                mess = tr("-%1- unknow ").arg(QString::number(ret));              break;
             }
         CApp::pCApp()->outMessage(tr("C_ScruteFTP_Thread::run() thread getFile finished <font color=#ff5400><b>%1</b></font> code : %2 ").arg(distantFileName, mess));
        }
}
