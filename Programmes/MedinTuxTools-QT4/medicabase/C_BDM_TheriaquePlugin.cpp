#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <float.h>

#include "C_BDM_TheriaquePlugin.h"
#include "C_PosologieGrammar.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"

//---------------------------- C_BDM_TheriaquePlugin ---------------------------------------
C_BDM_TheriaquePlugin::C_BDM_TheriaquePlugin(const QString &confDatas, const QString &prefixLabel, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
    : C_BaseCommon(parent), C_BDM_PluginI()
{ m_dataSourceVersion             = "09052012";  // TODO  en attendant le numero provenant de la BD theriaque
  m_pQSqlQuery                    = 0;
  m_pQSqlQuery2                   = 0;
  m_pQSqlQuery3                   = 0;
  m_firstTimeGetNextSmrInfoForCIP = true;

  if (logWidget)        { C_Log::setLogWidget(logWidget);
                        }
  if (pQProgressBar)    { C_BaseCommon::setProgressWidget(pQProgressBar);
                        }
  if (init(confDatas, prefixLabel)==0)
     {outMessage(  C_BaseCommon::getLastError());
      return;
     }
}

//-------------------------------- ~C_BDM_TheriaquePlugin ---------------------------------------
C_BDM_TheriaquePlugin::~C_BDM_TheriaquePlugin()
{if (m_pQSqlQuery)    delete m_pQSqlQuery;
 if (m_pQSqlQuery2)   delete m_pQSqlQuery2;
 if (m_pQSqlQuery3)   delete m_pQSqlQuery3;
 //if (m_pDbi_Database) delete m_pDbi_Database;  detruite dans destructeur du parent
}
//-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
/*! \brief return if  plugin is HAS certified
 *  \return return HAS certified status
 */
bool C_BDM_TheriaquePlugin::is_plugin_HAS_certified()
{return false;
}
//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_TheriaquePlugin::plugin_name()
{return "Theriaque";
}
//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_TheriaquePlugin::plugin_icon()
{ // return QPixmap(theriaque_xpm);     // marche po
 QString path = m_pathAppli+"Ressources/theriaque.png";
 return QPixmap(path);
}
//---------------------------------------- get_database -------------------------------------------------------
/*! \brief  return active database. This facility is to do extended SQL querys which are not in API
 *  \return  database
 */
QSqlDatabase C_BDM_TheriaquePlugin::get_database()
{ return database();
}
//-------------------------------- plugin_image_source ------------------------------------------------------
/*! \brief  return image source url for monographies
 *  \return return image source url for monographies
 */
QString C_BDM_TheriaquePlugin::plugin_image_source()
{   QString pathFile = CGestIni::Param_ReadUniqueParam(m_datasIni.toLatin1(),QString("Connexion_"+plugin_name()).toLatin1(), "PathImageSource");
    if ( QDir::isRelativePath ( pathFile ) ) pathFile  = QDir::cleanPath ( pathFile.prepend(m_pathAppli) );
    if (!pathFile.endsWith("/")) pathFile += "/";
    return pathFile;
}

//---------------------------- getLastError ---------------------------------------
QString C_BDM_TheriaquePlugin::getLastError()
{return C_BaseCommon::getLastError();
}

//---------------------------- init ---------------------------------------
int C_BDM_TheriaquePlugin::init(const QString &confData, const QString &prefixLabel)
{QString errMess           =   "";
 m_pathAppli               =   CGestIni::Param_ReadUniqueParam(confData.toLatin1(),"BDM_Configuration", "PathAppli");
 m_pathIni                 =   CGestIni::Param_ReadUniqueParam(confData.toLatin1(),"BDM_Configuration", "PathIni");
 setInitialisedSate(0);
 if (m_pathAppli.length()==0)
    {setLastError (tr("C_BDM_TheriaquePlugin::init() BDM_Configuration/PathAppli is empty"));
     return 0;
    }
 if (!QFile::exists ( m_pathAppli ))
    {setLastError (tr("C_BDM_TheriaquePlugin::init() BDM_Configuration/PathAppli don't exists"));
     return 0;
    }
 if (m_pathIni.length()==0)
    {setLastError (tr("C_BDM_TheriaquePlugin::init() BDM_Configuration/PathIni is empty"));
     return 0;
    }
 if (QDir::isRelativePath ( m_pathIni )) m_pathIni = m_pathIni.prepend(m_pathAppli);
 m_pathIni = QDir::cleanPath ( m_pathIni );
 if (!QFile::exists ( m_pathIni ))
    {setLastError (tr("C_BDM_TheriaquePlugin::init() BDM_Configuration/PathIni don't exists"));
     return 0;
    }
 m_datasIni       =  CGestIni::Param_UpdateFromDisk(m_pathIni);
 if (m_datasIni.length()==0)
    {setLastError (tr("C_BDM_TheriaquePlugin::init() m_datasIni is empty"));
     return 0;
    }
 if (BaseConnect(m_datasIni,  prefixLabel+"BDM_THERIAQUE", "theriaque", &errMess, "Connexion_Theriaque" )==0)
    {setLastError (tr("C_BDM_TheriaquePlugin::init() error in BDM connexion BaseConnect()\n%1").arg(errMess));
     return 0;
    }
 m_pQSqlQuery  = new QSqlQuery (QString::null , database() );
 m_pQSqlQuery2 = new QSqlQuery (QString::null , database() );
 m_pQSqlQuery3 = new QSqlQuery (QString::null , database() );

 if (dbi_init_database("dbi_theriaque", prefixLabel, "TH", this)==0)
    {setLastError (tr("C_BDM_TheriaquePlugin::init() error in dbi_init_database()\n"));
     return 0;
    }

 if (createAdditionnalTables()==0)
    {setLastError (tr("C_BDM_TheriaquePlugin::init() error in createAdditionnalTables()\n"));
     return 0;
    }
 setInitialisedSate(1);
 return 1;
}

//---------------------------- createAdditionnalTables ----------------------------------------------------------------------
int           C_BDM_TheriaquePlugin::createAdditionnalTables()
{   if (dbi_database()==0)  return 0;
    if (dbi_database()->isThisTableExist("my_cache_monographie") ) return 1;
    if (dbi_database()->createTable("CREATE TABLE my_cache_monographie ("
                                     "`my_cache_monographie_pk`            BIGINT  NOT NULL AUTO_INCREMENT,"
                                     "`my_cache_monographie_owner`         VARCHAR(40)  ,"
                                     "`my_cache_monographie_lang`          VARCHAR(4)   ,"
                                     "`my_cache_monographie_mono_No`       VARCHAR(8)   ,"
                                     "`my_cache_monographie_cip`           VARCHAR(20)  ,"
                                     "`my_cache_monographie_mono`          LONGBLOB     ,"
                                     "PRIMARY KEY (`my_cache_monographie_pk`)"
                                     ")"
                                     )==0) return 0;
    parseHtmlGestionDesRisques(m_pathAppli + "Ressources/PlanDeGestionDesRisques.html");
    return 1;
}

//---------------------------- setLogWidget ---------------------------------------
void          C_BDM_TheriaquePlugin::setLogWidget(QTextEdit *logWidget)
{C_Log::setLogWidget(logWidget);
}

//---------------------------- setProgressWidget ---------------------------------------
void          C_BDM_TheriaquePlugin::setProgressWidget(QProgressBar *pQProgressBar)
{setProgressWidget(pQProgressBar);
}

//---------------------------- dataSourceVersion ---------------------------------------
QString       C_BDM_TheriaquePlugin::dataSourceVersion()
{   C_GET_THE_INFOS_CUR C_GET_THE_INFOS(database(), this);
    if (C_GET_THE_INFOS.next ( "" )
       )
       { m_dataSourceVersion = tr("Theriaque Version : %1  date d'extraction : %2").arg( C_GET_THE_INFOS.VERS(), C_GET_THE_INFOS.DATE_EXT().toString("dd-MM-yyyy"));
       }
    return  m_dataSourceVersion;

}

//---------------------------- lang ---------------------------------------
QString       C_BDM_TheriaquePlugin::lang()              {return "fr"; }
//---------------------------- owner ---------------------------------------
QString       C_BDM_TheriaquePlugin::owner()             {return "TH"; }
//---------------------------- nextATC_Code ---------------------------------------
int           C_BDM_TheriaquePlugin::nextATC_Code(QString &codeAtc, QString &libelle)
{
    if (codeAtc.length()==0 && libelle.length()==0)    // premiere fois, apres il devrait contenir le retour SQL
       {
        m_position = 0;
        if (!database().isOpen() && database().open()== false)
           {outMessage( tr("ERREUR : C_BDM_TheriaquePlugin::nextATC_Code() database can not be open"), __FILE__, __LINE__);  return 0;
           }
        QString     requete  =" SELECT CATC_CODE_PK, CATC_NOMF FROM catc_classeatc ORDER BY CATC_CODE_PK";
        m_pQSqlQuery->exec(requete);
        if ( ! m_pQSqlQuery->isActive() )
            { outSQL_error(*m_pQSqlQuery, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::nextATC_Code()</b></font>"), requete, __FILE__, __LINE__  );
              return -1;
            }
       }
    if (m_pQSqlQuery->next())
       {  ++m_position;
          codeAtc     = m_pQSqlQuery->value(0).toString().trimmed();
          libelle     = m_pQSqlQuery->value(1).toString().trimmed();
          return m_position;
       }
    m_position = 0;
    return 0;
}

//-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
/*! \brief test compatibility with maximum and minimun authorized dose from a drug and patient context.
 *  \param C_PatientCtx *pC_PatientCtx
 *  \param const QString &grammar prescription for this drug
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>  C_BDM_TheriaquePlugin::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)
{
     QList<C_InterResultPair>  interResultPairList;
     if (pC_PatientCtx == 0) return interResultPairList;
     double      poids                 = 0;
     QString     s_poids               = "";
     double      surface               = 0;
     int         ageInDays             = 0;
     QString     s_age                 = "";
     char        sexe                  = 'I';
     QString     s_sexe                = "";

     double q_min                      = DBL_MAX;
     double q_max                      = -1;
     int nextPos                       =  0;
     QString    id                     = CGestIni::getXmlData( "gph_id",      grammar, &nextPos);
     QString    id_type                = CGestIni::getXmlData( "gph_it",      grammar, &nextPos);
     if ( id_type   != "CIP" ) return interResultPairList;
     QString    datePrise              = CGestIni::getXmlData( "gph_dt",      grammar, &nextPos).left(10); //        date et heure du debut de prise de la prescription format dd-MM-yyyy hh:mm:ss (la date de fin est calculee avec <gph_cy>)

     QString drugName                  = CGestIni::getXmlData( "gph_na",      grammar, &nextPos);
     // QString up_forme                  = CGestIni::getXmlData( "gph_uf",      grammar, &nextPos);
     // QString facteurCorpo              = CGestIni::getXmlData( "gph_pfc",     grammar, &nextPos);  // 10
     // QString unite_facteurCorpo        = CGestIni::getXmlData( "gph_pfcunit", grammar, &nextPos);  // kg m2
     QStringList seqList               = CGestIni::getXmlDataList( "gpp",     grammar, &nextPos);


     C_PosologieGrammar::toMinMax_UP(seqList,  q_min,  q_max);   // normalement si un factuer corporel existe les valeurs q_min et q_max sont deja multiplees par lui
     QString s_q_min                   = QString::number(q_min,'f',2);
     QString s_q_max                   = QString::number(q_max,'f',2);
     if (pC_PatientCtx)
        {s_poids    = pC_PatientCtx->poids();
         poids      = s_poids.toDouble();
         surface    = pC_PatientCtx->surfaceToDouble();
         ageInDays  = pC_PatientCtx->get_dateOfBirth().daysTo ( QDate::fromString(datePrise, "dd-MM-yyyy") );    // age du patient en jours au moment de la redaction de l'ordo
         s_sexe     = pC_PatientCtx->get_sexe();
         sexe       = s_sexe.toLatin1()[0];
         s_age      = pC_PatientCtx->get_age();
        }

     //........ recupererra les donnees de la base .....................
    double  drug_min                   = DBL_MAX;
    double  drug_max                   = -1;
    QString drug_qu_unit               = "";
    QString drug_fact_corp             = "";
    double  drug_fact_corp_qu          = 0;
    QString drug_DoseMaxDuree          = "";

    int isResponse  = 0;
    double   tMin   = DBL_MAX;
    double   tMax   = 0.0;

    C_GET_PRISE_UNIT_FROM_CODE_ID_CUR  C_GET_PRISE_UNIT_FROM_CODE_ID ( database(), this );
    while ( C_GET_PRISE_UNIT_FROM_CODE_ID.next( CIP_to_ID(id) ) )
          { ++isResponse;

            tMin              = C_GET_PRISE_UNIT_FROM_CODE_ID.MINI().toDouble();
            tMax              = C_GET_PRISE_UNIT_FROM_CODE_ID.MAXI().toDouble();

            drug_qu_unit      = C_GET_PRISE_UNIT_FROM_CODE_ID.FORME_UNIT();
            drug_fact_corp    = C_GET_PRISE_UNIT_FROM_CODE_ID.FACTEUR_CORPOREL(&drug_fact_corp_qu);
            drug_DoseMaxDuree = C_GET_PRISE_UNIT_FROM_CODE_ID.SPEC_ADMIN();
            if (drug_fact_corp_qu)
               {
                 if      (drug_fact_corp=="KG") {tMin = tMin * drug_fact_corp_qu * poids;   tMax = tMax * drug_fact_corp_qu * poids;}
                 else if (drug_fact_corp=="M2") {tMin = tMin * drug_fact_corp_qu * surface; tMax = tMax * drug_fact_corp_qu * surface;}

               }
            drug_min              = qMin(tMin, drug_min);
            drug_max              = qMax(tMax, drug_max);
          }
    if (isResponse)
       { if (q_min<drug_min)
            { interResultPairList.append( C_InterResultPair(  s_q_min  , "DOSE", tr(" %1 %2").arg(s_q_min, drug_qu_unit) ,
                                                              id       , "CIP", drugName,
                                                              tr("Dose minimum %1 trop faible <br/>").arg(s_q_min+" "+drug_qu_unit),"IT_DOSE", "2"
                                                           )
                                        );

            }
        if (q_max>drug_max)
           { interResultPairList.append( C_InterResultPair(  s_q_max  , "DOSE", tr(" %1 %2").arg(s_q_max, drug_qu_unit) ,
                                                             id       , "CIP", drugName,
                                                             tr("Dose maximum %1 trop elev\303\251e <br/>").arg(s_q_max+" "+drug_qu_unit),"IT_DOSE", "4"
                                                          )
                                       );

           }
       }
    else
       { interResultPairList.append( C_InterResultPair(  s_q_max  , "DOSE", tr("Contr\303\264le des doses") ,
                                                         id       , "CIP", drugName,
                                                         tr("Pas de donn\303\251es pour le contr\303\264le des doses <br/>"),"IT_DOSE", "1"
                                                      )
                                   );

       }
    return interResultPairList;
}

//-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
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
 - Le nouveau-n\303\251, de la naissance \303\240 21 jours
 - Le nourrisson, de 21 jours \303\240 2 ans.
*/
 QList<C_InterResultPair>        C_BDM_TheriaquePlugin::getInteractionAgeSexePoidsSportVigil(const QString &cip,
                                                                                             const QString &_age ,
                                                                                             const QString &sexe,
                                                                                             const QString &poids,
                                                                                             const QString &taille,
                                                                                             const QString &amenorrhee,
                                                                                             const QString &clairance,
                                                                                             int   allaitement,
                                                                                             bool  sport       /* = true */,
                                                                                             bool  vigil       /* = true */,
                                                                                             QString drugName  /* = "" */)
 {
     Q_UNUSED(sexe);
     Q_UNUSED(taille);
     Q_UNUSED(amenorrhee);
     Q_UNUSED(clairance);
     Q_UNUSED(allaitement);
     QList<C_InterResultPair>        interResultPairList;
     QString s_codeId    = QString::number(CIP_to_ID(cip));
     QString niveau      = "";
     QString inquire     = "";
     int        age      = _age.toInt();
     C_GET_CIPEMG_SPE_CUR          C_GET_CIPEMG_SPE         ( database(), this );     // retirer les CI
     C_GET_THE_DET_CIPEMG_CUR      C_GET_THE_DET_CIPEMG     ( database(), this );     // extraire les info
     //........... Extraction des CI niveau 'type'  pour la specialite 's_codeId' ............
     if      (age<1)                    inquire  = "'OC4'";
     else if (age>=1  && age <= 24)     inquire  = "'OC3'";
     else if (age>24  && age <= 183)    inquire  = "'OC1'";
     else if (age>183 && age <= 780)  { inquire  = "'OH'"; if (poids.toInt()<40) inquire += ",'JO3'";}
     else if (age>780 )               { inquire  = "'OG'"; if (poids.toInt()<40) inquire += ",'JO3'";}
     if (sport)                         inquire += ",'XB'";
     while ( C_GET_CIPEMG_SPE.next( s_codeId,                                                               // code produit theriaque
                                    "'C','P'",                                                              // 'C' pour contre-indications et deconseilles, 'P' pour precautions et mises en garde
                                    inquire,                                                                // limits just a this terrain's codes
                                    ""                                                                      // exclude this terrain's codes
                                  ) )      // "'C', 'P','I','N'"
           { // CODE_TERRAIN()
             // XB  Sportif/dopage
             // OH  Adulte        15 ans et trois mois et plus
             // OC1 Enfant        2 ans -- 15 ans et trois mois
             // OC3 Nourrisson    1 mois -- 2 ans
             // OC4 nouveau ne    < 1 mois
             // OG  sujet age     > 65 (OMS)
             // AB5 hypersensibilite
             // PA  Grossesse
             // PC  allaitement
             // QC0 interaction d'origine  alimentaire
             // XG  examens de laboratoire
             // QB5 autres traitements en cours
             // PL  quelque soit le terrain
             // JO3 patient de faible poids
             QString str    = "";
             QString tmp    = "";
             QChar        c = C_GET_CIPEMG_SPE.CODE_TERRAIN().at(0);
             int isResult;
             while ( (isResult=C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 1  )) )
                  { tmp += "<br/>" + C_GET_THE_DET_CIPEMG.TEXTDET();   // 1 titres secondaires de la CI
                  }

             tmp = tmp.prepend(C_GET_CIPEMG_SPE.LIBELLE());   // prefixer par le titre principal

             while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 2  ) )
                   { niveau  = C_GET_THE_DET_CIPEMG.TEXTDET();          // 2 niveau de la CI
                   }

             while ( (isResult=C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 3  )) )
                   { if (isResult>1) str += "<br/>";
                     str += C_GET_THE_DET_CIPEMG.TEXTDET();           // 3 lignes accessoires :  Cf. GROSSESSE ET ALLAITEMENT Cf. GROSSESSE ET ALLAITEMENT
                     if (c=='X' && C_GET_THE_DET_CIPEMG.TEXTDET().startsWith(tr("SUBSTANCE INTERDITE")))
                        {niveau = "CONTRE-INDICATIONS";
                        }
                   }

             while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 4  ) )
                   { str +=  "<br/>" + C_GET_THE_DET_CIPEMG.TEXTDET(); // 4 grande information de texte
                   }


             if (tmp.length())
                {
                 if (c=='O')
                    { interResultPairList.append( C_InterResultPair( _age    , "AGE", C_GET_CIPEMG_SPE.LIBELLE(),
                                                                     cip     , "CIP", drugName,
                                                                     tr("Age du patient : <br/>")+C_GET_CIPEMG_SPE.LIBELLE()+"<br/>"+str,"IT_AGE", ciTextLevelToLevel(niveau)
                                                                   )
                                                );
                    }
                 else if (c=='X')
                    { interResultPairList.append( C_InterResultPair( _age    , "DOPE", C_GET_CIPEMG_SPE.LIBELLE(),
                                                                     cip     , "CIP", drugName,
                                                                     tr("Sport : <br/>")+C_GET_CIPEMG_SPE.LIBELLE()+"<br/>"+str,"IT_DOPE", ciTextLevelToLevel(niveau)
                                                                   )
                                                );
                    }
                 else if (c=='J')
                    { interResultPairList.append( C_InterResultPair( _age    , "POIDS", C_GET_CIPEMG_SPE.LIBELLE(),
                                                                     cip     , "CIP", drugName,
                                                                     tr("Faible poids : <br/>")+C_GET_CIPEMG_SPE.LIBELLE()+"<br/>"+str,"IT_POIDS", ciTextLevelToLevel(niveau)
                                                                   )
                                                );
                    }
                }
           }
     if (vigil) interResultPairList += getInteractionVigilanceSpe(cip, s_codeId,  drugName  );
     return interResultPairList;
 }
 //-------------------------------- getInteractionVigilance ------------------------------------------------------
 QList<C_InterResultPair>        C_BDM_TheriaquePlugin::getInteractionVigilance(const QString &cip,  QString drugName  /* = "" */)
 {  return getInteractionVigilanceSpe(cip, QString::number(CIP_to_ID(cip)), drugName );
 }
 //-------------------------------- getInteractionVigilanceSpe ------------------------------------------------------
 QList<C_InterResultPair>        C_BDM_TheriaquePlugin::getInteractionVigilanceSpe(const QString &cip, const QString &s_codeId,  QString drugName  /* = "" */)
 {   QList<C_InterResultPair>        interResultPairList;
     QString txt    = "";
     QString niveau = "";
     C_GET_THE_FCO_ID_BY_SPE_CUR  C_GET_THE_FCO_ID_BY_SPE ( database(), this );
     C_GET_THE_DET_FCO_CUR        C_GET_THE_DET_FCO       ( database(), this );
     while ( C_GET_THE_FCO_ID_BY_SPE.next( s_codeId  ) )
           { while ( C_GET_THE_DET_FCO.next( C_GET_THE_FCO_ID_BY_SPE.IDFCO()  ) )
                   { if (C_GET_THE_DET_FCO.GRP()=="01" )
                        { if  ( C_GET_THE_DET_FCO.INFO_01().toUpper() != "OUI") continue;
                        }
                     else
                        { txt +=  C_GET_THE_DET_FCO.INFO_01() + "<br/>";
                        }
                     if (C_GET_THE_DET_FCO.GRP()=="02" )        // niveau de gravit\303\251
                        {if      (C_GET_THE_DET_FCO.INFO_01().startsWith("NIV 1")) niveau = "2";
                         else if (C_GET_THE_DET_FCO.INFO_01().startsWith("NIV 2")) niveau = "3";
                         else if (C_GET_THE_DET_FCO.INFO_01().startsWith("NIV 3")) niveau = "4";
                         else                                                      niveau = "1";
                        }
                   }
             txt +=  C_GET_THE_FCO_ID_BY_SPE.FCO_TXT() + "<br/>";
             txt +=  C_GET_THE_FCO_ID_BY_SPE.INFO_01() + tr(" en date du : ") + C_GET_THE_FCO_ID_BY_SPE.DAT_CRE().toString("dd-MM-yyyy") + "<br/>";
             if (niveau.length()==0) continue;
             interResultPairList.append( C_InterResultPair( ""      , "VIGIL",  tr("Effets sur l'aptitude \303\240 conduire des v\303\251hicules et \303\240 utiliser des machines"),
                                                            cip     , "CIP",    drugName,
                                                            tr("Effets sur l'aptitude \303\240 conduire des v\303\251hicules et \303\240 utiliser des machines : <br/>")+txt,"IT_VIG",
                                                            niveau     //, ciTextLevelToLevel(niveau)
                                                          )
                                       );
           }
     return interResultPairList;
 }
//-------------------------------- getInteractionCim10 ------------------------------------------------------
/*! \brief  return physiopath interactions from patient and drug.
 *  \param  const QString& cip drug cip identification .
 *  \param  const QStringList &cim10List cim10 pathologies list .
 *  \param  QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
 //  case 'C': return tr("CONTRE-INDICATIONS");
 //  case 'P': return tr("PRECAUTIONS D'EMPLOIS");
 //  case 'I': return tr("NON INDICATIONS");
 //  case 'N': return tr("NON CONTRE-INDICATIONS");
 //  default : return tr("CHAIS PAS C'EST QUOI");
QList<C_InterResultPair> C_BDM_TheriaquePlugin::getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName   )
{   QList<C_InterResultPair> interResultPairList;
    QString s_codeId    = QString::number(CIP_to_ID(cip));
    QMap         <QString, C_InterResultPair> matResult;
    matResult = get_Cim10_InterResultPair_Map(s_codeId, "'C'", cip, cim10List , drugName);
    QMapIterator <QString, C_InterResultPair> it1(matResult);
    while (it1.hasNext())
          {
            it1.next();
            interResultPairList.append( it1.value() );
          }
    matResult = get_Cim10_InterResultPair_Map(s_codeId, "'P'", cip, cim10List , drugName);
    QMapIterator <QString, C_InterResultPair> it2(matResult);
    while (it2.hasNext())
          { it2.next();
            interResultPairList.append( it2.value() );
          }
    return interResultPairList;
}
//-------------------------------- get_Cim10_InterResultPair_Map ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QStringList &cim10List cim10 pathologies list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
 //  case 'C': return tr("CONTRE-INDICATIONS");
 //  case 'P': return tr("PRECAUTIONS D'EMPLOIS");
 //  case 'I': return tr("NON INDICATIONS");
 //  case 'N': return tr("NON CONTRE-INDICATIONS");
 //  default : return tr("CHAIS PAS C'EST QUOI");
QMap<QString, C_InterResultPair> C_BDM_TheriaquePlugin::get_Cim10_InterResultPair_Map(const QString &s_codeId, const QString &level,const QString &cip, const QStringList &cim10List , QString drugName)
{   QMap<QString,C_InterResultPair> cimMap;
    QString niveau       = "";
    QString  cim10       = "";
    QString  typInter    = "IT_CIM";
    C_GET_CIPEMG_SPE_CUR          C_GET_CIPEMG_SPE         ( database(), this );     // retirer les CI
    C_GET_THE_DET_CIPEMG_CUR      C_GET_THE_DET_CIPEMG     ( database(), this );     // extraire les info
    //........... Extraction des CI niveau 'type'  pour la specialite 's_codeId' ............

    while ( C_GET_CIPEMG_SPE.next( s_codeId,                                                               // code produit theriaque
                                   level,                                                                  // 'C' pour contre-indications et deconseilles, 'P' pour precautions et mises en garde
                                   "",                                                                     // limits just a this terrain's codes
                                   "'OC1','OC3','OC3','OC4','OG','OH','AB5','QC0','XB','XG','QB5','PL'"    // exclude this terrain's codes
                                 ) )      // "'C', 'P','I','N'"
          { // CODE_TERRAIN()
            // XB  Sportif/dopage
            // OH  Adulte
            // OC1 Enfant
            // OC3 Nourrisson
            // OC4 nouveau ne
            // OG  sujet age
            // AB5 hypersensibilite
            // PA  Grossesse
            // PC  allaitement
            // QC0 interaction d'origine  alimentaire
            // XG  examens de laboratoire
            // QB5 autres traitements en cours
            // PL quelque soit le terrain
            // JO3 patient de faible poids
            QString str    = "";
            QString tmp    = "";
            int isResult;
            if      (C_GET_CIPEMG_SPE.CODE_TERRAIN()=="PC") typInter    = "IT_ALL";
            else if (C_GET_CIPEMG_SPE.CODE_TERRAIN()=="PA") typInter    = "IT_GRO";
            else                                            typInter    = "IT_CIM";
            while ( (isResult=C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 1  )) )
                 { tmp += "<br/>" + C_GET_THE_DET_CIPEMG.TEXTDET();   // 1 titres secondaires de la CI
                 }

            tmp = tmp.prepend(C_GET_CIPEMG_SPE.LIBELLE());   // prefixer par le titre principal

            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 2  ) )
                  { niveau  = C_GET_THE_DET_CIPEMG.TEXTDET();          // 2 niveau de la CI
                  }

            while ( (isResult=C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 3  )) )
                  { if (isResult>1) str += "<br/>";
                    str += C_GET_THE_DET_CIPEMG.TEXTDET();           // 3 lignes accessoires :  Cf. GROSSESSE ET ALLAITEMENT Cf. GROSSESSE ET ALLAITEMENT
                  }

            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 4  ) )
                  { str +=  "<br/>" + C_GET_THE_DET_CIPEMG.TEXTDET(); // 4 grande information de texte
                  }

            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 5  ) )
                  { cim10 = C_GET_THE_DET_CIPEMG.INFO_01();
                    if (isThisCim10Code_in_Cim10List(cim10, cim10List))
                       {cimMap[cim10] =  C_InterResultPair( cim10    , "CIM", C_GET_THE_DET_CIPEMG.TEXTDET(),
                                                            cip      , "CIP", drugName,
                                                            tr("Etat physiopathologique du patient : <br/>")+tmp+"<br/>"+str,typInter, ciTextLevelToLevel(niveau)
                                                          );
                       }
                  }
            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 6  ) )
                  { cim10 = C_GET_THE_DET_CIPEMG.INFO_01();
                    if (isThisCim10Code_in_Cim10List(cim10, cim10List))
                       {cimMap[cim10] =  C_InterResultPair( cim10    , "CIM", C_GET_THE_DET_CIPEMG.TEXTDET(),
                                                            cip      , "CIP", drugName,
                                                            tr("Etat physiopathologique du patient : <br/>")+tmp+"<br/>"+str,typInter, ciTextLevelToLevel(niveau)
                                                          );
                       }
                  }
          }
    return cimMap;
}
//-------------------------------- ciTextLevelToLevel ------------------------------------------------------
QString C_BDM_TheriaquePlugin::ciTextLevelToLevel( QString level)
{level.truncate(4);
 if (level=="CONT") return "4";
 if (level=="UTIL") return "3";
 if (level=="MISE") return "2";
 if (level=="PREC") return "1";
 if (level=="RECO") return "0";
 return "32";
}

//--------------------------- isThisCim10Code_in_Cim10List -----------------------------------------------------------
bool C_BDM_TheriaquePlugin::isThisCim10Code_in_Cim10List(const QString &cim10, const QStringList &Cim10List)
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

//--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
/*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
 *  \param const QString cip is drug cip code.
 *  \param QString *rcp is pointer on QString to return RCP result if not zero .
 *  \return CIS code string
*/
QString C_BDM_TheriaquePlugin::get_CIS_and_RCP_From_CIP(const QString &/*cip*/, QString *rcp /* =0 */)
{ if (rcp) *rcp="";
  return QString::null;
}
//-------------------------------- isStupefiant ------------------------------------------------------
/*! \brief return if the product identified par Id is a stupefiant
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return 1 is stupefiant
 */
