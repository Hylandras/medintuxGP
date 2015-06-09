// C++ Implementation:
/********************************* C_Organiseur.cpp ***********************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: roland-sevin@medintux.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat � l'Energie Atomique
    - CEA,
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.
                             FRANCE
    Centre National de la Recherche Scientifique
    - CNRS,
                             3 rue Michel-Ange, 75794 Paris cedex 16.
                             FRANCE
    Institut National de Recherche en Informatique et en Automatique
    - INRIA,
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153
                             FRANCE

          This program is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          CeCILL  License (GPL compatible) for more details.

*/

#include <qwidget.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qheader.h>
#include <qvariant.h>
#include <qcursor.h>
#include <qbutton.h>
#include <qcheckbox.h>
#include <qcursor.h>

#include "CApp.h"

#include "ui/Form_Organiseur.h"
#include "ui/Dlg_Calendar.h"
#include "ui/DlgListOff_Listes.h"

#include "../../drtux/src/C_DlgListFieldMngr.h"
#include "C_Organiseur.h"
#include "CHtmlTools.h"
#include "CGenTools.h"
#include "C_TokenInterpret.h"
#include "C_Dlg_Pendule.h"
#include "CMDI_Generic.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "../../MedinTuxTools/ThemePopup.h"  // Gestion du thème de l'appli
#include "../../MedinTuxTools/CMaskedLineEdit.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "MyEditText.h"
#include "drtux.h"
#include "CPreview.h"
#include "C_FormGlossaire.h"
#include "../drtux/src/C_Dlg_VigieFiltreConf.h"

#define pMB G_pCApp->m_pCMoteurBase
#define CP_Deb 3
#define CP_Fin 7
#define DT_Deb 9
#define DT_Fin 10

//===============================================================  C_VigieListView =========================================================================
//-------------------------------- C_VigieListView ----------------------------------------------------
C_VigieListView::C_VigieListView( QString *ptr, QWidget *parent, const char *name )
    : C_DragQListView( ptr, parent, name )
{setMouseTracking ( TRUE );
 m_PathDocuments                   = G_pCApp->m_PathGlossaire+"Vigie";
 m_DefaultMaskDirectory            = m_PathDocuments;
}

//-------------------------------- C_VigieListView ----------------------------------------------------
C_VigieListView::C_VigieListView( QWidget *parent, const char *name )
    : C_DragQListView( parent, name )
{setMouseTracking ( TRUE );
 m_PathDocuments                   = G_pCApp->m_PathGlossaire+"Vigie";
 m_DefaultMaskDirectory            = m_PathDocuments;
}

//-------------------------------- clear ----------------------------------------------------
void C_VigieListView::clear()
{QListView::clear();
 m_PatientPk.clear();
}
//-------------------------------- clear ----------------------------------------------------
void C_VigieListView::initialisePathGlossaire(const QString &pathDocuments, const QString &defaultMaskDirectory)
{m_PathDocuments                   = pathDocuments;          // G_pCApp->m_PathGlossaire+"Vigie"
 m_DefaultMaskDirectory            = defaultMaskDirectory;
}

//-------------------------------- GetPatientRootItem ----------------------------------------------------
//
// typedef struct
//       {QString               m_Name;
//        C_DragQListViewItem  *m_pC_DragQListViewItem;
//       } PATIENT_ROOT_ITEM;

C_PatientRootItem *C_VigieListView::GetPatientRootItem(const QString &pk_Doss)
{ PK_PATIENT_MAP::Iterator it = m_PatientPk.find(pk_Doss);
  if (it != m_PatientPk.end()) return &it.data();
  return 0;
}

//-------------------------------- On_DirectoryGlossaireChanged ----------------------------------------------------
void C_VigieListView::On_DirectoryGlossaireChanged(const QString &pathDocuments, const QString &defaultMaskDirectory)
{if (defaultMaskDirectory == m_DefaultMaskDirectory) m_PathDocuments = pathDocuments;
}

//----------------------------------------------- eventFilter ---------------------------------------------------
bool C_VigieListView::eventFilter ( QObject * o, QEvent * e )
{if (e->type ()==QEvent::Enter) mouseEnter ( );  //Enter = 10, Leave = 11
 if (e->type ()==QEvent::Leave) mouseLeave ( );  //Enter = 10, Leave = 11
 return C_DragQListView::eventFilter(o,e);
}

//----------------------------------------------- On_DragMoveEvent ------------------------------------------------------------------------
void C_VigieListView::DoConnexionOnGossaire(C_FormGlossaire *pC_FormGlossaire)
{if (pC_FormGlossaire==0) return;
 connect( pC_FormGlossaire, SIGNAL(Sign_DirectoryChanged(const QString &, const QString &)),  this,   SLOT(  On_DirectoryGlossaireChanged(const QString &, const QString &)) );
 connect( this, SIGNAL(Sign_MouseEnter(QString &, QString &)),  pC_FormGlossaire,         SLOT(  On_Sign_MouseEnter(QString &, QString &)) );
 connect( this, SIGNAL(Sign_MouseLeave(QString &, QString &)),  pC_FormGlossaire,         SLOT(  On_Sign_MouseLeave(QString &, QString &)) );
}

//----------------------------------------------- mouseMoveEvent ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris rentre dans la rubrique
 */
void C_VigieListView::mouseEnter ( )
{emit Sign_MouseEnter (m_PathDocuments, m_DefaultMaskDirectory);
}

//----------------------------------------------- mouseLeave ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris sort de la rubrique
 */
void C_VigieListView::mouseLeave ( )
{emit Sign_MouseLeave (m_PathDocuments, m_DefaultMaskDirectory);
}

//-------------------------------- CreatePatientRootItem ----------------------------------------------------
// typedef struct
//       {QString               m_Name;
//        C_DragQListViewItem  *m_pC_DragQListViewItem;
//       } PATIENT_ROOT_ITEM;

C_PatientRootItem *C_VigieListView::CreatePatientRootItem(const QString &pk_Doss, bool mustBeOpenClose /*=TRUE*/)
{QString nom, prenom, guid;
 pMB->GetPatientNomPrenomPk(mustBeOpenClose, pk_Doss,  &nom, &prenom, &guid);
 //QVariant qvar;
 //pMB->GetFieldValue(pMB->m_DOSS_IDENT_TBL_NAME, pMB->m_DOSS_IDENT_NSS, pMB->m_DOSS_IDENT_REF_PK , G_pCApp->m_pDrTux->m_ID_Doss, &qvar);
 //QDate qDteNss = qvar.toDate();
 //QString age =  pMB->CalculeDiffAge(&qDteNss);
 //C_PatientRootItem cMachinPk_nom (nom, prenom, guid, pk_Doss, age, this);       // cree l'item de la QListView et le stocke dans m_pC_DragQListViewItem
 C_PatientRootItem cMachinPk_nom (nom, prenom, guid, pk_Doss, this);       // cree l'item de la QListView et le stocke dans m_pC_DragQListViewItem
 return &(m_PatientPk.insert(pk_Doss, cMachinPk_nom).data());
}

//-------------------------------- FindOrCreatePatientRootItem ----------------------------------------------------
C_PatientRootItem *C_VigieListView::FindOrCreatePatientRootItem(const QString &pk_Doss, bool mustBeOpenClose /*=TRUE*/)
{ //....................... chercher si pas deja cree ...................................................
  C_PatientRootItem *pC_PatientRootItem = GetPatientRootItem(pk_Doss);
  //....................... le creer eventuellement ...................................................
  if (pC_PatientRootItem)   return pC_PatientRootItem;
  return CreatePatientRootItem(pk_Doss, mustBeOpenClose);
}

//===============================================================  C_Organiseur =========================================================================
 /*! \class C_Organiseur
 *  \brief Formulaire de la vigie
*/
C_Organiseur::C_Organiseur(EVNT_LIST *pEvnmtList, RUBREC_LIST *pRubList, const QString &dossPk, QWidget * parent /* = 0 */, const char * name /*= 0*/, WFlags f /*= 0*/)
 :Form_Organiseur(parent, name, f), m_pEvnmtList(pEvnmtList), m_pRubList(pRubList), m_DossPk(dossPk)
{   QString value;
    m_log                  = "";
    m_IsDragedFile         = FALSE;
    m_PathProfiles         = G_pCApp->m_PathGlossaire+"Vigie/Ressources.sys/";
    m_BaseProfils          = "";
    m_ClasseursAllVisibles = 0;
    m_LastImgPath          = "";

    //............. bouton de filtre actif ou pas .................
    checkBox_filtreActif-> setChecked (TRUE ) ;
    m_hierarchFilter = FALSE;
    if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"adm"))
       {checkBox_filtreActif->show();
       }
    else
       {checkBox_filtreActif->hide();
       }
    //............... rajouter un spliter horizontal et listview ...................................
    //m_splitter = new QSplitter( this, "m_splitterOrga" );
    //m_splitter->setOrientation( QSplitter::Vertical );

    m_ListView_MonitorPatient = new C_VigieListView( frame_ListView, "m_ListView_MonitorPatient" );

    //frame_ListViewLayout = new QGridLayout( frame_ListView, 1, 1, 0, 0, "frame_ListViewLayout");

    //listView2 = new QListView( frame_ListView, "listView2" );
    //listView2->addColumn( tr( "Column 1" ) );
    //listView2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)2, 0, 0, listView2->sizePolicy().hasHeightForWidth() ) );
    listView2->hide();
    frame_ListViewLayout->addWidget( m_ListView_MonitorPatient, 0, 0 );



    //m_ListView_MonitorPatient->addColumn( tr( "Patient" ) );
    m_ListView_MonitorPatient->addColumn( tr( "libelle" ) );
    m_ListView_MonitorPatient->addColumn( tr( " " ) );
    m_ListView_MonitorPatient->addColumn( tr( " " ) );
    m_ListView_MonitorPatient->addColumn( tr( "       " ) );
    m_ListView_MonitorPatient->addColumn( tr( "       " ) );
    m_ListView_MonitorPatient->addColumn( tr( "       " ) );
    m_ListView_MonitorPatient->addColumn( tr( "       " ) );
    m_ListView_MonitorPatient->addColumn( tr( "       " ) );
    m_ListView_MonitorPatient->addColumn( tr( ">" ) );
    m_ListView_MonitorPatient->addColumn( tr( "Date de début" ) );
    m_ListView_MonitorPatient->addColumn( tr( "Date de fin" ) );
    m_ListView_MonitorPatient->addColumn( tr( "<" ) );
    //m_ListView_MonitorPatient->addColumn( tr( "pkDoss" ) );
    //m_ListView_MonitorPatient->addColumn( tr( "pkDoc" ) );
    //m_ListView_MonitorPatient->addColumn( tr( "pkEvnt" ) );


    m_ListView_MonitorPatient->setMaximumSize( QSize( 32767, 32767 ) );
    m_ListView_MonitorPatient->setMouseTracking( FALSE );
    m_ListView_MonitorPatient->setAcceptDrops( TRUE );
    m_ListView_MonitorPatient->setFrameShape( QListView::StyledPanel );
    m_ListView_MonitorPatient->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    m_ListView_MonitorPatient->setFrameShadow( QListView::Sunken );
    m_ListView_MonitorPatient->setRootIsDecorated( TRUE );
    m_ListView_MonitorPatient->setAllColumnsShowFocus ( TRUE );
   //m_ListView_MonitorPatient->setState(TO_DEBUG);
   // Form_OrganiseurQFrameLayout->addLayout( layout16, 1, 1 );
   //frame_ListView
   //Form_OrganiseurQFrameLayout->addWidget( m_ListView_MonitorPatient, 1, 2 );
  connect( comboBox_Filtre,                SIGNAL( activated ( const QString &  )  ),                         this, SLOT( makeListeMonitor(const QString &) ) );
  connect( pushButtonRaffraichir,          SIGNAL( clicked() ),                                               this, SLOT( makeListeMonitor() ) );
  connect( pushButtonDateDeb ,             SIGNAL( clicked() ),                                               this, SLOT( pushButtonDateDeb_clicked() ) );
  connect( pushButtonDateFin ,             SIGNAL( clicked() ),                                               this, SLOT( pushButtonDateFin_clicked() ) );
  connect( m_ListView_MonitorPatient,      SIGNAL( Sign_ItemClicked( QListViewItem *)),                       this, SLOT( On_Sign_ItemClicked(QListViewItem* ) ) );
  connect( m_ListView_MonitorPatient,      SIGNAL( clicked( QListViewItem *, const QPoint & , int )),         this, SLOT( On_ItemClicked(QListViewItem*, const QPoint &, int ) ) );
  connect( but_ZoomPlus ,                  SIGNAL( clicked() ),                                               this, SLOT( On_but_ZoomPlus_clicked() ) );
  connect( but_ZoomMinus ,                 SIGNAL( clicked() ),                                               this, SLOT( On_but_ZoomMinus_clicked() ) );
  connect( but_ZoomDefault ,               SIGNAL( clicked() ),                                               this, SLOT( On_but_ZoomDefault_clicked() ) );
  connect( but_SaveProfile ,               SIGNAL( clicked() ),                                               this, SLOT( On_but_SaveProfile_clicked() ) );
  connect( but_LoadProfile ,               SIGNAL( clicked() ),                                               this, SLOT( On_but_LoadProfile_clicked() ) );
  connect( checkBox_filtreActif ,          SIGNAL( clicked() ),                                               this, SLOT( On_checkBox_filtreActif_clicked() ) );
  connect( but_ConfFiltre ,                SIGNAL( clicked() ),                                               this, SLOT( On_but_ConfFiltre_clicked() ) );
  connect( but_ConfFiltre_Plus ,           SIGNAL( clicked() ),                                               this, SLOT( On_but_ConfFiltre_Plus_clicked() ) );
  connect( but_ConfFiltre_Moins ,          SIGNAL( clicked() ),                                               this, SLOT( On_but_ConfFiltre_Moins_clicked() ) );
  connect( but_ConfFiltre_HideShow,        SIGNAL( clicked() ),                                               this, SLOT( On_but_ConfFiltre_HideShow() ) );
  connect( m_ListView_MonitorPatient,      SIGNAL(Sign_ItemDraged_On_AnotherItem(QListViewItem *, QListViewItem  *,       const QString&)),
                                this,      SLOT(    On_ItemDraged_On_AnotherItem(QListViewItem *, QListViewItem  *,       const QString&)) );
  connect( m_ListView_MonitorPatient,      SIGNAL(Sign_DragMoveEvent(QListViewItem *, bool&)),
                                this,      SLOT(    On_DragMoveEvent(QListViewItem *, bool&)) );
  connect( m_ListView_MonitorPatient,      SIGNAL(Sign_FilesDraged_On_Item(QListViewItem *,             QStrList &,       const QString&)),
                                this,      SLOT(    On_FilesDraged_On_Item(QListViewItem *,             QStrList &,       const QString&)) );
  connect( m_ListView_MonitorPatient,      SIGNAL(Sign_TextDraged_On_Item(QListViewItem *,        const QString  &,       const QString&)),
                                this,      SLOT(    On_TextDraged_On_Item(QListViewItem *,        const QString  &,       const QString&)) );
  connect( m_ListView_MonitorPatient,      SIGNAL(contextMenuRequested ( QListViewItem * , const QPoint & , int  )),
                                this,      SLOT(    On_ContextMenuRequested(QListViewItem * , const QPoint & , int)) );

  m_ToDeletePixmap        =   Theme::getIcon("16x16/Vigie_annule.png") ;
  m_ToDestroyPixmap       =   Theme::getIcon("16x16/Vigie_annuleFic.png") ;
  m_IsClasseurPixmap      =   Theme::getIcon("16x16/IsClasseur.png") ;
  m_IsMenuHerePixmap      =   Theme::getIcon("16x16/IsMenuHere.png") ;
  m_DateDeb               =   Theme::getIcon("16x16/calendrierDeb.png");
  m_DateFin               =   Theme::getIcon("16x16/calendrierFin.png");
  but_ZoomPlus->setPixmap               (Theme::getIcon("16x16/viewmag+.png"));
  but_ZoomMinus->setPixmap              (Theme::getIcon("16x16/viewmag-.png"));
  but_ZoomDefault->setPixmap            (Theme::getIcon("16x16/viewmag1.png"));
  but_SaveProfile->setPixmap            (Theme::getIcon("16x16/SaveProfile.png"));
  but_LoadProfile->setPixmap            (Theme::getIcon("16x16/LoadProfile.png"));
  but_ConfFiltre->setPixmap             (Theme::getIcon("16x16/Filtre.png"));
  but_ConfFiltre_Plus->setPixmap        (Theme::getIcon("16x16/FiltrePlus.png"));
  but_ConfFiltre_Moins->setPixmap       (Theme::getIcon("16x16/FiltreMoins.png"));
  pushButtonDateDeb->setIconSet( Theme::getIconSet("16x16/calendrierDeb.png"));
  pushButtonDateFin->setIconSet( Theme::getIconSet("16x16/calendrierFin.png"));
  mle_deb->setText(QDate::currentDate().toString("dd/MM/yyyy"));
  mle_fin->setText(QDate::currentDate().toString("dd/MM/yyyy"));
  mle_deb->setText("01/01/2000");
  mle_HeureDeb->setText("00:00");
  mle_HeureFin->setText("23:59");

  QString xml = "";
  //............. charger les profils liés à l'utilisateur .......................
  m_BaseProfils = loadProfile(m_PathProfiles+"Profiles/VigieProfile.txt", 1);  // doit etre dans le glossaire avec les classeurs concernes  et non dans le profil user
  /*
  READ_USER_PARAM(USER_PARAM, "Dock_Vigie", tr("Profil"), &xml);
  if (xml.length())
     {CGestIni::retrieveStringForParam(xml);
      SetProfile(xml);
     }
  xml = "";
  */
  READ_USER_PARAM(USER_PARAM,  "Dock_Vigie", tr("Filtres"), &xml);
  if (xml.length())
     {CGestIni::retrieveStringForParam(xml);
      SetFilter(xml);
     }
  xml = "";
  //.............. checkbox activant ou pas les filtres hierarchiques .....................
  READ_USER_PARAM(USER_PARAM, "Dock_Vigie", tr("HierarchFilter"), &xml);
  xml = xml.upper();
  if (xml[0]=='O') {checkBox_filtreActif-> setChecked (TRUE );  m_hierarchFilter=TRUE;}
  else             {checkBox_filtreActif-> setChecked (FALSE ); m_hierarchFilter=FALSE;}
  //.............. combo des filtres hierarchiques par defaut .....................
  xml = "";
  READ_USER_PARAM(USER_PARAM, "Dock_Vigie", tr("DefaultHierarchFilter"), &xml);
  if (xml.length())
     {QListBoxItem *pQListBoxItem = comboBox_FiltreVisu->listBox()->findItem ( xml, Qt::ExactMatch );
      if (pQListBoxItem)
         { int index  = comboBox_FiltreVisu->listBox()->index (pQListBoxItem);
           if (index != -1) comboBox_FiltreVisu->setCurrentItem ( index);
         }
     }
  else
     {comboBox_FiltreVisu->setCurrentItem(0);
     }
  //.............. combo des filtres hierarchiques par defaut .....................
  frame_ConfigHeader->show();
  xml = "";
  READ_USER_PARAM(USER_PARAM, "Dock_Vigie", tr("Tools visibility"), &xml);
  if (xml.length())
     {if (xml.lower()[0]=='n') frame_ConfigHeader->hide();
     }
  //.............. combo de tous les patients ou celui en cours .....................
  /*
  xml = "";
  READ_USER_PARAM(USER_PARAM,  "Dock_Vigie", tr("ListViewMode"), &xml);
  if (xml.length())
     {QListBoxItem *pQListBoxItem = comboBox_Filtre->listBox()->findItem ( xml, Qt::ExactMatch );
      if (pQListBoxItem)
         { int index  = comboBox_Filtre->listBox()->index (pQListBoxItem);
           if (index != -1) comboBox_Filtre->setCurrentItem ( index);
         }
     }
  */
  comboBox_Filtre->setCurrentItem (1);

  connect( comboBox_FiltreVisu, SIGNAL( activated ( const QString &  )  ), this, SLOT( On_comboBox_FiltreVisu_activated(const QString &) ) );
  if (comboBox_FiltreVisu->count()) {//comboBox_FiltreVisu->setCurrentItem(0);
                                     On_comboBox_FiltreVisu_activated(comboBox_FiltreVisu->currentText());
                                    }
  else                              { makeListeMonitor();
                                    }
  //.............. activer le patient en cours ........................................
  setListviewOnDossier(dossPk);
  m_ListView_MonitorPatient->setMouseTracking ( TRUE );

  //.............. activer le patient en cours ........................................
  CGestIni::Param_ReadParam(USER_PARAM, "Dock_Vigie", "FontSize", &value );
  if (value.length())
     {QFont f = font();
      f.setPointSize( value.toInt() );
      setFont( f );
      Set_ZoomFontSize(value.toInt());
     }
}
//-------------------------------- C_Organiseur ----------------------------------------------------
 C_Organiseur::~C_Organiseur()
{
};
//-------------------------------- On_but_ConfFiltre_HideShow ----------------------------------------------------
void C_Organiseur::On_but_ConfFiltre_HideShow()
{ QString mode = tr("non");
  if (frame_ConfigHeader->isVisible()) {frame_ConfigHeader->hide();  mode = tr("non");}
  else                                 {frame_ConfigHeader->show();  mode = tr("oui");}
  WRITE_USER_PARAM(&USER_PARAM, "Dock_Vigie", tr("Tools visibility"), mode);
}

