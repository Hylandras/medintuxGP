/************************************** C_Cps.cpp *********************************
 *                                                                                *
 *   #include"C_Cps.h"                                                            *
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

#include"C_Cps.h"
#include "../MedinTuxTools/CGestIni.h"
#include "CSesam_API_InfoGroupe.h"
 
//-------------------------------- C_Cps --------------------------------
C_Cps::C_Cps(const QString &codePorteur, const QString &serializeData)
    : m_CodePorteur(codePorteur), m_Erreur("")
{ if (serializeData.length()) UnSerialize(serializeData);
}
//------------------------------------------ codeSpecialiteToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 2 Champ 14
QString        C_Cps::codeSpecialiteToString(int code)
{switch (code)
   {case  1  : return TR("Médecine générale");
    case  2  : return TR("Anesthésie réanimation");
    case  3  : return TR("Cardiologie");
    case  4  : return TR("Chirurgie générale");
    case  5  : return TR("Dermato-vénérologie");
    case  6  : return TR("Radiologie");
    case  7  : return TR("Gynécologie-obstétrique");
    case  8  : return TR("Gastro-entéro-hépatologie");
    case  9  : return TR("Médecine interne");
    case  10 : return TR("Neuro-chirurgie");
    case  11 : return TR("Oto-rhino-laryngologie");
    case  12 : return TR("Pédiatrie");
    case  13 : return TR("Pneumologie");
    case  14 : return TR("Rhumatologie");
    case  15 : return TR("Ophtamologie");
    case  16 : return TR("Chirurgie urologique");
    case  17 : return TR("Neuro-psychiatrie");
    case  18 : return TR("Stomatologie");
    case  19 : return TR("Chirurgie-dentaire");
    case  21 : return TR("Sage-femme");
    case  24 : return TR("Infirmier(e)");
    case  26 : return TR("Masseur-kinésithérapeute");
    case  27 : return TR("Pédicure-podologue");
    case  28 : return TR("Orthophoniste");
    case  29 : return TR("Orthoptiste");
    case  30 : return TR("Laboratoire d'analyses médicales");
    case  31 : return TR("Rééducation-réadaptation");
    case  32 : return TR("Neurologie");
    case  33 : return TR("Psychiatrie");
    case  35 : return TR("Néphrologie");
    case  36:  return TR("Chirurgie dentaire spécialité O.D.F");
    case  37 : return TR("Anatomo-cyto-pathologie");
    case  38 : return TR("Directeur de laboratoire médecin");
    case  39 : return TR("Laboratoire d'analyses de biologie médicale polyvalent");
    case  40 : return TR("Laboratoire d'analyses de biologie et de cytologie pathologique");
    case  41 : return TR("Chirurgie orthopédique");
    case  42 : return TR("Endocrinologie métabolisme");
    case  43 : return TR("Chirurgie infantile");
    case  44 : return TR("Chirurgie maxillo-faciale");
    case  45 : return TR("Chirurgie maxillo-faciale et stomatologie");
    case  46 : return TR("Chirurgie plastique et esthétique");
    case  47 : return TR("Chirurgie thoracique et cardio vasculaire");
    case  48 : return TR("Chirurgie vasculaire");
    case  49 : return TR("Chirurgie viscérale et digestive");
    case  50 : return TR("Pharmacien");
    case  51 : return TR("Pharmacien mutualiste");
    case  70 : return TR("Gynécologie médicale");
    case  71 : return TR("Hématologie");
    case  72 : return TR("Médecine nucléaire");
    case  73 : return TR("Oncologie médicale");
    case  74 : return TR("Oncologie radiothérapique");
    case  75 : return TR("Psychiatrie infantile");
    case  76 : return TR("Radiothérapie");
    case  77 : return TR("Obstétrique");
    case  78 : return TR("Génétique médicale");
    default  : return TR("code inconnu : ")+QString::number(code);
   }
}

//------------------------------------------ codeTypeCartePS_ToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 1 Champ 1
QString        C_Cps::codeTypeCartePS_ToString(int code)
{switch (code)
   {case  0  : return TR("Carte de Professionnel de Santé (CPS)");
    case  1  : return TR("Carte de Professionnel de Santé en Formation (CPF)");
    case  2  : return TR("Carte de Personnel d'Etablissement de Santé (CDE/CPE)");
    case  3  : return TR("Carte de Personnel Autorisé (CPA)");
    case  4  : return TR("Carte de Serveur Applicatif (CSA)");
    default  : return TR("code inconnu : ")+QString::number(code);
   }
}

//------------------------------------------ codeModeExercice_ToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 1 Champ 1
QString        C_Cps::codeModeExercice_ToString(int code)
{switch (code)
   {case  0  : return TR("Libéral, exploitant ou commerçant");
    case  1  : return TR("Salarié");
    case  4  : return TR("Remplaçant");
    case  7  : return TR("Non rétribué");
    default  : return TR("code inconnu : ")+QString::number(code);
   }
}

//------------------------------------------ codeType_ID_Nationale_ToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 1 Champ 2
QString        C_Cps::codeType_ID_Nationale_ToString(int code)
{switch (code)
   {case  0  : return TR("ADELI");
    case  1  : return TR("ADELI-Rang/N° de registre");
    case  2  : return TR("DRASS");
    case  3  : return TR("FINESS/N° de registre");
    case  4  : return TR("SIREN/N° de registre");
    case  5  : return TR("SIRET/N° de registre");
    case  6  : return TR("SIRET/Nom Application/Nom Serveur/N° Carte");
    case  7  : return TR("FINESS/Nom Application/Nom Serveur/N° Carte");
    case  8  : return TR("ADELI-Rang/Nom Application/Nom Serveur/N° Carte");
    case  9  : return TR("Etudiant");
    default  : return TR("code inconnu : ")+QString::number(code);
   }
}

//------------------------------------------ codeTypeDeCarte_ToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 2 Champ 4
QString        C_Cps::codeTypeDeCarte_ToString(int code)
{switch (code)
   {case  0  : return TR("Carte de Professionnel ou Personnel de Santé");
    case  1  : return TR("Carte patient/assuré");
    case  2  : return TR("Module de Sécurité");
    case  80 : return TR("Carte test de Professionnel ou Personnel de Santé");
    case  81 : return TR("Carte test patient/assuré");
    case  82 : return TR("Module de Sécurité de test");
    default  : return TR("code inconnu : ")+QString::number(code);
   }
}

//------------------------------------------ codeConvention_ToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 2 Champ 13
QString        C_Cps::codeConvention_ToString(int code)
{switch (code)
   {case  0  : return TR("Non conventionné");
    case  1  : return TR("Conventionné");
    case  2  : return TR("Conventionné avec dépassement");
    case  3  : return TR("Conventionné avec honoraires libres");
    default  : return TR("codeConvention_ToString() code inconnu : ") + QString::number(code);
   }
}
//------------------------------------------ codeSecteurActivite_ToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 2 Champ 4
QString        C_Cps::codeSecteurActivite_ToString(int code)
{switch (code)
   {case  10  : return TR("Etablissements publics de santé (y compris SAMU - SMUR)");
    case  11  : return TR("Hôpitaux militaires");
    case  16  : return TR("Etablis. de santé privés participant à l'exécution du service public (PSPH)");
    case  17  : return TR("Etablissements de santé privés à but non lucratif (non PSPH)");
    case  25  : return TR("Dispensaires de soins et autres établis sans hospitalisation, centres de santé");
    case  26  : return TR("Autres structures de soins relevant du Service de Santé des Armées");
    case  31  : return TR("Cabinets individuels");
    case  32  : return TR("Cabinets libéraux de groupe ou associations");
    case  33  : return TR("Exercice en société");
    case  35  : return TR("Transports sanitaires privés (sociétés d'ambulance)");
    case  37  : return TR("Entreprises d'intérim");
    case  41  : return TR("Etablissements de soins et de prévention");
    case  42  : return TR("Prévention et soins en entreprise (y compris médecine du travail)");
    case  43  : return TR("Santé scolaire et universitaire");
    case  45  : return TR("PMI, planification familiale");
    case  51  : return TR("Etablissements pour personnes handicapées ou en difficulté");
    case  53  : return TR("Etablissements pour personnes âgées");
    case  54  : return TR("Etablissements et services sociaux d'aide à la famille");
    case  55  : return TR("Etablissements de formation (écoles) et d'enseignement (universités)");
    case  56  : return TR("Etablissements de protection de l'enfance");
    case  57  : return TR("Etablissements d'hébergement et de réadaptation");
    case  58  : return TR("Recherche dans un organisme public ou à but non lucratif (INSERM, ORS, CNRS...)");
    case  61  : return TR("Expertise Sociétés d'Assurance privées");
    case  62  : return TR("Organismes de Sécurité Sociale (tous régimes)");
    case  63  : return TR("Praticien de l'aide sociale");
    case  65  : return TR("Ministère des Affaires Sociales et de la Santé et Services déconcentrés");
    case  66  : return TR("Collectivités territoriales");
    case  71  : return TR("Laboratoires d'analyses médicales extra-hospitalières");
    case  75  : return TR("Autres établis. sanitaires (CTS, stockage produits humains, centres antipoison)");
    case  81  : return TR("Production et commerce en gros de produits à usage médical");
    case  85  : return TR("Commerce de détail, biens à usage médical (lunetterie, orthopédie et prothèses)");
    case  86  : return TR("Pharmacies d'officine");
    case  91  : return TR("Autres secteurs d'activité (y compris activités non médicales)");
    case  92  : return TR("Secteur non défini");
    default   : return TR("code inconnu : ")+QString::number(code);
   }
}

//------------------------------------------ codeCiviliteToString ---------------------------------------------------------
// ce code provient aprés lecture de la carte CPS du Groupe 1 Champ 12
QString        C_Cps::codeCiviliteToString(int code)
{switch (code)
   {case  1  : return TR("Adjudant");
    case  2  : return TR("Amiral");
    case  3  : return TR("Aspirant");
    case  4  : return TR("Aumônier");
    case  5  : return TR("Capitaine");
    case  6  : return TR("Cardinal");
    case  7  : return TR("Chanoine");
    case  8  : return TR("Colonel");
    case  9  : return TR("Commandant");
    case  10 : return TR("Commissaire");
    case  11 : return TR("Conseiller");
    case  12 : return TR("Directeur");
    case  13 : return TR("Docteur");
    case  14 : return TR("Douanier");
    case  15 : return TR("Epoux(se)");
    case  16 : return TR("Evêque");
    case  17 : return TR("Général");
    case  18 : return TR("Gouverneur");
    case  19 : return TR("Ingénieur");
    case  20 : return TR("Inspecteur");
    case  21 : return TR("Lieutenant");
    case  22 : return TR("Madame");
    case  23 : return TR("Mademoiselle");
    case  24 : return TR("Maître");
    case  25 : return TR("Maréchal");
    case  26 : return TR("Médecin");
    case  30 : return TR("Monseigneur");
    case  31 : return TR("Monsieur");
    case  32 : return TR("Notre-Dame");
    case  33 : return TR("Pasteur");
    case  34 : return TR("Préfet");
    case  35 : return TR("Président");
    case  36 : return TR("Professeur");
    case  37 : return TR("Recteur");
    case  38 : return TR("Sergent");
    case  39 : return TR("Sous-Préfet");
    case  40 : return TR("Technicien");
    case  41 : return TR("Veuve");
    case  42 : return TR("Carte Serveur Applicatif");
    default  : return TR("code inconnu : ")+QString::number(code);
   }
}

//-------------------------------- GetCodePorteur --------------------------------
QString C_Cps::GetCodePorteur()
{return m_CodePorteur;
}
//-------------------------------- SetCodePorteur --------------------------------
void C_Cps::SetCodePorteur(const QString &codePorteur)
{ m_CodePorteur = codePorteur;
}
//-------------------------------- GetMember --------------------------------
QString C_Cps::GetMember(int memberIndex, unsigned short  iDGroupeCourant)
{if (iDGroupeCourant==1)
    {switch(memberIndex)
     {case 1: return m_TypePS;
      case 2: return m_TypeIdNat;      // 3 == RPPS
      case 3: return m_NIR;
      case 4: return m_Clef_NIR;
      case 5: return m_CodeCivilite;
      case 6: return m_Nom;
      case 7: return m_Prenom;
     }
    }
 else if (iDGroupeCourant==2)
    {switch(memberIndex)
     {case 1:  return m_NumLog;
      case 2:  return m_ModeExe;
      case 3:  return m_StatutExe;
      case 4:  return m_SectActivite;
      case 5:  return m_TypIdentStruct;       // 4 = RPPS
      case 6:  return m_NumIdentStruct;       // RPPS
      case 7:  return m_NumIdentStructClef;   // clef RPPS
      case 8:  return m_RaisonSocialeStruct;
      case 9:  return m_NumIdentFactuPs;
      case 10: return m_NumIdentFactuPsClef;
      case 11: return m_NumIdentPsRempla;
      case 12: return m_NumIdentPsRemplaClef;
      case 13: return m_CodeConvention;
      case 14: return m_CodeSpecialite;
      case 15: return m_CodeZoneTarif;
      case 16: return m_CodeZoneIk;
      case 17: return m_CodeAgr1;
      case 18: return m_CodeAgr2;
      case 19: return m_CodeAgr3;
      case 20: return m_HabilitationSignFact;
      case 21: return m_HabilitationSignLot;
     }
    }
 return QString::null;
}
//-------------------------------- SetMember --------------------------------
int C_Cps::SetMember(int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, size_t /*len_data*/)
{if (iDGroupeCourant==1)
    {switch(memberIndex)
     {case 1: m_TypePS       = memberData; break;
      case 2: m_TypeIdNat    = memberData; break;
      case 3: m_NIR          = memberData; break;
      case 4: m_Clef_NIR     = memberData; break;
      case 5: m_CodeCivilite = memberData; break;
      case 6: m_Nom          = memberData; break;
      case 7: m_Prenom       = memberData; break;
      default: return -1;
     }
    }
 else if (iDGroupeCourant==2)
    {switch(memberIndex)
     {case 1:  m_NumLog                = QString::number(memberData[0]); break;
      case 2:  m_ModeExe               = memberData; break;
      case 3:  m_StatutExe             = memberData; break;
      case 4:  m_SectActivite          = memberData; break;
      case 5:  m_TypIdentStruct        = memberData; break;
      case 6:  m_NumIdentStruct        = memberData; break;
      case 7:  m_NumIdentStructClef    = memberData; break;
      case 8:  m_RaisonSocialeStruct   = memberData; break;
      case 9:  m_NumIdentFactuPs       = memberData; break;
      case 10: m_NumIdentFactuPsClef   = memberData; break;
      case 11: m_NumIdentPsRempla      = memberData; break;
      case 12: m_NumIdentPsRemplaClef  = memberData; break;
      case 13: m_CodeConvention        = memberData; break;
      case 14: m_CodeSpecialite        = memberData; break;
      case 15: m_CodeZoneTarif         = memberData; break;
      case 16: m_CodeZoneIk            = memberData; break;
      case 17: m_CodeAgr1              = memberData; break;
      case 18: m_CodeAgr2              = memberData; break;
      case 19: m_CodeAgr3              = memberData; break;
      case 20: m_HabilitationSignFact  = memberData; break;
      case 21: m_HabilitationSignLot   = memberData; break;
      default: return -2;
     }
    }
 else return -3;
 return 0;
}

