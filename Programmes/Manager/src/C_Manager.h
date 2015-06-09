/********************************* C_Manager.h ***********************************
*  #include "C_Manager.h"                                                        *
* ...............................................................................*
*   Project   :  MedinTux  (typographie made in france)                          *
*   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
*   by        :  Sevin Roland     from   MedinTux project                        *
*   E-Mail    : roland-sevin@medintux.org                                        *
*   Web site  : www.medintux.org                                                 *
* ...............................................................................*
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
*   License (GPL compatible)                                                     *
*                              http://www.cecill.info/                           *
*   as published by :                                                            *
*                                                                                *
*   Commissariat ?  l'Energie Atomique                                            *
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

#ifndef C_MANAGER_H
#define C_MANAGER_H

#include <QtGui/QMainWindow>
#include <QAbstractSocket>
#include <QPushButton>
#include <QProcess>
#include <QNetworkInterface>
#include <QKeyEvent>
#include <QStyledItemDelegate>

#include "../../MedinTuxTools-QT4/Agenda/C_RendezVous.h"

namespace Ui
{
   class C_ManagerClass;

}
/*
//====================================== C_KeyPressControl =======================================================
class C_KeyPressControl : public QObject
{
    Q_OBJECT
public:
    C_KeyPressControl(QObject *parent)
        : QObject(parent)
    {

    }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
*/

//================================ C_LevelOruItemDelegate =====================================================
    class C_LevelOruItemDelegate : public QStyledItemDelegate
    {
        Q_OBJECT
    public:
        C_LevelOruItemDelegate(QObject *parent = 0);
        void paint(QPainter *painter, const QStyleOptionViewItem &option,  const QModelIndex &index) const;
    };
//====================================== Wdg_ButtonPtr =======================================================
class Wdg_ButtonPtr : public QPushButton
{
   Q_OBJECT

public:
    Wdg_ButtonPtr(QWidget *parent , const QString &name)
    : QPushButton(parent)
    {setObjectName(name);
     connect( this , SIGNAL(clicked ()), this, SLOT(Slot_IsClicked ()) );
     m_Ptr_1 = 0;
     m_Ptr_2 = 0;
    }
    void  setPtr_1(void *ptr){m_Ptr_1 = ptr;}
    void *getPtr_1() {return m_Ptr_1;}
    void  setPtr_2(void *ptr){m_Ptr_2 = ptr;}
    void *getPtr_2() {return m_Ptr_2;}
    void  setPtr_3(void *ptr){m_Ptr_3 = ptr;}
    void *getPtr_3() {return m_Ptr_3;}
    void  setPtr_4(void *ptr){m_Ptr_4 = ptr;}
    void *getPtr_4() {return m_Ptr_4;}
   ~Wdg_ButtonPtr(){}
protected:
   void *m_Ptr_1;
   void *m_Ptr_2;
   void *m_Ptr_3;
   void *m_Ptr_4;
protected slots:
    void Slot_IsClicked()
    {emit clicked(this);
    }
signals:
   void clicked(Wdg_ButtonPtr*pWdg_ButtonPtr);
};

