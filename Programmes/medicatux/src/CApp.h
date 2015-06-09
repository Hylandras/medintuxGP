/********************************* CApp.h **********************************
 *  #include "CApp.h"                                                      *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright : (C) 2005-2006-2007-2008-2009-2010 and for the eternity    *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    : roland-sevin@medintux.org                                 *
 *   Web site  : www.medintux.org                                          *
 * ........................................................................*
 *                                                                         *
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
#ifndef C_APP_H
#define C_APP_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QComboBox>
#include <QSharedMemory>
#include <QTimer>
#include <QMainWindow>

// #include "../../MedinTuxTools-QT4/medicabase/C_BaseMedica.h"
// #include "../../MedinTuxTools-QT4/C_BaseDatasemp/C_BaseDatasemp.h"
#include "../../MedinTuxTools-QT4/C_AppCore.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"
#include "C_Macro.h"

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    if fatal error in class initialisation occured  CApp::pCApp()==0
 *    so all publics functions will be accessibles by CApp::pCApp()->publicFonctions().
 */


class CApp : public C_AppCore , public C_Macro
{
   Q_OBJECT
public:
     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();

    static CApp         *pCApp();                                  /*!< retourne l'instance de l'application          */
    QString              loadContentsIfBeginByTokenFile(QString &arg);
    QString              resolvePath(QString pathFile, const QString &pathGlossaire  = "");
    QString              resolvePathKeyWord(QString pathFile, const QString &pathGlossaire  = "" );
    bool                 isEndPageDisplay()         {return m_EndPageDisplay;}
    QString              patientCtxFileName()       {return m_PatientCtxFileName;}
    QString              userCtxFileName()          {return m_UserCtxFileName;}
    QString              grammarFileName()          {return m_GrammarFileName;}
    long                 waitForEndPageDisplay(long timeOut, QWebView *pQWebView=0);
    void                 setEndPageDisplay(bool state){m_EndPageDisplay = state;}
    //--------------------------------------------- return_mode -------------------------------------------------------------------
    /*! \brief retourne le mode de sortie desire
    */
    QString return_mode();
    //------------------------ getDocument --------------------------------------------------
    /*! \brief get current document html (used by Macro)
     */
    QString *getDocument(){return 0;}

    //------------------------------------ replaceComboboxListInParam -------------------------------
    /*! \brief replace list variables in section parameters by datas from combobox list
     *  \param QComboBox *pQComboBox  combobox from which list must be replace
     *  \param const QString &section section where to find list of variables
     *  \param lconst QString &variable   variable name of each list elements
     */
    void replaceComboboxListInParam(QComboBox *pQComboBox, const QString &section, const QString &variable);

    //------------------------------------ outMessage -------------------------------
    /*! \brief output a message on std error out put and logwidget if defined
     *  \param mess  const QString  message to display
     *  \param file  const QString source file from where the message is display
     *  \param line  const QString source line in file from where the message is display
     *  \return message diplayed
     */
    void                 outMessage( const QString &mess, const QString &file   = ""  , const int line   = -1 );
    //--------------------------------- DoPopupList -----------------------------------------------------
    /*! \brief cree et active un popup menu apartir d'une liste d'items
     *  \param list : QStringList qui contient tous les item si icone associee du theme commence par #iconefile#reste du texte
     *  \return une QString qui contient le nom de l'item selectionne.
    */
    QString              DoPopupList(QStringList &list, const QString& sep = "|", int posToHide  = 0 );

    //------------------------ declareMainWindow --------------------------------------------------
    /*! \brief filter to control message and make a pseudo modal state
     */
    void declareMainWindow( QMainWindow *pQMainWindow){m_pQMainWindow = pQMainWindow;}


    QMap <QString, C_BDM_PluginI*> m_pC_BDM_PluginI_Map;
    C_UserCtx                     *m_pC_UserCtx;
    C_PatientCtx                  *m_pC_PatientCtx;
    QString                        m_lastArg;
private:


    //................... les objets suivants sont relatifs au contexte de l'application..............................................
    bool                           m_EndPageDisplay;
    QString                        m_PatientCtxFileName;
    QString                        m_UserCtxFileName;
    QString                        m_GrammarFileName;
    QString                        m_mode;                                 /*!< is set in pCApp destructor (wen                                                      */
    QSharedMemory                 *m_pQSharedMemory;
    QTimer                        *m_pQSharedMemoryCheckTimer;
    QMainWindow                   *m_pQMainWindow;

private slots:
    //------------------------------------- Slot_QSharedMemoryCheckTimer ------------------------------------
    /*! \brief this slot must be called to check shared memory contents
     */
    void Slot_QSharedMemoryCheckTimer();
};

#endif

