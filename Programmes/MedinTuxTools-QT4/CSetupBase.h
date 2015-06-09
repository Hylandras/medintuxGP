/********************************* CSetupBase.h ***********************************
 *                                                                                *
 *   #include "CSetupBase.h"                                                      *
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


#ifndef  CSETUP_BASE_H
#define  CSETUP_BASE_H

#include <qsqldatabase.h>



//===================================== CSetupBase ========================================================================
class CSetupBase
{


  //............................... METHODES .........................................................................

 public:

  //................................... configuration et connection de la base de données ..............................
   CSetupBase();
  ~CSetupBase();

  QSqlDatabase*  BaseConnect( const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                              const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                              const char* user,          // = "root"
                              const char* password,      // = ""
                              const char* hostname,      // = "localhost"
                              const char* baseName,
                              QString*    errMess=0,
                              int*        errNum=0);

  int           CreationBase(QSqlDatabase* mysqlbase, QString &nameBase, QString *errMess =0, int *errNum =0, const QString &driver="QMYSQL3", const QString &user="root", const QString &passWord="", const QString &hostName="localhost");
  int           DropBase(QSqlDatabase* mysqlbase, QString &nomBase, QString *errMess =0, int *errNum =0);
  //......................................... DATA ...............................................

};

#endif

