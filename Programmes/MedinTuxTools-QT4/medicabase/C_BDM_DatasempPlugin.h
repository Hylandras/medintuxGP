#ifndef C_BDM_DATASEMPPLUGIN_H
#define C_BDM_DATASEMPPLUGIN_H

#include <QObject>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QObject>

#include "../C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/medicabase/C_InterResultPair.h"

//=================== C_AllergItem ===================================
/*! \class C_AllergItem
 *  \brief Class to maintain allergies informations
 */
class  C_AllergItem
{ public:
    explicit C_AllergItem     (const QString &code          ,
                               const QString &code_type     ,
                               const QString &drug_name     ,
                               const QString &libelle       ,
                               const QString &code_produit  ,
                               const QString &code_classe   ,
                               const QString &code_1        ,
                               const QString &code_2
                              )
    { m_code          = code;
      m_code_type     = code_type;
      m_drug_name     = drug_name;
      m_libelle       = libelle;
      m_code_produit  = code_produit;
      m_code_classe   = code_classe;
      m_code_1        = code_1;
      m_code_2        = code_2;
    }
    C_AllergItem(){}
public:
   QString  code()          const {return m_code;}
   QString  code_type()     const {return m_code_type;}
   QString  drug_name()     const {return m_drug_name;}
   QString  libelle()       const {return m_libelle;}
   QString  code_produit()  const {return m_code_produit;}
   QString  code_classe()   const {return m_code_classe;}
   QString  code_1()        const {return m_code_1;}
   QString  code_2()        const {return m_code_2;}

private:
   QString   m_code;
   QString   m_code_type;
   QString   m_drug_name;
   QString   m_libelle;
   QString   m_code_produit;
   QString   m_code_classe;
   QString   m_code_1;
   QString   m_code_2;
};

