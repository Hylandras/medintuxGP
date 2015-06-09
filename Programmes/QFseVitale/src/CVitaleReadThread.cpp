/********************************* CVitaleReadThread.cpp *************************
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


#include <qobject.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include <qmessagebox.h>

#include "../../MedinTuxTools/CGestIni.h"
#include "../../SesamVitale-QT3/C_CpsExt.h"
#include "../../SesamVitale-QT3/C_VitaleExt.h"
#include "../../SesamVitale-QT3/CSesam_API.h"
#include "CVitaleReadThread.h"


//------------------------------------------------ CVitaleReadThread ------------------------------------------------------
CVitaleReadThread::CVitaleReadThread(const QString &pathExchFile,
                                     const QString &cmd,
                                     const QString &pathLog,
                                     const QString &pathGalss,
                                     const QString &codePorteur,
                                     const QString &ressPS,
                                     const QString &ressLecteur,
                                     const QString &dateConsult
                                     )
    :QThread(), m_Command(cmd), m_PathLog(pathLog), m_PathGalss(pathGalss), m_CodePorteur(codePorteur),
     m_RessPS(ressPS), m_RessLecteur(ressLecteur), m_DateConsult(dateConsult),m_LastErreur("")
{ QFile::remove(pathExchFile);
  m_RetFile = new QFile( pathExchFile );
  if ( m_RetFile && !m_RetFile->open(IO_ReadWrite))
     { delete m_RetFile;
       m_RetFile = 0;
     }
  if (m_DateConsult.length()<=0) m_DateConsult = QDate::currentDate().toString("yyyyMMdd");
}

//------------------------------------------------ ~CVitaleReadThread ------------------------------------------------------
CVitaleReadThread::~CVitaleReadThread()
{if (m_RetFile)
    {  m_RetFile->close();
       delete m_RetFile;
       m_RetFile = 0;
     }
 m_Run = 0;
}
//------------------------------------------------ stop ------------------------------------------------------
void CVitaleReadThread::stop()
{if (m_RetFile)
    {  m_RetFile->close();
       delete m_RetFile;
       m_RetFile = 0;
     }
 m_Run = 0;
}
//------------------------------------------------ start -------------------------------------------------------------------
void  CVitaleReadThread::start()
{m_Run     = 1;
 m_Count   = 0;
 QThread::start(QThread::IdlePriority);
}

//------------------------------------------------ readSesamVitale ---------------------------------------------------------
void  CVitaleReadThread::readSesamVitale()
{        m_Run     = 1;
         CSesam_API sesamAPI(m_CodePorteur ,
                             m_RessPS      ,
                             m_RessLecteur ,
                             m_PathGalss  ,
                             m_PathLog ,
                             m_DateConsult );
         unsigned short session;
         //unsigned short  nbEssais                = 3+1;

         //------------------------------------------ Paramètres d'appel --------------------------------------------------------------------
         sesamAPI.toLog(TR("Command : ")            + m_Command);
         sesamAPI.toLog(TR("Ressources PS : ")      + sesamAPI.m_RessPS);
         sesamAPI.toLog(TR("Ressource Lecteur : ")  + sesamAPI.m_RessLecteur);
         sesamAPI.toLog(TR("Code Porteur PS : ")    + "****"); //sesamAPI.m_CodePorteur);
         sesamAPI.toLog(TR("DateConsult : ")        + sesamAPI.m_DateConsult);

         //------------------------------------------ Initialisation des Librairies ---------------------------------------------------------
         sesamAPI.toLog(TR("Retour SSV_InitLIB : ") + sesamAPI.SSV_SSV_ErrCodeToString(sesamAPI.SSV_SSV_InitLIB()));
         //------------------------------------------ Reserver Session ----------------------------------------------------------------------
         sesamAPI.toLog(TR("Retour SGD_ReserverSession : ")  + sesamAPI.SSV_SSV_ErrCodeToString(sesamAPI.SGD_SGD_ReserverSession(5000, &session)));
         sesamAPI.toLog(TR("Numéro de Session : ")          + QString::number(session));
         if (m_Command==TR("READ_CONFIG"))        //     READ_CONFIG ("Lecture des la carte CPS en cours.....")
            {toExchgFile(sesamAPI.readConfig(session));
            }
         if (m_Command==TR("READ_CPS"))           //     READ_CPS ("Lecture de la carte CPS en cours.....")
            {C_CpsExt cpsExt(&sesamAPI, session);
             toExchgFile(cpsExt.Serialize());
            }
         if (m_Command==TR("READ_VITALE"))        //     READ_VITALE ("Lecture de la Carte Vitale en cours.....")
            {C_VitaleExt vitaleExt(&sesamAPI, session);
             toExchgFile(vitaleExt.Serialize());
            }
        //------------------------------------------ Liberer Session ---------------------------------------------------------
        sesamAPI.toLog(TR("Retour SGD_SGD_LibererSession : ") + sesamAPI.SSV_SSV_ErrCodeToString(sesamAPI.SGD_SGD_LibererSession(session)));
        //Libérer  des Librairies
        //sesamAPI.toLog(QString().sprintf("Retour SSV_TermLIB :\n %s\n",(const char*)sesamAPI.SSV_SSV_ErrCodeToString(sesamAPI.SSV_SSV_TermLIB()));
        m_Run = 0;
}

//---------------------------------------- toExchgFile() ----------------------------------------------
void CVitaleReadThread::toExchgFile( QString stringlog)
{   if (stringlog == QString::null) return;
    if (stringlog == "\r\n")     stringlog = " ";
    if (m_RetFile)
       {
            m_RetFile->atEnd();                                     // on se positionne en fin de fichier
            m_RetFile->writeBlock(stringlog, stringlog.length());   // on ecrit la donnée
            m_RetFile->writeBlock("\r\n",2);                        // on rajoute un retour à la ligne
            m_RetFile->flush();                                     // on force la mise à jour du fichier log sur le disque
       }
}
