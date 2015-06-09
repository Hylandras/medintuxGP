#include <QTreeWidgetItem>
#include <QSqlDatabase>
#include <QProgressBar>
#include <QSqlQuery>
#include <QSqlError>
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QCursor>
#include <QSqlDriver>
#include <QProgressDialog>
#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QMessageBox>
#include <QTextCodec>
#include <QDir>
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_ApiVar.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
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

//=================== C_BDM_DrugListRecord ===============================================
//---------------------------- serialize ------------------------------------------------
QString C_BDM_DrugListRecord::serialize( int with_Header /* = 1 */, const QString &ofset /* ="" */) const
{return serialize(this, with_Header, ofset);
}

//---------------------------- serialize ------------------------------------------------
QString C_BDM_DrugListRecord::serialize(const C_BDM_DrugListRecord *pC_BDM_DrugListRecord, int with_Header /* = 1 */, const QString &ofset /* ="" */, int htmlConvert /* = CGestIni::SupInfAnd */)
{QString modeleXML = "";
 if (with_Header)
    { modeleXML += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   "<!-- Projet    : MedinTux                              -->\n"
                   "<!-- Module    : medicabase                            -->\n"
                   "<!-- Objet     : C_BDM_DrugListRecord                  -->\n"
                   "<!-- guru mail : sevin-roland@medintux.org             -->\n"
                   "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n";
    }
 modeleXML +=      ofset + "<C_BDM_DrugListRecord>\n";
 modeleXML +=      ofset + "   <version>1</version>\n";
      //............................. C_BDM_DrugListRecord ..............................
      CGestIni::addXmlData("m_id",                 QByteArray().append(pC_BDM_DrugListRecord->m_id),                         modeleXML,false, ofset, htmlConvert); //pC_RendezVous->m_date.toString(Qt::ISODate), modeleXML,0);
      CGestIni::addXmlData("m_id_type",            QByteArray().append(pC_BDM_DrugListRecord->m_id_type),                    modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_commercialName",     QByteArray().append(pC_BDM_DrugListRecord->m_commercialName),             modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_dci_1Name",          QByteArray().append(pC_BDM_DrugListRecord->m_dci_1Name),                  modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_dci_2Name",          QByteArray().append(pC_BDM_DrugListRecord->m_dci_2Name),                  modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_dci_3Name",          QByteArray().append(pC_BDM_DrugListRecord->m_dci_3Name),                  modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_owner",              QByteArray().append(pC_BDM_DrugListRecord->m_owner),                      modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_lang",               QByteArray().append(pC_BDM_DrugListRecord->m_lang),                       modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_nb_up",              QByteArray().append(pC_BDM_DrugListRecord->m_nb_up),                      modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_ucd_price",          QByteArray().append(pC_BDM_DrugListRecord->m_ucd_price),                  modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_atc",                QByteArray().append(pC_BDM_DrugListRecord->m_atc),                        modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_generique",          QByteArray().append(pC_BDM_DrugListRecord->m_generique),                  modeleXML,false, ofset, htmlConvert);
      CGestIni::addXmlData("m_statut",             QByteArray().append(pC_BDM_DrugListRecord->m_statut),                     modeleXML,false, ofset, htmlConvert);

 modeleXML += ofset +  "</C_BDM_DrugListRecord>\n";
 return modeleXML;
}

//---------------------------- unSerialize ------------------------------------------------
C_BDM_DrugListRecord *C_BDM_DrugListRecord::unSerialize(const QString & data, C_BDM_DrugListRecord *obj)
{   int nextPos=0;
    //............................. C_BDM_DrugListRecord ..............................
    obj->m_id                 = CGestIni::getXmlData("m_id",                 data, &nextPos);
    obj->m_id_type            = CGestIni::getXmlData("m_id_type",            data, &nextPos);
    obj->m_commercialName     = CGestIni::getXmlData("m_commercialName",     data, &nextPos);
    obj->m_dci_1Name          = CGestIni::getXmlData("m_dci_1Name",          data, &nextPos);
    obj->m_dci_2Name          = CGestIni::getXmlData("m_dci_2Name",          data, &nextPos);
    obj->m_dci_3Name          = CGestIni::getXmlData("m_dci_3Name",          data, &nextPos);
    obj->m_owner              = CGestIni::getXmlData("m_owner",              data, &nextPos);
    obj->m_lang               = CGestIni::getXmlData("m_lang",               data, &nextPos);
    obj->m_nb_up              = CGestIni::getXmlData("m_nb_up",              data, &nextPos);
    obj->m_ucd_price          = CGestIni::getXmlData("m_ucd_price",          data, &nextPos);
    obj->m_atc                = CGestIni::getXmlData("m_atc",                data, &nextPos);
    obj->m_generique          = CGestIni::getXmlData("m_generique",          data, &nextPos);
    obj->m_statut             = CGestIni::getXmlData("m_statut",             data, &nextPos);
    return obj;
}

//---------------------------- unSerialize ------------------------------------------------
C_BDM_DrugListRecord C_BDM_DrugListRecord::unSerialize(const QString & data)
{C_BDM_DrugListRecord obj;
 unSerialize(data, &obj);
 return obj;
}

//=================== C_OrdoLineRecord ===============================================================
//---------------------------- serialize ------------------------------------------------
QString C_OrdoLineRecord::serialize() const
{return serialize(this);
}
//---------------------------- serialize ------------------------------------------------
QString C_OrdoLineRecord::serialize(const C_OrdoLineRecord *pC_OrdoLineRecord)
{QString modeleXML =
 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
 "<!-- Projet    : MedinTux                              -->\n"
 "<!-- Module    : medicabase                            -->\n"
 "<!-- Objet     : C_OrdoLineRecord                      -->\n"
 "<!-- guru mail : sevin-roland@medintux.org             -->\n"
 "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n"
 "<C_OrdoLineRecord>\n"
 "   <version>1</version>\n";
      //............................. C_BDM_DrugListRecord ..............................
      CGestIni::addXmlData("m_id",                 QByteArray().append(pC_OrdoLineRecord->m_id),                         modeleXML,0); //pC_RendezVous->m_date.toString(Qt::ISODate), modeleXML,0);
      CGestIni::addXmlData("m_id_type",            QByteArray().append(pC_OrdoLineRecord->m_id_type),                    modeleXML,0);
      CGestIni::addXmlData("m_commercialName",     QByteArray().append(pC_OrdoLineRecord->m_commercialName),             modeleXML,0);
      CGestIni::addXmlData("m_dci_1Name",          QByteArray().append(pC_OrdoLineRecord->m_dci_1Name),                  modeleXML,0);
      CGestIni::addXmlData("m_dci_2Name",          QByteArray().append(pC_OrdoLineRecord->m_dci_2Name),                  modeleXML,0);
      CGestIni::addXmlData("m_dci_3Name",          QByteArray().append(pC_OrdoLineRecord->m_dci_3Name),                  modeleXML,0);
      CGestIni::addXmlData("m_owner",              QByteArray().append(pC_OrdoLineRecord->m_owner),                      modeleXML,0);
      CGestIni::addXmlData("m_lang",               QByteArray().append(pC_OrdoLineRecord->m_lang),                       modeleXML,0);
      CGestIni::addXmlData("m_nb_up",              QByteArray().append(pC_OrdoLineRecord->m_nb_up),                      modeleXML,0);
      CGestIni::addXmlData("m_ucd_price",          QByteArray().append(pC_OrdoLineRecord->m_ucd_price),                  modeleXML,0);
      CGestIni::addXmlData("m_atc",                QByteArray().append(pC_OrdoLineRecord->atc()),                        modeleXML,0);
      CGestIni::addXmlData("m_generique",          QByteArray().append(pC_OrdoLineRecord->m_generique),                  modeleXML,0);
      CGestIni::addXmlData("m_statut",             QByteArray().append(pC_OrdoLineRecord->m_statut),                     modeleXML,0);
      //............................. C_OrdoLineRecord ..................................
      CGestIni::addXmlData("m_ordo_key",             QByteArray().append(pC_OrdoLineRecord->m_ordo_key),                         modeleXML,0);
      CGestIni::addXmlData("m_numOrdre",             QByteArray().append(pC_OrdoLineRecord->m_numOrdre),                         modeleXML,0);
      CGestIni::addXmlData("m_secureState",          QByteArray().append(pC_OrdoLineRecord->m_secureState),                      modeleXML,0);
      CGestIni::addXmlData("m_date_debut",           QByteArray().append(pC_OrdoLineRecord->m_date_debut.toString(Qt::ISODate)), modeleXML,0);
      CGestIni::addXmlData("m_date_fin",             QByteArray().append(pC_OrdoLineRecord->m_date_fin.toString(Qt::ISODate)),   modeleXML,0);
      CGestIni::addXmlData("m_pk_patient",           QByteArray().append(pC_OrdoLineRecord->m_pk_patient),                       modeleXML,0);
      CGestIni::addXmlData("m_guid_patient",         QByteArray().append(pC_OrdoLineRecord->m_guid_patient),                     modeleXML,0);
      CGestIni::addXmlData("m_code_indication",      QByteArray().append(pC_OrdoLineRecord->m_code_indication),                  modeleXML,0);
      CGestIni::addXmlData("m_type_code_indication", QByteArray().append(pC_OrdoLineRecord->m_type_code_indication),             modeleXML,0);
      CGestIni::addXmlData("m_smr",                  QByteArray().append(pC_OrdoLineRecord->m_smr),                              modeleXML,0);
      CGestIni::addXmlData("m_smr_indication",       QByteArray().append(pC_OrdoLineRecord->m_smr_indication),                   modeleXML,0);
      CGestIni::addXmlData("m_voie_admin",           QByteArray().append(pC_OrdoLineRecord->m_voie_admin),                       modeleXML,0);
      CGestIni::addXmlData("m_ald",                  QByteArray().append(pC_OrdoLineRecord->m_ald),                              modeleXML,0);
      CGestIni::addXmlData("m_substituable",         QByteArray().append(pC_OrdoLineRecord->m_substituable),                     modeleXML,0);
      CGestIni::addXmlData("m_renouvelable",         QByteArray().append(pC_OrdoLineRecord->m_renouvelable),                     modeleXML,0);
      CGestIni::addXmlData("m_quantite_totale",      QByteArray().append(pC_OrdoLineRecord->m_quantite_totale),                  modeleXML,0);
      CGestIni::addXmlData("m_forme_galenique",      QByteArray().append(pC_OrdoLineRecord->m_forme_galenique),                  modeleXML,0);
      CGestIni::addXmlData("m_unite_de_prise",       QByteArray().append(pC_OrdoLineRecord->m_unite_de_prise),                   modeleXML,0);
      CGestIni::addXmlData("m_quantite_par_up",      QByteArray().append(pC_OrdoLineRecord->m_quantite_par_up),                  modeleXML,0);
      CGestIni::addXmlData("m_unite_quantite",       QByteArray().append(pC_OrdoLineRecord->m_unite_quantite),                   modeleXML,0);
      CGestIni::addXmlData("m_note_textuelle_libre", QByteArray().append(pC_OrdoLineRecord->m_note_textuelle_libre),             modeleXML,0);
      CGestIni::addXmlData("m_schema_posologique",   QByteArray().append(pC_OrdoLineRecord->m_schema_posologique),               modeleXML,0, "", CGestIni::noConvert);
 modeleXML +=
 "</C_OrdoLineRecord>\n";
 return modeleXML;
}

//---------------------------- unSerialize ------------------------------------------------
C_OrdoLineRecord *   C_OrdoLineRecord::unSerialize(const QString & data, C_OrdoLineRecord *obj)
{   int nextPos=0;
    //............................. C_BDM_DrugListRecord ..............................
    obj->m_id                  = CGestIni::getXmlData("m_id",                 data, &nextPos);
    obj->m_id_type             = CGestIni::getXmlData("m_id_type",            data, &nextPos);
    obj->m_commercialName      = CGestIni::getXmlData("m_commercialName",     data, &nextPos);
    obj->m_dci_1Name           = CGestIni::getXmlData("m_dci_1Name",          data, &nextPos);
    obj->m_dci_2Name           = CGestIni::getXmlData("m_dci_2Name",          data, &nextPos);
    obj->m_dci_3Name           = CGestIni::getXmlData("m_dci_3Name",          data, &nextPos);
    obj->m_owner               = CGestIni::getXmlData("m_owner",              data, &nextPos);
    obj->m_lang                = CGestIni::getXmlData("m_lang",               data, &nextPos);
    obj->m_nb_up               = CGestIni::getXmlData("m_nb_up",              data, &nextPos);
    obj->m_ucd_price           = CGestIni::getXmlData("m_ucd_price",          data, &nextPos);
    obj->m_atc                 = CGestIni::getXmlData("m_atc",                data, &nextPos);
    obj->m_generique           = CGestIni::getXmlData("m_generique",          data, &nextPos);
    obj->m_statut              = CGestIni::getXmlData("m_statut",             data, &nextPos);

if (data.indexOf("<C_OrdoLineRecord>") != -1)    //.... si le xml est celui d'une ordonnance complete exploiter le xml
   {
    //............................. C_OrdoLineRecord ..................................
    obj->m_ordo_key             = CGestIni::getXmlData("m_ordo_key",                         data, &nextPos);
    obj->m_numOrdre             = CGestIni::getXmlData("m_numOrdre",                         data, &nextPos);
    obj->m_secureState          = CGestIni::getXmlData("m_secureState",                      data, &nextPos);
    obj->m_date_debut           = QDateTime::fromString(CGestIni::getXmlData("m_date_debut", data, &nextPos),Qt::ISODate);
    obj->m_date_fin             = QDateTime::fromString(CGestIni::getXmlData("m_date_fin",   data, &nextPos),Qt::ISODate);
    obj->m_pk_patient           = CGestIni::getXmlData("m_pk_patient",                       data, &nextPos);
    obj->m_guid_patient         = CGestIni::getXmlData("m_guid_patient",                     data, &nextPos);
    obj->m_code_indication      = CGestIni::getXmlData("m_code_indication",                  data, &nextPos);
    obj->m_type_code_indication = CGestIni::getXmlData("m_type_code_indication",             data, &nextPos);
    obj->m_smr                  = CGestIni::getXmlData("m_smr",                              data, &nextPos);
    obj->m_smr_indication       = CGestIni::getXmlData("m_smr_indication",                   data, &nextPos);
    obj->m_voie_admin           = CGestIni::getXmlData("m_voie_admin",                       data, &nextPos);
    obj->m_ald                  = CGestIni::getXmlData("m_ald",                              data, &nextPos);
    obj->m_substituable         = CGestIni::getXmlData("m_substituable",                     data, &nextPos);
    obj->m_renouvelable         = CGestIni::getXmlData("m_renouvelable",                     data, &nextPos);
    obj->m_quantite_totale      = CGestIni::getXmlData("m_quantite_totale",                  data, &nextPos);
    obj->m_forme_galenique      = CGestIni::getXmlData("m_forme_galenique",                  data, &nextPos);
    obj->m_unite_de_prise       = CGestIni::getXmlData("m_unite_de_prise",                   data, &nextPos);
    obj->m_quantite_par_up      = CGestIni::getXmlData("m_quantite_par_up",                  data, &nextPos);
    obj->m_note_textuelle_libre = CGestIni::getXmlData("m_note_textuelle_libre",             data, &nextPos);
    obj->m_schema_posologique   = CGestIni::getXmlData("m_schema_posologique",               data, &nextPos);
   }
else //.... si le xml est incomplet (celui d'une C_BDM_DrugListRecord) alors completer avec l'API exploiter le xml
   {obj->set_ordo_key("")                  ;
    obj->set_numOrdre("")                  ;
    obj->set_secureState("")               ;
  //obj->set_date_debut("")                ;
  //obj->set_date_fin( "")                 ;
    obj->set_pk_patient("")                ;
    obj->set_guid_patient("")              ;
    obj->set_code_indication("")           ;
    obj->set_type_code_indication("")      ;
    obj->set_smr("")                       ;
    obj->set_smr_indication("")            ;
    obj->set_voie_admin("")                ;
    obj->set_ald("")                       ;
    obj->set_substituable("")              ;
    obj->set_renouvelable("")              ;
    obj->set_quantite_totale("")           ;
    obj->set_forme_galenique("")           ;
    obj->set_unite_de_prise("")            ;
    obj->set_quantite_par_up("")           ;
    obj->set_unite_quantite("")            ;
    obj->set_note_textuelle_libre("")      ;
    obj->set_schema_posologique("")        ;
   }
    return obj;
}

//---------------------------- unSerialize ------------------------------------------------
C_OrdoLineRecord C_OrdoLineRecord::unSerialize(const QString & data)
{C_OrdoLineRecord obj;
 unSerialize(data, &obj);
 return obj;
}

//=================== C_BDM_PluginI ===================================
C_BDM_PluginI::C_BDM_PluginI()
    : C_BDM_ApiVar()
{   m_isInitialised     = 0;
    m_pDbi_Database     = 0;
//$    m_pC_PatientCtx     = 0;
}
//--------------------------------------------- ~C_BDM_PluginI -------------------------------------------------------------------
/*! \brief destructor  C_BDM_PluginI
*/
// IMPORTANT de mettre le destructeur en virtuel et de l'implementer dans le CPP sinon explose lors du delete
C_BDM_PluginI::~C_BDM_PluginI()
{
}

