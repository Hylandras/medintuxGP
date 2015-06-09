#ifndef C_APP_H
#define C_APP_H

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
#else
   #define F_EXE ""
#endif

#define PROG_NAME "med_stat"

#include "../../drtux/src/CMoteur_Base.h"
#include "../../MedinTuxTools/CMedicaBase.h"

#ifdef SESAMVITALE_VERSION
    #include "../../SesamVitale-QT3/C_MUI_App.h"
    #include "../../SesamVitale-QT3/C_Cps.h"
    #include "../../SesamVitale-QT3/C_Vitale.h"
#else
    #include "../../MedinTuxTools/C_MUI_App.h"
#endif

class med_stat;
class QAction;
class QStringList;
//================================================= CApp ==============================================================
class CApp : public C_MUI_App
{

   Q_OBJECT

 public:

     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();
     void saveLastUserOn_Ini(const QString &user, const QString &signUser, const QString &password, bool passwordMustBeRecorded  = 1 );
     int AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool passwordMustBeRecorded  = 1 );
     bool IsAppStartWithUser();
     void PositionneBonDroits();
     bool IsThisDroitExist(const char *listDroits, const char *droitToFind);
     static QStringList get_CustomRubNameList();
     static QString convertToIni(const QString &text);
     static QString convertToText(const QString &text);

public:


static QString       m_PathAppli;                // chemin de l'Applicatif
//............................ pointeurs sur les objets principaux generaux Uniques ...................................................
//                             l'interdependance de ces differentes classes rend
//                             plus simple de les rendre accesible par un pointeur global
//                             Initialisé lors de la creation de ces classes qui doivent être
//                             instanciées qu'une seule fois pendant toute l'application
//                             la remise à zero se fait lors de la destruction de la classe.

//.................. les deux  objets suivants sont INDISPENSABLES à l'application ....................................................
//                   et seule une instance de ces objets doit exister par application
QString              m_NUM_VERSION;
CMoteurBase         *m_pCMoteurBase;                // classe d'outils de gestion des données en base de donnée
CMedicaBase         *m_pCMedicaBase;                // moteur de la base de donnée medicaments
med_stat            *m_pMed_stat;                   // classe widget principal et parentes de toutes != 0 si constructeur OK
//................... les objets suivants sont crees à la demande dans l'application..............................................
//                    mais seule une instance de ces objets peut exister par application
QString              m_CriptedPassWord;
QString              m_SignUser;
QString              m_User;
//................. pour faire jonction avec agenda .........................................
QString              m_LastNom;
QString              m_LastPrenom;
QString              m_LastGUID;
QString              m_LastPK;

QString              m_LastTel;
QString              m_LastType;
QString              m_PathIni;
QString              m_LocalParam;
QString              m_LastError;
QString              m_Droits;
int                  m_LastDuree;
bool                 m_IsAppStartWithUser;
QString              m_PathTheme;
//.......... parametres d'exportation ....................
QString              m_ExportFileName;
QString              m_ExportDebLine;
QString              m_ExportEndLine;
QString              m_ExportDebField;
QString              m_ExportEndField;
QString              m_nb_max_results;
QAction             *m_OptionExportRun;

bool                 m_DenombreEnabled;
bool                 m_ResultEnabled;
//......................... carte PS .............................
#ifdef SESAMVITALE_VERSION
  C_Cps  *m_pCps;
#endif
public slots:
     //--------------------------------------------- quit -------------------------------------------------------------------
     /*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
     */
     void  quit();

signals:
 void  Sign_QuitterRequired();
};

extern CApp *G_pCApp ;

#endif