int C_BDM_TheriaquePlugin::isStupefiant ( const QString &cip )
{QString stup = "";
 C_GET_THE_SPECIALITE_CUR    C_GET_THE_SPECIALITE   ( database(), this );
 while ( C_GET_THE_SPECIALITE.next (  cip ) )
       {stup = C_GET_THE_SPECIALITE.SP_LISTE();
        if (stup.contains("STUPEFIANT")) return 1;
       }
 return 0;
}
//-------------------------------- code_ATC_To_Name ------------------------------------------------------
/*! \brief return if ATC libelle from code ATC
 *  \param const QString &code      code ATC .
 *  \return QString return libelle
 */
QString C_BDM_TheriaquePlugin::code_ATC_To_Name(const QString &code)
{   C_GET_ATC_LIBELLE_CUR  C_GET_ATC_LIBELLE ( database(), this );
    QString ret = "";
    while ( C_GET_ATC_LIBELLE.next( code ) )
          { ret = C_GET_ATC_LIBELLE.LIBELLE();
          }
    return ret;
}

//-------------------------------- getDefinedDailyDoseFromCIP ------------------------------------------------------
/*! \brief return the defined daily dose from cip.
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString defined daily dose
 */
QString   C_BDM_TheriaquePlugin::getDefinedDailyDoseFromCIP ( const QString &cip)
{   QString definedDailyDose = "";
    C_GET_THE_SPECIALITE_CUR    C_GET_THE_SPECIALITE   ( database(), this );
    int isResult       = 0;
    int codeId         = 0;
    while ( C_GET_THE_SPECIALITE.next (  cip ) )              // results cursor
          {codeId  = C_GET_THE_SPECIALITE.SP_CODE_SQ_PK();    // id theriaque
          }
    C_GET_THE_ATC_DDD_CUR  C_GET_THE_ATC_DDD ( database() , this );
    while ( (isResult=C_GET_THE_ATC_DDD.next( QString::number(codeId), 2)) )
         { definedDailyDose = C_GET_THE_ATC_DDD.DOSAGE()+"|"+ C_GET_THE_ATC_DDD.NOMUNITE();
         }
    return definedDailyDose;

}
//-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
/*! \brief return the drug route from cip.
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with the drug route
 */
QString   C_BDM_TheriaquePlugin::getDrugRouteFromCIP ( const QString &cip)
{   // C_GET_THE_SPECIALITE_CUR    C_GET_THE_SPECIALITE   ( database(), this );
    // int isResult       = 0;
    int codeId         = CIP_to_ID(cip);
   // while ( C_GET_THE_SPECIALITE.next (  cip ) )              // results cursor
   //       {codeId  = C_GET_THE_SPECIALITE.SP_CODE_SQ_PK();    // id theriaque
   //       }
    QString result;
    C_GET_THE_VOIE_SPE_CUR  C_GET_THE_VOIE_SPE ( database(), this );
    while ( C_GET_THE_VOIE_SPE.next( codeId  ) )
          { result += C_GET_THE_VOIE_SPE.TXTVOIE()+", ";
          }
    /*
    C_GET_THE_ATC_DDD_CUR  C_GET_THE_ATC_DDD ( database() , this );
    while ( (isResult=C_GET_THE_ATC_DDD.next( QString::number(codeId), 2)) )
         { return  C_GET_THE_ATC_DDD.NOMVOIE();
         }
    */
    result.chop(2);
    return result;
}
//-------------------------------- getPosologiesFromCIP ------------------------------------------------------
/*! \brief return posologies list as posologie grammar QString
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
 *  \return QStringList with grammar QString list
 */
QStringList   C_BDM_TheriaquePlugin::getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip )
{   Q_UNUSED(pC_PatientCtx);
    QStringList   retList;
    QStringList   unitPosoSegment;
    QString     s_codeId         = QString::number(CIP_to_ID(cip));
    int          addTerr         = 0;
    int          type            = 0;

    //QString codeTerrain;          // pour l'indication
    //QString codeTypTerrain;       // pour l'indication
    //QString terrain;              // pour l'indication
    //QString voie;                 // orale intraveineuse sous cutanee rectale
    QString unitFormPrise        = "";             // comprime sachet seringue preremplie etc...
    QString min_poso             = "";             // 100
    QString max_poso             = "";             // 200
    QString unit_poso            = "";             // UI mg ml
    QString par_facteurCorporel  = "";             // 40
    QString unit_facteurCorporel = "";             // kg (40 UI/kg)
    QString q_unitPosoByUnitForm = "";             // 100  (100 mg / comprime)
    QString min_fce              = "";             // 2
    QString max_fce              = "";             // 6
    QString unit_fce             = "";             // jour  (2 a 6 cp par jour)
    QString min_duree            = "";             // 10
    QString max_duree            = "";             // 30
    QString unit_duree           = "";             // jour  (pendant 10 a 30 jours)

    //QString subTerrain   = "";
    QString numFic               = "";
    QString voieAd               = "";
    QString terrain              = "";
    QString natIndic             = "";
    QString cpmIndic             = "";
    QString grp                  = "";
    QString min                  = "";
    QString max                  = "";
    QString result               = "";
    QString cplNat               = "";
    int     cur_grp              = -1;
    int     typ_grp              = -1;
    int     sub_grp              = -1;


    QMultiMap <QString,QString> numFic_Indic;

    C_GET_THE_POSO_CUR          C_GET_THE_POSO ( database(), this );
    C_GET_THE_DET_POSO_SPE_CUR  C_GET_THE_DET_POSO_SPE ( database(), this );
    //.................. relever toutes les indications primaires ................................
    while ( C_GET_THE_POSO.next(  s_codeId  ) )
          { type     = C_GET_THE_POSO.TYP();
            switch(type)
            { case 0: numFic    = C_GET_THE_POSO.MY_NOFIC();                                   break;
              case 1: voieAd    = C_GET_THE_POSO.MY_INFO_01();                                 break;
              case 2: terrain  += C_GET_THE_POSO.MY_INFO_01()+", ";                            break;
              case 3: {natIndic += " "+C_GET_THE_POSO.MY_INFO_01()+" ";
                      if (addTerr==0)
                         {terrain += "  (";
                          ++addTerr;
                         }
                      terrain  += C_GET_THE_POSO.MY_INFO_01().toLower() +", ";
                      }break;
              case 4: cpmIndic += " "+C_GET_THE_POSO.MY_INFO_01()+" ";    break;
              default :
              case 5: { if (terrain.length() && addTerr) terrain += ")";
                        if (natIndic.length())
                           {terrain += " " + natIndic + " " + cpmIndic;
                           }
                        numFic_Indic.insert(numFic,terrain);
                        terrain  = "";
                        numFic   = "";
                        natIndic = "";
                      }
            }
            /*
            if (type==5)
               {if (terrain.length() && addTerr) terrain += ")";
                if (natIndic.length())
                   {terrain += " " + natIndic + " " + cpmIndic;
                   }
                numFic_Indic.insert(numFic,terrain);
               }
            */
          } // endwhile ( C_GET_THE_POSO.next(  s_codeId  ) )

    //.................. pour chaque indications primaires relever les posologies................................
    QMapIterator<QString, QString> it(numFic_Indic);
    while  (it.hasNext())
           {    it.next();
                numFic  = it.key();
                terrain = it.value();
                while ( C_GET_THE_DET_POSO_SPE.next(numFic , 2  ) )
                      {

                        grp          = C_GET_THE_DET_POSO_SPE.GRP();
                        min          = C_GET_THE_DET_POSO_SPE.MINI();
                        max          = C_GET_THE_DET_POSO_SPE.MAXI();

                        //                                                       ____ grp[0]
                        //                                                      |     ___ typ_grp
                        //                                                      |    |  ___ sub_grp
                        //                                                      |    | |
                        //--- complement nature de dose                         1.N\302\272.0.IPOCOTE_NUM_ORD_PK
                        //--- minimum et maximum de la posologie                1.N\302\272.1.0
                        //--- frequence minimum et maximum                      1.N\302\272.2.0
                        //--- commentaire frequence                             1.N\302\272.2.1
                        //--- dur\303\251e minimum et dur\303\251e maximum                    1.N\302\272.2.2
                        //--- commentaire dur\303\251e                                 1.N\302\272.3.IPOCOD_NUM_ORD_PK
                        //--- Commentaire Adaptation                            2.0.1.FPOCOAD_NUM_ORD_PK
                        //--- Commentaire surveillance                          2.1.1.FPOCOSU_NUM_ORD_PK
                        //--- Commentaire recommandation                        2.2.1.FPOCOAU_NUM_ORD_PK
                        cur_grp = -1;      //  a priori le premier groupe part de zero
                        if (grp[0]=='1')
                           {
                            if (cur_grp != -1 && cur_grp != grp.toLatin1()[2])
                               {retList.append( posoToGrammar( cip,
                                                               "",                         // code terrain pour l'indication
                                                               "",                         // code typ terrain pour l'indication
                                                               terrain + " " + cplNat,     // terrain pour l'indication
                                                               voieAd,                     // orale intraveineuse sous cutanee rectale
                                                               unitFormPrise,              // comprime sachet seringue preremplie etc...
                                                               min_poso,                   // 100
                                                               max_poso,                   // 200
                                                               unit_poso,                  // UI mg ml
                                                               par_facteurCorporel,        // 40
                                                               unit_facteurCorporel,       // kg (40 UI/kg)
                                                               q_unitPosoByUnitForm,       // 100  (100 mg / comprime)
                                                               min_fce,                    // 2
                                                               max_fce,                    // 6
                                                               unit_fce,                   // jour  (2 a 6 cp par jour)
                                                               min_duree,                  // 10
                                                               max_duree,                  // 30
                                                               unit_duree                  // jour  (pendant 10 a 30 jours)
                                                             )
                                                 );
                                unitFormPrise        = "";             // comprime sachet seringue preremplie etc...
                                min_poso             = "";             // 100
                                max_poso             = "";             // 200
                                unit_poso            = "";             // UI mg ml
                                par_facteurCorporel  = "";             // 40
                                unit_facteurCorporel = "";             // kg (40 UI/kg)
                                q_unitPosoByUnitForm = "";             // 100  (100 mg / comprime)
                                min_fce              = "";             // 2
                                max_fce              = "";             // 6
                                unit_fce             = "";             // jour  (2 a 6 cp par jour)
                                min_duree            = "";             // 10
                                max_duree            = "";             // 30
                                unit_duree           = "";             // semaines
                                cplNat               = "";

                               }
                             cur_grp = grp.toLatin1()[2];
                             typ_grp = grp.toLatin1()[4];
                             sub_grp = grp.toLatin1()[6];
                             if (typ_grp=='0')          // --- complement posologie (nature de dose)
                                {cplNat  = C_GET_THE_DET_POSO_SPE.INFO_01();
                                }
                             if (typ_grp=='1')          // --- minimum et maximum de la posologie
                                {min_poso     = min;
                                 max_poso     = max;
                                 cplNat       = C_GET_THE_DET_POSO_SPE.NATURE()+" "+cplNat;
                                 unit_poso    = C_GET_THE_DET_POSO_SPE.INFO_02();
                                 unitPosoSegment = unit_poso.split('/');     // UI anti-Xa/ADMINISTRATION   // UI anti-Xa/KG/ADMINISTRATION
                                 if (unitPosoSegment.size()>=1)
                                    {unit_poso                = unitPosoSegment[0];
                                    }
                                 if (unitPosoSegment.size()>=2 )
                                    {unit_facteurCorporel     = unitPosoSegment[1];
                                     if (unit_facteurCorporel.contains("KG"))
                                        {par_facteurCorporel  = toNum(unit_facteurCorporel, "$stopIfNotNum");
                                         unit_facteurCorporel = "KG";
                                        }
                                     unit_facteurCorporel = unitPosoSegment[0];
                                    }
                                }
                             if (typ_grp=='2')          // --- frequence
                                {
                                 if (sub_grp=='0')      // --- frequence minimum et maximum
                                    { min_fce  = min;              // 2
                                      max_fce  = max;              // 6
                                      unit_fce = C_GET_THE_DET_POSO_SPE.INFO_02();             // jour  (2 a 6 cp par jour)
                                    }
                                 if (sub_grp=='2')      // --- dur\303\251e minimum et dur\303\251e maximum        Dur\303\251e de traitement max
                                    { min_duree  = min;            // 10
                                      max_duree  = max;            // 30
                                      unit_duree = C_GET_THE_DET_POSO_SPE.INFO_02();    // jour  (pendant 10 a 30 jours)
                                    }
                                }
                           }
                      } // end while ( C_GET_THE_DET_POSO_SPE.next(numFic , 2  ) )
                /*
                retList.append( posoToGrammar( cip,
                                               "",                         // code terrain pour l'indication
                                               "",                         // code typ terrain pour l'indication
                                               terrain + " " + cplNat,     // terrain pour l'indication
                                               voieAd,                     // orale intraveineuse sous cutanee rectale
                                               "? unit form",                // comprime sachet seringue preremplie etc...
                                               min_poso,                   // 100
                                               max_poso,                   // 200
                                               unit_poso,                  // UI mg ml
                                               par_facteurCorporel,        // 40
                                               unit_facteurCorporel,       // kg (40 UI/kg)
                                               q_unitPosoByUnitForm,       // 100  (100 mg / comprime)
                                               min_fce,                    // 2
                                               max_fce,                    // 6
                                               unit_fce,                   // jour  (2 a 6 cp par jour)
                                               min_duree,                  // 10
                                               max_duree,                  // 30
                                               unit_duree                  // jour  (pendant 10 a 30 jours)
                                             )
                                );
                unitFormPrise        = "";             // comprime sachet seringue preremplie etc...
                min_poso             = "";             // 100
                max_poso             = "";             // 200
                unit_poso            = "";             // UI mg ml
                par_facteurCorporel  = "";             // 40
                unit_facteurCorporel = "";             // kg (40 UI/kg)
                q_unitPosoByUnitForm = "";             // 100  (100 mg / comprime)
                min_fce              = "";             // 2
                max_fce              = "";             // 6
                unit_fce             = "";             // jour  (2 a 6 cp par jour)
                min_duree            = "";             // 10
                max_duree            = "";             // 30
                unit_duree           = "";             // semaines
                */
                result   = "";
                natIndic = "";
                cpmIndic = "";
                cplNat   = "";
                addTerr  = 0;
          }// end  while ( C_GET_THE_POSO.next(  s_codeId  ) )
 return retList;
}

//------------------------------------------ toNum -----------------------------------------
/*! \brief ne retient d'une chaine de caracteres que les valeurs numeriques et le point (au passage les virgules seront transformees en point).
 *  \param text           const QString & texte a convertir en numerique
 *  \param cutFirstStr_in const QString & mode de conversion numerique :
 SI   contient $keepSign     la conversion tient compte du signe.
 SI   contient $stopIfNotNum la conversion s'arrete au premier caractere non numerique.
 SI   contient $allString tous les caracteres numeriques de la chaine seront retenus.
 SI   contient $toInt la conversion ne conservera que la partie entiere.
 SI   toute autre valeur, la conversion s'arretera a la premiere occurence de cette valeur.
 *  \param int *pos si diff\303\251rent de zero alors la position dans la chaine apr\303\250s la valeur numerique
 *  \return une QString resultat du traitement
 */
QString C_BDM_TheriaquePlugin::toNum(const QString &text, const QString &cutFirstStr_in /* = ""*/, int *next_pos /* = 0*/)
{if (next_pos)  *next_pos  = -1;
 QString str               = text.trimmed();
 QString cutFirstStr       = cutFirstStr_in;
 bool    cutFirstNonNum    = true;
 bool    keepSign          = (cutFirstStr.indexOf ("$keepSign") != -1);
 if (keepSign) cutFirstStr =  cutFirstStr.remove  ("$keepSign");
 bool    toInt             = (cutFirstStr.indexOf ("$toInt")    != -1);
 if (toInt) cutFirstStr    =  cutFirstStr.remove  ("$toInt");

 if (cutFirstStr.length())
    {
     if      (cutFirstStr.indexOf("$stopIfNotNum") != -1)    cutFirstNonNum = true;
     else if (cutFirstStr.indexOf("$allString")    != -1)    cutFirstNonNum = false;
     else {int pos = str.indexOf(cutFirstStr);
           if (pos != -1) str.truncate(pos);
          }
    }
 int     end = str.length();
 QString ret = "";
 int       i = 0;
 while (i<end)
     {if (str.at(i)>='0' && str[i]<='9')                      {ret += str.at(i);}
      else if (str.at(i)=='-' || str[i]=='+')                 { if (keepSign) ret += str.at(i);}
      else if (str.at(i)=='.')                                {ret += ".";   }
      else if (str.at(i)==',')                                {ret += ".";   }
      else if (cutFirstNonNum)                                {if (next_pos) *next_pos = i;   i = end;   }
      ++i;
     }
 if (next_pos && *next_pos==-1) *next_pos = i;
 if (toInt)
    {int pos  =     ret.indexOf(".");
     if (pos != -1) ret.truncate(pos);
    }
 return ret;
}

//------------------------------------------ posoToGrammar -------------------------------------------------------------------
// l'enfer existe sur terre
QString C_BDM_TheriaquePlugin::posoToGrammar(const QString &cip,                  // code cip
                                             const QString &codeTerrain,          // pour l'indication
                                             const QString &codeTypTerrain,       // pour l'indication
                                             const QString &terrain,              // pour l'indication
                                             const QString &voie,                 // orale intraveineuse sous cutanee rectale
                                             const QString &unitFormPrise,        // comprime sachet seringue preremplie etc...
                                             const QString &min_poso,             // 100
                                             const QString &max_poso,             // 200
                                             const QString &unit_poso,            // UI mg ml
                                             const QString &par_facteurCorporel,  // 40
                                             const QString &unit_facteurCorporel, // kg (40 UI/kg)
                                             const QString &q_unitPosoByUnitForm, // 100  (100 mg / comprime)
                                             const QString &min_fce,              // 2
                                             const QString &max_fce,              // 6
                                             const QString &unit_fce,             // jour  (2 a 6 cp par jour)
                                             const QString &min_duree,            // 10
                                             const QString &max_duree,            // 30
                                             const QString &unit_duree            // jour  (pendant 10 a 30 jours)
                                            )
{
 QString empty  = "";
 QString gramar = "";
 QString  ret   = "";
           ret += "<gph>\n"
                  " <gph_id>"      + cip                  + "</gph_id>\n"          // cip
                  " <gph_it>"      + "CIP"                + "</gph_it>\n"          // id_type
                  " <gph_pk>"      + empty                + "</gph_pk>\n"          // pk du favori
                  " <gph_uf>"      + unitFormPrise        + "</gph_uf>\n"          // forme de l'unite de prise
                  " <gph_voie>"    + voie                 + "</gph_voie>\n"        // voie d'administration
                  " <gph_fmin>"    + min_fce              + "</gph_fmin>\n"        // frequence minimum
                  " <gph_fmax>"    + max_fce              + "</gph_fmax>\n"        // frequence maximum
                  " <gph_funit>"   + unit_fce             + "</gph_funit>\n"       // frequence unite
                  " <gph_pmint>"   + min_poso             + "</gph_pmin>\n"        // posologie minimum
                  " <gph_pmax>"    + max_poso             + "</gph_pmax>\n"        // posologie maximum
                  " <gph_punit>"   + unit_poso            + "</gph_punit>\n"       // posologie unite
                  " <gph_pfc>"     + par_facteurCorporel  + "</gph_pfc>\n"         // posologie facteur corporel qu de gph_punit par facteur corporel
                  " <gph_pfcunit>" + unit_facteurCorporel + "</gph_pfcunit>\n"     // posologie facteur corporel unite Kg cm
                  " <gph_pqbyuf>"  + q_unitPosoByUnitForm + "</gph_pqbyuf>\n"      // quantite d'unite posologique par unite de forme de gph_punit par gph_uf
                  " <gph_dmin>"    + min_duree            + "</gph_dmin>\n"        // duree minimum
                  " <gph_dmax>"    + max_duree            + "</gph_dmax>\n"        // duree maximum
                  " <gph_dunit>"   + unit_duree           + "</gph_dunit>\n"       // duree unite
                  " <gph_cm></gph_cm>\n"             // conversion mode  UP_QUANTITE MOMENT_MEAL ALL_LITERAL
                  " <gph_in>\n"                      // liste des indications
                           "<gph_il>"+codeTerrain+"|"+codeTypTerrain+"|"+terrain+"</gph_il>"
                 " </gph_in>"                   // fin de la liste des indications <gph_il> </gph_il>
                 " <gph_co></gph_co>\n"
                 "</gph>\n"
                 "<gps>\n"
                 "</gps>\n";
           //     <gpp>[1;m|2;d|4;s]J8</gpp>\n
           //     <gpp>[0.5;m|1;d15r|2;sr30|3;h10-13]J4\</gpp>\n
           //     <gpp>[0.5;m|1;s]J3</gpp>\n

           if (min_poso.length() && max_poso.length()) gramar += "<gpp>[" + min_poso + "-" + max_poso+";{{MOMENT}}]" ; //result += " de " + min + tr(" \303\240 ") + max + " " + C_GET_THE_DET_POSO_SPE.INFO_02();
           else if (min_poso.length())                 gramar += "<gpp>[" + min_poso + ";{{MOMENT}}]";                 //C_GET_THE_DET_POSO_SPE.INFO_02();
           else if (max_poso.length())                 gramar += "<gpp>[" + max_poso + ";{{MOMENT}}]";                 // result += max + " " + C_GET_THE_DET_POSO_SPE.INFO_02();
           //if (C_GET_THE_DET_POSO_SPE.INFO_01().length()) result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();

           return ret;
}