//-------------------------------- getInteractionAllergies ------------------------------------------------------
/*! \brief return allergicals interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
// ASPIRINE|DCI|ASPIRINE|ALD|Allergie(Allergie)|plutot intol\303\251rance|06-06-1956|
// 3515495|CIP|LAMALINE|Sport|Allergie(Allergie)|urticaire g\303\251n\303\251ralis\303\251 |06-06-1956|
// 3007528|CIP|ASPIRINE|ALD|Allergie(Allergie)|plut\303\264t intol\303\251rance|06-06-1956|
// J01CR02|ATC|AMOXICILINE ET INHIBITEUR D'ENZYME|ALD|Allergie(Allergie)|pas de commentaire|06-06-1956|
QList<C_InterResultPair> C_BDM_PluginI::getInteractionAllergies(const QString &cip, QList<C_LifeEvent> lifeEventList, QString drugName /* = "" */ )
{QList<C_InterResultPair>   interResultPairList;               // liste qui sera nourie avec toutes les interactions
 if (drugName.length()==0)  drugName = dbi_cip_drugName(cip);
 //....................... gestion des excipients .................................
 QList<C_ExcipientItem> excipients = getExcipientsFromCIP(cip);   // recuperer la liste des excipients du medicament
 if ( excipients.size() )                                              // si pas d'exipient dans le medicament on ne teste pas
    { QString code_typ = excipients[0].code_typ();                     // recuperer le type de code retourne par l'implementation de la base medicamenteuse
      for ( int i = 0; i<lifeEventList.size();++i )                    // pour chaque evenement de vie
          { if ( lifeEventList[i].codeTyp()==code_typ )                // si un concerne une allergie a un excipient (le code type depend de l'implementation de la BDM)
               { QString code_produit  = lifeEventList[i].code();      // noter son code chimique
                 for ( int u=0; u<excipients.size();++u)               // pour chaque excipient du medicament
                     { if ( code_produit==excipients[u].code() )       // verifier si celui pour lequel le patient est allergique  est present dans celui du medicament
                          { interResultPairList.append ( C_InterResultPair( code_produit, code_typ, excipients[u].name(),   // auquel cas ajouter a la liste des interactions
                                                                            cip         , "CIP", drugName,
                                                                            QObject::tr( "<u><b>ATTENTION ALLERGIE DECLAREE A UN EXCIPIENT</b></u> <br />"
                                                                                         "Ce produit <b>%1</b> "
                                                                                         "de code CIP : <b>%2</b><br />"
                                                                                         "contient un excipient : <b>%3</b> de code %4<br />"
                                                                                       ).arg( drugName , cip,excipients[u].name() , code_produit ) +
                                                                            QObject::tr("pour lequel ce patient est d\303\251clar\303\251 allergique<br />"),
                                                                            "PATCTXALL","1")
                                                       );
                          } // end if ( code_produit==excipients[u].code() )
                     } // for ( int u=0; u<excipients.size();++u)
               } // if ( lifeEventList[i].codeTyp()==code_typ )
          } // for ( int i = 0; i<lifeEventList.size();++i )
    } // if ( excipients.size() )
 QString     codePatho;
 QString     codePathoTyp;
 QString     termPatho;
 QString     dci;
 QString     comment;
 QString     date;
 QString     drugATC  = dbi_cip_atcName(cip);
 QString     ctx_ATC  = "";
 QMap <QString, QString> dciPathoMap;     // assure unicite des clefs et tri des DCI

 //..................... construire la liste des dci auxquelles ce patient est allergique ....................
 for (int i = 0; i<lifeEventList.size();++i)
     {     //..................... recuperer la liste des dci auxquelles ce patient est allergique ....................
           //                      TODO faire cache
           codePathoTyp  = lifeEventList[i].codeTyp(); // piecesList.at(1).trimmed();
           if (codePathoTyp=="DCI")
              { dciPathoMap[lifeEventList[i].libelle()]= codePathoTyp; // dciPathoMap[piecesList.at(2).trimmed()]= codePathoTyp;
              }
            else if (codePathoTyp=="CIP")
              { /*
                C_BDM_DrugListRecord listProdPatho;
                QList < C_BDM_DrugListRecord > productsList = dbi_selectProductsList (lifeEventList[i].code(), C_BDM_PluginI::cip_filter);
                if ( productsList.size())   // attention un CIP note dans le dossier malade peut ne plus etre (obsolete plus vendu etc..)  dans la liste des CIP de la BDM
                   { listProdPatho          = productsList.at(0);
                    codePatho              = lifeEventList[i].code();    // piecesList.at(0).trimmed();
                    dci = listProdPatho.dci_1Name(); if (dci.length()) dciPathoMap[dci]=codePatho;
                    dci = listProdPatho.dci_2Name(); if (dci.length()) dciPathoMap[dci]=codePatho;
                    dci = listProdPatho.dci_3Name(); if (dci.length()) dciPathoMap[dci]=codePatho;
                   }
                */
                codePatho            = lifeEventList[i].code();    // piecesList.at(0).trimmed();
                QStringList dci_list = get_DCI_list_FromCIP( codePatho );
                for ( int dl=0; dl<dci_list.size(); ++dl )
                    { dciPathoMap[dci_list.at(dl)]=codePatho;
                    }
              }
     }
 //.................. tester si les DCI de ce produit sont dans la liste des allergies DCI de ce patient .................
 QStringList                        dciList;
 // QList < C_BDM_DrugListRecord > productsList = dbi_selectProductsList (cip,       C_BDM_PluginI::cip_filter);
 // if (productsList.size())
    { /*
      C_BDM_DrugListRecord listProdCip   =  productsList.at(0);   // obtenir la liste des 3 premieres DCI de ce produit
      dci = listProdCip.dci_1Name(); if (dci.length()) dciList.append(dci);
      dci = listProdCip.dci_2Name(); if (dci.length()) dciList.append(dci);
      dci = listProdCip.dci_3Name(); if (dci.length()) dciList.append(dci);
      */
      dciList = get_DCI_list_FromCIP( cip );
      for (int l = 0; l < dciList.size(); ++l)
          { dci =  dciList.at(l);
            if ( dciPathoMap.contains(dci) )
               { codePatho = dciPathoMap[dci];
                 interResultPairList.append ( C_InterResultPair(codePatho, "DCI", termPatho,
                                                                cip      , "CIP", drugName,
                                                                QObject::tr( "<u><b>ATTENTION ALLERGIE A UN COMPOSANT DCI</b></u> <br />"
                                                                             "Ce produit <b>%1</b> "
                                                                             "de code CIP : <b>%2</b><br />"
                                                                             "contient un composant DCI : <b>%3</b>").arg(drugName,cip,dci) +
                                                                              ((codePatho==QString("DCI"))?QString(""):QObject::tr (" contenu par le produit <b>%1</b> "
                                                                             "de code CIP : <b>%2</b>,<br />"
                                                                            ).arg(dbi_cip_drugName(codePatho),codePatho)) +
                                                                 QObject::tr("pour lequel ce patient est d\303\251clar\303\251 allergique<br />"),
                                                                 "PATCTXALL","1")
                                            );

               } //if ( dciPathoMap.contains(dci) )
          }  //for (int l = 0; l < dciList.size(); ++l)
  } // endif (productsList.size())
 //................... tester l'ATC ..................................................
 for (int i = 0; i<lifeEventList.size();++i)
     {     codePatho     = lifeEventList[i].code();    // piecesList.at(0).trimmed();
           codePathoTyp  = lifeEventList[i].codeTyp(); // piecesList.at(1).trimmed();
           termPatho     = lifeEventList[i].libelle();

           if (codePathoTyp =="ATC")
              { if (drugATC.contains(codePatho) || drugATC.left(5)==codePatho.left(5))    // SUR ATC tester au niveau de la famille chimique
                   {comment    = lifeEventList[i].comment();
                    date       = lifeEventList[i].dateDeb();
                    if (comment.length())
                       {comment = QObject::tr("<br /><b> commentaire :</b> ") + comment;
                        if (date.length()) comment += QObject::tr(" en date du : ") + date;
                       }
                    interResultPairList.append(C_InterResultPair(codePatho, "ATC", termPatho,
                                                                 cip      , "CIP", drugName,
                                                                 QObject::tr("<u><b>ATTENTION ALLERGIE ATC</b></u> <br />"
                                                                             "Allergie d\303\251clar\303\251e \303\240 cette famille de produits : <b>%3</b> "
                                                                             "de code ATC : <b>%1</b><br />"
                                                                             "dont fait partie ce produit : <b>%5</b><br />"
                                                                             "%2")
                                                                              .arg(codePatho ,  comment.length()?comment:"", code_ATC_To_Name(codePatho),drugName),
                                                                 "PATCTXALL","1"));
                   }
               else if (codePatho.length()==7)
                   { QMap <QString, QString> mapDCI_CIP = dbi_atc_to_cip_map(codePatho);
                     for (int l = 0; l < dciList.size(); ++l)
                         { comment    = lifeEventList[i].comment();
                           date       = lifeEventList[i].dateDeb();
                           if (comment.length())
                              { comment = QObject::tr("<br /><b> commentaire :</b> ") + comment;
                                if (date.length()) comment += QObject::tr(" en date du : ") + date;
                              }
                           dci =  dciList.at(l);
                           if (mapDCI_CIP.contains(dci))
                              { interResultPairList.append(C_InterResultPair(codePatho, "ATC", termPatho,
                                                                             cip      , "CIP", drugName,
                                                                             QObject::tr("<u><b>ATTENTION ALLERGIE ATC</b></u> <br />"
                                                                                         "Allergie d\303\251clar\303\251e \303\240 cette famille <b>ATC</b> de produits : <b>%3</b> "
                                                                                         "de code ATC : <b>%1</b><br />"
                                                                                         "dont fait partie un composant DCI <b>%6</b> du produit : <b>%5</b><br />"
                                                                                         "%2")
                                                                                              .arg(codePatho ,  comment.length()?comment:"", code_ATC_To_Name(codePatho),drugName,dci),
                                                                             "PATCTXALL","1"));
                              }
                         }
                   }
              }
     }
 //................... tester le CIP ..................................................
 for (int i = 0; i<lifeEventList.size();++i)
     {     codePatho     = lifeEventList[i].code();    // piecesList.at(0).trimmed();
           codePathoTyp  = lifeEventList[i].codeTyp(); // piecesList.at(1).trimmed();
           termPatho     = lifeEventList[i].libelle();
           if (codePathoTyp=="CIP")
              { ctx_ATC = dbi_cip_atcName(codePatho).left(5);
                if (codePatho==cip)                              // SUR CIP :  cas certain car l'allergie du patient est en code CIP
                   { comment    = lifeEventList[i].comment();
                     date       = lifeEventList[i].dateDeb();
                     if (comment.length())
                        {comment = QObject::tr("<br /><b> commentaire :</b> ") + comment;
                         if (date.length()) comment += QObject::tr(" en date du : ") + date;
                        }

                     interResultPairList.append(C_InterResultPair(codePatho, "CIP", termPatho,
                                                                  cip      , "CIP", drugName,
                                                                  QObject::tr("<u><b>ATTENTION ALLERGIE SUR CIP</b></u> <br />"
                                                                              "Allergie formelle d\303\251clar\303\251e pour le code CIP %1 <br />"
                                                                              "li\303\251 \303\240 ce produit : <b>%2</b>"
                                                                              "%3")
                                                                              .arg(codePatho, drugName, comment.length()?comment:""),
                                                                  "PATCTXALL","1"));

                   }
                else if (ctx_ATC == drugATC.left(5))           // SUR FAMILLE ATC cas moins certain car on teste la famille ATC famille chimique de ce code CIP (AMOXICILLINE echappe a allergie a AUGMENTIN)
                   { comment    = lifeEventList[i].comment();
                     date       = lifeEventList[i].dateDeb();
                     if (comment.length())
                        {comment = QObject::tr("<br /><b> commentaire :</b> ") + comment;
                          if (date.length()) comment += QObject::tr(" en date du : ") + date;
                        }

                     interResultPairList.append(C_InterResultPair(codePatho, "CIP", termPatho,
                                                                  cip      , "CIP", drugName,
                                                                  QObject::tr("<u><b>ATTENTION ALLERGIE FAMILLE ATC</b></u> <br />"
                                                                              "Ce produit <b>%1</b> "
                                                                              "de code CIP : <b>%2</b><br />"
                                                                              "appartient \303\240 la m\303\252me famille de produits : <b>%3</b> "
                                                                              "de code ATC : <b>%4</b> <br />"
                                                                              "qu'un produit <b>%5</b> "
                                                                              "de code CIP : <b>%6</b><br />"
                                                                              "auquel ce patient est d\303\251clar\303\251 allergique<br />"
                                                                              "%7")
                                                                              .arg(drugName,                         // 1
                                                                       cip,                                          // 2
                                                                       code_ATC_To_Name(ctx_ATC) ,                   // 3
                                                                       ctx_ATC,                                      // 4
                                                                       dbi_cip_drugName(codePatho),                  // 5
                                                                       codePatho,                                    // 6
                                                                       comment.length()?comment:""),                 // 7
                                                                  "PATCTXALL","1"
                                                                 )
                                               );
                   }
                else
                   {
                   }
              }//if (piecesList.at(1)=="CIP")
     }//for (int i = 0; i<allergieslist.size();++i)
 return  interResultPairList;
}
//-------------------------------- dbi_cip_atcName ------------------------------------------------------
/*! \brief return ATC code from CIP code.
 *  \param const QString& cip drug cip identification .
 *  \return QString ATC code from this drug
 */
QString C_BDM_PluginI::dbi_cip_atcName(const QString& cip)
{   QSqlQuery query(QString::null , dbi_database()->database() );
    QString atc      = "";
    QString requete  =  QString (" SELECT %1 FROM %2 "
                                 " WHERE  %3  = \"%4\" "
                                 " AND    %5  = \"CIP\""
                                ).arg( m_BDM_DRUGLIST_ATC, m_BDM_DRUGLIST_TBL_NAME, m_BDM_DRUGLIST_ID, cip, m_BDM_DRUGLIST_TYPE_ID);
    if (!query.exec(requete))
       {dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_Api::ATC_from_CIP()", requete, __FILE__, __LINE__);
        return QString::null;
       }
    if (query.isActive() )
        {while(query.next())    // il ne devrait il n'y en avoir qu'un
              {atc = query.value(0).toString();      // date
              }
        }
    return atc;
}
//-------------------------------- dbi_cip_drugName ------------------------------------------------------
/*! \brief return drug name code from CIP code.
 *  \param const QString& cip drug cip identification .
 *  \return QString drug name
 */
QString C_BDM_PluginI::dbi_cip_drugName(const QString& cip)
{   QSqlQuery query(QString::null , dbi_database()->database() );
    QString value    = "";
    QString requete  =  QString (" SELECT %1 FROM %2 "
                                 " WHERE  %3  = \"%4\" "
                                 " AND    %5  = \"CIP\""
                                ).arg( m_BDM_DRUGLIST_LIBELLE, m_BDM_DRUGLIST_TBL_NAME, m_BDM_DRUGLIST_ID, cip, m_BDM_DRUGLIST_TYPE_ID);
    if (!query.exec(requete))
       {dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_Api::CIP_to_DrugName()", requete, __FILE__, __LINE__);
        return QString::null;
       }
    if (query.isActive() )
        {while(query.next())    // il ne devrait il n'y en avoir qu'un
              {value = query.value(0).toString();      // date
              }
        }
    return value;
}
//-------------------------------- dbi_atc_to_cip_map ------------------------------------------------------
/*! \brief return QMap <dci_name, cip>  from CIP code.
 *  \param atc     atc code from which we want DCI list.
 *  \return QMap <dci_name, cip>
 */

QMap<QString, QString> C_BDM_PluginI::dbi_atc_to_cip_map(const QString &atc)
{QMap<QString, QString> map;        //DCI_name - cip
 QString dci_1            = "";
 QString dci_2            = "";
 QString dci_3            = "";
 QString          requete =           " SELECT "
                                      "`"+m_BDM_DRUGLIST_ID        +"`,"     // 0
                                      "`"+m_BDM_DRUGLIST_TYPE_ID   +"`,"     // 1
                                      "`"+m_BDM_DRUGLIST_DCI_1     +"`,"     // 2
                                      "`"+m_BDM_DRUGLIST_DCI_2     +"`,"     // 3
                                      "`"+m_BDM_DRUGLIST_DCI_3     +"` "     // 4
                                      " FROM "+m_BDM_DRUGLIST_TBL_NAME + " WHERE `"+m_BDM_DRUGLIST_ATC + "`LIKE  \"%" + atc + "%\";";
 QSqlQuery query (requete , dbi_database()->database() );
 dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_PluginI::dbi_atc_to_cip_map()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           { dci_1 = query.value(2).toString().trimmed();
             dci_2 = query.value(3).toString().trimmed();
             dci_3 = query.value(4).toString().trimmed();
             if (dci_1.length()) map[dci_1] = query.value(0).toString();
             if (dci_2.length()) map[dci_2] = query.value(0).toString();
             if (dci_3.length()) map[dci_3] = query.value(0).toString();
           }
    }
 return   map;
}
//------------------------ dbi_DrugListCount -----------------------------------------
/*! \brief return DrugList records number
 *  \return return DrugList records number
 */
int C_BDM_PluginI::dbi_DrugListCount()
{   QString requete  =  " SELECT COUNT(*)  FROM " + m_BDM_DRUGLIST_TBL_NAME;
    QSqlQuery query(requete , dbi_database()->database() );
    dbi_database()->outSQL_error(query, QObject::tr("<font color=#ff0000><b>ERREUR  : C_BDM_PluginI::dbi_DrugListCount() </b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {return (long) query.value(0).toInt();
       }
    return 0;
}

//------------------------ dbi_selectProductsList -----------------------------------------
/*! \brief select drugs list from user input, for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param const QString textSelect user text fr select drugs.
 *  \param C_BDM_Api::flags filterFlag selection filter.
 *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
 */
const QList < C_BDM_DrugListRecord > C_BDM_PluginI::dbi_selectProductsList (const QString &text, C_BDM_PluginI::flags filterFlag  /* = C_BDM_Api::all_filter */ , int limit   /*=200*/)
{QList < C_BDM_DrugListRecord > productsList;
 if (!dbi_database()->database().isOpen() && dbi_database()->database().open()== false)
    { dbi_database()->outMessage( QObject::tr("ERREUR : C_BDM_Api::dbi_selectProductsList() database can not be opened"), __FILE__, __LINE__);  return productsList;
    }
 QString     requete = dbi_makeSelectProductsListQuery(text, filterFlag);
 if (limit) requete += " limit 0," + QString::number(limit);
 QSqlQuery query (requete , dbi_database()->database() );
 dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_Api::dbi_selectProductsList()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           {QString prix = "00000000" + query.value(10).toString(); prix = prix.right(6); prix.insert(4,',');
            productsList.append( C_BDM_DrugListRecord(query.value(2).toString(),
                                                      query.value(3).toString(),
                                                      query.value(5).toString(),
                                                      query.value(6).toString(),
                                                      query.value(7).toString(),
                                                      query.value(8).toString(),
                                                      query.value(0).toString(),
                                                      query.value(1).toString(),
                                                      query.value(9).toString(),
                                                      prix,    // prix
                                                      query.value(4).toString(),
                                                      query.value(11).toString(),
                                                      query.value(12).toString()
                                                      )
                               );
           }    // end while (query.next())
    }  // endif (query.isActive() )
 return productsList;
}
//-------------------------------- dbi_makeSelectProductsListQuery --------------------------------------------
/*! \brief make select query to select drugs list from user input, for a good performance
 *  response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param const QString textSelect user text fr select drugs.
 *  \param C_BDM_Api::flags filterFlag selection filter.
 *  \return query result string
 */

QString C_BDM_PluginI::dbi_makeSelectProductsListQuery(const    QString &text, C_BDM_PluginI::flags filterFlag)
{QString        requete =         " SELECT "
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
                                  "`"+m_BDM_DRUGLIST_GENERIQUE +"`,"     // 11
                                  "`"+m_BDM_DRUGLIST_STATUT    +"`,"     // 12
                                  "`"+m_BDM_DRUGLIST_SPEC_FK   +"` "     // 13
                                  " FROM "+m_BDM_DRUGLIST_TBL_NAME;
 if (filterFlag&C_BDM_PluginI::cip_list)
    {requete  +=  QString(" WHERE  %1 IN (%2) ").arg(m_BDM_DRUGLIST_ID, text);        // liste de cip de la forme 'cip1','cip2','cip3'
    }
 else if (text.length())
    {requete  +=  QString(" WHERE (");
     if (filterFlag&C_BDM_PluginI::nom_filter)  requete  += m_BDM_DRUGLIST_LIBELLE     + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::atc_filter)  requete  += m_BDM_DRUGLIST_ATC         + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::dci_filter)  requete  += m_BDM_DRUGLIST_DCI_1       + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::cip_filter)  requete  += m_BDM_DRUGLIST_ID          + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::dc3_filter)  requete  += m_BDM_DRUGLIST_DCI_2       + " LIKE \"%1%\" OR " + m_BDM_DRUGLIST_DCI_3 + " LIKE \"%1%\" OR ";
     if ( !requete.endsWith("OR ") )            requete  += m_BDM_DRUGLIST_LIBELLE     + " LIKE \"%1%\" OR ";      // si aucun filtre detecte on est juste avec 'WHERE (' donc rajouter une selection sur le libelle
     requete.chop(4);                                // virer le dernier " OR "
     requete  += ") ";                               // fermer la parenthese du WHERE
     requete   = requete.arg(text.toUpper());        // placer les caracteres de l'utilisateur
     if (filterFlag&C_BDM_PluginI::gen_filter) {requete  +=  " AND (" + m_BDM_DRUGLIST_GENERIQUE   + " IN ('G','R')) ";}
     if (filterFlag&C_BDM_PluginI::vil_filter && filterFlag&C_BDM_PluginI::hop_filter)
                                               {requete  +=  " AND (" + m_BDM_DRUGLIST_STATUT      + " = 'VH' OR " + m_BDM_DRUGLIST_STATUT + " = 'H') ";}
     else if (filterFlag&C_BDM_PluginI::vil_filter)
                                               {requete  +=  " AND (" + m_BDM_DRUGLIST_STATUT      + " = 'VH'         ) ";}
     else if (filterFlag&C_BDM_PluginI::hop_filter)
                                               {requete  +=  " AND (" + m_BDM_DRUGLIST_STATUT      + " = 'H'         ) ";}
     if (filterFlag&C_BDM_PluginI::atc_filter) {requete  +=  " AND (" + m_BDM_DRUGLIST_ATC         + "!= ''          ) ";}
     //requete  += dbi_database()->ownersSelectMention(m_BDM_DRUGLIST_TBL_NAME, C_BaseCommon::WhereAlreadyIn);  // rajouter le filtre du proprio
     if (filterFlag&C_BDM_PluginI::ucd_filter)  requete  +=  " ORDER BY " + m_BDM_DRUGLIST_UCD_PRICE;
    }
 else
    {QString where_and = " WHERE";
     //requete  += dbi_database()->ownersSelectMention(m_BDM_DRUGLIST_TBL_NAME, C_BaseCommon::WhereMustBeAdd);
     if (filterFlag&C_BDM_PluginI::gen_filter) {requete  +=  where_and + " (" + m_BDM_DRUGLIST_GENERIQUE   + " IN ('G','R')) ";                                  where_and = " AND";}
     if (filterFlag&C_BDM_PluginI::vil_filter && filterFlag&C_BDM_Api::hop_filter)
                                               {requete  +=  where_and + " (" + m_BDM_DRUGLIST_STATUT      + " = 'VH' OR " + m_BDM_DRUGLIST_STATUT + " = 'H') "; where_and = " AND";}
     else if (filterFlag&C_BDM_PluginI::vil_filter)
                                               {requete  +=  where_and + " (" + m_BDM_DRUGLIST_STATUT      + " = 'VH'        ) ";                                where_and = " AND";}
     else if (filterFlag&C_BDM_PluginI::hop_filter)
                                               {requete  +=  where_and + " (" + m_BDM_DRUGLIST_STATUT      + " = 'H'         ) ";                                where_and = " AND";}
     if (filterFlag&C_BDM_PluginI::atc_filter) {requete  +=  " AND (" + m_BDM_DRUGLIST_ATC         + "!= ''          ) ";}
     if (filterFlag&C_BDM_PluginI::ucd_filter)  requete  +=  " ORDER BY " + m_BDM_DRUGLIST_UCD_PRICE;
    }
  // CGestIni::Param_UpdateToDisk(QDir::homePath ()+ "/requete.sql",requete);
  return requete;
}
//-------------------------------- dbi_selectPosologieHitList ------------------------------------------------------
/*! \brief select drugs posologies hits list from drug code
 *  \param const QString &drug_id : as variable name shows, it's drug identification key.
 *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
 *  \param int limit : limit number of returns.
 *  \return QMap <int, QString> where int is order number and QString is grammar string started by 'cip/pk!' mention
 */
