/********************************* CMDI_Terrain.cpp *******************************
 *                                                                                *
 *   #include "CMDI_Terrain.h"                                                    *
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

#include "CMDI_Terrain.h"
#include <qvbox.h>
#include <qmessagebox.h> 

#include "CApp.h"
#include "drtux.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Terrain ===========================================================================



//---------------------------------- CMDI_Terrain -----------------------------------------------------------------------------
CMDI_Terrain::CMDI_Terrain( QWidget* parent, const char* name, int wflags,  CMoteurBase *pCMoteurBase, RUBREC_LIST *pRubList,
                            const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* /*user*/, int type)
    :CMDI_Generic( parent, name, wflags , pCMoteurBase, pRubList, type) //: QMainWindow( parent, name, wflags )
{   SetType(TYP_TERRAIN);
    SetPrintableState(FALSE);
    //.............. positionner les dimensions de la fenetre mère de toutes ............................
    //QRect qrect (170,0,600,400);
    //setGeometry (qrect);

    //............... placer en premier un gestionnaire vertical de QWidget ..............................
    //                en fils de la fenetre mère:
    //                les enfants de ce gestionnaire seront:
    //                             le dialogue: m_pFormRubObservation    qui sera en haut
    //                             l'éditeur de texte: m_pMyEditText     qui sera en dessous
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin(0);
    pQVCentral->setSpacing(0);
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //......................... creation du dialogue:  m_pFormRubObservation .............................
    m_pFormRubTerrain    = new FormRubTerrain(pQVCentral, QString("Form") + name );
    if (m_pFormRubTerrain)
       {m_pFormRubTerrain->m_pCMoteurBase = pCMoteurBase;
        m_pFormRubTerrain->m_pRubList     = pRubList;
        if (num_GUID)   m_pFormRubTerrain->m_NumGUID    = num_GUID;
        else            m_pFormRubTerrain->m_NumGUID    = "";
        if (id_doss)    m_pFormRubTerrain->m_ID_Doss    = id_doss;
        else            m_pFormRubTerrain->m_ID_Doss    = "";
        if (dossNom)    m_pFormRubTerrain->m_DossNom    = dossNom;
        else            m_pFormRubTerrain->m_DossNom    = "";
        if (dossPrenom) m_pFormRubTerrain->m_DossPrenom = dossPrenom;
        else            m_pFormRubTerrain->m_DossPrenom = "";
        //if (user)       m_pFormRubTerrain->m_User       = user;
        //else            m_pFormRubTerrain->m_User       = "";
        m_pFormRubTerrain->SetData();
       }
    setIcon(Theme::getIcon("16x16/type_terrain.png"));
    setFocusProxy( m_pFormRubTerrain );
    setCentralWidget( pQVCentral );
}

//---------------------------------- ~CMDI_Terrain -----------------------------------------------------------------------------
CMDI_Terrain::~CMDI_Terrain()
{emit Sign_RubIsDeleted(CMDI_Terrain::S_GetRubName());
 if (G_pCApp->m_pDrTux) G_pCApp->m_pDrTux->m_pCMDI_Terrain = 0;
}

//---------------------------------- GetCurrent_RubList_ID -----------------------------------------------------------------------------
int     CMDI_Terrain::GetCurrent_RubList_ID()
{return m_pFormRubTerrain->Current_RubList_ID();   // noter rubrique affichée;
}
//---------------------------------- Current_RubList_Iterator -----------------------------------------------------------------------------
RUBREC_LIST::iterator CMDI_Terrain::Current_RubList_Iterator()
{return m_pFormRubTerrain->Current_RubList_Iterator();
}

//---------------------------------- IfModified_SaveInRubList -----------------------------------------------------------------------------
void CMDI_Terrain::IfModified_SaveInRubList()
{m_pFormRubTerrain->IfModified_SaveInRubList();
}

