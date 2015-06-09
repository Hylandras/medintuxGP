/*********************************** C_RubObservation.cpp *************************
 *                                                                                *
 *   #include "C_RubObservation.h"                                                *
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
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qclipboard.h>

#include <qvariant.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qtoolbar.h>


#include "C_RubObservation.h"
#include "CMDI_Observation.h"

C_RubObservation::C_RubObservation(CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
                                   const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user,
                                   QWidget* parent , const char* rubName , QString prDroit, WFlags fl /* = 0 */ )
    : QWidget( parent, rubName, fl ), m_pCMoteurBase(pCMoteurBase), m_pRubList(pRubList)
{   if ( !rubName )  setName( "RubNoName" );
    m_RubType     = G_pCApp->RubNameToType(rubName);
    m_PrefixDroit = prDroit;
    setMinimumSize( QSize( 0, 32 ) );
    setMaximumSize( QSize( 32767, 32767 ) );

    m_IsModifiable  =  0;
    m_pMyEditText   =  0;
    m_LastRub       = -1;
    m_MaxImageWidth = 600;
    QString   val1  = "";

    if (READ_LOCAL_PARAM(LOCAL_PARAM, rubName, "MaxImageWidth", &val1 )==0) m_MaxImageWidth =  val1.toInt(); // splash utilisé provisoirement

    if (num_GUID)    m_NumGUID    = num_GUID;
    else             m_NumGUID    = "";
    if (id_doss)     m_ID_Doss    = id_doss;
    else             m_ID_Doss    = "";
    if (dossNom)     m_DossNom    = dossNom;
    else             m_DossNom    = "";
    if (dossPrenom)  m_DossPrenom = dossPrenom;
    else             m_DossPrenom = "";
    if (dossPrenom)  m_User       = user;
    else             m_User       = "";

    //..................... vient de designer ...............................................
    m_pQToolBar_Combo  = new QToolBar( (QMainWindow*)parent );
    m_pQToolBar_Button = new QToolBar( (QMainWindow*)parent );
    //m_pQToolBar_Button->setOrientation (Qt::Vertical  );
    ((QMainWindow*)parent)->moveDockWindow(m_pQToolBar_Button, Qt::DockLeft);

    FormRubDocumentsLayout = new QGridLayout( this, 1, 1, 0, 0, "FormRubObservationLayout");

    textLabel_NbRub = new QPushButton( m_pQToolBar_Combo, "textLabel_NbRub" );
    textLabel_NbRub->setProperty( "minimumSize", QSize( 40, 22 ) );
    textLabel_NbRub->setProperty( "maximumSize", QSize( 40, 22 ) );
#ifdef Q_OS_MACX
	textLabel_NbRub->setFlat (TRUE); 
	textLabel_NbRub->setMinimumSize (50, 22);
	textLabel_NbRub->setFont(G_pCApp->font());
#endif	
    comboBox_RubName = new QComboBox( FALSE, m_pQToolBar_Combo, "comboBox_RubName" );
    comboBox_RubName->setProperty( "minimumSize", QSize( 200, 22 ) );
    comboBox_RubName->setProperty( "maximumSize", QSize( 32767, 22 ) );
    QFont comboBox_RubName_font(  comboBox_RubName->font() );
    comboBox_RubName->setFont( comboBox_RubName_font );
    comboBox_RubName->setProperty( "sizeLimit", 20 );

    pushButtonRubDateChange = new QPushButton( m_pQToolBar_Combo, "pushButtonRubDateChange" );
    pushButtonRubDateChange->setProperty( "minimumSize", QSize( 22, 22 ) );
    pushButtonRubDateChange->setProperty( "maximumSize", QSize( 22, 22 ) );

    textLabel1 = new QLabel( m_pQToolBar_Combo, "textLabel1" );
    textLabel1->setProperty( "minimumSize", QSize( 55, 22 ) );
    textLabel1->setProperty( "maximumSize", QSize( 55, 22 ) );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1->setFont( textLabel1_font );

    textLabel_UserName = new QLabel( m_pQToolBar_Combo, "textLabel_UserName" );
    textLabel_UserName->setProperty( "minimumSize", QSize( 100, 22 ) );
    textLabel_UserName->setProperty( "maximumSize", QSize( 115, 22 ) );
    QFont textLabel_UserName_font(  textLabel_UserName->font() );
    textLabel_UserName->setFont( textLabel_UserName_font );
    textLabel_UserName->setProperty( "frameShape", "StyledPanel" );
    textLabel_UserName->setProperty( "frameShadow", "Sunken" );
    textLabel_UserName->setProperty( "scaledContents", QVariant( FALSE, 0 ) );
    textLabel_UserName->setProperty( "alignment", int( QLabel::AlignCenter ) );

    pushButtonSave = new QPushButton( m_pQToolBar_Button, "pushButtonSave" );
    pushButtonSave->setProperty( "maximumSize", QSize( 35, 30 ) );

    pushButtonPrint = new QPushButton( m_pQToolBar_Button, "pushButtonPrint" );
    pushButtonPrint->setProperty( "maximumSize", QSize( 35, 30 ) );

    pushButtonDelete = new QPushButton( m_pQToolBar_Button, "pushButtonDelete" );
    pushButtonDelete->setProperty( "maximumSize", QSize( 35, 30 ) );

    pushButtonAdd = new QPushButton( m_pQToolBar_Button, "pushButtonAdd" );
    pushButtonAdd->setProperty( "maximumSize", QSize( 35, 30 ) );

    pushButtonExporter = new QPushButton( m_pQToolBar_Button, "pushButtonExporter" );
    pushButtonExporter->setProperty( "maximumSize", QSize( 35, 30 ) );

    languageChange();
    resize( QSize(839, 148).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( comboBox_RubName, pushButtonRubDateChange );
    setTabOrder( pushButtonRubDateChange, pushButtonSave );
    setTabOrder( pushButtonSave, pushButtonPrint );
    setTabOrder( pushButtonPrint, pushButtonDelete );
    setTabOrder( pushButtonDelete, pushButtonExporter );
    //.................................................................

    pushButtonSave->setIconSet           (Theme::getIconSave());
    pushButtonPrint->setIconSet          (Theme::getIconPrint());
    pushButtonDelete->setIconSet         (Theme::getIconDelete());
    pushButtonRubDateChange->setIconSet  (Theme::getIconChangeDate());
    pushButtonAdd->setIconSet            (Theme::getIcon("22x22/fileopen.png") );
    pushButtonExporter->setIconSet       (Theme::getIcon("filesaveExport.png") );

 //.................. on crée le splitter vertical qui contiendra ........................................
 //                   l'icon  view à gauche et le viewer de documments à droite
 //m_splitterVertical  = new QSplitter( this, "splitter1" );
 //m_splitterVertical->setOrientation( QSplitter::Horizontal );

 //m_iconViewDocuments = new QIconView( m_splitterVertical, "iconViewDocuments" );

 m_pMyEditText = new MyEditText( pCMoteurBase->m_DataBase, this, rubName );
 m_pMyEditText->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, m_pMyEditText->sizePolicy().hasHeightForWidth() ) );
 m_pMyEditText->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
 m_pMyEditText->SetTypeRubrique(m_RubType);
 m_pMyEditText->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
 /*
 connect( m_pMyEditText,     SIGNAL( Signal_Key_Delete_Pressed(int)),      this,    SLOT  ( CheckIfImageIsToRemoveFromIconView(int))  );
 connect( m_pMyEditText,     SIGNAL( Signal_Key_Backspace_Pressed(int)),   this,    SLOT  ( CheckIfImageIsToRemoveFromIconView(int))  );
 connect( m_pMyEditText,     SIGNAL( Signal_Del_Before()),                 this,    SLOT  ( On_Signal_Del_Before())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Del_After()),                  this,    SLOT  ( On_Signal_Del_After())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Replace_Before()),             this,    SLOT  ( On_Signal_Del_Before())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Replace_After()),              this,    SLOT  ( On_Signal_Del_After())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Cut_Before()),                 this,    SLOT  ( On_Signal_Del_Before())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Cut_After()),                  this,    SLOT  ( On_Signal_Del_After())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Paste_Before()),               this,    SLOT  ( On_Signal_Del_Before())    );
 connect( m_pMyEditText,     SIGNAL( Signal_Paste_After()),                this,    SLOT  ( On_Signal_Paste_After())  );
*/
 //........................ ajouter cela au dialogue principal .........................................
 //void addMultiCellWidget ( QWidget * w, int fromRow, int toRow, int fromCol, int toCol, int alignment = 0 )
 FormRubDocumentsLayout->addMultiCellWidget( m_pMyEditText, 0, 0, 0, 9 );
 //setDefaultWordWrapFromIniFile();
 //m_iconViewDocuments->hide();
 pushButtonExporter->hide();
 //QValueList <int> list;
 //list.append(100);
 //list.append(width()-100);
 //m_splitterVertical->setSizes(list);

 initData();
 // signals and slots connections
 connect( textLabel_NbRub,         SIGNAL( pressed() ),        this, SLOT( On_buttonDragClicked() ));
 connect( pushButtonDelete,        SIGNAL( clicked() ),        this, SLOT( pushButtonDelete_clicked() ) );
 connect( pushButtonSave,          SIGNAL( clicked() ),        this, SLOT( pushButtonSave_clicked() ) );
 connect( pushButtonRubDateChange, SIGNAL( clicked() ),        this, SLOT( pushButtonRubDateChange_clicked() ) );
 connect( comboBox_RubName,        SIGNAL( highlighted(int) ), this, SLOT( comboBox_RubName_highlighted(int) ) );
 connect( comboBox_RubName,        SIGNAL( activated(int) ),   this, SLOT( comboBox_RubName_activated(int) ) );
 
 #ifdef Q_OS_MACX
    pushButtonAdd->setFlat (TRUE); 
    pushButtonPrint->setFlat (TRUE); 
    pushButtonSave->setFlat (TRUE); 
    pushButtonPrint->setFlat (TRUE); 
    pushButtonDelete->setFlat (TRUE); 
    pushButtonRubDateChange->setFlat (TRUE); 
    pushButtonExporter->setFlat (TRUE);