//====================================== C_Manager =======================================================
class C_Frm_Agenda;
class C_Frm_UserList;
class QTreeWidget;
class QTreeWidgetItem;
class CMoteurBase;
class QLabel;
class PtrListIdentFieldData;
class QBoxLayout;
class QFrame;
class C_RendezVous;
class QComboBox;
class QTimer;
class QLineEdit;
class QUrl;
class QWebView;
class C_Vitale;
class C_Manager : public QMainWindow
{
   Q_OBJECT

public:
    C_Manager(CMoteurBase *pCMoteurBase, QWidget *parent = 0, const QString & name="C_Manager for MedinTux");
   ~C_Manager();
    void testInterface();
    //.................... nomadisme ...........................
    int     reconnectToNomade(int gestionNomadisme);
    int     reconnectToMaster(int gestionNomadisme);
    void    waitForsyncEmulDemonResponse(const QString &action);  //master_Log_File, read_Master_Log_Pos
    void    setConnectParametres(const QString &scriptSqlIn,
                                 const QString master_Host         ="",
                                 const QString master_User         ="",
                                 const QString master_Port         ="",
                                 const QString masterKey           ="",
                                 const QString master_Log_File     ="",
                                 const QString read_Master_Log_Pos =""
                                );
    void    putScriptWaitFlag(const QString &ScriptStatusFile);
    QString getScriptNomadePath();
    int     waitForEndScript(const QString &ScriptStatusFile);
    QString socketStateToString(QAbstractSocket::SocketState socketState, const QString& prefix);
    int     testConnection(QString connexionVar,  QString *qstr=0 );
    void    setBatchParametres(const QString &scriptSqlIn, const QStringList &parmList);
    void    setAndSaveNomadeState (int nomadismeStateToSet);
    QString tryToFindConnectionFromList (QString connexionVar,  QString *errMess);
    void    action_fullSwitchReplication_triggered();
    void    action_fullSynchroReplication_triggered();
    QString extractVarFromMysqlStatus(const QString &repliSatus, QString var, int &pos);
    int     setComboBoxOnValue(QComboBox *pQComboBox, const QString& value);
    void    SetModeOn_Button_Affichage_EnCours(const QString &mode);
    //.................. accessoires ...........................
    void demarrerAccessoires();
    void demarrerUnAccessoire(QString accessoire_path);
    //.................. agenda .................................
    void            initComboBoxAgendaUser();
    C_Frm_UserList *getFrm_UserList() {return m_pC_Frm_UserList;}
    void            get_RecordDispos(const QString &mode="FICHE");
    void            reinitAgendaOnUser(const QString &user , const QString &signUser);
    void            reinitListUserOnUser(const QString &user , const QString &signUser);
    void            editUser(QString cps);
    void            eraseUserAgenda(C_Frm_Agenda* pC_Frm_Agenda);
    C_Frm_Agenda   *addUserAgenda(const QString &user, QDate date,  QFrame **ppQFrame = 0);
    void            deleteAgenda(const QString &signUser);
    QStringList     deleteAllAgendas();
    QStringList     getAgendasList();
    QString         execCalendrier(const QDate &dateIn);
    void            setUserAgendaVisible(const QString &signUser);
    void            allAgendasReconnect( const QString &driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                         const QString &baseToConnect, // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                         const QString &user,          // = "root"
                                         const QString &pasword,       // = ""
                                         const QString &hostname,      // = "localhost"
                                         const QString &port           // = "port",          // = "port"
                                       );
    //.................. interface ..........................
    void initWidgetsList();
    void    setAllWidgetsOnFont(const QFont &font);
    void setInterfaceOnMode(const QString &droits, int mode);
    void setVerrouForCurrentIdent();
    void setVerrou(bool state);
    bool isVerrou()            {return m_IsVerrouOn;}
    void setPushButtonDetruireStateFromDroits(bool state);
    QString getBoxStyle(int mode);
    QString getTabStyle(int mode);
    void    setTitle();
    void    replaceInitialDockVisibility();
    //.............. liste patient ...........................
    void lineEdit_NomDossier_textChanged(const QString &nom);
    void lineEdit_PrenomDossier_textChanged(const QString &prenom);
    void listPatientsByDateNss(QString dateNss);
    QTreeWidgetItem *getSelectedListViewPatientItem();
    QTreeWidgetItem *getSelectedListViewItem(QTreeWidget *pQTreeWidget);
    void initListePatient( const QString & qstr_nom, const QString & qstr_prenom);
    int   DataToFormulaire( const QTreeWidgetItem *pQTreeWidgetItem );
    int  _DataToFormulaire(const QString &refToPrimkeyDoss, const QString &nom, const QString &prenom);
    int  DataToFormulaire(const QString &refToPrimkeyDoss, const QString &nom, const QString &prenom , const QString &guid);
    QString ToNum(QString in);
    void setCVonPatient (C_Vitale *pcVitale, int occurence, QString VitaleOrSigems); // SIGEMS
    void setMedWebTuxOnPatient();
    //void setCVonPatient (C_Vitale *pcVitale, int occurence);
    //.............. mode graphique ..........................
    void setInterfaceOnProfil(int mode);
    int  getInterfaceMode(){return m_InterfaceMode;}
    void setInterfaceOnCurrentProfil();
    //............... identite ...............................
    void setIdentModifiedState(int state);
    bool identIsModified(){return m_Ident_IsModified;}
    void setIdentiteEnabled();
    void setIdentiteDisabled();
    bool    dateOfBirthControl();
    bool    sexControl();
    bool    nameControl();
    void tryToSetCodePostalFromVille();
    void tryToSetVilleFromCodePostal();
    void tryToSetTitreFromDateNss();
    void tryToSetInterfaceFromAyantDroit( int qualiteAssure, int focusMustBePotionned);
    void setFieldListData(PtrListIdentFieldData &list_field);
    void clearIdentFields();
    void clearDroitsFields();
    void setInterfaceOnVitaleData(void *pcvitale_in, int occurence);
    void setInterfaceOnDroitsSociaux(C_Vitale *pcVitale, int occurence, int anyWay  = 0 );
    int  setSexFromQualiteAyantDroit(int qualiteAyantDroit);
    void compareListVitaleWithBase();
    void savModifications(const QString &oldNom, const QString &oldPrenom, const QString &oldPk, const QString &oldGuid, int confirmModif=1);
    void exeAnnuaire(QString idInterv );
    void accederDossier( QString nom,  QString prenom, const QString &primKey, const QString &guid, const QString &user, const QString &sign_user);
    void onCreateAction(int lauchEnabled    = 1 );
    int  alertVerrou(const QString &userBy, const QString &dossNom, const QString &dossPrenom);
    void setMedWebTuxOnUser();
    void tryToStopAPropos();
    QString appelPyxvital (QString listeParametres);                       // Cz_Pyxvital
    long        GetOruList(QTreeWidget     *pQlistView);                   // ORU
    QStringList GetOruSql_XML_List();                                      // ORU
    long    GetEntreesSigemsList( QTreeWidget     *pQlistView  );          // SIGEMS
    void    create_CV_with_patient_Sigems();                               // SIGEMS
    QString DouJeViens(QTreeWidgetItem *pQListViewItem);                   // SIGEMS
    void    changeAllAgendasGuid(const QString &old_guid, const QString &new_guid);
private slots:
   virtual void closeEvent(QCloseEvent *event);
   void Slot_listView_Oru_Clicked( QTreeWidgetItem * , int);
   void Slot_listView_Oru_DoubleClicked( QTreeWidgetItem * , int);
   void Slot_listView_Oru_ContextMenuRequested (const QPoint &);
   void Slot_GetOruList();

