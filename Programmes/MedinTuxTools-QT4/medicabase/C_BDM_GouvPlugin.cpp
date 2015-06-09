
#include "C_BDM_GouvPlugin.h"
#include <QDir>
#include <QSqlDatabase>

//-------------------------------- C_BDM_GouvPlugin ------------------------------------------------------

C_BDM_GouvPlugin::C_BDM_GouvPlugin(const QString &confDatas, const QString &prefixLabel, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
: C_BaseCommon(parent) , C_BDM_PluginI()
{ m_dataSourceVersion             = "No database drugs";     // TODO  en attendant le numero provenant de la BD datasemp
  m_pluginName                    = "C_BDM_GouvPlugin";      // TODO  change this name
  m_pQSqlQuery                    = 0;
  setInitialisedSate(false);

  if (logWidget)        { C_Log::setLogWidget(logWidget);
                        }
  if (pQProgressBar)    { C_BaseCommon::setProgressWidget(pQProgressBar);
                        }
  if ( init (confDatas, prefixLabel) == 0 )
     { outMessage(  C_BaseCommon::getLastError() );
     }
}

//-------------------------------- ~C_BDM_GouvPlugin ------------------------------------------------------
C_BDM_GouvPlugin::~C_BDM_GouvPlugin()
{ if (m_pathAppli.length()) m_pathAppli = "";
  if (m_pQSqlQuery) delete m_pQSqlQuery;
}

//-------------------------------- init ------------------------------------------------------
int C_BDM_GouvPlugin::init ( const QString &confData     , const QString &prefixLabel)
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
    m_pQSqlQuery  = new QSqlQuery (QString::null , database() );
    if (dbi_init_database("dbi_"+plugin_name(), prefixLabel, "GB", this)==0)
       { setLastError (tr("%1::init() error in dbi_init_database()\n").arg(plugin_name()));
         return 0;
       }
    setInitialisedSate(1);
    return 1;
}

//-------------------------------- setLogWidget ------------------------------------------------------
void C_BDM_GouvPlugin::setLogWidget      ( QTextEdit *logWidget )
{C_Log::setLogWidget(logWidget);
}
//-------------------------------- setProgressWidget ------------------------------------------------------
void C_BDM_GouvPlugin::setProgressWidget ( QProgressBar *pQProgressBar )
{C_BaseCommon::setProgressWidget(pQProgressBar);
}

//-------------------------------- progressWidget ------------------------------------------------------
QProgressBar* C_BDM_GouvPlugin::progressWidget()
{ return C_BaseCommon::progressWidget();
}
//-------------------------------- dataSourceVersion ------------------------------------------------------
QString  C_BDM_GouvPlugin::dataSourceVersion()
{
    QSqlQuery query (QString("SELECT gouv_statut_version FROM bdm_gouv.gouv_statut") , database() );
    if (query.isActive() && query.next())
       { m_dataSourceVersion = "Base BDMP : "+query.value(0).toString();
       }
    return m_dataSourceVersion;
}

//-------------------------------- lang ------------------------------------------------------
QString  C_BDM_GouvPlugin::lang()
{ return "fr";
}
//-------------------------------- owner ------------------------------------------------------
QString  C_BDM_GouvPlugin::owner()
{ return "OWN";
}
//-------------------------------- getLastError ------------------------------------------------------
QString  C_BDM_GouvPlugin::getLastError()
{return C_BaseCommon::getLastError();
}
//-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
/*! \brief return if  plugin is HAS certified
 *  \return return HAS certified status
 */
bool C_BDM_GouvPlugin::is_plugin_HAS_certified()
{ return false;
}
//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_GouvPlugin::plugin_name()
{ return m_pluginName ;
}

//---------------------------------------- get_database -------------------------------------------------------
/*! \brief  return active database. This facility is to do extended SQL querys which are not in API
 *  \return  database
 */
QSqlDatabase C_BDM_GouvPlugin::get_database()
{ return database();
}

//-------------------------------- plugin_image_source ------------------------------------------------------
/*! \brief  return image source url for monographies
 *  \return return image source url for monographies
 */
