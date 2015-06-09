/***************************** C_PatientCtx.cpp ***************************
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

#include "C_PatientCtx.h"
#include "../C_PatientBase/C_PatientBase.h"
#include "../univers/C_DateTools.h"
#include "../CGestIni.h"
#include <QDebug>
#include <QObject>
//=========================================== C_LifeEvent =================================================
//--------------------------------- C_LifeEvent -----------------------------------------------------
C_LifeEvent::C_LifeEvent()
{setLifeEvent("","","","","","","","");
}
//--------------------------------- C_LifeEvent -----------------------------------------------------
C_LifeEvent::C_LifeEvent(const QStringList &atcdList, int index)
{if (index<atcdList.count())
    {setLifeEvent(atcdList.at(index));
    }
 else
    {setLifeEvent("","","","","","","","");
    }
}
//--------------------------------- C_LifeEvent -----------------------------------------------------
C_LifeEvent::C_LifeEvent(const QString &atcdStr)
{QStringList itemsList = atcdStr.trimmed().split('|');
 setLifeEvent(itemsList[0],
              itemsList[1],
              itemsList[2],
              itemsList[3],
              itemsList[4],
              itemsList[5],
              itemsList[6],
              itemsList[7]
            );
}
//--------------------------------- C_LifeEvent -----------------------------------------------------
C_LifeEvent::C_LifeEvent( const QString     &code          ,
                          const QString     &codeTyp       ,
                          const QString     &libelle       ,
                          const QString     &aldSport      ,
                          const QString     &medintuxClass ,
                          const QString     &comment       ,
                          const QString     &dateDeb       ,
                          const QString     &dateFin
            )
{setLifeEvent(code          ,
              codeTyp       ,
              libelle       ,
              aldSport      ,
              medintuxClass ,
              comment       ,
              dateDeb       ,
              dateFin);
}
//--------------------------------- setLifeEvent -----------------------------------------------------
void C_LifeEvent::setLifeEvent(const QString &atcdStr)
{   QStringList itemsList = atcdStr.trimmed().split('|');
    setLifeEvent(itemsList[0],
                 itemsList[1],
                 itemsList[2],
                 itemsList[3],
                 itemsList[4],
                 itemsList[5],
                 itemsList[6],
                 itemsList[7]
               );
}
//--------------------------------- setLifeEvent -----------------------------------------------------
void C_LifeEvent::setLifeEvent(const QString     &code          ,
                               const QString     &codeTyp       ,
                               const QString     &libelle       ,
                               const QString     &aldSport      ,
                               const QString     &medintuxClass ,
                               const QString     &comment       ,
                               const QString     &dateDeb       ,
                               const QString     &dateFin)
{   m_code          = code.trimmed();
    m_codeTyp       = codeTyp.trimmed();
    m_libelle       = libelle.trimmed();
    m_aldSport      = aldSport.trimmed();
    m_medintuxClass = medintuxClass.trimmed();
    m_comment       = comment.trimmed();
    m_dateDeb       = dateDeb.trimmed();
    m_dateFin       = dateFin.trimmed();
}

//=========================================== C_PatientBioCtx =================================================

//--------------------------------- C_PatientBioCtx -----------------------------------------------------
C_PatientBioCtx::C_PatientBioCtx(  const QString &bio_hprim       /* =""   */     // derniers resultats hprim du patient
                                 , const QString &transTables     /* =""   */     // tables de conversion NABM -- resultats labo
                                 , const QString &transTablesName /* =""   */     // nom du fichier de la table de conversion
                                )
{   set_bio_hprim(bio_hprim);
    m_transTableCodes.clear();
    m_transTableCodesName = "";
    set_transTableCodes(transTables, transTablesName);
}

//--------------------------------- set_bio_hprim -----------------------------------------------------
void    C_PatientBioCtx::set_bio_hprim(const QString &bio_hprim)
{m_bio_hprim    = bio_hprim;  //H|\^&~|
 if (bio_hprim.length()<6)
    {m_fieldSep = "";
    }
 else
    {
     int pos        = bio_hprim.indexOf(bio_hprim[1]);
     if (pos != -1) m_fieldSep = m_fieldSep.mid(1, pos-1);
     else           m_fieldSep = m_fieldSep.mid(1, 5);
    }
}