//-------------------------------- Serialize --------------------------------
QString C_Cps::Serialize()
{QString result;
 result += "\r\n[CPS_GR1]";
 result += TR("\r\n; Groupe : 0001") + "  (" + CSesam_API_InfoGroupe::NumGroupeToString(1)+ ")";

 result += "\r\n        m_TypePS = "       +  m_TypePS        + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,1);
 result += "\r\n        m_TypeIdNat = "    +  m_TypeIdNat     + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,2);
 result += "\r\n        m_NIR = "          +  m_NIR           + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,3);
 result += "\r\n        m_Clef_NIR = "     +  m_Clef_NIR      + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,4);
 result += "\r\n        m_CodeCivilite = " +  m_CodeCivilite  + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,5);
 result += "\r\n        m_Nom = "          +  m_Nom           + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,6);
 result += "\r\n        m_Prenom = "       +  m_Prenom        + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(1,7);

 result += "\r\n\r\n[CPS_GR2]";
 result += TR("\r\n; Groupe : 0002") + "  (" + CSesam_API_InfoGroupe::NumGroupeToString(2)+ ")";

 result += "\r\n        m_NumLog = "               +  m_NumLog               + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,1);
 result += "\r\n        m_ModeExe = "              +  m_ModeExe              + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,2);
 result += "\r\n        m_StatutExe = "            +  m_StatutExe            + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,3);
 result += "\r\n        m_SectActivite = "         +  m_SectActivite         + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,4);
 result += "\r\n        m_TypIdentStruct = "       +  m_TypIdentStruct       + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,5);
 result += "\r\n        m_NumIdentStruct = "       +  m_NumIdentStruct       + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,6);
 result += "\r\n        m_NumIdentStructClef = "   +  m_NumIdentStructClef   + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,7);
 result += "\r\n        m_RaisonSocialeStruct = "  +  m_RaisonSocialeStruct  + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,8);
 result += "\r\n        m_NumIdentFactuPs = "      +  m_NumIdentFactuPs      + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,9);
 result += "\r\n        m_NumIdentFactuPsClef = "  +  m_NumIdentFactuPsClef  + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,10);
 result += "\r\n        m_NumIdentPsRempla = "     +  m_NumIdentPsRempla     + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,11);
 result += "\r\n        m_NumIdentPsRemplaClef = " +  m_NumIdentPsRemplaClef + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,12);
 result += "\r\n        m_CodeConvention = "       +  m_CodeConvention       + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,13);
 result += "\r\n        m_CodeSpecialite = "       +  m_CodeSpecialite       + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,14);
 result += "\r\n        m_CodeZoneTarif = "        +  m_CodeZoneTarif        + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,15);
 result += "\r\n        m_CodeZoneIk = "           +  m_CodeZoneIk           + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,16);
 result += "\r\n        m_CodeAgr1 = "             +  m_CodeAgr1             + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,17);
 result += "\r\n        m_CodeAgr2 = "             +  m_CodeAgr2             + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,18);
 result += "\r\n        m_CodeAgr3 = "             +  m_CodeAgr3             + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,19);
 result += "\r\n        m_HabilitationSignFact = " +  m_HabilitationSignFact + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,20);
 result += "\r\n        m_HabilitationSignLot = "  +  m_HabilitationSignLot  + ",  " + CSesam_API_InfoGroupe::NumGroupeToString(2,21);

 if (m_CodePorteur.length())
    {result += "\r\n\r\n[CPS_CODE]";
     //result += "\r\n        m_CodePorteur = "  +  m_CodePorteur  + ", oh my god !! le code porteur ";
     result += "\r\n        m_CodePorteur = ****, oh my god !! le code porteur ";
    }
 if (m_Erreur.length())
    {result += "\r\n\r\n[ERR_CODE]";
     result += "\r\n        m_Erreur = "       +  m_Erreur       + ", horreur un message d'erreur";
    }
 return result;
}

//-------------------------------- UnSerialize --------------------------------
void C_Cps::UnSerialize(const QString &serializeData)
{     char             *pt   = (char*)(const char*) serializeData;
      //.................................. PARAMETRES ...........................................
      QString     data       = "";
      QString     var_name   = "";
      QString      section   = "";
      while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
           {if (section=="CPS_GR1")
               {int index = 1;
                while (*pt && *pt != '[')
                      {pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &data );
                       SetMember(index++, 1, data, data.length());
                      }
                }
            if (section=="CPS_GR2")
               {int index = 1;
                while (*pt && *pt != '[')
                      {pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &data );
                       SetMember(index++, 2, data, data.length());
                      }
                }
            if (section=="CPS_CODE")
               { pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &data);
                 SetCodePorteur(data);
                }
            section="";
           }
}