//-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
/*! \brief return interactions xml results from a cip list.
 *  \param const QStringLits &cipList      drugs CIP codes list to test.
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
QList<C_InterResultPair> C_BDM_TheriaquePlugin::getInteractionPairFromCipList ( const QStringList &cipList)
{   QList<C_InterResultPair> retList;
    QString     listId     = "";
    QString     cip        = "";
    QString     text       = "";
    int         id         = 0;
    //QString     toDump     = "";
    QMap <int, QString> map_id_cip;       // permettra de retrouver le cip a partir de l'id theriaque

    if (cipList.count()<=1) return  retList;
    //............. creer la liste des id theriaque .....................
    for (int i=0; i<cipList.count(); ++i)
        {cip     = cipList.at(i);
         id      = CIP_to_ID(cip);
         listId += QString::number(id) + "','";      // chaine d'appel de la requete  1236','4569','7895','
         map_id_cip[id] = cip;                       // on mape l'id theriaque avec le cip
        }
    listId = listId.left(listId.length()-3);         // virer le dernier  ','
    //............ requeter et extraire les donnees ........................
    C_GET_THE_GTIAM_TXT_CUR  C_GET_THE_GTIAM_TXT ( database(), this );
    C_GET_THE_GTIAM_ID_CUR   C_GET_THE_GTIAM_ID  ( database(), this );
    while ( C_GET_THE_GTIAM_ID.next( listId , "4"  ) )
    {   text = "";
        while ( C_GET_THE_GTIAM_TXT.next(  C_GET_THE_GTIAM_ID.MY_IDINTER()  ) )
              { text += C_GET_THE_GTIAM_TXT.MY_TEXTE()+"\n";
              }
        retList.append( C_InterResultPair(map_id_cip[C_GET_THE_GTIAM_ID.ID_T1()],  "CIP",     C_GET_THE_GTIAM_ID.TERME_1(),
                                          map_id_cip[C_GET_THE_GTIAM_ID.ID_T2()],  "CIP",     C_GET_THE_GTIAM_ID.TERME_2(),
                                          text.replace("<","&lt;").replace(">","&gt;").replace("\n","<br/>").remove('\r'), "AFSSAPS", gitiamLevelToBdmLevel(C_GET_THE_GTIAM_ID.NIVEAU())));
    }
    return retList;
}
//-------------------------------- gitiamLevelToBdmLevel ------------------------------------------------------
QString   C_BDM_TheriaquePlugin::gitiamLevelToBdmLevel ( const QString &niveau)
{switch (niveau.toInt())
    { case 1:  return  "4";         // m_ci_assinterdit
      case 2:  return  "3";         // m_ci_precautions
      case 3:  return  "2";         // m_ci_precautions
      case 4:  return  "1";         // m_ci_eff_indesir
      case 7:  return  "4";         //
      case 11: return  "4";         //
      default: return  "4";         //
    }
}

//-------------------------------- getInteractionFromCipList ------------------------------------------------------
/*! \brief return interactions xml results from a cip list.
 *  \param const QStringLits &cipList      drugs CIP codes list to test.
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
/*
QStringList   C_BDM_TheriaquePlugin::getInteractionFromCipList ( const QStringList &cipList)
{QString     listId     = "";
 QString     interXml   = "";
 QString     cip        = "";
 QString     text       = "";
 int         id         = 0;
 QMap <int, QString> map_id_cip;       // permettra de retrouver le cip a partir de l'id theriaque
 QStringList listRet;
 if (cipList.count()<=1) return  QStringList();
 //............. creer la liste des id theriaque .....................
 for (int i=0; i<cipList.count(); ++i)
     {cip     = cipList.at(i);
      id      = CIP_to_ID(cip);
      listId += QString::number(id) + "','";      // chaine d'appel de la requete  1236','4569','7895','
      map_id_cip[id] = cip;                       // on mape l'id theriaque avec le cip
     }
 listId = listId.left(listId.length()-3);         // virer le dernier  ','
 //............ requeter et extraire les donnees ........................
 C_GET_THE_GTIAM_TXT_CUR  C_GET_THE_GTIAM_TXT ( database(), this );
 C_GET_THE_GTIAM_ID_CUR   C_GET_THE_GTIAM_ID  ( database(), this );
 while ( C_GET_THE_GTIAM_ID.next( listId , "4"  ) )
 {   text = "";
     while ( C_GET_THE_GTIAM_TXT.next(  C_GET_THE_GTIAM_ID.MY_IDINTER()  ) )
           { text += C_GET_THE_GTIAM_TXT.MY_TEXTE()+"\n";
           }
     interXml = QString("<itli>"
                        "<itli_code1>%1</itli_code1><itli_typcode1>%2</itli_typcode1><itli_terme1>%3</itli_terme1>"
                        "<itli_code2>%4</itli_code2><itli_typcode2>%5</itli_typcode2><itli_terme2>%6</itli_terme2>"
                        "<itli_texte>%7</itli_texte><itli_ityp>%8</itli_ityp><itli_level>%9</itli_level>"
                        "</itli>").arg(  map_id_cip[C_GET_THE_GTIAM_ID.ID_T1()], "CIP",     C_GET_THE_GTIAM_ID.TERME_1(),
                                         map_id_cip[C_GET_THE_GTIAM_ID.ID_T2()], "CIP",     C_GET_THE_GTIAM_ID.TERME_2(),
                                         text.replace("\n","<br/>").remove('\r'),"AFSSAPS", gitiamLevelToBdmLevel(C_GET_THE_GTIAM_ID.NIVEAU()));

     listRet << interXml;
 }
 return listRet;
}
*/
//---------------------------- parseHtmlGestionDesRisques ---------------------------------------
/*! \brief create an init the pgr table for gestion des risques (not found in theriaque database)
 *  \param const QString &file     HAS html file from wich extracts and parses data.
 *  \return records number
 */

// http://www.ansm.sante.fr/var/ansm_site/storage/original/application/bf4aaa2f8ad5d04b325f1275ef4d7616.pdf
int           C_BDM_TheriaquePlugin::parseHtmlGestionDesRisques(const QString &file)
{   if (dbi_database()==0)  return 0;
    QString htmlPrefix = "http://www.ansm.sante.fr/var/ansm_site/storage/original/application/";
    QString motifDeb   = "<a  class=\"\" href=\"/var/ansm_site/storage/original/application/";
    QString motifEnd   = "<img src=\"/design/afssaps/images/ico_files/pdf.gif\" alt=\"\" />";
    QString motifMid   = ".pdf\" target=\"_blank\"> ";
    QString pgr        = CGestIni::Param_UpdateFromDisk(file); if (pgr.length()==0) return 0;
    QString extracted  = "";
    QString nameSpe    = "";
    QString infoPgr    = "";
    QString namePdf    = "";
    int pos            = 0;
    int e              = 0;
    int deb            = 0;
    int end            = 0;
    m_PgrMap.clear();
    while (deb!=-1 && deb < pgr.length())
    {deb       = pgr.indexOf(motifDeb, deb);
     if (deb  == -1)  break;
     deb      += motifDeb.length();
     end       = pgr.indexOf(motifEnd, deb);
     if (end==-1)  break;
     //.............. traiter l'extraction .............
     extracted = pgr.mid(deb,end-deb).trimmed();      // dc21b76c363f06b65935c095a3dfb87c.pdf" target="_blank"> ACLASTA (30/05/2008)
     pos       = extracted.indexOf(motifMid);         //                                 ^
     if (pos  != -1)
        {namePdf  = extracted.left(pos)+".pdf";
         pos     += motifMid.length();                //                                                        ^
         if (pos != -1)
            { e = extracted.indexOf('(', pos);
              if (e != -1)
                 {nameSpe = extracted.mid(pos , e-pos).trimmed();;
                  //deb     = e;
                  infoPgr = extracted.mid(e).trimmed() + htmlPrefix + namePdf;
                  m_PgrMap[nameSpe] = infoPgr;
                 }
            }
        }
    }
    //....................... on cree la table ......................................
    if (dbi_database()->dropTable(  "my_sp_pgr")==0) return 0;
    if (dbi_database()->createTable("CREATE TABLE `my_sp_pgr` ("
                    "`my_sp_pgr_pk`            BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`my_sp_pgr_owner`         VARCHAR(40)  ,"
                    "`my_sp_pgr_lang`          VARCHAR(4)   ,"
                    "`my_sp_pgr_libelle`       VARCHAR(255) ,"
                    "`my_sp_pgr_pdf`           VARCHAR(255) ,"
                    "`my_sp_pgr_url`           VARCHAR(255) ,"
                    "`my_sp_pgr_sp_code_sq_pk` BIGINT ,"
                    "PRIMARY KEY (`my_sp_pgr_pk`)"
                    ")"
                    )==0) return 0;
    //............ on parse le QMAP et on rempli la table ....................
    //.......................... numero de version ..........................................
    QString prepare  =      (" INSERT INTO `my_sp_pgr` ( "
                             "`my_sp_pgr_owner`,"         // 0
                             "`my_sp_pgr_lang`,"          // 1
                             "`my_sp_pgr_libelle`,"       // 2
                             "`my_sp_pgr_pdf`,"           // 3
                             "`my_sp_pgr_url`,"           // 4
                             "`my_sp_pgr_sp_code_sq_pk`"  // 5
                            ") VALUES (?,?,?,?,?,?)");

    QSqlQuery query(QString::null , dbi_database()->database() );
    QMapIterator<QString, QString> it(m_PgrMap);
    while (it.hasNext())
      { it.next();
        query.prepare(prepare);
        query.bindValue(0,  owner());
        query.bindValue(1,  lang()) ;
        query.bindValue(2,  it.key()) ;
        query.bindValue(3,  "") ;
        query.bindValue(4,  it.value());
        query.bindValue(5,  "");
        if ( !query.exec())
           { outSQL_error( query, "ERREUR  : C_BDM_Api::parseHtmlGestionDesRisques() queryInsert", prepare, __FILE__, __LINE__);
           }
      }
   return m_PgrMap.size();
}
//---------------------------- makeLigneGenerique ---------------------------------------
QString       C_BDM_TheriaquePlugin::makeLigneGenerique(const QString &nom, const QString &cip)
{   QString    ret  = "";
    QString s_effno = "";
    QString s_ratio = tr("indisponible");
    QString s_emba  = "";
    QString s_prix  = getPrixUCDFromCIP(cip);            // es tu l\303\240 ?
    double    prix  = s_prix.toDouble();
    if (prix!=0.0)
       { QString s_quantite = "";
         double    quantite = 0.0;
         //plan A C_GET_COND_FROM_CIP_CUR
         //plan B C_GET_PRE_COND_PRIM : 8 SACHET(S) 1.566 G
         C_GET_COND_FROM_CIP_CUR  C_GET_COND_FROM_CIP ( database(), this );
         while ( C_GET_COND_FROM_CIP.next( cip) )
               { s_quantite = C_GET_COND_FROM_CIP.NB_UNITE();
                 s_emba     = C_GET_COND_FROM_CIP.UNITE_NAME();
               }
         quantite = s_quantite.toDouble();
         if (quantite==0.0)
            { C_GET_PRE_COND_PRIM_CUR  C_GET_PRE_COND_PRIM ( database(), this );
              while ( C_GET_PRE_COND_PRIM.next( cip) )
                    { s_quantite = C_GET_PRE_COND_PRIM.NB_CONTENANT();
                      s_emba     = C_GET_PRE_COND_PRIM.EMBALLAGE();
                      if (C_GET_PRE_COND_PRIM.QUANTITE().toInt()>0 )
                         {s_quantite = QString::number(s_quantite.toInt()*C_GET_PRE_COND_PRIM.QUANTITE().toInt());
                          s_emba     = C_GET_PRE_COND_PRIM.QUANTITE_UNITE();
                         }
                    }
            }
         quantite = s_quantite.toDouble();
         if (quantite!=0.0 )
            {
              double ratio = s_prix.toDouble()*10/s_quantite.toDouble();
              s_ratio      = QString::number(ratio,'f',3)+"<br/>"
                             + s_quantite + " " + s_emba;
            }
        }
    else
       {s_ratio = tr("prix indisponible");
       }
    //....... recuperer le code specialite ......................
    QString s_code_sp = "";
    C_GET_THE_CODE_FROM_CIP_CUR  C_GET_THE_CODE_FROM_CIP ( database(), this );
    while ( C_GET_THE_CODE_FROM_CIP.next( cip) )
          { s_code_sp = C_GET_THE_CODE_FROM_CIP.THE_CODE();
          }

    C_GET_THE_EXP_ID_CUR   C_GET_THE_EXP_ID( database() , this );
    C_GET_THE_SUB_SPE_CUR  C_GET_THE_SUB_SPE( database() , this );
    while ( C_GET_THE_SUB_SPE.next( s_code_sp.toInt() ,1  ) )                        // on parse les excipients
          {while ( C_GET_THE_EXP_ID.next( C_GET_THE_SUB_SPE.CODESUBST() ) )  // on verifie si effet notoire
                 {s_effno += C_GET_THE_SUB_SPE.LIBSUBST()+"<br/>";
                 }
          }
    if   (s_effno.endsWith("<br/>")) s_effno = s_effno.left(s_effno.length()-4);
    else                            s_effno =  tr("non concern\303\251");

    ret += "<TR><TD width=\"45%\";>"+nom+"<br/>"+insertHtmlLink(cip,QString("Drug_CIP=")+cip)+"</TD>"+"<TD>"+s_ratio+"</TD>"+"<TD>"+s_prix+"</TD>"+"<TD>"+s_effno+"</TD></TR>";
    return ret;
}
//------------------------------ insertHtmlLink ------------------------------------------
QString       C_BDM_TheriaquePlugin::insertHtmlLink(const QString text, const QString &url)
{       return QString  ("<a href=\"%2\">%1</a>").arg(text, url);
}
//--------------------------- CIP_to_ID -----------------------------------------------------------
/*! \brief return theriaque ID code from CIP code.
 *  \param const QString cip is drug cip code.
 *  \return theriaque ID code
*/
int  C_BDM_TheriaquePlugin::CIP_to_ID(const QString &cip)
{   //............. recuperer code id teriaque .........................
    C_GET_THE_SPECIALITE_CUR    C_GET_THE_SPECIALITE   ( database(), this );
    while ( C_GET_THE_SPECIALITE.next (  cip ) )           // results cursor
          {return C_GET_THE_SPECIALITE.SP_CODE_SQ_PK();    // id theriaque
          }
    return 0;
}

//-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
/*! \brief return the prise unit form from cip. (comprim\303\251s, suppositoires sachets,ampoules,doses kg, cuill\303\250res \303\240 caf\303\251 etc...)
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \return QString with unit form
 */
