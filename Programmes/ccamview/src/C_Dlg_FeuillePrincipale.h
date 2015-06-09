//
// C++ Interface:
//
// Description:
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef C_DLG_FEUILLEPRINCIPALE_H
#define C_DLG_FEUILLEPRINCIPALE_H
#include <Q3ListViewItem>
#include <Q3ListView>
#include <Q3HttpResponseHeader>
#include <QProcess>

#include "FeuillePrincipale.h"

#include <stdlib.h>

#include "TempActeXPM.xpm"
#include "ValidActeXPM.xpm"
#include "NewActeXPM.xpm"
#include "ModifActeXPM.xpm"
//extern QString Global_DebugStr;
//.................... qsqldatabase ......................
#include "qsqldatabase.h"
#include "CMoteurCCAM_Base.h"
#include "qsqlquery.h"
#include "qmessagebox.h"
#include "qtooltip.h"
#include "CSoapInterface.h"
#include "Global.h"
#include "LivreFerme.xpm"
#include "LivreOuvert.xpm"
#include "Dlg_SigemsLogin.h"
#include "Dlg_Praticiens.h"
#include "Dlg_Categories.h"
#include "Dlg_PatientActeProperty.h"
#include "DlgPrint.h"
#include "qdir.h"
#include "qfiledialog.h"
#include "Dlg_MyAlertBox.h"
#include "qapplication.h"
#include "qstatusbar.h"
#include "Dlg_ChargeMAJCCAM.h"

#include <Q3ListBox>
#include <Q3ValueList>
#include <Q3StyleSheet>
#include <Q3ListViewItemIterator>
//#include <QX11Info>
#include <Q3PopupMenu>
#include <QPaintDevice>
#include <Q3ValueList>
#include <Q3PtrList>
#include <Q3Button>
#include <Q3ComboBox>
#include <Q3ListBox>
#include <Q3ListBoxItem>

#include "../../MedinTuxTools-QT4/CGestIni.h"

enum MODE
    {   SIGEMS_REAL_TIME   = 0x0001,
        LOCAL_MODE         = 0x0002,
        FULL_FIND          = 0x0004,
        XML_MONITORING     = 0x0008
    };
// Sous Windows virer dans le .ui tous les <includehint>cmaskedlineedit.h</includehint>
enum LVACTES {LVA_DATE            ,    // 0
              LVA_HEURE           ,    // 1
              LVA_PRATICIENNAME   ,    // 2
              LVA_CODECCAM        ,    // 3
              LVA_ASSO_WITH       ,    // 4
              LVA_CODE_ASSO       ,    // 5
              LVA_LIBELLEACTE     ,    // 6
              LVA_TARIF           ,    // 7
              LVA_PHASE           ,    // 8
              LVA_ACTIVITE        ,    // 9
              LVA_MODIFICATEUR1   ,    // 10
              LVA_MODIFICATEUR2   ,    // 11
              LVA_MODIFICATEUR3   ,    // 12
              LVA_MODIFICATEUR4   ,    // 13
              LVA_TRF_CALC        ,    // 14
              LVA_NB              ,    // 15
              LVA_NUMDOSS         ,    // 16
              LVA_NOMENCLAT       ,    // 17
              LVA_NIDPRATICIEN    ,    // 18
              LVA_TYP             ,    // 19
              LVA_ENFANT          ,    // 20
              LVA_PRESCRIPTEUR    ,    // 21
              LVA_CODE_REJET      ,    // 22
              LVA_REGLE_ASSO           // 23
           };

// conversion des index indexes des onglets en noms
enum ONGLETS_R {ONGLET_RECHERCHE_CODE_ACTE,
                ONGLET_THESAURUS,
                ONGLET_RECHERCHE_PATIENT};
enum OBJETS_I {TB_NOTES,
               TB_PHASES,
               TB_HIERARCHIE,
               TB_PROCEDURES,
               LV_ASSOCIES,
               LV_INCOMPATIBLES,
               LV_COMPLEMENTAIRES,
               TB_MODIFICATEURS};

#define LIBELLE_WIDTH 120
#define MESSAGE_DOSS_NO_SELECTED "Pour cette action il vous faut auparavant:\n    => activer l'onglet \"Recherche Patient\".\n    => sélectionner un patient.\n    => sélectionner un dossier de séjour de ce patient.\n          afin que son numéro de dossier de séjour soit renseigné.\n"




