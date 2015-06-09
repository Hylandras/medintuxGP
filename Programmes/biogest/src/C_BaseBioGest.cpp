#include "C_BaseBioGest.h"
#include <QDomElement>
#include <QDomNode>
#include <QSqlQuery>
#include <QSqlQuery>
#include <QVariant>
#include <QTreeWidget>
#include <QSqlError>
#include <QMap>

#include "C_DSVar.h"
#include "CApp.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#define TEST_SEP      1
#define TEST_NAME     5
#define TEST_RESULT   7
#define TEST_UNIT     8
#define TEST_MINMAX   9

//================================================ CExpediteur ==============================================
//------------------------------------------ CExpediteur -----------------------------------
CExpediteur::CExpediteur()
     {   m_nomOriginal      = "";
         m_codeExpediteur   = "";
         m_nomExpediteur    = "";
         m_codeDestinataire = "";
         m_nomDestinataire  = "";
         m_dateEmission     = "";
         m_heureEmission    = "";
     }
//------------------------------------------ CExpediteur -----------------------------------
CExpediteur::CExpediteur(const QString &hprim_line, QString &separator)
     {setFromHprimLine(hprim_line, separator);
     }

//------------------------------------------ setFromHprimLine -----------------------------------
//H|\^&~|CLIR949.hpr||CASA\LABORATOIRE||ORU|||M070780168\SA CLINIQUE DE MARIGNANE||P|H2.0\:C|201204042104
void CExpediteur::setFromHprimLine(const QString &hprim_line, QString &separator)
{  QStringList list                         = hprim_line.split( separator[0] , QString::KeepEmptyParts );
   if (list.size()>2)    m_nomOriginal      = list[2];
   QStringList section;
   if (list.size()>4)    section            = list[4].split( separator[1] , QString::KeepEmptyParts );
   if (section.size()>0) m_codeExpediteur   = section[0];
   if (section.size()>1) m_nomExpediteur    = section[1];
   if (list.size()>9)    section            = list[9].split( separator[1] , QString::KeepEmptyParts );
   if (section.size()>0) m_codeDestinataire = section[0];
   if (section.size()>1) m_nomDestinataire  = section[1];
   if (list.size()>13)   m_dateEmission     = list[13].left(8);
   if (list.size()>13)   m_heureEmission    = list[13].right(4);
}


//================================================ CPatient ======================================================================
//-------------------------------- CPatient -----------------------------------
CPatient::CPatient()
{  m_nom                   = "";
   m_prenom                = "";
   m_dateNaissance         = "";
   m_sexe                  = "";
   m_OBR_Ident             = "";
   m_Service               = "";
   m_Separator             = "";
   m_dateExam              = "";
   m_heureExam             = "";
   m_pkRecord              = "";
   m_fileNameRef           = "";
   m_nbOBX                 = "";
}
CPatient::CPatient(const QString& nom,
                   const QString& prenom,
                   const QString& dateNss,
                   const QString& sexe,
                   const QString& obrIdent,
                   const QString& service,
                   const QString& separator,
                   const QString& dateExam,
                   const QString& heureExam,
                   const QString& pk             /* ="" */,
                   const QString& nbOBX          /* ="" */,
                   const QString& fileNameRef    /* ="" */
                  )
{m_nom                   = nom;
 m_prenom                = prenom;
 m_dateNaissance         = dateNss;
 m_sexe                  = sexe;
 m_OBR_Ident             = obrIdent;
 m_Service               = service;
 m_Separator             = separator;
 m_dateExam              = dateExam;
 m_heureExam             = heureExam;
 m_pkRecord              = pk;
 m_nbOBX                 = nbOBX;
 m_fileNameRef           = fileNameRef;

}

//-------------------------------- CPatient -----------------------------------
CPatient::CPatient(const QString& hprim_line, const QString &separator)
     {setFromHprimLine(hprim_line, separator);
     }
//-------------------------------- setFromHprimLine -----------------------------------
// P|0001|240932||12012010|BRAHMIA\RAFIK||19721129|M||2 RUE BIS DE L EPIS       *\\AVIGNON\\84000|
void CPatient::setFromHprimLine( const QString& hprim_line, const QString &separator)
{  QStringList list                      = hprim_line.split( separator[0] , QString::KeepEmptyParts );
   QStringList section;
   if (list.size()>5)    section         = list[5].split(    separator[1] , QString::KeepEmptyParts );
   if (section.size()>0) m_nom           = section[0];
   if (section.size()>1) m_prenom        = section[1];
   if (list.size()>7)    m_dateNaissance = list[7];
   if (list.size()>8)    m_sexe          = list[8];
   m_Service                             = "";
   m_Separator                           = separator;
}
//-------------------------------- getOBRId -----------------------------------
QString CPatient::getOBRId()
{return m_OBR_Ident;
}
//-------------------------------- setOBRId -----------------------------------
void CPatient::setOBRId( const QString &ident)
{m_OBR_Ident = ident;
 m_OBR_Ident = m_OBR_Ident.remove('\\');
}

//-------------------------------- getPkRecord -----------------------------------
QString CPatient::getPkRecord(){return m_pkRecord;}
//-------------------------------- setPkRecord -----------------------------------
void CPatient::setPkRecord(const QString &value) {if (value.length()) m_pkRecord=value;}
//-------------------------------- getDateExam -----------------------------------
QString CPatient::getDateExam(){return m_dateExam;}
//-------------------------------- setDateExam -----------------------------------
void CPatient::setDateExam(const QString &value) {if (value.length()) m_dateExam=value;}
//-------------------------------- getHeureExam -----------------------------------
QString CPatient::getHeureExam(){return m_heureExam;}
//-------------------------------- setHeureExam -----------------------------------
void CPatient::setHeureExam(const QString &value){if (value.length()) m_heureExam=value;}
//-------------------------------- getFileRef -----------------------------------
QString CPatient::getFileRef(){return m_fileNameRef;}
//-------------------------------- setFileRef -----------------------------------
void CPatient::setFileRef(const QString &value){if (value.length())   m_fileNameRef=value;}
//-------------------------------- getNbObx -----------------------------------
QString CPatient::getNbObx(){return m_nbOBX;}
//-------------------------------- setNbObx -----------------------------------
void CPatient::setNbObx(const QString &value){if (value.length())     m_nbOBX=value;}

//====================================== C_BaseBioGest ===============================================================================
//--------------------------- C_BaseBioGest ---------------------------------------------------------
C_BaseBioGest::C_BaseBioGest(const QString &param , QString& confData, QObject *parent)
    : C_BaseCommon(parent) , C_DSVar( confData)
{if (!C_DSVar::areFieldsNamesValids())  C_BaseBioGest::setLastError(C_DSVar::getLastError());
 reConnect(param);
 m_SepVal        = 'a';
 m_HprimCodeList = setCodeHprimListFromTemplate( CApp::pCApp()->getTemplate() );  // relever les codes hprim utilisee parr le template
 CApp::pCApp()->outMessage(tr("C_BaseBioGest::C_BaseBioGest() number of Hprim codes found :%1").arg(QString::number(m_HprimCodeList.size())));
 CApp::pCApp()->outMessage(tr("C_BaseBioGest::C_BaseBioGest() number of Hprim codes\n%1").arg(m_HprimCodeList.join ( "-" ) ));
}
//--------------------------- getVersionConfig -----------------------------------------------------------
QString   C_BaseBioGest::getVersionConfig()
{return m_VERSION;
}
//--------------------------- reConnect -----------------------------------------------------------
bool       C_BaseBioGest::reConnect(const QString &param)
{bool ok = BaseConnect(param,"C_BaseBioGest_LabelBase", m_SCHEMA_NAME);
 if (!ok)    C_BaseBioGest::setLastError(C_BaseCommon::getLastError());
 return ok;
}

//--------------------------- setLastError -----------------------------------------------------------
void       C_BaseBioGest::setLastError(const QString &error){ m_LastError = error;}

