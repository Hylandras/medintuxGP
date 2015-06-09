 /********************************* CMetaField.cpp **********************************
 \file CMetaField.cpp
 \brief (put some short descripion here).

     #include "CMetaField.h"
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


#include "CMetaField.h"
#include <qregexp.h>
#include "CApp.h"

//------------------------------------------ GetValue ---------------------------------------------------------
QString  CMetaField::GetValue(const QString & input_text, int &curs_pos, int record_id)
{m_LastError = "";
 // m_blob_data = input_text;
 switch (m_IsValid)
   {case 0:               //                      EXTRACT_IN_TEXT (balise de début, balise de fin )
          {//m_ExtractedValues.clear();
           bool curs_pos_valid = TRUE;
           int deb             = input_text.find(m_ArgList[0], curs_pos);
           if (deb==-1) {deb = input_text.find(m_ArgList[0], 0); curs_pos_valid = FALSE;}
           int end = deb;
           if (deb != -1)
              {deb +=  m_ArgList[0].length();
               end  = input_text.find(m_ArgList[1], deb);
               if (end != -1)
                  {if (curs_pos_valid == TRUE ) curs_pos       = end +  m_ArgList[1].length();
                   QString res = input_text.mid(deb,end-deb).stripWhiteSpace();
                   m_ExtractedValues.append( new CMetaValue(m_Name,  res, record_id) );
                   if (res.length()==0)
                      { m_LastError =  TR("WARNING : Champ : \"") + TR(m_ArgList[0]) +"\" \""+ TR(m_ArgList[1]) + TR("\" non renseigné");
                      }
                   return res;
                  }
               else
                  { m_LastError = TR("Balise de fin : \"") + TR(m_ArgList[1]) + TR("\" non trouvée");
                    return QString::null;
                  }
              }
           else
              { m_LastError =  TR("Balise de début : \"") + TR(m_ArgList[0]) + TR("\" non trouvée");
                return QString::null;
              }
          }break;
    case 1:               //                      EXTRACT_IN_LINE (balise de début)
          {//m_ExtractedValues.clear();
           bool curs_pos_valid = TRUE;
           int deb             = input_text.find(m_ArgList[0], curs_pos);
           if (deb==-1) {deb = input_text.find(m_ArgList[0], 0); curs_pos_valid = FALSE;}
           int end = deb;
           if (deb != -1)
              {deb +=  m_ArgList[0].length();
                              end  = input_text.find("\r\n", deb);
               if (end == -1) end  = input_text.find("\r",   deb);
               if (end == -1) end  = input_text.find("\n",   deb);
               if (end != -1)
                  {curs_pos = end;
                   if (curs_pos_valid == TRUE ) {while (curs_pos<input_text.length() && (input_text[curs_pos]=="\r" ||input_text[curs_pos]=="\n")) ++curs_pos;}
                   QString res = input_text.mid(deb,end-deb).stripWhiteSpace();
                   m_ExtractedValues.append( new CMetaValue(m_Name,  res,  record_id) );
                   if (res.length()==0)
                      {m_LastError =  TR("WARNING : Champ : \"") + TR(m_ArgList[0]) + TR("\" non renseigné");
                      }
                   return res;
                  }
               else
                  {m_LastError = TR("Champ : \"") + TR(m_ArgList[0]) + TR("\" fin de ligne non trouvée");
                   return QString::null;
                  }
              }
           else
              {m_LastError =  TR("Balise de début : \"") +TR( m_ArgList[0]) + TR("\" non trouvée");
               return QString::null;
              }
          }break;
    case 2:               //                      FIND_IN_TEXT (valeur à chercher)
          {//m_ExtractedValues.clear();
           int deb = input_text.find(m_ArgList[0]);
           if (deb != -1)
              {m_ExtractedValues.append( new CMetaValue( m_Name,  m_ArgList[0],  record_id) );
               return m_ArgList[0];
              }
           else
              {return QString::null;
              }
          }break;
    case 3:               //                      TERRAIN_GET_ATCD
          {
          }break;
    case 4:               //                      TERRAIN_GET_TTT_FOND
          {
          }break;
    case 5:               //                      LIST_PRESCRIPTION
          {int                f = 0;
           QString          res = "";
           QString  ordoStruct  = "";
           //m_ExtractedValues.clear();
           m_CPosologieOrdoList.clear();
           CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(input_text, 0, &ordoStruct);    // y isoler et recuperer les données calculables
           m_pCMedicaBase->Medica_PosologieListDeserialize(ordoStruct,  m_CPosologieOrdoList);
           QPL_CPosologie::iterator it;
           if (m_ArgList[0].contains ( TR("accessoires"), FALSE) )
              {for (it = m_CPosologieOrdoList.begin(); it != m_CPosologieOrdoList.end(); ++it )
                   {  if ((*it).m_IdTable==5) m_ExtractedValues.append( new CMetaValue( m_Name,
                                                                                        m_pCMedicaBase->Medica_GetMedicamentNameByCIP((*it).m_MEDICA_POSO_CIP, 5),
                                                                                        record_id
                                                                                      )
                                                                      );
                   }
               ++f;
              }
           if (m_ArgList[0].contains (TR("produits"), FALSE) )
              {for (it = m_CPosologieOrdoList.begin(); it != m_CPosologieOrdoList.end(); ++it )
                   {  if ((*it).m_IdTable==1) m_ExtractedValues.append( new  CMetaValue( m_Name,
                                                                                         m_pCMedicaBase->Medica_GetMedicamentNameByCIP((*it).m_MEDICA_POSO_CIP, 1),
                                                                                        record_id
                                                                                       )
                                                                      );
                   }
               ++f;
              }
          if (f==0)
             {for (it = m_CPosologieOrdoList.begin(); it != m_CPosologieOrdoList.end(); ++it )
                   {  m_ExtractedValues.append( new CMetaValue( m_Name,
                                                                m_pCMedicaBase->Medica_GetMedicamentNameByCIP((*it).m_MEDICA_POSO_CIP,(*it).m_IdTable),
                                                                record_id
                                                              )
                                              );
                   }
              }
           return res;
          }break;
    case 14:               //                      EXTRACT_AGE
          {//m_ExtractedValues.clear();
           m_ExtractedValues.append( new CMetaValue( m_Name, input_text, record_id ) );
           return input_text;
          }break;
    case 15:               //                      GET_ALL_VALUE_IN_TEXT
          {//m_ExtractedValues.clear();
           QString   res = "";
           int       deb = input_text.find(m_ArgList[0], curs_pos);
           int       end = input_text.find(m_ArgList[1], deb + m_ArgList[0].length());
           if (deb != -1) deb += m_ArgList[0].length();
           while (deb != -1 && end != -1)
              { res  = input_text.mid(deb, end-deb).stripWhiteSpace();
                m_ExtractedValues.append( new CMetaValue(m_Name, res, record_id)  );
                if (res.length()==0)
                   {m_LastError =  TR("WARNING : Champ : \"") + TR(m_ArgList[0]) +"\" \""+ TR(m_ArgList[1]) + TR("\" non renseigné");
                   }
                deb = input_text.find(m_ArgList[0], end + m_ArgList[1].length());
                if (deb != -1)
                   {deb += m_ArgList[0].length();
                    end = input_text.find(m_ArgList[1], deb );
                   }
              }
            return res;
          }break;
   }
 m_LastError =  TR("fonction : ") + QString::number(m_IsValid) + TR(" non connue");
 return QString::null;
}
//------------------------------------------------------------ isRecordInPositiveList-------------------------------------------------------
bool CMetaField::isRecordInPositiveList(int record_id)
{ for ( int i=0; i<m_PositiveList.count(); ++i )
      { CMetaValue *pCMetaValue = m_PositiveList.at(i);
        if (record_id==pCMetaValue->field_record_id()) return true;
      }
  return false; 
}
//------------------------------------------------------------ GetListOperateur_ID-------------------------------------------------------
int CMetaField::GetListOperateur_ID(QString operateur)
{return QStringList::split("@",TR(OPERATEUR_LIST)).findIndex(operateur);
}

//------------------------------------------------------------ GetListOperateur-------------------------------------------------------
QStringList CMetaField::GetListOperateur(QStringList &opList)
{opList.clear();
 opList = QStringList::split("@",TR(OPERATEUR_LIST));
 return opList;
}

//------------------------------------------ decomposeToken ---------------------------------------------------------
int CMetaField::decomposeToken(const QString &expression, QString &function, QStringList &argList)
{function = "";
 argList.clear();
 QRegExp rx( "[(].*[)]" );
 int len = 0;
 int pos = rx.match ( expression, 0, &len );
 if (pos==-1) return 0;
 QString param = expression.mid(pos+1,len-2).stripWhiteSpace();
 if (param.length())
    {argList       = QStringList::split(',' , param);
     for ( QStringList::Iterator it = argList.begin(); it != argList.end(); ++it )
         {*it = (*it).stripWhiteSpace();
         }
    }
 function      = expression.left(pos).stripWhiteSpace();
 return GetSyntaxID_FromSyntaxString(function);
}

//------------------------------------------------ GetPossiblesRubTypesList --------------------------------------------------------
QStringList CMetaField::GetPossiblesRubTypesList(int strictSqlfield /*=0*/)
{QStringList resultList;
 if (strictSqlfield)      resultList = QStringList::split(",", TR (TYPES_RUB_POSSIBLES));
 else                     resultList = QStringList::split(",", TR (TYPES_POSSIBLES));

 QStringList customList = CApp::get_CustomRubNameList();
 for ( int i=0; i<customList.count(); ++i )
     { QStringList parts = QStringList::split("|",customList[i]);
       resultList << parts[0];
     }
 return resultList;
}

