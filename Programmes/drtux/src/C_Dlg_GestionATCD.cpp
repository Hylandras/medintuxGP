
/******************** C_Dlg_GestionATCD.cpp ***************************************
 *                                                                                *
 *   #include "C_Dlg_GestionATCD.h"                                               *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 ->2009 by Sevin Roland  and Data Medical Design *
 *   E-Mail: roland-sevin@medintux.org                                            *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
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
 #include "C_Dlg_GestionATCD.h"
 #include <qdir.h>
 #include <qfile.h>
 #include <qtextstream.h>
 #include <qpixmap.h>
 #include <qradiobutton.h>

 #include <qstringlist.h>
 #include <qvariant.h>
 #include <qsql.h>
 #include <qsqldriver.h>
 #include <qfiledialog.h>
 #include <qmessagebox.h>
 #include <qmap.h>

 #include <qtextstream.h>
 #include <qpushbutton.h>
 #include <qlabel.h>
 #include <qheader.h>
 #include <qlistview.h>
 #include <qsplitter.h>
 #include <qtabwidget.h>
 #include <qwidget.h>
 #include <qlineedit.h>
 #include <qtextbrowser.h>
 #include <qframe.h>
 #include <qlayout.h>
 #include <qtooltip.h>
 #include <qwhatsthis.h>
 #include <qptrlist.h>
 #include <qcheckbox.h>

 #include "CMoteur_Base.h"
 #include "ui/DlgAtcd_txt.h"
 #include "C_ListViewATCDManager.h"
 #include "C_Dlg_FamilleGenre.h"
 #include "C_KeyPressEater.h"

 #include "../../MedinTuxTools/CGenTools.h"
 #include "../../MedinTuxTools/CGestIni.h"
 #include "../../MedinTuxTools/CMedicaBase.h"
 #include "../../MedinTuxTools/CMaskedLineEdit.h"
 #include "CApp.h"



#define H_CODE             6
#define LV_PROD_CODE_PROD  1
#define CIM10_DATA_BASE_NAME  "DataSrceODBC-MySQL-CIM10"

#define CISP_FILTER_ALL         0
#define CISP_FILTER_SYMPTOMES   1
#define CISP_FILTER_INFECTION   2
#define CISP_FILTER_NEOPLASIES  3
#define CISP_FILTER_TRAUMATISME 4
#define CISP_FILTER_CONGENITAL  5
#define CISP_FILTER_AUTRE_DIAG  6


// appelee par void Atcd_Code::addATCD_CIM10()

//-----------------------------------------------------  C_Dlg_GestionATCD -------------------------------------------
C_Dlg_GestionATCD::C_Dlg_GestionATCD(int tab, QString mode, QWidget* parent , const char* name , bool modal , WFlags fl )
        : Dlg_GestionATCD ( parent, name, modal, fl )
{   m_CloseAfterQuery   = 0;
    m_IsModified        = 0;
    m_IsModifiable      = 1;
    m_Family            = "";
    m_DataBase          = 0;
    m_SaveInitialState  = "";
    m_isCispInitialised = FALSE;
    m_TAB_LIBELLE       = 0;
    m_TAB_RUBRIQUE      = 1;
    m_TAB_ALLERGIE      = 2;
    m_TAB_THESAURUS     = 3;
    m_TAB_CISP          = 4;
    m_pLastIntolItem    = 0;
    m_Cisp_ClassCodes   = "SINTCD";
    if (G_pCApp->m_pCMedicaBase==0) G_pCApp->m_pCMedicaBase = new CMedicaBase;
    if (G_pCApp->m_pCMedicaBase==0)     return;
    setWFlags(Qt::WType_Dialog|Qt::WStyle_NormalBorder|Qt::WStyle_Title|Qt::WStyle_SysMenu|Qt::WStyle_Minimize|Qt::WStyle_Maximize|Qt::WStyle_MinMax );
    //................... un petit bidouillage accelerateur ...................................................
    C_KeyPressEater *keyPressEater = new C_KeyPressEater( listViewThesaurus, this ,"myMachinEvent" );
    connect( keyPressEater,  SIGNAL( Sign_F2_Pressed() ), this, SLOT( Slot_lineEditThesaurusFind1_F2_Pressed() ) );
    connect( keyPressEater,  SIGNAL( Sign_F6_Pressed() ), this, SLOT( Slot_lineEditThesaurusFind1_F6_Pressed() ) );
    lineEditThesaurusFind1->installEventFilter( keyPressEater );
    listViewThesaurus->installEventFilter( keyPressEater );
    this->installEventFilter( keyPressEater );


    Slot_lineEditNomAllergie_textChanged("a");
    Slot_lineEditNomMedicament_textChanged("A");

    setIcon( Theme::getIcon("Cim10All_Icon.png"));
    pushButtonAddMedicament->hide();
    listViewCim10_Choix->setSelectionMode ( QListView::Extended );
    m_pC_ListViewATCDManager  = new C_ListViewATCDManager(this, "gestListViewGestATCD", listViewCim10_Choix, G_pCApp->m_pAtcd_Code, &m_IsModified, &m_IsModifiable, &m_TypeATCD_Selectionne);
    if ( ! (G_pCApp->m_pCMedicaBase->Medica_GetBaseMode() & CMedicaBase::Datasemp) )
       {listViewAllergies->hide();
       }
    if (mode != "CIM10" && mode != "Cisp")
       { connect( listViewCim10_Choix, SIGNAL( contextMenuRequested(QListViewItem*,const QPoint&,int) ), m_pC_ListViewATCDManager,  SLOT( Slot_listView_ATCD_contextMenuRequested(QListViewItem*,const QPoint&,int) ) );
         initListChoixATCD();
       }
    else if (mode == "CIM10")
       { tabWidgetDicoATCD->removePage (tabWidgetDicoATCD->page ( m_TAB_ALLERGIE ) );
         m_TAB_THESAURUS   =  2;
         m_TAB_ALLERGIE    = -1;
         m_TAB_CISP        =  3;
         pushButtonAddFreeToChoix->hide();
         pushButtonThesaurusFreeAdd->hide();
         listViewCim10_Choix->clear();
         textLabelChoix->setText(tr("Liste des codes CIM10 sélectionnés"));
         pushButton_AddToChoix->hide();
       }
    else if (mode == "Cisp")
       { tabWidgetDicoATCD->removePage (tabWidgetDicoATCD->page ( 0 ) );
         tabWidgetDicoATCD->removePage (tabWidgetDicoATCD->page ( 0 ) );
         tabWidgetDicoATCD->removePage (tabWidgetDicoATCD->page ( 0 ) );   // on vire les deux tab du cim 10 et celui des allergies
         tabWidgetDicoATCD->removePage (tabWidgetDicoATCD->page ( 0 ) );   // on vire les deux tab du cim 10 celui des allergies celui du thesaurus
         m_TAB_THESAURUS   = -1;
         m_TAB_LIBELLE     = -1;
         m_TAB_RUBRIQUE    = -1;
         m_TAB_ALLERGIE    = -1;
         m_TAB_ALLERGIE    = -1;
         m_TAB_CISP        =  0;
         pushButtonAddFreeToChoix->hide();
         pushButtonThesaurusFreeAdd->hide();
         pushButtonThesaurusAdd->hide();
         pushButtonThesaurusFreeAdd->hide();
         pushButtonThesaurusExport->hide();
         pushButtonThesaurusImport->hide();
         listViewCim10_Choix->clear();
         textLabelChoix->setText(tr("Liste des codes Cisp sélectionnés"));
         pushButton_AddToChoix->hide();
       }
    tabWidgetDicoATCD->setCurrentPage ( m_TAB_THESAURUS );

    textLabel_Cisp_contient->hide();
    lineEditAutolcator_Cisp1->hide();
    lineEditAutolcator_Cisp2->hide();
    textLabel_Cisp_et_contient->hide();

    // signals and slots connections
    m_CispMiniPixmap  = Theme::getIcon( "16x16/CispItemTab.png");
    m_CimxMiniPixmap  = Theme::getIcon( "16x16/Cim10ItemTab.png");
    m_CimxCodefinal   = Theme::getIcon( "16x16/codefinal.png");
    m_CimxChapitre    = Theme::getIcon( "16x16/chapitre.png");
    pushButtonThesaurusSave->setPixmap(Theme::getIcon( "16x16/save.png"));
    pushButtonThesaurusDel->setPixmap(Theme::getIcon( "16x16/DelDoc.png"));
    pushButtonThesaurusAdd->setPixmap(Theme::getIcon( "16x16/AddToThesaurus.png"));
    pushButtonThesaurusFreeAdd->setPixmap(Theme::getIcon( "16x16/AddFreeTextToThesaurus.png"));
    pushButtonAddFreeToChoix->setPixmap(Theme::getIcon( "16x16/AddFreeTextToAllergie.png"));
    pushButtonAddMedicament->setPixmap(Theme::getIcon( "16x16/AddMedicament.png"));
    pushButtonFamilleGenreEdit->setPixmap(Theme::getIcon( "16x16/configure.png"));
    pushButtonThesaurus_Edit->setPixmap(Theme::getIcon( "16x16/ThesaurusItemEdit.png"));
    pushButton_AddToChoix->setPixmap(Theme::getIcon( "22x22/nextSlow.png"));
    pushButtonRemoveFromChoix->setPixmap(Theme::getIcon( "22x22/removeAtcd.png"));
    pushButton_AddToChoixFast->setPixmap(Theme::getIcon( "22x22/nextFast.png"));
    pushButtonThesaurusExport->setPixmap(Theme::getIcon( "22x22/saveFavoris.png"));
    pushButtonThesaurusImport->setPixmap(Theme::getIcon( "22x22/loadFavoris.png"));
    tabWidgetDicoATCD->setTabIconSet ( tabWidgetDicoATCD->page ( m_TAB_THESAURUS ),  QIconSet (Theme::getIcon( "16x16/ThesaurusTab.png")) );
    tabWidgetDicoATCD->setTabIconSet ( tabWidgetDicoATCD->page ( m_TAB_ALLERGIE ),   QIconSet (Theme::getIcon( "16x16/AllergieTab.png")) );
    tabWidgetDicoATCD->setTabIconSet ( tabWidgetDicoATCD->page ( m_TAB_LIBELLE ),    QIconSet (m_CimxMiniPixmap) );
    tabWidgetDicoATCD->setTabIconSet ( tabWidgetDicoATCD->page ( m_TAB_RUBRIQUE ),   QIconSet (m_CimxMiniPixmap) );
    tabWidgetDicoATCD->setTabIconSet ( tabWidgetDicoATCD->page ( m_TAB_CISP ),       QIconSet (m_CispMiniPixmap)  );
    connect( pushButtonThesaurusExport,   SIGNAL( clicked() ), this, SLOT( Slot_ThesaurusExport() ) );
    connect( pushButtonThesaurusImport,   SIGNAL( clicked() ), this, SLOT( Slot_ThesaurusImport() ) );
    connect( pushButtonThesaurusSave,     SIGNAL( clicked() ), this, SLOT( Slot_SaveThesaurus() ) );
    connect( pushButtonThesaurusDel,      SIGNAL( clicked() ), this, SLOT( Slot_ThesaurusDel() ) );
    connect( buttonOk,                    SIGNAL( clicked() ), this, SLOT( Slot_accept() ) );
    connect( buttonCancel,                SIGNAL( clicked() ), this, SLOT( Slot_reject() ) );
    connect( pushButton_AddToChoix,       SIGNAL( clicked() ), this, SLOT( Slot_pushButton_AddToChoix_clicked() ) );
    connect( pushButton_AddToChoixFast,   SIGNAL( clicked() ), this, SLOT( Slot_pushButton_AddToChoixFast_clicked() ) );
    connect( pushButtonRemoveFromChoix,   SIGNAL( clicked() ), this, SLOT( Slot_pushButtonRemoveFromChoix_clicked() ) );
    connect( pushButtonThesaurus_Edit,    SIGNAL( clicked() ), this, SLOT( Slot_pushButtonThesaurus_Edit_clicked() ) );
    connect( pushButtonThesaurusAdd,      SIGNAL( clicked() ), this, SLOT( Slot_pushButtonThesaurusAdd_clicked() ) );
    connect( pushButtonThesaurusFreeAdd,  SIGNAL( clicked() ), this, SLOT( Slot_pushButtonThesaurusFreeAdd_clicked() ) );
    connect( pushButtonAddFreeToChoix,    SIGNAL( clicked() ), this, SLOT( Slot_pushButtonAddFreeToChoix_clicked() ) );
    connect( pushButtonAddMedicament,     SIGNAL( clicked() ), this, SLOT( Slot_pushButtonAddMedicament_clicked() ) );
    connect( pushButtonFamilleGenreEdit,  SIGNAL( clicked() ), this, SLOT( Slot_pushButtonFamilleGenreEdit() ) );
    connect( tabWidgetDicoATCD,           SIGNAL( currentChanged(QWidget*) ),      this, SLOT( Slot_tabWidgetDicoATCD_currentChanged(QWidget*) ) );
    connect( listViewCim10_rubriques,     SIGNAL( clicked(QListViewItem*) ),       this, SLOT( Slot_listViewCim10_rubriques_clicked(QListViewItem*) ) );
    connect( listViewCim10_rubriques,     SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listViewCim10_rubriques_doubleClicked(QListViewItem*) ) );
    //connect( listViewCim10_rubriques,     SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int  )), this, SLOT( Slot_listViewCim10_rubriques_contextMenuRequested( QListViewItem *, const QPoint &, int  ) ) );
    connect( lineEditAutolcatorLibelle_2, SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditAutolcatorLibelle_2_textChanged(const QString&) ) );
    connect( lineEditAutolcatorLibelle,   SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditAutolcatorLibelle_textChanged(const QString&) ) );
    connect( lineEditThesaurusFind1,      SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditThesaurusFind1_textChanged(const QString&) ) );
    connect( checkBox_filter_chapitres_cimx,  SIGNAL( stateChanged ( int  ) ),     this, SLOT( Slot_checkBox_filter_chapitres_cimxChanged(int) ) );

    connect( lineEditAutolcator_Cisp1,   SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditAutolcator_Cisp1_textChanged(const QString&) ) );
    connect( lineEditAutolcator_Cisp2,   SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditAutolcator_Cisp2_textChanged(const QString&) ) );
    connect( listView_Cisp,              SIGNAL( clicked(QListViewItem*) ),       this, SLOT( Slot_listView_Cisp_clicked(QListViewItem*) ) );
    connect( listView_Cisp,              SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listView_Cisp_doubleClicked(QListViewItem*) ) );
    connect( comboBox_Cisp_filter_Chapi, SIGNAL( highlighted(const QString &) ),  this, SLOT( Slot_comboBox_Cisp_filter_Chapi_highlighted(const QString &) ) );
    connect( comboBox_Cisp_filter_Class, SIGNAL( highlighted(int) ),              this, SLOT( Slot_comboBox_Cisp_filter_Class_highlighted(int) ) );
    connect( checkBox_Cisp_filter_cimx,  SIGNAL( stateChanged ( int  ) ),         this, SLOT( Slot_checkBox_Cisp_filter_cimx_stateChanged(int) ) );

    connect( lineEditThesaurusFind1,      SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditThesaurusFind1_textChanged(const QString&) ) );


    //connect( listViewCim10_Libelles,      SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int  )), this, SLOT( Slot_listViewCim10_Libelles_contextMenuRequested( QListViewItem *, const QPoint &, int  ) ) );
    connect( listViewCim10_Libelles,      SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listViewCim10_Libelles_doubleClicked(QListViewItem*) ) );
    connect( listView_Produits,           SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listView_Produits_doubleClicked( QListViewItem * ) ) );
    connect( listViewAllergies,           SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listViewAllergies_doubleClicked( QListViewItem * ) ) );
    connect( listViewCim10_Choix,         SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listViewCim10_Choix_doubleClicked( QListViewItem * ) ) );

    connect( listViewThesaurus,           SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listViewThesaurus_doubleClicked(QListViewItem*) ) );
    connect( listViewThesaurus,           SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int  )), this, SLOT( Slot_listViewThesaurus_contextMenuRequested( QListViewItem *, const QPoint &, int  ) ) );
    connect( listViewThesaurus,           SIGNAL( itemRenamed ( QListViewItem * , int , const QString &  )),      this, SLOT( Slot_listViewThesaurus_itemRenamed ( QListViewItem * , int , const QString &  ) ) );
    connect( lineEditNomAllergie,         SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditNomAllergie_textChanged(const QString&) ) );
    connect( lineEditNomAllergie,         SIGNAL( textChanged(const QString&) ),   this, SLOT( Slot_lineEditNomMedicament_textChanged(const QString&) ) );
    //connect( listViewAllergies,           SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int  )), this, SLOT( Slot_listViewAllergies_contextMenuRequested( QListViewItem *, const QPoint &, int  ) ) );
    connect( listViewAllergies,           SIGNAL( clicked(QListViewItem*) ),       this, SLOT( Slot_listViewAllergies_clicked(QListViewItem*) ) );
    connect( listView_Produits,           SIGNAL( clicked(QListViewItem*) ),       this, SLOT( Slot_listView_Produits_clicked(QListViewItem*) ) );
    connect( listView_ATC,                SIGNAL( clicked(QListViewItem*) ),       this, SLOT( Slot_listView_ATC_clicked(QListViewItem*) ) );
    connect( listView_ATC,                SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( Slot_listView_ATC_doubleClicked(QListViewItem*) ) );
    //......................... Liste ATC .............................................................................................
    if (listView_ATC->childCount() <10 ) G_pCApp->m_pCMedicaBase->Medica_FillQListView_ATC(listView_ATC);

    QValueList <int> list;
    int w = (width()/10)*5;
    list.append(w);
    list.append(width()-w);
    splitter5->setSizes(list);
    if (tab != -1) setOnglet(tab);
    readAndSetWindowPos();
}

//-----------------------------------------------------  ~C_Dlg_GestionATCD -------------------------------------------
C_Dlg_GestionATCD::~C_Dlg_GestionATCD()
{if (m_DataBase)
    {if (m_DataBase->isOpen()) m_DataBase->close();
    }
 writeWindowPos();
}

//-------------------------------------- initThesaurus -------------------------------------------------------
void C_Dlg_GestionATCD::initThesaurus()
{   initComboFamilleGenre();
    if (m_DataBase)      initThesaurusListe();
}
//-------------------------------------- initComboFamilleGenre -------------------------------------------------------
void C_Dlg_GestionATCD::initComboFamilleGenre()
{   CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", m_structStr);
    connect( m_Combo_Family, SIGNAL( highlighted(const QString & ) ), this,    SLOT  ( Slot_comboBox_Family_highlighted(const QString &) ) );
    connect( m_Combo_Family, SIGNAL( activated (const QString&) ),    this,    SLOT  ( Slot_combo_Family_activated(const QString&) ) );

    connect( m_Combo_Genre, SIGNAL( highlighted(const QString & ) ), this,    SLOT  ( Slot_comboBox_Genre_highlighted( const QString&) ) );
    connect( m_Combo_Genre, SIGNAL( activated(const QString & ) ),   this,    SLOT  ( Slot_combo_Genre_activated( const QString&) ) );

    m_ATCD_FamilyList.clear();
    QString menuStr        = m_structStr;
    QString section        = "";
    int posDebSection      = -1;
    int posEndSection      = 0;
    while ( (posDebSection = menuStr.find("[",posEndSection)) != -1 )
       {++posDebSection;
        if ( (posEndSection = menuStr.find("]",posDebSection)) != -1 )
           {section = menuStr.mid(posDebSection,posEndSection-posDebSection).stripWhiteSpace();
            if (section.length())
               {m_ATCD_FamilyList<<section;
               }
           }
       }
    m_ATCD_FamilyList.prepend(tr("Toutes les catégories"));
    m_Combo_Family->clear();
    m_Combo_Family->setCurrentItem(0);
    m_Combo_Family->insertStringList(  m_ATCD_FamilyList );
    Slot_comboBox_Family_highlighted(  m_Combo_Family->text ( 0 )  );
}
//-----------------------------------------------------  readLine -------------------------------------------
unsigned long  C_Dlg_GestionATCD::readLine(QFile *pQFile, char *buffer, QString &outParam, unsigned long nbMax)
{
         unsigned long nb                              =  pQFile->readLine(buffer, nbMax);
         buffer[nb]                                    =  0;
         if (CGestIni::IsUtf8(buffer))     outParam    =  QString::fromUtf8 ( buffer ) ;
         else                              outParam    =  buffer;
         return nb;
}
//-------------------------------------- initTableCisp -------------------------------------------------------
int C_Dlg_GestionATCD::initTableCisp()
{  QString        requete = ""; // a modifier pour QT4
   bool               ok  = false;
   //................. ouvrir la base .....................................
   if (OpenBase()==0)          return 0;

   //........... ya t-il la table Cisp .................
   QSqlDriver *pQSqlDriver =  m_DataBase->driver();
   if ( pQSqlDriver == 0)      return 0;
   //..................... verifier si les tables deja en place correspondent avec celles ..................
   //                      indiquees par le fichier de configuration
   QStringList tablesList   = pQSqlDriver->tables("1");  tablesList.sort();
   QStringList::Iterator it = tablesList.begin();
   while ( it != tablesList.end() )
         {if (*it=="cisp_dico")
             {ok=true;
              break;
             }
          ++it;
         }
   QSqlQuery query(QString::null , m_DataBase );
   if (!ok)
      {
       //........... ya pas la table CispCim10 on la cree .................
       QString prepare  = "";
       QString line     = "";
       QString fname    = "";
       QString codeCisp = "";
       QString codeCimx = "";
       long      nbMax  = 32000;              //
       char     *buffer = 0;
       int         pos  = 0;
       int         len  = 0;
       QChar         c  = ' ';
       fname            = G_pCApp->m_PathAppli + "Ressources/Cisp-Cimx.txt";
       if ( !QFile::exists( fname ) )            return 0;
       if ( !QFile::exists( G_pCApp->m_PathAppli + "Ressources/Cisp-02.txt" )) return 0;
       //.................on efface la table .....................
       requete = "DROP TABLE IF EXISTS `cisp_cimx`";
       if (!query.exec(requete))                 return 0;
       //.................on cree la table .....................
       requete =    " CREATE TABLE `cisp_cimx` ("
                    "`cisp_cimx_pk`            BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`cisp_cimx_owner`         VARCHAR(40)  ,"
                    "`cisp_cimx_cisp_code`     VARCHAR(8)   ,"
                    "`cisp_cimx_cimx_code`     VARCHAR(8)   ,"
                    "`cisp_cimx_libelle`       VARCHAR(1024) ,"
                    "`cisp_cimx_date`          DATETIME     ,"
                    "PRIMARY KEY (`cisp_cimx_pk`)"
                    ");";
       if (!query.exec(requete))                 return 0;
       //........... on l'initialise .................
       //            on parse el fichier texte ligne a ligne
       QFile file( fname );
       if ( !file.open( IO_ReadOnly  ) )         return 0;
       buffer = new char[nbMax+5];    // +5 pour permettre analyse utf8 qui explore trois apres
       if (buffer ==0)                           return 0;
       //.......... on lit le fichier ligne a ligne ........................
       prepare  = " INSERT INTO `cisp_cimx` (`cisp_cimx_owner`, `cisp_cimx_cisp_code`,`cisp_cimx_cimx_code`,`cisp_cimx_libelle`,`cisp_cimx_date`) "
                  " VALUES (?,?,?,?,?)";
       while (!file.atEnd())
        { readLine(&file, buffer, line, nbMax);
          //.................creation de la table .....................
          line     = line.remove('\"').upper().remove('\n').remove('\r');
          pos      = line.find(",");
          codeCimx = line.left(pos);
          query.prepare(prepare);
          query.bindValue(0, "MT");
          query.bindValue(1, line.mid(pos+1));         // cisp
          query.bindValue(2, codeCimx);                // cimx
          query.bindValue(3, getCimxLibelleFromCimxCode( codeCimx ).left(1023));
          query.bindValue(4, QDateTime::currentDateTime());
          if ( !query.exec())  {file.close();delete []buffer; return 0;}
        }
       file.close();         // on a plus besoin du fourbi
       delete []buffer;
       requete = "DROP TABLE IF EXISTS `cisp_dico`";
       if (!query.exec(requete))                 return 0;
       //.................on cree la table .....................
       requete =    " CREATE TABLE `cisp_dico` ("
                    "`cisp_dico_pk`            BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`cisp_dico_owner`         VARCHAR(40)  ,"
                    "`cisp_dico_libelle`       VARCHAR(400) ,"
                    "`cisp_dico_code`          VARCHAR(10)  ,"
                    "`cisp_dico_class`         VARCHAR(10)  ,"
                    "`cisp_dico_cimx_list`     VARCHAR(500)  ,"
                    "`cisp_dico_date`          DATETIME     ,"
                    "PRIMARY KEY (`cisp_dico_pk`)"
                    ");";
       if (!query.exec(requete))                 return 0;
       //........... on l'initialise .................
       //            on parse el fichier texte ligne a ligne
       fname            = G_pCApp->m_PathAppli + "Ressources/Cisp-02.txt";
       file.setName ( fname );
       if ( !file.open( IO_ReadOnly  ) )         return 0;
       buffer = new char[nbMax+5];    // +5 pour permettre analyse utf8 qui explore trois apres
       if (buffer ==0)                           return 0;
       //.......... on lit le fichier ligne a ligne ........................
       prepare  = " INSERT INTO `cisp_dico` (`cisp_dico_owner`, `cisp_dico_libelle`,`cisp_dico_code`,`cisp_dico_class`,`cisp_dico_cimx_list`,`cisp_dico_date`) "
                  " VALUES (?,?,?,?,?,?)";
       while (!file.atEnd())
        { readLine(&file, buffer, line, nbMax);
          //.................creation de la table .....................
          pos      = -1;
          len      = line.length();
          while (pos<len && line[++pos] == ' ' );           //  avancer jusqu'au premier caractere qui n'est pas un espace
          line     = line.mid(pos).remove('\n').remove('\r');
          codeCisp = line.left(4).upper();
          query.prepare(prepare);
          query.bindValue(0, "MT");
          query.bindValue(1, line.mid(4));                    // Y02 douleur des testicules, du scrotum
          query.bindValue(2, codeCisp);
          query.bindValue(3, QString::number(pos/8));         // le nombre de tabulations definit la classe.
          query.bindValue(4, getCispToCim10Str(codeCisp));      //getCispToCim10Str(codeCisp));
          query.bindValue(5, QDateTime::currentDateTime());
          if ( !query.exec())  {file.close();delete []buffer; return 0;}
        }
       file.close();         // on a plus besoin du fourbi
       delete []buffer;
      } // endif (!ok)
   //........................ mapper les listes fixes .....................................
    m_CispClassesList.clear();     // "SINTCD"
    m_CispClassesList<<tr("SYMPTÔMES ET PLAINTES")        // 1  S
                     <<tr("INFECTIONS")                   // 2  I
                     <<tr("NÉOPLASIES")                   // 3  N
                     <<tr("TRAUMATISMES")                 // 4  T
                     <<tr("ANOMALIES CONGÉNITALES")       // 5  C
                     <<tr("AUTRES DIAGNOSTICS");          // 6  D
   comboBox_Cisp_filter_Class->insertStringList (m_CispClassesList);
   comboBox_Cisp_filter_Class->insertStringList (tr("  Tous les items"),0);  // on rajoute deux espaces pour aspect idem a celui des chapitres

   // "-FDABHKLNPRSTUWXYZ"
   m_CispChapitresMap.clear();
   m_CispChapitresMap.insert( 'F',  tr("oeil"));
   m_CispChapitresMap.insert( 'D',  tr("système digestif"));
   m_CispChapitresMap.insert( 'A',  tr("général et non spécifié"));
   m_CispChapitresMap.insert( '-',  tr("procédures"));
   m_CispChapitresMap.insert( 'B',  tr("sang système hématopoïétique ou immunologique"));
   m_CispChapitresMap.insert( 'H',  tr("oreille"));
   m_CispChapitresMap.insert( 'K',  tr("cardio-vasculaire"));
   m_CispChapitresMap.insert( 'L',  tr("ostéo-articulaire"));
   m_CispChapitresMap.insert( 'N',  tr("neurologique"));
   m_CispChapitresMap.insert( 'P',  tr("psychologique"));
   m_CispChapitresMap.insert( 'R',  tr("respiratoire"));
   m_CispChapitresMap.insert( 'S',  tr("peau"));
   m_CispChapitresMap.insert( 'T',  tr("métabolisme nutrition endocrinien"));
   m_CispChapitresMap.insert( 'U',  tr("systèmeme urinaire"));
   m_CispChapitresMap.insert( 'W',  tr("grossesse, accouchement et planning familial"));
   m_CispChapitresMap.insert( 'X',  tr("système génital féminin et sein"));
   m_CispChapitresMap.insert( 'Y',  tr("système génital masculin et sein"));
   m_CispChapitresMap.insert( 'Z',  tr("social"));
   m_isCispInitialised = TRUE;

   QMap<QChar, QString>::Iterator im;
   for ( im = m_CispChapitresMap.begin(); im != m_CispChapitresMap.end(); ++im )
       {comboBox_Cisp_filter_Chapi->insertItem(QString(im.key())+" "+im.data());
       }
   comboBox_Cisp_filter_Chapi->insertStringList (tr("  Tous les items"),0);
   listView_Cisp_filter();
 return 1;
}
//-------------------------------------- getCimxLibelleFromCimxCode -------------------------------------------------------
QString  C_Dlg_GestionATCD::getCispChapitreFromCodeCisp(QString cispCode)
{cispCode     =     cispCode.remove('(').remove(')').remove('-');
 int posEgal  =     cispCode.find("=");
 if (posEgal != -1) cispCode = cispCode.left(posEgal); // traiter le cas du code compose
 QChar   q_chapi_code   = cispCode.at(0);                              // la premiere lettre du cispCode est son chapitre expl X70 A99
 return m_CispChapitresMap[q_chapi_code];
}

//-------------------------------------- getCimxLibelleFromCimxCode -------------------------------------------------------
QString  C_Dlg_GestionATCD::getCimxLibelleFromCimxCode(const QString &codeCimx, int stopToFirstResponse /* = 0 */)
{  QString libelle = "";
   QString   toAdd = "";
   QString requete = QString(" SELECT libelle FROM `master`"
                             " JOIN `libelle` on master.SID=libelle.SID"
                             " WHERE master.code='%1'").arg(codeCimx);
   QSqlQuery query(QString::null , m_DataBase );
   if ( !query.exec(requete))     return QString::null;
   if ( !query.isActive())        return QString::null;
   while (query.next())
         {toAdd    = CGestIni::Utf8_Query(query, 0).stripWhiteSpace();
          if (toAdd.length()) libelle += CGestIni::Utf8_Query(query, 0) + " | ";
          if (stopToFirstResponse) break;
         }
   return libelle.left(libelle.length()-3);      // virer le dernier " | "
}
//-------------------------------------- getCispLibelleFromCispCode -------------------------------------------------------
QString  C_Dlg_GestionATCD::getCispLibelleFromCispCode(const QString &codeCispIn)
{  QString codeCisp = codeCispIn; codeCisp = codeCisp.remove('(').remove(')').remove('-');
   int     posEgal  = codeCisp.find("="); if (posEgal != -1) codeCisp = codeCisp.left(posEgal); // traiter le cas du code compose
   QString requete  = QString(" SELECT cisp_dico_libelle FROM `cisp_dico`"
                              " WHERE  cisp_dico_code='%1'").arg(codeCisp);
   QSqlQuery query(QString::null , m_DataBase );
   if ( !query.exec(requete))     return QString::null;
   if ( !query.isActive())        return QString::null;
   if ( !query.next())            return QString::null;
   return  CGestIni::Utf8_Query(query, 0).stripWhiteSpace().remove('\r').remove('\n');
}

