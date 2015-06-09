/********************************* CMDI_Observation.cpp **************************
 *                                                                                *
 *   #include "CMDI_Observation.h"                                                *
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
#include <qfile.h>
#include <qfileinfo.h>
#include <qfiledialog.h>

#include "CMDI_Observation.h"
#include <qvbox.h>
#include <qmenubar.h>

#include "CApp.h"
#include "drtux.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------- CMDI_Observation -----------------------------------------------------------------------------
CMDI_Observation::CMDI_Observation( QWidget* parent, const char* rub_name, int wflags,  CMoteurBase *pCMoteurBase, RUBREC_LIST *pRubList,
                                    const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user, int type, QString prDroit)
    :CMDI_Generic( parent, rub_name, wflags , pCMoteurBase, pRubList, type) //: QMainWindow( parent, name, wflags )
{   SetType(type);
    SetPrintableState(TRUE);
    m_PrefixDroit = prDroit;
    //......................... creation du dialogue:  m_pC_RubObservation .............................
    m_pC_RubObservation  =    new C_RubObservation(pCMoteurBase, pRubList,
                                                  num_GUID, id_doss,  dossNom,  dossPrenom,  user,
                                                  this, rub_name , prDroit);

    m_CustomMenuRequestState = 0;

    if (m_pC_RubObservation)
       {//.................... creation de l'éditeur de texte: m_pC_RubObservation->m_pMyEditText...............................
        //                     à ratacher au membre: MyEditText *m_pC_RubObservation->m_pMyEditText de :  m_pC_RubObservation
        //                     afin d'en disposer à l'interieur de sa classe.
        //..................... connecter impression ......................................................
        connect( m_pC_RubObservation->pushButtonPrint,     SIGNAL( clicked()),   this,  SLOT  ( OnPrint())  );
        connect( m_pC_RubObservation->pushButtonAdd,       SIGNAL( clicked()),   this,  SLOT  ( On_pushButtonAppend())  );
        //.................. connecter les plugin de l'editeur de texte.............................................
        //                   au systeme de menu Custom

        connect( m_pC_RubObservation->m_pMyEditText,   SIGNAL( Sign_ExeCommand(QString&)),  this, SLOT  ( Slot_ExeCommand(QString&))
               );
        connect( m_pC_RubObservation->m_pMyEditText,   SIGNAL( Sign_Get_Custom_Menu(const QString &, QString&, QString &, QKeySequence &)),
                 this,                                 SLOT  ( Slot_Get_Custom_Menu(const QString &, QString&, QString &, QKeySequence &))
               );

       }
    
    setIcon(Theme::getIcon(QString("16x16/type_")+rub_name+".png") );
    setCentralWidget( m_pC_RubObservation );
}

//---------------------------------- ~CMDI_Observation -----------------------------------------------------------------------------
CMDI_Observation::~CMDI_Observation()
{QString rubName = GetRubName();
 emit Sign_RubIsDeleted(rubName);
 if (rubName==tr("Observation") && G_pCApp->m_pDrTux) G_pCApp->m_pDrTux->m_pCMDI_Observation = 0;
}

//---------------------------------- GetCurrent_RubList_ID -----------------------------------------------------------------------------
int     CMDI_Observation::GetCurrent_RubList_ID()
{IfModified_SaveInRubList();
 return m_pC_RubObservation->Current_RubList_ID();   // noter rubrique affichée;
}

//---------------------------------- Current_RubList_Iterator -----------------------------------------------------------------------------
RUBREC_LIST::iterator CMDI_Observation::Current_RubList_Iterator()
{return m_pC_RubObservation->Current_RubList_Iterator();
}

//----------------------------- reinitComboBoxWithRubList -----------------------------------
/*! \brief initialise la combo box avec les donnees de la rublist */
int  CMDI_Observation::reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay /*=""*/)
{ int index = 0;
  int nb = m_pCMoteurBase->initConboBoxWithRubList(pRubList, m_pC_RubObservation->comboBox_RubName, QString::number(GetType()), pkToDisplay, &index);
  if (nb<=0)
     {m_pC_RubObservation->hide();
     }
  else
     {m_pC_RubObservation->show();
     }
  if (index == -1) index =  nb-1;
  m_pC_RubObservation->m_LastRub = -1;
  m_pC_RubObservation->comboBox_RubName_activated(index);
  m_pC_RubObservation->comboBox_RubName->setCurrentItem ( index );
  m_pC_RubObservation->textLabel_NbRub->setText(QString::number(nb));
  return nb;
}

//----------------------------- GetDocListComboBox -----------------------------------
/*! \brief retourne la combobox affectee a al liste des documents */
QComboBox    *CMDI_Observation::GetDocListComboBox()
{return m_pC_RubObservation->comboBox_RubName;
}
//----------------------------- initData -----------------------------------
/*! \brief initialise la rubrique avec les données la concernant */
int  CMDI_Observation::initData()
{ m_pC_RubObservation->m_LastRub    = -1;   // tres tres important
  m_pC_RubObservation->initData();
  return 0;
}

