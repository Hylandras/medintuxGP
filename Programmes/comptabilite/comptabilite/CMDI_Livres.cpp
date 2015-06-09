 /********************************* CMDI_Livres.cpp **********************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
/*! \file CMDI_Livres.cpp
    \brief (put some short descripion here).

     #include "CMDI_Livres.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Livres.h"


CMDI_Livres::CMDI_Livres  ( QWidget* parent, const char* name,
				    int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags, pComptaMoteurBase)

{if (G_pCApp->m_Verbose) qWarning("CMDI_Livres :: Création de CMDI_Livres");
 m_pComptaMoteurBase = pComptaMoteurBase;

 if (G_pCApp->m_Verbose) qWarning("CMDI_Livres :: Création de  l'interface de CMDI_Livres");
 m_pGLBA = new GrandLivre_BilanAnnuel(this);
 if (!m_pGLBA) return ;
 m_pGLBA->setParams(m_pComptaMoteurBase, G_pCApp->m_PathSauvegarde);


 QString ba = "";
 m_pComptaMoteurBase->getBilanAnnuel( QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()),
                                      ba, ComptaMoteurBase::ForBilanAnnuel);
 m_pGLBA->bilanAnnuel->setText(ba);

 ba = "";
 m_pComptaMoteurBase->getBilanAnnuel(QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()),
                                     ba, ComptaMoteurBase::ForGrandLivre);
 m_pGLBA->gdLivre->setText(ba);

 setFocusProxy( m_pGLBA );
 setCentralWidget( m_pGLBA );
 SetType(TYP_LIVRES);
}


CMDI_Livres::~CMDI_Livres()
{if (G_pCApp->m_Verbose) qWarning("CMDI_Livres::destruct");

if (G_pCApp->m_Verbose) qWarning("CMDI_Livres::destruct Fin");
}

/*! \brief Slot activé lorsque les mouvements / immobilisations ont été modifiés par l'utilisateur.
*/
void CMDI_Livres::recalculLivres()
{QString ba = "";
 m_pComptaMoteurBase->getBilanAnnuel(QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()), ba, FALSE);
 m_pGLBA->bilanAnnuel->setText(ba);

 ba = "";
 m_pComptaMoteurBase->getBilanAnnuel(QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()), ba, TRUE);
 m_pGLBA->gdLivre->setText(ba);
}

 
//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Livres::ActiverRubrique(const char *rubName)
{if (strncmp(rubName,"Livre",5)!=0) return;
 GlobalActiverWidget(this);
}


