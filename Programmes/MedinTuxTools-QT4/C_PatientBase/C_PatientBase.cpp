/***************************** C_PatientBase.cpp **************************
*  #include "C_PatientBase.h"        QT4-ready                            *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "C_PatientBase.h"
#include "../CGestIni.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QString>

//-------------------------------------- C_PatientBase -----------------------------------------------
C_PatientBase::C_PatientBase(const QString   &confData,                        // definition des champs de la base de donnée (ceux de la liste des champs)
                             const QString   &params ,                         // parametres de connexion a la base de données
                             const QString   &pathIni,
                             QObject         *parent
                            )
        : C_BaseCommon(parent), C_PatientBase_Var( confData)
{   m_pathIni       =  pathIni;
    m_pQSqlQuery    = 0;
    QString errMess = "";
    //.............. se connecter a la base de donnees patient .............................
    if (BaseConnect(  params,                       // contenu du .ini
                      "PATIENT_BASE",               // label (ID unique) de la connexion a la base
                      "",                           // on indique pas le nom de la base de donnee afin de recuperer celui du .ini
                      &errMess,                     // on veut les messages de retour
                      "Connexion Patient Base"      // la section dans laquelle trouver l'entree 'Parametres = QMYSQL3 , DrTuxTest , root ,  , localhost , 3306'
                   )==0)
       {m_isValid = 0;
        return;
       }
    //.............. encoder et enregistrer le password encodé du ini .............................
    EncodePassword_InConnectionParam(params, "Connexion Patient Base", "Parametres", &errMess );
    CGestIni::Param_UpdateToDisk(m_pathIni, params);
    //...... ok on est bon ....................................
    m_isValid    = 1;
    m_pQSqlQuery = new QSqlQuery (QString::null , this->database());
    QSqlQuery(this->database());
    //..................... creer les tables indispensables ..................................
    //                      au cas ou elles seraient absentes
    createTablesDatees("VAR_contexte_biologique",     QStringList()<<"creat|4"<<"u_creat|8"
                                                                   <<"gb|4"<<"u_gb|8"
                                                                   <<"gr|4"<<"u_gr|8"
                                                                   <<"plq|4"<<"u_plq|8"
                                                                   <<"tp|4"<<"u_tp|8"
                                                                   <<"tck|4"<<"u_tck|8"
                                                                   <<"ka|4"<<"u_ka|8"
                                                                   <<"inr|4"<<"u_inr|8"
                                                                   <<"bnp|4"<<"u_bnp|8"
                                                                   <<"a_urique|4"<<"u_a_urique|8"
                                                                   <<"cpk|4"<<"u_cpk|8"
                                                                   <<"ldh|4"<<"u_ldh|8"
                                                                   <<"got|4"<<"u_got|8"
                                                                   <<"gpt|4"<<"u_gpt|8"
                                                                   <<"cholesterol|4"<<"u_cholesterol|8"
                                                                   <<"triglycerides|4"<<"u_triglycerides|8"
                                                                   <<"tsh|4"<<"u_tsh|8"
                                                                   <<"hb_glyquee|4"<<"u_hb_glyquee|8"
                                                                   <<"hb|4"<<"u_hb|8"
                                                                   );
    createTablesDatees("VAR_poids_taille",     QStringList()<<"poids|4" <<"taille|4");
    createTablesDatees("VAR_grossesse",        QStringList()<<"nombre foetus|4"<<"note|256");
    createTablesDatees("VAR_allaitement",      QStringList()<<"complement|100"<<"note|256");
    createTablesDatees("allergies",     QStringList()<<"libelle|80" <<"code|10"<<"code_type|10"<<"level|4"<<"classe|100"<<"note|256"<<"ald|1");
    createTablesDatees("antecedents",   QStringList()<<"libelle|200"<<"code|10"<<"code_type|10"<<"level|4"<<"classe|100"<<"note|256"<<"ald|1");
    createTablesDatees("prescriptions", QStringList()<<"date_redaction|20"    // yyyy-MM-dd hh:mm:ss
                                                     <<"libelle|100"
                                                     <<"dci_1|100"
                                                     <<"dci_2|100"
                                                     <<"dci_3|100"
                                                     <<"code_produit|10"      // code cip
                                                     <<"code_type|5"          // cip
                                                     <<"code_atc|7"
                                                     <<"code_indication|10" <<"type_code_indication|10"
                                                     <<"smr_global|4"       <<"smr_indication|4"
                                                     <<"voie|10"
                                                     <<"ald|2"
                                                     <<"unite_duree|20"
                                                     <<"substituable|2"
                                                     <<"u_de_prise|10"        // comprime cas ampoule spray
                                                     <<"q_par_up|10"          // 5
                                                     <<"u_qantite|10"         // mg
                                                     <<"q_up_par_u_temps|10"  // 4 (cp par jour)
                                                     <<"u_temps|2"            // s m h d w m y
                                                     <<"q_u_temps|10"         // duree
                                                     // <<"q_up_total|10"        //redondant car u_de_prise x q_up_par_u_temps x q_u_temps
                                                     <<"prix_par_up|10"       //
                                                     // <<"prix_total|10"        // redondant car  q_up_total x prix_par_up
                                                     <<"note|256"
                                                     <<"schema|256");
}

//-------------------------------- ~C_PatientBase ------------------------------------------------------
/*! \brief destructor  C_PatientBase
*/
C_PatientBase::~C_PatientBase()
{if (m_pQSqlQuery)     delete m_pQSqlQuery;
}
////////////////////////////////////////// IDENTIFICATION ///////////////////////////////////////////////////////
//-------------------------------------- guid_To_Pk_Name_Forename -----------------------------------------------
QString C_PatientBase::guid_To_Pk_Name_Forename(const QString &guid, QString *u_name /* = 0 */, QString *p_name /* = 0 */)
{   QString pk = "-1";
    QString sql_query = QString (" SELECT %1, %2, %3  \n"        // pk,name,forname
                                 " FROM   %4          \n"        // table_index
                                 " WHERE  %5 = '%6'   \n")       // guid = guid
                                .arg(m_DOSS_INDEX_PRIM_KEY,    // %1  -- 0
                                     m_DOSS_INDEX_NOM,         // %2  -- 1
                                     m_DOSS_INDEX_PRENOM,      // %3  -- 2
                                     m_DOSS_INDEX_TBL_NAME,    // %4
                                     m_DOSS_INDEX_GUID,        // %5
                                     guid                      // %6
                                    );
    m_pQSqlQuery->exec(sql_query);
    if ( ! m_pQSqlQuery->isActive() )
       { emit Sign_onError(*m_pQSqlQuery , tr("<font color=#ff0000><b>ERREUR  : guid_To_Pk_Name_Forename() </b></font>"), sql_query, __FILE__, __LINE__  );
         return pk;
       }

if (m_pQSqlQuery->next())
   {  pk                      =  m_pQSqlQuery->value(0).toString();
      if (u_name ) *u_name    =  m_pQSqlQuery->value(1).toString();
      if (p_name ) *p_name    =  m_pQSqlQuery->value(2).toString();
   }
 return pk;
}

