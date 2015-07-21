//
// C++ Implementation:
/********************************* C_RubIdentite.cpp *****************************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: medintux@medintux.org

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

#include "C_RubIdentite.h"

#include <math.h>
#include <stdlib.h>

#include <qfiledialog.h>
#include <qclipboard.h>
#include <qdragobject.h>

#include <qvariant.h>
#include <math.h>
#include <qdatetime.h>
#include <qprocess.h>
#include <qtimer.h>
#include <qeventloop.h>
#include <../../MedinTuxTools/CMaskedLineEdit.h>

#include <qdragobject.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpoint.h>

#include "Global.h"
#include "CMDI_Ident.h"

#include "../../MedinTuxTools/Codecs/qmdcodec.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "CPreview.h"

#include "ui/DlgDateNom.h"
#include "C_Dlg_ImageProperty.h"
#include "../../MedinTuxTools/C_ImageList.h"
#include "../../MedinTuxTools/CHtmlTools.h"

#include "drtux.h"

#define ID_INTERV   4
#define QUAL_INTERV 3

#define LIST_NOM       0
#define LIST_PRENOM    1
#define LIST_ID_KEY    2
#define LIST_GUID      3

//======================================================== C_RubIdentite =======================================================
/*! \class C_RubIdentite
 *  \brief Classe C_RubIdentite.
*/



//----------------------------------------- C_RubIdentite --------------------------------
/*! \brief Constructeur de la classe. */
C_RubIdentite::C_RubIdentite(CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
                                const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user,
                                QWidget* parent /* = 0 */, const char* name /* = 0 */, WFlags fl /* = 0 */ )
    : FormRubIdent(parent,name,fl), m_pCMoteurBase(pCMoteurBase), m_pRubList(pRubList)
{m_IsModifiable  =  0;
 m_pMyEditText   =  0;
 m_LastRub       = -1;
 //m_MaxImageWidth = 600;
 //QString   val1  = "";
 //if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, CMDI_Documents::S_GetRubName(), "MaxImageWidth", &val1 )==0) m_MaxImageWidth =  val1.toInt(); // splash utilisé provisoirement


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

 pushButtonSave->setIconSet           (Theme::getIconSave());
 pushButtonPrint->setIconSet          (Theme::getIconPrint());
 pushButtonDelete->setIconSet         (Theme::getIconDelete());
 pushButtonRubDateChange->setIconSet  (Theme::getIconChangeDate());
 pushButtonAdd->setIconSet            (Theme::getIcon("22x22/fileopen.png") );
 pushButtonExporter->setIconSet       (Theme::getIcon("filesaveExport.png") );

 #ifdef Q_OS_MACX
    pushButtonAdd->setFlat (TRUE);
	pushButtonPrint->setFlat (TRUE);
    pushButtonSave->setFlat (TRUE);
    pushButtonPrint->setFlat (TRUE);
    pushButtonDelete->setFlat (TRUE);
    pushButtonRubDateChange->setFlat (TRUE);
    pushButtonExporter->setFlat (TRUE);

	textLabel_NbRub->setFlat (TRUE);
	textLabel_NbRub->setMinimumSize (50, 22);
	textLabel_NbRub->setFont(G_pCApp->font());
#endif


 //.................. on crée le splitter vertical qui contiendra ........................................
 //                   l'icon  view à gauche et le viewer de documments à droite
 //m_splitterVertical  = new QSplitter( tab_Documents, "splitter1" );
 //m_splitterVertical->setOrientation( QSplitter::Horizontal );

 //m_iconViewDocuments = new QIconView( m_splitterVertical, "iconViewDocuments" );

  //.................. recuperer la qualite des ayants droits......................
    QString param;
    CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "Ressources/QualiteBeneficiaire.tbl" , param);
    param = param.stripWhiteSpace();
    param = param.remove('\r');
    param = param.replace(',',' ');
    comboBoxQualiteAyantDroit->clear();
    QStringList lst = QStringList::split('\n', param);
    lst.remove(lst.first ());  // virer les lignes de commentaires
    lst.remove(lst.first ());
    comboBoxQualiteAyantDroit->insertStringList(lst);    //Qt::CaseSensitivity


    m_pMyEditText = new MyEditText( pCMoteurBase->m_DataBase, tab_Documents, "textEditDocuments" );
    m_pMyEditText->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)3, 0, 0, m_pMyEditText->sizePolicy().hasHeightForWidth() ) );
    tab_DocumentsLayout->addWidget( m_pMyEditText, 1, 0 );

 m_pMyEditText->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, m_pMyEditText->sizePolicy().hasHeightForWidth() ) );
 m_pMyEditText->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
 m_pMyEditText->SetTypeRubrique(CMDI_Ident::S_GetType());
 m_pMyEditText->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
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

   // signals and slots connections (vient de .cpp de designer)
 connect( pushButtonCreate,      SIGNAL( clicked() ),                     this, SLOT( pushButtonCreate_clicked() ) );
 connect( pushButtonSavModif,    SIGNAL( clicked() ),                     this, SLOT( pushButtonSavModif_clicked() ) );
 connect( pushButtonCancel,      SIGNAL( clicked() ),                     this, SLOT( pushButtonCancel_clicked() ) );
 connect( pushButton_AddMedTTT,  SIGNAL( clicked() ),                     this, SLOT( pushButton_AddMedTTT_clicked() ) );
 connect( pushButton_DelMedTTT,  SIGNAL( clicked() ),                     this, SLOT( pushButton_DelMedTTT_clicked() ) );
 //connect( pushButton_ViewNotes,  SIGNAL( clicked() ),                     this, SLOT( pushButton_ViewNotes_clicked() ) );
 //connect( checkBox_IsNotPatient, SIGNAL( clicked() ),                     this, SLOT( checkBox_IsNotPatient_clicked() ) );
 connect( listView_Praticiens,   SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( listView_Praticiens_doubleClicked(QListViewItem*) ) );
 connect( listView_Praticiens,   SIGNAL( contextMenuRequested(QListViewItem*,const QPoint&,int) ), this, SLOT( listView_Praticiens_contextMenuRequested(QListViewItem*,const QPoint&,int) ) );

 connect( textEdit_Note,                SIGNAL( textChanged() ),                     this, SLOT(   textEdit_Note_textChanged() ) );
 connect( comboBoxQualiteAyantDroit,    SIGNAL( activated (int ) ),           this, SLOT(   OnActivated_comboBoxQualiteAyantDroit( int)) );

 //........................ ajouter cela au dialogue principal .........................................
 //layout20->addWidget( m_pMyEditText, 1, 0 );
 //tab_DocumentsLayout->addMultiCellWidget( m_pMyEditText, 1, 1, 0, 9 );
 //frame_Editeur = new QFrame( tab_Documents, "frame_Editeur" );
 setDefaultWordWrapFromIniFile();

 connect( pushButtonSave,               SIGNAL( clicked()),         this,    SLOT  ( On_pushButtonSave())  );
 connect( pushButtonRubDateChange,      SIGNAL( clicked()),         this,    SLOT  ( pushButtonRubDateChange_clicked()) );
 connect( comboBox_RubName,             SIGNAL( highlighted(int) ), this,    SLOT  ( comboBox_RubName_highlighted(int) ) );
 connect( comboBox_RubName,             SIGNAL( activated(int) ),   this,    SLOT  ( comboBox_RubName_activated(int) ) );
 connect( pushButtonDelete,             SIGNAL( clicked() ),        this,    SLOT  ( pushButtonDelete_clicked() ) );

 pushButtonExporter->hide();

 //............................. charger les documents.....................................................................
 initData();

 pushButtonCreate->hide();
 pushButtonSavModif->show();  //void Sig_CarEvent(QEvent * ev);
 pushButtonCancel->show();

 //connect( lineEdit_Sexe,     SIGNAL( Sig_FocusOut ()),           this,     SLOT(   OnOutFocuslineEdit_DtNss())  );
 connect( comboBoxSexe->lineEdit(),    SIGNAL(lostFocus () ),      this, SLOT(   OnOutFocuslineEdit_DtNss()) );

 connect( lineEdit_DtNss,    SIGNAL( Sig_FocusOut ()),          this,     SLOT(   OnOutFocuslineEdit_DtNss())  );
 connect( lineEditVille,     SIGNAL( Sig_FocusOut ()),          this,     SLOT(   OnOutFocusVille())  );
 connect( lineEdit_CdPostal, SIGNAL( Sig_FocusOut()),           this,     SLOT(   OnOutFocusCodePostal()) );
 connect( lineEdit_CdPostal, SIGNAL( Sig_CarEvent(QEvent* )),   this,     SLOT(   OnCharEventCodePostal(QEvent* ))  );
 connect( lineEdit_NumSecu,  SIGNAL( Sig_CarEvent(QEvent* )),   this,     SLOT(   OnCharEventNumSecu(QEvent* ))   );
 connect( lineEdit_NumSecu,  SIGNAL( Sig_FocusIn()),            this,     SLOT(   OnInFocusNumSecu()) );
 connect( lineEdit_NumSecu,  SIGNAL( Sig_FocusOut()),           this,     SLOT(   OnOutFocusNumSecu()) );

 connect(textLabel_NbRub,    SIGNAL( pressed() ),               this,     SLOT( On_buttonDragClicked() ));

 m_Contacts_Run     = 0;
 m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 m_pThemePopup      = 0;
 SetInterfaceOnDroits(G_pCApp->m_Droits);
 lineEdit_NomDossier->setFocus();
 SetData();
}


