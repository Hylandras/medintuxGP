#ifndef C_BDM_API_H
#define C_BDM_API_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include <QDebug>
#include <QPixmap>

#include "../C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_ApiVar.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PatientCtx.h"    // utile pour C_LifeEvent
#include "../../MedinTuxTools-QT4/medicabase/C_UserCtx.h"

#include "C_PosologieGrammar.h"
#include "C_InterResultPair.h"

#define getListMedicaments selectProductsList
#define CIP_to_ATC ATC_from_CIP

#define INDIC_COL_ICONE    0
#define INDIC_COL_LIBELLE  0
#define INDIC_COL_SMR      1
#define INDIC_COL_DATE     2
#define INDIC_COL_COMMENT  3
#define INDIC_COL_CODE     4
#define INDIC_COL_TYPE     5
#define INDIC_COL_REFUNDED 6
#define INDIC_COL_CIP      7

class QTreeWidget;
class QTextEdit;
class QTreeWidgetItem;
class QTimer;
class QSqlQuery;
class C_PatientCtx;

typedef struct  { int     m_ge;
                  int     m_cip;
                  int     m_cip2;
                  int     m_id_1;
                  int     m_id_2;
                  QString m_drugName;
                  QString m_drugName_2;

                  QString   m_level;
                  QString   m_product;
                  QString   m_libelle;
                  QString   m_libelle_2;
                  QString   m_document;
                  QString   m_reference;
                  QDateTime m_date;
                } INTER_STRUCT;
//====================================== C_BiblioData =====================================
class C_BiblioData
{
 public:
    C_BiblioData();
    explicit    C_BiblioData(const QString pk,
                             const QString owner,
                             const QString lang,
                             const QString url,
                             const QString id,
                             const QString date,
                             const QString note,
                             const QString type,
                             const QString mime,
                             const QString blob_pk,
                             const QString data
                             );
     QString m_pk;
     QString m_id;
     QString m_owner;
     QString m_lang;
     QString m_url;
     QString m_date;
     QString m_note;
     QString m_type;
     QString m_mime;
     QString m_blob_pk;
     QString m_data;
};

//=================== C_CompositionItem ===================================
/*! \class C_CompositionItem
 *  \brief Class to maintain a dci composition definition item dci name, amount, unit
 */
class  C_CompositionItem
{ public:
    explicit C_CompositionItem(const QString &name        ,
                               const QString &amount      ,
                               const QString &unit        ,
                               const QString &unit_plural
                              )
    { m_name         = name;
      m_amount       = amount;
      m_unit         = unit;
      m_unit_plural  = unit_plural;
    }
    C_CompositionItem(){}
public:
   QString  name()         const {return m_name;}
   QString  amount()       const {return m_amount;}
   QString  unit()         const {return m_unit;}
   QString  unit_plural()  const {return m_unit_plural;}
private:
   QString m_name;
   QString m_amount;
   QString m_unit;
   QString m_unit_plural;
};
//=================== C_ExcipientItem ===================================
/*! \class C_ExcipientItem
 *  \brief Class to maintain a excipient composition definition
 */

class  C_ExcipientItem
{ public:
    explicit C_ExcipientItem(  const QString &name          ,
                               const QString &amount      ,
                               const QString &unit        ,
                               const QString &code        ,
                               const QString &code_typ
                              )
    { m_name         = name;
      m_amount       = amount;
      m_unit         = unit;
      m_code         = code;
      m_code_typ     = code_typ;
    }
    C_ExcipientItem(){}
public:
   QString  name()         const {return m_name;}
   QString  amount()       const {return m_amount;}
   QString  unit()         const {return m_unit;}
   QString  code()         const {return m_code;}
   QString  code_typ()     const {return m_code_typ;}
private:
   QString m_name;
   QString m_amount;
   QString m_unit;
   QString m_code;
   QString m_code_typ;
};
//=================== C_PrescriptionRestreinte ===================================
/*! \class C_PrescriptionRestreinte
 *  \brief Class to maintain restrictives prescriptions informations
 */

class  C_PrescriptionRestreinte
{ public:
    explicit C_PrescriptionRestreinte(  const QString &cip                 = "",
                                        const QString &drug_name           = "",
                                        const QString &registre            = "",
                                        const QString &preconisations      = "",
                                        const QString &duree_max           = "",
                                        const QString &fractionnement      = "",
                                        const QString &site_renouvellement = "",
                                        const QString &site_dispensation   = "",
                                        const QString &liste               = "",
                                        const QString &isStupefiant        = "",
                                        const QString &prescr_initiale     = "",
                                        const QString &dispen_initiale     = "",
                                        const QString &portee              = "",  // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
                                        const QString &exception           = "",  // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
                                        const QString &gestion_ordonnance  = "",  // Ordonnance sécurisée ET/OU Renouvellement interdit sur ordonnance ET/OU Chevauchement interdit de la délivrance prescription)
                                        const QStringList &prescipteurs    = QStringList(),
                                        bool           isValid             = false
                                     )
    { m_cip                 = cip;
      m_drug_name           = drug_name;
      m_registre            = registre;
      m_preconisations      = preconisations;
      m_duree_max           = duree_max;
      m_fractionnement      = fractionnement;
      m_site_renouvellement = site_renouvellement;
      m_site_dispensation   = site_dispensation;
      m_liste               = liste;
      m_isValid             = isValid;
      m_isStupefiant        = isStupefiant;
      m_prescr_initiale     = prescr_initiale;
      m_dispen_initiale     = dispen_initiale;
      m_portee              = portee;              // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
      m_exception           = exception;           // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
      m_gestion_ordonnance  = gestion_ordonnance;  // Ordonnance sécurisée ET/OU Renouvellement interdit sur ordonnance ET/OU Chevauchement interdit de la délivrance prescription)
      m_prescripteurs       = prescipteurs;
    }
    C_PrescriptionRestreinte(){}
public:
   QString  cip()                   const {return m_cip;}
   QString  drug_name()             const {return m_drug_name;}
   QString  registre()              const {return m_registre;}
   QString  preconisations()        const {return m_preconisations;}
   QString  duree_max()             const {return m_duree_max;}
   QString  fractionnement()        const {return m_fractionnement;}
   QString  site_renouvellement()   const {return m_site_renouvellement;}
   QString  site_dispensation()     const {return m_site_dispensation;}
   QString  liste()                 const {return m_liste;}
   QString  isStupefiant()          const {return m_isStupefiant;}
   QString  prescr_initiale()       const {return m_prescr_initiale;}
   QString  dispen_initiale()       const {return m_dispen_initiale;}
   QString  portee()                const {return m_portee;}  // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
   QString  exception()             const {return m_exception;}  // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
   QString  gestion_ordonnance()    const {return m_gestion_ordonnance;}  // Ordonnance sécurisée ET/OU Renouvellement interdit sur ordonnance ET/OU Chevauchement interdit de la délivrance prescription)
   QStringList  prescripteurs()     const {return m_prescripteurs;}
   bool     isValid()               const {return m_isValid;}

   QString toString()
           { QString message                   ="";
             QString str  = registre();
             if ( str.length() ) message      += QObject::tr("<b>Registre prescriptif</b> : %1<br />").arg(str);
             if ( prescripteurs().size() )
                { message      += QObject::tr("<b>Prescripteurs autoris\303\251s</b> : <br />");
                  for ( int i=0; i<prescripteurs().size(); ++i)
                      {  message      += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + prescripteurs()[i] + "<br />";
                      }
                }
             str          = preconisations();
             if ( str.length() ) message      += QObject::tr("<b>Conduite pour le suivi</b> : <br />%1<br />").arg(str);
             str          = duree_max();
             if ( str.length() ) message      += QObject::tr("<b>Dur\303\251e maximale</b> : %1 jours<br />").arg(str);
             str          = fractionnement();
             if ( str.length() ) message      += QObject::tr("<b>Fractionnement</b> : %1 jours<br />").arg(str);
             str          = site_renouvellement();
             if ( str.length() ) message      += QObject::tr("<b>Site de prescription</b> : %1<br />").arg( str );
             str          = site_dispensation();
             if ( str.length() ) message      += QObject::tr("<b>Site de dispensation</b> : %1<br />").arg( str );
             str          = dispen_initiale();
             if ( str.length() ) message      += QObject::tr("<b>Site de la prescription initiale</b> : %1<br />").arg( str );
             str          = dispen_initiale();
             if ( str.length() ) message      += QObject::tr("<b>Site de la dispensation initiale</b> : %1<br />").arg( str );
             str          = portee();
             if ( str.length() ) message      += QObject::tr("<b>Renouvellement obligatoire sur site initial au bout de </b> : %1 jours<br />").arg( str );
             str          = gestion_ordonnance();
             if ( str.length() ) message      += QObject::tr("<b>Ordonance</b> : %1<br />").arg( str );
             return message;
           }

private:
   QString m_cip;
   QString m_drug_name;
   QString m_registre;
   QString m_preconisations;
   QString m_duree_max;
   QString m_fractionnement;
   QString m_site_renouvellement;
   QString m_site_dispensation;
   QString m_liste;
   QString m_isStupefiant;
   QString m_prescr_initiale;
   QString m_dispen_initiale;
   QString m_portee;              // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
   QString m_exception;           // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
   QString m_gestion_ordonnance;  // Ordonnance sécurisée ET/OU Renouvellement interdit sur ordonnance ET/OU Chevauchement interdit de la délivrance prescription)
   QStringList m_prescripteurs;
   bool    m_isValid;

};

//=================== C_BDM_IndicationRecord ===================================
/*! \class C_BDM_IndicationRecord
 *  \brief Class to indications datas access for a drug .
 */

