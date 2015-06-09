
#include "C_BDM_BaseModelePlugin.h"
#include <QDir>
#include <QSqlDatabase>


#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QDomElement>
#include <QDomNode>
#include <QSqlQuery>
#include <QVariant>
#include <QMultiMap>
#include <float.h>
#include <QObject.h>
#include "C_BDM_BaseModelePlugin.h"

#include "C_PosologieGrammar.h"
#include "C_PatientCtx.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"

//-------------------------------- C_BDM_BaseModelePlugin ------------------------------------------------------
C_BDM_BaseModelePlugin::C_BDM_BaseModelePlugin(const QString &confDatas, const QString &prefixLabel, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
    : C_BaseCommon(parent) , C_BDM_PluginI()
{ m_dataSourceVersion             = "09052012";  // TODO  en attendant le numero provenant de la BD datasemp
  m_firstTimeGetNextSmrInfoForCIP = true;
  m_pQSqlQuery                    = 0;
  if (logWidget)        { C_Log::setLogWidget(logWidget);
                        }
  if (pQProgressBar)    { C_BaseCommon::setProgressWidget(pQProgressBar);
                        }
  if (init(confDatas, prefixLabel)==0)
     {outMessage(  C_BaseCommon::getLastError());
      return;
     }
}

//-------------------------------- ~C_BDM_BaseModelePlugin ---------------------------------------
C_BDM_BaseModelePlugin::~C_BDM_BaseModelePlugin()
{if (m_pQSqlQuery) delete m_pQSqlQuery;
}

//-------------------------------- init ------------------------------------------------------
int C_BDM_BaseModelePlugin::init              ( const QString &confData   , const QString &prefixLabel  )
{   QString errMess           =   "";
    m_pathAppli               =   CGestIni::Param_ReadUniqueParam(confData.toAscii(),"BDM_Configuration", "PathAppli");
    m_pathIni                 =   CGestIni::Param_ReadUniqueParam(confData.toAscii(),"BDM_Configuration", "PathIni");
    setInitialisedSate(0);
    if (m_pathAppli.length()==0)
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() BDM_Configuration/PathAppli is empty"));
        return 0;
       }
    if (!QFile::exists ( m_pathAppli ))
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() BDM_Configuration/PathAppli don't exists"));
        return 0;
       }
    if (m_pathIni.length()==0)
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() BDM_Configuration/PathIni is empty"));
        return 0;
       }
    if (QDir::isRelativePath ( m_pathIni )) m_pathIni = m_pathIni.prepend(m_pathAppli);
    m_pathIni = QDir::cleanPath ( m_pathIni );
    if (!QFile::exists ( m_pathIni ))
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() BDM_Configuration/PathIni don't exists"));
        return 0;
       }
    m_datasIni       =  CGestIni::Param_UpdateFromDisk(m_pathIni);
    if (m_datasIni.length()==0)
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() m_datasIni is empty"));
        return 0;
       }
    if (BaseConnect(m_datasIni, prefixLabel+"BDM_DATASEMP", "DatasempTest", &errMess, "Connexion_Datasemp" )==0)
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() error in BDM connexion BaseConnect()\n%1").arg(errMess));
        return 0;
       }
    m_pQSqlQuery  = new QSqlQuery (QString::null , database() );

    if (dbi_init_database("dbi_datasemp",prefixLabel, "DS", this)==0)
       {setLastError (QObject::tr("C_BDM_BaseModelePlugin::init() error in dbi_init_database()\n"));
        return 0;
       }
    setInitialisedSate(1);
    return 1;

}
//-------------------------------- setLogWidget ------------------------------------------------------
void  C_BDM_BaseModelePlugin::setLogWidget      ( QTextEdit *logWidget        )
{C_Log::setLogWidget(logWidget);
}
//-------------------------------- setProgressWidget ------------------------------------------------------
void  C_BDM_BaseModelePlugin::setProgressWidget ( QProgressBar *pQProgressBar )
{//setProgressWidget(pQProgressBar);
 C_BaseCommon::setProgressWidget(pQProgressBar);
}

//-------------------------------- dataSourceVersion ------------------------------------------------------
QString C_BDM_BaseModelePlugin::dataSourceVersion()
{   QString numEdition   = "";
    QString packBCArrete = "";
    QString ods          =  Datasemp_GetBaseVersion(&numEdition,&packBCArrete);
    m_dataSourceVersion  =  QObject::tr("Datasemp Version : %1  date d'extraction : %2").arg( numEdition, packBCArrete);
    return  m_dataSourceVersion;
}
//-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
/*! \brief return if  plugin is HAS certified
 *  \return return HAS certified status
 */
bool C_BDM_BaseModelePlugin::is_plugin_HAS_certified()
{return false;
}
//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_BaseModelePlugin::plugin_name()
{return "Datasemp";
}
//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_BaseModelePlugin::plugin_icon()
{// QPixmap icon(vidal_22x20_xpm);
 // return icon;
 QString path = m_pathAppli+"Ressources/datasemp.png";
 return QPixmap(path);
}
//---------------------------------------- get_database -------------------------------------------------------
/*! \brief  return active database. This facility is to do extended SQL querys which are not in API
 *  \return  database
 */
QSqlDatabase C_BDM_BaseModelePlugin::get_database()
{ return database();
}
//-------------------------------- plugin_image_source ------------------------------------------------------
/*! \brief  return image source url for monographies
 *  \return return image source url for monographies
 */
QString C_BDM_BaseModelePlugin::plugin_image_source()
{   QString pathFile = CGestIni::Param_ReadUniqueParam(m_datasIni.toAscii(),QString("Connexion_"+plugin_name()).toAscii(), "PathImageSource");
    if ( QDir::isRelativePath ( pathFile ) ) pathFile  = QDir::cleanPath ( pathFile.prepend(m_pathAppli) );
    if (!pathFile.endsWith("/")) pathFile += "/";
    return pathFile;
}
//-------------------------------- lang ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::lang()
{return "fr";
}
//-------------------------------- owner ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::owner()
{return "DS";
}
//-------------------------------- getLastError ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::getLastError()
{return C_BaseCommon::getLastError();
}

//-------------------------------- getCompositionFromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QList<C_InterResultPair> result list where
 *              C_CompositionItem.name()              is the composant name (DCI)
 *              C_CompositionItem.amount()            is the quantity
 *              C_CompositionItem.unit()              is the quantity unit
 *              C_CompositionItem.unit_plural()       is the accorded quantity unit if more than one
 */
/*
SELECT t4B.f8 as drugName,
       t08.f2 as codeSubType,
       t1C.f1 as subType

FROM  t08,
      t1C,
      t4B
WHERE t08.f0 = t4B.f0                     -- t08.f0 = '053133'
AND   t08.f2 = t1C.f0
and   t4B.f8 like "%LAM%"
order by t4B.f8
-- AND   t07.f1 = '0'
LIMIT 0,30000
 */
/*
SELECT t07.f4 as Quantite,
       t24.f1 as QuUnitName,
       t27.f2 as SubName,
       t00.f3 as DrugName,
       t07.f2 as CodeSbc,
       t07.f1 as CodeSection,
       t07.f3 as NumOrdre,
       t07.f5 as QuUnitCode
FROM  t07,
      t00,
      t27,
      t24
WHERE t07.f0 = t00.f0
AND   t00.f2 = '3517301'
AND   t27.f0 = t07.f2
AND   t07.f1 = '0'
AND   t24.f0 = t07.f5

SELECT t00.f3        as drugName,
       t07.f4        as Qu,
       t24.f1        as QuUnitName,
       UPPER(t27.f2) as SubName

FROM  t07,
      t27,
      t08,
      t24,
      t00
WHERE t00.f3 like '%LAMALINE%' -- t00.f2 = '3024490'
AND   t07.f0 = t00.f0
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
and   t08.f1 = '0'
and   t08.f0 = t00.f0
AND   t24.f0 = t07.f5
ORDER BY t00.f2

SELECT UPPER(t27.f2) as SubName,
       t24.f1        as QuUnitName,
       t07.f4        as Qu
FROM  t07,
      t27,
      t08,
      t24,
      t00
WHERE t00.f2 = '3024490'
AND   t07.f0 = t00.f0
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
and   t08.f1 = '0'
and   t08.f0 = t00.f0
AND   t24.f0 = t07.f5

SELECT UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t07.f4        as qu,
       t24.f5        as unit
FROM  t07,
      t27,
      t08,
      t00,
      t24
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
AND   t00.f3 like '%COLCHIMAX%'
ORDER BY t00.f3

 SELECT t54.f0 AS num_subst,
        t54.f2 AS nom_subst,
        t00.f3 AS nom_medoc,
        t00.f0 AS num_medoc ,
        t00.f1 AS cuv_medoc
 FROM  t54
       INNER JOIN t07 ON t54.f0=t07.f2
       INNER JOIN t00 ON t07.f0=t00.f0

 WHERE t54.f1='FRA'
 -- AND CAST(t54.f2 AS CHAR) LIKE '%AMOXI%'
  AND  t54.f3 = TRUE
 -- AND  t00.f3 like '%AUGMEN%'
 ORDER BY t00.f3
 limit 0,10000

*/
 QList<C_CompositionItem> C_BDM_BaseModelePlugin::getCompositionFromCIP(const QString &cip)

 { QList<C_CompositionItem> retList;
   QString requete = QString (
                               " SELECT   t07.f4,          \n"         // 0   quantite
                               "          t24.f1,          \n"         // 1   unite quantite au singulier
                               "          UPPER(t27.f2),   \n"         // 2   libelle de la substance
                               "          t24.f5,          \n"         // 3   unite quantite au pluriel
                               "          t27.f0           \n"         // 4   code de la substance
                               " FROM  t07,                \n"
                               "       t27,                \n"
                               "       t08,                \n"
                               "       t24,                \n"
                               "       t00                 \n"
                               " WHERE t00.f2 = '%1'       \n"
                               " AND   t07.f0 = t00.f0     \n"
                               " AND   t27.f0 = t07.f2     \n"
                               " AND   t07.f1 = t08.f1     \n"
                               " AND   t08.f1 = '0'        \n"
                               " AND   t08.f0 = t00.f0     \n"
                               " AND   t24.f0 = t07.f5     \n"
                               " ORDER BY t07.f3           \n"
                             ).arg(cip);
   QSqlQuery query(requete , database() );
   if (! query.isActive())                             return retList;
   while (query.next())
      { QString          dose = query.value(0).toString();
        QString unitSingulier = Utf8_Query(query, 1);
        QString   unitPluriel = Utf8_Query(query, 3);
        QString   substance   = get_SubstancePere( query.value(4).toString() );  // on tente la substance pere (vaut mieux exploiter le nommage le plus simple du pere sans les acides chlorydrates ..)
        if (substance.length()==0) substance = CGestIni::Utf8_Query(query, 2);
        if (unitPluriel.length()==0) unitPluriel = unitSingulier;  // si pas de pluiel c'est que c'est le meme que celui singulier
        retList.append ( C_CompositionItem (substance ,
                                            dose                           ,
                                            unitSingulier                  ,
                                            unitPluriel
                                           )
                       );
      }
   return  retList;
 }

//--------------------------- get_CIS_and_RCP_From_CIP -----------------------------------------------------------
/*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
 *  \param const QString cip is drug cip code.
 *  \param QString *rcp is pointer on QString to return RCP result if not zero .
 *  \return CIS code string
*/
QString C_BDM_BaseModelePlugin::get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp  /* =0 */ )
{Q_UNUSED(cip);
 Q_UNUSED(rcp);
 return "";
}
//-------------------------------- isStupefiant ------------------------------------------------------
/*! \brief return if the product identified par Id is a stupefiant
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return 1 is stupefiant
 */
/*
SELECT t60.f1 as libelle,
       t63.f1 as code,
       t00.f3 as drug_name
FROM   t63,
       t60,
       t00
-- where  t00.f2='3690548'   -- morphine
-- where  t00.f2='3575327'   -- actifed
where  t63.f0=t00.f1
-- AND    t63.f0=t00.f1
AND    t60.f0=t63.f1
AND    t63.f1>1
AND    t63.f1<4
-- AND    t63.f1='2'
ORDER by t63.f1
LIMIT 0,30000
 */
int C_BDM_BaseModelePlugin::isStupefiant ( const QString &cip )
{
    QString requete = QString (
                                " SELECT t63.f1          \n"
                                " FROM   t63,            \n"
                                "        t00             \n"
                                " WHERE  t00.f2 = '%1'   \n"  // -- 3690548 morphine   -- 3575327 actifed
                                " AND    t63.f0 = t00.f1 \n"
                                " AND    t63.f1>1        \n"
                                " AND    t63.f1<4        \n"
                              ).arg(cip);
    QSqlQuery query(requete , database() );
    return (query.isActive() && query.next());
}
//-------------------------------- code_ATC_To_Name ------------------------------------------------------
/*! \brief return if ATC libelle from code ATC
 *  \param const QString &code      code ATC .
 *  \return QString return libelle
 */
QString C_BDM_BaseModelePlugin::code_ATC_To_Name(const QString &code)
{   QSqlQuery query (QString("SELECT f2 FROM t42 WHERE f0 ='%1'").arg(code) , database() );
    if (query.isActive() && query.next())
       {return Utf8_Query(query, 0);
       }
    return "";
}

//-------------------------------- getPosologiesFromCIP ------------------------------------------------------
/*! \brief return posologies list as posologie grammar QString
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
 *  \return QStringList with grammar QString list
 */
QStringList   C_BDM_BaseModelePlugin::getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip )
{Q_UNUSED(pC_PatientCtx);
 Q_UNUSED(cip);
 return QStringList();
}
//-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QStringList result list with dci composants
 */
QStringList C_BDM_BaseModelePlugin::get_DCI_list_FromCIP(const QString &cip)
{
    QStringList retList;
    QString requete = QString (
                                " SELECT   UPPER(t27.f2)    \n"         // 0
                                " FROM  t11,                \n"
                                "       t27,                \n"
                                "       t00                 \n"
                                " WHERE t11.f0 = t00.f0     \n"
                                " AND   t00.f2 =  '%1'      \n"
                                " AND   t11.f1 = t27.f0     \n"
                              ).arg(cip);

    QSqlQuery query(requete , database() );
    if (! query.isActive())                             return retList;
    while (query.next())
       { retList.append ( CGestIni::Utf8_Query(query, 0) );
       }
    return  retList;
}

//-------------------------------- codePeriodToDays ------------------------------------------------------
double C_BDM_BaseModelePlugin::codePeriodToDays (int code_period, double qu)
{switch (code_period)
   {case 44:   return  qu * 365.25;                     // ans --> jours
    case 43:   return  qu * 30.5;                       // mois --> jours
    case 42:   return  qu * 7;                          // semaine --> jours
    case 38:   return  qu * 1;                          // jours --> jours
   }
 return 0;
}