//--------------------------- getLastError -----------------------------------------------------------
QString    C_BaseBioGest::getLastError(){return m_LastError;}
//--------------------------- fillPatientList -----------------------------------------------------------
/*                  OBSOLETE LA FONCTION UTILISEE EST DESORMAIS fill_CPatientMap()
int    C_BaseBioGest::fillPatientList(QTreeWidget* pQTreeWidget,
                                      const QString &debut ,
                                      const QString &fin ,
                                      const QString &nom,
                                      const QString &_service )
{   pQTreeWidget->clear();
    int nb                  = 0;
    bool whereMustBePresent = true;
    QString         service = _service;
    //........... ne garder que l'utile ..................
    //            ce qu'il y a apres le :
    if ( service.length())
       { service     = _service;
         int     pos = service.lastIndexOf(':');
         if (pos !=-1) service= service.mid(pos+1).trimmed();
       }
    //................. filtrer selon donnée d'entrée ..................................................
    //                  peut être limiter la requete ???
    QString requete = QString("SELECT  ")                        +
                                  m_LABO_PATIENTS_NOM            +","+   // %1 --> 0
                                  m_LABO_PATIENTS_PRENOM         +","+   // %2 --> 1
                                  m_LABO_PATIENTS_DATE_EXAMEN    +","+   // %3 --> 2
                                  m_LABO_PATIENTS_HEURE_EXAMEN   +","+   // %4 --> 3
                                  m_LABO_PATIENTS_DATENSS        +","+   // %5 --> 4
                                  m_LABO_PATIENTS_SEXE           +","+   // %6 --> 5
                                  m_LABO_PATIENTS_NB_OBX         +","+   // %7 --> 6
                                  m_LABO_PATIENTS_PK             +","+   // %8 --> 7
                                  m_LABO_PATIENTS_SERVICE        +" "    // %9 --> 8
                        " FROM "+ m_LABO_PATIENTS_TBL_NAME ;
    if ( nom != "" )
       { requete            += " WHERE nom LIKE '" + nom + "%'";
         whereMustBePresent  = false;
       }
    if (CApp::pCApp()->isDateFilterEnabled())
       { if (debut != "" && fin != "")
            {   requete += QString(" %1 dateExamen >= '%2' AND dateExamen <= '%3'").arg(whereMustBePresent?" WHERE ":" AND ",debut,fin);
                whereMustBePresent  = false;
            }
          else if (debut != "")
            {   requete += QString(" %1  dateExamen = '%2'").arg(whereMustBePresent?" WHERE ":" AND ",debut);
                whereMustBePresent  = false;
            }
       }
    if (service.length() && service != tr("___"))
       { requete += QString(" %1  service = '%2'").arg(whereMustBePresent?" WHERE ":" AND ",service);
         whereMustBePresent  = false;
       }
    QSqlQuery query(requete , database() );
    //.................. Recuperer la liste des patients ..............................................
    if ( query.isActive() )
       {while (query.next())
              {const QString pk  = query.value(7).toString();
               int nbr           = getNbResultForThisPatient(pk, m_HprimCodeList);
               if (CApp::pCApp()->justPatientsWithResults() && nbr==0)         continue;
               QTreeWidgetItem* pQTreeWidgetItem = new QTreeWidgetItem(pQTreeWidget);
               if (pQTreeWidgetItem)
                  {
                   pQTreeWidgetItem->setText(0,query.value(0).toString());     // nom
                   pQTreeWidgetItem->setText(1,query.value(1).toString());     // prenom
                   pQTreeWidgetItem->setText(2,query.value(2).toString());     // date
                   pQTreeWidgetItem->setText(3,query.value(3).toString());     // heure
                   pQTreeWidgetItem->setText(4,query.value(4).toString());     // Dt Naiss
                   pQTreeWidgetItem->setText(5,query.value(5).toString());     // Sexe
                   pQTreeWidgetItem->setText(6,QString::number(nbr));          // nb OBX
                   pQTreeWidgetItem->setText(7,pk);                            // ID
                   pQTreeWidgetItem->setText(8,query.value(8).toString());     // service
                   ++nb;
                  }

               //if (nb){QListViewItem *pQListViewItem = new QListViewItem (listview,
               //                                                           query.value(1).toString(),  // nom          0
               //                                                           query.value(2).toString(),  // prenom       1
               //                                                           query.value(3).toString(),  // date         2
               //                                                           query.value(4).toString(),  // heure        3
               //                                                           pk,                         // ID           4
               //                                                           QString::number(nb));       // nb           5
               //        if (pQListViewItem) pQListViewItem->setPixmap ( 0, QPixmap (LED_grise_xpm) );
               //       }
              }
        }
    else
        {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::fillPatientList() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                   __FILE__, __LINE__  );
        }
    return nb;
}
*/

//--------------------------- fill_CPatientMap -----------------------------------------------------------
int    C_BaseBioGest::fill_CPatientMap(QMap<QString, CPatient> &patientPk_CPatient_Map,
                                       const QString &debut ,
                                       const QString &fin ,
                                       const QString &nom,
                                       const QString &_service )
{   patientPk_CPatient_Map.clear();;
    bool whereMustBePresent = true;
    QString         service = _service;
    //........... ne garder que l'utile ..................
    //            ce qu'il y a apres le :
    if ( service.length())
       { service     = _service;
         int     pos = service.lastIndexOf(':');
         if (pos !=-1) service= service.mid(pos+1).trimmed();
       }
    //................. filtrer selon donnée d'entrée ..................................................
    //                  peut être limiter la requete ???
    QString requete = QString("SELECT  ")                        +
                                  m_LABO_PATIENTS_NOM            +","+   // %1  --> 0
                                  m_LABO_PATIENTS_PRENOM         +","+   // %2  --> 1
                                  m_LABO_PATIENTS_DATE_EXAMEN    +","+   // %3  --> 2
                                  m_LABO_PATIENTS_HEURE_EXAMEN   +","+   // %4  --> 3
                                  m_LABO_PATIENTS_DATENSS        +","+   // %5  --> 4
                                  m_LABO_PATIENTS_SEXE           +","+   // %6  --> 5
                                  m_LABO_PATIENTS_NB_OBX         +","+   // %7  --> 6
                                  m_LABO_PATIENTS_PK             +","+   // %8  --> 7
                                  m_LABO_PATIENTS_SERVICE        +","+   // %9  --> 8
                                  m_LABO_PATIENTS_DEF_EXAMEN     +","+   // %10 --> 9
                                  m_LABO_PATIENTS_HPRIMSEP       +","+   // %11 --> 10
                                  m_LABO_PATIENTS_FILENAME_REF   +" "    // %12 --> 11
                        " FROM "+ m_LABO_PATIENTS_TBL_NAME ;
    if ( nom != "" )
       { requete            += " WHERE nom LIKE '" + nom + "%'";
         whereMustBePresent  = false;
       }
    if (CApp::pCApp()->isDateFilterEnabled())
       { if (debut != "" && fin != "")
            {   requete += QString(" %1 dateExamen >= '%2' AND dateExamen <= '%3'").arg(whereMustBePresent?" WHERE ":" AND ",debut,fin);
                whereMustBePresent  = false;
            }
          else if (debut != "")
            {   requete += QString(" %1  dateExamen = '%2'").arg(whereMustBePresent?" WHERE ":" AND ",debut);
                whereMustBePresent  = false;
            }
       }
    if (service.length() && service != tr("___"))
       { requete += QString(" %1  service = '%2'").arg(whereMustBePresent?" WHERE ":" AND ",service);
         whereMustBePresent  = false;
       }
    //..................... limiter eventuellement la liste ..........................................
    QString limit = CApp::pCApp()->getResultsLimit();
    if (limit.length() && limit.toInt()>0) requete += " LIMIT "+ limit;
    QSqlQuery query(requete , database() );
    //.................. Recuperer la liste des patients ..............................................
    if ( query.isActive() )
       {while (query.next())
              {const QString pk  = query.value(7).toString();
               int nbr           = getNbResultForThisPatient(pk, m_HprimCodeList);
               if (CApp::pCApp()->justPatientsWithResults() && nbr==0)         continue;
               patientPk_CPatient_Map.insert(pk, CPatient( query.value(0).toString(),  // nom
                                                           query.value(1).toString(),  // prenom
                                                           query.value(4).toString(),  // dt Nss
                                                           query.value(5).toString(),  // sexe
                                                           query.value(9).toString(),  // obr ident
                                                           query.value(8).toString(),  // service
                                                           query.value(10).toString(), // separator
                                                           query.value(2).toString(),  // date
                                                           query.value(3).toString(),  // heure
                                                           query.value(7).toString(),  // pk
                                                           QString::number(nbr),       // nb obx valides
                                                           query.value(11).toString()  // filenameRef
                                                         )
                                            );

              }
        }
    else
        {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::fill_CPatientMap() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                   __FILE__, __LINE__  );
        }
    return patientPk_CPatient_Map.size();
}
//------------------------------------ getExamensListFromPatient() --------------------------------------------
// QString  m_LABO_EXAMENS_TBL_NAME;               // nom  de la table des resultats hprim
// QString  m_LABO_EXAMENS_REF_PK;                 // reference sur le patient a qui appartient ce resultat
// QString  m_LABO_EXAMENS_PK;                     // clef primaire
// QString  m_LABO_EXAMENS_OBX_HPRIM;              // donnees hprim brutes
int C_BaseBioGest::getExamensListFromPatient (const QString &id_patient , QStringList &examList)
{   int nb          = 0;
    //................. filtrer selon donnée d'entrée ..................................................
    QString requete = QString("SELECT %1 FROM %2 WHERE %3 = '%4'" )
                              .arg(m_LABO_EXAMENS_OBX_HPRIM,      // %1  --> 0
                                   m_LABO_EXAMENS_TBL_NAME,       // %2
                                   m_LABO_EXAMENS_REF_PK,         // %3
                                   id_patient);                   // %4
    QSqlQuery query (requete , database() );
    //.................. si la requète a un resultat ..............................................
    if (query.isActive())
       {
        examList.clear();
        while (query.next())
           { examList.append(query.value(0).toString());
             ++nb;
           }
       }
    else
        {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::getExamensListFromPatient() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                   __FILE__, __LINE__  );
        }
    return nb;
}

