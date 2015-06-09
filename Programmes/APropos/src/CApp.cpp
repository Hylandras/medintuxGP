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
 *   Commissariat a  l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
#include <QTextCodec>

#if QT_VERSION >= 0x050000    // Qt 4.1.2, the QT_VERSION macro will expand to 0x040102.
      #include <QtWidgets/QMessageBox>
      #include <QtWidgets/QApplication>
#else
      #include <QMessageBox>
      #include <QApplication>
#endif

#include <QDebug>
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application


//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
:QApplication(argc,argv), m_NameAppli(mui_name)
{   // QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    //.............................. recuperer le numer de vesrsion du .pro ..........................................
    //                               le mettre dans un tableau static tague pour
    //                               qu'il soit rep?rable dans le binaire
    static char NUMTAB_VERSION[]     = "==##@@=="NUM_VERSION"==@@##==";    // defini dans le .pro
    //.................... recuperer path de demarrage de l'appli ...........................
    QFileInfo qfi(argv[0]);
    m_PathAppli       = CGestIni::Construct_PathBin_Module(m_NameAppli, qfi.path ());
    //.............................. initialiser le theme ..........................................
    m_PathTheme     =  "../../Themes/Default/";
    //if (CGestIni::Param_ReadParam( m_LocalParam.toLatin1(), "Theme", "Path", &m_PathTheme) != QString::null )  // zero = pas d'erreur
    //   { m_PathTheme     =  "../../Themes/Default/";                                   // valeur par defaut si pas de theme explicite
    //   }
    if (!QDir(m_PathTheme).exists())     m_PathTheme     =  "../../Themes/Default/";        // valeur par defaut
    if ( QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
    m_PathTheme = QDir::cleanPath(m_PathTheme) + "/";
    Theme::setPath(m_PathTheme);
    //........ poditionner les valeurs par defaut ..............................
    // C_DlgApropos::C_DlgApropos(QWidget *parent, const QString &pathAppli, const QString& module, const QString& description, const QString& version, const QString &pathChangements, const QString &pathapropos) :

    m_ModuleName      = m_NameAppli;
    m_Description     = tr("Gestion de l'information d'un module");
    m_Version         = NUMTAB_VERSION;
    m_Version         = m_Version.remove("@").remove("#").remove("=")+ "  Qt : " + QT_VERSION_STR;
    m_PathChangements = m_PathAppli + "Ressources/Changements.html";
    m_PathApropos     = "";
    m_PathHelp        = "";
    m_BaseVersion     = tr("sans objet");
    if ( argc==2 )         // si un seul parametre alors c'est un D&D sur l'icone ou autre demande de visualisation de fichier
       { m_PathIcone       = Theme::getPath(true) + "22x22/help.png";
         m_PathHelp        = argv[0];
       }
    //.............. recuperer si il le faut les valeurs des parametres d'appel ....................
    if (argc>1)  m_ModuleName      = CGestIni::fromMyUTF8(argv[1]);
    if (argc>2)  m_Description     = CGestIni::fromMyUTF8(argv[2]);
    if (argc>3)  m_Version         = CGestIni::fromMyUTF8(argv[3]);
    if (argc>4)  m_PathChangements = CGestIni::fromMyUTF8(argv[4]);
    if (argc>5)  m_PathIcone       = CGestIni::fromMyUTF8(argv[5]);
    if (argc>6)  m_PathHelp        = CGestIni::fromMyUTF8(argv[6]);
    if (argc>7)  m_PathApropos     = CGestIni::fromMyUTF8(argv[7]);
    if (argc>8)  m_BaseVersion     = CGestIni::fromMyUTF8(argv[8]);
    if (m_PathApropos.length()==0) m_PathApropos = m_PathAppli + "Ressources/A_propos.html";
    // qDebug()<<tr("-A- argc : ") + QString::number(argc);
    // qDebug()<<tr("-0- m_PathHelp : ") + m_PathHelp;
    if (m_PathHelp.length()==0)
       { m_PathHelp    = m_PathAppli + "../../Doc/" + m_ModuleName; //qDebug()<<tr("-1- m_PathHelp : ") + m_PathHelp;
         m_PathHelp    = QDir::cleanPath(m_PathHelp);               //qDebug()<<tr("-2- cleanPath(m_PathHelp) : ") + m_PathHelp;
         m_PathHelp    = m_PathHelp + "/index.html";                //qDebug()<<tr("-3- m_PathHelp+/index.html : ") + m_PathHelp;
       }
    if (m_PathIcone.length()==0 || !QFile::exists(m_PathIcone)) m_PathIcone = QDir::cleanPath(m_PathAppli + "../../Doc/" + m_ModuleName ) + "/" + m_ModuleName+".png";
    if (m_PathIcone.length()==0 || !QFile::exists(m_PathIcone)) m_PathIcone = m_PathAppli + "Ressources/null.png";
    setWindowIcon (QPixmap(m_PathIcone));
    G_pCApp    =    this;
}