class QDomElement;
//===================================== C_BDM_DatasempPlugin ==============================
class C_BDM_DatasempPlugin  :  public  C_BaseCommon ,  public C_BDM_PluginI
{
    Q_OBJECT
public:
    explicit C_BDM_DatasempPlugin(const QString &confData, const QString &prefixLabel, QObject *parent = 0, QTextEdit *logWidget=0, QProgressBar *pQProgressBar=0);
    ~C_BDM_DatasempPlugin();
     //------------------ INTERFACE VIRTUELLE -----------------
     int           init              ( const QString &confData     , const QString &prefixLabel);
     void          setLogWidget      ( QTextEdit *logWidget        );
     void          setProgressWidget ( QProgressBar *pQProgressBar );
     QProgressBar* progressWidget()  { return C_BaseCommon::progressWidget(); }
     QString       dataSourceVersion();
     QString       lang();
     QString       owner();
     QString       getLastError();
     //-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
     /*! \brief return if  plugin is HAS certified
      *  \return return HAS certified status
      */
     bool is_plugin_HAS_certified();
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
     //-------------------------------- getCompositionFromCIP ------------------------------------------------------
     /*! \brief return drug composition.
      *  \param const QString& cip drug cip identification .
      *  \return QList<C_CompositionItem> result list where
      *              C_CompositionItem.name()              is the composant name (DCI)
      *              C_CompositionItem.amount()            is the quantity
      *              C_CompositionItem.unit()              is the quantity unit
      *              C_CompositionItem.unit_plural()       is the accorded quantity unit if more than one
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
     //--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
     /*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
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
     int isStupefiant ( const QString &cip );
     //-------------------------------- code_ATC_To_Name ------------------------------------------------------
     /*! \brief return if ATC libelle from code ATC
      *  \param const QString &code      code ATC .
      *  \return QString return libelle
      */
     QString code_ATC_To_Name(const QString &code);
     //-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
     /*! \brief return the drug route from cip.
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString with the drug route
      */
     QString   getDrugRouteFromCIP ( const QString &cip);
     //-------------------------------- getPosologiesFromCIP ------------------------------------------------------
     /*! \brief return posologies list as posologie grammar QString
      *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
      *  \param const QString &cipOrGrammar is cip code or if len > 20 it can be a string with grammar allready fill with name, dci, cip basics informations from product etc...
      *  \return QStringList with grammar QString list
      */
     QStringList   getPosologiesFromCIP (C_PatientCtx *pC_PatientCtx, const QString &cipOrGrammar);
     //-------------------------------- getInteractionCim10 ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QStringList &cim10List cim10 pathologies list .
      *  \param QString drugName (default empty) the drug name.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     QList<C_InterResultPair> getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  = ""  );
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
     QList<C_InterResultPair> getInteractionRedundancy(const QStringList &edit_cipList, C_PatientCtx *pC_PatientCtx);
     //-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
     /*! \brief test compatibility with maximum and minimun authorized dose and duration from a drug and patient context.
      *  \param C_PatientCtx *pC_PatientCtx
      *  \param const QString &grammar prescription for this drug
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     QList<C_InterResultPair>  getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar);
     //-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
     /*! \brief return interactions xml results from a cip list.
      *  \param const QStringList &cipList      drugs CIP codes list to test.
      *  \param QString drugName (default empty) the drug name.
      *  \return result interaction pair list
      */
     QList<C_InterResultPair> getInteractionPairFromCipList ( const QStringList &cipList);
     //-------------------------------- getInteractionFromCipList ------------------------------------------------------
     /*! \brief return interactions xml results from a cip list.
      *  \param const QStringList &cipList      drugs CIP codes list to test.
      *  \return QStringList where each line is a xml result interaction
      */
     // QStringList   getInteractionFromCipList ( const QStringList &cipList);
     //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QString &age patient age .
      *  \param const QString &sexe patient sexe F-M-I.
      *  \param const QString &poids patient poids .
      *  \param const QString &taille patient size .
      *  \param const QString &amenorrhee number in weks
      *  \param const QString &clairance in ml/min
      *  \param int   allaitement in number of days
      *  \param bool sport if true particular attention for doping drugs is required  .
      *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
      *  \param QString drugName (default empty) the drug name.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
      QList<C_InterResultPair>        getInteractionAgeSexePoidsSportVigil( const QString &cip,
                                                                            const QString &age ,
                                                                            const QString &sexe,
                                                                            const QString &poids,
                                                                            const QString &taille,
                                                                            const QString &amenorrhee,
                                                                            const QString &clairance,
                                                                            int   allaitement,
                                                                            bool  sport = true,
                                                                            bool  vigil = true,
                                                                            QString drugName  = ""  );

     //....................... classification ATC .....................................
     int           nextATC_Code(QString &codeAtc, QString &libelle);

     //.................. liste des medicaments ................
     long          drugsList_Count();
     int           getDrugList_Start();
     int           getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord);
     //------------------------------ getRefundedIndications ------------------------------------------
     /*! \brief select indications informations for a drug.
      *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
      *  \param const QString &code_type  code typ as giving by the drugs BDM
      *  \return QList <C_BDM_IndicationRecord> indications result list
      */
     QList <C_BDM_IndicationRecord> getRefundedIndications( const QString &code_indication,   const QString &code_type );
     //------------------------------ getIndicationsFromText ------------------------------------------
     /*! \brief select indications informations for a drug.
      *  \param const QString &text      text from wich we want indications datas.
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
      *                                  C_BDM_PluginI::REFUND      we check if indications are linked to refund ( m_refundLink will contains price)
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
     //------------------------------ getMonographieFromCIP ------------------------------------------
     /*! \brief select monographie for a drug.
      *  \param const QString &cip            code CIP of the drug from wich we want datas.
      *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
      *  \return QString contains monographie
      */
     QString             getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ );
     //-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
     /*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres a cafe etc...)
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString with unit form
      */
     QString             getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName /* = 0 */);
     //-------------------------------- getFormeFromCIP ------------------------------------------------------
     /*! \brief return drug presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QString with presentation form
      */
     QString   getFormeFromCIP ( const QString &cip) ;

     ///////////////////////////////////////////////////////// PRIVATE /////////////////////////////////////////////////////////////////////
 private:
     //------------------------------------ getAuthorizedPrescriptors -------------------------------
     /*! \brief get authorized prescriptors list.
      *  \param  QString      code_produit  datasemp prouct code
      *  \return QStringList  with all authorized prescriptors list
      */
     QStringList                 getAuthorizedPrescriptors(const QString &code_produit);
     //------------------------------------ getAuthorizedPrescriptorsFromCIP -------------------------------
     /*! \brief  get authorized prescriptors list from CIP.
      *  \param  QString      cip  can be : '1234','1232' or 1234 or ('1234','1235')
      *  \return QMap <int,C_PrescriptionRestreinte>  with all authorized prescriptors\
      *          QMap list  where key is cip code and value is C_PrescriptionRestreinte class where\
      *          cip code, drug name, and prescriptors list are filled
      */
     QMap<int, C_PrescriptionRestreinte> getAuthorizedPrescriptorsFromCIP(const QString &cip);
     //------------------------------------ addCollapsibleJSPanel -------------------------------
     /*! \brief create  hmtl block which is a collapsible panel which works with JS SpryCollapsiblePanel.js.
      *  \param int   index of panel (for identification anchor).
      *  \param QString &date           it will contains date        of panel.
      *  \param QString &level          it will contains level       of SMR ASM if present (right information).
      *  \param QString &indication     it will contains indication  red text .
      *  \param QString &descriptif     it will contains description which is collapsible .
      *  \param QString &codeCimx       it will contains code CIM10  of SMR ASM if present.
      *  \param QString &path           path of the JS script and images.
      *  \param QString &javaS_CreatePanel_Body      JS instanciation panels Widget list on wich append panel instance.
      *  \return QString which is html block to append.
      */
     QString  addCollapsibleJSPanel       ( int   index,
                                            const QString &date,
                                            const QString &level,
                                            const QString &libelle,
                                            const QString &codeCimx,
                                            const QString &indication,
                                            const QString &path,
                                            QString &descriptif,
                                            QString &javaS_CreatePanel_Body ,
                                            const QString &className  = "CollapsiblePanelTab" );
     //-------------------------------- getDrugRoutePosoOnT2M_T15 ------------------------------------------------------
     /*! \brief return drug route
      *  \param const QString &t2Pf0 key from T2P.f0 (posologie table)
      *  \return  drug route
      */
     QString  getDrugRoutePosoOnT2M_T15 (const QString &t2Pf0);
     //-------------------------------- get_LibelleT24 ------------------------------------------------------
     /*! \brief   return libelle from table t24
      *  \param   const QString &t24f0 key from t24.f0
      *  \param   int quForPlurial if > 1 and plurial form exists, it's will be returned
      *  \return  libelle
      */
     QString  get_LibelleT24 (const QString &t24f0, int quForPlurial=0);
     //-------------------------------- getPhysioStateForThisPosoOnT2L_T2A ------------------------------------------------------
     /*! \brief return posology's sex
      *  \param const QString &t2Pf0 key from T2P.f0 (posologie table)
      *  \param QString &infValue  inferior value will be returned in this
      *  \param QString &infValue  superior value will be returned in this
      *  \param QString &codeUnit  unit code  will be returned in this
      *  \param QString &unit      unit will be returned in this
      *  \return  property type (age, poids,sexe ...)
      */
     QString   getPhysioStateForThisPosoOnT2L_T2A (const QString &t2Pf0, QString &infValue, QString &supValue, QString &codeUnit, QString &unitLibelle);

     //......... monographie .....................................
     void      Datasemp_SetHTMLFontProperty(const QString &family, const QString &titleSize, const QString &chapitreSize, const QString &paragraphSize, const QString &normalText);
     QString   Datasemp_GetMonographie(const QString &code_UV);
     QString   Datasemp_GetMonographieFromCodeProduit(const QString &code_produit);
     QString   Datasemp_SGML_to_html( QString& input, QString  name   = QString::null );
     void      Datasemp_Xml_to_html( QString& input, QString  name  );
     void      Datasemp_Xml_ListChild(QString& input, const QDomElement &parentElement, int &xmlDepth );
     //........ tools ..............................................................................................................
     int       Datasemp_isThisInteractionPresentInListe(int cipToExclude, int itCode, const QMap <int, INTER_STRUCT> &cip_interMultiMap, INTER_STRUCT &inter_act);
     //------------------------------ Datasemp_get_CIP_From_DatasempIndicationCode ------------------------------------------
     /*! \brief select cip list from datasemp indication code
      *  \param const QString &code     datasemp indication code from wich we want datas.
      *  \return QStringList cip result list
      */
     QStringList Datasemp_get_CIP_From_DatasempIndicationCode(const QString &code , bool codesAreInList  = false );
     //------------------------------ Datasemp_get_CIP_From_CIM10 ------------------------------------------
     /*! \brief select cip list from cim10 code
      *  \param const QString &code      cim10 code from wich we want datas.
      *  \return QStringList cip result list
      */
     QStringList Datasemp_get_CIP_From_CIM10(const QString &code);
     //------------------------------ Datasemp_get_CIP_From_ASMR_SMR_Code ------------------------------------------
     /*! \brief select cip list from  ASMR SMR code.
      *  \param const QString &code      Datasemp code ASMR or SMR from wich we want datas.
      *  \param const QString &typCode =="DS_A" for ASMR code or typCode =="DS_S" for SMR code
      *  \return QStringList cip result list
      */
     QStringList Datasemp_get_CIP_From_ASMR_SMR_Code(const QString &code, const QString &typCode );

     //-------------------------------- Datasemp_get_Cim10_InterResultPair_Map ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QStringList &cim10List cim10 pathologies list .
      *  \param QString drugName (default empty) the drug name.
      *  \return QMap<QString, C_InterResultPair> result interaction pair QMap (key is cip)
      */
     QMap<QString, C_InterResultPair> Datasemp_get_Cim10_InterResultPair_Map(const QString &cip, const QStringList &cim10List , QString drugName);
     //------------------------------ Datasemp_getASMR_SMR_FromCIP ------------------------------------------
     /*! \brief select indications ASMR SMR informations for a drug.
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \param const QString &filter   filter wanted datas  a combination | of this flags
      *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
      *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
      *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
      *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
      *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
      *  \return QList <C_BDM_IndicationRecord> indications result list
      */
     QList <C_BDM_IndicationRecord> Datasemp_getASMR_SMR_FromCIP(const QString &cip, int filter );
     //------------------------------ Datasemp_getIndications_FromCIP ------------------------------------------
     /*! \brief select indications ASMR SMR informations for a drug.
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \param const QString &filter   filter wanted datas  a combination | of this flags
      *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
      *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
      *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
      *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
      *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
      *                                  C_BDM_PluginI::REFUND      we check if indications are linked to refund ( m_refundLink will contains price)
      *  \param QString *codeIndicList = 0   if not null datasemp indications codes list will be returned as '1223','2365','987'
      *  \return QList <C_BDM_IndicationRecord> indications result list
      */
     QList <C_BDM_IndicationRecord> Datasemp_getIndications_FromCIP(const QString &cip, int filter , QString *codeIndicList = 0);
     //------------------------------ Datasemp_get_Remboursables_Indic_From_CIP ------------------------------------------
     /*! \brief select refunds indications list for a drug.
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \return QMap <QString QString> key is indication code, value is price
      */
     QMap <QString, QString> Datasemp_get_Remboursables_Indic_From_CIP( const QString &cip );
     //------------------------------ Datasemp_get_CIM10_From_Datasemp_Indic ------------------------------------------
     /*! \brief select CIMX indications from datasemp indication code
      *  \param const QString &cip      code CIP of the drug from wich we want datas.
      *  \param const QString &code     datasemp indication code CIP of the drug from wich we want datas.
      *  \return QList <C_BDM_IndicationRecord> indications result list
      */
     QList <C_BDM_IndicationRecord> Datasemp_get_CIM10_From_Datasemp_Indic(const QString &cip, const QString &code );

     //--------------------------- Datasemp_isThisCim10Code_in_Cim10List -----------------------------------------------
     /*! \brief return if a cim10 code is in cim10 patient list.
      *  \param const QString& cim10 cim10 code to test presence in Cim10List patient codes .
      *  \param const QStringList &cim10List cim10 patient pathologies list .
      *  \return bool true if code already present false otherwise
      */
     bool           Datasemp_isThisCim10Code_in_Cim10List(const QString &cim10, const QStringList &Cim10List);
     //-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
     /*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
      *         if there is not restrictive prescription for this CIP  isValid() is false;
      *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
      *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
      *  \return QList<C_PrescriptionRestreinte>
      */
     QList<C_PrescriptionRestreinte> getPrescriptionRestreinteFromCIP(const QString &cip);
     //------------------------------ getMonographieFromCIP ------------------------------------------
     /*! \brief return normalised ASMR number betwen  0 to 4 from datasem level
      *  \param const QString &level            datasem level
      *  \return QString contains normalised level
      */
     QString        Datasemp_ASMR_Level_To_level ( const QString &level);
     //------------------------------ Datasemp_get_Father_From_CIM10 ------------------------------------------
     /*! \brief return ascendant cimX code from a cimX code
      *  \param const QString &code  cimX code from wich we want father
      *  \return QString father cimX code
      */
     QString        Datasemp_get_Father_From_CIM10(const QString &code);

     //--------------------------- get_SubstancePere -----------------------------------------------------------
     /*! \brief return father libelle or a substance
      *  \param const QString code from substance.
      *  \return libelle of father substance or empty if not.
     */
     QString get_SubstancePere(const QString &code);
     //-------------------------------- convertDureeQmaxToMinutes -------------------------------------------------
     /*! \brief times periods values for posology max are in textual format. To numericals controls there must be converted in numerical values
      *  \param const QString& perioForQuMax textual period .
      *  \return int numerical value
      */
     int  convertDureeQmaxToMinutes(const QString &perioForQuMax, QString *valueTps=0);
     bool           alert_If_IN_Limits(int level);
     QString        Datasemp_GetBaseVersion( QString *numEdition, QString *packBCArrete =0, QString *packIRArrete=0);
     QString        Datasemp_LevelToLevelStr(int niveau);
     int            Datasemp_LevelToLevelInt(int niveau);
     QString        Utf8_Query(QSqlQuery &cur, int field);
     QString        code_UV_to_dispVilleHopital(const QString &cip_code);
     QString        code_CPG_to_code_UCD(const QString &codeCPG);
     QString        code_CPG_to_StatutGenerique(const QString &codeCPG);
     QStringList    code_CPG_to_DCI_1_2_3(const QString &codeCPG);
     QString        convertPrixDecimalToCts(const QString &prix);
     double         codePeriodToDays (int code_period, double qu);
     QString        drugListMakeSQLFilter();

 private:
     QString m_dataSourceVersion;
     bool    m_firstTimeGetNextSmrInfoForCIP;
     QString m_pathAppli;
     QString m_pathIni;
     QString m_datasIni;
     QMap <QString, QString>           m_PgrMap;
     QMap <QString, C_InterResultPair> m_interCIP_cache;
     QString m_FontFamily;
     QString m_FontTitleSize;
     QString m_FontchapitreSize;
     QString m_FontparagraphSize;
     QString m_FontnormalTextSize;

     QSqlQuery *m_pQSqlQuery;
     int        m_position;
