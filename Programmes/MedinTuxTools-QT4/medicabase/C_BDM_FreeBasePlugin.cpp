
//-------------------------------- C_BDM_BaseModelePlugin ------------------------------------------------------
C_BDM_BaseModelePlugin::C_BDM_BaseModelePlugin(QObject *parent = 0)(const QString &confDatas, const QString &prefixLabel, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
: C_BaseCommon(parent) , C_BDM_PluginI()
{ Q_UNUSED (confDatas);
  Q_UNUSED (prefixLabel);
  Q_UNUSED (logWidget);
  Q_UNUSED (pQProgressBar);
  m_dataSourceVersion             = "09052012";  // TODO  en attendant le numero provenant de la BD datasemp
  m_lasError                      = "";
}
//-------------------------------- ~C_BDM_BaseModelePlugin ------------------------------------------------------
 ~C_BDM_BaseModelePlugin::C_BDM_BaseModelePlugin()
{
    
}
//-------------------------------- init ------------------------------------------------------
int C_BDM_BaseModelePlugin::init ( const QString &confData     , const QString &prefixLabel)
{
    
}
//-------------------------------- setLogWidget ------------------------------------------------------
void C_BDM_BaseModelePlugin::setLogWidget      ( QTextEdit *logWidget        )
{
    
}
//-------------------------------- setProgressWidget ------------------------------------------------------
void C_BDM_BaseModelePlugin::setProgressWidget ( QProgressBar *pQProgressBar )
{
    
}
//-------------------------------- progressWidget ------------------------------------------------------
QProgressBar* C_BDM_BaseModelePlugin::progressWidget()
{ return C_BaseCommon::progressWidget();
}
//-------------------------------- dataSourceVersion ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::dataSourceVersion()
{ return m_dataSourceVersion;
}

//-------------------------------- lang ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::lang()
{ return "fr";
}
//-------------------------------- owner ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::owner()
{ return "OWN";
}
//-------------------------------- getLastError ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::getLastError()
{return m_lasError;
}
//-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
/*! \brief return if  plugin is HAS certified
 *  \return return HAS certified status
 */
bool C_BDM_BaseModelePlugin::is_plugin_HAS_certified()
{ return false;
}
//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_BaseModelePlugin::plugin_name();
//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_BaseModelePlugin::plugin_icon();
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
 *  \param const QString &amenorrhÈe number in weeks
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
QString   getFormeFromCIP ( const QString &cip) ;private:
