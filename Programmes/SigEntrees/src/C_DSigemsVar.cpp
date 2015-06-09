 /********************************* C_DSVar.cpp ***********************************
 *  #include "C_DSVar.h"                                                          *
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
 *   Commissariat � l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F�d�ration, 75752 PARIS cedex 15.   *
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

// C++ Interface: C_DSVar.cpp
//
#include "C_DSigemsVar.h"
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) ++(a)
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')++(a); while( *(a) && (*(a)=='\r'|| *(a)=='\n'))++(a)

//-----------------------------------------------------  C_DSVar -------------------------------------------
C_DSigemsVar::C_DSigemsVar(  QString& confData)
{m_LastError             = "";
 if (confData.length()==0) {m_LastError +=  QObject::tr("\n '%1' \nFichier de configuration des bases absent ou vide ").arg(Q_FUNC_INFO); return;}
 initValues(confData);
}

//-----------------------------------------------------  SetConfBase -------------------------------------------
int C_DSigemsVar::initValues( QString& confData)
{ QByteArray ba = confData.toAscii();
  QString   err = "";
  char     *deb = (char*)(const char *)ba;
  char      *pt = deb;
  int      line = 0;

    pt = SetConfBase_SetProperties(pt,  m_VERSION,                  "VERSION",                  &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SCHEMA_NAME,              "SCHEMA_NAME",              &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENTREES_TBL_NAME,     "SIG_ENTREES_TBL_NAME",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_PK,               "SIG_ENT_PK",               &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_NOM,              "SIG_ENT_NOM",              &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_PRENOM,           "SIG_ENT_PRENOM",           &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_SEXE,             "SIG_ENT_SEXE",             &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_DATE_NSS,         "SIG_ENT_DATE_NSS",         &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_NUM_SS,           "SIG_ENT_NUM_SS",           &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_DATE_ENTREE,      "SIG_ENT_DATE_ENTREE",      &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_NOM_FIC_HPRIM,    "SIG_ENT_NOM_FIC_HPRIM",    &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_GUID_PATIENT,     "SIG_ENT_GUID_PATIENT",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_QUALITE,          "SIG_ENT_QUALITE",          &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_RANG_GEME,        "SIG_ENT_RANG_GEME",        &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_BLOB_HPR,         "SIG_ENT_BLOB_HPR",         &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_DATE_PEC,         "SIG_ENT_DATE_PEC",         &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_SIG_ENT_NUM_PAT_SIGEMS,   "SIG_ENT_NUM_PAT_SIGEMS",   &line , err); if (err.length())     goto SetConfBase_Error;

    // Table des rubriques pour test si patient pris en charge.
    pt = SetConfBase_SetProperties(pt,  m_DOSS_RUB_HEAD_TBL_NAME,     "DOSS_RUB_HEAD_TBL_NAME", &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_DOSS_RUB_HEAD_GUID,         "DOSS_RUB_HEAD_GUID",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_DOSS_RUB_HEAD_DATE,         "DOSS_RUB_HEAD_DATE",     &line , err); if (err.length())     goto SetConfBase_Error;

    // Table des index nomprenom pour test si patient existe dans drtux
    pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_TBL_NAME,     "DOSS_INDEX_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_GUID,         "DOSS_INDEX_GUID",           &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_NOM,          "DOSS_INDEX_NOM",            &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_DOSS_INDEX_PRENOM,       "DOSS_INDEX_PRENOM",         &line , err); if (err.length())     goto SetConfBase_Error;

  return 1;

SetConfBase_Error:
  m_LastError += err;
  return 0;
}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *C_DSigemsVar::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
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
                {err  = QObject::tr("Syntax Error: %1\r\n End delimiter \"'\"  not find at line : ").arg(Q_FUNC_INFO);
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = QObject::tr("Syntax Error: %1\r\n Start delimiter \"'\"  not find at line : ").arg(Q_FUNC_INFO);
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)
     NEXT_LINE(pt);
     ++nLine;
    }
 err  = QObject::tr("\nErreur de syntaxe : %3"
                    "\nle fichier de configuration des bases 'DataBase.cfg'"
                    "\ncomporte les erreurs suivantes : \n"
                    "      ==> la mention %1 est non trouv\303\251e \303\240 la ligne : %2").arg(token, QString::number(nLine),Q_FUNC_INFO);
 if (line) *line = nLine;
 return pt;
}
