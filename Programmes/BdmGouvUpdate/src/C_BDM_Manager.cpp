#include "C_BDM_Manager.h"
#include "CApp.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QProgressBar>
#include <QDateTime>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>

#define MAX_READ        32000
#define OFSET           ".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"

//-------------------------------- C_BDM_Manager ------------------------------------------------------
C_BDM_Manager::C_BDM_Manager(const QString &iniParam, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
: C_BaseCommon(parent)
{ m_dateANSMInter                 = "";
  m_dataSourceVersion             = "09052012";  // TODO  en attendant le numero provenant de la BD datasemp
  m_lasError                      = "";
  m_isInitialised                 =  0;
  m_lastDate                      = "";
  if (logWidget)        { C_Log::setLogWidget(logWidget);
                        }
  if (pQProgressBar)    { C_BaseCommon::setProgressWidget(pQProgressBar);
                        }
/*
[Connexion_C_BDM_GouvPlugin]
  Parametres = QMYSQL3 , bdm_gouv , root ,  , localhost , 3306
  TimeToReconnect   = 3600000
  PathImageSource   = ../../Themes/Default/medicatux
  max results = 50
*/
  //....................... relever les parametres de connexion ...............................
  QString driver;             // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
  QString schema;             // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
  QString user;               // = "root"
  QString password;           // = ""
  QString hostname;           // = "localhost"
  QString port;               // = "3306"
  if ( CGestIni::Param_ReadParam(  iniParam.toLatin1(), "Connexion_C_BDM_GouvPlugin", "Parametres", &driver, &schema, &user, &password, &hostname, &port ) !=0 )  // zero = pas d'erreur
     { outMessage( tr("ERREUR : C_BDM_Manager::C_BDM_Manager() parameters not found in Section : [Connexion_C_BDM_GouvPlugin] Variable : Parametres"), __FILE__, __LINE__);
       return;
     }
  if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));

  //....................... creer la base ..........................................................
  //                    on utilise mysql comme point d'entree
  QString err = C_BaseCommon::createBase(driver, "mysql", user, password, hostname, port, schema);
  if ( err.length() )
     { outMessage( tr("ERREUR   : C_BDM_Manager::C_BDM_Manager() create database error : "), __FILE__, __LINE__);
       outMessage( tr("driver   : %1").arg(driver));
       outMessage( tr("schema   : %1").arg(schema));
       outMessage( tr("user     : %1").arg(user));
       outMessage( tr("password : %1").arg(password));
       outMessage( tr("hostname : %1").arg(hostname));
       outMessage( tr("port     : %1").arg(port));
       return;
     }
  //....................... se connecter a la base ...............................
  QString errMess = "";
  bool ret = BaseConnect("GOUV_DBM_BASE",
                          driver,
                          schema,
                          user ,
                          password ,
                          hostname ,
                          port.toInt(),
                          DB_TIME_TO_RECONNECT,
                          &errMess   );
  if (ret==0)
     { outMessage( tr("ERREUR   : C_BDM_Manager::C_BDM_Manager() database connexion error : "), __FILE__, __LINE__);
       outMessage( tr("driver   : %1").arg(driver));
       outMessage( tr("schema   : %1").arg(schema));
       outMessage( tr("user     : %1").arg(user));
       outMessage( tr("password : %1").arg(password));
       outMessage( tr("hostname : %1").arg(hostname));
       outMessage( tr("port     : %1").arg(port));
       return;
     }
  outMessage( tr("================= Open Database : <font color=#ff7800><b>%1</b></font> ================================").arg(schema));
  outMessage( tr(". driver   : %1").arg(driver));
  outMessage( tr(". schema   : %1").arg(schema));
  outMessage( tr(". user     : %1").arg(user));
  outMessage( tr(". password : %1").arg(password));
  outMessage( tr(". hostname : %1").arg(hostname));
  outMessage( tr(". port     : %1").arg(port));

  m_isInitialised = 1;
}
//-------------------------------- ~C_BDM_Manager ------------------------------------------------------
C_BDM_Manager:: ~C_BDM_Manager()
{

}

