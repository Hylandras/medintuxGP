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
 *   Commissariat �  l'Energie Atomique                                            *
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

// C++ Interface: C_DSVar.h


#ifndef C_DSVar_H
#define C_DSVar_H

#include <QString>
#include <QObject>

#define TR  QObject::tr
//=============================================================== C_DSVar ===========================================================
class C_DSVar
{
public:
    C_DSVar( QString& confData );
    int           initValues( QString& confData);
    static char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line, QString &err);
    int           areFieldsNamesValids() {return (m_LastError.length()==0);}
    QString       getLastError()         {return  m_LastError;}
public:
    QString  m_VERSION;
    QString  m_SCHEMA_NAME;
    //........................ m_LABO_EXAMENS_TBL_NAME table des examens.........................................
    QString  m_LABO_EXAMENS_TBL_NAME;               // nom  de la table des resultats hprim
    QString  m_LABO_EXAMENS_REF_PK;                 // reference sur le patient a qui appartient ce resultat
    QString  m_LABO_EXAMENS_PK;                     // clef primaire
    QString  m_LABO_EXAMENS_OBX_HPRIM;              // données hprim brutes
    //........................ table des patients .........................................
    QString  m_LABO_PATIENTS_TBL_NAME;         // nom de la table des patients
    QString  m_LABO_PATIENTS_PK;               // clef primaire
    QString  m_LABO_PATIENTS_SERVICE;          // service auquel appartient ce patient
    QString  m_LABO_PATIENTS_NB_OBX;           // nombre d'OBX (examens)
    QString  m_LABO_PATIENTS_NOM_EXPED;        // nom de l'expediteur
    QString  m_LABO_PATIENTS_CODE_EXPED;       // code expediteur
    QString  m_LABO_PATIENTS_NOM_DESTI;        // nom du destinataire                                                              Secabilite
    QString  m_LABO_PATIENTS_CODE_DESTI;       // code destinataire
    QString  m_LABO_PATIENTS_DATE_EMISSION;    // date emission
    QString  m_LABO_PATIENTS_HEURE_EMISSION;   // heure emission
    QString  m_LABO_PATIENTS_NOM;              // nom patient
    QString  m_LABO_PATIENTS_PRENOM;           // prenom patient
    QString  m_LABO_PATIENTS_DATENSS;          // date naissance
    QString  m_LABO_PATIENTS_SEXE;             // sexe
    QString  m_LABO_PATIENTS_DATE_EXAMEN;        // date examen
    QString  m_LABO_PATIENTS_HEURE_EXAMEN;       // heure examen
    QString  m_LABO_PATIENTS_HPRIMSEP;           // chaine des separateurs HPRIM
    QString  m_LABO_PATIENTS_DEF_EXAMEN;         // definition des examens
    QString  m_LABO_PATIENTS_FILENAME_REF;       // nom du fichier de reference

    //........................ Table des minima maximas .........................................
    QString  m_LABO_MINMAX_TBL_NAME;           // nom de la table des minimum et maximums
    QString  m_LABO_MINMAX_PK;                 // clef primaire
    QString  m_LABO_MINMAX_EXAMEN;             // examen lie a ces valeurs min max
    QString  m_LABO_MINMAX_SEXE;               // sexe lie a ce maximum minimum
    QString  m_LABO_MINMAX_MAX;                // valeur maximum
    QString  m_LABO_MINMAX_MIN;                // valeur minimum

private:
   QString      m_LastError;
};

#endif
