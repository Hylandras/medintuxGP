/************************************** C_Vitale.cpp *********************************
 *                                                                                *
 *   #include"C_Vitale.h"                                                            *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/
#include"qdatetime.h"

#include"C_Vitale.h"
#include "CSesam_API_InfoGroupe.h"
#include "../MedinTuxTools/CGestIni.h"

//-------------------------------- C_Vitale -----------------------------------------------------
C_Vitale::C_Vitale(const QString &serializeData)
    :  m_Erreur("")
{ if (serializeData.length()) UnSerialize(serializeData);
}

//----------------------------------- Code_AiguillageSTS_ToList -----------------------------------------
QStringList C_Vitale::Code_AiguillageSTS_ToList()
{QStringList lst;
 lst.append(TR("4 - Service Tarification FFSA"));
 lst.append(TR("8 - Service Tarification CTIP"));
 lst.append(TR("M - Service Tarification FNMF"));
 lst.append(TR("R - Service Tarification A8"));
 return lst;
}

//----------------------------------- Code_AiguillageSTS_ToName ------------------------------------------)
QString C_Vitale::Code_AiguillageSTS_ToName( const QString code )
{if (code=="4") return TR("4 - Service Tarification FFSA");
 if (code=="8") return TR("8 - Service Tarification CTIP");
 if (code=="M") return TR("M - Service Tarification FNMF");
 if (code=="R") return TR("R - Service Tarification A8");
 return TR("Code_AiguillageSTS_ToName () code inconnu : ") + code;
}

//----------------------------------- CodeIndicTT_MUT_ToList -----------------------------------------
QStringList C_Vitale::CodeIndicTT_MUT_ToList()
{QStringList lst;
 lst.append(TR("00 - Pas de tiers-payant complémentaire autorisé"));
 lst.append(TR("01 - Tiers-payant complémentaire autorisé selon accord PS et AMC"));
 lst.append(TR("02 - Tiers-payant complémentaire autorisé selon réglementation"));
 lst.append(TR("62 - Bénéficiaire de la CMU Tiers-payant réglementaire. Pas d'éclatement possible"));
 lst.append(TR("71 - Bénéficiaire sortant de CMU complémentaire. Pas d'éclatement possible"));
 return lst;
}

//----------------------------------- CodeIndicTT_MUT_ToName ------------------------------------------)
QString C_Vitale::CodeIndicTT_MUT_ToName( const QString &codeIndic )
{switch (codeIndic.toInt())
        {case 0   :return TR ("00 - Pas de tiers-payant complémentaire autorisé");
         case 1   :return TR ("01 - Tiers-payant complémentaire autorisé selon accord PS et AMC");
         case 2   :return TR ("02 - Tiers-payant complémentaire autorisé selon réglementation");
         case 62  :return TR ("62 - Bénéficiaire de la CMU Tiers-payant réglementaire. Pas d'éclatement possible");
         case 71  :return TR ("71 - Bénéficiaire sortant de CMU complémentaire. Pas d'éclatement possible");
         default  :return TR ("CodeIndicTT_MUT_ToName() Code indicateur de traitement Mutuelle inconnu : ") + codeIndic;
        }
}

//----------------------------------- CodeIndicTT_AMC_ToList -----------------------------------------
QStringList C_Vitale::CodeIndicTT_AMC_ToList()
{QStringList lst;
 lst.append(TR("00 - Pas de DRE possible, pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("12 - DRE possible pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("13 - DRE possible Hors Tiers Payant, flux de rectification possible entre AMO et AMC"));
 lst.append(TR("22 - DRE possible en Tiers Payant, pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("23 - DRE possible en Tiers Payant, flux de rectification possible entre AMO et AMC"));
 lst.append(TR("32 - DRE possible en Tiers Payant et Hors Tiers Payant, pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("33 - DRE possible en Tiers Payant et Hors Tiers Payant, flux de rectification possible entre AMO et AMC"));
 lst.append(TR("60 - Bénéficiaire CMU complémentaire, Pas de DRE possible, pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("62 - Bénéficiaire CMU complémentaire, DRE possible, pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("63 - Bénéficiaire CMU complémentaire, DRE possible, flux de rectification possible entre AMO et AMC"));
 lst.append(TR("72 - Bénéficiaire sortant de CMU complémentaire, DRE possible, pas de flux de rectification possible entre AMO et AMC"));
 lst.append(TR("73 - Bénéficiaire sortant de CMU complémentaire, DRE possible, flux de rectification possible entre AMO et AMC"));
 return lst;
}

//----------------------------------- CodeIndicTT_AMC_ToName ------------------------------------------
QString C_Vitale::CodeIndicTT_AMC_ToName( const QString &codeIndic )
{switch (codeIndic.toInt())
        {case 0   :return TR ("00 - Pas de DRE possible, pas de flux de rectification possible entre AMO et AMC");
         case 12  :return TR ("12 - DRE possible pas de flux de rectification possible entre AMO et AMC");
         case 13  :return TR ("13 - DRE possible Hors Tiers Payant, flux de rectification possible entre AMO et AMC");
         case 22  :return TR ("22 - DRE possible en Tiers Payant, pas de flux de rectification possible entre AMO et AMC");
         case 23  :return TR ("23 - DRE possible en Tiers Payant, flux de rectification possible entre AMO et AMC");
         case 32  :return TR ("32 - DRE possible en Tiers Payant et Hors Tiers Payant, pas de flux de rectification possible entre AMO et AMC");
         case 33  :return TR ("33 - DRE possible en Tiers Payant et Hors Tiers Payant, flux de rectification possible entre AMO et AMC");
         case 60  :return TR ("60 - Bénéficiaire CMU complémentaire, Pas de DRE possible, pas de flux de rectification possible entre AMO et AMC");
         case 62  :return TR ("62 - Bénéficiaire CMU complémentaire, DRE possible, pas de flux de rectification possible entre AMO et AMC");
         case 63  :return TR ("63 - Bénéficiaire CMU complémentaire, DRE possible, flux de rectification possible entre AMO et AMC");
         case 72  :return TR ("72 - Bénéficiaire sortant de CMU complémentaire, DRE possible, pas de flux de rectification possible entre AMO et AMC");
         case 73  :return TR ("73 - Bénéficiaire sortant de CMU complémentaire, DRE possible, flux de rectification possible entre AMO et AMC");
         default  :return TR ("CodeIndicTT_AMCToName() Code service AMO inconnu : ") + codeIndic;
        }
}

//----------------------------------- CodeService_AMO_ToList -----------------------------------------
QStringList C_Vitale::CodeService_AMO_ToList()
{QStringList lst;
 lst.append(TR("00 - Pas de service AMO spécifique"));
 lst.append(TR("01 - CMU complémentaire gérée par une AMO"));
 lst.append(TR("02 - CMU complémentaire gérée par une AMC en convention ou délégation avec une AMO"));
 lst.append(TR("03 - CMU complémentaire gérée par une AMC"));
 lst.append(TR("10 - Sortant de CMU complémentaire"));
 return lst;
}

//----------------------------------- CodeServiceAMOToName ------------------------------------------
QString C_Vitale::CodeService_AMO_ToName( const QString &codeService )
{switch (codeService.toInt())
        {case 0  :return TR ("00 - Pas de service AMO spécifique");
         case 1  :return TR ("01 - CMU complémentaire gérée par une AMO");
         case 2  :return TR ("02 - CMU complémentaire gérée par une AMC en convention ou délégation avec une AMO");
         case 3  :return TR ("03 - CMU complémentaire gérée par une AMC");
         case 10 :return TR ("10 - Sortant de CMU complémentaire");
         default :return TR ("CodeServiceAMOToName() Code service AMO inconnu : ") + codeService;
        }
}

//----------------------------------- CodeQualiteToName ------------------------------------------
QString C_Vitale::CodeQualiteToName( const QString &codeQualite )
{switch (codeQualite.toInt())
        {case 0  :return TR ("Assuré");
         case 1  :return TR ("Ascendant, descendant, collatéraux ascendants");
         case 2  :return TR ("Conjoint");
         case 3  :return TR ("Conjoint divorcé");
         case 4  :return TR ("Concubin");
         case 5  :return TR ("Conjoint séparé");
         case 6  :return TR ("Enfant");
         case 8  :return TR ("Conjoint veuf");
         case 9  :return TR ("Autre ayant-droit");
         default :return TR ("CodeQualiteToName() Autre ayant-droit inconnu : ") + codeQualite;
        }
}

//----------------------------------- Mutnum_To_Complementaire ------------------------------------------
QString C_Vitale::Mutnum_To_Complementaire(const QString &dataCorrespondance,
                                           QString  mutnum,
                                           QString  codeRegime,
                                           QString  caisseGest,
                                           QString  centreGest,
                                           QString *pOrgaComplName)
{ QString strToMatch = mutnum+";"+codeRegime + caisseGest + centreGest;
  char *pt_toMatch = (char*) (const char*) strToMatch;
  char *endm       = pt_toMatch + strToMatch.length();
  char *pt_ligne   = (char*) (const char*) dataCorrespondance;
  while (*pt_ligne)
        {char *ptm   = pt_toMatch;
         char *ptl   = pt_ligne;
         while (ptm < endm && *ptl && (*ptm == *ptl ||*ptl == '?') )
               { ++ptl;
                 ++ptm;
               }
         ptl = CGestIni::GotoNextDebLigne(pt_ligne);                               // repérer la fin de ligne (debut de ligne suivante)
         //............ si match exact......................
         if (ptm==endm)
            {ptm                  = pt_ligne + 19;                                 // début du  N° organisme complémentaire
             endm                 = ptm;  while ( *endm && *endm !=';' ) ++endm;   // rechercher ';' séparateur de fin
             QString numOrgaCompl = QString::fromLatin1 ( ptm, endm-ptm);
             if (pOrgaComplName)
                { ++endm;                                                          // passer le ';' aprés N° organisme complémentaire
                 *pOrgaComplName =  QString::fromLatin1 ( endm, ptl - endm);
                  pOrgaComplName->remove('\n');
                  pOrgaComplName->remove('\r');
                }
             return numOrgaCompl;
            }
         while ( *ptl && (*ptl==' ' || *ptl=='\t') ) ++ptl;                        // éliminer éventuelle bavures de début de ligne
         pt_ligne = ptl;
        }
  if (pOrgaComplName) *pOrgaComplName = TR("Pas de résultat pour MUTNUM = ") + mutnum + TR(" code orga = ") + strToMatch;
  return QString::null;
}

//----------------------------------- CodeRegimeToName ------------------------------------------
QString C_Vitale::CodeRegimeToName(const QString &dataCaisse, int avecNumero/* =1 */)
{ QString codeRegime = GetMember(10,101, 1);
  QString caisseGest = GetMember(11,101, 1);
  C_Organisme organisme(dataCaisse, codeRegime, caisseGest);
  if (organisme.m_Error.length()) return CodeRegimeToName(codeRegime, caisseGest, avecNumero);

  QString ret;
  if (avecNumero) ret +=  codeRegime + " ";
  ret += organisme.m_Name;
  if (caisseGest=="349" || organisme.m_CentreGestionnaire=="0349") ret += " (carte Test CNDA)";
  return TR(ret);
}