QString C_BDM_GouvPlugin::plugin_image_source()
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
QList<C_CompositionItem> C_BDM_GouvPlugin::getCompositionFromCIP(const QString &cip)
{ QList <C_CompositionItem> result;
  QString requete = QString ( " SELECT gouv_cis_compo_libelle_subst,"
                              "        gouv_cis_compo_dosage_subst,"
                              "        gouv_cis_compo_link"
                              " FROM   gouv_cis_cip,"
                              "        gouv_cis,"
                              "        gouv_cis_compo"
                              " where  gouv_cis.gouv_cis_code_cis=gouv_cis_cip.gouv_cis_cip_code_cis"
                              " and    gouv_cis_compo.gouv_cis_compo_code_cis=gouv_cis_cip.gouv_cis_cip_code_cis"
                              " and    gouv_cis_compo_dosage_subst != ''"          // dosage non vide
                              " and    gouv_cis_cip_code_cip7 = '%1'"
                              " order by gouv_cis_compo_link" ).arg(cip);
   QSqlQuery query(requete , database() );
  if (!query.isActive())
     { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::get_CIS_and_RCP_From_CIP() </b></font>"), requete, __FILE__, __LINE__  );
       return result;
     }
  //........ ASTUCE on mappe le produit avec son numero d'ordre (link)...............
  //         afin d'eviter les doublons de substance (ST SA) pouvant
  //         sortir (rappel : la clef d'une QMap est unique)
  QMap <int,QString> mapProd;
  while (query.next() )
     {  int     link  = query.value(2).toInt();
        mapProd[link] = CGestIni::Utf8_Query(query, 0)+"_"+query.value(1).toString();  // unir nom substance et dosage
     }
  //.......... on itere la QMap sur tous les produits .........................
  QMapIterator<int, QString> it(mapProd);
  while (it.hasNext())
      { it.next();
        QStringList pL  = it.value().split('_');  // separer nom substance et dosage
        QString produit = pL[0];
        QString dosage  = pL[1];

        //.......... normaliser produit.............
        int pos               = produit.indexOf('(');
        int len               = dosage.length();
        if (pos !=-1) produit = produit.left(pos).trimmed();
        pos                   = produit.indexOf("SODIQUE");
        if (pos !=-1) produit = produit.left(pos).trimmed();

        produit = produit.remove(" BASE").trimmed();
        produit = produit.remove(" ANHYDRE").trimmed();
        produit = produit.remove(tr("TRIHYDRAT\303\211E")).trimmed();
        produit = produit.remove("SODIQUE").trimmed();
        produit = produit.remove("CHLORHYDRATE D'").trimmed();
        produit = produit.remove("CHLORHYDRATE DE").trimmed();
        produit = produit.remove(tr("H\303\211MIFUMARATE DE")).trimmed();
        produit = produit.remove("CITRATE DE").trimmed();
        produit = produit.remove("SULFATE DE").trimmed();
        produit = produit.remove("PHOSPHATE DE").trimmed();
        produit = produit.remove(tr("AC303\211TATE DE")).trimmed();
        produit = produit.remove(tr("H\303\211MIHYDRAT\303\211E")).trimmed();
        /*
        DICHLORHYDRATE DE
        CITRATE DE
        SULFATE DE
        MONOHYDRATÉ
        MONOSODIQUE
        HYDRATÉ
        BROMHYDRATE DE
        ACÉTATE DE
        HÉMIFUMARATE DE
        */
        pos                   = 0;
        while ( pos<len && ((dosage[pos]>='0'&& dosage[pos]<='9') || dosage[pos]==',' || dosage[pos]=='.'|| dosage[pos]==' ') ) ++pos;
        result.append  ( C_CompositionItem ( produit                       ,
                                             dosage.left(pos).remove(' ')  ,
                                             dosage.mid(pos).trimmed()     ,
                                             ""
                                           )
                       );
     }  //while (it.hasNext())
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
QList<C_ExcipientItem> C_BDM_GouvPlugin::getExcipientsFromCIP(const QString &cip)
{   Q_UNUSED(cip);
    QList<C_ExcipientItem>        result;
    return result;
}
//-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QStringList result list with dci composants
 */
QStringList C_BDM_GouvPlugin::get_DCI_list_FromCIP(const QString &cip)
{   QString cis         = get_CIS_and_RCP_From_CIP(cip);
    QStringList retList = code_CIS_to_DCI_1_2_3( cis,0);
    return retList;
}

//--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
/*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
 *  \param const QString cip is drug cip code.
 *  \param QString *rcp is pointer on QString to return RCP result if not zero .
 *  \return CIS code string
 */
QString C_BDM_GouvPlugin::get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp /* =0 */)
{   if (rcp) *rcp = "";
    QString requete = QString(" SELECT gouv_cis_cip_code_cis FROM gouv_cis_cip where gouv_cis_cip_code_cip7='%1'").arg(cip);
    QSqlQuery query(requete , database() );
    outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::get_CIS_and_RCP_From_CIP() </b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {return query.value(0).toString();
       }
    return "";
}

//-------------------------------- isStupefiant ------------------------------------------------------
/*! \brief return if the product identified par Id is a stupefiant
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return 1 is stupefiant
 */
int C_BDM_GouvPlugin::isStupefiant ( const QString &cip )
{ Q_UNUSED(cip);
  return 0;
}

//-------------------------------- code_ATC_To_Name ------------------------------------------------------
/*! \brief return if ATC libelle from code ATC
 *  \param const QString &code      code ATC .
 *  \return QString return libelle
 */
QString C_BDM_GouvPlugin::code_ATC_To_Name(const QString &code)
{   QSqlQuery query (QString("SELECT bdm_atc_code FROM bdm_atc WHERE bdm_atc_libelle ='%1'").arg(code) , database() );
    if (query.isActive() && query.next())
       {return CGestIni::Utf8_Query(query, 0);
       }
    return "";
}

//-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
/*! \brief return the drug route from cip.
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with the drug route
 */
QString   C_BDM_GouvPlugin::getDrugRouteFromCIP ( const QString &cip)
{ QString requete = QString (  " SELECT gouv_cis_voie"
                               " FROM   gouv_cis_cip,"
                               "        gouv_cis"
                               " where  gouv_cis_code_cis=gouv_cis_cip_code_cis"
                               " and    gouv_cis_cip_code_cip7 = '%1'").arg(cip);
  QString ret = "";
  QSqlQuery query(requete , database() );
  if    ( !query.isActive() )
        { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::getDrugRouteFromCIP() </b></font>"), requete, __FILE__, __LINE__  );
          return ret;
        }
  while (  query.next() )
        { ret = CGestIni::Utf8_Query(query, 0);
          ret = ret.replace(';',',');
          return ret;
        }
  return ret;
}

//-------------------------------- getPosologiesFromCIP ------------------------------------------------------
/*! \brief return posologies list as posologie grammar QString
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
 *  \return QStringList with grammar QString list
 */