//-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
/*! \brief test compatibility with maximum and minimun authorized dose and duration from a drug and patient context.
 *  \param C_PatientCtx *pC_PatientCtx
 *  \param const QString &grammar prescription for this drug
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>  C_BDM_BaseModelePlugin::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)
{
/*
SELECT
t00.f2  as cip,
t4B.f8  as name,
t2P.f2  as QuMini,
t2P.f7  as QuMaxi,
t2p.f8  as QuCode,
tq.f1   as QuUnit,
t2P.fC  as DoseMaxi,
tmax.f1 as unitDoseMaxi,
t2P.f9  as DureQuMaxi,
t2P.fE  as DureDoseMaxi,
t2L.f1  as BorneInf,
t2L.f2  as BorneSup,
t2L.f3  as  codeUnit,   -- 1 kg  44 an  43 mois 42 semaine  0 pas d'unite
tl.f1   as unitBorne
FROM
t24 tl,
t24 tmax,
t24 tq,
t2l,
t2P,
t4B,
t00
where   t00.f0= t4B.f0
and      t2L.f4=t2P.f0
and      t2P.f1=t4B.f0
and      tl.f0=t2L.f3
and      tmax.f0=t2P.fD
and      tq.f0=t2P.f8
-- and      t00.f2='3364062'  -- 3364062 / advil    3176159 / HICONCIL
limit      0,30000
*/
    QList<C_InterResultPair>  interResultPairList;
    if (pC_PatientCtx == 0) return interResultPairList;
    double      poids                 = 0;
    QString   s_poids                 = "";
    double      surface               = 0;
    int         ageInDays             = 0;
    QString     s_age                 = "";
    char        sexe                  = 'I';
    QString     s_sexe                = "";
    int            pos                = -1;
    QDate       dateOfPrise;
    /* toutes les 4 a 6 heures (46)
       par jour   par 24 heures
       par 24 heures (espacer les prises de 6 heures)
       1 prise
       par semaine
       par an
       par mois
       toutes les n heures (12 NH)
       par heure
       par minute
       par 48 heures
       type de frequence non-specifie
    */
    double q_min                      = 0;
    double q_max                      = 0;
    int nextPos                       = 0;
    QString    id                     = CGestIni::getXmlData( "gph_id",      grammar, &nextPos);
    QString    id_type                = CGestIni::getXmlData( "gph_it",      grammar, &nextPos);
    if ( id_type   != "CIP" )   return interResultPairList;
    QString    datePrise              = CGestIni::getXmlData( "gph_dt",      grammar, &nextPos).left(10); //        date et heure du debut de prise de la prescription format dd-MM-yyyy hh:mm:ss (la date de fin est calculee avec <gph_cy>)
    QString drugName                  = CGestIni::getXmlData( "gph_na",      grammar, &nextPos);
    QString duration                  = CGestIni::getXmlData( "gph_cy",      grammar, &nextPos);
    QStringList seqList               = CGestIni::getXmlDataList( "gpp",     grammar, &nextPos);

    double durationInDays             = C_PosologieGrammar::totalDurationInDays( duration , 1);

    QString  up_formes_min_max_period = "";
    /*int up_frequency                  = */ C_PosologieGrammar::toMinMax_UP(seqList,  q_min,  q_max, up_formes_min_max_period);
    QString s_q_min                   = QString::number(q_min,'f',2);
    QString s_q_max                   = QString::number(q_max,'f',2);
    //............ si doses non renseign\303\251es alors cassos car controles non possibles ..........................
    if (q_min==0 && q_max==0)    // Alerte : dose non precisees
       { interResultPairList.append( C_InterResultPair(  s_q_min+";"+s_q_max    , "QUANT", QObject::tr("DOSES NON PRECIS\303\211ES") ,
                                                         id                     , "CIP"  , drugName,
                                                         QObject::tr("Les doses indiqu\303\251es pour ce produit ne permettent pas de contr\303\264ler correctement la prescription, "
                                                            "car elles sont non renseign\303\251es"
                                                           ),"IT_QUANT", "1"
                                                      )
                                   );
         return interResultPairList;
       }
    if (pC_PatientCtx)
       {s_poids     = pC_PatientCtx->poids();
        poids       = pC_PatientCtx->poidsToDouble();
        surface     = pC_PatientCtx->surfaceToDouble();
        dateOfPrise = QDate::fromString(datePrise, "dd-MM-yyyy");
        ageInDays   = pC_PatientCtx->get_dateOfBirth().daysTo ( dateOfPrise );    // age du patient en jours au moment de la redaction de l'ordo
        s_sexe      = pC_PatientCtx->get_sexe();
        sexe        = s_sexe.toLatin1()[0];
        s_age       = pC_PatientCtx->get_age();
       }

    //........ recuperera les donnees de la base .....................
    double  drug_min                   = 0;
    double  drug_max                   = 0;
    QString s_drug_min                 = "";
    QString s_drug_max                 = "";
    QString drug_qu_unit               = "";
    QString drug_fact_corp             = "";
    double  drug_dose_max              = -1;
    QString drug_DoseMaxDuree          = "";
    double  drug_epp_inf               = 0;
    double  drug_epp_sup               = 0;
    int     drug_epp_unit              = 0;
    char    drug_epp_flag              = 0;
    bool    isNoResponse               = true;
    int  ageIsNotInLimits              = 1;
    double  drug_max_age               = 0;
    double  drug_min_age               = DBL_MAX;
    int  weightIsNotInLimits           = 1;
    double  drug_max_weight            = 0;
    double  drug_min_weight            = DBL_MAX;
    double  drug_max_period            = 0;
    double  drug_min_period            = DBL_MAX;
    int isSexResponse                  = 0;
    QString requete =           " SELECT                   \n"
                             // "  t00.f2                  \n"   //    as cip
                             // " ,t4B.f8                  \n"   //    as name
                                "  t2P.f2                  \n"   // 0  as QuMini
                                " ,t2P.f7                  \n"   // 1  as QuMaxi
                             // " ,t2p.f8                  \n"   //    as QuCode
                                " ,tq.f1                   \n"   // 2  as QuUnit        -- cuillere-mesure de 5 millilitres/kg    /m2
                                " ,t2P.fC                  \n"   // 3  as DoseMaxi
                             // " ,tmax.f1                 \n"   //    as unitDoseMaxi  -- dose-graduation/kg
                             // " ,t2P.f9                  \n"   //    as DureQuMaxi    -- idem  DureDoseMaxi  t2P.fE
                                " ,t2P.fE                  \n"   // 4  as DureDoseMaxi  -- toutes les 4 a 6 heures (46)
                                " ,t2L.f1                  \n"   // 5  as BorneInf
                                " ,t2L.f2                  \n"   // 6  as BorneSup
                                " ,t2L.f3                  \n"   // 7  as codeUnit        -- 1 kg  44 an  43 mois 42 semaine 38 jours  0 pas d'unite
                                " ,t2L.f6                  \n"   // 8  as Flag         A/Allaitement F/Feminin M/Masculain
                                " ,t2P.fL                  \n"   // 9  as QuDureMini
                                " ,t2P.fM                  \n"   // 10 as CodeUnitQuDureMini
                             // " ,td_min.f1               \n"   //    as UnitDureMini
                                " ,t2P.fN                  \n"   // 11 as QuDureMaxi
                                " ,t2P.fO                  \n"   // 12 as CodeUnitQuDureMaxi
                             // " ,td_max.f1               \n"   //    as UnitDureMaxi
                             // " ,tl.f1                   \n"   //    as unitBorne
                                " FROM                     \n"
                             // " t24 tL,                  \n"
                             // " t24 tmax,                \n"
                                " t24 tq,                  \n"
                                " t2L,                     \n"
                                " t2P,                     \n"
                                " t4B,                     \n"
                                " t00                      \n"
                                " WHERE    t00.f0= t4B.f0  \n"
                                " AND      t2L.f4=t2P.f0   \n"
                                " AND      t2P.f1=t4B.f0   \n"
                             // " AND      tl.f0=t2L.f3    \n"
                             // " AND      tmax.f0=t2P.fD  \n"
                                " AND      tq.f0=t2P.f8    \n";
                             // " AND      t00.f2='%1'     \n" // -- 3364062 / advil    3176159 / HICONCIL"

    if (id.startsWith("'"))
        requete +=  QString (  " AND    t00.f2 IN (%1)  " ).arg(id);    // cip datasemp code sous forme de liste
    else
        requete +=  QString (  " AND    t00.f2 = '%1'  "  ).arg(id);    // cip datasemp code
    // CGestIni::Param_UpdateToDisk("/home/ro/requete.sql", requete);
    QSqlQuery query(requete , database() );
    if (! query.isActive())  return interResultPairList;
    while (query.next())
       { isNoResponse        = false;
         drug_min            = query.value(0).toDouble();
         drug_max            = query.value(1).toDouble();
         drug_qu_unit        = CGestIni::Utf8_Query(query, 2);
         pos                 = drug_qu_unit.indexOf("/");
         if ( pos==-1 )
            { drug_fact_corp = "";
            }
         else
            { drug_fact_corp = drug_qu_unit.mid(pos+1,1);
              drug_qu_unit   = drug_qu_unit.left(pos);
            }
         drug_dose_max       = query.value(3).toDouble();
         drug_DoseMaxDuree   = CGestIni::Utf8_Query(query, 4);
         drug_epp_inf        = query.value(5).toDouble();
         drug_epp_sup        = query.value(6).toDouble();
         drug_epp_unit       = query.value(7).toInt();
         drug_epp_flag       = query.value(8).toChar().toLatin1();
         drug_min_period     = qMin(codePeriodToDays ( query.value(10).toInt(), query.value(9).toDouble()  ) , drug_min_period)  ;
         drug_max_period     = qMax(codePeriodToDays ( query.value(12).toInt(), query.value(11).toDouble() ) , drug_max_period)  ;

         if (drug_fact_corp=="k")
            { drug_min *= poids;
              drug_max *= poids;
            }
         else if (drug_fact_corp=="m")
            { drug_min *= surface;
              drug_max *= surface;
            }
         s_drug_min = QString::number(drug_min,'f',2);
         s_drug_max = QString::number(drug_max,'f',2);

         switch (drug_epp_unit)
            { //............ drug_epp_inf et drug_epp_sup expriment un poids et poso pour un poids entre __ et __ .................
              case 1:               // kg
                  { drug_max_weight  = qMax( drug_epp_sup , drug_max_weight );
                    drug_min_weight  = qMin( drug_epp_inf , drug_min_weight );
                    if (poids>drug_epp_inf && poids<drug_epp_sup)   // Poso conseillee pour cette fourchette de poids
                       {  weightIsNotInLimits = 0;              // ok au moins une reponse existe pour ce poids
                          if (q_min  && q_min<drug_min)         // alert on est en dessous du minimum conseille pour ce poids
                             { interResultPairList.append( C_InterResultPair(  s_q_min  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_min, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               QObject::tr("Dose prescrite minimum %1 trop faible.<br/>"
                                                                                  "La dose minimum permise est de %2 pour un poids de %3 Kg"
                                                                                 ).arg(s_q_min+" "+drug_qu_unit, s_drug_min+" "+drug_qu_unit, s_poids),"IT_QUANT", "2"
                                                                            )
                                                          );

                             }
                          if (q_max  && q_max>drug_max)         // alert on est au dessus du maximum conseille pour ce poids
                             { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_max, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               QObject::tr("Dose prescrite maximum %1 trop elev\303\251e.<br/>"
                                                                                  "Dose maximum permise est de %2 pour un poids de %3 Kg"
                                                                                 ).arg(s_q_max+" "+drug_qu_unit, s_drug_max+" "+drug_qu_unit, s_poids),"IT_QUANT", "4"
                                                                            )
                                                          );
                             }
                       }
                  } break;
              //............ drug_epp_inf et drug_epp_sup expriment un age et une poso pour un age entre __ et __ .................
              //             on transforme tous les ages en jours (permet de comparer toutes les sorties SQL)
              case 44:              // an
              case 43:              // mois
              case 42:              // semaine
              case 38:              // jour
                  { drug_epp_inf = codePeriodToDays ( drug_epp_unit, drug_epp_inf );   // normaliser les valeurs drug_epp_inf en jours
                    drug_epp_sup = codePeriodToDays ( drug_epp_unit, drug_epp_sup );   // normaliser les valeurs drug_epp_sup en jours
                    drug_max_age = qMax( drug_epp_sup , drug_max_age );
                    drug_min_age = qMin( drug_epp_inf , drug_min_age );
                    if (ageInDays>drug_epp_inf && ageInDays<drug_epp_sup)   // Poso conseillee pour cette fourchette de jours
                       {  ageIsNotInLimits = 0;
                          if (q_min  && q_min<drug_min)         // alert quantite en dessous du minimum conseille pour cet age
                             { interResultPairList.append( C_InterResultPair(   s_q_min  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_min, drug_qu_unit) ,
                                                                                id       , "CIP"  , drugName,
                                                                                QObject::tr("Dose prescrite minimum %1 trop faible.<br/>"
                                                                                   "La dose minimum permise est de %2 pour un \303\242ge de %3 "
                                                                                  ).arg(s_q_min+" "+drug_qu_unit, s_drug_min+" "+drug_qu_unit, s_age),"IT_QUANT", "2"
                                                                            )
                                                         );

                             }
                          if (q_max  && q_max>drug_max)         // alert quantite  au dessus du maximum conseille pour cet age
                             { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_max, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               QObject::tr("Dose prescrite %1 trop elev\303\251e.<br/>"
                                                                                  "La dose maximum permise est de %2 pour un \303\242ge de %3 "
                                                                                 ).arg(s_q_max+" "+drug_qu_unit, s_drug_max+" "+drug_qu_unit, s_age),"IT_QUANT", "4"
                                                                            )
                                                         );
                             }
                       }
                  } break;
              //............ pas de systeme de poso selon facteur corporel .................
              //             mais le sexe peut etre une donnee de choix de la posologie
              default:
                  { if (drug_epp_flag==sexe)          // Poso conseillee pour le sexe en cours
                       {  isSexResponse = 0;
                          if (q_min  && q_min<drug_min)         // alert quantite en dessous du minimum conseille pour ce sexe
                             { interResultPairList.append( C_InterResultPair(  s_q_min  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE POUR LE SEXE %3").arg(s_q_min, drug_qu_unit, s_sexe) ,
                                                                               id       , "CIP" , drugName,
                                                                               QObject::tr("Dose prescrite minimum %1 trop faible.<br/>"
                                                                                  "La dose minimum permise pour le sexe %3 est de %2"
                                                                                 ).arg( s_q_min+" "+drug_qu_unit , s_drug_min+" "+drug_qu_unit, s_sexe ),"IT_QUANT", "2"
                                                                            )
                                                         );

                             }
                          if (q_max  && q_max>drug_max)         // alert quantite  au dessus du maximum conseille pour ce sexe
                             { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE POUR LE SEXE %3").arg(s_q_min, drug_qu_unit, s_sexe) ,
                                                                               id       , "CIP"  , drugName,
                                                                               QObject::tr("Dose prescrite maximum %1 trop elev\303\251e.<br/>"
                                                                                  "La dose maximum permise pour le sexe %3 est de %2 "
                                                                                 ).arg( s_q_max+" "+drug_qu_unit , s_drug_max+" "+drug_qu_unit, s_sexe ),"IT_QUANT", "4"
                                                                            )
                                                         );
                             }
                       }
                    else    // Alerte :aucun critere physio pathologique mais quantites en dehors des limites
                       {
                          if (q_min  && q_min<drug_min)         // alert quantite en dessous du minimum conseille
                             { interResultPairList.append( C_InterResultPair(  s_q_min  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_min, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               QObject::tr("Dose minimum %1 trop faible (pas de facteur physiopathologique)<br/>"
                                                                                  "La dose minimum permise est de %2"
                                                                                 ).arg( s_q_max+" "+drug_qu_unit , s_drug_min+" "+drug_qu_unit ),"IT_QUANT", "2"
                                                                            )
                                                         );
                             }
                          if (q_max  && q_max>drug_max)         // alert quantite  au dessus du maximum conseille
                             { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", QObject::tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_max, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               QObject::tr("Dose maximum %1 trop elev\303\251e (pas de facteur physiopathologique)<br/>"
                                                                                  "La dose maximum permise est de %2"
                                                                                 ).arg( s_q_max+" "+drug_qu_unit ,  s_drug_max+" "+drug_qu_unit ),"IT_QUANT", "4"
                                                                            )
                                                         );
                             }
                       }
                  }
            } // switch (drug_epp_unit)
       } // end while (query.next())
    if (isNoResponse)
       { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", QObject::tr("CONTROLE DES DOSES ET DUREES NON POSSIBLE") ,
                                                         id       , "CIP"  , drugName,
                                                         QObject::tr("Donn\303\251es non trouv\303\251es pour le contr\303\264le des doses <br/>"),"IT_QUANT", "1"
                                                      )
                                   );
         return interResultPairList;
       }

    if ( drug_min_period && durationInDays < drug_min_period)    // Alerte : duree prescription trop faible
       { QString s_durationInDays = QString::number(durationInDays,'f',0);
         interResultPairList.append( C_InterResultPair(  s_durationInDays       , "TEMPS"        , QObject::tr("DUREE DE PRESCRIPTION DE %1 JOUR(S) TROP COURTE").arg(s_durationInDays) ,
                                                         id                     , "CIP"          , drugName,
                                                         QObject::tr("La dur\303\251e de prescription %1 jours(s) est trop faible. "
                                                            "La fourchette de durees permise pour une prescription correcte de ce produit est de %2 \303\240 %3 jour(s)"
                                                           ).arg(s_durationInDays, QString::number(drug_min_period,'f',2), QString::number(drug_max_period,'f',2)),"IT_TEMPS", "2"
                                                      )
                                   );
       }
    if ( drug_max_period && durationInDays > drug_max_period)    // Alerte : duree prescription trop elevee
       { QString s_durationInDays = QString::number(durationInDays,'f',0);
         interResultPairList.append( C_InterResultPair(  s_durationInDays       , "TEMPS"        , QObject::tr("DUREE DE PRESCRIPTION DE %1 JOUR(S) TROP GRANDE").arg(s_durationInDays) ,
                                                         id                     , "CIP"          , drugName,
                                                         QObject::tr("La dur\303\251e de prescription %1 jours(s) est trop grande. "
                                                            "La fourchette de durees permise pour une prescription correcte de ce produit est de %2 \303\240 %3 jour(s)"
                                                           ).arg(s_durationInDays, QString::number(drug_min_period,'f',2), QString::number(drug_max_period,'f',2)),"IT_TEMPS", "2"
                                                      )
                                   );
       }
    if ( weightIsNotInLimits && drug_max_weight>0 )    // Alerte : poids evalue (drug_max_weight>0) mais en dehors des limites (il y a eu des reponse mais aucune correspond)
       { interResultPairList.append( C_InterResultPair(  s_q_min+";"+s_q_max    , "QUANT", QObject::tr("POIDS PATIENT %1 Kg INCOMPATIBLE").arg(s_poids) ,
                                                         id                     , "CIP"  , drugName,
                                                         QObject::tr("Ce produit ne permet pas d'\303\251tablir une prescription correcte pour le poids de %1 kg de ce patient. "
                                                            "La fourchette de poids permise pour une prescription correcte de ce produit est de %2 \303\240 %3 kg"
                                                           ).arg(s_poids, QString::number(drug_min_weight,'f',2), QString::number(drug_max_weight,'f',2)),"IT_POIDS", "2"
                                                      )
                                   );
       }
    if ( ageIsNotInLimits && drug_max_age>0 )        // Alerte : age evalue (drug_max_age>0 ) mais en dehors des limites (il y a eu des reponse mais aucune correspond)
       {
         interResultPairList.append( C_InterResultPair(  s_q_min+";"+s_q_max    , "QUANT", QObject::tr("AGE %1 INCOMPATIBLE").arg(s_age) ,
                                                          id                     , "CIP" , drugName,
                                                          QObject::tr("Ce produit ne permet pas d'\303\251tablir une prescription correcte pour l'\303\242ge de %1 de ce patient. "
                                                             "La fourchette d'\303\242ge permise pour une prescription correcte est de %2 \303\240 %3  an(s)"
                                                            ).arg(s_age, QString::number(drug_min_age/365.25,'f',2),QString::number(drug_max_age/365.25,'f',2)),"IT_YEARS", "2"
                                                       )
                                    );
       }
    return interResultPairList;
}
//-------------------------------- getInteractionCim10 ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QStringList &cim10List cim10 pathologies list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
/*
SELECT t55.f0 AS code_vidal_patho,
       t4N.f2 as nom,
       t4N.f3 as codeCim10,
       t50.f1 as nameDatasemp
FROM t4N ,
     t55 ,
     t50
WHERE t4N.f0=t55.f1
AND   t50.f0=t55.f0
AND   t4N.f3='Z32.1'

SELECT
       t52.f2 as niveau,
       t1C.f1 as libelleNiveau,
       t00.f3 as drug_name,
       t50.f1 AS CI_Vidal,
       t4N.f2 AS CIM_X,
       t4N.f3 AS codeCIM_X
FROM t52
INNER JOIN t00 ON t52.f1 = t00.f0
INNER JOIN t1C ON t52.f2 = t1C.f0  -- libelle
INNER JOIN t50 ON t52.f0 = t50.f0,
t55,
t4N
WHERE t00.f2='3514685'
-- AND t00.f3 LIKE 'ASPEGIC 1000%'
AND t55.f1 = t4N.f0
AND t55.f0 = t50.f0
AND t4N.f3 like 'K27%' -- 'K27'
limit 0,100

 SELECT
 t52.f2 ,                                             --  0 as niveau
 t1C.f1 ,                                             --  1 as libelleNiveau
 t50.f1 ,                                             --  2 as CI_Vidal
 t4N.f2 ,                                             --  3 as CIM_X
 t4N.f3 ,                                             --  4 as codeCIM_X
 t00.f3                                               --  5 as drug_name
 FROM t52
 INNER JOIN t00 ON t52.f1 = t00.f0
 INNER JOIN t1C ON t52.f2 = t1C.f0                    --  libelle Niveau
 INNER JOIN t50 ON t52.f0 = t50.f0,
 t55,                                                 --  lien patho vidal et cim10
 t4N                                                  --  table cim10
 WHERE t00.f2 ='3777769'                              --  cip '3844187'
 AND   t55.f1 = t4N.f0
 AND   t55.f0 = t50.f0
 AND   t4N.f3 IN ('K35','J45.0','J45','Z32.1','Z32','N19','Q89.3','Q89','Z39.1','Z39')                                  --  liste des pathos patient
 -- AND   t52.f2 IN ('2999','2991','2990','2996')
 */