//---------------------------------- RubOnUserChange -----------------------------------------------------------------------------
void CMDI_Terrain::RubOnUserChange(const char* /*user*/, const char* /*sign_user*/)
{//m_pFormRubTerrain->m_User       = user;
}

//----------------------------- initData -----------------------------------
/*! \brief initialise la rubrique avec les données la concernant */
int  CMDI_Terrain::initData()
{m_pFormRubTerrain->m_LastRub    = -1;   // tres tres important
 m_pFormRubTerrain->SetData();
 m_pFormRubTerrain->setModifiedState(0);
 m_pFormRubTerrain->ButtonSaveDisplay(G_pCApp->m_Droits);
 return 0;

}

//----------------------------- SetCurentDocByPrimkey -----------------------------------
/*! \brief initialise l'affichage sur le document désigné par const QString &pk */
int  CMDI_Terrain::SetCurentDocByPrimkey(const QString &pk)
{return m_pFormRubTerrain->SetCurentDocByPrimkey(pk);
}

//----------------------------- reinitComboBoxWithRubList -----------------------------------
/*! \brief initialise la combo box avec les donnees de la rublist */
int  CMDI_Terrain::reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay /*=""*/)
{ int index = 0;
  int nb = m_pCMoteurBase->initConboBoxWithRubList(pRubList, m_pFormRubTerrain->comboBox_RubName, QString::number(CMDI_Terrain::S_GetType()), pkToDisplay, &index);
  if (index == -1) index =  nb-1;
  m_pFormRubTerrain->m_LastRub = -1;
  m_pFormRubTerrain->comboBox_RubName->setCurrentItem ( index );
  m_pFormRubTerrain->comboBox_RubName_activated(index);
  //m_pFormRubTerrain->textLabel_NbRub->setText(QString::number(nb));
  return nb;
}

//----------------------------- GetDocListComboBox -----------------------------------
/*! \brief retourne la combobox affectee a al liste des documents */
QComboBox    *CMDI_Terrain::GetDocListComboBox()
{return m_pFormRubTerrain->comboBox_RubName;
}
//----------------------------- ChangeCurrentComboBoxItemName -----------------------------------
/*! \brief SLOT qui change le nom de l'item courant de la comboBox */
void CMDI_Terrain::ChangeCurrentComboBoxItemName(const QString &name)
{if (name.length()==0)               return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) m_pFormRubTerrain->comboBox_RubName->listBox()->item (  m_pFormRubTerrain->comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)             return;
 pCPrtQListBoxItem->setText ( name );
}

//----------------------------- GetCurrentEditor -----------------------------------
/*! \brief retourne l'éditeur de texte courant avancé type MyEditText */
MyEditText  *CMDI_Terrain::GetCurrentEditor()
{return 0;          // y en a pas
}

//---------------------------------- SetInterfaceOnDroits -----------------------------------------------------------------------------
/*! \brief positionne l'interface sur les droits */
void CMDI_Terrain::SetInterfaceOnDroits(const QString &droits)
{//if (droits.find("ata")!=-1) m_pFormRubTerrain->m_IsAllDroitsOn = TRUE;
 //else                        m_pFormRubTerrain->m_IsAllDroitsOn = FALSE;

 m_pFormRubTerrain->m_IsAllDroitsOn = G_pCApp->IsThisDroitExist(droits, "ata");
 m_pFormRubTerrain->SetInterfaceOnDroits(droits);
}

