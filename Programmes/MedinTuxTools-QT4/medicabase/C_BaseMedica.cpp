#include "C_BaseMedica.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QTreeWidget>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QLabel>
#include <QTreeWidgetItemIterator>
#include <QProgressBar>
#include <QApplication>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebView>
#include <QWebFrame>
#include <QUrl>

#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#include "../C_Utils_Log.h"

#define MAX_READ        32000
#define OFSET           ".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
//--------------------------- C_BaseMedica -----------------------------------------------------------
C_BaseMedica::C_BaseMedica( const QString &param , QString& confData, QObject *parent) :
    C_BaseCommon(parent), C_DBVar( confData)

{if (!C_DBVar::areFieldsNamesValids())                       C_BaseMedica::setLastError(C_DBVar::getLastError());
 if (!BaseConnect(param,"BaseInteractions","BaseGetTest"))   C_BaseMedica::setLastError(C_BaseCommon::getLastError());
 m_lang                   = "fr";
}
//--------------------------- setLastError -----------------------------------------------------------
void       C_BaseMedica::setLastError(const QString &error){ m_LastError = error;}
//--------------------------- getLastError -----------------------------------------------------------
QString    C_BaseMedica::getLastError(){return m_LastError;}

//----------------------------------------------------- Datasemp_ToMedicaTux -----------------------------------------------------------
long C_BaseMedica::afssaps_ToMedicaTux()
{ //if (pQProgressBar) pQProgressBar->setTotalSteps(15400); // 103237  15445
  /*
  QString up_unit, pa_unit, pa_qu, secabilite, is_generic,formStruct, nbPrises, prixBoite, libelle ;
  QString remboursementSS, tableau,labo, atc, pkt00, pkt4b, codePD, codeUV, rcp,cis, cip;
  //m_Progress = 0;
  //m_Run      = 1;
  if (!database().isOpen() && database().open()== FALSE) return 0;
  //m_FormeUPList.clear();
 //........................................creer les tables  ................................................
 //if (Medica_CreateTableSpecificationProduit(m_MEDICA_SPEC_TBL_NAME)==0)      return 0;  //restons conservateur
 if (Medica_CreateTableIndexProduit(m_MEDICA_INDEX_MED_TBL_NAME)==0)         return 0;

 //................. CREER L'INDEX SRC....................................................................................

 QString  requete     = "SELECT  `afs_ciscip_cip_code`, "    // 341687620 sachet(s)-dose(s) papier polypropylène
                               " `afs_ciscip_cis_code`, "    // 685862031 flacon(s) en verre brun de 60 ml avec fermeture de sécurité enfant avec pipette(s) graduée(s) en verre
                               " `afs_ciscip_libelle`,  "    //           plaquette(s) thermoformée(s) PVDC polyéthylène aluminium de 14 comprimé(s)`
                               "  FROM   `afs_ciscip` WHERE `afs_ciscip_comm_stat` NOT LIKE  \"%arrêt%\""; //                       ^^ casser la
 secabilite = 4;
 is_generic = "";
 tableau    = "";

 QSqlQuery query(QString::null , database() );
 if (!query.exec(requete))
    {qDebug(tr("\nERREUR  : C_BaseCommon::isThisValueLikeInTable_ToPk( \n%1\nREQUETE : %2").arg(query.lastError().text(),requete).toAscii());  // on efface la table
     return 0;
    }
 if (query.isActive())
    {while (query.next())
       {cis = query.value(0).toString();
        cip = query.value(1).toString();
        createRecordIndexProduit(  cip,         // cip
                                   libelle,     // libelle
                                   "",          // pk_T00
                                   "",          // pk_T4B
                                   "",          // cd_prod
                                   "",          // cd_uv
                                   "",          // is_gene
                                  "0"           // is_dispo
                                );
        afssaps_GetSpecifications(cis,
                                  cip,
                                  up_unit,         // ok
                                  pa_unit,         // ok
                                  pa_qu,           // ok
                                  secabilite,      // ok
                                  is_generic,      // ok
                                  formStruct,      // ok
                                  nbPrises,        // ok
                                  prixBoite,       // ok
                                  remboursementSS, // ok
                                  tableau,         // ok
                                  labo,            // ok
                                  atc,             // ok
                                  pkt00,            // = QString::null
                                  pkt4b,            // = QString::null
                                  codePD,           // = QString::null
                                  codeUV            // = QString::null
                                 );
        afssaps_SetSpecifications(cip,
                                  up_unit,         // ok
                                  pa_unit,         // ok
                                  pa_qu,           // ok
                                  secabilite,      // ok
                                  is_generic,      // ok
                                  formStruct,      // ok
                                  nbPrises,        // ok
                                  prixBoite,       // ok
                                  remboursementSS, // ok
                                  tableau,         // ok
                                  labo,            // ok
                                  atc,             // ok
                                  pkt00,            // = QString::null
                                  pkt4b,            // = QString::null
                                  codePD,           // = QString::null
                                  codeUV            // = QString::null
                                 );
       }
    }

  QSqlQuery* pSqlQuery = new QSqlQuery(requeteSrc , m_MedicaBase );
  //................ scanner les enregistrements ................................................
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (pSqlQuery && pSqlQuery->isActive())
     {
      while (pSqlQuery->next() && m_Run) // &&nb < 400)
        {    QString cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc;
             ++m_Progress;
             if (pQProgressBar) pQProgressBar->setProgress(m_Progress);
             cip = pSqlQuery->value( 1 ).toString();
             if (//MysteriousBaseGetSpec(pSqlQuery->value( 2 ).toString(),    // PK_T00
                 //                      pSqlQuery->value( 3 ).toString(),    // PK_T4B
                 //                         cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc)
                 Datasemp_GetSpecifications(cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc,             // ok
                                            pSqlQuery->value( 2 ).toString(),            // PK_T00  = QString::null
                                            pSqlQuery->value( 3 ).toString(),            // PK_T4B  = QString::null
                                            pSqlQuery->value( 4 ).toString(),            // CD_PROD = QString::null
                                            pSqlQuery->value( 5 ).toString()             // CD_UV = QString::null
                                           )
                )
                {Medica_InsertNewSpec(cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc);
                }
             nb++;
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  OutSQL_error( *pSqlQuery, "Datasemp_ToMedicaTux()", requeteSrc);
  if (pSqlQuery) delete  pSqlQuery;

  //Medica_CreateTableFormeProduit(m_MEDICA_FORME_UP_TBL_NAME, m_FormeUPList);
  //Medica_CreateTableFormeProduit(m_MEDICA_FORME_PA_TBL_NAME, m_FormePAList);
  //Medica_CreateTableFormeProduit(m_MEDICA_FORME_ST_TBL_NAME, m_FormeSTList);
  //..................... placer le numero de version du datasemp au niveau de l'index ..........................
  //QString numEdition;
  //Datasemp_GetBaseVersion( &numEdition, 0, 0);
  //Medica_SetDatasempVersion(numEdition, CMedicaBase::Data);
  return nb;
  */
    return 0;
}


/*
CApp::pCApp()->getDB()->createTable(
    "CREATE TABLE `afs_cis` (                          "
    "  `afs_cis_pk` bigint(20) NOT NULL auto_increment,"
    "  `afs_cis_owner` char(40)                       ,"
    "  `afs_cis_code` char(8)                         ,"
    "  `afs_cis_libelle` char(255)                    ,"
    "  `afs_cis_forme` char(100)                      ,"
    "  `afs_cis_voie` char(140)                       ,"
    "  `afs_cis_amm` char(40)                         ,"
    "  `afs_cis_procedure` char(40)                   ,"
    "  `afs_cis_comm` char(40)                        ,"
    "  `afs_cis_rcp` char(6)                          ,"
    "  PRIMARY KEY  (`afs_cis_pk`)                     "
    ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
    CApp::pCApp()->getDB()->createTable(
    "CREATE TABLE `afs_ciscip` (                        "
    "`afs_ciscip_pk` bigint(20) NOT NULL auto_increment,"
    "`afs_ciscip_owner` char(40)                       ,"
    "`afs_ciscip_cis_code` char(8)                     ,"
    "`afs_ciscip_cip_code` char(7)                     ,"
    "`afs_ciscip_libelle` char(255)                    ,"
    "`afs_ciscip_adm_stat` char(50)                    ,"
    "`afs_ciscip_comm_stat` char(100)                  ,"
    "`afs_ciscip_date_decl` char(10)                   ,"
    "`afs_ciscip_cip_long` char(13)                    ,"
    "PRIMARY KEY  (`afs_ciscip_pk`)                     "
    ") ENGINE=MyISAM AUTO_INCREMENT=0;                  ");
    CApp::pCApp()->getDB()->createTable(
    "CREATE TABLE `afs_comp` (                         "
    "`afs_comp_pk` bigint(20) NOT NULL auto_increment ,"
    "`afs_comp_owner` char(40)                        ,"
    "`afs_comp_cis_code` char(8)                      ,"
    "`afs_comp_forme` char(100)                       ,"
    "`afs_comp_subst` char(8)                         ,"
    "`afs_comp_libelle` char(255)                     ,"
    "`afs_comp_dosage` char(100)                      ,"
    "`afs_comp_dose_ref` char(100)                    ,"
    "`afs_comp_nature` char(4)                        ,"
    "`afs_comp_lien` char(6)                          ,"
    "PRIMARY KEY  (`afs_comp_pk`)                      "
    ") ENGINE=MyISAM AUTO_INCREMENT=0                  ");
    */
//----------------------------------------------------- Datasemp_GetSpecifications ---------------------------------------------------------------------------
QString C_BaseMedica::afssaps_GetSpecifications(  const QString &cis,
                                                  const QString &cip,
                                                  QString &up_unit,         // ok
                                                  QString &pa_unit,         // ok
                                                  QString &pa_qu,           // ok
                                                  QString &secabilite,      // ok
                                                  QString &is_generic,      // ok
                                                  QString &formStruct,      // ok
                                                  QString &nbPrises,        // ok
                                                  QString &prixBoite,       // ok
                                                  QString &remboursementSS, // ok
                                                  QString &tableau,         // ok
                                                  QString &labo,            // ok
                                                  QString &atc,             // ok
                                                  QString pkt00,            // = QString::null
                                                  QString pkt4b,            // = QString::null
                                                  QString codePD,           // = QString::null
                                                  QString codeUV            // = QString::null
                                                )
{

/*
    "CREATE TABLE `afs_cis` (                          "
    "  `afs_cis_pk` bigint(20) NOT NULL auto_increment,"
    "  `afs_cis_owner` char(40)                       ,"
    "  `afs_cis_code` char(8)                         ,"
    "  `afs_cis_libelle` char(255)                    ,"
    "  `afs_cis_forme` char(100)                      ,"
    "  `afs_cis_voie` char(140)                       ,"
    "  `afs_cis_amm` char(40)                         ,"
    "  `afs_cis_procedure` char(40)                   ,"
    "  `afs_cis_comm` char(40)                        ,"
    "  `afs_cis_rcp` char(6)                          ,"
    "  PRIMARY KEY  (`afs_cis_pk`)                     "
    ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");

*/
    /*
        //.................... recuperer le CIP .................................................................
        QSqlQuery query(QString::null , database() );
        QString requete  = QString("SELECT "
                                   "`afs_cis_libelle`,"
                                   "`afs_cis_forme`  ,"
                                   "`afs_cis_voie`   ,"
                                   "`afs_cis_rcp`     "
                                   " FROM `afs_cis` WHERE `afs_cis_code`  = \"%1\" ").arg( cis );
        requete += ownersSelectMention("it_compo");
        if (!query.exec(requete))
          {qDebug(tr("ERREUR  : C_BaseCommon::getNameFacteurFromPk() \n%1\nREQUETE : %2").arg(query.lastError().text(),requete).toAscii());
           return QString::null;
          }
        if (query.isActive() )
            {while(query.next())    // il ne devrait il n'y en avoir qu'un
                  {return query.value(0).toString();
                  }
            }
        else
           {return QString::null;
           }
        return QString::null;
  //................... si accelerateurs de recherche non renseignés, les renseigner ..........................
  //                    ultra lent !!!!
  if (pkt00.length()==0 || pkt4b.length()==0 || codePD.length()==0 || codeUV.length()==0)
     {QString requete("SELECT ");
      // 2  PrimaryKey ________________
      // 1  Code unite de vente ___    |
      // 0  code produit    ___    |   |
      //                       |   |   |
      requete +=             "f0, f1, ge FROM t00 WHERE f2 = '" + cip + "'";
      QSqlQuery query(requete , database() );
      if (! (query.isActive() && query.next()))                             return QString::null;
      codePD = Utf8_Query(query,0);
      codeUV = Utf8_Query(query,1);
      pkt00  = Utf8_Query(query,2);
      Datasemp_GetField("t4B", "f0",  codePD, "f2", &pkt4b);
     }
  //............... aller à la pêche dans la t4b .....................................
  QStringList list;
  Datasemp_GetT4B( list, pkt4b);
  formStruct      = Datasemp_GetField("t25", "f0", list[T4B_F1_FORME], "f1");                // forme structuree
  atc             = list[T4B_FA_ATC_CLASS];
  Datasemp_GetFicheEconomique( cip, prixBoite, labo, tableau, remboursementSS, codeUV   );
  / *
  prixBoite       = Datasemp_GetField("t01", "f0",  codeUV,           "f3");   // SELECT fieldToGet FROM table WHERE testField = testValue
  labo            = Datasemp_GetField("t02", "f0",  codeUV,           "f1");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir code fabriquant
  labo            = Datasemp_GetField("t17", "f0",  labo,             "f2");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir libelle nom fabriquant
  remboursementSS = Datasemp_GetField("t03", "f0",  codeUV,           "f2");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir code remboursement
  remboursementSS = Datasemp_GetField("t1D", "f0",  remboursementSS,  "f1");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir libelle remboursement
  tableau         = Datasemp_GetField("t03", "f0",  codeUV,           "f3");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir liste
  * /
  is_generic      = Datasemp_GetGenericState(codePD, 0);
  pa_unit         = Datasemp_GetDosage(list[T4B_FB_CODE_DOSAGE], list[T4B_F0_CODE_PRODUIT]).remove(" ; ");

  //............... separer ivraie du bon grain .....................................
  pa_qu           = "1";
  if (pa_unit[0]>='0' && pa_unit[0]<='9' || pa_unit[0]=='.' || pa_unit[0]==',')
     {int pos = 0;
      while (pa_unit[pos]>='0' && pa_unit[pos]<='9' || pa_unit[pos]==',' || pa_unit[pos]=='.') ++pos;
      pa_qu       = pa_unit.left(pos).replace(",",".");
      pa_unit     = pa_unit.mid(pos);
     }
  //................. il  en faut un peu de la t00 .........................

  secabilite      = "1";
  if      (formStruct.find(". séc")!=-1)          secabilite = "2";
  else if (formStruct.find("quadrisécable")!=-1)  secabilite = "4";
  else if (formStruct.find("sécable")!=-1)        secabilite = "2";
  return codePD;
  */
    return QString::null;
}
//----------------------------------------------------- afssaps_setSpecifications ---------------------------------------------------------------------------
QString C_BaseMedica::afssaps_GetATCCode_fromLibelle(  const QString &libelle)
{
    return QString::null;
}