//-------------------------------- create_CIS_COMPO_bdpm ------------------------------------------------------
void  C_BDM_Manager::Slot_makeTables()
{   QDateTime dtDeb       = QDateTime::currentDateTime();
    outMessage( tr("================= start database initialisation : %1 =====").arg(dtDeb.toString("dd-MM-yyyy hh:mm:ss")));
    create_DBF("bdm_atc.csv");
    create_DBF("BDM_CIP.csv");
    create_DBF("BDM_TG.csv");
    create_CIS_INFOS_bdpm();
    create_CIS_CPD_bdpm();
    create_CIS_HAS_LINK_CT_bdpm();
    create_CIS_HAS_ASMR_bdpm();
    create_CIS_HAS_SMR_bdpm();
    create_CIS_CIP_bdpm();
    create_CIS_COMPO_bdpm();
    create_CIS_bdpm();
    create_CIS_GENE_bdpm();
    long    s = dtDeb.secsTo (QDateTime::currentDateTime());
    long    h = s/3600; s -=  h*3600;
    long    m = s/60;   s -=  m*60;
    outMessage( QObject::tr( "=== Intégration SQL terminée en : <font color=#ff0000><b>%1</b></font> Version : <font color=#ff0000><b>%2</b></font> ==")
                                            .arg(QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s", m_lastDate));
}

//-------------------------------- create_CIS_INFOS_bdpm ------------------------------------------------------
int  C_BDM_Manager::create_DBF(const QString &_fname)
{   QString fname                = CApp::pCApp()->pathAppli()+"Ressources/download/"+_fname;
    QString field_def            = "";
    QString requete              = "";
    QString tableName            = _fname.toLower();  tableName.chop(4); // virer le ".cvs"
    QString header               = "";
    QString fieldName            = "";
    QStringList fields_Header_list;
    QStringList field_def_struct;
    if ( !QFile::exists( fname ) )
       { outMessage( QObject::tr("ERREUR : C_BDM_Manager::create_DBF() '%1' don't exists").arg(fname), __FILE__, __LINE__);
         return 0;
       }

    QFile file( fname );
    if ( !file.open( QIODevice::ReadOnly ) ) return 0;
    long maxlen           = 3512000;
    char    *buffer       = new char[maxlen+50];    // +5 pour permettre analyse utf8 qui explore trois apres
    QString line          = "";
    int  len_read         = 0;
    if (buffer==0) {      outMessage( QObject::tr("C_BDM_Manager::create_CIS_CIP_bdpm() Reservation du buffer de lecture <font color=#01eaff><b>%1</b></font> octets impossible.")
                                                  .arg(QString::number(maxlen)));
                          return 0;
                   }

    //.......... on déclare une variable pour lire le fichier ligne à ligne ..................
    //           et on lit la premiere ligne (celle des headers)
    line             = read_line( &file, buffer, len_read, maxlen);
    int            p = line.length();
    while (p>0 && (line[p-1]=='\n' || line[p-1]=='\r'|| line[p-1]=='\t')) {--p;}
    line.truncate(p);
    fields_Header_list = line.split("\t");
    delete buffer;
    if (fields_Header_list.length()==0)
       { outMessage( QObject::tr("ERREUR : C_BDM_Manager::create_DBF() empty headers list ").arg(fname), __FILE__, __LINE__);
         return 0;
       }
    requete = "CREATE TABLE `"+tableName+"` (\n"+
             " `"+tableName+"_pk`         bigint(20) NOT NULL auto_increment ,\n";
    // NOM_COURT,C,50
    // CODE_GRP,N,6,0
    // NOM_GRP_G,C,124
    // DT_DEB_AFS,D
    for (int i=0; i<fields_Header_list.size();++i)
       {  field_def        = fields_Header_list[i];
          field_def_struct = field_def.split(',');
          if (field_def_struct.size()<2) continue;
          fieldName        = tableName+"_"+field_def_struct[0].toLower();
          header          += fieldName + ",";
          requete         += " `"+fieldName+"` ";
          switch (field_def_struct[1][0].toLatin1())
          { case 'N':
            case 'C':
              {  if (field_def_struct[2].toInt()<=255)  requete += QString("char(%1)").arg(field_def_struct[2])+",\n";
                 else                                   requete += QString("varchar(%1)").arg(field_def_struct[2])+",\n";
              }  break;
            case 'D':
              {                                         requete += "char(10),\n";
              }  break;
            default:
              {  if (field_def_struct[2].toInt()<=255)  requete += QString("char(%1)").arg(field_def_struct[2])+",\n";
                 else                                   requete += QString("varchar(%1)").arg(field_def_struct[2])+",\n";
              }  break;
          } //endswitch (field_def_struct[1].toLatin1())
       } // endfor (int i=0; i<fields_Header_list.size();++i)
    header.chop(1);    // virer la derniere virgule
    requete += "PRIMARY KEY  (`"+tableName+"_pk`)\n"
               ") ENGINE=MyISAM AUTO_INCREMENT=0 ;";
    dropTable(tableName);
    createTable(requete);
    return    fill_table( tableName, header, _fname,1);
}

//-------------------------------- create_CIS_INFOS_bdpm ------------------------------------------------------
/*
Le nom de ce fichier est : CIS_InfoImportantes_AAAAMMJJhhmiss_bdpm.txt
On ajoute la date de génération au nom de ce fichier car, contrairement aux autres fichiers, celui-ci est généré en direct.
Il contient l’ensemble des informations importantes disponibles pour les médicaments de la base de données publique des médicaments.
Les éléments mis à disposition dans ce fichier sont les suivants :
- Code CIS
- Date de début de l’information de sécurité (format JJ/MM/AAAA)
- Date de fin de l’information de sécurité (format JJ/MM/AAAA)
- Texte à afficher et lien vers l’information de sécurité
*/
int  C_BDM_Manager::create_CIS_INFOS_bdpm()
{   QString date_time;
    QString tableName = "gouv_cis_infos";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                    \n"
        "  `gouv_cis_infos_pk`         bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_infos_cis`        char(8)                            ,\n"   // 0
        "  `gouv_cis_infos_date_deb`   char(10)                           ,\n"   // 1
        "  `gouv_cis_infos_date_fin`   char(10)                           ,\n"   // 2
        "  `gouv_cis_infos_memo`       TEXT                               ,\n"   // 3
        "  PRIMARY KEY  (`gouv_cis_infos_pk`)                              \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        //............... rechercher le fichier plus recent ................................
        //                CIS_InfoImportantes_AAAAMMJJhhmiss_bdpm.txt
        QString fname         = "";
        double  last_date     =  0;
        QStringList fnameList = CGestIni::listDirectory(CApp::pCApp()->pathAppli()+"Ressources/download/", "", "CIS_InfoImportantes_");
        for (int i=0; i<fnameList.size();++i)
            { date_time = CGestIni::getListBetweenTwoTags(fnameList[i], "CIS_InfoImportantes_", "_bdpm.txt")[0];
              if ( date_time.toDouble() > last_date)
                 { last_date   = date_time.toDouble();
                   fname       = fnameList[i];
                   m_lastDate  = date_time;
                 }
            }
        if (fname.length()==0) return 0;
        QString header    = "gouv_cis_infos_cis,"         // 0
                            "gouv_cis_infos_date_deb,"    // 1
                            "gouv_cis_infos_date_fin,"    // 2
                            "gouv_cis_infos_memo";        // 3
    int ret = fill_table( tableName, header, fname);
    if (ret==0) return 0;
    //............. recuperer la date pour faire le versionning ....................
    tableName = "gouv_statut";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                     \n"
        "  `gouv_statut_pk`             bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_statut_version`        char(50)                           ,\n"   // 0
        "  PRIMARY KEY  (`gouv_statut_pk`)                                  \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
    header    = "gouv_statut_version";         // 0
    fname     = "CIS_VERSION_bdpm.txt";
    CGestIni::Param_UpdateToDisk(CApp::pCApp()->pathAppli()+"Ressources/download/"+fname, "Date : " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") + " Version : " + m_lastDate);
    return fill_table( tableName, header, fname);
}

//-------------------------------- create_CIS_CPD_bdpm ------------------------------------------------------
/*
Le nom de ce fichier est : CIS_CPD_bdpm.txt
Il contient l’ensemble des conditions de prescription et de délivrance associées à un médicament.
Les éléments mis à disposition dans ce fichier sont les suivants :
- Code CIS
- Condition de prescription ou de délivrance
*/
int  C_BDM_Manager::create_CIS_CPD_bdpm()
{
    QString tableName = "gouv_cis_cpd";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                  \n"
        "  `gouv_cis_cpd_pk`         bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_cpd_cis`        char(8)                            ,\n"   // 0
        "  `gouv_cis_cpd_condition`  char(255)                          ,\n"   // 1
        "  PRIMARY KEY  (`gouv_cis_cpd_pk`)                              \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_CPD_bdpm.txt";
        QString header    = "gouv_cis_cpd_cis,"           // 0
                            "gouv_cis_cpd_condition";     // 1
    return    fill_table( tableName, header, fname);
}
//-------------------------------- create_CIS_HAS_LINK_CT_bdpm ------------------------------------------------------
/*
Le nom de ce fichier est : HAS_LiensPageCT_bdpm.txt
Il contient l’ensemble des liens permettant d’accéder aux avis complets de la commission de la transparence pour les SMR et ASMR postérieurs à l’année 2002.
Les éléments mis à disposition dans ce fichier sont les suivants :
- Code de dossier HAS
- Lien vers les pages d’avis de la CT
*/
int  C_BDM_Manager::create_CIS_HAS_LINK_CT_bdpm()
{
    QString tableName = "gouv_cis_has_link_ct";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                          \n"
        "  `gouv_cis_has_link_ct_pk`         bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_has_link_ct_code_doss`  char(15)                           ,\n"   // 1
        "  `gouv_cis_has_link_ct_page_ct`    char(255)                          ,\n"   // 2
        "  PRIMARY KEY  (`gouv_cis_has_link_ct_pk`)                              \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "HAS_LiensPageCT_bdpm.txt";
        QString header    = "gouv_cis_has_link_ct_code_doss,"   // 0
                            "gouv_cis_has_link_ct_page_ct";     // 1
    return    fill_table( tableName, header, fname);
}
//-------------------------------- create_CIS_HAS_ASMR_bdpm ------------------------------------------------------
/*
Le nom de ce fichier est : CIS_HAS_ASMR_bdpm.txt
Il contient l’ensemble des avis d’ASMR de la HAS postérieurs à l’année 2002.
Les éléments mis à disposition dans ce fichier sont les suivants :
- Code CIS
- Code de dossier HAS
- Motif d’évaluation
- Date de l’avis de la Commission de la transparence (format AAAAMMJJ)
- V aleur de l’ ASMR
- Libelle de l’ ASMR
*/
int  C_BDM_Manager::create_CIS_HAS_ASMR_bdpm()
{
    QString tableName = "gouv_cis_has_asmr";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                       \n"
        "  `gouv_cis_has_asmr_pk`         bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_has_asmr_cis`        char(8)                            ,\n"   // 0
        "  `gouv_cis_has_asmr_code_doss`  char(15)                           ,\n"   // 1
        "  `gouv_cis_has_asmr_motif`      char(80)                           ,\n"   // 2
        "  `gouv_cis_has_asmr_date`       char(10)                           ,\n"   // 3
        "  `gouv_cis_has_asmr_level`      char(20)                           ,\n"   // 4
        "  `gouv_cis_has_asmr_memo`       TEXT                               ,\n"   // 5
        "  PRIMARY KEY  (`gouv_cis_has_asmr_pk`)                              \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_HAS_ASMR_bdpm.txt";
        QString header    = "gouv_cis_has_asmr_cis,"          // 0
                            "gouv_cis_has_asmr_code_doss,"    // 1
                            "gouv_cis_has_asmr_motif,"        // 2
                            "gouv_cis_has_asmr_date,"         // 3
                            "gouv_cis_has_asmr_level,"        // 4
                            "gouv_cis_has_asmr_memo";         // 5
    return    fill_table( tableName, header, fname);
}
//-------------------------------- create_CIS_HAS_SMR_bdpm ------------------------------------------------------
/*
Le nom de ce fichier est : CIS_HAS_SMR_bdpm.txt
Il contient l’ensemble des avis de SMR de la HAS postérieurs à l’année 2002.
Les éléments mis à disposition dans ce fichier sont les suivants :
- Code CIS
￼￼- Code de dossier HAS
- Motif d’évaluation
- Date de l’avis de la Commission de la transparence (format AAAAMMJJ)
- V aleur du SMR
- Libelle du SMR
*/
int  C_BDM_Manager::create_CIS_HAS_SMR_bdpm()
{
    QString tableName = "gouv_cis_has_smr";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                       \n"
        "  `gouv_cis_has_smr_pk`          bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_has_smr_cis`         char(8)                            ,\n"   // 0
        "  `gouv_cis_has_smr_code_doss`   char(15)                           ,\n"   // 1
        "  `gouv_cis_has_smr_motif`       char(80)                           ,\n"   // 2
        "  `gouv_cis_has_smr_date`        char(10)                           ,\n"   // 3
        "  `gouv_cis_has_smr_level`       char(20)                           ,\n"   // 4
        "  `gouv_cis_has_smr_memo`        TEXT                               ,\n"   // 5
        "  PRIMARY KEY  (`gouv_cis_has_smr_pk`)                               \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_HAS_SMR_bdpm.txt";
        QString header    = "gouv_cis_has_smr_cis,"          // 0
                            "gouv_cis_has_smr_code_doss,"    // 1
                            "gouv_cis_has_smr_motif,"        // 2
                            "gouv_cis_has_smr_date,"         // 3
                            "gouv_cis_has_smr_level,"        // 4
                            "gouv_cis_has_smr_memo";         // 5
    return    fill_table( tableName, header, fname);
}
//-------------------------------- create_CIS_GENE_bdpm ------------------------------------------------------
/*
 *
 * Le nom de ce fichier est : CIS_GENER_bdpm.txt
Il contient l’ensemble groupes génériques, avec les médicaments en faisant partie.
Les éléments mis à disposition dans ce fichier sont les suivants :
- Identifiant du groupe générique
- Libellé du groupe générique
- Code CIS
- Type de générique, avec les valeurs suivantes :
          o 0:«princeps»
          o 1:«générique»
          o 2 : « génériques par complémentarité posologique » o 4 : « générique substituable »
- Numéro permettant de trier les éléments d’un groupe
*/
int  C_BDM_Manager::create_CIS_GENE_bdpm()
{
    QString tableName = "gouv_cis_gener";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                   \n"
        "  `gouv_cis_gener_pk`              bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_gener_id`              char(255)                          ,\n"   // 0
        "  `gouv_cis_gener_libelle`         char(255)                          ,\n"   // 1
        "  `gouv_cis_gener_code_cis`        char(8)                            ,\n"   // 2
        "  `gouv_cis_gener_type`            char(2)                            ,\n"   // 3
        "  `gouv_cis_gener_num_tri`         char(15)                           ,\n"   // 4
        "  PRIMARY KEY  (`gouv_cis_gener_pk`)                                   \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_GENER_bdpm.txt";
        QString header    = "gouv_cis_gener_id,"           // 0
                            "gouv_cis_gener_libelle,"      // 1
                            "gouv_cis_gener_code_cis,"     // 2
                            "gouv_cis_gener_type,"         // 3
                            "gouv_cis_gener_num_tri";      // 4
    return    fill_table( tableName, header, fname);
}

//-------------------------------- create_CIS_COMPO_bdpm ------------------------------------------------------
/*
 *
- Code CIS (Code Identifiant de Spécialité)
- Dénomination du médicament
- Forme pharmaceutique
- Voies d'administration (avec un séparateur « ; » entre chaque valeur quand il y en plusieurs)
- Statut administratif de l’autorisation de mise sur le marché (AMM)
- Type de procédure d'autorisation de mise sur le marché (AMM)
- Etat de commercialisation
- Date d’AMM (format JJ/MM/AAAA)
- StatutBdm : valeurs possibles : « Alerte » (icône rouge) ou « Warning disponibilité » (icône grise)
- Numéro de l’autorisation européenne
- Titulaire(s) : S’il y a plusieurs titulaires, les valeurs seront séparées par des « ; »
- Surveillance renforcée (triangle noir) : valeurs « Oui » ou « Non »
*/
int  C_BDM_Manager::create_CIS_bdpm()
{
    QString tableName = "gouv_cis";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                   \n"
        "  `gouv_cis_pk`              bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_code_cis`        char(8)                            ,\n"   // 0
        "  `gouv_cis_denomination`    char(255)                          ,\n"   // 1
        "  `gouv_cis_forme`           char(100)                          ,\n"   // 2
        "  `gouv_cis_voie`            char(100)                          ,\n"   // 3
        "  `gouv_cis_amm_statut`      char(40)                           ,\n"   // 4
        "  `gouv_cis_amm_type_proc`   char(40)                           ,\n"   // 5
        "  `gouv_cis_comm_statut`     char(40)                           ,\n"   // 6
        "  `gouv_cis_amm_date`        char(10)                           ,\n"   // 7
        "  `gouv_cis_alerte`          char(30)                           ,\n"   // 8
        "  `gouv_cis_num_europ_auto`  char(15)                           ,\n"   // 9
        "  `gouv_cis_titulaire`       char(50)                           ,\n"   // 10
        "  `gouv_cis_surveillance`    char(10)                           ,\n"   // 11
        "  PRIMARY KEY  (`gouv_cis_pk`)                                   \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_bdpm.txt";
        QString header    = "gouv_cis_code_cis,"               // 0
                            "gouv_cis_denomination,"           // 1
                            "gouv_cis_forme,"                  // 2
                            "gouv_cis_voie,"                   // 3
                            "gouv_cis_amm_statut,"             // 4
                            "gouv_cis_amm_type_proc,"          // 5
                            "gouv_cis_comm_statut,"            // 6
                            "gouv_cis_amm_date,"               // 7
                            "gouv_cis_alerte,"                 // 8
                            "gouv_cis_num_europ_auto,"         // 9
                            "gouv_cis_titulaire,"              // 10
                            "gouv_cis_surveillance";           // 11

    return    fill_table( tableName, header, fname);
}

//-------------------------------- create_CIS_COMPO_bdpm ------------------------------------------------------
int  C_BDM_Manager::create_CIS_COMPO_bdpm()
{
    QString tableName = "gouv_cis_compo";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `"+tableName+"` (                                       \n"
        "  `gouv_cis_compo_pk`            bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_compo_code_cis`      char(8)                            ,\n"   // 0
        "  `gouv_cis_compo_forme`         char(50)                           ,\n"   // 1
        "  `gouv_cis_compo_code_subst`    char(10)                           ,\n"   // 2
        "  `gouv_cis_compo_libelle_subst` char(255)                          ,\n"   // 3
        "  `gouv_cis_compo_dosage_subst`  char(100)                          ,\n"   // 4
        "  `gouv_cis_compo_dosage_ref`    char(100)                          ,\n"   // 5
        "  `gouv_cis_compo_nature`        char(4)                            ,\n"   // 6
        "  `gouv_cis_compo_link`          char(13)                           ,\n"   // 7
        "  PRIMARY KEY  (`gouv_cis_compo_pk`)                                 \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_COMPO_bdpm.txt";
        QString header    = "gouv_cis_compo_code_cis,"        // 0
                            "gouv_cis_compo_forme,"           // 1
                            "gouv_cis_compo_code_subst,"      // 2
                            "gouv_cis_compo_libelle_subst,"   // 3
                            "gouv_cis_compo_dosage_subst,"    // 4
                            "gouv_cis_compo_dosage_ref,"      // 5
                            "gouv_cis_compo_nature,"          // 6
                            "gouv_cis_compo_link";            // 7

    return    fill_table( tableName, header, fname);
}

//-------------------------------- create_CIS_CIP_bdpm ------------------------------------------------------
/*
        CIS_CIP_bdpm
        Code CIS
         ￼￼￼￼Code CIP7
        Libellé de la présentation
        Statut administratif de la présentation
        Etat de commercialisation
        Date de la déclaration de commercialisation
        Code CIP13
        Agrément aux collectivités
        Taux de remboursement
        Prix du médicament en euro
        Indications ouvrant droit au remboursement
*/
int  C_BDM_Manager::create_CIS_CIP_bdpm()
{
    QString tableName = "gouv_cis_cip";
    dropTable(tableName);
    createTable(
        "CREATE TABLE `gouv_cis_cip` (                                     \n"
        "  `gouv_cis_cip_pk`           bigint(20) NOT NULL auto_increment ,\n"
        "  `gouv_cis_cip_code_cis`     char(8)                            ,\n"   // 0
        "  `gouv_cis_cip_code_cip7`    char(7)                            ,\n"   // 1
        "  `gouv_cis_cip_libelle`      char(255)                          ,\n"   // 2
        "  `gouv_cis_cip_status_adm`   char(100)                          ,\n"   // 3
        "  `gouv_cis_cip_status_com`   char(100)                          ,\n"   // 4
        "  `gouv_cis_cip_date_com`     char(10)                           ,\n"   // 5
        "  `gouv_cis_cip_cip13`        char(13)                           ,\n"   // 6
        "  `gouv_cis_cip_agr_coll`     char(10)                           ,\n"   // 7
        "  `gouv_cis_cip_taux_remb`    char(6)                            ,\n"   // 8
        "  `gouv_cis_cip_prix`         char(20)                           ,\n"   // 9
        "  `gouv_cis_cip_remb_memo`    TEXT                               ,\n"   // 10
        "  PRIMARY KEY  (`gouv_cis_cip_pk`)                                \n"
        ") ENGINE=MyISAM AUTO_INCREMENT=0 ;                ");
        QString fname     = "CIS_CIP_bdpm.txt";
        QString header    = "gouv_cis_cip_code_cis,"      // 0
                            "gouv_cis_cip_code_cip7,"     // 1
                            "gouv_cis_cip_libelle,"       // 2
                            "gouv_cis_cip_status_adm,"    // 3
                            "gouv_cis_cip_status_com,"    // 4
                            "gouv_cis_cip_date_com,"      // 5
                            "gouv_cis_cip_cip13,"         // 6
                            "gouv_cis_cip_agr_coll,"      // 7
                            "gouv_cis_cip_taux_remb,"     // 8
                            "gouv_cis_cip_prix,"          // 9
                            "gouv_cis_cip_remb_memo";     // 10
    return    fill_table( tableName, header, fname);
}

//----------------------------------------------------- fill_table    -------------------------------------------
int C_BDM_Manager::fill_table( const QString &tableName, const QString &header, const QString &_fname, int skipFirstLine /*=0*/)
{       QString fname                = CApp::pCApp()->pathAppli()+"Ressources/download/"+_fname;
        QStringList fields_Name_list = header.split(",");
        QStringList fields_Data_list;
        QProgressBar *pQProgressBar  = progressWidget();
        if ( !QFile::exists( fname ) )
           { outMessage( QObject::tr("ERREUR : C_BDM_Manager::create_CIS_CIP_bdpm() '%1' don't exists").arg(fname), __FILE__, __LINE__);
             return 0;
           }

        QFile file( fname );
        if ( !file.open( QIODevice::ReadOnly ) ) return 0;
        long maxlen           = 3512000;
        char    *buffer       = new char[maxlen+50];    // +5 pour permettre analyse utf8 qui explore trois apres
        QString requete       = "";
        QString line          = "";
        int  len_read         = 0;
        int          pos_line = 0;
        int    nb_initialised = 0;
        if (buffer==0) {      outMessage( QObject::tr("C_BDM_Manager::create_CIS_CIP_bdpm() Reservation du buffer de lecture <font color=#01eaff><b>%1</b></font> octets impossible.")
                                                      .arg(QString::number(maxlen)));
                              return 0;
                       }
        outMessage( tr(".               create Table : <font color=#01eaff><b>%1</b></font>").arg(tableName));
        m_position = 0;
        if (pQProgressBar) pQProgressBar->setRange(0,file.size()/1000);
        //.......... on déclare une variable pour lire le fichier ligne à ligne ..................
        QSqlQuery query(QString::null, database());
        while (!file.atEnd())
        {   line             = read_line( &file, buffer, len_read, maxlen);
            m_position      += len_read;
            int            p = line.length();
            ++ pos_line;
            if (skipFirstLine)
              { skipFirstLine = 0;
                continue;
              }
            while (p>0 && (line[p-1]=='\n' || line[p-1]=='\r'|| line[p-1]=='\t')) {--p;}
            line.truncate(p);
            fields_Data_list = line.split("\t");
            if (fields_Data_list.size()<=fields_Name_list.size())
               {    QString prepare    = "INSERT INTO `"+ tableName +"` ( \n";
                    QString bind_value = ") VALUES (";
                    for (int i=0; i<fields_Data_list.size();++i)
                        { prepare       += "`"+ fields_Name_list[i]      +"`,\n";
                          bind_value    += "?,";
                        }
                    prepare.chop(2);
                    bind_value.chop(1);
                    prepare += bind_value + ")";
                    if ( !query.prepare(prepare))
                       {
                         outMessage( QObject::tr("ERROR : C_BDM_Manager::create_CIS_CIP_bdpm() prepare \n%1 \n Last error : <font color=#01eaff><b>%2</b></font>.")
                                                                             .arg(requete,query.lastError().text()));
                       }
                    else
                       {
                         for (int i=0; i<fields_Data_list.size();++i)
                             { query.bindValue(i, fields_Data_list[i]);
                             }
                         if ( !query.exec())
                            { outMessage(  QObject::tr("ERREUR  : C_BDM_Manager::create_CIS_CIP_bdpm() query.exec \n%1\nREQUETE : %2").arg(query.lastError().text(),prepare).toLatin1());
                            }
                         else
                            { ++nb_initialised;
                            }
                       }
               }  // if (fields_Data_list.size()==fields_Name_list.size())
            else
               { outMessage(  QObject::tr("ERREUR  : C_BDM_Manager::INSERT INTO( %1 ) fields numbers Datas : %2 greater than Header : %3")
                              .arg(QString::number(pos_line),QString::number(fields_Data_list.size()),QString::number(fields_Name_list.size())));
               }
            if (pQProgressBar)  {pQProgressBar->setValue(m_position/1000); qApp->processEvents();qApp->processEvents();}
        }  // endwhile (!file.atEnd())
        outMessage( tr(". initialised with <font color=#ffe26e><b>%1 rows.</b></font>").arg(QString::number(nb_initialised)));
        outMessage(" ");
        delete buffer;
 return 1;
}

//----------------------------------------------------- read_line    -------------------------------------------
QString  C_BDM_Manager::read_line( QFile *file, char *buffer, int &len_read, long maxlen)
{       len_read         = (int)file->readLine ( buffer, maxlen );
        buffer[len_read] = 0;    //  on assure le zero de fin
        int isUtf8       = CGestIni::IsUtf8  ( buffer);
        if (isUtf8)
           {return  QString::fromUtf8 ( buffer ) ;
           }
        else
           {return  QString (buffer);
           }
}

//-------------------------------- dataSourceVersion ------------------------------------------------------
QString  C_BDM_Manager::dataSourceVersion()
{ return m_dataSourceVersion;
}

//-------------------------------- lang ------------------------------------------------------
QString  C_BDM_Manager::lang()
{ return "fr";
}
//-------------------------------- owner ------------------------------------------------------
QString  C_BDM_Manager::owner()
{ return "OWN";
}
//-------------------------------- getLastError ------------------------------------------------------
QString  C_BDM_Manager::getLastError()
{return m_lasError;
}

/****************************************************************************************************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// INTERACTIONS /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*****************************************************************************************************************/

//--------------------------- it_initInteractionDataBase -----------------------------------------------------------
int C_BDM_Manager::it_initInteractionDataBase( QTreeWidget   *pQTreeWidget,
                                               QLabel        *plabelSynthese,
                                               const QString &owner
                                             )
{
    //............... rechercher le fichier plus recent ................................
    //                interractionsAfssaps-20140512.txt
    QString _fname         = "";
    double  last_date     =  0;
    QString   date_time   = "";
    QStringList fnameList = CGestIni::listDirectory(CApp::pCApp()->pathAppli()+"Ressources/download/", "", "interractionsAfssaps-");
    for (int i=0; i<fnameList.size();++i)
        { date_time = CGestIni::getListBetweenTwoTags(fnameList[i], "interractionsAfssaps-", ".txt")[0];
          if ( date_time.toDouble() > last_date)
             { last_date   = date_time.toDouble();
                _fname     = fnameList[i];
             }
          }
    if (_fname.length()==0) return 0;
    QString fname = CApp::pCApp()->pathAppli()+"Ressources/download/"+_fname;
    addOwner(owner);
    QString composant               ="";
    QString interactant             =" ";
    QString lineTrm                 ="";
    int     posInterRes             = -1;
    int     pos                     = -1;
    QString s                       = "";
    int     position                = 0;
    QString productName             = "";
    QTreeWidgetItem *pItemComposant = 0;
    QTreeWidgetItem *pItemInteract  = 0;
    QStringList  membreList;
    QMap <QString,int> produits_Map;  // la clef est le nom du produit (pour qu'il soit unique) la value est sans importance;
    QMap <QString,int> className_Map; // la clef est le nom de la classe (pour qu'ellr soit unique) la value est sans importance;

    int posFirstNSpace;
    //.............. menage .....................
    pQTreeWidget->clear();
    //....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fname ) )            return 0;
    outMessage( tr("----------  Cr\303\251ation des tables d'interactions <font color=#01eaff><b>%1</b></font> en cours ----------------").arg( "it_compo, it_group, it_inter, it_link"));
    outMessage( tr(" Fichier source : <font color=#01eaff><b>%1</b></font>").arg( _fname ));
    plabelSynthese->setText(tr("Analyse de premier niveau (isoler les composants)"));
    outMessage( tr("%1Analyse de premier niveau <font color=#ffd270><b>%2</b></font>").arg(OFSET,"(isoler les composants)"));
    //...................... PARSER LE FICHIER L'ECLATER ET..............................
    //                       REMPLIR le QTreeWidget avec les infos

    QFile file( fname );
    if ( !file.open( QIODevice::ReadOnly ) ) return 0;
    long maxlen           = MAX_READ;
    char    *buffer       = new char[maxlen+50];    // +5 pour permettre analyse utf8 qui explore trois apres
    QString line          = "";
    int  len_read         = 0;

    if (buffer==0) {      outMessage( QObject::tr("C_BDM_Manager::create_CIS_CIP_bdpm() Reservation du buffer de lecture <font color=#01eaff><b>%1</b></font> octets impossible.")
                                                  .arg(QString::number(maxlen)));
                          return 0;
                   }
    while (!file.atEnd())
          { line             = read_line( &file, buffer, len_read, MAX_READ);
            position        += len_read; if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}

           lineTrm          = line.trimmed();
           posFirstNSpace   = posFirstNonSpace(line);
           //................. rechercher separation mecanisme / conduite a tenir .....................
           if (posInterRes==-1)  posInterRes = line.indexOf("CONTRE-INDICATION");
           if (posInterRes==-1)  posInterRes = line.indexOf("Association DECONSEILLEE");
           if (posInterRes==-1)  posInterRes = line.indexOf(tr("Précaution d'emploi"));
           if (posInterRes==-1)  posInterRes = line.indexOf("A prendre en compte");
           if (posInterRes==-1)  posInterRes = line.indexOf("CI - ASDEC");
           if (posInterRes==-1)  posInterRes = line.indexOf("CI - PE");
           if (posInterRes==-1)  posInterRes = line.indexOf("CI - APEC");
           if (posInterRes==-1)  posInterRes = line.indexOf("ASDEC - PE");
           if (posInterRes==-1)  posInterRes = line.indexOf("ASDEC - APEC");
           //if (posInterRes==-1) posInterRes = line.indexOf("CI - ASDEC - APEC");
           //if (posInterRes==-1) posInterRes = line.indexOf("CI - ASDEC - PE");
            if (lineTrm .startsWith("INHIBITEURS PUISSANT"))
               {
                qDebug()<<"INHIBITEURS PUISSANT";
               }
           //............................. numero de page .............................................
           if (lineTrm.length()< 8 && lineTrm[0]>='0'&&lineTrm[0]<='9')
              {//qDebug(tr("Page : %1").arg(lineTrm).toLatin1());
              }
           else if (lineTrm.startsWith("ANSM -"))
              {if (m_dateANSMInter.length()==0){ m_dateANSMInter=lineTrm.left(26).trimmed();}
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
              {//qDebug(tr("=> commentaire : %1 <=").arg(lineTrm).toLatin1());
               if (pItemComposant)
                  {s = pItemComposant->text(1); if (s.length()) s+="\n"; s += lineTrm;
                   pItemComposant->setText(1,s);
                  }
              }
           //..................... titre d'un composant .........................................
           else if (posFirstNSpace<1 && lineTrm[0] != '+')
              {composant   = lineTrm;
               interactant = "";
               //qDebug(tr("=> composant : %1 <=").arg(composant).toLatin1());
               pItemComposant = new QTreeWidgetItem(pQTreeWidget);
               pItemComposant->setText(0, composant);
               pItemComposant->setExpanded(TRUE);
              }
           //..................... titre d'un interactant .........................................
           else if (posFirstNSpace<2 && lineTrm[0] == '+' && interactant != lineTrm)
              {interactant = lineTrm;
               composant   = "";
               posInterRes = -1;    // reinitialiser la variable au prochain tour (normalement la prochaine ligne a venir le permet)
               //qDebug(tr("     + interactant : %1").arg(interactant).toLatin1());
               if (pItemComposant)
                  { pItemInteract = new QTreeWidgetItem(pItemComposant);
                    pItemInteract->setText(0, interactant);
                    //....... verifier que le composant de classe ait au moins une liste de produit (au moins lui meme si mentionne juste un produit) .....
                    s   = pItemComposant->text(1);
                    pos = posLastEnclosedSymbol(s,"(",")");
                    if (pos==-1) pItemComposant->setText(1,"("+pItemComposant->text(0)+")");
                  }
              }
          } // endwhile (!file.atEnd() && (tryCreate || tryInsert))

    //...................... REMPLIR la QMap des noms de classe ..............................
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
       if (s .startsWith("INHIBITEURS PUISSANT"))
          {
           qDebug()<<"INHIBITEURS PUISSANT";
          }
       if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
       ++ position;
       if ( s[0] != '+' )                              // un nom de la classe ne commence pas par +
          { className_Map[s.trimmed().toUpper()]=-1;   // on cree l'item dans la QMap des noms de classe_it
          } // if ( s[0] != '+' )
       ++it;
     } //while (*it)
    //................... maintenant que la QMap des noms de classe_it est renseignee ...........................
    //                    on peut reparser les composants precedes du + en eliminant ceux qui sont des noms de classe
    //                    pour n'ajouter que les produits.
    //
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(pQTreeWidget->topLevelItemCount()/10));
    position = 0;
    it       = QTreeWidgetItemIterator(pQTreeWidget);
    plabelSynthese->setText(tr("Extraction des produits d'interactions."));
    outMessage( tr("%1Extraction des <font color=#ffd270><b>%2</b></font> en cours....").arg(OFSET,"produits d'interactions"));
    while (*it)
     { s = (*it)->text(0);
       if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
       ++ position;
       if (s[0] == '+')                                                      // on est sur un interactant d'une classe qui peut etre soit un nom de classe_it soit un produit isole
          { s = s.mid(1).trimmed().toUpper();                                // on elimine le + on trimme on majuscule
            QMap<QString, int>::const_iterator fi = className_Map.find( s ); // on recherche si ce n'est pas une classe_it
            if ( fi == className_Map.end() )                                 // et si pas classe_it --> ajouter nom du produit dans la liste
               { productName = s.simplified();
                 productName = CGestIni::CutStrLeft(productName, "(");
                 productName = CGestIni::CutStrLeft(productName, "/");
                 produits_Map[productName]=0;
               }
          }
       else                                                                  // on est sur une definition de classe_it
          { s   = (*it)->text(1).trimmed();                                  // on recupere le texte de la classe_it pour en extraire la liste des composants (entre parentheses)
            pos = posLastEnclosedSymbol(s,"(",")", &posFermant);
            if ( pos != -1 )
               { if (posFermant != -1) s.truncate(posFermant);
                 ++pos;
                 line = s;
                 s = s.mid(pos);
                 s = s.trimmed();
                 if (!s.startsWith("sauf") && !line.startsWith("Voir ") && !s.startsWith("voir "))
                    { membreList = s.split(',');
                      for ( i=0; i<membreList.size(); ++i)
                          { productName = membreList[i].trimmed().toUpper().replace("\n"," ").simplified();
                            if (productName.startsWith("DI(ACEFYLLINE)"))  // di(acefylline) cas particulier pourri
                               { s = CGestIni::CutStrRight(productName, ")").trimmed();
                                 produits_Map[s]=0;
                                 productName = CGestIni::CutStrLeft(productName, ")").remove('(');
                                 produits_Map[productName]=0;
                               }
                            else
                               { productName = CGestIni::CutStrLeft(productName, "(");
                                 productName = CGestIni::CutStrLeft(productName, "/");
                                 produits_Map[productName]=0;
                               }
                          }
                    } // if (!s.startsWith("sauf") && !line.startsWith("Voir ") && !s.startsWith("voir "))
               } // if ( pos != -1 )
          }  // if (s[0] == '+')
       ++it;
     }  // while (*it)

    //...................... CREATION DE LA TABLE PRODUITS D'INTERACTION ..............................
    if (dropTable(  "it_product")==0) return 0;
    if (createTable(" CREATE TABLE `it_product` ("
                 "`it_product_pk`          BIGINT NOT NULL AUTO_INCREMENT,"
                 "`it_product_owner`       CHAR(40)   ,"
                 "`it_product_libelle`     CHAR(255)  ,"
                 "PRIMARY KEY (`it_product_pk`)"
                 ")")==0) return 0;
    // .......... creer la liste des produits avec leur pk .................................
    it_initialiseProductList(produits_Map,owner);

    //...................... CREATION DES DEFINITIONS DES CLASSES D'INTERACTIONS ..............................
    if (dropTable(  "it_classe_def")==0) return 0;
    if (createTable(" CREATE TABLE `it_classe_def` ("
                    "`it_classe_def_pk`          BIGINT NOT NULL AUTO_INCREMENT,"
                    "`it_classe_def_owner`       CHAR(40)       ,"
                    "`it_classe_def_name`        CHAR(255)      ,"
                    "`it_classe_def_memo`        VARCHAR(4096)  ,"
                    "PRIMARY KEY (`it_classe_def_pk`)"
                    ")")==0) return 0;
    //...................... CREATION DES MEMBRES DE CLASSE D'INTERACTION ..............................
    if (dropTable(  "it_classe_mbr")==0) return 0;
    if (createTable(" CREATE TABLE `it_classe_mbr` ("
                    "`it_classe_mbr_pk`            BIGINT NOT NULL AUTO_INCREMENT,"
                    "`it_classe_mbr_owner`         CHAR(40)   ,"
                    "`it_classe_mbr_product_name`  CHAR(255)  ,"
                    "`it_classe_mbr_product_pk`    BIGINT NOT NULL  ,"
                    "`it_classe_mbr_classe_def_pk` BIGINT NOT NULL  ,"
                    "PRIMARY KEY (`it_classe_mbr_pk`)"
                    ")")==0) return 0;

    //...................... remplir les tables des definitions de classes ..............................
    //                       et creer les membres de chaque classe
    QString pkc = "";
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(pQTreeWidget->topLevelItemCount()/10));
    position = 0;
    it    = QTreeWidgetItemIterator(pQTreeWidget);
    plabelSynthese->setText(tr("Creation des classes d'interactions."));
    outMessage( tr("%1Creation des <font color=#ffd270><b>%2</b></font> en cours....").arg(OFSET,"classes d'interactions"));
    while (*it)
     { s = (*it)->text(0);
       if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
       ++ position;
       //....................... DEFINITION DE LA CLASSE ..........................................
       //                        ET DE SES MEMBRES
       if ( s[0] != '+' )                              // un nom de la classe ne commence pas par +
          { pkc = it_createClassDef((*it)->text(0).trimmed(), (*it)->text(1).trimmed(), owner);
            (*it)->setText(2, pkc );                   // on met le Pk en 3eme position
            //................. extraire la liste des produit de cette classe et ...........................
            //                  creer un membre de cette classe pour chacun de ses produits
            s   = (*it)->text(1).trimmed();                                  // on recupere le texte de la classe_it pour en extraire la liste des composants (entre parentheses)
            pos = posLastEnclosedSymbol(s,"(",")", &posFermant);
            if ( pos != -1 )
               {  if (posFermant != -1) s.truncate(posFermant);
                  ++pos;
                  line = s;
                  s = s.mid(pos);
                  s = s.trimmed();
                  if (!s.startsWith("sauf") && !line.startsWith("Voir ") && !s.startsWith("voir "))
                     { membreList = s.split(',');
                       for ( i=0; i<membreList.size(); ++i)
                           { productName = membreList[i].trimmed().toUpper().replace("\n"," ").simplified();
                             if (productName.startsWith("DI(ACEFYLLINE)"))  // di(acefylline) cas particulier pourri
                                { s = CGestIni::CutStrRight(productName, ")").trimmed();
                                  produits_Map[s]=0;
                                  productName = CGestIni::CutStrLeft(productName, ")").remove('(');
                                  it_createClassMember(pkc, productName, owner);
                                 }
                             else
                                 { productName = CGestIni::CutStrLeft(productName, "(");
                                   productName = CGestIni::CutStrLeft(productName, "/");
                                   it_createClassMember(pkc, productName, owner);
                                 }
                           }
                     } // if (!s.startsWith("sauf") && !line.startsWith("Voir ") && !s.startsWith("voir "))
               } // if ( pos != -1 )
          } // if ( s[0] != '+' )
       ++it;
     } //while (*it)

    //...................... creer les liens d'interaction ..............................
    //                       possible car toutes les classes avec les membres sont deja crees
    //...................... CREATION DES LIENS D'INTERACTION ..............................
    if (dropTable(  "it_classe_lnk")==0) return 0;
    if (createTable(" CREATE TABLE `it_classe_lnk` ("
                    "`it_classe_lnk_pk`            BIGINT NOT NULL AUTO_INCREMENT,"
                    "`it_classe_lnk_owner`         CHAR(40)          ,"
                    "`it_classe_lnk_mbr_pk1`       BIGINT NOT NULL   ,"
                    "`it_classe_lnk_mbr_pk2`       BIGINT NOT NULL   ,"
                    "`it_classe_lnk_pair_pk1Pk2`   CHAR(40)          ,"
                    "`it_classe_lnk_mecanism_mpk`  BIGINT NOT NULL   ,"
                    "`it_classe_lnk_level`         CHAR(128)         ,"
                    "`it_classe_lnk_todo_mpk`      BIGINT NOT NULL   ,"
                    "PRIMARY KEY (`it_classe_lnk_pk`)"
                    ")")==0) return 0;
    //...................... CREATION DE LA TABLE MEMOS D'INTERACTION ..............................
    if (dropTable(  "it_memo")==0) return 0;
    if (createTable(" CREATE TABLE `it_memo` ("
                 "`it_memo_pk`          BIGINT NOT NULL AUTO_INCREMENT,"
                 "`it_memo_owner`       CHAR(40)   ,"
                 "`it_memo_text`        VARCHAR(4096)  ,"
                 "PRIMARY KEY (`it_memo_pk`)"
                 ")")==0) return 0;
    pkc = "";
    if (m_pQProgressBar) m_pQProgressBar->setRange(0,(pQTreeWidget->topLevelItemCount()/10));
    position = 0;
    it    = QTreeWidgetItemIterator(pQTreeWidget);
    plabelSynthese->setText(tr("Creation des liens d'interactions."));
    outMessage( tr("%1Creation des <font color=#ffd270><b>%2</b></font> en cours....").arg(OFSET,"liens d'interactions"));
    while (*it)
     {
       if (m_pQProgressBar)  {m_pQProgressBar->setValue(position/10); qApp->processEvents();qApp->processEvents();}
       ++ position;
       //.......... faire la liste des membres de cette classe d'interaction ..........................................
       s   = (*it)->text(0);   // nom de la classe
       if ( s[0] != '+' )
          { pkc = (*it)->text(2);
          }
       else
          { QString className = s.mid(1).trimmed().toUpper();
            QString mecanism  =  (*it)->text(1);
            QString level     =  it_normalisedLevel((*it)->text(2));
            QString cat       =  (*it)->text(3);
            QString pkc2      =  isThisValueInTable("it_classe_def","it_classe_def_name", className);
            if (pkc2.length())
               { it_createClassLink(pkc, pkc2, mecanism, level,cat, owner);
               }
          }
       ++it;
     } //while (*it)
    //...................... CREATION DU STATUT ..............................
    if (dropTable(  "it_status")==0) return 0;
    if (createTable("CREATE TABLE `it_status` ("
                    "`it_status_pk`            BIGINT  NOT NULL AUTO_INCREMENT,"
                    "`it_status_owner`         VARCHAR(40)      ,"
                    "`it_status_version`       VARCHAR(80),"
                    "PRIMARY KEY (`it_status_pk`)"
                    ")"
                    )==0) return 0;
    it_setStatut( m_dateANSMInter, owner );
    plabelSynthese->setText(tr("nombre d'items : %1").arg(QString::number(i)));
    outMessage( tr("----------  Fin de la cr\303\251ation des tables d'interactions : <font color=#01eaff><b>%1</b></font> ----------------").arg( m_dateANSMInter ));
    return 1;
}
//--------------------------- it_createClassLink -----------------------------------------------------------
QString C_BDM_Manager::it_createClassLink(const QString &class_pk1, const QString &class_pk2, const QString &mecanism, const QString &level,const QString &cat, const QString &owner)
{
    /*
    "`it_classe_lnk_pk`            BIGINT NOT NULL AUTO_INCREMENT,"
    "`it_classe_lnk_owner`         CHAR(40)          ,"
    "`it_classe_lnk_mbr_pk1`       BIGINT NOT NULL   ,"
    "`it_classe_lnk_mbr_pk2`       BIGINT NOT NULL   ,"
    "`it_classe_lnk_pair_pk1Pk2`   CHAR(40)          ,"
    "`it_classe_lnk_mecanism_mpk`  BIGINT NOT NULL   ,"
    "`it_classe_lnk_level`         CHAR(1)           ,"
    "`it_classe_lnk_todo_mpk`      BIGINT NOT NULL   ,"
    */
    QString pk_mecanism = it_createMemo(mecanism , owner);
    QString pk_cat      = it_createMemo(cat ,      owner);
    QString prepare     = QLatin1String("INSERT INTO it_classe_lnk (`it_classe_lnk_owner`,`it_classe_lnk_mbr_pk1`,`it_classe_lnk_mbr_pk2`,`it_classe_lnk_pair_pk1Pk2`,`it_classe_lnk_mecanism_mpk`,`it_classe_lnk_level`,`it_classe_lnk_todo_mpk`) "
                                        "VALUES (?,?,?,?,?,?,?)");
    if (!database().isOpen() && database().open()== FALSE) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, class_pk1);
    query.bindValue(2, class_pk2);
    query.bindValue(3, class_pk1+"|"+class_pk2);
    query.bindValue(4, pk_mecanism);
    query.bindValue(5, level);
    query.bindValue(6, pk_cat);
    if ( !query.exec())
       { outSQL_error( query, "ERREUR  : C_BaseMedica::it_createClassLink()", prepare, __FILE__, __LINE__);
         return QString::null;
       }
    return "1";
}
//--------------------------- it_createClassMember -----------------------------------------------------------
QString C_BDM_Manager::it_createClassMember(const QString &class_pk, const QString &product_name,const QString &owner)
{
    QString pk_product = isThisValueInTable("it_product","it_product_libelle", product_name);
    if (pk_product.length()==0)
       { outMessage( tr("ERREUR  : C_BaseMedica::it_createClassMember() don't retrieve PK for this product '%1'").arg(product_name) );
         return QString::null;
       }

    QString prepare  = QLatin1String("INSERT INTO it_classe_mbr (`it_classe_mbr_owner`,`it_classe_mbr_product_name`,`it_classe_mbr_product_pk`,`it_classe_mbr_classe_def_pk`) "
                                     "VALUES (?,?,?,?)");
    if (!database().isOpen() && database().open()== FALSE) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, product_name);
    query.bindValue(2, pk_product);
    query.bindValue(3, class_pk);
    if ( !query.exec())
       { outSQL_error( query, "ERREUR  : C_BaseMedica::it_createClassMember()", prepare, __FILE__, __LINE__);
         return QString::null;
       }
    return "1";
}

