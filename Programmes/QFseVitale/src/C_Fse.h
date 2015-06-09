 /********************************* C_Fse.h ************************************
 \file C_Fse.h
 \brief (put some short descripion here).

     #include "C_Fse.h"
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


#ifndef QFSE_C_Fse
#define QFSE_C_Fse

#include "../../SesamVitale-QT3/CSesam_API.h"
#define TR QObject::tr

//............................................. quelques définitions ..........................................................
class QString;


//================================================= C_Fse ==============================================================
class C_Fse
{

 public:
      C_Fse(CSesam_API* pCSesam_API , unsigned short session = -1 , const QString &codePorteur="", const QString &serializeData="");
     ~C_Fse();
     void            SetSession(unsigned short session);
     unsigned short  GetSession();
     int     SetMember(int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, size_t len_data, int occur);
     QString GetMember(int memberIndex, unsigned short  iDGroupeCourant, int occur);
     QString GetCodePorteur();
     int     GetNbOccurences(unsigned short  iDGroupeCourant);
     void    SetCodePorteur(const QString &codePorteur);
     QString sectionValuesToStr(unsigned short  iDGroupeCourant, int occur);
     QString Serialize();
     void    UnSerialize(const QString &data);
     static QString Get_CSV_TableRef(const char *csvData,
                                     const char *refToMatch,
                                     int colToExplore,
                                     QStringList &listToReturn,
                                     char sepChp  = '#'  ,
                                     char delStr  = '|' );
     static char *GotoEndRaw( char *pt, char sepChp  = '#'  , char delStr  = '|' );
 public:
    CSesam_API*    m_pCSesam_API;
    unsigned short m_Session;
    QString        m_CodePorteur;
    QString        m_FseData;


};


#endif