//--------------------------------- set_transTableCodes -----------------------------------------------------
void    C_PatientBioCtx::set_transTableCodes(const QMap <QString , QString> &transTableCodes, const QString &transTableCodesName)
{m_transTableCodes      = transTableCodes;
 m_transTableCodesName  = transTableCodesName;
}
//--------------------------------- set_transTableCodes -----------------------------------------------------
void    C_PatientBioCtx::set_transTableCodes(const QString &tableConv, const QString &transTableCodesName, const QString &sep  /* = "|" */)
{int         posNext        = 0;
 int         postNotBlank   = 0;
 QStringList fields;
 QString     line           = "";
 m_transTableCodesName      = "";
 m_transTableCodes.clear();
 while  ( (posNext = CGestIni::readNextLine(tableConv, posNext, line))  != tableConv.length() )       // on lit une ligne de texte
        { if ( !line.contains(sep))      continue;
          if ( (postNotBlank=CGestIni::gotoNextNotBlank(line, 0) != line.length()) &&
               line[postNotBlank]==';')  continue;
          fields = line.split(sep);
          m_transTableCodes[fields[0]]=fields[1];
        }
 if (m_transTableCodes.size()) m_transTableCodesName = transTableCodesName;
}

//--------------------------------- get_transTableCodesName -----------------------------------------------------
QString    C_PatientBioCtx::get_transTableCodesName()
{return m_transTableCodesName;
}

//--------------------------------- code_Acte_To_Value -----------------------------------------------------
QString C_PatientBioCtx::code_Acte_To_Value (QString codeActe, QString *pUnit /* = 0 */, QString *isAnormal /* = 0 */, QString *valNormales /* = 0 */)
{
    //............ verifier si cet acte ne doit pas etre traduit en un autre ...........................................
    //             en effet C_PatientBioCtx utilise ses propres codes (NABM) pour extraire les donnees OBX
    //             les labos n'utilisent surement pas ces codes, donc une table de correspondance permet de
    //             faire correspondre les codes NBAM utilises par les methodes pre parametrees aux codes
    //             retrouves dans l'OBX d'un labo X a partir de la table :
    //             m_transTableCodes QMap <QString C_PatientBioCtx_NBAMCode, QString laboXX_code>
    if (m_transTableCodesName.length())
       { QMap<QString, QString>::const_iterator    i = m_transTableCodes.find(codeActe);
         if (i != m_transTableCodes.end()) {codeActe = i.value();}
       }
    return code_Acte_To_Value (m_bio_hprim, codeActe, pUnit , isAnormal , valNormales );
}
//--------------------------------- code_Acte_To_Value -----------------------------------------------------
QString C_PatientBioCtx::code_Acte_To_Value (const QString &hprim, const QString &codeActe, QString *pUnit /* = 0 */, QString *isAnormal /* = 0 */, QString *valNormales /* = 0 */)
{   int         posNext        =  0;
    QString     line           = "";
    QString     fieldSep       = hprim.mid(1,5);
    QStringList obxFields;
    if (pUnit)             *pUnit       = "";
    if (isAnormal)         *isAnormal   = "";
    if (valNormales)       *valNormales = "";
    while  ( (posNext = CGestIni::readNextLine(hprim, posNext, line))  != hprim.length() )       // on lit une ligne de texte
    { line = line.trimmed();
      if ( ! line.startsWith("OBX"))        continue;
      obxFields = line.split(fieldSep[0]);  //  fieldSep =  |\^&~
      if (obxFields.size() < 4)             continue;
      if (obxFields[3].trimmed()!=codeActe) continue;
      if (pUnit)             *pUnit       = obxFields[6];
      if (valNormales)       *valNormales = obxFields[7];       // 32.0-36.0
      if (isAnormal)         *isAnormal   = obxFields[8];

      return obxFields[5];
    }
    return QString("");
}

//--------------------------------- obx_to_result -----------------------------------------------------
QString C_PatientBioCtx::obx_to_result (const QString &obx   ,
                                        const QString &      , /*sexe = "I" */
                                        const QString &sep   , /*     = "|" */
                                        QString *pUnit       , /* = 0 */
                                        QString *isAnormal   , /* = 0 */
                                        QString *valNormales   /* = 0 */
                                        )
{   QStringList              obxFields  = obx.split(sep);
    if (pUnit)              *pUnit      = "";
    if (isAnormal)         *isAnormal   = "";
    if (valNormales)       *valNormales = "";
    if (obxFields.size())
       {if (pUnit)             *pUnit       = obxFields[6];
        if (isAnormal)         *isAnormal   = obxFields[8];
        if (valNormales)       *valNormales = obxFields[7];
        return obxFields[5];
       }
   return QString("");
}

