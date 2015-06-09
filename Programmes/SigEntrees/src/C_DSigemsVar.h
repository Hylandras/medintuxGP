 /********************************* C_DSVar.h ********************************
 *  #include "C_DSVar.h"                                                     *
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
 *   Commissariat      l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F ©d ©ration, 75752 PARIS cedex 15.   *
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

// C++ Interface: C_DSVar.h


#ifndef C_DSigemsVar_H
#define C_DSigemsVar_H

#include <QString>
#include <QObject>

#define TR  QObject::tr
//=============================================================== C_DSigemsVar ===========================================================
class C_DSigemsVar
{
public:
    C_DSigemsVar( QString& confData );
    int           initValues( QString& confData);
    static char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line, QString &err);
    int           areFieldsNamesValids() {return (m_LastError.length()==0);}
    QString       getLastError()         {return  m_LastError;}
public:

    QString  m_SCHEMA_NAME;
    QString  m_VERSION;                     // --------  Numero de version de la base de donnee --------
    QString  m_SIG_ENTREES_TBL_NAME ;       // --------  nom de la table des entrees SIGEMS --------
    QString  m_SIG_ENT_PK ;                 // Cl  _ Auto increment
    QString  m_SIG_ENT_NOM  ;               // Nom patient entre
    QString  m_SIG_ENT_PRENOM  ;            // Prenom patient entre
    QString  m_SIG_ENT_SEXE  ;              // Sexe Patient entre
    QString  m_SIG_ENT_DATE_NSS ;           // Date naissance Pateint entre
    QString  m_SIG_ENT_NUM_SS ;             // Num Secu Patient entre
    QString  m_SIG_ENT_DATE_ENTREE ;        // Date d'entree du Patient
    QString  m_SIG_ENT_NOM_FIC_HPRIM ;      // Nom fichier HPRIM des entrants SIGEMS
    QString  m_SIG_ENT_GUID_PATIENT ;       // Guid du Patient Medintux s'il existe
    QString  m_SIG_ENT_QUALITE;             // Qualite bnef
    QString  m_SIG_ENT_RANG_GEME;           // rang gemelaire
    QString  m_SIG_ENT_BLOB_HPR ;           // Blob contenant le fichier HPRIM recu
    QString  m_SIG_ENT_DATE_PEC;            // Date de prise en charge dans medintux
    QString  m_SIG_ENT_NUM_PAT_SIGEMS;      // Numero de patient sigems qui ira dans GUID

    QString  m_DOSS_RUB_HEAD_TBL_NAME;      // Table des rubriquesHead
    QString  m_DOSS_RUB_HEAD_GUID;          // Guid patient
    QString  m_DOSS_RUB_HEAD_DATE;          // date du doc


    QString  m_DOSS_INDEX_TBL_NAME ;         // --------  nom de la table de l'index des  patients  --------
    QString  m_DOSS_INDEX_GUID;              // GUID identificateur unique du N de Dossier du patient
    QString  m_DOSS_INDEX_NOM;               // Nom habituel et usuel du patient
    QString  m_DOSS_INDEX_PRENOM;            // Prenom habituel et usuel du patient


private:
   QString   m_LastError;
};

#endif