//-------------------------------------- Slot_comboBox_Cisp_filter_Chapi_highlighted -------------------------------------------------------
QString  C_Dlg_GestionATCD::getCispToCim10Str(const QString &codeCisp)
{  QString listCode = "";
   /*
   QString requete  = QString(" SELECT master.code,cisp_cimx_cisp_code, FR_OMS FROM `master`"
                              " join `libelle` on master.SID=libelle.SID"
                              " join `cisp_cimx` on cisp_cimx_cimx_code=master.code"
                              " where cisp_cimx_cisp_code ='%1'").arg(codeCisp);
   */
   QString requete  = QString(" SELECT cisp_cimx_cimx_code FROM cisp_cimx WHERE `cisp_cimx_cisp_code`='%1'").arg(codeCisp);
   QSqlQuery query(QString::null , m_DataBase );
   if ( !query.exec(requete))     return QString::null;
   if ( !query.isActive())        return QString::null;
   while (query.next())
         {listCode += query.value(0).toString() + "|";
         }
  return listCode.left(listCode.length()-1);    // virer le dernier '|'
}

//-------------------------------------- Slot_comboBox_Cisp_filter_Chapi_highlighted -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_comboBox_Cisp_filter_Chapi_highlighted(const QString &chapiText)
{setCispFilterFromCombosStates( chapiText, comboBox_Cisp_filter_Class->currentItem() );
}
//-------------------------------------- Slot_comboBox_Cisp_filter_Class_highlighted -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_comboBox_Cisp_filter_Class_highlighted(int classIndex)
{setCispFilterFromCombosStates( comboBox_Cisp_filter_Chapi->currentText() , classIndex );
}
//-------------------------------------- Slot_comboBox_Cisp_filter_Class_highlighted -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_checkBox_Cisp_filter_cimx_stateChanged(int )
{setCispFilterFromCombosStates( comboBox_Cisp_filter_Chapi->currentText(), comboBox_Cisp_filter_Class->currentItem() );
}
//-------------------------------------- setCispFifterFromCombosStates -------------------------------------------------------
void C_Dlg_GestionATCD::setCispFilterFromCombosStates( const QString &chapiText, int classIndex )
{bool allChapi = chapiText.left(1)==" ";
 QString                       classFilter =  "SINTCD";                               // on veut a priori tout index 0 est "Tous les items"
 if (classIndex)               classFilter =  m_Cisp_ClassCodes.mid(classIndex-1,1);  // sauf si le contraire est spécifie
 QString                       chapiFilter = "-FDABHKLNPRSTUWXYZ";                    // on veut a priori tout index 0 est "  Tous les items"
 if ( ! allChapi)              chapiFilter = chapiText.left(1);                       // sauf si le contraire est spécifie
 listView_Cisp_filter( chapiFilter , classFilter );
}

