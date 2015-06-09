#ifndef C_BDM_CONTROL_H
#define C_BDM_CONTROL_H
/**************************** C_BDM_Control.h *****************************
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

#include <QObject>
#include <QThread>
#include <QMutex>

#include "../../MedinTuxTools-QT4/medicabase/C_BDM_TheriaquePlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_DatasempPlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_GouvPlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_GenericPlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"

//=================== C_BDM_Control ===================================
/*! \class C_Gest_BDM_Pool
 *  \brief Class to maintain all the DB element for API
 */
class C_BDM_Control : public QObject
{
    Q_OBJECT
public:
    //-------------------------------- C_BDM_Control ------------------------------------------------------
    /*! \brief  the constructor intialize BDM structures (each thread must have separate QSqlDatabase)
     *  \param  const QString &pathAppli     general application path
     *  \param  const QString &pathIni       general configuration file application path
     *  \param  const QString &pathTheme     icones and pixmap folder
     *  \param  QTextEdit       *pLogWidget  pointer QTextEdit log widget (can be set to zero).
     *  \param  QObject *parent              parent object
     */
    explicit C_BDM_Control(const QString &pathAppli       ,
                           const QString &pathIni         ,
                           const QString &pathTheme       ,
                           QTextEdit     *logWidget       ,
                           QObject *parent);

     ~C_BDM_Control();
public:
    //-------------------------------- setPatientAndUserContexte ------------------------------------------------------
    /*! \brief return a pointer on api
      */
   C_BDM_Api  *BDM_Api()      const {return m_pC_BDM_Api;}
   //-------------------------------- setPatientAndUserContexte ------------------------------------------------------
   /*! \brief set user and patient context
    *  \param  C_PatientCtx *pC_PatientCtx pointer on patient context (must be persistant while C_BDM_Control exists)
    *  \param  C_UserCtx *pC_UserCtx       pointer on user context (must be persistant while C_BDM_Control exists)
    */
   void setPatientAndUserContexte(C_PatientCtx *pC_PatientCtx, C_UserCtx *pC_UserCtx);
   //-------------------------------- set_DBM_Plugin ------------------------------------------------------
   /*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
    *  \param QString namePlugin of C_BDM_InitPluginI data source ( 'theriaque' 'datasemp')
    *  \param int oldMustBeDeleted
    *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
    *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
    *  \return C_BDM_PluginI * pointer on plugin or zero if error
    */
   C_BDM_PluginI *set_BDM_Plugin(const QString &_namePlugin, int oldMustBeDeleted /* = 0 */);

protected:
   QString m_pathAppli;
   QString m_pathIni;
   QString m_dataIni;
   QString m_pathTheme;
   QString m_lastMessage;

   C_BDM_Api                     *m_pC_BDM_Api;
   QMap <QString, C_BDM_PluginI*> m_pC_BDM_PluginI_Map;

signals:

public slots:

};

//============================= InteractionsThread ==============================================
//-------------------------------- C_InteractionsThread ------------------------------------------------------
/*! \brief  the constructor intialize BDM structures (each thread must have separate QSqlDatabase)
 *  \param  const QString &pathAppli     general application path
 *  \param  const QString &pathIni       general configuration file application path
 *  \param  const QString &pathTheme     icones and pixmap folder
 *  \param  QObject *parent              parent object
 */
class C_InteractionsThread : public QThread
{
public:

    C_InteractionsThread(  const QString &pathAppli         ,
                           const QString &pathIni           ,
                           const QString &pathTheme         ,
                           C_PatientCtx *pC_PatientCtx      ,
                           C_UserCtx    *pC_UserCtx         ,
                           QTextEdit     *logWidget         ,
                           QObject *parent
                         );
    ~C_InteractionsThread();
    //-------------------------------- set_Thread_BDM_Plugin ------------------------------------------------------
    /*! \brief set user BDM plugin interface (class interface for unified commercial database).
     *  \param  const QString &_namePlugin name of plugin (Datasemp, Theriaque ...)
     *  \param int oldMustBeDeleted
     *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
     *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
     */
    void set_Thread_BDM_Plugin(const QString &_namePlugin, int oldMustBeDeleted=0);
    //-------------------------------- setContext ------------------------------------------------------
    /*! \brief set user and patient context.    This methode must tbe called before start thread to update context.
     *  \param  C_PatientCtx *pC_PatientCtx     pointer on patient context (must be persistant while C_BDM_Control exists)
     *  \param  C_UserCtx *pC_UserCtx           pointer on user context (must be persistant while C_BDM_Control exists)
     *  \param  const QStringList &grammarList  products list
     */
    void setContext(C_PatientCtx *pC_PatientCtx, C_UserCtx *pC_UserCtx, const QStringList &grammarList);

    //------------------------------------ html_result --------------------------------------------------------------------
    /*! \brief   return  interaction result html
     *  \return  QString interaction result html
     */
    QString html_result();
private:
    //------------------------------------ run --------------------------------------------------------------------
    /*! \brief main methode called  when thread run
     */
    void run();

    QString             m_HtmlResult;
    C_BDM_Control      *m_pC_BDM_Control;
    C_PatientCtx       *m_pC_PatientCtx;
    C_UserCtx          *m_pC_UserCtx;
    QStringList         m_grammarList;
    QMutex              m_mutex;                         /*!< pour la synchro du thread interraction */

};


#endif // C_BDM_CONTROL_H
