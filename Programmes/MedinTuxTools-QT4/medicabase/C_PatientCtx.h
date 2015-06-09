/***************************** C_PatientCtx.h *****************************
*  #include "C_PatientCtx.h"        QT4-ready                             *
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

#ifndef C_PATIENTCTX_H
#define C_PATIENTCTX_H
#include <QMap>
#include <QStringList>
#include <QObject>
#include <math.h>
#include <QDebug>
#include "../univers/C_Universal_Ident.h"
#include "../univers/C_Human_Properties.h"
class C_PatientBase;

//=========================================== C_LifeEvent =================================================
/*   K27|CIM|ulcere gastro duodenal|ALD|Medical(Gastro)|pas de commentaire|06-06-1956|
     N19|CIM|Insuffisance renale|ALD|Medical(uro nephro)|pas de commentaire|06-06-1956|
     W78|CISP|Grossesse||Medical(Obstetrique)|pas de commentaire|10-10-2012|
     Z32|CIM|Grossesse||Medical(Obstetrique)|pas de commentaire|10-10-2012|
     Z39.1|CIM|Allaitement||Medical(Puerperalite)|pas de commentaire|10-10-2012|
     |MD|Pratique du judo a haut niveau|Sport|Habitudes(Activite Sportive)|Attention aux produits dopants|10-10-2012|
*/
/*! \class  C_LifeEvent
 *  \brief  class to store a patient pathologie or life event
 */

class C_LifeEvent
{
public:

