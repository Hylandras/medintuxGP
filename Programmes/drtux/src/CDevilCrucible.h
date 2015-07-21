
/*********************************** CDevilCrucible.h *****************************
 *                                                                                *
 *   #include "CDevilCrucible.h"                                                  *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
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
#ifndef  CDEVILCRUCIBLE_H
#define  CDEVILCRUCIBLE_H


#include <qstring.h>
#include <qmap.h>

#include <stdlib.h>
#include <qobject.h>
#include <math.h>
#include <qdatetime.h>
#include <qmemarray.h>
#include <qpixmap.h>
#include <qpainter.h>

#include "CApp.h"
#include "CDevilGraph.h"
#include "CDevilCalc.h"
#include "CRubList.h"
#include "CMoteur_Base.h"
#include "../../MedinTuxTools/CMedicaBase.h"
#include "Rubriques.h"


typedef  void (CDevilCrucible::*PrtItemScript)(QString&, QString, QString&);
typedef  QString (CDevilCrucible::*PrtFunctionScript)(QString&, QString&);


/*! \class CDevilCrucible
 *  \brief Classe de script de MedinTux.
 * DrTux possède un langage de script permettant d'accéder à diverses fonctions et informations du dossier patient.
 * L'appel aux éléments de script se fait dans les masques de documents. Les instructions de script doivent être délimitées par des accolade {{ INSTRUCTION }}. Le langage de script de DrTux est récursif et permet l'utilisation d'élément de script à l'intérieur même d'une autre instruction de script. Exemple :
 * {{:: Calc ( 5, *, {{POIDS PATIENT}} ) }} -> calculera le poids multiplié par 5.
 * Le texte de script est directement remplacé par le résultat de l'instruction exécutée. Dans notre exemple, le script sera exécuté en deux fois.
 * 1 = {{:: Calc ( 5, *, 30 ) }}  puis
 * 2 = 150
 * Ce langage de script permet de créer des formulaires dynamique, des protocoles de soins...
 *
 * Divers éléments sont accessibles :
 * - les informations concernant le patient (items),
 * - les informations concernant le praticien (items),
 * - de nombreuses fonctions d'extraction de données, de calcul, de tri... (fonctions)
 *
 * Cette classe une fois instanciée correctement donne la possibilité d'interpréter le script placé dans le document passé en paramètre. Pour interpréter le script il faut appeler DoMixture.
 *
 * La fonction GetResolvTokenValue() remplace un item de script (un item est non précédé par les deux fois deux points '::' au sein des accolades) par son interprétation. Les fonctions sont explosées selon la première lettre de l'item de script. Les fonctions se nomment exeItemsFrom_A_To_D(), exeItemsFrom_E_To_H(), exeItemsFrom_I_To_L(), exeItemsFrom_M_To_P(), exeItemsFrom_Q_To_T(), exeItemsFrom_U_To_X(), exeItemsFrom_Y_To_Z(). Une tableau de pointeur de fonction (ItemDispatcher pour les items de script et FunctionDispatcher pour les fonctions de script) défini dans le constructeur de la classe est chargé de l'aiguillage.
 *
 *
 * <b>Fonctions générales de la classe :</b>
 * - DoMixture() : fonction derésolution des macros et scripts contenus dans le document afférent et donné lors de la
 *                 construction de la classe, cette méthode est la seule de la classe CDevilCrucible à devoir être appelée,
 *                 avec bien sûr le constructeur de la classe.
 */


//====================================== CDevilCrucible ==============================================================

class CDevilCrucible
{
public:
  enum ACTION_TYPE { DoAll=0, JustForWatch=1};
 /*! \brief Constructeur de la classe CDevilCrucible par lequel est précisé le contexte du dossier dans les paramètres d'entrée
 */