class  C_BDM_IndicationRecord
{
public:
  explicit C_BDM_IndicationRecord(const QString &cip,
                                  const QString &date,
                                  const QString &level,
                                  const QString &levelTyp,
                                  const QString &levelLibelle,
                                  const QString &indication,
                                  const QString &descriptif,
                                  const QString &code,
                                  const QString &codeTyp,
                                  const QString &refundLink = ""
                                 )
  {m_cip           = cip;
   m_date          = date;
   m_level         = level;
   m_levelTyp      = levelTyp;
   m_levelLibelle  = levelLibelle;
   m_indication    = indication;
   m_descriptif    = descriptif;
   m_code          = code;
   m_codeTyp       = codeTyp;
   m_refundLink    = refundLink;
  }
  C_BDM_IndicationRecord(){}
  QString cip()          const {return m_cip;}
  QString date()         const {return m_date;}
  QString level()        const {return m_level;}
  QString levelTyp()     const {return m_levelTyp;}
  QString levelLibelle() const {return m_levelLibelle;}
  QString indication()   const {return m_indication;}
  QString descriptif()   const {return m_descriptif;}
  QString code()         const {return m_code;}
  QString codeTyp()      const {return m_codeTyp;}
  QString refundLink()   const {return m_refundLink;}

  void set_cip(          const QString &value)           {m_cip          = value;}
  void set_date(         const QString &value)           {m_date         = value;}
  void set_level(        const QString &value)           {m_level        = value;}
  void set_levelTyp(     const QString &value)           {m_levelTyp     = value;}
  void set_levelLibelle( const QString &value)           {m_levelLibelle = value;}
  void set_indication(   const QString &value)           {m_indication   = value;}
  void set_descriptif(   const QString &value)           {m_descriptif   = value;}
  void set_code(         const QString &value)           {m_code         = value;}
  void set_codeTyp(      const QString &value)           {m_codeTyp      = value;}
  void set_refundLink(   const QString &value)           {m_refundLink   = value;}

private:
  QString m_cip;
  QString m_date;
  QString m_level;
  QString m_levelTyp;
  QString m_levelLibelle;
  QString m_indication;
  QString m_descriptif;
  QString m_code;
  QString m_codeTyp;
  QString m_refundLink;
};

//=================== C_BDM_DrugListRecord ===================================
/*! \class C_BDM_DrugListRecord
 *  \brief Class to datas access for a drug line record in drugs list.
 */

class C_BDM_DrugListRecord
{
public:
    explicit C_BDM_DrugListRecord(){}

    C_BDM_DrugListRecord(const QString xmlData)
    {unSerialize(xmlData,this);
    }

    C_BDM_DrugListRecord(QString id,
                         QString id_type,
                         QString commercialName,
                         QString dci_1Name,
                         QString dc2_1Name,
                         QString dc3_1Name,
                         QString owner,
                         QString lang,
                         QString nb_up,
                         QString ucd_price,
                         QString atc,
                         QString generique,
                         QString statut
                        )
     {m_id                 = id;
      m_id_type            = id_type;
      m_commercialName     = commercialName;
      m_dci_1Name          = dci_1Name;
      m_dci_2Name          = dc2_1Name;
      m_dci_3Name          = dc3_1Name;
      m_owner              = owner;
      m_lang               = lang;
      m_nb_up              = nb_up;
      m_ucd_price          = ucd_price;
      m_atc                = atc;
      m_generique          = generique;
      m_statut             = statut;
     }

     QString id()                 const {return m_id;}
     QString id_type()            const {return m_id_type;}
     QString commercialName()     const {return m_commercialName;}
     QString dci_1Name()          const {return m_dci_1Name;}
     QString dci_2Name()          const {return m_dci_2Name;}
     QString dci_3Name()          const {return m_dci_3Name;}
     QString owner()              const {return m_owner;}
     QString lang()               const {return m_lang;}
     QString nb_up()              const {return m_nb_up;}
     QString ucd_price()          const {return m_ucd_price;}
     QString atc()                const {return m_atc;}
     QString generique()          const {return m_generique;}
     QString statut()             const {return m_statut;}

     bool can_use_API()           const {return (m_owner!="DS_A" && m_id=="CIP");}

     void set_id(const QString value)                  {m_id                  = value;}
     void set_id_type(const QString value)             {m_id_type             = value;}
     void set_commercialName(const QString value)      {m_commercialName      = value;}
     void set_dci_1Name(const QString value)           {m_dci_1Name           = value;}
     void set_dci_2Name(const QString value)           {m_dci_2Name           = value;}
     void set_dci_3Name(const QString value)           {m_dci_3Name           = value;}
     void set_owner(const QString value)               {m_owner               = value;}
     void set_lang(const QString value)                {m_lang                = value;}
     void set_nb_up(const QString value)               {m_nb_up               = value;}
     void set_ucd_price(const QString value)           {m_ucd_price           = value;}
     void set_atc(const QString value)                 {m_atc                 = value;}
     void set_generique(const QString value)           {m_generique           = value;}
     void set_statut(const QString value)              {m_statut              = value;}


     QString                       serialize(int with_Header = 1 , const QString &ofset  ="" ) const;
     static QString                serialize(const C_BDM_DrugListRecord *pC_BDM_DrugListRecord, int with_Header = 1 , const QString &ofset ="" , int htmlConvert = CGestIni::SupInfAnd);
     static C_BDM_DrugListRecord*  unSerialize(const QString & data, C_BDM_DrugListRecord *obj);
     static C_BDM_DrugListRecord   unSerialize(const QString & data);

protected :
        QString m_id;
        QString m_id_type;
        QString m_commercialName;
        QString m_dci_1Name;
        QString m_dci_2Name;
        QString m_dci_3Name;
        QString m_owner;
        QString m_lang;
        QString m_nb_up;
        QString m_ucd_price;
        QString m_atc;
        QString m_generique;
        QString m_statut;
};

//=================== C_OrdoLineRecord ===================================
class C_OrdoLineRecord : public C_BDM_DrugListRecord
{
public:
    explicit C_OrdoLineRecord(){}

    C_OrdoLineRecord(const QString xmlData)
    {unSerialize(xmlData,this);
    }

    C_OrdoLineRecord( const C_BDM_DrugListRecord &bdm_DrugListRecord)
        : C_BDM_DrugListRecord( bdm_DrugListRecord.id(),
                                bdm_DrugListRecord.id_type(),
                                bdm_DrugListRecord.commercialName(),
                                bdm_DrugListRecord.dci_1Name(),
                                bdm_DrugListRecord.dci_2Name(),
                                bdm_DrugListRecord.dci_3Name(),
                                bdm_DrugListRecord.owner(),
                                bdm_DrugListRecord.lang(),
                                bdm_DrugListRecord.nb_up(),
                                bdm_DrugListRecord.ucd_price(),
                                bdm_DrugListRecord.atc(),
                                bdm_DrugListRecord.generique(),
                                bdm_DrugListRecord.statut()
                              )
    {   m_ordo_key               =   "";
        m_numOrdre               =   "";
        m_secureState            =   "";
        m_date_debut             =   QDateTime::currentDateTime();
        m_date_redac             =   QDateTime::currentDateTime();
        m_date_fin               =   QDateTime::currentDateTime();
        m_pk_patient             =   "";
        m_guid_patient           =   "";
        m_code_indication        =   "";
        m_type_code_indication   =   "";
        m_smr                    =   "";
        m_smr_indication         =   "";
        m_voie_admin             =   "";
        m_ald                    =   "";
        m_substituable           =   "";
        m_renouvelable           =   "";
        m_quantite_totale        =   "";
        m_forme_galenique        =   "";
        m_unite_de_prise         =   "";
        m_quantite_par_up        =   "";
        m_unite_quantite         =   "";
        m_note_textuelle_libre   =   "";
        m_schema_posologique     =   "";
    }

    C_OrdoLineRecord( const C_OrdoLineRecord &ordoLineRecord)
        : C_BDM_DrugListRecord( ordoLineRecord.id(),
                                ordoLineRecord.id_type(),
                                ordoLineRecord.commercialName(),
                                ordoLineRecord.dci_1Name(),
                                ordoLineRecord.dci_2Name(),
                                ordoLineRecord.dci_3Name(),
                                ordoLineRecord.owner(),
                                ordoLineRecord.lang(),
                                ordoLineRecord.nb_up(),
                                ordoLineRecord.ucd_price(),
                                ordoLineRecord.atc(),
                                ordoLineRecord.generique(),
                                ordoLineRecord.statut()
                              )
    {   m_ordo_key               =   ordoLineRecord.ordo_key();
        m_numOrdre               =   ordoLineRecord.numOrdre();
        m_secureState            =   ordoLineRecord.secureState();
        m_date_debut             =   ordoLineRecord.date_debut();
        m_date_redac             =   ordoLineRecord.date_redac();
        m_date_fin               =   ordoLineRecord.date_fin();
        m_pk_patient             =   ordoLineRecord.pk_patient();
        m_guid_patient           =   ordoLineRecord.guid_patient();
        m_code_indication        =   ordoLineRecord.code_indication();
        m_type_code_indication   =   ordoLineRecord.type_code_indication();
        m_smr                    =   ordoLineRecord.smr();
        m_smr_indication         =   ordoLineRecord.smr_indication();
        m_voie_admin             =   ordoLineRecord.voie_admin();
        m_ald                    =   ordoLineRecord.ald();
        m_substituable           =   ordoLineRecord.substituable();
        m_renouvelable           =   ordoLineRecord.renouvelable();
        m_quantite_totale        =   ordoLineRecord.quantite_totale();
        m_forme_galenique        =   ordoLineRecord.forme_galenique();
        m_unite_de_prise         =   ordoLineRecord.unite_de_prise();
        m_quantite_par_up        =   ordoLineRecord.quantite_par_up();
        m_unite_quantite         =   ordoLineRecord.unite_quantite();
        m_note_textuelle_libre   =   ordoLineRecord.note_textuelle_libre();
        m_schema_posologique     =   ordoLineRecord.schema_posologique();
    }