//------------------------------------ getNbResultForThisPatient() --------------------------------------------
int C_BaseBioGest::getNbResultForThisPatient (const QString &pk_patient ,  QStringList listResultToCheck)
{   int nb = 0;
    //................. filtrer selon donnée d'entrée ..................................................
    //                  peut être limiter la requete ???
    QString requete= QString("SELECT %1 FROM %2 WHERE %3 = '%4'")
                             .arg(m_LABO_EXAMENS_OBX_HPRIM, m_LABO_EXAMENS_TBL_NAME, m_LABO_EXAMENS_REF_PK, pk_patient );

    QSqlQuery query (requete , database() );
    //.................. si la requète a un resultat ..............................................
    if (query.isActive())
       {
        while (query.next())
              {QString segment   =  query.value(0).toString();
               QString test_name = Get_Result_testType(segment, TEST_NAME);
               int        pos    = test_name.indexOf(segment[1]);
               if ( pos != -1 ) {test_name.truncate(pos);}
               int ut = listResultToCheck.indexOf ( test_name );
               if ( ut != -1 )
                  { ++nb;
                    listResultToCheck.removeAt( ut );
                  }
              }
       }
    else
        {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::getNbResultForThisPatient() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                   __FILE__, __LINE__  );
        }
    return nb;
}

//----------------------------------------------------- setCodeHprimListFromTemplate ----------------------------------------------------------
// parse le fichier du modele et en retire la liste de tous les codes Hprim s'y trouvant
QStringList C_BaseBioGest::setCodeHprimListFromTemplate( const QString &modele)
{
     int  pos            = 0;
     int  end            = 0;
     QString testString  = "";
     QStringList testListMask;
     //................ parser le masque d'afichage ...............................
     //                 pour recuperer liste des tests
     //                 le concernant
     while ( ( pos         = modele.indexOf("$CODES_GROUP=", pos)) != -1 )
           {   end         = GotoEndOfLigne(modele, pos);
               pos        += 13;
               testString += modele.mid(pos,end-pos).trimmed()+"|";
           }
     //...................... verifier si la liste du patient....................................
     //                       contient des tests demandés par
     //                       le masque d'affichage
     testListMask = testString.split('|');
     testListMask.sort();
     return testListMask;
}

//----------------------------------------------------- GetMinMaxValue ----------------------------------------------------------
// QString  m_LABO_MINMAX_TBL_NAME;           // nom de la table des minimum et maximums
// QString  m_LABO_MINMAX_PK;                 // clef primaire
// QString  m_LABO_MINMAX_EXAMEN;             // examen lie a ces valeurs min max
// QString  m_LABO_MINMAX_SEXE;               // sexe lie a ce maximum minimum
// QString  m_LABO_MINMAX_MAX;                // valeur maximum
// QString  m_LABO_MINMAX_MIN;                // valeur minimum

int  C_BaseBioGest::GetMinMaxValue(const QString &test_name, const QString &sexe, QString &test_valMin, QString &test_valMax)
{   QString requete= QString("SELECT %1, %2 FROM %3 WHERE %4 = '%5' AND %6 = '%7'" )
                            .arg(m_LABO_MINMAX_MIN,      // %1 ---> 0
                                 m_LABO_MINMAX_MAX,      // %2 ---> 1
                                 m_LABO_MINMAX_TBL_NAME, // %3
                                 m_LABO_MINMAX_EXAMEN,   // %4
                                 test_name,              // %5
                                 m_LABO_MINMAX_SEXE,     // %6
                                 sexe);                  // %7
    QSqlQuery query (requete, database() );
    //.................. si la requète a un resultat ..............................................
    if (query.isActive() && query.next())
       {test_valMin = query.value(0).toString();
        test_valMax = query.value(1).toString();
        return 1;
       }
    return 0;
}

//----------------------------------------------------- GetPatientByPrimKey ----------------------------------------------------------
/*
QString  m_LABO_PATIENTS_TBL_NAME;         // nom de la table des patients
QString  m_LABO_PATIENTS_PK;               // clef primaire
QString  m_LABO_PATIENTS_SERVICE;          // service auquel appartient ce patient
QString  m_LABO_PATIENTS_NB_OBX;           // nombre d'OBX (examens)
QString  m_LABO_PATIENTS_NOM_EXPED;        // nom de l'expediteur
QString  m_LABO_PATIENTS_CODE_EXPED;       // code expediteur
QString  m_LABO_PATIENTS_NOM_DESTI;        // nom du destinataire                                                              Secabilite
QString  m_LABO_PATIENTS_CODE_DESTI;       // code destinataire
QString  m_LABO_PATIENTS_DATE_EMISSION;    // date emission
QString  m_LABO_PATIENTS_HEURE_EMISSION;   // heure emission
QString  m_LABO_PATIENTS_NOM;              // nom patient
QString  m_LABO_PATIENTS_PRENOM;           // prenom patient
QString  m_LABO_PATIENTS_DATENSS;          // date naissance
QString  m_LABO_PATIENTS_SEXE;             // sexe
QString  m_LABO_PATIENTS_DATE_EXAMEN;      // date examen
QString  m_LABO_PATIENTS_HEURE_EXAMEN;     // heure examen
QString  m_LABO_PATIENTS_HPRIMSEP;         // chaine des separateurs HPRIM
QString  m_LABO_PATIENTS_DEF_EXAMEN;       // definition des examens
*/
int  C_BaseBioGest::GetPatientByPrimKey(const QString &primKey, CPatient &patient, CExpediteur &expediteur)
{
    QString requete = QString("SELECT ")               +
                      m_LABO_PATIENTS_NOM              + "," +   // 0
                      m_LABO_PATIENTS_PRENOM           + "," +   // 1
                      m_LABO_PATIENTS_DATENSS          + "," +   // 2
                      m_LABO_PATIENTS_SEXE             + "," +   // 3
                      m_LABO_PATIENTS_DATE_EXAMEN      + "," +   // 4
                      m_LABO_PATIENTS_HEURE_EXAMEN     + "," +   // 5
                      m_LABO_PATIENTS_DEF_EXAMEN       + "," +   // 6
                      m_LABO_PATIENTS_HPRIMSEP         + "," +   // 7
                      m_LABO_PATIENTS_SERVICE          + "," +   // 8
                      m_LABO_PATIENTS_CODE_EXPED       + "," +   // 9
                      m_LABO_PATIENTS_NOM_EXPED        + "," +   // 10
                      m_LABO_PATIENTS_CODE_DESTI       + "," +   // 11
                      m_LABO_PATIENTS_NOM_DESTI        + "," +   // 12
                      m_LABO_PATIENTS_DATE_EMISSION    + "," +   // 13
                      m_LABO_PATIENTS_HEURE_EMISSION   + "," +   // 14
                      m_LABO_PATIENTS_FILENAME_REF     + " " +   // 15
                      "FROM " + m_LABO_PATIENTS_TBL_NAME + " WHERE " + m_LABO_PATIENTS_PK + "='" + primKey + "'";
    QSqlQuery query (requete, database() );
    //.................. si la requète a un resultat ..............................................
    if ( !query.isActive() )
       { CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::GetPatientByPrimKey() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                           __FILE__, __LINE__  );
         return 0;
       }
    if (query.next())
       {patient.m_nom           = query.value(0).toString();
        patient.m_prenom        = query.value(1).toString();
        patient.m_dateNaissance = query.value(2).toString();
        patient.m_sexe          = query.value(3).toString();
        //patient.m_dateExamen    = query.value(4).toString();
        //patient.m_heureExamen   = query.value(5).toString();
        patient.setOBRId(      query.value(6).toString() );
        patient.m_Separator     = query.value(7).toString();
        patient.m_Service       = query.value(8).toString();
        //expediteur.m_nomOriginal      = query.value(7).toString();
        expediteur.m_codeExpediteur   = query.value(9).toString();
        expediteur.m_nomExpediteur    = query.value(10).toString();
        expediteur.m_codeDestinataire = query.value(11).toString();
        expediteur.m_nomDestinataire  = query.value(12).toString();
        expediteur.m_dateEmission     = query.value(13).toString();
        expediteur.m_heureEmission    = query.value(14).toString();
        patient.setFileRef(             query.value(15).toString());
        return 1;
       }
    return 0;
}
//-----------------------------------------------------  CreateNewPatient -------------------------------------------
QString C_BaseBioGest::CreateNewPatient(CPatient &patient, CExpediteur &expediteur)
{   QString ret      = "";
    QString prepare  = "";
    prepare         += "INSERT INTO "                 + m_LABO_PATIENTS_TBL_NAME  + " ( \n";
    prepare         += m_LABO_PATIENTS_HPRIMSEP       + " ,\n";         // 0
    prepare         += m_LABO_PATIENTS_NOM            + " ,\n";         // 1
    prepare         += m_LABO_PATIENTS_PRENOM         + " ,\n";         // 2
    prepare         += m_LABO_PATIENTS_DATENSS        + " ,\n";         // 3
    prepare         += m_LABO_PATIENTS_SEXE           + " ,\n";         // 4
    prepare         += m_LABO_PATIENTS_DATE_EXAMEN    + " ,\n";         // 5
    prepare         += m_LABO_PATIENTS_HEURE_EXAMEN   + " ,\n";         // 6
    prepare         += m_LABO_PATIENTS_DEF_EXAMEN     + " ,\n";         // 7
    prepare         += m_LABO_PATIENTS_SERVICE        + " ,\n";         // 8
    prepare         += m_LABO_PATIENTS_CODE_EXPED     + " ,\n";         // 9
    prepare         += m_LABO_PATIENTS_NOM_EXPED      + " ,\n";         // 10
    prepare         += m_LABO_PATIENTS_CODE_DESTI     + " ,\n";         // 11
    prepare         += m_LABO_PATIENTS_NOM_DESTI      + " ,\n";         // 12
    prepare         += m_LABO_PATIENTS_DATE_EMISSION  + " ,\n";         // 13
    prepare         += m_LABO_PATIENTS_HEURE_EMISSION + " ,\n";         // 14
    prepare         += m_LABO_PATIENTS_FILENAME_REF   + " )\n";         // 15
    prepare         += " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) ";

    QSqlQuery query(database());
    query.prepare(prepare);
    query.bindValue(0,  patient.m_Separator);                  // 0
    query.bindValue(1,  patient.m_nom);                        // 1
    query.bindValue(2,  patient.m_prenom);                     // 2
    query.bindValue(3,  patient.m_dateNaissance);              // 3
    query.bindValue(4,  patient.m_sexe);                       // 4
    query.bindValue(5,  expediteur.m_dateEmission);            // 5
    query.bindValue(6,  expediteur.m_heureEmission);           // 6
    query.bindValue(7,  patient.getOBRId());                   // 7
    query.bindValue(8,  patient.m_Service);                    // 8
    query.bindValue(9,  expediteur.m_codeExpediteur);          // 9
    query.bindValue(10, expediteur.m_nomExpediteur);           // 10
    query.bindValue(11, expediteur.m_codeDestinataire);        // 11
    query.bindValue(12, expediteur.m_nomDestinataire);         // 12
    query.bindValue(13, expediteur.m_dateEmission);            // 13
    query.bindValue(14, expediteur.m_heureEmission);           // 14
    query.bindValue(15, patient.getFileRef());                 // 15


    if ( !query.exec() )
       { CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::CreateNewPatient() \n%1\nREQUEST: %2").arg(query.lastError().text(), prepare).toAscii(),__FILE__, __LINE__  );
         return QString::null;
       }
    return getLastPrimaryKey( m_LABO_PATIENTS_TBL_NAME, m_LABO_PATIENTS_PK , patient.getOBRId() , m_LABO_PATIENTS_DEF_EXAMEN);
}
//--------------------------------- setService -------------------------------------------------------
int C_BaseBioGest::setService(const QString &pk, const QString &service)
{
 QString   requete  = QString ("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5' ").arg(m_LABO_PATIENTS_TBL_NAME, m_LABO_PATIENTS_SERVICE, service, m_LABO_PATIENTS_PK, pk);
 //.................. Executer la requete ...............................................
 QSqlQuery query ( database() );
 if ( !    query.exec( requete ) )
    { CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::setService() \n%1\nREQUEST: %2").arg(query.lastError().text(), requete).toAscii(),__FILE__, __LINE__  );
      return 0;
    }
 return 1;
}
//-----------------------------------------------------  IsThisPatientExist -------------------------------------------
QString C_BaseBioGest::IsThisPatientExist( CPatient &patient )
{
  QString ret("");
  //................. filtrer selon donnée d'entrée ..................................................
  QString requete = QString( "SELECT %1 FROM %2 WHERE %3 = '%4'" ).arg(m_LABO_PATIENTS_PK, m_LABO_PATIENTS_TBL_NAME, m_LABO_PATIENTS_DEF_EXAMEN, patient.getOBRId());
  QSqlQuery query(requete , database() );

  //.................. si la requète a un resultat ..............................................
  if ( query.isActive() )
     { if (query.next() ) ret= query.value(0).toString();
     }
  return ret;
}

