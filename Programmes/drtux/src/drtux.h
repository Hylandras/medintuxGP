/************************************** drtux.h ***********************************
 *                                                                                *
 *   #include "drtux.h"                                                           *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/


#ifndef DRTUX_H
#define DRTUX_H

#include <qmainwindow.h>
#include <qobject.h>
#include <qprocess.h>
#include <qtextedit.h>
#include <qworkspace.h>
#include <qaction.h>
#include <qpushbutton.h>
#include <qsizegrip.h>
#include <qtimer.h>
#include <qtoolbox.h>
#include <qptrlist.h>

#include "ui/FormConfig.h"
#include "ui/Dock_TabRubrique.h"
#include "ui/Dock_Menu.h"
#include "ui/FormGlossaire.h"         // vient de  Glossaire.ui

#include "CMoteur_Base.h"
#include "C_FormGlossaire.h"
#include "FormOngletRubrique.h"

#include "CRubList.h"
#include "CDevilCrucible.h"
#include "../../MedinTuxTools/ThemePopup.h"

class CMDI_Ident;
class CMDI_Prescription;
class CMDI_Terrain;
class CMDI_Observation;
class CMDI_NewObservation;
class CMDI_ChoixPatient;
class CMDI_Documents;
class CMDI_GroupedView;
class MyEditText;
class C_Organiseur;
class C_Dlg_GestionATCD;
//====================================== C_UserQAction =======================================================
class C_UserQAction : public QAction
{

    Q_OBJECT

 public :
 C_UserQAction ( const QString & text, const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject * parent, const char * name = 0, bool toggle = FALSE )
     : QAction( text,  icon,  menuText,  accel,  parent,  name, toggle  )
     {m_Period  = 0;
      m_Script  = "";
      m_pQTimer = 0;
      connectMenu();
     }
   void    prependToScript(const QString& mention){m_Script = m_Script.prepend(mention);}
   void    setScript(const QString &script){m_Script=script;};
   QString getScript(){return m_Script; };
   void    setTimer(const int &period)
           {m_Period  = period;
            if (m_pQTimer)
               {delete m_pQTimer;
                m_pQTimer=0;
               }
            if (m_Period>0)
               {m_pQTimer = new QTimer( this, QString("TimerUserAction_")+name());
                if (m_pQTimer) connect( m_pQTimer, SIGNAL(timeout()), this, SLOT(Slot_UserQActionOn_TimerDone()) );
               }
           };
    void    startTimer()
           {if (m_Period<0)
               {QTimer::singleShot ( QMAX(100, -m_Period), this, SLOT(Slot_UserQActionOn_TimerDone()) );
               }
            else if (m_pQTimer && m_Period>0) m_pQTimer->changeInterval ( QMAX(100, m_Period) );
           }
    void    pauseTimer(bool isPause)
           {if (m_pQTimer)
               {if (isPause)                m_pQTimer->stop();
                else if (m_Period>0)        m_pQTimer->changeInterval ( QMAX(100, m_Period) );
               }
           }
    void connectMenu()
         {connect( this, SIGNAL( activated() ), this, SLOT( Slot_On_UserQAction_Activated() ) );
         }
 private :
   QString m_Script;
   int     m_Period;
   QTimer *m_pQTimer;
private slots:
    void  Slot_UserQActionOn_TimerDone()
          {emit Sign_UserQAction_TimerDone(m_Script, this);
          }
    void  Slot_On_UserQAction_Activated()
          {emit Sign_UserQAction_Activated(m_Script, this);
          }
signals:
    void  Sign_UserQAction_TimerDone(const QString&, C_UserQAction* );
    void  Sign_UserQAction_Activated(const QString&, C_UserQAction* );
};

//====================================== DrTux =======================================================


/*! \class DrTux
 *  \brief Main class MDI framework derived from QMainWindow.
 *
 *  - this framework contains on left:
 *         -# Buttons of each windows.
 *         -# and the Glossaire.
 *  - this framework contains on midle:
 *         -# all the specifics window's rubrics.
 */