   void Slot_SauverLesMeubles();
   void Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus);
   void Slot_actionRecordWindowsPos_triggered (bool);
   void Slot_actionSetGlobalFont_triggered (bool);
   void Slot_actionHideShowLogo_triggered (bool);
   void Slot_action_DebloquerBases (bool);
   void Slot_retranslateUi();

   void Slot_tabWidget_Central_currentChanged (int index);
   void Slot_LocationEdit_returnPressed();
   void Slot_webView_MedWebTux_urlChanged ( const QUrl &url  );
   void Slot_On_webView_MedWebTux_loadStarted();
   void Slot_On_webView_MedWebTux_loadProgress(int progress);
   void Slot_On_webView_MedWebTux_loadFinished(bool ok);

   void Slot_actionWebPrint (bool);
   void Slot_actionWebHome (bool);
   void Slot_actionWebHelp (bool);
   void Slot_actionWebFind (bool);
   void Slot_actionMenuNew_clicked( bool );
   void Slot_WebFindEdit_returnPressed();
   void Slot_WebFindEdit_textChanged (const QString &text );
   void Slot_action_AccesWebMedTux (bool);
   void Slot_actionApropos (bool);
   void Slot_actionAproposDisplay();
   void Slot_action_Aide(bool);
   void Slot_actionWebStop (bool);

   void Slot_OnTimer();
   void Slot_OutFocuslineEdit_DtNss(QFocusEvent*, int &);
   void Slot_OutFocusVille(QFocusEvent*, int &);
   void Slot_OutFocusCodePostal(QFocusEvent*, int &);
   void Slot_CharEventCodePostal(QKeyEvent *, int &);
   void Slot_CharEventNumSecu(QKeyEvent *, int &);
   void Slot_InFocusNumSecu(QFocusEvent*, int &);
   void Slot_OutFocusNumSecu(QFocusEvent*, int &);
   void Slot_comboBoxAgendaUser_activated( const QString& );
   //.............. agenda ..................................
   void Slot_LauchPatient(const QString &);
   void Slot_LauchPatient(const QString &, C_RendezVous *);
   void Slot_buttonAgendaDelete_Clicked(Wdg_ButtonPtr *pWdg_ButtonPtr);
   void Slot_pQPushButtonAgendaDate_Clicked ( Wdg_ButtonPtr *pWdg_ButtonPtr);
   void Slot_pQPushButtonMenuAgenda_Clicked(Wdg_ButtonPtr* pWdg_ButtonPtr);
   void Slot_pQPushButtonNextDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr); // CZA
   void Slot_pQPushButtonPreviusDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr); // CZA
   void Slot_pQPushButtonThisDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr);
   void Slot_pQPushButtonWeekDay_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr); // CZA
   void Slot_pQPushButtonFreeSpace_Clicked (Wdg_ButtonPtr*);
   void Slot_pQPushButtonPatientRdvList_Clicked(Wdg_ButtonPtr* pWdg_ButtonPtr);
   void Slot_pQPushButtonUserRdvList_Clicked (Wdg_ButtonPtr* pWdg_ButtonPtr);
   void Slot_setUserAgendaVisible();
   void Slot_ReinitModeWeekDayMonth_OnDate(const QDate &date , const QString &mode, C_Frm_Agenda *pC_Frm_Agenda);
   void Slot_OnFastAccesAgendaButtonClicked(QAbstractButton *);
   //........................ connecter les slot de changement ............................................................................................
   void Slot_TextChanged_lineEdit_NomDossier(const QString & );
   void Slot_TextChanged_lineEdit_PrenomDossier(const QString & );
   void Slot_TextChanged_lineEdit_NomDeNss(const QString & );
   void Slot_TextChanged_lineEdit_DtNss(const QString & );
   void Slot_TextChanged_lineEdit_Titre(const QString & );
   void Slot_TextChanged_lineEdit_Profession(const QString & );
   void Slot_TextChanged_textEdit_Adresse( );
   void Slot_TextChanged_lineEdit_Tel1(const QString & );
   void Slot_TextChanged_lineEdit_Tel2(const QString & );
   void Slot_TextChanged_lineEdit_Tel3(const QString & );
   void Slot_TextChanged_lineEdit_CdPostal(const QString & );
   void Slot_TextChanged_lineEditVille(const QString & );
   void Slot_TextChanged_lineEdit_Email(const QString & );
   void Slot_TextChanged_lineEdit_NumSecu(const QString & );
   void Slot_TextChanged_lineEdit_NomAssure(const QString & );
   void Slot_TextChanged_lineEdit_PrenomAssure(const QString & );
   void Slot_TextChanged_textEdit_Note( );
   void Slot_TextChanged_lineEdit_RangGeme(const QString &);
   void Slot_TextChanged_comboBoxSexe( const QString &);
   void Slot_TextChanged_comboBoxQualiteAyantDroit( const QString &);
   void Slot_TextChanged_lineEdit_DtNss_Assure(const QString &);

    void  Slot_action_ModeConnect_triggered (bool);
    void  Slot_action_ToModeConnect_triggered (bool){}
    void  Slot_action_ToModeNomade_triggered (bool);
    void  Slot_hostMasterFound ();
    void  Slot_hostSlaveFound ();
    void  Slot_hostMasterError (QAbstractSocket::SocketError err);
    void  Slot_hostSlaveError  (QAbstractSocket::SocketError err);
    void  Slot_hostMasterStateChanged (QAbstractSocket::SocketState socketState);
    void  Slot_hostSlaveStateChanged (QAbstractSocket::SocketState socketState);

    void Slot_listView_Praticiens_DoubleClicked( QTreeWidgetItem * , int);
    void Slot_listView_Vitale_Clicked( QTreeWidgetItem * item, int column );
    void Slot_listView_Vitale_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int);
    void Slot_listView_Patient_Clicked( QTreeWidgetItem * , int);
    void Slot_listView_Patient_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int);
    void Slot_listView_Praticiens_contextMenuRequested (const QPoint &);
    void Slot_pushButtonAcceder_Clicked();
    void Slot_pushButton_AddMedTTT_clicked();
    void Slot_pushButton_AddMedTTT_Fast_clicked ();
    void Slot_pushButton_DelMedTTT_clicked();
    void Slot_pushButtonCreate();
    void Slot_pushButtonSavModifWithoutConfirmation();
    void Slot_pushButtonSavModif();
    void Slot_pushBut_ExitMultiCritere();
    void Slot_pushButton_Effacer_clicked();
    void Slot_pushButton_MultiCritere();
    void Slot_pushBut_MultiCritFind();
    void Slot_pushButton_ReinitListe_clicked();
    void Slot_pushButtonInfoClicked();
    void Slot_pushButtonDetruire_clicked();
    void Slot_pushButtonNew_clicked();
    void Slot_NewDossWithoutErase();
    void Slot_pushButtonMenuNew_clicked();
    void Slot_pushButtonCancelCreate_clicked();
    void Slot_lineEditAutoLocator_keyPressEvent(QKeyEvent *, int &);
    void Slot_lineEditAutoLocator_textChanged(const QString &);
    void Slot_ListView_itemSelectionChanged();
    void Slot_giveDraglistView_PatientItemData(QString &data, QTreeWidgetItem *pQTreeWidgetItem);
    void Slot_ListView_ContextMenuRequested (const QPoint &);
    void Slot_listView_DoublonsSelectionChanged();
    void Slot_listView_Doublons_contextMenuRequested (const QPoint &);
    void Slot_listView_Doublons_MenuRequested();
    void Slot_listView_VitaleSelectionChanged ();
    // SIGEMS DEB
    void Slot_listView_VitaleOrSigemsSelectionChanged (QString WhatListView);
    void Slot_listView_SigemsSelectionChanged ();
    void Slot_listView_Sigems_Clicked( QTreeWidgetItem * item, int column );
    void Slot_listView_Sigems_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int);
    void Slot_listView_Vitale_ContextMenuRequested (const QPoint &);
    void Slot_listView_Sigems_ContextMenuRequested (const QPoint &);
    void Slot_listView_VitaleOrSigems_MenuRequested (QString WhatTreeView);
    // SIGEMS FIN
    void Slot_reinitManagerOnUser(QTreeWidgetItem *pUserItem , QTreeWidgetItem *pSignUserItem);
    void Slot_reinitManagerOnUser(const QString &user        , const QString &signUser);
    void Slot_UserSelected_InListUser(QTreeWidgetItem *pUserItem , QTreeWidgetItem *pSignUserItem);
    void Slot_RecordPos();
    void Slot_RecordDispos();
    void Slot_CPS_IsClicked();
    void Slot_pushButton_action_UserParams();
    void Slot_EditUser(const QString&, const QString&);
    void Slot_UserMustBeUnloged(int &ret);
    void Slot_pushButtonVitale_clicked();
    void Slot_action_Imprimer(bool);
    void Slot_pushButton_SigemsPA_Clicked();
    void Slot_Type_Affichage_Change();          // CZA
    void Slot_launchSpecificJob (bool);

    void Slot_pushButton_FSE();                                        // Cz_Pyxvital
    // void Slot_saisieFSEenCours();                                      // PYX_11_2014 Cz_Pyxvital
    void Slot_Button_Vitale_ContextMenuRequested (const QPoint &);     // Cz_Pyxvital
    void Slot_Button_Vitale_MenuRequested ();                          // Cz_Pyxvital
    void Controle_Solde_Patient(QString guidPatient);                  // CZ_Cpta
    void Slot_Saisie_Reglement();                                      // CZ_Cpta
    void appelCompta (QString nomProgCPta);                            // CZ_Cpta2
