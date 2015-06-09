 /********************************* C_DBVarDrTux.cpp ******************************
 *  #include "C_DBVarDrTux.h"                                                     *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
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

// C++ Interface: C_DBVarDrTux.cpp
//
#include "C_DBVarDrTux.h"
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++

//-----------------------------------------------------  C_DBVarDrTux -------------------------------------------
C_DBVarDrTux::C_DBVarDrTux(  QString& confData, QString *errMess)
{m_IsDBFieldsNamesValids = 0;
if (confData.length()==0) {if (errMess) (*errMess) +=  TR("\r\n Fichier de configuration des bases absent ou vide "); return;}
 m_IsDBFieldsNamesValids =  initValues(confData, errMess);
}

//-----------------------------------------------------  SetConfBase -------------------------------------------
int C_DBVarDrTux::initValues( QString& confData, QString *errMess)
{ QByteArray ba = confData.toAscii();
  QString   err = "";
  char     *deb = (char*)(const char *)ba;
  char      *pt = deb;
  int      line = 0;
  //.............................. NUMERO DE VERSION DE LA BASE .........................................................
  pt = SetConfBase_SetProperties(deb, m_VERSION_NUMBER,           "VERSION_NUMBER",            &line , err); if (err.length())     goto SetConfBase_Error;

  //.............................. DOSS_INDEX_TBL_NAME .........................................................
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_TBL_NAME,      "DOSS_INDEX_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_GUID,          "DOSS_INDEX_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_NOM,           "DOSS_INDEX_NOM",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_PRENOM,        "DOSS_INDEX_PRENOM",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_PRIM_KEY,      "DOSS_INDEX_PRIM_KEY",       &line , err); if (err.length())     goto SetConfBase_Error;

  //............................ DOSS_IDENT_TBL_NAME ...........................................................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TBL_NAME,     "DOSS_IDENT_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_GUID,         "DOSS_IDENT_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_JFNOM,        "DOSS_IDENT_JFNOM",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NSS,          "DOSS_IDENT_NSS",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_SEX,          "DOSS_IDENT_SEX",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NB_ENF,       "DOSS_IDENT_NB_ENF",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_RUE,          "DOSS_IDENT_RUE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_CODE_POST,    "DOSS_IDENT_CODE_POST",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_VILLE,        "DOSS_IDENT_VILLE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_CEDEX,        "DOSS_IDENT_CEDEX",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_PAYS,         "DOSS_IDENT_PAYS",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL1,         "DOSS_IDENT_TEL1",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL1_TYP,     "DOSS_IDENT_TEL1_TYP",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL2,         "DOSS_IDENT_TEL2",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL2_TYP,     "DOSS_IDENT_TEL2_TYP",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL3,         "DOSS_IDENT_TEL3",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TEL3_TYP,     "DOSS_IDENT_TEL3_TYP",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_EMAIL,        "DOSS_IDENT_EMAIL",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NUMSS,        "DOSS_IDENT_NUMSS",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_IS_ASS,       "DOSS_IDENT_IS_ASS",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_GEME,         "DOSS_IDENT_GEME",           &line , err); if (err.length())     goto SetConfBase_Error;           // rang gemellaire
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NOM_ASS,      "DOSS_IDENT_NOM_ASS",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_PRENOM_ASS,   "DOSS_IDENT_PRENOM_ASS",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_NSS_ASS,      "DOSS_IDENT_NSS_ASS",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_PROF,         "DOSS_IDENT_PROF",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_TITRE,        "DOSS_IDENT_TITRE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_IDENT_REF_PK,       "DOSS_IDENT_REF_PK",         &line , err); if (err.length())     goto SetConfBase_Error;

  //................. DOSS_NOTE_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_TBL_NAME,       "DOSS_NOTE_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_PK,             "DOSS_NOTE_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_PAT_GUID,       "DOSS_NOTE_PAT_GUID",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_PAT_PK,         "DOSS_NOTE_PAT_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_NOTE_HTML,           "DOSS_NOTE_HTML",             &line , err); if (err.length())     goto SetConfBase_Error;

  //................. DOSS_INTERVENANTS_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_TBL_NAME,       "DOSS_INTERVENANTS_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_PK,             "DOSS_INTERVENANTS_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_PAT_GUID,       "DOSS_INTERVENANTS_PAT_GUID",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_PAT_PK,         "DOSS_INTERVENANTS_PAT_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_INTERVPK,       "DOSS_INTERVENANTS_INTERVPK",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_INTERVENANTS_TYPE,           "DOSS_INTERVENANTS_TYPE",             &line , err); if (err.length())     goto SetConfBase_Error;

  //................. INTERVENANTS_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_TBL_NAME,       "INTERVENANTS_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_PK,             "INTERVENANTS_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_NOM,            "INTERVENANTS_NOM",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_PRENOM,         "INTERVENANTS_PRENOM",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_SPEC,           "INTERVENANTS_SPEC",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_TITRE,          "INTERVENANTS_TITRE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_RUE,            "INTERVENANTS_RUE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_CODE_POST,      "INTERVENANTS_CODE_POST",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_INTERVENANTS_VILLE,          "INTERVENANTS_VILLE",            &line , err); if (err.length())     goto SetConfBase_Error;

  //............................ CIM10_USER_FAV_TBL_NAME ...........................................................
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_TBL_NAME,     "CIM10_USER_FAV_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_PK,           "CIM10_USER_FAV_PK "  ,          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_USER,         "CIM10_USER_FAV_USER" ,          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_CIM10_USER_FAV_DATA,         "CIM10_USER_FAV_DATA",           &line , err); if (err.length())     goto SetConfBase_Error;

  //............................ DOSS_RUB_HEAD_TBL_NAME ...........................................................
  pt = SetConfBase_SetProperties(pt,  m_DOSS_RUB_HEAD_TBL_NAME,      "DOSS_RUB_HEAD_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PRIMKEY,       "DOSS_RUB_HEAD_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_GUID,          "DOSS_RUB_HEAD_GUID",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_TYPE,          "DOSS_RUB_HEAD_TYPE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_SUBTYPE,       "DOSS_RUB_HEAD_SUBTYPE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_NOM_DATE,      "DOSS_RUB_HEAD_NOM_DATE",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_DATE,          "DOSS_RUB_HEAD_DATE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_USER,          "DOSS_RUB_HEAD_USER",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_SIGN_USER,     "DOSS_RUB_HEAD_SIGN_USER",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_REF_BLOBS_PK,  "DOSS_RUB_HEAD_REF_BLOBS_PK",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_FIN,           "DOSS_RUB_HEAD_FIN",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_DUREE_MOD,     "DOSS_RUB_HEAD_DUREE_MOD",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_REF_DOSS_PK,   "DOSS_RUB_HEAD_REF_DOSS_PK",      &line , err); if (err.length())     goto SetConfBase_Error;

  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_0,        "DOSS_RUB_HEAD_PROP_0",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_1,        "DOSS_RUB_HEAD_PROP_1",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_2,        "DOSS_RUB_HEAD_PROP_2",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_3,        "DOSS_RUB_HEAD_PROP_3",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_HEAD_PROP_4,        "DOSS_RUB_HEAD_PROP_4",           &line , err); if (err.length())     goto SetConfBase_Error;

  //................. DOSS_RUB_DATA_TBL_NAME ......................................................................................
  pt = SetConfBase_SetProperties(pt,  m_DOSS_RUB_DATA_TBL_NAME,      "DOSS_RUB_DATA_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_GUID,          "DOSS_RUB_DATA_GUID",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_PRIMKEY,       "DOSS_RUB_DATA_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_DOSS_RUB_DATA_BLOB,          "DOSS_RUB_DATA_BLOB",             &line , err); if (err.length())     goto SetConfBase_Error;

  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_TBL_NAME,          "EVNT_LIST_TBL_NAME",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_PK,                "EVNT_LIST_PK",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_DOSS_PK,                "EVNT_DOSS_PK",                   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_PERE,              "EVNT_LIST_PERE",                 &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_EVNT_LIST_PKDOC,             "EVNT_LIST_PKDOC",                &line , err); if (err.length())     goto SetConfBase_Error;


  //................. USER_IDENT_TBL_NAME ......................................................................................
  pt = SetConfBase_SetProperties(pt,  m_USER_IDENT_TBL_NAME,      "USER_IDENT_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NOM,           "USER_IDENT_NOM",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PRENOM,        "USER_IDENT_PRENOM",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TITRE,         "USER_IDENT_TITRE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_LOGIN,         "USER_IDENT_LOGIN",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PASSWORD,      "USER_IDENT_PASSWORD",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_DROITS,        "USER_IDENT_DROITS",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_SEXE,          "USER_IDENT_SEXE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NUM_ORDRE,     "USER_IDENT_NUM_ORDRE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NUM_RPPS,      "USER_IDENT_NUM_RPPS",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CLEF_RPPS,     "USER_IDENT_CLEF_RPPS",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PRIMKEY,       "USER_IDENT_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_GUID,          "USER_IDENT_GUID",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CONVENTION,    "USER_IDENT_CONVENTION",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CODE_POST,     "USER_IDENT_CODE_POST",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_VILLE,         "USER_IDENT_VILLE",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_RUE,           "USER_IDENT_RUE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_QUALITE,       "USER_IDENT_QUALITE",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_1,         "USER_IDENT_TEL_1",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_2,         "USER_IDENT_TEL_2",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_3,         "USER_IDENT_TEL_3",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_EMAIL,         "USER_IDENT_EMAIL",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NOTE,          "USER_IDENT_NOTE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CODE_REGIME,   "USER_IDENT_CODE_REGIME",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CHER,          "USER_IDENT_CHER",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_TYPE_1,    "USER_IDENT_TEL_TYPE_1",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_TYPE_2,    "USER_IDENT_TEL_TYPE_2",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_TYPE_3,    "USER_IDENT_TEL_TYPE_3",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_ABR_1,     "USER_IDENT_TEL_ABR_1",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_ABR_2,     "USER_IDENT_TEL_ABR_2",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TEL_ABR_3,     "USER_IDENT_TEL_ABR_3",        &line , err); if (err.length())     goto SetConfBase_Error;

  //................. USER_DATA_TBL_NAME ............................
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_TBL_NAME,      "USER_DATA_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_PK,            "USER_DATA_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_REF_PK,        "USER_DATA_REF_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_NAMEDOC,       "USER_DATA_NAMEDOC",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_TYPE,          "USER_DATA_TYPE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_DATA_BLOB,          "USER_DATA_BLOB",             &line , err); if (err.length())     goto SetConfBase_Error;

  //................. VERROUS ............................
  pt = SetConfBase_SetProperties(pt,   m_VERROU_TBL_NAME,         "VERROU_TBL_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_PK,               "VERROU_PK",                  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_DOSS_PK,          "VERROU_DOSS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_DOSS_GUID,        "VERROU_DOSS_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_RUB_NAME,         "VERROU_RUB_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_RUB_PK,           "VERROU_RUB_PK",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_USER_NAME,        "VERROU_USER_NAME",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_SIGN_USER_NAME,   "VERROU_SIGN_USER_NAME",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_VERROU_DATE_TIME,        "VERROU_DATE_TIME",           &line , err); if (err.length())     goto SetConfBase_Error;

 //................. USER_PERMS ............................
  pt = SetConfBase_SetProperties(pt,  m_USER_PERMS_TBL_NAME,      "USER_PERMS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;       // nom de la table des droits des utilisateurs non prescripteurs
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_PK,            "USER_PERMS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;       // clef primaire de cet enregistrement
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_SIGN_GUID,     "USER_PERMS_SIGN_GUID",      &line , err); if (err.length())     goto SetConfBase_Error;       // Ref au GUID du signataire autorisant cet utilisateur
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_GUID,   "USER_PERMS_FRIEND_GUID",    &line , err); if (err.length())     goto SetConfBase_Error;       // GUID de cet utilisateur
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_DROITS, "USER_PERMS_FRIEND_DROITS",  &line , err); if (err.length())     goto SetConfBase_Error;       // Droits que le signataire attribue a cet utilisateur
  pt = SetConfBase_SetProperties(pt,   m_USER_PERMS_FRIEND_BLOBS,  "USER_PERMS_FRIEND_BLOBS",   &line , err); if (err.length())     goto SetConfBase_Error;       // Configuration des autorisations fines (liste des fichiers autorises avec les droits)

  pt = SetConfBase_SetProperties(pt,   m_CODES_POSTAUX_TBL_NAME,        "CODES_POSTAUX_TBL_NAME",          &line , err); if (err.length())     goto SetConfBase_Error;  // nom de la table des codes postaux
  pt = SetConfBase_SetProperties(pt,   m_AGENDA_MASK_TBL_NAME,          "AGENDA_MASK_TBL_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;  // table des masques agenda
  pt = SetConfBase_SetProperties(pt,   m_AGENDA_TBL_NAME,               "AGENDA_TBL_NAME"  ,               &line , err); if (err.length())     goto SetConfBase_Error;  // table de l'agenda
  pt = SetConfBase_SetProperties(pt,   m_AGENDA_COLOR_PROFILS_TBL_NAME, "AGENDA_COLOR_PROFILS_TBL_NAME" ,  &line , err); if (err.length())     goto SetConfBase_Error;  // table des couleurs de profils agenda
  pt = SetConfBase_SetProperties(pt,   m_VERSION_TBL_NAME,              "VERSION_TBL_NAME" ,               &line , err); if (err.length())     goto SetConfBase_Error;  // table des versions du programme
  return 1;

SetConfBase_Error:
  if (errMess) *errMess += err;
  return 0;

}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *C_DBVarDrTux::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
{int nLine      = *line;
 err            = "";
 long len_token = strlen (token);
 while(*pt)
    {SKIP_BLANK_CAR(pt);
     if (strncmp(pt, token, len_token)==0)
        {//return pt+len_token;
         pt         += len_token;
         err         = "";
         propertie   = "";
         //............. chercher premier delimiteur ...............................
          while(*pt && *pt!='\r' && *pt !='\n' && *pt !='\'' ) pt++;
          if (*pt=='\'')         // si delimiteur pas trouve erreur
             {pt++;               // passer le delimiteur
              char *end = pt;
              //............. chercher deuxieme delimiteur ...............................
              while(*end && *end!='\r' && *end !='\n' && *end !='\'')end++;
              if (*end=='\'')
                 {propertie.setLatin1(pt, end-pt);
                  if (line) *line = nLine;
                  return end +1;
                 }
              else
                {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not find at line Num: ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not find at line Num: ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)

     NEXT_LINE(pt);
     ++nLine;
    }
 err  = TR("\r\nErreur de syntaxe : C_DBVarDrTux::SetConfBase()"
           "\r\nle fichier de configuration des bases 'DataBase.cfg'"
           "\r\ncomporte les erreurs suivantes : \r\n");
 err += TR("      ==> la mention %1 est non trouvee a la ligne Num: %2").arg(token, QString::number(nLine));
 if (line) *line = nLine;
 return pt;
}