QStringList  C_BDM_PluginI::dbi_selectPosologieHitList(const QString &drug_id, const QString &drug_id_type /* = "CIP" */, int limit /* = 15  */)
{
    QStringList listResult;
    if (!dbi_database())  return listResult;
    if (!dbi_database()->database().isOpen() && dbi_database()->database().open()== false)
       { dbi_database()->outMessage( QObject::tr("ERREUR : C_BDM_Api::selectPosologieHitList() database can not be opened"), __FILE__, __LINE__);  return listResult;
       }
    int     score    = 0;
    QString grammar  = "";
    QString pk_score = "";
    QString  requete = " SELECT "
                       "`"           + m_BDM_POSO_HIT_PK            + "`,"     // 0
                       "`"           + m_BDM_POSO_HIT_SCORE         + "`,"     // 1
                       "`"           + m_BDM_POSO_HIT_GRAMMAR       + "` "     // 2
                       " FROM  "     + m_BDM_POSO_HIT_TBL_NAME      +
                       " WHERE "     + m_BDM_POSO_HIT_DRUG_ID       + " = '" + drug_id       + "' "
                       " AND   "     + m_BDM_POSO_HIT_DRUG_TYPE_ID  + " = '" + drug_id_type  + "' "
                       " ORDER BY `" + m_BDM_POSO_HIT_SCORE         + "` DESC LIMIT "        + QString::number(limit);

    QSqlQuery query (requete , dbi_database()->database() );
    dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_Api::selectPosologieHitList()", requete, __FILE__, __LINE__);
    if (query.isActive() )
       {while (query.next())
              {pk_score = query.value(0).toString();                 // m_BDM_POSO_HIT_PK
               score    = query.value(1).toInt();                    // m_BDM_POSO_HIT_SCORE
               grammar  = CGestIni::Utf8_Query(query, 2);            // m_BDM_POSO_HIT_GRAMMAR
               //........... noter le pk et cip de ce favoris .................................
               CGestIni::setXmlData("gph_pk", pk_score,      grammar);          // noter le pk
               CGestIni::setXmlData("gph_id", drug_id,       grammar);          // noter le cip
               CGestIni::setXmlData("gph_it", drug_id_type,  grammar);          // noter le type
               if (score<0 )  listResult.prepend( grammar );    // si durci placer devant
               else           listResult.append(  grammar );    // si non durci placer apres
              }    // end while (query.next())
       }  // endif (query.isActive() )
    return listResult;
}
//-------------------------------- dbi_deleteItemPosologieHit ------------------------------------------------------
/*! \brief delete a item from drug hit parade.
 *  \param const QString &pk :  item pk to delete
 *  \return true in all is ok otherwise false
 */
int C_BDM_PluginI::dbi_deleteItemPosologieHit(const QString &pk )
{   if (!dbi_database())  return false;
    if (!dbi_database()->database().isOpen() && dbi_database()->database().open()== false)
       { dbi_database()->outMessage( QObject::tr("ERREUR : C_BDM_Api::deleteItemPosologieHit() database can not be opened"), __FILE__, __LINE__);  return false;
       }
    QString requete    = QString("DELETE FROM %1 WHERE %2='%3' ").arg(m_BDM_POSO_HIT_TBL_NAME, m_BDM_POSO_HIT_PK, pk);
    QSqlQuery query (dbi_database()->database() );

    if (!query.exec(requete))
       {dbi_database()->outSQL_error( query, QObject::tr("ERREUR : C_BDM_Api::deleteItemPosologieHit() can't remove record"), requete, __FILE__, __LINE__);
        return false;
       }
    return true;
}

//-------------------------------- dbi_increasePosologieHit ------------------------------------------------------
/*! \brief increase a posologie drug hit parade adjusted in real time of use. Create record only if not exists.
 *  \param const QString &grammar :     grammar input string as
 *  \param const QString &drug_id : as variable name shows, it's drug identification key.
 *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
 *  \param int mode  default = INCREASE_HIT/increase hit  else FIX_HIT add the posologie whith fix hit.
 *  \return pk of created or updated record if all is ok otherwise empty string
 */

QString C_BDM_PluginI::dbi_increasePosologieHit(const QString &grammar, const QString &drug_id, const QString &drug_id_type /* = "CIP" */,  int mode  /* = INCREASE_HIT */)
{
    if (!dbi_database())  return QString::null;
    if (!dbi_database()->database().isOpen() && dbi_database()->database().open()== false)
       { dbi_database()->outMessage( QObject::tr("ERROR : C_BDM_Api::increasePosologieHit() database can not be opened"), __FILE__, __LINE__);  return QString::null;
       }

    int  nextPos      = 0;
    QString header    = CGestIni::getXmlData("gph", grammar, &nextPos);
    QString sequences = CGestIni::getXmlData("gps", grammar, &nextPos);

    nextPos                 = 0;
    QString status          = CGestIni::getXmlData("status",  header, &nextPos);
    if (status=="P")        return QString::null;    // ce favoris provient de la base de donnees ne pas enregistrer de hit dessus
    QString commercialName  = CGestIni::getXmlData("gph_na",  header, &nextPos);
    QString compoXmlParts   = CGestIni::getXmlData("gph_dcl", header, &nextPos);
    QString cycle           = CGestIni::getXmlData("gph_cy",  header, &nextPos);
    QString substituable    = CGestIni::getXmlData("gph_sb",  header, &nextPos);
    QString dci_displayMode = CGestIni::getXmlData("gph_dci", header, &nextPos);     // affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
    QString up_forme        = CGestIni::getXmlData("gph_uf",  header, &nextPos);     // comprime injection

    QString qPA_min_By_FactCorp       = CGestIni::getXmlData("gph_pmin",     header, &nextPos);
    QString qPA_max_By_FactCorp       = CGestIni::getXmlData("gph_pmax",     header, &nextPos);
    QString unitePoso                 = CGestIni::getXmlData("gph_punit",    header, &nextPos);  // mg
    QString facteurCorpo              = CGestIni::getXmlData("gph_pfc",      header, &nextPos);  // 10
    QString unite_facteurCorpo        = CGestIni::getXmlData("gph_pfcunit",  header, &nextPos);  // kg m2
    QString qPA_ByUnitPrise           = CGestIni::getXmlData("gph_pqbyuf",   header, &nextPos);  // 500 mg (par comprime)
    QString gph_psecable              = CGestIni::getXmlData("gph_psecable", header, &nextPos);  // secabilite : -1/non secable 4/secable en 4 etc...

    QString literalite_N    = CGestIni::getXmlData("gph_nm",      header, &nextPos).remove('\n');      // cm comme conversion mode
    QString literalite_T    = CGestIni::getXmlData("gph_cm",      header, &nextPos).remove('\n');      // cm comme conversion mode
    QString terrainStr      = CGestIni::getXmlData("gph_tr",      header, &nextPos).remove('\n');
    QString indicStr        = CGestIni::getXmlData("gph_in",      header, &nextPos).remove('\n');
    QString comment         = CGestIni::getXmlData("gph_co",      header, &nextPos);

    //.......... on ne reconstruit que ce dont on a besoin .......................................
    //           a enregistrer pour un favori
    QString ToStore   = QString ("<gph>\n"
                                 " <gph_na>%1</gph_na>\n"             //       nom commercial et usuel du produit
                                 " <gph_dcl>"                         //       liste des compositions pas de retour chariot car %3 le possede deja vu l'extraction de 'sequences'
                                 "   %2"                              //       items de la liste des compositions
                                 " </gph_dcl>\n"
                                 " <gph_pk></gph_pk>\n"
                                 " <gph_id></gph_id>\n"
                                 " <gph_it></gph_it>\n"
                                ).arg(commercialName, compoXmlParts)
                      + QString (
                                 " <gph_cy>%1</gph_cy>\n"
                                 " <gph_sb>%2</gph_sb>\n"
                                 " <gph_dci>%3</gph_dci>\n"
                                 " <gph_uf>%4</gph_uf>\n"
                                ).arg(cycle, substituable, dci_displayMode, up_forme)
                      + QString (" <gph_pmin>%1</gph_pmin>\n"         //        posologie minimum
                                 " <gph_pmax>%2</gph_pmax>\n"         //        posologie maximum
                                 " <gph_punit>%3</gph_punit>\n"       //        posologie unite
                                 " <gph_pfc>%4</gph_pfc>\n"           //        posologie facteur corporel qu de gph_punit par facteur corporel
                                 " <gph_pfcunit>%5</gph_pfcunit>\n"   //        posologie facteur corporel unite Kg cm
                                 " <gph_pqbyuf>%6</gph_pqbyuf>\n"     //        quantite d'unite posologique par unite de forme de gph_punit par gph_uf
                                 " <gph_psecable>%7</gph_psecable>\n" //........secabilite : -1/non secable 4/secable en 4 etc...
                                ).arg(qPA_min_By_FactCorp, qPA_max_By_FactCorp, unitePoso, facteurCorpo, unite_facteurCorpo, qPA_ByUnitPrise, gph_psecable)
                      + QString (
                                 " <gph_nm>%1</gph_nm>\n"
                                 " <gph_cm>%2</gph_cm>\n"
                                 " <gph_tr>%3</gph_tr>\n"
                                 " <gph_in>%4</gph_in>\n"
                                 " <gph_co>%5</gph_co>\n"
                                 "</gph>\n"
                                 "<gps>"      // pas de retour chariot car %3 le possede deja vu l'extraction de 'sequences'
                                 "%6"
                                 "</gps>\n"
                                ).arg(literalite_N, literalite_T, terrainStr, indicStr, comment, sequences);

    QStringList indicList   =  C_PosologieGrammar::get_indicationsList(indicStr);
    QString pk              =  C_PosologieGrammar::pk(grammar);       // recuperer si cette posologie n'est pas deja pourvue d'un pk auquel cas c'en est une deja existante
    QString prepare         =  "";
    int     hit             =  0;
    QSqlQuery query(QString::null , dbi_database()->database() );

    //......................... on renseigne la liste ...................................
    if (pk.length()==0)
       {  prepare  =   (" INSERT INTO "+m_BDM_POSO_HIT_TBL_NAME+"( \n"
                        "`"+m_BDM_POSO_HIT_OWNER               +"`,\n"     // 0
                        "`"+m_BDM_POSO_HIT_LANG                +"`,\n"     // 1
                        "`"+m_BDM_POSO_HIT_SCORE               +"`,\n"     // 2
                        "`"+m_BDM_POSO_HIT_GRAMMAR             +"`,\n"     // 3
                        "`"+m_BDM_POSO_HIT_ORDER               +"`,\n"     // 4
                        "`"+m_BDM_POSO_HIT_CODE_INDIC          +"`,\n"     // 5
                        "`"+m_BDM_POSO_HIT_CODE_INDIC_TYPE     +"`,\n"     // 6
                        "`"+m_BDM_POSO_HIT_LIBELLE_INDIC       +"`,\n"     // 7
                        "`"+m_BDM_POSO_HIT_DRUG_ID             +"`,\n"     // 8
                        "`"+m_BDM_POSO_HIT_DRUG_TYPE_ID        +"` \n"     // 9
                        ") VALUES (?,?,?,?,?,?,?,?,?,?)\n");
          if (mode==C_BDM_Api::INCREASE_HIT) hit =  1;
          else                               hit = -1;
       }
    else
       {
          prepare   = " UPDATE " + m_BDM_POSO_HIT_TBL_NAME    + " SET   \n"
                      " `"+m_BDM_POSO_HIT_OWNER               +"` = ? ,\n"     // 0
                      " `"+m_BDM_POSO_HIT_LANG                +"` = ? ,\n"     // 1
                      " `"+m_BDM_POSO_HIT_SCORE               +"` = ? ,\n"     // 2
                      " `"+m_BDM_POSO_HIT_GRAMMAR             +"` = ? ,\n"     // 3
                      " `"+m_BDM_POSO_HIT_ORDER               +"` = ? ,\n"     // 4
                      " `"+m_BDM_POSO_HIT_CODE_INDIC          +"` = ? ,\n"     // 5
                      " `"+m_BDM_POSO_HIT_CODE_INDIC_TYPE     +"` = ? ,\n"     // 6
                      " `"+m_BDM_POSO_HIT_LIBELLE_INDIC       +"` = ? ,\n"     // 7
                      " `"+m_BDM_POSO_HIT_DRUG_ID             +"` = ? ,\n"     // 8
                      " `"+m_BDM_POSO_HIT_DRUG_TYPE_ID        +"` = ?  \n"     // 9
                      " WHERE "   + m_BDM_POSO_HIT_PK         +"  =   '"  + pk  + "'\n";
         //......... recuperer la valeur absolue du hit .................................
         hit = qAbs(dbi_database()->isThisValueInTable(m_BDM_POSO_HIT_TBL_NAME, m_BDM_POSO_HIT_PK, pk, m_BDM_POSO_HIT_SCORE,"NO-OWNER-FILTER").toInt()) + 1;
         //......... si mode durci on incremente quand meme et on met en negatif .....................................................
         if ( mode!=C_BDM_Api::INCREASE_HIT )
            {hit  = -hit;
            }
       }
    //...... on recupere la premiere indication ...........
    QStringList indicParts;
    if ( indicList.count() ) indicParts = indicList[0].split('|');

    query.prepare(prepare);
    query.bindValue(0, "BDM");
    query.bindValue(1, "fr");
    query.bindValue(2, QString::number(hit));
    query.bindValue(3, ToStore);
    query.bindValue(4, "0");          // on se fout de l'ordre dans l'ordonnance pour une ligne de favoris
    if (indicParts.count()>=3)
       {
        query.bindValue(5, indicParts[0].left(20)); //  m_BDM_POSO_HIT_CODE_INDIC
        query.bindValue(6, indicParts[1].left(60)); //  m_BDM_POSO_HIT_CODE_INDIC_TYPE
        query.bindValue(7, indicParts[2].left(255)); //  m_BDM_POSO_HIT_LIBELLE_INDIC
       }
    else if( indicParts.count()>=1 )
       {
        query.bindValue(5, "");
        query.bindValue(6, "");
        query.bindValue(7, indicParts[0].left(255));
       }
    else
       {
        query.bindValue(5, "");
        query.bindValue(6, "");
        query.bindValue(7, "");
       }
    query.bindValue(8, drug_id);
    query.bindValue(9, drug_id_type);

    if ( !query.exec())
       { dbi_database()->outSQL_error( query, "ERROR  : C_BDM_Api::increasePosologieHit() queryInsert", prepare, __FILE__, __LINE__);
         return QString::null;
       }
    if (pk.length()==0 ) return dbi_database()->getLastPrimaryKey(  m_BDM_POSO_HIT_TBL_NAME, m_BDM_POSO_HIT_PK);
    return pk;
}
//------------------------ dbi_selectFavorisList -----------------------------------------
/*! \brief select drugs in hitlist from user input, for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param const QString textSelect user text fr select drugs.
 *  \param C_BDM_Api::flags filterFlag selection filter.
 *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
 */
/*
       QString  m_BDM_POSO_HIT_TBL_NAME;        // nom de la table de la table des favoris lies a un medicament
       QString  m_BDM_POSO_HIT_PK;              // clef primaire
       QString  m_BDM_POSO_HIT_OWNER;           // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
       QString  m_BDM_POSO_HIT_LANG;            // langue de la donnee (en fr etc...)
       QString  m_BDM_POSO_HIT_SCORE;           // niveau du hit
       QString  m_BDM_POSO_HIT_GRAMMAR;         // posologie liee a ce favoris
       QString  m_BDM_POSO_HIT_ORDER;           // niveau du hit (nombre d'utilisations sera utilise pour son classement)
       QString  m_BDM_POSO_HIT_CODE_INDIC;      // codage de l'indication liee a ce hit
       QString  m_BDM_POSO_HIT_CODE_INDIC_TYPE; // type de codage CIM10 etc ....
       QString  m_BDM_POSO_HIT_LIBELLE_INDIC;   // libelle de l'indication principale liee a ce favori
       QString  m_BDM_POSO_HIT_DRUG_ID;         // identificateur unique du medicament relie \303\240 ce hit
       QString  m_BDM_POSO_HIT_DRUG_TYPE_ID;    // type d'identificateur de l'id du medicament (CIP CIP7 CIS)

 */
const QList < C_BDM_DrugListRecord > C_BDM_PluginI::dbi_selectFavorisList (const QString &_text, C_BDM_PluginI::flags filterFlag  /* = C_BDM_Api::all_filter */ , int limit   /*=200*/)
{QList < C_BDM_DrugListRecord > productsList;
 Q_UNUSED(filterFlag);
 Q_UNUSED(limit);
 QString text    = _text.toUpper().remove('%');   // filtrage non sql donce virer les % qui manqueront pas d'arriver
 bool isIdList   = false;
 QString grammar = "";
 if (!dbi_database()->database().isOpen() && dbi_database()->database().open()== false)
    { dbi_database()->outMessage( QObject::tr("ERREUR : C_BDM_Api::dbi_selectFavorisList() database can not be opened"), __FILE__, __LINE__);
      return productsList;
    }
 QString  requete =  " SELECT DISTINCT `" + m_BDM_POSO_HIT_DRUG_ID            + "`,"     // 0    id
                     "`"                  + m_BDM_POSO_HIT_DRUG_TYPE_ID       + "`, "    // 1    id_type
                     "`"                  + m_BDM_POSO_HIT_GRAMMAR            + "` "     // 2    grammaire
                     " FROM  "            + m_BDM_POSO_HIT_TBL_NAME           ;

 if (text.startsWith("ID_LIST"))   // ID_LIST('123','236',789')
    {     requete += " WHERE `"          + m_BDM_POSO_HIT_DRUG_ID            + "` IN  " + text.mid(7);
          isIdList = true;   // pour eviter de fltrer par le texte
    }
          requete +=" GROUP BY `"        + m_BDM_POSO_HIT_DRUG_ID            + "`";

        //          " ORDER BY `"        + m_BDM_POSO_HIT_SCORE              + "` DESC LIMIT "        + QString::number(limit);

 productsList.clear();
 QSqlQuery query (requete , dbi_database()->database() );
 dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_Api::dbi_selectFavorisList()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           { grammar                     = CGestIni::Utf8_Query(query, 2) ;
             int  nextPos                = 0;
             QString commercialName      = CGestIni::getXmlData("gph_na",      grammar, &nextPos);
             QString compoXmlParts       = CGestIni::getXmlData("gph_dcl",     grammar, &nextPos);
             if (isIdList == false                          &&
                 text.length()                              &&
                 ! (commercialName.toUpper().contains(text) ||
                    compoXmlParts.toUpper() .contains(text)
                   )
                 ) continue;
             QString id                  = query.value(0).toString();
             QString id_type             = query.value(1).toString();
             QStringList dci_NameList    = CGestIni::getXmlDataList( "gph_dcn",     compoXmlParts);
             dci_NameList << "" << "" << ""; // on assure au moins une liste de trois elements
             productsList.append( C_BDM_DrugListRecord( id,
                                                        id_type,
                                                        commercialName,
                                                        dci_NameList[0],
                                                        dci_NameList[1],
                                                        dci_NameList[2],
                                                        "FAV",
                                                        "fr",
                                                        "",                  // ucd
                                                        "",                  // prix
                                                        "",                  // atc  (sera renseigne lors du clic)
                                                        "",                  // generique
                                                        ""                   // statut
                                                       )
                            );
           }    // end while (query.next())
    }  // endif (query.isActive() )
 return productsList;
}

//-------------------------------- dbi_verifyDataSourceVersion ------------------------------------------------------
/*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
 *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
 *  \return true in all is ok otherwise false (0)
 */
