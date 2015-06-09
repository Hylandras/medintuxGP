#ifndef C_BDM_BaseModelePlugin_H
#define C_BDM_BaseModelePlugin_H

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

class QDomElement;
class C_BDM_BaseModelePlugin :  public  C_BaseCommon ,  public C_BDM_PluginI
{
    Q_OBJECT

public:
    explicit C_BDM_BaseModelePlugin(const QString &confData, const QString &prefixLabel, QObject *parent = 0, QTextEdit *logWidget=0, QProgressBar *pQProgressBar=0);
    ~C_BDM_BaseModelePlugin();
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
     *  \return QList<C_InterResultPair> result list where
     *              C_CompositionItem.name()              is the composant name (DCI)
     *              C_CompositionItem.amount()            is the quantity
     *              C_CompositionItem.unit()              is the quantity unit
     *              C_CompositionItem.unit_plural()       is the accorded quantity unit if more than one
     */
     QList<C_CompositionItem> getCompositionFromCIP(const QString &cip);
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
    //-------------------------------- getInteractionRedundancy ------------------------------------------------------
    /*! \brief return redundancy composition in a cip list of drugs.
     *  \param cipList QStringList &cipList drugs cip list .
     *  \return QList<C_InterResultPair> result interaction pair list
     */
    QList<C_InterResultPair> getInteractionRedundancy( const QStringList &cipList);
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
     *   each interaction line is a xml structure where :
     *   <itli>           </itli> delimits each interaction line structured as follows.
     *   <itli_code1>     </itli_code1>       delimits first  term code (if exists)
     *   <itli_typcode1>  </itli_typcode1>    delimits first  term codetyp examples "CIP ATC CIM10 CISP NONE AL PO"
     *   <itli_terme1>    </itli_terme1>      delimits first  term libelle text
     *   <itli_code2>     </itli_code2>       delimits second term code (if exists)
     *   <itli_typcode2>  </itli_typcode2>    delimits second term codetyp examples "CIP ATC CIM10 CISP NONE"
     *   <itli_terme2>    </itli_terme2>      delimits second term libelle text
     *   <itli_terme2>    </itli_terme2>      delimits second term libelle text
     *   <itli_texte>     </itli_texte>       delimits text interaction information
     *   <itli_ityp>      </itli_ityp>        delimits interaction type (RESERVED)
     *   <itli_level>     </itli_level>       interraction level
     */
    // QStringList   getInteractionFromCipList ( const QStringList &cipList);
    //-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
    /*! \brief return physiopath interactions from patient and drug.
     *  \param const QString& cip drug cip identification .
     *  \param const QString &age patient age .
     *  \param const QString &sexe patient sexe F-M-I.
     *  \param const QString &poids patient poids .
     *  \param const QString &taille patient size .
     *  \param const QString &amenorrhée number in weeks
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
    //------------------------------ getMonographieFromCIP ------------------------------------------
    /*! \brief select monographie for a drug.
     *  \param const QString &cip            code CIP of the drug from wich wee want datas.
     *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
     *  \return QString contains monographie
     */
    QString             getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ );
    //-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
    /*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres Ã  cafÃ© etc...)
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

    ///////////////////////////////////////////////////////// PRIVATE /////////////////////////////////////////////////////////////////////
private:
    //......... monographie .....................................
    void      Datasemp_SetHTMLFontProperty(const QString &family, const QString &titleSize, const QString &chapitreSize, const QString &paragraphSize, const QString &normalText);
    QString   Datasemp_GetMonographie(const QString &code_UV);
    QString   Datasemp_SGML_to_html( QString& input, QString  name   = QString::null );
    void      Datasemp_Xml_to_html( QString& input, QString  name  );
    void      Datasemp_Xml_ListChild(QString& input, const QDomElement &parentElement, int &xmlDepth );
    //........ tools ..............................................................................................................
    int       Datasemp_isThisInteractionPresentInListe(int cipToExclude, int itCode, const QMap <int, INTER_STRUCT> &cip_interMultiMap);
    //------------------------------ Datasemp_get_CIP_From_DatasempIndicationCode ------------------------------------------
    /*! \brief select cip list from datasemp indication code
     *  \param const QString &code     datasemp indication code from wich wee want datas.
     *  \return QStringList cip result list
     */
    QStringList Datasemp_get_CIP_From_DatasempIndicationCode(const QString &code , bool codesAreInList  = false );
    //------------------------------ Datasemp_get_CIP_From_CIM10 ------------------------------------------
    /*! \brief select cip list from cim10 code
     *  \param const QString &code      cim10 code from wich wee want datas.
     *  \return QStringList cip result list
     */
    QStringList Datasemp_get_CIP_From_CIM10(const QString &code);
    //------------------------------ Datasemp_get_CIP_From_ASMR_SMR_Code ------------------------------------------
    /*! \brief select cip list from  ASMR SMR code.
     *  \param const QString &code      Datasemp code ASMR or SMR from wich wee want datas.
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
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
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
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
     *  \param const QString &filter   filter wanted datas  a combination | of this flags
     *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
     *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
     *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
     *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
     *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
     *  \return QList <C_BDM_IndicationRecord> indications result list
     */
    QList <C_BDM_IndicationRecord> Datasemp_getIndications_FromCIP(const QString &cip, int filter );

    //------------------------------ Datasemp_get_CIM10_From_Datasemp_Indic ------------------------------------------
    /*! \brief select CIMX indications from datasemp indication code
     *  \param const QString &cip      code CIP of the drug from wich wee want datas.
     *  \param const QString &code     datasemp indication code CIP of the drug from wich wee want datas.
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
    //------------------------------ getMonographieFromCIP ------------------------------------------
    /*! \brief return normalised ASMR number between  0 to 4 from datasem level
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
    QMap <QString, QString> m_PgrMap;

    QString m_FontFamily;
    QString m_FontTitleSize;
    QString m_FontchapitreSize;
    QString m_FontparagraphSize;
    QString m_FontnormalTextSize;

    QSqlQuery *m_pQSqlQuery;
    int        m_position;
signals:

public slots:
};


#endif // C_BDM_BaseModelePlugin_H
