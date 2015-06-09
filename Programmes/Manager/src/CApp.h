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


#include "../../MedinTuxTools-QT4/Agenda/CMoteurAgenda.h"
#include "../../MedinTuxTools-QT4/CMoteur_Base.h"
#include "../../MedinTuxTools-QT4/CCoolPopup.h"     // Gestion des popups surgissants
#include "../../MedinTuxTools-QT4/C_AppCore.h"

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
#else
   #define F_EXE ""
#endif

#define TR QObject::tr

#define LIST_NOM           0
#define LIST_PRENOM        1
#define LIST_PK_PATIENT    2
#define LIST_GUID          3

#define POS_QUAL_INTERV 3
#define POS_ID_INTERV   4


#ifdef SESAM_VERSION
    #include "../../SesamVitale-QT4/C_Cps.h"
    #include "../../SesamVitale-QT4/C_Vitale.h"
#endif
#ifdef ENTREES_SIGEMS
    #include "../../SigEntrees/src/C_DSigemsVar.h"
#endif

class CCoolPopup;

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    so all publics functions will be accessibles by this pointer.
 */

class CApp : public C_AppCore
{
   Q_OBJECT
 public:
   enum exeMode {
        endWait,
        noWait
    };
     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();
     //..................... utilisateur ........................................
     void    saveLastUserOn_Ini( bool passwordMustBeRecorded );
     void    saveLastUserOn_Ini( const QString &user, const QString &signUser, const QString &password, bool passwordMustBeRecorded  = 1 );
     int     AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool passwordMustBeRecorded  = 1 );
     bool    IsAppStartWithUser();
     QString getUser(){return m_User;}
     QString getSignUser(){return m_SignUser;}
     void    changeAllModuleConnectionParam(const QString &driverName,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                            const QString &baseToConnect,     // nom de la base: si QODBC3 -> nom de la source de donn�es (userDSN)
                                            const QString &userName,          // = "root"
                                            const QString &passWord,          // = ""
                                            const QString &hostName,          // = "localhost"
                                            const QString &port               // = "port",
                                           );
     void changeAllModuleConnectionParamMessage(const QString &place, const QString &path);
     /*! \brief retourne le contexte de la configuration de demarrage */
     QString getConfigContext();
     /*! \brief retourne le numero de version */
     QString getNumVers();
     int isSigemsValid();
     //...................... identite patient ....................................
     QString identitePatientNom()   {return m_LastNom;}
     QString identitePatientPrenom(){return m_LastPrenom;}
     QString identitePatientGUID()  {return m_LastGUID;}
     QString identitePatientPk()    {return m_LastPK;}
     void    setCurrentIdentite(QTreeWidgetItem *pQListViewItem);
     void    clearCurrentIdentite(){m_LastNom="";m_LastPrenom="";m_LastGUID="";m_LastPK="";}
     bool    isCurrentIdentiteValid(){return m_LastGUID!="";}
     bool    isCurrentIdentiteSame(QTreeWidgetItem *pQListViewItem);
     //...................... gestion des droits ....................................
     /*! \brief retourne la liste de droits */
     QString getDroits(){return m_Droits;}
     bool IsThisDroitExistForThisUser( const QString &signUser, const char *droitToFind);
     bool IsThisDroitExist(const char *droitToFind){return IsThisDroitExist(m_Droits.toAscii(), droitToFind);}
     bool IsThisDroitExist(const char *listDroits, QString droitToFind_in);
     void lireDroitsUtilisateurs();

     QString        GetMySqlPass();
     static QString DoPopupList(QStringList &list, const QString& sep = "|", int posToHide = 0);

     /*! \brief Affiche un popup amusant */
     void  CouCou(const QString &message, const QString &imagePath="", int tempo = 0);
     void  CouCou(const QString &message, const QColor &color, const QFont &ft , int tempo = 0, const QString &imagePath="");
     void  launchSpecificJob(QString nameOfJob); // CZB
     QString  execCalendrier(const QDate &dateIn);
     QString  PluginExe(        QObject         * pQObject,
                                const char      * pluginScript,
                                int               waitFlag  = CApp::endWait);
     QString  PluginExe(        QObject         *pQObject,
                                const QString    &pathPlugin,  // Chemin de l'executable plugin �  actionner sans l'extension .exe
                                const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                const QString    &maskExch,    // texte du fichier d'exchange
                                const QString    &guid,
                                const QString    &dosPk,
                                const QString    &terPk,
                                const QString    &ordPk ,
                                QStringList      &param,
                                int               waitFlag  = CApp::endWait );