int C_BDM_PluginI::dbi_verifyDataSourceVersion()
{if (m_pDbi_Database==0)  return 0;
 if (dataSourceVersion() != dbi_lastDataSourceVersion())
    { if ( dbi_updateDrugLists()==0 )       return 0;
    }
 return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ PROTECTED ZONE -----------------------------------------------------------------------------

//-------------------------------- dbi_init_database ------------------------------------------------------
/*! \brief connect and init the additionnal database.
 *  this function needs an ANOTHER VALID and connected DATABASE to acced SQL
 *  to eventualy create additionnal database, if not already created.
 *  So for this, it is necessary that the DERIVED class has already initialised his proper database,
 *  and it is responsable to call this method with his proper database in databaseAlreadyInitialised
 *  argument.
 *  \param const QSqlDatabase &databaseAlreadyInitialised  SQL database already initialised
 *  \return true if all is OK false otherwise.
 */

bool    C_BDM_PluginI::dbi_init_database(const QString &baseName, const QString &prefixLabelName, const QString &owner, C_BaseCommon *databaseAlreadyInitialised)
{   //................... creer si besoin la base additionnelle ............................................
    //                    a besoin pour cela d'une AUTRE BASE VALIDE connectee pour acceder au SQL
    //                    de creation, il est donc necessaire que la base derivee par l'implementation
    //                    commerciale soit deja initialisee car c'est elle qui servira a acceder
    //                    au SQL a travers   databaseAlreadyInitialised
    //                    c'est le plugin de l'implementation qui a la responsabilite d'initialiser
    //                    cette base en appelant cette methode et qui est responsable du nom ici : baseName

    if (databaseAlreadyInitialised->createBase(baseName)==0)
       { qDebug()<<Q_FUNC_INFO<<"C_BDM_PluginI::dbi_init_database() error in createBase() '"<<baseName<<"'\n";
         return 0;
       }
    m_pDbi_Database  = new C_BaseCommon(databaseAlreadyInitialised->parent());
    if (m_pDbi_Database == 0) return 0;

    m_pDbi_Database->C_Log::setLogWidget(databaseAlreadyInitialised->logWidget());
    m_pDbi_Database->setProgressWidget(databaseAlreadyInitialised->progressWidget());

    if (m_pDbi_Database->BaseConnect( prefixLabelName+baseName,  // le label a le meme nom que la base de donnee
                                      databaseAlreadyInitialised->database().driverName(),
                                      baseName,  // nom de la base : "dbi_theriaque" "dbi_datasemp"
                                      databaseAlreadyInitialised->database().userName() ,
                                      databaseAlreadyInitialised->database().password() ,
                                      databaseAlreadyInitialised->database().hostName() ,
                                      databaseAlreadyInitialised->database().port()
                                    )
       )
       { //........... a ce stade il est possible d'utiliser les fonctions de base .........................
         //            deja implementees dans le plugin
         //            ici on cree la table de la liste des drogues
         m_pDbi_Database->addOwner(owner);
         dbi_verifyDataSourceVersion();  // verifier et eventuelle mis a jour de la liste des drogues
         dbi_createPosologieHit(C_BDM_PluginI::ONLY_IF_NOT_EXISTS );
         return 1;
       }
   return 0;
}

//-------------------------------- isInitialised ------------------------------------------------------
int C_BDM_PluginI::isInitialised()
{return m_isInitialised;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ PRIVATE ZONE -----------------------------------------------------------------------------

//-------------------------------- setInitialisedSate ------------------------------------------------------
void C_BDM_PluginI::setInitialisedSate(int state)
{m_isInitialised = state;
}

//-------------------------------- dbi_lastDataSourceVersion ------------------------------------------------------
/*! \brief return last datasource version
 */
QString C_BDM_PluginI::dbi_lastDataSourceVersion()
{   if (! dbi_database() )  return "";
    if (! dbi_database()->database().isOpen() )
       {m_pDbi_Database->outMessage( QObject::tr("ERREUR : C_BDM_PluginI::dbi_lastDataSourceVersion() database can not be open"), __FILE__, __LINE__);
        return "";
       }
    QSqlQuery query(QString::null , dbi_database()->database() );
    QString version  = "";
    QString requete  =  QString (" SELECT %1 FROM %2 "
                                 " WHERE  %3  = \"%4\" ").arg( m_BDM_VERSION_NUMBER, m_BDM_VERSION_TBL_NAME, m_BDM_VERSION_OWNER, owner());
    if (!query.exec(requete))
       {dbi_database()->outSQL_error( query, "ERREUR  : C_BDM_PluginI::dbi_lastDataSourceVersion()", requete, __FILE__, __LINE__);
        return QString::null;
       }
    if (query.isActive() )
        {while(query.next())    // il ne devrait il n'y en avoir qu'un
              {version = query.value(0).toString();      // date
              }
        }
    return version;
}

//-------------------------------- dbi_updateDrugLists ------------------------------------------------------
/*! \brief for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \return true in all is ok otherwise false
 */
bool C_BDM_PluginI::dbi_updateDrugLists()
{   if (! dbi_database() )                         return false;
    if (  getDrugList_Start()==0 )                 return false;
    if (! dbi_database()->database().isOpen() )
       {  dbi_database()->outMessage( QObject::tr("ERREUR : C_BDM_PluginI::dbi_updateDrugLists() database can not be open"), __FILE__, __LINE__);
          return 0;
       }
    //....................... on cree la table ......................................
    if ( dbi_database()->dropTable(  m_BDM_VERSION_TBL_NAME)==0) return 0;
    if ( dbi_database()->createTable("CREATE TABLE `"+m_BDM_VERSION_TBL_NAME+"` ("
                    "`"+m_BDM_VERSION_PK        +"` BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`"+m_BDM_VERSION_OWNER     +"` VARCHAR(40) ,"
                    "`"+m_BDM_VERSION_LANG      +"` VARCHAR(4)  ,"
                    "`"+m_BDM_VERSION_NUMBER    +"` VARCHAR(120) ,"
                    "PRIMARY KEY (`"+m_BDM_VERSION_PK+"`)"
                    ")"
                    )==0) return 0;

    //....................... on cree la table ......................................
    if ( dbi_database()->dropTable(  m_BDM_DRUGLIST_TBL_NAME)==0) return 0;
    if ( dbi_database()->createTable("CREATE TABLE `"+m_BDM_DRUGLIST_TBL_NAME+"` ("
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
                    "`"+m_BDM_DRUGLIST_GENERIQUE +"` VARCHAR(1)  ,"
                    "`"+m_BDM_DRUGLIST_STATUT    +"` VARCHAR(100)  ,"
                    "`"+m_BDM_DRUGLIST_SPEC_FK   +"` BIGINT      ,"
                    "PRIMARY KEY (`"+m_BDM_DRUGLIST_PK+"`)"
                    ")"
                    )==0) return 0;

    long nbRecordsToParse = drugsList_Count();
    dbi_database()->outMessage(  QObject::tr("Nb records <font color=#00c0ff>%1</font> : <font color=#00ffff>%2</font>").arg(m_BDM_DRUGLIST_TBL_NAME,QString::number(nbRecordsToParse)));
    long position         = 0;

    if (progressWidget()) progressWidget()->setRange(0, nbRecordsToParse/10);
    QProgressDialog progress(QObject::tr("Drugs List Initialisation in action..."), QObject::tr("Abort"), 0, nbRecordsToParse/10, 0);
    progress.setWindowModality(Qt::WindowModal);

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
                                     "`"+m_BDM_DRUGLIST_GENERIQUE +"`,"     // 11
                                     "`"+m_BDM_DRUGLIST_STATUT    +"`,"     // 12
                                     "`"+m_BDM_DRUGLIST_SPEC_FK   +"` "     // 13
                                     ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    C_BDM_DrugListRecord c_bdm_druglistrecord;
    QSqlQuery queryInsert(QString::null , dbi_database()->database() );
    while (getDrugList_NextRecord(c_bdm_druglistrecord))
        {queryInsert.prepare(prepare);
         queryInsert.bindValue(0,  c_bdm_druglistrecord.owner());
         queryInsert.bindValue(1,  c_bdm_druglistrecord.lang());
         queryInsert.bindValue(2,  c_bdm_druglistrecord.id());
         queryInsert.bindValue(3,  c_bdm_druglistrecord.id_type());
         queryInsert.bindValue(4,  c_bdm_druglistrecord.atc());
         queryInsert.bindValue(5,  c_bdm_druglistrecord.commercialName());
         queryInsert.bindValue(6,  c_bdm_druglistrecord.dci_1Name());
         queryInsert.bindValue(7,  c_bdm_druglistrecord.dci_2Name());
         queryInsert.bindValue(8,  c_bdm_druglistrecord.dci_3Name());
         queryInsert.bindValue(9,  c_bdm_druglistrecord.nb_up());
         queryInsert.bindValue(10, c_bdm_druglistrecord.ucd_price());
         queryInsert.bindValue(11, c_bdm_druglistrecord.generique());
         queryInsert.bindValue(12, c_bdm_druglistrecord.statut());
         queryInsert.bindValue(13, "0");                                // pour l'instant les spec son vides
         if ( !queryInsert.exec())
            { dbi_database()->outSQL_error( queryInsert, "ERREUR  : C_BDM_Api::dbi_updateDrugLists() queryInsert", prepare, __FILE__, __LINE__);
              return 0;
            }
        if (progress.wasCanceled())  return 0;
        ++position;
        if (progressWidget())  {progressWidget()->setValue(position/10); qApp->processEvents();qApp->processEvents();}
        progress.setValue(position/10);
        }
     progress.setValue(nbRecordsToParse/10);
     //.......................... numero de version ..........................................
     prepare  =              (" INSERT INTO "+m_BDM_VERSION_TBL_NAME+" ( "
                              "`"+m_BDM_VERSION_OWNER     +"`,"     // 0
                              "`"+m_BDM_VERSION_LANG      +"`,"     // 1
                              "`"+m_BDM_VERSION_NUMBER    +"` "     // 2
                             ") VALUES (?,?,?)");

    QSqlQuery queryVersion(QString::null , dbi_database()->database() );
    queryVersion.prepare(prepare);
    queryVersion.bindValue(0,  owner());
    queryVersion.bindValue(1,  lang());
    queryVersion.bindValue(2,  dataSourceVersion());
    if ( !queryVersion.exec())
       {  dbi_database()->outSQL_error( queryVersion, "ERREUR  : C_BDM_Api::dbi_updateDrugLists() queryVersion", prepare, __FILE__, __LINE__);
          return 0;
       }
    return 1;
}
//-------------------------------- dbi_createPosologieHit ------------------------------------------------------
/*! \brief this table records a posologie drug hit parade adjusted in real time of use.
 *  \param int mode  default = ONLY_IF_NOT_EXISTS/create table only if not exists, otherwise create table even if exists.
 *  \return true in all is ok otherwise false
 */
int C_BDM_PluginI::dbi_createPosologieHit(int mode  /* = ONLY_IF_NOT_EXISTS */)
{
    if (! dbi_database() )                         return 0;
    if (! dbi_database()->database().isOpen() )
       {  dbi_database()->outMessage( QObject::tr("ERREUR : C_BDM_PluginI::createPosologieHit() database can not be open"), __FILE__, __LINE__);
          return 0;
       }

    //....................... on cree la table ......................................
    if (mode == C_BDM_PluginI::ONLY_IF_NOT_EXISTS &&  dbi_database()->isThisTableExist(m_BDM_POSO_HIT_TBL_NAME) )      return 1; // si exite on ne fait rien
    if (dbi_database()->dropTable  (  m_BDM_POSO_HIT_TBL_NAME )==0)                                                    return 0; // on la detruit
    if (dbi_database()->createTable(
                    "CREATE TABLE `"+m_BDM_POSO_HIT_TBL_NAME+"` ("                             // nom de la table de la table des favoris lies a un medicament
                    "`"+m_BDM_POSO_HIT_PK              +"` BIGINT  NOT NULL AUTO_INCREMENT,"   // clef primaire
                    "`"+m_BDM_POSO_HIT_OWNER           +"` VARCHAR(40)  ,"                     // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
                    "`"+m_BDM_POSO_HIT_LANG            +"` VARCHAR(4)   ,"                     // langue de la donnee (en fr etc...)
                    "`"+m_BDM_POSO_HIT_SCORE           +"` BIGINT       ,"                     // niveau du hit (nombre d'utilisation sera utilise pour son classement)
                    "`"+m_BDM_POSO_HIT_GRAMMAR         +"` VARCHAR(4048),"                     // grammaire de la posologie liee a ce favoris
                    "`"+m_BDM_POSO_HIT_ORDER           +"` BIGINT       ,"                     // ordre rigidifie du hit donne la position du favori quelque soit son score
                    "`"+m_BDM_POSO_HIT_CODE_INDIC      +"` VARCHAR(20)  ,"                     // codage de l'indications liees a ce hit
                    "`"+m_BDM_POSO_HIT_CODE_INDIC_TYPE +"` VARCHAR(60)  ,"                     // type de codage CIM10 etc ....
                    "`"+m_BDM_POSO_HIT_LIBELLE_INDIC   +"` VARCHAR(256) ,"                     // libelle de l'indication principale liee a ce hit
                    "`"+m_BDM_POSO_HIT_DRUG_ID         +"` VARCHAR(40)  ,"                     // identificateur unique du medicament relie \303\240 ce hit
                    "`"+m_BDM_POSO_HIT_DRUG_TYPE_ID    +"` VARCHAR(8)   ,"                     // type d'identificateur de l'id du medicament (CIP CIP7 CIS)
                    "PRIMARY KEY (`"+m_BDM_POSO_HIT_PK+"`)"
                    ")"
                    )==0) return 0;

    return 1;
}

//================================ C_BDM_Api ==========================================================================
//-------------------------------- C_BDM_Api ------------------------------------------------------
/*! \brief constructor
 *  \param const QString   &confData           fields definitions.
 *  \param const QString   &connectParams      connexions parameters.
 *  \param const QString   &imagePath          image path.
 *  \param const QString   &pathAppli          application path.
 *  \param const QString   &pathIni            application configuration path.
 *  \param QObject         *parent             pointer QObject parent.
 *  \param QTextEdit       *pLogWidget         pointer QTextEdit log widget (can be omitted).
 *  \param int              userMonographie    default value is 1, zero/if we don't want UserMonographie (expl we need just interaction control)
*/
C_BDM_Api::C_BDM_Api( const QString   &confData,                        // definition des champs de la base de donn\303\251e (ceux de la liste des drogues)
                      const QString   &prefixLabel,                     // prefixage du label (permettre plusieurs base)
                      const QString   &connectParams ,                  // parametres de connexion a la base de donn\303\251es
                      const QString   &imagePath,                       // chemin des images
                      const QString   &pathAppli,                       // chemin de l'applicatif
                      const QString   &pathIni,                         // chemin du fichier de configuration general
                      QObject         *parent,                          // objet parent
                      QTextEdit       *pLogWidget,                      // = 0 pointer QTextEdit log widget (can be omitted because zro by default).
                      int              userMonographie                  // default value is 1, zero/if we don't want UserMonographie (expl we need just interaction control)
                    )
: C_BaseCommon(parent), C_BDM_ApiVar( confData )
{

#ifndef QT_5
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
#endif

 m_pC_BDM_PluginI                = 0;
 m_isValid                       = 0;
 m_DrugListNb                    = -1;
 m_imagePath                     = imagePath;
 m_pathAppli                     = pathAppli;
 m_pathIni                       = pathIni;
 m_stopAllInterraction           = 0;
 m_defaultDurationInSeconds      = CGestIni::Param_ReadUniqueParam(connectParams.toLatin1(),"medicatux session","default duration in seconds").toInt();
 this->setObjectName(prefixLabel+"C_BDM_Api");
 if (pLogWidget) this->setLogWidget(pLogWidget);
 if (confData.length()==0) qDebug()<<"WARNING : C_BDM_Api::C_BDM_Api() empty confData";
 if ( connectBase(connectParams, prefixLabel )==0 )               {m_isValid = 0; return;}
 if (   userMonographie                                    &&    //  if User Monographie is nedeed
      ! isThisTableExist(m_BDM_USER_MONOGRAPHIE_TBL_NAME)  &&    //  and not present,
      ! createCustomMonographieTable()                           //  and cant be created
    )
    { m_isValid = 0;                                             // invalid API
      return;
    }
 m_isValid = 1;
}

//-------------------------------- ~C_BDM_Api ------------------------------------------------------
/*! \brief destructor  C_BDM_Api
*/
C_BDM_Api::~C_BDM_Api()
{qDebug() <<  objectName() + tr(" is deleted");
}

//-------------------------------- connectToDataSource ------------------------------------------------------
/*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
 *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
 *  \return true in all is ok otherwise false (0)
 */
int C_BDM_Api::connectToDataSource( C_BDM_PluginI *pC_BDM_PluginI)
{m_DrugListNb = -1;
 m_isValid    =  0;
 if ( !pC_BDM_PluginI->isInitialised() ) return 0;
 m_pC_BDM_PluginI = pC_BDM_PluginI;
 m_DrugListNb     = m_pC_BDM_PluginI->dbi_DrugListCount();
 m_isValid        = 1;
 return 1;
}
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
int C_BDM_Api::changeDataBaseConnexion(const QString &driver, const QString &schema, const QString &user, const QString &password, const QString &hostname, const QString &port)
{ m_isValid = 0;
  if ( C_BaseCommon::changeDataBaseConnexion( driver, schema, user, password, hostname, port )==0) { return 0;}
  if ( ! isThisTableExist(m_BDM_USER_MONOGRAPHIE_TBL_NAME)  &&
       ! createCustomMonographieTable() )                                                          { return 0;}
  if ( m_pC_BDM_PluginI ==0 )                                                                      { return 0;}
  if ( !m_pC_BDM_PluginI->isInitialised() )                                                        { return 0;}
  m_pC_BDM_PluginI->dbi_verifyDataSourceVersion();
  m_DrugListNb  = m_pC_BDM_PluginI->dbi_DrugListCount();
  m_isValid     = 1;
  return 1;
}

//-------------------------------- createCustomMonographieTable ------------------------------------------------------
/*! \brief create custom monographie table
 *  \return 1/all is ok 0/list not created
 */
int C_BDM_Api::createCustomMonographieTable()
{
 if ( createTable("CREATE TABLE `" + m_BDM_USER_MONOGRAPHIE_TBL_NAME + "` ("
                "`"+m_BDM_USER_MONOGRAPHIE_PK        +"` BIGINT  NOT NULL AUTO_INCREMENT,"
                "`"+m_BDM_USER_MONOGRAPHIE_OWNER     +"` VARCHAR(40) ,"
                "`"+m_BDM_USER_MONOGRAPHIE_LANG      +"` VARCHAR(4)  ,"
                "`"+m_BDM_USER_MONOGRAPHIE_ID        +"` VARCHAR(40) ,"
                "`"+m_BDM_USER_MONOGRAPHIE_TYPE_ID   +"` VARCHAR(8)  ,"
                "`"+m_BDM_USER_MONOGRAPHIE_LIBELLE   +"` VARCHAR(128),"
                "`"+m_BDM_USER_MONOGRAPHIE_AUTEUR    +"` VARCHAR(128),"
                "`"+m_BDM_USER_MONOGRAPHIE_DATE      +"` VARCHAR(128),"
                "`"+m_BDM_USER_MONOGRAPHIE_HTML_BLOB +"` LONGBLOB,"
                "PRIMARY KEY (`"+m_BDM_USER_MONOGRAPHIE_PK+"`)"
                ")"
                )==0) return 0;
 return 1;
}
//-------------------------------- selectCustomMonographie --------------------------------------------
/*! \brief make select query to select drugs list from user input, for a good performance
 *  \param C_BDM_DrugListRecord  &product from wich we wants informations
 *  \param QString    libelle the monographie
 *  \param QString    author of the monographie
 *  \param QDateTime  dt_deb start date of informations to retrieve
 *  \param QDateTime  dt_deb end date of informations  to retrieve
 *  \return QString which is monographie
 */
QStringList C_BDM_Api::selectCustomMonographie(const C_BDM_DrugListRecord  &c_bdm_druglistrecord ,  const QString &libelle, const QString &author, const QDateTime &dt_deb, const QDateTime &dt_end)
{
  QStringList ret_Html_List;
  QString           html = "";
  QString        requete =        " SELECT "
                                  "`"+m_BDM_USER_MONOGRAPHIE_LIBELLE          +"`,"     // 0
                                  "`"+m_BDM_USER_MONOGRAPHIE_AUTEUR           +"`,"     // 1
                                  "`"+m_BDM_USER_MONOGRAPHIE_DATE             +"`,"     // 1
                                  "`"+m_BDM_USER_MONOGRAPHIE_HTML_BLOB        +"`,"     // 3
                                  "`"+m_BDM_USER_MONOGRAPHIE_PK               +"` "     // 4
                                  " FROM  " + m_BDM_USER_MONOGRAPHIE_TBL_NAME +
                                  " WHERE " + m_BDM_USER_MONOGRAPHIE_ID       + " = '" +c_bdm_druglistrecord.id() + "'";
 if (libelle.length())
    {requete  +=  " AND  "+ m_BDM_USER_MONOGRAPHIE_LIBELLE + " LIKE '%" + libelle + "%'";
    }
 if (author.length())
    {requete  +=  " AND  "+ m_BDM_USER_MONOGRAPHIE_AUTEUR  + " LIKE '%" + author + "%'";
    }
 if (dt_deb.isValid() && dt_end.isValid())
    {requete  +=  " AND  "+ m_BDM_USER_MONOGRAPHIE_DATE  + " >= '" + dt_deb.toString("yyyy-MM-dd hh:mm:ss") + "'"
                  " AND  "+ m_BDM_USER_MONOGRAPHIE_DATE  + " <= '" + dt_end.toString("yyyy-MM-dd hh:mm:ss") + "'";
    }
 if (!database().isOpen() && database().open()== false)
    { outMessage( QObject::tr("ERREUR : C_BDM_Api::selectCustomMonographie() database can not be opened"), __FILE__, __LINE__);  return ret_Html_List;
    }
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : C_BDM_Api::selectCustomMonographie()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           {html.clear();
            QString title    = CGestIni::Utf8_Query(query, 0);      // libelle de l'information ou de la note
            QString pk       = query.value(4).toString();           // primary key de l'enregistrement de l'information ou de la note
            QString urlAncre = "ancre_id_["+ pk + "-" + title + "]_";
            //html            += "<hr />";
            html            += "<a name=\""          + urlAncre +"\"></a><br />";                                                       // l'ancre d'acces
            html            += "<hr/><p align=\"center\"><font color=#0000ff><img src=\"NewNotePicto.png\">&nbsp;&nbsp;&nbsp;<b>NOTE COMPL&#201;MENTAIRE PERSONNELLE</b></font>"
                                "&nbsp;&nbsp;&nbsp;<a href=\"#monographie_start\"><img src=\"HomePicto.png\"> Revenir au d&#233;but de la page</a></p><hr/>" ;
            html            += tr("Title: ")         + title                                                        + "<br />";
            html            += tr("Added by: ")      + CGestIni::Utf8_Query(query, 1)                               + "<br />";
            html            += tr("Date: ")          + query.value(2).toDateTime().toString("dd-MM-yyyy hh:mm:ss")  + "<br />";
            html            +=                         CGestIni::Utf8_Query(query, 3)                               + "<br />";
            ret_Html_List  << html;
           }    // end while (query.next())
    }  // endif (query.isActive() )
 return ret_Html_List;
}
//-------------------------------- get_CustomMonographie --------------------------------------------
/*! \brief make select query to select drugs list from user input, for a good performance
 *  \param C_BDM_DrugListRecord  &product from wich we wants informations
 *  \param QString &pk primary key of the recor to retrieve
 *  \param QString    libelle the monographie to get
 *  \param QString    author of the monographie  to get
 *  \param QDateTime  dt_deb start date  to get
 *  \return QString which is monographie WITHOUT title header
 */
QString C_BDM_Api::get_CustomMonographie( const QString &pk, QString &libelle,  QString &author,  QDateTime &dt_deb)
{
  QString           html = "";
  QString        requete =        " SELECT "
                                  "`"+m_BDM_USER_MONOGRAPHIE_LIBELLE          +"`,"     // 0
                                  "`"+m_BDM_USER_MONOGRAPHIE_AUTEUR           +"`,"     // 1
                                  "`"+m_BDM_USER_MONOGRAPHIE_DATE             +"`,"     // 1
                                  "`"+m_BDM_USER_MONOGRAPHIE_HTML_BLOB        +"` "     // 3
                                  " FROM  " + m_BDM_USER_MONOGRAPHIE_TBL_NAME +
                                  " WHERE " + m_BDM_USER_MONOGRAPHIE_PK       + " = '" +pk+ "'";
 QSqlQuery query (requete , database() );
 outSQL_error( query, "ERREUR  : C_BDM_Api::get_CustomMonographie()", requete, __FILE__, __LINE__);
 if (query.isActive() )
    {while (query.next())
           {html.clear();
            libelle    = CGestIni::Utf8_Query(query, 0);      // libelle de l'information ou de la note
            author     = CGestIni::Utf8_Query(query, 1);
            dt_deb     = query.value(2).toDateTime();
            html       = CGestIni::Utf8_Query(query, 3);
           }    // end while (query.next())
    }  // endif (query.isActive() )
 return html;
}

//-------------------------------- add_Monographie_to_CustomMonographie --------------------------------------------
/*! \brief add in dbm a product Monographie to a product
 *  \param QString    libelle of the title of monographie
 *  \param QString    author of the monographie
 *  \param QByteArray datas the datas of monographie
 *  \param QDateTime  date of informations
 *  \param C_BDM_DrugListRecord  &product on wich add informations
 *  \return 1/all is ok 0/product not added
 */
int C_BDM_Api::add_Monographie_to_CustomMonographie(const QString &libelle, const QString &author, const QByteArray &datas, const QDateTime &dt, const C_BDM_DrugListRecord  &c_bdm_druglistrecord)
{
    if (!database().isOpen() && database().open()== false)
       { outMessage( QObject::tr("ERREUR : C_BDM_Api::add_Monographie_to_CustomMonographie() database can not be opened"), __FILE__, __LINE__);  return 0;
       }
    //......................... on renseigne la liste ...................................
    QString prepare  =              (" INSERT INTO "+ m_BDM_USER_MONOGRAPHIE_TBL_NAME +"( "
                                     "`"+m_BDM_USER_MONOGRAPHIE_OWNER     +"`,"     // 0
                                     "`"+m_BDM_USER_MONOGRAPHIE_LANG      +"`,"     // 1
                                     "`"+m_BDM_USER_MONOGRAPHIE_ID        +"`,"     // 2
                                     "`"+m_BDM_USER_MONOGRAPHIE_TYPE_ID   +"`,"     // 3
                                     "`"+m_BDM_USER_MONOGRAPHIE_LIBELLE   +"`,"     // 4
                                     "`"+m_BDM_USER_MONOGRAPHIE_AUTEUR    +"`,"     // 5
                                     "`"+m_BDM_USER_MONOGRAPHIE_DATE      +"`,"     // 6
                                     "`"+m_BDM_USER_MONOGRAPHIE_HTML_BLOB +"` "     // 7
                                     ") VALUES (?,?,?,?,?,?,?,?)");
    QSqlQuery queryInsert(QString::null , database() );
    //for (int i = 0; i < drugsList.size(); ++i)
        { //C_BDM_DrugListRecord c_bdm_druglistrecord = drugsList.at(i);
          queryInsert.prepare(prepare);
          queryInsert.bindValue(0,  c_bdm_druglistrecord.owner());
          queryInsert.bindValue(1,  c_bdm_druglistrecord.lang());
          queryInsert.bindValue(2,  c_bdm_druglistrecord.id());
          queryInsert.bindValue(3,  c_bdm_druglistrecord.id_type());
          queryInsert.bindValue(4,  libelle);
          queryInsert.bindValue(5,  author);
          queryInsert.bindValue(6,  dt);
          queryInsert.bindValue(7,  datas);
          if ( !queryInsert.exec())
             { outSQL_error( queryInsert, "ERREUR  : C_BDM_Api::add_Monographie_to_CustomMonographie() queryInsert", prepare, __FILE__, __LINE__);
               return 0;
             }
        }
    return 1;
}
//-------------------------------- update_CustomMonographie --------------------------------------------
/*! \brief delete Custom Monographie
 *  \param QString    pk primary key of the recodr to update
 *  \param QString    libelle of the title of monographie
 *  \param QString    author of the monographie
 *  \param QByteArray datas the datas of monographie
 *  \param QDateTime  date of informations
 *  \return 1/all is ok 0/product not updated
 */
bool C_BDM_Api::update_CustomMonographie(const QString &pk, const QString &libelle, const QString &author, const QDateTime &dt, const QByteArray &datas)
{
    if (!database().isOpen() && database().open()== false)
       { outMessage( QObject::tr("ERREUR : C_BDM_Api::update_CustomMonographie() database can not be opened"), __FILE__, __LINE__);  return 0;
       }
    QString prepare  = " UPDATE " + m_BDM_USER_MONOGRAPHIE_TBL_NAME    + " SET  \n"
                       " `"+m_BDM_USER_MONOGRAPHIE_LIBELLE             +"` = ? ,\n"     // 0
                       " `"+m_BDM_USER_MONOGRAPHIE_AUTEUR              +"` = ? ,\n"     // 1
                       " `"+m_BDM_USER_MONOGRAPHIE_DATE                +"` = ? ,\n"     // 2
                       " `"+m_BDM_USER_MONOGRAPHIE_HTML_BLOB           +"` = ?  \n"     // 3
                       " WHERE "   + m_BDM_USER_MONOGRAPHIE_PK         +"  =   '"  + pk  + "'\n";

    //...... on binde ...........
    QSqlQuery query (database());
    query.prepare(prepare);
    query.bindValue(0, libelle);
    query.bindValue(1, author);
    query.bindValue(2, dt);
    query.bindValue(3, datas);

    if ( !query.exec())
       { outSQL_error( query, "ERROR  : C_BDM_Api::update_CustomMonographie() queryInsert", prepare, __FILE__, __LINE__);
         return 0;
       }
    return 1;
}

//-------------------------------- delete_CustomMonographie --------------------------------------------
/*! \brief delete Custom Monographie
 *  \param QString    pk primary key of the recodr to delete
 *  \return 1/all is ok 0/product not deleted
 */
bool C_BDM_Api::delete_CustomMonographie(const QString &pk)
{
    if (!database().isOpen() && database().open()== false)
       { outMessage( QObject::tr("ERREUR : C_BDM_Api::delete_CustomMonographie() database can not be opened"), __FILE__, __LINE__);  return 0;
       }
    QString requete    =         " DELETE FROM " + m_BDM_USER_MONOGRAPHIE_TBL_NAME  +
                                 " WHERE "       + m_BDM_USER_MONOGRAPHIE_PK        + "='" + pk      + "' ";
    QSqlQuery query (database());
    if ( !query.exec(requete) )
       { outSQL_error( query, tr("ERROR: C_BDM_Api::delete_CustomMonographie() "), requete, __FILE__, __LINE__);
         return false;
       }
    return true;
}

//-------------------------------- customProductsTableName [static] ------------------------------------------------------
/*! \brief create custom drugs list table name from userName and listName
 *  \param QString userName the user name for this custom list.
 *  \param QString listName the name for this custom list.
 *  \param QString prefix to name this table.
 *  \return QString wich is table name
 */
// le PB : le nommage des tables sous W ne respecte pas forcement les Maj/Min
//         lorsque l'on cree une table MachinTrucBIDULE il sera cree machintrucbidule.
//         Lors du listage des tables ce sera machintrucbidule qui sera retourne
//         donc pour contourner ce delice incertain que nous offre W TOUT SERA EN MINUSCULE
QString C_BDM_Api::customProductsTableName(const QString &userName, const QString &listName, const QString &prefix /* = "perso_drugslist" */)
{return (prefix+"_"+listName+"_"+userName).toLower();
}

//-------------------------------- createCustomProductsList ------------------------------------------------------
/*! \brief create custom drugs list
 *  \param QString userName the user name for this custom list.
 *  \param QString listName the name for this custom list.
 *  \return 1/all is ok 0/list not created
 */
// le PB : le nommage des tables sous W ne respecte pas forcement les Maj/Min
//         lorsque l'on cree une table MachinTrucBIDULE il sera cree machintrucbidule.
//         Lors du listage des tables ce sera machintrucbidule qui sera retourne
//         donc pour contourner ce delice incertain que nous offre W TOUT SERA EN MINUSCULE
int C_BDM_Api::createCustomProductsList(const QString &userName, const QString &listName)
{
 if ( createTable("CREATE TABLE `" + customProductsTableName(userName,listName) + "` ("
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
                "`"+m_BDM_DRUGLIST_UCD_PRICE +"` CHAR(15)    ,"
                "`"+m_BDM_DRUGLIST_GENERIQUE +"` VARCHAR(1)  ,"
                "`"+m_BDM_DRUGLIST_STATUT    +"` VARCHAR(100)  ,"
                "`"+m_BDM_DRUGLIST_SPEC_FK   +"` BIGINT      ,"
                "PRIMARY KEY (`"+m_BDM_DRUGLIST_PK+"`)"
                ")"
                )==0) return 0;
 return 1;
}

//-------------------------------- deleteCustomProductsList ------------------------------------------------------
/*! \brief delete custom drugs list
 *  \param QString userName the user name of this custom list.
 *  \param QString listName the name of this custom list.
 *  \return 1/all is ok 0/list not deleted
 */
// le PB : le nommage des tables sous W ne respecte pas forcement les Maj/Min
//         lorsque l'on cree une table MachinTrucBIDULE il sera cree machintrucbidule.
//         Lors du listage des tables ce sera machintrucbidule qui sera retourne
//         donc pour contourner ce delice incertain que nous offre W TOUT SERA EN MINUSCULE
int C_BDM_Api::deleteCustomProductsList(const QString &userName, const QString &listName)
{
 return dropTable( customProductsTableName(userName,listName) );
}

//-------------------------------- getListCustomProductsList ------------------------------------------------------
/*! \brief  return a list of custom drugs list for a user
 *  \param  QString userName the user name
 *  \return QStringList of custom drugs list for this user
 */
// le PB : le nommage des tables sous W ne respecte pas forcement les Maj/Min
//         lorsque l'on cree une table MachinTrucBIDULE il sera cree machintrucbidule.
//         Lors du listage des tables ce sera machintrucbidule qui sera retourne
//         donc pour contourner ce delice incertain que nous offre W TOUT SERA EN MINUSCULE
QStringList C_BDM_Api::getListCustomProductsList(const QString &_userName)
{
 QString     userName  = _userName.toLower();
 QString     tableName = "";
 QStringList tableList = database().tables(QSql::Tables );
 QStringList retList;
 for (int i=0; i<tableList.size();++i)
     {tableName = tableList[i];
      if ( tableName.startsWith("perso_drugslist_") && tableName.endsWith(userName) )
         { int posToCut          = tableName.indexOf("_"+userName);
           QString nameToDisplay = tableList[i].left(posToCut);
           nameToDisplay         = nameToDisplay.mid(16,1).toUpper()+ nameToDisplay.mid(17); // pour faire plus jouli on met la premiere lettre en Maj
           if (retList.indexOf(nameToDisplay) == -1 && isThisTableExist(tableName)) retList.append(nameToDisplay);
         }
     }
 return retList;
}

//-------------------------------- add_Product_to_CustomProductsList --------------------------------------------
/*! \brief add in dbm a product list to a custom products list
 *  \param QString userName the user name
 *  \param QString listName the name for this custom list.
 *  \param QList < C_BDM_DrugListRecord > &drugs List products to add
 *  \return 1/all is ok 0/product not added
 */
int C_BDM_Api::add_Product_to_CustomProductsList(const QString &_userName, const QString &listName, const QList < C_BDM_DrugListRecord > &drugsList)
{
    if (drugsList.size()==0)
       { outMessage( QObject::tr("ERREUR : C_BDM_Api::add_Product_to_CustomProductsList() empty drugs list"), __FILE__, __LINE__);            return 0;
       }
    if (!database().isOpen() && database().open()== false)
       { outMessage( QObject::tr("ERREUR : C_BDM_Api::add_Product_to_CustomProductsList() database can not be opened"), __FILE__, __LINE__);  return 0;
       }
    //......................... on renseigne la liste ...................................
    QString prepare  =              (" INSERT INTO "+customProductsTableName(_userName,listName) +"( "
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
                                     "`"+m_BDM_DRUGLIST_GENERIQUE +"`,"     // 11
                                     "`"+m_BDM_DRUGLIST_STATUT    +"`,"     // 12
                                     "`"+m_BDM_DRUGLIST_SPEC_FK   +"` "     // 13
                                     ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    QSqlQuery queryInsert(QString::null , database() );
    for (int i = 0; i < drugsList.size(); ++i)
        { C_BDM_DrugListRecord c_bdm_druglistrecord = drugsList.at(i);
          queryInsert.prepare(prepare);
          queryInsert.bindValue(0,  c_bdm_druglistrecord.owner());
          queryInsert.bindValue(1,  c_bdm_druglistrecord.lang());
          queryInsert.bindValue(2,  c_bdm_druglistrecord.id());
          queryInsert.bindValue(3,  c_bdm_druglistrecord.id_type());
          queryInsert.bindValue(4,  c_bdm_druglistrecord.atc());
          queryInsert.bindValue(5,  c_bdm_druglistrecord.commercialName());
          queryInsert.bindValue(6,  c_bdm_druglistrecord.dci_1Name());
          queryInsert.bindValue(7,  c_bdm_druglistrecord.dci_2Name());
          queryInsert.bindValue(8,  c_bdm_druglistrecord.dci_3Name());
          queryInsert.bindValue(9,  c_bdm_druglistrecord.nb_up());
          queryInsert.bindValue(10, c_bdm_druglistrecord.ucd_price());
          queryInsert.bindValue(11, c_bdm_druglistrecord.generique());
          queryInsert.bindValue(12, c_bdm_druglistrecord.statut());
          queryInsert.bindValue(13, "0");                                // pour l'instant les spec son vides
          if ( !queryInsert.exec())
             { outSQL_error( queryInsert, "ERREUR  : C_BDM_Api::add_Product_to_CustomProductsList() queryInsert", prepare, __FILE__, __LINE__);
               return 0;
             }
      }
    return 1;
}

//-------------------------------- del_Product_from_CustomProductsList --------------------------------------------
/*! \brief remove a product to a custom products list
 *  \param QString userName the user name
 *  \param QString listName the name for this custom list.
 *  \param const C_BDM_DrugListRecord &drugListRecord product to remove
 *  \return true/all is ok false/product not deleted
 */
bool C_BDM_Api::del_Product_from_CustomProductsList(const QString &userName, const QString &listName, const C_BDM_DrugListRecord &drugListRecord)
{
     QSqlQuery query( database() );
     QString requete    =         " DELETE FROM " + customProductsTableName(userName,listName)           +
                                  " WHERE "       + m_BDM_DRUGLIST_ID      + "='" + drugListRecord.id()      + "' "+
                                  " AND "         + m_BDM_DRUGLIST_TYPE_ID + "='" + drugListRecord.id_type() + "' ";
     if ( !query.exec(requete) )
        { outSQL_error( query, tr("ERROR: C_BDM_Api::del_Product_from_CustomProductsList() "), requete, __FILE__, __LINE__);
          return false;
        }
     return true;
}
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
QString C_BDM_Api::makeSelectCustomProductsListQuery(const QString &_userName, const QString &listName, const    QString &text, C_BDM_PluginI::flags filterFlag)
{

 QString        requete =         " SELECT "
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
                                  "`"+m_BDM_DRUGLIST_GENERIQUE +"`,"     // 11
                                  "`"+m_BDM_DRUGLIST_STATUT    +"`,"     // 12
                                  "`"+m_BDM_DRUGLIST_SPEC_FK   +"` "     // 13
                                  " FROM " + customProductsTableName(_userName,listName) ;

 if (filterFlag&C_BDM_PluginI::cip_list)
    {requete  +=  QString(" WHERE  %1 IN (%2) ").arg(m_BDM_DRUGLIST_ID, text);        // liste de cip de la forme 'cip1','cip2','cip3'
    }
 else if (text.length())
    {requete  +=  QString(" WHERE (");
     if (filterFlag&C_BDM_PluginI::nom_filter)  requete  += m_BDM_DRUGLIST_LIBELLE     + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::atc_filter)  requete  += m_BDM_DRUGLIST_ATC         + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::dci_filter)  requete  += m_BDM_DRUGLIST_DCI_1       + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::cip_filter)  requete  += m_BDM_DRUGLIST_ID          + " LIKE \"%1%\" OR ";
     if (filterFlag&C_BDM_PluginI::dc3_filter)  requete  += m_BDM_DRUGLIST_DCI_2       + " LIKE \"%1%\" OR " + m_BDM_DRUGLIST_DCI_3 + " LIKE \"%1%\" OR ";
     requete.chop(4);                                // virer le dernier " OR "
     requete  += ") ";                               // fermer la parenthese du WHERE
     requete   = requete.arg(text.toUpper());        // placer les caracteres de l'utilisateur
     if (filterFlag&C_BDM_PluginI::gen_filter) {requete  +=  " AND (" + m_BDM_DRUGLIST_GENERIQUE   + " IN ('G','R')) ";}
     if (filterFlag&C_BDM_PluginI::vil_filter && filterFlag&C_BDM_PluginI::hop_filter)
                                               {requete  +=  " AND (" + m_BDM_DRUGLIST_STATUT      + " = 'VH' OR " + m_BDM_DRUGLIST_STATUT + " = 'H') ";}
     else if (filterFlag&C_BDM_PluginI::vil_filter)
                                               {requete  +=  " AND (" + m_BDM_DRUGLIST_STATUT      + " = 'VH'         ) ";}
     else if (filterFlag&C_BDM_PluginI::hop_filter)
                                               {requete  +=  " AND (" + m_BDM_DRUGLIST_STATUT      + " = 'H'         ) ";}
     //requete  += dbi_database()->ownersSelectMention(m_BDM_DRUGLIST_TBL_NAME, C_BaseCommon::WhereAlreadyIn);  // rajouter le filtre du proprio
     if (filterFlag&C_BDM_PluginI::ucd_filter)  requete  +=  " ORDER BY " + m_BDM_DRUGLIST_UCD_PRICE;
    }
 else
    {QString where_and = " WHERE";
     //requete  += dbi_database()->ownersSelectMention(m_BDM_DRUGLIST_TBL_NAME, C_BaseCommon::WhereMustBeAdd);
     if (filterFlag&C_BDM_PluginI::gen_filter) {requete  +=  where_and + " (" + m_BDM_DRUGLIST_GENERIQUE   + " IN ('G','R')) ";                                  where_and = " AND";}
     if (filterFlag&C_BDM_PluginI::vil_filter && filterFlag&C_BDM_Api::hop_filter)
                                               {requete  +=  where_and + " (" + m_BDM_DRUGLIST_STATUT      + " = 'VH' OR " + m_BDM_DRUGLIST_STATUT + " = 'H') "; where_and = " AND";}
     else if (filterFlag&C_BDM_PluginI::vil_filter)
                                               {requete  +=  where_and + " (" + m_BDM_DRUGLIST_STATUT      + " = 'VH'        ) ";                                where_and = " AND";}
     else if (filterFlag&C_BDM_PluginI::hop_filter)
                                               {requete  +=  where_and + " (" + m_BDM_DRUGLIST_STATUT      + " = 'H'         ) ";                                where_and = " AND";}
     if (filterFlag&C_BDM_PluginI::ucd_filter)  requete  +=  " ORDER BY " + m_BDM_DRUGLIST_UCD_PRICE;
    }
  // CGestIni::Param_UpdateToDisk("/Users/rolandsevin/Documents/requete.txt",requete);
  return requete;
}
//-------------------------------- selectCustomProductsList ------------------------------------------------------
/*! \brief return a list of custom drugs list for a user
 *  \param QString userName the user name
 *  \param QString listName the name for this custom list.
 *  \param QString text drug to find.
 *  \param C_BDM_Api::flags filterFlag selection filter default value    = C_BDM_PluginI::all_filter.
 *  \return QList < C_BDM_DrugListRecord >  drugs list for this user
 */