//-------------------------------- initialisePathGlossaire ----------------------------------------------------
void C_Organiseur::initialisePathGlossaire(const QString &pathDocuments, const QString &defaultMaskDirectory)
{m_ListView_MonitorPatient->initialisePathGlossaire(pathDocuments, defaultMaskDirectory);
}

//----------------------------------------------- setListviewOnDossier ------------------------------------------------------------------------
void C_Organiseur::setListviewOnDossier(const QString &dossPk)
{ C_DragQListViewItem* pC_DragQListViewItem = GetListViewItemROOTFromPkDoss( dossPk);
  if (pC_DragQListViewItem)
     {m_ListView_MonitorPatient->setSelected ( pC_DragQListViewItem, TRUE );
      m_ListView_MonitorPatient->setCurrentItem ( pC_DragQListViewItem );
      m_ListView_MonitorPatient->ensureItemVisible ( pC_DragQListViewItem );
     }
}

//----------------------------------------------- setIniParamFromContext ------------------------------------------------------------------------
void C_Organiseur::SetInterfaceOnDroits(const QString &droits)
{  if (!G_pCApp->IsThisDroitExist(droits,"adm"))
     {  but_SaveProfile->hide();
        but_LoadProfile->hide();
        but_ConfFiltre->hide();
        but_ConfFiltre_Plus->hide();
        but_ConfFiltre_Moins->hide();
        checkBox_filtreActif->hide();
     }
   else
     {  but_SaveProfile->show();
        but_LoadProfile->show();
        but_ConfFiltre->show();
        but_ConfFiltre_Plus->show();
        but_ConfFiltre_Moins->show();
        checkBox_filtreActif->show();
     }
}

//----------------------------------------------- setIniParamFromContext ------------------------------------------------------------------------
void C_Organiseur::setIniParamFromContext(QString &param)
{QString xml = GetProfile();
 CGestIni::formatStringForParam(xml);
 CGestIni::Param_WriteParam(&param, "Dock_Vigie", tr("Profil"), xml);
 xml = GetFilter();
 CGestIni::formatStringForParam(xml);
 CGestIni::Param_WriteParam(&param, "Dock_Vigie", tr("Filtres"), xml);
 if (checkBox_filtreActif->isChecked())  xml = "Ok";
 else                                    xml = "Non";
 CGestIni::Param_WriteParam(&param, "Dock_Vigie", tr("HierarchFilter"), xml);
 CGestIni::Param_WriteParam(&param, "Dock_Vigie", tr("DefaultHierarchFilter"), comboBox_FiltreVisu->currentText());
 CGestIni::Param_WriteParam(&param, "Dock_Vigie", tr("ListViewMode"),          comboBox_Filtre->currentText());
}

//----------------------------------------------- saveUserProfil ------------------------------------------------------------------------
void C_Organiseur::saveUserProfil()
{
 QString  xml = "";
 xml = GetProfile();
 CGestIni::formatStringForParam(xml);
 WRITE_USER_PARAM(&USER_PARAM,  "Dock_Vigie", tr("Profil"), xml);
 UPDATE_USER_PARAM(&USER_PARAM, G_pCApp->m_User);
 saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");  // sutout le placer en local dans les classeurs concern� de la vigie
}
//----------------------------------------------- On_but_ConfFiltre_clicked ------------------------------------------------------------------------
void C_Organiseur::On_comboBox_FiltreVisu_activated(const QString &str)
{QString xml          = getFilterValue(str, "authorized");
 m_FiltreHierarchList = QStringList::split("\n",xml);
 makeListeMonitor();
}

//----------------------------------------------- On_but_ConfFiltre_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_ConfFiltre_clicked()
{if (comboBox_FiltreVisu->count () == 0)
    {CouCou(tr("Liste de filtres vide, veuillez en créer un en cliquant sur le bouton 'Ajouter un filtre'"));
     return;
    }
 QString oldName    = comboBox_FiltreVisu->currentText();
 QString filterName = comboBox_FiltreVisu->currentText();
 QString  value     =  getFilterValue(filterName, "authorized");
 C_Dlg_VigieFiltreConf *dlg = new C_Dlg_VigieFiltreConf(&filterName, &value);
 if (dlg == 0) return;
 if (dlg->exec()==QDialog::Accepted)
    {if      (oldName==filterName)
             {if (value.length()) setFilterValue(filterName, "authorized", value);
             }
     else if (comboBox_FiltreVisu->listBox()-> findItem (filterName, Qt::ExactMatch )==0)
             {if (value.length() && filterName.length()) setFilterValue(filterName, "authorized", value);
              comboBox_FiltreVisu->insertItem(filterName);
             }
     else CouCou(tr("Ce nom existe déjà veuillez en choisir un autre"));
    }
 delete dlg;
}

//----------------------------------------------- On_but_ConfFiltre_Plus_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_ConfFiltre_Plus_clicked()
{QString filterName = "";
 QString  value     = "";
 C_Dlg_VigieFiltreConf *dlg = new C_Dlg_VigieFiltreConf(&filterName, &value);
 if (dlg == 0) return;
 if (dlg->exec()==QDialog::Accepted)
    {
     if (filterName.length() && comboBox_FiltreVisu->listBox()->findItem (filterName, Qt::ExactMatch )==0 )
        { setFilterValue(filterName, "authorized", value);
          comboBox_FiltreVisu->insertItem(filterName);
          QListBoxItem *pQListBoxItem = comboBox_FiltreVisu->listBox()->findItem (filterName, Qt::ExactMatch );
          if (pQListBoxItem)
             { int id =  comboBox_FiltreVisu->listBox()->index( pQListBoxItem );
               comboBox_FiltreVisu->setCurrentItem ( id );
             }
        }
    }
 delete dlg;
}
//----------------------------------------------- On_but_ConfFiltre_Moins_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_ConfFiltre_Moins_clicked()
{if (comboBox_FiltreVisu->count () == 0)
    {CouCou(tr("Opération impossible car la liste des filtres est vide"));
     return;
    }
 QString filterName = comboBox_FiltreVisu->currentText();
 if ( QMessageBox::warning( this, tr(" Destruction d'un filtre de classeurs"),
                                  tr ( " La destruction de ce filtre de classeurs : <br><b>") +
                                  filterName                                                  +
                                  tr(  "</b><br> sera <u>DÉFINITIVE</u><br>"
                                  " Confirmez-vous la destruction ?<br>") ,
                                  tr("&Détruire"), tr("Annu&ler"), 0,
                                  1, 1 ) == 0)
          { delFilter(filterName);
            QListBoxItem *pQListBoxItem  = comboBox_FiltreVisu->listBox()->findItem (filterName, Qt::ExactMatch );
            if (pQListBoxItem)
               {int id  =  comboBox_FiltreVisu->listBox()->index ( pQListBoxItem );
                if (id != -1)
                   {comboBox_FiltreVisu->removeItem ( id ) ;
                    if (id >= comboBox_FiltreVisu->count()) --id;
                    comboBox_FiltreVisu->setCurrentItem ( id );
                   }
                if (comboBox_FiltreVisu->count())
                   {On_comboBox_FiltreVisu_activated(comboBox_FiltreVisu->currentText ());
                   }
                else
                  {m_FiltreHierarchList.clear();
                   makeListeMonitor();
                  }
               }
          }
}

//----------------------------------------------- On_checkBox_filtreActif_clicked ------------------------------------------------------------------------
void C_Organiseur::On_checkBox_filtreActif_clicked()
{if (checkBox_filtreActif->isChecked())  m_hierarchFilter = TRUE;
 else                                    m_hierarchFilter = FALSE;
 makeListeMonitor();
}

//----------------------------------------------- On_but_SaveProfile_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_SaveProfile_clicked()
{QString s = QFileDialog::getSaveFileName(
                    m_PathProfiles+"Profiles",
                    "Profils (*.xml *.txt)",
                    this,
                    tr("save file dialog"),
                    tr("Choose a filename to save under") );
 if ( s.length() )
    {saveProfile(s);
    }
}
//----------------------------------------------- On_but_LoadProfile ------------------------------------------------------------------------
void C_Organiseur::On_but_LoadProfile_clicked()
{QString s = QFileDialog::getOpenFileName(
                    m_PathProfiles+"Profiles",
                    "Profils (*.xml *.txt)",
                    this,
                    tr("open file dialog"),
                    tr("Choose a file") );
 if ( s.length() )
    {if (QFile::exists(s)) loadProfile(s);
     else                  CouCou(tr("Erreur C_Organiseur::On_but_SaveProfile_clicked() 'Fichier non trouvé : '")+ s);
    }
}

//----------------------------------------------- GetPatientRootItem ------------------------------------------------------------------------
C_PatientRootItem *C_Organiseur::GetPatientRootItem(const QString &pk_Doss)
{return m_ListView_MonitorPatient->GetPatientRootItem(pk_Doss);
}

//----------------------------------------------- On_DragMoveEvent ------------------------------------------------------------------------
void C_Organiseur::DoConnexionOnGossaire(C_FormGlossaire *pC_FormGlossaire)
{m_ListView_MonitorPatient->DoConnexionOnGossaire(pC_FormGlossaire);
 connect( this, SIGNAL(Sign_MouseEnter(QString &, QString &)),  pC_FormGlossaire, SLOT(  On_Sign_MouseEnter(QString &, QString &)) );
}

//----------------------------------------------- On_DragMoveEvent ------------------------------------------------------------------------
void C_Organiseur::On_DragMoveEvent(QListViewItem *pQListViewItem, bool &dragBreak)
{if (pQListViewItem==0) return;
 if (pQListViewItem-> listView () == m_ListView_MonitorPatient)
    {C_DragQListViewItem *pC_DragQListViewItem = (C_DragQListViewItem *) pQListViewItem;
     QString itemDossPk = pC_DragQListViewItem->GET_PK_DOS();
     if ( itemDossPk != m_DossPk)
        {dragBreak = TRUE;
        }
    }
}

//----------------------------------------------- On_but_ZoomPlus_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_ZoomPlus_clicked()
{ QFont f = font();
  int   s = f.pointSize() + 1;
  f.setPointSize( s ) ;
  setFont( f );
  Set_ZoomFontSize(s);
  WRITE_USER_PARAM  (&USER_PARAM, "Dock_Vigie", "FontSize", QString::number(s));
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User );
}

//----------------------------------------------- On_but_ZoomMinus_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_ZoomMinus_clicked()
{ QFont f = font();
  int   s = f.pointSize() - 1;
  f.setPointSize( s ) ;
  setFont( f );
  Set_ZoomFontSize(s);
  WRITE_USER_PARAM  (&USER_PARAM, "Dock_Vigie", "FontSize", QString::number(s));
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User );
}

//----------------------------------------------- On_but_ZoomPlus_clicked ------------------------------------------------------------------------
void C_Organiseur::On_but_ZoomDefault_clicked()
{ QFont f = font();
  f.setPointSize( 9 ) ;
  setFont( f );
  Set_ZoomFontSize(9);
  WRITE_USER_PARAM  (&USER_PARAM, "Dock_Vigie", "FontSize", QString::number(9));
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User );
}

//----------------------------------------------- Set_ZoomFontSize ------------------------------------------------------------------------
void C_Organiseur::Set_ZoomFontSize(int size)
{QFont f;
 f = textLabel_Du->font();              f.setPointSize( size ); textLabel_Du->setFont(f);
 f = textLabel_De->font();              f.setPointSize( size ); textLabel_De->setFont(f);
 f = checkBox_filtreActif->font();      f.setPointSize( size ); checkBox_filtreActif->setFont(f);
 f = textLabel_Au->font();              f.setPointSize( size ); textLabel_Au->setFont(f);
 f = textLabel_a->font();               f.setPointSize( size ); textLabel_a->setFont(f);
 f = comboBox_FiltreVisu->font();       f.setPointSize( size ); comboBox_FiltreVisu->setFont(f);
 f = comboBox_Filtre->font();           f.setPointSize( size ); comboBox_Filtre->setFont(f);
 f = m_ListView_MonitorPatient->font(); f.setPointSize( size ); m_ListView_MonitorPatient->setFont(f);
 f = m_ListView_MonitorPatient->header()->font(); f.setPointSize( size ); m_ListView_MonitorPatient->header()->setFont(f);
 f = pushButtonRaffraichir->font();     f.setPointSize( size ); pushButtonRaffraichir->setFont(f);
}

//----------------------------------------------- SetListViewItemFromRecord ---------------------------------------------------
/*! \brief Fonction qui remplie les colonnes d'un item de la listview avec un enregistrement de la liste d'evennements pEvnmtList
 *  \param pQListViewItem  pQListViewItem* :  un pointeur sur l'item de la listview a renseigner
 *  \param pCRubRecord     CRubRecord* :  un pointeur sur l'enregistrement la liste des documents avec lequel renseigner l'item
 *  \param pCRubEvntPair   CRubEvntPair* :  un pointeur sur  l'enregistrement de la liste d'evennements avec lequel renseigner l'item
 *  \param pQListViewItemPere   C_DragQListViewItem : sur l'item du pere
 *  \return neant
*/
void C_Organiseur::SetListViewItemFromRecord(C_DragQListViewItem *pQListViewItem, CRubRecord *pCRubRecord , CRubEvntPair *pCRubEvntPair)
{ if (pQListViewItem==0 || pCRubRecord==0) return;
  //pQListViewItem->setDragEnabled ( TRUE );
  int i = 0;
  pQListViewItem->setText(i++, pCRubRecord->m_Libelle);    // Evnmt_Type  8
  pQListViewItem->setText(i++, "");                        // icone IsUnique
  pQListViewItem->setText(i++, "");                        // icone effacement/valide
  pQListViewItem->setText(i++, pCRubRecord->m_Prop_0);     // Evnmt_SubType
  pQListViewItem->setText(i++, pCRubRecord->m_Prop_1);     // Evnmt_SubType
  pQListViewItem->setText(i++, pCRubRecord->m_Prop_2);     // Evnmt_SubType
  pQListViewItem->setText(i++, pCRubRecord->m_Prop_3);     // Evnmt_SubType
  pQListViewItem->setText(i++, pCRubRecord->m_Prop_4);     // Evnmt_SubType
  i++;
  pQListViewItem->setText(i++, QueryDateHeureToNatural(pCRubRecord->m_Date)); //pCRubRecord->m_Date.replace("T",tr(" à ")));         // Evnmt_DateTimeDeb
  pQListViewItem->setText(i++, QueryDateHeureToNatural(pCRubRecord->m_Fin)); //pCRubRecord->m_Fin.replace("T",tr(" à ")));         // Evnmt_DateTimeFin
  //pQListViewItem->setText(i++, pCRubEvntPair->m_DossPk);         // Evnmt_DateTimeFin
  //pQListViewItem->setText(i++, pCRubRecord->m_PrimKey);         // Evnmt_DateTimeFin
  //pQListViewItem->setText(i++, pCRubEvntPair->m_Pk);         // Evnmt_DateTimeFin
  //........... stocker hors affichage ...............................
  pQListViewItem->SET_PK_EVNT(pCRubEvntPair->m_Pk);
  pQListViewItem->SET_PK_DOC(pCRubRecord->m_PrimKey);
  pQListViewItem->SET_PK_DOS(pCRubEvntPair->m_DossPk);
  pQListViewItem->setOpen(FALSE);
  //............ visiter le profil pour parametrer l'item ...........
  setItemOnProfile(pQListViewItem);
  //............ traiter l'attribut d'éfacement .....................
  if (pCRubEvntPair->IsStatePlaced(TO_DELETE))
     {if (pCRubEvntPair->IsStatePlaced(TO_REMOVE))
         pQListViewItem->setPixmap ( 2, m_ToDestroyPixmap );
      else
         pQListViewItem->setPixmap ( 2, m_ToDeletePixmap );
     }
  else
     {pQListViewItem->setPixmap ( 2, 0);
     }
  pQListViewItem->setPixmap ( DT_Deb-1,   m_DateDeb);
  pQListViewItem->setPixmap ( DT_Fin+1,   m_DateFin);
  pQListViewItem->setState(IS_CLASSEUR);
}

//----------------------------------------------- QueryDateHeureToNatural ------------------------------------------------------------------------
QString C_Organiseur::QueryDateHeureToNatural(const QString &date)
{if (date.length()==0) return QString::null;
 QString text = C_TokenInterpret::toNum(date,"$allString");
 return        QString(" ") + text.mid(6,2) +"-"+text.mid(4,2)+"-"+text.mid(0,4) + tr(" à ") + text.mid(8,2)+"h"+text.mid(10,2);
}

//----------------------------------------------- NaturalDateHeureToQuery ------------------------------------------------------------------------
QString C_Organiseur::NaturalDateHeureToQuery(const QString &date)
{if (date.length()==0) return QString::null;
 QString text = C_TokenInterpret::toNum(date,"$allString");
 return        text.mid(4,4) + text.mid(2,2)+text.mid(0,2) + "T" + text.mid(8);
}