//--------------------------- it_createClassDef -----------------------------------------------------------
QString C_BDM_Manager::it_createClassDef(const QString &class_name, const QString &memo,const QString &owner)
{

    QString prepare  = QLatin1String("INSERT INTO it_classe_def (`it_classe_def_owner`,`it_classe_def_name`,`it_classe_def_memo`) "
                                     "VALUES (?,?,?)");
    if (!database().isOpen() && database().open()== FALSE) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, class_name);
    query.bindValue(2, memo);
    if ( !query.exec())
       { outSQL_error( query, "ERREUR  : C_BaseMedica::it_createClassDef()", prepare, __FILE__, __LINE__);
         return QString::null;
       }
    return getLastPrimaryKey( "it_classe_def", "it_classe_def_pk");
}
//--------------------------- it_createMemo -----------------------------------------------------------
QString C_BDM_Manager::it_createMemo(const QString &_memo,const QString &owner)
{
    QString memo = _memo;
    memo = memo.replace("\"","\\\"").replace("(","\\(").replace(")","\\)");
    QString pk = isThisValueInTable("it_memo","it_memo_text", memo);
    if (pk.length())
       { C_Utils_Log::outMessage( 0, tr("WARNING  : C_BDM_Manager::it_createMemo() : not added because it is already in '%2'").arg("it_memo").toLatin1());
         //...................... ^ en mettant zero on empeche la sortie dans le widget de log et elle se fait juste dans la console d'erreurs normale
         return pk;      // si deja present cassos
       }
    QString prepare  = QString("INSERT INTO it_memo (`it_memo_owner`,`it_memo_text`) "
                               "VALUES (?,?)");
    if (!database().isOpen() && database().open()== FALSE) return QString::null;
    QSqlQuery query(QString::null, database());
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, memo);
    if ( !query.exec() )
       { outSQL_error( query, "ERREUR  : C_BaseMedica::it_createMemo()", prepare, __FILE__, __LINE__);
         return QString::null;
       }
    return getLastPrimaryKey( "it_memo", "it_memo_pk");
}
//--------------------------- it_initialiseProductList -----------------------------------------------------------
int C_BDM_Manager::it_initialiseProductList(const QMap <QString,int> &produits_Map,const QString &owner)
{ int i            = 0;
  QString prepare  = QString(" INSERT INTO it_product (`it_product_owner`,`it_product_libelle`)"
                             " VALUES (?,?)");
  QSqlQuery query(QString::null , database() );
  QMapIterator<QString, int> it(produits_Map);
  while (it.hasNext())
        {  it.next();
           query.prepare(prepare);
           query.bindValue(0, owner);
           query.bindValue(1, it.key());
           if ( !query.exec())
              { outSQL_error(query, "ERREUR  : C_BDM_Manager::it_initialiseProductList()", prepare, __FILE__, __LINE__);
              }
           else
              { ++i;
              }
        }
  return i;
}