//------------------------------------------------ RubNameToRubType --------------------------------------------------------
QString CMetaField::RubNameToRubType(const QString &name)
{              QString rubName = name.lower();
               if      (rubName==TR("observation"))        return "20030000";
               else if (rubName==TR("prescription vidal")) return "20020100";
               else if (rubName==TR("prescription"))       return "20020200";
               else if (rubName==TR("certificat"))         return "20020300";
               else if (rubName==TR("courrier"))           return "20020500";
               else if (rubName==TR("terrain"))            return "20060000";
               else if (rubName==TR("age patient"))        return "99900000";
               else if (rubName==TR("âge patient"))        return "99900000";
               else{     //    [Rubriques Automatiques]
                         //         1= Biologie|bi|20070000
                         //         2= Imagerie|im|20040000
                     QStringList customList = CApp::get_CustomRubNameList();
                     for ( int i=0; i<customList.count(); ++i )
                         { QStringList parts = QStringList::split("|",customList[i].lower());
                           if (parts[0]==rubName)   return  parts[2];
                         }
                     return  rubName;
                   }
}

//------------------------------------------------ GetSyntaxListFromRubriqueName --------------------------------------------------------
// #define  TYPES_POSSIBLES "Observation,Prescription Vidal,Prescription,Certificat,Courrier,Terrain"