//----------------------------------------- ~C_RubIdentite --------------------------------
C_RubIdentite::~C_RubIdentite()
{
 //ImageListClear();
 G_pCApp->m_C_ImageList.resetList();          // remet à zero la listes de references et enlève les images de la factory
 G_pCApp->m_C_ImageList.resetRef();
 //m_iconViewDocuments->clear();
}

/*$SPECIALIZATION$*/

//--------------------------------- On_buttonDragClicked ------------------------------------------------------
// connect(textLabel_NbRub, SIGNAL( clicked() ), this, SLOT( On_buttonDragClicked() ));
//   virtual void          On_buttonDragClicked();
void  C_RubIdentite::On_buttonDragClicked()
{   RUBREC_LIST::iterator it = Current_RubList_Iterator();
    if ( it != m_pRubList->end() ) (*it).dragRubrique(this);
}

//----------------------------------------- SetDefautMask --------------------------------
/*! \brief charge le masque d'insertion par defaut des images
 *  \param path : chemin du masque d'insertion
*/

void  C_RubIdentite::SetDefaultMaskFile(QString path)
{m_InsertMask = path;
}
/*$SPECIALIZATION$*/
/*
//--------------------------------- OnSignal_EditImage -------------------------------------------------------------
void C_RubIdentite::OnSignal_EditImage(QString &imageName)
{C_Dlg_ImageProperty* dlg = new C_Dlg_ImageProperty(m_pMyEditText, imageName, &G_pCApp->m_C_ImageList, this, "C_Dlg_ImageProperty");
 if (dlg==0) return;
 dlg->exec();
 delete dlg;
}
*/
//--------------------------------- pushButtonDelete_clicked -------------------------------------------------------------
void C_RubIdentite::pushButtonDelete_clicked()
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
 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted()); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete());    pushButtonDelete->setDown(FALSE);}
}

//----------------------------------------- CheckIfImageIsToRemovedFromIconView --------------------------------
/*! \brief SLOT verifie si une images est présente dans le bloc texte, ou près du curseur texte et si c'est le cas la retire de la liste et de l'iconView
 *  \param key : action au curseur  key == Qt::Key_Backspace (regarder l'image derrière) key== Qt::Key_Delete (regarder l'image devant) le curseur
*/

void C_RubIdentite::CheckIfImageIsToRemoveFromIconView(int key)
{//.................. recuperer position de depart .............................................
 QStringList    imageNameList;
 //QIconViewItem *pQIconViewItem = 0;
 QString        imgName        = "";

 GetImageListToDelete(imageNameList,  key);
 for ( QStringList::Iterator it = imageNameList.begin(); it != imageNameList.end(); ++it )
     {imgName = *it;
      if (G_pCApp->m_C_ImageList.removeFromList(imgName)==0)          // retourne le nb de reference restante à l'image
         { //if ( (pQIconViewItem = m_iconViewDocuments->findItem ( imgName, Qt::ExactMatch )) )                      // && refImageCount(m_pMyEditText->text(), imgName)<=1
           //   {delete pQIconViewItem;
           //   }
         }
     }
}

//----------------------------------------- refImageCount --------------------------------
/*! \brief compte le nombre de références à cette image dans le texte
*/
int C_RubIdentite::refImageCount(const QString &text, const QString &image)
{int deb_tag            = 0;          // chercher le premier tag image en partant du debut du bloc
 int end_tag            = 0;
 int     nb             = 0;
 QString           tag  = "";
 while ( (deb_tag  =  text.find("<img", end_tag)) != -1)
       {  end_tag  =  text.find('>',    deb_tag + 4);              // chercher fin du tag image
        if (end_tag ==-1)     return nb;                           // pas normal (tag non fermé)
        ++end_tag;                                                 // faire pointer après ">"
        tag = text.mid(deb_tag, end_tag-deb_tag);
        if (tag.find(image) != -1) ++nb;
        }
 return nb;
}

//----------------------------------------- refImageReinit --------------------------------
/*! \brief reinitialise le nombre de références de la liste d'images
*/
void C_RubIdentite::refImageReinit(const QString &qs_text)
{
 //................ effacer toutes les references aux images ...................
 G_pCApp->m_C_ImageList.resetRef();
 //............... reincrementer les réferences à  ces images ...................
 QString   imgName = "";
 long       from   = 0;
 long    deb_tag   = 0;
 long     lenText  = qs_text.length();
 while (from>=0 && from < lenText)
       {deb_tag     = CHtmlTools::IsThisTagImage(qs_text, from , &imgName );
        if (deb_tag>=0)  G_pCApp->m_C_ImageList.incRef(imgName);
       }

}
//----------------------------------------- On_Signal_Del_Before --------------------------------
/*! \brief SLOT verifie si une images est présente dans le bloc texte, ou près du curseur texte et si c'est le cas la retire de la liste et de l'iconView avant effacement
*/
void C_RubIdentite::On_Signal_Del_Before()
{
}

//----------------------------------------- On_Signal_Del_After --------------------------------
/*! \brief SLOT verifie si une images est présente dans le bloc texte, ou près du curseur texte et si c'est le cas la retire de la liste et de l'iconView avant effacement
*/
void C_RubIdentite::On_Signal_Del_After()
{//ReinitQIconViewOnText();
  refImageReinit(m_pMyEditText->text());
}

//----------------------------------------- ReinitQIconViewOnText --------------------------------
/*! \brief reinitialise l'iconView sur les images du texte
*/
/*
void C_RubIdentite::ReinitQIconViewOnText()
{m_iconViewDocuments->clear();
 //............. remettre toutes les references de la liste d'image sur les images du texte ...............
 refImageReinit(m_pMyEditText->text());

 //.............  parcourir la liste d'images et  ...............
 //               si une reference est trouvée afficher l'icone
 C_ImgInf *pC_ImgInf;
 QPixmap   retPixMap;
 QDictIterator <C_ImgInf> it( G_pCApp->m_C_ImageList.m_C_ImageRefDict );
 for( ; it.current(); ++it )
    {pC_ImgInf = it.current();
     if (pC_ImgInf && pC_ImgInf->m_NbRef > 0)
        {//G_pCApp->m_C_ImageList.getPixmap( pC_ImgInf->m_Name , retPixMap);
         //QImage qi = retPixMap.convertToImage().scale ( 60, 60 );
         //new QIconViewItem( m_iconViewDocuments, pC_ImgInf->m_Name, QPixmap (qi )  );
        }
    }
}
*/
//----------------------------------------- On_Signal_Paste --------------------------------
/*! \brief SLOT qui remet à jour l'iconView aprés un paste
*/

void C_RubIdentite::On_Signal_Paste_After()
{//ReinitQIconViewOnText();
 refImageReinit(m_pMyEditText->text());
}

