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
#include <QtGui/QApplication>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QTextEdit>

#include "../../MedinTuxTools-QT4/C_AppCore.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"

#include "C_BaseSigEntrees.h"
#include "C_Dlg_SigEntrees.h"

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    if fatal error in class initialisation occured  CApp::pCApp()==0
 *    so all publics functions will be accessibles by CApp::pCApp()->publicFonctions().
 */
class CCoolPopup;
class QComboBox;
class CApp : public C_AppCore
{
   Q_OBJECT
public:
     enum exeMode { endWait,  noWait };
     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();

    static CApp         *pCApp();                                  /*!< retourne l'instance de l'application          */
    //------------------------------------ outMessage -------------------------------
    /*! \brief output a message on std error out put and logwidget if defined
     *  \param mess  const QString  message to display
     *  \param file  const QString source file from where the message is display
     *  \param line  const QString source line in file from where the message is display
     *  \return message diplayed
     */
    void                 outMessage( const QString &mess, const QString &file   = ""  , const int line   = -1 );
    //------------------------------------ getProfilsList -------------------------------
    /*! \brief get list variables in section parameters
     *  \param const QString &section    section where to find list of variables
     *  \param lconst QString &varToFind   variable name of each list elements
     *  \return variables list
     */
    //QStringList getProfilsList(const QString &varToFind, const QString &section  = "SigEntreesbiogest session" );
    QStringList getProfilsList(const QString &varToFind, const QString &section  = "sigentrees session" );

    //------------------------------------ replaceComboboxListInParam -------------------------------
    /*! \brief replace list variables in section parameters by datas from combobox list
     *  \param QComboBox *pQComboBox  combobox from which list must be replace
     *  \param const QString &section section where to find list of variables
     *  \param lconst QString &variable   variable name of each list elements
     */
    void replaceComboboxListInParam(QComboBox *pQComboBox, const QString &section, const QString &variable);
    //------------------------------------ createDb -------------------------------
    /*! \brief cree et instancie la base de donnees C_BaseSigEntrees *m_pC_BaseSigEntrees
     */
    //bool createDb();
    bool controlDb();
    //--------------------------------- DoPopupList -----------------------------------------------------
    /*! \brief cree et active un popup menu apartir d'une liste d'items
     *  \param list : QStringList qui contient tous les item si icone associee du theme commence par #iconefile#reste du texte
     *  \return une QString qui contient le nom de l'item selectionne.
    */
    QString DoPopupList(QStringList &list, const QString& sep = "|", int posToHide  = 0 );

    //------------------------------------ connectDb -------------------------------
    /*! \brief reconnecte la base de donnees
     */
    bool connectDb();
    //------------------------------------ getDB -------------------------------
    /*! \brief retourne une instance du gestionnaire de base de donn�e
     *  \return pointer on C_BaseSigEntrees class
     */
    C_BaseSigEntrees* getDB();
    //------------------------------------ u_Html -------------------------------
    /*! \brief return a pointer on global instance of C_Utils_Html
    *  \return pointer on C_Utils_Html class
    */
    C_Utils_Html *u_Html();
    //------------------------------------ initUtilsHtml -------------------------------
    /*! \brief create the global instance on C_Utils_Html
    *  \param QObject *parent parent object
    *  \param QNetworkAccessManager *pQNetworkAccessManager  QNetworkAccessManager to manage web functions
    *  \param QTextEdit *pQTextEditLog  QTextEdit window for log message
    */
    void initUtilsHtml(QObject *parent , QNetworkAccessManager *pQNetworkAccessManager , QTextEdit *pQTextEditLog );
    //------------------------------------ isFtpDaemonEnabled -------------------------------
    /*! \brief retourne si le demon ftp doit etre active en lisant le fichier de configuration
    */
    bool isFtpDaemonEnabled();
    //------------------------------------ setFtpDaemonEnabled -------------------------------
    /*! \brief place l'etat actif ou inactif du demon ftp
    */
     void setFtpDaemonEnabled(int state=true);
     //------------------------------------ isTraceEnabled -------------------------------
     /*! \brief retourne si la fenetre de log doit etre active en lisant le fichier de configuration
     */
     bool isTraceEnabled();
     //------------------------------------ setPathTemp -------------------------------
     /*! \brief positionne le chemin du repertoire temporaire
     *  \param const QString &pathTemp valeur que prendra le chemin du repertoire temporaire
     */
     void setPathTemp(const QString &pathTemp);
     //------------------------------------ justPatientsWithResults -------------------------------
     /*! \brief retourne si les patients sans resultats doivent �tre affiches
      */
     bool justPatientsWithResults();
     //------------------------------------ getResultsLimit -------------------------------
     /*! \brief retourne le nombre limite de resultats a afficher
      */
     QString getResultsLimit();
     //-------------------------- setJustPatientsWithResults -------------------------------------------
     /*! \brief place l'etat actif ou inactif si les patients sans resultats doivent �tre affiches
      */
     void setJustPatientsWithResults(int state=true );
     //------------------------------------ getPathTemp -------------------------------
     /*! \brief retourne le chemin du repertoire temporaire
      *  \return QString  le chemin du repertoire temporaire
      */
     QString getPathTemp();
     //------------------------------------ isDateFilterEnabled -------------------------------
     /*! \brief retourne si le filtre des dates est actif en lisant le fichier de configuration
      */
     bool isDateFilterEnabled();
     //-------------------------- setDateFilterEnabled -------------------------------------------
     /*! \brief place l'etat actif ou inactif du filtre de dates
      */
     void setDateFilterEnabled(int state =true );
     //-------------------------- setdefaultService -------------------------------------------
     /*! \brief retourne le service par defaut
      */
     QString  getdefaultService();
     //-------------------------- setdefaultService -------------------------------------------
     /*! \brief place le service par defaut
      */
     void setdefaultService(const QString &serv);
     QString  getdefaultFtp();

