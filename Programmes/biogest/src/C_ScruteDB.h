/********************************* C_ScruteDB.h ***************************
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

#ifndef C_SCRUTEDB_H
#define C_SCRUTEDB_H

#include <QObject>
#include <QMap>
#include <QThread>
#include <QPixmap>
#include "C_BaseBioGest.h"

//========================================= C_ScruteDB ========================================
/*! \class C_ScruteDB
 *  \brief class to scann a hprim database and maintains a CPatient list.
 *  - this Class needs to know C_BaseBioGest *pC_BaseBioGest where are the database tools and hprim analyser.
 *  - this Class needs to know C_Dlg_BioGest *pC_Dlg_BioGest where are the main dialog class (needs for somes dates and anothers contexts data).
 *  - this Class needs to know CApp::pCApp() to read the ftp connections parameters.
 *    first  you need to create a class instance, when this done, the distant FTP repertory is connected.
 *    second you need to call scruteEtTraiteLesFichiersDistants();  sometimes for longtimes... so works in a thread.
 *    this method loads the files from  distant directory, analyse hprim files and store data in database.
 */
class C_Dlg_BioGest;
class C_ScruteDB_Thread;
class QTimer;
class C_ScruteDB : public QObject
{
    Q_OBJECT
public:
    explicit C_ScruteDB(C_Dlg_BioGest *pC_Dlg_BioGest, QObject *parent = 0);
    ~C_ScruteDB();
    void     scruteEtTraiteLaBaseDeDonnee(int clearQTreeWidget =  0 );
    void     setScrutationInterval(int interval);
    void     startScrutation();
    void     stopScrutation();
    void     traiteLaBaseDeDonnee();
    void     setService(const  QString &pk, const QString &service);
    QPixmap             m_IsAlreadyView;
    QPixmap             m_newPatient;
    QPixmap             m_resultsChanged;
signals:
    void     Sign_DB_AnalyseFinished(const QString &message);

public slots:
     void Slot_on_ScrutationTimerOut();

private:

    QMap<QString, CPatient> m_PatientPk_CPatient_Map;
    C_Dlg_BioGest          *m_pC_Dlg_BioGest;
    C_ScruteDB_Thread      *m_pC_ScruteDB_Thread;
    QTimer                 *m_ScrutationTimer;

};
//================================== C_ScruteDB_Thread ============================================================
/*! \class C_ScruteDB_Thread
 *  \brief class to put in thread longtimes procedures of C_ScruteDB class.
 */

class C_ScruteDB_Thread : public QThread
{
public:
    C_ScruteDB_Thread(C_ScruteDB *pC_ScruteDB);
    void run();
    int getProgress (){return m_progress;}

    int          m_clearQTreeWidget;
private :
    C_ScruteDB  *m_pC_ScruteDB;
    int          m_progress;
};

#endif // C_SCRUTEDB_H
