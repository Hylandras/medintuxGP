/********************************* CMoteur_Base.h *********************************
 *                                                                                *
 *   #include "CMoteur_Base.h"                                                    *
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
#ifndef  CMOTEUR_BASE_H
#define  CMOTEUR_BASE_H

//=============================================== INCLUDES ===================================================================
#include <qobject.h>
#include <qapplication.h>
#include <qvaluelist.h>
#include <qsqldatabase.h>
#include <qstring.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qdatetime.h>
#include <qcombobox.h>
#include <qcstring.h>
#include <qtextedit.h>
#include <qsqlcursor.h>
#include <qsqlrecord.h>
#include <qdatetime.h>
#include <qdom.h>
#include <qcursor.h>
#include <qprogressbar.h>
//#include "structureparser.h"
#ifndef _GEST_USER                   // placer  _GEST_USER  dans: QMake configuration, Onglet compilateur, champ definition.
   #include "structureparser.h"
#endif
#include "CRubList.h"
#include "../../MedinTuxTools/CGestIni.h"


//.............. pour type de setData() de la fiche identite ..............................................................
#define NEW_IDENTITE 1

#define  FTN_MAJ_STRING         01
#define  FTN_STRING            02
#define  FTN_NUM_STRING        03
#define  FTN_INTEGER           12
#define  FTN_CHAR              23
#define  FTN_DATETIME          34
#define  FTN_DATETIME_TIRET    36

//.......................... Note concernant "Rubriques.h" .................................................................
#include "Rubriques.h"
// Ce fichier definit les differents types de documents ratachés à un patient:
//
// 2001-0000 à 2001-9999   --> Courier
// 2002-0000 à 2002-9999   --> Prescription avec: 2002-0000 à 2002-0000  --> OldPrescription
//                                                2002-0200 à 2002-0299  --> NewPrescription
//                                                2002-0300 à 2002-0399  --> NewCertificat
// 2003-0000 à 2003-9999   --> Observations avec: 2003-0000 à 2003-0000  --> OldObservation
//                                                2003-0200 à 2003-0299  --> NewObservation
//                                                2003-0300 à 2003-0399  --> UrgObservation

//.......................... Note pour disposer des GUID ...................................................................
// pour disposer des fonctions GUID: il faut installer avec drakconf:  libext2fs2-devel-1.32-2mdk
// qui installe les bibliothèques:
//
//                  /usr/include/uuid
//                  /usr/include/uuid/uuid.h
//                  /usr/lib/libuuid.a
//                  /usr/lib/libuuid.so    (lien sur: /lib/libuuid.so.1.2)
//                  /lib/libuuid.so.1.2
//                  /lib/libuuid.so.1      (lien sur: /lib/libuuid.so.1.2)
//
// pour pouvoir linker avec ces bibliothèques rajouter dans:
//
//                Boite de dialogue de QMake, bouton de droite: "Configurer le projet" (icone en forme de clef anglaise)
//                Onglet: "Bibliothèques" , zone de dialogue du bas: "Link libraries outside project"
//                la ligne: /lib/libuuid.so.1.2
//
// Rajouter le .h suivant pour inclure les prototypes des fonctions fournies par la lib:     /lib/libuuid.so.1.2
/*
#ifdef Q_WS_X11
     #include <uuid/uuid.h>
     typedef struct _GUID {u_int32_t    Data1;
                           u_int16_t    Data2;
                           u_int16_t    Data3;
                           u_int8_t     Data4[8];
                          } GUID;
     #define F_EXT  ""
#else
     #include <quuid.h>
     #include <objbase.h>
     #define F_EXT  ".exe"
#endif
*/

//=============================================== DEFINE ===================================================================

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))
#define SKIP_BLANK_CAR(a)     while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define NEXT_LINE(a)          while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++

#define   RUB_READ_IN_LIST        2
#define   RUB_READ_ON_DISK        1

#define   RUB_IS_TO_DELETE        1

#define  TYP_DECOD_INDIC                1
#define  TYP_DECOD_ORDO                 2
#define  TYP_DECOD_WITH_MODIF_ZONE      4