class DrTux: public QMainWindow
{
    Q_OBJECT

public:
    enum ASSISTANT_Mode { InsertString,
                          ReturnString,
                          ATCDString,
                          GestionATCD
                        };
    enum SAVE_Mode      { NoInquire = 0,
                          Inquire   = 1
                        };
    enum LAP_Mode      {  Modification     = 1,
                          NewDocument      = 0,
                          IS_ALREADY_LAUCH = 2,
                          IS_NOT_FOUND     = 3,
                          LAP_NOT_STARTED  = 4,
                          LAP_STARTED      = 5,
                          OLD_ORDO_STRUCT  = 6
                        };

    //! Constructor of DrTux.
    /*!
     \sa QMainWindow()
     \param user user name .
     \param user major user name .
     \param num_GUID unique patient GUID .
     \param id_doss primary key from m_DOSS_IDENT_TBL_NAME.
     \param dossNom patient name.
     \param dossPrenom patient forname.
     \param iniFile configuration path (drtux.ini or Manager.ini).
     \param cfgBaseFile data base configuration.
   */
    DrTux();

   ~DrTux();

         ////////////////////////////////////////////
         ///////////////  DONNES    /////////////////
         ////////////////////////////////////////////


    //Noms des rubriques définies dans: CApp.cpp dans m_ListRubName
    CMDI_Ident *         m_pCMDI_Ident;                       /*!< Pointeur vers la fenetre Identité */
    CMDI_Prescription*   m_pCMDI_Prescription;                /*!< Pointeur vers la fenetre Prescription */
    CMDI_Observation*    m_pCMDI_Observation;                 /*!< Pointeur vers la fenetre Observation */
    CMDI_ChoixPatient*   m_pCMDI_ChoixPatient;                /*!< Pointeur vers la fenetre Choix Patient */
    CMDI_Terrain*        m_pCMDI_Terrain;                     /*!< Pointeur vers la fenetre Terrain */

    RUBREC_LIST          m_RubList;                           /*!< liste des rubriques (documents du dossier médical */
    EVNT_LIST            m_EvnList;                           /*!< liste des liens entre rubriques (documents du dossier médical */
    QWorkspace          *m_pQWorkSpaceRub;                    /*!< workspace CMDI contenant les rubriques */

    FormOngletRubrique  *m_pFormOngletRubrique;               /*!< widget d'onglets du haut de l'espace de travail*/

    QDockWindow         *m_pDock_Menu;                        /*!< widget dock du Menu "Accès Rapide */
    QDockWindow         *m_pDockGlossaire;                    /*!< widget dock du glossaire */
    QDockWindow         *m_pDock_TabRubrique;                 /*!< widget dock des boutons des rubriques */
    QDockWindow         *m_pDock_Organiseur;                  /*!< widget dock de l'organiseur */

    C_FormGlossaire     *m_pFormGlossaire;                    /*!< widget  du glossaire */
    Dock_TabRubrique    *m_pForm_TabRubrique;                 /*!< widget  des boutons des rubriques */
    C_Organiseur        *m_pC_Organiseur;                     /*!< widget  de l'organiseur */
    Dock_Menu           *m_pForm_Menu;                        /*!< widget  Menu "Accès Rapide" */

    //QString              m_DossPrenom;                        /*!< Préom du dossier en cours */
    //QString              m_DossNom;                           /*!< Nom du dossier en cours */
    int                  m_IsModifiable;                      /*!< Possibilité de modification */