#endif

}

C_RubObservation::~C_RubObservation()
{
}
//----------------------------------------- languageChange --------------------------------
void C_RubObservation::languageChange()
{
    setProperty( "caption", tr( "InvisibleStringFormPrescription" ) );
    textLabel_NbRub->setProperty( "text", QString::null );
    QToolTip::add( textLabel_NbRub, trUtf8( "\x4d\x6f\x64\x69\x66\x69\x65\x72\x20\x6c\x61\x20\x64\x61\x74\x65\x20\x65\x74\x20\x6c"
    "\x27\x69\x6e\x74\x69\x74\x75\x6c\xc3\xa9\x20\x64\x75\x20\x64\x6f\x63\x75\x6d\x65"
    "\x6e\x74" ) );
    pushButtonRubDateChange->setProperty( "text", QString::null );
    QToolTip::add( pushButtonRubDateChange, trUtf8( "\x4d\x6f\x64\x69\x66\x69\x65\x72\x20\x6c\x61\x20\x64\x61\x74\x65\x20\x65\x74\x20\x6c"
    "\x27\x69\x6e\x74\x69\x74\x75\x6c\xc3\xa9\x20\x64\x75\x20\x64\x6f\x63\x75\x6d\x65"
    "\x6e\x74" ) );
    textLabel1->setProperty( "text", tr( " vu par :" ) );
    textLabel_UserName->setProperty( "text", tr( "Utilisateur" ) );
    pushButtonSave->setProperty( "text", QString::null );
    QToolTip::add( pushButtonSave, tr( "Enregistrer tout le dossier" ) );
    pushButtonPrint->setProperty( "text", QString::null );
    QToolTip::add( pushButtonPrint, trUtf8( "\x49\x6d\x70\x72\x69\x6d\x65\x72\x20\x63\x65\x74\x74\x65\x20\x66\x65\x6e\xc3\xaa\x74"
    "\x72\x65" ) );
    pushButtonDelete->setProperty( "text", QString::null );
    QToolTip::add( pushButtonDelete, trUtf8( "\x50\x6c\x61\x63\x65\x72\x20\x6c\x65\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x20\x65\x6e"
    "\x20\x63\x6f\x75\x72\x73\x20\x63\x6f\x6d\x6d\x65\x20\x64\x6f\x63\x75\x6d\x65\x6e"
    "\x74\x20\xc3\xa0\x20\x65\x66\x66\x61\x63\x65\x72\x20\x28\x6e\x65\x20\x73\x65\x72"
    "\x61\x20\x65\x66\x66\x61\x63\xc3\xa9\x20\x71\x75\x27\x61\x70\x72\xc3\xa8\x73\x20"
    "\x65\x6e\x72\x65\x67\x69\x73\x74\x72\x65\x6d\x65\x6e\x74\x29" ) );
    pushButtonAdd->setProperty( "text", QString::null );
    QToolTip::add( pushButtonAdd, tr( "Nouveau document..." ) );
    pushButtonExporter->setProperty( "text", QString::null );
    QToolTip::add( pushButtonExporter, tr( "Exporter cette image..." ) );
}

