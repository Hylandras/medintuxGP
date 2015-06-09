 /********************************* CDenombre.cpp **********************************
 \file CDenombre.cpp
 \brief (put some short descripion here).

     #include "CDenombre.h"
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


#include "CDenombre.h"
#include "../../MedinTuxTools/CGestIni.h"
//        <rect>
//            <x>0</x>
//            <y>0</y>
//            <width>443</width>
//            <height>242</height>
//        </rect>
//===================================================== CDenombre ==============================================================
   CDenombre::CDenombre( const char* class_name,
                         const char* field_work
                       )
              {m_Name           = class_name;  // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_FieldWork      = field_work;
               m_ValidTestList.clear();
               m_ValidTestList.setAutoDelete ( TRUE );
               m_IsValid        = 1;
               m_NbTest         = 0;
               m_pCMetaField    = 0;        // pointe sur le metachamp auquel est relié la classe de comptage
               m_Field_ID       = -1;
              }
   CDenombre::CDenombre ( const QString &xml_def )
              {UnSerialize(xml_def);
              }
   CDenombre::~CDenombre()
              {
              }

//----------------------------------------------- DoResult ----------------------------------------------------------
void CDenombre::DoResult(const QMap <QString,QString> &map_fieldName_value)
{QString res;
 if (m_FieldWork[0]=='*')    // le champ associe est un meta champ
    {if (m_pCMetaField) // m_ExtractedValues
        { /*
         int   nb      = m_pCMetaField->m_ExtractedValues.count();
         for (int i=0; i<nb; ++i)
             { res = m_pCMetaField->m_ExtractedValues.at(i)->field_value();
               res = res.remove("\t");
               res = res.remove("\r");
               res = res.remove("\n");
               AddResult(res.stripWhiteSpace());
             }
         */ 

         int   nb      = m_pCMetaField->m_PositiveList.count();
         for (int i=0; i<nb; ++i)
             { res = m_pCMetaField->m_PositiveList.at(i)->field_value();
               res = res.remove("\t");
               res = res.remove("\r");
               res = res.remove("\n");
               AddResult(res.stripWhiteSpace());
             }

        }
    }
 else                        // le champ associe est un champ de la base
    {QString field_name = m_FieldWork.mid(2);  // le debut du nom du champ associe commence par '° '(si meta champ ) ou '* ' (si champ de base de donnees)
     res = map_fieldName_value[field_name];
     AddResult(res.stripWhiteSpace());
    }
}

//------------------------------------------------ AddResult --------------------------------------------------------
int CDenombre::AddResult(const QString & result)
{int nb = 1;
 QMap<QString, int >::Iterator it  = m_mapResultCount.find(result);
 if (it != m_mapResultCount.end())
    {nb  = it.data() + 1;
     m_mapResultCount[result] = nb;
    }
 else
    {m_mapResultCount.insert ( result, nb );
    }
 return nb;
}

//------------------------------------------------ OutResult --------------------------------------------------------
QString CDenombre::OutResult()
{int nb_total = 0;
 QString res = TR("\n ==>  Résultats de : ") + m_Name + TR(" nombre total : ~#=.=#~");
 QMap<QString, int >::Iterator it;
 for ( it = m_mapResultCount.begin(); it != m_mapResultCount.end(); ++it )
     {QString libelle = it.key().stripWhiteSpace();
      if (libelle.length()==0) libelle = TR("Non renseigné");
      nb_total += it.data();
      res  += TR("\n\t\t") + QString::number(it.data()) + TR("\t") + libelle  ;
     }
 res = res.replace("~#=.=#~", QString::number( nb_total));
 return res;
}


//------------------------------------------------ UnSerialize --------------------------------------------------------
int CDenombre::UnSerialize(const QString &data, int pos /* = 0 */)
{m_IsValid        = 0;
 QString dataToExamine;
 int   i = 0;
 if ( (GetXMLValue(data, "<CDenombre_Class>", dataToExamine)==-1))                                             return -1;
 if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_Name>",          m_Name, pos))==-1)                                // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_Name>",      m_Name))==-1)                           return -1;     // si echec tenter du début
    }
 XMLToStrString(m_Name);
 if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_FieldWork>",      m_FieldWork, pos))==-1)                          // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_FieldWork>",  m_FieldWork))==-1)                     return -1;     // si echec tenter du début
    }
 XMLToStrString(m_FieldWork);

 //................................... la liste des tests ............................................................
 //                                    n'est pas obligatoire si il n'y a pas
 //                                    de tests mais seulement un comptage simple
 m_NbTest = 0;
 m_ValidTestList.clear();
 QString st_nb_test;
 if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_ValidTestList_count>",       st_nb_test, pos))==-1)                 // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_ValidTestList_count>",   st_nb_test))==-1)           return -1;     // si echec tenter du début
    }
 int nb = st_nb_test.toInt();
 QString xmlData;
 for ( i=0; i<nb; ++i)
     {if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_m_ValidTestList_data>",       xmlData, pos))==-1)    return -1;
      else
        {CDenombreValidTest *pCDenombreValidTest = new CDenombreValidTest(xmlData);
         if (pCDenombreValidTest && pCDenombreValidTest->isValid())  m_ValidTestList.append(pCDenombreValidTest);
        }
     }
 m_NbTest = m_ValidTestList.count();
 m_IsValid        = 1;
 return pos;
}