//--------------------------------- code_Acte_To_Obx -----------------------------------------------------
QString code_Acte_To_Obx(const QString &code, const QString &hprim)
{   int     posNext        =  0;
    QString fieldSep       = hprim.mid(1,5);
    QString line           = "";
    QStringList obxFields;
    while  ( (posNext = CGestIni::readNextLine(hprim, posNext, line))  != hprim.length() )       // on lit une ligne de texte
       {if ( ! line.startsWith("OBX")) continue;
        obxFields = line.split(fieldSep[0]);        //  m_fieldSep =  |\^&~
        if (! obxFields.size())        continue;
        if (obxFields[3].trimmed()==code) return line.remove('\r').remove('\n');
       }
   return QString("");
}

//=========================================== C_PatientCtx ====================================================
C_PatientCtx::C_PatientCtx()
{   m_id_type       = "";
    m_poids         = "";
    m_taille        = "";
    m_surface       = "";
    m_surfaceDouble = 0.0;
    m_poidsDouble   = 0.0;
    m_tailleDouble  = 0.0;
    m_ageInDays     = 0;
}

C_PatientCtx::C_PatientCtx(   const QString &dataXml
                            , const QString &transTables      /* ="" */
                            , const QString &transTablesName  /* ="" */
                          )
{ unserialize(this, dataXml);
  set_transTableCodes(transTables, transTablesName);
  for (int i = 0; i < m_lifeEventList.size(); ++i)
      {if ( m_lifeEventList[i].codeTyp()=="CIM")
          { if ( m_lifeEventList[i].dateFin().length() &&
                 QDate::fromString(m_lifeEventList[i].dateFin(),"dd-MM-yyyy")<QDate::currentDate()
               ) continue;
            m_cim10List.append(m_lifeEventList[i].code().remove('.'));
          } // endif ( m_lifeEventList[i].codeTyp()=="CIM")
     } // end for (int i = 0; i < m_lifeEventList.size(); ++i)
}

//--------------------------------- setLifeEventList -----------------------------------------------------
void  C_PatientCtx::setLifeEventList(const QStringList &pathologiesList)
{for (int i=0; i<pathologiesList.count();++i)
    {m_lifeEventList.append(C_LifeEvent(pathologiesList[i]));
    }
}

//--------------------------------- unserialize -----------------------------------------------------
void  C_PatientCtx::unserialize(const QString &dataXml)
{unserialize(this, dataXml);
}
//--------------------------------- unserialize -----------------------------------------------------
C_PatientCtx  *C_PatientCtx::unserialize(C_PatientCtx *obj, const QString &dataXml)
{
    int nextPos = 0;
    //............................. C_BDM_DrugListRecord ..............................
    obj->set_id(                 CGestIni::getXmlData("m_id",                  dataXml, &nextPos));
    obj->set_id_type(            CGestIni::getXmlData("m_id_type",             dataXml, &nextPos));
    obj->set_usual_name(         CGestIni::getXmlData("m_usual_name",          dataXml, &nextPos));
    obj->set_forename(           CGestIni::getXmlData("m_forename",            dataXml, &nextPos));
    obj->set_young_name(         CGestIni::getXmlData("m_young_name",          dataXml, &nextPos));
    obj->set_sexe(               CGestIni::getXmlData("m_sexe",                dataXml, &nextPos));
    QString dateFormat =         CGestIni::getXmlData("m_dateFormat",          dataXml, &nextPos);
    obj->set_ageOrDateOfBirth(   CGestIni::getXmlData("m_ageOrDateOfBirth",    dataXml, &nextPos), dateFormat);
    obj->set_poids(              CGestIni::getXmlData("m_poids",               dataXml, &nextPos));
    obj->set_taille(             CGestIni::getXmlData("m_taille",              dataXml, &nextPos));
    obj->set_bio_hprim(          CGestIni::getXmlData("m_bio_hprim",           dataXml, &nextPos));
    obj->set_allergies(          CGestIni::getXmlData("m_allergies",           dataXml, &nextPos));
    obj->set_antecedents(        CGestIni::getXmlData("m_antecedents",         dataXml, &nextPos));
    obj->set_prescriptions(      CGestIni::getXmlData("m_prescriptions",       dataXml, &nextPos));
    // doivent OBLIGATOIREMENT se faire apres  set_poids() et set_taille() age etc
    // car calculee d'apres eux
    obj->calc_surface();
    obj->calc_AgeInDays();

    return obj;
}