QList<C_InterResultPair> C_BDM_BaseModelePlugin::getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  /* = "" */ )
{   QList<C_InterResultPair>            interResultPairList;
    if (cim10List.length()==0)   return interResultPairList;
    QMap         <QString, C_InterResultPair>  mapResult;
    mapResult = Datasemp_get_Cim10_InterResultPair_Map( cip, cim10List , drugName);        // la map evite les doublons
    QMapIterator <QString, C_InterResultPair> it1(mapResult);
    while ( it1.hasNext() )
          {  it1.next();
             interResultPairList.append( it1.value() );
          }
    return interResultPairList;
}

     //-------------------------------- Datasemp_get_Cim10_InterResultPair_Map ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QStringList &cim10List cim10 pathologies list .
      *  \param QString drugName (default empty) the drug name.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     QMap<QString, C_InterResultPair> C_BDM_BaseModelePlugin::Datasemp_get_Cim10_InterResultPair_Map(const QString &cip, const QStringList &cim10List , QString drugName)
     {   QMap<QString,C_InterResultPair> cimMap;
         int     niveau              = -1;
         QString  cim10              = "";
         QString  str                = "";
         QString codesCim10Patients  = "(";
         for (int i=0; i<cim10List.size(); ++i)
             { str = cim10List[i];
               if (str.length()>3)
                  { codesCim10Patients += "'" + str.insert(3,'.') + "',";
                    codesCim10Patients += "'" + str.left(3)       + "',";
                  }
               else
                  {codesCim10Patients += "'" + str      + "',";
                  }
             }
         codesCim10Patients.chop(1);   // virer la derniere virgule
         codesCim10Patients += ")";

         QString requete = QString (
                                    " SELECT                                        \n"
                                    " t52.f2 ,                                      \n"   // 0 as niveau
                                    " t1C.f1 ,                                      \n"   // 1 as libelleNiveau
                                    " t50.f1 ,                                      \n"   // 2 as CI_Vidal
                                    " t4N.f2 ,                                      \n"   // 3 as CIM_X
                                    " t4N.f3 ,                                      \n"   // 4 as codeCIM_X
                                    " t00.f3                                        \n"   // 5 as drug_name
                                    " FROM t52                                      \n"
                                    " INNER JOIN t00 ON t52.f1 = t00.f0             \n"
                                    " INNER JOIN t1C ON t52.f2 = t1C.f0             \n"   // libelle Niveau
                                    " INNER JOIN t50 ON t52.f0 = t50.f0,            \n"
                                    " t55,                                          \n"   // lien patho vidal et cim10
                                    " t4N                                           \n"   // table cim10
                                    " WHERE t00.f2 ='%1'                            \n"   // cip
                                    " AND   t55.f1 = t4N.f0                         \n"
                                    " AND   t55.f0 = t50.f0                         \n"
                                    " AND   t4N.f3 IN %2                            \n"   // liste des pathos patient
                            //      " AND   t52.f2 IN ('2999','2991','2990','2996') \n"   // ultra lent --> il vaut mieux filtrer les niveaux par programmation
                                    ).arg(cip, codesCim10Patients);

          QSqlQuery query(requete , database() );
          if    (  !query.isActive() ) return cimMap;
          while (   query.next()     )
                {   niveau = query.value(0).toInt();
                    switch(niveau)   // fitrage des niveaux par programmation car en SQL c'est archi lent ??!!
                          {case 2999:      // CI absolue
                           case 2991:      // effet ind\303\251sirable information
                           case 2988:      // Mise en garde
                           case 2990:      //  precaution d'emploi
                           case 2996:      // CI relative
                               { cim10  = query.value(4).toString().remove('.');
                                 if ( Datasemp_isThisCim10Code_in_Cim10List(cim10, cim10List))
                                    { cimMap[cim10] =  C_InterResultPair(  cim10    , "CIM", CGestIni::Utf8_Query(query, 3)+ " ("+query.value(4).toString()+")",
                                                                           cip      , "CIP", drugName,
                                                                           QObject::tr("Etat physiopathologique du patient : <br/>")+CGestIni::Utf8_Query(query, 1)+"<br/>"+CGestIni::Utf8_Query(query, 2),"IT_CIM", Datasemp_LevelToLevelStr(niveau)
                                                                        );
                                    } // if ( Datasemp_isThisCim10Code_in_Cim10List(cim10, cim10List))
                               } // case
                          } // switch(niveau)
                }
         return cimMap;
     }

     //--------------------------- Datasemp_isThisCim10Code_in_Cim10List -----------------------------------------------------------
     /*! \brief return if a cim10 code is in cim10 patient list.
      *  \param const QString& cim10 cim10 code to test presence in Cim10List patient codes .
      *  \param const QStringList &cim10List cim10 patient pathologies list .
      *  \return bool true if code already present false otherwise
      */
     bool C_BDM_BaseModelePlugin::Datasemp_isThisCim10Code_in_Cim10List(const QString &cim10, const QStringList &Cim10List)
     {
      if (cim10.length()==0) return false;
      int pos          = cim10.indexOf('-');
      QString cim      = "";
      QString code     = "";
      int      lenCmp  = 0;
      int           i  = 0;
      //............. ce n'est pas un code compose on cherche en direct dans la liste ......................
      if (pos == -1)
         { for ( i=0; i<Cim10List.count();++i)
               {code   = Cim10List[i];
                cim    = cim10;
                lenCmp = qMin( cim.length(), code.length() );
                cim.truncate(lenCmp);
                code.truncate(lenCmp);
                if (cim==code)
                   {return true;
                   }
               }
           return false;
         }
      //............. c'est un code compose N84-N89 on cherche en parcourant dans la liste ..................
      int n            = 0;
      QString cim10Deb = cim10.left(pos);
      QString cim10End = cim10.mid(pos+1);
      cim10Deb.truncate(3);
      cim10End.truncate(3);
      int nMin         = cim10Deb.mid(1).toInt();
      int nMax         = cim10End.mid(1).toInt();;

      for ( i=0; i<Cim10List.size();++i)
          { code   = Cim10List[i].left(3);
            if (cim10[0] != code[0]) continue;
            n      = code.mid(1).toInt();
            if (n>=nMin && n<=nMax) return true;
          }
      return false;
     }

//-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
/*! \brief return interactions xml results from a cip list.
 *  \param const QStringList &cipList      drugs CIP codes list to test.
 *  \param QString drugName (default empty) the drug name.
 *  \return result interaction pair list
 */
 /*
SELECT
      t00.f2 as cip,
      t37.f0 AS inter0,
      t37.f1 AS inter1,
      t4B.f4 as  classeIT,
      t00.f3 as drugName,
      t10.f5 as document,
      A.f1   as classe,
      B.f1   as composant
       -- t37.f1 pointe sur t10.f0 et f1 (f1 est la plus grande valeur et f0 la plus petite)
       -- t10.f5 est le libelle XML de l'interaction
FROM t4B,
     t37,
     t00,
     t10,
     t26 A,
     t26 B
WHERE t4B.f4 = t37.f0
AND   t4B.f0 = t00.f0
AND   t37.f1 = t10.f0
AND   A.f0   = t37.f0
AND   B.f0   = t37.f1
AND   t00.f2 IN ('3230700','3024490')      -- josacine colchimax
limit 0,100


SELECT
      t00.f2 as cip,
      I0.f0  AS inter_I0_F0,
      I0.f1  AS inter_I0_F1,
 --     I1.f0  AS inter_I1_F0,
 --     I1.f1  AS inter_I1_F1,
      TA.f0  as TAf0,
      TA.f1  as TAf1,
 --     TB.f0  as TBf0,
 --     TB.f1  as TBf1,
      t4B.f4 as  classeIT,
      t00.f3 as drugName,
      TA.f5 as documentTA
--      TB.f5 as documentTB
     -- A.f1   as classe,
     -- B.f1   as composant
       -- t37.f1 pointe sur t10.f0 et f1 (f1 est la plus grande valeur et f0 la plus petite)
       -- t10.f5 est le libelle XML de l'interaction
 FROM t4B,
     t37 I0,
--     t37 I1,
     t00,
     t10 TA
--     t10 TB,
--     t26 A,
--     t26 B
 WHERE t4B.f0 = t00.f0
 AND   t4B.f4 = I0.f0
-- AND   t4B.f4 = I1.f0
 AND   I0.f1  = TA.f0
-- AND   I1.f1  = TB.f0
-- AND   A.f0   = I0.f0
-- AND   B.f0   = I0.f1
-- AND   A.f0   = I1.f0
-- AND   B.f0   = I1.f1
-- AND   TB.f0  = TB.f1
-- AND TA.f0 = TA.f1
AND   t00.f2 IN ('3230700','3024490')      -- josacine ,'3024490' colchimax
limit 0,300

SELECT
      t10.ge as pk_Interaction,
      t00.f2 as cip,
      t37.f0 as Id_Classe_Produit,
      t37.f1 as Id_Classe_Composant_1,
      t10.f1 as Id_Classe_Composant_2,
      t10.f2 as Niveau_Alerte,
      t00.f3 as drugName,
      t10.f5 as document
 FROM t4B,
      t37,
      t00,
      t10

 WHERE t4B.f0 = t00.f0
 AND   t4B.f4 = t37.f0
 AND   t37.f1 = t10.f0
 AND   t00.f2 IN ('3230700','3024490')      -- '3230700' josacine ,'3024490' colchimax
limit 0,300

 */