    C_OrdoLineRecord    (QString   id,
                         QString   id_type,
                         QString   commercialName,
                         QString   dci_1Name,
                         QString   dci_2Name,
                         QString   dci_3Name,
                         QString   owner,
                         QString   lang,
                         QString   ucd,
                         QString   ucd_price,
                         QString   atc,
                         QString   generique,
                         QString   statut,

                         QString   ordo_key,
                         QString   numOrdre,
                         QString   secureState,
                         QDateTime date_debut,
                         QDateTime date_redac,
                         QDateTime date_fin,
                         QString   pk_patient,
                         QString   guid_patient,
                         QString   code_indication,
                         QString   type_code_indication,
                         QString   smr,
                         QString   smr_indication,
                         QString   voie_admin,
                         QString   ald,
                         QString   substituable,
                         QString   renouvelable,
                         QString   quantite_totale,
                         QString   forme_galenique,
                         QString   unite_de_prise,
                         QString   quantite_par_up,
                         QString   unite_quantite,
                         QString   note_textuelle_libre,
                         QString   schema_posologique
                        )
                            : C_BDM_DrugListRecord( id,
                                                    id_type,
                                                    commercialName,
                                                    dci_1Name,
                                                    dci_2Name,
                                                    dci_3Name,
                                                    owner,
                                                    lang,
                                                    ucd,
                                                    ucd_price,
                                                    atc,
                                                    generique,
                                                    statut)
    {m_ordo_key               =   ordo_key;
     m_numOrdre               =   numOrdre;
     m_secureState            =   secureState;
     m_date_debut             =   date_debut;
     m_date_redac             =   date_redac;
     m_date_fin               =   date_fin;
     m_pk_patient             =   pk_patient;
     m_guid_patient           =   guid_patient;
     m_code_indication        =   code_indication;
     m_type_code_indication   =   type_code_indication;
     m_smr                    =   smr;
     m_smr_indication         =   smr_indication;
     m_voie_admin             =   voie_admin;
     m_ald                    =   ald;
     m_substituable           =   substituable;
     m_renouvelable           =   renouvelable;
     m_quantite_totale        =   quantite_totale;
     m_forme_galenique        =   forme_galenique;
     m_unite_de_prise         =   unite_de_prise;
     m_quantite_par_up        =   quantite_par_up;
     m_unite_quantite         =   unite_quantite;
     m_note_textuelle_libre   =   note_textuelle_libre;
     m_schema_posologique     =   schema_posologique;
    }

    //.......... methodes ..............
     void   set_ordo_key(const QString &value)                  {  m_ordo_key    = value;}
     void   set_numOrdre(const QString &value)                  {  m_numOrdre    = value;}
     void   set_secureState(const QString &value)               {  m_secureState = value;}
     void   set_date_debut(const QDateTime &value)              {  m_date_debut  = value;}
     void   set_date_redac(const QDateTime &value)              {  m_date_redac  = value;}
     void   set_date_fin(  const QDateTime &value)              {  m_date_fin    = value;}
     void   set_pk_patient(const QString &value)                {  m_pk_patient  = value;}
     void   set_guid_patient(const QString &value)              {  m_guid_patient  = value;}
     void   set_code_indication(const QString &value)           {  m_code_indication  = value;}
     void   set_type_code_indication(const QString &value)      {  m_type_code_indication  = value;}
     void   set_smr(const QString &value)                       {  m_smr  = value;}
     void   set_smr_indication(const QString &value)            {  m_smr_indication  = value;}
     void   set_voie_admin(const QString &value)                {  m_voie_admin  = value;}
     void   set_ald(const QString &value)                       {  m_ald  = value;}
     void   set_substituable(const QString &value)              {  m_substituable  = value;}
     void   set_renouvelable(const QString &value)              {  m_renouvelable  = value;}
     void   set_quantite_totale(const QString &value)           {  m_quantite_totale  = value;}
     void   set_forme_galenique(const QString &value)           {  m_forme_galenique  = value;}
     void   set_unite_de_prise(const QString &value)            {  m_unite_de_prise  = value;}
     void   set_quantite_par_up(const QString &value)           {  m_quantite_par_up  = value;}
     void   set_unite_quantite(const QString &value)            {  m_unite_quantite  = value;}
     void   set_note_textuelle_libre(const QString &value)      {  m_note_textuelle_libre  = value;}
     void   set_schema_posologique(const QString &value)        {  m_schema_posologique  = value;}
     //.......... methodes acces ..............
     QString   ordo_key()            const    { return m_ordo_key;}
     QString   numOrdre()            const    { return m_numOrdre;}
     QString   secureState()         const    { return m_secureState;}
     QDateTime date_debut()          const    { return m_date_debut;}
     QDateTime date_redac()          const    { return m_date_redac;}
     QDateTime date_fin()            const    { return m_date_fin;}
     QString pk_patient()            const    { return m_pk_patient;}
     QString guid_patient()          const    { return m_guid_patient;}
     QString code_indication()       const    { return m_code_indication;}
     QString type_code_indication()  const    { return m_type_code_indication;}
     QString smr()                   const    { return m_smr;}
     QString smr_indication()        const    { return m_smr_indication;}
     QString voie_admin()            const    { return m_voie_admin;}
     QString ald()                   const    { return m_ald;}
     QString substituable()          const    { return m_substituable;}
     QString renouvelable()          const    { return m_renouvelable;}
     QString quantite_totale()       const    { return m_quantite_totale;}
     QString forme_galenique()       const    { return m_forme_galenique;}
     QString unite_de_prise()        const    { return m_unite_de_prise;}
     QString quantite_par_up()       const    { return m_quantite_par_up;}
     QString unite_quantite()        const    { return m_unite_quantite;}
     QString note_textuelle_libre()  const    { return m_note_textuelle_libre;}
     QString schema_posologique()    const    { return m_schema_posologique;}

     QString                  serialize() const;
     static QString           serialize(const C_OrdoLineRecord *pC_OrdoLineRecord);
     static C_OrdoLineRecord* unSerialize(const QString & data, C_OrdoLineRecord *pC_OrdoLineRecord);
     static C_OrdoLineRecord  unSerialize(const QString & data);

 private:
     QString   m_ordo_key;        // cette valeur est la clef (exemple: QDateTime de redaction) d'appartenance a une ordonnance composee de plusieurs lignes de C_OrdoLineRecord
     QString   m_numOrdre;        // eventuel numero d'ordre du produit dans l'ordonnance a laquelle il appartient
     QString   m_secureState;     // etat des alertes signalees lors de l'enregistrement (permet de retrouver rappidement les prescriptions potentiellement a problemes)
     QDateTime m_date_debut;
     QDateTime m_date_redac;
     QDateTime m_date_fin;
     QString   m_pk_patient;
     QString   m_guid_patient;
     QString   m_code_indication;          // code : cim10,Cisp, separes par des epsace si plusieurs codes) etc...
     QString   m_type_code_indication;     // origine du code
     QString   m_smr;                      // SMR lie a cette prescription
     QString   m_smr_indication;           // si selon indication (alors
     QString   m_voie_admin;
     QString   m_ald;
     QString   m_substituable;
     QString   m_renouvelable;
     QString   m_quantite_totale;
     QString   m_forme_galenique;
     QString   m_unite_de_prise;
     QString   m_quantite_par_up;
     QString   m_unite_quantite;
     QString   m_note_textuelle_libre;
     QString   m_schema_posologique;
};


//======================================================= C_BDM_PluginI ===================================
/*! \class C_BDM_PluginI
 *  \brief pure virtual Interface Class to provide data to C_BDM_Api database, each data commercial sources must provide
 * a plugin derived from this modele.
 */

class C_BDM_PluginI : C_BDM_ApiVar
{

public:
    enum flags  {no_filter   = 0,
                 atc_filter  = 1,
                 nom_filter  = 2,
                 dci_filter  = 4,
                 ucd_filter  = 8,
                 dc3_filter  = 16,
                 dca_filter  = 20,
                 cip_filter  = 32,
                 gen_filter  = 64,
                 vil_filter  = 128,
                 hop_filter  = 256,
                 cip_list    = 512,
                 all_filter  = 511     // ne pas inclure dans all_filter le filtre cip_list (qui est un filtrage sur liste CIP exclusivement)
                };
    enum hit_flag  {ONLY_IF_NOT_EXISTS   = 0,
                    EVEN_IF_EXISTS       = 1,
                    INCREASE_HIT         = 2,
                    FIX_HIT              = 3
                   };
    enum indic_flag  { ASMR     = 1,       // we want indications with ASMR level (if exists)
                       SMR      = 2,       // we want indications with SMR level (if exists)
                       CIM      = 4,       // we want coded CIMX indications
                       BDM      = 8,       // we want specifics database coded indications
                       ALL_TYPE = 12,      // we want all coded indications without SMR ASM
                       ALL_INDIC= 15,
                       REFUND   = 16       // we check if indications are linked to refund ( m_refundLink will be whith info to this property)
                     };
    explicit C_BDM_PluginI();
    // IMPORTANT de mettre le destructeur en virtuel pur et de l'implementer dans le CPP sinon explose lors du delete
    virtual ~C_BDM_PluginI()=0;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //------------------ INTERFACE VIRTUELLE ------------------------------------------------------------------------
    //                   chaque implementation commerciale devra implementer les methodes virtuelles pures de cette partie
    virtual int                   init(const QString &confDatas, const QString &prefixName)=0;
    virtual void                  setLogWidget      ( QTextEdit    *logWidget)=0;
    virtual void                  setProgressWidget ( QProgressBar *pQProgressBar)=0;  //progressWidget
    virtual QProgressBar *        progressWidget()=0;                                  //progressWidget
    virtual QString               dataSourceVersion()=0;
    virtual QString               lang()=0;
    virtual QString               owner()=0;
    virtual QString               getLastError()=0;
    //-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
    /*! \brief return if  plugin is HAS certified
     *  \return return HAS certified status
     */
    virtual bool is_plugin_HAS_certified()=0;