//----------------------------------------------- setItemOnProfile ------------------------------------------------------------------------
void C_Organiseur::setItemOnProfile(C_DragQListViewItem *pQListViewItem)
{ QString name = pQListViewItem->text(0);

  OpClProfiles::Iterator it;
  it = m_UserClasseurProfiles.find(name);
  if (it != m_UserClasseurProfiles.end())
     {QString dataToExamine = it.data();
      QString   value       = "";
        int       pos         = 0;
        pQListViewItem->setOpen(FALSE);
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<isOpen>", value, pos))!=-1 )
           {if (value=="1") pQListViewItem->setOpen(TRUE);
           }
        else pos = 0;
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<icone>", value, pos))!=-1 )
           { pQListViewItem->setPixmap ( 0, m_PathProfiles + "Icones/"+value );
           }
        else pos = 0;
        pQListViewItem->setPixmap ( 1,  0); //m_PathProfiles + "Icones/IsMulti.png"
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<IsUnique>", value, pos))!=-1 )
           { if (value=="1") pQListViewItem->setPixmap ( 1, m_IsClasseurPixmap );
           }
        else pos = 0;
        pos = 0;
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<menuProp0>", value, pos))!=-1 )
           { if (value.length()) pQListViewItem->setPixmap ( CP_Deb, m_IsMenuHerePixmap );
             else                pQListViewItem->setPixmap ( CP_Deb, 0 );
           }
        else pos = 0;
        pos = 0;
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<menuProp1>", value, pos))!=-1 )
           { if (value.length()) pQListViewItem->setPixmap ( CP_Deb+1, m_IsMenuHerePixmap );
             else                pQListViewItem->setPixmap ( CP_Deb+1, 0 );
           }
        else pos = 0;
        pos = 0;
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<menuProp2>", value, pos))!=-1 )
           { if (value.length()) pQListViewItem->setPixmap ( CP_Deb+2, m_IsMenuHerePixmap );
             else                pQListViewItem->setPixmap ( CP_Deb+2, 0 );
           }
        else pos = 0;
        pos = 0;
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<menuProp3>", value, pos))!=-1 )
           { if (value.length()) pQListViewItem->setPixmap ( CP_Deb+3, m_IsMenuHerePixmap );
       else                pQListViewItem->setPixmap ( CP_Deb+3, 0 );
     }
        else pos = 0;
        pos = 0;
        if ( (pos = CGenTools::GetXMLValue(dataToExamine, "<menuProp4>", value, pos))!=-1 )
           { if (value.length()) pQListViewItem->setPixmap ( CP_Deb+4, m_IsMenuHerePixmap );
             else                pQListViewItem->setPixmap ( CP_Deb+4, 0 );
           }
        else pos = 0;
     }
}

//----------------------------------------------- loadProfile ------------------------------------------------------------------------
QString C_Organiseur::loadProfile(const QString &path, int just_profile /*=0*/)
{QString xml_p = "";     // profil
 QString xml_f = "";     // filtres
 CGestIni::Param_UpdateFromDisk(path, xml_p );
 int pos = xml_p.find("<!DOCTYPE CFilter>");
 if (pos != -1)
    {xml_f = xml_p.mid(pos);
     xml_p.truncate(pos);
    }
 if (xml_f.length() && just_profile==0) SetFilter  (xml_f);
 if (xml_p.length() && just_profile==1) SetProfile (xml_p);
 return xml_p;
}

//----------------------------------------------- saveProfile ------------------------------------------------------------------------
void  C_Organiseur::saveProfile(const QString &path, int just_profile  /*=0*/)
{
 if (just_profile==0) CGestIni::Param_UpdateToDisk(path, GetProfile()+"\n"+GetFilter() );
 else                 CGestIni::Param_UpdateToDisk(path, GetProfile()+"\n");
}

//----------------------------------------------- GetProfile ------------------------------------------------------------------------
QString  C_Organiseur::GetProfile()
{if ( m_UserClasseurProfiles.isEmpty () ) return QString::null;
 QString xml = "<!DOCTYPE CVigie>\n<CVigie version=\"0.0\" stdsetdef=\"1\">\n";
 OpClProfiles::Iterator it;
 for ( it           = m_UserClasseurProfiles.begin(); it != m_UserClasseurProfiles.end(); ++it )
    {QString str   = it.data();
     QString value = "";
     xml += "\n  <Classeur name=\"" + it.key() + "\">";
     if ( CGenTools::GetXMLValue(str, "<isOpen>",    value, 0) !=-1 )  xml += "\n    <isOpen>"    + value + "</isOpen>";
     if ( CGenTools::GetXMLValue(str, "<icone>",     value, 0) !=-1 )  xml += "\n    <icone>"     + value + "</icone>";
     if ( CGenTools::GetXMLValue(str, "<IsUnique>",  value, 0) !=-1 )  xml += "\n    <IsUnique>"  + value + "</IsUnique>";
     if ( CGenTools::GetXMLValue(str, "<menuProp0>", value, 0) !=-1 )  xml += "\n    <menuProp0>" + value + "</menuProp0>";
     if ( CGenTools::GetXMLValue(str, "<menuProp1>", value, 0) !=-1 )  xml += "\n    <menuProp1>" + value + "</menuProp1>";
     if ( CGenTools::GetXMLValue(str, "<menuProp2>", value, 0) !=-1 )  xml += "\n    <menuProp2>" + value + "</menuProp2>";
     if ( CGenTools::GetXMLValue(str, "<menuProp3>", value, 0) !=-1 )  xml += "\n    <menuProp3>" + value + "</menuProp3>";
     if ( CGenTools::GetXMLValue(str, "<menuProp4>", value, 0) !=-1 )  xml += "\n    <menuProp4>" + value + "</menuProp4>";
     xml += "\n  </Classeur>\n";
    }
 xml += "\n</CVigie>";
return xml;
}

//----------------------------------------------- GetFilter ------------------------------------------------------------------------
QString  C_Organiseur::GetFilter()
{if ( m_FilterProfiles.isEmpty () ) return QString::null;
 QString xml = "<!DOCTYPE CFilter>\n<CFilter version=\"0.0\" stdsetdef=\"1\">\n";
 OpClProfiles::Iterator it;
 for ( it           = m_FilterProfiles.begin(); it != m_FilterProfiles.end(); ++it )
    {QString str   = it.data();
     QString value = "";
     xml += "\n  <Filter name=\"" + it.key() + "\">";
     if ( CGenTools::GetXMLValue(str, "<authorized>",    value, 0) !=-1 )  xml += "\n    <authorized>"    + value + "</authorized>";
     xml += "\n  </Filter>\n";
    }
 xml += "\n</CFilter>";
return xml;
}

//----------------------------------------------- delFilter ------------------------------------------------------------------------
void  C_Organiseur::delFilter(const QString &filterName)
{if ( m_FilterProfiles.isEmpty () ) return ;
 m_FilterProfiles.erase ( filterName );
}

//----------------------------------------------- SetFilter ------------------------------------------------------------------------
int  C_Organiseur::SetFilter(QString xml)
{//......... virer le dernier .................
 int pos = xml.findRev("</CFilter>");
 if (pos != -1) xml.truncate(pos);
 pos = xml.find("<Filter");
 if (pos != -1) xml = xml.mid(pos+7);
 QStringList listFilters = QStringList::split("<Filter", xml);
 m_FilterProfiles.clear();
 comboBox_FiltreVisu->clear();
 for ( QStringList::Iterator it = listFilters.begin(); it != listFilters.end(); ++it )
     { int          pos_deb      = 0;
       int          pos_end      = 0;
       QString filter_name     = "";
       QString filter (*it);
      //................. nom du classeur ............................................
      filter_name    = "";
      if ((pos_deb   = filter.find("=")) != -1)
         { ++pos_deb;
           if ((pos_end    = filter.find(">",pos_deb))!=-1)
              {filter_name = filter.mid(pos_deb,pos_end-pos_deb).stripWhiteSpace();
               filter_name = filter_name.remove('\"');
              }
         }
      //................. ajouter le profile à la QMap ................................
      if (filter_name.length()==0)
         {CouCou(tr("Erreur C_Organiseur::SetFilter() 'le nom du filtre n'a pas été trouvé'"));
         }
      m_FilterProfiles[filter_name] = filter;
      comboBox_FiltreVisu->insertItem(filter_name);
     }
 return m_FilterProfiles.count();
}

//----------------------------------------------- SetProfile ------------------------------------------------------------------------
int  C_Organiseur::SetProfile(QString xml)
{//......... virer le dernier .................
 int pos = xml.findRev("</CVigie>");
 if (pos != -1) xml.truncate(pos);
 pos = xml.find("<Classeur");
 if (pos != -1) xml = xml.mid(pos+9);

 QStringList listClasseurs = QStringList::split("<Classeur", xml);
 m_UserClasseurProfiles.clear();
 for ( QStringList::Iterator it = listClasseurs.begin(); it != listClasseurs.end(); ++it )
     { int          pos_deb      = 0;
       int          pos_end      = 0;
       QString classeur_name     = "";
       QString classeur (*it);
      //................. nom du classeur ............................................
      classeur_name  = "";
      if ((pos_deb   = classeur.find("=")) != -1)
         { ++pos_deb;
           if ((pos_end = classeur.find(">",pos_deb))!=-1)
              {classeur_name = classeur.mid(pos_deb,pos_end-pos_deb).stripWhiteSpace();
               classeur_name = classeur_name.remove('\"');
              }
         }
      //................. ajouter le profile à la QMap ................................
      if (classeur_name.length()==0)
         {CouCou(tr("Erreur C_Organiseur::SetProfile() 'le nom du classeur n'a pas été trouvé'"));
         }
      m_UserClasseurProfiles[classeur_name] = classeur;
     }
 return m_UserClasseurProfiles.count();
}

//----------------------------------------------- setProfileValue ------------------------------------------------------------------------
void C_Organiseur::setProfileValue(const QString &name, const QString &xmltag, const QString & value)
{ QString profile = "";
  //................. chercher si le profile de ce classeur existe .....................
  //                  si pas trouvé le créer
  OpClProfiles::Iterator it = m_UserClasseurProfiles.find(name);
  if (it == m_UserClasseurProfiles.end())
     {profile = " name=\""; profile += name + "\">\n";
     }
 else
     {profile = m_UserClasseurProfiles[name];
     }
 int        deb  = profile.find(QString("<")+xmltag+">");
 if (deb == -1)       // le tag n'a pas ete trouve donc on suppose qu'il n'existe pas on le cree
    {int    pos  = profile.findRev("</Classeur>");
     if (   pos != -1) profile.truncate(pos);           // couper juste apres "</Classeur>" pour faire propre
     pos  = profile.length();
     while (pos>0 && profile[pos-1]!='>') --pos;
     profile.truncate(pos);
     profile                  += QString("\n     <")+xmltag+">"+ value + "</"+xmltag+">\n  </Classeur>\n";
     m_UserClasseurProfiles[name] = profile;
     return;
    }
 deb += xmltag.length() + 2;
 int         fin = profile.find(QString("</")+xmltag+">");
 if (fin == -1)     return;
 profile = profile.replace(deb,fin-deb,value);
 m_UserClasseurProfiles[name] = profile;
}

//----------------------------------------------- getProfileValue ------------------------------------------------------------------------
QString C_Organiseur::getProfileValue(const QString &name, const QString &xmltag)
{OpClProfiles::Iterator it = m_UserClasseurProfiles.find(name);
 if (it == m_UserClasseurProfiles.end()) return QString::null;
 QString value;
 if ( CGenTools::GetXMLValue(m_UserClasseurProfiles[name], QString("<")+xmltag+">", value, 0) != -1 ) return value;
 return QString::null;
}

//----------------------------------------------- setFilterValue ------------------------------------------------------------------------
void C_Organiseur::setFilterValue(const QString &name, const QString &xmltag, const QString & value)
{ QString profile = "";
  //................. chercher si le profile de ce classeur existe .....................
  //                  si pas trouvé le créer
  OpClProfiles::Iterator it = m_FilterProfiles.find(name);
  if (it == m_FilterProfiles.end())
     {profile = " name=\""; profile += name + "\">\n";
     }
 else
     {profile = m_FilterProfiles[name];
     }
 int        deb  = profile.find(QString("<")+xmltag+">");
 if (deb == -1)       // le tag n'a pas ete trouve donc on suppose qu'il n'existe pas on le cree
    {int    pos  = profile.findRev("</Filter>");
     if (   pos != -1) profile.truncate(pos);           // couper juste apres "</Classeur>" pour faire propre
     pos  = profile.length();
     while (pos>0 && profile[pos-1]!='>') --pos;
     profile.truncate(pos);
     profile                  += QString("\n     <")+xmltag+">"+ value + "</"+xmltag+">\n  </Filter>\n";
     m_FilterProfiles[name] = profile;
     return;
    }
 deb += xmltag.length() + 2;
 int         fin = profile.find(QString("</")+xmltag+">");
 if (fin == -1)     return;
 profile = profile.replace(deb,fin-deb,value);
 m_FilterProfiles[name] = profile;
}

//----------------------------------------------- getFilterValue ------------------------------------------------------------------------
QString C_Organiseur::getFilterValue(const QString &name, const QString &xmltag)
{OpClProfiles::Iterator it = m_FilterProfiles.find(name);
 if (it == m_FilterProfiles.end()) return QString::null;
 QString value;
 if ( CGenTools::GetXMLValue(m_FilterProfiles[name], QString("<")+xmltag+">", value, 0) != -1 ) return value;
 return QString::null;
}

//----------------------------------------------- On_IdentChange ------------------------------------------------------------------------
/*! \brief Slot qui repond a la demande de changement d'identite
*/
void C_Organiseur::On_IdentChange(const char* /*dossGUID*/,  const char* dossPk, const char* /*dossNom*/,  const char* /*dossPrenom*/)
{m_DossPk = dossPk;
 m_pEvnmtList->clear();
 pMB->Evnmt_InitList( m_pEvnmtList,   m_DossPk);
 makeListeMonitor();
}

//----------------------------------------------- displayWarningItemInEraseState ------------------------------------------------------------------------
/*! \brief fonction qui affiche un message d'erreur lors de blocage car effacement positionne
*/
void C_Organiseur::displayWarningItemInEraseState()
{//QMessageBox::warning( this, "La vigie vous informe ...",
 //                                 tr("Opération impossible :\n"
 //                                    "Un des éléments impliqués ou un ascendant\n"
 //                                    "de cet élément est destiné à l'effacement.\n"
 //                                    "Veuillez remettre l'élément responsable de cet état,\n"
 //                                    "en état de non effacement.\n\n"
 //                                   ),
 //                                 tr("Annuler"), 0, 0, 0, 1 );
 CouCou(tr("Opération impossible :\n"
                                     "Un des éléments impliqués ou un ascendant\n"
                                     "de cet élément est destiné à l'effacement.\n"
                                     "Veuillez remettre l'élément responsable de cet état,\n"
                                     "en état de non effacement.\n\n"
                                    ));
}

//----------------------------------------------- testErrorDate ------------------------------------------------------------------------
/*! \brief fonction qui affiche un message d'erreur lors erreur de date ou heure
*/
int C_Organiseur::testErrorDate(int & isInvalid, const QString &date, const QString &heure, const QString & titre)
{  QString message = "";
   if (isInvalid == 2)
        {message =  titre + tr("\n Opération impossible :\n la date : ") + date + tr(" et l'heure : ") + heure + tr(" sont invalides. ");
         // QMessageBox::warning( this, titre,
         //                        QString ("<b><u>")+titre+"</b></u>"+
         //                        tr("<br>Opération impossible :<br>"
         //                           "la date : <b>") + date + tr("</b><br> et l'heure : <b>") + heure + tr("</b><br> sont invalides."
         //                           "Veuillez recommencer votre saisie."
         //                          ),
         //                         tr("Annuler"), 0, 0, 0, 1 );
        }
     else if (isInvalid & 1)
        {message =  titre + tr("\n Opération impossible :\n la date : ") + date +  tr(" est invalide. ");
         //QMessageBox::warning( this, titre,
         //                        QString ("<b><u>")+titre+"</b></u>"+
         //                        tr("<br>Opération impossible :<br>"
         //                           "la date : <b>") + date + tr("</b><br> est invalide."
         //                           "Veuillez recommencer votre saisie."
         //                          ),
         //                         tr("Annuler"), 0, 0, 0, 1 );
        }
    else if (isInvalid & 2)
        {message =  titre + tr("\n Opération impossible :\n l'heure : ") + heure +  tr(" est invalide. ");
         //QMessageBox::warning( this, titre,
         //                        QString ("<b><u>")+titre+"</b></u>"+
         //                        tr("<br>Opération impossible :<br>"
         //                           "l'heure : <b>") + heure + tr("</b><br> est invalide."
         //                           "Veuillez recommencer votre saisie."
         //                          ),
         //                         tr("Annuler"), 0, 0, 0, 1 );
        }
 if (message.length()) CouCou(message + tr("Veuillez recommencer votre saisie."));
 // if (message.length()) QMessageBox::warning( this, titre, message + tr("Veuillez recommencer votre saisie."),  tr("Annuler"), 0, 0, 0, 1 );
 return isInvalid;
}

//----------------------------------------------- makeListeMonitorFromList ------------------------------------------------------------------------
/*! \brief fonction qui recree toute la listview a partir des listes : m_pEvnmtList et m_pRubList
*/
void C_Organiseur::makeListeMonitor()
{makeListeMonitor("");
}

//----------------------------------------------- makeListeMonitorFromList ------------------------------------------------------------------------
/*! \brief fonction qui recree toute la listview a partir des listes : m_pEvnmtList et m_pRubList
*/
void C_Organiseur::makeListeMonitor(const QString &)
{//QString filtre = comboBox_FiltreVisu->currentText ();
 if (comboBox_Filtre->currentText () != tr("Dossier en cours"))
    {int isInvalid;
     QDateTime deb,fin;
     deb = CGenTools::setQDateTime( mle_deb->text() + mle_HeureDeb->text() , isInvalid);
     if (testErrorDate(isInvalid,   mle_deb->text() , mle_HeureDeb->text(), tr("Saisie date de début erronée")))
        {return;
        }
     fin = CGenTools::setQDateTime( mle_fin->text() + mle_HeureFin->text() , isInvalid);
     if (testErrorDate(isInvalid,   mle_fin->text() , mle_HeureFin->text(), tr("Saisie date de fin erronée")))
        {return;
        }
     makeListeMonitorFromSQL(m_Filtre, deb, fin);
    }
 else
    {makeListeMonitorFromList();
    }
}

//----------------------------------------------- makeListeMonitorFromList ------------------------------------------------------------------------
/*! \brief fonction qui recree toute la listview a partir des listes : m_pEvnmtList et m_pRubList
*/
void C_Organiseur::makeListeMonitorFromList()
{if (m_IsDragedFile) return ;
 //............ reperer l'eventuel Pk affiché pour le reaficher ensuite ....................
 QString                          pk  = "";
 C_DragQListViewItem  *pQListViewItem = (C_DragQListViewItem*) m_ListView_MonitorPatient->currentItem ();
 if ( pQListViewItem != 0 )       pk  = pQListViewItem->GET_PK_EVNT();
 //............ créer la liste ....................
 makeListeMonitorFromList(m_pEvnmtList, m_pRubList);
 //............ reaficher le Pk ensuite ....................
 if (pk.length()==0) return;
 pQListViewItem = GetListViewItem_FromPkEvnt(pk);
   if (pQListViewItem)
      {pQListViewItem->setSelected ( TRUE ) ;
       m_ListView_MonitorPatient->setCurrentItem ( pQListViewItem );
      }
}

