
#include "C_BDM_GenericPlugin.h"
#include <QDir>
#include <QSqlDatabase>

//-------------------------------- C_BDM_GenericPlugin ------------------------------------------------------

C_BDM_GenericPlugin::C_BDM_GenericPlugin(const QString &confDatas, const QString &prefixLabel, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
: C_BaseCommon(parent) , C_BDM_PluginI()
{ m_dataSourceVersion             = "No database drugs";        // TODO  en attendant le numero provenant de la BD datasemp
  m_pluginName                    = "C_BDM_GenericPlugin";      // TODO  change this name
  setInitialisedSate(false);

  if (logWidget)        { C_Log::setLogWidget(logWidget);
                        }
  if (pQProgressBar)    { C_BaseCommon::setProgressWidget(pQProgressBar);
                        }
  if ( init (confDatas, prefixLabel) == 0 )
     { outMessage(  C_BaseCommon::getLastError() );
     }
}

//-------------------------------- ~C_BDM_GenericPlugin ------------------------------------------------------
C_BDM_GenericPlugin::~C_BDM_GenericPlugin()
{if (m_pathAppli.length()) m_pathAppli = "";
}

//-------------------------------- init ------------------------------------------------------
int C_BDM_GenericPlugin::init ( const QString &confData     , const QString &prefixLabel)
{   QString errMess           =   "";
    m_pathAppli               =   CGestIni::Param_ReadUniqueParam(confData.toLatin1(),"BDM_Configuration", "PathAppli");
    m_pathIni                 =   CGestIni::Param_ReadUniqueParam(confData.toLatin1(),"BDM_Configuration", "PathIni");
    setInitialisedSate(0);
    if (m_pathAppli.length()==0)
       {setLastError (tr("%1::init() BDM_Configuration/PathAppli is empty").arg(plugin_name()));
        return 0;
       }
    if (!QFile::exists ( m_pathAppli ))
       {setLastError (tr("%1::init() BDM_Configuration/PathAppli don't exists").arg(plugin_name()));
        return 0;
       }
    if (m_pathIni.length()==0)
       {setLastError (tr("%1::init() BDM_Configuration/PathIni is empty").arg(plugin_name()));
        return 0;
       }
    if (QDir::isRelativePath ( m_pathIni )) m_pathIni = m_pathIni.prepend(m_pathAppli);
    m_pathIni = QDir::cleanPath ( m_pathIni );
    if (!QFile::exists ( m_pathIni ))
       {setLastError (tr("%1::init() BDM_Configuration/PathIni don't exists").arg(plugin_name()));
        return 0;
       }
    m_datasIni       =  CGestIni::Param_UpdateFromDisk(m_pathIni);
    if (m_datasIni.length()==0)
       {setLastError (tr("%1::init() m_datasIni is empty").arg(plugin_name()));
        return 0;
       }
    //............ ICI se fait la connexion a la base Commerciale du plugin............................................................
    //             ce qui lui permettra par la suite d'acceder aux requetes SQL qui lui sont propres
    //             pour cela dans le m_datasIni devrait se retrouver les informations suivantes :
    //
    //               [Connexion_Datasemp]
    //                          ^^^^^^^^
    //                              |_______ nom du plugin tel que delivre par plugin_name()
    //
    //               Parametres = QMYSQL3 , DatasempTest , root ,  , localhost , 3306
    //                                         ^
    //                                         |_____________ nom de la base de donnee de la base commerciale
    //
    //               TimeToReconnect   = 3600000
    //                                       ^
    //                                       |_____________ temps en microsecondes avant de lancer une requete sql de reconnexion
    //
    //               PathImageSource   = ../../drtux/bin/Ressources/ImgVidalDgl
    //                                       ^
    //                                       |_____________ chemin des images pour afficher la monographie

    QString driver;             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
    QString dataBaseToConnect;  // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
    QString user;               // = "root"
    QString password;           // = ""
    QString hostname;           // = "localhost"
    QString port;               // = "3306"
    QString section                = "Connexion_"+plugin_name();
    QString timeToReconnect        = "";

    if (CGestIni::Param_ReadParam(  m_datasIni.toLatin1(), section.toLatin1(), "Parametres", &driver, &dataBaseToConnect, &user, &password, &hostname, &port) !=0 )  // zero = pas d'erreur
       { setLastError (tr("%1::init() error in dbi_init_database()\n"
                          "CGestIni::Param_ReadParam() may be : key 'Parametres' omitted in section '%2'").arg(plugin_name(),"Connexion_"+plugin_name()));
         return 0;
       }
     if (CGestIni::Param_ReadParam(  m_datasIni.toLatin1(), section.toLatin1(), "TimeToReconnect", &timeToReconnect) !=0 )  // zero = pas d'erreur
       { timeToReconnect = QString::number(DB_TIME_TO_RECONNECT);
       }
    if ( ! BaseConnect( prefixLabel + plugin_name() , driver, dataBaseToConnect, user, password,  hostname, port.toInt(), timeToReconnect.toInt(), &errMess ) )
       { setLastError (tr("%1::init() error in BDM connexion BaseConnect()\n%2").arg(plugin_name(), errMess));
         return 0;
       }
    if (dbi_init_database("dbi_"+plugin_name(), prefixLabel, "GB", this)==0)
       { setLastError (tr("%1::init() error in dbi_init_database()\n").arg(plugin_name()));
         return 0;
       }
    setInitialisedSate(1);
    return 1;
}

//-------------------------------- setLogWidget ------------------------------------------------------
void C_BDM_GenericPlugin::setLogWidget      ( QTextEdit *logWidget )
{C_Log::setLogWidget(logWidget);
}
//-------------------------------- setProgressWidget ------------------------------------------------------
void C_BDM_GenericPlugin::setProgressWidget ( QProgressBar *pQProgressBar )
{C_BaseCommon::setProgressWidget(pQProgressBar);
}

//-------------------------------- progressWidget ------------------------------------------------------
QProgressBar* C_BDM_GenericPlugin::progressWidget()
{ return C_BaseCommon::progressWidget();
}
//-------------------------------- dataSourceVersion ------------------------------------------------------
QString  C_BDM_GenericPlugin::dataSourceVersion()
{ return m_dataSourceVersion;
}

//-------------------------------- lang ------------------------------------------------------
QString  C_BDM_GenericPlugin::lang()
{ return "fr";
}
//-------------------------------- owner ------------------------------------------------------
QString  C_BDM_GenericPlugin::owner()
{ return "OWN";
}
//-------------------------------- getLastError ------------------------------------------------------
QString  C_BDM_GenericPlugin::getLastError()
{return C_BaseCommon::getLastError();
}
//-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
/*! \brief return if  plugin is HAS certified
 *  \return return HAS certified status
 */
bool C_BDM_GenericPlugin::is_plugin_HAS_certified()
{ return false;
}
//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_GenericPlugin::plugin_name()
{ return m_pluginName ;
}

//---------------------------------------- get_database -------------------------------------------------------
/*! \brief  return active database. This facility is to do extended SQL querys which are not in API
 *  \return  database
 */
QSqlDatabase C_BDM_GenericPlugin::get_database()
{ return database();
}

//-------------------------------- plugin_image_source ------------------------------------------------------
/*! \brief  return image source url for monographies
 *  \return return image source url for monographies
 */
QString C_BDM_GenericPlugin::plugin_image_source()
{   QString pathFile = CGestIni::Param_ReadUniqueParam(m_datasIni.toLatin1(),QString("Connexion_"+plugin_name()).toLatin1(), "PathImageSource");
    if ( QDir::isRelativePath ( pathFile ) ) pathFile  = QDir::cleanPath ( pathFile.prepend(m_pathAppli) );
    if (!pathFile.endsWith("/")) pathFile += "/";
    return pathFile;
}
//-------------------------------- getCompositionFromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QList<C_CompositionItem> result list where
 *              C_CompositionItem.name()              is the composant name (DCI)
 *              C_CompositionItem.amount()            is the quantity
 *              C_CompositionItem.unit()              is the quantity unit
 *              C_CompositionItem.unit_plural()       is the accorded quantity unit if more than one
 */
QList<C_CompositionItem> C_BDM_GenericPlugin::getCompositionFromCIP(const QString &cip)
{ Q_UNUSED(cip);
  QList <C_CompositionItem> result;
  return result;
}
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
QList<C_ExcipientItem> C_BDM_GenericPlugin::getExcipientsFromCIP(const QString &cip)
{   Q_UNUSED(cip);
    QList<C_ExcipientItem>        result;
    return result;
}
//-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QStringList result list with dci composants
 */
QStringList C_BDM_GenericPlugin::get_DCI_list_FromCIP(const QString &cip)
{ Q_UNUSED(cip);
  QStringList retList;
  return retList;
}

//--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
/*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
 *  \param const QString cip is drug cip code.
 *  \param QString *rcp is pointer on QString to return RCP result if not zero .
 *  \return CIS code string
 */
QString C_BDM_GenericPlugin::get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp /* =0 */)
{ Q_UNUSED(cip);
  Q_UNUSED(rcp);
  return "";
}

