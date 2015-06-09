/********************************* CMDI_Ident.cpp ********************************
 *                                                                                *
 *   #include "CMDI_Ident.h"                                                      *
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
#include <qvbox.h>
#include <qtabwidget.h>

#include "CMDI_Ident.h"
#include "CApp.h"
#include "drtux.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/Codecs/qmdcodec.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Ident ============================================================================



//---------------------------------- CMDI_Ident --------------------------------------------------------------------------------
CMDI_Ident::CMDI_Ident( QWidget* parent, const char* name, int wflags,
                        CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
                        const char* num_GUID, const char* id_doss,
                        const char* dossNom, const char* dossPrenom, const char* user, int type)
   :CMDI_Generic( parent, name, wflags , pCMoteurBase, pRubList, type) // : QMainWindow( parent, name, wflags )
{   SetType(TYP_IDENTITE);
    m_PrefixDroit = "ie";
    SetPrintableState(TRUE);
    QVBox *pQVCentral = new QVBox( this );
    m_pFormRubIdent = new C_RubIdentite( pCMoteurBase, pRubList,
                                         num_GUID, id_doss,  dossNom,  dossPrenom,  user,
                                         pQVCentral, QString("Form")+name);
    if (m_pFormRubIdent)
       {      //..................... connecter impression ......................................................
         connect( m_pFormRubIdent->pushButtonPrint,      SIGNAL( clicked()),   this,    SLOT  ( OnPrint())  );
         connect( m_pFormRubIdent->pushButtonAdd,        SIGNAL( clicked()),   this,    SLOT  ( On_pushButtonAppend())  );

       }
    setIcon(Theme::getIcon("16x16/type_ident.png"));
    setFocusProxy( m_pFormRubIdent );
    setCentralWidget( pQVCentral );
}

//---------------------------------- ~CMDI_Ident -------------------------------------------------------------------------------
CMDI_Ident::~CMDI_Ident()
{emit Sign_RubIsDeleted(CMDI_Ident::S_GetRubName());
 if (G_pCApp->m_pDrTux) G_pCApp->m_pDrTux->m_pCMDI_Ident = 0;
}
//---------------------------------- setSaveButtonVisible -----------------------------------------------------------------------------
/*! \brief fonction qui met le botuon de sauvegarde visible */
void CMDI_Ident:: setSaveButtonVisible(int state )
{if (state==1)         m_pFormRubIdent->pushButtonSave->show();
 else if (state ==0)   m_pFormRubIdent->pushButtonSave->hide();
}

//---------------------------------- ActiverRubrique -----------------------------------------------------------------------------
/*! \brief SLOT appelé pour activer la rubrique si mode = 0 on montre la rubrique telle quelle si 1 onglet administatratif (0), 2 onglet document(1). */
void CMDI_Ident::ActiverRubrique(const char *rubName, int mode)
{ if (rubName != m_RubName) return;
  switch (mode)
     {case 0: showNormal(); return;
      default: m_pFormRubIdent->tabWidget_Ident->setCurrentPage (mode-1); showNormal(); return;
     }
}

//---------------------------------- RubOnUserChange -----------------------------------------------------------------------------
/*! \brief SLOT appelé pour modifier l'utilisateur en cours de la fenêtre CMDI. */
void CMDI_Ident::RubOnUserChange(const char*user, const char* /*sign_user*/)
{ m_pFormRubIdent->m_User       = user;
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
/*! \brief Ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI qui doit donc adapter son contenu à un nouveau malade.*/
void CMDI_Ident::OnIdentChange(const char* num_GUID,  const char* id_doss,
                               const char* dossNom ,  const char* dossPrenom )
{ m_pFormRubIdent->m_NumGUID    = num_GUID;
  m_pFormRubIdent->m_ID_Doss    = id_doss;
  m_pFormRubIdent->m_DossNom    = dossNom;
  m_pFormRubIdent->m_DossPrenom = dossPrenom;
  m_pFormRubIdent->SetData();
  m_pFormRubIdent->initData();
}

//---------------------------------- Slot_OnGlossaireFileClicked -----------------------------------------------------------------------------
/*! \brief SLOT qui répond au click d'un fichier du glossaire cela sert à mettre à jour le nom du masque par defaut 'm_pFormRubIdent->m_InsertMask'
 *  \param file :     fichier cliqué dans le glossaire
 *  \param typ_rub :  type de la rubrique concernée par ce répertoire du glossaire
*/
void CMDI_Ident::Slot_OnGlossaireFileClicked(const char *file , int typ_rub)
{if (typ_rub == CMDI_Ident::S_GetType()) m_pFormRubIdent->SetDefaultMaskFile(file);
}

//---------------------------------- GetCurrent_RubList_ID -----------------------------------------------------------------------------
/*! \brief fonction retournant l'indice (base 0 ) du document courant de la rubrique, dans la liste des documents. */
int     CMDI_Ident::GetCurrent_RubList_ID()
{return m_pFormRubIdent->Current_RubList_ID();   // noter rubrique affichée;
}
//----------------------------- reinitComboBoxWithRubList -----------------------------------
/*! \brief initialise la combo box avec les donnees de la rublist */
int  CMDI_Ident::reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay /*=""*/)
{ int index  = 0;
  int nb     = m_pCMoteurBase->initConboBoxWithRubList(pRubList, m_pFormRubIdent->comboBox_RubName, QString::number(CMDI_Ident::S_GetType()),pkToDisplay, &index);
  if (nb<=0)
     {m_pFormRubIdent->hide();
     }
  else
     {m_pFormRubIdent->show();
     }
  if (index == -1) index =  nb-1;
  m_pFormRubIdent->m_LastRub = -1;
  m_pFormRubIdent->comboBox_RubName_activated(index);
  m_pFormRubIdent->comboBox_RubName->setCurrentItem ( index );
  m_pFormRubIdent->textLabel_NbRub->setText(QString::number(nb));
  return nb;
}