QStringList CMetaField::GetSyntaxListFromRubriqueName(const QString &name, QStringList &list)
{int id = GetPossiblesRubTypesList().findIndex(name);
 if (id >= 7) id = 7;     // Rubriques custom
 switch (id)
   { case 7:              //  Rubriques custom
     case 0:              //  Observation
     case 2:              //  Prescription
     case 3:              //  Certificat
     case 4:              //  Courrier
          list.append(TR("EXTRACT_IN_TEXT ( balise de début, balise de fin )"));
          list.append(TR("EXTRACT_IN_LINE ( balise de début )"));
          list.append(TR("FIND_IN_TEXT ( valeur à chercher )"));
          list.append(TR("GET_ALL_VALUE_IN_TEXT ( balise de début, balise de fin )"));
          break;
     case 1:              //  Prescription Vidal
          list.append(TR("EXTRACT_IN_TEXT ( balise de début, balise de fin )"));
          list.append(TR("EXTRACT_IN_LINE ( balise de début )"));
          list.append(TR("FIND_IN_TEXT ( valeur à chercher )"));
          list.append(TR("GET_ALL_VALUE_IN_TEXT ( balise de début, balise de fin )"));
          //list.append(TR("GET_CIP_CODES ( )"));
          //list.append(TR("GET_PRESCRIPTION_NAME ( )"));
          list.append(TR("LIST_PRESCRIPTION ( Accessoires & Produits )"));
          break;
     case 5:              //  Terrain
          list.append(TR("EXTRACT_VAR ( Nom du tableau , Nom de la variable )"));
          list.append(TR("EXTRACT_ATCD_TEXTE ()"));
          list.append(TR("EXTRACT_ATCD_CIM10_CODE ()"));
          list.append(TR("EXTRACT_ATCD_CIM10_LIBELLE ()"));
          list.append(TR("EXTRACT_ATCD_CIM10_LIBELLE_CODE ()"));
          list.append(TR("EXTRACT_ATCD_ALLERGIQUE_LIBELLE ()"));
          list.append(TR("EXTRACT_ATCD_ALLERGIQUE_CODE ()"));
          list.append(TR("EXTRACT_ATCD_ALLERGIQUE_LIBELLE_CODE ()"));
         break;
     case 6:              //  Age patient
          list.append(TR("EXTRACT_AGE ()"));
         break;
   }
 return list;
}


