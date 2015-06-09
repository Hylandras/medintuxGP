 /********************************* CMDI_Depots.cpp **********************************
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
/*! \file CMDI_Depots.cpp
    \brief (put some short descripion here).

     #include "CMDI_Depots.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Depots.h"

/*! \class CMDI_Depots
 *  \brief Fenêtre pour la gestion des remises de chèques et d'espèces.
 *  \sa Depots_Ui, Depots
*/

//----------------------- CMDI_Depots --------------------------------------
CMDI_Depots::CMDI_Depots( QWidget* parent, const char* name,
		int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags , pComptaMoteurBase)
{ if (G_pCApp->m_Verbose) qWarning("Création CMDI_Depots");
  m_pDepots_Ui = 0;
  m_pDepots_Ui = new Depots_Ui(pComptaMoteurBase, this, "m_pDepots_Ui" );
  if (m_pDepots_Ui)
   { m_pDepots_Ui->show();
     connect ( m_pDepots_Ui, SIGNAL( depotsChange1() ), this, SLOT( emitSignal()) );
   }
  else { return ;}
  setFocusProxy( m_pDepots_Ui );
  setCentralWidget( m_pDepots_Ui );
  SetType(TYP_BANQUE);
}


//---------------------------------- ~CMDI_Depots ------------------------------------------------------------------------
CMDI_Depots::~CMDI_Depots()
{if (G_pCApp->m_Verbose) qWarning("CMDI_Depots::Destruct");
 if (m_pDepots_Ui) delete m_pDepots_Ui;
 if (G_pCApp->m_Verbose) qWarning("CMDI_Depots::Destruct Fin");
}

void CMDI_Depots::comptesChanged()
{ m_pDepots_Ui->getComboComptes();
}

//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Depots::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, TR("Banque"), 5)!=0) return;
 GlobalActiverWidget(this);
}


/*! \brief SLOT appelé lorsque l'utilisateur modifie le filtre. Met à jour le tableau des chèques à déposer. */
void CMDI_Depots::changeTableFilter()
{ m_pDepots_Ui->viewDepot( m_pDepots_Ui->m_Depots_Liste->count() );  }

void CMDI_Depots::emitSignal()
{ emit( depotsChange() ); }