/*$SPECIALIZATION$*/
//=============================================== IMPLEMENTTION =====================================================

//--------------------------------- On_buttonDragClicked ------------------------------------------------------
// connect(textLabel_NbRub, SIGNAL( clicked() ), this, SLOT( On_buttonDragClicked() ));
//   virtual void          On_buttonDragClicked();
void  C_RubObservation::On_buttonDragClicked()
{   RUBREC_LIST::iterator it = Current_RubList_Iterator();
    if ( it != m_pRubList->end() ) (*it).dragRubrique(this);
}

//--------------------------------- SetModifiableState -----------------------------------------------------------------------
void  C_RubObservation::SetModifiableState(int state)
{m_IsModifiable = state;
 if (m_IsModifiable)
    {pushButtonSave->show();
     pushButtonRubDateChange->show();
     pushButtonDelete->show();
    }
 else
    {pushButtonSave->hide();
     pushButtonRubDateChange->hide();
     pushButtonDelete->hide();
    }
}

//--------------------------------- FormPatientList_destroyed -------------------------------------------------------------
void C_RubObservation::FormPatientList_destroyed( QObject * )
{//if (m_pMyEditText!=0) m_pMyEditText->mimeSourceFactory()->setData( "myimage", 0 );
}
//----------------------------- clearDocument -----------------------------------
/*! \brief Fonction efface le document en cours (images, liste des références d'images, l'iconview)
*/
void C_RubObservation::clearDocument()
{m_pMyEditText->setText("");
 //G_pCApp->m_C_ImageList.resetList();          // remet à zero la listes de references et enlève les images de la factory (pas bon pour le copier coller)
 G_pCApp->m_C_ImageList.resetRef();
 //m_iconViewDocuments->clear();
}
//----------------------------------------- initData --------------------------------
/*! \brief Met à jour le ComboBox et le texte. */
int C_RubObservation::initData()
{
 if (m_pCMoteurBase==0) return 0;
 //................. mettre à jour le texte ............................
 long nb = initComboListHistoric();
 if (nb>0)
    {comboBox_RubName->setCurrentItem (nb-1);
     comboBox_RubName_activated( nb-1);
     if (m_pMyEditText!=0) {m_pMyEditText->m_State = 1;
                            m_pMyEditText->show();
                           }
     show();
    }
 else
    {if (m_pMyEditText!=0){m_pMyEditText->m_State = 0;
                           m_pMyEditText->hide();
                           textLabel_UserName->setText("");
                           m_pMyEditText->setText("");
                          }
     //hide();
    }
 textLabel_NbRub->setText(QString::number(nb));
 return nb;
}