QList<C_InterResultPair> C_BDM_BaseModelePlugin::getInteractionPairFromCipList ( const QStringList &cipList)
{ QList<C_InterResultPair>        interResultPairList;
  int cipListSize = cipList.size();
  if (cipListSize <= 1 )  return  interResultPairList;
  INTER_STRUCT inter;
  QMap      <int, INTER_STRUCT> cip_interMultiMap;    // liste globale de toutes les interactions de tous les cip de la liste confondus mapee sur le cip
  QMap      <int, INTER_STRUCT> ge_interResult;       // liste des interactions interferentes mapee sur le 'ge' de la table t10;
  //....... preparer la liste SQL des cip .................
  QString codesCip  = "(";
  for (int i=0; i<cipListSize; ++i)
      { codesCip += "'" + cipList[i] + "',";
      }
  codesCip.chop(1);   // virer la derniere virgule
  codesCip += ")";
  //..................... la requete sql .................
  QString requete = QString (
                             " SELECT     \n"
                             " t10.ge ,   \n" // 0
                             " t00.f2 ,   \n" // 1 as cip,
                             " t37.f1 ,   \n" // 2 as Id_Classe_Composant_1,
                             " t10.f1 ,   \n" // 3 as Id_Classe_Composant_2,
                             " t10.f2 ,   \n" // 4 as level,
                             " t26.f1 ,   \n" // 5 as Id_Classe_Composant_2_Name
                             " t00.f3 ,   \n" // 6 as drugName,
                             " t10.f5     \n" // 7 as document SGML
                             " FROM t4B,  \n"
                             " t37,       \n"
                             " t00,       \n"
                             " t26,       \n"
                             " t10        \n"
                             " WHERE t4B.f0 = t00.f0  \n"
                             " AND   t4B.f4 = t37.f0  \n"
                             " AND   t37.f1 = t10.f0  \n"
                             " AND   t26.f0 = t10.f1  \n"
                             " AND   t00.f2 IN %1     \n"
                             ).arg(codesCip);

   QSqlQuery query(requete , database() );
   if    ( !query.isActive() ) return interResultPairList;
   //......... relever toutes les lignes d'interactions possibles ...........
   //          pour chaque cip de la liste. Elles sont en vrac.
   while (  query.next() )
         {
          //........ relever les infos .............
          inter.m_ge       = query.value(0).toInt();
          inter.m_cip      = query.value(1).toInt();
          inter.m_id_1     = query.value(2).toInt();
          inter.m_id_2     = query.value(3).toInt();          // produit avec lequel ce CIP interfere
          inter.m_level    = query.value(4).toString();
          inter.m_product  = CGestIni::Utf8_Query(query, 5);  // query.value(5).toString();       // nom du produit avec lequel ce CIP interfere
          inter.m_drugName = CGestIni::Utf8_Query(query, 6);  // query.value(6).toString();
          inter.m_document = query.value(7).toString();
          inter.m_cip2     = 0;                               // terme 2 non connu pour l'instant sera initialise lors test interaction
          cip_interMultiMap.insertMulti(inter.m_cip, inter);  // on accepte les key multiples
        }
   //........... retrouver dans la liste globale ...........................................
   //            de toutes les interactions possibles de chaque cip, celles
   //            qui interferent entre elles
   for (int i=0; i<cipListSize; ++i)
       { int cipToFind = cipList[i].toInt();
         //......... recuperer toutes les interactions du cip a tester..........
         QList<INTER_STRUCT> inter_list = cip_interMultiMap.values(cipToFind);
         //......... pour chacune des interactions du cip a tester ............
         //          voir  elles sont referencees par d'autres cip de la liste
         //          des cip de l'ordo
         for (int l = 0; l < inter_list.size(); ++l)
             { inter = inter_list.at(l);
               //....................... on reteste cette inter.m_id_2 dans toute .........................
               //                        la liste des interactions trouvees
               //                        si trouvee il est retourne
               int cip_2 = Datasemp_isThisInteractionPresentInListe(cipToFind, inter.m_id_2,  cip_interMultiMap);
               if (cip_2)
                  { inter.m_cip2 = cip_2;
                    ge_interResult[inter.m_ge] = inter;          // on mape sur le PK pour ne pas avoir de doublons
                  }
             } //for (int l = 0; l < inter_list.size(); ++l)
       } //for (int i=0; i<cipListSize; ++i)

   //.......... sortir les resultats .......................................................................
   QMapIterator< int, INTER_STRUCT> it (ge_interResult);
   while (it.hasNext())
   {   it.next();
       inter        = it.value();
       QString cip2 = QString::number(inter.m_cip2);
       interResultPairList.append (  C_InterResultPair ( QString::number(inter.m_cip)    ,                 "CIP",       inter.m_drugName,
                                                         cip2                            ,                 "CIP",       inter.m_product,
                                                         QObject::tr("Interaction m\303\251dicamenteuse avec : %1 <br/>%2")
                                                         .arg(dbi_cip_drugName(cip2), Datasemp_SGML_to_html(inter.m_document))     , "AFSSAPS", inter.m_level
                                                       )
                                  );
   }
  return interResultPairList;
}
//-------------------------------- Datasemp_isThisInteractionPresentInListe ------------------------------------------------------
int C_BDM_BaseModelePlugin::Datasemp_isThisInteractionPresentInListe(int cipToExclude, int itCode, const QMap <int, INTER_STRUCT> &cip_interMultiMap)
{   INTER_STRUCT inter;
    QMapIterator< int, INTER_STRUCT> it (cip_interMultiMap);
    while (it.hasNext())
    {   it.next();
        if (it.key() != cipToExclude)   // on ne ne teste pas le cip qui verifie si il interfere avec les autres
           { inter    = it.value();
             if (inter.m_id_1==itCode) return inter.m_cip;
           }
    }
    return 0;
}
//-------------------------------- getInteractionRedundancy ------------------------------------------------------
/*! \brief return redundancy composition in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_BaseModelePlugin::getInteractionRedundancy( const QStringList &cipList)
{   QList<C_InterResultPair>            interResultPairList;
    if ( cipList.length() <= 1 ) return interResultPairList;         // il faut au moins deux produits pour une redondance
    INTER_STRUCT inter;
    QMap      <QString, INTER_STRUCT> subst_MultiMap;          // liste associant key = nom de la substance  data = INTER_STRUCT donnee du produit contenant cette substance
    QMap      <QString, QString>      subst_DistinctMap;       // liste des subtances QMap pour avoir unicite des noms
    QString   substName = "";
    //.......... creer la liste des substances de l'ordonnance ..........................................
    QString s_cip_list = cipList.join("','");
    QString requete    = QString (   " SELECT t00.f2 ,         \n"    // 0   as cip
                                     "        t00.f3 ,         \n"    // 1   as drug_name
                                     "        t27.f2           \n"    // 2   as subst_Name
                                     " FROM   t00,             \n"
                                     "        t11,             \n"
                                     "        t27              \n"
                                     " WHERE  t00.f2 IN ('%1') \n"
                                     " AND    t11.f0 = t00.f0  \n"
                                     " AND    t27.f0 = t11.f1  \n"
                                     " ORDER BY        t11.f1  \n"  ).arg(s_cip_list);

    QSqlQuery query(requete , database() );
    if    ( !query.isActive() ) return interResultPairList;
    while (  query.next() )
       { inter.m_cip      = query.value(0).toInt();
         inter.m_drugName = CGestIni::Utf8_Query(query, 1);
         inter.m_product  = CGestIni::Utf8_Query(query, 2);
         int nb_cip       = cipList.count( QString::number(inter.m_cip));
         if ( nb_cip >= 2 )                    // PB si plusieurs fois fois le meme CIP seule une reponse est retournee par la requete donc detecter dans la liste des CIP si ce CIP est pluriel
            { for (int i=0; i<nb_cip; ++i)     //    et rajouter autant de fois le produit (on accepte les clefs multiples sur la substance pour mapper chaque specialite dessus)
                  { subst_MultiMap.insertMulti(inter.m_product, inter);
                  }
            }
         else
            { subst_MultiMap.insertMulti(inter.m_product, inter);          // on accepte les clefs multiples sur la substance pour mapper chaque specialite dessus
            }
         subst_DistinctMap.insert(inter.m_product,inter.m_product);        // on accepte pas les clefs multiples ==> liste de substance unique ordonnee
       }
    //......... parser toutes la liste des substances uniques ordonnees et voir si plusieurs produits repondent .......................
    QMapIterator<QString, QString> it(subst_DistinctMap);
    while (it.hasNext())
          { it.next();
            substName                      = it.value();
            QList<INTER_STRUCT> inter_list = subst_MultiMap.values( substName );
            int  nb  = inter_list.size();
            if ( nb >= 2 )   // une redondance est trouvee car deux CIP avec la meme substance
               { QString title = QObject::tr(" <b>%1</b> redondances de subtance possibles ").arg(QString::number(nb));
                 QString avec  = "";
                 for (int i=0; i<inter_list.size(); ++i)
                     {avec += "<br/><b>" + inter_list[i].m_drugName+"</b>";
                     }
                 interResultPairList.append( C_InterResultPair( ""       , "REDON", title,
                                                                ""       , "REDON", substName,
                                                                QObject::tr("%1 produits avec la même substance <b>%2</b> on \303\251t\303\251 trouv\303\251s :<br/>").arg(QString::number(nb), substName) + avec + "<br/>","REDONDANCES", QString::number(nb)
                                                              )
                                           );

               }  // if ( inter_list.size() >= 2 )
          } // while (it.hasNext())
    return interResultPairList;
}

//-------------------------------- getInteractionTerrain ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QString &age patient age .
 *  \param const QString &sexe patient sexe F-M-I.
 *  \param const QString &poids patient poids .
 *  \param const QString &taille patient size .
 *  \param const QString &amenorrhee in weeks
 *  \param const QString &clairance in ml/min
 *  \param int   allaitement in number of days
 *  \param bool sport if true particular attention for doping drugs is required  .
 *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
 /*
SELECT
       t52.f2 as niveau,
       t1C.f1 as libelleNiveau,
       t00.f3 as drug_name,
       t50.f1 AS CI_Vidal,
       t4N.f2 AS CIM_X,
       t4N.f3 AS codeCIM_X
FROM t52
INNER JOIN t4B ON t52.f1 = t4B.f0
INNER JOIN t1C ON t52.f2 = t1C.f0  -- libelle
INNER JOIN t50 ON t52.f0 = t50.f0,
t55,
t4N,
t00
WHERE t00.f0 = t4B.f0
-- AND t00.f3 LIKE 'ASPEGIC 1000%'
-- AND t00.f2='3514685'
AND t55.f1 = t4N.f0
AND t55.f0 = t50.f0
AND t52.f2 IN ('2999','2991','2990','2996')
-- AND t4N.f3 like 'K27%' -- 'K27'
limit 0,3000

 SELECT
 t00.f2          as cip,
 t2A.f0          as codeTerrain,
 t50.f1          as libelle_terrain,
 t2G.f2          as borneInf,
 t2G.f1          as borneSup,
 t2G.f6          as All_SexeFM,
 t52.f2          as niveau,
 t45.f5          as document,
 t1C.f1          as libelle_niveau,
 t2G.f3          as unitCode,
 t4B.fG          as dopant,
 t4B.fD          as vigilance
 FROM t00,t2G
 INNER JOIN t52 ON t2G.f4 = t52.f0
 INNER JOIN t4B ON t52.f1 = t4B.f0
 INNER JOIN t2A ON t2G.f0 = t2A.f0
 INNER JOIN t1C ON t52.f2 = t1C.f0
 INNER JOIN t50 ON t52.f0 = t50.f0,
 t45
 WHERE t00.f0 = t4B.f0
 AND   t45.f0 = t52.f6
 AND   t00.f3 like '%ABILIFY %'
 AND   t2A.f0 = 1
-- AND   t00.f2 = '%1' -- cip
 limit 0,100

select t00.f2 as cip,
       t00.f3 as drugName,
       t4B.fg as dopant,
       t4B.fd as codeVigil,
       t5A.f1 as libelleVigil,
       t5A.f3 as niveauVigil
from   t4B,
       t00,
       t5A
where  t4B.f0 = t00.f0
and    t4B.fD = t5A.f0
limit 0,200
 */
 QList<C_InterResultPair>    C_BDM_BaseModelePlugin::getInteractionAgeSexePoidsSportVigil( const QString &cip,
                                                                                         const QString &_age ,
                                                                                         const QString &sexe,
                                                                                         const QString &_poids,
                                                                                         const QString &taille,
                                                                                         const QString &_amenorrhee,
                                                                                         const QString &_clairance,
                                                                                         int   allaitement,
                                                                                         bool  sport       /* = true */,
                                                                                         bool  vigil       /* = true */,
                                                                                         QString drugName  /*= "" */ )
 {
     Q_UNUSED (taille);
     QList<C_InterResultPair>        interResultPairList;
     int poids       = _poids.toInt();
     int amenorrhee  = _amenorrhee.toInt();
     int clairance   = _clairance.toInt(); if (clairance<0) clairance = 100;   // -1 si non definie
     QString requete = QString (
                                " SELECT t2A.f0 ,                    \n"   // 0   as codeTerrain
                                " t2G.f1,                            \n"   // 1   as borneInf
                                " t2G.f2,                            \n"   // 2   as borneSup
                                " t2G.f6,                            \n"   // 3   as All_SexeFM
                                " t52.f2,                            \n"   // 4   as niveau
                                " t52.f5,                            \n"   // 5   as document
                                " t50.f1,                            \n"   // 6   as libelle_terrain
                                " t1C.f1,                            \n"   // 7   as libelle_niveau
                                " t2G.f3,                            \n"   // 8   as unitCode
                                " t4B.fG,                            \n"   // 9   as dopant
                                " t4B.fD,                            \n"   // 10  as vigilance
                                " t4B.fE                             \n"   // 11  as commentaire vigilance
                                " FROM t00,t2G                       \n"
                                " INNER JOIN t52 ON t2G.f4 = t52.f0  \n"
                                " INNER JOIN t4B ON t52.f1 = t4B.f0  \n"
                                " INNER JOIN t2A ON t2G.f0 = t2A.f0  \n"
                                " INNER JOIN t1C ON t52.f2 = t1C.f0  \n"
                                " INNER JOIN t50 ON t52.f0 = t50.f0  \n"
                                " WHERE t00.f0 = t4B.f0              \n"
                                " AND   t00.f2 = '%1'                \n"
                                ).arg(cip);

      QSqlQuery query(requete , database() );
      if    ( !query.isActive() ) return interResultPairList;
      C_InterResultPair insuffRenaleInterResultPair;
      C_InterResultPair ageInterResultPair;
      int lastIRLevel  = -1;
      int lastAgeLevel = -1;   // 3600 mois = 300 ans
      QString s_bSup;
      QString s_bInf;
      bool dopeIsNotEvaluated = true;
      bool vigiIsNotEvaluated = true;
      while (  query.next() )
            {  int codeTerrain = query.value(0).toInt();
               s_bInf          = query.value(1).toString();
               s_bSup          = query.value(2).toString();
               int borneInf    = s_bInf.toInt();
               int borneSup    = s_bSup.toInt();
               QString SexeAll = query.value(3).toString();
               int level       = query.value(4).toInt();
               QString texte   = query.value(5).toString(); // CGestIni::Utf8_Query(query, 5);
               int unitCode    = query.value(8).toInt();
               QString title   = "";
               QByteArray ba   = query.value(9).toByteArray();   // query.value(9).toBool()  fonctionne pas sous mac ??!!
               if (dopeIsNotEvaluated && ba.length() && ba[0])
                  { title = QObject::tr(" Produit pouvant être positif lors des contr\303\264les anti-dopage");
                    interResultPairList.append( C_InterResultPair( ""      , "DOPE", title,
                                                                   cip     , "CIP", drugName,
                                                                   QObject::tr("Sport : <br/>") +title + "<br/>","IT_DOPE", sport==true?"4":"0"
                                                                 )
                                               );
                   dopeIsNotEvaluated = false;                        // c'est fait (ne pas le faire deux fois)

                  }
               int niveauVigi = query.value(10).toInt();
               if (vigiIsNotEvaluated && niveauVigi && niveauVigi<= 5)
                  { QString niveau =  query.value(10).toString();
                    title = QObject::tr(" Produit alt\303\251rant la vigilance (niveau %1)").arg(QString::number(niveauVigi));
                    interResultPairList.append( C_InterResultPair( ""     , "VIGIL", QObject::tr("Effets sur l'aptitude \303\240 conduire des v\303\251hicules et \303\240 utiliser des machines"),
                                                                   cip    , "CIP"  , drugName,
                                                                   QObject::tr("Vigilance : <br/>") +title + "<br/>"+CGestIni::Utf8_Query(query, 11), vigil==true?"IT_VIG":"IT_W_VIG", niveau
                                                                 )
                                               );
                    vigiIsNotEvaluated = false;    // c'est fait (ne pas le faire deux fois)
                  }
                       title   = CGestIni::Utf8_Query(query, 6);
               int api_level   = 0;
               switch (codeTerrain)
                      {
                       /* a table t2G contient les donn\303\251es correspondantes \303\240 celle du lexique (t50) :
                          Dans le cas ci-dessus nous avons :
                          Pr\303\251caution d'emploi pour enfant de moins de 15 ans correspond aux valeurs 0 (t2G.f1) et 15 (t2G.f2), si l?\303\242ge saisi est entre 0 et 15  il faut afficher une alerte.
                          Mise en garde R\303\251serv\303\251 \303\240 l'adulte correspond aux valeurs 15(t2G.f1) et 0(t2G.f2), si l'\303\242ge saisi est entre 0 et 15  il faut afficher une alerte.
                          Pour les valeurs de t52.f2 (2990 [Pr\303\251caution d'emploi], 2996 [Contre-indication relative] et 2999 [contre-indication absolue]), les valeurs dans t2G sont des valeurs exclusives
                          si une donn\303\251e  correspond \303\240 un EPP*, il faut afficher une alerte.
                          Pour la valeur de   t52.f2 (2988 [Mise en garde]) les valeurs dans t2G sont inclusives  si une donn\303\251e  ne correspond pas \303\240 un EPP*,  il faut afficher une alerte de 'reserve'.
                          Pour la valeur de   t52.f2 (2994 [Indication]), les valeurs dans t2G sont inclusives comme il peut \303\251xister plusieurs indications sans pr\303\251conisation d'\303\242ge, ceci est donn\303\251 \303\240 titre 'indicatif'.
                          Il est peut-être judicieux de n'afficher qu?une seule alerte pour l'\303\242ge, et d?indiquer qu'il y a une mise en garde et une pr\303\251caution d'emploi.
                       */
                       case 1:  // age
                             {
                                double d_borneSup = s_bSup.toDouble();
                                double d_borneInf = s_bInf.toDouble();
                                double d_age      =   _age.toDouble();
                                if (unitCode==44)   //  44-an 43-mois 42 semaines
                                   { d_borneInf *= 12.0;
                                     d_borneSup *= 12.0;
                                   }
                                if (d_borneSup==0) d_borneSup = 3600;   // 300 ans
                                //............. traiter le cas des inclusifs exclusifs ...........
                                int isAlert             = 0;
                                QString strAlert        = "";
                                QString ageToDisplaySup = "";
                                QString ageToDisplayInf = "";
                                QString ageToDisplay    = "";
                                if (d_borneSup>=24) ageToDisplaySup = QString::number(d_borneSup/12.0)  + " ans";
                                else                ageToDisplaySup = QString::number(d_borneSup,'g',0) + " mois";
                                if (d_borneInf>=24) ageToDisplayInf = QString::number(d_borneInf/12.0)  + " ans";
                                else                ageToDisplayInf = QString::number(d_borneInf,'g',0) + " mois";
                                if (d_age>=24)      ageToDisplay    = QString::number(d_age/12.0)       + " ans";
                                else                ageToDisplay    = QString::number(d_age,'g',0)      + " mois";
                                if    (level==2988)
                                      { isAlert  = ! (d_age >= d_borneInf && d_age < d_borneSup);   // d_borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                        strAlert = QObject::tr("Age du patient <b>%1</b> en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(ageToDisplay, ageToDisplayInf, ageToDisplaySup);
                                      }
                                else
                                      { isAlert  =   (d_age >  d_borneInf && d_age < d_borneSup);   // d_borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                        strAlert = QObject::tr("Age du patient <b>%1</b> incompatible car entre <b>%2</b> et <b>%3</b><br/>").arg(ageToDisplay, ageToDisplayInf, ageToDisplaySup);
                                      }
                                api_level = Datasemp_LevelToLevelInt(level);
                                if ( api_level > lastAgeLevel && isAlert) // on garde la raison la plus grave
                                   { ageInterResultPair.setDatas( _age   , "AGE", title,
                                                                  cip    , "CIP", drugName,
                                                                  strAlert + texte +"<br/>"
                                                                  ,"IT_YEARS", Datasemp_LevelToLevelStr(level)
                                                                );
                                    lastAgeLevel = api_level;
                                  }
                                break;
                             }  // end case
                       case 2:  // poids
                             {  if (borneSup==0) borneSup = 500;   // 500 kg
                                //............. traiter le cas des inclusifs exclusifs ...........
                                int isAlert      = 0;
                                QString strAlert = "";
                                if    (level==2988)
                                      { isAlert  = ! (poids >= borneInf && poids < borneSup);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                        strAlert = QObject::tr("Poids du patient <b>%1 kg</b> en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(_poids, QString::number(borneInf), QString::number(borneSup));
                                      }
                                else
                                      { isAlert =   (poids >  borneInf && poids < borneSup);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                        strAlert = QObject::tr("Poids du patient <b>%1 kg</b> incompatible car entre <b>%2</b> et <b>%3</b><br/>").arg(_poids, QString::number(borneInf), QString::number(borneSup));
                                      }

                                if (isAlert)
                                   { interResultPairList.append( C_InterResultPair( _poids  , "POIDS", title,
                                                                                    cip     , "CIP", drugName,
                                                                                    strAlert + texte +"<br/>"
                                                                                    ,"IT_POIDS",      Datasemp_LevelToLevelStr(level)
                                                                                  )
                                                               );
                                   }
                                break;
                             } // end case
                      case 3:  // Gossesse
                            { if (borneSup==0) borneSup = 500;   // 500 semaines d'amenorrhee

                              //............. traiter le cas des inclusifs exclusifs ...........
                              QString s_amenorrhee = QString::number(amenorrhee);
                              int isAlert          = 0;
                              QString strAlert     = "";
                              if    (level==2988)
                                    { isAlert  = ! (amenorrhee >= borneInf && amenorrhee < borneSup);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                      strAlert = QObject::tr("Age de grossesse de la patiente <b>%1</b> (en semaines d'am\303\251norrh\303\251e)  en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(s_amenorrhee, QString::number(borneInf), QString::number(borneSup));
                                    }
                              else
                                    { isAlert  =   (amenorrhee >  borneInf && amenorrhee < borneSup);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                      strAlert = QObject::tr("Age de grossesse de la patiente <b>%1</b> (en semaines d'am\303\251norrh\303\251e)  incompatible car entre <b>%2</b> et <b>%3</b><br/>").arg(s_amenorrhee, QString::number(borneInf), QString::number(borneSup));
                                    }

                              if (isAlert)
                                 { interResultPairList.append( C_InterResultPair( "Z32.1"   , "CIM", title,
                                                                                  cip       , "CIP", drugName,
                                                                                  strAlert + texte +"<br/>"
                                                                                  ,"IT_GRO",      Datasemp_LevelToLevelStr(level)
                                                                                )
                                                             );
                                 }
                               break;
                            }  // end case
                      case 4:  // allaitement (bornes sans signification)
                            { if (allaitement)
                                 { interResultPairList.append( C_InterResultPair( "Z39.1" , "CIM", title,
                                                                                   cip    , "CIP", drugName,
                                                                                   QObject::tr("Allaitement : <br/>")+ texte + "<br/>" ,"IT_ALL",      Datasemp_LevelToLevelStr(level)
                                                                                 )
                                                              );
                                 }
                               break;
                            }  // end case
                      case 5:  // insuffisance renale
                            { if (borneSup==0) borneSup = 500;   // 500 ml/mn
                              //............. traiter le cas des inclusifs exclusifs ...........
                              int isAlert          = 0;
                              QString strAlert     = "";
                              if    (level==2988)
                                    { isAlert  = ! (clairance >= borneInf && clairance < borneSup);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                      strAlert = QObject::tr("Clairance du patient <b>%1 ml/min</b> en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(_clairance, QString::number(borneInf), QString::number(borneSup));
                                    }
                              else
                                    { isAlert  =   (clairance >  borneInf && clairance < borneSup);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                      strAlert = QObject::tr("Clairance du patient <b>%1 ml/min</b> incompatible car entre <b>%2</b> et <b>%3</b><br/>").arg(_clairance, QString::number(borneInf), QString::number(borneSup));
                                    }
                              api_level = Datasemp_LevelToLevelInt(level);
                              if (api_level > lastIRLevel &&  isAlert)    // on garde la raison la plus grave
                                 { insuffRenaleInterResultPair.setDatas(  "N19"     , "CIM", title,
                                                                          cip       , "CIP", drugName,
                                                                          strAlert + texte +"<br/>"
                                                                          ,"IT_CIM",      Datasemp_LevelToLevelStr(level)
                                                                       );
                                   lastIRLevel = api_level;
                                 }

                               break;
                            }  // end case
                      case 6:  // sexe
                            { if (sexe == SexeAll)
                                 {interResultPairList.append( C_InterResultPair(  " "    , "", title,
                                                                                   cip     , "CIP", drugName,
                                                                                   QObject::tr("Sexe : <br/>")+title+"<br/>"+ texte + "<br/>" ,"IT_SEX",      Datasemp_LevelToLevelStr(level)
                                                                               )
                                                            );
                                 }
                               break;
                            }  // end case
                      }
            } // end while (  query.next() )
      if (insuffRenaleInterResultPair.Code1()=="N19")    interResultPairList.append(insuffRenaleInterResultPair);   // si contient la valeur "N19" c'est qu'il a ete initialise
      if (ageInterResultPair.CodeTyp1()=="AGE")          interResultPairList.append(ageInterResultPair);            // si contient la valeur "AGE" c'est qu'il a ete initialise
      return interResultPairList;
 }

 //-------------------------------- nextATC_Code ------------------------------------------------------
 /*
 Table 42 : Dictionnaire des classes ATC
 Classification OMS des produits, par intentions th\303\251rapeutiques.
         Code	Description	Type	Taille	Relation
 1  	 0	Code classe ATC
 Code structur\303\251, traduisant une arborescence.	[9] Texte longueur variable	 7
         1	Intitul\303\251 anglais
 Intitul\303\251 de la classe en anglais (OMS)	[9] Texte longueur variable	 127
         2	Intitul\303\251 fran\303\247ais
 Intitul\303\251 de la classe en fran\303\247ais	[9] Texte longueur variable	 255
 */
int  C_BDM_BaseModelePlugin::nextATC_Code(QString &codeAtc, QString &libelle)
{
    if (codeAtc.length()==0 && libelle.length()==0)    // premiere fois, apres il devrait contenir le retour SQL
       {
        m_position = 0;
        if (!database().isOpen() && database().open()== FALSE)
           { outMessage( QObject::tr("ERREUR : C_BDM_BaseModelePlugin::nextATC_Code() database can not be open"), __FILE__, __LINE__);  return 0;
           }
        QString     requete  =" SELECT f0, f2 FROM t42 ORDER BY f0";
        m_pQSqlQuery->exec(requete);
        if ( ! m_pQSqlQuery->isActive() )
           { outSQL_error(*m_pQSqlQuery, QObject::tr("<font color=#ff0000><b>ERREUR  : C_BDM_BaseModelePlugin::nextATC_Code()</b></font>"), requete, __FILE__, __LINE__  );
             return -1;
           }
       }
    if (m_pQSqlQuery->next())
       {  ++m_position;
          codeAtc     = m_pQSqlQuery->value(0).toString();
          libelle     = CGestIni::Utf8_Query(m_pQSqlQuery, 1);
          return m_position;
       }
    m_position = 0;
    return 0;
}
//-------------------------------- drugListMakeSQLFilter ------------------------------------------------------
/*
 SELECT t00.f2 as Cip,
        t00.f3 as DrugName,
        t25.f1 as natureName,
        t4B.fA as ATC,
        -- t34.f1 as disponibilite,
        t63.fE as hosto,          -- == HOSTO ->1   != HOSTO --> 0
        t01.f1 as Prix
 FROM  t00
      ,t4B
      ,t25
      ,t02
 --     ,t34
      ,t63
      ,t01
 WHERE  t02.f0  = t00.f1
 AND    t02.f3  = '0'      -- doit etre disponible
 AND    t4B.fA != 'NULL'   -- si c'est un medoc faut un code ATC
 AND    t00.f0  = t4B.f0
 AND    t25.f0  = t4B.f1
 -- AND    t34.f0  = t02.f3
 AND    t00.f1  = t63.f0
 AND    t01.f0  = t00.f1
 -- AND    t63.fE  = "0"      -- == HOSTO ->1   != HOSTO --> 0
 ORDER  by t01.f1
 LIMIT 0,30000

SELECT UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t00.f2        as cip,
       t07.f4        as qu,
       t24.f5        as unit,
       t63.fE        as hosto,
       t01.f1        as Prix,
       t4B.fA        as atc
FROM  t07,
      t27,
      t08,
      t00,
      t24,
      t02,
      t4B,
      t01,
      t63
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t00.f0 = t4B.f0
AND   t4B.fA IS NOT NULL
-- AND   t4B.fA IS NULL
AND   t00.f1 = t63.f0
AND   t01.f0 = t00.f1
AND   t02.f0 = t00.f1
AND   t02.f3 = '0'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t07.f3 = '1'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
-- AND   t00.f3 like '%AUGMENTIN%'
ORDER BY t00.f3
 */
QString  C_BDM_BaseModelePlugin::drugListMakeSQLFilter()
{   //.......... cette requete elimine les produits sans composition ............
    //           non disponibles et sans code ATC
    /*
    return   " FROM  t07                  \n"
          // "      ,t27                  \n"
             "      ,t08                  \n"
             "      ,t00                  \n"
             "      ,t24                  \n"
             "      ,t02                  \n"
             "      ,t4B                  \n"
             "      ,t01                  \n"
             " WHERE t07.f0 = t00.f0      \n"
             " AND   t00.f0 = t4B.f0      \n"
             " AND   t4B.fA IS NOT NULL   \n" // code ATC non null   -- AND   t4B.fA IS NULL
             " AND   t01.f0 = t00.f1      \n"
             " AND   t02.f0 = t00.f1      \n"
          //   " AND   t02.f3 = '0'         \n" // disponible
          // " AND   t27.f0 = t07.f2      \n"
             " AND   t07.f1 = t08.f1      \n"
             " AND   t08.f1 = '0'         \n" // Numero de section, code section
             " AND   t07.f3 = '1'         \n" // ordre substance 1 (pour limiter a une reponse si plusieurs composants)
             " AND   t08.f0 = t00.f0      \n"
             " AND   t24.f0 = t07.f5      \n"
             " ORDER BY t00.f3            \n";
     */
    return   " FROM  t00                  \n"
             "      ,t02                  \n"
             "      ,t4B                  \n"
             "      ,t01                  \n"
             " WHERE  t00.f0 = t4B.f0     \n"
          //   " AND   t4B.fA IS NOT NULL   \n" // code ATC non null   -- AND   t4B.fA IS NULL
             " AND   t01.f0 = t00.f1      \n"
             " AND   t02.f0 = t00.f1      \n"
             " AND   t00.fK LIKE '1%'     \n"   // famille datasemp K = 1 = Sp\303\251cialit\303\251s ; 2 = Di\303\251t\303\251tiques ; 3 = V\303\251t\303\251rinaires ; 4 = Parapharmacie ; 5 = Accessoires ; 6 = Divers (herboristerie, etc.) ; 7 = Hom\303\251opathie
             " AND   t02.f3 = '0'         \n"   // disponible
          //   " AND   t4b.f7 like '1%'     \n"
             " ORDER BY t00.f3            \n";
}

//-------------------------------- drugsList_Count ------------------------------------------------------
long  C_BDM_BaseModelePlugin::drugsList_Count()
{   m_position = 0;
    if (!database().isOpen() && database().open()== FALSE)
       {outMessage( QObject::tr("ERREUR : C_BDM_BaseModelePlugin::drugsList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    QString requete  =
            " SELECT COUNT(*)            \n"
            + drugListMakeSQLFilter();

    QSqlQuery query(requete , database() );
    outSQL_error(query, QObject::tr("<font color=#ff0000><b>ERREUR  : C_BDM_BaseModelePlugin::drugsList_Count() </b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {return (long) query.value(0).toDouble();
       }
    return 0;
}

//-------------------------------- getDrugList_Start ------------------------------------------------------
int C_BDM_BaseModelePlugin::getDrugList_Start()
{   m_position = 0;
    if (!database().isOpen() && database().open()== FALSE)
       {outMessage( QObject::tr("ERREUR : C_BDM_BaseModelePlugin::drugsList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    m_pQSqlQuery->exec( "ALTER TABLE `DatasempTest`.`t00` ADD INDEX `a2prim` (`f2`)" );    // index ultra-critique sur le cip
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_BaseModelePlugin::getDrugList_Start() create index t00.f2 `a2prim`", "", __FILE__, __LINE__);
    //.......... cette requete elimine les produits sans composition ............
    //           non disponibles sans code ATC
    QString requete  =
            " SELECT                     \n"
            " t00.f0,                    \n"    // 0 as cpg,
            " t00.f2,                    \n"    // 1 as cip,
            " t00.f3,                    \n"    // 2 as drugName,
            " t4B.fA,                    \n"    // 3 as atc,
            " t01.f3,                    \n"    // 4 as Prix
            " t02.f2                     \n"    // 5 as 4/ville    2/hosto
            + drugListMakeSQLFilter();
   // CGestIni::Param_UpdateToDisk("/home/ro/requete.sql",requete);
    m_pQSqlQuery->exec( requete );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_BaseModelePlugin::getDrugList_Start()", requete, __FILE__, __LINE__);
    if ( !m_pQSqlQuery->isActive() ) return 0;
    else                             return 1;
}

//-------------------------------- convertPrixDecimalToCts ------------------------------------------------------
QString  C_BDM_BaseModelePlugin::convertPrixDecimalToCts(const QString &prix)
{QString  int_part = "";
 QString  dec_part = "";
 int             i = 0;
 int           len = prix.length();
 int           pos = prix.indexOf('.');
 if (pos==-1)  pos = prix.indexOf(',');
 if (pos==-1)                    // pas de partie d\303\251cimale exple 0012  12 faut rajouter deux zeros
    { int_part = prix;
      for ( i=0; i<len; ++i)   // enlever les zeros non significatifs de debut
          { if (int_part.toAscii()[i]!='0')
               {break;
               }
          }
      int_part = int_part.mid(i);       // si i==len cas du "0000000" alors ret.mid(i) --> "" donc ok
      int_part += "00";
      return int_part;
    }
  else                       // il y a partie d\303\251cimale exple 0012.25  12.1 faut ajuster la partie decimale a deux chiffres
    { int_part = prix.left(pos);
      for ( i=0; i<len; ++i)   // enlever les zeros non significatifs de debut
          { if (int_part.toAscii()[i]!='0')
               {break;
               }
          }
      int_part = int_part.mid(i);         // si i==len cas du "0000000" alors ret.mid(i) --> "" donc ok
      dec_part = prix.mid(pos+1) + "00";  // on y rajoute deux zeros quite a les couper apres
      dec_part = dec_part.left(2);        // on garde deux chiffres pour la partie decimale
      return int_part + dec_part;
    }
}

//-------------------------------- getDrugList_NextRecord ------------------------------------------------------
int  C_BDM_BaseModelePlugin::getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord)
{
        QStringList dciList;
        QString     code_cpg  = "";
        QString     libelle   = "";
        QString     cip_code  = "";
        QString     ucd_code  = "";
        QString     atc_code  = "";
        QString     prix      = "";
        QString     generique = "";
        QString     statut    = "";

      if (m_pQSqlQuery->next() && m_position < 200000)
         {code_cpg     = m_pQSqlQuery->value(0).toString();
          cip_code     = m_pQSqlQuery->value(1).toString();
          libelle      = CGestIni::Utf8_Query(*m_pQSqlQuery, 2);
          atc_code     = m_pQSqlQuery->value(3).toString();
          prix         = convertPrixDecimalToCts(m_pQSqlQuery->value(4).toString());
          statut       = m_pQSqlQuery->value(5).toInt()==2?"H":"VH";
          ucd_code     = code_CPG_to_code_UCD(code_cpg);
          generique    = code_CPG_to_StatutGenerique(code_cpg);
          dciList      = code_CPG_to_DCI_1_2_3(code_cpg);

          rC_BDM_DrugListRecord.set_commercialName(libelle);
          rC_BDM_DrugListRecord.set_id(cip_code);
          rC_BDM_DrugListRecord.set_id_type("CIP");
          rC_BDM_DrugListRecord.set_owner(owner());
          rC_BDM_DrugListRecord.set_lang(lang());
          rC_BDM_DrugListRecord.set_atc(atc_code);
          rC_BDM_DrugListRecord.set_dci_1Name(dciList[0]);
          rC_BDM_DrugListRecord.set_dci_2Name(dciList[1]);
          rC_BDM_DrugListRecord.set_dci_3Name(dciList[2]);
          rC_BDM_DrugListRecord.set_ucd(ucd_code);
          rC_BDM_DrugListRecord.set_ucd_price(prix);
          rC_BDM_DrugListRecord.set_generique(generique);
          rC_BDM_DrugListRecord.set_statut(statut);
          ++m_position;
          return 1;
        }  // endif (query.isActive() )
        return 0;
}

//------------------------------ getIndicationsFromCIP ------------------------------------------
/*
// SMR ASMR selon cip
SELECT t00.f2 as cip,
       t3C.f0 as id_indic,
       t00.f3 as drugName,
       t3C.f3 as niveau,
       t3E.f1 as libelleNiveau,
       t3E.f2 as infoNiveau,
       t3C.f4 as indication,
       t3C.f2 as type
 FROM
       t3C,
       t3E,
       t00,
       t4B
where t4B.f0=t00.f0
and   t00.f2='3646903'       -- lovenox
and   t3C.f1=t4B.f0
and   t3C.f3=t3E.f0
-- and   t3C.f2='A'
limit 0,300

// indications DATASEMP from cip
 SELECT t52.f0 ,
        t00.f3 as drugName,
        t50.f1
 FROM   t52,
        t00,
        t50
 WHERE t52.f1 = t00.f0
 AND t52.f2 >= 2993
 AND t52.f2 <= 2995
 AND t50.f0 = t52.f0
 -- and t00.f3 like '%LOVE%'
 and t00.f2 = '3646926'
limit 0,100

// indications CIM10 from cip
SELECT t00.f2 as cip,
       t52.f2 as niveau,
       t1C.f1 as libelleNiveau,
       t00.f3 as drug_name,
       t50.f1 AS CI_Vidal,
       t4N.f2 AS CIM_X,
       t4N.f3 AS codeCIM_X
FROM t52
INNER JOIN t4B ON t52.f1 = t4B.f0
INNER JOIN t1C ON t52.f2 = t1C.f0  -- libelle
INNER JOIN t50 ON t52.f0 = t50.f0,
t55,
t4N,
t00
WHERE t00.f0 = t4B.f0
-- AND t00.f3 LIKE 'LOVE%'
AND t00.f2='3646926'       -- lovenox
AND t55.f1 = t4N.f0
AND t55.f0 = t50.f0
AND t52.f2 >= 2993        -- t52 selection les indications ou ci du produit en relation avec t00.f0
AND t52.f2 <= 2995
-- AND t4N.f3 like 'K27%' -- 'K27'
limit 0,3000
*/
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
QList <C_BDM_IndicationRecord> C_BDM_BaseModelePlugin::getIndicationsFromCIP( const QString &cip,   int filter )
 {
 QList <C_BDM_IndicationRecord> retList;
 retList         = C_BDM_BaseModelePlugin::Datasemp_getIndications_FromCIP(cip, filter );
 if ( filter&C_BDM_PluginI::ASMR || filter&C_BDM_PluginI::SMR )
    {retList += Datasemp_getASMR_SMR_FromCIP(cip, filter);
    }
 return retList;
}

//------------------------------ getCIPFromIndication ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code       indication code of the drugs from wich wee want datas.
 *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
 *  \return QStringList cip result list
 */
QStringList C_BDM_BaseModelePlugin::getCIPFromIndication( const QString &code, const QString &codeType )
{QStringList retList;
 if (codeType=="CIM")                               retList +=  Datasemp_get_CIP_From_CIM10(code);
 else if (codeType == "DS_I")                       retList +=  Datasemp_get_CIP_From_DatasempIndicationCode(code);
 else if (codeType == "DS_A"||codeType == "DS_S")   retList +=  Datasemp_get_CIP_From_ASMR_SMR_Code(code, codeType );
 return retList;
}

//------------------------------ getMonographieFromCIP ------------------------------------------
/*! \brief select monographie for a drug.
 *  \param const QString &cip            code CIP of the drug from wich wee want datas.
 *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
 *  \return QString contains monographie
 */
QString  C_BDM_BaseModelePlugin::getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ )
{
  Q_UNUSED(dataMustBeReadInCache);

  QString requete("");
  // 2  intitule du medoc ______________________________
  // 1  Donnees XML ____________________________        |
  // 0  Code unite de vente __________          |       |
  //                                  |         |       |
  requete +=       " SELECT     t00.f1,   t4B.f5, t4B.f2 "
                   " FROM       t00,t4B                          "
                   " WHERE      t00.f0 = t4B.f0                  "
                   " AND        t00.f2 = '" + cip + "'" ;
  QSqlQuery query(requete , database() );
  if (! (query.isActive() && query.next()))                             return QString::null;

  QString mono = Datasemp_GetMonographie( query.value(0).toString() );
  if (mono.length()) return mono;

  //................. si pas de monographie vidal pour ce produit...................................................
  //                  creer une page html a  partir des infos XML
  mono = CGestIni::Utf8_Query(query, 1);  // recuperer les donnes XML
  if ( mono.length() )
     { Datasemp_Xml_to_html(mono, CGestIni::Utf8_Query(query, 2));
     }
  return mono;
}

//-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
/*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuillères a caf\303\251 etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with unit form
 */
/*
SELECT  UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t07.f4        as qu,
       t24.f1        as unit,
       t24.f5        as unitPlur
FROM  t07,
      t27,
      t08,
      t00,
      t24
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f3 = 1
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
AND   t00.f3 like '%LOVENOX%'
ORDER BY t00.f3
LIMIT 0,30000
 */
QString  C_BDM_BaseModelePlugin::getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName /* = 0 */)
{Q_UNUSED(commercialName);
 QString ret ="";
 QString requete = QString (
        " SELECT t24.f1                 \n"   //  as natureName
        " FROM   t24,                   \n"
        "        t00                    \n"
        " WHERE  t00.f2 = '%1'          \n"  // -- 3690548 morphine   -- 3575327 actifed
        " AND    t00.f7 = t24.f0        \n"
      ).arg(cip);
QSqlQuery query(requete , database() );
if (query.isActive() && query.next())
   {ret = CGestIni::Utf8_Query(query, 0);
   }
if (ret.length()) return ret;
//............. plan B avec les poso ......................
requete = QString ( " SELECT                   \n"
                    " t24.f1                   \n"  // as QuUnit
                    " FROM                     \n"
                    " t24,                     \n"
                    " t2P,                     \n"
                    " t00                      \n"
                    " WHERE  t2P.f1 = t00.f0   \n"
                    " AND    t24.f0 = t2P.f8   \n"
                    " AND    t00.f2 = '%1'     \n"  //IN ('3364062', '3176159', '3902447') -- 3364062 / advil    3176159 / HICONCIL
                ).arg(cip);
query.exec(requete );
if (query.isActive() && query.next())
   {ret = CGestIni::Utf8_Query(query, 0);
   }
return ret;
}
//-------------------------------- getFormeFromCIP -----------------------------------------------------------------
/*
SELECT t25.f1  as natureName,
        t00.f2 as Cip
 FROM   t25,
        t00,
        t4B
 WHERE  t00.f2 = '3364062'           -- 3364062 advil susp buvable -- 3690548 morphine   -- 3575327 actifed
 AND    t25.f0 = t4B.f1
 AND    t00.f0 = t4B.f0

SELECT  *
 FROM   t25,
        t00,
        t4B,
        t08
 WHERE  t00.f2 = '3364062'           -- 3364062 advil susp buvable -- 3690548 morphine   -- 3575327 actifed
 AND    t25.f0 = t4B.f1
 AND    t00.f0 = t4B.f0
 AND    t08.f0 = t4B.f0
 limit 0,10000

SELECT  *
 FROM   -- t25,
        -- t24,
        t00,
        t4B,
        t08
 WHERE  t00.f2 = '3364062'           -- 3364062 advil susp buvable -- 3690548 morphine   -- 3575327 actifed
 -- AND    t25.f0 = t4B.f1
 AND    t00.f0 = t4B.f0
 AND    t08.f0 = t4B.f0
 -- AND    t24.f0 = t08.f8
 limit 0,10000

 */
/*! \brief return drug galenic presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with presentation form
 */
QString  C_BDM_BaseModelePlugin::getFormeFromCIP ( const QString &cip)
{    QString requete = QString (
                                " SELECT t25.f1                 \n"   //  as natureName
                                " FROM   t25,                   \n"
                                "        t00                    \n"
                                " WHERE  t00.f2 = '%1'          \n"  // -- 3690548 morphine   -- 3575327 actifed
                                " AND    t25.f0 = t4B.f1        \n"
                              ).arg(cip);
    QSqlQuery query(requete , database() );
    if (query.isActive() && query.next())
       {return CGestIni::Utf8_Query(query, 0);
       }
    return "";
}
//-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
/*
SELECT t00.f2       as cip,
       t00.f3       as drugName,
       t15.f1       as voie
FROM   t00
      ,t0A
      ,t15
WHERE  t0A.f0=t00.f0
AND    t15.f0=t0A.f1
AND    t00.f2='5610708'
*/
/*! \brief return the drug route from cip.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with the drug route
 */
QString   C_BDM_BaseModelePlugin::getDrugRouteFromCIP ( const QString &cip)
{QString requete = QString (
                             " SELECT               \n"
                             "        t15.f1        \n"    // as voie
                             " FROM   t00           \n"
                             "       ,t0A           \n"
                             "       ,t15           \n"
                             " WHERE  t0A.f0=t00.f0 \n"
                             " AND    t15.f0=t0A.f1 \n"
                             " AND    t00.f2='%1'   \n"
                           ).arg(cip);
 QString ret = "";
 QSqlQuery query(requete , database() );
 if    ( !query.isActive() ) return ret;
 while (  query.next() )
       { ret += CGestIni::Utf8_Query(query, 0) + ", ";
       }
 ret.chop(2);
 return ret;
}

///////////////////////////////////////////////////////// DATASEMP //////////////////////////////////////////////////////////////


//--------------------------- get_SubstancePere -----------------------------------------------------------
/*! \brief return father libelle or a substance
 *  \param const QString code from substance.
 *  \return libelle of father substance or empty if not.
*/
QString C_BDM_BaseModelePlugin::get_SubstancePere(const QString &code)
{   return "";
    QString requete = QString (
                              " SELECT   UPPER(t27.f2)    \n"         // 0   libelle de la substance
                              " FROM  t27,                \n"
                              "       t59                 \n"
                              " WHERE t59.f0 = '%1'       \n"         // code du fils
                              " AND   t59.f1 = t27.f0     \n"         // code pere lie a la table des substances
                             ).arg(code);
  QSqlQuery query(requete , database() );
  if (! query.isActive())                             return "";
  QString ret = "";
  while (query.next())   // on suppose que la derniere occurence est la bonne substance exemple : cordarone -->chorydrate puis amiodarone
      { ret = Utf8_Query(query, 0);
      }
  return ret;
}
//-------------------------------- code_CPG_to_DCI_1_2_3 ------------------------------------------------------
/*
SELECT UPPER(t27.f2) as SubName
FROM  t07,
      t27,
      t08
WHERE t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = '202489'       -- COLCHIMAX
ORDER BY t07.f3

SELECT UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t07.f4        as qu,
       t24.f5        as unit
FROM  t07,
      t27,
      t08,
      t00,
      t24
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
AND   t00.f3 like '%COLCHIMAX%'
ORDER BY t00.f3
*/
QStringList C_BDM_BaseModelePlugin::code_CPG_to_DCI_1_2_3(const QString &codeCPG)    // code cpg t00.f0
{   QStringList retList;
    /*
    QString requete = QString (
                                " SELECT   UPPER(t27.f2)    \n"         // 0
                                " FROM  t07,                \n"
                                "       t27,                \n"
                                "       t08                 \n"
                                " WHERE t07.f0 = '%1'       \n"
                                " AND   t27.f0 = t07.f2     \n"
                                " AND   t07.f1 = t08.f1     \n"
                                " AND   t08.f1 = '0'        \n"
                                " AND   t08.f0 = '%1'       \n"
                                " ORDER BY t07.f3           \n"
                              ).arg(codeCPG);
    */
    QString requete = QString (
                             // " SELECT   t07.f4,          \n"         // 0   quantite
                             // "          t24.f1,          \n"         // 1   unite quantite au singulier
                                " SELECT   UPPER(t27.f2),   \n"         // 2   0 libelle de la substance
                             // "          t24.f5,          \n"         // 3   unite quantite au pluriel
                                "          t27.f0           \n"         // 4   1 code de la substance
                                " FROM  t07,                \n"
                                "       t27,                \n"
                                "       t08                 \n"
                         //     "       t24                 \n"
                         //     "       t00                 \n"
                         //     " WHERE t00.f2 = '%1'       \n"    // t00.f2 as cip,
                         //     " WHERE t00.f0 = '%1'       \n"    // t00.f0 as cpg,
                         //     " AND   t07.f0 = t00.f0     \n"
                                " WHERE t07.f0 = '%1'       \n"
                                " AND   t27.f0 = t07.f2     \n"
                                " AND   t07.f1 = t08.f1     \n"
                                " AND   t08.f1 = '0'        \n"
                                " AND   t08.f0 = t07.f0     \n"
                         //     " AND   t24.f0 = t07.f5     \n"
                                " ORDER BY t07.f3           \n"
                               ).arg(codeCPG);
    QSqlQuery query(requete , database() );
    if (! query.isActive())                             return retList;
    while (query.next())
       { QString   substance   = get_SubstancePere( query.value(1).toString() );  // on tente la substance pere (vaut mieux exploiter le nommage le plus simple du pere sans les acides chlorydrates ..)
         if (substance.length()==0) substance = CGestIni::Utf8_Query(query, 0);
         retList.append ( substance );
       }
    retList.append("");     // on rajoute 3 vides pour que la liste ait toujours au moins trois donnees
    retList.append("");
    retList.append("");
    return  retList;
    /*
    QString requete = QString (
                                " SELECT   UPPER(t27.f2)    \n"         // 0
                                " FROM  t11,                \n"
                                "       t27                 \n"
                                " WHERE t11.f0 = '%1'       \n"
                                " AND   t11.f1 = t27.f0     \n"
                              ).arg(codeCPG);

    QSqlQuery query(requete , database() );
    if (! query.isActive())                             return retList;
    while (query.next())
       { retList.append ( CGestIni::Utf8_Query(query, 0) );
       }
    retList.append("");     // on rajoute 3 vides pour que la liste ai toujours au moins trois donnees
    retList.append("");
    retList.append("");
    return  retList;
  */
}

//-------------------------------- code_CPG_to_StatutGenerique ------------------------------------------------------
/*
RAPPEL   DU CONTENU DE CETTE TABLE DE LIENS
Vous pouvez d\303\251terminer si une sp\303\251cialit\303\251 appartient au r\303\251pertoire des g\303\251n\303\251riques en \303\251tant soit
     -   un r\303\251f\303\251rent (t3F.f2=1)
     -   un g\303\251n\303\251rique (t3F.f2=6)
     -   un quasi-g\303\251n\303\251rique (t3F.f2=7).
RAPPEL   SUR LES QUASI-GENERIQUES
Sp\303\251cialit\303\251 substituable « S » conform\303\251ment au D\303\251cret 2011-149 du 3 f\303\251vrier 2011 relatif \303\240
l?inscription au r\303\251pertoire des groupes g\303\251n\303\251riques des sp\303\251cialit\303\251s qui se pr\303\251sentent sous une forme
pharmaceutique orale \303\240 lib\303\251ration modifi\303\251e diff\303\251rente de celle de la sp\303\251cialit\303\251 de r\303\251f\303\251rence.


Table 3F : Relation entre produit (r\303\251pertoire des g\303\251n\303\251riques et sp\303\251cialit\303\251
pharmaceutique proche)
Relation entre produit (r\303\251pertoire des g\303\251n\303\251riques et sp\303\251cialit\303\251 pharmaceutique proche). Une
sp\303\251cialit\303\251 pharmaceutique proche est une sp\303\251cialit\303\251 de même composition en substance(s)
active(s) de même dosage(s) de même forme(s) et même voie d'administration
     Code                          Description                             Type       Taille Relation
 1           Code du produit                                          [8] Texte
     0                                                                                6          4B.0
             Code du produit membre d'un groupe.                      longueur fixe
             Code du groupe ou sp\303\251cialit\303\251 proche
 2                                                                    [8] Texte
     1       Code du groupe g\303\251n\303\251rique ou de la sp\303\251cialit\303\251                             6          4B.0
                                                                      longueur fixe
             pharmaceutique proche
             Champ de bits qualifiant la relation.
             Champ de bits qualifiant la relation. 1 : R\303\251f\303\251rent 6 :   [2] Entier
     2                                                                                0          1F.0
             G\303\251n\303\251rique 7 : Substituable 8 : Sp\303\251cialit\303\251 pharmaceutique Octet
             proche
             Pointeur \303\251ventuel vers un commentaire
                                                                      [5] Entier long
             Certaines \303\251quivalences sont accompagn\303\251es d'un
     3                                                                (4 octets       0          1C.0
             commentaire, conserv\303\251 dans la table 1C. Dans ce cas, ce
                                                                      binaires)
             champ en contient le code.
             Commentaires sur la participation du produit au groupe   [L] Structure
     5                                                                                0
             Commentaires sur la participation du produit au groupe.  SGML
             Date de mise \303\240 jour de la fiche technique
     C                                                                [S] Date        0          3F.5
             Date de dernière modification du contenu du champ 5.

*/
QString C_BDM_BaseModelePlugin::code_CPG_to_StatutGenerique(const QString &codeCPG)
{   int s;
    QSqlQuery query (QString("SELECT f2 FROM t3F WHERE f0 ='%1'").arg(codeCPG) , database() );
    if (query.isActive() && query.next())
       {s = query.value(0).toInt();
        switch(s)
              { case 1: return "R";
                case 6: return "G";
                case 7: return "S";
              }
       }
    return "";
}
//-------------------------------- code_CPG_to_code_UCD ------------------------------------------------------
QString C_BDM_BaseModelePlugin::code_CPG_to_code_UCD(const QString &codeCPG)
{   QSqlQuery query (QString("SELECT f1 FROM t0K WHERE f0 ='%1'").arg(codeCPG) , database() );
    if (query.isActive() && query.next())
       {return query.value(0).toString();
       }
    return "";
}
//---------------------------- code_UV_to_dispVilleHopital ---------------------------------------
/*
SELECT t00.f2 as Cip,
       t00.f3 as DrugName,
       t25.f1 as natureName,
       t4B.fA as ATC,
       t02.f2 as codeLieux,
       t1A.f1 as lieux,
       -- t34.f1 as disponibilite,
--       t63.fE as hosto,          -- == HOSTO ->1   != HOSTO --> 0
       t01.f1 as Prix
FROM  t00
     ,t4B
     ,t25
     ,t02
     ,t1A
--     ,t34
--     ,t63
     ,t01
WHERE  t02.f0  = t00.f1
AND    t02.f3  = '0'      -- doit etre disponible
AND    t4B.fA != 'NULL'   -- si c'est un medoc faut un code ATC
AND    t00.f0  = t4B.f0
AND    t25.f0  = t4B.f1
AND     t1A.f0  = t02.f2
-- AND    t34.f0  = t02.f3
-- AND    t00.f1  = t63.f0
AND    t01.f0  = t00.f1
-- AND    t63.fE  = "0"      -- == HOSTO ->1   != HOSTO --> 0
ORDER  by t00.f3
LIMIT 0,30000
*/
QString C_BDM_BaseModelePlugin::code_UV_to_dispVilleHopital(const QString &codeUV)
{
    QString requete = QString (
                               " SELECT                   \n"
                               "        t02.f2            \n" // 2 = hopitaux as codeLieux,
                               " FROM   t00               \n"
                               "       ,t02               \n"
                               " WHERE  t02.f0  = t00.f1  \n"
                               " AND    t00.f1  = '%1'    \n"
                              ).arg(codeUV);
    int statut = 4;
    QSqlQuery query (requete , database() );
    if (query.isActive() && query.next())
       { statut = query.value(0).toInt();
         if (statut==2) return "H";
       }
    return "VH";
}
//--------------------------------- Datasemp_LevelToLevelStr ------------------------------------------------------------
/*
case 2999:      // CI absolue
case 2991:      // effet ind\303\251sirable information
case 2988:      // Mise en garde
case 2990:      //  precaution d'emploi
case 2996:      // CI relative
*/
QString   C_BDM_BaseModelePlugin::Datasemp_LevelToLevelStr(int niveau)
{ switch (niveau)
  { case  2991: return "0";         // effet indesirable
    case  2990: return "1";         // precaution d'emploi
    case  2988: return "2";         // mise en garde
    case  2996: return "3";         // Contre-indication relative (deconseillee)
    case  2999: return "4";         // Contre-indication absolue
    default:    return "4";
  }
}
//--------------------------------- Datasemp_LevelToLevelInt ------------------------------------------------------------
int   C_BDM_BaseModelePlugin::Datasemp_LevelToLevelInt(int niveau)
{ switch (niveau)
  { case  2991: return 0;         // effet indesirable
    case  2990: return 1;         // precaution d'emploi
    case  2988: return 2;         // mise en garde
    case  2996: return 3;         // Contre-indication relative (deconseillee)
    case  2999: return 4;         // Contre-indication absolue
    default:    return 4;
  }
}
//--------------------------------- Utf8_Query ------------------------------------------------------------
QString C_BDM_BaseModelePlugin::Utf8_Query(QSqlQuery &cur, int field)
{ return CGestIni::Utf8_Query(&cur, field);
}

//-----------------------------------------------------  Datasemp_SetHTMLFontProperty -------------------------------------------
void C_BDM_BaseModelePlugin::Datasemp_SetHTMLFontProperty(const QString &family, const QString &titleSize, const QString &chapitreSize, const QString &paragraphSize, const QString &normalText)
{  m_FontFamily         = family;
   m_FontTitleSize      = titleSize;
   m_FontchapitreSize   = chapitreSize;
   m_FontparagraphSize  = paragraphSize;
   m_FontnormalTextSize = normalText;
}
//------------------------------ Datasemp_get_CIP_From_DatasempIndicationCode ------------------------------------------
/*! \brief select cip list from datasemp indication code
 *  \param const QString &code     datasemp indication code from wich wee want datas.
 *  \return QStringList cip result list
 */
QStringList C_BDM_BaseModelePlugin::Datasemp_get_CIP_From_DatasempIndicationCode(const QString &code , bool codesAreInList /* = false */)
{
  QStringList retList;
  //.............. on cherche les codes vidal ..................................
  QString requete =
                             " SELECT t00.f2" // 0 as cip
                             " FROM   t52   "
                             "       ,t00   "
                             " WHERE  t52.f1 = t00.f0  "
                             " AND    t52.f2 >= 2993   "
                             " AND    t52.f2 <= 2995   ";
         if (codesAreInList||code.startsWith("'"))
             requete +=  QString (  " AND    t52.f0 IN (%1)  " ).arg(code);    // indication datasemp code sous forme de liste
         else
             requete +=  QString (  " AND    t52.f0 = '%1'  "  ).arg(code);    // indication datasemp code

  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { retList << query.value(0).toString();
        }
 return retList;
}
//------------------------------ Datasemp_get_CIP_From_CIM10 ------------------------------------------
/*! \brief select cip list from cim10 code
 *  \param const QString &code      cim10 code from wich wee want datas.
 *  \return QStringList cip result list
 */
QStringList C_BDM_BaseModelePlugin::Datasemp_get_CIP_From_CIM10(const QString &code)
{   QStringList  ds_codes;
    //............... on cherche les codes indications datasemp correspondants a ce code cim10 .........
    // bool         run = true;
    int pos          = -1;
    QString codeCimX = code;
    int       testNb = 0;
    do {  //.......... rechercher les codes indications Datasemp a partir des codes CimX .........................
          //           car seuls les codes indications Datasemp sont en relation avec les CIP

          QString requete =       " SELECT DISTINCT(t52.f0)   \n"     // 0 as codeDS,
                                  " FROM   t52,               \n"
                                  "        t55,               \n"
                                  "        t4N                \n"
                                  " WHERE  t55.f1  = t4N.f0   \n"
                                  " AND    t52.f0  = t55.f0   \n"
                                  " AND    t52.f2 >= 2993     \n"
                                  " AND    t52.f2 <= 2995     \n";
          pos = codeCimX.indexOf('-');      // K20-K31
          if (pos==-1)
             { requete += QString (" AND    t4N.f3  = '%1'     \n").arg(codeCimX);                                                   // code cim 10
             }
          else
             { requete += QString (" AND    t4N.f3 >='%1' AND t4N.f3 <= '%2'     \n").arg(codeCimX.left(pos),codeCimX.mid(pos+1));   // code cim 10
             }

          QSqlQuery query(requete , database() );
          if    ( !query.isActive() ) return QStringList();
          while (  query.next() )
                { ds_codes << query.value(0).toString();
                }
          if (ds_codes.size()==0)                                    //--> si pas de resultat on tente avec le pere du code cimX
             { codeCimX = Datasemp_get_Father_From_CIM10(codeCimX);
               if (codeCimX.length()==0) testNb = 2; //run = false;  // si pas de pere on arrete la recherche et on ne reboucle pas
               ++ testNb;                                            // on compte le nombre de fois que le pere est recheche (car une fois suffit)
             }
          else testNb = 2; // run = false;                           //--> Ok on a trouve on se casse
       } while ( /* run &&  */ testNb <= 1);                         // testNb <= 1 car on ne remonte que d'un pere
    if (ds_codes.size()==0)          return QStringList();
    //......... on cherche les cip correspondants aux indications datasemp ..............
    return  Datasemp_get_CIP_From_DatasempIndicationCode( "'"+ds_codes.join("','")+"'", true );   // true pour : on passe une liste de cip et non un cip isole
}

//------------------------------ Datasemp_get_Father_From_CIM10 ------------------------------------------
/*! \brief return ascendant cimX code from a cimX code
 *  \param const QString &code  cimX code from wich we want father
 *  \return QString father cimX code
 */
QString C_BDM_BaseModelePlugin::Datasemp_get_Father_From_CIM10(const QString &code)
{
     QString requete = QString ( " SELECT    ts.f3                 \n" //as Cim_Pere
                           //      "        ,ts.f2 as Libelle_Pere \n"
                           //      "        ,td.f3 as Cim_Test     \n"
                           //      "        ,td.f2 as Libelle_Test \n"
                                 " FROM   t4S,                   \n"
                                 "        t4N ts,                \n"
                                 "        t4N td                 \n"
                                 " WHERE  t4S.f0 = ts.f0         \n"
                                 " AND    td.f3  = '%1'          \n"
                                 " AND    t4S.f1 = td.f0         \n"
                               ).arg(code);
     QSqlQuery query(requete , database() );
     if    ( !query.isActive() ) return "";
     while (  query.next() )
           { return query.value(0).toString();
           }
     return "";
}

//------------------------------ Datasemp_get_CIP_From_ASMR_SMR_Code ------------------------------------------
/*! \brief select cip list from  ASMR SMR code.
 *  \param const QString &code      Datasemp code ASMR or SMR from wich wee want datas.
 *  \param const QString &typCode =="DS_A" for ASMR code or typCode =="DS_S" for SMR code
 *  \return QStringList cip result list
 */
QStringList C_BDM_BaseModelePlugin::Datasemp_get_CIP_From_ASMR_SMR_Code(const QString &code, const QString &typCode )
{
  QStringList retList;
  QString requete = QString (
                             "SELECT t00.f2,"       // 0  as cip,
                             " FROM         "
                             "        t3C,  "
                             "        t00,  "
                             "        t4B   "
                             " where        t4B.f0=t00.f0               "
                             "        and   t3C.f0='%1'                 "  // -- code
                             "        and   t3C.f1=t4B.f0               "
                           ).arg(code);
      if ( typCode=="DS_A" )
         {    requete +=          "        and   (t3C.f2='A')  ";
         }
      else if ( typCode=="DS_S"  )
         {    requete +=          "        and   (t3C.f2='S')  ";
         }
  requete +=                      "        order by t3C.f2     ";
  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { retList << query.value(0).toString();
        }
 return retList;
}
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
QList <C_BDM_IndicationRecord> C_BDM_BaseModelePlugin::Datasemp_getIndications_FromCIP(const QString &cip, int filter )
{
  QList <C_BDM_IndicationRecord> retList;
  QList <C_BDM_IndicationRecord> cimList;
  //.............. on cherche les codes vidal ..................................
  QString requete = QString (
                             " SELECT t52.f0," // 0 as code ,
                             "        t50.f1 " // 1 as indication
                             " FROM   t52,   "
                             "        t00,   "
                             "        t50    "
                             " WHERE  t52.f1 = t00.f0  "
                             " AND    t52.f2 >= 2993   "
                             " AND    t52.f2 <= 2995   "
                             " AND    t50.f0 = t52.f0  "
                             " AND    t00.f2 = '%1'    "
                            ).arg(cip);

  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { retList.append( C_BDM_IndicationRecord( cip,                             // const QString &cip,
                                                  "",                              // const QString &date
                                                  "",                              // QString &level,
                                                  "",                              // QString &levelTyp,
                                                  "",                              // QString &levelLibelle,
                                                  CGestIni::Utf8_Query(query, 1),  // QString &indication,
                                                  "",                              // QString &descriptif,
                                                  query.value(0).toString(),       // QString &code,
                                                  "DS_I"                           // QString &codeTyp "DS_I"/indication datasemp
                                                )
                        );
       }
 //............... on cherche les codes cim10 correspondants a ce code vidal .........
 if ( filter&C_BDM_PluginI::CIM )
    { for (int i = 0; i < retList.size(); ++i)
          { cimList += Datasemp_get_CIM10_From_Datasemp_Indic(cip, retList.at(i).code() );
          }
    }
 if      ( filter&C_BDM_PluginI::CIM  && filter&C_BDM_PluginI::BDM)  return retList+cimList;
 else if ( filter&C_BDM_PluginI::BDM)                                return retList;
 else if ( filter&C_BDM_PluginI::CIM)                                return cimList;
 else                                                                return retList;
}
//------------------------------ Datasemp_get_CIM10_From_Datasemp_Indic ------------------------------------------
/*! \brief select CIMX indications from datasemp indication code
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param const QString &code     datasemp indication code CIP of the drug from wich wee want datas.
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_BaseModelePlugin::Datasemp_get_CIM10_From_Datasemp_Indic(const QString &cip, const QString &code )
{   QList <C_BDM_IndicationRecord> retList;
    //............... on cherche les codes cim10 correspondants a ce code vidal .........
    QString requete = QString (
                                  " SELECT DISTINCT(t4N.f3),  "     // 0 as code CIM10,
                                  "        t52.f0,            "     // 1 as codeDS,
                                  "        t52.f2,            "     // 2 as niveau,
                                  "        t50.f1,            "     // 3 AS libelle_CI_Vidal,
                                  "        t4N.f2             "     // 4 AS libelle_CIM_X
                                  " FROM   t52,               "
                                  "        t50,               "
                                  "        t55,               "
                                  "        t4N                "
                                  " WHERE  t55.f1 = t4N.f0    "
                                  " AND   t52.f0  = t55.f0    "
                                  " AND   t52.f2 >= 2993      "
                                  " AND   t52.f2 <= 2995      "
                                  " AND   t50.f0  = t52.f0    "
                                  " AND   t52.f0  = '%1'      "
                                ).arg(code);               // code vidal
    QSqlQuery query(requete , database() );
    if    ( !query.isActive() ) return retList;
    while (  query.next() )
          { retList.append( C_BDM_IndicationRecord( cip,                              // const QString &cip,
                                                    "",                               // const QString &date
                                                    "",                               // QString &level,
                                                    "",                               // QString &levelTyp,
                                                    "",                               // QString &levelLibelle,
                                                    CGestIni::Utf8_Query(query, 4),   // QString &indication,query.value(4).toString()
                                                    "",                               // QString &descriptif,
                                                    query.value(0).toString(),        // QString &code,
                                                    "CIM"                             // QString &codeTyp "DS_I"/indication datasemp
                                                  )
                          );
          }
    return retList;
}

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
QList <C_BDM_IndicationRecord> C_BDM_BaseModelePlugin::Datasemp_getASMR_SMR_FromCIP(const QString &cip, int filter )
{
  QList <C_BDM_IndicationRecord> retList;
  QString requete = QString (
                             "SELECT t00.f2,"       // 0  as cip,
                             "       t3C.f0,"       // 1  as id_indic,
                             "       t00.f3,"       // 2  as drugName,
                             "       t3C.f3,"       // 3  as niveau,
                             "       t3E.f1,"       // 4  as libelleNiveau,
                             "       t3E.f2,"       // 5  as infoNiveau,
                             "       t3C.f4,"       // 6  as indication,
                             "       t3C.f2,"       // 7  as type A ou S
                             "       t3C.f5,"       // 8  as date
                             "       t3C.f8 "       // 9  as memo
                             " FROM         "
                             "        t3C,  "
                             "        t3E,  "
                             "        t00,  "
                             "        t4B   "
                             " where  t4B.f0=t00.f0                     "
                             "        and   t00.f2='%1'                 "  // -- cip
                             "        and   t3C.f1=t4B.f0               "
                             "        and   t3C.f3=t3E.f0               "
                           ).arg(cip);
      if ( filter&C_BDM_PluginI::ASMR && filter&C_BDM_PluginI::SMR )
         {    requete +=          "        and   (t3C.f2='A' or t3C.f2='S')  ";
         }
      else if ( filter&C_BDM_PluginI::ASMR )
         {    requete +=          "        and   (t3C.f2='A')  ";
         }
      else if ( filter&C_BDM_PluginI::SMR )
         {    requete +=          "        and   (t3C.f2='S')  ";
         }
  requete +=                      "        order by t3C.f2     ";
  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { QString levelTyp  = query.value(7).toString();
          retList.append( C_BDM_IndicationRecord( cip,                             // const QString &cip,
                                                  query.value(8).toString(),       // const QString &date
                                                  (levelTyp=="A")? "A"+Datasemp_ASMR_Level_To_level (query.value(3).toString())
                                                                 : "S"+query.value(3).toString(), // QString &level,
                                                  levelTyp,                        // QString &levelTyp A ou S
                                                  CGestIni::Utf8_Query(query, 4),  // query.value(4).toString(),       // QString &levelLibelle,
                                                  query.value(6).toString(),       // QString &indication,CGestIni::Utf8_Query(query, 6)
                                                  CGestIni::Utf8_Query(query, 8),  // QString &descriptif,
                                                  query.value(1).toString(),       // QString &code,
                                                  "DS_"+levelTyp                   // QString &codeTyp "DS_A"/indication ASMR    "DS_S"/indication SMR
                                                )
                        );
       }
 return retList;
}

//------------------------------ Datasemp_ASMR_Level_To_level ------------------------------------------
/*! \brief return normalised ASMR number between  0 to 4 from datasem level
 *  \param const QString &level            datasem level
 *  \return QString contains normalised level
 */
QString  C_BDM_BaseModelePlugin::Datasemp_ASMR_Level_To_level (    const QString &level)
{switch(level.toInt())
    { case 105 : return "0";   // pas d'amelioration
      case 104 : return "1";   // amelioration mineure
      case 103 : return "2";   // modere
      case 102 : return "3";   // importante
      case 101 : return "4";   // majeure
      default  : return  "";   // chais pas c'est quoi
    }

}
//-----------------------------------------------------  Datasemp_GetMonographie -------------------------------------------
//                  la norme vidal est que les noms des images des mononographies sont en majuscules : src="IMAGES/B00/C01/D35.GIF"
QString   C_BDM_BaseModelePlugin::Datasemp_GetMonographie(const QString &code_UV)
{ QString  result = "";
  int       f     = -1;
  //................. chercher si existe une  monographie vidal pour ce produit...................................................
  //                              recuperer code document de la monographie
  QSqlQuery query (QString( " SELECT t5C.f3          "
                            " FROM   t5D, t5C        "
                            " WHERE  t5C.f0 = t5D.f1 "
                            " AND    t5D.f0 = '%1'").arg( code_UV ) , database() );

  if (query.isActive() &&query.next())                           result     =  query.value(0).toString();
  if ( (f=result.indexOf("<html",0, Qt::CaseInsensitive)) > 0)   result.remove (0, f );
  return result;
}

//-------------------------------------- Datasemp_SGML_to_html -------------------------------------------------------
QString C_BDM_BaseModelePlugin::Datasemp_SGML_to_html( QString& input, QString  name  /* = QString::null */ )
{
 int xmlDepth      = -1;
 //................ convertir les retours chariot en trucs non XML (ne doivent pas etres interprÃ©tÃ©s) ................
 input.replace("<BR/>","{{@}}");
 QDomDocument doc( "mydocument" );
 if ( !doc.setContent( input ) ) return "";

 // input =  "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
 //         "<body style=\"font-size:16pt;font-family:Arial\">";

 if (name.length())
    { input += "<p align=\"center\"><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-style:italic;font-weight:600;color:#AA0000\">";
      input += name  +  "</span></p> <hr size=\"2\" />";
    }
 Datasemp_Xml_ListChild(input, doc.documentElement(), xmlDepth );
 // input     += "</body></html>";
 input.replace("\303\242","\'");
 return input;
}
//-------------------------------------- Datasemp_Xml_to_html -------------------------------------------------------
void C_BDM_BaseModelePlugin::Datasemp_Xml_to_html( QString& input, QString  name  /* = QString::null */ )
{
 int xmlDepth      = -1;
 //................ convertir les retours chariot en trucs non XML (ne doivent pas etres interprÃ©tÃ©s) ................
 input.replace("<BR/>","{{@}}");
 QDomDocument doc( "mydocument" );
 if ( !doc.setContent( input ) ) return;

 input =  "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
          "<body style=\"font-size:16pt;font-family:Arial\">";

 if (name != QString::null)
    {input += "<p align=\"center\"><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-style:italic;font-weight:600;color:#AA0000\">";
     input += name  +  "</span></p> <hr size=\"2\" />";
    }
 Datasemp_Xml_ListChild(input, doc.documentElement(), xmlDepth );
 input     += "</body></html>";
 input.replace("\303\242","\'");
}

//-------------------------------------- Datasemp_Xml_ListChild -------------------------------------------------------
void C_BDM_BaseModelePlugin::Datasemp_Xml_ListChild(QString& input, const QDomElement &parentElement, int &xmlDepth )
{   ++xmlDepth;
    QString ofset = "";
    for (int i=0; i<xmlDepth; i++) ofset += "\t";
    QDomNode node = parentElement.firstChild();
    while ( !node.isNull() )
       {if ( node.isElement() /* && node.nodeName() == "outline" */)
           {QDomElement e   = node.toElement();
            QString text    = e.text();
            QString tagName = e.tagName();
            QString title   = e.attribute("TITRE");
            int         tag = 1;
            if (tagName=="RUBRIQUE")
               {input +=  ofset + "<br /><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-weight:600;text-decoration:underline;color:#AA0000\">";
                input +=  title + "</span><br />";
               }
            else if (tagName=="SSRUBR")
               {title.replace(">", "&gt;");
                title.replace("<", "&lt;");
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontchapitreSize+"pt;font-weight:600;text-decoration:underline;color:#000000\">";
                input += title + "</span><br />";
               }
            else if (tagName=="CAS")
               {title.replace(">", "&gt;");
                title.replace("<", "&lt;");
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontparagraphSize+"pt;font-weight:600;color:#0000FF\">";
                input += title + "</span><br />";
               }
            else tag=0;
            if (tag && text!="")
               {text.replace(">", "&gt;");
                text.replace("<", "&lt;");
                text.replace("{{@}}", " <br>" + ofset); // replacer les retours chariots normaux
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontnormalTextSize+"pt;color:#00007F\">";
                input += text  + "</span><br />";
               }
            //........... APPEL RECURSIF ...........
            Datasemp_Xml_ListChild( input, node.toElement(), xmlDepth );
           }
        node = node.nextSibling();
       }
    --xmlDepth;
}

//----------------------------------------------- Datasemp_GetBaseVersion ---------------------------------------------------------------------
QString C_BDM_BaseModelePlugin::Datasemp_GetBaseVersion( QString *numEdition, QString *packBCArrete /* = 0 */, QString *packIRArrete /* = 0 */)
{ QString ods("");
  if      (packBCArrete ) *packBCArrete = "";
  if      (packIRArrete)  *packIRArrete = "";
  if      (numEdition)    *numEdition = "";

  //.................. Preparer la requete .....................................
  QSqlQuery query("SELECT f0,f1 FROM tFB", database() );
  //.................. si la requete a un resultat .............................
  if (query.isActive())
     {while (query.next())
            {if      (packBCArrete && query.value(0).toString()=="ARRETE PACK BC") *packBCArrete = query.value(1).toString();
             else if (packIRArrete && query.value(0).toString()=="ARRETE PACK IR") *packIRArrete = query.value(1).toString();
             else if (numEdition   && query.value(0).toString()=="EDITION NUMERO") *numEdition   = query.value(1).toString();
             else if (query.value(0).toString()=="ODS")                             ods          = query.value(1).toString();
            }
     }
  QString err;
  // OutSQL_error( query, "Datasemp_GetBaseVersion()", "SELECT f0,f1, FROM tFB", &err);
  return ods.remove("#00");
}


