/********************************* C_ScruteFTP.h **************************
*  #include "CGestIni.h"         QT4-ready                                *
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

#ifndef C_SCRUTEFTP_H
#define C_SCRUTEFTP_H

#include <QObject>
#include <QFtp>
#include <QStringList>
#include <QThread>
#include <QPixmap>

class C_BaseSigEntrees;
class C_ScruteFTP_Thread;
class QTimer;
//================================== C_ScruteFTP ============================================================
/*! \class C_ScruteFTP
 *  \brief class to scann a distant FTP hprim files directory, analyse the files and if ok put the files in database .
 *  - this Class needs to know C_BaseSigEntrees *pC_BaseSigEntrees where are the database tools and hprim analyser.
 *  - this Class needs to know CApp::pCApp() to read the ftp connections parameters, and somes generals informations.
 *    first  you need to create a class instance, when this done, the distant FTP repertory is connected.
 *    second you need to call scruteEtTraiteLesFichiersDistants();  sometimes for longtimes... so works in a thread.
 *    this method loads the files from  distant directory, analyse hprim files and store data in database.
 */

class C_ScruteFTP : public  QFtp
{
    Q_OBJECT
public:
    //------------------------------------ C_ScruteFTP -------------------------------
    /*! \brief  constructor class
     *  \param  C_BaseSigEntrees *pC_BaseSigEntrees class for analyse hprim file and store data in database
     *  \return QStringList list of distants files names
     */
    explicit C_ScruteFTP(QObject *parent = 0 );
    ~C_ScruteFTP();
    //------------------------------------ connectFTP -------------------------------
    /*! \brief connect FTP from ini parameters
     */
    void connectFTP();
    //------------------------------------ closeFTP -------------------------------
    /*! \brief close ftp
     */
    void closeFTP();
    //------------------------------------ getDistantsFilesList -------------------------------
    /*! \brief loads the files list of distant directory
     *  \return QStringList list of distants files names
     */
    QStringList getDistantsFilesList(){return m_distantsFilesList;}
    //------------------------------------ getDistantsFilesList -------------------------------
    /*! \brief loads the files from  distant directory analyse  hprim files and store data in database, this
     */
    void        scruteEtTraiteLesFichiersDistants();
    //------------------------------------ getDistantsFilesList -------------------------------
    /*! \brief loads a file from  distant directory analyse  hprim file and store data in database, this
     */
    QString     download(const QString &file);
    //------------------------------------ eraseFtpDistantFile -------------------------------
    /*! \brief erase a distant file on ftp
     *  \param  const QString &localFileName distant file to erase
     */
    void eraseFtpDistantFile(const QString &distantFileName);
    //------------------------------------ mustDistantsFileBeErased -------------------------------
    /*! \brief test if distants files must bee erased after donwloaded
     */
    bool mustDistantsFileBeErased();
    //------------------------------------ mustLocalsFileBeErased -------------------------------
    /*! \brief test if locals files must bee erased after donwloaded
     */
    bool mustLocalsFileBeErased();
    //------------------------------------ setDistantsFileBeErasedState -------------------------------
    /*! \briefset if distants files must be to erase
     *  \param  bool state state to set
     */
    void setDistantsFileBeErasedState(bool state);
    //------------------------------------ setLocalsFileBeErasedState -------------------------------
    /*! \brief set if locals files must be to erase
     *  \param  bool state state to set
     */
    void setLocalsFileBeErasedState(bool state);
    //------------------------------------ setScrutationTime -------------------------------
    /*! \brief set if the interval between two scrutations
     *  \param  int time duration inseconds
     */
    void setScrutationInterval(int interval);
    //------------------------------------ startScrutation -------------------------------
    /*! \brief start ftp scrutations
     */
    void startScrutation();
    //------------------------------------ startScrutation -------------------------------
    /*! \brief stop ftp scrutations
     */
    void stopScrutation();

signals:
   void Sign_FilesAnalyseFinished();
   void Sign_FTP_status(int status);
public slots:
   void Slot_on_ScrutationTimerOut();
   void Slot_stateChanged(int state);
   void Slot_commandStarted ( int  );
   void Slot_commandFinished( int , bool  );
   void Slot_listInfo(const QUrlInfo& );

private:
   void                get_DistantsFiles();
   C_ScruteFTP_Thread *m_pC_ScruteFTP_Thread;
   QString             m_vaitForAction;
   QString             m_hostname;
   QString             m_port;
   QString             m_pass;
   QString             m_login;
   QString             m_distantPath;
   bool                m_eraseDistantsFiles;
   bool                m_eraseLocalsFiles;
   bool                m_FTPmustBeReconnected;
   QStringList         m_distantsFilesList;
   QTimer             *m_ScrutationTimer;
};

//================================== C_ScruteFTP_Thread ============================================================
/*! \class C_ScruteFTP_Thread
 *  \brief class to put in thread longtimes procedures of C_ScruteFTP class.
 */

class C_ScruteFTP_Thread : public QThread
{
public:
    C_ScruteFTP_Thread(C_ScruteFTP *pC_ScruteFTP);
    void run();
    int getProgress (){return m_progress;}
private :
    C_ScruteFTP *m_pC_ScruteFTP;
    int          m_progress;
};

#endif // C_SCRUTEFTP_H