//--------------------------------- calc_AgeInDays -----------------------------------------------------
void    C_PatientCtx::calc_AgeInDays()
{   m_ageInDays =   get_dateOfBirth().daysTo ( QDate::currentDate() );
}
//--------------------------------- set_poids -----------------------------------------------------
void    C_PatientCtx::set_poids(const QString &poids)
{   m_poids       = poids;
    m_poidsDouble = poids.toDouble();
}
//--------------------------------- set_surface -----------------------------------------------------
void    C_PatientCtx::set_surface(const QString &value) // en m2
{   m_surface       = value;
    m_surface       = m_surface.replace(',','.');
    int p           = m_surface.indexOf('.');
    int partI       = 0;
    if (p!=-1)
       { partI = m_surface.left(p).toInt();
       }
    else
       { partI = m_surface.toInt();
       }
    if (partI>5)      // si superieur a 4 m2 on suppose que la surface est exprimee en cm2 on converti et corrige en m2
       { m_surfaceDouble = m_surface.toDouble()/10000.0;
         m_surface       = QString::number(m_surfaceDouble,'f',0);
       }
    else
       { m_surfaceDouble = m_surface.toDouble();
       }
}
//--------------------------------- set_taille -----------------------------------------------------
void    C_PatientCtx::set_taille(const QString &taille)    // doit etre exprimee en cm
{   m_taille       = taille;
    m_taille       = m_taille.replace(',','.');
    int p          = m_taille.indexOf('.');
    int partI      = 0;
    if (p!=-1)
       { partI = m_taille.left(p).toInt();
       }
    else
       { partI = taille.toInt();
       }
    if (partI<3)      // si exprimee en metres on corrige en cm
       { m_tailleDouble = taille.toDouble()*100;
         m_taille       = QString::number(m_tailleDouble,'f',0);
       }
    else
       { m_tailleDouble = taille.toDouble();
       }
}
//--------------------------------- calc_surface -----------------------------------------------------
void    C_PatientCtx::calc_surface()
{   m_surfaceDouble = calc_surfaceMosteller(m_tailleDouble, m_poidsDouble);
    m_surface       = QString::number(m_surfaceDouble,'f',2);
}
//--------------------------------- set_allergies -----------------------------------------------------
void    C_PatientCtx::set_allergies(const QString &value)
{m_allergiesList = value.trimmed().split("\n",QString::SkipEmptyParts);
 setLifeEventList(m_allergiesList);
}
//--------------------------------- set_antecedents -----------------------------------------------------
void    C_PatientCtx::set_antecedents(const QString &value)
{m_pathologiesList = value.trimmed().split("\n",QString::SkipEmptyParts);
 setLifeEventList(m_pathologiesList);
}
//--------------------------------- set_prescriptions -----------------------------------------------------
void   C_PatientCtx::set_prescriptions(const QString &value)
{ m_prescriptionsList  = CGestIni::getXmlDataList("OrdoLine", value);
  //................ separer traitement de fond de ceux intercurrents ......................
  for (int i=0; i<m_prescriptionsList.size();++i)
      { int nextPos       = 0;
        QString grammar   = m_prescriptionsList[i];
        QString status    = CGestIni::getXmlData("status",       grammar, &nextPos);
        QString gph_id    = CGestIni::getXmlData("gph_id",       grammar, &nextPos);
        QString gph_it    = CGestIni::getXmlData("gph_it",       grammar, &nextPos);
        QString gph_df    = CGestIni::getXmlData("gph_df",       grammar, &nextPos);   // date de fin
        //................ creer les listes de CIP ....................................
        if (status.contains('F')) { m_prescriptionsFondList  << grammar;  // y a tout non filtre sur les dates
                                    if ( gph_it=="CIP" &&
                                         QDateTime::currentDateTime() < QDateTime::fromString( gph_df , "dd-MM-yyyy hh:mm:ss")
                                       )
                                        m_CIP_Fond  << gph_id;            // creer les listes des CIP encore actifs
                                  }
        else                      { m_prescriptionsInterList << grammar;  // y a tout non filtre sur les dates
                                    if ( gph_it=="CIP" &&
                                         QDateTime::currentDateTime() < QDateTime::fromString( gph_df , "dd-MM-yyyy hh:mm:ss")
                                       ) m_CIP_Inter << gph_id;           // creer les listes des CIP encore actifs
                                  }
      }
}
//--------------------------------- insuffisance_renale -----------------------------------------------------
int    C_PatientCtx::insuffisance_renale(QString *pComment  /* = 0 */)
{   int           nb_jours  = 0;
    int           pos       = 0;
    QString         comment = "";
    QString       clairance = "";
    if (pComment) *pComment = "";
    for (int i=0; i<m_lifeEventList.count();++i)
        { nb_jours = m_lifeEventList[i].insuffisance_renale(&comment);
          if (pComment) *pComment = comment;
          pos = comment.indexOf("lairance");
          if (pos != -1)
             {pos += 8;
              while (pos<comment.length())
                   {if (comment.at(pos)=='.' ||
                        comment.at(pos)==',' ||
                        comment.at(pos)==' ' ||
                        (comment.at(pos)>='0' && comment.at(pos)<='9') ||
                        comment.at(pos)=='='
                       )
                       { clairance += comment.at(pos);
                       }
                    else
                       { break;
                       }
                    ++pos;
                   }
             }
         if (clairance.length()) return clairance.remove(" ").remove("=").replace(",",".").toInt();
        }

    //......... si pas d'indication d'insuff renale dans les ATCD ...............
    //          voir si il n' y en a pas une dans la biologie (creat)
    // - Chez l'homme  = 1.25 x Poids (kg) x (140-age) / creatinine (micro-mol/l)
    // - Chez la femme = 1.04 x Poids (kg) x (140-age) / creatinine (micro-mol/l)

    QString v_norm;
    QString value = creat(0, 0, &v_norm); // on a besoin de la valeur et des valeurs normales (separees par un tiret)
              pos = v_norm.indexOf('-');
    float      vMax, v;
    if  (pos   != -1)
        {vMax   = v_norm.mid(pos+1).toFloat();
         v      = value.toFloat();
         if (v >= vMax) return  -1;                                         // RETOURNER  -1 de l'insuffisance renale (nb de jours non connus)
        }
    return -1;                                                              // RETOURNER   0 pas d'insuffisance renale
}

