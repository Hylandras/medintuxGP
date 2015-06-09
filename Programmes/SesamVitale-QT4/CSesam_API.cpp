/*********************************     .cpp ***************************************
 *                                                                                *
 *                                                                                *
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
#include "CSesam_API.h"
#include "ssv.h"
#include "sgd.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "CSesam_API_InfoGroupe.h"
#include <string.h>
#include <qtimer.h>
#include <stdio.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>

//------------------------------------------ CSesam_API ---------------------------------------------------------
CSesam_API::CSesam_API(const char* codePorteur ,
                       const char* ressPS      ,
                       const char* ressLecteur ,
                       const char* path_galss  ,
                       const char* pathLog     ,
                       const char* dateConsult)
  : m_PathGalss(path_galss)  ,   m_CodePorteur(codePorteur) , m_RessPS(ressPS) ,  m_RessLecteur(ressLecteur) , m_DateConsult (dateConsult)

{
    #if   defined(Q_WS_WIN)
         m_PathGalss ="C:\\WINDOWS\\galsvw32.exe";
    #elif defined(Q_WS_X11)
         m_PathGalss = "/usr/local/galss/galsvlux";
    #elif defined(Q_WS_MAC)
         m_PathGalss = "/usr/local/galss/galsvlux";
    #endif

    if (m_DateConsult.length()<=0) m_DateConsult = QDate::currentDate().toString("yyyyMMdd");
    m_ChampTaille    = 2048;
    m_Champ          = new char[m_ChampTaille];
    m_LogFile        = 0;
    m_Galss.addArgument( path_galss );
    m_Galss.addArgument( "-r" );
    m_Galss.start();
    QString qs_pathToLog = pathLog;

    if (pathLog && pathLog[0]!=0)
       {if (qs_pathToLog.find("NO_LOGFILE")==-1)
           {QString datejournal  = QDateTime::currentDateTime().toString("-yyyyMMdd-hmmss");
            qs_pathToLog        += "Galss" + datejournal + ".log";
            m_LogFile = new QFile( qs_pathToLog );
            if ( m_LogFile && !m_LogFile->open(IO_WriteOnly))
               { delete m_LogFile;
                 m_LogFile = 0;
                 return;
               }
           }
       }
    toLog("\r\n");
    toLog( TR(" ===== CSesam_API start : path Galss : ") + path_galss + " =====");
    toLog(TR("path log :")       + qs_pathToLog);
}

//------------------------------------------ ~CSesam_API ---------------------------------------------------------
CSesam_API::~CSesam_API()
{ if (m_Champ ) delete[] m_Champ;
  if (m_LogFile)
     {  toLog(" ===== CSesam_API stop ========");
        m_LogFile->flush();
        m_LogFile->close();
        delete m_LogFile;
        m_LogFile = 0;
     }
}

//------------------------------------------ SGD_SGD_ReserverSession ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_ReserverSession(size_t taille, unsigned short* numeroSession)
{ return SGD_ReserverSession (taille, numeroSession);
}

//------------------------------------------ LireEtAfficherChamps ---------------------------------------------------------
void CSesam_API::LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, QStringList *pQStringList)
{size_t tailleMax = m_ChampTaille;
 int i=0;
 nb++;
 if (pQStringList) pQStringList->clear();
 for (i=1; i<nb; ++i)
    {/*int ret__LireChamp   = */  SGD_SGD_LireChamp(numeroSession, iDGroupeCourant, occurenceGroupeCourant, i, m_Champ, &tailleMax);
     if (pQStringList) pQStringList->append(m_Champ);
     toLog(TR("Retour LireChamp N° " + QString::number(i) + " : " ) +  CGestIni::CharStrToQStringUtf8(m_Champ).replace('\r',' ').replace('\n',' ') + " ("+ CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, i) + ")");
     tailleMax            = m_ChampTaille;
    }
}
//------------------------------------------ LireEtAfficherChamps ---------------------------------------------------------
void CSesam_API::LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, QString *pData )
{size_t tailleMax = m_ChampTaille;
 int i=0;
 nb++;
 for (i=1; i<nb; ++i)
    {/*int ret__LireChamp   = */  SGD_SGD_LireChamp(numeroSession, iDGroupeCourant, occurenceGroupeCourant, i, m_Champ, &tailleMax);
     toLog(TR("Retour LireChamp N° " + QString::number(i) + " : " ) +  CGestIni::CharStrToQStringUtf8(m_Champ).replace('\r',' ').replace('\n',' ') + " ("+ CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, i) + ")", pData);
     tailleMax            = m_ChampTaille;
    }
}
//------------------------------------------ LireEtAfficherChamps ---------------------------------------------------------
void CSesam_API::LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, C_Cps *pC_Cps)
{size_t tailleMax = m_ChampTaille;
 int i=0;
 nb++;

 for (i=1; i<nb; ++i)
    {/*int ret__LireChamp   = */  SGD_SGD_LireChamp(numeroSession, iDGroupeCourant, occurenceGroupeCourant, i, m_Champ, &tailleMax);

     toLog(TR("Retour LireChamp N° " + QString::number(i) + " : ") +  CGestIni::CharStrToQStringUtf8(m_Champ).replace('\r',' ').replace('\n',' ') + " ("+ CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, i) + ")");
     pC_Cps->SetMember(i, iDGroupeCourant, m_Champ, tailleMax);
     tailleMax            = m_ChampTaille;
    }
}