//----------------------------------------- GetImageListToDelete --------------------------------
/*! \brief Fonction qui crée la liste d'images concernées par soit un bloc à effacer soit collées au curseur
 *  \param imageNameList : QStringList qui contiendra la liste des images concernées
 *  \param key : action au curseur  key == Qt::Key_Backspace (regarder l'image derrière) key== Qt::Key_Delete (regarder l'image devant) le curseur
*/

void C_RubIdentite::GetImageListToDelete(QStringList &imageNameList, int key)
{
 int ori_index;
 int ori_para;
 int deb_para;
 int deb_index;
 int old_deb_par;
 int old_deb_index;
 int old_end_par;
 int old_end_index;
 QString qs_text = "";
 QString imgName = "";
 //................. si positions non correctes ................................................
 //                  on utilise la position courante
 m_pMyEditText->getCursorPosition (&ori_para, &ori_index );

 //................. sauvegarder l'ancienne selection ..........................................
 m_pMyEditText->getSelection(&old_deb_par, &old_deb_index, &old_end_par, &old_end_index);
 // ................ si il y a une selection faire la liste des images contenues dans le bloc ......................
 if (old_deb_par != -1)
    {qs_text = m_pMyEditText->selectedText();
     if (qs_text.length())
        {long       from   = 0;
         long    deb_tag   = 0;
         while (from>=0 && from<(long)qs_text.length())
               {deb_tag     = CHtmlTools::IsThisTagImage(qs_text, from , &imgName );
                if (deb_tag>=0) imageNameList.append(imgName);
               } //end while (from>=0 && from<txt.length())
        }
    }
 else
    {//................. se positionner là où le boutton droit est appuyé ..........................
     m_pMyEditText->setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)

     //.................. faire bouger le curseur un coup en avant et arrière ...............
     //                   seul moyen pour que le curseur "prenne la fonte"
     //                   et de la noter avant de l'envoyer dans des zones inconnues
     m_pMyEditText->moveCursor (QTextEdit::MoveBackward);  //MoveBackward, MoveForward
     m_pMyEditText->moveCursor (QTextEdit::MoveForward);   //MoveBackward, MoveForward
     m_pMyEditText->m_LastFont  = m_pMyEditText->currentFont();            //relever la police de caractère à l'endroit de l'insertion
     m_pMyEditText->m_LastColor = m_pMyEditText->color();
     //.................. se mettre en debut ou fin de document .......................
     if   (key== Qt::Key_Delete) m_pMyEditText->moveCursor (QTextEdit::MoveEnd);             // chercher après le curseur
     else                        m_pMyEditText->moveCursor (QTextEdit::MoveHome);            // chercher avant le curseur
     //....................... recuperer la position de début de document .................................
     //                        la recherche se fait en reculant ou avançant à partir du curseur texte
     m_pMyEditText->getCursorPosition (&deb_para, &deb_index );
     m_pMyEditText->setSelection(deb_para, deb_index, ori_para, ori_index);
     qs_text = m_pMyEditText->selectedText().remove("<!--StartFragment-->"); // recuperer le texte selectionné et virer le tag rajouter par 'selectedText()'
     m_pMyEditText->removeSelection();                        // deselectionner
     m_pMyEditText->setCurrentFont(m_pMyEditText->m_LastFont);
     m_pMyEditText->setColor(m_pMyEditText->m_LastColor);
     m_pMyEditText->setCursorPosition (ori_para, ori_index );

     if   (key== Qt::Key_Delete)
          {imgName = CHtmlTools::checkIfImageAfter(qs_text);
          }
     else
          {imgName = CHtmlTools::checkIfImageBefore(qs_text);
          }
     if (imgName.length()) imageNameList.append(imgName);
     m_pMyEditText->setUpdatesEnabled( TRUE );
    }
}

//----------------------------------------- ImageListClear --------------------------------
void C_RubIdentite::ImageListClear()
{
}

//----------------------------------------- On_pushButtonSave --------------------------------
void C_RubIdentite::On_pushButtonSave()
{if (m_IsModifiable) emit Sign_SaveButtonClicked();
}

//--------------------------------- pushButtonSavModif_clicked -------------------------------------------------------------
void C_RubIdentite::pushButtonSavModif_clicked()
{if (m_IsModifiable==0) return;
 if (! G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"idc") ) return;
 QString nom       = lineEdit_NomDossier->text().stripWhiteSpace () ;
 QString prenom    = lineEdit_PremomDossier->text().stripWhiteSpace () ;
 QString numGUID   = textLabel_NumDoss->text();
 QString note      = textEdit_Note->text();

 if (nom == "")
    {nom = tr("ANONYME");
     lineEdit_NomDossier->setText(nom);
    }

 // typedef QPtrList<CIdentFieldData> PtrListIdentFieldData
 PtrListIdentFieldData list_field;   //  créer la liste: nom champs <-> valeurs
 list_field.setAutoDelete(TRUE);
 SetFieldListData(list_field);

 nom    = nom.upper();
 prenom = prenom.upper();
 m_pCMoteurBase->DossIdentIndexUpdate(m_ID_Doss, nom, prenom, &list_field );
 m_pCMoteurBase->PatientNoteWrite(m_ID_Doss ,numGUID,  note);
 emit Sign_IdentModified(numGUID,  m_ID_Doss,  nom, prenom   );
 emit Sign_SaveButtonClicked();
 emit Sign_Ident_KillMe();
}

//----------------------------- clearDocument -----------------------------------
/*! \brief Fonction efface le document en cours (images, liste des références d'images, l'iconview)
*/
void C_RubIdentite::clearDocument()
{m_pMyEditText->setText("");
 //G_pCApp->m_C_ImageList.resetList();          // remet à zero la listes de references et enlève les images de la factory (pas bon pour le copier coller)
 G_pCApp->m_C_ImageList.resetRef();
 //m_iconViewDocuments->clear();
}

//----------------------------- decodeStringTo_pQPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image...
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap *C_RubIdentite::decodeStringTo_pQPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return new QPixmap ( ba_dec );
}

//----------------------------- decodeStringTo_QPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image...
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap C_RubIdentite::decodeStringTo_QPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return QPixmap ( ba_dec );
}

//----------------------------- encodePixmapToBase64 -----------------------------------
/*! \brief Encode en base64 la QPixmap passée en paramètre */
QString  C_RubIdentite::encodePixmapToBase64(const QPixmap& pixIn)
{ QByteArray ba_enc;
  QBuffer buffer( ba_enc );
  buffer.open( IO_WriteOnly );
  pixIn.save( &buffer, "PNG" ); // writes pixmap into ba in PNG format
  return QString ( QCodecs::base64Encode( ba_enc , TRUE) );
}
//----------------------------- encodePixmapToBase64 -----------------------------------
/*! \brief Encode en base64 la QPixmap passée en paramètre */
QString  C_RubIdentite::encodePixmapToBase64(QPixmap* pQPixmap)
{ if (pQPixmap==0) return "";
  QByteArray ba_enc;
  QBuffer buffer( ba_enc );
  buffer.open( IO_WriteOnly );
  pQPixmap->save( &buffer, "PNG" ); // writes pixmap into ba in PNG format
  return QString ( QCodecs::base64Encode( ba_enc , TRUE) );
}

//----------------------------------------- C_RubIdentite --------------------------------
/*! \brief Retourne l'iterator (CRubRecord) de la RubList pour le document en cours. */
RUBREC_LIST::iterator C_RubIdentite::Current_RubList_Iterator()
{int id = Current_RubList_ID();
 if (id==-1) return  m_pRubList->end();
             return  m_pRubList->at(id);
}

//----------------------------------------- Current_RubList_ID --------------------------------
/*! \brief Retourne l'id dans la RubList du document en cours. */
int C_RubIdentite::Current_RubList_ID()
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (pCPrtQListBoxItem) return pCPrtQListBoxItem->GetPk().toInt();
 return -1;
}

//----------------------------------------- IfModified_SaveInRubList --------------------------------
/*! \brief Sauvegarde le contenu du MyEditText dans la liste des rubriques. */
int C_RubIdentite::IfModified_SaveInRubList()
{//........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_IsModifiable && m_pMyEditText && m_pMyEditText->isModified()  && m_LastRub != -1)
    {SaveTextInRubList();
     return TRUE;
    }
 return FALSE;
}