//--------------------------------- sport -----------------------------------------------------
int    C_PatientCtx::sport(QString *pComment  /* = 0 */)
{    int           nb_jours  = 0;
     if (pComment) *pComment = "";
     for (int i=0; i<m_lifeEventList.count();++i)
         {nb_jours = m_lifeEventList[i].sport(pComment);
          if (nb_jours) return nb_jours;
         }
     return 0;
}
//--------------------------------- vigilance -----------------------------------------------------
int    C_PatientCtx::vigilance(QString *pComment  /* = 0 */)
{    int           nb_jours  = 0;
     if (pComment) *pComment = "";
     for (int i=0; i<m_lifeEventList.count();++i)
         {nb_jours = m_lifeEventList[i].vigilance(pComment);
          if (nb_jours) return nb_jours;
         }
     return 0;
}
//--------------------------------- ald -----------------------------------------------------
int    C_PatientCtx::ald(QString *pComment  /* = 0 */)
{    int           nb_jours  = 0;
     if (pComment) *pComment = "";
     for (int i=0; i<m_lifeEventList.count();++i)
         {nb_jours = m_lifeEventList[i].ald(pComment);
          if (nb_jours) return nb_jours;
         }
     return 0;
}
//--------------------------------- allaitement -----------------------------------------------------
int    C_PatientCtx::allaitement(QString *pComment  /* = 0 */)
{    int           nb_jours    = 0;
     if (pComment) *pComment   = "";
     if (get_sexe().toLower() != "f" ) return 0;
     for (int i=0; i<m_lifeEventList.count();++i)
         {nb_jours = m_lifeEventList[i].allaitement(pComment);
          if (nb_jours) return nb_jours;
         }
     return 0;
}
//--------------------------------- grossesse -----------------------------------------------------
int    C_PatientCtx::grossesse(QString *pComment /* = 0 */)
{int           nb_jours  = 0;
 if (pComment) pComment->clear();
 for (int i=0; i<m_lifeEventList.count();++i)
     {nb_jours = m_lifeEventList[i].grossesse(pComment);
      if (nb_jours) return nb_jours;
     }
 return 0;
}
//--------------------------------- pathologiesStringMap -----------------------------------------------------
/*   K27|CIM|ulcere gastro duodenal|ALD|Medical(Gastro)|pas de commentaire|06-06-1956|
     N19|CIM|Insuffisance renale|ALD|Medical(uro nephro)|pas de commentaire|06-06-1956|
     W78|CISP|Grossesse||Medical(Obstetrique)|pas de commentaire|10-10-2012|
     Z32|CIM|Grossesse||Medical(Obstetrique)|pas de commentaire|10-10-2012|
     Z39.1|CIM|Allaitement||Medical(Puerperalite)|pas de commentaire|10-10-2012|
     |MD|Pratique du judo a haut niveau|Sport|Habitudes(Activite Sportive)|Attention aux produits dopants|10-10-2012|
*/
QMap<QString,QString>    C_PatientCtx::pathologiesStringMap(const QString &codeTyp /* = "CIM" */, int flag /*= C_PatientCtx::ALL_ATCD*/)
{QMap<QString, QString> map;
 C_LifeEvent            lifeEvent;
 QString                code;
 for (int i=0; i<m_lifeEventList.count();++i)
    {lifeEvent = m_lifeEventList[i];
     code      = lifeEvent.code();
     if (code==codeTyp)
        {switch (flag)
           {case C_PatientCtx::ONLY_ACTIV :
                 {if (lifeEvent.dateFin().length()==0)         // pas de date de fin donc encore actif
                     {map[code]=lifeEvent.libelle()+"\n"+lifeEvent.comment()+"\n"+lifeEvent.medintuxClass();
                     }
                 }break;
            case C_PatientCtx::ONLY_INACTIV :
                 {if (lifeEvent.dateFin().length())            // date de fin presente donc inactif
                     {map[code]=lifeEvent.libelle()+"\n"+lifeEvent.comment()+"\n"+lifeEvent.medintuxClass();
                     }
                 }break;
            default :
                 {map[code]=lifeEvent.libelle()+"\n"+lifeEvent.comment()+"\n"+lifeEvent.medintuxClass();
                 }
           } //switch (flag)
        } //if (code==codeTyp)
    } //for (int i=0; i<m_pathologiesList.count();++i)
 return map;
}