//----------------------------------------------- On_FilesDraged_On_Item ---------------------------------------------------
/*! \brief Fonction qui ajoute et cree un document medintux tout en l'ajoutant dans l'organiseur
 *  \param itemDst  :  un pointeur sur l'item de l'oganiseur sur lequel doit etre place le nouveau document (si zero alors le document est racine)
 *  \param fileList : liste des fichiers modeles devant etre crees
 *  \param verbe    : texte decrivant l'action : copie, lien, deplacement, effacement (inutilise pour l'instant).
*/
void C_Organiseur::On_FilesDraged_On_Item(QListViewItem *itemDst,    QStrList &fileList,       const QString &verbe)
{C_DragQListViewItem *pC_DragQListViewItem = (C_DragQListViewItem*)itemDst;
 if (pC_DragQListViewItem && pC_DragQListViewItem->GET_PK_EVNT()[0]=='R')
    { pC_DragQListViewItem = 0;   // le document sera à la racine
    }
 QString  pDocSerialise("");
 QString  pk("");
 if (pC_DragQListViewItem)
    {pk = pC_DragQListViewItem->GET_PK_EVNT();  // itemDst->text(8) Head_Pk
     CRubEvntPair *pCRubEvntPairDst = m_pEvnmtList->GetEvenementRecordInListFromPk(pk);
     if (pCRubEvntPairDst==0) return;
     if (pCRubEvntPairDst->IsStatePlaced(TO_DELETE))
        {displayWarningItemInEraseState();
         return;
        }
     else if (m_pEvnmtList-> FirstStateFindInParents( pCRubEvntPairDst, TO_DELETE))
        { displayWarningItemInEraseState();
          return;
        }
    }
 m_IsDragedFile = TRUE;   // inhiber les macros de modification de la listview
 QString evntPk = "";
 if (pC_DragQListViewItem) evntPk = pC_DragQListViewItem->GET_PK_EVNT();            // sauver les meubles pour retrouver ensuite le bon item
 emit Sign_DragFileInOrganiseur(QString((const char*)fileList.at(0)), &pDocSerialise);
 if (evntPk.length()) pC_DragQListViewItem = GetListViewItem_FromPkEvnt(evntPk);    // recuperer les meubles
 m_IsDragedFile = FALSE;
 On_TextDraged_On_Item(pC_DragQListViewItem, pDocSerialise, verbe);
}

//----------------------------------------------- On_TextDraged_On_Item ---------------------------------------------------
/*! \brief Fonction qui ajoute dans l'organiseur un document medintux existant
 *  \param itemDst :  un pointeur sur l'item de l'oganiseur sur lequel doit etre place le document medintux existant (si zero alors le document est racine)
 *  \param serialize :  texte XML definissant les proprietes du document medintux
 *  \param verbe :  texte decrivant l'action : copie, lien, deplacement, effacement( inutilise pour l'instant).
*/
void C_Organiseur::On_TextDraged_On_Item(QListViewItem *itemDst,  const QString  &serialize,     const QString & /*verbe*/)
{C_DragQListViewItem *pC_DragQListViewItem = (C_DragQListViewItem*)itemDst;

 if (pC_DragQListViewItem && pC_DragQListViewItem->GET_PK_EVNT()[0]=='R')
    { pC_DragQListViewItem = 0;   // le document sera à la racine
    }

 CRubRecord rubRecord(serialize);      // cree un enregistrement copie de celui de depart (qui a ete drague) avec donnees de serialisation
 if (rubRecord.getLastError().length())   {qDebug(rubRecord.getLastError());  return;  }
 QString                          pk = "";
 C_DragQListViewItem *pQListViewItem = 0;

 //..................... si item de destination present alors il est fils sinon il est racine ........................................
 if (pC_DragQListViewItem)
              {pk = pC_DragQListViewItem->GET_PK_EVNT();
               CRubEvntPair *pCRubEvntPairDst = m_pEvnmtList->GetEvenementRecordInListFromPk(pk);
               if (pCRubEvntPairDst==0) return;
               if (pCRubEvntPairDst->IsStatePlaced(TO_DELETE))
                  { displayWarningItemInEraseState();
                    return;
                  }
               else if (m_pEvnmtList-> FirstStateFindInParents( pCRubEvntPairDst, TO_DELETE))
                  { displayWarningItemInEraseState();
                    return;
                   }
               pQListViewItem = new C_DragQListViewItem(itemDst);
              }
 else         {
               C_PatientRootItem         *pC_PatientRootItem  = m_ListView_MonitorPatient->FindOrCreatePatientRootItem(m_DossPk);
               if (pC_PatientRootItem)    {pQListViewItem     = new C_DragQListViewItem(pC_PatientRootItem->m_pC_DragQListViewItem);
                                           pC_PatientRootItem->m_pC_DragQListViewItem->setPixmap ( 0, Theme::getIcon("16x16/identityRed.png") );
                                          }
               pk ="0";       // racine
              }

 if (pQListViewItem==0)                 return;


 if (pk.length())
    { CRubEvntPair cRubEvntPair (pk, rubRecord.m_PrimKey,  m_pEvnmtList->Evnmt_GetPkProvisoire(),  m_DossPk  );
      cRubEvntPair.setState(TO_CREATE);
      m_pEvnmtList->append(cRubEvntPair );
      SetListViewItemFromRecord(pQListViewItem, &rubRecord , &cRubEvntPair);
      pQListViewItem->setDropEnabled (TRUE );
    }
}

//----------------------------------------------- On_ItemDraged_On_AnotherItem ---------------------------------------------------
/*! \brief Fonction qui repond au deplacement d'un item dans l'organiseur
 *  \param itemSrc :  un pointeur sur l'item source de l'oganiseur ayant ete deplace.
 *  \param itemDst :  un pointeur sur l'item destination de l'oganiseur sur lequel a ete lache l'item source (si zero alors le document est racine)
 *  \param verbe   :  texte decrivant l'action : copie, lien, deplacement, effacement (inutilise pour l'instant).
*/
void C_Organiseur::On_ItemDraged_On_AnotherItem(QListViewItem *itemSrc, QListViewItem *itemDst, const QString & /*verbe*/)
{ C_DragQListViewItem* pC_DragQListViewItemSrc = (C_DragQListViewItem*)itemSrc;
  if (pC_DragQListViewItemSrc == 0)                   return;
  if (pC_DragQListViewItemSrc->GET_PK_EVNT()[0]=='R') return;

  QString                   pere = "0";
  QString                  pkDst = "";
  CRubEvntPair *pCRubEvntPairDst = 0;
  QString                  pkSrc = ((C_DragQListViewItem*)itemSrc)->GET_PK_EVNT();    // retourne le Pk de l'item à modifier dans la m_pEvnmtList
  CRubEvntPair *pCRubEvntPairSrc = m_pEvnmtList->GetEvenementRecordInListFromPk(pkSrc);
  if (pCRubEvntPairSrc==0) return;
  if (itemDst)
     {                     pkDst = ((C_DragQListViewItem*)itemDst)->GET_PK_EVNT();    // recupere le Pk de l'item destination dans la m_pEvnmtList
                pCRubEvntPairDst = m_pEvnmtList->GetEvenementRecordInListFromPk(pkDst);
     }
  if (pCRubEvntPairDst)
     { if (pCRubEvntPairDst->IsStatePlaced(TO_DELETE))
          {displayWarningItemInEraseState();
          }
       else if (m_pEvnmtList-> FirstStateFindInParents( pCRubEvntPairDst, TO_DELETE))
          {displayWarningItemInEraseState();
          }
       else if (m_pEvnmtList->IsThisItemInBranch( pCRubEvntPairSrc, pCRubEvntPairDst->m_Pk))
          { //QMessageBox::warning( this, "La vigie vous informe ...",
            //                     tr("Opération impossible :\n"
            //                        "Il n'est pas possible de placer un ascendant\n"
            //                        "sur une de ses propres branches.\n\n"
            //                       ),
            //                      tr("Annuler"), 0, 0, 0, 1 );
             CouCou(tr("Opération impossible : Il n'est pas possible de placer un ascendant sur une de ses propres branches." ));
          }
       else
          {pCRubEvntPairSrc->setState(TO_UPDATE);
           pCRubEvntPairSrc->m_Pere = pCRubEvntPairDst->m_Pk;       // il change de pere (car drague dessus)
          }
     }
  else
     {pCRubEvntPairSrc->setState(TO_UPDATE);
      pCRubEvntPairSrc->m_Pere = "0";                            // il devient racine car drague sur rien)
     }
  makeListeMonitor();                                    // faut reaficcher la hierarchie
}
//----------------------------------------------- isThisHierarchieExist ---------------------------------------------------
/*! \brief Fonction qui verfie si le chemin donne en entree correspond a une hirarchie de documents de l'organiseur
 *  \param hierarch : QString de la hierarchie a chercher (termes separes par /)
*/
CRubEvntPair   * C_Organiseur::isThisHierarchieExist( QString & hierarch )
{if (!hierarch.endsWith("/")) hierarch += "/";
 m_pFindHierarchEvnt           = 0;
 int pos_deb                   = 0;
 int pos_end                   = hierarch.find('/');
 QString strToCompare          = hierarch.mid(pos_deb, pos_end-pos_deb);
 QDateTime   date              = QDateTime::currentDateTime ();
 m_HierarchRubRecordList.clear();  // main
 //............ si hierarchie donnee en entrée  ...................
 //             rechercher si elle correspond à une existante dans la base
 if (strToCompare.length())
    {CRubEvntList::iterator it;
     for (it = m_pEvnmtList->begin(); it !=  m_pEvnmtList->end() && m_pFindHierarchEvnt == 0; ++it )
         {if ((*it).m_Pere == "0")
             {CRubRecord* pCRubRecord = pMB->RubRecordFromPk(m_pRubList, (*it).m_PkDoc);
              if (pCRubRecord && pCRubRecord->m_Libelle == strToCompare && pCRubRecord->m_Fin.length()==0) // ok ce bout de hierarchie correspond, aller voir si les fils sont Ok avec prochain bout
                 {m_HierarchRubRecordList.append(pCRubRecord);
                  int nextPosDeb = pos_end+1;
                  if (nextPosDeb == (int)hierarch.length())  // si on est au bout de la chaine hierarchique et que ca matche c'est Ok
                     {m_pFindHierarchEvnt = &(*it);
                     }
                  else
                     {while (Find_Evnt_In_Hierarchie(&(*it), hierarch, date, nextPosDeb, hierarch.find('/', nextPosDeb)));
                     }
                 }
             }
         }
    }
 return m_pFindHierarchEvnt;
}

//----------------------------------------------- Append_Doc_In_Hierarchie ---------------------------------------------------
/*! \brief Fonction qui ajoute un document medintux existant dans l'organiseur
 *  \param pCRubRecordToAdd  : CRubRecord*  un pointeur sur l'enregistrement à rajouter
 *  \param hierarch : QString de la hierarchie ou placer le documment (termes separes par /)
*/
QString C_Organiseur::Append_Doc_In_Hierarchie(CRubRecord* pCRubRecordToAdd, QString hierarch, int ifNoHierarchAppendDoc /*=0*/ )
{if (m_IsDragedFile) return QString::null;
 QDateTime   date              = QDateTime::currentDateTime ();
 QString     mess              = "";
 /*
 if (!hierarch.endsWith("/")) hierarch += "/";
 m_pFindHierarchEvnt           = 0;
 int pos_deb                   = 0;
 int pos_end                   = hierarch.find('/');
 QString strToCompare          = hierarch.mid(pos_deb, pos_end-pos_deb);

 m_HierarchRubRecordList.clear();  // main
 //............ si hierarchie donnee en entrée  ...................
 //             rechercher si elle correspond à une existante dans la base
 if (strToCompare.length())
    {CRubEvntList::iterator it;
     for (it = m_pEvnmtList->begin(); it !=  m_pEvnmtList->end() && m_pFindHierarchEvnt == 0; ++it )
         {if ((*it).m_Pere == "0")
             {CRubRecord* pCRubRecord = pMB->RubRecordFromPk(m_pRubList, (*it).m_PkDoc);
              if (pCRubRecord && pCRubRecord->m_Libelle == strToCompare && pCRubRecord->m_Fin.length()==0) // ok ce bout de hierarchie correspond, aller voir si les fils sont Ok avec prochain bout
                 {m_HierarchRubRecordList.append(pCRubRecord);
                  int nextPosDeb = pos_end+1;
                  if (nextPosDeb == (int)hierarch.length())  // si on est au bout de la chaine hierarchique et que ca matche c'est Ok
                     {m_pFindHierarchEvnt = &(*it);
                     }
                  else
                     {while (Find_Evnt_In_Hierarchie(&(*it), hierarch, date, nextPosDeb, hierarch.find('/', nextPosDeb)));
                     }
                 }
             }
         }
    }
  */
 CRubEvntPair   *pFindHierarchEvnt = isThisHierarchieExist(hierarch);
 if (pFindHierarchEvnt==0)      //>>> ............ PAS DE HIERARCHIE TROUVEE POUR CET ELEMENT ...................
    {if (hierarch=="/")                  //||| >>> SI la hierarchie donnée est ROOT créer l'élément ROOT
        {if (GetRootEvntWithSameNameNameAndDate(m_pRubList, pCRubRecordToAdd->m_Libelle, pCRubRecordToAdd->m_Date)==0) // si pas de root ouvert pour cette date alors on rajoute
            {On_TextDraged_On_Item(0,  pCRubRecordToAdd->serialize(),    "Copy");                 // 0 pour le placer en racine et creer un element racine
            }
         else
            {QString libelle = pCRubRecordToAdd->m_Libelle; //libelle.replace(" ","&nbsp;");
             //QMessageBox::warning( this, "La vigie vous informe ...",
             //                    tr("<u>Opération impossible</u> :<br>"
             //                       "Un élément racine en cours d'activité : <b>") + libelle + tr("</b>  du même nom existe déjà."
             //                      ),
             //                     tr("Annuler"), 0, 0, 0, 1 );
             //CouCou( tr("Opération impossible : Un élément racine en cours d'activité : ") + libelle + tr(" du même nom existe déjà." ));
             //if (pC_FormGlossaire) pC_FormGlossaire->Slot_SetCurrentPath(oldPathDocuments);
             if (ifNoHierarchAppendDoc>0) // si le document doit tout de meme etre insere meme si hierarchie non trouvée
                {CouCou( tr("Ce document : '") + libelle + tr("' sera juste inséré comme document, et non classé (il existe déjà un classeur du même non à la racine)" ));
                }
             else
                {pCRubRecordToAdd->m_LastErr = "__ERREUR Fusion :__";
                 return tr("__ERREUR Fusion :__ Cet élément '") + libelle + tr("' n'a pu être inséré dans la vigie car il en existe  déjà un ouvert à ce nom. Veuillez l'effacer si il est inutile. ") ;
                }
            }
        }                                //||| >>> SI la hierachie donnée n'est pas ROOT on signale que non trouvée
     else
        {//QMessageBox::warning( this, "La vigie vous informe ...",
         //                        tr("<u>Opération impossible</u> :<br>"
         //                           "Le classeur de destination : <b>") + hierarch + tr("</b>  où doit être placé ce document n'a pas été trouvé, veuillez le classer manuellement."
         //                          ),
         //                         tr("Annuler"), 0, 0, 0, 1 );
         if (ifNoHierarchAppendDoc>0) // si le document doit tout de meme etre insere meme si hierarchie non trouvée
            {CouCou( tr("Ce document : '") + pCRubRecordToAdd->m_Libelle + tr("' sera juste inséré comme document, et non classé car le classeur de destination est non trouvé" ));
            }
         else
            {mess = tr("Opération impossible : Le classeur de destination : ") + hierarch + tr(" où doit être placé ce document '") + pCRubRecordToAdd->m_Libelle + tr("' n'a pas été trouvé, veuillez le classer manuellement." );
             CouCou( mess);
             m_log += mess+"\n";
             //CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"VigieLog.txt", m_log);
             //if (pC_FormGlossaire) pC_FormGlossaire->Slot_SetCurrentPath(oldPathDocuments);
             pCRubRecordToAdd->m_LastErr = "__ERREUR Fusion :__";
             return tr("__ERREUR Fusion :__ Le classeur de destination '") + hierarch + tr("' où doit être placé ce document n'a pas été trouvé\\, veuillez le classer manuellement. ") ;
            }
        }
    }
 else                             //>>> ............ HIERARCHIE TROUVEE POUR CET ELEMENT ...................
    {C_DragQListViewItem* pC_DragQListViewItem = GetListViewItem_FromPkEvnt(pFindHierarchEvnt->m_Pk);
     QString isUnique    = getProfileValue(pCRubRecordToAdd->m_Libelle, "IsUnique");
     QString isInVisible = getProfileValue(pCRubRecordToAdd->m_Libelle, "IsInVisible");
     if (pC_DragQListViewItem)
        { if (m_ClasseursAllVisibles==0 && isInVisible=="1")
             {QString libelle = pCRubRecordToAdd->m_Libelle; //libelle.replace(" ","&nbsp;");
              //QMessageBox::warning( this, "La vigie vous informe ...",
              //                     tr("<u>Opération impossible</u> :<br>"
              //                         "Un classeur unique correspondants à ce document : <b>") + libelle + tr("</b>  existe déjà."
              //                       ),
              //                     tr("Annuler"), 0, 0, 0, 1 );
              if (ifNoHierarchAppendDoc>0) // si le document doit tout de meme etre insere meme si hierarchie non trouvée
                 {CouCou( tr("Ce document : '") + libelle + tr("' sera juste inséré comme document, et non classé car le classeur de destination est non accessible avec ce profil utilisateur" ));
                 }
              else
                 {CouCou( tr("Opération impossible : Le profil utilisateur en cours ne permet pas de voir ce classeur : ") + libelle + tr(" ." ));
                  //if (pC_FormGlossaire) pC_FormGlossaire->Slot_SetCurrentPath(oldPathDocuments);
                  pCRubRecordToAdd->m_LastErr = "__ERREUR Fusion :__";
                  return tr("__ERREUR Fusion :__ Cet élément '") + libelle + tr("' n'a pu être inséré dans la vigie car le profil utilisateur en cours ne permet pas de le voir ") ;
                 }
             }
          if (isUnique=="1" && Find_Evnt_WhithSameName_InFratrie(m_pRubList, pFindHierarchEvnt, pCRubRecordToAdd->m_Libelle))
             {QString libelle = pCRubRecordToAdd->m_Libelle; //libelle.replace(" ","&nbsp;");
              //QMessageBox::warning( this, "La vigie vous informe ...",
              //                     tr("<u>Opération impossible</u> :<br>"
              //                         "Un classeur unique correspondants à ce document : <b>") + libelle + tr("</b>  existe déjà."
              //                       ),
              //                     tr("Annuler"), 0, 0, 0, 1 );
              //CouCou( tr("Opération impossible : Un classeur unique correspondant à ce document : ") + libelle + tr(" existe déjà." ));
              //if (pC_FormGlossaire) pC_FormGlossaire->Slot_SetCurrentPath(oldPathDocuments);
              if (ifNoHierarchAppendDoc>0) // si le document doit tout de meme etre insere meme si hierarchie non trouvée
                 {CouCou( tr("Ce document : '") + libelle + tr("' sera juste inséré comme document, et non comme classeur il en existe déjà un unique" ));
                 }
              else
                 {pCRubRecordToAdd->m_LastErr = "__ERREUR Fusion :__";
                  return tr("__ERREUR Fusion :__ Cet élément '") + libelle + tr("' n'a pu être inséré dans la vigie un classeur unique portant le même nom existe déjà ") ;
                 }
             }
          else
             {On_TextDraged_On_Item(pC_DragQListViewItem,  pCRubRecordToAdd->serialize(),    "Copy");
             }
        }
     else
        {    QString libelle = pCRubRecordToAdd->m_Libelle; //libelle.replace(" ","&nbsp;");
             //QMessageBox::warning( this, "La vigie vous informe ...",
             //                      tr("<u>Opération impossible</u> :<br>"
             //                          "les classeurs correspondants à ce document : <b>") + libelle + tr("</b>  ne sont pas visibles (changez votre filtre d'affichage)."
             //                        ),
             //                      tr("Annuler"), 0, 0, 0, 1 );
             if (ifNoHierarchAppendDoc>0) // si le document doit tout de meme etre insere meme si hierarchie non trouvée
                 {CouCou( tr("Ce document : '") + libelle + tr("' sera juste inséré comme document, et non classé car le classeur de destination est non accessible avec ce profil d'affichage" ));
                 }
             else
                 {CouCou( tr("Opération impossible : les classeurs correspondants à ce document : ") + libelle + tr(" ne sont pas visibles (changez votre filtre d'affichage)." ));
                  //if (pC_FormGlossaire) pC_FormGlossaire->Slot_SetCurrentPath(oldPathDocuments);
                  pCRubRecordToAdd->m_LastErr = "__ERREUR Fusion :__";
                  return tr("__ERREUR Fusion :__ Cet élément '") + libelle + tr("' n'a pu être inséré dans la vigie car les classeurs parents d'insertion sont non visibles ") ;
                 }
        }
    }
 return QString::null;
}