//--------------------------------- initComboListHistoric -------------------------------------------------------------
/*! \brief Met à jour le comboBox avec la RubList. */
long  C_RubIdentite::initComboListHistoric(void)
{return m_pCMoteurBase->initConboBoxWithRubList(m_pRubList, comboBox_RubName, QString::number(CMDI_Ident::S_GetType()));
}

//----------------------------------------- comboBox_RubName_activated --------------------------------
/*! \brief Met à jour l'affichage de MyEditText avec le contenu du document sélectionné. */
void C_RubIdentite::comboBox_RubName_activated( int item )
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
 int ret   = m_pCMoteurBase->GetDataFromRubList(data, it);
 int     f = G_pCApp->decodeMedinTuxData( data, m_pMyEditText, ret);
 m_LastRub = id;

 //........................ régler droits utilisateurs ......................................................................
 QString user              = (*it).m_User;
 QString doc_sign_user     = (*it).m_SignUser;

 if (G_pCApp->m_User != doc_sign_user)  // si l'utilisateur actuel n'est pas le même que le signataire du document vérifier les droits
    {QString friend_droits     = G_pCApp->m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  doc_sign_user , G_pCApp->m_User);
     if (G_pCApp->IsThisDroitExist(friend_droits, "iev")) {                                                            pushButtonDelete->setEnabled(TRUE); pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate());}
     else                                                 { m_pMyEditText->setText(G_pCApp->m_MessVisuDocNotPossible); pushButtonDelete->setEnabled(FALSE);pushButtonRubDateChange->setIconSet(Theme::getIconCadenas());pushButtonDelete->setEnabled(FALSE);}
     if (G_pCApp->IsThisDroitExist(friend_droits, "iec")) { m_pMyEditText->setReadOnly (FALSE );pushButtonAdd->setEnabled(TRUE); pushButtonDelete->setEnabled(TRUE); pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate());}
     else                                                 { m_pMyEditText->setReadOnly (TRUE ); pushButtonAdd->setEnabled(FALSE);pushButtonDelete->setEnabled(FALSE);pushButtonRubDateChange->setIconSet(Theme::getIconCadenas());}
     if (G_pCApp->IsThisDroitExist(friend_droits, "iep")) { pushButtonPrint->setEnabled(TRUE);}
     else                                                 { pushButtonPrint->setEnabled(FALSE);}
    }
 else
    {m_pMyEditText->setReadOnly (FALSE );pushButtonDelete->setEnabled(TRUE); pushButtonPrint->setEnabled(TRUE);pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate());
    }
 textLabel_UserName->setText(user + "::" + doc_sign_user);

 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted()); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete());    pushButtonDelete->setDown(FALSE);}
 if (m_pMyEditText!=0 && f==0) m_pMyEditText->setText("");
}

//----------------------------------------- comboBox_RubName_highlighted ------------
/*! \brief Slot appelé par le ComboBox lorsqu'un item est mis en Highlight. Affiche le contenu du document sélectionné. */
void C_RubIdentite::comboBox_RubName_highlighted( int item )
{comboBox_RubName_activated(item);
}

//----------------------------------------- initData --------------------------------
/*! \brief Met à jour le ComboBox et le texte. */
void C_RubIdentite::initData()
{
 QString qstr;
 if (m_pCMoteurBase==0) return;
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
 textLabel_NbRub->setText(qstr.setNum(nb));
}


//----------------------------------------- setDefaultWordWrapFromIniFile --------------------------------
void C_RubIdentite::setDefaultWordWrapFromIniFile()
{   //................................. un peu de geometrie ........................................
    QString val = "-1";
    if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Documents", "Largeur visualisation", &val) !=0 )  // zero = pas d'erreur
       {m_pMyEditText->setWordWrap(QTextEdit::WidgetWidth);   //NoWrap, WidgetWidth, FixedPixelWidth, FixedColumnWidth
       }
    else
       {m_pMyEditText->setWordWrap (QTextEdit::FixedPixelWidth);
        m_pMyEditText->setWrapColumnOrWidth ( val.toInt() );  //   (793-37 );   //( corresponds A4 width  21*72/2.54 )
       }
}


//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void C_RubIdentite::OnMenuActivated (int id)
{if (id != -1 && m_pThemePopup)
    {m_CurrentPopupItem = m_pThemePopup->text(id);
    }
}

//--------------------- SetCurentDocByPrimkey ------------------------------------------
/*! \brief Affiche le document dont la clé primaire est le primKey passé en paramètre. Utilisé pour lier le Dock_Menu à cette CMDI.
*/
int C_RubIdentite::SetCurentDocByPrimkey(const QString &primKey)
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

//--------------------------------- SaveTextInRubList--------------------------------
/*! \brief Sauvegarde le contenu du document dans la RubList.
 *  \sa CMoteurBase::ReplaceDataInRubList
*/
void C_RubIdentite::SaveTextInRubList()
{if (m_IsModifiable==0) return;
 m_pCMoteurBase->ReplaceDataInRubList(G_pCApp->interfaceToData(m_pMyEditText), m_pRubList, m_LastRub );
 m_pMyEditText->setModified (FALSE);
}

//--------------------------------- GetRubDateName -----------------------------------
/*! \brief Récupère la Date et le Libellé du document en cours. La date et le libellé sont sauvegardés dans les variables de la classe (date , name ).
*/
void C_RubIdentite::GetRubDateName( QString &name, QString &date )
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)             return;

 RUBREC_LIST::iterator it = m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());
 date = (*it).m_Date;
 name = (*it).m_Libelle;
}

//-------------------------- pushButtonRubDateChange_clicked ----------------------------------
/*! \brief Affiche le dialogue de modification du libelle du sous-type et de la date du document.
*/
void C_RubIdentite::pushButtonRubDateChange_clicked()
{if (m_IsModifiable==0)                  return;
 if (m_pMyEditText->isReadOnly())        return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)                 return;

 RUBREC_LIST::iterator it   =  m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());       // indice de liste
 DlgDateNom *dlg            =  new DlgDateNom(this,"DateNOm_Dlg",TRUE);                      // TRUE Pour modal
 if (dlg ==0)                            return ;
 dlg->initDialog( &(*it).m_Date, &(*it).m_Libelle, &(*it).m_SubType);

 if ( dlg->exec() == QDialog::Accepted )
    { //................... remettre à jour le comboBox ................................
//       pCPrtQListBoxItem->SetType((*it).m_Date);
      pCPrtQListBoxItem->SetDate((*it).m_Date);
      pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
      //................... mettre le document en modified (en replaçant les data ds rubrique liste) ......................
      if (m_pMyEditText && m_LastRub != -1)
         {SaveTextInRubList();
         }
    }
 delete dlg;
}

//----------------------------------------- SetRubDateName --------------------------------
/*! \brief définit les dates et libelles du document en cours. */
void C_RubIdentite::SetRubDateName( const QString & name, const QString & date )
{if (m_IsModifiable==0)                   return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)                  return;

 RUBREC_LIST::iterator it   =  m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());
 //................... remettre à jour les valeurs de la liste ................................
 if (name.length()<=0 && date.length()<19) return; //2004-03-10 19:21:19
 if (name.length()>0)   (*it).m_Libelle = name;
 if (date.length()==19) (*it).m_Date    = date;

 //................... remettre à jour le comboBox ................................
//  pCPrtQListBoxItem->SetType((*it).m_Date);
 pCPrtQListBoxItem->SetDate((*it).m_Date);
 pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
 //................... mettre le document en modified (en replaçant les data ds rubrique liste) ......................
 if (m_pMyEditText && m_LastRub != -1)
    {SaveTextInRubList();
    }
}


