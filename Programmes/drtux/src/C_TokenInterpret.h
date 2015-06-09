/*********************************** C_TokenInterpret.h *****************************
 *                                                                                *
 *   #include "C_TokenInterpret.h"                                                  *
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
#ifndef  C_TOKENINTERPRET_H
#define  C_TOKENINTERPRET_H


#include <qstring.h>
#include <qstringlist.h>
#include <stdlib.h>

#include <qpoint.h>
//.......... forUse_G_MacroTab_List ............
static int forUse_G_MacroTab_List();

class CDevilCrucible;
class CRubRecord;
class QDate;
class QTime;
//=============================================== KEY_MACRO ==============================================================
/*! \class   KEY_MACRO
    \brief   structure associant : un pointeur sur un mot clef, et un pointeur sur la fonction à executer pour ce mot clef,
  *   ces structures sont rassemblées au sein de 26 tableaux (un par lettre de l'alphabet),
  *   chacun de ces tableaux ( G_MacroTab_A[] ... à ... G_MacroTab_X[] )  contient triées alphabetiquement toutes les structures KEY_MACRO
  *   dont le membre : m_Token commence par la lettre pour lequel le tableau est dédié.
  */
typedef struct
       {const char       *m_Token;                   /*!< pointeur sur le mot clef de la fonction ou macro */
        QString         (*m_Function)(QStringList&); /*!< pointeur la fonction à executer pour ce mot clef */
       } KEY_MACRO;

 //====================================== KEY_MACRO_TAB_DEF ==============================================================
/*! \class   KEY_MACRO_TAB_DEF
     \brief  structure stockant : un pointeur sur un tableau de KEY_MACRO et la taille en octet de ce tableau
  *   il y a 26 structures KEY_MACRO_TAB_DEF dont les 26 pointeurs sur ( G_MacroTab_A[] ... à ... G_MacroTab_X[] ) sont contenus dans le tableau G_MacroTab_List[],
  *   il y a  1 structure  KEY_MACRO_TAB_DEF par lettre de l'alphabet.
  */
typedef struct
       {KEY_MACRO        *m_MacroTab;  /*!< pointeur sur un des tableaux ( G_MacroTab_A[] ... à ... G_MacroTab_X[] ) de fonctions/clefs  KEY_MACRO[] */
        int               m_size;      /*!< taille du tableau en octets (initialisé lors de la compilation) */
       } KEY_MACRO_TAB_DEF;


//====================================== C_TokenInterpret ==============================================================
/*! \class C_TokenInterpret
 *  \brief Classe d'interpretation des macro et instructions de scripts de MedinTux.
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

 * <b> Méthode d'acés aux fonctions de la classe :</b>
 * Cette classe utilise un intertpréteur de token ou mots clefs très rapide
 * et performant basé sur deux méthodes :
 * - le premier tri très rapide se sert de la première lettre de l'instruction
 *   pour accéder en adressage direct au tableau des instructions commençant par cette lettre.
 *   chacun des pointeurs sur un de ces 26 tableaux est contenu dans un tableau de 26 pointeurs,
 *   suivant : G_MacroTab_List[]
 *
 * - chacun de ces tableaux contient une liste triée des instructions commençant par la lettre
 *   correspondant à ce tableau. Une fois ce tableau accédé, un seconde recherche dicotomique
 *   rapide (par bsearch) est effectuée afin de trouver l'item définitif correspondant au mot clef.
 *
 * Cette méthode de recherche a un défaut : elle impose que l'ensemble des fonctions soient statiques
 * afin de disposer de leurs pointeurs à la compilation.
 * Cela n'a pas trop d'mportance dans la mesure ou les fonctions de cette classe ne sont accédées
 * qu'au sein de la classe : CDevilCrucible et NULLE PART AILLEURS.
 *
 * <b>Fonctions d'extraction de la base de données patient :</b>
 * - Func_Extract_ATCD() : extrait les antécédent du terrain en cours d'affichage (celui de Atcd_Code)
 * - Func_Extract() : extrait et insère du texte du texte lui même en partant d'un délimiteur de début et un délimiteur de fin
 * - Func_Extract_Table() : extrait et insère les variables du terrain en cours
 * - Func_Extract_TTT() : extrait et insère le traitement en cours
 * - Func_Extract_VAR() : extrait et insère des éléments des variables du patient
 * - GetPoidsTaille() : A recoder
 * - GetVarNeerer() (appelé à devenir obsolete) récupère une variable à proximité d'une date
 * - GetVar() : récupère les valeurs d'une variable du terrain
 * - Terme_Obst() : calcul le terme obstétrical à la date du jour.
 *
 * <b>Fonctions d'extraction de la base de données praticien :</b>
 * - DoFusionIntervenantStruct()
 *
 * <b>Fonctions de calcul :</b>
 * - GetDDR() récupère la date des dernières règles,
 * - daysTo() calcul le nombre de jour entre deux dates
 * -
 * <b>Fonctions graphiques :</b>
 * <b>Fonctions de gestion des documents :</b>
 * - DocNameToDocNumType() transforme le type de document ("Observation","Prescription"...) en son type numérique,
  */