//----------------------------------------------- CouCou ---------------------------------------------------
void C_Organiseur::CouCou(const QString &mess)
{G_pCApp->CouCou(mess,Theme::getPath() + "MessagePopVigie.png");
}

//----------------------------------------------- Find_Evnt_In_Hierarchie ---------------------------------------------------
/*! \brief Fonction qui explore la descendance d'un evenement pour voir si il correspond a la hierarchie donnee en entree si trouve : m_pFindHierarchEvnt contient le pointeur de l'evenement final
 *  \param CRubEvntPair *hierarchEvntPere enregistrement du pere dont il faut explorer la descendance
 *  \param hierarch : QString de la hierarchie ou placer le documment (termes separes par /)
 *  \param date :  date du trux à placer (pour distinguer ceux avec la meme hierarchie mais de date differente)
 *  \param pos_deb : int de debut du terme pointe dans la hierarchie
 *  \param pos_end : int de fin du terme pointe dans la hierarchie
*/
int C_Organiseur::Find_Evnt_In_Hierarchie(CRubEvntPair *hierarchEvntPere, const QString hierarch, const QDateTime &date, int pos_deb, int pos_end)
{if ( pos_end == -1) return 0;
 QString                      strToCompare = hierarch.mid(pos_deb, pos_end-pos_deb);
 //...................... relever tous les enfants..............................
 //                             du meme pere
 CRubEvntList::iterator it;
 for (it = m_pEvnmtList->begin(); it !=  m_pEvnmtList->end() && m_pFindHierarchEvnt==0; ++it )
     {if ((*it).m_Pere == hierarchEvntPere->m_Pk)  // si c'est un enfant du pere
         {CRubRecord* pCRubRecord = pMB->RubRecordFromPk(m_pRubList, (*it).m_PkDoc);
          if ( pCRubRecord && pCRubRecord->m_Libelle == strToCompare)
             {m_HierarchRubRecordList.append(pCRubRecord);
              int nextPosDeb = pos_end+1;
              if (nextPosDeb == (int)hierarch.length())            // si on est au bout de la chaine hierarchique et que ca matche c'est Ok
                 {m_pFindHierarchEvnt = &(*it);
                  return 0;
                 }
              else
                 {while (Find_Evnt_In_Hierarchie(&(*it), hierarch, date, nextPosDeb, hierarch.find('/', nextPosDeb)));
                 }
             }
         }
     }
 return 0;
}

//----------------------------------------------- Find_Evnt_WhithSameName_InFratrie ---------------------------------------------------
/*! \brief Fonction qui explore la fratrie de la descendance (uniquement la fratrie pas les ils des fils etc...) d'un evenement à la recherche d'un nom semblable au libelle
 *  \param pRubList   RUBREC_LIST* :  un pointeur sur la liste des documents
 *  \param pEvntPere : CRubEvntPair* evenement pere dont il faut explorer la fratrie de sa descendance
 *  \param libelle : QString du nom dont il faut rechercher le meme element dans la fratrie
*/
CRubEvntPair* C_Organiseur::Find_Evnt_WhithSameName_InFratrie(RUBREC_LIST   *pRubList, CRubEvntPair* pEvntPere, const QString libelle)
{ CRubEvntList::iterator it;
  for (it = m_pEvnmtList->begin(); it !=  m_pEvnmtList->end(); ++it )
      {if ((*it).m_Pere == pEvntPere->m_Pk)
          {CRubRecord* pCRubRecord = pMB->RubRecordFromPk(pRubList, (*it).m_PkDoc);
           if (pCRubRecord && pCRubRecord->m_Libelle == libelle)
              { return &(*it);
              }
          }
      }
  return 0;
}

//----------------------------------------------- GetRootEvntWithSameNameNameAndDate ---------------------------------------------------
/*! \brief Fonction qui recherche dans les classeurs racines un ouvert (date de fin non fermee) avec le meme libelle
 *  \param pk           const QString : le pk de l'evennement
 *  \return CRubEvntPair* : pointeur sur l'enregistrement ou zero si pas trouve
*/

CRubEvntPair* C_Organiseur::GetRootEvntWithSameNameNameAndDate(RUBREC_LIST   *pRubList, const QString &libelle, const QString & /*date*/)
{ CRubEvntList::iterator it;
  for (it = m_pEvnmtList->begin(); it !=  m_pEvnmtList->end(); ++it )
      {if ((*it).m_Pere == "0")
          {CRubRecord* pCRubRecord = pMB->RubRecordFromPk(pRubList, (*it).m_PkDoc);
           if (pCRubRecord && pCRubRecord->m_Libelle == libelle && pCRubRecord->m_Fin.length()==0)
              { return &(*it);
              }
          }
      }
 return 0;
}

//----------------------------------------------- On_Sign_ItemClicked ---------------------------------------------------
void C_Organiseur::On_Sign_ItemClicked(QListViewItem *pQListViewItem)
{QPoint p(0,0) ;
 On_ItemClicked(pQListViewItem, QPoint(0,0) ,0);
}

//----------------------------------------------- On_ItemClicked ---------------------------------------------------
/*! \brief slot repondant au signal : Sign_ItemClicked( QListViewItem *)  activant dans medintux le document pointe par un des item de la lisbox
 *  @param pQListViewItem item dont il faut activer le document correspondant
 */
void C_Organiseur::On_ItemClicked(QListViewItem *pQListViewItem, const QPoint &pos , int c)
{
 C_DragQListViewItem* pC_DragQListViewItem     = (C_DragQListViewItem*)pQListViewItem;
 C_DragQListViewItem* pC_DragRootQListViewItem = 0;
 if (pC_DragQListViewItem==0) {return;
                              }
 if (c >= DT_Deb-1 && c<=DT_Fin+1)
    {On_ContextMenuRequested(pQListViewItem , pos , c);
     return;
    }
 int xs = m_ListView_MonitorPatient->verticalScrollBar()->value();
 QString itemDossPk = pC_DragQListViewItem->GET_PK_DOS();
 QString   pkDoc    = pC_DragQListViewItem->GET_PK_DOC();
 QString   pkEvn    = pC_DragQListViewItem->GET_PK_EVNT();
 clearListViewHeaderOnItem();
 if ( itemDossPk != m_DossPk)
    {QString nom, prenom, guid;
     m_ListView_MonitorPatient->dragClose();
     pMB->GetPatientNomPrenomPk(TRUE, itemDossPk,  &nom, &prenom, &guid);
     //............. apres cet emit la listview a ete reconstruite donc .........
     //              les pointeurs ne sont plus bons
     emit Sign_IdentChangeInOrganiseur(guid,  itemDossPk, nom,  prenom);
     pC_DragRootQListViewItem = GetListViewItemROOTFromPkDoss(itemDossPk);
     if (pkEvn[0]=='R')
        {pC_DragQListViewItem = pC_DragRootQListViewItem;
         if (pC_DragQListViewItem)
             { m_ListView_MonitorPatient->setSelected ( pC_DragQListViewItem, TRUE );
               m_ListView_MonitorPatient->setCurrentItem ( pC_DragQListViewItem );
               //m_ListView_MonitorPatient->verticalScrollBar()->setValue(xs);
               m_ListView_MonitorPatient->ensureItemVisible ( pC_DragQListViewItem );
             }
        }
     else
        { pC_DragQListViewItem = GetListViewItemFromPkDoc(pkDoc);
          if (pC_DragQListViewItem)
             { m_ListView_MonitorPatient->setSelected ( pC_DragQListViewItem, TRUE );
               m_ListView_MonitorPatient->setCurrentItem ( pC_DragQListViewItem );
               //m_ListView_MonitorPatient->verticalScrollBar()->setValue(xs);
               m_ListView_MonitorPatient->ensureItemVisible ( pC_DragQListViewItem );
             }
        }
     //pC_DragRootQListViewItem->setPixmap ( 0, Theme::getIcon("16x16/identityRed.png") );
    }
 if (pC_DragQListViewItem)
    {setListViewHeaderOnItem(pC_DragQListViewItem);
     QString hierarch = GetItemHierarchie(pC_DragQListViewItem, C_Organiseur::justtDir);
     QString defPath  = QString("#")+m_ListView_MonitorPatient->m_DefaultMaskDirectory; // # pour forcer le positionnment sur le repertoire
     hierarch.prepend(G_pCApp->m_PathGlossaire+"Vigie/");
     if (QFile::exists(hierarch))
        {emit Sign_MouseEnter(hierarch, defPath);
        }
    }
 CRubRecord *pCRubRecord;
 if (G_pCApp->GetCurrentRubrique( 0 ,  0 , &pCRubRecord ))
    {if (pCRubRecord==0 || pCRubRecord->m_PrimKey != pkDoc)  Sign_ActiverDocument(pkDoc, "", 2);
     m_ListView_MonitorPatient->mouseEnter();
    }
 m_ListView_MonitorPatient->verticalScrollBar()->setValue(xs);
}

//----------------------------------------------- setListViewHeaderOnItem ---------------------------------------------------
/*! \brief positionne les titres du header de la listview sur l'item clique
 *  @param pQListViewItem item dont il faut activer le document correspondant
 */
void C_Organiseur::setListViewHeaderOnItem(C_DragQListViewItem *pQListViewItem)
{ QHeader *pQHeader = m_ListView_MonitorPatient->header();
  pQHeader->setLabel ( 0,   pQListViewItem->text(0));
  OpClProfiles::Iterator it = m_UserClasseurProfiles.find(pQListViewItem->text(0));
  if (it == m_UserClasseurProfiles.end()) return;         // pas de profile pour ce classeur cassos
  QString dataToExamine = it.data();
  QString   value       = "";
  if ( CGenTools::GetXMLValue(dataToExamine, "<menuProp0>", value, 0)!=-1 ) pQHeader->setLabel ( CP_Deb,   value);
  if ( CGenTools::GetXMLValue(dataToExamine, "<menuProp1>", value, 0)!=-1 ) pQHeader->setLabel ( CP_Deb+1, value);
  if ( CGenTools::GetXMLValue(dataToExamine, "<menuProp2>", value, 0)!=-1 ) pQHeader->setLabel ( CP_Deb+2, value);
  if ( CGenTools::GetXMLValue(dataToExamine, "<menuProp3>", value, 0)!=-1 ) pQHeader->setLabel ( CP_Deb+3, value);
  if ( CGenTools::GetXMLValue(dataToExamine, "<menuProp4>", value, 0)!=-1 ) pQHeader->setLabel ( CP_Deb+4, value);
  //pQHeader->adjustHeaderSize();
}

//----------------------------------------------- clearListViewHeaderOnItem ---------------------------------------------------
/*! \brief efface les titres du header de la listview
 */
