/********************************* CGetDatasempAppCtx.cpp *************************
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

#include <qapplication.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qtimer.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qdatetime.h>
#include "CGetDatasempAppCtx.h"
//-------------------------------------------- CGetDatasempAppCtx ---------------------------------------------------------------------
// un plungin est un executable communicant avec DrTux par l'intermediare du protocole suivant:
// DrTux appelle le plugin avec dans la ligne de commande:
// en :
//   0  Chemin de l'executable plugin à actionner
//   1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
//   2  Chemin de l'executable appelant
//   3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour
//                        ce fichier est retourné modifié par le plugin, ce fichier peut etre un masque html ou
//                        tout autre délire en accord avec la syntaxe du plugin dès fois que ...
//                        si ce fichier est vide, où n'existe pas le plugin retournera son résultat dans ce fichier
//                        d'échange que l'appelant récupera
//                        Le nom de ce fichier d'echange comporte : NomDuPlugin-GUID_Patient-
//   4  GUID du patient
//   5  PrimKey de l'observation en cours
//   6  PrimKey du terrain en cours
//   7  PrimKey de l'ordonnance en cours
//   8  Reservé
//   9  Reservé
//   10 Reservé
//   11 Reservé
//   12 Nb autres parametres d'appels specifiques au plugin
//   13 --> 13+Nb paramètres
//.......... QFseVitale ............................
//  if (argc>13) m_Command     = argv[13];  // nb_param = argc - 13;
//  if (argc>14) m_PathLog     = argv[14];
//  if (argc>15) m_CodePorteur = argv[15];
//  if (argc>16) m_DateConsult = argv[16];
//------------------------------------------------- CGetDatasempAppCtx ----------------------------------------------------------------
CGetDatasempAppCtx::CGetDatasempAppCtx(int & argc, char ** argv, const QString &name)
{ //............... recuperer chemin global et nom de l'appli ............................
  QFileInfo qfi(argv[0]);
  m_PathAppli             =  CGestIni::Construct_PathBin_Module("qgetdatasemp",  qfi.dirPath (true)); //qfi.dirPath (true) + QDir::separator();
  m_PathRessources        =  m_PathAppli +  "Ressources" + QDir::separator();
  m_PathLog               =  m_PathAppli +  "Trace"      + QDir::separator();
  m_PathExchg             =  m_PathLog   +  "Exchange.txt";
  m_Command               = "GetDatasempData";
  m_ParamData             = "";           // vient de CGestIni
  m_ParamPath             = "";           // vient de CGestIni

  if (name.length()) m_NameAppli = name;
  else               m_NameAppli = qfi.baseName();
  if (argc>=13)
     {  m_PathExchg    = argv[3];
        m_PathParent   = argv[2];
        m_ParamPath    = argv[1];
        m_GUID         = argv[4];
        m_IsPlugin     = 1;
     }
  else
     {  m_PathParent   = "";
        m_ParamPath    = "****";        // utiliser le .ini local
        m_GUID         = "";
        m_IsPlugin     = 0;
        if (argc>=1) m_Command      =  argv[1];
     }
  if (m_ParamPath=="****") m_ParamPath  = m_PathAppli + "qgetdatasemp.ini";  // utiliser le .ini local
  if (argc>13) m_Command     = argv[13];  // nb_param = argc - 13;
  if (argc>14) m_PathLog     = argv[14];


  //.................. recuperer parametres de connexion au serveur ...............................................
  //                   et les appliquer au  m_PathRessources + "MedicaBase.ini"
 //                   (qui servira par la suite pour acceder aux bases therapeutiques)
  CGestIni::Param_UpdateFromDisk(m_ParamPath, m_ParamData);
  QString driver, base, login, pass, host, port;
  if (CGestIni::Param_ReadParam(m_ParamData, "Connexion", "Parametres",  &driver, &base, &login, &pass, &host, &port)==0)   // recuperer les param de connexion au serveur
     {QString outParam = CGestIni::Param_UpdateFromDisk( m_PathRessources + "MedicaBase.ini");              // charger le fichier a modifier
      if (CGestIni::Param_ReadParam(outParam,   "Connexion", "Parametres MedicaBase",  &driver, &base)==0)
          CGestIni::Param_WriteParam(&outParam, "Connexion", "Parametres MedicaBase",   driver,  base,  login,  pass,  host,  port);
      if (CGestIni::Param_ReadParam(outParam,   "Connexion", "Parametres SrceBase",    &driver, &base)==0)
          CGestIni::Param_WriteParam(&outParam, "Connexion", "Parametres SrceBase",     driver,  base,  login,  pass,  host,  port);
      if (CGestIni::Param_ReadParam(outParam,   "Connexion", "Parametres Get_Base",    &driver, &base)==0)
          CGestIni::Param_WriteParam(&outParam, "Connexion", "Parametres Get_Base",     driver,  base,  login,  pass,  host,  port);
      CGestIni::Param_UpdateToDisk(   m_PathRessources + "MedicaBase.ini", outParam );                      // enregistrer le fichier modifie
     }
//......................... gérer les spécificité selon OS ....................................................
#if   defined(Q_WS_WIN)

#elif defined(Q_WS_X11)

#elif defined(Q_WS_MAC)

#endif

 return;
}

//------------------------------------------------- ~CFseVitaleAppCtx ----------------------------------------------------------------
CGetDatasempAppCtx::~CGetDatasempAppCtx()
{
}
