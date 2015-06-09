 /********************************* CMDI_Depots.h ************************************
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
/*! \file CMDI_Depots.h
    \brief (put some short descripion here).

     #include "CMDI_Depots.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef CMDI_DEPOTS_H
#define CMDI_DEPOTS_H

#include <qobject.h>

#include "CMDI_Generic.h"
#include "ComptaMoteurBase.h"
#include "compta.h"
#include "CApp.h"
#include "Depots.h"
#include "Listes.h"

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/


class CMDI_Depots : public  CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Depots( QWidget* parent, const char* name,
		int wflags , ComptaMoteurBase *pComptaMoteurBase);
   ~CMDI_Depots();

public:

public slots:
    void	comptesChanged();
    void	changeTableFilter();
    void 	ActiverRubrique (const char *rubName);
    void	emitSignal();

signals:
    void	depotsChange();

public:
    Depots_Ui*		m_pDepots_Ui;

};

#endif