//--------------------------------- initComboListHistoric -------------------------------------------------------------
/*! \brief initialise le combo de la Rub List.
 *  \return nombre d'observations ajoutées.
 */
long  C_RubObservation::initComboListHistoric(void)
{return m_pCMoteurBase->initConboBoxWithRubList(m_pRubList, comboBox_RubName, QString::number(m_RubType));
}

//--------------------------------- comboBox_RubName_highlighted -------------------------------------------------------------
void C_RubObservation::comboBox_RubName_highlighted( int item )
{comboBox_RubName_activated(item);
}

//--------------------------------- comboBox_RubName_activated -------------------------------------------------------------
void C_RubObservation::comboBox_RubName_activated( int item)
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( item );
 if (pCPrtQListBoxItem == 0) return;
 int     id  =  pCPrtQListBoxItem->GetPk().toInt();

 QByteArray          data;
 //char                *ptr = 0;
 RUBREC_LIST::iterator it = m_pRubList->at(id);
 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_pMyEditText && m_pMyEditText->isModified() && m_LastRub != -1)
    {SaveTextInRubList();
    }
 //........................... retirer le texte de la nouvelle rubrique a afficher ..............................
 //
 int ret         = m_pCMoteurBase->GetDataFromRubList(data, it);
 // debug .....................
 // QString rubName = (*it).m_Libelle;
 // QString str_d = QString(data);
 //CGestIni::Param_UpdateToDisk("/home/ro/data.txt",str_d);
 int     f       = G_pCApp->decodeMedinTuxData( data, m_pMyEditText, ret);
 m_LastRub       = id;

 //........................ régler droits utilisateurs ......................................................................
 QString user              = (*it).m_User;
 QString doc_sign_user     = (*it).m_SignUser;

 if (G_pCApp->m_User != doc_sign_user)  // si l'utilisateur actuel n'est pas le même que le signataire du document vérifier les droits
    {QString friend_droits     = G_pCApp->m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  doc_sign_user , G_pCApp->m_User);
     if (G_pCApp->IsThisDroitExist(friend_droits, m_PrefixDroit+"v")) {                                                            pushButtonDelete->setEnabled(TRUE); pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate() );}
     else                                                             { m_pMyEditText->setText(G_pCApp->m_MessVisuDocNotPossible); pushButtonDelete->setEnabled(FALSE);pushButtonRubDateChange->setIconSet(Theme::getIconCadenas() );}
     if (G_pCApp->IsThisDroitExist(friend_droits, m_PrefixDroit+"c")) { m_pMyEditText->setReadOnly (FALSE );pushButtonDelete->setEnabled(TRUE);pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate() );}
     else                                                             { m_pMyEditText->setReadOnly (TRUE ); pushButtonDelete->setEnabled(FALSE);pushButtonRubDateChange->setIconSet(Theme::getIconCadenas() );}
     if (G_pCApp->IsThisDroitExist(friend_droits, m_PrefixDroit+"p")) { pushButtonPrint->setEnabled(TRUE);}
     else                                                             { pushButtonPrint->setEnabled(FALSE);}
    }
 else
    {m_pMyEditText->setReadOnly (FALSE );pushButtonDelete->setEnabled(TRUE); pushButtonPrint->setEnabled(TRUE);pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate() );
    }
 textLabel_UserName->setText(user + "::" + doc_sign_user);

 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted() ); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete() );    pushButtonDelete->setDown(FALSE);}
 if (m_pMyEditText!=0 && f==0) m_pMyEditText->setText("");
}

