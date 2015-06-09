 /********************************* CMetaFiltre.cpp **********************************
 \file CMetaFiltre.cpp
 \brief (put some short descripion here).

     #include "CMetaFiltre.h"
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


#include "CMetaFiltre.h"


//===================================================== CMetaFiltreLine =====================================================

//---------------------------------------------- CMetaFiltreLine --------------------------------------------------------------
CMetaFiltreLine::CMetaFiltreLine(      PRT_LIST_CMetaField* pCMetaFieldList,
                                 const QString &metaFieldName,
                                 const QString &operateurDeTest,
                                 const QString &refValue,
                                 const QString &logicOp,
                                 const QString &typeToConvert)
{ CMetaField *pCMetaField;
  m_TestResult         = FALSE;
  m_pCMetaField        = 0;
  m_Name               = metaFieldName;
  m_logicOpID          = CMetaFiltreLine::GetListLogicOper_ID(logicOp);
  m_OpCode             = CMetaField::GetListOperateur_ID(operateurDeTest);
  m_IsValid            = FALSE;

  for ( pCMetaField    = pCMetaFieldList->first(); pCMetaField; pCMetaField = pCMetaFieldList->next() )
      { if (m_Name == pCMetaField->m_Name)    { m_pCMetaField = pCMetaField;  break;  }  // on a trouve l'indice du metachamp dans la liste des meta-champs
      } // endfor ( pCMetaField   = m_CMetaFieldList.first();
  if (m_pCMetaField == 0)  return;
  SetRefValueToCorrectConversion(refValue, typeToConvert);
}

//---------------------------------------------- DoTest -----------------------------------------------------------------------
//  recupere la valeur du meta champ directement dans le metachamp et la teste avec le test
//      #define OPERATEUR_LIST "Vide#>#<#<=#">=#est égal#contient#commence par#finit par#est différent de#ne contient pas"
bool CMetaFiltreLine::DoTest()
{ return DoTest(m_pCMetaField, m_OpCode, m_ConvID);
}

bool CMetaFiltreLine::DoTest(CMetaField *pCMetaField, int op_code, int conv_id)
{if (pCMetaField == 0) return 0;
 int ret       = FALSE;
 int result    = FALSE;
 int   nb      = pCMetaField->m_ExtractedValues.count();
                 pCMetaField->m_PositiveList.clear();
 for (int i=0; i<nb; ++i)
     { QString  fieldValue = pCMetaField->m_ExtractedValues.at(i)->field_value();
       switch (op_code)
          {case 0:                //   Vide pas de test on retourne TRUE si champ plein FALSE si vide
                                    ret = (fieldValue.length()>0);
                break;
           case 1:                // >
                switch (conv_id)
                        {case 0:    ret =  (fieldValue                            > m_RefQStringValue);    break;    //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper()                    > m_RefQStringValue);    break;    //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower()                    > m_RefQStringValue);    break;    //   texte (mis en Minuscules)
                         case 3:    ret =  (fieldValue.toLong()                   > m_RefLongValue);       break;    //   Numerique entier
                         case 4:    ret =  (fieldValue.toFloat()                  > m_RefFloatValue);      break;    //   flotant  Qt::ISODate
                         case 5:{                                                                                    //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                                 QDateTime qdt;
                                 if (StringDateNaturalToQDateTime(fieldValue, qdt))
                                    ret =  (qdt                                   > m_RefQDateTimeValue);
                                 else
                                    ret =  FALSE;
                                }                                                                          break;
                         case 6:    ret =  (HeureMinuteSeconde_To_Val(fieldValue) > m_RefLongValue);       break;    //   Heure
                         default:   ret =  FALSE;                                                          break;    //   si le test est inconnu
                        }
                break;
           case 2:                // <
                switch (conv_id)
                        {case 0:    ret =  (fieldValue                            < m_RefQStringValue);    break;    //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper()                    < m_RefQStringValue);    break;    //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower()                    < m_RefQStringValue);    break;    //   texte (mis en Minuscules)
                         case 3:    ret =  (fieldValue.toLong()                   < m_RefLongValue);       break;    //   Numerique entier
                         case 4:    ret =  (fieldValue.toFloat()                  < m_RefFloatValue);      break;    //   flotant  Qt::ISODate
                         case 5:{                                                                                    //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                                 QDateTime qdt;
                                 if (StringDateNaturalToQDateTime(fieldValue, qdt))
                                    ret =  (qdt                                   < m_RefQDateTimeValue);
                                 else
                                    ret =  FALSE;
                                }                                                                          break;
                         case 6:    ret =  (HeureMinuteSeconde_To_Val(fieldValue)    < m_RefLongValue);    break;     //   Heure
                         default:   ret =  FALSE;                                                          break;     //   si le test est inconnu
                        }
                break;
           case 3:                // <=
                switch (conv_id)
                        {case 0:    ret =  (fieldValue                            <= m_RefQStringValue);   break;     //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper()                    <= m_RefQStringValue);   break;     //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower()                    <= m_RefQStringValue);   break;     //   texte (mis en Minuscules)
                         case 3:    ret =  (fieldValue.toLong()                   <= m_RefLongValue);      break;     //   Numerique entier
                         case 4:    ret =  (fieldValue.toFloat()                  <= m_RefFloatValue);     break;     //   flotant  Qt::ISODate
                         case 5:{                                                                                     //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                                 QDateTime qdt;
                                 if (StringDateNaturalToQDateTime(fieldValue, qdt))
                                    ret =  (qdt                                   <= m_RefQDateTimeValue);
                                 else
                                    ret =  FALSE;
                                }                                                                          break;
                         case 6:    ret =  (HeureMinuteSeconde_To_Val(fieldValue)    <= m_RefLongValue);   break;     //   Heure
                         default:   ret =  FALSE;                                                          break;     //   si le test est inconnu
                        }
                break;
           case 4:                // >=
                switch (conv_id)
                        {case 0:    ret =  (fieldValue                            >= m_RefQStringValue);   break;     //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper()                    >= m_RefQStringValue);   break;     //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower()                    >= m_RefQStringValue);   break;     //   texte (mis en Minuscules)
                         case 3:    ret =  (fieldValue.toLong()                   >= m_RefLongValue);      break;     //   Numerique entier
                         case 4:    ret =  (fieldValue.toFloat()                  >= m_RefFloatValue);     break;     //   flotant  Qt::ISODate
                         case 5:{                                                                                     //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                                 QDateTime qdt;
                                 if (StringDateNaturalToQDateTime(fieldValue, qdt))
                                    ret =  (qdt                                   >= m_RefQDateTimeValue);
                                 else
                                    ret =  FALSE;
                                }                                                                          break;
                         case 6:    ret =  (HeureMinuteSeconde_To_Val(fieldValue)    >= m_RefLongValue);   break;      //   Heure
                         default:   ret =  FALSE;                                                          break;      //   si le test est inconnu
                        }
                break;
           case 5:                // est égal
                switch (conv_id)
                        {case 0:    ret =  (fieldValue                            == m_RefQStringValue);   break;     //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper()                    == m_RefQStringValue);   break;     //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower()                    == m_RefQStringValue);   break;     //   texte (mis en Minuscules)
                         case 3:    ret =  (fieldValue.toLong()                   == m_RefLongValue);      break;     //   Numerique entier
                         case 4:    ret =  (fieldValue.toFloat()                  == m_RefFloatValue);     break;     //   flotant  Qt::ISODate
                         case 5:{                                                                                     //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                                 QDateTime qdt;
                                 if (StringDateNaturalToQDateTime(fieldValue, qdt))
                                    ret =  (qdt                                   == m_RefQDateTimeValue);
                                 else
                                    ret =  FALSE;
                                }                                                                          break;
                         case 6:    ret =  (HeureMinuteSeconde_To_Val(fieldValue)    == m_RefLongValue);   break;      //   Heure
                         default:   ret =  FALSE;                                                          break;      //   si le test est inconnu
                        }
                break;
           case 6:                // contient
                switch (conv_id)
                        {case 0:    ret =  (fieldValue.find(m_RefQStringValue) != -1 );                    break;      //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper().find(m_RefQStringValue) != -1);             break;      //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower().find(m_RefQStringValue) != -1);             break;      //   texte (mis en Minuscules)
                         default:   ret =  FALSE;                                                          break;      //   si le test est inconnu
                        }
                break;
           case 7:                // commence
                switch (conv_id)
                        {case 0:    ret =  (fieldValue.startsWith(m_RefQStringValue)  );                   break;    //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper().startsWith(m_RefQStringValue) );            break;    //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower().startsWith(m_RefQStringValue) );            break;    //   texte (mis en Minuscules)
                         default:   ret =  FALSE;                                                          break;    //   si le test est inconnu
                        }
                break;
           case 8:                // finit par
                switch (conv_id)
                        {case 0:    ret =  (fieldValue.endsWith (m_RefQStringValue)  );                    break;    //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper().endsWith(m_RefQStringValue) );              break;    //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower().endsWith(m_RefQStringValue) );              break;    //   texte (mis en Minuscules)
                         default:   ret =  FALSE;                                                          break;    //   si le test est inconnu
                        }
                break;
           case 9:                // est différent de
                switch (conv_id)
                        {case 0:    ret =  (fieldValue                            != m_RefQStringValue);   break;     //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper()                    != m_RefQStringValue);   break;     //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower()                    != m_RefQStringValue);   break;     //   texte (mis en Minuscules)
                         case 3:    ret =  (fieldValue.toLong()                   != m_RefLongValue);      break;     //   Numerique entier
                         case 4:    ret =  (fieldValue.toFloat()                  != m_RefFloatValue);     break;     //   flotant  Qt::ISODate
                         case 5:{                                                                                     //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                                 QDateTime qdt;
                                 if (StringDateNaturalToQDateTime(fieldValue, qdt))
                                    ret =  (qdt                                   != m_RefQDateTimeValue);
                                 else
                                    ret =  FALSE;
                                }                                                                         break;
                         case 6:    ret =  (HeureMinuteSeconde_To_Val(fieldValue)    != m_RefLongValue);  break;         //   Heure
                         default:   ret =  FALSE;                                                         break;         //   si le test est inconnu
                        }
                break;
           case 10:               // ne contient pas
                switch (conv_id)
                        {case 0:    ret =  (fieldValue.find(m_RefQStringValue) == -1 );                   break;    //   Texte (sans modification)
                         case 1:    ret =  (fieldValue.upper().find(m_RefQStringValue) == -1);            break;    //   TEXTE (mis en Majuscules)
                         case 2:    ret =  (fieldValue.lower().find(m_RefQStringValue) == -1);            break;    //   texte (mis en Minuscules)
                         default:   ret =  FALSE;                                                         break;    //    si le test est inconnu
                        }
                break;
          } // end switch (m_test)
       result |= ret;
       if (ret) { pCMetaField->m_PositiveList.append (new CMetaValue(   pCMetaField->m_ExtractedValues.at(i)->field_name(),
                                                                        pCMetaField->m_ExtractedValues.at(i)->field_value(),
                                                                        pCMetaField->m_ExtractedValues.at(i)->field_record_id()
                                                                    )
                                                     );
                }
     } //end for (int i=0; i<nb; ++i)
  return result;
}

//---------------------------------------------- SetRefValueToCorrectConversion -----------------------------------------------------------------------
//      #define CONV_TYPE_LIST "Texte (sans modification#TEXTE (mis en Majuscules)#texte (mis en Minuscules)#Numerique entier#flotant#Date#Heure#Temps (Date Heure)"
void CMetaFiltreLine::SetRefValueToCorrectConversion(const QString &refValue, const QString &typeToConvert)
{  if (typeToConvert.length())   m_ConvID  = GetListConversion_ID(typeToConvert);
   else                          m_ConvID  = 0;
   if (m_ConvID<0 || m_ConvID>6) m_IsValid = FALSE;
   else                          m_IsValid = TRUE;
   switch (m_ConvID)
          {case 0:                //   Texte (sans modification)
                m_RefQStringValue   = refValue;
                break;
           case 1:                //   TEXTE (mis en Majuscules)
                m_RefQStringValue   = refValue.upper();
                break;
           case 2:                //   texte (mis en Minuscules)
                m_RefQStringValue   = refValue.lower();
                break;
           case 3:                //   Numerique entier
                m_RefLongValue      = refValue.toLong();
                break;
           case 4:                //   flotant  Qt::ISODate
                m_RefFloatValue     = refValue.toFloat();
                break;
           case 5:                //   Date                 Qt::TextDate, the string format is HH:MM:SS; e.g. 1 second before midnight would be "23:59:59".
                m_RefQDateTimeValue = QDateTime::fromString(typeToConvert, Qt::ISODate); // QDate::fromString ( const QString & s, Qt::DateFormat f = Qt::TextDate )
                break;
           case 6:                //   Heure les heures converties en secondes
                m_RefLongValue      = HeureMinuteSeconde_To_Val(refValue); // QDate::fromString ( const QString & s, Qt::DateFormat f = Qt::TextDate )
                break;
           }
}

//-------------------------------------------------------------- StringDateNaturalToQDateTime --------------------------------------------------------------------
 int CMetaFiltreLine::StringDateNaturalToQDateTime(const QString &str_in, QDateTime & qdt_out)
{int pos = 0;
 QString day   = ExtraitChiffre(str_in, pos);
 if (day.length()==0)    return 0;
 QString month = ExtraitChiffre(str_in, pos);
 if (month.length()==0)  return 0;
 QString year = ExtraitChiffre(str_in, pos);
 if (year.length()==0)   return 0;
 if (year.length()==2)  year.prepend("20");  // corriger style abrégé 01/01/05
 qdt_out = QDateTime::fromString (year+"-"+month+"-"+day+"T00:00:00", Qt::ISODate );           //YYYY-MM-DDTHH:MM:SS
 return 1;
}

//-------------------------------------------------------------- ExtraitChiffre --------------------------------------------------------------------
// ACTION: à partir d'un pointeur texte quelconque va jusqu'au premier chiffre
//         rencontré, et tant qu'il y a des chiffres les collecte et retourne
//         dans long *result la valeur de ces chiffres.
// ENTREE: char **pt_t: pointe sur le pointeur de texte du texte.
//                      en sortie il pointe sur le caractère situé apres le chiffre
//                      si un chiffre à ete trouvé sinon il ne change pas
//         long *result: le resultat trouvé sera placé là
//         char *end:  pointeur sur la fin du texte. si il est à zero alors il sera
//                     considéré que la fin du texte est le zero de fin.
// RETOUR: QString : qui est le chiffre trouvé

QString CMetaFiltreLine::ExtraitChiffre(const QString &str_in, int &pos)
{QString result = "";
 long       len = str_in.length();
 while (pos<len &&  (str_in[pos] != '\r' && str_in[pos] != '\n'  && !(str_in[pos]>='0' || str_in[pos]>='9') )) ++pos;              // passser tout ce qui n'est pas nombre
 if (str_in[pos] == '\r' || str_in[pos] == '\n') return result;
 while (pos<len)
   {if (str_in[pos]>='0' || str_in[pos]>='9')    result += str_in[pos];        // passser tout ce qui n'est pas nombre
    else                                         return result;
    ++pos;
   }
 return result;
}

//-------------------------------------------------------------- HeureMinuteSeconde_To_Val --------------------------------------------------------------------
long CMetaFiltreLine::HeureMinuteSeconde_To_Val(const char* heure_in)
{char heure[7];
 char buf[3];
 if (strlen(heure_in)==1)          // si 1 chiffres alors completer les secondes et minutes
    {strcpy(heure, "000000");
     memcpy(heure+1, heure_in,1);
    }
 if (strlen(heure_in)==2)          // si 2 chiffres alors completer les secondes et minutes
    {memcpy(heure, heure_in,2);
     strcpy(heure+2, "0000");
    }
 if (strlen(heure_in)==4)          // si 4 chiffres alors completer les secondes
    {memcpy(heure, heure_in,4);
     strcpy(heure+4, "00");
    }
 else if (strlen(heure_in)==6)     // si 6 chiffres tout prendre
    {memcpy(heure, heure_in,6);
     heure[6]=0;
    }
 else                              // si rien de tout cela cassos
    {return -1;
    }
 long hv,mv,sv;
 //.......... heure ...........
 memcpy(buf, heure_in,   2);
 buf[2]=0;
 hv=atol(buf);

 memcpy(buf, heure_in+2, 2);
 buf[2]=0;
 mv=atol(buf);

 memcpy(buf, heure_in+4, 2);
 buf[2]=0;
 sv=atol(buf);

 return  hv*3600+mv*60+sv;
}


//-------------------------------------- ExtractNumber ----------------------------------------------------------------
QString CMetaFiltreLine::ExtractNumber(const QString &text)
{QString str("");
 int end = text.length();
 int   i = -1;
 while (++i < end)
       {if (  (text[i]>='0' && text[i]<='9') ||  text[i]<='.')     str += text[i];
       }
 return str;
}

//------------------------------------------------------------ GetListConversion_ID-------------------------------------------------------
int CMetaFiltreLine::GetListConversion_ID(QString operateur)
{return QStringList::split("@",TR(CONV_TYPE_LIST)).findIndex(operateur);
}

//------------------------------------------------------------ GetListConversion-------------------------------------------------------
QStringList CMetaFiltreLine::GetListConversion(QStringList &opList)
{opList.clear();
 opList = QStringList::split("@",TR(CONV_TYPE_LIST));
 return opList;
}

//------------------------------------------------------------ GetListLogicOper_ID-------------------------------------------------------
int CMetaFiltreLine::GetListLogicOper_ID(QString operateur)
{return QStringList::split("@",TR(LOGIC_OP_LIST)).findIndex(operateur);
}

//------------------------------------------------------------ GetListLogicOper-------------------------------------------------------
QStringList CMetaFiltreLine::GetListLogicOper(QStringList &opList)
{opList.clear();
 opList = QStringList::split("@",TR(LOGIC_OP_LIST));
 return opList;
}



//===================================================== CMetaFiltre =================================================================================
//          delimiteur logique | nom   metachamp       |   operateur de test    |  valeur de reference à tester | operateur logique avec champ suivant
//----------------------------------------------------------------------------------------------------------------------------------------------------
//                (            ,  circoonstances :     ,    contient            ,  scolaire                     ,      et
//                             ,  sexe                 ,        =               ,    F                          ,
//                ) et (       ,  Mode Arrivee :       ,    contient            ,  pompier                      ,      ou
//                             ,  Mode Arrivee :       ,    contient            , samu                          ,
//                )
//
//
//----------------------------------------------------- CMetaFiltre ---------------------------------------------------------------------------------
CMetaFiltre::CMetaFiltre(  PRT_LIST_CMetaField* pCMetaFieldList, QListView * pQListViewFiltreDef )
{m_pCMetaFieldList           = pCMetaFieldList;  // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
 m_pQListViewFiltreDef       = pQListViewFiltreDef;
 m_CMetaFiltreLineList.setAutoDelete(TRUE);
 m_CMetaFiltreLineList.clear();
 QListViewItemIterator it( m_pQListViewFiltreDef );
 CMetaFiltreLine *pCMetaFiltreLine     = 0;
 CMetaFiltreLine *pLastCMetaFiltreLine = 0;
 int                     delimiteur_ID = 0;
 while ( it.current() )
       { QListViewItem     *pQListViewItem = it.current();
         // #define DELIMITER_TYPE_LIST  "  Vide #  ) #  ( #  ) et (#  ) ou ("
         if ( (delimiteur_ID=GetDelimiterList_ID( pQListViewItem->text(0) )) == 1) break;          //  delimiteur de fin ')'
         //......................................................................................
         pCMetaFiltreLine = new CMetaFiltreLine( pCMetaFieldList,
                                                 pQListViewItem->text(1),
                                                 pQListViewItem->text(2),
                                                 pQListViewItem->text(3),
                                                 pQListViewItem->text(4),
                                                 pQListViewItem->text(5));
          //..........si le delimiteur logique alors placer en negatif son indice dans le dernier m_OpCode rencontré ............
          //          pour le différencier des opérateurs logiques avec champ suivant
          if (pLastCMetaFiltreLine && delimiteur_ID==3||delimiteur_ID==4) pLastCMetaFiltreLine->m_logicOpID = -delimiteur_ID; // on met les délimiteurs logiques en négatif
          if (pCMetaFiltreLine)
             {if (pCMetaFiltreLine->m_IsValid)
                 { m_CMetaFiltreLineList.append( pCMetaFiltreLine );
                   pLastCMetaFiltreLine = pCMetaFiltreLine;
                 }
              else
                 { delete pCMetaFiltreLine;
                 }
             }
         ++it;
       }
}

//------------------------------------------------------------ DoTest-------------------------------------------------------
bool CMetaFiltre::DoTest()
{CMetaFiltreLine *pCMetaFiltreLine;
 CMetaFiltreLine *pLastCMetaFiltreLine = 0;
 int              result = TRUE;   // à priori si le filtre n'est pas fonctionnel il laisse tout passer
 QValueStack<int> stack;

 for ( pCMetaFiltreLine = m_CMetaFiltreLineList.first(); pCMetaFiltreLine; pCMetaFiltreLine = m_CMetaFiltreLineList.next() )
     { //............................... si lignes precedentes action selon operateur logique ..............................
       //                                soit operateur logique avec champ suivant
       //                                soit delimiteur logique
       if ( pLastCMetaFiltreLine )
          { if (pLastCMetaFiltreLine->m_logicOpID > 0) //............. operateur logique avec le champ suivant....................
               {//#define LOGIC_OP_LIST        "  Vide #  et               #  ou               #  sans             "
                switch(pLastCMetaFiltreLine->m_logicOpID)
                  {case 1:   result &=  pCMetaFiltreLine->DoTest(); break;
                   case 2:   result |=  pCMetaFiltreLine->DoTest(); break;
                   default:  result |=  pCMetaFiltreLine->DoTest(); break;       // par defaut ou est plus permissif
                  }
               }
            else                                    // ............. delimiteur logique (négatif)................................
               {stack.push( result );                              // empiler la valeur d'avant
                stack.push( pLastCMetaFiltreLine->m_logicOpID );   // empiler l'operateur logique avec la valeur suivante
                result =  pCMetaFiltreLine->DoTest();              // noter la valeur suivante
               }
          }
        else //............................. première ligne (on note la valeur) ................................................
          { result =  pCMetaFiltreLine->DoTest();
          }
       pLastCMetaFiltreLine = pCMetaFiltreLine;
     }
 stack.push( result );

 //................... depiler la pile et appliquer les operations logiques ......................................
 //   valeur opérateur valeur opérateur valeur.
 int opCode   = 0;
 int isValue  = 1;
 while ( ! stack.isEmpty() )
    { if (isValue)   // c'est Valeur qui sort
         {if (opCode==0)                    //=> au premier coup l'op code est forcément à zero et la dernière valeur empilée est récuperée directement
             {result = stack.pop();
             }
          else                              //=> ensuite il y a un op code à appliquer sur la valeur précedente
             {switch(opCode)
                 {case -3:   result &= stack.pop(); break;    //   ) et (
                  case -4:   result |= stack.pop(); break;    //   ) ou (
                  default:   result |= stack.pop(); break;    // par defaut ou est plus permissif
                 }
             }
         }
      else          // c'est l'op code qui sort (le noter)
         {opCode = stack.pop();
         }
      isValue ^= 1;         // inverser l'indicateur de type de valeur
    }
 return result;
}
//------------------------------------------------------------ recordTestState-------------------------------------------------------
bool CMetaFiltre::recordTestState( int record_id )
{
  //........... cet enregistrement est il concerne par un test positif ...............
  CMetaField *pCMetaField = 0;
  for ( pCMetaField   = m_pCMetaFieldList->first(); pCMetaField; pCMetaField = m_pCMetaFieldList->next() )
      { if ( pCMetaField->isRecordInPositiveList( record_id ) ) return true;
      }
  return false;
}
//------------------------------------------------------------ GetDelimiterList-------------------------------------------------------
QStringList CMetaFiltre::GetDelimiterList(QStringList &opList)
{opList.clear();
 opList = QStringList::split("@",TR(DELIMITER_TYPE_LIST));
 return opList;
}

//------------------------------------------------------------ GetDelimiterList_ID-------------------------------------------------------
int CMetaFiltre::GetDelimiterList_ID(QString operateur)
{return QStringList::split("@",TR(DELIMITER_TYPE_LIST)).findIndex(operateur);
}


