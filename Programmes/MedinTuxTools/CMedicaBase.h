/***************************************************************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   ro@vaio_ro                                                            *
 *                                                                         *
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
#ifndef  CMEDICA_BASE_H
#define  CMEDICA_BASE_H

#include <qsqldatabase.h>
#include <qlistview.h>
#include <qstringlist.h>
#include <qdom.h>
#include <qobject.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <math.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qapplication.h>

#define MEDICA_BASE     "MEDICA_BASE"
#define DATASEMP_BASE   "DATASEMP_BASE"
#define GET_BASE        "GET_BASE"

template <typename T>
inline const T &qMin(const T &a, const T &b) { if (a < b) return a; return b; }
template <typename T>
inline const T &qMax(const T &a, const T &b) { if (a < b) return b; return a; }
template <typename T>
inline const T &qBound(const T &min, const T &val, const T &max)
{ return qMax(min, qMin(max, val)); }

#define SKIP_BLANK_CAR(a)     while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define NEXT_LINE(a)          while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++
#define TR  QObject::tr

#define T4B_F0_CODE_PRODUIT   0      // f0 Code produit
#define T4B_F1_FORME          1      // f1 Forme ou nature
#define T4B_F2_LIBELLE        2      // f2 intitulé complet
#define T4B_F4_INTERACT_CLASS 3      // f4 Classe d'interactions
#define T4B_F5_FICHE_CARACT   4      // f5 Fiche de caractéristiques
#define T4B_F6_CHIM_CLASS     5      // f6 Classe chimique
#define T4B_F7_SEMP_CLASS     6      // f7 Famille SEMP
#define T4B_F8_LIBELLE_COURT  7      // f8 Intitulé abrégé
#define T4B_F9_CLINIQ_QUAL    8      // f9 Qualificatifs cliniques
#define T4B_FA_ATC_CLASS      9      // fA Classe ATC
#define T4B_FB_CODE_DOSAGE    10     // fB Code dosage
#define T4B_FC_DATE_MAJ       11     // fC Date de mise à jour de la fiche de caractéristiques
#define T4B_FP_ABTACT_FLAGS   12     // fP Flags pour les produits "abstraits
//===================================================== CPosologie =====================================================
class CPosologie
{

public:
    //................... enumerations ..............................................
    enum POSO_TYPE
    {   Nothing     = 0x0000,
        Textuelle   = 0x0001,
        Structuree  = 0x0002,
        AllMode     = 0x0003,
        Accessoires = 0x0004
    };
    //................... constructeur destructeur ..................................
    CPosologie(){};
    CPosologie(const QString& cip,
               const QString& pk,
               const QString& libelle,
               const QString& ageMin,
               const QString& ageMax,
               const QString& sexe,
               const QString& terrain,
               const QString& poidsMin,
               const QString& poidsMax,
               const QString& doseMin,
               const QString& doseMax,
               const QString& doseUnit,
               const QString& doseLimitMax,
               const QString& factCorpQu,
               const QString& factCorpUnit,
               const QString& periode,
               const QString& equiCoeff,
               const QString& unitCoeff,
               const QString& nbPrises,
               const QString& repart,
               const QString& divers,
               const QString& pendant       = "691200",        // 8 jours
               const QString& secabilite    = "1"  ,

               const QString& extraPk       = "",
               const QString& note          = "" ,
               int         posoAdjust       = 50 ,
               int         numOrdre         = 1  ,                       // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
               const QString& posoTextuelle = "" ,
               int         ald              = 0  ,
               int         posoType         = CPosologie::Structuree,    // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
               int         idTable          = 1
              )
               : m_MEDICA_POSO_CIP(cip),
                 m_MEDICA_POSO_PK(pk),
                 m_MEDICA_POSO_LIBELLE(libelle),
                 m_MEDICA_POSO_AGE_MIN(ageMin),
                 m_MEDICA_POSO_AGE_MAX(ageMax),
                 m_MEDICA_POSO_SEXE(sexe),
                 m_MEDICA_POSO_TERRAIN(terrain),
                 m_MEDICA_POSO_POIDS_MIN(poidsMin),
                 m_MEDICA_POSO_POIDS_MAX(poidsMax),
                 m_MEDICA_POSO_DOSE_MIN(doseMin),
                 m_MEDICA_POSO_DOSE_MAX(doseMax),
                 m_MEDICA_POSO_DOSE_UNIT(doseUnit),
                 m_MEDICA_POSO_DOSE_LIMIT_MAX(doseLimitMax),
                 m_MEDICA_POSO_FACT_CORP_QU(factCorpQu),
                 m_MEDICA_POSO_FACT_CORP_UNIT(factCorpUnit),
                 m_MEDICA_POSO_PERIODE(periode),
                 m_MEDICA_POSO_EQUI_COEFF(equiCoeff),
                 m_MEDICA_POSO_UNIT_COEFF(unitCoeff),
                 m_MEDICA_POSO_NB_PRISES(nbPrises),           // nombre de prises conseillées
                 m_MEDICA_POSO_REPART_PRISES(repart),         // description structurée de la répartition des prises
                 m_MEDICA_POSO_DIVERS(divers),                // description structurée de la répartition des prises
                 m_MEDICA_POSO_PENDANT(pendant),              // duree conseille de traitement
                 m_MEDICA_POSO_SECABILITE(secabilite),
                 m_MEDICA_POSO_EXTRA_PK(extraPk),             // description structurée de la répartition des prises
                 m_Note(note),
                 m_PosoAdjust(posoAdjust),
                 m_NumOrdre(numOrdre),                        // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
                 m_PosoTextuelle(posoTextuelle),
                 m_ALD(ald),
                 m_PosoType(posoType),                        // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
                 m_IdTable(idTable)
              {
              }
    CPosologie(const QString &xmlInput)
              {setDataFrom_XML_String( xmlInput );
              }
    ~CPosologie()
              {
              }
   void setData(const QString& cip,
                const QString& pk,
                const QString& libelle,
                const QString& ageMin,
                const QString& ageMax,
                const QString& sexe,
                const QString& terrain,
                const QString& poidsMin,
                const QString& poidsMax,
                const QString& doseMin,
                const QString& doseMax,
                const QString& doseUnit,
                const QString& doseLimitMax,
                const QString& factCorpQu,
                const QString& factCorpUnit,
                const QString& periode,
                const QString& equiCoeff,
                const QString& unitCoeff,
                const QString& nbPrises,
                const QString& repart,
                const QString& divers,
                const QString& pendant       = "691200",        // 8 jours
                const QString& secabilite    = "1",
                const QString& extraPk       = "" ,
                const QString& note          = "" ,
                int         posoAdjust       = 50 ,
                int         numOrdre         = 1  ,
                const QString& posoTextuelle = "" ,
                int         ald              = 0  ,
                int         posoType         = CPosologie::Structuree,   // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
                int         idTable          = 1
               )
               {m_MEDICA_POSO_CIP               = cip;
                m_MEDICA_POSO_PK                = pk;
                m_MEDICA_POSO_LIBELLE           = libelle;
                m_MEDICA_POSO_AGE_MIN           = ageMin;
                m_MEDICA_POSO_AGE_MAX           = ageMax;
                m_MEDICA_POSO_SEXE              = sexe;
                m_MEDICA_POSO_TERRAIN           = terrain;
                m_MEDICA_POSO_POIDS_MIN         = poidsMin;
                m_MEDICA_POSO_POIDS_MAX         = poidsMax;
                m_MEDICA_POSO_DOSE_MIN          = doseMin;
                m_MEDICA_POSO_DOSE_MAX          = doseMax;
                m_MEDICA_POSO_DOSE_UNIT         = doseUnit;
                m_MEDICA_POSO_DOSE_LIMIT_MAX    = doseLimitMax;
                m_MEDICA_POSO_FACT_CORP_QU      = factCorpQu;
                m_MEDICA_POSO_FACT_CORP_UNIT    = factCorpUnit;
                m_MEDICA_POSO_PERIODE           = periode;
                m_MEDICA_POSO_EQUI_COEFF        = equiCoeff;
                m_MEDICA_POSO_UNIT_COEFF        = unitCoeff;
                m_MEDICA_POSO_NB_PRISES         = nbPrises;           // nombre de prises conseillées
                m_MEDICA_POSO_REPART_PRISES     = repart;             // description structurée de la répartition des prises
                m_MEDICA_POSO_DIVERS            = divers;             // heu !!
                m_MEDICA_POSO_PENDANT           = pendant;            // heu !!
                m_MEDICA_POSO_SECABILITE        = secabilite;         // ajustement de la secabilite
                m_MEDICA_POSO_EXTRA_PK          = extraPk;
                m_Note                          = note;
                m_PosoAdjust                    = posoAdjust;
                m_NumOrdre                      = numOrdre;           // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
                m_PosoTextuelle                 = posoTextuelle;
                m_ALD                           = ald;
                m_PosoType                      = posoType;
                m_IdTable                       = idTable;             // indice de la table d'index
               }
   int setDataFrom_XML_String(const QString &xmlInput, int p=0)            // fonction de deserialisation de l'ordonnance
               {int curPos(p);

                m_MEDICA_POSO_CIP               = extractXmlValue(xmlInput, curPos, "cip");
                m_MEDICA_POSO_PK                = extractXmlValue(xmlInput, curPos, "pk");
                m_MEDICA_POSO_LIBELLE           = extractXmlValue(xmlInput, curPos, "libelle");
                //m_MEDICA_POSO_AGE_MIN           = extractXmlValue(xmlInput, curPos, "pk");
                //m_MEDICA_POSO_AGE_MAX           = ageMax;
                m_MEDICA_POSO_SEXE              = extractXmlValue(xmlInput, curPos, "sexe");
                m_MEDICA_POSO_TERRAIN           = extractXmlValue(xmlInput, curPos, "terrain");
                //m_MEDICA_POSO_POIDS_MIN         = poidsMin;
                //m_MEDICA_POSO_POIDS_MAX         = poidsMax;
                m_MEDICA_POSO_DOSE_MIN          = extractXmlValue(xmlInput, curPos, "doseMin");
                m_MEDICA_POSO_DOSE_MAX          = extractXmlValue(xmlInput, curPos, "doseMax");
                m_MEDICA_POSO_DOSE_UNIT         = extractXmlValue(xmlInput, curPos, "doseUnit");
                m_MEDICA_POSO_DOSE_LIMIT_MAX    = extractXmlValue(xmlInput, curPos, "doseLimitMax");
                m_MEDICA_POSO_FACT_CORP_QU      = extractXmlValue(xmlInput, curPos, "factCorpQU");
                m_MEDICA_POSO_FACT_CORP_UNIT    = extractXmlValue(xmlInput, curPos, "factCorpUnit");
                m_MEDICA_POSO_PERIODE           = extractXmlValue(xmlInput, curPos, "periode");
                m_MEDICA_POSO_EQUI_COEFF        = extractXmlValue(xmlInput, curPos, "equiCoeff");
                m_MEDICA_POSO_UNIT_COEFF        = extractXmlValue(xmlInput, curPos, "unitCoeff");
                m_MEDICA_POSO_NB_PRISES         = extractXmlValue(xmlInput, curPos, "nbPrises");
                m_MEDICA_POSO_REPART_PRISES     = extractXmlValue(xmlInput, curPos, "schemaPrise");
                m_MEDICA_POSO_DIVERS            = extractXmlValue(xmlInput, curPos, "divers");
                m_MEDICA_POSO_EXTRA_PK          = extractXmlValue(xmlInput, curPos, "extraPk");
                m_Note                          = extractXmlValue(xmlInput, curPos, "note");
                m_PosoAdjust                    = extractXmlValue(xmlInput, curPos, "posoAdjust").toInt();
                m_MEDICA_POSO_SECABILITE        = extractXmlValue(xmlInput, curPos, "secabilite");
                m_NumOrdre                      = extractXmlValue(xmlInput, curPos, "numOrdre").toInt();
                m_PosoTextuelle                 = extractXmlValue(xmlInput, curPos, "posoTextuelle");
                m_ALD                           = extractXmlValue(xmlInput, curPos, "ald").toInt();
                m_PosoType                      = extractXmlValue(xmlInput, curPos, "posoType").toInt();
                m_MEDICA_POSO_PENDANT           = extractXmlValue(xmlInput, curPos, "duree");
                m_IdTable                       = extractXmlValue(xmlInput, curPos, "IdTable").toInt();
                //.................. extraire le poids et age du terrain (a virer plus tard) ....................
                //           (sera inutile lorsque age et poids seront intégrés dans le terrain)
                int pos, deb, end;
                if ( (pos = m_MEDICA_POSO_TERRAIN.find("$AG")) != -1 )
                   {  deb  = pos;
                      pos += 4;                                              // passer le premier    ':'
                      end  = m_MEDICA_POSO_TERRAIN.find(':',pos);                          // trouver le deuxième  ':'
                      m_MEDICA_POSO_AGE_MIN = (m_MEDICA_POSO_TERRAIN.mid(pos, end-pos));
                      pos  = end + 1;   end =  m_MEDICA_POSO_TERRAIN.find(':',pos);         // trouver le troisième ':'
                      m_MEDICA_POSO_AGE_MAX = (m_MEDICA_POSO_TERRAIN.mid(pos, end-pos));
                      //........ virer la donnée: $AG:XX:XX:mois de la chaine terrrain ..............
                      end  = m_MEDICA_POSO_TERRAIN.find("mois", pos);
                      if (end != -1) m_MEDICA_POSO_TERRAIN.remove(deb, end-deb+4);
                   }
                if ( (pos  = m_MEDICA_POSO_TERRAIN.find("$PS")) != -1 )
                   {  deb  = pos;
                      pos += 4; // passer le premier ':'
                      end  = m_MEDICA_POSO_TERRAIN.find(':',pos);
                      m_MEDICA_POSO_POIDS_MIN = (m_MEDICA_POSO_TERRAIN.mid(pos, end-pos));
                      pos  = end + 1;     end =  m_MEDICA_POSO_TERRAIN.find(':',pos);
                      m_MEDICA_POSO_POIDS_MAX = (m_MEDICA_POSO_TERRAIN.mid(pos, end-pos));
                      //........ virer la donnée: $PS:XX:XX:Kg de la chaine terrrain ..............
                      end  = m_MEDICA_POSO_TERRAIN.find("Kg", pos);
                      if (end != -1) m_MEDICA_POSO_TERRAIN.remove(deb, end-deb+2);
                   }
                return curPos;
              }
   QString to_XML_String()                                            // fonction de serialisation de l'ordonnance
             {QString result("");
              //.............. pour la version 1 on stocke déjà l'age et poids dans terrain ....................
              //               on se met un peu en avance sur les versions futures !!!
              QString terrain("");
              terrain += "$AG:"+m_MEDICA_POSO_AGE_MIN  +":"+m_MEDICA_POSO_AGE_MAX  +":mois";
              terrain += "$PS:"+m_MEDICA_POSO_POIDS_MIN+":"+m_MEDICA_POSO_POIDS_MAX+":Kg";
              //.............. pour le reste il faut TOUT stocker au cas où la base de données .................
              //               ne comporeterai plus la posologie !!! (effacée ou autres surprises)
              result   += "<Posologie>\r\n";
              result   += "<numVersion>"      "1"                                             "</numVersion>\r\n";
              result   += "<cip>"            + m_MEDICA_POSO_CIP                             +"</cip>\r\n";
              result   += "<pk>"             + m_MEDICA_POSO_PK                              +"</pk>\r\n";
              result   += "<libelle>"        + StrtoXml( m_MEDICA_POSO_LIBELLE )             +"</libelle>\r\n";
              result   += "<sexe>"           + m_MEDICA_POSO_SEXE                            +"</sexe>\r\n";
              result   += "<terrain>"        + StrtoXml( m_MEDICA_POSO_TERRAIN + terrain )   +"</terrain>\r\n";
              result   += "<doseMin>"        + m_MEDICA_POSO_DOSE_MIN                        +"</doseMin>\r\n";
              result   += "<doseMax>"        + m_MEDICA_POSO_DOSE_MAX                        +"</doseMax>\r\n";
              result   += "<doseUnit>"       + StrtoXml( m_MEDICA_POSO_DOSE_UNIT )           +"</doseUnit>\r\n";
              result   += "<doseLimitMax>"   + m_MEDICA_POSO_DOSE_LIMIT_MAX                  +"</doseLimitMax>\r\n";
              result   += "<factCorpQU>"     + m_MEDICA_POSO_FACT_CORP_QU                    +"</factCorpQU>\r\n";
              result   += "<factCorpUnit>"   + StrtoXml( m_MEDICA_POSO_FACT_CORP_UNIT )      +"</factCorpUnit>\r\n";
              result   += "<periode>"        + m_MEDICA_POSO_PERIODE                         +"</periode>\r\n";
              result   += "<equiCoeff>"      + m_MEDICA_POSO_EQUI_COEFF                      +"</equiCoeff>\r\n";
              result   += "<unitCoeff>"      + StrtoXml( m_MEDICA_POSO_UNIT_COEFF )          +"</unitCoeff>\r\n";
              result   += "<nbPrises>"       + m_MEDICA_POSO_NB_PRISES                       +"</nbPrises>\r\n";
              result   += "<schemaPrise>"    + StrtoXml( m_MEDICA_POSO_REPART_PRISES )       +"</schemaPrise>\r\n";
              result   += "<divers>"         + StrtoXml( m_MEDICA_POSO_DIVERS )              +"</divers>\r\n";
              result   += "<extraPk>"        + m_MEDICA_POSO_EXTRA_PK                        +"</extraPk>\r\n";
              result   += "<note>"           + StrtoXml( m_Note )                            +"</note>\r\n";
              //................. données de réglage ......................................................................
              result   += "<posoAdjust>"     + QString::number(m_PosoAdjust)                 +"</posoAdjust>\r\n";
              result   += "<secabilite>"     + m_MEDICA_POSO_SECABILITE                      +"</secabilite>\r\n";
              result   += "<numOrdre>"       + QString::number(m_NumOrdre)                   +"</numOrdre>\r\n";
              result   += "<posoTextuelle>"  + m_PosoTextuelle                               +"</posoTextuelle>\r\n";
              result   += "<ald>"            + QString::number(m_ALD)                        +"</ald>\r\n";
              result   += "<posoType>"       + QString::number(m_PosoType)                   +"</posoType>\r\n";
              result   += "<duree>"          + m_MEDICA_POSO_PENDANT                         +"</duree>\r\n";
              result   += "<IdTable>"        + QString::number(m_IdTable)                    +"</IdTable>\r\n";
              result   += "</Posologie>\r\n";
              return result;
             }
   static QString StrtoXml(const QString &in)
             {QString out (in);
              out.replace(TR("&"),TR("&amp;"));  // en premier
              out.replace("<",TR("&lt;"));
              out.replace(">",TR("&gt;"));
              return out;
             }
   static QString XmltoStr(const QString &in)
             {QString out (in);
              out.replace(TR("&lt;") , "<");
              out.replace(TR("&gt;") , ">");
              out.replace(TR("&amp;"), TR("&")); // en dernier
              return out;
             }
  static QString extractXmlValue(const QString &xmlInput, int &curPos, const QString& s_tag)
             {QString tag(s_tag); tag.prepend('<');tag.append('>');
              int deb = xmlInput.find(tag, curPos);
              int end = deb;
              if (deb==-1)   // si echec  à partir de la position actuelle du curseur alors essayer depuis le debut (plus long mais independant des positions)
                 {//int tmp_curPos = 0;
                  //int tmp_deb    = xmlInput.find(tag, tmp_curPos);
                  //if (tmp_deb==-1) return QString("");
                  //tmp_deb += tag.length();
                  //tag.insert (1,'/' );
                  //end = xmlInput.find(tag, tmp_deb);
                  //if (end ==-1) {qDebug(QObject::tr(" EndTag: '") +tag+ QObject::tr("' not found"));  return QString("");}
                  //return XmltoStr( xmlInput.mid(tmp_deb, end-tmp_deb) );
                  return QString("");
                 }
              deb    += tag.length();
              tag.insert (1,'/' );
              end = xmlInput.find(tag, deb);
              if (end ==-1) {qDebug(QObject::tr(" EndTag: '") +tag+ QObject::tr("' not found"));  return QString("");}
              curPos  = end + tag.length();
              return XmltoStr( xmlInput.mid(deb, end-deb) );
             }
    //..................................... data ......................................................
public:
    //......................... données structurelles .............................................
    //                          provenant de la base médicamenteuse
    QString       m_MEDICA_POSO_CIP;                 // code CIP pour lequel cette posologie est définie
    QString       m_MEDICA_POSO_PK;                  // Clef primaire
    QString       m_MEDICA_POSO_LIBELLE;             // description textuelle de l'indication de cette posologie
    QString       m_MEDICA_POSO_AGE_MIN;             // age minimum pour laquelle cette posologie est définie
    QString       m_MEDICA_POSO_AGE_MAX;             // age maximum pour laquelle cette posologie est définie
    QString       m_MEDICA_POSO_SEXE;                // sexe: ( 'F' 'M' ou rien si indépendant du sexe) pour lequel cette posologie est définie
    QString       m_MEDICA_POSO_TERRAIN;             // Chaine de caractère structurée indiquant le terrain pour lequel cette posologie est définie
    QString       m_MEDICA_POSO_POIDS_MIN;           // poids minimum pour laquelle cette posologie est définie
    QString       m_MEDICA_POSO_POIDS_MAX;           // poids maximum pour laquelle cette posologie est définie
    QString       m_MEDICA_POSO_DOSE_MIN;            // dose minimum par unite de temps (m_MEDICA_POSO_PERIODE): conseillée
    QString       m_MEDICA_POSO_DOSE_MAX;            // dose maximum par unite de temps (m_MEDICA_POSO_PERIODE): conseillée
    QString       m_MEDICA_POSO_DOSE_UNIT;           // unité de la dose: m_MEDICA_POSO_DOSE_MIN et m_MEDICA_POSO_DOSE_MAX
    QString       m_MEDICA_POSO_DOSE_LIMIT_MAX;      // dose maximum autorisée pour m_MEDICA_POSO_DOSE_MIN et m_MEDICA_POSO_DOSE_MAX
    QString       m_MEDICA_POSO_FACT_CORP_QU;        // Qu de facteur corporel ou 0 ou rien si m_MEDICA_POSO_DOSE_MIN et m_MEDICA_POSO_DOSE_MAX n'en dépendent pas
    QString       m_MEDICA_POSO_FACT_CORP_UNIT;      // unite de facteur corporel: m2  Kg ou rien
    QString       m_MEDICA_POSO_PERIODE;             // durée en secondes pour laquelle est définie la posologie
    QString       m_MEDICA_POSO_EQUI_COEFF;          // coeff de conversion (est le nbr de: m_MEDICA_POSO_DOSE_UNIT contenu dans m_MEDICA_POSO_UNIT_COEFF )
    QString       m_MEDICA_POSO_UNIT_COEFF;          // unite de la forme convertie à partir des données de poids/facteur corporel/dose min et max
    QString       m_MEDICA_POSO_NB_PRISES;           // nombre de prises conseillées
    QString       m_MEDICA_POSO_REPART_PRISES;       // description structurée de la répartition des prises
    QString       m_MEDICA_POSO_DIVERS;              // texte libre un peu ce que l'on veut
    QString       m_MEDICA_POSO_PENDANT;             // durée conseillé du traitement
    QString       m_MEDICA_POSO_SECABILITE;          // secabilité du produit
    QString       m_MEDICA_POSO_EXTRA_PK;            // si present = Pk de la table de poso personnelle où se trouve aussi la donnée.
    QString       m_Note;                            // commentaire concernant cette posologie
    //......................... données fonctionnelles .............................................
    //                          non dépendantes de la base médicamenteuse mais des
    //                          ajustements de la prescription
    int           m_PosoAdjust;                      // ajustement par rapport à fourchette de doses initiales conseillées de 1 à 100 %
    int           m_NumOrdre;                       // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
    QString       m_PosoTextuelle;                   // posologie manuelle ajoutée par l'utilisateur (complément textuel)
    int           m_ALD;                             // à 0 si pas en rapport avec longue maladie
    int           m_PosoType;                        // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
    int           m_IdTable;                         // indice de la table d'index Datasemp: 1 = Specialités médicamenteuses pour le reste voir doc
};

//..................... Liste de la classe definie au dessus .........................................................
typedef QValueList<CPosologie> QPL_CPosologie;

//===================================== CMedicaBase ========================================================================
class CMedicaBase
{
 //............................... METHODES .........................................................................

 public:
     enum LIST_TYPE
    {   UP         = 0x0000,
        PA         = 0x0001,
        ST         = 0x0002,
        AD         = 0x0004
    };
      enum LIST_MODE
    {   WithPK              = 0x0000,
        JustLibelle         = 0x0001,
        NoClearList         = 0x0002,
        ClearList           = 0x0004,
    };
      enum VERS_MODE
    {   Data                = 0x0000,
        Index               = 0x0001,
        IndexAndData        = 0x0002
    };
      enum UPDATE_MODE
    {   Remove              = 0x0001,
        Insert              = 0x0002,
        Update              = 0x0004,
        AllUpdate           = 0x0007
    };
      enum PAGE_MODE
    {   Datasemp           = 0x0001,
        MedicaBase         = 0x0002,
        PrintableForm      = 0x0004,
        InteractiveForm    = 0x0008
    };
      enum flags  {NOT_LITERAL        = 0,
                   UP_QUANTITE        = 1,
                   UP_JUST_FRACTION   = 2,
                   DURATION           = 4,
                   SEQUENCE_HOUR      = 8,
                   MOMENT_MEAL        = 16,
                   INDICATION_LIST    = 32,
                   INDICATION_CODE    = 64,
                   INDICATION_TYPE    = 128,
                   ALL_LITERAL        = 31
                  };
      enum indicFlag  { CODE       = 0,
                        CODE_TYPE  = 1,
                        LIBELLE    = 2,
                        ALL_INDIC  = 3
                      };

 // Code de classification arborescente
 // Le premier caractère représente la "section" :
 // 1 = Spécialités ;
 // 2 = Diététiques ;
 // 3 = Vétérinaires ;
 // 4 = Parapharmacie ;
 // 5 = Accessoires ;
 // 6 = Divers (herboristerie, etc.) ;
 // 7 = Homéopathie...
      enum INDEX_TYPE
    {   MED_TBL_NAME           = 0x0001,
        DIET_TBL_NAME          = 0x0002,
        VETO_TBL_NAME          = 0x0003,
        PARA_TBL_NAME          = 0x0004,
        ACCS_TBL_NAME          = 0x0005,
        HERBO_TBL_NAME         = 0x0006,
        HOMEO_TBL_NAME         = 0x0007
    };

  //................................... configuration et connection de la base de données ..............................
   CMedicaBase();
   CMedicaBase(QString path_MedicaBase_ini,  QString path_MedicaBase_cfg);
   void Init_Base( QString &path_MedicaBase_ini,  QString &path_MedicaBase_cfg);
  ~CMedicaBase();
  QSqlDatabase*  BaseConnect(const char*    driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                             const char*    DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                             const char*    user,          // = "root"
                             const char*    pasword,       // = ""
                             const char*    hostname,      // = "localhost"
                             const QString &port,
                             const char*    baseName,
                             QString*       errMess=0);
  int            SetConfBase(const char* confFile, QString err);
  char          *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err);
  int            OutSQL_error( const QSqlQuery &cur, const char *messFunc =0, const char *requete =0,  QString *ret =0);
  QString        Utf8_Query(QSqlQuery &cur, int field);
  //............................ medica Base ........................................................
  int            save_TraitementEnCours(QString pk_doss,
                                        QString numGUID,
                                        QString user,
                                        QString signUser,
                                        QPL_CPosologie ordoList,
                                        QSqlDatabase *dataBaseDst );
  int            GotoDebug();
  void           Medica_SetBaseMode(int mode);          // pour mode limité (base Get)
  int            Medica_GetBaseMode();
  QString        Medica_GetDatasempVersion(VERS_MODE mode = CMedicaBase::Data);
  QString        Medica_SetDatasempVersion(QString version, VERS_MODE mode = CMedicaBase::Data);
  QString        Medica_PosologieToPrescriptionHtml(  CPosologie        &poso                ,
                                                      const QString     &spoids              ,
                                                      const QString     &staille             ,
                                                                int      id_list = -1        ,   // par defaut pas de gadjet de modification
                                                      QString      prodName      = ""        ,
                                                      QString      factCorpUnit  = ""
                                                   );
  QString        Medica_PosologieToSimpleText(  CPosologie    &poso         ,
                                                const QString &spoids       ,
                                                const QString &staille      ,
                                                QString prodName      = ""  ,   //   si présent évitera la recherche couteuse
                                                QString factCorpUnit  = ""      //  si présent évitera la recherche couteuse
                                                );
  float          Medica_ArondirUniteDePrise( float value, int secabilite  );
  float          Medica_ArondirUniteDePrise( QString value, int secabilite );
  QString        Medica_AdjustPluriel(const QString &unit , const float &f_dose);
  QString        Medica_GetCodeATC(QString cip,
                                   QString pkt4b  = QString::null );
  bool           Medica_CreateTablePosologies(const QString &table_name, int dropBefore=1);
  bool           Medica_CreateTableIndexProduit( const QString &table_name );
  bool           Medica_CreateTableSpecificationProduit( const QString &table_name );
  bool           Medica_CreateTableFormeProduit(const QString &table_name, int dropBefore =1);
  bool           Medica_CreateTableFormeProduit( const QString &table_name, QStringList &formes);
  bool           Medica_CreateTableATC(const QString &table_name);
  bool           Medica_InsertNewProduitInIndex( const QString &libelle,
                                                 const QString &cip,
                                                 const QString &pkt00,
                                                 const QString &pkt4b,
                                                 const QString &code_prod,
                                                 const QString &code_UV,
                                                 const QString &isGene,
                                                 const QString &isDispo);
   bool          Medica_InsertNewSpec          ( const QString &cip,
                                                 const QString &up_unit,
                                                 const QString &pa_unit,
                                                 const QString &pa_qu,
                                                 const QString &secabilite,
                                                 const QString &is_generic,
                                                 const QString &formStruct,
                                                 const QString &nbFormes,
                                                 const QString &prixBoite,
                                                 const QString &remboursementSS,
                                                 const QString &tableau,
                                                 const QString &labo,
                                                 const QString &atc
                                                );
   int           Medica_PercentError(const float &val1, const float &val2);
   int           Medica_GetUnitesList( LIST_TYPE list_type,        QStringList &list, int mode = CMedicaBase::WithPK);
   int           Medica_GetUnitesList( const QString &table_name,  QStringList &list, int mode = CMedicaBase::WithPK);
   int           Medica_UnitePriseAdd(const QString &table_name, const QString &libelle, const QString &pk);
   long          Medica_GetUnite(const QString &table_name, QStringList &unitList);

   void          Medica_GetPosologies(const QString &pk, CPosologie &posologie );
   long          Medica_GetPosologies(QString cip, QPL_CPosologie &pQPL_CPosologie );
   QString       Medica_GetHtmlPosoSpecPage(QString cip,
                                            QString name,
                                            QPL_CPosologie &list_CPosologie,
                                            int tableIndex = 1,
                                            QString pkt00  = QString::null,           // = QString::null
                                            QString pkt4b  = QString::null,           // = QString::null
                                            QString codePD = QString::null,           // = QString::null
                                            QString codeUV = QString::null            // = QString::null
                                           );
   QString       Medica_PosologieSpecPrisesToHtml(const CPosologie &poso);
   QString       Medica_PosologieSpecPrisesToHtml(const char *pt_i, long len);
   QString       Medica_OpToTxt(int op, const QString &article);
   char         *Medica_ExtractNumber(char *pt, QString &val);
   QString       Medica_MinutesToTime(const QString minutes);
   QString       Medica_PosologieTerrainToHtml(const CPosologie &poso);

   QString          Medica_Has_StructDataToOldXmlStruct(const QString &hasXml);
   QString          textTo_UP_Code( QString text );
   static int       Medica_dureeTotaleRenouvToSecond(QString cycle);
   static int       Medica_dureeTotaleToSecond(const QString &seq_days);
   static QString   Medica_sequenceToOldMinMaxPoso( const QString &sequence,  QString &q_min,  QString &q_max,  QString &nb_prises);
   static QString   Medica_sequence_timeToOldString(const QString &seq_time,  QString &q_min,  QString &q_max,  QString &nb_prises);
   // static QString   Medica_dureeTotaleRenouvToString(QString cycle,     int must_be_literal = 0 /* = C_PosologieGrammar::NOT_LITERAL */);
   // static QString   Medica_dureeTotaleToString(const QString &seq_days, int must_be_literal = 0 /* = C_PosologieGrammar::NOT_LITERAL */);
   // static QString   Medica_sequenceToString(int numSeq, const QString &sequence, int must_be_literal  /* = C_PosologieGrammar::NOT_LITERAL */, const QString &up_forme);
   // static QString   Medica_sequence_daysToString(int numSeq, const QString &seq_days, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */);
   // static QString   Medica_sequence_timeToString(const QString &seq_time, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */, const QString &_up_forme /* = "" */);

   static long      Medica_DiskDataSplitIn_HtmlData_StructData(const QString &txt, QString *stringDST_text /*=0 */, QString *stringDST_struct /*=0 */);
   static long      Medica_DiskDataSplitIn_HtmlData_HAS_StructData(const QString &txt, QString *stringDST_text /*=0 */, QString *stringDST_struct /*=0 */);

   QString       Medica_PosologieListToXMLOrdoStruct( QPL_CPosologie list_CPosologie, const QString &spoids, const QString &staille , const QString &lap_ordo);
   QString       Medica_PosologieToHtmlOrdo( QPL_CPosologie pQPL_CPosologie ,
                                             const QString &spoids          ,
                                             const QString &staille         ,
                                             const int mode = CMedicaBase::InteractiveForm); //CMedicaBase::InteractiveForm: affichage avec les pictogrammes de modification
   void          Medica_PosologieToHtmlSpec(const long num, const long id_list, const CPosologie &poso, QString &page, int tableIndex);
   QString       Medica_PosologieAgeToHtml(QString s_month);
   QString       Medica_PosologieTerrainToHtml(const QString &terrain);
   QString       Medica_PosologiePoidsToHtml(QString s_poids);
   QString       Medica_PosologiePeriodeToHtml(const QString s_secondes);
   QString       Medica_PosologieDureeToHtml(const QString s_secondes);
   QString       Medica_PosologieDureeJourToHtml(const QString jours);
   QString       Medica_PosologieFactCorpToHtml(const CPosologie &poso);
   QString       Medica_PosologieUP_UnitToHtml(const QString &unit ,const QString &dose);
   QString       Medica_PosologieUP_FloatValueToStrValue(float up);
   static long   Medica_PosologieListDeserialize(const QString &ordoStruct, QPL_CPosologie &list_CPosologie);
   static long   Medica_PosologieListSerialize(QString &ordoStruct,    QPL_CPosologie &list_CPosologie);
   static QString Medica_PosologieListSerialize(  QPL_CPosologie &list_CPosologie);
   long          Medica_PosologieMergePersoBase(int mode);
   bool          Medica_PosologieDeleteToBase(const QString &primKey, const QString &tableName );
   bool          Medica_PosologieDeleteToBaseByCIP( const QString &codeCIP, const QString &tableName );
   bool          Medica_PosologieUpdateToBase(const CPosologie &poso, const QString &tableName );
   bool          Medica_PosologieUpdateToBase (   const QString &primKey,
                                                  const QString &libelle,
                                                  const QString &cip,
                                                  const QString &ageMin,
                                                  const QString &ageMax,
                                                  const QString &sexe,
                                                  const QString &terrain,
                                                  const QString &poidsMin,
                                                  const QString &poidsMax,
                                                  const QString &doseMin,
                                                  const QString &doseMax,
                                                  const QString &doseUnitPk,
                                                  const QString &doseLimitMax,
                                                  const QString &factCorpQu,
                                                  const QString &factCorpUnitPk,
                                                  const QString &periode,
                                                  const QString &equiCoeff,
                                                  const QString &equiCoeffUnitPk,
                                                  const QString &nbPrises,
                                                  const QString &repart,
                                                  const QString &divers,
                                                  const QString &pendant,
                                                  const QString &secabilite,
                                                  const QString &extraPk,
                                                  const QString &tableName
                                               );
   bool          Medica_PosologieAppendToBase( const CPosologie &poso, const QString &tableName );
   bool          Medica_PosologieAppendToBase( const QString &libelle,
                                               const QString &cip,
                                               const QString &ageMin,
                                               const QString &ageMax,
                                               const QString &sexe,
                                               const QString &terrain,
                                               const QString &poidsMin,
                                               const QString &poidsMax,
                                               const QString &doseMin,
                                               const QString &doseMax,
                                               const QString &doseUnit,
                                               const QString &doseLimitMax,
                                               const QString &factCorpQu,
                                               const QString &factCorpUnit,
                                               const QString &periode,
                                               const QString &equiCoeff,
                                               const QString &equiCoeffUnit,
                                               const QString &nbPrises,
                                               const QString &repart,
                                               const QString &divers,
                                               const QString &pendant,
                                               const QString &secabilite,
                                               const QString &extraPk
                                           );
   bool          Medica_PosologieAppendToBase( const QString &libelle,
                                               const QString &cip,
                                               const QString &ageMin,
                                               const QString &ageMax,
                                               const QString &sexe,
                                               const QString &terrain,
                                               const QString &poidsMin,
                                               const QString &poidsMax,
                                               const QString &doseMin,
                                               const QString &doseMax,
                                               const QString &doseUnitPk,
                                               const QString &doseLimitMax,
                                               const QString &factCorpQu,
                                               const QString &factCorpUnitPk,
                                               const QString &periode,
                                               const QString &equiCoeff,
                                               const QString &equiCoeffUnitPk,
                                               const QString &nbPrises,
                                               const QString &repart,
                                               const QString &divers,
                                               const QString &pendant,
                                               const QString &secabilite,
                                               const QString &extraPk,
                                               const QString &tableName
                                            );
  QString        Medica_GetSecabilite( const QString &cip);
  QString        Medica_GetSpecificationsPrise(QString pk);
  int            Medica_GetSpecifications( QString cip, QStringList &qstringList);
  QString        Medica_GetSpecifications(        const QString &cip,
                                                  QString &up_unit,         // ok
                                                  QString &pa_unit,         // ok
                                                  QString &pa_qu,
                                                  QString &secabilite,      // ok
                                                  QString &is_generic,      // ok
                                                  QString &formStruct,      // ok
                                                  QString &nbPrises,        // ok
                                                  QString &prixBoite,       // ok
                                                  QString &remboursementSS, // ok
                                                  QString &tableau,         // ok
                                                  QString &labo,            // ok
                                                  QString &atc              // ok
                                                );
  QString        Medica_GetLibelleATC( const QString &codeATC);
  int            Medica_GetSchemaDePrise(const QString &pk_poso, QString &nbPrise, QString &schemaPrise);
  bool           Medica_IsThisFormeExist(QStringList &list, const QString &forme, int &next);
  long           Medica_GetLastPrimKey(const QString &table_name, const QString &primKeyName);
  void           Medica_FillQListView_ATC(QListView *pQListView );
  QListViewItem *Medica_SetQListViewOnATC(QListView *pQListView ,
                                          const QString &atc_code, int ListMustBeClosed  = 1 );
  long           Medica_GetMedicamentListByFamily( QListView *pQlistView          ,
                                                   const int  prodFamily     = 1  ,
                                                   const QString &name       = "A",
                                                   const QString &isGene     = "" ,
                                                   const QString &isDispo    = "" );
  long           Medica_GetMedicamentList( QListView *pQlistView,
                                           const QString &name     = "",
                                           const QString &isGene   = "",
                                           const QString &isDispo  = ""
                                          );
  QString        Medica_GetMedicamentNameByCIP( const QString &cip , int prodFamily = 1 );
  long           Medica_GetMedicamentListByCIP( QListView *pQlistView , const int  prodFamily , const QString &cip);
  int            Medica_GetMedicamentDatasempTablesCodesByCIP( const QString &cip, QStringList &codesList, int prodFamily = 1 );
  int            Medica_GetProductsListFrom_SQL_String(QListView *pQlistView, const QString &requete , int f= CMedicaBase::ClearList);
  long           Medica_GetMedicamentListByATC(QListView *pQlistView,
                                               const QString   &pATC,
                                               const QString &name     = "",
                                               const QString &isGene   = "",
                                               const QString &isDispo  = ""
                                              );
  QString        Medica_GetUnite(const QString &table_name, const QString &code);
  QString        Medica_GetUniteID(const QString &table_name, const QString &libelle);
  QString        Medica_GetNbRecord(const QString &table);
  char*          Medica_ExtractNextNumericalData(char* pt, char *end,  QString &val);
  char*          Medica_ExtractNextTextData(char* pt, char *end,  QString &val);
  long           Medica_GetProgress(){return m_Progress;};
  void           Medica_initComboDuree(QComboBox *comboBoxPendant);
  void           Medica_SetComboOnDuree(QComboBox *comboBoxPendant, const QString &duree, const char* defaultDuree = 0);
  float          Medica_GetComboCPrtDuree(QComboBox *comboBoxPendant);
  void           Medica_Stop(){m_Run = 0;};
  float          Medica_ExtractNumber(const QString text);
  float          Medica_dureeMenuTextToSecond( const QString &text );

  //............................ BaseGet ............................................................
  long           BaseGet_To_MedicaTux(QProgressBar *pQProgressBar);
  long           BaseGet_To_MedicaTuxIndex(QProgressBar *pQProgressBar);
  QString        BaseGet_QPA_Unit_to_Unit(const QString &paq_unit);
  QString        BaseGet_GetLibelleATC( const QString &codeATC, QString *tauxSS=0);

  //.................... Datasemp .........................................................
  void           Datasemp_FillQListView_ATC(QListView *pQListView );
  QString        Datasemp_GetComposition(const QString &codeProduit);
  QString        Datasemp_Update(const QString &fname,
                                 long &nb_sqlLine,
                                 UPDATE_MODE mode         = CMedicaBase::AllUpdate,     // = CMedicaBase::AllUpdate
                                 QSqlDatabase* pDataBase  = 0,                          // = 0 si pas egal zero alors les requetes SQL de mise à jour seront executées
                                 const char* fileOut      = 0                           // = 0 si pas egal zero alors le fichier SQL de mise à jour sera ecrit
                                );
  QString        Datasemp_GetBaseVersion( QString *numEdition, QString *packBCArrete, QString *packIRArrete);
  QString        Datasemp_GetDiskVersion(const QString &fname);
  QString        Datasemp_UpdateInTable(char* &pt, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase);
  QString        Datasemp_InsertInTable(char* &pt, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase);
  QString        Datasemp_DeleteInTable(char* &pt, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase);

  QString        Datasemp_DecodeSpecialCar(const QString &val);                           // fonction ultra non conforme depuis UTF8  URGENCE A LA REVOIR
  void           Datasemp_OutMessage( QTextEdit *logWidget, const QString &mess);
  int            Datasemp_MakeAllIndexProduit(QProgressBar *pQProgressBar,        QTextEdit *logWidget  = 0 );
  int            Datasemp_MakeIndex(int table_type, QProgressBar *pQProgressBar,  QTextEdit *logWidget  = 0 );
  QString        Datasemp_GetPA07(QString code_prod);
  QString        Datasemp_GetLibelleATC( const QString &codeATC);
  QString        Datasemp_GetSpecifications(      const QString &cip,
                                                  QString &up_unit,         // ok
                                                  QString &pa_unit,         // ok
                                                  QString &pa_qu,
                                                  QString &secabilite,      // ok
                                                  QString &is_generic,      // ok
                                                  QString &formStruct,      // ok
                                                  QString &nbPrises,        // ok
                                                  QString &prixBoite,       // ok
                                                  QString &remboursementSS, // ok
                                                  QString &tableau,         // ok
                                                  QString &labo,            // ok
                                                  QString &atc,             // ok
                                                  QString pkt00  = QString::null,           // = QString::null
                                                  QString pkt4b  = QString::null,           // = QString::null
                                                  QString codePD = QString::null,           // = QString::null
                                                  QString codeUV = QString::null            // = QString::null
                                                );
  QString        Datasemp_GetSecabilite(  const QString &cip,
                                          QString        pkt4b   = QString::null
                                       );
  QString        Datasemp_CIP_To_CodeProduit(const QString &cip);
  void           Datasemp_GetT4B_by_CodeProduit(QStringList &qstringList, QString codeProduit);
  QString        Datasemp_GetGenericRefState(const QString &codeProduit);
  QString        Datasemp_GetGenericState(const QString &codeProduit,  int f=1);
  QString        Datasemp_GetGenericStateEx(const QString &codeProduit, QSqlQuery  &query);
  QString        Datasemp_GetDisponibiliteState(const QString &codeUV, int f=1 );
  QString        Datasemp_GetDisponibiliteStateEx(const QString &codeUV, QSqlQuery  &query);
  void           Datasemp_GetT4B(QStringList &qstringList, QString pk);
  void           Datasemp_GetT00(QStringList &qstringList, QString pk);
  long           Datasemp_GetIndicationList(QListView *pQlistView,   const QString   &qstr_nom );
  long           Datasemp_GetSubstanceList(QListView  *pQlistView,   const QString   &qstr_nom );
  QString        Datasemp_GetField(const char* table,             // SELECT fieldToGet FROM table WHERE testField = testValue
                                   const char *testField,
                                   const char *testValue,
                                   const char *fieldToGet,
                                   QString    *pk  = 0 );
  QString        Datasemp_GetDosageT07( const QString &code_prod, int mode = 0, QString *pa_quIn = 0 , QString *pa_unitIn  = 0 , QString *substanceIn  = 0 );
  QString        Datasemp_GetDosage( const QString &codeDosage);
  QString        Datasemp_GetDosage(  const QString &codeDosage, const QString &code_prod , QString *pa_qu = 0 , QString *pa_unit  = 0 ,  QString *substance  = 0);
  void           Datasemp_SetHTMLFontProperty( const QString &family,
                                               const QString &titleSize,
                                               const QString &chapitreSize,
                                               const QString &paragraphSize,
                                               const QString &normalText);
  QString        Datasemp_GetMonographie(const QString &code_UV, const QString &pkt4B);
  QString        Datasemp_GetMonographieByCIP(const QString &codeCIP);
  void           Datasemp_Xml_to_html( QString& input,
                              QString  name   = QString::null
                            );
  void           Datasemp_Xml_ListChild(QString& input, const QDomElement &parentElement );
  void           Datasemp_IsExistClassTherapSempChild(QListView *pQListView, const QString &codeParent , QListViewItem *pQListViewParentItem=0);
  void           Datasemp_FillQListView_ClassTherapSemp(QListView *pQListView );
  QString        Datasemp_GetLibelleClassTherapSemp( const QString &code);
  void           Datasemp_AddRootClassTherapSempCode(const QString &code);
  void           Datasemp_GetAllClassTherapSemp_ChildCode(QListViewItem *pQListViewItem, QStringList &list);
  void           Datasemp_GetAllClassTherapSemp_ChildCode(QStringList &list, const QString &codeParent);
  long           Datasemp_GetMedicamentListByClassTheraVidal(       QListView *pQlistView ,
                                                                    QListViewItem *pQListViewItem,
                                                                    const QString &name           = "",
                                                                    const QString &isGene         = "", // = ""
                                                                    const QString &isDispo        = "", // = ""
                                                                    QProgressBar *pQProgressBar   = 0
                                                            );
  long           Datasemp_GetMedicamentListByCodeIndic( QListView     *pQlistView,
                                                        const QString &code_Indic,
                                                        const QString &isGene  = "", // = ""
                                                        const QString &isDispo = ""  // = ""
                                                   );
  long           Datasemp_GetMedicamentListBySubstance( QListView       *pQlistView,
                                                         const QString  &code_sbce,
                                                         const QString  &isGene  = "", // = ""
                                                         const QString  &isDispo = ""  // = ""
                                                       );
  long           Datasemp_GetCodesClassTherapVidal( const QString &codeProduit,
                                                    QStringList   &codesList);
  long           Datasemp_GetSubtancesListByCodeProd( QListView *pQlistView ,  const QString &code_prod );
  long           Datasemp_GetIndicationsListByCodeProd(QListView *pQlistView , const QString &code_prod);
  long           Datasemp_GetSubtanceList(const QString &code_prod, QStringList &list);
  void           Datasemp_SetQListViewOnClassTherapVidal( QListView *pQListView , QStringList &codesList , int ListMustBeClosed = 1  );
  void           Datasemp_GetDaily(const QString &atc,
                                   const QString &code_voie,
                                   QString &dailyDose,
                                   QString &dailyUnit,
                                   QString &substance);
  void           Datasemp_GetFicheEconomique(const QString &code_CIP,
                                                   QString &prixstr,
                                                   QString &labo,
                                                   QString &liste,
                                                   QString &tauxss ,
                                                   QString  codeUV  = QString::null
                                                 );
  long           Datasemp_ToMedicaTux(QProgressBar *pQProgressBar);
  QString        Datasemp_GetNbRecord(const QString table);