//-------------------------------- isStupefiant ------------------------------------------------------
/*! \brief return if the product identified par Id is a stupefiant
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return 1 is stupefiant
 */
int C_BDM_GenericPlugin::isStupefiant ( const QString &cip )
{ Q_UNUSED(cip);
  return 0;
}

//-------------------------------- code_ATC_To_Name ------------------------------------------------------
/*! \brief return if ATC libelle from code ATC
 *  \param const QString &code      code ATC .
 *  \return QString return libelle
 */
QString C_BDM_GenericPlugin::code_ATC_To_Name(const QString &code)
{ Q_UNUSED(code);
  return "";
}

//-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
/*! \brief return the drug route from cip.
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with the drug route
 */
QString   C_BDM_GenericPlugin::getDrugRouteFromCIP ( const QString &cip)
{ Q_UNUSED(cip);
  return "";
}

//-------------------------------- getPosologiesFromCIP ------------------------------------------------------
/*! \brief return posologies list as posologie grammar QString
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
 *  \return QStringList with grammar QString list
 */
QStringList   C_BDM_GenericPlugin::getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip )
{ Q_UNUSED(cip);
  Q_UNUSED(pC_PatientCtx);
  return QStringList();
}

//-------------------------------- getInteractionCim10 ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QStringList &cim10List cim10 pathologies list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GenericPlugin::getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  /* = "" */  )
{
  Q_UNUSED(cip);
  Q_UNUSED(cim10List);
  Q_UNUSED(drugName);
  QList<C_InterResultPair> interResultPair;
  return interResultPair;
}
//-------------------------------- getInteractionPhysicoChimiques ------------------------------------------------------
/*! \brief return chimico physiques interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GenericPlugin::getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{ Q_UNUSED(cipList);
  Q_UNUSED(pC_PatientCtx);
  QList<C_InterResultPair>            interResultPairList;
  return interResultPairList;
}
//-------------------------------- getInteractionRedundancy ------------------------------------------------------
/*! \brief return redundancy composition in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GenericPlugin::getInteractionRedundancy( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{
    Q_UNUSED(pC_PatientCtx);
    Q_UNUSED(cipList);
    QList<C_InterResultPair> interResultPair;
    return interResultPair;
}
//-------------------------------- getAllergiesFromCipList ------------------------------------------------------
/*! \brief return Allergies interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GenericPlugin::getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{Q_UNUSED(cipList);
 Q_UNUSED(pC_PatientCtx);
 QList<C_InterResultPair> interResultPairList;
 return interResultPairList;
}
//-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
/*! \brief test compatibility with maximum and minimun authorized dose and duration from a drug and patient context.
 *  \param C_PatientCtx *pC_PatientCtx
 *  \param const QString &grammar prescription for this drug
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>  C_BDM_GenericPlugin::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)
{
  Q_UNUSED(pC_PatientCtx);
  Q_UNUSED(grammar);
  QList<C_InterResultPair> interResultPair;
  return interResultPair;
}
//-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
/*! \brief return interactions xml results from a cip list.
 *  \param const QStringList &cipList      drugs CIP codes list to test.
 *  \param QString drugName (default empty) the drug name.
 *  \return result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GenericPlugin::getInteractionPairFromCipList ( const QStringList &cipList)
{
  Q_UNUSED(cipList);
  QList<C_InterResultPair> interResultPair;
  return interResultPair;
}

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
QList<C_InterResultPair>  C_BDM_GenericPlugin::getInteractionAgeSexePoidsSportVigil( const QString &cip,
                                                                     const QString &age ,
                                                                     const QString &sexe,
                                                                     const QString &poids,
                                                                     const QString &taille,
                                                                     const QString &amenorrhee,
                                                                     const QString &clairance,
                                                                     int   allaitement,
                                                                     bool  sport      /* = true */ ,
                                                                     bool  vigil      /* = true */ ,
                                                                     QString drugName /* = "" */ )
{
  Q_UNUSED(cip);
  Q_UNUSED(age);
  Q_UNUSED(sexe);
  Q_UNUSED(poids);
  Q_UNUSED(taille);
  Q_UNUSED(amenorrhee);
  Q_UNUSED(clairance);
  Q_UNUSED(allaitement);
  Q_UNUSED(sport);
  Q_UNUSED(vigil);
  Q_UNUSED(drugName);
  QList<C_InterResultPair> interResultPair;
  return interResultPair;
}