//----------------------------- SetCurentDocByPrimkey -----------------------------------
/*! \brief initialise l'affichage sur le document désigné par const QString &pk */
int  CMDI_Observation::SetCurentDocByPrimkey(const QString &pk)
{return m_pC_RubObservation->SetCurentDocByPrimkey(pk);
}

//----------------------------- ChangeCurrentComboBoxItemName -----------------------------------
/*! \brief SLOT qui change le nom de l'item courant de la comboBox */
void CMDI_Observation::ChangeCurrentComboBoxItemName(const QString &name)
{if (name.length()==0)               return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) m_pC_RubObservation->comboBox_RubName->listBox()->item (  m_pC_RubObservation->comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)             return;
 pCPrtQListBoxItem->setText ( name );
}

//----------------------------- GetCurrentEditor -----------------------------------
/*! \brief retourne l'éditeur de texte courant avancé type MyEditText */
MyEditText  *CMDI_Observation::GetCurrentEditor()
{return m_pC_RubObservation->m_pMyEditText;
}

//---------------------------------- SetInterfaceOnDroits ----------------------------------------------------------------------------------------
/*! \brief positionne l'interface sur les droits */
void CMDI_Observation::SetInterfaceOnDroits(const QString &/*droits*/)
{m_pC_RubObservation->comboBox_RubName_activated( m_pC_RubObservation->comboBox_RubName->currentItem() );
}

//---------------------------------- setSaveButtonVisible -----------------------------------------------------------------------------
/*! \brief fonction qui met le botuon de sauvegarde visible */
void CMDI_Observation:: setSaveButtonVisible(int state )
{if (state==1)         m_pC_RubObservation->pushButtonSave->show();
 else if (state ==0)   m_pC_RubObservation->pushButtonSave->hide();
}

//---------------------------------- On_pushButtonAppend -----------------------------------------------------------------------------
/*! \brief fonction qui répond à l'appui du bouton de chargement d'une image */
void CMDI_Observation::On_pushButtonAppend()
{  if (G_pCApp->On_pushButtonAppend(this))
      {m_pC_RubObservation->pushButtonSave->show();
      }
}

//---------------------------------- DoActionWithThisDocument -----------------------------------------------------------------------------
/*! \brief fait avec le document donné en entrée une action dépendante du verbe  */
void   CMDI_Observation::DoActionWithThisDocument(const QString &verbe, int typ, const char *path /* =0 */, const QDateTime *pQDateTime /*=0 */, const QString *libelle /*=0 */, const QString *subType /*=0 */)
{G_pCApp->DoActionWithThisDocument(this, verbe, typ, path, pQDateTime, libelle, subType );
}

//---------------------------------- AddNewDocument -----------------------------------------------------------------------------
/*! \brief replace le document en cours en replaçant le contenu dans la liste des rubriques, et ajoute un nouveau document à la liste */
CRubRecord*   CMDI_Observation::AddNewDocument(const QString &strData_in, int typ, const char *path /*=0*/ , const QDateTime *pQDateTime /*=0*/, const QString *libelle /*=0*/, const QString *subType  /*=0*/, int noInquire /*=0*/)
{CRubRecord* pCRubRecord = G_pCApp->AddNewDocument(this, strData_in,  typ, path  , pQDateTime, libelle, subType, noInquire );
 if (pCRubRecord)
    {
     CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) G_pCApp->IsExistRubrique(pCRubRecord->m_Type.toInt());
     if (pCMDI_Generic==0) return pCRubRecord;
     //........................... reinitialiser l'interface .........................................................
     pCMDI_Generic->setSaveButtonVisible(TRUE);
     pCMDI_Generic->initData();
    }
 return pCRubRecord;
}

//---------------------------------- IfModified_SaveInRubList -----------------------------------------------------------------------------
void CMDI_Observation::IfModified_SaveInRubList()
{m_pC_RubObservation->IfModified_SaveInRubList();
}
//----------------------------------------- GetDefaultMaskFile --------------------------------
/*! \brief retourne le masque d'insertion par defaut des images
 *  \param path : chemin du masque d'insertion
*/

QString  CMDI_Observation::GetDefaultMaskFile()
{QString insertMask;
 if (G_pCApp->m_pDrTux==0) return QString::null;
 if (G_pCApp->m_pDrTux->m_pFormGlossaire == 0)
    {return G_pCApp->m_PathGlossaire+GetRubName()+"/STD.html";
    }
 if (insertMask.length()==0 ||  !QFile::exists( insertMask )) insertMask = G_pCApp->m_pDrTux->m_pFormGlossaire->GetCurrentFileName();
 return insertMask ;
}

//---------------------------------- OnPrint -----------------------------------------------------------------------------
void CMDI_Observation::OnPrint()
{m_pC_RubObservation->IfModified_SaveInRubList();
 emit Sign_Print();
}