//----------------------------------- CodeRegimeToName ------------------------------------------
QString C_Vitale::CodeRegimeToName( int avecNumero/* =1 */)
{return CodeRegimeToName( GetMember(10,101, 1) , GetMember(11,101, 1) , avecNumero);
}

//----------------------------------- CodeRegimeToName ------------------------------------------
QString C_Vitale::CodeRegimeToName( const QString &codeRegime , const QString &caisseGest , int avecNumero/* =1 */)
{QString centreGest = GetMember(12,101, 1);
 QString ret;
 if (avecNumero) ret +=  codeRegime + " ";
 switch (codeRegime.toInt())
        {case 1 :
              if (caisseGest=="999")    ret += "GIE-SESAM-Vitale";
              else                      ret += "Régime général";
                                                                               break;
         case 2 :                       ret += "Régime agricole";              break;
         case 3:                        ret += "AMPI";                         break;
         case 4 :                       ret += "CPR SNCF";                     break;
         case 5 :                       ret += "RATP";                         break;
         case 6 :                       ret += "ENIM";                         break;
         case 7 :                       ret += "Régime minier";                break;
         case 8 :                       ret += "CNMSS";                        break;
         case 9 :                       ret += "CPM Banque de France";         break;
         case 10:                       ret += "CRPCEN";                       break;
         case 12:                       ret += "CCIP";                         break;
         case 14:                       ret += "Assemblée Nationale";          break;
         case 15:                       ret += "Sénat";                        break;
         case 16:                       ret += "Port Autonome de Bordeau";     break;
         case 90:                       ret += "CAVIMAC";                      break;
         case 91:                       ret += "MGEN";                         break;
         case 92:                       ret += "MGPTT";                        break;
         case 93:                       ret += "MGP";                          break;
         case 94:                       ret += "MFP";                          break;
         case 95:                       ret += "MNH";                          break;
         case 96:                       ret += "MNAM";                         break;
         case 99:                       ret += "Sections locales mutualistes"; break;
        }
  if (caisseGest=="349" || centreGest=="0349") ret += " (carte Test CNDA)";
  return TR(ret);
}