//....................... classification ATC .....................................
int           C_BDM_GenericPlugin::nextATC_Code(QString &codeAtc, QString &libelle)
{   Q_UNUSED(codeAtc);
    Q_UNUSED(libelle);
    return 0;
}

//.................. liste des medicaments ................
long          C_BDM_GenericPlugin::drugsList_Count()
{ return 0;
}

int           C_BDM_GenericPlugin::getDrugList_Start()
{ return 0;
}

//------------------------------ getIndicationsFromCIP ------------------------------------------
int           C_BDM_GenericPlugin::getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord)
{ Q_UNUSED(rC_BDM_DrugListRecord);
  return 0;
}
//------------------------------ getRefundedIndications ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
 *  \param const QString &code_type  code typ as giving by the drugs BDM
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_GenericPlugin::getRefundedIndications( const QString &code_indication,   const QString &code_type )
{   Q_UNUSED(code_indication);
    Q_UNUSED(code_type);
    QList <C_BDM_IndicationRecord> retList;
    return    retList;
}

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
 QList <C_BDM_IndicationRecord> C_BDM_GenericPlugin::getIndicationsFromText( const QString &text,   int filter )
 { Q_UNUSED(text);
   Q_UNUSED(filter);
   QList <C_BDM_IndicationRecord> retList;
   return    retList;
 }
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
QList <C_BDM_IndicationRecord> C_BDM_GenericPlugin::getIndicationsFromCIP( const QString &cip,   int filter )
{
    Q_UNUSED(cip);
    Q_UNUSED(filter);
    QList <C_BDM_IndicationRecord>  indicationRecord;
    return indicationRecord;
}

