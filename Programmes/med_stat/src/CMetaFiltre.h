 /********************************* CMetaFiltre.h ************************************
 \file CMetaFiltre.h
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


#ifndef MED_STAT_CMetaFiltre
#define MED_STAT_CMetaFiltre

#include <qlistview.h>
#include <qdatetime.h>
#include <qobject.h>
#include <qvaluestack.h>

#include <stdlib.h>

#include "CMetaField.h"

#define TR  QObject::tr
#define CONV_TYPE_LIST       "Texte (sans modification)@TEXTE (mis en Majuscules)@texte (mis en Minuscules)@Numerique entier@flotant@Date (YYYY-MM-DDTHH:MM:SS)@Heure"
#define LOGIC_OP_LIST        "  Vide @  et               @  ou               @  sans             "
#define DELIMITER_TYPE_LIST  "  Vide @  ) @  ( @  ) et (@  ) ou ("
//===================================================== CMetaFiltreLine =====================================================
class CMetaFiltreLine
{
    //................... constructeur destructeur ..................................
public:
      CMetaFiltreLine(                 PRT_LIST_CMetaField* pCMetaFieldList,
                                 const QString &metaFieldName,
                                 const QString &operateurDeTest,
                                 const QString &refValue,
                                 const QString &logicOp,
                                 const QString &typeToConvert);
     ~CMetaFiltreLine()
              {
              };
   void           SetRefValueToCorrectConversion(const QString &refValue, const QString &typeToConvert);
   bool           DoTest();
   bool           DoTest(CMetaField *pCMetaField, int op_code, int conv_id);
   static         int  GetListConversion_ID(QString operateur);
   static QStringList  GetListConversion(QStringList &opList);
   static         int  GetListLogicOper_ID(QString operateur);
   static QStringList  GetListLogicOper(QStringList &opList);
   static QString      ExtractNumber(const QString &text);
   static QString      ExtraitChiffre(const QString &str_in, int &pos);
   static int          StringDateNaturalToQDateTime(const QString &str_in, QDateTime & qdt_out);
   static long         HeureMinuteSeconde_To_Val(const char* heure_in);
public:

   QString       m_Name;                      // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
   int           m_OpCode;                    // code operateur du test
   int           m_logicOpID;                 // code operateur logique avec le champ suivant
   CMetaField *  m_pCMetaField;               // pointeur sur le meta-champ asocié
   //........................ types des valeur de réference ...............................
   //                         on fait la conversion à la création
   //                         de façon à ne plus la refaire ensuite
   QDateTime   m_RefQDateTimeValue;         // valeur de référence avec laquelle tester
   QString     m_RefQStringValue;           // valeur de référence avec laquelle tester
   float       m_RefFloatValue;             // valeur de référence avec laquelle tester
   long        m_RefLongValue;              // valeur de référence avec laquelle tester

   int         m_ConvID;                    // indice de la conversion à utiliser (para rapport à la table CONV_TYPE_LIST)

   bool        m_TestResult;                // resultat du test
   bool        m_IsValid;                   // TRUE si initialisation correcte


};


typedef QPtrList<CMetaFiltreLine> QPTRLIST_CMETAFILTRELINE;



//===================================================== CMetaFiltre =====================================================
class CMetaFiltre
{
    //................... constructeur destructeur ..................................
public:
      CMetaFiltre(  PRT_LIST_CMetaField* pCMetaFieldList,  QListView * pQListViewFiltreDef);
     ~CMetaFiltre()
              {
              };


public:
    static QStringList GetDelimiterList(QStringList &opList);
    static         int GetDelimiterList_ID(QString operateur);
    bool           DoTest();
    bool           recordTestState(int record_id);
private:
   PRT_LIST_CMetaField          *m_pCMetaFieldList;
   QListView                    *m_pQListViewFiltreDef;
   QPTRLIST_CMETAFILTRELINE      m_CMetaFiltreLineList;
};


#endif