    //-------------------------------- plugin_name ------------------------------------------------------
    /*! \brief return active plugin name
     *  \return return active plugin name
     */
    virtual QString plugin_name()=0;
    //-------------------------------- plugin_icon ------------------------------------------------------
    /*! \brief return active plugin_icon
     *  \return return active plugin_icon
     */
    virtual QPixmap plugin_icon()=0;
    //---------------------------------------- get_database -------------------------------------------------------
    /*! \brief  return active database. This facility is to do extended SQL querys which are not in API
     *  \return  database
     */
    virtual QSqlDatabase get_database()=0;

    //-------------------------------- plugin_image_source ------------------------------------------------------
    /*! \brief  return image source url for monographies
     *  \return return image source url for monographies
     */
    virtual QString plugin_image_source()=0;

    //-------------------------------- getCompositionFromCIP ------------------------------------------------------
    /*! \brief return drug composition.
     *  \param const QString& cip drug cip identification .
     *  \return QList<C_CompositionItem> result list where
     *              C_CompositionItem.name()              is the composant name (DCI)
     *              C_CompositionItem.amount()            is the quantity
     *              C_CompositionItem.unit()              is the quantity unit
     *              C_CompositionItem.unit_plural()       is the accorded quantity unit if more than one
     */
    virtual QList<C_CompositionItem> getCompositionFromCIP(const QString &cip)=0;
    //-------------------------------- getExcipientsFromCIP ------------------------------------------------------
    /*! \brief return drug excipients list .
     *  \param const QString& cip drug cip identification .
     *  \return QList<C_ExcipientItem> result list where
     *              C_ExcipientItem.name()              is the composant name (excipient)
     *              C_ExcipientItem.amount()            is the quantity if present (not regular)
     *              C_ExcipientItem.unit()              is the quantity unit if present (not regular)
     *              C_ExcipientItem.code()              is the substance code  (specific at BDM implementation)
     *              C_ExcipientItem.code_typ            is the code type (specific at BDM implementation)
     */
    virtual QList<C_ExcipientItem> getExcipientsFromCIP(const QString &cip)=0;
    //-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
    /*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
     *         if there is not restrictive prescription for this CIP  isValid() is false;
     *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
     *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
     *  \return QList<C_PrescriptionRestreinte>
     */
    virtual QList<C_PrescriptionRestreinte> getPrescriptionRestreinteFromCIP(const QString &cip)=0;
    //-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
    /*! \brief return drug composition.
     *  \param const QString& cip drug cip identification .
     *  \return QStringList result list with dci composants
     */
    virtual QStringList get_DCI_list_FromCIP(const QString &cip)=0;

    //--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
    /*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
     *  \param const QString cip is drug cip code.
     *  \param QString *rcp is pointer on QString to return RCP result if not zero .
     *  \return CIS code string
    */
    virtual  QString              get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp =0 )=0;
    //.................. classification ATC ....................
    virtual int                   nextATC_Code(QString &codeAtc, QString &libelle)=0;

    //.................. liste des medicaments ................
    virtual long                  drugsList_Count()=0;
    virtual int                   getDrugList_Start()=0;
    virtual int                   getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord)=0;
    //-------------------------------- getInteractionPhysicoChimiques ------------------------------------------------------
    /*! \brief return chimico physiques interactions in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    virtual QList<C_InterResultPair> getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)=0;
    //-------------------------------- getAllergiesFromCipList ------------------------------------------------------
    /*! \brief return Allergies interactions in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    virtual QList<C_InterResultPair> getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)=0;

    //-------------------------------- getInteractionRedundancy ------------------------------------------------------
    /*! \brief return redundancy composition in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    virtual QList<C_InterResultPair> getInteractionRedundancy( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)=0;
    //------------------------------ getRefundedIndications ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
     *  \param const QString &code_type  code typ as giving by the drugs BDM
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
    virtual QList <C_BDM_IndicationRecord> getRefundedIndications( const QString &code_indication,   const QString &code_type )=0;
    //------------------------------ getIndicationsFromText ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &text      text from wich we want indications datas.
     *  \param const QString &filter   filter wanted datas  a combination | of this flags
     *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
     *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
     *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
     *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
     *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
     *                                  C_BDM_PluginI::REFUND      we check if indications are linked to refund ( m_refundLink will contains price)
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
    virtual QList <C_BDM_IndicationRecord> getIndicationsFromText( const QString &text,   int filter )=0;
    //------------------------------ getIndicationsFromCIP ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \param const QString &filter   filter wanted datas  a combination | of this flags
     *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
     *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
     *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
     *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
     *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
     virtual QList <C_BDM_IndicationRecord> getIndicationsFromCIP( const QString &cip,   int filter )=0;
     //------------------------------ getCIPFromIndication ------------------------------------------
     /*! \brief select indications informations for a drug.
      *  \param const QString &code       indication code of the drugs from wich we want datas.
      *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
      *  \return QStringList cip result list
      */
     virtual QStringList getCIPFromIndication( const QString &code, const QString &codeType )=0;
     //-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
     /*! \brief test compatibility with maximum and minimun authorized dose from a drug and patient context.
      *  \param C_PatientCtx *pC_PatientCtx
      *  \param const QString &grammar prescription for this drug
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     virtual QList<C_InterResultPair>  getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)=0;
     //-------------------------------- getMonographieFromCIP ------------------------------------------------------
     /*! \brief select monographie for a drug.
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString contains monographie
      */
     virtual QString  getMonographieFromCIP ( const QString &cip ,   bool dataMustBeReadInCache  = true )=0;
     //-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
     /*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres a cafe etc...)
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString with unit form
      */
     virtual QString             getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName = 0 )=0;
     //-------------------------------- getFormeFromCIP ------------------------------------------------------
     /*! \brief return drug presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString with presentation form
      */
     virtual QString             getFormeFromCIP ( const QString &cip)=0;
     //-------------------------------- isStupefiant ------------------------------------------------------
     /*! \brief return if the product identified par Id is a stupefiant
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return 1 is stupefiant
      */
     virtual int             isStupefiant ( const QString &cip )=0;
     //-------------------------------- code_ATC_To_Name ------------------------------------------------------
     /*! \brief return if ATC libelle from code ATC
      *  \param const QString &code      code ATC .
      *  \return QString return libelle
      */
     virtual QString code_ATC_To_Name(const QString &code)=0;
     //-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
     /*! \brief return the drug route from cip.
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString with the drug route
      */
     virtual QString   getDrugRouteFromCIP ( const QString &cip)=0;
     //-------------------------------- getPosologiesFromCIP ------------------------------------------------------
     /*! \brief return posologies list as posologie grammar QString
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
      *  \return QStringList with grammar QString list
      */
     virtual QStringList   getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip )=0;
     //-------------------------------- getInteractionCim10 ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QStringList &cim10List cim10 pathologies list .
      *  \param QString drugName (default empty) the drug name.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     virtual QList<C_InterResultPair> getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  = ""  )=0;
     //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QString &age patient age .
      *  \param const QString &sexe patient sexe F-M-I.
      *  \param const QString &poids patient poids .
      *  \param const QString &taille patient size .
      *  \param const QString &amenorrhee in weks.
      *  \param const QString &clairance in ml/min
      *  \param int   allaitement in number of days
      *  \param bool sport if true particular attention for doping drugs is required  .
      *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
      *  \param QString drugName (default empty) the drug name.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
      virtual QList<C_InterResultPair>        getInteractionAgeSexePoidsSportVigil( const QString &cip,
                                                                                    const QString &age ,
                                                                                    const QString &sexe,
                                                                                    const QString &poids,
                                                                                    const QString &taille,
                                                                                    const QString &amenorrhee,
                                                                                    const QString &clairance,
                                                                                    int   allaitement       ,
                                                                                    bool  sport       = true,
                                                                                    bool  vigil       = true,
                                                                                    QString drugName  = ""  )=0;
     //-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
     /*! \brief return interactions xml results from a cip list.
      *  \param const QStringList &cipList      drugs CIP codes list to test.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     virtual QList<C_InterResultPair> getInteractionPairFromCipList ( const QStringList &cipList)=0;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //------------------ INTERFACE IMPLEMENTEE NON VIRTUELLE----------------------------------------------------------
    //                   chaque plugin offrira la des methodes
    //                   qu'il n'aura pas a implementer
    //-------------------------------- isInitialised ------------------------------------------------------
    /*! \brief return initialised statut .
    */
    int           isInitialised();
    //-------------------------------- getInteractionAllergies ------------------------------------------------------
    /*! \brief return allergicals interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    // ASPIRINE|DCI|ASPIRINE|ALD|Allergie(Allergie)|plutot intolerance|06-06-1956|
    // 3515495|CIP|LAMALINE|Sport|Allergie(Allergie)|urticaire generalise |06-06-1956|
    // 3007528|CIP|ASPIRINE|ALD|Allergie(Allergie)|plutot intolerance|06-06-1956|
    // J01CR02|ATC|AMOXICILINE ET INHIBITEUR D'ENZYME|ALD|Allergie(Allergie)|pas de commentaire|06-06-1956|

    QList<C_InterResultPair> getInteractionAllergies(const QString &cip, QList<C_LifeEvent> lifeEventList, QString drugName  = ""  );
    //-------------------------------- dbi_cip_atcName ------------------------------------------------------
    /*! \brief return ATC code from CIP code.
     *  \param const QString& cip drug cip identification .
     *  \return QString ATC code from this drug
     */
    QString                              dbi_cip_atcName(const QString& cip);
    //-------------------------------- dbi_cip_drugName ------------------------------------------------------
    /*! \brief return drug name code from CIP code.
     *  \param const QString& cip drug cip identification .
     *  \return QString drug name
     */
    QString                              dbi_cip_drugName(const QString& cip);
    //-------------------------------- dbi_atc_to_cip_map ------------------------------------------------------------
    /*! \brief return QMap <dci_name, cip>  from CIP code.
     *  \param atc     atc code from which we want DCI list.
     *  \return QMap <dci_name, cip>
     */
    QMap<QString, QString>               dbi_atc_to_cip_map(const QString &atc);
    //-------------------------------- dbi_selectPosologieHitList ----------------------------------------------------
    /*! \brief select drugs posologies hits list from drug code
     *  \param const QString &drug_id : as variable name shows, it's drug identification key.
     *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
     *  \param int limit : limit number of returns.
     *  \return QMap <int, QString> where int is order number and QString is grammar string started by 'cip/pk!' mention
     */
    QStringList  dbi_selectPosologieHitList(const QString &drug_id, const QString &drug_id_type = "CIP", int limit  = 15  );

    //-------------------------------- dbi_increasePosologieHit ------------------------------------------------------
    /*! \brief increase a posologie drug hit parade adjusted in real time of use. Create record only if not exists.
     *  \param const QString &grammar :     grammar input string as
     *  "345687/123!ALL_LITERAL=comprime[1;m|2;M|4;s]/8\n[0.5;m|1;M15r|2;sr30|3;h10-13]/4\n[0.5;m|1;s]/3".
     *  where cip code is 345687 and if present facultative pk code 123 after '/' symbol.
     *  \param const QString &drug_id : as variable name shows, it's drug identification key.
     *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
     *  \param int mode  default = INCREASE_HIT/increase hit  else FIX_HIT add the posologie whith fix hit.
     *  \return pk of created or updated record if all is ok otherwise empty string
     */
    QString dbi_increasePosologieHit(const QString &grammar, const QString &id, const QString &drug_id_type  = "CIP" ,  int mode   = INCREASE_HIT );

    //-------------------------------- dbi_deleteItemPosologieHit ----------------------------------------------------
    /*! \brief delete a item from drug hit parade.
     *  \param const QString &pk :  item pk to delete
     *  \return true in all is ok otherwise false
     */
    int dbi_deleteItemPosologieHit(const QString &pk );

    //------------------------ dbi_selectProductsList ----------------------------------------------------------------
    /*! \brief select drugs list from user input, for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
     */
    const QList < C_BDM_DrugListRecord > dbi_selectProductsList   (const QString &text, C_BDM_PluginI::flags filterFlag   = C_BDM_PluginI::all_filter  , int limit  =200);

    //------------------------ dbi_selectFavorisList -----------------------------------------
    /*! \brief select drugs in hitlist from user input, for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
     */
    const QList < C_BDM_DrugListRecord > dbi_selectFavorisList (const QString &text, C_BDM_PluginI::flags filterFlag  = C_BDM_PluginI::all_filter  , int limit   =200);


    //------------------------ dbi_updateDrugLists -----------------------------------------
    /*! \brief for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
     *  \return true in all is ok otherwise false
     */
    bool dbi_updateDrugLists();

    //------------------------ dbi_DrugListCount -----------------------------------------
    /*! \brief return DrugList records number
     *  \return return DrugList records number
     */
    int dbi_DrugListCount();

    //-------------------------------- dbi_createPosologieHit ------------------------------------------------------
    /*! \brief this table records a posologie drug hit parade adjusted in real time of use.
     *  \param int mode  default = ONLY_IF_NOT_EXISTS/create table only if not exists, otherwise create table even if exists.
     *  \return true in all is ok otherwise false
     */
    int dbi_createPosologieHit(int mode = ONLY_IF_NOT_EXISTS );

    //-------------------------------- dbi_verifyDataSourceVersion ------------------------------------------------------
    /*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
     *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
     *  \return true in all is ok otherwise false (0)
     */
    int dbi_verifyDataSourceVersion();

