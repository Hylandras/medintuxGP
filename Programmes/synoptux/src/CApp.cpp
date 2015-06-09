/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �   l'Energie Atomique                                           *
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
//
//    mysqldump --quote-name --compatible=mysql323 CCAMTest | gzip > /home/ro/CCAMTest.gz
//    Les spectatrices ne veulent pas qu'on les ennuie dans la file.

#include "CApp.h"

#include <QFileInfo>
#include <QFile>

#include <QStringList>
#include <QTextStream>
#include <QIODevice>
#include <QProcess>
#include <QDateTime>
#include <QTextCodec>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "C_BaseSynoptux.h"

static CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application accessible avec CApp::pCApp()

//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{if (m_pC_BaseSynoptux)   delete m_pC_BaseSynoptux;
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
:   C_AppCore( mui_name, argc, argv )
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF8") );
    G_pCApp = 0;
    //.............................. recuperer config de la base ..........................................
    QString paramCfgData = CGestIni::Param_UpdateFromDisk(m_PathAppli + m_NameAppli+"Bases.cfg");
    C_BaseCommon::EncodePassword_InConnectionParam(m_IniParam, "Connexion");                       // on encode les password si pas deja fait

    //.............................. initialiser la base C_BaseMedica ..........................................
    m_pC_BaseSynoptux = 0;

    m_pC_BaseSynoptux =  new C_BaseSynoptux(m_IniParam, paramCfgData, this);
    if (m_pC_BaseSynoptux ==0)
       {qDebug () << tr("new m_pC_BaseSynoptux failed");
        return;
       }
    if (m_pC_BaseSynoptux->getLastError().length())
       {qDebug() << m_pC_BaseSynoptux->getLastError();
        QMessageBox::warning(0, mui_name,
                             tr("Opening datbase failed<br />%1").arg(m_pC_BaseSynoptux->getLastError()),
                             QMessageBox::Cancel
                            );
        delete m_pC_BaseSynoptux;
        m_pC_BaseSynoptux = 0;
        return;
       }
    setQuitOnLastWindowClosed(TRUE);
    G_pCApp           =    this;
}


//--------------------------------------------- pCApp -------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
*/
CApp* CApp::pCApp()
{return G_pCApp;
}


//------------------------------ loadContentsIfBeginByTokenFile --------------------------------------------------
/*! \brief Teste si la chaine arg commence par $File et si c'est le cas la charge avec le contenu du fichier.
 *  \param  QString& arg : argument a evaluer et eventuellement charger avec le contenu d'un fichier
*/
QString CApp::loadContentsIfBeginByTokenFile(QString &arg)
{if (arg.startsWith("$File"))
    {arg = resolvePath(arg.mid(5));
     arg = CGestIni::Param_UpdateFromDisk(arg);
    }
 return arg;
}

/*------------------------------ resolvePath --------------------------------------------------------------------*/
/*! \brief resoud un chemin comportant les eventuelles macros $Glossaire $Vigie $ToAbsPath $EndToAbsPath par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie absolutise si necessaire
*/
QString CApp::resolvePath(QString pathFile, const QString &pathGlossaire  /* = ""*/ )
{    pathFile = resolvePathKeyWord(pathFile, pathGlossaire);
     if ( QDir::isRelativePath ( pathFile ) ) pathFile  = QDir::cleanPath ( pathFile.prepend(m_PathAppli) );
     return pathFile;
}
/*------------------------------ resolvePathKeyWord --------------------------------------------------------------------*/
/*! \brief resoud un chemin comportant les eventuelles macros $Glossaire $Vigie $ToAbsPath $EndToAbsPath par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie absolutise que lors des mots clefs et UNIQUEMENT DANS CE CAS (utile lors arguments de programmes externes)
*/
QString CApp::resolvePathKeyWord(QString pathFile, const QString &pathGlossaire /* = "" */)
{    pathFile  = pathFile.trimmed().replace("$Glossaire", pathGlossaire);
     pathFile  = pathFile.replace("$Vigie", pathGlossaire + "Vigie");
     //if (pathFile[1]==':') pathFile  = pathFile.replace (1, 1, '|' ); //proteger le : de  C:
     //if (pathFile.startsWith("//")) pathFile = QString("**") + pathFile.mid(2);  // prot?ger les doubles "//" du debut (cas des repertoires reseaux)
     //pathFile.replace("//","/");    pathFile.replace("*","/"); pathFile.replace("|",":");
     //............. gestion des motsclefs ..........
     int pos     = -1;
     int deb     = -1;
     QString tmp = "";
     if ((pos = pathFile.lastIndexOf("$EndToAbsPath")) != -1)
        {do
          { if ( (deb     = pathFile.lastIndexOf("$ToAbsPath"),pos) != -1)
               { pathFile = pathFile.remove(pos,13);   // virer le $EndToAbsPath
                 tmp      = QDir::cleanPath (m_PathAppli + pathFile.mid(deb+10, pos - deb -10).trimmed());
                 pathFile = pathFile.replace (deb, pos - deb, tmp );
                 pos      = pathFile.lastIndexOf("$EndToAbsPath", deb);
               }
            else
               { pos = -1;
               }
          } while (pos != -1);
        }
     else if ( (pos = pathFile.indexOf("$ToAbsPath")) != -1)
        { pathFile = pathFile.left(pos) + QDir::cleanPath (m_PathAppli + pathFile.mid(pos+10).trimmed());
        }
     return pathFile;
}