    //........................ necessaire à la surveillance du verrou ........................
    QTimer              *m_pTimerVerrou;                  /*!< Délai de verrouillage */
    //long                 m_TimerVerrouDelay;
    //....................... necesaire à la gestion des textes ......................
    QComboBox           *m_comboStyle;                          /*!< Combobox pour définition de style de paragraphe */
    QComboBox           *m_comboFont;                           /*!< Combobox pour défintion de la police de caractère */
    QComboBox           *m_comboSize;                           /*!< Combobox pour défintion de la taille de la police */
    QAction *m_pActionTextBold,                           /*!< Action de menu */
            *m_pActionTextUnderline,                      /*!< Action de menu */
            *m_pActionTextItalic,                         /*!< Action de menu */
            *m_pActionTextColor,                          /*!< Action de menu */
            *m_pActionFastTextColor,                      /*!< Action de menu */
            *m_pActionAlignLeft,                          /*!< Action de menu */
            *m_pActionAlignCenter,                        /*!< Action de menu */
            *m_pActionAlignRight,                         /*!< Action de menu */
            *m_pActionAlignJustify,                       /*!< Action de menu */
            *m_pActionDossierSave,                    /*!< Action de menu */
            *m_pActionCreateListe,                    /*!< Action de menu */
            *m_pActionModifieListe,                   /*!< Action de menu */
            *m_pActionListPatDelete,                  /*!< Action de menu */
            *m_pActionListPatCreate,                  /*!< Action de menu */
            *m_pActionQuitDrTux,                      /*!< Action de menu */
            *m_pActionCIM10,                          /*!< Action de menu */
            *m_pActionLabo,                           /*!< Action de menu */
            *m_pActionCCAM,                           /*!< Action de menu */
            *m_pActionVigie,                          /*!< Action de menu */
            *m_pActionVidal,                          /*!< Action de menu */
            *m_pActionRubMenu,                        /*!< Action de menu */
            *m_pActionActiverDockVigie,               /*!< Action de menu */
            *m_pActionActiverDockButts,               /*!< Action de menu */
            *m_pActionActiverDockGloss,               /*!< Action de menu */
            *m_pActionActiverDockResum;               /*!< Action de menu */
    QToolBar     *m_pToolBarRubMenu;
    QToolBar     *m_pToolBarFile;
    ThemePopup   *m_pMenuFile;                        /*!< Menu pour le terrain */
    ThemePopup   *m_pMenuTerrain;                     /*!< Menu pour le terrain */
    ThemePopup   *m_fenetres;                         /*!< Menu pour les fenetres */
    QActionGroup *m_grp;                              /*!< non documenté */
    QColor        m_LasTextColorChoice;               /*!< dernière couleur utilisée pour le texte */


         ////////////////////////////////////////////
         ///////////////  METHODES  /////////////////
         ////////////////////////////////////////////

    QSqlDatabase *Get_CIM10_QSqlDatabase(){return m_DataBaseCIM10;}
    void Set_CIM10_QSqlDatabase(QSqlDatabase *pQSqlDatabase){m_DataBaseCIM10=pQSqlDatabase;}
    int  getCurrentPageID_ByTitle(QTabWidget *pQTabWidget, const QString &title);
    void SetZoom(int zoom);
    int          getDockMenu_Width();
    QDockWindow* SetDockMenu_OnOff(QDockWindow* pQDockWindow,  QString name);
    void SetModifiableState(int state);
    int  AlertVerrou(const QString &userHostName);
    void ForceWritting();
    void SaveLastSessionInfo();
    void FusionneDocument(QString  &document, const QString &user_doc, RUBREC_LIST::iterator it, const DOCUMENT_DISPLAY_MAP &currentRubIdMap);
    void FusionneDocument(QString  *pDocument, const QString &user_doc, CRubRecord *pCRubCurrentRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap);
    int  DateUserConflictResolve(QString &user, QString &sign_user, QString &date, const QString &obs_user,   const QString &obs_date);
    bool    connectDlgAtcdDialogToDataBase(C_Dlg_GestionATCD *dlg);
    QString CodageCisp( const QString &chapiFilter, const QString &classFilter, const QString &templateStr, const QString &showCombos="Chapitres Rubriques");
    QString CodageCim10(int mode, const QString &listCode=0, int tabToSet=-1);
    //QString ActesClassants(int mode);
    QString Codage_CCAM(int mode);
    void    Get_RecordDispos();
    void    GetCurrentRubriquesPk(QString &obsPk, QString &ordPk, QString &terPk);
    QString DocPrimKeyToRubName(QString docPk);
    QString            PluginExe(       QObject          *pQObject,
                                        const QString     &pathAppli,
                                        const QString     &pathExe,
                                        const QString     &pathIni,
                                        const QString     &pathExch,
                                        const QString     &guid,
                                        const QString     &dosPk,
                                        const QString     &terPk,
                                        const QString     &ordPk ,
                                        QStringList       &param,
                                        int   waitFlag = CMoteurBase::endWait);
    QString         CCAM_Exe( QObject *pQObject,
                              QString pathAppli,
                              QString identPrimKey,
                              QString guid,
                              const QString &local_param,
                              const QString &user);
    void LancerVidal(int delay);
    QString     Lap_RubListExport_Ordo(RUBREC_LIST *pRubList, const QString &path, int modifMode);
    int         Lap_Lauch(const QString &ordo, const QString &from);
    void        Lap_StopProcess();
    QString     Lap_getPatientContext(const QString &prescriptions);
    QString     Lap_getPrescripteurContext();
    QString     Lap_ExchangesFilesToDataBlob(QString *p_html =0);
    QStringList Lap_NewDataToSimpleText(const QString &ordo);
    //------------------------------ Lap_RemoveExchangeFiles ----------------------
    /*! \brief efface les fichiers d'echange de ce patient
    */
    void    Lap_RemoveExchangeFiles();
    int  isThisDockMenu_Desired(QString name);
    void setStateActivationDockMenu(QAction *pQAction, const QString &dockName, const QString text);