//-------------------------------------- pk_to_BaseIdent -----------------------------------------------
int C_PatientBase::pk_to_BaseIdent(const QString &pk,
                                   QString *pm_DOSS_IDENT_NSS,
                                   QString *pm_DOSS_IDENT_SEX,
                                   QString *pm_DOSS_IDENT_JFNOM)
{QString sql_query  = "";
         sql_query +=         " SELECT " + m_DOSS_IDENT_NSS          +", \n"
                              "        " + m_DOSS_IDENT_SEX          +", \n"
                              "        " + m_DOSS_IDENT_JFNOM        +"  \n"
                              " FROM   " + m_DOSS_IDENT_TBL_NAME     +"  \n"
                              " WHERE  " + m_DOSS_IDENT_REF_PK       +" = '" + pk + "' \n";
 m_pQSqlQuery->exec(sql_query);
 if ( ! m_pQSqlQuery->isActive() )
    { emit Sign_onError(*m_pQSqlQuery , tr("<font color=#ff0000><b>ERREUR  : pk_to_BaseIdent() </b></font>"), sql_query, __FILE__, __LINE__  );
      return 0;
    }

 if (m_pQSqlQuery->next())
    {  if (pm_DOSS_IDENT_NSS )   *pm_DOSS_IDENT_NSS    =  m_pQSqlQuery->value(0).toDate().toString("yyyy-MM-dd");
       if (pm_DOSS_IDENT_SEX )   *pm_DOSS_IDENT_SEX    =  m_pQSqlQuery->value(1).toString();
       if (pm_DOSS_IDENT_JFNOM ) *pm_DOSS_IDENT_JFNOM  =  m_pQSqlQuery->value(2).toString();
    }
 return 1;

}
////////////////////////////////////////////// TABLES DATEES ///////////////////////////////////////////////////////////////////////////////
int C_PatientBase::createTablesDatees(const QString tableName, const QStringList fieldsList)
{   QString requete  = "";
            requete += QString ("CREATE TABLE %1 (\n"
                                " %1_pk BIGINT  NOT NULL AUTO_INCREMENT,\n"
                                " %1_owner         VARCHAR(40)  ,\n"
                                " %1_lang          VARCHAR(4)   ,\n"
                                " %1_status        VARCHAR(4)   ,\n"        // delete - active
                                " %1_human_guid    VARCHAR(40)  ,\n"
                                " %1_ref_human_pk  VARCHAR(40)  ,\n"
                                " %1_sign_user     VARCHAR(40)  ,\n"
                                " %1_user          VARCHAR(40)  ,\n"
                                " %1_datedeb       DATETIME     ,\n"
                                " %1_datefin       DATETIME     ,\n"
                                ).arg(tableName);
            //............ add specifics fields ...............................
            for (int i=0; i<fieldsList.size(); ++i)
                {   QStringList defLst =  fieldsList[i].split("|");    // field_name|size
                    requete           +=  QString(" %1_%2       VARCHAR(%3)  ,\n").arg(tableName, defLst[0], defLst[1]);
                }
            requete += QString("PRIMARY KEY (%1_pk))\n").arg(tableName);

            if (createTable(requete)==0) return 0;
            return 1;
}


