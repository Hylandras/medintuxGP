/********************************* CMDI_ChoixPatient.cpp *************************
 *                                                                                *
 *   #include "CMDI_ChoixPatient.h"                                               *
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

#include "CMDI_ChoixPatient.h"
#include "CApp.h"
#include "drtux.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_ChoixPatient =======================================================================



//---------------------------------- CMDI_ChoixPatient -----------------------------------------------------------------------------
CMDI_ChoixPatient::CMDI_ChoixPatient( QWidget* parent, const char* name, int wflags,  CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
                                      const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user, int type)
   :CMDI_Generic( parent, name, wflags , pCMoteurBase, pRubList, type) // : QMainWindow( parent, name, wflags )
{
    SetType(TYP_CHOIX_PATIENT);
    SetPrintableState(FALSE);
    m_pFormRubChoixPatient = new FormRubChoixPatient(this, QString("Form")+name );

    if (m_pFormRubChoixPatient)
        {m_pFormRubChoixPatient->m_pCMoteurBase = pCMoteurBase;
         if (num_GUID)   m_pFormRubChoixPatient->m_NumGUID    = num_GUID;
         else            m_pFormRubChoixPatient->m_NumGUID    = "";
         if (id_doss)    m_pFormRubChoixPatient->m_ID_Doss    = id_doss;
         else            m_pFormRubChoixPatient->m_ID_Doss    = "";
         if (dossNom)    m_pFormRubChoixPatient->m_DossNom    = dossNom;
         else            m_pFormRubChoixPatient->m_DossNom    = "";
         if (dossPrenom) m_pFormRubChoixPatient->m_DossPrenom = dossPrenom;
         else            m_pFormRubChoixPatient->m_DossPrenom = "";
         if (dossPrenom) m_pFormRubChoixPatient->m_User       = user;
         else            m_pFormRubChoixPatient->m_User       = "";
         m_pFormRubChoixPatient->InitAllList("","");
         //m_pFormPatientList->textEdit_Rubrique->installEventFilter( keyPressEater );
        }
    setIcon(Theme::getIcon("16x16/type_patientlist.png"));
    setFocusProxy( m_pFormRubChoixPatient );
    setCentralWidget( m_pFormRubChoixPatient );
}

//---------------------------------- ~CMDI_ChoixPatient ------------------------------------------------------------------------
CMDI_ChoixPatient::~CMDI_ChoixPatient()
{emit Sign_RubIsDeleted(CMDI_ChoixPatient::S_GetRubName());
 if (G_pCApp->m_pDrTux) G_pCApp->m_pDrTux->m_pCMDI_ChoixPatient = 0;
}

//---------------------------------- DoConnexionOnGossaire -----------------------------------------------------------------------------
// on surcharge pour annuler les automatismes de la classe parente
void CMDI_ChoixPatient::DoConnexionOnGossaire(C_FormGlossaire *)
{//G_pCApp->CouCou(tr("Choix patient en cours (pas de connexion au Glossaire)..."));
}
//----------------------------------------------- mouseEnter ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris rentre dans la rubrique surcharge pour annuler les automatismes de la classe parente

 */
void CMDI_ChoixPatient::mouseEnter ()
{//emit Sign_MouseEnter(m_PathDocuments, m_DefaultMaskDirectory);
 //G_pCApp->CouCou(tr("On entre dans Choix patient"));
}

//----------------------------------------------- mouseLeave ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris sort de la rubrique surcharge pour annuler les automatismes de la classe parente
 */
void CMDI_ChoixPatient::mouseLeave ()
{//emit Sign_MouseLeave(m_PathDocuments, m_DefaultMaskDirectory);
 //G_pCApp->CouCou(tr("On sort de Choix patient"));
}

//---------------------------------- RubOnUserChange -----------------------------------------------------------------------------
void CMDI_ChoixPatient::RubOnUserChange(const char*user, const char* /*sign_user*/)
{m_pFormRubChoixPatient->m_User       = user;
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

//void CMDI_ChoixPatient::OnIdentChange(const char* num_GUID,  const char* id_doss,
//                                      const char* dossNom ,  const char* dossPrenom )
void CMDI_ChoixPatient::OnIdentChange(const char* ,  const char* ,
                                      const char*  ,  const char*  )
{
}

//---------------------------------- SetInterfaceOnDroits -----------------------------------------------------------------------------
void CMDI_ChoixPatient::SetInterfaceOnDroits(const QString &droits)
{m_pFormRubChoixPatient->SetInterfaceOnDroits(droits);
}