//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// indentite ui.h////////////////////////
//////////////////////////////////////////////////////////////////////////////
//--------------------------------- OnActivated_comboBoxQualiteAyantDroit ------------------------------------------------------
void  C_RubIdentite::OnActivated_comboBoxQualiteAyantDroit( int qualiteAssure)
{if (qualiteAssure ==0)
    {lineEdit_NomAssure->setEnabled(FALSE);
     lineEdit_PrenomAssure->setEnabled(FALSE);
     lineEdit_DtNss_Assure->setEnabled(FALSE);
    }
 else
    {lineEdit_NomAssure->setEnabled(TRUE);
     lineEdit_PrenomAssure->setEnabled(TRUE);
     lineEdit_DtNss_Assure->setEnabled(TRUE);
     if (lineEdit_NumSecu->text().length()==13)                              lineEdit_NomAssure->setFocus();
     if (lineEdit_NomAssure->text().stripWhiteSpace().length()==0)           lineEdit_NomAssure->setText(lineEdit_NomDossier->text());
     if (lineEdit_PrenomAssure->text().stripWhiteSpace().length()==0)        lineEdit_PrenomAssure->setText(lineEdit_PremomDossier->text());
    }
}

//--------------------------------- listView_Praticiens_contextMenuRequested -----------------------------------------------------------------------
void C_RubIdentite::listView_Praticiens_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int )
{if (pQListViewItem==0)            return;
 QStringList lst;
 QString removeProp = tr("Retirer la propriété 'En qualité de'");
 CGestIni::Param_GetList(G_pCApp->m_DrTuxParam, "Qualite intervenant", "",  lst );
 if (lst.count()==0)               return;
 lst.append ( "-------------" );
 lst.append ( removeProp );
 QString ret = G_pCApp->DoPopupList(lst);
 if (ret.length()==0)              return;
 if (ret ==removeProp) ret="";
 pQListViewItem->setText(QUAL_INTERV, ret);
 if (pushButtonCreate->isShown())  return;     // on se contente de renseigner uniquement la listview (l'enregistrement se fera apres click sur enregistrer)

 m_pCMoteurBase->PatientIntervenantsWrite( m_ID_Doss , m_NumGUID, pQListViewItem->text(ID_INTERV),  pQListViewItem->text(QUAL_INTERV));
}

//----------------------------------------- SetInterfaceOnDroits ------------------------------------------------------------------------
void C_RubIdentite::SetInterfaceOnDroits(const QString &droits)
{//....................... droits idv-idc-idg ...................................................................
 if (G_pCApp->IsThisDroitExist(droits,"idv"))
    {groupBox1->show();
     groupBox2->show();
     //tabWidgetNotes->show();
     //pushButton_ViewNotes->show();
     textLabelNumeroDoss->show();
     textLabel_NumDoss->show();
    }
 else
    {groupBox1->hide();
     groupBox2->hide();
     //tabWidgetNotes->hide();
     //pushButton_ViewNotes->hide();
     textLabelNumeroDoss->hide();
     textLabel_NumDoss->hide();
    }
 if (G_pCApp->IsThisDroitExist(droits,"idc"))
    {pushButtonSavModif->setEnabled(TRUE);
     pushButtonCreate->setEnabled(TRUE);
     pushButton_AddMedTTT->setEnabled(TRUE);
     pushButton_DelMedTTT->setEnabled(TRUE);
    }
 else
    {pushButtonSavModif->setEnabled(FALSE);
     pushButtonCreate->setEnabled(FALSE);
     pushButton_AddMedTTT->setEnabled(FALSE);
     pushButton_DelMedTTT->setEnabled(FALSE);
    }
}

//----------------------------------------- OnOutFocuslineEdit_DtNss --------------------------------------------------
/*! \brief ACTION: calcule l'age entre deux dates de début et de fin.
// ENTREE: QDate *pDebut: pointe sur une QDate de debut (exemple date de naissance)
//         QDate *pFin:   pointe sur une QDate de  fin (peut etre zero alors il sera pris la date en cours)
//         bool bTexte:   TRUE   alors la date sera exprimée comme: 4 ans 6 mois 3 jours
//                        FALSE  alors la date sera exprimée comme un chiffre
//         int bAbrege:   2  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        1  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        0  et alors les sorties seront detaillées avec si age<3 avec les mois et jours:
// QString CMoteurBase::CalculeDiffAge(QDate *pDebut, QDate *pFin, bool bTexte, int bAbrege)
*/
void C_RubIdentite::OnOutFocuslineEdit_DtNss()
{ // `HistN0_Date`  >=  "2001-05-19T11:09:40" AND `HistN0_Date`  <=  "2001-05-20T11:09:41";
 QString resolvToken("");
 QString        dtNss      = lineEdit_DtNss->text().mid(4)+lineEdit_DtNss->text().mid(2,2)+lineEdit_DtNss->text().mid(0,2);
 QDate        qDteNss(lineEdit_DtNss->text().mid(4,4).toInt(),
                      lineEdit_DtNss->text().mid(2,2).toInt(),
                      lineEdit_DtNss->text().mid(0,2).toInt()
                     );
 QString sAge = m_pCMoteurBase->CalculeDiffAge(&qDteNss, 0, TRUE, 0);
 //textLabel_Age->setText(sAge);
 int i = 0; while (sAge[i]>='0' && sAge[i]<='9') ++i; sAge.truncate(i);
 int              age      = sAge.toInt();

 if (age < 15)          resolvToken="L'enfant";
 if (age >= 15 && age <= 21)
    {if (comboBoxSexe->currentItem()==1)      resolvToken="Mademoiselle";
     else                                     resolvToken="Monsieur";
    }
 if (age>21)
    {if (comboBoxSexe->currentItem()==1)      resolvToken="Madame";
     else                                     resolvToken="Monsieur";
    }
 lineEdit_Titre->setText(resolvToken);
}

//--------------------------------- SetModifiableState -----------------------------------------------------------------------
void  C_RubIdentite::SetModifiableState(int state)
{m_IsModifiable = state;
 if (m_IsModifiable)
    {pushButtonSavModif->show();
     pushButtonSave->show();
     pushButton_AddMedTTT->show();
     pushButton_DelMedTTT->show();
     pushButtonRubDateChange->show();
     pushButtonDelete->show();
    }
 else
    {pushButtonSavModif->hide();
     pushButton_AddMedTTT->hide();
     pushButton_DelMedTTT->hide();
     pushButtonSave->hide();
     pushButtonRubDateChange->hide();
     pushButtonDelete->hide();
    }
}

//--------------------------------- OnInFocusNumSecu -----------------------------------------------------------------------
void  C_RubIdentite::OnInFocusNumSecu()
{QString numsecu = lineEdit_NumSecu->text();  //18105m_IsModifiable = 0;

 if (numsecu.length()==0)
    {if (comboBoxSexe->currentItem() != 1)  numsecu += "1";
     else                                   numsecu += "2";
     QString dtnss = lineEdit_DtNss->text();
     if (dtnss.length()==8)
         {numsecu += dtnss.mid(6);
          numsecu += dtnss.mid(2,2);
         }
     lineEdit_NumSecu->setText(numsecu);
    }
}

//--------------------------------- OnOutFocusNumSecu -------------------------------------------------------------
void  C_RubIdentite::OnOutFocusNumSecu()
{QString numsecu = lineEdit_NumSecu->text();  //18105
 if (numsecu.length()==13)
    {lineEdit_NssClef->setText(ComputeClef_Secu(numsecu));
    }
 else
    {lineEdit_NssClef->clear();
    }

 QString leftNumSecu("");
 if (comboBoxSexe->currentItem() != 1) leftNumSecu += "1";
 else                                  leftNumSecu += "2";
 QString dtnss = lineEdit_DtNss->text();
 if (dtnss.length()==8)
    {leftNumSecu += dtnss.mid(6);
     leftNumSecu += dtnss.mid(2,2);
    }
 if (numsecu.length()>=5 && leftNumSecu.length()>=5 && leftNumSecu != numsecu.left(5))
    {if (comboBoxQualiteAyantDroit->currentItem()==0) comboBoxQualiteAyantDroit->setCurrentItem(9);  // autre ayant droit
     lineEdit_NomAssure->setEnabled(TRUE);
     lineEdit_PrenomAssure->setEnabled(TRUE);
     lineEdit_DtNss_Assure->setEnabled(TRUE);
     if (lineEdit_NomAssure->text()=="")     lineEdit_NomAssure->setText(lineEdit_NomDossier->text());
     if (numsecu.length()==13)
        {lineEdit_NomAssure->setFocus();
         lineEdit_NomAssure->setCursorPosition (-1);
        }
    }
 else
    {comboBoxQualiteAyantDroit->setCurrentItem(0);
     lineEdit_NomAssure->setEnabled(FALSE);
     lineEdit_PrenomAssure->setEnabled(FALSE);
     lineEdit_DtNss_Assure->setEnabled(FALSE);
    }
}