//#ifdef ENTREES_SIGEMS
    void Slot_OnTimerEntrants();                                       // SIGEMS
    void Slot_OnTimerSuspendEntrants();                                // SIGEMS
//#endif
private:
   Ui::C_ManagerClass *m_pGUI;
   QProcess                     *m_Apropos_Proc;
   QMap <QString ,C_Frm_Agenda*> m_AgendaMap;
   QFrame                       *m_pMultiAgendaFrame;
   QBoxLayout                   *m_pAgendaQLayout;
   QLineEdit          *m_locationEdit;
   QLineEdit          *m_WebFindEdit;
   QMenuBar           *m_menuBar;
   QToolBar           *m_WebToolBar;
   QToolBar           *m_NomadismeToolBar;
   //............ bouton bascule agenda .......
   QString             m_strGotoAgenda;
   QString             m_strGotoPatientList;
   //............ menu fichiers .......
   QMenu              *m_menuFichiers;
   QAction            *m_action_ModeNomade;
   QAction            *m_action_ToModeConnect;
   QAction            *m_action_AccesWebMedTux;
   QAction            *m_action_DebloquerBases;
   QAction            *m_action_UserParams;
   QAction            *m_action_NouveauDossier;
   QAction            *m_actionImprimer;
   QAction            *m_actionSpecifique;
   QAction            *m_actionQuitter;
   //............ menu fenetre .......
   QMenu              *m_menuFenetre;
   QAction            *m_action_dockMenu;
   QAction            *m_actionHideShowLogo;
   QAction            *m_actionSetGlobalFont;
   QAction            *m_actionRecordWindowsPos;
   //............ menu web .......
   QAction            *m_action_WebPrint;
   QAction            *m_action_WebHome;
   QAction            *m_action_WebHelp;
   QAction            *m_action_WebFind;
   QAction            *m_action_WebStop;
   //............ menu a propos .......
   QMenu              *m_menuInfo;
   QAction            *m_action_A_Propos;
   QAction            *m_action_Aide;
   QAction            *m_actionTypeAffichage; // CZA

   C_Frm_UserList     *m_pC_Frm_UserList;
   int                 m_InterfaceMode;
   CMoteurBase        *m_pCMoteurBase;
   int                 m_Ident_IsModified;
   bool                m_IsVerrouOn;
   QLabel *            m_AgendaDockTitle;
   QString             m_List_GUI_Mode[3];
   QMenu*              m_ButtonNew_PopMenu;
   bool                m_Contacts_Run;
   QAbstractSocket    *m_pMasterSocket;
   QAbstractSocket    *m_pSlaveSocket;
   QString             m_tmpSignUser;
   QTimer             *m_pReconnectTimer;
   int                 m_AgendaInitialVisbility;
   int                 m_UserListInitialVisbility;
   int                 m_HasLapCompatibility;       // rs has
   int                 m_SexControl;                // rs has
   QWebView           *m_webView_MedWebTux;
   QString             m_ficFacturePar;                             // Cz_Pyxvital
   QString             m_ficPatientPar;                             // Cz_Pyxvital
   QTimer             *m_timerFSE;                                  // Cz_Pyxvital
   bool                m_FSEenCours;                                // Cz_Pyxvital Ordre de FSE en cours sur Pyxvital
   QString             m_Facture_Seule;                             // Cz_Pyxvital
   QString             m_typFact;                                   // CZ_Cpta
   QString             m_Type_Affichage_EnCours;                    // CZA
   Wdg_ButtonPtr      *m_Button_Affichage_EnCours;

   QString             m_Entrants_Sigems;                           // SIGEMS
   QString             m_Nb_Heures_Derniers_Entrants;               // SIGEMS
   QString             m_Heure_Deb_Main_Courante;                   // SIGEMS
   QString             m_delaySuspendEntrantsTimer;                 // SIGEMS
   QTimer             *m_pListUpdateTimer;                            // SIGEMS
   QTimer             *m_pSuspendEntrantsTimer;                     // SIGEMS
   QString             m_pathOru;
   int                 m_oruCriticalWait;
   bool                m_oruContextActivated;
   bool                m_oruMethodIsSQL;
signals:
   void               Sign_applicationMustBeStop();
};

#endif // C_MANAGER_H
