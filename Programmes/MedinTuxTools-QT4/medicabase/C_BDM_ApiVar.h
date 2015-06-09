/********************************* C_BDM_ApiVar.h ********************************
*  #include "C_BDM_ApiVar.h"                                                     *
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
*   Commissariat a  l'Energie Atomique                                           *
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
#ifndef C_BDM_APIVAR_H
#define C_BDM_APIVAR_H

#include <QString>
#include <QObject>

#define TR  QObject::tr

//=============================================================== C_BDM_ApiVar ===========================================================
class C_BDM_ApiVar
{
public:
    C_BDM_ApiVar(const QString& confData="");

       void          initDefaultValues();
       int           initValues( const QString& confData);
       static char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line, QString &err);
       int           areFieldsNamesValids() {return (m_LastError.length()==0);}
       QString       getLastError()         {return  m_LastError;}
   public:
       //........................ m_BDM_VERSION_TBL_NAME  Table d numero de version .........................................
       QString  m_BDM_VERSION_TBL_NAME;           // nom de la table des numeros de version
       QString  m_BDM_VERSION_PK ;                // clef primaire
       QString  m_BDM_VERSION_OWNER;              // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
       QString  m_BDM_VERSION_LANG;               // langue de la donnee (en fr etc...)
       QString  m_BDM_VERSION_NUMBER;             // numero de version

       //........................ m_BDM_DRUGLIST_TBL_NAME  Table de la liste des drogues .........................................
       QString  m_BDM_DRUGLIST_TBL_NAME;      // nom de la table de la liste des medicaments
       QString  m_BDM_DRUGLIST_PK;            // clef primaire
       QString  m_BDM_DRUGLIST_OWNER;         // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
       QString  m_BDM_DRUGLIST_LANG;          // langue de la donnee (en fr etc...)
       QString  m_BDM_DRUGLIST_ID;            // identificateur unique du medicament
       QString  m_BDM_DRUGLIST_TYPE_ID;       // type d'identificateur (CIP CIP7 CIS)
       QString  m_BDM_DRUGLIST_ATC;           // Code ATC
       QString  m_BDM_DRUGLIST_LIBELLE;       // libelle de la classe ATC
       QString  m_BDM_DRUGLIST_DCI_1;         // premiere  substance DCI (libelle ATC)
       QString  m_BDM_DRUGLIST_DCI_2;         // deuxieme  substance DCI (si produit compose)
       QString  m_BDM_DRUGLIST_DCI_3;         // troisieme substance DCI (si produit compose)
       QString  m_BDM_DRUGLIST_UCD;           // code UCD
       QString  m_BDM_DRUGLIST_UCD_PRICE;     // Prix UCD
       QString  m_BDM_DRUGLIST_GENERIQUE;     // genericite
       QString  m_BDM_DRUGLIST_STATUT;        // statut (ville hopital)
       QString  m_BDM_DRUGLIST_SPEC_FK;       // pointeur sur les specifications produits

       //........................ m_BDM_USER_MONOGRAPHIE_TBL_NAME  table des monographies utilisateur .........................................
       QString  m_BDM_USER_MONOGRAPHIE_TBL_NAME;      // nom de la table des monographies utilisateur
       QString  m_BDM_USER_MONOGRAPHIE_PK;            // clef primaire
       QString  m_BDM_USER_MONOGRAPHIE_OWNER;         // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
       QString  m_BDM_USER_MONOGRAPHIE_LANG;          // langue de la donnee (en fr etc...)
       QString  m_BDM_USER_MONOGRAPHIE_ID;            // identificateur unique du medicament
       QString  m_BDM_USER_MONOGRAPHIE_TYPE_ID;       // type d'identificateur (CIP CIP7 CIS)
       QString  m_BDM_USER_MONOGRAPHIE_LIBELLE;       // titre de la monographie
       QString  m_BDM_USER_MONOGRAPHIE_AUTEUR;        // auteur de la monographie
       QString  m_BDM_USER_MONOGRAPHIE_DATE;          // date de la monographie
       QString  m_BDM_USER_MONOGRAPHIE_HTML_BLOB;     // donnees de la monographie

       //........................ m_BDM_POSO_HIT_TBL_NAME  Table de la liste des drogues .........................................
       QString  m_BDM_POSO_HIT_TBL_NAME;        // nom de la table de la table des favoris lies a un medicament
       QString  m_BDM_POSO_HIT_PK;              // clef primaire
       QString  m_BDM_POSO_HIT_OWNER;           // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
       QString  m_BDM_POSO_HIT_LANG;            // langue de la donnee (en fr etc...)
       QString  m_BDM_POSO_HIT_SCORE;           // niveau du hit
       QString  m_BDM_POSO_HIT_GRAMMAR;         // posologie liee a ce favoris
       QString  m_BDM_POSO_HIT_ORDER;           // niveau du hit (nombre d'utilisations sera utilise pour son classement)
       QString  m_BDM_POSO_HIT_CODE_INDIC;      // codage de l'indication liee a ce hit
       QString  m_BDM_POSO_HIT_CODE_INDIC_TYPE; // type de codage CIM10 etc ....
       QString  m_BDM_POSO_HIT_LIBELLE_INDIC;   // libelle de l'indication principale liee a ce favori
       QString  m_BDM_POSO_HIT_DRUG_ID;         // identificateur unique du medicament relie à ce hit
       QString  m_BDM_POSO_HIT_DRUG_TYPE_ID;    // type d'identificateur de l'id du medicament (CIP CIP7 CIS)


   private:
      QString      m_LastError;
   };

#endif // C_BDM_APIVAR_H