     //-------------------------- setdefaultService -------------------------------------------
     /*! \brief place les parametres TC IP par defaut
      */
     void    setdefaultDB_Connexion(const QString &param);
     QString getdefaultDB_Connexion();

     //----------------------------------- CouCouKill ---------------------------------------------------
     /*! \brief Detruit le popup amusant */
     void  CouCouKill();

     //----------------------------------- CouCou ---------------------------------------------------
     /*! \brief Affiche un popup amusant */
     void  CouCou(const QString &message,
                  const QString &imagePath="",
                  int tempo = 0);

     //----------------------------------- CouCou ---------------------------------------------------
     /*! \brief Affiche un popup amusant */
     void  CouCou(const QString &message,
                  const QColor  &color,
                  const QFont  &ft ,
                  int   tempo = 0,
                  const QString &imagePath="");

     //----------------------------------- execCalendrier ---------------------------------------------------
     /*! \brief very bad olds methodes for external calendar call
      *  \return QString date choice in calendar in french: 'choix dans la date et non doigt dans la ...'
      */
     QString execCalendrier(         const QDate &dateIn);
     QString PluginExe(              QObject         * pQObject,
                                     const char      * pluginScript,
                                     int               /*waitFlag*/);
     QString PluginExe(              QObject         */*pQObject*/,
                                     const QString    &pathPlugin,  // Chemin de l'executable plugin ?  actionner sans l'extension .exe
                                     const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                     const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                     const QString    &maskExch,    // texte du fichier d'exchange
                                     const QString    &guid,
                                     const QString    &dosPk,
                                     const QString    &terPk,
                                     const QString    &ordPk ,
                                     QStringList      &param,
                                     int               waitFlag );

//-------------------- SIG
     Ui::MainWindow *G_p_Dlg_SigEntrees;
//--------------------


private:
    //................... les objets suivants sont relatifs au contexte de l'application..............................................
    QString           m_HprimTempl;              /*!< HPRIM template in use to present and select HPRIM datas to display*/
    C_BaseSigEntrees *m_pC_BaseSigEntrees;
    C_Utils_Html     *m_pC_Utils_Html;
    QString           m_pathTemplate;
    QString           m_pathTemp;
    QString           m_PluginRun;
    CCoolPopup       *m_pCCoolPopup;
    int               m_CCoolPopupTempo;          /*!< temps de persistance du menu ?  l'?�cran en microsecondes */

private slots:
    void Slot_error ( QProcess::ProcessError err );
    void Slot_CouCouUp(   CCoolPopup * pCCoolPopup);
    void Slot_CouCouStop( CCoolPopup * pCCoolPopup);
    void Slot_CouCouDestroy();

signals:
    void   Sign_CouCouDestroy();

};

#endif