    //......................... menus utilisateurs ...........................................
    void            InstallCustomMenu();
    C_UserQAction * Create_CustomAction(QString path);
    void            appendCustomQAction(C_UserQAction *pQAction);
    void            ExeScript(QString &text);
    QString         On_CustomAction(QString& script, C_UserQAction *pQAction);
    void            OnChangePathGlossaire(QString & dir);
    void            initAllRubriquesComboWithRubList();
    void            connectAllCustomQAction();
    //........................ creation des fenêtres MDI ......................................
    //                         dans l'espace rubrique:  m_pQWorkSpaceRub
    CMDI_Ident*          CMDI_IdentCreate        (const char* num_GUID, const char* id_doss,
                                                  const char* dossNom , const char* dossPrenom, const char* user, QString rubName);
    CMDI_Prescription*   CMDI_PrescriptionCreate (const char* num_GUID, const char* id_doss,
                                                  const char* dossNom , const char* dossPrenom, const char* user, QString rubName);
    CMDI_ChoixPatient*   CMDI_ChoixPatientCreate (const char* num_GUID, const char* id_doss,
                                                  const char* dossNom,  const char* dossPrenom, const char* user, QString rubName);
    CMDI_Terrain*        CMDI_TerrainCreate      (const char* num_GUID, const char* id_doss,
                                                  const char* dossNom,  const char* dossPrenom, const char* user, QString rubName);
    CMDI_Observation*    CMDI_RubriqueCreate    (const char* num_GUID, const char* id_doss,
                                                  const char* dossNom,  const char* dossPrenom, const char* user, QString rubName, QString prDroit);
    DOCUMENT_DISPLAY_MAP  GetMapActiveID_Doc();
    QString   MapActiveID_Doc(DOCUMENT_DISPLAY_MAP &mapId);
    CRubRecord* DocPrimKeyToRubRecord(const QString &docPk);
         ////////////////////////////////////////////
         ///////////////  SLOTS     /////////////////
         ////////////////////////////////////////////

public slots:
    void SetDefaultDoc();
    int  SauverDossierAvantNouvelleAction(SAVE_Mode mode = DrTux::Inquire);
    void Slot_SauverLesMeubles();
    void recordPathModulesMenus(QString path);
    void Slot_OnGlossaireLocalise();
    void LapClose();
    void Slot_LapExited();
    void SlotMakeVisible(const QString&);
    void setZoom8();
    void setZoom9();
    void setZoom10();
    void setZoom11();
    void setZoom12();
    void setZoom14();
    void setZoom16();
    void setZoom18();
    void setZoom20();
    void AutoSetActiveWindow();
    void AfficheRubriques();
    void Slot_TestVerrou();
    void Slot_MenuRubriques();
    void ActiverVigie();
    void Slot_GetActiveGUID(QString &guid);
    bool Slot_ActiverDocument(QString docPk, QString rubName, int mode = 0);
    void Slot_NoteToBeSynchronized(int emetteur, const char* txt);
    QString        Slot_RecordDispos();
    QWidget*       IsExistRubrique(const char* rubName);
    CMDI_Generic * IsExistRubrique(int rubTyp);
    QWidget*  GetCurrentRubrique(QString *pRubName=0, int* rub_type = 0, CRubRecord **ppCRubRecord = 0 );
    void      OnGlossaireFileToEdit(QString path, QString action, int typ);
    void      DuplicateRecordWithNewDate(RUBREC_LIST::iterator it, QDateTime &qdt, const QString &libelle, const QString &subType);
    CRubRecord*  OnGlossaireFileClicked(const char *path, int mode, int noInquire=0);
    CRubRecord*  AddNewRecordToRubrique(const QString &strData, int typ, const char *path = 0, const QDateTime *pQDateTime = 0, const QString *libelle = 0, const QString *subType = 0, int noInquire =0);
    //------------------------------ updateAllRubriquesEditorsInRubList ---------------------------------------
    /*! \brief toutes les données en cours d'édition sont replacées dans la structure de liste , prêtes à etre enregistrées */
    void      updateAllRubriquesEditorsInRubList();
    