//----------------------------------- CodeRegimeToOrganismeDestinataire ------------------------------------------
QString C_Vitale::CodeRegimeToOrganismeDestinataire(QString &codeCentreInformatique, const QString &dataCaisses /* = "" */)
{QString codeRegime = GetMember(10,101, 1);
 QString caisseGest = GetMember(11,101, 1);
 QString centreGest = GetMember(12,101, 1);

 if (codeRegime == "01" && caisseGest == "999")                              // cartes de test
     codeCentreInformatique = "999";                                         // Code centre informatique
  else
     codeCentreInformatique = "000";
 switch (codeRegime.toInt())
        {case 1 :             // régime général
         case 5 :             // RATP
         case 6 :             // ENIM
         case 7 :             // Régime minier
         case 8 :             // CNMSS
         case 9 :             // CPM Banque de France
         case 10:             // CRPCEN
         case 14:             // Assemblée Nationale
         case 15:             // Sénat
         case 16:             // Port Autonome de Bordeau
         case 90:             // CAVIMAC
         case 92:             // MGPTT
                   return caisseGest;    // Caisse gestionnaire

         case 2 :             // Régime agricole       ( règle en page 19/75)
              if (codeRegime.endsWith ("5"))
                 { return codeRegime.left(2) + "1";
                 }
              else return caisseGest;    // Caisse gestionnaire
         case 4 :             // CPR SNCF
                   return QString("110");
         case 12:             // CCIP
                   return QString("001");
         case 91:             // MGEN
              if (  caisseGest=="751" && centreGest=="0501")
                   return QString("999");
              else return caisseGest;    // Caisse gestionnaire
         case 93:             // MGP
              if (  caisseGest=="751" ||
                    caisseGest=="781" ||
                    caisseGest=="911" ||
                    caisseGest=="921" ||
                    caisseGest=="931" ||
                    caisseGest=="941" ||
                    caisseGest=="951"
                 )
                   return centreGest;
              else
                   return caisseGest;
         case 94:             // MFP
              if (  caisseGest=="751" ||
                    caisseGest=="781" ||
                    caisseGest=="911" ||
                    caisseGest=="921" ||
                    caisseGest=="931" ||
                    caisseGest=="941" ||
                    caisseGest=="951"
                 )
                 {  if (centreGest=="0511") return QString("0514");
                    else                    return centreGest;
                 }
              else                          return caisseGest;
         case 95:             // MNH
         case 96:             // MNAM
              if (  caisseGest=="751" ||
                    caisseGest=="781" ||
                    caisseGest=="911" ||
                    caisseGest=="921" ||
                    caisseGest=="931" ||
                    caisseGest=="941" ||
                    caisseGest=="951"
                 )
                   return QString("751");
              else
                   return caisseGest;
         default :
              if (  dataCaisses.length() )
                 {  C_Organisme organisme(dataCaisses, codeRegime, caisseGest);
                    if (organisme.m_Error.length()==0)
                       { return organisme.m_OrganismeDestinataire;
                       }
                    else
                       { return caisseGest;
                       }
                 }
              else return caisseGest;
         //case 3:              // AMPI
         //case 99:             // Sections locales mutualistes
        }
  return caisseGest;
}
//-------------------------------- GetAdresseAssure --------------------------------
QString C_Vitale::GetAdresseAssure(const QString& sep)
{       QString adresse="";
        int last_occurence = GetNbOccurences(104) + 1;
        int i;
        for (i=1; i<last_occurence; ++i)
            {if (GetMember(14,  104, i).toInt()==0)    //  qualite ayant-droit si zero c'est l'assure
                {adresse =     GetMember(4,  104, i) + sep +
                               GetMember(5,  104, i) + sep +
                               GetMember(6,  104, i) + sep +
                               GetMember(7,  104, i)  ;
                 return adresse.stripWhiteSpace();
                }
            }
        return "";
}

