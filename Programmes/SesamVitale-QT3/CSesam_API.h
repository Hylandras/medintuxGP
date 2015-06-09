/********************************* CSesam_API.h ***********************************
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
// Note :
//       sous windows pour que soit cree en meme temps que la dll le .lib il faut faire
//       pr��er les d�laration de classes par : __declspec(dllexport)
// ne pas oublier de changer  implib:"Release/qfselib.lib par implib:"../bin/qfselib.lib
// dans le setting, onglet link  et cela pour les deux versions Release et Debug.

#ifndef CSESAM_API_H
#define CSESAM_API_H
#include <string.h>
#include <qstring.h>
#include <qobject.h>
#include <qprocess.h>
#include <qfile.h>
#include <qstringlist.h>
#include "../../SesamVitale-QT3/C_Cps.h"
#include "../../SesamVitale-QT3/CSesam_API_InfoGroupe.h"
#include "../../SesamVitale-QT3/C_Vitale.h"

#define TR QObject::tr
//..................................... specifites selon plateforme et OS .................................
#if   defined(Q_WS_WIN)

#elif defined(Q_WS_X11)
   #define __declspec(dllexport)
#elif defined(Q_WS_MAC)

#endif

/**
@author Roland SEVIN
*/
//================================================ CMesureTemps ================================================
#include <qdatetime.h>
class __declspec(dllexport) CMesureTemps
{
 public:
 CMesureTemps(){};

 void InitTime()
    {m_dtDeb = QDateTime::currentDateTime();
    };
 QString GetResult()
    {QDateTime     dtEnd = QDateTime::currentDateTime();
     long              s = m_dtDeb.secsTo (dtEnd);
     long              h = s/3600; s -=  h*3600;
     long              m = s/60;   s -=  m*60;
     return QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s";
    };

 protected:
  QDateTime m_dtDeb;
 };

//================================================ CSesam_API ================================================
class __declspec(dllexport) CSesam_API
{

public:
    CSesam_API(const char* codePorteur = "1234",
               const char* ressPS      = "CPS",
               const char* ressLecteur = "Log_SV",
               const char* path_galss  = "/usr/local/galss/galsvlux" ,
               const char* pathLog     = "",
               const char* dateConsult = ""
               );
    ~CSesam_API();

    QString        readConfig(unsigned short session);
    unsigned short SGD_SGD_ReserverSession(size_t Taille, unsigned short* NumeroSession);
    unsigned short SGD_SGD_LibererSession(unsigned short numeroSession);
    unsigned short SGD_SGD_AjouterGroupe(unsigned short numeroSession,  unsigned short idGroupe);
    unsigned short SGD_SGD_AjouterChamp (unsigned short numeroSession,  const char*    valeurChamp);
    unsigned short SGD_SGD_AjouterChampBin( unsigned short numeroSession,
                                            const char*    valeurChamp,
                                            size_t         tailleChamp);
    unsigned short SGD_SGD_AjouterChampFichier (unsigned short numeroSession, const char* nomFichier);
    unsigned short SGD_SGD_LireGroupeSuivant(unsigned short  numeroSession,
                                             unsigned short  iDGroupeCourant,
                                             unsigned short  occurenceGroupeCourant,
                                             unsigned short* iDGroupeSuivant,
                                             unsigned short* occurenceGroupeSuivant);
    unsigned short SGD_SGD_LireChamp(unsigned short  numeroSession,
                                     unsigned short  iDGroupe,
                                     unsigned short  occurenceGroupe,
                                     unsigned short  numeroChamp,
                                     char*           valeurChamp,
                                     size_t*         tailleMax);
    void           LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, QStringList *pQStringList=0);
    void           LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, QString  *pData);
    void           LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, C_Cps    *pC_Cps);
    void           LireEtAfficherChamps(int nb, unsigned short  numeroSession, unsigned short  iDGroupeCourant, unsigned short  occurenceGroupeCourant, C_Vitale *pC_Vitale);
    unsigned short SIS_SIS_LireConfig(unsigned short sessionConfig);
    unsigned short SSV_SSV_InitLIB();
    unsigned short SSV_SSV_TermLIB();
    unsigned short SSV_SIS_LireCartePS(const char* NomRessourcesPS,
                           const char* NomRessourceLecteur,
                           const char* CodePorteurPS,
                           unsigned short NumeroSession);
    unsigned short SSV_SIS_LireDroitsVitale(const char* nomRessourcesPS,
                                            const char* nomRessourceLecteur,
                                            const char* codePorteurPS,
                                            const char* dateConsultation,
                                            unsigned short numeroSession);

    QString        SSV_SSV_ErrCodeToString(int code);
    void           SetMember(QString        &data,
                             const QString  &label,
                             int             memberIndex,
                             unsigned short  iDGroupeCourant,
                             const char     *memberData,
                             int             occur);
    void           toLogGroupe(unsigned short  iDGroupeSuivant  , unsigned short  occurenceGroupeSuivant , QString *pData  = 0 );
    void           toLog( QString stringlog, QString *pData  = 0);

 public:
    QString  m_PathGalss;
    QString  m_CodePorteur;
    QString  m_RessPS;
    QString  m_RessLecteur;
    QString  m_DateConsult;
    QString  m_DataRead;
 private:
    char     *m_Champ;
    size_t    m_ChampTaille;
    QProcess  m_Galss;
    QFile    *m_LogFile;
};

#endif
