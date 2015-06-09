 /********************************* CMetaField.h ************************************
 \file CMetaField.h
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


#ifndef MED_STAT_CMetaField
#define MED_STAT_CMetaField

#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#define TR  QObject::tr

#include "../../MedinTuxTools/CMedicaBase.h"

#define RUBRIQUE_EXTRACT       "RUBRIQUE_EXTRACT"
#define ORDONNANCE_VIDAL       "ORDONNANCE_VIDAL"
#define TERRAIN_GET_ATCD_CIM10 "TERRAIN_GET_ATCD_CIM10"
#define TERRAIN_GET_ATCD       "TERRAIN_GET_ATCD"
#define TERRAIN_GET_TTT_FOND   "TERRAIN_GET_TTT_FOND"
#define TERRAIN_GET_VARIABLE   "TERRAIN_GET_VARIABLE"

// #define TYP_TERRAIN               20060000
// #define TYP_ORDO_CALC_CURENT      20099999  // pour que date ordo soit la date courante lors appel DrTux::AddNewRecordToRubrique()



// #define TYP_OBSERVATION    20030000   // +0 à +999 recouvre les observations textuelles
// #define TYP_PRESCRIPTION   20020000   // +0 à +999 recouvre toutes les prescriptions (certif + ordo)

#define OPERATEUR_LIST "Vide@>@<@<=@>=@est égal@contient@commence par@finit par@est différent de@ne contient pas"

//...................... nouveaux sous types ...........................................................

// #define TYP_ORDO_CALC      20020100
// #define TYP_ORDONNANCE     20020200
// #define TYP_CERTIFICAT     20020300
// #define TYP_COURRIER       20020500
// #define TYP_URG_OBSERV     20040000
#define  TYPES_POSSIBLES      "Observation,Prescription Vidal,Prescription,Certificat,Courrier,Terrain,Age Patient"
#define  TYPES_RUB_POSSIBLES  "Observation,Prescription Vidal,Prescription,Certificat,Courrier,Terrain"
//===================================================== CMetaValue =====================================================
class CMetaValue
{public:
    CMetaValue( const char* field_name,
                const char* field_value,
                int         field_record_id
              )
   { m_field_name      = field_name;
     m_field_value     = field_value;
     m_field_record_id = field_record_id;
   }
   QString field_name()       {return m_field_name;}
   QString field_value()      {return m_field_value;}
   int     field_record_id()  {return m_field_record_id;}
private:
   QString m_field_name;
   QString m_field_value;
   int     m_field_record_id;
};
//===================================================== CMetaField =====================================================
class CMetaField
{
    //................... constructeur destructeur ..................................
public:
    CMetaField(const char* field_name,
               const char* field_Rubrique,
               const char* field_Expression,
               CMedicaBase *pCMedicaBase
              )
              {m_pCMedicaBase   = pCMedicaBase;
               m_Name           = field_name;  // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_Rubrique       = field_Rubrique;
               m_Rubrique       = m_Rubrique.stripWhiteSpace();
               m_Name           = m_Name.stripWhiteSpace();
               m_Rubrique       = RubNameToRubType(m_Rubrique);       // convertir un eventuel nommage en clair en type exple "Observation" --> "20030000"
               m_Expression     = field_Expression;
               m_Fonction       = "";
               m_ArgList.clear();
               m_IsValid = decomposeToken(m_Expression, m_Fonction, m_ArgList);
               m_CPosologieOrdoList.clear();
               m_ExtractedValues.setAutoDelete(true);
               m_PositiveList.setAutoDelete(true);
              }
    ~CMetaField()
              {
              }
    QString   GetValue(const QString & input_text, int &curs_pos, int record_id);
    QString   LastError(){return m_LastError;};
    // QString   blob_data(){return m_blob_data;}
    int       isValid(){return m_IsValid;};
    static int          GetListOperateur_ID(QString operateur);
    static QStringList  GetListOperateur(QStringList &opList);
    static int          decomposeToken(const QString &expression, QString &function, QStringList &argList);
    static QStringList  GetPossiblesRubTypesList(int strictSqlfield =0);
    static QStringList  GetSyntaxListFromRubriqueName(const QString &name, QStringList &list);
    static QStringList  GetAllPossiblesFunctionSyntax();
    static QStringList  GetAllPossiblesTokenSyntax();
    static QString      TokenSyntaxToFunctionSyntax(const QString &tokenToFind);
    static int          GetSyntaxID_FromSyntaxString(const QString &name);
    static QString      RubNameToRubType(const QString &rubName);
    bool                isRecordInPositiveList(int record_id);
    QList <CMetaValue>    m_ExtractedValues;
    QList <CMetaValue>    m_PositiveList;

    QString        m_Rubrique;
    QString        m_Name;
    QString        m_LastError;
    CMedicaBase   *m_pCMedicaBase;                // moteur de la base de donnée medicaments
    QPL_CPosologie m_CPosologieOrdoList;
    
private:
    //................... data ......................................................
    QString       m_Expression;
    QString       m_Fonction;
    QStringList   m_ArgList;
    int           m_IsValid;
    // QString       m_blob_data;
};
typedef QPtrList<CMetaField> PRT_LIST_CMetaField;
#endif