#define  ERR_PASS_MAX       1
#define  ERR_PASS_ZERO      2
#define  ERR_PASS_OK        3

#define BASE_DRTUX "BaseDrTux"

//====================================== PROTOTYPES fonctions hors classes===================================================


//===================================================== CIdentFieldData =====================================================
class CIdentFieldData
{
    //................... constructeur destructeur ..................................
public:
    CIdentFieldData(const char* field_name,
                    const char* field_value,
                    int field_type
                   )
              {m_Type           = field_type;  // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_Name           = field_name;
               m_Value          = field_value;
              }
    ~CIdentFieldData()
              {
              }
    //................... data ......................................................
public:
    int           m_Type;
    QString       m_Name;
    QString       m_Value;
};

//..................... Liste de la classe definie au dessus .........................................................
typedef QPtrList<CIdentFieldData> PtrListIdentFieldData;

//===================================== CMoteurBase ========================================================================
class CMoteurBase:public CGestIni
{
 //............................... METHODES .........................................................................

 public:
   enum exeMode {
        endWait,
        noWait
    };
  //................................... configuration et connection de la base de données ..............................
  CMoteurBase();
  CMoteurBase(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
              const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
              const char* user,          // = "root"
              const char* password,      // = ""
                  const char* hostname,
                          const char* confFile,
              const char* baseLabel,
                          QString*    errMess);

  CMoteurBase(const char* driver,                 // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
              const char* DataBaseName,           // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                  const char* user     = "ro",        // = "root"        utilisateur
              const char* password = "",          // = ""            paswsword
                  const char* hostname = "localhost", // = "localhost"   on travaille en local sur notre disque
                  const char* confFile = 0,               // si zero il sera choisi: DEFAULT_CONF_BASE par defaut
                  QString*    errMess  = 0);

  ~CMoteurBase();
  void initBase   (const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                   const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                   const char* user,          // = "root"
                   const char* password,      // = ""
                       const char* hostname,
                               const char* confFile,
                   const char* baseLabel,
                               QString*    errMess);