//------------------------------------ recordHprimLineResult() --------------------------------------------
// ACTION: enregistre dans la table des résultats une ligne de resultat avec l'id_Patient auquel il
//         appartient
int C_BaseBioGest::recordHprimLineResult(const QString &separator, QString &segment_line, QString &id_Patient)
{     QString prepare  = "";
      prepare         += "INSERT INTO "                 + m_LABO_EXAMENS_TBL_NAME  + " ( \n";
      prepare         += m_LABO_EXAMENS_REF_PK          + " ,\n";         // 0
      prepare         += m_LABO_EXAMENS_OBX_HPRIM       + " )\n";         // 1
      prepare         += " VALUES (?,?) ";

      QSqlQuery query(database());
      query.prepare(prepare);
      query.bindValue(0, id_Patient);                              // 0
      query.bindValue(1, separator + separator[0] + segment_line); // 1

      if ( !query.exec() )
         { CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseBioGest::recordHprimLineResult() \n%1\nREQUEST: %2").arg(query.lastError().text(), prepare).toAscii(),__FILE__, __LINE__  );
           return 0;
         }
      return 1;
}

//------------------------------------ analyseFile() --------------------------------------------
int C_BaseBioGest::analyseFile(const QString &fileName)
{   int         line_number   =  0;
    int         pos           = -1;
    int         end           = -1;
    QString     next_line     = "";
    QString     segment_line  = "";
    QString     pk_Patient    = "";
    QString     segment_type  = "";
    QString     separator     = "";
    QString     modele        = CApp::pCApp()->getTemplate();
    QStringList servicesList  = CApp::pCApp()->getProfilsList("_Service");
    QStringList servicesDefs, servicesDefs_1;    // dans les resultats retournes par casalta deux type d'endroits sont possibles....
    CExpediteur expediteur;    // expediteur courant
    CPatient    patient;       // patient courant
    for (int i=0; i<servicesList.size(); ++i)
        {QString str        = servicesList[i];
         pos                = str.indexOf(':');
         if (pos != -1) str =  str.mid(pos+1);
         servicesList[i]    = str.trimmed();
        }
    //............... Pan A relever dans le modele les elements permettant de relever le  service ..........
    //                tres variable selon les labos
    pos = modele.indexOf("$FIELD_SERVICE=");    // pos utilisé provisoirement
    if (pos != -1) end =  modele.indexOf("$FIN_FIELD_SERVICE");
    if (end != -1)
       {pos += 15;
        segment_line = modele.mid(pos,end-pos);         //$FIELD_SERVICE=OBX;16;2$FIN_FIELD_SERVICE
        servicesDefs = segment_line.split('|');
       }
    //...............Plan B relever dans le modele les elements permettant de relever le  service ..........
    //                tres variable selon les labos meme prevoir un plan B si la premiere definition
    pos = modele.indexOf("$FIELD_SERVICE_1=");    // pos utilisé provisoirement
    if (pos != -1) end =  modele.indexOf("$FIN_FIELD_SERVICE_1");
    if (end != -1)
       {pos += 15;
        segment_line   = modele.mid(pos,end-pos);         //$FIELD_SERVICE=OBX;16;2$FIN_FIELD_SERVICE
        servicesDefs_1 = segment_line.split('|');
       }
    //............... relever les elements permettant de relever le  service ...................
    pos = modele.indexOf("$SEP_VAL=");
    if (pos != -1) m_SepVal = modele.mid(pos+9,1);
    //....................... on initialise le resultat = faux .............................
    int result       = C_BaseBioGest::Error /*0*/;
    //........................on créé un fichier virtuel du fichier ......................
    QFile file( fileName );
    //........................on teste la taille du fichier : ...............................
    //                        si elle est égale à zéro on retourne sans analyser
    if (file.size() == 0) return C_BaseBioGest::Useless/*1*/;
    // on ouvre le fichier en lecture
    QString baseName = QFileInfo(fileName).completeBaseName();
    patient.setFileRef(baseName);
    if ( file.open( QIODevice::ReadOnly ) )
       {//......... on crée un stream sur le fichier .....................................
        QTextStream stream ( &file );
        stream.setCodec("ISO 8859-1");
        //.......... on déclare une variable pour le lire ligne à ligne ..................
        //.............. tant que la fin du fichier n'est pas atteinte ...................
        // enum State { Error , Useless , IsPatient, IsExpediteur, IsResultData, IsResultHeader, IsEndOfFile };
        if (!stream.atEnd())  segment_line    = stream.readLine().trimmed();
        line_number = 1;
        while (!stream.atEnd())
           {//...........on lit une ligne ...............................................................
            next_line    = stream.readLine().trimmed();

            if (segment_line.length())  // peut y en avoir des vides !!!
               {
                //........... entete du fichier on en extrait: .............................................
                //            l'expéditeur en cours, les separateurs
                //      H|~^\&|URG062.HPR||133002782~CASALTA||ORU|||MARURG~Marignane Urgence||P|H2.0|200410291528
                //      H|\^&~|CLIR949.hpr||CASA\LABORATOIRE||ORU|||M070780168\SA CLINIQUE DE MARIGNANE||P|H2.0\:C|201204042104
                //
                if (line_number==1)
                   {
                     if (segment_line[0] == '%' && segment_line.mid(1,3)=="PDF")           return C_BaseBioGest::Pdf;
                     if (segment_line[0] != 'H')                                           return C_BaseBioGest::Not_Hprim;
                     segment_type  = "H";
                     separator     = segment_line[1];         // H|~^\&|URG06 --> prend la valeur |
                     int deb       = 1;                       //  ^--- 1 pointe dessus
                     int end       = segment_line.indexOf(separator[0], 2);
                     if (end != -1)  {separator = segment_line.mid(deb,end-deb); }
                   }
                else
                   { pos           = segment_line.indexOf(separator[0]);  if  (pos==-1)    return C_BaseBioGest::SeparatorNotFound;
                     segment_type  = segment_line.left(pos);
                   }
                //........... traiter les segments sur plusieurs lignes  ............................................................
                // OBR|0001|60425414\1204256414|\1204256414|K\POTASSIUM^TCKMC\TCA (Sur STA Compact MC)^TPMC\Taux de prothrombine^TRAIMC\Patient traitÈ par:^DDI\D-DIMERES^BNPMC\BNP (peptide natriurÈtique de type B):^TROPO\TROPONINE:^MYOM
                // A|C\MYOGLOBINE:^NORM\Normales hormones : METTRE  "NORM"...^FIB\FIBRINOGENE^MYCPN0\Mycoplasma pneumoniae - sÈrologie IgG^CARD00\Anticorps anti-cardiolipine - IgG - sÈrum$^CARDM0\Anticorps anti-cardiolipine - IgM - sÈrum$
                // A|^AAN000\Anticorps anti-nuclÈaires - sÈrum - SÈrum$^ANCA00\Anticorps anti-cytoplasme des polynuclÈai$^CHPN00\Chlamydophila pneumoniae - sÈrologie IgG/$^LEGIO0\LÈgionellose - sÈrogroupes 1 ? 6 - sÈrolo$|R||201204251856|
                // A|201204251856|||N|||||131218026\HERVE Robert\\USC3|0491171280|||||20120427|
                if ( next_line[0] == 'A' )
                   {CApp::pCApp()->outMessage(tr("C_BaseBioGest::analyseFile() segment A '%1'\n.   --> '%1'' ").arg(next_line));
                    segment_line += next_line.mid(2);
                     continue;
                   }

                if ( segment_type == "H" )
                   { expediteur.setFromHprimLine(segment_line, separator);
                   }
                //........... entete de patient ............................................................
                //    P|0001||||LAGRECA~CALOGERA||19271207|F||~~~~|||||||||||||20041030^0928
                //    P|0001|240932||12012010|BRAHMIA\RAFIK||19721129|M||2 RUE BIS DE L EPIS       *\\AVIGNON\\84000|
                //
                else if ( segment_type == "P" )            // definition d'un patient (tous les résultats placé à sa suite lui appartiennent)
                   { patient.setFromHprimLine(segment_line, separator);
                   }
                //........... entete de definion de resultat ...............................................
                //            doit être suivi immédiatement par une ligne d'OBX
                //   OBR|0001|041030MA1045~041030MA1045||N^PLQ^F^TP^TCK|R||200410300928|200410300928|||N||||200410300928|
                //
                else if ( segment_type == "OBR" )     // definition des resultats rendus (doit suivre après OBR une série d'OBX)
                   {patient.m_Service = getService(separator, segment_type, servicesDefs, servicesList, segment_line);                                       // plan A
                    if (patient.m_Service.length()==0) patient.m_Service = getService(separator, segment_type, servicesDefs_1, servicesList, segment_line);  // plan B
                    patient.setOBRId( Get_Hprim_UniqueId(segment_line, separator) );  // positionne m_OBR_Ident sur la valeur tiree du fichier hprim
                    pk_Patient   = GetIDPatient(patient, expediteur);                 // recherche dans la base le patient avec cet m_OBR_Ident, si pas trouve le cree, le retour est le pk
                    CApp::pCApp()->outMessage(tr("C_BaseBioGest::analyseFile()OBR final service '%1'\n.   --> '%2'' ").arg(patient.m_Service, segment_line));
                    //if (pk_Patient.length()) recordHprimLineResult (separator, segment_line, pk_Patient);
                   }
                //.............. detail des examens .........................................................
                //    OBX|0001|TX|N    010~n u m e r a t i o n   g l o b||.||||||||                           type demande de l'examen
                //    OBX|0001|NM|N    020~globules blancs..............:||5960|/mm3|vr : 4 a  10000||||||    type rendu   de l'examen
                //    OBX|0002|ST|GRS||4.51|million/mm|4.10-5.70|N|||F|||201204042026|RIP\10060|
                else if ( segment_type == "OBX" )
                   { //QString type = Get_Result_testType(segment_line);
                     //if (m_obxFilter.length()==0 || m_obxFilter.find(type) != -1)
                        {if (pk_Patient.length()) recordHprimLineResult (separator, segment_line, pk_Patient);
                        }
                   }
                 else if ( segment_type == "L" )      // voila c'est la fin du fichier
                   {
                   }
             } //if (segment_line.length())
            segment_line = next_line;
            ++line_number;
        } // end while (!stream.atEnd())
        // on initialise le resultat = vrai
        result = C_BaseBioGest::Analysed/*2*/;
    } // if ( file.open( QIODevice::ReadOnly ) )
  else
    {return C_BaseBioGest::FileCanNotOpen;
    }
    // on renvoi le resultat
  return result;
}
//------------------------------------ getService --------------------------------------------
QString C_BaseBioGest::getService(const QString & separator, const QString &segment_type, const QStringList &servicesDefs, const QStringList &servicesList,const QString &segment_line)
{if (servicesDefs.size() && segment_type==servicesDefs[0])
    {QStringList lst = segment_line.split(separator[0]);
     QString fromStr = lst[servicesDefs[1].toInt()];
     if (fromStr.length()&& servicesDefs.size()>2)
        {int fi  = servicesDefs[2].toInt();
         lst     = fromStr.split(separator[1]);
         if (lst.size()>fi)fromStr = lst[fi];
        }
     int isOk = servicesList.indexOf(fromStr);
     if (isOk != -1) return fromStr;
    }
    return QString::null;
}

