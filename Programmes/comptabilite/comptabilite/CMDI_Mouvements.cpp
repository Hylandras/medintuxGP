 /****************************** CMDI_Mouvements.cpp ******************************
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
/*! \file CMDI_Mouvements.cpp
    \brief Classe CMDI_Mouvements.

     #include "CMDI_Mouvements.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

/*! \class CMDI_Mouvements
    \brief Gère la fenêtre de gestion des mouvements.

     #include "CMDI_Mouvements.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Mouvements.h"

/*! \brief Constructeur de la classe.
 *  \todo Problème de GridLayout à corriger +++
*/

CMDI_Mouvements::CMDI_Mouvements  ( QWidget* parent, const char* name,
				    int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags, pComptaMoteurBase)

{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_Livres");
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_pMvts_Ui = 0;

 m_pMvts_Ui = new Mvts_Ui(this, "Mvts_Ui");
 if (!m_pMvts_Ui) return ;
//  connect ( m_pMvts_Ui, 	SIGNAL( mvtsChange() ), 
// 	   this, 	SLOT ( emitSignalChange() ) );
 m_pMvts_Ui->setParams( m_pComptaMoteurBase );

 setFocusProxy( m_pMvts_Ui );
 setCentralWidget( m_pMvts_Ui );
 SetType(TYP_MOUVEMENTS);

}


CMDI_Mouvements::~CMDI_Mouvements()
{ if (G_pCApp->m_Verbose) qWarning("CMDI_Mouvements::Destruct");
  if (m_pMvts_Ui)		delete m_pMvts_Ui;
  if (G_pCApp->m_Verbose) qWarning("CMDI_Mouvements::Destruct Fin");
}


void CMDI_Mouvements::getComboComptes()
{ m_pMvts_Ui->getComboComptes();
//  m_pImmob_Ui->getComboComptes();
}


void CMDI_Mouvements::getMouvements()
{ m_pMvts_Ui->getMouvements();
//  m_pImmob_Ui->setTableTo();
//  QString ba = "";
//  m_pComptaMoteurBase->getBilanAnnuel(QString::number(G_pCApp->m_Filtre_Date_Debut.year()), ba);
//  bilan->setText(ba);
}


void CMDI_Mouvements::emitSignalChange()
{ getMouvements();
  emit mouvementsChange();
}


//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Mouvements::ActiverRubrique(const char *rubName)
{if (strncmp(rubName,"Mouve",5)!=0) return;
 GlobalActiverWidget(this);
}