//------------------------------------------ LireEtAfficherChamps ---------------------------------------------------------
void CSesam_API::LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, C_Vitale *pC_Vitale)
{size_t tailleMax = m_ChampTaille;
 int i=0;
 nb++;

 for (i=1; i<nb; ++i)
    {/*int ret__LireChamp   = */  SGD_SGD_LireChamp(numeroSession, iDGroupeCourant, occurenceGroupeCourant, i, m_Champ, &tailleMax);

     toLog(TR("Retour LireChamp N° " + QString::number(i) + " : " ) +  CGestIni::CharStrToQStringUtf8(m_Champ).replace('\r',' ').replace('\n',' ') + " ("+ CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, i) + ")");
     pC_Vitale->SetMember(i, iDGroupeCourant, m_Champ, tailleMax, occurenceGroupeCourant);
     tailleMax            = m_ChampTaille;
    }
}

//------------------------------------------ SGD_SGD_LibererSession ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_LibererSession(unsigned short numeroSession)
{ return  SGD_LibererSession (numeroSession);
}

//------------------------------------------ SIS_SIS_LireConfig ---------------------------------------------------------
unsigned short CSesam_API::SIS_SIS_LireConfig(unsigned short sessionConfig)
{return SIS_LireConfig(sessionConfig);
}

//------------------------------------------ SSV_SSV_InitLIB ---------------------------------------------------------
unsigned short CSesam_API::SSV_SSV_InitLIB()
{return SSV_InitLIB();
}

//------------------------------------------ SGD_SGD_AjouterGroupe ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_AjouterGroupe(unsigned short numeroSession, unsigned short idGroupe)
{return SGD_AjouterGroupe(numeroSession, idGroupe);
}

//------------------------------------------ SGD_SGD_AjouterChamp ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_AjouterChamp (unsigned short numeroSession,  const char*    valeurChamp)
{return SGD_AjouterChamp (numeroSession,   valeurChamp);
}

//------------------------------------------ SGD_SGD_AjouterChampBin ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_AjouterChampBin( unsigned short numeroSession,
                                                    const char*    valeurChamp,
                                                    size_t         tailleChamp)
{return SGD_AjouterChampBin( numeroSession, valeurChamp,tailleChamp);
}


//------------------------------------------ SGD_SGD_AjouterChampBin ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_AjouterChampFichier (unsigned short numeroSession, const char*    nomFichier)
{return SGD_AjouterChampFichier (numeroSession,  nomFichier);
}

//------------------------------------------ SGD_SGD_LireGroupeSuivant ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_LireGroupeSuivant( unsigned short  numeroSession,
                                                      unsigned short  iDGroupeCourant,
                                                      unsigned short  occurenceGroupeCourant,
                                                      unsigned short* iDGroupeSuivant,
                                                      unsigned short* occurenceGroupeSuivant)
{return SGD_LireGroupeSuivant(numeroSession,iDGroupeCourant, occurenceGroupeCourant, iDGroupeSuivant, occurenceGroupeSuivant);
}
//------------------------------------------ SGD_SGD_LireChamp ---------------------------------------------------------
unsigned short CSesam_API::SGD_SGD_LireChamp( unsigned short  numeroSession,
                                              unsigned short  iDGroupe,
                                              unsigned short  occurenceGroupe,
                                              unsigned short  numeroChamp,
                                              char*           valeurChamp,
                                              size_t*         tailleMax)
{return SGD_LireChamp(numeroSession, iDGroupe, occurenceGroupe, numeroChamp, valeurChamp,tailleMax);
}

//------------------------------------------ SSV_SSV_TermLIB ---------------------------------------------------------
unsigned short CSesam_API::SSV_SSV_TermLIB()
{return SSV_TermLIB();
}


//------------------------------------------ SSV_SIS_LireCartePS ---------------------------------------------------------
unsigned short CSesam_API::SSV_SIS_LireCartePS(const char* nomRessourcesPS,
                           const char* nomRessourceLecteur,
                           const char* codePorteurPS,
                           unsigned short numeroSession)
{  return SIS_LireCartePS(nomRessourcesPS, nomRessourceLecteur,codePorteurPS, numeroSession);
}

//------------------------------------------ SSV_SIS_LireDroitsVitale ---------------------------------------------------------
unsigned short CSesam_API::SSV_SIS_LireDroitsVitale(const char* nomRessourcesPS,
                                                     const char* nomRessourceLecteur,
                                                     const char* codePorteurPS,
                                                     const char* dateConsultation,
                                                     unsigned short numeroSession)
{  return SIS_LireDroitsVitale(nomRessourcesPS, nomRessourceLecteur, codePorteurPS, dateConsultation, numeroSession);
}

//---------------------------------------- readConfig() ----------------------------------------------
QString CSesam_API::readConfig(unsigned short session)
{
             unsigned short  retour;
             unsigned short  iDGroupeSuivant         = 0;
             unsigned short  occurenceGroupeSuivant  = 0;
             unsigned short  ret__LireGroupeSuivant  = 0;
             QString  dataToReturn;


             CMesureTemps    mesureTemps;
             toLog("\r\n",                                   &dataToReturn);
             toLog(TR("_______ SIS_SIS_LireConfig _______"), &dataToReturn);
             mesureTemps.InitTime();
             retour = SIS_SIS_LireConfig(session);
             toLog(TR("Retour SIS_SIS_LireConfig : ")       + SSV_SSV_ErrCodeToString(retour), &dataToReturn);
             toLog(TR("=> Lecture SIS_SIS_LireConfig en: ") + mesureTemps.GetResult(),         &dataToReturn);
             mesureTemps.InitTime();
             while (retour==0)
             {ret__LireGroupeSuivant   =  SGD_SGD_LireGroupeSuivant(session, iDGroupeSuivant, occurenceGroupeSuivant,  &iDGroupeSuivant, &occurenceGroupeSuivant);
              if (ret__LireGroupeSuivant==0xF80B) break;
              if (iDGroupeSuivant==60)
                 { toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant , &dataToReturn);
                   LireEtAfficherChamps(3, session, iDGroupeSuivant, occurenceGroupeSuivant,    &dataToReturn);
                 }
              if (iDGroupeSuivant==61)
                 { toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant , &dataToReturn);
                   LireEtAfficherChamps(9, session, iDGroupeSuivant, occurenceGroupeSuivant,    &dataToReturn);
                 }
              if (iDGroupeSuivant==62)
                 { toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant , &dataToReturn);
                   LireEtAfficherChamps(6, session, iDGroupeSuivant, occurenceGroupeSuivant,    &dataToReturn);
                 }
              if (iDGroupeSuivant==63)
                 { toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant , &dataToReturn);
                   LireEtAfficherChamps(4, session, iDGroupeSuivant, occurenceGroupeSuivant,    &dataToReturn);
                 }
              if (iDGroupeSuivant==64)
                 { toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant , &dataToReturn);
                   LireEtAfficherChamps(3, session, iDGroupeSuivant, occurenceGroupeSuivant,    &dataToReturn);
                 }
             }
             toLog(TR("=> Lecture Groupes SIS_SIS_LireConfig en : ") + mesureTemps.GetResult(), &dataToReturn);
             return dataToReturn;
}

