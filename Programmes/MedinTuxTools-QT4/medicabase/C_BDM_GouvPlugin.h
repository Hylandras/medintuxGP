#ifndef C_BDM_GouvPlugin_H
#define C_BDM_GouvPlugin_H

#include "../C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/medicabase/C_InterResultPair.h"
#include <QObject>
#include <QSqlQuery>

typedef struct  { QString m_date;
                  QString m_level;
                  QString m_mecanism;
                  QString m_conduite;
                  int     m_isValid;
                } INTER_BDM;

typedef struct  { QString  m_cip;
                  QString  m_product_code;
                  QString  m_product;
                  QString  m_drugName;
                } PRODUCT_CIP;

class C_BDM_GouvPlugin :  public  C_BaseCommon ,  public C_BDM_PluginI
{
    Q_OBJECT

public:
    explicit C_BDM_GouvPlugin(const QString &confData, const QString &prefixLabel, QObject *parent = 0, QTextEdit *logWidget=0, QProgressBar *pQProgressBar=0);
    ~C_BDM_GouvPlugin();
    //------------------ INTERFACE VIRTUELLE -----------------
    int           init              ( const QString &confData     , const QString &prefixLabel);
    void          setLogWidget      ( QTextEdit *logWidget        );
    void          setProgressWidget ( QProgressBar *pQProgressBar );
    QProgressBar* progressWidget();
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
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
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
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
     *  \return QString with the drug route
     */
    QString   getDrugRouteFromCIP ( const QString &cip);
    //-------------------------------- getPosologiesFromCIP ------------------------------------------------------
    /*! \brief return posologies list as posologie grammar QString
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
     *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
     *  \return QStringList with grammar QString list
     */
    QStringList   getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip );
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
    //-------------------------------- getInteractionRedundancy ------------------------------------------------------
    /*! \brief return redundancy composition in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionRedundancy( const QStringList &cipList, C_PatientCtx *pC_PatientCtx);
    //-------------------------------- getAllergiesFromCipList ------------------------------------------------------
    /*! \brief return Allergies interactions in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx);
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

    //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param const QString &age patient age .
     *  \param const QString &sexe patient sexe F-M-I.
     *  \param const QString &poids patient poids .
     *  \param const QString &taille patient size .
     *  \param const QString &amenorrhÈe number in weeks
     *  \param const QString &clairance in ml/min
     *  \param int   allaitement in number of days
     *  \param bool sport if true particular attention for doping drugs is required  .
     *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
     *  \param QString drugName (default empty) the drug name.
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair>       getInteractionAgeSexePoidsSportVigil( const QString &cip,
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
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
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
     *  \param const QString &code       indication code of the drugs from wich wee want datas.
     *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
     *  \return QStringList cip result list
     */
    QStringList getCIPFromIndication( const QString &code, const QString &codeType );
    //-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
    /*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
     *         if there is not restrictive prescription for this CIP  isValid() is false;
     *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
     *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
     *  \return QList<C_PrescriptionRestreinte>
     */
    QList<C_PrescriptionRestreinte> getPrescriptionRestreinteFromCIP(const QString &cip);
    //------------------------------ getMonographieFromCIP ------------------------------------------
    /*! \brief select monographie for a drug.
     *  \param const QString &cip            code CIP of the drug from wich wee want datas.
     *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
     *  \return QString contains monographie
     */
    QString             getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ );
    //-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
    /*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres √  caf√© etc...)
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
     *  \return QString with unit form
     */
    QString             getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName /* = 0 */);
    //-------------------------------- getFormeFromCIP ------------------------------------------------------
    /*! \brief return drug presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
     *  \return QString with presentation form
     */
    QString   getFormeFromCIP ( const QString &cip) ;
    ///////////////////////////////////////////// PRIVATE EXTRA PLUGIN //////////////////////////////////////////////////////////////////
private:
    QString                  code_CIP_to_ATC(const QString &cip);
    QString                  code_CIS_to_StatutGenerique(const QString &code_cis);
    QStringList              code_CIS_to_DCI_1_2_3(const QString &code_cis, int completeTo3 = 1);
    QString                  convertPrixDecimalToCts(const QString &prix);
    QStringList              tryToFindProduct(QString produit);
    QList<C_InterResultPair> getInteractionList(const PRODUCT_CIP &produit1, const PRODUCT_CIP &produit2,const QString &pk_prod1, const QString &pk_prod2);
private:
   QString       m_dataSourceVersion;
   QString       m_pluginName;
   QString       m_pathAppli;
   QString       m_pathIni;
   QString       m_datasIni;

   QSqlQuery    *m_pQSqlQuery;
   int           m_position;
signals:
public slots:

};


#endif // C_BDM_GouvPlugin_H