     CDevilCrucible(QString                *pDocument     ,                            /*!< texte du document à fusionner */
                    CMoteurBase            *pCMoteurBase  ,                            /*!< moteur de base de données */
                    CMedicaBase            *pCMedicaBase  ,                            /*!< moteur de base de données medicamenteuses */
                    RUBREC_LIST            *pRubList      ,                            /*!< liste des documents composant le dossier patient */
                    QString                &identPrimKey  ,                            /*!< il faut les renseignements sur le patient */
                    const DOCUMENT_DISPLAY_MAP  *pCurrentRubIdMap,                           /*!< liste des rubriques courantes (affichées) */
                    QString                &userActuelPk  ,                            /*!< utilisateur courant du programme */
                    CRubRecord            *pCRubCurrentRecord,                         /*!< pointeur sur l'enregistrement rubrique en cours (si il y en a un) */
                    VAR_MAP               *pVariables        ,                         /*!< pointeur sur la liste des variables (zero par defaut) */
                    ACTION_TYPE            isJustForWatch=CDevilCrucible::DoAll        /*!< to specifie that fusion must be doo just for see */
                   );

     ~CDevilCrucible();

// ****************************************************************************************************** //
// *                                    FONCTIONS GENERALES D'INTERPRETATION                            * //
// ****************************************************************************************************** //
 /*! \brief Cette fonction est la seule de la classe CDevilCrucible à devoir être appelée. Sa mission est de résoudre toutes les macros et scripts du document.
 * De par la complexité et la richesse du langage de script, le contexte entier du dossier est fourni lors de l'appel
 * du constrcteur de la classe.
 * \return neant
 */
    void                DoMixture               ();
    CMoteurBase*        GetMB                   () { return m_pMB; }
    QString             GetMIdentPrimKey        () { return m_IdentPrimKey; }
    QString             GetUserDocPk            () { return m_UserDocPk; }
    void                regenerateDisplayMap    ();

// ****************************************************************************************************** //
// *                                    FONCTIONS D'EXTRACTION                                          * //
// ****************************************************************************************************** //


     CRubRecord             *m_pCRubCurrentRecord;/*!< current  rubrique record  (if exist) */
     VAR_MAP                *m_pVariables;        /*!< contiendra la liste des variables locales des scripts */
     CMoteurBase            *m_pMB;               /*!< pointer to CMoteurBase class to provide database functions manipulation's */
     QString                 m_IdentPrimKey;      /*!< patient identity primary key on m_DOSS_IDENT_TBL_NAME */
     QString                 m_UserDocPk;         /*!< current  user primary key on m_USER_IDENT_TBL_NAME from document user */
     RUBREC_LIST            *m_pRubList;          /*!< pointer to RUBREC_LIST documents list of the patient */
     DOCUMENT_DISPLAY_MAP   *m_pCurDisplayDocMap; /*!< pointer to DOCUMENT_DISPLAY_MAP, current displaying documents list */
     CMedicaBase            *m_pMD;               /*!< pointer to CMedicaBase class to provide drugs database functions manipulation's */
     QString                *m_pDocument;         /*!< QString pointer to the text to be modified */
     QString                 m_UserActuelPk;      /*!< current  user primary key on m_USER_IDENT_TBL_NAME from actual   user */
     int                     m_isJustForWatch;    /*!< to specifie that fusion must be doo just for see */
     QString                 m_afterDataTag;
protected :
     bool                    m_VariablesToDelete; /*!< contiendra un flag indiquant si c'est au destructeur de detruire la liste des variables */

     QString            GetResolvTokenValue           (QString &document,int pos_deb, int len);
     int                DoMixture                     (int pos_deb, int pos_max);
     int                ReplaceTokenByValue           (QString &document,int pos_deb, int len,  int *pDeltaLen =0);
     QString            IdentValidCarToFieldName      (int c);
     void               DoFusionIntervenantStruct     (const char *primKeyPat ,
                                                       const QString &specialite,
                                                       QString validStr,
                                                       QString &prefix_ID,
                                                       const QString &style,
                                                       const QString &decalage,
                                                       QString &text);
    void               exeItemsFrom_I_To_L  (QString &resolvToken, QString token, QString &document);

};


#endif