//--------------------------------- OnOutFocusVille -----------------------------------------------------------------------
void  C_RubIdentite::OnOutFocusVille()
{if (m_pCMoteurBase==0) return;
 QString codePostal("");
 QString ville = lineEditVille->text().stripWhiteSpace();
 //.......... si pas de ville saisie on tente avec le code postal .........................
 if (ville.length()==0)
    {OnOutFocusCodePostal();
     return;
    }
 //............... chercher liste de codepostaux pouvant correspondre à cette ville .............
 QStringList codePostalList;
 m_pCMoteurBase->GetCodePostalFromVille( ville.replace("SAINT ","ST "),  codePostalList);
 if (codePostalList.findIndex ( lineEdit_CdPostal->text()+" "+lineEditVille->text() ) != -1 ) return;

 int nb = codePostalList.count();
 if (nb==1)
    {codePostal = codePostalList[0];
    }
 else if (nb>1 && nb<35 )
    {
     codePostal = DoPopupList(codePostalList, lineEditVille->x(), lineEditVille->y());
    }
 //............ separer la ville du code postal ...................................................
 int pos = codePostal.find(' ');
 if (pos != -1)
    {ville      = codePostal.mid(pos+1);
     codePostal = codePostal.left(pos);
    }
 if (//lineEdit_CdPostal->text().length()<5           ||                               // si code postal incomplet
     (codePostal.length()!=0 /*&& lineEdit_CdPostal->text() != codePostal */)            // ou non conforme avec celui de la ville
    )
    {lineEdit_CdPostal->setText(codePostal );                                        // placer celui de la ville
     lineEditVille->setText(ville);
     return;
    }

 //.............. si pas de code postal trouvé pour cette ville tenter de placer la ville du code postal .............
 if (codePostal.length()==0 )
    {ville = m_pCMoteurBase->GetVilleFromCodePostal(lineEdit_CdPostal->text());
     if (ville.length()!=0) lineEditVille->setText(ville);
    }
}

//--------------------------------- OnOutFocusCodePostal -------------------------------------------------------------
void  C_RubIdentite::OnOutFocusCodePostal()
{if (m_pCMoteurBase==0) return;
 QStringList villeList;
 if (lineEdit_CdPostal->text().length()==5)
    {m_pCMoteurBase->GetVilleFromCodePostal(lineEdit_CdPostal->text(), villeList);
     int nb = villeList.count();
     if (nb==1)
        {lineEditVille->setText(villeList[0]);
        }
     else if (nb>1 && nb<35 )
        {if (villeList.findIndex ( lineEditVille->text()) == -1 )  // si la ville ne correspond pas au code postal
            {QString ville = DoPopupList(villeList, lineEdit_CdPostal->x(), lineEdit_CdPostal->y());
             if (ville.length()>0) lineEditVille->setText(ville);
            }
        }
    }
 else
    {lineEditVille->clear();
    }
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString C_RubIdentite::DoPopupList(QStringList &list, int x ,int y)
{m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 if (m_pThemePopup) return m_RetPopupItem;
 m_pThemePopup      = new ThemePopup(0, "MyPopupMenu" );
 if (m_pThemePopup==0)       return m_RetPopupItem;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
    {m_pThemePopup->insertItem( *(it),    this, SLOT( GetPopupItem()),  CTRL+Key_L );
    }
 //................. connecter le menu .................................................................
 connect( m_pThemePopup, SIGNAL( activated( int)),
          this,          SLOT(OnMenuActivated( int))
        );

 m_pThemePopup->exec(mapToGlobal (QPoint ( x, y)));
 delete  m_pThemePopup;
 m_pThemePopup = 0;
 return  m_RetPopupItem;
}


//--------------------------------- GetPopupItem  -------------------------------------------------------------
void C_RubIdentite::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}

//--------------------------------- OnCharEventCodePostal -------------------------------------------------------------
void  C_RubIdentite::OnCharEventCodePostal(QEvent* )
{OnOutFocusCodePostal();
}

//--------------------------------- OnCharEventNumSecu-------------------------------------------------------------
void  C_RubIdentite::OnCharEventNumSecu(QEvent * )
{OnOutFocusNumSecu();
}

/*--------------------------------- ComputeClef_Secu ----------------------------*/
QString C_RubIdentite::ComputeClef_Secu(const char *txt)
{if (strlen(txt) !=13) return QString("  ");
 QString result (QString::number( 97 - fmod(atof(txt) , 97)));
 if (result.length()<2) result.prepend('0');
 return result;
}

//--------------------------------- SetData-------------------------------------------------------------
void C_RubIdentite::SetData()
{
 if (m_pCMoteurBase==0) return;
 pushButtonCreate->hide();
 if (m_IsModifiable) pushButtonSavModif->show();
 pushButtonCancel->show();
 if (DataToFormulaire (m_ID_Doss))
    {lineEdit_NomDossier->setText(m_DossNom);        // avec Nom    deja connu recupere dans la liste
     lineEdit_PremomDossier->setText(m_DossPrenom);  // avec Prenom deja connu recupere dans la liste
    }
 else
    {pushButtonSavModif->hide();
     ClearDialog();
    }
}
//--------------------------------- SetData-------------------------------------------------------------
void C_RubIdentite::SetData(int flag)
{C_RubIdentite::SetData();
 if (flag & NEW_IDENTITE)
    {ClearDialog();
     textLabel_NumDoss->setText(m_pCMoteurBase->GUID_Create());
     pushButtonCreate->show();
     pushButtonSavModif->hide();
     pushButtonCancel->show();
    }
}

//--------------------------------- ClearDialog -----------------------------------------------------------------
void C_RubIdentite::ClearDialog()
{    lineEdit_NomAssure->clear();
     lineEdit_DtNss_Assure->clear();
     lineEdit_NomDeNss->clear();
     lineEdit_PrenomAssure->clear();
     lineEdit_Titre->clear();
     textEdit_Adresse->clear();
     lineEdit_Tel1->clear();
     lineEdit_Tel2->clear();
     lineEdit_Tel3->clear();
     lineEdit_CdPostal->clear();
     lineEditVille->clear();
     //lineEdit_Sexe->clear();
     comboBoxSexe->setCurrentItem(2);  // indetermine
     textLabel_NumDoss->clear();
     lineEdit_Email->clear();
     lineEdit_NomDossier->clear();
     lineEdit_PremomDossier->clear();
     lineEdit_Profession->clear();
     QDateTime  dt ;
     dt.setTime_t(0);
     //....................... trucs speciaux ........................................
     QDate qDteNss =  dt.date () ;
     lineEdit_DtNss->setText(qDteNss.toString(QString("ddMMyyyy")));
     lineEdit_NumSecu->clear();
     lineEdit_NssClef->clear();
     textEdit_Note->clear();
     listView_Praticiens->clear();
}

