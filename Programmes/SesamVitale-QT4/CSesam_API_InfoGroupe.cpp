
 /********************************* CSesam_API_InfoGroupe.cpp *********************
 \file CSesam_API_InfoGroupe.cpp
 \brief (put some short descripion here).

 ********************************** CSesam_API_InfoGroupe.cpp *********************
 *  #include "CSesam_API_InfoGroupe.h"                                            *
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

#include "CSesam_API_InfoGroupe.h"

//------------------------------------------ CSesam_API_InfoGroupe ---------------------------------------------------------
CSesam_API_InfoGroupe::CSesam_API_InfoGroupe()
{
}

//------------------------------------------ NumGroupeToString ---------------------------------------------------------
// zip:/home/ro/Documents/DataMedicalDesign/sesam-vitale/doc_FSV_1.40.3.2.zip/ssv-mp-003api_ssv_4.03doc_prog-fra-mai_05.pdf
QString        CSesam_API_InfoGroupe::NumGroupeToString(int groupe, int champ)
{
 switch (groupe)
{ //.................... Lire Carte Pratricien ........................................................
  case 1:    {switch (champ)
                 {case -1 : return TR("Titulaire carte PS");
                  case  1 : return TR("Type de carte PS");
                  case  2 : return TR("Type d'identification nationale");
                  case  3 : return TR("N° d'identification nationale");
                  case  4 : return TR("Clé du n° d'identification nationale");
                  case  5 : return TR("Code civilité");
                  case  6 : return TR("Nom du PS");
                  case  7 : return TR("Prénom du PS");
                  default : return messageChampInconnu( groupe, champ);
                 }
             }
  case 2:    {switch (champ)
                 {case -1 :  return TR("Situation du PS");
                  case  1 :  return TR("N° logique de la situation de facturation du PS");
                  case  2 :  return TR("Mode d'exercice (Libéral, salarié, remplacement, etc..)");
                  case  3 :  return TR("Statut d'exercice");
                  case  4 :  return TR("secteur d'activité");
                  case  5 :  return TR("Type d'identification structure");
                  case  6 :  return TR("N° d'identification structure");
                  case  7 :  return TR("Clé du N° d'identification structure");
                  case  8 :  return TR("Raison sociale structure");
                  case  9 :  return TR("N° d'identification de facturation du PS");
                  case  10 : return TR("Clé du N° d'identification de facturation du PS");
                  case  11 : return TR("N° d'identification du PS remplaçant");
                  case  12 : return TR("Clé du N° d'identification du PS remplaçant");
                  case  13 : return TR("Code conventionnel");
                  case  14 : return TR("Code spécialité");
                  case  15 : return TR("Code zone tarifaire");
                  case  16 : return TR("Code zone IK");
                  case  17 : return TR("Code agrément 1");
                  case  18 : return TR("Code agrément 2");
                  case  19 : return TR("Code agrément 3");
                  case  20 : return TR("Habilitation �  signer une facture");
                  case  21 : return TR("Habilitation �  signer un lot");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  //.................... Lire Vitale ........................................................
  case 40:   {switch (champ)                                                        // 1.31
                 {case -1 :  return TR("Données Carte Vitale et Assuré");
                  case  1 :  return TR("Type de carte Vitale");
                  case  2 :  return TR("N° de série de la carte Vitale");
                  case  3 :  return TR("Type d'identification du porteur de la carte (assuré, ou ayant droit)");
                  case  4 :  return TR("N° national d'immatriculation : NIR");
                  case  5 :  return TR("Clé du NIR");
                  case  6 :  return TR("Code régime");
                  case  7 :  return TR("Caisse gestionnaire");
                  case  8 :  return TR("Centre gestionnaire");
                  case  9 :  return TR("Code gestion");
                  default :  return messageChampInconnu( groupe, champ);
                 }
             }
  case 41:   {switch (champ)
                 {case -1 :  return TR("Données Bénéficiaire et périodes de droits");             // 1.31
                  case  1 :  return TR("Nom usuel du bénéficiaire");
                  case  2 :  return TR("Nom patronymique du bénéficiaire");
                  case  3 :  return TR("Prénom du bénéficiaire");
                  case  4 :  return TR("Adresse");
                  case  5 :  return TR("Date de naissance du bénéficiaire");
                  case  6 :  return TR("Rang de naissance");
                  case  7 :  return TR("Qualité (lien de parenté)");
                  case  8 :  return TR("Identification Mutuelle");
                  case  9 :  return TR("Garanties effectives");
                  case  10 : return TR("Nombre de périodes de droits AMO");
                  case  11 : return TR("Date de début des droits AMO");
                  case  12 : return TR("Date de fin des droits AMO");
                  case  13 : return TR("Nombre de périodes de droits Mutuelle");
                  case  14 : return TR("Date de début des droits Mutuelle");
                  case  15 : return TR("Date de fin des droits Mutuelle");
                  case  16 : return TR("Nombre de périodes de code couverture");
                  case  17 : return TR("Date de début code couverture");
                  case  18 : return TR("Date de fin code couverture");
                  case  19 : return TR("Code ALD");
                  case  20 : return TR("Code situation");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 101:  {switch (champ)
                 {case -1 :  return TR("Données Assuré (1 occurence)");
                  case  1 :  return TR("Type de carte Vitale");
                  case  2 :  return TR("N° de série de la carte Vitale");
                  case  3 :  return TR("Date de fin de validité de la carte Vitale");
                  case  4 :  return TR("Données administration carte RUF1");
                  case  5 :  return TR("Données administration carte RUF2");
                  case  6 :  return TR("Données RUF administration");
                  case  7 :  return TR("Type d'identification du porteur de carte (assuré ou ayant droit)");
                  case  8 :  return TR("N° national d'immatriculation : NIR");
                  case  9 :  return TR("Clé du NIR");
                  case  10 : return TR("Code régime");
                  case  11 : return TR("Caisse gestionnaire");
                  case  12 : return TR("Centre gestionnaire");
                  case  13 : return TR("Code gestion");
                  case  14 : return TR("Données RUF Famille");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 102:  {switch (champ)
                 {case -1 :  return TR("Service AMO Famille (0 �  1 occurence)");
                  case  1 :  return TR("Code service AMO Famille");
                  case  2 :  return TR("Date de début du service AMO Famille");
                  case  3 :  return TR("Date de fin du service AMO Famille");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 103:  {switch (champ)
                 {case -1 :  return TR("Données Accident du Travail (0 �  1 occurence)");
                  case  1 :  return TR("Organisme gestionnaire du risque AT");                                             // AT 0
                  case  2 :  return TR("Code AT BGDH (ce champ est non renseigné pour la première occurrence)");
                  case  3 :  return TR("Identifiant AT (ce champ est non renseigné pour la première occurrence)");
                  case  4 :  return TR("Organisme gestionnaire AT1");                                                      // AT 1
                  case  5 :  return TR("Code AT1 BGDH (Code Budget Global Des Hôpitaux)");
                  case  6 :  return TR("Identifiant AT1");
                  case  7 :  return TR("Organisme gestionnaire AT2)");                                                     // AT 2
                  case  8 :  return TR("Code AT2 BGDH (Code Budget Global Des Hôpitaux)");
                  case  9 :  return TR("Identifiant AT2");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 104:  {switch (champ)
                 {case -1 :  return TR("Données Bénéficiaire (1 �  6 occurences)");
                  case  1 :  return TR("Nom usuel du bénéficiaire");
                  case  2 :  return TR("Nom patronymique du bénéficiaire");
                  case  3 :  return TR("Prénom du bénéficiaire");
                  case  4 :  return TR("Adresse ligne 1 (chez..., N° appt, ou de boîte �  lettres, escalier, couloir, étage)");
                  case  5 :  return TR("Adresse ligne 2 (résidence, bâtiment, immeuble, tour, entrée)");
                  case  6 :  return TR("Adresse ligne 3 (N° et libellé de la voie)");
                  case  7 :  return TR("Adresse ligne 4 (Lieu-dit ou service particulier de distribution)");
                  case  8 :  return TR("Adresse ligne 5 (code postal et localité)");
                  case  9 :  return TR("NIR certifié");
                  case  10 : return TR("Clé du NIR certifié");
                  case  11 : return TR("Date de certification du NIR");
                  case  12 : return TR("Date de naissance du bénéficiaire");
                  case  13 : return TR("Rang de naissance du bénéficiaire");
                  case  14 : return TR("Qualité (lien de parenté entre l'ayant-droit et l'assuré)");
                  case  15 : return TR("Code service AMO bénéficiaire (00 Pas de service sinon CMU Gérée par 01-AMO 02-AMC-AMO 03-AMC 10-Sortant)");
                  case  16 : return TR("Date de début de service AMO bénéficiaire");
                  case  17 : return TR("Date de fin de service AMO bénéficiaire");
                  case  18 : return TR("Données RUF AMO bénéficiaire");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 105:  {switch (champ)
                 {case -1 :  return TR("Période de droits AMO (0 �  3 occurences)");
                  case  1 :  return TR("Date de début droits AMO");
                  case  2 :  return TR("Date de fin droits AMO");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 106:  {switch (champ)
                 {case -1 :  return TR("Période de code couverture (0 �  7 occurences)");
                  case  1 :  return TR("Date de début code couverture");
                  case  2 :  return TR("Date de fin code couverture");
                  case  3 :  return TR("Code ALD");
                  case  4 :  return TR("Code situation");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 107:  {switch (champ)
                 {case -1 :  return TR("Données Mutuelle (0 �  1 occurence)");
                  case  1 :  return TR("Identification mutuelle");
                  case  2 :  return TR("Garanties effectives");
                  case  3 :  return TR("Indicateur de traitement mutuelle");
                  case  4 :  return TR("Type de services associés");
                  case  5 :  return TR("Services associés au contrat");
                  case  6 :  return TR("Code aiguillage STS");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 108:  {switch (champ)
                 {case -1 :  return TR("Période de droits Mutuelle (0 �  3 occurences par bénéficiaire)");
                  case  1 :  return TR("Date de début droits Mutuelle");
                  case  2 :  return TR("Date de fin droits Mutuelle");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 109:  {switch (champ)
                 {case -1 :  return TR("Données de la Complémentaire de santé (0 �  1 occurence par bénéficiaire)");
                  case  1 :  return TR("Numéro complémentaire B2");
                  case  2 :  return TR("Numéro complémentaire EDI");
                  case  3 :  return TR("Numéro d'adhérent AMC");
                  case  4 :  return TR("Indicateur de traitement AMC (service accepté par AMC : Tiers payant, CMU, DRE etc..)");
                  case  5 :  return TR("Date de début de validité de présence des données AMC");
                  case  6 :  return TR("Date de fin de validité de présence des données AMC");
                  case  7 :  return TR("Code de routage AMC");
                  case  8 :  return TR("Identifiant d'hôte");
                  case  9 :  return TR("Nom de domaine AMC");
                  case  10 : return TR("Code aiguillage STS");
                  case  11 : return TR("Type de services associés");
                  case  12 : return TR("Services associés au contrat");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 111:  {switch (champ)
                 {case -1 :  return TR("Zone Commune (0 �  1 occurence par bénéficiaire)");
                  case  1 :  return TR("Zone commune complémentaire (RUF bénéficiaire)");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 60:   {switch (champ)
                 {case -1 :  return TR("En-tête de configuration");
                  case  1 :  return TR("N° de version du SSV");
                  case  2 :  return TR("N° de version du GALSS");
                  case  3 :  return TR("N° de version du PSS");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 61:   {switch (champ)
                 {case -1 :  return TR("Configuration lecteur");
                  case  1 :  return TR("Nom du constructeur du lecteur");
                  case  2 :  return TR("Type du lecteur");
                  case  3 :  return TR("N° de série du lecteur");
                  case  4 :  return TR("OS du lecteur");
                  case  5 :  return TR("Nombre de logiciel du lecteur");
                  case  6 :  return TR("Nom du logiciel du lecteur");
                  case  7 :  return TR("N° de version du logiciel du lecteur");
                  case  8 :  return TR("Date et heure du lecteur");
                  case  9 :  return TR("Checksum du logiciel du lecteur");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 62:   {switch (champ)
                 {case -1 :  return TR("Composant SSV");
                  case  1 :  return TR("N° de version de la bibliothèque des SSV");
                  case  2 :  return TR("Libellé du fichier des tables des SSV");
                  case  3 :  return TR("N° de version du fichier des tables des SSV");
                  case  4 :  return TR("Nombre des fichiers contenant des tables d'organisme destinataire");
                  case  5 :  return TR("Code régime des organismes destinataires contenus dans le fichier");
                  case  6 :  return TR("N° version (RUF)");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 63:   {switch (champ)
                 {case -1 :  return TR("Composant GALSS");
                  case  1 :  return TR("N° de version de la bibliothèque des fonctions du GALSS");
                  case  2 :  return TR("N° de version du serveur des fonctions du GALSS");
                  case  3 :  return TR("N° de version du client des fonctions du GALSS");
                  case  4 :  return TR("N° de version de la bibliothèque de gestion du fichier de configuration du GALSS");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
 case 64:    {switch (champ)
                 {case -1 :  return TR("Composant SESAM-Vitale");
                  case  1 :  return TR("Identifiant du composant pour le suivi de parc");
                  case  2 :  return TR("Libellé descriptif du composant");
                  case  3 :  return TR("N° de version du composant");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1110: {switch (champ)
                 {case -1 :  return TR("Groupe Identification facture");
                  case  1 :  return TR("N° de facture");
                  case  2 :  return TR("Nature d'opération");
                  case  3 :  return TR("Date d'élaboration de la facture");
                  case  4 :  return TR("N° logique de la situation de facturation du PS");
                  case  5 :  return TR("N° de facture rectifié");
                  case  6 :  return TR("Date d'élaboration de la facture rectifiée");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1120: {switch (champ)
                 {case -1 :  return TR("Groupe Identification du Professionnel de santé");
                  case  1 :  return TR("N° d'identification de facturation du Professionnel de santé");
                  case  2 :  return TR("Clé du N° d'identification de facturation du Professionnel de santé");
                  case  3 :  return TR("Nom du Professionnel de santé");
                  case  4 :  return TR("Prénom du Professionnel de santé");
                  case  5 :  return TR("Code conventionnel sous lequel exerce le Professionnel de santé");
                  case  6 :  return TR("Code spécialité");
                  case  7 :  return TR("Code zone tarifaire");
                  case  8 :  return TR("Code Zone IK");
                  case  9 :  return TR("Code agrément 1");
                  case  10:  return TR("Code agrément 2");
                  case  11:  return TR("Code agrément 3");
                  case  12:  return TR("Conditions d'exercice (Libéral L  Salarié S)");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1130: {switch (champ)
                 {case -1 :  return TR("Groupe Identification du bénéficiaire");
                  case  1 :  return TR("Type d'identification du bénéficiaire");
                  case  2 :  return TR("N° National d'immatriculation NIR");
                  case  3 :  return TR("Clé du NIR");
                  case  4 :  return TR("Date de naissance du bénéficiaire");
                  case  5 :  return TR("Rang de naissance du bénéficiaire");
                  case  6 :  return TR("Qualité");
                  case  7 :  return TR("Code couverture code ALD Code situation (optionnel en non sécurisé)");
                  case  8 :  return TR("Code de présentation du carnet Médical");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1140: {switch (champ)
                 {case -1 :  return TR("Groupe Identification Structure");
                  case  1 :  return TR("Type d'identification de Structure");
                  case  2 :  return TR("N° Identification de structure dans laquelle le professionnel de santé exerce");
                  case  3 :  return TR("Clé du N° Identification de structure");
                  case  4 :  return TR("Nom de la structure");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1150: {switch (champ)
                 {case -1 :  return TR("Groupe Identification Remplaçant");
                  case  1 :  return TR("N° d'identification de facturation du Professionnel de santé Remplaçant");
                  case  2 :  return TR("Clé du N° d'identification de facturation");
                  case  3 :  return TR("Nom du Professionnel de santé Remplaçant");
                  case  4 :  return TR("Prénom du Professionnel de santé Remplaçant");
                  case  5 :  return TR("Indicateur de signature (facture élaborée par un remplaçant ou sous sa responsabilité) ");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1160: {switch (champ)
                 {case -1 :  return TR("Groupe Identification Individu");
                  case  1 :  return TR("N° national d'immatriculation individu");
                  case  2 :  return TR("Clé du N° national d'immatriculation individu");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1220: {switch (champ)
                 {case -1 :  return TR("Groupe Arrêt de travail prescrit");
                  case  1 :  return TR("Nombre de jours d'arrêt de travail prescrit");
                  case  2 :  return TR("Date de fin d'arrêt de travail");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1310: {switch (champ)
                 {case -1 :  return TR("Groupe Organismes AMO (obligatoire)");
                  case  1 :  return TR("Code régime");
                  case  2 :  return TR("Caisse gestionnaire");
                  case  3 :  return TR("Centre gestionnaire");
                  case  4 :  return TR("Code organisme destinataire");
                  case  5 :  return TR("Code centre informatique");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  case 1321: {switch (champ)
                 {case -1 :  return TR("Groupe Organismes AMC (complémentaire)");
                  case  1 :  return TR("Type de contrat");
                  case  2 :  return TR("N° organisme complémentaire (carte vitale ou autre support)");
                  case  3 :  return TR("N° d'adhérent �  un organisme complémentaire");
                  case  4 :  return TR("Donées AMC Zone 1");
                  case  5 :  return TR("Donées AMC Zone 2");
                  case  6 :  return TR("N° organisme complémentaire EDI (carte vitale ou autre support)");
                  case  7 :  return TR("Code application (99 ou 98)");
                  case  8 :  return TR("Code de routage flux AMC (carte vitale table conversion ou autre support)");
                  case  9 :  return TR("Domaine (carte vitale table conversion ou autre support)");
                  case  10:  return TR("Indicateur de traitement AMC(carte vitale ou autre support) ");
                  case  11:  return TR("Identifiant hote gestionnaire (carte vitale table conversion ou autre support)");
                  case  12:  return TR("Identifiant de l'assuré AMC (carte vitale ou autre support AMC)");
                  case  13:  return TR("Zone supplémentaire non certifiée (Certifiée ou non certifiée si type de contrat 99");
                  default  : return messageChampInconnu( groupe, champ);
                 }
             }
  default: return TR("groupe inconnu : ")  + QString::number(groupe);
 }
}
//--------------------------------------------- messageChampInconnu -------------------------------------------
QString        CSesam_API_InfoGroupe::messageChampInconnu(int groupe, int champ)
{ return TR("code champ inconnu : ") + QString::number(champ) + TR(" pour ce groupe : ") + QString::number(groupe);
}