//---------------------------------- Current_RubList_Iterator -----------------------------------------------------------------------------
RUBREC_LIST::iterator CMDI_Ident::Current_RubList_Iterator()
{return m_pFormRubIdent->Current_RubList_Iterator();
}

//---------------------------------- OnPrint -----------------------------------------------------------------------------
/*! \brief SLOT appelé pour imprimer le contenu de la rubrique. */
void CMDI_Ident::OnPrint()
{m_pFormRubIdent->IfModified_SaveInRubList();
 emit Sign_Print();
}

//---------------------------------- SetInterfaceOnDroits ----------------------------------------------------------------------------------------
/*! \brief SLOT appelé par qui veut, pour adapter l'interface de la rubrique aux droits de l'utilisateur.
*/
void CMDI_Ident::SetInterfaceOnDroits(const QString &/*droits*/)
{m_pFormRubIdent->comboBox_RubName_activated( m_pFormRubIdent->comboBox_RubName->currentItem() );
}

//----------------------------- SetModifiableState -----------------------------------
/*! \brief SLOT qui définit la permission de modification des données. */
void CMDI_Ident::SetModifiableState(int state)
{if (m_pFormRubIdent )m_pFormRubIdent->SetModifiableState(state);
}

//----------------------------- SetCurentDocByPrimkey -----------------------------------
/*! \brief initialise l'affichage sur le document désigné par const QString &pk */
int  CMDI_Ident::SetCurentDocByPrimkey(const QString &pk)
{return m_pFormRubIdent->SetCurentDocByPrimkey(pk);
}


//----------------------------- ChangeCurrentComboBoxItemName -----------------------------------
/*! \brief SLOT qui change le nom de l'item courant de la comboBox */
void CMDI_Ident::ChangeCurrentComboBoxItemName(const QString &name)
{if (name.length()==0)               return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) m_pFormRubIdent->comboBox_RubName->listBox()->item (  m_pFormRubIdent->comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)             return;
 pCPrtQListBoxItem->setText ( name );
}
//----------------------------- GetDocListComboBox -----------------------------------
/*! \brief retourne la combobox affectee a al liste des documents */
QComboBox    *CMDI_Ident::GetDocListComboBox()
{return m_pFormRubIdent->comboBox_RubName;
}

//----------------------------- GetCurrentEditor -----------------------------------
/*! \brief retourne l'éditeur de texte courant avancé type MyEditText */
MyEditText  *CMDI_Ident::GetCurrentEditor()
{return m_pFormRubIdent->m_pMyEditText;
}

//----------------------------- initData -----------------------------------
/*! \brief initialise la rubrique avec les données la concernant */
int  CMDI_Ident::initData()
{m_pFormRubIdent->m_LastRub    = -1;   // tres tres important
 m_pFormRubIdent->initData();
 return 0;
}

//---------------------------------- IfModified_SaveInRubList -----------------------------------------------------------------------------
/*! \brief sauvegarde le document en replaçant le contenu dans la liste des rubriques */
void CMDI_Ident::IfModified_SaveInRubList()
{m_pFormRubIdent->IfModified_SaveInRubList();
}

//---------------------------------- On_pushButtonAppend -----------------------------------------------------------------------------
/*! \brief fonction qui répond à l'appui du bouton de chargement d'une image */
void CMDI_Ident::On_pushButtonAppend()
{  if (G_pCApp->On_pushButtonAppend(this))
      {m_pFormRubIdent->pushButtonSave->show();
      }
}

//----------------------------------------- GetDefaultMaskFile --------------------------------
/*! \brief retourne le masque d'insertion par defaut des images
 *  \param path : chemin du masque d'insertion
*/
QString  CMDI_Ident::GetDefaultMaskFile()
{QString insertMask;
 if (G_pCApp->m_pDrTux==0) return QString::null;
 if (G_pCApp->m_pDrTux->m_pFormGlossaire == 0)
    {return G_pCApp->m_PathGlossaire + GetRubName()+"/STD.html";
    }
 if (insertMask.length()==0 ||  !QFile::exists( insertMask )) insertMask = G_pCApp->m_pDrTux->m_pFormGlossaire->GetCurrentFileName();
 return insertMask ;
}

//---------------------------------- DoActionWithThisDocument -----------------------------------------------------------------------------
/*! \brief fait avec le document donné en entrée une action dépendante du verbe  */
void   CMDI_Ident::DoActionWithThisDocument(const QString &verbe, int typ, const char *path /* =0 */, const QDateTime *pQDateTime /*=0 */, const QString *libelle /*=0 */, const QString *subType /*=0 */)
{G_pCApp->DoActionWithThisDocument(this, verbe, typ, path, pQDateTime, libelle, subType );
}

//---------------------------------- AddNewDocument -----------------------------------------------------------------------------
/*! \brief replace le document en cours en replaçant le contenu dans la liste des rubriques, et ajoute un nouveau document à la liste */
CRubRecord*  CMDI_Ident::AddNewDocument(const QString &strData_in, int typ, const char *path , const QDateTime *pQDateTime, const QString *libelle, const QString * subType, int noInquire)
{
 CRubRecord* pCRubRecord = G_pCApp->AddNewDocument(this, strData_in,  typ, path  , pQDateTime, libelle, subType, noInquire );
 if (pCRubRecord)
    { //........................... reinitialiser l'interface .........................................................
     m_pFormRubIdent->pushButtonSave->show();
     m_pFormRubIdent->initData();
    }
 return pCRubRecord;
}