//-------------------------------------- listView_Cisp_filter -------------------------------------------------------
int C_Dlg_GestionATCD::listView_Cisp_filter(const QString &chapiFilter     /* ="-FDABHKLNPRSTUWXYZ" */ ,
                                            const QString &classFilter     /* = "SINTCD"            */ ,
                                            QListView *pQTreeWidget        /* = 0                   */ )
{  if ( ! m_isCispInitialised )   return 0;
   if ( pQTreeWidget==0 )  pQTreeWidget =  listView_Cisp;
   QString       codeCisp = "";
   QString        classe  = "";
   QString       libelle  = "";

   //....................... ici la table `cisp_dico` devrait etre ok ......................
   // on la lit en entier pour initialiser la listViewCim10_Libelles
   QSqlQuery query(QString::null , m_DataBase );
   QString requete  = "SELECT  `cisp_dico_libelle`,`cisp_dico_code`,`cisp_dico_class`,`cisp_dico_owner` FROM `cisp_dico` ORDER BY `cisp_dico_code`";
   if ( !query.exec(requete))     return 0;
   if ( !query.isActive())        return 0;
   QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
   pQTreeWidget->clear();
   while (query.next())
         { codeCisp                  = query.value(1).toString();
           classe                    = query.value(2).toString();
           libelle                   = CGestIni::Utf8_Query(query, 0);
           QListViewItem *parentItem = getCispParentItemFromCodeAndClasse(pQTreeWidget, codeCisp, classe.toInt(), chapiFilter, classFilter);
           if (parentItem)
              { QListViewItem *item =   new QListViewItem (parentItem);     // a modifier pour QT4
                if (item)
                   {item->setText(0, libelle);
                    item->setText(1, codeCisp);
                    item->setPixmap ( 0, m_CispMiniPixmap);
                    if (checkBox_Cisp_filter_cimx->isChecked())
                       {fillCispItemWithAssociatedCimx(item, libelle, codeCisp);
                       }
                   }
              }
         }
  //................ si seulement un chapitre on ouvre tout ..............
  //if (chapiFilter.length()==1)
     {QListViewItemIterator it( listView_Cisp );
      while ( it.current() )
            { (it.current()->setOpen (true ));
            ++it;
            }
     }
  QApplication::restoreOverrideCursor();
  return 1;
  //comboBox_Cisp_filter
}
//-------------------------------------- fillCispItemWithAssociatedCimx -------------------------------------------------------
int C_Dlg_GestionATCD::fillCispItemWithAssociatedCimx( QListViewItem *pQTreeWidgetItem, const QString &/*libelleCisp*/, const QString &codeCisp)
{  if ( ! pQTreeWidgetItem )     return 0;
   QString       codeCimx = "";
   QString       libelle  = "";

   QSqlQuery query(QString::null , m_DataBase );

   QString requete  = QString(" SELECT FR_OMS, master.code FROM `master`"
                              " join `libelle` on master.SID=libelle.SID"
                              " join `cisp_cimx` on cisp_cimx_cimx_code=master.code"
                              " where cisp_cimx_cisp_code ='%1'").arg(codeCisp);
   /*
   QString requete  = QString(" SELECT cisp_cimx_libelle, cisp_cimx_cimx_code FROM `cisp_cimx`"
                              " WHERE  cisp_cimx_cisp_code ='%1'").arg(codeCisp);
   */
   if ( !query.exec(requete))     return 0;
   if ( !query.isActive())        return 0;

   while (query.next())
         { libelle                   = CGestIni::Utf8_Query(query, 0);
           if (libelle.length()==0) continue;
           codeCimx                  = query.value(1).toString();
           QListViewItem *item =   new QListViewItem (pQTreeWidgetItem);     // a modifier pour QT4
           if (item)
              {item->setText(0, libelle);
               item->setText(1, codeCisp    + "=" +codeCimx);
               item->setPixmap ( 0, m_CimxMiniPixmap);
              }
         }
  return 1;
  //comboBox_Cisp_filter
}

//-------------------------------------- getCispParentItemFromCodeAndClasse -------------------------------------------------------
QListViewItem *C_Dlg_GestionATCD::getCispParentItemFromCodeAndClasse(QListView     *pQTreeWidget,    // QListView ou doit se faire l'affichage des mentions cisp
                                                                     const QString &cispCode,        // code cisp X70 A99 de la mention
                                                                     int   i_class_code,             // classe de la mention (0-"SYMPTÔMES ET PLAINTES" 1-"INFECTION" ...)
                                                                     const QString &chapiFilter,     // chaine du filtre d'entree : un ou plusieurs caracteres de "-FDABHKLNPRSTUWXYZ"
                                                                     const QString &classFilter)     // chaine du filtre d'entree : un ou plusieurs caracteres de "SINTCD"
{

 //................. filtrer ................................................
 QChar   q_class_code   = m_Cisp_ClassCodes.at(i_class_code);          // "SINTCD" convertir le code de classe en lettre (pour tester si present dans le filtre d'entree)
 QChar   q_chapi_code   = cispCode.at(0);                              // la premiere lettre du cispCode est son chapitre expl X70 A99
 if (! classFilter.contains (q_class_code) )  return 0;                // si code de classe pas present et non autorise par le filtre d'entree des classes   cassos
 if (! chapiFilter.contains (q_chapi_code) )  return 0;                // si code de classe pas present et non autorise par le filtre d'entree des chapitres adios
 //.................. recuperer les elements ................................
 QString chapitre = m_CispChapitresMap[q_chapi_code].upper();              // recuperer le nom du chapitre
 QString classe   = m_CispClassesList[i_class_code];                       // recuperer le nom de la classe

 //.............. trouver l'item du chapitre ...............................
 //               ou le creer si n'existe pas
 QListViewItem * chapitreItem = 0;
 QListViewItem * classeItem   = 0;
 QListViewItem * myChild      =  pQTreeWidget->firstChild();
 while ( myChild )
       {if (myChild->text(0)==chapitre) chapitreItem = myChild;
        myChild = myChild->nextSibling();
       }
 if (chapitreItem==0)            // si pas trouve le creer
    {chapitreItem = new QListViewItem (pQTreeWidget);
     if (chapitreItem)
        {chapitreItem->setText(0,chapitre.upper());
         chapitreItem->setText(1,cispCode.left(1));
        }
    }
 if (chapitreItem==0)     return 0;
 if (cispCode.at(0)=="-") return chapitreItem;   // pas de classe pour les items procedures tout se fait sous lui
 //.............. trouver l'item de la classe .......................
 //               ou le creer si n'existe pas
 myChild = chapitreItem->firstChild();
 while ( myChild )
       {if (myChild->text(0)==classe) classeItem = myChild;
        myChild = myChild->nextSibling();
       }
 if (classeItem==0)            // si pas trouve le creer en fils de celui du chapitre
    {classeItem = new QListViewItem (chapitreItem);
     if (classeItem)
        {classeItem->setText(0,classe);
         classeItem->setText(1,QString::number(i_class_code));
        }
    }
 return classeItem;
}

