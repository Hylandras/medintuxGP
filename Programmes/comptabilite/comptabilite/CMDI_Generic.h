 /********************************* CMDI_Generic.h ************************************
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
/*! \file CMDI_Generic.h
    \brief (put some short descripion here).

     #include "CMDI_Generic.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef CMDI_GENERIC_H
#define CMDI_GENERIC_H

#include <qmainwindow.h>
#include <qobject.h>
#include "ComptaMoteurBase.h"
#include "Listes.h"

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/

class CMDI_Generic : public QMainWindow
{
    Q_OBJECT

public:
   CMDI_Generic (QWidget* parent, const char* name, int wflags, ComptaMoteurBase *pComptaMoteurBase)
       :QMainWindow( parent, name, wflags )
       {m_pComptaMoteurBase = pComptaMoteurBase;
        m_IsModified   = FALSE;
       }

   int     GetType()
              {return m_Type;
              }
   void    SetType(int type)
              {m_Type = type;
              }
   void    setCurrentUser(Utilisateurs* u)	
					{ qWarning("setting u"); 
					  m_currentUser = new Utilisateurs(); 
					  m_currentUser = u; }


public:
   ComptaMoteurBase*    m_pComptaMoteurBase;
   bool			m_IsModified;
   Utilisateurs*	m_currentUser;
   bool                 m_verbose;


private:
   int              m_Type;
};

#endif