//----------------------------------------------------- afssaps_setSpecifications ---------------------------------------------------------------------------
QString C_BaseMedica::afssaps_SetSpecifications(  const QString &cip,
                                                  QString &up_unit,         // ok
                                                  QString &pa_unit,         // ok
                                                  QString &pa_qu,           // ok
                                                  QString &secabilite,      // ok
                                                  QString &is_generic,      // ok
                                                  QString &formStruct,      // ok
                                                  QString &nbPrises,        // ok
                                                  QString &prixBoite,       // ok
                                                  QString &remboursementSS, // ok
                                                  QString &tableau,         // ok
                                                  QString &labo,            // ok
                                                  QString &atc,             // ok
                                                  QString pkt00,            // = QString::null
                                                  QString pkt4b,            // = QString::null
                                                  QString codePD,           // = QString::null
                                                  QString codeUV            // = QString::null
                                                )
{
 return QString::null;
}

//----------------------------------------------------- Medica_CreateTableSpecificationProduit ---------------------------------------------------------------
int C_BaseMedica::Medica_CreateTableSpecificationProduit(const QString &table_name)
{//............................ effacer la table si elle existe .....................................
 if ( dropTable(table_name) ==0 )  return 0;

 //............................ creer la table  .....................................
 QString requeteDst = "";
 requeteDst += " CREATE TABLE `"  + table_name ;
 requeteDst += "` ( `" +            m_MEDICA_SPEC_CIP             + "`       char(7)  BINARY NOT NULL , "
               "    `" +            m_MEDICA_SPEC_UP_UNIT         + "`       int(11)  default NULL , "        // Pk Table forme et unites: (comprimé ampoules etc ...)   UPUnit
               "    `" +            m_MEDICA_SPEC_PA_UNIT         + "`       int(11)  default NULL , "        // Pk Table forme et unites:   Unite de principe actif  (mg ml etc...):                                PAUnit
               "    `" +            m_MEDICA_SPEC_PA_QU           + "`       float    default NULL , "        // Quantite de principe actif par unité de prise (500):                       QPAbyUP
               "    `" +            m_MEDICA_SPEC_SECABILITE      + "`       char(1)  BINARY default NULL, "  // 0 (insecable) 2 (en 2) 3 (en 3)  etc..
               "    `" +            m_MEDICA_SPEC_IS_GENERIC      + "`       char(2)  BINARY default NULL, "  // G  pour genérique R pour réference d'un groupe ATC
               "    `" +            m_MEDICA_SPEC_FORM_STRUC      + "`       int(11)  default NULL, "         // Pk Table forme et unites: forme structurée (solution buvable injectable etc ...)
               "    `" +            m_MEDICA_SPEC_NB_FORME        + "`       int(11)  default NULL, "
               "    `" +            m_MEDICA_SPEC_PRIX_BOITE      + "`       float    default NULL, "
               "    `" +            m_MEDICA_SPEC_REMBOURSEMENT   + "`       char(32) BINARY default NULL, "
               "    `" +            m_MEDICA_SPEC_TABLEAU         + "`       char(1)  BINARY default NULL, "
               "    `" +            m_MEDICA_SPEC_LABO            + "`       char(45) BINARY default NULL, "
               "    `" +            m_MEDICA_SPEC_ATC             + "`       char(7)  BINARY default NULL, "
               "     PRIMARY KEY("+ m_MEDICA_SPEC_CIP             + ") "
               "  )";
 return createTable(requeteDst);
}

//----------------------------------------------------- Medica_CreateTableIndexProduit ---------------------------------------------------------------
int C_BaseMedica::Medica_CreateTableIndexProduit(const QString &table_name)
{
 //............................ effacer la table si elle existe .....................................
  if ( dropTable(table_name) ==0 )  return 0;

 //............................ creer la table  .....................................
 QString requeteDst = "";
 requeteDst  = "";
 requeteDst += " CREATE TABLE `"  + table_name ;
 requeteDst += "` ( `" +            m_MEDICA_INDEX_PK       + "`       int(11)     NOT NULL auto_increment, "
               "    `" +            m_MEDICA_INDEX_LIBELLE  + "`       varchar(45) BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_CIP      + "`       char(7)     BINARY default NULL , "
               "    `" +            m_MEDICA_INDEX_PK_T00   + "`       int(11)     NOT NULL , "
               "    `" +            m_MEDICA_INDEX_PK_T4B   + "`       int(11)     default NULL , "
               "    `" +            m_MEDICA_INDEX_CD_PROD  + "`       char(6)     BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_CD_UV    + "`       char(10)    BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_IS_GENE  + "`       char(1)     BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_IS_DISPO + "`       char(4)     BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_RCP      + "`       char(10)    BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_CIS      + "`       char(10)    BINARY default NULL, "
               "     PRIMARY KEY("+ m_MEDICA_INDEX_PK       + ") "
               "  )";
 return createTable(requeteDst);
}

//--------------------------- createRecordIndexProduit -----------------------------------------------------------
QString C_BaseMedica::createRecordIndexProduit(const QString &cip,
                                               const QString &libelle,
                                               const QString &pk_T00,
                                               const QString &pk_T4B,
                                               const QString &cd_prod,
                                               const QString &cd_uv,
                                               const QString &is_gene,
                                               const QString &is_dispo,
                                               const QString &rcp,
                                               const QString &cis
                                              )
{
    /*
    if (!isInOwnerList(owner))
       {qDebug(tr("ERREUR  : C_BaseCommon::createGroupRowInBase() \nThis owner : '%1' is not found").arg(owner).toAscii());
        return QString::null;
       }
    */
    QString prepare  = "";
            prepare  = "INSERT INTO `"+ m_MEDICA_INDEX_MED_TBL_NAME +"` ( "     +
                        "`"+ m_MEDICA_INDEX_CIP      +"`,"+
                        "`"+ m_MEDICA_INDEX_LIBELLE  +"`,"+
                        "`"+ m_MEDICA_INDEX_PK_T00   +"`,"+
                        "`"+ m_MEDICA_INDEX_PK_T4B   +"`,"+
                        "`"+ m_MEDICA_INDEX_CD_PROD  +"`,"+
                        "`"+ m_MEDICA_INDEX_CD_UV    +"`,"+
                        "`"+ m_MEDICA_INDEX_IS_GENE  +"`,"+
                        "`"+ m_MEDICA_INDEX_IS_DISPO +"` "+
                        "`"+ m_MEDICA_INDEX_RCP      +"` "+
                        "`"+ m_MEDICA_INDEX_CIS      +"` "+
                        ") VALUES (?,?,?,?,?,?,?,?,?,?)";
            if (!database().isOpen() && database().open()== false) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, cip);
    query.bindValue(1, libelle);
    query.bindValue(2, pk_T00);
    query.bindValue(3, pk_T4B);
    query.bindValue(4, cd_prod);
    query.bindValue(5, cd_uv);
    query.bindValue(6, is_gene);
    query.bindValue(7, is_dispo);
    query.bindValue(8, rcp);
    query.bindValue(9, cis);
    if ( !query.exec())
       {outMessage(  tr("ERREUR  : C_BaseMedica::createRecordIndexProduit \n%1\nREQUETE : %2").arg(query.lastError().text(),prepare).toLatin1());
        return QString::null;
       }
    return getLastPrimaryKey( m_MEDICA_INDEX_MED_TBL_NAME, m_MEDICA_INDEX_PK);
}


//--------------------------- getBiblioDataFromShortCIP -----------------------------------------------------------
QString C_BaseMedica::biblio_getDataFromShortCIP(const QString &cip)
{ return getDCI_CompositionFromCIP(cip);
}

//--------------------------- getDCI_CompositionFromCIP -----------------------------------------------------------
QString C_BaseMedica::getDCI_CompositionFromCIP(const QString &cip)
{QString cis = get_CIS_From_CIP(cip);
 if (cis.length()==0) return    QString::null;
 QStringList dciList = isThisValueInTable_ToList("afs_comp", "afs_comp_cis_code", cis, "afs_comp_libelle");
 QString ret = QString::null;
 for (int i=0; i<dciList.count(); ++i)
     {ret += dciList[i]+"<br>";
     }
 return ret;
}
//--------------------------- get_CIS_From_CIP -----------------------------------------------------------
QString C_BaseMedica::get_CIS_From_CIP(const QString &cip)
{QStringList list = isThisValueInTable_ToList("afs_ciscip", "afs_ciscip_cip_code", cip,     "afs_ciscip_cis_code");
 if (list.count()) return list[0];
 return QString::null;
}
//--------------------------- get_CIP_From_CIS -----------------------------------------------------------
QString C_BaseMedica::get_CIP_From_CIS(const QString &cis)
{QStringList list = isThisValueInTable_ToList("afs_ciscip", "afs_ciscip_cis_code", cis,     "afs_ciscip_cip_code");
 if (list.count()) return list[0];
 return QString::null;
}

//----------------------------------------------------- cis_to_libelle_atc ---------------------------------------------------------------
QString C_BaseMedica::cis_to_libelle_atc(const QString &cis)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cip_to_libelle_atc ---------------------------------------------------------------
QString C_BaseMedica::cip_to_libelle_atc(const QString &cip)
{QString requete  =      QString("SELECT  CLASSE_ATC FROM BDM_CIP WHERE CIP7 ='%1'").arg(cip);
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : cip_to_libelle_atc()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next()) {return query.value(0).toString();}
    }
 return QString::null;
}
//----------------------------------------------------- cis_to_ucd ---------------------------------------------------------------
QString C_BaseMedica::cis_to_ucd(const QString &cis)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cip_to_ucd ---------------------------------------------------------------
QString C_BaseMedica::cip_to_ucd(const QString &cip)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cip_to_price ---------------------------------------------------------------
QString C_BaseMedica::cip_to_price(const QString &cip)
{QString requete  =      QString("SELECT  PRIX_E FROM BDM_PRIX WHERE CIP7 ='%1'").arg(cip);
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : cip_to_price()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next()) {return query.value(0).toString();}
    }
 return QString::null;
}

//----------------------------------------------------- ucd_to_price ---------------------------------------------------------------
QString C_BaseMedica::ucd_to_price(const QString &ucd)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cis_to_smr ---------------------------------------------------------------
QString C_BaseMedica::cis_to_smr(const QString &cis)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cip_to_smr ---------------------------------------------------------------
QString C_BaseMedica::cip_to_smr(const QString &cip)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cis_to_asmr ---------------------------------------------------------------
QString C_BaseMedica::cis_to_asmr(const QString &cis)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cip_to_asmr ---------------------------------------------------------------
QString C_BaseMedica::cip_to_asmr(const QString &cip)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cis_to_spec ---------------------------------------------------------------
QString C_BaseMedica::cis_to_spec(const QString &cis)
{QString ret = "";
 return ret;
}
//----------------------------------------------------- cip_to_spec ---------------------------------------------------------------
QString C_BaseMedica::cip_to_spec(const QString &cip)
{QString ret = "";
 return ret;
}

//--------------------------- get_RCP_From_CIS -----------------------------------------------------------
QString C_BaseMedica::get_RCP_From_CIS(const QString &cis)
{QStringList list = isThisValueInTable_ToList("afs_cis", "afs_cis_code", cis,  "afs_cis_rcp");
 if (list.count()) return list[0];
 return QString::null;
}

//--------------------------- biblio_setLink_Pages -----------------------------------------------------------
/*! \brief  recherche toutes les pages possibles pour tous les produits de la liste sur une url distante
*   \param  const QString &biblioType           type de donnee ( "AFSSAPS RCP FOR CIP"  "VIDAL MONO FOR CIP"
*   \param  const QString &urlIn                url ou doit se faire la recherche avec les variables tags {{ATC}} {{RCP}} {{CIS}} {{CIP7]] {{DCI}} {{NAME}}
*   \param  const QString &owner                proprietaire des donnees
*   \return
*/

long C_BaseMedica::biblio_setLink_Pages(const QString &biblioType,  QString urlIn, const QString &owner)
{    // {{ATC}} {{RCP}} {{CIS}} {{CIP7]] {{DCI}} {{NAME}}
     if (m_pQProgressBar)
        {m_position = 0;
         m_pQProgressBar->setRange(0,(int)(countRecords("BDM_CIP")/10));
        }
     QString       cip      = "";
     QString       cis      = "";
     QString       rcp      = "";
     QString       url      = "";
     long            i      = 0;
     long           nb      = 0;
     QString       requete  =  QString("SELECT  CIP7, NOM_COURT, CODE_ATC FROM BDM_CIP WHERE CODE_ATC <>'Z' AND CODE_ATC <>''");
     QSqlQuery query (requete , database() );
     outSQL_error( query, "ERREUR  : C_BaseMedica::biblio_setLink_Pages()", requete, __FILE__, __LINE__);
     if (query.isActive() )
        {while (query.next())
               {++ nb;
                cip = query.value(0).toString();
                cis = get_CIS_From_CIP(cip);
                rcp = get_RCP_From_CIS(cis);
                url = urlIn; url.replace("{{ATC}}",query.value(2).toString()); url.replace("{{RCP}}",rcp); url.replace("{{CIS}}",cis); url.replace("{{CIP7}}",cip);
                //if (rcp.length())
                   {++i;
                    biblio_setRecord(cip,
                                     owner,
                                     "fr",
                                      url,
                                     QDate::currentDate ().toString("dd-MM-yyyy"),
                                     tr("RCP for : %1 CIP : %2").arg(CGestIni::Utf8_Query(query, 1), cip),
                                     biblioType,     //"AFSSAPS RCP FOR CIP",
                                     "text/html",
                                     "");
                   }
                if (m_pQProgressBar)  {m_pQProgressBar->setValue(m_position/10); qApp->processEvents();qApp->processEvents();}
                ++ m_position;
               }    // end while (query.next())
        }  // endif (query.isActive() )
     outMessage(  tr("Nombre de %1 RCP trouv\303\251s sur %2 enregistrements.").arg(QString::number(i),QString::number(nb)));
     return i;
}