class C_TokenInterpret
{
 public:
  C_TokenInterpret();
 ~C_TokenInterpret();
  static void       init(CDevilCrucible *pCDevil);
  static int        ResolvFunctionToken( CDevilCrucible *pCDC, QString &resolvToken, QString token);
  static int        ResolvMacroToken(    CDevilCrucible *pCDC, QString &resolvToken, QString token);
  static QString ABS_PATH                      (QStringList &arg_list);
  static QString APPEND_DOC_IN_CLASSEUR        (QStringList &arg_list);
  static QString COPY_FILE                     (QStringList &arg_list);
  static QString CALC                          (QStringList &arg_list);
  static QString VAR_CALC                      (QStringList &arg_list);
  static QString DO_LISTE                      (QStringList &arg_list);
  static QString DAYSTO                        (QStringList &arg_list);
  static QString DELETE_FILE                   (QStringList &arg_list);
  static QString EXE_PROCESS                   (QStringList &arg_list);
  static QString EXTRACT                       (QStringList &arg_list);
  static QString EXTRACT_ATCD                  (QStringList &arg_list);
  static QString EXTRACT_TABLE                 (QStringList &arg_list);
  static QString EXTRACT_TRAITEMENT            (QStringList &arg_list);
  static QString EXTRACT_VAR                   (QStringList &arg_list);
  static QString FILL                          (QStringList &arg_list);
  static QString FORM_DATE                     (QStringList &arg_list);
  static QString FORM_DATE_HEURE               (QStringList &arg_list);
  static QString FORM_HEURE                    (QStringList &arg_list);
  static QString GET_DOC_DATE_FIN              (QStringList &arg_list);
  static QString GET_DOC_PROP_0                (QStringList &arg_list);
  static QString GET_DOC_PROP_1                (QStringList &arg_list);
  static QString GET_DOC_PROP_2                (QStringList &arg_list);
  static QString GET_DOC_PROP_3                (QStringList &arg_list);
  static QString GET_DOC_PROP_4                (QStringList &arg_list);
  static QString GRAPH                         (QStringList &arg_list);
  static QString IMPORT                        (QStringList &arg_list);
  static QString INTITULE                      (QStringList &arg_list);
  static QString INDEX_OF                      (QStringList &arg_list);
  static QString INSERT_CISP                   (QStringList &arg_list);
  static QString INSERT_IMAGE                  (QStringList &arg_list);
  static QString LENGTH                        (QStringList &arg_list);
  static QString LOWER                         (QStringList &arg_list);
  static QString CURRENT_EDITOR                (QStringList &arg_list);
  static QString NOUVEAU_DOCUMENT              (QStringList &arg_list);
  static QString PUT_VAR                       (QStringList &arg_list);
  static QString EXIT                          (QStringList &arg_list);
  static QString REINIT_CLASSEUR               (QStringList &arg_list);
  static QString REPLACE_BY                    (QStringList &arg_list);
  static QString REPLACE_IMAGE                 (QStringList &arg_list);
  static QString REPLACE_IN_FILE               (QStringList &arg_list);
  static QString RUBRIQUE_SHOW                 (QStringList &arg_list);
  static QString SET_DOC_DATE_FIN              (QStringList &arg_list);
  static QString SET_DOC_DATE_DEB              (QStringList &arg_list);
  static QString SET_DOC_PROP_0                (QStringList &arg_list);
  static QString SET_DOC_PROP_1                (QStringList &arg_list);
  static QString SET_DOC_PROP_2                (QStringList &arg_list);
  static QString SET_DOC_PROP_3                (QStringList &arg_list);
  static QString SET_DOC_PROP_4                (QStringList &arg_list);
  static QString SET_VAR                       (QStringList &arg_list);
  static QString VAR_LIST_CLEAR                (QStringList &arg_list);
  static QString VAR_SPLIT                     (QStringList &arg_list);
  static QString SOUS_TYPE                     (QStringList &arg_list);
  static QString SQL_EXEC                      (QStringList &arg_list);
  //-------------------------- SQL_WRITE -------------------------------------------
  /*! \brief Fonction qui permet de mettre a jour ou inserer des donnees dans une table SQL Si l'enregistrement existe deja il est mis a jour sinon il est insere
   *  \param arg_list const &QStringList  : reference sur la liste des arguments de la fonction
   *                       premier  argument : nom de la table a mettre a jour
   *                       deuxieme argument : condition WHERE (la mention WHERE ne doit pas etre fournie) pour selectionner les enregistrements a mettre a jour
   *                       troisieme arguement : nom du champ a mettre a jour doit etre suivi du
   *                       quatrieme arguement : valeur a mettre a jour dans ce champ.
   *                              .
   *                              .
   *                       nieme argument    : nom du champ a mettre a jour doit etre suivi du
   *                       nieme argument +1 : valeur a placer dans ce champ
   *  \return        une chaine vide si tout est Ok, sinon un message d'erreur.
   */
  static QString SQL_WRITE                     (QStringList &arg_list);
  static QString SQL_GET_DOSS_GUID             (QStringList &arg_list);
  static QString SQL_GET_DOSS_PK               (QStringList &arg_list);
  static QString SQL_GET_RUB_HEAD_PK           (QStringList &arg_list);
  static QString SQL_SELECT                    (QStringList &arg_list);
  static QString TEST                          (QStringList &arg_list);

  static QString EMAIL_MEDECIN                 (QStringList &arg_list);
  static QString TEL_1_MEDECIN                 (QStringList &arg_list);
  static QString TEL_2_MEDECIN                 (QStringList &arg_list);
  static QString TEL_3_MEDECIN                 (QStringList &arg_list);
  static QString USER_EMAIL_MEDECIN            (QStringList &arg_list);
  static QString USER_TEL_1_MEDECIN            (QStringList &arg_list);
  static QString USER_TEL_2_MEDECIN            (QStringList &arg_list);
  static QString USER_TEL_3_MEDECIN            (QStringList &arg_list);

  /*! \brief convertit en texte en enlevant les tags html : <br><b>utilisation :</b> <br>       {{<b>TO_SCRIPT</b> = <i>texte à convertir peut être sur plusieurs lignes</i> }}
  *  <br>afin que les virgules ne soient pas interrétées comme séparateur d'argument, elles sont précédées de \\
  *  et lors de l'appel du script elles sont replacées normalement par la fonction d'extraction des arguments
  */
  static QString TO_SCRIPT                     (QStringList &arg_list);
  static QString UPPER                         (QStringList &arg_list);