    //--------------------------------- C_LifeEvent -----------------------------------------------------
    C_LifeEvent();
    //--------------------------------- C_LifeEvent -----------------------------------------------------
    C_LifeEvent(const QStringList &atcdList, int index);
    //--------------------------------- C_LifeEvent -----------------------------------------------------
    C_LifeEvent(const QString &atcdStr);
    //--------------------------------- C_LifeEvent -----------------------------------------------------
    C_LifeEvent( const QString     &code          ,
                 const QString     &codeTyp       ,
                 const QString     &libelle       ,
                 const QString     &aldSport      ,
                 const QString     &medintuxClass ,
                 const QString     &comment       ,
                 const QString     &dateDeb       ,
                 const QString     &dateFin
                );
    //--------------------------------- setLifeEvent -----------------------------------------------------
    void  setLifeEvent(const QString &atcdStr);
    //--------------------------------- code -----------------------------------------------------
    void  setLifeEvent(const QString     &code          ,
                       const QString     &codeTyp       ,
                       const QString     &libelle       ,
                       const QString     &aldSport      ,
                       const QString     &medintuxClass ,
                       const QString     &comment       ,
                       const QString     &dateDeb       ,
                       const QString     &dateFin);
    //--------------------------------- code -----------------------------------------------------
    QString code()    const   {return m_code;}
    //--------------------------------- codeTyp -----------------------------------------------------
    QString codeTyp() const      {return m_codeTyp;}
    //--------------------------------- libelle -----------------------------------------------------
    QString libelle() const      {return m_libelle;}
    //--------------------------------- sport -----------------------------------------------------
    int sport(QString *pComment = 0) const
    {
        if (m_dateFin.length()==0                                     // si la date de fin n'est pas precisee l'ATCD est encore actif
            &&  m_aldSport.toLower().contains(QObject::tr("sport"))   // on explore le libelle au cas ou c'est le seul precise
           )
           {if (pComment)  *pComment = m_comment;
            QDate qd_deb = QDate::fromString(m_dateDeb,"dd-MM-yyyy");
            return qd_deb.daysTo(QDate::currentDate());
           }
        return 0;
    }
    //--------------------------------- vigilance -----------------------------------------------------
    int vigilance(QString *pComment = 0) const
    {
        if (m_dateFin.length()==0                                     // si la date de fin n'est pas precisee l'ATCD est encore actif
            &&  m_aldSport.toLower().contains(QObject::tr("vigi"))    // on explore le libelle au cas ou c'est le seul precise
           )
           {if (pComment)  *pComment = m_comment;
            QDate qd_deb = QDate::fromString(m_dateDeb,"dd-MM-yyyy");
            return qd_deb.daysTo(QDate::currentDate());
           }
        return 0;
    }
    //--------------------------------- ald -----------------------------------------------------
    int ald(QString *pComment = 0) const
    {
        if (m_dateFin.length()==0                                     // si la date de fin n'est pas precisee l'ATCD est encore actif
            &&  m_aldSport.toLower().contains(QObject::tr("ald"))     // on explore le libelle au cas ou c'est le seul precise
           )
           {if (pComment)  *pComment = m_comment;
            QDate qd_deb = QDate::fromString(m_dateDeb,"dd-MM-yyyy");
            return qd_deb.daysTo(QDate::currentDate());
           }
        return 0;
    }
    //--------------------------------- AldSportVigilanceProperty -----------------------------------------------------
    QString AldSportVigilanceProperty() const {return m_aldSport;}
    //--------------------------------- medintuxClass -----------------------------------------------------
    QString medintuxClass() const {return m_medintuxClass;}
    //--------------------------------- comment -----------------------------------------------------
    QString comment()   const    {return m_comment;}
    //--------------------------------- dateDeb -----------------------------------------------------
    QString dateDeb()   const    {return m_dateDeb;}
    //--------------------------------- dateFin -----------------------------------------------------
    QString dateFin()   const    {return m_dateFin;}
    //--------------------------------- insuffisance_renale -----------------------------------------------------
    int  insuffisance_renale(QString *pComment   = 0 )
    {       QString     code     = m_code;
            QString     libelle  = m_libelle.toLower();
            //......... faire le menage dans N17-N19 et ou  N18.15 ..............
            //          on ne retient que le code principal qui est le seul utile
            //          pour detecter l'insuffisance renale
            int pos   = code.indexOf('-');
            if (pos != -1) code = code.left(pos);
                pos   = code.indexOf('.');
            if (pos != -1) code = code.left(pos);

            int num   = code.mid(1).toInt();
            if (m_dateFin.length()==0                            // si la date de fin n'est pas precisee l'ATCD est encore actif
                && (   (code[0]=='N' && num >= 17 &&  num <= 19 )
                    ||  code   =="I13.2"   // et cardiopathie hypertensive avec ins cardiaque congestive
                    ||  code   =="I13.1"   // et cardiopathie hypertensive
                    ||  code   =="O90.4"   // aigue post partum
                    ||  code   =="O99.8"   // aigue post partum
                    ||  code   =="T79.5"   // apres ecrasement
                    ||  code   =="O08.4"   // apres etats classes O00 a O07
                    ||  code   =="P96.0"   // congenitale
                    ||  code   =="I12.0"   // hypertensive
                    ||  (  libelle.contains(QObject::tr("insuf"))         // on explore le libelle au cas ou toutes les autres methodes echouent
                           && (   libelle.contains(QObject::tr("r\303\251nale"))
                               || libelle.contains(QObject::tr("renale"))
                              )
                        )
                   )
               )
               {if (pComment)  *pComment = m_comment;
                QDate qd_deb = QDate::fromString(m_dateDeb,"dd-MM-yyyy");
                return qd_deb.daysTo(QDate::currentDate());                     // RETOURNER le nbr de jours de l'insuffisance renale
               }
            return 0;
    }
    //--------------------------------- allaitement -----------------------------------------------------
    int  allaitement(QString *pComment   = 0 )
    {
        if (m_dateFin.length()==0   // si la date de fin n'est pas precisee l'ATCD est encore actif
            && (     m_code   =="Z39.1"
                 ||  m_libelle.toLower().contains(QObject::tr("allaitement"))   // on explore le libelle au cas ou c'est le seul precise
               )
           )
           {if (pComment)  *pComment = m_comment;
            QDate qd_deb = QDate::fromString(m_dateDeb,"dd-MM-yyyy");
            return qd_deb.daysTo(QDate::currentDate());
           }
        return 0;
    }
    //--------------------------------- amenorrhee_weeks -----------------------------------------------------
    int amenorrhee_weeks()
    {return grossesse()/7;
    }