//------------------------------------------------ Serialize --------------------------------------------------------
QString CDenombre::Serialize(const QString &in_ofset)
{   QString ofset = in_ofset;
            ofset = ofset.prepend("\n");
    QString result  = ofset + "<CDenombre_Class>";
            result += ofset + "   <CDenombre_m_Name>"      + StrStringToXML(m_Name      ) + "</CDenombre_m_Name>";
            result += ofset + "   <CDenombre_m_FieldWork>" + StrStringToXML(m_FieldWork ) + "</CDenombre_m_FieldWork>";
            result += ofset + "   <CDenombre_m_ValidTestList>";
            result += ofset + "      <CDenombre_m_ValidTestList_count>"+ QString::number(m_ValidTestList.count()) + "</CDenombre_m_ValidTestList_count>";
    CDenombreValidTest *pCDenombreValidTest;
    for ( pCDenombreValidTest = m_ValidTestList.first(); pCDenombreValidTest; pCDenombreValidTest = m_ValidTestList.next() )
        {result    += ofset + "      <CDenombre_m_ValidTestList_data>";
         result    += ofset +            pCDenombreValidTest->Serialize(in_ofset+"          ");
         result    += ofset + "      </CDenombre_m_ValidTestList_data>";}
         result    += ofset + "   </CDenombre_m_ValidTestList>";
         result    += ofset + "</CDenombre_Class>";
    return result;
}

//------------------------------------------------ GetXMLValue --------------------------------------------------------
// ACTION extrait la valeur contenue entre deux balises XML
// ENTREE : const QString &dataToExamine : texte a analyser
//          const QString &xmlToken : token de la balise XML doit être de  la forme : "<xxxxxxxxxx>"
//          QString &valueToGet     : QString dans laquelle déposer le résultat
//          int pos /* = -1 */   : position à laquelle rechercher la recherche si -1 (valeur par défaut) la recherche se fait à partir du début
//                                 si l'on est certain que les tokens sont ordonnés alors l'on peut se servr du retour de la fonction
//                                 pour positionner pos de façon à accélérer le traitement.
// RETOUR : int position située aprés le token fermant, permet d'avancer sur le token suivant.

int  CDenombre::GetXMLValue(const QString &dataToExamine, const QString &xmlToken_in, QString &valueToGet, int pos /* = 0 */)
{valueToGet       = "";
 QString xmlToken = xmlToken_in;
 int deb = dataToExamine.find(xmlToken, pos);
 if (deb == -1) {m_LastError = TR("ERREUR CDenombreValidTest::GetXMLValue() Mention de début manquante : \"" + xmlToken + "\""); return -1;}
 deb += xmlToken.length();
 int end = dataToExamine.find(xmlToken.insert(1,"/"), deb);
 if (end == -1) {m_LastError = TR("ERREUR CDenombreValidTest::GetXMLValue() Mention de  fin  manquante : \"" + xmlToken +"\"");  return -1;}

 valueToGet = dataToExamine.mid(deb , end - deb );
 return end + xmlToken.length();
}
//---------------------------------- StrStringToXML ---------------------------------------------------------------------------
QString CDenombre::StrStringToXML( const QString &txt_in)
{QString txt = txt_in;
 txt.replace(">", "&gt;");
 txt.replace("<", "&lt;");
 return txt;
}
//---------------------------------- XMLToStrString ---------------------------------------------------------------------------
QString CDenombre::XMLToStrString( QString &txt)
{txt.replace("&gt;",">");
 txt.replace("&lt;","<");
 return txt;
}



//===================================================== CDenombreValidTest =====================================================

//------------------------------------------------ UnSerialize --------------------------------------------------------
// NOTE au départ on suppose que l'ordre des variables à rechercher est : m_Test, m_RefValue, m_LogicOp comme devrait le faire
//      la fonction de sérialisation
// ACTION initialise la classe à partir des données XML fournies en entree.
// ENTREE : const QString &data : données XML
//          int pos /* = -1 */   : position à laquelle rechercher la recherche si -1 (valeur par défaut) la recherche se fait à partir du début
//                                 si l'on est certain que les tokens sont ordonnés alors l'on peut se servr du retour de la fonction
//                                 pour positionner pos de façon à accélérer le traitement.
// RETOUR : int position située aprés le token fermant, permet d'avancer sur le token suivant.