//-------------------------------------- Slot_lineEditAutolcator_Cisp1_textChanged -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditAutolcator_Cisp1_textChanged(const QString&)
{
}
//-------------------------------------- Slot_lineEditAutolcator_Cisp2_textChanged -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditAutolcator_Cisp2_textChanged(const QString&)
{
}
//-------------------------------------- Slot_listView_Cisp_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_Cisp_clicked(QListViewItem *pQListViewItem)
{if (pQListViewItem==0)  return;
 if (pQListViewItem->depth ()==2 && pQListViewItem->childCount () ==0)
    { fillCispItemWithAssociatedCimx(pQListViewItem, pQListViewItem->text(0), pQListViewItem->text(1));
    }
 else
    {pQListViewItem->setOpen (pQListViewItem->isOpen()==FALSE);  // ouvre si ferme / ferme si ouvert
    }
}
//-------------------------------------- Slot_listView_Cisp_doubleClicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_Cisp_doubleClicked(QListViewItem *pQListViewItem)
{common_listView_doubleClicked(pQListViewItem);
}

//-------------------------------------- Slot_lineEditThesaurusFind1_F2_Pressed -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditThesaurusFind1_F2_Pressed()
{QListViewItem *pQListViewItem  =  listViewThesaurus->currentItem ();
 if (pQListViewItem==0 && listViewThesaurus->childCount()==1)
    {pQListViewItem = listViewThesaurus->firstChild();
    }
 if (pQListViewItem)
    {listViewThesaurus->setSelected ( pQListViewItem, TRUE );
    }
 Slot_pushButton_AddToChoixFast_clicked();
}
//-------------------------------------- Slot_lineEditThesaurusFind1_F6_Pressed -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditThesaurusFind1_F6_Pressed()
{QListViewItem *pQListViewItem  =  listViewThesaurus->currentItem ();
 if (pQListViewItem==0 && listViewThesaurus->childCount()==1)
    {pQListViewItem = listViewThesaurus->firstChild();
    }
if (pQListViewItem)
    {listViewThesaurus->setSelected ( pQListViewItem, TRUE );
    }
 Slot_pushButton_AddToChoix_clicked();
}

//-------------------------------------- Slot_pushButtonFamilleGenreEdit -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonFamilleGenreEdit()
{ C_Dlg_FamilleGenre *pC_Dlg_FamilleGenre = new C_Dlg_FamilleGenre(&m_structStr, this); if (pC_Dlg_FamilleGenre==0) return;
  pC_Dlg_FamilleGenre->exec();
  if (pC_Dlg_FamilleGenre->isModified())
     {initComboFamilleGenre();
     }
  delete pC_Dlg_FamilleGenre;
}
//-------------------------------------- Slot_pushButtonThesaurusAdd_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_ThesaurusImport()
{QString s = QFileDialog::getOpenFileName(
                    G_pCApp->m_PathAppli+"Ressources",
                    tr("Favoris (*.fav)"),
                    this,
                    tr("open file dialog"),
                    tr("Choose a file") );
 if ( s.length() )
    {if (QFile::exists(s))
        {QString data;
         CGestIni::Param_UpdateFromDisk(s, data );
         unSerialiseFavoris(data);
         filtrerListeThesaurus(listViewThesaurus);
        }
    }
}
//-------------------------------------- Slot_pushButtonThesaurusAdd_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_ThesaurusExport()
{QString s = QFileDialog::getSaveFileName(
                    G_pCApp->m_PathAppli+"Ressources",
                    tr("Favoris (*.fav)"),
                    this,
                    tr("save file dialog"),
                    tr("Choose a filename to save under") );
 if ( s.length() )
    {QFileInfo fi(s);
     if (fi.extension() != "fav")
        {s = fi.dirPath()+"/"+fi.baseName()+".fav";
        }
     CGestIni::Param_UpdateToDisk(s, serialiseFavoris() );
    }
}

//-------------------------------------- Slot_pushButtonThesaurusAdd_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonThesaurusAdd_clicked()
{    int                 tab_index = tabWidgetDicoATCD->currentPageIndex ();
     QListViewItem *pQListViewItem = 0;
     if (tab_index==m_TAB_LIBELLE)
        { if (pQListViewItem==0) pQListViewItem = listViewCim10_Libelles->currentItem ();
          if (pQListViewItem )   appendToThesaurus(pQListViewItem->text(0), SID_to_CIM10( pQListViewItem->text(2) ).prepend("~").append("~") );
        }
     else if (tab_index==m_TAB_RUBRIQUE)
        { if (pQListViewItem==0) pQListViewItem = listViewCim10_rubriques->currentItem ();
          if (pQListViewItem )   appendToThesaurus(pQListViewItem->text(0), SID_to_CIM10( pQListViewItem->text(2) ).prepend("~").append("~") );
        }
     else if (tab_index==m_TAB_CISP)
        { if (pQListViewItem==0) pQListViewItem = listView_Cisp->currentItem ();
          if (pQListViewItem==0)                  return;
          if (pQListViewItem->text(1).length()<3) return;   // on est sur un chapitre ou classe
          appendToThesaurus(pQListViewItem->text(0), pQListViewItem->text(1).prepend("-(").append(")-"));
        }
     else if (tab_index==m_TAB_ALLERGIE)
        { if (pQListViewItem==0) pQListViewItem = listViewAllergies->selectedItem ();
          if (pQListViewItem)   {appendToThesaurus(pQListViewItem->text(0), pQListViewItem->text(1).prepend("(").append(")")); return;}
          if (pQListViewItem==0) pQListViewItem = listView_Produits->selectedItem ();
          if (pQListViewItem)    appendToThesaurus(pQListViewItem->text(0), pQListViewItem->text(4).prepend("(-").append("-)"));
          if (pQListViewItem==0) pQListViewItem = listView_ATC->selectedItem ();
          if (pQListViewItem)    appendToThesaurus(pQListViewItem->text(0), pQListViewItem->text(1).prepend("(.").append(".)"));
        }
     else if (tab_index==m_TAB_THESAURUS)
        {   //if (pQListViewItem==0) pQListViewItem = listViewThesaurus->currentItem ();
          //if (pQListViewItem==0) return;
        }
}

//-------------------------------------- Slot_pushButtonThesaurusFreeAdd_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonThesaurusFreeAdd_clicked()
{appendToThesaurus(tr("Ajout d'un antécédent textuel aux favoris"), "");
}

//-------------------------------------- Slot_pushButtonAddFreeToChoix_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonAddFreeToChoix_clicked()
{CPrtQListViewItem* pCPrtQListViewItem  = new CPrtQListViewItem ( listViewCim10_Choix, 0,  "");
 if (pCPrtQListViewItem==0) return;
 if (m_pC_ListViewATCDManager->ATCD_EditListViewItem(pCPrtQListViewItem)==0){delete pCPrtQListViewItem; return;}
}
//-------------------------------------- Slot_pushButtonAddMedicament_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonAddMedicament_clicked()
{
}

//-----------------------------------------------------  Slot_listViewCim10_rubriques_contextMenuRequested -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewAllergies_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &/*pt*/, int /*c*/ )
{listView_Produits->clearSelection ();
 if (pQListViewItem==0)    return;
 QStringList optList;
 optList.append(tr ("Ajouter cet item aux favoris."));
 QString result = DoPopupList(optList, this);
 if (result.length() == 0) return;

 if (result== tr("Ajouter cet item aux favoris."))
    {appendToThesaurus(pQListViewItem->text(0), pQListViewItem->text(1).prepend("(").append(")"));
    }
}

//-------------------------------------- Slot_listViewAllergies_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewAllergies_clicked(QListViewItem *pQListViewItem)
{ listView_Produits->clearSelection ();
  m_pLastIntolItem = pQListViewItem;
 //listView_ATC->clearSelection ();
}
//-------------------------------------- Slot_listView_ATC_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_ATC_clicked(QListViewItem *pQListViewItem)
{ if (pQListViewItem==0) return;
  listViewAllergies->clearSelection ();
  listView_Produits->clearSelection ();
  QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
  int nb = G_pCApp->m_pCMedicaBase->Medica_GetMedicamentListByATC(  listView_Produits ,
                                                                    pQListViewItem->text(1), // 1 = code ATC de la listview ATC
                                                                    "",           //lineEdit_Produit->text(),
                                                                    QString(""),
                                                                    QString("0")
                                                             );
  m_pLastIntolItem = pQListViewItem;
  textLabelStatut->setText(QString::number(nb) + tr (" produits trouvés"));
  QApplication::restoreOverrideCursor();
}
//-------------------------------------- Slot_listView_Produits_clicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_Produits_clicked(QListViewItem *pQListViewItem)
{ if (pQListViewItem==0) return;
  listView_ATC->clearSelection ();
  long nb = G_pCApp->m_pCMedicaBase->Datasemp_GetSubtancesListByCodeProd(listViewAllergies, pQListViewItem->text( LV_PROD_CODE_PROD));
  if (nb>1)
     textLabelStatut->setText(QString::number(nb) + tr(" substances trouvées"));
  else if (nb==1)
     textLabelStatut->setText(tr("Une substance trouvée"));
  else
     textLabelStatut->setText(tr("Aucune substance trouvée"));
 if (nb)
    {QString atc = G_pCApp->m_pCMedicaBase->Medica_GetCodeATC(pQListViewItem->text( 4),            // cip
                                                              pQListViewItem->text( 3));           // pk_t4b
     /*QListViewItem *pQListViewATCItem = */ G_pCApp->m_pCMedicaBase->Medica_SetQListViewOnATC( listView_ATC , atc.remove('%').remove('_') );  //setCurrentItem ( QListViewItem * i )
     //if (pQListViewATCItem) listView_ATC->setCurrentItem( pQListViewATCItem );
    }
 m_pLastIntolItem = pQListViewItem;
 //listView_ATC->clearSelection ();
}

//-------------------------------------- Slot_listView_Produits_contextMenuRequested -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_Produits_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int  )
{listViewAllergies->clearSelection ();
 //listView_ATC->clearSelection ();
 if (pQListViewItem==0)    return;
 QStringList optList;
 optList.append(tr ("Ajouter cet item aux favoris."));
 QString result = DoPopupList(optList, this);
 if (result.length() == 0) return;

 if (result== tr("Ajouter cet item aux favoris."))
    {appendToThesaurus(pQListViewItem->text(0), pQListViewItem->text(4).prepend("(-").append("-)"));
    }
}
//-------------------------------------- initListChoixATCD -------------------------------------------------------
void C_Dlg_GestionATCD::initListChoixATCD()
{ if (G_pCApp->m_pAtcd_Code)
     { G_pCApp->m_pAtcd_Code->atcd_To_ListView(*listViewCim10_Choix);
       m_SaveInitialState = G_pCApp->m_pAtcd_Code->toOldIni();
     }
}

//-------------------------------------- Slot_lineEditNomAllergie_textChanged -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditNomAllergie_textChanged( const QString &qtext )
{long nb = G_pCApp->m_pCMedicaBase->Datasemp_GetSubstanceList(listViewAllergies,  qtext );

 if (nb>1)
     textLabelStatut->setText(QString::number(nb) + tr(" substances trouvées"));
 else if (nb==1)
     textLabelStatut->setText(tr("Une substance trouvée"));
 else
     textLabelStatut->setText(tr("Aucune substance trouvée"));
 lineEditNomAllergie->setFocus ();

 nb = G_pCApp->m_pCMedicaBase->Medica_GetMedicamentList( listView_Produits ,
                                                         "A",
                                                         "",
                                                         "0"
                                                       );
 //................ rechercher uccesivement dans chaque niveau ATC ..........................
 QListViewItem*         pQListViewItem = findListViewItem(listView_ATC, 0, qtext, 0);
 if (pQListViewItem==0) pQListViewItem = findListViewItem(listView_ATC, 1, qtext, 0);
 if (pQListViewItem==0) pQListViewItem = findListViewItem(listView_ATC, 2, qtext, 0);
 if (pQListViewItem==0) pQListViewItem = findListViewItem(listView_ATC, 3, qtext, 0);
 if (pQListViewItem==0) pQListViewItem = findListViewItem(listView_ATC, 4, qtext, 0);
 if (pQListViewItem==0) return;

 pQListViewItem->setOpen (true);
 listView_ATC->setSelected ( pQListViewItem, true );
 listView_ATC->ensureItemVisible ( pQListViewItem  );

}
//-------------------------------------- findListViewItem -------------------------------------------------------
QListViewItem* C_Dlg_GestionATCD::findListViewItem(QListView* pQListView, int depth, const QString &text, int pos_text_in_item /* = 0 */ )
{
    QListViewItemIterator it( pQListView );
    while ( it.current() )
          { it.current()->setOpen (false);
            if ( it.current()->depth()== depth)
               { QString text_item = it.current()->text(pos_text_in_item);
                 if (text_item.contains(text,false)) return it.current();
               }
            ++it;
          }
    return 0;
}

//-------------------------------------- Slot_lineEditNomAllergie_textChanged -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditNomMedicament_textChanged(const QString &qtext)
{long nb = G_pCApp->m_pCMedicaBase->Medica_GetMedicamentList( listView_Produits ,
                                                              qtext,
                                                              "",
                                                              "0"
                                                            );
 if (nb>1)
     textLabelStatut->setText(QString::number(nb) + tr(" substances trouvées"));
 else if (nb==1)
     textLabelStatut->setText(tr("Une substance trouvée"));
 else
     textLabelStatut->setText(tr("Aucune substance trouvée"));
}

//-----------------------------------------------------  OpenBase -------------------------------------------
int   C_Dlg_GestionATCD::OpenBase()
{//if (m_DataBase==0)                                            return 0;
 if (m_CloseAfterQuery==0)                                     return 1;
 if (m_DataBase->isOpen()==FALSE && m_DataBase->open()==FALSE) return 0;
 return 1;
}
//-----------------------------------------------------  CloseBase -------------------------------------------
void C_Dlg_GestionATCD::setOpenCloseMode(int mode)
{m_CloseAfterQuery=mode; if (m_CloseAfterQuery==0 && m_DataBase &&  m_DataBase->isOpen()==FALSE) m_DataBase->open();
}