protected:
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //--------------------------- PROTECTED ZONE ---------------------------------------------------------------------
    //--------------------------- dbi_database -----------------------------------------------------------------------
    /*! \brief return intermediate QSqlDatabase (prefixed with dbi_  ) which contains additionals services tables as :
     *    'bdm_druglist' 'my_cache_monographie' .
    */
    C_BaseCommon         *dbi_database() {return m_pDbi_Database;}

    //-------------------------------- dbi_init_database -------------------------------------------------------------
    /*! \brief connect and init the additionnal database.
    *  this function needs an ANOTHER VALID and connected DATABASE to acced SQL functions,
    *  to eventualy create additionnal database, if not already created.
    *  So for this, it is necessary that the DERIVED class has already initialised his proper database,
    *  and it is responsable to call this method with his proper database in databaseAlreadyInitialised
    *  argument.
    *  \param const QSqlDatabase &databaseAlreadyInitialised  SQL database already initialised
    *  \return true if all is OK false otherwise.
    */
    bool    dbi_init_database(const QString &baseName, const QString &prefixLabelName, const QString &owner, C_BaseCommon *databaseAlreadyInitialised);

    //-------------------------------- dbi_makeSelectProductsListQuery ---------------------------------------------
    /*! \brief make select query to select drugs list from user input, for a good performance
     *  response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return query result string
     */
    QString   dbi_makeSelectProductsListQuery(const QString &text, C_BDM_PluginI::flags filterFlag);


    //-------------------------------- setInitialisedSate ----------------------------------------------------------
    /*! \brief set Initialised Sate
    */
    void setInitialisedSate(int state);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //------------------ PRIVATE ZONE -------------------------------------------------------------------------------
private:

    //-------------------------------- dbi_lastDataSourceVersion ------------------------------------------------------
    /*! \brief return last DataSourceVersion
    */
    QString dbi_lastDataSourceVersion();

private:
    int                 m_isInitialised;
//$    C_PatientCtx       *m_pC_PatientCtx;
    C_BaseCommon       *m_pDbi_Database;

};

//=========================================================== C_BDM_Api ===================================
/*! \class C_BDM_Api
 *  \brief Class Api to use for LAP GUI for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  so we needs SQL functions to manage this druglist.
 */