    //--------------------------------- grossesse -----------------------------------------------------
    int grossesse(QString *pComment  = 0 )
        {

         if (   m_dateFin.length()==0               // si la date de fin n'est pas precisee l'ATCD est encore actif
             && (   m_code=="W78"
                 || m_code.startsWith("Z32")       // grossesse
                 || m_code=="Z33"
                 || m_code=="Z34"
                 || m_code=="O99.0"
                 || m_code=="O30"
                 || m_code=="O31"
                 || m_code=="O43"
                 || m_code=="O99.4"
                 || m_code=="O24.9"
                 || m_code=="O24.0"
                 || m_code=="O24.1"
                 || m_code=="O24.2"
                 || m_code=="O24.3"
                 || m_code=="O24.0"
                 || m_libelle.toLower().contains(QObject::tr("rossesse"))   // on explore le libelle au cas ou c'est le seul precise
                )
            )
            {QDate qd_deb = QDate::fromString(m_dateDeb,"dd-MM-yyyy");      // si la date de grossesse correspond a une grossesse superieure a 10 mois on la considere terminee
             if (qd_deb.addMonths(10)>QDate::currentDate())
                {if (pComment)  *pComment = m_comment;
                 return  qd_deb.daysTo(QDate::currentDate());
                }
            }
         return 0;
        }

private:
    QString     m_code;
    QString     m_codeTyp;
    QString     m_libelle;
    QString     m_aldSport;
    QString     m_medintuxClass;
    QString     m_comment;
    QString     m_dateDeb;
    QString     m_dateFin;
};

//=========================================== C_PatientBioCtx =================================================
class C_PatientBioCtx
{
public:
    C_PatientBioCtx    ( const QString &bio_hprim  =  "" , const QString &transTables="", const QString &transTablesName="");
    void            set_bio_hprim(const QString &bio_hprim);
    QString         get_transTableCodesName();
    void            set_transTableCodes(const QString &tableConv, const QString &transTableCodesName, const QString &sep="|");
    void            set_transTableCodes(const QMap <QString , QString> &transTableCodes, const QString &transTableCodesName);
            QString code_Acte_To_Value (QString codeActe, QString *pUnit /* = 0 */, QString *isAnormal /* = 0 */, QString *valNormales /* = 0 */);
    static  QString code_Acte_To_Value (const QString &hprim, const QString &codeActe, QString *pUnit /* = 0 */, QString *isAnormal /* = 0 */, QString *valNormales /* = 0 */);
    static  QString code_Acte_To_Obx(const QString &code, const QString &hprim);
    static  QString obx_to_result (const QString &obx, const QString &sexe = "I", const QString &sep = "|", QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0 );

    QString         bio_hprim()      {return m_bio_hprim;}
    //....................... methodes pre parametrees avec codes NABM .......................................................................................
    QString creat    (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="592")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString uree     (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="591")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString t3       (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1206")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString t4       (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1207")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString tsh      (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1208")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString hba1c    (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1577")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString uricemie (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="532")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString tp       (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="126")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString inr      (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="127")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString tck      (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1127")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString bili_c   (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1601")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString bnp      (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1821")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString ka       (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1608")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString tropo    (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="7335")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString pcr      (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1804")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString vs       (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="1124")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString plq      (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const  QString &codeActe="1107")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
    QString hc       (QString *pUnit=0, QString *isAnormal=0, QString *valNormales=0, const QString &codeActe="2108")
                     { return code_Acte_To_Value (codeActe, pUnit , isAnormal , valNormales );
                     }
private:
    QString                  m_bio_hprim;
    QString                  m_fieldSep;
    QMap <QString , QString> m_transTableCodes;
    QString                  m_transTableCodesName;

};

//=========================================== C_PatientCtx =================================================
/*! \class  C_PatientCtx
 *  \brief  patient prescription context if guid is valid data's will be extract from patient attached to this guid, otherwise input datas will be used
 *  \note   date of birth is sometimes unknown. So it is always possible to determine aproximative or precise age. So when date of birth is unknown,
 *          age is used and date of birth set to invalid date (to indicate that only age is known)
 */

class C_PatientCtx:   public C_Human_Ident, public C_PatientBioCtx
{

public:
    enum atcd  {ONLY_ACTIV       = 1,
                ONLY_INACTIV     = 2,
                ALL_ATCD         = 3
               };
    C_PatientCtx();
    C_PatientCtx( const QString &dataXml, const QString &transTables="", const QString &transTablesName="");
    ~C_PatientCtx(){qDebug()<< "C_PatientCtx is deleted";}
    void                         unserialize(const QString &dataXml);
    static  C_PatientCtx        *unserialize(C_PatientCtx *pC_PatientCtx, const QString &dataXml);