void C_Organiseur::clearListViewHeaderOnItem()
{ QHeader *pQHeader = m_ListView_MonitorPatient->header();
  pQHeader->setLabel ( 0,          "");
  pQHeader->setLabel ( CP_Deb,     "");
  pQHeader->setLabel ( CP_Deb+1,   "");
  pQHeader->setLabel ( CP_Deb+2,   "");
  pQHeader->setLabel ( CP_Deb+3,   "");
  pQHeader->setLabel ( CP_Deb+4,   "");
}
/*
//---------------------------------------- setHeure ---------------------------------------
QTime C_Organiseur::hhmmssToQTime(const QString & heure )
{ int h=0; int m=0; int s=0; int ms=0; int len = heure.length();
  QString str = C_TokenInterpret::toNum(heure,"$allString");
  len         = str.length();
  if (len>=2) h  = str.mid(0,2).toInt();  // hh
  if (len>=4) m  = str.mid(2,2).toInt();  // hhmm
  if (len>=6) s  = str.mid(4,2).toInt();  // hhmmss
  if (len>=9) ms = str.mid(6).toInt();    // hhmmss
  return QTime( h,  m,  s,  ms );
}
*/
//---------------------------------------- ddMMyyyyToQDate ---------------------------------------
QDate C_Organiseur::ddMMyyyyToQDate(const QString & date )
{ QString text = C_TokenInterpret::toNum(date,"$allString");
  int jour, mois, annee;
  jour  = text.mid(0,2).toInt();
  mois  = text.mid(2,2).toInt();
  annee = text.mid(4,4).toInt();
  return QDate(annee, mois, jour);
}
//----------------------------------------------- On_ContextMenuRequested ------------------------------------------------------------------------
/*! \brief Slot qui repond a la demande de menu contextuel sur un item (ou la solitude d'un obscur codeur)
*/
void C_Organiseur::On_ContextMenuRequested(QListViewItem *pQListViewItem , const QPoint & /*pos*/ , int c)
{QStringList menuList;
 int xs = m_ListView_MonitorPatient->verticalScrollBar()->value();

 //............... recuperer a qui on a affaire ..................
 C_DragQListViewItem* pC_DragQListViewItem = (C_DragQListViewItem*)pQListViewItem;
 if (pC_DragQListViewItem==0)     return;
 //............................. changer de patient .....................................................
 if ( pC_DragQListViewItem->GET_PK_DOS() != m_DossPk)
    {//................. recuperer le pk du pointeur avant reconstruction de la liste ...................
     QString evntPk = pC_DragQListViewItem->GET_PK_EVNT();
     On_Sign_ItemClicked(pC_DragQListViewItem);  // attention cet appel modifie la liste et donc pointeurs d'avant plus valides
     //................. recuperer le pointeur avec le pk ...............................................
     pC_DragQListViewItem = GetListViewItem_FromPkEvnt(evntPk);
     if (pC_DragQListViewItem==0) return;
    }
 CRubEvntPair* pCRubEvntPair = m_pEvnmtList->GetEvenementRecordInListFromPk(pC_DragQListViewItem->GET_PK_EVNT());
 if (pCRubEvntPair==0)            return;

 //................... recuperer le document lie et l'activer si besoin ...........................
 CRubRecord *pCRubRecord;
 if (G_pCApp->GetCurrentRubrique( 0 ,  0 , &pCRubRecord ))
    {if (pCRubRecord==0 || pCRubRecord->m_PrimKey != pC_DragQListViewItem->GET_PK_DOC())  Sign_ActiverDocument(pC_DragQListViewItem->GET_PK_DOC(), "", 2);
    }
 //m_ListView_MonitorPatient->ensureItemVisible ( pC_DragQListViewItem );
 G_pCApp->GetCurrentRubrique( 0 ,  0 , &pCRubRecord );
 QString evntPk      = pC_DragQListViewItem->GET_PK_EVNT();        // sauver les meubles pour retrouver ensuite le bon item
  if (c==DT_Deb-1)
     {ChangeDateTime(pC_DragQListViewItem, pCRubRecord, "DateDeb");
      return;
     }
  else if (c==DT_Fin+1)
     {ChangeDateTime(pC_DragQListViewItem, pCRubRecord, "DateFin");
      return;
     }
  else if (c==DT_Deb)
    {C_Dlg_Pendule* dlg = new C_Dlg_Pendule( this);
     if (dlg==0)                  return;
     dlg->move(QCursor::pos());
     dlg->setCaption(tr("Heure de Début"));
     //................. relever les dates de  debut et de fin ..................
     int pos;
     QString txt_date;
     QString txt_time;
     QString txt_dateTime;
     txt_dateTime = pC_DragQListViewItem->text(DT_Deb)+":00";
     pos          =  txt_dateTime.find(tr("à")); if (pos != -1) {txt_date=txt_dateTime.left(pos); txt_time = txt_dateTime.mid(pos+1).stripWhiteSpace();}
     QTime t_deb  = C_TokenInterpret::hhmmssToQTime(txt_time );    if ( !t_deb.isValid () || txt_time.length()==0) { t_deb = QTime::currentTime(); }
     QDate d_deb  = ddMMyyyyToQDate(txt_date );  if ( !d_deb.isValid ())                         { d_deb = QDate::currentDate(); }
     txt_dateTime = pC_DragQListViewItem->text(DT_Fin)+":00";
     txt_date     = txt_time = "";
     pos          = txt_dateTime.find(tr("à")); if (pos != -1) {txt_date=txt_dateTime.left(pos); txt_time = txt_dateTime.mid(pos+1).stripWhiteSpace();}
     QTime t_fin  = C_TokenInterpret::hhmmssToQTime(txt_time );     // ne pas corriger car si fin invalide (par exemple vide) on peut quand meme rentrer la date de debut
     QDate d_fin  = ddMMyyyyToQDate(txt_date );   // ne pas corriger car si fin invalide (par exemple vide) on peut quand meme rentrer la date de debut

     dlg->setTime(t_deb);
     if (dlg->exec() == QDialog::Accepted && pCRubRecord)
        {  //............. controle  des resultats .........................
            QDateTime dt_deb(d_deb,dlg->getTime());
            QDateTime dt_end(d_fin, t_fin);
            if ( !dt_end.isValid() || dt_deb<=dt_end)
               {//............. insertion des resultats .........................
                txt_date            = dt_deb.toString("yyyy-MM-dd") + "T" + dlg->getTime().toString("hh:mm:ss");
                pCRubRecord->m_Date = txt_date;
                pC_DragQListViewItem->setText(DT_Deb, QueryDateHeureToNatural(txt_date));
                G_pCApp->putCurrentRubriqueOnModifiedState();
               }
            else
               {CouCou(tr("la date et l'heure de début ne peuvent pas être supérieures à la date et heure de fin. "));
               }
        }
     delete dlg;
     return;
    }
 else if (c==DT_Fin)
    {C_Dlg_Pendule* dlg = new C_Dlg_Pendule( this);
     if (dlg==0)                  return;
     dlg->move(QCursor::pos());
     dlg->setCaption(tr("Heure de fin"));
     //................. relever les dates de  debut et de fin ..................
     int pos;
     QString txt_date;
     QString txt_time;
     QString txt_dateTime;
     txt_dateTime = pC_DragQListViewItem->text(DT_Deb)+":00";
     pos          =  txt_dateTime.find(tr("à")); if (pos != -1) {txt_date=txt_dateTime.left(pos); txt_time = txt_dateTime.mid(pos+1).stripWhiteSpace();}
     QTime t_deb  = C_TokenInterpret::hhmmssToQTime(txt_time );    if ( !t_deb.isValid () || txt_time.length()==0) { t_deb = QTime::currentTime(); }
     QDate d_deb  = ddMMyyyyToQDate(txt_date );  if ( !d_deb.isValid ())                         { d_deb = QDate::currentDate(); }
     txt_dateTime = pC_DragQListViewItem->text(DT_Fin)+":00";
     txt_date     = txt_time = "";
     pos          =  txt_dateTime.find(tr("à")); if (pos != -1) {txt_date=txt_dateTime.left(pos); txt_time = txt_dateTime.mid(pos+1).stripWhiteSpace();}
     QTime t_fin  = C_TokenInterpret::hhmmssToQTime(txt_time );    if ( !t_fin.isValid ()|| txt_time.length()==0) { if (t_deb.isValid()) t_fin = t_deb; else t_fin = QTime::currentTime(); }
     QDate d_fin  = ddMMyyyyToQDate(txt_date );  if ( !d_fin.isValid ())                        { if (d_deb.isValid()) d_fin = d_deb; else d_fin = QDate::currentDate(); }

     dlg->setTime(t_fin);
     if (dlg->exec() == QDialog::Accepted && pCRubRecord)
        {   //............. controle  des resultats .........................
            QDateTime dt_deb(d_deb,t_deb);
            QDateTime dt_end(d_fin,dlg->getTime());
            if (dt_deb<=dt_end)
               {//............. insertion des resultats .........................
                txt_date            = dt_end.toString("yyyy-MM-dd") + "T" + dlg->getTime().toString("hh:mm:ss");
                pCRubRecord->m_Fin  = txt_date;
                pC_DragQListViewItem->setText(DT_Fin, QueryDateHeureToNatural(txt_date));
                G_pCApp->putCurrentRubriqueOnModifiedState();
               }
            else
               {CouCou(tr("la date et l'heure de fin ne peuvent pas être inférieures à la date et heure de début. "));
               }
        }
     delete dlg;
     return;
    }
 else if (c >= CP_Deb && c <= CP_Fin)
    {QString   listeDeroulante  = "";
     QString   text             = pC_DragQListViewItem->text(0);
     OpClProfiles::Iterator it  = m_UserClasseurProfiles.find(text);
     if (it != m_UserClasseurProfiles.end())
        { QString dataToExamine     = it.data();
          CGenTools::GetXMLValue(dataToExamine, "<menuProp"+QString::number(c-CP_Deb)+">", listeDeroulante, 0);
        }
     if (listeDeroulante.length())
        {
          clearListViewHeaderOnItem();
          setListViewHeaderOnItem(pC_DragQListViewItem);
          QString listInUse   = "";
          //..........appel d'un editeur de texte pour activer le menu .............
          text  =  "<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body>"
                   "<p><font color=#000000 face=\"Arial\">"
                   "<span style=\"font-family:Arial;font-size:10pt;color:#0000ff\">";
          text +=  listeDeroulante +
                   "</span></p></font></body></qt></html>";
          MyEditText *pMyEditText =  new MyEditText(text, pMB->m_DataBase);
          if (pMyEditText==0)     return;
          QObject::connect( pMyEditText,   SIGNAL( Sign_Exe_Mixture(QString&)),   G_pCApp->m_pDrTux,  SLOT  ( Slot_ExeMixture(QString&))  );
          //pMyEditText->show();            // pour debug
          pMyEditText->moveCursor (QTextEdit::MoveEnd);
          int ori_para, ori_index;
          pMyEditText->getCursorPosition (&ori_para, &ori_index );
          pMyEditText->DoContextListPopup(&listInUse);
          pMyEditText->setSelection(0, 0, ori_para, ori_index);
          pMyEditText->del();
          text = pMyEditText->text();
          delete pMyEditText;
          pC_DragQListViewItem = GetListViewItem_FromPkEvnt(evntPk); // recuperer les meubles car liste peut avoir ete modifiee par les macros
          //...................... si changement de liste le repercuter ......................................
          if (listInUse.length() && listeDeroulante != listInUse)
             {setProfileValue(pC_DragQListViewItem->text(0), "menuProp"+QString::number(c-CP_Deb), listInUse);
              //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
              saveUserProfil();
              setListViewHeaderOnItem(pC_DragQListViewItem);
              text = "";
             }
          //................................. inserer le texte ..............................................
          CHtmlTools::HtmlToAscii(text,CHtmlTools::StrictASCII);
          if (text.length())
             {text = text.stripWhiteSpace();
              text = text.remove('\r');
              text = text.remove('\n');
              pC_DragQListViewItem->setText(c,text);
              switch(c-CP_Deb)
                    {case 0: pCRubRecord->m_Prop_0 = text; break;
                     case 1: pCRubRecord->m_Prop_1 = text; break;
                     case 2: pCRubRecord->m_Prop_2 = text; break;
                     case 3: pCRubRecord->m_Prop_3 = text; break;
                     case 4: pCRubRecord->m_Prop_4 = text; break;
                    }  // end switch(c-CP_Deb)
              G_pCApp->putCurrentRubriqueOnModifiedState();
              pQListViewItem = GetListViewItem_FromPkEvnt(evntPk);
              //pQListViewItem = GetListViewItem_FromPkEvnt(pk);
              if (pQListViewItem)
                 {pQListViewItem->setSelected ( TRUE ) ;
                  m_ListView_MonitorPatient->setCurrentItem ( pQListViewItem );
                  //m_ListView_MonitorPatient->ensureItemVisible ( pQListViewItem );
                  m_ListView_MonitorPatient->verticalScrollBar()->setValue(xs);
                 }
             } // end if (text.length())
        } // end if (listeDeroulante.length())
    else if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"glc"))
        {QString itemSetProp   = tr("Choisir une liste déroulante pour cette propriété ...");
         menuList.append(QString("#ToolGestList_Item.png#") + itemSetProp);
         menuList.append("#exit.png#Ne rien faire et quitter ce menu");
         QString menuItem = G_pCApp->DoPopupList(menuList);
         if (menuItem == itemSetProp)
            {  menuItem = "";
               switch(c-CP_Deb)
                     {case 0: menuItem =setMenuList(pC_DragQListViewItem->text(0), "menuProp0"); break;
                      case 1: menuItem =setMenuList(pC_DragQListViewItem->text(0), "menuProp1"); break;
                      case 2: menuItem =setMenuList(pC_DragQListViewItem->text(0), "menuProp2"); break;
                      case 3: menuItem =setMenuList(pC_DragQListViewItem->text(0), "menuProp3"); break;
                      case 4: menuItem =setMenuList(pC_DragQListViewItem->text(0), "menuProp4"); break;
                     }
               if (menuItem.length())
                  {pC_DragQListViewItem->setPixmap ( c, m_IsMenuHerePixmap );
                   clearListViewHeaderOnItem();
                   setListViewHeaderOnItem(pC_DragQListViewItem);
                   pQListViewItem = GetListViewItem_FromPkEvnt(evntPk);
                   //pQListViewItem = GetListViewItem_FromPkEvnt(pk);
                   if (pQListViewItem)
                      {pQListViewItem->setSelected ( TRUE ) ;
                       m_ListView_MonitorPatient->setCurrentItem ( pQListViewItem );
                       //m_ListView_MonitorPatient->ensureItemVisible ( pQListViewItem );
                       m_ListView_MonitorPatient->verticalScrollBar()->setValue(xs);
                      }
                  } // end if (menuItem.length())
            } // end if (menuItem == itemSetProp)
        }
    return;
   } // end if (c >= CP_Deb && c <= CP_Fin)
 //..................... batir le menu contextuel ....................................................
 QString itemDel        = tr("Positionner cet élément sur effacement");
 QString itemDelFic     = tr("Positionner cet élément et les fiches allant avec, sur effacement");
 QString itemOklFic     = tr("Retirer cet élément et les fiches allant avec, de ceux à effacer");
 QString itemOk         = tr("Retirer cet élément de ceux à effacer");
 QString itemDateDeb    = tr("Modifier la date et heure de début");
 QString itemDateFin    = tr("Modifier la date et heure de Fin");

 QString itemSetOpen    = tr("Cet élément doit être ouvert par défaut");
 QString itemSetClose   = tr("Cet élément doit être fermé  par défaut");
 QString itemIsUnique   = tr("Cet élément doit être le seul de la fratrie");
 QString itemIsMultip   = tr("Cet élément peut être multiple dans la fratrie");
 QString itemSetIcone   = tr("Choisir une icône pour cet élément...");
 QString itemRemIcone   = tr("Retirer l'icône pour cet élément...");
 QString itemSetProp0   = tr("Choisir une liste déroulante pour la propriété N°1...");
 QString itemSetProp1   = tr("Choisir une liste déroulante pour la propriété N°2...");
 QString itemSetProp2   = tr("Choisir une liste déroulante pour la propriété N°3...");
 QString itemSetProp3   = tr("Choisir une liste déroulante pour la propriété N°4...");
 QString itemSetProp4   = tr("Choisir une liste déroulante pour la propriété N°5...");
 QString itemDelProp0   = tr("Retirer la liste déroulante de la propriété N°1...");
 QString itemDelProp1   = tr("Retirer la liste déroulante de la propriété N°2...");
 QString itemDelProp2   = tr("Retirer la liste déroulante de la propriété N°3...");
 QString itemDelProp3   = tr("Retirer la liste déroulante de la propriété N°4...");
 QString itemDelProp4   = tr("Retirer la liste déroulante de la propriété N°5...");
 QString addToFilter    = tr("Ajouter ce classeur à la liste du filtre en cours");
 QString newClasseur    = tr("Créer un nouveau classeur");

 menuList.append(QString("#16x16/calendrierDeb.png#") + itemDateDeb);
 menuList.append(QString("#16x16/calendrierFin.png#") + itemDateFin);
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"adm"))
   {menuList.append("----");
    menuList.append(QString("#16x16/Vigie_annule.png#")    + itemDel);
    menuList.append(QString("#16x16/Vigie_ok.png#")        + itemOk);
    menuList.append("----");
    menuList.append(QString("#16x16/Vigie_annuleFic.png#") + itemDelFic);
    menuList.append(QString("#16x16/Vigie_OkFic.png#")     + itemOklFic);
   }
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"glc")||G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"adm"))
   {menuList.append("----");
    menuList.append(QString("#16x16/setToOpen.png#")    + itemSetOpen);
    menuList.append(QString("#16x16/setToClose.png#")   + itemSetClose);
    menuList.append("----");
    menuList.append(QString("#16x16/IsMulti.png#")      + itemIsMultip);
    menuList.append(QString("#16x16/IsClasseur.png#")   + itemIsUnique);
    menuList.append("----");
    menuList.append(QString("#16x16/setIcone.png#")     + itemSetIcone);
    menuList.append(QString("#16x16/RemIcone.png#")     + itemRemIcone);
    menuList.append("----");
    menuList.append(QString("#16x16/IsMenuHere.png#")   + itemSetProp0);
    menuList.append(QString("#16x16/IsMenuHere.png#")   + itemSetProp1);
    menuList.append(QString("#16x16/IsMenuHere.png#")   + itemSetProp2);
    menuList.append(QString("#16x16/IsMenuHere.png#")   + itemSetProp3);
    menuList.append(QString("#16x16/IsMenuHere.png#")   + itemSetProp4);
    menuList.append("----");
    menuList.append(QString("#16x16/IsNotMenuHere.png#")   + itemDelProp0);
    menuList.append(QString("#16x16/IsNotMenuHere.png#")   + itemDelProp1);
    menuList.append(QString("#16x16/IsNotMenuHere.png#")   + itemDelProp2);
    menuList.append(QString("#16x16/IsNotMenuHere.png#")   + itemDelProp3);
    menuList.append(QString("#16x16/IsNotMenuHere.png#")   + itemDelProp4);
    menuList.append("----");
    menuList.append(QString("#16x16/FiltrePlus.png#")      + addToFilter);
    menuList.append(QString("#16x16/NewClasseur.png#")     + newClasseur);
  }
 QString menuItem = G_pCApp->DoPopupList(menuList);
 if (menuItem.length()==0) return;
 //...................... juste retiré du système de classement (pas du dossier) .................
 if (menuItem==itemDel)
    {pCRubEvntPair->setState(TO_DELETE);
     m_pEvnmtList->Evnmt_Set_StateOn_AllFils(pCRubEvntPair->m_Pk, TO_DELETE);
    }
 else if(menuItem==itemOk)
    {if (m_pEvnmtList->FirstStateFindInParents( pCRubEvntPair, TO_DELETE))
        {displayWarningItemInEraseState();
         return;
        }
     pCRubEvntPair->removeState(TO_DELETE|TO_REMOVE);
     m_pEvnmtList->Evnmt_Remove_StateOn_AllFils(pCRubEvntPair->m_Pk, TO_DELETE|TO_REMOVE);
    }
 //...................... effacement du système de classement et du dossier.................
 else if (menuItem==itemDelFic)
    { //..... l'item .......
      pCRubEvntPair->setState(TO_DELETE|TO_REMOVE);
      CRubRecord* pCRubRecord = pMB->RubRecordFromPk( m_pRubList, pCRubEvntPair->m_PkDoc);
      if (pCRubRecord) pCRubRecord->m_State |= RUB_IS_TO_DELETE;
      //.. les fils.........
      Evnmt_Set_StateOn_AllFilsAndDoc(pCRubEvntPair->m_Pk, TO_DELETE|TO_REMOVE);
    }
 else if (menuItem==itemOklFic)
    { if (m_pEvnmtList->FirstStateFindInParents( pCRubEvntPair, TO_DELETE))
         {displayWarningItemInEraseState();
          return;
         }
      //..... l'item .......
      pCRubEvntPair->removeState(TO_DELETE|TO_REMOVE);
      CRubRecord* pCRubRecord = pMB->RubRecordFromPk( m_pRubList, pCRubEvntPair->m_PkDoc);
      if (pCRubRecord) pCRubRecord->m_State &= ~RUB_IS_TO_DELETE;
      //.. les fils.........
      Evnmt_Set_StateOn_AllFilsAndDoc(pCRubEvntPair->m_Pk, C_Organiseur::toKeep);
    }
 else if (menuItem==itemDateDeb)
    {  ChangeDateTime(pC_DragQListViewItem, pCRubRecord, "DateFin");
       return;
    }
 else if (menuItem==itemDateFin)
    {  ChangeDateTime(pC_DragQListViewItem, pCRubRecord, "DateDeb");
       return;
    }
 else if(menuItem==itemSetOpen)
    {setProfileValue(pC_DragQListViewItem->text(0), "isOpen", "1");
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
     saveUserProfil();
     setItemOnProfile(pC_DragQListViewItem);
    }
 else if(menuItem==itemSetClose)
    {setProfileValue(pC_DragQListViewItem->text(0), "isOpen", "0");
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
     saveUserProfil();
     setItemOnProfile(pC_DragQListViewItem);
    }
 else if(menuItem==itemSetIcone)
    {QString icone;
     if (pushButton_SetIcone_clicked(icone))
        {setProfileValue(pC_DragQListViewItem->text(0), "icone", icone);
         //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
         saveUserProfil();
         //pC_DragQListViewItem->setPixmap ( 0, m_PathProfiles + "Icones/" + icone );
         setItemOnProfile(pC_DragQListViewItem);
        }
    }
 else if(menuItem==itemRemIcone)
    {setProfileValue(pC_DragQListViewItem->text(0), "icone", "");
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
     saveUserProfil();
     //pC_DragQListViewItem->setPixmap ( 0, 0 );
     setItemOnProfile(pC_DragQListViewItem);
    }
 else if(menuItem==itemSetProp0)
    {setMenuList(pC_DragQListViewItem->text(0), "menuProp0");
    }
 else if(menuItem==itemSetProp1)
    {setMenuList(pC_DragQListViewItem->text(0), "menuProp1");
    }
 else if(menuItem==itemSetProp2)
    {setMenuList(pC_DragQListViewItem->text(0), "menuProp2");
    }
 else if(menuItem==itemSetProp3)
    {setMenuList(pC_DragQListViewItem->text(0), "menuProp3");
    }
 else if(menuItem==itemSetProp4)
    {setMenuList(pC_DragQListViewItem->text(0), "menuProp4");
    }
 else if(menuItem==itemDelProp0)
    {removeMenuList(pC_DragQListViewItem->text(0), "menuProp0");
     pC_DragQListViewItem->setPixmap ( CP_Deb+0, 0 );
     pC_DragQListViewItem->setText   ( CP_Deb+0, "" );
     return;
    }
 else if(menuItem==itemDelProp1)
    {removeMenuList(pC_DragQListViewItem->text(0), "menuProp1");
     pC_DragQListViewItem->setPixmap ( CP_Deb+1, 0 );
     pC_DragQListViewItem->setText   ( CP_Deb+1, "" );
     return;
    }
 else if(menuItem==itemDelProp2)
    {removeMenuList(pC_DragQListViewItem->text(0), "menuProp2");
     pC_DragQListViewItem->setPixmap ( CP_Deb+2, 0 );
     pC_DragQListViewItem->setText   ( CP_Deb+2, "" );
     return;
    }
 else if(menuItem==itemDelProp3)
    {removeMenuList(pC_DragQListViewItem->text(0), "menuProp3");
     pC_DragQListViewItem->setPixmap ( CP_Deb+3, 0 );
     pC_DragQListViewItem->setText   ( CP_Deb+3, "" );
     return;
    }
 else if(menuItem==itemDelProp4)
    {removeMenuList(pC_DragQListViewItem->text(0), "menuProp4");
     pC_DragQListViewItem->setPixmap ( CP_Deb+4, 0 );
     pC_DragQListViewItem->setText   ( CP_Deb+4, "" );
     return;
    }

 else if(menuItem==itemIsUnique)
    {setProfileValue(pC_DragQListViewItem->text(0), "IsUnique", "1");
     //pC_DragQListViewItem->setPixmap ( 1, m_IsClasseurPixmap );
     saveUserProfil();
     setItemOnProfile(pC_DragQListViewItem);
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
    }
 else if(menuItem==itemIsMultip)
    {setProfileValue(pC_DragQListViewItem->text(0), "IsUnique", "0");
     //pC_DragQListViewItem->setPixmap ( 1,  0);    //
     saveUserProfil();
     setItemOnProfile(pC_DragQListViewItem);
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
    }
 else if(menuItem==addToFilter)
    {//setProfileValue(pC_DragQListViewItem->text(0), "IsUnique", "0");
     //pC_DragQListViewItem->setPixmap ( 1,  0);    //
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
     if (comboBox_FiltreVisu->count()==0)
        {CouCou(tr("Pas de filtre à paramétrer car liste vide "));
         return;
        }
     QString filterName  = comboBox_FiltreVisu->currentText();
     QString fullpath    = GetItemHierarchie(pC_DragQListViewItem);
     QString xml         = getFilterValue(filterName, "authorized");
     if (xml.length()) xml += "\n";
     xml += fullpath;
     setFilterValue(filterName, "authorized", xml);
     return;
    }