//------------------------------------ Get_Hprim_UniqueId() --------------------------------------------
QString C_BaseBioGest::Get_Hprim_UniqueId(QString &segment_line, QString sep)
{//  OBX|0001|TX|N
 //  OBR|0001|041030MA1045~041030MA1045|N
 //  OBR|0001|60412444\1204126444|\1204126444|ORIG\Commentaire :^U\ACIDE URIQUE|R||201204122234|201204122234|||N|||||131184079\LE NAY CARUELLE Francis\\CMUR||||||20120412|
 if (segment_line.length()==0) return QString::null;
 QStringList list = segment_line.split( sep[0] , QString::KeepEmptyParts );
 if (list.size()<3) return QString::null;
 else               return list[2].remove('\\');
}

//------------------------------------ GetIDPatient() --------------------------------------------
// ACTION: recherche dans la base de donnée si il existe un patient déjà existant pour
//         l'expediteur donné dans                              : CExpediteur &expediteur
//         et pour les caractéristiques du patient données dans : CPatient    &patient
//         si il existe dèjà alors on retourne la clef primaire de l'enregistrement
//         Sinon on crée l'enregistrement et on retourne la clef primaire
//         chaque résultat aura pour réference la clef primaire d'un enregistrement de ce type
// RETOUR: clef primaire de l'enregistrement patient-expediteur

QString C_BaseBioGest::GetIDPatient(CPatient &patient, CExpediteur &expediteur)
{QString pk = IsThisPatientExist(patient/*, expediteur*/);
 if (pk.length()) return pk;
 else             return CreateNewPatient(patient, expediteur);
}

//----------------------------------------------------- FusionneModeleWithResultList ----------------------------------------------------------
QString C_BaseBioGest::FusionneModeleWithResultList(const QString &id_patient, QStringList &resultList, QString &modele, QStringList &serviceList)
{QString      result("");
 CPatient     patient;
 CExpediteur  expediteur;
 //............ recuperer patient et expediteur .......................................................
 if (GetPatientByPrimKey(id_patient, patient, expediteur) == 0) return result;
 //........................... faire les remplacements entete .........................................
 result = FusionneModeleWithInfosPatient(modele, patient, expediteur, serviceList);
 if (result.length()) result.replace("{{patientID}}", id_patient);
 //........................... faire les remplacements ................................................
 int group_deb       = modele.indexOf("$DEB_GROUP=", 0);
 int group_end       = group_deb;
 int deb_bloc        = 0;
 int fin_bloc        = 0;
 long pos_title      = 0;
 long      nb_bloc   = 0;
 //.................recuperer separateur de valeur normales..................
 deb_bloc = modele.indexOf("$SEP_VAL=", deb_bloc);    // deb_bloc utilisé provisoirement
 if (deb_bloc != -1) m_SepVal = modele.mid(deb_bloc+9,1);
 //................ parser le masque d'afichage ...............................
 while (group_deb != -1)
       {
        group_end = modele.indexOf("$FIN_GROUP", group_deb);
        if (group_end != -1)
           {QString titre = GetTitreGroup( group_deb,  group_end, modele);
            pos_title     = result.length();
            nb_bloc       = 0;
            deb_bloc      = modele.indexOf("$DEB_BLOC", deb_bloc);
            while (deb_bloc   != -1 && deb_bloc < group_end)
               {deb_bloc  += 9;
                fin_bloc   = modele.indexOf("$FIN_BLOC", deb_bloc);
                if (fin_bloc != -1)
                   {QString document("");
                    if (FusionneModeleBlocWithResultList(deb_bloc, fin_bloc, resultList, modele, document, patient))
                       {result += document;
                        ++nb_bloc;
                       }
                   }
                else return QString(tr("Erreur de syntaxe : $FIN_BLOC non trouve"));
                deb_bloc   = modele.indexOf("$DEB_BLOC", fin_bloc);
               }
            if (nb_bloc) result.insert (pos_title, titre);
           }
        else return QString(tr("Erreur de syntaxe : $FIN_GROUP   non trouve"));
        group_deb       = modele.indexOf("$DEB_GROUP=", group_end);
       }
 return result;
}