//-------------------------------- SetMember --------------------------------
void CSesam_API::SetMember(QString &data, const QString &label, int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, int occur)
{QString section = label + "_GR-" + QString::number(iDGroupeCourant) + "-OCC-" + QString::number(occur);
 QString member  = QString("m_Num-")  + QString::number(memberIndex);
 QString value   = memberData;
 if (data.find(section)==-1)
    {data += "\r\n["+section+"]\r\n";
     data += TR("; Groupe : ") +
             QString::number(iDGroupeCourant).rightJustify(4,'0') +
             "  (" + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant)+ ")\r\n";
    }
 CGestIni::Param_WriteParam(&data, section, member, value /*, QString::number(memberIndex)*/, CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, memberIndex));
}

//---------------------------------------- toLog() ----------------------------------------------
void CSesam_API::toLog( QString stringlog, QString *pData /* = 0 */ )
{   if (stringlog == QString::null) return;
    if (stringlog != "\r\n")    stringlog.prepend(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss  ::  "));
    else                        stringlog = " ";
    if (m_LogFile)
       {QTextStream ts( m_LogFile );
        ts << stringlog << "\r\n";
       }
     if (pData)
        {pData->append(stringlog + "\r\n");
        }
}

//------------------------------------------ toLogGroupe ---------------------------------------------------------
void CSesam_API::toLogGroupe(unsigned short  iDGroupeSuivant  , unsigned short  occurenceGroupeSuivant  , QString *pData /* = 0 */)
{ toLog(TR("...... Retour LireGroupeSuivant N° : " ) + QString::number( iDGroupeSuivant ) +  " (" + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeSuivant) + ") : Occurence : ......"  + QString::number( occurenceGroupeSuivant), pData);
}