//--------------------------------- DataToFormulaire -------------------------------------------------------------
int C_RubIdentite::DataToFormulaire( QString &refToPrimkeyDoss )
{/*
   QString      m_DOSS_IDENT_GUID;        // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_IDENT_NSS ;        // date de naissance
   QString      m_DOSS_IDENT_SEX;         // sexe
   QString      m_DOSS_IDENT_JFNOM;       // nom de jeune fille
   QString      m_DOSS_IDENT_NB_ENF;      // nombre d'enfants
   QString      m_DOSS_IDENT_RUE;         // adresse
   QString      m_DOSS_IDENT_CODE_POST;   // Code postal
   QString      m_DOSS_IDENT_VILLE;       // Ville
   QString      m_DOSS_IDENT_CEDEX;       // cedex
   QString      m_DOSS_IDENT_PAYS;        // pays
   QString      m_DOSS_IDENT_TEL1;        // Telephone 1
   QString      m_DOSS_IDENT_TEL2;        // Telephone 2
   QString      m_DOSS_IDENT_TEL3;        // Telephone 3
   QString      m_DOSS_IDENT_TEL1_TYP;    // Telephone 1  type (parent amis etc...)
   QString      m_DOSS_IDENT_TEL2_TYP;    // Telephone 2
   QString      m_DOSS_IDENT_TEL3_TYP;    // Telephone 3
   QString      m_DOSS_IDENT_NUMSS;       // Numéro de secu
   QString      m_DOSS_IDENT_EMAIL;       // Numéro de secu
   QString      m_DOSS_IDENT_IS_ASS;      // le patient est il l'assuré
   QString      m_DOSS_IDENT_NOM_ASS;     // nom de l'assuré
   QString      m_DOSS_IDENT_PRENOM_ASS;  // prénom de l'assuré
   QString      m_DOSS_IDENT_PROF;        // profession
   QString      m_DOSS_IDENT_TITRE;       // Titre( Dr Mr Md Pr ...)
   QString      m_DOSS_IDENT_REF_PK;      // reference à la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX
*/
 //............................... recuperer les notes ............................................
 QString data;
 m_pCMoteurBase->PatientNoteGetData(refToPrimkeyDoss ,  data);
 textEdit_Note->setText(data);
 m_pCMoteurBase->PatientIntervenantsGetData(refToPrimkeyDoss , listView_Praticiens);
 //............... chaines d'erreurs  ..................................................
 if  (m_pCMoteurBase->OpenBase()==0) return 0;

  //............... crer un curseur SQL ..................................................
    QString requete  = QString( "SELECT ");
            requete += m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS          + ", " +       // 0
                       m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS       + ", " +       // 1
                       m_pCMoteurBase->m_DOSS_IDENT_RUE              + ", " +       // 2
                       m_pCMoteurBase->m_DOSS_IDENT_TEL1             + ", " +       // 3
                       m_pCMoteurBase->m_DOSS_IDENT_TEL2             + ", " +       // 4
                       m_pCMoteurBase->m_DOSS_IDENT_TEL3             + ", " +       // 5
                       m_pCMoteurBase->m_DOSS_IDENT_EMAIL            + ", " +       // 6
                       m_pCMoteurBase->m_DOSS_IDENT_CODE_POST        + ", " +       // 7
                       m_pCMoteurBase->m_DOSS_IDENT_VILLE            + ", " +       // 8
                       m_pCMoteurBase->m_DOSS_IDENT_SEX              + ", " +       // 9
                       m_pCMoteurBase->m_DOSS_IDENT_GUID             + ", " +       // 10
                       m_pCMoteurBase->m_DOSS_IDENT_JFNOM            + ", " +       // 11
                       m_pCMoteurBase->m_DOSS_IDENT_TITRE            + ", " +       // 12
                       m_pCMoteurBase->m_DOSS_IDENT_PROF             + ", " +       // 13
                       m_pCMoteurBase->m_DOSS_IDENT_NSS              + ", " +       // 14
                       m_pCMoteurBase->m_DOSS_IDENT_NUMSS            + ", " +       // 15
                       m_pCMoteurBase->m_DOSS_IDENT_IS_ASS           + ", " +       // 16
                       m_pCMoteurBase->m_DOSS_IDENT_NSS_ASS          + ", " +       // 17
                       m_pCMoteurBase->m_DOSS_IDENT_GEME             + " FROM "  +  // 18
                       m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME         + " WHERE " +
                       m_pCMoteurBase->m_DOSS_IDENT_REF_PK           + " = '"    + refToPrimkeyDoss + "';";

    QSqlQuery query( requete  , m_pCMoteurBase->m_DataBase );                     // ouvrir un curseur sur la table
    if ( query.isActive() && query.next())
       {lineEdit_NomAssure->setText(     CGestIni::Utf8_Query(query , 0)      );
        lineEdit_PrenomAssure->setText(  CGestIni::Utf8_Query(query , 1)      );

        textEdit_Adresse->setText(       CGestIni::Utf8_Query(query , 2)      );
        lineEdit_Tel1->setText(          ToNum(query.value( 3).toString())    );
        lineEdit_Tel2->setText(          ToNum(query.value( 4).toString())    );
        lineEdit_Tel3->setText(          ToNum(query.value( 5).toString())    );
        lineEdit_Email->setText(         CGestIni::Utf8_Query(query , 6)      );
        lineEdit_CdPostal->setText(      ToNum(query.value( 7).toString())    );
        lineEditVille->setText(          CGestIni::Utf8_Query(query , 8)      );
        //............................ sexe ...........................................
        QString sexe                       =    CGestIni::Utf8_Query(query , 9)    ;
        if       (sexe.left(1).lower() == "m")  comboBoxSexe->setCurrentItem ( 0 ) ;
        else  if (sexe.left(1).lower() == "f")  comboBoxSexe->setCurrentItem ( 1 ) ;
        else                                    comboBoxSexe->setCurrentItem ( 2 ) ;
        lineEdit_RangGeme->setText(        query.value( 18).toString()          );

        textLabel_NumDoss->setText(      CGestIni::Utf8_Query(query , 10)     );
        lineEdit_NomDeNss->setText(      CGestIni::Utf8_Query(query , 11)     );
        lineEdit_Titre->setText(         CGestIni::Utf8_Query(query , 12)     );
        lineEdit_Profession->setText(    CGestIni::Utf8_Query(query , 13)     );
        //.............. qualite assure ..........................
        int qualiteAssure = query.value( 16).toInt()-1;   // 0 l'assuré est le patient  (1 moins 1)
        if (qualiteAssure < 0) qualiteAssure = 9;
        if (qualiteAssure > 9) qualiteAssure = 9;
        comboBoxQualiteAyantDroit->setCurrentItem(qualiteAssure);
        OnActivated_comboBoxQualiteAyantDroit(qualiteAssure);

        //....................... trucs speciaux ........................................
        //QDate qDteNss =                  query.value( 14).toDate();    // dnss patient
        lineEdit_DtNss->setText(CHtmlTools::yyyyMMdd_to_ddMMyyyy(query.value( 14).toString()));

        //qDteNss =                  query.value( 17).toDate();          // dnss assure
        lineEdit_DtNss_Assure->setText(CHtmlTools::yyyyMMdd_to_ddMMyyyy(query.value( 17).toString()));

        //QDate qDteNss =                 query.value( 14).toDate();
        //lineEdit_DtNss->setText(qDteNss.toString(QString("ddMMyyyy")));

        QString   nss =                 query.value( 15).toString();
        lineEdit_NumSecu->setText( nss.left(13)  );
        lineEdit_NssClef->setText( nss.mid(13)   );

        m_pCMoteurBase->CloseBase();
       return 1;
    }
else
    {
     m_pCMoteurBase->OutSQL_error(query);
     m_pCMoteurBase->CloseBase();
     return 0;
    }
}

//--------------------------------- ToNum ---------------------------------------------------------------------------------
QString C_RubIdentite::ToNum(QString in)
{for(uint i=0; i<in.length(); )
    {if (in.at(i)<'0' || in.at(i)>'9') in.remove(i,1);
     else ++i;
    }
 return in;
}

//--------------------------------- pushButtonCreate_clicked -------------------------------------------------------------
void C_RubIdentite::pushButtonCreate_clicked()
{if (! G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"idc") ) return;
 QString nom       = lineEdit_NomDossier->text().stripWhiteSpace () ;
 QString prenom    = lineEdit_PremomDossier->text().stripWhiteSpace () ;
 QString numGUID   = textLabel_NumDoss->text();
 if (nom == "") nom = tr("ANONYME");


 PtrListIdentFieldData list_field;    //  créer la liste: nom champs <-> valeurs
 list_field.setAutoDelete(TRUE);
 SetFieldListData(list_field);


 m_ID_Doss = m_pCMoteurBase->DossIdentIndexCreate(nom, prenom, numGUID,  &list_field );
 pushButtonCreate->hide();
 if (m_IsModifiable) pushButtonSavModif->show();
 pushButtonCancel->show();
 if (m_ID_Doss != "")
    {//.......................... ecrire la note ............................................
     nom    = nom.upper();
     prenom = prenom.upper();
     m_pCMoteurBase->PatientNoteWrite(m_ID_Doss ,numGUID,  textEdit_Note->text());
     //...................... ecrire les intervenants ................................................................
     QListViewItemIterator it( listView_Praticiens );
     while ( it.current() )
           {QListViewItem *item = it.current();
            ++it;
            m_pCMoteurBase->PatientIntervenantsWrite( m_ID_Doss , numGUID,  item->text(ID_INTERV),  item->text(QUAL_INTERV));
           }
     //.......................... faire part au Dr Tux du changement d'identité ............................................
     emit  Sign_PatientListMustBeUpdated(numGUID,        m_ID_Doss,      nom,            prenom  );
     emit  Sign_IdentChange(             numGUID,        m_ID_Doss,      nom,            prenom  );
    }
 emit Sign_Ident_KillMe();
}


