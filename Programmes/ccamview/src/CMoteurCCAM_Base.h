/***************************************************************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 * *********************************************************************** *
 *  Portions of this code are derivated from PIP                           *
 *   and Copyright (C) René BEDDOK                                         *
 *   http://membres.lycos.fr/pradeau/PMSI/outils/CCAM/CCAM_acceuil.htm     *
 * ********************** Many Thanks to René BEDDOK ********************* *
 *     auteur de PIP logiciel de codage CCAM sous licence GPL              *
 *     auquel CCAM_View fait de larges emprunts dans ce fichier source     *
 *     notamment au niveau de la structure de la base de données           *
 *     et des requêtes SQL permettant de l'exploiter                       *
 * *********************************************************************** *
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
#ifndef  CMOTEUR_CCAM_BASE_H
#define  CMOTEUR_CCAM_BASE_H

//=============================================== INCLUDES ======================================================================
#include <qsqldatabase.h>
#include <qstring.h>
#include <q3listview.h>
#include <qlabel.h>
#include <q3textbrowser.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qdatetime.h>
#include "Global.h"
//Added by qt3to4:
#include <QSqlQuery>


//=============================================== DEFINE ========================================================================
#define BASE_CCAM "BaseCCAM"
#define BASE_USER "BaseUSER"
#define LIST_TO_CLEAR 1
#define LIST_NO_CLEAR 0

#define SKIP_BLANK_CAR(a)     while( *(a) && (*(a)==' ' || *(a)=='\t')) (a)++
#define NEXT_LINE(a)          while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++

#define  ERR_PASS_MAX       1
#define  ERR_PASS_ZERO      2
#define  ERR_PASS_OK        3
#define  UINT16       quint16
//=============================================== TYPEDEF ========================================================================

//........................... structure d'echange pour la fonction: .............................................................
//                            void GetInfoMedecinsFromNid(const QString &nid, INFO_MED &info_med);
typedef struct _INFO_MED
      {QString m_Code;
       QString m_Password;
       QString m_CodeMedinTux;
       QString m_PasswordMedinTux;
       QString m_Nom;
       QString m_Prenom;
       QString m_Nid;
       QString m_Spec;
       QString m_Pk;
      }INFO_MED;




//===================================== CMoteurCCAM_Base ========================================================================
class CMoteurCCAM_Base
{
 //............................... METHODES ............................................................................
 public:

  //................................... configuration et connection de la base de données ..............................
  CMoteurCCAM_Base();
  CMoteurCCAM_Base(const char* driver,                 // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                   const char* DataBaseName,           // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                   const char* user     = "ro",        // = "root"        utilisateur
                   const char* password = "",          // = ""            paswsword
                   const char* hostname = "localhost", // = "localhost"   on travaille en local sur notre disque
                   QString*    errMess  = 0,
                   int*        errNum   = 0);

  ~CMoteurCCAM_Base();
  QSqlDatabase  *ConnectUserBase(const char*   driver,         // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                 const char*   DataBaseName,   // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                 const char*   user,           // = "root"
                                 const char*   password,       // = ""
                                 const char*   hostname,
                                 QString*      errMess = 0,
                                 int*          errNum  = 0);
  QSqlDatabase  *BaseConnect(const char*   driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                             const char*   DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                             QSqlDatabase &r_QSqlDatabase, // reference sur la database �  initialiser
                             const char*   user,          // = "root"
                             const char*   pasword,       // = ""
                             const char*   hostname,      // = "localhost"
                             const char*   baseName,
                             QString*      errMess=0,
                             int*          errNum=0);

  int    SetConfBase(const char* confFile, QString *errMess);
  char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err);
  QString        nearDateEffet(const QString &date);
  QString        nearDateEffet(const QString &date, QSqlQuery &query);
  void           initArbo01(Q3ListView*     pQListView );
  void           initArbo02(Q3ListViewItem*     pQListViewItem, QString codeMenu );
  QString        GetCCAM_Version();
  long           GetActesParMotsClef(Q3ListView*    pQListView,
                                    const QString &mot_cle_saisie1,
                                    const QString &mot_cle_saisie2 ,
                                    const QString &codeCCAM,
                                    const QString &activite,
                                    int            listMustBeCleared =1);
  long           GetActesIncompatibles(QStringList&     pQStringList,  const QString &codeCCAM, const QString &date);
  long           GetActesIncompatibles(Q3ListView*     pQListView,const QString &codeCCAM, const QString &date);
  long           GetHierarchieActe(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM, const QString &libelle);
  long           GetActesParArborescence(Q3ListView *pQListView, QStringList& arborescence);
  long           GetNotes(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM);
  long           GetActesIsolesProcedures(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM, const QString &date);
  long           GetModificateurs(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM, const QString &date);
  long           GetActesAssocies(const QString &codeCCAM, const QString &activiteActe, const QString &dateActe, CODE_ASSO_LIST &codeAssoList);
  long           GetPhases(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM, const QString &date);

  long           GetPhasesActe(QStringList &qstringList,const QString &codeCCAM);
  QString        GetPrixListeActe( CCAM_ACTE *ccamActe, int nb_actes);
  QString        GetPrixActe(const CCAM_ACTE &ccamActe, const QString &code_asso="1");
  QString        GetPrixActe(         const QString &codeCCAM,
                                      const QString &dateStr,
                                      const QString &code_asso="1",
                                      const QString &activite="",
                                      const QString &phase="",
                                      const QString &mod_1="",
                                      const QString &mod_2="",
                                      const QString &mod_3="",
                                      const QString &mod_4="");

  QString        GetPrixActe(         const QString &codeCCAM,
                                      const QDate   &dateconst,
                                      const QString &code_asso="1",
                                      const QString &activite="",
                                      const QString &phase="",
                                      const QString &mod_1="",
                                      const QString &mod_2="",
                                      const QString &mod_3="",
                                      const QString &mod_4="",
                                      const QString &spec_domtom="",
                                      const QString &regle="");
  QString        GetCoeffGene(const QString &regle, const QString &date_acte );
  QString        GetPrixBaseActe(const QString &codeCCAM, const QString &activite, const QString &phase,const QString &date_acte );
  double         CalcMajoModificateurs(const double l_tauxDeBase, const QString &mod_1, const QString &mod_2, const QString &mod_3, const QString &mod_4, const QString &date_acte );
  int            GetModifificateurCoef(const QString &mod, QString date_acte, double &coef, double &forfait);
  QString        GetLibelleModificateur(const QString &codeMod);
  long           GetModificateurActe(QStringList &qstringList,const QString &codeCCAM, const QString &date);

  long           GetActiviteActe(QStringList &qstringList,const QString &codeCCAM);
  QString        GetPlaceArbo(const QString &codeCCAM );
  void           GetSexeActe(QString &qstring, const QString &codeCCAM);
  long           GetGestesComplementaires(Q3ListView *pQListView_AA,Q3ListView *pQListView_GC,const QString &codeCCAM, const QString &date);

  void           OutSQL_error(QSqlQuery &cur, const char *messFunc =0, const char *requete =0);
  long           CheckThesaurus(Q3ListView *pQListView);
  int            CheckBasePatient();
  int            CreeTableMedecins();
  int            DropTablePraticiens();
  long           GetMedecins(Q3ListView *pQListView);
  void           GetInfoMedecinsFromCode(const QString &codeMEDECIN, INFO_MED &info_med);
  void           GetInfoMedecinsFromNid(const QString &nid, INFO_MED &info_med);
  bool           MedecinExisteByPrimKey(const QString &pk);
  bool           MedecinExisteByLoginMedinTux(const QString &loginMedinTux);
  bool           MedecinExiste(const QString &codeMEDECIN);
  bool           CreateMedecins( INFO_MED &info_med );
  bool           MAJMedecins(INFO_MED &info_med);
  int            DeleteMedecin( const char *primKey);
  bool           VerificationLogin(const QString &utilisateur,  QString password, QString *realPass=0);
  long           AjoutActesThesaurus(const QString &utilisateur, const QString &codeACTE, QString* pCodeCategorie=0, QString* pCodeUsuel=0);
  bool           SupprimeThesaurusPraticien(const QString &code_utilisateur);
  long           GetThesaurus(Q3ListView *pQListView,const QString &utilisateur,const QString &categorie);
  QString        CodeCCAMToPlaceArbo(const QString& code);
  long           GetInfoActeThesaurus(QComboBox *pQComboBox,QLineEdit *pQLineEdit,const QString &serie);
  bool           MAJActeThesaurus(const QString &serie,const QString &categorie,const QString &lib_usuel);
  long           GetComboCategorie(QComboBox *pQComboBox,const QString &code_utilisateur);
  bool           SupprimeActeThesaurus(const QString &serie);
  long           GetComboPraticiens(QComboBox *pQComboBox);
  QString        GetNidPraticiens(const QString &code_utilisateur);
  int            SetComboPraticiensOnNID(QComboBox* pQComboBox, QString& nid);
  QString        CodeCCAMToLibelleLong(const QString& code);
  QString        CalculeDiffAge(QDate *pDebut, QDate *pFin=NULL, bool bTexte=TRUE, int bAbrege=1);
  bool           MAJAddActe(ENT_ACTE &acte);
  bool           MAJAddCompatibilite(QString &codeacte,QString &activite,QString &dateeffet,ENT_COMPATIBILITE *compatibilite);
  bool           MAJAddActiviteActe(QString &codeacte,QString &activite);
  bool           CheckTableVersion();
  QString        CheckIntegration(VER_BASE &version);
  bool           MAJAddVersionBase(VER_BASE &version);
  bool           MAJAddProcedure(QString &codeacte,QString &dateeffet,QString *procedure);
  bool           MAJAddIncompatibilite(QString &codeacte,QString &dateeffet,QString *incompatibilite);
  bool           MAJAddModificateurActe(QString &codeacte,QString &activite,QString &dateeffet,QString *modificateur);
  bool           MAJAddNoteActe(QString &codeacte,NOTE_ACTE &noteacte);
  bool           MAJAddPhaseActe(ENT_PHASE &phase);
  bool           MAJAddArborescence(ENT_ARBO &arborescence);
  bool           MAJAddNoteArborescence(ENT_NOTEARBO &notearbo);
  bool           MAJAddTabActivite(TAB_ACTIVITE &tabactivite);
  bool           MAJAddTabAssociation(TAB_ASSO *tabassociation);
  bool           MAJAddTabModificateur(TAB_MODIFICATEUR &tabmodificateur);
  bool           MAJAddTypesNotes(TYPE_NOTE &typesnotes);
  bool           MAJAddTabPhase(TAB_PHASE &tabphase);
  bool           MAJAddDatesEffet(QStringList liste);
  void           CreationBase(QString &nomBase);
  void           CreationTables();
  QString        GetTablesEssentielles();
  long           ImportTablesEssentielles(QString &fichier);
  bool           MAJAddTableAcces1(QString &valeur1,QString &valeur2,QString &valeur3);
  bool           MAJAddTableAcces2(QString &valeur1,QString &valeur2);
  bool           MAJAddTableAction1(QString &valeur1,QString &valeur2,QString &valeur3);
  bool           MAJAddTableAction2(QString &valeur1,QString &valeur2,QString &valeur3);
  bool           MAJAddTableTopographie1(QString &valeur1,QString &valeur2);
  bool           MAJAddTableTopographie2(QString &valeur1,QString &valeur2,QString &valeur3);
  // debut des maj pour tarifs
  bool           MAJAddTabTB03(TAB_TB03 *tabtb03);
  bool           MAJAddTabTB04(TAB_TB04 *tabtb04);
  bool           MAJAddTabTB05(TAB_TB05 *tabtb05);
  bool           MAJAddTabTB06(TAB_TB06 *tabtb06);
  bool           MAJAddTabTB07(TAB_TB07 *tabtb07);
  bool           MAJAddTabTB08(TAB_TB08 *tabtb08);
  bool           MAJAddTabTB09(TAB_TB09 *tabtb09);
  bool           MAJAddTabTB10(TAB_TB10 *tabtb10);
  bool           MAJAddTabTB11(TAB_TB11 *tabtb11);
  bool           MAJAddTabTB12(TAB_TB12 *tabtb12);
  bool           MAJAddTabTB13(TAB_TB13 *tabtb13);
  bool           MAJAddTabTB14(TAB_TB14 *tabtb14);
  bool           MAJAddTabTB15(TAB_TB15 *tabtb15);
  bool           MAJAddTabTB17(TAB_TB17 *tabtb17);
  bool           MAJAddTabTB18(TAB_TB18 *tabtb18);
  bool           MAJAddTabTB19(TAB_TB19 *tabtb19);
  bool           MAJAddTabTB20(TAB_TB20 *tabtb20);
  bool           MAJAddCondGenerales(TAB_CONDGEN &tabcondgen);
  bool           MAJAddClasseDmt(TAB_CLASSE_DMT &tabclassedmt);
  bool           MAJAddExoTm(TAB_EXO_TM &tabexotm);
  bool           MAJAddNatAssurance(TAB_NAT_ASS &tabnatass);
  bool           MAJAddAdmRemboursement(TAB_REMBOURS &tabrembours);
  bool           MAJAddFraisDeplacement(TAB_FRAIS_DEP &tabfraisdep);
  bool           MAJAddTypeActe(TAB_TYPE_ACTE &tabtypeacte);
  bool           MAJAddTypeForfait(TAB_TYPE_FORFAIT &tabtypeforfait);
  bool           MAJAddExtDoc(TAB_EXT_DOC &tabextdoc);
  bool           MAJAddCatMed(TAB_CAT_MED &tabcatmed);
  bool           MAJAddCodRegroup(TAB_COD_REGROUP &tabcodregroup);
  bool           MAJAddCatSpec(TAB_CAT_SPEC &tabcatspec);
  bool           MAJAddAgrRadio(TAB_AGR_RAD &tabagrrad);
  bool           MAJAddCodePaiement(TAB_CODE_PAIEMENT &tabcodepaiement);
  bool           MAJAddNumDents(TAB_NUM_DENTS &tabnumdents);
  bool           MAJAddDom(TAB_DOM &tabdom);
  bool           MAJAddAssNonPrevue(TAB_ASS_NPREV &tabassnprev);
  bool           MAJAddRglTarif(TAB_RGL_TARIF &tabrgltarif);
  bool           MAJAddSpecialite(TAB_SPEC &tabspec);
  bool           MAJAddDMT(TAB_DMT &tabdmt);
  bool           MAJAddCompatExoTM(TAB_COMPAT_EXOTM *tabcompatexotm);
  bool           MAJAddValUnitOeuvre(TAB_VAL_UNITE_OEUVRE *tabvalunitoeuvre);
  bool           MAJAddCondGenActe(QString &codeacte,COND_GEN_ACTE *condgenacte);
  bool           MAJAddRecMedActe(QString &codeacte,QString &activite,REC_MED &recmedacte);
  bool           MAJAddListeMots(LISTE_MOTS &liste_mots);
  bool           MAJAddHistorique(ENT_HISTO_PHASE &historique);
  QString        GetTableMedecins();
  long           ImportTableMedecins(QWidget* parent,QString &fichier);
  // fin des maj pour tarifs

  bool           DeleteAllData(const QString &nom_table);
  static    long GotoDebug();
  static     int PassWordEncode(QString &pass);
  static     int PassWordDecode(const char* str_to_decode, const char* str_pass);
  static     QString UINTtoHex( UINT16 val);
  static      UINT16 HexToUINT( const char *str);
  //............................... DATA .............................................................................
 public:
  //................................... Database Config File for CCAM VIew .......................................................

 //................. USER_IDENT ............................
   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
   QString      m_USER_IDENT_TYPE_TABLE;         // SIGEMS pure / MedinTux
   QString      m_USER_IDENT_NOM;                // Nom
   QString      m_USER_IDENT_PRENOM;             // Prenom
   QString      m_USER_IDENT_LOGIN;              // Identifiant d'utilisateur DrTUX
   QString      m_USER_IDENT_PASSWORD;           // mot de passe              DrTux
   QString      m_USER_IDENT_NUM_ORDRE;          // Numéro d'ordre (si il y en a un)
   QString      m_USER_IDENT_PRIMKEY;            // clef primaire de cet enregistrement
   QString      m_USER_IDENT_SIGEMS_LOG;         // login    SIGEMS
   QString      m_USER_IDENT_SIGEMS_PAS;         // password SIGEMS
   QString      m_USER_IDENT_CODE_SPEC;          // password SIGEMS


 //......................... parametres de connexion ...............................................................
  QSqlDatabase  *m_pCCAM_Base;
  QSqlDatabase  *m_pUser_Base;
  QString       m_DriverName;
  QString       m_BaseName;
  QString       m_UserName;
  QString       m_PassWord;
  QString       m_HostName;
  QString       m_Param;
 private:
  QSqlDatabase  m_obj_CCAM_Base;
  QSqlDatabase  m_obj_User_Base;
};

#endif


