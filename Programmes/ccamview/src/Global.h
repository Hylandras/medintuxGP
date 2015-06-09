/**************************** Global.h *************************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef  CGESTINIGLOBAL_H
#define  CGESTINIGLOBAL_H

//=============================================== INCLUDES ===================================================================
#include <qstring.h>
#include <qdatetime.h>
#include <qstringlist.h>
//#include <qvaluelist.h>
#include <Q3ValueList>
#include "../../MedinTuxTools-QT4/CGestIni.h"

#ifdef Q_WS_X11
    #define NAME_APPLI        "ccamview"
    #define NAME_APPLI_EXT    ""
#else
    #define NAME_APPLI        "ccamview"
    #define NAME_APPLI_EXT    ".exe"
#endif

#define SYS_FOLDER "Ressources"
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))
//........................... Classe d'echange pour la fonction: .............................................................
//                            long GetActesAssocies(QString &codeCCAM, CODE_ASSO_LIST codeAssoList);
class CODE_ASSO_STRUCT
    {
    public:
        CODE_ASSO_STRUCT(){};
        CODE_ASSO_STRUCT( const QString& code,
                          const QString& libelle,
                          const QString& activite,
                          const QString& compoAsso,
                          const QString& regle )
            : m_CodeCCAM(code), m_LibelleActe(libelle), m_Activite(activite), m_IsCompOrAsso(compoAsso), m_regleAsso(regle)
        {}
    public:
       QString  m_CodeCCAM;
       QString  m_LibelleActe;
       QString  m_Activite;
       QString  m_IsCompOrAsso;
       QString  m_regleAsso;
    };



typedef Q3ValueList<CODE_ASSO_STRUCT> CODE_ASSO_LIST;

//........................... structure pour le dialogue Dlg_ActeProperty .............................................................
typedef struct _CCAM_ACTE
    {QString     m_CodeCCAM;
     QString     m_Code_asso;
     QString     m_Activite;
     QString     m_Phase;
     QString     m_Modificateur1;
     QString     m_Modificateur2;
     QString     m_Modificateur3;
     QString     m_Modificateur4;
     QString     m_NidPraticien;
     QDate       m_Date;
     QTime       m_Heure;
     QString     m_PraticienName;
     QString     m_LibelleActe;
     QString     m_Droits;
     QStringList m_ActesAssocies;
    }CCAM_ACTE;

//........................... structure pour l'impression ............................................................................
typedef struct _IDENT_PAT
    {QString m_Nom;
     QString m_Prenom;
     QString m_NomNaissance;
     QString m_Adresse1;
     QString m_Adresse2;
     QString m_CodePostal;
     QString m_Ville;
     QString m_VilleNss;
     QString m_Tel;
     QString m_DateNss;
     QString m_Sexe;
     QString m_NumSecu;
     QString m_NumPatient;
     QString m_NumDossier;
     QString m_DateEntree;
     QString m_DateSortie;
     QString m_BaseTravail;
     QString m_PraticienNid;
     QString m_Utilisateur;
    }IDENT_PAT;
//........................... structure pour les entites ............................................................................
typedef struct _NB_ENT
    {QString m_Numero;
     QString m_Rubrique;
     long m_Nombre;
    }NB_ENT;

typedef struct _ENT_ACTE
    {QString m_CodeActe;
     QString m_LibelleCourt;
     QString m_TypeActe;
     QString m_Sexe;
     QString m_DateCreation;
     QString m_DateFin;
     QString m_Assurance1;
     QString m_Assurance2;
     QString m_Assurance3;
     QString m_Assurance4;
     QString m_Assurance5;
     QString m_Assurance6;
     QString m_Assurance7;
     QString m_Assurance8;
     QString m_Assurance9;
     QString m_Assurance10;
     QString m_Deplacement;
     QString m_Arborescence1;
     QString m_Arborescence2;
     QString m_Arborescence3;
     QString m_Arborescence4;
     QString m_Arborescence5;
     QString m_Arborescence6;
     QString m_Arborescence7;
     QString m_Arborescence8;
     QString m_Arborescence9;
     QString m_Arborescence10;
     QString m_PlaceArbo;
     QString m_CodeStructure;
     QString m_CodePrecedent;
     QString m_CodeSuivant;
     QString m_LibelleLong;
     QString m_Ccam26;
     QString m_Ccam27;
     QString m_Ccam28;
     QString m_Ccam22;
     QString m_Ccam24;
     QString m_Ccam25;
     QString m_Ccam31;
     QString m_Ccam34;
    }ENT_ACTE;

typedef struct _NOTE_ACTE
   {QString m_TypeNote;
    QString m_TexteNote;
   }NOTE_ACTE;

typedef struct _ENT_ACTIVITE
   {QString m_CodeActe;
    QString m_CodeActivite;
   }ENT_ACTIVITE;

typedef struct _ENT_HISTO_ACTE
   {QString m_DateEffet;
    QString m_DateAM;
    QString m_DateJO;
    QString m_CategorieMedicale;
    QString m_SpeExecPermise1;
    QString m_SpeExecPermise2;
    QString m_SpeExecPermise3;
    QString m_SpeExecPermise4;
    QString m_SpeExecPermise5;
    QString m_SpeExecPermise6;
    QString m_SpeExecPermise7;
    QString m_SpeExecPermise8;
    QString m_SpeExecPermise9;
    QString m_SpeExecPermise10;
    QString m_CodeRegroupement;
   }ENT_HISTO_ACTE;

typedef struct _ENT_HISTO_ACTIVITE
   {QString m_DateEffet;
    QString m_DateAM;
    QString m_DateJO;
   }ENT_HISTO_ACTIVITE;

typedef struct _ENT_COMPATIBILITE
   {QString m_CodeActe;
    QString m_CodeActivite;
    QString m_RegleTarifaire;
   }ENT_COMPATIBILITE;

typedef struct _ENT_PHASE
   {QString m_CodeActe;
    QString m_CodeActivite;
    QString m_CodePhase;
    QString m_NbDents;
    QString m_AgeMini;
    QString m_AgeMaxi;
    QString m_ICR;
    QString m_Classant;
   }ENT_PHASE;

typedef struct _ENT_ARBO
   {QString m_CodeMenu;
    QString m_CodePere;
    QString m_Rang;
    QString m_Libelle;
   }ENT_ARBO;

typedef struct _ENT_NOTEARBO
   {QString m_CodeMenu;
    QString m_TypeNote;
    QString m_TexteNote;
   }ENT_NOTEARBO;

typedef struct _ENT_PREC
   {int m_Entite;
    int m_Rubrique;
    int m_Sequence;
   }ENT_PREC;

typedef struct _TAB_ACTIVITE
   {QString m_CodeActivite;
    QString m_Libelle;
   }TAB_ACTIVITE;

typedef struct _TAB_ASSO
   {QString m_CodeAsso;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Coefficient;
   }TAB_ASSO;

typedef struct _TAB_MODIFICATEUR
   {QString m_CodeModificateur;
    QString m_Libelle;
   }TAB_MODIFICATEUR;

typedef struct _TYPE_NOTE
   {QString m_TypeNote;
    QString m_Libelle;
   }TYPE_NOTE;

typedef struct _TAB_PHASE
   {QString m_CodePhase;
    QString m_Libelle;
   }TAB_PHASE;

typedef struct _VER_BASE
   {QString m_IdFichier;
    QString m_DateCreation;
    QString m_DateIntegration;
    QString m_Chrono;
    QString m_TypeFichier;
    QString m_NumVersion;
    QString m_Status;
   }VER_BASE;

// Debut des mises à jours pour les tarifs
// Regles tarifaires
typedef struct _TAB_TB03
   {QString m_RegleTarifaire;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Coefficient;
   }TAB_TB03;
// Regroupement des spécialités
typedef struct _TAB_TB04
   {QString m_Specialite;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_ClasseSpecialite;
   }TAB_TB04;
// Association - code nature prestation & type de forfait
typedef struct _TAB_TB05
   {QString m_NatPrestation;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_TypeForfait;
   }TAB_TB05;
// Compatibilité - code modificateur & age du bénéficiaire
typedef struct _TAB_TB06
   {QString m_CodeModificateur;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_UniteAgeMini;
    QString m_AgeMini;
    QString m_UniteAgeMaxi;
    QString m_AgeMaxi;
   }TAB_TB06;
// Seuil minimum pour éxonération TM
typedef struct _TAB_TB07
   {QString m_DateDebut;
    QString m_DateFin;
    QString m_S1;
    QString m_S2;
    QString m_S3;
    QString m_S4;
   }TAB_TB07;
// Table des jours fériés nationaux et spécifiques par caisse
typedef struct _TAB_TB08
   {QString m_Caisse;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_TypeFerie;
    QString m_Jour;
   }TAB_TB08;
// Compatibilité entre modificateurs
typedef struct _TAB_TB09
   {QString m_CodeModif;
    QString m_ModifCompat;
    QString m_DateDebut;
    QString m_DateFin;
   }TAB_TB09;
// Cohérence des modificateurs entre les différents codes activité d'un même acte
typedef struct _TAB_TB10
   {QString m_Modificateur;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Coherence;
    QString m_PresMultiple;
   }TAB_TB10;
// Association code modificateur et coefficient ou forfait
typedef struct _TAB_TB11
   {QString m_Modificateur;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Forfait;
    QString m_Coef;
   }TAB_TB11;
// Réduction base de remboursement pour non conventionnés
typedef struct _TAB_TB12
   {QString m_DateDebut;
    QString m_DateFin;
    QString m_Forfait;
    QString m_Coef;
   }TAB_TB12;
// Nature de prestation caractérisant le codage CCAM
typedef struct _TAB_TB13
   {QString m_Prestation;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Equiv;
   }TAB_TB13;
// Association Discipline médico tarifaire (DMT) et classe de discipline
typedef struct _TAB_TB14
   {QString m_Dmt;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Classe;
   }TAB_TB14;
// Modificateur donnant lieu à information aux OC
typedef struct _TAB_TB15
   {QString m_Modificateur;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_ModifOC;
   }TAB_TB15;
// Date d'obligation du codage CCAM pour les nature de prestation NGAP
typedef struct _TAB_TB17
   {QString m_Prestation;
    QString m_Specialite;
    QString m_DateObligation;
   }TAB_TB17;
// Localisation des dents
typedef struct _TAB_TB18
   {QString m_DateDebut;
    QString m_DateFin;
    QString m_Localisation;
   }TAB_TB18;
// Nombre de modificateurs tarifiants
typedef struct _TAB_TB19
   {QString m_DateDebut;
    QString m_DateFin;
    QString m_NbModif;
   }TAB_TB19;
// Concepts divers CCAM
typedef struct _TAB_TB20
   {QString m_Concept;
    QString m_DateDebut;
    QString m_DateFin;
    QString m_Valeur;
   }TAB_TB20;
// Table de codification des conditions générales - Enregistrement 051
typedef struct _TAB_CONDGEN
   {QString m_Condition;
    QString m_Libelle;
   }TAB_CONDGEN;
// Table de codification des classes de DMT - Enregistrement 052
typedef struct _TAB_CLASSE_DMT
   {QString m_ClasseDMT;
    QString m_Libelle;
   }TAB_CLASSE_DMT;
// Table de codification des exonérations du ticket modérateur - Enregistrement 053
typedef struct _TAB_EXO_TM
   {QString m_CodeExo;
    QString m_Libelle;
   }TAB_EXO_TM;
// Table de codification des natures d'assurances - Enregistrement 054
typedef struct _TAB_NAT_ASS
   {QString m_NatAss;
    QString m_Libelle;
   }TAB_NAT_ASS;
// Table de codification des codes d'admission au remboursement - Enregistrement 055
typedef struct _TAB_REMBOURS
   {QString m_AdmRem;
    QString m_Libelle;
   }TAB_REMBOURS;
// Table de codification des frais de déplacement - Enregistrement 056
typedef struct _TAB_FRAIS_DEP
   {QString m_Code;
    QString m_Libelle;
   }TAB_FRAIS_DEP;
// Table de codification des types d'actes - Enregistrement 057
typedef struct _TAB_TYPE_ACTE
   {QString m_Code;
    QString m_Libelle;
   }TAB_TYPE_ACTE;
// Table de codification des types de forfait - Enregistrement 058
typedef struct _TAB_TYPE_FORFAIT
   {QString m_Code;
    QString m_Libelle;
   }TAB_TYPE_FORFAIT;
// Table de codification des extensions documentaires - Enregistrement 059
typedef struct _TAB_EXT_DOC
   {QString m_Code;
    QString m_Libelle;
   }TAB_EXT_DOC;
// Table de codification des catégories médicales - Enregistrement 061
typedef struct _TAB_CAT_MED
   {QString m_Code;
    QString m_Libelle;
   }TAB_CAT_MED;
// Table de codification des codes de regroupement - Enregistrement 062
typedef struct _TAB_COD_REGROUP
   {QString m_Code;
    QString m_Libelle;
   }TAB_COD_REGROUP;
// Table de codification des catégories de spécialités - Enregistrement 063
typedef struct _TAB_CAT_SPEC
   {QString m_Code;
    QString m_Libelle;
   }TAB_CAT_SPEC;
// Table de codification des agréments radio - Enregistrement 064
typedef struct _TAB_AGR_RAD
   {QString m_Code;
    QString m_Libelle;
   }TAB_AGR_RAD;
// Table de codification des codes de paiement - Enregistrement 065
typedef struct _TAB_CODE_PAIEMENT
   {QString m_Code;
    QString m_Libelle;
   }TAB_CODE_PAIEMENT;
// Table de codification des numéros de dents - Enregistrement 067
typedef struct _TAB_NUM_DENTS
   {QString m_Code;
    QString m_Libelle;
   }TAB_NUM_DENTS;
// Table de codification des caisses des départements d'outre mer - Enregistrement 068
typedef struct _TAB_DOM
   {QString m_Code;
    QString m_Libelle;
   }TAB_DOM;
// Table de codification des codes association non prévue - Enregistrement 080
typedef struct _TAB_ASS_NPREV
   {QString m_Code;
    QString m_Libelle;
   }TAB_ASS_NPREV;
// Table de codification des règles tarifaires - Enregistrement 081
typedef struct _TAB_RGL_TARIF
   {QString m_Code;
    QString m_Libelle;
   }TAB_RGL_TARIF;
// Table de codification des spécialités - Enregistrement 082
typedef struct _TAB_SPEC
   {QString m_Code;
    QString m_Libelle;
   }TAB_SPEC;
// Table de codification des DMT - Enregistrement 084
typedef struct _TAB_DMT
   {QString m_Code;
    QString m_Libelle;
   }TAB_DMT;
// Table de compatibilité des éxonérations TM - Enregistrement 090
typedef struct _TAB_COMPAT_EXOTM
   {QString m_ExoTM;
    QString m_ExoTMCompat;
   }TAB_COMPAT_EXOTM;
// Table des valeurs d'unité d'oeuvre - Enregistrement 093
typedef struct _TAB_VAL_UNITE_OEUVRE
   {QString m_Code;
    QString m_DateEffet;
    QString m_Valeur;
   }TAB_VAL_UNITE_OEUVRE;
// Liste des mots - Enregistrement 099
typedef struct _LISTE_MOTS
   {QString m_Mot;
    QString m_Definition;
   }LISTE_MOTS;
// Conditions générales d'un acte - Enregistrement 101 51
typedef struct _COND_GEN_ACTE
   {QString m_Code;
   }COND_GEN_ACTE;
// Recommandation médicale d'un acte - Enregistrement 201 50
typedef struct _REC_MED
   {QString m_NumRecMed;
    QString m_TexteRecMed;
   }REC_MED;
// Historique niveau acte/activité/phase - Enregistrement 310
typedef struct _ENT_HISTO_PHASE
   {QString m_CodeActe;
    QString m_CodeActivite;
    QString m_CodePhase;
    QString m_DateEffet;
    QString m_DateAM;
    QString m_DateJO;
    QString m_NbMaxSeances;
    QString m_UniteOeuvre;
    QString m_CoefUO;
    QString m_CodePaiement;
    QString m_PU;
    QString m_CoefMAJDom1;
    QString m_CoefMAJDom2;
    QString m_CoefMAJDom3;
    QString m_CoefMAJDom4;
    QString m_SuppChgCab;
   }ENT_HISTO_PHASE;

// fin des mises à jours pour les tarifs


//........................... Chemins Frequements utilisés ............................................................................
extern QString       GlobalPathAppli;

//........................... Pointeur sur le chemin du fichier .ini ses parametre  ...........................................................
extern QString       Global_PathIni;        // Pointeur sur le chemin du fichier .ini
extern QString       Global_ParamData;      // Pointeur sur les parametre charges a partir du fichier .ini

//........................... Quelques fonctions Globales Hors classe .................................................................
//                                 ( c'est pas bien ça !!! mais y en a plus)
#endif