//-----------------------------------------------------  CloseBase -------------------------------------------
void   C_Dlg_GestionATCD::CloseBase() //m_CloseAfterQuery
{if (m_CloseAfterQuery==0) return;
 if (m_DataBase->isOpen()) m_DataBase->close();
}

//--------------------------------- Utf8_Query ------------------------------------------------------------
QString C_Dlg_GestionATCD::Utf8_Query(QSqlQuery &cur, int field)
{QCString     cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (CGestIni::IsUtf8( ptr , cString.length()))
    {return QString::fromUtf8 ( ptr, cString.length() );
    }
 else
    {return QString(ptr) ;
    }
}

//-----------------------------------------------------  setOnglet -------------------------------------------
void C_Dlg_GestionATCD::setOnglet(int onglet)
{tabWidgetDicoATCD->setCurrentPage (  onglet );
 switch(onglet)
 {case 0: lineEditAutolcatorLibelle->setFocus();       break;
  case 2: lineEditNomAllergie->setFocus();             break;
  case 3: lineEditThesaurusFind1->setFocus();          break;
 }
}
//-----------------------------------------------------  setListCode -------------------------------------------
void C_Dlg_GestionATCD::setListCode(const QString &str_listCode)
{ QStringList listCode = QStringList::split("@@",str_listCode);   // str += item->text(0) + "::"+item->text(1)+"@@";
  for ( QStringList::Iterator it = listCode.begin(); it != listCode.end(); ++it )
      {QString libelle = *it;
       int p = libelle.find("::");
       if (p != -1)
          { AddItemToListChoix(libelle.left(p), libelle.mid(p+2), m_Family);
          }
     }
}
//----------------------------------------------------- dataBaseSet -------------------------------------------
void  C_Dlg_GestionATCD::dataBaseSet(QSqlDatabase  *pQSqlDatabase)
    {
        m_DataBase    = pQSqlDatabase;
        m_DriverName  = m_DataBase->driverName();
        m_BaseName    = m_DataBase->databaseName();
        m_UserName    = m_DataBase->userName();
        m_PassWord    = m_DataBase->password();
        m_HostName    = m_DataBase->hostName();
       if ( ! m_DataBase->isOpen() ) m_DataBase->open();
       initThesaurus();
       initTableCisp();
    }

//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase*  C_Dlg_GestionATCD::BaseConnect(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                              const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                              const char* user,          // = "root"
                                              const char* password,      // = ""
                                              const char* hostname,      // = "localhost"
                                              QString     port,          // = "3306"
                                              QString*    errMess)
    {

        QSqlDatabase *newDB = QSqlDatabase::addDatabase( driver , CIM10_DATA_BASE_NAME);
        if (newDB<=0) return 0;
        newDB->setDatabaseName( DataBaseName );
        newDB->setUserName( user );
        newDB->setPassword( password );
        newDB->setHostName( hostname);
        newDB->setPort( port.toInt());

        if ( ! newDB->open() )
           {QString qstr = "";
            qstr += "Failed to open database: " + QString(driver) + "  " + QString(DataBaseName) + "\r\n" +
                     newDB->lastError().driverText() + "\r\n" + newDB->lastError().databaseText();

            if (errMess) *errMess = qstr;
            else          qWarning(qstr);
            return 0;
           }
        m_DriverName  = driver;
        m_BaseName    = DataBaseName;
        m_UserName    = user;
        m_PassWord    = password;
        m_HostName    = hostname;
        m_Port        = port;
        m_DataBase    = newDB;

        if ( ! m_DataBase->isOpen() ) m_DataBase->open();
        initThesaurus();
        initTableCisp();
        return newDB;
    }

//==================================== ONGLET URGENCES =================================================================

//----------------------------------------------- FiltrerListe -----------------------------------------------
long C_Dlg_GestionATCD::FiltrerListe( QListView *pQlistView, QString str1, QString str2)
{
 pQlistView->clear();
 //............. deux champs vides.....................
 if (str1 == "" && str2 =="")
    {for ( QStringList::Iterator it = m_StringList.begin(); it != m_StringList.end(); ++it )
         {AddItemToListViewUrg (listViewThesaurus, *it);
         }
    }
 //............. deux champs pleins.....................
 else if (str1 != "" && str2 !="")
    {for ( QStringList::Iterator it = m_StringList.begin(); it != m_StringList.end(); ++it )
         {if ((*it).contains (str1, FALSE) &&  (*it).contains (str2, FALSE))
             {AddItemToListViewUrg (listViewThesaurus, *it);
             }
         }
    }
 //.............  1er champ plein.....................
 else if (str1 != "")
    {for ( QStringList::Iterator it = m_StringList.begin(); it != m_StringList.end(); ++it )
         {if ((*it).contains (str1, FALSE))
             {AddItemToListViewUrg (listViewThesaurus, *it);
             }
         }
    }
 //.............  2ème champ plein.....................
 else
    {for ( QStringList::Iterator it = m_StringList.begin(); it != m_StringList.end(); ++it )
         {if ((*it).contains (str2, FALSE))
             {AddItemToListViewUrg (listViewThesaurus, *it);
             }
         }
    }
 return 0;
}

//----------------------------- AddItemToListViewUrg ----------------------------------------------------------
QListViewItem *C_Dlg_GestionATCD::AddItemToListViewUrg (QListView *pQlistView, QString itemText)
{int pos = itemText.find('\t');  // rechercher le separateur entre le code et le libelle
 if (pos == -1) return 0;

 QListViewItem *element = new QListViewItem( pQlistView,
                                             itemText.left(pos),   // libelle
                                             itemText.mid(pos+1)   // code
                                           );
 return element ;
}

//==================================== ONGLET THESAURUS =====================================================================================

//--------------------------------- Slot_listViewCim10_Choix_doubleClicked -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewCim10_Choix_doubleClicked( QListViewItem * pQListViewItem)
{if (pQListViewItem==0) return;
 DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(this); if (!pDlgAtcd_txt)     return;

 pDlgAtcd_txt->setToUpdate(TRUE);
 pDlgAtcd_txt->m_LineEdit_Libelle_Long->hide();

 pDlgAtcd_txt->m_LineEdit_Libelle->setText(pQListViewItem->text(LV_NAME));
 pDlgAtcd_txt->setFamilyGenre(pQListViewItem->text(LV_TYPE));
 pDlgAtcd_txt->m_LineEdit_Commentaire->setText(pQListViewItem->text(LV_COMM));
 pDlgAtcd_txt->cMaskedLineDateDebAtcd->setText(pQListViewItem->text(LV_DDEB).remove('-'));
 pDlgAtcd_txt->cMaskedLineDateFinAtcd->setText(pQListViewItem->text(LV_DFIN).remove('-'));

 //....................... etat ALD Sport ................................
 pDlgAtcd_txt->setSate_Ald_Sport(pQListViewItem->text(5));

 pDlgAtcd_txt->lbl_Titre->setText(tr("Modification d'un antécedent"));
 //pDlgAtcd_txt->setComboOnValue(pDlgAtcd_txt->m_Combo_Family,tr("Médicaux"));
 pDlgAtcd_txt->exec();
 if (pDlgAtcd_txt->isFamilleGenreModified()) {initComboFamilleGenre();}
 if (pDlgAtcd_txt->result() == QDialog::Accepted && pDlgAtcd_txt->m_LineEdit_Libelle->text().stripWhiteSpace().length())
    { QString dateDeb = pDlgAtcd_txt->getDateDeb();
      QString dateFin = pDlgAtcd_txt->getDateFin();
      pQListViewItem->setText(LV_NAME, pDlgAtcd_txt->getLibelle());
      pQListViewItem->setText(LV_TYPE, pDlgAtcd_txt->getFamilleGenre());
      pQListViewItem->setText(LV_COMM, pDlgAtcd_txt->getComment());

      //.............. dates debut fin ...........................
      pQListViewItem->setText(LV_DDEB, dateDeb );
      pQListViewItem->setText(LV_DFIN, dateFin );
      pQListViewItem->setPixmap(LV_DDEB, Atcd_Code::datesDebFinToPixmap(CGenTools::dd_MM_yyyy_ToIsoDate(dateDeb), CGenTools::dd_MM_yyyy_ToIsoDate(dateFin)) );

      //............. ald sport ....................
      QString ald_sport = pDlgAtcd_txt->getSate_Ald_Sport();
      pQListViewItem->setText(LV_ALSP,   ald_sport );
      pQListViewItem->setPixmap(LV_TYPE, Atcd_Code::ald_sport_codeToPixmap(ald_sport));
    }
 delete pDlgAtcd_txt;
}


//--------------------------------- Slot_pushButtonThesaurus_Edit_clicked -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonThesaurus_Edit_clicked()
{QListViewItem *           pQListViewItem =   listViewThesaurus->currentItem ();
 if (pQListViewItem == 0)  pQListViewItem =   listViewThesaurus->selectedItem ();
 if (pQListViewItem == 0)                                              return;
 DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(this); if (!pDlgAtcd_txt) return;
 pDlgAtcd_txt->setDialogForAddToThesaurus();
 pDlgAtcd_txt->setFamilyGenre(pQListViewItem->text(1));
 pDlgAtcd_txt->m_LineEdit_Libelle->setText(pQListViewItem->text(0));
 pDlgAtcd_txt->m_LineEdit_Libelle_Long->setText(pQListViewItem->text(3));
 pDlgAtcd_txt->setComboOnValue(pDlgAtcd_txt->m_Combo_Family,tr("Médicaux"));
 pDlgAtcd_txt->exec();
 if (pDlgAtcd_txt->isFamilleGenreModified()) {initComboFamilleGenre();}
 if (pDlgAtcd_txt->result() == QDialog::Accepted && pDlgAtcd_txt->m_LineEdit_Libelle->text().stripWhiteSpace().length())
    { int id                  = pQListViewItem->text(4).toInt();
      pQListViewItem->setText(0, pDlgAtcd_txt->m_LineEdit_Libelle->text());
      pQListViewItem->setText(1, pDlgAtcd_txt->getFamilleGenre());
      pQListViewItem->setText(3, pDlgAtcd_txt->m_LineEdit_Libelle_Long->text());
      m_ThesaurusList[id]     = thesaurusQListViewItemToText(pQListViewItem);
      Slot_SaveThesaurus();
    }
 delete pDlgAtcd_txt;
}

//--------------------------------- appendToThesaurus -------------------------------------------------------------
void C_Dlg_GestionATCD::appendToThesaurus(const QString &libelle, const QString &code)
{if (m_TAB_ALLERGIE    == -1)
    {
    }
 DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(this);
 if (!pDlgAtcd_txt) return;
 pDlgAtcd_txt->setDialogForAddToThesaurus();
 if (code[0]=='(')
    {QString family;
     QStringList list = m_ATCD_FamilyList.grep (tr("allerg"), FALSE );
     if (list.size())   family = list[0];
     else               family = tr("Allergiques");
     pDlgAtcd_txt->setFamilyGenre(family);
    }
 else
    {pDlgAtcd_txt->setFamilyGenre(tr("Médicaux"));
    }
 pDlgAtcd_txt->m_LineEdit_Libelle->setText(libelle);
 pDlgAtcd_txt->m_LineEdit_Libelle_Long->setText(libelle);

 pDlgAtcd_txt->exec();
 if (pDlgAtcd_txt->isFamilleGenreModified()) {initComboFamilleGenre();}
 if (pDlgAtcd_txt->result() == QDialog::Accepted && pDlgAtcd_txt->m_LineEdit_Libelle->text().stripWhiteSpace().length())
    { int id = m_ThesaurusList.size();
      QListViewItem* pQListViewItem = new QListViewItem( listViewThesaurus, pDlgAtcd_txt->m_LineEdit_Libelle->text(), pDlgAtcd_txt->getFamilleGenre(), code, pDlgAtcd_txt->m_LineEdit_Libelle_Long->text(), QString::number(id));
      if (pQListViewItem)
         { pQListViewItem->setRenameEnabled (0, TRUE );
           m_ThesaurusList.append(thesaurusQListViewItemToText(pQListViewItem));
         }
      Slot_SaveThesaurus();
    }
 delete pDlgAtcd_txt;
}

//--------------------------------- thesaurusQListViewItemToText -------------------------------------------------------------
QString C_Dlg_GestionATCD::thesaurusQListViewItemToText(QListViewItem * pQListViewItem)
{QString libelle = pQListViewItem->text(0);
 QString    code = pQListViewItem->text(2);
 QString  result = libelle.replace(","," ").replace("[","(").replace("]",")") + "," + pQListViewItem->text(1) + "," + code+","+pQListViewItem->text(3)+","+pQListViewItem->text(4);
 return (result);
}

//--------------------------------- Slot_listViewThesaurus_itemRenamed -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewThesaurus_itemRenamed ( QListViewItem * pQListViewItem , int /*col*/, const QString &text  )
{m_pQListViewItem        = pQListViewItem;
 if (pQListViewItem==0) return;
 int id                  = pQListViewItem->text(4).toInt();
 pQListViewItem->text(0) = text;
 m_ThesaurusList[id]     = thesaurusQListViewItemToText(pQListViewItem);
 Slot_SaveThesaurus();
}

//------------------------------------ Slot_listViewThesaurus_contextMenuRequested --------------------------------------------------