//------------------------------ getCIPFromIndication ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code       indication code of the drugs from wich wee want datas.
 *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
 *  \return QStringList cip result list
 */
QStringList C_BDM_GenericPlugin::getCIPFromIndication( const QString &code, const QString &codeType )
{
    Q_UNUSED(code);
    Q_UNUSED(codeType);
    return QStringList();
}
//-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
/*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
 *         if there is not restrictive prescription for this CIP  isValid() is false;
 *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
 *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
 *  \return QList<C_PrescriptionRestreinte>
 */
QList<C_PrescriptionRestreinte> C_BDM_GenericPlugin::getPrescriptionRestreinteFromCIP(const QString &cip)
{   Q_UNUSED( cip );
    QList<C_PrescriptionRestreinte> retList;
    return retList;
}
//------------------------------ getMonographieFromCIP ------------------------------------------
/*! \brief select monographie for a drug.
 *  \param const QString &cip            code CIP of the drug from wich wee want datas.
 *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
 *  \return QString contains monographie
 */
QString   C_BDM_GenericPlugin::getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ )
{
    Q_UNUSED(cip);
    Q_UNUSED(dataMustBeReadInCache);
    return "";
}

//-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
/*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres √  caf√© etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with unit form
 */
QString    C_BDM_GenericPlugin::getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName /* = 0 */)
{
    Q_UNUSED(cip);
    Q_UNUSED(commercialName);
    return "";

}

