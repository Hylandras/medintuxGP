 /********************************* C_Theriaque_DBVar.cpp *************************
 *  #include "C_Theriaque_DBVar.h"                                                *
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

// C++ Interface: C_Theriaque_DBVar.cpp
//

#include "C_Theriaque_DBVar.h"
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) ++(a)
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')++(a); while( *(a) && (*(a)=='\r'|| *(a)=='\n'))++(a)

//-----------------------------------------------------  C_DBVar -------------------------------------------
C_Theriaque_DBVar::C_Theriaque_DBVar()
{m_LastError     =  QObject::tr("\r\n Class C_Theriaque_DBVar  variables are not initialised ");
}

//-----------------------------------------------------  SetConfBase -------------------------------------------
int C_Theriaque_DBVar::initValues( QString& confData)
{ QByteArray ba = confData.toLatin1();
  QString   err = "";
  char     *deb = (char*)(const char *)ba;
  char      *pt = deb;
  int      line = 0;

    pt = SetConfBase_SetProperties(pt,  m_TH_SPECIALITE_TBL_NAME,        "m_TH_SPECIALITE_TBL_NAME",     &line , err); if (err.length())     goto SetConfBase_Error;
    pt = SetConfBase_SetProperties(pt,  m_TH_PRESENTATION_TBL_NAME,      "m_TH_PRESENTATION_TBL_NAME",   &line , err); if (err.length())     goto SetConfBase_Error;
    m_LastError = err;

  return 1;

SetConfBase_Error:
  m_LastError += err;
  return 0;
}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *C_Theriaque_DBVar::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
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
                { propertie= QString::fromLatin1( pt, end-pt );
                  if (line) *line = nLine;
                  return end +1;
                 }
              else
                {err  = QObject::tr("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not find at line : ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
             {err  = QObject::tr("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not find at line : ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)
     NEXT_LINE(pt);
     ++nLine;
    }
 err  = QObject::tr("\nErreur de syntaxe : C_DBVar::SetConfBase_SetProperties()"
                    "\nle fichier de configuration des bases 'DataBase.cfg'"
                    "\ncomporte les erreurs suivantes : \n"
                    "      ==> la mention %1 est non trouv\303\251e \303\240 la ligne : %2").arg(token, QString::number(nLine));
 if (line) *line = nLine;
 return pt;
}