//--------------------------- setLinkRCP -----------------------------------------------------------
long C_BaseMedica::setLinkRCP()
{
     if (m_pQProgressBar)
        {m_position = 0;
         m_pQProgressBar->setRange(0,(int)(countRecords("BDM_CIP")/10));
        }
     QString       cip      = "";
     QString       cis      = "";
     QString       rcp      = "";
     long            i      = 0;
     long           nb      = 0;
     QString       requete  =  QString("SELECT  CIP7, NOM_COURT FROM BDM_CIP WHERE CODE_ATC <>'Z' AND CODE_ATC <>''");
     QSqlQuery query (requete , database() );
     outSQL_error(query, "ERREUR  : setLinkRCP()", requete, __FILE__, __LINE__);
     if (query.isActive() )
        {while (query.next())
               {++ nb;
                cip = query.value(0).toString();
                cis = get_CIS_From_CIP(cip);
                rcp = get_RCP_From_CIS(cis);
                if (rcp.length())
                   {++i;
                    biblio_setRecord(cip,
                                     "AF",
                                     "fr",
                                     QString ("http://afssaps-prd.afssaps.fr/php/ecodex/notice/N%1.htm").arg(rcp),
                                     QDate::currentDate ().toString("dd-MM-yyyy"),
                                     tr("RCP for : %1 CIP : %2").arg(CGestIni::Utf8_Query(query, 1), cip),
                                     "AFSSAPS RCP FOR CIP",
                                     "text/html",
                                     "");
                   }
                if (m_pQProgressBar)  {m_pQProgressBar->setValue(m_position/10); qApp->processEvents();qApp->processEvents();}
                ++ m_position;
               }    // end while (query.next())
        }  // endif (query.isActive() )
     outMessage( tr("Nombre de %1 RCP trouv\303\251s sur %2 enregistrements.").arg(QString::number(i),QString::number(nb)));
     return i;
}
/*
//--------------------------- biblio_ReindexeFK -----------------------------------------------------------
// n'a servi qu'une fois pour reindexer les clef etrangere de header avec celle du blob
int C_BaseMedica::biblio_ReindexeFK()
{ //....................... recuperer le HEADER ............................................
  int position = 0;
  if (m_pQProgressBar) m_pQProgressBar->setRange(0,(int)(countRecords(m_BIBLIO_H_TBL_NAME)/10));

  QString requete = QString("SELECT  `") + m_BIBLIO_H_PK + "` FROM `" + m_BIBLIO_H_TBL_NAME + "`";
  QString head_pk = "";
  QString blob_pk = "";
  QSqlQuery query (database());
  if (!query.exec(requete))
     {outSQL_error( query, "ERREUR  : C_BaseMedica::biblio_ReindexeFK() H", requete, __FILE__, __LINE__);
      return 0;
     }

  if (query.isActive() )
     {while (query.next())
             {head_pk = query.value(0).toString();                                                         // on recupere le pk du header
              blob_pk = isThisValueInTable (m_BIBLIO_B_TBL_NAME, m_BIBLIO_B_RPK, head_pk, m_BIBLIO_B_PK);  // on cherche  le pk du blob avec le pk du header
              if (blob_pk.length())
                 {updateValueInTable( m_BIBLIO_H_TBL_NAME, m_BIBLIO_H_BLOB_FK, blob_pk, head_pk );         // on met a jour la clef etrangere du header avec le pk du blob
                 }
              ++position;
              if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
             }
     }
  return position;
}
*/
//--------------------------- biblio_setLink_Pages -----------------------------------------------------------
/*! \brief  cree l'enregistrement des donnees bibliographiques liees a un produit identifie par son id
*   \param  const QString &id                   identifiant unique de la donnee medicament
*   \param  const QString &biblioType           type de donnee ( "AFSSAPS RCP FOR CIP"  "VIDAL MONO FOR CIP"
*   \param  const QString &urlIn                url ou doit se faire la recherche avec les variables tags {{RCP}} {{CIS}} {{CIP7}}
*   \param  const QString &owner                proprietaire des donnees
*   \param  const QString &data                 donnees
*   \return bool                                true si ok false si pas ok
*/

bool C_BaseMedica::biblio_setLink_Page(const QString &id, const QString &biblioType,  QString urlIn, const QString &owner, const QString &data)
{    // {{ATC}} {{RCP}} {{CIS}} {{CIP7]] {{DCI}} {{NAME}}
     QString       cis      = "";
     QString       rcp      = "";
     QString       url      = "";

     cis = get_CIS_From_CIP(id);      // id est CIP en attendant mieux
     rcp = get_RCP_From_CIS(cis);
     url = urlIn; url.replace("{{RCP}}",rcp); url.replace("{{CIS}}",cis); url.replace("{{CIP7}}",id);
     //if (rcp.length())
        {biblio_setRecord( id,
                           owner,
                           "fr",
                           url,
                           QDate::currentDate ().toString("dd-MM-yyyy"),
                           tr("RCP for CIS :  %1").arg(cis),
                           biblioType,     //"AFSSAPS RCP FOR CIP",
                           "text/html",
                           data);
         return true;
        }
     return false;
}

//--------------------------- biblio_setRecord -----------------------------------------------------------
QString C_BaseMedica::biblio_setRecord(const QString id,
                                       const QString owner,
                                       const QString lang,
                                       const QString url,
                                       const QString date,
                                       const QString note,
                                       const QString type,
                                       const QString mime,
                                       const QString data)
{QString pk = biblio_getPk_Record ( id,  lang, type );
 if (pk.length())
    {  return biblio_UpdateRecord (pk, id, owner, lang, url, date, note, type, mime, data);
    }
 else
    {  return biblio_AppendRecord (id, owner, lang, url, date, note, type, mime, data);
    }
}

//--------------------------- biblio_AppendRecord -----------------------------------------------------------
QString C_BaseMedica::biblio_AppendRecord(const QString id,
                                          const QString owner,
                                          const QString lang,
                                          const QString url,
                                          const QString date,
                                          const QString note,
                                          const QString type,
                                          const QString mime,
                                          const QString data)
{   QString prepare  = "";
    prepare         += "INSERT INTO "  + m_BIBLIO_H_TBL_NAME  + " ( \n";
    prepare         +=  m_BIBLIO_H_OWNER        + " ,\n"; // 0
    prepare         +=  m_BIBLIO_H_LANG         + " ,\n"; // 1
    prepare         +=  m_BIBLIO_H_URL          + " ,\n"; // 2
    prepare         +=  m_BIBLIO_H_ID           + " ,\n"; // 3
    prepare         +=  m_BIBLIO_H_DATE         + " ,\n"; // 4
    prepare         +=  m_BIBLIO_H_NOTE         + " ,\n"; // 5
    prepare         +=  m_BIBLIO_H_TYPE         + " ,\n"; // 6
    prepare         +=  m_BIBLIO_H_MIME         + "  \n"; // 7
    prepare         +=                            " )\n  VALUES (?,?,?,?,?,?,?,?) ";
    QSqlQuery query(database());
    if (!query.prepare(prepare))
       {outSQL_error( query, "ERREUR  : biblio_AppendRecord() prepare", prepare, __FILE__, __LINE__);
        return QString::null;
       }
    query.bindValue(0, owner);
    query.bindValue(1, lang);
    query.bindValue(2, url);
    query.bindValue(3, id);
    query.bindValue(4, date);
    query.bindValue(5, note);
    query.bindValue(6, type);
    query.bindValue(7, mime);
    if (!query.exec())
       {outSQL_error( query, "ERREUR  : biblio_AppendRecord() exec", prepare, __FILE__, __LINE__);
        return QString::null;
       }
    //.................. verifier si la creation du header a bien fonctionne ....................
    QString head_pk  =  biblio_getPk_Record ( id,  lang, type );
    if (head_pk.length()==0)
       {return QString::null;
       }
    //.................. creer le blob ..........................................................
    QString blob_pk =  biblio_setBlob( head_pk, owner, data);
    //................. si Ok updater la clef etrangere du header avec la clef du blob ..........
    if (blob_pk.length() &&
        updateValueInTable( m_BIBLIO_H_TBL_NAME, m_BIBLIO_H_BLOB_FK, blob_pk, head_pk )
       )
       {return blob_pk;
       }
    //.................. si pas OK effacer le header .............................................
    deleteRecordInTable(m_BIBLIO_H_TBL_NAME, head_pk);
    return QString::null;
}

//--------------------------- biblio_UpdateRecord -----------------------------------------------------------
QString C_BaseMedica::biblio_UpdateRecord(const QString pk,
                                          const QString id,
                                          const QString owner,
                                          const QString lang,
                                          const QString url,
                                          const QString date,
                                          const QString note,
                                          const QString type,
                                          const QString mime,
                                          const QString data)
{
    QString prepare  = QString("UPDATE ")        + m_BIBLIO_H_TBL_NAME   + " SET \n";
    prepare         +=  m_BIBLIO_H_OWNER         + "=?  ,\n"; // 0
    prepare         +=  m_BIBLIO_H_LANG          + "=?  ,\n"; // 1
    prepare         +=  m_BIBLIO_H_URL           + "=?  ,\n"; // 2
    prepare         +=  m_BIBLIO_H_ID            + "=?  ,\n"; // 3
    prepare         +=  m_BIBLIO_H_DATE          + "=?  ,\n"; // 4
    prepare         +=  m_BIBLIO_H_NOTE          + "=?  ,\n"; // 5
    prepare         +=  m_BIBLIO_H_TYPE          + "=?  ,\n"; // 6
    prepare         +=  m_BIBLIO_H_MIME          + "=?   \n"; // 7
    prepare         += " WHERE " + m_BIBLIO_H_PK + " ='"  + pk  + "'";
    QSqlQuery query(database());
    if (!query.prepare(prepare))
       {outSQL_error(query, "ERREUR  : biblio_UpdateRecord() prepare", prepare, __FILE__, __LINE__);
        return QString::null;
       }
    query.bindValue(0, owner);
    query.bindValue(1, lang);
    query.bindValue(2, url);
    query.bindValue(3, id);
    query.bindValue(4, date);
    query.bindValue(5, note);
    query.bindValue(6, type);
    query.bindValue(7, mime);
    //outMessage( tr("<b>-      -C_BaseMedica::biblio_UpdateRecord()</b> :%1=%2 %3=%4------").arg(m_BIBLIO_H_MIME, mime));

    if (!query.exec())
       {outSQL_error(query, "ERREUR  : biblio_UpdateRecord() exec", prepare, __FILE__, __LINE__);
        return QString::null;
       }
    return biblio_setBlob( pk,  owner, data);
}


//--------------------------- biblio_setBlob -----------------------------------------------------------
// bdt_biblio_h_bdt_biblio_b_fk
QString C_BaseMedica::biblio_setBlob(  const QString head_Pk, const QString ownerIn, const QString data)
{//............ regarder si blob pour le Pk de ce header (alors on update).................
 QString blob_pk = isThisValueInTable (m_BIBLIO_B_TBL_NAME, m_BIBLIO_B_RPK, head_Pk, m_BIBLIO_B_PK);
 QString owner   = ownerIn;
 if (owner.length()==0) owner = isThisValueInTable (m_BIBLIO_B_TBL_NAME, m_BIBLIO_B_RPK, head_Pk, m_BIBLIO_B_OWNER);
 if (blob_pk.length())
    {  QString prepare  = QString("UPDATE ")          + m_BIBLIO_B_TBL_NAME   + " SET \n";
       prepare         +=  m_BIBLIO_B_BLOB            + "=?, \n";
       prepare         +=  m_BIBLIO_B_OWNER           + "=?  \n";
       prepare         += " WHERE " + m_BIBLIO_B_PK   + " ='"  + blob_pk  + "'";
       QSqlQuery query(database());
       if (!query.prepare(prepare))
          {outSQL_error( query, "ERREUR  : biblio_setBlob UPDATE () prepare", prepare, __FILE__, __LINE__);
           return QString::null;
          }
       query.bindValue(0, data);
       query.bindValue(1, owner);
       if (!query.exec())
          {outSQL_error( query, "ERREUR  : biblio_setBlob UPDATE ()  exec", prepare, __FILE__, __LINE__);
           return QString::null;
          }
       return blob_pk;
    }
 else
    {  QString prepare  = "";
       prepare         += "INSERT INTO "           + m_BIBLIO_B_TBL_NAME  + " ( \n";
       prepare         +=  m_BIBLIO_B_BLOB         + "  ,\n";
       prepare         +=  m_BIBLIO_B_RPK          + "  ,\n";
       prepare         +=  m_BIBLIO_B_OWNER        + "   \n";
       prepare         +=                                                   " )\n  VALUES (?,?,?) ";
       QSqlQuery query(database());
       if (!query.prepare(prepare))
          {outSQL_error( query, "ERREUR  : biblio_setBlob INSERT() prepare", prepare, __FILE__, __LINE__);
           return QString::null;
          }
       query.bindValue(0, data);
       query.bindValue(1, head_Pk);
       query.bindValue(2, owner);
       if (!query.exec())
          {outSQL_error( query, "ERREUR  : biblio_AppendRecord() exec", prepare, __FILE__, __LINE__);
           return QString::null;
          }
       //............. retourner la clef primaire du blob (soit nouvellement cree soit mis a jour) ............
       return  isThisValueInTable (m_BIBLIO_B_TBL_NAME, m_BIBLIO_B_RPK, head_Pk, m_BIBLIO_B_PK); // recuperer le pk de blob pour le refPk du header
    }
 return QString::null;
}

//--------------------------- biblio_DeleteRecords -----------------------------------------------------------
/*! \brief  delete biblio records
*   \param  const QString &id          id drugs from which records must be deleted if empty all records whith type and lang will be deleted
*   \param  const QString &lang        langue exp : "fr"
*   \param  const QString &type        record type exp : "Afssaps" "Afssaps RCP For CIP" etc..
*   \return int                        number of deleted records
*/

int  C_BaseMedica::biblio_DeleteRecords(const QString &id, const QString &lang,  const QString &type)
{int nb_deleted  =  0;
 QString head_pk = "";
 QString blob_fk = "";
 //.................................... les blobs ..........................................................
 QString          requete  =  QString("SELECT  ")                 +
                                "`"         + m_BIBLIO_H_PK       + "`,"      // 0 primary key de la table des bibliographies
                                "`"         + m_BIBLIO_H_BLOB_FK  + "` "      // 1 clef etrangere sur le Blob des donnees
                                "  FROM  `" + m_BIBLIO_H_TBL_NAME + "`"
                                "  WHERE `" + m_BIBLIO_H_LANG     + "` = \"" + lang   + "\"";
 if (id.length()) requete +=    "  AND   `" + m_BIBLIO_H_ID       + "` = \"" + id     + "\"";
                  requete +=    "  AND   `" + m_BIBLIO_H_TYPE     + "` = \"" + type   + "\""
                          + ownersSelectMention(m_BIBLIO_H_TBL_NAME);
 QSqlQuery query (database());
 if (!query.exec(requete))
    {outSQL_error( query, "ERREUR  : C_BaseMedica::biblio_DeleteRecords()", requete, __FILE__, __LINE__);
     return nb_deleted;
    }
 if (query.isActive() )
    {while (query.next())
           {head_pk    = query.value(0).toString();
            blob_fk    = query.value(1).toString();
            biblio_DeleteBlobRecord(blob_fk);
            ++nb_deleted;
           }
    }
 //..................... les headers ...............................................................
                  requete  =  QString("DELETE  FROM `%1` ").arg(m_BIBLIO_H_TBL_NAME);
                  requete +=    "  WHERE `" + m_BIBLIO_H_LANG     + "` = \"" + lang   + "\"";
 if (id.length()) requete +=    "  AND   `" + m_BIBLIO_H_ID       + "` = \"" + id     + "\"";
                  requete +=    "  AND   `" + m_BIBLIO_H_TYPE     + "` = \"" + type   + "\""
                          + ownersSelectMention(m_BIBLIO_H_TBL_NAME);
 if (!query.exec(requete))
    {outSQL_error( query, "ERREUR  : C_BaseMedica::biblio_DeleteRecords(headers)", requete, __FILE__, __LINE__);
     return nb_deleted;
    }
 return nb_deleted;
}

