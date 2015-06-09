 /********************************* C_DBVar.cpp ***********************************
 *  #include "C_DBVar.h"                                                          *
 * ...............................................................................*
 *   ProjCURt   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux projCURt                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CCURILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cCURill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la RCURherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de RCURherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CCURILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

// C++ Interface: C_DBVar.cpp
//
#include "C_DBVar.h"
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) ++(a)
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')++(a); while( *(a) && (*(a)=='\r'|| *(a)=='\n'))++(a)

//-----------------------------------------------------  C_DBVar -------------------------------------------
C_DBVar::C_DBVar(  QString& confData)
{m_LastError             = "";
 if (confData.length()==0) {m_LastError +=  TR("\r\n Configuration database file empty or unreachable "); return;}
 initValues(confData);
}
//-----------------------------------------------------  getVersionNumber -------------------------------------------
QString C_DBVar::getVersionNumber()
{return m_VERSION_NUMBER;
}

//-----------------------------------------------------  SetConfBase -------------------------------------------
int C_DBVar::initValues( QString& confData)
{ QByteArray ba = confData.toAscii();
  QString   err = "";
  char     *deb = (char*)(const char *)ba;
  char      *pt = deb;
  int      line = 0;
  pt = SetConfBase_SetProperties(pt,  m_VERSION_NUMBER,    "m_VERSION_NUMBER",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_BO_TBL_NAME,       "m_BO_TBL_NAME",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_BO_PK,             "m_BO_PK",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_BO_LIBELLE,        "m_BO_LIBELLE",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_BO_COLOR_BG,       "m_BO_COLOR_BG",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_BO_COLOR_TITLE,    "m_BO_COLOR_TITLE",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_BO_NB_MAXI_PAT,    "m_BO_NB_MAXI_PAT",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_BO_TYPE,           "m_BO_TYPE",         &line , err); if (err.length())     goto SetConfBase_Error;

  pt = SetConfBase_SetProperties(pt , m_DE_TBL_NAME        , "m_DE_TBL_NAME",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_DE_PK              , "m_DE_PK",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_DE_DESTINATION     , "m_DE_DESTINATION", &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_DE_OUT_TYPE        , "m_DE_OUT_TYPE",    &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_CUR_TBL_NAME    , "m_CUR_TBL_NAME",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_PK          , "m_CUR_PK",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_REF_BOX_PK  , "m_CUR_REF_BOX_PK",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_NOM_MED     , "m_CUR_NOM_MED",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_NOM_PAT     , "m_CUR_NOM_PAT",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_PRENOM_PAT  , "m_CUR_PRENOM_PAT",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_H_ENTREE    , "m_CUR_H_ENTREE",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_H_SORTIE    , "m_CUR_H_SORTIE",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_GUID_PAT    , "m_CUR_GUID_PAT",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_REPLIER     , "m_CUR_REPLIER",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_PROG_ANNEXE , "m_CUR_PROG_ANNEXE", &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_ARGS_ANNEXE , "m_CUR_ARGS_ANNEXE", &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_DESTINATION , "m_CUR_DESTINATION", &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_ANONYME     , "m_CUR_ANONYME",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_NOTE        , "m_CUR_NOTE",        &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_TBL_NAME,        "m_CUR_TASK_TBL_NAME",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_PK,              "m_CUR_TASK_PK",                &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_REF_CUR_PK,      "m_CUR_TASK_REF_CUR_PK",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_CODE,            "m_CUR_TASK_CODE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_NOTE,            "m_CUR_TASK_NOTE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_STATE_CUR,       "m_CUR_TASK_STATE_CUR",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_H_START_EXPECT,  "m_CUR_TASK_H_START_EXPECT",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_H_END_EXPECT,    "m_CUR_TASK_H_END_EXPECT",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_H_START,         "m_CUR_TASK_H_START",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_H_END,           "m_CUR_TASK_H_END",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_PRIOR,           "m_CUR_TASK_PRIOR",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_COMMENT,         "m_CUR_TASK_COMMENT",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_CUR_TASK_AGENT,           "m_CUR_TASK_AGENT",             &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_STATUS_TBL_NAME,          "m_STATUS_TBL_NAME",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_STATUS_PK,                "m_STATUS_PK",                  &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_STATUS_LIBELLE,           "m_STATUS_LIBELLE",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_STATUS_COLOR,             "m_STATUS_COLOR",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_STATUS_FINISHED,          "m_STATUS_FINISHED",            &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_TASK_STATUS_TBL_NAME,     "m_TASK_STATUS_TBL_NAME",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_STATUS_CODE,         "m_TASK_STATUS_CODE",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_STATUS_STATE,        "m_TASK_STATUS_STATE",     &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_TRACE_TBL_NAME,           "m_TRACE_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_PK,                 "m_TRACE_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_REF_BOX_PK,         "m_TRACE_REF_BOX_PK",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_NOM_PAT,            "m_TRACE_NOM_PAT",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_PRENOM_PAT,         "m_TRACE_PRENOM_PAT",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_SIGNER,             "m_TRACE_SIGNER",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_TASK_CODE,          "m_TRACE_TASK_CODE",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_DATETIME,           "m_TRACE_DATETIME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_LIBELLE,            "m_TRACE_LIBELLE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_ACTION,             "m_TRACE_ACTION",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_NUM_CUR,            "m_TRACE_NUM_CUR",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_LIBELLE_STATUS,     "m_TRACE_LIBELLE_STATUS", &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_COMMENT,            "m_TRACE_COMMENT",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TRACE_AGENT,              "m_TRACE_AGENT",          &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_SIGNER_TBL_NAME,          "m_SIGNER_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_SIGNER_PK,                "m_SIGNER_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_SIGNER_CODE,              "m_SIGNER_CODE",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_SIGNER_NOM,               "m_SIGNER_NOM",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_SIGNER_DROITS,            "m_SIGNER_DROITS",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_SIGNER_COLOR,             "m_SIGNER_COLOR",          &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_TASK_TBL_NAME,          "m_TASK_TBL_NAME",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_PK,                "m_TASK_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_LIBELLE,           "m_TASK_LIBELLE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_COLOR,             "m_TASK_COLOR",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_MAX_DURATION,      "m_TASK_MAX_DURATION",   &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_ALARM_COLOR,       "m_TASK_ALARM_COLOR",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_TASK_MENU_BUTTON,       "m_TASK_MENU_BUTTON",    &line , err); if (err.length())     goto SetConfBase_Error;

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
                {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not found at line : ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not found at line : ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)
     NEXT_LINE(pt);
     ++nLine;
    }
 err  = TR("\nSyntax error : C_DBVar::SetConfBase_SetProperties()"
           "\ndatabase configuration file 'DataBase.cfg'"
           "\ncontains following errors: \n"
           "      ==> mention %1 is not found on line : %2").arg(token, QString::number(nLine));
 if (line) *line = nLine;
 return pt;
}