else if(menuItem==newClasseur)
    { //................. choisir le fichier image src à insérer .................................................
     QFileDialog *fd = new QFileDialog  ( G_pCApp->m_PathAppli + "Ressources/Documents par defaut",
                                         tr( "Fichiers document") + QString(" *.html *.txt ;;") + tr("Tous les fichiers (*)" ),
                                         this,
                                         "OuvrirDlg"
                                        ) ;
    if (fd==0)                                            return;
    if ( ! (fd->exec() == QDialog::Accepted) )
       {delete fd;
        return;
       }
    QString filename = fd->selectedFile();
    delete fd;
    if ( !filename.isEmpty() && QFile::exists( filename ))
       {QString toImport;
        CGestIni::Param_UpdateFromDisk(filename, toImport);
        QString rubriqueCible = "Vigie";
        CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
        if (pCMDI_Generic==0) G_pCApp->m_pDrTux->OnActiverOrCreateRubrique(rubriqueCible);
        pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
        if (pCMDI_Generic==0) return;
        pCMDI_Generic->AddNewDocument(toImport,  pCMDI_Generic->GetType(), filename, 0,0,0, 1);
       }
    }
//...................... reafficher la liste ...........................................
makeListeMonitor();

pQListViewItem = GetListViewItem_FromPkEvnt(evntPk);
if (pQListViewItem)
   {pQListViewItem->setSelected ( TRUE ) ;
    m_ListView_MonitorPatient->setCurrentItem ( pQListViewItem );
    m_ListView_MonitorPatient->verticalScrollBar()->setValue(xs);
    m_ListView_MonitorPatient->ensureItemVisible ( pQListViewItem );
   }
}

//----------------------------------------------- ChangeDateTime ---------------------------------------------------
void C_Organiseur::ChangeDateTime(C_DragQListViewItem *pC_DragQListViewItem, CRubRecord *pCRubRecord, const QString &debOrEnd)
{       QString txt_date;
        Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        if(debOrEnd=="DateDeb")   {txt_date = pC_DragQListViewItem->text(DT_Deb); dlg->setCaption(tr("Date de début")); dlg->setIcon ( Theme::getIcon("16x16/calendrierDeb.png")); }
        else                      {txt_date = pC_DragQListViewItem->text(DT_Fin); dlg->setCaption(tr("Date de fin"));   dlg->setIcon ( Theme::getIcon("16x16/calendrierFin.png"));}
        txt_date += ":00";
        Dlg_DateInit(dlg);
        if ( ! dlg->setDateTimeNatural_jjMMyyyy_Order(txt_date ))
           {dlg->setDate  (QDate::currentDate());
            dlg->setHeure (QTime::currentTime());
           }
        if (dlg->exec()== QDialog::Accepted )
           {int isInValid_deb, isInValid_fin;

            txt_date = dlg->getDate().toString("yyyy-MM-dd") + "T" + dlg->timeEdit_Heure->time ().toString("hh:mm:ss");
            if(debOrEnd=="DateDeb")
              {//.......... controle des saisies ..............
               QDateTime dt_deb = CGenTools::setQDateTime(dlg->getDate().toString("dd-MM-yyyy") + dlg->timeEdit_Heure->time ().toString("hh:mm:ss"), isInValid_deb);
               QDateTime dt_fin = CGenTools::setQDateTime(pC_DragQListViewItem->text(DT_Fin), isInValid_fin);
               if (isInValid_deb) CouCou(tr("La date de début est invalide"));
               else
                  { if (dt_deb<dt_fin || isInValid_fin)
                       {pC_DragQListViewItem->setText(DT_Deb, QueryDateHeureToNatural(txt_date));
                        pCRubRecord->m_Date = txt_date;
                        G_pCApp->putCurrentRubriqueOnModifiedState();
                       }
                    else if ( !isInValid_fin) CouCou(tr("La date de début ne peut être supérieure à la date de fin"));
                  }
              }
            else
              {QDateTime dt_deb = CGenTools::setQDateTime(pC_DragQListViewItem->text(DT_Deb), isInValid_deb);
               QDateTime dt_fin = CGenTools::setQDateTime(dlg->getDate().toString("dd-MM-yyyy") + dlg->timeEdit_Heure->time ().toString("hh:mm:ss"), isInValid_fin);
               if (isInValid_deb) CouCou(tr("La date de début est invalide"));
               else
                  { if (dt_deb<dt_fin || isInValid_fin)
                       {pC_DragQListViewItem->setText(DT_Fin, QueryDateHeureToNatural(txt_date));
                        pCRubRecord->m_Fin  = txt_date;
                        G_pCApp->putCurrentRubriqueOnModifiedState();
                       }
                    else if (!isInValid_fin) CouCou(tr("La date de fin ne peut être inférieure à la date de début"));
                  }
              }
           }
        delete dlg;
}

//----------------------------------------------- Evnmt_Set_StateOn_AllFilsAndDoc ---------------------------------------------------
int C_Organiseur::Evnmt_Set_StateOn_AllFilsAndDoc(QString pk_Pere, int state)
{ CRubEvntList::iterator it;
  for (it = m_pEvnmtList->begin(); it !=  m_pEvnmtList->end(); ++it )
      {if (pk_Pere == (*it).m_Pere)
          {if (state&C_Organiseur::toDelete)
              {(*it).setState(state);
               CRubRecord* pCRubRecord = pMB->RubRecordFromPk( m_pRubList, (*it).m_PkDoc);
               if (pCRubRecord) pCRubRecord->m_State |= RUB_IS_TO_DELETE;
               //pMB->RubList_SetItemDeleted(m_pRubList, (*it).m_PkDoc.toInt());
              }
           else
              {(*it).removeState(TO_DELETE);
               CRubRecord* pCRubRecord = pMB->RubRecordFromPk( m_pRubList, (*it).m_PkDoc);
               if (pCRubRecord) pCRubRecord->m_State &= ~RUB_IS_TO_DELETE;
               //pMB->RubList_SetItemNotDeleted(m_pRubList, (*it).m_PkDoc.toInt());
              }
            while (Evnmt_Set_StateOn_AllFilsAndDoc((*it).m_Pk, state)); // voir si la filliation n'est pas a effacer
          }
      }
 return 0;
}

//------------------------------------ GetItemHierarchie --------------------------------------------------
QString C_Organiseur::GetItemHierarchie(C_DragQListViewItem *pC_DragQListViewItem, int justtDir /* = C_Organiseur::withFileName */)
{    QString fullpath = pC_DragQListViewItem->text(0);
     while ( (pC_DragQListViewItem = (C_DragQListViewItem*)pC_DragQListViewItem->parent()) )
           { if ( pC_DragQListViewItem->parent() ) fullpath = pC_DragQListViewItem->text(0) + "/" + fullpath;
           }

     if (justtDir==C_Organiseur::justtDir)
        {while (!QFile::exists(G_pCApp->m_PathGlossaire+"Vigie/"+fullpath))
               {//qDebug(G_pCApp->m_PathGlossaire+"Vigie/"+fullpath);
                int pos  = fullpath.findRev("/");
                if (pos != -1) fullpath.truncate(pos);
                else break;
               }
         if (QFileInfo(G_pCApp->m_PathGlossaire+fullpath).isFile())
            {int pos  = fullpath.findRev("/");
             if (pos != -1) fullpath.truncate(pos);
            }
        }
     return fullpath;
}
//------------------------------------ setMenuList --------------------------------------------------
QString C_Organiseur::setMenuList(QString classeurName, QString property)
{ QString        listName   = "";
  QString        ret        = QString::null;
  //................ chercher une eventuelle liste déroutante (sic) ........................
  //                 existante deja pour ce menu
  OpClProfiles::Iterator it = m_UserClasseurProfiles.find(classeurName);
  if (it != m_UserClasseurProfiles.end())
     {QString dataToExamine = it.data();
      QString   value       = "";
      if ( CGenTools::GetXMLValue(dataToExamine, QString("<")+property+">", value, 0) !=-1 )  listName = value;
     }
  //..................... dialoguer dialoguez ...............................................
 C_DlgListFieldMngr *dlg = new C_DlgListFieldMngr(0,"ListManager_Dial",TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
 if (dlg ==0) return QString::null;
 dlg->initDialog(G_pCApp->m_DrTuxParam, G_pCApp->m_PathGlossaire, G_pCApp->m_PathAppli);
 if (listName.length()) dlg->selectAndOpenItem(listName);
 dlg->setReturnVal(&listName);      // donner la variable où doit être placée a valeur de retour

 if (dlg->exec() == QDialog::Accepted && listName.length())
    {setProfileValue(classeurName, property, listName);
     //saveProfile(m_PathProfiles+"Profiles/VigieProfile.txt");
     saveUserProfil();
     ret = listName;
    }
 if (G_pCApp->m_pDrTux) G_pCApp->m_pDrTux->recordPathModulesMenus(dlg->m_PathMenuLib);
 delete dlg;
 return ret;
}
//------------------------------------ removeMenuList --------------------------------------------------
QString C_Organiseur::removeMenuList(QString classeurName, QString property)
{ QString        listName   = "";
  setProfileValue(classeurName, property, listName);
  saveUserProfil();
  return QString::null;
}

//------------------------------------ pushButton_SetIcone_clicked --------------------------------------------------
int C_Organiseur::pushButton_SetIcone_clicked(QString &icone)
{ //QString path_dst = G_pCApp->m_PathAppli + "Ressources" + QDir::separator() + "RubriquesTextes";
    if (m_LastImgPath.length()==0) m_LastImgPath = m_PathProfiles +"Icones/";
    QString path_dst = m_PathProfiles +"Icones/";
    //................. choisir le fichier image src à insérer .................................................
    QFileDialog *fd = new QFileDialog  ( m_LastImgPath,
                                         tr( "Fichiers images") + QString("*.png *.png *.jpg *.gif *.bmp ;;") + tr("Tous les fichiers (*)" ),
                                         this,
                                         "OuvrirDlg"
                                        ) ;
    if (fd==0)                                            return 0;
    CPreview* p = new CPreview;
    //fd->setMode(QFileDialog::ExistingFiles);   //preview mode ne fonctionne pas avec ce mode !!
    fd->setInfoPreviewEnabled( TRUE );
    fd->setInfoPreview( p, p );
    fd->setPreviewMode( QFileDialog::Info );
    if ( ! (fd->exec() == QDialog::Accepted) )
       {delete fd;
        return 0;
       }
    QString filename = fd->selectedFile();
    delete fd;
    if ( !filename.isEmpty() && QFile::exists( filename ))
       {filename      = CHtmlTools::Copy_File(filename, path_dst);
        icone         = filename;
        m_LastImgPath = QFileInfo(filename).filePath();
        return 1;
       }
    return 0;
}

//----------------------------------------------- makeListeMonitorFromList ---------------------------------------------------
/*! \brief fonction qui recree toute la listview a partir des listes : m_pEvnmtList et m_pRubList
 *  \param pEvnmtList EVNT_LIST* :  un pointeur sur la liste de l'arborescence des evenenements
 *  \param pRubList   RUBREC_LIST* :  un pointeur sur la liste des documents
*/
/*
              {m_PrimKey        = primKey;               // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_Libelle        = libelle;
               m_Type           = type;
               m_SubType        = subType;
               m_Date           = date;
               m_User           = user;
               m_SignUser       = sign_user;
               m_Ref_Blobs_PK   = refBlobsPk;
               m_State          = 0;
               m_IsRoot         = 0;
               m_Fin            = "";
               m_DureeMod       = 0;
               m_Prop_0         = "";
               m_Prop_1         = "";
               m_Prop_2         = "";
               m_Prop_3         = "";
               m_Prop_4         = "";
*/
void C_Organiseur::makeListeMonitorFromList(EVNT_LIST *pEvnmtList, RUBREC_LIST *pRubList, bool clearListView /* = FALSE */ )
{ if (clearListView) m_ListView_MonitorPatient->clear();
  if (pRubList->size() == 0)                                               return;
  C_DragQListViewItem *pQListViewItem = 0;
  CRubRecord          *pCRubRecord    = 0;
  int isInvalid = 0;
  QDateTime refDeb = CGenTools::setQDateTime( mle_deb->text() + mle_HeureDeb->text() , isInvalid);
  QDateTime RefEnd = CGenTools::setQDateTime( mle_fin->text() + mle_HeureFin->text() , isInvalid);

  EVNT_LIST::iterator it;
  for (it = pEvnmtList->begin(); it !=  pEvnmtList->end(); ++it )
      {if ((*it).m_Pere == "0")
          { pCRubRecord = pMB->RubRecordFromPk( pRubList, (*it).m_PkDoc);
            C_PatientRootItem  *pC_PatientRootItem = m_ListView_MonitorPatient->FindOrCreatePatientRootItem((*it).m_DossPk);
            if ( pC_PatientRootItem && pCRubRecord)
               {int ok = 1;
                if (m_FiltreHierarchList.count()) ok = isThisHierarchAuthorized(pCRubRecord->m_Libelle) + 1;
                              //.................. selon les dates ..........................................
                //   Qt::ISODate - ISO 8601 extended format (YYYY-MM-DD, or with time, YYYY-MM-DDTHH:MM:SS
               QDateTime deb = QDateTime::fromString ( pCRubRecord->m_Date, Qt::ISODate );
               QDateTime end = QDateTime::fromString ( pCRubRecord->m_Fin,  Qt::ISODate );
               if (deb.isValid() && end.isValid() && end<refDeb) ok = 0;

                if (ok && (pQListViewItem  = new C_DragQListViewItem (pC_PatientRootItem->m_pC_DragQListViewItem)))
                   { pC_PatientRootItem->m_pC_DragQListViewItem->SET_PK_EVNT("RA");           // signaler qu'i est il est actif
                     //pC_PatientRootItem->m_pC_DragQListViewItem->setPixmap ( 0, Theme::getIcon("16x16/identityRed.png") );
                     SetListViewItemFromRecord(pQListViewItem, pCRubRecord , &(*it));
                     pQListViewItem->setDropEnabled (TRUE );
                     while (GetFilsFromPereInList(pEvnmtList, pRubList, (*it).m_Pk, pQListViewItem , pCRubRecord->m_Libelle));    // voir si le fils n'a pas lui aussi des fils
                   }
               }
            if ( pC_PatientRootItem) pC_PatientRootItem->m_pC_DragQListViewItem->setPixmap ( 0, Theme::getIcon("16x16/identityRed.png") );
          }
      }
}

//----------------------------------------------- isThisHierarchAuthorized ---------------------------------------------------
int C_Organiseur::isThisHierarchAuthorized(const QString &hierarch )
{//int i = 0;
 if (m_hierarchFilter==FALSE) return 1;
 for ( QStringList::Iterator it = m_FiltreHierarchList.begin(); it != m_FiltreHierarchList.end(); ++it )
     {QString authorized = *it;
      if (hierarch.length()>authorized.length())
         {if ( hierarch.startsWith(authorized) ) return 1;
         }
      else
         {if ( authorized.startsWith(hierarch) ) return 1;
         }
     }
 return -1;
}
//----------------------------------------------- GetFilsFromPereInList ---------------------------------------------------
/*! \brief Fonction recursive auto appelee qui explore tous les fils d'un pere de la liste d'evennements pEvnmtList et cree les items correspondants dans la listview
 *  \param pEvnmtList EVNT_LIST* :  un pointeur sur la liste de l'arborescence des evenenements
 *  \param pRubList   RUBREC_LIST* :  un pointeur sur la liste des documents
 *  \param pk_pere   QString : sur le pk de l'enregistrement pere
 *  \param pQListViewItemPere   C_DragQListViewItem : sur l'item du pere
 *  \return bool TRUE si l'exploration est finie
*/
 int C_Organiseur::GetFilsFromPereInList(EVNT_LIST *pEvnmtList, RUBREC_LIST *pRubList, const QString &pk_pere, C_DragQListViewItem *pQListViewItemPere , const QString &hierarch)
{ C_DragQListViewItem *pQListViewItem = 0;
  CRubRecord          *pCRubRecord    = 0;

  int isInvalid = 0;
  QDateTime refDeb = CGenTools::setQDateTime( mle_deb->text() + mle_HeureDeb->text() , isInvalid);
  QDateTime RefEnd = CGenTools::setQDateTime( mle_fin->text() + mle_HeureFin->text() , isInvalid);

  EVNT_LIST::iterator it;
  for (it = pEvnmtList->begin(); it !=  pEvnmtList->end(); ++it )
      {if ((*it).m_Pere == pk_pere)
          {if ( (pCRubRecord = pMB->RubRecordFromPk( pRubList, (*it).m_PkDoc)) )
              {int ok = 1;
               //................... selon les filtres .....................................
               if (m_FiltreHierarchList.count()) ok = isThisHierarchAuthorized(hierarch+"/"+ pCRubRecord->m_Libelle) + 1;
               //.................. selon les dates ..........................................
               //   Qt::ISODate - ISO 8601 extended format (YYYY-MM-DD, or with time, YYYY-MM-DDTHH:MM:SS
               QDateTime deb = QDateTime::fromString ( pCRubRecord->m_Date, Qt::ISODate );
               QDateTime end = QDateTime::fromString ( pCRubRecord->m_Fin,  Qt::ISODate );
               if (deb.isValid() && end.isValid() && end<refDeb) ok = 0;

               if (ok && (pQListViewItem  = new C_DragQListViewItem (pQListViewItemPere)))
                  { SetListViewItemFromRecord(pQListViewItem, pCRubRecord , &(*it));
                    pQListViewItem->setDropEnabled (TRUE );
                    while (GetFilsFromPereInList(pEvnmtList, pRubList, (*it).m_Pk, pQListViewItem, hierarch+"/"+ pCRubRecord->m_Libelle ));    // voir si le fils n'a pas lui aussi des fils
                  }
              }
          }
      }
  return 0;
}

//----------------------------------------------- GetListViewItem_FromPkEvnt ---------------------------------------------------
/*! \brief Fonction qui explore la ListView àa la recherche d'un tem avec un EvntPk donne
 *  \param evntPk  : const QString &  Pk a rechercher
 *  \return evntPk  : C_DragQListViewItem * pointeur sur l'item trouve zero si non trouve.
*/
C_DragQListViewItem* C_Organiseur::GetListViewItem_FromPkEvnt(const QString &eventPk)
{C_DragQListViewItem *pC_DragQListViewItem;
 QListViewItemIterator it( m_ListView_MonitorPatient );
 while ( it.current() )
        {pC_DragQListViewItem = (C_DragQListViewItem*)it.current();
         if (pC_DragQListViewItem->GET_PK_EVNT()==eventPk) return pC_DragQListViewItem;
        ++it;
        }
 return 0;
}

//----------------------------------------------- GetListViewItem_FromPkEvnt ---------------------------------------------------
/*! \brief Fonction qui explore la ListView àa la recherche d'un item avec un pkDoc donne
 *  \param evntPk  : const QString &  Pk a rechercher
 *  \return evntPk  : C_DragQListViewItem * pointeur sur l'item trouve zero si non trouve.
*/
C_DragQListViewItem* C_Organiseur::GetListViewItemFromPkDoc(const QString &pkDoc)
{C_DragQListViewItem *pC_DragQListViewItem;
 QListViewItemIterator it( m_ListView_MonitorPatient );
 while ( it.current() )
        {pC_DragQListViewItem = (C_DragQListViewItem*)it.current();
         if (pC_DragQListViewItem->GET_PK_DOC()==pkDoc) return pC_DragQListViewItem;
        ++it;
        }
 return 0;
}

//----------------------------------------------- GetListViewItemROOTFromPkDoss ---------------------------------------------------
/*! \brief Fonction qui retourne l'item ROOT de la listview, selon le pk du dosier
 *  \param pQListView   QListView *:  un pointeur sur la listview
 *  \param pk     QString :  primary key de l'enregistrement a retrouver
 *  \return C_DragQListViewItem* pointeur sur l'item correspondant au primary key fourni en entree ou zero si pas trouve
*/
C_DragQListViewItem* C_Organiseur::GetListViewItemROOTFromPkDoss( const QString &pkDoss)
{ C_DragQListViewItem * pC_DragQListViewItem;
  QListViewItemIterator it( m_ListView_MonitorPatient );
  while ( it.current() )
        { pC_DragQListViewItem = (C_DragQListViewItem*)it.current();
          if (pC_DragQListViewItem->GET_PK_EVNT()[0]=='R' && pC_DragQListViewItem->GET_PK_DOS() == pkDoss) return pC_DragQListViewItem;
          ++it;
        }
  return 0;
}

//----------------------------------- pushButtonDateDeb_clicked() ----------------------------------
void C_Organiseur::pushButtonDateDeb_clicked()
    {   Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        dlg->setCaption(tr("Date de début"));
        Dlg_DateInit(dlg);
        QString text = C_TokenInterpret::toNum(mle_deb->text(),"$allString");
        int jour, mois, annee;
        jour  = text.mid(0,2).toInt();
        mois  = text.mid(2,2).toInt();
        annee = text.mid(4,4).toInt();
        if (QDate::isValid ( annee, mois, jour ))
           {dlg->setDate(QDate(annee, mois, jour));
            dlg->setHeure(mle_HeureDeb->text());
           }
        if (dlg->exec()== QDialog::Accepted )
           {mle_deb->setText(dlg->getDate().toString("ddMMyyyy"));
            mle_HeureDeb->setText(dlg->timeEdit_Heure->time ().toString("hhmm"));
            makeListeMonitor();
           }
        delete dlg;
    }

//----------------------------------- pushButtonDateFin_clicked() ----------------------------------
void C_Organiseur::pushButtonDateFin_clicked()
{       Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        dlg->setCaption(tr("Date de fin"));
        Dlg_DateInit(dlg);
        QString text = C_TokenInterpret::toNum(mle_fin->text(),"$allString");
        int jour, mois, annee;
        jour  = text.mid(0,2).toInt();
        mois  = text.mid(2,2).toInt();
        annee = text.mid(4,4).toInt();
        if (QDate::isValid ( annee, mois, jour ) )
           {dlg->setDate(QDate(annee, mois, jour));
            dlg->setHeure(mle_HeureFin->text());
           }
        if (dlg->exec()== QDialog::Accepted )
           {mle_fin->setText(dlg->getDate().toString("ddMMyyyy"));
            mle_HeureFin->setText(dlg->timeEdit_Heure->time ().toString("hhmm"));
            makeListeMonitor();
           }
        delete dlg;
    }

//----------------------------------- Dlg_DateInit ----------------------------------
void C_Organiseur::Dlg_DateInit(Dlg_Calendar *dlg)
    {   dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
        dlg->setMenuPreselectDateOn();
        dlg->Month_setIcon(    Theme::getIcon("16x16/1leftarrow.png")   , Theme::getIcon("16x16/1rightarrow.png") );
        dlg->Year_setIcon(     Theme::getIcon("16x16/2leftarrow.png") ,   Theme::getIcon("16x16/2rightarrow.png") );
        dlg->DateMenu_setIcon( Theme::getIcon("16x16/DatePreselect.png")  );
        QStringList  preselectList;
        if (GetPreselectDateList(preselectList))  dlg->setMenuPreSelectDate(preselectList);
    }

//----------------------------------- GetPreselectDateList ----------------------------------
int C_Organiseur::GetPreselectDateList(QStringList & rQStringList)
{QString      preselectStr;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "Ressources/PreselectDates.txt", preselectStr);
 if (preselectStr.length()==0) return 0;
 preselectStr = preselectStr.stripWhiteSpace();
 preselectStr = preselectStr.remove("\r");
 preselectStr = preselectStr.remove("\n");
 rQStringList = QStringList::split (';', preselectStr);
 return rQStringList.count();
}