//--------------------------- biblio_DeleteBlobRecord -----------------------------------------------------------
bool   C_BaseMedica::biblio_DeleteBlobRecord(const QString &blob_pk )
{
 QSqlQuery query (database());
 QString requete  =  QString(" DELETE  FROM `%1` ").arg(m_BIBLIO_B_TBL_NAME)       +
                             " WHERE `" + m_BIBLIO_B_PK     + "` = \"" + blob_pk   + "\""
                             + ownersSelectMention(m_BIBLIO_B_TBL_NAME);
 if (!query.exec(requete))
    {outSQL_error( query, "ERREUR  : C_BaseMedica::biblio_DeleteBlobRecord()", requete, __FILE__, __LINE__);
     return false;
    }
 return true;
}

//--------------------------- biblio_getDataFrom_TypeLangId -----------------------------------------------------------
C_BiblioData C_BaseMedica::biblio_getDataFrom_TypeLangId(const QString id,
                                                         const QString lang,
                                                         const QString type)
{C_BiblioData cb;
 QString head_pk = biblio_getPk_Record( id, lang, type  );
 //outMessage( tr("<b>----- C_BaseMedica::biblio_getDataFrom_TypeLangId()</b> :head_pk=%4 for id=%1 lang=%2 type=%3------").arg(id,lang,type,head_pk));
 if (head_pk.length()==0) return cb;
 cb = biblio_getBiblioDataFromHeadPk(head_pk);
 //if (cb.m_data.length()) outMessage( tr("<b>.        <b>cb.m_data=<b><font color=#00ff00> %1 </font>").arg(cb.m_data.left(20)));
 //else                    outMessage( tr("<b>.        <b>cb.m_data=<b><font color=#00ff00> %1 </font>").arg("not found"));
 return cb;
}

//--------------------------- biblio_getBiblioDataFromHeadPk -----------------------------------------------------------
C_BiblioData C_BaseMedica::biblio_getBiblioDataFromHeadPk(const QString head_pk)
{C_BiblioData cb;
 QString mime = "";
 QString blob_fk = "";

    //....................... recuperer le HEADER ............................................
 QString       requete  =  QString("SELECT  ")                         +
                                    + "`"        + m_BIBLIO_H_PK       + "` ," +     // 0 primary key de la table des bibliographies
                                    + "`"        + m_BIBLIO_H_OWNER    + "` ," +     // 1 proprio de la table des bibliographies
                                    + "`"        + m_BIBLIO_H_LANG     + "` ," +     // 2 langue
                                    + "`"        + m_BIBLIO_H_URL      + "` ," +     // 3 url
                                    + "`"        + m_BIBLIO_H_ID       + "` ," +     // 4 identificateur lie a cet enregistrement
                                    + "`"        + m_BIBLIO_H_DATE     + "` ," +     // 5 date de l'enregistrement
                                    + "`"        + m_BIBLIO_H_NOTE     + "` ," +     // 6 note concernant cet enregisrement
                                    + "`"        + m_BIBLIO_H_TYPE     + "` ," +     // 7 type de cet enregisteement "AFSSAPS RCP CIP"
                                    + "`"        + m_BIBLIO_H_MIME     + "` ," +     // 8 mime type de cet enregistrement
                                    + "`"        + m_BIBLIO_H_BLOB_FK  + "`  " +     // 9 clef etrangere sur le Blob des donnees
                                    + "  FROM `" + m_BIBLIO_H_TBL_NAME + "` WHERE `" + m_BIBLIO_H_PK + "` = " + head_pk + ownersSelectMention(m_BIBLIO_H_TBL_NAME);

 QSqlQuery query (database());
 if (!query.exec(requete))
    {outSQL_error( query, "ERREUR  : biblio_getBiblioDataFromRPk() H", requete, __FILE__, __LINE__);
     return cb;
    }
 /*
               C_BiblioData( const QString pk,      // 0
                             const QString owner,   // 1
                             const QString lang,    // 2
                             const QString url,     // 3
                             const QString id,      // 4
                             const QString date,    // 5
                             const QString note,    // 6
                             const QString type,    // 7
                             const QString mime,    // 8
                             const QString blob_pk, // 9
                             const QString data     // 10
                           )
                              */
 if (query.isActive() )
    {while (query.next())
           {mime    = query.value(8).toString();
            blob_fk = query.value(9).toString();
            //outMessage( tr("<b>.       mime field    : <b>%1=<b><font color=#00ff00> %2 </font>").arg(m_BIBLIO_H_MIME,mime));
            //outMessage( tr("<b>.       blob_fk field : <b>%1=<b><font color=#00ff00> %2 </font>").arg(m_BIBLIO_H_BLOB_FK,blob_fk));
            cb = C_BiblioData( query.value(0).toString(),     // primary key de la table des bibliographies
                               query.value(1).toString(),     // proprio de la table des bibliographies
                               query.value(2).toString(),     // langue
                               query.value(3).toString(),     // url
                               query.value(4).toString(),     // identificateur lie a cet enregistrement
                               query.value(5).toString(),     // date de l'enregistrement
                               query.value(6).toString(),     // note concernant cet enregisrement
                               query.value(7).toString(),     // type de cet enregisteement "AFSSAPS RCP CIP"
                               query.value(8).toString(),     // mime type de cet enregistrement
                               query.value(9).toString(),     // clef etrangere sur le Blob des donnees
                               ""
                             );
           }
    }
 //outMessage( tr(".     <b>cb.m_blob_pk=</b><font color=#ffffff>%1</font>").arg(cb.m_blob_pk));
 //....................... recuperer le BLOB ............................................
 requete  =  QString( QString("SELECT `") +  m_BIBLIO_B_BLOB  + "` FROM `" + m_BIBLIO_B_TBL_NAME + "` WHERE "
                                       "`"+  m_BIBLIO_B_PK    +"`  = \"%1\" " ).arg(cb.m_blob_pk) + ownersSelectMention(m_BIBLIO_B_TBL_NAME);
 if (!query.exec(requete))
    {outSQL_error( query, "ERREUR  : biblio_getBiblioDataFromRPk() B", requete, __FILE__, __LINE__);
     return cb;
    }
  if (query.isActive() )
     {while (query.next())
            {cb.m_data = CGestIni::Utf8_Query(query, 0);
            }
     }
  return cb;
}

//--------------------------- biblio_getBlobDataFromHeadPk -----------------------------------------------------------
QString C_BaseMedica::biblio_getBlobDataFromHeadPk(const QString head_pk)
{QString       requete  =  QString( QString("SELECT `") +  m_BIBLIO_B_BLOB  + "` FROM `" + m_BIBLIO_B_TBL_NAME + "` WHERE "
                                   "`"+ m_BIBLIO_B_PK  +"`  = \"%1\" "
                                   ).arg(head_pk) + ownersSelectMention(m_BIBLIO_B_TBL_NAME);
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : biblio_getDataFromRPk()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           {return query.value(0).toString();
           }
     }
 return QString::null;
}

//--------------------------- biblio_getPk_Record -----------------------------------------------------------
QString C_BaseMedica::biblio_getPk_Record(const QString id,
                                          const QString lang,
                                          const QString type
                                         )
{QString       requete  =  QString( QString("SELECT `") +  m_BIBLIO_H_PK  + "` FROM `" + m_BIBLIO_H_TBL_NAME + "` WHERE "
                                   "`"+ m_BIBLIO_H_LANG +"`  = \"%1\" AND "
                                   "`"+ m_BIBLIO_H_ID   +"`  = \"%2\" AND "
                                   "`"+ m_BIBLIO_H_TYPE +"`  = \"%3\"     "
                                   ).arg(lang, id, type) + ownersSelectMention(m_BIBLIO_H_TBL_NAME);
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : biblio_getPk_Record()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           {return query.value(0).toString();
           }
     }
 return QString::null;
}