//----------------------------------------------------- FusionneModeleWithInfosPatient ----------------------------------------------------------
QString C_BaseBioGest::FusionneModeleWithInfosPatient(const QString &modele, CPatient &patient, CExpediteur &expediteur, QStringList &serviceList)
{//$DEB_ENTETE Résultats de: {{nom}} {{prenom}} en date du: {{date_examen}} à {{heure_examen}}
 //$FIN_ENTETE
 QString result("");
 int deb_bloc = modele.indexOf("$DEB_ENTETE", 0);
 if (deb_bloc== -1) return result;
 deb_bloc += 11;
 int end_bloc = modele.indexOf("$FIN_ENTETE", deb_bloc);
 if (end_bloc== -1) return QString (tr("Erreur de syntaxe : marqueur de fin d'entete: $FIN_ENTETE absent"));          // erreur de syntaxe
 result = modele.mid(deb_bloc, end_bloc - deb_bloc);

 result.replace("{{nom}}"           ,   patient.m_nom);
 result.replace("{{prenom}}"        ,   patient.m_prenom);
 result.replace("{{dateNaissance}}" ,   DateToString(patient.m_dateNaissance,"-") );
 result.replace("{{sexe}}"          ,   patient.m_sexe);
 result.replace("{{dateExamen}}"    ,   DateToString(expediteur.m_dateEmission,"-"));
 result.replace("{{heureExamen}}"   ,   HeureToString(expediteur.m_heureEmission,":") );
 result.replace("{{OBR_Ident}}"     ,   patient.getOBRId());
 result.replace("{{fileNameRef}}"   ,   patient.getFileRef());
 result.replace("{{service}}"       ,   ServiceCodeToLibelle(patient.m_Service, serviceList) );
 QDate dateNss(patient.m_dateNaissance.left(4).toInt(),
               patient.m_dateNaissance.mid(4,2).toInt(),
               patient.m_dateNaissance.mid(6,2).toInt()
              );
 QString age = CalculeDiffAge(&dateNss); //QDate ( int y, int m, int d )
 result.replace("{{age}}" ,       age);
 result.replace("{{codeExpediteur}}" ,     expediteur.m_codeExpediteur);
 result.replace("{{nomExpediteur}}" ,      expediteur.m_nomExpediteur);
 result.replace("{{codeDestinataire}}" ,   expediteur.m_codeDestinataire);
 result.replace("{{nomDestinataire}}" ,    expediteur.m_nomDestinataire);
 result.replace("{{dateEmission}}" ,       DateToString(expediteur.m_dateEmission,"-"));
 result.replace("{{heureEmission}}" ,      HeureToString(expediteur.m_heureEmission,":") );
 return result;
}
//----------------------------------------------------- ServiceCodeToLibelle ----------------------------------------------------------
QString C_BaseBioGest::ServiceCodeToLibelle(const QString &code, QStringList &serviceList)
{for ( QStringList::Iterator it = serviceList.begin(); it != serviceList.end(); ++it )
     {QString str = *it;
      int p       = str.lastIndexOf(":");
      if (p != -1)
         {if (str.mid(p+1).trimmed() == code) return str.left(p).trimmed();
         }
     }
 return tr("non précisé");
}

//----------------------------------------------------- DateToString ----------------------------------------------------------
QString C_BaseBioGest::DateToString(const QString &date, const char* sep)
{QString result = date.mid(6,2)+sep+date.mid(4,2)+sep+date.mid(0,4);
 return  result;
}
//----------------------------------------------------- HeureToString ----------------------------------------------------------
QString C_BaseBioGest::HeureToString(const QString &heure, const char* sep)
{QString result = heure.left(2)+sep+heure.mid(2,2);
 return  result;
}

//----------------------------------------------------- GetTitreGroup -------------------------------------------------------------
QString C_BaseBioGest::GetTitreGroup(long group_deb, long group_end, QString &modele)
{//$TITRE_GROUP=<b><u>Examens sanguins</b></u>
 int pos = modele.indexOf("$TITRE_GROUP=", group_deb);
 if (pos == -1 || pos>group_end) return QString("");
 pos += 13;
 int end = GotoEndOfLigne(modele, pos);
 if (end == -1 || end>group_end) return QString("");
 return modele.mid(pos, end - pos).trimmed();
}

