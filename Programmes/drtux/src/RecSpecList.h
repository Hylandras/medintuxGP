/********************************** CRecSpecList.h *******************************
 *                                                                                *
 *   #include "CRecSpecList.h"                                                    *
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

#ifndef REC_SPEC_LIST_H
#define REC_SPEC_LIST_H

#include <qvaluelist.h>
#include <qsqldatabase.h>
#include <qcstring.h>

//...................... Classe dont il sera fait une liste pour chaque enregistrement ...............................
//

class CRecSpec
      {public:
       CRecSpec(){}    //indispensable pour QValuelist
       CRecSpec( const char *nom, const char *prenom, long index)
          {qstrncpy(ms_Nom,     nom,      sizeof(ms_Nom)-1);
           qstrncpy(ms_Prenom,  prenom,   sizeof(ms_Prenom)-1);
           ms_RecNum = index;
          }

       char   ms_Nom[32];
       char   ms_Prenom[32];
       long   ms_RecNum;
      };

//..................... Liste de la classe definie au dessus .........................................................
//                      la classe dialogue Praticien_Dlg possede un membre: REC_SPEC_LIST m_RecSpeclist;
//                      qui stockera les noms, prenoms, specialités, n° d'enregistrement de chaque
//                      enregistrement de la base.
//                      l'incorporation et declaration du membre se fait au niveau de QT Designer;
//                      Ce .h est ausi incorporé au niveau de QT Designer.

typedef QValueList<CRecSpec> REC_SPEC_LIST;

#endif

