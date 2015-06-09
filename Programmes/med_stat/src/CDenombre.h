 /********************************* CDenombre.h ************************************
 \file CDenombre.h
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


#ifndef MED_STAT_CDenombre
#define MED_STAT_CDenombre

#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qptrlist.h>
#include <qmap.h>
#include <qsqlquery.h>

#include "CMetaField.h"

#define TR  QObject::tr

#define RUBRIQUE_EXTRACT       "RUBRIQUE_EXTRACT"
#define ORDONNANCE_VIDAL       "ORDONNANCE_VIDAL"
#define TERRAIN_GET_ATCD_CIM10 "TERRAIN_GET_ATCD_CIM10"
#define TERRAIN_GET_ATCD       "TERRAIN_GET_ATCD"
#define TERRAIN_GET_TTT_FOND   "TERRAIN_GET_TTT_FOND"
#define TERRAIN_GET_VARIABLE   "TERRAIN_GET_VARIABLE"

#define  TEST_POSSIBLES "=,>,<,<=,>=,contient,ne contient pas,!=,commence par,ne commence pas par,finit par,ne finit pas par,expression régulière"



//===================================================== CDenombreValidTest =====================================================
class CDenombreValidTest
{
    //................... constructeur destructeur ..................................
public:
    CDenombreValidTest(const char* test,
                       const char* refValue,
                       const char* regExp,
                       const char* logicOp
                      )
              {m_Test           = test;  // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_RefValue       = refValue;
               m_RegExp         = regExp;
               m_LogicOp        = logicOp;
               m_IsValid        = isClassValid();
              }
      CDenombreValidTest(const QString &xml_def
              )
              {m_Test           = "";  // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_RefValue       = "";
               m_LogicOp        = "";
               m_RegExp         = "";
               UnSerialize( xml_def );
               m_IsValid        = isClassValid();
              }
    ~CDenombreValidTest()
              {
              }
    int                doTest(const QString &valueToTest);
    QString            LastError(){return m_LastError;};
    int                isValid()  {return m_IsValid;};
    QString            Serialize(const QString &ofset="");
    int                UnSerialize(const QString &data, int pos = 0 );
    static int         isTestValid(const QString &test);
    static QStringList GetPossiblesTestList();
    static QString     GetPossiblesTestString();
    static QString     StrStringToXML( const QString &txt_in);
    static QString     XMLToStrString( QString &txt);

public:
    QString       m_Test;
    QString       m_RefValue;
    QString       m_LogicOp;
    QString       m_RegExp;

private:
    int      GetXMLValue(const QString &dataToExamine, const QString &xmlToken, QString &valueToGet, int pos=0);
    int      isClassValid();
    //................... data ......................................................
    QString       m_LastError;
    int           m_IsValid;
};
typedef QPtrList<CDenombreValidTest> PRT_LIST_CDenombreValidTest;

//===================================================== CDenombre =====================================================
class CDenombre
{
    //................... constructeur destructeur ..................................
public:
     CDenombre ( const char* class_name,
                 const char* field_work
              );
     CDenombre ( const QString &xml_def );
    ~CDenombre();
    void               Reinit(){m_mapResultCount.clear();};
    int                AddResult(const QString & result);
    QString            GetValue(const QString & input_text, int &curs_pos);
    QString            LastError(){return m_LastError;};
    int                isValid(){return m_IsValid;};
    int                getNbTest(){return m_NbTest;};
    QString            Serialize(const QString &ofset="");
    int                UnSerialize(const QString &data, int pos = 0 );
    static QString     StrStringToXML( const QString &txt_in);
    static QString     XMLToStrString( QString &txt);
    QString            GetName(){return m_Name;};
    QString            GetFieldName(){return m_FieldWork;};
    QString            SetName(const QString &new_name){QString old = m_Name; m_Name = new_name; return old;};
    QString            SetFieldName(const QString &new_name){QString old = m_FieldWork; m_FieldWork = new_name; return old;};
    void               DoResult(const QMap <QString,QString> &map_fieldName_value);
    QString            OutResult();

    QMap<QString, int >           m_mapResultCount;
    PRT_LIST_CDenombreValidTest   m_ValidTestList;

    CMetaField*                   m_pCMetaField;
    int                           m_Field_ID;
    QString                       m_Name;
    QString                       m_FieldWork;

private:
    int     GetXMLValue(const QString &dataToExamine, const QString &xmlToken, QString &valueToGet, int pos=0);
    //................... data ......................................................
    int                           m_NbTest;
    QString                       m_LastError;
    int                           m_IsValid;

};
typedef QPtrList<CDenombre> PRT_LIST_CDenombre;
#endif