    void      OnDrTuxSaveRubList();
    void      SetTitle();
    void      recordProfil();
    void add_ATCD_Favori();
    void add_ATCD_Allergique();
    void modifier_TTT_Fond();
    void add_ATCD_Libre();
    void add_CIM10();
    void add_Cisp();
    void On_Vigie_visibilityChanged ( bool visible );
    //..............................................................
    //
    int  GetPossibleObservationDate(const QString &date);
    void OnQuitterDrTux();
    void OnMenuQuitterDrTux();
    void OnCreateNewDoc();
    void Codage_CCAM();
    void PasteLabo();
    void OnDrTuxUserChange();
    void SetInterfaceOnDroits(const QString &droits);
    void OnDrTuxExeAnother(QString num_GUID,  QString id_doss,
                           QString dossNom ,  QString dossPrenom);
    void OnDrTuxPatientListMustBeUpdated(const char* num_GUID,  const char* id_doss,
                                         const char* dossNom ,  const char* dossPrenom );
    void OnDrTuxIdentModified(const char* num_GUID,  const char* id_doss,
                              const char* dossNom ,  const char* dossPrenom );
    void OnDrTuxIdentChange(const char* num_GUID,  const char* id_doss,
                            const char* dossNom ,  const char* dossPrenom );
    void OnDrTuxIdentDelete(const char* num_GUID,  const char* id_doss,
                            const char* dossNom ,  const char* dossPrenom );
    void OnDrTuxIdentCurrentDelete();
    void OnActiverRubrique(const char* rubName, int mode=0);
    void OnActiverOrCreateRubrique(const char* rubName, int mode=0);
    void OnDeleteRubrique(const char* rubName);
    void OnOngletRubriquesChanged(QWidget * pQwidget);
    void OnCreateNewIdent();
    void On_CreateNewRootInOrganiseur(QString primKey);
    void On_DragFileInOrganiseur(QString file, QString *pDocSerialize);
    void setupDocks();             // cree les docks selon les paramètres utilisateur
    void Slot_SetDockMenu_AccesRapide();
    void Slot_SetDockMenu_Glossaire();
    void Slot_SetDockMenu_RubriquesButton();
    void Slot_SetDockMenu_Organiseur();

    void Slot_CreateAllCurrentsTTT();
    void Slot_LapAllActivePrescExited();

