/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
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
 *   Commissariat à l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15. *
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
#include <qdir.h>
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qprocess.h>
#include <qeventloop.h>

#include "CApp.h"
#include "../../MedinTuxTools/cdlg_passwordget.h"
#include "../../MedinTuxTools/CGestIni.h"
#define PROG_NAME "qmedicabase"
CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::~CApp()
{if (m_pCMedicaBase)  delete m_pCMedicaBase;
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
    : C_MUI_App(mui_name,  argc, argv )
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );


 //............................ pointeurs sur les objets principaux generaux Uniques ...................................................
//                             l'interdependance de ces differentes classes rend
//                             plus simple de les rendre accesible par un pointeur global
//                             Initialisé lors de la creation de ces classes qui doivent être
//                             instanciées qu'une seule fois pendant toute l'application
//                             la remise à zero se fait lors de la destruction de la classe.

//.................. les deux  objets suivants sont INDISPENSABLES à l'application ....................................................
//                   et seule une instance de ces objets doit exister par application

m_pCMedicaBase              = 0;             // moteur de la base de donnée medicaments
//................... les objets suivants sont crees à la demande dans l'application..............................................
//                    mais seule une instance de ces objets peut exister par application
m_PathAppli                 = "";                // chemin de l'Applicatif
m_LocalParam                = "";

//......................... carte PS .............................
#ifdef COMMERCIAL_VERSION
     qDebug ("Commercial version");
#else
     qDebug ("Free version");
#endif
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    //.................... recuperer path de demarrage de l'appli ...........................
    QFileInfo qfi(argv[0]);
    m_PathAppli    =  CGestIni::Construct_PathBin_Module( PROG_NAME,  qfi.dirPath (true));
    m_PathIni      =  CGestIni::Construct_Name_File_Ini(  PROG_NAME,  qfi.dirPath (true), "");

    activateMainWidget ();
    initialize();
    m_pCMedicaBase = new CMedicaBase(m_PathAppli + PROG_NAME".ini", m_PathAppli + "Ressources/MedicaBase.cfg");
    CGestIni::Param_UpdateFromDisk(m_PathIni, m_LocalParam);
  //.................. recuperer parametres de connexion au serveur ...............................................
  //                   et les appliquer au  "MedicaBase.ini"
  //                   (qui servira par la suite pour acceder aux bases therapeutiques)

  QString driver, base, login, pass, host, port;
  if (CGestIni::Param_ReadParam(m_LocalParam, "Connexion", "Parametres",  &driver, &base, &login, &pass, &host, &port)==0)   // recuperer les param de connexion au serveur
     {
      if (CGestIni::Param_ReadParam(m_LocalParam,   "Connexion", "Parametres MedicaBase",  &driver, &base)==0)
          CGestIni::Param_WriteParam(&m_LocalParam, "Connexion", "Parametres MedicaBase",   driver,  base,  login,  pass,  host,  port);
      if (CGestIni::Param_ReadParam(m_LocalParam,   "Connexion", "Parametres SrceBase",    &driver, &base)==0)
          CGestIni::Param_WriteParam(&m_LocalParam, "Connexion", "Parametres SrceBase",     driver,  base,  login,  pass,  host,  port);
      if (CGestIni::Param_ReadParam(m_LocalParam,   "Connexion", "Parametres Get_Base",    &driver, &base)==0)
          CGestIni::Param_WriteParam(&m_LocalParam, "Connexion", "Parametres Get_Base",     driver,  base,  login,  pass,  host,  port);
      CGestIni::Param_UpdateToDisk(   m_PathIni, m_LocalParam );                      // enregistrer le fichier modifie
     }

    G_pCApp                  =    this;
}

