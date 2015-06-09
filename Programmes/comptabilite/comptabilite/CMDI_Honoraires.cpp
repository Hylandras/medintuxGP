 /********************************* CMDI_Honoraires.cpp **********************************
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
/*! \file CMDI_Honoraires.cpp
    \brief Classe CMDI_Honoraires.

     #include "CMDI_Honoraires.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

/*! \class CMDI_Honoraires
    \brief Gère la fenêtre de gestion des honorairess.

     #include "CMDI_Honoraires.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Honoraires.h"


CMDI_Honoraires::CMDI_Honoraires  ( QWidget* parent, const char* name,
				    int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags, pComptaMoteurBase)

{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_Honoraires");
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_pHono_View = new Hono_View(this);
 if (!m_pHono_View) return ;
 m_pHono_View ->setParams(m_pComptaMoteurBase, G_pCApp->m_currentUser, G_pCApp->m_Verbose);

 setFocusProxy( m_pHono_View  );
 setCentralWidget( m_pHono_View  );
 SetType(TYP_HONO);
}

/*! \brief Destructeur de la classe. */
CMDI_Honoraires::~CMDI_Honoraires()
{ if (G_pCApp->m_Verbose) qWarning("CMDI_Honoraires::Destruct");
  if (m_pHono_View ) 	delete m_pHono_View ;
  //m_pComptaMoteurBase deleted par CApp 
  if (G_pCApp->m_Verbose) qWarning("CMDI_Honoraires::Destruct Fin");
}


//************************* changeTableFilter *************************
/*! \brief SLOT activé par la fenêtre de filtrage (Filtre_Hono). Modifie le filtre de la table des honoraires. 
*/
void CMDI_Honoraires::changeTableFilter( )
{ if (m_pHono_View) m_pHono_View->changeTableFilter();
}


//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Honoraires::ActiverRubrique(const char *rubName)
{if (strncmp(rubName,"Honor",5)!=0) return;
 GlobalActiverWidget(this);
}