  QSqlDatabase*  BaseConnect(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                             const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                             const char* user,          // = "root"
                             const char* pasword,       // = ""
                             const char* hostname,      // = "localhost"
                             QString*    errMess ,
                             QString     baseLabel);
  int      SetConfBase(const char* confFile, QString *errMess);
  char    *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err);
  double   GetMedinTuxVersion(QString &version);
  QString  GetNbRecord(const QString &table);
  QString  GetDataBaseLabel(){return m_BaseLabel;};
  QString  Utf8_Query(QSqlQuery &cur, int field);
  //.................................... NOTES et INTERVENANTS ........................................................................
  void      PatientIntervenantsGetData( const char *primKeyPat , QListView *pQListView, QString *errMess =0 );
  void      PatientIntervenantsGetData(const char *primKeyPat , QStringList &intervList, const QString &specialite="", const char *validStr ="NP", const char *decalage="\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
  QString   IdentValidCarToFieldName(int c);
  int       PatientIntervenantsWrite( const char *primKeyPat , const char* guidPat,  const char* intervPk );
  QString   PatientIntervenantsIsThisExist( const char* intervPk, const char *primKeyPat, QLabel *errMess=0 );
  int       PatientIntervenantsCreate(const char *primKeyPat , const char* guidPat,  const char* intervPk );
  int       PatientIntervenantsDelete( const char *primKey);
  int       PatientIntervenantsAllDelete( const char *primKeyPat);
  QString   PatientNoteIsThisExist(  const char *primKeyPat, QLabel *errMess  = 0 );
  void      PatientNoteGetData(const char *primKeyPat ,  QString& data, QString *errMess=0);
  int       PatientNoteDelete( const char *primKey);
  int       PatientNoteWrite(  const char *primKeyPat , const char* guidPat,  const char* data);
  int       PatientNoteCreate( const char *primKeyPat , const char* guidPat,  const char* data);
  int       PatientNoteUpdate( const char* primKey,     const char *primKeyPat , const char* guidPat,  const char* data);
//.................................... CCAM ........................................................................

  static void     GotoDebug();
  //..................................... Codes Postaux ..............................................................
  QString         GetVilleFromCodePostal(const QString &codePostal);
  QString         GetCodePostalFromVille(const QString &Ville);
  void            GetVilleFromCodePostal(const QString &codePostal, QStringList &villeList);
  void            GetCodePostalFromVille(const QString &ville     , QStringList &codePostalList);

  //..................................... conversions de bases ............................................................
  int ConvertToNew(QLabel* pQlabelMess  = 0 , QProgressBar* pQProgressBar  = 0 );

  //..................................................................................................................
  long            GetPatientList(       QListView *pQlistView,     // pointeur sur une QListView à 3 colonnes
                                  const QString   &qstr_nom,       // nom d'entrée
                                  const QString   &qstr_prenom,    // prenom d'entré
                                        QLabel    *statutMess=0,   // pointeur sur Qlabel de sortie message
                                        QLabel    *errMess=0       // pointeur sur Qlabel de sortie message
                                 );
  //.................................... identite et Dossiers....................................................................
  QString         DossIdentIndexCreate(QString& nom, QString& prenom,    QString& numGUID, PtrListIdentFieldData *plist_field );
  long            DossIdentDataCreate(QString& refPk, QString& numGUID, PtrListIdentFieldData *plist_field );
  long            DossIdentIndexUpdate(QString& refDossPk, QString& nom, QString& prenom, PtrListIdentFieldData *plist_field );
  long            DossIdentDataUpdate(QString& refDossPk,     PtrListIdentFieldData *plist_field = 0);
  long            DossIdentBufferSetValue(QSqlRecord *buffer, PtrListIdentFieldData *plist_field);
  void            DossIdentDelete(RUBREC_LIST *pRubList, QString num_GUID, QString id_doss);
  QString         GetDossPrimKeyFromGUID(const QString &guid);

  //................................ param ...........................................................................
  void            Param_SavParam(QString user, QString *pParam  = 0 );
  void            Param_GetParam(QString user, QString *pParam  = 0 );
  //..................................................................................................................
  QString         PermsUserCreateOrUpdate(const QString &primKey , const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs);
  int             PermsUserUpdate(const QString &primKey , const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs);
  QString         PermsUserCreate(const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs);
  int             PermsUserDeleteFromPk( const char *primKey);
  int             PermsUserListDelete( const char *signUserGUID);
  int             PermsUserRefToUserDelete( const char *userGUID);
  long            PermsUserGetList(          QListView *pQlistView,
                                       const QString   &signGUID,
                                       const QString   &imagePath   = "",
                                        QLabel         *statutMess  = 0 ,
                                        QLabel         *errMess     = 0
                             );
  void            PermsUserChildGetList(         QListViewItem *pQlistViewItem,
                                                  const QString  &signGUID
                                        );
  QString         PermsUserIsThisUserFriendFromName(   const QString  &signUser, const QString  &friendUser);
  QString         PermsUserIsThisUserFriendFromGUID(   const QString  &signGUID, const QString  &friendGUID);
  int             PermsUserSignUserListForThisUser(   const QString  &user, QStringList  &userList,  const char  *pDroit = 0);
  static bool     IsThisDroitExist(const char *listDroits, const char *droitToFind);
  int             TypeDoc_To_PrintTypeDoc(int typeDoc);
  QString         TypeDoc_To_PrintTypeDoc(const QString &typeDoc);
  void            GetUserListType( QStringList &typList , QString    *errMess  = 0 );
  void            GetUserList( QStringList &qstringList , QLabel    *errMess  = 0 );
  long            GetUserList   (       QListView *pQlistView,     // pointeur sur une QListView
                                  const QString   &qstr_nom,       // nom d'entrée
                                  const QString   &qstr_prenom,    // prenom d'entré
                                  const bool       childsMustBeFind = TRUE,
                                  const QString   *qstr_UserQualite = 0,
                                        QLabel    *statutMess=0,   // pointeur sur Qlabel de sortie message
                                        QLabel    *errMess=0       // pointeur sur Qlabel de sortie message
                                 );
  long            GetUserDocList( QListView   *pQlistView,
                                  const char      *primKey,
                                  const char      *qstr_nom = 0,      /* = 0  */
                                  QLabel    *statutMess     = 0,      /* = 0  */
                                  QLabel    *errMess        = 0       /* = 0  */
                                );
  void            GetUserList( QListView *pQlistView, const QStringList &listUserAlreadyPresent, QString    *errMess  = 0  );
  int             UserDroitsCount(const QString &droitToFind , QString    *errMess  = 0 );
  QString         GetUserPrimKey(  QString user, QString *cripted_password = 0);
  int             SaveUserPassWord(QString user, QString  cripted_password);
  int             VerifyUserPassWord(QString user, QString password);
  QString         GetUserPermisions( QString &user, QString    *errMess  = 0 );
  QString         GetUserDocument(   const char*    user     = "ro",
                                     const char* nameDocType = "Ordo Standard",
                                     int            type     = TYP_ORDONNANCE,
                                     QString*      pFoot     = 0,
                                     int        sub_type     = 0,
                                     QString*  pk_record     = 0
                                 );
  QString         GetUserDocument(QString         dataPk,
                                  int             type       = TYP_ORDONNANCE,    // TYP_ORDONNANCE 2  TYP_CERTIFICAT 3 TYP_OBSERVATION 4
                                  QString*        pFoot      = 0,                 // adresse de la chaine où l'on veut recuperer le pied de page
                                  int             sub_type   = 0                  // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                 );
  long            InitComboWithUserDocList( QComboBox       *pQComboBox,
                                            QString         user,
                                            QString         type1 = QString::null,
                                            QString         type2 = QString::null,
                                            int             clear = 1
                                          );

  void            OutSQL_error(QSqlQuery &cur, const char *messFunc=0, const char *requete=0);
  QSqlQuery      *GetIdentiteFromPrimKey(const char* PrimKey );    // identité dossier patient
  QString         CreateNewUser(const QString &nom, const QString &prenom, const QString &login, const QString &droit="med", const QString &pass="",const QString &nir="");
  QString         GetUserPrimKeyFromNumOrdre(const QString &nir);
  QString         GetUserPrimKeyFromGUID(const QString &guid);
  QSqlQuery      *GetUserIdentiteFromPrimKey(const char *userPrimKey , QStringList &fiedList);  // identité dossier utilisateur
  QString         IsThisUserDocExist( int type, const char *refUserPrimKey, const char *nom=0,   QLabel *errMess = 0 );
  QString         User_GetBestHeadFoot_ToPrintDoc(  RUBREC_LIST::iterator it, const char *refUserPrimKey,     QLabel *errMess = 0 );
  QString         GetUserDataFromPrimKey(const char *primKey);
  QString         ReadDrTuxUserParametres(const QString &user);
  int             CreateUserDataFromPrimKey(int type, const char *refUserPrimKey , const char* name,  const char* data);
  int             UpdateUserDataFromPrimKey( const char *primKey, int type, const char *refUserPrimKey , const char* name,  const char* data);
  int             DeleteUserDataFromPrimKey( const char *primKey );
  int             DeleteUserFromPrimKey(const char *refUserPrimKey);
 //............................................ Rubriques...............................................................
  long            GetAllRubriqueFromDate(RUBREC_LIST *pRubList, const QDate &qdtRef, QString type="ALL_TYP",  QStringList *pRubNameList =0);
  int             ChangeRubUserName(QString userName, QString rubPrimKey, QString *errMess=0);
  long            initRubriquesList(RUBREC_LIST *pRubList, QString m_NumGUID);
  long            initConboBoxWithRubList(RUBREC_LIST *pRubList, QComboBox* pComboBox, QString type);
  void            ReplaceDataInRubList(const QString &data, RUBREC_LIST *pRubList, int id);
  void            ReplaceDataInRubList(const char* text, long len_data, RUBREC_LIST *pRubList, int id);
  int             GetDataFromRubList(QByteArray &pQByteArray, RUBREC_LIST *pRubList, int id);
  int             GetDataFromRubList(QByteArray &pQByteArray, RUBREC_LIST::iterator it);
  int             GetDataFromRubList( QString &stringDST, RUBREC_LIST *pRubList, int id);
  QString         GetDataFromRubList( CRubRecord  *pCRubRecord);
  int             GetDataFromRubList( QString &stringDST, CRubRecord  *pCRubRecord);
  void            RubList_SetItemDeleted(RUBREC_LIST *pRubList, int id);
  void            RubList_SetItemNotDeleted(RUBREC_LIST *pRubList, int id);
  int             RubList_IsItemDeleted(RUBREC_LIST *pRubList, int id);
  QString         GetRubPrimKeyFromGUID(const QString &guid, QString *ref_blobsPk);

  //..................................................................................................
  QString         GetFieldValue( QString table, QString fieldToGet, QString fieldToMatch, QString valueToMatch,  QVariant *pQVariant=0);
  QString         GetFieldValue_WithoutOpenCloseBase(QString table, QString fieldToGet, QString fieldToMatch, QString valueToMatch,  QVariant *pQVariant=0);
  //.......................... sauvegarde dossier ....................................................
  long            RubListSave( RUBREC_LIST *pRubList, QString numGUID);
  long            RubNbToSave( RUBREC_LIST *pRubList);
  int             RubListUpdate(RUBREC_LIST::iterator it);
  int             RubListBlobUpdate(RUBREC_LIST::iterator it);
  int             RubListCreate(RUBREC_LIST::iterator it, QString numGUID);
  QString         RubListBlobCreate(RUBREC_LIST::iterator it, QString numGUID);
  int             RubListDelete(RUBREC_LIST::iterator it);
  int             GetDataFromRub(QByteArray &qByteArray, const QString &blobsPrimKey);
  //................................................... Verrous .......................................
  int             VerrouilleDossier    ( const QString &dossGUID,
                                         const QString &userName,
                                         QString &usedBy
                                       );
  int             VerrouilleTable(const QString &tableName, QSqlDatabase* dataBase);
  int             DeVerrouilleTable( QSqlDatabase* dataBase);
  int             IsDossierVerrouille (  const QString &dossGUID, QString &usedBy );
  void            DeVerrouilleDossier (  const QString &dossGUID);
  QString         VerrouilleRubrique(    const QString &dossPk,
                                         const QString &dossGUID,
                                         const QString &rubName,
                                         const QString &rubPk,
                                         const QString &userName,
                                         const QString &dateTime = ""
                                    );
  void            DeVerrouilleRubrique(    const QString &dossPk,
                                           const QString &dossGUID,
                                           const QString &rubName,
                                           const QString &rubPk,
                                           const QString &userName,
                                           const QString &dateTime
                                       );
  //............................. faut bien generer les GUID et codage decodage mots de passe .........................................
  static int      PassWordEncode(QString &pass);
  static int      PassWordDecode(const char* str_to_decode, const char* str_pass);
  static  UINT16  HexToUINT( const char *str);
  static  QString UINTtoHex( UINT16 val);
  QString         GUID_Create( /*const char *format="%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"*/);
  //QString         GuidToString(GUID *guid, const char *format="%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X");
  static QString  CalculeDiffAge(QDate *pDebut, QDate *pFin = NULL, bool bTexte = TRUE , int bAbrege = 1 );
  //...................................... routine utilitaires diverses ..........................................
  static char    *GotoNextDebLigne(const char* txt);
  static char    *GotoEndOfLigne(const char* txt);
  static char    *GotoNextSignifiant(const char* txt);
  static char    *JumpObject(char *pt, char *end_rtf);
  static QString  RTF_ToASCII(const char *ptIn, long len);
  static int      IsRtfControlCar(char *pt);

  //............................... DATA .............................................................................
 public:

  //......................... parametres de connexion ...............................................................
  QSqlDatabase* m_DataBase;
  QString       m_DriverName;
  QString       m_BaseName;
  QString       m_UserName;
  QString       m_PassWord;
  QString       m_HostName;

  //......................... configuration des tables ...............................................................

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
   QString      m_DOSS_IDENT_NUMSS;         // Numéro de secu
   QString      m_DOSS_IDENT_IS_ASS;        // le patient est il l'assuré
   QString      m_DOSS_IDENT_NOM_ASS;       // nom de l'assuré
   QString      m_DOSS_IDENT_PRENOM_ASS;    // prénom de l'assuré
   QString      m_DOSS_IDENT_PROF;          // profession
   QString      m_DOSS_IDENT_TITRE;         // Titre( Dr Mr Md Pr ...)
   QString      m_DOSS_IDENT_REF_PK;        // reference à la primary key: DOSS_INDEX_PRIM_KEY de la table: DOSS_INDEX

   //................. DOSS_NOTE ............................
   QString      m_DOSS_NOTE_TBL_NAME;               // nom de la table de la note du patient
   QString      m_DOSS_NOTE_PK;                     // Clef primaire
   QString      m_DOSS_NOTE_PAT_GUID;               // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_NOTE_PAT_PK ;                // Clef primaire du dossier auquel fait reference cette note
   QString      m_DOSS_NOTE_HTML;                   // Données HTML de la note

   //................. DOSS_INTERVENANTS ............................
   QString      m_DOSS_INTERVENANTS_TBL_NAME;      // nom de la table des intervenants d'un patient
   QString      m_DOSS_INTERVENANTS_PK;            // Clef primaire de cet enregistrement
   QString      m_DOSS_INTERVENANTS_PAT_GUID;      // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_INTERVENANTS_PAT_PK ;       // Clef primaire du dossier auquel fait reference cet intervenant
   QString      m_DOSS_INTERVENANTS_INTERVPK;      // Clef primaire de l'intervenant auquel fait reference cet enregistrement

   //................. INTERVENANTS ............................
   QString      m_INTERVENANTS_TBL_NAME;            // nom de la table des intervenants
   QString      m_INTERVENANTS_PK;                  // primary key
   QString      m_INTERVENANTS_NOM;                 // nom de l'intervenant
   QString      m_INTERVENANTS_PRENOM;              // prenom de l'intervenant
   QString      m_INTERVENANTS_SPEC;                // spécialité de l'intervenant
   QString      m_INTERVENANTS_TITRE;               // titre de l'intervenant (Dr Pr Mr Mm etc...)
   QString      m_INTERVENANTS_RUE;                 // rue de l'intervenant
   QString      m_INTERVENANTS_CODE_POST;           // Code postal de l'intervenant
   QString      m_INTERVENANTS_VILLE;               // Ville de l'intervenant (exple: Medecin Urgentiste)

  //................. DOSS_HISTO ............................
   QString      m_DOSS_HISTO_TBL_NAME;    // nom de la table de l'HISTORIQUE
   QString      m_DOSS_HISTO_GUID;        // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_HISTO_DATE;        // date de l'enregistrement
   QString      m_DOSS_HISTO_DUREE;       // duree de l'évènement
   QString      m_DOSS_HISTO_USER;        // utilisateur associé à cette date

 //................. DOSS_RUB_HEAD ............................
   QString      m_DOSS_RUB_HEAD_TBL_NAME;        // nom de la table de l'header des RUBRIQUES
   QString      m_DOSS_RUB_HEAD_PRIMKEY;         // clef primaire de cet enregistrement
   QString      m_DOSS_RUB_HEAD_GUID;            // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_RUB_HEAD_REF_DOSS_PK;     // Clef primaire du dossier auquel l'enregistrement fait reference (doublon avec m_DOSS_RUB_HEAD_GUID)
   QString      m_DOSS_RUB_HEAD_NOM_DATE;        // Court texte descriptif de l'enregistrement.
   QString      m_DOSS_RUB_HEAD_DATE;            // date de l'enregistrement
   QString      m_DOSS_RUB_HEAD_DUREE;           // duree de l'enregistrement
   QString      m_DOSS_RUB_HEAD_TYPE;            // Type de rubrique
   QString      m_DOSS_RUB_HEAD_USER;            // utilisateur associé à rubrique
   QString      m_DOSS_RUB_HEAD_SIGN_USER;       // utilisateur associé signataire de la rubrique
   QString      m_DOSS_RUB_HEAD_REF_BLOBS_PK;    // reference au Pk du blobs

  //................. DOSS_RUB_DATA ............................
   QString      m_DOSS_RUB_DATA_TBL_NAME;        // nom de la table des données
   QString      m_DOSS_RUB_DATA_GUID;            // GUID du dossier auquel l'enregistrement fait reference
   QString      m_DOSS_RUB_DATA_PRIMKEY;         // clef primaire de cet enregistrement  (le meme que le header)
   QString      m_DOSS_RUB_DATA_BLOB;            // données binaires

  //................. USER_IDENT ............................
   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données
   QString      m_USER_IDENT_NOM;                // Nom
   QString      m_USER_IDENT_PRENOM;             // Prenom
   QString      m_USER_IDENT_TITRE;              // Titre( Dr Mr Md Pr ...)
   QString      m_USER_IDENT_LOGIN;              // Identifiant d'utilisateur
   QString      m_USER_IDENT_PASSWORD;           // mot de passe
   QString      m_USER_IDENT_DROITS;             // Droits de cet utilisateur
   QString      m_USER_IDENT_SEXE;               // sexe de l'utilisateur (si il y en a un)
   QString      m_USER_IDENT_NUM_ORDRE;          // Numéro d'ordre (si il y en a un)
   QString      m_USER_IDENT_PRIMKEY;            // clef primaire de cet enregistrement
   QString      m_USER_IDENT_GUID;               // Identifiant unique de cet utilisateur
   QString      m_USER_IDENT_CONVENTION;         // convention exple "01 MEDECINE GENERALE"
   QString      m_USER_IDENT_CODE_POST;          // Code postal
   QString      m_USER_IDENT_VILLE;              // Ville d'exercice
   QString      m_USER_IDENT_RUE;                // Rue d'exercice
   QString      m_USER_IDENT_QUALITE;            // Type d'intervenant (exple: Medecin Urgentiste)

  //................. USER_DATA ............................
   QString      m_USER_DATA_TBL_NAME;           // nom de la table des données des RUBRIQUES
   QString      m_USER_DATA_PK;                 // clef primaire de cet enregistrement
   QString      m_USER_DATA_REF_PK;             // Ref à la clef primaire de l'utilisateur auquel appartiennent les données
   QString      m_USER_DATA_NAMEDOC;            // nom des données
   QString      m_USER_DATA_TYPE;               // type de données
   QString      m_USER_DATA_BLOB;               // données proprement dites

   //................. VERROU ............................
   QString      m_VERROU_TBL_NAME;              // nom de la table des données des verrous
   QString      m_VERROU_PK;                    // clef primaire de cet enregistrement
   QString      m_VERROU_DOSS_PK;               // Ref à la clef primaire du dosssier verrouillé
   QString      m_VERROU_DOSS_GUID;             // GUID du dossier verrouillé
   QString      m_VERROU_RUB_NAME;              // Nom de la rubrique verrouillee
   QString      m_VERROU_RUB_PK;                // PK du header de rubrique verrouille
   QString      m_VERROU_USER_NAME;             // utilisateur ayant verrouillé les données
   QString      m_VERROU_SIGN_USER_NAME;        // utilisateur signataire ayant verrouill? les donn?es
   QString      m_VERROU_DATE_TIME;             // date de verrouillage des données

  //................. USER_PERMS ............................
   QString      m_USER_PERMS_TBL_NAME;          // nom de la table des droits des utilisateurs non prescripteurs
   QString      m_USER_PERMS_PK;                // clef primaire de cet enregistrement
   QString      m_USER_PERMS_SIGN_GUID;         // Ref au GUID du signataire autorisant cet utilisateur
   QString      m_USER_PERMS_FRIEND_GUID;       // GUID de cet utilisateur
   QString      m_USER_PERMS_FRIEND_DROITS;     // Droits que le signataire attribue à cet utilisateur
   QString      m_USER_PERMS_FRIEND_BLOBS;      // Configuration des autorisations fines (liste des fichiers autorisés avec les droits)
private:
   QString      m_BaseLabel;
   int          m_XmlDepth;
   int          m_ConvertRun;

};


#endif