//---------------------------------- AddNewDocument -----------------------------------------------------------------------------
/*! \brief replace le document en cours en replaçant le contenu dans la liste des rubriques, et ajoute un nouveau document à la liste */
CRubRecord* CMDI_Terrain::AddNewDocument(const QString &strData, int typ, const char *path /*=0*/ , const QDateTime *pQDateTime/*=0*/, const QString * /*=0*/, const QString *subType /*=0*/, int  /*=0*/)
{//............. placer les données de la rubrique en cours d'edition, si necessaire .............................
 //              dans la liste des rubriques, donc à sauver (que si necessaire CAD modifiées)
 int              ret       =   0;
 QDateTime        qdt       =   QDateTime::currentDateTime();
 if (pQDateTime)  qdt       =  *pQDateTime;
 m_pFormRubTerrain->IfModified_SaveInRubList();
 //............. ajouter la nouvelle rubrique .........................................
 if (m_pFormRubTerrain->nbDate())  // si un tecase TYP_OBSERVATION:rrain existe déjà on demande
    {ret = QMessageBox::warning ( this, tr ("Nouveau Terrain "),
                                        tr ("Un terrain existe déjà \r\n"
                                            "Faut-il vraiment en créer un deuxième ?\r\n"
                                            "ou ajouter cette variable aux autres"),
                                        tr ("&Créer"), tr("A&jouter"), tr("Annu&ler"),
                                        1, 1 );
     if (ret == 2) return 0;
    }
 if (ret==1)
    {m_pFormRubTerrain->newVAR_Add(path);
    }
 else
   {RUBREC_LIST::Iterator last  = m_pRubList->append(CRubRecord(G_pCApp->m_pCMoteurBase->GetPkDoc_Provisoire(m_pRubList),                                     // PrimKey
                                  "Terrain",                               // libelle
                                  QString::number(typ),                    // type
                                  subType ? *subType : "Default SubType",  // sous type
                                  qdt.toString ("yyyy-MM-ddThh:mm:ss"),    // date l'enregistrement
                                  G_pCApp->m_User,                         // utilisateur
                                  G_pCApp->m_SignUser,                     // utilisateur signataire
                                  strData,                                 // données texte
                                  strData.length()                         // longueur des données
                              )
                    );
    // cela permet de reinitialiser les combos box & cie
    m_pFormRubTerrain->SetData();
    m_pFormRubTerrain->m_IsModified = 1;
    m_pFormRubTerrain->pushButtonSave->show();
    return &(*last);
   }
 return 0;
}


//---------------------------------- OnIdentChange ----------------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Terrain::OnIdentChange(const char* num_GUID,  const char* id_doss,
                                 const char* dossNom ,  const char* dossPrenom )
{ m_pFormRubTerrain->m_NumGUID    = num_GUID;
  m_pFormRubTerrain->m_ID_Doss    = id_doss;
  m_pFormRubTerrain->m_DossNom    = dossNom;
  m_pFormRubTerrain->m_DossPrenom = dossPrenom;
  m_pFormRubTerrain->m_LastRub    = -1;          // tres tres important
  m_pFormRubTerrain->SetData();
}
//---------------------------------- GetTaille -----------------------------------------------------------------------------
QString CMDI_Terrain::GetTaille()
{return m_pFormRubTerrain->GetTaille();
}
//---------------------------------- GetPoids -----------------------------------------------------------------------------
QString CMDI_Terrain::GetPoids()
{return m_pFormRubTerrain->GetPoids();
}
//---------------------------------- SetPoids -----------------------------------------------------------------------------
void CMDI_Terrain::SetPoids(QString poids)
{m_pFormRubTerrain->VAR_SetPoids(poids);
}
//---------------------------------- SetTaille -----------------------------------------------------------------------------
void CMDI_Terrain::SetTaille(QString taille)
{ m_pFormRubTerrain->VAR_SetPoids(taille);
}
//---------------------------------- SetPoidsTaille -----------------------------------------------------------------------------
void CMDI_Terrain::SetPoidsTaille(QString poids, QString taille)
{ m_pFormRubTerrain->VAR_SetPoidsTaille(poids,taille);
}

//---------------------------------- GetSurface -----------------------------------------------------------------------------
QString CMDI_Terrain::GetSurface()
{return m_pFormRubTerrain->GetSurface();
}

//---------------------------------- SetModifiableState -----------------------------------------------------------------------------
void CMDI_Terrain::SetModifiableState(int state)
{m_pFormRubTerrain->SetModifiableState(state);
}
