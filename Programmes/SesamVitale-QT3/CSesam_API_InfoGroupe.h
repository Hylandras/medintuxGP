 /********************************* CSesam_API_InfoGroupe.h **********************
 \file CSesam_API_InfoGroupe.h
 \brief (put some short descripion here).

     #include "CSesam_API_InfoGroupe.h"
     Project: MedinTux
     Copyright (C) 2003 2004 2005 2006 2007
     by Sevin Roland  and Data Medical Design
     E-Mail: medintux@medintux.org

 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
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


#ifndef QFSE_CSesam_API_InfoGroupe
#define QFSE_CSesam_API_InfoGroupe

#include <qstring.h>
#include <qobject.h>
#define TR QObject::tr
//================================================= CSesam_API_InfoGroupe ==============================================================
class CSesam_API_InfoGroupe
{

 public:
     CSesam_API_InfoGroupe();     
     static QString NumGroupeToString(int groupe, int champ = -1);
     static QString messageChampInconnu(int groupe, int champ);
};



#endif

