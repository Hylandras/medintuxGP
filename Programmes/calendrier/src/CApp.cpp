
/************************************** CApp.cpp **********************************
 *                                                                                *
 *   #include "CApp.h"                                                            *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: roland-sevin@medintux.org                                            *
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
#include <qtextcodec.h>
#include <qfileinfo.h>
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "CApp.h"

CApp* G_pCApp = 0;  /*!< string which contains pointer on global instance of application */

//-------------------------------------------- GotoDebug --------------------------------------------------
//int CApp::GotoDebug()
//{return 0;
//}

//-------------------------------------------- CApp -------------------------------------------------------
CApp::CApp(int & argc, char ** argv)
        : QApplication (  argc, argv )
{ QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
  QFileInfo qfi(argv[0]);
  m_NameAppli        = NAME_APPLI;
  m_PathAppli        = CGestIni::Construct_PathBin_Module(m_NameAppli,  qfi.dirPath (true));
  //m_PathIni         =  m_PathAppli  + m_ApliName + ".ini";         // chemin du fichier de configuration
  //m_PathRessources  =  m_PathAppli  + "Ressources/";               // repertoire es ressources
  m_PathExchg       =  ""; if (argc>=3)  m_PathExchg    = argv[3]; // chemin du fichier d'echange
  //m_Param           =  "";                                         // contiendra les parametres du .ini

  //CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param );             // charger le fichier de configuration
   //....................... systheme du theme ..............................................
   if (CGestIni::Param_ReadParam( m_PathAppli + m_NameAppli + ".ini", "Theme", "Path", &m_PathTheme) !=0 )  // zero = pas d'erreur
       { m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
       }
   if (!QDir(m_PathTheme).exists()) m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
   if (QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
   m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
   Theme::setPath(m_PathTheme);

  //............... initialiser le pointeur global de l'appli ............................
  G_pCApp = this;
}