class C_BDM_Api : public C_BaseCommon,            // le gestionnaire generique de bases de donnee
                         C_BDM_ApiVar             // le nom des champs de la base de donnees
{
    Q_OBJECT
public:
    enum flags  {no_filter   = 0,
                 atc_filter  = 1,
                 nom_filter  = 2,
                 dci_filter  = 4,
                 ucd_filter  = 8,
                 dc3_filter  = 16,
                 cip_filter  = 32,
                 gen_filter  = 64,
                 vil_filter  = 128,
                 hop_filter  = 256,
                 all_filter  = 511
                };
    enum hit_flag  {ONLY_IF_NOT_EXISTS   = 0,
                    EVEN_IF_EXISTS       = 1,
                    INCREASE_HIT         = 2,
                    FIX_HIT              = 3
                   };
    // affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
    enum dci_flag  {COM      = 0,
                    COM_DCI  = 1,
                    DCI_COM  = 2,
                    DCI      = 3
                   };

    //------------------------ C_BDM_Api -----------------------------------------
    /*! \brief constructor
     *  \param const QString   &confData:          fields name definitions.
     *  \param const QString   &connectParams      connexions parameters.
     *  \param const QString   &imagePath          image path.
     *  \param const QString   &pathAppli          application path.
     *  \param const QString   &pathIni            application configuration path.
     *  \param QObject         *parent             pointer QObject parent.
     *  \param QTextEdit       *pLogWidget         pointer QTextEdit log widget (can be omitted).
     *  \param int              userMonographie    default value is 1, zero/if we don't want UserMonographie (expl we need just interaction control)
    */
    C_BDM_Api           ( const QString   &confData,                        // definition des champs de la base de donnee (ceux de la liste des drogues)
                          const QString   &prefixLabelName,                 // prefixage du label (permettre plusieurs base)
                          const QString   &connectParams ,                  // parametres de connexion a la base de donnees
                          const QString   &imagePath,                       // chemin des images
                          const QString   &pathAppli,                       // chemin de l'applicatif
                          const QString   &pathIni,                         // chemin du fichier de configuration general
                          QObject         *parent,                          // objet parent
                          QTextEdit       *pLogWidget = 0,                  // pointer QTextEdit log widget (can be omitted).
                          int              userMonographie = 1              // default value is 1, zero/if we don't want UserMonographie (expl we need just interaction control)
                        );

    //------------------------ ~C_BDM_Api -----------------------------------------
    /*! \brief destructor  C_BDM_Api
    */
    ~C_BDM_Api();

    //........................ INTERFACE D'INITIALISATION ...................................
    //------------------------ changeDataBaseConnexion -----------------------------------------
    /*! \brief change database connexion.
     *  \param  const QString &driver driver name QDB2 QIBASE QMYSQL QOCI QODBC QPSQL QSQLITE QSQLITE2 QTDS
     *  \param  const QString &schema schema database name to coonect
     *  \param  const QString &user connexion user name
     *  \param  const QString &_password password : encoded (begin by '#')  or not encoded (don't begin by '#')
     *  \param  const QString &hostname IP adress or domain name
     *  \param  const QString &port port on wich connexion must be opened
     *  \return true in all is ok otherwise false
     */
    int changeDataBaseConnexion(const QString &driver, const QString &schema, const QString &user, const QString &password, const QString &hostname, const QString &port);
    //------------------------ connectToDataSource -----------------------------------------
    /*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
     *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
     *  \return true in all is ok otherwise false
     */
    int connectToDataSource(C_BDM_PluginI *pC_BDM_PluginI);
    //-------------------------------- getInteractionPhysicoChimiques ------------------------------------------------------
    /*! \brief return chimico physiques interactions in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx);
    //-------------------------------- getAllergiesFromCipList ------------------------------------------------------
    /*! \brief return Allergies interactions in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx);
    //-------------------------------- getInteractionRedundancy ------------------------------------------------------
    /*! \brief return redundancy composition in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionRedundancy(const QStringList &cipList, C_PatientCtx *pC_PatientCtx);
    //------------------------ isValid -----------------------------------------
    /*! \brief return validation state of this class. if no valid the class can't works
     */
    int      isValid();

    //........................ INTERFACE D'UTILISATION ....................................................
    //-------------------------------- createCustomMonographieTable ------------------------------------------------------
    /*! \brief create custom monographie table
     *  \return 1/all is ok 0/list not created
     */
    int createCustomMonographieTable();
    //-------------------------------- add_Monographie_to_CustomMonographie --------------------------------------------
    /*! \brief add in dbm a product Monographie to a product
     *  \param QString    libelle the monographie
     *  \param QString    author of the monographie
     *  \param QByteArray datas of monographie
     *  \param QDateTime  date of informations
     *  \param C_BDM_DrugListRecord  &product on wich add informations
     *  \return 1/all is ok 0/datas are added
     */
    int add_Monographie_to_CustomMonographie(const QString &libelle,const QString &author, const QByteArray &datas, const QDateTime &dt, const C_BDM_DrugListRecord  &product);
    //-------------------------------- selectCustomMonographie --------------------------------------------
    /*! \brief make select query to select drugs list from user input, for a good performance
     *  \param C_BDM_DrugListRecord  &product from wich we wants informations
     *  \param QString    libelle the monographie
     *  \param QString    author of the monographie
     *  \param QDateTime  dt_deb start date of informations to retrieve
     *  \param QDateTime  dt_deb end date of informations  to retrieve
     *  \return QString which is monographie
     */
    QStringList selectCustomMonographie(const C_BDM_DrugListRecord  &c_bdm_druglistrecord, const QString &libelle = "", const QString &author="", const QDateTime &dt_deb=QDateTime(), const QDateTime &dt_end=QDateTime());
    //-------------------------------- get_CustomMonographie --------------------------------------------
    /*! \brief make select query to select drugs list from user input, for a good performance
     *  \param C_BDM_DrugListRecord  &product from wich we wants informations
     *  \param QString &pk primary key of the recor to retrieve
     *  \param QString    libelle the monographie to get
     *  \param QString    author of the monographie  to get
     *  \param QDateTime  dt_deb start date  to get
     *  \return QString which is monographie WITHOUT title header
     */
    QString get_CustomMonographie( const QString &pk, QString &libelle,  QString &author,  QDateTime &dt_deb);
    //-------------------------------- delete_CustomMonographie --------------------------------------------
    /*! \brief delete Custom Monographie
     *  \param QString    pk primary key of the recodr to delete
     *  \return 1/all is ok 0/product not deleted
     */
    bool delete_CustomMonographie(const QString &pk);
    //-------------------------------- update_CustomMonographie --------------------------------------------
    /*! \brief delete Custom Monographie
     *  \param QString    pk primary key of the recodr to update
     *  \param QString    libelle of the title of monographie
     *  \param QString    author of the monographie
     *  \param QByteArray datas the datas of monographie
     *  \param QDateTime  date of informations
     *  \return 1/all is ok 0/product not updated
     */
    bool update_CustomMonographie(const QString &pk, const QString &libelle, const QString &author, const QDateTime &dt, const QByteArray &datas);
    //-------------------------------- customProductsTableName ------------------------------------------------------
    /*! \brief create custom drugs list table name from userName and listName
     *  \param QString userName the user name for this custom list.
     *  \param QString listName the name for this custom list.
     *  \param QString prefix to name this table.
     *  \return QString wich is table name
     */
    static QString customProductsTableName(const QString &userName, const QString &listName, const QString &prefix = "perso_drugslist" );

    //-------------------------------- createCustomProductsList ------------------------------------------------------
    /*! \brief create custom drugs list
     *  \param QString userName the user name for this custom list.
     *  \param QString listName the uname for this custom list.
     *  \return 1/all is ok 0/list not created
     */
    int createCustomProductsList(const QString &userName, const QString &listName);

    //-------------------------------- deleteCustomProductsList ------------------------------------------------------
    /*! \brief delete custom drugs list
     *  \param QString userName the user name for this custom list.
     *  \param QString listName the uname for this custom list.
     *  \return 1/all is ok 0/list not deleted
     */
    int deleteCustomProductsList(const QString &userName, const QString &listName);
    //-------------------------------- getListCustomProductsList ------------------------------------------------------
    /*! \brief return a list of custom drugs list for a user
     *  \param QString userName the user name
     *  \return QStringList of custom drugs list for this user
     */
    QStringList getListCustomProductsList(const QString &userName);
    //-------------------------------- add_Product_to_CustomProductsList --------------------------------------------
    /*! \brief add in dbm a product list to a custom products list
     *  \param QString userName the user name
     *  \param QString listName the name for this custom list.
     *  \param QList < C_BDM_DrugListRecord > &drugs List products to add
     *  \return 1/all is ok 0/product not added
     */
    int add_Product_to_CustomProductsList(const QString &userName, const QString &listName, const QList < C_BDM_DrugListRecord > &drugsList);
    //-------------------------------- del_Product_from_CustomProductsList --------------------------------------------
    /*! \brief remove a product to a custom products list
     *  \param QString userName the user name
     *  \param QString listName the name for this custom list.
     *  \param const C_BDM_DrugListRecord &drugListRecord product to remove
     *  \return true/all is ok false/product not deleted
     */
    bool del_Product_from_CustomProductsList(const QString &userName, const QString &listName, const C_BDM_DrugListRecord &drugListRecord);
    //-------------------------------- selectCustomProductsList ------------------------------------------------------
    /*! \brief return a list of custom drugs list for a user
     *  \param QString userName the user name
     *  \param QString listName the name for this custom list.
     *  \param QString text drug to find.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return QList < C_BDM_DrugListRecord >  drugs list for this user
     */
    const QList < C_BDM_DrugListRecord > selectCustomProductsList(const QString &userName, const QString &listName, const QString &text,  C_BDM_PluginI::flags filterFlag =  C_BDM_PluginI::all_filter);
    //-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
    /*! \brief return if  plugin is HAS certified
     *  \return return HAS certified status
     */
    bool is_plugin_HAS_certified();
    //-------------------------------- HAS_NotCertifiedAlertText ------------------------------------------------------
    /*! \brief return text to display when medicatux is HAS not certified
     *  \param const QString& pathImage if is not empty de feualt path image is updated .
     *  \return return HAS uncertified string
     */
    QString  HAS_NotCertifiedAlertText(const QString &pathImage  = "" );
    //-------------------------------- plugin_name ------------------------------------------------------
    /*! \brief return active plugin name
     *  \return return active plugin name
     */
    QString plugin_name();
    //-------------------------------- plugin_icon ------------------------------------------------------
    /*! \brief return active plugin_icon
     *  \return return active plugin_icon
     */
    QPixmap plugin_icon();
    //---------------------------------------- get_database -------------------------------------------------------
    /*! \brief  return active database. This facility is to do extended SQL querys which are not in API
     *  \return  database
     */
    QSqlDatabase get_database();
    //-------------------------------- plugin_image_source ------------------------------------------------------
    /*! \brief  return image source url for monographies
     *  \return return image source url for monographies
     */
    QString plugin_image_source();
    //-------------------------------- drugsList_Size ------------------------------------------------------
    /*! \brief return size of drugs list. This return only cache value and proceed to no SQL operation
     */
    int                 drugsList_Size();
    //------------------------ selectProductsList -----------------------------------------
    /*! \brief select drugs list from user input, for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param QTreeWidget *pQtreeWidget: QTreeWidget to fill with datas.
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return number products
     */
    int selectProductsList (  QTreeWidget *pQTreeWidget,                                      // QTreeWidget graphique a renseigner avec les donnees
                              const QString textSelect        = "",                           // saisie partielle d'un texte pour recherche
                              C_BDM_PluginI::flags filterFlag = C_BDM_PluginI::all_filter,    // filtre de saisie
                              int limit = 200
                            );
    //------------------------ selectProductsList -----------------------------------------
    /*! \brief select drugs list from user input, for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
     */
    const QList < C_BDM_DrugListRecord > selectProductsList (const QString &texte, C_BDM_PluginI::flags = C_BDM_PluginI::all_filter, int limit = 200);
    //------------------------ selectFavorisList -----------------------------------------
    /*! \brief select drugs in hitlist from user input, for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
     */
    const QList < C_BDM_DrugListRecord > selectFavorisList (const QString &text, C_BDM_PluginI::flags filterFlag   = C_BDM_PluginI::all_filter  , int limit = 200);

    //------------------------ dbi_updateDrugLists -----------------------------------------
    /*! \brief for a good performance response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
     *  \return true in all is ok otherwise false
     */
    bool updateDrugLists();
    //------------------------------ getRefundedIndications ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
     *  \param const QString &code_type  code typ as giving by the drugs BDM
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
    QList <C_BDM_IndicationRecord> getRefundedIndications( const QString &code_indication,   const QString &code_type );
    //------------------------ getListIndications -----------------------------------------
    /*! \brief select indications list from product identified by const QString &id, const QString &id_type
     *  filtred by desired smr
     * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param QString &id code of the drug from wich we want indications exemple cip code of the drug.
     *  \param QString &id_type type of id code for exemple "CIP" for cip code, "THE" for theriaque code
     *  \param QString &smrLevel list of the smr desired smr levels filter as this
     *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
     */
    QStringList getListIndications (const QString &id, const QString &id_type, const QString &smrLevel);
    //------------------------------ getIndicationsFromText ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &text     text from wich we want indications datas.
     *  \param const QString &filter   filter wanted datas  a combination | of this flags
     *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
     *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
     *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
     *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
     *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
    QList <C_BDM_IndicationRecord> getIndicationsFromText( const QString &text,   int filter );
    //------------------------------ getIndicationsFromCIP ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \param const QString &filter   filter wanted datas  a combination | of this flags
     *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
     *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
     *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
     *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
     *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
    QList <C_BDM_IndicationRecord> getIndicationsFromCIP( const QString &cip,   int filter );

    //------------------------------ getCIPFromIndication ------------------------------------------
    /*! \brief select indications informations for a drug.
     *  \param const QString &code       indication code of the drugs from wich we want datas.
     *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
     *  \return QStringList cip result list
     */
    QStringList getCIPFromIndication( const QString &code, const QString &codeType );
    //-------------------------------- getInteractionPrescriptionRestreintePairFromCipList ------------------------------------------------------
    /*! \brief return all restrictives prescriptions allert from cip list
     *  \param const QStringList &cipList      drugs CIP codes list to test.
     *  \return QList<C_InterResultPair> with result interactions
     */
    QList<C_InterResultPair> getInteractionPrescriptionRestreintePairFromCipList ( const QStringList &cipList);
    //-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
    /*! \brief test compatibility with maximum and minimun authorized dose from a drug list and patient context.
     *  \param C_PatientCtx *pC_PatientCtx
     *  \param const QStringList &grammarList grammar prescription list
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QStringList &grammarList);

    //-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
    /*! \brief test compatibility with maximum and minimun authorized dose from a drug and patient context.
     *  \param C_PatientCtx *pC_PatientCtx
     *  \param const QString &grammar prescription for this drug
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair>  getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar);

    //-------------------------------- fill_ATC_treeWidget ------------------------------------------------------
    /*! \brief fill pQTreeWidget with ATC classification
     *  \param pQTreeWidget QTreeWidget to fill with datas     .
     */
    void fill_ATC_treeWidget(QTreeWidget *pQTreeWidget);

    //-------------------------------- ATC_from_CIP ------------------------------------------------------
    /*! \brief return ATC code from CIP code.
     *  \param const QString& cip drug cip identification .
     *  \return QString ATC code from this drug
     */
    QString ATC_from_CIP(const QString& cip);
    //-------------------------------- code_ATC_To_Name ------------------------------------------------------
    /*! \brief return if ATC libelle from code ATC
     *  \param const QString &code      code ATC .
     *  \return QString return libelle
     */
    QString code_ATC_To_Name(const QString &code);
    //-------------------------------- CIP_to_DrugName ------------------------------------------------------
    /*! \brief return drug name code from CIP code.
     *  \param const QString& cip drug cip identification .
     *  \return QString drug name
     */
    QString CIP_to_DrugName(const QString& cip);
    //-------------------------------- getCompositionFromCIP ------------------------------------------------------
    /*! \brief return drug composition.
     *  \param const QString& cip drug cip identification .
     *  \return QList<C_CompositionItem> result list where
     *              C_CompositionItem.name()       is the composant name (DCI)
     *              C_CompositionItem.amount()     is the quantity
     *              C_CompositionItem.unit()       is the quantity unit
     */
     QList<C_CompositionItem> getCompositionFromCIP(const QString &cip);
     //-------------------------------- getExcipientsFromCIP ------------------------------------------------------
     /*! \brief return drug excipients list .
      *  \param const QString& cip drug cip identification .
      *  \return QList<C_ExcipientItem> result list where
      *              C_ExcipientItem.name()              is the composant name (excipient)
      *              C_ExcipientItem.amount()            is the quantity if present (not regular)
      *              C_ExcipientItem.unit()              is the quantity unit if present (not regular)
      *              C_ExcipientItem.code()              is the substance code  (specific at BDM implementation)
      *              C_ExcipientItem.code_typ            is the code type (specific at BDM implementation)
      */
     QList<C_ExcipientItem> getExcipientsFromCIP(const QString &cip);
     //-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
     /*! \brief return drug composition.
      *  \param const QString& cip drug cip identification .
      *  \return QStringList result list with dci composants
      */
      QStringList get_DCI_list_FromCIP(const QString &cip);
      //-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
      /*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
       *         if there is not restrictive prescription for this CIP  isValid() is false;
       *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
       *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
       *  \return QList<C_PrescriptionRestreinte>
       */
      QList<C_PrescriptionRestreinte> getPrescriptionRestreinteFromCIP(const QString &cip);
    //-------------------------------- getMonographieFromCIP ------------------------------------------------------
    /*! \brief select monographie for a drug.
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \return QString contains monographie
     */
    QString             getMonographieFromCIP (  const QString &cip,   bool dataMustBeReadInCache  = true );
    //-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
    /*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres a cafe etc...)
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \return QString with unit form
     */
    QString             getPriseUnitFormFromCIP ( const QString &cip, const QString &commercialName = 0);
    //-------------------------------- getFormeFromCIP ------------------------------------------------------
    /*! \brief return drug presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \return QString with presentation form
     */
    QString             getFormeFromCIP ( const QString &cip);

    //-------------------------------- dataSourceVersion ------------------------------------------------------
    /*! \brief return datasource version
     */
    QString dataSourceVersion();
    //--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
    /*! \brief return ANSM (ext afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ext afssaps) web site.
     *  \param const QString cip is drug cip code.
     *  \param QString *rcp is pointer on QString to return RCP result if not zero .
     *  \return CIS code string
    */
    QString get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp =0 );
    //-------------------------------- isStupefiant ------------------------------------------------------
    /*! \brief return if the product identified par Id is a stupefiant
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \return 1 is stupefiant
     */
    int             isStupefiant ( const QString &cip );
    //-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
    /*! \brief return the drug route from cip.
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \return QString with the drug route
     */
    QString   getDrugRouteFromCIP ( const QString &cip);
    //-------------------------------- getPosologiesFromCIP ------------------------------------------------------
    /*! \brief return posologies list as posologie grammar QString
     *  \param const QString &cip      code CIP of the drug from wich we want datas.
     *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
     *  \return QStringList with grammar QString list
     */
    QStringList   getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip );

    //------------------------------- interaction_Text -----------------------------------------------------------------
     /*! \brief return html text for interaction betwwen two composants ( C_InterResultPair  &interResultPair ) .
      *  \param const C_InterResultPair  &interResultPair  structure wich represents interaction betwwen two composants
      *  \return QString html out result
      */
    QString  interaction_Text(const C_InterResultPair  &interResultPair);
    //------------------------ evaluateAllInteractions -----------------------------------------
    /*! \brief evaluate all interaction from a products list (const QStringList &grammarList) and  patient context (implicite in C_BDM_Api).
     *  \param const QStringList &grammarList  products list
     *  \return QString html result
     */
    QString evaluateAllInteractions(const QStringList &grammarList);
    //------------------------ htmlEncapsulateInteraction -----------------------------------------
    /*! \brief encapsulate interaction text in html struct.
     *  \param const QString &text_in  text to encapsulate
     *  \return QString html out result
     */
    QString htmlEncapsulateInteraction(const QString &text_in);
    //------------------------------- interactionTerrain_LevelToTexte [static] -----------------------------------------------------------------
    /*! \brief return QString level interaction betwen a products ant patient sane state.
     *  \param int level of interaction .
     *  \return QString
     */