//--------------------------------- IfModified_SaveInRubList -------------------------------------------------------------
int C_RubObservation::IfModified_SaveInRubList()
{//........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_IsModifiable && m_pMyEditText && m_pMyEditText->isModified()  && m_LastRub != -1)
    {SaveTextInRubList();
     return TRUE;
    }
 return FALSE;
}

//--------------------------------- pushButtonDelete_clicked -------------------------------------------------------------
void C_RubObservation::pushButtonDelete_clicked()
{
 if (m_IsModifiable==0) return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem) return;
 int id  =  pCPrtQListBoxItem->GetPk().toInt();

 RUBREC_LIST::iterator it = m_pRubList->at(id);
 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if ((*it).m_State & RUB_IS_TO_DELETE)
    {m_pCMoteurBase->RubList_SetItemNotDeleted(m_pRubList, id);
    }
 else
    {m_pCMoteurBase->RubList_SetItemDeleted(m_pRubList, id);
    }
 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted() ); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete() );    pushButtonDelete->setDown(FALSE);}
}

//--------------------------------- pushButtonSave_clicked -------------------------------------------------------------
void C_RubObservation::pushButtonSave_clicked()
{if (m_IsModifiable) emit Sign_SaveButtonClicked();                                           // et voila
}

//--------------------------------- Current_RubList_ID -------------------------------------------------------------
int C_RubObservation::Current_RubList_ID()
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (pCPrtQListBoxItem) return pCPrtQListBoxItem->GetPk().toInt();
 return -1;
}

