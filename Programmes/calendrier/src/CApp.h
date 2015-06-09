/********************************** CApp.h ****************************************
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
#ifndef CPLUGAPP_H
#define CPLUGAPP_H

#include <qapplication.h>
#include <qstring.h>
#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli

#define NAME_APPLI "calendrier"

//............................................. quelques définitions ..........................................................


/*! \brief Plugin class for MedinTux .
 *
 * this class products a html file  \n
 * which is a pregnent calendar to give.\n
 * the first plugin parameter : argv[13 + 0]; \n
 * is the date of the first day of menstruation .
 */

//================================================= CApp ==============================================================
class CApp : public QApplication //, public CGestIni
{

   Q_OBJECT

public:

    CApp(int & argc, char ** argv);
    //int  GotoDebug();

public slots:


signals:


public:
  QString m_NameAppli;             /*!< string which contains Plugin application Name */
  QString m_PathAppli;             /*!< string which contains path of application executable  */
  //QString m_PathIni        ;       /*!< string which contains path of configuration file  */
  //QString m_PathRessources ;       /*!< string which contains path of application ressource directory  */
  QString m_PathExchg      ;       /*!< string which contains path of exchange file  */
  QString m_PathTheme;             /*!< string which contains path of icon theme  */
    //QString m_Param          ;       /*!< string where is loaded the configuration file */
};

extern CApp* G_pCApp;

#endif            // #define CPLUGAPP_H
