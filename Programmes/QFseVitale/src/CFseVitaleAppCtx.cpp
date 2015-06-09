/********************************* FseVitaleAppCtx.cpp ****************************
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
#include "CFseVitaleAppCtx.h"
#include "../../MedinTuxTools/CGestIni.h"
//-------------------------------------------- CFseVitaleAppCtx ---------------------------------------------------------------------
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
//------------------------------------------------- CFseVitaleAppCtx ----------------------------------------------------------------
CFseVitaleAppCtx::CFseVitaleAppCtx(int & argc, char ** argv, const QString &name)

{ //............... recuperer chemin global et nom de l'appli ............................
  QFileInfo qfi(argv[0]);
  m_PathAppli             =  qfi.dirPath (true) + QDir::separator();
  m_PathImages            =  m_PathAppli +  "Ressources/";
  m_PathLog               =  m_PathAppli +  "Trace/";
  m_PathExchg             =  m_PathLog   +  "Exchange.txt";
  m_Command               =  "MAKE_FSE";
  /*
  m_Command               =  "ALL_COMMAND";
  m_Display               =  QObject::tr("Execute all commands ...");
  m_PathGalss             = "/usr/local/galss/gals";
  m_CodePorteur           = "1234";
  m_RessPS                = "CPS";
  m_RessLecteur           = "Log_SV";
  m_DateConsult           = QDate::currentDate().toString("yyyyMMdd");
  m_LocalParam            = "";
  */

  if (name.length()) m_NameAppli = name;
  else               m_NameAppli = qfi.baseName();
  if (argc>=13)
     {  m_PathExchg    = argv[3];
        m_PathParent   = argv[2];
        m_PathIni      = argv[1];
        m_GUID         = argv[4];
        m_IsPlugin     = 1;
       }
    else
       {m_PathParent   = "";
        m_PathIni      = "****";        // utiliser le .ini local
        m_GUID         = "";
        m_IsPlugin     = 0;
       }
  if (m_PathIni=="****") m_PathIni  = m_PathAppli + "QFseVitale.ini";  // utiliser le .ini local
  if (argc>13) m_Command     = argv[13];  // nb_param = argc - 13;
  if (argc>14) m_PathLog     = argv[14];
  if (argc>16) m_DateConsult = argv[15];
  m_CodePorteur = "";
  //.................. recuperer parametres sesam-vitale ...............................................
  CGestIni::Param_UpdateFromDisk(m_PathIni, m_LocalParam);
  CGestIni::Param_ReadParam(m_LocalParam, "Sesam-Vitale", "ressourceLecteur",       &m_RessLecteur);
  CGestIni::Param_ReadParam(m_LocalParam, "Sesam-Vitale", "ressourcePS",            &m_RessPS);
  CGestIni::Param_ReadParam(m_LocalParam, "Sesam-Vitale", "pathGalss",              &m_PathGalss);
  CGestIni::Param_ReadParam(m_LocalParam, "Sesam-Vitale", m_Command+"_Display",     &m_Display);

  if ( QDir::isRelativePath ( m_PathGalss ) )   m_PathGalss   = QDir::cleanDirPath (m_PathGalss.prepend(m_PathAppli) );
  if ( QDir::isRelativePath ( m_PathLog ) )     m_PathLog     = QDir::cleanDirPath (m_PathLog.prepend(m_PathAppli) ) + "/";
  if ( QDir::isRelativePath ( m_PathExchg ) )   m_PathExchg   = QDir::cleanDirPath (m_PathExchg.prepend(m_PathAppli) ) + "/";
  //QMessageBox::information (0, "voila", m_PathLog, 1 );
//......................... gérer les spécificité selon OS ....................................................
#if   defined(Q_WS_WIN)
   if (QFileInfo(m_PathGalss).fileName()=="gals") m_PathGalss += "vw32.exe";
#elif defined(Q_WS_X11)
   if (QFileInfo(m_PathGalss).fileName()=="gals") m_PathGalss += "vlux";
#elif defined(Q_WS_MAC)
   if (QFileInfo(m_PathGalss).fileName()=="gals") m_PathGalss += "vosx";
#endif

 return;
}

//------------------------------------------------- ~CFseVitaleAppCtx ----------------------------------------------------------------
CFseVitaleAppCtx::~CFseVitaleAppCtx()
{
}