//--------------------------------- SetFieldListData ------------------------------------------------------------------------
void C_RubIdentite::SetFieldListData(PtrListIdentFieldData &list_field)
{list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS,    lineEdit_NomAssure->text().stripWhiteSpace(),          FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS, lineEdit_PrenomAssure->text().stripWhiteSpace(),       FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_JFNOM,      lineEdit_NomDeNss->text().stripWhiteSpace(),           FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TITRE,      lineEdit_Titre->text().stripWhiteSpace(),              FTN_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_RUE,        textEdit_Adresse->text(),            FTN_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TEL1,       lineEdit_Tel1->text(),               FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TEL2,       lineEdit_Tel2->text(),               FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_TEL3,       lineEdit_Tel3->text(),               FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_EMAIL,      lineEdit_Email->text(),              FTN_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_CODE_POST,  lineEdit_CdPostal->text(),           FTN_INTEGER));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_VILLE,      lineEditVille->text(),               FTN_MAJ_STRING));
 QString sexe;
 int i = comboBoxSexe->currentItem();
 if (i == 0)        sexe = "M";
 else  if (i == 1)  sexe = "F";
 else               sexe = "I";
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_SEX,        sexe,                                FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NUMSS,      lineEdit_NumSecu->text()+
                                                                                 lineEdit_NssClef->text(),            FTN_NUM_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NSS,        CHtmlTools::ddMMyyyy_to_yyyyMMdd(lineEdit_DtNss->text(),"-")        + " 00:00:00", FTN_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_NSS_ASS,    CHtmlTools::ddMMyyyy_to_yyyyMMdd(lineEdit_DtNss_Assure->text(),"-") + " 00:00:00", FTN_STRING));

 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_IS_ASS,     QString::number(comboBoxQualiteAyantDroit->currentItem()+1),  FTN_INTEGER));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_PROF,       lineEdit_Profession->text(),         FTN_MAJ_STRING));
 list_field.append( new CIdentFieldData(m_pCMoteurBase->m_DOSS_IDENT_GEME,       lineEdit_RangGeme->text(),           FTN_MAJ_STRING));
}

//--------------------------------- pushButtonCancel_clicked -------------------------------------------------------------
void C_RubIdentite::pushButtonCancel_clicked()
{emit Sign_Ident_KillMe();
}

//--------------------------------- listView_Praticiens_doubleClicked -------------------------------------------------------------
void C_RubIdentite::listView_Praticiens_doubleClicked( QListViewItem *pQListViewItem )
{QString idInterv = "";
 if (pQListViewItem) idInterv = pQListViewItem->text(ID_INTERV);
 exeAnnuaire(idInterv);
}
//--------------------------------- pushButton_AddMedTTT_clicked -------------------------------------------------------------
void C_RubIdentite::pushButton_AddMedTTT_clicked()
{QString idInterv = "";
 //............ recuperer ID praticien en cours .......................................
 QListViewItem *pQListViewItem = listView_Praticiens-> selectedItem ();
 if (pQListViewItem) idInterv = pQListViewItem->text(ID_INTERV);
 exeAnnuaire( idInterv );

}
//--------------------------------- exeAnnuaire -------------------------------------------------------------
void C_RubIdentite::exeAnnuaire(QString idInterv )
{/*
 QString pathExe, pathExch;
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Gestion des contacts", "PathExe",     &pathExe) !=0)             return;         // path editeur de texte non défini
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Repertoire Temporaire", "Repertoire", &pathExch)!=0)             return;         // path editeur de texte non défini
 if ( QDir::isRelativePath ( pathExch ) )   pathExch = QDir::cleanDirPath (pathExch.prepend(G_pCApp->m_PathAppli) );
 //if ( QDir::isRelativePath (pathExe ) )   pathExe  = QDir::cleanDirPath (pathExe.prepend(G_pCApp->m_PathAppli) );
 */
 if (! G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"idc") ) return;
 QString pathExe  = CGestIni::Construct_Name_Exe("personnes",QFileInfo (qApp->argv()[0]).dirPath (true));
 QString pathExch;


 // path editeur de texte non défini
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Repertoire Temporaire", "Repertoire", &pathExch)!=0)             return;         // path editeur de texte non défini
 // Adaptation à la plateforme
 pathExch = CGestIni::Construct_PathBin_Module("Manager",QFileInfo (qApp->argv()[0]).dirPath (true)) + pathExch;

 QProcess*  proc = new QProcess( this );
 if (proc==0) return;
 QString exchFile = QDir::cleanDirPath (pathExch + QDir::separator() +  "GetPersonnne" + m_NumGUID + ".ech");

 QFile file( exchFile );
 if (file.exists( exchFile )) file.remove();

 proc->addArgument( pathExe);
 proc->addArgument( exchFile );
 proc->addArgument( "Record");
 proc->addArgument( idInterv);
 proc->addArgument( G_pCApp->m_Droits);
 // long tempo = 0;
 if (m_Contacts_Run==FALSE && proc->start() )
    {m_Contacts_Run = TRUE;
     QTimer timer;               // pour etre sur qu'un evenement se produise
     timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
     qApp->processEvents ();
     while(proc->isRunning () )
           {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     m_Contacts_Run = FALSE;
     //......... recuperer fichier d'echange .............
     if ( !file.open( IO_ReadOnly ) ) return;
     file.close();
     QString ret="";
     CGestIni::Param_UpdateFromDisk(exchFile, ret);    // 281|ANTOINE|Christine|Viscéral
     QStringList lst = QStringList::split ('|', ret,TRUE );
     QString numGUID = textLabel_NumDoss->text();

     if (pushButtonCreate->isShown()) new QListViewItem( listView_Praticiens, lst[3], lst[1], lst[2], "", lst[0] );
     else if (m_pCMoteurBase->PatientIntervenantsWrite( m_ID_Doss , numGUID,  lst[0], "") != 0)
        {new QListViewItem( listView_Praticiens, lst[3], lst[1], lst[2], "", lst[0] );
        }
    }
 if (file.exists( exchFile )) file.remove();
 delete proc;
}

//--------------------------------- pushButton_DelMedTTT_clicked -------------------------------------------------------------
void C_RubIdentite::pushButton_DelMedTTT_clicked()
{if (! G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"idc") ) return;
 QListViewItem* pQListViewItem = listView_Praticiens->currentItem ();
 if (pQListViewItem==0) return;
 if ( !(pushButtonCreate->isShown()) ) m_pCMoteurBase->PatientIntervenantsDelete( pQListViewItem->text(4));
 delete pQListViewItem;
}

//--------------------------------- pushButton_ViewNotes_clicked -------------------------------------------------------------
void C_RubIdentite::pushButton_ViewNotes_clicked()
{if (m_NoteViewState == FALSE)
    { m_NoteViewState = TRUE;
      //pushButton_ViewNotes->setIconSet (m_IconIsNoteShow);
      //tabWidgetNotes->show();
    }
 else
    {m_NoteViewState = FALSE;
     //pushButton_ViewNotes->setIconSet (m_IconIsNoteHide);
     //tabWidgetNotes->hide();
    }
}

//--------------------------------- synchronizeNoteWith -------------------------------------------------------------
void C_RubIdentite::synchronizeNoteWith( const char *txt )
{ textEdit_Note->setText( txt );
}

//--------------------------------- textEdit_Note_textChanged -------------------------------------------------------------
void C_RubIdentite::textEdit_Note_textChanged()
{ emit Sign_NoteToBeSynchronized( 1, textEdit_Note->text() );
}
