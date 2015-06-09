 /********************************* Defines.h ************************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
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
/*! \file Defines.h
    \brief Toutes les MACROS sont définies dans ce fichier.

     #include "Defines.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef COMPTA_DEFINES
#define COMPTA_DEFINES

#define NOM_APPLICATION	"Comptabilité pour MedinTux"

#define VERSION_CODE	QObject::tr("version 1.11")
#define DATE_VERSION	QObject::tr("28 Juin 2008")


/*! \brief Définitions pour la base de données de compta */
#define BASE_COMPTA_DRIVER	"QMYSQL3"
/*! \brief Définitions pour la base de données de compta */
#define BASE_COMPTA_NOM		"comptabilite"

/*! \brief Définitions pour la base de données de drtux */
#define BASE_TUX_NOM		"DrTuxTest"
/*! \brief Définitions pour la base de données de drtux */
#define BASE_TUX_TABLE		"Personnes"


/*! \brief Définitions pour la base de données de compta : champs Chèques de la table honoraires */
#define CHEQUES	"chq"
/*! \brief Définitions pour la base de données de compta : champs Espèces de la table honoraires */
#define ESPECES	"esp"
/*! \brief Définitions pour la base de données de compta : champs CB de la table honoraires */
#define CB	"cb"
/*! \brief Définitions pour la base de données de compta : champs DAF de la table honoraires */
#define DAF	"daf"
/*! \brief Définitions pour la base de données de compta : champs du de la table honoraires */
#define CMU	"cmu"
#define Virement	"vir"
/*! \brief Définitions pour la base de données de compta : champs Autres de la table honoraires */
#define AUTRES	"autre"
/*! \brief Définitions pour la base de données de compta : champs Validé de la table honoraires */
#define VALIDE	"val"


/*! \brief Champ 'Remarque' des honoraires : Identifiant pour les chèques déposés . Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total. */
#define CHEQUE_DEPOSE		"|dep"
/*! \brief Champ 'Remarque' des honoraires : Identifiant pour les chèques déposés . Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total. */
#define CHEQUE_PAS_DEPOSE	"|chq"
/*! \brief Champ 'Remarque' des honoraires : Identifiant pour les DAF validés. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total. */
#define DAF_OK			"|dok"
/*! \brief Champ 'Remarque' des honoraires : Identifiant pour les DAF à valider. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total. */
#define DAF_PAS_OK		"|daf"
/*! \brief Champ 'Remarque' des honoraires : Identifiant pour les CMU validés. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total. */
#define CMU_OK			"|cok"
/*! \brief Champ 'Remarque' des honoraires : Identifiant pour les CMU à valider. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total. */
#define CMU_PAS_OK		"|cmu"
/*! \brief Champ 'Remarque' des honoraires : Identifiant pour l'émetteur d'un chèque. Le nom est juste après. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define EMETTEUR		"|em="
/*! \brief Champ 'Remarque' des honoraires : Identifiant précédent la date de validation comptable de l'honoraire. Le nom est juste après. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.
 \sa ComptaMoteurBase, valideHonoraires 
*/
#define HONO_VALIDE		"|OK="
/*! \brief Champ 'Remarque' des comptes : indique le compte par défaut. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define CPT_PAR_DEFAUT		"|def"
/*! \brief Champ 'Remarque' des dépôts : indique le compte sur lequel est fait le dépôt au format RIB uniquement. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define RIB_DEPOT		"|rib"
/*! \brief Champ 'Remarque' des dépôts : indique l'identifiant du compte utilisé pour le dépôt. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define ID_CPT_DEPOT		"|cpt"
/*! \brief Champ 'Blob' des dépôts : indique qu'il s'agit d'un dépôt d'espèces. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define DEPOT_ESPECES		"|esp"
/*! \brief Champ 'Remarque' des dépôts : indique le montant total du dépôt. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define TOTAL_DEPOT		"|tt="

/*! \brief Champ 'Remarque' des mouvements : indique le mode de paiement. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define PAIEMENT_CHEQUE		"|chq"
/*! \brief Champ 'Remarque' des mouvements : indique le mode de paiement. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define PAIEMENT_ESPECES	"|esp"
/*! \brief Champ 'Remarque' des mouvements : indique le mode de paiement. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define PAIEMENT_CB		"|cb_"
/*! \brief Champ 'Remarque' des mouvements : indique le mode de paiement. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define PAIEMENT_VIREMENT	"|vir"
/*! \brief Champ 'Remarque' des mouvements : indique le mode de paiement. Attention toutes les références du champs 'Remarque' des honoraires doivent commencer par '|' et faire quatre caractères de long au total.*/
#define PAIEMENT_AUTRE		"|aut"

/*! \brief Mouvements / Immobilisations : indique l'id des immobilisations dans la table "mouvements_disponibles".*/
#define ID_IMMOBILISATIONS	41
/*! \brief Mouvements / Immobilisations : dans le champs remarque ce marqueur indique que le mouvement est lié à une immobilisation dont l'id est précisée juste après.*/
#define IS_IMMOBILISATION	"|im="
/*! \brief Mouvements / Immobilisations : dans le champs remarque ce marqueur indique le montant de la VNC de l'immobilisation.*/
#define VNC_IMMOBILISATION	"|vnc"
/*! \brief Mouvements / Immobilisations : dans le champs remarque ce marqueur indique le montant du taux de l'immobilisation.*/
#define TAUX_IMMOBILISATION	"|tx="


#include <qobject.h>
#define TR QObject::tr

#define TYP_HONO	1
#define TYP_BANQUE      2
#define TYP_ACTES	3
#define TYP_LIVRE	4
#define TYP_DAF		5
#define TYP_MOUVEMENTS	6
#define TYP_RELEVE	7
#define TYP_LIVRES	8
#define TYP_PARAM	10
#define TYP_CMU		11

/*! \brief Pour la fonction de création du filtre dans ComptaMoteurBase. Ne renvoie que les dates. */
#define DATES_ONLY	1


#endif