static QString  interactionTerrain_LevelToTexte(int level);
    //------------------------------- afssaps_interactions_LevelToTexte [static]-----------------------------------------------------------------
    /*! \brief return QString level interaction betwen two products.
     *  \param int level of interaction .
     *  \return QString
     */
static QString  afssaps_interactions_LevelToTexte(int level);

    //------------------------------- level_SMR_ToLibelle [static]-----------------------------------------------------------------
    /*! \brief return QString level interaction betwen two products.
     *  \param int level of interaction .
     *  \return QString
     */
static QString level_SMR_ToLibelle(int level);

    //------------------------------- noSecureText -----------------------------------------------------------------
    /*! \brief return html text for products not evaluated by interaction engine.
     *  \return html text
     */
    QString noSecureProductText(const QString &productName);
    //------------------------------- terrainLevel_ToPixmapPath -----------------------------------------------------------------
    /*! \brief return pixmap path for terrain level.
     *  \param int terrain level
     *  \return QString pixmap path.
    */
    QString terrainLevel_ToPixmapPath(int level);
    //-------------------------------- allergie_LevelToPixmapPath ------------------------------------------------------
    /*! \brief return pixmap path for allergie level.
     *  \param int allergie level
     *  \return QString pixmap path.
     */
    QString allergie_LevelToPixmapPath(int level) const;

    //-------------------------------- afssaps_LevelToPixmapPath ------------------------------------------------------
    /*! \brief return pixmap path for afssaps level.
     *  \param int afssaps level
     *  \return QString pixmap path.
     */
    QString afssaps_LevelToPixmapPath(int level) const;
    //-------------------------------- getInteractionAllergies ------------------------------------------------------
    /*! \brief return allergicals interactions from patient and drugs.
     *  \param const QString& QStringList drug cip drug list .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionAllergiesFromCipList( const QStringList &cipList );
    //-------------------------------- getInteractionAllergies ------------------------------------------------------
    /*! \brief return allergicals interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionAllergies(const QString &cip , QString drugName  = "" );
    //-------------------------------- getInteractionCim10 ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionCim10(const QString &cip, QString drugName  = ""  );
    //-------------------------------- getInteractionCim10FromCipList ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QStringList &cipList drugs cip list .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionCim10FromCipList( const QStringList &cipList);

    //-------------------------------- getInteractionCim10 ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param const QStringList &cim10List cim10 pathologies list .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  = ""  );
    //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QStringList &cipList drugs cip list .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair>         getInteractionAgeSexePoidsSportVigil( const QStringList &cipList);

    //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
     QList<C_InterResultPair>        getInteractionAgeSexePoidsSportVigil(const QString &cip, QString drugName ="" );
    //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param const QString &age patient age .
     *  \param const QString &sexe patient sexe F-M-I.
     *  \param const QString &poids patient poids .
     *  \param const QString &taille patient size .
     *  \param const QString &amenorrhee in weks.
     *  \param const QString &clairance in ml/min
     *  \param int            allaitement in number of days
     *  \param bool sport if true particular attention for doping drugs is required  .
     *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
     QList<C_InterResultPair>        getInteractionAgeSexePoidsSportVigil(const QString &cip,
                                                                          const QString &age ,
                                                                          const QString &sexe,
                                                                          const QString &poids,
                                                                          const QString &taille,
                                                                          const QString &amenorrhee,
                                                                          const QString &clairance,
                                                                          int   allaitement,
                                                                          bool  sport       = true,
                                                                          bool  vigil       = true,
                                                                          QString drugName  = ""  );
    //-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
    /*! \brief return interactions xml results from a cip list.
     *  \param const QStringList &cipList      drugs CIP codes list to test.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionPairFromCipList ( const QStringList &cipList);

    //-------------------------------- selectPosologieHitList ------------------------------------------------------
    /*! \brief select drugs posologies hits list from drug code
     *  \param const QString &drug_id : as variable name shows, it's drug identification key.
     *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
     *  \param int limit : limit number of returns.
     *  \return QMap <int, QString> where int is order number and QString is grammar string started by 'cip/pk!' mention
     */
    QStringList  selectPosologieHitList(const QString &drug_id, const QString &drug_id_type = "CIP", int limit  = 15  );

    //-------------------------------- increasePosologieHit ------------------------------------------------------
    /*! \brief increase a posologie drug hit parade adjusted in real time of use. Create record only if not exists.
     *  \param const QString &grammar :     grammar input string as
     *  "345687/123!ALL_LITERAL=comprime[1;m|2;M|4;s]/8\n[0.5;m|1;M15r|2;sr30|3;h10-13]/4\n[0.5;m|1;s]/3".
     *  where cip code is 345687 and if present facultative pk code 123 after '/' symbol.
     *  \param const QString &drug_id : as variable name shows, it's drug identification key.
     *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
     *  \param int mode  default = INCREASE_HIT/increase hit  else FIX_HIT add the posologie whith fix hit.
     *  \return pk of created or updated record if all is ok otherwise empty string
     */
     QString increasePosologieHit(const QString &grammar, const QString &id, const QString &drug_id_type  = "CIP" ,  int mode   = INCREASE_HIT );

    //-------------------------------- deleteItemPosologieHit ------------------------------------------------------
    /*! \brief delete a item from drug hit parade.
     *  \param const QString &pk :  item pk to delete
     *  \return true in all is ok otherwise false
     */
    int deleteItemPosologieHit(const QString &pk );
    //-------------------------------- reinitHitsList ------------------------------------------------------
    /*! \brief delete the actual hit list and remake a new.
     */
    void reinitHitsList();
    //-------------------------------- patientContexte ------------------------------------------------------
    /*! \brief return a poointer on C_PatientCtx (patient contexte) .
    */
    C_PatientCtx *patientContexte();
    //-------------------------------- setPatientContexte ------------------------------------------------------
    /*! \brief set patient contexte.
    */
    void setPatientContexte(C_PatientCtx* patientCtx);
    //--------------------------------------- fillTreeWidgetIndicationsFromPatientContext -------------------------------------------------------
    /*! \brief fill treewidget indication with patient pathologies as indications.
     * implicit input : m_pC_PatientCtx wich maintains patient pathologies list
     *  \param pQTreeWidget :  pointer to QTreeWidget in wich add informations
     *  \param icon :         icon associated to this item
     *  \return nothing
    */
    void fillTreeWidgetIndicationsFromPatientContext(QTreeWidget *pQTreeWidget, const QIcon &icon);
    //-------------------------------- patientContexte ------------------------------------------------------
    /*! \brief return a poointer on C_UserCtx (user contexte) .
    */
    C_UserCtx *userContexte();
    //-------------------------------- setUserContexte ------------------------------------------------------
    /*! \brief set user context
    */
    void setUserContexte(C_UserCtx* userCtx);
    //-------------------------------- getUserInfos ------------------------------------------------------
    /*! \brief get user informations (if user exists)
     *  \param QString *name :  pointer to a QString in wich if not zero will be returned name
     *  \param QString *first_name :  pointer to a QString in wich if not zero will be returned first name
     *  \param QString *rpps :  pointer to a QString in wich if not zero will be returned rpps
     *  \return login in all is ok otherwise empty string
    */
    QString getUserInfos(QString *name, QString *first_name, QString *rpps);
    //-------------------------------- pathImage ------------------------------------------------------
    /*! \brief return  pathImage .
    */
    QString pathImage(){return m_imagePath;}
    //-------------------------------- pathAppli ------------------------------------------------------
    /*! \brief return  pathAppli .
    */
    QString pathAppli(){return m_pathAppli;}
    //-------------------------------- pathIni ------------------------------------------------------
    /*! \brief return  pathIni .
    */
    QString pathIni(){return m_pathIni;}
    //-------------------------------- defaultDurationInSeconds ------------------------------------------------------
    /*! \brief return  default duration of a prescription product in seconds .
    */
    int defaultDurationInSeconds(){return m_defaultDurationInSeconds;}
    //-------------------------------- setDefaultDurationInSeconds ------------------------------------------------------
    /*! \brief set  default duration of a prescription product in seconds .
    */
    void setDefaultDurationInSeconds(int value) { m_defaultDurationInSeconds = value;}
    //-------------------------------- stopAllInteractions ------------------------------------------------------
    /*! \brief set  m_stopAllInterraction to 1 to stops evaluateAllInteractions() .
    */
    void stopAllInteractions();
    //-------------------------------- deleteUnsignificativesZeros ------------------------------------------------------
    /*! \brief delete unsignificative zeros 000123.23  --> 123.00 .
    */
    static QString deleteUnsignificativesZeros(const QString &value);
    //-------------------------------- Wait[static] ------------------------------------------------------
    /*! \brief wait ms microseconds
     * \param : ms time to wait in microseconds
    */
    // static void Wait(int ms);