//-------------------------------- GetCodePostalVilleAssure --------------------------------
void  C_Vitale::GetCodePostalVilleAssure(QString* codePostal, QString* ville)
{       int last_occurence = GetNbOccurences(104) + 1;
        int i;
        for (i=1; i<last_occurence; ++i)
            {if (GetMember(14,  104, i).toInt()==0)    //  qualite ayant-droit si zero c'est l'assure
                {QString codePostLocalite = GetMember(8,  104, i);
                 if (codePostal) *codePostal =   codePostLocalite.left(5) ;
                 if (ville)      *ville      =   codePostLocalite.mid(6)  ;
                 return ;
                }
            }
}

//-------------------------------- GetCodePostalVilleAyanDroit --------------------------------
void  C_Vitale::GetCodePostalVilleAyanDroit(int occurence, QString* codePostal, QString* ville)
{                QString codePostLocalite = GetMember(8,  104, occurence);
                 if (codePostal) *codePostal =   codePostLocalite.left(5) ;
                 if (ville)      *ville      =   codePostLocalite.mid(6)  ;
}

//-------------------------------- Compare --------------------------------
// si nom prenom sont vides (valeurs par defaut) on ne les utilise pas pour la comparaison
int C_Vitale::Compare(int occurence, const QString &numSecu, const QString &dtNss, const QString &rangGem, const QString &nom,const QString &prenom)
{int differences = 0;
 if (nom.length()    && nom.stripWhiteSpace()    != GetMember(1,  104, occurence).stripWhiteSpace()) ++differences;
 if (prenom.length() && prenom.stripWhiteSpace() != GetMember(3,  104, occurence).stripWhiteSpace()) ++differences;
 if (numSecu != GetMember(8,  101, 1))                                                               ++differences;
 if (rangGem != GetRangGemellaire( occurence))                                                       ++differences;
 QString dtNss_V = GetDateNaissanceAyantDroit(occurence,0,"");
 if (dtNss_V.length()==0)                              // si date de naissance CV invalide et la qualite d'ayant droit de l'occurence est l'assuré c'est OK
    {if (GetMember(14,  104, occurence).toInt()!=0)                                                  ++differences;
    }
 else
    {if (ToNum(dtNss) != dtNss_V)                                                                    ++differences;
    }
 return differences;
}
//--------------------------------- ToNum ---------------------------------------------------------------------------------
QString C_Vitale::ToNum(QString in)
{for(uint i=0; i<in.length(); )
    {if (in.at(i)<'0' || in.at(i)>'9') in.remove(i,1);
        else ++i;
    }
    return in;
}
//-------------------------------- GetRangGemellaire --------------------------------
QString C_Vitale::GetRangGemellaire(int occurence)
{       QString rangGem  = GetMember(13,  104, occurence).stripWhiteSpace();
        if (rangGem.length()==0) rangGem = rangGem.prepend("01");
        if (rangGem.length()==1) rangGem = rangGem.prepend("0");
        if (rangGem=="00")       rangGem = "01";
        return rangGem;
}

