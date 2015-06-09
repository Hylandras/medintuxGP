#ifndef C_APP_H
#define C_APP_H

#include <qobject.h>
#include <qstring.h>
#include "../../MedinTuxTools/CMedicaBase.h"
#include "../../MedinTuxTools/CW_EditText.h"
#include "../../MedinTuxTools/CSetupBase.h"
#include "../../MedinTuxTools/CMedicaBase.h"

#include "../../drtux/src/CMoteur_Base.h"

#include <qapplication.h>

#ifdef SESAMVITALE_VERSION
    #include "../../SesamVitale-QT3/C_Cps.h"
    #include "../../SesamVitale-QT3/C_Vitale.h"
#endif

static char CHANGEMENTS[]     = "==========================================================================================================<br>"
                                "Modif RS gest_user du 22-02-2013 Version 2.16.000<br>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>FONCTIONNALITES</u></b> :  - Intégration du RPPS <br/>"
                                "  Deux champs en plus dans la table <b>Personnes</b> : <b>NumRPPS</b> et <b>ClefRPPS</b> <br/>"
                                "- <b><u>FICHIERS</u></b> :<br/>"
                                "Modification pour integrer ces deux champs  :   <b>/Manager/bin/DataBase.cfg</b><br/><br/>"
                                "==========================================================================================================<br>"
                                "Modif RS gest_user du 14-06-2010 Version 2.14.000<br/>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>FONCTIONNALITES</u></b> :  - Intégration du module de gestion APropos<br/>"
                                "  de l'information A propos de la suite MedinTux<br/>"
                                "- <b><u>FICHIERS</u></b> :<br/>"
                                "<u>en plus</u> :   /Themes/Default/32x32/gest_user.png<br/>";


//================================================= CApp ================= =============================================
class CApp : public QApplication
{
   Q_OBJECT

 public:

     CApp(int & argc, char ** argv, QString &erreur, const char*user = 0, const char*password = 0);
    ~CApp();
    QString  AdjustGlobalDroits(const QString &droits);
    void     AddDroit( QString &listDroits, QString droitToadd);
    void     RemoveDroit( QString &listDroits, QString droitToRemove);
    bool     IsThisDroitExist(const char *listDroits, const char *droitToFind);
    QString  GetListDroitsPossibles(QStringList *plist_droits=0, int clearList = 1);
    bool     IsAdministrator();
    int      AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool passwordMustBeRecorded  = 1 );
    void     saveLastUserOn_Ini( bool passwordMustBeRecorded  = 1 );
    bool     IsAppStartWithUser();

public slots:
     //--------------------------------------------- quit -------------------------------------------------------------------
     /*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
     */
     void  quit();

 public:
    CMoteurBase         *m_pCMoteurBase;          // moteur de la base de donnée

//......................... contexte ......................................................

    QString              m_NUM_VERSION;          /*!< stockera le numéro de version */
    QString              m_PathAppli;
    QString              m_PathIni;
    QString              m_PathCfg_Base;
    QString              m_User;
    QString              m_CriptedPassWord;
    QString              m_Droits;
    bool                 m_Contacts_Run;
    bool                 m_IsAppStartWithUser;
    QString              m_PermsChoice;
    QString              m_PathTheme;
    QString              m_LastError;
//......................... carte PS .............................
#ifdef SESAMVITALE_VERSION
    C_Cps  *m_pCps;
#endif
signals:
     void  Sign_QuitterRequired();
};

extern CApp* G_pCApp;

#endif

