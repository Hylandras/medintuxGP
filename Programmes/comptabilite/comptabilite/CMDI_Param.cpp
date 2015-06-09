 /********************************* CMDI_Param.cpp **********************************
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
/*! \file CMDI_Param.cpp
    \brief (put some short descripion here).

     #include "CMDI_Param.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Param.h"

//---------------------------------- CMDI_Param -----------------------------------------------------------------------------
CMDI_Param::CMDI_Param( QWidget* parent, const char* name,
		int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags , pComptaMoteurBase)
{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_Param");
 m_pParametres_Ui = 0;
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_pParametres_Ui = new Parametres_Ui(this);
 if (!m_pParametres_Ui) return ;

 m_pParametres_Ui->setParams(m_pComptaMoteurBase, G_pCApp->m_currentUser, 
			     G_pCApp->m_PathSauvegarde, G_pCApp->m_Verbose);

 setFocusProxy( m_pParametres_Ui );
 setCentralWidget( m_pParametres_Ui );
 SetType(TYP_PARAM);
}


/*! \brief Destructeur de la classe */
CMDI_Param::~CMDI_Param()
{if (G_pCApp->m_Verbose) qWarning("CMDI_Param::Destruct");
 if (m_pParametres_Ui)		delete m_pParametres_Ui;
 if (G_pCApp->m_Verbose) qWarning("CMDI_Param::Destruct Fin");
}



//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Param::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, tr("Param"), 5)!=0) return;
 GlobalActiverWidget(this);
}


