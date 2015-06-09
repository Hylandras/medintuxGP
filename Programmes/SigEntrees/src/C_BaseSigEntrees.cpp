#include "C_BaseSigEntrees.h"
#include <QDomElement>
#include <QDomNode>
#include <QSqlQuery>
#include <QSqlQuery>
#include <QVariant>
#include <QTreeWidget>
#include <QSqlError>
#include <QMap>

#include "C_DSigemsVar.h"
#include "CApp.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#define TEST_SEP      1
#define TEST_NAME     5
#define TEST_RESULT   7
#define TEST_UNIT     8
#define TEST_MINMAX   9

//================================================ CPatient ======================================================================
//-------------------------------- CPatient -----------------------------------
CPatient::CPatient()
{  m_nom                   = "";
   m_prenom                = "";
   m_dateNaissance         = "";
   m_sexe                  = "";
   m_OBR_Ident             = "";
   //m_Service               = "";
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
{Q_UNUSED(service);
 m_nom                   = nom;
 m_prenom                = prenom;
 m_dateNaissance         = dateNss;
 m_sexe                  = sexe;
 m_OBR_Ident             = obrIdent;
 //m_Service               = service;
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

//-------------------------------- setFromHprimLineH -----------------------------------
// Recup date et heure dans ligne H
// H|~^\&|PRES1818.HPR|SIGEMS|M070780168~SA CLINIQUE DE MARIGNANE|AV.DU GENERAL RAOUL SALAN~~MARIGNANE CEDEX~~13721~|ADM|04.42.78.45.00||CASA~LABORATOIRE||P|H2.0~C|201209030951
void CPatient::setFromHprimLineH( const QString& hprim_line, const QString &separator)
{  QStringList list = hprim_line.split( separator[0] , QString::KeepEmptyParts );
   if (list.size()>13)
        {QString datez         = list[13];
         m_dateEntree = datez.mid(0,4) + "-" + datez.mid(4,2) + "-" + datez.mid(6,2) + " " + datez.mid(8,2) + ":" + datez.mid(10,2) + ":00";
        }
}


//-------------------------------- setFromHprimLine -----------------------------------
// P|0005|138496||12030920|VICENTE~PATRICIA|GALLO|19651109|F||BD DELATTRE DE TASSIGNY~RES LE MOULIN D AVAL BAT B~MARIGNANE~~13700~||06 84 41 17 17|131151912~PASCHE||||||||||20120902^20120903|ER|NULL~NULL~URGE|||U|||||

void CPatient::setFromHprimLine( const QString& hprim_line, const QString &separator)
{  QStringList list                      = hprim_line.split( separator[0] , QString::KeepEmptyParts );
   QStringList section;
   QString  datez;
   if (list.size()>2)    m_num_pat_sigems = list[2];     // Identifiant du patient  ???
   if (list.size()>4)    m_numEntreeSigems= list[2];     // Identifiant de l'entree ????
   if (list.size()>5)    section          = list[5].split(    separator[1] , QString::KeepEmptyParts );
   if (section.size()>0) m_nom            = section[0];
   if (section.size()>1) m_prenom         = section[1];

   if (list.size()>7)
        {datez = list[7];
         m_dateNaissance =  datez.mid(0,4) + "-" + datez.mid(4,2) + "-" + datez.mid(6,2) + " 00:00:00";
        }
   if (list.size()>8)    m_sexe          = list[8];

}
//-------------------------------- setFromHprimLineAP -----------------------------------
// Recup des infos Sesam Vitale
// AP|0001|10|20120101|20121231|164031302811320|01|1|03|043|5100|2|0|TORTAROLO~PIERRE||100||N||||||
void CPatient::setFromHprimLineAP( const QString& hprim_line, const QString &separator)
{  QStringList list                      = hprim_line.split( separator[0] , QString::KeepEmptyParts );

      if (list.size()>5)    m_numSS         = list[5];
      if (list.size()>6)    m_Qualite       = list[6];
      if (list.size()>7)    m_Rang_geme     = list[7];
}

//-------------------------------- getPkRecord -----------------------------------
QString CPatient::getPkRecord(){return m_pkRecord;}
//-------------------------------- setPkRecord -----------------------------------
void CPatient::setPkRecord(const QString &value) {if (value.length()) m_pkRecord=value;}
//-------------------------------- getDateExam -----------------------------------
QString CPatient::getDateExam(){return m_dateEntree.mid(0,10);}
//-------------------------------- setDateExam -----------------------------------
void CPatient::setDateExam(const QString &value) {if (value.length()) m_dateExam=value;}
//-------------------------------- getHeureExam -----------------------------------
QString CPatient::getHeureExam(){return m_dateEntree.mid(11,5);}
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

//====================================== C_BaseSigEntrees ===============================================================================
//--------------------------- C_BaseSigEntrees ---------------------------------------------------------
C_BaseSigEntrees::C_BaseSigEntrees(const QString &param , QString& confData, QObject *parent)
    : C_BaseCommon(parent) , C_DSigemsVar( confData)
{if (!C_DSigemsVar::areFieldsNamesValids())  C_BaseSigEntrees::setLastError(C_DSigemsVar::getLastError());
 reConnect(param);
 m_SepVal        = 'a';
 //m_HprimCodeList = setCodeHprimListFromTemplate( CApp::pCApp()->getTemplate() );  // relever les codes hprim utilisee parr le template
 CApp::pCApp()->outMessage(tr("C_BaseSigEntrees::C_BaseSigEntrees() number of Hprim codes found :%1").arg(QString::number(m_HprimCodeList.size())));
 CApp::pCApp()->outMessage(tr("C_BaseSigEntrees::C_BaseSigEntrees() number of Hprim codes\n%1").arg(m_HprimCodeList.join ( "-" ) ));
}
//--------------------------- getVersionConfig -----------------------------------------------------------
QString   C_BaseSigEntrees::getVersionConfig()
{return m_VERSION;
}
//--------------------------- reConnect -----------------------------------------------------------
bool       C_BaseSigEntrees::reConnect(const QString &param)
{

 bool ok = BaseConnect(param,"C_BaseSigEntrees_LabelBase", m_SCHEMA_NAME);
 if (!ok)    C_BaseSigEntrees::setLastError(C_BaseCommon::getLastError());

 return ok;
}

//--------------------------- setLastError -----------------------------------------------------------
void       C_BaseSigEntrees::setLastError(const QString &error){ m_LastError = error;}

//--------------------------- getLastError -----------------------------------------------------------
QString    C_BaseSigEntrees::getLastError(){return m_LastError;}


//--------------------------- fill_CPatientMap -----------------------------------------------------------
int    C_BaseSigEntrees::fill_CPatientMap(QMap<QString, CPatient> &patientPk_CPatient_Map,
                                       const QString &debut ,
                                       const QString &fin ,
                                       const QString &nom,
                                       const QString &_service )
{   Q_UNUSED(patientPk_CPatient_Map);
    Q_UNUSED(debut);
    Q_UNUSED(fin);
    Q_UNUSED(nom);
    Q_UNUSED(_service);
    /*
    patientPk_CPatient_Map.clear();;
    bool whereMustBePresent = true;
    QString         service = _service;
    //........... ne garder que l'utile ..................
    //            ce qu'il y a apres le :
    if ( service.length())
       { service     = _service;
         int     pos = service.lastIndexOf(':');
         if (pos !=-1) service= service.mid(pos+1).trimmed();
       }
    //................. filtrer selon donn�e d'entr�e ..................................................
    //                  peut �tre limiter la requete ???
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
        {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseSigEntrees::fill_CPatientMap() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                   __FILE__, __LINE__  );
        }
    return patientPk_CPatient_Map.size();
    */ return 0;
}
//------------------------------------ getExamensListFromPatient() --------------------------------------------
// QString  m_LABO_EXAMENS_TBL_NAME;               // nom  de la table des resultats hprim
// QString  m_LABO_EXAMENS_REF_PK;                 // reference sur le patient a qui appartient ce resultat
// QString  m_LABO_EXAMENS_PK;                     // clef primaire
// QString  m_LABO_EXAMENS_OBX_HPRIM;              // donnees hprim brutes
int C_BaseSigEntrees::getExamensListFromPatient (const QString &id_patient , QStringList &examList)
{
   Q_UNUSED(id_patient);
   Q_UNUSED(examList);
   //int nb          = 0;
   /*
    //................. filtrer selon donn�e d'entr�e ..................................................
    QString requete = QString("SELECT %1 FROM %2 WHERE %3 = '%4'" )
                              .arg(m_LABO_EXAMENS_OBX_HPRIM,      // %1  --> 0
                                   m_LABO_EXAMENS_TBL_NAME,       // %2
                                   m_LABO_EXAMENS_REF_PK,         // %3
                                   id_patient);                   // %4
    QSqlQuery query (requete , database() );
    //.................. si la requ�te a un resultat ..............................................
    if (query.isActive())
       {
        examList.clear();
        while (query.next())
           { examList.append(query.value(0).toString());
             ++nb;
           }
       }
    else
        {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseSigEntrees::getExamensListFromPatient() \n%1\nREQUEST : %2").arg(query.lastError().text(),requete).toAscii(),
                                   __FILE__, __LINE__  );
        }
    return nb;
    */ return 0;
}
/*
ES_ID_Primkey, ES_Nom, ES_Prenom, ES_Nom_JF, ES_Sexe, ES_Date_Naiss, ES_Num_SS,
ES_Adresse, ES_CP, ES_Ville, ES_Tel, ES_Status, ES_Date_Entree, ES_Nom_Fichier_Entrees,
ES_GUID_Pat, ES_ID_Sigems, ES_Blob_HPR,
ES_Rang_Gem, ES_Qualite,
ES_Nom_Assure, ES_Prenom_Assure,ES_Date_Naiss_assure
*/
//-----------------------------------------------------  CreateNewEntry -------------------------------------------
QString C_BaseSigEntrees::CreateNewEntry(CPatient &patient)
{
    // controle si patient Sigems existe dans Medintux
    patient.m_GUID_Pat = IsThisPatientExist( patient );

    QString prepare  = "";
    prepare         += "INSERT INTO "               + m_SIG_ENTREES_TBL_NAME  + " ( \n";
    prepare         += m_SIG_ENT_GUID_PATIENT       + " , ";         // 0
    prepare         += m_SIG_ENT_NOM                + " , ";         // 1
    prepare         += m_SIG_ENT_PRENOM             + " , ";         // 2
    prepare         += m_SIG_ENT_SEXE               + " , ";         // 3
    prepare         += m_SIG_ENT_DATE_NSS           + " , ";         // 4
    prepare         += m_SIG_ENT_NUM_SS             + " , ";         // 5
    prepare         += m_SIG_ENT_DATE_ENTREE        + " , ";         // 6
    prepare         += m_SIG_ENT_BLOB_HPR           + " , ";         // 7
    prepare         += m_SIG_ENT_PK                 + " , ";         // 8
    prepare         += m_SIG_ENT_QUALITE            + " , ";         // 9
    prepare         += m_SIG_ENT_RANG_GEME          + " , ";         // 10
    prepare         += m_SIG_ENT_NOM_FIC_HPRIM      + " , ";         // 11
    prepare         += m_SIG_ENT_DATE_PEC           + " , ";         // 12
    prepare         += m_SIG_ENT_NUM_PAT_SIGEMS     + " ) ";         // 13

     prepare         += " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?) ";

    QSqlQuery query(database());
    query.prepare(prepare);
    query.bindValue(0,  patient.m_GUID_Pat);                   // 0
    query.bindValue(1,  patient.m_nom);                        // 1
    query.bindValue(2,  patient.m_prenom);                     // 2
    query.bindValue(3,  patient.m_sexe);                       // 3
    query.bindValue(4,  patient.m_dateNaissance);              // 4
    query.bindValue(5,  patient.m_numSS);                      // 5
    query.bindValue(6,  patient.m_dateEntree);                 // 6
    query.bindValue(7,  patient.m_Blob_HPR);                   // 7
    query.bindValue(8,  patient.m_numEntreeSigems);            // 8
    query.bindValue(9,  patient.m_Qualite);                    // 9
    query.bindValue(10, patient.m_Rang_geme);                  // 10
    query.bindValue(11, patient.getFileRef());                 // 11
    query.bindValue(12, DATE_NULLE);                           // 12
    query.bindValue(13, patient.m_num_pat_sigems);             // 13

    if ( !query.exec() )
       { CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseSigEntrees::CreateNewEntry() \n%1\nREQUEST: %2").arg(query.lastError().text(), prepare).toAscii(),__FILE__, __LINE__  );
         CApp::pCApp()->outMessage(tr("\nREQUEST COMPLETE: %2").arg( "\n.              patient.m_GUID_Pat : "           + patient.m_GUID_Pat        +    // 0
                                                                     "\n.              patient.m_nom : "                + patient.m_nom             +    // 1
                                                                     "\n.              patient.m_prenom : "             + patient.m_prenom          +    // 2
                                                                     "\n.              patient.m_sexe : "               + patient.m_sexe            +    // 3
                                                                     "\n.              patient.m_dateNaissance : "      + patient.m_dateNaissance   +    // 4
                                                                     "\n.              patient.m_numSS : "              + patient.m_numSS           +    // 5
                                                                     "\n.              patient.m_dateEntree : "         + patient.m_dateEntree      +    // 6
                                                                     "\n.              patient.m_Blob_HPR : "           + patient.m_Blob_HPR        +    // 7
                                                                     "\n.              patient.m_numEntreeSigems : "    + patient.m_numEntreeSigems +    // 8
                                                                     "\n.              patient.m_Qualite : "            + patient.m_Qualite         +    // 9
                                                                     "\n.              patient.m_Rang_geme : "          + patient.m_Rang_geme       +    // 10
                                                                     "\n.              patient.patient.getFileRef() : " + patient.getFileRef()      +    // 11
                                                                     "\n.              patient.DATE_NULLE : "           + DATE_NULLE                +    // 12
                                                                     "\npatient.m_num_pat_sigems : "     + patient.m_num_pat_sigems       // 13
                                                                   ).toAscii(),__FILE__, __LINE__  );
        return QString::null;
       }

    addEntryIntreeWidget_Patients(patient);

    return getLastPrimaryKey( m_SIG_ENTREES_TBL_NAME, m_SIG_ENT_PK, m_SIG_ENT_NOM, m_SIG_ENT_PRENOM);

}

//Ajout d'une entree dans la liste
//--------------------------------------- Slot_on_ScrutationTimerOut -------------------------------------------
void C_BaseSigEntrees::addEntryIntreeWidget_Patients(CPatient &patient)
{
    QTreeWidgetItem* pQTreeWidgetItem = 0;
    pQTreeWidgetItem = new QTreeWidgetItem(CApp::pCApp()->G_p_Dlg_SigEntrees->treeWidget_Patients);
    if (pQTreeWidgetItem)
       {pQTreeWidgetItem->setText(0, patient.m_nom);                        // nom
        pQTreeWidgetItem->setText(1, patient.m_prenom);                     // prenom
        pQTreeWidgetItem->setText(2, patient.getDateExam());                // date
        pQTreeWidgetItem->setText(3, patient.getHeureExam());               // heure
        pQTreeWidgetItem->setText(4, patient.m_dateNaissance.mid(0,10));    // Dt Naiss
        pQTreeWidgetItem->setText(5, patient.m_sexe);                       // Sexe
 /*
        pQTreeWidgetItem->setText(6, patient.getNbObx());        // nb OBX
  */
        pQTreeWidgetItem->setText(7, patient.m_numEntreeSigems);            // pk entree

    //    pQTreeWidgetItem->setIcon(0, m_newPatient);
    }
}

// Controle si patient existe dans medintux
//-----------------------------------------  IsThisPatientExist -------------------------------------------
QString C_BaseSigEntrees::IsThisPatientExist( CPatient &patient )
{
    QString ret("");
    QString requete;
       requete = "SELECT " + m_DOSS_INDEX_GUID
               + " FROM "  + m_DOSS_INDEX_TBL_NAME
               + " WHERE " + m_DOSS_INDEX_GUID      + " = '" + patient.m_num_pat_sigems  + "'"
               + " AND "   + m_DOSS_INDEX_NOM       + " = '" + patient.m_nom             + "'"
               + " AND "   + m_DOSS_INDEX_PRENOM    + " = '" + patient.m_prenom          + "'";

       QSqlQuery query(requete , database() );
        if ( query.isActive() )
           { if (query.next() )
               ret = query.value(0).toString();
           }

    return ret;
}


//----------------------------------- Exemple de fichier HPRIM recu ------------------------
/*
H|~^\&|PRES1819.HPR|SIGEMS|M070780168~SA CLINIQUE DE MARIGNANE|AV.DU GENERAL RAOUL SALAN~~MARIGNANE CEDEX~~13721~|ADM|04.42.78.45.00||CASA~LABORATOIRE||P|H2.0~C|201209031001
P|0001|246948||12030922|TORTAROLO~PIERRE||19640330|M||RN 113 LA GARANNE~HAMEAU LA GARANNE~BERRE L ETANG~~13130~||06 08 87 29 61|131141152~FIGUIERE||||||||||20120903|ER|NULL~NULL~URGE|||U|||||
AP|0001|10|20120101|20121231|164031302811320|01|1|03|043|5100|2|0|TORTAROLO~PIERRE||100||N||||||
AC|0001|MU000017|1|20120903|20130302||
P|0002|107624||11040646|JACQUEMIN~ERIC||19660303|M||1 LES CYCLAMENS~RUE DES SYMPHONIDES~MARTIGUES~~13500~||04 42 49 69 62|131169617~LORCA||||||||||20111123^20111125|IP|318P~318P~CHIR|||U|||||
AP|0001|10|20100701|20120630|166035741206764|01|1|01|131|0043|2|0|JACQUEMIN~ERIC||100||N|||DE|~~~~13~||
AC|0001|MU002557|1|20111123|20120521||
*/
//------------------------------------ analyseFile() --------------------------------------------

int C_BaseSigEntrees::analyseFile(const QString &fileName)
{

//return 0;}

    int         line_number   =  0;
    int         pos           = -1;
    //int         end           = -1;
    QString     next_line     = "";
    QString     segment_line  = "";
    QString     pk_Patient    = "";
    QString     segment_type  = "";
    QString     separator     = "";

    CPatient    patient;       // patient courant

    int result       = C_BaseSigEntrees::Error ;

    //........................on cr�� un fichier virtuel du fichier ......................
    QFile file( fileName );
    //........................on test la taille du fichier : ...............................
    //                        si elle est �gale � z�ro on retourne sans analyser
    if (file.size() == 0) return C_BaseSigEntrees::Useless;
    // on ouvre le fichier en lecture
    QString baseName = QFileInfo(fileName).completeBaseName();
    patient.setFileRef(baseName);
    if ( file.open( QIODevice::ReadOnly ) )
       {//......... on cr�e un stream sur le fichier .....................................
        QTextStream stream ( &file );
        stream.setCodec("ISO 8859-1");
        //.......... on d�clare une variable pour le lire ligne � ligne ..................
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
                //            l'exp�diteur en cours, les separateurs
                //      H|~^\&|PRES1819.HPR|SIGEMS|M070780168~SA CLINIQUE DE MARIGNANE|AV.DU GENERAL RAOUL SALAN~~MARIGNANE CEDEX~~13721~|ADM|04.42.78.45.00||CASA~LABORATOIRE||P|H2.0~C|201209031001
                //
                if (line_number==1)
                   {
                     // if (segment_line[0] == '%' && segment_line.mid(1,3)=="PDF")           return C_BaseSigEntrees::Pdf;
                     if (segment_line[0] != 'H')    return C_BaseSigEntrees::Not_Hprim;
                     segment_type  = "H";
                     separator     = segment_line[1];         // H|~^\&|URG06 --> prend la valeur |
                     int deb       = 1;                       //  ^--- 1 pointe dessus
                     int end       = segment_line.indexOf(separator[0], 2);
                     if (end != -1)  {separator = segment_line.mid(deb,end-deb); }
                   }
                else
                   { pos           = segment_line.indexOf(separator[0]);  if  (pos==-1)    return C_BaseSigEntrees::SeparatorNotFound;
                     segment_type  = segment_line.left(pos);
                   }
                //........... traiter les segments sur plusieurs lignes  ............................................................
                // P|0001|246948||12030922|TORTAROLO~PIERRE||19640330|M||RN 113 LA GARANNE~HAMEAU LA GARANNE~BERRE L ETANG~~13130~||06 08 87 29 61|131141152~FIGUIERE||||||||||20120903|ER|NULL~NULL~URGE|||U|||||
                // AP|0001|10|20120101|20121231|164031302811320|01|1|03|043|5100|2|0|TORTAROLO~PIERRE||100||N||||||
                // AC|0001|MU000017|1|20120903|20130302||

                if ( segment_type == "H" )
                   {
                   // recup date et heure du fichier. ??? PAs l'heure dans l'enreg Patient
                     patient.setFromHprimLineH(segment_line, separator);
                   }
                //........... entete de patient ............................................................
                //    P|0001||||LAGRECA~CALOGERA||19271207|F||~~~~|||||||||||||20041030^0928
                //    P|0001|240932||12012010|BRAHMIA\RAFIK||19721129|M||2 RUE BIS DE L EPIS       *\\AVIGNON\\84000|
                //
                else if ( segment_type == "P" )            // definition d'un patient (tous les r�sultats plac� � sa suite lui appartiennent)
                   { patient.setFromHprimLine(segment_line, separator);
                     patient.m_Blob_HPR = segment_line;
                   }
                //............ doit �tre suivi imm�diatement par une ligne d'AP
                //   AP|0001|10|20120101|20121231|164031302811320|01|1|03|043|5100|2|0|TORTAROLO~PIERRE||100||N||||||
                //
                else if ( segment_type == "AP" )     // ?????
                   {
                    patient.setFromHprimLineAP(segment_line, separator);
                    patient.m_Blob_HPR += "\n" + segment_line;
                    pk_Patient = CreateNewEntry(patient);
                }
                //........... doit �tre suivi imm�diatement par une ligne d'AC
                /* Le AC N'EST PAS OBLIGATOIRE
                //    AC|0001|MU000017|1|20120903|20130302||
                else if ( segment_type == "AC" )
                   {
                    patient.m_Blob_HPR += "\n" + segment_line;
                    pk_Patient = CreateNewEntry(patient);
                   }
                   */
                 else if ( segment_type == "L" )      // voila c'est la fin du fichier
                   {
                   }
             } //if (segment_line.length())
            segment_line = next_line;
            ++line_number;
        } // end while (!stream.atEnd())
        // on initialise le resultat = vrai
        result = C_BaseSigEntrees::Analysed;
    } // if ( file.open( QIODevice::ReadOnly ) )
  else
    {return C_BaseSigEntrees::FileCanNotOpen;
    }
    // on renvoi le resultat
  return result;
}

