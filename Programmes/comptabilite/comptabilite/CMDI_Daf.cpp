 /********************************* CMDI_DAF.cpp **********************************
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
/*! \file CMDI_Daf.cpp
    \brief (put some short descripion here).

     #include "CMDI_Daf.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Daf.h"


CMDI_DAF::CMDI_DAF  ( QWidget* parent, const char* name,
                      int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags, pComptaMoteurBase)

{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_DAF");
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_pDaf_View = new Daf_View(this);
 if (!m_pDaf_View) return ;
 m_pDaf_View->setParams( m_pComptaMoteurBase, G_pCApp->m_currentUser, G_pCApp->m_Verbose);

 setFocusProxy( m_pDaf_View );
 setCentralWidget( m_pDaf_View );
 SetType(TYP_DAF);

}


CMDI_DAF::~CMDI_DAF()
{if (G_pCApp->m_Verbose) qWarning("CMDI_DAF::destruct");
 if (m_pDaf_View)		delete m_pDaf_View;
 if (G_pCApp->m_Verbose) qWarning("CMDI_DAF::destruct");
}


void CMDI_DAF::filtreChanged()
{ if (m_pDaf_View) m_pDaf_View->setTableTo();
}

//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_DAF::ActiverRubrique(const char *rubName)
{if (strncmp(rubName,"DAF",3)!=0) return;
 GlobalActiverWidget(this);
}