public:
QList<QWidget*>      m_widgetList;
QFont                m_GuiFont;
QString              m_PathSigemsPA;     /*!< chemin de l'interface Sigem (si existe)    */
//.................. identite en cours ............................................
QString              m_LastNom;          /*!< nom de l'identite en cours    */
QString              m_LastPrenom;       /*!< prenom de l'identite en cours */
QString              m_LastGUID;         /*!< GUID de l'identite en cours   */
QString              m_LastPK;           /*!< Pk de l'identite en cours     */
//................... les objets suivants sont relatifs au contexte de l'application..................................
QString              m_LastError;       /*!< dernière erreur rencontrée */
QString              m_MySql_Pass;      /*!< mot de passe de connexion mysql */
QString              m_PluginRun;       /*!<indique si un process bloquant est en cours */
//................... les objets suivants sont relatifs aux utilisateurs..............................................
QString              m_CriptedPassWord;     /*!< mot de passe crypté de l'utilisateur MedinTux */
QString              m_SignUser;            /*!< login l'utilisateur signataire MedinTux */
QString              m_User;                /*!< login l'utilisateur en cours */
QString              m_Droits;              /*!< droits l'utilisateur en cours */
bool                 m_IsAppStartWithUser;  /*!< si TRUE l'application a demarré avec un utilisateur predefini */
QString              m_NUM_VERSION;         /*!< num?ro de version du programme */
//................... les objets suivants sont relatifs aux moteurs de bases...........................................
CMoteurAgenda       *m_pCMoteurAgenda;      /*!< pointeur sur le gestionnaire de base de donnée de l'agenda */
CMoteurBase         *m_pCMoteurBase;        /*!< pointeur sur le gestionnaire de base de donnée de drtux */
int                  m_IsGestionNomadisme;  /*!< variable stockant si la gestion du nomadisme est active*/
int                  m_IsNomadeActif;       /*!< variable stockant l'état du nomadisme */
//........................ le message popup .................................
CCoolPopup          *m_pCCoolPopup;              /*!< Pour afficher un message en popup surgissant en bas �  droite */
int                  m_CCoolPopupTempo;          /*!< temps de persistance du menu �  l'écran en microsecondes */
QSplashScreen       *m_pQSplashScreen;           /*!< splash screen */
//......................... carte PS .............................
#ifdef SESAM_VERSION
  C_Cps     *m_pCps;              /*!< contient les donnees de la carte cps */
  C_Vitale  *m_pVitale;           /*!< contient les donnees de la carte sv */
#endif
#ifdef ENTREES_SIGEMS
  C_DSigemsVar *m_pC_DSigemsVar;  /*!< contient les noms des champs des tables sigems */
#endif

public slots:
     void  Slot_OnSplash_Clicked(QMouseEvent *, int &);
     void  CouCouUp(   CCoolPopup * pCCoolPopup);
     void  CouCouStop( CCoolPopup * pCCoolPopup);
     void  CouCouDestroy();
     void  Slot_error ( QProcess::ProcessError err );
     virtual void quit();
 signals:
     void Sign_QuitterRequired();
};

extern CApp* G_pCApp;                                 /*!< pointeur global sur l'application */

#endif