//-------------------------------- getFormeFromCIP ------------------------------------------------------
/*! \brief return drug presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with presentation form
 */
QString   C_BDM_GenericPlugin::getFormeFromCIP ( const QString &cip)
{
    Q_UNUSED(cip);
    return "";
}

//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_GenericPlugin::plugin_icon()
{/* XPM */

    static char const * dossier_rouge_xpm[] =  {
    "20 20 197 2"
    "  	c None"
    ". 	c #9E5C5C"
    "+ 	c #C59494"
    "@ 	c #9B5D5D"
    "# 	c #B37070"
    "$ 	c #C17E7E"
    "% 	c #D09191"
    "& 	c #D89595"
    "* 	c #D99696"
    "= 	c #DA9C9C"
    "- 	c #DC9C9C"
    "; 	c #E19F9F"
    "> 	c #E69797"
    ", 	c #8E3F3F"
    "' 	c #E89C9C"
    ") 	c #E39393"
    "! 	c #E39292"
    "~ 	c #E19191"
    "{ 	c #E09090"
    "] 	c #E29090"
    "^ 	c #DB8B8B"
    "/ 	c #D29595"
    "( 	c #DC9696"
    "_ 	c #DB9292"
    ": 	c #DA8787"
    "< 	c #E28787"
    "[ 	c #934444"
    "} 	c #E38989"
    "| 	c #DB8181"
    "1 	c #DB8282"
    "2 	c #DC8484"
    "3 	c #C87878"
    "4 	c #A26262"
    "5 	c #D18F8F"
    "6 	c #DD9191"
    "7 	c #DA8383"
    "8 	c #E07C7C"
    "9 	c #8F3B3B"
    "0 	c #E28181"
    "a 	c #DB7979"
    "b 	c #DB7A7A"
    "c 	c #DD7B7B"
    "d 	c #925151"
    "e 	c #7F4747"
    "f 	c #DD9292"
    "g 	c #DB9393"
    "h 	c #DB7C7C"
    "i 	c #DE7070"
    "j 	c #8D3333"
    "k 	c #E37878"
    "l 	c #DB7171"
    "m 	c #DC7373"
    "n 	c #944C4C"
    "o 	c #834646"
    "p 	c #DA7272"
    "q 	c #DE6666"
    "r 	c #8B2E2E"
    "s 	c #E36F6F"
    "t 	c #DB6868"
    "u 	c #DB6969"
    "v 	c #DC6A6A"
    "w 	c #944646"
    "x 	c #834141"
    "y 	c #D28E8E"
    "z 	c #DB8E8E"
    "A 	c #DA6868"
    "B 	c #DE5B5B"
    "C 	c #E26666"
    "D 	c #DB5F5F"
    "E 	c #DB6060"
    "F 	c #DC6161"
    "G 	c #934040"
    "H 	c #843C3C"
    "I 	c #DD9393"
    "J 	c #DB8787"
    "K 	c #DA5E5E"
    "L 	c #DE5050"
    "M 	c #E15C5C"
    "N 	c #DB5656"
    "O 	c #DB5757"
    "P 	c #DC5757"
    "Q 	c #933A3A"
    "R 	c #843838"
    "S 	c #DB7E7E"
    "T 	c #DB5353"
    "U 	c #DD4545"
    "V 	c #E15454"
    "W 	c #DB4E4E"
    "X 	c #DB4F4F"
    "Y 	c #DC4F4F"
    "Z 	c #933434"
    "` 	c #843434"
    " .	c #DB7575"
    "..	c #DA4A4A"
    "+.	c #DE3B3B"
    "@.	c #E14B4B"
    "#.	c #DB4545"
    "$.	c #DB4646"
    "%.	c #DC4646"
    "&.	c #932E2E"
    "*.	c #842F2F"
    "=.	c #D38E8E"
    "-.	c #DD8D8D"
    ";.	c #DB6A6A"
    ">.	c #DB4040"
    ",.	c #DD3131"
    "'.	c #E54141"
    ").	c #DB3C3C"
    "!.	c #DB3D3D"
    "~.	c #DC3D3D"
    "{.	c #932828"
    "].	c #842929"
    "^.	c #D59393"
    "/.	c #DD8787"
    "(.	c #DB3636"
    "_.	c #DC2626"
    ":.	c #B73D3D"
    "<.	c #E03232"
    "[.	c #DB3333"
    "}.	c #DB3434"
    "|.	c #912222"
    "1.	c #801B1B"
    "2.	c #AB6262"
    "3.	c #DF8686"
    "4.	c #DB5555"
    "5.	c #DB2D2D"
    "6.	c #DC1D1D"
    "7.	c #EC8484"
    "8.	c #DC2D2D"
    "9.	c #DB2A2A"
    "0.	c #DB2B2B"
    "a.	c #DD2C2C"
    "b.	c #9E2020"
    "c.	c #801515"
    "d.	c #8F3535"
    "e.	c #DD7F7F"
    "f.	c #DB4A4A"
    "g.	c #DB2222"
    "h.	c #DC1515"
    "i.	c #2A1313"
    "j.	c #EAACAC"
    "k.	c #DD2626"
    "l.	c #DB2121"
    "m.	c #DB2323"
    "n.	c #DE2424"
    "o.	c #CC2121"
    "p.	c #821010"
    "q.	c #913434"
    "r.	c #DE7676"
    "s.	c #DB1818"
    "t.	c #DB0F0F"
    "u.	c #291313"
    "v.	c #E9A3A3"
    "w.	c #DD1D1D"
    "x.	c #DB1919"
    "y.	c #DB1A1A"
    "z.	c #DD1A1A"
    "A.	c #D01919"
    "B.	c #840C0C"
    "C.	c #902F2F"
    "D.	c #DD6D6D"
    "E.	c #DB0707"
    "F.	c #ED9292"
    "G.	c #281313"
    "H.	c #EBA19F"
    "I.	c #DD1414"
    "J.	c #DB1010"
    "K.	c #DB1111"
    "L.	c #DD1111"
    "M.	c #D10C0C"
    "N.	c #880202"
    "O.	c #952A2A"
    "P.	c #DD6565"
    "Q.	c #D90202"
    "R.	c #DF2626"
    "S.	c #2B1413"
    "T.	c #DD9BAA"
    "U.	c #DC0808"
    "V.	c #DA0000"
    "W.	c #D90000"
    "X.	c #DB0202"
    "Y.	c #DB2020"
    "Z.	c #7E2020"
    "`.	c #733535"
    " +	c #DE5C5C"
    ".+	c #D91111"
    "++	c #E03131"
    "@+	c #1B0F0F"
    "#+	c #B45C65"
    "$+	c #DF0808"
    "%+	c #E12A2A"
    "&+	c #E65454"
    "*+	c #EC7A7A"
    "=+	c #2C1C1C"
    "-+	c #E24848"
    ";+	c #E24646"
    ">+	c #E68989"
    "                                . +     "
    "                @ # $ % & * = - ; >     "
    "          , ' ) ! ~ { ] ^ / ( _ : <     "
    "          [ } | 1 1 2 3 4 5 6 _ 7 8     "
    "          9 0 a a b c d e 5 f g h i     "
    "          j k l l l m n o 5 f _ p q     "
    "          r s t t u v w x y f z A B     "
    "            C D D E F G H y I J K L     "
    "            M N N O P Q R y I S T U     "
    "            V W W X Y Z ` y 6  ...+.    "
    "            @.#.#.$.%.&.*.=.-.;.>.,.    "
    "            '.).).!.~.{.].^./.D (._.    "
    "          :.<.[.}.}.}.|.1.2.3.4.5.6.    "
    "          7.8.9.0.0.a.b.c.d.e.f.g.h.    "
    "        i.j.k.l.g.m.n.o.p.q.r.>.s.t.    "
    "        u.v.w.s.x.y.z.A.B.C.D.(.t.E.F.  "
    "        G.H.I.J.K.K.L.M.N.O.P.0.Q.R.    "
    "        S.T.U.V.W.W.X.Y.Z.`. +.+++      "
    "        @+#+$+%+&+*+      =+-+;+        "
    "                            >+          "};

 return QPixmap(dossier_rouge_xpm);
}