const QList < C_BDM_DrugListRecord > C_BDM_Api::selectCustomProductsList(const QString &_userName,  const QString &listName,
                                                                         const QString &text,       C_BDM_PluginI::flags filterFlag /* =    = C_BDM_PluginI::all_filter */ )
{
    QList < C_BDM_DrugListRecord > productsList;
    if (!database().isOpen() && database().open()== false)
       { outMessage( QObject::tr("ERREUR : C_BDM_Api::selectCustomProductsList() database can not be opened"), __FILE__, __LINE__);  return productsList;
       }
    QString     requete = makeSelectCustomProductsListQuery( _userName, listName, text, filterFlag );
    QSqlQuery query (requete , database() );
    outSQL_error( query, "ERREUR  : C_BDM_Api::selectCustomProductsList()", requete, __FILE__, __LINE__);

    if ( query.isActive() )
       {while (query.next())
              {QString prix = "00000000" + query.value(10).toString(); prix = prix.right(6); prix.insert(4,'.');
               productsList.append( C_BDM_DrugListRecord(query.value(2).toString(),
                                                         query.value(3).toString(),
                                                         query.value(5).toString(),
                                                         query.value(6).toString(),
                                                         query.value(7).toString(),
                                                         query.value(8).toString(),
                                                         query.value(0).toString(),
                                                         query.value(1).toString(),
                                                         query.value(9).toString(),
                                                         prix,    // prix
                                                         query.value(4).toString(),
                                                         query.value(11).toString(),
                                                         query.value(12).toString()
                                                         )
                                  );
              }    // end while (query.next())
       }  // endif (query.isActive() )
    return productsList;
}