int CDenombreValidTest::UnSerialize(const QString &data, int pos /* = 0 */)
{QString dataToExamine;
 if ( (GetXMLValue(data, "<CDenombreValidTest_Class>", dataToExamine))==-1)                            return -1;
 if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_Test>",          m_Test, pos))==-1)                    // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_Test>",      m_Test))==-1)          return -1;     // si echec tenter du début
    }
 XMLToStrString(m_Test);

 if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_RefValue>",      m_RefValue, pos))==-1)                // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_RefValue>",  m_RefValue))==-1)      return -1;     // si echec tenter du début
    }
 XMLToStrString(m_RefValue);

 if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_LogicOp>",       m_LogicOp, pos))==-1)                 // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_LogicOp>",   m_LogicOp))==-1)       return -1;     // si echec tenter du début
    }
 XMLToStrString(m_LogicOp);

 if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_RegExp>",        m_RegExp, pos))==-1)                  // tenter avec curseur
    {if ( (pos = GetXMLValue(dataToExamine, "<CDenombreValidTest_m_RegExp>",    m_RegExp))==-1)        return -1;     // si echec tenter du début
    }
 XMLToStrString(m_RegExp);

 return pos;
}

//------------------------------------------------ Serialize --------------------------------------------------------
QString CDenombreValidTest::Serialize(const QString &in_ofset)
{   QString ofset = in_ofset;
            ofset = ofset.prepend("\n");
    QString result  = ofset + "<CDenombreValidTest_Class>";
            result += ofset + "   <CDenombreValidTest_m_Test>"      + StrStringToXML(m_Test)      + "</CDenombreValidTest_m_Test>";
            result += ofset + "   <CDenombreValidTest_m_RefValue>"  + StrStringToXML(m_RefValue)  + "</CDenombreValidTest_m_RefValue>";
            result += ofset + "   <CDenombreValidTest_m_LogicOp>"   + StrStringToXML(m_LogicOp)   + "</CDenombreValidTest_m_LogicOp>";
            result += ofset + "   <CDenombreValidTest_m_RegExp>"    + StrStringToXML(m_RegExp)    + "</CDenombreValidTest_m_RegExp>";
            result += ofset + "</CDenombreValidTest_Class>";
    return result;
}

//------------------------------------------------ doTest --------------------------------------------------------
int CDenombreValidTest::doTest(const QString &valueToTest)
{return 1;
}

//------------------------------------------------ isClassValid --------------------------------------------------------
int CDenombreValidTest::isClassValid()
{if (GetPossiblesTestList().findIndex ( m_Test)==-1) {m_LastError = TR("ERREUR CDenombreValidTest::isValid() test manquant ou non valide");                 return 0;}          // il faut que le test soit présent
 if (m_RefValue.length() == 0)                       {m_LastError = TR("ERREUR CDenombreValidTest::isValid() valeur de référence manquante");               return 0;}          //
 return 1;
}

//------------------------------------------------ isTestValid --------------------------------------------------------
int CDenombreValidTest::isTestValid(const QString &test)
{if (GetPossiblesTestList().findIndex ( test)==-1) return 0;
 return 1;
}

//------------------------------------------------ GetListPossiblesTest --------------------------------------------------------
QStringList CDenombreValidTest::GetPossiblesTestList()
{return QStringList::split(",",GetPossiblesTestString());
}

//------------------------------------------------ GetPossiblesTestString --------------------------------------------------------
QString CDenombreValidTest::GetPossiblesTestString()
{return TR (TEST_POSSIBLES);
}

//------------------------------------------------ GetXMLValue --------------------------------------------------------
// ACTION extrait la valeur contenue entre deux balises XML
// ENTREE : const QString &dataToExamine : texte a analyser
//          const QString &xmlToken : token de la balise XML doit être de  la forme : "<xxxxxxxxxx>"
//          QString &valueToGet     : QString dans laquelle déposer le résultat
//          int pos /* = -1 */   : position à laquelle rechercher la recherche si -1 (valeur par défaut) la recherche se fait à partir du début
//                                 si l'on est certain que les tokens sont ordonnés alors l'on peut se servr du retour de la fonction
//                                 pour positionner pos de façon à accélérer le traitement.
// RETOUR : int position située aprés le token fermant, permet d'avancer sur le token suivant.

int  CDenombreValidTest::GetXMLValue(const QString &dataToExamine, const QString &xmlToken_in, QString &valueToGet, int pos /* = 0 */)
{valueToGet       = "";
 QString xmlToken = xmlToken_in;
 int deb = dataToExamine.find(xmlToken, pos);
 if (deb == -1) {m_LastError = TR("ERREUR CDenombreValidTest::GetXMLValue() Mention de début manquante : \"" + xmlToken + "\""); return -1;}
 deb += xmlToken.length();
 int end = dataToExamine.find(xmlToken.insert(1,"/"), deb);
 if (end == -1) {m_LastError = TR("ERREUR CDenombreValidTest::GetXMLValue() Mention de  fin  manquante : \"" + xmlToken +"\"");  return -1;}

 valueToGet = dataToExamine.mid(deb , end - deb );
 return end + xmlToken.length();
}

//---------------------------------- StrStringToXML ---------------------------------------------------------------------------
QString CDenombreValidTest::StrStringToXML( const QString &txt_in)
{QString txt = txt_in;
 txt.replace(">", "&gt;");
 txt.replace("<", "&lt;");
 return txt;
}
//---------------------------------- XMLToStrString ---------------------------------------------------------------------------
QString CDenombreValidTest::XMLToStrString( QString &txt)
{txt.replace("&gt;",">");
 txt.replace("&lt;","<");
 return txt;
}