//---------------------------------- RubOnUserChange -----------------------------------------------------------------------------
void CMDI_Observation::RubOnUserChange(const char*user, const char* /*sign_user*/)
{m_pC_RubObservation->m_User       = user;
}

//---------------------------------- OnIdentChange ----------------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Observation::OnIdentChange(const char* num_GUID,  const char* id_doss,
                                     const char* dossNom ,  const char* dossPrenom )
{ m_pC_RubObservation->m_NumGUID    = num_GUID;
  m_pC_RubObservation->m_ID_Doss    = id_doss;
  m_pC_RubObservation->m_DossNom    = dossNom;
  m_pC_RubObservation->m_DossPrenom = dossPrenom;
  m_pC_RubObservation->m_LastRub    = -1;          // tres tres important
  m_pC_RubObservation->initData();
}
//---------------------------------- SetZoom ----------------------------------------------------------------------------------------
void CMDI_Observation::SetZoom(int /*zoom*/)
{//m_pC_RubObservation->m_pMyEditText->SetZoom (zoom);
}
//---------------------------------- Slot_Get_Custom_Menu -----------------------------------------------------------------------------
void CMDI_Observation::Slot_Get_Custom_Menu(const QString &state, QString &menuIcon, QString &menuItem, QKeySequence &accel)
{if (state == "$StartRequest")    m_CustomMenuRequestState = 0;
 else                           ++m_CustomMenuRequestState;
 switch(m_CustomMenuRequestState)
   {case 0:
         menuIcon = G_pCApp->m_PathAppli + "Ressources/icones/backup_section.png";
         menuItem = tr("Enregi&strer comme document type");
         accel    = CTRL+Key_S;
         break;
    default:
         menuIcon = "";
         menuItem = "";
   }
}
//---------------------------------- Slot_ExeCommand -----------------------------------------------------------------------------
void CMDI_Observation::Slot_ExeCommand(QString &command)
{  RUBREC_LIST::iterator it = Current_RubList_Iterator();
   if (it ==  m_pC_RubObservation->m_pRubList->end() )  return;

   if (command==tr("Enregi&strer comme document type"))
      { QString   path    = G_pCApp->m_PathGlossaire + GetRubName();
        //int        typ    = (*it).m_Type.toInt();
        QString    ext    = ".html";
        QString    txt    = m_pC_RubObservation->m_pMyEditText->text();
        QString  filtre   = tr( " Fichiers HTML (*.htm *.html);;  Tous les fichiers (*)");
         //.................. selectionner le fichier .......................................
        QFileDialog fd (path,  filtre,  this, "SaveDlg" ) ;
        fd.setMode(QFileDialog::AnyFile );    // selection multiple
        if ( fd.exec() != QDialog::Accepted )           return;
        QString fn = fd.selectedFile();
        if ( fn.isEmpty() )                             return;
        fn         = QFileInfo(fn).dirPath() + "/" + QFileInfo(fn).baseName() + ext;
        QFile file( fn);
        if ( !file.open( IO_WriteOnly ) )               return;
        QTextStream ts( &file );
        CHtmlTools::ElimineAncresRedondantes(txt);
        CHtmlTools::setTextDefaultAtribut(txt, "text",          m_pC_RubObservation->m_pMyEditText->m_DefaultTextColor);
        CHtmlTools::setTextDefaultAtribut(txt, "bgcolor",       m_pC_RubObservation->m_pMyEditText->m_DefaultBackgroundColor);
        CHtmlTools::QRichTextToHtml(txt);
        ts << txt;
      }
}
//---------------------------------- setWordWrap ----------------------------------------------------------------------------------------
void CMDI_Observation::setWordWrap(int width)
{if (m_pC_RubObservation->m_pMyEditText==0) return;
 if (width == -1)
    {m_pC_RubObservation->m_pMyEditText->setWordWrap(QTextEdit::WidgetWidth);   //NoWrap, WidgetWidth, FixedPixelWidth, FixedColumnWidth
     //setWrapColumnOrWidth (793-37 );
    }
 else
    {m_pC_RubObservation->m_pMyEditText->setWordWrap (QTextEdit::FixedPixelWidth);
     m_pC_RubObservation->m_pMyEditText->setWrapColumnOrWidth (width );  //   (793-37 );   //( corresponds A4 width  21*72/2.54 )
    }
}
//---------------------------------- SetModifiableState -----------------------------------------------------------------------------
void CMDI_Observation::SetModifiableState(int state)
{m_pC_RubObservation->SetModifiableState(state);
}

//---------------------------------- setDataToModifiedState -----------------------------------------------------------------------------
/*! \brief place le document en etat modifié, afin qu'au prochain enregistrement elles soient mises à jour */
void CMDI_Observation::setDataToModifiedState()
{ if (m_pC_RubObservation->m_pMyEditText && m_pC_RubObservation->m_LastRub != -1)
     {m_pC_RubObservation->SaveTextInRubList();
     }
}