void C_Dlg_GestionATCD::Slot_listViewThesaurus_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &/*pos*/, int c )
{
 m_pQListViewItem = pQListViewItem;
 if (pQListViewItem==0) return;
 ThemePopup *pQPopupMenu             = 0;
 QString ret = "";
 QStringList optionList;
 switch(c)
 {case 0:  // libelle usuel
     {
     }
     break;
  case 1:          // famille
     { m_TypeATCD_Selectionne    = "";
       pQPopupMenu               = new ThemePopup(this, "MyPopupMenu" );
       add_popMenu_ATCD_Type(pQPopupMenu);
       pQPopupMenu->exec(QCursor::pos());
       delete pQPopupMenu;
     }
     break;
 }
}
//------------------------------------ Add_popMenu_ATCD_Type --------------------------------------------------
/*! \brief Ajoute le menu de selection du type d'antécedent �  un menu quelconque
*/
void C_Dlg_GestionATCD::add_popMenu_ATCD_Type(QPopupMenu* pQPopupMenu)
{connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_menuActionSetFamilleGenre()));
 G_pCApp->addPopupHierarchique(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", pQPopupMenu, &m_TypeATCD_Selectionne);
}
//------------------------------------ Slot_menuActionSetFamilleGenre --------------------------------------------------
/*! \brief Modifie la rubrique d'un ATCD par le biais de la classe Atcd_Code.
*/
void C_Dlg_GestionATCD::Slot_menuActionSetFamilleGenre()
{
 disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_menuActionSetFamilleGenre()));
 m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 //................. parcourir tous les items ............
 QListViewItemIterator it( listViewThesaurus );
 while ( it.current() )
    { QListViewItem* pQListViewItem = it.current();
      if (pQListViewItem->isSelected())
         {pQListViewItem->setText( 1, m_TypeATCD_Selectionne);
         }
      ++it;
    }
 Slot_SaveThesaurus();
}

//-----------------------------------------------------  Slot_ThesaurusDel -------------------------------------------
void C_Dlg_GestionATCD::Slot_ThesaurusDel()
{int ret = QMessageBox::question( this,
                                  tr("Fermeture du dossier actif"),
                                  tr("Confirmez vous l'effacement de tous les éléments sélectionnés?"),
                                  tr("Tout effacer"),
                                  tr("Annuler"),
                                  QString::null,
                                  1,
                                  1);
 if (ret==1) return;
 QPtrList<QListViewItem> list;
 list.setAutoDelete( TRUE );                            // the list owns the objects
 QListViewItemIterator it( listViewThesaurus );
 int id = -1;
 while ( it.current() )
    { QListViewItem* pQListViewItem = it.current();
      if (pQListViewItem-> isSelected ())
         {id                            = pQListViewItem->text(4).toInt();
          m_ThesaurusList[id]           = QString::null;
         }
          //list.append(pQListViewItem);
      ++it;
    }
 //............ rebatir la liste du thesaurus ............................
 QStringList tmpList;
 for (int i = 0; i < (int)m_ThesaurusList.size(); ++i)
     {if (m_ThesaurusList[i].length())
          {tmpList.append(m_ThesaurusList[i]);
          }
     }
 m_ThesaurusList.clear();
 m_ThesaurusList = tmpList;
 //............... reafficher la nouvlle liste ....................
 filtrerListeThesaurus(listViewThesaurus);
 Slot_SaveThesaurus();
 m_pQListViewItem = 0;
}

//--------------------------------- Slot_comboBox_Genre_highlighted -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_comboBox_Genre_highlighted( const QString& /*section*/ )
{filtrerListeThesaurus(listViewThesaurus);
}
//--------------------------------- Slot_combo_Family_activated -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_combo_Genre_activated( const QString& /*section*/ )
{filtrerListeThesaurus(listViewThesaurus);
}
//--------------------------------- comboBox_Family_highlighted -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_comboBox_Family_highlighted( const QString& section )
{ m_ATCD_GenreList.clear();
 CGestIni::Param_GetList(m_structStr, section, "",  m_ATCD_GenreList );
 m_ATCD_GenreList.prepend(section);
 m_Combo_Genre->clear();
 m_Combo_Genre->insertStringList( m_ATCD_GenreList );

 filtrerListeThesaurus(listViewThesaurus);
}
//--------------------------------- Slot_combo_Family_activated -------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_combo_Family_activated( const QString& section )
{ m_ATCD_GenreList.clear();
 CGestIni::Param_GetList(m_structStr, section, "",  m_ATCD_GenreList );
 m_ATCD_GenreList.prepend(section);
 m_Combo_Genre->clear();
 m_Combo_Genre->insertStringList( m_ATCD_GenreList );

 filtrerListeThesaurus(listViewThesaurus);
}
//-----------------------------------------------------  Slot_lineEditThesaurusFind1_textChanged -------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditThesaurusFind1_textChanged( const QString &/*str*/ )
{filtrerListeThesaurus(listViewThesaurus);
}

//-----------------------------------------------------  FiltrerListeThesaurus -------------------------------------------
void C_Dlg_GestionATCD::filtrerListeThesaurus(QListView* pQListView)
{ //disconnect( m_Combo_Genre, SIGNAL( highlighted(const QString & ) ), this,    SLOT  ( Slot_comboBox_Genre_highlighted( const QString&) ) );
  //disconnect( m_Combo_Genre, SIGNAL( activated(const QString & ) ),   this,    SLOT  ( Slot_combo_Genre_activated( const QString&) ) );

 pQListView->clear();
 QString nomUsuel, famille, genre, code, libelleCim10;
 QString famlilyFilter = m_Combo_Family->currentText();
 QString textFilter    = lineEditThesaurusFind1->text();
 QString genreFilter   = m_Combo_Genre->currentText().stripWhiteSpace();
 int            pos    = -1;

 for (int i = 0; i < (int)m_ThesaurusList.size(); ++i)
     { CGestIni::Param_ExtraireValeurs(m_ThesaurusList[i], &nomUsuel, &famille, &code, &libelleCim10,0,0,0,0,0,0);
       int toShow = 1;
       //............ filtrer selon les familles .....................
       if (m_Combo_Family->currentItem() !=0 )     // famlilyFilter== tr("Toutes les catégories"))
          {genre       = "";
           if ( (pos   = famille.find("(")) !=-1)
              {genre   = famille.mid(pos+1).remove(')').stripWhiteSpace();
               famille = famille.left(pos).stripWhiteSpace();
              }
           if (famille != famlilyFilter)            toShow = 0;
           if (m_Combo_Genre->currentItem() !=0)
              { if (genre != genreFilter)           toShow = 0;
              }
          }
       //............ filtrer selon le texte saisi .....................
       if (nomUsuel.find(textFilter,0,FALSE) == -1) toShow = 0;
       // qDebug( QString("ToShow: %1 ") .arg(toShow) + QString(  "  Famille : '%1' --> familleFilter : '%2'       Genre : '%3' --> genreFilter : '%4'").arg(famille, famlilyFilter, genre, genreFilter));
       //............ filtrer les familles .....................
       if (m_TAB_ALLERGIE==-1)   // a -1 si mode CIM10 seul sans gestion ATCD
          {if (code[0]!='~' )                       toShow = 0;
          }
       if (toShow)
          {QListViewItem* pQListViewItem = new QListViewItem( pQListView, nomUsuel, famille + (genre.length() ? " ("+genre+")":""), code, libelleCim10, QString::number(i));
           if (pQListViewItem) pQListViewItem->setRenameEnabled (0, TRUE );
          }
     }
 m_pQListViewItem = 0;
 //connect( m_Combo_Genre, SIGNAL( highlighted(const QString & ) ), this,    SLOT  ( Slot_comboBox_Genre_highlighted( const QString&) ) );
 //connect( m_Combo_Genre, SIGNAL( activated(const QString & ) ),   this,    SLOT  ( Slot_combo_Genre_activated( const QString&) ) );

}

//-----------------------------------------------------  listViewThesaurus_returnPressed -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewThesaurus_returnPressed( QListViewItem *pQListViewItem )
{if (pQListViewItem==0)                          return;
 if (listViewThesaurus->childCount () !=1)       return;   // on accepte return que lorsque un seul élément est affiché
 m_pQListViewItem = pQListViewItem;

  new QListViewItem( listViewCim10_Choix,      // inserer dans la liste de choix
                     pQListViewItem->text(3),   // libelle
                     pQListViewItem->text(2)    // code CIM10
                   );
 accept();      // ok on se casse avec tout ça
}

//-----------------------------------------------------  Slot_lineEditThesaurusFind1_returnPressed -------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditThesaurusFind1_returnPressed()
{Slot_listViewThesaurus_returnPressed( 0 );
}

//-----------------------------------------------------  serialiseFavoris -------------------------------------------
QString C_Dlg_GestionATCD::serialiseFavoris()
{    listViewThesaurusToThesaurusList();
     QString nomUsuel, famille, code, libelleCim10;
     QString dataToExport;
     int id = 0;
     for (int i = 0; i < (int)m_ThesaurusList.size(); ++i)
         { QString data = m_ThesaurusList[i];
           if (data.length())
              {++id;
               CGestIni::Param_ExtraireValeurs(data, &nomUsuel, &famille, &code, &libelleCim10,0,0,0,0,0,0);
               CGestIni::Param_WriteParam(&dataToExport, "ATCD Thesaurus", QString::number(id), nomUsuel, famille, code, libelleCim10);
              }
         }
     return dataToExport;
}

//-----------------------------------------------------  unSerialiseFavoris -------------------------------------------
void C_Dlg_GestionATCD::unSerialiseFavoris(QString & data)
{m_ThesaurusList.clear();
 CGestIni::Param_GetList(data, "ATCD Thesaurus", "",  m_ThesaurusList );
}
//-----------------------------------------------------  listViewThesaurusToThesaurusList -------------------------------------------
void C_Dlg_GestionATCD::listViewThesaurusToThesaurusList()
{QListViewItemIterator it( listViewThesaurus );
 while ( it.current() )
    { QListViewItem* pQListViewItem = it.current();
      int id = pQListViewItem->text(4).toInt();
      m_ThesaurusList[id] = thesaurusQListViewItemToText(pQListViewItem);
      ++it;
    }
}

//-----------------------------------------------------  initThesaurusListe -------------------------------------------
//  1 = HTA,Medical (cardio vasculaire), ancien, découverte lors d'un AIT,12-12-2009,I10,hypertension essentielle (primitive)

void C_Dlg_GestionATCD::initThesaurusListe()
{
  G_pCApp->m_pCMoteurBase->initThesaurusListe(m_ThesaurusList, G_pCApp->m_User);
  if (m_ThesaurusList.count()==0)
     {QString path = G_pCApp->m_PathAppli+"Ressources/FavorisCIM10.fav";
      if (QFile::exists(path))
         {QString data;
          CGestIni::Param_UpdateFromDisk(path, data );
          unSerialiseFavoris(data);
          Slot_SaveThesaurus();
         }
    }
  filtrerListeThesaurus(listViewThesaurus);
}

//-----------------------------------------------------  Slot_SaveThesaurus -------------------------------------------
void C_Dlg_GestionATCD::Slot_SaveThesaurus()
{
     QString dataToExport = serialiseFavoris();
     G_pCApp->m_pCMoteurBase->saveThesaurus(dataToExport, G_pCApp->m_User);
     /*
     QString dataToExport = serialiseFavoris();
     QSqlQuery query(QString::null, m_DataBase);
     query.exec(QString("DELETE FROM cim10_user_favoris  WHERE User = '") + G_pCApp->m_User + "'");  // on y va bourrin (tres peu de donnees)
     query.prepare("INSERT INTO `cim10_user_favoris` ( `User` , `Data` ) VALUES (?,?) ");
     query.bindValue(0, G_pCApp->m_User);
     query.bindValue(1, dataToExport);
     query.exec();
    */
}

//==================================== ONGLET LIBELLES =================================================================

 #define NUM_LIBELLE_FIELD_FR_OMS     5
 #define NUM_LIBELLE_FIELD_SID        1

 #define NUM_MASTER_FIELD_SID         0
 #define NUM_MASTER_FIELD_CODE        1
 #define NUM_MASTER_FIELD_ID1         6
 #define NUM_MASTER_FIELD_ID2         7
 #define NUM_MASTER_FIELD_ID3         8
 #define NUM_MASTER_FIELD_ID4         9
 #define NUM_MASTER_FIELD_ID5         10
 #define NUM_MASTER_FIELD_ID6         11
 #define NUM_MASTER_FIELD_ID7         12

//-----------------------------------------------------  Slot_listViewCim10_Libelles_contextMenuRequested -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewCim10_Libelles_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &/*pt*/, int /*c*/ )
{if (pQListViewItem==0)    return;
 QStringList optList;
 optList.append(tr ("Ajouter cet item aux favoris."));
 QString result = DoPopupList(optList, this);
 if (result.length() == 0) return;

 if (result== tr("Ajouter cet item aux favoris."))
    {appendToThesaurus(pQListViewItem->text(0), SID_to_CIM10( pQListViewItem->text(2) ).prepend("~").append("~") );
    }
}
//--------------------------------- DoPopupList -----------------------------------------------------
/*! \brief cree et active un popup menu apartir d'une liste d'items
 *  \param list : QStringList qui contient tous les item si icone associee du theme commence par #iconefile#reste du texte
 *  \return une QString qui contient le nom de l'item selectionne.
*/
QString C_Dlg_GestionATCD::DoPopupList(QStringList &list, QWidget *parent)
{ThemePopup *pThemePopup = new ThemePopup(list, parent, "MyPopupMenu");
 if (pThemePopup == 0)     return QString::null;
 QString ret = pThemePopup->DoPopupList();
 delete  pThemePopup;
 return  ret;
}
//-----------------------------------------------------  Slot_checkBox_filter_chapitres_cimxChanged -------------------------------------------
void C_Dlg_GestionATCD::Slot_checkBox_filter_chapitres_cimxChanged(int)
{ CIM10GetLibellesList(listViewCim10_Libelles, lineEditAutolcatorLibelle->text(), lineEditAutolcatorLibelle_2->text(), 0, 0);
}

