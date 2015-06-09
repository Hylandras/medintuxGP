
 /********************************* CApp.h ************************************
 \file CApp.h
 \brief (put some short descripion here).

     Project: MedinTux
     Copyright (C) 2008 by
     by Sevin Roland  and Data Medical Design
     E-Mail: roland-sevin@medintux.org

 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
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





#ifndef C_APP_H
#define C_APP_H

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
#else
   #define F_EXE ""
#endif

#define PROG_NAME "qmedicabase"
#define TR QObject::tr
#include "../../MedinTuxTools/CMedicaBase.h"

#ifdef SESAMVITALE_VERSION
    #include "../../SesamVitale-QT3/C_MUI_App.h"
    #include "../../SesamVitale-QT3/C_Cps.h"
    #include "../../SesamVitale-QT3/C_Vitale.h"
#else
    #include "../../MedinTuxTools/C_MUI_App.h"
#endif



//================================================= CApp ==============================================================
class CApp : public C_MUI_App
{
 public:

     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();
     void saveLastUserOn_Ini(const QString &user, const QString &signUser, const QString &password, bool passwordMustBeRecorded  = 1 );
     int AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool passwordMustBeRecorded  = 1 );
     bool IsAppStartWithUser();
     void PositionneBonDroits();
     bool IsThisDroitExist(const char *listDroits, const char *droitToFind);

public:


QString             m_PathAppli;                // chemin de l'Applicatif
//............................ pointeurs sur les objets principaux generaux Uniques ...................................................
//                             l'interdependance de ces differentes classes rend
//                             plus simple de les rendre accesible par un pointeur global
//                             Initialisé lors de la creation de ces classes qui doivent être
//                             instanciées qu'une seule fois pendant toute l'application
//                             la remise à zero se fait lors de la destruction de la classe.

//.................. les deux  objets suivants sont INDISPENSABLES à l'application ....................................................
//                   et seule une instance de ces objets doit exister par application
CMedicaBase         *m_pCMedicaBase;                // moteur de la base de donnée medicaments
QString              m_LocalParam;                  // fichier de configuration
QString              m_PathIni;                     // endroit ou trouver le fichier de configuration
};

extern CApp* G_pCApp;

#endif