  static QString ADRESSE_COMPLETE_MEDECIN      (QStringList &arg_list);
  static QString ADRESSE_COMPLETE_PATIENT      (QStringList &arg_list);
  static QString ADRESSE_MEDECIN               (QStringList &arg_list);
  static QString ADRESSE_PATIENT               (QStringList &arg_list);
  static QString ADRESSE_PATIENT_UNE_LIGNE     (QStringList &arg_list);
  static QString AGE_PATIENT                   (QStringList &arg_list);
  static QString AGE_PATIENT_ABREGE            (QStringList &arg_list);
  static QString AGE_PATIENT_ANNEE_ABREGE      (QStringList &arg_list);
  static QString BMI_PATIENT                   (QStringList &arg_list);
  static QString BASE_THERAPEUTIQUE            (QStringList &arg_list);
  static QString CODE_POSTAL_PATIENT           (QStringList &arg_list);
  static QString CONVENTION_MEDECIN            (QStringList &arg_list);
  static QString CADRE                         (QStringList &arg_list);
  static QString CP_MEDECIN                    (QStringList &arg_list);
  static QString COMMIT                        (QStringList &arg_list);
  static QString DATECOURANTE                  (QStringList &arg_list);
  static QString DATE_VISITE                   (QStringList &arg_list);
  static QString DATE_CALC                     (QStringList &arg_list);
  static QString DATE_DOCUMENT                 (QStringList &arg_list);
  static QString DATE_FROMNUM                  (QStringList &arg_list);
  static QString DATE_NAISSANCE_ASSURE         (QStringList &arg_list);
  static QString DATE_NAISSANCE_PATIENT        (QStringList &arg_list);
  static QString DATE_TONUM                    (QStringList &arg_list);
  static QString DDR                           (QStringList &arg_list);
  static QString EMAIL_PATIENT                 (QStringList &arg_list);
  static QString EXPORT                        (QStringList &arg_list);
  static QString EXPORT_TABLE                  (QStringList &arg_list);
  static QString FILE_EXISTS                   (QStringList &arg_list);
  static QString GET_LAST_IMAGE_NAME           (QStringList &arg_list);
  static QString GET_LAST_IMAGE_SRC_NAME       (QStringList &arg_list);
  static QString GET_SAVE_FILE_NAME            (QStringList &arg_list);
  /*! \brief retourne le texte en cours de selection */
  static QString GET_SELECTION                 (QStringList &arg_list);
  static QString LINE_EDIT                     (QStringList &arg_list);
  static QString LIST_DIR                      (QStringList &arg_list);
  static QString HHHMM                         (QStringList &arg_list);
  static QString IDENTIFICATION_MEDECIN        (QStringList &arg_list);
  static QString INTERVENANT                   (QStringList &arg_list);
  static QString INCLUDE                       (QStringList &arg_list);
  static QString INCLUDE                       (QStringList &arg_list, QString &path);
  static QString IS_DATE_VALID                 (QStringList &arg_list);
  static QString JOIN                          (QStringList &arg_list);
  static QString KEY_VALUE                     (QStringList &arg_list);
  static QString MESSAGE_ALERT                 (QStringList &arg_list);
  static QString MESSAGE_POPUP                 (QStringList &arg_list);
  static QString NOM_MEDECIN                   (QStringList &arg_list);
  static QString NOM_PATIENT                   (QStringList &arg_list);
  static QString NOM_PRENOM_ASSURE             (QStringList &arg_list);
  static QString NOM_PRENOM_INTERVENANT        (QStringList &arg_list);
  static QString NOM_PRENOM_MEDECIN            (QStringList &arg_list);
  static QString POLITESSE_MEDECIN             (QStringList &arg_list);
  static QString NOM_PRENOM_PATIENT            (QStringList &arg_list);
  static QString NOM_NAISSANCE_PATIENT         (QStringList &arg_list);
  static QString MOVE_CURSOR                   (QStringList &arg_list);
  static QString NOMBRE_D_ENFANT_PATIENT       (QStringList &arg_list);
  static QString NOP                           (QStringList &arg_list);
  static QString MID                           (QStringList &arg_list);
  static QString LEFT                          (QStringList &arg_list);
  static QString RIGHT                         (QStringList &arg_list);
  static QString CLEAR_SPACES                  (QStringList &arg_list);
  static QString NUM_SECU_PATIENT              (QStringList &arg_list);
  static QString NUM_CLEF_PATIENT              (QStringList &arg_list);
  static QString NUM_TEL3_PATIENT              (QStringList &arg_list);
  static QString NUM_TEL2_PATIENT              (QStringList &arg_list);
  static QString NUM_TEL1_PATIENT              (QStringList &arg_list);
  static QString OPEN_FILE_NAME                (QStringList &arg_list);
  static QString OS                            (QStringList &arg_list);
  static QString POIDS_PATIENT                 (QStringList &arg_list);
  static QString PRENOM_MEDECIN                (QStringList &arg_list);
  static QString PRENOM_PATIENT                (QStringList &arg_list);
  static QString PROFESSION_PATIENT            (QStringList &arg_list);
  static QString SAVE_CURRENT_DOC              (QStringList &arg_list);
  static QString SAVE_LAST_IMAGE_NAME          (QStringList &arg_list);
  static QString SEXE_MEDECIN                  (QStringList &arg_list);
  static QString SIMPLIFY                      (QStringList &arg_list);
  static QString SEXE_PATIENT                  (QStringList &arg_list);
  static QString SPECIALITE_MEDECIN            (QStringList &arg_list);
  static QString SURFACE_PATIENT               (QStringList &arg_list);
  static QString TAILLE_PATIENT                (QStringList &arg_list);
  static QString TERME_GROSSESSE               (QStringList &arg_list);
  static QString TITRE_PATIENT                 (QStringList &arg_list);
  static QString TITRE_PS                      (QStringList &arg_list);
  static QString TO_NUM                        (QStringList &arg_list);
  static QString USER_ADRESSE_COMPLETE_MEDECIN (QStringList &arg_list);
  static QString USER_ADRESSE_MEDECIN          (QStringList &arg_list);
  static QString USER_CP_MEDECIN               (QStringList &arg_list);
  static QString USER_CONVENTION_MEDECIN       (QStringList &arg_list);
  static QString USER_IDENTIFICATION_MEDECIN   (QStringList &arg_list);
  static QString USER_GUID                     (QStringList &arg_list);
  static QString USER_NOM_MEDECIN              (QStringList &arg_list);
  static QString USER_NOM_PRENOM_MEDECIN       (QStringList &arg_list);
  static QString USER_POLITESSE_MEDECIN        (QStringList &arg_list);
  static QString USER_PRENOM_MEDECIN           (QStringList &arg_list);
  static QString USER_RPPS                     (QStringList &arg_list);
  static QString USER_CLEF_RPPS                (QStringList &arg_list);
  static QString USER_SEXE_MEDECIN             (QStringList &arg_list);
  static QString USER_SPECIALITE_MEDECIN       (QStringList &arg_list);
  static QString USER_TITRE_PS                 (QStringList &arg_list);
  static QString USER_VILLE_MEDECIN            (QStringList &arg_list);
  static QString USER_LOGIN_MEDECIN            (QStringList &arg_list);
  static QString VALIDER_DOSSIER               (QStringList &arg_list);
  static QString VILLE_MEDECIN                 (QStringList &arg_list);
  static QString VILLE_PATIENT                 (QStringList &arg_list);
  static QString WAIT_CURSOR                   (QStringList &arg_list);
//.......... eviter message Not Used ............
  int justForUsed() {return forUse_G_MacroTab_List();}

