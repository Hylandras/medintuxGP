 /********************************* C_Fse.cpp *************************************
 \file C_Fse.cpp
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


#include "C_Fse.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "../../SesamVitale-QT3/CSesam_API_InfoGroupe.h"

//-------------------------------- C_Fse -----------------------------------------
C_Fse::C_Fse(CSesam_API* pCSesam_API , unsigned short session  , const QString &codePorteur, const QString &serializeData)
:   m_pCSesam_API(pCSesam_API), m_Session(session), m_CodePorteur(codePorteur), m_FseData(serializeData)
{if (serializeData.length()) UnSerialize(serializeData);
}

//-------------------------------- C_Fse -----------------------------------------
C_Fse::~C_Fse()
{
}

//-------------------------------- SetSession --------------------------------
void C_Fse::SetSession(unsigned short session)
{m_Session = session;
}

//-------------------------------- SetSession --------------------------------
unsigned short C_Fse::GetSession()
{//if (m_Session==-1 && m_pCSesam_API)
 //   {m_pCSesam_API->toLog(TR("Retour SGD_ReserverSession (C_Fse:GetSession) : ")  + sesamAPI.SSV_SSV_ErrCodeToString(m_pCSesam_API->SGD_SGD_ReserverSession(64000, &m_Session)));
 //    m_pCSesam_API->toLog(TR("Numéro de Session (C_Fse:GetSession) : ")           + QString::number(m_Session));
 //   }
 return m_Session;
}

//-------------------------------- GetCodePorteur --------------------------------
QString C_Fse::GetCodePorteur()
{return m_CodePorteur;
}

//-------------------------------- SetCodePorteur --------------------------------
void C_Fse::SetCodePorteur(const QString &codePorteur)
{ m_CodePorteur = codePorteur;
}

//-------------------------------- GetNbOccurences --------------------------------
int C_Fse::GetNbOccurences(unsigned short  iDGroupeCourant)
{int occur       = 1;
 int pos         = 0;
 QString section = QString("FSE_GR-") + (QString::number(iDGroupeCourant)).rightJustify(4,'0') + "-OCC-";
 while ( (pos = m_FseData.find(section + (QString::number(occur)).rightJustify(4,'0'), pos+20)) != -1 )
    {occur++;
    }
 return occur -1;
}

//-------------------------------- GetMember --------------------------------
QString C_Fse::GetMember(int memberIndex, unsigned short  iDGroupeCourant, int occur)
{QString section = sectionValuesToStr(iDGroupeCourant, occur);
 QString member  = QString("m_Num-")  + QString::number(memberIndex);
 QString value   = "";
 if (CGestIni::Param_ReadParam(m_FseData, section, member, &value)==0) return value;
 return QString::null;
}

//-------------------------------- SetMember --------------------------------
int C_Fse::SetMember(int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, size_t /*len_data*/, int occur)
{QString section = sectionValuesToStr(iDGroupeCourant, occur);
 QString member  = QString("m_Num-")  + QString::number(memberIndex);
 QString value   = memberData;
 if (m_FseData.find(section)==-1)
    {m_FseData += "\r\n["+section+"]\r\n";
     m_FseData += TR("; Groupe : ") +
                  QString::number(iDGroupeCourant).rightJustify(4,'0') +
                  "  (" + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant)+ ")\r\n";
    }
 CGestIni::Param_WriteParam(&m_FseData, section, member, value/*, QString::number(memberIndex)*/, CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, memberIndex));
 return 0;
}

//-------------------------------- Serialize --------------------------------
QString C_Fse::sectionValuesToStr(unsigned short  iDGroupeCourant, int occur)
{return QString("FSE_GR-") + (QString::number(iDGroupeCourant)).rightJustify(4,'0') + "-OCC-" + (QString::number(occur)).rightJustify(4,'0');
}

//-------------------------------- Serialize --------------------------------
QString C_Fse::Serialize()
{if (m_FseData.find("[CPS_CODE]")==-1) m_FseData += "\r\n[CPS_CODE]\r\n";
 CGestIni::Param_WriteParam(&m_FseData, "CPS_CODE", "m_CodePorteur", m_CodePorteur, "1");
 return m_FseData ;
}

