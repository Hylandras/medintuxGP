 /********************************* C_VitaleExt.h ************************************
 \file C_VitaleExt.h
 \brief (put some short descripion here).

     #include "C_VitaleExt.h"
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


#ifndef QFSE_C_VitaleExt
#define QFSE_C_VitaleExt
#include "C_Vitale.h"
#include "CSesam_API.h"

//================================================= C_VitaleExt ==============================================================
class C_VitaleExt : public C_Vitale
{

 public:
     C_VitaleExt( const QString &serializeData="");
     C_VitaleExt( CSesam_API* pCSesam_API , unsigned short session = 0XFFFF );

     void readCard( unsigned short session , CSesam_API* pCSesam_API = 0);

 public:
   CSesam_API* m_pCSesam_API;
};



#endif

