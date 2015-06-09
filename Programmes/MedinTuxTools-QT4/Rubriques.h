/********************************** Rubriques.h ***********************************
 *                                                                                *
 *   #include "Rubriques.h"                                                       *
 *   Project: MedinTux    and Data Medical Design                                 *
 *   Copyright (C) 2003 2004 2005 by Sébastien SIVERA & Sevin Roland              *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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


#ifndef RUBRIQUES_H
#define RUBRIQUES_H



//........................ types de rubriques ..........................................................

#define TYP_IDENTITE              20090000
#define TYP_CHOIX_PATIENT         2          // +2 à +2001
#define TYP_PARAM                 5
#define TYP_TERRAIN               20060000
#define TYP_DOCUMENTS             20080000
#define TYP_OBSERVATION           20030000   // +0 à +999 recouvre les observations textuelles
#define TYP_PRESCRIPTION          20020000   // +0 à +999 recouvre toutes les prescriptions (certif + ordo)
#define TYP_GROUPEDVIEW		  19840000



//...................... nouveaux sous types ...........................................................
//#define TYP_ORDO_CALC_CURENT      20020999  // pour que date ordo soit la date courante lors appel DrTux::AddNewRecordToRubrique()

#define TYP_ORDO_CALC             20020100
#define TYP_ORDONNANCE            20020200
#define TYP_CERTIFICAT            20020300
#define TYP_COURRIER              20020500
#define TYP_URG_OBSERV            20040000



#endif