//----------------------------------------------- MakeSQLFieldSelect ---------------------------------------------------
QString C_Organiseur::MakeSQLFieldSelect()
{return  QString("SELECT ")                                                       +
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_NOM_DATE      + ", " +     // 0
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_TYPE          + ", " +     // 1
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_SUBTYPE       + ", " +     // 2
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_DATE          + ", " +     // 3
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_FIN           + ", " +     // 4
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_GUID          + ", " +     // 5
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PRIMKEY       + ", " +     // 6
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_DUREE_MOD     + ", " +     // 7
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PROP_0        + ", " +     // 8
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PROP_1        + ", " +     // 9
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PROP_2        + ", " +     // 10
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PROP_3        + ", " +     // 11
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PROP_4        + ", " +     // 12
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_REF_DOSS_PK   + ", " +     // 13
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_USER          + ", " +     // 14
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_SIGN_USER     + ", " +     // 15
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_REF_BLOBS_PK  + ", " +     // 16

      pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PK                    + ", " +     // 17
      pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_DOSS_PK                    + ", " +     // 18
      pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PERE                  + ", " +     // 19
      pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PKDOC                 + "  " ;     // 20
}

//----------------------------------------------- queryToRecord ---------------------------------------------------
CRubRecord C_Organiseur::queryToRecord(QSqlQuery &query)
{ return                             CRubRecord (  query.value( 6 ).toString(),            // primary key
                                                   CGestIni::Utf8_Query(query, 0 ),        // libelle
                                                   query.value( 1 ).toString(),            // type
                                                   CGestIni::Utf8_Query(query, 2 ),        // sub type
                                                   query.value( 3 ).toString(),            // date
                                                   query.value( 14).toString(),            // user
                                                   query.value( 15).toString(),            // signuser,
                                                                             0,            // data
                                                                             0,            // len data
                                                   query.value( 16).toString(),            // refblobpk
                                                   query.value( 4).toString(),             // fin
                                                   query.value( 7).toInt(),                // duree
                                                   CGestIni::Utf8_Query(query, 8 ),        // prop-0
                                                   CGestIni::Utf8_Query(query, 9 ),        // prop-1
                                                   CGestIni::Utf8_Query(query, 10),        // prop-2
                                                   CGestIni::Utf8_Query(query, 11),        // prop-3
                                                   CGestIni::Utf8_Query(query, 12)         // prop-4
                                               );
}

//----------------------------------------------- queryToEvnt ---------------------------------------------------
CRubEvntPair C_Organiseur::queryToEvnt(QSqlQuery &query)
{return                            CRubEvntPair(query.value( 19).toString(),    // const char* pere,
                                                query.value( 20).toString(),    // const char* pkDoc,
                                                query.value( 17).toString(),    // const char* pk,
                                                query.value( 18).toString()     // const char* dossPk
                                               );
}


//----------------------------------------------- makeListeMonitorFromSQL ---------------------------------------------------
/*
   QString      m_DOSS_RUB_HEAD_TBL_NAME;        // nom de la table de l'header des RUBRIQUES
   QString      m_DOSS_RUB_HEAD_PRIMKEY;         // clef primaire de cet enregistrement
   QString      m_DOSS_RUB_HEAD_GUID;            // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_RUB_HEAD_TYPE;            // Type de rubrique
   QString      m_DOSS_RUB_HEAD_NOM_DATE;        // Court texte descriptif de l'enregistrement.
   QString      m_DOSS_RUB_HEAD_SUBTYPE;         // sous Type de rubrique (sert notamment à orienter sur un modèle d'ipression)
   QString      m_DOSS_RUB_HEAD_DATE;            // date de l'enregistrement
   QString      m_DOSS_RUB_HEAD_USER;            // utilisateur associé à rubrique
   QString      m_DOSS_RUB_HEAD_SIGN_USER;       // utilisateur associé signataire de la rubrique
   QString      m_DOSS_RUB_HEAD_FIN;             // date de fin de l'enregistrement
   QString      m_DOSS_RUB_HEAD_DUREE_MOD;       // duree de vie du document en modification
   QString      m_DOSS_RUB_HEAD_PROP_0;          // Propriete n 0   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_1;          // Propriete n 1   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_2;          // Propriete n 2   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_3;          // Propriete n 3   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_4;          // Propriete n 4   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_ISROOT;          // champ definissant si cet enregistrement est une racine dans l'organiseur
   QString      m_DOSS_RUB_HEAD_REF_DOSS_PK;     // Clef primaire du dossier auquel l'enregistrement fait reference (doublon avec m_DOSS_RUB_HEAD_GUID)
   QString      m_DOSS_RUB_HEAD_REF_BLOBS_PK;    // reference au Pk du blobs

   QString      m_DOSS_INDEX_TBL_NAME;         // non de la table DU FICHIER GENERAL D'INDEX DES DOSSIERS
   QString      m_DOSS_INDEX_GUID;             // un champ GUID unique qualifiant le numero de dossier
   QString      m_DOSS_INDEX_NOM;              // un champ NOM   qualifiant le nom du dossier
   QString      m_DOSS_INDEX_PRENOM;           // un champ PRENOM   qualifiant le prenom du dossier
   QString      m_DOSS_INDEX_PRIM_KEY;         // une clef primaire (doublon avec le GUID) mais permettant acces rapide

   QString      m_EVNT_LIST_TBL_NAME;          // nom de la table des droits des utilisateurs non prescripteurs
   QString      m_EVNT_LIST_PK;                // clef primaire de cet enregistrement
   QString      m_EVNT_DOSS_PK;                // clef primaire du dossier patient duquel depend ce patient
   QString      m_EVNT_LIST_PERE;              // clef primaire d'un enr pere de la table m_EVNT_LIST_TBL_NAME
   QString      m_EVNT_LIST_PKDOC;             // clef primaire d'un enr fils de la table DOSS_RUB_HEAD_TBL_NAME

*/
// Filtres : je ne veux que ce qui concerne tel libelle exemple pansements
void C_Organiseur::makeListeMonitorFromSQL(const QString & /*filtre*/ , const QDateTime &dateTimeDeb, const QDateTime &dateTimeFin)
{   QString reqDateDebStr = dateTimeDeb.date().toString("yyyyMMdd");
    QString reqDateFinStr = dateTimeFin.date().toString("yyyyMMdd");
    bool     isDossListDo = FALSE;
    QString headerRequete = MakeSQLFieldSelect();
    //....................... filtrer sur les dates et les classeurs racines ..........................................................
    QString requete       = headerRequete                                +
                          " FROM "       + pMB->m_DOSS_RUB_HEAD_TBL_NAME +
                          " INNER JOIN " + pMB->m_EVNT_LIST_TBL_NAME     + " ON " +
                          pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PRIMKEY + "=" +
                          pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PKDOC +
                          " WHERE " + pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_DATE      + ">= '" + reqDateDebStr + mle_HeureDeb->text() +"00' AND " +
                                      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_DATE      + "<= '" + reqDateFinStr + mle_HeureFin->text() +"00' AND ";
//                         if (filtre.length() && filtre != tr("Tous les classeurs visibles") )
//                                      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_NOM_DATE  + " = '" + filtre                               +"'   AND ";
                          requete  += pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PERE              + " = '0'";

            //.............................. cette premiere boucle ne selectionne que les classeurs racines .................................
            //                                                 (pMB->m_EVNT_LIST_PERE = "0")
            if (pMB->OpenBase()==0)  return ;  // si point = erreur d'ouverture de la base
                C_DragQListViewItem *pQListViewItem = 0;
                QSqlQuery query(requete , pMB->m_DataBase );
                if (query.isActive())
                   { m_ListView_MonitorPatient->clear();
                     while (query.next())
                           {
                            //............ si c'est le dossier en cours on se sert des elements en cours ................
                            if (query.value( 18).toString()==m_DossPk)
                               { if (isDossListDo==FALSE)
                                     {makeListeMonitorFromList(m_pEvnmtList, m_pRubList, FALSE);
                                      pMB->OpenBase(); // car fermee lors de makeListeMonitorFromList
                                      isDossListDo = TRUE;
                                     }
                               }
                            else
                               { pQListViewItem = 0;
                                 CRubRecord rubRecord(  query.value( 6 ).toString(),            // primary key
                                                   CGestIni::Utf8_Query(query, 0 ),        // libelle
                                                   query.value( 1 ).toString(),            // type
                                                   CGestIni::Utf8_Query(query, 2 ),        // sub type
                                                   query.value( 3 ).toString(),            // date
                                                   query.value( 14).toString(),            // user
                                                   query.value( 15).toString(),            // signuser,
                                                                             0,            // data
                                                                             0,            // len data
                                                   query.value( 16).toString(),            // refblobpk
                                                   query.value( 4).toString(),             // fin
                                                   query.value( 7).toInt(),                // duree
                                                   CGestIni::Utf8_Query(query, 8 ),        // prop-0
                                                   CGestIni::Utf8_Query(query, 9 ),        // prop-1
                                                   CGestIni::Utf8_Query(query, 10),        // prop-2
                                                   CGestIni::Utf8_Query(query, 11),        // prop-3
                                                   CGestIni::Utf8_Query(query, 12)         // prop-4
                                               );
                                 CRubEvntPair crpair(query.value( 19).toString(),    // const char* pere,
                                                query.value( 20).toString(),    // const char* pkDoc,
                                                query.value( 17).toString(),    // const char* pk,
                                                query.value( 18).toString()     // const char* dossPk
                                               );
                                 //................... rechercher le patient de ce classeur racine ....................................
                                 C_PatientRootItem *pC_PatientRootItem = m_ListView_MonitorPatient->GetPatientRootItem(crpair.m_DossPk);
                                 if (pC_PatientRootItem==0)  // si pas trouve on cree l'item racine Patient (nom prenom)
                                    {pC_PatientRootItem = m_ListView_MonitorPatient->CreatePatientRootItem(crpair.m_DossPk , FALSE);
                                    }
                                 int ok = 1;
                                 QString lib = rubRecord.m_Libelle;
                                 if (m_FiltreHierarchList.count()) ok = isThisHierarchAuthorized(rubRecord.m_Libelle) + 1;
                                  //...................... recuperer l'item racine Patient (nom prenom) ................................
                                 if (ok)
                                    {if (pC_PatientRootItem)    pQListViewItem     = new C_DragQListViewItem(pC_PatientRootItem->m_pC_DragQListViewItem);
                                     if (pQListViewItem)
                                         {  SetListViewItemFromRecord( pQListViewItem, &rubRecord , &crpair);   // renseigner l'item de la listview
                                         }
                                    }
                                 //pQListViewItem->setDropEnabled ( FALSE );
                                 //pQListViewItem->setEnabled ( FALSE );
                                 if (pQListViewItem) GetEvenementsFilsFromSQL(  pQListViewItem, crpair, headerRequete, rubRecord.m_Libelle);  // rehercher les enfants
                                }
                           } //end while (query.next())
                    // peut être FALSE au cas où il n'a pas encore été enregistré (cas du patient en cours avec des classeurs nouveaux)
                    // et alors query.value( 18).toString()==m_DossPk ne peut pas fonctionner
                    if (isDossListDo==FALSE)
                       {makeListeMonitorFromList(m_pEvnmtList, m_pRubList, FALSE);
                        pMB->OpenBase(); // car fermee lors de makeListeMonitorFromList
                        isDossListDo = TRUE;
                       }
                   } //if (query.isActive())
                else
                   {pMB->OutSQL_error(query, "makeListeMonitor( )" , requete );
                   }
             pMB->CloseBase();
}
//----------------------------------------------- GetEvenementsFils ---------------------------------------------------
 QListViewItem* C_Organiseur::GetEvenementsFilsFromSQL(C_DragQListViewItem *pQListViewItem, const CRubEvntPair &crpair, const QString &headerRequete, const QString &hierarch)
{     QString requete =     headerRequete                              +
      " FROM "       + pMB->m_EVNT_LIST_TBL_NAME                       +
      " INNER JOIN " + pMB->m_DOSS_RUB_HEAD_TBL_NAME                   + " ON "    +
      pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PKDOC             + "="       +
      pMB->m_DOSS_RUB_HEAD_TBL_NAME+"."+pMB->m_DOSS_RUB_HEAD_PRIMKEY   + " WHERE " +
      pMB->m_EVNT_LIST_TBL_NAME+"."+pMB->m_EVNT_LIST_PERE              + "="       + crpair.m_Pk;

  QSqlQuery query(requete , pMB->m_DataBase );
  if (query.isActive())
     {while (query.next())
            {
                            CRubRecord rubRecord(  query.value( 6 ).toString(),            // primary key
                                                   CGestIni::Utf8_Query(query, 0 ),        // libelle
                                                   query.value( 1 ).toString(),            // type
                                                   CGestIni::Utf8_Query(query, 2 ),        // sub type
                                                   query.value( 3 ).toString(),            // date
                                                   query.value( 14).toString(),            // user
                                                   query.value( 15).toString(),            // signuser,
                                                                             0,            // data
                                                                             0,            // len data
                                                   query.value( 16).toString(),            // refblobpk
                                                   query.value( 4).toString(),             // fin
                                                   query.value( 7).toInt(),                // duree
                                                   CGestIni::Utf8_Query(query, 8 ),        // prop-0
                                                   CGestIni::Utf8_Query(query, 9 ),        // prop-1
                                                   CGestIni::Utf8_Query(query, 10),        // prop-2
                                                   CGestIni::Utf8_Query(query, 11),        // prop-3
                                                   CGestIni::Utf8_Query(query, 12)         // prop-4
                                               );
                            CRubEvntPair crPair(query.value( 19).toString(),    // const char* pere,
                                                query.value( 20).toString(),    // const char* pkDoc,
                                                query.value( 17).toString(),    // const char* pk,
                                                query.value( 18).toString()     // const char* dossPk
                                               );
               int ok                                   = 1;
               //C_DragQListViewItem *pQListViewItem_Fils = pQListViewItem;
               QString lib = rubRecord.m_Libelle;
               if (m_FiltreHierarchList.count()) ok = isThisHierarchAuthorized(hierarch + "/" + rubRecord.m_Libelle) + 1;
               if (ok && pQListViewItem)
                  {C_DragQListViewItem *pQListViewItem_Fils = new C_DragQListViewItem (pQListViewItem);
                   if (pQListViewItem_Fils)
                      {SetListViewItemFromRecord( pQListViewItem_Fils, &rubRecord , &crPair);
                       while (GetEvenementsFilsFromSQL(pQListViewItem_Fils, crPair, headerRequete, hierarch + "/" + rubRecord.m_Libelle));
                      }
                  }
               //pQListViewItem->setDropEnabled (FALSE );
               //pQListViewItem->setEnabled ( FALSE );
               //else
                  {
                   // while (GetEvenementsFilsFromSQL(pQListViewItem_Fils, crPair, headerRequete, hierarch + "/" + rubRecord.m_Libelle));
                  }
            }
     }
  else
     {pMB->OutSQL_error(query, "GetEvenementsFils( )" , requete /*=0*/);
     }
  return 0;
}