//----------------------------------------------------- FusionneModeleBlocWithResultList ----------------------------------------------------------
int  C_BaseBioGest::FusionneModeleBlocWithResultList(long deb_bloc, long fin_bloc, QStringList &examList, QString &modele, QString &document, const CPatient &patient)
{QMap < QString , QString > map_err_deb, map_err_fin, map_test_hprim;
 QString value;

 QStringList test_list;
 int pos = deb_bloc;
 int end = 0;
 document = "";
 //....................... relever les codes necessaires à ce bloc .........................................
 // $CODES=NV   020|FV   060|FV   020|FV   040|FV   080|FV   100
 pos = modele.indexOf("$CODES=", deb_bloc);
 if (pos==-1 || pos>fin_bloc)                                          return 0;      // erreur syntaxe
 pos      += 7;
 end       = GotoEndOfLigne(modele, pos);
 value     = modele.mid(pos,end-pos).trimmed();
 test_list = value.split('|');
 if (MapTestWithSegmentHprim(test_list, map_test_hprim, examList)==0)  return 0;   // pas de resultat pour ce bloc
 int nb = 0;
 //........ relever les tag de debutet fin des erreurs pour chaque code de ce bloc ...............................
 // $DEB_ERR.TCK  020=<span style="font-weight:600;color:#550000">
 nb = FillMapWhithTagEndDebTetsName(map_err_deb, deb_bloc, fin_bloc,  modele, "$DEB_ERR.");
 if (nb==-1) return 0; // erreur syntaxe
 nb = FillMapWhithTagEndDebTetsName(map_err_fin, deb_bloc, fin_bloc,  modele, "$FIN_ERR.");
 if (nb==-1) return 0; // erreur syntaxe

 //........ relever les tag de debut de ligne de ce bloc...............................
 // $DEB_LIGNE=<br>
 QString tag_deb_ligne;
 pos = modele.indexOf("$DEB_LIGNE=", pos);
 if (pos != -1 && pos<fin_bloc)
    {end           = GotoEndOfLigne(modele, pos);
     tag_deb_ligne = modele.mid(pos,end-pos).trimmed();
     pos = tag_deb_ligne.indexOf('=');
     if (pos != -1) tag_deb_ligne = tag_deb_ligne.mid(pos+1);
    }
 //........ relever les tag de fin de ligne de ce bloc...............................
 // $FIN_LIGNE=
 QString tag_fin_ligne;
 pos = modele.indexOf("$FIN_LIGNE=", pos);
 if (pos != -1 && pos<fin_bloc)
    {end           = GotoEndOfLigne(modele, pos);
     tag_fin_ligne = modele.mid(pos,end-pos).trimmed();
     pos = tag_fin_ligne.indexOf('=');
     if (pos != -1) tag_fin_ligne = tag_fin_ligne.mid(pos+1);
    }

 //..................... s'occuper maintenant du texte ................................

 // $DEB_TEXTE={{IF=TCK  020}} &nbsp;&nbsp;&nbsp;&nbsp;<b>TCK:&nbsp;témoin</b>&nbsp;&nbsp;{{$DEB_ERR.TCK  020}}{{TCK  020}}{{$FIN_ERR.TCK  020}}
 //                            &nbsp;&nbsp;<b>patient:</b>&nbsp;&nbsp;{{$DEB_ERR.TCK  030}}{{TCK  030}}{{$FIN_ERR.TCK  030}}&nbsp;&nbsp;{{$UNIT.TCK  020}}
 //            {{/IF}}
 //            {{IF=TCK  040}} &nbsp;&nbsp;&nbsp;&nbsp;<b>TCA:&nbsp;patient/témoin</b>&nbsp;&nbsp;{{$DEB_ERR.TCK  040}}{{TCK  040}}{{$FIN_ERR.TCK  040}}
 //            {{/IF}}
 // $FIN_TEXTE
 pos = modele.indexOf("$DEB_TEXTE=", deb_bloc);
 if (pos == -1  || pos>fin_bloc)           return 0;  // erreur syntaxe
 int deb_texte =  pos + 11;
 int end_texte =  modele.indexOf("$FIN_TEXTE", deb_texte);
 if (end_texte== -1 || end_texte>fin_bloc) return 0;  // erreur syntaxe

 document = modele.mid(deb_texte, end_texte-deb_texte);
 document.prepend(tag_deb_ligne);
 QString      tagMinMax = "";
 QString      toReplace;
 QString      replaceBy;
 QString      test_name;
 QStringList  segmentList;
 QStringList  minMaxList;
 QString      lineResult  = "";
 QString      test_valMax = "";
 QString      test_valMin = "";
 int          erreur      = 0;
 int          pos_deb     = 0;
 int          pos_end     = 0;
 //........................... faire les remplacemenst ................................................
 pos_deb = document.indexOf("{{", pos_end);
 while (pos_deb != -1)
       {pos_end = document.indexOf("}}", pos_deb);
        if (pos_end != -1 &&  pos_deb != -1)
           {toReplace = document.mid(pos_deb, pos_end-pos_deb+2);
            //............. ON AFFICHE ET REMPLACE LA VALEUR DU RESULTAT ..................................
            if (toReplace.left(5)=="{{IF=")   // une ligne de resultats commence toujours par "{{IF=" et se termine par {{/IF}}
               {document.replace ( pos_deb, toReplace.length(), "");   // effacer le tag de debut {{IF= XXX}} pour ne pas repasser ici
                pos_end      = document.indexOf("{{/IF}}", pos_deb);   // reperer la fin de la ligne conditionnelle
                test_name    = toReplace.mid(5, toReplace.length()-7); // recuperer nom du test à afficher
                lineResult   = map_test_hprim[test_name];
                segmentList  = lineResult.split(lineResult[0], QString::KeepEmptyParts);      // recuperer le segment de valeurs Hprim pour ce test
                test_valMax  = "";
                test_valMin  = "";
                erreur       = 0;
                replaceBy    = "";
                //................... si il y a un resultat pour ce test à afficher .........
                //                    recuperer les valeurs pour tester depassements
                if (segmentList.count()>=TEST_MINMAX)
                   {replaceBy    = segmentList[TEST_RESULT];                                            // valeur Hprim du resultat
                    //   >> on tente de recuperer d'abord dans le HPRIM
                    if (m_SepVal=="-"||m_SepVal=="+")                                                   // cas particulier de certains blaireaux -min--max
                       {pos = segmentList[TEST_MINMAX].lastIndexOf(m_SepVal);
                        if (pos != -1 && segmentList[TEST_MINMAX].mid(pos-1,1)==m_SepVal) --pos;
                       }
                    else
                      {pos            = segmentList[TEST_MINMAX].indexOf(m_SepVal);
                      }
                    if (pos != -1)
                       {test_valMax   = segmentList[TEST_MINMAX].mid(pos+1);
                        test_valMin   = segmentList[TEST_MINMAX].left(pos);
                        erreur        = TestMinMax(replaceBy, test_valMin, test_valMax);
                       }
                    //   >> on tente ensuite de recuperer ensuite dans les valeurs en base
                    else if (GetMinMaxValue(test_name, patient.m_sexe, test_valMin, test_valMax))
                       {erreur        = TestMinMax(replaceBy, test_valMin, test_valMax);
                       }
                    //   >> on tente ensuite de recuperer ensuite dans le dans le modele
                    //      {{$NL.INRMC|(N: de [min] à [max])|(NL: environ 1)}}
                    else
                       {end       = -1;
                        tagMinMax = QString("{{$NL.%1").arg(test_name);
                        pos       = toReplace.indexOf(tagMinMax);
                        if (pos  != -1) end        = toReplace.indexOf("}}",pos);
                        if (end  != -1)
                           {pos        = pos+6+tagMinMax.length();
                            tagMinMax  = tagMinMax.mid(pos,end-pos).trimmed();
                            if (tagMinMax.length())
                               {minMaxList = tagMinMax.split('|', QString::KeepEmptyParts);
                                if (minMaxList.size()>2)
                                   {pos          =  -1;
                                    test_valMin  = "";
                                    test_valMax  = "";
                                    tagMinMax    = minMaxList[2];
                                    test_valMin  = C_Utils_Html::toNum(tagMinMax, "$keepSign$stopIfNotNum", &pos);
                                    if (pos != -1) test_valMax  = C_Utils_Html::toNum(tagMinMax.mid(pos), "$keepSign", &pos);
                                    erreur       = TestMinMax(replaceBy, test_valMin, test_valMax);
                                   }
                               }
                           }
                       }
                   }   // if (segmentList.count()>=TEST_MINMAX)
                if (replaceBy.length()==0)                                           // si valeur non présente ne pas inclure ce resultat et tout effacer
                   {document.replace ( pos_end, 7, "");                              // effacer {{/IF}}
                    document.replace ( pos_deb, pos_end - pos_deb, "");              // effacer de {{IF= XXX}} à  {{/IF}} (toute la ligne)
                   }
                else //................ en effaçant juste  {{IF= XXX}} et {{/IF}} ..................
                     //                 la ligne se place dans la config normale traitée dans le dernier: else
                   {document.replace ( pos_end, 7, "");                              // effacer {{/IF}}
                   }
               }  // if (toReplace.left(5)=="{{IF=")

            //............. ON AFFICHE ET REMPLACE LES VALEURS MIN ET MAX DU RESULTAT ..................................
            else if (toReplace.left(6)=="{{$NL.")    // valeur max et min {{$NL.TP   020|(NL: de [min] à [max])|(NL: de 70 à 100 )}}
               {int Ok = 0;
                QString name = toReplace.mid(6,toReplace.length()-8);
                //........... recuperer les segments du tag .........................
                minMaxList   = name.split('|', QString::KeepEmptyParts);
                name         = minMaxList[0];
                replaceBy    = Get_Result_testType(lineResult, TEST_MINMAX);
                if (name == segmentList[TEST_NAME] && (test_valMax.length()||test_valMin.length())) Ok = 1;
                if ( Ok==0 )
                   {pos          = replaceBy.indexOf(m_SepVal);
                    //................. rechercher valeurs min et max ...................
                    //                  directement dans le resultat du patient
                    if (pos != -1)
                       {test_valMax   = toNumStr(replaceBy.mid(pos+1));
                        test_valMin   = toNumStr(replaceBy.left(pos));
                        if (test_valMax.length()||test_valMin.length()) Ok = 1;   // si au moins une des deux valeurs présentes c'est Ok
                      }
                    //................. si pas trouvée chercher dans la base de données ................
                    //                  des valeurs normales
                    if ( Ok==0 )
                       {Ok =GetMinMaxValue(name, patient.m_sexe, test_valMin, test_valMax);
                       }
                   }
                if (Ok)
                   {replaceBy = minMaxList[1];
                    replaceBy.replace("[min]",  test_valMin);
                    replaceBy.replace("[max]",  test_valMax);
                   }
                else
                   {replaceBy = minMaxList[2].replace("}}","");
                   }
                document.replace ( pos_deb, toReplace.length(), replaceBy);
               }
            else if (toReplace.left(11)=="{{$DEB_ERR.")
               {if (erreur)
                   {QString name = toReplace.mid(11,toReplace.length()-13);
                    document.replace ( pos_deb, pos_end - pos_deb+2, map_err_deb[name]);
                   }
                else
                   {document.replace ( pos_deb, pos_end - pos_deb+2, "");
                   }
               }
            else if (toReplace.left(11)=="{{$FIN_ERR.")
               {if (erreur)
                   {QString name = toReplace.mid(11,toReplace.length()-13);
                    document.replace ( pos_deb, pos_end - pos_deb+2, map_err_fin[name]);
                   }
                else
                   {document.replace ( pos_deb, pos_end - pos_deb+2, "");
                   }
               }
            else if (toReplace.left(8)=="{{$UNIT.")
               {QString name = toReplace.mid(8,toReplace.length()-10);
                replaceBy    = Get_Result_testType(lineResult, TEST_UNIT);
                document.replace ( pos_deb, toReplace.length(), replaceBy);
               }
            else     //   {{XXX}}    test name
               {QString name = toReplace.mid(2,toReplace.length()-4);
                replaceBy    = Get_Result_testType(lineResult, TEST_RESULT);
                replaceBy.replace('>',"&gt;");
                replaceBy.replace('<',"&lt;");
                int p = replaceBy.indexOf(lineResult[1]); if (p!=-1) replaceBy.truncate(p);
                document.replace ( pos_deb, toReplace.length(), replaceBy);
                pos_deb  +=  replaceBy.length();
               }
           }
        pos_deb = document.indexOf("{{", pos_deb);
       }
 return 1;
}
//------------------------------------ Get_Result_testType() --------------------------------------------
QString C_BaseBioGest::Get_Result_testType(const QString &segment_line , int posToRetrieve)
{//  OBX|0001|TX|N
 //  OBR|0001|041030MA1045~041030MA1045|N
 if (segment_line.length()==0) return QString::null;
 QChar sep  = segment_line[0];
 int   i    = 0;
 int end    = 0;
 int pos    = -2;   // pour passer le premier tour
 while (pos != 0)
       {if (pos == -2) pos = 0;
        end  = segment_line.indexOf(sep, pos);
        if      (end == -1 )            return segment_line.mid(pos);
        else if (i   == posToRetrieve ) return segment_line.mid(pos, end-pos);
        ++i;
        pos = end + 1;
       }
 return QString("");
}
//----------------------------------------------------- FillMapWhithTagEndDebTetsName -------------------------------------------------------------
int C_BaseBioGest::FillMapWhithTagEndDebTetsName(QMap < QString , QString >  &hprimMap, long deb_bloc, long fin_bloc, QString &modele, const QString &token)
{QString tag, value;
 long pos = deb_bloc;
 long end = 0;
 int  nb  = 0;
 while(pos<fin_bloc && pos != -1)
       {pos = modele.indexOf(token, pos);
        if (pos != -1 && pos<fin_bloc)
           {pos += 9;
            end = modele.indexOf("=", pos);
            if (end != -1 && end<fin_bloc)
               {tag = modele.mid(pos,end-pos).trimmed();
                pos = end + 1;
                end = GotoEndOfLigne(modele, pos);
                if (end != -1)  value = modele.mid(pos, end-pos);
                else            return -1;      // erreur syntaxe
                hprimMap.insert (tag, value);
                ++nb;
               }
           }
      }// while(pos<fin_bloc)
 return nb;
}
//----------------------------------------------------- MapTestWithSegmentHprim----------------------------------------------------------
// ACTION: crée une QMap liste avec comme clef le nom de l'examen dont on va avoir besoin et comme valeur son segment HPRIM
//         au cas où il est présent dans la liste de resultats
int C_BaseBioGest::MapTestWithSegmentHprim(QStringList &test_list, QMap < QString , QString > &map_list, QStringList &resultList)
{int nb = 0;
 //OBX|0001|NM|N    020~globules blancs..............:||13570|/mm3|vr : 4 a  10000||||||
 for ( QStringList::Iterator it = resultList.begin(); it != resultList.end(); ++it )
     {QString test_name = Get_Result_testType(*it, TEST_NAME);
      int           pos = test_name.indexOf( (*it)[1]);
      if (pos != -1)
         {test_name.truncate(pos);
         }
      if (test_list.indexOf( test_name ) != -1)
         {map_list.insert(test_name, *it);
          ++nb;
         }
     }
 return nb;
}