private:
  char*          strFindRev(char car,  const char* pt_i, long len);
  long           strFindNext(char car, const char* pt_i, long len);
  char          *strFindNext(char car, const char* pt_i, const char* end);
public:
  //......................................... DATA ........................................................

  //........................ MEDICA_TUX  Table index des produits .........................................
  //                         la structure d' l'index est commune
  //

  QString  m_MEDICA_INDEX_MED_TBL_NAME;         // nom  de la table index general des médicaments de medica
  QString  m_MEDICA_INDEX_DIET_TBL_NAME;        // nom  de la table index general des produits diététiques
  QString  m_MEDICA_INDEX_VETO_TBL_NAME;        // nom  de la table index general des produits vétérinaires
  QString  m_MEDICA_INDEX_PARA_TBL_NAME;        // nom  de la table index general des produits de parapharmacie
  QString  m_MEDICA_INDEX_ACCS_TBL_NAME;        // nom  de la table index general des produits TIPS et accessoires
  QString  m_MEDICA_INDEX_HERBO_TBL_NAME;       // nom  de la table index general des produits divers et herboristerie
  QString  m_MEDICA_INDEX_HOMEO_TBL_NAME;       // nom  de la table index general des produits homéopathiques
  QString  m_MEDICA_INDEX_PK;                   // Primary Key de la table
  QString  m_MEDICA_INDEX_LIBELLE;              // nom  du libelle (non du médicament)
  QString  m_MEDICA_INDEX_CIP;                  // code CIP  EST LA CLEF PRIMAIRE  !!
  //          si les 4 champs suivants sont vides alors le vidal n'est pas dispo
  //          chercher alors dans medica tux
  QString  m_MEDICA_INDEX_PK_T00;             // Vidal: reference de ce produit dans la table T00
  QString  m_MEDICA_INDEX_PK_T4B;             // Vidal: reference de ce produit dans la table T4B
  QString  m_MEDICA_INDEX_CD_PROD;            // Vidal: code Vidal PRODUIT
  QString  m_MEDICA_INDEX_CD_UV;              // Vidal: code Vidal Unite de vente
  QString  m_MEDICA_INDEX_IS_GENE;            // medicament generique ?
  QString  m_MEDICA_INDEX_IS_DISPO;           // produit disponible
  //........................ MEDICA_TUX  Table de SPECIFICATION .........................................
  QString  m_MEDICA_SPEC_TBL_NAME;           // nom de la table des specifications
  QString  m_MEDICA_SPEC_CIP;                // code CIP
  QString  m_MEDICA_SPEC_UP_UNIT;            // Primary Key de la table des formes et unites (comprimé ampoules etc ...)   UPUnit
  QString  m_MEDICA_SPEC_PA_UNIT;            // Unite    de principe actif  (mg ml etc...):                                PAUnit
  QString  m_MEDICA_SPEC_PA_QU;              // Quantite de principe actif par unité de prise:                             QPAbyUP
  QString  m_MEDICA_SPEC_SECABILITE;         // Secabilite:                                                                Secabilite
  QString  m_MEDICA_SPEC_IS_GENERIC;         // G  pour générique R pour réference d'un groupe ATC
  QString  m_MEDICA_SPEC_FORM_STRUC;         // Primary Key de la table des formes (solution buvable injectable etc ...)
  QString  m_MEDICA_SPEC_NB_FORME;           // Nb d'unite de prise (de forme) par boite
  QString  m_MEDICA_SPEC_PRIX_BOITE;         // prix de la boite
  QString  m_MEDICA_SPEC_REMBOURSEMENT;      // Remboursement secu
  QString  m_MEDICA_SPEC_TABLEAU;            // Tableau
  QString  m_MEDICA_SPEC_LABO;               // laboraoire pharmaceutique
  QString  m_MEDICA_SPEC_ATC;                // classe ATC
  //........................ MEDICA_TUX  Table des formes .........................................
  QString  m_MEDICA_FORME_UP_PERSO_TBL_NAME; // nom de la table des formes unite de prise personnelle
  QString  m_MEDICA_FORME_AD_PERSO_TBL_NAME; // nom de la table des forme des posologies  personnelle
  QString  m_MEDICA_FORME_UP_TBL_NAME;       // nom de la table des formes unite de prise
  QString  m_MEDICA_FORME_PA_TBL_NAME;       // nom de la table des formes principe actif (dosage)
  QString  m_MEDICA_FORME_ST_TBL_NAME;       // nom de la table des formes structurées
  QString  m_MEDICA_FORME_AD_TBL_NAME;       // nom de la table des forme des posologies
  QString  m_MEDICA_FORME_PK;                // Primary Key de la table des formes
  QString  m_MEDICA_FORME_LIBELLE;           // libelle de la forme
  //........................ MEDICA_TUX  Table des classes ATC .........................................
  QString  m_MEDICA_ATC_TBL_NAME;            // nom de la table des classes ATC
  QString  m_MEDICA_ATC_CODE;                // Code ATC
  QString  m_MEDICA_ATC_LIBELLE;             // libelle de la classe ATC
  //........................ MEDICA_TUX  Table des Posologies .........................................
  QString  m_MEDICA_POSO_PERSO_TBL_NAME;      // nom de la table des Posologies personnelle
  QString  m_MEDICA_POSO_TBL_NAME;            // nom de la table des Posologies
  QString  m_MEDICA_POSO_PK;                  // clef primaire
  QString  m_MEDICA_POSO_EXTRA_PK;            // si renseigné: clef primaire des données personnelles avec laquelle cette donnée a été créée
  QString  m_MEDICA_POSO_CIP;                 // code CIP
  QString  m_MEDICA_POSO_LIBELLE;             // Libelle précisant l'indication particulière au contexte
  QString  m_MEDICA_POSO_AGE_MIN;             // Age minimum d'application de la posologie
  QString  m_MEDICA_POSO_AGE_MAX;             // Age maximum d'application de la posologie
  QString  m_MEDICA_POSO_SEXE;                // Sexe d'application de la posologie
  QString  m_MEDICA_POSO_TERRAIN;             // chaine decrivant le terrain sous forme structurée
  QString  m_MEDICA_POSO_POIDS_MIN;           // Poids minimum d'application de la posologie
  QString  m_MEDICA_POSO_POIDS_MAX;           // Poids maximum d'application de la posologie
  QString  m_MEDICA_POSO_DOSE_MIN;            // dose minimum de la posologie pour cette indication
  QString  m_MEDICA_POSO_DOSE_MAX;            // dose maximum de la posologie pour cette indication
  QString  m_MEDICA_POSO_DOSE_UNIT;           // unite de la dose posologie pour cette indication
  QString  m_MEDICA_POSO_DOSE_LIMIT_MAX;      // dose maximum possible
  QString  m_MEDICA_POSO_FACT_CORP_QU;        // quantité d'unité de facteur corporel pour cette dose
  QString  m_MEDICA_POSO_FACT_CORP_UNIT;      // unité de facteur corporel pour cette dose
  QString  m_MEDICA_POSO_PERIODE;             // période en secondes pour lauqelle est exprimée cette posologie
  QString  m_MEDICA_POSO_EQUI_COEFF;          // coefficient pour transformer l'unite de prise 1 en unite de prise 2
  QString  m_MEDICA_POSO_UNIT_COEFF;          // unite deuxième forme de la dose posologie pour cette indication
  QString  m_MEDICA_POSO_NB_PRISES;           // nombre de prises conseillées
  QString  m_MEDICA_POSO_REPART_PRISES;       // description structurée de la répartition des prises
  QString  m_MEDICA_POSO_DIVERS;              // description structurée de la répartition des prises
  QString  m_MEDICA_POSO_PENDANT;             // durée de traitement conseillée par defaut
  QString  m_MEDICA_POSO_SECABILITE;          // secabilite

  //....données generales.........
  QString       m_DriverName ;
  QString       m_BaseName;
  QString       m_UserName;
  QString       m_PassWord;
  QString       m_HostName;
  QString       m_Port;
  //......................................... DATA ...............................................
  QSqlDatabase *m_DataBaseSrce;         // vidal data semp (si existe)
  QSqlDatabase *m_MedicaBase;           // base perso
  QSqlDatabase *m_DataGet_Base;         // base GET

  //......................................... DATA ...............................................
  QString      m_IniParam;

  private:
   int          m_MaxList;
   int          m_XmlDepth;
   QString      m_FontFamily;
   QString      m_FontTitleSize;
   QString      m_FontchapitreSize;
   QString      m_FontparagraphSize;
   QString      m_FontnormalTextSize;
   QStringList  m_FormeUPList;
   QStringList  m_FormeADList;
   QStringList  m_FormePAList;
   QStringList  m_FormeSTList;
   long         m_Progress;

   QString      m_InUseBase;
   int          m_ModeBase;    // CMedicaBase::MedicaBase ou CMedicaBase::Datasemp
   int          m_Run;
#ifdef _NOBODY_KNOW
   #include "Extra_CMedicaBase_H.cpp"
#endif //#ifdef _NOBODY_KNOW

};

#endif