//-------------------------------- UnSerialize --------------------------------
void C_Fse::UnSerialize(const QString &serializeData)
{     m_FseData = serializeData;
      CGestIni::Param_ReadParam(m_FseData, "CPS_CODE" , "m_CodePorteur", &m_CodePorteur);
}


//-------------------------------- Get_CSV_TableRef --------------------------------
QString C_Fse::Get_CSV_TableRef(const char *csvData,
                                const char *refToMatch,
                                int colToExplore,
                                QStringList &listToReturn,
                                char sepChp /* = '#' */ ,
                                char delStr /* = '|' */)
{listToReturn.clear();
 if (csvData==0)           return TR("Erreur GetTableRef() : csvData pointeur à 0 ");
 if (*csvData==0)          return TR("Erreur GetTableRef() : csvData vide ");
 long  lenToMatch = strlen(refToMatch);
 if (lenToMatch==0)        return TR("Erreur GetTableRef() : refToMatch vide ");

 char *pt         = (char*) (const char*) csvData;
 char *pt_debchp  = pt;
 char *pt_endchp  = 0;
 int     ind_chp  = 0;
 char *pt_debRaw  = pt;
// int     ind_raw  = 0;

 while ( *pt )
   {
    if ( *pt==delStr )                       // délimiteur de début de chaine
       { ++pt;                               // passer   le delimiteur de début de chaine
         pt_debchp = pt;
         while (*pt && *pt != delStr) ++pt;  // charcher le delimiteur de fin de chaine
         if (*pt==0)       return TR("Erreur GetTableRef() : délimiteur de fin de champ non trouvé.");        // erreur de syntaxe : délimiteur de fin de champ non trouvé
         pt_endchp = pt;
         ++pt;                               // passer   le delimiteur de fin de chaine
         //................ ici on a un champ délimité par : pt_debchp (apres delimiteur) et pt_endchp (avant delimiteur)
         if (ind_chp==colToExplore)
            { long      len = pt_endchp - pt_debchp;
              bool   isSame = FALSE;
              if (len == lenToMatch)
                 { isSame        = TRUE;
                   char *ptComp  = (char*) (const char*) pt_debchp;
                   char *ptmatch = (char*) (const char*) refToMatch;
                   while (ptComp < pt_endchp)
                         { if ( *ptComp != *ptmatch) isSame = FALSE;
                           ++ptComp;
                           ++ptmatch;
                         }
                 }
              //................... si trouvé ...............................................
              //                    sortir liste des champs
              if (isSame)
                 {char *pt_endRaw = GotoEndRaw(pt , sepChp , delStr);
                  QString raw  = QString::fromLatin1 ( pt_debRaw, pt_endRaw - pt_debRaw ).remove(delStr);
                  listToReturn = QStringList::split ( sepChp, raw, TRUE );
                  return QString::null;             // erreur vide
                 }
            } //endif (ind_chp==colToExplore)
         //............... ici pt pointe aprés une fin de chaine (apres le delimiteur) ..................
         ++ind_chp;

         if ( *pt != sepChp)
            {ind_chp = 0;
             while (*pt && *pt != delStr) ++pt; // passer les tous les trucs jusqu'à un début de champ
             pt_debRaw = pt;
            }
       } //end if ( *pt==delStr )
    else
       {++pt;
       }
   } // end while ( *pt )
 return QString::null;
}

//-------------------------------- GotoEndRaw --------------------------------
// LA fin d'une rangée est atteinte lorsque il n'est plus trouvé de délimiteur de champ
// aprés un delimiteur de fin ce chaine |champ1|#|champ2|#|champ3||champ1|#|champ2|#|champ3|
// char *pt doit impérativement pointer en dehors d'un champ
// retourne un pointeur placé après le dernier delimiteur de fin de chaine
char* C_Fse::GotoEndRaw(char *pt, char sepChp /* = '#' */ , char delStr /* = '|' */)
{while (*pt)
   {//.............. boucle de recherche du delimiteur de debut ......................
    if (*pt==delStr)
       {++pt;
        //.............. boucle de recherche du delimiteur de fin......................
        while (*pt && *pt!=delStr) ++pt;
        ++pt;
        if (*pt!=sepChp) return pt;
       }
    ++pt;
   }
 return pt;
}