//-----------------------------------------------------  CIM10GetLibellesList -------------------------------------------
long C_Dlg_GestionATCD::CIM10GetLibellesList(  QListView *pQlistView,
                                               QString    mot_cle_saisie1,
                                               QString    mot_cle_saisie2,
                                               QLabel    *statutMess, /* = 0  */
                                               QLabel    *errMess     /* = 0  */
                                             )
{//................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText(tr("CMoteurBase::CIM10GetLibelleList(): Echec de l'ouverture de la base"));
      return 0;
     }
  int     nb = 200;
  int      i = 0;
  QString code;
  QString requete_libelle;
  requete_libelle           = QString(" SELECT libelle.SID, FR_OMS, code  FROM  libelle"
                                      " join master on master.SID=libelle.SID WHERE ");

  if (mot_cle_saisie1.length() && mot_cle_saisie2.length())
     {requete_libelle       +=  " (   FR_OMS LIKE '%"  + mot_cle_saisie1 + "%'" ;
      requete_libelle       +=  "  OR FR_OMS LIKE '%"  + QString::fromUtf8(mot_cle_saisie1)  + "%' ) AND" ;
      requete_libelle       +=  " (   FR_OMS LIKE '%"  + mot_cle_saisie2 + "%' " ;
      requete_libelle       +=  "  OR FR_OMS LIKE '%"  + QString::fromUtf8(mot_cle_saisie2)  + "%' )" ;
     }
  else if (mot_cle_saisie1.length())
     {requete_libelle       +=  "(   FR_OMS LIKE '%"  + mot_cle_saisie1 + "%' ";
      requete_libelle       +=  " OR FR_OMS LIKE '%"  + QString::fromUtf8(mot_cle_saisie1)  + "%' )" ;
     }
  else if (mot_cle_saisie2.length())
     {requete_libelle       +=  "(   FR_OMS LIKE '%"  + mot_cle_saisie2 + "%' ";
      requete_libelle       +=  " OR FR_OMS LIKE '%"  + QString::fromUtf8(mot_cle_saisie2)  + "%' )" ;
     }
   else
     {requete_libelle       +=  "FR_OMS LIKE 'a%' ";
     }
   if ( !checkBox_filter_chapitres_cimx->isChecked())
        requete_libelle           += " AND code LIKE '%.%'";
  //CONVERT(_utf8 '%...%' USING utf8) COLLATE utf8_general_ci :
  //QCString cst = QString::utf8 () ;
  QSqlQuery query(requete_libelle.utf8() , m_DataBase );

  if (query.isActive())
     {pQlistView->clear();
      while (query.next()&&i<nb)
         {code = query.value(2).toString();
          QListViewItem *element = new QListViewItem( pQlistView,
                                                      Utf8_Query(query, 1 ),
                                                      code,
                                                      query.value(0).toString()
                                                    );
          if (element)
             {i++;
              if (code.contains("."))
                 {element->setPixmap(0, m_CimxCodefinal);
                 }
              else
                 {element->setPixmap(0, m_CimxChapitre);
                 }
             }
         }
     }
 else
     {qDebug(requete_libelle);
      qDebug(query.lastError().text());
     }
  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { QString qstr1;
       qstr1.sprintf(tr("Trouvés : %i sur: %i"), i, nb);
       statutMess->setText(qstr1);
     }
  CloseBase();
  return i;
}



//==================================== ONGLET RUBRIQUES =================================================================
//-----------------------------------------------------  Slot_listViewCim10_rubriques_contextMenuRequested -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewCim10_rubriques_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &/*pt*/, int /*c*/ )
{if (pQListViewItem==0)    return;
 QStringList optList;
 optList.append(tr ("Ajouter cet item aux favoris."));
 QString result = DoPopupList(optList, this);
 if (result.length() == 0) return;

 if (result== tr("Ajouter cet item aux favoris."))
    {appendToThesaurus(pQListViewItem->text(0), SID_to_CIM10( pQListViewItem->text(2) ).prepend("~").append("~"));
    }
}

//-----------------------------------------------------  CIM10GetLibelleList -------------------------------------------

long C_Dlg_GestionATCD::CIM10GetRubriquesList( QListView *pQlistView,
                                               QLabel    *statutMess, /* = 0  */
                                               QLabel    *errMess     /* = 0  */
                                             )
{//................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText(tr("CMoteurBase::CIM10GetLibelleList(): Echec de l'ouverture de la base"));
      return 0;
     }
  QString requete_master;
  QString requete_libelle;

  requete_master        = QString("SELECT * FROM ")    + "master" + " WHERE ";
  requete_master       += QString("level")             + " ='"              + "1" + "'";

  QSqlQuery query(requete_master , m_DataBase );

  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview
  int i  = 0;
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {pQlistView->clear();
      while (query.next())
        {QString qsSID          = query.value(NUM_MASTER_FIELD_SID).toString();
         QString qscode         = query.value(NUM_MASTER_FIELD_CODE).toString();
         requete_libelle        = QString("SELECT * FROM ")    + "libelle" + " WHERE ";
         requete_libelle       += QString("SID")               + " ='"     + qsSID + "'";
         //requete_libelle       += QString(" ORDER BY ")        + "FR_OMS";
         QSqlQuery queryLibelle(requete_libelle , m_DataBase );
         if (queryLibelle.isActive() && queryLibelle.next())
            {//............... on rempli la listbox selon son nombre de colonnes .....................
              new QListViewItem( pQlistView,
                                 Utf8_Query(queryLibelle, NUM_LIBELLE_FIELD_FR_OMS ),
                                 qscode, qsSID
                               );
             i++;
            }
         nb++;
        } //end while (pSqlQueryMaster->next())
     } //endif (pSqlQueryMaster &&pSqlQueryMaster->isActive())
  //ListViewPatient->setCurrentItem(0);   // se placer sur le premier enregistrement de la liste
  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { QString qstr1;
       qstr1.sprintf(tr("Trouvés : %i sur: %i"), i, nb);
       statutMess->setText(qstr1);
     }
  CloseBase();
  return i;
}

//-----------------------------------------------------  CIM10GetSubLevelLibelleList -------------------------------------------
long C_Dlg_GestionATCD::CIM10GetSubLevelLibelleList(  QListViewItem *qlistViewItem,       // pere
                                                      QString        ID_Father,
                                                      QLabel        *statutMess, /* = 0  */
                                                      QLabel        *errMess     /* = 0  */
                                                    )
{ if (qlistViewItem==0)                return 0;
  if (qlistViewItem->childCount ()>0)
     {if (qlistViewItem->isOpen()) qlistViewItem->setOpen (FALSE);
      else                         qlistViewItem->setOpen (TRUE);
      return 0;
     }
  //................. Preparer la requete .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText(tr("CMoteurBase::CIM10GetLibelleList(): Echec de l'ouverture de la base"));
      return 0;
     }
  QString level = "";
  QString id    = "id" + level.setNum (qlistViewItem->depth()+1);
  level.setNum (qlistViewItem->depth()+2);
  QString requete_master;
  QString requete_libelle;

  requete_master        = QString("SELECT * FROM ")    + "master" + " WHERE ";
  requete_master       += QString("level")             + " ='"              + level     + "' AND ";
  requete_master       += id                           + " ='"              + ID_Father + "'";

  QSqlQuery  queryMaster(requete_master , m_DataBase );

  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview
  int i  = 0;
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (queryMaster.isActive())
     {while (queryMaster.next())
        {QString qsSID          = queryMaster.value(NUM_MASTER_FIELD_SID).toString();
         QString qscode         = queryMaster.value(NUM_MASTER_FIELD_CODE).toString();
         requete_libelle        = QString("SELECT * FROM ")    + "libelle" + " WHERE ";
         requete_libelle       += QString("SID")               + " ='"              + qsSID + "'";
         //requete_libelle       += QString(" ORDER BY ")        + "FR_OMS";
         QSqlQuery queryLibelle(requete_libelle , m_DataBase );
         if (queryLibelle.isActive() && queryLibelle.next())
            {new QListViewItem( qlistViewItem,
                                 Utf8_Query(queryLibelle, NUM_LIBELLE_FIELD_FR_OMS ),
                                 qscode, qsSID
                                );
             i++;
            }
         nb++;
        } //end while (pSqlQueryMaster->next())
     } //endif (pSqlQueryMaster &&pSqlQueryMaster->isActive())
  if (qlistViewItem->childCount ()>0) qlistViewItem->setOpen (TRUE);

  //....................... sortir le message d'erreur si besoin ..........................................
  if (statutMess)
     { QString qstr1;
       qstr1.sprintf(tr("Trouvés : %i sur: %i"), i, nb);
       statutMess->setText(qstr1);
     }
  CloseBase();
  return i;
}



//-----------------------------------------------------  GetDlgListCode -------------------------------------------
/*
 #define m_TAB_LIBELLE   0
 #define m_TAB_RUBRIQUE  1
 #define m_TAB_ALLERGIE  2
 #define m_TAB_THESAURUS 3
*/
QListViewItem *C_Dlg_GestionATCD::GetDlgListCode(int tab_index, QListViewItem *pQListViewItem)
{    m_Code    = "";
     m_Family  = "";
     m_Libelle = "";
     qDebug(QString::number(tab_index));
     if (tab_index==m_TAB_LIBELLE)
        { if (pQListViewItem==0) pQListViewItem = listViewCim10_Libelles->currentItem ();
          if (pQListViewItem==0) return 0;
          m_Libelle = pQListViewItem->text(0);
          m_Code    = SID_to_CIM10(pQListViewItem->text(2), 0 ).prepend('~').append('~');
        }
     else if (tab_index==m_TAB_RUBRIQUE)
        { if (pQListViewItem==0) pQListViewItem = listViewCim10_rubriques->currentItem ();
          if (pQListViewItem==0) return 0;
          m_Code    = pQListViewItem->text(1).prepend('~').append('~');
          m_Libelle = pQListViewItem->text(0);
        }
     else if (tab_index==m_TAB_CISP)
        { if (pQListViewItem==0) pQListViewItem = listView_Cisp->currentItem ();
          if (pQListViewItem==0) return 0;
          if (pQListViewItem->text(1).length()<3) return 0;                 // si on est sur un chapitre ou classe
          m_Code    = pQListViewItem->text(1).prepend("-(").append(")-");
          m_Libelle = pQListViewItem->text(0);
          if (m_Code.find('=') != -1)  // traiter le cas des codes composes CISP-CIMX
             { m_Libelle = tr(" CIMX : ") + m_Libelle;
               m_Comment = tr(" CISP : ") + getCispLibelleFromCispCode(pQListViewItem->text(1));
             }
        }
     else if (tab_index==m_TAB_ALLERGIE)
        { if ( (m_pLastIntolItem=listViewAllergies->selectedItem()) )
             {m_Libelle     = m_pLastIntolItem->text(0);
              m_Code        = m_pLastIntolItem->text(1).prepend('(').append(')');
             }
          else if ( (m_pLastIntolItem=listView_Produits->selectedItem()) )
             {m_Libelle     = m_pLastIntolItem->text(0);
              m_Code        = m_pLastIntolItem->text(4).prepend("(-").append("-)");
             }
          else if ( (m_pLastIntolItem=listView_ATC->selectedItem()) )
             {m_Libelle     = m_pLastIntolItem->text(0);
              m_Code        = m_pLastIntolItem->text(1).prepend("(.").append(".)");
             }
          QStringList list  = m_ATCD_FamilyList.grep (tr("allerg"), FALSE );
          if (list.size())    m_Family = list[0];
          else                m_Family = tr("Allergiques");
        }
     else if (tab_index==m_TAB_THESAURUS)
        { if (pQListViewItem==0) pQListViewItem = listViewThesaurus->currentItem ();
          if (pQListViewItem==0) return 0;
          m_Code    = pQListViewItem->text(2);
          m_Libelle = pQListViewItem->text(3);
          m_Family  = pQListViewItem->text(1);
        }
 return pQListViewItem;
}
//-----------------------------------------------------  GetDlgListViewFromTab -------------------------------------------
/*
 #define m_TAB_LIBELLE   0
 #define m_TAB_RUBRIQUE  1
 #define m_TAB_ALLERGIE  2
 #define m_TAB_THESAURUS 3
*/
QListView *C_Dlg_GestionATCD::GetDlgListViewFromTab(int tab_index)
{    if (tab_index==m_TAB_LIBELLE)
        { return listViewCim10_Libelles;
        }
     else if (tab_index==m_TAB_RUBRIQUE)
        { return listViewCim10_rubriques;
        }
     else if (tab_index==m_TAB_CISP)
        { return listView_Cisp;
        }
     else if (tab_index==m_TAB_ALLERGIE)
        { if (m_pLastIntolItem==0) return 0;
          if      ( listViewAllergies->selectedItem() == m_pLastIntolItem ) return listViewAllergies;
          else if ( listView_Produits->selectedItem() == m_pLastIntolItem ) return listView_Produits;
          else if ( listView_ATC->selectedItem()      == m_pLastIntolItem ) return listView_ATC;
          return 0;
        }
     else if (tab_index==m_TAB_THESAURUS)
        { return listViewThesaurus;
        }
 return 0;
}
//-----------------------------------------------------  setLineEditFocusFromTab -------------------------------------------
/*
 #define m_TAB_LIBELLE   0
 #define m_TAB_RUBRIQUE  1
 #define m_TAB_ALLERGIE  2
 #define m_TAB_THESAURUS 3
*/
void C_Dlg_GestionATCD::setLineEditFocusFromTab(int tab_index)
{    if (tab_index==m_TAB_LIBELLE)
        { lineEditAutolcatorLibelle_2->setText("");
          lineEditAutolcatorLibelle->selectAll();
          lineEditAutolcatorLibelle->setFocus();
        }
     if (tab_index==m_TAB_CISP)
        { lineEditAutolcator_Cisp2->setText("");
          lineEditAutolcator_Cisp1->selectAll();
          lineEditAutolcator_Cisp1->setFocus();
        }
     else if (tab_index==m_TAB_RUBRIQUE)
        { return;
        }
     else if (tab_index==m_TAB_ALLERGIE)
        {lineEditNomAllergie->selectAll();
         lineEditNomAllergie->setFocus();
        }
     else if (tab_index==m_TAB_THESAURUS)
        {lineEditThesaurusFind1->selectAll();
         lineEditThesaurusFind1->setFocus();
        }
}
//----------------------------------------------------- AddItemToListChoix --------------------------------------
void C_Dlg_GestionATCD::AddItemToListChoix(const QString &libelle_in, const QString &code, const QString familyGenre)
{ int posEgal      = code.find("=");
  QString libelle  = libelle_in;
  QString comment  = "";
  if (posEgal != -1)            // Traiter le cas des codes composes CISP=CIMX (le libelle est alors celui du CIMX)
     {comment  = tr(" CISP : ") + getCispLibelleFromCispCode(code);  // traite aussi le cas des codes composes et encadres par "-("
      //libelle  = tr(" CIMX : ") + libelle; deja fait en amont
     }
  if (m_TAB_ALLERGIE==-1)   // a -1 si mode CIM10 seul sans gestion ATCD ou mode Cisp seul
     {QDate dt_deb= QDate();
      QDate dt_fin= QDate();
      // m_Rubrique=,m_Libelle,m_Code=,m_Etat=0,m_Commentaire=,m_Id_ATCD,m_ald
      Atcd_Element atcd_Element ("", libelle, dt_deb , code,dt_fin, comment,"");
      G_pCApp->m_pAtcd_Code->atcd_Element_To_ListViewItem(atcd_Element, listViewCim10_Choix);
      return;
     }
   //......... si item atcd rajoute rajouter celui de la listView allant avec ......................
  G_pCApp->m_pAtcd_Code->addATCD(this, libelle, code, familyGenre, comment, Atcd_Code::sendNotModifMessage);
  Atcd_Element* pAtcd_Element = G_pCApp->m_pAtcd_Code->get_lastElementAdded();
  if (pAtcd_Element) G_pCApp->m_pAtcd_Code->atcd_Element_To_ListViewItem(*pAtcd_Element, listViewCim10_Choix);
}