//------------------------------------ Get_Hprim_UniqueId() --------------------------------------------
QString C_BaseSigEntrees::Get_Hprim_UniqueId(QString &segment_line, QString sep)
{//  OBX|0001|TX|N
 //  OBR|0001|041030MA1045~041030MA1045|N
 //  OBR|0001|60412444\1204126444|\1204126444|ORIG\Commentaire :^U\ACIDE URIQUE|R||201204122234|201204122234|||N|||||131184079\LE NAY CARUELLE Francis\\CMUR||||||20120412|
 if (segment_line.length()==0) return QString::null;
 QStringList list = segment_line.split( sep[0] , QString::KeepEmptyParts );
 if (list.size()<3) return QString::null;
 else               return list[2].remove('\\');
}

//-----------------------------------------  MAJ_Patients_PrisEnCharge -------------------------------------------
// pour chaque patient dans la table entrees sans date de prise en charge,
// on verifie s'il existe dans drtux > Maj du guid pat dans tables desentrees
// on verifie si un doc > date entree > Maj date de prise en charge
void C_BaseSigEntrees::MAJ_Patients_PrisEnCharge(  )
{
int ecart_heure_systeme = CApp::pCApp()->readUniqueParam("sigentrees session", "Ecart heure systeme serveur Sigems en minutes").toInt();

QString     requete,requeterub;
QString     majGuidPat;
CPatient    patient;

    requete = "SELECT " + m_SIG_ENT_PK              + ","           // 0
                        + m_SIG_ENT_GUID_PATIENT    + ","           // 1
                        + m_SIG_ENT_NUM_PAT_SIGEMS  + ","           // 2
                        + m_SIG_ENT_NOM             + ","           // 3
                        + m_SIG_ENT_PRENOM          + ","           // 4
                        + m_SIG_ENT_DATE_ENTREE                     // 5
            + " FROM "  + m_SIG_ENTREES_TBL_NAME
            + " WHERE " + m_SIG_ENT_DATE_PEC     + " = '"  DATE_NULLE "'";

   QSqlQuery query(requete , database() );
   while( query.isActive() && query.next() )
        {majGuidPat = query.value(1).toString();        // guid ou vide
       // Verif si patient existe.
       if (majGuidPat.length() < 1 )
        {
        patient.m_num_pat_sigems = query.value(2).toString();
        patient.m_nom            = query.value(3).toString();
        patient.m_prenom         = query.value(4).toString();
        majGuidPat               = IsThisPatientExist( patient );
        }
       // On ajuste la date d'entree avec Ecart des heures systeme Sigems - Medintux
       // on eleve 15 mn a l'heure d'entree au cas ou...
       QDateTime DateEntree = query.value(5).toDateTime();
       DateEntree = DateEntree.addSecs(ecart_heure_systeme * -60);
       QString sDateEntree = DateEntree.toString("yyyy-MM-dd hh:mm:ss");
        // recherche d'un doc dans rubriquehead
       QString majDatePEC = "";
       if (majGuidPat.length() > 0)      // uniquement si patient existe ds drtux
           {
            requeterub = "SELECT " + m_DOSS_RUB_HEAD_DATE            // 0
                       + " FROM "  + m_DOSS_RUB_HEAD_TBL_NAME
                       + " WHERE " + m_DOSS_RUB_HEAD_GUID   + " = '" + majGuidPat                 + "'"
                       + " AND "   + m_DOSS_RUB_HEAD_DATE   + " > '" + sDateEntree + "'";
           QSqlQuery queryRub(requeterub , database() );
            if ( queryRub.isActive() )
                { if (queryRub.next() )
                    {majDatePEC = queryRub.value(0).toString();
                    } // fin if patient pris en charge
                } // fin isActive
           } // fin if guid existe

       // maj de la date de prise en charge dans table entrees
       if (majGuidPat.length() > 0 || majDatePEC.length() > 0)
            {
             QString reqUpdate;
             reqUpdate = "UPDATE " + m_SIG_ENTREES_TBL_NAME
                       + " SET " ;
             if (majGuidPat.length() > 0)
                 reqUpdate +=  m_SIG_ENT_GUID_PATIENT  + " = '" + majGuidPat + "'";
             if (majDatePEC.length() > 0)
                {if (majGuidPat.length() > 0)
                    reqUpdate += " , ";
                 reqUpdate +=  m_SIG_ENT_DATE_PEC  + " = '" + majDatePEC + "'" ;
                } // fin if majDatePec > 0

             reqUpdate += " WHERE " + m_SIG_ENT_PK  + " = '" + query.value(0).toString()  + "'";
             QSqlQuery queryU(reqUpdate , database() );
            if (!queryU.exec() )
                 {CApp::pCApp()->outMessage(tr("\nERROR  : C_BaseSigEntrees::MAJ_Patients_PrisEnCharge() \n%1\nREQUEST : %2").arg(queryU.lastError().text(),reqUpdate).toAscii(),
                                               __FILE__, __LINE__  );
                 }
            } // fin if y'a une maj a faire
       } // fin while lecture table entrees.
}

//-----------------------------------------  read_Blob_Hprim -------------------------------------------
//  Relecture du blob Hprim pour affichage en brut
QString C_BaseSigEntrees::read_Blob_Hprim(QString pkEntree )
{
QString     requete;
QString     blobHprim = "";

    requete = "SELECT " + m_SIG_ENT_BLOB_HPR                        // 0
            + " FROM "  + m_SIG_ENTREES_TBL_NAME
            + " WHERE " + m_SIG_ENT_PK     + " = '" + pkEntree + "'";

   QSqlQuery query(requete , database() );
   while( query.isActive() && query.next() )
        {blobHprim = query.value(0).toString();        // blobHprim
        }
     return blobHprim;
}