//-------------------------------- GetDateNaissanceAyantDroit --------------------------------
QString C_Vitale::GetDateNaissanceAyantDroit(int occurence, QDate* pQDate, const QString& sep)
{QString qDteNss =    GetMember(12, 104, occurence);                   // 199306010000

 int jour, mois, annee;
 jour  = qDteNss.mid(6,2).toInt();
 mois  = qDteNss.mid(4,2).toInt();
 annee = qDteNss.mid(0,4).toInt();
 if (QDate::isValid ( annee, mois, jour ) ) {if (pQDate) pQDate->setYMD ( annee, mois, jour ); return qDteNss.mid(6,2)+sep+qDteNss.mid(4,2)+sep+qDteNss.mid(0,4);}
 else return "";
}

//-------------------------------- GetDateNaissanceAssure --------------------------------
QString  C_Vitale::GetDateNaissanceAssure(QDate* pQDate, const QString& sep)
{       int last_occurence = GetNbOccurences(104) + 1;
        int i;
        for (i=1; i<last_occurence; ++i)
            {if (GetMember(14,  104, i).toInt()==0)    //  qualite ayant-droit si zero c'est l'assure
                {QString qDteNss =    GetMember(12, 104, i);                   // 199306010000
                 //qDteNss =    qDteNss.mid(6,2)+qDteNss.mid(4,2)+qDteNss.mid(0,4);        // 01061993

                 int jour, mois, annee;
                 jour  = qDteNss.mid(6,2).toInt();
                 mois  = qDteNss.mid(4,2).toInt();
                 annee = qDteNss.mid(0,4).toInt();
                 if (QDate::isValid ( annee, mois, jour ) ) {if (pQDate) pQDate->setYMD ( annee, mois, jour ); return qDteNss.mid(6,2)+sep+qDteNss.mid(4,2)+sep+qDteNss.mid(0,4);}
                 else return "";
                }
            }
        return "";
}