//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_Api::plugin_name()
{if (m_pC_BDM_PluginI ==0) return "";
 return m_pC_BDM_PluginI-> plugin_name();
}

//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_Api::plugin_icon()
{if (m_pC_BDM_PluginI ==0) return QPixmap();
 return m_pC_BDM_PluginI->plugin_icon();
}
//---------------------------------------- get_database -------------------------------------------------------
/*! \brief  return active database. This facility is to do extended SQL querys which are not in API
 *  \return  database
 */
QSqlDatabase C_BDM_Api::get_database()
{   if (m_pC_BDM_PluginI ==0) return QSqlDatabase ();
    return m_pC_BDM_PluginI->get_database();
}
//-------------------------------- plugin_image_source ------------------------------------------------------
/*! \brief  return image source url for monographies
 *  \return return image source url for monographies
 */
QString C_BDM_Api::plugin_image_source()
{   if (m_pC_BDM_PluginI ==0) return "";
    return m_pC_BDM_PluginI->plugin_image_source();
}
//-------------------------------- isValid ------------------------------------------------------
/*! \brief return validation state of this class. if no valid the class can't works
 */
int C_BDM_Api::isValid()
{return m_isValid;
}

//-------------------------------- patientContexte ------------------------------------------------------
/*! \brief return a poointer on C_PatientCtx (patient context) .
*/
C_PatientCtx *C_BDM_Api::patientContexte()
{   //if ( !m_pC_BDM_PluginI )                  return 0;
    //return m_pC_BDM_PluginI->patientContexte();
    return m_pC_PatientCtx;
}
//-------------------------------- setPatientContexte ------------------------------------------------------
/*! \brief set patient context.
*/
void C_BDM_Api::setPatientContexte(C_PatientCtx* patientCtx)
{   //if ( !m_pC_BDM_PluginI )                  return ;
    //m_pC_BDM_PluginI->setPatientContexte( patientCtx );
    m_pC_PatientCtx  = patientCtx;
}
//--------------------------------------- fillTreeWidgetIndicationsFromPatientContext -------------------------------------------------------
/*! \brief fill treewidget indication with patient pathologies as indications.
 * implicit input : m_pC_PatientCtx wich maintains patient pathologies list
 *  \param pQTreeWidget :  pointer to QTreeWidget in wich add informations
 *  \param icon :         icon associated to this item
 *  \return nothing
*/

void C_BDM_Api::fillTreeWidgetIndicationsFromPatientContext(QTreeWidget *pQTreeWidget, const QIcon &item_icone)
{ QTreeWidgetItem                    *item  = 0;
  if (m_pC_PatientCtx)
    {QList <C_LifeEvent> eventLifeList = m_pC_PatientCtx->lifeEventList();
     //QIcon item_icone  (m_imagePath+"medicatux/terrain_indic.png");
     for (int i = 0; i < eventLifeList.size(); ++i)
         {C_LifeEvent lifeEvent = eventLifeList.at(i);
          QString codeTyp       = lifeEvent.codeTyp();
          if ( codeTyp=="CIM" )
             { item   = new QTreeWidgetItem (pQTreeWidget);
               item->setIcon(INDIC_COL_ICONE,item_icone);
               item->setText(INDIC_COL_LIBELLE,lifeEvent.libelle());    // libelle
               item->setText(INDIC_COL_SMR,"");                         // niveau y en a pas
               item->setText(INDIC_COL_DATE,lifeEvent.dateDeb());       // date
               item->setText(INDIC_COL_CODE,lifeEvent.code());          // code
               item->setText(INDIC_COL_TYPE,codeTyp);                   // code type
               item->setText(INDIC_COL_COMMENT,lifeEvent.comment());    // commentaire
               item->setText(INDIC_COL_REFUNDED,"");                    // reboursement (y en a pas car pas lie a un CIP)
               item->setText(INDIC_COL_CIP,"");                         // pas lie a un CIP
             }
         }
    }
}
//-------------------------------- patientContexte ------------------------------------------------------
/*! \brief return a poointer on C_UserCtx (user contexte) .
*/
C_UserCtx *C_BDM_Api::userContexte()
{   //if ( !m_pC_BDM_PluginI )                  return 0;
    //return m_pC_BDM_PluginI->patientContexte();
    return m_pC_UserCtx;
}
//-------------------------------- setUserContexte ------------------------------------------------------
/*! \brief set user context
*/
void C_BDM_Api::setUserContexte(C_UserCtx* userCtx)
{   //if ( !m_pC_BDM_PluginI )                  return ;
    //m_pC_BDM_PluginI->setPatientContexte( patientCtx );
    m_pC_UserCtx  = userCtx;
}
//-------------------------------- getUserInfos ------------------------------------------------------
/*! \brief get user informations (if user exists)
 *  \param QString *name :  pointer to a QString in wich if not zero will be returned name
 *  \param QString *first_name :  pointer to a QString in wich if not zero will be returned first name
 *  \param QString *rpps :  pointer to a QString in wich if not zero will be returned rpps
 *  \return login in all is ok otherwise empty string
*/
QString C_BDM_Api::getUserInfos(QString *name, QString *first_name, QString *rpps)
{   QString u_name       = "";
    QString u_first_name = "";
    QString u_rpps       = "";
    QString u_login      = "";


    if (m_pC_UserCtx != 0)
       { u_name       = m_pC_UserCtx->get_usual_name();
         u_first_name = m_pC_UserCtx->get_forename();
         u_rpps       = m_pC_UserCtx->get_Nu_RPPS()+"|"+m_pC_UserCtx->get_Nu_ClefRPPS();
         u_login      = m_pC_UserCtx->get_Login();
       }


    if (name)       *name       = u_name;
    if (first_name) *first_name = u_first_name;
    if (rpps)       *rpps       = u_rpps;
    return u_login;
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
 QList<C_CompositionItem> C_BDM_Api::getCompositionFromCIP(const QString &cip)
 {   QList<C_CompositionItem>        compositionItemList;
     if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getCompositionFromCIP( cip );
     return compositionItemList;
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
 QList<C_ExcipientItem> C_BDM_Api::getExcipientsFromCIP(const QString &cip)
 {   QList<C_ExcipientItem>        compositionItemList;
     if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getExcipientsFromCIP( cip );
     return compositionItemList;
 }
 //-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
 /*! \brief test compatibility with maximum and minimun authorized dose from a drug list and patient context.
  *  \param C_PatientCtx *pC_PatientCtx
  *  \param const QStringList &grammarList grammar prescription list
  *  \return QList<C_InterResultPair> result interaction pair list
  */
 QList<C_InterResultPair>  C_BDM_Api::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QStringList &grammarList)
 {   QList<C_InterResultPair> interResultPairList;
     for (int i=0; i<grammarList.size(); ++i)
         { if (m_stopAllInterraction) {return interResultPairList;}
           interResultPairList.append( getInteractionDoseMinMax_fromCIP(pC_PatientCtx, grammarList[i]) );
         }
     return interResultPairList;
 }
 //-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
 /*! \brief test compatibility with maximum and minimun authorized dose from a drug and patient context.
  *  \param C_PatientCtx *pC_PatientCtx
  *  \param const QString &grammar prescription for this drug
  *  \return QList<C_InterResultPair> result interaction pair list
  */
 QList<C_InterResultPair>  C_BDM_Api::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)
 {   QList<C_InterResultPair>        interResultPairList;
     if (m_pC_BDM_PluginI==0) return interResultPairList; // interResultPairList;
     return m_pC_BDM_PluginI->getInteractionDoseMinMax_fromCIP( pC_PatientCtx, grammar);
 }

 //-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
 /*! \brief return drug composition.
  *  \param const QString& cip drug cip identification .
  *  \return QStringList result list with dci composants
  */
  QStringList C_BDM_Api::get_DCI_list_FromCIP(const QString &cip)
  { if (m_pC_BDM_PluginI ==0) return QStringList();
      return m_pC_BDM_PluginI->get_DCI_list_FromCIP(cip);
  }

 //-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
 /*! \brief return if  plugin is HAS certified
  *  \return return HAS certified status
  */
 bool C_BDM_Api::is_plugin_HAS_certified()
 {   if (m_pC_BDM_PluginI ==0) return false;
     return m_pC_BDM_PluginI->is_plugin_HAS_certified();
 }

 //-------------------------------- HAS_NotCertifiedAlertText ------------------------------------------------------
 /*! \brief return text to display when medicatux is HAS not certified
  *  \param const QString& pathImage if is not empty de feualt path image is updated .
  *  \return return HAS uncertified string
  */
 QString  C_BDM_Api::HAS_NotCertifiedAlertText(const QString &pathImage /* = "" */)
 { if (pathImage.length()) m_imagePath = pathImage;
   QString text = tr("<b>PROGRAMME D'AIDE A LA PRESCRIPTION NON SECURIS\303\211</b><hr/><p align=\"center\"><img src=\"%1\"  /></p><br />%2")
                 .arg( m_imagePath+"no_has_certified.png" ,
                       tr("Ce logiciel est <u>non agr\303\251\303\251 par l'HAS</u>.<br/>"
                          "Tous les contr\303\264les de s\303\251curit\303\251 sur les allergies, interactions, terrain etc. ne sont pr\303\251sents qu'\303\240 titre indicatif.<br/>"
                          "Ces contr\303\264les sont effectu\303\251s \303\240 partir des donn\303\251es suivantes : <b>%1</b> <br/>"
                          "Il est vous est donc recommand\303\251 d'\303\252tre particulièrement vigilant lors de son utilisation. Il n'en est pas moins agr\303\251able.").arg(dataSourceVersion())
                );
   return htmlEncapsulateInteraction(text);
 }


 //-------------------------------- allergie_LevelToPixmapPath ------------------------------------------------------
 /*! \brief return pixmap path for allergie level.
  *  \param int allergie level
  *  \return QString pixmap path.
  */
  QString C_BDM_Api::allergie_LevelToPixmapPath(int level) const
  {switch (level)
     {
      case 1:     return m_imagePath+"ci_allergies.png";
      default:    return m_imagePath+"ci_allergies.png";
     }
  }

  //------------------------------- noSecureText -----------------------------------------------------------------
  /*! \brief return html text for products not evaluated by interaction engine.
   *  \return html text
   */
  QString C_BDM_Api::noSecureProductText(const QString &productName)
  {QString text = tr("<b>PRODUIT NON SECURIS\303\211</b><hr/><b><u>%1</u> </b><br /><img src=\"%2\" height=\"42\" width=\"42\" /><br />%3")
                  .arg( productName,
                        m_imagePath + "no_secure.png"  ,
                        tr("Ce produit provient d'une origine non s\303\251curis\303\251e.<br />"
                           "Il ne fait l'objet d'aucun contr\303\264le de s\303\251curit\303\251 sur les allergies, interactions, terrain etc....<br />"
                           "Il est vous est donc recommand\303\251 d'\303\252tre particulièrement vigilant lors de sa prescription.")
                 );
   return htmlEncapsulateInteraction(text);
  }

 //------------------------------- interaction_Text -----------------------------------------------------------------
  /*! \brief return html text for interaction betwwen two composants ( C_InterResultPair  &interResultPair ) .
   *  \param const C_InterResultPair  &interResultPair  structure wich represents interaction betwwen two composants
   *  \return QString html out result
   */
 QString  C_BDM_Api::interaction_Text(const C_InterResultPair  &interResultPair)
 {   QString optional_icone = "";
     QString main_icone     = "";
     QString level_texte    = "";
     int     level          = interResultPair.Level().toInt();
     char    typ            = interResultPair.Typ().toLatin1()[0];
     switch (typ)
            { case 'I': { main_icone      = terrainLevel_ToPixmapPath(level);
                          char    typ_c   = interResultPair.Typ().mid(3,1).toLatin1()[0];                           // on se place apres le "IT_"
                          switch(typ_c)
                             {case 'G':    optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(6)+"\" />";        break;   // IT_GRO      alerte sur une incompatibilite avec une grossesse
                              case 'A':    optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(7)+"\" />";        break;   // IT_ALL      alerte sur une incompatibilite avec un allaitement
                              case 'V':    optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(6+level)+"\" />";  break;   // IT_VIG      alerte critique sur la vigilance suit la regle des contre-indications (patient declare comme vigilance intacte attendue)
                              case 'W':  { optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(6+level)+"\" />";    // juste le Warning (patient non declare comme vigilance intacte attendue)
                                           level          = 0; // juste se placer au niveau informatif
                                                               // level_texte    = tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#02BD02\">Vigilance : Information</td></tr></table>");
                                         }                                                                                      break;   // IT_W_VIG    alerte non critique sur la vigilance (patient non signale conducteur ou machiniste)
                              case 'D':    optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(12)+"\" />";       break;   // IT_DOPE     alerte sur un produit dopant
                              case 'Q':    optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(level)+"\" />";    break;   // IT_QUANT    alerte sur la quantite
                              case 'T':    optional_icone = QString("<img src=\"")+terrainLevel_ToPixmapPath(level)+"\" />";    break;   // IT_TEMPS    alerte sur la duree
                              case 'Y':    optional_icone = QString("<img src=\"")+m_imagePath+"agevie.png"+"\" />";            break;   // IT_YEARS    alerte sur l'age
                              case 'P':    optional_icone = QString("<img src=\"")+m_imagePath+"poids.png"+"\" />";             break;   // IT_POIDS    alerte sur le poids
                             }
                          level_texte     =  interactionTerrain_LevelToTexte(level);

                        } break;
              case 'A': { main_icone      =  afssaps_LevelToPixmapPath(level);                          // AFSSAPS
                          optional_icone  = "";
                          level_texte     =  afssaps_interactions_LevelToTexte(level);
                        } break;
              case 'P': { main_icone      = allergie_LevelToPixmapPath(level);                          // PATCTXALL
                          optional_icone  = "";
                          level_texte     = tr ("<table><tr><td width=100% align=\"left\" bgcolor=\"#FF0000\">CONTRE-INDICATION ABSOLUE ALLERGIE DECLAREE</td></tr></table>");
                        } break;
              case 'R': { main_icone      = m_imagePath+"redondances.png";
                          optional_icone  = "";
                          level_texte     = tr ("<table><tr><td width=100% align=\"left\" bgcolor=\"#00fcff\">REDONDANCES DE SUBSTANCE</td></tr></table>");
                        } break;
              case 'H': { main_icone      = m_imagePath+"physicochimique.png";
                          optional_icone  = "";
                          level_texte     = tr ("<table><tr><td width=100% align=\"left\" bgcolor=\"#00fcff\">INCOMPATIBILITE PHYSICOCHIMIQUE</td></tr></table>");
                        } break;
              case 'E': { main_icone      = m_imagePath+"prescription_restreinte.png";  //
                          optional_icone  = "";
                         level_texte     = tr ("<table><tr><td width=100% align=\"left\" bgcolor=\"#00fcff\">PRESCRIPTION RESTREINTE</td></tr></table>");
                        } break;
              case 'S': { main_icone      = m_imagePath+"prescription_restreinte.png";  //
                          optional_icone  = "";
                          level_texte     = tr ("<table><tr><td width=100% align=\"left\" bgcolor=\"#00fcff\">PRESCRIPTION DE STUPEFIANTS</td></tr></table>");
                        } break;
              case 'X': { main_icone      = m_imagePath+"prescription_restreinte.png";  //
                          optional_icone  = "";
                          level_texte     = tr ("<table><tr><td width=100% align=\"left\" bgcolor=\"#00fcff\">PRESCRIPTION D'EXCEPTION</td></tr></table>");
                        } break;
            }
     QString text = QString("<b>%6</b><hr/><u>Interaction entre</u> : <b>%1</b> avec <b>%2</b> <br /><img src=\"%3\" />%5<br />%4")
                     .arg( interResultPair.Term1(),
                           interResultPair.Term2(),
                           main_icone,
                           interResultPair.Info(),
                           optional_icone,
                           level_texte                 // mention annoncant l'information
                         );

     if (text.length()==0) return "";
     return htmlEncapsulateInteraction(text);
 }

 //------------------------ htmlEncapsulateInteraction -----------------------------------------
 /*! \brief encapsulate interaction text in html struct.
  *  \param const QString &text_in  text to encapsulate
  *  \return QString html out result
  */
 QString C_BDM_Api::htmlEncapsulateInteraction(const QString &text_in)
 {    QString html_out =  QString ("<table cellSpacing=\"0\"  cellpadding=\"4\" width=100% border=\"1\">"    // #FF8000
                                 "<tbody>"
                                 "<tr>"
                                 "<td width=100% align=\"left\" bgcolor=\"#FEFFDD\">")+
                                 text_in +
                                 "</td>"
                                 "</tbody>"
                                 "</table>";
      return html_out;
 }
 //-------------------------------- stopAllInteractions ------------------------------------------------------
 /*! \brief set  m_stopAllInterraction to 1 to stops evaluateAllInteractions() .
 */
 void C_BDM_Api::stopAllInteractions()
 {m_stopAllInterraction = 1;
 }

 //------------------------ evaluateAllInteractions -----------------------------------------
 /*! \brief evaluate all interaction from a products list (const QStringList &grammarList) and  patient context (implicite in C_BDM_Api).
  *  \param const QStringList &grammarList  products list from current patient ordonnance to be modified
  *  \return QString html result
  */
 QString C_BDM_Api::evaluateAllInteractions(const QStringList &grammarList)
 {   QString                       strAbort = tr("Abort all interraction");
     if (m_stopAllInterraction)      { return strAbort; }
     QStringList                       cipList;
     int                               i     = 0;
     QString                           html  = HAS_NotCertifiedAlertText();
     for ( i=0; i<grammarList.size(); ++i )
     {   int nextPos           = 0;
         QString    id         = CGestIni::getXmlData(     "gph_id",    grammarList.at(i), &nextPos);
         QString    id_type    = CGestIni::getXmlData(     "gph_it",    grammarList.at(i), &nextPos);
         QString    name       = CGestIni::getXmlData(     "gph_na",    grammarList.at(i), &nextPos);
         if (id_type == "CIP")   cipList  << id;
         else                    html     += noSecureProductText(name);
     }

     //................ on recueille ......................
     QList<C_InterResultPair> interResultPairList;
     interResultPairList                        += getInteractionPrescriptionRestreintePairFromCipList ( cipList );
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionDoseMinMax_fromCIP(  m_pC_PatientCtx, grammarList);
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionAllergiesFromCipList( cipList );    // Allergies traitees en dehors de la base de donnee medicamenteuse
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionPairFromCipList ( QStringList()<<cipList<<m_pC_PatientCtx->list_cip_traitement_fond() );
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionCim10FromCipList( cipList );
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionAgeSexePoidsSportVigil( cipList );   // TODO cache ici
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionRedundancy( cipList , m_pC_PatientCtx);       // si produit (sur le CIP specialite) en cours est deja prescrit dans traitement de fond on ne considere pas comme redondance
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getInteractionPhysicoChimiques( cipList , m_pC_PatientCtx);
     if (m_stopAllInterraction) {return strAbort;}
     interResultPairList                        += getAllergiesFromCipList( cipList , m_pC_PatientCtx);  // Allergies traitees avec la base de donnee medicamenteuse
     if (m_stopAllInterraction) {return strAbort;}


     //................ on classe par gravite et date .........
     C_InterResultPair              interResultPair;
     QMap<QDateTime, C_InterResultPair> map_level_4;
     QMap<QDateTime, C_InterResultPair> map_level_x;
     // QList<C_InterResultPair> interResultPairList_2;
     // QMap<QWidget *> list_level_3;
     // QMap<QWidget *> list_level_2;
     // QList<QWidget *> list_level_4;

     while ( !interResultPairList.isEmpty() )
     {   if (m_stopAllInterraction) {return strAbort;}
         interResultPair = interResultPairList.takeFirst();
         int     level   = interResultPair.Level().toInt();
         if (level>=4) map_level_4[interResultPair.createDateTime()]=interResultPair;   // alimenter la liste de niveaux 4 en la triant par date
         else          map_level_x[interResultPair.createDateTime()]=interResultPair;   // alimenter la liste des autres niveaux 4 en la triant par date
     }
     //................ on affiche les plus graves avant les autres .........
     QMapIterator<QDateTime, C_InterResultPair> it(map_level_4);
     it.toBack();
     while (it.hasPrevious())  // it.toBack(); et hasPrevious() car on inverse pour que la date la plus recente soit avant les autres
     {   if (m_stopAllInterraction) {return strAbort;}
         it.previous();
         interResultPair = it.value();
         html += interaction_Text(interResultPair);
     }
     QMapIterator<QDateTime, C_InterResultPair> lt(map_level_x);
     lt.toBack();
     while (lt.hasPrevious())
     {   if (m_stopAllInterraction) {return strAbort;}
         lt.previous();
         interResultPair = lt.value();
         html += interaction_Text(interResultPair);
     }

     /*
     //................ on affiche ............................
     for ( i = 0; i < interResultPairList.size(); ++i )
         {   if (m_stopAllInterraction) {return strAbort;}
             interResultPair = interResultPairList.at(i);
             html += interaction_Text(interResultPair);
         } // for (int i = 0; i < interResultPairList.size(); ++i)
      */
     return html;
 }
 //------------------------------- interactionTerrain_LevelToTexte [static] -----------------------------------------------------------------
 /*! \brief return QString level interaction betwen a products ant patient sane state.
  *  \param int level of interaction .
  *  \return QString
  */
 QString  C_BDM_Api::interactionTerrain_LevelToTexte(int level)
 {switch(level)
  {case 0:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#02BD02\">Terrain : Information</td></tr></table>");
   case 1:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#ffde00\">Mise en garde</td></tr></table>");
   case 2:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#ffb400\">Precautions d'emploi</td></tr></table>");
   case 3:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#ff7800\">UTILISATION DECONSEILLEE</td></tr></table>");
   case 4:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#FF0000\">CONTRE-INDICATION ABSOLUE</td></tr></table>");
   default: return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#0000FF\">CONTRE-INDICATION Niveau inconnu</td></tr></table>");
  }
 }
 //------------------------------- afssaps_interactions_LevelToTexte [static]-----------------------------------------------------------------
 /*! \brief return QString level interaction betwen two products.
  *  \param int level of interaction .
  *  \return QString
  */
 QString  C_BDM_Api::afssaps_interactions_LevelToTexte(int level)
 {switch(level)
  { case 0:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#02BD02\">Interaction : Information</td></tr></table>");
    case 1:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#ffde00\">Effets ind\303\251sirables</td></tr></table>");
    case 2:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#ffb400\">Pr\303\251cautions d'emploi</td></tr></table>");
    case 3:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#ff7800\">Association DECONSEILLEE</td></tr></table>");
    case 4:  return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#FF0000\">CONTRE-INDICATION Association CONTRE INDIQUEE</td></tr></table>");
    default: return tr("<table><tr><td width=100% align=\"left\" bgcolor=\"#0000FF\">CONTRE-INDICATION Niveau inconnu</td></tr>");
   }
 }
 //------------------------------- terrainLevel_ToPixmapPath -----------------------------------------------------------------
 /*! \brief return pixmap path for terrain level.
  *  \param int terrain level
  *  \return QString pixmap path.
 */
 QString C_BDM_Api::terrainLevel_ToPixmapPath(int level)
 {switch (level)
     {
      case 0:     return m_imagePath+"ci_terrain_inf.png";
      case 1:     return m_imagePath+"ci_terrain_mis.png";
      case 2:     return m_imagePath+"ci_terrain_pre.png";
      case 3:     return m_imagePath+"ci_terrain_dec.png";
      case 4:     return m_imagePath+"ci_terrain_abs.png";
      case 5:     return m_imagePath+"ci_terrain_pre.png";
      case 6:     return m_imagePath+"ci_terrain_gro.png";
      case 7:     return m_imagePath+"ci_terrain_all.png";
      case 8:     return m_imagePath+"ci_vigilance_010.png";
      case 9:     return m_imagePath+"ci_vigilance_020.png";
      case 10:    return m_imagePath+"ci_vigilance_030.png";
      case 11:    return m_imagePath+"ci_terrain_inf.png";
      case 12:    return m_imagePath+"ci_sport_01.png";
      default:    return "";
     }
 }
 //-------------------------------- afssaps_LevelToPixmapPath ------------------------------------------------------
 /*! \brief return pixmap path for afssaps level.
  *  \param int afssaps level
  *  \return QString pixmap path.
  */