QStringList   C_BDM_GouvPlugin::getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip )
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
QList<C_InterResultPair> C_BDM_GouvPlugin::getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  /* = "" */  )
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
QList<C_InterResultPair> C_BDM_GouvPlugin::getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{ Q_UNUSED(cipList);
  Q_UNUSED(pC_PatientCtx);
  QList<C_InterResultPair>            interResultPairList;
  return interResultPairList;
}
//-------------------------------- getAllergiesFromCipList ------------------------------------------------------
/*! \brief return Allergies interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GouvPlugin::getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{Q_UNUSED(cipList);
 Q_UNUSED(pC_PatientCtx);
 QList<C_InterResultPair> interResultPairList;
 return interResultPairList;
}
//-------------------------------- getInteractionRedundancy ------------------------------------------------------
/*! \brief return redundancy composition in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_GouvPlugin::getInteractionRedundancy( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{
    Q_UNUSED(pC_PatientCtx);
    QList<C_InterResultPair>            interResultPairList;
    if ( cipList.length() <= 1 ) return interResultPairList;         // il faut au moins deux produits pour une redondance
    INTER_STRUCT inter;
    QMap      <QString, INTER_STRUCT> subst_MultiMap;          // liste associant key = nom de la substance  data = INTER_STRUCT donnee du produit contenant cette substance
    QMap      <QString, QString>      subst_DistinctMap;       // liste des subtances QMap pour avoir unicite des noms
    QString   substName = "";
    //.......... creer la liste des substances de l'ordonnance ..........................................
    QString s_cip_list = cipList.join("','");
    QString requete = QString (  " SELECT gouv_cis_cip_code_cip7,"         // 0   as cip
                                 "        gouv_cis_denomination,"          // 1   as drug_name
                                 "        gouv_cis_compo_libelle_subst,"   // 2   as subst_Name
                                 "        gouv_cis_compo_code_subst"       // 3   as subst_code
                                 " FROM   gouv_cis_cip,"
                                 "        gouv_cis,"
                                 "        gouv_cis_compo"
                                 " WHERE  gouv_cis_code_cis      = gouv_cis_cip_code_cis"
                                 " AND    gouv_cis_cip_code_cis  = gouv_cis_compo_code_cis"
                                 " AND    gouv_cis_cip_code_cip7 in ('%1')").arg(s_cip_list);

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
               { QString title = tr(" <b>%1</b> redondances de subtance possibles ").arg(QString::number(nb));
                 QString avec  = "";
                 for (int i=0; i<inter_list.size(); ++i)
                     {avec += "<br/><b>" + inter_list[i].m_drugName+"</b>";
                     }
                 interResultPairList.append( C_InterResultPair( ""       , "REDON", title,
                                                                ""       , "REDON", substName,
                                                                tr("%1 produits avec la même substance <b>%2</b> ont \303\251t\303\251 trouv\303\251s :<br/>").arg(QString::number(nb), substName) + avec + "<br/>","REDONDANCES", QString::number(nb)
                                                              )
                                           );

               }  // if ( inter_list.size() >= 2 )
          } // while (it.hasNext())
    return interResultPairList;
}

//-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
/*! \brief test compatibility with maximum and minimun authorized dose and duration from a drug and patient context.
 *  \param C_PatientCtx *pC_PatientCtx
 *  \param const QString &grammar prescription for this drug
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>  C_BDM_GouvPlugin::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)
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
QList<C_InterResultPair> C_BDM_GouvPlugin::getInteractionPairFromCipList ( const QStringList &cipList)
{
      QList<C_InterResultPair>            interResultPairList;
      if ( cipList.length() <= 1 ) return interResultPairList;         // il faut au moins deux produits pour une interaction
      PRODUCT_CIP inter;
      QMap      <QString, PRODUCT_CIP>  cip_subst_Inter_Map;           // liste associant key = nom de la substance  data = INTER_STRUCT donnee du produit contenant cette substance
      QMap      <QString, QString>      cip_subst_couple_Map;          // liste associant key = nom de la substance  data = INTER_STRUCT donnee du produit contenant cette substance
      //.......... creer la liste des couples de substance de l'ordonnance ..........................................
      QString s_cip_list = cipList.join("','");
      QString key1       = "";
      QString key2       = "";
      QString couple     = "";
      QStringList coupleL;
      QString requete = QString (  " SELECT gouv_cis_cip_code_cip7"          // 0   as cip
                                   "       ,gouv_cis_denomination"           // 1   as drug_name
                                   "       ,gouv_cis_compo_libelle_subst"    // 2   as subst_Name
                              //   "       ,gouv_cis_compo_code_subst"       // 3   as subst_code
                                   " FROM   gouv_cis_cip,"
                                   "        gouv_cis,"
                                   "        gouv_cis_compo"
                                   " WHERE  gouv_cis_code_cis      = gouv_cis_cip_code_cis"
                                   " AND    gouv_cis_cip_code_cis  = gouv_cis_compo_code_cis"
                                   " AND    gouv_cis_cip_code_cip7 in ('%1')").arg(s_cip_list);

      QSqlQuery query(requete , database() );
      if    ( !query.isActive() ) return interResultPairList;

      while (  query.next() )
         { inter.m_cip                   = query.value(0).toString();                  // CIP
           inter.m_drugName              = CGestIni::Utf8_Query(query, 1);
           QString prod_str              = CGestIni::Utf8_Query(query, 2).toUpper();
           int pos                       = prod_str.indexOf('(');
           if (pos !=-1) inter.m_product = prod_str.left(pos).trimmed();
           else          inter.m_product = prod_str;
           //........ mapper le produit avec clef unique .....................
           //         cip.code-produit
           key1             = inter.m_cip+"."+inter.m_product;
           cip_subst_Inter_Map.insert( key1, inter);       // liste des couples uniques 'cip.code-produit'
         }
      //........ accoupler cette clef avec les autres  .....................
      //         pour former les couples de test :
      //         - eliminer les BB ou AA
      //         - la liste des couples est unique, 'AB' est equivalent a 'BA'
      //           donc pour unififier :
      //                si on tombe sur BA on fait si A<B --> AB
      //                A   B    C    D
      //                AB       AD
      //                         BC   BD
      //                CA            CD
      //                DA       DC
      //
      QList<PRODUCT_CIP> interList = cip_subst_Inter_Map.values();
      for   (int i=0;i<interList.size();++i)
            { //......... si AB ou BA ne conserver que AB .........
              inter = interList[i];
              key1  = inter.m_cip+"."+inter.m_product;
              for   (int u=0;u<interList.size();++u)
                    { inter = interList[u];
                      key2  = inter.m_cip+"."+inter.m_product;
                      if (key2 == key1) continue;
                      if (key1 <  key2) couple = key1+"|"+key2;
                      else              couple = key2+"|"+key1;
                      cip_subst_couple_Map.insert(couple, couple);     // on se fout de la value la QMap est juste pour le tri et l'unicite
                    }
            }
      //......... parser toutes la liste des couples de produits et verfier si .......................
      //          ils interactent entre eux
      QMapIterator<QString, QString> it(cip_subst_couple_Map);
      while (it.hasNext())
            { it.next();
              couple                = it.key();
              coupleL               = couple.split("|");
              key1                  = coupleL[0];
              key2                  = coupleL[1];
              PRODUCT_CIP produit1  = cip_subst_Inter_Map[key1];
              PRODUCT_CIP produit2  = cip_subst_Inter_Map[key2];
              //................... rechercher si les substances des deux produits ...............
              //                    existent dans le systeme d'interaction (pas certain car sources differentes)
              QStringList pk1_List   = isThisValueLikeInTable_ToList( "it_product",
                                                                      "it_product_libelle",
                                                                       produit1.m_product);
              QStringList pk2_List   = isThisValueLikeInTable_ToList( "it_product",
                                                                      "it_product_libelle",
                                                                       produit2.m_product);
              int nbpk1              = pk1_List.count();  // la liste ne devrait contenir qu'un element
              int nbpk2              = pk2_List.count();  // la liste ne devrait contenir qu'un element
              if (nbpk1==0)  {pk1_List =tryToFindProduct(produit1.m_product); nbpk1 = pk1_List.count();}
              if (nbpk2==0)  {pk2_List =tryToFindProduct(produit2.m_product); nbpk2 = pk2_List.count();}
              if (nbpk1==0 || nbpk2==0) continue;
              interResultPairList << getInteractionList(produit1, produit2, pk1_List[0], pk2_List[0]);
             } // while (it.hasNext())
  return interResultPairList;
}
//--------------------------- tryToFindProduct -----------------------------------------------------------
QStringList C_BDM_GouvPlugin::tryToFindProduct(QString produit)
{ produit = produit.replace('('," ");
  produit = produit.replace(')'," ");
  produit = produit.replace('/'," ");
  produit = produit.remove(" BASE").trimmed();
  produit = produit.remove(" ANHYDRE").trimmed();
  produit = produit.remove(tr("TRIHYDRAT\303\211E")).trimmed();
  produit = produit.remove("SODIQUE").trimmed();
  produit = produit.remove("CHLORHYDRATE D'").trimmed();
  produit = produit.remove("CHLORHYDRATE DE").trimmed();
  produit = produit.remove(tr("H\303\211MIFUMARATE DE")).trimmed();
  produit = produit.remove("CITRATE DE").trimmed();
  produit = produit.remove("SULFATE DE").trimmed();
  produit = produit.remove("PHOSPHATE DE").trimmed();
  produit = produit.remove(tr("AC303\211TATE DE")).trimmed();
  produit = produit.remove(tr("H\303\211MIHYDRAT\303\211E")).trimmed();
  QStringList list = produit.split(' ');
  QStringList retlist;
  for (int i=0; i<list.size(); ++i)
       { retlist = isThisValueLikeInTable_ToList( "it_product",
                                        "it_product_libelle",
                                         list[0]);
       if (retlist.length()) return retlist;
       }
  return retlist;
}
//--------------------------- getInteractionList -----------------------------------------------------------
QList<C_InterResultPair>  C_BDM_GouvPlugin::getInteractionList(const PRODUCT_CIP &produit1, const PRODUCT_CIP &produit2,const QString &pk_prod1, const QString &pk_prod2)
{ QList<C_InterResultPair>            interResultPairList;
  QString requete;
  //.................... liste des classes d'interaction  .........................
  //                     pour chacun des produits on recherche les classes d'interactions
  //                     auquelles il appartient
  requete  = QString( " SELECT     d1.it_classe_def_pk         ,"      // 0
                      "            d1.it_classe_def_name       ,"      // 1
                      "            m1.it_classe_mbr_product_pk ,"      // 2
                      "            d1.it_classe_def_memo        "      // 3
                      " FROM                               "
                      "            it_classe_mbr m1        "
                      "           ,it_classe_def d1        "
                      " where                              "
                      " (m1.it_classe_mbr_product_pk = '%1' or "
                      "  m1.it_classe_mbr_product_pk = '%2')   "
                      "  and  d1.it_classe_def_pk    = m1.it_classe_mbr_classe_def_pk ").arg (pk_prod1, pk_prod2);

  QStringList pk_prod1_ItClassList;
  QStringList pk_prod2_ItClassList;
  QMap  <QString, QString>   classMapMemo;  // relevera les memo de chaque classe d'interaction
  QSqlQuery query1(requete , database() );

  if ( ! query1.exec(requete) )
     { outSQL_error( query1, "ERREUR  : query1 getInteractionList()", requete, __FILE__, __LINE__);
       return interResultPairList;
     }
  if ( ! query1.isActive() )
     { outSQL_error( query1, "ERREUR  : query1 getInteractionList()", requete, __FILE__, __LINE__);
       return interResultPairList;
     }

  while ( query1.next() )
        { QString class_pk   = query1.value(0).toString();
          QString class_name = query1.value(1).toString();
          QString prod_pk    = query1.value(2).toString();      //
          QString memo       = query1.value(3).toString();      //
          if ( prod_pk == pk_prod1 ) pk_prod1_ItClassList.append( class_pk );
          else                       pk_prod2_ItClassList.append( class_pk );
          classMapMemo[class_pk]= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b><u>"+class_name+"</u></b> "+memo;   // relever le memo lie au pk de cette classe
        }
  //..................... pour chaque element de la liste des classes d'interactions ..................................
  //                      croiser en recherchant une interaction avec l'autre liste
  // 1-........ accoupler cette clef avec les autres  .....................
  //         pour former les couples de test :
  //         - eliminer les BB ou AA
  //         - la liste des couples est unique, 'AB' est equivalent a 'BA'
  //           donc pour unififier :
  //                si on tombe sur BA on fait si A<B --> AB
  //                A   B    C    D
  //                AB       AD
  //                         BC   BD
  //                CA            CD
  //                DA       DC
  //
  QString key1   = "";
  QString key2   = "";
  QString couple = "";
  QMap  <QString, int> coupleMap; // QMap pour preserver l'unicite des couples
  for   (int i=0; i<pk_prod1_ItClassList.size(); ++i)
        { //......... si AB ou BA ne conserver que AB .........
          key1  = pk_prod1_ItClassList[i];
          for   (int u=0; u<pk_prod2_ItClassList.size(); ++u)
                { key2 = pk_prod2_ItClassList[u];
                  if (key2 == key1) continue;
                  if (key1 <  key2) couple = key1+"|"+key2;
                  else              couple = key2+"|"+key1;
                  coupleMap.insert(couple, 0);     // on se fout de la value la QMap est juste pour le tri et l'unicite des couples
                }
        }

  // 2A-........ pour chaque couple de clef chercher une interaction .....................
  //             version avec IN (   ,   ,   )
  QString keyslist = "'";
  QMapIterator<QString, int> it(coupleMap);
  while (it.hasNext())
        { it.next();
          keyslist += it.key() +"','";
        }
  keyslist.chop(2);
  if (keyslist.trimmed().length()==0) return interResultPairList;

  requete   = QString( " SELECT     l1.it_classe_lnk_mbr_pk1     ,"      // 0
                       "            l1.it_classe_lnk_mbr_pk2     ,"      // 1
                       "            l1.it_classe_lnk_level       ,"      // 2
                       "            t_mecanism.it_memo_text      ,"      // 3
                       "            t_todo.it_memo_text           "      // 4
                       " FROM                                     "
                       "            it_classe_lnk  l1             "
                       "           ,it_memo        t_mecanism     "
                       "           ,it_memo        t_todo         "
                       " where                                    "
                       "     l1.it_classe_lnk_pair_pk1Pk2 IN (%1) "
                       " AND l1.it_classe_lnk_mecanism_mpk = t_mecanism.it_memo_pk"
                       " AND l1.it_classe_lnk_todo_mpk     = t_todo.it_memo_pk").arg (keyslist);
  QSqlQuery query2(requete , database() );

  if ( ! query2.exec(requete) )
     { outSQL_error( query2, "ERREUR  : query2 getInteractionList()", requete, __FILE__, __LINE__);
       return interResultPairList;
     }
  if ( ! query2.isActive() )
     { outSQL_error( query2, "ERREUR  : query2 getInteractionList()", requete, __FILE__, __LINE__);
       return interResultPairList;
     }
  while ( query2.next() )
        { QString pk1        = query2.value(0).toString();
          QString pk2        = query2.value(1).toString();
          QString level      = query2.value(2).toString();
          QString mecanism   = query2.value(3).toString();
          QString todo       = query2.value(4).toString().trimmed();
          if (produit1.m_cip != produit2.m_cip)      // ne pas tenir compte des produits type : DUOPLAVIN=ASPIRINE/PLAVIX qui interactent dans le meme produit
             { interResultPairList.append(
                                           C_InterResultPair ( produit1.m_cip , "CIP", produit1.m_drugName,
                                                               produit2.m_cip , "CIP", produit2.m_drugName,
                                                               tr("<b><u>Produits concern\303\251s<u> :</b><br/>")   + produit1.m_product  +" et "  + produit2.m_product + "<br/>"+
                                                               tr("<b><u>Classes concern\303\251es<u> :</b><br/>")   +
                                                               classMapMemo[pk1] +"<br/>"+
                                                               classMapMemo[pk2] +"<br/>"+
                                                               tr("<b><u>M\303\251canisme<u> :</b><br/>")            + mecanism+ "<br/>"+
                                                               (todo.length()?tr("<b><u>Conduite \303\240 tenir<u> :</b><br/>") + todo:""),
                                                               "AFSSAPS", level
                                                             )
                                         );
             } // if (produit1.m_cip != produit2.m_cip)
        } // endwhile ( query2.next() )


  // 2B-........ pour chaque couple de clef chercher une interaction .....................
  //             version avec une requete a chaque iteration

  /*
  QString keyslist = "'";
  QMapIterator<QString, int> it(coupleMap);
  while (it.hasNext())
        { it.next();
          couple    = it.key();
          keyslist += couple +"','";
          requete   = QString( " SELECT     l1.it_classe_lnk_mbr_pk1     ,"      // 0
                               "            l1.it_classe_lnk_mbr_pk2     ,"      // 1
                               "            l1.it_classe_lnk_level       ,"      // 2
                               "            t_mecanism.it_memo_text      ,"      // 3
                               "            t_todo.it_memo_text           "      // 4
                               " FROM                                     "
                               "            it_classe_lnk  l1             "
                               "           ,it_memo        t_mecanism     "
                               "           ,it_memo        t_todo         "
                               " where                                    "
                               "     l1.it_classe_lnk_pair_pk1Pk2  = '%1' "
                               " AND l1.it_classe_lnk_mecanism_mpk = t_mecanism.it_memo_pk"
                               " AND l1.it_classe_lnk_todo_mpk     = t_todo.it_memo_pk").arg (couple);
          QSqlQuery query2(requete , database() );

          if ( ! query2.exec(requete) )
             { outSQL_error( query2, "ERREUR  : query2 getInteractionList()", requete, __FILE__, __LINE__);
               continue;
             }
          if ( ! query2.isActive() )
             { outSQL_error( query2, "ERREUR  : query2 getInteractionList()", requete, __FILE__, __LINE__);
               continue;
             }
          while ( query2.next() )
                { QString pk1        = query2.value(0).toString();
                  QString pk2        = query2.value(1).toString();
                  QString level      = query2.value(2).toString();
                  QString mecanism   = query2.value(3).toString();
                  QString todo       = query2.value(4).toString().trimmed();
                  if (produit1.m_cip != produit2.m_cip)                       // ne pas tenir compte des produits type : DUOPLAVIN=ASPIRINE/PLAVIX qui interactent dans le meme produit
                     { interResultPairList.append(
                                              C_InterResultPair ( produit1.m_cip , "CIP", produit1.m_drugName,
                                                                  produit2.m_cip , "CIP", produit2.m_drugName,
                                                                  tr("<b><u>Produits concern\303\251s<u> :</b><br/>")   + produit1.m_product  +" et "  + produit2.m_product + "<br/>"+
                                                                  tr("<b><u>Classes concern\303\251es<u> :</b><br/>")   +
                                                                  classMapMemo[pk1] +"<br/>"+
                                                                  classMapMemo[pk2] +"<br/>"+
                                                                  tr("<b><u>M\303\251canisme<u> :</b><br/>")            + mecanism+ "<br/>"+
                                                                  (todo.length()?tr("<b><u>Conduite \303\240 tenir<u> :</b><br/>") + todo:""),
                                                                  "AFSSAPS", level
                                                                )
                                             );
                     } //if (produit1.m_cip != produit2.m_cip)
                } // endwhile ( query2.next() )
       }
  */
  return interResultPairList;
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
QList<C_InterResultPair>  C_BDM_GouvPlugin::getInteractionAgeSexePoidsSportVigil( const QString &cip,
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
//-------------------------------- code_CIP_to_ATC ------------------------------------------------------.
QString          C_BDM_GouvPlugin::code_CIP_to_ATC(const QString &cip)
{   QString requete = QString( "SELECT bdm_cip_code_atc FROM bdm_gouv.bdm_cip where bdm_cip_cip7 = '%1'").arg(cip);
    QSqlQuery query(requete , database() );
    outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::code_CIP_to_ATC() </b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {return query.value(0).toString();
       }
    return "";
}

//-------------------------------- nextATC_Code ------------------------------------------------------.
int  C_BDM_GouvPlugin::nextATC_Code(QString &codeAtc, QString &libelle)
{
   if (codeAtc.length()==0 && libelle.length()==0)    // premiere fois, apres il devrait contenir le retour SQL
      {
       m_position = 0;
       if (!database().isOpen() && database().open() == false)
          { outMessage( tr("ERREUR : C_BDM_GouvPlugin::nextATC_Code() database can not be open"), __FILE__, __LINE__);  return 0;
          }
       QString     requete  =" SELECT bdm_atc_code, bdm_atc_libelle FROM bdm_atc ORDER BY bdm_atc_code";
       m_pQSqlQuery->exec(requete);
       if ( ! m_pQSqlQuery->isActive() )
          { outSQL_error(*m_pQSqlQuery, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::nextATC_Code()</b></font>"), requete, __FILE__, __LINE__  );
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

//-------------------------------- drugsList_Count ------------------------------------------------------.
long          C_BDM_GouvPlugin::drugsList_Count()
{
   m_position = 0;
   if (!database().isOpen() && database().open() == false)
      {outMessage( tr("ERREUR : C_BDM_GouvPlugin::drugsList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
      }
   QString requete  =
           " SELECT COUNT(*)    \n"
           " FROM   gouv_cis_cip,\n"
           "        gouv_cis\n"
           " WHERE  gouv_cis.gouv_cis_code_cis=gouv_cis_cip.gouv_cis_cip_code_cis\n"
      //     " AND    gouv_cis_cip_status_adm != 'Présentation abrogée'\n"
      //     " AND    gouv_cis_cip_status_com not like '%arrêt%'\n"
           " AND    gouv_cis_comm_statut not like 'Non%'\n";
   QSqlQuery query(requete , database() );
   outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::drugsList_Count() </b></font>"), requete, __FILE__, __LINE__  );
   if (query.isActive() && query.next())
      {return (long) query.value(0).toDouble();
      }
   return 0;
}

//------------------------------ getDrugList_Start ------------------------------------------
int           C_BDM_GouvPlugin::getDrugList_Start()
{   m_position      = 0;
    m_pQSqlQuery->exec( "ALTER TABLE `bdm_gouv`.`bdm_atc`                 ADD INDEX `ind_0` (`bdm_atc_code`)" );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_GouvPlugin::getDrugList_Start() create index <b>ind_0</b> for <b>bdm_atc_code</b> ", "");
    m_pQSqlQuery->exec( "ALTER TABLE `bdm_gouv`.`gouv_cis_cip`            ADD INDEX `ind_0` (`gouv_cis_cip_code_cis`)" );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_GouvPlugin::getDrugList_Start() create index <b>ind_0</b> for <b>gouv_cis_cip_code_cis</b> ", "");
    m_pQSqlQuery->exec( "ALTER TABLE `bdm_gouv`.`gouv_cis_cip`            ADD INDEX `ind_1` (`gouv_cis_cip_code_cip7`)" );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_GouvPlugin::getDrugList_Start() create index <b>ind_1</b> for <b>gouv_cis_cip_code_cip7</b> ", "");
    m_pQSqlQuery->exec( "ALTER TABLE `bdm_gouv`.`bdm_cip`                 ADD INDEX `ind_0` (`bdm_cip_cip7`)" );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_GouvPlugin::getDrugList_Start() create index <b>ind_0</b> for <b>bdm_cip_cip7</b> ", "");

    QString requete =
                     " SELECT"
                     "        gouv_cis.gouv_cis_code_cis,\n"             // 0
                     "        gouv_cis_cip.gouv_cis_cip_code_cip7,\n"    // 1
                     "        gouv_cis.gouv_cis_denomination,\n"         // 2
                     "        gouv_cis_cip.gouv_cis_cip_prix\n"          // 3
                     " FROM   gouv_cis_cip,\n"
                     "        gouv_cis\n"
                     " WHERE  gouv_cis.gouv_cis_code_cis=gouv_cis_cip.gouv_cis_cip_code_cis\n"
                //     " AND    gouv_cis_cip_status_adm != 'Présentation abrogée'\n"
                //     " AND    gouv_cis_cip_status_com not like '%arrêt%'\n"
                     " AND    gouv_cis_comm_statut not like 'Non%'\n"
                    ;
    m_pQSqlQuery->exec( requete );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_GouvPlugin::getDrugList_Start()", requete, __FILE__, __LINE__);
    if ( !m_pQSqlQuery->isActive() ) return 0;
    else                             return 1;
    return 0;
}

//------------------------------ getDrugList_NextRecord ------------------------------------------
int           C_BDM_GouvPlugin::getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord)
{ Q_UNUSED(rC_BDM_DrugListRecord);
    QStringList dciList;
    QString     code_cis  = "";
    QString     libelle   = "";
    QString     cip_code  = "";
    QString     nb_up     = "";
    QString     atc_code  = "";
    QString     prix      = "";
    QString     generique = "";
    QString     statut    = "VH";

  if (m_pQSqlQuery->next() && m_position < 200000)
     {code_cis     = m_pQSqlQuery->value(0).toString();
      cip_code     = m_pQSqlQuery->value(1).toString();
      libelle      = CGestIni::Utf8_Query(*m_pQSqlQuery, 2);
      atc_code     = code_CIP_to_ATC(cip_code);
      prix         = C_BDM_Api::deleteUnsignificativesZeros(convertPrixDecimalToCts(m_pQSqlQuery->value(3).toString()));
      //statut       = m_pQSqlQuery->value(5).toInt()==2?"H":"VH";
      //ucd_code     = code_CPG_to_code_UCD(code_cpg);
      generique    = code_CIS_to_StatutGenerique(code_cis);
      dciList      = code_CIS_to_DCI_1_2_3(code_cis);

      rC_BDM_DrugListRecord.set_commercialName(libelle);
      rC_BDM_DrugListRecord.set_id(cip_code);
      rC_BDM_DrugListRecord.set_id_type("CIP");
      rC_BDM_DrugListRecord.set_owner(owner());
      rC_BDM_DrugListRecord.set_lang(lang());
      rC_BDM_DrugListRecord.set_atc(atc_code);
      rC_BDM_DrugListRecord.set_dci_1Name(dciList[0]);
      rC_BDM_DrugListRecord.set_dci_2Name(dciList[1]);
      rC_BDM_DrugListRecord.set_dci_3Name(dciList[2]);
      rC_BDM_DrugListRecord.set_nb_up(nb_up);
      rC_BDM_DrugListRecord.set_ucd_price(prix);
      rC_BDM_DrugListRecord.set_generique(generique);
      rC_BDM_DrugListRecord.set_statut(statut);
      ++m_position;
      return 1;
    }  // endif (query.isActive() )
  return 0;
}
//------------------------------ getRefundedIndications ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
 *  \param const QString &code_type  code typ as giving by the drugs BDM
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_GouvPlugin::getRefundedIndications( const QString &code_indication,   const QString &code_type )
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
 QList <C_BDM_IndicationRecord> C_BDM_GouvPlugin::getIndicationsFromText( const QString &text,   int filter )
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
QList <C_BDM_IndicationRecord> C_BDM_GouvPlugin::getIndicationsFromCIP( const QString &cip,   int filter )
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
QStringList C_BDM_GouvPlugin::getCIPFromIndication( const QString &code, const QString &codeType )
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
QList<C_PrescriptionRestreinte> C_BDM_GouvPlugin::getPrescriptionRestreinteFromCIP(const QString &cip)
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
QString   C_BDM_GouvPlugin::getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ )
{
    Q_UNUSED(cip);
    Q_UNUSED(dataMustBeReadInCache);
    return "";
}

//-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
/*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres a cafe etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with unit form
 */
QString    C_BDM_GouvPlugin::getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName /* = 0 */)
{
 Q_UNUSED(commercialName);
 QString requete = QString (  " SELECT gouv_cis_forme"
                              " FROM   gouv_cis_cip,"
                              "        gouv_cis"
                              " where  gouv_cis_code_cis=gouv_cis_cip_code_cis"
                              " and    gouv_cis_cip_code_cip7 = '%1'").arg(cip);

 QString ret = "";
 QSqlQuery query(requete , database() );
 if    ( !query.isActive() )
       { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::getFormeFromCIP() </b></font>"), requete, __FILE__, __LINE__  );
         return ret;
       }
 while (  query.next() )
       { ret += CGestIni::Utf8_Query(query, 0);
       }
 return ret;
}

//-------------------------------- getFormeFromCIP ------------------------------------------------------
/*! \brief return drug presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with presentation form
 */
QString   C_BDM_GouvPlugin::getFormeFromCIP ( const QString &cip)
{QString requete = QString (  " SELECT gouv_cis_forme"
                              " FROM   gouv_cis_cip,"
                              "        gouv_cis"
                              " where  gouv_cis_code_cis=gouv_cis_cip_code_cis"
                              " and    gouv_cis_cip_code_cip7 = '%1'").arg(cip);

 QString ret = "";
 QSqlQuery query(requete , database() );
 if    ( !query.isActive() )
       { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::getFormeFromCIP() </b></font>"), requete, __FILE__, __LINE__  );
         return ret;
       }
 while (  query.next() )
       { ret += CGestIni::Utf8_Query(query, 0);
       }
 return ret;
}

//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_GouvPlugin::plugin_icon()
{/* XPM */
  //  QString path = m_pathAppli+"Ressources/Gouv BDM.png";
  //  return QPixmap(path);

    static char const * dossier_rouge_xpm[] =  {
"19 18 110 2",
"  	c #EAEAEA",
". 	c #F3F3F3",
"+ 	c #F1F0F3",
"@ 	c #BEB3F8",
"# 	c #9A8AF9",
"$ 	c #9482F9",
"% 	c #F7F7F7",
"& 	c #BFB5FA",
"* 	c #C0B5FA",
"= 	c #F8F8F8",
"- 	c #9B8BFA",
"; 	c #9582F9",
"> 	c #9483F5",
", 	c #A193F5",
"' 	c #9687F7",
") 	c #9282F9",
"! 	c #9382FB",
"~ 	c #9283F9",
"{ 	c #F9F9F9",
"] 	c #9482F7",
"^ 	c #C3BCE1",
"/ 	c #F6F5F4",
"( 	c #F3F2F9",
"_ 	c #C7BDF7",
": 	c #9C8CF7",
"< 	c #9182F8",
"[ 	c #9686EB",
"} 	c #D2CBCE",
"| 	c #F3EDEB",
"1 	c #FBF9F8",
"2 	c #FCFDFC",
"3 	c #F6EBEE",
"4 	c #C66B8D",
"5 	c #A27AD6",
"6 	c #9482F8",
"7 	c #FAFAFA",
"8 	c #9A7EE8",
"9 	c #B06FB2",
"0 	c #CC6B82",
"a 	c #E08F8C",
"b 	c #C0B8B8",
"c 	c #D7D0D0",
"d 	c #ECE2E0",
"e 	c #F7F2F1",
"f 	c #F2AA9D",
"g 	c #F37551",
"h 	c #F2775B",
"i 	c #CE6979",
"j 	c #9E7BDF",
"k 	c #9C7CE5",
"l 	c #E46E5E",
"m 	c #F47752",
"n 	c #F16947",
"o 	c #EE5A40",
"p 	c #E0D5D3",
"q 	c #B4A9A9",
"r 	c #BDB3B5",
"s 	c #DACDCB",
"t 	c #ED563E",
"u 	c #F14226",
"v 	c #EF4D30",
"w 	c #F47250",
"x 	c #D8676B",
"y 	c #9581F7",
"z 	c #FBFBFB",
"A 	c #9382F8",
"B 	c #9F6EB8",
"C 	c #EF4328",
"D 	c #EE4326",
"E 	c #F04228",
"F 	c #EE4325",
"G 	c #DBADA8",
"H 	c #BAB0BA",
"I 	c #8E829E",
"J 	c #906E93",
"K 	c #CE4540",
"L 	c #ED4225",
"M 	c #EF4228",
"N 	c #D94B3E",
"O 	c #9180F3",
"P 	c #8D77DA",
"Q 	c #C24D52",
"R 	c #DC4435",
"S 	c #C14A4E",
"T 	c #A4587E",
"U 	c #8B70BD",
"V 	c #887ADE",
"W 	c #9080F4",
"X 	c #9182F6",
"Y 	c #8B79DF",
"Z 	c #946098",
"` 	c #C64748",
" .	c #D3483F",
"..	c #986198",
"+.	c #9282F5",
"@.	c #FCFCFC",
"#.	c #8C79E1",
"$.	c #8976D9",
"%.	c #8E7FF0",
"&.	c #9381F7",
"*.	c #9481F7",
"=.	c #8B7BE9",
"-.	c #8677D8",
";.	c #9080F2",
">.	c #9581F8",
",.	c #FDFDFD",
"'.	c #FEFEFE",
").	c #C3B8FD",
"!.	c #FCFBFE",
"~.	c #FFFFFF",
"                                      ",
". + @ # $ $ $ $ $ $ $ $ $ $ # @ + . . ",
"% & $ $ $ $ $ $ $ $ $ $ $ $ $ $ * % % ",
"= - $ $ $ $ $ $ $ $ $ $ ; $ $ $ - = = ",
"= $ $ $ $ $ $ > , ' ) ! $ ~ ! $ $ = = ",
"{ $ $ $ $ $ ] ^ / ( _ : < ~ ; $ $ { { ",
"{ $ ) ! ; $ [ } | 1 2 3 4 5 $ ) 6 { { ",
"7 $ $ 8 9 0 a b c d e f g h i j $ 7 7 ",
"7 ! k l m n o p q r s t u v w x y 7 7 ",
"z A B C D E F G H I J K L M M N O z z ",
"z $ P Q R S T U V W X Y Z `  ...+.z z ",
"@.! $ #.$.%.&.$ 6 $ ! ) *.=.-.;.>.@.@.",
"@.$ ) $ $ ; $ $ $ $ $ $ $ $ $ $ $ @.@.",
",.$ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ ,.,.",
",.- $ $ $ $ $ $ $ $ $ $ $ $ $ $ - ,.,.",
"'.).$ $ $ $ $ $ $ $ $ $ $ $ $ $ ).'.'.",
"'.!.).- $ $ $ $ $ $ $ $ $ $ - ).!.'.'.",
"~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~."};
 return QPixmap(dossier_rouge_xpm);
}


//------------------------------ code_CIS_to_DCI_1_2_3 ------------------------------------------
QStringList C_BDM_GouvPlugin::code_CIS_to_DCI_1_2_3(const QString &code_cis, int completeTo3 /* = 1 */ )
{   QStringList ret_list;
    QString libelle = "";
    QString requete = QString (
            " SELECT gouv_cis_compo_libelle_subst "
            " FROM   gouv_cis_compo "
            " where  gouv_cis_compo_code_cis='%1' ").arg(code_cis)
           ;
    QSqlQuery query(requete , database() );

    if (!query.isActive())
       { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_GouvPlugin::code_CIS_to_DCI_1_2_3() </b></font>"), requete, __FILE__, __LINE__  );
         if (completeTo3)
            { ret_list.append("");     // on rajoute 3 vides pour que la liste ait toujours au moins trois donnees
              ret_list.append("");
              ret_list.append("");
            }
         return ret_list;
       }
    while (query.next())
       {  libelle = CGestIni::Utf8_Query(query, 0);
          ret_list.append(libelle);
       }
    if (completeTo3)
       { ret_list.append("");     // on rajoute 3 vides pour que la liste ait toujours au moins trois donnees
         ret_list.append("");
         ret_list.append("");
       }
    return ret_list;
}
//-------------------------------- convertPrixDecimalToCts ------------------------------------------------------
QString  C_BDM_GouvPlugin::convertPrixDecimalToCts(const QString &prix)
{QString  int_part = "";
 QString  dec_part = "";
 int             i = 0;
 int           len = prix.length();
 int           pos = prix.indexOf('.');
 if (pos==-1)  pos = prix.indexOf(',');
 if (pos==-1)                    // pas de partie d\303\251cimale exple 0012  12 faut rajouter deux zeros
    { int_part = prix;
      for ( i=0; i<len; ++i)   // enlever les zeros non significatifs de debut
          { if (int_part.toLatin1()[i]!='0')
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
          { if (int_part.toLatin1()[i]!='0')
               {break;
               }
          }
      int_part = int_part.mid(i);         // si i==len cas du "0000000" alors ret.mid(i) --> "" donc ok
      dec_part = prix.mid(pos+1) + "00";  // on y rajoute deux zeros quite a les couper apres
      dec_part = dec_part.left(2);        // on garde deux chiffres pour la partie decimale
      return int_part + dec_part;
    }
}
//------------------------------ code_CIS_to_StatutGenerique ------------------------------------------
QString C_BDM_GouvPlugin::code_CIS_to_StatutGenerique(const QString &code_cis)
{   int s;
    QString requete = QString(" SELECT gouv_cis_gener_type\n"
                              " FROM   gouv_cis_gener\n"
                              " where  gouv_cis_gener_code_cis='%1'\n").arg(code_cis);
    QSqlQuery query (requete , database() );
    if (query.isActive() && query.next())
       {s = query.value(0).toInt();
        switch(s)
              { case 1: return "R";
                case 2: return "G";
                case 4: return "S";
              }
       }
    return "";
}

