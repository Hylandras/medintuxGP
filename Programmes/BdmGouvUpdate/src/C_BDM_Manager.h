#ifndef C_BDM_Manager_H
#define C_BDM_Manager_H

#include "../../MedinTuxTools-QT4/C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include <QObject>
#include <QFile>

class QSqlQuery;
class QTreeWidget;
class QTreeWidgetItem;
class QLabel;

class C_BDM_Manager : public  C_BaseCommon
{
    Q_OBJECT
public:
     C_BDM_Manager(const QString &iniParam, QObject *parent = 0, QTextEdit *logWidget=0, QProgressBar *pQProgressBar=0);
     ~C_BDM_Manager();

    int           create_DBF(const QString &_fname);
    int           create_CIS_INFOS_bdpm();
    int           create_CIS_CPD_bdpm();
    int           create_CIS_HAS_LINK_CT_bdpm();
    int           create_CIS_HAS_ASMR_bdpm();
    int           create_CIS_HAS_SMR_bdpm();
    int           create_CIS_GENE_bdpm();
    int           create_CIS_bdpm();
    int           create_CIS_CIP_bdpm();
    int           create_CIS_COMPO_bdpm();
    int           fill_table(const QString &tableName, const QString &header, const QString &_fname, int skipFirstLine=0);
    QString       read_line( QFile *file, char *buffer, int &len_read, long maxlen);
    QProgressBar* progressWidget()  { return C_BaseCommon::progressWidget(); }
    QString       dataSourceVersion();
    QString       lang();
    QString       owner();
    QString       getLastError();
    /////////////////////////// INTERACTIONS /////////////////////////////////
    int     it_initInteractionDataBase(        QTreeWidget   *pQTreeWidget,
                                               QLabel        *plabelSynthese,
                                               const QString &owner
                                              );

    QString it_normalisedLevel(const QString   &exoticLevel);
    QString it_createClassLink(const QString &class_pk1, const QString &class_pk2, const QString &mecanism, const QString &level,const QString &cat, const QString &owner);
    QString it_createClassMember(const QString &class_pk, const QString &product_name,const QString &owner);
    QString it_createClassDef(const QString &class_name, const QString &memo,const QString &owner);
    int     it_initialiseProductList(const QMap <QString,int> &produits_Map, const QString &owner);
    QString it_createMemo(const QString &memo,const QString &owner);
    QString it_setStatut(const QString &statut, const QString   &owner);

private:
    QString m_lasError;
    QString m_dataSourceVersion;
    bool    m_isInitialised;
    qint64  m_position;
    QString m_lastDate;
    QString m_dateANSMInter;
public slots:
    void Slot_makeTables();
};


#endif // C_BDM_Manager_H