//------------------------------------------------- TestMinMax --------------------------------------------------------------------------
int C_BaseBioGest::TestMinMax( QString &sval, const QString &min, const QString &max)
{//sval<0.10 min 0.00 max 0.10
 //sval>4013 min 17.4 max 105.7
 bool maxok, minok, valok;
 sval=sval.remove('>').remove('<');
 sval=toNumStr(sval);
 float val     = sval.toFloat(&valok);
 if (valok==FALSE) return 0;
 float valMax  = toNumStr(max).toFloat(&maxok);
 float valMin  = toNumStr(min).toFloat(&minok);
 if (maxok==false && minok==false) return 0;

 if (maxok==false && val<valMin)   return 1;
 if (minok==false && val>valMax)   return 1;
 if (val>valMax || val<valMin)     return 1;
 return 0;
}

//----------------------------------------------------- toNumStr -----------------------------------------------------------------------
QString C_BaseBioGest::toNumStr(const QString &str)
{ return C_Utils_Html::toNum(str, "$keepSign$stopIfNotNum");
}

//------------------------------ GotoEndOfLigne -----------------------------------------
long C_BaseBioGest::GotoEndOfLigne(const QString &txt, int i /*=0*/)
{int len = txt.length();
 while (i<len)
    {
      if (txt[i]=='\\') {++i; continue;}
      if (txt[i]=='/' && txt[i+1]=='/') return i;
      if (txt[i]=='\r' || txt[i]=='\n') return i;
      ++i;
     }
 return len;
}

//-----------------------------------------------------  CalculeDiffAge -------------------------------------------------
// ACTION: calcule l'age entre deux dates de début et de fin.
// ENTREE: QDate *pDebut: pointe sur une QDate de debut (exemple date de naissance)
//         QDate *pFin:   pointe sur une QDate de  fin (peut etre zero alors il sera pris la date en cours)
//         bool bTexte:   TRUE   alors la date sera exprimée comme: 4 ans 6 mois 3 jours
//                        FALSE  alors la date sera exprimée comme un chiffre
//         int bAbrege:   2  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        1  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        0  et alors les sorties seront detaillées avec si age<3 avec les mois et jours:
QString C_BaseBioGest::CalculeDiffAge(QDate *pDebut, QDate *pFin/*=NULL*/, bool bTexte/*=TRUE*/, int bAbrege/*=1*/)
{
 char finMois[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 int an=0;
 int mois=0;
 int jour=0;
 int dayYearDebut;
 int dayYearFin;
 QString buffer;
 QString sRes("");
 bool bFinAlloue = FALSE;
 long lEtatDayYear;

 if (pDebut && pDebut->isValid ())
    {
     if (pFin==NULL)
        {pFin       = new QDate;
         *pFin      = QDate::currentDate ();
         bFinAlloue = TRUE;
        }//if

     if (pFin->isValid ())
        {
         //.........................................Partie calcul ..........................................
         //........Le jour de l'année .................
         dayYearDebut = pDebut->dayOfYear ();
         dayYearFin   = pFin->dayOfYear ();
         //................................ bisextiles and cie ..................................................
         //ATTENTION A L'ANNEE BISEXTILLE (sauf en l'an 1900, car les débuts de siecles ne sont pas bisextille sauf les débuts de millénaire
         //60eme jour => 29 Février
         if (pDebut->year()%4==0 && pDebut->year()%100 && pDebut->year()%1000    &&  dayYearDebut>60 )
             dayYearDebut--;
         if (pFin->year()%4==0   && pFin->year()%100   && pFin->year()%1000      &&  dayYearFin>60)
             dayYearFin--;

          //....................Position des jours l'un par rapport à l'autre ....................................
          if      (dayYearFin<dayYearDebut)     lEtatDayYear = -1;
          else if (dayYearFin>dayYearDebut)     lEtatDayYear =  1;
          else                                  lEtatDayYear =  0;

          //...................Calcul année (-1 si le jour de l'année en cour est <) .............................
          an = pFin->year() - pDebut->year() - (lEtatDayYear<0? 1 :0);

          if (lEtatDayYear!=0)
             {
              //.......................Calcul mois .....................................
              if (lEtatDayYear<0)
                  mois = 12 - pDebut->month() + pFin->month()   - (pFin->day()<pDebut->day()? 1 :0);
              else
                  mois = pFin->month()        - pDebut->month() - (pFin->day()<pDebut->day()? 1 :0);

              //...................... Calcul du jour ...................................
              if (pFin->day() < pDebut->day())
                  jour=(finMois[pFin->month()>1 ? pFin->month()-2 : 11] - pDebut->day()) + pFin->day();
              else
                  jour=pFin->day() - pDebut->day();
             } //if(lEtatDayYear!=0)

             //.............................................. Partie texte .........................................
          if (an)
             {
              if (bTexte)
                  //buffer.sprintf("%d an%s ", an, an>1 ? "s" : "");
                  buffer = QString::number(an) + tr(" an") + ((an>1) ? "s " : " ");
              else
                  buffer.sprintf("%d ", an);
              sRes = buffer;
             } //if (an)
          //.................... si bAbrege==2 alors on ne sort que les années .......................................
          if (bAbrege==2)
             {if (an==0) sRes="0";
              if(bFinAlloue)  delete pFin;
              return sRes;
             }
          if (!bAbrege || an < 3 /* || an<3*/) // age Inferieur à 3 ans et version detaillée
             {
              if (mois || !bTexte)
                 {
                  if (bTexte)
                      //buffer.sprintf("%d mois ", mois);  // en fançais " mois " à déjà un s en [4] donc ne pas rajouter si plusieurs
                     {QString ms = tr(" mois");  // va bien y avoir un dialecte où ça ne va pas marcher
                      buffer     = QString::number(mois)+ ms + ((mois>1&&ms[4]!='s') ? "s " : " ");
                     }
                  else
                      buffer.sprintf("%d ", mois);
                  sRes+=buffer;
                } // if (mois || !bTexte)

              if (jour || !bTexte)
                 {
                  if (bTexte)
                      //buffer.sprintf("%d jour%s ", jour, jour>1 ? "s" : "");
                      buffer = QString::number(jour) + tr(" jour") + ((jour>1) ? "s " : " ");
                  else
                      buffer.sprintf("%d ", jour);
                  sRes += buffer;
                 } // if  (jour || !bTexte)
              }// if (!bAbrege || an<3) //Inferieure à 3 ans
          }// if (pFin->m_status==COleDateTime::valid)
     }// if (pDebut && pDebut->m_status==COleDateTime::valid)
     if(bFinAlloue)  delete pFin;
     return sRes;
}
