 /********************************* C_DBVarDrTux.h ********************************
 *  #include "C_DBVarDrTux.h"                                                     *
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

// C++ Interface: C_DBVarDrTux.h


#ifndef C_DBVarDrTux_H
#define C_DBVarDrTux_H

#include <qstring.h>
#include <qobject.h>
#include "C_DBVarDrTux.h"

#define TR  QObject::tr
//=============================================================== C_DBVarDrTux ===========================================================
class C_DBVarDrTux
{
public:
    C_DBVarDrTux(  QString& confData, QString *errMess);
    int           initValues( QString& confData, QString *errMess);
    static char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err);
    int           areFieldsNamesValids() {return m_IsDBFieldsNamesValids;}
public:
  //................. DOSS_INDEX ............................
  QString      m_DOSS_INDEX_TBL_NAME;         // non de la table DU FICHIER GENERAL D'INDEX DES DOSSIERS
  QString      m_DOSS_INDEX_GUID;             // un champ GUID unique qualifiant le numero de dossier
  QString      m_DOSS_INDEX_NOM;              // un champ NOM   qualifiant le nom du dossier
  QString      m_DOSS_INDEX_PRENOM;           // un champ PRENOM   qualifiant le prenom du dossier
  QString      m_DOSS_INDEX_PRIM_KEY;         // une clef primaire (doublon avec le GUID) mais permettant acces rapide

   //................. DOSS_IDENT ............................
   QString      m_DOSS_IDENT_TBL_NAME;      // nom de la table d' IDENTITE SIGNALETIQUE
   QString      m_DOSS_IDENT_GUID;          // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_IDENT_NSS ;          // date de naissance
   QString      m_DOSS_IDENT_SEX;           // sexe
   QString      m_DOSS_IDENT_JFNOM;         // nom de jeune fille
   QString      m_DOSS_IDENT_NB_ENF;        // nombre d'enfants
   QString      m_DOSS_IDENT_RUE;           // adresse
   QString      m_DOSS_IDENT_CODE_POST;     // Code postal
   QString      m_DOSS_IDENT_VILLE;         // Ville
   QString      m_DOSS_IDENT_CEDEX;         // cedex
   QString      m_DOSS_IDENT_PAYS;          // pays
   QString      m_DOSS_IDENT_TEL1;          // Telephone 1
   QString      m_DOSS_IDENT_TEL2;          // Telephone 2
   QString      m_DOSS_IDENT_TEL3;          // Telephone 3
   QString      m_DOSS_IDENT_TEL1_TYP;      // Telephone 1  type (parent amis etc...)
   QString      m_DOSS_IDENT_TEL2_TYP;      // Telephone 2
   QString      m_DOSS_IDENT_TEL3_TYP;      // Telephone 3
   QString      m_DOSS_IDENT_EMAIL;         // Email
   QString      m_DOSS_IDENT_NUMSS;         // Numero de secu
   QString      m_DOSS_IDENT_IS_ASS;        // le patient est il l'assure
   QString      m_DOSS_IDENT_GEME;          // rang gemellaire
   QString      m_DOSS_IDENT_NOM_ASS;       // nom de l'assure
   QString      m_DOSS_IDENT_PRENOM_ASS;    // prenom de l'assure
   QString      m_DOSS_IDENT_NSS_ASS;       // date de naissance de l'assure
   QString      m_DOSS_IDENT_PROF;          // profession
   QString      m_DOSS_IDENT_TITRE;         // Titre( Dr Mr Md Pr ...)
   QString      m_DOSS_IDENT_REF_PK;        // reference a la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX

   //................. DOSS_NOTE ............................
   QString      m_DOSS_NOTE_TBL_NAME;               // nom de la table de la note du patient
   QString      m_DOSS_NOTE_PK;                     // Clef primaire
   QString      m_DOSS_NOTE_PAT_GUID;               // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_NOTE_PAT_PK ;                // Clef primaire du dossier auquel fait reference cette note
   QString      m_DOSS_NOTE_HTML;                   // Donnees HTML de la note

   //................. DOSS_INTERVENANTS ............................
   QString      m_DOSS_INTERVENANTS_TBL_NAME;      // nom de la table des intervenants d'un patient
   QString      m_DOSS_INTERVENANTS_PK;            // Clef primaire de cet enregistrement
   QString      m_DOSS_INTERVENANTS_PAT_GUID;      // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_INTERVENANTS_PAT_PK ;       // Clef primaire du dossier auquel fait reference cet intervenant
   QString      m_DOSS_INTERVENANTS_INTERVPK;      // Clef primaire de l'intervenant auquel fait reference cet enregistrement
   QString      m_DOSS_INTERVENANTS_TYPE;          // type d'intervenant (medecin refenrent etc ...)
   //................. INTERVENANTS ............................
   QString      m_INTERVENANTS_TBL_NAME;            // nom de la table des intervenants
   QString      m_INTERVENANTS_PK;                  // primary key
   QString      m_INTERVENANTS_NOM;                 // nom de l'intervenant
   QString      m_INTERVENANTS_PRENOM;              // prenom de l'intervenant
   QString      m_INTERVENANTS_SPEC;                // specialite de l'intervenant
   QString      m_INTERVENANTS_TITRE;               // titre de l'intervenant (Dr Pr Mr Mm etc...)
   QString      m_INTERVENANTS_RUE;                 // rue de l'intervenant
   QString      m_INTERVENANTS_CODE_POST;           // Code postal de l'intervenant
   QString      m_INTERVENANTS_VILLE;               // Ville de l'intervenant (exple: Medecin Urgentiste)

 //................. CIM10_USER_FAVORIS ............................
   QString      m_CIM10_USER_FAV_TBL_NAME;          // nom de la table de l'header des FAVORIS CIM 10
   QString      m_CIM10_USER_FAV_PK;                // clef primaire de l'enregistrement
   QString      m_CIM10_USER_FAV_USER;              // login de l'utilisateur auquel appartiennent les favoris
   QString      m_CIM10_USER_FAV_DATA;              // donnee des favoris (BLOB)

 //................. DOSS_RUB_HEAD ............................
   QString      m_DOSS_RUB_HEAD_TBL_NAME;        // nom de la table de l'header des RUBRIQUES
   QString      m_DOSS_RUB_HEAD_PRIMKEY;         // clef primaire de cet enregistrement
   QString      m_DOSS_RUB_HEAD_GUID;            // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_RUB_HEAD_TYPE;            // Type de rubrique
   QString      m_DOSS_RUB_HEAD_NOM_DATE;        // Court texte descriptif de l'enregistrement.
   QString      m_DOSS_RUB_HEAD_SUBTYPE;         // sous Type de rubrique (sert notamment a orienter sur un modèle d'ipression)
   QString      m_DOSS_RUB_HEAD_DATE;            // date de l'enregistrement
   QString      m_DOSS_RUB_HEAD_USER;            // utilisateur associe a rubrique
   QString      m_DOSS_RUB_HEAD_SIGN_USER;       // utilisateur associe signataire de la rubrique
   QString      m_DOSS_RUB_HEAD_FIN;             // date de fin de l'enregistrement
   QString      m_DOSS_RUB_HEAD_DUREE_MOD;       // duree de vie du document en modification
   QString      m_DOSS_RUB_HEAD_PROP_0;          // Propriete n 0   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_1;          // Propriete n 1   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_2;          // Propriete n 2   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_3;          // Propriete n 3   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_PROP_4;          // Propriete n 4   pour le classement de l'enregistrement dans l'organiseur
   QString      m_DOSS_RUB_HEAD_REF_DOSS_PK;     // Clef primaire du dossier auquel l'enregistrement fait reference (doublon avec m_DOSS_RUB_HEAD_GUID)
   QString      m_DOSS_RUB_HEAD_REF_BLOBS_PK;    // reference au Pk du blobs

  //................. DOSS_RUB_DATA ............................
   QString      m_DOSS_RUB_DATA_TBL_NAME;        // nom de la table des donnees
   QString      m_DOSS_RUB_DATA_GUID;            // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_RUB_DATA_PRIMKEY;         // clef primaire de cet enregistrement  (le meme que le header)
   QString      m_DOSS_RUB_DATA_BLOB;            // donnees binaires

  //................. EVNT_LIST ............................
   QString      m_EVNT_LIST_TBL_NAME;          // nom de la table des droits des utilisateurs non prescripteurs
   QString      m_EVNT_LIST_PK;                // clef primaire de cet enregistrement
   QString      m_EVNT_DOSS_PK;                // clef primaire du dossier patient duquel depend ce patient
   QString      m_EVNT_LIST_PERE;              // clef primaire d'un enr pere de la table m_EVNT_LIST_TBL_NAME
   QString      m_EVNT_LIST_PKDOC;             // clef primaire d'un enr fils de la table DOSS_RUB_HEAD_TBL_NAME

  //................. USER_IDENT ............................
   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des donnees
   QString      m_USER_IDENT_NOM;                // Nom
   QString      m_USER_IDENT_PRENOM;             // Prenom
   QString      m_USER_IDENT_TITRE;              // Titre( Dr Mr Md Pr ...)
   QString      m_USER_IDENT_LOGIN;              // Identifiant d'utilisateur
   QString      m_USER_IDENT_PASSWORD;           // mot de passe
   QString      m_USER_IDENT_DROITS;             // Droits de cet utilisateur
   QString      m_USER_IDENT_SEXE;               // sexe de l'utilisateur (si il y en a un)
   QString      m_USER_IDENT_NUM_ORDRE;          // Numero d'ordre (si il y en a un)
   QString      m_USER_IDENT_NUM_RPPS;           // Numero RPPS (si il y en a un)
   QString      m_USER_IDENT_CLEF_RPPS;          // Clef RPPS (si il y en a une)
   QString      m_USER_IDENT_PRIMKEY;            // clef primaire de cet enregistrement
   QString      m_USER_IDENT_GUID;               // Identifiant unique de cet utilisateur
   QString      m_USER_IDENT_CONVENTION;         // convention exple "01 MEDECINE GENERALE"
   QString      m_USER_IDENT_CODE_POST;          // Code postal
   QString      m_USER_IDENT_VILLE;              // Ville d'exercice
   QString      m_USER_IDENT_RUE;                // Rue d'exercice
   QString      m_USER_IDENT_QUALITE;            // Type d'intervenant (exple: Medecin Urgentiste)
   QString      m_USER_IDENT_TEL_1;                               // Tel_1 de cet intervenant
   QString      m_USER_IDENT_TEL_2;                               // Tel_2 de cet intervenant
   QString      m_USER_IDENT_TEL_3;                               // Tel_3 de cet intervenant
   QString      m_USER_IDENT_EMAIL;                               // Email de cet intervenant
   QString      m_USER_IDENT_NOTE;                                // note de l'intervenant
   QString      m_USER_IDENT_CODE_REGIME;                         // code regime de l'intervenant
   QString      m_USER_IDENT_CHER;                                // Mention familliere de intervenant Cher Ami Cher confrere
   QString      m_USER_IDENT_TEL_TYPE_1;                          // Tel_1 de cet intervenant
   QString      m_USER_IDENT_TEL_TYPE_2;                          // Tel_2 de cet intervenant
   QString      m_USER_IDENT_TEL_TYPE_3;                          // Tel_3 de cet intervenant
   QString      m_USER_IDENT_TEL_ABR_1;                           // Tel_1 de cet intervenant
   QString      m_USER_IDENT_TEL_ABR_2;                           // Tel_2 de cet intervenant
   QString      m_USER_IDENT_TEL_ABR_3;                           // Tel_3 de cet intervenant

  //................. CODES POSTAUX ................................................................................
  QString      m_CODES_POSTAUX_TBL_NAME;   // nom de la table des codes postaux

  //................. USER_DATA ............................
   QString      m_USER_DATA_TBL_NAME;           // nom de la table des donnees des RUBRIQUES
   QString      m_USER_DATA_PK;                 // clef primaire de cet enregistrement
   QString      m_USER_DATA_REF_PK;             // Ref a la clef primaire de l'utilisateur auquel appartiennent les donnees
   QString      m_USER_DATA_NAMEDOC;            // nom des donnees
   QString      m_USER_DATA_TYPE;               // type de donnees
   QString      m_USER_DATA_BLOB;               // donnees proprement dites

   //................. VERROU ............................
   QString      m_VERROU_TBL_NAME;              // nom de la table des donnees des verrous
   QString      m_VERROU_PK;                    // clef primaire de cet enregistrement
   QString      m_VERROU_DOSS_PK;               // Ref a la clef primaire du dosssier verrouille
   QString      m_VERROU_DOSS_GUID;             // GUID du dossier verrouille
   QString      m_VERROU_RUB_NAME;              // Nom de la rubrique verrouillee
   QString      m_VERROU_RUB_PK;                // PK du header de rubrique verrouille
   QString      m_VERROU_USER_NAME;             // utilisateur ayant verrouille les donnees
   QString      m_VERROU_SIGN_USER_NAME;        // utilisateur signataire ayant verrouill? les donn?es
   QString      m_VERROU_DATE_TIME;             // date de verrouillage des donnees

  //................. USER_PERMS ............................
   QString      m_USER_PERMS_TBL_NAME;          // nom de la table des droits des utilisateurs non prescripteurs
   QString      m_USER_PERMS_PK;                // clef primaire de cet enregistrement
   QString      m_USER_PERMS_SIGN_GUID;         // Ref au GUID du signataire autorisant cet utilisateur
   QString      m_USER_PERMS_FRIEND_GUID;       // GUID de cet utilisateur
   QString      m_USER_PERMS_FRIEND_DROITS;     // Droits que le signataire attribue a cet utilisateur
   QString      m_USER_PERMS_FRIEND_BLOBS;      // Configuration des autorisations fines (liste des fichiers autorises avec les droits)
   QString      m_AGENDA_MASK_TBL_NAME;         // table des masques agenda
   QString      m_AGENDA_TBL_NAME;              // table de l'agenda
   QString      m_AGENDA_COLOR_PROFILS_TBL_NAME;// table des couleurs de profils agenda
   QString      m_VERSION_TBL_NAME;             // table des versions du programme
   QString      m_VERSION_NUMBER;               // numero de version de la base de donnee

private:
   int          m_IsDBFieldsNamesValids;
};

#endif
