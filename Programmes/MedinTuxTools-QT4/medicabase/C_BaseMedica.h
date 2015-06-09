#ifndef C_BASEMEDICA_H
#define C_BASEMEDICA_H

#include "../C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/medicabase/C_DBVar.h"
#include <QObject>



//====================================== C_It_Composant =====================================
class C_It_Composant
{
public:
    explicit    C_It_Composant(const QString &libelle, const QString &descr, const QString &id_ctx, const QString &id_ctx_type)
    {m_libelle      = libelle;
     m_descr        = descr;
     m_id_ctx       = id_ctx;
     m_id_ctx_type  = id_ctx_type;
    }

    QString get_Libelle()     {return m_libelle;}
    QString get_Descriptif()  {return m_descr;}
    QString get_Id_Ctx()      {return m_id_ctx;}
    QString get_Id_Ctx_Type() {return m_id_ctx_type;}

private:
    QString m_libelle;
    QString m_descr;
    QString m_id_ctx;
    QString m_id_ctx_type;
};

class QTreeWidget;
class QTreeWidgetItem;
class QLabel;
class QProgressBar;
class QNetworkAccessManager;
class QNetworkReply;
class QWebView;
class CookieJar;

//====================================== C_BiblioData =====================================
class C_BiblioData
{
 public:
    C_BiblioData();
    explicit    C_BiblioData(const QString pk,
                             const QString owner,
                             const QString lang,
                             const QString url,
                             const QString id,
                             const QString date,
                             const QString note,
                             const QString type,
                             const QString mime,
                             const QString blob_pk,
                             const QString data
                             );
     QString m_pk;
     QString m_id;
     QString m_owner;
     QString m_lang;
     QString m_url;
     QString m_date;
     QString m_note;
     QString m_type;
     QString m_mime;
     QString m_blob_pk;
     QString m_data;
};


//====================================== C_BaseMedica =====================================
class C_BaseMedica : public C_BaseCommon, C_DBVar
{
    Q_OBJECT

public:
    enum flags  {no_filter   = 0,
                 atc_filter  = 1,
                 nom_filter  = 2,
                 dci_filter  = 4,
                 ucd_filter  = 8,
                 all_filter  = 15
                };

    explicit    C_BaseMedica(const QString &param , QString& confData, QObject *parent );
    //...................... settings .........................................
    //void       set_CookieJar(CookieJar *pCookieJar);
    //CookieJar *cookieJar();
    QString     getLastError();
    int         initInteractionDataBase(const QString &file,
                                        QTreeWidget   *pQTreeWidget,
                                        QTreeWidget   *pQTreeWidgetGroup,
                                        QTreeWidget   *pQTreeWidgetSynthese,
                                        QLabel*        plabelSynthese,
                                        const QString &owner ="");
    int  createInteractionForThisComposant (QTreeWidget     *pQTreeWidgetSynthese,
                                            QTreeWidget     *pQTreeWidgetGroup,
                                            QTreeWidgetItem *pQTreeWidgetItemInteractant,
                                            const QString   &libelle_FacteurPere,
                                            const QString   &LibelleInteractant,
                                            const QString   &owner ="" );
    int  setInteractionItem(    QTreeWidget     *pQTreeWidgetSynthese,
                                QTreeWidget     *pQTreeWidgetGroup,
                                const QString   &facteur1,
                                const QString   &facteur2,
                                const QString   &pk2,
                                const QString   &mecanisme,
                                const QString   &niveau,
                                const QString   &cat,
                                const QString   &owner ="");
    QString          tagNiveauToStr(QString &cat);
    QString          it_createInteractionRecord(const QString   &niveau, const QString   &mecanisme, const QString   &cat, const QString &owner);
    QString          it_createLinkFacteursInteraction(const QString &pk1, const QString &pk2, const QString &pki, const QString &class_it, const QString &owner);
    QString          it_createComposant( const QString &libelle,
                                         const QString &descr,
                                         const QString &id_ctx_type,
                                         const QString &id_ctx ,
                                         const QString &id_class,
                                         const QString &owner);

    QString          testInteraction(QTreeWidget *pQTreeWidget, const QString &class_it, const QString &produit_1, const QString &produit_2);
    QString          getNameFacteurFromPk(const QString &pk);
    QTreeWidgetItem *getInteraction(   const QString &pk,      QTreeWidget *pQTreeWidget);

    QTreeWidgetItem *isThisGroupeItem( const QString &libelle, QTreeWidget *pQTreeWidgetGroup);