QString   C_BDM_TheriaquePlugin::getPriseUnitFormFromCIP ( const QString &cip, const QString &commercialName /*=0*/ )
{
    int codeId      = CIP_to_ID(cip);
    QString message = "";
    QString forme   = "";
    /*
    C_GET_THE_SUB_SPE_CUR  C_GET_THE_SUB_SPE( database() , this );
    while ( C_GET_THE_SUB_SPE.next(codeId ,2  ) )
          {if (commercialName.length())
              {message += QString ("... C_GET_THE_SUB_SPE : \n LIBSUBST %1 \n DOSESUBST %2 \n UDOSESUBST %3 \n TYPSUBST %4 \n ").arg(
               C_GET_THE_SUB_SPE.LIBSUBST(),
               C_GET_THE_SUB_SPE.DOSESUBST(),
               C_GET_THE_SUB_SPE.UDOSESUBST(),
               C_GET_THE_SUB_SPE.TYPSUBST());
              }
          }
    */
    //............ verifier si cette requete repond (c'est parfois le cas ..) ..................
    //             reponses plus adaptees que la suivante si repond
    C_GET_COND_FROM_CIP_CUR  C_GET_COND_FROM_CIP ( database(), this );
    while ( C_GET_COND_FROM_CIP.next( cip) )
          { if (commercialName.length())
               {message += QString ("... C_GET_COND_FROM_CIP : \n PRE_NAME %1 \n NB_UNITE %2 \n UNITE_NAME %3 \n ").arg(
                C_GET_COND_FROM_CIP.PRE_NAME(),
                C_GET_COND_FROM_CIP.NB_UNITE(),
                C_GET_COND_FROM_CIP.UNITE_NAME());
               }
           forme  =  C_GET_COND_FROM_CIP.UNITE_NAME().toLower().remove("(s)");
          }

    if (forme.length())
       {QString grml = forme.left(6);
        if      (grml=="compri") return forme;
        else if (grml=="sachet") return forme;
        else if (grml=="ampoul") return forme;
        else if (grml=="inject") return forme;
        else if (grml=="goutte") return forme;
       }
    /*
    C_GET_PRE_COND_PRIM_CUR  C_GET_PRE_COND_PRIM ( database(), this );
    while ( C_GET_PRE_COND_PRIM.next( cip) )
          { if (commercialName.length())
               {message += tr("...Conditionnement primaire : \n NB_CONTENANT %1\n EMBALLAGE %2\n QUANTITE %3\n QUANTITE_UNITE %4\n ").arg
                          (C_GET_PRE_COND_PRIM.NB_CONTENANT(),
                           C_GET_PRE_COND_PRIM.EMBALLAGE(),
                           C_GET_PRE_COND_PRIM.QUANTITE(),
                           C_GET_PRE_COND_PRIM.QUANTITE_UNITE());
               }
          }
    */
    //............ sinon autre methode  ...........................................................
    forme = "";
    C_GET_PRISE_UNIT_FROM_CODE_ID_CUR  C_GET_PRISE_UNIT_FROM_CODE_ID ( database(), this );
    while ( C_GET_PRISE_UNIT_FROM_CODE_ID.next( codeId ) )
          {
            if (commercialName.length())
               { message += QString ("... C_GET_PRISE_UNIT_FROM_CODE_ID : \n NOFIC %1 \n NATURE %2 \n MINI %3 \n FORME %4 \n MAXI %5 \n").arg(
                 C_GET_PRISE_UNIT_FROM_CODE_ID.NOFIC(),
                 C_GET_PRISE_UNIT_FROM_CODE_ID.NATURE(),
                 C_GET_PRISE_UNIT_FROM_CODE_ID.MINI(),
                 C_GET_PRISE_UNIT_FROM_CODE_ID.FORME(),
                 C_GET_PRISE_UNIT_FROM_CODE_ID.MAXI());
               }
            if (forme.length()==0)
               {forme   = C_GET_PRISE_UNIT_FROM_CODE_ID.FORME_UNIT();
               }
          }
    if (commercialName.length()) qDebug()<< message.prepend(tr("---- getPriseUnitFormFromCIP (%1)-----\n").arg(commercialName));
    return QString(forme);
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
 QList<C_CompositionItem> C_BDM_TheriaquePlugin::getCompositionFromCIP(const QString &cip)
 {   QList<C_CompositionItem> retList;
     double codeId       = CIP_to_ID(cip);
     double lastNumOrdre = -1;
     C_GET_THE_SUB_SPE_CUR  C_GET_THE_SUB_SPE( database() , this );
     while ( C_GET_THE_SUB_SPE.next( codeId , 2  ) )
           {QString qu = C_GET_THE_SUB_SPE.DOSESUBST();
            int pos = qu.indexOf('.');
            if (pos != -1)
               { int i   = qu.length()-1;
                 while (i > pos && qu.at(i)=='0')  --i;
                 if (i==pos) qu=qu.left(i);     // ya que des zero apres la virgule
                 else        qu=qu.left(i+1);
               }
            //........................ theriaque peut renvoyer la variete de situations suivantes ..........................
            //   pour le meme composant donc meme NUMORDRE() :
            //        exple LAMALINE NUMORDRE() == 2
            //                    C_GET_THE_SUB_SPE.DOSESUBST() 1er  tour  -->  OPIUM PULVERISE 10 MG
            //                    C_GET_THE_SUB_SPE.DOSESUBST() 2eme tour  -->  exprim\303\251 (e) en MORPHINE BASE ANHYDRE 1 MG
            //        exple AUGMENTIN NUMORDRE() == 1
            //                    C_GET_THE_SUB_SPE.DOSESUBST() 1er  tour -->   NON RENSEIGNE
            //                    C_GET_THE_SUB_SPE.DOSESUBST() 2eme tour  -->  exprim\303\251 (e) en AMOXICILLINE ANHYDRE 100 MG
            //        exple DOLIPRANE NUMORDRE() == 1     (pas de deuxieme tout pour la meme substance)
            //                    C_GET_THE_SUB_SPE.DOSESUBST() 1er  tour  -->  PARACETAMOL 100 MG
            //
            //  explicit C_InterResultPair(const QString &code1, const QString &codeTyp1,  const QString &term1,
            //                             const QString &code2, const QString &codeTyp2,  const QString &term2,
            //                             const QString &info,  const QString &typ,       const QString &level)
            if (C_GET_THE_SUB_SPE.DOSESUBST() != "NON RENSEIGNE" &&  // si NON RENSEIGNE esperons qu'une autre forme valide sorte au prochain tour
                C_GET_THE_SUB_SPE.NUMORDRE()  != lastNumOrdre        // on ne retient qu'une seule forme d'expression du composant
               )
               {
                  retList.append ( C_CompositionItem( C_GET_THE_SUB_SPE.LIBSUBST().remove("exprim\303\251(e) en "),
                                                      qu,
                                                      C_GET_THE_SUB_SPE.UDOSESUBST(),
                                                      C_GET_THE_SUB_SPE.UDOSESUBST()+"(s)"
                                                     )
                  /*
                                   C_InterResultPair(qu,  C_GET_THE_SUB_SPE.UDOSESUBST()       ,       C_GET_THE_SUB_SPE.LIBSUBST().remove("exprim\303\251(e) en "),
                                                     "",  C_GET_THE_SUB_SPE.UDOSESUBST()+"(s)" ,                                                          "",   // pluriel pareil
                                                     "",                            "TH",  QString::number(C_GET_THE_SUB_SPE.NUMORDRE())
                                                    )
                  */
                                 );
                  lastNumOrdre = C_GET_THE_SUB_SPE.NUMORDRE();
               }
           }
     /*
     QString html;
     C_GET_THE_SUB_PRECCOMP_SPE_CUR  C_GET_THE_SUB_PRECCOMP_SPE ( database(), this );
     while ( C_GET_THE_SUB_PRECCOMP_SPE.next( codeId  ) )
           { html += tr("<br><b> GET_THE_SUB_PRECCOMP_SPE : </b>  ") + C_GET_THE_SUB_PRECCOMP_SPE.TEXTPREC();
           }
     */
    return retList;
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
 QList<C_ExcipientItem> C_BDM_TheriaquePlugin::getExcipientsFromCIP(const QString &cip)
 {   Q_UNUSED(cip);
     QList<C_ExcipientItem>        result;
     return result;
 }
 //-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
 /*! \brief return drug composition.
  *  \param const QString& cip drug cip identification .
  *  \return QStringList result list with dci composants
  */
 QStringList C_BDM_TheriaquePlugin::get_DCI_list_FromCIP(const QString &cip)
 {
          QStringList retList;
          double lastNumOrdre = -1;
          double codeId       = CIP_to_ID(cip);
          C_GET_THE_SUB_SPE_CUR  C_GET_THE_SUB_SPE( database() , this );
          while ( C_GET_THE_SUB_SPE.next( codeId , 2  ) )
                {//........................ theriaque peut renvoyer la variete de situations suivantes ..........................
                 //   pour le meme composant donc meme NUMORDRE() :
                 //        exple LAMALINE NUMORDRE() == 2
                 //                    C_GET_THE_SUB_SPE.DOSESUBST() 1er  tour  -->  OPIUM PULVERISE 10 MG
                 //                    C_GET_THE_SUB_SPE.DOSESUBST() 2eme tour  -->  exprim\303\251 (e) en MORPHINE BASE ANHYDRE 1 MG
                 //        exple AUGMENTIN NUMORDRE() == 1
                 //                    C_GET_THE_SUB_SPE.DOSESUBST() 1er  tour -->   NON RENSEIGNE
                 //                    C_GET_THE_SUB_SPE.DOSESUBST() 2eme tour  -->  exprim\303\251 (e) en AMOXICILLINE ANHYDRE 100 MG
                 //        exple DOLIPRANE NUMORDRE() == 1     (pas de deuxieme tout pour la meme substance)
                 //                    C_GET_THE_SUB_SPE.DOSESUBST() 1er  tour  -->  PARACETAMOL 100 MG
                 //
                 //  explicit C_InterResultPair(const QString &code1, const QString &codeTyp1,  const QString &term1,
                 //                             const QString &code2, const QString &codeTyp2,  const QString &term2,
                 //                             const QString &info,  const QString &typ,       const QString &level)
                 if (C_GET_THE_SUB_SPE.DOSESUBST() != "NON RENSEIGNE" &&  // si NON RENSEIGNE esperons qu'une autre forme valide sorte au prochain tour
                     C_GET_THE_SUB_SPE.NUMORDRE()  != lastNumOrdre        // on ne retient qu'une seule forme d'expression du composant
                    )
                    {
                       retList.append ( C_GET_THE_SUB_SPE.LIBSUBST().remove("exprim\303\251(e) en ") );
                       lastNumOrdre = C_GET_THE_SUB_SPE.NUMORDRE();
                    }
                }
         return retList;
 }

 //-------------------------------- getFormeFromCIP ------------------------------------------------------
 /*! \brief return drug presentation form from cip. (comprim\303\251s, suppositoires sachets,ampoules, suspension buvable etc...)
  *  \param const QString &cip      code CIP of the drug from wich wee want datas.
  *  \return QString with presentation form
  */
 QString   C_BDM_TheriaquePlugin::getFormeFromCIP ( const QString &cip)
 {   double  codeId       = CIP_to_ID(cip);

     QString result       = "";
     C_GET_THE_FORME_SPE_CUR  C_GET_THE_FORME_SPE ( database(), this );
     while ( C_GET_THE_FORME_SPE.next( codeId  ) )
           { result += " "+ C_GET_THE_FORME_SPE.LIBFO();
           }
     /*
     QString txt;
     C_GET_THE_FORME_COMP_SPE_CUR  C_GET_THE_FORME_COMP_SPE ( database(), this );
     //while ( C_GET_THE_FORME_COMP_SPE.next( codeId , 1  ) )
     //      { txt += " "+ C_GET_THE_FORME_COMP_SPE.LIBFO();
     //      }
     //C_GET_THE_FORME_COMP_SPE_CUR  C_GET_THE_FORME_COMP_SPE ( database(), this );
     while ( C_GET_THE_FORME_COMP_SPE.next( codeId , 2  ) )
           { txt += " " + C_GET_THE_FORME_COMP_SPE.LIBFO();
           }
    */
     return result;
 }
 //-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
 /*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
  *         if there is not restrictive prescription for this CIP  isValid() is false;
  *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
  *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
  *  \return QList<C_PrescriptionRestreinte>
  */
 QList<C_PrescriptionRestreinte> C_BDM_TheriaquePlugin::getPrescriptionRestreinteFromCIP(const QString &cip)
 {   Q_UNUSED( cip );
     QList<C_PrescriptionRestreinte> retList;
     return retList;
 }
//------------------------------ getMonographieFromCIP ------------------------------------------
/*! \brief select monographie for a drug.
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
 *  \return QString contains monographie
 */
QString       C_BDM_TheriaquePlugin::getMonographieFromCIP ( const QString &cip , bool dataMustBeReadInCache /* = TRUE */ )
{   Q_UNUSED(dataMustBeReadInCache);
    if (0) //dataMustBeReadInCache)
       {C_GET_MY_MONO_INCACHE_CUR  C_GET_MY_MONO_INCACHE ( dbi_database()->database(), this );
        if ( C_GET_MY_MONO_INCACHE.next( cip) )
           { return  C_GET_MY_MONO_INCACHE.MONOGRAPHIE();
           }
       }

 /*
  Formes pharmaceutiques et composition. *
  Classification et pharmacologie. *
  Recommandations. *
  Renseignements administratifs. *
  Conservation.*
  Indications. *
  Non indications.*
  Posologie administrations.*
  Contre-indications.*
  Mise en garde et pr\303\251cautions d'emploi.*
  Non contre-indications.*
  Interactions m\303\251dicamenteuses.*
  Grossesse et allaitement. *
  Conduite de v\303\251hicules. *
  Effets ind\303\251sirables.*
  */
 QString javaS_CreatePanel_Body =  "<script type=\"text/javascript\">\n"             // ce bloc contient les creations des panneaux et est \303\240 la fin
                                   "<!--\n";
 QString path                   = ".";
 QString block                  = ".";
 QString tmp                    = "";
 QString str                    = "";
 QString dum                    = "";
 QString html                   = QString  ("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
                                               "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
                                               "<head>\n"
                                               "<title>BDM -Monographie : %2</title>\n"
                                               "<link href=\"%1/monographie_THE.css\" rel=\"stylesheet\" type=\"text/css\">\n"
                                               "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF8\">\n"
                                               "<script src=\"%1/SpryCollapsiblePanel.js\" type=\"text/javascript\"></script>\n"
                                               "<link href=\"%1/SpryCollapsiblePanel.css\" rel=\"stylesheet\" type=\"text/css\">\n"
                                               "<script id=\"clientEventHandlersJS\" language=\"javascript\">\n"
                                               "<!--\n"
                                               "function openClosePanel(numpanel)\n"
                                               "{    if (eval(\"CollapsiblePanel\" + numpanel).contentIsOpen == false) {\n"
                                               "        document.images['bouton' + numpanel].src = \"%1/panel_fermer.png\";\n"
                                               "        document.images['bouton' + numpanel].title = \"Fermer\";\n"
                                               "        } else {\n"
                                               "        document.images['bouton' + numpanel].src = \"%1/panel_ouvrir.png\";\n"
                                               "        document.images['bouton' + numpanel].title = \"Ouvrir\";\n"
                                               "        }\n"
                                               "}\n"
                                               "//-->\n"
                                               "</script>\n"
                                               "</head>\n"
                                               "<body>\n"
                                           ).arg(path, dataSourceVersion() ) ;

     //................................. construction de l'HTML ......................................

     C_GET_THE_SPECIALITE_CUR    C_GET_THE_SPECIALITE   ( database(), this );
     int indexCollapsiblePanel = 0;
     int isResult              = 0;
     while ( C_GET_THE_SPECIALITE.next (  cip ) )    // results cursor
           {// int SP_PR_CODE_FK = C_GET_THE_SPECIALITE.SP_PR_CODE_FK();
            int SP_CODE_SQ_PK  = C_GET_THE_SPECIALITE.SP_CODE_SQ_PK();    // id theriaque
            int codeId         = SP_CODE_SQ_PK;
            QString s_codeId   = QString::number(SP_CODE_SQ_PK);
            QString s_subAct_1 = "";
            // ....... titre du medoc ............................................................
            QString title = QString("<b>")+C_GET_THE_SPECIALITE.SP_NOM();
            title += "</b><font size=1> ID : "+s_codeId+" CIP : "+cip+"<br/>("+C_GET_THE_SPECIALITE.SP_NOMLONG();
            if (C_GET_THE_SPECIALITE.SP_NOMCOMP().length()) title += " "+ C_GET_THE_SPECIALITE.SP_NOMCOMP();
            title += ")</font>";
            html  += tr("<p class=nom_spe > %1</p>") .arg(title);
            //html +="<SCRIPT language=\"JavaScript\" src=\"menu_dynamique.js\"></SCRIPT>\n";
            //........................... GENERIQUES ........................................................
            block = "";
            tmp   = "";
            dum   = "";
            if (C_GET_THE_SPECIALITE.SP_TYPE_SPE()=="G"||C_GET_THE_SPECIALITE.SP_TYPE_SPE()=="R")
               {C_GET_THE_GEN_SPE_CUR  C_GET_THE_GEN_SPE ( database(), this );
                QString last_gen = "";
                QString last_cip = "";

                tmp      = "";
                str      = "";
                dum      = "";
                //,,,,,,,,,,,,,, parfois la specialite cliquee est un generique non listee dans liste afssaps .............
                if (C_GET_THE_SPECIALITE.SP_TYPE_SPE()=="G")
                   { str +=   makeLigneGenerique(C_GET_THE_SPECIALITE.SP_NOM(),cip );    // on le rentre l\303\240 car parfois pas dans liste afssaps
                   }
                //,,,,,,,,,,,,,, parfois la specialite cliquee est un generique referent .............
                if (C_GET_THE_SPECIALITE.SP_TYPE_SPE()=="R")
                   {tmp      = C_GET_THE_SPECIALITE.SP_NOM();
                    dum      = cip;
                    last_gen = tmp;     // on note en cas de panique (aucun truc valide)
                    last_cip = cip;
                    if (getPrixUCDFromCIP(cip).length()==0) // si prix non valide on ne retient pas
                       {tmp  = "";
                        dum  = "";
                       }
                   }

                //,,,,,,,,,,,,,, si referent non valide aller \303\240 la p\303\252che .............
                while (  dum.length()==0 &&   (isResult=C_GET_THE_GEN_SPE.next(codeId , 1 ) ) ) // -- 1 - generiques theriaque
                      { if (C_GET_THE_GEN_SPE.MY_GSP_NOM().endsWith("NSFP")) continue;   // ne pas retenir les NeSeFaitPlus
                        if (tmp.length()==0 && C_GET_THE_GEN_SPE.GEN_REF() == C_GET_THE_GEN_SPE.SP_CODE())
                           { tmp  =  C_GET_THE_GEN_SPE.MY_GSP_NOM();         //  Plan A relever le referent pour le titre
                             dum  =  C_GET_THE_GEN_SPE.PRE_CODE();
                           }
                        if (C_GET_THE_GEN_SPE.GEN_TYPE()=="R")
                           { tmp  =  C_GET_THE_GEN_SPE.MY_GSP_NOM();         //  Plan B relever le referent pour le titre
                             dum  =  C_GET_THE_GEN_SPE.PRE_CODE();
                           }
                        if (tmp.length())
                           { last_gen       = tmp;     // on note en cas de panique (aucun truc valide)
                             last_cip       = dum;
                             if (getPrixUCDFromCIP(dum).length()==0) // si prix non vaide on ne retient pas
                                {tmp  = "";
                                 dum  = "";
                                }
                           }
                      }
                //........... on a trouve un referent on l'ajoute ..............................
                if (dum.length())
                   {tmp     +=   " (r\303\251f. Th\303\251riaque)";
                    str     +=   makeLigneGenerique(tmp , dum );
                   }
                isResult = 0;
                while ( C_GET_THE_GEN_SPE.next(codeId , 2 )  ) // -- 2 -  generiques affsapps
                      {if (C_GET_THE_GEN_SPE.MY_GSP_NOM().endsWith("NSFP")) continue;   // ne pas retenir les NeSeFaitPlus
                       if (dum.length()==0 && C_GET_THE_GEN_SPE.GEN_REF() == C_GET_THE_GEN_SPE.SP_CODE())
                          { tmp  =  C_GET_THE_GEN_SPE.MY_GSP_NOM() + " (r\303\251f. Ansm/Afssaps)";         //  Plan A relever le referent pour le titre
                            dum  =  C_GET_THE_GEN_SPE.PRE_CODE();
                          }
                       if (dum.length()==0 && C_GET_THE_GEN_SPE.GEN_TYPE()=="R" )
                          { tmp  =  C_GET_THE_GEN_SPE.MY_GSP_NOM() + " (r\303\251f. Ansm/Afssaps)";
                            dum  =  C_GET_THE_GEN_SPE.PRE_CODE();
                          }
                       if (isResult==0) block += "<TABLE BORDER=\"1px\"; width=\"100%\">             "
                                                 "<TR>                             "
                                                 "<TH align=\"left\"; width=\"45%\";> Sp\303\251cialit\303\251s g\303\251n\303\251riques Afssaps </TH>  "
                                                 "<TH align=\"left\";> Ratio prix/u</TH>      "
                                                 "<TH align=\"left\";> prix </TH>                  "
                                                 "<TH align=\"left\";> Exp. effets notoires</TH>   "
                                                 "</TR>                            ";
                        str +=   makeLigneGenerique(C_GET_THE_GEN_SPE.MY_GSP_NOM(), C_GET_THE_GEN_SPE.PRE_CODE() );
                        ++isResult;
                      }
                //.......... si pas moyen de trouver un referent dans Afssaps ou Theriaque (avec prix) ............
                //           on prend le dernier Theriaque trouv\303\251 (forc\303\251ment sans prix)
                if (dum.length()==0)
                   {tmp     =   last_gen + " (r\303\251f. Th\303\251riaque)";
                    dum     =   last_cip;
                    str     +=  makeLigneGenerique(tmp , dum );
                   }
                if (str.length())
                   {str   += "</TABLE> ";
                    block += str;
                   }
                if (block.length())
                   {html  += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                          "autres", //C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                                          "",
                                                          tr("R\303\251f\303\251rent g\303\251n\303\251rique : %1<br/>Cip %2 ").arg(tmp,dum),
                                                          "",                                        // cim 10
                                                          " ",
                                                          path,   //R\303\251f. : Journal Officiel du
                                                          block ,
                                                          javaS_CreatePanel_Body,
                                                          "panel_EffeNotoireCss");
                   }
              }
            //......................... COMPOSITION ..............................................................................................
            block = "";
            C_GET_THE_SUB_SPE_CUR  C_GET_THE_SUB_SPE( database() , this );
            while ( (isResult=C_GET_THE_SUB_SPE.next(codeId ,2  )) )
                  {if (isResult==1) {block += tr("<u><b>Substance(s) active(s)</b></u>"); s_subAct_1 = C_GET_THE_SUB_SPE.LIBSUBST();}
                   block += tr("<br/>&nbsp;&nbsp;&nbsp;&nbsp;")  + " " + C_GET_THE_SUB_SPE.LIBSUBST() + "  " +C_GET_THE_SUB_SPE.DOSESUBST()+ " " +C_GET_THE_SUB_SPE.UDOSESUBST();
                  }
            C_GET_THE_EXP_ID_CUR   C_GET_THE_EXP_ID( database() , this );
            QStringList exipientsSansEffetNotoiresList;
            isResult = 0;
            while ( C_GET_THE_SUB_SPE.next(codeId ,1  ))
                  {int isNotEffetNotoire = 1;
                   while ( C_GET_THE_EXP_ID.next(C_GET_THE_SUB_SPE.CODESUBST()))
                      {if (isResult==0) block += tr("<br/><br/><b><u>Exipients \303\240 effets notoires.</b></u>");
                       block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                             C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                                             "",
                                                             C_GET_THE_SUB_SPE.LIBSUBST()+ "  " +C_GET_THE_SUB_SPE.DOSESUBST()+ " " +C_GET_THE_SUB_SPE.UDOSESUBST(),
                                                             "",                                        // cim 10
                                                             tr(" "),
                                                             path,   //R\303\251f. : Journal Officiel du
                                                             C_GET_THE_EXP_ID.SAU_TEXTE()
                                                             .replace(">","&gt;").replace("<","&lt;")
                                                             .replace("- D\303\251nomination",          "<br/> <b><u>D\303\251nomination</u></b>")
                                                             .replace("- Seuil",                        "<br/> <b><u>Seuil</u></b>")
                                                             .replace("-&gt; Seuil",                    "<br/> <b><u>Seuil</u></b>")
                                                             .replace("* Voie d'administration",        "<br/> <b><u>Voie d'administration</u></b>")
                                                             .replace("- Voie d'administration",        "<br/> <b><u>Voie d'administration</u></b>")
                                                             .replace("-&gt; Voie d'administration",    "<br/> <b><u>Voie d'administration</u></b>")
                                                             .replace("- Informations",                 "<br/> <b><u>Informations</u></b>")
                                                             .replace("R\303\251f. : Journal Officiel", "<br/> <b><u>R\303\251f. : Journal Officiel</u></b>")
                                                             ,
                                                             javaS_CreatePanel_Body,
                                                             "panel_EffeNotoireCss");
                       ++isResult;
                       isNotEffetNotoire=0;
                      }
                   if (isNotEffetNotoire)  // si pas d'effet notoire affichage ligne simple
                      {exipientsSansEffetNotoiresList << tr("<br/>&nbsp;&nbsp;&nbsp;&nbsp;") + " " + C_GET_THE_SUB_SPE.LIBSUBST()+ "  " +C_GET_THE_SUB_SPE.DOSESUBST()+ " " +C_GET_THE_SUB_SPE.UDOSESUBST();
                      }
                  }
            if (isResult==0)block += "<br/>";
            for (isResult=0; isResult<exipientsSansEffetNotoiresList.size();++isResult)
                {if (isResult==0) block += tr("<b><u>Exipients sans effets notoires.</b></u>");
                 block += exipientsSansEffetNotoiresList[isResult];
                }
            tmp = "";     // precision concernant la composition
            C_GET_THE_SUB_PRECCOMP_SPE_CUR  C_GET_THE_SUB_PRECCOMP_SPE ( database(), this );
            while ( (isResult=C_GET_THE_SUB_PRECCOMP_SPE.next( codeId  )) )
                  { tmp = C_GET_THE_SUB_PRECCOMP_SPE.TEXTPREC() +"<br/>"+C_GET_THE_SUB_PRECCOMP_SPE.REF_OFF()+" "+C_GET_THE_SUB_PRECCOMP_SPE.DAT_OFF().toString("dd-MM-yyyy");
                  }
            //................. conditionnement ........................................
            /*
            C_GET_THE_PRE_CDT_CUR  C_GET_THE_PRE_CDT ( database(), this );
            while ( C_GET_THE_PRE_CDT.next( cip , 1  ) )
                  { block += tr("<br/><b><u>Conditionnement</u></b> ")
                             + " type     "  + C_GET_THE_PRE_CDT.TYPE_PRE_CDT()
                             + " info_1   "  + C_GET_THE_PRE_CDT.INFO_1()
                             + " valeur_1 "  + QString::number(C_GET_THE_PRE_CDT.VALEUR_1())
                             + " info_2   "  + C_GET_THE_PRE_CDT.INFO_2()
                             + " valeur_2 "  + QString::number(C_GET_THE_PRE_CDT.VALEUR_2())
                             + " info_3   "  + C_GET_THE_PRE_CDT.INFO_3()
                             + " valeur_3 "  + QString::number(C_GET_THE_PRE_CDT.VALEUR_3())
                             + " valeur_4 "  + QString::number(C_GET_THE_PRE_CDT.VALEUR_4())
                             ;
                  }
            */
            C_GET_COND_FROM_CIP_CUR  C_GET_COND_FROM_CIP ( database(), this );
            while ( C_GET_COND_FROM_CIP.next( cip) )
                  { block += tr("<br/><b><u>Conditionnement</u> : </b> :   ")
                               + C_GET_COND_FROM_CIP.PRE_NAME() + " "
                               + C_GET_COND_FROM_CIP.NB_UNITE() + " "
                               + C_GET_COND_FROM_CIP.UNITE_NAME();
                  }
            str = "";
            C_GET_PRE_COND_PRIM_CUR  C_GET_PRE_COND_PRIM ( database(), this );
            while ( C_GET_PRE_COND_PRIM.next( cip) )
                  { str = tr("<br/><b><u>Conditionnement primaire</u> : </b>  ")
                          + C_GET_PRE_COND_PRIM.NB_CONTENANT()    + "  "
                          + C_GET_PRE_COND_PRIM.EMBALLAGE()       + "  "
                          + C_GET_PRE_COND_PRIM.QUANTITE()        + "  "
                          + C_GET_PRE_COND_PRIM.QUANTITE_UNITE()  + "  ";
                  }
            block += str;
            html  += tr("<p class=nom_chapitres> COMPOSITION %1</p> %2") .arg(tmp, block);

            //........................... CLASSIFICATION ATC................................................................................
            block = tr("<b><u>Classification ATC</u> : %1</b>").arg(C_GET_THE_SPECIALITE.SP_CATC_CODE_FK());
            tmp   = C_GET_THE_SPECIALITE.SP_CATC_CODE_FK();
            C_GET_THE_ATC_ID_CUR  C_GET_THE_ATC_ID ( database(), this );
            int n = 0;
            for (int i=0; i<=tmp.length();++i)
                {QString code = tmp.left(i);
                 while ( C_GET_THE_ATC_ID.next( code  ) )
                       { n +=2 ;
                         block  += tr ("<br/>%1").arg(QString("&nbsp;&nbsp;").repeated(n))
                                + C_GET_THE_ATC_ID.CATC_NOMF()
                                   + " : " + code;
                       }
                }
            C_GET_MARCHE_PUBLIC_SPE_CUR  C_GET_MARCHE_PUBLIC_SPE ( database(), this );
            while ( C_GET_MARCHE_PUBLIC_SPE.next( s_codeId ) )
                  { block += tr("<br/><b><u>Nomenclature du code des march\303\251s publics</b></u> : %1").arg(C_GET_MARCHE_PUBLIC_SPE.CMP_LIBELLE());
                  }

            html += tr("<p class=nom_chapitres> CLASSIFICATIONS</p>%1") .arg(block);
            //............................ RENSEIGNEMENTS ADMINISTRATIFS ...................................................................

            isResult = 0;         //.......................... dispensation ....................................
            block    = "";
            C_GET_THE_PRE_DSP_CUR C_GET_THE_PRE_DSP( database() , this );
            while ( (isResult=C_GET_THE_PRE_DSP.next(cip ,1  )) )
                  {if (isResult==1) block += tr("<b><u>Dispensation.</b></u>");
                   if (C_GET_THE_PRE_DSP.INFO_1().contains("LISTE")) continue;
                   block += "<br/> <b>&nbsp; &nbsp; &nbsp;</b>"+C_GET_THE_PRE_DSP.INFO_1();
                  }
            while ( (isResult=C_GET_THE_PRE_DSP.next(cip ,2  )) )
                  {if (isResult==0) block += tr("<b><u>Dispensation.</b></u>");
                   block += "<br/> <b>&nbsp; &nbsp; &nbsp;</b>"+C_GET_THE_PRE_DSP.INFO_1();
                   ++isResult;
                  }


            block += tr("<br/><b><u>Liste</u> : </b> %1") .arg(C_GET_THE_SPECIALITE.SP_LISTE());
            block +="<br/><i>&nbsp; &nbsp; &nbsp; liste I : toxiques dans les conditions normales d'emploi."
                    "<br/>&nbsp; &nbsp; &nbsp; liste II : dangereux en conditions anormales d'emploi.</i>";
            block += tr("<br/><b><u>Statut labo</u> : </b>%1") .arg(C_GET_THE_SPECIALITE.STATUT_LABO());
            //.............. agrement aux collectivites ..........................
            C_GET_AGR_COLLECT_FROM_CIP_CUR  C_GET_AGR_COLLECT_FROM_CIP ( database(), this );
            while ( C_GET_AGR_COLLECT_FROM_CIP.next( cip) )
                  { if ( C_GET_AGR_COLLECT_FROM_CIP.AGRCOLL()=="O")
                       { block += tr("<br/><b><u>Agr\303\251ment aux collectivit\303\251s</u> : </b> Date JO ")
                             + C_GET_AGR_COLLECT_FROM_CIP.DATEJOCOLL("dd-MM-yyyy");
                       }
                  }
            C_GET_RES_HOP_FROM_CIP_CUR  C_GET_RES_HOP_FROM_CIP ( database(), this );
            while ( C_GET_RES_HOP_FROM_CIP.next( cip) )
                  { block += tr("<br/><b><u> Reserv\303\251 \303\240 l'hopital</u> : </b>  ") + C_GET_RES_HOP_FROM_CIP.RESULT();
                  }

            QString remb = "";     //....................  CONDITIONS DE PRESCRIPTION, DE D\303\251LIVRANCE REMBOURSEMENT.........
            isResult     = 0;
            C_GET_THE_PRE_RBT_CUR  C_GET_THE_PRE_RBT ( database(), this );
            while ( C_GET_THE_PRE_RBT.next( cip , 1  ) )
                  { if (C_GET_THE_PRE_RBT.TYPE_PRE_RBT()=="1")       // C_GET_THE_PRE_RBT.TYPE_PRE_RBT()[0]=='1')              // taux de remboursement C_GET_THE_PRE_RBT.INFO_1()[0]>='0' && C_GET_THE_PRE_RBT.INFO_1()[0]<='9')
                       { remb  += tr("<br/>&nbsp; &nbsp; &nbsp;") + C_GET_THE_PRE_RBT.INFO_1();
                         if (C_GET_THE_PRE_RBT.DATE_2().isValid()) remb  += " date d'application : " + C_GET_THE_PRE_RBT.DATE_2().toString("dd-MM-yyyy");
                         if (C_GET_THE_PRE_RBT.TEXTE().length())   remb  += " (" + C_GET_THE_PRE_RBT.TEXTE()+")";
                       }
                    else  if (C_GET_THE_PRE_RBT.TYPE_PRE_RBT()=="3")      // pathologies ALD  RAPPORT DIRECT  RAPPORT INDIRECT SANS RAPPORT
                       { if (isResult==0) block += tr("<br/><b><u>Statut(s) du remboursement selon pathologie.</u></b>");
                         block += "<br/>&nbsp; &nbsp; &nbsp;";
                         if (C_GET_THE_PRE_RBT.INFO_1().contains("INDIRECT"))
                            {block += QString("<font color=#1e6302>%1</font>").arg(C_GET_THE_PRE_RBT.INFO_1());
                            }
                         else if (C_GET_THE_PRE_RBT.INFO_1().contains("SANS"))
                            {block += QString("<font color=#939393>%1</font>").arg( C_GET_THE_PRE_RBT.INFO_1());
                            }
                         else
                            {block += QString("<font color=#1e6302><b>%1</b></font>").arg(C_GET_THE_PRE_RBT.INFO_1());
                            }
                         ++isResult;
                       }
                  }
            if (remb.length())  block += tr("<br/><b><u>Taux de remboursement.</u></b>") + remb;
            C_GET_THE_SPE_STATUT_CUR  C_GET_THE_SPE_STATUT ( database(), this );
            while ( (isResult=C_GET_THE_SPE_STATUT.next( codeId  ) ) )
                  {if (isResult>=2)
                      {if (isResult==2) block += tr("<br/><b><u>Condition de prescription et de d\303\251livrance.</u></b>");
                       block += "<br/>&nbsp; &nbsp; &nbsp;";
                       block += C_GET_THE_SPE_STATUT.INFO_01().replace(">","&gt;").replace("<","&lt;").replace("- D\303\251cision","<br/>- D\303\251cision").replace("- Arr\303\252t\303\251","<br/>- Arr\303\252t\303\251").replace("*","<br/> ")
                                    + " " + C_GET_THE_SPE_STATUT.DATE_01().toString("dd-MM-yyyy")
                                    + " " + C_GET_THE_SPE_STATUT.DATE_02().toString("dd-MM-yyyy");
                      }
                  }
            C_GET_THE_LAB_SPE_CUR  C_GET_THE_LAB_SPE( database() , this );
            while ( C_GET_THE_LAB_SPE.next(codeId ,1  ) )
                  {block += tr("<br/><b><u>\303\251tablissement d\303\251tenteur de l'AMM</u> : </b>%1") .arg(C_GET_THE_LAB_SPE.NomLab()) +  tr("  date : %1") .arg(C_GET_THE_LAB_SPE.DateRef().toString("dd-MM-yyyy"));;
                  }
            while ( C_GET_THE_LAB_SPE.next(codeId ,2  ) )
                  {block += tr("<br/><b><u>\303\251tablissement exploitant</u> : </b>%1") .arg(C_GET_THE_LAB_SPE.NomLab()) +  tr("  date : %1") .arg(C_GET_THE_LAB_SPE.DateRef().toString("dd-MM-yyyy"));;
                  }
            C_GET_THE_URL_CUR  C_GET_THE_URL ( database() , this );
            while ( C_GET_THE_URL.next( codeId ) )
                  { block += tr("<br/><b><u>Liens internet</u> : </b>") + insertHtmlLink(C_GET_THE_URL.URL(),C_GET_THE_URL.URL());
                  };
            //........... tarif ..................................
            C_GET_PRIX_FROM_CIP_CUR  C_GET_PRIX_FROM_CIP ( database(), this );
            while ( C_GET_PRIX_FROM_CIP.next( cip ) )
                  { block += tr("<br/><b><u>Prix TTC</u> : </b>  ") + C_GET_PRIX_FROM_CIP.PRIX() + " EUROS en date du " + C_GET_PRIX_FROM_CIP.DATE("dd-MM-yyyy");
                  }

            C_GET_FORFAIT_PRE_SPE_CUR  C_GET_FORFAIT_PRE_SPE ( database(), this );
            while ( C_GET_FORFAIT_PRE_SPE.next( cip, 0) )
                  {  block += tr("<br/><b><u>Tarif forfaitaire de remboursement hospitalier(0)</u> : </b>") + C_GET_FORFAIT_PRE_SPE.TARIF() +tr(" Euros en date du : ") + C_GET_FORFAIT_PRE_SPE.DATE().toString("dd-MM-yyyy");
                  }
            while ( C_GET_FORFAIT_PRE_SPE.next( cip, 1) )
                  {  block += tr("<br/><b><u>Tarif forfaitaire de remboursement hospitalier(1)</u> : </b>") + C_GET_FORFAIT_PRE_SPE.TARIF() +tr(" Euros en date du : ") + C_GET_FORFAIT_PRE_SPE.DATE().toString("dd-MM-yyyy");
                  }
            //..................... plan de gestion des risques ......................................
            C_GET_MY_PGR_CUR  C_GET_MY_PGR ( dbi_database()->database(), this );
            while ( C_GET_MY_PGR.next(  C_GET_THE_SPECIALITE.SP_NOM(), s_subAct_1) )
                  { block += tr("<br/><b><u>Fait l'objet d'un plan de gestion des risques</u> : </b>")+ tr(" en date du : ") + C_GET_MY_PGR.DATE("dd-MM-yyyy") + "<br/>"
                          + insertHtmlLink(C_GET_MY_PGR.URL(), QString("PgrUrl=") + C_GET_MY_PGR.URL());
                  }
            html += tr("<p class=nom_chapitres>RENSEIGNEMENTS ADMINISTRATIFS </p>%1") .arg(block);

            //.......................... DDD et voie....................................
            block = "";
            C_GET_THE_ATC_DDD_CUR  C_GET_THE_ATC_DDD ( database() , this );
            while ( (isResult=C_GET_THE_ATC_DDD.next( C_GET_THE_SPECIALITE.SP_CATC_CODE_FK(), 1)) )
                  { if (isResult==1) block += tr("<b><u>Dose journali\303\250re conseill\303\251e pour la classe ATC</u> : (%1)<br/></b>").arg(C_GET_THE_SPECIALITE.SP_CATC_CODE_FK());
                    block += trUtf8("<b>&nbsp; &nbsp; &nbsp;</b>  ")            + //C_GET_THE_ATC_DDD.CATC()    +
                                                              trUtf8(" voie   : ")      + C_GET_THE_ATC_DDD.NOMVOIE() +
                                                              trUtf8(" ")               + C_GET_THE_ATC_DDD.DOSAGE()  +
                                                              trUtf8(" %1 par jour").arg( C_GET_THE_ATC_DDD.NOMUNITE() );

                  };
            while ( (isResult=C_GET_THE_ATC_DDD.next( QString::number(C_GET_THE_SPECIALITE.SP_CODE_SQ_PK()), 2)) )
                 { if (isResult==1) block += tr("<br/><b><u>Dose journali\303\250re conseill\303\251e pour cette sp\303\251cialit\303\251.<br/></b></u>");
                   block += trUtf8("<b>&nbsp; &nbsp; &nbsp;</b>  ")             + //C_GET_THE_ATC_DDD.CATC()    +
                                                              trUtf8(" voie   : ")      + C_GET_THE_ATC_DDD.NOMVOIE() +
                                                              trUtf8(" ")               + C_GET_THE_ATC_DDD.DOSAGE()  +
                                                              trUtf8(" %1 par jour").arg( C_GET_THE_ATC_DDD.NOMUNITE() );
                 };
            if (block.length()) html  += tr("<p class=nom_chapitres> VOIE D'ADMINISTRATION DOSE CONSEILLEE </p> %1") .arg(block);

            //............................ INDICATIONS ...............................
            block = "";
            C_GET_THE_IND_SPE_CUR  C_GET_THE_IND_SPE( database() , this );
            while ( C_GET_THE_IND_SPE.next( codeId  ))
                  {str   = "";
                   C_GET_THE_SMR_SPE_CUR  C_GET_THE_SMR_SPE( database() , this );
                   while ( C_GET_THE_SMR_SPE.next( C_GET_THE_IND_SPE.CODEIND(), C_GET_THE_SPECIALITE.SP_CODE_SQ_PK() ))
                         {str += C_GET_THE_SMR_SPE.TEXTSMR();
                         }
                   tmp         = "";
                   QString txt = "";
                   C_GET_THE_DET_IND_CUR  C_GET_THE_DET_IND ( database(), this );
                   while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 5  ) )      // 5 libelle CIM 10
                         { if (C_GET_THE_DET_IND.CODEFILS() != "..") tmp += C_GET_THE_DET_IND.LIBLONG()+" (" + C_GET_THE_DET_IND.CODEFILS() + ")<br/>";
                         }
                   while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 1  ) )       // 1 EN RELAIS DE LA VOIE PARENTERALE 2 DANS LES FORMES AIGUES 4"" 6""  8 AMM/IDIQUE       9 les indication tienne comte ...pareil que C_GET_THE_IND_SPE.FIN_TEXT()
                         { txt +=  C_GET_THE_DET_IND.LIBCOURT()+"<br/>";
                         }
                   while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 2  ) )       // 1 EN RELAIS DE LA VOIE PARENTERALE 2 DANS LES FORMES AIGUES 4"" 6""  8 AMM/IDIQUE       9 les indication tienne comte ...pareil que C_GET_THE_IND_SPE.FIN_TEXT()
                         { txt +=  C_GET_THE_DET_IND.LIBCOURT()+"<br/>";
                         }
                   if (tmp.length()==0)
                      {while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 7  ) )   // 7 libelle NON CIM 10
                          { tmp += C_GET_THE_DET_IND.LIBCOURT() + "<br/>";
                          }
                      }
                   txt   += "\n"+C_GET_THE_IND_SPE.FIN_TEXT().replace(">","&gt;").replace("<","&lt;") .replace("- ","<br/> ");
                   block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                                              "",
                                                                              tmp,                                        // code cim 10 si il exite
                                                                              str.length()?"NIVEAU SMR <br/>":"",
                                                                              str.remove("NIVEAU "),                                        // smr
                                                                              "",
                                                                              path,   //R\303\251f. : Journal Officiel du
                                                                              txt ,
                                                                              javaS_CreatePanel_Body,
                                                                              "panel_EffeNotoireCss");
                  }
            if (block.length()) html  += tr("<p class=nom_chapitres> INDICATIONS </p> %1") .arg(block);
            //........................... GROSSESSE ALLAITEMENT PROCREATION ....................
            makeGrossesseAllaitementProcreationBlock(html, codeId,indexCollapsiblePanel, path, javaS_CreatePanel_Body);

            //........................... CONTRE-INDICATIONS ...................................
            makeCIBlock(html, s_codeId, "'C'",indexCollapsiblePanel,path,javaS_CreatePanel_Body);
            makeCIBlock(html, s_codeId, "'P'",indexCollapsiblePanel,path,javaS_CreatePanel_Body);
            makeCIBlock(html, s_codeId, "'I'",indexCollapsiblePanel,path,javaS_CreatePanel_Body);
            makeCIBlock(html, s_codeId, "'N'",indexCollapsiblePanel,path,javaS_CreatePanel_Body);

            //........................... CONTRE-INDICATIONS AFFSAPS...................................
            block = "";
            C_GET_THE_REF_CIPEMG_RCP_CUR  C_GET_THE_REF_CIPEMG_RCP ( database(), this );     // extraire les CI RCP afssaps
            while ( C_GET_THE_REF_CIPEMG_RCP.next( s_codeId, 1))
                  { block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                           C_GET_THE_REF_CIPEMG_RCP.REF()+" "+C_GET_THE_REF_CIPEMG_RCP.DAT_REF().toString("dd-MM-yyyy"),
                                                           C_GET_THE_REF_CIPEMG_RCP.REF_TXT().left(60)+"..." ,                                       // code cim 10 si il exite
                                                           "",
                                                           tr("Commentaire RCP"),
                                                           "",
                                                           path,
                                                           C_GET_THE_REF_CIPEMG_RCP.REF_TXT().replace(">","&gt;").replace("<","&lt;") ,
                                                           javaS_CreatePanel_Body,
                                                           "panel_EffeNotoireCss");
                  }
            while ( C_GET_THE_REF_CIPEMG_RCP.next(s_codeId, 2))
                  { block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                           C_GET_THE_REF_CIPEMG_RCP.REF()+" "+C_GET_THE_REF_CIPEMG_RCP.DAT_REF().toString("dd-MM-yyyy"),
                                                           C_GET_THE_REF_CIPEMG_RCP.REF_TXT().left(60)+"..." ,                                       // code cim 10 si il exite
                                                           "",
                                                           tr("Commentaire RCP"),
                                                           "",
                                                           path,
                                                           C_GET_THE_REF_CIPEMG_RCP.REF_TXT().replace(">","&gt;").replace("<","&lt;") ,
                                                           javaS_CreatePanel_Body,
                                                           "panel_EffeNotoireCss");
                   }
            if (block.length()) html  += tr("<p class=nom_chapitres> COMMENTAIRES RCP (CONTRE-INDICATIONS, PRECAUTIONS) </p> %1") .arg(block);
            // ............................. SECURITE PRECLINIQUE ............................................................................................................
            makeInteractionBlock          (html, codeId,    indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makeSecuritePrecliniqueBlock  (html, codeId,    indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makeCinetiqueBlock            (html, codeId,    indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makePharmacoDynamiqueBlock    (html, codeId,    indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makeEffetConduiteBlock        (html, s_codeId,  indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makeConservationBlock         (html, cip,       indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makeUtilReconstAdminBlock     (html, codeId,    indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makePosologiesBlock           (html, s_codeId,  indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
            makeFichEtRefOfficiellesBlock (html, s_codeId,  indexCollapsiblePanel ,  path , javaS_CreatePanel_Body);
            makeEffetsIndesirablesBlock   (html, codeId,    indexCollapsiblePanel,   path,  javaS_CreatePanel_Body);
           };
  html += javaS_CreatePanel_Body +
          "//-->\n"
          "</script>\n"
     "    </body>\n"
     "<html>\n";
  // CGestIni::Param_UpdateToDisk("/home/ro/mono.html",html);
  //........................ on insere dans le cache .................................
  C_SET_MY_MONO_INCACHE_CUR  C_SET_MY_MONO_INCACHE ( dbi_database()->database(), this );
  C_SET_MY_MONO_INCACHE.setData(cip, html, 900000, owner(), lang());
 return html;
}
//------------------------------------ makeFichEtRefOfficiellesBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeFichEtRefOfficiellesBlock(QString &html, const QString &s_codeId,
                                                                   int &indexCollapsiblePanel , const QString & path , QString &javaS_CreatePanel_Body)
{
    QString block        = "";
    int  isResult        = 0;
    double lastNumFiche  = -1;

    C_GET_THE_CHOIX_CUR    C_GET_THE_CHOIX   ( database() , this );
    C_GET_THE_DOC_SPE_CUR  C_GET_THE_DOC_SPE ( database() , this );
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 1 ) )  // 'A_TRANS' avis commission de transpareence HAUTE AUTORITE DE SANTE COMMISSION DE LA TRANSPARENCE AVIS DE LA COMMISSION
          { if (lastNumFiche==C_GET_THE_CHOIX.NU_FICHE()) continue;
            lastNumFiche = C_GET_THE_CHOIX.NU_FICHE();
            while ( C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 1 ) )
                  { if (isResult==0) block += tr("<br/><b><u>Avis des commissions de transparence.</u><br/></b>");
                    block += addCollapsibleJSPanel      ( indexCollapsiblePanel++,
                                                          C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                          C_GET_THE_CHOIX.PRECISION1(),
                                                          C_GET_THE_CHOIX.TITRE(),
                                                          "",                                        // cim 10
                                                          tr("avis commission de transparence"),
                                                          path,
                                                          C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                          javaS_CreatePanel_Body );
                    ++isResult;
                  };
          };
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 2 ) )  // 'F_TRANS' fiches de transparence
          { while (C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 2 ) )
              { if (isResult==0) block += tr("<br/><b><u>Fiches de transparence.<br/></b></u>");
                block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                      C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                      C_GET_THE_CHOIX.PRECISION1(),
                                                      C_GET_THE_CHOIX.TITRE(),
                                                      "",                                        // cim 10
                                                      tr("Fiche de transparence"),
                                                      path,
                                                      C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                      javaS_CreatePanel_Body );
                ++isResult;
              };
          };
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 3 ) )  // 'AFS_ANA' AGENCE FRANÇAISE DE SECURITE SANITAIRE DES PRODUITS DE SANTE et conf consensus RECOMMANDATIONS DE BONNE PRATIQUE PREVENTION
          { while (C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 3 ) )
              { if (isResult==0) block += tr("<br/><b><u>Recommandations et protocoles th\303\251rapeutiques Afssaps, HAS, INCa<br/></b></u>");
                block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                      C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                      C_GET_THE_CHOIX.PRECISION1(),
                                                      C_GET_THE_CHOIX.TITRE(),
                                                      "",                                        // cim 10
                                                      tr("Recommandations et protocoles th\303\251rapeutiques Afssaps, HAS, INCa"),
                                                      path,
                                                      C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                      javaS_CreatePanel_Body );
                ++isResult;
              };
          };
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 4 ) )  // 'RAPPE'  RAPPORT PUBLIC D'EVALUATION LOVENOX
          { while (C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 4 ) )
              { if (isResult==0) block += tr("<br/><b><u>Rapports publics d'\303\251valuation.<br/></b></u>");
                block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                      C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                      C_GET_THE_CHOIX.PRECISION1(),
                                                      C_GET_THE_CHOIX.TITRE(),
                                                      "",                                        // cim 10
                                                      tr("Rapport public d'\303\251valuation"),
                                                      path,
                                                      C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                      javaS_CreatePanel_Body );
                ++isResult;
              };
          };
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 5 ) )  // C_PAT' CONSEILS/EDUCATION SANITAIRE CONSEILS au aptient
          { while ( C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 5 ) )
              { if (isResult==0) block += tr("<br/><b><u>Conseils au patient, \303\251ducation sanitaire.<br/></b></u>");
                block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                      C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                      C_GET_THE_CHOIX.PRECISION1(),
                                                      C_GET_THE_CHOIX.TITRE(),
                                                      "",                                        // cim 10
                                                      tr("Conseils au patient, \303\251ducation sanitaire"),
                                                      path,
                                                      C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                      javaS_CreatePanel_Body );
                ++isResult;
              };
          };
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 6 ) )  // 'F_TER'
          { while ( C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 6 ) )
              { if (isResult==0) block += tr("<br/><b><u>Fiches terrain.<br/></b></u>");
                block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                      C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                      C_GET_THE_CHOIX.PRECISION1(),
                                                      C_GET_THE_CHOIX.TITRE(),
                                                      "",                                        // cim 10
                                                      tr("Fiche terrain."),
                                                      path,
                                                      C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                      javaS_CreatePanel_Body );
                ++isResult;
              };
          };
    isResult = 0;
    while (C_GET_THE_CHOIX.next( s_codeId , 7 ) )  // 'RMO'     PRESCRIPTION DES ANTIBIOTIQUES EN PRATIQUE COURANTE
          { while (C_GET_THE_DOC_SPE.next( C_GET_THE_CHOIX.NU_FICHE() , 7 ) )
                  { if (isResult==0) block += tr("<br/><b><u>Fiches RMO et de bonnes pratiques.<br/></b></u>");
                    block += addCollapsibleJSPanel   ( indexCollapsiblePanel++,
                                                      C_GET_THE_CHOIX.DATE_TEXTE().toString("dd-MM-yyyy"),
                                                      C_GET_THE_CHOIX.PRECISION1(),
                                                      C_GET_THE_CHOIX.TITRE(),
                                                      "",                                        // cim 10
                                                      tr("Fiche RMO et de bonne pratique"),
                                                      path,
                                                      C_GET_THE_DOC_SPE.TEXTE().replace(">","&gt;").replace("<","&lt;").replace(" -","<br/> "),
                                                      javaS_CreatePanel_Body );
                    ++isResult;
                  };
          };
    if (block.length()) html  += tr("<p class=nom_chapitres> FICHES ET REFERENCES OFFICIELLES </p> %1") .arg(block);
    return block;
}
//------------------------------------ makePosologiesBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makePosologiesBlock(QString &html, const QString &s_codeId,
                                                          int &indexCollapsiblePanel , const QString & path , QString &javaS_CreatePanel_Body)
{
    QString block   = "";
    QString str     = "";
    C_GET_THE_POSO_CUR  C_GET_THE_POSO ( database(), this );
    /*
     //+         -- 0 fiche posologie
     //+         -- 1 voies d'administration
     //+         -- 2 Terrain
     //+         -- 3 nature indication
     //+         -- 4 complement nature indication
     //+         -- 5 r\303\251f\303\251rence officielle
      QString MY_NOFIC()   {return m_MY_NOFIC;}
      int     TYP()        {return m_TYP;}
      QString MY_INFO_01() {return m_MY_INFO_01;}
      QString VALEUR_01()  {return m_VALEUR_01;}
      QString VALEUR_02()  {return m_VALEUR_02;}
      QDate   DATE_01()                         {return m_DATE_01;}
      QString DATE_01(const QString &format)    {return m_DATE_01.toString(format);}
     */
    C_GET_THE_DET_POSO_SPE_CUR  C_GET_THE_DET_POSO_SPE ( database(), this );

    int       addTerr    = 0;
    int       type       = 0;
    QString numFic       = "";
    QString voieAd       = "";
    QString terrain      = "";
    QString natIndic     = "";
    QString cpmIndic     = "";
    QString grp          = "";
    QString min          = "";
    QString max          = "";
    QString result       = "";
    QString cplNat       = "";
    int     cur_grp      = -1;
    int     last_grp     = -1;
    int     typ_grp      = -1;
    int     sub_grp      = -1;
    while ( C_GET_THE_POSO.next(  s_codeId  ) )
          { type = C_GET_THE_POSO.TYP();
            switch(type)
            { case 0: numFic    = C_GET_THE_POSO.MY_NOFIC();                                   break;
              case 1: voieAd    = C_GET_THE_POSO.MY_INFO_01();                                 break;
              case 2: terrain  += C_GET_THE_POSO.MY_INFO_01()+", ";                            break;
              case 3: {natIndic += "\302\272&nbsp;&nbsp;&nbsp;"+C_GET_THE_POSO.MY_INFO_01()+"<br/>";
                      if (addTerr==0)
                         {terrain += " <br/>(";
                          ++addTerr;
                         }
                      terrain  += C_GET_THE_POSO.MY_INFO_01().toLower() +", ";
                      }break;
              case 4: cpmIndic += "\302\272&nbsp;&nbsp;&nbsp;"+C_GET_THE_POSO.MY_INFO_01()+"<br/>";    break;
            }
            if (type==5)
               {if (terrain.length() && addTerr) terrain += ")";
                if (natIndic.length())
                   {str += tr("<h2 class=titre_resume>Indication(s)</h2>%1 ").arg( natIndic + "<br/>" + cpmIndic );
                   }
                while ( C_GET_THE_DET_POSO_SPE.next(numFic , 2  ) )
                      {
                        grp     = C_GET_THE_DET_POSO_SPE.GRP();
                        min     = C_GET_THE_DET_POSO_SPE.MINI();
                        max     = C_GET_THE_DET_POSO_SPE.MAXI();

                        str += tr("<br/><b> Num fic : </b>  ") + numFic;
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.GRP() : </b>  ")     + grp;
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.NATURE() : </b>  ")  + C_GET_THE_DET_POSO_SPE.NATURE();
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.INFO_01() : </b>  ") + C_GET_THE_DET_POSO_SPE.INFO_01();
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.MINI() : </b>  ")    + C_GET_THE_DET_POSO_SPE.MINI();
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.INFO_02() : </b>  ") + C_GET_THE_DET_POSO_SPE.INFO_02();
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.MAXI() : </b>  ")    + C_GET_THE_DET_POSO_SPE.MAXI();
                        str += tr("<br/><b> C_GET_THE_DET_POSO_SPE.INFO_03() : </b>  ") + C_GET_THE_DET_POSO_SPE.INFO_03();

                        //                                                       ____ grp[0]
                        //                                                      |     ___ typ_grp
                        //                                                      |    |  ___ sub_grp
                        //                                                      |    | |
                        //--- complement nature de dose                         1.N\302\272.0.IPOCOTE_NUM_ORD_PK
                        //--- minimum et maximum de la posologie                1.N\302\272.1.0
                        //--- frequence minimum et maximum                      1.N\302\272.2.0
                        //--- commentaire frequence                             1.N\302\272.2.1
                        //--- dur\303\251e minimum et dur\303\251e maximum                    1.N\302\272.2.2
                        //--- commentaire dur\303\251e                                 1.N\302\272.3.IPOCOD_NUM_ORD_PK
                        //--- Commentaire Adaptation                            2.0.1.FPOCOAD_NUM_ORD_PK
                        //--- Commentaire surveillance                          2.1.1.FPOCOSU_NUM_ORD_PK
                        //--- Commentaire recommandation                        2.2.1.FPOCOAU_NUM_ORD_PK

                        if (grp[0]=='1')
                           { cur_grp = grp.toLatin1()[2];
                             typ_grp = grp.toLatin1()[4];
                             sub_grp = grp.toLatin1()[6];
                             if (typ_grp=='0')          // --- complement posologie (nature de dose)
                                {cplNat  = C_GET_THE_DET_POSO_SPE.INFO_01();
                                }
                             if (typ_grp=='1')          // --- minimum et maximum de la posologie
                                {result += tr("<h2 class=titre_resume>Posologie %1</h2>").arg( C_GET_THE_DET_POSO_SPE.NATURE()+" "+cplNat );
                                 if (min.length() && max.length()) result += " de " + min + tr(" \303\240 ") + max + " " + C_GET_THE_DET_POSO_SPE.INFO_02();
                                 else if (min.length())            result += min + " " + C_GET_THE_DET_POSO_SPE.INFO_02();
                                 else if (max.length())            result += max + " " + C_GET_THE_DET_POSO_SPE.INFO_02();
                                 if (C_GET_THE_DET_POSO_SPE.INFO_01().length()) result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();
                                }
                             if (typ_grp=='2')          // --- frequence
                                {
                                 if (sub_grp=='0')      // --- frequence minimum et maximum
                                    { result += tr("<h2 class=titre_resume>Fr\303\251quence</h2> %1").arg( C_GET_THE_DET_POSO_SPE.NATURE() );
                                      if (min.length() && max.length()) result += " de " + min + " " + C_GET_THE_DET_POSO_SPE.INFO_02() + tr(" \303\240 ") + max + " " + C_GET_THE_DET_POSO_SPE.INFO_03() + " ";
                                      else if (min.length())            result += min + " " + C_GET_THE_DET_POSO_SPE.INFO_02() + " ";
                                      else if (max.length())            result += max + " " + C_GET_THE_DET_POSO_SPE.INFO_03() + " ";
                                      if (C_GET_THE_DET_POSO_SPE.INFO_01().length()) result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();
                                    }
                                 if (sub_grp=='1' && C_GET_THE_DET_POSO_SPE.INFO_01().length())      // --- commentaire frequence
                                    { result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();
                                    }
                                 if (sub_grp=='2')      // --- dur\303\251e minimum et dur\303\251e maximum        Dur\303\251e de traitement max
                                    { result += tr("<h2 class=titre_resume>Dur\303\251e de traitement</h2> %1").arg( C_GET_THE_DET_POSO_SPE.NATURE() );
                                      if (min.length() && max.length()) result += " de " + min + " " + C_GET_THE_DET_POSO_SPE.INFO_02() + tr(" \303\240 ") + max + " " + C_GET_THE_DET_POSO_SPE.INFO_03() + " ";
                                      else if (min.length())            result += min + " " + C_GET_THE_DET_POSO_SPE.INFO_02() + " ";
                                      else if (max.length())            result += max + " " + C_GET_THE_DET_POSO_SPE.INFO_03() + " ";
                                      if (C_GET_THE_DET_POSO_SPE.INFO_01().length()) result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();
                                    }
                                }
                             if (typ_grp=='3')          // --- commentaire dur\303\251e
                                {result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();
                                }
                             last_grp = cur_grp;
                           }
                        if (grp[0]=='2')
                           { result += "<br/>" + C_GET_THE_DET_POSO_SPE.INFO_01();
                           }
                      } // end while ( C_GET_THE_DET_POSO_SPE.next(numFic , 2  ) )
                //............. texte additionnel ...................................
                C_GET_THE_POSO_TEXT_CUR  C_GET_THE_POSO_TEXT ( database(), this );
                while ( C_GET_THE_POSO_TEXT.next( numFic  ) )
                      { result += tr("<h2 class=titre_resume>Remarque</h2>") + C_GET_THE_POSO_TEXT.TEXTE().replace("* ","<br/> * ");
                      }
                //............ on insere une ligne de posologie .......................
                if (str.length())
                   { str+= result;
                     block += addCollapsibleJSPanel       (              indexCollapsiblePanel++,
                                                                         C_GET_THE_POSO.DATE_01("dd-MM-yyyy"),
                                                                         C_GET_THE_POSO.MY_INFO_01(),
                                                                         terrain,
                                                                         voieAd.prepend("<br/> Voie : "),
                                                                         "",
                                                                         path,
                                                                         str,
                                                                         javaS_CreatePanel_Body,
                                                                         "panel_EffeNotoireCss");
                   }
                numFic   = "";
                str      = "";
                result   = "";
                terrain  = "";
                natIndic = "";
                cpmIndic = "";
                cplNat   = "";
                addTerr  = 0;
               }
          }
    if (block.length()) html  += tr("<p class=nom_chapitres> POSOLOGIES </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeEffetsIndesirablesBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeEffetsIndesirablesBlock(QString &html, double codeId,
                                                 int &indexCollapsiblePanel , const QString & path , QString &javaS_CreatePanel_Body)
{
    QString block   = "";
    block           =   makeEffetsIndesirables_subBlock( codeId, 1,  indexCollapsiblePanel, path,javaS_CreatePanel_Body);
    if (block.length()) html  += tr("<p class=nom_chapitres>EFFETS IND\303\251SIRABLES \303\240 DOSES TH\303\251RAPEUTIQUES (Cliniques)</p> %1") .arg(block);
    block           =   makeEffetsIndesirables_subBlock( codeId, 2,  indexCollapsiblePanel, path,javaS_CreatePanel_Body);
    if (block.length()) html  += tr("<p class=nom_chapitres>EFFETS IND\303\251SIRABLES \303\240 DOSES TH\303\251RAPEUTIQUES (Paracliniques)</p> %1") .arg(block);
    block           =   makeEffetsIndesirablesSurdosage_subBlock( codeId, 3,  indexCollapsiblePanel, path,javaS_CreatePanel_Body);
    if (block.length()) html  += tr("<p class=nom_chapitres>EFFETS IND\303\251SIRABLES EN CAS DE SURDOSAGE (Cliniques)</p> %1") .arg(block);
    block           =   makeEffetsIndesirablesSurdosage_subBlock( codeId, 4,  indexCollapsiblePanel, path,javaS_CreatePanel_Body);
    if (block.length()) html  += tr("<p class=nom_chapitres>EFFETS IND\303\251SIRABLES EN CAS DE SURDOSAGE (Paracliniques)</p> %1") .arg(block);

    return block;
}
//------------------------------------ makeEffetsIndesirables_subBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeEffetsIndesirables_subBlock(double codeId, int type,
                                                                              int &indexCollapsiblePanel , const QString & path , QString &javaS_CreatePanel_Body)
{   int isResult         = 0;
    QString refOff       = "";
    QString date         = "";
    QString block        = "";
    QString str          = "";
    QString terrFav      = "";
    QString circFav      = "";
    QString momSurv      = "";
    QString fceText      = "";
    QString survText     = "";
    QString chimClass    = "";
    QString pharClass    = "";
    QString natEiClin    = "";
    QString natParaClin  = "";
    QString commNat      = "";
    QString catLibelle   = "";
    QString commDoseThe  = "";
    QString natuTxt      = "";
    QString cat1         = "";
    QString cat2         = "";

    C_GET_THE_EFFIND_SPE_CUR      C_GET_THE_EFFIND_SPE ( database(), this );
    C_GET_THE_DET_EFFIND_CUR      C_GET_THE_DET_EFFIND ( database(), this );
    /*
      double  CODEEFFIND() {return m_CODEEFFIND;}
      QString TEXTEFFET()  {return m_TEXTEFFET;}
    */
    /*
    if ( TypId == 1) {            // -- terrain favorisant
    } else if (  TypId == 2) {    // -- autres circonstances favorisantes
    } else if (  TypId == 3) {    // -- moment survenue
    } else if (  TypId == 4) {    // -- texte fr\303\251quence
    } else if (  TypId == 5) {    // -- texte survenue
    } else if (  TypId == 6) {    // -- liste classe chimique
    } else if (  TypId == 7) {    // -- texte classe pharmaco
    } else if (  TypId == 8) {    // -- nature(s) des EI cliniques
    } else if (  TypId == 9) {    // -- nature paraclinique
    } else if (  TypId == 10) {   // -- commentaire nature
    } else if (  TypId == 11) {   // -- conduite \303\240 tenir
    } else if (  TypId == 12) {   // -- commentaire dose th\303\251rapeutique
    } else if (  TypId == 13) {   // -- texte Nature(s) EI
    } else if (  TypId == 14) {   // -- texte conduite \303\240 tenir
    } else if (  TypId == 15) {   // -- texte conduite \303\240 tenir

    QString CODEIND();
    QString LIBIND();
    */

    while ( C_GET_THE_EFFIND_SPE.next( codeId , type  ) )       //   1/clinique   2/paraclinique 3/
          { terrFav      = "";
            circFav      = "";
            momSurv      = "";
            fceText      = "";
            survText     = "";
            chimClass    = "";
            pharClass    = "";
            natEiClin    = "";
            natParaClin  = "";
            commNat      = "";
            catLibelle   = "";
            commDoseThe  = "";
            natuTxt      = "";
            cat1         = "";
            cat2         = "";
            str          = "";
            // str += "<hr><br/><b><u>"+C_GET_THE_EFFIND_SPE.TEXTEFFET()+"</b></u><br/>";
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 1  )) )
                  { // str += tr("<br/><b>Terrain favorisant : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    terrFav += C_GET_THE_DET_EFFIND.LIBIND();  terrFav += ",   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 2  )) )
                  { // str += tr("<br/><b>Autres circonstances favorisantes : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    circFav += C_GET_THE_DET_EFFIND.LIBIND();  circFav += "   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 3  )) )
                  { // str += tr("<br/><b>Moment de survenue : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    momSurv += C_GET_THE_DET_EFFIND.LIBIND();  momSurv += " ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 4  )) )
                  { // str += tr("<br/><b>Texte fr\303\251quence : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    fceText  += C_GET_THE_DET_EFFIND.LIBIND();  fceText += "   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 5 )) )
                  { // str += tr("<br/><b>texte survenue : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    survText += C_GET_THE_DET_EFFIND.LIBIND(); survText += "   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 6  )) )
                  { // str += tr("<br/><b>liste classe chimique : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    chimClass += C_GET_THE_DET_EFFIND.LIBIND(); chimClass += "   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 7  )) )
                  { // str += tr("<br/><b>texte classe pharmaco : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    pharClass += C_GET_THE_DET_EFFIND.LIBIND(); pharClass += "   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 8  )) )
                  { // str += tr("<br/><b>nature(s) des EI cliniques : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    natEiClin  += C_GET_THE_DET_EFFIND.LIBIND();  natEiClin += "   ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 9  )) )
                  { // str += tr("<br/><b>nature paraclinique : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    natParaClin +=  C_GET_THE_DET_EFFIND.LIBIND();  natParaClin += "  ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 10  )) )
                  { //str += tr("<br/><b>commentaire nature : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    commNat  +=  C_GET_THE_DET_EFFIND.LIBIND();  commNat += "  ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 11  )) )
                  { // str += tr("<br/><b>conduite \303\240 tenir : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    catLibelle +=  C_GET_THE_DET_EFFIND.LIBIND();  catLibelle += "  ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 12  )) )
                  { // str += tr("<br/><b>commentaire dose th\303\251rapeutique : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    commDoseThe +=  C_GET_THE_DET_EFFIND.LIBIND();  commDoseThe += "  ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 13  )) )
                  { // str += tr("<br/><b>texte Nature(s) EI : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                    natuTxt +=  C_GET_THE_DET_EFFIND.LIBIND();  natuTxt += "  ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 14  )) )
                  { // str += tr("<br/><b>texte conduite \303\240 tenir : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                     cat1 +=  C_GET_THE_DET_EFFIND.LIBIND();  cat1 += "  ";
                  }
            while ( (isResult = C_GET_THE_DET_EFFIND.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 15  )) )
                  { //str += tr("<br/><b>texte conduite \303\240 tenir : </b>  ") + C_GET_THE_DET_EFFIND.LIBIND();
                     cat2 +=  C_GET_THE_DET_EFFIND.LIBIND();   cat2 += "  ";
                  }
            if (terrFav.length())     str += tr("<br/><b>Terrain favorisant : </b>  ")                + terrFav;
            if (circFav.length())     str += tr("<br/><b>Autres circonstances favorisantes : </b>  ") + circFav;
            if (momSurv.length())     str += tr("<br/><b>Moment de survenue : </b>  ")                + momSurv;
            if (fceText.length())     str += tr("<br/><b>Commentaire fr\303\251quence : </b>  ")             + fceText;
            if (survText.length())    str += tr("<br/><b>Commentaire survenue : </b>  ")              + survText;
            if (chimClass.length())   str += tr("<br/><b>Classe chimique : </b>  ")                   + chimClass;
            if (pharClass.length())   str += tr("<br/><b>Classe pharmacologique : </b>  ")            + pharClass;
            if (natEiClin.length()   && type==1) str += tr("<br/><b>Nature des effets cliniques : </b>  ")       + natEiClin;
            if (natParaClin.length() && type==2) str += tr("<br/><b>Nature des effets paracliniques : </b>  ")   + natParaClin;
            //if (commNat.length())     str += tr("<br/><b>Commentaire nature : </b>  ")                + commNat;
            if (catLibelle.length())  str += tr("<br/><b>Conduite \303\240 tenir : </b>  ")                  + catLibelle;
            if (commDoseThe.length()) str += tr("<br/><b>Commentaire dose th\303\251rapeutique : </b>  ")    + commDoseThe;
            if (natuTxt.length())     str += tr("<br/><b>Commentaire effets ind\303\251sirables : </b>  ")   + natuTxt;
            if (cat1.length())        str += tr("<br/><b>Commentaire conduite \303\240 tenir 1 : </b>  ")    + cat1;
            if (cat2.length())        str += tr("<br/><b>Commentaire conduite \303\240 tenir 2 : </b>  ")    + cat2;
            if (str.length())
               { block += addCollapsibleJSPanel       (             indexCollapsiblePanel++,
                                                                    date,
                                                                    refOff,                           // reference officielle
                                                                    C_GET_THE_EFFIND_SPE.TEXTEFFET(),      //
                                                                    commNat,                            // Passage transplacentaire                                                                   "",
                                                                    "",
                                                                    path,
                                                                    str,
                                                                    javaS_CreatePanel_Body,
                                                                    "panel_EffeNotoireCss");
               }
          }
   return block;
}
//------------------------------------ makeEffetsIndesirablesSurdosage_subBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeEffetsIndesirablesSurdosage_subBlock(double codeId, int type,
                                                 int &indexCollapsiblePanel , const QString & path , QString &javaS_CreatePanel_Body)
{   int isResult         = 0;
    QString refOff       = "";
    QString date         = "";
    QString block        = "";
    QString str          = "";

    C_GET_THE_EFFIND_SPE_CUR           C_GET_THE_EFFIND_SPE          ( database(), this );
    C_GET_DETAIL_EFFIND_SURDOSAGE_CUR  C_GET_DETAIL_EFFIND_SURDOSAGE ( database(), this );
    /*
      double  CODEEFFIND() {return m_CODEEFFIND;}
      QString TEXTEFFET()  {return m_TEXTEFFET;}
    */
    /*
     // 1 -- classe chimique
     // 2 -- classe pharmacotherapeutique
     // 3 -- nature du surdosage clinique
     // 4 -- nature du surdosage paraclinique
     // 5 -- commentaire nature du surdosage
     // 6 -- conduite \303\240 tenir nature du surdosage
     // 7 -- texte nature du surdosage
     // 8 -- texte conduite \303\240 tenir

     double  NOFIC();
     QString GRP();
     QString INFO_01();
     QString TXT();
    */

    while ( C_GET_THE_EFFIND_SPE.next( codeId , type  ) )       //   1/clinique   2/paraclinique 3/
          {
            QString chimClass    = "";           // 1
            QString pharClass    = "";           // 2
            QString natEiClin    = "";           // 3
            QString natParaClin  = "";           // 4
            QString commSurdo    = "";           // 5
            QString catNatSur    = "";           // 6
            QString natuTxt      = "";           // 7
            QString catText      = "";           // 8

            str          = "";
            // str += "<hr><br/><b><u>"+C_GET_THE_EFFIND_SPE.TEXTEFFET()+"</b></u><br/>";
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 1  )) )
                  { chimClass += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT();  chimClass += ",   ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 2  )) )
                  { pharClass += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT();  pharClass += "   ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 3  )) )
                  { natEiClin += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT();  natEiClin += ",  ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 4  )) )
                  { natParaClin  += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT();  natParaClin += ",  ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 5 )) )
                  { commSurdo += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT(); commSurdo += "   ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 6  )) )
                  { catNatSur += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT(); catNatSur += "   ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 7  )) )
                  { natuTxt += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT(); natuTxt += "   ";
                  }
            while ( (isResult = C_GET_DETAIL_EFFIND_SURDOSAGE.next( C_GET_THE_EFFIND_SPE.CODEEFFIND() , 8  )) )
                  { catText  += C_GET_DETAIL_EFFIND_SURDOSAGE.TXT();  catText += "   ";
                  }
            if (chimClass.length())     str += tr("<br/><b>Classe chimique : </b>  ")                                + chimClass;
            if (pharClass.length())     str += tr("<br/><b>Classe pharmaco th\303\251rapeutique : </b>  ")                  + pharClass;
            if (natEiClin.length()   && type== 3)   str += tr("<br/><b>Nature des effets cliniques : </b>  ")        + natEiClin;
            if (natParaClin.length() && type== 4)   str += tr("<br/><b>Nature des effets paracliniques : </b>  ")    + natParaClin;
            if (commSurdo.length())     str += tr("<br/><b>Commentaire : </b>  ")                                    + commSurdo;
            if (catNatSur.length())     str += tr("<br/><b>Conduite \303\240 tenir selon nature du surdosage: </b>  ")      + catNatSur;
            if (natuTxt.length())       str += tr("<br/><b>Commentaire nature du surdosage : </b>  ")                + natuTxt;
            if (catText.length())       str += tr("<br/><b>Commentaire conduite \303\240 tenir : </b>  ")                   + catText;
            if (str.length())
               { block += addCollapsibleJSPanel       (             indexCollapsiblePanel++,
                                                                    date,
                                                                    refOff,                            // reference officielle
                                                                    C_GET_THE_EFFIND_SPE.TEXTEFFET(),  //
                                                                    commSurdo,                         // Passage transplacentaire                                                                   "",
                                                                    "",
                                                                    path,
                                                                    str,
                                                                    javaS_CreatePanel_Body,
                                                                    "panel_EffeNotoireCss");
               }
          }
   return block;
}
//------------------------------------ makeUtilReconstAdminBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeUtilReconstAdminBlock(QString &html, double codeId,
                                                 int &indexCollapsiblePanel , const QString & path , QString &javaS_CreatePanel_Body)
{   QString block    = "";
    QString titre    = "";
    QString refOff   = "";
    QString date     = "";
    QString str      = "";

    C_GET_THE_RECADM_SPE_CUR  C_GET_THE_RECADM_SPE ( database(), this );
    C_GET_THE_RECADM_ID_CUR   C_GET_THE_RECADM_ID  ( database(), this );
    while ( C_GET_THE_RECADM_SPE.next( codeId  ) )
      { //............................ Mode d'administration ..........................................
        while ( C_GET_THE_RECADM_ID.next( codeId , C_GET_THE_RECADM_SPE.FICID() , 1  ) )
              { titre = C_GET_THE_RECADM_ID.TEXTRECADM();
              }
        while ( C_GET_THE_RECADM_ID.next( codeId , C_GET_THE_RECADM_SPE.FICID() , 2  ) )
              { str   = C_GET_THE_RECADM_ID.TEXTRECADM().replace("* ","<br/><br/> * ").replace(". ",".<br/> ");
              }
        while ( C_GET_THE_RECADM_ID.next( codeId , C_GET_THE_RECADM_SPE.FICID() , 3  ) )
              { refOff = C_GET_THE_RECADM_ID.TEXTRECADM();
                date   = C_GET_THE_RECADM_ID.DTRECADM("dd-MM-yyyy");
              }
        if (str.length())
           { block += addCollapsibleJSPanel       (             indexCollapsiblePanel++,
                                                                date,
                                                                refOff,                           // reference officielle
                                                                tr("Mode d'administration"),      //
                                                                titre,                            // Passage transplacentaire                                                                   "",
                                                                "",
                                                                path,
                                                                str,
                                                                javaS_CreatePanel_Body,
                                                                "panel_EffeNotoireCss");
           }
        //............................ Recommandation utilisation,manipulation et elimination ..........................................
        while ( C_GET_THE_RECADM_ID.next( codeId , C_GET_THE_RECADM_SPE.FICID() , 5  ) )
              { titre = C_GET_THE_RECADM_ID.TEXTRECADM();
              }
        while ( C_GET_THE_RECADM_ID.next( codeId , C_GET_THE_RECADM_SPE.FICID() , 4  ) )
              { str = C_GET_THE_RECADM_ID.TEXTRECADM().replace("* ","<br/><br/> * ").replace(". ",".<br/> ");
              }
        if (str.length())
           { block += addCollapsibleJSPanel       (             indexCollapsiblePanel++,
                                                                date,
                                                                refOff,                          // reference officielle
                                                                tr("Recommandation utilisation,manipulation et \303\251limination"),     //
                                                                titre,                           // Passage transplacentaire
                                                                "",
                                                                path,
                                                                str,
                                                                javaS_CreatePanel_Body,
                                                                "panel_EffeNotoireCss");
           }
      }
if (block.length()) html  += tr("<p class=nom_chapitres> ADMINISTRATION UTILISATION MANIPULATION ELIMINATION</p> %1") .arg(block);
return block;
}
//------------------------------------ makeInteractionBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeInteractionBlock(QString &html, double CodeId,
                                                 int & /* indexCollapsiblePanel */, const QString & /*path */, QString & /*javaS_CreatePanel_Body */)
{   QString block    = "";
    C_GET_THE_INTER_SPE_CUR  C_GET_THE_INTER_SPE ( database(), this );
    while ( C_GET_THE_INTER_SPE.next( CodeId  ) )
          { block +=  C_GET_THE_INTER_SPE.TEXTINTER().replace("* ","<br/><br/> * ").replace(" - ","<br/> - ").replace("+ "," <br/><br/> +") + "<br/>";
            block +=  tr("<b> R\303\251f. officielle : </b>  ") + C_GET_THE_INTER_SPE.REFOFF() + " en date du " + C_GET_THE_INTER_SPE.DTREF("dd-MM-yyyy") + "<br/>";
          }
    if (block.length()) html  += tr("<p class=nom_chapitres> INTERACTIONS </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeConservationBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeConservationBlock(QString &html, QString cip,
                                                 int & /* indexCollapsiblePanel */, const QString & /*path */, QString & /*javaS_CreatePanel_Body */)
{   QString block    = "";
    C_GET_THE_PRE_CSV_CUR  C_GET_THE_PRE_CSV ( database(), this );
    while ( C_GET_THE_PRE_CSV.next( cip , 1  ) )
          { if (C_GET_THE_PRE_CSV.TYPE_PRE_CSV()=="11")
               { block += tr("<b> Dur\303\251e de conservation : </b> %1 <br/>").arg( QString::number(C_GET_THE_PRE_CSV.VALEUR_1()) + " " +C_GET_THE_PRE_CSV.INFO_1() );
               }
            if (C_GET_THE_PRE_CSV.TYPE_PRE_CSV()=="211")
               { block += tr("<b> Temp\303\251rature : </b> %1 <br/>").arg(C_GET_THE_PRE_CSV.INFO_1());
               }
            if (C_GET_THE_PRE_CSV.TYPE_PRE_CSV()=="3")
               { block +=  tr(" %1<br/>").arg(C_GET_THE_PRE_CSV.TEXTE());
               }
            if (C_GET_THE_PRE_CSV.TYPE_PRE_CSV()=="4")
               { block +=  tr("<b> R\303\251f. Officielle : </b>  %1<br/>").arg(C_GET_THE_PRE_CSV.INFO_1() + " en date du " + C_GET_THE_PRE_CSV.DATE_1("dd-MM-yyyy"));
               }
          }
    if (block.length()) html  += tr("<p class=nom_chapitres> CONSERVATION </p> %1") .arg(block);
    return block;
}
//------------------------------------ makePharmacoDynamiqueBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makePharmacoDynamiqueBlock(QString &html, int codeId,
                                                 int & /* indexCollapsiblePanel */, const QString & /*path */, QString & /*javaS_CreatePanel_Body */)
{   QString block    = "";
    C_GET_THE_DET_PHDYNA_CUR  C_GET_THE_DET_PHDYNA ( database(), this );
    while ( C_GET_THE_DET_PHDYNA.next( codeId, 1  ) )
          { block +=     C_GET_THE_DET_PHDYNA.TEXTEPH().replace("* ","<br/><br/> * ");
          }
    while ( C_GET_THE_DET_PHDYNA.next( codeId, 2  ) )
          { block +=    tr("<br/><b>R\303\251f. Officielle</b> : ") + C_GET_THE_DET_PHDYNA.TEXTEPH()
                      + tr(" En date du : ") + C_GET_THE_DET_PHDYNA.DTREF("dd-MM-yyyy");
          }

    if (block.length()) html  += tr("<p class=nom_chapitres> Propri\303\251t\303\251s pharmacodynamiques </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeEffetConduiteBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeEffetConduiteBlock(QString &html, const QString &s_codeId,
                                                            int & /* indexCollapsiblePanel */, const QString & /*path */, QString & /*javaS_CreatePanel_Body */)
{    QString block    = "";
    /*
      double  IDFCO()   {return m_IDFCO;}
      QString FCO_TXT() {return m_FCO_TXT;}
      QString INFO_01() {return m_INFO_01;}
      QDate   DAT_CRE() {return m_DAT_CRE;}


      double  FCOID()   {return m_FCOID;}
      QString GRP()     {return m_GRP;}
      QString INFO_01() {return m_INFO_01;}
      QDate   DAT_CRE() {return m_DAT_CRE;}
      QDate   DAT_MAJ() {return m_DAT_MAJ;}
    */
    C_GET_THE_FCO_ID_BY_SPE_CUR  C_GET_THE_FCO_ID_BY_SPE ( database(), this );
    C_GET_THE_DET_FCO_CUR        C_GET_THE_DET_FCO ( database(), this );
    while ( C_GET_THE_FCO_ID_BY_SPE.next( s_codeId  ) )
          { while ( C_GET_THE_DET_FCO.next( C_GET_THE_FCO_ID_BY_SPE.IDFCO()  ) )
                  { if (C_GET_THE_DET_FCO.GRP()=="01" )
                       { if  ( C_GET_THE_DET_FCO.INFO_01().toUpper() != "OUI") return QString("");
                       }
                    else
                       { block +=  C_GET_THE_DET_FCO.INFO_01() + "<br/>";
                       }
                  }
            block +=  C_GET_THE_FCO_ID_BY_SPE.FCO_TXT() + "<br/>";
            block +=  C_GET_THE_FCO_ID_BY_SPE.INFO_01() + tr(" en date du : ") + C_GET_THE_FCO_ID_BY_SPE.DAT_CRE().toString("dd-MM-yyyy") + "<br/>";

          }
    if (block.length()) html  += tr("<p class=nom_chapitres> Effets sur l'aptitude \303\240 conduire des v\303\251hicules et \303\240 utiliser des machines </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeCinetiqueBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeCinetiqueBlock(QString &html, int codeId,
                                                 int & /* indexCollapsiblePanel */, const QString & /*path */, QString & /*javaS_CreatePanel_Body */)
{   QString block    = "";
    //QString str      = "";
    //QString titre    = "";
    //QString refOff   = "";
    /*
      double  CODEPHARMACO()  {return m_CODEPHARMACO;}
      QDate   DTREF()         {return m_DTREF;}
      QString PHARMACOTEXT()  {return m_PHARMACOTEXT;}
      QString REFOFFICIELLE() {return m_REFOFFICIELLE;}

     */
    C_GET_THE_CINETIQUE_SPE_CUR  C_GET_THE_CINETIQUE_SPE ( database(), this );
    while ( C_GET_THE_CINETIQUE_SPE.next( codeId  ) )
          { block +=    C_GET_THE_CINETIQUE_SPE.PHARMACOTEXT().replace("* ","<br/><br/> * ")
                      + tr("<br/><b>R\303\251f. Officielle</b> : ") + C_GET_THE_CINETIQUE_SPE.REFOFFICIELLE()
                      + tr(" En date du : ") + C_GET_THE_CINETIQUE_SPE.DTREF("dd-MM-yyyy");
          }

    if (block.length()) html  += tr("<p class=nom_chapitres> Propri\303\251t\303\251s pharmacocin\303\251tiques </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeSecuritePrecliniqueBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeSecuritePrecliniqueBlock(QString &html, int codeId,
                                                 int & /* indexCollapsiblePanel */, const QString & /*path */, QString & /*javaS_CreatePanel_Body*/)
{   QString block    = "";
    //QString str      = "";
    //QString titre    = "";
    //QString refOff   = "";
    /*
      QString MY_CODEFIC() {return m_MY_CODEFIC;}
      QString MY_TEXTFIC() {return m_MY_TEXTFIC;}
      QString MY_DATECRE() {return m_MY_DATECRE;}
      QString MY_DATEMAJ() {return m_MY_DATEMAJ;}

      QString MY_REFOFF()                       {return m_MY_REFOFF;}
      QDate   MY_DATEREF()                      {return m_MY_DATEREF;}

     */
    C_GET_THE_SECUPRECLINIQUE_SPE_CUR  C_GET_THE_SECUPRECLINIQUE_SPE ( database(), this );
    C_GET_THE_REF_SECUPRECLINIQUE_CUR  C_GET_THE_REF_SECUPRECLINIQUE ( database(), this );
    while ( C_GET_THE_SECUPRECLINIQUE_SPE.next( codeId  ) )
          { block += C_GET_THE_SECUPRECLINIQUE_SPE.MY_TEXTFIC();
            while ( C_GET_THE_REF_SECUPRECLINIQUE.next( codeId , C_GET_THE_SECUPRECLINIQUE_SPE.MY_CODEFIC()  ) )
                  { block += tr("<br/><b>R\303\251f. Officielle</b> : ") + C_GET_THE_REF_SECUPRECLINIQUE.MY_REFOFF()
                             + tr(" En date du : ") + C_GET_THE_REF_SECUPRECLINIQUE.MY_DATEREF("dd-MM-yyyy");
                  }
          }
    if (block.length()) html  += tr("<p class=nom_chapitres> S\303\251curit\303\251 pr\303\251clinique </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeGrossesseAllaitementProcreationBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeGrossesseAllaitementProcreationBlock(QString &html, int codeId,
                                                 int &indexCollapsiblePanel, const QString &path, QString &javaS_CreatePanel_Body)
{   QString block    = "";
    //................................. GROSSESSE ..........................................................................
    QString str      = "";
    QString titre    = "";
    QString refOff   = "";
    C_GET_THE_GR_FIC_SPE_CUR  C_GET_THE_GR_FIC_SPE ( database(), this );
    C_GET_THE_GR_SPE_CUR      C_GET_THE_GR_SPE     ( database(), this );
    while ( C_GET_THE_GR_FIC_SPE.next( codeId  ) )
          {
            if ( C_GET_THE_GR_FIC_SPE.PASSAGELAIT()[0]=='O' )
               { titre += tr("<b><u>Passage transplacentaire</u></b>");
               }
            /*
            if ( C_GET_THE_GR_FIC_SPE.PASSAGELAIT()[0]=='N' )
               { block += tr("<b><u>Passage dans plasma maternels</u> : </b> Non <br/>");
               }
            if ( C_GET_THE_GR_FIC_SPE.FIXPROT()[0]=='O' )
               { block += tr("<b><u>Fixation prot\303\251ique</u> : </b> Oui <br/>");
               }
            if ( C_GET_THE_GR_FIC_SPE.FIXPROT()[0]=='N' )
               { block += tr("<b><u>Fixation prot\303\251ique</u> : </b> Non <br/>");
               }
            if ( C_GET_THE_GR_FIC_SPE.PLASMALAIT()[0]=='O' )
               { block += tr("<b><u>Rapport concentration plasma maternel</u> : </b> Oui <br/>");
               }
            if ( C_GET_THE_GR_FIC_SPE.PLASMALAIT()[0]=='N' )
               { block += tr("<b><u>Rapport concentration plasma maternels</u> : </b> Non <br/>");
               }
            if ( C_GET_THE_GR_FIC_SPE.CONCENT()[0]=='O' )
               { block += tr("<b><u>Concentration dans 100 ml de plasma maternel</u> : </b> Oui <br/>");
               }
            if ( C_GET_THE_GR_FIC_SPE.CONCENT()[0]=='N' )
               { block += tr("<b><u>Concentration dans 100 ml de plasma maternel</u> : </b> Non <br/>");
               }

            while ( C_GET_THE_GR_SPE.next( codeId ,1 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )   // inutile
                  { str += tr("<br/><b> Informations g\303\251n\303\251rales de la fiche : Id theriaque </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
             */
            while ( C_GET_THE_GR_SPE.next( codeId ,17 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )  // reference officielle
                  { refOff +=  C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,2 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) // Observations
                  { str += tr("<br/><b> Donn\303\251es exp\303\251rimentales animales : </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,3 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )  // texte observation
                  { str += "<br/>" + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,4 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) // risques
                  { str += tr("<br/><b> Risques </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,5 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { str += tr("<br/><b> Circonstances favorisantes </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }

            while ( C_GET_THE_GR_SPE.next( codeId ,6 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) // Observations
                  { str += tr("<br/><b> Donn\303\251es cliniques humaines :  </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,7 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { str += "<br/>" + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,8 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) // Risques
                  { str += tr("<br/><b> Risques </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,9 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) // Periodes critiques
                  { str += tr("<br/><b> Periodes critiques </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,10 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) //Circonstances favorisantes
                  { str += tr("<br/><b> Circonstances favorisantes </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }

            while ( C_GET_THE_GR_SPE.next( codeId ,11 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) ) // Niveau de risque
                  { str += tr("<br/><b> Recommandations et conduite \303\240 tenirs (niveau de risque) : </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,12 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { str += "<br/>" + C_GET_THE_GR_SPE.TEXTGR();
                  }

            while ( C_GET_THE_GR_SPE.next( codeId ,13 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )  // conduite a tenir
                  { str += tr("<br/><b> Recommandations et conduite \303\240 tenir chez la femme enceinte : </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,14 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )  // Texte conduite a tenir
                  { str += "<br/>" + C_GET_THE_GR_SPE.TEXTGR();
                  }

            while ( C_GET_THE_GR_SPE.next( codeId ,15 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )  // conduite a tenir
                  { str += tr("<br/><b> Recommandations et conduite \303\240 tenir chez le nouveau n\303\251 : </b>  ") + C_GET_THE_GR_SPE.TEXTGR();
                  }
            while ( C_GET_THE_GR_SPE.next( codeId ,16 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )  // Texte conduite a tenir
                  { str += "<br/>" + C_GET_THE_GR_SPE.TEXTGR();
                  }
          }
    //........... ajout des infos concernant la grossesse dans un collapsible panel............
    if (str.length())
       { block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                            refOff.remove("00:00:00"),
                                                            "",               // reference officielle
                                                            tr("Grossesse"),      //
                                                            titre,                // Passage transplacentaire                                        // smr
                                                            "",
                                                            path,
                                                            str,
                                                            javaS_CreatePanel_Body,
                                                            "panel_EffeNotoireCss");
       }
    //.................................... ALLAITEMENT ......................................................................
    str     = "";
    titre   = "";
    refOff  = "";
    C_GET_THE_AL_FIC_SPE_CUR  C_GET_THE_AL_FIC_SPE ( database(), this );
    C_GET_THE_AL_SPE_CUR      C_GET_THE_AL_SPE     ( database(), this );
    while ( C_GET_THE_AL_FIC_SPE.next( codeId  ) )
          {if ( C_GET_THE_AL_FIC_SPE.PASSAGELAIT()[0]=='O' )
              { titre += tr("<b><u>Passage dans le lait</u><b>");
              }
          if ( C_GET_THE_AL_FIC_SPE.FIXPROT()[0]=='O' )
             { str += tr("<b><u>Fixation prot\303\251ique</u> : </b> Oui <br/>");
             }
          if ( C_GET_THE_AL_FIC_SPE.FIXPROT()[0]=='N' )
             { str += tr("<b><u>Fixation prot\303\251ique</u> : </b> Non <br/>");
             }
          if ( C_GET_THE_AL_FIC_SPE.PLASMALAIT()[0]=='O' )
             { str += tr("<b><u>Rapport concentration dans le lait maternel</u> : </b> Oui <br/>");
             }
          if ( C_GET_THE_AL_FIC_SPE.PLASMALAIT()[0]=='N' )
             { str += tr("<b><u>Rapport concentration dans le lait maternel</u> : </b> Non <br/>");
             }
          if ( C_GET_THE_AL_FIC_SPE.CONCENT()[0]=='O' )
             { str += tr("<b><u>Concentration dans 100 ml de lait maternel</u> : </b> Oui <br/>");
             }
          if ( C_GET_THE_AL_FIC_SPE.CONCENT()[0]=='N' )
             { str += tr("<b><u>Concentration dans 100 ml de lait maternel</u> : </b> Non <br/>");
             }
           while ( C_GET_THE_AL_SPE.next( codeId , 7 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { refOff +=  C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 1 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Conduite \303\240 tenir : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 2 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Conduite a tenir : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 3 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Recommandation : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 4 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Recommandation : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 5 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Observation : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 6 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Circonstance favorisante : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
           while ( C_GET_THE_AL_SPE.next( codeId , 8 , C_GET_THE_AL_FIC_SPE.CODE_FICHE()  ) )
                 { str += tr("<br/><b> Observations chez le nouveau-n\303\251 : </b>  ") + C_GET_THE_AL_SPE.TEXTAL();
                 }
          }
    //........... ajout des infos concernant l'allaitement dans un collapsible panel............
    if (str.length())
       { block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                            refOff.remove("00:00:00"),
                                                            "",               // reference officielle
                                                            tr("Allaitement"),    //
                                                            titre,                // Passage transplacentaire                                        // smr
                                                            "",
                                                            path,
                                                            str,
                                                            javaS_CreatePanel_Body,
                                                            "panel_EffeNotoireCss");
       }
    //.............................................. PROCREATION .............................................
    str     = "";
    titre   = "";
    refOff  = "";
    int ok  = 0;
    C_GET_THE_FPRO_SPE_CUR  C_GET_THE_FPRO_SPE ( database(), this );
    while ( C_GET_THE_GR_FIC_SPE.next( codeId  ) )
          {
            while ( C_GET_THE_FPRO_SPE.next( codeId , 1 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { str += "<br/>" + C_GET_THE_FPRO_SPE.TEXTFP();
                  }
            while ( C_GET_THE_FPRO_SPE.next( codeId , 2 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { str += "<br/>" + C_GET_THE_FPRO_SPE.TEXTFP();
                  }
            while ( C_GET_THE_FPRO_SPE.next( codeId , 3 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { if (C_GET_THE_FPRO_SPE.TEXTFP() == "AUCUNE INFORMATION RECENSEE")  ok = 0;
                    else                                                               ok = 1;
                    str += "<br/>" + C_GET_THE_FPRO_SPE.TEXTFP();
                  }
            while ( C_GET_THE_FPRO_SPE.next( codeId , 4 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { str += "<br/>" + C_GET_THE_FPRO_SPE.TEXTFP();
                  }
            while ( C_GET_THE_FPRO_SPE.next( codeId , 5 , C_GET_THE_GR_FIC_SPE.MY_CODE_FICHE()  ) )
                  { refOff +=  C_GET_THE_FPRO_SPE.TEXTFP();
                  }
          }
    //........... ajout des infos concernant la procreation dans un collapsible panel............
    if (ok && str.length())
       { block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                            refOff.remove("00:00:00"),
                                                            "",               // reference officielle
                                                            tr("Procr\303\251ation"),    //
                                                            titre,                // Passage transplacentaire                                        // smr
                                                            "",
                                                            path,
                                                            str,
                                                            javaS_CreatePanel_Body,
                                                            "panel_EffeNotoireCss");
       }

    if (block.length()) html  += tr("<p class=nom_chapitres> Grossesse Allaitement Procr\303\251ation </p> %1") .arg(block);
    return block;
}
//------------------------------------ makeCIBlock -------------------------------
QString       C_BDM_TheriaquePlugin::makeCIBlock(QString &html, const QString &s_codeId, const QString &type,
                                                 int &indexCollapsiblePanel, const QString &path, QString &javaS_CreatePanel_Body)
{   QString block       = "";
    C_GET_CIPEMG_SPE_CUR          C_GET_CIPEMG_SPE         ( database(), this );     // retirer les CI
    C_GET_THE_DET_CIPEMG_CUR      C_GET_THE_DET_CIPEMG     ( database(), this );     // extraire les info
    C_GET_THE_REF_CIPEMG_CUR      C_GET_THE_REF_CIPEMG     ( database(), this );     // extraire les dates et amm concern\303\251s
    QString niveau = "";
    //........... Extraction des CI niveau 'type'  pour la specialite 's_codeId' ............
    while ( C_GET_CIPEMG_SPE.next( s_codeId, type) )      // "'C', 'P','I','N'"
          { QMap <QString , QString > cimMap;
            QString tmp    = "";
            QString str    = "";
            int isResult;
            while ( (isResult=C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 1  )) )
                 { tmp += "<br/>" + C_GET_THE_DET_CIPEMG.TEXTDET();   // 1 titres secondaires de la CI
                 }
            tmp = tmp.prepend(C_GET_CIPEMG_SPE.LIBELLE());   // prefixer par le titre principal
            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 2  ) )
                  { niveau  = C_GET_THE_DET_CIPEMG.TEXTDET()
                              .remove("CONTRE-INDICATION");          // 2 niveau de la CI
                  }
            while ( (isResult=C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 3  )) )
                  { if (isResult>1) str += "<br/>";
                    str += C_GET_THE_DET_CIPEMG.TEXTDET();           // 3 lignes accesoires :  Cf. GROSSESSE ET ALLAITEMENT Cf. GROSSESSE ET ALLAITEMENT
                  }
            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 4  ) )
                  { str +=  "<br/>" + C_GET_THE_DET_CIPEMG.TEXTDET(); // 4 grande information de texte
                  }
            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 5  ) )
                  { cimMap[C_GET_THE_DET_CIPEMG.INFO_01()] = C_GET_THE_DET_CIPEMG.TEXTDET();
                                                                     // 5 CIM10 reli\303\251 au terrain
                  }
            while ( C_GET_THE_DET_CIPEMG.next( C_GET_CIPEMG_SPE.CODE_CIPEMG() , C_GET_CIPEMG_SPE.CODE_TERRAIN() , C_GET_CIPEMG_SPE.CODE_NATURE() , C_GET_CIPEMG_SPE.NUM_SEQ() , 6  ) )
                  { cimMap[C_GET_THE_DET_CIPEMG.INFO_01()] = C_GET_THE_DET_CIPEMG.TEXTDET().replace(">","&gt;").replace("<","&lt;");
                                                                     // 6 CIM10 reli\303\251 au commentaires terrain
                  }
            while ( C_GET_THE_REF_CIPEMG.next( s_codeId.toInt() , C_GET_CIPEMG_SPE.CODE_CIPEMG()  ) )
                  { QString dt = C_GET_THE_REF_CIPEMG.DTREFOFF().toString("dd-MM-yyyy");
                    str += "<br/><b><u>" + C_GET_THE_REF_CIPEMG.REFOFF()+"</u></b> "+ dt +"<br/>";
                  }
            //........... liste des patho CIM10 ............
            QString cim10  = "";
            QMapIterator <QString, QString> it(cimMap) ;
            while ( it.hasNext() )
                  { it.next();
                    if (! it.value().toLower().startsWith("non con") && ! it.value().toLower().startsWith("non att"))
                         cim10 +=  it.value() + " (<b>" + it.key()+"</b>)<br/>";
                  }
            if (cim10.endsWith("<br/>"))
               {//cim10 = cim10.left(cim10.length()-4);
                cim10 = cim10.prepend("<b><u>Pathologie CIM 10</u></b><br/>");
                str   = str.prepend(cim10);
               }
            //........... ajout de la ligne de CI ............
            block += addCollapsibleJSPanel       ( indexCollapsiblePanel++,
                                                                    tmp,
                                                                    "",                                        // code cim 10 si il exite
                                                                    "NIVEAU CI",   //str.length()?"NIVEAU SMR <br/>":"",
                                                                    QString("<br/>")+niveau,    // str.remove("NIVEAU "),                                        // smr
                                                                    "",
                                                                    path,   //R\303\251f. : Journal Officiel du
                                                                    str.replace("- ","<br/>\302\272"),
                                                                    javaS_CreatePanel_Body,
                                                                    "panel_EffeNotoireCss");
          }

    niveau = codeNatureCi_to_Text(QString(type).remove('\'').toLatin1()[0]);
    if (block.length()) html  += tr("<p class=nom_chapitres> %2 </p> %1") .arg(block, niveau);
    return block;
}
//------------------------------------ codeNatureCi_to_Text -------------------------------
QString       C_BDM_TheriaquePlugin::codeNatureCi_to_Text(char code)
{switch ((int)code)
 {
   case 'C': return tr("CONTRE-INDICATIONS");
   case 'P': return tr("PRECAUTIONS D'EMPLOIS");
   case 'I': return tr("NON INDICATIONS");
   case 'N': return tr("NON CONTRE-INDICATIONS");
   default : return tr("CHAIS PAS C'EST QUOI");
 }
}

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
QString  C_BDM_TheriaquePlugin::addCollapsibleJSPanel       ( int   index,
                                                              const QString &date,
                                                              const QString &level,
                                                              const QString &libelle,
                                                              const QString &codeCimx,
                                                              const QString &indication,
                                                              const QString &path,
                                                              QString &descriptif,
                                                              QString &javaS_CreatePanel_Body ,
                                                              const QString &className /* = "CollapsiblePanelTab" */)
{       QString idStr = QString::number(index);
        //............... cree les collapse panell .................................................
        descriptif += "<br/>";
        descriptif  = C_Utils_Html::makeLinkList(descriptif,"<br/>","EOL", idStr).replace("- ","<br/>&nbsp;&nbsp;&nbsp;\302\272");
        javaS_CreatePanel_Body += QString("    var CollapsiblePanel%1 = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel%1\", {contentIsOpen:false});\n"
                                         ).arg(idStr);
        return                    QString("\n"
                                          "<a name=\"smr%1\"></a>\n"       // ancre
                                          "<div id=\"CollapsiblePanel%1\" class=\"CollapsiblePanel CollapsiblePanelClosed\">\n"
                                          "  <div class=\"%9\" tabindex=\"0\" onclick=\"openClosePanel(%1)\">\n"
                                          "     <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"99%\">\n"
                                          "        <tbody>\n"
                                          "           <tr>\n"
                                          "               <td class=\"bdm_titre_chapitre_bleu\">%8<font color=#9f0000>  %4 %5</font> </td>\n"
                                          "               <td align=\"right\" class=\"bdm_info_statut\"> %2 <br/> %3</td>\n"
                                          "               <td align=\"right\" width=\"30\"><img src=\"%6/panel_ouvrir.png\" name=\"bouton%1\" title=\"Ouvrir\"></td>\n"
                                          "           </tr>\n"
                                          "        </tbody>\n"
                                          "     </table>\n"
                                          "  </div>\n"
                                          "  <div class=\"CollapsiblePanelContent\" style=\"visibility: hidden; display: block; height: 524px; \">\n"
                                          "      <div id=\"data%1\">\n"
                                          "        %7\n"                    // truc \303\240 afficher
                                          "      </div>\n"
                                          "  </div>\n"     // %1  %2     %3      %4          %5      %6      %7         %8    %9
                                          "</div>\n").arg(idStr, date, level, codeCimx, indication, path, descriptif, libelle, className);
}
//-------------------------------- getInteractionPhysicoChimiques ------------------------------------------------------
/*! \brief return chimico physiques interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_TheriaquePlugin::getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
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
QList<C_InterResultPair> C_BDM_TheriaquePlugin::getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
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
QList<C_InterResultPair> C_BDM_TheriaquePlugin::getInteractionRedundancy( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{Q_UNUSED(cipList);
 Q_UNUSED(pC_PatientCtx);
 QList<C_InterResultPair> interResultPairList;
 return interResultPairList;
}
//------------------------------ getRefundedIndications ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
 *  \param const QString &code_type  code typ as giving by the drugs BDM
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_TheriaquePlugin::getRefundedIndications( const QString &code_indication,   const QString &code_type )
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
/*........................ usage .......................................
    C_GET_SPE_FROM_INDICATION_TEXT_CUR  C_GET_SPE_FROM_INDICATION_TEXT ( database(), this );
    while ( C_GET_SPE_FROM_INDICATION_TEXT.next( "%asthme%") )
          { html += tr(         "<br/><b> C_GET_SPE_FROM_INDICATION_TEXT_CUR : </b>"
                                "<br/> specialite         : " + C_GET_SPE_FROM_INDICATION_TEXT.SPE_NOM()          +
                                "<br/> code spe           : " + C_GET_SPE_FROM_INDICATION_TEXT.SPE_CODE_ID()      +
                                "<br/> cip                : " + C_GET_SPE_FROM_INDICATION_TEXT.CIP()              +
                                "<br/> Libelle Indication : " + C_GET_SPE_FROM_INDICATION_TEXT.IND_TEXT()         +
                                "<br/> code indication    : " + C_GET_SPE_FROM_INDICATION_TEXT.IND_PK()           +
                                "<br/> date               : " + C_GET_SPE_FROM_INDICATION_TEXT.DATE("dd-MM-yyyy")
                       );
          }
 */
/*
  explicit C_BDM_IndicationRecord(const QString &cip,
                                  const QString &date,
                                  const QString &level,
                                  const QString &levelTyp,
                                  const QString &levelLibelle,
                                  const QString &indication,
                                  const QString &descriptif,
                                  const QString &code,
                                  const QString &codeTyp
                                 )
*/
 QList <C_BDM_IndicationRecord> C_BDM_TheriaquePlugin::getIndicationsFromText( const QString &text,   int filter )
 {      Q_UNUSED(filter);
        QList <C_BDM_IndicationRecord> retList;
        C_GET_SPE_FROM_INDICATION_TEXT_CUR  C_GET_SPE_FROM_INDICATION_TEXT ( database(), this );
        while     ( C_GET_SPE_FROM_INDICATION_TEXT.next( text ) )
                  { retList.append(C_BDM_IndicationRecord( C_GET_SPE_FROM_INDICATION_TEXT.CIP(),
                                                           C_GET_SPE_FROM_INDICATION_TEXT.DATE("dd-MM-yyyy"),
                                                           "",
                                                           "",
                                                           "",
                                                           C_GET_SPE_FROM_INDICATION_TEXT.IND_TEXT(),
                                                           "",
                                                           C_GET_SPE_FROM_INDICATION_TEXT.IND_PK(),
                                                           "TH_I"
                                                          )

                                  );
                  }
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
QList <C_BDM_IndicationRecord> C_BDM_TheriaquePlugin::getIndicationsFromCIP( const QString &cip,   int filter )
{Q_UNUSED(filter);
 double  codeId       = CIP_to_ID(cip);
 QString niveau       = "";
 QString niveauTyp    = "";
 QString libelleSMR   = "";
 QString descriptif   = "";
 QMap  <QString, C_BDM_IndicationRecord> mapResult;        // on mappe pour eviter les doublons de codes rendus en cas de descriptifs diff\303\251rents...
 QList <C_BDM_IndicationRecord>          retList;
 C_GET_THE_IND_SPE_CUR  C_GET_THE_IND_SPE( database() , this );
 if ( filter&C_BDM_PluginI::CIM )
    {while ( C_GET_THE_IND_SPE.next( codeId  ))
       {
        //............ relever le smr ....................
        libelleSMR   = "";
        niveau       = "";
        descriptif   = "";
        C_GET_THE_SMR_SPE_CUR  C_GET_THE_SMR_SPE( database() , this );
        while ( C_GET_THE_SMR_SPE.next( C_GET_THE_IND_SPE.CODEIND(), codeId ))
              {libelleSMR  =  C_GET_THE_SMR_SPE.TEXTSMR();
               niveau      =  C_GET_THE_SMR_SPE.NIVEAU();
              }
        if (niveau.length()) niveauTyp = "S";
        else                 niveauTyp = "";
        //............ relever les details ....................
        C_GET_THE_DET_IND_CUR  C_GET_THE_DET_IND ( database(), this );
        //............ relever le texte ....................
        descriptif   += "\n"+C_GET_THE_IND_SPE.FIN_TEXT().replace(">","&gt;").replace("<","&lt;") .replace("- ","<br/>\302\272 ");
        while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 1  ) )       // 1 EN RELAIS DE LA VOIE PARENTERALE 2 DANS LES FORMES AIGUES 4"" 6""  8 AMM/IDIQUE       9 les indication tienne comte ...pareil que C_GET_THE_IND_SPE.FIN_TEXT()
              { descriptif +=  C_GET_THE_DET_IND.LIBCOURT()+"<br/>";
              }
        while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 2  ) )       // 1 EN RELAIS DE LA VOIE PARENTERALE 2 DANS LES FORMES AIGUES 4"" 6""  8 AMM/IDIQUE       9 les indication tienne comte ...pareil que C_GET_THE_IND_SPE.FIN_TEXT()
              { descriptif +=  C_GET_THE_DET_IND.LIBCOURT()+"<br/>";
              }
        //........... relever les indication CIM 10 correspondntes ........................
        while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 5  ) )      // 5 libelle CIM 10
              { if (C_GET_THE_DET_IND.CODEFILS()[0] != '.')
                   { mapResult[C_GET_THE_DET_IND.CODEFILS()]  = C_BDM_IndicationRecord( cip,
                                                                                        C_GET_THE_SMR_SPE.DTSMR().toString("dd-MM-yyyy"),
                                                                                        niveau.length()?"S"+TheriaqueSMR_LevelToNormalisedLevel(niveau.toInt()):"",
                                                                                        niveauTyp,
                                                                                        libelleSMR,
                                                                                        C_GET_THE_DET_IND.LIBLONG(),
                                                                                        descriptif,
                                                                                        C_GET_THE_DET_IND.CODEFILS(),
                                                                                        "CIM"
                                                                                       );

                   }
              } // while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 5  ) )      // 5 libelle CIM 10
       } //while ( C_GET_THE_IND_SPE.next( codeId  ))
    } // if ( filter&C_BDM_PluginI::BDM )

 if ( filter&C_BDM_PluginI::BDM )
    {while ( C_GET_THE_IND_SPE.next( codeId  ))
       { //............ relever le smr ....................
         libelleSMR   = "";
         niveau       = "";
         descriptif   = "";
         C_GET_THE_SMR_SPE_CUR  C_GET_THE_SMR_SPE( database() , this );
         while ( C_GET_THE_SMR_SPE.next( C_GET_THE_IND_SPE.CODEIND(), codeId ))
               {libelleSMR  =  C_GET_THE_SMR_SPE.TEXTSMR();
                niveau      =  C_GET_THE_SMR_SPE.NIVEAU();
               }
         if (niveau.length()) niveauTyp = "S";
         else                 niveauTyp = "";
         //............ relever les details ....................
         C_GET_THE_DET_IND_CUR  C_GET_THE_DET_IND ( database(), this );
         //............ relever le texte ....................
         descriptif   += "\n"+C_GET_THE_IND_SPE.FIN_TEXT().replace(">","&gt;").replace("<","&lt;") .replace("- ","<br/>\302\272 ");
         while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 1  ) )       // 1 EN RELAIS DE LA VOIE PARENTERALE 2 DANS LES FORMES AIGUES 4"" 6""  8 AMM/IDIQUE       9 les indication tienne comte ...pareil que C_GET_THE_IND_SPE.FIN_TEXT()
               { descriptif +=  C_GET_THE_DET_IND.LIBCOURT()+"<br/>";
               }
         while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 2  ) )       // 1 EN RELAIS DE LA VOIE PARENTERALE 2 DANS LES FORMES AIGUES 4"" 6""  8 AMM/IDIQUE       9 les indication tienne comte ...pareil que C_GET_THE_IND_SPE.FIN_TEXT()
               { descriptif +=  C_GET_THE_DET_IND.LIBCOURT()+"<br/>";
               }

         while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 7  ) )   // 7 libelle NON CIM 10
               { mapResult[QString::number(C_GET_THE_IND_SPE.CODEIND())]=
                                 C_BDM_IndicationRecord( cip,
                                                         C_GET_THE_SMR_SPE.DTSMR().toString("dd-MM-yyyy"),
                                                         niveau.length()?"S"+TheriaqueSMR_LevelToNormalisedLevel(niveau.toInt()):"",
                                                         niveauTyp,
                                                         libelleSMR,
                                                         C_GET_THE_DET_IND.LIBCOURT(),
                                                         descriptif,
                                                         QString::number(C_GET_THE_IND_SPE.CODEIND()),
                                                         "TH_I"
                                                       );

               }  //while ( C_GET_THE_DET_IND.next( C_GET_THE_IND_SPE.CODEIND() , 7  ) )   // 7 libelle NON CIM 10
       } // while ( C_GET_THE_IND_SPE.next( codeId  ))
    } //if ( filter&C_BDM_PluginI::BDM )
 QMapIterator<QString, C_BDM_IndicationRecord> itm(mapResult);
 while (itm.hasNext())
 {   itm.next();
     retList.append(itm.value());
 }
 return retList;
}
//------------------------------ getCIPFromIndication ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code       indication code of the drugs from wich wee want datas.
 *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
 *  \return QStringList cip result list
 */
/*
 SELECT   CIM_LIBELLE_LONG  as LibelleCim10 ,
          T1.CIM_CODE_PK    as CodeCim10    ,
          PRE_CODE_PK       as cip          ,
          SP_NOM            as DrugName
 FROM     sp_specialite         ,
          pre_presentation      ,
          finsp_indspe          ,
          fin_ficheindic      T3,
          cim10               T1,
          cimcdf_cim10_codif  T2
 WHERE    FIN_CODE_SQ_PK=FINSP_FIN_CODE_FK_PK
          AND    T1.CIM_CODE_PK               = T2.CIMCDF_CIM_CODE_FK_PK
          AND    T2.CIMCDF_CDF_CODE_FK_PK     = T3.FIN_CDF_NAIN_CODE_FK_PK
          AND    T2.CIMCDF_CDF_NUMERO_FK_PK   = 'NN'
          AND    PRE_SP_CODE_FK               = SP_CODE_SQ_PK
          AND    FINSP_SP_CODE_FK_PK          = PRE_SP_CODE_FK
          AND    T1.CIM_CODE_PK               = 'L50'
       --   AND finsp_indspe.FINSP_FIN_CODE_FK_PK = '9310'
*/
QStringList C_BDM_TheriaquePlugin::getCIPFromIndication( const QString &code, const QString &codeType )
{ QStringList retList;
  if (codeType=="CIM")
     { C_GET_CIP_FROM_CODE_CIM_CUR  C_GET_CIP_FROM_CODE_CIM ( database(), this );
       while ( C_GET_CIP_FROM_CODE_CIM.next( code ) )
             { retList += C_GET_CIP_FROM_CODE_CIM.CIP() ;
             }
     }
  else
    {
      C_GET_CIP_FROM_CODE_IND_CUR  C_GET_CIP_FROM_CODE_IND ( database(), this );
      while ( C_GET_CIP_FROM_CODE_IND.next( code ) )
            { retList += C_GET_CIP_FROM_CODE_IND.CIP() ;
            }
    }
 return retList;
}

//---------------------------- getNextSmrInfoForCIP ---------------------------------------
/*! \brief select SMR and ASM informations for a drug. You must call this methode While returns true.
 *  \param const QString &cip      code CIP of the drug from wich wee want datas.
 *  \param QString &date           it will contains date        of SMR ASM.
 *  \param QString &level          it will contains level       of SMR ASM..
 *  \param QString &indication     it will contains indication  of SMR ASM..
 *  \param QString &descriptif     it will contains description of SMR ASM..
 *  \param QString &codeCimx       it will contains code CIM10  of SMR ASM..
 *  \return true while datas are presents.. You must loop while returns true.
 */
/*
bool          C_BDM_TheriaquePlugin::getNextSmrInfoForCIP (     const QString &cip,
                                                                QString &date,
                                                                QString &level,
                                                                QString &levelLibelle,
                                                                QString &indication,
                                                                QString &descriptif,
                                                                QString &codeCimx
                                                          )
{if (m_firstTimeGetNextSmrInfoForCIP == true )
    { if ( getSmrList_Start(cip)==0 ) return false;
      m_firstTimeGetNextSmrInfoForCIP = false;
    }
 C_BDM_SmrListRecord c_bdm_smrlistrecord;
 if ( getSmrList_NextRecord(c_bdm_smrlistrecord) )
    { date         = c_bdm_smrlistrecord.datetime();
      level        = c_bdm_smrlistrecord.level();
      levelLibelle = c_bdm_smrlistrecord.level_libelle();
      indication   = c_bdm_smrlistrecord.libelle_2();
      descriptif   = c_bdm_smrlistrecord.descrip_2();
      codeCimx     = c_bdm_smrlistrecord.id_2();
      return true;
    }
 else
    { m_firstTimeGetNextSmrInfoForCIP = true;
      return false;
    }
}
*/
//---------------------------- TheriaqueSMR_LevelToNormalisedLevel ---------------------------------------
QString C_BDM_TheriaquePlugin::TheriaqueSMR_LevelToNormalisedLevel(int th_level)
{switch(th_level)
 {case 1  : return "2";    // faible
  case 2  : return "4";    // important
  case 3  : return "1";    // insuffisant
  case 4  : return "3";    // modere
  default : return "";
 }
}

//---------------------------- getDispVilleHopitalFromCIP ---------------------------------------
QString C_BDM_TheriaquePlugin::getDispVilleHopitalFromCIP(const QString &cip_code)
{
    QString     requete  =
    // -- liste des dispensations a partir du code CIP --
    " SElECT  PREDILI_CDF_LIEU_CODE_FK_PK   "
    " FROM    predili_lieu_dispensat        "
    " WHERE   PREDILI_PRE_CODE_FK_PK     = '" + cip_code + "'";
   QSqlQuery query(requete , database() );
   QString result = "H";
   if (query.isActive())
      {while (query.next())
          {if (query.value(0).toString()=="D1")
              {result = "VH";
               break;
              }
          }
      }
   else
      {outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::getDispVilleHopitalFromCIP() </b></font>"), requete, __FILE__, __LINE__  );
      }
   return result;
}

//---------------------------- getDciValuesFromCIP ---------------------------------------
QStringList C_BDM_TheriaquePlugin::getDciValuesFromCIP(const QString &cip_code)
{


    QStringList list;
    QList<C_CompositionItem> compositionList = getCompositionFromCIP(cip_code);

    //.................. composition DCI .....................................................................
    //              C_InterResultPair.Code1()     is the quantity
    //              C_InterResultPair.CodeTyp1()  is the quantity unit
    //              C_InterResultPair.Term1()     is the composant name (DCI)
    int nb = qMin(compositionList.size(),3);
    for (int i = 0; i < nb; ++i)
        { //interResultPair     =  compositionList.at(i);
          //list               +=  interResultPair.Term1();
            list               +=  compositionList.at(i).name();
        }

    /*
    QStringList list;
    QString     requete  =
   // -- liste des compositions DCI a partir du code CIP --
   " SELECT SAC_NOM"
   " FROM cosac_compo_subact "
   " join sac_subactive    on COSAC_SAC_CODE_FK_PK = SAC_CODE_SQ_PK "
   " join sp_specialite    on COSAC_SP_CODE_FK_PK  = SP_CODE_SQ_PK "
   " join pre_presentation on PRE_SP_CODE_FK       = SP_CODE_SQ_PK "
   " where PRE_CODE_PK='"+cip_code+"'";
   QSqlQuery query(requete , database() );

   if (query.isActive())
      {while (query.next())
          {list<< query.value(0).toString();
          }
      }
   else
      {outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::getDciValuesFromCIP() </b></font>"), requete, __FILE__, __LINE__  );
      }
   */
   return list;
}

//---------------------------- getPrixUCDFromCIP ---------------------------------------
QString C_BDM_TheriaquePlugin::getPrixUCDFromCIP(const QString &cip_code)
{QString         ret  = "";
 C_GET_PRIX_FROM_CIP_CUR  C_GET_PRIX_FROM_CIP ( database(), this );
 while ( C_GET_PRIX_FROM_CIP.next( cip_code ) )
       { ret = C_GET_PRIX_FROM_CIP.PRIX();
       }
 return ret;

}

////////////////////////////// smrList /////////////////////////////////////////////
/*
void set_id_1(QString value)          {m_id_1          = value;}
void set_id_type_1(QString value)     {m_id_type_1     = value;}
void set_libelle_1(QString value)     {m_libelle_1     = value;}
void set_descrip_1(QString value)     {m_descrip_1     = value;}
void set_id_2(QString value)          {m_id_2          = value;}
void set_id_type_2(QString value)     {m_id_type_2     = value;}
void set_libelle_2(QString value)     {m_libelle_2     = value;}
void set_descrip_2(QString value)     {m_descrip_2     = value;}
void set_level(QString value)         {m_level         = value;}
void set_level_libelle(QString value) {m_level_libelle = value;}
void set_cat(QString value)           {m_cat           = value;}
void set_smr_type(QString value)      {m_smr_type      = value;}
void set_datetime(QString value)      {m_datetime      = value;}
void set_owner(QString value)         {m_owner         = value;}
void set_lang(QString value)          {m_lang          = value;}
*/
//-------------------------------- smrList_Count ---------------------------------------
// service m\303\251dical rendu  SMR
//                   therLevel             MT level
// SMR majeur                  80   \303\240 99   90
// SMR important         2     60   \303\240 79   70
// SMR Mod\303\251r\303\251            4     40   \303\240 59   50
// SMR Faible            1     20   \303\240 39   30
// SMR insuffisant       3     1    \303\240 19   10
// SMR non pr\303\251cis\303\251       0                  0

// L'am\303\251lioration du service m\303\251dical rendu (ASMR)

// Niveau I.   : Progr\303\251s th\303\251rapeutique majeur
// Niveau II.  : Am\303\251lioration importante en termes d'efficacit\303\251 th\303\251rapeutique et/ou de r\303\251duction des effets ind\303\251sirables
// Niveau III. : Am\303\251lioration modeste en termes d'efficacit\303\251 th\303\251rapeutique et/ou de r\303\251duction des effets ind\303\251sirables
// Niceau IV.  : Am\303\251lioration mineure en termes d'efficacit\303\251 th\303\251rapeutique et/ou d'utilit\303\251 au plan clinique (acceptabilit\303\251, commodit\303\251 d'emploi, obervance), compl\303\251ment de gamme justifi\303\251 ou avantage potentiel li\303\251 aux propri\303\251t\303\251s pharmaceutiques ou au moindre risque d'interaction m\303\251dicamenteuse
// Niveau V.   : Pas d'am\303\251lioration avec avis favorable \303\240 l'inscription
// Niveau VI   : Avis d\303\251favorable \303\240 linscription collectivit\303\251 ou S\303\251curit\303\251 sociale
// ASMR 00     : Am\303\251lioration difficile \303\240 pr\303\251ciser

int   C_BDM_TheriaquePlugin::smrList_Count()
{   m_position      = 0;
    int    nbRecord = 0;
    if (!database().isOpen() && database().open()== false)
       {outMessage( tr("ERREUR : C_BDM_TheriaquePlugin::smrList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
       }
  //......................... on commence par les SMR lies aux indications ...................
  QString     requete  =
  " SELECT COUNT(*) "
  " FROM finsmr_indic_smr_atr "
  " join sp_specialite         on FINSMR_SP_CODE_FK_PK=SP_CODE_SQ_PK "
  " join pre_presentation      on PRE_SP_CODE_FK=SP_CODE_SQ_PK "
  " join cdf_codif             on CDF_CODE_PK=FINSMR_CDF_SMR_CODE_FK_PK "
  " join fin_ficheindic        on FIN_CODE_SQ_PK=FINSMR_FIN_CODE_FK_PK "
  " join cimcdf_cim10_codif    on CIMCDF_CDF_CODE_FK_PK=FIN_CDF_NAIN_CODE_FK_PK "
  " join cim10                 on CIM_CODE_PK=CIMCDF_CIM_CODE_FK_PK "
  " WHERE CDF_NUMERO_PK='27AND CIMCDF_CDF_NUMERO_FK_PK = 'NN'";
    QSqlQuery query(requete , database() );
    outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::smrList_Count() SMRI</b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {nbRecord += query.value(0).toInt();
       }
    //......................... on poursuit par les SMR Globaux ...................
  /*
  requete  =
  " SELECT COUNT(*) "
  " FROM spsmr_service_medical_rendu "
  " join sp_specialite            on SPSMR_SP_CODE_FK_PK=SP_CODE_SQ_PK "
  " join pre_presentation         on PRE_SP_CODE_FK=SP_CODE_SQ_PK "
  " join cdf_codif                on CDF_CODE_PK=SPSMR_CDF_SMR_CODE_FK_PK "
  " where CDF_NUMERO_PK='27' ";  // group by (PRE_CODE_PK)"
  */
  requete  =
  " SELECT  count(*) from "
  " ( select distinct ATR_DATE_REDACTION,PRE_CODE_PK, SP_NOMLONG, ATR_CODE_SQ_PK,       CDF_NOM,     ATR_TEXTE,      CDF_CODE_PK"
  " FROM spsmr_service_medical_rendu"
  " join sp_specialite                    on SPSMR_SP_CODE_FK_PK     = SP_CODE_SQ_PK "
  " join pre_presentation                 on PRE_SP_CODE_FK          = SP_CODE_SQ_PK "
  " join spatr_spec_avistransparence      on SPATR_SP_CODE_FK_PK     = SP_CODE_SQ_PK"
  " join atr_aviscommissiontransparence   on SPATR_ATR_CODE_FK_PK    = ATR_CODE_SQ_PK"
  " join cdf_codif                        on CDF_CODE_PK             = SPSMR_CDF_SMR_CODE_FK_PK "
  " where CDF_NUMERO_PK='27' "
  " ) as foo ";
  /*
  " SELECT  COUNT(*) "
  " FROM   spatr_spec_avistransparence"
  " join   atr_aviscommissiontransparence on SPATR_ATR_CODE_FK_PK     = ATR_CODE_SQ_PK"
  " join   spsmr_service_medical_rendu    on SPATR_SP_CODE_FK_PK      = SPSMR_SP_CODE_FK_PK"
  " join   sp_specialite                  on SPSMR_SP_CODE_FK_PK      = SP_CODE_SQ_PK "
  " join   pre_presentation               on PRE_SP_CODE_FK           = SP_CODE_SQ_PK "
  " join   cdf_codif                      on SPSMR_CDF_SMR_CODE_FK_PK = CDF_CODE_PK"
  " Where  CDF_NUMERO_PK                  = '27'";

  " SELECT COUNT(*) "
  " FROM   atrsmr_aviscomtrans_smr"
  " join   cdf_codif                       on  ATRSMR_CDF_SMR_CODE_FK_PK   = CDF_CODE_PK"
  " join   atr_aviscommissiontransparence  on  ATRSMR_ATR_CODE_FK_PK       = ATR_CODE_SQ_PK"
  " join   spatr_spec_avistransparence     on  SPATR_ATR_CODE_FK_PK        = ATR_CODE_SQ_PK"
  " join   sp_specialite                   on  SP_CODE_SQ_PK               = SPATR_SP_CODE_FK_PK"
  " join   pre_presentation                on  PRE_SP_CODE_FK              = SP_CODE_SQ_PK"
  " where  CDF_NUMERO_PK                    = '27' ";
  */
    query.exec(requete);
    outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::smrList_Count() SMRG</b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {nbRecord += query.value(0).toInt();
       }
    //......................... on poursuit par les ASMR ...................
    requete  =
    " SELECT COUNT(*) "
    " FROM   atrasmr_aviscomtrans_asmr"
    " join   cdf_codif                       on  ATRASMR_CDF_ASMR_CODE_FK_PK = CDF_CODE_PK"
    " join   atr_aviscommissiontransparence  on  ATRASMR_ATR_CODE_FK_PK      = ATR_CODE_SQ_PK"
    " join   spatr_spec_avistransparence     on  SPATR_ATR_CODE_FK_PK        = ATR_CODE_SQ_PK"
    " join   sp_specialite                   on  SP_CODE_SQ_PK               = SPATR_SP_CODE_FK_PK"
    " join   pre_presentation                on  PRE_SP_CODE_FK              = SP_CODE_SQ_PK"
    " where  CDF_NUMERO_PK                    = '23'"
    " and ATRASMR_CDF_ASMR_CODE_FK_PK    like 'A%'";

      query.exec(requete);
      outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::smrList_Count() ASMR</b></font>"), requete, __FILE__, __LINE__  );
      if (query.isActive() && query.next())
         {nbRecord += query.value(0).toInt();
         }
  return nbRecord;
}

//-------------------------------- getSmrList_Start ---------------------------------------
int   C_BDM_TheriaquePlugin::getSmrList_Start()
{return getSmrList_Start("");
}

//-------------------------------- getSmrList_Start ---------------------------------------
int   C_BDM_TheriaquePlugin::getSmrList_Start(const QString &cip)
{   m_position = 0;
    if (!database().isOpen() && database().open()== false)
       {outMessage( tr("ERREUR : C_BDM_TheriaquePlugin::getSmrList_Start() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    QString     requete  =
    //       0                   1            2           3            4                 5          6
    //       date                id_1 (cip)   libelle_1   id_2 (cimx)  libelle_2         descrip_2  therLevel
    " SELECT FINSMR_DATE_SMR_PK, PRE_CODE_PK, SP_NOMLONG, CIM_CODE_PK, CIM_LIBELLE_LONG, FIN_TEXTE, FINSMR_CDF_SMR_CODE_FK_PK "
    " FROM finsmr_indic_smr_atr "
    " join sp_specialite         on FINSMR_SP_CODE_FK_PK=SP_CODE_SQ_PK "
    " join pre_presentation      on PRE_SP_CODE_FK=SP_CODE_SQ_PK "
    " join cdf_codif             on CDF_CODE_PK=FINSMR_CDF_SMR_CODE_FK_PK "
    " join fin_ficheindic        on FIN_CODE_SQ_PK=FINSMR_FIN_CODE_FK_PK "
    " join cimcdf_cim10_codif    on CIMCDF_CDF_CODE_FK_PK=FIN_CDF_NAIN_CODE_FK_PK "
    " join cim10                 on CIM_CODE_PK=CIMCDF_CIM_CODE_FK_PK "
    " WHERE CDF_NUMERO_PK='27' AND CIMCDF_CDF_NUMERO_FK_PK = 'NN'";
    if (cip.length()) requete += " AND PRE_CODE_PK = '"+cip+"'";
    m_pQSqlQuery->exec( requete );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_TheriaquePlugin::getSmrList_Start() 1", requete, __FILE__, __LINE__);
    if ( !m_pQSqlQuery->isActive() ) return 0;
    //......................... on poursuit par les SMR Globaux ...................
    requete  =
    " SELECT SPSMR_DATE, PRE_CODE_PK, SP_NOMLONG, SPSMR_CDF_SMR_CODE_FK_PK "
    " FROM spsmr_service_medical_rendu "
    " join sp_specialite            on SPSMR_SP_CODE_FK_PK=SP_CODE_SQ_PK "
    " join pre_presentation         on PRE_SP_CODE_FK=SP_CODE_SQ_PK "
    " join cdf_codif                on CDF_CODE_PK=SPSMR_CDF_SMR_CODE_FK_PK "
    " where CDF_NUMERO_PK='27'"; // group by (PRE_CODE_PK) ";

    m_pQSqlQuery2->exec( requete );
    outSQL_error( *m_pQSqlQuery2, "ERREUR  : C_BDM_TheriaquePlugin::getSmrList_Start() 2", requete, __FILE__, __LINE__);
    if ( !m_pQSqlQuery2->isActive() ) return 0;

    requete  =
    //                0                   1            2           3              4            5          6
    //                date                id_1 (cip)   libelle_1   id_2           libelle_2    descrip_2  therLevel
    " SELECT distinct ATR_DATE_REDACTION,PRE_CODE_PK, SP_NOMLONG, ATR_CODE_SQ_PK, CDF_NOM,     ATR_TEXTE, CDF_CODE_PK"
    " FROM spsmr_service_medical_rendu"
    " join sp_specialite                    on SPSMR_SP_CODE_FK_PK     = SP_CODE_SQ_PK "
    " join pre_presentation                 on PRE_SP_CODE_FK          = SP_CODE_SQ_PK "
    " join spatr_spec_avistransparence      on SPATR_SP_CODE_FK_PK     = SP_CODE_SQ_PK"
    " join atr_aviscommissiontransparence   on SPATR_ATR_CODE_FK_PK    = ATR_CODE_SQ_PK"
    " join cdf_codif                        on CDF_CODE_PK             = SPSMR_CDF_SMR_CODE_FK_PK "
    " where CDF_NUMERO_PK='27' ";

    if (cip.length()) requete += " AND PRE_CODE_PK = '"+cip+"'";
    m_pQSqlQuery2->exec( requete );
    outSQL_error( *m_pQSqlQuery2, "ERREUR  : C_BDM_TheriaquePlugin::getSmrList_Start() 2", requete, __FILE__, __LINE__);
    if ( !m_pQSqlQuery2->isActive() ) return 0;

    requete  =
    //       0                   1            2           3                     4                 5          6
    //       date                id_1 (cip)   libelle_1   id_2                  libelle_2    descrip_2       therLevel
    " SELECT ATR_DATE_REDACTION, PRE_CODE_PK, SP_NOMLONG, ATR_CODE_SQ_PK,       CDF_NOM,     ATR_TEXTE,      ATRASMR_CDF_ASMR_CODE_FK_PK"
    " FROM   atrasmr_aviscomtrans_asmr"
    " join   cdf_codif                       on  ATRASMR_CDF_ASMR_CODE_FK_PK = CDF_CODE_PK"
    " join   atr_aviscommissiontransparence  on  ATRASMR_ATR_CODE_FK_PK      = ATR_CODE_SQ_PK"
    " join   spatr_spec_avistransparence     on  SPATR_ATR_CODE_FK_PK        = ATR_CODE_SQ_PK"
    " join   sp_specialite                   on  SP_CODE_SQ_PK               = SPATR_SP_CODE_FK_PK"
    " join   pre_presentation                on  PRE_SP_CODE_FK              = SP_CODE_SQ_PK"
    " where  CDF_NUMERO_PK               = '23'"
    " and ATRASMR_CDF_ASMR_CODE_FK_PK    like 'A%'";
    if (cip.length()) requete += " AND PRE_CODE_PK = '"+cip+"'";
    m_pQSqlQuery3->exec( requete );
    outSQL_error( *m_pQSqlQuery3, "ERREUR  : C_BDM_TheriaquePlugin::getSmrList_Start() 3", requete, __FILE__, __LINE__);
    if ( !m_pQSqlQuery3->isActive() ) return 0;

    return 1;
}

//-------------------------------- getSmrList_NextRecord ---------------------------------------
// service m\303\251dical rendu  SMR
//                   therLevel             MT level
// SMR majeur                  80   \303\240 99   90    / 20 -> 4
// SMR important         2     60   \303\240 79   70    / 20 -> 3
// SMR Mod\303\251r\303\251            4     40   \303\240 59   50    / 20 -> 2
// SMR Faible            1     20   \303\240 39   30    / 20 -> 1
// SMR insuffisant       3     1    \303\240 19   10    / 20 -> 0
// SMR non pr\303\251cis\303\251       0                  0
/*
int   C_BDM_TheriaquePlugin::getSmrList_NextRecord (C_BDM_SmrListRecord  &rC_BDM_SmrListRecord)
{   QStringList s_libelle;
    //............................... tables de correspondance niveau du SMR theriaque et niveau MedinTux .......................................
    //           code theriaque   0                             1                 2                    3                      4                               5                 6
    static int  s_level[7]     = {0,                            30,               70,                  10,                    50,                             90,               0};
    static int  s_asmrLevel[7] = {78,                           62,               46,                  30,                    24,                             8,                0};
                s_libelle  <<tr("SMR non pr\303\251cis\303\251")<<tr("SMR faible")<<tr("SMR important")<<tr("SMR insuffisant")<<tr("SMR mod\303\251r\303\251")<<tr("SMR majeur")<<tr("SMR difficile \303\240 pr\303\251ciser");
  //          0                   1            2           3            4                 5          6
  //          date                id_1 (cip)   libelle_1   id_2 (cimx)  libelle_2         descrip_2  therLevel
  // " SELECT FINSMR_DATE_SMR_PK, PRE_CODE_PK, SP_NOMLONG, CIM_CODE_PK, CIM_LIBELLE_LONG, FIN_TEXTE, FINSMR_CDF_SMR_CODE_FK_PK "

  if (m_pQSqlQuery->next() && m_position < 200000)
     {int theriaqueLevel = m_pQSqlQuery->value(6).toInt();
      rC_BDM_SmrListRecord.set_id_1(m_pQSqlQuery->value(1).toString());
      rC_BDM_SmrListRecord.set_id_type_1("CIP");
      rC_BDM_SmrListRecord.set_libelle_1(CGestIni::Utf8_Query(*m_pQSqlQuery, 2));
      rC_BDM_SmrListRecord.set_descrip_1(tr("drug for SMR by CIP"));
      rC_BDM_SmrListRecord.set_id_2(m_pQSqlQuery->value(3).toString());
      rC_BDM_SmrListRecord.set_id_type_2("CIMX");
      rC_BDM_SmrListRecord.set_libelle_2(CGestIni::Utf8_Query(*m_pQSqlQuery, 4));
      rC_BDM_SmrListRecord.set_descrip_2(CGestIni::Utf8_Query(*m_pQSqlQuery, 5));
      rC_BDM_SmrListRecord.set_level(QString::number(s_level[theriaqueLevel]));
      rC_BDM_SmrListRecord.set_level_libelle(s_libelle[theriaqueLevel].toLatin1());
      rC_BDM_SmrListRecord.set_cat("SMRI");
      rC_BDM_SmrListRecord.set_smr_type("SMR");   // ASMR SMR
      rC_BDM_SmrListRecord.set_datetime(m_pQSqlQuery->value(0).toString());
      rC_BDM_SmrListRecord.set_owner(owner());
      rC_BDM_SmrListRecord.set_lang(lang());
      ++m_position;
      return 1;
    }  // endif (query.isActive() )
  //         0                   1            2           3                4            5               6
  //         date                id_1 (cip)   libelle_1   id_2             libelle_2    descrip_2       therLevel
  //" SELECT ATR_DATE_REDACTION, PRE_CODE_PK, SP_NOMLONG, ATR_CODE_SQ_PK,  CDF_NOM,     ATR_TEXTE,      ATRSMR_CDF_SMR_CODE_FK_PK"
  if (m_pQSqlQuery2->next() && m_position < 200000)
     {int theriaqueLevel = m_pQSqlQuery2->value(6).toInt();
      rC_BDM_SmrListRecord.set_id_1(m_pQSqlQuery2->value(1).toString());
      rC_BDM_SmrListRecord.set_id_type_1("CIP");
      rC_BDM_SmrListRecord.set_libelle_1(CGestIni::Utf8_Query(*m_pQSqlQuery2, 2));
      rC_BDM_SmrListRecord.set_descrip_1(tr("drug for SMR by CIP"));

      rC_BDM_SmrListRecord.set_id_2("0");
      rC_BDM_SmrListRecord.set_id_type_2("SMRG");
      rC_BDM_SmrListRecord.set_libelle_2(tr("SMR global en dehors d'une indication"));
      rC_BDM_SmrListRecord.set_descrip_2(CGestIni::Utf8_Query(*m_pQSqlQuery2, 5).replace("\n","<br/>"));
      rC_BDM_SmrListRecord.set_level(QString::number(s_level[theriaqueLevel]));
      rC_BDM_SmrListRecord.set_level_libelle(m_pQSqlQuery2->value(4).toString());
      rC_BDM_SmrListRecord.set_cat("SMRG");
      rC_BDM_SmrListRecord.set_smr_type("SMR");   // ASMR SMR
      rC_BDM_SmrListRecord.set_datetime(m_pQSqlQuery2->value(0).toString());
      rC_BDM_SmrListRecord.set_owner(owner());
      rC_BDM_SmrListRecord.set_lang(lang());
      ++m_position;
      return 1;
    }  // endif (query.isActive() )
    //         0                   1            2           3                4                 5          6
    //         date                id_1 (cip)   libelle_1   id_2             libelle_2    descrip_2       therLevel
    //" SELECT ATR_DATE_REDACTION, PRE_CODE_PK, SP_NOMLONG, ATR_CODE_SQ_PK,  CDF_NOM,     ATR_TEXTE,      ATRASMR_CDF_ASMR_CODE_FK_PK"
    if (m_pQSqlQuery3->next() && m_position < 200000)
       {int theriaqueLevel = m_pQSqlQuery3->value(6).toString().mid(1).toInt();
        rC_BDM_SmrListRecord.set_id_1(m_pQSqlQuery3->value(1).toString());
        rC_BDM_SmrListRecord.set_id_type_1("CIP");
        rC_BDM_SmrListRecord.set_libelle_1(CGestIni::Utf8_Query(*m_pQSqlQuery3, 2));
        rC_BDM_SmrListRecord.set_descrip_1(tr("drug for SMR by CIP"));
        rC_BDM_SmrListRecord.set_id_2("0");

        rC_BDM_SmrListRecord.set_id_type_2("ASMRG");
        rC_BDM_SmrListRecord.set_libelle_2(tr("ASMR global en dehors d'une indication"));
        rC_BDM_SmrListRecord.set_descrip_2(CGestIni::Utf8_Query(*m_pQSqlQuery3, 5).replace("\n","<br/>"));
        rC_BDM_SmrListRecord.set_level(QString::number(s_asmrLevel[theriaqueLevel]));
        rC_BDM_SmrListRecord.set_level_libelle(m_pQSqlQuery3->value(4).toString());
        rC_BDM_SmrListRecord.set_cat("ASMR");
        rC_BDM_SmrListRecord.set_smr_type("ASMR");   // ASMR SMR
        rC_BDM_SmrListRecord.set_datetime(m_pQSqlQuery3->value(0).toString());
        rC_BDM_SmrListRecord.set_owner(owner());
        rC_BDM_SmrListRecord.set_lang(lang());
        ++m_position;
        return 1;
      }  // endif (query.isActive() )
 return 0;
}
*/
////////////////////////////// drugsList /////////////////////////////////////////////
//---------------------------- drugsList_Count ---------------------------------------
/*
QString id()             {return m_id;}              // ok
QString id_type()        {return m_id_type;}         // ok
QString commercialName() {return m_commercialName;}  // ok
QString dci_1Name()      {return m_dci_1Name;}       // ok
QString dci_2Name()      {return m_dci_2Name;}       // ok
QString dci_3Name()      {return m_dci_3Name;}       // ok
QString owner()          {return m_owner;}           // ok
QString lang()           {return m_lang;}            // ok
QString ucd()            {return m_ucd;}             // ok
QString ucd_price()      {return m_ucd_price;}       // ok
QString atc()            {return m_atc;}             // ok
*/
long C_BDM_TheriaquePlugin::drugsList_Count()
{   m_position = 0;
    if (!database().isOpen() && database().open()== false)
       {outMessage( tr("ERREUR : C_BDM_TheriaquePlugin::drugsList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    QString     requete  =
            " SELECT COUNT(*)"
            " FROM sp_specialite left join pre_presentation on PRE_SP_CODE_FK=SP_CODE_SQ_PK"
            " join cdf_codif on SP_CDF_SLAB_CODE_FK=CDF_CODE_PK "
            " where  (CDF_NUMERO_PK='15' or CDF_NUMERO_PK='08')"
            " and CDF_NOM like 'LISTE%' "
            " and SP_CIPUCD<>'' "
            " and PRE_ETAT_COMMER='D'";
    QSqlQuery query(requete , database() );
    outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_TheriaquePlugin::drugsList_Count() </b></font>"), requete, __FILE__, __LINE__  );
      if (query.isActive() && query.next())
         {return (long) query.value(0).toDouble();
         }
    return 0;
}
//---------------------------- getDrugList_Start ---------------------------------------
int C_BDM_TheriaquePlugin::getDrugList_Start()
{m_position = 0;
 if (!database().isOpen() && database().open()== false)
    {outMessage( tr("ERREUR : C_BDM_TheriaquePlugin::getDrugList_Start() database can not be open"), __FILE__, __LINE__);  return 0;
    }
 QString     requete  =
 //              0           1          2                3                   4
 //        commercialName   ucd        atc              id (cip)          generique
 " SELECT  SP_NOM,        SP_CIPUCD, SP_CATC_CODE_FK, PRE_CODE_PK,        SP_TYPE_SPE"
 " FROM sp_specialite left join pre_presentation on PRE_SP_CODE_FK=SP_CODE_SQ_PK"
 " join cdf_codif on SP_CDF_SLAB_CODE_FK=CDF_CODE_PK "
 " where  (CDF_NUMERO_PK='15' or CDF_NUMERO_PK='08')"
 " and CDF_NOM like 'LISTE%' "
 " and SP_CIPUCD<>'' "
 " and PRE_ETAT_COMMER='D'";
 m_pQSqlQuery->exec( requete );
 outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_TheriaquePlugin::getDrugList_Start()", requete, __FILE__, __LINE__);
 if ( !m_pQSqlQuery->isActive() ) return 0;
 else                             return 1;
return 0;
}

//---------------------------- getDrugList_NextRecord ---------------------------------------
int  C_BDM_TheriaquePlugin::getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord)
{   QStringList dciList;
    QString     libelle   = "";
    QString     cip_code  = "";
    QString     nb_up     = "";
    QString     atc_code  = "";
    QString     dci_1     = "";
    QString     dci_2     = "";
    QString     dci_3     = "";
    QString     prix      = "";
    QString     generique = "";
    QString     statut    = "";

  if (m_pQSqlQuery->next() && m_position < 200000)
     {libelle      = CGestIni::Utf8_Query(*m_pQSqlQuery, 0);
      cip_code     = m_pQSqlQuery->value(3).toString();
      atc_code     = m_pQSqlQuery->value(2).toString();
      // ucd_code     = m_pQSqlQuery->value(1).toString();
      generique    = m_pQSqlQuery->value(4).toString();
      statut       = getDispVilleHopitalFromCIP(cip_code);
      dciList      = getDciValuesFromCIP(cip_code);
      prix         = getPrixUCDFromCIP(cip_code).remove('.');
      dci_1        = "";
      dci_2        = "";
      dci_3        = "";
      if (dciList.count())
         {if (dciList.count()>0) dci_1 = dciList[0];
          if (dciList.count()>1) dci_2 = dciList[1];
          if (dciList.count()>2) dci_3 = dciList[2];
         }
      rC_BDM_DrugListRecord.set_commercialName(libelle);
      rC_BDM_DrugListRecord.set_id(cip_code);
      rC_BDM_DrugListRecord.set_id_type("CIP");
      rC_BDM_DrugListRecord.set_owner(owner());
      rC_BDM_DrugListRecord.set_lang(lang());
      rC_BDM_DrugListRecord.set_atc(atc_code);
      rC_BDM_DrugListRecord.set_dci_1Name(dci_1);
      rC_BDM_DrugListRecord.set_dci_2Name(dci_2);
      rC_BDM_DrugListRecord.set_dci_3Name(dci_3);
      rC_BDM_DrugListRecord.set_nb_up(nb_up);
      rC_BDM_DrugListRecord.set_ucd_price(prix);
      rC_BDM_DrugListRecord.set_generique(generique);
      rC_BDM_DrugListRecord.set_statut(statut);
      ++m_position;
      return 1;
    }  // endif (query.isActive() )
    return 0;
}
//---------------------------- Slot_On_TheriaqueCursorError ---------------------------------------
void C_BDM_TheriaquePlugin::Slot_On_TheriaqueCursorError(const QSqlQuery &sqlquery, const QString &from, const QString &query, const QString &file, int line)
{outSQL_error(sqlquery , from, query, file, line  );
}



