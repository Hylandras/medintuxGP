 /********************************* CMDI_Actes.cpp **********************************
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
/*! \file CMDI_Actes.cpp
    \brief (put some short descripion here).

     #include "CMDI_Actes.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Actes.h"

CMDI_Actes::CMDI_Actes  ( QWidget* parent, const char* name,
			  int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags, pComptaMoteurBase)

{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_Actes");
 pActes_Ui = new Actes_Ui(pComptaMoteurBase, this, "pActes_Ui" );
 if (pActes_Ui)
  { connect (pActes_Ui, SIGNAL ( sign_Actes_Ui_ActesListeModif() ),
	 	this,   SLOT   ( slot_CMDI_ActesListeModif() ));
  }
 setFocusProxy( pActes_Ui );
 setCentralWidget( pActes_Ui );
 SetType(TYP_ACTES);
}


CMDI_Actes::~CMDI_Actes()
{ if (G_pCApp->m_Verbose) qWarning("CMDI_Actes::Destruct");
  if (pActes_Ui) {if (G_pCApp->m_Verbose) qWarning("CMDI_Actes::Deleting pActes_Ui"); delete pActes_Ui;}
  if (G_pCApp->m_Verbose) qWarning("CMDI_Actes::Destruc Fin");
}


//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Actes::ActiverRubrique(const char *rubName)
{if (strncmp(rubName,"Actes",5)!=0) return;
 GlobalActiverWidget(this);
}


void CMDI_Actes::slot_CMDI_ActesListeModif()
{ emit sign_ActesListeModif(); }


