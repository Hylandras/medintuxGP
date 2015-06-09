/*********************************************************************************
 *                                                                                *
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

#include "C_DBVar.h"

#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) ++(a)
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')++(a); while( *(a) && (*(a)=='\r'|| *(a)=='\n'))++(a)

//-----------------------------------------------------  C_DSVar -------------------------------------------
C_DBVar::C_DBVar(  QString& confData)
{m_LastError             = "";
 if (confData.length()==0) {m_LastError +=  TR("\r\n Fichier de configuration des bases absent ou vide "); return;}
 initValues(confData);
}

//-----------------------------------------------------  SetConfBase -------------------------------------------
int C_DBVar::initValues( QString& confData)
{ QByteArray ba = confData.toAscii();
  QString   err = "";
  char     *deb = (char*)(const char *)ba;
  char      *pt = deb;
  int      line = 0;
    pt = SetConfBase_SetProperties(pt,  m_VERSION,                   "m_VERSION",                  &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SCHEMA_NAME,               "m_SCHEMA_NAME",              &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_LABO_EXAMENS_TBL_NAME,     "m_LABO_EXAMENS_TBL_NAME",    &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_LABO_EXAMENS_REF_PK,       "m_LABO_EXAMENS_REF_PK",      &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_LABO_EXAMENS_PK,           "m_LABO_EXAMENS_PK",          &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_LABO_EXAMENS_OBX_HPRIM,    "m_LABO_EXAMENS_OBX_HPRIM",   &line , err); if (err.length())     goto SetConfBase_Error;

    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_TBL_NAME,      "m_LABO_PATIENTS_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_PK,            "m_LABO_PATIENTS_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_SERVICE,       "m_LABO_PATIENTS_SERVICE",        &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_NB_OBX,        "m_LABO_PATIENTS_NB_OBX",         &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_NOM_EXPED,     "m_LABO_PATIENTS_NOM_EXPED",      &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_CODE_EXPED,    "m_LABO_PATIENTS_CODE_EXPED",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_NOM_DESTI,     "m_LABO_PATIENTS_NOM_DESTI",      &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_CODE_DESTI,    "m_LABO_PATIENTS_CODE_DESTI",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_DATE_EMISSION, "m_LABO_PATIENTS_DATE_EMISSION",  &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_HEURE_EMISSION,"m_LABO_PATIENTS_HEURE_EMISSION", &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_NOM,           "m_LABO_PATIENTS_NOM",            &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_PRENOM,        "m_LABO_PATIENTS_PRENOM",         &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_DATENSS,       "m_LABO_PATIENTS_DATENSS",        &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_SEXE,          "m_LABO_PATIENTS_SEXE",           &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_DATE_EXAMEN,   "m_LABO_PATIENTS_DATE_EXAMEN",    &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_HEURE_EXAMEN,  "m_LABO_PATIENTS_HEURE_EXAMEN",   &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_HPRIMSEP,      "m_LABO_PATIENTS_HPRIMSEP",       &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_DEF_EXAMEN,    "m_LABO_PATIENTS_DEF_EXAMEN",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_PATIENTS_FILENAME_REF,  "m_LABO_PATIENTS_FILENAME_REF",   &line , err); if (err.length())     goto SetConfBase_Error;

    pt = SetConfBase_SetProperties(pt , m_LABO_MINMAX_TBL_NAME,        "m_LABO_MINMAX_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_MINMAX_PK,              "m_LABO_MINMAX_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_MINMAX_EXAMEN,          "m_LABO_MINMAX_EXAMEN",           &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_MINMAX_SEXE,            "m_LABO_MINMAX_SEXE",             &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_MINMAX_MAX,             "m_LABO_MINMAX_MAX",              &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt , m_LABO_MINMAX_MIN,             "m_LABO_MINMAX_MIN",              &line , err); if (err.length())     goto SetConfBase_Error;

  return 1;

SetConfBase_Error:
  m_LastError += err;
  return 0;
}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *C_DBVar::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
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
                { propertie= QString::fromAscii ( pt, end-pt );
                  if (line) *line = nLine;
                  return end +1;
                 }
              else
                {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not find at line : ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not find at line : ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)
     NEXT_LINE(pt);
     ++nLine;
    }
 err  = TR("\nErreur de syntaxe : C_DBVar::SetConfBase_SetProperties()"
           "\nle fichier de configuration des bases 'DataBase.cfg'"
           "\ncomporte les erreurs suivantes : \n"
           "      ==> la mention %1 est non trouv\303\251e \303\240 la ligne : %2").arg(token, QString::number(nLine));
 if (line) *line = nLine;
 return pt;
}