    QMap<QString,QString>        pathologiesStringMap    (const QString &codeTyp = "CIM", int flag= C_PatientCtx::ALL_ATCD);
    QMap<QString,C_LifeEvent>    pathologiesEventLifeMap (const QString &codeTyp = "CIM", int flag= C_PatientCtx::ALL_ATCD);

    QList <C_LifeEvent>          lifeEventList()     {return m_lifeEventList;}
    QStringList                  allergiesList()     {return m_allergiesList;}
    QStringList                  pathologiesList()   {return m_pathologiesList;}
    QStringList                  cim10List()         {return m_cim10List;}
    QStringList                  prescriptionsList() {return m_prescriptionsList;}
    QStringList                  prescriptionsFondList()  {return m_prescriptionsFondList;}
    QStringList                  prescriptionsInterList() {return m_prescriptionsInterList;}
    QStringList                  list_cip_traitement_fond  () {return m_CIP_Fond;}
    QStringList                  list_cip_traitement_inter () {return m_CIP_Inter;}
    QString                      poids()             {return m_poids;}
    QString                      taille()            {return m_taille;}
    QString                      surface()           {return m_surface;}
    int                          ageInDays()         {return m_ageInDays;}
    double                       surfaceToDouble()   {return m_surfaceDouble;}
    double                       poidsToDouble()     {return m_poidsDouble;}
    double                       tailleToDouble()    {return m_tailleDouble;}
    static double                calc_surfaceMosteller(double taille, double poids) {return sqrt(taille * poids / 3600);}    // formule de Mosteller : Surface corporelle (m²) =  [Taille(cm) x Poids(kg) /3600]0,5
    double                       calc_surfaceToDouble()                             {return calc_surfaceMosteller(m_taille.toDouble(),m_poids.toDouble());}
    QString                      calc_surfaceToString()                             {return QString::number(calc_surfaceToDouble(),'f',2);}
    int                          sport(               QString *pComment  = 0 );
    int                          ald(                 QString *pComment  = 0 );
    int                          vigilance(           QString *pComment  = 0 );
    int                          grossesse(           QString *pComment  = 0 );
    int                          allaitement(         QString *pComment  = 0 );
    int                          insuffisance_renale( QString *pComment  = 0 );
    void        calc_surface();
    void        calc_AgeInDays();
    void        set_id(                    QString value)                  { set_guid(value);}
    void        set_id_type (              QString value)                  { m_id_type = value;}
    void        set_poids(           const QString &value);
    void        set_taille(          const QString &value);
    void        set_surface(         const QString &value);
    void        set_allaitement(     const QString &value);
    void        set_grossesse(       const QString &value);
    void        set_allergies(       const QString &value);
    void        set_antecedents(     const QString &value);
    void        set_prescriptions(   const QString &value);
    void        set_sportif(         const QString &date);
    void        set_allergiesList(   const QStringList &allergiesList)     { m_allergiesList     = allergiesList;}
    void        set_pathologiesList( const QStringList &pathologiesList)   { m_pathologiesList   = pathologiesList;}
    void        setLifeEventList(    const QStringList &pathologiesList);

private:
   QList <C_LifeEvent> m_lifeEventList;
   QStringList         m_allergiesList;
   QStringList         m_pathologiesList;
   QStringList         m_prescriptionsList;
   QStringList         m_prescriptionsFondList;
   QStringList         m_prescriptionsInterList;
   QStringList         m_CIP_Fond;
   QStringList         m_CIP_Inter;
   QString             m_id_type;
   QString             m_poids;
   QString             m_taille;
   QString             m_surface;
   double              m_surfaceDouble;
   double              m_poidsDouble;
   double              m_tailleDouble;
   int                 m_ageInDays;
   QStringList         m_cim10List;
};

#endif // C_PATIENTCTX_H