signals:

public slots:
    void Slot_On_CursorError ( const QSqlQuery &qsqlquery, const QString &from, const QString &sql_query, const QString &file, int line  );
};

//================================== C_GetPosologieEPP_CUR   -A0-   ==================================
/***************************************************************************
*   Nom           :  C_GetPosologieEPP_CUR
-----------------------------------------------------------------------------
*   Date creation : 20/04/2014
-----------------------------------------------------------------------------
*   Description   : cette procedure nous renvoie le terrain
*                   correspondant au code d'une posologie
****************************************************************************/
/*........................ usage .......................................
    C_GetPosologieEPP_CUR  C_GetPosologieEPP ( database(), this );
    while ( C_GetPosologieEPP.next( t2Pf0 ) )
          { html += tr("<br><b> C_GetPosologieEPP : </b>") + C_GetPosologieEPP.TYPE()
                   + "<br/>.                 inf Value : " + C_GetPosologieEPP.TYPE_LIBELLE()
                   + "<br/>.                 inf Value : " + C_GetPosologieEPP.INF_VALUE()
                   + "<br/>.                 sup Value : " + C_GetPosologieEPP.SUP_VALUE();
                   + "<br/>.                 unit code : " + C_GetPosologieEPP.UNIT_CODE();
                   + "<br/>.              unit libelle : " + C_GetPosologieEPP.UNIT_LIBELLE();
                   + "<br/>.               descripteur : " + C_GetPosologieEPP.m_DESCRIPTEUR(); // (Valeurs possibles - A:Allaitement - F:Sexe féminin - M:Sexe masculin)
          }
*/
class C_GetPosologieEPP_CUR : public QObject, public QSqlQuery
{
   Q_OBJECT
public:
    C_GetPosologieEPP_CUR(QSqlDatabase db, QObject *parent)
          : QObject(parent), QSqlQuery(db)
        { m_isNextData = 0;
          if (parent)
             {connect( this,   SIGNAL (Sign_onError        ( const QSqlQuery &, const QString &, const QString &, const QString &, int  )),
                       parent,  SLOT  (Slot_On_CursorError ( const QSqlQuery &, const QString &, const QString &, const QString &, int  )) );
             }
        }
    int next(const QString &t2Pf0)
    {   if ( m_isNextData == 0 )
           {  QString sql_query =  QString ( " SELECT   \n"
                                             "  t2L.f0  \n"   // 0  property code (age, poids taille surface etc...)
                                             " ,t2A.f1  \n"   // 1  property libelle
                                             " ,t2L.f1  \n"   // 2  inferior value
                                             " ,t2L.f2  \n"   // 3  superior value
                                             " ,t2L.f3  \n"   // 4  unit code
                                             " ,t24.f1  \n"   // 5  unit libelle
                                             " ,t2L.f6  \n"   // 6  descripteur (Valeurs possibles - A:Allaitement - F:Sexe féminin - M:Sexe masculin)
                                             " FROM     \n"
                                             "  t2L,    \n"
                                             "  t24,    \n"
                                             "  t2A     \n"
                                             " WHERE    t2L.f4='%1'   \n"
                                             " AND      t2L.f3=t24.f0 \n"
                                             " AND      t2L.f0=t2A.f0 \n").arg(t2Pf0);
              exec(sql_query);
              if ( ! isActive() )
                 { emit Sign_onError(*this , tr("<font color=#ff0000><b>ERREUR  : C_GetPosologieEPP_CUR() </b></font>"), sql_query, __FILE__, __LINE__  );
                   return 0;
                 }
           }
        if (QSqlQuery::next())
           {  ++m_isNextData;
              m_TYPE         = value(0).toInt();
              m_TYPE_LIBELLE = value(1).toString();
              m_INF_VALUE    = value(2).toString();
              m_SUP_VALUE    = value(3).toString();
              m_UNIT_CODE    = value(4).toString();
              m_UNIT_LIBELLE = value(5).toString();
              m_DESCRIPTEUR  = value(6).toString().toLatin1()[0];
              return m_isNextData;
           }
        m_isNextData = 0;
        return 0;
    }
    int TYPE()             {return m_TYPE;}
    QString TYPE_LIBELLE() {return m_TYPE_LIBELLE;}
    QString INF_VALUE()    {return m_INF_VALUE;}
    QString SUP_VALUE()    {return m_SUP_VALUE;}
    QString UNIT_CODE()    {return m_UNIT_CODE;}
    QString UNIT_LIBELLE() {return m_UNIT_LIBELLE;}
    char    DESCRIPTEUR()  {return m_DESCRIPTEUR;}
 signals:
     void Sign_onError(const QSqlQuery &qsqlquery, const QString &from, const QString &sql_query, const QString &file, int line);
 private:
     //.......... procedure datas .............................
     int     m_isNextData;
     //.......... cursor datas ...................
     int     m_TYPE;
     QString m_TYPE_LIBELLE;
     QString m_INF_VALUE;
     QString m_SUP_VALUE;
     QString m_UNIT_CODE;
     QString m_UNIT_LIBELLE;
     char    m_DESCRIPTEUR;
};
#endif // C_BDM_DATASEMPPLUGIN_H