//--------------------------------- Current_RubList_Iterator -------------------------------------------------------------
RUBREC_LIST::iterator C_RubObservation::Current_RubList_Iterator()
{int id = Current_RubList_ID();
 if (id==-1) return  m_pRubList->end();
             return  m_pRubList->at(id);
}

//--------------------------------- SetCurentDocByPrimkey -------------------------------------------------------------
int C_RubObservation::SetCurentDocByPrimkey(const QString &primKey)
{QListBox *pQListBox = comboBox_RubName->listBox();
 int last            = comboBox_RubName->count ();
 int i;
 for (i=0; i<last; i++)
     {CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) pQListBox->item ( i );
     if (pCPrtQListBoxItem->GetUser() == primKey)
        {comboBox_RubName->setCurrentItem(i);
         comboBox_RubName_activated(i);
         return i;
        }
     }
 return -1; // pas trouvé
}

//--------------------------------- pushButtonRubDateChange_clicked -------------------------------------------------------------
void C_RubObservation::pushButtonRubDateChange_clicked()
{if (m_IsModifiable==0)                  return;
 if (m_pMyEditText->isReadOnly())        return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)                 return;

 RUBREC_LIST::iterator it   =  m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());
 DlgDateNom *dlg = new DlgDateNom(this,"DateNOm_Dlg",TRUE);        // TRUE Pour modal
 if (dlg ==0)                            return ;
 dlg->initDialog( &(*it).m_Date, &(*it).m_Libelle, &(*it).m_SubType);

 if ( dlg->exec() == QDialog::Accepted )
    { //................... remettre à jour le comboBox ................................
      pCPrtQListBoxItem->SetType((*it).m_Date);
      pCPrtQListBoxItem->SetDate((*it).m_Date);
      pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
      //................... mettre le document en modified (en replaçant les data ds rubrique liste) ......................
      if (m_pMyEditText && m_LastRub != -1)
         {SaveTextInRubList();
         }
    }
 delete dlg;
}

//--------------------------------- SetRubDateName -------------------------------------------------------------
void C_RubObservation::SetRubDateName( const QString &name, const QString &date )
{
 if (m_IsModifiable==0)                   return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)                  return;

 RUBREC_LIST::iterator it   =  m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());
 //................... remettre à jour les valeurs de la liste ................................
 if (name.length()<=0 && date.length()<19) return; //2004-03-10 19:21:19
 if (name.length()>0)   (*it).m_Libelle = name;
 if (date.length()==19) (*it).m_Date    = date;

 //................... remettre à jour le comboBox ................................
 pCPrtQListBoxItem->SetType((*it).m_Date);
 pCPrtQListBoxItem->SetDate((*it).m_Date);
 pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
 //................... mettre le document en modified (en replaçant les data ds rubrique liste) ......................
 if (m_pMyEditText && m_LastRub != -1)
    {SaveTextInRubList();
    }
}

//--------------------------------- GetRubDateName -------------------------------------------------------------
void C_RubObservation::GetRubDateName( QString &name, QString &date )
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)             return;

 RUBREC_LIST::iterator it = m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());
 date = (*it).m_Date;
 name = (*it).m_Libelle;
}

//--------------------------------- SaveTextInRubList -------------------------------------------------------------
/*! \brief Sauvegarde le contenu du document dans la RubList.
 *  \sa CMoteurBase::ReplaceDataInRubList
*/
void C_RubObservation::SaveTextInRubList()
{
 if (m_IsModifiable==0) return;
 m_pCMoteurBase->ReplaceDataInRubList(G_pCApp->interfaceToData(m_pMyEditText), m_pRubList, m_LastRub );
 m_pMyEditText->setModified (FALSE);
}