  static QString     GetVarNeerer (QString &section, QString &variable, QString &dateSel);
  static QString     GetVarNeerer (QStringList &valuesList, QString &dateSel);
  static QStringList GetVar (QString &section_var, QString &name_var);
  static void        GetPoidsTaille(const char* pt_doc, QString &tableau, QString &poids_name, QString &taille_name, QString &poids_var, QString &taille_var);
  static QString     GetDateOfActiveRubrique();
  static CRubRecord *GetIDCurrentDoc(const QString &doc_type);
  //static QString     DocNameToDocNumType(const QString &doc_name, int part = 8 );
  static QString     Func_Extract(QString &doc_type, QString &tag_deb, QString &tag_end, QString &find_to, QString &replace_by, const QString justNum="");
  static QString     GetDDR                  ();
  static QString     Terme_Obst              (QDate DDR ,   QDate dt);
  static QString     Terme_Obst              (QString DDR , QDate dt);
  static int         FindToken(const QString &tokenToFind, const QString &document, int *fin  = 0);
  static int         FindToken(int pos_deb, int pos_max, const QString &tokenToFind, const QString &document, int *deb, int *fin = 0);
  static QString     toNum(const QString &text, const QString &cutFirstStr="");
  static QTime       hhmmssToQTime(const QString & heure );
  static QString    Func_Extract_ATCD_D(    const QString &str_data,
                                          const QString &typePatern,
                                          const QString &linePatern,
                                          const QString &datePrefix,
                                          const QString &aldPrefix);
  static QString    Func_Extract_ATCD(    const QString &typePatern,
                                          const QString &linePatern,
                                          const QString &datePrefix,
                                          const QString &aldPrefix);
  static QPoint      adjustScreenDialogPosition(QPoint pos, QWidget* pQWidget);
 protected:
  static QString    argUnProtect(QString arg);
  static bool       isReservedKeyWord(const QString &word);
  static int        comparatorMacro (const void *a, const void *b);
  static int        exeFunction(QString func, const QString &args, QString &resolvToken);
  static long       extractArgList(QStringList &arg_list, const QString &arg_str);
  static long       findLastDelimiter(QString &str, char delimiter);
  static QString    Func_Extract_TTT(QString title, QString linePatern, QString renouvInterc, QString tableau, QString poids_name, QString taille_name);
  static QString    Func_Extract_TTT_D(const QString &str_data, QString title, QString linePatern, QString renouvInterc, QString tableau, QString poids_name, QString taille_name);
  static QString    constructExportExportTableDefinition(const QString &data, QStringList &list_SECTION);
  static QString    Func_Export_Tables(      QString date_deb         = "1900-01-01T00:00:00",    // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                             QString date_end         = "3000-01-01T00:00:00",    // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                             QString exportDefinition = "",                       // liste des tables a exportet style : VAR Constantes de Base[Pouls =50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
                                             QString tableSep         = "<br>",                   // motif de separation  entre les tables  <br> par defaut
                                             QString cell_W           = "25%" ,                   // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                             QString cell_H           = "30",                     // hauteur de la cellule
                                             QString border_W         = "1",                      // largeur de la bordure
                                             QString width            = "400",                    // largeur du tableau
                                             QString dateTimeFormat   = "dd-MM-yyyy hh:mm",       // format d'affichage de la date
                                             QString dateTimeWidth    = "100");                   // largeur de la cellule de la date
  static QString    Func_Export_Tables_D(    const QString &str_data,
                                             QString date_deb         = "1900-01-01T00:00:00",    // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                             QString date_end         = "3000-01-01T00:00:00",    // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                             QString exportDefinition = "",                       // liste des tables a exportet style : VAR Constantes de Base[Pouls =50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
                                             QString tableSep         = "<br>",                   // motif de separation  entre les tables  <br> par defaut
                                             QString cell_W           = "25%" ,                   // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                             QString cell_H           = "30",                     // hauteur de la cellule
                                             QString border_W         = "1",                      // largeur de la bordure
                                             QString width            = "400",                    // largeur du tableau
                                             QString dateTimeFormat   = "dd-MM-yyyy hh:mm",       // format d'affichage de la date
                                             QString dateTimeWidth    = "100");                   // largeur de la cellule de la date
  static QString    Func_Extract_Table(    QString section_VAR,
                                           QString list_VAR,
                                           QString date_deb,
                                           QString date_end,
                                           QString cell_W ,
                                           QString cell_H,
                                           QString border_W,
                                           QString width,
                                           QString dateTimeFormat,
                                           QString dateTimeWidth);
static QString    Func_Extract_Table_D(    const QString &str_data,
                                           QString section_VAR,
                                           QString list_VAR,
                                           QString date_deb,
                                           QString date_end,
                                           QString cell_W ,
                                           QString cell_H,
                                           QString border_W,
                                           QString width,
                                           QString dateTimeFormat,
                                           QString dateTimeWidth
                                          );
  static QString     Func_Extract_VAR(QString &section_VAR, QString &name_VAR, QString &date_deb, QString &date_end, QString &date_intervalle);

};
  //...................................... tableaux de fonctions .............................................................
  static KEY_MACRO     G_MacroTab_A[ ] =  {{"ABS_PATH",                       C_TokenInterpret::ABS_PATH},
                                           {"ADRESSE_COMPLETE_MEDECIN",       C_TokenInterpret::ADRESSE_COMPLETE_MEDECIN},
                                           {"ADRESSE_COMPLETE_PATIENT",       C_TokenInterpret::ADRESSE_COMPLETE_PATIENT},
                                           {"ADRESSE_MEDECIN",                C_TokenInterpret::ADRESSE_MEDECIN},
                                           {"ADRESSE_PATIENT",                C_TokenInterpret::ADRESSE_PATIENT},
                                           {"ADRESSE_PATIENT_UNE_LIGNE",      C_TokenInterpret::ADRESSE_PATIENT_UNE_LIGNE},
                                           {"AGE_PATIENT",                    C_TokenInterpret::AGE_PATIENT},
                                           {"AGE_PATIENT_ABREGE",             C_TokenInterpret::AGE_PATIENT_ABREGE},
                                           {"AGE_PATIENT_ANNEE_ABREGE",       C_TokenInterpret::AGE_PATIENT_ANNEE_ABREGE},
                                           {"AP",                             C_TokenInterpret::ABS_PATH},
                                           {"APPEND_DOC_IN_CLASSEUR",         C_TokenInterpret::APPEND_DOC_IN_CLASSEUR}
                                          };
  static KEY_MACRO     G_MacroTab_B[ ] =  {{"BASE_THERAPEUTIQUE",             C_TokenInterpret::BASE_THERAPEUTIQUE},
                                           {"BMI",                            C_TokenInterpret::BMI_PATIENT},
                                           {"BMI_PATIENT",                    C_TokenInterpret::BMI_PATIENT}
                                          };
  static KEY_MACRO     G_MacroTab_C[ ] =  {{"CADRE",                          C_TokenInterpret::CADRE},
                                           {"CALC",                           C_TokenInterpret::CALC},
                                           {"CE",                             C_TokenInterpret::CURRENT_EDITOR},
                                           {"CLEAR_SPACES",                   C_TokenInterpret::CLEAR_SPACES},
                                           {"CODE_POSTAL_PATIENT",            C_TokenInterpret::CODE_POSTAL_PATIENT},
                                           {"COMMIT",                         C_TokenInterpret::VALIDER_DOSSIER},
                                           {"CONVENTION_MEDECIN",             C_TokenInterpret::CONVENTION_MEDECIN},
                                           {"COPY_FILE",                      C_TokenInterpret::COPY_FILE},
                                           {"CP_MEDECIN",                     C_TokenInterpret::CP_MEDECIN},
                                           {"CURRENT_EDITOR",                 C_TokenInterpret::CURRENT_EDITOR},
                                           {"CS",                             C_TokenInterpret::CLEAR_SPACES}
                                          };
  static KEY_MACRO     G_MacroTab_D[ ] =  {{"DATE",                           C_TokenInterpret::DATECOURANTE},
                                           {"DATECOURANTE",                   C_TokenInterpret::DATECOURANTE},
                                           {"DATE_CALC",                      C_TokenInterpret::DATE_CALC},
                                           {"DATE_DERNIERES_REGLES",          C_TokenInterpret::DDR},
                                           {"DATE_DOCUMENT",                  C_TokenInterpret::DATE_DOCUMENT},
                                           {"DATE_FROMNUM",                   C_TokenInterpret::DATE_FROMNUM},
                                           {"DATE_NAISSANCE_ASSURE",          C_TokenInterpret::DATE_NAISSANCE_ASSURE},
                                           {"DATE_NAISSANCE_PATIENT",         C_TokenInterpret::DATE_NAISSANCE_PATIENT},
                                           {"DATE_TIRET",                     C_TokenInterpret::DATECOURANTE},
                                           {"DATE_TONUM",                     C_TokenInterpret::DATE_TONUM},
                                           {"DATE_VISITE",                    C_TokenInterpret::DATE_VISITE},
                                           {"DAYSTO",                         C_TokenInterpret::DAYSTO},
                                           {"DD-MM-YYYY",                     C_TokenInterpret::DATECOURANTE},
                                           {"DDR",                            C_TokenInterpret::DDR},
                                           {"DELETE_FILE",                    C_TokenInterpret::DELETE_FILE},
                                           {"DOSS_GUID",                      C_TokenInterpret::SQL_GET_DOSS_GUID},
                                           {"DO_LISTE",                       C_TokenInterpret::DO_LISTE},
                                           {"DT",                             C_TokenInterpret::DATECOURANTE},
                                           {"DTC",                            C_TokenInterpret::DATE_CALC},
                                           {"DTFN",                           C_TokenInterpret::DATE_FROMNUM},
                                           {"DTN",                            C_TokenInterpret::DATE_TONUM}
                                          };
  static KEY_MACRO     G_MacroTab_E[ ] =  {{"EMAIL_MEDECIN",                  C_TokenInterpret::EMAIL_MEDECIN},
                                           {"EMAIL_PATIENT",                  C_TokenInterpret::EMAIL_PATIENT},
                                           {"EXE_PROCESS",                    C_TokenInterpret::EXE_PROCESS},
                                           {"EXIT",                           C_TokenInterpret::EXIT},
                                           {"EXPORT",                         C_TokenInterpret::EXPORT},
                                           {"EXPORT_TABLE",                   C_TokenInterpret::EXPORT_TABLE},
                                           {"EXTRACT",                        C_TokenInterpret::EXTRACT},
                                           {"EXTRACT_ATCD",                   C_TokenInterpret::EXTRACT_ATCD},
                                           {"EXTRACT_TABLE",                  C_TokenInterpret::EXTRACT_TABLE},
                                           {"EXTRACT_TRAITEMENT",             C_TokenInterpret::EXTRACT_TRAITEMENT},
                                           {"EXTRACT_VAR",                    C_TokenInterpret::EXTRACT_VAR}
                                          };

  static KEY_MACRO     G_MacroTab_F[ ] =  {{"FE",                             C_TokenInterpret::FILE_EXISTS},
                                           {"FILE_EXISTS",                    C_TokenInterpret::FILE_EXISTS},
                                           {"FILL",                           C_TokenInterpret::FILL},
                                           {"FORM_DATE",                      C_TokenInterpret::FORM_DATE},
                                           {"FORM_DATE_HEURE",                C_TokenInterpret::FORM_DATE_HEURE},
                                           {"FORM_HEURE",                     C_TokenInterpret::FORM_HEURE},
                                           {"FORM_MESSAGE",                   C_TokenInterpret::MESSAGE_ALERT},
                                           {"FORM_POPUP_MESSAGE",             C_TokenInterpret::MESSAGE_POPUP}
                                          };
  static KEY_MACRO     G_MacroTab_G[ ] =  {{"GET_DOC_DATE_FIN",               C_TokenInterpret::GET_DOC_DATE_FIN},
                                           {"GET_DOC_PROP_0",                 C_TokenInterpret::GET_DOC_PROP_0},
                                           {"GET_DOC_PROP_1",                 C_TokenInterpret::GET_DOC_PROP_1},
                                           {"GET_DOC_PROP_2",                 C_TokenInterpret::GET_DOC_PROP_2},
                                           {"GET_DOC_PROP_3",                 C_TokenInterpret::GET_DOC_PROP_3},
                                           {"GET_DOC_PROP_4",                 C_TokenInterpret::GET_DOC_PROP_4},
                                           {"GET_DOSS_GUID",                  C_TokenInterpret::SQL_GET_DOSS_GUID},
                                           {"GET_LAST_IMAGE_NAME",            C_TokenInterpret::GET_LAST_IMAGE_NAME},
                                           {"GET_LAST_IMAGE_SRC_NAME",        C_TokenInterpret::GET_LAST_IMAGE_SRC_NAME},
                                           {"GET_SAVE_FILE_NAME",             C_TokenInterpret::GET_SAVE_FILE_NAME},
                                           {"GET_SELECTION",                  C_TokenInterpret::GET_SELECTION},
                                           {"GRAPH",                          C_TokenInterpret::GRAPH},
                                           {"GUID",                           C_TokenInterpret::SQL_GET_DOSS_GUID}
                                          };
  static KEY_MACRO     G_MacroTab_H[ ] =  {{"HEURECOURANTE",                  C_TokenInterpret::HHHMM},
                                           {"HHHMM",                          C_TokenInterpret::HHHMM}
                                          };
  static KEY_MACRO     G_MacroTab_I[ ] =  {{"IDENTIFICATION_MEDECIN",         C_TokenInterpret::IDENTIFICATION_MEDECIN},
                                           {"IMC",                            C_TokenInterpret::BMI_PATIENT},
                                           {"IMPORT",                         C_TokenInterpret::IMPORT},
                                           {"INCLUDE",                        C_TokenInterpret::INCLUDE},
                                           {"INDEX_OF",                       C_TokenInterpret::INDEX_OF},
                                           {"INPORT",                         C_TokenInterpret::IMPORT},
                                           {"INSERT_CISP",                    C_TokenInterpret::INSERT_CISP},
                                           {"INSERT_IMAGE",                   C_TokenInterpret::INSERT_IMAGE},
                                           {"INTERVENANT",                    C_TokenInterpret::INTERVENANT},
                                           {"INTITULE",                       C_TokenInterpret::INTITULE},
                                           {"IO",                             C_TokenInterpret::INDEX_OF},
                                           {"ISDV",                           C_TokenInterpret::IS_DATE_VALID},
                                           {"IS_DATE_VALID",                  C_TokenInterpret::IS_DATE_VALID}
                                          };
  static KEY_MACRO     G_MacroTab_J[ ] =  {{"JOIN",                           C_TokenInterpret::JOIN}};
  static KEY_MACRO     G_MacroTab_K[ ] =  {{"KEY_VALUE",                      C_TokenInterpret::KEY_VALUE},
                                           {"KV",                             C_TokenInterpret::KEY_VALUE}
                                          };
  static KEY_MACRO     G_MacroTab_L[ ] =  {{"LEFT",                           C_TokenInterpret::LEFT},
                                           {"LEN",                            C_TokenInterpret::LENGTH},
                                           {"LENGTH",                         C_TokenInterpret::LENGTH},
                                           {"LINE_EDIT",                      C_TokenInterpret::LINE_EDIT},
                                           {"LIST_DIR",                       C_TokenInterpret::LIST_DIR},
                                           {"LOWER",                          C_TokenInterpret::LOWER}
                                          };
  static KEY_MACRO     G_MacroTab_M[ ] =  {{"MC",                             C_TokenInterpret::MOVE_CURSOR},
                                           {"MESSAGE_ALERT",                  C_TokenInterpret::MESSAGE_ALERT},
                                           {"MESSAGE_POPUP",                  C_TokenInterpret::MESSAGE_POPUP},
                                           {"MID",                            C_TokenInterpret::MID},
                                           {"MOVE_CURSOR",                    C_TokenInterpret::MOVE_CURSOR},
                                           {"M_NOP",                          C_TokenInterpret::NOP}
                                           };
  static KEY_MACRO     G_MacroTab_N[ ] =  {{"NOMBRE_D'ENFANT_PATIENT",        C_TokenInterpret::NOMBRE_D_ENFANT_PATIENT},
                                           {"NOM_MEDECIN",                    C_TokenInterpret::NOM_MEDECIN},
                                           {"NOM_NAISSANCE_PATIENT",          C_TokenInterpret::NOM_NAISSANCE_PATIENT},
                                           {"NOM_PATIENT",                    C_TokenInterpret::NOM_PATIENT},
                                           {"NOM_PRENOM_ASSURE",              C_TokenInterpret::NOM_PRENOM_ASSURE},
                                           {"NOM_PRENOM_INTERVENANT",         C_TokenInterpret::NOM_PRENOM_INTERVENANT},
                                           {"NOM_PRENOM_MEDECIN",             C_TokenInterpret::NOM_PRENOM_MEDECIN},
                                           {"NOM_PRENOM_PATIENT",             C_TokenInterpret::NOM_PRENOM_PATIENT},
                                           {"NOUVEAU_DOCUMENT",               C_TokenInterpret::NOUVEAU_DOCUMENT},
                                           {"NUM._SECU_PATIENT",              C_TokenInterpret::NUM_SECU_PATIENT},
                                           {"NUM._TEL1_PATIENT",              C_TokenInterpret::NUM_TEL1_PATIENT},
                                           {"NUM._TEL2_PATIENT",              C_TokenInterpret::NUM_TEL2_PATIENT},
                                           {"NUM._TEL3_PATIENT",              C_TokenInterpret::NUM_TEL3_PATIENT},
                                           {"NUM_CLEF_PATIENT",               C_TokenInterpret::NUM_CLEF_PATIENT},
                                           {"NUM_SECU_PATIENT",               C_TokenInterpret::NUM_SECU_PATIENT},
                                           {"NUM_TEL1_PATIENT",               C_TokenInterpret::NUM_TEL1_PATIENT},
                                           {"NUM_TEL2_PATIENT",               C_TokenInterpret::NUM_TEL2_PATIENT},
                                           {"NUM_TEL3_PATIENT",               C_TokenInterpret::NUM_TEL3_PATIENT}
                                          };
  static KEY_MACRO     G_MacroTab_O[ ] =  {{"OPEN_FILE_NAME",                 C_TokenInterpret::OPEN_FILE_NAME},
                                           {"OS",                             C_TokenInterpret::OS}
                                          };
  static KEY_MACRO     G_MacroTab_P[ ] =  {{"POIDS_ENFANT",                   C_TokenInterpret::POIDS_PATIENT},
                                           {"POIDS_PATIENT",                  C_TokenInterpret::POIDS_PATIENT},
                                           {"POLITESSE_MEDECIN",              C_TokenInterpret::POLITESSE_MEDECIN},
                                           {"PRENOM_MEDECIN",                 C_TokenInterpret::PRENOM_MEDECIN},
                                           {"PRENOM_PATIENT",                 C_TokenInterpret::PRENOM_PATIENT},
                                           {"PROFESSION_PATIENT",             C_TokenInterpret::PROFESSION_PATIENT},
                                           {"PUT_VAR",                        C_TokenInterpret::PUT_VAR}
                                          };
  static KEY_MACRO     G_MacroTab_Q[ ] =  {{"Q_NOP",                          C_TokenInterpret::NOP}};
  static KEY_MACRO     G_MacroTab_R[ ] =  {{"RB",                             C_TokenInterpret::REPLACE_BY},
                                           {"READ_KEY",                       C_TokenInterpret::KEY_VALUE},
                                           {"REINIT_CLASSEUR",                C_TokenInterpret::REINIT_CLASSEUR},
                                           {"REM",                            C_TokenInterpret::NOP},
                                           {"REMARQUE",                       C_TokenInterpret::NOP},
                                           {"REMOVE",                         C_TokenInterpret::SIMPLIFY},
                                           {"REPLACE",                        C_TokenInterpret::REPLACE_BY},
                                           {"REPLACE_IMAGE",                  C_TokenInterpret::REPLACE_IMAGE},
                                           {"REPLACE_IN_FILE",                C_TokenInterpret::REPLACE_IN_FILE},
                                           {"RES",                            C_TokenInterpret::VAR_CALC},
                                           {"RESOLVE",                        C_TokenInterpret::VAR_CALC},
                                           {"RESULT",                         C_TokenInterpret::VAR_CALC},
                                           {"RF",                             C_TokenInterpret::REPLACE_IN_FILE},
                                           {"RIGHT",                          C_TokenInterpret::RIGHT},
                                           {"RM",                             C_TokenInterpret::SIMPLIFY},
                                           {"RS",                             C_TokenInterpret::VAR_CALC},
                                           {"RUBRIQUE_SHOW",                  C_TokenInterpret::RUBRIQUE_SHOW}
                                           };
  static KEY_MACRO     G_MacroTab_S[ ] =  {{"SAVE_CURRENT_DOC",               C_TokenInterpret::SAVE_CURRENT_DOC},
                                           {"SAVE_LAST_IMAGE_NAME",           C_TokenInterpret::SAVE_LAST_IMAGE_NAME},
                                           {"SET_DATE_DOC",                   C_TokenInterpret::SET_DOC_DATE_DEB},
                                           {"SET_DOC_DATE_FIN",               C_TokenInterpret::SET_DOC_DATE_FIN},
                                           {"SET_DOC_DATE_DEB",               C_TokenInterpret::SET_DOC_DATE_DEB},
                                           {"SET_DOC_PROP_0",                 C_TokenInterpret::SET_DOC_PROP_0},
                                           {"SET_DOC_PROP_1",                 C_TokenInterpret::SET_DOC_PROP_1},
                                           {"SET_DOC_PROP_2",                 C_TokenInterpret::SET_DOC_PROP_2},
                                           {"SET_DOC_PROP_3",                 C_TokenInterpret::SET_DOC_PROP_3},
                                           {"SET_DOC_PROP_4",                 C_TokenInterpret::SET_DOC_PROP_4},
                                           {"SET_VAR",                        C_TokenInterpret::SET_VAR},
                                           {"SEXE_MEDECIN",                   C_TokenInterpret::SEXE_MEDECIN},
                                           {"SEXE_PATIENT",                   C_TokenInterpret::SEXE_PATIENT},
                                           {"SI",                             C_TokenInterpret::TEST},
                                           {"SIMPLIFY",                       C_TokenInterpret::SIMPLIFY},
                                           {"SOUS_TYPE",                      C_TokenInterpret::SOUS_TYPE},
                                           {"SPECIALITE_MEDECIN",             C_TokenInterpret::SPECIALITE_MEDECIN},
                                           {"SQL_EXEC",                       C_TokenInterpret::SQL_EXEC},
                                           {"SQL_GET_DOSS_GUID",              C_TokenInterpret::SQL_GET_DOSS_GUID},
                                           {"SQL_GET_DOSS_PK",                C_TokenInterpret::SQL_GET_DOSS_PK},
                                           {"SQL_GET_RUB_HEAD_PK",            C_TokenInterpret::SQL_GET_RUB_HEAD_PK},
                                           {"SQL_SELECT",                     C_TokenInterpret::SQL_SELECT},
                                           {"SQL_WRITE",                      C_TokenInterpret::SQL_WRITE},
                                           {"SURFACE_PATIENT",                C_TokenInterpret::SURFACE_PATIENT}
                                          };
  static KEY_MACRO     G_MacroTab_T[ ] =  {{"TAILLE_PATIENT",                 C_TokenInterpret::TAILLE_PATIENT},
                                           {"TEL_1_MEDECIN",                  C_TokenInterpret::TEL_1_MEDECIN},
                                           {"TEL_2_MEDECIN",                  C_TokenInterpret::TEL_2_MEDECIN},
                                           {"TEL_3_MEDECIN",                  C_TokenInterpret::TEL_3_MEDECIN},
                                           {"TERME_GROSSESSE",                C_TokenInterpret::TERME_GROSSESSE},
                                           {"TEST",                           C_TokenInterpret::TEST},
                                           {"TITRE_PATIENT",                  C_TokenInterpret::TITRE_PATIENT},
                                           {"TITRE_PS",                       C_TokenInterpret::TITRE_PS},
                                           {"TN",                             C_TokenInterpret::TO_NUM},
                                           {"TO_SCRIPT",                      C_TokenInterpret::TO_SCRIPT},
                                           {"TO_NUM",                         C_TokenInterpret::TO_NUM}
                                          };
  static KEY_MACRO     G_MacroTab_U[ ] =  {{"UPPER",                          C_TokenInterpret::UPPER},
                                           {"USER_ADRESSE_COMPLETE_MEDECIN",  C_TokenInterpret::USER_ADRESSE_COMPLETE_MEDECIN},
                                           {"USER_ADRESSE_MEDECIN",           C_TokenInterpret::USER_ADRESSE_MEDECIN},
                                           {"USER_CLEF_RPPS",                 C_TokenInterpret::USER_CLEF_RPPS},
                                           {"USER_CONVENTION_MEDECIN",        C_TokenInterpret::USER_CONVENTION_MEDECIN},
                                           {"USER_CP_MEDECIN",                C_TokenInterpret::USER_CP_MEDECIN},
                                           {"USER_EMAIL_MEDECIN",             C_TokenInterpret::USER_EMAIL_MEDECIN},
                                           {"USER_GUID",                      C_TokenInterpret::USER_GUID},
                                           {"USER_IDENTIFICATION_MEDECIN",    C_TokenInterpret::USER_IDENTIFICATION_MEDECIN},
                                           {"USER_LOGIN",                     C_TokenInterpret::USER_LOGIN_MEDECIN},
                                           {"USER_LOGIN_MEDECIN",             C_TokenInterpret::USER_LOGIN_MEDECIN},
                                           {"USER_NOM_MEDECIN",               C_TokenInterpret::USER_NOM_MEDECIN},
                                           {"USER_NOM_PRENOM_MEDECIN",        C_TokenInterpret::USER_NOM_PRENOM_MEDECIN},
                                           {"USER_POLITESSE_MEDECIN",         C_TokenInterpret::USER_POLITESSE_MEDECIN},
                                           {"USER_PRENOM_MEDECIN",            C_TokenInterpret::USER_PRENOM_MEDECIN},
                                           {"USER_RPPS",                      C_TokenInterpret::USER_RPPS},
                                           {"USER_SEXE_MEDECIN",              C_TokenInterpret::USER_SEXE_MEDECIN},
                                           {"USER_SPECIALITE_MEDECIN",        C_TokenInterpret::USER_SPECIALITE_MEDECIN},
                                           {"USER_TEL_1_MEDECIN",             C_TokenInterpret::USER_TEL_1_MEDECIN},
                                           {"USER_TEL_2_MEDECIN",             C_TokenInterpret::USER_TEL_2_MEDECIN},
                                           {"USER_TEL_3_MEDECIN",             C_TokenInterpret::USER_TEL_3_MEDECIN},
                                           {"USER_TITRE_PS",                  C_TokenInterpret::USER_TITRE_PS},
                                           {"USER_VILLE_MEDECIN",             C_TokenInterpret::USER_VILLE_MEDECIN}
                                          };
  static KEY_MACRO     G_MacroTab_V[ ] =  {{"VALIDER_DOSSIER",                C_TokenInterpret::VALIDER_DOSSIER},
                                           {"VAR",                            C_TokenInterpret::PUT_VAR},
                                           {"VAR_CALC",                       C_TokenInterpret::VAR_CALC},
                                           {"VAR_LIST_CLEAR",                 C_TokenInterpret::VAR_LIST_CLEAR},
                                           {"VAR_SET",                        C_TokenInterpret::SET_VAR},
                                           {"VAR_SPLIT",                      C_TokenInterpret::VAR_SPLIT},
                                           {"VC",                             C_TokenInterpret::VAR_CALC},
                                           {"VILLE_MEDECIN",                  C_TokenInterpret::VILLE_MEDECIN},
                                           {"VILLE_PATIENT",                  C_TokenInterpret::VILLE_PATIENT}
                                          };
  static KEY_MACRO     G_MacroTab_W[ ]  = {{"WAIT_CURSOR",                    C_TokenInterpret::WAIT_CURSOR},
                                           {"WC",                             C_TokenInterpret::WAIT_CURSOR}
                                          };
  static KEY_MACRO     G_MacroTab_X[ ]  = {{"X_NOP",                          C_TokenInterpret::NOP}};
  static KEY_MACRO     G_MacroTab_Y[ ]  = {{"Y_NOP",                          C_TokenInterpret::NOP}};
  static KEY_MACRO     G_MacroTab_Z[ ]  = {{"ZAB",                            C_TokenInterpret::VALIDER_DOSSIER}
                                          };
  //................... le tableau des mots reserves utilise la meme mecanique ......................
  static KEY_MACRO     G_MacroTab_WR[ ] = {{"$ALLSTRING",                     C_TokenInterpret::NOP},
                                           {"$DATEDEB",                       C_TokenInterpret::NOP},
                                           {"$DATEDEBFICHE",                  C_TokenInterpret::NOP},
                                           {"$DATEFIN",                       C_TokenInterpret::NOP},
                                           {"$DATEFINFICHE",                  C_TokenInterpret::NOP},
                                           {"$DUREE",                         C_TokenInterpret::NOP},
                                           {"$ENDTOABSPATH",                  C_TokenInterpret::NOP},
                                           {"$END_WIDTH",                     C_TokenInterpret::NOP},
                                           {"$FILE",                          C_TokenInterpret::NOP},
                                           {"$FROMSTART",                     C_TokenInterpret::NOP},
                                           {"$FUSION_ADD",                    C_TokenInterpret::NOP},
                                           {"$FUSION_CREATE",                 C_TokenInterpret::NOP},
                                           {"$GLOSSAIRE",                     C_TokenInterpret::NOP},
                                           {"$GUIDPAT",                       C_TokenInterpret::NOP},
                                           {"$HEUREDEB",                      C_TokenInterpret::NOP},
                                           {"$HEUREFIN",                      C_TokenInterpret::NOP},
                                           {"$HOME",                          C_TokenInterpret::NOP},
                                           {"$ISABSPATH",                     C_TokenInterpret::NOP},
                                           {"$ISBINPATH",                     C_TokenInterpret::NOP},
                                           {"$KEEPHTML",                      C_TokenInterpret::NOP},
                                           {"$KEEP_ORIGINAL_H",               C_TokenInterpret::NOP},
                                           {"$KEEP_ORIGINAL_W",               C_TokenInterpret::NOP},
                                           {"$KEEP_SRC_NAME",                 C_TokenInterpret::NOP},
                                           {"$NO KEY",                        C_TokenInterpret::NOP},
                                           {"$NOADDEXE",                      C_TokenInterpret::NOP},
                                           {"$NOMPATIENT",                    C_TokenInterpret::NOP},
                                           {"$NOMRUB",                        C_TokenInterpret::NOP},
                                           {"$NUMFICHERUB",                   C_TokenInterpret::NOP},
                                           {"$NUMFICHETOT",                   C_TokenInterpret::NOP},
                                           {"$PASTE",                         C_TokenInterpret::NOP},
                                           {"$PKFICHE",                       C_TokenInterpret::NOP},
                                           {"$PKPAT",                         C_TokenInterpret::NOP},
                                           {"$PRENOMPATIENT",                 C_TokenInterpret::NOP},
                                           {"$PREVISUALISATION",              C_TokenInterpret::NOP},
                                           {"$PREVISUALISATION_END",          C_TokenInterpret::NOP},
                                           {"$PRINT_HEADER_CHANGED",          C_TokenInterpret::NOP},
                                           {"$PRINT_SCRIPT_BEFORE",           C_TokenInterpret::NOP},
                                           {"$PRINT_SCRIPT_END",              C_TokenInterpret::NOP},
                                           {"$PRINT_STOP",                    C_TokenInterpret::NOP},
                                           {"$REMOVE_SRC_FILE",               C_TokenInterpret::NOP},
                                           {"$SELECTION",                     C_TokenInterpret::NOP},
                                           {"$SET_VAR",                       C_TokenInterpret::NOP},
                                           {"$SIGNUSER",                      C_TokenInterpret::NOP},
                                           {"$STOPIFNOTNUM",                  C_TokenInterpret::NOP},
                                           {"$TABLENAME",                     C_TokenInterpret::NOP},
                                           {"$THEME",                         C_TokenInterpret::NOP},
                                           {"$TITRERUB",                      C_TokenInterpret::NOP},
                                           {"$TOABSPATH",                     C_TokenInterpret::NOP},
                                           {"$TOEND",                         C_TokenInterpret::NOP},
                                           {"$TOINT",                         C_TokenInterpret::NOP},
                                           {"$TOSTDIN",                       C_TokenInterpret::NOP},
                                           {"$USER",                          C_TokenInterpret::NOP},
                                           {"$USER_PARAM",                    C_TokenInterpret::NOP},
                                           {"$VIGIE",                         C_TokenInterpret::NOP},
                                           {"$WIDTH",                         C_TokenInterpret::NOP},
                                          };

  //...................................... tableaux des tableaux de fonctions .............................................................
  //                                       un tableau par lettre de l'alphabet
  static KEY_MACRO_TAB_DEF G_MacroTab_List[]  ={ {G_MacroTab_A,  sizeof(G_MacroTab_A)  / sizeof(KEY_MACRO)},   // 0
                                                 {G_MacroTab_B,  sizeof(G_MacroTab_B)  / sizeof(KEY_MACRO)},   // 1
                                                 {G_MacroTab_C,  sizeof(G_MacroTab_C)  / sizeof(KEY_MACRO)},   // 2
                                                 {G_MacroTab_D,  sizeof(G_MacroTab_D)  / sizeof(KEY_MACRO)},   // 3
                                                 {G_MacroTab_E,  sizeof(G_MacroTab_E)  / sizeof(KEY_MACRO)},   // 4
                                                 {G_MacroTab_F,  sizeof(G_MacroTab_F)  / sizeof(KEY_MACRO)},   // 5
                                                 {G_MacroTab_G,  sizeof(G_MacroTab_G)  / sizeof(KEY_MACRO)},   // 6
                                                 {G_MacroTab_H,  sizeof(G_MacroTab_H)  / sizeof(KEY_MACRO)},   // 7
                                                 {G_MacroTab_I,  sizeof(G_MacroTab_I)  / sizeof(KEY_MACRO)},   // 8 
                                                 {G_MacroTab_J,  sizeof(G_MacroTab_J)  / sizeof(KEY_MACRO)},   // 9
                                                 {G_MacroTab_K,  sizeof(G_MacroTab_K)  / sizeof(KEY_MACRO)},   // 10
                                                 {G_MacroTab_L,  sizeof(G_MacroTab_L)  / sizeof(KEY_MACRO)},   // 11
                                                 {G_MacroTab_M,  sizeof(G_MacroTab_M)  / sizeof(KEY_MACRO)},   // 12
                                                 {G_MacroTab_N,  sizeof(G_MacroTab_N)  / sizeof(KEY_MACRO)},   // 13
                                                 {G_MacroTab_O,  sizeof(G_MacroTab_O)  / sizeof(KEY_MACRO)},   // 14
                                                 {G_MacroTab_P,  sizeof(G_MacroTab_P)  / sizeof(KEY_MACRO)},   // 15
                                                 {G_MacroTab_Q,  sizeof(G_MacroTab_Q)  / sizeof(KEY_MACRO)},   // 16
                                                 {G_MacroTab_R,  sizeof(G_MacroTab_R)  / sizeof(KEY_MACRO)},   // 17
                                                 {G_MacroTab_S,  sizeof(G_MacroTab_S)  / sizeof(KEY_MACRO)},   // 18
                                                 {G_MacroTab_T,  sizeof(G_MacroTab_T)  / sizeof(KEY_MACRO)},   // 19
                                                 {G_MacroTab_U,  sizeof(G_MacroTab_U)  / sizeof(KEY_MACRO)},   // 20
                                                 {G_MacroTab_V,  sizeof(G_MacroTab_V)  / sizeof(KEY_MACRO)},   // 21
                                                 {G_MacroTab_W,  sizeof(G_MacroTab_W)  / sizeof(KEY_MACRO)},   // 22
                                                 {G_MacroTab_X,  sizeof(G_MacroTab_X)  / sizeof(KEY_MACRO)},   // 23
                                                 {G_MacroTab_Y,  sizeof(G_MacroTab_Y)  / sizeof(KEY_MACRO)},   // 24
                                                 {G_MacroTab_Z,  sizeof(G_MacroTab_Z)  / sizeof(KEY_MACRO)},   // 25
                                                 {G_MacroTab_WR, sizeof(G_MacroTab_WR) / sizeof(KEY_MACRO)}    // 26
                                               };
//.......... forUse_G_MacroTab_List ..........................
int forUse_G_MacroTab_List() {return G_MacroTab_List->m_size;}

#endif