//--------------------------------- pathologiesEventLifeMap -----------------------------------------------------
QMap<QString, C_LifeEvent>    C_PatientCtx::pathologiesEventLifeMap(const QString &codeTyp /* = "CIM" */, int flag /*= C_PatientCtx::ALL_ATCD*/)
{QMap<QString, C_LifeEvent> map;
 C_LifeEvent                lifeEvent;
 QString                    code;
 for (int i=0; i<m_lifeEventList.count();++i)
    {lifeEvent = m_lifeEventList[i];
     code      = lifeEvent.code();
     if (code==codeTyp)
        {switch (flag)
           {case C_PatientCtx::ONLY_ACTIV :
                 {if (lifeEvent.dateFin().length()==0)         // pas de date de fin donc encore actif
                     {map[code]=lifeEvent;
                     }
                 }break;
            case C_PatientCtx::ONLY_INACTIV :
                 {if (lifeEvent.dateFin().length())            // date de fin presente donc inactif
                     {map[code]=lifeEvent;
                     }
                 }break;
            default :
                 {map[code]=lifeEvent;
                 }
           } //switch (flag)
        } //if (code==codeTyp)
    } //for (int i=0; i<m_pathologiesList.count();++i)
 return map;
}

//--------------------------------- set_allaitement -----------------------------------------------------
void    C_PatientCtx::set_allaitement(const QString & /*date debut allaitement*/)
{
}
//--------------------------------- set_grossesse -----------------------------------------------------
void    C_PatientCtx::set_grossesse(const QString & /*date debut grossesse*/)
{
}
//--------------------------------- set_grossesse -----------------------------------------------------
void    C_PatientCtx::set_sportif(const QString & /*date pratique*/)
{
}