//................................ PRIVATE ....................................................................
private :

    //................. initialisations ............................
    int      connectBase(const QString &params, const QString &prefixLabel);

    //-------------------------------- drugsList_Count ------------------------------------------------------
    /*! \brief return size of drugs list. proceed to SQL operation and set cache list number.
    */
    int      drugsList_Count();
    //-------------------------------- makeSelectCustomProductsListQuery --------------------------------------------
    /*! \brief make select query to select drugs list from user input, for a good performance
     *  response a rapid special drugs list is implemented
     *  this list is to update each news datas base source versions datas. We proceed update if
     *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
     *  \param QString userName the user name
     *  \param QString listName the name for this custom list.
     *  \param const QString textSelect user text fr select drugs.
     *  \param C_BDM_Api::flags filterFlag selection filter.
     *  \return query result string
     */
    QString makeSelectCustomProductsListQuery(const QString &userName, const QString &listName, const    QString &text, C_BDM_PluginI::flags filterFlag   = C_BDM_PluginI::all_filter);

private:
    C_PatientCtx       *m_pC_PatientCtx;
    C_UserCtx          *m_pC_UserCtx;
    int                 m_isValid;
    int                 m_DrugListNb;
    C_BDM_PluginI      *m_pC_BDM_PluginI;
    QString             m_imagePath;
    QString             m_pathAppli;
    QString             m_pathIni;
    int                 m_defaultDurationInSeconds;
    int                 m_stopAllInterraction;
    //............. les caches ............................
    QList <C_InterResultPair>  m_AllergieCacheList;
    QList <C_InterResultPair>  m_interResultCacheList;
    QList <C_InterResultPair>  m_interPhysicoChimiquesCacheList;
    QMap <QString , C_InterResultPair>  m_interResultPrescrRestrCacheList;   // key is cip value C_InterResultPair
};

#endif // C_BDM_API_H