    void setupFileActions();       // cree la barre d'outils et menu lié aux fonctions disque
    void setupTextActions();       // cree la barre d'outils et menu lié aux attributs texte (gras italique etc..)
    void setupEditActions();       // cree la barre d'outils et menu lié aux actions (couper coller etc...)
    void setupToolsActions();      // cree la barre d'outils et menu lié aux outils (gestion des listes etc...)
    void setupToolsTerrain();
    void setupVigieActions();
    //.............. formatage du texte ......................................................
    MyEditText *currentEditor()const;
    void        textFamily( const QString & );     //......... on positionne un attribut de l'editeur
    void        textItalic();
    void        textBold();
    void        textUnderline();
    void        textColor();
    void        textSize( const QString & );
    void        textAlign( QAction* );
    void        textStyle( int );
    void        editUndo();
    void        editRedo();
    void        editCut();
    void        editCopy();
    void        editPaste();
    void        editSelectAll();
    void        fast_textColor();
    void        getFormConfig();
    void        editorChanged( QWidget * );        //......... l'editeur signale un changement
    void        fontChanged( const QFont &f );
    void        colorChanged( const QColor &c );
    void        alignmentChanged( int a );
    void        doConnections( QTextEdit *e );     // connecte un editeur de texte aux fonctions de mise a jour des boutons

    //.............. outils du texte ......................................................
    void        CodageCim10();
    void        ListManager();
    void        CreateList();
    void        DisplayContextListPopup();
    void        VidalDataSemp();
    void        UserChange();
    void        Slot_ExePlugin(QString &plugin,   CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap);
    void        Slot_ExeMixture(QString &mixture, CRubRecord *pCRubRecord, const DOCUMENT_DISPLAY_MAP &currentRubIdMap);
    void        Slot_Renouveler_TTT_Fond();
    void        Slot_PutPrescriptionInTerrain(RUBREC_LIST::iterator it);
    void        OnSign_Ident_KillMe();
    void        TimedDeleteRubIdentite();
    void        Fenetres_en_cascade();
    void        Fenetres_en_mosaique();
    void        Fenetres_RetablirPosition();
    void        Fenetres_Identiques();

         ////////////////////////////////////////////
         ///////////////  SIGNAUX   /////////////////
         ////////////////////////////////////////////


signals:
    void  Sign_OnIdentChange(const char* ,  const char* , const char*  ,  const char*  );
    void  Sign_OnUserChange(const char*, const char* );
    void  Sign_ActiverRubrique(const char*, int);
    void  Sign_QuitterDrTux();
    void  Sign_GlossaireRefresh();
    void  Sign_DrTux_Renouveler_TTT_Fond();
    void  Sign_DrTux_PutPrescriptionInTerrain(RUBREC_LIST::iterator );
    void  Sign_OnGlossaireFileClicked(const char* , int );


protected:
    void closeEvent( QCloseEvent* );
    void RubListMakeWhithNewDoss();
    void DocTypeSave(MyEditText *edit, QString &f_name);


private slots:
    void savePos();
    void saveOnlyChildPos();
    void savePos(int mode);
    void Slot_actionApropos();
    void Slot_actionAproposDisplay();
    void tryToStopAPropos();
    void filePrint();

private:
    QSqlDatabase *m_DataBaseCIM10;
    int           m_CCAM_Run;
    QString       m_PluginRun;
    QProcess     *m_Apropos_Proc;
    QProcess     *m_Lap_Proc;

    //.................. menus optionnels utilisateurs.....................
    ThemePopup        *m_Custom_Menu;
    QToolBar          *m_Custom_ToolBar;
    QPtrList<C_UserQAction> m_pCustomActionTab;
    int                m_Custom_Menu_ID;
    bool               m_VidalRun;
    int                m_idToolMenu;

private slots:
QString   Slot_On_UserQAction_TimerDone(const QString& script,   C_UserQAction *pQAction);
QString   Slot_On_UserQAction_Activated(const QString& script,   C_UserQAction *pQAction);
};

#endif
