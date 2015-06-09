/************************************** Global.h *********************************
 *                                                                                *
 *   #include "Global.h"                                                          *
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

#ifndef DR_TUX_GLOBAL_H
#define DR_TUX_GLOBAL_H

#include <qtextedit.h>
#include <qstring.h>

//................................. sur la definition ds chemins types peut mieux faire !! .........................................
//#define RGB(r,g,b)  (0xFF<<24|(((int)(r)&0xFF)<<16)|(((int)(g)&0xFF)<<8)|(((int)(b)&0xFF)))
//#define RGB(r,g,b)  ((((int)(r)&0xFF)<<24)|(((int)(g)&0xFF)<<16)|(((int)(b)&0xFF)<<8)|0xFF)



#ifdef Q_WS_WIN
    #define PROG_NAME "drtux.exe"
    #define F_EXT  ".exe"
    #define INSERTION_ROOT            "\\Champs d'insertions"
    #define MASQUES_OBSERV_ROOT       "\\Observation"
    #define MASQUES_PRESCR_ROOT       "\\Prescription"
    #define MASQUES_PRESCR_CERTIF     "\\Prescription\\Certificats\\"
    #define MASQUES_PRESCR_ORDO       "\\Prescription\\Ordonnances\\"
    #define MASQUES_PRESCR_COURRIER   "\\Prescription\\Courriers\\"
    #define CHEMIN_CIM10_URG          "\\CIM10Urg"
    #define CHEMIN_TERRAIN            "\\Terrain"
#endif
#ifdef Q_WS_MAC
    #define PROG_NAME "drtux"
    #define F_EXT  ".app"
    #define INSERTION_ROOT            "/Champs d'insertions"
    #define MASQUES_OBSERV_ROOT       "/Observation"
    #define MASQUES_PRESCR_ROOT       "/Prescription"
    #define MASQUES_PRESCR_CERTIF     "/Prescription/Certificats/"
    #define MASQUES_PRESCR_ORDO       "/Prescription/Ordonnances/"
    #define MASQUES_PRESCR_COURRIER   "/Prescription/Courriers/"
    #define CHEMIN_CIM10_URG          "/CIM10Urg"
    #define CHEMIN_TERRAIN            "/Terrain"
#endif
#ifdef Q_WS_X11
    #define PROG_NAME "drtux"
    #define F_EXT  ""
    #define INSERTION_ROOT            "/Champs d'insertions"
    #define MASQUES_OBSERV_ROOT       "/Observation"
    #define MASQUES_PRESCR_ROOT       "/Prescription"
    #define MASQUES_PRESCR_CERTIF     "/Prescription/Certificats/"
    #define MASQUES_PRESCR_ORDO       "/Prescription/Ordonnances/"
    #define MASQUES_PRESCR_COURRIER   "/Prescription/Courriers/"
    #define CHEMIN_CIM10_URG          "/CIM10Urg"
    #define CHEMIN_TERRAIN            "/Terrain"
#endif

#define NO_PATIENT_CHOICE  1
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))
#define SKIP_BLANK_CAR(a)     while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define NEXT_LINE(a)          while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++

//.............. pour type de setData() de la fiche identite ..............................................................
#define  NEW_IDENTITE 1
#define  FTN_MAJ_STRING 01
#define  FTN_STRING     02
#define  FTN_NUM_STRING 03
#define  FTN_INTEGER    12
#define  FTN_CHAR       23
#define  FTN_DATETIME   34

#define DEB_PAR              "<br />"
#define END_PAR              ""

#define ASCII_DEB_PAR        "\r\n"
#define ASCII_END_PAR        ""
#define NB_SP_INDENT         2
#define CR_TO_HTML           1

#endif