//-----------------------------------------------------  SID_to_CIM10 -------------------------------------------
QString C_Dlg_GestionATCD::SID_to_CIM10(QString qsSID,  QLabel *errMess )
{  QString qscode("");
  //................. verifier validité de la base .....................................
  if (OpenBase()==0)
     {if (errMess) errMess->setText(tr("CMoteurBase::CIM10GetLibelleList(): Echec de l'ouverture de la base"));
      return qscode;
     }
  //.................. preparer la requete .....................................................
  QString requete_master;
  requete_master        = QString("SELECT * FROM master WHERE SID = '") + qsSID + "'";

  QSqlQuery* pSqlQueryMaster = new QSqlQuery(requete_master , m_DataBase );
  //.................. si la requète a un resultat ..............................................
  if (pSqlQueryMaster && pSqlQueryMaster->isActive() && pSqlQueryMaster->next())
     {qscode         = pSqlQueryMaster->value(NUM_MASTER_FIELD_CODE).toString();
     } //endif (pSqlQueryMaster &&pSqlQueryMaster->isActive())
  if (pSqlQueryMaster) delete  pSqlQueryMaster;
  CloseBase();
  return qscode;
}


//////////////////////////////////////////////////////////////// SLOTS ///////////////////////////////////////////////////////////////
//-----------------------------------------------------  Slot_pushButton_AddToChoixFast_clicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButton_AddToChoixFast_clicked()
{
 QWidget     *pQWidget = tabWidgetDicoATCD->currentPage();
 if (pQWidget==0)              return;
 int         tab_index = tabWidgetDicoATCD->indexOf ( pQWidget );
 QListView *pQListView = C_Dlg_GestionATCD::GetDlgListViewFromTab(tab_index); // recuperer la listview source en fonction de l'onglet
 if (pQListView==0)            return;
 QListViewItemIterator it( pQListView );
 while ( it.current() )
    {
      CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
      if (pCPrt->isSelected())
         {QDate dt_deb = QDate();
          QDate dt_fin = QDate();
          GetDlgListCode(tab_index, pCPrt);
          Atcd_Element atcd_Element (m_Family, m_Libelle, dt_deb , m_Code,dt_fin, m_Comment,"");
          G_pCApp->m_pAtcd_Code->atcd_Element_To_ListViewItem(atcd_Element, listViewCim10_Choix);
         }
      ++it;
    }
 setLineEditFocusFromTab(tab_index);

}

//-----------------------------------------------------  Slot_pushButton_AddToChoix_clicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButton_AddToChoix_clicked()
{QWidget * pQWidget = tabWidgetDicoATCD->currentPage();
 if (pQWidget==0)              return;

 GetDlgListCode(tabWidgetDicoATCD->indexOf ( pQWidget), 0);
 int            tab_index = tabWidgetDicoATCD->indexOf ( pQWidget);

 if (m_Libelle != "") AddItemToListChoix(m_Libelle, m_Code, m_Family);
 setLineEditFocusFromTab(tab_index);
}

//-----------------------------------------------------  Slot_pushButtonRemoveFromChoix_clicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_pushButtonRemoveFromChoix_clicked()
{QListViewItem *element = listViewCim10_Choix->currentItem ();
 if (element == 0) return;
 delete element;
}

//-----------------------------------------------------  Slot_listViewCim10_Libelles_doubleClicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewCim10_Libelles_doubleClicked( QListViewItem *pQListViewItem )
{common_listView_doubleClicked(pQListViewItem);
}
//-------------------------------------- Slot_listView_ATC_doubleClicked -------------------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_ATC_doubleClicked(QListViewItem *pQListViewItem)
{common_listView_doubleClicked(pQListViewItem);
}
//-----------------------------------------------------  Slot_listViewCim10_rubriques_doubleClicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewCim10_rubriques_doubleClicked( QListViewItem *pQListViewItem )
{common_listView_doubleClicked(pQListViewItem);
}
//-----------------------------------------------------  Slot_listViewThesaurus_doubleClicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewThesaurus_doubleClicked( QListViewItem *pQListViewItem )
{common_listView_doubleClicked(pQListViewItem);
}
//-----------------------------------------------------  Slot_listView_Produits_doubleClicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_listView_Produits_doubleClicked( QListViewItem *pQListViewItem )
{common_listView_doubleClicked(pQListViewItem);
}
//-----------------------------------------------------  Slot_listViewAllergies_doubleClicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewAllergies_doubleClicked( QListViewItem *pQListViewItem )
{common_listView_doubleClicked(pQListViewItem);
}
//-----------------------------------------------------  common_listView_doubleClicked -------------------------------------------
void C_Dlg_GestionATCD::common_listView_doubleClicked( QListViewItem *pQListViewItem )
{QWidget * pQWidget = tabWidgetDicoATCD->currentPage();
 if (pQWidget==0) return;
 GetDlgListCode(tabWidgetDicoATCD->indexOf ( pQWidget), pQListViewItem);
 if (m_Libelle!= "")
    {AddItemToListChoix(m_Libelle, m_Code, m_Family);
    }
 Slot_accept();
}
//-----------------------------------------------------  close -----------------------------------------------------------------
void C_Dlg_GestionATCD::close()
{writeWindowPos();
}

//-----------------------------------------------------  Slot_accept -----------------------------------------------------------------
void C_Dlg_GestionATCD::Slot_accept()
{
  if (G_pCApp->m_pAtcd_Code && m_TAB_ALLERGIE != -1)     // m_TAB_ALLERGIE = -1 si CIM10 seulement et non gestion des ATCD
     { G_pCApp->m_pAtcd_Code->updateFromListViewTerrain( listViewCim10_Choix );
     }
  writeWindowPos();
  accept();
}
//-----------------------------------------------------  Slot_reject -------------------------------------------
void C_Dlg_GestionATCD::Slot_reject()
{ if (G_pCApp->m_pAtcd_Code && m_TAB_ALLERGIE != -1)     // m_TAB_ALLERGIE = -1 si CIM10 seulement et non gestion des ATCD
     { G_pCApp->m_pAtcd_Code->setTerrain(m_SaveInitialState,0);
       G_pCApp->m_pAtcd_Code->emitATCD_Changed();
     }
  writeWindowPos();
  reject();
}
//-----------------------------------------------------  writeWindowPos -------------------------------------------
void C_Dlg_GestionATCD::writeWindowPos()
{ QValueList <int> list = splitter5->sizes();
  QString cisp_filter_cimx_checkBox_state      = checkBox_Cisp_filter_cimx->isChecked()?"Actif":"Inactif";
  QString checkBox_filter_chapitres_cimx_state = checkBox_filter_chapitres_cimx->isChecked()?"Actif":"Inactif";
  WRITE_USER_PARAM  (&USER_PARAM, "Dlg_ATCD", "WindowPos", QString::number(x()),
                                                           QString::number(y()),
                                                           QString::number(width()),
                                                           QString::number(height()),
                                                           QString::number(list[0]),
                                                           QString::number(list[1]));
  WRITE_USER_PARAM  (&USER_PARAM, "Dlg_ATCD", "Lister codes Cim10 avec Cisp", cisp_filter_cimx_checkBox_state);
  WRITE_USER_PARAM  (&USER_PARAM, "Dlg_ATCD", "Afficher les chapitres Cim10", checkBox_filter_chapitres_cimx_state);
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User);
}
 //-----------------------------------------------------  readAndSetWindowPos -------------------------------------------
void C_Dlg_GestionATCD::readAndSetWindowPos()
{QString x,y,w,h,spliterW0,spliterW1, cisp_filter_cimx_checkBox_state, checkBox_filter_chapitres_cimx_state;    // checkBox_Cisp_filter_cimx
 QValueList <int> list;
 if (READ_USER_PARAM(USER_PARAM, "Dlg_ATCD", "WindowPos", &x,&y,&w,&h,&spliterW0,&spliterW1)==0)  // zero = pas d'erreur
    {move(x.toInt(),     y.toInt());
     resize(w.toInt(),   h.toInt());
     list.append( spliterW0.toInt());
     list.append( spliterW1.toInt());
     splitter5->setSizes(list);
    }
 if (READ_USER_PARAM(USER_PARAM, "Dlg_ATCD", "Lister codes Cim10 avec Cisp", &cisp_filter_cimx_checkBox_state)==0)  // zero = pas d'erreur
    {checkBox_Cisp_filter_cimx->setChecked(cisp_filter_cimx_checkBox_state.upper().left(1)=="A");
    }
 if (READ_USER_PARAM(USER_PARAM, "Dlg_ATCD", "Afficher les chapitres Cim10", &checkBox_filter_chapitres_cimx_state)==0)  // zero = pas d'erreur
    {checkBox_filter_chapitres_cimx->setChecked(checkBox_filter_chapitres_cimx_state.upper().left(1)=="A");
    }
}

//-----------------------------------------------------  Slot_listViewCim10_rubriques_clicked -------------------------------------------
void C_Dlg_GestionATCD::Slot_listViewCim10_rubriques_clicked( QListViewItem * qlistViewItem )
{if (qlistViewItem==0) return;
 CIM10GetSubLevelLibelleList(qlistViewItem,       // pere
                             qlistViewItem->text(2),
                             0, /* = 0  */
                             0 /* = 0  */
                            );
}

//-----------------------------------------------------  Slot_tabWidgetDicoATCD_currentChanged -------------------------------------------

/*
 #define m_TAB_LIBELLE   0
 #define m_TAB_RUBRIQUE  1
 #define m_TAB_ALLERGIE  2
 #define m_TAB_THESAURUS 3
 #define m_TAB_CISP      4
*/
void C_Dlg_GestionATCD::Slot_tabWidgetDicoATCD_currentChanged( QWidget * pQWidget)
{
     int tab_index = tabWidgetDicoATCD->indexOf ( pQWidget);
     if (tab_index==m_TAB_LIBELLE)
        { pushButtonThesaurusAdd->setEnabled(TRUE);
          if (listViewCim10_Libelles->childCount ()<=0)
             {CIM10GetLibellesList(listViewCim10_Libelles, QString(""), QString(""), 0, 0);
             }
        }
     else if (tab_index==m_TAB_RUBRIQUE)
        { pushButtonThesaurusAdd->setEnabled(TRUE);
          if (listViewCim10_rubriques->childCount ()<=0)
             {CIM10GetRubriquesList( listViewCim10_rubriques,0,0);
              lineEditAutolcatorLibelle->setFocus ();
             }
        }
     else if (tab_index==m_TAB_ALLERGIE)
        { pushButtonThesaurusAdd->setEnabled(TRUE);
          lineEditNomAllergie->setFocus ();
        }
     else if (tab_index==m_TAB_CISP)
        { pushButtonThesaurusAdd->setEnabled(TRUE);
          lineEditAutolcator_Cisp1->setFocus ();
        }
     else if (tab_index==m_TAB_THESAURUS)
        { pushButtonThesaurusAdd->setEnabled(FALSE);
          lineEditThesaurusFind1->setFocus ();
          listViewThesaurus->selectAll (FALSE );
          QListViewItem* pQListViewItem = listViewThesaurus->firstChild();
          if (pQListViewItem)
             {listViewThesaurus->setCurrentItem ( pQListViewItem );
              listViewThesaurus->setSelected ( pQListViewItem, TRUE );
             }
        }
}

//-----------------------------------------------------  Slot_lineEditAutolcatorLibelle_textChanged -------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditAutolcatorLibelle_textChanged( const QString & )
{CIM10GetLibellesList(listViewCim10_Libelles, lineEditAutolcatorLibelle->text(), lineEditAutolcatorLibelle_2->text(), 0, 0);
}
//-----------------------------------------------------  Slot_lineEditAutolcatorLibelle_textChanged -------------------------------------------
void C_Dlg_GestionATCD::Slot_lineEditAutolcatorLibelle_2_textChanged( const QString & )
{CIM10GetLibellesList(listViewCim10_Libelles, lineEditAutolcatorLibelle->text(), lineEditAutolcatorLibelle_2->text(), 0, 0);
}