//-------------------------------- GetNomPrenomAssure --------------------------------
QString  C_Vitale::GetNomPrenomAssure(const QString& sep, QString* p_nom, QString* p_prenom, QString* p_jfille)
{       int last_occurence = GetNbOccurences(104) + 1;
        int i;
        for (i=1; i<last_occurence; ++i)
            {if (GetMember(14,  104, i).toInt()==0)    //  qualite ayant-droit si zero c'est l'assure
                {QString    nom = GetMember(1,  104, i).stripWhiteSpace();   // recuperer son nom
                 QString prenom = GetMember(3,  104, i).stripWhiteSpace();   // recuperer son prenom
                 if (p_nom)     *p_nom     =   nom;      // recuperer son nom
                 if (p_prenom)  *p_prenom  =   prenom;   // recuperer son prenom
                 if (p_jfille)  *p_jfille  =   GetMember(2,  104, i).stripWhiteSpace();   // recuperer son nom de jeune fille
                 return nom + sep + prenom;
                }
            }
        return "";
}

//-------------------------------- GetNomPrenomAyanDroit --------------------------------
QString C_Vitale::GetNomPrenomAyanDroit(int occurence, const QString& sep, QString* p_nom, QString* p_prenom, QString* p_jfille)
{     QString    nom = GetMember(1,  104, occurence).stripWhiteSpace();   // recuperer son nom
      QString prenom = GetMember(3,  104, occurence).stripWhiteSpace();   // recuperer son prenom
      if (p_nom)     *p_nom    =   nom;   // recuperer son nom
      if (p_prenom)  *p_prenom =   prenom;   // recuperer son nom
      if (p_jfille)  *p_jfille =   GetMember(2,  104, occurence).stripWhiteSpace();   // recuperer son prenom
      return nom + sep + prenom;
}