//--------------------------- initInteractionDataBase -----------------------------------------------------------
int C_BaseMedica::initInteractionDataBase( const QString &fileName,
                                           QTreeWidget   *pQTreeWidget,
                                           QTreeWidget   *pQTreeWidgetGroup,
                                           QTreeWidget   *pQTreeWidgetSynthese,
                                           QLabel        *plabelSynthese,
                                           const QString &owner
                                          )
{   addOwner(owner);
    QString composant       ="";
    QString interactant     =" ";
    QString lineTrm         ="";
    int     posInterRes     = -1;
    int     pos             = -1;
    QString s               = "";
    int     position        = 0;
    QTreeWidgetItem *pItemComposant = 0;
    QTreeWidgetItem *pItemInteract  = 0;
    QByteArray   ba;
    QStringList  membreList;
    int posFirstNSpace;
    //.............. menage .....................
    pQTreeWidget->clear();
    pQTreeWidgetGroup->clear();
    pQTreeWidgetSynthese->clear();
    //....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fileName ) )            return 0;
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly  ) )    return 0;
    outMessage( tr("----------  Cr\303\251ation des tables d'interactions <font color=#01eaff><b>%1</b></font> en cours ----------------").arg( "it_compo, it_group, it_inter, it_link"));
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(int)(file.size()/10));
    plabelSynthese->setText(tr("Analyse de premier niveau (isoler les composants)"));
    outMessage( tr("%1Analyse de premier niveau <font color=#ffd270><b>%2</b></font>").arg(OFSET,"(isoler les composants)"));
    //...................... PARSER LE FICHIER L'ECLATER ET..............................
    //                       REMPLIR le QTreeWidget avec les infos
    QString line            = "";
    while (!file.atEnd())
          {
           ba               = file.readLine(MAX_READ);  position += ba.size(); if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
           line             = CGestIni::fromMyUTF8(ba.data());
           lineTrm          = line.trimmed();
           posFirstNSpace   = posFirstNonSpace(line);
           //................. rechercher separation mecanisme / conduite a tenir .....................
           if (posInterRes==-1) posInterRes = line.indexOf("CONTRE-INDICATION");
           if (posInterRes==-1) posInterRes = line.indexOf("Association DECONSEILLEE");
           if (posInterRes==-1) posInterRes = line.indexOf("Pr\303\251caution d'emploi");
           if (posInterRes==-1) posInterRes = line.indexOf("A prendre en compte");
           if (posInterRes==-1) posInterRes = line.indexOf("CI - ASDEC");
           if (posInterRes==-1) posInterRes = line.indexOf("CI - PE");
           if (posInterRes==-1) posInterRes = line.indexOf("CI - APEC");
           if (posInterRes==-1) posInterRes = line.indexOf("ASDEC - PE");
           if (posInterRes==-1) posInterRes = line.indexOf("ASDEC - APEC");
           //if (posInterRes==-1) posInterRes = line.indexOf("CI - ASDEC - APEC");
           //if (posInterRes==-1) posInterRes = line.indexOf("CI - ASDEC - PE");

           //............................. numero de page .............................................
           if (lineTrm.length()< 8 && lineTrm[0]>='0'&&lineTrm[0]<='9')
              {//qDebug(tr("Page : %1").arg(lineTrm).toAscii());
              }
           //............................. donnees textuelle d'un interactant .............................................
           else if (lineTrm[0] != '+' && posFirstNSpace > 8 && posInterRes>20 && interactant.length() && composant.length()==0)
              {if (pItemInteract)
                  {//................. avant posInterRes on est sur le commentaire de l'interactant ............................
                   s = pItemInteract->text(1); if (s.length()) s+="\n"; s += line.mid(posFirstNSpace,posInterRes-posFirstNSpace).trimmed();
                   pItemInteract->setText(1,s);
                   //................. apres posInterRes on est sur le commentaire de l'interaction ............................
                                                                     // le type d'interaction est normalement sur la premiere ligne apres le nom de l'interactant
                   if (pItemInteract->text(2).length()==0)           // si vide c'est qu'on est sur la premiere ligne (la ou se trouvent 'CI - APEC','Précaution d'emploi' etc)
                      {pItemInteract->setText(2,line.mid(posInterRes).trimmed());  // c'est donc renseigne donc les lignes qui suivent seront le commentaire de l'interaction
                      }
                   else
                      {s = pItemInteract->text(3); if (s.length()) s+="\n";  s += line.mid(posInterRes).trimmed();
                       pItemInteract->setText(3,s);
                      }
                  }
              }
           //....................... donnees textuelles d'un composant ................................
           else if (lineTrm[0] != '+' && posFirstNSpace < 4 && composant.length() && interactant.length()==0)
              {//qDebug(tr("=> commentaire : %1 <=").arg(lineTrm).toAscii());
               if (pItemComposant)
                  {s = pItemComposant->text(1); if (s.length()) s+="\n"; s += lineTrm;
                   pItemComposant->setText(1,s);
                  }
              }
           //..................... titre d'un composant .........................................
           else if (posFirstNSpace<1 && lineTrm[0] != '+')
              {composant   = lineTrm;
               interactant = "";
               //qDebug(tr("=> composant : %1 <=").arg(composant).toAscii());
               pItemComposant = new QTreeWidgetItem(pQTreeWidget);
               pItemComposant->setText(0, composant);
               pItemComposant->setExpanded(true);
              }
           //..................... titre d'un interactant .........................................
           else if (posFirstNSpace<2 && lineTrm[0] == '+' && interactant != lineTrm)
              {interactant = lineTrm;
               composant   = "";
               posInterRes = -1;    // reinitialiser la variable au prochain tour (normalement la prochaine ligne a venir le permet)
               //qDebug(tr("     + interactant : %1").arg(interactant).toAscii());
               if (pItemComposant)
                  {pItemInteract = new QTreeWidgetItem(pItemComposant);
                   pItemInteract->setText(0, interactant);
                  }
              }
          } // endwhile (!file.atEnd() && (tryCreate || tryInsert))

    //...................... REMPLIR LE QTreeWidget DES GROUPES ..............................
    int posFermant;
    int i;
    int n    = 0 ;
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(pQTreeWidget->topLevelItemCount()/10));
    position = 0;
    QTreeWidgetItemIterator it(pQTreeWidget);
    plabelSynthese->setText(tr("Extraction des classes d'interactions."));
    outMessage( tr("%1Extraction des <font color=#ffd270><b>%2</b></font> en cours....").arg(OFSET,"classes d'interactions"));
    while (*it)
     { s = (*it)->text(0);
       if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
       ++ position;
       if (s[0] != '+')
        {s = (*it)->text(1).trimmed();
         pos = posLastEnclosedSymbol(s,"(",")", &posFermant);
         if (pos != -1)
            {if (posFermant != -1) s.truncate(posFermant);
             ++pos;
             line = s;
             s=s.mid(pos);
             s = s.trimmed();
             if (!s.startsWith("sauf") && !line.startsWith("Voir ") && !s.startsWith("voir "))
                {pItemComposant = new QTreeWidgetItem(pQTreeWidgetGroup);  ++n;
                 pItemComposant->setText(0, (*it)->text(0).trimmed());
                 pItemComposant->setText(1, (*it)->text(1).trimmed());
                 pItemComposant->setExpanded(true);
                 membreList = s.split(',');
                 for ( i=0; i<membreList.size(); ++i)
                     {pItemInteract = new QTreeWidgetItem(pItemComposant); ++n;
                      pItemInteract->setText(0, membreList[i].trimmed());
                     }
                }
            }
        }
       ++it;
     }

    //...................... CREATION DES TABLES D'INTERACTION ..............................
    if (dropTable(  "it_compo")==0) return 0;
    if (createTable(" CREATE TABLE `it_compo` ("
                    "`it_compo_pk`          BIGINT NOT NULL AUTO_INCREMENT,"
                    "`it_compo_owner`       VARCHAR(40)   ,"
                    "`it_compo_name`        VARCHAR(255)  ,"
                    "`it_compo_descr`       VARCHAR(4048) ,"
                    "`it_compo_id_ctx`      VARCHAR(80)   ,"
                    "`it_compo_id_ctx_type` VARCHAR(40)   ,"
                    "`it_compo_class`       VARCHAR(4)    ,"
                    "`it_compo_date`        DATETIME      ,"
                    "PRIMARY KEY (`it_compo_pk`)"
                    ")")==0) return 0;
    if (dropTable(  "it_group")==0) return 0;
    if (createTable("CREATE TABLE `it_group` ("
                    "`it_group_pk`          BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`it_group_owner`       VARCHAR(40)  ,"
                    "`it_group_libelle`     VARCHAR(255) ,"
                    "`it_group_note`        TEXT         ,"
                    "`it_group_id`          CHAR(10)     ,"
                    "PRIMARY KEY (`it_group_pk`)"
                    ")"
                    )==0) return 0;
    if (dropTable(  "it_inter")==0) return 0;
    if (createTable("CREATE TABLE `it_inter` ("
                    "`it_inter_pk`           BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`it_inter_owner`        VARCHAR(40)   ,"
                    "`it_inter_regle`        VARCHAR(255)  ,"
                    "`it_inter_mecanisme`    VARCHAR(2000) ,"
                    "`it_inter_cat`          VARCHAR(2000) ,"
                    "`it_inter_date`         DATETIME      ,"
                    "PRIMARY KEY (`it_inter_pk`)"
                    ")"
                    )==0) return 0;
    if (dropTable(  "it_link")==0) return 0;
    if (createTable("CREATE TABLE `it_link` ("
                    "`it_link_pk`            BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`it_link_owner`         VARCHAR(40)      ,"
                    "`it_link_inter_pk`      BIGINT  NOT NULL ,"
                    "`it_link_compo_pk1`     BIGINT  NOT NULL ,"
                    "`it_link_compo_pk2`     BIGINT  NOT NULL ,"
                    "`it_link_class`         VARCHAR(4)       ,"
                    "`it_link_date`          DATETIME         ,"
                    "PRIMARY KEY (`it_link_pk`)"
                    ")"
                    )==0) return 0;

    //...................... REMPLIR LA TABLE DES COMPOSANTS ..............................
    //                       ET DES GROUPES DE COMPOSANTS
    QString libelle = "";
    QString pk      = "";
    QString id      = "";
    it              = QTreeWidgetItemIterator(pQTreeWidget); // pQTreeWidget // pQTreeWidgetSynthese
    i               = 0;
    position        = 0;
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(pQTreeWidget->topLevelItemCount()/10));
    plabelSynthese->setText(tr("Remplissage de la table : 'it_compo' et 'it_group'"));
    outMessage(  tr("%1Remplissage des tables <font color=#01eaff><b>%2</b></font> en cours....").arg( OFSET, "'it_compo' et 'it_group'"));
    while (*it)
     {
        if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
        ++ position;
        libelle = (*it)->text(0);                           // nom du composant
        if (libelle[0] != '+')  //<<<<<<<<<<<<<<<<< composant pere >>>>>>>>>>>>>>>>>>>>>>>>>>
        {//.............. deployer le groupe si necessaire .........................
         QTreeWidgetItem* pQTreeWidgetComposant = (*it);
         QTreeWidgetItem* pQTreeWidgetItemGroup = isThisGroupeItem( libelle, pQTreeWidgetGroup);
         QString              text_In_Composant = pQTreeWidgetComposant->text(1);
         if (pQTreeWidgetItemGroup)
            {id   = createGroupRowInBase(  libelle, pQTreeWidgetItemGroup->text(1),"-1",  owner);   // id est le pk de l'enregistrement dans la table it_group
             pk   = it_createComposant( libelle, text_In_Composant, "it_group_G", id, "ITMD" , owner);
             ++i;
             pQTreeWidgetComposant->setText(4, pk);
             int nbChilds =	pQTreeWidgetItemGroup->childCount ();
             int idChild  = 0;
             for (idChild = 0; idChild<nbChilds; ++idChild)
                 {pk   = createGroupRowInBase(  pQTreeWidgetItemGroup->child(idChild)->text(0),"", id, owner);
                  pk   = it_createComposant( pQTreeWidgetItemGroup->child(idChild)->text(0), text_In_Composant, "it_group_M", pk ,"ITMD", owner);
                  ++i;
                 }
            }
         else
            {pk   = it_createComposant( libelle.trimmed(), text_In_Composant, "it_compo", QString::number(i) , "ITMD",  owner);
             ++i;
            }
        }
       else                    //<<<<<<<<<<<<<<<<< composant fils >>>>>>>>>>>>>>>>>>>>>>>>>>
        {
            pk   = it_createComposant( libelle.mid(1).trimmed(), (*it)->text(1), "it_compo", QString::number(i), "ITMD", owner );
            ++i;
        }
       ++it;
     }

    //...................... REMPLIR LA TABLE DES INTERACTIONS ..............................
    QString libelle_interactant = "";
    it                          = QTreeWidgetItemIterator(pQTreeWidget); // pQTreeWidget // pQTreeWidgetSynthese
    i                           = 0;
    position                    = 0;
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(pQTreeWidget->topLevelItemCount()/10));
    plabelSynthese->setText(tr("Remplissage de la table : 'it_inter'"));
    outMessage( tr("%1Remplissage de la table '<font color=#01eaff><b>%2</b></font>' en cours....").arg(OFSET, "it_inter"));
    while (*it)
     { if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
       ++ position;
       libelle = (*it)->text(0);                  // nom du composant
       if (libelle[0] == '+')                     //<<<<<<<<<<<<<<<<< composant fils >>>>>>>>>>>>>>>>>>>>>>>>>>
          {libelle_interactant = libelle.mid(1).trimmed();    // virer le +
           //.............. deployer le groupe si necessaire .........................
           QTreeWidgetItem* pQTreeWidgetItemFacteurPere = (*it)->parent();   // rechercher l'objet d'interaction avec lequel cet interactant interacte
           QTreeWidgetItem* pQTreeWidgetItemInteractant = (*it);
           QTreeWidgetItem* pQTreeWidgetItemGroup       = isThisGroupeItem( libelle_interactant, pQTreeWidgetGroup);

           //............. on cree  l'interaction pour le composant lui meme ............................
           i += createInteractionForThisComposant(pQTreeWidgetSynthese,
                                                  pQTreeWidgetGroup,
                                                  pQTreeWidgetItemInteractant,
                                                  pQTreeWidgetItemFacteurPere->text(0),     // nom du pere
                                                  libelle_interactant,
                                                  owner
                                                 );                     // nom de l'interactant
           //............. on cree  l'interaction pour les eventuels alias fils ............................
           if (pQTreeWidgetItemGroup)   // on deploie pour les fils
              {
               int nbChilds =	pQTreeWidgetItemGroup->childCount ();
               int idChild  = 0;
               for (idChild = 0; idChild<nbChilds; ++idChild)
                 {i += createInteractionForThisComposant(pQTreeWidgetSynthese,
                                                         pQTreeWidgetGroup,
                                                         pQTreeWidgetItemInteractant,
                                                         pQTreeWidgetItemFacteurPere->text(0),              // nom du pere
                                                         pQTreeWidgetItemGroup->child(idChild)->text(0),
                                                         owner
                                                        );   // nom de l'interactant (ici alias du groupe)
                 }
              }
        }
       ++it;
     }

    //...................... REMPLIR LA TABLE DES LIENS PRODUITS avec INTERACTIONS ..............................
    it                          = QTreeWidgetItemIterator(pQTreeWidgetSynthese); // pQTreeWidget // pQTreeWidgetSynthese
    position                    = 0;
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(int)(pQTreeWidgetSynthese->topLevelItemCount()/10));
    plabelSynthese->setText(tr("Remplissage de la table : 'it_link'"));
    outMessage(  tr("%1Remplissage de la table '<font color=#01eaff><b>%2</b></font>' en cours....").arg( OFSET, "it_link"));
    while (*it)
     {  if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
        ++ position;
        it_createLinkFacteursInteraction((*it)->text(1), (*it)->text(3), (*it)->text(7), "ITMD", owner);
        ++it;
     }
 plabelSynthese->setText(tr("nombre d'items : %1").arg(QString::number(i)));
 outMessage( tr("----------  Fin de la cr\303\251ation des tables <font color=#01eaff><b>%1</b></font> ----------------").arg( "it_compo, it_group, it_inter, it_link"));
 return 1;
}


//----------------------------------------------------- BDM_druglist_tbl_name ---------------------------------------------------------------
QString C_BaseMedica::BDM_druglist_tbl_name(){return m_BDM_DRUGLIST_TBL_NAME;}