//--------------------------- it_normalisedLevel -----------------------------------------------------------
QString C_BDM_Manager::it_normalisedLevel(const QString   &exoticLevel)
{
    /*
       return "0";         // effet indesirable
       return "1";         // precaution d'emploi
       return "2";         // mise en garde
       return "3";         // Contre-indication relative (deconseillee)
       return "4";         // Contre-indication absolue
     */
    if      (exoticLevel == "Association DECONSEILLEE")  return "3";
    else if (exoticLevel.contains("caution d'emploi"))   return "1";
    else if (exoticLevel == "A prendre en compte")       return "2";
    else if (exoticLevel == "CI - ASDEC - APEC")         return "4";
    else if (exoticLevel == "ASDEC - APEC")              return "3";
    else if (exoticLevel == "ASDEC - PE")                return "3";
    else if (exoticLevel == "CI - PE")                   return "4";
    else if (exoticLevel == "CI - ASDEC")                return "4";
    else if (exoticLevel == "CONTRE-INDICATION")         return "4";
    else if (exoticLevel == "CI - ASDEC - PE")           return "3";
    else                                                 return "4";

}

//--------------------------- it_setStatut -----------------------------------------------------------
QString C_BDM_Manager::it_setStatut(const QString &statut, const QString   &owner)
{   if (!database().isOpen() && database().open()== FALSE) return "";
    //............... on a pas trouve on cree ............................
    QString prepare  = QLatin1String(" INSERT INTO it_status (`it_status_owner`, `it_status_version`)"
                                     " VALUES (?,?)");
    QSqlQuery query(QString::null , database() );
    query.prepare(prepare);
    query.bindValue(0, owner);
    query.bindValue(1, statut);
    if ( !query.exec())
       { outSQL_error(query, "ERREUR  : C_BDM_Manager::it_setStatut()", prepare, __FILE__, __LINE__);
         return QString::null;
       }
    return "";
}