//------------------------------------------ SSV_SSV_ErrCodeToString ---------------------------------------------------------
// zip:/home/ro/Documents/DataMedicalDesign/sesam-vitale/doc_FSV_1.40.3.2.zip/ssv-mp-003api_ssv_4.03doc_prog-fra-mai_05.pdf
QString        CSesam_API::SSV_SSV_ErrCodeToString(int code)
{switch(code)
 {

  case 1:       return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Erreur interne au Gestionnaire d'Accès au Lecteur Santé Social (GALSS)\r\n"
                               "    => Mettez fin à l'application et faites décharger le GALSS de la mémoire de l'ordinateur puis\r\n"
                               "       réinstallez-le (le manuel d?installation des FSV donne toutes les informations nécessaires à ce sujet)");
  case 2:       return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Nombre de sessions demandées trop important par rapport à la mémoire disponible.\r\n"
                               "    Voir aussi les erreurs relatives aux modules SESAM-Vitale page 202\r\n"
                               "    => Faites fermer par l'utilisateur toutes les autres applications inutilement ouvertes,\r\n"
                               "       surtout celles qui utilisent le GALSS.");
  case 4:       return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Pose d'exclusivité impossible: une autre application monopolise le canal qui permet\r\n"
                               "d'accéder à la ressource.\r\n"
                               "    Lire Configuration: Ces erreurs indiquent qu'un lecteur est inaccessible. Mais la zone\r\n"
                               "    de mémoire fournie en sortie par la fonction présente le premier groupe 60 et autant de\r\n"
                               "    groupes 61 que de lecteurs accessibles.\r\n"
                               "    => Il est recommandé de permettre au professionnel de santé de recommencer l?opération.\r\n"
                               "       Si l?erreur persiste, cas rare (c?est une application non SESAM-Vitale utilisant le lecteur qui\r\n"
                               "       s?est bloquée), fermer les applications accédant au lecteur et faites décharger le GALSS de la\r\n"
                               "       mémoire de l?ordinateur (le manuel d?installation des FSV donne toutes les informations\r\n"
                               "       nécessaires à ce sujet).");
  case 8:       return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Pose d'exclusivité impossible: une autre application a déjà posé une exclusivité via la\r\n"
                               "même fonction rendant un lecteur inaccessible.\r\n"
                               "    => Assurez-vous qu'aucune autre application n'appelle simultanément la même");
  case 16:      return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Allocation de mémoire insuffisante pour y copier le contexte de la Carte de Professionnel\r\n"
                               "  de Santé.\r\n"
                               "° Formater Factures: dans le cas d?une version des SSV antérieure à 1.22, elle indique\r\n"
                               "  que la version du Logiciel Lecteur sollicité est supérieure à 1.27 et incompatible (ce Logi-\r\n"
                               "  ciel Lecteur et la fonction qui le sollicite n'ont pas été conçus de paire).\r\n"
                               "    => Avertissez l'utilisateur qu'une autre application que le progiciel de santé sollicitant la CPS\r\n"
                               "       provoque cette erreur. Faites fermer cette application et le progiciel de santé et faites déchar-\r\n"
                               "       ger le GALSS de la mémoire de l'ordinateur (le manuel d?installation des FSV donne toutes\r\n"
                               "       les informations nécessaires à ce sujet).");
  case 64:      return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Echange abandonné par le lecteur (code retourné en cas de réinitialisation du lecteur),\r\n"
                               "  cette erreur indique qu'un lecteur est inaccessible.\r\n"
                               "° Lire Configuration: Le traitement est interrompu dès que l'une de ces erreurs est ren-\r\n"
                               "  contrée. Mais la zone de mémoire fournie en sortie par la fonction présente le premier\r\n"
                               "  groupe 60 et autant de groupe 61 que de lecteurs figurant à la suite du lecteur inacces-\r\n"
                               "  sible dans le fichier GALSS.INI. En effet, les lecteurs sont traités dans l'ordre inverse\r\n"
                               "  de celui de leur apparition dans ce fichier ; ainsi il est possible de repérer le lecteur\r\n"
                               "  à incriminer en comparant le contenu de cette zone de mémoire avec la configuration décrite\r\n"
                               "  dans le fichier GALSS.INI.\r\n"
                               "    => Avertissez l'utilisateur qu'une erreur de synchronisation des échanges entre un lecteur et le\r\n"
                               "       poste de travail est survenue, fermez l'application et faites décharger le GALSS de la\r\n"
                               "       mémoire de l'ordinateur (le manuel d?installation des FSV donne toutes les informations\r\n"
                               "       nécessaires à ce sujet).");
  case 512:     return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Erreur de protocole lors d'un échange avec le lecteur. Une mauvaise synchronisation\r\n"
                               "  des échanges entre le lecteur et le poste de travail génère cette erreur.\r\n"
                               "° Lire Configuration: Ces erreurs indiquent qu'un lecteur est inaccessible. Mais la zone\r\n"
                               "  de mémoire fournie en sortie par la fonction Lire Configuration présente le premier\r\n"
                               "  groupe 60 et autant de groupes 61 que de lecteurs accessibles.\r\n"
                               "° Formater Lot: Si une série de lots était en cours de traitement, elle est alors interrompue\r\n"
                               "  et le lot qui était fourni en entrée devra être traité comme le premier lot d'une nouvelle\r\n"
                               "  série et le code porteur devra donc être ressaisi lors du rappel de la fonction Formater\r\n"
                               "  Lot.\r\n"
                               "    => Rappelez la fonction et, en cas de persistance, demandez à l'utilisateur d'éteindre le lecteur\r\n"
                               "       puis de le remettre sous tension et d'attendre qu'il affiche son message d'attente\r\n"
                               "      \"ATTENTE ORDRE\", soit environ 20 secondes, avant de rappeler la fonction.");
  case 1024:    return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Absence de réponse du lecteur lors d'un échange. Ce code est retourné en cas de pro-\r\n"
                               "  blème de connexion ou d'incompatibilité de configuration entre le lecteur et le poste de\r\n"
                               "  travail.\r\n"
                               "° Lire Configuration: Dans ce cas, seuls les lecteurs qui ne répondent pas ne sont pas\r\n"
                               "  décrits par un groupe 61 dans la zone de mémoire fournie en sortie par la fonction; ceci\r\n"
                               "  permet de repérer les lecteurs à incriminer en comparant le contenu de cette zone de\r\n"
                               "  mémoire avec la configuration décrite dans le fichier GALSS.INI.\r\n"
                               "    => Faites vérifier successivement (le manuel d'installation et de configuration donne toutes les\r\n"
                               "       informations nécessaires à ce sujet): l'affichage du message \"ATTENTE ORDRE\" sur l'écran\r\n"
                               "       du lecteur, l'alimentation du lecteur en tension, la connexion et l'état du câble de liaison,\r\n"
                               "       l'adéquation entre : le nom de ressource passé à la fonction via son premier paramètre, le\r\n"
                               "       fichier de configuration GALSS.INI, la configuration du lecteur (adresses physiques et\r\n"
                               "       vitesse de communication).");
  case 2048:    return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Ouverture de session impossible : une session liant la ressource à une autre application\r\n"
                               "  est déjà ouverte par la même fonction.\r\n"
                               "° Lire Configuration: Ces erreurs indiquent qu'un lecteur est inaccessible. Le traitement\r\n"
                               "  est interrompu dès que l'une de ces erreurs est rencontrée. Mais la zone de mémoire\r\n"
                               "  fournie en sortie par la fonction présente autant de lecteurs figurant à la suite du lecteur\r\n"
                               "  inaccessible dans le fichier GALSS.INI. En effet, les lecteurs sont traités dans l'ordre\r\n"
                               "  inverse de celui de leur apparition dans ce fichier ; ainsi il est possible de repérer le lec-\r\n"
                               "  teur à incriminer en comparant le contenu de cette zone de mémoire avec la\r\n"
                               "  configuration décrite dans le fichier GALSS.INI.\r\n"
                               "    => Il est recommandé de rappeler la fonction 3 fois (un outil de sécurisation de messagerie peut\r\n"
                               "       accèder au lecteur).\r\n"
                               "       Assurez-vous qu'aucune application ne fait appel aux ressources d'un lecteur situé sur le\r\n"
                               "       même canal via la même fonction.");
  case 32769:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Fichier de configuration GALSS.INI non trouvé ou illisible.\r\n"
                               "    => Assurez-vous de la présence et de la lisibilité du fichier de configuration GALSS.INI dans le\r\n"
                               "       répertoire prévu, le manuel d'installation et de configuration donne toutes les informations\r\n"
                               "       nécessaires à ce sujet.");
  case 32770:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Serveur des fonctions du GALSS non trouvé. Ce cas concerne les environnements fonc-\r\n"
                               "tionnant en mode client-serveur. Dans ces environnements, le serveur du GALSS est\r\n"
                               "repérable à son fichier qui commence par «GALSV».\r\n"
                               "    => Assurez-vous de la présence et de la lisibilité de ce fichier exécutable dans le répertoire\r\n"
                               "       prévu, le manuel d'installation et de configuration donne toutes les informations nécessaires\r\n"
                               "       à ce sujet.");
  case 32771:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Soit le nombre de canaux de communication indiqué dans le fichier de configuration\r\n"
                               "  GALSS.INI (c?est à dire la variable « Nbcanaux ») est nul,\r\n"
                               "° soit les ressources sont trop nombreuses dans ce fichier pour être copiées dans la\r\n"
                               "  mémoire disponible.\r\n"
                               "    => Faites corriger le fichier GALSS.INI si besoin. Sinon faites fermer les applications inutile-\r\n"
                               "       ment ouvertes, puis rappelez la fonction. Une persistance de l'erreur indiquerait une insuffi-\r\n"
                               "       sance des ressources en mémoire vive.");
  case 32772:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Section [CANAL] ou bien ligne de données \"Protocole\" absente du fichier de configu-\r\n"
                               "ration GALSS.INI.\r\n"
                               "    => Vérifiez la déclaration du ou des canaux de communication et du protocole utilisés pour\r\n"
                               "       relier le ou les lecteur de cartes au poste de travail dans le fichier de configuration, le manuel\r\n"
                               "       d'installation et de configuration donne toutes les informations nécessaires à ce sujet.");
  case 32773:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Valeur du mot-clé \"Protocole\" (indiquée dans la section [CANAL] du fichier de configu-\r\n"
                               "ration GALSS.INI) inconnue.\r\n"
                               "    => Donnez la valeur 0 à ce mot-clé dans le fichier de configuration GALSS.INI pour indiquer que\r\n"
                               "       le protocole utilisé par le GALSS est le PSS, le manuel d'installation et de configuration\r\n"
                               "       donne toutes les informations nécessaires à ce sujet.");
  case 32774:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Section [PROTOCOLE0] ou bien ligne de données \"NomLib\" absente ou incorrecte\r\n"
                               "dans le fichier de configuration GALSS.INI, ou encore échec du chargement en\r\n"
                               "mémoire de la bibliothèque de fonctions (de l'initiateur du PSS) indiquée dans cette ligne\r\n"
                               "de données.\r\n"
                               "    => Vérifiez la déclaration du PSS dans le fichier de configuration GALSS.INI, le manuel d'instal-\r\n"
                               "       lation et de configuration donne toutes les informations nécessaires à ce sujet, et fermez\r\n"
                               "       toutes les autres applications inutilement ouvertes puis rappelez la fonction. Une persistance\r\n"
                               "       de l'erreur pourrait indiquer une insuffisance des ressources en mémoire vive.");
  case 32775:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Ligne de données \"Tcanal\" et/ou \"Index\" (de la section [CANAL]) absente ou incorrecte\r\n"
                               "dans le fichier de configuration GALSS.INI.\r\n"
                               "    => Donnez la valeur 1 au mot-clé \"TCanal\" et affectez le numéro du port de communication\r\n"
                               "      (reliant le lecteur de cartes à puce au poste de travail) au mot-clé \"Index\".");
  case 32778:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "La fonction d'initialisation du PSS signale une erreur.\r\n"
                               "    => Vérifiez les valeurs des paramètres de configuration du PSS (indiqués dans la section\r\n"
                               "       [PROTOCOLE0]), surtout les paramètres temporels de la ligne de données \"Config\", le\r\n"
                               "       manuel d'installation et de configuration donne toutes les informations nécessaires à ce\r\n"
                               "       sujet.");
  case 32780:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Soit les valeurs des lignes de données \"Tcanal\" et/ou \"Caracteristiques\" (de la section\r\n"
                               "  [CANAL] du fichier de configuration GALSS.INI) sont incorrectes,sous UNIX vérifier aussi le fichier : io_comm.ini\r\n"
                               "° soit la fonction de connexion du PSS signale une erreur.\r\n"
                               "    => Assurez-vous qu'aucune application n'utilise le port de communication reliant le lecteur au\r\n"
                               "       poste de travail et vérifiez que le mot-clé \"TCanal\" a la valeur 1 et, le mot-clé \"Caracteristi-\r\n"
                               "       ques\", la vitesse de communication du lecteur (exprimée en bauds) suivie des valeurs\r\n"
                               "       1,8,0,0, le manuel d'installation et de configuration donne toutes les informations nécessai-\r\n"
                               "       res à ce sujet.");
  case 32781:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "° Section [CONFIG] ou bien ligne de données \"NbCanaux\" absent du fichier de configu-\r\n"
                               "  ration GALSS.INI.\r\n"
                               "° Cette erreur peut survenir lorsque le fichier de configuration GALSS.INI est ouvert par\r\n"
                               "  une autre application : dans ce cas, il faut mettre fin à cette application.\r\n"
                               "    => Vérifiez la présence de cette section et de sa ligne de données dans le fichier de configura-\r\n"
                               "       tion GALSS.INI, le manuel d'installation et de configuration donne toutes les informations\r\n"
                               "       nécessaires à ce sujet.");
  case 32782:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Versions des fichiers relatifs au GALSS et au PSS incompatibles.\r\n"
                               "    => Faites vérifier l'installation et la configuration des fournitures SESAM-Vitale, particulière-\r\n"
                               "       ment des modules GALSS et PSS, le manuel d'installation et de configuration donne toutes\r\n"
                               "       les informations nécessaires à ce sujet.");
  case 32783:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Valeur de \"PAD\" manquante dans le fichier de configuration GALSS.INI ou non com-\r\n"
                               "prise entre 0 et 255.\r\n"
                               "    => Faites vérifier à l'utilisateur la configuration des fournitures SESAM-Vitale, le manuel d'ins-\r\n"
                               "       tallation et de configuration donne toutes les informations nécessaires à ce sujet, en particu-\r\n"
                               "       lier le contenu du fichier deconfiguration GALSS.INI relatif aux adresses physiques (PAD) et\r\n"
                               "       ses identifiants (NomLAD, NbAlias, NomAlias1, NomAlias2, etc.).");
  case 32784:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Valeur de \"LAD\" manquante dans le fichier de configuration GALSS.INI ou non com-\r\n"
                               "prise entre 0 et 255.\r\n"
                               "    => Faites vérifier à l'utilisateur la configuration des fournitures SESAM-Vitale, le manuel d'ins-\r\n"
                               "       tallation et de configuration donne toutes les informations nécessaires à ce sujet, en particu-\r\n"
                               "       lier le contenu du fichier deconfiguration GALSS.INI relatif aux adresses logiques (LAD) et\r\n"
                               "       ses identifiants (NomLAD, NbAlias, NomAlias1, NomAlias2, etc.).");
  case 32785:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Nom ou alias de \"LAD\" manquante dans le fichier de configuration GALSS.INI ou supérieur\r\n"
                               "à 8 caractères.\r\n"
                               "    => Faites vérifier à l'utilisateur la configuration des fournitures SESAM-Vitale, le manuel d'ins-\r\n"
                               "       tallation et de configuration donne toutes les informations nécessaires à ce sujet, en particu-\r\n"
                               "       lier le contenu du fichier deconfiguration GALSS.INI relatif aux adresses logiques (LAD) et\r\n"
                               "       ses identifiants (NomLAD, NbAlias, NomAlias1, NomAlias2, etc.).");
  case 53760:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Numéro de module à analyser invalide");
  case 53761:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "erreur d?ouverture du fichier de trace en écriture");
  case 61441:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "La Carte du Professionnel de Santé est absente du lecteur\r\n"
                              "    => Faites vérifier à l'utilisateur l'introduction et le maintien de sa carte de Professionnel de\r\n"
                              "       Santé dans la fente du lecteur qui lui est destinée et rappelez la fonction. Le manuel d'instal-\r\n"
                              "       lation et de configuration donne toutes les informations nécessaires à ces vérifications ; il\r\n"
                              "       indique notamment dans quelle fente doit être introduite chaque carte à puce. La fonction\r\n"
                              "       Lire Date Lecteur permet également de restituer la date courante du lecteur."
                              );
  case 61443:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° le code porteur présenté est erroné\r\n"
                              "° Avertissez l'utilisateur du risque de blocage de sa carte de PS168 et faites lui ressaisir son\r\n"
                              "   code porteur afin de passer à la fonction via son 3ème paramètre lors de son rappel.\r\n"
                              "   que trois codes porteurs erronés ont été successivement saisis ou fournis en entrée aux\r\n"
                              "   services SESAM-Vitale, une information inscrite sur la puce de la Carte du Professionnel de\r\n"
                              "   Santé signale que cette carte est bloquée. Dès lors, la carte est inutilisable tant que le\r\n"
                              "   Professionnel de santé ne l'a pas fait débloquer par le centre émetteur de sa carte.\r\n"
                              "   Mais ATTENTION : le nombbre de ces déblocages est limité à 10 par Carte de Professionnel de Santé.");
  case 61444:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° Carte du Professionnel de Santé est non valide ou inexploitable par le Logiciel Lecteur.\r\n"
                              "° La CPS n?a pas de DAM (Domaine d?Assurance Maladie)\r\n"
                              "    => Faites vérifier à l'utilisateur\r\n"
                              "       - le sens d'introduction de sa carte dans la fente du lecteur\r\n"
                              "       - la date courante du lecteur ( voir le manuel d'installation et de configuration)\r\n)"
                              "       En cas de persistance du message, avertissez l'utilisateur que sa carte est inutilisable tant\r\n"
                              "       qu'une intervention du centre émetteur de cette carte ne l'a pas remise en état d'usage et\r\n"
                              "       mettez fin aux traitements."
                              );
  case 61457:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° La Carte Vitale est absente.\r\n"
                              "    => Faites vérifier à l'utilisateur l'introduction et le maintien de la carte dans la fente du lecteur\r\n"
                              "       qui lui est destinée et rappelez la fonction (le manuel d'installation et de configuration donne\r\n"
                              "       toutes les informations nécessaires à ces vérifications ; il indique notamment dans quelle\r\n"
                              "       fente doit être introduite chaque carte à puce).");

  case 61460:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° Défault de communication avec la carte Vitale.\r\n"
                              "    => Faites vérifier à l'utilisateur le sens d'introduction de sa carte dans la fente du lecteur\r\n"
                              "       qui lui est destinée (le manuel d'installation et de configuration donne\r\n"
                              "       toutes les informations nécessaires à ces vérifications ; il indique notamment dans quelle\r\n"
                              "       fente doit être introduite chaque carte à puce) et rappelez la fonction. En cas de persistance\r\n"
                              "       du message, avertissez l'utilisateur que la carte Vitale de l'assuré est intilisable et mettez"
                              "       fin aux traitements");

  case 61461:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° La carte Vitale est retirée du lecteur.\r\n"
                              "    => Faites vérifier à l'utilisateur le sens d'introduction de sa carte dans la fente du lecteur\r\n"
                              "       qui lui est destinée  et rappelez la fonction (le manuel d'installation et de configuration donne\r\n"
                              "       toutes les informations nécessaires à ces vérifications ; il indique notamment dans quelle\r\n"
                              "       fente doit être introduite chaque carte à puce).");

  case 61478:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° Les 2 cartes introduites dans le lecteur sont de types incompatibles : l'une est une carte\r\n"
                              "  de test alors que l'autre est une carte réelle.\r\n"
                              "° Formater Lot: Le type de la Carte de Professionnel de Santé (CPS) introduite dans le\r\n"
                              "  lecteur est incompatible avec le type des Factures du lot : Soit ces Factures sont réelles\r\n"
                              "  alors qu'une CPS de test est introduite dans le lecteur, soit ce sont des Factures de test\r\n"
                              "  alors qu'une CPS réelle est introduite dans le lecteur.\r\n"
                              "    => Faites vérifier à l'utilisateur que les deux cartes utilisées pour constituer la Facture sont de\r\n"
                              "       même type. Le cas échéant, faites introduire ces deux cartes dans le lecteur avant de rappe-\r\n"
                              "       ler la fonction. Sinon, faites reconstituer la Facture à partir de deux cartes de même type.\r\n"
                              "       Formater Lot: Faites procéder au remplacement de la Carte de Professionnel de Santé intro-\r\n"
                              "       duite dans le lecteur par une Carte de Professionnel de Santé autorisée à signer ce lot, puis\r\n"
                              "       rappelez la fonction.");
  case 61680:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Présence simultanée de périodes de droits Mutuelles et AMC au niveau de la carte Vitale.\r\n"
                               "    => En cas de persistance, il est probable que la carte Vitale soit hors d'usage et que son utilisa-\r\n"
                               "       tion nécessite une intervention préalable de son centre émetteur.");
  case 61695:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Message du lecteur incohérent.\r\n"
                               "    => Demandez à l'utilisateur d'éteindre le lecteur puis de le remettre sous tension. Attendez que\r\n"
                               "       ce lecteur affiche \" ATTENTE ORDRE \" avant de rappeler la fonction. En cas de réinitialisa-\r\n"
                               "       tion du lecteur, il faut en attendre la fin (indiquée par l'affichage du message \" ATTENTE\r\n"
                               "       ORDRE \" sur l'écran du lecteur), soit environ 20 secondes, avant tout appel de fonction.\r\n"
                               "    => Formater Lot: La série qui était en cours de traitement est alors interrompue : le lot de cette\r\n"
                               "       série qui était fourni en entrée à l'appel ayant provoqué l'erreur devra être traité comme le 1er\r\n"
                               "       lot d'une nouvelle série et le code porteur devra donc être ressaisi lors de ce rappel de la\r\n"
                               "       fonction.");
  case 61696:   return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                       TR(     "Erreur de synchronisation dans les échanges entre le lecteur de cartes et le poste de travail.\r\n"
                               "    => Demandez �l'utilisateur d'éteindre le lecteur puis de le remettre sous tension et d'attendre\r\n"
                               "       qu'il affiche son message d'attente \"ATTENTE ORDRE\" avant de rappeler la fonction."
                               "    => Formater Lot: La série qui était en cours de traitement est alors interrompue : le lot de cette\r\n"
                               "       série qui était fourni en entrée �l'appel ayant provoqué l'erreur devra être traité comme le 1er\r\n"
                               "       lot d'une nouvelle série et le code porteur devra donc être ressaisi lors de ce rappel de la\r\n"
                               "       fonction.\r\n");
  case 61697:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Nom de ressource inexistant dans le fichier de configuration GALSS.INI ou ressources\r\n"
                              "incompatibles (appartenant à des lecteurs différents).\r\n"
                              "    => Vérifiez l'adéquation et l?existence entre le nom de ressource passé à la fonction en paramè-\r\n"
                              "       tre et le nom de ressource identifiant le lecteur requis dans le fichier de configuration\r\n"
                              "       GALSS.INI et que ces deux ressources sont rattachées au même lecteur (même PAD).\r\n"
                              "       ° Formater Lot Vérifiez l'existence des noms de ressources passés à la fonction dans le fichier de configu-\r\n"
                              "         ration GALSS.INI et que ces deux ressources sont rattachées au même lecteur (même PAD).\r\n"
                              "         Le retour de cette erreur en cours de série n'interrompt pas cette série : elle peut être pour-\r\n"
                              "         suivie en renseignant correctement les paramètres incriminés.\r\n"
                              "       Le manuel d'installation et de configuration donne toutes les informations nécessaires à ces\r\n"
                              "       vérifications et corrections ; il précise notamment l'identification des ressources dans le\r\n"
                              "       fichier GALSS.INI et les relations entre le contenu de ce fichier et la configuration du lecteur.");
  case 62976:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "La fonction Initialiser Bibliothèque n'est pas encore appelée\r\n"
                              "ou la fonction Terminer a déjà été appelée\r\n"
                              "° Appelez la fonction Initialiser Bibliothèque.");
  case 62992:
  case 62993:
  case 62994:
  case 62995:
  case 62996:
  case 62997:
  case 62998:
  case 62999:
  case 63000:
  case 63001:
  case 63002:
  case 63003:
  case 63004:
  case 63005:
  case 63006:
  case 63007:
            {  int n = code - 62992 ;
               return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Le fichier contenant les tables SSV, identifié par : ") + QString::number(n) + TR(" , est inaccessible en lecture (inexistant\r\n"
                              "ou pas de droits d'accés)\r\n"
                              "    => Ces erreurs peuvent apparaitre en phase de mise au point du progiciel de santé, mais ne\r\n"
                              "       doivent plus se produire à son utilisation.");
            }
  case 63008:
  case 63009:
  case 63010:
  case 63011:
  case 63012:
  case 63013:
  case 63014:
  case 63015:
  case 63016:
  case 63017:
  case 63018:
  case 63019:
  case 63020:
  case 63021:
  case 63022:
  case 63023:
            {  int n = code - 63008 ;
               return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "La version du fichier contenant les tables SSV, identifié par : ") + QString::number(n) + TR(" , est incompatible avec la\r\n"
                              "bibliothèque des SSV.\r\n"
                              "    =>Vérifiez la cohérence entre les versions des SSV et du fichier. Si elles sont différentes rem-\r\n"
                              "      remplacez le fichier contenant les tables par le fichier d'origine.");
            }
  case 63024:
  case 63025:
  case 63026:
  case 63027:
  case 63028:
  case 63029:
  case 63030:
  case 63031:
  case 63032:
  case 63033:
  case 63034:
  case 63035:
  case 63036:
  case 63037:
  case 63038:
  case 63039:
            {  int n = code - 63024 ;
               return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "L'intégrité du fichier contenant les tables SSV, identifié par : ") + QString::number(n) + TR(" , est incorrecte\r\n"
                              "    => Remplacez le fichier existant sur le poste de travail par le fichier dorigine");
            }
  case 63040:
  case 63041:
  case 63042:
  case 63043:
  case 63044:
  case 63045:
  case 63046:
  case 63047:
  case 63048:
  case 63049:
  case 63050:
  case 63051:
  case 63052:
  case 63053:
  case 63054:
  case 63055:
            {  int n = code - 63040 ;
               return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "La structure interne du fichier contenant les tables SSV, identifié par : ") + QString::number(n) + TR(" , est invalide\r\n"
                              "    => Remplacez le fichier existant sur le poste de travail par le fichier dorigine");
            }
  case 63056:
  case 63057:
  case 63058:
  case 63059:
  case 63060:
  case 63061:
  case 63062:
  case 63063:
  case 63064:
  case 63065:
  case 63066:
  case 63067:
  case 63068:
  case 63069:
  case 63070:
  case 63071:
            {  int n = code - 63056 ;
               return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Des tables du fichier, identifié par : ") + QString::number(n) + TR(" , n'ont pas pu être chargé en mémoire\r\n"
                              "    => Libérez de la place en mémoire.");
            }
  case 63072:
  case 63073:
  case 63074:
  case 63075:
  case 63076:
  case 63077:
  case 63078:
  case 63079:
  case 63080:
  case 63081:
  case 63082:
  case 63083:
  case 63084:
  case 63085:
  case 63086:
  case 63087:
            {  int n = code - 63072 ;
               return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Le nom du fichier contenant les tables SSV, identifié par : ") + QString::number(n) + TR(" , est invalide\r\n"
                              "    => Remplacez le fichier incriminé par le fichier d'origine (si le problème persiste contactez le\r\n"
                              "       GIE SESAM-Vitale).");
            }
  case 63088:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "La fonction Initialiser Bibliothèque est déjà appelée\r\n"
                              "    => Ces erreurs peuvent apparaitre en phase de mise au point du progiciel de santé, mais ne\r\n"
                              "       doivent plus se produire à son utilisation.\r\n"
                              "    => Appelez Terminer puis Initialiser Bibliothèque");
  case 63104:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Le chemin des tables binaires des SSV est absent du fichier sesam.ini\r\n"
                              "    => Mettre à jour le fichier sesam.ini suivant les données de lannexe  Fichier sesam.ini  du\r\n"
                              "       manuel dinstallation.");
  case 63217:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Le chemin du répertoire de travail est absent du fichier sesam.ini\r\n"
                              "   (section [commun], clé repertoiretravail)\r\n"
                              "    => Mettre à jour le fichier sesam.ini suivant les données de lannexe  Fichier sesam.ini  du\r\n"
                              "       manuel dinstallation.");
  case 63218:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Les fichiers dextension adm ne sont pas accessibles en écriture\r\n"
                              "    => Vérifier les accés en écriture pour les dits fichiers du\r\n"
                              "       manuel dinstallation.");
  /*
  case 63217:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° le fichier de configuration sesam.ini ne contient pas le chemin du répertoire de travail\r\n"
                              "   (section [commun], clé repertoiretravail)\r\n"
                              "° le fichier de configuration sesam.ini est inexistant ou il est impossible de le lire (droit\r\n"
                              "   d?accès)");
  case 63218:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° le répertoire de travail est inexistant\r\n"
                              "° les droits en écriture dans ce répertoire ne sont pas donnés\r\n"
                               "  vérifiez si le répertoire: sesam  est accessible en écriture");
 */

  /*
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  case 53760:  return TR(     "Numéro de module à analyser invalide");
  */

  case 63488:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° erreur des paramètres de sortie :\r\n"
                              "    - pointeur NULL (ValeurChamp ou TailleMax)\r\n"
                              "    - nombre de caractères insuffisant (TailleMax)");
  case 63489:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° erreur d?allocation de la zone de mémoire\r\n"
                              "°  Redimensionner Zone de mémoire : Dans ce cas, la valeur du paramètre\r\n"
                              "   NouvelleTaille est nulle ou inférieure à l?espace occupé par les données");
  case 63490:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "Un des paramètres d'entrée est non alloué ou invalide\r\n"
                              "erreur de paramètres (ex : ValeurChamp =NULL et TailleChamp > 0)");
  case 63492:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "zone de mémoire réservée saturée, il est impossible d?y ajouter un champ");
  case 63493:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "champ (ou groupe) de données non trouvé (NumeroChamp ou IdGroupe)\r\n"
                              "ou OccurrenceGroupe égale à 0");
  case 63494:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "OccurrenceGroupe non trouvée (la fonction SSV, SRT ou STS\r\n"
                              "des groupes d?identifiant IdGroupe, mais en quantité inférieure)");
  case 63495:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "° numéro de session invalide\r\n"
                              "° fonctions de constitution : zone de mémoire non réservée\r\n"
                              "° fonctions d?extraction : aucune donnée en sortie");
  case 63497:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "aucune en-tête de groupe n?a été ajoutée à la zone de mémoire");
  case 63498:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "plus de numéro de session disponible");
  case 63499:  return TR(     " Erreur N°: ") + QString::number(code) + "\r\n" +
                      TR(     "le groupe est le dernier de la zone de sortie");
  default :    return QString::number(code);
 }
}