//----------------------------------------------------- BDM_initDrugListFromAfssapsAndBDM ---------------------------------------------------------------
int C_BaseMedica::BDM_initDrugListFromAfssapsAndBDM(   const QString &owner, const QString &lang /* = "fr" */ )
{
    //........................ MEDICA_TUX  Table de la liste des drogues .........................................
    // QString  m_BDM_DRUGLIST_TBL_NAME;      // nom de la table de la liste des medicaments
    // QString  m_BDM_DRUGLIST_PK;            // clef primaire
    // QString  m_BDM_DRUGLIST_OWNER;         // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
    // QString  m_BDM_DRUGLIST_LANG;          // langue de la donnee (en fr etc...)
    // QString  m_BDM_DRUGLIST_ID;            // identificateur unique du medicament
    // QString  m_BDM_DRUGLIST_TYPE_ID;       // type d'identificateur (CIP CIP7 CIS)
    // QString  m_BDM_DRUGLIST_ATC;           // Code ATC
    // QString  m_BDM_DRUGLIST_LIBELLE;       // libelle de la classe ATC
    // QString  m_BDM_DRUGLIST_DCI_1;         // premiere  substance DCI (libelle ATC)
    // QString  m_BDM_DRUGLIST_DCI_2;         // deuxieme  substance DCI (si produit compose)
    // QString  m_BDM_DRUGLIST_DCI_3;         // troisieme substance DCI (si produit compose)
    // QString  m_BDM_DRUGLIST_UCD;           // code UCD
    // QString  m_BDM_DRUGLIST_UCD_PRICE;     // Prix UCD
    // QString  m_BDM_DRUGLIST_SMR;           // Niveau du SMR
    // QString  m_BDM_DRUGLIST_PK_SPEC;       // pointeur sur les specifications produits
    if (!database().isOpen() && database().open()== false)
       {outMessage( tr("ERREUR : BDM_initDrugListFromAfssapsAndBDM() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    //....................... on cree la table ......................................
    if (dropTable(  m_BDM_DRUGLIST_TBL_NAME)==0) return 0;
    if (createTable("CREATE TABLE `"+m_BDM_DRUGLIST_TBL_NAME+"` ("
                    "`"+m_BDM_DRUGLIST_PK        +"` BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`"+m_BDM_DRUGLIST_OWNER     +"` VARCHAR(40) ,"
                    "`"+m_BDM_DRUGLIST_LANG      +"` VARCHAR(4)  ,"
                    "`"+m_BDM_DRUGLIST_ID        +"` VARCHAR(40) ,"
                    "`"+m_BDM_DRUGLIST_TYPE_ID   +"` VARCHAR(8)  ,"
                    "`"+m_BDM_DRUGLIST_ATC       +"` VARCHAR(7)  ,"
                    "`"+m_BDM_DRUGLIST_LIBELLE   +"` VARCHAR(128),"
                    "`"+m_BDM_DRUGLIST_DCI_1     +"` VARCHAR(128),"
                    "`"+m_BDM_DRUGLIST_DCI_2     +"` VARCHAR(128),"
                    "`"+m_BDM_DRUGLIST_DCI_3     +"` VARCHAR(128),"
                    "`"+m_BDM_DRUGLIST_UCD       +"` VARCHAR(7)  ,"
                    "`"+m_BDM_DRUGLIST_UCD_PRICE +"` BIGINT      ,"
                    "`"+m_BDM_DRUGLIST_SMR       +"` BIGINT      ,"
                    "`"+m_BDM_DRUGLIST_ASMR      +"` BIGINT      ,"
                    "`"+m_BDM_DRUGLIST_PK_SPEC   +"` BIGINT      ,"
                    "PRIMARY KEY (`"+m_BDM_DRUGLIST_PK+"`)"
                    ")"
                    )==0) return 0;
    //long nbRecordsToParse = countRecords("afs_cis", "WHERE afs_cis_amm = \"AMM active\"");
    long nbRecordsToParse = countRecords("BDM_CIP", " WHERE CODE_ATC <>'Z' AND CODE_ATC <>''");
    outMessage(  tr("Nb records <font color=#00c0ff>%1</font> : <font color=#00ffff>%2</font>").arg(m_BDM_DRUGLIST_TBL_NAME,QString::number(nbRecordsToParse)));
    long position         = 0;
    if (m_pQProgressBar) m_pQProgressBar->setRange(0, nbRecordsToParse/10);

    //......................... on renseigne la liste ...................................
    QString prepare  =              (" INSERT INTO "+m_BDM_DRUGLIST_TBL_NAME+"( "
                                     "`"+m_BDM_DRUGLIST_OWNER     +"`,"     // 0
                                     "`"+m_BDM_DRUGLIST_LANG      +"`,"     // 1
                                     "`"+m_BDM_DRUGLIST_ID        +"`,"     // 2
                                     "`"+m_BDM_DRUGLIST_TYPE_ID   +"`,"     // 3
                                     "`"+m_BDM_DRUGLIST_ATC       +"`,"     // 4
                                     "`"+m_BDM_DRUGLIST_LIBELLE   +"`,"     // 5
                                     "`"+m_BDM_DRUGLIST_DCI_1     +"`,"     // 6
                                     "`"+m_BDM_DRUGLIST_DCI_2     +"`,"     // 7
                                     "`"+m_BDM_DRUGLIST_DCI_3     +"`,"     // 8
                                     "`"+m_BDM_DRUGLIST_UCD       +"`,"     // 9
                                     "`"+m_BDM_DRUGLIST_UCD_PRICE +"`,"     // 10
                                     "`"+m_BDM_DRUGLIST_SMR       +"`,"     // 11
                                     "`"+m_BDM_DRUGLIST_ASMR      +"`,"     // 12
                                     "`"+m_BDM_DRUGLIST_PK_SPEC   +"` "     // 13
                                     ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    QStringList dciList;
    QString     libelle  = "";
    QString     libelle1 = "";
    QString     libelle2 = "";
    QString     cis_code = "";
    QString     cip_code = "";
    QString     ucd_code = "";
    QString     atc_code = "";
    QString     dci_1    = "";
    QString     dci_2    = "";
    QString     dci_3    = "";
    QString     requete  =      "SELECT  NOM_COURT, NOM_LONG1, NOM_LONG2, CIP7, CODE_ATC, CIP_UCD "
                                "FROM BDM_CIP WHERE CODE_ATC <>'Z' AND CODE_ATC <>''";
    QSqlQuery query (requete , database() );
    QSqlQuery queryInsert(QString::null , database() );
    outSQL_error( query, "ERREUR  : BDM_initDrugListFromAfssapsAndBDM()", requete, __FILE__, __LINE__);
    if (query.isActive() )
       {while (query.next() && position < 20000)
              {libelle      = CGestIni::Utf8_Query(query, 0);
               libelle1     = CGestIni::Utf8_Query(query, 1);
               libelle2     = CGestIni::Utf8_Query(query, 2);
               if (libelle1.left(8)==libelle.left(8)) libelle = libelle1;
               libelle += libelle2;
               cip_code     = query.value(3).toString(); // get_CIP_From_CIS(cis_code);
               cis_code     = get_CIS_From_CIP(cip_code);
               atc_code     = query.value(4).toString();
               ucd_code     = query.value(5).toString(); // cis_to_ucd(cis_code);
               dciList      = isThisValueInTable_ToList("afs_comp", "afs_comp_cis_code", cis_code, "afs_comp_libelle",0);
               dci_1        = "";
               dci_2        = "";
               dci_3        = "";
               if (dciList.count())
                  {if (dciList.count()>0) dci_1 = dciList[0];
                   if (dciList.count()>1) dci_2 = dciList[1];
                   if (dciList.count()>2) dci_3 = dciList[2];
                  }
               else
                  {dci_1 = cip_to_libelle_atc(cip_code);
                   if (dci_1.length()==0) outMessage(  tr("WARNING : DCI not found for : %1").arg(libelle));
                  }
               queryInsert.prepare(prepare);
               queryInsert.bindValue(0,  owner);
               queryInsert.bindValue(1,  lang);
               queryInsert.bindValue(2,  cip_code);
               queryInsert.bindValue(3,  "CIP");
               queryInsert.bindValue(4,  atc_code);
               queryInsert.bindValue(5,  libelle);
               queryInsert.bindValue(6,  dci_1);
               queryInsert.bindValue(7,  dci_2);
               queryInsert.bindValue(8,  dci_3);
               queryInsert.bindValue(9,  ucd_code);
               queryInsert.bindValue(10, cip_to_price(cip_code));
               queryInsert.bindValue(11, cis_to_smr(cis_code));
               queryInsert.bindValue(12, cis_to_asmr(cis_code));
               queryInsert.bindValue(13, cis_to_spec(cis_code));
               if ( !queryInsert.exec())
                  {outSQL_error( queryInsert, "ERREUR  : C_BaseMedica::it_createLinkFacteursInteraction()", prepare, __FILE__, __LINE__);
                   return 0;
                  }
               ++position;
               if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
               //item->setText(2,query.value(2).toString());
              }    // end while (query.next())
       }  // endif (query.isActive() )
    return 1;
}

//----------------------------------------------------- cis_to_atc ---------------------------------------------------------------
QString C_BaseMedica::cis_to_atc(const QString &cis)
{QString cip = get_CIP_From_CIS(cis);
 return cip_to_atc(cip, cis);
}

//----------------------------------------------------- cis_to_atc ---------------------------------------------------------------
QString C_BaseMedica::cip_to_atc(const QString &cip, QString cis /* = "" */)
{
 QString atc         = "";
 QString requete     =  QString("SELECT CODE_ATC FROM BDM_CIP WHERE  CIP7 ='%1'").arg(cip);
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : 1 cis_to_atc()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           {atc = query.value(0).toString();
            if (atc.length()) return atc;
           }
    }
 return atc;
}

//----------------------------------------------------- fill_treeWidget_ClassesIt ---------------------------------------------------------------
void C_BaseMedica::fill_treeWidget_ClassesIt(QTreeWidget *pQTreeWidget)
{   if (!database().isOpen() && database().open()== false)
       {outMessage(  tr("ERREUR : fill_treeWidget_ClassesIt() la base ne peut pas s'ouvrir"), __FILE__, __LINE__);  return ;
       }
     QTreeWidgetItem *itemPere = 0;
     QTreeWidgetItem *item     = 0;
     QString          id       = "";
     QString          requete  = QString( "SELECT  it_group_id, it_group_libelle, it_group_note "
                                          "FROM    it_group") + ownersSelectMention("it_group", C_BaseCommon::WhereMustBeAdd);
      QSqlQuery query (requete , database() );
      outSQL_error( query, "ERREUR  : fill_treeWidget_ClassesIt()", requete, __FILE__, __LINE__);
      if (query.isActive() )
         {pQTreeWidget->clear();
          while (query.next())
                {
                 item = 0;
                 id   = query.value(0).toString();
                 if (id.toInt()==-1) {item = new QTreeWidgetItem (pQTreeWidget); itemPere = item;}
                 else if (itemPere)  {item = new QTreeWidgetItem (itemPere);}
                 if (item)
                    {item->setText(0,CGestIni::Utf8_Query(query, 1));
                     item->setText(1,CGestIni::Utf8_Query(query, 2));
                     item->setText(2,id);
                    }
                }    // end while (query.next())
         }  // endif (query.isActive() )
}

//----------------------------------------------------- fillQListView_ATC ---------------------------------------------------------------
// 1er niveau  : classe anatomique principale
// 2ème niveau : sous classe thérapeutique
// 3ème niveau : sous classe pharmacologiq
// 4ème niveau : sous classe chimique
// 5ème niveau : substance active

void C_BaseMedica::fillQListView_ATC(QTreeWidget *pQTreeWidget )
{ if (!database().isOpen() && database().open()== false)
     {outMessage( tr("ERREUR : fillQListView_ATC() la base ne peut pas s'ouvrir"), __FILE__, __LINE__);  return ;
     }
  QTreeWidgetItem *curParentItem = 0;
  QTreeWidgetItem          *item = 0;
  long                    curLen = 0;
  QString      requete("SELECT ");
  requete +=      m_MEDICA_ATC_LIBELLE  + "," +  m_MEDICA_ATC_CODE  +
                  " FROM " + m_MEDICA_ATC_TBL_NAME + " WHERE " + m_MEDICA_ATC_LANG + " = \"" + m_lang + "\" ORDER BY " +  m_MEDICA_ATC_CODE;

  QSqlQuery query (requete , database() );
  outSQL_error( query, "ERREUR  : fillQListView_ATC()", requete, __FILE__, __LINE__);
  if (query.isActive() )
     {pQTreeWidget->clear();
      while (query.next())
            {QString atc_libelle = CGestIni::Utf8_Query(query, 0);
             QString   atc_code  = CGestIni::Utf8_Query(query, 1).trimmed();
             int            len  = atc_code.length();

             if (len==1)
                { curParentItem = new QTreeWidgetItem (pQTreeWidget);
                  curParentItem->setText(0,atc_libelle);
                  curParentItem->setText(1,atc_code);
                  curLen = len;
                }
             else if (len > curLen)
                { item =   new QTreeWidgetItem (curParentItem);
                  item->setText(0,atc_libelle);
                  item->setText(1,atc_code);
                  if (len != 7) curParentItem = item;
                  curLen = len;
                }
             else if (len == curLen)
                { item =   new QTreeWidgetItem (curParentItem);
                  item->setText(0,atc_libelle);
                  item->setText(1,atc_code);
                  curLen = len;
                }
             else if (len < curLen)
                { //......... remonter tant que le père n'a pas de partie comumune avec le fils ............
                  while( curParentItem->parent() != 0 &&  atc_code.left(curParentItem->text(1).length()) != curParentItem->text(1))
                       {curParentItem = curParentItem->parent();
                       }
                  curParentItem = new QTreeWidgetItem (curParentItem);
                  curParentItem->setText(0,atc_libelle);
                  curParentItem->setText(1,atc_code);
                  curLen = len;
                }
            }    // end while (query.next())
     }  // endif (query.isActive() )
}

//--------------------------- initAfssapsRestructureATC -----------------------------------------------------------
void C_BaseMedica::initAfssapsATC(const QString &path)
{
    //................. creation de la table d'origine ..........................
    dropTable("afs_atcTmp");
    createTable(
    "CREATE TABLE `afs_atcTmp` (                         "
    "`afs_atcTmp_pk` bigint(20) NOT NULL auto_increment ,"
    "`afs_atcTmp_owner` char(40)                        ,"
    "`afs_atcTmp_code`  char(10)                        ,"
    "`afs_atcTmp_en`    char(255)                       ,"
    "`afs_atcTmp_fr`    char(255)                       ,"
    "`afs_atcTmp_de`    char(255)                       ,"
    "PRIMARY KEY  (`afs_atcTmp_pk`)                      "
    ") AUTO_INCREMENT=0                    ");
    initAfssapsDataBase(path , "afs_atcTmp" , "\",\"" , "\"");
    //................. creation de la table de destination ..........................
    dropTable("afs_atc");
    createTable(
    "CREATE TABLE `afs_atc` (                          "
    "`afs_atc_pk` bigint(20) NOT NULL auto_increment  ,"
    "`afs_atc_owner`      char(40)                    ,"
    "`afs_atc_code`       char(10)                    ,"
    "`afs_atc_libelle`    char(255)                   ,"
    "`afs_atc_lang`       char(10)                    ,"
    "PRIMARY KEY  (`afs_atc_pk`)                       "
    ")AUTO_INCREMENT=0                  ");

 //..................... conversion de table .....................................
 if (!database().isOpen() && database().open()== false)
    {outMessage( tr("ERREUR : initAfssapsATC() la base ne peut pas s'ouvrir"), __FILE__, __LINE__);    return ;}
  QString   requete = "";
  QString   prepare = "";
  QSqlQuery querySrc (QString::null , database() );
  QSqlQuery queryDst (QString::null , database() );
  requete  = QString( "SELECT   afs_atcTmp_owner, afs_atcTmp_code, afs_atcTmp_en, afs_atcTmp_fr, afs_atcTmp_de  FROM `afs_atcTmp`");
  if (!querySrc.exec(requete ))
     {outSQL_error( querySrc, "ERREUR  : initAfssapsATC()", requete, __FILE__, __LINE__);               return;}
  if ( !querySrc.isActive() )           return;

  while(querySrc.next())
     {
      prepare  = QLatin1String(" INSERT INTO afs_atc ( `afs_atc_owner`,`afs_atc_code`,`afs_atc_libelle`,`afs_atc_lang`)  VALUES (?,?,?,?)");
      queryDst.prepare(prepare);
      queryDst.bindValue(0, querySrc.value(0).toString()); // owner
      queryDst.bindValue(1, querySrc.value(1).toString()); // code
      queryDst.bindValue(2, querySrc.value(2).toString()); // libelle
      queryDst.bindValue(3, "en");                         // langue
      if ( !queryDst.exec())  {outSQL_error( queryDst, "ERREUR  : C_BaseMedica::initAfssapsATC()", prepare, __FILE__, __LINE__);  return;}
      queryDst.prepare(prepare);
      queryDst.bindValue(0, querySrc.value(0).toString()); // owner
      queryDst.bindValue(1, querySrc.value(1).toString()); // code
      queryDst.bindValue(2, querySrc.value(3).toString()); // libelle
      queryDst.bindValue(3, "fr");                         // langue
      if ( !queryDst.exec())  {outSQL_error( queryDst, "ERREUR  : C_BaseMedica::initAfssapsATC()", prepare, __FILE__, __LINE__);  return;}
      queryDst.prepare(prepare);
      queryDst.bindValue(0, querySrc.value(0).toString()); // owner
      queryDst.bindValue(1, querySrc.value(1).toString()); // code
      queryDst.bindValue(2, querySrc.value(4).toString()); // libelle
      queryDst.bindValue(3, "de");                         // langue
      if ( !queryDst.exec()) {outSQL_error( queryDst, "ERREUR  : C_BaseMedica::initAfssapsATC()", prepare, __FILE__, __LINE__);   return;}
     }
 dropTable("afs_atcTmp");
}

//--------------------------- testInteraction -----------------------------------------------------------
QString C_BaseMedica::testInteraction(QTreeWidget *pQTreeWidget, const QString &class_it, const QString &produit_1, const QString &produit_2)
{pQTreeWidget->clear();
 if (!database().isOpen() && database().open()== false) return QString::null;
 QSqlQuery query(QString::null , database() );
 QString requete  = "";
 int          nb  = 0;
 int        il_1  = 0;
 int        il_2  = 0;
 QStringList pk1_List   = produit_1.length() ? isThisValueLikeInTable_ToList("it_compo",
                                                                             "it_compo_name",
                                                                             produit_1.toUpper(),
                                                                             "",
                                                                             QString(" AND `it_compo_class`=\"%1\" ").arg(class_it)) : QStringList();
 QStringList pk2_List   = produit_2.length() ? isThisValueLikeInTable_ToList("it_compo",
                                                                             "it_compo_name",
                                                                             produit_2.toUpper(),
                                                                             "",
                                                                             QString(" AND `it_compo_class`=\"%1\" ").arg(class_it)) : QStringList();
 int nbpk1              = pk1_List.count();
 int nbpk2              = pk2_List.count();

 if (nbpk1 && nbpk2)
    {if (nbpk1 * nbpk2 >50)
        {return tr("Trop grand nombre de requ\303\252tes : %1").arg(QString::number(nbpk1 * nbpk2));
        }
     for (il_1=0; il_1<nbpk1; ++il_1)
         {for (il_2=0; il_2<nbpk2; ++il_2)
              {requete  = QString("SELECT `it_link_inter_pk`,`it_link_compo_pk1`,`it_link_compo_pk2` FROM `it_link`"
                                  " WHERE `it_link_compo_pk1`  = \"%1\" AND `it_link_compo_pk2`  = \"%2\"").arg( pk1_List[il_1], pk2_List[il_2] );
               nb += execQueryGetInteraction(query, requete, pQTreeWidget);
              }
         }
    }
  else if (nbpk1)
    {if (nbpk1>50)
        {return tr("Trop grand nombre de requ\303\252tes : %1").arg(QString::number(nbpk1));
        }
     for (il_1=0; il_1<nbpk1; ++il_1)
         {requete  = QString("SELECT `it_link_inter_pk`,`it_link_compo_pk1`,`it_link_compo_pk2` FROM `it_link`"
                             " WHERE `it_link_compo_pk1`  = \"%1\"").arg( pk1_List[il_1]);
          nb += execQueryGetInteraction(query, requete, pQTreeWidget);
         }
     if (nb==0)
        {for (il_1=0; il_1<nbpk1; ++il_1)
             {requete  = QString("SELECT `it_link_inter_pk`,`it_link_compo_pk1`,`it_link_compo_pk2` FROM `it_link`"
                                " WHERE `it_link_compo_pk2`  = \"%1\"").arg( pk1_List[il_1]);
              nb += execQueryGetInteraction(query, requete, pQTreeWidget);
             }
        }
    }
  else if (nbpk2)
    {if (nbpk2>50)
        {return tr("Trop grand nombre de requ\303\252tes : %1").arg(QString::number(nbpk2));
        }
    for (il_2=0; il_2<nbpk2;++il_2)
         {requete  = QString("SELECT `it_link_inter_pk`,`it_link_compo_pk1`,`it_link_compo_pk2` FROM `it_link`"
                             " WHERE `it_link_compo_pk2`  = \"%1\"").arg( pk2_List[il_2]);
          nb += execQueryGetInteraction(query, requete, pQTreeWidget);
         }
    if (nb==0)
       {for (il_2=0; il_2<nbpk2; ++il_2)
            {requete  = QString("SELECT `it_link_inter_pk`,`it_link_compo_pk1`,`it_link_compo_pk2` FROM `it_link`"
                               " WHERE `it_link_compo_pk1`  = \"%1\"").arg( pk2_List[il_2]);
             nb += execQueryGetInteraction(query, requete, pQTreeWidget);
            }
       }
    }
 return tr("Nombre de r\303\251ponses : %1").arg(QString::number(nb));
}
/*
//--------------------------- execQueryGetInteraction -----------------------------------------------------------
int C_BaseMedica::createATCTableFromGet(QTreeWidget *pQTreeWidget)
{

}
*/
//--------------------------- execQueryGetInteraction -----------------------------------------------------------
int C_BaseMedica::execQueryGetInteraction(QSqlQuery &query, const QString &requete, QTreeWidget *pQTreeWidget)
{if (!query.exec(requete))
    {outSQL_error( query, "ERREUR  : execQueryGetInteraction()", requete, __FILE__, __LINE__);
     return 0;
    }
  int i = 0;
  if (query.isActive() )
      {while(query.next())
            {QTreeWidgetItem *pItem = getInteraction(query.value(0).toString(), pQTreeWidget);
             if (pItem)
                { pItem->setText(1, getNameFacteurFromPk(query.value(1).toString()) +
                                    " et " +
                                    getNameFacteurFromPk(query.value(2).toString()));
                  ++i;
                }
            }
      }
 return i;
}

//--------------------------- getNameFacteurFromPk -----------------------------------------------------------
QString C_BaseMedica::getNameFacteurFromPk(const QString &pk)
{  QSqlQuery query(QString::null , database() );
    QString requete  = QString("SELECT `it_compo_name`, `it_compo_descr` FROM `it_compo`"
                               " WHERE `it_compo_pk`  = \"%1\" ").arg( pk );
    requete += ownersSelectMention("it_compo");
    if (!query.exec(requete))
      {outSQL_error( query, "ERREUR  : getNameFacteurFromPk()", requete, __FILE__, __LINE__);
       return QString::null;
      }
    if (query.isActive() )
        {while(query.next())    // il ne devrait il n'y en avoir qu'un
              {return query.value(0).toString();
              }
        }
    else
       {return QString::null;
       }
    return QString::null;
}

//--------------------------- getInteraction -----------------------------------------------------------
QTreeWidgetItem * C_BaseMedica::getInteraction(const QString &pk, QTreeWidget *pQTreeWidget)
{   QSqlQuery query(QString::null , database() );
    QString requete  =  QString("SELECT `it_inter_date`,`it_inter_regle`, `it_inter_mecanisme`, `it_inter_cat`,`it_inter_owner` FROM `it_inter`"
                                " WHERE `it_inter_pk`  = \"%1\" ").arg( pk);
    requete += ownersSelectMention("it_inter");
    if (!query.exec(requete))
       {outSQL_error( query, "ERREUR  : getInteraction()", requete, __FILE__, __LINE__);
        return 0;
       }
    if (query.isActive() )
        {while(query.next())    // il ne devrait il n'y en avoir qu'un
              {QTreeWidgetItem *pItem = new QTreeWidgetItem(pQTreeWidget);
               pItem->setText(0, query.value(0).toString());      // date
            // pItem->setText(1, "les composants de l'interaction seront mis apres"
               pItem->setText(2, query.value(1).toString());      // la regle est le niveau
               pItem->setText(3, query.value(2).toString());      // texte sur le mecanisme
               pItem->setText(4, query.value(3).toString());      // texte sur la conduite a tenir
               pItem->setText(5, query.value(4).toString());      // proprio
               return pItem;
              }
        }
    else
       {return 0;
       }
    return 0;
}

//--------------------------- createInteractionForThisComposant -----------------------------------------------------------
int C_BaseMedica::createInteractionForThisComposant( QTreeWidget     *pQTreeWidgetSynthese,
                                                     QTreeWidget     *pQTreeWidgetGroup,
                                                     QTreeWidgetItem *pQTreeWidgetItemInteractant,
                                                     const QString   &libelle_FacteurPere,
                                                     const QString   &libelleInteractant,
                                                     const QString   &owner
                                                    )
{int nb                                       = 0;
 QString pk_interactant                       = isThisValueInTable("it_compo", "it_compo_name",  libelleInteractant.toUpper());
 QString err                                  = "";
 if (pk_interactant.length()==0)
    {err = tr("ERREUR : C_BaseMedica::createInteractionForThisComposant()\n pas de composant trouvé pour cette valeur '%1' ").arg(libelleInteractant).toLatin1();
     outMessage(err);
     return nb;
    }

 QString  mecanisme   = pQTreeWidgetItemInteractant->text(1);
 QString  niveau      = pQTreeWidgetItemInteractant->text(2);
 QString  cat         = pQTreeWidgetItemInteractant->text(3).trimmed();
 if (cat.length()==0 )
    {     cat         = niveau;  // placer à tout prix quelque chose (notamment pour les niveaux composes)
          //....... si niveau compose (donc non simple) .............
          if     (cat!="CONTRE-INDICATION"         &&
                  cat!="Association DECONSEILLEE"  &&
                  cat!="Précaution d'emploi"       &&
                  cat!="A prendre en compte"
                 )

         {cat         = cat.remove("-").remove(" ");   // epurer la chaine CI - ASDEC - APEC
          cat         = cat.replace("CI"    ,"||-093CIN");
          cat         = cat.replace("ASDEC" ,"||-073ADN");
          cat         = cat.replace("PE"    ,"||-043PEN");
          cat         = cat.replace("APEC"  ,"||-023PCN");
         }
    }

          cat         = cat.replace("Association contre-indiquée avec"                  ,"||-100CIA");
          cat         = cat.replace("Contre-indication avec"                            ,"||-100CIA");
          cat         = cat.replace("Contre-indications avec"                           ,"||-100CIA");
          cat         = cat.replace("Contre-indication"                                 ,"||-100CIN");
          cat         = cat.replace("Contre-indications"                                ,"||-100CIN");
          cat         = cat.replace("CONTRE-INDICATION"                                 ,"||-100CIN");
          cat         = cat.replace("- contre-indication avec"                          ,"||-100CIP");

          cat         = cat.replace("Association déconseillée avec"                     ,"||-075ADA");
          cat         = cat.replace("Associations déconseillées avec"                   ,"||-075ADA");
          cat         = cat.replace("Association DECONSEILLEE"                          ,"||-075ADN");
          cat         = cat.replace("Associations déconseillées"                        ,"||-075ADN");
          cat         = cat.replace("Association déconseillée"                          ,"||-075ADN");

          cat         = cat.replace("Précaution d'emploi avec"                          ,"||-050PEA");
          cat         = cat.replace("Précautions d'emploi avec"                         ,"||-050PEA");
          cat         = cat.replace("Précautions d'emploi"                              ,"||-050PEN");
          cat         = cat.replace("Précaution d'emploi"                               ,"||-050PEN");
          cat         = cat.replace("Association nécessitant une précaution d'emploi"   ,"||-050PEN");
          cat         = cat.replace("- précaution d'emploi avec"                        ,"||-050PEP");

          cat         = cat.replace("A prendre en compte avec"                          ,"||-025PCA");
          cat         = cat.replace("A prendre en compte"                               ,"||-025PCN");

 // >>>>> -1- il faut faire face a ce genre de truc <<<<<<<<<<<<<<
 //  CI - PE
 // Avec le méthotrexate utilisé à des doses > 20 mg/semaine :
 // ||-100CIP avec l'acide acétylsalicylique utilisé à doses
 // antalgiques, antipyrétiques ou anti-inflammatoires
 // ||-050PCP avec des doses antiagrégantes plaquettaires
 // d'acide acétylsalicylique. Contrôle hebdomadaire de l'hémogramme
 // durant les premières semaines de l'association. Surveillance accrue
 // en cas d'altération (même légère) de la fonction rénale, ainsi que chez
 // le sujet âgé.
 // Avec le méthotrexate utilisé à des doses =< 20 mg/semaine :
 // ||-050PCP avec l'acide acétylsalicylique utilisé à doses
 // antalgiques, antipyrétiques ou anti-inflammatoires. Contrôle
 // hebdomadaire de l'hémogramme durant les premières semaines de
 // l'association. Surveillance accrue en cas d'altération (même légère) de
 // la fonction rénale, ainsi que chez le sujet âgé.
 QStringList catList;
 //............ rendre un cas particulier en cas general ..........................
 int      pos_deb     = -1;
 int      pos_end     = -1;
 int      pos_split   = -1;
 int      i           = 0;
 QString  before_str  = "";     // isolera la phrase située entre 'Avec le' et '||'
 QString catTmp       = "";
 QString str          = "";
 pos_split            = cat.indexOf("Avec le ");
 while (pos_split != -1  )
    {pos_deb                      = pos_split;
     pos_split                    = cat.indexOf("Avec le ", pos_split + 8);
     if (pos_split != -1) pos_end = pos_split;
     else                 pos_end = cat.length();
     str                          = cat.mid(pos_deb,pos_end-pos_deb);
     catList                      = str.split("||");
     before_str                   = catList[0].remove(":");
     for ( i=1; i<catList.size(); ++i)                        // on part de 1 car le premier element est 'Avec le ...' etc ...
         {catTmp += QString("||") + catList[i].left(7) + " :\n" + before_str + " et " + catList[i].mid(7);
         }
    }
 if (catTmp.length()) cat = catTmp;
 //............ traiter les cas generaux ..........................
 catList  = cat.split("||");
 if (catList.size()>1)                                           // creer une interaction pour chaque niveau
    {for ( i=1; i<catList.size(); ++i)                           // on part de 1 car le premier element est 'CI - ASDEC - APEC' etc ...
         {niveau = catList[i].left(4);
          nb    += setInteractionItem( pQTreeWidgetSynthese,
                                       pQTreeWidgetGroup,
                                       libelle_FacteurPere,
                                       libelleInteractant,
                                       pk_interactant,
                                       mecanisme,
                                       niveau,
                                       tagNiveauToStr(catList[i]),
                                       owner
                                      );
         }
    }
 else
    {if      (niveau=="CONTRE-INDICATION")        { cat.prepend(QString("%1 :\n").arg(niveau)); niveau = "-094";}
     else if (niveau=="Association DECONSEILLEE") { cat.prepend(QString("%1 :\n").arg(niveau)); niveau = "-074";}
     else if (niveau=="Précaution d'emploi")      { cat.prepend(QString("%1 :\n").arg(niveau)); niveau = "-044";}
     else if (niveau=="A prendre en compte")      { cat.prepend(QString("%1 :\n").arg(niveau)); niveau = "-024";}

     nb += setInteractionItem(pQTreeWidgetSynthese,
                              pQTreeWidgetGroup,
                              libelle_FacteurPere,
                              libelleInteractant,
                              pk_interactant,
                              mecanisme,
                              niveau,
                              cat,
                              owner
                             );
    }
 return nb;
}
//--------------------------- tagNiveauToStr -----------------------------------------------------------
QString C_BaseMedica::tagNiveauToStr(QString &cat)
{QString niveau = cat.mid(4,2);
 QString avec   = cat.mid(6,1);
 QString str    = "";
 if      (niveau=="CI") str  = tr("CONTRE-INDICATION");
 if      (niveau=="AD") str  = tr("Association DECONSEILLEE");
 if      (niveau=="PE") str  = tr("Pr\303\251cautions d'emploi");
 if      (niveau=="PC") str  = tr("A prendre en compte");
 if      (avec  =="A")  str += tr(" avec ");
 return str + cat.mid(7);
}

//--------------------------- createInteractionForThisComposant -----------------------------------------------------------
int C_BaseMedica::setInteractionItem(    QTreeWidget     *pQTreeWidgetSynthese,
                                         QTreeWidget     *pQTreeWidgetGroup,
                                         const QString   &facteur1,
                                         const QString   &facteur2,
                                         const QString   &pk2,
                                         const QString   &mecanisme,
                                         const QString   &niveau,
                                         const QString   &cat,
                                         const QString   &owner
                                        )
{   int                          nb              = 0;
    QTreeWidgetItem *pItemComposant              = 0;;
    QTreeWidgetItem *pQTreeWidgetItemPereGroup   = 0;
    QString          aliasNamePere               = "";
    QString          pk_FacteurPere              = isThisValueInTable("it_compo", "it_compo_name", facteur1.toUpper());
    QString          pk_interact                 = it_createInteractionRecord( niveau, mecanisme, cat, owner);
    pItemComposant = new QTreeWidgetItem(pQTreeWidgetSynthese);
    pItemComposant->setText(0, facteur1);
    pItemComposant->setText(1, pk_FacteurPere);
    pItemComposant->setText(2, facteur2);
    pItemComposant->setText(3, pk2);
    pItemComposant->setText(4, mecanisme);      // mecanisme
    pItemComposant->setText(5, niveau);         // niveau
    pItemComposant->setText(6, cat);            // note
    pItemComposant->setText(7, pk_interact);    // pk de l'interaction

    ++nb;
    //............ creer les interactions pour les alias du groupe de ce pere .............
    pQTreeWidgetItemPereGroup   = isThisGroupeItem( facteur1.toUpper(), pQTreeWidgetGroup);
    if (pQTreeWidgetItemPereGroup)
       {int nbChilds    = pQTreeWidgetItemPereGroup->childCount ();
        int idChild     = 0;
        for (idChild    = 0; idChild<nbChilds; ++idChild)
          { aliasNamePere  = pQTreeWidgetItemPereGroup->child(idChild)->text(0);
            pk_FacteurPere = isThisValueInTable("it_compo", "it_compo_name", aliasNamePere.toUpper(),""," AND `it_compo_class`=\"ITMD\" ");  // normalement la base devrait avoir tous les composants (y compris les alias) renseignes
            pItemComposant = new QTreeWidgetItem(pQTreeWidgetSynthese);
            pItemComposant->setText(0, aliasNamePere);
            pItemComposant->setText(1, pk_FacteurPere);
            pItemComposant->setText(2, facteur2);
            pItemComposant->setText(3, pk2);
            pItemComposant->setText(4, mecanisme);
            pItemComposant->setText(5, niveau);
            pItemComposant->setText(6, cat);
            pItemComposant->setText(7, pk_interact);    // pk de l'interaction
            ++nb;
          }
       }
    return nb;
}

// AMIODARONE
// Voir aussi : antiarythmiques - autres médicaments susceptibles de donner des torsades de pointes - bradycardisants - médicaments susceptibles de donner des
// torsades de pointes - torsadogènes (sauf antiparasitaires, neuroleptiques, méthadone)

// CISAPRIDE
//  Voir aussi : autres médicaments susceptibles de donner des torsades de pointes - médicaments susceptibles de donner des torsades de pointes - substrats à
//  risque du CYP3A4 - torsadogènes (sauf antiparasitaires, neuroleptiques, méthadone)

// CEFAMANDOLE
//  Voir aussi : antabuse (réaction)

// CLARITHROMYCINE
//  Voir aussi : inhibiteurs puissants du CYP3A4 - macrolides (sauf spiramycine)

//  CYTOTOXIQUES
//   (altretamine, amsacrine, asparaginase, azathioprine, bleomycine, busulfan, capecitabine, carboplatine, carmustine, chlorambucil, chlormethine, cisplatine,
//   cladribine, cyclophosphamide, cytarabine, dacarbazine, dactinomycine, daunorubicine, docetaxel, doxorubicine, epirubicine, estramustine, etoposide, fludarabine,
//   fluorouracile, fotemustine, gemcitabine, hydroxycarbamide, idarubicine, ifosfamide, irinotecan, lomustine, melphalan, mercaptopurine, methotrexate, miltefosine,
//   mitoguazone, mitomycine c, mitoxantrone, oxaliplatine, paclitaxel, pentostatine, pipobroman, pirarubicine, plicamycine, procarbazine, raltitrexed, streptozocine,
//   tegafur, temozolomide, thiotepa, tioguanine, topotecane, vinblastine, vincristine, vindesine, vinorelbine)

//   DEXAMETHASONE
//   Voir aussi : glucocorticoïdes (sauf hydrocortisone en traitement substitutif) - hypokaliémiants
//--------------------------- isThisGroupeItem -----------------------------------------------------------
QTreeWidgetItem *C_BaseMedica::isThisGroupeItem( const QString &libelle, QTreeWidget *pQTreeWidgetGroup)
{   QTreeWidgetItemIterator it (pQTreeWidgetGroup);
    while (*it)
     { QTreeWidgetItem * pQTreeWidgetItem = (*it);                           // nom du composant
       if (pQTreeWidgetItem->parent()==0 && pQTreeWidgetItem->text(0)==libelle) return pQTreeWidgetItem;
       ++it;
     }
    return 0;
}

//--------------------------- it_createLinkFacteursInteraction -----------------------------------------------------------
QString C_BaseMedica::it_createLinkFacteursInteraction(const QString &pk1, const QString &pk2, const QString &pki, const QString &class_it, const QString &owner)
{    QString err = "";
     if (!isInOwnerList(owner))
        {err = tr("ERREUR  : C_BaseCommon::it_it_createLinkFacteursInteraction() \nThis owner : '%1' is not found").arg(owner).toLatin1();
         outMessage(err);
         return QString::null;
        }

     if (!database().isOpen() && database().open() == false) return QString::null;
     QSqlQuery query(QString::null , database() );
     QString prepare  = QLatin1String(" INSERT INTO it_link ( `it_link_owner`, `it_link_compo_pk1`,`it_link_compo_pk2`, `it_link_inter_pk`, `it_link_class`)"
                                      " VALUES (?,?,?,?,?)");
     query.prepare(prepare);
     query.bindValue(0, owner);
     query.bindValue(1, pk1);
     query.bindValue(2, pk2);
     query.bindValue(3, pki);
     query.bindValue(4, class_it);
     if ( !query.exec())
        {outSQL_error( query, "ERREUR  : C_BaseMedica::it_it_createLinkFacteursInteraction()", prepare, __FILE__, __LINE__);
         return QString::null;
        }
     return getLastPrimaryKey( "it_link", "it_link_pk");
}

//--------------------------- it_createInteractionRecord -----------------------------------------------------------
QString C_BaseMedica::it_createInteractionRecord( const QString   &niveau, const QString   &mecanisme, const QString   &cat, const QString   &owner)
{   QString err = "";
    if (!isInOwnerList(owner))
       {err = tr("ERREUR  : C_BaseCommon::it_createInteractionRecord() : This owner : '%1' is not found").arg(owner).toLatin1();
        outMessage( err);
        return QString::null;
       }
    if (!database().isOpen() && database().open()== false) return QString::null;
    QString c   = cat;
    QString mec = mecanisme;
    QSqlQuery query(QString::null , database() );
    QString requete  = QString("SELECT `it_inter_pk` FROM `it_inter` WHERE ("
                               "`it_inter_regle`      = \"%1\" AND "
                               "`it_inter_mecanisme`  = \"%2\" AND "
                               "`it_inter_cat`        = \"%3\" ) "
                               ).arg(niveau, mec.replace("\"","\\\"").replace(",","\\,"), c.replace("\"","\\\"").replace(",","\\,") );
    requete += ownersSelectMention("it_inter");
    if (!query.exec(requete))
       { outSQL_error(query, "ERREUR  : C_BaseMedica::it_createInteractionRecord()", requete, __FILE__, __LINE__);
         return QString::null;
       }
    if ( query.next() ) return query.value(0).toString();    // on a trouve on se casse avec

 //............... on a pas trouve on cree ............................
 QString prepare  = QLatin1String(" INSERT INTO it_inter (`it_inter_owner`, `it_inter_regle`,`it_inter_mecanisme`, `it_inter_cat`)"
                                  " VALUES (?,?,?,?)");
 query.prepare(prepare);
 query.bindValue(0, owner);
 query.bindValue(1, niveau);
 query.bindValue(2, mecanisme);
 query.bindValue(3, cat);
 if ( !query.exec())
    {outSQL_error(query, "ERREUR  : C_BaseMedica::it_createInteractionRecord()", prepare, __FILE__, __LINE__);
     return QString::null;
    }
 return getLastPrimaryKey( "it_inter", "it_inter_pk");
}

//--------------------------- it_createComposant -----------------------------------------------------------
QString C_BaseMedica::it_createComposant(const QString &libelle, const QString &descr, const QString &id_ctx_type, const QString &id_ctx, const QString &id_class, const QString   &owner)
{

    if (!isInOwnerList(owner))
       {outMessage( tr("ERREUR  : C_BaseCommon::it_createComposant() : This owner : '%1' is not found").arg(owner).toLatin1());
        return QString::null;
       }

    QString s = descr;
    int pos   = posLastEnclosedSymbol(s,"(",")");
    if (pos  != -1) s.truncate(pos);

    if (isThisValueInTable("it_compo","it_compo_name", libelle.toUpper(),"", QString(" AND `it_compo_class`=\"%1\" ").arg(id_class)).length())
       {C_Utils_Log::outMessage( 0, tr("WARNING  : C_BaseMedica::it_createComposant() : '%1' not added because it is already in '%2'").arg(libelle.toUpper(),"it_compo").toLatin1());
        //...................... ^ en mettant zero on empeche la sortie dans le widget de log et elle se fait juste dans la console d'erreurs normale
        return QString::null;      // si deja present cassos
       }
    QString prepare  = QLatin1String("INSERT INTO it_compo (`it_compo_owner`, `it_compo_name`,`it_compo_descr`,`it_compo_id_ctx`, `it_compo_id_ctx_type`, `it_compo_class`) "
                                     "VALUES (?,?,?,?,?,?)");
    if (!database().isOpen() && database().open()== false) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, libelle.toUpper());
    query.bindValue(2, s);
    query.bindValue(3, id_ctx);
    query.bindValue(4, id_ctx_type);
    query.bindValue(5, id_class);
    if ( !query.exec())
       {outSQL_error( query, "ERREUR  : C_BaseMedica::it_createComposant()", prepare, __FILE__, __LINE__);
        return QString::null;
       }
    return getLastPrimaryKey( "it_compo", "it_compo_pk");
}



//--------------------------- createGroupRowInBase -----------------------------------------------------------
QString C_BaseMedica::createGroupRowInBase(const QString &libelle,const QString &note, const QString id, const QString   &owner)
{
    QString err = "";
    if (!isInOwnerList(owner))
       {err = tr("ERREUR  : C_BaseCommon::createGroupRowInBase() \nThis owner : '%1' is not found").arg(owner).toLatin1();
        outMessage(  err);
        return QString::null;
       }

    QString prepare  = "";
            prepare  = "INSERT INTO it_group ( `it_group_owner`,`it_group_libelle`,`it_group_note`,`it_group_id`) "
                       "VALUES (?,?,?,?)";
    if (!database().isOpen() && database().open()== false) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, libelle.toUpper());
    query.bindValue(2, note);
    query.bindValue(3, id);
    if ( !query.exec())
       {outSQL_error( query, "ERREUR  : C_BaseMedica::createGroupInBase()", prepare, __FILE__, __LINE__);
        return QString::null;
       }
    return getLastPrimaryKey( "it_group", "it_group_pk");
}

//--------------------------- initATCBase -----------------------------------------------------------
// "A01AA30","COMBINATIONS","ASSOCIATIONS","KOMBINATIONEN"
// "A01AA51","SODIUM FLUORIDE, COMBINATIONS","FLUORURE DE SODIUM, ASSOCIATIONS","NATRIUMFLUORID, KOMBINATIONEN"

int C_BaseMedica::initATCBase(const QString &fileName, const QString &tableName)
{   //....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fileName ) )
       {outMessage(  tr("ERREUR : Ce fichier : '%1' n'existe pas.").arg(fileName).toLatin1(), __FILE__, __LINE__);
        return 0;
       }

  return 0;
}

//--------------------------- initAfssapsDataBase -----------------------------------------------------------
int C_BaseMedica::initAfssapsDataBase(const QString &fileName, const QString &tableName, const QString &separator /*= "\t" */, const QString &toClean /*= "" */)
{QStringList fieldsList;
 QStringList bindList;
 QString err        = "";
 QString prepare    = "";
 QString fieldName  = "";
 QString fieldOwner = tableName + "_owner";
 int      position  = 0;
 QString     line   = "";
 QByteArray   ba;
 //....................... ouvrir le fichier en mode QTextStream ...........................
 if ( !QFile::exists( fileName ) )
    {outMessage( tr("ERREUR : initAfssapsDataBase() Ce fichier : '%1' n'existe pas.").arg(fileName).toLatin1(), __FILE__, __LINE__);
                                                        return 0;
    }
 QFile file( fileName );
 if ( !file.open( QIODevice::ReadOnly  ) )
    {outMessage( tr("ERREUR : initAfssapsDataBase() Ce fichier : '%1' n'est pas accessible en lecture.").arg(fileName).toLatin1(), __FILE__, __LINE__);
                                                        return 0;
    }

 if (!database().isOpen() && database().open()== false)
    {outMessage( tr("ERREUR : initAfssapsDataBase() la base ne peut pas s'ouvrir"), __FILE__, __LINE__);
     file.close();                                      return 0;
    }
 QSqlQuery query(QString::null, database());
 //................... verifier si table existe dans la base ...............................
 fieldsList = tablesList();    // faut pas deconner on se sert de fieldsList
 if (fieldsList.indexOf(tableName)==0)
    {err = tr("Cette table : '%1' n'existe pas.").arg(tableName).toLatin1();
     outMessage( err, __FILE__, __LINE__);
     file.close();
                                                        return 0;
    }

 //................... recuperer la liste des champs ...............................
 QSqlRecord record = database().record ( tableName );
 fieldsList.clear();
 int nb = record.count();
 int  i = 0;
 outMessage( tr("----------- liste des champs de la table : '%1' ---------------").arg(tableName).toLatin1());
 for (i = 0; i<nb; ++i)
     {fieldName = record.fieldName(i);
      if (!fieldName.endsWith("_pk") && !fieldName.endsWith("_owner") ) fieldsList<<record.fieldName(i);   // on ne prend pas le pk ni owner car auto alimente
     }
 if (fieldsList.count()==0)
    {err = tr("Les champs de cette table : '%1.%2' n'existent pas ou sont non accessibles..").arg(database().databaseName(),tableName).toLatin1();
     outMessage(  err, __FILE__, __LINE__);
     file.close();                                      return 0;
    }
 //.................... effacer la table ....................................................
 if (eraseTable(tableName)==0) {file.close();           return 0;}

 //.................... preparer l'insertion ...............................................
 prepare          = "INSERT INTO "  + tableName  + " ( \n";
 nb = fieldsList.count();
 for ( i=0; i<nb; ++i)
     {            fieldName        = fieldsList[i];
                  prepare         += fieldName ;
      if (i<nb-1) prepare         +=" ,";
      else        prepare         += "," + fieldOwner + ") VALUES (";
      err = tr("table : '%3' champ Num %2 = '%1'").arg(fieldName, QString::number(i), tableName).toLatin1();
      outMessage( err);
     }
 for ( i=0; i<nb; ++i)
     {if (i<nb-1) prepare         +=" ?,";
      else        prepare         +=" ?,?)";
     }

 //....................... parser le fichier et ajouter les champs ...........................
 if (m_pQProgressBar) m_pQProgressBar->setRange(0,(int)(file.size()/10));
 int nbRows         = 0;
 int nbWrited       = 0;
 while (!file.atEnd())
       {
        ba        = file.readLine(MAX_READ);
        position += ba.size(); if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
        line      = CGestIni::fromMyUTF8(ba.data()).trimmed();
        if (toClean.length())
           {if (line.startsWith(toClean)) line = line.mid(toClean.length());
            if (line.endsWith(toClean))   line.truncate(line.length()-toClean.length());
           }
        bindList = line.split(separator);
        //if (bindList.size()==fieldsList.size())
           {query.prepare(prepare);
            for ( i=0; i<fieldsList.size(); ++i)
                {
                 if  (i<bindList.size())  query.bindValue(i, bindList[i]);
                 else                     query.bindValue(i, "");
                }
            query.bindValue(i, "AF");
            if ( !query.exec())
               {outSQL_error( query, "ERREUR  : C_BaseMedica::initAfssapsDataBase()", prepare, __FILE__, __LINE__);
               }
            else
               {
                ++nbWrited;
               }
           }
        /*
        else if (nbRows==0)
          {qDebug(tr("ERREUR  : C_BaseMedica::initAfssapsDataBase \n"
                     "Le nombre de champs : %1  de la table '%2'\n"
                     "differe de celui des donnees: %3").arg(QString::number(fieldsList.size()), tableName, QString::number(bindList.size())).toAscii());
            for ( i=0; i<bindList.size(); ++i)
                {qDebug(tr("Donnee num : %1 : '%2'").arg(QString::number(i), bindList[i]).toAscii());
                }
           }
        */
        ++nbRows;
       }
 err = tr("Nombre de lignes ecrites : %1 sur %2").arg(QString::number(nbWrited), QString::number(nbRows)).toLatin1();
 outMessage( err);
 file.close();
 return nbWrited;
}

//====================================== C_BiblioData =====================================
C_BiblioData ::C_BiblioData(){}
C_BiblioData ::C_BiblioData( const QString pk,      // 0
                             const QString owner,   // 1
                             const QString lang,    // 2
                             const QString url,     // 3
                             const QString id,      // 4
                             const QString date,    // 5
                             const QString note,    // 6
                             const QString type,    // 7
                             const QString mime,    // 8
                             const QString blob_pk, // 9
                             const QString data     // 10
                           )
{
      m_pk      = pk;
      m_id      = id;
      m_owner   = owner;
      m_lang    = lang;
      m_url     = url;
      m_date    = date;
      m_note    = note;
      m_type    = type;
      m_mime    = mime;
      m_blob_pk = blob_pk;
      m_data    = data;
}