QString C_BDM_Api::afssaps_LevelToPixmapPath(int level) const
 {
   switch (level)
    {
     case 4:     return m_imagePath+"ci_assinterdit.png";;
     case 3:     return m_imagePath+"ci_deconseille.png";
     case 2:     return m_imagePath+"ci_precautions.png";
     default:    return m_imagePath+"ci_eff_indesir.png";
    }
 }
  //-------------------------------- getInteractionRedundancy ------------------------------------------------------
  /*! \brief return redundancy composition in a cip list of drugs.
   *  \param cipList QStringList &cipList drugs cip list .
   *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
   *  \return QList<C_InterResultPair> result interaction pair list
   */
QList<C_InterResultPair> C_BDM_Api::getInteractionRedundancy( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
  {   QList<C_InterResultPair>        interResultPairList;
      if (m_stopAllInterraction) {return interResultPairList;}
      if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getInteractionRedundancy( cipList, pC_PatientCtx );
      return interResultPairList;
  }
//-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QStringList &cipList drugs cip list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionAgeSexePoidsSportVigil( const QStringList &cipList)
{   QList<C_InterResultPair>        interResultPairList;
    //QString cip;
    if (cipList.size()==0)                        return  interResultPairList;
    if (m_pC_PatientCtx==0)                       return  interResultPairList;
    for (int i=0; i < cipList.size(); ++i)
        {  //cip = cipList[i];
           interResultPairList += getInteractionAgeSexePoidsSportVigil(cipList[i],  CIP_to_DrugName(cipList[i])  );
        }
    return interResultPairList;
}
//-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>    C_BDM_Api::getInteractionAgeSexePoidsSportVigil(const QString &cip, QString drugName )
{ QList<C_InterResultPair>        interResultPairList;
  if (m_pC_PatientCtx==0)    {return interResultPairList;}
  if (m_stopAllInterraction) {return interResultPairList;}
  return getInteractionAgeSexePoidsSportVigil(cip,
                                              m_pC_PatientCtx->get_age(1),
                                              m_pC_PatientCtx->get_sexe(),
                                              m_pC_PatientCtx->poids(),
                                              m_pC_PatientCtx->taille(),
                                              QString::number(m_pC_PatientCtx->grossesse()/7),
                                              QString::number(m_pC_PatientCtx->insuffisance_renale()),
                                              m_pC_PatientCtx->allaitement(),
                                              m_pC_PatientCtx->sport(),
                                              m_pC_PatientCtx->vigilance() ,
                                              drugName  );
}

//-------------------------------- getInteractionAgeSexePoidsSportVigil ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QString &age patient age .
 *  \param const QString &sexe patient sexe F-M-I.
 *  \param const QString &poids patient poids .
 *  \param const QString &taille patient size .
 *  \param const QString &amenorrhee in weks
 *  \param const QString &clairance in ml/min
 *  \param int   allaitement in number of days
 *  \param bool sport if true particular attention for doping drugs is required  .
 *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>     C_BDM_Api::getInteractionAgeSexePoidsSportVigil ( const QString &cip,
                                                                               const QString &age ,
                                                                               const QString &sexe,
                                                                               const QString &poids,
                                                                               const QString &taille,
                                                                               const QString &amenorrhee,
                                                                               const QString &clairance,
                                                                               int            allaitement,
                                                                               bool    sport     /* = true */,
                                                                               bool    vigil     /* = true */,
                                                                               QString drugName  /* = "" */)
{
  QList<C_InterResultPair>        interResultPairList;
  if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getInteractionAgeSexePoidsSportVigil(cip, age, sexe, poids, taille, amenorrhee, clairance, allaitement, sport, vigil , drugName  );
  return interResultPairList;
}
//-------------------------------- getInteractionCim10FromCipList ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QStringList &cipList drugs cip list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionCim10FromCipList( const QStringList &cipList)
{   QList<C_InterResultPair>        interResultPairList;
    if (m_stopAllInterraction) {return interResultPairList;}
    QString cip;
    if (cipList.size()==0)                        return  interResultPairList;
    if (m_pC_PatientCtx==0)                       return  interResultPairList;
    if (m_pC_PatientCtx->cim10List().size()==0)   return  interResultPairList;
    //................ virer allaitement et grossesse de la liste des CIM 10 ................................
    //                (evaluee de bien meilleure facon ailleurs dans getInteractionAgeSexePoidsSportVigil)
    //                pour eviter la redondance de detection
    QStringList c10List;   // on copie la liste des cim10 pour virer tout ce qui touche a la grossesse et allaitement (evaluee de bien meilleure facon ailleurs)
    for (int i=0; i<m_pC_PatientCtx->cim10List().size(); ++i)
        { QString cim = m_pC_PatientCtx->cim10List()[i];
          if (cim.startsWith("Z32"))   continue;
          if (cim.startsWith("Z39.1")) continue;
          c10List << cim;
        }
    //................ on confronte chaque CIP a la  liste des CIM10 ................................
    for (int i=0; i < cipList.size(); ++i)
        {  if (m_stopAllInterraction) {return interResultPairList;}
           cip = cipList[i];
           interResultPairList += getInteractionCim10(cip, c10List , CIP_to_DrugName(cip)  );
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
QList<C_InterResultPair> C_BDM_Api::getInteractionCim10(const QString &cip, QString drugName /* = "" */ )
{QList<C_InterResultPair>        interResultPairList;
 if (m_pC_PatientCtx==0)                       return  interResultPairList;
 if (m_pC_PatientCtx->cim10List().size()==0)   return  interResultPairList;
 return getInteractionCim10(cip, m_pC_PatientCtx->cim10List() , drugName  );
}

//-------------------------------- getInteractionCim10 ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QStringList &cim10List cim10 pathologies list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  )
{   QList<C_InterResultPair> interResultPairList;
    // on cherche dans le cache de resultats car op\303\251ration couteuse en SQL
    for (int i = 0; i < m_interResultCacheList.size(); ++i)
        { C_InterResultPair interResultPair = m_interResultCacheList.at(i);
          if (interResultPair.Code2()==cip )
             {
               outMessage(tr(".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;getInteractionCim10() get in cache : <b>%1 cip : %2 </b> %3").arg(drugName,cip,interResultPair.Term1()));
               if (interResultPair.Typ().length()) // ne pas retenir le resultat bidon (type vide)
                   interResultPairList.append(interResultPair);
               else
                   return interResultPairList;
             }
         }
    if (interResultPairList.size())  return interResultPairList;    // si trouve dans le cache on retourne le resultat
    // on cherche dans le SQL et on le met en cache car long
    // PB logique : si pas de resultat il convient quand meme de cacher cette absence de resultat
    //              pour ce cip, sinon la requete SQL couteuse sera reevaluee pour ce cip
    if (m_pC_BDM_PluginI)
       {
         interResultPairList     = m_pC_BDM_PluginI->getInteractionCim10 ( cip, cim10List,  drugName); // SQL couteux
         //......... si pas de resultat en creer un bidon (type vide) ...............................
         //          avec ce cip pour que le cache fonctionne
         if (interResultPairList.size()==0)
            { m_interResultCacheList.append( C_InterResultPair("" , ""      ,"dumy",
                                                               cip, "CIP"   ,"dumy",
                                                               "" , ""      ,"0"
                                                              )
                                           );
              // outMessage(tr(".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;getInteractionCim10() put in cache : <b>%1 cip : %2 </b> NULL").arg(drugName,cip));
            }
         else
            { m_interResultCacheList += interResultPairList;
             // outMessage(tr(".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;getInteractionCim10() put in cache : <b>%1 cip : %2 </b> %3").arg(drugName, cip, interResultPair.Term1()));
            } //if (interResultPairList.size()==0)
       }
    return interResultPairList; //
}

//-------------------------------- getInteractionAllergies ------------------------------------------------------
/*! \brief return allergicals interactions from patient and drugs.
 *  \param const QString& QStringList drug cip drug list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionAllergiesFromCipList( const QStringList &cipList )
{ QList<C_InterResultPair> interResultPairList;
  for (int i=0; i < cipList.size(); ++i)
      {if (m_stopAllInterraction) {return interResultPairList;}
       interResultPairList += getInteractionAllergies( cipList[i] );
      }
  return interResultPairList;
}
//-------------------------------- getAllergiesFromCipList ------------------------------------------------------
/*! \brief return Allergies interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_Api::getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{   if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getAllergiesFromCipList(cipList, pC_PatientCtx);
    QList <C_InterResultPair> retDumy;
    return retDumy;
}
//-------------------------------- getInteractionAllergies ------------------------------------------------------
/*! \brief return allergicals interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
//        |DCI|ASPIRINE|ALD|Allergie(Allergie)|plut\303\264t intol\303\251rance|06-06-1956|
// 3515495|CIP|LAMALINE|Sport|Allergie(Allergie)|urticaire g\303\251n\303\251ralis\303\251 |06-06-1956|
// 3007528|CIP|ASPIRINE|ALD|Allergie(Allergie)|plut\303\264t intol\303\251rance|06-06-1956|
// J01CR02|ATC|AMOXICILINE ET INHIBITEUR D'ENZYME|ALD|Allergie(Allergie)|pas de commentaire|06-06-1956|
QList<C_InterResultPair> C_BDM_Api::getInteractionAllergies(const QString &cip, QString drugName /* = "" */ )
{   QList<C_InterResultPair>                interResultPairList;
    if (m_pC_PatientCtx==0)         return  interResultPairList;
    if (m_pC_BDM_PluginI==0)        return  interResultPairList;
    if (m_stopAllInterraction)     {return  interResultPairList;}
     // on cherche dans le cache de resultats car op\303\251ration couteuse en SQL
     for (int i = 0; i < m_AllergieCacheList.size(); ++i)
         { if (m_stopAllInterraction) {return interResultPairList;}
           C_InterResultPair interResultPair = m_AllergieCacheList.at(i);
           if (interResultPair.Code2()==cip )
              {
               outMessage(tr(".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;getInteractionAllergies() get in cache : <b>%1 cip : %2 </b> %3").arg(drugName,cip,interResultPair.Term1()));
               if (interResultPair.Typ().length())    // ne pas retenir le resultat bidon (type vide)
                   interResultPairList.append(interResultPair);
                else
                   return interResultPairList;
              }
          }
     if (interResultPairList.size())  return interResultPairList;    // si trouve dans le cache on retourne le resultat
     // on cherche dans le SQL et on le met en cache car long
     // PB logique : si pas de resultat il convient quand meme de cacher cette absence de resultat
     //              pour ce cip, sinon la requete SQL couteuse sera reevaluee pour ce cip
     if (m_pC_BDM_PluginI)
        { interResultPairList     = m_pC_BDM_PluginI->getInteractionAllergies(cip, m_pC_PatientCtx->lifeEventList(),  drugName );  // operation couteuse
         //......... si pas de resultat en creer un bidon (type vide) ...............................
         //          avec ce cip pour que le cache fonctionne
          if (interResultPairList.size()==0)
             { m_AllergieCacheList.append( C_InterResultPair("" , ""      ,"dumy",
                                                             cip, "CIP"   ,"dumy",
                                                             "" , ""      ,"0"
                                                            )
                                         );
               // outMessage(tr(".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;getInteractionAllergies() put in cache : <b>%1 cip : %2 </b> NULL").arg(drugName,cip));
             }
          else
             { m_AllergieCacheList += interResultPairList;
               // outMessage(tr(".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;getInteractionAllergies() put in cache : <b>%1 cip : %2 </b> %3").arg(drugName, cip, interResultPair.Term1()));
             } //if (interResultPairList.size()==0)
        }
     return interResultPairList;
}

//-------------------------------- ATC_from_CIP ------------------------------------------------------
/*! \brief return ATC code from CIP code.
 *  \param const QString& cip drug cip identification .
 *  \return QString ATC code from this drug
 */
QString C_BDM_Api::ATC_from_CIP(const QString& cip)
{   if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->dbi_cip_atcName(cip);
    return "";
}

//-------------------------------- CIP_to_DrugName ------------------------------------------------------
/*! \brief return drug name code from CIP code.
 *  \param const QString& cip drug cip identification .
 *  \return QString drug name
 */
QString C_BDM_Api::CIP_to_DrugName(const QString& cip)
{   if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->dbi_cip_drugName(cip);
    return "";
}

//-------------------------------- drugsList_Count ------------------------------------------------------
/*! \brief return size of drugs list. proceed to SQL op\303\251ration and set cache list number.
 */
int C_BDM_Api::drugsList_Count()
{if (m_pC_BDM_PluginI==0)        return  0;
 return m_pC_BDM_PluginI->dbi_DrugListCount();
}
//-------------------------------- drugsList_Size ------------------------------------------------------
/*! \brief return size of drugs list. This return only cache value and proceed to no SQL op\303\251ration
 */
int  C_BDM_Api::drugsList_Size()
{   if (m_DrugListNb==-1) m_DrugListNb = drugsList_Count();   // si pas initialisee ob fait unefois l'operation couteuse
    return m_DrugListNb;
}
//------------------------ selectProductsList -----------------------------------------
/*! \brief select drugs list from user input, for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param const QString textSelect user text fr select drugs.
 *  \param C_BDM_Api::flags filterFlag selection filter.
 *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
 */
const QList < C_BDM_DrugListRecord > C_BDM_Api::selectProductsList (const QString &text, C_BDM_PluginI::flags filterFlag  /* = C_BDM_Api::all_filter */ , int limit /* = 200 */)
{ QList < C_BDM_DrugListRecord >          resultList;
  if (m_pC_BDM_PluginI==0)        return  resultList;
  return m_pC_BDM_PluginI->dbi_selectProductsList(text, filterFlag, limit);
}
//------------------------ selectFavorisList -----------------------------------------
/*! \brief select drugs in hitlist from user input, for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param const QString textSelect user text fr select drugs.
 *  \param C_BDM_Api::flags filterFlag selection filter.
 *  \return QList < C_BDM_DrugListRecord >  list of C_BDM_DrugListRecord  results
 */
const QList < C_BDM_DrugListRecord > C_BDM_Api::selectFavorisList (const QString &text, C_BDM_PluginI::flags filterFlag  /* = C_BDM_Api::all_filter */ , int limit /* = 200 */)
{ QList < C_BDM_DrugListRecord >          resultList;
  if (m_pC_BDM_PluginI==0)        return  resultList;
  return m_pC_BDM_PluginI->dbi_selectFavorisList(text, filterFlag, limit);
}
//-------------------------------- selectProductsList ------------------------------------------------------
/*! \brief select drugs list from user input, for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 * m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param QTreeWidget *pQTreeWidget: QTreeWidget to fill with datas.
 *  \param const QString textSelect user text fr select drugs.
 *  \param C_BDM_Api::flags filterFlag selection filter.
 *  \return number products
 */
int  C_BDM_Api::selectProductsList(QTreeWidget     *pQTreeWidget,                                    // QTreeWidget graphique \303\240 renseigner avec les donn\303\251es
                                   const    QString text               /* = ""  */,                       // saisie partielle d'un texte pour recherche
                                   C_BDM_PluginI::flags filterFlag,    /* = C_BDM_Api::flags */      // filtre de saisie
                                   int limit                           /* = 200 */
                                   )
{   pQTreeWidget->clear();
    if (m_isValid==0) return 0;
    QList < C_BDM_DrugListRecord >  resultList;
    C_BDM_DrugListRecord            drugRecord;
    QTreeWidgetItem                *item  = 0;
    if (m_pC_BDM_PluginI==0)        return  0;
    resultList             = m_pC_BDM_PluginI->dbi_selectProductsList(text,  (C_BDM_PluginI::flags) (filterFlag & C_BDM_PluginI::all_filter)  , limit);
    if (resultList.size()==0)       return  0;
    for (int i = 0; i < resultList.size(); ++i)
        { drugRecord = resultList.at(i);
          item =   new QTreeWidgetItem (pQTreeWidget);
          item->setText(0,drugRecord.commercialName());   // libelle
          item->setText(1,drugRecord.dci_1Name());        // DCI_1
          item->setText(2,drugRecord.dci_2Name());        // DCI_2
          item->setText(3,drugRecord.dci_3Name());        // DCI_3
          item->setText(4,drugRecord.ucd_price());        // Prix
          item->setText(5,drugRecord.atc());              // ATC
          item->setText(6,drugRecord.id());               // ID CIP
          item->setText(8,drugRecord.owner());            // PROVENANCE
          item->setText(9,drugRecord.id_type());          // ID TYPE
          item->setText(11,drugRecord.generique());       // generique   G R ou S
          item->setText(12,drugRecord.statut());          // statut  Ville Hopital VH ou H
          item->setText(13,drugRecord.nb_up());           // nbr d'UP
        }
    return pQTreeWidget->topLevelItemCount();
}
//------------------------ updateDrugLists -----------------------------------------
/*! \brief for a good performance response a rapid special drugs list is implemented
 *  this list is to update each news datas base source versions datas. We proceed update if
 *  m_DatasSourceVersion differs from pC_BDM_PluginI->dataSourceVersion();
 *  \param C_BDM_InitPluginI *pC_BDM_InitPluginI: data interface which provides data to  C_BDM_Api.
 *  \return true in all is ok otherwise false
 */
bool C_BDM_Api::updateDrugLists()
{   if (m_pC_BDM_PluginI==0)  return false;
    else                      return  m_pC_BDM_PluginI->dbi_updateDrugLists();
}
//-------------------------------- selectPosologieHitList ------------------------------------------------------
/*! \brief select drugs posologies hits list from drug code
 *  \param const QString &drug_id : as variable name shows, it's drug identification key.
 *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
 *  \param int limit : limit number of returns.
 *  \return QMap <int, QString> where int is order number and QString is grammar string started by 'cip/pk!' mention
 */
QStringList  C_BDM_Api::selectPosologieHitList(const QString &drug_id, const QString &drug_id_type /* = "CIP" */, int limit /* = 15  */)
{   if (m_pC_BDM_PluginI==0)  return  QStringList();
    else                      return  m_pC_BDM_PluginI->dbi_selectPosologieHitList(drug_id, drug_id_type, limit );
}
//-------------------------------- deleteItemPosologieHit ------------------------------------------------------
/*! \brief delete a item from drug hit parade.
 *  \param const QString &pk :  item pk to delete
 *  \return true in all is ok otherwise false
 */
int C_BDM_Api::deleteItemPosologieHit(const QString &pk )
{   if (m_pC_BDM_PluginI==0)  return  false;
    else                      return  m_pC_BDM_PluginI->dbi_deleteItemPosologieHit( pk );
}

//-------------------------------- increasePosologieHit ------------------------------------------------------
/*! \brief increase a posologie drug hit parade adjusted in real time of use. Create record only if not exists.
 *  \param const QString &grammar :     grammar input string as
 *  \param const QString &drug_id : as variable name shows, it's drug identification key.
 *  \param const QString &drug_id_type : as variable name shows, it's drug identification key type.
 *  \param int mode  default = INCREASE_HIT/increase hit  else FIX_HIT add the posologie whith fix hit.
 *  \return pk of created or updated record if all is ok otherwise empty string
 */
QString C_BDM_Api::increasePosologieHit(const QString &grammar, const QString &drug_id, const QString &drug_id_type /* = "CIP" */, int mode  /* = INCREASE_HIT */)
{   if (m_pC_BDM_PluginI==0)  return  QString::null;
    else                      return  m_pC_BDM_PluginI->dbi_increasePosologieHit(grammar, drug_id, drug_id_type /* = "CIP" */,  mode  /* = INCREASE_HIT */);
}
//-------------------------------- reinitHitsList ------------------------------------------------------
/*! \brief delete the actual hit list and remake a new.
 */
void C_BDM_Api::reinitHitsList()
{   if (m_pC_BDM_PluginI==0)  return;
    if (QMessageBox::warning ( 0, tr("R\303\251initialisation des la listes des posologies favorites"),
                               tr("ATTENTION : la r\303\251initialisation de la liste des posologies favorites, efface la liste actuelle"),
                               QMessageBox::Cancel|QMessageBox::Yes,
                               QMessageBox::Cancel
                             ) == QMessageBox::Yes
       )
       { m_pC_BDM_PluginI->dbi_createPosologieHit(C_BDM_PluginI::EVEN_IF_EXISTS );
       }
}
//-------------------------------- dataSourceVersion ------------------------------------------------------
/*! \brief return datasource version
 */
QString C_BDM_Api::dataSourceVersion()
{   if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->dataSourceVersion();
    return tr("no drugs data source selected");
}

 //------------------------------- level_SMR_ToLibelle [static]-----------------------------------------------------------------
 /*! \brief return QString level interaction betwen two products.
  *  \param int level of interaction .
  *  \return QString
  */
 /*
  SMR majeur        remboursement 100%
  SMR important     remboursement 65%
  SMR Mod\303\251r\303\251        remboursement 35%
  SMR Faible        remboursement 15%
  SMR insuffisant   remboursement 0% remboursement avis d\303\251favorable \303\240 l'inscription sur la liste des m\303\251dicaments remboursables
  SMR non pr\303\251cis\303\251   remboursement 0%

  ASMR 1  Am\303\251lioration majeure
  ASMR 2  Am\303\251lioration importante
  ASMR 3  Am\303\251lioration modeste
  ASMR 4  Am\303\251lioration mineure
  ASMR 5  Aucune am\303\251lioration
  ASMR 6  Avis d\303\251favorable inscription Collectivit\303\251 ou S\303\251curit\303\251 Sociale
  ASMR 00 Am\303\251lioration difficile \303\240 pr\303\251ciser

  case 1  : return "2";    // faible
  case 2  : return "4";    // important
  case 3  : return "1";    // insuffisant
  case 4  : return "3";    // modere
*/

QString C_BDM_Api::level_SMR_ToLibelle(int level)
{switch(level)
   { case 1  : return tr("SMR insuffisant");          // insuffisant remboursement 0%
     case 2  : return tr("SMR faible");               // faible      remboursement 15%
     case 3  : return tr("SMR mod\303\251r\303\251");               // modere      remboursement 35%
     case 4  : return tr("SMR important");            // important   remboursement 65%
     case 5  : return tr("SMR majeur");               // majeur      remboursement 100%
     default : return tr("SMR inconnu");
   }
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
 *                                  C_BDM_PluginI::REFUND      we check if indications are linked to refund ( m_refundLink will contains price)
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_Api::getIndicationsFromText( const QString &text,   int filter )
{if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getIndicationsFromText(text, filter);
 QList <C_BDM_IndicationRecord> retDumy;
 return retDumy;
}
//------------------------------ getRefundedIndications ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
 *  \param const QString &code_type  code typ as giving by the drugs BDM
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_Api::getRefundedIndications( const QString &code_indication,   const QString &code_type )
{ if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getRefundedIndications(code_indication, code_type);
  QList <C_BDM_IndicationRecord> retDumy;
  return retDumy;
}
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
QList <C_BDM_IndicationRecord> C_BDM_Api::getIndicationsFromCIP( const QString &cip,   int filter )
{if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getIndicationsFromCIP(cip, filter);
 QList <C_BDM_IndicationRecord> retDumy;
 return retDumy;
}

//------------------------------ getCIPFromIndication ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code       indication code of the drugs from wich we want datas.
 *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
 *  \return QStringList cip result list
 */
 QStringList C_BDM_Api::getCIPFromIndication( const QString &code, const QString &codeType )
 {   if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getCIPFromIndication( code, codeType );
     return QStringList();
 }
 //-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
 /*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
  *         if there is not restrictive prescription for this CIP  isValid() is false;
  *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
  *                        as 'cip_0','cip_1' .... 'cip_n' (distinction is on argument starting with ' )
  *  \return QList<C_PrescriptionRestreinte>
  */
 QList<C_PrescriptionRestreinte> C_BDM_Api::getPrescriptionRestreinteFromCIP(const QString &cip)
 {    QList<C_PrescriptionRestreinte> retList;
      if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getPrescriptionRestreinteFromCIP(cip);
      QList <C_BDM_IndicationRecord> retDumy;
      return retList;
 }
//-------------------------------- getMonographieFromCIP ------------------------------------------------------
/*! \brief select monographie for a drug.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString contains monographie
 */
QString  C_BDM_Api::getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ )
{if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getMonographieFromCIP ( cip  , dataMustBeReadInCache);
 return QString::null;
}

//-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
/*! \brief return the drug route from cip.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with the drug route
 */
QString   C_BDM_Api::getDrugRouteFromCIP ( const QString &cip)
{if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getDrugRouteFromCIP ( cip );
 return QString::null;
}

//-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
/*! \brief return the prise unit form from cip. (comprim\303\251s, suppositoires sachets,ampoules,doses kg, cuillères \303\240 caf\303\251 etc...)
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with unit form
 */
QString   C_BDM_Api::getPriseUnitFormFromCIP ( const QString &cip, const QString &commercialName /* = 0 */)
{if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getPriseUnitFormFromCIP ( cip , commercialName);
 return QString::null;
}
//-------------------------------- getFormeFromCIP ------------------------------------------------------
/*! \brief return drug presentation form from cip. (comprim\303\251s, suppositoires sachets,ampoules, suspension buvable etc...)
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with presentation form
 */
QString   C_BDM_Api::getFormeFromCIP ( const QString &cip)
{if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getFormeFromCIP ( cip );
 return QString::null;
}
//-------------------------------- getPosologiesFromCIP ------------------------------------------------------
/*! \brief return posologies list as posologie grammar QString
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
 *  \return QStringList with grammar QString list
 */
QStringList   C_BDM_Api::getPosologiesFromCIP ( C_PatientCtx *pC_PatientCtx, const QString &cip )
{ if (m_pC_BDM_PluginI) return m_pC_BDM_PluginI->getPosologiesFromCIP (pC_PatientCtx, cip );
  return QStringList();
}
//-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
/*! \brief return interactions xml results from a cip list.
 *  \param const QStringList &cipList      drugs CIP codes list to test.
 *  \return QList<C_InterResultPair> with result interactions
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionPairFromCipList ( const QStringList &cipList)
{   QList<C_InterResultPair> interResultPairList;
    if (m_stopAllInterraction) {return interResultPairList;}
    // for (int i=0; i<cipList.size(); ++i) //  qDebug() << "CIP :" << cipList[i];
        // outMessage("CIP :" + cipList[i]);   // ATTENTION :  gros plantage du thread sous mac
        // qDebug() << "CIP :" << cipList[i];

    if (m_pC_BDM_PluginI) interResultPairList = m_pC_BDM_PluginI->getInteractionPairFromCipList ( cipList );
    //............ traiter les redondance A<-->B B<-->A ............................
    //             en creant une QMap triee sur un pseudo code eliminant les redondances
    if (interResultPairList.count()==0) return interResultPairList;
    if (m_stopAllInterraction) {return interResultPairList;}

    C_InterResultPair                  interResultPair;
    QMap <QString, C_InterResultPair>  mapResults;
    QString cip_i1   = "";
    QString cip_i2   = "";
    QString keycode  = "";
    for (int i = 0; i < interResultPairList.size(); ++i)
        { interResultPair     = interResultPairList.at(i);
          cip_i1              = interResultPair.Code1();
          cip_i2              = interResultPair.Code2();
          keycode             = (cip_i1<cip_i2) ? cip_i1+" "+cip_i2 : cip_i2+" "+cip_i1;    // astuce pour eviter les redondances A<-->B et B<-->A
          keycode            += " "+interResultPair.Level();                                // on cree un pseudo code pour eviter les redondances
          mapResults[keycode] = interResultPair;
        }
    //............ on reprend le map qui devrait etre sans redondance ................
    QMapIterator<QString, C_InterResultPair> it ( mapResults );
    interResultPairList.clear();
    while (it.hasNext())
       {it.next();
        interResultPairList.append( it.value() );
       }
    return interResultPairList;
}
//-------------------------------- getInteractionPhysicoChimiques ------------------------------------------------------
/*! \brief return chimico physiques interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{ QList<C_InterResultPair>        interResultPairList;
  if ( m_pC_BDM_PluginI == 0 ) {return interResultPairList;}
  if (m_stopAllInterraction)   {return interResultPairList;}
  if ( cipList.size()==0 )     {return interResultPairList;}
  return m_pC_BDM_PluginI->getInteractionPhysicoChimiques( cipList, pC_PatientCtx );
  /*..... le cache doit cacher les resultats vides pour un CIP donne ....................
  QStringList cip_list = cipList;
  //......... ne requeter que sur la liste des cip non trouves dans le cache ............
  for ( int i=0; i<m_interPhysicoChimiquesCacheList.size(); ++i )
      { QString cip = m_interPhysicoChimiquesCacheList[i].Code1();
        if ( cip_list.contains( cip ) ) cip_list.removeAll( cip ); // on enleve le cip de la liste a requeter
      }
  interResultPairList.append(m_interPhysicoChimiquesCacheList);

  if (cip_list.size()==0) return interResultPairList;
  //.............. on recupere les valeurs restantes et on met en cache .............
  if (m_stopAllInterraction)   {return interResultPairList;}
  QList<C_InterResultPair> interResultPairs = m_pC_BDM_PluginI->getInteractionPhysicoChimiques( cip_list, pC_PatientCtx );
  m_interPhysicoChimiquesCacheList.append( interResultPairs );  // on  ajoute au cache
  interResultPairList.append(interResultPairs);                 // on ajoute au resultat
  return interResultPairList;
  */
}
//-------------------------------- getInteractionPrescriptionRestreintePairFromCipList ------------------------------------------------------
/*! \brief return all restrictives prescriptions allert from cip list
 *  \param const QStringList &cipList      drugs CIP codes list to test.
 *  \return QList<C_InterResultPair> with result interactions
 */
QList<C_InterResultPair> C_BDM_Api::getInteractionPrescriptionRestreintePairFromCipList ( const QStringList &cipList)
{   QList<C_InterResultPair> interResultPairList;
    if ( m_pC_BDM_PluginI == 0 ) {return interResultPairList;}
    if ( m_stopAllInterraction ) {return interResultPairList;}
    if ( cipList.size()==0 )     {return interResultPairList;}
    C_InterResultPair  result;
    QStringList        cip_list;
    //......... ne requeter que sur la liste des cip non trouves dans le cache ............
    for ( int i=0; i<cipList.size(); ++i )
        { QString cip = cipList[i];
          if ( m_stopAllInterraction ) {return interResultPairList;}
          if ( m_interResultPrescrRestrCacheList.contains(cip) )
             { result = m_interResultPrescrRestrCacheList[cip];
               if (result.Code2() != "DUMY") interResultPairList.append( result );
             }
          else
             { cip_list << cip;     // non trouve dans le cache --> on ajoute ce cip a la liste a traiter
             }
        }
    //......... requeter sur la liste des cip non trouves dans le cache ............
    if ( cip_list .size() )
       { QString cip_str = "('" + cip_list.join("','") + "')";
         QList<C_PrescriptionRestreinte> prescr_restr_list = m_pC_BDM_PluginI->getPrescriptionRestreinteFromCIP(cip_str);
         for ( int i=0; i<prescr_restr_list.size(); ++i )
             { if ( m_stopAllInterraction ) {return interResultPairList;}
               C_PrescriptionRestreinte prescr = prescr_restr_list[i];
               QString message = prescr.toString();

               if ( prescr.isStupefiant()=="1" )
                  {
                    result = C_InterResultPair( prescr.cip(), "CIP", prescr.drug_name(),   // auquel cas ajouter a la liste des interactions
                                                prescr.cip(), "CIP", tr("Prescription de stup\303\251fiants"),
                                                QObject::tr( "<u><b>M\303\211DICAMENT STUP\303\211FIANT</b></u> <br />")
                                                + message,
                                                "STUP",
                                               "1");
                  }
               else if ( prescr.exception()=="1" )
                  {
                    result = C_InterResultPair( prescr.cip(), "CIP", prescr.drug_name(),   // auquel cas ajouter a la liste des interactions
                                                prescr.cip(), "CIP", tr("Prescription d'exception"),
                                                QObject::tr( "<u><b>M\303\211DICAMENT \303\200 PRESCRIPTION EXCEPTIONNELLE</b></u> <br />")
                                                + message +
                                                QObject::tr("<br/><b>Remboursable seulement</b> s'il est prescrit sur une ordonnance 4 volets.<br/><b>(Cerfa 12708*01)</b>"),
                                                "XCEPT",
                                                "1");
                  }
               else
                  {
                    result = C_InterResultPair( prescr.cip(), "CIP", prescr.drug_name(),   // auquel cas ajouter a la liste des interactions
                                                prescr.cip(), "CIP", tr("Prescription restreinte"),
                                                QObject::tr( "<u><b>M\303\211DICAMENT \303\200 PRESCRIPTION RESTREINTE</b></u> <br />")
                                                + message,
                                                "ESTR",
                                                "1");
                  }
               interResultPairList.append ( result );                            // inserer dans la liste des resultats
               cip_list.removeAll(prescr.cip());                                 // virer le cip de la liste des cip a traiter
               m_interResultPrescrRestrCacheList.insert( prescr.cip(), result ); // inserer dans le cache (ne plus requeter ensuite pour ce produit)
             }
         for ( int i=0; i<cip_list.size(); ++i )
             {  result = C_InterResultPair( cip_list[i], "CIP", "",   // ajouter au cache un truc vide pour noter que le cip est traite mais sans resultat
                                            "DUMY", "", "",
                                            "",
                                            "ESTR",
                                            "1");
                m_interResultPrescrRestrCacheList.insert( cip_list[i], result );
             }
       }
    return interResultPairList;
}

//--------------------------- get_CIS_RCP_From_CIP -----------------------------------------------------------
/*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
 *  \param const QString cip is drug cip code.
 *  \param QString *rcp is pointer on QString to return RCP result if not zero .
 *  \return CIS code string
*/
QString C_BDM_Api::get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp /* =0 */)
{QSqlQuery query(QString::null , database() );
 QString     cis = "";
 QString requete = "";
 if (rcp)   *rcp = "";
 //.......... on essaie avec le plugin commercial ...............
 if (m_pC_BDM_PluginI)
    {cis           = m_pC_BDM_PluginI->get_CIS_and_RCP_From_CIP(cip, rcp);
    }
 //.......... si CIS pas trouve on essaie de trouver le CIS avec le plan B (tables afssaps) ...............
 if (cis.length()==0)
    {requete = QString("SELECT `afs_ciscip_cis_code` FROM `afs_ciscip` WHERE `afs_ciscip_cip_code`  = \"%1\"").arg(cip );
     if (!query.exec(requete))
        {QString err = tr("\nERROR: C_BDM_Api::get_CIS_RCP_From_CIP( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
         outMessage(err);
         return QString::null;
        }
     if ( query.next() ) cis = query.value(0).toString();
    }

 //.......... si CIS trouve et RCP non trouve on essaie si besoin (rcp != 0) de trouver le RCP avec le plan B (tables afssaps) ...............
 if (rcp && cis.length() && rcp->length()==0)
    {requete = QString("SELECT `afs_cis_rcp` FROM `afs_cis` WHERE `afs_cis_code`  = \"%1\"").arg(cis );
     if (!query.exec(requete))
        {QString err = tr("\nERROR: C_BDM_Api::get_CIS_RCP_From_CIP( \n%1\nREQUEST: %2").arg(query.lastError().text(),requete).toLatin1();
         outMessage(err);
         return cis;
        }
     if ( query.next() ) *rcp = query.value(0).toString();
    }
 return cis;
}
//-------------------------------- isStupefiant ------------------------------------------------------
/*! \brief return if the product identified par Id is a stupefiant
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return 1 is stupefiant
 */
int C_BDM_Api::isStupefiant ( const QString &cip )
{if (m_pC_BDM_PluginI)
    {return m_pC_BDM_PluginI->isStupefiant (cip );   // is_stupefiant
    }
 return 0;
}
//-------------------------------- code_ATC_To_Name ------------------------------------------------------
/*! \brief return if ATC libelle from code ATC
 *  \param const QString &code      code ATC .
 *  \return QString return libelle
 */
QString C_BDM_Api::code_ATC_To_Name(const QString &code)
{   if (m_pC_BDM_PluginI)
    {return m_pC_BDM_PluginI->code_ATC_To_Name ( code );  //atc_name
    }
 return "";
}

//-------------------------------- fill_ATC_treeWidget ------------------------------------------------------
/*! \brief fill pQTreeWidget with ATC classification
 *  \param pQTreeWidget QTreeWidget to fill with datas     .
 */
void C_BDM_Api::fill_ATC_treeWidget(QTreeWidget *pQTreeWidget)
{       if (m_pC_BDM_PluginI==0) return;
        QTreeWidgetItem *curParentItem = 0;
        QTreeWidgetItem          *item = 0;
        long                    curLen = 0;
        QString            atc_libelle = "";
        QString              atc_code  = "";

        pQTreeWidget->clear();
        while (m_pC_BDM_PluginI->nextATC_Code(atc_code, atc_libelle))
              {int            len  = atc_code.length();

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
                  { //......... remonter tant que le père n'a pas de partie commune avec le fils ............
                    while( curParentItem->parent() != 0 &&  atc_code.left(curParentItem->text(1).length()) != curParentItem->text(1))
                         {curParentItem = curParentItem->parent();
                         }
                    curParentItem = new QTreeWidgetItem (curParentItem);
                    curParentItem->setText(0,atc_libelle);
                    curParentItem->setText(1,atc_code);
                    curLen = len;
                  }
              }    // end while (query.next()
}
//-------------------------------- deleteUnsignificativesZeros ------------------------------------------------------
/*! \brief delete unsignificative zeros 000123.23  --> 123.00 .
*/
QString C_BDM_Api::deleteUnsignificativesZeros(const QString &value)
{ int i          = 0;
  while ( i < value.length() )
     { if ( value[i] == '0' ) ++i;
       else                   break;
     }
  if ( i==value.length())                  return  "0.00";
  if (value[i] == '.'||value[i] == ',')    return  "0" + value.mid(i);
  else                                     return        value.mid(i);
}
//-------------------------------- Wait[static] ------------------------------------------------------
/*! \brief wait ms microseconds
 * \param : ms time to wait in microseconds
*/
/*
void C_BDM_Api::Wait(int ms)
{ QDateTime td = QDateTime::currentDateTime();
  while (td.msecsTo ( QDateTime::currentDateTime() )<ms)
  {
  }
}
*/
/////////////////////////////////////////////////////////////////// PRIVATE ZONE ///////////////////////////////////////////////////////////////////
//-------------------------------- connectBase ------------------------------------------------------
// on partage la meme base de donnees que celle des patients afin de pouvoir acceder aux donnees patients
// notamment l'historique des ordonnances pour redondances, interactions sur celles en cours
// et recheches statistiques. le modele de stockage sera donc celui de DrTux.
// les acces des BLOBS de DrTux se feront en lecture seulement. Les ordo en cours non enregistres seront elles transmises
// en fichiers de liaison.
// bref un beau foutoir.
int C_BDM_Api::connectBase(const QString &params, const QString &prefixLabel)
{   QString errMess;
    if (BaseConnect( params,                      // parametres.ini
                     prefixLabel+"C_BDM_API",     // label de la connexion SQL
                     "",                          // vide --> le nom de la base sera celui trouve dans les parametres de la section
                     &errMess,                    // recevra le message d'erreur
                     "Connexion_Patient_Base"     // section ou trouver les parametres de connexion a la base
                   )==0
       )
       {setLastError (tr("C_BDM_Api::connectBase() error in BDM connexion \n%1").arg(errMess));
        return 0;
       }
 return 1;
}


