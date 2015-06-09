 /********************************* CMoteur_Base.h ********************************
 *  #include "CMoteur_Base.h"                                                     *
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
 *   Commissariat √  l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F√©d√©ration, 75752 PARIS cedex 15.   *
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
#include <QTreeWidget>

#include <qobject.h>
#include <qtimer.h>
#include <qapplication.h>
#include <q3valuelist.h>
#include <qsqldatabase.h>
#include <qstring.h>
#include <q3listview.h>
#include <qlabel.h>
#include <qdatetime.h>
#include <Q3ComboBox>
#include <q3cstring.h>
#include <q3textedit.h>
#include <q3sqlcursor.h>
#include <qsqlrecord.h>
#include <qdatetime.h>
#include <qcursor.h>
#include <q3progressbar.h>
//Added by qt3to4:
#include <QSqlQuery>
#include <Q3PtrList>


#include "CRubList.h"
#include "CGestIni.h"
#include "C_DBVarDrTux.h"


//.............. pour type de setData() de la fiche identite ..............................................................
#define NEW_IDENTITE 1
#define  ST_LIST_PATIENT_MAX "200"
#define  NB_LIST_PATIENT_MAX  200
#define  FTN_MAJ_STRING        01
#define  FTN_STRING            02
#define  FTN_NUM_STRING        03
#define  FTN_INTEGER           12
#define  FTN_CHAR              23
#define  FTN_DATETIME          34
#define  FTN_DATETIME_TIRET    36
#define  FTN_STR_DATETIME      38
//.......................... Note concernant "Rubriques.h" .................................................................
#include "Rubriques.h"
// Ce fichier definit les differents types de documents ratach√©s √  un patient:
//
// 2001-0000 √  2001-9999   --> Courier
// 2002-0000 √  2002-9999   --> Prescription avec: 2002-0000 √  2002-0000  --> OldPrescription
//                                                2002-0200 √  2002-0299  --> NewPrescription
//                                                2002-0300 √  2002-0399  --> NewCertificat
// 2003-0000 √  2003-9999   --> Observations avec: 2003-0000 √  2003-0000  --> OldObservation
//                                                2003-0200 √  2003-0299  --> NewObservation
//                                                2003-0300 √  2003-0399  --> UrgObservation

//.......................... Note pour disposer des GUID ...................................................................
// pour disposer des fonctions GUID: il faut installer avec drakconf:  libext2fs2-devel-1.32-2mdk
// qui installe les biblioth√®ques:
//
//                  /usr/include/uuid
//                  /usr/include/uuid/uuid.h
//                  /usr/lib/libuuid.a
//                  /usr/lib/libuuid.so    (lien sur: /lib/libuuid.so.1.2)
//                  /lib/libuuid.so.1.2
//                  /lib/libuuid.so.1      (lien sur: /lib/libuuid.so.1.2)
//
// pour pouvoir linker avec ces biblioth√®ques rajouter dans:
//
//                Boite de dialogue de QMake, bouton de droite: "Configurer le projet" (icone en forme de clef anglaise)
//                Onglet: "Biblioth√®ques" , zone de dialogue du bas: "Link libraries outside project"
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

#define   Min(a, b)                ((a) < (b) ? (a) : (b))
#define   Max(a, b)                ((a) > (b) ? (a) : (b))
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++
#define   DUMMY_TEST_SQL_COMMAND   "SELECT 1"
#define   DB_TIME_TO_RECONNECT     1800000  //reconnexion toutes les 1/2 heures
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
              {m_Type           = field_type;  // peut etre √  zero lors de nouveaux documents dans liste et non enregistr√©s
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
//typedef QList<CIdentFieldData> PtrListIdentFieldData;
//======================== PtrListIdentFieldData =============================

class PtrListIdentFieldData : public QList <CIdentFieldData*>
{
public:
    //------------------------ PtrListIdentFieldData -------------------------------
    PtrListIdentFieldData( const PtrListIdentFieldData& l )
        : QList<CIdentFieldData*>(l)
    {
    }
    PtrListIdentFieldData()
         : QList<CIdentFieldData*>()
    {
    }
    //------------------------ ~PtrListIdentFieldData -------------------------------
    ~PtrListIdentFieldData(){clear(); }
    //------------------------ appendField -------------------------------
    CIdentFieldData *appendField(CIdentFieldData* pCIdentFieldData)
    {append ( pCIdentFieldData );
     return pCIdentFieldData;
    }
    //------------------------ clear -------------------------------
    void clear()
    { while (!isEmpty())
        {CIdentFieldData* pCIdentFieldData = takeFirst();
         delete pCIdentFieldData;
        }
    }
};

//===================================== CMoteurBase ========================================================================
class CMoteurBase: public QObject,          // classe QObject classique
                   public C_DBVarDrTux      // classe des noms des tables et des champs
{
 Q_OBJECT

 //............................... METHODES .........................................................................

 public:
   enum exeMode {
        endWait,
        noWait
    };
   enum verifyMode {
        verifyBaseStruct,
        withoutVerifyBaseStruct
    };
   enum syncMode {
        notNomade        = 0,
        fullReplication  = 1,        // le nomade est soit connecte au serveur soit deconnecte (nomade)
        fullSynchro      = 2,        //.le nomade est toujours deconnecte et parfois se synchronise au reseau de reference
        progMasterUpdate = 3,
        scriptOnly       = 4
    };
   enum GUID_Mode {
        byQT        = 0,        // algorithme de QT
        byMyself    = 1         // algorithme interne ‡ MedinTux
    };
  //................................... configuration et connection de la base de donn√©es ..............................
  CMoteurBase(const QString & driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
              const QString & DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de donn√©es (userDSN)
              const QString & user,          // = "root"
              const QString & password,      // = ""
              const QString & hostname,
              const QString & port,
                    QString & confData,
              const QString & baseLabel,
              QString*    errMess,
              QObject *       parent = 0,
              const QString   & name = "CMoteurBase",
              verifyMode verifyMode  = CMoteurBase::verifyBaseStruct);

  ~CMoteurBase();
  void         killBase();
  //...............................  versions ...................................................
    int      GetMedinTuxNormalisedVersion();
    static int      normaliseVersion(QString &version, const QString &sep ="");
    int      normaliseVersion(const QString &version, const QString &sep = "");
    int      GetMedinTuxNormalisedVersion(QString &version, const QString &sep ="");
    double   GetMedinTuxVersion();
    double   GetMedinTuxVersion(QString &version);
    void     SetMedinTuxVersion(const QString &version);

  int          verifyBaseIntegrity(const QString &confFile, QString *errMess);
  QStringList  getTableListFromCfgFile(const QString &confFile, QString *errMess);
  int          tryToSetThisTable(const QString &tbl, const QString fileSQL, const QString &baseLabel);
  void         ParseSQL_InsertInto(QString &text, const QString &baseLabel);

  int            BaseConnect(const QString & driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                             const QString & DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de donn√©es (userDSN)
                             const QString & user,          // = "root"
                             const QString & pasword,       // = ""
                             const QString & hostname,      // = "localhost"
                             const QString & port,
                             QString*        errMess ,
                             QString         baseLabel);

  QSqlDatabase getDatabase() const {return QSqlDatabase::database(m_BaseLabel);}

  QString  GetNbRecord(const QString &table);
  QString  GetDataBaseLabel(){return m_BaseLabel;}
  QString  Utf8_Query(QSqlQuery &cur, int field);
  /*
  //..................................... Nomadisme ...........................................
  int     synchroNomadeToMasterUpdate(         const QString & driver,
                                               const QString & baseLabel,
                                               const QString & baseToOpen,
                                               const QString & user,
                                               const QString & pasword,
                                               const QString & hostname,
                                               const QString & port);

  void    synchroDossierDelete( const QString &primKeyPat,  QSqlDatabase *destMasterDB);

  QString synchroIsThisIdentNoteExist(const QString &pk, QSqlDatabase *destMasterDB);
  void    synchroIdentNoteDelete( const QString &primKey,  QSqlDatabase *destMasterDB);
  void    synchroIdentIntervenantsDelete( const char *primKeyPat, QSqlDatabase *destMasterDB);

  QStringList    synchroGetTablesList();
  void    synchroSetDossierToDelete( const QString &primKeyPat);
  void    synchroTablesCreate();
  void    synchroTablesCreateDispatcher(const QString &table);
  void    synchroTableNoteCreate(const QString &table, QSqlRecord &rec, const QString &primKey, const QString &primKeyName);
  void    synchroTableRubriqueHeadBlobsCreate(const QString &table, QSqlRecord &rec, const QString &primKey, const QString &primKeyName);
  void    synchroTableIndexFichePatientCreate(const QString &table, const QSqlRecord &rec, const QString &primKey, const QString &primKeyName);
  QString synchroTableCreate(const QString &table, const QSqlRecord &rec, const QString &primKey, const QString &primKeyName);
  void    synchroTablesDelete();
  void    synchroTableDelete(const QString &table, const QSqlRecord &rec, const QString &primKey, const QString &primKeyName);
  void    synchroTablesUpdate();
  void    synchroTableUpdate(const QString &table, const QSqlRecord &rec, const QString &primKey, const QString &primKeyName);

  void    synchroInitTables();
  void    synchroInitTable(const QString &tableName);
  void    synchroDropTables();
  void    synchroDropTable(const QString &tableName);
  void    synchroWriteMofidication(QString tableName, const QString &pk_toWrite, const QString &pkName, const QString &modif);
  void    synchroDeleteStatus(QString tableName, const QString &pk_toDelete);
  QString synchroGetStatus(QString tableName, const QString &pk_toGet, QString *pk_ret =0);
 */
  //.................................... NOTES et INTERVENANTS ........................................................................
  //-------------------------------------- PatientIntervenantsGetData ----------------------------------------------------------------------------
  /*! \brief Fonction qui filtre, recupere et renseigne une QTreeWidget avec la liste des intervenants correspondant au patient dont la primKeyPat est donn√©e en entr√©e
  *  \param pQlistView     QTreeWidget*  : pointeur sur la QTreeWidget √  remplir avec la liste des intervenants
  *  \param primKeyPat     const char*   : pointeur sur la clef primaire du patient
  *  \param errMess        QString *    : pointeur (par defaut nul) sur une QString dans laquelle retourner un eventuel message d'erreur
 */
  void         PatientIntervenantsGetData( const char *primKeyPat , QTreeWidget *pQListView, QString *errMess =0 );
  QStringList  PatientIntervenantsGetData(const QString &qualite, const QString &sep ="|", QString *errMess = 0);
  void         PatientIntervenantsGetData( const char *primKeyPat , QStringList &intervList, const QString &specialite="", const char *validStr ="NP", const char *decalage="\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
  QString   IdentValidCarToFieldName(int c);
  int       PatientIntervenantsWrite( const char *primKeyPat , const char* guidPat,  const char* intervPk , const QString &qualite);
  QString   PatientIntervenantsIsThisExist( const char* intervPk, const char *primKeyPat, QLabel *errMess=0 );
  int       PatientIntervenantsCreate(const char *primKeyPat , const char* guidPat,  const char* intervPk , const QString &qualite);
  int       PatientIntervenantsUpdate(const QString &pk, const char *primKeyPat , const char* guidPat,  const char* intervPk, const QString &qualite);
  int       PatientIntervenantsDelete( const char *primKey, const char *path_GUID  = 0 );
  int       PatientIntervenantsAllDelete( const char *primKeyPat);
  QString   PatientNoteIsThisExist(  const char *primKeyPat, QLabel *errMess  = 0 );
  void      PatientNoteGetData(const char *primKeyPat ,  QString& data, QString *errMess=0);
  int       PatientNoteDelete( const char *primKey,                                                                       int mode /*= CMoteurBase::esclave*/);
  int       PatientNoteWrite(  const char *primKeyPat , const char* guidPat,  const char* data,                           int mode /*= CMoteurBase::esclave*/);
  int       PatientNoteCreate( const char *primKeyPat , const char* guidPat,  const char* data,                           int mode /*= CMoteurBase::esclave*/);
  int       PatientNoteUpdate( const char* primKey,     const char *primKeyPat , const char* guidPat,  const char* data,  int mode /*= CMoteurBase::esclave*/);
//.................................... CCAM ........................................................................

  static void     GotoDebug();
  //..................................... Codes Postaux ..............................................................
  QString         GetVilleFromCodePostal(const QString &codePostal);
  QString         GetCodePostalFromVille(const QString &Ville);
  void            GetVilleFromCodePostal(const QString &codePostal, QStringList &villeList);
  void            GetCodePostalFromVille(const QString &ville     , QStringList &codePostalList);
  //..................................... conversions et maintenance de bases ............................................................
  void RenameTableWhithCorrectName();
  void RenameTableWhithCorrectName(const QString &correct_name_table, QSqlQuery *pQSqlQuery);
  int  ConvertToNew(QLabel* pQlabelMess  = 0 , Q3ProgressBar* pQProgressBar  = 0 );
  int  ResoudreDoublons(QTreeWidgetItem *pQListViewItem_Src, QTreeWidget *pQListView,                                     int mode /*= CMoteurBase::esclave*/);
  // SIGEMS DEB
  int  Replace_GUID_Patient_By_ID_Sigems_Everywhere(QTreeWidgetItem *pQListViewItem_Src, QString ID_Sigems);
  // SIGEMS FIN

  //..................................................................................................................
long              GetPatientListFromNomPrenomSecuDtNssGeme( QTreeWidget     *pQlistView,
                                                            const QString   &qstr_nom,
                                                            const QString   &qstr_prenom,
                                                            const QString   &qstr_DtNss,
                                                            const QString   &qstr_Secu,
                                                            const QString   &qstr_Geme,
                                                            const QString   &typeAssure,       // 0 c'est l'assure pour le reste c'est les ayant droit
                                                            const QString   &adresse,
                                                            const QString   &codePost,
                                                            const QString   &ville,
                                                            const QString   &sexe,
                                                            const QString   &profession,
                                                            const QString   &nomJfille,
                                                            const QString   &nomAss,
                                                            const QString   &prenomAss,
                                                            const QString   &tel1,
                                                            const QString   &tel2,
                                                            const QString   &tel3,
                                                            const QString   &email,
                                                            const QString   &egal_like    = " = ",
                                                            QLabel          *statutMess   = 0  ,
                                                            QString         *errMess      = 0
                                                           );
  long            GetPatientList(     QTreeWidget *pQlistView,     // pointeur sur une QListView √  3 colonnes
                                  const QString   &qstr_nom,       // nom d'entr√©e
                                  const QString   &qstr_prenom,    // prenom d'entr√©
                                        QLabel    *statutMess=0,   // pointeur sur Qlabel de sortie message
                                        QString   *errMess=0       // pointeur sur Qlabel de sortie message
                                 );
  void            GetPatientNomPrenomPk        ( bool mustBeOpenClose, const QString   &pk,    QString   *nom,    QString   *prenom = 0,   QString  *guid = 0);
  int             GetPatientNomPrenomPkFromGuid( bool mustBeOpenClose, const QString   &guid,  QString   *nom,    QString   *prenom = 0,   QString  *pk   = 0);
  //.................................... identite et Dossiers....................................................................
  QString         DossIdentIndexCreate(const QString &nom, const QString &prenom,    const QString &numGUID, PtrListIdentFieldData *plist_field );
  long            DossIdentDataCreate(QString& refPk, QString& numGUID, PtrListIdentFieldData *plist_field  ,                     int mode/* = CMoteurBase::esclave*/);
  long            DossIdentIndexUpdate(QString& refDossPk, QString& nom, QString& prenom, PtrListIdentFieldData *plist_field  ,   int mode /*= CMoteurBase::esclave*/);
  long            DossIdentDataUpdate(QString& refDossPk,     PtrListIdentFieldData *plist_field ,                                int mode /*= CMoteurBase::esclave*/);
  long            DossIdentBufferSetValue(QSqlRecord *buffer, PtrListIdentFieldData *plist_field);
  void            DossIdentDelete(RUBREC_LIST *pRubList, QString num_GUID, QString id_doss ,                                      int mode /*= CMoteurBase::esclave*/);
  QString         GetDossPrimKeyFromGUID(const QString &guid, int isBaseMusBeClosed = 1 );
  //-------------------------------------- reindexerFichePatient ----------------------------------------------------------------------------
  /*! \brief replace le bon Pk de l'index patient sur tous les enregistrement concernes a partir du GUID patient
   *  \param gui : guid du patient
  *   \return QString pk_du dossier si erreur(pas de dosseir pour ce pk) est vide
  */
  QString reindexerFichePatient( QString guid );
  //................................ param ...........................................................................
  void            Param_SavParam(QString *pParam, QString user);
  void            Param_GetParam(QString *pParam, QString user);
  //..................................................................................................................
  QString         PermsUserCreateOrUpdate(const QString &primKey , const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs);
  int             PermsUserUpdate(const QString &primKey , const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs);
  QString         PermsUserCreate(const QString &droits , const QString &signGUID, const QString &permsGUID,  const QString &blobs);
  int             PermsUserDeleteFromPk( const char *primKey);
  int             PermsUserListDelete( const char *signUserGUID);
  int             PermsUserRefToUserDelete( const char *userGUID);
  long            PermsUserGetList(          Q3ListView *pQlistView,
                                       const QString   &signGUID,
                                       const QString   &imagePath   = "",
                                        QLabel         *statutMess  = 0 ,
                                        QLabel         *errMess     = 0
                             );
  void            PermsUserFatherGetQlistView( Q3ListView* pQlistView, const QString  &friendUser , const QString  &imagePath , QString    *errMess  = 0 );
  //-----------------------------------------------------  SQL_WRITE -------------------------------------------
  /*! \brief Fonction qui permet de mettre a jour ou inserer des donnees dans une table SQL Si l'enregistrement existe deja il est mis a jour sinon il est insere
   *  \param arg_list const &QStringList  : reference sur la liste des arguments de la fonction
   *                       premier  argument : nom de la table a mettre a jour
   *                       deuxieme argument : condition WHERE (la mention WHERE ne doit pas etre fournie) pour selectionner les enregistrements a mettre a jour
   *                       troisieme argument : nom du champ a mettre a jour doit etre suivi du
   *                       quatrieme argument : valeur a mettre a jour dans ce champ.
   *                              .
   *                              .
   *                       nieme argument    : nom du champ a mettre a jour doit etre suivi du
   *                       nieme argument +1 : valeur a placer dans ce champ
   *  \return        une chaine vide si tout est Ok, sinon un message d'erreur.
   */
  QString         SQL_WRITE(const QStringList &arg_list);
  //------------------------------------------ PermsUserChildGetList -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere et renseigne une QTreeWidgetItem avec le nom des utilisateurs delegues d'un utilisateur principal
   *  \param pQlistViewItem    QTreeWidgetItem*  : item dans lequel il faut rajouter les utilisateurs delegues
   *  \param signGUID          const QString& : GUID de l'utilisateur principal dont il faut retrouver les utilisteurs deleges
   *  \return neant
   */
  void            PermsUserChildGetList(         QTreeWidgetItem *pQlistViewItem ,  const QString  &signGUID );
  QString         PermsUserIsThisUserFriendFromName(   const QString  &signUser, const QString  &friendUser);
  QString         PermsUserIsThisUserFriendFromGUID(   const QString  &signGUID, const QString  &friendGUID);
  int             PermsUserSignUserListForThisUser(   const QString  &user, QStringList  &userList,  const char  *pDroit = 0);
  static bool     IsThisDroitExist(const char *listDroits, const char *droitToFind);
  int             TypeDoc_To_PrintTypeDoc(int typeDoc);
  QString         TypeDoc_To_PrintTypeDoc(const QString &typeDoc);

  //------------------------------------------ GetUserNomPrenom -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere le nom et pr√©nom a d'un medecin utilisateur √  partir de son login
   *  \param user     &QString  : reference sur la QString qui contient le nom de l'utilisateur
   *  \param nom      &QString  : reference sur la QString qui contiendra le nom de l'utilisateur
   *  \param prennom   &QString  : reference sur la QString qui contiendra le prenom de l'utilisateur
   *  \return neant;
   */
  void         GetUserNomPrenom( const QString &user, QString &nom, QString &prenom, QString    *errMess  = 0 );
  //------------------------------------------ GetUserNomPrenomFromGUID -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere le nom et prenom a d'un medecin utilisateur a partir de son login
   *  \param guid      &QString  : reference sur la QString qui contient le guid de l'utilisateur
   *  \param nom       *QString  : pointeur sur la QString qui contiendra le nom de l'utilisateur
   *  \param prennom   *QString  : pointeur sur la QString qui contiendra le prenom de l'utilisateur
   *  \return QString login de l'utilisateur
   */
  QString  GetUserNomPrenomFromGUID( const QString &guid, QString *nom = 0, QString *prenom = 0, QString    *errMess  = 0 );

  //------------------------------------------ GetUserSpecialite -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere la specialite d'un medecin utilisateur
   *  \param user     &QString  : reference sur la QString qui contient le nom de l'utilisateur
   *  \param errMess   QString* : (par defaut zero) si renseigne (non √  zero) il y est retourne l'eventuel message d'erreur
   *  \return QString   chaine de caracteres du texte d'une eventuelle erreur (vide sinon).
   */
  QString         GetUserSpecialite( QString &user, QString    *errMess  = 0 );

  //------------------------------------------ GetUserListType -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere et renseigne une QStringList avec le nom des specialites
   *  \param typList    &QStringList  : reference sur la QStringList √  remplir avec le nom des specialites
   *  \param errMess         QString* : (par defaut zero) si renseigne (non √  zero) il y est retourne l'eventuel message d'erreur
   *  \return neant
   */
  void            GetUserListType( QStringList &typList , QString    *errMess  = 0 );

  //------------------------------------------ GetUserList -------------------------------------------------------------------------
  /*! \brief Fonction qui recupere et renseigne une QStringList avec le nom des utilisateurs
   *  \param qstringList    &QStringList  : reference sur la QStringList √  remplir avec le nom des utilisateurs
   *  \param filterOnDroits &QStringList  : reference sur une chaine contenant les droits dont on veut les utilisateurs par defaut '-sgn'
   *  \return QString   retourne une chaine contenant une eventuelle erreur (vide si pas d'erreur)
   */
  QString         GetUserList( QStringList &qstringList , const QString &filterOnDroits="-sgn");
  //------------------------------------------ GetUserList -------------------------------------------------------------------------
  /*! \brief Fonction qui filtre, recupere et renseigne une QTreeWidget avec le nom des utilisateurs filtree par certains crit√®res
   *  \param pQlistView    *QTreeWidget  : pointeur sur la QTreeWidget √  remplir avec le nom des utilisateurs
   *  \param qstr_nom       const QString& : (par defaut vide) caracteres pour filtrer les nom des utilisateurs debutant par ces caracteres
   *  \param qstr_prenom    const QString& : (par defaut vide) caracteres pour filtrer le prenom des utilisateurs debutant par ces caracteres
   *  \param childsMustBeFind    BOOL :  (par defaut TRUE) si TRUE remplir les utilisateurs retrouv√©s avec ses utilisateurs de confiance
   *  \param qstr_UserQualite    const QString& : (par defaut vide) si contient quelque chose alors filtrer sur la Qualite de l'utilsateur (sp√©cialit√©)
   *  \param statutMess      QString* : (par defaut zero) si renseigne (non √  zero) il y est retourne le message de statut
   *  \param errMess         QString* : (par defaut zero) si renseigne (non √  zero) il y est retourne l'eventuel message d'erreur
   *  \return long nombre d'utilisateurs.
   */
  long            GetUserList   (      QTreeWidget *pQlistView,                 // pointeur sur une QTreeWidget
                                  const QString   &qstr_nom         = "",       // filtre sur le nom d'entr√©e
                                  const QString   &qstr_prenom      = "",       // filtre sur le prenom d'entr√©
                                  const bool       childsMustBeFind = TRUE,     // Flag indiquant si l'on doit ausi remplir les utilisateurs delegues
                                  const QString   &qstr_UserQualite = "",       // filtre sur la specialite
                                        QString   *statutMess       = 0,        // pointeur sur le statut de sortie message
                                        QString   *errMess          = 0         // pointeur sur Qlabel de sortie message
                                 );
  //------------------------------------------ GetUserList -------------------------------------------------------------------------
  /*! \brief Fonction qui filtre, recupere et renseigne une QTreeWidget avec le nom des utilisateurs  filtree par certains crit√®res
   *  \param pQlistView    *QTreeWidget  : pointeur sur la QTreeWidget √  remplir avec le nom des utilisateurs
   *  \param listUserAlreadyPresent       const QStringList& :  reference sur une liste d'utilisateurs deja presents √  ne pas retourner
   *  \param specialite    const QString& : (par defaut vide) si contient quelque chose alors filtrer sur la Qualite de l'utilsateur (sp√©cialit√©)
   *  \return QString   chaine de caracteres du texte d'une eventuelle erreur (vide sinon).
   */
  QString         GetUserList(  QTreeWidget *pQlistView, const QStringList &listUserAlreadyPresent, const QString &specialite = "" );

  long            GetUserDocList( Q3ListView   *pQlistView,
                                  const char      *primKey,
                                  const char      *qstr_nom = 0,      /* = 0  */
                                  QLabel    *statutMess     = 0,      /* = 0  */
                                  QLabel    *errMess        = 0       /* = 0  */
                                );
  int             UserDroitsCount(const QString &droitToFind , QString    *errMess  = 0 );
  QString         GetUserPrimKey(  QString user, QString *cripted_password = 0);
  int             SaveUserPassWord(QString user, QString  cripted_password);
  int             VerifyUserPassWord(QString user, QString password);

  //-----------------------------------------------------  GetUserPermisions -------------------------------------------
  /*! \brief Fonction qui recupere la chaine de caracteres des droit utilisateurs
   *  \param user     &QString  : reference sur la QString qui contient le nom de l'utilisateur
   *  \param errMess   QString* : (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
   *  \return          QString qui est la chaine des droits utilisateur (exple '-med-agh-agm-adm' )
   */
  QString         GetUserPermisions(  const QString &user, QString    *errMess  = 0 );

  //-----------------------------------------------------  SetUserPermisions -------------------------------------------
  /*! \brief Fonction qui positionne les droits sur un utilisateur
   *  \param primkey  &QString  : reference sur la QString qui contient la clef primaire de l'identit√© de l'utilisateur
   *  \param droits   &QString  : reference sur la QString qui contient les droits de l'utilisateur
   *  \param errMess   QString* : (par defaut zero) si renseigne (non a zero) il y est retourne l'eventuel message d'erreur
   *  \return          bool  TRUE si tout est ok FALSE dans le cas contraire
   */

  bool            SetUserPermisions( const QString &primkey, const QString &droits, QString *errMess  = 0);
  QString         GetUserDocument(   const char*    user     = "ro",
                                     const char* nameDocType = "Ordo Standard",
                                     int            type     = TYP_ORDONNANCE,
                                     QString*      pFoot     = 0,
                                     int        sub_type     = 0,
                                     QString*  pk_record     = 0
                                 );
  QString         GetUserDocument(QString         dataPk,
                                  int             type       = TYP_ORDONNANCE,    // TYP_ORDONNANCE 2  TYP_CERTIFICAT 3 TYP_OBSERVATION 4
                                  QString*        pFoot      = 0,                 // adresse de la chaine o√π l'on veut recuperer le pied de page
                                  int             sub_type   = 0                  // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                 );
  /*
  long            InitComboWithUserDocList( Q3ComboBox       *pQ3ComboBox,
                                            QString         user,
                                            QString       rubName,
                                            QString         type1 = QString::null,
                                            QString         type2 = QString::null,
                                            int             clear = 1
                                          );
*/
 //------------------------------------------ OutSQL_error -------------------------------------------------------------------------
 /*! \brief Fonction de gestion des sorties des erreurs SQL
   *  \param error      QSqlError  : referrence sur la QSqlError √  exploiter
   *  \param messFunc   const char * : √  zero si non utilise, sinon pointeur sur le texte d'un message additionnel pour presenter l'erreur (exemple la classe et fonction ou est l'erreur)
   *  \param requete    const char *requete :  √  zero si non utilise, sinon pointeur sur le texte de la requete responsable de l'erreur
   *  \return QString   chaine de caracteres du texte complet de l'erreur.
   */
  QString         OutSQL_error(const QSqlError &error, const char *messFunc =0, const char *requete =0);
  //------------------------------------------ OutSQL_error -------------------------------------------------------------------------
  /*! \brief Fonction de gestion des sorties des erreurs SQL
   *  \param query      QSqlQuery  : referrence sur la QSqlQuery √  exploiter
   *  \param messFunc   const char * : √  zero si non utilise, sinon pointeur sur le texte d'un message additionnel pour presenter l'erreur (exemple la classe et fonction ou est l'erreur)
   *  \param requete    const char *requete :  √  zero si non utilise, sinon pointeur sur le texte de la requete responsable de l'erreur
   *  \return QString   chaine de caracteres du texte complet de l'erreur.
   */
  QString         OutSQL_error(const QSqlQuery &query, const char *messFunc=0, const char *requete=0);

  QSqlQuery      *GetIdentiteFromPrimKey(const char* PrimKey );    // identit√© dossier patient
  QSqlQuery      *GetIdentiteFromGUID(const char *guid );          // identit√© dossier patient
  QString         CreateNewUser(const QString &nom, const QString &prenom, const QString &login, const QString &droit="med", const QString &pass="",const QString &nir="");
  QString         GetUserPrimKeyFromNumOrdre( QString nir,  QString droit = QString::null, QStringList *pFiedList=0);
  QString         GetUserPrimKeyFromGUID(const QString &guid);
  QSqlQuery      *GetUserIdentiteFromPrimKey(const char *userPrimKey , QStringList &fiedList);  // identit√© dossier utilisateur
  QString         IsThisUserDocExist( int type, const char *refUserPrimKey, const char *nom=0,   QLabel *errMess = 0 );
  QString         User_GetBestHeadFoot_ToPrintDoc(  RUBREC_LIST::iterator it, const QString &rubName, const char *refUserPrimKey,     QLabel *errMess = 0 );
  QString         GetUserDataFromPrimKey(const char *primKey);
  QString         ReadDrTuxUserParametres(const QString &user);
  int             CreateUserDataFromPrimKey(int type, const char *refUserPrimKey , const char* name,  const char* data);
  int             UpdateUserDataFromPrimKey( const char *primKey, int type, const char *refUserPrimKey , const char* name,  const char* data);
  int             DeleteUserDataFromPrimKey( const char *primKey );
  int             DeleteUserDataFromRefUserPrimKey( const char *refUserPrimKey);
  int             DeleteUserFromPrimKey(const char *refUserPrimKey);
 //............................................ Rubriques...............................................................
  void            Evnmt_InitList(EVNT_LIST *pEvnmtList, QString dossPk);
  int             Evnmt_GetFils(EVNT_LIST *pEvnmtList,  const QString &evnmt_pere);
  void            Evnmt_UpdateToDisk(EVNT_LIST *pEvnmtList);
  int             Evnmt_EraseAllFils(EVNT_LIST *pEvnmtList, const QString &evnmt_pere);
  /*
  QString         Evnmt_GetPkProvisoire(EVNT_LIST *pEvnmtList);
  void            Evnmt_SetDefinitivePk(EVNT_LIST *pEvnmtList, const QString &provPk, const QString &defPk);
  int             Evnmt_SetToDelete_Pk(EVNT_LIST *pEvnmtList,  const QString &pk);
  int             Evnmt_Set_StateOn_AllFils(EVNT_LIST *pEvnmtList, QString pk_Pere, int state);
  */
  long            GetAllRubriqueFromDate(RUBREC_LIST *pRubList, const QDate &qdtRef, QString type="ALL_TYP",  QStringList *pRubNameList =0);
  int             ChangeRubUserName(QString userName, QString rubPrimKey, QString *errMess=0);
  long            initRubriquesList(RUBREC_LIST *pRubList, QString m_NumGUID);
  //long            initConboBoxWithRubList(RUBREC_LIST *pRubList, Q3ComboBox* pComboBox, QString type);
  void            ReplaceDataInRubList(const QString &data,             RUBREC_LIST *pRubList, int id);
  void            ReplaceDataInRubList(const QString & qString,         RUBREC_LIST::iterator it);
  void            ReplaceDataInRubList(const char* text, long len_data, RUBREC_LIST *pRubList, int id);
  void            ReplaceDataInRubList(const char* text, long len_data, RUBREC_LIST::iterator it);
  int             GetDataFromRubList(QByteArray &pQByteArray, RUBREC_LIST *pRubList, int id);
  int             GetDataFromRubList(QByteArray &pQByteArray, RUBREC_LIST::iterator it);
  int             GetDataFromRubList( QString &stringDST, RUBREC_LIST *pRubList, int id);
  QString         GetDataFromRubList( CRubRecord  *pCRubRecord);
  int             GetDataFromRubList( QString &stringDST, CRubRecord  *pCRubRecord);
  void            RubList_SetItemDeleted(RUBREC_LIST *pRubList, int id);
  void            RubList_SetItemNotDeleted(RUBREC_LIST *pRubList, int id);
  int             RubList_IsItemDeleted(RUBREC_LIST *pRubList, int id);

  //..................................................................................................
  QString         GetFieldValue( QString table, QString fieldToGet, QString fieldToMatch, QString valueToMatch,  QVariant *pQVariant=0);
  QString         GetFieldValue_WithoutOpenCloseBase(QString table, QString fieldToGet, QString fieldToMatch,    QString valueToMatch,  QVariant *pQVariant=0);
  //.......................... sauvegarde dossier ....................................................
  long            TesteAndCorrigeDossIndexPkFromHeaders();
  QString         GetPkDoc_Provisoire(RUBREC_LIST *pRubList);
  long            RubNbToSave( RUBREC_LIST *pRubList);
  long            RubListSave( RUBREC_LIST *pRubList, EVNT_LIST *pEvnmtList, QString numGUID, int mode /*= CMoteurBase::esclave*/);
  int             RubListUpdate(RUBREC_LIST::iterator it,                                     int mode /*= CMoteurBase::esclave*/);
  int             RubListDelete(RUBREC_LIST::iterator it,                                     int mode /*= CMoteurBase::esclave*/);
  QString         RubListCreate(RUBREC_LIST::iterator it, QString numGUID,                    int mode /*= CMoteurBase::esclave*/);
  int             RubListBlobUpdate(RUBREC_LIST::iterator it);
  QString         RubListBlobCreate(RUBREC_LIST::iterator it, QString numGUID);
  int             GetDataFromRub(QByteArray &qByteArray, const QString &blobsPrimKey);
  long            GetRecorFromHeadPk(CRubRecord &record, const QString &headPk, const QString &whereCondition = "" );
  /*! \brief Fonction qui retourne un enregistrement de la liste des documents a partir du primary key qui lui est associe
   *  \param pRubList   RUBREC_LIST* :  un pointeur sur la liste des documents
   *  \param pk     QString :  primary key de l'enregistrement a retrouver
   *  \return CRubRecord* ponteur sur l'enregistrement correspondant au primary key fourni en entree ou zero si pas trouve
  */
  CRubRecord*     RubRecordFromPk( RUBREC_LIST *pRubList, const QString &pk);

  //................................................... Verrous .......................................
  int             VerrouilleDossier    ( const QString &dossGUID,
                                         const QString &userName,
                                         QString &usedBy
                                       );
  int             VerrouilleTable(const QString &tableName, QSqlDatabase* dataBase);
  int             DeVerrouilleTable( QSqlDatabase* dataBase);
  int             IsDossierVerrouille (  const QString &dossGUID);
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
  void            DeVerrouilleTout(  );
  //void            setOpenCloseMode(int mode){/*m_CloseAfterQuery=mode; if (m_CloseAfterQuery==0 && m_DataBase &&  m_DataBase->isOpen()==FALSE) m_DataBase->open(); */}
  int             OpenBase();
  void            CloseBase(); //m_CloseAfterQuery

  //............................. faut bien generer les GUID et codage decodage mots de passe .........................................
  static int      PassWordEncode(QString &pass);
  static int      PassWordDecode(const char* str_to_decode, const char* str_pass);
  static  quint16  HexToUINT( const char *str);
  static  QString UINTtoHex( quint16 val);
  QString       GUID_Create();
  void          set_GUID_CreateMode(GUID_Mode mode ){m_GUID_CreateMode=mode;}
  GUID_Mode     get_GUID_CreateMode();
  static int      CalculeDiffAge_Num(QDate *pDebut, QDate *pFin =0, int *mois_ret =0, int *jours_ret =0);
  static QString  CalculeDiffAge(QDate *pDebut, QDate *pFin = NULL, bool bTexte = TRUE , int bAbrege = 1 );
  //...................................... routine utilitaires diverses ..........................................
  static char    *GotoNextDebLigne(const char* txt);
  static char    *GotoEndOfLigne(const char* txt);
  static char    *GotoNextSignifiant(const char* txt);
  static char    *JumpObject(char *pt, char *end_rtf);
  static QString  RTF_ToASCII(const char *ptIn, long len);
  static int      IsRtfControlCar(char *pt);
  static QString  ComputeClef_Secu(const char *txt);
    //............................... DATA .............................................................................
 public:

  //......................... parametres de connexion ...............................................................
  int           m_IsValid;
  //QSqlDatabase* m_DataBase;
  QString       m_DriverName;
  QString       m_BaseName;
  QString       m_UserName;
  QString       m_PassWord;
  QString       m_HostName;
  QString       m_Port;
  QTimer       *m_pReconnectTimer;

private:
   QString      m_BaseLabel;
   QString      m_DestBaseLabel;
   int          m_XmlDepth;
   int          m_ConvertRun;
   GUID_Mode    m_GUID_CreateMode;
//................................. SLOTS SIGNALS ............................................
public slots:
  void On_ReconnectTimerDone();
signals:
  void Sign_ReconnectTimerDone();

};


#endif

