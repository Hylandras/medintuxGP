/************************************** C_Cps.h ***********************************
 *                                                                                *
 *   #include"C_Cps.h"                                                            *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
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

#ifndef C_CPS_H
#define C_CPS_H

#include <qobject.h>
#include <qstring.h>

#define TR QObject::tr

#define CPS_TypePS       1
#define CPS_TypeIdNat    2
#define CPS_NIR          3
#define CPS_Clef_NIR     4
#define CPS_CodeCivilite 5
#define CPS_Nom          6
#define CPS_Prenom       7

#define CPS_NumLog                1
#define CPS_ModeExe               2
#define CPS_StatutExe             3
#define CPS_SectActivite          4
#define CPS_TypIdentStruct        5
#define CPS_NumIdentStruct        6
#define CPS_NumIdentStructClef    7
#define CPS_RaisonSocialeStruct   8
#define CPS_NumIdentFactuPs       9
#define CPS_NumIdentFactuPsClef   10
#define CPS_NumIdentPsRempla      11
#define CPS_NumIdentPsRemplaClef  12
#define CPS_CodeConvention        13
#define CPS_CodeSpecialite        14
#define CPS_CodeZoneTarif         15
#define CPS_CodeZoneIk            16
#define CPS_CodeAgr1              17
#define CPS_CodeAgr2              18
#define CPS_CodeAgr3              19
#define CPS_HabilitationSignFact  20
#define CPS_HabilitationSignLot   21

//..................................... specifites selon plateforme et OS .................................
#if   defined(Q_WS_WIN)

#elif defined(Q_WS_X11)
   #define __declspec(dllexport)
#elif defined(Q_WS_MAC)

#endif

//............................................. quelques définitions ..........................................................
class QString;


//================================================= C_Cps ==============================================================
class  C_Cps
{

 public:
     C_Cps(const QString &codePorteur="", const QString &serializeData="");

     int     SetMember(int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, size_t len_data);
     QString GetMember(int memberIndex, unsigned short  iDGroupeCourant);
     QString GetCodePorteur();
     void    SetCodePorteur(const QString &codePorteur);
     QString Serialize();
     void    UnSerialize(const QString &data);
     static QString  codeConvention_ToString(int code);
     static QString  codeSpecialiteToString(int code);
     static QString  codeTypeCartePS_ToString(int code);
     static QString  codeModeExercice_ToString(int code);
     static QString  codeType_ID_Nationale_ToString(int code);
     static QString  codeTypeDeCarte_ToString(int code);
     static QString  codeSecteurActivite_ToString(int code);
     static QString  codeCiviliteToString(int code);

 public:
    QString m_TypePS;
    QString m_TypeIdNat;
    QString m_NIR;
    QString m_Clef_NIR;
    QString m_CodeCivilite;
    QString m_Nom;
    QString m_Prenom;

    QString m_NumLog;
    QString m_ModeExe;
    QString m_StatutExe;
    QString m_SectActivite;
    QString m_TypIdentStruct;
    QString m_NumIdentStruct;
    QString m_NumIdentStructClef;
    QString m_RaisonSocialeStruct;
    QString m_NumIdentFactuPs;
    QString m_NumIdentFactuPsClef;
    QString m_NumIdentPsRempla;
    QString m_NumIdentPsRemplaClef;
    QString m_CodeConvention;
    QString m_CodeSpecialite;
    QString m_CodeZoneTarif;
    QString m_CodeZoneIk;
    QString m_CodeAgr1;
    QString m_CodeAgr2;
    QString m_CodeAgr3;
    QString m_HabilitationSignFact;
    QString m_HabilitationSignLot;
    QString m_CodePorteur;
    QString m_Erreur;


};


#endif