class C_Dlg_FeuillePrincipale : public FeuillePrincipale
{
  Q_OBJECT

public:
  C_Dlg_FeuillePrincipale(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
  ~C_Dlg_FeuillePrincipale();
  /*$PUBLIC_FUNCTIONS$*/
  virtual int           ReplaceThesaurusTokenByValue( INFO_MED * pInfoMed, Q3ListViewItem * pQListViewItemActes, QString & document, int pos_deb, int len );
  virtual QString       Get_Head_Foot_Thesaurus( QString * foot );
  virtual int           ReplaceTokenByValue( IDENT_PAT * pIdentPat, Q3ListViewItem * pQListViewItemActes, QString & document, int pos_deb, int len );
  virtual QString       Get_Head_Foot( QString * foot );
  virtual QString       Get_Export_Head_Foot();
  virtual QString       SetToNum( QString txt );
  virtual int           MessageAlert( const char * title, const char * message, const char * button1, const char * button2, const char * button3, const char * button4 );
  virtual QDate         StrDateToQDate( QString date_acte, int & error );
  virtual int           GetDateEntreeSortie( QDate & dateEntree, QDate & dateSortie, QString numDoss );
  virtual QString       GetSexePatient();
  virtual Q3ListViewItem * IsThisActeInListViewActe( const QString & acte, const QDateTime dateTimeEntree, const QDateTime dateTimeSortie, CCAM_ACTE * pCCAMActe );
  virtual int           IsWarningActe( Q3ListView * pQListView, CCAM_ACTE & ccamActe, QDateTime dateTimeEntree, QDateTime dateTimeSortie, QString * pErrorStr );
  virtual Q3ListViewItem * IsThisActeInListViewActeModifier( const QString & acte, const QDateTime dateTimeEntree, const QDateTime dateTimeSortie, CCAM_ACTE * pCCAMActe, Q3ListViewItem * item_to_modif );
  virtual int           IsWarningActeModifier( Q3ListView * pQListView, CCAM_ACTE & ccamActe, QDate dateEntree, QDate dateSortie, Q3ListViewItem * item_to_modif, QString * pErrorStr );
  virtual bool          VerifieModificateur( QStringList & strList, QString mod );
  virtual int           VerifieActe( Q3ListView * pQListView, CCAM_ACTE & ccamActe, QDate dateEntree, QDate dateSortie, QString * pErrorStr );
  virtual Q3ListViewItem * PutActeInListView( Q3ListViewItem * element, Q3ListViewItem * pere, CCAM_ACTE & ccamActe, const QPixmap & pixmap );
  virtual QString       CalculePrixTotal();
  virtual QString       GetNumDossierFromEditables();
  virtual bool          IsSameActe( Q3ListViewItem * pQListViewItem, QStringList & data_list );
  virtual int           IsThisActeInThesaurus( Q3ListView * pQListView, QString codeActe );
  virtual QString       DoPopupList( QStringList & list );
  virtual QString       GetNidComboPraticiens();
  virtual QString       GetCodeComboPraticiens();
  virtual QString       GetComboActivite();
  virtual int           initDialog( CMoteurCCAM_Base * pCMoteurCCAM_Base, const QString & code_utilisateur );
  virtual int           getCurrentPageID_ByTitle( QTabWidget * pQTabWidget, const QString & title );
  virtual QString       GetComboDateEffet();
  void                  tryToStopAPropos();
public slots:
  /*$PUBLIC_SLOTS$*/
  void                  Slot_ActivateMainWindow();
  void                  Slot_actionAproposDisplay();
  virtual void          edit_Code_lostFocus();
  virtual void          fileExporter_la_table_des_mdecinsAction_activated();
  virtual void          fileImporter_la_table_des_medecinsAction_activated();
  virtual void          fileEffacer_le_thsaurusAction_activated();
  virtual void          fileImporter_un_thsaurusAction_activated();
  virtual void          fileExporter_le_thsaurusAction_activated();
  virtual void          configurationImport_Tables_EssentiellesAction_activated();
  virtual void          configurationExport_Table_ACTION1Action_activated();
  virtual void          Get_BodyPartsThesaurus( QString & bodyHead, QString & bodyActe, QString & bodyfoot );
  virtual void          CreateBodyThesaurus( Q3ListView * pQListViewActes, QString & body, int repeat_nb, int flag );
  virtual void          FusionneThesaurus( INFO_MED * pInfoMed, Q3ListViewItem * pQListViewItemActes, QString & document );
  virtual void          bouton_ImprimerThesaurus_clicked();
  virtual void          GetDateEntreeSortie( QString & dateEntree, QString & dateSortie );
  virtual void          FusionneDocument( IDENT_PAT * pIdentPat, Q3ListViewItem * pQListViewItemActes, QString & document );
  virtual void          CreateBody( Q3ListView * pQListViewActes, QString & body, int repeat_nb, int flag );
  virtual void          Get_BodyParts( QString & bodyHead, QString & bodyActe, QString & bodyfoot );
  virtual void          pushButtonActePatImprimer_clicked();
  virtual void          HtmlExport();
  virtual void          Imprimer( int page );
  virtual void          Imprimer();
  virtual void          pushButtonActePatEffacer_clicked();
  virtual void          AboutCCAM_View();
  virtual void          listview_ListeActePat_doubleClicked( Q3ListViewItem * pQListViewItem );
  virtual void          listview_ListeDossPat_clicked( Q3ListViewItem * pQListViewItem );
  virtual void          listview_ListePatient_clicked( Q3ListViewItem * pQListViewItem );
  virtual void          pushButtonDossRech_clicked();
  virtual void          pushButtonRechAvancee_clicked();
  virtual void          lineedit_NumDoss_lostFocus();
  virtual void          lineedit_NumDoss_textChanged( const QString & st );
  virtual void          lineedit_NumDossAn_textChanged( const QString & str );
  virtual void          SetFocusOnNumDoss();
  virtual void          SetFocusOnNumDossAn();
  virtual void          lineedit_NumDoss_returnPressed();
  virtual void          lineedit_NumDossAn_returnPressed();
  virtual void          pushButtonChercherPatient_clicked();
  virtual void          pushButtonClear_clicked();
  virtual void          pushButtonNumDossier_clicked();
  virtual void          comboBox_Bases_highlighted( const QString & base );
  virtual void          comboBox_Bases_activated( int );
  virtual void          ClearAll();
  virtual void          ClearAllIdentiteWithout4RechFields();
  virtual void          ClearAllIdentite();
  virtual void          pushButtonRecActesListToSigems_clicked();
  virtual void          checkBoxTestMode_stateChanged( int state );
  virtual void          Slot_responseHeaderReceived( const Q3HttpResponseHeader & response );
  virtual void          Slot_SOAP_SelectDossierHospi_done( bool state );
  virtual void          SelectDossierHospiSigems( const QString & numDoss );
  virtual void          Slot_SOAP_SearchDossier_done( bool state );
  virtual void          SearchDossierSigems( const QString & ippSigemsPatient );
  virtual void          Slot_SOAP_SelectPatient_done( bool state );
  virtual void          SelectPatientSigems( const QString & ipp );
  virtual void          Slot_SOAP_GetDatabasesList_done( bool state );
  virtual void          GetBaseSigems();
  virtual void          Slot_SOAP_GetPatientList_done( bool state );
  virtual void          ChercherPatient();
  virtual void          Slot_SOAP_ListerSigemsActesPatient_done( bool state );
  virtual void          Slot_SOAP_ListerSigemsActesTempPatient_done( bool state );
  virtual void          ListerSigemsActesPatient( QString numDoss, QString idPrat, QString mode );
  virtual void          Slot_SOAP_EffacerActeSigems_done( bool state );
  virtual void          EffacerActeSigems( QStringList & data_list );
  virtual void          EffacerActeSigems( Q3ListViewItem * pQListViewItem );
  virtual void          Display_createActeTemp_SigemsError( QString error_str );
  virtual void          Slot_SOAP_EnregistrerActeSigems_done( bool state );
  virtual void          EnregistrerActeSigems( Q3ListViewItem * pQListViewItem );
  virtual void          Slot_SOAP_EnregistrerActeParDossierSigems_done( bool state );
  virtual void          EnregistrerActeParDossierSigems();
  virtual void          DisplayColoredXML_Response( QString & xml, CSoapInterface * pcSoapI );
  virtual void          DisplayColoredXML_Request( QString & result );
  virtual void          configurationMise__jour_de_la_Base_CCAMAction_activated();
  virtual void          AjouterActeAuDossier( int flag, QString codeCCAM );
  virtual void          SetNumDossierToEditables( QString numDoss );
  virtual void          QStringList_To_QListViewItemActe( Q3ListViewItem * pQListViewItem, QStringList & list );
  virtual void          QListViewItemActe_To_QStringList( Q3ListViewItem * pQListViewItem, QStringList & list );
  virtual void          EraseAllSameActes( Q3ListViewItem * pQListViewItem );
  virtual void          deleteAllChilds( Q3ListViewItem * pQListViewItem );
  virtual void          pushButtonActePatModifier_clicked();
  virtual void          GetActesAssocies( const QString & codeCCAM, QStringList & codeAssoList );
  virtual void          textbrowser_hierarchie_anchorClicked( const QString &, const QString & link );
  virtual void          textbrowser_procedures_anchorClicked( const QString &, const QString & link );
  virtual void          bouton_AjouterDossier_clicked();
  virtual void          lview_GestesComplementaires_doubleClicked( Q3ListViewItem * pQListViewItem );
  virtual void          lview_ActesTrouves_doubleClicked( Q3ListViewItem * pQListViewItem );
  virtual void          listview_Thesaurus_doubleClicked( Q3ListViewItem * pQListViewItem );
  virtual void          combo_Praticiens_activated( int );
  virtual void          combo_Categories_activated( int );
  virtual void          bouton_SupprimeActeThesaurus_clicked();
  virtual void          bouton_ModifActeThesaurus_clicked();
  virtual void          listview_Thesaurus_selectionChanged();
  virtual void          ChangeEtatBoutonDossierPatient( int appelant );
  virtual void          ChangeEtatBoutonThesaurus();
  virtual void          lview_ActesTrouves_selectionChanged();
  virtual void          lview_Hierarchie_selectionChanged( Q3ListViewItem * );
  virtual void          lview_Hierarchie_currentChanged( Q3ListViewItem * );
  virtual void          bouton_AjouterThesaurus_clicked();
  virtual void          menu_drop_praticiens_clicked();
  virtual void          menu_praticien_clicked();
  virtual void          ChangeEtatOnglet( int objet );
  virtual void          NoeudOuvert( Q3ListViewItem * item );
  virtual void          NoeudFerme( Q3ListViewItem * item );
  virtual void          cbox_SiteAnatomique_highlighted( const QString & qstr );
  virtual void          lview_Hierarchie_clicked( Q3ListViewItem * pQListViewItem );
  virtual void          comboBoxActivite_activated( const QString & );
  virtual void          GetPopupItem();
  virtual void          OnMenuActivated( int id );
  virtual void          listview_ListeActePat_clicked( Q3ListViewItem * pQListViewItem, const QPoint & p, int c );
  virtual void          listview_Thesaurus_clicked( Q3ListViewItem * pQListViewItem );
  virtual void          ClearAllInformations();
  virtual void          lview_ActesTrouves_clicked( Q3ListViewItem * pQListViewItem );
  virtual void          recherche_par_code( const QString & );
  virtual void          Modif_MotCle1( const QString & mot_cle_saisi1 );
  virtual void          Modif_MotCle2( const QString & mot_cle_saisi2 );
  virtual void          critere_code( const QString & );
  virtual void          change_onglet_recherche( QWidget * onglet_selectionne );
  virtual void          FillComboModesAcces();
  virtual void          FillComboAction();
  virtual void          FillComboSitesAnatomiques();
  virtual void          FillComboDateEffet();
  virtual void          FillComboActivite();
  virtual void          closeEvent( QCloseEvent * ce );
  virtual void          SetInterface();
  virtual void          comboBoxDateEffet_activated( const QString & );

protected:
  /*$PROTECTED_FUNCTIONS$*/
    QString           m_NUM_VERSION;         /*!< numero de version du programme */
    QProcess         *m_Apropos_Proc;
    QString           m_DateEffet;
    Q3PopupMenu*      m_pQPopupMenu;
    QString           m_RetPopupItem;
    QString           m_CurrentPopupItem;
    int               m_ModeFlag;
    QTime             m_LastTime;
    QDate             m_LastDate;
    QString           m_SigemsVers;
    QString           m_NumKeyboardMap;
    QString           m_Code_Utilisateur;
    CSoapInterface*   m_pcSoapI;
    CMoteurCCAM_Base* m_pCMoteurCCAM_Base;
    QString           m_UrlSigemsWebServices;
    QString           m_SigemsPassword;
    QString           m_SigemsLogin;
    int               m_UpdatePatList;
    QString           m_Code_Praticien;
    QString           m_GuidDrTux;
    int               m_IsEraseActeOK;
    Q3ListViewItem*   m_pQListViewItemToRecord;
    Q3ListViewItem*   m_pQListViewItemToErase;

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