    QString          createGroupRowInBase(  const QString &libelle,const QString &note, const QString id, const QString &owner);
    int              initATCBase(const QString &fileName, const QString &tableName);
    int              initAfssapsDataBase(   const QString &file,    const QString &tableName, const QString &separator = "\t", const QString &toClean = ""  );
    //............................ BDM interface .............................
    int             BDM_initDrugListFromAfssapsAndBDM(    const QString &owner, const QString &lang  = "fr"   );
    QString         BDM_druglist_tbl_name();
    //................................ GUI ................................
    void  fillQListView_ATC(QTreeWidget *pQTreeWidget );
    void  fill_treeWidget_ClassesIt(QTreeWidget *pQTreeWidget);
    //................................ COVERSIONS ........................
    QString   getDCI_CompositionFromCIP(const QString &cip);

    QString   get_CIS_From_CIP(const QString &cip);
    QString   get_CIP_From_CIS(const QString &cis);
    QString   get_RCP_From_CIS(const QString &cis);

    QString cis_to_atc(const QString &cis);
    QString cip_to_atc(const QString &cip, QString cis = QString::null );

    QString cis_to_libelle_atc(const QString &cis);
    QString cip_to_libelle_atc(const QString &cip);

    QString cis_to_ucd(const QString &cis);
    QString cip_to_ucd(const QString &cip);

    QString cip_to_price(const QString &cip);
    QString ucd_to_price(const QString &ucd);

    QString cis_to_smr(const QString &cis);
    QString cip_to_smr(const QString &cip);
    QString cis_to_asmr(const QString &cis);
    QString cip_to_asmr(const QString &cip);

    QString cis_to_spec(const QString &cis);
    QString cip_to_spec(const QString &cip);


    C_BiblioData biblio_getBiblioDataFromHeadPk(const QString head_pk);
    QString      biblio_getPk_Record(const QString cip, const QString lang,  const QString type  );
    C_BiblioData biblio_getDataFrom_TypeLangId(const QString id, const QString lang,  const QString type);
    QString      biblio_getBlobDataFromHeadPk(const QString head_pk);
    QString      biblio_getDataFromShortCIP(const QString &cip);
    bool         biblio_setLink_Page(const QString &id, const QString &biblioType,  QString urlIn, const QString &owner, const QString &data);
    long         biblio_setLink_Pages(const QString &biblioType,  QString urlIn, const QString &owner);
    QString      biblio_RcpPageFromCIP(const QString &cip);
    QString      biblio_setRecord ( const QString cip,
                                    const QString owner,
                                    const QString lang,
                                    const QString url,
                                    const QString date,
                                    const QString note,
                                    const QString type,
                                    const QString mime,
                                    const QString data);
    QString   biblio_UpdateRecord(const QString pk,
                                  const QString cip,
                                  const QString owner,
                                  const QString lang,
                                  const QString url,
                                  const QString date,
                                  const QString note,
                                  const QString type,
                                  const QString mime,
                                  const QString data);
    QString   biblio_AppendRecord(const QString cip,
                                  const QString owner,
                                  const QString lang,
                                  const QString url,
                                  const QString date,
                                  const QString note,
                                  const QString type,
                                  const QString mime,
                                  const QString data);
    int       biblio_DeleteRecords(const QString &id, const QString &lang,  const QString &type);
    bool      biblio_DeleteBlobRecord(const QString &pk);
    QString   biblio_setBlob(  const QString refPk, const QString owner, const QString data);
    long      setLinkRCP();
    //int       biblio_ReindexeFK();
    //..................... conversion ..................................................
    void               initAfssapsATC(const QString &path);
    long               afssaps_ToMedicaTux();
    QString            afssaps_GetATCCode_fromLibelle(const QString &libelle);
    int                Medica_CreateTableIndexProduit(const QString &table_name);
    int                Medica_CreateTableSpecificationProduit(const QString &table_name);
    QString            createRecordIndexProduit(   const QString &cip,
                                                   const QString &libelle,
                                                   const QString &pk_T00,
                                                   const QString &pk_T4B,
                                                   const QString &cd_prod,
                                                   const QString &cd_uv,
                                                   const QString &is_gene,
                                                   const QString &is_dispo,
                                                   const QString &rcp,
                                                   const QString &cis
                                                  );
    QString            afssaps_GetSpecifications(     const QString &cis,
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
                                                    );
    QString            afssaps_SetSpecifications(     const QString &cip,
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
                                                    );
private:
    int              execQueryGetInteraction(QSqlQuery &query, const QString &requete, QTreeWidget *pQTreeWidget);
    void             setLastError(const QString &error);

signals:

public slots:

private:
    QString                m_LastError;
    int                    m_Progress;
    QString                m_lang;
};

#endif // C_BASEMEDICA_H