//------------------------------------------------ GetAllPossiblesFunctionSyntax --------------------------------------------------------
QStringList CMetaField::GetAllPossiblesFunctionSyntax()
{QStringList list;
 list.append(TR("EXTRACT_IN_TEXT ( balise de début, balise de fin )"));
 list.append(TR("EXTRACT_IN_LINE ( balise de début )"));
 list.append(TR("FIND_IN_TEXT ( valeur à chercher )"));
 list.append(TR("GET_CIP_CODES ( )"));
 list.append(TR("GET_PRESCRIPTION_NAME ( )"));
 list.append(TR("LIST_PRESCRIPTION ( Accessoires & Produits )"));
 list.append(TR("EXTRACT_VAR ( Nom du tableau , Nom de la variable )"));
 list.append(TR("EXTRACT_ATCD_TEXTE ()"));
 list.append(TR("EXTRACT_ATCD_CIM10_CODE ()"));
 list.append(TR("EXTRACT_ATCD_CIM10_LIBELLE ()"));
 list.append(TR("EXTRACT_ATCD_CIM10_LIBELLE_CODE ()"));
 list.append(TR("EXTRACT_ATCD_ALLERGIQUE_LIBELLE ()"));
 list.append(TR("EXTRACT_ATCD_ALLERGIQUE_CODE ()"));
 list.append(TR("EXTRACT_ATCD_ALLERGIQUE_LIBELLE_CODE ()"));
 list.append(TR("EXTRACT_AGE ()"));
 list.append(TR("GET_ALL_VALUE_IN_TEXT ( balise de début, balise de fin )"));
 return list;
}

//------------------------------------------------ GetAllPossiblesTokenSyntax --------------------------------------------------------
// #define  TYPES_POSSIBLES "Observation,Prescription Vidal,Prescription,Certificat,Courrier,Terrain"

QStringList CMetaField::GetAllPossiblesTokenSyntax()
{QStringList list;
 list.append(TR("EXTRACT_IN_TEXT"));
 list.append(TR("EXTRACT_IN_LINE"));
 list.append(TR("FIND_IN_TEXT"));
 list.append(TR("GET_CIP_CODES"));
 list.append(TR("GET_PRESCRIPTION_NAME"));
 list.append(TR("LIST_PRESCRIPTION"));
 list.append(TR("EXTRACT_VAR"));
 list.append(TR("EXTRACT_ATCD_TEXTE"));
 list.append(TR("EXTRACT_ATCD_CIM10_CODE"));
 list.append(TR("EXTRACT_ATCD_CIM10_LIBELLE"));
 list.append(TR("EXTRACT_ATCD_CIM10_LIBELLE_CODE"));
 list.append(TR("EXTRACT_ATCD_ALLERGIQUE_LIBELLE"));
 list.append(TR("EXTRACT_ATCD_ALLERGIQUE_CODE"));
 list.append(TR("EXTRACT_ATCD_ALLERGIQUE_LIBELLE_CODE"));
 list.append(TR("EXTRACT_AGE"));
 list.append(TR("GET_ALL_VALUE_IN_TEXT"));
 return list;
}

//------------------------------------------------ TokenSyntaxToFunctionSyntax --------------------------------------------------------
QString CMetaField::TokenSyntaxToFunctionSyntax(const QString &tokenToFind)
{QString      tokenToMatch  = tokenToFind;
 int                   pos  = tokenToMatch.find('(');
 if (pos!=-1) tokenToMatch  = tokenToMatch.left(pos-1);
 tokenToMatch  = tokenToMatch.stripWhiteSpace();

 QStringList     syntaxList = GetAllPossiblesFunctionSyntax();
 int                     id = GetSyntaxID_FromSyntaxString( tokenToMatch );
 if (id != -1) return syntaxList[id];
 return QString::null;
}

//------------------------------------------------ GetSyntaxID_FromSyntaxString --------------------------------------------------------
// #define  TYPES_POSSIBLES "Observation,Prescription Vidal,Prescription,Certificat,Courrier,Terrain"

int CMetaField::GetSyntaxID_FromSyntaxString(const QString &name)
{QString      tokenToMatch  = name;
 int                   pos  = tokenToMatch.find('(');
 if (pos!=-1) tokenToMatch  = tokenToMatch.left(pos-1);
 tokenToMatch  = tokenToMatch.stripWhiteSpace();
 return GetAllPossiblesTokenSyntax().findIndex(tokenToMatch.upper());
}