//-------------------------------- GetAdresseAyanDroit --------------------------------
QString C_Vitale::GetAdresseAyanDroit(int occurence, const QString& sep)
{
        QString adresse =  GetMember(4,  104, occurence) + sep +
                           GetMember(5,  104, occurence) + sep +
                           GetMember(6,  104, occurence) + sep +
                           GetMember(7,  104, occurence);
        return adresse.stripWhiteSpace();
}

//-------------------------------- GetNbOccurences --------------------------------
int C_Vitale::GetNbOccurences(unsigned short  iDGroupeCourant)
{return GetNbOccurences(m_VitaleData, iDGroupeCourant);
}

//-------------------------------- GetNbOccurences --------------------------------
int C_Vitale::GetNbOccurences(const QString &data, unsigned short  iDGroupeCourant)
{int occur       = 0;
 int pos         = 0;
 QString section = QString("VIT_GR-") + QString::number(iDGroupeCourant) + "-OCC-";
 while ( (pos = data.find(section , pos+12)) != -1 )
    {++occur;
    }
 return occur;

}

//-------------------------------- GetMember --------------------------------
QString C_Vitale::GetMember(int memberIndex, unsigned short  iDGroupeCourant, int occur)
{QString section = QString("VIT_GR-") + QString::number(iDGroupeCourant) + "-OCC-" + QString::number(occur);
 QString member  = QString("m_Num-")  + QString::number(memberIndex);
 QString value   = "";
 if (CGestIni::Param_ReadParam(m_VitaleData, section, member, &value)==0) return value.replace("\\,",",").replace("\\;",";");
 return QString::null;
}

//-------------------------------- SetMember --------------------------------
int C_Vitale::SetMember(int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, size_t /*len_data*/, int occur)
{QString section = QString("VIT_GR-") + QString::number(iDGroupeCourant) + "-OCC-" + QString::number(occur);
 QString member  = QString("m_Num-")  + QString::number(memberIndex);
 QString value   = memberData;
 if (m_VitaleData.find(section)==-1)
    {m_VitaleData += "\r\n["+section+"]\r\n";
     m_VitaleData += TR("; Groupe : ") +
                     QString::number(iDGroupeCourant).rightJustify(4,'0') +
                     "  (" + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant)+ ")\r\n";
    }
 value.replace(",","\\,");
 value.replace(";","\\;");
 CGestIni::Param_WriteParam(&m_VitaleData, section, member, value , CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, memberIndex));
 return 0;
}

//-------------------------------- Serialize --------------------------------
QString C_Vitale::Serialize()
{if (m_Erreur.length())
    {if (m_VitaleData.find("[ERR_CODE]")==-1) m_VitaleData += "\r\n[ERR_CODE]\r\n";
     CGestIni::Param_WriteParam(&m_VitaleData, "ERR_CODE", "m_Erreur",           m_Erreur,          "1");
    }
 return m_VitaleData ;
}

//-------------------------------- UnSerialize --------------------------------
void C_Vitale::UnSerialize(const QString &serializeData)
{     m_VitaleData = serializeData;
}
